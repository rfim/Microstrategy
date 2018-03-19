//
//  DSSStatisticsSTOS.h
//  ReportServiceCore
//
//  Created by He Tengfei on 3/7/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//
#ifndef DSSStatisticsSTOS_h
#define DSSStatisticsSTOS_h 1

#include "DSSSimpleFunction.h"

class DSSStatisticsSTOS :public DSSSimpleFunction
{
public:
   
    DSSStatisticsSTOS();
    
    ~DSSStatisticsSTOS();

    //	Derived class must implement this method.
    int Calculate(Int32 nParam, double *pParam, double *pResult, DSSDataFlag *pFlag);
   
    //	The derived class should implemented this method.
    int setProperty(Int32 Index, CComVariant *pValue);
       
    int Init(Int32 iFunctionIndex);

private:
    Int32 mFunctionIndex;
    bool mCumulative;
    double mLowerBound;
    double mUpperBound;
};

#endif