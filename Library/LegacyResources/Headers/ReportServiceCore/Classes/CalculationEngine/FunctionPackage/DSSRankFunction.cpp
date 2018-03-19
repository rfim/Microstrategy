
//
//  DSSRankFunction.cpp
//  ReportServiceCore
//
//  Created by Zhang Jia on 7/20/11.
//  Copyright 2011 MicroStrategy Beijing R&D Center. All rights reserved.
//

#include "DSSRankFunction.h"

#import "DSSFunctionUtility.h"
#import "DSSSort_includes.h"
#import "DFCDataConversion.h"
#import "DSSStrongPtr.h"

// jxie, 2011-1-12, TQMS 450452
class DateSort
{
public:
	DateSort( MDataType::DSSDateTime *dates, bool asc): mdates(dates), mAsc(asc) {}
	Int32 operator () (Int32 i, Int32 j)
	{
		if (mAsc) 	return mdates[i] < mdates[j];
		else return mdates[i] > mdates[j];
	}
    
private:
	MDataType::DSSDateTime *mdates;
	bool mAsc;
};

// jxie, 2011-1-12, TQMS 450452
class VariantSort
{
public:
	VariantSort( CComVariant *vars, bool asc ): mpVars(vars), mAsc(asc) {}
	Int32 operator () (Int32 i, Int32 j)
	{
		DSSDataFlag lFlag = DssDataOk;
		CComVariant lVar1, lVar2;
		getVariant(mpVars, i, lVar1);
		getVariant(mpVars, j, lVar2);
		if (mAsc)	return variantCompare(lVar1, lVar2, &lFlag) < 0;
		else	return variantCompare(lVar1, lVar2, &lFlag) > 0;
	}
    
private:
	CComVariant *mpVars;
	bool mAsc;
};

DSSRankFunction::DSSRankFunction(): mByValue(true), mAsc(true),
mpValues(NULL), mnDataSize(0), mpResults(NULL), mpFlags(NULL), 
mpDates(NULL), mnNoNullSize(0), mNullInclude(0)
{
}

DSSRankFunction::~DSSRankFunction()
{
 	delete [] mpValues;
	delete [] mpResults;
	delete [] mpFlags;
	delete [] mpDates;
    
	// jxie, 2011-1-13, TQMS 450452
	mpVars.Clear();
}

int DSSRankFunction::Calculate ()
{
    int hr = S_OK;
    
	Int32 i = 0;
    
	//prepare inputs
	if (mpValues) 
    { 
        // numerical rank
        Rank(mnNoNullSize, mpValues, mpResults, mAsc);
	} 
    else if (mpDates) 
    { 
        // rank dates        
		rankDate(mnNoNullSize, mpDates, mpResults, mAsc);
	} 
    else if (mpVars.vt != VT_EMPTY) 
    { 
        // rank VARIANT data, jxie, 2011-1-12, TQMS 450452        
		hRankVariant(mnNoNullSize, &mpVars, mpResults, mAsc);        
	} 
    else
    {
		AE_ASSERT(false);
    }
    
	//ctang : if Null in first, add the rank
	if (mNullInclude == First && mnNoNullSize != mnDataSize)
	{
		for (i = 0; i < mnNoNullSize; i++)
			//TQMS 443728 ctang 2010-12-7
			mpResults[i] += mnDataSize - mnNoNullSize;
	}
    
	// calculate percentage
	if(!mByValue) 
    {
		for(i = 0; i < mnNoNullSize; i++)
			mpResults[i] = mpResults[i] / (double)mnDataSize;
	}
    
	//strech the result to the right place
	Int32 nSize = mnNoNullSize;
	for(i = mnDataSize; i >  0 ; i --) 
    {
		if (mpFlags[i - 1] == DssDataOk) 
        {
			mpResults[i - 1] = mpResults[nSize - 1];
			nSize --;
		}
		else if (mNullInclude != Ignore && /*TQMS 444299 ctang 2010-12-8*/mpFlags[i - 1] != DssDataOk)
		{
			Int32 lNullRank = mNullInclude == First ? 1 :  mnNoNullSize + 1;
			if (mByValue)
				mpResults[i - 1] = lNullRank;
			else
				mpResults[i - 1] = (double)(lNullRank) / (double)mnDataSize;
		}
	}
    
	AE_ASSERT(nSize == 0);
    
	return hr = S_OK;
}

