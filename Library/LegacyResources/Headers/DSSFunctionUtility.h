/*
 *  DSSFunctionUtility.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 2/4/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef FUNCTION_UTILITY_H
#define FUNCTION_UTILITY_H 1

#import <math.h>
#import <float.h>
#include <algorithm>
#import "DFCDataConversion.h"
#include <ctype.h>
#include "DSSAEHelper.h" // 2/22/2012 tehe we need include this header file

#include "DSSBigDecimalWrapper.h"
#include "DSSCellDataWrapper.h"

using namespace std;

class RankPred
{
public:
	RankPred(double * pData, bool fAsc)
	:mpData(pData), mfAsc(fAsc){};	
	bool operator()(int i, int j)
	{
		if(mfAsc)
		{
			if(mpData[i] < mpData[j])
				return true;
			
			return false;
		}
		
		if(mpData[i] > mpData[j])
			return true;
		
		return false;
	}
private:
	double * mpData;
	bool mfAsc;
	
};

inline void Rank(int nSize, double * pData, double *opResult, bool fAsc)
{
    // Veracode: CWE ID 191: Integer Underflow (Wrap or Wraparound)
	if (nSize < 0) nSize = 0;
    // Veracode
	int iSize = nSize;
	
	int *lRank = new int[nSize];
	
	int i = 0;
	for(i = 0; i < iSize; i++)
		lRank[i] = i;
	
	RankPred lPred(pData, fAsc);
	sort(lRank, lRank+iSize, lPred);
	
	//need to search and set the tie to the same rank
	//later we can decide if this processing should only be done as necessary
	double Prev;
	int PrevRank;
	double temp;
	
	if(iSize > 0)
	{
		Prev = pData[lRank[0]];
		PrevRank = 1;	//rank should start from 1
		opResult[lRank[0]] = PrevRank;	//set the right rank 
		
		
		for(i = 1; i < iSize; i++)
		{
			temp = pData[lRank[i]];
			if(temp == Prev)
				opResult[lRank[i]] = PrevRank;
			else
			{
				//it should only be the case that it become smaller
				Prev = temp;
				PrevRank = i + 1;
				opResult[lRank[i]] = PrevRank;
			}
		}
		
	}
	
	delete [] lRank;
	
}

inline int RangeValues(int nSize, double *pData, double Index, double *pLeft, double *pRight, int *pLeftRepeat, int *pRightRepeat)
{
	// nSize should greater than 0
	if(nSize <= 0) return 0;
	
	// check empty pointer
	if(!pLeft || !pRight || !pData) return 0;
	
	// initialize the return values
	*pLeft = -DBL_MAX;
	*pRight = DBL_MAX; 
	
	*pLeftRepeat = 0;
	*pRightRepeat = 0;
	
	double lMin = DBL_MAX;
	double lMax = -DBL_MAX;
	
	for(int i = 0; i < nSize; i++)
	{
		// left boundary
		if((pData[i] <= Index) && (pData[i] > (*pLeft)))
			*pLeft = pData[i];
		
		// right boundary
		if((pData[i] >= Index) && (pData[i] < (*pRight)))
			*pRight = pData[i];
		if(lMin != *pLeft)
		{
			lMin = *pLeft;
			*pLeftRepeat = 1;
		}
		else if(lMin == pData[i])
			(*pLeftRepeat)++;
		
		if(lMax != *pRight)
		{
			lMax = *pRight;
			*pRightRepeat = 1;
		}
		else if(lMax == pData[i])
			(*pRightRepeat)++;
		
	}
	
	return 1;
}

class Asc
{
public:
	bool operator () (double x, double y)
	{
		return x < y;
	}
};

class Desc
{
public:
	bool operator () (double x, double y)
	{
		return x > y;
	}
};

inline int Median(int nSize, double *pData, double *pResult)
{
	//array size should greater than 0
	if(nSize < 0) return 0;
	
	// check pointer
	if(!pData || !pResult) return 0;
	
	double *lpInput = new double[nSize];
	int i;
	for(i = 0; i < nSize; i++)
		lpInput[i] = pData[i];
	
	// sort the input data
	sort(lpInput, lpInput+nSize, Asc());
	
	int lIndex1, lIndex2;
	lIndex1 = (nSize + 1) / 2;
	lIndex2 = (nSize + 2) / 2;
	
	if(lIndex1 == lIndex2)
		*pResult = lpInput[lIndex1-1];
	else
		*pResult = (lpInput[lIndex1-1] + lpInput[lIndex2-1]) / 2;
	
	delete [] lpInput;
	
	return 1;
}

template <class Type>
inline int Partition(Type* pData, int left, int right)
{
	Type tmp = pData[left];
	
	int i = left + 1;
	int j = i;
	for (; i < right; i++)
	{
		if (pData[i] < tmp)
		{
			if (i != j)
				std::swap(pData[i], pData[j]);
			j++;
		}
	}
	std::swap(pData[left], pData[j-1]);
	
	return j-1;	
}

// this function quickly finds the "nIndex"th element of ordered pData, without sorting
// e.g. pData = [5,7,3,6,8], then QP(2) = 6, since index of 6 is 2 in the sorted array (3,5,6,7,8)
// ***********************************************************
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// note: after calling this function, pData may be reordered
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ***********************************************************
template <class Type>
inline Type QuickPosition(int nSize, Type* pData, int nIndex)
{// please check the parameter before calling this function
	if (nSize == 1)
		return pData[0];
	
	int pos = 0;
	
	int begin = 0, end = nSize;
	while(begin < end)
	{
		pos = Partition(pData, begin, end);
		if (pos == nIndex)
			break;
		
		if (pos > nIndex)
			end = pos;
		else
			begin = pos+1;
	}
	return pData[pos];
}

//Another implementation of Standard deviation, which uses incremental algorithm to avoid round off errors
//From artical "Algorithm to compute Mean and Standard Deviation for Pocket Calculator"
//Source: http://netdial.caribe.net/~rabeldin/amsd.htm#c03
//Xiaonan Han 2/15/00

inline double VarianceImpl(int nSize, double *pData){
	
	double d1, d2, lM1, lM2, lSS1, lSS2;
	int i;
	
	d1 = 0.0;
	lM1 = pData[0];
	lSS1 = 0.0;
	
	for (i = 1; i < nSize; i ++){
		d2 = (pData[i] - lM1) / (i + 1);
		lM2 = lM1 + d2;
		lSS2 = lSS1 + (i + 1) * i * d2 * d2;
		
		d1 = d2;
		lM1 = lM2;
		lSS1 = lSS2;
	}
	
	return (lSS1 / (nSize - 1));
	
}

inline int Variance(int nSize, double *pData, double *opResult)
{
	//array size should greater than 0
	if(nSize < 2) return 0;    // return error
	
	// check pointer
	if(!pData || !opResult) return 0;  // return error
	
	*opResult = VarianceImpl(nSize, pData);
	
	return 1;
}



inline int VarianceP(int nSize, double *pData, double *opResult)
{
	//array size should greater than 0
	if(nSize < 1) return 0;    // return error
	
	// check pointer
	if(!pData || !opResult) return 0;  // return error
	
	*opResult = 0.0;
	int i;
	
	double d1, d2, lM1, lM2, lSS1, lSS2;
	
	d1 = 0.0;
	lM1 = pData[0];
	lSS1 = 0.0;
	
	for (i = 1; i < nSize; i ++){
		d2 = (pData[i] - lM1) / (i + 1);
		lM2 = lM1 + d2;
		lSS2 = lSS1 + (i + 1) * i * d2 * d2;
		
		d1 = d2;
		lM1 = lM2;
		lSS1 = lSS2;
	}
	
	*opResult = lSS1 / nSize;
	
	return 1;
}
        
        inline int Geomean(int nSize, double *pData, double *opResult)
        {
            double lnSum = 0;
            int i = 0;
            for (i = 0; i < nSize; i++) {
                if(pData[i] > 0)
                {
                    lnSum += log(pData[i]);
                }
                else
                {
                    return E_FAIL;
                }
            }
            
            *opResult = exp(lnSum / nSize);
            return S_OK;
        }

#ifndef _MATH_ERROR_HANDLE
#define _MATH_ERROR_HANDLE

enum MathError
{
	MathNoError		= 0,
	MathOverflow	= 1,
	MathIllegalOp	= 2,
	MathConvergence	= 3,
	MathDomain		= 4,
};

inline void SetMathError(enum MathError *value, enum MathError error)
{
	if (*value < error)
		*value = error;
}

inline double CheckValue(enum MathError *error, double value)
{
	if (isnan(value))
		SetMathError(error, MathIllegalOp);
	else if (!isfinite(value))
		SetMathError(error, MathOverflow);
	return value;
}

inline double DomainError(enum MathError *error)
{
	SetMathError(error, MathDomain);
	return 0.0;
}

inline enum MathError *ClearError(enum MathError *error)
{
	*error = MathNoError;
	return error;
}

#endif

//Added by Xiaonan Han
//Double comparision functions
#define MaxDoubleErr			1E-10

inline bool DoubleEqual(double a, double b) {
	
	double diff = a > b ? a - b : b - a;
	
	if (diff < MaxDoubleErr)
		return true;
	else 
		return false;
}

// qjiang 08/07/2009
// rewrite the following comparison functions
// a LessThan b  ==> a < b && |a-b| >= MaxDoubleErr ==> b-a >= MaxDoubleErr
// a LessEqual b ==> a < b || |a-b| < MaxDoubleErr  ==> a-b < MaxDoubleErr
// other 2 cases (GreatThan, GreatEqual) can be deduced similarly

//a < b returns true. otherwise false
inline bool DoubleLessThan(double a, double b){
	
	return (b - a) >= MaxDoubleErr;
	/*if ( DoubleEqual(a, b))
	 return false;
	 
	 return (a < b);*/
}

