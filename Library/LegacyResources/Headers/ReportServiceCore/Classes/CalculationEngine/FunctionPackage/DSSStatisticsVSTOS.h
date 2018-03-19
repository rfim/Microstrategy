//
//  DSSStatisticsVSTOS.h
//  ReportServiceCore
//
//  Created by He Tengfei on 3/7/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//
#ifndef DSSStatisticsVSTOS_h
#define DSSStatisticsVSTOS_h 1

#include "DSSGenericFunction.h"

#include "statistic.h"

class DSSStatisticsVSTOS :public DSSGenericFunction
{
public:
    DSSStatisticsVSTOS();
    
    ~DSSStatisticsVSTOS();
    
    int setProperty (Int32 Index, CComVariant *pValue);
    
    int Init (Int32 iFunctionIndex);
    
    int Calculate ();
    
    int GetOutput (Int32 nSize, Int32 *nDataSize, double *pData, DSSDataFlag *pFlag);
    
    int GetOutputType (EnumDSSParameterType *pType);
    
    int SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSDataFlag *pFlag);
    
    int GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType);
    
    int Clear ();
    
    int IsNullProcessable (bool *pCanProcessNULL);

private:
    Int32 mFunctionIndex;
    enum Hypothesis mHypothesis;
    Int32 mVSize;
    double *mV;
    double mS;

};

#endif