int DSSRankFunction::rankDate(Int32 nSize, MDataType::DSSDateTime *pDate, double *pResult, bool fAsc)
{
    int hr = S_OK;
    
	if (nSize <= 0)
		return hr = S_OK;
    
	DateSort lDates(pDate, fAsc);
    
	Int32 *pVec = new Int32[nSize];
	CHECK_NEWED_PTR(pVec);
    MBase::DSSStrongPtr<Int32, MBase::DeleteArray<Int32> > vecPtr(pVec);
    
	Int32 i = 0;
	for (i = 0; i < nSize; i ++)
		pVec[i] = i;
    
	_xtab_sort(pVec, pVec + nSize, lDates);
    
	//need to search and set the tie to the same rank
	MDataType::DSSDateTime *pPrev;
	Int32 prevRank = 0;
    
	prevRank = 1;
	pPrev = &pDate[pVec[0]];
	pResult[pVec[0]] = 1;
    
	for(i = 1; i < nSize; i++)	
    {
		if (pDate[pVec[i]] == *pPrev) 
        {   // same rank
			pResult[pVec[i]] = prevRank;
        }
		else 
        {   // new rank
			prevRank = i  +  1;
			pPrev = &pDate[pVec[i]];
			pResult[pVec[i]] = prevRank;
		}
	}
        
	return hr = S_OK;    
}

int DSSRankFunction::GetOutput (Int32 nSize, Int32 *nDataSize, CComVariant *pData, DSSDataFlag *pFlag)
{
	int hr = S_OK;
    
	CHECK_PTR(nDataSize);
	CHECK_PTR(pData);
	CHECK_PTR(pFlag);
    
	if (nSize < mnDataSize)
		return hr = E_FAIL;
    
	LONG i;
    
	pData->vt = VT_ARRAY|VT_VARIANT;
	SAFEARRAYBOUND lSABound;
	lSABound.lLbound = 0;
	lSABound.cElements = mnDataSize;
	pData->parray = SafeArrayCreate(VT_VARIANT, 1, &lSABound);
    
	for(i = 0; i< mnDataSize; i++)
	{
		CComVariant element;
        
		if (mpFlags[i] == DssDataOk || (mNullInclude != Ignore && /*TQMS 444299 ctang 2010-12-8*/mpFlags[i] != DssDataOk))
			DFCDataConversion::ConvertToVariant((unsigned char *)(&(mpResults[i])), DssDataTypeDouble, 0, &element);
        
		SafeArrayPutElement(pData->parray, &i, &element);
        
		if (mNullInclude != Ignore && /*TQMS 444299 ctang 2010-12-8*/mpFlags[i] != DssDataOk)
			pFlag[i] = DssDataOk;
		else
			pFlag[i] = mpFlags[i];
    }
    
	*nDataSize = mnDataSize;
    
	return hr = S_OK;
}

int DSSRankFunction::setProperty (Int32 Index, CComVariant *pValue)
{
    int hr = S_OK;
    
	switch(Index)
	{
        case 1: return SetPropertyAsc(pValue);
        case 2: return SetPropertyByValue(pValue);
        case 3: return SetPropertyNullInclude(pValue);
        default:
            return hr = E_FAIL;
	}
}

