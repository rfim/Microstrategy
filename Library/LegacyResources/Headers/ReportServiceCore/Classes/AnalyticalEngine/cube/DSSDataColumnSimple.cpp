/*
 *  DSSDataColumnSimple.cpp
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import <memory.h>
#include <stdlib.h>
#include "DSSDataColumnSimple.h"
#import "DSSDateTime.h"
#import "DSSCellFmtData.h"
#import "DSSBigDecimal.h"
#import "DSSDataSource.h"
#pragma clang diagnostic ignored"-Wdynamic-class-memaccess"
#define CHECK_NEWED_PTR(p) {if (!p) return E_OUT_OF_MEMORY;}

//helper function to compare data
inline int _order(const double &data1, const double &data2)
{
	if (data1 < data2) return -1;
	if (data1 > data2) return 1;
	return 0;
}

inline int _order(const MDataType::DSSDateTime &data1, const MDataType::DSSDateTime &data2)
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
//TODO:
template<class T>
DSSDataColumnSimple<T>::DSSDataColumnSimple() : 
mpData(NULL), mDataSize(0), mppPartitionList(NULL), mnPartitions(0)
,mpMMPBuffer(NULL), mMMPBufSize(0),mbUseMappedFile(false)
,mFilePath("")
{
}

template<class T>
DSSDataColumnSimple<T>::~DSSDataColumnSimple()
{
	long long lDataSize = mRowCount * sizeof(T);
	
    //un map the file buffer
    UnMapFileBuffer();
    
	if (mpData && !mppPartitionList)
	{
		//472269, if we use mppPartitionList, mpData will point to mppPartitionList
		
	    delete[] mpData;
		mpData = NULL;
MEM_GOVERNOR_RELEASE(lDataSize,lDataSize)
	}
	
	if (mppPartitionList)
	{
		int lnPart = (mRowCount >> COL_PRTTN_BIT) + 1;
		int lnReleasedPartions = 0;
		for (int i = 0; i < lnPart; i++)
		{
			if (mppPartitionList[i])
			{
				delete[] mppPartitionList[i];
				if (i < lnPart - 1)
					lnReleasedPartions ++;		//don't count the last partition as its size may be different
			}
			mppPartitionList[i] = NULL;
		}
MEM_GOVERNOR_RELEASE(lnReleasedPartions * COLUMN_PARTITION_SIZE * sizeof(T), lDataSize)		

		delete[] mppPartitionList;
		mppPartitionList = NULL;
MEM_GOVERNOR_RELEASE(lnPart * sizeof(T*), lDataSize)		
	}
	if (mpFlags)
	{
		delete[] mpFlags;
		mpFlags = NULL;
MEM_GOVERNOR_RELEASE(mRowCount * sizeof(DSSDataFlag), lDataSize)
	}
}

template class DSSDataColumnSimple<short>;
template class DSSDataColumnSimple<int>;
template class DSSDataColumnSimple<long long>;
template class DSSDataColumnSimple<float>;
template class DSSDataColumnSimple<double>;
template class DSSDataColumnSimple<bool>;
template class DSSDataColumnSimple<char>;
template class DSSDataColumnSimple<MDataType::DSSDateTime>;
template class DSSDataColumnSimple<MDataType::DSSCellFmtData>;
template class DSSDataColumnSimple<MDataType::DSSBigDecimal>;

template <>
int DSSDataColumnSimple<MDataType::DSSDateTime>::getNumericVal(long long iRow, double &orVal, DSSDataFlag &orFlag)
{
	return E_UNEXPECTED_DATATYPE;
}

template<>
int DSSDataColumnSimple<MDataType::DSSDateTime>::setNumeric(long long iRow, double iVal, DSSDataFlag iFlag)
{
	return E_UNEXPECTED_DATATYPE;
}

template <>
int DSSDataColumnSimple<MDataType::DSSCellFmtData>::getNumericVal(long long iRow, double &orVal, DSSDataFlag &orFlag)
{
	//432181, we can get numerical value for cell format data
	if(iRow >= mRowCount)
		return E_INVALIDINDEX;
	
	orFlag = mpFlags != NULL ? mpFlags[iRow] : DssDataOk;
	if (orFlag == DssDataOk)
	{
		MDataType::DSSCellFmtData* lpDataPtr = NULL;
		int hr = hGetDataPtr(iRow, &lpDataPtr);
		CHECK_HR;
		CHECK_PTR(lpDataPtr);
		
		orVal = lpDataPtr->GetValue();
	}
	else
		orVal = 0;
	return S_OK;
}

template<>
int DSSDataColumnSimple<MDataType::DSSCellFmtData>::setNumeric(long long iRow, double iVal, DSSDataFlag iFlag)
{
	return E_UNEXPECTED_DATATYPE;
}
template <>
int DSSDataColumnSimple<MDataType::DSSBigDecimal>::getNumericVal(long long iRow, double &orVal, DSSDataFlag &orFlag)
{
	//471199, try to convert big decimal to double
	if(iRow >= mRowCount)
		return E_INVALIDINDEX;
	
	orFlag = mpFlags != NULL ? mpFlags[iRow] : DssDataOk;
	if (orFlag == DssDataOk)
	{
		MDataType::DSSBigDecimal *lpData = NULL;
		int hr = hGetDataPtr(iRow, &lpData);
		CHECK_HR;
		CHECK_PTR(lpData);
		
		orVal = atof(lpData->ConvertToString().c_str());
	}
	else
		orVal = 0;
	
	return S_OK;
}

template<>
int DSSDataColumnSimple<MDataType::DSSBigDecimal>::setNumeric(long long iRow, double iVal, DSSDataFlag iFlag)
{
	return E_UNEXPECTED_DATATYPE;
}
template<class T>
int DSSDataColumnSimple<T>::Init(EnumDSSDataType iDataType, long long iRowCount, bool ibUseMappedFile)
{
    if (mpData) {
        MEM_GOVERNOR_RELEASE(mRowCount * sizeof(T),mRowCount * sizeof(T))
        delete[] mpData;
        mpData = NULL;
    }
    
    // Veracode: CWE ID 195: Signed to Unsigned Conversion Error
    if (iRowCount < 0) {
        return E_INVALIDARG;
    }
    // Veracode
	mRowCount = iRowCount;
	mDataType = iDataType;
	mDataSize = sizeof(T);
	mbUseMappedFile = ibUseMappedFile;
	if (iRowCount <= COLUMN_PARTITION_SIZE || ibUseMappedFile)
	{
        if(mRowCount > 0)
        {
            int lReserveDataSize = iRowCount * sizeof(T);
MEM_GOVERNOR_RESERVE(lReserveDataSize, iRowCount * sizeof(T))
            mpData = new T[iRowCount];
            CHECK_NEWED_PTR(mpData);
MEM_GOVERNOR_COMMIT(lReserveDataSize)
            //lwang, 09/17/2008, 326418; init mData to 0
            if (iDataType != DssDataTypeDate && iDataType != DssDataTypeTimeStamp && iDataType != DssDataTypeTime && iDataType != DssDataTypeCellFormatData && iDataType !=DssDataTypeBigDecimal)
                memset((void*)mpData, 0, iRowCount * sizeof(T));
        }
	}
	else
	{
		int hr = CreatePartition();
		CHECK_HR;
		
		mpData = mppPartitionList[0];
	}
    
    //526227, performance, creat flag array on demand
	if (iDataType == DssDataTypeMissing)
    {
        int hr = hCreateFlagArray();
        CHECK_HR;
    }
	/*	for(; i < iRowCount; i++)
			mpFlags[i] = DssDataUnknown;*/
