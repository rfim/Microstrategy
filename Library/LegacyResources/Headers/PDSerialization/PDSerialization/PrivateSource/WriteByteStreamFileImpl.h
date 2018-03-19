//==============================================================================================
//	FILENAME	:	WriteByteStreamFileImpl.h
//	AUTHOR		:	Deepak Karuppiah, Yi Luo
//	CREATION	:	04/08/2008
//	Copyright (C) MicroStrategy, Inc. 2008
//==============================================================================================

#ifndef Serialization_WriteByteStreamFileImpl_H
#define Serialization_WriteByteStreamFileImpl_H

#include "PDSerialization/PDSerialization/WriteByteStream.h"

#include "PDSerialization/PDSerialization/PDSerialization.h"
#include "FileSystem/FileSystem/WriteableFile.h"

namespace MSerialization
{

class WriteByteStreamFileImpl : public WriteByteStream
{
public:
	WriteByteStreamFileImpl(MFileSystem::WriteableFile::Ptr& iFilePtr);
	~WriteByteStreamFileImpl() throw();

	void Write(const unsigned char * pData, unsigned Int32 cb, ReservedPosition* ipReservedOffset  );
	void Write(const unsigned char * pData, Int64 cb, ReservedPosition* ipReservedOffset );
	void Abandon();

	ReservedPosition ReserveBytes(unsigned Int32 iByteCnt);

	// flush out the buffer and close the file
	void CommitStream();

	StreamType GetStreamType();
	Int64 GetSize() throw();
	void Pack();

	void SetTimeout(Int64 iTimeoutInMilliseconds) {};

	// functions unique to this class
	MFileSystem::WriteableFile::ReturnPtr ReleaseFilePtr();

private:	// function section

	void SetToBuffer(unsigned char iByte, unsigned Int32 cb);
	void WriteToBuffer(const unsigned char* ipData, Int64 cb);

private:	// data section

#ifdef WIN32
	MBase::StrongPtr<MFileSystem::WriteableFile::WriteSmartLock_NoWait> mFileLockPtr;
#endif

	MFileSystem::WriteableFile::Ptr mFilePtr;
	bool mFileWriteFailed;

	MBase::StrongPtr<char> mFileWriteBuffer;
	unsigned Int32 mUsedBufferLength;

	Int64 mStreamSize;

	bool mAbandonStream;

//	MBase::Timer mTimer;
};

	
};

#endif  // Serialization_WriteByteStreamFileImpl_H
