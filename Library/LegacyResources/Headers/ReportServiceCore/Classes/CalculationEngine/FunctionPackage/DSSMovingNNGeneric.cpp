//
//  DSSNNMovingGeneric.cpp
//  ReportServiceCore
//
//  Created by Guanlin Shen on 01/26/01.
//  Copyright 2001 MicroStrategy, Inc. All rights reserved.
//

#include "DSSMovingNNGeneric.h"

#import "DSSFunctionUtility.h"

 DSSMovingNNGeneric:: DSSMovingNNGeneric()
: mpDataFlag(NULL), mpResultFlag(NULL),mDataSize(-1), mWinSize(0),
// jxie, 2010-11-15
mfIsNumeric(false), mpNumericInputData(NULL), mpNumericResultData(NULL),
// jxie, 2010-12-16
mOldCount(0)
{
}

DSSMovingNNGeneric::~ DSSMovingNNGeneric()
{
	Clear();
}

int DSSMovingNNGeneric::setProperty (Int32 Index, CComVariant *pValue)
{
 	return E_NOTIMPL;
}

int DSSMovingNNGeneric::Calculate ()
{
    int hr = S_OK;
    
	Int32 i = 0;
	if (mDataSize == -1)
		return hr = E_FAIL;	// input parameters not set
    
	if (mfIsNumeric)
		return hNumericCalculate();
    
	mpResultFlag = new DSSDataFlag[mDataSize];
	CHECK_NEWED_PTR(mpResultFlag);
    
	for (i = 0; i < mDataSize; i++)
		mpResultFlag[i] = DssDataUnknown;
    
	mResultData.vt = VT_ARRAY|VT_VARIANT;
	SAFEARRAYBOUND lSABound;
	lSABound.lLbound = 0;
	lSABound.cElements = mDataSize;
	mResultData.parray = SafeArrayCreate(VT_VARIANT, 1, &lSABound);
    
	switch (mFunctionIndex)
	{
        case DssFuncMovingMax:
        case DssFuncMovingMin:
		{            
			for (i = 0; i < mDataSize; i++)
				CalculateMoveMaxMin(i);
		}            
            break;
            
        case DssFuncMovingCount:
		{
			for (i = 0; i < mDataSize; i++)
				hOptimizedCount(i);
		}            
            break;
            
        default:
            return hr = E_FAIL;
	}
    
	return hr = S_OK;
}

int DSSMovingNNGeneric::CalculateMoveMaxMin(Int32 idx)
{
    int hr = S_OK;
    
	Int32 i = 0,  iStart = 0;    
    
	if (idx >= mWinSize - 1)
		iStart = idx - mWinSize + 1;
	else
		iStart = 0;
    
	CComVariant lResult;
	for (i = iStart; i <= idx; i ++)
	{
		if (mpDataFlag[i] == DssDataOk)
		{
			CComVariant  lVar;
			getVariant(&mInputData, i, lVar);
            
			if (mpResultFlag[idx] != DssDataOk)  // the first element from the start of the window
			{
				mpResultFlag[idx] = DssDataOk;
				lResult.Copy(&lVar);
			}
			else 
			{
				DSSDataFlag lFlag = DssDataOk;
                
				Int32  lComp = variantCompare(lResult, lVar, &lFlag);
				if (lFlag == DssDataOk)	
				{ // comparison succeeded, check result
					if (lComp > 0 && mFunctionIndex == DssFuncMovingMin)
						lResult.Copy(&lVar);
					else if (lComp < 0 && mFunctionIndex == DssFuncMovingMax)
						lResult.Copy(&lVar);                    
				} 
			} //if-else 
            
		} //if 
        
	} //for
    
	setVariant(&mResultData, idx, lResult);
    
	return hr = S_OK;
}

int DSSMovingNNGeneric::CalculateMoveCount(Int32 idx)
{
    int hr = S_OK;
    
	Int32 i = 0,  iStart = 0 ;
        
	if (idx >= mWinSize - 1)
		iStart = idx - mWinSize + 1;
	else
		iStart = 0;
        
	Int32 lCount = 0;
    
	for (i = iStart; i <= idx; i ++)
	{
		if (mpDataFlag[i] == DssDataOk)
			lCount ++;
	}	
	
	CComVariant lResult;
	hr = DFCDataConversion::ConvertToVariant((unsigned char *)&lCount, DssDataTypeLong, 0, &lResult);
	if(hr != S_OK) return hr;
    
	setVariant(&mResultData, idx, lResult);
    
	mpResultFlag[idx] = DssDataOk;
    
	return hr = S_OK;
}

