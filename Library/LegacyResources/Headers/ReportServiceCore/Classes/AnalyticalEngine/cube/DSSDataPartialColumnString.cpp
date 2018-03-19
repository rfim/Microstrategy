//
//  DSSDataPartialColumnString.cpp
//  ReportServiceCore
//
//  Created by Tang, Cheng on 5/18/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//


#include "DSSDataPartialColumnString.h"
#include "DSSBinaryDataReader.h"

static const double gSecondsSince1970 = MDataType::DSSDateTime(1970, 1, 1).ToDATE();

DSSDataPartialColumnString::DSSDataPartialColumnString() : mMaxRowSize(-1)
{}

DSSDataPartialColumnString::~DSSDataPartialColumnString(){}


int DSSDataPartialColumnString::hCompareData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags, bool iIsNullDisplayOnTop, bool ibSimpleComparison)
{
    if (iRow >= mRowCount)
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

int DSSDataPartialColumnString::getSizeOfData()
{
    return mMaxRowSize;
}

int DSSDataPartialColumnString::getRowSize(long long iRow, int &orSize)
{
    if (iRow >= mRowCount)
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

    return lpHolder->getRowSize(iRow - mvStartKey[lOffset], orSize);
}

int DSSDataPartialColumnString::setData(long long iRow, char *ipData, int iSize, DSSDataFlag iFlag)
{
    if (iRow >= mRowCount)
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
        CHECK_HR;
        if (mMaxRowSize == 0)
        {
            hr = lpHolder->getRowSize(iRow - mvStartKey[lOffset], mMaxRowSize);
            CHECK_HR;
        }
    }
	else if (mDataType != DssDataTypeDate && mDataType != DssDataTypeTimeStamp && mDataType != DssDataTypeTime && mDataType != DssDataTypeCellFormatData && mDataType !=DssDataTypeBigDecimal) 
    {
        hr = lpHolder->setDataToNULL(iRow - mvStartKey[lOffset], iSize);
        CHECK_HR;
        
        if (mMaxRowSize < iSize)
            mMaxRowSize = iSize;
    }
	
	if(iFlag != DssDataOk)
		this->mbAllDataValid = false;
	
	return S_OK;
}

int DSSDataPartialColumnString::getData(long long iRow, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag)
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
        orType = mDataType;
        orFlag = mpFlags[iRow];
        return S_OK;
    }
}

int DSSDataPartialColumnString::getDataCopy(long long iRow, char *pBuffer, int iBufSize, int &orCopiedSize, EnumDSSDataType &orType, DSSDataFlag &orFlag)
{
    if (iRow >= mRowCount || (!pBuffer))
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
        
        orType = mDataType;
        orFlag = mpFlags[iRow];
        return S_OK;
    }
	
}


int DSSDataPartialColumnString::hLoadData(DSSBinaryDataReader *ipReadStream, DSSDataHolder* ipHolder, EnumDSSDataType iDataType, int nElements)
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
	for (i = 0; i < nElements; i++)
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
        
        if (mMaxRowSize < lRowSize)
            mMaxRowSize = lRowSize;
	}
    
    return S_OK;
    
}


