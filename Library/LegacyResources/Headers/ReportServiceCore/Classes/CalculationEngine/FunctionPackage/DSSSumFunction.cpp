/*
 *  DSSSumFunction.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 2/4/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#include "DSSSumFunction.h"
#import "DSSFunctionUtility.h"
#import "CellDataHelper.h"

#define DOUBLE_MAX 1.7e308

DSSSumFunction::DSSSumFunction():mDistinct(false)
{}

DSSSumFunction::~DSSSumFunction()
{}

int DSSSumFunction::Calculate(int nSize, double *pVec, double *opResult, DSSDataFlag *pFlag)
{
	if(nSize < 1) 
		return E_INVALIDARG;
	
	CHECK_PTR(pVec);
	CHECK_PTR(opResult);
	CHECK_PTR(pFlag);
	*pFlag = DssDataOk;
	
	enum MathError error = MathNoError;
	int i = 0;
	switch(mFunctionIndex)
	{
		case DssFuncSum:
			*opResult = 0;
			for (i = 0; i < nSize; i++)
				*opResult = (*opResult) + pVec[i];
			
			*pFlag = DssDataOk;
			break;
			
		case DssFuncAvg:
			
			*opResult = AvgImpl(nSize, pVec);
			*pFlag = DssDataOk;
			break;
			
		case DssFuncProduct:
			*opResult = 1;
			for (i = 0; i < nSize; i++)
				*opResult = (*opResult) * pVec[i];
			
			*pFlag = DssDataOk;
			break;
			
		case DssFuncMedian:
			if(!Median(nSize, pVec, opResult))
			{
				*pFlag = DssDataInvalid;
				return E_FAIL;
				// AE_MSG; YMA; Internal Error
			}
			
			*pFlag = DssDataOk;
			break;
			
		default: 
			return E_FAIL;
			// AE_MSG YMA, Unkown function
	}
	
	CheckValue(&error, *opResult);
	
	if(error != MathNoError)
	{
		//ytang 2008-06-25 TQMS313044
		*opResult = 0;
		if (error == MathOverflow || error == MathIllegalOp) {
			*pFlag = DssDataOverflow;		
		} else {
			*pFlag = DssDataInvalid;
		}  
		//*pFlag = DssDataInvalid;
	} 
	
	
	return S_OK;
}

//xhan 03/07/2001 incremental average algorithm
double DSSSumFunction::AvgImpl(int nSize, double *pData)
{
    // double d1;
	double d2, M1, M2;
	int i = 0;
	
	//lwang, 08/25/2008, TQMS 325456
	//use incremental average algorithm only if SUM(pData[i]) will overflow
	double max = DOUBLE_MAX / nSize;
	double *lp = pData;
	for (i = 0; i < nSize; i++)
	{
		if (*lp++ >= max)
			break;
	}
	
	if (i < nSize)		//will overflow
	{
		// d1 = 0.0;
		M1 = pData[0];
		
		for (i = 1; i < nSize; i ++){
			d2 = (pData[i] - M1) / (i + 1);
			M2 = M1 + d2;
			
			// d1 = d2;
			M1 = M2;
		}
	}
	else		//won't overflow
	{
		lp = pData;
		M1 = *lp++;
		for ( i = 1; i < nSize; i++)
			M1 = M1 + *lp++;
		M1 = M1 / nSize;
	}
	
	return M1;
}


// qjiang 04/16/2009
int DSSSumFunction::DirectCalculate(int iSize, int * ipGroupStart, int *ipBlockAddress, void* ipData, double iDivider, EnumDSSDataType iDataType, DSSDataFlag* ipDataFlag, bool iAllDataValid,
														   EnumNullCheckingOption iNullCheckingOption, double* opResult, DSSDataFlag* opDataFlag)

{
	/*
	 It's caller's responsibility to make sure all parameters are valid.
	 */
	int hr = S_OK;
	bool lAllDataOK = iAllDataValid;
	
	enum MathError error = MathNoError;
	
	bool lNeedConvert = true;
	
	//all of the four types of functions in this class support DirectCalculate
	switch(mFunctionIndex)
	{
		case DssFuncSum: 
			hr = hGroupAggImpl(iSize, ipGroupStart, ipBlockAddress, ipData, iDataType, ipDataFlag, lAllDataOK, iNullCheckingOption, opResult, opDataFlag);
			CHECK_HR;
			break;
			
		case DssFuncAvg:
			hr = hGroupAvgImpl(iSize, ipGroupStart, ipBlockAddress, ipData, iDataType, ipDataFlag, lAllDataOK, iNullCheckingOption, opResult, opDataFlag);
			CHECK_HR;
			break;
			
		case DssFuncMedian:
			hr = hGroupMedianImpl(iSize, ipGroupStart, ipBlockAddress, ipData, iDataType, ipDataFlag, lAllDataOK, iNullCheckingOption, opResult, opDataFlag);
			CHECK_HR;
			break;
			
		case DssFuncProduct:
			lNeedConvert = false; // product will handle divider inside the implementation
			hr = hGroupProductImpl(iSize, ipGroupStart, ipBlockAddress, ipData, iDivider, iDataType, ipDataFlag, lAllDataOK, iNullCheckingOption, opResult, opDataFlag);
			CHECK_HR;
			
			break;
			
		default: 
			return hr = E_FAIL;
			// AE_MSG YMA, Unkown function
	}
	
	CheckValue(&error, *opResult);
	
	if(error != MathNoError)
	{
		*opResult = 0;
		//ytang 2008-06-24 TQMS313044
		if (error == MathOverflow) {
			*opDataFlag = DssDataOverflow;		
		} else {
			*opDataFlag = DssDataInvalid;
		}
		//*opDataFlag = DssDataInvalid;
	}
	
	if (lNeedConvert && *opDataFlag == DssDataOk && iDivider > 1)
		*opResult /= iDivider;
	
	return S_OK;
}

