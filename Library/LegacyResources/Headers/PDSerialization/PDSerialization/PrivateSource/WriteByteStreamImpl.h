//==============================================================================================
//	FILENAME	:	WriteByteStreamImpl.h
//	AUTHOR		:	Yi Luo
//	CREATION	:	07/11/2002
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================

#ifndef Serialization_WriteByteStreamImpl_H
#define Serialization_WriteByteStreamImpl_H

#include "PDSerialization/PDSerialization/PDSerialization.h"
#include "Base/Interfaces/BaseException.h"
#include "Base/Interfaces/BaseMemoryContract.h"
#include "DataType/DataType/Binary.h"
#include "DataType/DataType/BinaryCollection.h"
#include "PDCHeader/PDCvector"

#include "PDSerialization/PDSerialization/WriteByteStream.h"


namespace MSerialization
{

/*
	This is local memory based ByteStream. The memory is managed by the BinaryCollection passed into this object
	via the constructor. WriteByteStreamImpl doesn't own that BinaryCollection anytime.
*/
class WriteByteStreamImpl : public WriteByteStream
{
public:
	WriteByteStreamImpl(MDataType::BinaryCollection* ipBins, unsigned Int32 iBlockSize, MBase::MemoryContract* ipMemoryContract);
	~WriteByteStreamImpl() throw();

	void Write(const unsigned char * pData, unsigned Int32 cb, ReservedPosition* ipReservedOffset);
	void Write(const unsigned char * pData, Int64 cb, ReservedPosition* ipReservedOffset );
	
	void Abandon();

	ReservedPosition  ReserveBytes(unsigned Int32 iByteCnt);

	// pack all the data into a Binary. and release the control of the data
	// for File-based stream, this means flushing out the buffer.
	void CommitStream();

	Int64 GetSize() throw();
	StreamType GetStreamType();

	// release extra free memory in the last block
	void Pack();

	// functions unique to this class

	// merge the content of ipStream into this stream. 
	// take over the ownership of the contents.
	void Merge(MDataType::BinaryCollection* ipBinCol);

	void SetTimeout(Int64 iTimeoutInMilliSeconds) { };

private:
	// append a binary to the end of list, and reset 3 pointers for subsequent writing
	void AppendBinaryWithSize(Int64 iSize);

	MDataType::BinaryCollection* mpBinaryList;

	unsigned char * mLastBlockPtr;  // the workspace for the writing is now the last Binary itself. We get rid of temp buffer now. 8/24/06

	MBase::MemoryContractPtr  mpMemContract;


	unsigned Int32 mBlockSize;

	unsigned char * mpFirstFreeByte;
	unsigned Int32 mLastBlockFreeSize;

	Int64 mStreamSize;
	bool mAbandonStream;
};

	
};

#endif  // Serialization_WriteByteStreamImpl_H
