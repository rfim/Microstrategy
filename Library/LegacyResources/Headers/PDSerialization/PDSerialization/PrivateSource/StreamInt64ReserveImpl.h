//==============================================================================================
//	FILENAME	:	StreamInt64ReserveImpl.h
//	AUTHOR		:	Yi Luo
//	CREATION	:	07/10/2002
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================

#ifndef Serialization_StreamInt64ReserveImpl_H
#define Serialization_StreamInt64ReserveImpl_H

#include "PDSerialization/PDSerialization/StreamInt64Reserve.h"
#include "PDSerialization/PDSerialization/WriteByteStream.h"

namespace MSerialization
{
class StreamInt64ReserveImpl : public StreamInt64Reserve
{
public:
	void Write(Int64 data);
	void Delete() throw();
	StreamInt64ReserveImpl(WriteByteStream *ipBStream, const ReservedPosition& iReservedOffset,
			       bool iRawFormat = false, Int64 defValue = 0x0BADF00D);
	~StreamInt64ReserveImpl() throw();

private:
	WriteByteStream *mpBStream;
	ReservedPosition mReservedOffset;
	bool mRawFormat;
};

class StreamInt32ReserveImpl : public StreamInt32Reserve
{
public:
	void Write(Int32 data);
	void Delete() throw();
	StreamInt32ReserveImpl(WriteByteStream *ipBStream, const ReservedPosition& iReservedOffset, 
			       bool iRawFormat = false, Int32 defValue = 0x0BADF00D);
	~StreamInt32ReserveImpl() throw();

private:
	WriteByteStream *mpBStream;
	ReservedPosition mReservedOffset;
	bool mRawFormat;
};

};

#endif // Serialization_StreamInt64ReserveImpl_H
