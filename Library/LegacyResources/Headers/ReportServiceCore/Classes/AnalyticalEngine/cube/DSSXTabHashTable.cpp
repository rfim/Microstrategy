/*
 *  DSSXTabHashTable.cpp
 *  Currency Converter
 *
 *  Created by Xiaolong Wang on 12/18/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#include "DSSXTabHashTable.h"
#import <memory.h>
#import <stdlib.h>
#import <math.h>
#import "DSSDataSource.h"
#include "DSSAEHelper.h"
#include "DSSCMBlockFileUtil.h"

#define MAX_ROW_COUNT  1048576
// Class DSSXTabHashTable
DSSXTabHashTable::DSSXTabHashTable()
:mpBuffer(NULL), mnBuckets(0), mnUnits(0), mBuckets(NULL),
mpBucketMask(NULL), mpBucketSpace(NULL), mpUnitBitOffset(NULL),
mpNodeList(NULL),
mpWildCardUnits(NULL)
{
	// 8/24/2012 tehe
    mbUseMappedFile = false;
    mBlockType = cmXTabHashTable;
}


DSSXTabHashTable::~DSSXTabHashTable()
{
	if (mpBucketSpace)
		delete [] mpBucketSpace;
	mpBucketSpace = NULL;
	
	if (mpBucketMask)
		delete [] mpBucketMask;
	mpBucketMask = NULL;
	
	if (mpUnitBitOffset)
		delete [] mpUnitBitOffset;
	mpUnitBitOffset = NULL;
	
	if (mpWildCardUnits)
		delete [] mpWildCardUnits;
	mpWildCardUnits = NULL;
    
	if (!mbUseMappedFile)
    {
        if (mBuckets)
            delete [] mBuckets;
        mBuckets = NULL;
        
        if (mpNodeList)
        {
            delete [] mpNodeList;
            mpNodeList = NULL;
            
            int lReservedSize = (mnNodes + 1) * sizeof(_Node);
            MEM_GOVERNOR_RELEASE(lReservedSize, lReservedSize)
        }
    }
    
    releaseMMapMemory();
}



//## Other Operations (implementation)
int  DSSXTabHashTable::Init(int nBuckets, int nUnits, int *pUnitBucketSpace)
{
	mnBuckets = nBuckets;
	AE_ASSERT(mnBuckets > 0);
	//	mBuckets = new (mpBuffer) _Node* [mnBuckets + 1];
	//	CHECK_NEWED_PTR(mBuckets);
	//	memset(mBuckets, 0 , sizeof(_Node*) * (mnBuckets + 1));
	
	mBuckets = new unsigned int[mnBuckets + 1];
	CHECK_NEWED_PTR(mBuckets);
	memset(mBuckets, 0 , sizeof(unsigned int) * (mnBuckets + 1));
	
	mnUnits = nUnits;
    if(mnUnits == 0)
        return 0;
	// Veracode: CWE ID 195: Signed to Unsigned Conversion Error
    AE_ASSERT(mnUnits > 0);
    // Veracode
	mpBucketSpace = new int[mnUnits];
	CHECK_NEWED_PTR(mpBucketSpace);
	memmove(mpBucketSpace, pUnitBucketSpace , sizeof(int) * mnUnits);
	
	mpBucketMask = new  int[mnUnits];
	CHECK_NEWED_PTR(mpBucketMask);
	
	int iUnit = 0;
	for (iUnit = 0; iUnit < mnUnits; iUnit ++)
		mpBucketMask[iUnit] = (1 << mpBucketSpace[iUnit]) - 1;
	
	mpUnitBitOffset = new int[mnUnits];
	CHECK_NEWED_PTR(mpUnitBitOffset);
	
	if (mnUnits > 0)
		mpUnitBitOffset[mnUnits - 1] = 0;
	
	for (iUnit = mnUnits - 2; iUnit >= 0; iUnit --)
		mpUnitBitOffset[iUnit] = mpBucketSpace[iUnit + 1] + mpUnitBitOffset[iUnit + 1];
	
	//For example, there are 3 attributes A1(0..15), A2(0..9), A3(0..3) and nRow = 100
	//then lBucketSpace = 7, mnBuckets = 128, lpUnitKeySpace[] = (4, 4, 2)
	// mpBucketMask[] = (7, 7, 1), mpBucketSpace[] = (3, 3, 1), mpUnitBitOffset[] = (4, 1, 0)
	
	//pre-allocate the wildcard unit array
	mpWildCardUnits = new int[mnUnits];
	
	return 0;
}



//There is no consideration for redundant keys.
//put tuple (pkey, value) into hashtable
/*
 void DSSXTabHashTable::Hash(int *pKey, int value){
 
 //this function should not be called
 _ASSERTE(false);
 
 unsigned int lBucket = HashFunc(pKey);
 _ASSERTE(lBucket < mnBuckets);
 
 _Node *lNode = new (mpBuffer) _Node(value, pKey, mBuckets[lBucket]);
 mBuckets[lBucket] = lNode;
 
 }
 */