//        memset(mpFlags, DssDataUnknown, iRowCount * sizeof(DSSDataFlag));
		/*for(; i < iRowCount; i++)
			mpFlags[i] = DssDataMissing;*/
//        memset(mpFlags, DssDataMissing, iRowCount * sizeof(DSSDataFlag));
	
	return S_OK;
}

template<class T>
long long DSSDataColumnSimple<T>::getRowCount()
{
    //ctang 2012-7-24
	return mfHasJoinNULL ? mRowCount - 1 : mRowCount;
}

template<class T>
int DSSDataColumnSimple<T>::get_RowCount(int* opRowCount)
{
    if(opRowCount)
    {
        *opRowCount = getRowCount();
    }
    return S_OK;
}

template <class T>
bool DSSDataColumnSimple<T>::IsRefColumn()
{
    return false;
}

template<class T>
int DSSDataColumnSimple<T>::getSizeOfData()
{
	return mDataSize;
}

template<class T>
int DSSDataColumnSimple<T>::getRowSize(long long iRow, int &orSize)
{
	//every row has the same size for simple column
	orSize = mDataSize;
	return S_OK;
}

template<class T>
int DSSDataColumnSimple<T>::setData(long long iRow, char *ipData, int iSize, DSSDataFlag iFlag)
{
	if (iRow >= mRowCount || iSize > mDataSize)
		return E_INVALIDARG;
	
	T* lpData = NULL;
	int hr = hGetDataPtr(iRow, &lpData);
	CHECK_HR;
	CHECK_PTR(lpData);
	
	if (iFlag == DssDataOk)
	{
        if(mDataType == DssDataTypeCellFormatData && iSize == sizeof(double))
        {
            double lVal = *(double*)ipData;
            void* lpTempData = lpData;
            ((MDataType::DSSCellFmtData*)(lpTempData))->setValue(lVal);
        }
        else if (mDivider > 1)
        {
            assert(mDataType == DssDataTypeInteger);
            int * lpDataTemp = reinterpret_cast<int *>(lpData);
            double lData = *reinterpret_cast<double *>(ipData) * mDivider;
            lData += lData > 0 ? 0.5 : -0.5;
            *lpDataTemp = (int)lData;
        }
        else
            *lpData = *(T *)ipData;
        if(mpFlags && mpFlags[iRow] != DssDataOk)
        {
            mpFlags[iRow] = DssDataOk;
        }
	}
	else
    {
        if (!mpFlags)
        {
            hr = hCreateFlagArray();
            CHECK_PTR(mpFlags);
        }
        mpFlags[iRow] = iFlag;
        
        if (mDataType != DssDataTypeDate && mDataType != DssDataTypeTimeStamp && mDataType != DssDataTypeTime && mDataType != DssDataTypeCellFormatData && mDataType !=DssDataTypeBigDecimal) 
        {
            memset((void*)lpData, 0, mDataSize);
        }
    }
	
	if(iFlag != DssDataOk)
		this->mbAllDataValid = false;
	
	return S_OK;
}

template<class T>
int DSSDataColumnSimple<T>::getData(long long iRow, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag)
{
	if (iRow >= mRowCount || iRow < 0 || !oppData)
		return E_INVALIDARG;
	
	orSize = mDataSize;
	orType = mDataType;
	orFlag = mpFlags != NULL ? mpFlags[iRow] : DssDataOk;
	T* lpDataPtr = NULL;
	int hr = hGetDataPtr(iRow, &lpDataPtr);
	CHECK_HR;
	CHECK_PTR(lpDataPtr);
    if (mDivider > 1) {
        assert(mDataType == DssDataTypeInteger);
        orType = DssDataTypeDouble;
        orSize = sizeof(double);
        int* lpData = reinterpret_cast<int *>(lpDataPtr);
        double lData = (double)(*lpData)/mDivider;
        (*oppData) = reinterpret_cast<const char *>(&lData);
        return S_OK;
    }
	(*oppData) = reinterpret_cast<const char *>(lpDataPtr);
	
	return S_OK;
}

template<class T>
int DSSDataColumnSimple<T>::getDataCopy(long long iRow, char *pBuffer, int iBufSize,
									 int &orCopiedSize, EnumDSSDataType &orType, DSSDataFlag &orFlag)
{
	if (iRow >= mRowCount || (!pBuffer) || iBufSize < mDataSize)
		return E_INVALIDARG;
	
	T* lpDataPtr = NULL;
	int hr = hGetDataPtr(iRow, &lpDataPtr);
	CHECK_HR;
	CHECK_PTR(lpDataPtr);
	
	*(T*)pBuffer = *lpDataPtr;
	orCopiedSize = mDataSize;
	orType = mDataType;
	orFlag = mpFlags != NULL ? mpFlags[iRow] : DssDataOk;
	
	return S_OK;
}

//set data to mpData [iStart, iEnd)
template<class T>
int DSSDataColumnSimple<T>::setDataInBulk(long long iStart, long long iEnd, char *ipData, int iSizeInByte, DSSDataFlag *ipFlags, int iSIBFlags)
{
	if (iStart < 0 || iEnd > mRowCount || iStart > iEnd)
		return E_INVALIDINDEX;
	
    //fali, currently this method is only called by loadCubeSlice in which the mDivider is handled separatedly. No support now.
    if (mDivider > 1)
        return E_INVALIDARG;

	long long lnRows = iEnd - iStart;
	
	if (lnRows * mDataSize != iSizeInByte || !ipData)
		return E_INVALIDARG;

	if (iSIBFlags > 0 && (lnRows * sizeof(DSSDataFlag) != iSIBFlags || !ipFlags))
		return E_INVALIDARG;

	int hr = S_OK;
	
	if (mRowCount <= COLUMN_PARTITION_SIZE || mbUseMappedFile)
	{
		memmove((void*)(mpData + iStart), ipData, iSizeInByte);
	}
	else
	{
		long long lStartGroup = iStart >> COL_PRTTN_BIT;
		long long lStartOffset = iStart - (lStartGroup << COL_PRTTN_BIT);
		
		long long lEndGroup = iEnd >> COL_PRTTN_BIT;
		long long lEndOffset = iEnd - (lEndGroup << COL_PRTTN_BIT);
		
		T* lpPartition = NULL;
		hr = hGetPartition(lStartGroup, &lpPartition);
		CHECK_HR;
		
		if (lStartGroup == lEndGroup)
		{
			memmove((void*)(lpPartition + lStartOffset), ipData, iSizeInByte);
		}
		else
		{
			long long count = (COLUMN_PARTITION_SIZE - lStartOffset) * mDataSize;
			memmove((void*)(lpPartition + lStartOffset), ipData, count);
			
			long long lBytesOfPartition = COLUMN_PARTITION_SIZE * mDataSize;
			for (long long p = lStartGroup + 1; p <= lEndGroup - 1; p++)
			{
				hr = hGetPartition(p, &lpPartition);
				CHECK_HR;
				
				memmove((void*)lpPartition, ipData + count, lBytesOfPartition);
				count += lBytesOfPartition;
			}
			
			hr = hGetPartition(lEndGroup, &lpPartition);
			CHECK_HR;
			
			memmove((void*)lpPartition, ipData + count, lEndOffset * mDataSize);
			count += lEndOffset * mDataSize;
			_ASSERT(count == iSizeInByte);
		}
	}

	
	if (iSIBFlags == 0)		//all valid
	{
        if (mpFlags)
        {
            memset(mpFlags + iStart, 0, sizeof(DSSDataFlag) * lnRows);
        }
		return S_OK;
	}
	else
	{
		return setFlags(iStart, iEnd, ipFlags, iSIBFlags);
	}
}

