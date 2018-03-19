//
//  DSSDimtyUnit.h
//  ReportServiceCore
//
//  Created by Zhu, Wei on 3/13/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef __ReportServiceCore__DSSDimtyUnit__
#define __ReportServiceCore__DSSDimtyUnit__

#include "DSSSecondClassObject.h"

class DSSDimtyUnit: public DSSSecondClassObject
{
private:
    EnumDSSDimtyUnitType mUnitType;
    GUID mTargetID;
    EnumDSSAggregation mAggregationType;
    EnumDSSFiltering mFiltering;
    bool mGroupBy;
    int mRelativePosition;
    GUID mRoleRefID;
    std::vector<int> mAxisCollection;
    
public:
    DSSDimtyUnit();
    DSSDimtyUnit(DSSObjectContext* ipObjectContext);
    ~DSSDimtyUnit();
    
    GUID getRoleID();
    EnumDSSDimtyUnitType getUnitType();
    bool getGroupBy();
    EnumDSSAggregation getAggregation();
    EnumDSSFiltering getFiltering();
    GUID getTargetID();
    int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
    int Count();
};

inline GUID DSSDimtyUnit::getRoleID()
{
    return mRoleRefID;
}

inline EnumDSSDimtyUnitType DSSDimtyUnit::getUnitType()
{
    return mUnitType;
}

inline bool DSSDimtyUnit::getGroupBy()
{
    return mGroupBy;
}

inline EnumDSSAggregation DSSDimtyUnit::getAggregation()
{
    return mAggregationType;
}

inline EnumDSSFiltering DSSDimtyUnit::getFiltering()
{
    return mFiltering;
}

inline GUID DSSDimtyUnit::getTargetID()
{
    return mTargetID;
}

#endif /* defined(__ReportServiceCore__DSSDimtyUnit__) */
