//==============================================================================================
//	FILENAME	:	WriteByteStreamImpl.cpp
//	AUTHOR		:	Yi Luo
//	CREATION	:	07/11/2002
//	Copyright (C) MicroStrategy, Inc. 2002
//==============================================================================================

#include "StdAfx.h"

#include "WriteByteStreamImpl.h"
#include "Base/Defines/ExceptionImplementation.h"
#include "DataType/DataType/Binary.h"
#include "Facilities.h"
#include <stdlib.h>
#include <memory.h>

#ifdef WIN32
#pragma warning(disable:4273)	// 'MSerialization::CreateWriteByteStream' : inconsistent dll linkage
#endif

namespace MSerialization
{

WriteByteStream::ReturnPtr CreateWriteByteStream(MDataType::BinaryCollection* ipBins, 
												  unsigned Int32 iBlockSize, 
												  MBase::MemoryContract* ipMemoryContract)
{
	_ASSERTE(ipBins);
	return new WriteByteStreamImpl(ipBins, iBlockSize, ipMemoryContract);
}


/*==================================================================
common code of WriteByteStreamImpl for both File and memory-based implementation
==================================================================*/

void WriteByteStreamImpl::CommitStream()
{	// do nothing
}

StreamType WriteByteStreamImpl::GetStreamType()
{
	return 	ST_MEMORY; 
}

Int64 WriteByteStreamImpl::GetSize() throw()
{
	return mStreamSize;
}

void WriteByteStreamImpl::Abandon()
{
	mAbandonStream = true;
}

WriteByteStreamImpl::WriteByteStreamImpl(MDataType::BinaryCollection* ipBins,
										 unsigned Int32 iBlockSize, MBase::MemoryContract* ipMemoryContract)
: mLastBlockPtr(NULL)
, mpMemContract(ipMemoryContract)
, mBlockSize(iBlockSize)
, mpFirstFreeByte(0)
, mLastBlockFreeSize(0)
, mpBinaryList(ipBins)
, mStreamSize(0)
, mAbandonStream(false)
{
	AppendBinaryWithSize(mBlockSize);
}

WriteByteStreamImpl::~WriteByteStreamImpl() throw()
{
}

void WriteByteStreamImpl::Write(const unsigned char * pData, unsigned Int32 cb, ReservedPosition* ipReservedOffset )
{
	_ASSERTE(!mAbandonStream && "Stream abandoned!!");
	Write(pData, (Int64) cb, ipReservedOffset);
}

/*	yluo: we will allow for large chunk > 4 GB, given the availability of 64-bit Unix machine. */
void WriteByteStreamImpl::Write(const unsigned char * pData, Int64 cb, ReservedPosition* ipReservedOffset)
{
	_ASSERTE(!mAbandonStream && "Stream abandoned!!");

	if ( ipReservedOffset != 0 )
	{	// write to reserved location
		memmove(ipReservedOffset->mpAddr, pData, cb);
		return;
	}

	if ( mLastBlockFreeSize < cb )
	{
		// at first, release the extra free memory in the last memory block
		Pack();
		
		if ( cb > mBlockSize )
		{	// larger than default block size. Create new block with current size plus some extra for later use
			AppendBinaryWithSize(cb + mBlockSize);
		}
		else
		{	// work directly on the last block with default size
			AppendBinaryWithSize(mBlockSize);
		}

		// now we have mpFirstFreeByt and mLastBlockFreeSize set to the right value inside AppendBinaryWithSize().
	}
	
	_ASSERTE( mLastBlockFreeSize >= cb );

	::memmove(mpFirstFreeByte, pData, cb);
	mStreamSize += cb;
	
	mpFirstFreeByte += cb;
	mLastBlockFreeSize -= cb;
}

/// ReserveBytes in memory
ReservedPosition WriteByteStreamImpl::ReserveBytes(unsigned Int32 iByteCount)
{
	_ASSERTE(!mAbandonStream && "Stream abandoned!!");

	MBase::StrongArrayPtr<unsigned char> lData(new unsigned char [iByteCount]);
	::memset(lData.Get(), 0, iByteCount);

	Write(lData.Get(), (Int64) iByteCount, 0);

	unsigned char * lpRes = mpFirstFreeByte - iByteCount;	// we write directly to the binary. 

	ReservedPosition lResult((void*) lpRes);
	return lResult;		
}


void WriteByteStreamImpl::Merge(MDataType::BinaryCollection* ipBinCol)
{
	_ASSERTE(!mAbandonStream && "Stream abandoned!!");

	// at first, pack the stream to put the working data into the mpBinaryCollection
	Pack();
	mStreamSize += ipBinCol->GetTotalLength();
	mpBinaryList->Merge(ipBinCol);
}


// put the last block into the BinaryCollection, zap any unwritten part.
void WriteByteStreamImpl::Pack()
{
	_ASSERTE(!mAbandonStream && "Stream abandoned!!");

	if ( mLastBlockFreeSize == 0 )
		return;	// last block is full. No need to zap.
	
	const_cast<MDataType::Binary*>(mpBinaryList->GetBinary(mpBinaryList->GetBinaryCount() -1 ))->ZapTail(mLastBlockFreeSize);
	mLastBlockFreeSize = 0;	// no more free bytes in last block
}


void WriteByteStreamImpl::AppendBinaryWithSize(Int64 iSize)
{
	_ASSERTE(!mAbandonStream && "Stream abandoned!!");

	MCMRESULT lReq = MCM_OK;
	if ( ! mpMemContract.IsNull() && (lReq = mpMemContract->Alloc(iSize)) != MCM_OK )
	{
		throw MBase::ExceptionImplementation<WriteStreamException>("Not enough memory left.", lReq);
	}

	MDataType::BinaryPtr lBinPtr(MDataType::CreateBinary(iSize) );
	if ( ! mpMemContract.IsNull() )
	{
		mpMemContract->Free(iSize);
	}

	mpBinaryList->Insert( lBinPtr.Get() );
	mLastBlockPtr = (unsigned char*)lBinPtr->c_ptr() ;
	lBinPtr.GiveUp();

	::memset(mLastBlockPtr , 0, iSize);

	mpFirstFreeByte = mLastBlockPtr;
	mLastBlockFreeSize = iSize;
}

}
