
//
//  DSSViewDataSetHelper.cpp
//  ReportServiceCore
//
//  Created by Yang Zhuo on 9/21/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//

#include "DSSViewDataSetHelper.h"
#include "DSSTabularIndex.h"
#include "DSSStrongPtr.h"
#import "DSSRWDataModel.h"
#import "DSSXTabBitVector.h"

#include "DSSCMNodeModel.h"
#include "DSSFileDataReader.h"

static const double gSecondsSince1970 = MDataType::DSSDateTime(1970, 1, 1).ToDATE();

enum EIndexOptType
{
	EIndexOptTypeReserved = 0,
	EIndexOptTypeTrueLevel,
	EIndexOptTypeRowMapShort,
	EIndexOptTypeRowMapInt,
	EIndexOptTypeRowMapCompressedShort,
	EIndexOptTypeRowMapCompressedInt,
	EIndexOptTypeCompressed,
	EIndexOptTypeOriginal,
	EIndexOptTypeColShort,
	EIndexOptTypeColInt,
	EIndexOptTypeColCompressedShort,
	EIndexOptTypeColCompressedInt,
    EIndexOptTypeNoOrderShort,
    EIndexOptTypeNoOrderInt,
	EIndexOptTypeReserved2,
};
// 6/11/2012 tehe: isOnDemandLoading should only set to true during the first loading
int DSSViewDataSetHelper::loadColumn(DSSBinaryDataReader * ipReadStream, DSSDataColumn *lpColumn, bool isOnDemandLoading, vector<int>* ipFilterList)
{
    int lOffset = ipReadStream->GetPosition();
    
    // tehe 5/22/2012
    if(isOnDemandLoading)
    {
        // int lOffset = ipReadStream->GetPosition();
        lpColumn->setDataBeginPosition(lOffset);
    }
    
    int hr = S_OK;
    
    EnumDSSDataType lDataType = lpColumn->getDataType();
    int lnElements = lpColumn->getRowCount();
    
    //zhyang, for partial retrieval, implement it later
    vector<int> lvPartialEleList;
    lvPartialEleList.resize(0);
    
    vector<int>* lpFilterList = ipFilterList ? ipFilterList : &lvPartialEleList;
    
    bool lbPartialData = lpFilterList->size() > 0;
    
    if (lbPartialData)
    {
        lnElements = lpFilterList->size();
    }
    
    bool lbIsRawData = true;
    // hr = ipReadStream->ReadBool(&lbIsRawData);
    // CHECK_HR;
    
    if (lbIsRawData)
    {
        if (DssDataTypeVarChar != lDataType && DssDataTypeBigDecimal != lDataType &&
            DssDataTypeMBChar != lDataType && DssDataTypeBinary != lDataType )//&& DssDataTypeCellFormatData != lDataType)
        {
            hr = hLoadRawDataColumn(ipReadStream, lpColumn, (EnumDSSDataType)lDataType, lnElements, *lpFilterList, isOnDemandLoading);
            CHECK_HR;
        }
        else
        {
            hr = hLoadStringDataColumn(ipReadStream, lpColumn, (EnumDSSDataType)lDataType, lnElements, *lpFilterList, isOnDemandLoading);
            CHECK_HR;
        }
    }
    else
    {
        hr = hLoadStringDataColumn(ipReadStream, lpColumn, (EnumDSSDataType)lDataType, lnElements, *lpFilterList, isOnDemandLoading);
        CHECK_HR;
    }
    
    vector<char> lvFlags;
    
    // 5/22/2012 tehe
    int lFlagPosition = ipReadStream->GetPosition();
    if(!isOnDemandLoading)
    {
        hr = ipReadStream->ReadByteArray(lvFlags);
        CHECK_HR;
    }
    // 5/17/2012 tehe: skip flags
    else
    {
        // int lFlagPosition = ipReadStream->GetPosition();
        lpColumn->setFlagBeginPosition(lFlagPosition);
        
        int lSize = 0;
        hr = ipReadStream->ReadInt(&lSize);
        CHECK_HR;
        
        lSize *= sizeof(char);
        ipReadStream->Skip(lSize);
        
    }
    
    if (!lvFlags.empty())
    {
        if(lbPartialData)
            hr = lpColumn->setFlags(0, lvFlags.size(), (DSSDataFlag*)(&lvFlags[0]), (int*)(&(*lpFilterList)[0]), lpFilterList->size());
        else
            hr = lpColumn->setFlags(0, lvFlags.size(), (DSSDataFlag*)(&lvFlags[0]), lvFlags.size() * sizeof(char));
        CHECK_HR;
    }
    
    if(isOnDemandLoading)
    {
        int lOffset;
        lOffset = ipReadStream->GetPosition();
        lpColumn->setDataEndPosition(lOffset);
    }
    
    // 6/29/2012 tehe
    //#if _DEBUG
    //    std::cout<<"column position:"<<lOffset<<" flagPosition:"<<lFlagPosition<<" difference:"<<(lFlagPosition-lOffset)<<endl;
    //#endif
    return hr;
}

size_t DSSViewDataSetHelper::hSizeOf(EnumDSSDataType iDataType)
{
	switch (iDataType)
	{
		case DssDataTypeShort:
			return sizeof(short);
		case DssDataTypeInteger:
		case DssDataTypeLong:
			return sizeof(int);
		case DssDataTypeFloat:
			return sizeof(float);
		case DssDataTypeDouble:
			return sizeof(double);
		case DssDataTypeBool:
		case DssDataTypeMissing:
			return sizeof(bool);
		case DssDataTypeTime:
		case DssDataTypeDate:
		case DssDataTypeTimeStamp:
		case DssDataTypeCellFormatData:
			return sizeof(double);
		case DssDataTypeChar:
		case DssDataTypeReserved:
		case DssDataTypeUnknown:
			return sizeof(char);
        case DssDataTypeInt64://xiazhou, 2014/04/23, 875139
            return sizeof(long long);
		default:
			return 0;
			break;
	}
}