//ctang 2010-9-2
int DSSRankFunction::SetPropertyNullInclude(VARIANT *pValue)
{
	int hr = S_OK;
    
	CHECK_PTR(pValue);
    
	hr = VariantChangeType(pValue, pValue, 0, VT_I4);
	CHECK_HR;
    
	if(pValue->vt == VT_I4)        
		mNullInclude = pValue->intVal > 0 ? Last : (pValue->intVal == 0 ? Ignore : First);
	else
        return S_FALSE;
    
	return hr = S_OK;
}

int DSSRankFunction::SetPropertyByValue (CComVariant *pValue)
{
	int hr = S_OK;
    
	CHECK_PTR(pValue);
    
	hr = VariantChangeType(pValue, pValue, 0, VT_BOOL);
	CHECK_HR;
    
	if(pValue->vt == VT_BOOL)
        mByValue = pValue->boolVal ? true : false;
	else    
        return S_FALSE;
    
	return hr = S_OK;
}

int DSSRankFunction::SetPropertyAsc (VARIANT *pValue)
{
    int hr = S_OK;
	
	CHECK_PTR(pValue);
	
    // change the passed in variant to variant boolean type
	hr = VariantChangeType(pValue, pValue, 0, VT_BOOL);
	CHECK_HR;
    
	if(pValue->vt == VT_BOOL)
		mAsc = pValue->boolVal ? true : false;
	else
		return E_FAIL;
    
	return hr = S_OK;
}

int DSSRankFunction::GetOutputType(Int32 nSize,
                                   EnumDSSDataType * pInputType,
                                   EnumDSSDataType * pOutputDataType,
                                   EnumDSSParameterType *  pOutParamType)
{
    int hr = S_OK;
    
	CHECK_PTR(pOutputDataType);
	CHECK_PTR(pOutParamType);
    
	if (nSize != 1)
		return hr = E_FAIL;
    
	*pOutputDataType = DssDataTypeDouble;
	*pOutParamType = DssParameterVector;
    
	return hr = S_OK;
}

int DSSRankFunction::get_FunctionType (EnumDSSFunctionType *  pType)
{
    int hr = S_OK;
    
	CHECK_PTR(pType);
	*pType = DssFunctionTypeGeneric;
    
	return hr = S_OK;
}

int DSSRankFunction::Clear ()
{
	delete [] mpValues;
	mpValues = NULL;
    
	mnDataSize = 0;
    
	delete [] mpResults;
	mpResults = NULL;
    
	delete [] mpFlags;
	mpFlags = NULL;
    
	delete [] mpDates;
	mpDates = NULL;
    
    // jimzhang Aug/19/2011
    mpVars.Clear();
    
	return S_OK;
}