template<class T>
int DSSDataColumnSimple<T>::getNumericVal(long long iRow, double &orVal, DSSDataFlag &orFlag)
{
	if(iRow >= mRowCount)
		return E_INVALIDINDEX;
	
	T* lpDataPtr = NULL;
	int hr = hGetDataPtr(iRow, &lpDataPtr);
	CHECK_HR;
	CHECK_PTR(lpDataPtr);
	orVal = (double) (*lpDataPtr);
    if (mDivider > 1) {
        orVal = orVal/mDivider;
    }
	orFlag = mpFlags != NULL ? mpFlags[iRow] : DssDataOk;
	return S_OK;
}

template<class T>
int DSSDataColumnSimple<T>::setNumeric(long long iRow, double iVal, DSSDataFlag iFlag)
{
	if(iRow >= mRowCount)
		return E_INVALIDINDEX;
	
	if(iFlag != DssDataOk)
	{
		iVal = 0;
		mbAllDataValid = false;
	}
	
	T* lpDataPtr = NULL;
	int hr = hGetDataPtr(iRow, &lpDataPtr);
	CHECK_HR;
	CHECK_PTR(lpDataPtr);
	
	*lpDataPtr = iVal;
    if (mDivider > 1) {
        assert(mDataType == DssDataTypeInteger);
        double lData = iVal * mDivider;
        lData += lData > 0 ? 0.5 : -0.5;
        *lpDataPtr = (int)lData;
    }
    //if (iFlag != DssDataOk)
    {
        if (!mpFlags)
        {
            hr = hCreateFlagArray();
            CHECK_HR;
            CHECK_PTR(mpFlags);
        }
        mpFlags[iRow] = iFlag;
    }
	
	return S_OK;
}

template<class T>
int DSSDataColumnSimple<T>::getFlag(long long iRow, DSSDataFlag &orFlag)
{
	if(iRow >= mRowCount)
		return E_INVALIDINDEX;
	orFlag = mpFlags != NULL ? mpFlags[iRow] : DssDataOk;
	return S_OK;
}

template<class T>
int DSSDataColumnSimple<T>::setFlag(long long iRow, DSSDataFlag iFlag)
{
	if(iRow >= mRowCount)
		return E_INVALIDINDEX;
	
    if (!mpFlags)
    {
        int hr = hCreateFlagArray();
        CHECK_HR;
        CHECK_PTR(mpFlags);
    }
    mpFlags[iRow] = iFlag;
    
	if (iFlag != DssDataOk)
	{   
		if (mDataType != DssDataTypeDate && mDataType != DssDataTypeTimeStamp && mDataType != DssDataTypeTime && mDataType != DssDataTypeCellFormatData && mDataType !=DssDataTypeBigDecimal) 
		{
			T* lpDataPtr = NULL;
			int hr = hGetDataPtr(iRow, &lpDataPtr);
			CHECK_HR;
			CHECK_PTR(lpDataPtr);
			
			memset((void*)lpDataPtr, 0, mDataSize);
		}
		if(mbAllDataValid)
			mbAllDataValid = false;
	}
	
	return S_OK;
}

template<class T>
int DSSDataColumnSimple<T>::setFlags(long long iStart, long long iEnd, DSSDataFlag *ipFlags, int iSizeInByte)
{
	//if (mRowCount >= COLUMN_PARTITION_SIZE)
	//	return E_NOTIMPL;		//TODO: not implemented
	
	long long lnRows = iEnd - iStart;
	if (lnRows * sizeof(DSSDataFlag) != iSizeInByte || !ipFlags)
		return E_POINTER; 

    if (!mpFlags)
    {
        //if this API is called, ipFlags must contain Null flag
        int hr = hCreateFlagArray();
        CHECK_HR;
        CHECK_PTR(mpFlags);
    }
	memmove(mpFlags + iStart, ipFlags, iSizeInByte);
	
	//check each data flag
	bool lbAllValid = true;
	long long i = 0;
	if (mDataType != DssDataTypeDate && mDataType != DssDataTypeTimeStamp && mDataType != DssDataTypeTime && mDataType != DssDataTypeCellFormatData && mDataType !=DssDataTypeBigDecimal) 
	{
		for(i = iStart; i < iEnd; i++)
		{
			if(ipFlags[i] != DssDataOk)
			{
				T* lpData = NULL;
				int hr = hGetDataPtr(i, &lpData);
				CHECK_HR;
				CHECK_PTR(lpData);
				
				memset((void*)lpData, 0, mDataSize);
				if (lbAllValid)
					lbAllValid = false;
			}
		}
	}
	else {
		for(i = iStart; i < iEnd; i++)
		{
			if(ipFlags[i] != DssDataOk)
			{
				lbAllValid = false;
				break;			//don't reset memory for other data types
			}
		}
	}
	mbAllDataValid = lbAllValid;
	return S_OK;
}