//use pKey to look up hash table, return value
//return true if found
//		  false if not found
bool DSSXTabHashTable::LookUp(int *pKey, int * ipvalue){
	unsigned int lBucket = HashFunc(pKey);
	
	_Node * start = mpNodeList + mBuckets[lBucket];
	_Node * end = mpNodeList + mBuckets[lBucket+1];
	end --;
	
	//linear search
	while (start <= end)
	{
		//compare key
		int *pKey1 = pKey;
		//int *pKey2 = start->mpKey;
        // int *pKey2 = mpNodeKeys + start->mValue*mnUnits;
        int *pKey2 = mpNodeKeys + *start*mnUnits;
        
		int i = 0;
		for (i = -mnUnits; i < 0; i ++) {
			if (*pKey1++ != *pKey2++)
				break;
		}
		
		if(i >= 0)
		{
			//key matched, return mValue
			// *ipvalue = start->mValue;
            *ipvalue = *start;
			return true;
		}
		//key not matched, check the next one
		start++;
	}
	return false;
}
//*/

//pKey is a patial key, multiple values may be returned
//if pKey[iUnit] < 0 then pKey[iUnit] is a wild card key
//all matched values will be put into opVaule, nRow is the length of opValue
//if nRow is too small for opValue, then -1 will be returned.
//otherwise, return the number of values matched with the partialKey
int DSSXTabHashTable::PartialMatchLookUp(int *pKey, int nRow, int *opValue){
	int lBaseBucket = 0;
	int lnWildCardUnits = 0;
	int iUnit = 0;
	for (iUnit = 0; iUnit < mnUnits; iUnit ++){
		if (pKey[iUnit] != -100) //normal key
			lBaseBucket |= (pKey[iUnit] & mpBucketMask[iUnit]) << mpUnitBitOffset[iUnit];
		else //wildcar key
			mpWildCardUnits[lnWildCardUnits ++] = iUnit;
	}
	
	//calculate the number of buckets in wild card match
	int lnIterations = 1;
	for (iUnit = 0; iUnit < lnWildCardUnits; iUnit ++){
		lnIterations <<= mpBucketSpace[mpWildCardUnits[iUnit]];
	}
	
	int rVal = 0;
	//iterate through each relevant buckets to find out patially matched keys
	for (int i = 0; i < lnIterations; i ++){
		//calculate bucket address
		int lBucket = lBaseBucket;
		int num = i;
		for (iUnit = lnWildCardUnits - 1; iUnit >= 0; iUnit --){
			lBucket |= (num & mpBucketMask[mpWildCardUnits[iUnit]]) << mpUnitBitOffset[mpWildCardUnits[iUnit]];
			num >>= mpBucketSpace[mpWildCardUnits[iUnit]];
		}
		
		AE_ASSERT(lBucket < mnBuckets);
		_Node * lNode = mpNodeList + mBuckets[lBucket];
		_Node * lNodeEnd = mpNodeList + mBuckets[lBucket + 1];	//Note: it's valid since mBuckets[mnBuckets] exists
		while(lNode < lNodeEnd) {
            // int* lpKeys = mpNodeKeys + lNode->mValue*mnUnits;
            int* lpKeys = mpNodeKeys + *lNode*mnUnits;
            if(IsKeyPartialEqual(pKey, lpKeys)){
                //if(IsKeyPartialEqual(pKey, lNode->mpKey)) {
				if (rVal >= nRow)
					return -1;  // no enough space
				
				// opValue[rVal ++] = lNode->mValue;
                opValue[rVal ++] = *lNode;
				
			}
			
			//			lNode = lNode->mNext;
			lNode++;
		} //while
	} //for
	
	return rVal;
	
}

