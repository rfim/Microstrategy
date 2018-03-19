//
//  DSSdataholderString.cpp
//  ReportServiceCore
//
//  Created by Tang, Cheng on 5/18/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#import "DSSDataholderString.h"

#import "DSSAEHelper.h"
#import "DSSBinaryDataReader.h"
#import "DSSStrongPtr.h"


inline int lStrcmp(EnumDSSDataType iType, wchar_t * string1, wchar_t * string2, int size1, int size2, int locale=0, bool ibSimpleComparison = false)
{
	if (ibSimpleComparison || locale == 0)
		return wcsncmp(string1, string2, (size1 > size2)? size1 : size2);
	
	return DSSAEHelper::CompareStringsWithLocale(string1, string2, size1, size2, locale);
}

inline int lStrcmp(EnumDSSDataType iType, char * string1, char * string2, int size1, int size2, int locale=0, bool ibSimpleComparison = false)
{
	if(iType == DssDataTypeBinary || iType == DssDataTypeVarBin || iType == DssDataTypeLongVarBin)
	{
		//special handling for binary because of the lack of NULL termination
		//we could compare byte to byte
		int lSmall = size1 > size2 ? size2: size1;
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
			return 1;
		
		return 0;
	}
	
	if (ibSimpleComparison || locale == 0)
		return strncasecmp(string1, string2, (size1 > size2)? size1:size2);
	
	return DSSAEHelper::CompareStringsWithLocale(string1, string2, size1, size2, locale);
}


template <class T>
DSSDataholderString<T>::DSSDataholderString() : mnRow(-1), mppData(NULL), mpRowSize(NULL)
{
}

template <class T>
DSSDataholderString<T>::~DSSDataholderString()
{
    if (mppData)
    {
        for (int i = 0; i < mnRow; i++)
            if (mppData[i])
            {
                delete [] mppData[i];
                mppData[i] = NULL;
            }
        delete [] mppData;
        mppData = NULL;
    }
    if (mpRowSize)
    {
        delete [] mpRowSize;
        mpRowSize = NULL;
    }
}

template class DSSDataholderString<char>;
template class DSSDataholderString<wchar_t>;

template <class T>
int DSSDataholderString<T>::LoadDataFromBinary(DSSBinaryDataReader *ipReadStream)
{
    //todo
    return -1;
}

template <class T>
int DSSDataholderString<T>::Init(EnumDSSDataType iDataType, int iRowCount)
{
    // Veracode: CWE ID 191: Integer Underflow (Wrap or Wraparound)
	signed long long tmp = (signed long long) iRowCount;
	if (tmp > 0 && tmp <= INT_MAX)
    {
        mnRow = iRowCount;
        
        mpRowSize = new int[mnRow];
        CHECK_NEWED_PTR(mpRowSize);
        
        memset(mpRowSize, 0, sizeof(int) * mnRow);
        
        mppData = new T*[mnRow];
        CHECK_NEWED_PTR(mppData);
        
        memset(mppData, 0, sizeof(T*) * mnRow);
    }
    // Veracode
    return S_OK;
}

template <class T>
int DSSDataholderString<T>::getRowSize(int iRow, int &orSize)
{
    if (iRow < 0 || iRow >= mnRow)
        return E_INVALIDARG;
    
    CHECK_PTR(mpRowSize);
    orSize = mpRowSize[iRow];
    return S_OK;
}

template <class T>
int DSSDataholderString<T>::count()
{
    return mnRow;
}

template <class T>
int DSSDataholderString<T>::setData(int iRow, char *ipData, int iSizeInByte)
{
    if (iRow < 0 || iRow >= mnRow)
        return E_INVALIDARG;

    CHECK_PTR(mppData && mpRowSize)
    
    T *lpData = mppData[iRow];
    if (mpRowSize[iRow] < iSizeInByte) {
        if (lpData)
            delete[] lpData; // delete old data
        mppData[iRow] = (T *)(new char [iSizeInByte]);
        lpData = mppData[iRow];
        CHECK_NEWED_PTR(lpData);
    }
    
    memmove(lpData, ipData, iSizeInByte);
    mpRowSize[iRow] = iSizeInByte;
    return S_OK;
}

