//
//  DSSDataPartialColumn.cpp
//  ReportServiceCore
//
//  Created by Tang, Cheng on 5/18/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#include "DSSDataPartialColumn.h"
#include "DSSDataHolder.h"
#include "DSSAEHelper.h"
#include "DSSBinaryDataReader.h"

DSSDataPartialColumn::DSSDataPartialColumn()
{
}

DSSDataPartialColumn::~DSSDataPartialColumn()
{
    for (int i = 0; i < mvDataList.size(); i++)
        if (mvDataList[i])
            delete mvDataList[i];
    
    mvDataList.clear();
    mvGlobalKeyToListOffset.clear();
    mvStartKey.clear();
    
}


int DSSDataPartialColumn::LoadDataFromBinary(DSSBinaryDataReader *ipReadStream, bool iIsFirstTimeLoad)
{
    
    return iIsFirstTimeLoad ? FirstLoadDataFromBinary(ipReadStream) : PartialLoadDataFromBinary(ipReadStream);
}

int DSSDataPartialColumn::PartialLoadDataFromBinary(DSSBinaryDataReader* ipReadStream)
{
    if (!ipReadStream)
        return E_INVALIDARG;
    
    int lnList;
    int hr = ipReadStream->ReadInt(&lnList);
    CHECK_HR;
    
    //var lfString:Boolean = DFCXTabColHelper.IsStringType(_dataType);
    
    for (int iList = 0; iList < lnList; iList++)
    {
        //load the data list first
        int lStartKey;
        hr = ipReadStream->ReadInt(&lStartKey);
        CHECK_HR;
        
        int lnRow;
        hr = ipReadStream->ReadInt(&lnRow);
        CHECK_HR;
        
        DSSDataHolder* lpDataHolder;
        hr = DSSAEHelper::GenXTabDataHolder(mDataType, lnRow, &lpDataHolder);
        CHECK_HR;
        MBase::StrongPtr<DSSDataHolder> lpDataHolderStrong(lpDataHolder);
        
        hr = hLoadData(ipReadStream, lpDataHolder, mDataType, lnRow);
        CHECK_HR;
        
        //split the data list if neccessery
        int lPrev = -1;
        int j = 0;
        for (; j < lnRow && j + lStartKey < mRowCount; j++)
        {
            if (mvGlobalKeyToListOffset[j + lStartKey] == -1)
            {
                if (lPrev == -1)
                {
                    DSSDataHolder* lpDataList;
                    hr = DSSAEHelper::GenXTabDataHolder(mDataType, /*temp value*/0, &lpDataList);
                    CHECK_HR_PTR(lpDataList);
                    mvDataList.push_back(lpDataList);
                    mvStartKey.push_back(j + lStartKey);
                    lPrev = j;
                }
                mvGlobalKeyToListOffset[j + lStartKey] = mvDataList.size() - 1;
            }
            else//split
            {
                if (lPrev >= 0)
                {
                    DSSDataHolder* lpNew = mvDataList.size() > 0 ? mvDataList[mvDataList.size() - 1] : NULL;
                    if (lpNew)
                    {
                        hr = lpNew->Init(mDataType, j - lPrev);
                        CHECK_HR;
                        int count = 0;
                        for (int iRow = lPrev; iRow < j; iRow++)
                        {
                            char* lpData;
                            int lcbWritten2;

                            hr = lpDataHolder->getData(iRow, (const char**)(&lpData), lcbWritten2);
                            CHECK_HR;
                            
                            hr = lpNew->setData(count++, lpData, lcbWritten2);
                            CHECK_HR;
                        }
                    }
                }
                lPrev = -1;
            }
        }
        
        if (lPrev >= 0)//last List
        {
            DSSDataHolder* lpNew = mvDataList.size() > 0 ? mvDataList[mvDataList.size() - 1] : NULL;
            if (lpNew)
            {
                hr = lpNew->Init(mDataType, j - lPrev);
                CHECK_HR;
                int count = 0;
                for (int iRow = lPrev; iRow < j; iRow++)
                {
                    char* lpData;
                    int lcbWritten2;
                    
                    hr = lpDataHolder->getData(iRow, (const char**)(&lpData), lcbWritten2);
                    CHECK_HR;
                    
                    hr = lpNew->setData(count++, lpData, lcbWritten2);
                    CHECK_HR;
                }
            }
        }
        
        lpDataHolderStrong.GiveUp();
        
        //flag
        vector<char> lvFlags;
        hr = ipReadStream->ReadByteArray(lvFlags);
        CHECK_HR;
        
        if (!lvFlags.empty())
        {
            memmove(mpFlags + lStartKey, (DSSDataFlag*)(&lvFlags[0]), lvFlags.size());
            
            if (mbAllDataValid)
            {
                for(int i = 0; i < lvFlags.size(); i++)
                {
                    if(lvFlags[i] != DssDataOk)
                    {
                        mbAllDataValid = false;
                        break;
                    }
                }
            }
        }
        else
        {
            memset(mpFlags + lStartKey, 0, lnRow);
        }

    }
    return S_OK;
}

