//==============================================================================================
//	FILENAME	:	ReadByteStreamImpl.cpp
//	AUTHOR		:	Yi Luo
//	CREATION	:	07/12/2002
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================

#include "StdAfx.h"

#include "ReadByteStreamImpl.h"
#include "Base/Defines/ExceptionImplementation.h"
#include "Facilities.h"
#include "PDCHeader/PDCmemory.h"
#include "FileSystem/FileSystem/Path.h"

namespace MSerialization
{

/*----------------
public:
---------------*/

ReadByteStreamImpl::ReadByteStreamImpl(MDataType::Binary* ipB, bool iOwnBinary)
: mOwnBinary(iOwnBinary)
, mCountStack()
, mpNextRead(0)
, mLeftBytes(0)
, mStreamType(ST_MEMORY)
{
	_ASSERTE(ipB);

	mpBinCol = MDataType::CreateBinaryCollection();
	mpBinCol->Insert(ipB);

	Init();
}

ReadByteStreamImpl::ReadByteStreamImpl(MDataType::BinaryCollection* ipB, bool iOwnBinary)
: mpBinCol( ipB)
, mOwnBinary(iOwnBinary)
, mCountStack()
, mpNextRead(0)
, mLeftBytes(0)
, mStreamType(ST_MEMORY)
{
	Init();
}

ReadByteStreamImpl::ReadByteStreamImpl(MDataType::BinaryCollection* ipCollection,
									   MultiReadStream* ipStream)
 :  mpBinCol(ipCollection),
	mOwnBinary(false),	// The binary is owned by the multi-read stream, not this object
	mCountStack(),
	mpNextRead(0),
	mLeftBytes(0),
    	mMultiReadStreamPtr(ipStream),
	mStreamType(ST_MEMORY)
{
	_ASSERT(ipStream != NULL);
	_ASSERT(ipCollection != NULL);
	Init();
}

ReadByteStreamImpl::ReadByteStreamImpl(MFileSystem::ReadableFile::Ptr& iFilePtr)
	: mOwnBinary(false),
	  mCountStack(),
	  mpNextRead(0),
	  mLeftBytes(0),
	  mStreamType(ST_FILE)
{
	mFilePtr.Acquire(iFilePtr);

#ifdef WIN32
	MFileSystem::ReadableFile::ReadSmartLock_NoWait *lpFileLock = new MFileSystem::ReadableFile::ReadSmartLock_NoWait(*mFilePtr);

	if(lpFileLock->IsLocked())
	{
		mFileLockPtr.Acquire(lpFileLock);
	}
	else
	{
		std::wstring lMessage = L"ReadByteStreamImpl:: Cannot acquire read lock for file:: ";
		lMessage += MFileSystem::Path::ToAbsolute(mFilePtr->GetPath()).c_str();
		throw MBase::ExceptionImpl<ReadStreamException>(lMessage.c_str());
	}
#endif

	Init();
}

ReadByteStreamImpl::~ReadByteStreamImpl() throw()
{
	try
	{
		if ( mOwnBinary && mpBinCol )
		{
			mpBinCol->Delete();
			mpBinCol = 0;
		}

		mpNextRead = 0;
		mLeftBytes = 0;

#ifdef WIN32
	if(mStreamType == ST_FILE && mFileLockPtr->IsLocked())
		mFileLockPtr->Unlock();
#endif

	} catch (...)
	{
		// ignore
	}

}

void ReadByteStreamImpl::Read(unsigned char * pData, unsigned Int32 cb)
{
	Int64 lcb64 = (Int64) cb;
	Read(pData, lcb64);
}

void ReadByteStreamImpl::Read(unsigned char * pData, Int64 cb)
{
	switch(mStreamType)
	{
	case ST_MEMORY:
		Peek(pData, cb);
		Skip(cb);
		break;
	case ST_FILE:
		{
			mFilePtr->Read(pData, cb);

			mpNextRead += cb;
			mCurrentBlockLeftBytes -= cb;
			mLeftBytes -= cb;
			if ( ! mCountStack.empty() )
			{
				mCountStack.top() += cb;
			}
			break;
		}
	}
}


void ReadByteStreamImpl::Peek(unsigned char * pData, unsigned Int32 cb)
{
	if ( cb <= mLeftBytes )
	{
		Peek(pData, (Int64) cb);
	}

	else
	{
		throw MBase::ExceptionImplementation<ReadStreamException>("Not enough data for reading");
	}
}

void ReadByteStreamImpl::Peek(unsigned char * pData, Int64 iRequestedBytes)
{
	if ( iRequestedBytes <= mLeftBytes )
	{
		switch(mStreamType)
		{
		case ST_MEMORY:
		{
			Int64 lRequestedBytes = iRequestedBytes;

			unsigned char * lpData = pData;
			unsigned char * lpReadSource = mpNextRead;
			UInt64 lCurrentBlockLeftBytes = mCurrentBlockLeftBytes;
			unsigned Int32 lCurrentBlockNumber = mCurrentBlockNumber;

			if ( lRequestedBytes > lCurrentBlockLeftBytes )
			{
				while ( lRequestedBytes > lCurrentBlockLeftBytes )	// read from different binary
				{
					UInt64 lReadBytes = lCurrentBlockLeftBytes;

					::memmove(lpData, lpReadSource, lReadBytes);

					lpData += lReadBytes ;
					lRequestedBytes -= lReadBytes;

					lCurrentBlockNumber ++;
					lpReadSource = (unsigned char *) mpBinCol->GetBinary(lCurrentBlockNumber)->c_ptr();
					lCurrentBlockLeftBytes = (unsigned int)mpBinCol->GetBinary(lCurrentBlockNumber)->GetSize();
				}
			}

			_ASSERT( lRequestedBytes <= lCurrentBlockLeftBytes );

			::memmove(lpData, lpReadSource, lRequestedBytes);
			break;
		}
		case ST_FILE:
		{
			Int64 lCurrentPosition = mFilePtr->GetPosition();
			mFilePtr->Read(pData, iRequestedBytes);
			mFilePtr->SetPosition(lCurrentPosition);
			break;
		}
		}
		
	}
	else
	{
		throw MBase::ExceptionImplementation<ReadStreamException>("Not enough data for reading");
	}
}


void ReadByteStreamImpl::Skip(unsigned Int32 cb)
{
	Skip( (Int64) cb);
}

void ReadByteStreamImpl::Skip(Int64 cb)
{
	if ( cb <= mLeftBytes )
	{	
		switch(mStreamType)
		{
		case ST_MEMORY:
		{
			Int64 lSkipBytes = cb;

			while ( lSkipBytes >= mCurrentBlockLeftBytes && lSkipBytes > 0 )
			{
				lSkipBytes -= mCurrentBlockLeftBytes ;

				mCurrentBlockNumber ++;
				if (mCurrentBlockNumber == mpBinCol->GetBinaryCount() )
				{
					_ASSERT(lSkipBytes == 0 );  // we are at the end of the stream. No more skip request.
					_ASSERT(cb == mLeftBytes );

					mpNextRead = 0;
					mCurrentBlockLeftBytes = 0;
				}
				else
				{
					ParkReadPointer(mCurrentBlockNumber );
				}
			}

			mpNextRead += lSkipBytes;
			mCurrentBlockLeftBytes -= lSkipBytes;
			break;
		}
		case ST_FILE:
		{
			Int64 lSkipBytes = cb;
			mFilePtr->MovePosition(lSkipBytes);
			mpNextRead += lSkipBytes;
			mCurrentBlockLeftBytes -= lSkipBytes;
			break;
		}
		}
	} 
	else
	{
		throw MBase::ExceptionImplementation<ReadStreamException>("Not enough data for reading");
	}
	mLeftBytes -= cb;
	if ( ! mCountStack.empty() )
	{
		mCountStack.top() += cb;
	}
}

bool ReadByteStreamImpl::MoreData() throw()
{
	return ( mLeftBytes > 0 );
}

void ReadByteStreamImpl::Rewind() throw()
{
	Init();
	while ( ! mCountStack.empty() )
	{
		mCountStack.pop();
	}
}

Int64 ReadByteStreamImpl::Get64BitSize() throw()
{
	return mpBinCol->GetTotalLength();
}

Int64 ReadByteStreamImpl::GetNumberOfUnreadBytes() throw()
{
	return mLeftBytes;
}

MDataType::BinaryCollection* ReadByteStreamImpl::GiveOutBinaryCollection()
{

	MDataType::BinaryCollectionPtr lResPtr(mpBinCol);

	mpBinCol = MDataType::CreateBinaryCollection();
	mpBinCol->Insert(MDataType::CreateBinary());

	Rewind();

	return lResPtr.GiveUp();
}

MDataType::BinaryCollection* ReadByteStreamImpl::CopyOutBinaryCollection(Int32 iStartPosition) const
{
	MDataType::BinaryCollectionPtr lResPtr(MDataType::CreateBinaryCollection());

	Int32 lLeftToSkip = iStartPosition;

	for(Int32 lBinaryIndex = 0; lBinaryIndex < (Int32)mpBinCol->GetBinaryCount(); lBinaryIndex++)
	{
		const MDataType::Binary *lPtr = mpBinCol->GetBinary(lBinaryIndex);
		Int32 lBinLen = (Int32)lPtr->GetSize();
		if ( lLeftToSkip >= lBinLen )
		{
			lLeftToSkip -= lBinLen;
		}
		else
		{
			lResPtr->Insert(MDataType::CreateBinary(((char*)lPtr->c_ptr() + lLeftToSkip),
							lBinLen - lLeftToSkip ));
			lLeftToSkip = 0;
		}
	}

	return lResPtr.GiveUp();
}



void ReadByteStreamImpl::StartCount()
{
	mCountStack.push(0);
}

Int64 ReadByteStreamImpl::GetCount()
{
	_ASSERTE(! mCountStack.empty() );
	if ( mCountStack.empty() )
	{
		throw MBase::ExceptionImplementation<ReadStreamException>("No count available.");
	}

	return mCountStack.top();
}

void ReadByteStreamImpl::StopCount()
{
	if(!mCountStack.empty())
	{
		Int64 lTopCount = mCountStack.top();
		mCountStack.pop();

		if ( ! mCountStack.empty() )
		{
			mCountStack.top() = mCountStack.top() + lTopCount;
		}
	}
}

void ReadByteStreamImpl::Delete() throw()
{
	delete this;
}

// Wait for data in read streams
bool ReadByteStreamImpl::WaitForData(unsigned Int32 iTimeOut)
{
	// This function is not suppored!
	_ASSERTE(false);
	return false;
}

/*------------------------------
	private functions
---------------------------------*/
void ReadByteStreamImpl::Init()
{
	switch(mStreamType)
	{
	case ST_MEMORY:
		_ASSERTE(mpBinCol);
		_ASSERTE(mpBinCol->GetBinaryCount() > 0 );

		mCurrentBlockNumber = 0;
		mLeftBytes = mpBinCol->GetTotalLength();

		ParkReadPointer(0);
		break;
	case ST_FILE:
		_ASSERTE(!mFilePtr.IsNull());
 		mFilePtr->SetPosition(0);
		mLeftBytes = mFilePtr->GetSize();
		break;
	}
}

void ReadByteStreamImpl::ParkReadPointer(unsigned Int32 iCurrentBlockNumber)
{
	mpNextRead = (unsigned char *) mpBinCol->GetBinary(iCurrentBlockNumber)->c_ptr();
	mCurrentBlockLeftBytes = mpBinCol->GetBinary(iCurrentBlockNumber)->GetSize();
}


}
