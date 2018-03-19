/*
 *  DSSTabularIndex.cpp
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#include "stdio.h"
#include "DSSTabularIndex.h"
#import "DSSDataSource.h"
#import "DSSTabularUnit.h"
#import "DSSObjectContext.h"
#import "DSSObjectInfo.h"
#import "DSSTabularAttribute.h"
#import "DSSAttributeFormWrappers.h"
#import "DSSAttributeFormWrapper.h"
#import "DSSAttribute.h"
#import "DSSDataColumn.h"
#import "DSSStrongPtr.h"
#import "DSSXTabBitVector.h"
#import "DSSRelation.h"
#import "DSSGUnitContainer.h"
#import "DSSDataSource.h"

#import "DSSCMNodeModel.h"

#import "DSSCMBlockFileUtil.h"

#include <algorithm>
#include <stdlib.h>

#define MAXKEY 500
#define MINRATIO 0.002

#define MMAPINFOSIZE 3

ElementIndex::ElementIndex():mpElementAddress(NULL),mpRows(NULL),mpElements(NULL),mpIndex(NULL)
{
    mBlockType = cmElementIndex;
}

ElementIndex::~ElementIndex()
{
    if(!mbUseMappedFile)
    {
        if(mpElements)
            delete [] mpElements;
        if (mpElementAddress)
        {
            delete [] mpElementAddress;
        }
        if (mpRows)
        {
            delete [] mpRows;
        }
        
        //zhyang, 767298
        if (mpIndex)
        {
            MEM_GOVERNOR_RELEASE(mpIndex->mnRows * sizeof(int), mpIndex->mnRows * sizeof(int))
        }
    }
    
    releaseMMapMemory();
}

int ElementIndex::Size()
{
    int lSize = 0;
    if(mpElements)
    {
        lSize += mnElements;
    }
    if(mpElementAddress)
    {
        lSize += mnElements+1;
    }
    if(mpRows)
    {
        lSize += mpIndex->mnRows;
    }
    return lSize;
}

int ElementIndex::createMMapMemory(const char * ipFilePath, bool ibUseNewFile)
{
    bool lbNeedDoMMap = NeedDoMMap(ipFilePath);
    if(!lbNeedDoMMap)
        return S_OK;
    
    mFilePath = ipFilePath;
    
    const void *lpDataPtr[3];
    int lpDataSize[3];
    
    lpDataPtr[0] = mpElements;
    lpDataSize[0] = mnElements * sizeof(int);
    
    lpDataPtr[1] = mpElementAddress;
    if(mpElementAddress)
        lpDataSize[1] = (mnElements + 1) * sizeof(int);
    else
        lpDataSize[1] = 0;
    
    lpDataPtr[2] = mpRows;
    lpDataSize[2] = mpIndex->mnRows * sizeof(int);
    
    int lDataSize = lpDataSize[0] + lpDataSize[1] + lpDataSize[2];
    
    int hr = DSSCMBlockFileUtil::AddBlockHeader(ipFilePath, mBlockType, lDataSize);
    CHECK_HR;
    
    void *lpBuffer = NULL;
    hr = DataSourceUtil::createMappedFileBuffer(3, lpDataPtr, lpDataSize, ipFilePath, &ibUseNewFile, (void**)&mpMMPBuffer, &mMMPBufSize, &lpBuffer, &mOffset, &mOffsetAligned);
    
    CHECK_HR;
    
    delete [] mpElements;
    mpElements = (int*)lpBuffer;
    
    if(mpElementAddress)
    {
        delete [] mpElementAddress;
        mpElementAddress = (int *)((char *)lpBuffer+ lpDataSize[0]);
    }
    
    delete [] mpRows;
    mpRows = (int *) ((char*)lpBuffer + lpDataSize[0] + lpDataSize[1]);
    
    MEM_GOVERNOR_RELEASE((mpIndex->mnRows * sizeof(int)), (mpIndex->mnRows * sizeof(int)))
    
    mbUseMappedFile = true;
    
    return S_OK;
}

int ElementIndex::releaseMMapMemory()
{
    if(!(mbUseMappedFile && mFilePath != "" && mpMMPBuffer))
        return S_OK;
    
    int hr = DSSCMMemoryMap::releaseMMapMemory();
    CHECK_HR;
    
    mpElements = NULL;
    mpElementAddress = NULL;
    mpRows = NULL;
    
    return S_OK;
}

int ElementIndex::reloadMMapMemory()
{
    if(!(mbUseMappedFile && mFilePath!="" && mpMMPBuffer==NULL))
        return S_OK;
    
    int hr = DSSCMMemoryMap::reloadMMapMemory();
    CHECK_HR;
    
    int *lpBuffer = reinterpret_cast<int*>(mpMMPBuffer + mOffset);
    mpElements = (int*)lpBuffer;
    
    int lDataSize = mMMPBufSize - mOffset;
    if(lDataSize == (mnElements + mnElements +1 +mpIndex->mnRows)*sizeof(int))
    {
        mpElementAddress = (int *) ((char*)lpBuffer + mnElements * sizeof(int));
        
        mpRows = (int *) ((char*)lpBuffer + (mnElements + mnElements + 1) * sizeof(int));
    }
    else if(lDataSize == (mnElements + mpIndex->mnRows)*sizeof(int))
    {
        mpRows = (int *) ((char*)lpBuffer + mnElements * sizeof(int));
    }
    else
    {
        return S_FALSE;
    }
    
    return S_OK;
}

int ElementIndex::GetRawSize()
{
    return Size();
}


DSSXTabKeyIterator::DSSXTabKeyIterator(DSSTabularIndex *ipIndex, DSSTabularIndex *ipTargetIndex): mpLocalKeys(NULL), mnTargetUnits(0),mpTargetIndex(ipTargetIndex), mpTargetUnits(NULL)
{
    mpKeys = ipIndex->mpKeys;
    // mIsIntegerKey = true;
    // mnBytesPerRow = sizeof(int) * ipIndex->mnUnits;
	mnSourceUnits = ipIndex -> mnUnits;
    
	if (ipTargetIndex)
	{
		mnTargetUnits = ipTargetIndex->mnUnits;
		mpTargetUnits = new int[mnTargetUnits];
		for (int i = 0; i < mnTargetUnits; i++)
		{
			// find the units in ipIndex that matches the target units
			mpTargetUnits[i] = 0;
			int j = 0;
			for (; j < ipIndex->mnUnits; j++)
			{
				if (ipIndex->mpUnits[j] == ipTargetIndex->mpUnits[i])
				{
					mpTargetUnits[i] = j;
					break;
				}
			}
		}
        
		mpLocalKeys = new int[mnTargetUnits];
	}
    
	SetRow(0);
}

int DSSXTabKeyIterator::LookupTargetRow(int iRow, int *opTargetRow)
{
	SetRow(iRow);
    
	int *lpLocalKeys = mpLocalKeys;
	int *lpTargetUnits = mpTargetUnits;
	for (int i = 0; i < mnTargetUnits; i++) {
		*lpLocalKeys++ = GetCol(*lpTargetUnits++);
	}
    
    // 3/31/2012 tehe: here if we can't find the corrospondint row, we will ignore it. for some case, a row in main index may be not found in another index, you can take the #568619 for an example
    int hr = S_OK;
    hr = mpTargetIndex->LookupActualKey( mpLocalKeys, opTargetRow);
    if(hr != S_OK)
    {
        *opTargetRow = -1;
    }
    return S_OK;
}

DSSXTabKeyIterator::~DSSXTabKeyIterator()
{
	if (mpLocalKeys)
		delete [] mpLocalKeys;
    
    mpLocalKeys = NULL;
    
	if (mpTargetUnits)
		delete [] mpTargetUnits;
    
    mpTargetUnits = NULL;
}

DSSTabularIndex::DSSTabularIndex():mpUnits(NULL),mpLookUpKeyBuf(NULL),mpKeys(NULL)
,mnRows(0), mnUnits(0),isMainIndexOrder(false),mIsDistinct(false),  mfSortByLastUnit(false),mpvElementIndices(NULL)
,mbPartialLoading(false),mpHashTable(NULL),mbUseMap(true)
{
    mBlockType = cmTabularIndex;
}

DSSTabularIndex::~DSSTabularIndex()
{
    // tehe: release mmap memory first.
    releaseMMapMemory();
    
    if(mpHashTable)
    {
        delete mpHashTable;
        mpHashTable  = NULL;
    }
	if (mpUnits)
	{
		delete[] mpUnits;
	}
	if (mpLookUpKeyBuf)
	{
		delete[] mpLookUpKeyBuf;
	}
	if (mpKeys&&!mbUseMappedFile)
	{
		delete[] mpKeys;
        if(mbPartialLoading)
            mnRows = mvRowList.size();
        MEM_GOVERNOR_RELEASE(mnUnits * mnRows * sizeof(int), mnUnits * mnRows * sizeof(int))
	}
	mpKeys = mpUnits = mpLookUpKeyBuf = NULL;
    
    for(int i = 0; i < mvKeyDistributions.size(); ++i)
    {
        if(mvKeyDistributions[i])
        {
            delete [] mvKeyDistributions[i];
        }
    }
    
    // tehe 4/19/2012: memory leak
    ClearElementIndex();
}

int DSSTabularIndex::Count(int* opCount)
{
    if(opCount)
    {
        *opCount = mnRows;
    }
    return S_OK;
}

//xiazhou, 913807, 07/03/2014. Expose API to clear element index. Used in Offline UC(DDIC).
int DSSTabularIndex::ClearElementIndex()
{
    if(mpvElementIndices)
    {
        for (int i=0;i<mpvElementIndices->size(); i++) {
            if((*mpvElementIndices)[i])
            {
                delete (*mpvElementIndices)[i];
                (*mpvElementIndices)[i] = NULL;
            }
        }
        delete mpvElementIndices;
        mpvElementIndices = NULL;
    }
    
    return S_OK;
}

// tehe: the implements is different from server side, I am not sure it is right.
bool DSSTabularIndex::IsSameOrder(DSSTabularIndex* ipIndex)
{
    if (!ipIndex)
        return false;
    
    if (this == ipIndex)
        return true;
    
    // since we use seperate buffer for indices, here we should compare the parentBuffer
	if (isMainIndexOrder && ipIndex->isMainIndexOrder)
		return true;
    
	if (mnRows == ipIndex->mnRows
		&& mfSortByLastUnit
		&& ipIndex->mfSortByLastUnit
		&& mpUnits[mnUnits-1] == ipIndex->mpUnits[ipIndex->mnUnits - 1])
		return true;
    
	return false;
}

bool DSSTabularIndex::IsUnqualified(int iRow)
{
    assert(iRow < mnRows);
    return mUnqualifiedRows.Get(iRow);
}

DSSXTabBitVector* DSSTabularIndex::GetUnqualifiedRows()
{
	return &mUnqualifiedRows;
}

DSSXTabBitVector* DSSTabularIndex::GetHiddenRows()
{
    return &mHiddenRows;
}

int DSSTabularIndex::SortWithMask(int nUnits, int * ipUnitPos, DSSXTabBitVector* ipMask, int** oppSortedRowMap)
{
    int hr = S_OK;
    
	int * lpMaxUnitValue = new int[nUnits];
	CHECK_NEWED_PTR(lpMaxUnitValue);
    MBase::DSSStrongPtr<int> lpMaxUnitValueHolder(lpMaxUnitValue);
    
	int i;
	for (i = 0; i < nUnits; ++i)
		lpMaxUnitValue[i] = mvMaxKeys[ipUnitPos[i]];
    
	//int lnCount = 0;
    
	int lMaxUnitValue = -1;
	for (i = 0; i < nUnits; ++i) {
		if (lpMaxUnitValue[i] > lMaxUnitValue)
			lMaxUnitValue = lpMaxUnitValue[i];
	}
	int * lpCount = new int[lMaxUnitValue + 1];
	CHECK_NEWED_PTR(lpCount);
    MBase::DSSStrongPtr<int> lpCountHolder(lpCount);
    
	//if mask, re cacluate the key distribution
	//DSSXTabKeyIterator lIt(this);
    
	std::vector<int*> lvKeyDistributions;
	if (ipMask)
	{
		for (i = 0; i < nUnits; i++)
		{
			int *lpDist = new int[ lpMaxUnitValue[i] + 1];
			memset(lpDist, 0, sizeof(int) * (lpMaxUnitValue[i]+1));
			lvKeyDistributions.push_back(lpDist);
            
			ipMask->SetSeekPosition(0);
			int j = ipMask->GetNextSetPosition();
            
			while (j >=0 && j < mnRows)
			{
				int lKey = getKey(j, ipUnitPos[i]);
				lpDist[lKey]++;
				j = ipMask->GetNextSetPosition();
			}
            
			for (j = 1; j <= lpMaxUnitValue[i]; j++)
			{
				lpDist[j] = lpDist[j] + lpDist[j-1];
			}
		}
        
	}
    
	if (ipMask)
		AE_ASSERT(ipMask->GetLength() == mnRows);
    
	int lnRows;
	if (ipMask)
		lnRows = ipMask->GetSetSize();
	else
		lnRows = mnRows;
    
	int *lpSrc = NULL;
	int *lpDest = NULL;
    
	lpSrc = new int[lnRows];
	CHECK_NEWED_PTR(lpSrc);
    
	// Array of rows sorted
	lpDest = new int[lnRows];
	CHECK_NEWED_PTR(lpDest);
    
	// Init
	if (ipMask)
	{
		ipMask->SetSeekPosition(0);
		int j = ipMask->GetNextSetPosition();
        
		int iRow = 0;
		while (j >=0 && j < mnRows && iRow >=0 && iRow < lnRows)
		{
			lpSrc[iRow++] = j;
			j = ipMask->GetNextSetPosition();
		}
	}
	else
	{
		for (i = 0; i < lnRows; ++i)
			lpSrc[i] = i;
	}
    
    if(mvKeyDistributions.size() != mnUnits)
        hCalcKeyDistributions();
    
	for (i = nUnits - 1; i >= 0; --i) {
        
		if (ipMask)
			memmove(lpCount, lvKeyDistributions[i], (lpMaxUnitValue[i] + 1) * sizeof(int));
		else
			memmove(lpCount, mvKeyDistributions[ipUnitPos[i]], (lpMaxUnitValue[i] + 1) * sizeof(int));
        
		//int *lpKeyStart = ippKeys[i];
		int j;
		for (j = lnRows - 1; j >= 0; --j) {
			int lSortedRow = lpSrc[j];			// 1st memory load
			//use Iterator to get key
			int lKey = getKey(lSortedRow, ipUnitPos[i]);
			//int lKey = lpKeyStart[lSortedRow];	// 2nd memory load
			int *lpListLast = &lpCount[lKey];
			int lCount = (*lpListLast) - 1;		// 3rd memory load
			lpDest[lCount] = lSortedRow;			// 1st memory write
			*lpListLast = lCount;					// 2nd memory write
		}
        
 		// Swap lpSrc and lpDest
		int  * lpSwap = lpDest;
		lpDest = lpSrc;
		lpSrc = lpSwap;
	} // for
    
	*oppSortedRowMap = lpSrc;
    
    for (int i = 0; i < lvKeyDistributions.size(); i++)
        delete lvKeyDistributions[i];
    
    lvKeyDistributions.clear();
    
    delete lpDest;
    
	return hr;
}

int DSSTabularIndex::Init(int nUnits, int nRows, int *pUnits, int *ipKeys, DSSDataModelXTabImpl *ipTabularData,bool ibDelayLoad)
{
    if (nUnits < 0 || nRows < 0) {
        return E_INVALIDARG;
    }
    
	mnRows = nRows;
	mnUnits = nUnits;
    mpTabularData = ipTabularData;
	
    // Veracode: CWE ID 195: Signed to Unsigned Conversion Error
    signed long long tmp = (signed long long) nUnits;
    if (tmp > 0 && tmp <= INT_MAX/sizeof(int)) {
        mpUnits = new int[mnUnits];
        CHECK_NEWED_PTR(mpUnits);
        memmove(mpUnits, pUnits, sizeof(int) * nUnits);
        
        mpLookUpKeyBuf = new int[mnUnits];
        CHECK_NEWED_PTR(mpLookUpKeyBuf);
    }
    // Veracode
    
    mUnqualifiedRows.Init(mnRows, false); //zhyang, move it out of Veracode
    mHiddenRows.Init(mnRows, false);
    if (mnRows > 0) {
        mHiddenRows.BulkSet(0, mnRows -1, true);
    }
    
    //first clear previous loaded keys
    if(mpKeys && !mbUseMappedFile)
    {
        //fali, release the memory of mpKeys
        MEM_GOVERNOR_RELEASE(nUnits * nRows * sizeof(int), nUnits * nRows * sizeof(int));
        delete [] mpKeys;
        mpKeys = NULL;
        MEM_GOVERNOR_RELEASE(mnUnits * mvRowList.size() * sizeof(int), mnUnits * mvRowList.size() * sizeof(int))
        mvRowList.clear();
        mMapRow2PartialIndex.clear();
        
        mvMapRow2PartialIndex.clear();
    }
    
    if(!ibDelayLoad)
    {
        mbPartialLoading = false;
        
        if(ipKeys)
        {
            int lReservedSize = nUnits * nRows * sizeof(int);
            MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)
            // Veracode: CWE ID 190: Interger Overflow or Wraparound
            if (nUnits > 0 && nRows > 0 && INT_MAX/nUnits/sizeof(int) >= nRows)
            {
                mpKeys = new int[nUnits * nRows];
                CHECK_NEWED_PTR(mpKeys);  //zhyang, 815127, move the check inside if statement
            }
            // Veracode
            
            MEM_GOVERNOR_COMMIT(lReservedSize)
            memmove(mpKeys, ipKeys, sizeof(int) * nUnits * nRows);
            
            //fali, release the memory of ipKeys
            MEM_GOVERNOR_RELEASE(lReservedSize, lReservedSize);
            // tehe, key distribution depend on max key
            hCalcMaxKey();
            
            // 9/25/2012 tehe
            hCalcKeyDistributions();
        }
        
        // 9/5/2012 tehe
        mvElementCounts.resize(mnUnits, -1);
    }
    else
    {
        mbPartialLoading = true;
    }
    
    
	return 0;
}

void DSSTabularIndex::setKeys(int *ipKeys, int length)
{
    _ASSERT(length == mnUnits * mnRows);
    
    mpKeys = ipKeys;
    
    // tehe, key distribution depend on max key
    hCalcMaxKey();
    
    //msun, make it on demand
    //hCalcKeyDistributions();
}

void DSSTabularIndex::hCalcMaxKey()
{
    mvMaxKeys.resize(mnUnits);
    
    for (int iRow = 0; iRow < mnRows; iRow ++) {
        for (int iUnit = 0; iUnit < mnUnits; iUnit ++) {
            if (mvMaxKeys[iUnit] < mpKeys[mnUnits * (size_t)iRow + iUnit])
                mvMaxKeys[iUnit] = mpKeys[mnUnits * (size_t)iRow + iUnit];
        }
    }
}

int DSSTabularIndex::hCreateHashTable()
{
    int hr = S_OK;
    
    //zhyang, 31/08/2012, maxkey optimization
    bool ibNeedCalMaxKey = false;
    if(mvMaxKeys.size() == 0)
    {
        ibNeedCalMaxKey = true;
        mvMaxKeys.resize(mnUnits);
    }
    
    vector<int> lvBucketSpace(mnUnits);
    int lNumBuckets = 0;
    int *lpUnitBucketSpace = NULL;
    int *lpUnitMaxKey = NULL;
    
    if (mnUnits > 0)
    {
        //memset(&mvMaxKeys[0], 0, sizeof(int)*nUnits);
        lpUnitBucketSpace = &lvBucketSpace[0];
        lpUnitMaxKey = &mvMaxKeys[0];
    }
    
    if(mpHashTable)
    {
        delete mpHashTable;
        mpHashTable = NULL;
    }
    
    DSSXTabHashTable *lpHashTable = new DSSXTabHashTable();
    MBase::StrongPtr<DSSXTabHashTable> lpHolder(lpHashTable);
    
    hr = lpHashTable->CheckKeyValues(mnUnits, mnRows, mpKeys, lpUnitBucketSpace, lNumBuckets, lpUnitMaxKey, ibNeedCalMaxKey);
    CHECK_HR;
    
    hr = lpHashTable->Init(lNumBuckets, mnUnits, lpUnitBucketSpace);
    CHECK_HR;
    
    hr = lpHashTable->HashAll(mpKeys, mnUnits, mnRows);
    CHECK_HR;
    
    mpHashTable = lpHashTable;
    lpHolder.GiveUp();
    
    return hr;
}

// jimzhang, Sep/14/2012, at current, this overloaded Init() is only called by join logic in FG (DSSMultipleLevelJoinHelper and DSSJoinUtility).
//      In FG's join logic, only mnRows,mnUnits,mpUnits and mpKeys are needed in the whole lifecycle of index.
int DSSTabularIndex::Init(int inUnits, int inRows, int *ipUnits, int *ipKeys)
{
    if (inUnits < 0 || inRows < 0) {
        return E_INVALIDARG;
    }
    
    mnRows = inRows;
	mnUnits = inUnits;
    // Veracode: CWE ID 195: Signed to Unsigned Conversion Error
    signed long long tmp = (signed long long) inUnits;
    if (tmp > 0 && tmp <= INT_MAX) {
        mpUnits = new int[mnUnits];
        CHECK_NEWED_PTR(mpUnits);
        memmove(mpUnits, ipUnits, sizeof(int) * inUnits);
    }
    //yimliu, 911494, integer overflow.
    //Originally code.
    /*
    // Veracode
    int lnKeys = inUnits * inRows;
    int lReservedSize = lnKeys * sizeof(int);
    MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)
    // Veracode: CWE ID 190: Integer Overflow or Wraparound
    if (inUnits > 0 && inRows > 0 && INT_MAX/inUnits >= inRows)
    {
        mpKeys = new int[lnKeys];
        CHECK_NEWED_PTR(mpKeys); //zhyang, 815127, move the check inside if statement
    }
    // Veracode
    MEM_GOVERNOR_COMMIT(lReservedSize);
    */
    
    //Modified code.
    signed long long int lnKeys = inUnits * inRows;
    signed long long int lReservedSize = lnKeys * sizeof(int);
    if (inUnits > 0 && inRows > 0 && INT_MAX >= lReservedSize)
    {
        MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize);
        mpKeys = new int[lnKeys];
        CHECK_NEWED_PTR(mpKeys);
        MEM_GOVERNOR_COMMIT(lReservedSize);
    }
    else if(INT_MAX < lReservedSize)
    {
        return E_OUT_OF_MEMORY;
    }
    
    memmove(mpKeys, ipKeys, sizeof(int) * lnKeys);
    
    return S_OK;
}

