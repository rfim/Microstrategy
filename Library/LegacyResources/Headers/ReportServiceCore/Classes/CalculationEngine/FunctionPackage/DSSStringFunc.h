//
//  DSSStringFunc.h
//  ReportServiceCore
//
//  Created by He Tengfei on 7/3/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSStringFunc_h
#define ReportServiceCore_DSSStringFunc_h


#include "DSSNNSimpleFunction.h"

class DSSStringFunc :	public DSSNNSimpleFunction
{
    
public:
    DSSStringFunc();
    
    virtual ~DSSStringFunc();
    
    //	Derived class must implement this method.
    int Calculate (int nParam, CComVariant *ipParams, CComVariant *opResult, DSSDataFlag *pFlag);
    
    int GetOutputType(int size, EnumDSSDataType *inputType, EnumDSSDataType *outputType);
    
    // kshi 07-27-2011
    int SetStringCompareOption(VARIANT_BOOL iOpt);
	
    int Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag);
    
    virtual bool IsNNFunctionOnly() {return true;}
private:
    
    // deliu, 2012-05-25, TQMS 586121
    int hGetValueInt(CComVariant *pValue, EnumDSSDataType iType, int *opVar, bool fTowardZero);
    
    // 11/19/2012 tehe: inorder to both support char and wchar
    template <class T>
    int hGetPatternNext(T *ipPattern, int iLength, int *opNextVal);
    
    template <class T>
    int hKMPIndex(T *ipString, T *ipPattern, int iStrLength, int iPatternLength, int *pNextVal);
        
    bool mfTrimString;   
};

#endif
