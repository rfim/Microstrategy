//
//  DSSFinancialVSSTOS.h
//  ReportServiceCore
//
//  Created by He Tengfei on 2/22/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#ifndef DSSFinancialVSSTOS_h
#define DSSFinancialVSSTOS_h 1

#include "DSSGenericFunction.h"

class DSSFinancialVSSTOS: public DSSGenericFunction
{
public:
    DSSFinancialVSSTOS();
    
    ~DSSFinancialVSSTOS();
    
    int Calculate();    
    
    int GetOutput(Int32 nSize, 	// Input buffer size
                  Int32 *nDataSize, 	// return data size
                  double *pData, 	// Double array stores the result of function
                  DSSDataFlag *pFlag);	// Output flags) = 0; 
    
    int GetOutputType (EnumDSSParameterType *pType);
    
    int SetInputParam(Int32 nParamIndex, 	// The index of input parameter
                      Int32 nSize, 	// Size of input double array
                      double *pData,
                      DSSDataFlag *pFlag);
    
    int GetInputParamType(Int32 nParamIndex, EnumDSSParameterType *pType);    
    
    int Clear();    
    
    int IsNullProcessable(bool *pCanProcessNULL);
    
    int setProperty (Int32 Index, CComVariant *pValue);
    
    int Init (Int32 iFunctionIndex);

private:
    Int32 mFunctionIndex;
    
    double *mpValues;
    
    Int32	  mnDataSize;
    
    double  mfFinanicalRate;
    
    bool    mbFRReady;	// if Finanical Rate is initialized 
    
    double  mfReinvestmentRate;
    
    bool	  mbRRReady;	// if reinvestment rate is initialized
    
    double  mfResult;
    
    DSSDataFlag mFlag;
};

#endif