int DSSMovingNNGeneric::SetInputParam (Int32 nParamIndex, Int32 nSize, EnumDSSDataType dataType, CComVariant *pData, DSSDataFlag *pFlag)
{
    int hr = S_OK;
    
	if(nSize < 1) return E_FAIL;
	// gshen; AE_MSG; Invalid argument
	
	if(!pData)
		return E_FAIL;
	// gshen; AE_MSG; Invalid pointer    
    
	if (nParamIndex == 1)
	{
		if ((pData->vt & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT))
			return E_FAIL;
        
		mDataSize = nSize;
		mpDataFlag = new DSSDataFlag[mDataSize];
		CHECK_NEWED_PTR(mpDataFlag);
		memmove(mpDataFlag, pFlag, mDataSize * sizeof(DSSDataFlag));
        
		mInputData.Copy(pData);        
	}
	else if (nParamIndex == 2)
	{
		// dataType == DssDataTypeLong
		Int32 nLongSize = sizeof(Int32);
		hr = DFCDataConversion::ConvertFromVariant(pData, 
										DssDataTypeLong,
										(BYTE *)&mWinSize, 
										&nLongSize);
		CHECK_HR;
	}
	else 
    {
        return S_FALSE;
    }
    
	return hr = S_OK;
}

int DSSMovingNNGeneric::GetOutput (Int32 nSize, Int32 *nDataSize, CComVariant *pData, DSSDataFlag *pFlag)
{
	int hr = S_OK;
    
	CHECK_PTR(nDataSize);
	CHECK_PTR(pData);
	CHECK_PTR(pFlag);
        
	if(nSize < mDataSize)
        return S_FALSE;
        
	*nDataSize = mDataSize;
    
	AE_ASSERT(mpResultFlag != NULL);
    
	VariantCopy(pData, &mResultData);
    
	memmove(pFlag, mpResultFlag, mDataSize*sizeof(DSSDataFlag));
	
	return hr = S_OK;
}

int DSSMovingNNGeneric::GetOutputType (Int32 nSize, EnumDSSDataType *pInputType, EnumDSSDataType *pOutputDataType, EnumDSSParameterType *pOutParamType)
{
    int hr = S_OK;
    
	CHECK_PTR(pInputType);
	CHECK_PTR(pOutputDataType);
	CHECK_PTR(pOutParamType);
    
	*pOutParamType = DssParameterVector;
    
	if (mFunctionIndex == DssFuncMovingCount)
		*pOutputDataType = DssDataTypeLong;
	else if (mFunctionIndex == DssFuncMovingMax || mFunctionIndex == DssFuncMovingMin)
	{
		*pOutputDataType = pInputType[0];
	}
	else
		return hr = E_FAIL;
	
	return hr = S_OK;
}

int DSSMovingNNGeneric::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
    int hr = S_OK;
    
	if (!pType) 
		hr = E_FAIL;
    
	switch (nParamIndex)
	{
        case 1:
            *pType = DssParameterVector;
            hr = S_OK;
            break;
        case 2:
            *pType = DssParameterScalar;
            hr = S_OK;
            break;
        default:
            hr = E_FAIL;
            break;
	}
    
	return hr;
}

int DSSMovingNNGeneric::IsNullProcessable (bool *pCanProcessNULL)
{
    int hr = S_OK;
	
    if(!pCanProcessNULL) 
		return hr = E_FAIL;
	
	*pCanProcessNULL = true; // always assume input data is valid
    
	return hr = S_OK;
}

int DSSMovingNNGeneric::Clear()
{    
    int hr = S_OK;
    
	if (mpDataFlag)
		delete [] mpDataFlag;
    
	if (mpResultFlag)
		delete [] mpResultFlag;
    
	mpDataFlag = NULL;
	mpResultFlag = NULL;
    
	mWinSize = 0;
	mDataSize = -1;
    
	mInputData.Clear();
	mResultData.Clear();
    
	// jxie, 2010-11-18
	delete [] mpNumericResultData;
	delete [] mpNumericInputData;
	mfIsNumeric = false;
    
	// jxie, 2011-3-29, reset the pointers
	mpNumericResultData = NULL;
	mpNumericInputData = NULL;
    
	// jxie, 2010-12-16
	mOldCount = 0;
    
	return hr = S_OK;
}

// jxie, 2010-9-29
int DSSMovingNNGeneric::GetOutput(Int32 nSize, Int32 *nDataSize,  double * pData, DSSDataFlag * pFlag)
{
    int hr = S_OK;
    
	CHECK_PTR(nDataSize);
	CHECK_PTR(pData);
	CHECK_PTR(pFlag);
    
	if(nSize < mDataSize) 
		return S_FALSE;
    
	*nDataSize = mDataSize;
    
	AE_ASSERT(mpResultFlag != NULL);
    
	memmove(pData, mpNumericResultData, mDataSize * sizeof(double) );
	memmove(pFlag, mpResultFlag, mDataSize * sizeof(DSSDataFlag) );
	
	return hr = S_OK;
}

// jxie, 2010-9-29
int DSSMovingNNGeneric::GetOutputType(EnumDSSParameterType * pType)
{
	if(!pType) return E_POINTER;
    
	*pType = DssParameterVector;
	
	return S_OK;
}

