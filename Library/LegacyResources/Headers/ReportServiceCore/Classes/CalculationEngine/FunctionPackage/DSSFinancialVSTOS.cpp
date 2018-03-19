//
//  DSSFinancialVSTOS.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 2/22/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#include "DSSFinancialVSTOS.h"
#include "DSSFunctionUtility.h"
#include "financialFuncImplement.h"

DSSFinancialVSTOS::DSSFinancialVSTOS(): mFunctionIndex(0), mpValues(NULL), mnDataSize(0),
mfScalar(0), mbScalarReady(false), mfResult(0), mFlag(DssDataInvalid)
{
}

DSSFinancialVSTOS::~DSSFinancialVSTOS()
{
    if (mpValues) {
        delete [] mpValues;
        mpValues = NULL;
    }
}

int DSSFinancialVSTOS::Calculate()
{
    if(!mpValues || !mbScalarReady) 
		return E_FAIL;
    
	enum MathError error = MathNoError;
	HRESULT hr = S_OK;
    
	try {
		if (mFunctionIndex == 46) //fvschedule
			mfResult = _FVSCHEDULE(&error, mfScalar, mnDataSize, mpValues);
		else if (mFunctionIndex == 47) //npv
			mfResult = _NPV(&error, mfScalar, mnDataSize, mpValues);
		else
			hr = E_FAIL;
	} catch (...) {
		hr = E_FAIL;
	}
    
	if(error == MathNoError)
		mFlag = DssDataOk;
	else
		mFlag = DssDataInvalid;
    
	return hr;
}

int DSSFinancialVSTOS::GetOutput(Int32 nSize, 	// Input buffer size
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

int DSSFinancialVSTOS::GetOutputType (EnumDSSParameterType *pType)
{
    if(!pType) return E_POINTER;
    
	*pType = DssParameterScalar;
	
	return S_OK;
}

int DSSFinancialVSTOS::SetInputParam(Int32 nParamIndex, 	// The index of input parameter
                  Int32 nSize, 	// Size of input double array
                  double *pData,
                  DSSDataFlag *pFlag)
{
    if((nParamIndex < 1) || (nParamIndex > 2)) return E_INVALIDARG;
    
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
            
        case 2: // scalar
            if (nSize != 1)
                return E_FAIL;
            
            if(!mbScalarReady){
                mfScalar = *pData;
                mbScalarReady = true;
            } else 
                _ASSERT(false);
            
            break;
            
        default:
            _ASSERT(false);
            
	}
    
    
	return S_OK;
}

int DSSFinancialVSTOS::GetInputParamType(Int32 nParamIndex, EnumDSSParameterType *pType)
{
    if(!pType) return E_POINTER;
    
	switch (nParamIndex) {
        case 1:
            *pType = DssParameterVector;
            break;
        case 2:
            *pType = DssParameterScalar;
            break;
        default:
            return E_FAIL;
	}
    
	return S_OK;
}

int DSSFinancialVSTOS::Clear()
{
    delete [] mpValues;
	mpValues = NULL;
    
	mnDataSize = 0;
	mbScalarReady = false;
	mFlag = DssDataInvalid;
    
	return S_OK;
}

int DSSFinancialVSTOS::IsNullProcessable(bool *pCanProcessNULL)
{
    if(!pCanProcessNULL) return E_POINTER;
	
	*pCanProcessNULL = VARIANT_FALSE; // always assume input data is valid
    
	return S_OK;
}

int DSSFinancialVSTOS::setProperty (Int32 Index, CComVariant *pValue)
{
    return E_NOTIMPL;
}

int DSSFinancialVSTOS::Init (Int32 iFunctionIndex)
{
    _ASSERT(iFunctionIndex == 46 || iFunctionIndex == 47);
	if (iFunctionIndex != 46 && iFunctionIndex != 47) 
		return E_FAIL; // must be Fvschedule and NPV
    
  	mFunctionIndex = iFunctionIndex;
	return S_OK;
}