//a <= b returns true. otherwise false
inline bool DoubleLessEqual(double a, double b){
	
	return (a - b) < MaxDoubleErr;
	/*if ( DoubleEqual(a, b))
	 return true;
	 
	 return (a < b);*/
}


//a > b returns true. otherwise false
inline bool DoubleGreaterThan(double a, double b){
	
	return (a - b) >= MaxDoubleErr;
	/*if ( DoubleEqual(a, b))
	 return false;
	 
	 return (a > b);*/
}

//a >= b returns true. otherwise false
inline bool DoubleGreaterEqual(double a, double b){
	
	return (b - a) < MaxDoubleErr;
	/*if ( DoubleEqual(a, b))
	 return true;
	 
	 return (a > b);*/
}

//xzhu,2014/7/1/,function for calculating weighted mean average
inline Int32 WeightMeanAverage(Int32 nSize, double *pValue, double *pWeight, double *opResult)
{
	if(nSize < 0)
		return 0; //for no valid input, return 0
    
	if(!pValue || !pWeight || !opResult)
		return 0;
    
	double lTotalValue = 0.0;
	double lTotalWeight = 0.0;
	for(Int32 i = 0; i < nSize; i ++)
	{
		lTotalValue += pValue[i] * pWeight[i];
		lTotalWeight += pWeight[i];
	}
    
	//juzhang, 2014-3-10, TQMS 859077
	if(DoubleEqual(lTotalWeight, 0)) //total weight is equal to 0, return fail
		return 0;
    
	*opResult = lTotalValue / lTotalWeight;
    
	return 1;
}