int DSSRankFunction::SetInputParam (Int32 nParamIndex, Int32 nSize,EnumDSSDataType dataType, 
                                    CComVariant *pData, DSSDataFlag * pFlags)
{
 	int hr = S_OK;
    
	if((nParamIndex != 1) )
		return E_FAIL;
    
	if(nSize < 1)
		return E_FAIL;
    
	if(!pData	|| ( (pData->vt & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT)) )
		return E_FAIL;
    
	mpFlags = new DSSDataFlag[nSize];
	CHECK_NEWED_PTR(mpFlags);
	mpResults = new double[nSize];
	CHECK_NEWED_PTR(mpResults);
    
	LONG i;
    
	switch (dataType){
        case DssDataTypeShort:
        case DssDataTypeInteger:
        case DssDataTypeUnsigned:
        case DssDataTypeLong:
        case DssDataTypeFloat:
        case DssDataTypeReal:
        case DssDataTypeDouble:
        case DssDataTypeNumeric:
        case DssDataTypeDecimal:
        case DssDataTypeBool:
		{ //treat the value as double
			if(mpValues == NULL)	
            {
				mnDataSize = nSize;
                
				// allocat memory to record input data
				mpValues = new double[nSize];
				if(!mpValues) return E_OUTOFMEMORY;
                
				Int32 nDoubleSize = sizeof(double);
				mnNoNullSize = 0;
				for(i = 0; i < nSize; i++) 
                {
					mpFlags[i] = pFlags[i];
					if (pFlags[i] == DssDataOk) 
                    {
						CComVariant element;
                        
						SafeArrayGetElement(pData->parray, &i, &element);
						hr = DFCDataConversion::ConvertFromVariant(&element, DssDataTypeDouble,
                                                                   (BYTE *)&(mpValues[mnNoNullSize ++]), &nDoubleSize);
						if(FAILED(hr)) return E_FAIL;                        
					}
				}
			} 
            else 
            {
				AE_ASSERT(false);
			}
            
		}            
            break;
            
        case DssDataTypeDate:
        case DssDataTypeTime:
        case DssDataTypeTimeStamp:
		{
			if(mpDates == NULL)	
            {
				mnDataSize = nSize;
                
				// allocat memory to record input data
				mpDates = new MDataType::DSSDateTime[nSize];
				if(!mpDates) return E_OUTOFMEMORY;
                
				Int32 nDateSize = sizeof(MDataType::DSSDateTime);
                
				mnNoNullSize = 0;
				for(i = 0; i < nSize; i++) 
                {
					mpFlags[i] = pFlags[i];
					if (pFlags[i] == DssDataOk) 
                    {
						CComVariant element;
                        
						SafeArrayGetElement(pData->parray, &i, &element);
						hr = DFCDataConversion::ConvertFromVariant(&element, DssDataTypeDate,
                                                                   (BYTE *)&(mpDates[mnNoNullSize ++]), &nDateSize);
						if(FAILED(hr)) return E_FAIL;
                        
					}
				}
			} 
            else 
            {
				AE_ASSERT(false);
			}
		}
            break;
            
            // jxie, 2011-1-12, TQMS 450452
            // Support Rank for VARIANT input
        case DssDataTypeVarChar:
        case DssDataTypeNVarChar:
        case DssDataTypeMBChar:
        case DssDataTypeUTF8Char:
		{
			if(mpVars.vt == VT_EMPTY)	
            {
				mnDataSize = nSize;
                
				mnNoNullSize = 0;
				for(i = 0; i < nSize; i++)
				{
					mpFlags[i] = pFlags[i];
					if (pFlags[i] == DssDataOk)
						mnNoNullSize++;
				}
                
				mpVars.vt = VT_ARRAY|VT_VARIANT;
				SAFEARRAYBOUND lSABound;
				lSABound.lLbound = 0;
				lSABound.cElements = mnNoNullSize;
				mpVars.parray = SafeArrayCreate(VT_VARIANT, 1, &lSABound);
                
				Int32 lCount = 0;
				for(i = 0; i < nSize; i++)
				{
					if (pFlags[i] == DssDataOk)
					{
						CComVariant element;
						getVariant(pData, i, element);
						setVariant(&mpVars, lCount++, element);
					}
				}
			} 
            else 
            {
				AE_ASSERT(false);
			}
		}
            break;
            
        default:
            return hr = E_FAIL;;
	}  //switch    
    
	return hr = S_OK;
}

int DSSRankFunction::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
 	int hr = S_OK;
    
	CHECK_PTR(pType);
    
	switch (nParamIndex) 
    {
        case 1:
            *pType = DssParameterVector;
            break;
        default:
            return hr = E_FAIL;
	}
    
	return hr = S_OK;
}

int DSSRankFunction::IsNullProcessable(bool *pCanProcessNULL)
{
    int hr = S_OK;
    
	CHECK_PTR(pCanProcessNULL);
	if (mNullInclude == Ignore)
		*pCanProcessNULL = false;
	else
		*pCanProcessNULL = true; // always assume input data is valid
    
	return hr = S_OK;    
}

