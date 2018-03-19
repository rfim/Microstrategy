//
//  DSSNullZero.cpp
//  ReportServiceCore
//
//  Created by Zhang Jia on 7/22/11.
//  Copyright 2011 MicroStrategy Beijing R&D Center. All rights reserved.
//

#include "DSSNullZero.h"

#import "DFCDataConversion.h"
#import "DSSFunctionUtility.h"

#define UNINITIALIZED 1
#define INPUTBEINGSETTED 2
#define CALCULATED 3

DSSNullZero::DSSNullZero(): mpData(NULL), mpInputFlag(NULL), mpResultFlag(NULL), mnDataSize(0), mStatus(1), mDataType(DssDataTypeUnknown), mpResult(NULL)
//ytang 2008-06-17
, mTreatMathErrorAsNull(true), mpNumericInputData(NULL), mfIsNumeric(false)
{
}

DSSNullZero::~DSSNullZero()
{
	Clear();
}

int DSSNullZero::setProperty (Int32 Index, CComVariant *pValue)
{
    return E_NOTIMPL;
}

int DSSNullZero::Calculate ()
{    
	int hr;
    
	if(mStatus == CALCULATED) 
		return S_OK;
    
	Int32 i = 0;
	switch(mFunctionIndex)
	{
        case DssFuncIsNull:
		{
			mpResult = new bool[mnDataSize];
			mpResultFlag = new DSSDataFlag[mnDataSize];
			if(!mpResult || !mpResultFlag) return E_OUTOFMEMORY;
            
			for(i = 0; i < mnDataSize; i++)
			{
				//ytang 2008-06-17				
				if (!mTreatMathErrorAsNull) 
                {
					mpResult[i] = (mpInputFlag[i] != DssDataOk)
                    &&(mpInputFlag[i] != DssDataOverflow);
				} 
                else 
                {					
					mpResult[i] = (mpInputFlag[i] != DssDataOk);
				}
                
				mpResultFlag[i] = DssDataOk;
			}
		}
            break;
            
        case DssFuncIsNotNull:
		{
			mpResult = new bool[mnDataSize];
			mpResultFlag = new DSSDataFlag[mnDataSize];
			if(!mpResult || !mpResultFlag) return E_OUTOFMEMORY;
            
			for(i = 0; i < mnDataSize; i++)
			{	
				// ytang 2008-06-17				
				if (!mTreatMathErrorAsNull) 
                {
					mpResult[i] = (mpInputFlag[i] == DssDataOk)
                    ||(mpInputFlag[i] == DssDataOverflow);
				} 
                else 
                {
					mpResult[i] = (mpInputFlag[i] == DssDataOk);
				}
                
				mpResultFlag[i] = DssDataOk;
			}
		}
            break;
            
        case DssFuncNullToZero:
		{
			mpResultFlag = new DSSDataFlag[mnDataSize];
			if(!mpResultFlag) return E_OUTOFMEMORY;
            
			for(i = 0; i < mnDataSize; i++)
			{
				if (mpInputFlag[i] !=DssDataOk)
				{
					//ctang 2010-6-1 TQMS 410488
					if (mfIsNumeric)
					{
						mpNumericInputData[i] = 0;
						mpResultFlag[i] = DssDataOk;
					}
					else
					{
						double lValue = 0;
						
						if (mDataType == DssDataTypeShort || mDataType == DssDataTypeInteger ||
							mDataType == DssDataTypeLong  || mDataType == DssDataTypeFloat   ||
							mDataType == DssDataTypeDouble || mDataType == DssDataTypeBool ) {
							//OnlDFCDataConversion for numerical data type xhan 03/27/00
							hr = DFCDataConversion::ConvertToVariant((unsigned char *)&lValue, mDataType, 0, &(mpData[i]));
							if(hr != S_OK) return hr;
                            
							mpResultFlag[i] = DssDataOk;
						} 
						else
							mpResultFlag[i] = DssDataInvalid;                        
					}
				}
				else
					mpResultFlag[i] = DssDataOk;
			}            
		}
            break;
            
        case DssFuncZeroToNull:
		{
			mpResultFlag = new DSSDataFlag[mnDataSize];
			if(!mpResultFlag) return E_OUTOFMEMORY;
            
			for(i = 0; i < mnDataSize; i++)
			{
				
				Int32 lDoubleSize = sizeof(double);
				double lValue;
                
				if(mpInputFlag[i] == DssDataOk)
				{
					//ctang 2010-6-1 TQMS410488
					if (mfIsNumeric)
					{
						mpResultFlag[i] = mpNumericInputData[i] == 0 ? DssDataInvalid : DssDataOk;
					}
					else
					{
						hr = DFCDataConversion::ConvertFromVariant(&(mpData[i]), 
                                                                   DssDataTypeDouble, 
                                                                   (BYTE *)&lValue, 
                                                                   &lDoubleSize);
                        if (S_OK == hr && 0 == lValue)
							mpResultFlag[i] = DssDataInvalid;
						else
							mpResultFlag[i] = DssDataOk;
					}
				}
				else
					mpResultFlag[i] = mpInputFlag[i];
			}
		}
            break;
            
        default:
            return E_FAIL;
	}
    
	mStatus = CALCULATED;
    
	return S_OK;
}