int DSSTabularIndex::setHeaderKeys(vector<int>& ivKeys, vector<int>& ivRowList)
{
    int hr = S_OK;
    
    //first clear previous loaded keys
    if(mpKeys)
    {
        delete[] mpKeys;
        int lnRows = mnRows;
        if(mvRowList.size() > 0)
            lnRows = mvRowList.size();
        MEM_GOVERNOR_RELEASE(mnUnits * lnRows * sizeof(int), mnUnits * lnRows * sizeof(int))
        mvRowList.clear();
        mMapRow2PartialIndex.clear();
        
        mvMapRow2PartialIndex.clear();
    }
    
    mbPartialLoading = true;
    int nRows = ivRowList.size();
    int lReservedSize = mnUnits * nRows * sizeof(int);
    MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)
    mpKeys = new int[mnUnits * nRows];
    CHECK_NEWED_PTR(mpKeys);
    MEM_GOVERNOR_COMMIT(lReservedSize)
    memmove(mpKeys, &ivKeys[0], sizeof(int) * mnUnits * nRows);
    
    mvRowList.assign(ivRowList.begin(), ivRowList.end());
    
    //junwang, 2012/07/25
    if(nRows>mnRows/10)
    {
        mvMapRow2PartialIndex.resize(mnRows,-1);
        for (int i=0; i<ivRowList.size(); i++) {
            mvMapRow2PartialIndex[ivRowList[i]] = i;
        }
        mbUseMap = false;
    }
    else
    {
        for(int i = 0; i < ivRowList.size(); i++)
        {
            mMapRow2PartialIndex[ivRowList[i]] = i;
        }
        mbUseMap = true;
    }
    
    
    mvMaxKeys.resize(mnUnits);
    
    vector<int> lvBucketSpace(mnUnits);
    int lNumBuckets = 0;
    int *lpUnitBucketSpace = NULL;
    int *lpUnitMaxKey = NULL;
    
    if (mnUnits > 0)
    {
        memset(&mvMaxKeys[0], 0, sizeof(int)*mnUnits);
        lpUnitBucketSpace = &lvBucketSpace[0];
        lpUnitMaxKey = &mvMaxKeys[0];
    }
    
    if(mpHashTable)
    {
        delete mpHashTable;
        mpHashTable = NULL;
    }
    
    DSSXTabHashTable *lpHashTable = new DSSXTabHashTable();
    MBase::StrongPtr<DSSXTabHashTable> lpHolder(lpHashTable);
    
    hr = lpHashTable->CheckKeyValues(mnUnits, nRows, mpKeys, lpUnitBucketSpace, lNumBuckets, lpUnitMaxKey);
    CHECK_HR;
    
    hr = lpHashTable->Init(lNumBuckets, mnUnits, lpUnitBucketSpace);
    CHECK_HR;
    
    hr = lpHashTable->HashAll(mpKeys, mnUnits, nRows);
    CHECK_HR;
    
    mpHashTable = lpHashTable;
    lpHolder.GiveUp();
    
    //hCalcKeyDistributions();
    
    return S_OK;
}

int DSSTabularIndex::getNumUnits()
{
	return mnUnits;
}

int DSSTabularIndex::getUnits(int nUnits, int *pUnits)
{
	if (nUnits < mnUnits)
		return E_INVALIDARG;
	memmove(pUnits, mpUnits, sizeof(int) * nUnits);
	return S_OK;
}

int DSSTabularIndex::getUnits(vector<int> &orvUnits)
{
	if (mnUnits > 0)
	{
		orvUnits.insert(orvUnits.end(), mpUnits, mpUnits + mnUnits);
	}
	return S_OK;
}

//get keys
int DSSTabularIndex::Item(int iRow, int nKeys, int *opKey)
{
	int hr = ValidateItemEx(nKeys);
	CHECK_HR;
	return ItemEx(iRow, nKeys, opKey);
}

int DSSTabularIndex::ValidateItemEx(int nUnits)
{
	int iUnit = 0;
	for(iUnit = 0; iUnit < mnUnits; iUnit++)
	{
		int lUnits = mpUnits[iUnit];
		if(lUnits < nUnits)
			continue;
		else // mpUnits[iUnit] >= nUnits
			return E_FAIL;
	}
	return S_OK;
}
int DSSTabularIndex::ItemEx(int iRow, int nKeys, int *opKey)
{
    if (iRow >= mnRows)
		return E_INVALIDINDEX;
    
    if(mbPartialLoading)
    {
        //junwang, 2012/07/25
        if(mbUseMap)
        {
            std::map<int, int>::iterator it = mMapRow2PartialIndex.find(iRow);
            if(it!=mMapRow2PartialIndex.end())
                iRow = it->second;
            else
                return E_INVALIDINDEX;
        }
        else
        {
            if(iRow<0||iRow>=mvMapRow2PartialIndex.size())
                return E_INVALIDINDEX;
            
            iRow = mvMapRow2PartialIndex[iRow];
            if(iRow==-1)
                return E_INVALIDINDEX;
        }
        
        /*
         std::map<int, int>::iterator it = mMapRow2PartialIndex.find(iRow);
         if(it!=mMapRow2PartialIndex.end())
         iRow = it->second;
         else
         return E_INVALIDINDEX;
         */
    }
    
	int *lpKey = mpKeys + iRow * mnUnits;
	int *lpOff = mpUnits;
	
	int i = -mnUnits;
	for (; i < 0; i++)
	{
		opKey[*lpOff++] = *lpKey++;
	}
	return S_OK;
}

