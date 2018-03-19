//
//  DSSCMBlockFileUtil.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 1/30/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//
#ifdef uuid_t
#undef uuid_t
#endif
#include <unistd.h>
#include "DSSCMBlockFileUtil.h"

#include "DSSDataSource.h"
#import "DSSBinaryDataReader.h"

int DSSCMBlockFileUtil::CreateFile(const char * ipFilePath)
{
    CHECK_PTR(ipFilePath);
    
    int hr = S_OK;

    MBase::CString lDictionaryPath = ipFilePath;
    
    int pos = lDictionaryPath.find_last_of('/');
    
    if(pos == string::npos)
        return E_FAIL;
    
    lDictionaryPath = lDictionaryPath.substr(0,pos);

    if(!FileExistsAtPath(lDictionaryPath.c_str()))
    {
        hr = DataSourceUtil::createFolder(lDictionaryPath);
        if(hr != 0)
        	return hr;
    }

    FILE * stream = fopen(ipFilePath, "w+b");
    if (stream == NULL)
    {
        //can't open file to read and write
        return E_READFILE;
    }

    fclose(stream);
    
    return hr;
    
}

bool DSSCMBlockFileUtil::FileExistsAtPath(const char * ipFilePath)
{
    int ret = access(ipFilePath, F_OK);
    
    if(ret == 0)
        return true;
    
    return false;
}

int DSSCMBlockFileUtil::GetBlockData(const char * ipFilePath, EnumCMBlockType iBlockType,  char **oppData, int *opSize)
{
    if(oppData == NULL || opSize == NULL)
        return E_POINTER;
    
    CMBlockHeader lBlockHeader;
    int hr = GetBlockHeader(ipFilePath, iBlockType, lBlockHeader);
    if(hr != S_OK && hr != S_FALSE)
        return hr;
    
    // tehe: we can't find the block
    if(hr == S_FALSE)
    {
        *oppData = NULL;
        *opSize = 0;
        
        return S_OK;
    }

    FILE *stream = fopen(ipFilePath, "rb");
    if(stream == NULL)
        return E_READFILE;
    
    fseek(stream, lBlockHeader.mBlockOffset, SEEK_SET);
    
    char *lpData = new char[lBlockHeader.mBlockSize];
    
    fread(lpData,sizeof(char),lBlockHeader.mBlockSize,stream);
    
    fclose(stream);
    
    *oppData = lpData;
    
    *opSize = lBlockHeader.mBlockSize;
    
    return S_OK;
}

int DSSCMBlockFileUtil::GetBlockHeader(const char* ipFilePath,EnumCMBlockType iBlockType, CMBlockHeader & oBlockHeader)
{
    FILE *stream = NULL;
    if( (stream  = fopen(ipFilePath, "rb" )) == NULL )
    {
        return E_READFILE;
    }
    
    int hr = S_OK;
    CMBlockHeader lBlockHeader;
    lBlockHeader.mBlockType = cmBlockTypeUnknow;
    
    do {
        int numread = fread((char*)&lBlockHeader,sizeof(CMBlockHeader),1,stream);
        if(numread != 1)
        {
            if(!feof(stream))
            {
                hr = E_READFILE;
            }
            break;
        }
        
        if(lBlockHeader.mBlockType == iBlockType)
        {
            break;
        }
        
        fseek(stream, lBlockHeader.mBlockSize, SEEK_CUR);
        
    } while (!feof(stream));
    
    fclose(stream);
    
    // tehe: check read error
    CHECK_HR; 

    // tehe: can't find the block
    if(lBlockHeader.mBlockType != iBlockType)
    {
        oBlockHeader.mBlockOffset = -1;
        oBlockHeader.mBlockSize = 0;
        oBlockHeader.mBlockType = cmBlockTypeUnknow;
        
        return S_FALSE;
    }        

    if(lBlockHeader.mBlockSize <= 0 || lBlockHeader.mBlockOffset == -1)
    {
        return E_UNEXPECTED;
    }
    
    oBlockHeader.mBlockOffset = lBlockHeader.mBlockOffset;
    oBlockHeader.mBlockSize = lBlockHeader.mBlockSize;
    oBlockHeader.mBlockType = lBlockHeader.mBlockType;
    
    return S_OK;
}

