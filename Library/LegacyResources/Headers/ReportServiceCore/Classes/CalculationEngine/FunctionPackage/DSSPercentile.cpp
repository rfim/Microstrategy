//
//  DSSPercentile.cpp
//  ReportServiceCore
//
//  Created by Jia Zhang on 7/27/11.
//  Copyright 2011 MicroStrategy, Inc. All rights reserved.
//

#include "DSSPercentile.h"

#import "DSSFunctionUtility.h"
#import "DSSStrongPtr.h"

DSSPercentile::DSSPercentile() : mAsc(true)
{
}

DSSPercentile::~DSSPercentile()
{
}

int DSSPercentile::setProperty (Int32 Index, CComVariant *pValue)
{
    int hr = S_OK;
    
	switch(Index)
	{
        case 1: 
            hr = VariantChangeType(pValue, pValue, 0, VT_BOOL);
            CHECK_HR;
            
            if(pValue->vt == VT_BOOL)
            {
                mAsc = pValue->boolVal ? true : false;
                return hr = S_OK;
            }
            else 
                return hr = E_FAIL;
        default: 
			return hr = E_FAIL;
	}
}

int DSSPercentile::Calculate ()
{
    return S_OK;
}

int DSSPercentile::GetOutput (Int32 nSize, Int32 *nDataSize, double *pData,  DSSDataFlag *pFlag)
{
    int hr = S_OK;
    
	if (nSize < 1) 
		return hr = E_FAIL;
    
	CHECK_PTR(pData);
	CHECK_PTR(pFlag);
	CHECK_PTR(nDataSize);
    
	CHECK_PTR(mV);
    
	*nDataSize = 1;
    
	if (DoubleLessThan(mS, 0.0) || DoubleGreaterThan(mS, 1.0) )
	{
		*pFlag = DssDataInvalid;
		return hr = S_OK;
	}
    
	if (mVSize == 1)
	{   //special case
		*pData = *mV;
		*pFlag = DssDataOk;
		return hr = S_OK;
	}    
    
	Int32 *lRank = new Int32[mVSize];    
	CHECK_NEWED_PTR(lRank);
    MBase::DSSStrongPtr<Int32, MBase::DeleteArray<Int32> > lRankPtr(lRank);
    
	Int32 i = 0;
	for(i = 0; i < mVSize; i++)
		lRank[i] = i;
    
	RankPred lPred(mV, mAsc);
	sort(lRank, lRank+mVSize, lPred);
	
	//need to search and set the tie to the same rank
	//later we can decide if this processing should only be done as necessary
	double Prev, Curr;
	double PrevRank, CurrRank;
    
	Curr = mV[lRank[0]];
	CurrRank = 0.0;	//rank should start from 0    
    
	for(i = 1; i < mVSize; i++)
	{
		Prev = Curr;
		Curr = mV[lRank[i]];
		PrevRank = CurrRank;
        
		CurrRank = (double)(i) / (double)(mVSize - 1);        
        
		if (DoubleGreaterEqual(CurrRank, mS) )
		{
			double ratio = (CurrRank - mS) / (CurrRank - PrevRank);
			*pData = ratio * Prev + (1.0 - ratio) * Curr;
			*pFlag = DssDataOk;			
			return hr = S_OK;
		}        
	}    
    
	return hr = E_FAIL;    
}

int DSSPercentile::IsNullProcessable (bool *pCanProcessNULL)
{
	CHECK_PTR(pCanProcessNULL);
        
	*pCanProcessNULL = false;
	
    return S_OK;
}