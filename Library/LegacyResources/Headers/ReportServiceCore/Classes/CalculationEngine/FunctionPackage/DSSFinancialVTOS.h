//
//  DSSFinancialVTOS.h
//  ReportServiceCore
//
//  Created by He Tengfei on 2/22/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//
#ifndef DSSFinancialVTOS_h
#define DSSFinancialVTOS_h 1

#include "DSSAggregationFunction.h"

class DSSFinancialVTOS : public DSSAggregationFunction
{
public:
    DSSFinancialVTOS();
    
    ~DSSFinancialVTOS();
    
    int Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag);

    // int CalculateRepeat(int nParam, int nRepeat, double **ipParams, double *opResult, DSSDataFlag *opFlag);
    
    int setProperty (Int32 iIndex, CComVariant *ipValue);
    
    int Init(int iIndex);

private:
    Int32 mFunctionIndex;
    double mProperty;
    Int32 mMethod;
    double mPrecision;
};
#endif