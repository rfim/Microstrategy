//==============================================================================================
//	FILENAME	:	ReadByteStream.h
//	AUTHOR		:	Yuan Wei
//	CREATION	:	12/31/2007
//	Copyright (C) MicroStrategy, Inc. 2007
//==============================================================================================

#ifndef Serialization_ReadByteStream_H
#define Serialization_ReadByteStream_H

#include "PDSerialization/PDSerialization/PDSerialization.h"
#include "DataType/DataType/Binary.h"
#include "DataType/DataType/BinaryCollection.h"
#include "FileSystem/FileSystem/ReadableFile.h"
#include <stack>
#if !defined(WIN32) && !defined(__ANDROID__)
#include "MultiProcess/SharedMemory/SharedMemory.h"
#endif
#include "PDSerialization/PDSerialization/PrivateSource/Facilities.h"

namespace MDataType
{
	class BinaryCollection;
}


namespace MSerialization
{

class DLL_PDSERIALIZATION_EXIM ReadByteStream
{
public:
	virtual void Read(unsigned char * pData, unsigned Int32 cb) = 0;
	virtual void Read(unsigned char * pData, Int64 cb) = 0;

	virtual void Rewind() = 0;

	virtual void Peek(unsigned char * pData, unsigned Int32 cb) = 0;
	virtual void Peek(unsigned char * pData, Int64 iRequestedBytes) = 0;
	virtual void Skip(unsigned Int32 cb) = 0;
	virtual void Skip(Int64 cb)= 0;

	virtual Int64 Get64BitSize() throw() = 0;
	virtual Int64 GetNumberOfUnreadBytes() throw() = 0;

	virtual MDataType::BinaryCollection* GiveOutBinaryCollection() = 0;
	virtual MDataType::BinaryCollection* CopyOutBinaryCollection(Int32 iStartPosition=0) const = 0;

	virtual void StartCount() = 0;
	virtual Int64 GetCount() = 0;
	virtual void StopCount() = 0;

	virtual void Delete() throw() = 0;

	virtual bool WaitForData(unsigned Int32 iTimeOut) = 0;

	virtual void SetTimeout(Int64 iTimeoutInMilliseconds) = 0;

	virtual ~ReadByteStream() throw()  {};
};

// Strong pointer to ReadByteStream
typedef MBase::StrongPtr<ReadByteStream, MBase::DeleteOperatorGeneric<ReadByteStream> > ReadByteStreamPtr;

#if !defined(WIN32) && !defined(__ANDROID__)
DLL_PDSERIALIZATION_EXIM ReadByteStream* CreateReadByteStreamFromSharedMem(MMultiProcess::SharedMemory* ipSharedMemPtr);
#endif
};

#endif // Serialization_ReadByteStream_H
