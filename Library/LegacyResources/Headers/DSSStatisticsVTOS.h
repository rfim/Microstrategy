//
//  DSSStatisticsVTOS.h
//  ReportServiceCore
//
//  Created by He Tengfei on 3/7/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//
#ifndef DSSStatisticsVTOS_h
#define DSSStatisticsVTOS_h 1

#include "DSSAggregationFunction.h"

class DSSStatisticsVTOS : public DSSAggregationFunction
{
public:
    DSSStatisticsVTOS();
    
    ~DSSStatisticsVTOS();
    
    //	Derived class should implement this method
    int Calculate(Int32 nSize, double *pVec, double *opResult, DSSDataFlag *pFlag);
    
    int CalculateRepeat(Int32 nSize, Int32 nRepeat, Int32 *pStartPoint, double *pVec, double *opResult, DSSDataFlag *pFlag);
    
    //	The derived class should implemented this method.
    int setProperty(Int32 Index, CComVariant *pValue);
    
    int Init(Int32 iFunctionIndex);
private:
    Int32 mFunctionIndex;
};
#endif