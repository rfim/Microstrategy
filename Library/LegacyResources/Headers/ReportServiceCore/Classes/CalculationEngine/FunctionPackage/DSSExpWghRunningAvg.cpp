//
//  DSSExpWghRunningAvg.cpp
//  ReportServiceCore
//
//  Created by Jia Zhang on 7/27/11.
//  Copyright 2011 MicroStrategy, Inc. All rights reserved.
//

#include "DSSExpWghRunningAvg.h"

#define UNINITIALIZED	-1

DSSExpWghRunningAvg::DSSExpWghRunningAvg(): mDataSize(UNINITIALIZED), mpData(NULL), 
mpResult(NULL), mpDataFlag(NULL), mpResultFlag(NULL), mRate(UNINITIALIZED)
{
}

DSSExpWghRunningAvg::~DSSExpWghRunningAvg()
{
	delete [] mpData;
	delete [] mpResult;
	delete [] mpDataFlag;
	delete [] mpResultFlag;
}

int DSSExpWghRunningAvg::setProperty (Int32 Index, CComVariant *pValue)
{
    return E_NOTIMPL;
}

int DSSExpWghRunningAvg::Calculate ()
{
	Int32 i = 0;
 	if (mDataSize == UNINITIALIZED)
		return E_FAIL;
    
	// check whether all the required input parameters have been set.
	if (!mpData || !mpDataFlag)
		return E_FAIL;    
    
	// Allocate space for mpResult and mpFlag
	mpResult = new double[mDataSize];  
	if(!mpResult) return E_OUTOFMEMORY;
    
	mpResultFlag = new DSSDataFlag[mDataSize];
	if(!mpResultFlag) return E_OUTOFMEMORY;
    
	for (i = 0; i < mDataSize; i++)
		mpResultFlag[i] = DssDataUnknown;
    
	if (mRate < 0)
		return S_OK;
    
	for (i = 0; i < mDataSize; i++)
		ExpBackward(i);
    
	return S_OK;
}

int DSSExpWghRunningAvg::SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSDataFlag*pFlag)
{
	if (nSize < 1) return E_INVALIDARG;
	
	if (!pData) return E_POINTER;
    
	switch (nParamIndex)
	{
		case 1:
			// value list
			mDataSize = nSize;
			// allocat memory to record input data
			mpData = new double[nSize];
			if(!mpData) return E_OUTOFMEMORY;
			memmove(mpData, pData, nSize * sizeof(double));
			mpDataFlag = new DSSDataFlag[nSize];
			if(!mpDataFlag) return E_OUTOFMEMORY;
			memmove(mpDataFlag, pFlag, nSize * sizeof(DSSDataFlag));
			break;
		case 2:
			mRate = *pData;
			break;
		default:
			return E_INVALIDARG;
	}
    
 	return S_OK;
}

int DSSExpWghRunningAvg::GetOutput (Int32 nSize, Int32 *nDataSize, double *pData, DSSDataFlag*pFlag)
{
	if(!nDataSize || !pData || !pFlag) return E_POINTER;
    
	// output buffer size shouldn't be smaller than mDataSize
	if (nSize < mDataSize) 
		return E_FAIL;
    
	// copy the result out
	Int32 i = 0;
	for(i = 0; i < mDataSize; i++)
	{
		pData[i] = mpResult[i];
		pFlag[i] = mpResultFlag[i];
	}
    
	*nDataSize = mDataSize;
    
	return S_OK;
}

int DSSExpWghRunningAvg::GetOutputType (EnumDSSParameterType *pType)
{
	if(!pType) return E_POINTER;
    
	*pType = DssParameterVector;
	
	return S_OK;
}

int DSSExpWghRunningAvg::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
 	if(!pType) return E_POINTER;
    
	switch (nParamIndex)
	{
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

int DSSExpWghRunningAvg::IsNullProcessable (bool *pCanProcessNULL)
{
	if(!pCanProcessNULL) return E_POINTER;
	
	*pCanProcessNULL = true; // always assume input data is valid
    
	return S_OK;
}

int DSSExpWghRunningAvg::Clear ()
{
	delete [] mpData;
	delete [] mpResult;
	delete [] mpDataFlag;
	delete [] mpResultFlag;
    
	mDataSize = UNINITIALIZED;
	mRate = UNINITIALIZED;
	//lwang; 12/01/2008; 337727, reset pointer
	mpResult = NULL;
	mpResultFlag = NULL;
	mpData = NULL;
	mpDataFlag = NULL;
    
	return S_OK;
}

void DSSExpWghRunningAvg::ExpBackward (Int32 iVal)
{
	Int32 i = 0;
	double lSigmaWeight, lSigmaWeightedX, CurrentWeight;    
    
	lSigmaWeight = 0;
	lSigmaWeightedX = 0;
	CurrentWeight = 1;
    
	// jxie, TQMS446409, 2010-12-20
	// if mRate > 1, use 1/mRate to calculate from start to end to avoid very big data
	if (mRate <= 1)
	{
		for (i = iVal; i >= 0; i--)
		{
			if (mpDataFlag[i] == DssDataOk)
			{
				lSigmaWeight += CurrentWeight;
				lSigmaWeightedX += mpData[i] * CurrentWeight;
				CurrentWeight *= mRate;
			}
		}
	}
	else
	{
		double lReverseRate = 1.0 / mRate;
		for (i = 0; i <= iVal; i++)
		{
			if (mpDataFlag[i] == DssDataOk)
			{
				lSigmaWeight += CurrentWeight;
				lSigmaWeightedX += mpData[i] * CurrentWeight;
				CurrentWeight *= lReverseRate;
			}
		}
	}
    
	if (lSigmaWeight != 0)
	{
		mpResultFlag[iVal] = DssDataOk;
		mpResult[iVal] = lSigmaWeightedX / lSigmaWeight;
	}
}
