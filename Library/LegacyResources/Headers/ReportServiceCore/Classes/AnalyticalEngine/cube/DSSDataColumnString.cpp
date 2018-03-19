/*
 *  DSSDataColumnString.cpp
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 1/21/10.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import <memory.h>
#include <stdlib.h>
#include "DSSDataColumnString.h"
#import "DSSAEHelper.h"
#import "DSSDataSource.h"

#import "DSSFunctionUtility.h"

#define CHECK_NEWED_PTR(p) {if (!p) return E_OUT_OF_MEMORY;}


inline int lStrcmp(EnumDSSDataType iType, wchar_t * string1, wchar_t * string2, int size1, int size2, int locale=0, bool ibSimpleComparison = false, bool ibTrimFlag = false, bool ibCaseSensitive = true)
{
	if (string1 == NULL || string2 == NULL)
	{
		if (string1 != NULL)
		{
			return 1;		//string1 is not NULL, string2 is NULL
		}
		else
		{					//string1 is NULL
			return string2 != NULL ? -1 : 0;		//string2 is not NULL : string2 is NULL
		}
	}
	
    // 12/18/2012 tehe
    if(ibTrimFlag)
    {
        trimSpace(string1);
        trimSpace(string2);
    }
	if (ibSimpleComparison || locale == 0)
		return wcsncmp(string1, string2, (size1 > size2)? size1 : size2);
	
	return DSSAEHelper::CompareStringsWithLocale(string1, string2, size1, size2, locale);
}

int compareWithCaseSensitive(const char * string1, const char * string2, int size1, int size2, bool ibCaseSensitive, bool ibSimpleComparison = true)
{
    if (ibSimpleComparison)
    {
        if (ibCaseSensitive)
        {
            return strncmp(string1, string2, (size1 > size2) ? size1 : size2);
        }
        else
        {
            return strncasecmp(string1, string2, (size1 > size2) ? size1 : size2);
        }
    }
    else
    {
        return DSSAEHelper::compareWithSpecialChar(string1, string2, ibCaseSensitive);
    }
}

inline int lStrcmp(EnumDSSDataType iType, char * string1, char * string2, int size1, int size2, int locale=0, bool ibSimpleComparison = true, bool ibTrimFlag = false, bool ibCaseSensitive = true)
{    
	if (string1 == NULL || string2 == NULL)
	{
		//488952, mppData[iRow] could be NULL because of incremental downloading
		if (string1 != NULL)
		{
			return 1;		//string1 is not NULL, string2 is NULL
		}
		else
		{					//string1 is NULL
			return string2 != NULL ? -1 : 0;		//string2 is not NULL : string2 is NULL
		}
	}
	
	if(iType == DssDataTypeBinary || iType == DssDataTypeVarBin || iType == DssDataTypeLongVarBin)
	{
		//special handling for binary because of the lack of NULL termination
		//we could compare byte to byte
        // jimzhang Jul/2/2012 TQMS 622014 comment out previous compare logic
		/*int lSmall = size1 > size2 ? size2: size1;
		for(int i = 0; i < lSmall; i ++)
		{
			if(string1[i] < string2[i])
				return -1;
			
			if(string1[i] > string2[i])
				return 1;
		}
		
		//have been the same so far
		if(size1 < size2)
			return -1;
		
		if(size1 > size2)
			return 1;*/
        
        // jimzhang Jul/2/2012 TQMS 622014 begin For data of "DssDataTypeBinary" type, iPad side has converted it 
        //      from binary data to hex string and populate column with hex sting at de-serialization stage.
        //      So, when comparing data of "DssDataTypeBinary" type, to be consistent with logic of server side (compare binary data byte by byte),
        //      we should first convert hex sting to binary data.
        int lSizeInByte1 = (size1-1)/2;
        int lSizeInByte2 = (size2-1)/2;
        
        int lSmall = lSizeInByte1 > lSizeInByte2 ? lSizeInByte2: lSizeInByte1;
		for (int i = 0; i < lSmall; i++)
		{   
            int lOff = 2*i;
            
            int lHigh = string1[lOff] <= '9' ? (string1[lOff] - '0') : (toupper(string1[lOff]) - 'A' + 10);
            int lLow = string1[lOff+1] <= '9' ? (string1[lOff+1] - '0') : (toupper(string1[lOff+1]) - 'A' + 10);
            char lByte1 = lHigh*16 + lLow;
            
            lHigh = string2[lOff] <= '9' ? (string2[lOff] - '0') : (toupper(string2[lOff]) - 'A' + 10);
            lLow = string2[lOff+1] <= '9' ? (string2[lOff+1] - '0') : (toupper(string2[lOff+1]) - 'A' + 10);
            char lByte2 = lHigh*16 + lLow;
            
			if (lByte1 < lByte2)
				return -1;
			
			if (lByte1 > lByte2)
				return 1;
		}
		
		//have been the same so far
		if(lSizeInByte1 < lSizeInByte2)
			return -1;
		
		if(lSizeInByte1 > lSizeInByte2)
			return 1;
        // jimzhang Jul/2/2012 TQMS 622014 end
		
		return 0;
	}
	
    // 12/18/2012 tehe
    if(ibTrimFlag)
    {
        //zhyang, 797812， when we need to do trim, create a new string for trim
        /*int len1 = strlen(string1) + 1;
        char *trimString1 = new char[len1];
        CHECK_NEWED_PTR(trimString1);
        MBase::StrongArrayPtr<char> lStrongPtrString1(trimString1);
        memmove(trimString1, string1, sizeof(char) * len1);
        
        int len2 = strlen(string2) + 1;
        char *trimString2 = new char[len2];
        MBase::StrongArrayPtr<char> lStrongPtrString2(trimString2);
        memmove(trimString2, string2, sizeof(char) * len2);
        
        trimSpace(trimString1);
        trimSpace(trimString2);
        
        if (ibSimpleComparison || locale == 0)
        {
            return compareWithCaseSensitive(trimString1, trimString2, size1, size2, ibCaseSensitive);
        }
        
        return DSSAEHelper::CompareStringsWithLocale(trimString1, trimString2, size1, size2, locale);*/
        
        //msun, de36871, I think it's equivalent to above logic. In some very certain cases, if two strings are not equal, the return value could be 1 while in above logic it is -1. (for example, "a   ", "a\t\t b")
        //fortunately, checking all the callers of this function, if ibTrimFlag is true, they only care if two strings are equal.
        int len1 = lengthTrimSpace(string1);
        int len2 = lengthTrimSpace(string2);
        
        if (ibSimpleComparison || locale == 0)
        {
            return compareWithCaseSensitive(string1, string2, len1, len2, ibCaseSensitive, ibSimpleComparison);
        }
        
        return DSSAEHelper::CompareStringsWithLocale(string1, string2, len1, len2, locale, ibSimpleComparison);

    }

    
    if (ibSimpleComparison || locale == 0)
    {
        return compareWithCaseSensitive(string1, string2, size1, size2, ibCaseSensitive, ibSimpleComparison);
    }
	
	return DSSAEHelper::CompareStringsWithLocale(string1, string2, size1, size2, locale, ibSimpleComparison);
}

