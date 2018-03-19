//
//  DSSDataPartialColumnSimple.cpp
//  ReportServiceCore
//
//  Created by Tang, Cheng on 5/18/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#include "DSSDataPartialColumnSimple.h"
#include "DSSViewDataSetHelper.h"

static const double gSecondsSince1970 = MDataType::DSSDateTime(1970, 1, 1).ToDATE();

DSSDataPartialColumnSimple::DSSDataPartialColumnSimple() : mDataSize(-1)
{}

DSSDataPartialColumnSimple::~DSSDataPartialColumnSimple()
{
}

int DSSDataPartialColumnSimple::hCompareData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags, bool iIsNullDisplayOnTop,bool ibSimpleComparison)
{
    if (iRow >= mRowCount || iSize > mDataSize)
		return E_INVALIDARG;
    
    int hr = S_OK;
    
    
    DSSDataHolder* lpHolder = NULL;
	int lOffset = mvGlobalKeyToListOffset[iRow];
    if (lOffset == -1 || lOffset >= mvDataList.size())
    {
        return E_FAIL;//something must be wrong
    }
    else
    {
        lpHolder = mvDataList[lOffset];
    }
	
    if(hr != S_OK || !lpHolder)
		return 0;
    
    return lpHolder->hCompareData(iRow - mvStartKey[lOffset], mDataType, ipData, iSize);
}

int DSSDataPartialColumnSimple::getSizeOfData()
{
    return mDataSize;
}

int DSSDataPartialColumnSimple::getRowSize(long long iRow, int &orSize)
{
    orSize = mDataSize;
    return S_OK;
}

int DSSDataPartialColumnSimple::setData(long long iRow, char *ipData, int iSize, DSSDataFlag iFlag)
{
    if (iRow >= mRowCount || iSize > mDataSize)
		return E_INVALIDARG;
	
    DSSDataHolder* lpHolder = NULL;
	int lOffset = mvGlobalKeyToListOffset[iRow];
    if (lOffset == -1 || lOffset >= mvDataList.size())
    {
        return E_FAIL;//something must be wrong
    }
    else
    {
        lpHolder = mvDataList[lOffset];
    }
	
    CHECK_PTR(lpHolder);
    
	mpFlags[iRow] = iFlag;
    
    int hr = S_OK;
	
	if (iFlag == DssDataOk)
	{
        hr = lpHolder->setData(iRow - mvStartKey[lOffset], ipData, iSize);
    }
	else if (mDataType != DssDataTypeDate && mDataType != DssDataTypeTimeStamp && mDataType != DssDataTypeTime && mDataType != DssDataTypeCellFormatData && mDataType !=DssDataTypeBigDecimal) 
    {
        hr = lpHolder->setDataToNULL(iRow - mvStartKey[lOffset], iSize);
    }
    CHECK_HR;
	
	if(iFlag != DssDataOk)
		this->mbAllDataValid = false;
	
	return S_OK;
}

int DSSDataPartialColumnSimple::getData(long long iRow, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag)
{
    if (iRow >= mRowCount || iRow < 0 || !oppData)
		return E_INVALIDARG;
	
    int lOffset = mvGlobalKeyToListOffset[iRow];
    if (lOffset == -1 || lOffset >= mvDataList.size())
    {
        //data missing
        orFlag = mpFlags[iRow];
        _ASSERT(orFlag != S_OK);
        return S_OK;
    }
    else
    {
        DSSDataHolder* lpHolder = mvDataList[lOffset];
        CHECK_PTR(lpHolder);
        int hr = lpHolder->getData(iRow - mvStartKey[lOffset], oppData, orSize);
        CHECK_HR;
        orSize = mDataSize;
        orType = mDataType;
        orFlag = mpFlags[iRow];
        return S_OK;
    }
}