// jxie, 2010-9-29
int DSSMovingNNGeneric::SetInputParam(Int32 nParamIndex, Int32 nSize, double * pData, DSSDataFlag * pFlag)
{
	int hr = S_OK;
    
	if(nSize < 1) return E_FAIL;
	// gshen; AE_MSG; Invalid argument
	
	if(!pData)
		return E_FAIL;
	// gshen; AE_MSG; Invalid pointer
    
	mfIsNumeric = true;
    
	if (nParamIndex == 1)
	{
		mDataSize = nSize;
		mpDataFlag = new DSSDataFlag[mDataSize];
		CHECK_NEWED_PTR(mpDataFlag);
		memmove(mpDataFlag, pFlag, mDataSize * sizeof(DSSDataFlag));
		mpNumericInputData = new double[mDataSize];
		memmove(mpNumericInputData, pData, mDataSize * sizeof(double));
        
	}
	else if (nParamIndex == 2)
	{
		mWinSize = ChangeDoubleToInt(*pData);
	}
	else
		S_FALSE;
        
	return hr = S_OK;
}

// jxie, 2010-9-29
int DSSMovingNNGeneric::hNumericCalculate ()
{
	int hr = S_OK;
	Int32 i = 0;
	mpResultFlag = new DSSDataFlag[mDataSize];
	CHECK_NEWED_PTR(mpResultFlag);
    
	for (i = 0; i < mDataSize; i++)
		mpResultFlag[i] = DssDataUnknown;
    
	mpNumericResultData = new double[mDataSize];
    
	switch (mFunctionIndex)
	{
        case DssFuncMovingMax:
        case DssFuncMovingMin:
		{            
			for (i = 0; i < mDataSize; i++)
				hNumericCalculateMoveMaxMin(i);
		}            
            break;
            
        case DssFuncMovingCount:
		{
			for (i = 0; i < mDataSize; i++)
				hNumericOptimizedCount(i);
		}            
            break;
            
        default:
            return hr = E_FAIL;
	}
    
	return hr = S_OK;
}

// jxie, 2010-9-29
int DSSMovingNNGeneric::hNumericCalculateMoveMaxMin(Int32 idx)
{
	 int hr = S_OK;
	Int32 i = 0,  iStart = 0 ;
    
	if (idx >= mWinSize - 1)
		iStart = idx - mWinSize + 1;
	else
		iStart = 0;
    
	double lResult;
	for (i = iStart; i <= idx; i ++)
	{
		if (mpDataFlag[i] == DssDataOk)
		{
			double lVar = mpNumericInputData[i];
            
			if (mpResultFlag[idx] != DssDataOk)  // the first element from the start of the window
			{
				mpResultFlag[idx] = DssDataOk;
				lResult = lVar;
			}
			else 
			{                
				if (lResult > lVar && mFunctionIndex == DssFuncMovingMin)
					lResult = lVar;
				else if (lResult < lVar && mFunctionIndex == DssFuncMovingMax)
					lResult = lVar;
			} //if-else
		} //if 
	} //for
    
	mpNumericResultData[idx] = lResult;
    
	return hr = S_OK;
}

// jxie, 2010-9-29
int DSSMovingNNGeneric::hNumericCalculateMoveCount(Int32 idx)
{
    int hr = S_OK;
	Int32 i = 0,  iStart = 0 ;
    
	if (idx >= mWinSize - 1)
		iStart = idx - mWinSize + 1;
	else
		iStart = 0;
    
	Int32 lCount = 0;
	for (i = iStart; i <= idx; i ++)
	{
		if (mpDataFlag[i] == DssDataOk)
			lCount ++;
	}
	
	mpNumericResultData[idx] = lCount;
	mpResultFlag[idx] = DssDataOk;
    
	return hr = S_OK;
}

// jxie, 2010-12-16
// reuse the calculation result of previous window to optimize the calculation
int DSSMovingNNGeneric::hNumericOptimizedCount(Int32 idx)
{
	if (idx + 1 > mWinSize)
	{
		Int32 iStart = idx - mWinSize;
        
		if (mpDataFlag[iStart] == DssDataOk)
			mOldCount--;
	}
	
	if (mpDataFlag[idx] == DssDataOk)
		mOldCount++;
	
	mpNumericResultData[idx] = mOldCount;
	mpResultFlag[idx] = DssDataOk;
    
	return S_OK;
}

// jxie, 2010-12-16
// reuse the calculation result of previous window to optimize the calculation
int DSSMovingNNGeneric::hOptimizedCount(Int32 idx)
{
	int hr = S_OK;
    
	if (idx + 1 > mWinSize)
	{
		Int32 iStart = idx - mWinSize;
        
		if (mpDataFlag[iStart] == DssDataOk)
			mOldCount--;
	}
	
	if (mpDataFlag[idx] == DssDataOk)
		mOldCount++;
    
	CComVariant lResult;
	hr = DFCDataConversion::ConvertToVariant((unsigned char *)&mOldCount, DssDataTypeLong, 0, &lResult);
	if(hr != S_OK) return hr;
    
	setVariant(&mResultData, idx, lResult);
    
	mpResultFlag[idx] = DssDataOk;
    
	return hr = S_OK;
}