template<class T>
DSSDataColumnString<T>::DSSDataColumnString():mMaxRowSize(0), mppData(NULL),mDataSize(0)
,mpMMPBuffer(NULL), mMMPBufSize(0),mbUseMappedFile(false),mbTrimString(true), mbUsePartition(false), mppPartitionList(NULL), mnPartitions(0)
{
}

template<class T>
DSSDataColumnString<T>::~DSSDataColumnString()
{
	int lDataSize = mRowCount * 10;		//assume 10 bytes for each string
    
	RecycleMemory();
	if(mppData && mDataSize > 0)
	{
		//released memory for *mppData released in RecycleMemory
		//we don't report to mem governer inside RecycleMemory because the menthod
		//may be called multiple times during the lifetime of the column
MEM_GOVERNOR_RELEASE(mDataSize, mDataSize)	
	}
		
	if (mppData)
	{
        delete [] mppData;
MEM_GOVERNOR_RELEASE(mRowCount * sizeof(strData<T>*), lDataSize)
	}
	
	if (mpFlags)
	{
		delete[] mpFlags;
		mpFlags = NULL;
MEM_GOVERNOR_RELEASE(mRowCount * sizeof(DSSDataFlag), lDataSize)
	}
    
    if(mppPartitionList)
    {
        delete[] mppPartitionList;
        mppPartitionList = NULL;
        long long lReserveSize = mnPartitions * sizeof(strData<T>**);
        MEM_GOVERNOR_RELEASE(lReserveSize, mRowCount * sizeof(strData<T>*));
    }
}

template class DSSDataColumnString<char>;
template class DSSDataColumnString<wchar_t>;

template<class T>
int DSSDataColumnString<T>::Init(EnumDSSDataType iDataType, long long iRowCount, bool ibUseMappedFile)
{
	mRowCount = iRowCount;
    
    mDataType = iDataType;
	mbAllDataValid = true;
    
    if(hNeedPartition())
    {
        mbUsePartition = true;
        return hCreatePartition();
    }
	
	long long lDataSize = iRowCount * 10;		//assume 10 bytes for each string
	long long	lReservedSize = iRowCount * sizeof(T*);
	{
MEM_GOVERNOR_RESERVE(lReservedSize, lDataSize)
	mppData = new strData<T> * [iRowCount];
	CHECK_NEWED_PTR(mppData);
MEM_GOVERNOR_COMMIT(lReservedSize)
	}
	memset(mppData, 0, iRowCount * sizeof(strData<T>*));
    
	//for(int i = 0; i < iRowCount; i++)
	//	mpFlags[i] = DssDataUnknown;
    //memset(mpFlags, DssDataUnknown, iRowCount * sizeof(DSSDataFlag));
	return S_OK;
}

template<class T>
long long DSSDataColumnString<T>::getRowCount()
{
	//ctang 2012-7-24
	return mfHasJoinNULL ? mRowCount - 1 : mRowCount;
}