//qjiang 04/16/2009
int DSSSumFunction::hGroupAggImpl(int iStart,
									   int * ipGroupLinkList,
									   int * ipBlockAddress,
									   void* ipData,
									   EnumDSSDataType iDataType,
									   DSSDataFlag* ipDataFlags,
									   bool iAllDataValid,
									   EnumNullCheckingOption iNullCheckingOption,
									   double* opResult,
									   DSSDataFlag* opDataFlag)
{
	int hr = S_OK;
	
	AE_ASSERT(mFunctionIndex == DssFuncSum);
	
	switch (iDataType)
	{
		case DssDataTypeDouble:
			hr = hSumImpl<double, double>(iStart, ipGroupLinkList, ipBlockAddress, (double*)ipData, ipDataFlags, iAllDataValid, iNullCheckingOption, opResult, opDataFlag);
			break;
		case DssDataTypeInteger:
			hr = hSumImpl<Int32, Int64>(iStart, ipGroupLinkList, ipBlockAddress, (Int32*)ipData, ipDataFlags, iAllDataValid, iNullCheckingOption, opResult, opDataFlag);
			break;
		case DssDataTypeShort:
			hr = hSumImpl<short, Int64>(iStart, ipGroupLinkList, ipBlockAddress, (short*)ipData, ipDataFlags, iAllDataValid, iNullCheckingOption, opResult, opDataFlag);
			break;
		default:
			return hr = E_FAIL;
			break;
	}
	
	return hr;
}

// qjiang 08/14/2009
// support DirectCalculate for average
int DSSSumFunction::hGroupAvgImpl(int iStart,
									   int * ipGroupLinkList,
									   int * ipBlockAddress,
									   void* ipData,
									   EnumDSSDataType iDataType,
									   DSSDataFlag* ipDataFlags,
									   bool iAllDataValid,
									   EnumNullCheckingOption iNullCheckingOption,
									   double* opResult,
									   DSSDataFlag* opDataFlag)
{
	int hr = S_OK;
	
	AE_ASSERT(mFunctionIndex == DssFuncAvg);
	
	switch (iDataType)
	{
		case DssDataTypeDouble:
			hr = hAvgImpl<double, double>(iStart, ipGroupLinkList, ipBlockAddress, (double*)ipData, ipDataFlags, iAllDataValid, iNullCheckingOption, opResult, opDataFlag);
			break;
		case DssDataTypeInteger:
			hr = hAvgImpl<Int32, Int64>(iStart, ipGroupLinkList, ipBlockAddress, (Int32*)ipData, ipDataFlags, iAllDataValid, iNullCheckingOption, opResult, opDataFlag);
			break;
		case DssDataTypeShort:
			hr = hAvgImpl<short, Int64>(iStart, ipGroupLinkList, ipBlockAddress, (short*)ipData, ipDataFlags, iAllDataValid, iNullCheckingOption, opResult, opDataFlag);
			break;
		default:
			return hr = E_FAIL;
			break;
	}
	
	return hr;
}