int DSSTabularIndex::LookUp(int nKeys, int *ipKey, int *opRow)
{
	int hr = ValidateLookUpEx(nKeys);
	CHECK_HR;
	
	return LookUpEx(nKeys, ipKey, mpLookUpKeyBuf, opRow);
}

int DSSTabularIndex::ValidateLookUpEx(int nUnits)
{
	if(mnUnits > 0)
	{
		if(nUnits <= mpUnits[mnUnits - 1])		//mpUnits is sorted
			return E_FAIL;
	}
	else
	{
		if(nUnits < 0)
			return E_FAIL;
	}
	return S_OK;
}

//Don't use LookUpEx() if pKey is NOT always logically valid (valid means ipKey exists).
//LookUpEx() will speed up the index lookup process. The caller should check parameters.
//ValidateLookUpEx() should be calledbefore LookUpEx().
int DSSTabularIndex::LookUpEx(int nUnits, int *ipKey, int *pKeyBuffer, int *opRow)
{
	int *lpKey = pKeyBuffer;
	int *lpUnits = mpUnits;
    
    if(!mpUnits && mnRows > 1)
    {
        //931 i-server
        return S_OK;
    }
    
	int i = -mnUnits;
	for(; i < 0; i ++)
		*lpKey++ = ipKey[*lpUnits++];
	
	int lRow = 0;
    
    // tehe
    if(mpHashTable == NULL)
    {
        int hr = hCreateHashTable();
        CHECK_HR_PTR(mpHashTable);
    }
    
	if (mpHashTable && mpHashTable->LookUp(pKeyBuffer, &lRow))
	{
        if(mbPartialLoading)
        {
            if(lRow < mvRowList.size())
                lRow = mvRowList[lRow];
            else
                return S_FALSE;
        }
        *opRow = lRow;
		return S_OK;
	}
	return S_FALSE;
}

int DSSTabularIndex::GenDistinctC(vector<int> &ivUnits, DSSTabularIndex *pIndices, int *opRowMap)
{
    int hr = S_OK;
    
    int lnUnits = ivUnits.size();
    
    //zhyang, 02/08/2011, we need to specify the vector size;
    //vector<int> lvPos;
    vector<int> lvPos(lnUnits);
    
    int *lpPos = &lvPos[0];
    
    pIndices->mIsDistinct = true;
    
    hr = hConvertUnitID(ivUnits, lvPos);
    CHECK_HR;
    
    //Currently we don't support ElemntBlock, maybe we will support it later
    //fali, also reserve space for new int[lnDistinct] and new int[lnUnits * lnDistinct].
    MEM_GOVERNOR_RESERVE(lnUnits * mnRows * sizeof(int) * 2 + mnRows, lnUnits * mnRows * sizeof(int) * 2 + mnRows)

    int *lpTempKeys = new int[lnUnits * mnRows];
    CHECK_NEWED_PTR(lpTempKeys);
    
    MEM_GOVERNOR_COMMIT(lnUnits * mnRows * sizeof(int) * 2 + mnRows);
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lTempKeyPtr(lpTempKeys);
    
    //yimliu, 2014/06/25, 918611, When this object deconstructs, it calls MEM_GOVERNOR_RELEASE.
    DSSMemoryGovernorHelper lTempKeyHelper(lnUnits * mnRows * sizeof(int));
    
    for(int i = 0; i < mnRows; ++i)
    {
        int *lpDest = lpTempKeys + i;
        int *lpSrc = &mpKeys[i * mnUnits];
        for(int j = 0; j < lnUnits; ++j)
        {
            *lpDest = *(lpSrc + lpPos[j]);
            lpDest += mnRows;
        }
    }
    
    int lnDistinct = mnRows;
    
    int *lpDistinct = new int[lnDistinct];
    CHECK_NEWED_PTR(lpDistinct);
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lDistinctPtr(lpDistinct);
    
    vector<int*> lvKeyColumns(lnUnits);
    for(int i = 0; i < lnUnits; ++i)
    {
        lvKeyColumns[i] = lpTempKeys + i * mnRows;
    }
    
    hr = Collapse(lnUnits, lpPos, &lvKeyColumns[0], lpDistinct, lnDistinct, opRowMap, false);
    
    if(S_OK != hr)
    {
        return hr;
    }
    
    if(!pIndices)
    {
        return hr;
    }
    
    int *lKey = new int[lnUnits * lnDistinct];
    CHECK_NEWED_PTR(lKey);
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lKeyPtr(lKey);
    
    bool lKeepOrder = false;
    if(mnRows == lnDistinct && lnUnits != 1)
    {
        if(opRowMap)
        {
            for(int i = 0; i < mnRows; ++i)
            {
                opRowMap[i] = i;
            }
        }
        lKeepOrder = true;
    }
    
    for(int j = 0; j < lnUnits; ++j)
    {
        int *lpSrc = &lpTempKeys[j * mnRows];
        int *lpDest = &lKey[j];
        
        if(lKeepOrder)
        {
            for(int i = 0; i < lnDistinct; ++i)
            {
                *lpDest = *lpSrc++;
                lpDest += lnUnits;
            }
        }
        else
        {
            for(int i = 0; i < lnDistinct; ++i)
            {
                *lpDest = *(lpSrc + lpDistinct[i]);
                lpDest += lnUnits;
            }
        }
    }
    
    if(lKeepOrder)
    {
        pIndices->isMainIndexOrder = isMainIndexOrder;
    }
    
    if(1 == lnUnits)
    {
        pIndices->mfSortByLastUnit = true;
    }
    
    vector<int> lvBaseIndexUnits;
    hr = this->getUnits(lvBaseIndexUnits);
    CHECK_HR;
    
    int lnBaseIndexUnits = lvBaseIndexUnits.size();
    
    vector<int> lvMaxKey(lnUnits);
    for(int i = 0; i < lnUnits; ++i)
    {
        int j = 0;
        for(j = 0; j < lnBaseIndexUnits; ++j)
        {
            if(ivUnits[i] == lvBaseIndexUnits[j])
            {
                lvMaxKey[i] = this->getMaxKey(j);
                break;
            }
        }
        if(j == lnBaseIndexUnits)
        {
            assert(false);
        }
    }
    
    hr = pIndices->setMaxKeys(lvMaxKey);
    CHECK_HR;
   
    //fali, release the memory of lpTempKeys and lpDistinct
    MEM_GOVERNOR_RELEASE(lnUnits * mnRows * sizeof(int) + mnRows, lnUnits * mnRows * sizeof(int) + mnRows);
    
	return hr = pIndices->Init(lnUnits, lnDistinct, &ivUnits[0], lKey, mpTabularData);
}

int DSSTabularIndex::getKey(int iRow, int iUnit)
{
	if (iRow >= mnRows || iUnit >= mnUnits)
		return DSSXTAB_TOTAL_KEY;
    
    if(mbPartialLoading)
    {
        //junwang, 2012/07/25
        if (mbUseMap)
        {
            std::map<int, int>::iterator it = mMapRow2PartialIndex.find(iRow);
            if(it!=mMapRow2PartialIndex.end())
                iRow = it->second;
            else
                return DSSXTAB_TOTAL_KEY;
        }
        else
        {
            if(iRow<0||iRow>=mvMapRow2PartialIndex.size()||mvMapRow2PartialIndex[iRow]==-1)
                return DSSXTAB_TOTAL_KEY;
            
            iRow = mvMapRow2PartialIndex[iRow];
        }
        
        /*
         if(mMapRow2PartialIndex.find(iRow) != mMapRow2PartialIndex.end())
         iRow = mMapRow2PartialIndex[iRow];
         else
         return DSSXTAB_TOTAL_KEY;
         */
    }
    
	return *(mpKeys + iRow * mnUnits + iUnit);
}

int DSSTabularIndex::size()
{
	int lSize = sizeof(this);
    
    lSize += sizeof(int) * mnRows * mnUnits;	//size of mpKeys;
	
	lSize += sizeof(int) * mnUnits * 2;		//size of mpUnits and mpLookUpKeyBuf
	
	lSize += DSSAEHelper::SizeOfVector(mvMaxKeys);
	
	std::vector<int> mvMaxKeys;
	
    if(mpHashTable)
    {
        lSize += mpHashTable->size();
    }
    
	return lSize;
}

void DSSTabularIndex::UpdateTimeStamp()
{
    
}

int DSSTabularIndex::hConvertUnitID(vector<int> &ivUnits, vector<int> &ivPos)
{
    int hr = S_OK;
    
    int prev = 0;
    int i = 0;
    int j = 0;
    
    for(i = 0; i < ivUnits.size(); ++i)
    {
        for(j = prev; j < mnUnits; ++j)
        {
            if(ivUnits[i] == mpUnits[j])
            {
                ivPos[i] = j;
                break;
            }
        }
        
        if(j >= mnUnits)
        {
            return E_INVALIDARG;
        }
        else
        {
            prev = j + 1;
        }
    }
    
    return hr = S_OK;
}


int DSSTabularIndex::Collapse(int nUnits, int * ipUnitPos, int **ippKeys, int * pDistinct, int& iorSize,
                              int* opRowMap, bool iRawSortResult)
{
    int hr = S_OK;
    
	int i, j;
    
	// Special case
	if (mnRows == 0) {
		iorSize = 0;
		return hr = S_OK;
	}
	if (nUnits == 0) {
		assert(iorSize > 0);
		pDistinct[0] = 0;
		iorSize = 1;
        
		if (opRowMap)
		{
			for (i = 0; i < mnRows; i++)
			{
				if (iRawSortResult)
					opRowMap[i] = i;
				else
					opRowMap[i] = 0;
			}
		}
		return hr = S_OK;
	}
    
	if (mnRows > 100000)
    {
        //print the log here
    }
    
	// First we get the max number of offset for each input units.
	// For example, if the original units are [A, B, C] and the input units are
	// [B, C]. Suppose the range of A, B and C are [0..X], [0..Y] and [0..Z] respectively.
	// We need to at least know Y and Z. This information is necessary for radix
	// sorting and other optimization such as using a bitmap.
    
	// Reluctant to use _alloc because it might not be portable.
	int * lpMaxUnitValue = new int[nUnits];
	CHECK_NEWED_PTR(lpMaxUnitValue);
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lMaxUnitValuePtr(lpMaxUnitValue);
    
	for (i = 0; i < nUnits; ++i)
		lpMaxUnitValue[i] = mvMaxKeys[ipUnitPos[i]];
    
	//xhan Remove bitmap optimization, because bitmap generation can't guarantee the key order in the index
	int lnCount = 0;
    
	// Now we have to use radix sort (bucket sort).
	// The algorithm is based on <Introduction To Algorithm>
	// by T. H. Cormen et al.
    
	int lMaxUnitValue = -1;
	for (i = 0; i < nUnits; ++i) {
		if (lpMaxUnitValue[i] > lMaxUnitValue)
			lMaxUnitValue = lpMaxUnitValue[i];
	}
    
	int * lpCount = new int[lMaxUnitValue + 1];
	CHECK_NEWED_PTR(lpCount);
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lCountPtr(lpCount);
    
	// Depending on the number of the units, the result is stored in either lpSrc or lpDst
	// Array of rows to be sort
	int *lpSrc = NULL;
	int *lpDest = NULL;
    
	lpSrc = new int[mnRows];
	CHECK_NEWED_PTR(lpSrc);
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lSrcPtr(lpSrc);
    
	// Array of rows sorted
	lpDest = new int[mnRows];
	CHECK_NEWED_PTR(lpDest);
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lDestKeyPtr(lpDest);
    
	// Init
	for (i = 0; i < mnRows; ++i)
		lpSrc[i] = i;
    
	if (mnRows > 100000)
    {
		//LOG_MESSAGE(Engine, Perf, "END: Index Collapse Sort Setup (%1 rows, %2 --> %3 units) ", << mnRows << mnUnits << nUnits);
    }
    
	if (mnRows > 100000)
    {
		//LOG_MESSAGE_NP(Engine, Perf, "BEGIN: Index Collapse Sort");
    }
    
	for (i = nUnits - 1; i >= 0; --i) {
        
		//int * lpKey = NULL;
        
		if (mnRows > 100000)
        {
			//LOG_MESSAGE(Engine, Perf, "BEGIN: Sort Unit %1 Setup (%2 elements)", <<i << lpMaxUnitValue[i]);
        }
        
		// The following is faster but not good for debugging
        if(mvKeyDistributions.size() != mnUnits)
            hCalcKeyDistributions();
        
		memmove(lpCount, mvKeyDistributions[ipUnitPos[i]], (lpMaxUnitValue[i] + 1) * sizeof(int));
		if (mnRows > 100000)
        {
			//LOG_MESSAGE(Engine, Perf, "END: Sort Unit %1 Setup (%2 elements)", <<i << lpMaxUnitValue[i]);
        }
        
        
		if (mnRows > 100000)
        {
			//LOG_MESSAGE(Engine, Perf, "BEGIN: Sorting Unit %1", <<i);
        }
		// lpCount[x] now contains the number of elements less than
		// or equal to x
		// j iterates over lpSrc
        
		{
			// lpCount[k] stores the last entry in the list for key k
			// lpSrc stores the sorted row number
			int *lpKeyStart = ippKeys[i];
            
			for (j = mnRows - 1; j >= 0; --j) {
				int lSortedRow = lpSrc[j];			// 1st memory load
				int lKey = lpKeyStart[lSortedRow];	// 2nd memory load
				int *lpListLast = &lpCount[lKey];
				int lCount = (*lpListLast) - 1;		// 3rd memory load
				lpDest[lCount] = lSortedRow;			// 1st memory write
				*lpListLast = lCount;					// 2nd memory write
			}
		}
        
		if (mnRows > 100000)
        {
			//LOG_MESSAGE(Engine, Perf, "END: Sorting Unit %1", <<i);
        }
        
		// Swap lpSrc and lpDest
		int  * lpSwap = lpDest;
		lpDest = lpSrc;
		lpSrc = lpSwap;
	} // for
    
    
	// At this moment lpSrc is sorted against pTempKey
	assert(lnCount < iorSize);
	pDistinct[lnCount++] = lpSrc[0];
	
	for (i = 1; i < mnRows; ++i) {
		// aye, we do search backward, which finds the mismatch quickly
		// Also, I believe there is a better algorithm to do this.
		for (j = nUnits - 1; j >= 0; j--) {
			int *p1 = ippKeys[j] + lpSrc[i];
			int *p2 = ippKeys[j] + lpSrc[i-1];
            
			if (*p1 != *(p2))
				break;
		}
		if (j >= 0) {
			_ASSERTE(lnCount < iorSize);
			pDistinct[lnCount++] = lpSrc[i];
		}
	}
    
	if (mnRows > 100000)
    {
		//LOG_MESSAGE_NP(Engine, Perf, "END: Index Collapse Sort");
    }
    
	//  Calculate the UnqualifiedRows for the new index
    /*
     orUnqualifiedRows.Init(lnCount, false );
     if (!mUnqualifiedRows.IsEmpty())
     {
     // Group the rows with the same keys together
     // We rely on the fact that pDistinct contains the first row in the group
     
     // Read a row's 'unqualified' value and calculate the group value.
     // If all members of a group are unqualified, the group is unqualified.
     // If it's the end of the current group, write the unqualified value of the group
     
     int lCurrentGroupNo = 0;
     int lNextGroupStart = (lCurrentGroupNo < lnCount -1)? pDistinct[lCurrentGroupNo+1] : -1;
     bool lIsGroupUnqualified = true;
     
     for (i = 0; i < mnRows; i++)
     {
     bool lIsRowUnqualified = IsUnqualified(lpSrc[i]);
     
     lIsGroupUnqualified = lIsGroupUnqualified && lIsRowUnqualified;
     
     // if it's the end of the current group, write the group value
     if ((i < mnRows - 1 && lpSrc[i+1] == lNextGroupStart) || i == mnRows - 1)
     {
     orUnqualifiedRows.Set(lCurrentGroupNo, lIsGroupUnqualified);
     
     lCurrentGroupNo++;
     lIsGroupUnqualified = true;
     lNextGroupStart = (lCurrentGroupNo < lnCount -1)? pDistinct[lCurrentGroupNo+1] : -1;
     }
     }
     }*/
    
	// If the caller is interested in the map from the base index to the result index,
	// perform the write here.
	// This code is better put together as the
	// We split the loop for performance reason.
	if (opRowMap)
	{
		if (iRawSortResult)
			memmove(opRowMap, lpSrc, sizeof(int) * mnRows);
		else
		{
			int lCurrentGroupNo = 0;
            
			for (i = 0; i < mnRows; i++)
			{
				if (lCurrentGroupNo < lnCount && lpSrc[i] == pDistinct[lCurrentGroupNo])
				{
					lCurrentGroupNo++;
				}
				opRowMap[lpSrc[i]] = lCurrentGroupNo - 1;
			}
		}
	}
    
	// To make the result exactly the same as previous implementation,
	// we should have done
	// std::sort(pDistinct, pDistinct + lnCount);
	// But this is not necessary.
    
	iorSize = lnCount;
    
    return hr = S_OK;
}

