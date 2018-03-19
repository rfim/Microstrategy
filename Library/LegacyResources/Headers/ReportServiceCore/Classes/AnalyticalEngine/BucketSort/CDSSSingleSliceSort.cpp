//
//  CDSSSingleSliceSort.cpp
//  ReportServiceCore
//
//  Created by Liao Taimin on 5/15/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#include "CDSSSingleSliceSort.h"
#import "DSSSortDefinition.h"
#import "DSSTabularIndex.h"
#import "DSSSliceCorrelator.h"
#import "DSSCubeDataSlice.h"
#import "DSSCube.h"
#import "DSSAEHelper.h"
#import "DSSSliceID.h"
#include "DSSSort_includes.h"

CDSSSingleSliceSort::CDSSSingleSliceSort(Int32 nMetrics, Int32 nUnit) : CDSSBucketSort(nMetrics, nUnit),
mfAscending(true),  mfIsNullDisplayOnTop(false), mnRow(0),mRowSize(0)//, mpBuf1(NULL), mpBuf2(NULL)
{
    
}


CDSSSingleSliceSort::~CDSSSingleSliceSort()
{}


//## Other Operations (implementation)
bool CDSSSingleSliceSort::operator()(Int32 row1, Int32 row2){
	return (Order(row1, row2) < 0);
}

Int32 CDSSSingleSliceSort::Order(Int32 row1, Int32 row2)
{
	Int32 hr = S_OK;
    
 	row1 = MapRow(row1);
	row2 = MapRow(row2);
    
	assert(row1 < mnRow && row2 < mnRow);
    
	Int32 lDataSize1 = 0;
	Int32 lDataSize2 = 0;
	DSSDataType_Type lType1 = DssDataTypeUnknown;
	DSSDataType_Type lType2 = DssDataTypeUnknown;
	DSSData_Flags lFlag = DssDataUnknown;
	bool lbInvalid1 = false;
	bool lbInvalid2 = false;
    
    BYTE *lpBuf1 = NULL;
	hr = mpSliceCorrelator->GetInputValue(0, row1, &lpBuf1, mRowSize, &lDataSize1, &lType1, &lFlag);
	if ( FAILED(hr) )
	{
		assert(false);
        return 0;
	}
    
	if (hr != S_OK || lFlag != DssDataOk)
		lbInvalid1 = true;
    
    BYTE *lpBuf2 = NULL;
	hr = mpSliceCorrelator->GetInputValue(0, row2, &lpBuf2, mRowSize, &lDataSize2, &lType2, &lFlag);
	if ( FAILED(hr) )
	{
		assert(false);
        return 0;
	}
    
	if (hr != S_OK || lFlag != DssDataOk)
		lbInvalid2 = true;    
    
	// check for NULLs
	// 05/14/2008 mwang sort the Nulls
	if (lbInvalid1 != lbInvalid2)
	{
		switch (mNullPosition)
		{
            case DssOLAPNullPositionLast:
                return lbInvalid2 ? -1 : 1;
            case DssOLAPNullPositionFirst:
                return lbInvalid1 ? -1 : 1;
            case DssOLAPNullPositionLargest:
                return (mfAscending && lbInvalid2) ? -1 : 1;
            case DssOLAPNullPositionSmallest:
                return (mfAscending && lbInvalid1) ? -1 : 1;
                
		}
	}
	else if (lbInvalid1 && lbInvalid2)
		return 0;
    
	// compare the data
	// ytang, TQMS345984, Jan-30-2009
	// Need datra sizes for Binary data types
	Int32 result = DSSAEHelper::CompareData(lType1, lpBuf1, lDataSize1, lType2, lpBuf2, lDataSize2);
    
	if (mfAscending) return result;
	else return -result;
	
    
}


// Additional Declarations
//## begin CDSSSingleSliceSort.declarations preserve=yes
Int32 CDSSSingleSliceSort::Init(DSSSortDefinition *pDefn, DSSTabularIndex* ipLookupIndex,  DSSSliceCorrelator *ipSliceCorrelator, DSSCubeDataSlice *ipSlice, DSSCube *ipCube)
{
	AE_ASSERT(pDefn != NULL);
	AE_ASSERT(ipSliceCorrelator != NULL);
	AE_ASSERT(ipLookupIndex);
    
    int hr = S_OK;
    
	mpSliceCorrelator = ipSliceCorrelator;
	mnRow = ipLookupIndex->Count();
    
	DSSCubeDataSlice *lpSlice;
	if (ipSlice)
		lpSlice = ipSlice;
	else
	{
		DSSSliceID lSliceID = pDefn->mSliceID;
        
		hr = lSliceID.GetSlice(&lpSlice);
		CHECK_HR_PTR(lpSlice);
	}
	// for simplicity, set the lpSlice as both the input and the output slice  of the SliceCorrelator, although it's only the input
	// yma 5/27/09, TQMS 361040
	// make sure a rowmap exists for retrieving the slice value
	hr = ipSliceCorrelator->Init(ipLookupIndex, 1, &lpSlice, lpSlice, ipCube, true);
	CHECK_HR;
    
	mfAscending = (pDefn->isAscending == true);
    
	mfIsNullDisplayOnTop = (pDefn->isNullOnTop == true);
    
	hr = lpSlice->getSizeOfData(mRowSize);
	CHECK_HR;
    
	if (mRowSize < sizeof(double))
		mRowSize = sizeof(double);
    
	//mpBuf1 = new unsigned char [mRowSize];
	//mpBuf2 = new unsigned char [mRowSize];
    
	mNullPosition = pDefn->mNullPosition;
	
	return hr = S_OK;
    
}

Int32 CDSSSingleSliceSort::Sort(Int32 **ppInputResult, Int32 **ppAuxArray,
								  Int32 *pBucketLinkList, Int32 nSize)
{	
	AE_ASSERT(nSize == mnRow);
    
	if (_xtab_sort(*ppInputResult, *ppInputResult + mnRow, *this) != S_OK)
		return E_FAIL;
    
	return S_OK;
}