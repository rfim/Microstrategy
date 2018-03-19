//
//  DSSStatisticsSTOS.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 3/7/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#include "DSSStatisticsSTOS.h"

#include "DSSStatistics_include.h"
#include "statistic.h"

#pragma warning(disable:4244)	//conversion from 'double' to 'int', possible loss of data

DSSStatisticsSTOS::DSSStatisticsSTOS()
: mFunctionIndex(0)
, mCumulative(0),mLowerBound(0.0), mUpperBound(1.0)
{
}


DSSStatisticsSTOS::~DSSStatisticsSTOS()
{
}

int DSSStatisticsSTOS::Calculate(Int32 nParam, double *pParam, double *pResult, DSSDataFlag *pFlag)
{
    enum MathError error;
    
	// zyu 07/05/2005
	// The following is a bad design. A table driven method is expected!
	switch (mFunctionIndex)
	{
        case 0: if (nParam != 3)
            return E_FAIL;
            
			*pResult = BETADIST(ClearError(&error), pParam[0], pParam[1], pParam[2], mLowerBound, mUpperBound);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 1:	if (nParam != 3)
            return E_FAIL;
            
			*pResult = BINOMDIST(ClearError(&error), (Int32)pParam[0], (Int32)pParam[1], pParam[2], mCumulative);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 2:	if (nParam != 3)
            return E_FAIL;
            
			*pResult = FDIST(ClearError(&error), pParam[0], (Int32)pParam[1], (Int32)pParam[2]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 3:	if (nParam != 2)
            return E_FAIL;
            
			*pResult = CHIDIST(ClearError(&error), pParam[0], (Int32)pParam[1]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 4:	if (nParam != 1)
            return E_FAIL;
            
			*pResult = NORMSDIST(ClearError(&error), pParam[0]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 5:	if (nParam != 3)
            return E_FAIL;
            
			*pResult = NORMDIST(ClearError(&error), pParam[0], pParam[1], pParam[2], mCumulative);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 6: if (nParam != 2)
            return E_FAIL;
            
			*pResult = EXPONDIST(ClearError(&error), pParam[0], pParam[1], mCumulative);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 7:	if (nParam != 3)
            return E_FAIL;
            
			*pResult = GAMMADIST(ClearError(&error), pParam[0], pParam[1], pParam[2], mCumulative);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 8:	if (nParam != 4)
            return E_FAIL;
            
			*pResult = HYPGEOMDIST(ClearError(&error), (Int32)pParam[0], (Int32)pParam[1], (Int32)pParam[2], (Int32)pParam[3]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 9:	if (nParam != 3)
            return E_FAIL;
            
			*pResult = LOGNORMDIST(ClearError(&error), pParam[0], pParam[1], pParam[2]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 10: if (nParam != 3)
            return E_FAIL;
            
			*pResult = NEGBINOMDIST(ClearError(&error), (Int32)pParam[0], (Int32)pParam[1], pParam[2]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 11: if (nParam != 2)
            return E_FAIL;
            
			*pResult = POISSON(ClearError(&error), (Int32)pParam[0], pParam[1], mCumulative);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 12: if (nParam != 2)
            return E_FAIL;
            
			*pResult = TDIST(ClearError(&error), pParam[0], (Int32)pParam[1]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 13: if (nParam != 3)
            return E_FAIL;
            
			*pResult = WEIBULL(ClearError(&error), pParam[0], pParam[1], pParam[2], mCumulative);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 14: if (nParam != 3)
            return E_FAIL;
            
			*pResult = BETAINVERSE(ClearError(&error), pParam[0], pParam[1], pParam[2], mLowerBound, mUpperBound);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 15: if (nParam != 3)
            return E_FAIL;
            
			*pResult = FINV(ClearError(&error), pParam[0], (Int32)pParam[1], (Int32)pParam[2]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 16: if (nParam != 2)
            return E_FAIL;
            
			*pResult = CHIINV(ClearError(&error), pParam[0], (Int32)pParam[1]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 17: if (nParam != 1)
            return E_FAIL;
            
			*pResult = NORMSINV(ClearError(&error), pParam[0]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 18: if (nParam != 3)
            return E_FAIL;
            
			*pResult = NORMINV(ClearError(&error), pParam[0], pParam[1], pParam[2]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 19: if (nParam != 3)
            return E_FAIL;
            
			*pResult = GAMMAINV(ClearError(&error), pParam[0], pParam[1], pParam[2]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 20: if (nParam != 3)
            return E_FAIL;
            
			*pResult = LOGINV(ClearError(&error), pParam[0], pParam[1], pParam[2]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 21: if (nParam != 2)
            return E_FAIL;
            
			*pResult = TINV(ClearError(&error), pParam[0], (Int32)pParam[1]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 22: if (nParam != 3)
            return E_FAIL;
            
			*pResult = CRITBINOM(ClearError(&error), (Int32)pParam[0], pParam[1], pParam[2]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 23: if (nParam != 1)
            return E_FAIL;
            
			*pResult = Fisher(ClearError(&error), pParam[0]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 24: if (nParam != 1)
            return E_FAIL;
            
			*pResult = FisherInv(ClearError(&error), pParam[0]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 25: if (nParam != 2)
            return E_FAIL;
            
			*pResult = PERMUT(ClearError(&error), (Int32)pParam[1], (Int32)pParam[0]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 26: if (nParam != 3)
            return E_FAIL;
            
			*pResult = Confidence(ClearError(&error), pParam[0], pParam[1], pParam[2]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case 27: if (nParam != 3)
            return E_FAIL;
            
			*pResult = Standardize(ClearError(&error), pParam[0], pParam[1], pParam[2]);
            
			if(error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
            // zyu 06/21/2005 
            // Issue # 99120
            // Additional functions to be used in consolidation. 
        case DssStatFuncLinearTrend_S: 
			if (nParam < 3) 
                return E_FAIL;
			*pResult = LinearTrend_S(ClearError(&error), pParam[nParam - 1], &pParam[0], nParam - 1);
            
			if (error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case DssStatFuncGrowth_S:
			if (nParam < 3)
				return E_FAIL;
			*pResult = Growth_S(ClearError(&error), pParam[nParam - 1], &pParam[0], nParam - 1 );
            
			if (error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        case DssStatFuncForecast_S:
			if (nParam % 2 == 0 || nParam < 5)
				return E_FAIL;
			*pResult = Forecast_S(ClearError(&error), 
                                  pParam[0], 
                                  &pParam[(nParam+1)/2],  // px[]
                                  &pParam[1],				// py[]
                                  (nParam-1)/2 );
			if (error == MathNoError)
				*pFlag = DssDataOk;
			else
				*pFlag = DssDataInvalid;
			break;
            
        default : return E_FAIL;
	}
    
	return S_OK;
}

int DSSStatisticsSTOS::setProperty(Int32 Index, CComVariant *pValue)
{
    HRESULT hr;
    
	switch (Index)
	{
        case 1:
            if (mFunctionIndex == 0 || mFunctionIndex==14)
            { // beta or beta inverse
                hr = VariantChangeType(pValue, pValue, 0, VT_R8);
                if ( FAILED(hr) ) return hr;
                
                if(pValue->vt == VT_R8)
                    mLowerBound = pValue->dblVal;
                else return E_FAIL;
            }
            else
            {
                hr = VariantChangeType(pValue, pValue, 0, VT_I4);
                if ( FAILED(hr) ) return hr;
                
                if(pValue->vt == VT_I4)
                    switch (pValue->lVal)
                {
					case 0:		mCumulative = true;
                        break;
					case 1:		mCumulative = false;
                        break;
					default : return E_FAIL;
                }
                else return E_FAIL;
            }
            break;
        case 2:
            if (mFunctionIndex == 0 || mFunctionIndex==14)
            { // beta or beta inverse
                hr = VariantChangeType(pValue, pValue, 0, VT_R8);
                if ( FAILED(hr) ) return hr;
                
                if(pValue->vt == VT_R8)
                    mUpperBound = pValue->dblVal;
                else return E_FAIL;
            }
            else return E_FAIL;
            break;
        default:
            return E_FAIL;
	}
    
	return S_OK;
}

int DSSStatisticsSTOS::Init(Int32 iFunctionIndex)
{
	mFunctionIndex = iFunctionIndex;
	return S_OK;
}