// qjiang 08/14/2009
// support DirectCalculate for median
int DSSSumFunction::hGroupMedianImpl(int iStart,
										  int * ipGroupLinkList,
										  int * ipBlockAddress,
										  void* ipData,
										  EnumDSSDataType iDataType,
										  DSSDataFlag* ipDataFlags,
										  bool iAllDataValid,
										  EnumNullCheckingOption iNullCheckingOption,
										  double* opResult,
										  DSSDataFlag* opDataFlag)
{
	int hr = S_OK;
	
	AE_ASSERT(mFunctionIndex == DssFuncMedian);
	
	switch (iDataType)
	{
		case DssDataTypeDouble:
			hr = hMedianImpl<double>(iStart, ipGroupLinkList, ipBlockAddress, (double*)ipData, ipDataFlags, iAllDataValid, iNullCheckingOption, opResult, opDataFlag);
			break;
		case DssDataTypeInteger:
			hr = hMedianImpl<Int32>(iStart, ipGroupLinkList, ipBlockAddress, (Int32*)ipData, ipDataFlags, iAllDataValid, iNullCheckingOption, opResult, opDataFlag);
			break;
		case DssDataTypeShort:
			hr = hMedianImpl<short>(iStart, ipGroupLinkList, ipBlockAddress, (short*)ipData, ipDataFlags, iAllDataValid, iNullCheckingOption, opResult, opDataFlag);
			break;
		default:
			return hr = E_FAIL;
			break;
	}
	
	return hr;
}

// qjiang 08/14/2009
// support DirectCalculate for product
int DSSSumFunction::hGroupProductImpl(int iStart,
										   int * ipGroupLinkList,
										   int * ipBlockAddress,
										   void* ipData, double iDivider,
										   EnumDSSDataType iDataType,
										   DSSDataFlag* ipDataFlags,
										   bool iAllDataValid,
										   EnumNullCheckingOption iNullCheckingOption,
										   double* opResult,
										   DSSDataFlag* opDataFlag)
{
	int hr = S_OK;
	
	AE_ASSERT(mFunctionIndex == DssFuncProduct);
	
	switch (iDataType)
	{
		case DssDataTypeDouble:
			hr = hProductImpl<double>(iStart, ipGroupLinkList, ipBlockAddress, (double*)ipData, iDivider, ipDataFlags, iAllDataValid, iNullCheckingOption, opResult, opDataFlag);
			break;
		case DssDataTypeInteger:
			hr = hProductImpl<Int32>(iStart, ipGroupLinkList, ipBlockAddress, (Int32*)ipData, iDivider, ipDataFlags, iAllDataValid, iNullCheckingOption, opResult, opDataFlag);
			break;
		case DssDataTypeShort:
			hr = hProductImpl<short>(iStart, ipGroupLinkList, ipBlockAddress, (short*)ipData, iDivider, ipDataFlags, iAllDataValid, iNullCheckingOption, opResult, opDataFlag);
			break;
		default:
			return hr = E_FAIL;
			break;
	}
	
	return hr;
}

// qjiang 08/14/2009
// support direct calculate on all functions in this class 
bool DSSSumFunction::SupportDirectCalculate()
{
	return true;
}


