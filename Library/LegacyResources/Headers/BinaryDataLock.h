//
//  BinaryDataLock.h
//  ReportServiceCore
//
//  Created by Yang Zhuo on 10/18/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#include<pthread.h>
//#include "GUID.h"
#include<map>
#include "Base/Base/GUID.h"

using MBase::operator <;
using namespace std;

#ifndef ReportServiceCore_BinaryDataLock_h
#define ReportServiceCore_BinaryDataLock_h
/*
bool operator < (const GUID &iID1, const GUID &iID2)
{
    return MBase::CompareGUID(iID1, iID2);
}*/

class BinaryDataLock;

class SmartLock
{
public:
    SmartLock(BinaryDataLock *ipBinaryLock, GUID &iGUID, pthread_mutex_t * ipMutex);
    SmartLock(BinaryDataLock *ipBinaryLock, void *objectPoint);
    SmartLock();
    ~SmartLock();
    void lock(BinaryDataLock *ipBinaryLock, void *objectPoint);
private:
    BinaryDataLock *mpBinaryLock;
    pthread_mutex_t *mpMutex;
    GUID mGUID;
};

class BinaryDataLock
{
public:
    static BinaryDataLock * getInstance();
    pthread_mutex_t *getDocumentLock(GUID &lID);
    GUID getObjectGUID(void *objectPoint);
private:
    BinaryDataLock();
    ~BinaryDataLock();
    static BinaryDataLock *mpInstance;
    map<GUID, pthread_mutex_t *> mLockMap;
    map<GUID, int> mLockUseTime;
    map<GUID, int> mLockRefCnt;
    int timetick;
    pthread_mutex_t *mMapProtectLock;
    
    typedef map<void *, GUID> ObjectGUIDMap;
    ObjectGUIDMap mObjectGUIDMap;

friend class SmartLock;
};

#endif
