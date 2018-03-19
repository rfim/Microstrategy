/*
 *  DSSDrillPaths.h
 *  ReportServiceCore
 *
 *  Created by JING LI on 4/28/10.
 *  Copyright 2010 Microstrategy Inc. All rights reserved.
 *
 */
#ifndef DRILLPATHS_h
#define DRILLPATHS_h 1

#include "DSSSecondClassObject.h"
#include <vector>

class DSSBinaryDataReader;
class DSSDrillPath;

class DSSDrillPaths : public DSSSecondClassObject
{
private:
	std::vector<DSSDrillPath*> mDrillPaths;
	void hClear();
public:	
	DSSDrillPaths();
	virtual ~DSSDrillPaths();
	DSSDrillPaths(DSSObjectContext* ipObjectContext);	
	int Count();
	DSSDrillPath* Item(int iIndex);
	void Add(DSSDrillPath* iDrillPath, int iIndex=-1);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader, bool iIsLoadExtendedProperties);
};

#endif

