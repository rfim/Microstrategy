//
//  DSSFirst.cpp
//  ReportServiceCore
//
//  Created by Yang Zhuo on 7/27/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//

#include "DSSFirst.h"

DSSFirst::DSSFirst()
: mResultFlag(DssDataInvalid)
{
}


DSSFirst::~DSSFirst()
{
}

int DSSFirst::setProperty (Int32 Index, CComVariant *pValue)
{
 	return E_NOTIMPL;
}
/*
 int DSSFirst::Calculate (Int32 nSize, double *pVec, double *opResult, DSSDataFlag *pFlag)
 {
 if(nSize < 1)
 return E_INVALIDARG;
 
 
 CHECK_PTR(pVec);
 CHECK_PTR(opResult);
 CHECK_PTR(pFlag);
 if (DssFuncFirst == mFunctionIndex) {
 *opResult = pVec[0];
 *pFlag = DssDataOk;
 }
 else
 {
 *opResult = pVec[nSize - 1];
 *pFlag = DssDataOk;
 }
 
 return S_OK;
 }
 
 int DSSFirst::Calculate(int nSize, CComVariant * pVec, EnumDSSDataType iType,
 CComVariant * pResult, DSSDataFlag  * pFlag)
 {
 if(nSize < 1)
 return E_INVALIDARG;
 
 
 CHECK_PTR(pVec);
 CHECK_PTR(pResult);
 CHECK_PTR(pFlag);
 
 VariantInit(pResult);
 if (mFunctionIndex == DssFuncFirst)
 {
 LONG i = 0;
 SafeArrayGetElement(pVec->parray, &i, pResult);
 *pFlag	= DssDataOk;
 
 } else if (mFunctionIndex == DssFuncLast)
 {
 LONG i = nSize - 1;
 SafeArrayGetElement(pVec->parray, &i, pResult);
 *pFlag	= DssDataOk;
 }
 
 return S_OK;
 }
 */
int DSSFirst::Calculate()
{
    return E_NOTIMPL;
}


int DSSFirst::SetInputParam (Int32 nParamIndex, Int32 nSize, EnumDSSDataType dataType,
                             CComVariant *pData, DSSDataFlag *pFlag)
{
    int hr = S_OK;
   	if(nParamIndex != 1)
    {
        return hr = E_FAIL;
    }
    
    
	if(nSize < 1)
    {
        return hr = E_FAIL;
    }
	
	if(!pData	|| ( (pData->vt & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT)) )
    {
        return hr = E_FAIL;
    }
    
	VariantInit(&mResult);
	if (mFunctionIndex == DssFuncFirst)
	{
		Int32 i = 0;
		for (; i < nSize; i ++)
			if (pFlag[i] == DssDataOk) //get first non-NULL element of array
				break;
        
		if (i == nSize)
			mResultFlag	= DssDataInvalid; // all NULLs
		else
		{
            
			LONG temp = i;
			SafeArrayGetElement(pData->parray, &temp, &mResult);
			mResultFlag	= DssDataOk;
		}
	} else if (mFunctionIndex == DssFuncLast)
	{
		Int32 i = nSize - 1;
		for (; i >= 0; i --)
            if (pFlag[i] == DssDataOk) //get first non-NULL element of array
				break;
        
		if (i < 0)
			mResultFlag	= DssDataInvalid; // all NULLs
		else
		{
            
			LONG temp = i;
			SafeArrayGetElement(pData->parray, &temp, &mResult);
			mResultFlag	= DssDataOk;
		}
        
	}
    
    
    
	return hr = S_OK;
}

int DSSFirst::GetOutput (Int32 nSize, Int32 *nDataSize, CComVariant *pData, DSSDataFlag *pFlag)
{
    assert(nSize == 1);
    
	CHECK_PTR(nDataSize);
	CHECK_PTR(pData);
	CHECK_PTR(pFlag);
    
	*nDataSize = 1;
	*pFlag = mResultFlag;
    
	if (mResultFlag == DssDataOk)
		VariantCopy(pData, &mResult);
    
    
	VariantClear(&mResult);
    
	return S_OK;
}

int DSSFirst::GetOutputType (Int32 nSize, EnumDSSDataType *pInputType,
                             EnumDSSDataType *pOutputDataType, EnumDSSParameterType *pOutParamType)
{
	CHECK_PTR(pInputType);
	CHECK_PTR(pOutputDataType);
	CHECK_PTR(pOutParamType);
    
	*pOutParamType = DssParameterScalar;
    
	AE_ASSERT(nSize == 1);
	
	if ( *pInputType == DssDataTypeMBChar || *pInputType == DssDataTypeUTF8Char)	// weiwang 2009-06-05 TQMS352644 UTF8 support
		*pOutputDataType = DssDataTypeVarChar;
	else
		*pOutputDataType = *pInputType;
    
	
	return S_OK;
}

int DSSFirst::getOutputType (EnumDSSDataType pInputType, EnumDSSDataType * pOutputDataType )
{
    CHECK_PTR(pOutputDataType);
    
    if ( pInputType == DssDataTypeMBChar || pInputType == DssDataTypeUTF8Char)	// weiwang 2009-06-05 TQMS352644 UTF8 support
		*pOutputDataType = DssDataTypeVarChar;
	else
		*pOutputDataType = pInputType;
    
	
	return S_OK;
}

int DSSFirst::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
	if (!pType)
    {
		return E_FAIL;
    }
    
	if (nParamIndex != 1)
    {
		return E_FAIL;
    }
    
	*pType = DssParameterVector;
    
	return S_OK;
    //## end CDSSFirst::GetInputParamType%3846DF0A0247.body
}

int DSSFirst::IsNullProcessable (bool *pCanProcessNULL)
{
	if(!pCanProcessNULL)
		return E_FAIL;
	
	*pCanProcessNULL = true; // always assume input data is valid
	return S_OK;
}

int DSSFirst::Clear()
{
 	return S_OK;
}

int DSSFirst::GetOutput(Int32 nSize, Int32 *nDataSize,  double * pData,  DSSDataFlag * pFlag)
{
    return E_NOTIMPL;
}

int DSSFirst::GetOutputType(EnumDSSParameterType * pType)
{
    *pType = DssParameterScalar;
    
    return S_OK;
}

int DSSFirst::SetInputParam(Int32 nParamIndex, Int32 nSize, double * pData,  DSSDataFlag * pFlag)
{
    return E_NOTIMPL;
}
