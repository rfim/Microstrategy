//
//  DSSDataRefColumn.cpp
//  ReportServiceCore
//
//  Created by Xiaoqi Zeng on 9/27/11.
//  Copyright 2011 MicroStrategy. All rights reserved.
//

#import "DSSDataRefColumn.h"
#import "DSSAEHelper.h"
#import "DSSBinaryDataReader.h"
#import "DSSModelPopulation.h"
#import "DSSStrongPtr.h"

#import "DSSDataPartialColumn.h"

#define CHECK_HR_BLOCK(type) {CHECK_HR; if (type != lBlockType) return E_DATABLOCK;}

DSSDataRefColumn::DSSDataRefColumn() : mfCompatible(false)
{
    
}

DSSDataRefColumn::~DSSDataRefColumn()
{
    for(int i = 0; i < mvColumnList.size(); i++)
    {
        if (mvColumnList[i])
        {
            delete mvColumnList[i];
            mvColumnList[i] = NULL;
        }
    }
}

bool DSSDataRefColumn::IsRefColumn()
{
    return true;
}

//int DSSDataRefColumn::Init(EnumDSSDataType iDataType, int iRowCount, bool ibUseMappedFile = false)
int DSSDataRefColumn::Init(EnumDSSDataType iDataType, long long iRowCount, bool ibUseMappedFile)
{
    mDataType = iDataType;
    mRowCount = iRowCount;
    
    return S_OK;
}

void DSSDataRefColumn::setRowSize(int iRowSize)
{
    mMaxRowSize = iRowSize;
}

int DSSDataRefColumn::LoadDataFromBinary(DSSBinaryDataReader *ipReadStream, bool iIsFirstTimeLoad)
{
    int hr = S_OK;
    
    if (iIsFirstTimeLoad)
    {
        hr = FirstLoadDataFromBinary(ipReadStream);
    }
    else 
        hr = PartialLoadDataFromBinary(ipReadStream);
    CHECK_HR;
    
    return S_OK;

}

int DSSDataRefColumn::SaveDataToSingleColumn(DSSDataColumn* ipColumn)
{
    if (!ipColumn)
        return E_INVALIDARG;
    
    int hr = S_OK;
    
    for (int iCol = 0; iCol < mvColumnList.size(); iCol++)
    {
        if (mvColumnList[iCol])
        {
            hr = mvColumnList[iCol]->SaveToSingleColumn(ipColumn, mvStartGlobalKey[iCol]);
            CHECK_HR;
        }
    }
    return S_OK;
}

int DSSDataRefColumn::PartialLoadDataFromBinary(DSSBinaryDataReader *ipReadStream)
{
    int hr;
    //    int lBlockType = 0;
    //	int hr = ipReadStream->BeginReadBlock(&lBlockType);
    //	CHECK_HR_BLOCK(EnumDataBlockRefColumn);
    
	//1. load type and row count
	int lType;
	hr = ipReadStream->ReadInt(&lType);
    CHECK_HR;
    
    int lRowSize;
    hr = ipReadStream->ReadInt(&lRowSize);
    CHECK_HR;
    
	int lRow;
	//load row count
	hr = ipReadStream->ReadInt(&lRow);
	CHECK_HR;
    
    setRowSize(lRowSize);    
    
	//load local column cout
	int lnPartialColumn;
	hr = ipReadStream->ReadInt(&lnPartialColumn);
	CHECK_HR;
    
    if (mvColumnList.size() != lnPartialColumn || mvStartGlobalKey.size() != lnPartialColumn)
    {
        AE_ASSERT(false);
    }
    
	//load each local column
	for (int i = 0; i < lnPartialColumn; i++)
	{
        hr = ipReadStream->ReadInt(&mvStartGlobalKey[i]);
        CHECK_HR;
        //load the flag of null column
        bool lfNotNull = true;
        hr = ipReadStream->ReadBool(&lfNotNull);
        CHECK_HR;
        
        if (!lfNotNull)
        {
            //push a null column to local columns
            //partial update, we can do nothing. just keep previously result
        }
        else
        {
            //1. load type and size
            int lColType;
            int lnColumnRow;
            
            hr = ipReadStream->ReadInt(&lColType);
            CHECK_HR;
            
            hr = ipReadStream->ReadInt(&lnColumnRow);
            CHECK_HR;
            
            if (!mvColumnList[i])
            {
                DSSDataPartialColumn* lpColumn;
                hr = DSSAEHelper::GenPartialCol((EnumDSSDataType)lColType, lnColumnRow, &lpColumn);
                MBase::DSSStrongPtr<DSSDataPartialColumn> lpColumnStrongPtr(lpColumn);
                CHECK_HR;
            
                hr = lpColumn->LoadDataFromBinary(ipReadStream, true);
                CHECK_HR;
            
                mvColumnList[i] = lpColumn;
            
                lpColumnStrongPtr.GiveUp();
            }
            else
                hr = mvColumnList[i]->LoadDataFromBinary(ipReadStream, false);
            CHECK_HR;
            
        }        
        
	}
    
    
    return hr;
}