template<class T>
int DSSDataColumnString<T>::get_RowCount(int* opRowCount)
{
    if(opRowCount)
    {
        *opRowCount = getRowCount();
    }
    return S_OK;
}

template <class T>
bool DSSDataColumnString<T>::IsRefColumn()
{
    return false;
}

template<class T>
int DSSDataColumnString<T>::getSizeOfData()
{
	return mMaxRowSize;
}

template<class T>
int DSSDataColumnString<T>::getRowSize(long long iRow, int &orSize)
{
	if(iRow >= mRowCount || iRow < 0)
		return E_INVALIDINDEX;
	
    int hr;
	strData<T>* lpData = hGetDataPtr(iRow, hr);
	orSize = lpData? lpData->mSize : 0;
	
	return hr;
}

template<class T>
int DSSDataColumnString<T>::setData(long long iRow, char *ipData, int iSizeInByte, DSSDataFlag iFlag)
{
    if(ipData == NULL && iSizeInByte != 0)
		return E_POINTER;
	
	if(iRow >= mRowCount || iRow < 0)
		return E_INVALIDINDEX;
    
    strData<T> **lppData = NULL;
    if(mbUsePartition)
    {
        
		long long lGroup = iRow >> COL_PRTTN_BIT;
		long long lOffset = iRow - (lGroup << COL_PRTTN_BIT);
		
		strData<T>** lpPartition = NULL;
		int hr = hGetPartition(lGroup, &lpPartition);
		if (lpPartition && hr == S_OK)
		{
            lppData = &(lpPartition[lOffset]);
        }
    }
    else
    {
        lppData = &mppData[iRow];
    }
	
	// qjiang, 2008-12-31 TQMS 322916, if cbData is 0, its type is DssDataOK, and it should be empty string
    // DE34506 - should check lppData first since it may be NULL due to memory limit
    CHECK_NEWED_PTR(lppData);
	if (iFlag != DssDataOk || iSizeInByte == 0)
	{	//NULL string or empty string
		//strData<T> *lpData = mppData[iRow];
		if (!*lppData)
        {
            *lppData = new strData<T>();
            CHECK_NEWED_PTR(*lppData);
        }
        
        if((*lppData)->mpData)
        {
            delete [] (*lppData)->mpData;
        }
        
        (*lppData)->mpData = (T*)(new char[sizeof(T)]);
		CHECK_NEWED_PTR((*lppData)->mpData);
		memset((*lppData)->mpData, 0, sizeof(T));
        
        (*lppData)->mSize = sizeof(T);
        (*lppData)->mFlag = iFlag;
        
		//mppData[iRow] = lpData;
        //mppData[iRow]->mFlag = iFlag;
        
		if (mMaxRowSize == 0)
			mMaxRowSize = sizeof(T);
		
	} else {
		//strData<T> *lpData = mppData[iRow];
		if (!(*lppData) || (*lppData)->mSize < iSizeInByte) {
            if (!(*lppData))
            {
                *lppData = new strData<T>();
                CHECK_NEWED_PTR(*lppData);
                //lpData = mppData[iRow];
            }
            
            if((*lppData)->mpData)
            {
                delete [] (*lppData)->mpData;
            }
            
            (*lppData)->mpData = (T*)(new char[iSizeInByte]);
            CHECK_NEWED_PTR((*lppData)->mpData);
		}
		
		(*lppData)->mSize = iSizeInByte;
        (*lppData)->mFlag = DssDataOk;
		memmove((*lppData)->mpData, ipData, iSizeInByte);
		if (mMaxRowSize < iSizeInByte)
			mMaxRowSize = iSizeInByte;
	}
	if (iFlag != DssDataOk)
		mbAllDataValid = false;
	
	return S_OK;
}

template<class T>
int DSSDataColumnString<T>::getData(long long iRow, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag)
{
	if (iRow >= mRowCount || iRow < 0 || !oppData)
		return E_INVALIDARG;
	
    int hr;
	strData<T> *lpData = hGetDataPtr(iRow, hr);
    
    orSize = lpData ? lpData->mSize : 0;
	orType = mDataType;
	orFlag = lpData? lpData->mFlag : DssDataUnknown;
	*oppData = lpData ? reinterpret_cast<const char*> (lpData->mpData) : NULL;
	
	return hr;
}

template<class T>
int DSSDataColumnString<T>::getDataCopy(long long iRow, char *pBuffer, int iBufSize, int &orCopiedSize, EnumDSSDataType &orType, DSSDataFlag &orFlag)
{
	if (iRow >= mRowCount || iRow < 0 || (!pBuffer))
		return E_INVALIDARG;
/*
	if (iBufSize > 0 && (!mppData[iRow] || mppData[iRow]->mSize < iBufSize))
		return E_INVALIDARG;
*/
    int hr;
    strData<T> *lpData = hGetDataPtr(iRow, hr);
    
    orCopiedSize = lpData ? lpData->mSize : 0;
	orType = mDataType;
	orFlag = lpData ? lpData->mFlag : DssDataUnknown;
	memmove(pBuffer, lpData ? lpData->mpData : NULL, orCopiedSize);
    
	return hr;
}

