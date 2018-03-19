//
//  DSSComparisonOperatorFunc.h
//  ReportServiceCore
//
//  Created by Zhuo Yang on 11/2/12.
//  Copyright (c) 2012 Microstrategy. All rights reserved.
//

//it likes the server side CDSSNNSimpleOperator.h/cpp
#ifndef DSSCOMPARISONFUNC_H
#define DSSCOMPARISONFUNC_H

#include "DSSNNSimpleFunction.h"

class DSSComparisonOperatorFunc : public DSSNNSimpleFunction
{
public:
    
    DSSComparisonOperatorFunc();
    ~DSSComparisonOperatorFunc();
    
    int Calculate(Int32 nSize, CComVariant *pVec, CComVariant *opResult, DSSDataFlag *pFlag);
    virtual int setProperty(Int32 index, CComVariant *pValue);
    //int Init(EnumDSSFunction iFunctionIndex);
    int GetOutputType(Int32 size, EnumDSSDataType *inputType, EnumDSSDataType *outputType);
    int Calculate(Int32 nSize, double *pVec, double *pResult, DSSDataFlag *pFlag);
    int CalculateRepeat(Int32 nParam, Int32 nRepeat, double **pParam, double *pResult, DSSDataFlag *pFlag);
    // int GetOperatorType(EnumDSSFunction *opType);
    virtual bool SupportDirectCalculate();  
    virtual int DirectCalculate(Int32 nParam, Int32* ipSize, 
                                Int32** ipOffsets, void** ipData, double* ipDivider, 
                                DSSDataFlag** ipFlags, VARIANT_BOOL* ipAllDataValid,
                                EnumDSSDataType* iType, DSSXTabBitVector* ipMask,
                                Int32 nResSize, double* opData, DSSDataFlag* opFlag, VARIANT_BOOL* opAllDataValid,
                                EnumNullCheckingOption iNullCheckingOption);
    
    // 2012/12/18 tehe #697869
    int SetStringCompareOption(VARIANT_BOOL iOpt);
private:
    
    // EnumDSSFunction mFunctionIndex;
    int hTryConvert(Int32 iSize, Int32* ipOffset, void *ipData, EnumDSSDataType iType, double iDivider, double **opData);
    int hDirectCalculateBetween(Int32* ipSize, double** ipData, DSSDataFlag** ipFlags, VARIANT_BOOL* ipAllDataValid, 
                                //CDSSXTabBitVector* ipMask,
                                Int32 nResSize, double* opData, DSSDataFlag* opFlag, 
                                VARIANT_BOOL* opAllDataValid, EnumNullCheckingOption iNullCheckingOption);

    int hVariantCompare(CComVariant *pParam, DSSDataFlag *pFlag);
    int hVariantBetween(CComVariant *pParam, DSSDataFlag *pFlag);
    bool hVariantStrOperator(CComVariant *pParam, DSSDataFlag *pFlag);
    
    
    // 12/18/2012 tehe
    bool mfTrimString;
};

#endif
