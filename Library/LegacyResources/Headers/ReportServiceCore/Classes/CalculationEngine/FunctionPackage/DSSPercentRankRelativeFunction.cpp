//
//	DSSPercentRankRelative.cpp
//  ReportServiceCore
//
//  Create by xzhu on 06/20/2014
//  Copyright (c) 2014 MicroStratey, Inc. All rights reserved.
//


#include "DSSPercentRankRelativeFunction.h"

DSSPercentRankRelativeFunction::DSSPercentRankRelativeFunction(): mpdistValues(NULL), mpinValues(NULL), mdblSigFactor(1000),
						mbTruncate(true), mbRankOutliers(false), mbInclusive(true), mDataSize(0), mdistValueCnt(0),
						mpResults(NULL), mpFlags(NULL), mIsOneInput(false)
						
{
}

DSSPercentRankRelativeFunction::~DSSPercentRankRelativeFunction()
{
	delete[] mpdistValues;
	delete[] mpinValues;
	delete[] mpResults;
	delete[] mpFlags;
}

int DSSPercentRankRelativeFunction::get_FunctionType (EnumDSSFunctionType *  pType)
{
	//AE_ENTER_FUNC("DSSPercentRankRelativeFunction::get_FunctionType", AE_TRACE_MINOR);

	if(pType == NULL)
		return E_POINTER;

	*pType = DssFunctionTypeGeneric;

	return S_OK;
}

int DSSPercentRankRelativeFunction::setProperty (Int32 Index, CComVariant *pValue)
{
	//AE_ENTER_FUNC("DSSPercentRankRelativeFunction::SetProperty", AE_TRACE_MINOR);
	
	if(pValue == NULL)
		return E_POINTER;
    
    int hr=S_OK;
	
	//if mIsOneInput is true, the function is PercentRank, in this case the parameter mbRankOutliers has no meaning, 
	//because there is only one input for PercentRank, so there are only one parameter (inclusive) for PercentRank
	//PercentRankRelative
	switch(Index)
	{
	case 1:  // inclusive
		{
			hr = VariantChangeType(pValue, pValue, 0, VT_BOOL);
			if ( FAILED(hr) ) return hr;

			if(pValue->vt == VT_BOOL)
			{
				mbInclusive = (pValue->boolVal==VARIANT_TRUE);
			}
			else
			{
				return E_FAIL;
			}
			break;
		}
	case 2:  // rankOutliers
		{
			if(mIsOneInput)
				return E_FAIL; //should not come here for PercentRank

			hr = VariantChangeType(pValue, pValue, 0, VT_BOOL);
			if ( FAILED(hr) ) return hr;

			if(pValue->vt == VT_BOOL)
			{
				mbRankOutliers = (pValue->boolVal==VARIANT_TRUE);
			}
			else
			{
				return E_FAIL;
			}
			break;
		}
	case 3: //significance
		{
			if(mIsOneInput)
				return E_FAIL; //should not come here for PercentRank

            hr = VariantChangeType(pValue, pValue, 0, VT_I4);
			if ( FAILED(hr) ) return hr;

			if(pValue->vt == VT_I4)
			{
				// validate value
				if(pValue->lVal < 1)
					pValue->lVal = 1;
				else if(pValue->lVal > 9)
					pValue->lVal = 9;

				mdblSigFactor = pow((double)10, pValue->lVal);
			}
			else
			{
				return E_FAIL;
			}
			break;
		}
	case 4: //truncate
		{
			if(mIsOneInput)
				return E_FAIL; //should not come here for PercentRank

			hr = VariantChangeType(pValue, pValue, 0, VT_BOOL);
			if ( FAILED(hr) ) return hr;

			if(pValue->vt == VT_BOOL)
			{
				mbTruncate = (pValue->boolVal==VARIANT_TRUE);
			}
			else
			{
				return E_FAIL;
			}
			break;
		}
	default:
		{
			return E_FAIL;  // invalid property index
		}
	}

	return S_OK;
}