template<class T>
int DSSDataColumnString<T>::setDataInBulk(long long iStart, long long iEnd, char *ipData, int iSizeInByte, DSSDataFlag *ipFlags, int iSIBFlags)
{
	return E_NOTIMPL;
}

template<>
int DSSDataColumnString<char>::getNumericVal(long long iRow, double &orVal, DSSDataFlag &orFlag)
{
	//467139, handle null strings
	if (iRow >= mRowCount || iRow < 0)
		return E_INVALIDARG;
	
    int hr;
    strData<char> *lpData = hGetDataPtr(iRow, hr);
    CHECK_HR;
	orFlag = lpData ? lpData->mFlag : DssDataUnknown;
	if (orFlag != DssDataOk)
	{
		orVal = 0;
		return S_OK;
	}
	//454635, there's need from graph matrix widget to convert string data to double
	orVal = lpData && lpData->mpData ? atof(lpData->mpData) : 0;
	orFlag = DssDataOk;
	if (orVal == 0)
	{
		int lSize = lpData ? lpData->mSize : 0;
		char *lpString = lpData && lpData->mpData ? lpData->mpData : NULL;
		for (int i = 0; i < lSize; i++)
		{
            if(lpString)
            {
                if (*lpString == ' ' || *lpString == '+' || *lpString == '-' || *lpString == '\t')
                {
                    lpString++;
                    continue;
                }
                else if (*lpString == '.')
                {
                    lpString++;
                    break;
                }
                else
                    break;
            }
		}
        if(lpString)
        {
            if (*lpString < '0' || *lpString > '9')
            {
                //invalid string
                return E_NaN;
            }
        }
		
	}
	return S_OK;
}

template<class T>
int DSSDataColumnString<T>::getNumericVal(long long iRow, double &orVal, DSSDataFlag &orFlag)
{
	return E_NOTIMPL;
}

template<class T>
int DSSDataColumnString<T>::setNumeric(long long iRow, double iVal, DSSDataFlag iFlag)
{
	return E_NOTIMPL;
}

template<class T>
int DSSDataColumnString<T>::getFlag(long long iRow, DSSDataFlag &orFlag)
{
	if(iRow >= mRowCount)
		return E_INVALIDINDEX;
    int hr;
	strData<T> *lpData = hGetDataPtr(iRow, hr);
	orFlag = lpData ? lpData->mFlag : DssDataUnknown;
	return hr;
}

template<class T>
int DSSDataColumnString<T>::setFlag(long long iRow, DSSDataFlag iFlag)
{
	if(iRow >= mRowCount)
		return E_INVALIDINDEX;
    
    int hr;
    strData<T> *lpData = hGetDataPtr(iRow, hr);
    
    if (lpData)
	{
        lpData->mFlag = iFlag;
    }
    if(iFlag != DssDataOk && mbAllDataValid)
        mbAllDataValid = false;
    
	return hr;
}

template<class T>
int DSSDataColumnString<T>::setFlags(long long iStart, long long iEnd, DSSDataFlag *ipFlags, int iSizeInByte)
{
	if (mRowCount * sizeof(DSSDataFlag) != iSizeInByte || !ipFlags)
		return E_INVALIDARG;
    
    int hr = S_OK;
    for (int i = 0; i < mRowCount; i++)
    {
        strData<T> *lpData = hGetDataPtr(i, hr);
        CHECK_HR;
        
        if(lpData)
        {
            lpData->mFlag = ipFlags[i];
        }
    }

	if (mbAllDataValid)
	{
		for(int i = iStart; i < iEnd; i++)
		{
			if(ipFlags[i] != DssDataOk)
			{
				mbAllDataValid = false;
				break;
			}
		}
	}
	return hr;
}

template<class T>
int DSSDataColumnString<T>::setFlags(long long iStart, long long iEnd, DSSDataFlag *ipFlags, int* iElementList, int iElementSize)
{
	if(iEnd - iStart != iElementSize || !ipFlags)
		return E_INVALIDARG;
    
    int hr;
    //msun, lpData will created and initialized if needed (by calling hGetDataPtr)
    //so there is no need to allocate memory in advance
	/*for(int i = 0; i <mRowCount; i++)
    {
        strData<T> *lpData = hGetDataPtr(i, hr);
        CHECK_HR;
        
        if(lpData)
        {
            lpData->mFlag = 0;
        }
    }*/
	for(int i = 0; i < iElementSize;i++)
	{
		if(iElementList[i] < 0 || iElementList[i] >= mRowCount)
			return E_INVALIDARG;
		
        strData<T> *lpData = hGetDataPtr(iElementList[i], hr);
        CHECK_HR;
        
        if(lpData)
        {
            lpData->mFlag = ipFlags[iStart + i];
        }
	}
	
	if (mbAllDataValid)
	{
		for(int i = iStart; i < iEnd; i++)
		{
			if(ipFlags[i] != DssDataOk)
			{
				mbAllDataValid = false;
				break;
			}
		}
	}
	return S_OK;
}


