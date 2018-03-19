//
//  DSSRunningNNGeneric.cpp
//  ReportServiceCore
//
//  Created by Guanlin Shen on 01/26/01.
//  Copyright 2001 MicroStrategy, Inc. All rights reserved.
//

#include "DSSRunningNNGeneric.h"

#import "DSSFunctionUtility.h"

DSSRunningNNGeneric::DSSRunningNNGeneric() : mDataSize(-1),
// jxie, 2010-11-15
mfIsNumeric(false), mpNumericResultData(NULL), mpNumericInputData(NULL)
{
}

DSSRunningNNGeneric::~DSSRunningNNGeneric()
{
	Clear();
}

int DSSRunningNNGeneric::setProperty (Int32 Index, CComVariant *pValue)
{
 	return E_NOTIMPL;
}

int DSSRunningNNGeneric::Calculate ()
{
    int hr = S_OK;
	Int32 i = 0;
    
	if (mDataSize == -1)
		return hr = E_FAIL;	// input parameters not set
    
	if (mfIsNumeric)
		return hNumericCalculate();
    
	mpResultFlagPtr.Reset(new DSSDataFlag[mDataSize]);
	CHECK_NEWED_PTR(mpResultFlagPtr.Get());
    
	for (i = 0; i < mDataSize; i++)
		mpResultFlagPtr[i] = DssDataUnknown;
    
	mResultData.vt = VT_ARRAY|VT_VARIANT;
	SAFEARRAYBOUND lSABound;
	lSABound.lLbound = 0;
	lSABound.cElements = mDataSize;
	mResultData.parray = SafeArrayCreate(VT_VARIANT, 1, &lSABound);
    
	switch (mFunctionIndex)
	{
        case DssFuncRunningMax:
        case DssFuncRunningMin:
            
            hr = CalculateRunMaxMin();
            break;
            
        case DssFuncFirstInRange:
            
            hr = CalculateFirst();
            break;
            
        case DssFuncLastInRange:
            hr = CalculateLast();
            break;
            
        case DssFuncRunningCount:
            
            hr = CalculateRunCount();
            break;
            
        default:
            return hr = E_FAIL;
	}
    
	return hr;
}

int DSSRunningNNGeneric::CalculateFirst()
{    
	//xhan 02/02/2005 
	//if FirstInRange(n - 1) is valid
	//	FirstInRange(n) = FirstInRange(n - 1) 
	//else 	if n is valid
	// FirstInRange(n) = n
    
	if (mDataSize < 1)
		return S_OK;
    
	Int32 i = 0;
	for (; i < mDataSize ; i ++)
	{
		CComVariant  lVar;
		if (i > 0 && mpDataFlagPtr[i - 1] == DssDataOk)
		{
            
			getVariant(&mResultData, i - 1, lVar);
			setVariant(&mResultData, i, lVar);			
			mpDataFlagPtr[i] = DssDataOk;
		} else if (mpDataFlagPtr[i] == DssDataOk) {
            
			getVariant(&mInputData, i, lVar);
			setVariant(&mResultData, i, lVar);			
			mpDataFlagPtr[i] = DssDataOk;
		}
	}    
    
	return  S_OK;
}

int DSSRunningNNGeneric::CalculateLast()
{
	//xhan 02/02/2005 
	//if n is valid
	//	LastInRange(n) = n 
	//else 	if LastInRange(n - 1) is valid
	// LastInRange(n) = LastInRange(n - 1)
    
	if (mDataSize < 1)
		return S_OK;
    
	Int32 i = 0;
	for (; i < mDataSize; i ++)
	{
		CComVariant  lVar;
		if (mpDataFlagPtr[i] == DssDataOk)
		{            
			getVariant(&mInputData, i, lVar);
			setVariant(&mResultData, i, lVar);			
			mpDataFlagPtr[i] = DssDataOk;
            
		} else if (i > 0 && mpDataFlagPtr[i - 1] == DssDataOk) {
            
			getVariant(&mResultData, i - 1, lVar);
			setVariant(&mResultData, i, lVar);			
			mpDataFlagPtr[i] = DssDataOk;
		}
	}
        
	return  S_OK;    
}