// jxie, 2010-11-12
// get the nIndex data from numeric data array
inline double GetNumericData(void* ipData, Int32 nIndex, EnumDSSDataType iType)
{
	double lResult = 0;
	switch(iType)
	{
		case DssDataTypeInteger:
			lResult = (static_cast<int*> (ipData) )[nIndex];
			break;
		case DssDataTypeDouble:
			lResult = (static_cast<double*> (ipData) )[nIndex];
			break;
		case DssDataTypeShort:
			lResult = (static_cast<short*> (ipData) )[nIndex];
			break;
		case DssDataTypeLong:
			lResult = (static_cast<long*> (ipData) )[nIndex];
			break;
		case DssDataTypeFloat:
			lResult = (static_cast<float*> (ipData) )[nIndex];
			break;
		case DssDataTypeBool:
			lResult = (static_cast<bool*> (ipData) )[nIndex];
			break;
		default:
			return 0.0;
	}
	return lResult;
}

// jxie, 2010-9-26
// use this for direct calculate
inline bool VarianceSegment(Int32 nSize, void *pData, EnumDSSDataType iDataType, DSSDataFlag* ipDataFlag,  
                            EnumNullCheckingOption iNullCheckingOption,
							Int32* ipRows, double &d1, double &lM1, double &lSS1, Int32 &offset)
{
	//array size should greater than 0
	if(nSize < 1) return false;    // return error
    
	// check pointer
	if(!pData) return false;  // return error
    
	Int32 i;
    
	double d2, lM2, lSS2;
	double lData = 0;
    
	for (i = 0; i < nSize; i++)
	{
		if (ipDataFlag[ipRows[i]] != DssDataOk)
		{
			if (iNullCheckingOption == DssNumericErrorTreatAs)
				lData = 0;
			else
				continue;
		}
		else
			lData = GetNumericData(pData, ipRows[i], iDataType);
        
		d2 = (lData - lM1) / (offset + 1);
		lM2 = lM1 + d2;
		lSS2 = lSS1 + (offset + 1) * offset * d2 * d2;
        
		d1 = d2;
		lM1 = lM2;
		lSS1 = lSS2;
        
		offset++;
	}
    
	return true;
}