void DSSTabularIndex::hCalcKeyDistributions(vector<int*> *ipKeyColumns, bool iDummyIndexOnly)
{
	// The mvMaxKeys stores the max key value for each unit
	int i = 0;
	if (iDummyIndexOnly)	//we keep the existing distribution vector and add the dummy unit to the end
		i = mnUnits - 1;
	else
    {
        // tehe 4/17/2012: memory leak
        for(int i = 0; i < mvKeyDistributions.size(); ++i)
        {
            if(mvKeyDistributions[i])
            {
                delete [] mvKeyDistributions[i];
                mvKeyDistributions[i] = NULL;
            }
        }
        mvKeyDistributions.clear();
    }
    
    
	for (; i < mnUnits; i++)
	{
		int *lpDist = new int[ mvMaxKeys[i] + 1];
        //CHECK_NEWED_PTR(lpDist);
        //MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lDistPtr(lpDist);
        
		mvKeyDistributions.push_back(lpDist);
		
		int j = 0;
        
		if (ipKeyColumns)
		{
			int *lpKey = (*ipKeyColumns)[i];
			CalcKeyDistributions(lpKey, mvMaxKeys[i], mnRows, lpDist);
		}
		else
		{
			memset(lpDist, 0, sizeof(int) * (mvMaxKeys[i]+1));
			for (j = 0; j < mnRows; j++)
			{
				int lKey = *(mpKeys + (size_t)mnUnits*j + i);
				lpDist[lKey]++;
			}
            
			for (j = 1; j <= mvMaxKeys[i]; j++)
			{
				lpDist[j] = lpDist[j] + lpDist[j-1];
			}
		}
	}
}

// add a static version, so that we can call it from other classes, e.g. CDSSIndexSpace
void DSSTabularIndex::CalcKeyDistributions(int* ipKeyCol, int iMaxKey, int nRow, int* opDist)
{
	assert(opDist);
    
	memset(opDist, 0, sizeof(int) * (iMaxKey+1));
    
	int* lpKey = ipKeyCol;
	for (int iRow = 0; iRow < nRow; iRow++)
	{
		opDist[*lpKey] ++;
		lpKey++;
	}
    
	for (int iKey = 1; iKey <= iMaxKey; iKey++)
	{
		opDist[iKey] += opDist[iKey-1];
	}
}


int DSSTabularIndex::TransformGlobalIndexToLocalIndex(int iGlobalIndex,int *opLocalIndex)
{
    for(int i=0;i<mnUnits;i++)
    {
        if(mpUnits[i]==iGlobalIndex)
        {
            *opLocalIndex=i;
            return S_OK;
        }
    }
    
    return S_FALSE;
}

int DSSTabularIndex::PartialMatch(const vector<int> &iUnits, const vector<vector<int> > &elements, int iUnit, vector<int> &iCandidateElements)
{
    int hr = S_OK;
    
    int *lBase = mpKeys;
    for(int i = 0; i < mnRows; ++i)
    {
        bool lFound = true;
        
        for(int j=0;j<iUnits.size();j++)
        {
            //speical support for only select total
            bool allTotal=true;
            
            for(int k=0;k<elements[j].size();k++)
            {
                if(elements[j].at(k)==-100)
                    continue;
                else
                {
                    allTotal=false;
                    break;
                }
            }
            
            if(allTotal&&elements[j].size()>0)
                continue;
            
            
            int localIndex=-1;
            
            hr=TransformGlobalIndexToLocalIndex(iUnits[j], &localIndex);
            
            CHECK_HR;
            
            int *lKey = lBase + localIndex;
            
            vector<int>::const_iterator it = find(elements[j].begin(), elements[j].end(), *lKey);
            
            if(it == elements[j].end())
            {
                lFound = false;
                break;
            }
        }
        if(lFound)
        {
            int localIndex=-1;
            
            hr=TransformGlobalIndexToLocalIndex(iUnit, &localIndex);
            
            CHECK_HR;
            
            iCandidateElements.push_back(*(lBase + localIndex));
        }
        
        lBase = lBase + mnUnits;
    }
    
    sort(iCandidateElements.begin(), iCandidateElements.end());
    vector<int>::iterator lCandidateIter;
    lCandidateIter = unique(iCandidateElements.begin(), iCandidateElements.end());
    iCandidateElements.erase(lCandidateIter, iCandidateElements.end());
    
    return hr;
}

int DSSTabularIndex::CalculateMaskUsingRelation(int iIndexUnit, GUID* iIndexGUID, int iNonIndexUnit, GUID* iNonIndexGUID,
                                                vector<int>& ivNonIndexUnitElement, DSSRelation* ipRelation,DSSGUnitContainer *ipGUnitContainer, DSSXTabBitVector& orBitVector, vector<int> *ipLocal2Global)
{
    AE_ASSERT(iIndexGUID&&iNonIndexGUID&&ipRelation);
    
    orBitVector.Init(mnRows, false);
    if(ivNonIndexUnitElement.empty()||mnRows==0)
        return S_OK;
    
    HRESULT hr = S_OK;
    
    bool lfHasTotal = false;
    for(int i=0; i<(int)ivNonIndexUnitElement.size();i++)
    {
        if(ivNonIndexUnitElement[i]==-100)//total
        {
            lfHasTotal = true; break;
        }
    }
    if(lfHasTotal)
    {
        orBitVector.BulkSet(0, mnRows-1, true);
        return S_OK;
    }
    
    int lnGlobalRowCount=0;
    
    hr=ipGUnitContainer->GetGlobalRowCount(iNonIndexUnit, &lnGlobalRowCount, false);
    
    CHECK_HR;
    
    bool lfCompoundNull=false;
    
    for(int i=0; i<(int)ivNonIndexUnitElement.size();i++)
    {
        if(ivNonIndexUnitElement[i]==lnGlobalRowCount)
        {
            lfCompoundNull = true;
            
            break;
        }
    }
    
    int lnIndexUnitElement = -1;
    int* lpIndexUnitElements = NULL;
    
    //ipRelation->getRelatedElements(*iNonIndexGUID, ivNonIndexUnitElement.size(), &ivNonIndexUnitElement[0], &lnIndexUnitElement,
    //                        &lpIndexUnitElements,lnGlobalRowCount,true);
    
    ipRelation->getRelatedElements(*iNonIndexGUID, ivNonIndexUnitElement.size(), &ivNonIndexUnitElement[0], &lnIndexUnitElement,
                                   &lpIndexUnitElements);
    
    int lIndexUnitPos = -1;
    for(int i=0; i<mnUnits;i++)
        if (mpUnits[i]==iIndexUnit)
        {
            lIndexUnitPos = i; break;
        }
    if(lIndexUnitPos == -1)
        return S_OK;
    
    //junwang, 05/16/2012, use ElementIndex to calculate the mask and considering null
    ElementIndex* lpElementIndex = NULL;
    hr = hGetElementIndexForUnit(lIndexUnitPos, &lpElementIndex);
    CHECK_HR;
    
    int lnTmpUnitElement = -1;
    int* lpTmpUnitElements = NULL;
    int* lpElements = lpElementIndex->mpElements;
    int lnElements =  lpElementIndex->mnElements;
    
    
    
    DSSTabularAttribute* lpTabularAttribute = NULL;
    
    ipGUnitContainer->getGlobalLookup(iIndexUnit,&lpTabularAttribute);
    
    DSSGlobalLookUpTable* lpLookUp = dynamic_cast<DSSGlobalLookUpTable*>(lpTabularAttribute);
    
    for(int j=0; j<lnElements; j++)
    {
        int lKey = lpElements[j];
        
        //xiazhou, 920138, 07/03/2014. If this is base index, then needs to transfer from local to global
        if(ipLocal2Global && !ipLocal2Global->empty())
        {
            if(lKey >= 0 && lKey < ipLocal2Global->size())
                lKey = ipLocal2Global->at(lKey);
        }
        
        //transfer DE to leaf elements since relation don't contain DE branch element
        set<int> lvLeafGlobalKeySet;
        
        vector<int> lvLeafGlobalKey;
        
        DSSAEHelper::convertDEToLeafElements(lKey, lpLookUp, lvLeafGlobalKeySet);
        
        lvLeafGlobalKey.assign(lvLeafGlobalKeySet.begin(), lvLeafGlobalKeySet.end());
        
        
        bool lfSet = false;
        
        if(lvLeafGlobalKey.size()>0)
        {
            int lnSize=lvLeafGlobalKey.size();
            
            for(int i=0;i<lnSize;i++)
            {
                int lLeafGlobalKey=lvLeafGlobalKey[i];
                
                int lElement = 0;
                for(;lElement<lnIndexUnitElement;lElement++)
                    if(lLeafGlobalKey == lpIndexUnitElements[lElement])
                        break;
                
                //hyan,1/22/2013
                //when parent select null, display all the child element which has no parent
                if(lElement == lnIndexUnitElement&&lfCompoundNull)
                {
                    ipRelation->getRelatedElements(*iIndexGUID, 1, &lLeafGlobalKey, &lnTmpUnitElement, &lpTmpUnitElements);
                    if(lnTmpUnitElement==0)
                        lfSet = true;
                }
                else if(lElement<lnIndexUnitElement)
                {
                    lfSet=true;
                    break;
                }
            }
        }
        else
        {
            int lElement = 0;
            for(;lElement<lnIndexUnitElement;lElement++)
                if(lKey == lpIndexUnitElements[lElement])
                    break;
            //when parent select null, display all the child element which has no parent
            
            if(lElement == lnIndexUnitElement&&lfCompoundNull)
            {
                ipRelation->getRelatedElements(*iIndexGUID, 1, &lKey, &lnTmpUnitElement, &lpTmpUnitElements);
                if(lnTmpUnitElement==0)
                    lfSet = true;
            }
            else if(lElement<lnIndexUnitElement)
                lfSet = true;
        }
        
        if(lfSet)
        {
            if (lpElementIndex->mpElementAddress)
            {
                for (int k= lpElementIndex->mpElementAddress[j]; k<lpElementIndex->mpElementAddress[j+1];k++)
                {
//                    orBitVector.Set(k, true);
//                    xiazhou, 928099. k is offset in mpRows, not row number of index.
                    orBitVector.Set(lpElementIndex->mpRows[k], true);
                }
            }
            else
                orBitVector.Set(lpElementIndex->mpRows[j], true);
        }
    }
    
    if(!mUnqualifiedRows.IsEmpty())
        orBitVector.CalculateAndNot(&mUnqualifiedRows);
    
    //junwang, 03/28/2012, memory leak
    if (lpIndexUnitElements!=NULL)
    {
        delete[] lpIndexUnitElements;
        lpIndexUnitElements = NULL;
    }
    if(lpTmpUnitElements!=NULL)
    {
        delete[] lpTmpUnitElements;
        lpTmpUnitElements = NULL;
    }
    
    return S_OK;
}

