//
//  DSSNNFinancialSTOS.h
//  ReportServiceCore
//
//  Created by He Tengfei on 2/22/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//
#ifndef DSSNNFinancialSTOS_h
#define DSSNNFinancialSTOS_h 1

#include "DSSNNSimpleFunction.h"

class DSSNNFinancialSTOS : public DSSNNSimpleFunction
{
public:
    DSSNNFinancialSTOS();
    
    ~DSSNNFinancialSTOS();
    
    int Calculate(int nParam, CComVariant *ipParams, CComVariant *opResult, DSSDataFlag *opFlag);
    
    int Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag);
    
    int GetOutputType (int size, EnumDSSDataType *inputType, EnumDSSDataType *outputType);
    
    int Init(int iIndex);
    
    int setProperty (Int32 iIndex, CComVariant *ipValue);
    
    int getDateFromVariant(VARIANT *ipParams, LONG index, double* outDate);
private:
    Int32 mFunctionIndex;
    
    double mProperties[3];
    
    int getParamFromVariant(VARIANT *pParam, LONG index, EnumDSSDataType type, BYTE *outResult,Int32 size);
};

#endif