bool DSSDataPartialColumn::IsRefColumn()
{
    return false;
}

int DSSDataPartialColumn::FirstLoadDataFromBinary(DSSBinaryDataReader* ipReadStream)
{
    int hr = S_OK;
    
    int lnList;
    hr = ipReadStream->ReadInt(&lnList);
    CHECK_HR;
    
    mvDataList.resize(lnList,0);
    
    mvStartKey.resize(lnList);
    
    for (int iList = 0; iList < lnList; iList++)
    {
        hr = ipReadStream->ReadInt(&mvStartKey[iList]);
        CHECK_HR;
        
        //todo load data
        int lnRow;
        hr = ipReadStream->ReadInt(&lnRow);
        CHECK_HR;
        
        DSSDataHolder* lpDataHolder;
        hr = DSSAEHelper::GenXTabDataHolder(mDataType, lnRow, &lpDataHolder);
        CHECK_HR_PTR(lpDataHolder);
        
       
        
        hr = hLoadData(ipReadStream, lpDataHolder, mDataType, lnRow);
        CHECK_HR;
        
        mvDataList[iList] = lpDataHolder;
        
        //flag
        vector<char> lvFlags;
        hr = ipReadStream->ReadByteArray(lvFlags);
        CHECK_HR;
        
        if (!lvFlags.empty())
        {
            memmove(mpFlags + mvStartKey[iList], (DSSDataFlag*)(&lvFlags[0]), lvFlags.size());
            
            if (mbAllDataValid)
            {
                for(int i = 0; i < lvFlags.size(); i++)
                {
                    if(lvFlags[i] != DssDataOk)
                    {
                        mbAllDataValid = false;
                        break;
                    }
                }
            }
        }
        else
        {
            memset(mpFlags + mvStartKey[iList], 0, lnRow);
        }

        
        //set mvMap
        for (int j = 0; j < lnRow; j++)
            if (mvStartKey[iList] + j < mRowCount)
                mvGlobalKeyToListOffset[mvStartKey[iList] + j] = iList;
        
    }
    
    return S_OK;
}

int DSSDataPartialColumn::SaveToSingleColumn(DSSDataColumn* ipColumn, int iOffset)
{
    if (!ipColumn)
        return E_INVALIDARG;
    int hr = S_OK;
    
    for (int i = 0; i < mvDataList.size(); i++)
    {
        if (mvDataList[i])
        {
            unsigned char* lpData2;
            int lcbWritten2;
            
            int lnRow = mvDataList[i]->count();
            for (int iRow = 0; iRow < lnRow; iRow++)
            {
                hr = mvDataList[i]->getData(iRow, (const char**)(&lpData2), lcbWritten2);
                CHECK_HR;
                
                hr = ipColumn->setData(iOffset + mvStartKey[i] + iRow, (char*)lpData2, lcbWritten2, mpFlags[mvStartKey[i] + iRow]);
                CHECK_HR;
            }
        }
    }
    return S_OK;
}