int DSSTabularIndex::CalculateMask(int nUnits, int *ipUnits, vector<int>* ipvElements, DSSXTabBitVector& orBitVector)
{
    HRESULT hr = S_OK;
	int i = 0;
	int j = 0;
    
	// init orBitVector
	orBitVector.Init(mnRows, false);
	if (mnRows == 0)
		return S_OK;
    
	orBitVector.BulkSet(0, mnRows-1, true);
    
	// #########################################################################################
	//                  TODO: shall we care about mUnqualifiedRows?
	// #########################################################################################
    // 2/9/2012 tehe we need care about unqualified rows, because some rows in DE may be filtered.
	if (!mUnqualifiedRows.IsEmpty())
		orBitVector.CalculateAndNot(&mUnqualifiedRows);
    
	// find valid unit pos
	vector<int> lvUnitPos;
	vector<int> lvUnitIndex;
    // tehe : lValid has no use
	// bool lValid = false;
	for (i = 0; i < nUnits; i++)
	{
		// lValid = false;
		for (j = 0; j < mnUnits; j++)
		{
			if (ipUnits[i] == mpUnits[j])
			{
				// lValid = true;
				lvUnitPos.push_back(j);
				lvUnitIndex.push_back(i);
				break;
			}
		}
	}
    
    //	if (!lValid)
    //		return S_OK;
    
	vector<int> lvFilterUnitPos;
	vector<int> lvFilterUnitIndex;
	int lnUnits = lvUnitPos.size();
	int lnElements;
	bool doFilter = true;
    
	for (i = 0; i < lnUnits; i++)
	{
        
		lnElements = ipvElements[lvUnitIndex[i]].size();
		if(lnElements == 0) // no element is selected
		{
			orBitVector.BulkSet(0, mnRows - 1, false);
			doFilter = false;
			break;
		}
		//vector<int> lvElements;
		//for (j = 0; j < lnElements; j++)
		//{
		//	if (ipvElements[lvUnitIndex[i]][j] == -1) // contains "ALL" element
		//
		//		break;
		//}
        
		//if(j == lnElements)
        
		//ctang: -1 should be an invalid element
		// bool lfNeedFilter = false;
        // 5/8/2012 #590102 tehe: here we should handle all element specialy
        bool lfNeedFilter = true;
        bool hasValidElement = false;
        
		for (j = 0; j < lnElements; j++)
		{
            //			if (ipvElements[lvUnitIndex[i]][j] != -1)
            //			{
            //				lfNeedFilter = true;
            //				break;
            //			}
            // tehe
            if (ipvElements[lvUnitIndex[i]][j] == -1) {
                lfNeedFilter = false;
                break;
            }
            else if (ipvElements[lvUnitIndex[i]][j] >-1)
            {
                hasValidElement = true;
            }
		}
        
		if (lfNeedFilter && hasValidElement)
		{
			lvFilterUnitPos.push_back(lvUnitPos[i]);
			lvFilterUnitIndex.push_back(lvUnitIndex[i]);
		}
        
	}
    
	if(!doFilter) // some of the unit filters out every rows
		return hr;
    
	if (lvFilterUnitPos.size() == 0)
	{
        // tehe: keep same as server side
		// orBitVector.BulkSet(0, mnRows - 1, false);
		return S_OK;
	}
    
	lnUnits = lvFilterUnitPos.size();
    
    //zhyang, currently we don't support ElementBlock
	// if it has element blocks, make use of element blocks instead of building ElementIndex
    /*	if(!mvElementBlocks.empty())
     {
     CDSSElementBlocks *lpElementBlock = NULL;
     int *lpBlockAddress;
     int *lpStartBlock, *lpEndBlock;
     int lBlock;
     int lStartRow, lEndRow;
     
     for (i = 0; i < lnUnits; i++)
     {
     lpElementBlock = mvElementBlocks[lvFilterUnitPos[i]];
     CDSSXTabBitVector lTmpBitVector;
     lTmpBitVector.Init(mnRows, false);
     lpBlockAddress = lpElementBlock->GetStartAddressPtr();
     lnElements = ipvElements[lvFilterUnitIndex[i]].size();
     for (j = 0; j < lnElements; j++)
     {
     // if the key is greater than the maxKey of the elment block, ignore it
     if (ipvElements[lvUnitIndex[i]][j] >= lpElementBlock->GetNumKeys())
     continue;
     
     lpElementBlock->GetBlocksForKey(ipvElements[lvUnitIndex[i]][j], lpStartBlock, lpEndBlock);
     
     while (lpStartBlock != lpEndBlock)
     {
     lBlock = *lpStartBlock++;
     if (lpBlockAddress)
     {
     lStartRow = lpBlockAddress[lBlock];
     lEndRow = lpBlockAddress[lBlock + 1] - 1;
     lTmpBitVector.BulkSet(lStartRow, lEndRow, true);
     }
     else
     {
     lTmpBitVector.Set(lBlock, true);
     }
     }
     }// end for Elements
     
     orBitVector.CalculateAnd(&lTmpBitVector);
     if(!orBitVector.IsAnyTrue(0, mnRows - 1))
     break;
     }// end for units
     return hr;
     }
     */
	// use ElementIndex to calculate the mask
	int lElement = 0;
	int lBegin = 0;
	int lEnd = 0;
	int lMid = -1;
	int *lpElements = NULL;
	ElementIndex *lpElementIndex;
	bool isFindAnyRow = false;
    
    // 9/5/2012 tehe: avoid build element index for every unit
    double lMinRatio = 1.0;
    int lUnitIndex = -1;
    
    if(mvKeyDistributions.size() != mnUnits)
    {
        hCalcKeyDistributions();
    }
    if (lnUnits > 1)
	{
		for (i = 0; i < lnUnits; i++)
		{
            lnElements = ipvElements[lvUnitIndex[i]].size();
            if(mvElementCounts[lvUnitPos[i]] == -1 && mvMaxKeys[lvUnitPos[i]] > MAXKEY)
            {
                vector<int> lvUniqueKey(mvMaxKeys[lvUnitPos[i]]+1);
                int *lpKeyDistribution = mvKeyDistributions[lvUnitPos[i]];
                
                lvUniqueKey[0] = lpKeyDistribution[0];
                int lnCount = 0;
                for(int i=1;i < lvUniqueKey.size(); i++)
                {
                    lvUniqueKey[i] = lpKeyDistribution[i] - lpKeyDistribution[i-1];
                    if(lvUniqueKey[i] != 0)
                        lnCount ++;
                }
                mvElementCounts[lvUnitPos[i]] = lnCount;
            }
            else if(mvMaxKeys[lvUnitPos[i]] > MAXKEY)
            {
                double lRatio = lnElements * 1.0 / mvElementCounts[lvUnitPos[i]];
                if(lRatio < lMinRatio)
                {
                    lMinRatio = lRatio;
                    lUnitIndex = i;
                }
            }
		}
	}
    
    if (lMinRatio < MINRATIO && lUnitIndex >= 0)	// ratio < 1/1000
	{
		DSSXTabBitVector lTmpBitVector;
		lTmpBitVector.Init(mnRows, false);
		lTmpBitVector.Clear();
		isFindAnyRow = false;
        
		lpElementIndex = NULL;
		hr = hGetElementIndexForUnit(lvFilterUnitPos[lUnitIndex], &lpElementIndex);
		CHECK_HR;
        
		lpElements = lpElementIndex->mpElements;
		lnElements = ipvElements[lvUnitIndex[lUnitIndex]].size();
		for (j = 0; j < lnElements; j++)
		{
			lElement = ipvElements[lvFilterUnitIndex[lUnitIndex]][j];
			if (lElement < 0)// rzhu, 03/07/2012, 560927, ignore total case
				continue;
			lBegin = 0;
			lEnd = lpElementIndex->mnElements - 1;
			lMid = -1;
			while(lBegin < lEnd)
			{
				if (lpElements[lBegin] == lElement)
				{
					lEnd = lBegin;
					break;
				}
				else if(lpElements[lEnd] == lElement)
				{
					lBegin = lEnd;
					break;
				}
                
				lMid = lBegin + (lEnd - lBegin) / 2;
                
				if (lpElements[lMid] < lElement)
					lBegin = lMid + 1;
				else
					lEnd = lMid;
			}
            
			if ((lBegin < lpElementIndex->mnElements) && lpElements[lBegin] == lElement)
			{ // found
				isFindAnyRow = true;
				if(lpElementIndex->mpElementAddress == NULL)
				{
					lTmpBitVector.Set(lpElementIndex->mpRows[lBegin], true);
				}
				else
				{
					for (Int32 iStart = lpElementIndex->mpElementAddress[lBegin]; iStart < lpElementIndex->mpElementAddress[lBegin+1]; iStart++)
					{
						lTmpBitVector.Set(lpElementIndex->mpRows[iStart], true);
					}
				}
			}// end of IF
		} // End of for Elements
        
		if (!isFindAnyRow)
        {
            orBitVector.BulkSet(0, mnRows-1, false); //zhyang, 854208
			return S_OK;	// no row is selected
        }
		orBitVector.CalculateAnd(&lTmpBitVector);
        
		// compare other units row by row
		DSSXTabKeyIterator lIt(this);
		orBitVector.SetSeekPosition(0);
		Int32 lRow = orBitVector.GetNextSetPosition();
        
		while (lRow < mnRows && lRow >= 0)
		{
			lIt.SetRow(lRow);
            
			for (i = 0; i < lnUnits; i++)
			{
				if (i == lUnitIndex)
					continue;
                
				Int32 lKey = lIt.GetCol(lvFilterUnitPos[i]);
                
				bool lfSelected = false;
				lnElements = ipvElements[lvUnitIndex[i]].size();
				for (j = 0; j < lnElements; j++)
				{
					lElement = ipvElements[lvFilterUnitIndex[i]][j];
					if (lElement < 0)// rzhu, 03/07/2012, 560927, ignore total case
						continue;
                    
					if (lElement == lKey)
					{
						lfSelected = true;
						break;
					}
				}
                
				if (!lfSelected)
				{
					orBitVector.Set(lRow, false);
					break;
				}
			}
            
			lRow = orBitVector.GetNextSetPosition();
		}
        
		return S_OK;
	}
    
	for ( i = 0; i < lnUnits; i++)
	{
		DSSXTabBitVector lTmpBitVector;
		lTmpBitVector.Init(mnRows, false);
		lTmpBitVector.Clear();
		isFindAnyRow = false;
        
		lpElementIndex = NULL;
		hr = hGetElementIndexForUnit(lvFilterUnitPos[i], &lpElementIndex);
		CHECK_HR;
        
		lpElements = lpElementIndex->mpElements;
		lnElements = ipvElements[lvUnitIndex[i]].size();
		for (j = 0; j < lnElements; j++)
		{
			lElement = ipvElements[lvFilterUnitIndex[i]][j];
			lBegin = 0;
			lEnd = lpElementIndex->mnElements - 1;
			lMid = -1;
			while(lBegin < lEnd)
			{
				if (lpElements[lBegin] == lElement)
				{
					lEnd = lBegin;
					break;
				}
				else if(lpElements[lEnd] == lElement)
				{
					lBegin = lEnd;
					break;
				}
                
				lMid = lBegin + (lEnd - lBegin) / 2;
                
				if (lpElements[lMid] < lElement)
					lBegin = lMid + 1;
				else
					lEnd = lMid;
			}
            
			if ((lBegin < lpElementIndex->mnElements) && lpElements[lBegin] == lElement)
			{ // found
				isFindAnyRow = true;
				if(lpElementIndex->mpElementAddress == NULL)
				{
					lTmpBitVector.Set(lpElementIndex->mpRows[lBegin], true);
				}
				else
				{
					for (int iStart = lpElementIndex->mpElementAddress[lBegin]; iStart < lpElementIndex->mpElementAddress[lBegin+1]; iStart++)
					{
						lTmpBitVector.Set(lpElementIndex->mpRows[iStart], true);
					}
				}
			}// end of IF
            
		} // End of for Elements
        
		orBitVector.CalculateAnd(&lTmpBitVector);
		if(!isFindAnyRow) break;
	}
    
	return S_OK;
}

int DSSTabularIndex::hGetElementIndexForUnit(int iUnitPos, ElementIndex** oppElementIndex)
{
	int hr = S_OK;
	// mpvElementIndices is not initialized yet
	if(mpvElementIndices == NULL)
	{
		mpvElementIndices = new vector<ElementIndex*>;
		CHECK_NEWED_PTR(mpvElementIndices);
        
		mpvElementIndices->resize(mnUnits, NULL);
	}
    
	ElementIndex *lpElementIndex = (*mpvElementIndices)[iUnitPos];
    
	if(lpElementIndex == NULL)//have not built yet
	{
        // #767710,tehe
		hr = hBuildElementIndexForUnit(iUnitPos, &lpElementIndex);
		CHECK_HR_PTR(lpElementIndex);
        
        (*mpvElementIndices)[iUnitPos] = lpElementIndex;
	}
    
	*oppElementIndex = lpElementIndex;
    
	return hr;
    
}

