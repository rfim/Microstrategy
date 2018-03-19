//
//  DSSBanding.h
//  ReportServiceCore
//
//  Created by Yang Zhuo on 7/26/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//

#ifndef CDSSBanding_h
#define CDSSBanding_h 1
#import "CalculationEngine/DSSSimpleFunction.h"

class DSSBanding : public DSSSimpleFunction
{
public:
    DSSBanding();
    ~DSSBanding();
    
    int Calculate(Int32 nParam, double *pParam, double *pResult, DSSDataFlag *pFlag);
    
    virtual int setProperty(Int32 Index, CComVariant *pValue);
    
    // int Init(EnumSimpleOperatorType iOperatorType);
    // int get_OperatorType(EnumSimpleOperatorType * opType);
    virtual bool SupportDirectCalculate();
    virtual int DirectCalculate(Int32 nParam, Int32* ipSize, Int32** ipOffsets, 
                        void** ipData, double* ipDivider, DSSDataFlag** ipFlags, VARIANT_BOOL* ipAllDataValid,
                        EnumDSSDataType* iType, DSSXTabBitVector* ipMask,
                        Int32 nResSize, double* opData, DSSDataFlag* opFlag, VARIANT_BOOL* opAllDataValid, 
                        EnumNullCheckingOption iNullCheckingOption);    
private:
    bool mHasResidue;
};

#endif