//construct the hashtable in one step by passing in all keys in ascending order(ibAscSorted = true)
//or un-sorted(ibAscSorted = false).
int DSSXTabHashTable::HashAll(int *pKey, int nUnits, int nRows)
{
    // Veracode: CWE ID 191: Integer Underflow (Wrap or Wraparound)
	if (nRows < 0) nRows = 0;
    // Veracode
	AE_ASSERT(nUnits == mnUnits);
	
	//create contract manager to govern temporary memory allocation.
    /*	MBase::Buffer* lpLocalBuffer = MContractMgr::CreateGovernedBuffer(1, 1, _T("DSSXTabHashTable"));
     if(!lpLocalBuffer)	// if unsuccessful, we assume it is 2-tier and we create normal buffer
     lpLocalBuffer = MBase::CreateBuffer(1, 1);
     //add a smart pointer
     MBase::BufferPtr lBufferMCMPtr(lpLocalBuffer);
     */
	unsigned int *lpHashResultList = NULL;
	int lReservedSize = nRows * sizeof(unsigned int);
    MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)
	lpHashResultList = new unsigned int[nRows];
	CHECK_NEWED_PTR(lpHashResultList);
    MEM_GOVERNOR_COMMIT(lReservedSize)
	memset(lpHashResultList, 0, sizeof(unsigned int) * nRows);
	
	int * lpKey = pKey;
	unsigned int *lpHashVal = lpHashResultList;
	unsigned int *lpBucketNodeCount = mBuckets;	//NOTE: the length of mBuckets is mnBuckets + 1;
	
	//save the hash value of each, and save the node number of each bucket to mBucket
	int i = 0;
	for(i = -nRows; i < 0; ++i)
	{
		*lpHashVal = HashFunc(lpKey);
		AE_ASSERT(*lpHashVal < mnBuckets);
		
		lpBucketNodeCount[*lpHashVal]++;
		lpKey += nUnits;
		lpHashVal++;
	}
	
	//calculate the offset of each bucket. mBuckets saves the end position of each bucket
	unsigned int *lpEndPos = mBuckets;
	lpEndPos++;						//lpEndPos[0] = mBucket[0];
	for (i = - mnBuckets; i < -1; ++i)
	{
		*lpEndPos += *(lpEndPos - 1);	//lpEndPos[i] = sum(mBucket[0]...mBucket[i - 1]);
		lpEndPos++;
	}
	*lpEndPos = *(lpEndPos - 1);	//lpEndPos[mnBuckets] = lpEndPos[mnBuckets - 1]
	//the extra node (mBuckets[mnBuckets]) also save the end position of the last bucket
    
	lReservedSize = (nRows + 1) * sizeof(_Node);
	{
		//there is local memory allocation need released.
		int hr = S_OK;
        MEM_GOVERNOR_RESERVE_LOCAL(lReservedSize, lReservedSize)
		if (hr != S_OK)
		{
			if (lpHashResultList)
			{
				delete [] lpHashResultList;
				int lReservedSize = nRows * sizeof(unsigned int);
                MEM_GOVERNOR_RELEASE(lReservedSize, lReservedSize)
			}
			return hr;
		}
        mpNodeList = new _Node[nRows + 1];
        CHECK_NEWED_PTR(mpNodeList);
        MEM_GOVERNOR_COMMIT(lReservedSize)
	}
	
	_Node * const lpHeader = mpNodeList;
	
    mpNodeKeys = pKey;
	lpKey = pKey;
	lpHashVal = lpHashResultList;
	_Node* lpNode = NULL;
	for(i = 0; i < nRows; ++i)
	{
		//for each bucket, insert the node from back to front.
		//evantually mBucket[i] becomes the buckets's start position.
		lpNode = lpHeader - 1 + mBuckets[*lpHashVal]--;
		//set the node value
		//lpNode->mpKey = lpKey;
		// lpNode->mValue = i;
		*lpNode = i;
		lpKey += nUnits;
		lpHashVal++;
	}
	
	mnNodes = nRows;
	
	if (lpHashResultList)
	{
		delete [] lpHashResultList;
		int lReservedSize = nRows * sizeof(unsigned int);
        MEM_GOVERNOR_RELEASE(lReservedSize, lReservedSize)
	}
	
	return 0;
}

