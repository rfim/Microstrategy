/*
 *  DSSUnitObjects.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 11-1-18.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef UNIT_OBJECTS_H
#define UNIT_OBJECTS_H

#include "DSSUnitObject.h"
#include <vector>

class DSSUnitObjects : public DSSSecondClassObject
{
private:
	std::vector<DSSUnitObject*> mUnitObjects;
public:
	DSSUnitObjects();
	virtual ~DSSUnitObjects();
	DSSUnitObjects(DSSObjectContext* ipObjectContext);
	int Count();
	DSSUnitObject* Item(int iIndex);
	void Add(DSSUnitObject* iUnitObject, int iIndex=-1);
	DSSUnitObject* FindByID(GUID iID);
};

#endif
