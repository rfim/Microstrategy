//
//  DSSCMNodeModel.h
//  ReportServiceCore
//
//  Created by He Tengfei on 12/26/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSNodeModelCacheManager_h
#define ReportServiceCore_DSSNodeModelCacheManager_h

class DSSRWNodeModel;
class DSSBinaryDataReader;
class CMIndexPoolInfo;
class DSSTabularIndex;
class DSSCMMemoryMap;

#include "DSSAEHelper.h"

typedef struct CMIndexInfo
{
    CMIndexInfo();
    int mnUnits;
    vector<int> mvUnits;
    int mnRowCount;
    int mMMapInfo[6]; // Index{mOffset, mOffsetAligned, mMMPBufSize}, Hashtable{mOffset, mOffsetAligned, mMMPBufSize}

    bool Equal(CMIndexInfo *ipIndexInfo);
    
    int Serialize(vector<char>& oPoolInfo);
    
    int Deserialize(const char *iPoolInfo, int iDataSize);
    
}CMIndexInfo;


class CMIndexPoolInfo
{
public:
    CMIndexPoolInfo();
    virtual ~CMIndexPoolInfo();
    
    
    int Serialize(vector<char>& oPoolInfo);
    int Deserialize(const char *iPoolInfo, int iDataSize);
    
    bool Equal(const CMIndexPoolInfo *ipIndexPoolInfo);
    
    void AddIndexInfo(CMIndexInfo *ipIndexInfo);
    CMIndexInfo *GetIndexInfo(int index) ;
    
    int Count();
    CMIndexInfo *Item(int index);
    
    int Size();
    
private:
    vector<CMIndexInfo *> mvIndexInfo; // mvIndexInfo[0] should be main index, now only main index
    
};

class DSSCMNodeModel
{
public:
    DSSCMNodeModel();
    virtual ~DSSCMNodeModel();

    virtual int SetNodeModel(DSSRWNodeModel *ipNodeModel);
    
    virtual bool IsSupportLoadFromCache();
    
    bool HitCache();
    
    virtual bool FindLocalCache();
    
    bool IsFirstTimeLoading();
    
    virtual int LoadFromLocalCache();
    
    // tehe: this method is used to support get large data from local cache, or caller want to read the data by himself. if there are several block with the same block type, it will only get the data of first block
    int GetBlockReaderStream(EnumCMBlockType iBlockType, DSSBinaryDataReader **oppBinaryStream);
    
    // tehe: if there are several block with the same block type, it will only get the data of first block.
    int GetBlockHeader(EnumCMBlockType iBlockType, CMBlockHeader & oBlockHeader);
    
    //  tehe: if block data is large, suggest you use GetBinaryStream instead of this method. if there are several block with the same block type, it will only get the data of first block
    int GetBlockData(EnumCMBlockType iBlockType, char ** oppData, int *opSize);
    
    // tehe: AppendNewBlock will not check whether the block is exist. if same block type exist, it will still add the new block. 
    int AppendNewBlock(EnumCMBlockType iBlockType, int iCount, const int *iDataSize, const char** ippData);
    
    int AppendNewBlock(EnumCMBlockType iBlockType, DSSBinaryDataReader * ipReadStream,int iBlockSize);
    
    virtual int AppendMoreBlocks(int iFlags);

    inline MBase::CString & GetLocalCachePath() { return mLocalCachePath; }
    
    int CreateLocalCache();
    
    int GetDocumentInfo(vector<char> & oDocInfo);
    
    int UpdateDocInfo();
    
    inline void SetStatus(EnumCMStatus iStatus) { mStatus = iStatus; };
    
    inline void AddMMapedIndex(DSSTabularIndex *ipIndex) { mvMMapedIndices.push_back(ipIndex) ;}
    
    inline void AddNewIndex(DSSTabularIndex *ipIndex) { mvNewIndices.push_back(ipIndex); }
    
    inline void AddNewIndexInfo(CMIndexInfo *ipNewIndexInfo) { mvNewIndeicesInfo.push_back(ipNewIndexInfo); }
    
    inline EnumCMStatus GetCMStatus() { return mStatus; }
     
    int CreateMMapMemory(DSSCMMemoryMap *ipMMapObject);
    
    virtual int RestoreContext();
    
    int AddBlockHeader(EnumCMBlockType iBlockType, int iBlockSize, CMBlockHeader *iopBlockHeader = NULL);
    
    int AppendBlockData(int iCount, const int *iDataSize, const char** ippData);
    
    int MMap(DSSCMMemoryMap *ipMMapObject, int iFileOffset, int iBinarySize);
    
    int GetMMapedIndexSize() { return mvMMapedIndices.size(); }
    EnumDSSRWNodeType getNodeType();
    
protected:
    DSSRWNodeModel *mpNodeModel;

    MBase::CString mLocalCachePath;
    
    EnumCMStatus mStatus;
    
    // tehe: used to keep which indices are mmaped to file, sometimes we want to know the order of mmaped indices
    vector<DSSTabularIndex *> mvMMapedIndices;
    
    // tehe: used to hold new index which is created during binary load workflow
    vector<DSSTabularIndex *> mvNewIndices;
    vector<CMIndexInfo *> mvNewIndeicesInfo;
private:
    
    MBase::CString mDataSourceFilePath;
    
    bool mbLocalCacheCreated;
};
#endif