int DSSRunningNNGeneric::CalculateRunMaxMin()
{    
	if (mDataSize < 1)
		return S_OK;
    
	if (mpDataFlagPtr[0] == DssDataOk) { //set the first element
		mpDataFlagPtr[0] = DssDataOk;
        
		CComVariant  lVar;
		getVariant(&mInputData, 0, lVar);
		setVariant(&mResultData, 0, lVar);			
	}
    
	//RunningMax(n) = Max(RunningMax(n - 1), n)
	//RunningMin(n) = Min(RunningMin(n - 1), n)
	Int32 i = 1;
	for ( ; i < mDataSize; i ++) 
	{
		CComVariant  lPreVar;
		CComVariant  lThisVar;
        
		if (mpDataFlagPtr[i - 1] == DssDataOk)
		{            
			getVariant(&mResultData, i - 1 , lPreVar);
			if (mpDataFlagPtr[i] == DssDataOk)
			{ 
				getVariant(&mInputData, i, lThisVar);
                
				//compare this and pre
				DSSDataFlag lFlag = DssDataOk;
				Int32  lComp = variantCompare(lThisVar, lPreVar, &lFlag);
                
				if (lFlag == DssDataOk)	
				{ // comparison succeeded, check result
					if (mFunctionIndex == DssFuncRunningMax)
					{	
						if (lComp > 0) 
							setVariant(&mResultData, i, lThisVar);
						else
							setVariant(&mResultData, i, lPreVar);
					} else { // (mFunctionIndex == DssFuncRunningMin)
						if (lComp < 0) 
							setVariant(&mResultData, i, lThisVar);
						else
							setVariant(&mResultData, i, lPreVar);                        
					}
                    
					mpDataFlagPtr[i] = DssDataOk;
				} 
                
			} else { // mpDataFlagPtr[i] != DssDataOk
				//assign pre to the result
				setVariant(&mResultData, i, lPreVar);
				mpDataFlagPtr[i] = DssDataOk;
			}
            
		} else { //mpDataFlagPtr[i - 1] != DssDataOk            
			if (mpDataFlagPtr[i] == DssDataOk)
			{ //set this var to result directly
				getVariant(&mInputData, i, lThisVar);
				setVariant(&mResultData, i, lThisVar);
				mpDataFlagPtr[i] = DssDataOk;                
			}
		}
	} //for
    
	return S_OK;
}

int DSSRunningNNGeneric::CalculateRunCount()
{
	//xhan 02/02/2005 
	//if n is valid
	//	RunningCount(n) = RunningCount(n - 1) + 1
	//else 
	// RunningCount(n) = RunningCount(n - 1)
    
	if (mDataSize < 1)
		return S_OK;
       
	Int32 lRunningCount = 0;
    
	Int32 i = 0;
	for (; i < mDataSize; i ++)
	{
		if (mpDataFlagPtr[i] == DssDataOk)
			lRunningCount ++;
        
		CComVariant lResult;
		int hr = DFCDataConversion::ConvertToVariant((unsigned char *)&lRunningCount, DssDataTypeLong, 0, &lResult);
		if(hr != S_OK) return hr;
        
		setVariant(&mResultData, i, lResult);
        
		mpDataFlagPtr[i] = DssDataOk;
	}
    
	return S_OK;
}

int DSSRunningNNGeneric::SetInputParam (Int32 nParamIndex, Int32 nSize, EnumDSSDataType dataType, CComVariant *pData, DSSDataFlag *pFlag)
{
    int hr = S_OK;
    
	if(nParamIndex != 1) return E_FAIL;
	// gshen; AE_MSG; Invalid argument
    
	if(nSize < 1) return E_FAIL;
	// gshen; AE_MSG; Invalid argument
	
	if(!pData || ( (pData->vt & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT)) )
		return E_FAIL;
	// gshen; AE_MSG; Invalid pointer
        
	mDataSize = nSize;
	mpDataFlagPtr.Reset(new DSSDataFlag[mDataSize]);
	CHECK_NEWED_PTR(mpDataFlagPtr.Get());
	memmove(mpDataFlagPtr.Get(), pFlag, mDataSize * sizeof(DSSDataFlag));
    
	mInputData.Copy(pData);
    
	return hr = S_OK;
}

int DSSRunningNNGeneric::GetOutput (Int32 nSize, Int32 *nDataSize, CComVariant *pData, DSSDataFlag *pFlag)
{
    int hr = S_OK;
    
	CHECK_PTR(nDataSize);
	CHECK_PTR(pData);
	CHECK_PTR(pFlag);    
    
	if(nSize < mDataSize) 
    {
        return S_FALSE;
    }
    
	*nDataSize = mDataSize;
    
	AE_ASSERT(!mpDataFlagPtr.IsNull());
    
	VariantCopy(pData, &mResultData);
	memmove(pFlag, mpDataFlagPtr.Get(), mDataSize*sizeof(DSSDataFlag));
	
	return hr = S_OK;
}

