//==============================================================================================
//	FILENAME	:	ReadByteStreamImpl.h
//	AUTHOR		:	Yi Luo
//	CREATION	:	07/12/2002
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================

#ifndef Serialization_ReadByteStreamImpl_H
#define Serialization_ReadByteStreamImpl_H

#include "PDSerialization/PDSerialization/PDSerialization.h"
#include "DataType/DataType/Binary.h"
#include "DataType/DataType/BinaryCollection.h"
#include "FileSystem/FileSystem/ReadableFile.h"
#include "PDSerialization/PDSerialization/MultiReadStream.h"
#include <stack>
#include "PDSerialization/PDSerialization/ReadByteStream.h"

namespace MSerialization
{

class ReadByteStreamImpl : public ReadByteStream
{
public:
	ReadByteStreamImpl(MDataType::BinaryCollection* ipB, bool iOwnBinary = true);
	ReadByteStreamImpl(MDataType::Binary* ipB, bool iOwnBinary = true);
	ReadByteStreamImpl(MFileSystem::ReadableFile::Ptr& iFilePtr);
	~ReadByteStreamImpl() throw() ;

	/**
		Constructor used to make a read byte stream on a multi-read stream.
		The byte stream will read from the data in the multi-read stream.  It
		will hold a reference to the multi-read stream object to prevent it from
		being deleted while this object is in use.
		@param ipCollection The binary collection that lies under the stream
		@param ipStream The multi-read stream from which the collection came.
	*/
	ReadByteStreamImpl(MDataType::BinaryCollection* ipCollection, MultiReadStream* ipStream);

	void Read(unsigned char * pData, unsigned Int32 cb);
	void Read(unsigned char * pData, Int64 cb);
	void Peek(unsigned char * pData, unsigned Int32 cb);
	void Peek(unsigned char * pData, Int64 iRequestedBytes);
//	const void * GetBlockStart(unsigned int cb);	we cannot provide pointer to a block because of the non-continuous binary
//	const void * GetBlockStart(Int64 cb);
	void Skip(unsigned Int32 cb);
	void Skip(Int64 cb);
	bool MoreData() throw() ;

	void Rewind() throw();

	Int64 Get64BitSize() throw();
	Int64 GetNumberOfUnreadBytes() throw();

	// Give out the binary collection. The caller takes over the ownership of the BinaryCollection
	MDataType::BinaryCollection* GiveOutBinaryCollection();
	// Copy out the binary collection. The caller takes over the ownership of the BinaryCollection
	// Should copy the binary from the specified location.
	MDataType::BinaryCollection* CopyOutBinaryCollection(Int32 iStartPosition=0) const;

	void StartCount();
	Int64 GetCount();
	void StopCount();

	void Delete() throw();

	// Wait for data in read streams
	bool WaitForData(unsigned Int32 iTimeOut);

	void SetTimeout(Int64 iTimeoutInMilliseconds) {} ;

private:

	void Init();

	StreamType mStreamType;

	void ParkReadPointer(unsigned Int32 iCurrentBlockNumber);

	MDataType::BinaryCollection * mpBinCol;
	bool mOwnBinary;
	unsigned char * mpNextRead;
	UInt64 mLeftBytes;
	UInt64 mCurrentBlockLeftBytes;
	unsigned Int32 mCurrentBlockNumber;

	std::stack<Int64> mCountStack;

	/// Reference to multi-read stream (if any) which owns our binary collection
	MultiReadStream::Ptr mMultiReadStreamPtr;

#ifdef WIN32
	MBase::StrongPtr<MFileSystem::ReadableFile::ReadSmartLock_NoWait> mFileLockPtr;
#endif

	MFileSystem::ReadableFile::Ptr mFilePtr;
};


};

#endif  // Serialization_ReadByteStreamImpl_H