template<class T>
int DSSDataColumnString<T>::Order(long long iRow1, long long iRow2)
{
	if (iRow1 < mRowCount && iRow2 < mRowCount)
	{
        int hr;
        strData<T>  *lpDataPtr1 = hGetDataPtr(iRow1, hr);
        CHECK_HR;
        strData<T>  *lpDataPtr2 = hGetDataPtr(iRow2, hr);
        CHECK_HR;
        
        DSSDataFlag lFlag1 = lpDataPtr1 ? lpDataPtr1->mFlag : DssDataUnknown;
        DSSDataFlag lFlag2 = lpDataPtr2 ? lpDataPtr2->mFlag : DssDataUnknown;
        T* lpData1 = lpDataPtr1 && lpDataPtr1->mpData ? lpDataPtr1->mpData : NULL;
        T* lpData2 = lpDataPtr2 && lpDataPtr2->mpData ? lpDataPtr2->mpData : NULL;
        int lSize1 = lpDataPtr1 ? lpDataPtr1->mSize : 0;
        int lSize2 = lpDataPtr2 ? lpDataPtr2->mSize : 0;
        
		if (lFlag1 == DssDataOk && lFlag2 == DssDataOk)
        {
            //zhyang, 801671
            if(mpInputRank)
                return mpInputRank[iRow1] < mpInputRank[iRow2] ? -1 : 1;
            else
                return lStrcmp(mDataType, lpData1, lpData2, lSize1, lSize2, 0, mbSimpleStrComparison, false, false);
        }
		else if (lFlag1 == DssDataOk)	//iRow1 is non-NULL and iRow2 is NULL
			return 1;
		else
			return lFlag2 == DssDataOk ? -1 : 0;	//iRow1 is NULL
	}
	else if (iRow1 < mRowCount)	//iRow2 is invalid, treat it as NULL and smaller than iRow1
		return 1;
	else
		return iRow2 < mRowCount ? -1 : 0;	//treate as equal if both are invalid, or smalller is iRow2 is valid
}



template<class T>
int DSSDataColumnString<T>::Order(long long iRow1, long long iRow2,bool fIsDisplayNullOnTop)
{
	if (iRow1 < mRowCount && iRow2 < mRowCount)
	{
        int hr;
        strData<T>  *lpDataPtr1 = hGetDataPtr(iRow1, hr);
        CHECK_HR;
        strData<T>  *lpDataPtr2 = hGetDataPtr(iRow2, hr);
        CHECK_HR;
        
        DSSDataFlag lFlag1 = lpDataPtr1 ? lpDataPtr1->mFlag : DssDataUnknown;
        DSSDataFlag lFlag2 = lpDataPtr2 ? lpDataPtr2->mFlag : DssDataUnknown;
        T* lpData1 = lpDataPtr1 && lpDataPtr1->mpData ? lpDataPtr1->mpData : NULL;
        T* lpData2 = lpDataPtr2 && lpDataPtr2->mpData ? lpDataPtr2->mpData : NULL;
        int lSize1 = lpDataPtr1 ? lpDataPtr1->mSize : 0;
        int lSize2 = lpDataPtr2 ? lpDataPtr2->mSize : 0;
        
		if (lFlag1 == DssDataOk && lFlag2 == DssDataOk)
        {
            //zhyang, 801671
            if(mpInputRank)
                return mpInputRank[iRow1] < mpInputRank[iRow2] ? -1 : 1;
            else
            {
                // jzeng US18084 comparison of sort is case ignore
                return lStrcmp(mDataType, lpData1, lpData2, lSize1, lSize2, 0, mbSimpleStrComparison, false, false);
            }
        }
        else
        {
            return hCompareFlag(lFlag1, lFlag2, fIsDisplayNullOnTop);
        }
    }
    else
    {
        DSSDataFlag lFlag1=(iRow1<mRowCount)?DssDataOk:DssDataUnknown;
        DSSDataFlag lFlag2=(iRow2<mRowCount)?DssDataOk:DssDataUnknown;
        
        return hCompareFlag(lFlag1, lFlag2, fIsDisplayNullOnTop);
    }
}

template<class T>
int DSSDataColumnString<T>::hCompareRow(long long iRow1, long long iRow2, bool iIsNullDisplayOnTop,int iLocale,  bool ibSimpleComparison)
{
    return DSSDataColumnString<T>::hCompareRow(iRow1, iRow2, iIsNullDisplayOnTop, iLocale, ibSimpleComparison, true);
}