int DSSViewDataSetHelper::hLoadRawDataColumn(DSSBinaryDataReader *ipReadStream, DSSDataColumn *ipColumn, EnumDSSDataType iDataType, int nElements, vector<int> &irvElementList, bool isOnDemandLoading)
{
    int hr = S_OK;
    
	bool lbIsFullColumn = irvElementList.empty();
	
	int lSizeOfDataType = hSizeOf(iDataType);
	int nSize = lSizeOfDataType * nElements;
    //zhyang, 699028, even there is no elements, there should be some metadata sent to client, need de-serialize them.
    
    //hyan,702024
	if (nSize <= 0&&DssDataTypeCellFormatData != iDataType)
		return 0;
    
    
    vector<char> lvRawDataBuffer;
    if(!isOnDemandLoading)
    {
        lvRawDataBuffer.resize(nSize);
        hr = ipReadStream->Read((unsigned char*)(&lvRawDataBuffer[0]), nSize, lSizeOfDataType);
        CHECK_HR;
        
        if (iDataType == DssDataTypeLong)
        {
            char *lpData = &lvRawDataBuffer[0];
            for (int i = 0; i < nElements; i++)
            {
                int elementID = lbIsFullColumn ? i : irvElementList[i];
                //411868, long long is 8 bytes in Mac OS
                long long lVal = *(int*)lpData;
                hr = ipColumn->setData(elementID, (char*)&lVal, sizeof(long long), 0);
                CHECK_HR;
                
                lpData += lSizeOfDataType;
            }
            return S_OK;
        }
        if (DssDataTypeCellFormatData == iDataType)
        {
            vector<int> lvType;
            vector<int> lvFmtIDs;
            vector<string> lvFmtGUIDStr;
            
            hr = ipReadStream->ReadIntArray(lvType);
            CHECK_HR;
            
            hr = ipReadStream->ReadIntArray(lvFmtIDs);
            CHECK_HR;
            
            hr = loadStringVector(ipReadStream, lvFmtGUIDStr);
            CHECK_HR;
            
            for(int i = 0; i < nElements; ++i)
            {
                GUID lGUID;
                hr = DSSViewDataSetHelper::str2GUID(lvFmtGUIDStr[lvFmtIDs[i]], lGUID);
                CHECK_HR;
                
                double lValue = *((double *)(&lvRawDataBuffer[i * lSizeOfDataType]));
                MDataType::DSSCellFmtData lCellFmtData(lValue, lvType[i], lGUID);
                
                //int size = sizeof(MDataType::DSSCellFmtData);
                
                hr = ipColumn->setData(i, (char *)(&lCellFmtData), sizeof(MDataType::DSSCellFmtData), 0);
                CHECK_HR;
            }
            return S_OK;
        }
        
        else if (iDataType < DssDataTypeDate || iDataType > DssDataTypeTimeStamp)
        {
            if (lbIsFullColumn)
            {
                return ipColumn->setDataInBulk(0, nElements, &lvRawDataBuffer[0], lvRawDataBuffer.size(), NULL, 0);
            }
            else
            {
                char *lpData = &lvRawDataBuffer[0];
                for (int i = 0; i < nElements; i++)
                {
                    int elementID = irvElementList[i];
                    hr = ipColumn->setData(elementID, lpData, lSizeOfDataType, DssDataOk);
                    CHECK_HR;
                    lpData += lSizeOfDataType;
                }
            }
            return S_OK;
        }
        else
        {
            char *lpData = &lvRawDataBuffer[0];
            for (int i = 0; i < nElements; i++)
            {
                int elementID = lbIsFullColumn ? i : irvElementList[i];
                
                MDataType::DSSDateTime lTmp(*((double*)lpData)/(double)MDataType::DAY_IN_MILLISECONDS + gSecondsSince1970);
                hr = ipColumn->setData(elementID, (char*)&lTmp, sizeof(MDataType::DSSDateTime), 0);
                CHECK_HR;
                lpData += 8;
            }
            return S_OK;
        }
        
    }
    // 5/17/2012 tehe: delay loading
	else
    {
        //zhyang, 700546
        //if(DssDataTypeCellFormatData != iDataType)
        {
            ipReadStream->Skip(nSize);
        }
        if(DssDataTypeCellFormatData == iDataType)
        {
            int lTypeSize = 0;
            int lFmtIDSize = 0;
            hr = ipReadStream->ReadInt(&lTypeSize);
            CHECK_HR;
            
            lTypeSize *= sizeof(int);
            ipReadStream->Skip(lTypeSize);
            
            hr = ipReadStream->ReadInt(&lFmtIDSize);
            CHECK_HR;
            
            lFmtIDSize *= sizeof(int);
            ipReadStream->Skip(lFmtIDSize);
            
            vector<string> lvFmtGUIDStr;
            hr = loadStringVector(ipReadStream, lvFmtGUIDStr);
            CHECK_HR;
            
        }
    }
    return hr;
}

int DSSViewDataSetHelper::hLoadStringDataColumn(DSSBinaryDataReader *ipReadStream, DSSDataColumn *ipColumn, EnumDSSDataType iDataType, int nElements, vector<int> &irvElementList, bool isOnDemandLoading)
{
    int hr = S_OK;
    
    if(!isOnDemandLoading)
    {
        
        bool lbIsString = (DssDataTypeChar == iDataType || DssDataTypeVarChar == iDataType
                           || DssDataTypeMBChar == iDataType || DssDataTypeUTF8Char == iDataType);
        
        bool lbIsDateTime = !lbIsString && (DssDataTypeDate == iDataType || DssDataTypeTime == iDataType
                                            || DssDataTypeTimeStamp == iDataType);
        
        vector<char> lvUTF8Data(32);		//initialize to 32 bytes
        
        vector<vector<char> > lvvCellFormat;
        
        int lBufferSize = 0;
        lBufferSize = ipColumn->getSizeOfData();
        CHECK_HR;
        
        if (lBufferSize < 8)
            lBufferSize = 8;
        vector<char> lvNumericalData(lBufferSize);
        
        bool lbIsFullColumn = irvElementList.empty();
        
        int i = 0;
        for (i = 0; i < nElements; i++)
        {
            int elementID = lbIsFullColumn ? i : irvElementList[i];
            
            unsigned int lStrLen = 0;
            hr = ipReadStream->ReadStringLen(&lStrLen);
            CHECK_HR;
            
            if (lStrLen > lvUTF8Data.size())
                lvUTF8Data.resize(lStrLen * 2);
            
            hr = ipReadStream->ReadUTF8String(&lvUTF8Data[0], lStrLen);
            CHECK_HR;
            
            if (lbIsString)
            {
                hr = ipColumn->setData(elementID, &lvUTF8Data[0], lStrLen * sizeof(char), DssDataOk);
                CHECK_HR;
            }
            else if(iDataType==DssDataTypeBigDecimal)
            {
                MDataType::DSSBigDecimal lBigDecimal(&(lvUTF8Data[0]));
                hr=ipColumn->setData(elementID, reinterpret_cast<char *> (&lBigDecimal), sizeof(MDataType::DSSBigDecimal),DssDataOk);
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
                
                hr = ipColumn->setData(elementID, &lHexString[0], (lStrLen * 2 + 1) * sizeof(char), DssDataOk);
                CHECK_HR;
            }
            else if (!lbIsDateTime)
            {
                int lDataSize = 0;
                hr = DSSAEHelper::ConvertStringToData(&lvUTF8Data[0], iDataType, &lvNumericalData[0], lBufferSize, lDataSize);
                CHECK_HR;
                
                
                hr = ipColumn->setData(elementID, &lvNumericalData[0], lDataSize, DssDataOk);
                CHECK_HR;
            }
            else
            {
                double lVal = atof(&lvUTF8Data[0]);
                MDataType::DSSDateTime lTmp(lVal/(double)MDataType::DAY_IN_MILLISECONDS + gSecondsSince1970);
                hr = ipColumn->setData(elementID, (char*)&lTmp, sizeof(MDataType::DSSDateTime), 0);
                CHECK_HR;
            }
        }
        
    }
    // 5/17/2012 tehe: delay loading
    else
    {
        unsigned int lStrLen = 0;
        int lSize = 0;
        for (int i = 0; i < nElements; i++)
        {
            hr = ipReadStream->ReadStringLen(&lStrLen);
            CHECK_HR;
            
            lSize = lStrLen * sizeof(char);
            ipReadStream->Skip(lSize);
        }
    }
    return S_OK;
    
}

int DSSViewDataSetHelper::loadStringVector(DSSBinaryDataReader * ipReadStream, vector<string> &ivStrs)
{
    int hr = S_OK;
    
    int lLen = -1;
    hr = ipReadStream->ReadInt(&lLen);
    CHECK_HR;
    
    for(int i = 0; i < lLen; ++i)
    {
        string lStr;
        hr = ipReadStream->ReadString(lStr);
        CHECK_HR;
        
        ivStrs.push_back(lStr);
    }
    
    return hr;
}

