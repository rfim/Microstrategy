//
//  DSSStatisticsGeneric.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 3/7/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#include "DSSStatisticsGeneric.h"

double (* gGenericFunctions[8])(enum MathError *, Int32, double[], Int32, double[]) =
{ Covariance, Correlation, Correlation, RSquare, ChiSqTest, Slope, Intercept, SteYX };
double (* gTestGenericFunctions[4])(enum MathError *, Int32, double[], Int32, double[], enum Hypothesis, double) =
{ PairedTTest, HomoscedasticTTest, HeteroscedasticTTest, FTest };
#define FTESTIDX	(11)
#define NOPROP		(8)

DSSStatisticsGeneric::DSSStatisticsGeneric()
: mFunctionIndex(0), mHypothesis(HypUnkown), moffset(0.0), mlenght1(0), mvector1(NULL), mlenght2(0), mvector2(NULL)
{
}


DSSStatisticsGeneric::~DSSStatisticsGeneric()
{
	Clear();
}


int DSSStatisticsGeneric::Calculate()
{
    return S_OK;
}

int DSSStatisticsGeneric::GetOutput(Int32 nSize, Int32 *nDataSize, double *pData, DSSDataFlag *pFlag)
{
    if (mvector1 == NULL || mvector2 == NULL) return E_FAIL;
	if (nSize < 1) return E_FAIL;
	if (nDataSize == NULL || pData == NULL || pFlag == NULL ) return E_POINTER;
	if (mlenght1 < 1 || mlenght2 < 1) return E_INVALIDARG;
    
	enum MathError error;
	
	*nDataSize = 1;
	if (mFunctionIndex < NOPROP)
        *pData = gGenericFunctions[mFunctionIndex](	ClearError(&error), 
                                                   mlenght1, mvector1, mlenght2, mvector2 );
	else *pData = gTestGenericFunctions[mFunctionIndex-NOPROP]( ClearError(&error), 
                                                               mlenght1, mvector1, 
                                                               mlenght2, mvector2,
                                                               mHypothesis, moffset );
    
    
	if(error == MathNoError)
		*pFlag = DssDataOk;
	else
		*pFlag = DssDataInvalid;
    
	return S_OK;
}

int DSSStatisticsGeneric::GetOutputType(EnumDSSParameterType *pType)
{
    if (pType == NULL) return E_POINTER;
	*pType = DssParameterScalar;
	return S_OK;
}

int DSSStatisticsGeneric::SetInputParam(Int32 nParamIndex, Int32 nSize, double *pData, DSSDataFlag *pFlag)
{
    // Veracode: CWE ID 191: Integer Underflow (Wrap or Wraparound)
	if (nSize < 0) nSize = 0;
    // Veracode
    if (pData == NULL) return E_POINTER;
	switch (nParamIndex)
	{
        case 1:
		{
			if (mvector1 != NULL) return E_FAIL;
            
			mlenght1 = nSize;
			mvector1 = new double[nSize];
			for (Int32 i=0; i<nSize; i++) mvector1[i] = pData[i];
		}
            break;
        case 2:
		{
			if (mvector2 != NULL) return E_FAIL;
            
			mlenght2 = nSize;
			mvector2 = new double[nSize];
			for (Int32 i=0; i<nSize; i++) mvector2[i] = pData[i];
		}
            break;
        default: return E_FAIL;
	}
	return S_OK;
}

int DSSStatisticsGeneric::GetInputParamType(Int32 nParamIndex, EnumDSSParameterType *pType)
{
    if (pType == NULL) return E_POINTER;
	if (nParamIndex != 1 && nParamIndex != 2) return E_FAIL;
    
	*pType = DssParameterVector;
	return S_OK;
}

int DSSStatisticsGeneric::Clear()
{
    if (mvector1 != NULL)
	{
		delete mvector1;
		mvector1 = NULL;
		mlenght1 = 0;
	}
	if (mvector2 != NULL)
	{
		delete mvector2;
		mvector2 = NULL;
		mlenght2 = 0;
	}
    
	return S_OK;
}

int DSSStatisticsGeneric::Init (Int32 iFunctionIndex)
{
    mFunctionIndex = iFunctionIndex;
	if ( iFunctionIndex == FTESTIDX )
		moffset = 1.0;
	
	return S_OK;
}

int DSSStatisticsGeneric::IsNullProcessable(bool *pCanProcessNULL)
{
    if (pCanProcessNULL == NULL) return E_FAIL;
    
	*pCanProcessNULL = VARIANT_FALSE;
	return S_OK;
}

int DSSStatisticsGeneric::setProperty(int Index, CComVariant *pValue)
{
    HRESULT hr;
    
	if (pValue == NULL) return E_POINTER;
    
	switch (Index)
	{
        case 1:
            hr = VariantChangeType(pValue, pValue, 0, VT_I4);
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
            else return E_FAIL;
            break;
            
        case 2:
            hr = VariantChangeType(pValue, pValue, 0, VT_R8);
            if ( FAILED(hr) ) return hr;
            
            if(pValue->vt == VT_R8)
                moffset = pValue->dblVal;
            else return E_FAIL;
            break;
            
        default:
            return E_FAIL;
	}
    
	return S_OK;
}
