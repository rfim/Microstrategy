//==============================================================================================
//	FILENAME	:	WriteByteStreamFileImpl.cpp
//	AUTHOR		:	Deepak Karuppiah, Yi Luo
//	CREATION	:	03/08/2008
//	Copyright (C) MicroStrategy, Inc. 2008
//==============================================================================================

#include "StdAfx.h"

#include "WriteByteStreamFileImpl.h"

#include "FileSystem/FileSystem/Path.h"
#include "Base/Defines/ExceptionImpl.h"

#include "Base/Base/Timer.h"


#ifdef __hpux
	#define FILE_WRITE_BUFFER_SIZE 102400  // 100K. we use large value for HP due to slow response of NFS on HP.
#else
	#define FILE_WRITE_BUFFER_SIZE 10240  // 10K
#endif

namespace MSerialization
{

WriteByteStream::ReturnPtr CreateWriteByteStream(MFileSystem::WriteableFile::Ptr& iFilePtr)
{
	return new WriteByteStreamFileImpl(iFilePtr);
}


WriteByteStreamFileImpl::WriteByteStreamFileImpl(MFileSystem::WriteableFile::Ptr& iFilePtr)
:  mStreamSize(0)
, mFileWriteFailed(false)
, mFileWriteBuffer(new char[FILE_WRITE_BUFFER_SIZE] )
, mUsedBufferLength(0)
, mAbandonStream(false)
// , mTimer()
{
	mFilePtr.Acquire(iFilePtr);

/*	{
		// test write performance
		// need write 3 MB data in 15,000 times. Each time 200 bytes.

#define MYSIZE 1   // 4000
#define UNITSIZE 200

		char* lBuffer[15000/MYSIZE ];
		for(int j = 0; j < 15000 /MYSIZE;  j++)
		{
			lBuffer[j] = new char[ UNITSIZE * MYSIZE];
			sprintf(lBuffer[j], "buffer %d\n", j);
		}


		MBase::Timer lTimer;

		for(int i=0; i < 15000 /MYSIZE ; i ++)
		{
			mFilePtr->Write(lBuffer[i], UNITSIZE * MYSIZE );
		}

		unsigned Int64 lWrite = lTimer.GetAgeInMilliseconds();

		const char* lpFileName = "/home/yluo/testextra";
		FILE* lpFile = fopen(lpFileName, "w");
		fprintf(lpFile, "Time in write %d ms\n", (unsigned Int32) lWrite);
		fclose(lpFile);

		mFilePtr->SetPosition(0);
	}
*/

#ifdef WIN32
	// Acquire write lock before writing to the file
	MFileSystem::WriteableFile::WriteSmartLock_NoWait *lpFileLock = new MFileSystem::WriteableFile::WriteSmartLock_NoWait(*mFilePtr);

	if(lpFileLock->IsLocked())
	{
		mFileLockPtr.Acquire(lpFileLock);
	}
	else
	{
		delete lpFileLock;
		std::wstring lMessage = L"WriteByteStreamFileImpl:: Cannot acquire write lock for file:: ";
		lMessage += MFileSystem::Path::ToAbsolute(mFilePtr->GetPath()).c_str();
		throw MBase::ExceptionImpl<WriteStreamException>(lMessage.c_str());
	}

	mFilePtr->SetPositionToEnd();
#endif

}

WriteByteStreamFileImpl::~WriteByteStreamFileImpl() throw()
{
	try
	{
		CommitStream();
	}
	catch (...)
	{	// we are in destructor. Ignore.
	}

/*	// test close performance
	unsigned Int64 lWrite = mTimer.GetAgeInMilliseconds();

	const char* lpFileName = "/home/yluo/testextra";
	FILE* lpFile = fopen(lpFileName, "w");
	fprintf(lpFile, "Time in file byte stream %d ms\n", (unsigned Int32) lWrite);
	fclose(lpFile);
*/
}



void WriteByteStreamFileImpl::CommitStream()
{
	_ASSERTE( ! mFilePtr.IsNull());

	// flush out the data in the buffer.
	if ( mUsedBufferLength > 0 )
	{
		try
		{
			mFilePtr->Write(mFileWriteBuffer.Get(), mUsedBufferLength );
			mUsedBufferLength = 0;
		}
		catch(MBase::String&)
		{
			mFilePtr->SetPosition(0);
			mFilePtr->Truncate();
			mFileWriteFailed = true;
			// we don't rethrow, because we need close the file.
		}
	}

	if(!mAbandonStream)
	{
		mFilePtr->CommitAndClose();
	} 
	else
	{
		mFilePtr->Flush();
	}

#ifdef WIN32
	if(mFileLockPtr->IsLocked())
	{
		mFileLockPtr->Unlock();
	}
#endif
}

void WriteByteStreamFileImpl::Abandon()
{
	mAbandonStream = true;
}

MFileSystem::WriteableFile::ReturnPtr WriteByteStreamFileImpl::ReleaseFilePtr()
{
	// First wipe the file clean by setting EOF to beginning of file
	mFilePtr->SetPosition(0);
	mFilePtr->Truncate();

	//Finally release the file pointer
	return mFilePtr.Return();
}

void WriteByteStreamFileImpl::Write(const unsigned char * pData, unsigned Int32 cb, ReservedPosition* ipReservedOffset )
{
	_ASSERTE(!mAbandonStream && "Stream abandoned!!");
	Write(pData, (Int64) cb, ipReservedOffset);
}

void WriteByteStreamFileImpl::Write(const unsigned char * pData, Int64 cb, ReservedPosition* ipReservedOffset)
{
	_ASSERTE(!mAbandonStream && "Stream abandoned!!");
	_ASSERTE( ! mFileWriteFailed );

	try
	{
		if (ipReservedOffset == 0) // Write to the current position
		{
			WriteToBuffer( pData, cb);
			mStreamSize += cb;
		}
		else // Write to a reserved location in file
		{
			Int64 lCurrentFilePosition = mFilePtr->GetPosition();

			Int64 lTargetFilePos = ipReservedOffset->mFilePos ;
			_ASSERTE( lTargetFilePos  <= lCurrentFilePosition  + mUsedBufferLength);

			if ( lTargetFilePos  < lCurrentFilePosition )
			{	// must write into file for old reservation
				Int64 lLeftBytesInFile = lCurrentFilePosition - lTargetFilePos  ;
				Int64 lSizeToWriteToFile = (cb > lLeftBytesInFile) ? lLeftBytesInFile : cb;

				mFilePtr->SetPosition(lTargetFilePos );
				mFilePtr->Write(pData, lSizeToWriteToFile);
				mFilePtr->SetPosition(lCurrentFilePosition);

				cb -= lSizeToWriteToFile;	// there could be left-over data
				pData += lSizeToWriteToFile;
				lTargetFilePos  += lSizeToWriteToFile;
			}

			if ( cb > 0 )
			{	// the reserved memory is still in the buffer
				Int64 lPositionInBuffer = lTargetFilePos  - lCurrentFilePosition ;
				_ASSERTE( cb <= mUsedBufferLength);	// the buffer must have enough data than the reserved piece.

				memmove( mFileWriteBuffer.Get() + lPositionInBuffer, pData, cb);
			}
		}
	}
	catch(MBase::String& eMessage)
	{
		mFilePtr->SetPosition(0);
		mFilePtr->Truncate();
		mFileWriteFailed = true;
		throw eMessage;
	}
}


ReservedPosition WriteByteStreamFileImpl::ReserveBytes(unsigned Int32 iByteCount)
{
	_ASSERTE(!mAbandonStream && "Stream abandoned!!");
	_ASSERTE( ! mFileWriteFailed );

	Int64 lCurrentPosition = mFilePtr->GetPosition() + mUsedBufferLength;

	try
	{
		SetToBuffer(0, iByteCount);
	}
	catch (MBase::String& eMessage)
	{
		mFilePtr->SetPosition(0);
		mFilePtr->Truncate();
		mFileWriteFailed = true;
		throw eMessage;
	}

	mStreamSize += iByteCount;

	ReservedPosition lRes(lCurrentPosition);
	return lRes;
}

/*==================================================================
		private methods
==================================================================*/

void WriteByteStreamFileImpl::WriteToBuffer(const unsigned char* ipData, Int64 cb)
{
	// write to the buffer
	_ASSERTE( mUsedBufferLength < FILE_WRITE_BUFFER_SIZE  );

	unsigned Int32 lFreeBufferSize = FILE_WRITE_BUFFER_SIZE - mUsedBufferLength;
	Int64 lSizeToSaveToBuffer = ( cb <= lFreeBufferSize ) ? cb : lFreeBufferSize ;

	memmove(  mFileWriteBuffer.Get()+mUsedBufferLength, ipData, lSizeToSaveToBuffer);

	mUsedBufferLength += lSizeToSaveToBuffer;
	if ( mUsedBufferLength == FILE_WRITE_BUFFER_SIZE )
	{	// flush the buffer into file
		mFilePtr->Write(mFileWriteBuffer.Get(), mUsedBufferLength );
		mUsedBufferLength = 0;
	}

	Int64 lUnsavedDataSize = cb - lSizeToSaveToBuffer;
	if ( lUnsavedDataSize > 0 )
	{	// need save extra data into the buffer

		_ASSERTE( mUsedBufferLength == 0);

		if ( lUnsavedDataSize >= FILE_WRITE_BUFFER_SIZE )
		{	// no need to put in buffer. Just write to file
			mFilePtr->Write( ipData + lSizeToSaveToBuffer, lUnsavedDataSize);
		}
		else
		{
			memmove(mFileWriteBuffer.Get()+mUsedBufferLength, ipData+ lSizeToSaveToBuffer, lUnsavedDataSize);
			mUsedBufferLength += lUnsavedDataSize;
		}

		_ASSERTE( mUsedBufferLength < FILE_WRITE_BUFFER_SIZE  );

	}
}

void WriteByteStreamFileImpl::SetToBuffer(unsigned char iByte, unsigned Int32 cb)
{
	_ASSERTE( mUsedBufferLength < FILE_WRITE_BUFFER_SIZE  );

	unsigned Int32 lFreeBufferSize = FILE_WRITE_BUFFER_SIZE - mUsedBufferLength;
	unsigned Int32 lSizeToSaveToBuffer = ( cb <= lFreeBufferSize ) ? cb : lFreeBufferSize ;

	memset(mFileWriteBuffer.Get()+mUsedBufferLength, iByte, lSizeToSaveToBuffer);

	mUsedBufferLength += lSizeToSaveToBuffer;
	if ( mUsedBufferLength == FILE_WRITE_BUFFER_SIZE )
	{	// flush the buffer into file
		mFilePtr->Write(mFileWriteBuffer.Get(), mUsedBufferLength );
		mUsedBufferLength = 0;
	}

	unsigned Int32 lUnsavedDataSize = cb - lSizeToSaveToBuffer;
	if ( lUnsavedDataSize > 0 )
	{	// need save extra data into the buffer
		_ASSERTE( mUsedBufferLength == 0);

		_ASSERTE(lSizeToSaveToBuffer < FILE_WRITE_BUFFER_SIZE );	// we don't expect to set > 100K with the same byte.

		memset(mFileWriteBuffer.Get() + mUsedBufferLength, iByte, lUnsavedDataSize);
		mUsedBufferLength += lUnsavedDataSize;
	}
}


void WriteByteStreamFileImpl::Pack()
{
	// nothing to pack.
}

StreamType WriteByteStreamFileImpl::GetStreamType()
{
	return ST_FILE;
}

Int64 WriteByteStreamFileImpl::GetSize() throw()
{
	return mStreamSize;
}

}
