//
//  DSSDateTimeFunc.h
//  ReportServiceCore
//
//  Created by Jia Zhang on 4/22/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef __ReportServiceCore__DSSDateTimeFunc__
#define __ReportServiceCore__DSSDateTimeFunc__

#include <iostream>

#include "DSSNNSimpleFunction.h"

class DSSDateTimeFunc :	public DSSNNSimpleFunction
{
    
public:
    DSSDateTimeFunc();
    
    ~DSSDateTimeFunc();
    
    int Calculate (Int32 nSize, CComVariant *pVec, CComVariant *opResult, DSSDataFlag *pFlag);
    
    int Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag);
    
    int  SetProperty (Int32 Index, CComVariant *pValue);
    
    int GetOutputType (Int32 size, EnumDSSDataType *inputType, EnumDSSDataType *outputType);
    
    // kshi 07-27-2011
    int SetStringCompareOption(VARIANT_BOOL iOpt);
    
    virtual bool IsNNFunctionOnly() {return true;}
	
private:
    //Int32 mktime(IDSSTime *ipTime, struct tm& orTime) const;
    
    Int32 mktime(MDataType::DSSDateTime *ipTime, struct tm& orTime) const;
    
    Int32 YMD2DAYS(Int32 inYear, Int32 inMonth, Int32 inDay);
    
    void DAYS2YMD(Int32 inDays, Int32& ornYear, Int32& ornMonth, Int32& ornDay);
    
private:
    bool mfTrimString;
};

#endif /* defined(__ReportServiceCore__DSSDateTimeFunc__) */