int DSSDataRefColumn::FirstLoadDataFromBinary(DSSBinaryDataReader *ipReadStream)
{
    int hr;
//    int lBlockType = 0;
//	int hr = ipReadStream->BeginReadBlock(&lBlockType);
//	CHECK_HR_BLOCK(EnumDataBlockRefColumn);
    
	//1. load type and row count
	int lType;
	hr = ipReadStream->ReadInt(&lType);
    CHECK_HR;
    
    int lRowSize;
    hr = ipReadStream->ReadInt(&lRowSize);
    CHECK_HR;
    
	int lRow;
	//load row count
	hr = ipReadStream->ReadInt(&lRow);
	CHECK_HR;
    
	//2. use row count to init map    
	Init((EnumDSSDataType)lType, lRow);
    setRowSize(lRowSize);    
    
	//load local column cout
	int lnPartialColumn;
	hr = ipReadStream->ReadInt(&lnPartialColumn);
	CHECK_HR;
    
    mvColumnList.resize(lnPartialColumn);
    mvStartGlobalKey.resize(lnPartialColumn);
    
	//load each local column
	for (int i = 0; i < lnPartialColumn; i++)
	{
        hr = ipReadStream->ReadInt(&mvStartGlobalKey[i]);
        CHECK_HR;
        //load the flag of null column
        bool lfNotNull = true;
        hr = ipReadStream->ReadBool(&lfNotNull);
        CHECK_HR;
        
        if (!lfNotNull)
        {
            //push a null column to local columns
            mvColumnList[i] = NULL;
        }
        else
        {
            //1. load type and size
            int lColType;
            int lnColumnRow;
                        
            hr = ipReadStream->ReadInt(&lColType);
            CHECK_HR;
            
            hr = ipReadStream->ReadInt(&lnColumnRow);
            CHECK_HR;
            
            //2. create data column
            //GenCol : type, size,
            DSSDataPartialColumn* lpColumn;
            hr = DSSAEHelper::GenPartialCol((EnumDSSDataType)lColType, lnColumnRow, &lpColumn);
            CHECK_HR;
            MBase::DSSStrongPtr<DSSDataPartialColumn> lpColumnStrongPtr(lpColumn);
            
            hr = lpColumn->LoadDataFromBinary(ipReadStream, true);
            CHECK_HR;
                        
            mvColumnList[i] = lpColumn;
            
            lpColumnStrongPtr.GiveUp();
        }        
        
	}
    
//	hr = ipReadStream->EndReadBlock();
//	CHECK_HR;
    
    return hr;

}


long long DSSDataRefColumn::getRowCount()
{
    return mRowCount;
}

int DSSDataRefColumn::get_RowCount(int* opRowCount)
{
    if(opRowCount)
    {
        *opRowCount = mRowCount;
    }
    return S_OK;
}

