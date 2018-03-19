//==============================================================================================
//	FILENAME	:	ReadBlockStreamImpl.cpp
//	AUTHOR		:	Yi Luo
//	CREATION	:	07/15/2002
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#include "StdAfx.h"

#include "ReadBlockStreamImpl.h"
#include "ReadDataStreamImpl.h"
#include "Base/Defines/ExceptionImplementation.h"
#include "PDSerialization/PDSerialization/WriteBlockStream.h"

namespace MSerialization
{

ReadBlockStream * 
ConvertBinary2ReadBlockStream(MDataType::Binary * ip, MBase::MemoryContract * ipMemCon )
{
	return new ReadBlockStreamImpl(ConvertBinary2ReadDataStream(ip, ipMemCon));
}

ReadBlockStream * ConvertBinaryCollection2ReadBlockStream(MDataType::BinaryCollection * ipBinaryCol, 
														MBase::MemoryContract * ipMemCon)
{
	return new ReadBlockStreamImpl(ipBinaryCol, ipMemCon);
}


ReadBlockStream * ConvertWriteBlockStream2ReadBlockStream(
									WriteBlockStream* ip, MBase::MemoryContract * ipMemCon )
{
	return new ReadBlockStreamImpl(ip->GiveOutBinaryCollection(), ipMemCon);
}

ReadBlockStream* GetReadBlockStreamFromFile(MFileSystem::ReadableFile::Ptr& iFilePtr)
{
	return new ReadBlockStreamImpl(iFilePtr);
}
void ReadBlockStreamImpl::FinishReadBlock()
{
	Int64 lRead = static_cast<ReadDataStreamImpl*>(mpDStream)->GetCount();
	if  (  lRead < mBlockSizeStack.top() )
	{	// the current block has more data
		(static_cast<ReadDataStreamImpl*>(mpDStream))->Skip(mBlockSizeStack.top() - lRead);
	}

	// now the read pointer is located right after the end of this block

	// clear bookkeeping stuff
	static_cast<ReadDataStreamImpl*>(mpDStream)->StopCount();
	mBlockSizeStack.pop();
}

bool ReadBlockStreamImpl::MoreDataInBlock()
{
	Int64 lRead = static_cast<ReadDataStreamImpl*>(mpDStream)->GetCount();
	return ( lRead < mBlockSizeStack.top() );  // the current block has more data
}


unsigned short ReadBlockStreamImpl::ReadBlockHeader()
{
	static_cast<ReadDataStreamImpl*>(mpDStream)->VerifyType(T_BLOCK);

	short lType;
	(*mpDStream) >> lType;

	// get the size of the block
	// at first, the Int64 value
	// then the complement of lower 32bit.
	Int64 lSize = 0;
	(*this) >> lSize;
	
	Int32 ldata = 0;
	(*this) >> ldata;
	unsigned Int32 lDown = ldata;
	lDown = ~ lDown;

	// compare this two to ensure the correctness of block size
	if ( lDown != (unsigned Int32) lSize )
	{
		throw MBase::ExceptionImplementation<SerializationException>("Corrupted stream: bad block size.");
	}

	mBlockSizeStack.push(lSize);
	
	// start to count the read on the current block
	static_cast<ReadDataStreamImpl*>(mpDStream)->StartCount();

	return lType;
}

// signal the start of a list
void ReadBlockStreamImpl::StartReadList()
{
	static_cast<ReadDataStreamImpl*>(mpDStream)->VerifyType(T_LIST);

	Int32 lCnt = 0;
	(*mpDStream) >> lCnt;
	Int32 lComp = 0;
	(*mpDStream) >> lComp;

	if (lComp != ~lCnt )
	{
		throw MBase::ExceptionImplementation<SerializationException>("Corrupted stream: bad list length.");
	}

	mLeftListCnt = lCnt;
	_ASSERTE( mLeftListCnt >= 0 );
}

// check whether the list has more element for reading.
bool ReadBlockStreamImpl::MoreElement() throw()
{
	bool more = mLeftListCnt > 0 ;
	if ( more )
		mLeftListCnt --;

	return more;
}


ReadBlockStreamImpl::ReadBlockStreamImpl(MDataType::BinaryCollection * ip, MBase::MemoryContract * ipMemCon )
: mBlockSizeStack()
, mLeftListCnt(0)
{
	mpDStream = new ReadDataStreamImpl(ip, ipMemCon);
	Init();
}

ReadBlockStreamImpl::ReadBlockStreamImpl(ReadDataStream* ipDataStream)
:	mBlockSizeStack(),
	mLeftListCnt(0),
	mpDStream(ipDataStream)
{
	Init();
}

ReadBlockStreamImpl::ReadBlockStreamImpl(MFileSystem::ReadableFile::Ptr& iFilePtr)
	: mBlockSizeStack(),
	  mLeftListCnt(0),
	  mpDStream(new ReadDataStreamImpl(iFilePtr))
{
	Init();
}

ReadBlockStreamImpl::~ReadBlockStreamImpl()
{
	if ( mpDStream )
	{
		static_cast<ReadDataStreamImpl*>(mpDStream)->StopCount();
		mpDStream->Delete();
		mpDStream = 0;
	}
}


/*--------------------------------
	function for ReadDataStream
--------------------------------*/
// binary
ReadDataStream& ReadBlockStreamImpl::operator >> (MDataType::Binary& v)
{
	(*mpDStream) >> v;
	return (*this);
}

// bool and bool array
ReadDataStream& ReadBlockStreamImpl::operator >> (bool& v)
{
	(*mpDStream) >> v;
	return (*this);
}


void ReadBlockStreamImpl::Read(bool * pv, unsigned Int32 cb)
{
	mpDStream->Read(pv, cb);
}

// raw binary array
void ReadBlockStreamImpl::Read(unsigned char * pv, unsigned Int32 cb)
{
	mpDStream->Read(pv, cb);
}


// short and short array
ReadDataStream& ReadBlockStreamImpl::operator >> (short & v)
{
	(*mpDStream) >> v;
	return (*this);
}

ReadDataStream& ReadBlockStreamImpl::operator >> (unsigned short & v)
{
	(*mpDStream) >> v;
	return (*this);
}


void ReadBlockStreamImpl::Read(short * pv, unsigned Int32 cb)
{
	mpDStream->Read(pv, cb);
}

// long and long array
ReadDataStream& ReadBlockStreamImpl::operator >> (Int32& v)
{
	(*mpDStream) >> v;
	return (*this);
}

ReadDataStream& ReadBlockStreamImpl::operator >> (unsigned Int32 & v)
{
	(*mpDStream) >> v;
	return (*this);
}

void ReadBlockStreamImpl::Read(Int32* pv, unsigned Int32 cb)
{
	mpDStream->Read(pv, cb);
}

// 64-bit integer and array
ReadDataStream& ReadBlockStreamImpl::operator >> (Int64& v)
{
	(*mpDStream) >> v;
	return (*this);
}

void ReadBlockStreamImpl::Read(Int64* pv, unsigned Int32 cb)
{
	mpDStream->Read(pv, cb);
}


// float and float array
ReadDataStream& ReadBlockStreamImpl::operator >> (float& v)
{
	(*mpDStream) >> v;
	return (*this);
}

void ReadBlockStreamImpl::Read(float* pv, unsigned Int32 cb)
{
	mpDStream->Read(pv, cb);
}


// double and double array
ReadDataStream& ReadBlockStreamImpl::operator >> (double& v)
{
	(*mpDStream) >> v;
	return (*this);
}

void ReadBlockStreamImpl::Read(double * pv, unsigned Int32 cb)
{
	mpDStream->Read(pv, cb);
}

#ifndef NO_WCHAR_SUPPORT
// wide string
MBase::ReturnWString ReadBlockStreamImpl::ReadWString()
{
	return mpDStream->ReadWString();
}
#endif
MBase::ReturnString ReadBlockStreamImpl::ReadString()
{
	return mpDStream->ReadString();
}


char ReadBlockStreamImpl::ReadByte()
{
	return mpDStream->ReadByte();
}

#ifndef NO_WCHAR_SUPPORT
wchar_t ReadBlockStreamImpl::ReadWChar()
{
	return mpDStream->ReadWChar();
}
#endif

// serialization of complext data type 

// GUID and GUID array
ReadDataStream& ReadBlockStreamImpl::operator >> (GUID& v)
{
	(*mpDStream) >> v;
	return (*this);
}

void ReadBlockStreamImpl::Read(GUID * pv, unsigned Int32 cb )
{
	mpDStream->Read(pv, cb);
}


// DateTime and DateTime array
ReadDataStream& ReadBlockStreamImpl::operator >> (MDataType::DateTime& v)
{
	(*mpDStream) >> v;
	return (*this);
}

void ReadBlockStreamImpl::Read(MDataType::DateTime * pv, unsigned Int32 cb)
{
	mpDStream->Read(pv, cb);
}

// DynamicTime and array
ReadDataStream& ReadBlockStreamImpl::operator >> (MDataType::DynamicTime& v)
{
	(*mpDStream) >> v;
	return (*this);
}

void ReadBlockStreamImpl::Read(MDataType::DynamicTime * pv, unsigned Int32 cb)
{
	mpDStream->Read(pv, cb);
}


// CellFmtData and CellFmtData array
ReadDataStream& ReadBlockStreamImpl::operator >> (MDataType::CellFmtData& v)
{
	(*mpDStream) >> v;
	return (*this);
}

void ReadBlockStreamImpl::Read(MDataType::CellFmtData * pv, unsigned Int32 cb)
{
	mpDStream->Read(pv, cb);
}


// Variant and Variant array
// when reading Variant array, the caller must allocate the Variant array, 
// If the Variants must use a Buffer, each element of the Array must be
// be initialized with the Buffer prior to call Read().
ReadDataStream& ReadBlockStreamImpl::operator >> (MDataType::Variant& v)
{
	(*mpDStream) >> v;
	return (*this);
}

void ReadBlockStreamImpl::Read(MDataType::Variant * pv, unsigned Int32 cb)
{
	mpDStream->Read(pv, cb);
}


ReadDataStream& ReadBlockStreamImpl::operator >> (SerializableObject& irValue)
{
	(*mpDStream) >> irValue;
	return (*this);
}

// special function for enhanced robustness. The original value and its 
// complementary value are expected to exist in the stream, so the reader 
// can compare the consecutive data to ensure the data is good.
Int32 ReadBlockStreamImpl::ReadSafeInt()
{
	return mpDStream->ReadSafeInt();
}


// rewind the read pointer to the beginning of the stream.
void ReadBlockStreamImpl::Rewind() 
{
	mpDStream->Rewind();
	static_cast<ReadDataStreamImpl*>(mpDStream)->StartCount();	// count for the whole stram

	while ( ! mBlockSizeStack.empty() )
	{
		mBlockSizeStack.pop();
	}
	mBlockSizeStack.push(mpDStream->GetNumberOfUnreadBytes());  // number of bytes for the whole stream

	mLeftListCnt = 0;
}


// skip the spcified bytes and start the next reading from the new location.
void ReadBlockStreamImpl::Skip(Int64 iOffset)
{
	(static_cast<ReadDataStreamImpl*>(mpDStream))->Skip(iOffset);
}

void ReadBlockStreamImpl::Skip(unsigned Int32 dwOffset)
{
	(static_cast<ReadDataStreamImpl*>(mpDStream))->Skip(dwOffset);
}


// get the physical size of the stream, including all the extra information for 
// any internal purpose such as error checking, bookkeeping and padding, 
unsigned Int32 ReadBlockStreamImpl::GetSize()
{
	return 	mpDStream->GetSize();
}

Int64 ReadBlockStreamImpl::Get64BitSize() throw()
{
	return 	mpDStream->Get64BitSize();
}


// get the number of bytes unread in the stream
Int64 ReadBlockStreamImpl::GetNumberOfUnreadBytes() throw()
{
	return 	mpDStream->GetNumberOfUnreadBytes();
}

MDataType::BinaryCollection* ReadBlockStreamImpl::GiveOutBinaryCollection()
{
	while ( ! mBlockSizeStack.empty() )
	{
		mBlockSizeStack.pop();
	}

	mLeftListCnt = 0;

	return mpDStream->GiveOutBinaryCollection();
}

MDataType::BinaryCollection* ReadBlockStreamImpl::CopyOutBinaryCollection() const
{
	return mpDStream->CopyOutBinaryCollection();
}

MDataType::BinaryCollection* ReadBlockStreamImpl::CopyOutContentBinaryCollection() const
{
	return mpDStream->CopyOutContentBinaryCollection();
}


// delete this stream. This is the only way a stream can be deleted.
void ReadBlockStreamImpl::Delete() throw() 
{
	delete this;
}

void ReadBlockStreamImpl::Init()
{
	_ASSERT( mpDStream != 0 );
	static_cast<ReadDataStreamImpl*>(mpDStream)->StartCount();	// count for the whole stram
	mBlockSizeStack.push(mpDStream->GetNumberOfUnreadBytes());  // number of bytes for the whole stream
}

TYPE ReadBlockStreamImpl::GetItemType()
{
	return mpDStream->GetItemType();
}

bool ReadBlockStreamImpl::IsEncrypted()
{
	return mpDStream->IsEncrypted();
}

bool ReadBlockStreamImpl::WaitForData(unsigned Int32 iTimeOut)
{
	// This function is not supported!
	_ASSERTE(false);
	return false;
}

/*------------------------------------------
	function for AutoReadDataStreamCloser
-------------------------------------------*/


AutoReadBlockStreamCloser::AutoReadBlockStreamCloser(ReadBlockStream* ipReadStream)
: mpReadStream(ipReadStream)
{
	_ASSERT(mpReadStream);
}


AutoReadBlockStreamCloser::~AutoReadBlockStreamCloser() throw()
{
	try
{
	_ASSERT(mpReadStream);
	mpReadStream->FinishReadBlock();
	mpReadStream = NULL;
}
	catch ( ... )
	{
		// do nothing. We might already be in the middle of exception handling. Just ignore the failure.
	}
}

}