int DSSPercentRankRelativeFunction::Calculate ()
{
	if(mpFlags == NULL && mDataSize)
	{
		mpFlags = new DSSData_Flags[mDataSize];
		CHECK_NEWED_PTR(mpFlags);
	}

	if(mpResults == NULL && mDataSize)
	{
		mpResults = new double[mDataSize];
		CHECK_NEWED_PTR(mpResults);
	}

	for(Int32 i = 0; i < mDataSize; i ++)
		mpFlags[i] = DssDataInvalid;

	if(mdistValueCnt > 0)
	{
		// calculate PercentRank
		HRESULT hr = CalcPercentRank(mpResults, mpFlags);
		CHECK_HR;
	}

	return S_OK;
}

int DSSPercentRankRelativeFunction::GetOutput(Int32 nSize, Int32 *nDataSize,  double * pData, DSSData_Flags * pFlag)
{
	//AE_ENTER_FUNC("DSSPercentRankRelativeFunction::GetOutput", AE_TRACE_MINOR);

	if(!nDataSize || !pData || !pFlag) 
		return E_POINTER;

	if (nSize < 1) 
		return E_FAIL;
	
	AE_ASSERT(nSize == mDataSize);

	for(Int32 i = 0; i < mDataSize; i ++)
	{
		pData[i] = mpResults[i];
		pFlag[i] = mpFlags[i];
	}
	*nDataSize = nSize;
	
	return S_OK;
}

int DSSPercentRankRelativeFunction::Clear ()
{
	//AE_ENTER_FUNC("DSSPercentRankRelativeFunction::Clear", AE_TRACE_MINOR);

	delete[] mpdistValues;
	mpdistValues = NULL;

	mdistValueCnt = 0;

	delete[] mpinValues;
	mpinValues = NULL;

	delete[] mpResults;
	mpResults = NULL;

	delete[] mpFlags;
	mpFlags = NULL;

	mDataSize = 0;

	return S_OK;
}

int DSSPercentRankRelativeFunction::SetInputParam(Int32 nParamIndex, Int32 nSize, double * pData, DSSData_Flags * pFlag)
{
	//AE_ENTER_FUNC("DSSPercentRankRelativeFunction::SetInputParam", AE_TRACE_MINOR);

	bool lbVector = false;
	
	if(!pData || !pFlag)
		return E_POINTER;

	if(nSize < 1)
		return E_FAIL;

	switch (nParamIndex)
	{
		case 1:  // mpdistValues
			{
				mpdistValues = new double[nSize];
				if (mpdistValues == NULL)
					return E_OUTOFMEMORY;

				if(mIsOneInput)//true, we will both set mpdistValues and mpinValues to the same value
				{
					mpinValues = new INVALUE_PAIR[nSize];
					if (mpinValues == NULL)
						return E_OUTOFMEMORY;
				}

				for(Int32 i=0;i<nSize;i++)
				{
					if (pFlag[i] == DssDataOk)
					{
						mpdistValues[mdistValueCnt] = pData[i];
						mdistValueCnt++;
					}

					if(mIsOneInput)
					{
						if (pFlag[i] == DssDataOk)
						{
							mpinValues[i].first = pData[i];
							mpinValues[i].second = true;
						}
						else
						{
							mpinValues[i].second = false;
						}
					}
				}
				lbVector = true;
				break;
			}
		case 2:  // mpinValues
			{
				if(mIsOneInput)
					AE_ASSERT(false);

				mpinValues = new INVALUE_PAIR[nSize];
				if (mpinValues == NULL)
					return E_OUTOFMEMORY;

				for(Int32 i=0;i<nSize;i++)
				{
					if (pFlag[i] == DssDataOk)
					{
						mpinValues[i].first = pData[i];
						mpinValues[i].second = true;
					}
					else
						mpinValues[i].second = false;
				}
				lbVector = true;
				break;
			}
		default:
			{
				return E_FAIL;  // invalid parameter index
			}
	}

	// validate vector input size
	if (lbVector)
	{
		// vector input...all vector inputs must have same size
		if (mDataSize == 0)
			// first vector input parameter
			mDataSize = nSize;
		else
		{
			// all vector input parameters expected to have same size
			if (mDataSize != nSize)
				return E_FAIL;
		}
	}
	//else...scalar input...no need to compare size to other inputs

	return S_OK;
}

int DSSPercentRankRelativeFunction::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
	//AE_ENTER_FUNC("DSSPercentRankRelativeFunction::GetInputParamType", AE_TRACE_MINOR);

	if(pType == NULL)
		return E_POINTER;
	
	if(nParamIndex == 1 || nParamIndex == 2)
		*pType = DssParameterVector;
	else
		return E_FAIL;

	return S_OK;
}

