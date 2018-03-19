//
//  DSSFinancialVSSTOS.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 2/22/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#include "DSSFinancialVSSTOS.h"
#include "DSSFunctionUtility.h"
#include "financialFuncImplement.h"

DSSFinancialVSSTOS::DSSFinancialVSSTOS(): mFunctionIndex(0), mpValues(NULL), mnDataSize(0),
mfFinanicalRate(0), mbFRReady(false), mfReinvestmentRate(0), mbRRReady(0),
mFlag(DssDataInvalid)
{
}

DSSFinancialVSSTOS::~DSSFinancialVSSTOS()
{
    if (mpValues) {
        delete [] mpValues;
        mpValues = NULL;
    }
}

int DSSFinancialVSSTOS::Calculate()
{
    if(!mpValues || !mbFRReady || !mbRRReady) 
		return E_FAIL;
    
	enum MathError error = MathNoError;
	HRESULT hr = S_OK;
    
	try {
		mfResult = _MIRR(&error, mnDataSize, mpValues, mfFinanicalRate, mfReinvestmentRate);
	} catch (...) {
		hr = E_FAIL;
	}
    
	if(error == MathNoError)
		mFlag = DssDataOk;
	else
		mFlag = DssDataInvalid;
    
	return hr;
}

int DSSFinancialVSSTOS::GetOutput(Int32 nSize, 	// Input buffer size
              Int32 *nDataSize, 	// return data size
              double *pData, 	// Double array stores the result of function
              DSSDataFlag *pFlag)	// Output flags) = 0; 
{
    if(!nDataSize || !pData || !pFlag) return E_POINTER;
    
	if (nSize < 1) 
		return E_FAIL;
    
	*pData = mfResult;
	*pFlag = mFlag;
	*nDataSize = 1;
    
	return S_OK;
}

int DSSFinancialVSSTOS::GetOutputType (EnumDSSParameterType *pType)
{
    if(!pType) return E_POINTER;
    
	*pType = DssParameterScalar;
	
	return S_OK;
}

int DSSFinancialVSSTOS::SetInputParam(Int32 nParamIndex, 	// The index of input parameter
                  Int32 nSize, 	// Size of input double array
                  double *pData,
                  DSSDataFlag *pFlag)
{
    if((nParamIndex < 1) || (nParamIndex > 3)) return E_INVALIDARG;
    
	if(nSize < 1) return E_INVALIDARG;
	
	if(!pData) return E_POINTER;
    
	switch(nParamIndex)	{
        case 1: //values vector
            if(mpValues == NULL)	{
                mnDataSize = nSize;
                
                // allocat memory to record input data
                mpValues = new double[nSize];
                if(!mpValues) return E_OUTOFMEMORY;
                
                Int32 i = 0;
                for(i = 0; i < nSize; i++)
                    mpValues[i] = pData[i];
            } else
                _ASSERT(false);
            
            break;
            
        case 2: // financial rate
            if (nSize != 1)
                return E_FAIL;
            
            if(!mbFRReady){
                mfFinanicalRate = *pData;
                mbFRReady = true;
            } else 
                _ASSERT(false);
            
            break;
            
        case 3: // reinvestment rate
            if (nSize != 1)
                return E_FAIL;
            
            if(!mbRRReady){
                mfReinvestmentRate = *pData;
                mbRRReady = true;
            } else 
                _ASSERT(false);
            
            break;
            
        default:
            _ASSERT(false);
            
	}
    
    
	return S_OK;
}

int DSSFinancialVSSTOS::GetInputParamType(Int32 nParamIndex, EnumDSSParameterType *pType)
{
    if(!pType) return E_POINTER;
    
	switch (nParamIndex) {
        case 1:
            *pType = DssParameterVector;
            break;
        case 2:
        case 3:
            *pType = DssParameterScalar;
            break;
        default:
            return E_FAIL;
	}
    
	return S_OK;
}

int DSSFinancialVSSTOS::Clear()
{
    if(mpValues)
    {
        delete [] mpValues;
        mpValues = NULL;
    }
	mnDataSize = 0;
	mbFRReady = false;
	mbRRReady = false;
	mFlag = DssDataInvalid;
    
	return S_OK;
}

int DSSFinancialVSSTOS::IsNullProcessable(bool *pCanProcessNULL)
{
    if(!pCanProcessNULL) return E_POINTER;
	
	*pCanProcessNULL = VARIANT_FALSE; // always assume input data is valid
    
	return S_OK;
}

int DSSFinancialVSSTOS::setProperty (Int32 Index, CComVariant *pValue)
{
    return E_NOTIMPL;
}

int DSSFinancialVSSTOS::Init (Int32 iFunctionIndex)
{
//    if (iFunctionIndex != 49) 
//		return E_FAIL; // must be MIRR
    
    _ASSERT( iFunctionIndex == 49);
	mFunctionIndex = iFunctionIndex;
	return S_OK;
}
