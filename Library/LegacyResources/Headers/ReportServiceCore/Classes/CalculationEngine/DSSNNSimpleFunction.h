//
//  DSSNNSimpleFunction.h
//  ReportServiceCore
//
//  Created by Zhang Jia on 7/22/11.
//  Copyright 2011 MicroStrategy Beijing R&D Center. All rights reserved.
//

#ifndef NNSimpleFunction_h
#define NNSimpleFunction_h 1

#import "CalculationEngine/DSSSimpleFunction.h"
#import "VariantCommonInclude.h"

class DSSNNSimpleFunction : public DSSSimpleFunction
{
public:
	DSSNNSimpleFunction();
	
	virtual ~DSSNNSimpleFunction();
	
	inline virtual EnumDSSFunctionType getFunctionType(){return DssFunctionTypeSimple;};	
	    
    virtual int Calculate(int nParam, CComVariant *ipParams, CComVariant *opResult, DSSDataFlag *opFlag) = 0;
	
	int CalculateRepeat(int nParam, int nRepeat, CComVariant *ipParams, CComVariant *opResult, DSSDataFlag *opFlag);
    
    virtual int GetOutputType (int size, EnumDSSDataType *inputType, EnumDSSDataType *outputType) = 0;
    
    virtual bool IsNNFunctionOnly() {return false;}
};

#endif