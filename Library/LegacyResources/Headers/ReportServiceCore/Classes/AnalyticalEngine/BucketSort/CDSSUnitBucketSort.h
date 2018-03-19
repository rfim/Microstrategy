//
//  CDSSUnitBucketSort.h
//  ReportServiceCore
//
//  Created by Liao Taimin on 5/15/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_CDSSUnitBucketSort_h
#define ReportServiceCore_CDSSUnitBucketSort_h

#include "CDSSBucketSort.h"

class CDSSUnitBucketSort : public CDSSBucketSort  
{
    
public:
    CDSSUnitBucketSort(Int32 nMetrics, Int32 nUnit);
    
    ~CDSSUnitBucketSort();
    
    //	Virtual method common to all Order Classes
    virtual Int32 Sort(Int32 **ppInputResult, Int32 **ppAuxArray, Int32 *pBucketLinkList, Int32 nSize);
    
protected:
    
    //come from sort defintion
    DSSSort_Subtotals mSubtotalPos;
    Int32 mAttrIndex;
    
    // constructed by the concrete unit
    Int32 *mpBucketLookupTable;
    Int32 mnBucket;
    
    // passed from xtab header
    Int32 *mKey; 
};


#endif
