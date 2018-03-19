//
//  DSSN_Tile.cpp
//  ReportServiceCore
//
//  Created by Zhang Jia on 7/20/11.
//  Copyright 2011 MicroStrategy Beijing R&D Center. All rights reserved.
//

#include "DSSN_Tile.h"

#import "DSSFunctionUtility.h"

// jxie, 2010-12-10
#define DOUBLE_MAX 1.7e308

DSSN_Tile::DSSN_Tile() : mpData(NULL), mnDataSize(0), 
mpResults(NULL), mNBuckets(4), mAsc(true), mStepValue(0)
{
}

DSSN_Tile::~DSSN_Tile()
{
	delete [] mpData;
	mpData = NULL;
	delete [] mpResults;
	mpResults = NULL;
}

//int DSSN_Tile::Init(EnumDssFunc iFunctionIndex)
//{        
//  	mFunctionIndex = iFunctionIndex;
//    
//	return S_OK;
//}

int DSSN_Tile::setProperty (Int32 Index, CComVariant *pValue)
{
    int hr = S_OK;
    
	switch(Index)
	{
        case 1: 
            // 3/22/2012 tehe
            if (mFunctionIndex == DssFuncN_Tile || mFunctionIndex == DssFuncNTile_V)
            //if (mFunctionIndex == DssFuncNTile) // jimzhang July/20/2011 : Is it need to add defination of DssFuncNTileV(DssFuncNTile_V) ?
            {
                hr = VariantChangeType(pValue, pValue, 0, VT_I4);                
                CHECK_HR;
                
                if(pValue->vt == VT_I4)
                {
                    if(pValue->lVal <= 0) 
                        return hr = E_FAIL;
                    
                    mNBuckets = pValue->lVal;
                    return hr = S_OK;
                }
                else
                    return hr = E_FAIL;
                break;
            } //if it is DssFuncNTile_S or DssFuncNTile_VS, function only has one function property Ascending or Descending            
        case 2:
            hr = VariantChangeType(pValue, pValue, 0, VT_BOOL);
            CHECK_HR;
            
            if(pValue->vt == VT_BOOL)
            {
                mAsc = pValue->boolVal ? true : false;
                return hr = S_OK;
            }
            else 
                return hr = E_FAIL;
            
            break;
            
        default: 
			AE_ASSERT(FALSE);
	}
    
	// dummy return
	return E_FAIL;
}


