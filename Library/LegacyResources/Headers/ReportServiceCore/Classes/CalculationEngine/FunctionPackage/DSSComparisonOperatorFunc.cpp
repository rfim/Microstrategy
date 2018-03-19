//
//  DSSComparisonOperatorFunc.cpp
//  ReportServiceCore
//
//  Created by Zhuo Yang on 11/2/12.
//  Copyright (c) 2012 Microstrategy. All rights reserved.
//

#include "DSSComparisonOperatorFunc.h"
#include "DSSFunctionUtility.h"
#import "DSSStrongPtr.h"

DSSComparisonOperatorFunc::DSSComparisonOperatorFunc():mfTrimString(true)
//               :mFunctionIndex(NUll)   //zhyang, 07/18/2011 DssFuncUnknown I'm not sure if it is right
{
    
}

DSSComparisonOperatorFunc::~DSSComparisonOperatorFunc()
{
    
}

int DSSComparisonOperatorFunc::CalculateRepeat(int nParam, int nRepeat, double **pParam,
                                               double *pResult, DSSDataFlag *pFlag)
{
    int hr = S_OK;
    
    if(!pParam || !pResult || !pFlag)
        return E_POINTER;
    
    for(Int32 i = 0; i < nRepeat; ++i)
    {
        hr = Calculate(nParam, pParam[i], pResult + i, pFlag + i);
        CHECK_HR;
    }
    
    return hr;
}

int DSSComparisonOperatorFunc::Calculate(int nSize, double *pVec, double *pResult, DSSDataFlag *pFlag)

{
    int hr = S_OK;
    
    enum MathError error = MathNoError;
    
    if(!pVec || !pResult || !pFlag)
        return E_POINTER;
    
    *pFlag = DssDataOk;
    
    switch (mFunctionIndex) {
        case DssFuncEquals:
            if(2 != nSize)
            {
                return E_FAIL;
            }
            *pResult = DoubleEqual(pVec[0], pVec[1]);
            break;
            
       case DssFuncNotEqual:
            if(2 != nSize)
            {
                return E_FAIL;
            }
            *pResult = !DoubleEqual(pVec[0], pVec[1]);
            break;
            
        case DssFuncGreater:
            if(2 != nSize)
            {
                return E_FAIL;
            }
            *pResult = DoubleGreaterThan(pVec[0], pVec[1]);
            break;
            
        case DssFuncLess:
            if(2 != nSize)
            {
                return E_FAIL;
            }
            *pResult = DoubleLessThan(pVec[0], pVec[1]);
            break;
            
        case DssFuncGreaterEqual:
            if(2 != nSize)
            {
                return E_FAIL;
            }
            *pResult = DoubleGreaterEqual(pVec[0], pVec[1]);
            break;
            
        case DssFuncLessEqual:
            if(2 != nSize)
            {
                return E_FAIL;
            }
            *pResult = DoubleLessEqual(pVec[0], pVec[1]);
            break;
            
        case DssFuncBetween:
        case DssFuncNotBetween:
            if(3 != nSize)
            {
                return E_FAIL;
            }
            
            if(pVec[1] < pVec[2])
            {
                *pResult = (DoubleGreaterEqual(pVec[0], pVec[1]) && DoubleLessEqual(pVec[0], pVec[2]));
            }
            else
            {
                *pResult = (DoubleGreaterEqual(pVec[0], pVec[2]) && DoubleLessEqual(pVec[0], pVec[1]));
            }
            
            if(DssFuncNotBetween == mFunctionIndex)
            {
                *pResult = !(*pResult);
            }
            break;
            
        default:
            return E_FAIL;
    }
    
    CheckValue(&error, *pResult);
    if(MathNoError != error)
    {
        *pResult = 0;
        *pFlag = DssDataInvalid;
    }
    
    return hr;
}