int DSSNullZero::GetOutput (Int32 nSize, Int32 *nDataSize, double *pData, DSSDataFlag *pFlag)
{
	if(!nDataSize || !pData || !pFlag) 
		return E_POINTER;
    
	if (nSize < mnDataSize) 
		return E_FAIL;
    
	LONG i;
	switch(mFunctionIndex)
	{
        case DssFuncIsNull:
        case DssFuncIsNotNull:
		{            
			for(i = 0; i < mnDataSize; i++)
			{
				pData[i] = mpResult[i];
				pFlag[i] = mpResultFlag[i];
			}
            
			*nDataSize = mnDataSize;
		}
            break;
            
        case DssFuncNullToZero:
        case DssFuncZeroToNull:
		{
			for(i = 0; i < mnDataSize; i++)
			{
				pData[i] = mpNumericInputData[i];
				pFlag[i] = mpResultFlag[i];
			}
            
			*nDataSize = mnDataSize;
		}
            break;
        default:
            return S_FALSE;
	}
    
	return S_OK;
}

int DSSNullZero::GetOutput (Int32 nSize, Int32 *nDataSize, CComVariant *pData, DSSDataFlag *pFlag)
{
	if(!nDataSize || !pData || !pFlag) 
		return E_POINTER;
    
	if (nSize < mnDataSize) 
		return E_FAIL;
    
	pData->vt = VT_ARRAY|VT_VARIANT;
	SAFEARRAYBOUND lSABound;
	lSABound.lLbound = 0;
	lSABound.cElements = mnDataSize;
	pData->parray = SafeArrayCreate(VT_VARIANT, 1, &lSABound);
	
	LONG i;
	switch(mFunctionIndex)
	{
        case DssFuncIsNull:
        case DssFuncIsNotNull:
		{            
			for(i = 0; i < mnDataSize; i++)
			{
				CComVariant lElement;	
				if(mpResultFlag[i] == DssDataOk)
					DFCDataConversion::ConvertToVariant((unsigned char *)(&(mpResult[i])), DssDataTypeBool, 0, &lElement);
                
				SafeArrayPutElement(pData->parray, &i, &lElement);
                
				pFlag[i] = mpResultFlag[i];
                
                
			}
            
			*nDataSize = mnDataSize;
		}
            break;
            
        case DssFuncNullToZero:
        case DssFuncZeroToNull:
		{
			for(i = 0; i < mnDataSize; i++)
			{
				SafeArrayPutElement(pData->parray, &i, &(mpData[i]));
				pFlag[i] = mpResultFlag[i];
			}
            
			*nDataSize = mnDataSize;
		}
            break;  
        default:
            return S_FALSE;
	}
    
	return S_OK;
}

//ctang 2010-6-1
int DSSNullZero::GetOutputType(EnumDSSParameterType *pType)
{
	*pType = DssParameterVector;
    
	return S_OK;
}

int DSSNullZero::GetOutputType (Int32 nSize, EnumDSSDataType *pInputType, EnumDSSDataType *pOutputDataType, EnumDSSParameterType *pOutputParamType)
{
	//ytang 2008-06-24
	if(nSize != 1 && nSize != 2) return E_INVALIDARG;
    
	if(!pOutputDataType || !pOutputParamType) return E_POINTER;
    
	switch(mFunctionIndex)
	{
        case DssFuncIsNull:
        case DssFuncIsNotNull:
            *pOutputDataType = DssDataTypeBool;
            break;
        case DssFuncNullToZero:
        case DssFuncZeroToNull:
            *pOutputDataType = pInputType[0];
            break;
        default:
            return S_FALSE;
	}
    
	*pOutputParamType = DssParameterVector;
    
	return S_OK;
}