// jxie, 2010-9-26
// use this for direct calculate
inline bool VarianceSegment(Int32 nSize, void *pData, EnumDSSDataType iDataType, DSSDataFlag* ipDataFlag, 
                            EnumNullCheckingOption iNullCheckingOption,
							Int32 iStart, double &d1, double &lM1, double &lSS1, Int32 &offset,
                            bool* opAllSkipCheckNull = NULL)
{
	//array size should greater than 0
	if(nSize < 1) return false;    // return error
    
	// check pointer
	if(!pData) return false;  // return error
    
	Int32 i;
    
	double d2, lM2, lSS2;
	double lData = 0;
    
	bool lfAllSkipCheckNull = true;
	DSSDataFlag lFlag = DssDataOk;
    
	for (i = 0; i < nSize; i++)
	{
		lFlag = ipDataFlag[iStart];
        //!!!!!
        //		if (lFlag == DssDataIgnoreCheckNull)
        //			continue;
        
		lfAllSkipCheckNull = false;
        
		if (lFlag != DssDataOk)
		{
			if (iNullCheckingOption == DssNumericErrorTreatAs)
				lData = 0;
			else
				continue;
		}
		else
			lData = GetNumericData(pData, iStart++, iDataType);
        
		d2 = (lData - lM1) / (offset + 1);
		lM2 = lM1 + d2;
		lSS2 = lSS1 + (offset + 1) * offset * d2 * d2;
        
		d1 = d2;
		lM1 = lM2;
		lSS1 = lSS2;
        
		offset++;
	}
    
	if (opAllSkipCheckNull)
		*opAllSkipCheckNull = lfAllSkipCheckNull;
    
	return true;
}


inline int getVariant(CComVariant *pParam, long index, CComVariant& element){
    
    SafeArrayGetElement(pParam->parray, &index, &element);
	return S_OK;
    
}

inline int setVariant(CComVariant *pParam,
                      long index, CComVariant &element){
    
    SafeArrayPutElement(pParam->parray, &index, &element);
	return S_OK;
    
}

//trim white space at the end of string
//Example string "Audio   ", after trimming, becomes "Audio"
inline Int32  trimSpace(wchar_t *str){
	Int32 sLen = wcslen(str);
	if (sLen < 1) 
		return 0;
    
	Int32 i = 0;
	for (i = sLen - 1; i >= 0; i --)
		if (iswspace(str[i]))
			str[i] = L'\0';
		else
			break;
    
	return i + 1;
}

inline Int32  trimSpace(char *str){
	Int32 sLen = strlen(str);
	if (sLen < 1) 
		return 0;
    
	Int32 i = 0;
	for (i = sLen - 1; i >= 0; i --)
		if (isspace(str[i]))
			str[i] = '\0';
		else
			break;
    
	return i + 1;
}
//7/21/2016, msun
inline int lengthTrimSpace(char *str)
{
    Int32 sLen = strlen(str);
    if (sLen < 1)
        return 0;
    
    Int32 i = 0;
    for (i = sLen - 1; i >= 0; i --)
        if (!isspace(str[i]))
            break;
    
    return i + 1;
}

// 7/4/2012 tehe
inline Int32 leftTrimSpace(wchar_t **ppStr){
	wchar_t *lpStr = *ppStr;
	Int32 sLen = wcslen(lpStr);
	if (sLen < 1)
		return 0;
    
	Int32 i = 0;
	for (i = 0; i < sLen; i++)
		if (iswspace(lpStr[i]))
			continue;
		else
			break;
    
	*ppStr = lpStr + i;
    
	return i + 1;
}