int DSSCMBlockFileUtil::CompareFileDate(const char* ipSourceFilePath, const char* ipTargetFilePath,int iFlag, bool &oEqual)
{
    char *lpBlockData = NULL;
    int lSize = 0;
    int hr = DSSCMBlockFileUtil::GetBlockData(ipTargetFilePath, cmDocumentInfo, &lpBlockData, &lSize);
    MBase::StrongPtr<char, MBase::DeleteArray<char> > lSPData(lpBlockData);
    CHECK_HR;
    
    // can't find the block
    if(lpBlockData == NULL && lSize == 0)
        return false;
    
    Int64 *lTargetTime = reinterpret_cast<Int64 *>(lpBlockData);
    
    Int64 lSourceTime = 0;
    
    DataSourceUtil::GetTimeIntervalSince1970(ipSourceFilePath, iFlag, lSourceTime);
    
    if(lSourceTime == *lTargetTime)
        oEqual = true;
    else
        oEqual = false;

    return S_OK;
}


int DSSCMBlockFileUtil::CompareBinary(DSSBinaryDataReader *ipReadStreamCurrent, DSSBinaryDataReader *ipReadStreamOld, int iBinarySize, bool &obBinaryEqual)
{
    int hr = S_OK;
    
    bool lbEqual = true;
    
    obBinaryEqual = false;
    
    char *lpCurBuffer = new char[MEM_GOVERNOR_SIZE];
    CHECK_NEWED_PTR(lpCurBuffer);
    MBase::StrongPtr<char> lSP(lpCurBuffer);
    
    char *lpOldBuffer = new char[MEM_GOVERNOR_SIZE];
    CHECK_NEWED_PTR(lpOldBuffer);
    MBase::StrongPtr<char> lSP2(lpOldBuffer);
    
    int ret = 0;
    int lBlockNum = iBinarySize / MEM_GOVERNOR_SIZE;
    
    for(int i=0;i<lBlockNum;i++)
    {
        hr = ipReadStreamCurrent->ReadByteArray(lpCurBuffer, MEM_GOVERNOR_SIZE);
        CHECK_HR;
        
        hr = ipReadStreamOld->ReadByteArray(lpOldBuffer, MEM_GOVERNOR_SIZE);
        CHECK_HR;
        
        ret = memcmp(lpCurBuffer, lpOldBuffer, MEM_GOVERNOR_SIZE);
        if(ret != 0)
        {
            lbEqual = false;
            break;
        }
    }
    
    if(lbEqual)
    {
        int remainBytes = iBinarySize % MEM_GOVERNOR_SIZE;
        if(remainBytes == 0)
        {
            lbEqual = true;
        }
        else
        {
            hr = ipReadStreamCurrent->ReadByteArray(lpCurBuffer, remainBytes);
            CHECK_HR;
            
            hr = ipReadStreamOld->ReadByteArray(lpOldBuffer, remainBytes);
            CHECK_HR;
            
            ret = memcmp(lpCurBuffer, lpOldBuffer, remainBytes);
            if(ret != 0)
            {
                lbEqual = false;
            }
            else
            {
                lbEqual = true;
            }
        }
    }
    
    obBinaryEqual = lbEqual;
    
    return hr;
}

int DSSCMBlockFileUtil::AppendNewBlock(const char* ipFilePath, EnumCMBlockType iBlockType, int iCount, const int *iDataSize, const char** ippData)
{
    long lBlockSize = 0;
    for(int i = 0; i < iCount; i++)
    {
        lBlockSize += iDataSize[i];
    }
    
    int hr = AddBlockHeader(ipFilePath, iBlockType, lBlockSize);
    CHECK_HR;
    
    hr = AppendBlockData(ipFilePath, iCount, iDataSize, ippData);
    CHECK_HR;
    
    return S_OK;
}

