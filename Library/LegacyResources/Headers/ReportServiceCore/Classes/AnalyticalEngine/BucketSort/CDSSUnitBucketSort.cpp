//
//  CDSSUnitBucketSort.cpp
//  ReportServiceCore
//
//  Created by Liao Taimin on 5/15/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#include "CDSSUnitBucketSort.h"
#include "DSSAEHelper.h"
#import "DSSStrongPtr.h"

CDSSUnitBucketSort::CDSSUnitBucketSort(Int32 nMetrics, Int32 nUnit)
: CDSSBucketSort(nMetrics, nUnit), mSubtotalPos(DssSubtotalsPositionLast),
mpBucketLookupTable(NULL), mnBucket(0), mAttrIndex(-1)
{
}

CDSSUnitBucketSort::~CDSSUnitBucketSort()
{
    //tliao, 912747, 2014/08/04, fix memory leak
    if (mpBucketLookupTable)
    {
        delete [] mpBucketLookupTable;
    }
}


//sort display rows according to bucket table
//ppInputResult will hold input array pointer at the beginning and ouptput array at end
//ppAuxArray will hold incomplete output array during the sorting
//To avoid memcpy, ppInputResult and ppAuxArray will swap at the end
int CDSSUnitBucketSort::Sort(Int32 **ppInputResult, Int32 **ppAuxArray,
                                 Int32 *pBucketLinkList, Int32 nSize)
{
	//link list for the buckets, the last one is for subtotal
	Int32 *lpStart = new Int32[mnBucket + 1];
	CHECK_NEWED_PTR(lpStart);
    //tliao, 912747, 2014/08/04, fix memory leak
    MBase::DSSStrongPtr<Int32, MBase::DeleteArray<Int32> > lStartPtr(lpStart);
    
	Int32 *lpEnd = new Int32[mnBucket + 1];
	CHECK_NEWED_PTR(lpEnd);
    MBase::DSSStrongPtr<Int32, MBase::DeleteArray<Int32> > lEndPtr(lpEnd);
    
	Int32 iRow = 0;
	for (iRow = 0;  iRow < mnBucket + 1; iRow ++)
		lpStart[iRow] = lpEnd[iRow] = -1;
    
	for (iRow = 0;  iRow < nSize; iRow ++)
		pBucketLinkList[iRow] = -1;
    
	Int32 *lpDisplayRow = *ppInputResult;
    
	//distribute the rows among the buckets
	for(iRow = 0; iRow < nSize; iRow++)	{
        
		//need to transform iRow first
		// lweng, 2012-5-15, TQMS 591559, the multiplication result should be unsigned int
		Int32 key = mKey[MapRow(lpDisplayRow[iRow]) * (size_t)mnUnit + mAttrIndex];
        
		Int32 bucket = 0;
		if (key == DSSXTAB_TOTAL_KEY) {//put it to the subtotal link list
			bucket = mnBucket;
		} else {
			bucket = mpBucketLookupTable[key];
		}
        
		assert(bucket >= 0 && bucket <= mnBucket);
        
		if(lpStart[bucket] == -1) { // if the list is empty
			lpStart[bucket] = iRow;
			lpEnd[bucket] = iRow;
		} else {
			pBucketLinkList[lpEnd[bucket]] = iRow;
			lpEnd[bucket] = iRow;
		}
        
	} //for
    
	Int32 *lpTemp = *ppAuxArray;
	if (mSubtotalPos == DssSubtotalsPositionFirst) {
		//put subtotal first
		lpTemp = Collect(lpTemp, lpDisplayRow, pBucketLinkList, lpStart[mnBucket]);
        
		for (iRow = 0; iRow < mnBucket; iRow ++)
			lpTemp = Collect(lpTemp, lpDisplayRow, pBucketLinkList, lpStart[iRow]);
        
	} else { // put subtotal last
        
		for (iRow = 0; iRow < mnBucket + 1; iRow ++)
			lpTemp = Collect(lpTemp, lpDisplayRow, pBucketLinkList, lpStart[iRow]);
        
	}
    
	//The sort result is in ppAuxArray until now
	//swap ppAuxArray and ppInputResult
	lpTemp = *ppAuxArray;
	*ppAuxArray = *ppInputResult;
	*ppInputResult = lpTemp;
    
    
	return S_OK;
    
}