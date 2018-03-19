//
//  DSSNNGenericFunction.h
//  ReportServiceCore
//
//  Created by Zhuo Yang on 11/5/12.
//  Copyright (c) 2012 Microstrategy. All rights reserved.
//

#ifndef ReportServiceCore_DSSNNGenericFunction_h
#define ReportServiceCore_DSSNNGenericFunction_h

#import "CalculationEngine/DSSGenericFunction.h"
#import "VariantCommonInclude.h"

class DSSNNGenericFunction : public DSSGenericFunction 
{
public:
    
    DSSNNGenericFunction();    
    
    virtual ~DSSNNGenericFunction();
    // 2/24/2012 tehe change the type of third param from Variant to ccomvariant
    virtual int GetOutput(Int32, Int32 *, CComVariant *, DSSDataFlag *) = 0;    
    
    virtual int GetOutputType(Int32, EnumDSSDataType *, EnumDSSDataType *, EnumDSSParameterType *) = 0;
    
    virtual int SetInputParam(Int32, Int32, EnumDSSDataType,CComVariant *, DSSDataFlag *) = 0;
    
    virtual bool IsNNFunctionOnly() {return false;}
};

#endif
