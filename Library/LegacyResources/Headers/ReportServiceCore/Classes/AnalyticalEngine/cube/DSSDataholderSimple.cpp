//
//  DSSDataholderSimple.cpp
//  ReportServiceCore
//
//  Created by Tang, Cheng on 5/18/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#import "DSSDataholderSimple.h"

#import "DSSAEHelper.h"
#import "DSSBinaryDataReader.h"
#import "DSSStrongPtr.h"
#pragma clang diagnostic ignored"-Wdynamic-class-memaccess"
//helper function to compare data
inline int _order(double data1, double data2)
{
	if (data1 < data2) return -1;
	if (data1 > data2) return 1;
	return 0;
}

inline int _order(MDataType::DSSDateTime data1, MDataType::DSSDateTime data2)
{
	return MDataType::DSSDateTime::DateTimeOrder(data1, data2);
}

inline int _order(const MDataType::DSSCellFmtData & data1, const MDataType::DSSCellFmtData& data2)
{
	if (data1 < data2)
		return -1;
	if (data2 < data1)
		return 1;
	return 0;
}
inline int _order(const MDataType::DSSBigDecimal & data1, const MDataType::DSSBigDecimal& data2)
{
	if (data1 < data2)
		return -1;
	if (data2 < data1)
		return 1;
	return 0;
}

template <class T>
DSSDataholderSimple<T>::DSSDataholderSimple():mnRow(-1), mpData(NULL)
{
    
}

template <class T>
int DSSDataholderSimple<T>::count()
{
    return mnRow;
}

template <class T>
DSSDataholderSimple<T>::~DSSDataholderSimple()
{
    if (mpData)
    {
        delete [] mpData;
        mpData = NULL;
    }
}

template class DSSDataholderSimple<short>;
template class DSSDataholderSimple<int>;
template class DSSDataholderSimple<long long>;
template class DSSDataholderSimple<float>;
template class DSSDataholderSimple<double>;
template class DSSDataholderSimple<bool>;
template class DSSDataholderSimple<char>;
template class DSSDataholderSimple<MDataType::DSSDateTime>;
template class DSSDataholderSimple<MDataType::DSSCellFmtData>;
template class DSSDataholderSimple<MDataType::DSSBigDecimal>;


template <class T>
int DSSDataholderSimple<T>::LoadDataFromBinary(DSSBinaryDataReader *ipReadStream)
{
    //todo
    return -1;
}

template <class T>
int DSSDataholderSimple<T>::Init(EnumDSSDataType iDataType, int iRowCount)
{
    if (iRowCount < 0) {
        return E_INVALIDARG;
    }
    
    // Veracode: CWE ID 191: Integer Underflow (Wrap or Wraparound)
	signed long long tmp = (signed long long) iRowCount;
	if (tmp > 0 && tmp <= INT_MAX)
    {
        mnRow = iRowCount;
        mpData = new T[iRowCount];
        CHECK_NEWED_PTR(mpData);
    }
    // Veracode
    return S_OK;
    
}

template <class T>
int DSSDataholderSimple<T>::getRowSize(int iRow, int &orSize)
{
    //should never call this
    orSize = sizeof(T);
    return S_OK;
}

template <class T>
int DSSDataholderSimple<T>::setData(int iRow, char *ipData, int iSizeInByte)
{
    if (iRow >= mnRow || iRow < 0 || !mpData)
		return E_INVALIDARG;
    
    mpData[iRow] = *(T*)ipData;
    return S_OK;
}

template <class T>
int DSSDataholderSimple<T>::setDataToNULL(int iRow, int iSizeInByte)
{
    if (iRow >= mnRow || iRow < 0 || !mpData)
		return E_INVALIDARG;
    
    memset((void*)(mpData + iRow), 0, iSizeInByte);
    return S_OK;
}

template <class T>
int DSSDataholderSimple<T>::getData(int iRow, const char **oppData, int &orSize)
{
    if (iRow >= mnRow || iRow < 0 || !mpData || !oppData)
		return E_INVALIDARG;
    
    (*oppData) = reinterpret_cast<const char *>(mpData + iRow);
    orSize = sizeof(T);
    return S_OK;
}