int DSSTabularIndex::hBuildElementIndexForUnit(int iUnitPos, ElementIndex** oppElementIndex)
{
	if(oppElementIndex == NULL)
		return E_POINTER;
    
    // #767710,tehe
	// ElementIndex *lpElementIndex = new ElementIndex;
	*oppElementIndex = NULL;
    
	// special cases
	if(mnRows == 0)
	{
        ElementIndex *lpElementIndex = new ElementIndex;
        
		lpElementIndex->mnElements = 0;
		lpElementIndex->mpElements = NULL;
		lpElementIndex->mpElementAddress = NULL;
		lpElementIndex->mpRows = NULL;
        lpElementIndex->mpIndex = NULL;
        
        *oppElementIndex = lpElementIndex;
		return S_OK;
	}
    
	int lMaxUnitValue = mvMaxKeys[iUnitPos];
    
    // tehe: add memory govern for element index
    int lReservedSize = mnRows * 2 *sizeof(int);
    MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)
    MEM_GOVERNOR_COMMIT(lReservedSize)
    
    // 9/16/2013 tehe: avoid to alloc tempkeys here, tempkeys is used to store the key value of iUnitPos. Actually it's not necessary, it will not save us much time but cost memory
	// get the keys for the unit
	// int  * lpTempKeys = new int[mnRows];
	// CHECK_NEWED_PTR(lpTempKeys);
    
    // tehe 4/19/2012: memory leak
    // MBase::StrongPtr<int,MBase::DeleteArray<int> > lStrongPtrTempKeys(lpTempKeys);
    
//	for(int i = 0; i < mnRows; i++)
//	{
//		*(lpTempKeys + i) = mpKeys[i * (size_t)mnUnits + iUnitPos];
//	}
    
	int* lpCount = new int[lMaxUnitValue + 1];
	CHECK_NEWED_PTR(lpCount);
    // tehe 4/19/2012: memory leak
    MBase::StrongPtr<int,MBase::DeleteArray<int> > lStrongPtrCount(lpCount);
    
    // tehe: optimize for memory, here don't need lpSrc
    int *lpDest = NULL;
    
	// Array of rows sorted
	lpDest = new int[mnRows];
	CHECK_NEWED_PTR(lpDest);
    MBase::DSSStrongPtr<int,MBase::DeleteArray<int> > lpDestSp(lpDest); // tehe: avoid leak
    
	// The following is faster but not good for debugging
    if(mvKeyDistributions.size() != mnUnits)
        hCalcKeyDistributions();
    
	memmove(lpCount, mvKeyDistributions[iUnitPos], (lMaxUnitValue + 1) * sizeof(int));
	// lpCount[x] now contains the number of elements less than
	// or equal to x
	{
		// lpCount[k] stores the last entry in the list for key k
		// int *lpKeyStart = lpTempKeys;
        
		for (int j = mnRows - 1; j >= 0; --j) {
            // tehe: we don't need 1st memory load, since lpSrc[j] equals j
			// int lKey = lpKeyStart[j];	// 2nd memory load
            int lKey = mpKeys[j * mnUnits + iUnitPos];
			int *lpListLast = &lpCount[lKey];
			int lCount = (*lpListLast) - 1;		// 3rd memory load
			lpDest[lCount] = j;			// 1st memory write
			*lpListLast = lCount;					// 2nd memory write
		}
	}
    
	// count distinct keys
    // tehe 4/19/2012: use strong ptr to avoid memory leak
	int lnDistinct = 1;
	int *lpDistinctKey = new int[lMaxUnitValue + 1];
    CHECK_NEWED_PTR(lpDistinctKey);
    MBase::StrongPtr<int,MBase::DeleteArray<int> > lStrongPtrDistinctKey(lpDistinctKey);
    
	int *lpDistinct = new int[lMaxUnitValue + 2];
    CHECK_NEWED_PTR(lpDistinct);
    MBase::StrongPtr<int,MBase::DeleteArray<int> > lStrongPtrDistinct(lpDistinct);
    
    // lKey1 = lpTempKeys[lpDest[0]];
	int lKey1 = mpKeys[lpDest[0] * mnUnits + iUnitPos];
	int lKey2 = -1;
	lpDistinctKey[0] = lKey1;
	lpDistinct[0] = 0;
    
	for (int i = 1; i < mnRows; i++)
	{
        // lKey2 = lpTempKeys[lpDest[i]];
		lKey2 = mpKeys[lpDest[i] * mnUnits + iUnitPos];
		if(lKey1!= lKey2)
		{
			lKey1 = lKey2;
			lpDistinctKey[lnDistinct] = lKey1;
			lpDistinct[lnDistinct] = i;
			lnDistinct++;
		}
	}
	lpDistinct[lnDistinct] = mnRows;
    
	int *lpElements = NULL;
	int* lpElementAddress = NULL;
    
	lpElements = new int[lnDistinct];
    CHECK_NEWED_PTR(lpElements);
    MBase::DSSStrongPtr<int,MBase::DeleteArray<int> > lpElementsSp(lpElements); // tehe: avoid leak
    
	memmove(lpElements, lpDistinctKey, lnDistinct * sizeof(int));
    
	// if there is no duplicate keys in each row, we dont need to save mpElementAddress
	// because the value of mpElementAddress should be 0 to mnElements
    MBase::DSSStrongPtr<int,MBase::DeleteArray<int> > lpElementAddressSp(NULL);
	if(lnDistinct < mnRows)
	{
		lpElementAddress = new int[lnDistinct + 1];
        CHECK_NEWED_PTR(lpElementAddress);
        lpElementAddressSp.Reset(lpElementAddress); // tehe: avoid leak
        
		memmove(lpElementAddress, lpDistinct, (lnDistinct + 1) * sizeof(int));
	}
    
    // #767710,tehe
    ElementIndex *lpElementIndex = new ElementIndex;
	lpElementIndex->mnElements = lnDistinct;
	lpElementIndex->mpElements = lpElements;
	lpElementIndex->mpElementAddress = lpElementAddress;
	lpElementIndex->mpRows = lpDest;
    lpElementIndex->mpIndex = this;
    
    lpElementsSp.GiveUp();
    lpDestSp.GiveUp();
    lpElementAddressSp.GiveUp();
    
    // 7/19/2012 tehe: for field group, element index will be built when do manipulation instead of at binary loading workflow, so in order to save memory for it, we will do mmap for element index here. mFilePath should be already set if the index is very large at binary loading workflow. For Grid, Element Index may already be built and do mmap at binary loading workflow. But it's OK, since we can handle this in createMMapMemory method. if it's already done mmap, we will not do it again.
    int hr = lpElementIndex->createMMapMemory(mFilePath.c_str());
    CHECK_HR;
    
    *oppElementIndex = lpElementIndex;
    
    MEM_GOVERNOR_RELEASE(mnRows *sizeof(int), mnRows *sizeof(int))
    
	return S_OK;
}

void DSSTabularIndex::toString()
{
    int hr = S_OK;
    printf("\nEnter DSSTabularIndex::toString() function\n");
    //1. object address
    //printf("DSSTabularIndex Address is %x\n", (unsigned int)this);
    printf("DSSTabularIndex Address is %lx\n", (unsigned long)this); // yanzli: support arm64
    //2. row count, unit list (name)
    printf("There are %d rows.\n", mnRows);
    printf("There are %d units, each of them are:\n", mnUnits);
    DSSObjectContext *lpObjectContext =  mpTabularData->getObjectContext();
    vector<DSSTabularAttribute *> lvAttr;
    vector< vector<int> *> lvDisplayCol;
    vector<string> lvHeader;
    for(int i = 0; i < mnUnits; ++i)
    {
        DSSTabularUnit *lpUnit = mpTabularData->Item(mpUnits[i]);
        DSSTabularAttribute *lpAttr = (DSSTabularAttribute*)lpUnit;
        lvAttr.push_back(lpAttr);
        GUID lID;
        lpAttr->getGUID(lID);
        DSSObjectInfo *lpObj = lpObjectContext->FindObjectH(lID);
        string lName = lpObj->getName();
        lvHeader.push_back(lName);
        printf("Unit %d is : %s\n", i, lName.c_str());
        
        DSSAttribute *lpAttrInfo = (DSSAttribute *)lpObj;
        DSSAttributeFormWrappers *lpDispWrappers = new DSSAttributeFormWrappers();
        lpAttrInfo->getDisplayFormWrappers(lpDispWrappers);
        vector<int> *lpCol = new vector<int>;
        for(int j = 0; j < lpDispWrappers->Count(); ++j)
        {
            int lBaseFormID = lpDispWrappers->Item(j)->getBaseFormID();
            int lFormPos = 0;
            lpAttr->FindFormWithLocale(0, lBaseFormID, 0, lFormPos);
            lpCol->push_back(lFormPos);
        }
        lvDisplayCol.push_back(lpCol);
        delete lpDispWrappers;
        lpDispWrappers = NULL;
    }
    //3. key table
    //a. printed row by row, with row number (0-based) in the front
    //b. display integer key + ID form value
    printf("Key Table:\n");
    
    //print the header
    printf("line\t");
    for(int i = 0; i < mnUnits; ++i)
    {
        printf("%s\t", lvHeader[i].c_str());
    }
    printf("\n");
    
    for(int i = 0; i < mnRows; ++i)
    {
        printf("%d\t", i);
        for (int j = 0; j < mnUnits; ++j)
        {
            printf("%d.", mpKeys[i * mnUnits + j]);
            
            for(int k = 0; k < lvDisplayCol[j]->size(); ++k)
            {
                DSSDataColumn *lpDataColumn = NULL;
                lvAttr[j]->Item((*lvDisplayCol[j])[k], &lpDataColumn);
                //printf(lDataColumn-);
                char *lpData = NULL;
                int lDataSize;
                EnumDSSDataType lDataType;
                DSSDataFlag lDataFlag;
                hr = lpDataColumn->getData(mpKeys[i * mnUnits + j], (const char**)(&lpData), lDataSize, lDataType, lDataFlag);
                if (S_OK != hr)
                {
                    printf("Can't get data from data column\n");
                    return;
                }
                
                if (DssDataOk != lDataFlag)
                {
                    printf("Data is invalid>\n");
                    continue;
                }
                
                switch(lDataType)
                {
                    case DssDataTypeShort:
                        printf("%d\t", *(reinterpret_cast<const short*>(lpData)));
                        break;
                        
                    case DssDataTypeInteger:
                        printf("%d\t", *(reinterpret_cast<const int*>(lpData)));
                        break;
                    case DssDataTypeLong:
                        printf("%lld\t", *(reinterpret_cast<const long long*>(lpData)));
                        break;
                        
                    case DssDataTypeFloat:
                        printf("%f\t", *(reinterpret_cast<const float*>(lpData)));
                        break;
                        
                    case DssDataTypeDouble:
                        printf("%lf\t", *(reinterpret_cast<const double*>(lpData)));
                        break;
                        
                    case DssDataTypeBool:
                        printf("%d\t", *(reinterpret_cast<const bool*>(lpData)));
                        break;
                        
                        // Unsupported data type on client
                    case DssDataTypeTime:
                    case DssDataTypeDate:
                    case DssDataTypeTimeStamp:
                    case DssDataTypeCellFormatData:
                    case DssDataTypeBigDecimal:
                    case DssDataTypeMBChar:
                        printf("<Unsupported Type>\n");
                        break;
                        
                        // jimzhang Aug/18/2011 : I am not sure if this is right.
                    case DssDataTypeVarChar:
                    case DssDataTypeNVarChar:
                        printf("%s\t", reinterpret_cast<const char*>(lpData) );
                        break;
                        
                    case DssDataTypeChar:
                    case DssDataTypeNChar:
                        printf("%s\t", reinterpret_cast<const char*>(lpData) );
                        break;
                        
                        // jimzhang Aug/18/2011 : I am not sure if this is right.
                    case DssDataTypeUTF8Char:	// weiwang 2009-06-05 TQMS352644 UTF8 support
                        printf("%s\t", reinterpret_cast<const char*>(lpData) );
                        break;
                        
                    case DssDataTypeBinary:
                    case DssDataTypeVarBin:
                    case DssDataTypeLongVarBin:
                    {
                        // the icbDataSize is used for binary data
                        wchar_t* lpTemp = new wchar_t [2*lDataSize+1];
                        for(int i=0;i < lDataSize;i++)
                        {
                            unsigned char lValue = lpData[i];
                            ::swprintf(lpTemp+i*2,4,L"%02X",lValue);
                        }
                        lpTemp[2*lDataSize] = 0;
                        
                        printf("%ls\t", reinterpret_cast<const wchar_t*>(lpTemp) );
                        
                        delete[] lpTemp;
                    }
                        break;
                        
                    default:
                        printf("<Unknow Type>\n");
                        break;
                } // switch(lDataType)
                
            }
            
        }
        printf("\n");
    }
    
    for(int i = 0; i < lvDisplayCol.size(); ++i)
    {
        delete lvDisplayCol[i];
    }
}

// 3/31/2012 tehe
int DSSTabularIndex::LookupActualKey(int *ipKeys, int *opRow)
{
    if(mpHashTable == NULL)
    {
        int hr = hCreateHashTable();
        CHECK_HR_PTR(mpHashTable);
    }
    
    if (mpHashTable->LookUp(ipKeys, opRow))
    {
        return S_OK;
    }
	*opRow = -1;
	return S_FALSE;
}

int DSSTabularIndex::loadBitVectorFromBinary(DSSBinaryDataReader * ipReadStream)
{
    int hr = mUnqualifiedRows.LoadFromBinary(ipReadStream);
    CHECK_HR;
    
    return  hr;
}

int DSSTabularIndex::loadHiddenMaskFromBinary(DSSBinaryDataReader * ipReadStream)
{
    int hr = mHiddenRows.LoadFromBinary(ipReadStream);
    CHECK_HR;
    
    return  hr;
}

int DSSTabularIndex::setDistinct(bool ibDistinct)
{
    mIsDistinct = ibDistinct;
    return S_OK;
}

void DSSTabularIndex::dump()
{
    std::cout<<"Units:  ";
    for (int i=0; i<mnUnits; i++) {
        std::cout<<mpUnits[i]<<"  ";
    }
    std::cout<<std::endl;
    
    std::cout<<"keys: "<<mnRows<<"row\n";
    for (int i=0; i<mnRows; i++)
    {
        for(int j=0;j<mnUnits;j++)
            std::cout<<mpKeys[mnUnits*i+j]<<"  ";
        std::cout<<std::endl;
    }
}