int DSSPercentRankRelativeFunction::IsNullProcessable(bool *pCanProcessNULL)
{
	//AE_ENTER_FUNC("DSSPercentRankRelativeFunction::IsNullProcessable", AE_TRACE_MINOR);
	if(!pCanProcessNULL)
		return E_FAIL;

	*pCanProcessNULL = VARIANT_TRUE;
	return S_OK;
}



int DSSPercentRankRelativeFunction::CalcPercentRank(double *pValues, DSSData_Flags *pFlag)
{
	// process inputs
	if(mdistValueCnt > 1)
	{
		// sort distribution values
		HRESULT hr = hQuickSort(mpdistValues, 0, mdistValueCnt - 1);
		CHECK_HR;

		Int32 lPercentDenom = mbInclusive ? (mdistValueCnt - 1) : (mdistValueCnt + 1);
		for(Int32 i=0;i<mDataSize;i++)
		{
			double dblInVal = mpinValues[i].first;

			// check for null input value
			if(!mpinValues[i].second)
				// inValue is missing...skip it...result will be empty cell
				continue;

			// check for outliers
			if(!mIsOneInput && IsOutlier(dblInVal))
			{
				if(mIsOneInput)//For PercentRank, should not come here
					AE_ASSERT(false);

				// outlier...check rank outliers flag
				if(mbRankOutliers)
				{
					// percentRank of outlier is 0 or 1
					pValues[i] = (mpinValues[i].first < mpdistValues[0]) ? 0 : 1;
					pFlag[i] = DssDataOk;
				}
				//else...not ranking outliers...return empty cell
			}
			else
			{
				// inValue falls with distribution...determine rank
				Int32 lRank = lPercentDenom;
				bool bCalcNormRank = false;	// true if (low < value < high) and value not in distribution
				double dblLeftVal;
				double dblRightVal;

				lRank = hBinarySearch(mpdistValues, dblInVal, 0, mdistValueCnt - 1);

				bCalcNormRank = (mpdistValues[lRank] != dblInVal);
				if(bCalcNormRank)
				{
					// rank must be > 1 (if 0, inVal matches first distVal, in which case calcNormRank will be false)
					dblLeftVal = mpdistValues[lRank - 1];
					dblRightVal = mpdistValues[lRank];
				}

				if(!mbInclusive)
					// adjust rank
					lRank ++;

				// calculate PercentRank
				double dblPercent;
				if(!bCalcNormRank)
					dblPercent = (double)lRank / (double)(lPercentDenom);
				else
				{
					// calculate normalized rank using the following formula:
					//	n1 + (x - o1) / (o2 - o1) * (n2 - n1) for o1 <= x <= o2
					// where o represents original (distribution) values, n represents normalized (rank) values, and x is the
					// in value to be ranked.  Since (n2 - n1) is guaranteed to be 1 (equivalent to (rank - (rank - 1))), this
					// factor can be eliminated.
					double lNormRank = (lRank - 1) + (dblInVal - dblLeftVal) / (dblRightVal - dblLeftVal);
					dblPercent = lNormRank / (double)(lPercentDenom);
				}

				// round result per specifications
				if(!mIsOneInput) //Do not round for PrecentRank
					pValues[i] = Round(dblPercent);
				else
					pValues[i] = dblPercent;

				pFlag[i] = DssDataOk;
			}
		}
	}
	else
	{
		// special case for distribution vectors of size 1
		for(Int32 i=0;i<mDataSize;i++)
		{
			if(mbRankOutliers)
			{
				pValues[i] = (mpinValues[i].first < mpdistValues[0]) ? 0 : 1;
				pFlag[i] = DssDataOk;
			}
			else if(mpinValues[i].first==mpdistValues[0])
			{
				pValues[i] = 1;
				pFlag[i] = DssDataOk;
			}
			//else... rankOutliers flag is false and value is outlier...return empty cell
		}
	}
	return S_OK;
}

int DSSPercentRankRelativeFunction::GetOutputType(EnumDSSParameterType * pType)
{
	//AE_ENTER_FUNC("DSSPercentRankRelativeFunction::GetOutputType", AE_TRACE_MINOR);

    CHECK_PTR(pType);

	*pType = DssParameterVector;

	return S_OK;
}

