//
//  DSSRunningNNGeneric.h
//  ReportServiceCore
//
//  Created by Guanlin Shen on 01/26/01.
//  Copyright 2001 MicroStrategy, Inc. All rights reserved.
//

#ifndef RunningNNGeneric_h
#define RunningNNGeneric_h 1

#import "DSSNNGenericFunction.h"
#import "Base/Defines/StrongPtr.h"

class DSSRunningNNGeneric : public DSSNNGenericFunction
{    
public:
    DSSRunningNNGeneric();
    
    ~DSSRunningNNGeneric();    
    
    //	There is only one property for this function object. The
    //	possible values are :
    //	    DssRelationalOpen
    //	    DssRelationalCloseFloor
    //	    DssRelationalCloseCeiling
    //	    DssRelationalCloseRound
    virtual int setProperty (Int32 Index, CComVariant *pValue);
    
    //	This method is to accomplish the calculation.
    int Calculate ();
    
    //	This method is to set the input parameter.
    int SetInputParam (Int32 nParamIndex, Int32 nSize, EnumDSSDataType dataType, CComVariant *pData, DSSDataFlag *pFlag);
    
    int GetOutput (Int32 nSize,         // Input buffer size
                   Int32 *nDataSize, 	// return data size
                   CComVariant *pData,      // Double array stores the result of function
                   DSSDataFlag *pFlag	// Output flags
                   );
    
    int GetOutputType (Int32 nSize, EnumDSSDataType *pInputType, EnumDSSDataType *pOutputDataType, EnumDSSParameterType *pOutParamType);
    
    int GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType);
    
    int IsNullProcessable (bool *pCanProcessNULL);
    
    int Clear ();
        
    // jxie, 2010-9-29
    // support numeric interface
    int GetOutput(Int32 nSize, Int32 *nDataSize,  double * pData, DSSDataFlag * pFlag);
    
    int GetOutputType(EnumDSSParameterType * pType);
    
    int SetInputParam(Int32 nParamIndex, Int32 nSize, double * pData, DSSDataFlag * pFlag);
        
private:
    Int32 mDataSize;    //	Size of the input metric    
    MBase::StrongArrayPtr<DSSDataFlag> mpDataFlagPtr;    
    CComVariant mInputData;  //Input variant safe array    
    CComVariant mResultData;  //Result variant    
    MBase::StrongArrayPtr<DSSDataFlag> mpResultFlagPtr;      
	
    // jxie, 2010-9-29
    bool mfIsNumeric;
    double* mpNumericResultData;
    double* mpNumericInputData;
    
private: 
    // Additional Implementation Declarations
	int CalculateRunMaxMin();
	int CalculateRunCount();
	int CalculateFirst();
	int CalculateLast();
    
	// jxie, 2010-9-29
	// functions for numeric interface
	int hNumericCalculate();
	int hNumericCalculateRunMaxMin();
	int hNumericCalculateRunCount();
	int hNumericCalculateFirst();
	int hNumericCalculateLast();
};


#endif