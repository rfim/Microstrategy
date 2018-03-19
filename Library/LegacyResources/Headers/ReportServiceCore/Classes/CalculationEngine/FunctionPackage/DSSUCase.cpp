
//
//  DSSUCase.cpp
//  ReportServiceCore
//
//  Created by Yang Zhuo on 7/27/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//

#include "DSSUCase.h"
#import "DSSFunctionUtility.h"

DSSUCase::DSSUCase():mfTrimString(true)
{
}


DSSUCase::~DSSUCase()
{
}

int DSSUCase::Calculate (Int32 nParam, CComVariant *pParam, CComVariant *pResult, DSSDataFlag *pFlag)
{
    int hr = S_OK;
  	if(!pParam || !pResult || !pFlag) 
    {
		return E_POINTER;
    }
    
	if ( (pParam->vt & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT))
    {
		return E_FAIL;
    }
    
	if (nParam != 1) 
    {
		return E_FAIL;
    }
    
	getVariant(pParam, 1, *pResult);
    
    if(pResult->vt == VT_EMPTY)
	{
		VariantInit(pResult);
		*pFlag = DssDataOk;
		return S_OK;
	}
    
    // tehe, TQMS819814
    if (pResult->vt != VT_BSTR)	//the input has to be string
	{
        CComVariant v1;
        getVariant(pParam, 0, v1);
        
        Int32 lTypeValue;
		Int32 lSizeInt = sizeof(Int32);
		int hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeInteger, (BYTE*)&lTypeValue, &lSizeInt);
		CHECK_HR;
        
		EnumDSSDataType lDataType = (EnumDSSDataType)lTypeValue;
        DFCDataConversion::DFCVariantChange2BSTR(pResult, pResult, &lDataType);
	}
    
    if(mfTrimString)
	{
		Int32 lsLen = 0;
		hr = DFCDataConversion::VariantSize(pResult, DssDataTypeVarChar, &lsLen);
		CHECK_HR;
        
		BYTE lpStr[lsLen];
		hr = DFCDataConversion::ConvertFromVariant(pResult, DssDataTypeVarChar, lpStr, &lsLen);
		CHECK_HR;
        
		hr = VariantClear(pResult);
		if(hr != S_OK)
			return hr;
        
        char *lpcStr = (char *)lpStr;
        trimSpace(lpcStr);

		hr = DFCDataConversion::ConvertToVariant(lpStr, DssDataTypeVarChar, 0, pResult);
		CHECK_HR;
	}
    
    //tliao, 2/20/2014, 849088, we should use UTF8 string to do upper/lower, the bstrVal of Variant is not always UTF8
    Int32 lsLen = 0;
    hr = DFCDataConversion::VariantSize(pResult, DssDataTypeVarChar, &lsLen);
    CHECK_HR;

    int lTmpLen = lsLen;
    BYTE lpString[lTmpLen];
    hr = DFCDataConversion::ConvertFromVariant(pResult, DssDataTypeVarChar, lpString, &lsLen);
    CHECK_HR;
    
    hr = VariantClear(pResult);
    if(hr != S_OK)
        return hr;
    
    char *lpStr = (char *)lpString;

    //if(strcmp((char*)pResult->bstrVal, "") == 0)
    if(strcmp(lpStr, "") == 0)
	{
		*pFlag = DssDataInvalid;
		return S_OK;
	}
	   
	// convert pResult->bstrVal to the upper case or lower case
	if (mFunctionIndex == DssFuncUCase) { //upper
		//BSTRToUpper(pResult->bstrVal);
        //char *lpStr = (char *)(pResult->bstrVal);
        Int32 len = lpStr ? strnlen(lpStr, lTmpLen) : 0;
        for (Int32 i  = 0; i < len; i++)
        {
            lpStr[i] = toupper(lpStr[i]);
        }
	} else { //lower 
		//BSTRToLower(pResult->bstrVal);	
        //char *lpStr = (char *)(pResult->bstrVal);
        Int32 len = lpStr ? strnlen(lpStr, lTmpLen) : 0;
        for (Int32 i  = 0; i < len; i++)
        {
            lpStr[i] = tolower(lpStr[i]);
        }
	}
    
    hr = DFCDataConversion::ConvertToVariant(lpString, DssDataTypeVarChar, 0, pResult);
    CHECK_HR;
    
	*pFlag = DssDataOk;
	return  S_OK; 
}

int DSSUCase::setProperty (Int32 Index, CComVariant *pValue)
{
	return E_NOTIMPL;
}

int DSSUCase::GetOutputType (Int32 size, EnumDSSDataType * inputType, EnumDSSDataType *outputType)
{
 	if(outputType == NULL || inputType == NULL) 
    {
		return E_POINTER;
    }
    
	if (size != 1)
    {
		return E_FAIL;
    }
    //tliao, 899309, 5/26/2014, port server logic
//	if (*inputType != DssDataTypeVarChar && *inputType != DssDataTypeMBChar && *inputType != DssDataTypeUTF8Char)
//		return E_FAIL;
    
	*outputType = DssDataTypeVarChar;
	return S_OK;
}

int DSSUCase::Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag)
{
    return E_NOTIMPL;
}

int DSSUCase::SetStringCompareOption(VARIANT_BOOL iOpt)
{
	mfTrimString = (iOpt == VARIANT_TRUE);
    
	return S_OK;
}