template<class T>
int DSSDataColumnString<T>::hCompareRow(long long iRow1, long long iRow2, bool iIsNullDisplayOnTop,int iLocale,  bool ibSimpleComparison, bool ibCaseSensitive)
{
    int hr;
    strData<T>  *lpDataPtr1 = hGetDataPtr(iRow1, hr);
    CHECK_HR;
    strData<T>  *lpDataPtr2 = hGetDataPtr(iRow2, hr);
    CHECK_HR;
    
    DSSDataFlag lFlag1 = lpDataPtr1 ? lpDataPtr1->mFlag : DssDataUnknown;
    DSSDataFlag lFlag2 = lpDataPtr2 ? lpDataPtr2->mFlag : DssDataUnknown;
    
    if(lFlag1 == DssDataOk && lFlag2 == DssDataOk)
    {
        //zhyang, 801671
        if(mpInputRank)
            return mpInputRank[iRow1] < mpInputRank[iRow2] ? -1 : 1;

        T* lpData1 = lpDataPtr1 && lpDataPtr1->mpData ? lpDataPtr1->mpData : NULL;
        T* lpData2 = lpDataPtr2 && lpDataPtr2->mpData ? lpDataPtr2->mpData : NULL;
        int lSize1 = lpDataPtr1 ? lpDataPtr1->mSize : 0;
        int lSize2 = lpDataPtr2 ? lpDataPtr2->mSize : 0;
        return lStrcmp(mDataType, lpData1, lpData2, lSize1, lSize2, iLocale, ibSimpleComparison, false, ibCaseSensitive);
    }
    else
        return hCompareFlag(lFlag1, lFlag2, iIsNullDisplayOnTop);
}


template<class T>
int DSSDataColumnString<T>::hCompareData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags, bool iIsNullDisplayOnTop, bool ibSimpleComparison)
{
    return DSSDataColumnString<T>::hCompareData(iRow, ipData, iSize, iFlags, iIsNullDisplayOnTop, ibSimpleComparison, true);
}

template<class T>
int DSSDataColumnString<T>::hCompareData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags, bool iIsNullDisplayOnTop, bool ibSimpleComparison, bool ibCaseSensitive)
{
    int hr;
    strData<T>  *lpDataPtr = hGetDataPtr(iRow, hr);
    CHECK_HR;
    DSSDataFlag lFlag = lpDataPtr ? lpDataPtr->mFlag : DssDataUnknown;
    
    if(iFlags == DssDataOk && lFlag == DssDataOk)
    {
        // T* lpData = mppData[iRow] && mppData[iRow]->mpData ? mppData[iRow]->mpData : NULL;
        // int lSize = mppData[iRow] ? mppData[iRow]->mSize : 0;
        T* lpData = lpDataPtr && lpDataPtr->mpData ? lpDataPtr->mpData : NULL;
        int lSize = lpDataPtr ? lpDataPtr->mSize : 0;
        return lStrcmp(mDataType, lpData, reinterpret_cast<T*> (ipData), lSize, iSize, 0, ibSimpleComparison, mbTrimString, ibCaseSensitive);
    }
    else
        return hCompareFlag(lFlag, iFlags, iIsNullDisplayOnTop);
}

template<class T>
int DSSDataColumnString<T>::size()
{
    if(mbUsePartition)
    {
        int lSize = sizeof(this);
        
        lSize += DSSDataColumn::size();
        
        int lnPartions = mnPartitions;
        
        lSize += lnPartions * sizeof(int); // mppPartitionList
        if(mppPartitionList)
        {
            //assert(false);
            for(int i = 0; i < mnPartitions; ++i)
            {
                if(mppPartitionList[i])
                {
                    long long lnRowCount = COLUMN_PARTITION_SIZE;
                    if (i == mnPartitions - 1)		//the last one:
                        lnRowCount = mRowCount - (i << COL_PRTTN_BIT);
                    
                    lSize += lnRowCount * sizeof(int); //mppPartitionList[i]
                    for(int j = 0; j < lnRowCount; ++j)
                    {
                        if(mppPartitionList[i][j])
                        {
                            lSize += mppPartitionList[i][j]->mSize;
                        }
                    }
                }
            }
        }
        return lSize;
    }
    
	int lSize = sizeof(this);
	
	lSize += DSSDataColumn::size();
	
	int lnRows = mRowCount;
	if (lnRows > 0)
	{
		lSize += sizeof(int) * lnRows;	//mpRowSize
		
		for (int i = 0; i < lnRows; i++)
		{
			lSize += mppData[i] ? mppData[i]->mSize : 0;		//mppData[i];
		}
	}
	return lSize;
}

template<class T>
int DSSDataColumnString<T>::CreatePartition()
{
/*	int lnPart = mRowCount / COLUMN_PARTITION_SIZE + 1;
	mppPartitionList = new T*[lnPart];
	int i = 0;
	for (i = 0; i < lnPart; i++)
	{
		mppPartitionList[i] = NULL;
	}
	hMapRows(0);
*/
	return S_OK;
}