int DSSDataRefColumn::getSizeOfData()
{
    return mMaxRowSize;
}


int DSSDataRefColumn::getRowSize(long long iRow, int &orSize)
{
    if (iRow < 0 || iRow > mRowCount)
        throw E_INVALIDARG;
    int lCol = hFindColIndex(iRow);
    if (lCol == -1)
        return mMaxRowSize;
    else
        return mvColumnList[lCol]->getRowSize(iRow - mvStartGlobalKey[lCol], orSize);
}


int DSSDataRefColumn::setData(long long iRow, char *ipData, int iSizeInByte, DSSDataFlag iFlag)
{
    if (iRow < 0 || iRow >= mRowCount)
        throw E_INVALIDARG;
    int lCol = hFindColIndex(iRow);
    if (lCol == -1)
        return E_INVALIDARG;
    else
        return mvColumnList[lCol]->setData(iRow - mvStartGlobalKey[lCol], ipData, iSizeInByte, iFlag);
}

int DSSDataRefColumn::hFindColIndex(long long iRow)
{
    if (iRow < 0 || iRow > mRowCount)
        return -1;
    
    if (iRow == mRowCount)
        return -1;
    
    //binary search
    int lStart = 0;
    int lEnd = mvStartGlobalKey.size() - 1;
    int lMid;
    
    while (lStart <= lEnd)
    {
        lMid = (lStart + lEnd) / 2;
        int lCurrentStart = mvStartGlobalKey[lMid];
        int lCurrentEnd = lMid == mvStartGlobalKey.size() - 1 ? mRowCount : mvStartGlobalKey[lMid + 1];
        
        if (iRow >= lCurrentStart && iRow < lCurrentEnd)
            return lMid;
        
        if (iRow < mvStartGlobalKey[lMid])
            lEnd = lMid - 1;
        else 
            lStart = lMid + 1;
    }
    
    return -1;
}

int DSSDataRefColumn::getData(long long iRow, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag)
{
	if (iRow > mRowCount || iRow < 0 || !oppData)
		return E_INVALIDARG;
    else if (iRow == mRowCount)//this index is for null value
    {
        orSize = 0;
        orFlag = DssDataNull;
        orType = mDataType;
        return S_OK;
    }
    
    int lLocalColumnId = hFindColIndex(iRow);
    
    if (lLocalColumnId >= 0 && mvColumnList[lLocalColumnId])        
    {
        return mvColumnList[lLocalColumnId]->getData(iRow - mvStartGlobalKey[lLocalColumnId], oppData, orSize, orType, orFlag);
    }
    // 2/29/2012 xiaoqi
	else
    {
        orSize = 0;
        orFlag = DssDataNull;
        orType = mDataType;
        return S_OK;
    }
	return E_FAIL;
}

int DSSDataRefColumn::syncDataTypeWithDataColumn()
{
    if (mRowCount == 0 || mvColumnList.size() == 0)
        return S_OK;
    
    EnumDSSDataType lDataColumnDataType = mvColumnList[0]->getDataType();
    mDataType = lDataColumnDataType;

    return S_OK;
}

int DSSDataRefColumn::getDataCopy(long long iRow, char *pBuffer, int iBufSize, int &orCopiedSize, EnumDSSDataType &orType, DSSDataFlag &orFlag)
{
	if (iRow >= mRowCount || iRow < 0 || !pBuffer)
		return E_INVALIDARG;
    else if (iRow == mRowCount)//this index is for null value
    {
        orCopiedSize = 0;
        orFlag = DssDataNull;
        orType = mDataType;
        return S_OK;
    }
    
    int lLocalColumnId = hFindColIndex(iRow);
    
    if (lLocalColumnId >= 0 && mvColumnList[lLocalColumnId])        
    {
        return mvColumnList[lLocalColumnId]->getDataCopy(iRow - mvStartGlobalKey[lLocalColumnId], pBuffer, iBufSize, orCopiedSize, orType, orFlag);
    }
    // 2/29/2012 xiaoqi
	else
    {
        orCopiedSize = 0;
        orFlag = DssDataNull;
        orType = mDataType;
        return S_OK;
    }
	return E_FAIL;
    
}


