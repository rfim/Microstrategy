//
//  CDSSMovingGeneric.h
//  ReportServiceCore
//
//  Created by Guanlin Shen on 09/14/99.
//  Copyright 1999 MicroStrategy, Inc. All rights reserved.
//

#ifndef MovingGeneric_h
#define MovingGeneric_h 1

#import "DSSGenericFunction.h"
#import "VariantCommonInclude.h"

class DSSMovingGeneric : public DSSGenericFunction
{    
public:
    DSSMovingGeneric();
    
    ~DSSMovingGeneric();
    
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
    int SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSDataFlag*pFlag);
    
    int GetOutput (Int32 nSize,         // Input buffer size
                   Int32 *nDataSize, 	// return data size
                   double *pData,       // Double array stores the result of function
                   DSSDataFlag*pFlag	// Output flags
                   );
    
    int GetOutputType (EnumDSSParameterType *pType);
    
    int GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType);
    
    int IsNullProcessable (bool *pCanProcessNULL);
    
    int Clear ();

private:
    int SumBackward (Int32 iVal);
    
    int SumForward (Int32 iVal);
    
    int AvgBackward (Int32 iVal);
    
    int AvgForward (Int32 iVal);
    
    int MaxBackward (Int32 iVal);
    
    int MaxForward (Int32 iVal);
    
    int MinBackward (Int32 iVal);
    
    int MinForward (Int32 iVal);
    
    int CountBackward (Int32 iVal);
    
    int StdevBackward (Int32 iVal);
    
    int StdevBackwardP(Int32 iVal);
    
    int DiffBackward (Int32 iVal);
    
    int CalculateMovingSum (void );
    
    int CalculateMovingAvg (void );
    
    int CalculateMovingMax (void );
    
    int CalculateMovingMin (void );
    
    int CalculateMovingCount (void );
    
    int CalculateMovingDifference (void );
    
    int CalculateMovingStdev (void );
    
    int CalculateMovingStdevP (void );
    
    Int32 mDataSize;    //	Size of the input metric 
    double *mpData;     //	hold the pointer to the input metric    
    DSSDataFlag*mpDataFlag;
    
    //	The result of the calculation. The size should be same as mDataSize.
    double *mpResult;
    
    DSSDataFlag*mpResultFlag;
    Int32 mWinSize;    
    
    // jxie, 2010-12-16
    // reuse the calculation result of the previous window
    int SumBasedOnPreviousWindow(Int32 iVal);
    int AvgBasedOnPreviousWindow(Int32 iVal);
    double mOldNum;
    Int32 mOldCount;    
};

#endif