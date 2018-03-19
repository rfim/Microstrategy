//
//  DSSDropZone.h
//  DashboardViewer
//
//  Created by Zeng, Jinliang on 6/18/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#ifndef DSS_Drop_Zone_H
#define DSS_Drop_Zone_H

#include "DSSSecondClassObject.h"

typedef GUID DSS_DZUnitID;

typedef EnumDSSObjectType EnumDSSDropZoneUnitType;

typedef enum {
    DssDropZoneReserved = 0,
    DssDropZoneRows = 1,
    DssDropZoneColumns = 2,
    DssDropZoneXAxis = 3,
    DssDropZoneYAxis = 4,
    DssDropZoneBreakBy = 5,
    DssDropZoneSliceBy = 6,
    DssDropZoneColorBy = 7,
    DssDropZoneSizeBy = 8,
    DssDropZoneTooltip = 9,
    DssDropZoneAngle = 10,
    DssDropZoneGrouping = 11,
    DssDropZoneGeo = 12,
    DssDropZoneLayout = 13,
    DssDropZoneFromItem = 14,
    DssDropZoneToItem = 15,
    DssDropZoneItemSize = 16,
    DssDropZoneLatitude = 17,
    DssDropZoneLongitude = 18,
    DssDropZoneLastOne,
    DssDropZoneRangeEnd = 255
} EnumDSSDropZoneType;

typedef struct DSSDropZoneUnit
{
public:
    DSSDropZoneUnit(DSS_DZUnitID lID, EnumDSSDropZoneUnitType lUnitType) :
        mID(lID), mUnitType(lUnitType), mDualAxisIndex(-1), mIsCondense(false)
    {
    }
    const DSS_DZUnitID& getID() const { return mID; }
    EnumDSSDropZoneUnitType getUnitType() const { return mUnitType; }
    int dualAxisIndex() const { return mDualAxisIndex; }
    int& dualAxisIndex() { return mDualAxisIndex; }
    bool isCondense() const { return mIsCondense; }
    bool& isCondense() { return mIsCondense; }
private:
    DSS_DZUnitID mID;
    EnumDSSDropZoneUnitType mUnitType;
    int mDualAxisIndex;
    bool mIsCondense;
} DSSDropZoneUnit;

class DSSDropZone : public DSSSecondClassObject
{
public:
	DSSDropZone() : mDropZoneType(DssDropZoneReserved) {}
	virtual ~DSSDropZone() { Clear(); }
	DSSDropZone(DSSObjectContext* ipObjectContext, EnumDSSDropZoneType iDZType = DssDropZoneReserved);
    
	int Count() const;
	DSSDropZoneUnit* Item(int iIndex) const;
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	int Find(const DSS_DZUnitID& iID) const;
    const DSSDropZoneUnit* getDropZoneUnit(const DSS_DZUnitID& iID) const;
	void Add(DSSDropZoneUnit* iDZUnit, int iPos = -1);
    void Delete(DSSDropZoneUnit* iDZUnit);
	void Clear();
	
    std::vector<DSSDropZoneUnit*> getDZUnits() { return mDZUnits; }
    EnumDSSDropZoneType getDropZoneType() { return mDropZoneType; }
    void setDropZoneType(EnumDSSDropZoneType iDropZoneType)
    {
        mDropZoneType = iDropZoneType;
    }

private:
	EnumDSSDropZoneType mDropZoneType;
	std::vector<DSSDropZoneUnit*> mDZUnits;
	
};

#endif /* defined(DSS_Drop_Zone_H) */