int DSSComparisonOperatorFunc::Calculate(int nSize, CComVariant *pVec, CComVariant *pResult, DSSDataFlag *pFlag)
{
    int hr = S_OK;
    
    if(!pVec || !pResult || !pFlag)
    {
        return E_POINTER;
    }
    
    if( ((pVec->vt) & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT))
    {
        return E_FAIL;
    }
    
    *pFlag = DssDataOk;
    double result = 0;
    
    switch (mFunctionIndex) {
        case DssFuncEquals:
            if(2 != nSize)
            {
                return E_FAIL;
            }
            result = (hVariantCompare(pVec, pFlag) == 0);
            break;
            
        case DssFuncNotEqual:
            if(2 != nSize)
            {
                return E_FAIL;
            }
            result = (hVariantCompare(pVec, pFlag) != 0);
            /*fali, 898082, comment out temporily since it affects view evaluation result, but it may influence conditional logic. Need further attention.
            //compare string to number
            if(pFlag && *pFlag == DssDataInvalid)
            {
                result = 1;
                *pFlag = DssDataOk;
            }*/
            break;
            
        case DssFuncGreater:
            if(2 != nSize)
            {
                return E_FAIL;
            }
            result = (hVariantCompare(pVec, pFlag) > 0);
            break;
            
        case DssFuncLess:
            if(2 != nSize)
            {
                return E_FAIL;
            }
            result = (hVariantCompare(pVec, pFlag) < 0);
            break;
            
        case DssFuncGreaterEqual:
            if(2 != nSize)
            {
                return E_FAIL;
            }
            result = (hVariantCompare(pVec, pFlag) >= 0);
            break;
            
        case DssFuncLessEqual:
            if(2 != nSize)
            {
                return E_FAIL;
            }
            result = (hVariantCompare(pVec, pFlag) <= 0);
            break;
            
        case DssFuncBetween:
            if(3 != nSize)
            {
                return E_FAIL;
            }
            result = hVariantBetween(pVec, pFlag);
            break;
            
        case DssFuncNotBetween:
            if(3 != nSize)
            {
                return E_FAIL;
            }
            result = !hVariantBetween(pVec, pFlag);
            /*fali, 898082, comment out temporily since it affects view evaluation result, but it may influence conditional logic. Need further attention.
             //compare string to number
            if(pFlag && *pFlag == DssDataInvalid)
            {
                result = 1;
                *pFlag = DssDataOk;
            }*/
            break;
            
        case DssFuncBeginsWith:
        case DssFuncEndsWith:
        case DssFuncContains:
            if(2 != nSize)
            {
                return E_FAIL;
            }
            result = hVariantStrOperator(pVec, pFlag);
            break;
            
        case DssFuncNotBeginsWith:
        case DssFuncNotEndsWith:
        case DssFuncNotContains:
            if(2 != nSize)
            {
                return E_FAIL;
            }
            result = !hVariantStrOperator(pVec, pFlag);
            break;
            
        default:
            return E_FAIL;
    }
    
    if(DssDataOk == *pFlag)
    {
        DFCDataConversion::ConvertToVariant((unsigned char*)&result, DssDataTypeDouble, 0, pResult);
    }
    
    return hr = S_OK;
}

int DSSComparisonOperatorFunc::setProperty(int Index, CComVariant *ipVal)
{
    return E_NOTIMPL;
}
//
//int DSSComparisonOperatorFunc::Init(EnumDSSFunction iFunctionIndex)
//{
//    mFunctionIndex = iFunctionIndex;
//    return S_OK;
//}

int DSSComparisonOperatorFunc::GetOutputType(int size, EnumDSSDataType *inputType, EnumDSSDataType *outputType)
{
    if(!outputType)
        return E_POINTER;
    
    *outputType = DssDataTypeDouble;
    
    return S_OK;
}


int DSSComparisonOperatorFunc::hVariantCompare(CComVariant *pParam, DSSDataFlag *pFlag)
{
    CComVariant v1, v2;
    getVariant(pParam, 0, v1);
    getVariant(pParam, 1, v2);

    Int32 result = variantCompare(v1, v2, pFlag,false, mfTrimString);
    
    return result;
}

int DSSComparisonOperatorFunc::hVariantBetween(CComVariant *pParam, DSSDataFlag *pFlag)
{
    CComVariant v1, v2, v3;
    getVariant(pParam, 0, v1);
    getVariant(pParam, 1, v2);
    getVariant(pParam, 2, v3);
    
    Int32 result = 0;
    Int32 temp = variantCompare(v2, v3, pFlag,false,mfTrimString);
    
    if(DssDataOk != *pFlag)
    {
        return result;
    }
    
    if(temp < 0)
    {
        result = ((variantCompare(v1, v2, pFlag,false,mfTrimString) >= 0) && (variantCompare(v1, v3, pFlag, false,mfTrimString) <= 0)); 
    }
    else
    {
        result = ((variantCompare(v1, v3, pFlag, false,mfTrimString) >= 0) && (variantCompare(v1, v2, pFlag, false,mfTrimString) <= 0)); 
    }
    
    return result;
}