int DSSViewDataSetHelper::loadIndex(DSSBinaryDataReader * ipReadStream, DSSTabularIndex **ippIndex, DSSTabularIndexPool *ipIndexPool,bool *opIndexExist,
                                    DSSCMNodeModel *ipCMNodeModel)
{
    int hr = S_OK;
    
    int lBlockType;
    hr = ipReadStream->BeginReadBlock(&lBlockType);
    CHECK_HR_BLOCK(EnumNewBlockIndex);
    
    int lObjectType = -1;
    hr = ipReadStream->ReadInt(&lObjectType);
    CHECK_HR_OBJTYPE(NewFlexObjectIndex);
    
    if(!ippIndex)
    {
        return E_FAIL;
    }
    
    int lnRows = 0;
    hr = ipReadStream->ReadInt(&lnRows);
    CHECK_HR;
    
    int lnUnits;
    hr = ipReadStream->ReadInt(&lnUnits);
    CHECK_HR;
    
    int *lpUnits = new int[lnUnits];
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lUnitsPtr(lpUnits);
    
    hr = ipReadStream->ReadIntArray(lpUnits, lnUnits);
    CHECK_HR;
    
    DSSTabularIndex *lpIndex = NULL;
    
    if(opIndexExist)
    {
        *opIndexExist = false;
    }
    
    bool lbFind = false;
    if(ipIndexPool)
    {
        lbFind = ipIndexPool->Search(lnUnits, lpUnits, &lpIndex);
    }
    
    if(!lbFind)
    {
        lpIndex = new DSSTabularIndex();
        
        int lBlockType;
        hr = ipReadStream->BeginReadBlock(&lBlockType);
        CHECK_HR_BLOCK(EnumNewBlockIndexKey);
        
        int *lpKeys = NULL;
        MBase::StrongPtr<int> lKeyPtr;
        // tehe: memory optimization
        long lReservedSize = lnUnits * lnRows * sizeof(int);
        
#ifdef __ANDROID__
		ipCMNodeModel = NULL;
#endif
        //de49639, msun, with financial report, we have displayMainIndex, but current cache logic only consider the mainIndex.
        //So for the displayMainIndex, it is possible the lReservedSize is big but the cache file is not created.
        bool lbCacheExist = false;
        if (ipCMNodeModel) {
            lbCacheExist = ipCMNodeModel->FindLocalCache();
        }
        //DE51608, msun, for FG case, lbCacheExist always false, so should ignore this check
        if(lReservedSize < MEM_GOVERNOR_SIZE || ipCMNodeModel == NULL || (!lbCacheExist && ipCMNodeModel->getNodeType() != DssRWNodeFieldGroup) || !DataSourceUtil::isMappedFileSupported())
        {
            MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)
            lpKeys = new int[lnUnits * lnRows];
            CHECK_NEWED_PTR(lpKeys);
            lKeyPtr.Reset(lpKeys);
            MEM_GOVERNOR_COMMIT(lReservedSize);
            
            hr = hLoadIndexKeys(ipReadStream, ipIndexPool, lnRows, lnUnits, lpUnits, lpKeys, NULL);
            CHECK_HR;
            
        }
        else
        {
            CMBlockHeader lBlockHeader;
            lBlockHeader.mBlockType = cmTabularIndex;
            lBlockHeader.mBlockSize = 0;
            lBlockHeader.mBlockOffset = -1;
            hr = ipCMNodeModel->AddBlockHeader(cmTabularIndex, lReservedSize, &lBlockHeader);
            CHECK_HR;
            
            
            hr = hLoadIndexKeys(ipReadStream, ipIndexPool, lnRows, lnUnits, lpUnits, lpKeys, ipCMNodeModel);
            CHECK_HR;
            
            hr = ipCMNodeModel->MMap(lpIndex, lBlockHeader.mBlockOffset, lBlockHeader.mBlockSize);
			CHECK_HR;
            
            lpKeys = const_cast<int*>(lpIndex->getKeyTable());
            
            
        }
        
        hr = lpIndex->Init(lnUnits, lnRows, lpUnits, NULL, NULL);
        CHECK_HR;
        
        lpIndex->setKeys(lpKeys, lnUnits * lnRows);
        
        lKeyPtr.GiveUp();
        
        hr = ipReadStream->EndReadBlock();
        CHECK_HR;
    }
    else
    {
        if(opIndexExist)
        {
            *opIndexExist = true;
        }
        
        int lBlockType;
        hr = ipReadStream->BeginReadBlock(&lBlockType);
        CHECK_HR_BLOCK(EnumNewBlockIndexKey);
        
        hr = ipReadStream->EndReadBlock();
        CHECK_HR;
    }
    
    int isMainIndex;
    hr = ipReadStream->ReadInt(&isMainIndex);
    
    if(isMainIndex != 0)
    {
        hr = lpIndex->loadBitVectorFromBinary(ipReadStream);
        CHECK_HR;
    }
    
    BYTE isDistinct;
    hr = ipReadStream->ReadByte(&isDistinct);
    CHECK_HR;
    
    hr = lpIndex->setDistinct((bool)isDistinct);
    CHECK_HR;
    
    //zhyang, performance
    BYTE isMainIndexOrder;
    hr = ipReadStream->ReadByte(&isMainIndexOrder);
    CHECK_HR;
    
    // tehe
    lpIndex->setMainIndexOrder(isMainIndexOrder);
    
    BYTE isSortByLastUnit;
    hr = ipReadStream->ReadByte(&isSortByLastUnit);
    CHECK_HR;
    
    lpIndex->setSortByLastUnit(isSortByLastUnit);
    
    if (ipReadStream->MoreDataInBlock() && ipReadStream->ProbeBlockHeader(EnumNewBlockIndexHiddenMask)) {
        int lBlockType;
        hr = ipReadStream->BeginReadBlock(&lBlockType);
        CHECK_HR_BLOCK(EnumNewBlockIndexHiddenMask);
        int hr = lpIndex->loadHiddenMaskFromBinary(ipReadStream);
        ipReadStream->EndReadBlock();
        CHECK_HR;
    }
    
    hr = ipReadStream->EndReadBlock();
    CHECK_HR;
    
    *ippIndex = lpIndex;
    
    return hr;
}

int DSSViewDataSetHelper::str2GUID(string &irVal, GUID &iGUID)
{
    int len = irVal.length();
	if (len > 0)
	{
#ifdef NO_WCHAR_SUPPORT
        MBase::String2Guid(iGUID, irVal.c_str());
#else
		vector<wchar_t> lString(len);
		mbstowcs(&lString[0], irVal.c_str(), len);
		MBase::String2Guid(iGUID, &lString[0]);
#endif
	}
	return S_OK;
}

int DSSViewDataSetHelper::hDecompressUnitKeys(int *ipKeys, int &iCurrentPos, int inUnits, int iUnit, int iStartPos, int iEndPos, vector<int>  &ivUnitCompressedKey, DSSXTabBitVector &iCompressedBitVector)
{
    int hr = S_OK;
    for(int i = iStartPos; i < iEndPos; ++i)
    {
        if(iCompressedBitVector.Get(2 * i) == true && iCompressedBitVector.Get(2 * i + 1) == true)
        {
            //range
            int lStartValue = ivUnitCompressedKey[i++];
            int lStepSize = ivUnitCompressedKey[i++];
            int lSteps = ivUnitCompressedKey[i];
            
            for (int j = 0; j <= lSteps; ++j) {
                ipKeys[iCurrentPos * inUnits + iUnit] = lStartValue + lStepSize * j;
                ++iCurrentPos;
            }
        }
        else if(iCompressedBitVector.Get(2 * i) == true && iCompressedBitVector.Get(2 * i + 1) == false)
        {
            //repeate
            int lRepeateLength = ivUnitCompressedKey[i++];
            int lRepeateTimes = ivUnitCompressedKey[i++];
            for(int j = 0; j < lRepeateTimes; j++)
            {
                hr = hDecompressUnitKeys(ipKeys, iCurrentPos, inUnits, iUnit, i, i + lRepeateLength, ivUnitCompressedKey, iCompressedBitVector);
                CHECK_HR;
            }
            i += lRepeateLength;
        }
        else
        {
            //data
            ipKeys[iCurrentPos * inUnits + iUnit] = ivUnitCompressedKey[i];
            ++iCurrentPos;
        }
    }
    return hr;
}

int DSSViewDataSetHelper::hLoadShortArray(DSSBinaryDataReader * ipReadStream, vector<int> &lData)
{
    int lLength = -1;
    int hr = ipReadStream->ReadInt(&lLength);
    CHECK_HR;
    
    //    for(int i = 0; i < lLength; ++i)
    //    {
    //        short lShort;
    //        hr = ipReadStream->ReadShort(&lShort);
    //        CHECK_HR;
    //
    //        lData.push_back(lShort);
    //    }
    // 8/27/2012 tehe: optimize hLoadShortArray,avoid push back
    if(lLength >0)
    {
        lData.resize(lLength);
        vector<short> lvData;
        lvData.resize(lLength);
        int hr = ipReadStream->ReadShortArray(&(lvData[0]), lLength);
        CHECK_HR;
        for (int i =0; i<lLength; i++)
        {
            lData[i] = lvData[i];
        }
    }
    
    return hr;
}

int DSSViewDataSetHelper::hDecompressRange(vector<int> &ivCompressedData, vector<int> &ovDecompressedData)
{
    int k = 0;
    for(int i = 0; i < ivCompressedData.size(); ++i)
    {
        int lStart = ivCompressedData[i++];
        int lStepSize = ivCompressedData[i++];
        int lStepNum = ivCompressedData[i];
        
        for(int j = 0; j <= lStepNum; ++j)
        {
            // ovDecompressedData.push_back(lStart + j * lStepSize);
            // 8/27/2012 tehe: avoid push_back method
            ovDecompressedData[k++] = lStart + j * lStepSize;
        }
    }
    return S_OK;
}

