//
//  DSSDropZoneSet.h
//  DashboardViewer
//
//  Created by Zeng, Jinliang on 6/19/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#ifndef DSS_DropZoneSet_H
#define DSS_DropZoneSet_H

#include "DSSSecondClassObject.h"

#include "DSSDropZone.h"

class DSSDropZoneSet: public DSSSecondClassObject
{
public:
	DSSDropZoneSet();
	virtual ~DSSDropZoneSet();
	DSSDropZoneSet(DSSObjectContext* ipObjectContext);

	int Count();
	DSSDropZone* Item(int iIndex);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	int Find(EnumDSSDropZoneType iType) const;
    const DSSDropZone* getDropZone(EnumDSSDropZoneType iType) const;
	void Add(DSSDropZone* iDropZone, int iPos = -1);
    void Delete(DSSDropZone* iDropZone);
	void Clear();
	
    std::vector<DSSDropZone*> getDropZones() { return mDropZones; }
    // property name support <type/dualAxisIndex/isCondense>
    // bool value return "1(true)/0(false)"
    // return null means unhit
    std::string getPropertyValue(EnumDSSDropZoneType iType, const GUID& iDZUnitID, const std::string& iPropertyName);
private:
	std::vector<DSSDropZone*> mDropZones;
};


#endif /* defined(DSS_DropZoneSet_H) */