//LookUpEx is faster than LookUp by skipping the last key comparison.
//Don't use LookUpEx() unless it's guaranteed that pKey is always a valid key (has an entry in the hash table). Use LookUp() instead
bool DSSXTabHashTable::LookUpEx(int *pKey, int *value){
	
	unsigned int lBucket = HashFunc(pKey);
	
	_Node * start = mpNodeList + mBuckets[lBucket];
	_Node * end = mpNodeList + mBuckets[lBucket+1];	//note: length of mBuckets is mnBuckets + 1
	end --;
	
	//linear search
	while (start < end)		//if only one node left, we'll return its value without key comparison
	{
		//compare keys
		int *pKey1 = pKey;
		//int *pKey2 = start->mpKey;
        // int *pKey2 = mpNodeKeys + start->mValue*mnUnits;
        int *pKey2 = mpNodeKeys + *start*mnUnits;
		int i = 0;
		for (i = -mnUnits; i < 0; i ++) {
			if (*pKey1++ != *pKey2++)
				break;
		}
		if(i >= 0)
		{
			//keys are matched, return mValue
			// *value = start->mValue;
            *value = *start;
			return true;
		}
		//keys are not matched, check the next node
		start++;
	}
	if (start == end)
	{
		// *value = start->mValue;
        *value = *start;
        
		return true;
	}
	return false;
}

