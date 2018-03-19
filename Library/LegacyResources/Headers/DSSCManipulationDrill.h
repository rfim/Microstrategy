/*
 *  DSSCManipulationDrill.h
 *  ReportServiceCore
 *
 *  Created by Wei Guo on 5/6/10.
 *  Copyright 2010 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef CMANIPULATION_DRILL_H
#define CMANIPULATION_DRILL_H

#include <vector>
#include <string>
#include "DSSRWGridIterator.h"
#include "DSSCellHandle.h"
#include "DSSCManipulation.h"

class DSSRWNode;
class DSSTemplateUnit;
class DSSTemplateMetric;
class DSSDrillPath;

typedef enum {
    DssNotKeepOriginal = 0,
    DssKeepOriginal = 1,
    DssDependOnServer = 2
} EnumKeepOriginalType;

class DSSCManipulationDrill : public DSSCManipulation
{
private:
	DSSRWNode* mpNode;
	DSSTemplateUnit* mpTemplateUnit;
	DSSTemplateMetric* mpTemplateMetric;
	std::vector<std::string> mElementIDs[2];	// Record the row and column element IDs
	std::vector<std::string> mGBElementIDs;
	std::vector<std::string> mCGBElementIDs;
	DSSDrillPath* mpDrillPath;
    std::string mMCSKeys;
    std::vector<std::string> mvNGBKeys;
	
public:
	DSSCManipulationDrill(DSSRWGridIterator* ipGridIterator, DSSCellHandle* ipCellHandle, DSSDrillPath* ipDrillPath);
	DSSCManipulationDrill(DSSRWGridIterator* ipGridIterator, DSSTemplateUnit* ipDrillUnit, DSSCellHandle* ipCellHandle, DSSDrillPath* ipDrillPath);
	DSSCManipulationDrill(DSSRWGridIterator* ipGridIterator, DSSTemplateUnit* ipDrillUnit, vector<DSSCellHandle*> ipCellHandle, DSSDrillPath* ipDrillPath);
    virtual ~DSSCManipulationDrill();
	
	int getTemplateUnitAxisIndex();
	int getTemplateUnitIndex();
	int getTemplateMetricIndex();

	std::string getNodeKey();
	int getNodeTreeType();

	int getDrillPathIndex();

    
	bool getDrillWithin();
	
	// Elements IDs related methods
	int getElementIDsSize(int iAxisIndex);	// axis index is 1-based DssAxisRow and DssAxisColumn
	std::vector<std::string> getElementIDsAsString(int iAxisIndex);
	
	// GB Elements IDs related methods
	int getGBElementIDsSize();
	std::vector<std::string> getGBElementIDsAsString();
	
	// CGB Elements IDs related methods
	int getCGBElementIDsSize();
	std::vector<std::string> getCGBElementIDsAsString();

	// Associated template nodes related methods
	int getAssociatedNodesCount();
	std::string getAssociatedNodeKeysAsString();
	
	int Execute();

    //ASYNCEXC: support drill path
    std::string getDrillPathName();

    EnumKeepOriginalType getKeepOriginal();
    
    std::string getHighestNGBKey();
    std::string getMCSKeys();
    
    int getNGBKeysSize();
    std::string getNGBKeysAsString();
};

inline int DSSCManipulationDrill::getNGBKeysSize()
{
    return (int)mvNGBKeys.size();
}

#endif