int DSSDataRefColumn::setDataInBulk(long long iStart, long long iEnd, char *ipData, int iSizeInByte, DSSDataFlag *ipFlags, int iSIBFlags)
{
    return E_NOTIMPL;
}


int DSSDataRefColumn::getNumericVal(long long iRow, double &orVal, DSSDataFlag &orFlag)
{
    if (iRow > mRowCount || iRow < 0)
		return E_INVALIDARG;
    else if (iRow == mRowCount)//this index is for null value
    {
        orVal = 0;
        orFlag = DssDataNull;
        return S_OK;
    }
    
    int lLocalColumnId = hFindColIndex(iRow);
    
    if (lLocalColumnId >= 0 && mvColumnList[lLocalColumnId])       
    {
        return mvColumnList[lLocalColumnId]->getNumericVal(iRow - mvStartGlobalKey[lLocalColumnId], orVal, orFlag);
    }
    // 2/29/2012 xiaoqi
	else
    {
        orVal = 0;
        orFlag = DssDataNull;
        return S_OK;
    }
	return E_FAIL;
}


int DSSDataRefColumn::setNumeric(long long iRow, double iVal, DSSDataFlag iFlag)
{
    return E_NOTIMPL;
}


int DSSDataRefColumn::getFlag(long long iRow, DSSDataFlag &orFlag)
{
    
    if (iRow > mRowCount || iRow < 0)
		return E_INVALIDARG;
    else if (iRow == mRowCount)//this index is for null value
    {
        orFlag = DssDataNull;
        return S_OK;
    }
    
    int lLocalColumnId = hFindColIndex(iRow);
    
    if (lLocalColumnId >= 0 && mvColumnList[lLocalColumnId])         
    {
        if (mvColumnList[lLocalColumnId])
            return mvColumnList[lLocalColumnId]->getFlag(iRow - mvStartGlobalKey[lLocalColumnId], orFlag);
        // 2/29/2012 xiaoqi
        else
        {
            orFlag = DssDataNull;
            return S_OK;
        }
    }

    //zhyang, 539211
    orFlag = DssDataMissing;
    return S_OK;
//	return E_FAIL;
}

int DSSDataRefColumn::setFlag(long long iRow, DSSDataFlag Flag)
{
    if (iRow >= mRowCount || iRow < 0)
		return E_INVALIDINDEX;
    

    int lLocalColumnId = hFindColIndex(iRow);       
    
    if (lLocalColumnId >= 0)
    {
        if (mvColumnList[lLocalColumnId])
        {
            int lLocalRow = iRow - mvStartGlobalKey[lLocalColumnId];             
            if (lLocalRow >= 0 && lLocalRow < mvColumnList[lLocalColumnId]->getRowCount())
            {
                return mvColumnList[lLocalColumnId]->setFlag(lLocalRow, Flag);
            }
            else return E_INVALIDINDEX;
        }
    }    
           
	return E_FAIL;
}


int DSSDataRefColumn::setFlags(long long iStart, long long iEnd, DSSDataFlag *ipFlags, int iSizeInByte)
{
    if (iStart > iEnd || iEnd >= mRowCount || iStart < 0 || !ipFlags)
		return E_INVALIDINDEX;
    
    for (int iRow = iStart; iRow < iEnd; iRow ++)
    {
        int hr = setFlag(iRow, ipFlags[iRow - iStart]);
        CHECK_HR;
    }
    
	return S_OK;
}


int DSSDataRefColumn::setFlags(long long iStart, long long iEnd, DSSDataFlag *ipFlags, int* iElementList, int iElementSize)
{
    if (iEnd - iStart != iElementSize || iStart > iEnd || !ipFlags)
		return E_INVALIDINDEX;
  	
	for(int iRow = 0; iRow < iElementSize;iRow++)
	{
		int hr = setFlag(iElementList[iRow], ipFlags[iStart + iRow]);
        CHECK_HR;
	}
    
	return S_OK;
}

