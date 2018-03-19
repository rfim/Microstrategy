/*
 *  DSSAggregationFunction.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 2/4/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef AggregationFunction_h
#define AggregationFunction_h 1

#import "CalculationEngine/DSSFunctionObject.h"

class DSSAggregationFunction : public DSSFunctionObject
{
public:
	DSSAggregationFunction();
	
	virtual ~DSSAggregationFunction();

	inline virtual EnumDSSFunctionType getFunctionType(){return DssFunctionTypeAggregation;};
	
	virtual int Calculate(int nSize, double *pVec, double *opResult, DSSDataFlag *pFlag);
	
	virtual bool SupportDirectCalculate();
	
	virtual int DirectCalculate(int iStart,
								int * ipGroupLinkList,
								int * ipBlockAddress,
								void* ipData,  double iDivider,
								EnumDSSDataType iDataType,
								DSSDataFlag* ipDataFlags,
								bool iAllDataValid,
								EnumNullCheckingOption iNullCheckingOption,
								double* opResult,
								DSSDataFlag* opDataFlag);
};

#endif