int DSSN_Tile::Calculate()
{
	int hr = S_OK;
    
	CHECK_PTR(mpData);
    
	if (mnDataSize == 0) 
		return hr = S_OK;
    
	mpResults = new double[mnDataSize];
	CHECK_NEWED_PTR(mpResults);
	
	Int32 i = 0;
    
	switch (mFunctionIndex)
    {
        case DssFuncN_Tile:
		{
			if (mNBuckets < 1)
				return hr = E_INVALIDARG;
            
			Rank(mnDataSize, mpData, mpResults, mAsc);
            
			for (i = 0; i < mnDataSize; i ++)
			{
				Int32 bucket = 0;
				bucket = (Int32)( (mpResults[i] * mNBuckets) / mnDataSize );
				if ( mpResults[i] * mNBuckets > bucket * mnDataSize )
					mpResults[i] = bucket + 1;
				else mpResults[i] = bucket;
			}
		}
            break;
            
            // jimzhang July/20/2011 : Is it need to add defination of DssFuncNTileV(DssFuncNTile_V) ?
   
        case DssFuncNTile_V:
        {
             if (mNBuckets < 1)
             return hr = E_INVALIDARG;
             
             double maxValue = mpData[0],
             minValue = mpData[0];
             
             for (i = 1; i < mnDataSize; i++)
             {
                 if (maxValue < mpData[i])
                        maxValue = mpData[i];
                 if (minValue > mpData[i])
                        minValue = mpData[i];
             }
             
             //calculate range of each segment
             double segRange = (maxValue - minValue) / mNBuckets;
             
             if (segRange > 0)
             {
                 for (i = 0; i < mnDataSize; i ++)
                 {
                    mpResults[i] = (Int32)((mpData[i] - minValue) / segRange + 1);
                    if (mpResults[i] > mNBuckets) //this is the maximum value
                        mpResults[i] = mNBuckets;
             
                     if (!mAsc) // if it is descending, reverse the ordering
                         mpResults[i] = mNBuckets - mpResults[i] + 1;
                }
             } else
             {
                 //if maxValue is same as minValue, then all values are in the same segment
                 for (i = 0; i < mnDataSize; i ++)
                     mpResults[i] = 1;
             }
        }
             
        break;

        case DssFuncNTile_S:
        {
            if (mStepValue < 1)
                return hr = E_INVALIDARG;
            
            Rank(mnDataSize, mpData, mpResults, mAsc);
            
            for (i = 0; i < mnDataSize; i ++)
                mpResults[i] = ceil(mpResults[i] / floor(mStepValue) ); 
        }
            
            break;
            
        case DssFuncNTile_VS:
		{            
			if (mStepValue < 0) 
				return hr = E_INVALIDARG;
            
			// find minimum and maximum input value
			double minValue = mpData[0];
			double maxValue = mpData[0];
			for (i = 1; i < mnDataSize; i++)
            {
				if (maxValue < mpData[i])
					maxValue = mpData[i];
				if (minValue > mpData[i])
					minValue = mpData[i];
			}
            
			if (mAsc)
            {
				for (i = 0; i < mnDataSize; i ++)
					mpResults[i] = floor( (mpData[i] - minValue) / mStepValue) + 1; 
			} 
            else 
            {
				for (i = 0; i < mnDataSize; i ++)
					mpResults[i] = floor( (maxValue - mpData[i]) / mStepValue) + 1; 
			}
            
		}
            break;
            
        default:
            AE_ASSERT(false);
            return hr = false;
	} //switch
    
	return hr = S_OK;
}

int DSSN_Tile::GetOutput (Int32 nSize, Int32 *nDataSize, double *pData, DSSDataFlag *pFlag)
{
	int hr = S_OK;
    
	CHECK_PTR(nDataSize);
	CHECK_PTR(pData);
	CHECK_PTR(pFlag);
    
	if (nSize < mnDataSize) 
		return hr = E_FAIL;
    
	Int32 i = 0;
	for(i = 0; i< mnDataSize; i++)
	{
		pData[i] = mpResults[i];
		pFlag[i] = DssDataOk;
	}
    
	*nDataSize = mnDataSize;
    
	return hr = S_OK;
}

int DSSN_Tile::GetOutput(Int32, Int32 *, VARIANT *, DSSDataFlag *)
{
    return E_NOTIMPL;
}

int DSSN_Tile::GetOutputType (EnumDSSParameterType * pType)
{
	int hr = S_OK;
    
	CHECK_PTR(pType);
    
	*pType = DssParameterVector;
	
	return hr = S_OK;
}

int DSSN_Tile::GetOutputType(Int32, EnumDSSDataType *, EnumDSSDataType *, EnumDSSParameterType *)
{
    return E_NOTIMPL;
}

int DSSN_Tile::Clear ()
{
	int hr = S_OK;
    
	delete [] mpData;
	mpData = NULL;
    
	delete [] mpResults;
	mpResults = NULL;
    
	mnDataSize = 0;
	mStepValue = 0;
    
	return hr = S_OK;
}

int DSSN_Tile::SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSDataFlag * /*pFlag*/)
{
	int hr = S_OK;
    
 	if((nParamIndex != 1 && nParamIndex != 2) ) 
		return hr = E_FAIL;
    
	if(nSize < 1) 
		return hr = E_INVALIDARG;
	
	CHECK_PTR(pData);
    
	switch(nParamIndex)	
    {            
        case 1: //values vector
        {
            if(mpData == NULL)	
            {
                mnDataSize = nSize;
                
                // allocat memory to record input data
                mpData = new double[nSize];
                CHECK_NEWED_PTR(mpData);
                // if(!mpData) return E_OUTOFMEMORY;
                
                Int32 i = 0;
                for(i = 0; i < nSize; i++)
                    mpData[i] = pData[i];
            } 
            else 
            {
                AE_ASSERT(false);
            }
        }            
            break;
            
        case 2: //step size for Ntile_S or NTile_SV
        {
            mStepValue = *pData;
        }
            break;
            
        default:
            AE_ASSERT(false);            
	}
    
    
	return hr = S_OK;
}

