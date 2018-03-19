//
//  DSSOfflineTransactionCSIInfo.h
//  ReportServiceCore
//
//  Created by Li, Dong on 2/11/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSOfflineTransactionCSIInfo_h
#define ReportServiceCore_DSSOfflineTransactionCSIInfo_h

#include <string>
#include "Base/Base/Buffer.h"
#include "SimpleBuffer.h"

class DSSCExpression;
class DSSBinaryDataReader;
class DFCCSITransactionBase;
class DSSOfflineTransactionCSIInfo
{
private:
    DFCCSITransactionBase* mpTransactionCSI;
    std::string mKey;
    int mIndex;
    MBase::Buffer* mpBuffer;
public:
    DSSOfflineTransactionCSIInfo();
    virtual ~DSSOfflineTransactionCSIInfo();
    DFCCSITransactionBase* getTransactionCSI();
    void setTransactionCSI(DFCCSITransactionBase* ipTransactionCSI);
    std::string getKey();
    void setKey(std::string iKey);
    int getIndex();
    void setIndex(int iIndex);
    int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
};
#endif