int DSSDataRefColumn::Order(long long iRow1, long long iRow2)
{
    
	DSSDataFlag lFlag1, lFlag2;
    
    int hr = getFlag(iRow1, lFlag1);
    CHECK_HR;
    
	hr = getFlag(iRow2, lFlag2);
    CHECK_HR;
    
	if (lFlag1 == DssDataOk && lFlag2 == DssDataOk)
	{
        
        int lColIndex1 = hFindColIndex(iRow1);
        int lColIndex2 = hFindColIndex(iRow2);
        
        
        DSSDataPartialColumn* lpCol1 = lColIndex1 >= 0 && lColIndex1 < mvColumnList.size() ? mvColumnList[lColIndex1] : NULL;
        DSSDataPartialColumn* lpCol2 = lColIndex2 >= 0 && lColIndex2 < mvColumnList.size() ? mvColumnList[lColIndex2] : NULL;
        
        bool lfNull1 = lpCol1 == NULL;
        bool lfNull2 = lpCol2 == NULL;
        
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
        
        int lLocalRow1 = iRow1 - mvStartGlobalKey[lColIndex1];
        int lLocalRow2 = iRow2 - mvStartGlobalKey[lColIndex2];
        
        if (lpCol1 == lpCol2)
            return lpCol1->Order(lLocalRow1, lLocalRow2);
        else
        {
            //compare data
            unsigned char* lpData2;
			int lcbWritten2;
			EnumDSSDataType lDataType2;
			DSSDataFlag  lFlags2;
            
            //junwang, 2012/02/29, #554978, use local key
			hr = lpCol2->getData(lLocalRow2, (const char**)(&lpData2), lcbWritten2, lDataType2, lFlags2);
            CHECK_HR;
            
            return lpCol1->CompareToData(lLocalRow1, lpData2, lcbWritten2, lFlag2, true, false);
        }
        
    }
	else
	{
		int lRank1 = (lFlag1 == DssDataOk) ? 1 : 0;
		int lRank2 = (lFlag2 == DssDataOk) ? 1 : 0;
        
		return lRank1 - lRank2;
	}
}


int DSSDataRefColumn::Order(long long iRow1, long long iRow2,bool fIsNullDisplayOnTop)
{
        
	DSSDataFlag lFlag1, lFlag2;
    
    int hr = getFlag(iRow1, lFlag1);
    CHECK_HR;
    
	hr = getFlag(iRow2, lFlag2);
    CHECK_HR;
    
	if (lFlag1 == DssDataOk && lFlag2 == DssDataOk)
	{
        
        int lColIndex1 = hFindColIndex(iRow1);
        int lColIndex2 = hFindColIndex(iRow2);

        
        DSSDataPartialColumn* lpCol1 = lColIndex1 >= 0 && lColIndex1 < mvColumnList.size() ? mvColumnList[lColIndex1] : NULL;
        DSSDataPartialColumn* lpCol2 = lColIndex2 >= 0 && lColIndex2 < mvColumnList.size() ? mvColumnList[lColIndex2] : NULL;
       
        bool lfNull1 = lpCol1 == NULL;
        bool lfNull2 = lpCol2 == NULL;
        
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
        
        int lLocalRow1 = iRow1 - mvStartGlobalKey[lColIndex1];
        int lLocalRow2 = iRow2 - mvStartGlobalKey[lColIndex2];
        
        if (lpCol1 == lpCol2)
            return lpCol1->Order(lLocalRow1, lLocalRow2);
        else
        {
            //compare data
            unsigned char* lpData2;
			int lcbWritten2;
			EnumDSSDataType lDataType2;
			DSSDataFlag  lFlags2;
            
            //junwang, 2012/02/29, #554978, use local key
			hr = lpCol2->getData(lLocalRow2, (const char**)(&lpData2), lcbWritten2, lDataType2, lFlags2);
            CHECK_HR;
            
            return lpCol1->CompareToData(lLocalRow1, lpData2, lcbWritten2, lFlag2, true, false);
        }
        
    }
	else
	{
        return hCompareFlag(lFlag1, lFlag2, fIsNullDisplayOnTop);
	}
}