template<class T>
int DSSDataColumnSimple<T>::setFlags(long long iStart, long long iEnd, DSSDataFlag *ipFlags, int* iElementList, int iElementSize)
{
	//if (mRowCount >= COLUMN_PARTITION_SIZE)
	//	return E_NOTIMPL;		//TODO: not implemented
	long long lnRows = iEnd - iStart;
	if (lnRows  != iElementSize || !ipFlags)
		return E_POINTER; 

    if (!mpFlags)
    {
        //if this API is called, ipFlags must contain Null flag
        int hr = hCreateFlagArray();
        CHECK_HR;
        CHECK_PTR(mpFlags);
    }
	memset(mpFlags, 0, mRowCount*sizeof(DSSDataFlag));
	
	for(long long i = 0; i < iElementSize;i++)
	{
		if(iElementList[i] < 0 || iElementList[i] >= mRowCount)
			return E_INVALIDARG;
		
		mpFlags[iElementList[i]] = ipFlags[iStart + i];
	}
	
	//check each data flag
	bool lbAllValid = true;
	long long i = 0;
	if (mDataType != DssDataTypeDate && mDataType != DssDataTypeTimeStamp && mDataType != DssDataTypeTime && mDataType != DssDataTypeCellFormatData && mDataType !=DssDataTypeBigDecimal) 
	{
		for(i = iStart; i < iEnd; i++)
		{
			if(ipFlags[i] != DssDataOk)
			{
				T* lpData = NULL;
				int hr = hGetDataPtr(/*ctang: fix partial problem*/iElementList ? iElementList[i] : i, &lpData);
				CHECK_HR;
				CHECK_PTR(lpData);
				
				memset((void*)lpData, 0, mDataSize);
				if (lbAllValid)
					lbAllValid = false;
			}
		}
	}
	else {
		for(i = iStart; i < iEnd; i++)
		{
			if(ipFlags[i] != DssDataOk)
			{
				lbAllValid = false;
				break;			//don't reset memory for other data types
			}
		}
	}
	mbAllDataValid = lbAllValid;
	return S_OK;
}

template<class T>
int DSSDataColumnSimple<T>::Order(long long iRow1, long long iRow2)
{
	if (iRow1 < mRowCount && iRow2 < mRowCount)
	{
        //zhyang, 801671
        if(mpInputRank)
            return mpInputRank[iRow1] < mpInputRank[iRow2] ? -1 : 1;
        
		if (mpFlags == NULL || (mpFlags[iRow1] == DssDataOk && mpFlags[iRow2] == DssDataOk))
		{
			int hr = S_OK;
			T* lpData1 = NULL;
			hr = hGetDataPtr(iRow1, &lpData1);
			if(hr != S_OK || !lpData1)
				return 0;
			
			T* lpData2 = NULL;
			hr = hGetDataPtr(iRow2, &lpData2);
			if(hr != S_OK || !lpData2)
				return 0;
			
			if (*lpData1 < *lpData2)
				return -1;
			else if (!(*lpData1 == *lpData2))
				return 1;
			else
				return 0;
		}
		else if (mpFlags[iRow1] == DssDataOk)	//iRow1 is non-NULL and iRow2 is NULL
			return 1;
		else
			return mpFlags[iRow2] == DssDataOk ? -1 : 0;	//iRow1 is NULL
	}
	else if (iRow1 < mRowCount)	//iRow2 is invalid, treat it as NULL and smaller than iRow1
		return 1;
	else
		return iRow2 < mRowCount ? -1 : 0;	//treate as equal if both are invalid, or smalller is iRow2 is valid
}