Int32 DSSPercentRankRelativeFunction::hPartition(double *pArray, Int32 iLeft, Int32 iRight, Int32 iPivot)
{
	double dblPivotValue = pArray[iPivot];

	// swap pivot and right elements
	double dblTemp = pArray[iRight];
	pArray[iRight] = pArray[iPivot];
	pArray[iPivot] = dblTemp;

	Int32 lStoreIndex = iLeft;
	for(Int32 i=iLeft;i<iRight;i++)
	{
		if(pArray[i] <= dblPivotValue)
		{
			// swap current and storeIndex elements
			dblTemp = pArray[lStoreIndex];
			pArray[lStoreIndex] = pArray[i];
			pArray[i] = dblTemp;

			lStoreIndex++;
		}
	}

	// swap storeIndex and right elements
	dblTemp = pArray[iRight];
	pArray[iRight] = pArray[lStoreIndex];
	pArray[lStoreIndex] = dblTemp;

	return lStoreIndex;
}

int DSSPercentRankRelativeFunction::hQuickSort(double *pArray, Int32 iLeft, Int32 iRight)
{
	// sort if array size > 1
	Int32 lSize = iRight - iLeft + 1;
	if(lSize > 1)
	{
		// choose pivot
		Int32 lPivot;
		Int32 lMiddle = iLeft + (iRight - iLeft) / 2;
		if(lSize < 5)
			lPivot = lMiddle;
		else
		{
			if(pArray[iLeft] >= pArray[lMiddle])
			{
				if(pArray[iLeft] <= pArray[iRight])
					lPivot = iLeft;
				else
					lPivot = (pArray[lMiddle] >= pArray[iRight]) ? lMiddle : iRight;
			}
			else
			{
				if(pArray[iLeft] >= pArray[iRight])
					lPivot = iLeft;
				else
					lPivot = (pArray[lMiddle] >= pArray[iRight]) ? iRight : lMiddle;
			}
		}

		// get array of bigger and smaller items and final position of pivot
		Int32 lNewPivot = hPartition(pArray, iLeft, iRight, lPivot);

		// recursively sort elements smaller than pivot
		hQuickSort(pArray, iLeft, lNewPivot - 1);

		// recursively sort elements at least as big as pivot
		hQuickSort(pArray, lNewPivot + 1, iRight);
	}

	return S_OK;
}

int DSSPercentRankRelativeFunction::hQuickSort2(double *pArray, Int32 iLeft, Int32 iRight)
{
	// initialize pivot value and left/right indexes
	double dblPivotValue = pArray[iLeft + (iRight - iLeft) / 2];
	Int32 lNewLeft = iLeft;
	Int32 lNewRight = iRight;

	// adjust new left/right indexes, based on pivot value
	while(lNewLeft <= lNewRight)
	{
		while(pArray[lNewLeft] < dblPivotValue)
			lNewLeft++;
		while(pArray[lNewRight] > dblPivotValue)
			lNewRight--;

		if(lNewLeft <= lNewRight)
		{
			// swap values
			double dblTemp = pArray[lNewRight];
			pArray[lNewRight] = pArray[lNewLeft];
			pArray[lNewLeft] = dblTemp;

			// adjust new indexes
			lNewLeft++;
			lNewRight--;
		}
	}

	// recursively sort elements smaller than pivot
	if(iLeft < lNewRight)
		hQuickSort2(pArray, iLeft, lNewRight);

	// recursively sort elements at least as big as pivot
	if(lNewLeft < iRight)
		hQuickSort2(pArray, lNewLeft, iRight);

	return S_OK;
}

Int32 DSSPercentRankRelativeFunction::hBinarySearch(double *pArray, double dblKey, Int32 iLeft, Int32 iRight)
{
	if(iLeft==iRight)
		return iLeft;

	Int32 lMiddle = iLeft + (iRight - iLeft) / 2;

	if(dblKey <= pArray[lMiddle])
		return hBinarySearch(pArray, dblKey, iLeft, lMiddle);
	else
		return hBinarySearch(pArray, dblKey, lMiddle + 1, iRight);
}

int DSSPercentRankRelativeFunction::SetAsSQLVersion(bool ibSQLVersion)
{
	mIsOneInput = ibSQLVersion;
	return S_OK;
}