int DSSViewDataSetHelper::PartialLoadColumn(DSSBinaryDataReader * ipReadStream, DSSDataColumn *lpColumn, vector<int> & ivPartialEleList)
{
    // 6/11/2012 tehe: we need this info(relative to model) to calculate the position of flag
    int lOffset = ipReadStream->GetPosition();
    
    int hr = S_OK;
    
    EnumDSSDataType lDataType = lpColumn->getDataType();
    
    bool lbIsRawData = true;
    //    hr = ipReadStream->ReadBool(&lbIsRawData);
    // CHECK_HR;
    
    if (lbIsRawData)
    {
        if (DssDataTypeVarChar != lDataType && DssDataTypeBigDecimal != lDataType &&
            DssDataTypeMBChar != lDataType && DssDataTypeBinary != lDataType )//&& DssDataTypeCellFormatData != lDataType)
        {
            hr = hPartialLoadRawDataColumn(ipReadStream, lpColumn, (EnumDSSDataType)lDataType, ivPartialEleList);
            CHECK_HR;
        }
        else
        {
            hr = hPartialLoadStringDataColumn(ipReadStream, lpColumn, (EnumDSSDataType)lDataType, ivPartialEleList);
            CHECK_HR;
        }
    }
    else
    {
        hr = hPartialLoadStringDataColumn(ipReadStream, lpColumn, (EnumDSSDataType)lDataType, ivPartialEleList);
        CHECK_HR;
    }
    
    vector<char> lvFlags;
    
    // 6/11/2012 tehe: we should adjust the position of flag
    int lFlagDataPosition = lpColumn->getFlagBeginPosition();
    int lColumnOffset = lpColumn->getDataBeginPostion();
    
    // 6/29/2012 tehe
    //#if _DEBUG
    //    std::cout<<"column position:"<<lOffset<<" flagPosition:"<<(lFlagDataPosition-lColumnOffset+lOffset)<<" difference:"<<(lFlagDataPosition-lColumnOffset)<<std::endl;
    //#endif
    
    ipReadStream->SetPosition(lFlagDataPosition-lColumnOffset+lOffset);
    
    hr = ipReadStream->ReadByteArray(lvFlags);
    CHECK_HR;
    
    // 6/29/2012 tehe
    //#if _DEBUG
    //    std::cout<<"flag value"<<std::endl;
    //    for(int i=0; i<lvFlags.size(); i++)
    //    {
    //        std::cout<<lvFlags[i]<<" ";
    //    }
    //    std::cout<<std::endl;
    //#endif
    
    if(!lvFlags.empty())
    {
        for (int i=0; i<ivPartialEleList.size(); i++)
        {
            hr = lpColumn->setFlag(ivPartialEleList[i], lvFlags[ivPartialEleList[i]]);
            CHECK_HR;
        }
    }
    
    return hr;
}

int DSSViewDataSetHelper::hPartialLoadRawDataColumn(DSSBinaryDataReader * ipReadStream, DSSDataColumn *lpColumn, EnumDSSDataType iDataType, vector<int> & ivPartialEleList)
{
    // tehe: special handling full slice
    int lRowCount = lpColumn->getRowCount();
    if(ivPartialEleList.size() == lRowCount)
    {
        vector<int> lPartialEleList;
        return hLoadRawDataColumn(ipReadStream, lpColumn, iDataType, lRowCount, lPartialEleList);
    }
    int hr = S_OK;
    int lSizeOfDataType = hSizeOf(iDataType);
    int lStartPosition = ipReadStream->GetPosition();
    
    // 5/22/2012 tehe: here we can analysis ivPartialEleList, so we can get sequential data from data stream, current we haven't implement it yet.
    unsigned char *lBuffer = new unsigned char [lSizeOfDataType];
    MBase::StrongPtr<unsigned char, MBase::DeleteArray<unsigned char> > pBuffer(lBuffer);
    
    vector<int> lvType;
    vector<int> lvFmtIDs;
    vector<string> lvFmtGUIDStr;
    if(iDataType == DssDataTypeCellFormatData)
    {
        ipReadStream->Skip(lSizeOfDataType*lRowCount);
        hr = ipReadStream->ReadIntArray(lvType);
        CHECK_HR;
        
        hr = ipReadStream->ReadIntArray(lvFmtIDs);
        CHECK_HR;
        
        hr = loadStringVector(ipReadStream, lvFmtGUIDStr);
        CHECK_HR;
    }
    
    ipReadStream->SetPosition(lStartPosition);
    // 6/29/2012 tehe
    //#if _DEBUG
    //    std::cout<<"begin partial column value:"<<std::endl;
    //#endif
    for(int i=0; i<ivPartialEleList.size(); i++)
    {
        int lOffset = ivPartialEleList[i]*lSizeOfDataType+lStartPosition;
        hr = ipReadStream->SetPosition(lOffset);
        CHECK_HR;
        hr = ipReadStream->Read(lBuffer, lSizeOfDataType, lSizeOfDataType);
        CHECK_HR;
        
        // 6/28/2012 tehe
        //#if _DEBUG
        //        double ldTemp = *(double*)lBuffer;
        //        std::cout<<ldTemp<<" ";
        //#endif
        
        if(iDataType == DssDataTypeLong)
        {
            //zhyang, 833899
            long long lVal = *(int*)lBuffer;
            hr = lpColumn->setData(ivPartialEleList[i], (char*)&lVal, sizeof(long long), 0);
            CHECK_HR;
        }
        else if (iDataType == DssDataTypeCellFormatData)
        {
            GUID lGUID;
            hr = DSSViewDataSetHelper::str2GUID(lvFmtGUIDStr[lvFmtIDs[ivPartialEleList[i]]], lGUID);
            CHECK_HR;
            
            double lValue = *((double *)(lBuffer));
            MDataType::DSSCellFmtData lCellFmtData(lValue, lvType[ivPartialEleList[i]], lGUID);
            
            hr = lpColumn->setData(ivPartialEleList[i], (char *)(&lCellFmtData), sizeof(MDataType::DSSCellFmtData), 0);
            CHECK_HR;
            
        }
        else if (iDataType < DssDataTypeDate || iDataType > DssDataTypeTimeStamp)
        {
            hr = lpColumn->setData(ivPartialEleList[i], (char*)lBuffer, lSizeOfDataType, DssDataOk);
            CHECK_HR;
        }
        else
        {
            MDataType::DSSDateTime lTmp(*((double*)lBuffer)/(double)MDataType::DAY_IN_MILLISECONDS + gSecondsSince1970);
            hr = lpColumn->setData(ivPartialEleList[i], (char*)&lTmp, sizeof(MDataType::DSSDateTime), 0);
            CHECK_HR;
        }
        
    }
    //#if _DEBUG
    //    std::cout<<"end partial column value:"<<std::endl;
    //#endif
    return S_OK;
}

