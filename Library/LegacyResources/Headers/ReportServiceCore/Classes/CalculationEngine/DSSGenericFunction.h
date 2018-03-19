//
//  DSSGenericFunction.h
//  ReportServiceCore
//
//  Created by Zhuo Yang on 11/5/12.
//  Copyright (c) 2012 Microstrategy. All rights reserved.
//

#ifndef ReportServiceCore_DSSGenericFunction_h
#define ReportServiceCore_DSSGenericFunction_h

#import "CalculationEngine/DSSFunctionObject.h"

class DSSGenericFunction : public DSSFunctionObject 
{
public:
    
    DSSGenericFunction();    
    
    virtual ~DSSGenericFunction();
    
    virtual int Calculate() = 0;    
    
    virtual int GetOutput(Int32, Int32 *, double *, DSSDataFlag *) = 0; 
    
    virtual int GetOutputType(EnumDSSParameterType *) = 0;
    
    virtual int SetInputParam(Int32, Int32, double *, DSSDataFlag *) = 0;
    
    virtual int GetInputParamType(Int32, EnumDSSParameterType *) = 0;    
    
    virtual int Clear() = 0;    
    
    virtual int IsNullProcessable(bool *) = 0;
    
    virtual bool SupportDirectCalculate();
    
    virtual int DirectCalculate(Int32 nParam, Int32* ipSize, 
                                Int32** ipOffsets, void** ipData, double* ipDivider, 
                                DSSData_Flags** ipFlags, VARIANT_BOOL* ipAllDataValid,
                                DSSDataType_Type* iType, vector<Int32> * ipvDataByGroup,
                                Int32 nResSize, double* opData, DSSData_Flags* opFlag, VARIANT_BOOL* opAllDataValid,
                                EnumNullCheckingOption iNullCheckingOption);
    
    //zhyang, 20/07/2011, implement pure virtual function in functionobject
    inline virtual EnumDSSFunctionType getFunctionType(){return DssFunctionTypeGeneric;};
    
    virtual int get_PropertyInfo(Int32 PropertyIndex, BSTR *pName, BSTR *pDescription, VARIANT *pValue);
};

#endif
