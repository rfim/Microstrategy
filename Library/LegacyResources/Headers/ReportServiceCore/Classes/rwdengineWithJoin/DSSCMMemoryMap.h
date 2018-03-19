//
//  IDSSCMMemoryMap.h
//  ReportServiceCore
//
//  Created by He Tengfei on 7/1/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_IDSSCMMemoryMap_h
#define ReportServiceCore_IDSSCMMemoryMap_h

#include "DSSAEHelper.h"

class DSSCMMemoryMap
{
public:
    DSSCMMemoryMap();
    virtual ~DSSCMMemoryMap();
    
    virtual int createMMapMemory(const char * ipFilePath, bool ibUseNewFile = false) = 0;
    
    virtual int GetRawSize() = 0;
    
    virtual int releaseMMapMemory();
    virtual int reloadMMapMemory();

    bool GetUseMappedFile() { return mbUseMappedFile; };
    
    void SetUseMappedFile(bool ibUseMappedFile) { mbUseMappedFile = ibUseMappedFile; }
    
    bool NeedDoMMap(const char *ipFilePath);
    
    int GetMMPBufSize() { return mMMPBufSize; }
    
    int GetOffset() { return mOffset; }
    
    int GetOffsetAligned() { return mOffsetAligned; }
    
    void SetMMPBufSize(int iMMpBufSize) { mMMPBufSize = iMMpBufSize; }
    
    void SetOffset(int iOffset) { mOffset = iOffset; }
    
    void SetOffsetAligned(int iOffsetAligned) { mOffsetAligned = iOffsetAligned; }
    
    void SetFilePath(const char* ipFilePath) { mFilePath = ipFilePath; }
    
    int MMap(const char *ipFilePath,int iFileOffset, int iBinarySize);
protected:
    
    std::string mFilePath;
    bool mbUseMappedFile;
    char * mpMMPBuffer;
    int mMMPBufSize;
    int mOffset;
    int mOffsetAligned;
    
    EnumCMBlockType mBlockType;
};

#endif