int DSSRunningNNGeneric::GetOutputType (Int32 nSize, EnumDSSDataType *pInputType, EnumDSSDataType *pOutputDataType, EnumDSSParameterType *pOutParamType)
{
    int hr = S_OK;
    
	CHECK_PTR(pInputType);
	CHECK_PTR(pOutputDataType);
	CHECK_PTR(pOutParamType);
    
	*pOutParamType = DssParameterVector;
    
	if (mFunctionIndex == DssFuncRunningCount)
		*pOutputDataType = DssDataTypeLong;
	else if (mFunctionIndex == DssFuncRunningMax || 
             mFunctionIndex == DssFuncRunningMin ||
             mFunctionIndex == DssFuncFirstInRange ||
             mFunctionIndex == DssFuncLastInRange)
	{
		*pOutputDataType = pInputType[0];
	}
	else
        return hr = E_FAIL;
	
	return hr = S_OK;
}

int DSSRunningNNGeneric::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
    int hr = S_OK;
    
	if (!pType) 
		return hr = E_FAIL;
    
	if (nParamIndex != 1)
		return hr = E_FAIL;
    
	*pType = DssParameterVector;
    
	return hr = S_OK;
}

int DSSRunningNNGeneric::IsNullProcessable (bool *pCanProcessNULL)
{
    int hr = S_OK;
	if(!pCanProcessNULL) 
		return hr = E_FAIL;
	
	*pCanProcessNULL = true; // always assume input data is valid
    
	return hr = S_OK;
}

int DSSRunningNNGeneric::Clear()
{
    int hr = S_OK;
    
	mInputData.Clear();
	mResultData.Clear();
    
	// jxie, 2010-11-15
	delete [] mpNumericResultData;
	delete [] mpNumericInputData;
	mfIsNumeric = false;
    
	// jxie, 2011-3-29, reset the pointers
	mpNumericResultData = NULL;
	mpNumericInputData = NULL;
    
	return hr = S_OK;
}

// jxie, 2010-9-29
int DSSRunningNNGeneric::GetOutput(Int32 nSize, Int32 *nDataSize,  double * pData, DSSDataFlag * pFlag)
{
    int hr = S_OK;
    
	CHECK_PTR(nDataSize);
	CHECK_PTR(pData);
	CHECK_PTR(pFlag);    
    
	if(nSize < mDataSize) 
    {
        return S_FALSE;
    }
    
	*nDataSize = mDataSize;
    
	AE_ASSERT(!mpDataFlagPtr.IsNull());
    
	memmove(pData, mpNumericResultData, mDataSize * sizeof(double) );
	memmove(pFlag, mpDataFlagPtr.Get(), mDataSize*sizeof(DSSDataFlag));
	
	return hr = S_OK;
}

// jxie, 2010-9-29
int DSSRunningNNGeneric::GetOutputType(EnumDSSParameterType * pType)
{
	if(!pType) return E_POINTER;
    
	*pType = DssParameterVector;
	
	return S_OK;
}

// jxie, 2010-9-29
int DSSRunningNNGeneric::SetInputParam(Int32 nParamIndex, Int32 nSize, double * pData, DSSDataFlag * pFlag)
{
    int hr = S_OK;
    
	if(nParamIndex != 1) return E_FAIL;
	// gshen; AE_MSG; Invalid argument
    
	if(nSize < 1) return E_FAIL;
	// gshen; AE_MSG; Invalid argument
	
	if(!pData)
		return E_FAIL;
	// gshen; AE_MSG; Invalid pointer
    
	mfIsNumeric = true;
    
	mDataSize = nSize;
	mpDataFlagPtr.Reset(new DSSDataFlag[mDataSize]);
	CHECK_NEWED_PTR(mpDataFlagPtr.Get());
	memmove(mpDataFlagPtr.Get(), pFlag, mDataSize * sizeof(DSSDataFlag));
	mpNumericInputData = new double[mDataSize];
	memmove(mpNumericInputData, pData, mDataSize * sizeof(double));
    
	return hr = S_OK;
}

// jxie, 2010-9-29
int DSSRunningNNGeneric::hNumericCalculate ()
{
	int hr = S_OK;
	Int32 i = 0;
    
	mpResultFlagPtr.Reset(new DSSDataFlag[mDataSize]);
	CHECK_NEWED_PTR(mpResultFlagPtr.Get());
    
	for (i = 0; i < mDataSize; i++)
		mpResultFlagPtr[i] = DssDataUnknown;
    
	mpNumericResultData = new double[mDataSize];
    
	switch (mFunctionIndex)
	{
        case DssFuncRunningMax:
        case DssFuncRunningMin:
            
            hr = hNumericCalculateRunMaxMin();
            break;
            
        case DssFuncFirstInRange:
            
            hr = hNumericCalculateFirst();
            break;
            
        case DssFuncLastInRange:
            hr = hNumericCalculateLast();
            break;
            
        case DssFuncRunningCount:
            
            hr = hNumericCalculateRunCount();
            break;
            
        default:
            return hr = E_FAIL;
	}
    
	return hr;
}

