//==============================================================================================
//	FILENAME	:	WriteByteStream.h
//	AUTHOR		:	Yuan Wei
//	CREATION	:	12/31/2007
//	Copyright (C) MicroStrategy, Inc. 2007
//==============================================================================================

#ifndef Serialization_WriteByteStream_H
#define Serialization_WriteByteStream_H

#include "PDSerialization.h"
#include "PDSerialization/PDSerialization/PrivateSource/Facilities.h"
#include "Base/Interfaces/BaseException.h"
#include "Base/Interfaces/BaseMemoryContract.h"
#include "DataType/DataType/BinaryCollection.h"
#include "FileSystem/FileSystem/WriteableFile.h"
#include "PDCHeader/PDCvector"
#ifndef __ANDROID__
#include "MultiProcess/SharedMemory/SharedMemory.h"
#endif
namespace MSerialization
{

class WriteByteStream;
class WriteByteStreamFileImpl;
class WriteByteStreamImpl;

class ReservedPosition
{
public:
	// ReservedPosition(const ReservedPosition& irCopy);	use default copy constructor
	~ReservedPosition() throw() { } ;

private:
	friend class WriteByteStreamFileImpl;
	friend class WriteByteStreamImpl;
	friend class WriteSharedMemByteStreamImpl;

	union
	{
		void* mpAddr;
		Int64 mFilePos;
	};

	ReservedPosition(void* ipMemAddr) : mpAddr(ipMemAddr) {} ;
	ReservedPosition(Int64 iFilePos) : mFilePos(iFilePos) { } ;

};


class DLL_PDSERIALIZATION_EXIM WriteByteStream
{
public:

	typedef MBase::StrongPtr<WriteByteStream, MBase::DeleteOperatorGeneric<WriteByteStream> > Ptr;
	typedef MBase::ReturnPtr<WriteByteStream, MBase::DeleteOperatorGeneric<WriteByteStream> > ReturnPtr;

	virtual ~WriteByteStream() throw()  {};
	virtual void Delete() throw() { delete this; } ;

	// Shared interface
	virtual void Write(const unsigned char * pData, unsigned Int32 cb, ReservedPosition* ipReservedOffset = 0 ) = 0;
	virtual void Write(const unsigned char * pData, Int64 cb, ReservedPosition* ipReservedOffset = 0) = 0;
	virtual void Abandon() = 0;

	// Interface functions for shared memory based write byte streams only
	// Mark the end of a stream
	virtual void CommitStream() = 0;

	// Interface functions for non-shared memory based write byte streams.
	virtual ReservedPosition ReserveBytes(unsigned Int32 iByteCnt) = 0;
	virtual Int64 GetSize() throw() = 0;
	virtual StreamType GetStreamType() = 0;
	virtual void Pack() = 0;

	virtual void SetTimeout(Int64 iTimeoutInMilliSeconds) = 0;
};


DLL_PDSERIALIZATION_EXIM WriteByteStream::ReturnPtr CreateWriteByteStream(MDataType::BinaryCollection* ipBins, 
																		  unsigned Int32 iBlockSize = 512, 
																		  MBase::MemoryContract* ipMemoryContract = NULL);

DLL_PDSERIALIZATION_EXIM WriteByteStream::ReturnPtr CreateWriteByteStream(MFileSystem::WriteableFile::Ptr& iFilePtr);
#ifndef __ANDROID__
DLL_PDSERIALIZATION_EXIM WriteByteStream::ReturnPtr CreateWriteByteStream(MMultiProcess::SharedMemory* ipSharedMemPtr,
																		   unsigned Int32 iBlockSize = 1024);
#endif


};

#endif // Serialization_WriteByteStream_H