template <class InputDataType, class TempType>
int DSSSumFunction::hSumImpl(int iSize, int * ipGroupStart, int *ipBlockAddress, InputDataType* ipData, DSSDataFlag *ipDataFlags, bool iAllDataOK, 
							  EnumNullCheckingOption iNullCheckingOption, double* opResult, DSSDataFlag* opDataFlag)
{
	*opResult = 0;
	*opDataFlag = DssDataOk;
	
//	int size = 0;
	
//	int hr = S_OK;
	
	TempType lResult = 0;
	
	if (iSize == 0)
	{
		*opDataFlag = DssDataInvalid;
		return S_OK;
	}
	
	// ychao; 29Jan2008; Direct Calculation; 
	// For sum function, treate as zero is the same as skip
//	bool lAllDataInvalid = true;
	
	/* ychao 05Feb2008; Direct Calculataion
	 Currently only DssNumericErrorSkip or DssNumericErrorTreatAs will call direct calculate
	 In case all rows are invalid and DssNumericErrorSkip, we should set the flag to invalid
	 */
	//fali, 820468, support sum(Distinct)
    if (!mDistinct)
    {
        if (ipBlockAddress == NULL)
        {
            int *lpRow = ipGroupStart;
            for (int i = iSize; i > 0; i--)
            {
                lResult += ipData[*lpRow];
                lpRow++;
            }
        }
        else
        {
            int *lpBlock = ipGroupStart;
            for (int i = iSize; i > 0; i--)
            {
                int *lpStart = &ipBlockAddress[*lpBlock];
                int lStart = *lpStart;
                int lBlockSize = *(lpStart + 1) - lStart;
                
                InputDataType *lpData = &ipData[lStart];
                for (int lRow = lBlockSize; lRow > 0; lRow--)
                {
                    lResult += *lpData;
                    lpData++;
                }
                lpBlock++;
            }
        }
    }
    else
    {
        //Sum(Distinct)
        vector<InputDataType> lvInputData;
        if (ipBlockAddress == NULL)
        {
            int *lpRow = ipGroupStart;
            for (int i = iSize; i > 0; i--)
            {
                lvInputData.push_back(ipData[*lpRow]);
                lpRow++;
            }
        }
        else
        {
            int *lpBlock = ipGroupStart;
            for (int i = iSize; i > 0; i--)
            {
                int *lpStart = &ipBlockAddress[*lpBlock];
                int lStart = *lpStart;
                int lBlockSize = *(lpStart + 1) - lStart;
                
                InputDataType *lpData = &ipData[lStart];
                for (int lRow = lBlockSize; lRow > 0; lRow--)
                {
                    lvInputData.push_back(*lpData);
                    lpData++;
                }
                lpBlock++;
            }
        }
        std::sort(lvInputData.begin(), lvInputData.end());
        lResult = lvInputData[0];
        for (int i = 1; i < lvInputData.size(); i++)
        {
            if (lvInputData[i-1] != lvInputData[i]) {
                lResult += lvInputData[i];
            }
        }
    }
	
	*opResult = (double)lResult;
	
	// If the result is 0, check whether all involved rows are invalid
	if(lResult == 0 && !iAllDataOK)
	{
		*opDataFlag = DssDataInvalid;
        
        bool lfAllSkipCheckNUll = true;
		
		for (int i = 0; i < iSize && *opDataFlag != DssDataOk; i++)
		{
			int lBlock = ipGroupStart[i];
			
			int lStart = ipBlockAddress ? ipBlockAddress[lBlock] : lBlock;
			int lEnd = ipBlockAddress ? ipBlockAddress[lBlock + 1] : lBlock + 1;
			
			for (int lRow = lStart; lRow < lEnd; lRow++)
            {
                DSSDataFlag lFlag = ipDataFlags[lRow];
                if(lFlag == DssDataIgnoreCheckNull)
                    continue;
                
                lfAllSkipCheckNUll = false;
                if(lFlag == DssDataOk || iNullCheckingOption == DssNumericErrorTreatAs)
				{
					*opDataFlag = DssDataOk;
					break;
				}
            }

		}
        if(lfAllSkipCheckNUll)
            *opDataFlag = DssDataIgnoreCheckNull;
	}
	
	return S_OK;
}

// qjiang
// direct calculate for average
template <class InputDataType, class TempType>
int DSSSumFunction::hAvgImpl(int iSize, int * ipGroupStart, int *ipBlockAddress, InputDataType* ipData, DSSDataFlag *ipDataFlags, bool iAllDataOK, 
							  EnumNullCheckingOption iNullCheckingOption, double* opResult, DSSDataFlag* opDataFlag)
{
	int hr = S_OK;
	double lResult = 0;
	
	// first get the sum
	hr = hSumImpl<InputDataType, TempType>(iSize, ipGroupStart, ipBlockAddress, ipData, ipDataFlags, iAllDataOK, iNullCheckingOption, &lResult, opDataFlag);
	CHECK_HR;
	
	if (*opDataFlag != DssDataOk)
		return S_OK;
	
	bool lfAllSkipCheckNull = false;
    // get the total
	int lnTotal = 0;
	
	if (iAllDataOK)
	{
		if (ipBlockAddress == NULL)
			lnTotal = iSize;
		else
		{
			for (int i = 0; i < iSize; i++)
			{
				int lBlock = ipGroupStart[i];
				lnTotal += ipBlockAddress[lBlock + 1] - ipBlockAddress[lBlock];
			}
		}
	}
	else
	{
        lfAllSkipCheckNull = true;
        DSSDataFlag lFlag = DssDataOk;
        
		for (int i = 0; i < iSize; i++)
		{
			int lBlock = ipGroupStart[i];
			
			int lStart = ipBlockAddress ? ipBlockAddress[lBlock] : lBlock;
			int lEnd = ipBlockAddress ? ipBlockAddress[lBlock + 1] : lBlock + 1;
			
			for (int lRow = lStart; lRow < lEnd; lRow++)
            {
                lFlag = ipDataFlags[lRow];
                if (lFlag == DssDataIgnoreCheckNull)
                    continue;
                
                lfAllSkipCheckNull = false;
                
                if(lFlag == DssDataOk || iNullCheckingOption == DssNumericErrorTreatAs)
				{
					lnTotal ++;
				}
            }
				
		}
	}
	
	if (lnTotal == 0)
    {
        *opDataFlag = DssDataInvalid;
        if (lfAllSkipCheckNull)
            *opDataFlag = DssDataIgnoreCheckNull;
    }
	else
		*opResult = lResult / lnTotal;
	
	return S_OK;
}