template<class T>
int DSSDataColumnSimple<T>::Order(long long iRow1, long long iRow2,bool fIsDisplayNullOnTop)
{
	if (iRow1 < mRowCount && iRow2 < mRowCount)
	{
        //zhyang, 801671
        if(mpInputRank)
            return mpInputRank[iRow1] < mpInputRank[iRow2] ? -1 : 1;
        
		if (mpFlags == NULL || (mpFlags[iRow1] == DssDataOk && mpFlags[iRow2] == DssDataOk))
		{
			int hr = S_OK;
			T* lpData1 = NULL;
			hr = hGetDataPtr(iRow1, &lpData1);
			if(hr != S_OK || !lpData1)
				return 0;
			
			T* lpData2 = NULL;
			hr = hGetDataPtr(iRow2, &lpData2);
			if(hr != S_OK || !lpData2)
				return 0;
			
			if (*lpData1 < *lpData2)
				return -1;
			else if (!(*lpData1 == *lpData2))
				return 1;
			else
				return 0;
		}
	    else
        {
            return hCompareFlag(mpFlags[iRow1], mpFlags[iRow2], fIsDisplayNullOnTop);
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
inline int DSSDataColumnSimple<T>::hCompareRow(long long iRow1, long long iRow2, bool iIsNullDisplayOnTop,int iLocale,  bool ibSimpleComparison)
{
	int hr = S_OK;
    if(mpFlags == NULL ||(mpFlags[iRow1] == DssDataOk && mpFlags[iRow2] == DssDataOk))
    {
        //zhyang, 801671
        if(mpInputRank)
            return mpInputRank[iRow1] < mpInputRank[iRow2] ? -1 : 1;
        
        T* lpData1 = NULL;
        T* lpData2 = NULL;
        
        hr = hGetDataPtr(iRow1,&lpData1);
        if(hr != S_OK || !lpData1)
            return 0;
        
        hr = hGetDataPtr(iRow2,&lpData2);
        if(hr != S_OK || !lpData2)
            return 0;
        
        return _order(*lpData1, *lpData2);
    }
    else
        return hCompareFlag(mpFlags[iRow1], mpFlags[iRow2], iIsNullDisplayOnTop);
}

template<class T>
inline int DSSDataColumnSimple<T>::hCompareData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags, bool iIsNullDisplayOnTop, bool ibSimpleComparison)
{
	int hr = S_OK;
    if(iFlags == DssDataOk && (mpFlags == NULL || mpFlags[iRow] == DssDataOk))
    {
        T* lpDataPtr = NULL;
        
        hr = hGetDataPtr(iRow,&lpDataPtr);
        if(hr != S_OK || !lpDataPtr)
            return 0;
        
        return _order(*lpDataPtr, * (reinterpret_cast<T*> (ipData)));
    }
    else
        return hCompareFlag(mpFlags != NULL ? mpFlags[iRow] : DssDataOk,
                            iFlags, iIsNullDisplayOnTop);
}

template<class T>
int DSSDataColumnSimple<T>::size()
{
	int lSize = sizeof(this);
	
	lSize += DSSDataColumn::size();
	
	if (mRowCount > 0)
	{
		if 	(mRowCount <= COLUMN_PARTITION_SIZE || mbUseMappedFile)
		{
			lSize += sizeof(T) * mRowCount;
		}
		else
		{
			if (mppPartitionList)
			{
				
				int lnPart = mnPartitions;
				
				_ASSERT(lnPart > 1);
				
				lSize += sizeof(T*) * lnPart;
				for (int i = 0; i < lnPart; i++)
				{
					if (mppPartitionList[i])
					{
						lSize += sizeof(T) * COLUMN_PARTITION_SIZE;
					}
				}
				lSize -= COLUMN_PARTITION_SIZE - (mRowCount % COLUMN_PARTITION_SIZE);
			}
		}
	}
	return lSize;
}

template<class T>
int DSSDataColumnSimple<T>::CreatePartition()
{
	long long lnPart = (mRowCount >> COL_PRTTN_BIT) + 1;
	mnPartitions = lnPart;
	
	long long lReserveSize = lnPart * sizeof(T*);
MEM_GOVERNOR_RESERVE(lReserveSize, mRowCount * sizeof(T));
	mppPartitionList = new T*[lnPart];
	CHECK_NEWED_PTR(mppPartitionList);
MEM_GOVERNOR_COMMIT(lReserveSize);
	
	int i = 0;
	for (i = 0; i < lnPart; i++)
	{
		mppPartitionList[i] = NULL;
	}
	//create partition 0
	return hGetPartition(0, NULL);
}

template<class T>
int DSSDataColumnSimple<T>::getRawDataAndFlags(void** opData, DSSDataFlag** opDataFlags)
{
	if (mRowCount <= COLUMN_PARTITION_SIZE || mbUseMappedFile)
	{
		if (opData)
			*opData = mpData;
		if (opDataFlags)
        {
            if (!mpFlags)
            {
                int hr =hCreateFlagArray(DssDataOk);
                CHECK_HR;
                CHECK_PTR(mpFlags);
            }
			*opDataFlags = mpFlags;
        }
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

template<class T>
int DSSDataColumnSimple<T>::hGetPartition(long long iGroup, T** oppPartition)
{
	T* lpPartition = mppPartitionList[iGroup];
	
	if (!lpPartition)
	{
		long long lnRowCount = COLUMN_PARTITION_SIZE;
		
		if (iGroup == mnPartitions - 1)		//the last one:
			lnRowCount = mRowCount - (iGroup << COL_PRTTN_BIT);
		
		long long lReserveSize = (iGroup == mnPartitions - 1) ? 0 : lnRowCount * sizeof(T);	//for the last partition, don't report it
MEM_GOVERNOR_RESERVE(lReserveSize, mRowCount * sizeof(T))
		lpPartition = new T[lnRowCount];
MEM_GOVERNOR_COMMIT(lReserveSize)		
		//lwang, 09/17/2008, 326418; init mData to 0
		if (mDataType != DssDataTypeDate && mDataType != DssDataTypeTimeStamp && mDataType != DssDataTypeTime && mDataType != DssDataTypeCellFormatData && mDataType !=DssDataTypeBigDecimal) 
			memset((void*)lpPartition, 0, lnRowCount * sizeof(T));
		
		mppPartitionList[iGroup] = lpPartition;
	}
	
	if (oppPartition)
	{
		*oppPartition = lpPartition;
	}
	return S_OK;
}

template<class T>
int DSSDataColumnSimple<T>::hGetDataPtr(long long iRow, T** oppDataPtr)
{
	if (mRowCount <= COLUMN_PARTITION_SIZE || mbUseMappedFile)
	{
		if(oppDataPtr)
			(*oppDataPtr) = mpData + iRow;
		return S_OK;
	}
	else
	{
		long long lGroup = iRow >> COL_PRTTN_BIT;
		long long lOffset = iRow - (lGroup << COL_PRTTN_BIT);
		
		T* lpPartition = NULL;
		int hr = hGetPartition(lGroup, &lpPartition);
		if (lpPartition && hr == S_OK)
		{
			if(oppDataPtr)
				(*oppDataPtr) = lpPartition + lOffset;
			
			return hr;
		}
		else
		{
//			_ASSERT(false);		//out of memory
			if(oppDataPtr)
				(*oppDataPtr) = mpData;
			
			return hr;
		}
	}
}

template<class T>
void DSSDataColumnSimple<T>::RecycleMemory()
{
	if (mppPartitionList)
	{
		long long lDataSize = mRowCount * sizeof(T);
		long long lnPart = (mRowCount >> COL_PRTTN_BIT) + 1;
		long long lnReleasedPartions = 0;
		for (long long i = 1; i < lnPart; i++)		//the first partition can't be released as it's shared by mpData
		{
			if (mppPartitionList[i])
			{
				delete[] mppPartitionList[i];
				if (i < lnPart - 1)
					lnReleasedPartions ++;		//don't count the last partition as its size may be different
			}
			mppPartitionList[i] = NULL;
		}
		
MEM_GOVERNOR_RELEASE(lnReleasedPartions * COLUMN_PARTITION_SIZE * sizeof(T), lDataSize)		
	}
	return;
}

template<class T>
int DSSDataColumnSimple<T>::hCreateFlagArray(EnumDSSDataFlag iFlag)
{
    long long iRowCount = mRowCount;
    
	long long lReserveSize = iRowCount * sizeof(DSSDataFlag);
    MEM_GOVERNOR_RESERVE(lReserveSize, iRowCount * sizeof(T))
	mpFlags = new DSSDataFlag [iRowCount];
	CHECK_NEWED_PTR(mpFlags);
    MEM_GOVERNOR_COMMIT(lReserveSize)
	
	//long long i = 0;

	if (mDataType == DssDataTypeMissing) {
        //xhan 10/21/04 missing column special treatment
        /*for(; i < iRowCount; i++)
			mpFlags[i] = DssDataMissing;*/
        memset(mpFlags, DssDataMissing, iRowCount * sizeof(DSSDataFlag));
    }
    else 
    {
		/*for(; i < iRowCount; i++)
			mpFlags[i] = iFlag;*/
        memset(mpFlags, iFlag, iRowCount * sizeof(DSSDataFlag));
	} 

    
	//lwang, 12/26/2008, 341680
	if (mDataType != DssDataTypeMissing)
		mbAllDataValid = true;
	else
		mbAllDataValid = false;
    
    return S_OK;
}

template<class T>
int DSSDataColumnSimple<T>::reserveMemForStrData()
{
	return 0;
}

template<class T>
int DSSDataColumnSimple<T>::createDataFlag(EnumDSSDataFlag iFlag)
{
    int hr = hCreateFlagArray(iFlag);
    return hr;
}

template<class T>
int DSSDataColumnSimple<T>::createMMapMemory(const char * ipFilePath, bool ibUseNewFile)
{   
    mFilePath = ipFilePath;
    int lDataSize = mRowCount * sizeof(T);
    if (lDataSize < MEM_GOVERNOR_SIZE)
        return S_OK;
    
    void *lpBuffer = NULL;
    const void *lpData = mpData;
    int hr = DataSourceUtil::createMappedFileBuffer(1, &lpData, &lDataSize, ipFilePath, &ibUseNewFile, &mpMMPBuffer, &mMMPBufSize, &lpBuffer, &mOffset, &mOffsetAligned);
    CHECK_HR;
    
    delete [] mpData;
    mpData = (T*)lpBuffer;
    return hr;
}

template<class T>
int DSSDataColumnSimple<T>::MapFileBuffer()
{
    int hr = S_OK;
    if(mFilePath != "" && !mpMMPBuffer)
    {
        hr = DataSourceUtil::reloadMappedFileBuffer(mFilePath.c_str(), mMMPBufSize, mOffset, mOffsetAligned, 
                                                    (void**) &mpMMPBuffer, (void**) &mpData);
        CHECK_HR;
    }
    return S_OK;
}

template<class T>
int DSSDataColumnSimple<T>::UnMapFileBuffer()
{
    if(mFilePath != "" && mpMMPBuffer)
    {
        int hr = DataSourceUtil::releaseMappedFileBuffer(mpMMPBuffer, mMMPBufSize);
        CHECK_HR;
        mpMMPBuffer = NULL;
        mpData = NULL;
    }
    return S_OK;
}

template <class T>
int DSSDataColumnSimple<T>::ExtendColumn(int iRows)
{
    //fali, this method can only support extend 1 row currently
    int hr = S_OK;
    //transaction don't support map file
    if(mFilePath!="")
        return S_FALSE;
    
    int lRowCount = mRowCount;
    mRowCount += iRows;
    
    if (mpData && !mppPartitionList)
	{
        MEM_GOVERNOR_RESERVE(mRowCount * sizeof(T), mRowCount * sizeof(T))
		T* lpData = new T[mRowCount];
        CHECK_NEWED_PTR(lpData);
        memset((void*)lpData, 0, mRowCount * sizeof(T));
        memmove((void*)lpData,(void*)mpData, lRowCount * sizeof(T));

        MEM_GOVERNOR_COMMIT((mRowCount) * sizeof(T))
	    delete[] mpData;
        MEM_GOVERNOR_RELEASE(lRowCount * sizeof(T),lRowCount * sizeof(T))
		mpData = lpData;
	}
	
	if (mppPartitionList)
	{
        //tliao, 895676, 5/23/2014, add handling for partition case
        long long lGroup = mRowCount >> COL_PRTTN_BIT;
		long long lNewRowCount = mRowCount - (lGroup << COL_PRTTN_BIT);
        long long lOldRowCount = lNewRowCount - 1;
		
		T* lpOldPartition = NULL;
		int hr = hGetPartition(lGroup, &lpOldPartition);
		if (lpOldPartition && hr == S_OK)
		{
            T* lpNewPartition = NULL;
            MEM_GOVERNOR_RESERVE(0, lNewRowCount * sizeof(T))
            lpNewPartition = new T[lNewRowCount];
            MEM_GOVERNOR_COMMIT(0)
            //lwang, 09/17/2008, 326418; init mData to 0
            if (mDataType != DssDataTypeDate && mDataType != DssDataTypeTimeStamp && mDataType != DssDataTypeTime && mDataType != DssDataTypeCellFormatData && mDataType !=DssDataTypeBigDecimal)
                memset((void*)lpNewPartition, 0, lNewRowCount * sizeof(T));
            
            memmove((void*)lpNewPartition, (void*)lpOldPartition, lOldRowCount * sizeof(T));
            
            delete [] lpOldPartition;
            MEM_GOVERNOR_RELEASE(lOldRowCount * sizeof(T),lOldRowCount * sizeof(T));
            
            mppPartitionList[lGroup] = lpNewPartition;
        }
	}
    
    if (mpFlags)
	{
        MEM_GOVERNOR_RESERVE(mRowCount * sizeof(DSSDataFlag), mRowCount * sizeof(DSSDataFlag))
		DSSDataFlag* lpFlag = new DSSDataFlag[mRowCount];
        CHECK_NEWED_PTR(lpFlag);
        memset(lpFlag, DssDataNull, mRowCount * sizeof(DSSDataFlag));
        memmove(lpFlag,mpFlags, lRowCount * sizeof(DSSDataFlag));
  
        MEM_GOVERNOR_COMMIT(mRowCount * sizeof(DSSDataFlag))
		delete[] mpFlags;
		mpFlags = lpFlag;
        MEM_GOVERNOR_RELEASE(lRowCount * sizeof(DSSDataFlag), lRowCount * sizeof(DSSDataFlag))
	}
    
    return hr;
}

template <>
int DSSDataColumnSimple<MDataType::DSSBigDecimal>::ExtendColumn(int iRows)
{
    //fali, this method can only support extend 1 row currently
    int hr = S_OK;
    //transaction don't support map file
    if(mFilePath!="")
        return S_FALSE;
    
    int lRowCount = mRowCount;
    mRowCount += iRows;
    
    if (mpData && !mppPartitionList)
	{
        MEM_GOVERNOR_RESERVE(mRowCount * sizeof(MDataType::DSSBigDecimal), mRowCount * sizeof(MDataType::DSSBigDecimal))
		MDataType::DSSBigDecimal* lpData = new MDataType::DSSBigDecimal[mRowCount];
        CHECK_NEWED_PTR(lpData);

        for (int iRowIndex=0; iRowIndex<lRowCount; iRowIndex++)
        {
            lpData[iRowIndex] = mpData[iRowIndex];
        }
        
        MEM_GOVERNOR_COMMIT((mRowCount) * sizeof(MDataType::DSSBigDecimal))
	    delete[] mpData;
        MEM_GOVERNOR_RELEASE(lRowCount * sizeof(MDataType::DSSBigDecimal),lRowCount * sizeof(MDataType::DSSBigDecimal))
		mpData = lpData;
	}
	
	if (mppPartitionList)
	{
        //tliao, 895676, 5/23/2014, add handling for partition case
        long long lGroup = mRowCount >> COL_PRTTN_BIT;
		long long lNewRowCount = mRowCount - (lGroup << COL_PRTTN_BIT);
        long long lOldRowCount = lNewRowCount - 1;
		
		MDataType::DSSBigDecimal* lpOldPartition = NULL;
		int hr = hGetPartition(lGroup, &lpOldPartition);
		if (lpOldPartition && hr == S_OK)
		{
            MDataType::DSSBigDecimal* lpNewPartition = NULL;
            MEM_GOVERNOR_RESERVE(0, lNewRowCount * sizeof(MDataType::DSSBigDecimal))
            lpNewPartition = new MDataType::DSSBigDecimal[lNewRowCount];
            MEM_GOVERNOR_COMMIT(0)
            
            for (int iRowIndex=0; iRowIndex<lOldRowCount; iRowIndex++)
            {
                lpNewPartition[iRowIndex] = lpOldPartition[iRowIndex];
            }
            
            delete [] lpOldPartition;
            MEM_GOVERNOR_RELEASE(lOldRowCount * sizeof(MDataType::DSSBigDecimal),lOldRowCount * sizeof(MDataType::DSSBigDecimal));
            
            mppPartitionList[lGroup] = lpNewPartition;
        }
	}
    
    if (mpFlags)
	{
        MEM_GOVERNOR_RESERVE(mRowCount * sizeof(DSSDataFlag), mRowCount * sizeof(DSSDataFlag))
		DSSDataFlag* lpFlag = new DSSDataFlag[mRowCount];
        CHECK_NEWED_PTR(lpFlag);
        memset(lpFlag, DssDataNull, mRowCount * sizeof(DSSDataFlag));
        memmove(lpFlag,mpFlags, lRowCount * sizeof(DSSDataFlag));
        
        MEM_GOVERNOR_COMMIT(mRowCount * sizeof(DSSDataFlag))
		delete[] mpFlags;
		mpFlags = lpFlag;
        MEM_GOVERNOR_RELEASE(lRowCount * sizeof(DSSDataFlag), lRowCount * sizeof(DSSDataFlag))
	}
    
    return hr;
}

template <class T>
int DSSDataColumnSimple<T>::Remove(int iRow)
{
    int hr = S_OK;
    //transaction don't support map file
    if(mFilePath!="")
        return S_FALSE;
    
    if (iRow < 0 || iRow >= mRowCount)
        return E_INVALIDARG;
    
    int lRowCount = mRowCount;
    mRowCount--;
    
    if (mpData && !mppPartitionList)
    {
        MEM_GOVERNOR_RESERVE(mRowCount * sizeof(T), mRowCount * sizeof(T))
        T* lpData = new T[mRowCount];
        CHECK_NEWED_PTR(lpData);
        memmove(lpData, mpData, iRow * sizeof(T));
        memmove(lpData+iRow, mpData+iRow+1, (lRowCount-iRow-1) * sizeof(T));
        delete[] mpData;
        MEM_GOVERNOR_RELEASE(lRowCount * sizeof(T),lRowCount * sizeof(T))
        mpData = lpData;
    }
    
    if (mppPartitionList)
	{
		//fali, 913842, handle partition cases
        long long lStartGroup = iRow >> COL_PRTTN_BIT;
		long long lStartOffset = iRow - (lStartGroup << COL_PRTTN_BIT);
		
		long long lEndGroup = mRowCount >> COL_PRTTN_BIT;
		long long lEndOffset = mRowCount - (lEndGroup << COL_PRTTN_BIT);
		
		T* lpPartition = NULL;
		hr = hGetPartition(lStartGroup, &lpPartition);
		CHECK_HR;
		
		if (lStartGroup == lEndGroup)
		{
			memmove((void*)(lpPartition + lStartOffset), (void*)(mppPartitionList[lStartGroup] + lStartOffset + 1), (mRowCount- iRow)* mDataSize);
		}
		else
		{
			long long count = (COLUMN_PARTITION_SIZE - lStartOffset) * mDataSize;
			memmove((void*)(lpPartition + lStartOffset), (void*)(mppPartitionList[lStartGroup] + lStartOffset + 1), count);
			memmove((void*)(lpPartition + COLUMN_PARTITION_SIZE - 1), (void*)mppPartitionList[lStartGroup+1], mDataSize);
            
			long long lBytesOfPartition = (COLUMN_PARTITION_SIZE - 1 )* mDataSize;
			for (long long p = lStartGroup + 1; p <= lEndGroup - 1; p++)
			{
                T* lpOldPartition = NULL;
                hr = hGetPartition(p, &lpOldPartition);
                
                T* lpNewPartition = NULL;
                MEM_GOVERNOR_RESERVE(0, COLUMN_PARTITION_SIZE * sizeof(T))
                lpNewPartition = new T[COLUMN_PARTITION_SIZE];
                MEM_GOVERNOR_COMMIT(0)
				
				memmove((void*)lpNewPartition, lpOldPartition+1, lBytesOfPartition);
				memmove((void*)(lpNewPartition + COLUMN_PARTITION_SIZE - 1), mppPartitionList[p+1], mDataSize);
                
                delete [] lpOldPartition;
                MEM_GOVERNOR_RELEASE(COLUMN_PARTITION_SIZE * sizeof(T),COLUMN_PARTITION_SIZE * sizeof(T));
                
                mppPartitionList[p] = lpNewPartition;
			}
            hr = hGetPartition(lEndGroup, &lpPartition);
            CHECK_HR;
            if (lEndOffset > 0)
            {
                T* lpNewPartition = NULL;
                MEM_GOVERNOR_RESERVE(0, (lEndOffset) * sizeof(T))
                lpNewPartition = new T[lEndOffset];
                MEM_GOVERNOR_COMMIT(0)
                
                memmove((void*)lpNewPartition, lpPartition+1, lEndOffset * mDataSize);
                mppPartitionList[lEndGroup] = lpNewPartition;
            }
            else
            {
                //fali, removing one element makes the partition number required one less than before , don't need to create a new partition and copy
                mppPartitionList[lEndGroup] = NULL;
            }
            delete [] lpPartition;
            MEM_GOVERNOR_RELEASE((lEndOffset+1) * sizeof(T),(lEndOffset+1) * sizeof(T));
        }
	}
    
    if (mpFlags)
    {
        MEM_GOVERNOR_RESERVE(mRowCount * sizeof(DSSDataFlag), mRowCount * sizeof(DSSDataFlag))
        DSSDataFlag* lpFlag = new DSSDataFlag[mRowCount];
        CHECK_NEWED_PTR(lpFlag);        
        memmove(lpFlag, mpFlags, iRow * sizeof(DSSDataFlag));
        memmove(lpFlag + iRow, mpFlags + iRow + 1, (lRowCount-iRow-1) * sizeof(DSSDataFlag));
        delete [] mpFlags;
        MEM_GOVERNOR_RELEASE(lRowCount * sizeof(DSSDataFlag), lRowCount * sizeof(DSSDataFlag))
        mpFlags = lpFlag;
    }
    
    return hr;
}

template <>
int DSSDataColumnSimple<MDataType::DSSBigDecimal>::Remove(int iRow)
{
    int hr = S_OK;
    //transaction don't support map file
    if(mFilePath!="")
        return S_FALSE;
    
    if (iRow < 0 || iRow >= mRowCount)
        return E_INVALIDARG;
    
    int lRowCount = mRowCount;
    mRowCount--;
    
    if (mpData && !mppPartitionList)
    {
        MEM_GOVERNOR_RESERVE(mRowCount * sizeof(MDataType::DSSBigDecimal), mRowCount * sizeof(MDataType::DSSBigDecimal))
        MDataType::DSSBigDecimal* lpData = new MDataType::DSSBigDecimal[mRowCount];
        CHECK_NEWED_PTR(lpData);
        for (int iRowIndex=0; iRowIndex<iRow; iRowIndex++)
        {
            lpData[iRowIndex] = mpData[iRowIndex];
        }
        
        for (int iRowIndex=iRow; iRowIndex<mRowCount; iRowIndex++)
        {
            lpData[iRowIndex] = mpData[iRowIndex+1];
        }

        delete[] mpData;
        MEM_GOVERNOR_RELEASE(lRowCount * sizeof(MDataType::DSSBigDecimal),lRowCount * sizeof(MDataType::DSSBigDecimal))
        mpData = lpData;
    }
    
    if (mppPartitionList)
	{
		//fali, 913842, handle partition cases
        long long lStartGroup = iRow >> COL_PRTTN_BIT;
		long long lStartOffset = iRow - (lStartGroup << COL_PRTTN_BIT);
		
		long long lEndGroup = mRowCount >> COL_PRTTN_BIT;
		long long lEndOffset = mRowCount - (lEndGroup << COL_PRTTN_BIT);
		
		MDataType::DSSBigDecimal* lpPartition = NULL;
		hr = hGetPartition(lStartGroup, &lpPartition);
		CHECK_HR;
		
		if (lStartGroup == lEndGroup)
		{
            for (int iRowIndex=lStartOffset; iRowIndex<lEndOffset; iRowIndex++)
            {
                lpPartition[iRowIndex] = mppPartitionList[lStartGroup][iRowIndex + 1];
            }
		}
		else
		{
            for (int iRowIndex=lStartOffset; iRowIndex<COLUMN_PARTITION_SIZE-1; iRowIndex++)
            {
                lpPartition[iRowIndex] = mppPartitionList[lStartGroup][iRowIndex + 1];
            }
            lpPartition[COLUMN_PARTITION_SIZE - 1] = mppPartitionList[lStartGroup+1][0];

			for (long long p = lStartGroup + 1; p <= lEndGroup - 1; p++)
			{
                MDataType::DSSBigDecimal* lpOldPartition = NULL;
                hr = hGetPartition(p, &lpOldPartition);
                
                MDataType::DSSBigDecimal* lpNewPartition = NULL;
                MEM_GOVERNOR_RESERVE(0, COLUMN_PARTITION_SIZE * sizeof(MDataType::DSSBigDecimal))
                lpNewPartition = new MDataType::DSSBigDecimal[COLUMN_PARTITION_SIZE];
                MEM_GOVERNOR_COMMIT(0)
				
                for (int iRowIndex=0; iRowIndex<COLUMN_PARTITION_SIZE-1; iRowIndex++)
                {
                    lpNewPartition[iRowIndex] = lpOldPartition[iRowIndex + 1];
                }
                lpNewPartition[COLUMN_PARTITION_SIZE - 1] = mppPartitionList[p+1][0];
                
                delete [] lpOldPartition;
                MEM_GOVERNOR_RELEASE(COLUMN_PARTITION_SIZE * sizeof(MDataType::DSSBigDecimal),COLUMN_PARTITION_SIZE * sizeof(MDataType::DSSBigDecimal));
                
                mppPartitionList[p] = lpNewPartition;
			}
            hr = hGetPartition(lEndGroup, &lpPartition);
            CHECK_HR;
            if (lEndOffset > 0)
            {
                MDataType::DSSBigDecimal* lpNewPartition = NULL;
                MEM_GOVERNOR_RESERVE(0, (lEndOffset) * sizeof(MDataType::DSSBigDecimal))
                lpNewPartition = new MDataType::DSSBigDecimal[lEndOffset];
                MEM_GOVERNOR_COMMIT(0)
                
                for (int iRowIndex=0; iRowIndex<lEndOffset; iRowIndex++)
                {
                    lpNewPartition[iRowIndex] = lpPartition[iRowIndex + 1];
                }
                mppPartitionList[lEndGroup] = lpNewPartition;
            }
            else
            {
                //fali, removing one element makes the partition number required one less than before , don't need to create a new partition and copy
                mppPartitionList[lEndGroup] = NULL;
            }
            delete [] lpPartition;
            MEM_GOVERNOR_RELEASE((lEndOffset+1) * sizeof(MDataType::DSSBigDecimal),(lEndOffset+1) * sizeof(MDataType::DSSBigDecimal));
        }
	}
    
    if (mpFlags)
    {
        MEM_GOVERNOR_RESERVE(mRowCount * sizeof(DSSDataFlag), mRowCount * sizeof(DSSDataFlag))
        DSSDataFlag* lpFlag = new DSSDataFlag[mRowCount];
        CHECK_NEWED_PTR(lpFlag);
        memmove(lpFlag, mpFlags, iRow * sizeof(DSSDataFlag));
        memmove(lpFlag + iRow, mpFlags + iRow + 1, (lRowCount-iRow-1) * sizeof(DSSDataFlag));
        delete [] mpFlags;
        MEM_GOVERNOR_RELEASE(lRowCount * sizeof(DSSDataFlag), lRowCount * sizeof(DSSDataFlag))
        mpFlags = lpFlag;
    }
    
    return hr;
}

template <class T>
int DSSDataColumnSimple<T>::Insert(int iRow)
{
    int hr = S_OK;
    //transaction don't support map file
    if(mFilePath!="")
        return S_FALSE;
    
    if (iRow < 0 || iRow >= mRowCount)
        return E_INVALIDARG;
    
    int lRowCount = mRowCount;
    mRowCount++;
    
    if (mpData && !mppPartitionList)
    {
        MEM_GOVERNOR_RESERVE(mRowCount * sizeof(T), mRowCount * sizeof(T))
        T* lpData = new T[mRowCount];
        CHECK_NEWED_PTR(lpData);
        memset(lpData, 0, mRowCount * sizeof(T));
        memmove(lpData, mpData, iRow * sizeof(T));
        memmove(lpData+iRow+1, mpData+iRow, (lRowCount-iRow) * sizeof(T));
        delete[] mpData;
        MEM_GOVERNOR_RELEASE(lRowCount * sizeof(T),lRowCount * sizeof(T))
        mpData = lpData;
    }
    
    if (mppPartitionList)
	{
		int lnPart = (lRowCount >> COL_PRTTN_BIT) + 1;
		int lnReleasedPartions = 0;
		for (int i = 0; i < lnPart; i++)
		{
			if (mppPartitionList[i])
			{
				delete[] mppPartitionList[i];
				if (i < lnPart - 1)
					lnReleasedPartions ++;		//don't count the last partition as its size may be different
			}
			mppPartitionList[i] = NULL;
		}
        MEM_GOVERNOR_RELEASE(lnReleasedPartions * COLUMN_PARTITION_SIZE * sizeof(T), lRowCount * sizeof(T))
        
		delete[] mppPartitionList;
		mppPartitionList = NULL;
        MEM_GOVERNOR_RELEASE(lnPart * sizeof(T*), lRowCount * sizeof(T))
        
        hr = CreatePartition();
		CHECK_HR;
		
		mpData = mppPartitionList[0];
	}
    
    if (mpFlags)
    {
        MEM_GOVERNOR_RESERVE(mRowCount * sizeof(DSSDataFlag), mRowCount * sizeof(DSSDataFlag))
        DSSDataFlag* lpFlag = new DSSDataFlag[mRowCount];
        CHECK_NEWED_PTR(lpFlag);
        memset(lpFlag, DssDataNull, mRowCount * sizeof(DSSDataFlag));
        memmove(lpFlag, mpFlags, iRow * sizeof(DSSDataFlag));
        memmove(lpFlag + iRow + 1, mpFlags + iRow, (lRowCount-iRow) * sizeof(DSSDataFlag));
        delete [] mpFlags;
        MEM_GOVERNOR_RELEASE(lRowCount * sizeof(DSSDataFlag), lRowCount * sizeof(DSSDataFlag))
        mpFlags = lpFlag;
    }
    
    return hr;
}




