int DSSDataPartialColumnSimple::getDataCopy(long long iRow, char *pBuffer, int iBufSize, int &orCopiedSize, EnumDSSDataType &orType, DSSDataFlag &orFlag)
{
    if (iRow >= mRowCount || (!pBuffer) || iBufSize < mDataSize)
		return E_INVALIDARG;
	
    int lOffset = mvGlobalKeyToListOffset[iRow];
    if (lOffset == -1 || lOffset >= mvDataList.size())
    {
        //data missing
        orFlag = mpFlags[iRow];
        _ASSERT(orFlag != S_OK);
        return S_OK;
    }
    else
    {
        DSSDataHolder* lpHolder = mvDataList[lOffset];
        CHECK_PTR(lpHolder);
        int hr = lpHolder->getDataCopy(iRow - mvStartKey[lOffset], pBuffer, iBufSize, orCopiedSize);
        CHECK_HR;
        orCopiedSize = mDataSize;
        orType = mDataType;
        orFlag = mpFlags[iRow];
        return S_OK;
    }
	
	return S_OK;
}



int DSSDataPartialColumnSimple::hLoadStringData(DSSBinaryDataReader *ipReadStream, DSSDataHolder* ipHolder, EnumDSSDataType iDataType, int nElement)
{
    int hr = S_OK;
	bool lbIsString = (DssDataTypeChar == iDataType || DssDataTypeVarChar == iDataType
                       || DssDataTypeMBChar == iDataType || DssDataTypeUTF8Char == iDataType);
	
	bool lbIsDateTime = !lbIsString && (DssDataTypeDate == iDataType || DssDataTypeTime == iDataType 
                                        || DssDataTypeTimeStamp == iDataType);
	
	vector<char> lvUTF8Data(32);		//initialize to 32 bytes
    
    vector<vector<char> > lvvCellFormat;
    
	int lBufferSize = 8;
    
	vector<char> lvNumericalData(lBufferSize);
	
    int i = 0;
	for (i = 0; i < nElement; i++)
	{
		unsigned int lStrLen = 0;
		hr = ipReadStream->ReadStringLen(&lStrLen);
		CHECK_HR;
		
		if (lStrLen > lvUTF8Data.size())
			lvUTF8Data.resize(lStrLen * 2);
        
        hr = ipReadStream->ReadUTF8String(&lvUTF8Data[0], lStrLen);
        CHECK_HR;
		
		if (lbIsString)
		{
			hr = ipHolder->setData(i, &lvUTF8Data[0], lStrLen * sizeof(char));
			CHECK_HR;
		}
		else if(iDataType==DssDataTypeBigDecimal)
		{
			MDataType::DSSBigDecimal lBigDecimal(&(lvUTF8Data[0])); 
			hr=ipHolder->setData(i, reinterpret_cast<char *> (&lBigDecimal), sizeof(MDataType::DSSBigDecimal));
			CHECK_HR;
		}
		else if (DssDataTypeBinary == iDataType)
		{
			//lwang, 07/07/2010, 413152, convert binary data to hex string
			vector<char> lHexString (lStrLen * 2 + 1);
			for (int j = 0; j < lStrLen; j++)
			{
				unsigned char lVal = lvUTF8Data[j];
				
				unsigned char loffset = (unsigned char)((lVal & 0xF0U) >> 4);
				lHexString[2 * j] = loffset <= 9 ? '0' + loffset : 'A' + loffset - 10;
				
				loffset = ((lVal & 0x0F));
				lHexString[2 * j + 1] = loffset <= 9 ? '0' + loffset : 'A' + loffset - 10;
			}
			lHexString[2 * lStrLen] = '\0';
			
			hr = ipHolder->setData(i, &lHexString[0], (lStrLen * 2 + 1) * sizeof(char));
			CHECK_HR;
		}
		else if (!lbIsDateTime)
		{
			int lDataSize = 0;
			hr = DSSAEHelper::ConvertStringToData(&lvUTF8Data[0], iDataType, &lvNumericalData[0], lBufferSize, lDataSize);
			CHECK_HR;
			
			
			hr = ipHolder->setData(i, &lvNumericalData[0], lDataSize);
			CHECK_HR;
		}
		else 
		{
            double lVal = atof(&lvUTF8Data[0]);
            MDataType::DSSDateTime lTmp(lVal/(double)MDataType::DAY_IN_MILLISECONDS + gSecondsSince1970);
            hr = ipHolder->setData(i, (char*)&lTmp, sizeof(MDataType::DSSDateTime));
            CHECK_HR;
		}
        
        int lRowSize;
        hr = ipHolder->getRowSize(i, lRowSize);
        CHECK_HR;
        
        if (mDataSize < lRowSize)
            mDataSize = lRowSize;
    }
    
    return S_OK;
    
}

