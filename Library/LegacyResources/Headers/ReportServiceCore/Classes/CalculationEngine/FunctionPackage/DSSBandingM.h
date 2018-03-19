//
//  DSSBandingM.h
//  ReportServiceCore
//
//  Created by Yang Zhuo on 7/27/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//

#ifndef CDSSBandingM_h
#define CDSSBandingM_h 1

#import "CalculationEngine/DSSGenericFunction.h"
#import "DSSStrongPtr.h"

class DSSBandingM : public DSSGenericFunction
{
public:
    DSSBandingM();
    ~DSSBandingM();
    
    virtual int setProperty (Int32 Index, CComVariant *pValue);
    int Calculate ();
    int SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSDataFlag *pFlag);
    int GetOutput (Int32 nSize, Int32 *nDataSize, double *pData, DSSDataFlag *pFlag);
    int GetOutputType (EnumDSSParameterType *pType);
    int GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType);
    int IsNullProcessable (bool *pCanProcessNULL);
    int Clear();
    // int Init (EnumSimpleOperatorType iFunctionIndex);
    int DirectCalculate (Int32 nParam, Int32* ipSize, 
                         void** ipData, double* ipDivider, DSSDataFlag **ipFlags, bool* ipAllDataValid,
                         EnumDSSDataType* iType, vector<Int32> * ipvDataByGroup,
                         Int32 nResSize, double* opData, DSSDataFlag* opFlag, bool* opAllDataValid, 
                         EnumNullCheckingOption iNullCheckingOption);
    
    bool SupportDirectCalculate();
private:
    int CalculateRunningSum();
    int CalculateRunningAvg();
    int CaculateRunnningStdev();

    bool mHasResidue;
    Int32 mDataSize;

    /*
    MBase::StrongArrayPtr<double> mpDataPtr;
    MBase::StrongArrayPtr<DSSDataFlag> mpDataFlagPtr;      
    
    
    MBase::StrongArrayPtr<double> mpResultPtr;
    MBase::StrongArrayPtr<DSSDataFlag> mpResultFlagPtr;      */
    double *mpDataPtr;
    DSSDataFlag *mpDataFlagPtr;
    
    double *mpResultPtr;
    DSSDataFlag *mpResultFlagPtr;
};

#endif