template <class T>
int DSSDataholderSimple<T>::getDataCopy(int iRow, char *pBuffer, int iBufSize, int &orCopiedSize)
{
    if (iRow >= mnRow || (!pBuffer))
		return E_INVALIDARG;
	
	CHECK_PTR(mpData);
	
	*(T*)pBuffer = mpData[iRow];
    
	return S_OK;
}

template <class T>
int DSSDataholderSimple<T>::setDataInBulk(char *ipData, int iSizeInByte)
{
    if (!ipData || !mpData)
        return E_INVALIDARG;
    memmove((void*)mpData, ipData, iSizeInByte);
    return S_OK;
}


template <>
int DSSDataholderSimple<MDataType::DSSDateTime>::getNumericVal(int iRow, double &orVal, DSSDataFlag &orFlag)
{
	return E_UNEXPECTED_DATATYPE;
}

template<>
int DSSDataholderSimple<MDataType::DSSDateTime>::setNumeric(int iRow, double iVal)
{
	return E_UNEXPECTED_DATATYPE;
}

template <>
int DSSDataholderSimple<MDataType::DSSCellFmtData>::getNumericVal(int iRow, double &orVal, DSSDataFlag &orFlag)
{
	//432181, we can get numerical value for cell format data
	if(iRow >= mnRow)
		return E_INVALIDINDEX;
	
	if (orFlag == DssDataOk)
	{
		MDataType::DSSCellFmtData* lpDataPtr = mpData + iRow;
		CHECK_PTR(lpDataPtr);
		
		orVal = lpDataPtr->GetValue();
	}
	else
		orVal = 0;
	return S_OK;
}

template<>
int DSSDataholderSimple<MDataType::DSSCellFmtData>::setNumeric(int iRow, double iVal)
{
	return E_UNEXPECTED_DATATYPE;
}
template <>
int DSSDataholderSimple<MDataType::DSSBigDecimal>::getNumericVal(int iRow, double &orVal, DSSDataFlag &orFlag)
{
	//471199, try to convert big decimal to double
	if(iRow >= mnRow)
		return E_INVALIDINDEX;
	
	if (orFlag == DssDataOk)
	{
		MDataType::DSSBigDecimal *lpData = mpData + iRow;
		CHECK_PTR(lpData);
		
		orVal = atof(lpData->ConvertToString().c_str());
	}
	else
		orVal = 0;
	
	return S_OK;
}

template<>
int DSSDataholderSimple<MDataType::DSSBigDecimal>::setNumeric(int iRow, double iVal)
{
	return E_UNEXPECTED_DATATYPE;
}

template <class T>
int DSSDataholderSimple<T>::getNumericVal(int iRow, double &orVal, DSSDataFlag &orFlag)
{
    if (iRow >= mnRow || iRow < 0 || !mpData)
         return E_INVALIDARG;
    
    orVal = (double)(mpData[iRow]);
    return S_OK;
}

template <class T>
int DSSDataholderSimple<T>::setNumeric(int iRow, double iVal)
{
    if (iRow >= mnRow || iRow < 0 || !mpData)
        return E_INVALIDARG;
    
    mpData[iRow] = iVal;
    return S_OK;
}

template <class T>
int DSSDataholderSimple<T>::hCompareData(int iRow, EnumDSSDataType iDataType, unsigned char* ipData, int iSize, bool ibSimpleComparison)
{
    if (iRow >= mnRow || iRow < 0 || !mpData)
        return E_INVALIDARG;
    
    return _order(mpData[iRow], * (reinterpret_cast<T*> (ipData)));
}

template <class T>
int DSSDataholderSimple<T>::ExtendColumn(int iRows)
{
    int hr = S_OK;
    
    int lRowCount = mnRow;
    mnRow += iRows;
	T* lpData = new T[mnRow];
    memmove(lpData,mpData, lRowCount * sizeof(T));
    delete[] mpData;
    mpData = lpData;
    return hr;
}