int DSSTabularIndex::createMMapMemory(const char *ipFilePath, bool ibUseNewFile)
{
    bool lbNeedDoMMap = NeedDoMMap(ipFilePath);
    if(!lbNeedDoMMap)
        return S_OK;
    
    mFilePath = ipFilePath;
    
    int hr = S_OK;
    
    void* lpBuffer = NULL;
    const void* lpData = mpKeys;
    
    int lDataSize = mnUnits*mnRows*sizeof(int);
    
    hr = DSSCMBlockFileUtil::AddBlockHeader(ipFilePath, mBlockType, lDataSize);
    CHECK_HR;
    
    hr = DataSourceUtil::createMappedFileBuffer(1, &lpData, &lDataSize, ipFilePath, &ibUseNewFile,(void **)&mpMMPBuffer, &mMMPBufSize,&lpBuffer,&mOffset ,&mOffsetAligned);
    CHECK_HR;
    
    delete[] mpKeys;
    MEM_GOVERNOR_RELEASE(mnUnits * mnRows * sizeof(int), mnUnits * mnRows * sizeof(int))
    
    mpKeys = (int*)lpBuffer;
    
    mbUseMappedFile = true;
    
    if(mpHashTable)
    {
        mpHashTable->setNodeKeys(mpKeys);
        
        hr = mpHashTable->createMMapMemory(ipFilePath, ibUseNewFile);
        CHECK_HR;
    }
    
    return S_OK;
}

int DSSTabularIndex::releaseMMapMemory()
{
    if(!(mbUseMappedFile && mFilePath != "" && mpMMPBuffer))
        return S_OK;
    
    int hr = DSSCMMemoryMap::releaseMMapMemory();
    CHECK_HR;
    
    mpKeys = NULL;
    
    if(mpHashTable)
    {
        hr = mpHashTable->releaseMMapMemory();
        CHECK_HR;
    }
    if(mpvElementIndices)
    {
        for(int i=0; i<mpvElementIndices->size(); i++)
        {
            ElementIndex *lpElementIndex = (*mpvElementIndices)[i];
            if(lpElementIndex)
            {
                hr = lpElementIndex->releaseMMapMemory();
                CHECK_HR;
            }
        }
    }
    
    return S_OK;
}

int DSSTabularIndex::reloadMMapMemory()
{
    if(!(mbUseMappedFile && mFilePath!="" && mpMMPBuffer==NULL))
        return S_OK;
    
    int hr = DSSCMMemoryMap::reloadMMapMemory();
    CHECK_HR;
    
    mpKeys = reinterpret_cast<int*>(mpMMPBuffer + mOffset);
    
    if(mpHashTable)
    {
        hr = mpHashTable->reloadMMapMemory();
        CHECK_HR;
        
        mpHashTable->setNodeKeys(mpKeys);
    }
    
    if(mpvElementIndices)
    {
        for(int i=0; i<mpvElementIndices->size(); i++)
        {
            ElementIndex *lpElementIndex = (*mpvElementIndices)[i];
            if(lpElementIndex)
            {
                hr = lpElementIndex->reloadMMapMemory();
                CHECK_HR;
            }
        }
    }
    
    return S_OK;
}

int DSSTabularIndex::GetElementsForUnit(Int32 iUnit, Int32 *pCount, Int32 **pElements)
{
	Int32 lUnitPos = -1;
	for (Int32 i = 0; i < mnUnits; i++)
	{
		if (iUnit == mpUnits[i])
		{
			lUnitPos = i;
			break;
		}
	}
    
	if (lUnitPos < 0)
	{
		*pCount = 0;
		*pElements = NULL;
		return S_OK;
	}
    
	// use ElementIndex to calculate the mask
	ElementIndex *lpElementIndex;
	HRESULT hr = hGetElementIndexForUnit(lUnitPos, &lpElementIndex);
	CHECK_HR;
    
	*pCount = lpElementIndex->mnElements;
	*pElements = lpElementIndex->mpElements;
	return S_OK;
}

int DSSTabularIndex::getMaxKey(int iUnit)
{
    assert(iUnit < mvMaxKeys.size());
    return mvMaxKeys[iUnit];
}
int DSSTabularIndex::setMaxKeys(vector<int> ivMaxKeys)
{
    mvMaxKeys = ivMaxKeys;
    return S_OK;
}

int DSSTabularIndex::addKeyDistribution(int *lpKeyDistribution)
{
    mvKeyDistributions.push_back(lpKeyDistribution);
    return S_OK;
}

void DSSTabularIndex::GetCMIndexInfo(CMIndexInfo **oppCMIndexInfo)
{
    CMIndexInfo *lpCMIndexInfo = new CMIndexInfo();
    
    lpCMIndexInfo->mnUnits = mnUnits;
    lpCMIndexInfo->mnRowCount = mnRows;
    lpCMIndexInfo->mvUnits.resize(mnUnits);
    
    memmove(&(lpCMIndexInfo->mvUnits[0]), mpUnits, mnUnits*sizeof(int));
    
    
    lpCMIndexInfo->mMMapInfo[0] = mOffset;
    lpCMIndexInfo->mMMapInfo[1] = mOffsetAligned;
    lpCMIndexInfo->mMMapInfo[2] = mMMPBufSize;
    
    if(mpHashTable)
    {
        lpCMIndexInfo->mMMapInfo[3] = mpHashTable->GetOffset();
        lpCMIndexInfo->mMMapInfo[4] = mpHashTable->GetOffsetAligned();
        lpCMIndexInfo->mMMapInfo[5] = mpHashTable->GetMMPBufSize();
    }
    else
    {
        lpCMIndexInfo->mMMapInfo[3] = -1;
        lpCMIndexInfo->mMMapInfo[4] = -1;
        lpCMIndexInfo->mMMapInfo[5] = 0;
    }
    
    *oppCMIndexInfo = lpCMIndexInfo;
    
    return ;
}

int DSSTabularIndex::GetMaxKeyDistribution(char **oppBinary, int *opSize)
{
    if(oppBinary == NULL || opSize == NULL)
        return E_POINTER;
    
    DSSBinaryDataWriter *lpWriter = new DSSBinaryDataWriter();
    MBase::StrongPtr<DSSBinaryDataWriter> lSPWriter(lpWriter);
    
    if(mvMaxKeys.size() > 0)
    {
        lpWriter->WriteInt(mvMaxKeys.size());
        int pos = lpWriter->GetPosition();
        lpWriter->WriteByteArrayAtOffset(pos, (const unsigned char *)&(mvMaxKeys[0]), mvMaxKeys.size() * sizeof(int));
    }
    else
    {
        lpWriter->WriteInt(0);
    }
    
    if(mvKeyDistributions.size() != mnUnits)
        hCalcKeyDistributions();
    
    for (int i=0; i<mnUnits; i++)
    {
        if(mvKeyDistributions[i])
        {
            lpWriter->WriteInt(mvMaxKeys[i]+1);
            
            int pos = lpWriter->GetPosition();
            
            lpWriter->WriteByteArrayAtOffset(pos, (const unsigned char *)(mvKeyDistributions[i]), (mvMaxKeys[i]+1)*sizeof(int));
        }
        else
        {
            lpWriter->WriteInt(0);
        }
    }
    
    int hr = lpWriter->getBinary((unsigned char **)oppBinary, opSize);
    CHECK_HR;
    
    return S_OK;
}

// 1/31/2012 tehe: return integer count of max key and key distribution
int DSSTabularIndex::SetMaxKeyDistribution(const char * ipMaxKeyDistribution, int iDataSize, int &oMaxKeyDistributionSize)
{
    CHECK_PTR(ipMaxKeyDistribution);
    
    if(iDataSize < sizeof(int))
        return S_FALSE;
    
    const int * lpTmpIndex = reinterpret_cast<const int*>(ipMaxKeyDistribution);
    
    int lTmpSize = *lpTmpIndex++;
    
    iDataSize -= sizeof(int);
    
    if(lTmpSize > 0)
    {
        if(iDataSize < mnUnits * sizeof(int))
            return S_FALSE;
        
        mvMaxKeys.resize(lTmpSize);
        memmove(&(mvMaxKeys[0]), lpTmpIndex, lTmpSize * sizeof(int));
        
        lpTmpIndex += lTmpSize;
    }
    
    mvKeyDistributions.resize(mnUnits);
    
    for(int i=0;i<mnUnits;i++)
    {
        lTmpSize = *lpTmpIndex++;
        iDataSize -= 1;
        
        if(lTmpSize > 0)
        {
            if(iDataSize < (mvMaxKeys[i]+1) *sizeof(int))
                return S_FALSE;
            
            mvKeyDistributions[i] = new int[ mvMaxKeys[i] + 1];
            
            memmove(mvKeyDistributions[i], lpTmpIndex, (mvMaxKeys[i] + 1) * sizeof(int));
            
            lpTmpIndex += lTmpSize;
        }
    }
    
    oMaxKeyDistributionSize = (char*)lpTmpIndex - ipMaxKeyDistribution;
    
    return S_OK;
}


// tehe : init index by local mmap file
int DSSTabularIndex::Init(MBase::CString iFilePath, const CMIndexInfo * ipIndexInfo)
{
    int hr = 0;
	
	mnRows = ipIndexInfo->mnRowCount;
	mnUnits = ipIndexInfo->mnUnits;
	
	mpUnits = new int[mnUnits];
    CHECK_NEWED_PTR(mpUnits);
    
	memmove(mpUnits, &(ipIndexInfo->mvUnits[0]), sizeof(int) * mnUnits);
    
    mUnqualifiedRows.Init(mnRows, false);
    mHiddenRows.Init(mnRows, false);
    if (mnRows > 0) {
        mHiddenRows.BulkSet(0, mnRows -1, true);
    }
    
    mpLookUpKeyBuf = new int[mnUnits];
    CHECK_NEWED_PTR(mpLookUpKeyBuf);
    
    mbPartialLoading = false;
    
    // 9/5/2012 tehe
    mvElementCounts.resize(mnUnits, -1);
    
    mFilePath = iFilePath;
    mbUseMappedFile = true;
    
    mOffset = ipIndexInfo->mMMapInfo[0];
    mOffsetAligned = ipIndexInfo->mMMapInfo[1];
    mMMPBufSize = ipIndexInfo->mMMapInfo[2];
    
    bool ibNeedCalMaxKey = false;
    if(mvMaxKeys.size() == 0)
    {
        ibNeedCalMaxKey = true;
        mvMaxKeys.resize(mnUnits,0);
    }
    
    vector<int> lvBucketSpace(mnUnits);
    int lNumBuckets = 0;
    int *lpUnitBucketSpace = NULL;
    int *lpUnitMaxKey = NULL;
    
    if (mnUnits > 0)
    {
        lpUnitBucketSpace = &lvBucketSpace[0];
        lpUnitMaxKey = &mvMaxKeys[0];
    }
    
    if(mpHashTable)
    {
        delete mpHashTable;
        mpHashTable = NULL;
    }
    
    hr = reloadMMapMemory();
    CHECK_HR;
    
    if(ipIndexInfo->mMMapInfo[3] != -1 && ipIndexInfo->mMMapInfo[4] != -1 && ipIndexInfo->mMMapInfo[5] > 0)
    {
        DSSXTabHashTable *lpHashTable = new DSSXTabHashTable();
        MBase::StrongPtr<DSSXTabHashTable> lpHolder(lpHashTable);

        hr = lpHashTable->CheckKeyValues(mnUnits, mnRows, mpKeys, lpUnitBucketSpace, lNumBuckets, lpUnitMaxKey, ibNeedCalMaxKey);
        CHECK_HR;
        
        hr = lpHashTable->Init(lNumBuckets, mnUnits, lpUnitBucketSpace, mnRows, mpKeys);
        CHECK_HR;
        
        lpHashTable->SetUseMappedFile(true);
        lpHashTable->SetFilePath(iFilePath.c_str());
        lpHashTable->SetOffset(ipIndexInfo->mMMapInfo[3]);
        lpHashTable->SetOffsetAligned(ipIndexInfo->mMMapInfo[4]);
        lpHashTable->SetMMPBufSize(ipIndexInfo->mMMapInfo[5]);
        
        hr = lpHashTable->reloadMMapMemory();
        CHECK_HR;
        
        mpHashTable = lpHashTable;
        lpHolder.GiveUp();
    }
    else
    {
        // tehe: it may has not a hashtable, so we can't load the hashtable from local file
    }
    
    if(mvKeyDistributions.size() != mnUnits)
        hCalcKeyDistributions();
    
    return S_OK;
}

int DSSTabularIndex::SetElementIndex(int iUnitIndex, ElementIndex *ipElementIndex)
{
    if(mpvElementIndices == NULL)
	{
		mpvElementIndices = new vector<ElementIndex*>;
		CHECK_NEWED_PTR(mpvElementIndices);
        
		mpvElementIndices->resize(mnUnits, NULL);
	}
    
    (*mpvElementIndices)[iUnitIndex] = ipElementIndex;
    
    return S_OK;
    
    return S_OK;
}

int DSSTabularIndex::GetRawSize()
{
    //yimliu, 2014/07/21, 932953. The size calculation method should be the same as in DSSViewDataSet::hApplyLocalCacheManager()
    return sizeof(int) * mnRows * mnUnits;
    //return size();
}

bool DSSTabularIndex::IsDistinct()
{
    return mIsDistinct;
}

DSSElementBlocks* DSSTabularIndex::getElementBlocks(int iUnit)
{
    if (iUnit < 0)
        return mpRootBlock;
    int i = 0;
    for (i = 0; i < mnUnits; i++)
        if (iUnit == mpUnits[i])
            break;
    
    if (i >= mnUnits)
        return NULL;
    else
        return mvElementBlocks[i];
}

bool DSSTabularIndex::hasElementBlocks()
{
    return !mvElementBlocks.empty();
}

void DSSTabularIndex::SetUnqualified(int iRow, bool iFlag)
{
	_ASSERTE(iRow < mnRows);
    
	return mUnqualifiedRows.Set(iRow, iFlag);
}