int DSSRankFunction::SetInputParam(int nParamIndex, int nSize, double *pData, DSSDataFlag *pFlag)
{    
	if((nParamIndex != 1) )
		return E_FAIL;
    
	if(nSize < 1)
		return E_FAIL;
    
	if(!pData)
		return E_FAIL;
    
	AE_ASSERT(!mpFlags && !mpResults);
    
	mpFlags = new DSSDataFlag[nSize];
	CHECK_NEWED_PTR(mpFlags);
	mpResults = new double[nSize];
	CHECK_NEWED_PTR(mpResults);
    
	LONG i;
    
	if(mpValues == NULL)	
	{
		mnDataSize = nSize;
		// allocat memory to record input data
		mpValues = new double[nSize];
		if(!mpValues) return E_OUTOFMEMORY;
        
		mnNoNullSize = 0;
		for(i = 0; i < nSize; i++) 
		{
			mpFlags[i] = pFlag[i];
			if (pFlag[i] == DssDataOk) 
			{
				mpValues[mnNoNullSize++] = pData[i];
			}
		}
	}
	else 
	{
		AE_ASSERT(false);
	}
	
	return S_OK;
}

int DSSRankFunction::GetOutput(int nSize, int *nDataSize, double *pData, DSSDataFlag *pFlag)
{
    int hr = S_OK;
    
	if(!nDataSize || !pData || !pFlag) 
		return E_POINTER;
    
	if (nSize < mnDataSize) 
		return E_FAIL;
    
	LONG i;
    
	for(i = 0; i< mnDataSize; i++)
	{
		if (mpFlags[i] == DssDataOk || (mNullInclude != Ignore && /*TQMS 444299 ctang 2010-12-8*/mpFlags[i] != DssDataOk))
			pData[i] = mpResults[i];
		if (mNullInclude != Ignore && /*TQMS 444299 ctang 2010-12-8*/mpFlags[i] != DssDataOk)
			pFlag[i] = DssDataOk;
		else
			pFlag[i] = mpFlags[i];
	}
    
	*nDataSize = mnDataSize;
    
	return hr = S_OK;
}

int DSSRankFunction::GetOutputType(EnumDSSParameterType *pType)
{
    int hr = S_OK;
    
	CHECK_PTR(pType);
    
	*pType = DssParameterVector;
    
	return hr = S_OK;
}

