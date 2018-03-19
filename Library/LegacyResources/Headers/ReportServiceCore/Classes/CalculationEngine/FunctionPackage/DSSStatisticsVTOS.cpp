//
//  DSSStatisticsVTOS.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 3/7/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#include "DSSStatisticsVTOS.h"
#include "statistic.h"

double (* gVTOSFunctions[6])(enum MathError *, Int32, double[]) =
{Mean, Variance, StDev, AvgDev, Kurtosis, Skew };

DSSStatisticsVTOS::DSSStatisticsVTOS()
: mFunctionIndex(0)
{
}


DSSStatisticsVTOS::~DSSStatisticsVTOS()
{
}

int DSSStatisticsVTOS::Calculate(Int32 nSize, double *pVec, double *opResult, DSSDataFlag *pFlag)
{
	if(nSize < 1) return E_INVALIDARG;
	if(!pVec || !opResult || !pFlag) return E_POINTER;
    
	enum MathError error;
	
    
	*opResult = gVTOSFunctions[mFunctionIndex](	ClearError(&error), 
                                               nSize, pVec );
    
	if(error == MathNoError)
		*pFlag = DssDataOk;
	else
		*pFlag = DssDataInvalid;
    
	return S_OK;
}

int DSSStatisticsVTOS::CalculateRepeat(Int32 nSize, Int32 nRepeat, Int32 *pStartPoint, double *pVec, double *opResult, DSSDataFlag *pFlag)
{
	Int32 i = 0;
	Int32 lPosition = 0;
	Int32 lSize = 0;
	HRESULT hr;
    
	for(i = 0; i < nRepeat; i ++)
	{
		lPosition = pStartPoint[i];
        
		if(i < nRepeat - 1)
			lSize = pStartPoint[i + 1] - lPosition;
		else
			lSize = nSize - lPosition;
        
		if((lPosition + lSize > nSize) || lSize < 0)
			return E_FAIL;
        
		// yma: special case when lSize is 0
		if(lSize > 0)
		{
			hr = Calculate(lSize, pVec+lPosition, opResult+i, pFlag+i);
			if FAILED(hr) return hr;
		}
		else
		{
			pFlag[i] = DssDataInvalid;
		}
	}
    
	return S_OK;
}

int DSSStatisticsVTOS::setProperty(Int32 Index, CComVariant *pValue)
{
	return E_NOTIMPL;
}

int DSSStatisticsVTOS::Init(Int32 iFunctionIndex)
{
	mFunctionIndex = iFunctionIndex;
	return S_OK;
}