//
//  DSSStatisticsSVVTOSSimple.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 3/7/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#include "DSSStatisticsSVVTOSSimple.h"

#include "DSSStatistics_include.h"
#include "statistic.h"

DSSStatisticsSVVTOSSimple::DSSStatisticsSVVTOSSimple()
: mFunctionIndex(0), mVSize1(0), mVSize2(0), mV1(NULL), mV2(NULL), mS(0.0), 
mResult(0), mFlag(DssDataInvalid)
{
}


DSSStatisticsSVVTOSSimple::~DSSStatisticsSVVTOSSimple()
{
	Clear();
}

int DSSStatisticsSVVTOSSimple::setProperty (Int32 Index, CComVariant *pValue)
{
	// For the functions we are currently implementing, we don't
	// have associated property. Extra logic can be added in if
	// future functions have it.
	return E_FAIL;
}

int DSSStatisticsSVVTOSSimple::Init (Int32 iFunctionIndex)
{
	mFunctionIndex = iFunctionIndex;
	return S_OK;
}

int DSSStatisticsSVVTOSSimple::Calculate ()
{
	if(!mV1 || !mV2) return E_FAIL;
    
	if (mVSize1 != mVSize2 || mVSize1 < 1) return E_INVALIDARG;
    
	enum MathError error;
	
	switch (mFunctionIndex) 
	{
		case DssStatFuncForecast_V:
			mResult = Forecast_S(ClearError(&error),
                                 mS,
                                 mV2,  // px
                                 mV1,  // py
                                 mVSize1);
			break;
		default : return E_FAIL;
	}
    
	if(error == MathNoError)
		mFlag = DssDataOk;
	else
		mFlag = DssDataInvalid;
	
	return S_OK;
}

int DSSStatisticsSVVTOSSimple::GetOutput (Int32 nSize, Int32 *nDataSize, double *pData, DSSDataFlag *pFlag)
{
	if(nSize < 1) return E_INVALIDARG;
	if(!pData || !pFlag || !nDataSize) return E_POINTER;
    
	*nDataSize = 1;
	*pData = mResult;
	*pFlag = mFlag;
    
	return S_OK;
}

int DSSStatisticsSVVTOSSimple::GetOutputType (EnumDSSParameterType *pType)
{
	if (pType == NULL) return E_POINTER;
	*pType = DssParameterScalar;
	return S_OK;
}

int DSSStatisticsSVVTOSSimple::SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSDataFlag *pFlag)
{
	if (!pData || !pFlag) return E_POINTER;
    
	switch (nParamIndex)
	{
        case 1:
            if (nSize != 1) return E_FAIL;
            mS = *pData;
            break;
        case 2:
		{
			if (nSize < 1) return E_FAIL;
			if (mV1) 
				delete[] mV1;
            
			mVSize1 = nSize;
			// For now, from heap
			mV1 = new double[nSize];
            
			for (Int32 i = 0; i < nSize; i++)
				mV1[i] = pData[i];
		}
            break;
        case 3:
		{
			if (nSize < 1) return E_FAIL;
			if (mV2)
				delete[] mV2;
            
			mVSize2= nSize;
			// For now, from heap
			mV2= new double[nSize];
            
			for (Int32 i = 0; i < nSize; i++)
				mV2[i] = pData[i];
		}
            break;
        default: return E_INVALIDARG;
	}
    
	return S_OK;
}

int DSSStatisticsSVVTOSSimple::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
	if (pType == NULL) return E_POINTER;
	switch (nParamIndex)
	{
        case 1:
            *pType = DssParameterScalar;
            break;
        case 2:
            *pType = DssParameterVector;
            break;
        case 3:
            *pType = DssParameterVector;
            break;
        default: return E_INVALIDARG;
	}
    
	return S_OK;
}

int DSSStatisticsSVVTOSSimple::Clear ()
{
	if (mV1 != NULL)
	{
		mVSize1 = 0;
        
		delete [] mV1;
		mV1 = NULL;
	}
    
	if (mV2 != NULL)
	{
		mVSize2 = 0;
        
		delete [] mV2;
		mV2 = NULL;
	}
    
    
	return S_OK;
}

int DSSStatisticsSVVTOSSimple::IsNullProcessable (bool *pCanProcessNULL)
{
	if (pCanProcessNULL == NULL) return E_FAIL;
    
	*pCanProcessNULL = VARIANT_FALSE;
	return S_OK;
}