int DSSDataPartialColumn::Init(EnumDSSDataType iDataType, long long iRowCount, bool ibUseMappedFile)
{
    mDataType = iDataType;
    mRowCount = iRowCount;
    mvGlobalKeyToListOffset.resize(iRowCount);
    
    memset(&mvGlobalKeyToListOffset[0], 0xff, mRowCount*sizeof(int));

    mpFlags = new DSSDataFlag [iRowCount];
	CHECK_NEWED_PTR(mpFlags);
	long long i = 0;
	for(; i < iRowCount; i++)
        mpFlags[i] = DssDataMissing;
    
	mbAllDataValid = false;
	
	return S_OK;
}


long long DSSDataPartialColumn::getRowCount()
{
    return mRowCount;
}

int DSSDataPartialColumn::get_RowCount(int* opRowCount)
{
    if(opRowCount)
    {
        *opRowCount = mRowCount;
    }
    return S_OK;
}

int DSSDataPartialColumn::Order(long long iRow1, long long iRow2)
{
    DSSDataFlag lFlag1, lFlag2;
	int hr = getFlag(iRow1, lFlag1);
    CHECK_HR;
    
	hr = getFlag(iRow2, lFlag2);
    CHECK_HR;
    
	if (lFlag1 == DssDataOk && lFlag2 == DssDataOk)
	{
		bool lfNull1 = false;
		bool lfNull2 = false;
        int lLocalIndex1 = -1, lLocalIndex2 = -1;
        int lLocalRow1, lLocalRow2;
		DSSDataHolder *lpList1 = NULL, *lpList2 = NULL;
        
        if (iRow1 < 0 || iRow1 >= mRowCount) lfNull1 = true;
        if (iRow2 < 0 || iRow2 >= mRowCount) lfNull2 = true;
        
        if (!lfNull1) lLocalIndex1 = mvGlobalKeyToListOffset[iRow1];
        if (!lfNull2) lLocalIndex2 = mvGlobalKeyToListOffset[iRow2];
        
        if (lLocalIndex1 < 0) lfNull1 = true;
        if (lLocalIndex2 < 0) lfNull2 = true;
        
        if (!lfNull1) lpList1 = mvDataList[lLocalIndex1];
        if (!lfNull2) lpList2 = mvDataList[lLocalIndex2];
        
        if (!lpList1) lfNull1 = true;
        if (!lpList2) lfNull2 = true;
        
		if (lfNull1 || lfNull2)
		{
			if (lfNull1)
			{
				if (lfNull2)
				{
					return 0;
				}
				else
				{
					return -1;
				}
			}
            else return 1;         
		}
        
		AE_ASSERT(lpList1 && lpList2);
		lLocalRow1 = iRow1 - mvStartKey[lLocalIndex1];
        lLocalRow2 = iRow2 - mvStartKey[lLocalIndex2];
        
        unsigned char* lpData2;
        int lcbWritten2;
        
        //junwang, 2012/02/29, #554978, use local key
        hr = lpList2->getData(lLocalRow2, (const char**)(&lpData2), lcbWritten2);
        CHECK_HR;
        
        hr = getRowSize(iRow2, lcbWritten2);
        CHECK_HR;

        return hCompareData(iRow1, lpData2, lcbWritten2, DssDataOk, false);
	}
	else
	{
		int lRank1 = (lFlag1 == DssDataOk) ? 1 : 0;
		int lRank2 = (lFlag2 == DssDataOk) ? 1 : 0;
        
		return lRank1 - lRank2;
	}
}