int DSSTabularIndex::Populate(DSSTabularIndex *pOriginal,
                              Int32 *pUnitsMap,
                              Int32 nMissingUnit,
                              Int32 nRowLimit,
                              Int32 ibElementBrowseCount,
                              Int32** ippRowMap)
{
    int lnUnits = pOriginal->mnUnits + nMissingUnit;
    
	// weiwang 2009-06-22 element browse
	int lnRows = (ibElementBrowseCount >= 0) ? ibElementBrowseCount : pOriginal->mnRows;
    
	//xhan 10/12/05 add row limit
	if (nRowLimit > 0 && lnRows > nRowLimit)
	{
		//mpittkin: we now fail reports that exceed the limit
		return E_DEFN_EXCEED_LIMIT;
	}
    
    MEM_GOVERNOR_RESERVE(sizeof(DSSXTabKey) * lnUnits * lnRows, sizeof(DSSXTabKey) * lnUnits * lnRows);
	int *lpKey = new DSSXTabKey[(size_t)lnUnits * lnRows];    
	CHECK_NEWED_PTR(lpKey);
    MEM_GOVERNOR_COMMIT(sizeof(DSSXTabKey) * lnUnits * lnRows);
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lKeyPtr(lpKey);
    
    int *lpKeyTemp = lpKey;
    
	if (ibElementBrowseCount >= 0)
	{
		AE_ASSERT(lnUnits == 1);
		for (Int32 i = 0; i < ibElementBrowseCount; i++)
			lpKey[i] = i;
	}
	else if (false)//pOriginal->mpSpace)
	{
		// aye, 5/12/2009, IndexSpace support
		DSSXTabKeyIterator lIt(pOriginal);
		Int32 lnOrigUnits = pOriginal->mnUnits;
		for (Int32 i = 0; i < lnRows; i ++)
		{
			Int32 j = 0;
			for (; j < lnOrigUnits; j++)
				*lpKeyTemp++ = lIt.GetCol(j);
            
			for (j = 0; j < nMissingUnit; j++)
				*lpKeyTemp++ = 0;
            
			lIt.NextRow();
		}
	}
	else
	{
		if (nMissingUnit == 0)
			memmove(lpKey, pOriginal -> getKeyTable(), lnUnits * lnRows * sizeof(DSSXTabKey));
		else
		{
			memset(lpKey, 0, lnUnits * lnRows * sizeof(DSSXTabKey));
			for (Int32 i = 0; i < lnRows; i ++)
				memmove(lpKey + i * (size_t)lnUnits, (pOriginal->getKeyTable()) + (size_t)i * pOriginal->mnUnits,
                       pOriginal->mnUnits * sizeof(DSSXTabKey) );
		}
	}
    /*
	// weiwang 2009-07-23 TQMS367397
	if (ibElementBrowseCount == -1)		// not a element browse report
		mvMaxKeys = pOriginal->mvMaxKeys;
	else if (mvMaxKeys.size() == 0)		// mpXORHashTable
		mvMaxKeys.push_back((ibElementBrowseCount == 0) ? ibElementBrowseCount : ibElementBrowseCount - 1);
    
	// aye, 288661, 11/2/07
	// get the max keys values for the missing units
	for (Int32 i = 0; i < nMissingUnit; i++)
		mvMaxKeys.push_back(0);
    
    */
	// weiwang 2010-06-07 use global key to substitute local key
	if (ippRowMap)
	{
		Int32 lnOrigUnits = pOriginal->mnUnits;
		for (Int32 j = 0; j < lnOrigUnits; j++)
		{
			if (ippRowMap[j])
			{
				lpKeyTemp = lpKey + j;
				for (Int32 i = 0; i < lnRows; i++)
				{
					*lpKeyTemp = ippRowMap[j][*lpKeyTemp];
                    /*
					if ((*lpKeyTemp) > mvMaxKeys[j])
						mvMaxKeys[j] = (*lpKeyTemp);
                    */
					lpKeyTemp += lnUnits;
				}
			}
		}
	}
    
	int *lpUnits = new Int32[lnUnits];
	CHECK_NEWED_PTR(lpUnits);
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lpUnitsPtr(lpUnits);
    
	//mDEVersion = pOriginal->mDEVersion;  // copy the version over
	Int32 iUnit = 0;
	for( iUnit = 0; iUnit < pOriginal->mnUnits; iUnit ++)
	{
		lpUnits[iUnit] = pUnitsMap[pOriginal->mpUnits[iUnit]];
	}
    
	for (iUnit = 0; iUnit < nMissingUnit; iUnit ++)
	{
		lpUnits[iUnit + pOriginal->mnUnits] = iUnit + pOriginal->mnUnits;
	}
    
    int hr = Init(lnUnits, lnRows, lpUnits, lpKey, NULL);
    CHECK_HR;
    
	/*
    mHashType = CheckKeyValues();
    
	if (ibElementBrowseCount >= 0)
	{
		mUnqualifiedRows.Init(ibElementBrowseCount, true);
        
		// weiwang 2009-07-23 speed up hCalcKeyDistributions() for element browse report
		mvKeyDistributions.resize(1);
		Int32 *lpDist = new (mpBuffer -> p) Int32[ mvMaxKeys[0] + 1];
		memset(lpDist, 0, sizeof(Int32) * (mvMaxKeys[0] + 1));
		mvKeyDistributions[0] = lpDist;
		if (ibElementBrowseCount > 0)
		{
			for (Int32 i = mvMaxKeys[0]; i >= 0; i--)
				lpDist[i] = i + 1;
		}
	}
	else
	{
		mUnqualifiedRows.Populate(pOriginal->GetUnqualifiedRows());
		hCalcKeyDistributions();
	}
    */

	return S_OK;
}

// this method is called in incremental update, decompressing the keys for the selected units
// the input rowmap vector are the rowmaps for all units in the tabulardata this indices belongs to, which are generated after the lookup table merge
// the output column is decided by the ipRowMap, if it is NULL, the output key column is the same length/order of the original one
// if ipRowMap is provided, then the output length will use the provided one, and the keys are directed by ipRowMap
int DSSTabularIndex::DecompressKeys(DSSXTabBitVector* ipMask, Int32 nUnits, Int32* pUnits,
                                    Int32** ippGlobalLUIndex, Int32* ipRowMap, Int32 nResultRow,
                                    vector<Int32*>& ovKeys, Int32 iDefaultValue)
{    
	Int32 lnResultRow = mnRows;
    
	if (ipRowMap != NULL)
		lnResultRow = nResultRow;
    
	DSSXTabKeyIterator lIt(this);
    
	Int32 lnUnits = nUnits;
	if (nUnits == -1)
		lnUnits = mnUnits;
    
	bool lfUseElementBlock = false;
	if (!mvElementBlocks.empty() && ipMask == NULL && ipRowMap == NULL && mnRows > 0) // if pure decompress from Cube, we should do as quick as possible
		lfUseElementBlock = true;
    
	Int32 lKey = 0;
	for (Int32 iUnit = 0; iUnit < lnUnits; iUnit ++)
	{
		Int32* lpRow = ipRowMap;
        
		Int32* lpKey = new Int32[lnResultRow];
		CHECK_NEWED_PTR(lpKey);
        
		ovKeys.push_back(lpKey);
        
		Int32 lCol = pUnits ? pUnits[iUnit] : iUnit;
        
		Int32* lpRowMap = ippGlobalLUIndex? ippGlobalLUIndex[mpUnits[lCol]] : NULL;
		Int32 lRow = 0;
        
		if (lfUseElementBlock)
		{
			DSSElementBlocks* lpBlocks = mvElementBlocks[lCol];
			Int32 *lpBlockAddress = lpBlocks -> GetStartAddressPtr();
			Int32 lnKey = lpBlocks->GetNumKeys();
			for (Int32 iKey = 0; iKey < lnKey; iKey++)
			{
				Int32 *lpStartBlock, *lpEndBlock;
				lpBlocks -> GetBlocksForKey(iKey, lpStartBlock, lpEndBlock);
                
				Int32 lStartBlock = *lpStartBlock;
				Int32 lRow = lpBlockAddress ? lpBlockAddress[lStartBlock] : lStartBlock;
				lIt.SetRow(lRow);
                
				// using this approach, for the same key, we only need to decompress it once
				Int32 lKey = lIt.GetCol(lCol);
				if (lpRowMap)
					lKey = lpRowMap[lKey];
                
				while (lpStartBlock != lpEndBlock)
				{
					Int32 lBlock = *lpStartBlock++;
                    
					if (lpBlockAddress)
					{
						for (Int32 iRow = lpBlockAddress[lBlock]; iRow < lpBlockAddress[lBlock+1]; iRow++)
							lpKey[iRow] = lKey;
					}
					else
						lpKey[lBlock] = lKey;
				}
                
			}
		}
		else
		{
			memset(lpKey, iDefaultValue, sizeof(Int32)*lnResultRow);
            
			if (ipRowMap != NULL)
			{
				lIt.SetRow(0);
				for (Int32 iRow = 0; iRow < mnRows; iRow++, lIt.NextRow())
				{
					lRow = *lpRow++;
					if (lRow == -1)
						continue;
                    
					lKey = lIt.GetCol(lCol);
					if (lpRowMap)
						lKey = lpRowMap[lKey];
                    
					lpKey[lRow] = lKey;
				}
			}
			else
			{
				lIt.SetRow(0);
				for (Int32 iRow = 0; iRow < mnRows; iRow ++, lIt.NextRow())
				{
					if (ipMask && ipMask->Get(iRow) == false)
						continue;
                    
					lRow = lIt.GetCol(lCol);
					lKey = lpRowMap ? lpRowMap[lRow] : lRow;
                    
					lpKey[iRow] = lKey;
				}
			}
            
		}
	}
    
	return S_OK;
}

// Class CDSSXTabIndices

//only reset keys, don't touch rowlist. used for transaction cube udpate, need to recalculate hash table
//fali, 960053, Performance. add parameter lbReCalKeyDistribution, if GenDistinct which includes hCalcKeyDistributions was called before, skip hCalcKeyDistributions.
int DSSTabularIndex::ResetHeaderKeys(int* ipKeys, int iSize, int inRows, bool lbReCalKeyDistribution, bool lbMatchKeys)
{
    int hr = S_OK;
    
    //TODO: do we support partial loaded index in new rwd?
    if(mbPartialLoading)
        return S_FALSE;
    
    //don't support change the number of units of the index
    if(iSize != inRows * mnUnits)
        return S_FALSE;
    
    //979089, if the new key is not the last one in the attribute lookup, we need to match the rows before resetting the keys, assume these two arrays only have one mismatched element, and ipKeys is always sorted
    bool lbReseted = false;
    if(lbMatchKeys && mnUnits == 1 && inRows == mnRows)
    {
        int i = mnRows-1; int j = mnRows-1;
        int mMismatch = -1; int iMismatch = -1;
        vector<int> lvKeys(mnRows,0);
        memmove(&lvKeys[0], mpKeys, sizeof(int) * mnRows);
        std::sort(lvKeys.begin(), lvKeys.end());
        for(; i >= 0 && j >= 0; i--)
        {
            while(lvKeys[i] != ipKeys[j] && j>= 0)
            {
                j --;
            }
            if(j < 0)
            {
                mMismatch = i;
                break;
            }
        }

        if(mMismatch >= 0)
        {
            for(i = mnRows-1, j = mnRows - 1; i >= 0 && j >= 0; j--)
            {
                while(lvKeys[i] != ipKeys[j] && i>= 0)
                {
                    i --;
                }
                if(i < 0)
                {
                    iMismatch = j;
                    break;
                }
            }
        }
        //verify if there is only one mismatch
        if(iMismatch >= 0)
        {
            bool moreMismatch = false;
            i = 0; j = 0;
            while (i < mnRows && j < mnRows)
            {
                if(i == mMismatch)
                {
                    i++;
                    continue;
                }
                if(j == iMismatch)
                {
                    j++;
                    continue;
                }
                if(lvKeys[i++] != ipKeys[j++])
                {
                    moreMismatch = true;
                    break;
                }
            }
            
            if(!moreMismatch)
            {
                for(int i = 0; i < mnRows; i++)
                {
                    if(mpKeys[i] == lvKeys[mMismatch])
                    {
                        mpKeys[i] = ipKeys[iMismatch];
                        lbReseted = true;
                        break;
                    }
                }
            }
        }
    }
    
    if(!lbReseted)
    {
        if(mpKeys && iSize != mnUnits * mnRows)
        {
            delete [] mpKeys;
            mpKeys = new int [iSize];
        }
        memmove(mpKeys, ipKeys, sizeof(int) * iSize);
    }
    
    mnRows = inRows;
    

    vector<int> lvBucketSpace(mnUnits);
    int lNumBuckets = 0;
    int *lpUnitBucketSpace = NULL;
    int *lpUnitMaxKey = NULL;
    
    if (mnUnits > 0)
    {
        memset(&mvMaxKeys[0], 0, sizeof(int)*mnUnits);
        lpUnitBucketSpace = &lvBucketSpace[0];
        lpUnitMaxKey = &mvMaxKeys[0];
    }
    
    if(mpHashTable)
        delete mpHashTable;
    
    mpHashTable = new DSSXTabHashTable();
    
    hr = mpHashTable->CheckKeyValues(mnUnits, mnRows, mpKeys, lpUnitBucketSpace, lNumBuckets, lpUnitMaxKey);
    CHECK_HR;
    
    hr = mpHashTable->Init(lNumBuckets, mnUnits, lpUnitBucketSpace);
    CHECK_HR;
    
    hr = mpHashTable->HashAll(mpKeys, mnUnits, mnRows);
    CHECK_HR;

    if (lbReCalKeyDistribution)
    {
        hCalcKeyDistributions(); // jimzhang, temp code, uncomment this line of code
    }    
    
    ClearElementIndex();//xiazhou, Element index is invalid now.
    
    return S_OK;
}
 
bool DSSTabularIndex::isOrderedLinearKey()
{
    if (mnUnits == 1) {
        int i = 0;
        int* lpKey = mpKeys;
        for (; i < mnRows; i++) {
            if (*lpKey != i)
            {
                break;
            }
            lpKey += mnUnits;
        }
        if (i == mnRows) {
            return true;
        }
    }
    return false;
}
