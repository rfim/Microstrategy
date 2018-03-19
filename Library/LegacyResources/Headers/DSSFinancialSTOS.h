//
//  DSSFinancialSTOS.h
//  ReportServiceCore
//
//  Created by He Tengfei on 2/22/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//
#ifndef DSSFinancialSTOS_h
#define DSSFinancialSTOS_h 1

#include "DSSSimpleFunction.h"

class DSSFinancialSTOS : public DSSSimpleFunction
{
public:
    DSSFinancialSTOS();
    
    ~DSSFinancialSTOS();
    
    int Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag);
    
    int Init(int iIndex);
    
    int setProperty (Int32 iIndex, CComVariant *ipValue);

private:
    int mFunctionIndex;
    
    double mProperties[3];
};

#endif