int DSSCMBlockFileUtil::AppendNewBlock(const char* ipFilePath, EnumCMBlockType iBlockType, DSSBinaryDataReader * ipReadStream,int iBlockSize)
{
    int hr = S_OK;
    
    hr = AddBlockHeader(ipFilePath, iBlockType, iBlockSize);
    CHECK_HR;
    
    FILE *stream = NULL;
	if( (stream  = fopen( ipFilePath, "r+b" )) == NULL )
    {
        return E_READFILE;
    }
    
    fseek(stream, 0, SEEK_END);
    
	char *lpBuffer = new char[MEM_GOVERNOR_SIZE];
    CHECK_NEWED_PTR(lpBuffer);
    MBase::StrongPtr<char> lSP(lpBuffer);
    
    int nBlocks = iBlockSize / MEM_GOVERNOR_SIZE;
    
    for(int i=0; i<nBlocks;i++)
    {
        hr = ipReadStream->ReadByteArray(lpBuffer, MEM_GOVERNOR_SIZE);
        CHECK_HR;
        
        fwrite(lpBuffer, 1, MEM_GOVERNOR_SIZE, stream);
    }
    
    int remainData = iBlockSize % MEM_GOVERNOR_SIZE;
    hr = ipReadStream->ReadByteArray(lpBuffer, remainData);
    CHECK_HR;
    
    fwrite(lpBuffer, 1, remainData, stream);
    
    fclose(stream);
    
    return S_OK;
}

int DSSCMBlockFileUtil::UpdateDocInfo(const char* ipFilePath, EnumCMBlockType iBlockType, const char *ipDocInfo, int iSize)
{
    _ASSERT(iBlockType == cmDocumentInfo);
    
    CMBlockHeader lBlockHeader;

    int hr = GetBlockHeader(ipFilePath, iBlockType, lBlockHeader);
    CHECK_HR;
    
    FILE *stream = fopen(ipFilePath, "r+b");
    if(stream == NULL)
        return E_READFILE;
    
    fseek(stream, lBlockHeader.mBlockOffset, SEEK_SET);
    
    fwrite(ipDocInfo,sizeof(char),iSize,stream);
    
    fclose(stream);
    
    return S_OK;
}

int DSSCMBlockFileUtil::AddBlockHeader(const char *ipFilePath, EnumCMBlockType iBlockType, int iBlockSize, CMBlockHeader *iopBlockHeader)
{
    FILE *stream = NULL;
    if( (stream  = fopen(ipFilePath, "r+b" )) == NULL )
    {
        return E_READFILE;
    }
    
    CMBlockHeader lBlockHeader;
    lBlockHeader.mBlockType = iBlockType;
    lBlockHeader.mBlockSize = iBlockSize;
    lBlockHeader.mBlockOffset = -1;
    
    fseek(stream, 0, SEEK_END);
    
    long lBlockHeaderOffset = ftell(stream);
    
    lBlockHeader.mBlockOffset = lBlockHeaderOffset + sizeof(CMBlockHeader);
    
    int ret = fwrite((char*)(&lBlockHeader), sizeof(CMBlockHeader), 1, stream);
    if (ret != 1)
        return E_FAIL;
    
    fclose(stream);
    
    if(iopBlockHeader)
    {
        iopBlockHeader->mBlockType = iBlockType;
        iopBlockHeader->mBlockSize = iBlockSize;
        iopBlockHeader->mBlockOffset = lBlockHeader.mBlockOffset;
    }
    return S_OK;
}

int DSSCMBlockFileUtil::RemoveFile(const char *ipFilePath)
{
    remove(ipFilePath);
    
    return S_OK;
}

int DSSCMBlockFileUtil::AppendBlockData(const char *ipFilePath, int iCount, const int *iDataSize, const char **ippData)
{
    FILE *stream = NULL;
    if( (stream  = fopen(ipFilePath, "r+b" )) == NULL )
    {
        return E_READFILE;
    }
    
    fseek(stream, 0, SEEK_END);
    
    for(int i = 0; i < iCount; i++)
    {
        fwrite(ippData[i], sizeof(char), iDataSize[i], stream);
    }
    
    fclose(stream);
    
    return S_OK;
}

