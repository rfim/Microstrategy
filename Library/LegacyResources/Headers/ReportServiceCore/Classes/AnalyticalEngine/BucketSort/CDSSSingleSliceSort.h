//
//  CDSSSingleSliceSort.h
//  ReportServiceCore
//
//  Created by Liao Taimin on 5/15/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_CDSSSingleSliceSort_h
#define ReportServiceCore_CDSSSingleSliceSort_h

#include "CDSSBucketSort.h"

class DSSSortDefinition;
class DSSTabularIndex;
class DSSSliceCorrelator;
class DSSCubeDataSlice;
class DSSCube;

class CDSSSingleSliceSort : public CDSSBucketSort
{
    
public:
    CDSSSingleSliceSort(Int32 nMetrics, Int32 nUnit);
    
    ~CDSSSingleSliceSort();
    
    virtual Int32 Sort(Int32 **ppInputResult, Int32 **ppAuxArray, Int32 *pBucketLinkList, Int32 nSize);
    
    // Additional Public Declarations
    Int32 Init(DSSSortDefinition *pDefn, DSSTabularIndex* ipLookupIndex,  DSSSliceCorrelator *ipSliceCorrelator, DSSCubeDataSlice *ipSlice, DSSCube *ipCube);
    
	bool operator()(Int32 row1, Int32 row2);
    
    
private:
    // Data Members for Class Attributes
    
    Int32 Order(Int32 row1, Int32 row2);
    
    //Come from sort definition
    bool mfAscending;
    bool mfIsNullDisplayOnTop;
    // 05/14/2008 mwang
    // support for sorting Null
    EnumDSSOLAPNullPosition mNullPosition;
    
    Int32 mnRow;
    Int32 *mpKey;
    DSSCubeDataSlice *mpSlice;
    
    
    //temporary use
    Int32 mRowSize;
    //unsigned char *mpBuf1;
    //unsigned char *mpBuf2;
    
    DSSSliceCorrelator *mpSliceCorrelator;
};

#endif
