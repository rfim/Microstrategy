//==============================================================================================
//	FILENAME	:	WriteBlockStreamImpl.cpp
//	AUTHOR		:	Yi Luo
//	CREATION	:	07/15/2002
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================

#include "StdAfx.h"

#include "Base/Defines/ExceptionImplementation.h"
#include "WriteBlockStreamImpl.h"
#include "WriteDataStreamImpl.h"
#include "Facilities.h"
#include "DataStreamHeader.h"

namespace MSerialization
{

WriteBlockStream*
CreateWriteBlockStream(MBase::MemoryContract * ipMemCon,
					   unsigned short header )
{
	_ASSERTE( header < 0x2000 );
	return new WriteBlockStreamImpl(ipMemCon, header);
}

WriteBlockStream*
CreateRawWriteBlockStream(MBase::MemoryContract * ipMemCon,
					   unsigned short header )
{
	_ASSERTE( header < 0x2000 );
#ifdef LITTLE_ENDIAN
	return new WriteBlockStreamImpl(ipMemCon, header | PDS_RAW_STREAM);
#else
	return new WriteBlockStreamImpl(ipMemCon, header | PDS_RAW_STREAM| PDS_BIG_ENDIAN_STREAM );
#endif
}

WriteBlockStream*
CreateSafeWriteBlockStream(MBase::MemoryContract * ipMemCon,
						   unsigned short header )
{
	_ASSERTE( header < 0x2000 );
	return new WriteBlockStreamImpl(ipMemCon, header | PDS_SAFE_STREAM);
}

WriteBlockStream*
CreateSafeRawWriteBlockStream(MBase::MemoryContract * ipMemCon,
						   unsigned short header )
{
	_ASSERTE( header < 0x2000 );
#ifdef LITTLE_ENDIAN
	return new WriteBlockStreamImpl(ipMemCon, header | PDS_SAFE_STREAM | PDS_RAW_STREAM);
#else
	return new WriteBlockStreamImpl(ipMemCon, header | PDS_SAFE_STREAM | PDS_RAW_STREAM| PDS_BIG_ENDIAN_STREAM );
#endif
}

WriteBlockStream* ConvertBinaries2WriteBlockStream(MDataType::BinaryCollection* ipBinaries,
													MBase::MemoryContract* ipMemCon)
{
	return new WriteBlockStreamImpl(ipBinaries, ipMemCon);
}

WriteBlockStream*
CreateWriteBlockStream(MFileSystem::WriteableFile::Ptr& iFilePtr, unsigned short header )
{
	_ASSERTE( header < 0x2000 );  // because we reserve bit above 0x2000 for special flags
	return new WriteBlockStreamImpl(iFilePtr, header);
}

WriteBlockStream*
CreateRawWriteBlockStream(MFileSystem::WriteableFile::Ptr& iFilePtr, unsigned short header )
{
	_ASSERTE( header < 0x2000 );

#ifdef LITTLE_ENDIAN
	return new WriteBlockStreamImpl(iFilePtr, header | PDS_RAW_STREAM );
#else
	return new WriteBlockStreamImpl(iFilePtr, header | PDS_RAW_STREAM | PDS_BIG_ENDIAN_STREAM );
#endif
}

WriteBlockStream*
CreateSafeWriteBlockStream(MFileSystem::WriteableFile::Ptr& iFilePtr, unsigned short header )
{
	_ASSERTE( header < 0x2000 );
	return new WriteBlockStreamImpl(iFilePtr, header | PDS_SAFE_STREAM );
}

WriteBlockStream*
CreateSafeRawWriteBlockStream(MFileSystem::WriteableFile::Ptr& iFilePtr, unsigned short header )
{
	_ASSERTE( header < 0x2000 );

#ifdef LITTLE_ENDIAN
	return new WriteBlockStreamImpl(iFilePtr, header | PDS_SAFE_STREAM | PDS_RAW_STREAM);
#else
	return new WriteBlockStreamImpl(iFilePtr, header | PDS_SAFE_STREAM | PDS_RAW_STREAM | PDS_BIG_ENDIAN_STREAM );
#endif
}


WriteBlockStream* ConvertFile2WriteBlockStream(MFileSystem::UpdateableFile::Ptr& iFilePtr, unsigned short header )
{
	return new WriteBlockStreamImpl(iFilePtr, header);
}

#ifndef __ANDROID__
WriteBlockStream*
CreatePartialEncryptedSafeWriteBlockStream(MBase::MemoryContract * ipMemCon,
						   unsigned short header,
						   MEncryptor::EncryptionVersion iEncryptionVersion,
						   const char* ipcKey)
{
	_ASSERTE( header < 0x2000 );
	switch (iEncryptionVersion)
	{
	case MEncryptor::SIMPLE:
		return new WriteBlockStreamImpl(ipMemCon, header | PDS_SAFE_STREAM | PDS_SIMPLE_ENCRYPT_STREAM, ipcKey);
	case MEncryptor::RC4:
		return new WriteBlockStreamImpl(ipMemCon, header | PDS_SAFE_STREAM | PDS_RC4_ENCRYPT_STREAM, ipcKey);
	case MEncryptor::TEA:
		return new WriteBlockStreamImpl(ipMemCon, header | PDS_SAFE_STREAM | PDS_TEA_ENCRYPT_STREAM, ipcKey);
	case MEncryptor::AES:
		return new WriteBlockStreamImpl(ipMemCon, header | PDS_SAFE_STREAM | PDS_AES_ENCRYPT_STREAM, ipcKey);
	default:
		_ASSERTE(false);
		return NULL;
	}
}

WriteBlockStream*
CreatePartialEncryptedSafeWriteBlockStream(MFileSystem::WriteableFile::Ptr& iFilePtr,
						   unsigned short header,
						   MEncryptor::EncryptionVersion iEncryptionVersion,
						   const char* ipcKey)
{
	_ASSERTE( header < 0x2000 );
	switch (iEncryptionVersion)
	{
	case MEncryptor::SIMPLE:
		return new WriteBlockStreamImpl(iFilePtr, header | PDS_SAFE_STREAM | PDS_SIMPLE_ENCRYPT_STREAM, ipcKey);
	case MEncryptor::RC4:
		return new WriteBlockStreamImpl(iFilePtr, header | PDS_SAFE_STREAM | PDS_RC4_ENCRYPT_STREAM, ipcKey);
	case MEncryptor::TEA:
		return new WriteBlockStreamImpl(iFilePtr, header | PDS_SAFE_STREAM | PDS_TEA_ENCRYPT_STREAM, ipcKey);
	case MEncryptor::AES:
		return new WriteBlockStreamImpl(iFilePtr, header | PDS_SAFE_STREAM | PDS_AES_ENCRYPT_STREAM, ipcKey);
	default:
		_ASSERTE(false);
		return NULL;
	}
}
#endif

/*-----------------
   public
--------------------*/

void WriteBlockStreamImpl::WriteBlockHeader(unsigned short blockType)
{
	_ASSERTE( mpPDStreamStack );
	_ASSERTE( mBlockPtrStack.size() == mBlockSizeStack.size() -1 );
	_ASSERTE( mBlockPtrStack.size() == mBlockTypeStack.size() );
	_ASSERTE( mBlockPtrStack.size() == mBlockPtr32Stack.size() );


	mpPDStreamStack->WriteType(T_BLOCK);

	short lType = blockType;
	(*mpPDStreamStack) << lType;
	mBlockTypeStack.push(lType);

	mBlockPtrStack.push(mpPDStreamStack->ReserveInt64());
	mBlockPtr32Stack.push(mpPDStreamStack->ReserveInt32());
	mBlockSizeStack.push(mpPDStreamStack->Get64BitSize());

	IncreaseListCount();
}

void WriteBlockStreamImpl::FinishWriteBlock(unsigned short iBlockType)
{

	if ( mBlockTypeStack.empty() )
	{
		_ASSERT(false);
		throw  MBase::ExceptionImplementation<WriteStreamException>("No open block. Cannot close block!");
	}

	if ( mBlockTypeStack.top() != iBlockType )
	{
		_ASSERT(false);
		throw  MBase::ExceptionImplementation<WriteStreamException>("Wrong block type.!");
	}


	FinishWriteBlock();

}

void WriteBlockStreamImpl::FinishWriteBlock()
{
	_ASSERTE( mBlockPtrStack.size() == mBlockSizeStack.size() -1 );
	_ASSERTE( mBlockPtrStack.size() == mBlockTypeStack.size() );
	_ASSERTE( mBlockPtrStack.size() == mBlockPtr32Stack.size() );


	if ( mBlockPtrStack.empty() )
	{
		_ASSERT(false);
		throw  MBase::ExceptionImplementation<WriteStreamException>("No open block. Cannot close block!");
	}

	// gzhou 10/22/03: Moved from FinishWriteBlockEx(), otherwise, the Object Server is broken.
	mBlockTypeStack.pop();

	// at first, get the length of the nested stream
	Int64 lSize = mpPDStreamStack->Get64BitSize() - mBlockSizeStack.top();
	mBlockPtrStack.top()->Write(lSize);
	mBlockPtrStack.top()->Delete();
	mBlockPtrStack.pop();

	// write the complementary of lower 32bit of block size
	// for robustness
	unsigned Int32 lSize32 = (unsigned Int32) lSize;
	lSize32 = ~lSize32;
	mBlockPtr32Stack.top()->Write(lSize32);
	mBlockPtr32Stack.top()->Delete();
	mBlockPtr32Stack.pop();

	mBlockSizeStack.pop();
}

// Start a new list
void WriteBlockStreamImpl::StartList()
{
	if ( mContinueList )
	{
		_ASSERT(false);
		// we don't supported nested list, so throw
		throw  MBase::ExceptionImplementation<WriteStreamException>("Open list exists. Cannot start list!");
	}

	mpPDStreamStack->WriteType(T_LIST);

	mpListCount = mpPDStreamStack->ReserveInt32();
	mpListCount->Write(0);
	mpCompListCount = mpPDStreamStack->ReserveInt32();
	mpCompListCount->Write(~ 0);
	mListCount = 0;
	mContinueList = true;
}

// continue a new list
void WriteBlockStreamImpl::ContinueList()
{
	if ( mpListCount == 0 )
	{
		_ASSERT(false);
		throw  MBase::ExceptionImplementation<WriteStreamException>("No open list. Cannot continue list!");
	}

	mContinueList = true;
}


WriteBlockStreamImpl::WriteBlockStreamImpl(MBase::MemoryContract * ipMemCon, unsigned short header, const char* ipcKey)
: mBlockPtrStack()
, mBlockPtr32Stack()
, mpPDStreamStack(0)
, mBlockSizeStack()
, mHeader(header)
, mpMemCon( ipMemCon)
, mpListCount(0)
, mpCompListCount(0)
, mListCount(0)
, mContinueList(false)
, mBlockTypeStack()
{
	if (header & (PDS_RC4_ENCRYPT_STREAM |PDS_TEA_ENCRYPT_STREAM |PDS_AES_ENCRYPT_STREAM))
		mEncryptKey = ipcKey ? ipcKey : "";
	mpPDStreamStack = new WriteDataStreamImpl(true, 512, ipMemCon, mHeader, mEncryptKey.c_str()) ;
	_ASSERTE(mpPDStreamStack);
	mBlockSizeStack.push(0);
}

WriteBlockStreamImpl::WriteBlockStreamImpl(MFileSystem::WriteableFile::Ptr& iFilePtr, unsigned short iHeader, const char* ipcKey)
: mBlockPtrStack()
, mBlockPtr32Stack()
, mpPDStreamStack(0)
, mBlockSizeStack()
, mHeader(iHeader)
, mpMemCon(0)
, mpListCount(0)
, mpCompListCount(0)
, mListCount(0)
, mContinueList(false)
, mBlockTypeStack()
{
	if (iHeader & (PDS_RC4_ENCRYPT_STREAM |PDS_TEA_ENCRYPT_STREAM |PDS_AES_ENCRYPT_STREAM))
		mEncryptKey = ipcKey ? ipcKey : "";
	mpPDStreamStack = new WriteDataStreamImpl(true, 512, iFilePtr, mHeader, mEncryptKey.c_str()) ;
	_ASSERTE(mpPDStreamStack);
	mBlockSizeStack.push(0);
}

WriteBlockStreamImpl::WriteBlockStreamImpl(MFileSystem::UpdateableFile::Ptr& iFilePtr, unsigned short iHeader)
: mBlockPtrStack()
, mBlockPtr32Stack()
, mpPDStreamStack(0)
, mBlockSizeStack()
, mHeader(iHeader)
, mpMemCon(0)
, mpListCount(0)
, mpCompListCount(0)
, mListCount(0)
, mContinueList(false)
, mBlockTypeStack()
{
	if ( iFilePtr->GetSize() > 0 )
	{
		// existing file. Append. Not erase.
		// mHeader = lHeader.GetHeaderValue(); we should have read this. Skipped for now since we control the TableArchive code.

		iFilePtr->SetPositionToEnd();
		MFileSystem::WriteableFile::Ptr lpWriteFile ( iFilePtr.GiveUp() );
		mpPDStreamStack = new WriteDataStreamImpl(false, 512, lpWriteFile , mHeader) ;
		_ASSERTE(mpPDStreamStack);
		mBlockSizeStack.push(0);
	}
	else
	{
		MFileSystem::WriteableFile::Ptr lpWriteFile ( iFilePtr.GiveUp() );
		mpPDStreamStack = new WriteDataStreamImpl(true, 512, lpWriteFile , mHeader) ;
		_ASSERTE(mpPDStreamStack);
		mBlockSizeStack.push(0);
	}

}


WriteBlockStreamImpl::WriteBlockStreamImpl(MDataType::BinaryCollection* ipBinaries,
						MBase::MemoryContract * ipMemCon)
: mBlockPtrStack()
, mBlockPtr32Stack()
, mpPDStreamStack(0)
, mBlockSizeStack()
, mpMemCon( ipMemCon)
, mpListCount(0)
, mpCompListCount(0)
, mListCount(0)
, mContinueList(false)
, mBlockTypeStack()
{

	DataStreamHeader lHeader;
	unsigned char* lpMemory = (unsigned char*) ipBinaries->GetBinary(0)->c_ptr();
	lHeader.Load(lpMemory);

	mHeader = lHeader.GetHeaderValue();

	mpPDStreamStack =  new WriteDataStreamImpl(ipBinaries, 512, ipMemCon) ;
	_ASSERTE(mpPDStreamStack);
	mBlockSizeStack.push(0);
}

WriteBlockStreamImpl::~WriteBlockStreamImpl() throw()
{
	while (!mBlockPtrStack.empty())
	{
		if (mBlockPtrStack.top())
		{
			mBlockPtrStack.top()->Delete();
		}

		mBlockPtrStack.pop();
	}

	while (!mBlockPtr32Stack.empty())
	{
		if (mBlockPtr32Stack.top())
		{
			mBlockPtr32Stack.top()->Delete();
		}

		mBlockPtr32Stack.pop();
	}

	if (mpPDStreamStack)
	{
		mpPDStreamStack->Delete();
	}

}

/*------------------------------------
	function for WriteDataStream
---------------------------------------*/

void WriteBlockStreamImpl::SafeWrite(Int32 v)
{
	mpPDStreamStack->SafeWrite(v);
	IncreaseListCount();
}

// write binary data
WriteDataStream& WriteBlockStreamImpl::operator <<(const MDataType::Binary&pv)
{
	(*mpPDStreamStack) << pv;
	IncreaseListCount();
	return (*this);
}
#ifndef NO_WCHAR_SUPPORT
// write Unicode string
WriteDataStream& WriteBlockStreamImpl::operator<<(const wchar_t * pv)
{
	(*mpPDStreamStack) << pv;
	IncreaseListCount();
	return (*this);
}
#endif
// write char string
WriteDataStream& WriteBlockStreamImpl::operator<<(const char * pv)
{
	(*mpPDStreamStack) << pv;
	IncreaseListCount();
	return (*this);
}


void WriteBlockStreamImpl::WriteByte(char iChar)
{
	mpPDStreamStack->WriteByte(iChar);
	IncreaseListCount();
}
#ifndef NO_WCHAR_SUPPORT
void WriteBlockStreamImpl::WriteWChar(const wchar_t iChar)
{
	mpPDStreamStack->WriteWChar(iChar);
	IncreaseListCount();
}
#endif

WriteDataStream& WriteBlockStreamImpl::operator<<(float pv)
{
	(*mpPDStreamStack) << pv;
	IncreaseListCount();
	return (*this);
}

WriteDataStream& WriteBlockStreamImpl::operator<<(double pv)
{
	(*mpPDStreamStack) << pv;
	IncreaseListCount();
	return (*this);
}

WriteDataStream& WriteBlockStreamImpl::operator<<(bool pv)
{
	(*mpPDStreamStack) << pv;
	IncreaseListCount();
	return (*this);
}

WriteDataStream& WriteBlockStreamImpl::operator<<(short pv)
{
	(*mpPDStreamStack) << pv;
	IncreaseListCount();
	return (*this);
}

WriteDataStream& WriteBlockStreamImpl::operator<<(unsigned short pv)
{
	(*mpPDStreamStack) << pv;
	IncreaseListCount();
	return (*this);
}


WriteDataStream& WriteBlockStreamImpl::operator<<(Int32 pv)
{
	(*mpPDStreamStack) << pv;
	IncreaseListCount();
	return (*this);
}

WriteDataStream& WriteBlockStreamImpl::operator<<(unsigned Int32 pv)
{
	(*mpPDStreamStack) << pv;
	IncreaseListCount();
	return (*this);
}

WriteDataStream& WriteBlockStreamImpl::operator<<(Int64 pv)
{
	(*mpPDStreamStack) << pv;
	IncreaseListCount();
	return (*this);
}

WriteDataStream& WriteBlockStreamImpl::operator<<(const GUID& pv)
{
	(*mpPDStreamStack) << pv;
	IncreaseListCount();
	return (*this);
}

WriteDataStream& WriteBlockStreamImpl::operator<<(const MDataType::DateTime & pv)
{
	(*mpPDStreamStack) << pv;
	IncreaseListCount();
	return (*this);
}

WriteDataStream& WriteBlockStreamImpl::operator<<(const MDataType::DynamicTime & pv)
{
	(*mpPDStreamStack) << pv;
	IncreaseListCount();
	return (*this);
}

WriteDataStream& WriteBlockStreamImpl::operator<<(const MDataType::CellFmtData & pv)
{
	(*mpPDStreamStack) << pv;
	IncreaseListCount();
	return (*this);
}


WriteDataStream& WriteBlockStreamImpl::operator<<(const MDataType::Variant & pv)
{
	(*mpPDStreamStack) << pv;
	IncreaseListCount();
	return (*this);
}

WriteDataStream& WriteBlockStreamImpl::operator<<(const SerializableObject& irObject)
{
	(*mpPDStreamStack) << irObject;
	IncreaseListCount();
	return (*this);
}

void WriteBlockStreamImpl::Write(float const *pv, unsigned Int32 cb)
{
	mpPDStreamStack->Write(pv, cb);
	IncreaseListCount();
}

void WriteBlockStreamImpl::Write(double const *pv, unsigned Int32 cb)
{
	mpPDStreamStack->Write(pv, cb);
	IncreaseListCount();
}

void WriteBlockStreamImpl::Write(bool const *pv, unsigned Int32 cb)
{
	mpPDStreamStack->Write(pv, cb);
	IncreaseListCount();
}

void WriteBlockStreamImpl::Write(unsigned char const *pv, unsigned Int32 cb)
{
	mpPDStreamStack->Write(pv, cb);
	IncreaseListCount();
}

void WriteBlockStreamImpl::Write(short const *pv, unsigned Int32 cb)
{
	mpPDStreamStack->Write(pv, cb);
	IncreaseListCount();
}

void WriteBlockStreamImpl::Write(Int32 const *pv, unsigned Int32 cb)
{
	mpPDStreamStack->Write(pv, cb);
	IncreaseListCount();
}

void WriteBlockStreamImpl::Write(Int64 const * pv, unsigned Int32 cb)
{
	mpPDStreamStack->Write(pv, cb);
	IncreaseListCount();
}

void WriteBlockStreamImpl::Write(GUID const *pv, unsigned Int32 cb)
{
	mpPDStreamStack->Write(pv, cb);
	IncreaseListCount();
}

void WriteBlockStreamImpl::Write(MDataType::DateTime const *pv, unsigned Int32 cb)
{
	mpPDStreamStack->Write(pv, cb);
	IncreaseListCount();
}

void WriteBlockStreamImpl::Write(MDataType::DynamicTime const *pv, unsigned Int32 cb)
{
	mpPDStreamStack->Write(pv, cb);
	IncreaseListCount();
}

void WriteBlockStreamImpl::Write(MDataType::CellFmtData const *pv, unsigned Int32 cb)
{
	mpPDStreamStack->Write(pv, cb);
	IncreaseListCount();
}


void WriteBlockStreamImpl::Write(MDataType::Variant const *pv, unsigned Int32 cb)
{
	mpPDStreamStack->Write(pv, cb);
	IncreaseListCount();
}


/* reserve a space of 64 bit integer in the stream, so the caller can fill the space with special value, such as the length of a block. We reserve 64-bit integer because we want to ensure the block can be larger than 4G.
*/
StreamInt64Reserve* WriteBlockStreamImpl::ReserveInt64()
{
	IncreaseListCount();
	return mpPDStreamStack->ReserveInt64();
}

StreamInt32Reserve* WriteBlockStreamImpl::ReserveInt32()
{
	IncreaseListCount();
	return mpPDStreamStack->ReserveInt32();
}

// return the binary data containing the stream, so other component can
// save it to file or send it over network.
// This call gives up the ownership of the data.
MDataType::Binary* WriteBlockStreamImpl::GiveOutBinary()
{
	if ( ! mBlockPtrStack.empty() )
	{
		_ASSERT(false);
		throw MBase::ExceptionImplementation<WriteStreamException>("Block still open. Cannot get binary data.");
	}

	MDataType::BinaryPtr lResult(mpPDStreamStack->GiveOutBinary());
	Rewind();
	return lResult.GiveUp();
}

/* return the binary data containing the stream as it is.
	This will NOT cause the underlying binary data to be copied and compacted.
*/
MDataType::BinaryCollection* WriteBlockStreamImpl::GiveOutBinaryCollection()
{
	if ( ! mBlockPtrStack.empty() )
	{
		_ASSERT(false);
		throw MBase::ExceptionImplementation<WriteStreamException>("Block still open. Cannot get binary data.");
	}

	MDataType::BinaryCollectionPtr lResult(mpPDStreamStack->GiveOutBinaryCollection());
	Rewind();

	return lResult.GiveUp();
}


/* return the binary data containing the stream as it is.
	This will NOT cause the underlying binary data to be copied and compacted.
*/
MDataType::BinaryCollection* WriteBlockStreamImpl::CopyOutBinaryCollection()
{
	if ( ! mBlockPtrStack.empty() )
	{
		_ASSERT(false);
		throw MBase::ExceptionImplementation<WriteStreamException>("Block still open. Cannot get binary data.");
	}

	return mpPDStreamStack->CopyOutBinaryCollection();
}


Int64 WriteBlockStreamImpl::Get64BitSize() throw()
{
	Int64 lRes = mpPDStreamStack->Get64BitSize() ;
	return lRes;
}

unsigned Int32 WriteBlockStreamImpl::GetSize()
{
	Int64 lRes = Get64BitSize();
	if ( MoreThan32Bit(lRes))
	{
		throw MBase::ExceptionImplementation<WriteStreamException>("Size is bigger than 4G (32bit).");
	}

	return (unsigned Int32) lRes;
}

StreamType WriteBlockStreamImpl::GetStreamType()
{
	return mpPDStreamStack->GetStreamType();
}

MFileSystem::WriteableFile::ReturnPtr WriteBlockStreamImpl::ReleaseFilePtr()
{
	return mpPDStreamStack->ReleaseFilePtr();
}

void WriteBlockStreamImpl::Rewind()
{
	while ( ! mBlockPtrStack.empty() )
	{
		mBlockPtrStack.top()->Delete();
		mBlockPtrStack.pop();
	}

	while ( ! mBlockPtr32Stack.empty() )
	{
		mBlockPtr32Stack.top()->Delete();
		mBlockPtr32Stack.pop();
	}

	StreamType lStreamType = GetStreamType();
	MFileSystem::WriteableFile::Ptr lFilePtr;
	if(lStreamType == ST_FILE)
	{
		MFileSystem::WriteableFile::Ptr llFilePtr(mpPDStreamStack->ReleaseFilePtr());
		lFilePtr.Acquire(llFilePtr);
	}

	mpPDStreamStack->Delete();
	mpPDStreamStack = 0;

	while ( ! mBlockSizeStack.empty() )
	{
		mBlockSizeStack.pop();
	}

	while ( ! mBlockTypeStack.empty() )
	{
		mBlockTypeStack.pop();
	}

	mpListCount = 0;
	mpCompListCount = 0;
	mListCount = 0;
	mContinueList = false;

	if(lStreamType == ST_FILE)
	{
		mpPDStreamStack = new WriteDataStreamImpl(true, 512, lFilePtr, mHeader, mEncryptKey.c_str()) ;
	}
	else
	{
		mpPDStreamStack = new WriteDataStreamImpl(true, 512, mpMemCon.Get(), mHeader, mEncryptKey.c_str()) ;
	}
	_ASSERTE(mpPDStreamStack);
	mBlockSizeStack.push(0);
}

void WriteBlockStreamImpl::CopyAppend(const ReadDataStream& irReadStream)
{
	mpPDStreamStack->CopyAppend(irReadStream);
	IncreaseListCount();
}

void WriteBlockStreamImpl::Delete() throw()
{
	delete this;
}

void WriteBlockStreamImpl::CommitStream()
{
	// This is only supported in shared memory data streams.
	_ASSERTE(false);
}

void WriteBlockStreamImpl::Abandon()
{
	mpPDStreamStack->Abandon();
}
/*-------------------------
	private functions
---------------------------*/
void WriteBlockStreamImpl::IncreaseListCount()
{
	if ( ! mContinueList )
	{
		mpListCount = 0;
		mpCompListCount = 0;
		mListCount = 0;
	}
	else
	{
		_ASSERTE(mpListCount);
		mListCount ++;
		mpListCount->Write(mListCount);
		mpCompListCount->Write(~mListCount);

		mContinueList = false;
	}

}
#ifndef NO_WCHAR_SUPPORT
// write Unicode string
void WriteBlockStreamImpl::WriteString(const wchar_t * pv)
{
	mpPDStreamStack->WriteString(pv);
	IncreaseListCount();
}
#endif
}
