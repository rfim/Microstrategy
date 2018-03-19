//
//  DSSStdMathSTOS.h
//  ReportServiceCore
//
//  Created by He Tengfei on 2/22/12.
//  Copyright 2012 MicroStrategy, Inc.. All rights reserved.
//


#ifndef DSSStdMathSTOS_h
#define DSSStdMathSTOS_h 1

#include "DSSSimpleFunction.h"

class DSSStdMathSTOS: public DSSSimpleFunction
{
public:
    DSSStdMathSTOS();
    
    ~DSSStdMathSTOS();
    
    int Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag);

    inline int Init(int iIndex) { mFunctionIndex = iIndex; return S_OK; }
    
    inline int getFunctionIndex() {return mFunctionIndex;}
    
private :
    int mFunctionIndex;
    
};

#endif