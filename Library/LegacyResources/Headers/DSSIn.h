//
//  DSSIn.h
//  ReportServiceCore
//
//  Created by Yang Zhuo on 7/26/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//
#ifndef CDSSIn_h
#define CDSSIn_h 1

#import "CalculationEngine/DSSNNSimpleFunction.h"

class DSSIn : public DSSNNSimpleFunction
{
    
public:
    DSSIn();
    ~DSSIn();    
    int Calculate (Int32 nSize, CComVariant *pVec, CComVariant *opResult, DSSDataFlag *pFlag);
    virtual int setProperty (Int32 Index, CComVariant *pValue);
    // int Init (EnumDSSFunction iFunctionIndex);
    int GetOutputType (Int32 size, EnumDSSDataType *inputType, EnumDSSDataType *outputType);
    int get_OperatorType(EnumDSSFunctionType * opType);
    
    //zhyang, 26/07/2011, the pure vitual function in DSSSimpleFunction, we need to impelment it.
    int Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag);
};



#endif