//zhyang, 22/07/2011, some plantfrom don't suppport wchar, Change all the strs to char*
//!!!!This need to be refined with locale
bool DSSComparisonOperatorFunc::hVariantStrOperator(CComVariant *pParam, DSSDataFlag *pFlag)
{
    
	//wchar_t *pattern, *str;	
    char* pattern = NULL;
    char *str = NULL;
	HRESULT hr1, hr2;
	bool result = false;
    
	hr1 = getVariantString(pParam, 0, &str);
	hr2 = getVariantString(pParam, 1, &pattern);
    
    MBase::DSSStrongPtr<char, MBase::DeleteArray<char> > lPatternPtr(pattern);
    MBase::DSSStrongPtr<char, MBase::DeleteArray<char> > lStrPtr(str);
    
  //  if(str && pattern)
   // {
        if (hr1 != S_OK || hr2 != S_OK)
            *pFlag = DssDataInvalid;
        else{
            //trimSpace((wchar_t *)str);
            //trimSpace((wchar_t *)pattern);
            
            Int32 sLen1 = strlen(str);
            Int32 sLen2 = strlen(pattern);
            
            if (sLen2 <= sLen1) {
                switch(mFunctionIndex) {
                    case DssFuncNotBeginsWith:
                    case DssFuncBeginsWith:
                        result = (strncmp(str, pattern, sLen2) == 0) ? true : false;
                        break;
                        
                    case DssFuncEndsWith:
                    case DssFuncNotEndsWith:
                        
                        result = (strncmp(str + sLen1 - sLen2, pattern, sLen2) == 0) ? true : false;
                        break;
                        
                    case DssFuncContains:
                    case DssFuncNotContains:
                        result = (strstr(str, pattern) != NULL) ? true : false;
                        break;
                    default:
                        break;
                }
            }
        }
        
    //}
	return result;
}

//int DSSComparisonOperatorFunc::GetOperatorType(EnumDSSFunction *opType)
//{
//    *opType = mFunctionIndex;
//    return  S_OK;
//}

int DSSComparisonOperatorFunc::DirectCalculate(Int32 nParam, Int32* ipSize, 
                                               Int32** ipOffsets, void** ipData, double* ipDivider, 
                                               DSSDataFlag** ipFlags, VARIANT_BOOL* ipAllDataValid,
                                               EnumDSSDataType* iType, DSSXTabBitVector* ipMask,
                                               Int32 nResSize, double* opData, DSSDataFlag* opFlag, 
                                               VARIANT_BOOL*   opAllDataValid,
                                               EnumNullCheckingOption iNullCheckingOption)
{
    int hr = S_OK;
    if(DssFuncBetween == mFunctionIndex || DssFuncNotBetween == mFunctionIndex)
    {
        assert(3 == nParam);
        double *lpData[3];
        
        for(Int32 i = 0; i < 3; ++i)
        {
            hr = hTryConvert(ipSize[i], ipOffsets[i], ipData[i], iType[i], ipDivider[i], &lpData[i]);
            CHECK_HR;
        }
        hr = hDirectCalculateBetween(ipSize, lpData, ipFlags, ipAllDataValid, nResSize, opData, opFlag, opAllDataValid, iNullCheckingOption);
        CHECK_HR;
    }
    else
    {
        assert(2 == nParam);
        hr = CDSSSimpleFunctionHelper::DirectCalculate(ipData[0], ipData[1], ipOffsets[0], ipOffsets[1], iType[0], iType[1], ipSize[0], 
                                                       ipSize[1], ipDivider[0], ipDivider[1], 
                                                       ipFlags[0], ipFlags[1], ipAllDataValid[0], ipAllDataValid[1],
                                                       ipMask,
                                                       nResSize, opData, opFlag, opAllDataValid, 
                                                       iNullCheckingOption, mFunctionIndex);
 
		CHECK_HR;
    }
    return hr;
}

bool DSSComparisonOperatorFunc::SupportDirectCalculate()
{    
    switch (mFunctionIndex) {
        case DssFuncEquals:
        case DssFuncNotEqual:
        case DssFuncGreater:
        case DssFuncLess:
        case DssFuncGreaterEqual:
        case DssFuncLessEqual:
        case DssFuncBetween:
        case DssFuncNotBetween:
            return true;
            break;
        default:
            break;
    }
    
    return false;
}

