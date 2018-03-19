//==============================================================================================
//	FILENAME	:	StreamInt64ReserveImpl.cpp
//	AUTHOR		:	Yi Luo
//	CREATION	:	07/10/2002
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================
#include "StdAfx.h"
#include "StreamInt64ReserveImpl.h"
#include "Facilities.h"

namespace MSerialization
{

//	public:

void StreamInt64ReserveImpl::Write(Int64 data)
{
	Int64 lDefValue = data;

#ifdef LITTLE_ENDIAN
	mpBStream->Write((unsigned char*) &lDefValue, (unsigned Int32) sizeof(Int64), &mReservedOffset);
#else
	if ( mRawFormat)
	{
		mpBStream->Write((unsigned char*) &lDefValue, (unsigned Int32) sizeof(Int64), &mReservedOffset);
	}
	else
	{
		Int64 lSwappedInt64 = const_swap8(lDefValue);
		mpBStream->Write( (unsigned char *)&lSwappedInt64, (unsigned Int32) sizeof(Int64), &mReservedOffset);			
	}
#endif
}

void StreamInt64ReserveImpl::Delete() throw()
{
	delete this;
}


StreamInt64ReserveImpl::StreamInt64ReserveImpl(WriteByteStream *ipBStream, const ReservedPosition& iReservedOffset, 
					       bool iRawFormat, Int64 defValue)
: mpBStream(ipBStream),
  mReservedOffset(iReservedOffset),
  mRawFormat(iRawFormat)
{
	_ASSERTE(ipBStream);
	Write(defValue);
}

StreamInt64ReserveImpl::~StreamInt64ReserveImpl() throw()
{
}


/*----------------------------------
	StreamInt32ReserveImpl
------------------------------------*/

void StreamInt32ReserveImpl::Write(Int32 data)
{
	Int32 lDefValue = data;

#ifdef LITTLE_ENDIAN
	mpBStream->Write((unsigned char*) &lDefValue, (unsigned Int32) sizeof(Int32), &mReservedOffset);
#else
	if ( mRawFormat)
	{
		mpBStream->Write((unsigned char*) &lDefValue, (unsigned Int32) sizeof(Int32), &mReservedOffset);
	}
	else
	{
		Int32 lSwappedInt32 = const_swap4(lDefValue);
		mpBStream->Write( (unsigned char *)&lSwappedInt32, (unsigned Int32) sizeof(Int32), &mReservedOffset);			
	}
#endif
}

void StreamInt32ReserveImpl::Delete() throw()
{
	delete this;
}

StreamInt32ReserveImpl::StreamInt32ReserveImpl(WriteByteStream *ipBStream, const ReservedPosition& iReservedOffset, 
					       bool iRawFormat, Int32 defValue)
: mpBStream(ipBStream),
  mReservedOffset(iReservedOffset),
  mRawFormat(iRawFormat)
{
	_ASSERTE(ipBStream);
	Write(defValue);
}

StreamInt32ReserveImpl::~StreamInt32ReserveImpl() throw()
{
}

}
