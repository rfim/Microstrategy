//
//  DSSRunningGeneric.cpp
//  ReportServiceCore
//
//  Created by Guanlin Shen on 09/14/99.
//  Copyright 1999 MicroStrategy, Inc. All rights reserved.
//

#include "DSSRunningGeneric.h"

#import "DSSFunctionUtility.h"

#define UNINITIALIZED -1

DSSRunningGeneric::DSSRunningGeneric() : mDataSize(UNINITIALIZED)
{
}

DSSRunningGeneric::~DSSRunningGeneric()
{
}

int DSSRunningGeneric::setProperty (Int32 Index, CComVariant *pValue)
{
    return E_NOTIMPL;
}

int DSSRunningGeneric::Calculate ()
{
 	Int32 i = 0;
 
	if(mDataSize == UNINITIALIZED || mFunctionIndex==DssFuncUnknown)
    {
        return S_FALSE;
    }
    
	// check whether all the required input parameters have been set.    
	if(!mpDataPtr.Get() || !mpDataFlagPtr.Get())
    {
        return S_FALSE;
    }
    
	// Allocate space for mpResultPtr and mpFlag
	mpResultPtr.Reset(new double[mDataSize]);  
	if(mpResultPtr.IsNull()) return E_OUTOFMEMORY;
    
	mpResultFlagPtr.Reset(new DSSDataFlag[mDataSize]);
	if(mpResultFlagPtr.IsNull()) return E_OUTOFMEMORY;
    
	for (i = 0; i < mDataSize; i++)
		mpResultFlagPtr[i] = DssDataUnknown;    
    
	switch (mFunctionIndex)
	{
		case DssFuncRunningSum:
			CalculateRunningSum();
			break;
            
		case DssFuncRunningAvg:
			CalculateRunningAvg();
			break;
            
		case DssFuncRunningStdev:
		case DssFuncRunningStdevP:
            
			CaculateRunnningStdev();
			break;
		default:
			return E_FAIL;
	}    
    
	return S_OK;
}

int DSSRunningGeneric::SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSDataFlag *pFlag)
{    
	if(nParamIndex != 1) return E_INVALIDARG;
    
	if(nSize < 1) return E_INVALIDARG;
	
	if(!pData) return E_POINTER;
    
	mDataSize = nSize;
	// allocat memory to record input data
	mpDataPtr.Reset(new double[nSize]);
	if(mpDataPtr.IsNull()) return E_OUTOFMEMORY;
	memmove(mpDataPtr.Get(), pData, nSize * sizeof(double));
    
	mpDataFlagPtr.Reset(new DSSDataFlag[nSize]);
	if(mpDataFlagPtr.IsNull()) return E_OUTOFMEMORY;
	memmove(mpDataFlagPtr.Get(), pFlag, nSize * sizeof(DSSDataFlag));    
    
	return S_OK;
}

int DSSRunningGeneric::GetOutput (Int32 nSize, Int32 *nDataSize, double *pData, DSSDataFlag *pFlag)
{
 	if(!nDataSize || !pData || !pFlag) return E_POINTER;
    
	// output buffer size shouldn't be smaller than mDataSize
	if(nSize < mDataSize) 
    {
        return S_FALSE;
    }
    
	// copy the result out
	Int32 i = 0;
	for(i = 0; i < mDataSize; i++)
	{
		pData[i] = mpResultPtr[i];
		pFlag[i] = mpResultFlagPtr[i];
	}
    
	*nDataSize = mDataSize;
    
	return S_OK;
}

int DSSRunningGeneric::GetOutputType (EnumDSSParameterType *pType)
{
  	if(!pType) return E_POINTER;
    
	*pType = DssParameterVector;
	
	return S_OK;
}

int DSSRunningGeneric::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
	if(!pType) return E_POINTER;
    
	if (nParamIndex != 1)
		return E_FAIL;
    
	*pType = DssParameterVector;
    
	return S_OK;
}

int DSSRunningGeneric::IsNullProcessable (bool *pCanProcessNULL)
{
	if(!pCanProcessNULL) return E_POINTER;
	
	*pCanProcessNULL = true; // 05/14/2008 mwang, now all olap func can process Null
    
	return S_OK;
}

int DSSRunningGeneric::Clear ()
{    
	mDataSize = UNINITIALIZED;
    
	return S_OK;
}

//xhan 02/02/05
int DSSRunningGeneric::CalculateRunningSum()
{
	// if n is valid
	//	RunningSum(n) = RunningSum(n - 1) + n
	// else
	//  RunningSum(n) = RunningSum(n - 1)
    
	double lRunningSum = 0;
 	Int32 i = 0;
    
	for (; i < mDataSize; i++)
	{
		if (mpDataFlagPtr[i] == DssDataOk)
		{
			lRunningSum += mpDataPtr[i];
			mpResultPtr[i] = lRunningSum;
			mpResultFlagPtr[i] = DssDataOk;
		} else 
		{
			mpResultPtr[i] = lRunningSum;
			if (i > 0)
				mpResultFlagPtr[i] = mpResultFlagPtr[i - 1];
            
		}
	}    
    
	return S_OK;  
}

//xhan 02/02/05
int DSSRunningGeneric::CalculateRunningAvg()
{
	//RunningAvg (n) = RunningSum(n) / RunningCount(n)
	// if n is valid
	//	RunningSum(n) = RunningSum(n - 1) + n
	//	RunningCount(n) = RunningCount(n - 1) + 1
	// else
	//  RunningSum(n) = RunningSum(n - 1)
	//	RunningCount(n) = RunningCount(n - 1)
    
	double lRunningSum = 0;
	Int32 lRunningCount = 0;
 	Int32 i = 0;
	for (; i < mDataSize; i++)
	{
		if (mpDataFlagPtr[i] == DssDataOk)
		{
			lRunningSum += mpDataPtr[i];
			lRunningCount ++;
		}
        
		if (lRunningCount > 0)
		{
			mpResultPtr[i] = lRunningSum / lRunningCount;
			mpResultFlagPtr[i] = DssDataOk;
		}
        
	}
    
    return S_OK;  
}

//xhan 02/02/05
int DSSRunningGeneric::CaculateRunnningStdev()
{
    // if n is valid, input array N = N-1 + n
	// RunningStdDev(n) = stdDev(N)
    
	MBase::StrongArrayPtr<double> lpTempData(new double[mDataSize]);
	if (lpTempData.IsNull())
		return E_OUTOFMEMORY;
    
	Int32 lRunningCount = 0;
	Int32 i = 0;
	for (; i < mDataSize; i++)
	{
		if (mpDataFlagPtr[i] == DssDataOk)
			lpTempData[lRunningCount ++] = mpDataPtr[i];
        
		Int32 lRet = 0;
        
		if (mFunctionIndex == DssFuncRunningStdev)
			lRet = Variance(lRunningCount, lpTempData.Get(), &(mpResultPtr[i]) );
		else  //mFunctionIndex = DssFuncRunningStdevP
			lRet = VarianceP(lRunningCount, lpTempData.Get(), &(mpResultPtr[i]) );
                
		if (!lRet) 
			mpResultFlagPtr[i] = DssDataInvalid;
		else 
		{
			mpResultPtr[i] = sqrt(mpResultPtr[i]);
			mpResultFlagPtr[i] = DssDataOk;
            
		}        
	}    
    
	return S_OK;
}