int DSSXTabHashTable::CheckKeyValues(int nUnits, int nRows, int *pKeys,
                                     int *pUnitBucketSpace, int &oNumBuckets, int * lpUnitMaxKey, bool ibNeedCalMaxKey)
{
    //	mvMaxKeys.resize(nUnits>0? nUnits : 1);
    //	int* lpUnitMaxKey = &mvMaxKeys[0];
	
    if(ibNeedCalMaxKey)
    {
        for (int iRow = 0; iRow < nRows; iRow ++) {
            for (int iUnit = 0; iUnit < nUnits; iUnit ++) {
                if (lpUnitMaxKey[iUnit] < pKeys[nUnits * (size_t)iRow + iUnit])
                    lpUnitMaxKey[iUnit] = pKeys[nUnits * (size_t)iRow + iUnit];
            }
        }
    }
	
	// aye, optimization for 1 unit case
    /*	if (nUnits == 1)
     {
     int i = 0;
     int *lpKey = pKeys;
     for (; i < mnRows; i ++)
     {
     if (*lpKey++ != i)
     break;
     }
     
     if (i == mnRows)
     return eLinearHash;
     }*/
	
	//nBuckets must be power of 2 and >= nRow
	int lBucketSpace = NumBitSpace(nRows);
	oNumBuckets = 1 << lBucketSpace;
	
	memset(pUnitBucketSpace, 0, sizeof(int) * nUnits);
	
	//if (nUnits == 0 || nRows == 0)
    //return eAggregationPartialMatch;
    //return -1;
	
	//First, we have to get the unit key space for each unit
	int *lpUnitKeySpace = reinterpret_cast<int *>(malloc(sizeof(int) * nUnits));
	int lTotalKeySpace = 0;
	int iUnit = 0;
	for (iUnit = 0; iUnit < nUnits; iUnit ++) {
		lpUnitKeySpace[iUnit] = NumBitSpace(lpUnitMaxKey[iUnit] + 1);
		lTotalKeySpace += lpUnitKeySpace[iUnit];
	}
	
	//xhan 06/17/05
	//Total bit loss should be lTotalKeySpace - lBucketSpace, but because each calculated unit key space is greater
	// than the real unit key space, on average, it is 1.5 time of the real unit key space, for each unit key space
	// we minus 1/2 bit as the adjustment.
	// double lAdjustedTotalKeySpace = lTotalKeySpace - (double) nUnits / 2;
	//caculate average unit bit space loss
	// double lAvgBitLoss = (lAdjustedTotalKeySpace - lBucketSpace) / nUnits;
	// if (lAvgBitLoss < 0)
    // lAvgBitLoss = 0;
	
	memset(pUnitBucketSpace, 0 , sizeof(int) * nUnits);
	//Allocate bucket space to each unit according the relative ratio of their key spaces
	if (lTotalKeySpace > 0){
		for (iUnit = 0; iUnit < nUnits; iUnit ++) {
			pUnitBucketSpace[iUnit] = floor((double) lpUnitKeySpace[iUnit] / lTotalKeySpace * (size_t)lBucketSpace + 0.5);
			
			//adjust key space after each allocation
			lTotalKeySpace -= lpUnitKeySpace[iUnit];
			lBucketSpace -= pUnitBucketSpace[iUnit];
			
			if (lTotalKeySpace < 1)
				break;
			
		}
		
		//all bits should have been allocated
		AE_ASSERT(lBucketSpace == 0 && lTotalKeySpace == 0);
	}
	
	
	//find out the maximum bit loss for one unit
	int lMaxUnitBitLoss = 0;
	for (iUnit = 0; iUnit < nUnits; iUnit ++) {
		if (lpUnitKeySpace[iUnit] - pUnitBucketSpace[iUnit] > lMaxUnitBitLoss)
			lMaxUnitBitLoss = lpUnitKeySpace[iUnit] - pUnitBucketSpace[iUnit];
	}
    
	free(lpUnitKeySpace);
	
	return 0;
}

//return an offset so that 1 << rval >= num and rval is the smallest number
//for example
// input	output
// 0		0
// 1		0
// 2		1
// 7		3
int DSSXTabHashTable::NumBitSpace(int num){
	int rval = 0;
	unsigned int space = 1;
	
	while (num > space) {
		space <<= 1;
		rval ++;
	}
	
	return rval;
}

int DSSXTabHashTable::size()
{
	int lSize = sizeof(this);
	
	lSize += sizeof(_Node) * (mnNodes + 1);		//size of mpNodeList;
	
	lSize += sizeof(unsigned int) * (mnBuckets + 1);	//size of mBuckets;
	
	lSize += sizeof(int) * mnUnits * 4;		//size of mpBucketMask, mpBucketSpace, mpUnitBitOffset and mpWildCardUnits;
	
	return lSize;
}

