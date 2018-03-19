//
//  DSSXTabBitVector.h
//  ReportServiceCore
//
//  Created by Yang Zhuo on 11/11/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//
#ifndef DSSXTABBITVECTOR_H
#define DSSXTABBITVECTOR_H

#import "DSSAEHelper.h"
class DSSBinaryDataReader;

// For overflow detection 
#define		XTAB_BV_BITS		32
#define		XTAB_BV_BYTES		4

// aye, 2007/05/18
// Re-implement the BitVector for use in DE. 
// We need to introduce a 'IsQualified' flag for each row. Use BitVector to save space.
class DSSXTabBitVector {
public:
	DSSXTabBitVector(unsigned Int32 iLength = 0);
    DSSXTabBitVector(unsigned Int32 iLength, MBase::Buffer * ipBuffer);
	~DSSXTabBitVector();
	void Init(Int32 iLength, bool iAllocate);
	void Populate(DSSXTabBitVector* ipBitVector);
	void Acquire(DSSXTabBitVector* ipBitVector);
	void Set(unsigned Int32 iIndex, bool iValue);
	bool Get(unsigned Int32 iIndex);
	bool IsEmpty() { return (mpData == NULL);}
	bool IsAllTrue();
	Int32 GetStorageSize();
	unsigned Int32* GetStorage();
	Int32 GetLength();
	void Resize(Int32 iSize);
	bool IsSuperset(DSSXTabBitVector* ipSet);
	void Clear();
	void CalculateAnd(DSSXTabBitVector*);
	void CalculateOr(DSSXTabBitVector*);
	void CalculateAndNot(DSSXTabBitVector*);
	void CalculateNot();
	void BulkSet(Int32 iStart, Int32 iEnd, bool iValue);
	void SetSeekPosition(unsigned Int32 iStart);
	inline	Int32 GetNextSetPosition();
	inline  Int32 GetNextUnsetPosition();
	bool IsAnyTrue(Int32 iStart, Int32 iEnd);
	bool IsSparse(double iSelectPercentage);
	Int32 GetEstimateSetSize();
	Int32 GetSetSize();
    int LoadFromBinary(DSSBinaryDataReader * ipReadStream);
    bool NeedDelete();
private:
	void hAllocateData();
	enum {XTabBitVectorBlock = 61};
	unsigned Int32 mLength;		// number of bits in the vector
	unsigned Int32 *mpData;		// the storage place
	unsigned Int32 mDataSize;		// number of Int32 used for storage	
	unsigned Int32 *mpDataCache;	// the allocated storage, but was released and cached by Clear()	
	unsigned Int32 mSeekPosition;
	unsigned Int32 mSeekData;
	Int32 mSetSize;
    // weilin DE39755 2016/09/29 add mpBuffer to avoid memory leak
    MBase::Buffer * mpBuffer;
};

inline	Int32 DSSXTabBitVector::GetNextSetPosition()
{
	// weiwang 2010-11-12 return value is within range [0, mLength) or -1
	if (IsEmpty() || mSeekPosition >= mLength)
		return -1;
    
	unsigned Int32 lData = mSeekData;
	unsigned Int32 lSeekPosition = mSeekPosition;
	
	// Find the first word that is set
	if (lData == 0)
	{
		unsigned Int32 lWord = lSeekPosition/ 32 + 1;
		while (lData == 0 && lWord < mDataSize)
			lData = mpData[lWord++];
        
		lSeekPosition = (lWord - 1) * 32;
	}
    
	// It's possible that the 
	if (lData == 0)
	{
		mSeekPosition = mLength;
		return -1;
	}
	else
	{
		while ((lData & 0x1) == 0)
		{
			lData = lData >> 1;
			lSeekPosition++;
		}
	}
    
	// weiwang 2010-11-12 with CalculateNot before, lSeekPosition may be >= mLength
	if (lSeekPosition >= mLength)
	{
		mSeekPosition = mLength;
		return -1;
	}
    
	// At the moment, lSeekPosition is the postion where the bit is set
	// We prepare for the next call
	mSeekPosition = lSeekPosition + 1;
    
	// weiwang 2010-11-12 need not prepare next word if ...
	if (mSeekPosition == mLength)
		return lSeekPosition;
    
	if ((mSeekPosition & 0x1f) == 0)
	{
		// need to read the next word
		mSeekData = mpData[mSeekPosition / 32];
	}
	else
	{
		mSeekData = lData >> 1;
	}
    
	return lSeekPosition;
}

inline Int32 DSSXTabBitVector::GetNextUnsetPosition()
{
	// weiwang 2010-11-12 make the same change as GetNextSetPosition()
	if (IsEmpty() || mSeekPosition >= mLength)
		return -1;
    
	unsigned Int32 lMask = 0xffffffffUL >> (mSeekPosition & 0x1f);
    
	Int32 lData = lMask & ~mSeekData;
	unsigned Int32 lSeekPosition = mSeekPosition;
	
	// Find the first word that is set
	if (lData == 0)
	{
		unsigned Int32 lWord = lSeekPosition/ 32 + 1;
		while (lData == 0 && lWord < mDataSize)
			lData = (Int32)(~mpData[lWord++]);
        
		lSeekPosition = (lWord - 1) * 32;
	}
    
	// It's possible that the 
	if (lData == 0)
	{
		mSeekPosition = mLength;
		return -1;
	}
	else
	{
		while ((lData & 0x1) == 0)
		{
			lData = lData >> 1;
			lSeekPosition++;
		}
	}
    
	if (lSeekPosition >= mLength)
	{
		mSeekPosition = mLength;
		return -1;
	}
    
	// At the moment, lSeekPosition is the postion where the bit is set
	// We prepare for the next call
	mSeekPosition = lSeekPosition + 1;
    
	if (mSeekPosition == mLength)
		return lSeekPosition;
    
	if ((mSeekPosition & 0x1f) == 0)
	{
		// need to read the next word
		mSeekData = mpData[mSeekPosition / 32];
	}
	else
	{
		lMask = 0xffffffffUL >> (mSeekPosition & 0x1f);
		mSeekData = lMask & ((~lData) >> 1);
	}
    
	return lSeekPosition;
}

#endif