//ctang 2010-6-1
int DSSNullZero::SetInputParam(int nParamIndex, int nSize, double *pData, DSSDataFlag *pFlag)
{
	if(nParamIndex != 1 && nParamIndex != 2) return E_INVALIDARG;
    
	if(nSize < 1) return E_INVALIDARG;
    
	//ytang 2008-06-18
	if (nParamIndex == 2) 
    {
		double lValue = *pData;
        
		if(lValue != 0)
			mTreatMathErrorAsNull = false;
        
		return S_OK;
	}
    
	mDataType = DssDataTypeDouble;
	mnDataSize = nSize;
    
	mpInputFlag = new DSSDataFlag[nSize];
	if(!mpInputFlag) return E_OUTOFMEMORY;
    
	// remember the flags of input data
	LONG i;
	for(i = 0; i < nSize; i++)
		mpInputFlag[i] = pFlag[i];
    
	switch(mFunctionIndex)
	{
            // for IsNull and IsNotNull, we don't care the data itself
        case DssFuncIsNull:
        case DssFuncIsNotNull:
            break; 
            
            // for NullToZero and ZeroToNull, copy all the input data
        case DssFuncNullToZero:
        case DssFuncZeroToNull:
		{
			mpNumericInputData = new double[nSize];
			if(!mpNumericInputData) return E_OUTOFMEMORY;
            
			for(i = 0; i < nSize; i++)
			{
				if(pFlag[i] == DssDataOk) 
					mpNumericInputData[i] = pData[i];
			}
            
		}
            break;
            
        default:
            return E_FAIL;
	}
    
	mfIsNumeric = true;
    
	mStatus = INPUTBEINGSETTED;
    
	return S_OK;
}

int DSSNullZero::SetInputParam (Int32 nParamIndex, Int32 nSize, EnumDSSDataType DataType, CComVariant *pData, DSSDataFlag *pFlag)
{
	//ytang 2008-06-23
	if(nParamIndex != 1 && nParamIndex != 2) return E_INVALIDARG;
    
	
    if(nSize < 1) return E_INVALIDARG;
    
	//ytang 2008-06-18
	if (nParamIndex == 2) {
		Int32 lDoubleSize = sizeof(double);
		double lValue;
		
		int hr = DFCDataConversion::ConvertFromVariant(pData, 
                                                       DataType, 
                                                       (BYTE *)&lValue, 
                                                       &lDoubleSize);
		CHECK_HR;
        
		if(lValue != 0)
			mTreatMathErrorAsNull = false;
        
		return S_OK;
	}
    
	mDataType = DataType;
	mnDataSize = nSize;
    
	mpInputFlag = new DSSDataFlag[nSize];
	if(!mpInputFlag) return E_OUTOFMEMORY;
    
	// remember the flags of input data
	LONG i;
	for(i = 0; i < nSize; i++)
		mpInputFlag[i] = pFlag[i];
    
	switch(mFunctionIndex)
	{
            // for IsNull and IsNotNull, we don't care the data itself
        case DssFuncIsNull:
        case DssFuncIsNotNull:
            break; 
            
            // for NullToZero and ZeroToNull, copy all the input data
        case DssFuncNullToZero:
        case DssFuncZeroToNull:
		{
			mpData = new CComVariant[nSize];
			if(!mpData) return E_OUTOFMEMORY;
            
			for(i = 0; i < nSize; i++)
			{
				CComVariant lElement;	
				if(pFlag[i] == DssDataOk) 
					SafeArrayGetElement(pData->parray, &i, &lElement);
                
				VariantCopy(&(mpData[i]), &lElement);                
			}
            
		}
            break;
            
        default:
            return E_FAIL;
	}
    
	mStatus = INPUTBEINGSETTED;
    
	return S_OK;
}

int DSSNullZero::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
	if(!pType) return E_POINTER;
    
	if(nParamIndex == 1)
		*pType = DssParameterVector;
	else
		*pType = DssParameterReserved;
    
	return S_OK;
}

int DSSNullZero::Clear ()
{
	if(mpData)
	{
		delete [] mpData;
		mpData = NULL;
	}
	
	if(mpInputFlag)
	{
		delete [] mpInputFlag;
		mpInputFlag = NULL;
	}
	
	if(mpResultFlag)
	{
		delete [] mpResultFlag;
		mpResultFlag = NULL;
	}
	
	mnDataSize = 0;
	mDataType = DssDataTypeUnknown;
	
	if(mpResult)
	{
		delete [] mpResult;
		mpResult = NULL;
	}
    
	if (mpNumericInputData)
	{
		delete [] mpNumericInputData;
		mpNumericInputData = NULL;
	}
    
	mStatus = UNINITIALIZED;
    
	return S_OK;
}

