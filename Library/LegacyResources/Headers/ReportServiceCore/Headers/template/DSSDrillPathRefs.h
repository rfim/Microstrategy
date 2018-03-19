/*
 *  DSSDrillPathRefs.h
 *  ReportServiceCore
 *
 *  Created by JING LI on 4/29/10.
 *  Copyright 2010 Microstrategy Inc. All rights reserved.
 *
 */

#ifndef DRILLPATHREFS_h
#define DRILLPATHREFS_h 1

#include "DSSSecondClassObject.h"
#include <vector>

class DSSBinaryDataReader;
class DSSDrillPath;
class DSSTemplate;

class DSSDrillPathRefs : public DSSSecondClassObject
{
private:
	std::vector<int> mDrillPathRefs;
    std::vector<int> mDrillWithinPathRefs;
	DSSTemplate* mpTemplate;
	void hClear();
    bool mDrillWithinOnly;
public:	
	DSSDrillPathRefs();
	virtual ~DSSDrillPathRefs();
	DSSDrillPathRefs(DSSObjectContext* ipObjectContext, DSSTemplate* ipTemplate);	
	int Count();
	DSSDrillPath* Item(int iIndex);
	int getDrillPathIndex(int iIndex);
	void Remove(int iIndex);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
    DSSTemplate* getTemplate();
    void Duplicate(DSSDrillPathRefs* ipPath, DSSTemplate* ipTemplate = NULL);
    
    void setFilterDrillWithin(bool isDrillWithinOnly) { mDrillWithinOnly = isDrillWithinOnly; }
    bool isDrillWithinOnly() const { return mDrillWithinOnly; }
    void refreshDrillWithinList();
};

#endif