// jxie, 2010-9-17
// support direct calculate for rank function
int DSSRankFunction::DirectCalculate (Int32 nParam, Int32* ipSize, Int32** ipOffsets, 
                                      void** ipData, double* ipDivider, DSSDataFlag** ipFlags, VARIANT_BOOL* ipAllDataValid,
                                      EnumDSSDataType* iType, vector<Int32> * ipvDataByGroup,
                                      Int32 nResSize, double* opData, DSSDataFlag* opFlag,
                                      VARIANT_BOOL* opAllDataValid, EnumNullCheckingOption iNullCheckingOption)
{
    int hr = S_OK;
    
	Int32 i = 0;
    
	if((nParam != 1) )
		return E_FAIL;
    
	Int32 lnSize = ipSize[0];
	if (ipvDataByGroup != NULL)
		lnSize = ipvDataByGroup->size();
	if(lnSize < 1 || !ipData[0])
		return E_FAIL;
	
	AE_ASSERT(opFlag && opData);
    
	*opAllDataValid = (ipAllDataValid[0] == VARIANT_TRUE) ? VARIANT_TRUE : VARIANT_FALSE;
	if (iNullCheckingOption == DssNumericErrorTreatAs)
		*opAllDataValid = VARIANT_TRUE;
    
	DSSDataFlag* lFlags = ipFlags[0];
	void* lpData = ipData[0];
	EnumDSSDataType lType = iType[0];
    
	//prepare inputs
	Int32 lnNonNullSize = 0;
	Int32 lRow = 0;
	for(i = 0; i < lnSize; i++) 
	{
		if (ipvDataByGroup != NULL)
			lRow = (*ipvDataByGroup)[i];
		else
			lRow = i;
        
		if (lFlags[lRow] != DssDataOk) 
		{
			// it may be changed later
			if (mNullInclude == Ignore && iNullCheckingOption == DssNumericErrorTreatAs)
			{
				opFlag[lRow] = DssDataOk;
				lnNonNullSize++;
			}
			else if (mNullInclude != Ignore)
			{
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
			opFlag[lRow] = DssDataOk;
			lnNonNullSize++;
		}
	}
    
	/*if (lnNonNullSize == 0)
     return hr = S_OK;*/
    
	// prepare lRank
	Int32* lRank = new Int32[lnNonNullSize];
    CHECK_PTR(lRank);
    MBase::StrongPtr<Int32, MBase::DeleteArray<Int32> > lRankPtr(lRank);
	Int32 lCount = 0;
	for(i = 0; i < lnSize; i++) 
	{
		if (ipvDataByGroup != NULL)
			lRow = (*ipvDataByGroup)[i];
		else
			lRow = i;
        
		if (lFlags[lRow] == DssDataOk || (mNullInclude == Ignore && iNullCheckingOption == DssNumericErrorTreatAs))
            //if (lFlags[lRow] == DssDataOk || iNullCheckingOption == DssNumericErrorTreatAs) 
			lRank[lCount++] = lRow;
		else if (mNullInclude == First)
		{
			opData[lRow] = 1;
		}
		else if (mNullInclude == Last)
		{
			opData[lRow] = lnNonNullSize + 1;
		}
	}
    
	Int32 lPrevRan = mNullInclude == First ? (lnSize - lnNonNullSize) : 0;
    
	RankForRows(lnNonNullSize, lpData, lFlags, lType, opData, mAsc, lRank, lPrevRan);
    
	// calculate percentage
	if(!mByValue)
	{
		Int32 lDataSize = mNullInclude != Ignore ? lnSize : lnNonNullSize;
        
		for(i = 0; i < lnSize; i++)
		{
			if (ipvDataByGroup != NULL)
				lRow = (*ipvDataByGroup)[i];
			else
				lRow = i;
            
			if (opFlag[lRow] == DssDataOk)
			{
				opData[lRow] = opData[lRow] / (double)lDataSize;
			}
		}
	}
    
	return hr = S_OK;
}

// jxie, 2010-9-19
// support direct calculate on all functions in this class 
bool DSSRankFunction::SupportDirectCalculate()
{
	return true;
}

// jxie, 2011-1-12, TQMS 450452
// Support Rank for VARIANT input
int DSSRankFunction::hRankVariant(Int32 nSize, CComVariant *pVar, double *pResult, bool fAsc)
{
    int hr = S_OK;
    
	if (nSize <= 0)
		return hr = S_OK;
    
	VariantSort lVars(pVar, fAsc);
    
	Int32 *pVec = new Int32[nSize];
	CHECK_NEWED_PTR(pVec);
    MBase::DSSStrongPtr<Int32, MBase::DeleteArray<Int32> > vecPtr(pVec);
    
	Int32 i = 0;
	for (i = 0; i < nSize; i ++)
		pVec[i] = i;
    
	_xtab_sort(pVec, pVec + nSize, lVars);
    
	//need to search and set the tie to the same rank
	CComVariant pPrev;
	CComVariant lVar;
	Int32 prevRank = 0;
    
	prevRank = 1;
	getVariant(pVar, pVec[0], pPrev);
	pResult[pVec[0]] = 1;
    
	for(i = 1; i < nSize; i++)	
    {
		getVariant(pVar, pVec[i], lVar);
        
		DSSDataFlag lFlag = DssDataOk;
		Int32  lComp = variantCompare(lVar, pPrev, &lFlag);
		if (lFlag == DssDataOk)	
		{   // comparison succeeded, check result
			if (lComp == 0)	// same rank
				pResult[pVec[i]] = prevRank;
			else 
            {   // new rank
				prevRank = i  +  1;
				pPrev.Copy(&lVar);
				pResult[pVec[i]] = prevRank;
			}
		}
	}
    
	return hr = S_OK;
}