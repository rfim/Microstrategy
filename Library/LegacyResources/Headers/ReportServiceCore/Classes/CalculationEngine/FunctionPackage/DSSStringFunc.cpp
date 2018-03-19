//
//  DSSStringFunc.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 7/3/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#include "DSSStringFunc.h"

#include "DSSFunctionUtility.h"
#include "Base/Base/BaseString.h"
#include "../../common/DSSStrongPtr.h"
#import <SimpleBuffer.h>

DSSStringFunc::DSSStringFunc(): mfTrimString(true)
{
    mFunctionIndex = DssFuncUnknown;
}

DSSStringFunc::~DSSStringFunc()
{
}

int DSSStringFunc::Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag)
{
    return E_NOTIMPL;
}

// deliu, 2012-04-12
/****************************************************/
/*		nParam:  num of values						*/
/*		pParam: DataType first, then Value			*/
/*      pResult: Output Value						*/
/****************************************************/
int DSSStringFunc::Calculate (int nParam,CComVariant *pParam, CComVariant *pResult, DSSDataFlag *pFlag)
{
  	if(!pParam || !pResult || !pFlag) 
		return E_POINTER;
    
	if ( (pParam->vt & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT))
		return E_FAIL;
    
	if (nParam < 1) 
		return E_FAIL;
    
	int hr = S_OK;
    // 7/4/2012 tehe 
//	MBase::BufferPtr lBufferPtr(MBase::CreateBuffer(1, 1));
//	MBase::Buffer *lpBuffer = lBufferPtr.Get();
    MBase::SimpleBuffer lSimpleBuffer;
	MBase::BufferPtr lBufferPtr(&lSimpleBuffer);
	MBase::Buffer *lpBuffer = lBufferPtr.Get();
	CHECK_NEWED_PTR(lpBuffer);
    
	switch (mFunctionIndex)
	{
        case DssFuncConcat:
		{
			int lMaxBuffer = 256;

            std::string lConcatStr;
            
            // 7/4/2012 tehe 
			BYTE *lpStr = NULL;
            //BYTE *lpStr = new BYTE[lMaxBuffer];
            //MBase::DSSStrongPtr<BYTE, MBase::DeleteArray<BYTE> > pStr;
            
			for(LONG i = 0; i < nParam; i++)
			{
				LONG index = 2 * i;
				CComVariant v1, v2;
				SafeArrayGetElement(pParam->parray, &index, &v1);
                
				index = 2 * i + 1;
				SafeArrayGetElement(pParam->parray, &index, &v2);
                
				if (v2.vt == VT_EMPTY)
				{
                    lConcatStr = lConcatStr + " ";
					continue;
				}
                
				if (v2.vt != VT_BSTR)
				{
					// deliu, 2012-06-21, TQMS 615084/616431/617117
					int lTypeValue;
					int lSizeInt = sizeof(int);
					hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeInteger, (BYTE*)&lTypeValue, &lSizeInt);
					CHECK_HR;
                    
					EnumDSSDataType lDataType = (EnumDSSDataType)lTypeValue;
					DFCDataConversion::DFCVariantChange2BSTR(&v2, &v2, &lDataType);
				}
                
				int lsLen = 0;
				hr = DFCDataConversion::VariantSize(&v2, DssDataTypeVarChar, &lsLen);
				CHECK_HR;
                
				if(lsLen <= 0)
				{
                    lConcatStr = lConcatStr + " ";
					continue;
				}
                
				if(lsLen > lMaxBuffer)
				{
					lMaxBuffer = lsLen;
                    // 7/4/2012 tehe
					//lpStr = new (lpBuffer) BYTE[lMaxBuffer];
                    //pStr.Reset(lptmpStr);
				}
                lpStr = new (lpBuffer) BYTE[lMaxBuffer];
                
				hr = DFCDataConversion::ConvertFromVariant(&v2, DssDataTypeVarChar, lpStr, &lsLen);
				CHECK_HR;
                
                char* lpcStr = (char*)lpStr;
                if(mfTrimString)
                    trimSpace(lpcStr);
                string lStr(lpcStr);
                
				if(i < nParam - 1)
                    lConcatStr = lConcatStr + lStr + " ";
				else
					lConcatStr = lConcatStr + lStr;
			}
            
			hr = DFCDataConversion::ConvertToVariant((BYTE *)lConcatStr.c_str(), DssDataTypeVarChar, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncConcatNoBlank:
		{
			int lMaxBuffer = 256;
            
            std::string lConcatStr;
            // 7/4/2012 tehe
//			BYTE *lpStr = NULL;
            //BYTE *lpStr = new BYTE[lMaxBuffer];
            //MBase::DSSStrongPtr<BYTE, MBase::DeleteArray<BYTE> > pStr;
            
			for(LONG i = 0; i < nParam; i++)
			{
				LONG index = 2 * i;
				CComVariant v1, v2;
				SafeArrayGetElement(pParam->parray, &index, &v1);
                
				index = 2 * i + 1;
				SafeArrayGetElement(pParam->parray, &index, &v2);
                
				if (v2.vt == VT_EMPTY)	continue;
                
                int lTypeValue = DssDataTypeUnknown;
				if (v2.vt != VT_BSTR)
				{
					int lSizeInt = sizeof(int);
					hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeInteger, (BYTE*)&lTypeValue, &lSizeInt);
					CHECK_HR;
                    
					EnumDSSDataType lDataType = (EnumDSSDataType)lTypeValue;
					
                    DFCDataConversion::DFCVariantChange2BSTR(&v2, &v2, &lDataType);
				}
                
				int lsLen = 0;
				hr = DFCDataConversion::VariantSize(&v2, DssDataTypeVarChar, &lsLen);
				CHECK_HR;
                
				if(lsLen <= 0)
					continue;
                
				if(lsLen > lMaxBuffer)
				{
					lMaxBuffer = lsLen;
					// lpStr = new (lpBuffer) BYTE[lMaxBuffer];
                    // 7/4/2012 tehe
					
                    //pStr.Reset(lptmpStr);
				}
                BYTE *lpStr = new BYTE[lMaxBuffer];
                
				hr = DFCDataConversion::ConvertFromVariant(&v2, DssDataTypeVarChar, lpStr, &lsLen);
				CHECK_HR;

                char* lpcStr = (char*)lpStr;
                if(mfTrimString)
                    trimSpace(lpcStr);
                string lStr(lpcStr);

                lConcatStr = lConcatStr + lStr;
			}
            
			if(lConcatStr.length() == 0)
			{
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            
			hr = DFCDataConversion::ConvertToVariant((BYTE *)lConcatStr.c_str(), DssDataTypeVarChar, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncSubStr:
		{
			if(nParam < 2) 
                return E_FAIL; // 7/4/2012 tehe
            
			CComVariant v[6];
			for(LONG index = 0; index < 2 * nParam; index++)
				SafeArrayGetElement(pParam->parray, &index, &v[index]);
            
			if(v[1].vt == VT_EMPTY || v[3].vt == VT_EMPTY)
			{
				VariantInit(pResult);
				*pFlag = DssDataOk;
				return S_OK;
			}
            
			if(v[1].vt != VT_BSTR)
			{
				int lTypeValue;
				int lSizeInt = sizeof(int);
				hr = DFCDataConversion::ConvertFromVariant(&v[0], DssDataTypeInteger, (BYTE*)&lTypeValue, &lSizeInt);
				CHECK_HR;
                
				EnumDSSDataType lDataType = (EnumDSSDataType)lTypeValue;
				DFCDataConversion::DFCVariantChange2BSTR(&v[1], &v[1], &lDataType);
			}
            
			int lsLen = 0;
			hr = DFCDataConversion::VariantSize(&v[1], DssDataTypeVarChar, &lsLen);
			CHECK_HR;
            
			BYTE *lpStr = new (lpBuffer) BYTE[lsLen];
            // 7/4/2012 tehe
			//BYTE *lpStr = new BYTE[lsLen];
            //MBase::DSSStrongPtr<BYTE, MBase::DeleteArray<BYTE> > pStr;
            
			hr = DFCDataConversion::ConvertFromVariant(&v[1], DssDataTypeVarChar, lpStr, &lsLen);
			CHECK_HR;
            
			char *lpcStr = (char *)lpStr;
			if(mfTrimString)
				trimSpace(lpcStr);
            
			if(strcmp(lpcStr, "") == 0)	// Empty string
			{
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            std::string lStr(lpcStr);
            
			int lPosition = 0;
			int lLength = -1;
			//int lSizeInt = sizeof(int);
            
			int lPosTypeValue;
			int lSize = sizeof(int);
			hr = DFCDataConversion::ConvertFromVariant(&v[2], DssDataTypeInteger, (BYTE*)&lPosTypeValue, &lSize);
			CHECK_HR;
            
			EnumDSSDataType lPosDataType = (EnumDSSDataType)lPosTypeValue;
			hr = hGetValueInt(&v[3], lPosDataType, &lPosition, true);
			// deliu, 2012-06-07, TQMS 608058/610601, return XTabNull for non-numerical parameter
			if (hr != S_OK)
			{
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            
			// if position is zero, then it is treated as 1
			lPosition = (lPosition == 0 ? 1 : lPosition);
            
			// if position is negative, then count backward from the end
            // 11/20/2012 tehe
			// int lStrLength = ( lsLen / sizeof(wchar_t) ) - 1;		
			int lStrLength = lStr.length();
            lPosition = (lPosition < 0 ? lPosition + lStrLength + 1 : lPosition);
            
            
			if(lPosition <= 0 || (lPosition > lStrLength))
			{
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            
			if(nParam == 2)		// default, return the whole string
				lLength = lStrLength;
			else
			{
				int lLenTypeValue;
				hr = DFCDataConversion::ConvertFromVariant(&v[4], DssDataTypeInteger, (BYTE*)&lLenTypeValue, &lSize);
				CHECK_HR;
                
				EnumDSSDataType lLenDataType = (EnumDSSDataType)lLenTypeValue;
				hr = hGetValueInt(&v[5], lLenDataType, &lLength, true);
				if ( (hr != S_OK) || (lLength <= 0) )	// Invalid if length less than 0
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}
			}

            std::string lSubStr = lStr.substr(lPosition - 1, lLength);		// position start from 1
            
			hr = DFCDataConversion::ConvertToVariant((BYTE *)lSubStr.c_str(), DssDataTypeVarChar, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncLeftStr:
		{
			if(nParam != 2) 
                return E_FAIL; // 7/4/2012 tehe
            
			CComVariant v[4];
			for(LONG index = 0; index < 2 * nParam; index++)
				SafeArrayGetElement(pParam->parray, &index, &v[index]);
            
			if(v[1].vt == VT_EMPTY || v[3].vt == VT_EMPTY)
			{
				VariantInit(pResult);
				*pFlag = DssDataOk;
				return S_OK;
			}
            
			if(v[1].vt != VT_BSTR)
			{
				int lTypeValue;
				int lSizeInt = sizeof(int);
				hr = DFCDataConversion::ConvertFromVariant(&v[0], DssDataTypeInteger, (BYTE*)&lTypeValue, &lSizeInt);
				CHECK_HR;
                
				EnumDSSDataType lDataType = (EnumDSSDataType)lTypeValue;
				DFCDataConversion::DFCVariantChange2BSTR(&v[1], &v[1], &lDataType);
			}
            
			int lsLen = 0;
			hr = DFCDataConversion::VariantSize(&v[1], DssDataTypeVarChar, &lsLen);
			CHECK_HR;
            
			BYTE *lpStr = new (lpBuffer) BYTE[lsLen];
            // 7/4/2012 tehe
			//BYTE *lpStr = new BYTE[lsLen];
            //MBase::DSSStrongPtr<BYTE, MBase::DeleteArray<BYTE> > pStr;
            
			hr = DFCDataConversion::ConvertFromVariant(&v[1], DssDataTypeVarChar, lpStr, &lsLen);
			CHECK_HR;
            
            char* lpcStr = (char*)lpStr;
            if(mfTrimString)
				trimSpace(lpcStr);
            if(strcmp(lpcStr, "") == 0)
			{
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            
			std::string lStr(lpcStr);
            
			int lLength = -1;
			int lLenTypeValue;
			int lSize = sizeof(int);
			hr = DFCDataConversion::ConvertFromVariant(&v[2], DssDataTypeInteger, (BYTE*)&lLenTypeValue, &lSize);
			CHECK_HR;
            
			EnumDSSDataType lLenDataType = (EnumDSSDataType)lLenTypeValue;
			hr = hGetValueInt(&v[3], lLenDataType, &lLength, true);
			if ( (hr != S_OK) || (lLength <= 0) )
			{
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            // 11/20/2012 tehe
			// int lStrLength = (lsLen / sizeof(wchar_t) ) - 1;
            int lStrLength = lStr.length();
			lLength = lLength > lStrLength ? lStrLength : lLength;

            std::string lSubStr = lStr.substr(0, lLength);
           
			hr = DFCDataConversion::ConvertToVariant((BYTE *)lSubStr.c_str(), DssDataTypeVarChar, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncRightStr:
		{
			if(nParam != 2) 
                return E_FAIL; // 7/4/2012 tehe
            
			CComVariant v[4];
			for(LONG index = 0; index < 2 * nParam; index++)
				SafeArrayGetElement(pParam->parray, &index, &v[index]);
            
			if(v[1].vt == VT_EMPTY || v[3].vt == VT_EMPTY)
			{
				VariantInit(pResult);
				*pFlag = DssDataOk;
				return S_OK;
			}
            
			if(v[1].vt != VT_BSTR)
			{
				int lTypeValue;
				int lSizeInt = sizeof(int);
				hr = DFCDataConversion::ConvertFromVariant(&v[0], DssDataTypeInteger, (BYTE*)&lTypeValue, &lSizeInt);
				CHECK_HR;
                
				EnumDSSDataType lDataType = (EnumDSSDataType)lTypeValue;
				DFCDataConversion::DFCVariantChange2BSTR(&v[1], &v[1], &lDataType);
			}
            
			int lsLen = 0;
			hr = DFCDataConversion::VariantSize(&v[1], DssDataTypeVarChar, &lsLen);
			CHECK_HR;
            
			BYTE *lpStr = new (lpBuffer) BYTE[lsLen];
            // 7/4/2012 tehe
			//BYTE *lpStr = new BYTE[lsLen];
            //MBase::DSSStrongPtr<BYTE, MBase::DeleteArray<BYTE> > pStr;
            
			hr = DFCDataConversion::ConvertFromVariant(&v[1], DssDataTypeVarChar, lpStr, &lsLen);
			CHECK_HR;
            
            char* lpcStr = (char*)lpStr;
            if(mfTrimString)
				trimSpace(lpcStr);
            if(strcmp(lpcStr, "") == 0)
			{
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            
			std::string lStr(lpcStr);
			int lLength = -1;
			int lLenTypeValue;
			int lSize = sizeof(int);
			hr = DFCDataConversion::ConvertFromVariant(&v[2], DssDataTypeInteger, (BYTE*)&lLenTypeValue, &lSize);
			CHECK_HR;
            
			EnumDSSDataType lLenDataType = (EnumDSSDataType)lLenTypeValue;
			hr = hGetValueInt(&v[3], lLenDataType, &lLength, false);
			if (hr != S_OK)
			{
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            
            // 11/20/2012 tehe
			// int lStrLength = (lsLen / sizeof(wchar_t) ) - 1;
            int lStrLength = lStr.length();
			if(lLength <= 0 || lLength > 2 * lStrLength + 1)
			{
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            
			lLength = lLength > lStrLength ? (lLength - 1) : lLength;
			lLength = lLength > lStrLength ? (lLength - lStrLength) : lLength;

            std::string lSubStr = lStr.substr(lStrLength - lLength, lLength);

			hr = DFCDataConversion::ConvertToVariant((BYTE *)lSubStr.c_str(), DssDataTypeVarChar, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncTrim:
		{
			if(nParam != 1) 
                return E_FAIL; // 7/4/2012 tehe
            
			CComVariant v;
			getVariant(pParam, 1, v);
            
			if(v.vt == VT_EMPTY)
			{
				VariantInit(pResult);
				*pFlag = DssDataOk;
				return S_OK;
			}
            
			if(v.vt != VT_BSTR)
			{
				CComVariant v1;
				getVariant(pParam, 0, v1);
                
				int lTypeValue;
				int lSizeInt = sizeof(int);
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeInteger, (BYTE*)&lTypeValue, &lSizeInt);
				CHECK_HR;
                
				EnumDSSDataType lDataType = (EnumDSSDataType)lTypeValue;
				DFCDataConversion::DFCVariantChange2BSTR(&v, &v, &lDataType);
			}
            
			int lsLen = 0;
			hr = DFCDataConversion::VariantSize(&v, DssDataTypeVarChar, &lsLen);
			CHECK_HR;
            
			BYTE *lpStr = new (lpBuffer) BYTE[lsLen];
            // 7/4/2012 tehe
			//BYTE *lpStr = new BYTE[lsLen];
            //MBase::DSSStrongPtr<BYTE, MBase::DeleteArray<BYTE> > pStr;
            
			hr = DFCDataConversion::ConvertFromVariant(&v, DssDataTypeVarChar, lpStr, &lsLen);
			CHECK_HR;
            
            char* lpcStr = (char*)lpStr;
            if(mfTrimString)
				trimSpace(lpcStr);
            if(strcmp(lpcStr, "") == 0)
			{
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            leftTrimSpace(&lpcStr);
			trimSpace(lpcStr);
			std::string lStr(lpcStr);

			hr = DFCDataConversion::ConvertToVariant((BYTE *)lStr.c_str(), DssDataTypeVarChar, 0, pResult);
			CHECK_HR;
		}
            break;
            
            //case DssFuncUCase:		// implemented in CDSSUCase
            //	break;
            //case DssFuncLCase:
            //	break;
            
        case DssFuncInitCap:
		{
			if(nParam != 1) 
                return E_FAIL; // 7/4/2012 tehe
            
			getVariant(pParam, 1, *pResult);
            
			if(pResult->vt == VT_EMPTY)
			{
				VariantInit(pResult);
				*pFlag = DssDataOk;
				return S_OK;
			}
            
			if(pResult->vt != VT_BSTR)
			{
				CComVariant v1;
				getVariant(pParam, 0, v1);
                
				int lTypeValue;
				int lSizeInt = sizeof(int);
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeInteger, (BYTE*)&lTypeValue, &lSizeInt);
				CHECK_HR;
                
				EnumDSSDataType lDataType = (EnumDSSDataType)lTypeValue;
				hr = DFCDataConversion::DFCVariantChange2BSTR(pResult, pResult, &lDataType);
                CHECK_HR;
			}
            
            if(strcmp((char*)(pResult->bstrVal), "") == 0)
			{
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            
            //fali, 860423, pResult->bstrVal is wide char value, need to convert to UTF8 char value before executing BSTRInitCap.
			//BSTRInitCap((char*)(pResult->bstrVal));
            int lsLen = 0;
            hr = DFCDataConversion::VariantSize(pResult, DssDataTypeVarChar, &lsLen);
            CHECK_HR;
            
            BYTE *lpStr = new (lpBuffer) BYTE[lsLen];
            // 7/4/2012 tehe
            //BYTE *lpStr = new BYTE[lsLen];
            //MBase::DSSStrongPtr<BYTE, MBase::DeleteArray<BYTE> > pStr;
            
            hr = DFCDataConversion::ConvertFromVariant(pResult, DssDataTypeVarChar, lpStr, &lsLen);
            CHECK_HR;
            
            BSTRInitCap((char *)lpStr);
            
			if(mfTrimString)
			{
				hr = VariantClear(pResult);
				if(hr != S_OK)
					return hr;

                trimSpace((char *)lpStr);
				if(strcmp((char *)lpStr, "") == 0)
				{
					*pFlag = DssDataInvalid;
					return S_OK;
				}

				hr = DFCDataConversion::ConvertToVariant(lpStr, DssDataTypeVarChar, 0, pResult);
				CHECK_HR;
			}
		}
            break;
            
        case DssFuncLTrim:
		{
			if(nParam != 1) 
                return E_FAIL; // 7/4/2012 tehe
            
			CComVariant v;
			getVariant(pParam, 1, v);
            
            
			if(v.vt == VT_EMPTY)
			{
				VariantInit(pResult);
				*pFlag = DssDataOk;
				return S_OK;
			}
            
			if(v.vt != VT_BSTR)
			{
				CComVariant v1;
				getVariant(pParam, 0, v1);
                
				int lTypeValue;
				int lSizeInt = sizeof(int);
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeInteger, (BYTE*)&lTypeValue, &lSizeInt);
				CHECK_HR;
                
				EnumDSSDataType lDataType = (EnumDSSDataType)lTypeValue;
				DFCDataConversion::DFCVariantChange2BSTR(&v, &v, &lDataType);
			}
            
			int lsLen = 0;
			hr = DFCDataConversion::VariantSize(&v, DssDataTypeVarChar, &lsLen);
			CHECK_HR;
            
			BYTE *lpStr = new (lpBuffer) BYTE[lsLen];
            // 7/4/2012 tehe
			//BYTE *lpStr = new BYTE[lsLen];
            //MBase::DSSStrongPtr<BYTE, MBase::DeleteArray<BYTE> > pStr;
            
			hr = DFCDataConversion::ConvertFromVariant(&v, DssDataTypeVarChar, lpStr, &lsLen);
			CHECK_HR;
  
            char* lpcStr = (char*)lpStr;
            if(mfTrimString)
				trimSpace(lpcStr);
            if(strcmp(lpcStr, "") == 0)
			{
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            leftTrimSpace(&lpcStr);
			std::string lStr(lpcStr);

			hr = DFCDataConversion::ConvertToVariant((BYTE *)lStr.c_str(), DssDataTypeVarChar, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncRTrim:
		{
			if(nParam != 1) 
                return E_FAIL; // 7/4/2012 tehe
            
			CComVariant v;
			getVariant(pParam, 1, v);
            
			if(v.vt == VT_EMPTY)
			{
				VariantInit(pResult);
				*pFlag = DssDataOk;
				return S_OK;
			}
            
			if(v.vt != VT_BSTR)
			{
				CComVariant v1;
				getVariant(pParam, 0, v1);
                
				int lTypeValue;
				int lSizeInt = sizeof(int);
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeInteger, (BYTE*)&lTypeValue, &lSizeInt);
				CHECK_HR;
                
				EnumDSSDataType lDataType = (EnumDSSDataType)lTypeValue;
				DFCDataConversion::DFCVariantChange2BSTR(&v, &v, &lDataType);
			}
            
			int lsLen = 0;
			hr = DFCDataConversion::VariantSize(&v, DssDataTypeVarChar, &lsLen);
			CHECK_HR;
            
			BYTE *lpStr = new (lpBuffer) BYTE[lsLen];
            // 7/4/2012 tehe
			//BYTE *lpStr = new BYTE[lsLen];
            //MBase::DSSStrongPtr<BYTE, MBase::DeleteArray<BYTE> > pStr;
            
			hr = DFCDataConversion::ConvertFromVariant(&v, DssDataTypeVarChar, lpStr, &lsLen);
			CHECK_HR;
        
            char* lpcStr = (char*)lpStr;
            if(mfTrimString)
				trimSpace(lpcStr);
            if(strcmp(lpcStr, "") == 0)
			{
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            trimSpace(lpcStr);
			std::string lStr(lpcStr);
            
			hr = DFCDataConversion::ConvertToVariant((BYTE *)lStr.c_str(), DssDataTypeVarChar, 0, pResult);
			CHECK_HR;
		}
            break;
            
        case DssFuncPosition:
		{
			if(nParam != 2) 
                return E_FAIL; // 7/4/2012 tehe
            
			CComVariant v[4];
			for(LONG index = 0; index < 2 * nParam; index++)
				SafeArrayGetElement(pParam->parray, &index, &v[index]);
			
			if(v[1].vt == VT_EMPTY || v[3].vt == VT_EMPTY)
			{
				VariantInit(pResult);
				*pFlag = DssDataOk;
				return S_OK;
			}
            
			if(v[1].vt != VT_BSTR)
			{
				int lTypeValue;
				int lSizeInt = sizeof(int);
				hr = DFCDataConversion::ConvertFromVariant(&v[0], DssDataTypeInteger, (BYTE*)&lTypeValue, &lSizeInt);
				CHECK_HR;
                
				EnumDSSDataType lDataType = (EnumDSSDataType)lTypeValue;
				DFCDataConversion::DFCVariantChange2BSTR(&v[1], &v[1], &lDataType);
			}
            
			if(v[3].vt != VT_BSTR)
			{
				int lTypeValue;
				int lSizeInt = sizeof(int);
				hr = DFCDataConversion::ConvertFromVariant(&v[2], DssDataTypeInteger, (BYTE*)&lTypeValue, &lSizeInt);
				CHECK_HR;
                
				EnumDSSDataType lDataType = (EnumDSSDataType)lTypeValue;
				DFCDataConversion::DFCVariantChange2BSTR(&v[3], &v[3], &lDataType);
			}
            
			// pattern string first, then string
			int lPatternLen = 0;
			hr = DFCDataConversion::VariantSize(&v[1], DssDataTypeVarChar, &lPatternLen);
			CHECK_HR;

            int lStr1Len = lPatternLen;
			BYTE *lpStr1 = new (lpBuffer) BYTE[lStr1Len];
            //BYTE *lpStr1 = new BYTE[lPatternLen];
            //MBase::DSSStrongPtr<BYTE, MBase::DeleteArray<BYTE> > pStr1;
            
            
			hr = DFCDataConversion::ConvertFromVariant(&v[1], DssDataTypeVarChar, lpStr1, &lPatternLen);
			CHECK_HR;
            
			int lStrLen = 0;
			hr = DFCDataConversion::VariantSize(&v[3], DssDataTypeVarChar, &lStrLen);
			CHECK_HR;

            int lStr2Len = lStrLen;
			BYTE *lpStr2 = new (lpBuffer) BYTE[lStr2Len];
            // 7/4/2012 tehe
			//BYTE *lpStr2 = new BYTE[lStrLen];
            //MBase::DSSStrongPtr<BYTE, MBase::DeleteArray<BYTE> > pStr2;
            
			hr = DFCDataConversion::ConvertFromVariant(&v[3], DssDataTypeVarChar, lpStr2, &lStrLen);
			CHECK_HR;
            
            char *lpPattern = (char *)lpStr1;
			char *lpString = (char *)lpStr2;
			if(mfTrimString)
			{
				trimSpace(lpPattern);
				trimSpace(lpString);
			}
            
			if(strcmp(lpPattern, "") == 0 || strcmp(lpString, "") == 0)
			{
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            
			int lnPattern = strnlen(lpPattern, lStr1Len);
			int lnString = strnlen(lpString, lStr2Len);

            int *lpNextVal = new (lpBuffer) int[lnPattern + 1];
            //MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > pNextVal;
            
			int lIndex = hKMPIndex(lpString, lpPattern, lnString, lnPattern, lpNextVal);
            
			lIndex = (lIndex < 0 ? 0 : lIndex + 1);		// position start from 1; if not find, then return 0
			hr = DFCDataConversion::ConvertToVariant((BYTE*)&lIndex, DssDataTypeInteger, 0, pResult);
			CHECK_HR;
    
            // 11/20/2012 tehe: if we don't change type here, it will fail in NumberFormatter::formatVlue:Size:Type:Flag
//            EnumDSSDataType lDataType = DssDataTypeInteger;
//            hr = DFCDataConversion::DFCVariantChange2BSTR(pResult, pResult, &lDataType);
//            CHECK_HR;
		}
            break;
            
        case DssFuncLength:
		{
			if(nParam != 1) 
                return E_FAIL;           // 7/4/2012 tehe 
            
			CComVariant v;
			getVariant(pParam, 1, v);
            
			if(v.vt == VT_EMPTY)
			{
				VariantInit(pResult);
				*pFlag = DssDataOk;
				return S_OK;
			}
            
			if(v.vt != VT_BSTR)
			{
				CComVariant v1;
				getVariant(pParam, 0, v1);
                
				int lTypeValue;
				int lSizeInt = sizeof(int);
				hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeInteger, (BYTE*)&lTypeValue, &lSizeInt);
				CHECK_HR;
                
				EnumDSSDataType lDataType = (EnumDSSDataType)lTypeValue;
				DFCDataConversion::DFCVariantChange2BSTR(&v, &v, &lDataType);
			}
            
			int lsLen = 0;
			hr = DFCDataConversion::VariantSize(&v, DssDataTypeVarChar, &lsLen);
			CHECK_HR;
            

            int lnLength = (lsLen / sizeof(char)) - 1;//strlen((char*)(v.bstrVal));
            
			if(mfTrimString)
			{
                int lTmpLen = lsLen;
				BYTE *lpStr = new (lpBuffer) BYTE[lTmpLen];
                // 7/4/2012 tehe
                //BYTE *lpStr = new BYTE[lsLen];
                //MBase::DSSStrongPtr<BYTE, MBase::DeleteArray<BYTE> > pStr;
                
				hr = DFCDataConversion::ConvertFromVariant(&v, DssDataTypeVarChar, lpStr, &lsLen);
				CHECK_HR;
                
                char *lpcStr = (char*)lpStr;
				trimSpace(lpcStr);
                lnLength = strnlen(lpcStr, lTmpLen);
			}
            
			hr = DFCDataConversion::ConvertToVariant((BYTE*)&lnLength, DssDataTypeInteger, 0, pResult);
			CHECK_HR;
            
            // 11/20/2012 tehe: if we don't change type here, it will fail in NumberFormatter::formatVlue:Size:Type:Flag
//            EnumDSSDataType lDataType = DssDataTypeInteger;
//            hr = DFCDataConversion::DFCVariantChange2BSTR(pResult, pResult, &lDataType);
//            CHECK_HR;
		}
            break;
            
        default:
            AE_ASSERT(false);
	}
    
	*pFlag = DssDataOk;
    
	return  S_OK; 
}

int DSSStringFunc::GetOutputType (int /*size*/, EnumDSSDataType * inputType,
                                                         EnumDSSDataType *outputType)
{
 	if(outputType == NULL || inputType == NULL) 
		return E_POINTER;
    
	//if (*inputType != DssDataTypeVarChar && *inputType != DssDataTypeMBChar && *inputType != DssDataTypeUTF8Char)
	//	return E_FAIL;
    
	if (mFunctionIndex == DssFuncLength || mFunctionIndex == DssFuncPosition)
		*outputType = DssDataTypeInteger;
	else
		*outputType = DssDataTypeVarChar;
    
	return S_OK;
}

int DSSStringFunc::SetStringCompareOption(VARIANT_BOOL iOpt)
{
	mfTrimString = (iOpt == VARIANT_TRUE);
    
	return S_OK;
}

int DSSStringFunc::hGetValueInt(CComVariant *pValue, EnumDSSDataType iType, int *opVar, bool fTowardZero)
{
    int hr = S_OK;
    
    switch (iType)
    {
        case DssDataTypeShort:
        {
            short lShortValue;
            int lSizeShort = sizeof(short);
            
            hr = DFCDataConversion::ConvertFromVariant(pValue, DssDataTypeShort, (BYTE*)&lShortValue, &lSizeShort);
            CHECK_HR;
            *opVar = lShortValue;
        }
            break;
            
        case DssDataTypeInteger:
        {
            int lIntValue;
            int lSizeInt = sizeof(int);
            
            hr = DFCDataConversion::ConvertFromVariant(pValue, DssDataTypeInteger, (BYTE*)&lIntValue, &lSizeInt);
            CHECK_HR;
            *opVar = lIntValue;
        }
            break;
            
        case DssDataTypeFloat:
        {
            float lFloatValue;
            int lSizeFloat = sizeof(float);
            
            hr = DFCDataConversion::ConvertFromVariant(pValue, DssDataTypeInteger, (BYTE*)&lFloatValue, &lSizeFloat);
            CHECK_HR;
            
            if(fTowardZero)
            {
                *opVar = (lFloatValue > 0 ? floor(lFloatValue) : ceil(lFloatValue) );
            }
            else
            {
                *opVar = (lFloatValue > 0 ? ceil(lFloatValue) : floor(lFloatValue) );
            }
        }
            break;
            
        case DssDataTypeDouble:
        {
            double lDoubleValue;
            int lSizeDouble = sizeof(double);
            
            hr = DFCDataConversion::ConvertFromVariant(pValue, DssDataTypeDouble, (BYTE*)&lDoubleValue, &lSizeDouble);
            CHECK_HR;
            
            if(fTowardZero)
            {
                *opVar = (lDoubleValue > 0 ? floor(lDoubleValue) : ceil(lDoubleValue) );
            }
            else
            {
                *opVar = (lDoubleValue > 0 ? ceil(lDoubleValue) : floor(lDoubleValue) );
            }
        }
            break;
            
        default:
            hr = E_INVALIDARG;
            break;
    }
    
    return hr;
}

template <class T>
int DSSStringFunc::hKMPIndex(T *ipString, T *ipPattern, int iStrLength, int iPatternLength, int *pNextVal)
{
    int lStr = 0;
    int lPattern = 0;
    int index;
    
    // find the nextval in the pattern
    hGetPatternNext(ipPattern, iPatternLength, pNextVal);
    
    while(lStr < iStrLength && lPattern < iPatternLength)
    {
        if (lPattern == -1 || ipString[lStr] == ipPattern[lPattern])
        {
            lStr++;
            lPattern++;
        }
        else
            lPattern = pNextVal[lPattern];
    }
    
    if(lPattern >= iPatternLength) 
        index = lStr - iPatternLength;
    else
        index = -1;
    
    return index;
}

template <class T>
int DSSStringFunc::hGetPatternNext(T *ipPattern, int iLength, int *opNextVal)
{
    int j = 0;
    int k = -1;
    opNextVal[0] = -1;
    
    while(j < iLength)
    {
        if (k == -1 || ipPattern[j] == ipPattern[k])
        {
            j++;
            k++;
            if(ipPattern[j] != ipPattern[k])
                opNextVal[j] = k;
            else
                opNextVal[j] = opNextVal[k];
        }
        else
            k = opNextVal[k];
    }
    
    return S_OK;
}