int DSSComparisonOperatorFunc::hTryConvert(int iSize, Int32* ipOffset, void *ipData, EnumDSSDataType iType, 
                                           double iDivider, double **opData)
{
    int hr = S_OK;
    
    CHECK_PTR(opData);
    
    double *lpData = NULL;
    Int32* lpOffset = ipOffset;
    
    switch (iType) 
    {
        case DssDataTypeNumeric:
        case DssDataTypeDecimal:
        case DssDataTypeDouble:
            // only need to check offset for double type
            if (lpOffset == NULL) 
            {
                lpData = (double *)ipData;
            }
            else
            {
                double* lpDouble = (double*)ipData;
				lpData = new double[iSize];
				CHECK_NEWED_PTR(lpData);
                
				for (Int32 i = 0; i < iSize; i++)
				{
					lpData[i] = lpDouble[*lpOffset];
					++lpOffset;
				}
            }
            break;
            
        case DssDataTypeLong:
        case DssDataTypeInteger:
        {
            bool lfDivider = (iDivider > 1);
            Int32 *lpInt = (Int32 *)ipData;
            lpData = new double[iSize];
            CHECK_NEWED_PTR(lpData);
            for(Int32 i = 0; i < iSize; ++i)
            {
                if(lpInt)
                    lpData[i] = lfDivider ? (lpInt[i]/iDivider) : lpInt[i];
            }
            break;
        }
            
        case DssDataTypeShort:
        {
            short *lpShort = (short *)ipData;
            lpData = new double[iSize];
            CHECK_NEWED_PTR(lpData);
            for(Int32 i = 0; i < iSize; ++i)
            {
                lpData[i] = lpShort[i];
            }
            break;
        }
            
            // qjiang 10/11/2011 TQMS507503 Add support for float type
        case DssDataTypeFloat:
		{
			float* lpFloat = (float*)ipData;
			lpData = new double[iSize];
			CHECK_NEWED_PTR(lpData);
			for (Int32 i = 0; i < iSize; i++)
			{
				lpData[i] = lpFloat[i];
			}
		}
        default:
            assert(false);
            return S_FALSE;
    }
    
    *opData = lpData;
    return hr;
}

int DSSComparisonOperatorFunc::hDirectCalculateBetween(int *ipSize, double **ipData, 
                                                       DSSDataFlag **ipFlags, VARIANT_BOOL *ipAllDataValid, 
                                                       int nResSize, double *opData, DSSDataFlag *opFlag, 
                                                       VARIANT_BOOL *opAllDataValid, 
                                                       EnumNullCheckingOption iNullCheckingOption)
{
    int hr = S_OK;
    memset(opFlag, DssDataOk, nResSize);
    
    bool lfBetween = (mFunctionIndex == DssFuncBetween);
    bool lfConst[3] = {ipSize[0] == 1, ipSize[1] == 1, ipSize[2] == 1};
    
    double lVar1 = ipData[0][0];
    double lVar2 = ipData[1][0];
    double lVar3 = ipData[2][0];
    
    bool lResult = false;
    bool lAllValid = true;
    
    for(Int32 iRow = 0; iRow < nResSize; ++iRow)
    {
        if(!lfConst[0])
        {
            lVar1 = ipData[0][iRow];
        }
        if(!lfConst[1])
        {
            lVar2 = ipData[1][iRow];
        }
        if(!lfConst[2])
        {
            lVar3 = ipData[2][iRow];
        }
        
        if(lVar2 < lVar3)
        {
            lResult = ((DoubleGreaterEqual(lVar1, lVar2)) && (DoubleLessEqual(lVar1, lVar3)));
        }
        else
        {
            lResult = ((DoubleGreaterEqual(lVar1, lVar3)) && (DoubleLessEqual(lVar1, lVar2)));
        }
        if(!lfBetween)
        {
            lResult = !lResult;
        }
        opData[iRow] = lResult;
    }
    
    if((VARIANT_TRUE == ipAllDataValid[0] && VARIANT_TRUE == ipAllDataValid[1]&& VARIANT_TRUE == ipAllDataValid[2])
       ||DssNumericErrorTreatAs == iNullCheckingOption)
    {
        lAllValid = true;
    }
    else
    {        
        for(Int32 iRow = 0; iRow < nResSize; ++iRow)
        {
            Int32 iCol = 0;
            for(iCol = 0; iCol < 3; ++iCol)
            {
                if(!lfConst[iCol] && DssDataOk != ipFlags[iCol][iRow])
                {
                    break;
                }
            }
            if(iCol < 3)
            {
                opFlag[iRow] = ipFlags[iCol][iRow];
                lAllValid = false;
            }
        }
    }
    
    *opAllDataValid = lAllValid ? VARIANT_TRUE : VARIANT_FALSE;
    return hr;
}


int DSSComparisonOperatorFunc::SetStringCompareOption(VARIANT_BOOL iOpt)
{
	mfTrimString = (iOpt == VARIANT_TRUE);
    
	return S_OK;
}