int DSSDataPartialColumn::Order(long long iRow1, long long iRow2,bool fIsDisplayNullOnTop)
{
    DSSDataFlag lFlag1, lFlag2;
	int hr = getFlag(iRow1, lFlag1);
    CHECK_HR;
    
	hr = getFlag(iRow2, lFlag2);
    CHECK_HR;
    
	if (lFlag1 == DssDataOk && lFlag2 == DssDataOk)
	{
		bool lfNull1 = false;
		bool lfNull2 = false;
        int lLocalIndex1 = -1, lLocalIndex2 = -1;
        int lLocalRow1, lLocalRow2;
		DSSDataHolder *lpList1 = NULL, *lpList2 = NULL;
        
        if (iRow1 < 0 || iRow1 >= mRowCount) lfNull1 = true;
        if (iRow2 < 0 || iRow2 >= mRowCount) lfNull2 = true;
        
        if (!lfNull1) lLocalIndex1 = mvGlobalKeyToListOffset[iRow1];
        if (!lfNull2) lLocalIndex2 = mvGlobalKeyToListOffset[iRow2];
        
        if (lLocalIndex1 < 0) lfNull1 = true;
        if (lLocalIndex2 < 0) lfNull2 = true;
        
        if (!lfNull1) lpList1 = mvDataList[lLocalIndex1];
        if (!lfNull2) lpList2 = mvDataList[lLocalIndex2];
        
        if (!lpList1) lfNull1 = true;
        if (!lpList2) lfNull2 = true;
        
		if (lfNull1 || lfNull2)
		{
			if (lfNull1)
			{
				if (lfNull2)
				{
					return 0;
				}
				else
				{
					return -1;
				}
			}
            else return 1;         
		}
        
		AE_ASSERT(lpList1 && lpList2);
		lLocalRow1 = iRow1 - mvStartKey[lLocalIndex1];
        lLocalRow2 = iRow2 - mvStartKey[lLocalIndex2];
        
        unsigned char* lpData2;
        int lcbWritten2;
        
        //junwang, 2012/02/29, #554978, use local key
        hr = lpList2->getData(lLocalRow2, (const char**)(&lpData2), lcbWritten2);
        CHECK_HR;
        
        hr = getRowSize(iRow2, lcbWritten2);
        CHECK_HR;
        
        return hCompareData(iRow1, lpData2, lcbWritten2, DssDataOk, false);
	}
	else
	{
		return hCompareFlag(lFlag1, lFlag2, fIsDisplayNullOnTop);
	}
}

 /* 
 int getSizeOfData();
 
 int getRowSize(int iRow, int &orSize);
  
 
int DSSDataPartialColumn::setData(int iRow, char *ipData, int iSizeInByte, DSSDataFlag iFlag)
{
    if (iRow >= mRowCount)
        return E_INVALIDARG;
    
    //todo
    return -1;
}

int DSSDataPartialColumn::getData(int iRow, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag)
{
    
}


 
 int getDataCopy(int iRow, char *pBuffer, int iBufSize, int &orCopiedSize, EnumDSSDataType &orType, DSSDataFlag &orFlag);
 */
 
int DSSDataPartialColumn::setDataInBulk(long long iStart, long long iEnd, char *ipData, int iSizeInByte, DSSDataFlag *ipFlags, int iSIBFlags)
{
    return E_NOTIMPL;
}

int DSSDataPartialColumn::getNumericVal(long long iRow, double &orVal, DSSDataFlag &orFlag)
{
    if (iRow >= mRowCount)
        return E_INVALIDARG;
    
    //todo:
    int lOffset = mvGlobalKeyToListOffset[iRow];
    if (lOffset == -1)//missing
    {
        orVal = 0;
        orFlag = DssDataMissing;
    }
    else if (lOffset >= mvDataList.size())
        return E_FAIL;
    else
    {
        DSSDataHolder* lpHolder = mvDataList[lOffset];
        CHECK_PTR(lpHolder);
        orFlag = mpFlags[iRow];
        int hr = lpHolder->getNumericVal(iRow - mvStartKey[lOffset], orVal, orFlag);
        CHECK_HR;
        orFlag = mpFlags[iRow];
    }
    return S_OK;
}

int DSSDataPartialColumn::setNumeric(long long iRow, double iVal, DSSDataFlag iFlag)
{
    if(iRow >= mRowCount)
		return E_INVALIDINDEX;
	
	if(iFlag != DssDataOk)
	{
		iVal = 0;
		mbAllDataValid = false;
	}
	
	int lOffset = mvGlobalKeyToListOffset[iRow];
    if (lOffset < 0 || lOffset >= mvDataList.size())//missing
        return E_FAIL;
    else
    {
        DSSDataHolder* lpHolder = mvDataList[lOffset];
        CHECK_PTR(lpHolder);
        int hr = lpHolder->setNumeric(iRow - mvStartKey[lOffset], iVal);
        CHECK_HR;
    }

	mpFlags[iRow] = iFlag;
	
	return S_OK;

}