// qjiang 08/20/2009
// direct calculate for median
// the old approach first sorts the column, then get the middle element
// we can avoid the sorting, using a similar way as QuickSort, but just find the "middle" element
template <class InputDataType>
int DSSSumFunction::hMedianImpl(int iSize, int * ipGroupStart, int *ipBlockAddress, InputDataType* ipData, DSSDataFlag *ipDataFlags, bool iAllDataOK, 
								 EnumNullCheckingOption iNullCheckingOption, double* opResult, DSSDataFlag* opDataFlag)
{
	*opResult = 0;
	*opDataFlag = DssDataOk;
	
//	int hr = S_OK;
	
	if (iSize == 0)
	{
		*opDataFlag = DssDataInvalid;
		return S_OK;
	}
	
	std::vector<InputDataType> lvData;
	
	bool lErrorSkip = (iNullCheckingOption == DssNumericErrorSkip);
	
    bool lfAllSkipCheckNull = true;
    
	for (int i = 0; i < iSize; i++)
	{
		int lBlock = ipGroupStart[i];
		
		int lStart = ipBlockAddress ? ipBlockAddress[lBlock] : lBlock;
		int lEnd = ipBlockAddress ? ipBlockAddress[lBlock + 1] : lBlock + 1;
		
		for (int lRow = lStart; lRow < lEnd; lRow++)
		{
            DSSDataFlag lFlag = ipDataFlags[lRow];
            if (lFlag == DssDataIgnoreCheckNull)
                continue;
            
            lfAllSkipCheckNull = false;
            
			if (lFlag != DssDataOk && lErrorSkip)
				continue;
			
			lvData.push_back(ipData[lRow]);
		}
	}
	
	int lSize = lvData.size();
	if (lSize == 0)
	{
		*opDataFlag = DssDataInvalid;
        
        if (lfAllSkipCheckNull)
            *opDataFlag = DssDataIgnoreCheckNull;
	}
	else
	{
		InputDataType* lpData = &lvData[0];
		int lIdx = lSize / 2;
		
		// this operation will move the "lIdx"th element to its real position, making all the left lIdx-1 elements less/lessEqual than it
		double lResult = QuickPosition(lSize, lpData, lIdx);
		if (lSize % 2 == 0)
		{// the length of the array is even, need to find the middle 2 elements.
			// here we only need to care the left part, and find the max
			InputDataType* p = std::max_element(lpData, lpData + lIdx);
			
			lResult += *p;
			lResult /= 2;
		}
		*opResult = lResult;
	}
	
	return S_OK;
}

