//
//  DSSStatisticsVSTOS.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 3/7/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#include "DSSStatisticsVSTOS.h"

double (* gVSTOSFunctions[2])(enum MathError *, Int32, double[], double, enum Hypothesis) =
{ MeanTTest, VarTest };

DSSStatisticsVSTOS::DSSStatisticsVSTOS()
: mFunctionIndex(0), mHypothesis(HypEqual), mVSize(0), mV(NULL), mS(0.0)
{
}


DSSStatisticsVSTOS::~DSSStatisticsVSTOS()
{
	Clear();
}

int DSSStatisticsVSTOS::setProperty (Int32 Index, CComVariant *pValue)
{
	if (Index != 1)
		return E_FAIL;
    
	HRESULT hr = VariantChangeType(pValue, pValue, 0, VT_I4);
	if ( FAILED(hr) ) return hr;
    
	if(pValue->vt == VT_I4)
		switch (pValue->lVal)
    {
        case -1:	mHypothesis = HypLess;
            break;
        case 0:		mHypothesis = HypEqual;
            break;
        case 1:		mHypothesis = HypGreater;
            break;
        case 2:		mHypothesis = HypOneSide;
            break;
        default : return E_FAIL;
    }
	else
        return E_FAIL;
    
	return S_OK;
}

int DSSStatisticsVSTOS::Init (Int32 iFunctionIndex)
{
	mFunctionIndex = iFunctionIndex;
	return S_OK;
}

int DSSStatisticsVSTOS::Calculate ()
{
	return S_OK;
}

int DSSStatisticsVSTOS::GetOutput (Int32 nSize, Int32 *nDataSize, double *pData, DSSDataFlag *pFlag)
{
	if(nSize < 1) return E_INVALIDARG;
	if(!pData || !pFlag || !nDataSize) return E_POINTER;
    
	if(!mV) return E_FAIL;
    
	enum MathError error;
	
	*nDataSize = 1;
	*pData = gVSTOSFunctions[mFunctionIndex](	ClearError(&error),
                                             mVSize,
                                             mV,
                                             mS,
                                             mHypothesis );
    
	if(error == MathNoError)
		*pFlag = DssDataOk;
	else
		*pFlag = DssDataInvalid;
	
	return S_OK;
}

int DSSStatisticsVSTOS::GetOutputType (EnumDSSParameterType *pType)
{
	if (pType == NULL) return E_POINTER;
	*pType = DssParameterScalar;
	return S_OK;
}

int DSSStatisticsVSTOS::SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSDataFlag *pFlag)
{
	if (!pData || !pFlag) return E_POINTER;
    
	switch (nParamIndex)
	{
        case 1:
		{
			if (nSize < 1) return E_FAIL;
			if (mV) 
				delete[] mV;
            
			mVSize = nSize;
			mV = new double[nSize];
            
			for (Int32 i=0; i<nSize; i++)
				mV[i] = pData[i];
		}
            break;
        case 2:
            if (nSize != 1) return E_FAIL;
            mS = *pData;
            break;
        default: return E_INVALIDARG;
	}
    
	return S_OK;
}

int DSSStatisticsVSTOS::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
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

int DSSStatisticsVSTOS::Clear ()
{
	if (mV != NULL)
	{
		mVSize = 0;
        
		delete [] mV;
		mV = NULL;
	}
    
	return S_OK;
}

int DSSStatisticsVSTOS::IsNullProcessable (bool *pCanProcessNULL)
{
	if (pCanProcessNULL == NULL) return E_FAIL;
    
	*pCanProcessNULL = VARIANT_FALSE;
	return S_OK;
}