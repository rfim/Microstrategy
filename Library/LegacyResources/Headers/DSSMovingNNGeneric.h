//
//  DSSNNMovingGeneric.h
//  ReportServiceCore
//
//  Created by Guanlin Shen on 01/26/01.
//  Copyright 2001 MicroStrategy, Inc. All rights reserved.
//

#ifndef MovingNNGeneric_h
#define MovingNNGeneric_h 1

#import "DSSNNGenericFunction.h"

class DSSMovingNNGeneric : public DSSNNGenericFunction
{    
public:
    DSSMovingNNGeneric();
    
    ~DSSMovingNNGeneric();    
        
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
    int SetInputParam (Int32 nParamIndex, Int32 nSize,  EnumDSSDataType dataType, CComVariant *pData,  DSSDataFlag *pFlag);
    
    int GetOutput (Int32 nSize,         // Input buffer size
                   Int32 *nDataSize,    // return data size
                   CComVariant *pData,      // Double array stores the result of function
                   DSSDataFlag *pFlag	// Output flags
                   );
    
    int GetOutputType (Int32 nSize,  EnumDSSDataType *pInputType,  EnumDSSDataType *pOutputDataType, EnumDSSParameterType *pOutParamType);
    
    int GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType);
    
    int IsNullProcessable (bool *pCanProcessNULL);
    
    int Clear ();    
    
    // jxie, 2010-9-29
    // support numeric interface
    int GetOutput(Int32 nSize, Int32 *nDataSize,  double * pData,  DSSDataFlag * pFlag);
    
    int GetOutputType(EnumDSSParameterType * pType);
    
    int SetInputParam(Int32 nParamIndex, Int32 nSize, double * pData,  DSSDataFlag * pFlag);
        
private:        
    Int32 mDataSize;    // Size of the input metric
    Int32 mWinSize;
    DSSDataFlag *mpDataFlag;    
    CComVariant mInputData;  //Input variant safe array    
    CComVariant mResultData;  //Result variant	
    DSSDataFlag *mpResultFlag;
    
    // jxie, 2010-9-29
    bool mfIsNumeric;
    double* mpNumericResultData;
    double* mpNumericInputData;    
        
private:
	int CalculateMoveMaxMin(Int32 idx);
	int CalculateMoveCount(Int32 idx);
    
	// jxie, 2010-9-29
	// functions for numeric interface
	int hNumericCalculate();
	int hNumericCalculateMoveMaxMin(Int32 idx);
	int hNumericCalculateMoveCount(Int32 idx);
    
	// jxie, 2010-12-16
	// reuse the calculation result of the previous window to optimize the calculation
	int hNumericOptimizedCount(Int32 idx);
	int hOptimizedCount(Int32 idx);
	Int32 mOldCount;
};

#endif