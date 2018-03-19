//
//  DSSStatisticsSVTOSSimple.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 3/7/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#include "DSSStatisticsSVTOSSimple.h"

#include "DSSStatistics_include.h"
#include "statistic.h"

DSSStatisticsSVTOSSimple::DSSStatisticsSVTOSSimple()
: mFunctionIndex(0), mVSize(0), mV(NULL), mS(0.0), 
mResult(0.0), mFlag(DssDataInvalid)
{
}

DSSStatisticsSVTOSSimple::~DSSStatisticsSVTOSSimple()
{
	Clear();
}

int DSSStatisticsSVTOSSimple::setProperty (Int32 Index, CComVariant *pValue)
{
	// For the functions we are currently implementing, we don't
	// have associated property. Extra logic can be added in if
	// future functions have it.
	return E_FAIL;
}

int DSSStatisticsSVTOSSimple::Init (Int32 iFunctionIndex)
{
	mFunctionIndex = iFunctionIndex;
	return S_OK;
}

int DSSStatisticsSVTOSSimple::Calculate ()
{
	if(!mV) 
		return E_FAIL;
    
	enum MathError error;
	
	switch (mFunctionIndex) 
	{
		case DssStatFuncLinearTrend_V:
			mResult = LinearTrend_S(ClearError(&error),
									mS,
									mV,
									mVSize);
			break;
            
		case DssStatFuncGrowth_V:
			mResult = Growth_S(ClearError(&error),
                               mS,
                               mV,
                               mVSize);
			break;
		default : return E_FAIL;
	}
    
	if(error == MathNoError)
		mFlag = DssDataOk;
	else
		mFlag = DssDataInvalid;
	
	return S_OK;
}

int DSSStatisticsSVTOSSimple::GetOutput (Int32 nSize, Int32 *nDataSize, double *pData, DSSDataFlag *pFlag)
{
	if(nSize < 1) return E_INVALIDARG;
	if(!pData || !pFlag || !nDataSize) return E_POINTER;
    
	*nDataSize = 1;
	*pData = mResult;
	*pFlag = mFlag;
	
	return S_OK;
}

int DSSStatisticsSVTOSSimple::GetOutputType (EnumDSSParameterType *pType)
{
	if (pType == NULL) return E_POINTER;
	*pType = DssParameterScalar;
	return S_OK;
}

int DSSStatisticsSVTOSSimple::SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSDataFlag *pFlag)
{
	if (!pData || !pFlag) return E_POINTER;
    
	switch (nParamIndex)
	{
        case 2:
            if (nSize != 1) return E_FAIL;
            mS = *pData;
            break;
        case 1:
		{
			if (nSize < 1) return E_FAIL;
			if (mV) 
				delete[] mV;
            
			mVSize = nSize;
			// For now, from heap
			mV = new double[nSize];
            
			for (Int32 i = 0; i < nSize; i++)
				mV[i] = pData[i];
		}
            break;
            
        default: return E_INVALIDARG;
	}
    
	return S_OK;
}

int DSSStatisticsSVTOSSimple::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
	if (pType == NULL) return E_POINTER;
	switch (nParamIndex)
	{
        case 1:
            *pType = DssParameterVector; 
            break;
        case 2:
            *pType = DssParameterScalar; 
            break;
        default: return E_INVALIDARG;
	}
    
	return S_OK;
}

int DSSStatisticsSVTOSSimple::Clear ()
{
	if (mV != NULL)
	{
		mVSize = 0;
        
		delete [] mV;
		mV = NULL;
	}
    
	return S_OK;
}

int DSSStatisticsSVTOSSimple::IsNullProcessable (bool *pCanProcessNULL)
{
	if (pCanProcessNULL == NULL) return E_FAIL;
    
	*pCanProcessNULL = VARIANT_FALSE;
	return S_OK;
}

