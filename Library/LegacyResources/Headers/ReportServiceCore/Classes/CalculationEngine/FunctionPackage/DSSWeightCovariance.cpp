//
//	DSSWeightCovariance.cpp
//  ReportServiceCore
//
//  Create by xzhu on 07/01/2014
//  Copyright (c) 2014 MicroStratey, Inc. All rights reserved.


#include "DSSWeightCovariance.h"
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

DSSWeightCovarianceFunction::DSSWeightCovarianceFunction():mbUseAsWeightCorr(false), mpxValues(NULL), mpyValues(NULL), mpWeights(NULL), mpFlags(NULL), mDataSize(0),
														mResult(0.0), mResultFlag(DssDataOk)
{
}

DSSWeightCovarianceFunction::~DSSWeightCovarianceFunction()
{
	delete[] mpxValues;
	delete[] mpyValues;
	delete[] mpWeights;
	delete[] mpFlags;
}

int DSSWeightCovarianceFunction::get_FunctionType (EnumDSSFunctionType *  pType)
{
	if (!pType) 
		return E_POINTER;

	*pType = DssFunctionTypeGeneric;

	return S_OK;
}

int DSSWeightCovarianceFunction::setProperty (Int32 Index, CComVariant *pValue)
{
	return E_NOTIMPL;
}

int DSSWeightCovarianceFunction::Calculate ()
{
	double* lpValidxValues = new double[mDataSize];
	CHECK_NEWED_PTR(lpValidxValues);

	MemManager lValidxValues;
	lValidxValues.mpBuffer = lpValidxValues;

	double* lpValidyValues = new double[mDataSize];
	CHECK_NEWED_PTR(lpValidyValues);

	MemManager lValidyValues;
	lValidyValues.mpBuffer = lpValidyValues;

	double* lpValidWeights = new double[mDataSize];
	CHECK_NEWED_PTR(lpValidWeights);

	MemManager lValidWeights;
	lValidWeights.mpBuffer = lpValidWeights;

	Int32 lIndex = 0;
	for(Int32 i = 0; i < mDataSize; i ++)
	{
		if(mpFlags[i] == DssDataOk)
		{
			lpValidxValues[lIndex] = mpxValues[i];
			lpValidyValues[lIndex] = mpyValues[i];
			lpValidWeights[lIndex ++] = mpWeights[i];
		}
	}

	//calculate weighted standard deviation
	double lxWeightAve = 0.0;
	if(WeightMeanAverage(lIndex, lpValidxValues, lpValidWeights, &lxWeightAve) == 0)
	{
		mResultFlag = DssDataInvalid; //failed to return
		return S_OK;
	}
	
	double lyWeightAve = 0.0;
	if(WeightMeanAverage(lIndex, lpValidyValues, lpValidWeights, &lyWeightAve) == 0)
	{
		mResultFlag = DssDataInvalid; //failed to return
		return S_OK;
	}

	double lTotalNumerator = 0.0;
	double lTotalWeight = 0.0;

	for(Int32 i = 0; i < lIndex; i ++)
	{
		lTotalNumerator += lpValidWeights[i] * (lpValidxValues[i] - lxWeightAve) * (lpValidyValues[i] - lyWeightAve);
		lTotalWeight += lpValidWeights[i];
	}

	double lWcov = 0.0;
	lWcov = lTotalNumerator / lTotalWeight;

	
	if(mbUseAsWeightCorr)
	{
		double lXXTotalNumerator = 0.0;
		double lYYTotalNumerator = 0.0;

		for(Int32 i = 0; i < lIndex; i ++)
		{
			lXXTotalNumerator += lpValidWeights[i] * (lpValidxValues[i] - lxWeightAve) * (lpValidxValues[i] - lxWeightAve);
			lYYTotalNumerator += lpValidWeights[i] * (lpValidyValues[i] - lyWeightAve) * (lpValidyValues[i] - lyWeightAve);
		}

		double lXXWcov = lXXTotalNumerator / lTotalWeight;
		double lYYWcov = lYYTotalNumerator / lTotalWeight;

		//The value of lXXWcov and lYYWcov is impossible to be negative.
		double tmp = lXXWcov * lYYWcov;
		tmp = tmp > 0 ? tmp : -tmp;
		
		if(tmp > ZERO_EPLISON) //not equal 0
			mResult = lWcov / sqrt(lXXWcov * lYYWcov);
		else
		{
			mResultFlag = DssDataInvalid;
			return S_OK;
		}
	}
	else
		mResult = lWcov;

	mResultFlag = DssDataOk;

	return S_OK;
}

int DSSWeightCovarianceFunction::SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSData_Flags *pFlag)
{
	if(!pData || !pFlag)
		return E_POINTER;

	switch(nParamIndex)
	{
	case 1:
		{
			//mpxValues
			if(mpxValues != NULL)
				delete[] mpxValues;

			mpxValues = new double[nSize];
			CHECK_NEWED_PTR(mpxValues);

			mpFlags = new DSSData_Flags[nSize];
			CHECK_NEWED_PTR(mpFlags);

			memmove(mpFlags, pFlag, sizeof(DSSData_Flags) * nSize);

			for(Int32 i = 0; i < nSize; i ++)
			{
				if(mpFlags[i] == DssDataOk)
					mpxValues[i] = pData[i];
				else
					mpxValues[i] = 0; //for invalid cell, mark it as 0
			}

			mDataSize = nSize;
		}
		break;
	case 2:
		{
			if(mDataSize != nSize)
				return E_FAIL; //the size of vector input and wights are different

			//mpyValues
			if(mpyValues != NULL)
				delete[] mpyValues;

			mpyValues = new double[nSize];
			CHECK_NEWED_PTR(mpyValues);

			for(Int32 i = 0; i < nSize; i ++)
			{
				if(pFlag[i] == DssDataOk)
				{
					mpyValues[i] = pData[i];
				}
				else
				{
					mpFlags[i] = pFlag[i];
				}
			}
		}
		break;
	case 3:
		{
			//mpWeights
			if(mDataSize != nSize)
				return E_FAIL; //the size of vector input and wights are different

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

int DSSWeightCovarianceFunction::GetOutput (Int32 nSize, Int32 *nDataSize, double *pData, DSSData_Flags *pFlag)
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

int DSSWeightCovarianceFunction::GetOutputType (EnumDSSParameterType *pType)
{
	if(pType == NULL)
		return E_FAIL;

	*pType = DssParameterScalar;

	return S_OK;
}

int DSSWeightCovarianceFunction::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
	if(pType == NULL)
		return E_POINTER;
	
	if(nParamIndex == 1 || nParamIndex == 2 || nParamIndex == 3)
		*pType = DssParameterVector;
	else
		return E_FAIL;

	return S_OK;
}

int DSSWeightCovarianceFunction::IsNullProcessable (bool *pCanProcessNULL)
{
	if(!pCanProcessNULL)
		return E_FAIL;

	*pCanProcessNULL = VARIANT_TRUE;
	return S_OK;
}

int DSSWeightCovarianceFunction::Clear ()
{
	delete[] mpxValues;
	mpxValues = NULL;

	delete[] mpyValues;
	mpyValues = NULL;

	delete[] mpWeights;
	mpWeights = NULL;
	
	delete[] mpFlags;
	mpFlags = NULL;

	mResult = 0.0;
	mResultFlag =  DssDataOk;
	mDataSize = 0;

	return S_OK;
}

int DSSWeightCovarianceFunction::SetUseAsWeightStdevP(bool iUseAsWeightCorr)
{
	mbUseAsWeightCorr = iUseAsWeightCorr;
	return S_OK;
}