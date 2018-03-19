//
//  DSSCMNodeModelUtil.h
//  ReportServiceCore
//
//  Created by He Tengfei on 1/30/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSNodeModelCacheManagerUtil_h
#define ReportServiceCore_DSSNodeModelCacheManagerUtil_h

#include "DSSAEHelper.h"

class DSSBinaryDataReader;

class DSSCMBlockFileUtil
{
public:
    
    static int CreateFile(const char * ipFilePath);
    
    static bool FileExistsAtPath(const char* ipFilePath);
    
    static int GetBlockData(const char * ipFilePath, EnumCMBlockType iBlockType,  char **oppData, int *opSize);
    
    // tehe: iFlag = 0, means compare create time, 1 means modify time, now we support compare create time on ios, but only support compare modify time on android
    static int CompareFileDate(const char* ipSourceFilePath, const char* ipTargetFilePath, int iFlag, bool &oEqual);
    
    // tehe : oBinaryOffset means the offset of block data, but not the offset of block header. It's good for caller to initialize oBlockHeader. we may not set value of oBlockHeader if error happens.
    static int GetBlockHeader(const char* ipFilePath,EnumCMBlockType iBlockType, CMBlockHeader & oBlockHeader);
    
    static int CompareBinary(DSSBinaryDataReader *ipReadStreamCurrent, DSSBinaryDataReader *ipReadStreamOld, int iBinarySize, bool &obBinaryEqual);
    
    // tehe: AppendNewBlock in this class will not judge whether the block already exists
    static int AppendNewBlock(const char* ipFilePath, EnumCMBlockType iBlockType, int iCount, const int *iDataSize, const char** ippData);
    
    static int AppendNewBlock(const char* ipFilePath, EnumCMBlockType lBlockType, DSSBinaryDataReader * ipReadStream,int iBlockSize);
    
    static int UpdateDocInfo(const char* ipFilePath, EnumCMBlockType lBlockType, const char *ipDocInfo, int iSize);
    
    static int AddBlockHeader(const char* ipFilePath,EnumCMBlockType iBlockType, int iBlockSize, CMBlockHeader *iopBlockHeader = NULL);
    
    static int AppendBlockData(const char* ipFilePath, int iCount, const int *iDataSize, const char** ippData);
    
    static int RemoveFile(const char * ipFilePath);
};

#endif