int DSSViewDataSetHelper::hPartialLoadStringDataColumn(DSSBinaryDataReader * ipReadStream, DSSDataColumn *lpColumn, EnumDSSDataType iDataType, vector<int> & ivPartialEleList)
{
    int lRowCount = lpColumn->getRowCount();
    if(ivPartialEleList.size() == lRowCount)
    {
        vector<int> lPartialEleList;
        return hLoadStringDataColumn(ipReadStream, lpColumn, iDataType, lRowCount, lPartialEleList);
    }
    
    int hr = S_OK;
    
    bool lbIsString = (DssDataTypeChar == iDataType || DssDataTypeVarChar == iDataType
                       || DssDataTypeMBChar == iDataType || DssDataTypeUTF8Char == iDataType);
    
    bool lbIsDateTime = !lbIsString && (DssDataTypeDate == iDataType || DssDataTypeTime == iDataType
                                        || DssDataTypeTimeStamp == iDataType);
    
    vector<char> lvUTF8Data(32);		//initialize to 32 bytes
    
    vector<vector<char> > lvvCellFormat;
    
    int lBufferSize = 0;
    lBufferSize = lpColumn->getSizeOfData();
    CHECK_HR;
    
    if (lBufferSize < 8)
        lBufferSize = 8;
    vector<char> lvNumericalData(lBufferSize);
    
    int j = 0;
    
    for(int i=0; i< lRowCount && j<ivPartialEleList.size(); i++)
    {
        unsigned int lStrLen = 0;
        hr = ipReadStream->ReadStringLen(&lStrLen);
        CHECK_HR;
        
        if(i != ivPartialEleList[j])
        {
            ipReadStream->Skip(lStrLen);
        }
        else
        {
            if (lStrLen > lvUTF8Data.size())
                lvUTF8Data.resize(lStrLen * 2);
            
            hr = ipReadStream->ReadUTF8String(&lvUTF8Data[0], lStrLen);
            CHECK_HR;
            
            if(lbIsString)
            {
                hr = lpColumn->setData(i, &lvUTF8Data[0], lStrLen * sizeof(char), DssDataOk);
                CHECK_HR;
                
            }
            else if(iDataType == DssDataTypeBigDecimal)
            {
                MDataType::DSSBigDecimal lBigDecimal(&(lvUTF8Data[0]));
                hr = lpColumn->setData(i, reinterpret_cast<char *> (&lBigDecimal), sizeof(MDataType::DSSBigDecimal),DssDataOk);
                CHECK_HR;
                
            }
            else if(iDataType == DssDataTypeBinary)
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
                
                hr = lpColumn->setData(i, &lHexString[0], (lStrLen * 2 + 1) * sizeof(char), DssDataOk);
                CHECK_HR;
                
            }
            else if (!lbIsDateTime)
            {
                int lDataSize = 0;
                hr = DSSAEHelper::ConvertStringToData(&lvUTF8Data[0], iDataType, &lvNumericalData[0], lBufferSize, lDataSize);
                CHECK_HR;
                
                hr = lpColumn->setData(i, &lvNumericalData[0], lDataSize, DssDataOk);
                CHECK_HR;
                
            }
            else
            {
                double lVal = atof(&lvUTF8Data[0]);
                MDataType::DSSDateTime lTmp(lVal/(double)MDataType::DAY_IN_MILLISECONDS + gSecondsSince1970);
                hr = lpColumn->setData(i, (char*)&lTmp, sizeof(MDataType::DSSDateTime), 0);
                CHECK_HR;
            }
            j++;
        }
    }
    return S_OK;
}

// 11/12/2012 tehe: initalize key value for index
int DSSViewDataSetHelper::hInitKeyValue(DSSTabularIndex *ipSourceIndex, int iUnitCount, int *ipUnits, int iRowCount, int *ipRowMap, int *iopKeys)
{
    CHECK_PTR(ipSourceIndex);
    CHECK_PTR(iopKeys);
    CHECK_PTR(ipUnits);
    
    int hr = S_OK;
    int lnSourceUnits = ipSourceIndex->getNumUnits();
    vector<int> lvSourceUnits;
    hr = ipSourceIndex->getUnits(lvSourceUnits);
    CHECK_HR;
    
    vector<int> lvUnitMap(iUnitCount);
    for(int i = 0; i < iUnitCount; ++i)
    {
        lvUnitMap[i] = -1;
        int j = 0;
        for(j = 0; j < lnSourceUnits; ++j)
        {
            if(ipUnits[i] == lvSourceUnits[j])
            {
                lvUnitMap[i] = j;
                break;
            }
        }
        if(j == lnSourceUnits)
            assert(false);
    }
    DSSXTabKeyIterator lKeyIterator(ipSourceIndex);
    vector<int> lvRowIndex;
    lvRowIndex.resize(lnSourceUnits, -1);
    // 9/6/2012 tehe: for performance
    for(int iRow = 0; iRow < iRowCount; ++iRow)
    {
        if(ipRowMap != NULL)
        {
            lKeyIterator.SetRow(ipRowMap[iRow]);
        }
        else
        {
            lKeyIterator.SetRow(iRow);
        }
        lKeyIterator.GetRow(lnSourceUnits, &(lvRowIndex[0]));
        
        for(int iUnit = 0; iUnit < iUnitCount; ++iUnit)
        {
            iopKeys[iUnitCount * iRow + iUnit] = lvRowIndex[ lvUnitMap[iUnit] ];
        }
    }
    return hr;
}

int DSSViewDataSetHelper::loadCompressedIntArray(DSSBinaryDataReader * ipReadStream, vector<int> &iovArray)
{
    int lUnitOptType = 0;
    int hr = ipReadStream->ReadInt(&lUnitOptType);
    CHECK_HR;
    
    int lOriginalLength;
    
    hr = ipReadStream->ReadInt(&lOriginalLength);
    CHECK_HR;
    
    switch (lUnitOptType) {
        case EIndexOptTypeColShort:
        {
            int lLength = -1;
            hr = ipReadStream->ReadInt(&lLength);
            CHECK_HR;
            
            // 8/21/2012 tehe: performance
            if(lLength > 0)
            {
                vector<short> lvValue(lLength);
                iovArray.resize(lLength);
                
                hr = ipReadStream->ReadShortArray(&(lvValue[0]), lLength);
                CHECK_HR;
                
                for (int i= 0; i< lLength; ++i)
                {
                    iovArray[i] = lvValue[i];
                }
                
            }
        }
            break;
        case EIndexOptTypeColInt:
        {
            hr = ipReadStream->ReadIntArray(iovArray);
            CHECK_HR;
        }
            break;
        case EIndexOptTypeColCompressedShort:
        {
            vector<int> lvCompressedKeys;
            
            hr = hLoadShortArray(ipReadStream, lvCompressedKeys);
            CHECK_HR;
            
            // 8/27/2012 tehe:optimize decompress range
            iovArray.resize(lOriginalLength);
            
            hr = hDecompressRange(lvCompressedKeys, iovArray);
            CHECK_HR;
        }
            break;
        case EIndexOptTypeColCompressedInt:
        {
            vector<int> lvCompressedKeys;
            
            hr = ipReadStream->ReadIntArray(lvCompressedKeys);
            CHECK_HR;
            
            // 8/27/2012 tehe:optimize decompress range
            iovArray.resize(lOriginalLength);
            hr = hDecompressRange(lvCompressedKeys, iovArray);
            CHECK_HR;
            
        }
            break;
        default:
            assert(false);
            break;
    }
    
    return hr;
}

struct ColIndexKeyInfo
{
    EIndexOptType optType;
    int curpos;
    int start, stepSize, stepNum;
};

