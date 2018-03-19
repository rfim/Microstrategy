//
//  DSSOfflineTransactionCSIInfos.h
//  ReportServiceCore
//
//  Created by Li, Dong on 2/11/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSOfflineTransactionCSIInfos_h
#define ReportServiceCore_DSSOfflineTransactionCSIInfos_h

#include <vector>
#include "DSSBinaryDataReader.h"
class DSSOfflineTransactionCSIInfo;
class DSSOfflineTransactionCSIInfos
{
private:
    std::vector<DSSOfflineTransactionCSIInfo*> mCSIInfos;
public:
    DSSOfflineTransactionCSIInfos();
    virtual ~DSSOfflineTransactionCSIInfos();
    void Add(DSSOfflineTransactionCSIInfo* ipCSIInfo, int iIndex = -1);
    int Count();
    DSSOfflineTransactionCSIInfo* Item(int iIndex);
    int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
};
#endif
