//
//  CDSSAttributeFormSort.h
//  ReportServiceCore
//
//  Created by Liao Taimin on 5/15/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_CDSSAttributeFormSort_h
#define ReportServiceCore_CDSSAttributeFormSort_h

#include "CDSSUnitBucketSort.h"
class DSSSortDefinition;
class DSSCube;
class DSSTabularData;
class DSSDataColumn;

class CDSSAttributeFormSort : public CDSSUnitBucketSort
{

public:
    CDSSAttributeFormSort(Int32 nMetrics, Int32 nUnit);
    
    ~CDSSAttributeFormSort();
    
    
    Int32 Init(DSSSortDefinition *pDefn, Int32 *Key, Int32 nSize, DSSCube *pCube);
    Int32 InitColumn(DSSSortDefinition *pDefn, Int32 *Key, Int32 nSize, DSSCube *pCube);
    
    bool operator ()(Int32 Index1, Int32 Index2);
    
private:
    
    Int32 hInitByColunm(DSSSortDefinition *pDefn, Int32 *Key, Int32 nSize, DSSTabularData *ipTabularData, Int32 iColIndex);
    
    
    // Data Members for Class Attributes
    
    //from sort defintion
    Int32 mfAscending;
    
    bool mfIsNullDisplayOnTop;
    
    Int32 mLocale;
    
    //From cube
    DSSDataColumn *mpCol;
    
};

#endif