int DSSViewDataSetHelper::hLoadIndexKeys(DSSBinaryDataReader *ipReadStream, DSSTabularIndexPool *ipIndexPool, int inRows, int inUnits, int *ipUnits, int *iopKeys, DSSCMNodeModel *ipCMNodeModel)
{
    
    bool lbUseCacheManager = ipCMNodeModel == NULL ? false : true;
    //lbUseCacheManager = false;
    
    int lIndexOptType = 0;
    int hr = ipReadStream->ReadInt(&lIndexOptType);
    CHECK_HR;
    
    vector<int> lvIndexOrder;
    if(EIndexOptTypeNoOrderShort == lIndexOptType)
    {
        hr = hLoadShortArray(ipReadStream, lvIndexOrder);
        CHECK_HR;
        assert(lvIndexOrder.size() == inRows);
    }
    else if(EIndexOptTypeNoOrderInt == lIndexOptType)
    {
        hr = ipReadStream->ReadIntArray(lvIndexOrder);
        CHECK_HR;
        assert(lvIndexOrder.size() == inRows);
    }
    //currently we only have original data
    
    // 9/3/2013 tehe: do optimize for buffer, avoid waste memory in setPosition of file reader
    // #define BUFFERSIZE 512*1024
    int BUFFERSIZE = DATA_BLOCK_SIZE * inUnits;
    
    int *lpbuffer = NULL;
    int lnMaxRowCanWrite = 0;
    if(lbUseCacheManager)
    {
        int reserverlen = BUFFERSIZE/sizeof(int);
        MEM_GOVERNOR_RESERVE(BUFFERSIZE, BUFFERSIZE)
        lpbuffer= new int[reserverlen];
        CHECK_NEWED_PTR(lpbuffer);
        MEM_GOVERNOR_COMMIT(BUFFERSIZE);
        
        if(inUnits != 0)
        {
            lnMaxRowCanWrite = reserverlen / (inUnits);
        }
    }
    MBase::StrongPtr<int, MBase::DeleteArray<int> > lpBufferStrongPtr(lpbuffer);
    //yimliu, 2014/06/25, 918611, Memory Governor. Call MEM_GOVERNOR_RELEASE when it deconstructs.
    DSSMemoryGovernorHelper MGovernorHelper(BUFFERSIZE);
    
    switch ((EIndexOptType)lIndexOptType) {
        case EIndexOptTypeTrueLevel:
            if (ipIndexPool)
            {
                DSSTabularIndex *lpMainIndex = NULL;
                hr = ipIndexPool->getMainIndex(&lpMainIndex);
                CHECK_HR;
                
                if(!lpMainIndex)
                {
                    assert(false);
                }
                
                if(lbUseCacheManager)
                {
                    int lnRemainRowCnt = inRows;
                    int lStartRow = 0;
                    while (lnRemainRowCnt > 0) {
                        int lnRow = min(lnRemainRowCnt, lnMaxRowCanWrite);
                        
                        vector<int> lvRowMap(lnRow);
                        for(int i = 0; i < lnRow; ++i)
                        {
                            lvRowMap[i] = lStartRow + i;
                        }
                        
                        hr = hInitKeyValue(lpMainIndex, inUnits, ipUnits, lnRow, &lvRowMap[0], lpbuffer);
                        CHECK_HR
                        
                        //write(lpbuffer, lnrow*4);
                        int lSize = lnRow * inUnits * sizeof(int);
                        hr = ipCMNodeModel->AppendBlockData(1, &lSize, (const char**)(&lpbuffer));
                        CHECK_HR;
                        
                        lnRemainRowCnt -= lnRow;
                        lStartRow += lnRow;
                    }
                }
                else
                {
                    hr = hInitKeyValue(lpMainIndex, inUnits, ipUnits, inRows, NULL, iopKeys);
                    CHECK_HR;
                }
            }
            else
            {
                assert(false);
            }
            break;
        case EIndexOptTypeRowMapShort:
            if (ipIndexPool)
            {
                DSSTabularIndex *lpMainIndex = NULL;
                hr = ipIndexPool->getMainIndex(&lpMainIndex);
                CHECK_HR;
                
                if(!lpMainIndex)
                {
                    assert(false);
                }
                
                if(lbUseCacheManager)
                {
                    int lShortArrayLen;
                    hr = ipReadStream->ReadInt(&lShortArrayLen);
                    CHECK_HR;
                    
                    assert(lShortArrayLen == inRows);
                    
                    int lnRemainRowCnt = inRows;
                    while (lnRemainRowCnt > 0)
                    {
                        int lnRow = min(lnRemainRowCnt, lnMaxRowCanWrite);
                        
                        vector<short> lvShortRowMap(lnRow);
                        hr = ipReadStream->ReadShortArray(&lvShortRowMap[0], lnRow);
                        CHECK_HR;
                        
                        vector<int> lvRowMap(lnRow);
                        for(int i = 0; i < lnRow; ++i)
                            lvRowMap[i] = lvShortRowMap[i];
                        
                        hr = hInitKeyValue(lpMainIndex, inUnits, ipUnits, lnRow, &lvRowMap[0], lpbuffer);
                        CHECK_HR
                        
                        int lSize = lnRow * inUnits * sizeof(int);
                        hr = ipCMNodeModel->AppendBlockData(1, &lSize, (const char**)(&lpbuffer));
                        CHECK_HR;
                        
                        lnRemainRowCnt -= lnRow;
                    }
                }
                else
                {
                    vector<int> lvRowMap;
                    hr = hLoadShortArray(ipReadStream, lvRowMap);
                    CHECK_HR;
                    
                    hr = hInitKeyValue(lpMainIndex, inUnits, ipUnits, inRows, &(lvRowMap[0]), iopKeys);
                    CHECK_HR;
                }
            }
            else
            {
                assert(false);
            }
            break;
            
        case EIndexOptTypeRowMapInt:
            if (ipIndexPool)
            {
                DSSTabularIndex *lpMainIndex = NULL;
                hr = ipIndexPool->getMainIndex(&lpMainIndex);
                CHECK_HR;
                
                if(!lpMainIndex)
                {
                    assert(false);
                }
                
                if(lbUseCacheManager)
                {
                    int lRowMapLen;
                    hr = ipReadStream->ReadInt(&lRowMapLen);
                    CHECK_HR;
                    
                    assert(lRowMapLen == inRows);
                    
                    int lnRemainRowCnt = inRows;
                    while (lnRemainRowCnt > 0) {
                        int lnRow = min(lnRemainRowCnt, lnMaxRowCanWrite);
                        
                        vector<int> lvRowMap(lnRow);
                        for(int i = 0; i < lnRow; ++i)
                        {
                            hr = ipReadStream->ReadInt(&lvRowMap[i]);
                        }
                        
                        hr = hInitKeyValue(lpMainIndex, inUnits, ipUnits, lnRow, &lvRowMap[0], lpbuffer);
                        CHECK_HR
                        
                        int lSize = lnRow * inUnits * sizeof(int);
                        hr = ipCMNodeModel->AppendBlockData(1, &lSize, (const char**)(&lpbuffer));
                        CHECK_HR;
                        
                        lnRemainRowCnt -= lnRow;
                    }
                }
                else
                {
                    vector<int> lvRowMap;
                    hr = ipReadStream->ReadIntArray(lvRowMap);
                    
                    hr = hInitKeyValue(lpMainIndex, inUnits, ipUnits, inRows, &(lvRowMap[0]), iopKeys);
                    CHECK_HR;
                }
            }
            else
            {
                assert(false);
            }
            break;
        case EIndexOptTypeRowMapCompressedShort:
            if (ipIndexPool)
            {
                DSSTabularIndex *lpMainIndex = NULL;
                hr = ipIndexPool->getMainIndex(&lpMainIndex);
                CHECK_HR;
                
                if(!lpMainIndex)
                {
                    assert(false);
                }
                
                if(lbUseCacheManager)
                {
                    int lShortArrayLen;
                    hr = ipReadStream->ReadInt(&lShortArrayLen);
                    CHECK_HR;
                    
                    int lStart = -1, lStepSize = -1, lStepNum = -1;
                    int lnRemainRowCnt = inRows;
                    while (lnRemainRowCnt > 0)
                    {
                        int lnRow = min(lnRemainRowCnt, lnMaxRowCanWrite);
                        int lnRowsNeedToDeCompress = lnRow;
                        vector<int> lvRowMap(lnRow);
                        int lRowMapIdx = 0;
                        while(lnRowsNeedToDeCompress > 0)
                        {
                            if(lStart == -1)
                            {
                                short lShort;
                                hr = ipReadStream->ReadShort(&lShort);
                                CHECK_HR;
                                lStart = lShort;
                                hr = ipReadStream->ReadShort(&lShort);
                                CHECK_HR;
                                lStepSize = lShort;
                                hr = ipReadStream->ReadShort(&lShort);
                                CHECK_HR;
                                lStepNum = lShort;
                            }
                            
                            int lnRowsCanDeCompress = min(lnRowsNeedToDeCompress, lStepNum + 1);
                            for(int i = 0; i < lnRowsCanDeCompress; ++i)
                            {
                                lvRowMap[lRowMapIdx++] = lStart + lStepSize * i;
                            }
                            
                            if(lnRowsCanDeCompress == lStepNum + 1)
                            {
                                lStart = -1;
                                lStepSize = -1;
                                lStepNum = -1;
                            }
                            else
                            {
                                lStart = lStart + lStepSize * lnRowsCanDeCompress;
                                lStepNum -= lnRowsCanDeCompress;
                            }
                            
                            lnRowsNeedToDeCompress -= lnRowsCanDeCompress;
                        }
                        
                        hr = hInitKeyValue(lpMainIndex, inUnits, ipUnits, lnRow, &lvRowMap[0], lpbuffer);
                        CHECK_HR;
                        
                        int lSize = lnRow * inUnits * sizeof(int);
                        hr = ipCMNodeModel->AppendBlockData(1, &lSize, (const char**)(&lpbuffer));
                        CHECK_HR;
                        
                        lnRemainRowCnt -= lnRow;
                    }
                }
                else
                {
                    vector<int> lvCompressedRowMap;
                    vector<int> lvRowMap;
                    hr = hLoadShortArray(ipReadStream, lvCompressedRowMap);
                    CHECK_HR;
                    // 8/27/2012 tehe:optimize decompress range
                    lvRowMap.resize(inRows);
                    hr = hDecompressRange(lvCompressedRowMap, lvRowMap);
                    CHECK_HR;
                    
                    hr = hInitKeyValue(lpMainIndex, inUnits, ipUnits, inRows, &(lvRowMap[0]), iopKeys);
                    CHECK_HR;
                }
            }
            else
            {
                assert(false);
            }
            break;
        case EIndexOptTypeRowMapCompressedInt:
            if (ipIndexPool)
            {
                DSSTabularIndex *lpMainIndex = NULL;
                hr = ipIndexPool->getMainIndex(&lpMainIndex);
                CHECK_HR;
                
                if(!lpMainIndex)
                {
                    assert(false);
                }
                
                if(lbUseCacheManager)
                {
                    int lIntArrayLen;
                    hr = ipReadStream->ReadInt(&lIntArrayLen);
                    CHECK_HR;
                    
                    int lStart = -1, lStepSize = -1, lStepNum = -1;
                    int lnRemainRowCnt = inRows;
                    while (lnRemainRowCnt > 0)
                    {
                        int lnRow = min(lnRemainRowCnt, lnMaxRowCanWrite);
                        int lnRowsNeedToDeCompress = lnRow;
                        vector<int> lvRowMap(lnRow);
                        int lRowMapIdx = 0;
                        while(lnRowsNeedToDeCompress > 0)
                        {
                            if(lStart == -1)
                            {
                                hr = ipReadStream->ReadInt(&lStart);
                                CHECK_HR;
                                hr = ipReadStream->ReadInt(&lStepSize);
                                CHECK_HR;
                                hr = ipReadStream->ReadInt(&lStepNum);
                                CHECK_HR;
                            }
                            
                            int lnRowsCanDeCompress = min(lnRowsNeedToDeCompress, lStepNum + 1);
                            for(int i = 0; i < lnRowsCanDeCompress; ++i)
                            {
                                lvRowMap[lRowMapIdx++] = lStart + lStepSize * i;
                            }
                            
                            if(lnRowsCanDeCompress == lStepNum + 1)
                            {
                                lStart = -1;
                                lStepSize = -1;
                                lStepNum = -1;
                            }
                            else
                            {
                                lStart = lStart + lStepSize * lnRowsCanDeCompress;
                                lStepNum -= lnRowsCanDeCompress;
                            }
                            
                            lnRowsNeedToDeCompress -= lnRowsCanDeCompress;
                        }
                        
                        hr = hInitKeyValue(lpMainIndex, inUnits, ipUnits, lnRow, &lvRowMap[0], lpbuffer);
                        CHECK_HR;
                        
                        int lSize = lnRow * inUnits * sizeof(int);
                        hr = ipCMNodeModel->AppendBlockData(1, &lSize, (const char**)(&lpbuffer));
                        CHECK_HR;
                        
                        lnRemainRowCnt -= lnRow;
                    }
                }
                else
                {
                    vector<int> lvCompressedRowMap;
                    vector<int> lvRowMap;
                    
                    //zhyang, 758892
                    //hr = hLoadShortArray(ipReadStream, lvCompressedRowMap);
                    hr = ipReadStream->ReadIntArray(lvCompressedRowMap);
                    CHECK_HR;
                    
                    // 8/27/2012 tehe:optimize decompress range
                    lvRowMap.resize(inRows);
                    hr = hDecompressRange(lvCompressedRowMap, lvRowMap);
                    CHECK_HR;
                    
                    hr = hInitKeyValue(lpMainIndex, inUnits, ipUnits, inRows, &(lvRowMap[0]), iopKeys);
                    CHECK_HR;
                }
                
                /*
                 */
                
            }
            else
            {
                assert(false);
            }
            break;
        case EIndexOptTypeCompressed:
            for(int iUnit = 0; iUnit < inUnits; ++iUnit)
            {
                vector<int> lvUnitCompressedKey;
                DSSXTabBitVector lCompressedBitVector;
                
                hr = ipReadStream->ReadIntArray(lvUnitCompressedKey);
                CHECK_HR;
                
                lCompressedBitVector.Init(2 * lvUnitCompressedKey.size() , false);
                
                hr = lCompressedBitVector.LoadFromBinary(ipReadStream);
                CHECK_HR;
                
                int lCurrentPos = 0;
                hr = hDecompressUnitKeys(iopKeys, lCurrentPos, inUnits, iUnit, 0, lvUnitCompressedKey.size(), lvUnitCompressedKey, lCompressedBitVector);
                CHECK_HR;
            }
            break;
        case EIndexOptTypeOriginal:
        case EIndexOptTypeNoOrderShort:
        case EIndexOptTypeNoOrderInt:
        {
            if(lbUseCacheManager)
            {
                vector<ColIndexKeyInfo> lvColIndexKeyInfo;
                //int lnMaxRow = BUFFERSIZE / (inUnits * 4);
                
                for(int iUnit = 0; iUnit < inUnits; ++iUnit)
                {
                    ColIndexKeyInfo lColKeyInfo;
                    
                    int lUnitOptType = 0;
                    hr = ipReadStream->ReadInt(&lUnitOptType);
                    CHECK_HR;
                    
                    int len;
                    hr = ipReadStream->ReadInt(&len);
                    CHECK_HR;
                    
                    lColKeyInfo.optType = (EIndexOptType)lUnitOptType;
                    lColKeyInfo.curpos = ipReadStream->GetPosition();
                    lColKeyInfo.start = -1;
                    lColKeyInfo.stepNum = -1;
                    lColKeyInfo.stepSize = -1;
                    
                    if(lColKeyInfo.optType == EIndexOptTypeColCompressedShort || lColKeyInfo.optType == EIndexOptTypeColShort)
                    {
                        hr = ipReadStream->Skip(len * sizeof(short));
                        CHECK_HR;
                    }
                    else if(lColKeyInfo.optType == EIndexOptTypeColCompressedInt || lColKeyInfo.optType == EIndexOptTypeColInt)
                    {
                        hr = ipReadStream->Skip(len * sizeof(int));
                        CHECK_HR;
                    }
                    else
                    {
                        assert(false);
                    }
                    lvColIndexKeyInfo.push_back(lColKeyInfo);
                }
                
                int remainRowCnt = inRows;
                //int lnMaxRowCanWrite = BUFFERSIZE / (inUnits * 4);
                while(remainRowCnt > 0)
                {
                    int lnRow = min(remainRowCnt, lnMaxRowCanWrite);
                    for(int iUnit = 0; iUnit < inUnits; ++iUnit)
                    {
                        hr = ipReadStream->SetPosition(lvColIndexKeyInfo[iUnit].curpos);
                        switch (lvColIndexKeyInfo[iUnit].optType) {
                            case EIndexOptTypeColShort:
                            {
                                vector<short> lvShort;
                                lvShort.resize(lnRow);
                                hr = ipReadStream->ReadShortArray(&lvShort[0], lnRow);
                                for (int i = 0; i < lnRow; ++i)
                                {
                                    lpbuffer[inUnits * i + iUnit] = lvShort[i];
                                }
                                lvColIndexKeyInfo[iUnit].curpos = ipReadStream->GetPosition();
                            }
                                break;
                            case EIndexOptTypeColInt:
                            {
                                for(int i = 0; i < lnRow; ++i)
                                {
                                    hr = ipReadStream->ReadInt(&lpbuffer[inUnits * i + iUnit]);
                                    CHECK_HR;
                                }
                                lvColIndexKeyInfo[iUnit].curpos = ipReadStream->GetPosition();
                            }
                                break;
                            case EIndexOptTypeColCompressedShort:
                            {
                                int lnRowToDecompress = lnRow;
                                int lRowidx = 0;
                                while(lnRowToDecompress != 0)
                                {
                                    if(lvColIndexKeyInfo[iUnit].start == -1)
                                    {
                                        short lShort;
                                        hr = ipReadStream->ReadShort(&lShort);
                                        CHECK_HR;
                                        lvColIndexKeyInfo[iUnit].start = lShort;
                                        hr = ipReadStream->ReadShort(&lShort);
                                        CHECK_HR;
                                        lvColIndexKeyInfo[iUnit].stepSize = lShort;
                                        hr = ipReadStream->ReadShort(&lShort);
                                        CHECK_HR;
                                        lvColIndexKeyInfo[iUnit].stepNum = lShort;
                                        lvColIndexKeyInfo[iUnit].curpos = ipReadStream->GetPosition();
                                    }
                                    
                                    //it remains some after decompress last time.
                                    int lnRowCanDecompress = min(lnRowToDecompress, lvColIndexKeyInfo[iUnit].stepNum+1);
                                    for(int i = 0; i < lnRowCanDecompress; ++i)
                                    {
                                        lpbuffer[inUnits * lRowidx + iUnit] = lvColIndexKeyInfo[iUnit].start + lvColIndexKeyInfo[iUnit].stepSize * i;
                                        lRowidx++;
                                    }
                                    //all keys are decompressed
                                    if(lnRowCanDecompress == lvColIndexKeyInfo[iUnit].stepNum + 1)
                                    {
                                        lvColIndexKeyInfo[iUnit].start = -1;
                                        lvColIndexKeyInfo[iUnit].stepNum = -1;
                                        lvColIndexKeyInfo[iUnit].stepSize = -1;
                                    }
                                    else
                                    {
                                        //still remains something
                                        lvColIndexKeyInfo[iUnit].start = lvColIndexKeyInfo[iUnit].start + lvColIndexKeyInfo[iUnit].stepSize * lnRowCanDecompress;
                                        lvColIndexKeyInfo[iUnit].stepNum -= lnRowCanDecompress;
                                    }
                                    lnRowToDecompress -= lnRowCanDecompress;
                                }
                            }
                                break;
                            case EIndexOptTypeColCompressedInt:
                            {
                                int lnRowToDecompress = lnRow;
                                int lRowidx = 0;
                                while(lnRowToDecompress != 0)
                                {
                                    if(lvColIndexKeyInfo[iUnit].start == -1)
                                    {
                                        hr = ipReadStream->ReadInt(&lvColIndexKeyInfo[iUnit].start);
                                        CHECK_HR;
                                        hr = ipReadStream->ReadInt(&lvColIndexKeyInfo[iUnit].stepSize);
                                        CHECK_HR;
                                        hr = ipReadStream->ReadInt(&lvColIndexKeyInfo[iUnit].stepNum);
                                        CHECK_HR;
                                        lvColIndexKeyInfo[iUnit].curpos = ipReadStream->GetPosition();
                                    }
                                    
                                    //it remains some after decompress last time.
                                    int lnRowCanDecompress = min(lnRowToDecompress, lvColIndexKeyInfo[iUnit].stepNum+1);
                                    for(int i = 0; i < lnRowCanDecompress; ++i)
                                    {
                                        lpbuffer[inUnits * lRowidx + iUnit] = lvColIndexKeyInfo[iUnit].start + lvColIndexKeyInfo[iUnit].stepSize * i;
                                        lRowidx++;
                                    }
                                    //all keys are decompressed
                                    if(lnRowCanDecompress == lvColIndexKeyInfo[iUnit].stepNum + 1)
                                    {
                                        lvColIndexKeyInfo[iUnit].start = -1;
                                        lvColIndexKeyInfo[iUnit].stepNum = -1;
                                        lvColIndexKeyInfo[iUnit].stepSize = -1;
                                    }
                                    else
                                    {
                                        //still remains something
                                        lvColIndexKeyInfo[iUnit].start = lvColIndexKeyInfo[iUnit].start + lvColIndexKeyInfo[iUnit].stepSize * lnRowCanDecompress;
                                        lvColIndexKeyInfo[iUnit].stepNum -= lnRowCanDecompress;
                                    }
                                    lnRowToDecompress -= lnRowCanDecompress;
                                }
                            }
                                break;
                            default:
                            {
                                assert(false);
                            }
                                break;
                        }
                    }
                    remainRowCnt -= lnRow;
                    
                    int lSize = lnRow * inUnits * sizeof(int);
                    hr = ipCMNodeModel->AppendBlockData(1, &lSize, (const char**)(&lpbuffer));
                    CHECK_HR;
                }
            }
            else
            {
                
                for(int iUnit = 0; iUnit < inUnits; ++iUnit)
                {
                    int lUnitOptType = 0;
                    hr = ipReadStream->ReadInt(&lUnitOptType);
                    CHECK_HR;
                    
                    switch (lUnitOptType) {
                        case EIndexOptTypeColShort:
                        {
                            int lLength = -1;
                            hr = ipReadStream->ReadInt(&lLength);
                            CHECK_HR;
                            
                            assert(lLength == inRows);
                            
                            // 8/21/2012 tehe: performance
                            if(lLength > 0)
                            {
                                vector<short> lvValue(lLength);
                                
                                hr = ipReadStream->ReadShortArray(&(lvValue[0]), lLength);
                                CHECK_HR;
                                
                                for (int i= 0; i< lLength; ++i)
                                {
                                    int lPos = (lvIndexOrder.size() > 0)? lvIndexOrder[i] : i;
                                    
                                    iopKeys[lPos * inUnits + iUnit] = lvValue[i];
                                }
                                
                            }
                        }
                            break;
                        case EIndexOptTypeColInt:
                        {
                            vector<int> lValues;
                            hr = ipReadStream->ReadIntArray(lValues);
                            CHECK_HR;
                            
                            assert(lValues.size() == inRows);
                            
                            for(int i = 0; i < lValues.size(); ++i)
                            {
                                int lPos = (lvIndexOrder.size() > 0)? lvIndexOrder[i] : i;
                                iopKeys[lPos * inUnits + iUnit] = lValues[i];
                            }
                        }
                            break;
                        case EIndexOptTypeColCompressedShort:
                        {
                            vector<int> lvCompressedKeys;
                            vector<int> lvKeys;
                            
                            hr = hLoadShortArray(ipReadStream, lvCompressedKeys);
                            CHECK_HR;
                            
                            // 8/27/2012 tehe:optimize decompress range
                            lvKeys.resize(inRows);
                            
                            hr = hDecompressRange(lvCompressedKeys, lvKeys);
                            CHECK_HR;
                            
                            
                            assert(lvKeys.size() == inRows);
                            
                            for(int i = 0; i < inRows; ++i)
                            {
                                int lPos = (lvIndexOrder.size() > 0)? lvIndexOrder[i] : i;
                                iopKeys[lPos * inUnits + iUnit] = lvKeys[i];
                            }
                        }
                            break;
                        case EIndexOptTypeColCompressedInt:
                        {
                            vector<int> lvCompressedKeys;
                            vector<int> lvKeys;
                            hr = ipReadStream->ReadIntArray(lvCompressedKeys);
                            CHECK_HR;
                            
                            // 8/27/2012 tehe:optimize decompress range
                            lvKeys.resize(inRows);
                            hr = hDecompressRange(lvCompressedKeys, lvKeys);
                            CHECK_HR;
                            
                            assert(lvKeys.size() == inRows);
                            
                            for(int i = 0; i < inRows; ++i)
                            {
                                int lPos = (lvIndexOrder.size() > 0)? lvIndexOrder[i] : i;
                                iopKeys[lPos * inUnits + iUnit] = lvKeys[i];
                            }
                        }
                            break;
                        default:
                            assert(false);
                            break;
                    }
                }
            }
            
            break;
        }
        default:
            assert(false);
            break;
    }
    
    if(lbUseCacheManager && lpbuffer)
    {
        //yimliu, 2014/06/25, 918611, Memory Governor. Give up calling MEM_GOVERNOR_RELEASE when it deconstructs.
        MGovernorHelper.GiveUpRelease();
        MEM_GOVERNOR_RELEASE(BUFFERSIZE, BUFFERSIZE);
    }
    
    return S_OK;
}