inline Int32 leftTrimSpace(char **ppStr){
	char *lpStr = *ppStr;
	Int32 sLen = strlen(lpStr);
	if (sLen < 1)
		return 0;
    
	Int32 i = 0;
	for (i = 0; i < sLen; i++)
		if (isspace(lpStr[i]))
			continue;
		else
			break;
    
	*ppStr = lpStr + i;
    
	return i + 1;
}


// 7/4/2012 tehe
#ifndef __ANDROID__
inline void BSTRInitCap(BSTR str)
{
    Int32 len = (str) ? SysStringLen(str) : 0;
    
    if (len < 1)	return;
    str[0] = towupper(str[0]);
    for (Int32 i = 1; i < len; i++)
        str[i] = towlower(str[i]);
}
#endif
inline void BSTRInitCap(char* str)
{
    Int32 len = strlen(str);
    
    if (len < 1)	return;
    str[0] = towupper(str[0]);
    for (Int32 i = 1; i < len; i++)
        str[i] = towlower(str[i]);
}


inline Int32  variantCompare(CComVariant& v1, CComVariant& v2,
                             DSSDataFlag *pFlag,  bool ibSimpleComparison = true, bool ibTrimString = true)
{    
    HRESULT hr1, hr2;
    Int32 result = 0;
    bool v1_date = false, v2_date = false, v1_bd = false, v2_bd = false;
    // jzeng; 10/22/15; to support Cell Format Data in variant compare
    bool v1_cd = false, v2_cd = false;
    
    DFCDataConversion::checkDispatchType(&v1, &v1_bd, &v1_cd, &v1_date);
    DFCDataConversion::checkDispatchType(&v2, &v2_bd, &v2_cd, &v2_date);
    
    //String, suppose they are unicode strings
    if(VT_BSTR == v1.vt && VT_BSTR == v2.vt)
    {
        Int32 sLen1 = 0;
        Int32 sLen2 = 0;
        
        hr1 = DFCDataConversion::VariantSize(&v1, DssDataTypeVarChar, &sLen1);
        hr2 = DFCDataConversion::VariantSize(&v2, DssDataTypeVarChar, &sLen2);
        
        if(S_OK != hr1 || S_OK != hr2)
        {
            *pFlag = DssDataInvalid;
        }
        else
        {
            // Veracode: CWE ID 195: Signed to Unsigned Conversion Error
            signed long long tmp1 = (signed long long) sLen1;
            signed long long tmp2 = (signed long long) sLen2;
            
            BYTE *str1 = NULL;
            BYTE *str2 = NULL;
            
            if (tmp1 > 0 && tmp2 > 0 && tmp1 <= INT_MAX && tmp2 <= INT_MAX) {
                str1 = new BYTE[sLen1];
                str2 = new BYTE[sLen2];
            }
            // Veracode
            
            hr1 = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeVarChar, str1, &sLen1);
            hr2 = DFCDataConversion::ConvertFromVariant(&v2, DssDataTypeVarChar, str2, &sLen2);
            
            if(S_OK != hr1 || S_OK != hr2)
            {
                *pFlag = DssDataInvalid;
            }
            else
            {
                // 12/18/2012 tehe #697869: current AE code will only use utf-8 code, so wchar_t is not supported
                // trimSpace((wchar_t *)str1);
                // trimSpace((wchar_t *)str2);
                if(ibTrimString)
                {
                    trimSpace((char*)str1);
                    trimSpace((char*)str2);
                }
                
                if(ibSimpleComparison)
                {
                    //result = wcscmp((wchar_t *)str1, (wchar_t *)str2);
                    result = strcmp((char *)str1, (char *)str2);
                }
                else
                {
                    //!!!!There is no compare function that take 2 parameters, find it later.
                    //                    result = DSSAEHelper::CompareStringsWithLocale((wchar_t *)str1, (wchar_t *)str2);
                    
                    // 12/18/2012 tehe: current we didn't support local now 
//                    if(ibCaseSensitive)
//                    {
                    	result = strcmp((char *)str1, (char *)str2);
//                    }
//                    else
//                    {
//					#ifndef __ANDROID__
//                        result = stricmp((char *)str1, (char *)str2);
//						#endif
//                    }
                }
            }
            
            delete []str1;
            str1 = NULL;
            delete []str2;
            str2 = NULL;
        }
    }
    else if(v1_date && v2_date)
    {
        double date1 = v1.date;
        double date2 = v2.date;
        
        if(DoubleEqual(date1, date2))
        {
            result = 0;
        }
        else if(DoubleLessThan(date1, date2))
        {
            result = -1;
        }
        else
        {
            result = 1;
        }
    }
    else if(v1_bd && v2_bd)
    {
        //Support for bigDecimal
        DSSBigDecimalWrapper *lpBDWrapper1 = dynamic_cast<DSSBigDecimalWrapper*>(v1.pdispVal);
        CHECK_PTR(lpBDWrapper1);
        DSSBigDecimalWrapper *lpBDWrapper2 = dynamic_cast<DSSBigDecimalWrapper*>(v2.pdispVal);
        CHECK_PTR(lpBDWrapper2);
        
        const MDataType::DSSBigDecimal *lpBD1 = lpBDWrapper1->getBigDecimal();
        CHECK_PTR(lpBD1);
        const MDataType::DSSBigDecimal *lpBD2 = lpBDWrapper2->getBigDecimal();
        CHECK_PTR(lpBD2);
        
        if(*lpBD1 == *lpBD2)
        {
            result = 0;
        }
        else if(*lpBD1 < *lpBD2)
        {
            result = -1;
        }
        else
        {
            result = 1;
        }
    }
    else if(v1_cd && v2_cd)
    {
        //Support for bigDecimal
        DSSCellDataWrapper *lpCDWrapper1 = dynamic_cast<DSSCellDataWrapper*>(v1.pdispVal);
        CHECK_PTR(lpCDWrapper1);
        DSSCellDataWrapper *lpCDWrapper2 = dynamic_cast<DSSCellDataWrapper*>(v2.pdispVal);
        CHECK_PTR(lpCDWrapper1);
        
        const MDataType::DSSCellFmtData *lpCD1 = lpCDWrapper1->getCellFormatData();
        CHECK_PTR(lpCD1);
        const MDataType::DSSCellFmtData *lpCD2 = lpCDWrapper2->getCellFormatData();
        CHECK_PTR(lpCD2);
        
        if(*lpCD1 == *lpCD2)
        {
            result = 0;
        }
        else if(*lpCD1 < *lpCD2)
        {
            result = -1;
        }
        else
        {
            result = 1;
        }
    }
    else
    {
        //numbers, one of them may be BigDecimal, but it is safe to convert to double
        Int32 sizeDouble = sizeof(double);
        double d1, d2;
        
        if (v1_cd) {
            Int32 lSize = sizeof(MDataType::DSSCellFmtData);
            MDataType::DSSCellFmtData cd;
            
            hr1 = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeCellFormatData, (BYTE *)&cd, &lSize);
            d1 = cd.GetValue();
        } else {
            hr1 = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeDouble, (BYTE *)&d1, &sizeDouble);
        }
        
        if (v2_cd) {
            Int32 lSize = sizeof(MDataType::DSSCellFmtData);
            MDataType::DSSCellFmtData cd;
            
            hr2 = DFCDataConversion::ConvertFromVariant(&v2, DssDataTypeCellFormatData, (BYTE *)&cd, &lSize);
            d2 = cd.GetValue();
        } else {
            hr2 = DFCDataConversion::ConvertFromVariant(&v2, DssDataTypeDouble, (BYTE *)&d2, &sizeDouble);
        }
        
        if(S_OK != hr1 || S_OK != hr2)
        {
            *pFlag = DssDataInvalid;
        }
        else
        {
            if(DoubleEqual(d1, d2))
            {
                result = 0;
            }
            else if(DoubleLessThan(d1, d2))
            {
                result = -1;
            }
            else
            {
                result = 1;
            }
        }
    }
    
    return result;  
}