int DSSDataPartialColumnSimple::hLoadData(DSSBinaryDataReader *ipReadStream, DSSDataHolder* ipHolder, EnumDSSDataType iDataType, int nElements)
{
    if (DssDataTypeVarChar != iDataType && DssDataTypeBigDecimal != iDataType && 
        DssDataTypeMBChar != iDataType && DssDataTypeBinary != iDataType )//&& DssDataTypeCellFormatData != lDataType)
        return hLoadRawData(ipReadStream, ipHolder, iDataType, nElements);
    else
        return hLoadStringData(ipReadStream, ipHolder, iDataType, nElements);
    
}
int DSSDataPartialColumnSimple::hLoadRawData(DSSBinaryDataReader *ipReadStream, DSSDataHolder* ipHolder, EnumDSSDataType iDataType, int nElements)
{
    int hr = S_OK;
	int lSizeOfDataType = DSSViewDataSetHelper::hSizeOf(iDataType);
    
    if (lSizeOfDataType <= 0)
        lSizeOfDataType = 8;
    
    hr = ipHolder->getRowSize(0, mDataSize);
	CHECK_HR;
    
    int nSize = lSizeOfDataType * nElements;
	if (nSize <= 0)
		return 0;
	vector<char> lvRawDataBuffer(nSize);
	hr = ipReadStream->Read((unsigned char*)(&lvRawDataBuffer[0]), nSize, lSizeOfDataType);
	CHECK_HR;
	
	if (iDataType == DssDataTypeLong)
	{
		char *lpData = &lvRawDataBuffer[0];
		for (int i = 0; i < nElements; i++)
		{
			//411868, long long is 8 bytes in Mac OS
			long long lVal = *((int*)(&lvRawDataBuffer[0]));
			hr = ipHolder->setData(i, (char*)&lVal, sizeof(long long));
			CHECK_HR;
			lpData += 8;
		}
		return S_OK;
	}
    if (DssDataTypeCellFormatData == iDataType)
    {
        vector<int> lvType;
        vector<int> lvFmtIDs;
        
        hr = ipReadStream->ReadIntArray(lvType);
        CHECK_HR;
        
        hr = ipReadStream->ReadIntArray(lvFmtIDs);
        CHECK_HR;
        
        for(int i = 0; i < nElements; ++i)
        {
            double lValue = *((double *)(&lvRawDataBuffer[i * lSizeOfDataType]));
            MDataType::DSSCellFmtData lCellFmtData(lValue, lvType[i], lvFmtIDs[i]);
            
            hr = ipHolder->setData(i, (char *)(&lCellFmtData), sizeof(MDataType::DSSCellFmtData));
            CHECK_HR;
        }
        return S_OK;
    }
    
   	else if (iDataType < DssDataTypeDate || iDataType > DssDataTypeTimeStamp)
	{
		return ipHolder->setDataInBulk(&lvRawDataBuffer[0], lvRawDataBuffer.size());
	}
	else 
	{
		char *lpData = &lvRawDataBuffer[0];
		for (int i = 0; i < nElements; i++)
		{
			
			MDataType::DSSDateTime lTmp(*((double*)lpData)/(double)MDataType::DAY_IN_MILLISECONDS + gSecondsSince1970);
			hr = ipHolder->setData(i, (char*)&lTmp, sizeof(MDataType::DSSDateTime));
			CHECK_HR;
			lpData += 8;
		}
		return S_OK;
	}
}
