//
//	DSSWeightMeanAve.cpp
//  ReportServiceCore
//
//  Create by xzhu on 07/20/2014
//  Copyright (c) 2014 MicroStratey, Inc. All rights reserved.

#include "DSSWeightMeanAve.h"
#include "DSSFunctionUtility.h"
#include <math.h>
#define ZERO_EPLISON (1.0e-15)

struct MemManager
{
	double *mpBuffer;
	MemManager():mpBuffer(NULL){}
	~MemManager()
	{
		delete[] mpBuffer;
		mpBuffer = NULL;
	}
};

DSSWeightMeanAveFunction::DSSWeightMeanAveFunction(): mpValues(NULL), mpWeights(NULL), mpFlags(NULL), mDataSize(0),
														mResult(0.0), mResultFlag(DssDataOk), mbUseAsWeightStdevP(false)
{
}

DSSWeightMeanAveFunction::~DSSWeightMeanAveFunction()
{
	delete[] mpValues;
	delete[] mpWeights;
	delete[] mpFlags;
}

int DSSWeightMeanAveFunction::get_FunctionType (EnumDSSFunctionType *  pType)
{
	if (!pType) 
		return E_POINTER;

	*pType = DssFunctionTypeGeneric;

	return S_OK;
}

int DSSWeightMeanAveFunction::setProperty (Int32 Index, CComVariant *pValue)
{
	return E_NOTIMPL;
}

int DSSWeightMeanAveFunction::Calculate ()
{
	double* lpValidValues = new double[mDataSize];
	CHECK_NEWED_PTR(lpValidValues);

	MemManager lValidValues;
	lValidValues.mpBuffer = lpValidValues;

	double* lpValidWeights = new double[mDataSize];
	CHECK_NEWED_PTR(lpValidWeights);

	MemManager lValidWeights;
	lValidWeights.mpBuffer = lpValidWeights;

	Int32 lIndex = 0;
	for(Int32 i = 0; i < mDataSize; i ++)
	{
		if(mpFlags[i] == DssDataOk)
		{
			lpValidValues[lIndex] = mpValues[i];
			lpValidWeights[lIndex ++] = mpWeights[i];
		}
	}

	if(mbUseAsWeightStdevP)
	{
		//calculate standard deviation
		double lWeightAve = 0.0;
		if(WeightMeanAverage(lIndex, lpValidValues, lpValidWeights, &lWeightAve) == 0)
		{
			mResultFlag = DssDataInvalid;
			return S_OK;
		}

		//Get the number of non-zero weights
		Int32 lNumNonZeroWeight = 0;
		for(Int32 i = 0; i < mDataSize; i ++)
		{
			if(mpFlags[i] == DssDataOk)
			{
				double tmp = mpWeights[i] > 0 ? mpWeights[i] : -mpWeights[i];
				if(tmp > ZERO_EPLISON) // not equal 0
					lNumNonZeroWeight ++;
			}
		}

		if(lNumNonZeroWeight <= 1)
		{
			mResultFlag = DssDataInvalid;
			return S_OK;
		}

		double lTotalWeight = 0.0;
		double lNumerator = 0.0;
		for(Int32 i = 0; i < lIndex; i ++)
		{
			lTotalWeight += lpValidWeights[i];
			lNumerator += lpValidWeights[i] * (lpValidValues[i] - lWeightAve) * (lpValidValues[i] - lWeightAve);
		}

		double lTmp = lNumerator / ((lNumNonZeroWeight - 1) * lTotalWeight / lNumNonZeroWeight);

		if(lTmp < 0)
		{
			mResultFlag = DssDataInvalid;
			return S_OK;
		}
		else
		{
			mResultFlag =  DssDataOk;
			mResult = sqrt(lTmp);
		}
	}
	else
	{
		//calculate Weight Mean Average
		if(mDataSize > 0)
		{
			//calcualte the Weighted Mean Average(WMA)
			if(WeightMeanAverage(lIndex, lpValidValues, lpValidWeights, &mResult) == 0)
			{
				mResultFlag = DssDataInvalid;
				return S_OK;
			}
			else
				mResultFlag =  DssDataOk;
		}
		else
		{
			mResultFlag = DssDataInvalid;
			return E_FAIL;
		}
	}

	return S_OK;
}

int DSSWeightMeanAveFunction::SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSData_Flags *pFlag)
{
	if(!pData || !pFlag)
		return E_POINTER;

	switch(nParamIndex)
	{
	case 1:
		{
			//mpValues
			if(mpValues != NULL)
				delete[] mpValues;

			mpValues = new double[nSize];
			CHECK_NEWED_PTR(mpValues);

			mpFlags = new DSSData_Flags[nSize];
			CHECK_NEWED_PTR(mpFlags);

			memmove(mpFlags, pFlag, sizeof(DSSData_Flags) * nSize);

			for(Int32 i = 0; i < nSize; i ++)
			{
				if(mpFlags[i] == DssDataOk)
					mpValues[i] = pData[i];
				else
					mpValues[i] = 0; //for invalid cell, mark it as 0
			}

			mDataSize = nSize;
		}
		break;
	case 2:
		{
			if(mDataSize != nSize)
				return E_FAIL; //the size of vector input and wights are different

			//mpWeights
			if(mpWeights != NULL)
				delete[] mpWeights;

			mpWeights = new double[nSize];
			CHECK_NEWED_PTR(mpWeights);

			for(Int32 i = 0; i < nSize; i ++)
			{
				if(pFlag[i] == DssDataOk)
				{
					mpWeights[i] = pData[i];

					if(mpWeights[i] < 0) //negative weight is not allowed
						return E_FAIL;
				}
				else
				{
					mpFlags[i] = pFlag[i];
				}
			}

		}
		break;
	default:
		{
			return E_FAIL;  // invalid property index
		}
	}
	return S_OK;
}

int DSSWeightMeanAveFunction::GetOutput (Int32 nSize, Int32 *nDataSize, double *pData, DSSData_Flags *pFlag)
{
	if(!nDataSize || !pData || !pFlag) 
		return E_POINTER;

	if (nSize < 1) 
		return E_FAIL;
	
	*pData = mResult;
	*pFlag = mResultFlag;

	*nDataSize = 1;

	return S_OK;
}

int DSSWeightMeanAveFunction::GetOutputType (EnumDSSParameterType *pType)
{
	if(pType == NULL)
		return E_FAIL;

	*pType = DssParameterScalar;

	return S_OK;
}

int DSSWeightMeanAveFunction::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
	if(pType == NULL)
		return E_POINTER;
	
	if(nParamIndex == 1 || nParamIndex == 2)
		*pType = DssParameterVector;
	else
		return E_FAIL;

	return S_OK;
}

int DSSWeightMeanAveFunction::IsNullProcessable (bool *pCanProcessNULL)
{
	if(!pCanProcessNULL)
		return E_FAIL;

	*pCanProcessNULL = VARIANT_TRUE;
	return S_OK;
}

int DSSWeightMeanAveFunction::Clear ()
{
	delete[] mpValues;
	mpValues = NULL;

	delete[] mpWeights;
	mpWeights = NULL;
	
	delete[] mpFlags;
	mpFlags = NULL;

	mResult = 0.0;
	mResultFlag =  DssDataOk;
	mDataSize = 0;

	return S_OK;
}

int DSSWeightMeanAveFunction::SetUseAsWeightStdevP(bool iUseAsWeightStdevP)
{
	mbUseAsWeightStdevP = iUseAsWeightStdevP;
	return S_OK;
}