int DSSDataPartialColumn::getFlag(long long iRow, DSSDataFlag &orFlag)
{
    if(iRow >= mRowCount)
		return E_INVALIDINDEX;
	orFlag = mpFlags[iRow];
	return S_OK;
}
 
int DSSDataPartialColumn::setFlag(long long iRow, DSSDataFlag Flag)
{
    if(iRow >= mRowCount)
		return E_INVALIDINDEX;
	
	mpFlags[iRow] = Flag;
	
	if(Flag != DssDataOk && mbAllDataValid)
		mbAllDataValid = false;
	
	return S_OK;

}

int DSSDataPartialColumn::setFlags(long long iStart, long long iEnd, DSSDataFlag *ipFlags, int iSizeInByte)
{
    if (mRowCount * sizeof(DSSDataFlag) != iSizeInByte || !ipFlags)
		return E_INVALIDARG;
	
	memmove(mpFlags, ipFlags, iSizeInByte);
    
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

int DSSDataPartialColumn::CompareToData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags, bool iIsNullDisplayOnTop, bool ibSimpleComparison)
{
    return hCompareData(iRow, ipData, iSize, iFlags, iIsNullDisplayOnTop);
}


int DSSDataPartialColumn::setFlags(long long iStart, long long iEnd, DSSDataFlag *ipFlags, int* iElementList, int iElementSize)
{
    return E_NOTIMPL;
}

int DSSDataPartialColumn::size()
{
    return E_NOTIMPL;
}
 
int DSSDataPartialColumn::CreatePartition()
{
    return E_NOTIMPL;
}
 
int DSSDataPartialColumn::getRawDataAndFlags(void** opData, DSSDataFlag** opDataFlags)
{
    return E_NOTIMPL;
}
 
void DSSDataPartialColumn::RecycleMemory()
{
    
}
 
int DSSDataPartialColumn::hCompareRow(long long iRow1, long long iRow2, bool iIsNullDisplayOnTop,int iLocale, bool ibSimpleComparison)
{
    return E_NOTIMPL;
}
int DSSDataPartialColumn::hCompareData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags, bool iIsNullDisplayOnTop, bool ibSimpleComparison)
{
    return E_NOTIMPL;
}

int DSSDataPartialColumn::reserveMemForStrData()
{
    return E_NOTIMPL;   //TODO: after merge to 930
}

int DSSDataPartialColumn::createDataFlag(EnumDSSDataFlag iFlag)
{
    return E_NOTIMPL;   //TODO: after merge to 930
}

int DSSDataPartialColumn::createMMapMemory(const char * ipFilePath, bool ibUseNewFile)
{
    return E_NOTIMPL;   //TODO: after merge to 930
}

int DSSDataPartialColumn::MapFileBuffer()
{
    return E_NOTIMPL;   //TODO: after merge to 930
}

int DSSDataPartialColumn::UnMapFileBuffer()
{
    return E_NOTIMPL;   //TODO: after merge to 930
}

int DSSDataPartialColumn::ExtendColumn(int iRows)
{
    int hr = S_OK;
    int lOffset = -1;
    int lMaxStartKey = -1;
    for(int i = 0; i < mvStartKey.size(); i++)
    {
        if(mvStartKey[i] > lMaxStartKey)
        {
            lMaxStartKey = mvStartKey[i];
            lOffset = i;
        }
    }
    if(lOffset >= 0)
    {
        mvDataList[lOffset]->ExtendColumn(iRows);
        for(int i = 0; i < iRows; i++)
            mvGlobalKeyToListOffset.push_back(lOffset);
    }
    if(mpFlags)
    {
        DSSDataFlag* lpFlags = new DSSDataFlag [mRowCount + iRows];
        CHECK_NEWED_PTR(lpFlags);
        
        memmove(lpFlags, mpFlags, mRowCount * sizeof(DSSDataFlag));
        delete mpFlags;
        mpFlags = lpFlags;
    }
    
    mRowCount += iRows;
    
    return hr;
}

int DSSDataPartialColumn::Remove(int iRow)
{
    return E_NOTIMPL;
}

int DSSDataPartialColumn::Insert(int iRow)
{
    return E_NOTIMPL;
}