int DSSN_Tile::SetInputParam(Int32, Int32, EnumDSSDataType,CComVariant *, DSSDataFlag *)
{
    return E_NOTIMPL;
}

int DSSN_Tile::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
	int hr = S_OK;
    
	CHECK_PTR(pType);
    
	switch (nParamIndex) 
    {
        case 1:
            *pType = DssParameterVector;
            break;
        case 2:
            AE_ASSERT(mFunctionIndex == DssFuncNTile_S || mFunctionIndex == DssFuncNTile_VS);
            *pType = DssParameterScalar;
            break;
        default:
            return hr = E_FAIL;
	}
    
	return hr = S_OK;
}

int DSSN_Tile::IsNullProcessable(bool *pCanProcessNULL)
{
	*pCanProcessNULL = false; // always assume input data is valid
    
	return S_OK;    
}

// jxie, 2010-10-11
// support direct calculate
int DSSN_Tile::DirectCalculate (Int32 nParam, Int32* ipSize,  Int32** ipOffsets, 
                                void** ipData, double* ipDivider, DSSDataFlag** ipFlags, VARIANT_BOOL* ipAllDataValid,
                                EnumDSSDataType* iType, vector<Int32> * ipvDataByGroup,
                                Int32 nResSize, double* opData, DSSDataFlag* opFlag, VARIANT_BOOL* opAllDataValid, 
                                EnumNullCheckingOption iNullCheckingOption)
{
	int hr = S_OK;
    
	if (nParam < 1 || nParam > 2)	return hr = E_FAIL;
    
	if (ipSize[0] < 1)	return hr = E_INVALIDARG;
	
	CHECK_PTR(ipData);
    
	// the 1st parameter: values vector
	mnDataSize = ipSize[0];
	if (ipvDataByGroup != NULL)
		mnDataSize = ipvDataByGroup->size();
    
	// the 2nd parameter: step size for Ntile_S or NTile_SV
	if (nParam == 2)
    {
        Int32 lRow = 0; //GetRow(ipOffsets[1], 0);
		mStepValue = GetNumericData(ipData[1], lRow, iType[1]);
    }
    
	if (mnDataSize == 0) 
		return hr = S_OK;
	
	Int32 i = 0;
	double lData = 0;
	*opAllDataValid = (ipAllDataValid[0] == VARIANT_TRUE) ? VARIANT_TRUE : VARIANT_FALSE;
	if (iNullCheckingOption == DssNumericErrorTreatAs)
		*opAllDataValid = VARIANT_TRUE;
    
	DSSDataFlag* lFlags = ipFlags[0];
	void* lpData = ipData[0];
	EnumDSSDataType lType = iType[0];
    
	Int32 lRow = 0;
	switch (mFunctionIndex)
    {
        case DssFuncN_Tile:
		{
			if (mNBuckets < 1)
				return hr = E_INVALIDARG;
            
			//prepare inputs
			Int32 lnNonNullSize = 0;
			for(i = 0; i < mnDataSize; i++) 
			{
				if (ipvDataByGroup != NULL)
					lRow = (*ipvDataByGroup)[i];
				else
					lRow = i;
                
				if (lFlags[lRow] != DssDataOk) 
				{
					if (iNullCheckingOption == DssNumericErrorTreatAs)
					{ 
						opFlag[lRow] = DssDataOk;
						lnNonNullSize++;
					}
					else
					{
						opFlag[lRow] = DssDataInvalid;
						*opAllDataValid = VARIANT_FALSE;
					}
				}
				else
				{
					lnNonNullSize++;
					opFlag[lRow] = DssDataOk;
				}
			}
            
			if (lnNonNullSize == 0)
				return hr = S_OK;
            
			// prepare lRank
			Int32* lRank = new Int32[lnNonNullSize];
			Int32 lCount = 0;
			for(i = 0; i < mnDataSize; i++) 
			{
				if (ipvDataByGroup != NULL)
					lRow = (*ipvDataByGroup)[i];
				else
					lRow = i;
                
				if (opFlag[lRow] == DssDataOk) 
					lRank[lCount++] = lRow;
			}
            
			RankForRows(lnNonNullSize, lpData, lFlags, lType, opData, mAsc, lRank);
			delete [] lRank;
            
			for (i = 0; i < mnDataSize; i ++)
			{
				if (ipvDataByGroup != NULL)
					lRow = (*ipvDataByGroup)[i];
				else
					lRow = i;
                
				if (opFlag[lRow] != DssDataOk)
					continue;
                
				Int32 bucket = 0;
				bucket = (Int32)( (opData[lRow] * mNBuckets) / (double)lnNonNullSize );
				if ( opData[lRow] * mNBuckets > bucket * (double)lnNonNullSize )
					opData[lRow] = bucket + 1;
				else opData[lRow] = bucket;
			}
		}
            break;
            
            // jimzhang July/20/2011 : need to define DssFuncNTileV ?
            
        case DssFuncNTile_V:
        {
             if (mNBuckets < 1)
                 return hr = E_INVALIDARG;
             
             double maxValue = -DOUBLE_MAX;
             double minValue = DOUBLE_MAX;
             
             for (i = 0; i < mnDataSize; i++)
             {
                 if (ipvDataByGroup != NULL)
                     lRow = (*ipvDataByGroup)[i];
                 else
                     lRow = i;
             
                 if (lFlags[lRow] != DssDataOk)
                 {
                     if (iNullCheckingOption == DssNumericErrorTreatAs)
                         lData = 0;
                     else
                         continue;
                 }
                 else
                     lData = GetNumericData(lpData, lRow, lType);
             
                 if (maxValue < lData)
                     maxValue = lData;
                 if (minValue > lData)
                     minValue = lData;
             }
             
             //calculate range of each segment
             double segRange = (maxValue - minValue) / mNBuckets;
             
             if (segRange > 0)
             {
                 for (i = 0; i < mnDataSize; i ++)
                 {
                     if (ipvDataByGroup != NULL)
                         lRow = (*ipvDataByGroup)[i];
                     else
                         lRow = i;
             
                     lData = GetNumericData(lpData, lRow, lType);
                     if (lFlags[lRow] != DssDataOk)
                     {
                         if (iNullCheckingOption == DssNumericErrorTreatAs)
                             lData = 0;
                         else
                         {
                             opFlag[lRow] = lFlags[lRow];
                             *opAllDataValid = VARIANT_FALSE;
                             continue;
                         }
                     }
             
                     opData[lRow] = (Int32)((lData - minValue) / segRange + 1);
                     if (opData[lRow] > mNBuckets) //this is the maximum value
                         opData[lRow] = mNBuckets;
             
                     if (!mAsc) // if it is descending, reverse the ordering
                         opData[lRow] = mNBuckets - opData[lRow] + 1;
             
                     opFlag[lRow] = DssDataOk;
                 }
             }
             else
             {
             //if maxValue is same as minValue, then all values are in the same segment
                 for (i = 0; i < mnDataSize; i ++)
                 {
                     if (ipvDataByGroup != NULL)
                         lRow = (*ipvDataByGroup)[i];
                     else
                         lRow = i;
             
                     if (lFlags[lRow] != DssDataOk)
                     {
                         if (iNullCheckingOption != DssNumericErrorTreatAs)
                         {
                             opFlag[lRow] = lFlags[lRow];
                             *opAllDataValid = VARIANT_FALSE;
                             continue;
                         }
                     }
             
                     opData[lRow] = 1;
                     opFlag[lRow] = DssDataOk;
                 }
             }
        }
             
        break;
            
        case DssFuncNTile_S:
		{
			if (mStepValue < 1)
				return hr = E_INVALIDARG;
            
			//prepare inputs
			Int32 lnNonNullSize = 0;
			Int32 lCount = 0;
			for(i = 0; i < mnDataSize; i++) 
			{
				if (ipvDataByGroup != NULL)
					lRow = (*ipvDataByGroup)[i];
				else
					lRow = i;
                
				if (lFlags[lRow] != DssDataOk) 
				{
					// it may be changed later
					if (iNullCheckingOption == DssNumericErrorTreatAs)
					{
						lnNonNullSize++;
						opFlag[lRow] = DssDataOk;
					}
					else
					{
						opFlag[lRow] = DssDataInvalid;
						*opAllDataValid = VARIANT_FALSE;
					}
				}
				else
				{
					lnNonNullSize++;
					opFlag[lRow] = DssDataOk;
				}
			}
            
			if (lnNonNullSize == 0)
				return hr = S_OK;
            
			// prepare lRank
			Int32* lRank = new Int32[lnNonNullSize];
			lCount = 0;
			for(i = 0; i < mnDataSize; i++) 
			{
				if (ipvDataByGroup != NULL)
					lRow = (*ipvDataByGroup)[i];
				else
					lRow = i;
                
				if (opFlag[lRow] == DssDataOk) 
					lRank[lCount++] = lRow;
			}
            
			RankForRows(lnNonNullSize, lpData, lFlags, lType, opData, mAsc, lRank);
			delete [] lRank;
            
			for (i = 0; i < mnDataSize; i ++)
			{
				if (ipvDataByGroup != NULL)
					lRow = (*ipvDataByGroup)[i];
				else
					lRow = i;
                
				if (opFlag[lRow] != DssDataOk)
					continue;
                
				opData[lRow] = ceil(opData[lRow] / floor(mStepValue) ); 
			}
		}
            
            break;
            
        case DssFuncNTile_VS:
		{
			if (mStepValue < 0) 
				return hr = E_INVALIDARG;
            
			// find minimum and maximum input value
			double maxValue = -DOUBLE_MAX;
			double minValue = DOUBLE_MAX;
            
			for (i = 0; i < mnDataSize; i++)
			{
				if (ipvDataByGroup != NULL)
					lRow = (*ipvDataByGroup)[i];
				else
					lRow = i;
                
				lData = GetNumericData(lpData, lRow, lType);
				if (lFlags[lRow] != DssDataOk)
				{
					if (iNullCheckingOption == DssNumericErrorTreatAs)
						lData = 0;
					else
						continue;
				}
                
				if (maxValue < lData)
					maxValue = lData;
				if (minValue > lData)
					minValue = lData;
			}
            
			if (mAsc)
            {
				for (i = 0; i < mnDataSize; i ++)
				{
					if (ipvDataByGroup != NULL)
						lRow = (*ipvDataByGroup)[i];
					else
						lRow = i;
                    
					lData = GetNumericData(lpData, lRow, lType);
					if (lFlags[lRow] != DssDataOk)
					{
						if (iNullCheckingOption == DssNumericErrorTreatAs)
							lData = 0;
						else
						{
							opFlag[lRow] = lFlags[lRow];
							*opAllDataValid = VARIANT_FALSE;
							continue;
						}
					}
                    
					opData[lRow] = floor( (lData - minValue) / mStepValue) + 1; 
					opFlag[lRow] = DssDataOk;
				}
			} 
            else 
            {
				for (i = 0; i < mnDataSize; i ++)
				{
					if (ipvDataByGroup != NULL)
						lRow = (*ipvDataByGroup)[i];
					else
						lRow = i;
                    
					if (lFlags[lRow] != DssDataOk)
					{
						if (iNullCheckingOption == DssNumericErrorTreatAs)
							lData = 0;
						else
						{
							opFlag[lRow] = lFlags[lRow];
							*opAllDataValid = VARIANT_FALSE;
							continue;
						}
					}
					else
						lData = GetNumericData(lpData, lRow, lType);
                    
					opData[lRow] = floor( (maxValue - lData) / mStepValue) + 1; 
					opFlag[lRow] = DssDataOk;
				}
			}
		}
            break;
            
        default:
            AE_ASSERT(false);
            return hr = false;
	} //switch
    
	return hr = S_OK;
}

// jxie, 2010-10-11
// support direct calculate on all functions in this class 
bool DSSN_Tile::SupportDirectCalculate()
{
	return true;
}