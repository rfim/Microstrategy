//
//  DSSDimty.h
//  ReportServiceCore
//
//  Created by Zhu, Wei on 3/13/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef __ReportServiceCore__DSSDimty__
#define __ReportServiceCore__DSSDimty__

#include <vector>
#include "DSSSecondClassObject.h"
#include "DSSDimtyUnit.h"

class DSSDimty: public DSSSecondClassObject
{
private:
    bool mCanContinue;
    bool mFilterReset;
    GUID mID;
    std::vector<DSSDimtyUnit*> mDimtyUnits;
public:
    DSSDimty();
    DSSDimty(DSSObjectContext* ipObjectContext);
    ~DSSDimty();
    
    bool getCanContinue();
    bool getFilterReset();
    DSSDimtyUnit* Item(int index);
    DSSObjectInfo* getInfo();
    int Count();
    int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
    int LoadDefnFromBinary2(DSSBinaryDataReader* ipReader); // without load the block header
};

inline bool DSSDimty::getCanContinue()
{
    return mCanContinue;
}

inline bool DSSDimty::getFilterReset()
{
    return mFilterReset;
}

inline int DSSDimty::Count()
{
    return mDimtyUnits.size();
}

#endif /* defined(__ReportServiceCore__DSSDimty__) */
