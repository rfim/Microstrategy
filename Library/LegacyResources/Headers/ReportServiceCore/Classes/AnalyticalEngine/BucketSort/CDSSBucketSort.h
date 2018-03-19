//
//  CDSSBucketSort.h
//  ReportServiceCore
//
//  Created by Liao Taimin on 5/15/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_CDSSBucketSort_h
#define ReportServiceCore_CDSSBucketSort_h

#include "DSSAEHelper.h"

class CDSSBucketSort
{
    
public:
    CDSSBucketSort(Int32 nMetrics, Int32 nUnit) {mnMetrics = nMetrics; mnUnit = nUnit; mbIgnoreHidden = true; }
    //tliao, 912747, 2014/08/04, fix memory leak
    virtual ~CDSSBucketSort() {}
    
    virtual Int32 Sort(Int32 **ppInputResult, Int32 **ppAuxArray, Int32 *pBucketLinkList, Int32 nSize) = 0;
    
    
    inline virtual bool operator() (Int32 Index1, Int32 Index2);
    
    inline void SetIgnoreHiddenFlag(bool ibIgnoreHidden);
    
protected:
    
    Int32 mnMetrics; //how many metrics needed to be considered.
	
    Int32 mnUnit; //how many units in xtabheader
    
    // weiwang 2009-08-12 TQMS352257 Flag for DE to use hidden element to sort or not
    bool mbIgnoreHidden;
    
protected:  //## implementation
    inline Int32 MapRow(Int32 nDisplayRow);
    
    //utility function
    inline Int32 * Collect(Int32 *pResRow, Int32 *pDisplayRow, Int32 *pGroupLinkList, Int32 bucketStart);
    
    
};

// Class CDSSBucketSort
//Map the row if there are multiple metrics
Int32 CDSSBucketSort::MapRow(Int32 nDisplayRow)
{
	return ( (mnMetrics <= 1) ? nDisplayRow : nDisplayRow / mnMetrics);
}

Int32 * CDSSBucketSort::Collect(Int32 *pResRow, Int32 *pDisplayRow, Int32 *pGroupLinkList, Int32 bucketStart)
{    
	Int32 lBucketRow = bucketStart;
	while (lBucketRow != -1) {
		*pResRow = pDisplayRow[lBucketRow];
		pResRow ++;
		lBucketRow = pGroupLinkList[lBucketRow];
	}
    
	return pResRow;
}

bool CDSSBucketSort::operator() (Int32 Index1, Int32 Index2)
{
	_ASSERT(false);
	return false;
}

void CDSSBucketSort::SetIgnoreHiddenFlag(bool ibIgnoreHidden)
{
	mbIgnoreHidden = ibIgnoreHidden;
}

#endif
