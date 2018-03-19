/*
 *  DSSAggregationFunction.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 2/4/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#include "DSSAggregationFunction.h"

DSSAggregationFunction::DSSAggregationFunction()
{}

DSSAggregationFunction::~DSSAggregationFunction()
{}

bool DSSAggregationFunction::SupportDirectCalculate()
{
	return false;
}

int DSSAggregationFunction::DirectCalculate(int iStart,
										 int * ipGroupLinkList,
										 int * ipBlockAddress,
										 void* ipData,  double iDivider,
										 EnumDSSDataType iDataType,
										 DSSDataFlag* ipDataFlags,
										 bool iAllDataValid,
										 EnumNullCheckingOption iNullCheckingOption,
										 double* opResult,
										 DSSDataFlag* opDataFlag)
{
	return E_NOTIMPL;
}

int DSSAggregationFunction::Calculate(int nSize, double *pVec, double *opResult, DSSDataFlag *pFlag)
{
	return E_NOTIMPL;
}