template <class InputDataType>
int DSSSumFunction::hProductImpl(int iSize, int * ipGroupStart, int *ipBlockAddress,  InputDataType* ipData, double iDivider, DSSDataFlag* ipDataFlags, bool iAllDataOK, 
								  EnumNullCheckingOption iNullCheckingOption, double* opResult, DSSDataFlag* opDataFlag)
{
	*opResult = 0;
	*opDataFlag = DssDataOk;
	
//	int hr = S_OK;
	
	if (iSize == 0)
	{
		*opDataFlag = DssDataInvalid;
		return S_OK;
	}
	
	bool lfConvert = (iDivider > 1);
	bool lErrorSkip = (iNullCheckingOption == DssNumericErrorSkip);
	
	double lResult = 1;
	bool lStopCalculation = false;
	bool lHasValidInput = false;
    bool lfAllSkipCheckNull = true;
    
	for (int i = 0; i < iSize; i++)
	{
		int lBlock = ipGroupStart[i];
		
		int lStart = ipBlockAddress ? ipBlockAddress[lBlock] : lBlock;
		int lEnd = ipBlockAddress ? ipBlockAddress[lBlock + 1] : lBlock + 1;
		
		for (int lRow = lStart; lRow < lEnd; lRow++)
		{
            DSSDataFlag lFlag = ipDataFlags[lRow];
            if (lFlag == DssDataIgnoreCheckNull)
                continue;
            
            lfAllSkipCheckNull = false;
            
			if (lFlag != DssDataOk)
			{
				if (lErrorSkip)
					continue;
				else
				{// error treat as 0, then the result must be 0, no need to do further calculation
					lStopCalculation = true;
					lResult = 0;
					break;
				}
			}
			lHasValidInput = true;
			lResult *= ipData[lRow];
			
			// convert back here to avoid the result becoming too huge
			if (lfConvert)
				lResult /= iDivider;
		}
		if (lStopCalculation)
			break;
	}
	
	if (lHasValidInput)
	{
		*opResult = lResult;
	}
	else
	{
		*opResult = 0;
		
        if (lfAllSkipCheckNull)
            *opDataFlag = DssDataIgnoreCheckNull;
		else if (lErrorSkip)
			*opDataFlag = DssDataInvalid;
	}
	
	return S_OK;
};

int DSSSumFunction::setProperty(Int32 Index, CComVariant *pValue)
{
	int hr = S_FALSE;
	if(Index<0)
	{
		if(pValue->vt != VT_BOOL)
		{
			hr = VariantChangeType(pValue, pValue, 0, VT_BOOL);
			CHECK_HR;
		}
		switch (Index)
		{
            case -1:
                mDistinct = pValue->boolVal ? true : false;
                hr = S_OK;
                break;
            default:
                hr = S_FALSE;
		}
		
	}
    return hr;
}
//tliao, 822148, 2014/09/23, support cell format data calculation
int DSSSumFunction::CalculateCellData(Int32 nParam, CComVariant * pParam, CComVariant * pResult, DSSDataFlag * pFlag)
{
    int hr;
    
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

void DSSSumFunction::CalculateCellTypeAndFormat(Int32 nParam, EnumCellFormatDataTypes *pTypes, DSS_ID *pFormatIDs, EnumCellFormatDataTypes *oResultType, DSS_ID *oResultFormatID)
{
	switch (mFunctionIndex)
	{
		case DssFuncSum:
		case DssFuncAvg:
		case DssFuncMedian:
		{
			*oResultType=CellDataHelper::CalculateCellDataType(nParam,pTypes);
			*oResultFormatID=CellDataHelper::CalculateCellDataFormatID(nParam,pFormatIDs);
			break;
		}
            
		case DssFuncProduct:
		{
			Int32 i;
			*oResultType=DssCellFormatDataTypeMIX;
			*oResultFormatID=CellDataHelper::GetMixFormatID();
            
			for(i=0; i<nParam; ++i)
			{
				if(i==0)
					*oResultType=pTypes[0];
				else if(*oResultType!=pTypes[i]) // A*B or const*A or A*const
				{
					if(*oResultType==DssCellFormatDataTypeDouble) // const*A
						*oResultType=pTypes[i];
					else if(pTypes[i]!=DssCellFormatDataTypeDouble) // A*B
					{
						*oResultType=DssCellFormatDataTypeMIX;
						break;
					}
				}
				else if(pTypes[i]!=DssCellFormatDataTypeDouble) // A*A
				{
					*oResultType=DssCellFormatDataTypeMIX;
					break;
				}
			}
            
			for(i=0; i<nParam; ++i)
			{
				if(i==0)
					*oResultFormatID=pFormatIDs[0];
				else if(!ATL::InlineIsEqualGUID(*oResultFormatID,pFormatIDs[i]))  // A*B or A*const or const*A
				{
					if(InlineIsGUIDNULL(*oResultFormatID))  // const*A
						*oResultFormatID=pFormatIDs[i];
					else if(!InlineIsGUIDNULL(pFormatIDs[i]))  // A*B
					{
						*oResultFormatID=CellDataHelper::GetMixFormatID();
						break;
					}
				}
				else if(!InlineIsGUIDNULL(pFormatIDs[i]))  // A*A
				{
					*oResultFormatID=CellDataHelper::GetMixFormatID();
					break;
				}
			}
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