// jxie, 2010-9-29
int DSSRunningNNGeneric::hNumericCalculateFirst()
{    
	//xhan 02/02/2005 
	//if FirstInRange(n - 1) is valid
	//	FirstInRange(n) = FirstInRange(n - 1) 
	//else 	if n is valid
	// FirstInRange(n) = n
	if (mDataSize < 1)
		return S_OK;
    
	Int32 i = 0;
	for (; i < mDataSize ; i ++)
	{
		if (i > 0 && mpDataFlagPtr[i - 1] == DssDataOk)
		{
			mpNumericResultData[i] = mpNumericResultData[i - 1];
			mpDataFlagPtr[i] = DssDataOk;
		}
		else if (mpDataFlagPtr[i] == DssDataOk)
		{
			mpNumericResultData[i] = mpNumericInputData[i];
			mpDataFlagPtr[i] = DssDataOk;
		}
	}
    
	return  S_OK;
}

// jxie, 2010-9-29
int DSSRunningNNGeneric::hNumericCalculateLast()
{
	//xhan 02/02/2005 
	//if n is valid
	//	LastInRange(n) = n 
	//else 	if LastInRange(n - 1) is valid
	// LastInRange(n) = LastInRange(n - 1)
    
	if (mDataSize < 1)
		return S_OK;
    
	Int32 i = 0;
	for (; i < mDataSize; i ++)
	{
		if (mpDataFlagPtr[i] == DssDataOk)
		{
			mpNumericResultData[i] = mpNumericInputData[i];
			mpDataFlagPtr[i] = DssDataOk;
            
		} else if (i > 0 && mpDataFlagPtr[i - 1] == DssDataOk) {
            
			mpNumericResultData[i] = mpNumericResultData[i - 1];
			mpDataFlagPtr[i] = DssDataOk;
		}
	}
    
	return  S_OK;
}

// jxie, 2010-9-29
int DSSRunningNNGeneric::hNumericCalculateRunMaxMin()
{    
	if (mDataSize < 1)
		return S_OK;
    
	if (mpDataFlagPtr[0] == DssDataOk) 
    { //set the first element
		mpDataFlagPtr[0] = DssDataOk;        
		mpNumericResultData[0] = mpNumericInputData[0];
	}
    
	//RunningMax(n) = Max(RunningMax(n - 1), n)
	//RunningMin(n) = Min(RunningMin(n - 1), n)
    
	Int32 i = 1;
	for ( ; i < mDataSize; i ++) 
	{
		double lPreVar;
		double lThisVar;
        
		if (mpDataFlagPtr[i - 1] == DssDataOk)
		{
			lPreVar = mpNumericResultData[i - 1];
			if (mpDataFlagPtr[i] == DssDataOk)
			{ 
				lThisVar = mpNumericInputData[i];
                
				//compare this and pre
				if (mFunctionIndex == DssFuncRunningMax)
				{	
					if (lThisVar > lPreVar) 
						mpNumericResultData[i] = lThisVar;
					else
						mpNumericResultData[i] = lPreVar;
				} else { // (mFunctionIndex == DssFuncRunningMin)
					if (lThisVar < lPreVar) 
						mpNumericResultData[i] = lThisVar;
					else
						mpNumericResultData[i] = lPreVar;
				}
                
				mpDataFlagPtr[i] = DssDataOk;
                
			} else { // mpDataFlagPtr[i] != DssDataOk
				//assign pre to the result
				mpNumericResultData[i] = lPreVar;
				mpDataFlagPtr[i] = DssDataOk;
			}            
		} else { //mpDataFlagPtr[i - 1] != DssDataOk            
			if (mpDataFlagPtr[i] == DssDataOk)
			{ //set this var to result directly
				mpNumericResultData[i] = mpNumericInputData[i];
				mpDataFlagPtr[i] = DssDataOk;
			}
		}
	} //for
    
	return S_OK;
}

// jxie, 2010-9-29
int DSSRunningNNGeneric::hNumericCalculateRunCount()
{
	//xhan 02/02/2005 
	//if n is valid
	//	RunningCount(n) = RunningCount(n - 1) + 1
	//else 
	// RunningCount(n) = RunningCount(n - 1)
	if (mDataSize < 1)
		return S_OK;
    
	Int32 lRunningCount = 0;
    
	Int32 i = 0;
	for (; i < mDataSize; i ++)
	{
		if (mpDataFlagPtr[i] == DssDataOk)
			lRunningCount ++;
        
		mpNumericResultData[i] = lRunningCount;
		mpDataFlagPtr[i] = DssDataOk;
	}
    
	return S_OK;
}