// jxie, 2010-11-10
// similar to RankPred, use this for rank direct calculation
template <class InputDataType>
class DirectRankPred
{
public:
	DirectRankPred(InputDataType* pData, DSSDataFlag *pFlag, bool fAsc)
    :mpData(pData), mpFlag(pFlag), mfAsc(fAsc){};
    
	bool operator()(Int32 i, Int32 j)
	{
		double lNum1, lNum2;
		if (mpFlag[i] == DssDataOk)
			lNum1 = (double)(mpData[i]);
		else
			lNum1 = 0;
		if (mpFlag[j] == DssDataOk)
			lNum2 = (double)(mpData[j]);
		else
			lNum2 = 0;
        
		if(mfAsc)
		{
			if(lNum1 < lNum2)
				return true;
            
			return false;
		}
        
		if(lNum1 > lNum2)
			return true;
        
		return false;
	}
private:
	InputDataType* mpData;
	DSSDataFlag* mpFlag;
	bool mfAsc;
    
};

// jxie, 2010-9-17
// move out for reusing in direct calculate
inline void RankForRows(Int32 iSize, void* pData, DSSDataFlag *pFlag, EnumDSSDataType iType,
						double* opResult, bool fAsc, Int32* lRank, Int32 iPrevRank = 0)
{
	DirectRankPred<Int32> lPred1(static_cast<Int32*>(pData), pFlag, fAsc);
	DirectRankPred<double> lPred2(static_cast<double*>(pData), pFlag, fAsc);
	DirectRankPred<short> lPred3(static_cast<short*>(pData), pFlag, fAsc);
	DirectRankPred<long> lPred4(static_cast<long*>(pData), pFlag, fAsc);
	DirectRankPred<float> lPred5(static_cast<float*>(pData), pFlag, fAsc);
	DirectRankPred<bool> lPred6(static_cast<bool*>(pData), pFlag, fAsc);
	switch(iType)
	{
		case DssDataTypeInteger:
			sort(lRank, lRank+iSize, lPred1);
			break;
		case DssDataTypeDouble:
			sort(lRank, lRank+iSize, lPred2);
			break;
		case DssDataTypeShort:
			sort(lRank, lRank+iSize, lPred3);
			break;
		case DssDataTypeLong:
			sort(lRank, lRank+iSize, lPred4);
			break;
		case DssDataTypeFloat:
			sort(lRank, lRank+iSize, lPred5);
			break;
		case DssDataTypeBool:
			sort(lRank, lRank+iSize, lPred6);
			break;
		default:
			return;
	}
    
	//need to search and set the tie to the same rank
	//later we can decide if this processing should only be done as necessary
	double Prev;
	Int32 PrevRank;
	double temp;
	Int32 i = 0;
    
	if(iSize > 0)
	{
		Prev = GetNumericData(pData, lRank[0], iType);
		PrevRank = 1 + iPrevRank;	//rank should start from 1
		opResult[lRank[0]] = PrevRank;	//set the right rank 
        
        
		for(i = 1; i < iSize; i++)
		{
			temp = GetNumericData(pData, lRank[i], iType);
			if(temp == Prev)
				opResult[lRank[i]] = PrevRank;
			else
			{
				//it should only be the case that it become smaller
				Prev = temp;
				PrevRank = i + 1;
				opResult[lRank[i]] = PrevRank;
			}
		}
        
	}
}