int DSSDataRefColumn::CompareToData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags, bool iIsNullDisplayOnTop, bool ibSimpleComparison)
{
    if (iRow >= mRowCount || iRow < 0)
		return E_INVALIDINDEX;
    
    int lLocalColumnId = hFindColIndex(iRow);       
    
    if (lLocalColumnId >= 0)
    {
        if (mvColumnList[lLocalColumnId])
        {
            int lLocalRow = iRow - mvStartGlobalKey[lLocalColumnId];             
            if (lLocalRow >= 0 && lLocalRow < mvColumnList[lLocalColumnId]->getRowCount())
            {
                return mvColumnList[lLocalColumnId]->CompareToData(lLocalRow, ipData, iSize, iFlags, iIsNullDisplayOnTop, ibSimpleComparison);
            }
            else return E_INVALIDINDEX;
        }
    }    
	return E_FAIL;    
	
}

int DSSDataRefColumn::CreatePartition()
{
    return E_NOTIMPL;
}

void DSSDataRefColumn::RecycleMemory()
{
}

int DSSDataRefColumn::hCompareRow(long long iRow1, long long iRow2, bool iIsNullDisplayOnTop, int iLocale, bool ibSimpleComparison )
{    
    //zhyang, just use it for test reason
    return Order(iRow1, iRow2);
}


int DSSDataRefColumn::hCompareData(long long iRow, unsigned char* ipData, int iSize,  DSSDataFlag iFlags, bool iIsNullDisplayOnTop, bool ibSimpleComparison)
{
    if (iRow >= mRowCount || iRow < 0)
		return E_INVALIDINDEX;
    
    int lLocalColumnId = hFindColIndex(iRow);   
    if (lLocalColumnId >= 0)
    {
        if (mvColumnList[lLocalColumnId])
        {
            int lLocalRow = iRow - mvStartGlobalKey[lLocalColumnId];             
            if (lLocalRow >= 0 && lLocalRow < mvColumnList[lLocalColumnId]->getRowCount())
            {
                return mvColumnList[lLocalColumnId]->CompareToData(lLocalRow, ipData, iSize, 0, true, false);
            }
            else return E_INVALIDINDEX;
        }
    }
    return S_OK;
}

int DSSDataRefColumn::reserveMemForStrData()
{
    return E_NOTIMPL;   //TODO: after merge to 930
}

int DSSDataRefColumn::createDataFlag(EnumDSSDataFlag iFlag)
{
    return E_NOTIMPL;   //TODO: after merge to 930
}

int DSSDataRefColumn::createMMapMemory(const char * ipFilePath, bool ibUseNewFile)
{
    return E_NOTIMPL;   //TODO: after merge to 930
}

int DSSDataRefColumn::MapFileBuffer()
{
    return E_NOTIMPL;   //TODO: after merge to 930
}

int DSSDataRefColumn::UnMapFileBuffer()
{
    return E_NOTIMPL;   //TODO: after merge to 930
}

int DSSDataRefColumn::ExtendColumn(int iRows)
{
    int hr = S_OK;
    //since hFindIndex uses binary search we can assume mvStartGlobalKey is sorted ascendingly
    int lCol = mvColumnList.size() - 1;
    if(lCol >= 0)
    {
        hr = mvColumnList[lCol]->ExtendColumn(iRows);
        CHECK_HR;
        
        mRowCount += iRows;
    }
    return hr;
}

int DSSDataRefColumn::Remove(int iRow)
{
    return E_NOTIMPL;
}

int DSSDataRefColumn::Insert(int iRow)
{
    return E_NOTIMPL;
}