template<class T>
void DSSDataColumnString<T>::RecycleMemory()
{
	if (mppData)
	{
		for (int i = 0; i < mRowCount; i++)
		{
			if (mppData[i])
            {	
                if(mppData[i]->mpData)
                {
                    delete[] mppData[i]->mpData;
                    mppData[i]->mpData = NULL;
                }
                delete mppData[i];
                mppData[i] = NULL;
			}
		}
	}
    
    if(mbUsePartition && mppPartitionList)
    {
        //assert(false);
        for(int i = 0; i < mnPartitions; ++i)
        {
            if(mppPartitionList[i])
            {
                long long lnRowCount = COLUMN_PARTITION_SIZE;
                if (i == mnPartitions - 1)		//the last one:
                    lnRowCount = mRowCount - (i << COL_PRTTN_BIT);
                
                for(int j = 0; j < lnRowCount; ++j)
                {
                    if(mppPartitionList[i][j])
                    {
                        if(mppPartitionList[i][j]->mpData)
                        {
                            delete mppPartitionList[i][j]->mpData;
                            mppPartitionList[i][j]->mpData = NULL;
                        }
                        delete mppPartitionList[i][j];
                        mppPartitionList[i][j] = NULL;
                    }
                }
                delete[] mppPartitionList[i];
                mppPartitionList[i] = NULL;
                long long lReserveSize = (i == mnPartitions - 1) ? 0 : lnRowCount * sizeof(strData<T>**);	//for the last partition, don't report it
                MEM_GOVERNOR_RELEASE(lReserveSize, mRowCount * sizeof(strData<T>*))
            }
        }
    }
}

template <class T>
int DSSDataColumnString<T>::reserveMemForStrData()
{
    if(!hNeedPartition())
    {
        mDataSize = mRowCount * 10;//assume 10 bytes for each string
        MEM_GOVERNOR_RESERVE(mDataSize, mDataSize)
        MEM_GOVERNOR_COMMIT(mDataSize)
    }
    return S_OK;
}

template <class T>
int DSSDataColumnString<T>::createDataFlag(EnumDSSDataFlag iFlag)//method used for simple column
{
    return S_OK;
}

template<class T>
int DSSDataColumnString<T>::createMMapMemory(const char * ipFilePath, bool ibUseNewFile)
{ 
    /*
    int lSizeOfStr = sizeof(T*) + sizeof(int) + sizeof(DSSDataFlag);
    int lDataSize = mRowCount * lSizeOfStr;
    if (lDataSize < MEM_GOVERNOR_SIZE)
        return S_OK;
    
    void *lpBuffer = NULL;
    const void *lpData = mppData;
    int hr = DataSourceUtil::createMappedFileBuffer(1, &lpData, &lDataSize, ipFilePath, ibUseNewFile, &mpMMPBuffer, &mMMPBufSize, &lpBuffer);
    CHECK_HR;
    
    delete [] mppData;
    mppData = (strData<T>**)lpBuffer;
    return hr;
    */
    return S_OK;
}

template<class T>
int DSSDataColumnString<T>::MapFileBuffer()
{
    return S_OK;
}

template<class T>
int DSSDataColumnString<T>::UnMapFileBuffer()
{
    return S_OK;
}

template<class T>
int DSSDataColumnString<T>::ExtendColumn(int inRows)
{
    int hr = S_OK;
    
    if (!mbUsePartition)
    {
        assert(mppData);
        MEM_GOVERNOR_RESERVE((inRows + mRowCount) * sizeof(strData<T>*), (inRows + mRowCount) * sizeof(strData<T>*))
        strData<T>** lppData = new strData<T>* [inRows + mRowCount];
        CHECK_NEWED_PTR(lppData);
        MEM_GOVERNOR_COMMIT((inRows + mRowCount) * sizeof(strData<T>*))
        //memmove(lppData,mppData,inRows * sizeof(strData<T>*));
        memmove(lppData,mppData,mRowCount * sizeof(strData<T>*));
        for (int i = 0; i < inRows; ++i) // initialize the extended data
            lppData[mRowCount + i] = 0;

        delete [] mppData;
        MEM_GOVERNOR_RELEASE(mRowCount * sizeof(strData<T>*), mRowCount * sizeof(strData<T>*))
        mppData = lppData;
        mRowCount += inRows;
    }else
    {
        assert(mppPartitionList);
        
        //tliao, 895676, 5/23/2014, add handling for partition case
        long long lGroup = (mRowCount + inRows) >> COL_PRTTN_BIT;
		long long lNewRowCount = mRowCount + inRows - (lGroup << COL_PRTTN_BIT);
        long long lOldRowCount = lNewRowCount - 1;
		
		strData<T>** lpOldPartition = NULL;
		int hr = hGetPartition(lGroup, &lpOldPartition);
		if (lpOldPartition && hr == S_OK)
		{
            strData<T>** lpNewPartition = NULL;
            MEM_GOVERNOR_RESERVE(0, lNewRowCount * sizeof(strData<T>*))
            lpNewPartition = new strData<T>*[lNewRowCount];
            MEM_GOVERNOR_COMMIT(0)
            //lwang, 09/17/2008, 326418; init mData to 0
            if (mDataType != DssDataTypeDate && mDataType != DssDataTypeTimeStamp && mDataType != DssDataTypeTime && mDataType != DssDataTypeCellFormatData && mDataType !=DssDataTypeBigDecimal)
                memset((void*)lpNewPartition, 0, lNewRowCount * sizeof(strData<T>*));
            
            memmove(lpNewPartition, lpOldPartition, lOldRowCount * sizeof(strData<T>*));
            
            delete [] lpOldPartition;
            MEM_GOVERNOR_RELEASE(lOldRowCount * sizeof(strData<T>*),lOldRowCount * sizeof(strData<T>*));
            
            mppPartitionList[lGroup] = lpNewPartition;
        }
        mRowCount += inRows;
    }
    return hr;
}

