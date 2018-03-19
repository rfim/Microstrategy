/*
 *  DSSCManipulationPivotUnit.h
 *  ReportServiceCore
 *
 *  Modified by wzhu on 12/22/11.
 *  Created by admin on 10/20/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef CMANIPULATION_PIVOT_UNIT_H
#define CMANIPULATION_PIVOT_UNIT_H

#import "GUID.h"
#import "DSSRWIterator.h"
#import "DSSCellHandle.h"
#include "DSSCManipulation.h"
#include "DSSAOPivotUnit.h"
class DSSRWTemplateNode;
class DSSTemplateAttribute;

class DSSCManipulationPivotUnit : public DSSCManipulation
{
private:
	DSSRWTemplateNode* mpTemplateNode;
	DSSTemplateUnit* mpUnit;
	//DSSRWIterator* mpIterator;
	int mSrcAxisIndex;
	int mSrcUnitPosition;
	int mDstAxisIndex; //destination axis
	int mDstUnitPosition; //destination unit position
	bool mRefreshView;
	bool mEnableDataRetrieval;
	
#ifndef __ANDROID__
	void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
public:
	DSSCManipulationPivotUnit(DSSRWTemplateNode* ipNode, DSSTemplateUnit* ipUnit, int idAxis, int idPos);
	DSSCManipulationPivotUnit(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSCManipulationPivotUnit();
	int Execute();
	DSSRWTemplateNode* getTemplateNode();
    void GenerateAODeltaXML();
};

#endif