template <class T>
int DSSDataholderString<T>::setDataToNULL(int iRow, int iSizeInByte)
{
    if (iRow < 0 || iRow >= mnRow)
        return E_INVALIDARG;

    CHECK_PTR(mppData && mpRowSize)
    
    T *lpData = mppData[iRow];
    if (lpData)
        delete[] lpData; // delete old data
    
    lpData = (T *)(new char [sizeof(T)]);
    CHECK_NEWED_PTR(lpData);
    memset(lpData, 0, sizeof(T));
    
    mppData[iRow] = lpData;
    mpRowSize[iRow] = sizeof(T);

    return S_OK;
}

template <class T>
int DSSDataholderString<T>::getData(int iRow, const char **oppData, int &orSize)
{
    if (iRow < 0 || iRow >= mnRow || !oppData)
        return E_INVALIDARG;
    
    CHECK_PTR(mppData);
    
    *oppData = reinterpret_cast<const char*> (mppData[iRow]);
    orSize = mpRowSize[iRow];
    return S_OK;
}

template <class T>
int DSSDataholderString<T>::getDataCopy(int iRow, char *pBuffer, int iBufSize, int &orCopiedSize)
{
    if (iRow >= mnRow || iRow < 0 || (!pBuffer))
		return E_INVALIDARG;
	
    CHECK_PTR(mpRowSize);
    
	if (iBufSize < mpRowSize[iRow])
		return E_INVALIDARG;
    
    orCopiedSize = mpRowSize[iRow];
	memmove(pBuffer, mppData[iRow], orCopiedSize);
    
    return S_OK;
}

template <class T>
int DSSDataholderString<T>::setDataInBulk(char *ipData, int iSizeInByte)
{
    return E_NOTIMPL;
}

template <>
int DSSDataholderString<char>::getNumericVal(int iRow, double &orVal, DSSDataFlag &orFlag)
{
    //467139, handle null strings
	if (iRow >= mnRow || iRow < 0)
		return E_INVALIDARG;
	CHECK_PTR(mppData);
	//454635, there's need from graph matrix widget to convert string data to double
	orVal = atof(mppData[iRow]);
	if (orVal == 0)
	{
		int lSize = mpRowSize[iRow];
		char *lpString = mppData[iRow];
		for (int i = 0; i < lSize; i++)
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
		if (*lpString < '0' || *lpString > '9')
		{
			//invalid string
			return E_NaN;
		}
		
	}
	return S_OK;
}

template<class T>
int DSSDataholderString<T>::getNumericVal(int iRow, double &orVal, DSSDataFlag &orFlag)
{
	return E_NOTIMPL;
}

template<class T>
int DSSDataholderString<T>::setNumeric(int iRow, double iVa)
{
	return E_NOTIMPL;
}

template<class T>
int DSSDataholderString<T>::hCompareData(int iRow, EnumDSSDataType iDataType, unsigned char* ipData, int iSize, bool ibSimpleComparison)
{
    if (iRow >= mnRow || iRow < 0 || !mppData)
        return E_INVALIDARG;
    
    return lStrcmp(iDataType, mppData[iRow], reinterpret_cast<T*> (ipData), mpRowSize[iRow], iSize, 0, ibSimpleComparison);
}

template<class T>
int DSSDataholderString<T>::ExtendColumn(int inRows)
{
    int hr = S_OK;
    
    T** lppData = new T* [inRows + mnRow];
    memmove(lppData,mppData,inRows * sizeof(T*));
    delete [] mppData;
    mnRow += inRows;
    mppData = lppData;
    return hr;
}