template<class T>
bool DSSDataColumnString<T>::hNeedPartition()
{
    return mRowCount >= 1024;
}

template<class T>
int DSSDataColumnString<T>::hCreatePartition()
{
	long long lnPart = (mRowCount >> COL_PRTTN_BIT) + 1;
	mnPartitions = lnPart;
	
	long long lReserveSize = lnPart * sizeof(strData<T>**);
    MEM_GOVERNOR_RESERVE(lReserveSize, mRowCount * sizeof(strData<T>*));
	mppPartitionList = new strData<T>**[lnPart];
	CHECK_NEWED_PTR(mppPartitionList);
    MEM_GOVERNOR_COMMIT(lReserveSize);
	
	int i = 0;
	for (i = 0; i < lnPart; i++)
	{
		mppPartitionList[i] = NULL;
	}
	//create partition 0
	//return hGetPartition(0, NULL);
    return S_OK;
}


template<class T>
int DSSDataColumnString<T>::hGetPartition(int iGroup, strData<T>*** oppPartition)
{
	strData<T>** lppPartition = mppPartitionList[iGroup];
	
	if (!lppPartition)
	{
		long long lnRowCount = COLUMN_PARTITION_SIZE;
		
		if (iGroup == mnPartitions - 1)		//the last one:
			lnRowCount = mRowCount - (iGroup << COL_PRTTN_BIT);
		
		long long lReserveSize = (iGroup == mnPartitions - 1) ? 0 : lnRowCount * sizeof(strData<T>**);	//for the last partition, don't report it
        MEM_GOVERNOR_RESERVE(lReserveSize, mRowCount * sizeof(strData<T>*))
		lppPartition = new strData<T>* [lnRowCount];
        MEM_GOVERNOR_COMMIT(lReserveSize)
		memset(lppPartition, 0, sizeof(strData<T>*) * lnRowCount);
		
		mppPartitionList[iGroup] = lppPartition;
	}
	
	if (oppPartition)
	{
		*oppPartition = lppPartition;
	}
	return S_OK;
}

template<class T>
strData<T>* DSSDataColumnString<T>::hGetDataPtr(long long iRow, int& oResult)
{
    strData<T> *lpData = NULL;
    if(mbUsePartition)
    {
        long long lGroup = iRow >> COL_PRTTN_BIT;
        long long lOffset = iRow - (lGroup << COL_PRTTN_BIT);
        
        strData<T>** lpPartition = NULL;
        oResult = hGetPartition(lGroup, &lpPartition);
        if (lpPartition && oResult == S_OK)
        {
            lpData = lpPartition[lOffset];
        }
    }
    else
    {
        lpData = mppData[iRow];
        oResult = S_OK;
    }
    
    return lpData;
}

template<class T>
int DSSDataColumnString<T>::Remove(int iRow)
{
    int hr = S_OK;
    
    if (iRow < 0 || iRow >= mRowCount)
        return E_INVALIDARG;
    
    MEM_GOVERNOR_RESERVE((mRowCount - 1) * sizeof(strData<T>*), (mRowCount - 1) * sizeof(strData<T>*))
    strData<T>** lppData = new strData<T>* [mRowCount - 1];
	CHECK_NEWED_PTR(lppData);    
    memmove(lppData, mppData, iRow * sizeof(strData<T>*));
    memmove(lppData + iRow, mppData + iRow + 1, (mRowCount - iRow - 1) * sizeof(strData<T>*));
    
    //delete mppData[iRow];
    delete [] mppData;
    MEM_GOVERNOR_RELEASE(mRowCount * sizeof(strData<T>*), mRowCount * sizeof(strData<T>*))
    mppData = lppData;
    mRowCount--;
    
    return hr;
}

template<class T>
int DSSDataColumnString<T>::Insert(int iRow)
{
    int hr = S_OK;
    
    if (iRow < 0 || iRow >= mRowCount)
        return E_INVALIDARG;
    
    MEM_GOVERNOR_RESERVE((mRowCount + 1) * sizeof(strData<T>*), (mRowCount + 1) * sizeof(strData<T>*))
    strData<T>** lppData = new strData<T>* [mRowCount + 1];
	CHECK_NEWED_PTR(lppData);
    memmove(lppData, mppData, iRow * sizeof(strData<T>*));
    lppData[mRowCount + iRow] = 0;
    memmove(lppData + iRow + 1, mppData + iRow, (mRowCount - iRow) * sizeof(strData<T>*));
    
    delete [] mppData;
    MEM_GOVERNOR_RELEASE(mRowCount * sizeof(strData<T>*), mRowCount * sizeof(strData<T>*))
    mppData = lppData;
    mRowCount++;
    
    return hr;
}


