int DSSXTabHashTable::createMMapMemory(const char *ipFilePath, bool ibUseNewFile)
{
    bool lbNeedDoMMap = NeedDoMMap(ipFilePath);
    if(!lbNeedDoMMap)
        return S_OK;
    
    mFilePath = ipFilePath;
    
    const void *lpDataPtr[2];
    int lpDataSize[2];
    
    lpDataPtr[0] = mpNodeList;
    lpDataSize[0] = (mnNodes + 1) * sizeof(_Node);
    
    lpDataPtr[1] = mBuckets;
    lpDataSize[1] = sizeof(int) * (mnBuckets + 1);      //	mBuckets = new unsigned int[mnBuckets + 1];
    
    int lDataSize = lpDataSize[0] + lpDataSize[1];
    
    int hr = DSSCMBlockFileUtil::AddBlockHeader(ipFilePath, mBlockType, lDataSize);
    CHECK_HR;
    
    void *lpBuffer = NULL;
    hr = DataSourceUtil::createMappedFileBuffer(2, lpDataPtr, lpDataSize, ipFilePath, &ibUseNewFile, (void**)&mpMMPBuffer, &mMMPBufSize, &lpBuffer, &mOffset, &mOffsetAligned);
    
    CHECK_HR;
    
    delete [] mpNodeList;
    mpNodeList = (_Node*)lpBuffer;
    
    delete [] mBuckets;
    mBuckets = (unsigned int *) ((char*)lpBuffer + lpDataSize[0]);
    
    MEM_GOVERNOR_RELEASE(lDataSize, lDataSize)
    
    mbUseMappedFile = true;
    
    return S_OK;
}

int DSSXTabHashTable::releaseMMapMemory()
{
    if(!(mbUseMappedFile && mFilePath != "" && mpMMPBuffer))
        return S_OK;
    
    int hr = DSSCMMemoryMap::releaseMMapMemory();
    CHECK_HR;
    
    mpNodeList = NULL;
    mBuckets = NULL;
    
    return S_OK;
}

int DSSXTabHashTable::reloadMMapMemory()
{
    if(!(mbUseMappedFile && mFilePath!="" && mpMMPBuffer==NULL))
        return S_OK;
    
    int hr = DSSCMMemoryMap::reloadMMapMemory();
    CHECK_HR;
    
    int *lpBuffer = reinterpret_cast<int*>(mpMMPBuffer + mOffset);
    mpNodeList = (_Node*)lpBuffer;
    mBuckets = (unsigned int *) ((char*)lpBuffer + (mnNodes + 1) * sizeof(_Node));
    
    return S_OK;
}

int DSSXTabHashTable::setNodeKeys(int *ipKeys)
{
    mpNodeKeys = ipKeys;
    return S_OK;
}

int DSSXTabHashTable::Init(int nBuckets, int nUnits, int *pUnitBucketSpace,int iRows, int* ipKeys)
{
    mnBuckets = nBuckets;
	AE_ASSERT(mnBuckets > 0);
	
	mnUnits = nUnits;
	
    // Veracode: CWE ID 195: Signed to Unsigned Conversion Error
    AE_ASSERT(mnUnits > 0);
    // Veracode
    
	mpBucketSpace = new int[mnUnits];
	CHECK_NEWED_PTR(mpBucketSpace);
	memmove(mpBucketSpace, pUnitBucketSpace , sizeof(int) * mnUnits);
	
	mpBucketMask = new  int[mnUnits];
	CHECK_NEWED_PTR(mpBucketMask);
	
	int iUnit = 0;
	for (iUnit = 0; iUnit < mnUnits; iUnit ++)
		mpBucketMask[iUnit] = (1 << mpBucketSpace[iUnit]) - 1;
	
	mpUnitBitOffset = new int[mnUnits];
	CHECK_NEWED_PTR(mpUnitBitOffset);
	
	if (mnUnits > 0)
		mpUnitBitOffset[mnUnits - 1] = 0;
	
	for (iUnit = mnUnits - 2; iUnit >= 0; iUnit --)
		mpUnitBitOffset[iUnit] = mpBucketSpace[iUnit + 1] + mpUnitBitOffset[iUnit + 1];
	
	//pre-allocate the wildcard unit array
	mpWildCardUnits = new int[mnUnits];
    
    mnNodes = iRows;
    mpNodeKeys = ipKeys;
    
    return S_OK;
}

int DSSXTabHashTable::GetRawSize()
{
    return size();
}