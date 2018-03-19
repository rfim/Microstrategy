//
//  DSSIndexFunction.h
//  ReportServiceCore
//
//  Created by Jia Zhang on 7/27/11.
//  Copyright 2011 MicroStrategy, Inc. All rights reserved.
//

#ifndef IndexFunction_h
#define IndexFunction_h 1

#import "DSSGenericFunction.h"

class DSSIndexFunction : public DSSGenericFunction
{
public:
    DSSIndexFunction();
    
    ~DSSIndexFunction();
    
    int GetOutputType (EnumDSSParameterType *pType);
    
    int SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSDataFlag *pFlag);
    
    int GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType);
    
    int Clear ();
        
protected:
    double *mV;
    Int32 mVSize;
    double mS;
    DSSDataFlag  mSFlag;
    DSSDataFlag *mVFlag;    
};

#endif