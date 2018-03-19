//
//  DSSOperatorGeneric.h
//  ReportServiceCore
//
//  Created by Zhuo Yang on 11/2/12.
//  Copyright (c) 2012 Microstrategy. All rights reserved.
//

#ifndef ReportServiceCore_DSSOperatorGeneric_h
#define ReportServiceCore_DSSOperatorGeneric_h

#import "DSSGenericFunction.h"
#import "Base/Base/Allocator.h"
//#import "VariantCommonInclude.h"
//#import "DSSFunctionObject.h"
//#import "DSSrwdconstants.h"

class DSSOperatorGeneric : public DSSGenericFunction
{
public:
    DSSOperatorGeneric();
    
    ~DSSOperatorGeneric();
    
    virtual int setProperty (Int32 Index, CComVariant *pValue);
    
    int Calculate ();
    
    int GetOutput (Int32 nSize,         // Input buffer size
                   Int32 *nDataSize, 	// return data size
                   double *pData,       // Double array stores the result of function
                   DSSDataFlag *pFlag	// Output flags
                   );
    
    int GetOutputType (EnumDSSParameterType *pType);
    
    int Clear ();
    
    int SetInputParam (Int32 nParamIndex, 	// The index of input parameter
                       Int32 nSize,         // Size of input double array
                       double *pData,
                       DSSDataFlag *pFlag);
    
    int GetInputParamType (Int32 nParamIndex, EnumDSSParameterType * pType);
    
    int IsNullProcessable(bool *pCanProcessNULL); 
    
    // jxie, 2010-9-25
    // support direct calculate
    virtual int DirectCalculate (Int32 nParam, Int32* ipSize, Int32** ipOffsets,
                         void** ipData, double* ipDivider, DSSData_Flags** ipFlags, VARIANT_BOOL* ipAllDataValid,
                         DSSDataType_Type* iType, vector<Int32> * ipvDataByGroup,
                         Int32 nResSize, double* opData, DSSData_Flags* opFlag, VARIANT_BOOL* opAllDataValid, 
                         EnumNullCheckingOption iNullCheckingOption);
    
    virtual bool SupportDirectCalculate();
    
    EnumDSSFunctionType getFunctionType(){return DssFunctionTypeGeneric;}
    
private:       
    double *mpResult;
    DSSDataFlag *mpResultFlag;
    
    bool **mppInput;
    DSSDataFlag **mppInputFlag;
    
    Int32 mNumParam;
    
    Int32 mParamSize;
};

#endif