//zhyang, 22/07/2011, In the server side, it use wchar, but some platform don't support wchar, so
//I change it to char. Need to be thought of other solution!!!!
inline int getVariantString(CComVariant *pParam, long index, char **pStr)
{
	CComVariant v1;
    
	SafeArrayGetElement(pParam->parray, &index, &v1);
    
	if (v1.vt != VT_BSTR) 
		return E_FAIL;
    
	HRESULT hr;
	Int32 sLen = 0;
	hr = DFCDataConversion::VariantSize(&v1, DssDataTypeVarChar, &sLen);
	CHECK_HR;
    
	*pStr = NULL;
	if (sLen <= 0)
		return S_OK;
    
	unsigned char * str = new unsigned char[sLen];
	hr = DFCDataConversion::ConvertFromVariant(&v1, DssDataTypeVarChar, str, &sLen);
	CHECK_HR;
    
    
	*pStr = (char*)str;
    
	return S_OK;
    
}

// jxie, 2010-11-19
// change double to integer for rounding
inline Int32 ChangeDoubleToInt(double iData)
{
	if (iData < INT_MIN)
		return INT_MIN;
	if (iData > INT_MAX)
		return INT_MAX;
    
	Int32 lResult = (Int32)(iData);
	if (iData >= 0)
	{
		if((lResult % 2 && iData - lResult >= 0.5) || (!(lResult % 2) && iData - lResult > 0.5))
			++lResult;
	}
	else
	{
		if((lResult % 2 && iData - lResult <= -0.5) || (!(lResult % 2) && iData - lResult < -0.5))
			--lResult;
	}
	return lResult;
}
#endif
