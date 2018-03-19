/*
 *  AggregateGeneral.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 2/4/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#include "DSSAggregateGeneralFunc.h"
#import "DSSFunctionUtility.h"
#import "DSSSort_includes.h"
#import "DSSXTabBitVector.h"
#import "CellDataHelper.h"

DSSAggregateGeneralFunc::DSSAggregateGeneralFunc()
{
    mResultFlag = DssDataUnknown;
    mDataSize = 0;
    mInputDataType = DssDataTypeUnknown;
    mDistinct = false;
    mRemovable = true;
    mInputDistinct = false;
    mpLookup = NULL;
    mpStack = NULL;
}

DSSAggregateGeneralFunc::~DSSAggregateGeneralFunc()
{}

// Caculate for numerical data
int DSSAggregateGeneralFunc::Calculate(int nSize, double * pVec, double * pResult, DSSDataFlag  * pFlag )
{
	if(nSize < 1) 
		return E_FAIL;
	
	CHECK_PTR(pVec);
	CHECK_PTR(pResult);
	CHECK_PTR(pFlag);
	*pFlag = DssDataOk;
	
	double * p = NULL;
	
	switch (mFunctionIndex)
	{
		case DssFuncMax:
			p = std::max_element(pVec, pVec + nSize);
			*pResult = *p;
			return S_OK;
			break;
			
		case DssFuncMin:
			p = std::min_element(pVec, pVec + nSize);
			*pResult = *p;
			return S_OK;
			break;
			
		case DssFuncCount:
			// aye, 6/11/2008
			// count distinct support
			if (mDistinct)
			{
				std::less<double> lDoubleLess;
				int hr = _xtab_sort(pVec, pVec + nSize, lDoubleLess);
				CHECK_HR;
				
				int lnDistinct = (nSize > 0)? 1 : 0;
				for (int i = 0; i < nSize - 1; i++)
					if (pVec[i] != pVec[i + 1])
						lnDistinct++;
				*pResult = lnDistinct;
			}
			else
				*pResult = nSize;
			return S_OK;
			break;
			
		case DssFuncMode:
			return CalculateMode(nSize, pVec, pResult, pFlag); 
			break;
			
		default:
			return E_FAIL;
			
	}
	
	return S_OK;
}

//Calculate for Variant Data
int DSSAggregateGeneralFunc::Calculate (int nSize, CComVariant * pVec, EnumDSSDataType iType, CComVariant * pResult,
                                        DSSDataFlag * pFlag )
{

    int hr = SetInputParam(nSize, iType, pVec);
    CHECK_HR;
    
    switch (mFunctionIndex) {
        case DssFuncMax:
        case DssFuncMin:
            hr = CalculateMaxMin();
            CHECK_HR
            break;
            
        case DssFuncCount:
            hr = CalculateCount();
            CHECK_HR;
            break;
            
        case DssFuncMode:
            hr = CalculateMode();
            CHECK_HR;
            break;
            
        default:
            break;
    }
    
    *pFlag = mResultFlag;
    
    if(DssDataOk == mResultFlag)
    {
        VariantCopy(pResult, &mResultData);
    }
    return S_OK;
}


inline int DSSAggregateGeneralFunc::SetInputParam (int nSize, EnumDSSDataType iType, CComVariant *pData)
{
    if(nSize < 1)
        return E_FAIL;
    
    if(!pData || ( ((pData->vt) & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT) ))
        return E_FAIL;
    
    mDataSize = nSize;
    mInputDataType = iType;
    mInputData.Copy(pData);
    
    return S_OK;
}

// Numerical version

int DSSAggregateGeneralFunc::CalculateMode(int nSize, double * pVec, double * pResult, DSSDataFlag  * pFlag)
{
	int hr = S_OK;
	if(mRemovable&&nSize==1){
		*pResult=pVec[0];
		*pFlag=DssDataOk;
		return S_OK;
	}
	if (nSize <= 1) {
		*pFlag = DssDataUnknown;
		return hr = S_OK;
	}
	
	std::less<double> lDoubleLess;
	hr = _xtab_sort(pVec, pVec + nSize, lDoubleLess);
	CHECK_HR;
	//std::sort(pVec, pVec + nSize, std::less<double>());
	
	// Plus one because both pVec and pVec + nSize are included.
//	MBase::StrongArrayPtr<double *> lItArray(new double * [nSize + 1]); 		
	vector<double *> lvData(nSize + 1);
	double **lItArray = &lvData[0];
	int lCount = 0;
	
	int i;
	double * p;
	
	p = lItArray[lCount++] = pVec;
	for (i = 1; i < nSize; ++i) {
		if (*p != pVec[i]) {
			p = lItArray[lCount++] = pVec + i;
		}
	}
	lItArray[lCount++] = pVec + i; // i.e. pVec + nSize
	
	int lMaxLength = lItArray[1] - lItArray[0];
	p = lItArray[0];
	for (i = 1; i < lCount - 1; ++i) {
		int lLength = lItArray[i + 1] - lItArray[i];
		if (lLength > lMaxLength) {
			lMaxLength = lLength;
			p = lItArray[i];
		}
	}
	*pFlag = (lMaxLength <= 1) ? DssDataUnknown : DssDataOk;
	*pResult = *p;
	return hr = S_OK;
}

//Pred function used by _xtab_sort
bool DSSAggregateGeneralFunc::operator()(int row1, int row2)
{
	int result = 0; 
	if (Order(row1, row2, result) != S_OK)
		throw(_xtab_sort_error(E_FAIL));
	
	return (result < 0);
}

int DSSAggregateGeneralFunc::Order(int row1, int row2, int& result ) 
{
	
	CComVariant  lVar1, lVar2;
	getVariant(&mInputData, row1, lVar1);
	getVariant(&mInputData, row2, lVar2);
	
	DSSDataFlag  lFlag = DssDataOk;
	result = variantCompare(lVar1, lVar2, &lFlag);
	
	if (lFlag != DssDataOk)
		return E_FAIL;
	
	return S_OK;
}

// qjiang 08/14/2009
// support of direct calculate on certain types of aggregation functions
bool DSSAggregateGeneralFunc::SupportDirectCalculate()
{
	switch(mFunctionIndex)
	{
		case DssFuncCount:
		case DssFuncMin:
		case DssFuncMax:
			return true;
			break;
		default:
			return false;
	}
}

// qjiang 08/14/2009
// based on the input data type, choose the right function template parameter
int DSSAggregateGeneralFunc::DirectCalculate(int iStart, int* ipGroupLinkList, int* ipBlockAddress, void* ipData, double iDivider,
																EnumDSSDataType  iType, DSSDataFlag * ipDataFlags, bool iAllDataValid, 
																EnumNullCheckingOption iNullCheckingOption, double* opResult, DSSDataFlag * opDataFlag)
{
	int hr = S_OK;
	
	bool lAllValid = iAllDataValid;
	
	switch(iType)
	{
		case DssDataTypeDouble:
			hr = hDirectCalculate(iStart, ipGroupLinkList, ipBlockAddress, (double*)ipData, iDivider, ipDataFlags, lAllValid, iNullCheckingOption, opResult, opDataFlag, false);
			break;
		case DssDataTypeInteger:
			hr = hDirectCalculate(iStart, ipGroupLinkList, ipBlockAddress, (int*)ipData, iDivider, ipDataFlags, lAllValid, iNullCheckingOption, opResult, opDataFlag, true);
			break;
		case DssDataTypeShort:
			hr = hDirectCalculate(iStart, ipGroupLinkList, ipBlockAddress, (short*)ipData, iDivider, ipDataFlags, lAllValid, iNullCheckingOption, opResult, opDataFlag, true);
			break;
		default:
			AE_ASSERT(false);
	}
	
	return hr;
}

template<class Type>
int DSSAggregateGeneralFunc::hDirectCalculate(int iSize, int * ipGroupLinkList, int *ipBlockAddress,  Type* ipData, double iDivider, DSSDataFlag * ipDataFlags, bool iAllDataOK, 
										   EnumNullCheckingOption iNullCheckingOption, double* opResult, DSSDataFlag * opDataFlag, bool ibInteger)
{
	int hr = S_OK;
	bool lNeedConvert = true;
	switch(mFunctionIndex)
	{
		case DssFuncMax:
		case DssFuncMin:
			hr = hDirectCalculateMaxMin(iSize, ipGroupLinkList, ipBlockAddress, ipData, ipDataFlags, iAllDataOK, iNullCheckingOption, opResult, opDataFlag);
			CHECK_HR;
			break;
			
		case DssFuncCount:
			lNeedConvert = false; // count is divider-insensitive
			hr = hDirectCalculateCount(iSize, ipGroupLinkList, ipBlockAddress, ipData, ipDataFlags, iAllDataOK, iNullCheckingOption, opResult, opDataFlag, ibInteger);
			CHECK_HR;
			break;
			
		default:
			return E_NOTIMPL;
	}
	
	if (lNeedConvert && iDivider > 1)
	{
		*opResult /= iDivider;
	}
	return S_OK;
}

// qjiang 08/14/2009
// direct calculate for Min/Max
template<class Type>
int DSSAggregateGeneralFunc::hDirectCalculateMaxMin(int iSize, int * ipGroupStart, int *ipBlockAddress,  Type* ipData, DSSDataFlag * ipDataFlags, bool iAllDataOK, 
												 EnumNullCheckingOption iNullCheckingOption, double* opResult, DSSDataFlag * opDataFlag)
{
	*opResult = 0;
	*opDataFlag = DssDataOk;
	
//	int hr = S_OK;
	
	if (iSize == 0)
	{
		*opDataFlag = DssDataInvalid;
		return S_OK;
	}
	
	// lResult is actually initialized as the "first" value of the input array
	Type lResult = 0;
	
	bool lFirst = true;
	bool lErrorSkip = (iNullCheckingOption == DssNumericErrorSkip);
	bool lCalculateMax = (mFunctionIndex == DssFuncMax);
	
	for (int i = 0; i < iSize; i++)
	{
		int lBlock = ipGroupStart[i];
		
		int lStart = ipBlockAddress ? ipBlockAddress[lBlock] : lBlock;
		int lEnd = ipBlockAddress ? ipBlockAddress[lBlock + 1] : lBlock + 1;
		
		for (int lRow = lStart; lRow < lEnd; lRow++)
		{
			if (ipDataFlags[lRow] != DssDataOk && lErrorSkip)
				continue;
			
			Type lData = ipData[lRow];
			if (lFirst)
			{
				lResult = lData;
				lFirst = false;
			}
			else if((lCalculateMax && lData > lResult) || (!lCalculateMax && lData < lResult))
			{
				lResult = lData;
			}
		}
	}
	
	if(lFirst) // no valid input
		*opDataFlag = DssDataInvalid;
	else
		*opResult = lResult;
	
	return S_OK;
}

// qjiang 08/14/2009
// direct calculate for count
// 09/21/2009 optimize for count distinct, previous method using std::set would cause concurrency problems
template<class Type>
int DSSAggregateGeneralFunc::hDirectCalculateCount(int iSize, int * ipGroupStart, int *ipBlockAddress, Type* ipData, DSSDataFlag * ipDataFlags, bool iAllDataOK, 
												EnumNullCheckingOption iNullCheckingOption, double* opResult, DSSDataFlag * opDataFlag, bool ibInteger)
{
	int hr = S_OK;
	
	if (iSize == 0)
	{
		*opResult = 0;
		*opDataFlag = DssDataInvalid;
		return S_OK;
	}
	
	int lMax = INT_MIN;
	int lMin = INT_MAX;
	
	int lnValid = 0;
	int lnInvalid = 0;
	
    bool lfCountRow = false;
	if (ipData == NULL // this is count(*)
		|| (iAllDataOK && !mDistinct) // all data valid and not count distinct
		|| (iAllDataOK && mInputDistinct)) // all data valid and input is distinct
		lfCountRow = true;
    
	// if input is integer column, and asks for distinct, get the range, which helps to decide whether to use bit vector approach
	bool lfGetRange = mDistinct && ibInteger && (mpLookup == NULL);
	bool lfErrorSkip = (iNullCheckingOption == DssNumericErrorSkip);
	
	// first, go over the input, to get the rough count, and if necessary, get the range
	for (int i = 0; i < iSize; ++i)
	{
		int lBlock = ipGroupStart[i];
		
		int lStart = ipBlockAddress ? ipBlockAddress[lBlock] : lBlock;
		int lEnd = ipBlockAddress ? ipBlockAddress[lBlock + 1] : lBlock + 1;
		
        if (lfCountRow || (iAllDataOK && !lfGetRange))
		{
			lnValid += lEnd-lStart;
		}
		else
		{
            for (int lRow = lStart; lRow < lEnd; lRow++)
            {
                if(ipDataFlags[lRow] == DssDataOk)
                {
                    lnValid ++;
                    if (lfGetRange)
                    {
                        Type lValue = ipData[lRow];
					
                        if (lValue > lMax)
                            lMax = lValue;
					
                        if (lValue < lMin)
                            lMin = lValue;
                    }
                }
                else
                {
                    lnInvalid ++;
                }
            }
		}
	}
    
    if (lnValid == 0 && lnInvalid == 0)
	{
		*opResult = 0;
		*opDataFlag = DssDataNull;
		return S_OK;
	}
	
	// simple count, return the result
    // count(*) should ignore the distinct property
	// if we know the input is already distinct, no need to do further distinct operation
	if (!mDistinct || ipData == NULL || mInputDistinct)
	{
		*opDataFlag = DssDataOk;
		if (lfErrorSkip)
		{
			*opResult = lnValid;
			
			if (lnValid == 0)
				*opDataFlag = DssDataInvalid;
		}
		else
		{
			*opResult = lnValid + lnInvalid;
		}
		return S_OK;
	}
	
	// no valid input, return 0 or 1 depends on the error checking
	if (lnValid == 0)
	{
		if (lfErrorSkip)
		{
			*opResult = 0;
			*opDataFlag = DssDataInvalid;
		}
		else
		{// here lnInvalid must > 0
			*opResult = 1;
			*opDataFlag = DssDataOk;
		}
		return S_OK;
	}
	
	// for count distinct, we choose different approach according to the input data type and range
	unsigned int lnTotal = lnValid + lnInvalid;
	bool lfUseBitVector = false;
	
	unsigned int lRange = 0;
	if (lfGetRange)
	{
		lRange = lMax - lMin; // this would not overflow
		
		if (lRange == UINT_MAX)
			lfUseBitVector = false;
		// memory for lnTotal integers = lnTotal * 4, memory for bit vector has length lRange = lRange / 8
		// therefore, if the memory is equal, the equaltion would be lRange = lnTotal * 32, here we allow the 
		// bit vector to have some more memory, since it performs better
		else if (lRange < 40*(size_t)lnTotal) 
		{
			++ lRange;
			lfUseBitVector = true;
		}
	}
	
	DSSXTabBitVector* lBV = new DSSXTabBitVector(lRange);

	Type* lpData = NULL;
    Int32* lpBottom = NULL;
	Int32* lpTop = NULL;
	if (mpLookup)
	{
		lpBottom = mpStack;
		lpTop = mpStack;
	}
	else
	{
	
        if (lfUseBitVector)
        {
            lBV->Init(lRange, true);
        }
        else
        {// not fit to use bit vector, have to do sorting here
            lpData = new Type[lnTotal];
            CHECK_NEWED_PTR(lpData);
        }
	}
	
    unsigned Int32 lResult = 0;
    
	// prepare the data
	int j = 0;
	for (int i = 0; i < iSize; ++i)
	{
		int lBlock = ipGroupStart[i];
		
		int lStart = ipBlockAddress ? ipBlockAddress[lBlock] : lBlock;
		int lEnd = ipBlockAddress ? ipBlockAddress[lBlock + 1] : lBlock + 1;
		
		for (int lRow = lStart; lRow < lEnd; lRow++)
		{
			if(ipDataFlags == NULL || ipDataFlags[lRow] == DssDataOk)
			{
				Type lValue = ipData[lRow];
                if (mpLookup)
				{
					Int32 lIndex = (Int32)lValue;
					if (!mpLookup[lIndex])
					{
						mpLookup[lIndex] = true;
						*lpTop = lIndex;
						lpTop++;
                        
						lResult++;
					}
				}
				else if (lfUseBitVector)
				{
                    lBV->Set((unsigned int)(lValue - lMin), true);
				}
				else
				{
					lpData[j++] = lValue;
				}
			}
		}
	}
	
	// calculate distinct
	if (mpLookup)
	{
		if (!lfErrorSkip && lnInvalid > 0 && !mpLookup[0])
			++ lResult;
        
		while (lpTop != lpBottom)
		{
			lpTop--;
			mpLookup[*lpTop] = false;
		}
	}
	else if (lfUseBitVector)
	{
		lResult = lBV->GetSetSize();
		if (!lfErrorSkip && lnInvalid > 0)
		{// check whether 0 appears in the valid input, if not, since treat null as 0, add this into count
			// (lMax < 0 || lMin > 0) means 0 is outof the range, !lBV.Get(-lMin) means 0 is in the range, and checking whether it is already set
			if (lMax < 0 || lMin > 0 || !lBV->Get(-lMin))
			{
				++ lResult;
			}
		}

	}
	else
	{// if exists invalid rows and treat null as 0, add a zero to the array, no matter 0 appears before, the following steps will distinct it
		if (!lfErrorSkip && lnInvalid > 0)
			lpData[lnValid++] = 0;
		
		std::less<Type> lTypeLess;
		hr = _xtab_sort(lpData, lpData + lnValid, lTypeLess);
		CHECK_HR;
		
		lResult = (lnValid > 0)? 1 : 0;
		for (int i = 0; i < lnValid - 1; i++)
			if (lpData[i] != lpData[i + 1])
				lResult++;
	}
	
	*opResult = lResult;
	*opDataFlag = DssDataOk;
	
	if (lResult == 0)
		*opDataFlag = DssDataInvalid;
	
	return S_OK;
}

int DSSAggregateGeneralFunc::CalculateMaxMin()
{
    mResultFlag = DssDataUnknown;
    
    int i = 0;
    for(i = 0; i < mDataSize; ++i)
    {
        CComVariant lVar;
        getVariant(&mInputData, i, lVar);
        if(mResultFlag != DssDataOk)
        {
            mResultFlag = DssDataOk;
            mResultData.Copy(&lVar);
        }
        else
        {
            DSSDataFlag lFlag = DssDataOk;
            Int32 lResult = variantCompare(mResultData, lVar, &lFlag);
            if(lResult < 0 && DssFuncMax == mFunctionIndex)
            {
                mResultData.Copy(&lVar);
            } 
            else if( lResult > 0 && DssFuncMin == mFunctionIndex)
            {
                mResultData.Copy(&lVar);
            }
        }
    }
    return S_OK;
}

int DSSAggregateGeneralFunc::CalculateCount()
{
    int hr = S_OK;
    int lCount = mDataSize;
    
    if(mDistinct && mDataSize > 1)
    {
        vector<Int32> lvSortedIndex(mDataSize);
        Int32* lpSortedIndexPtr = &lvSortedIndex[0];
        
        int i = 0;
        for(i = 0; i < mDataSize; ++i)
        {
            lpSortedIndexPtr[i] = i;
        }
        
        hr = _xtab_sort(lpSortedIndexPtr, lpSortedIndexPtr+mDataSize, *this);
        CHECK_HR;
        
        
        CComVariant lPreVar;
        getVariant(&mInputData, lpSortedIndexPtr[0], lPreVar);
        lCount = 1;
        
        for(i = 1; i < mDataSize; ++i)
        {
            CComVariant lNextVar;
            getVariant(&mInputData, lpSortedIndexPtr[i], lNextVar);
            
            DSSDataFlag lFlag = DssDataOk;
            Int32 lResult = variantCompare(lPreVar, lNextVar, &lFlag);
            if(DssDataOk != lFlag)
            {
                return E_FAIL;
            }
            
            if(0 != lResult)
            {
                lCount++;
            }
            lPreVar = lNextVar;
        }
    }
    
    hr = DFCDataConversion::ConvertToVariant((unsigned char *)&lCount, DssDataTypeLong, 0, &mResultData);
	if(hr != S_OK) return hr;
    
	mResultFlag = DssDataOk;
    
	return S_OK;
}


int DSSAggregateGeneralFunc::CalculateMode()
{
    int hr = S_OK;
    
    if(mRemovable && mDataSize==1)
    {
        getVariant(&mInputData, 0, mResultData);
        mResultFlag = DssDataOk;
        return S_OK;
    }
    
    mResultFlag = DssDataUnknown;
    
    if(mDataSize < 1)
        return S_OK;
    
    vector<Int32> lvSortedIndex(mDataSize);
    Int32* lpSortedIndexPtr = &lvSortedIndex[0];
    
    Int32 i = 0;
    for(i = 0; i < mDataSize; ++i)
    {
        lpSortedIndexPtr[i] = i;
    }
    
    hr = _xtab_sort(lpSortedIndexPtr, lpSortedIndexPtr+mDataSize, *this);
    CHECK_HR;
    
    Int32 lModeCandidate = -1;
    Int32 lMaxRepeat = 1;
    Int32 lCurrentLength = 1;
    
    CComVariant lVar, lCurrentVar;
    DSSDataFlag lFlag = DssDataOk;
    
    getVariant(&mInputData, 0, lVar);
    
    for(i = 1; i < mDataSize; ++i)
    {
        getVariant(&mInputData, lpSortedIndexPtr[i], lCurrentVar);
        
        Int32 lResult = variantCompare(lVar, lCurrentVar, &lFlag);
        if (DssDataOk != lFlag) {
            return E_FAIL;
        }
        
        if(0 == lResult)
        {
            ++lCurrentLength;
        }
        else
        {
            if(lCurrentLength > lMaxRepeat)
            {
                lMaxRepeat = lCurrentLength;
                lModeCandidate = i - 1;
            }
            
            lVar.Copy(&lCurrentVar);
            lCurrentLength = 1;
        }
    }
    
    if(lCurrentLength > lMaxRepeat)
    {
        lMaxRepeat = lCurrentLength;
        lModeCandidate = i - 1;
    }
    
    if(lModeCandidate != -1)
    {
        getVariant(&mInputData, lpSortedIndexPtr[lModeCandidate], mResultData);
        mResultFlag = DssDataOk;
    }
    
    return S_OK;
}

int DSSAggregateGeneralFunc::getOutputType(EnumDSSDataType InputType, EnumDSSDataType* pOutputType)
{
    if(!pOutputType)
    {
        return E_POINTER;
    }
    
    if(mFunctionIndex == DssFuncCount)
    {
        //*pOutputType = DssDataTypeLong;
        *pOutputType=DssDataTypeInteger;
    }
    else if(mFunctionIndex == DssFuncMax || mFunctionIndex == DssFuncMin || mFunctionIndex == DssFuncMode)
    {
        *pOutputType = InputType;
    }
    else
    {
        return E_FAIL;
    }
    
    return S_OK;
}

int DSSAggregateGeneralFunc::Clear()
{
    mDataSize = 0;
    mInputDataType = DssDataTypeUnknown;
    mResultFlag = DssDataUnknown;
    
    mInputData.Clear();
    mResultData.Clear();
    return S_OK;
}

int DSSAggregateGeneralFunc::setProperty(int Index, CComVariant *ipVal)
{
//    return E_NOTIMPL;
    
    //!!!!zhyang, 28/07/2011, we need to define some function property enums.
    if (!ipVal)
		return E_POINTER;
	HRESULT hr=S_OK;
	switch (Index)
	{
		case 1://only mode() has a property while other functions currently implemented in this class have not.
        {
            //xiyu,340091, when the type is not bool, convert it instead of returning failure.
            if (ipVal->vt != VT_BOOL){
                hr = VariantChangeType(ipVal, ipVal, 0, VT_BOOL);
                CHECK_HR;
            }
            
            if(mFunctionIndex==DssFuncMode){
                mRemovable=(ipVal->boolVal==VARIANT_TRUE);
            }
            break;
        }
		case -1: //DssFuncPropertyDistinct:
        {
            //xiyu,340091, when the type is not bool, convert it instead of returning failure.
            if (ipVal->vt != VT_BOOL){
                hr = VariantChangeType(ipVal, ipVal, 0, VT_BOOL);
                CHECK_HR;
            }
            
            mDistinct = (ipVal->boolVal == VARIANT_TRUE);
            break;
        }
		default:	// we ignore other properties
			break;
	}
    
	return hr;
}

bool DSSAggregateGeneralFunc::GetDistinctProperty()
{
    return mDistinct;
}

void DSSAggregateGeneralFunc::SetInputDistinct(bool iVal)
{
    mInputDistinct = iVal;
}

int DSSAggregateGeneralFunc::SetWorkingBuffer(/*MBase::Buffer* ipBuffer, */Int32 iMaxKey)
{
	if (mDistinct)
	{
		// OK, we need to do the distinct operation, allocate the buffer
		mpLookup = new bool[iMaxKey+1];
		CHECK_NEWED_PTR(mpLookup);
		memset(mpLookup, 0, sizeof(bool) * (iMaxKey+1));
        
		mpStack = new Int32[iMaxKey+2];
		CHECK_NEWED_PTR(mpStack);
	}
    
	return S_OK;
}
//tliao, 822148, 2014/09/23, support cell format data calculation
int DSSAggregateGeneralFunc::CalculateCellData(Int32 nParam, CComVariant * pParam, CComVariant * pResult, DSSData_Flags * pFlag)
{
	HRESULT hr;
    
	if (nParam < 1)
		return E_FAIL;
    
	if (!pParam || !pResult || !pFlag)
		return E_POINTER;
    
	if ((pParam->vt & (VT_ARRAY | VT_VARIANT)) != (VT_ARRAY | VT_VARIANT))
		return E_FAIL;
    
	vector<double> lvValues(nParam);
	vector<EnumCellFormatDataTypes> lvTypes(nParam);
	vector<DSS_ID> lvFormatIDs(nParam);
    
	hr=CellDataHelper::SplitCellData(nParam, pParam, lvValues, lvTypes, lvFormatIDs);
	CHECK_HR;
    
	*pFlag=DssDataOk;
    
	double lResult;
	hr=Calculate(nParam, &lvValues[0], &lResult, pFlag);
	CHECK_HR;
    
	EnumCellFormatDataTypes lResultType;
	DSS_ID lResultFormatID;
	CalculateCellTypeAndFormat(nParam, &lvTypes[0], &lvFormatIDs[0], &lResultType, &lResultFormatID);
    
	hr=CellDataHelper::CreateVariantFromCellData(lResult, lResultType, lResultFormatID, pResult);
	CHECK_HR;
    
	return S_OK;
}

void DSSAggregateGeneralFunc::CalculateCellTypeAndFormat(Int32 nParam, EnumCellFormatDataTypes *pTypes, DSS_ID *pFormatIDs, EnumCellFormatDataTypes *oResultType, DSS_ID *oResultFormatID)
{
	switch(mFunctionIndex)
	{
		case DssFuncCount:
		{
			*oResultType=DssCellFormatDataTypeDouble;
			*oResultFormatID=CellDataHelper::GetDefaultFormatID();
			break;
		}
		case DssFuncMin:
		case DssFuncMax:
		case DssFuncMode:
		{
			*oResultType=CellDataHelper::CalculateCellDataType(nParam,pTypes);
			*oResultFormatID=CellDataHelper::CalculateCellDataFormatID(nParam,pFormatIDs);
			break;
		}
		default:
		{
			*oResultType=DssCellFormatDataTypeDouble;
			*oResultFormatID=CellDataHelper::GetDefaultFormatID();
			break;
		}
	}
}