int DSSNullZero::IsNullProcessable (bool *pCanProcessNULL)
{
	if(!pCanProcessNULL) return E_POINTER;
    
	*pCanProcessNULL = true; // always assume input data is valid
    
	return S_OK;
}
// 3/21/2012 tehe
int DSSNullZero::Init (EnumSimpleOperatorType iFunctionIndex)
{
	mFunctionIndex = iFunctionIndex;
    
	mStatus = UNINITIALIZED;
    
	return S_OK;
}

// jxie, 2010-9-9
// support direct calculate on all functions in this class 
bool DSSNullZero::SupportDirectCalculate()
{
	return true;
}

// jxie, 2010-9-9
// support direct calculate function
int DSSNullZero::DirectCalculate(Int32 nParam, Int32* ipSize, 
                                 void** ipData, double* ipDivider, DSSDataFlag** ipFlags, VARIANT_BOOL* ipAllDataValid,
                                 EnumDSSDataType* iType, vector<Int32, MBase::Allocator<Int32> > * ipvDataByGroup,
                                 Int32 nResSize, double* opData, DSSDataFlag* opFlag, VARIANT_BOOL* opAllDataValid, EnumNullCheckingOption iNullCheckingOption)
{
	/*
     It's caller's responsibility to make sure all parameters are valid.
     */    
	AE_ASSERT(nParam <= 2);
    
	//ytang 2008-06-18
	if (nParam == 2)
	{
		double lValue = GetNumericData(ipData[1], 0, iType[1]);
		
		if(lValue != 0)
			mTreatMathErrorAsNull = false;
	}
    
	*opAllDataValid = VARIANT_TRUE;
	if (ipvDataByGroup != NULL)
		nResSize = ipvDataByGroup->size();
    
	DSSDataFlag* lFlags = ipFlags[0];
	void* lpData = ipData[0];
	EnumDSSDataType lType = iType[0];
    
	Int32 i = 0;
	Int32 lRow = 0;
	switch(mFunctionIndex)
	{
        case DssFuncIsNull:
		{
			if(!opData || !opFlag) return E_OUTOFMEMORY;
            
			for(i = 0; i < nResSize; i++)
			{
				if (ipvDataByGroup != NULL)
					lRow = (*ipvDataByGroup)[i];
				else
					lRow = i;
                
				//ytang 2008-06-17				
				if (!mTreatMathErrorAsNull) {
					opData[lRow] = (lFlags[lRow] != DssDataOk)
                    &&(lFlags[lRow] != DssDataOverflow);
				} else {					
					opData[lRow] = (lFlags[lRow] != DssDataOk);
				}
                
				opFlag[lRow] = DssDataOk;
			}
		}
            break;
            
        case DssFuncIsNotNull:
		{
			if(!opData || !opFlag) return E_OUTOFMEMORY;
            
			for(i = 0; i < nResSize; i++)
			{	
				if (ipvDataByGroup != NULL)
					lRow = (*ipvDataByGroup)[i];
				else
					lRow = i;
                
				// ytang 2008-06-17				
				if (!mTreatMathErrorAsNull) {
					opData[lRow] = (lFlags[lRow] == DssDataOk)
                    ||(lFlags[lRow] == DssDataOverflow);
				} else {
					opData[lRow] = (lFlags[lRow] == DssDataOk);
				}
                
				opFlag[lRow] = DssDataOk;
			}
		}
            break;
            
        case DssFuncNullToZero:
		{
			if(!opFlag) return E_OUTOFMEMORY;
            
			for(i = 0; i < nResSize; i++)
			{
				if (ipvDataByGroup != NULL)
					lRow = (*ipvDataByGroup)[i];
				else
					lRow = i;
                
				if (lFlags[lRow] != DssDataOk)
					// should be numeric in direct calculate function
					opData[lRow] = 0;
				else
					opData[lRow] = GetNumericData(lpData, lRow, lType);
                
				opFlag[lRow] = DssDataOk;
			}
            
		}
            break;
            
        case DssFuncZeroToNull:
		{
			if(!opFlag) return E_OUTOFMEMORY;
            
			double lData = 0;
			for(i = 0; i < nResSize; i++)
			{
				if (ipvDataByGroup != NULL)
					lRow = (*ipvDataByGroup)[i];
				else
					lRow = i;
                
				lData = GetNumericData(lpData, lRow, lType);
				if(lFlags[lRow] == DssDataOk)
					// should be numeric in direct calculate function
					opFlag[lRow] = (lData == 0) ? DssDataInvalid : DssDataOk;
				else
					opFlag[lRow] = lFlags[lRow];
                
				if (opFlag[lRow] != DssDataOk)
					*opAllDataValid = VARIANT_FALSE;
                
				opData[lRow] = lData;
			}
		}
            break;
            
        default:
            return E_FAIL;
	}
    
	return S_OK;
}