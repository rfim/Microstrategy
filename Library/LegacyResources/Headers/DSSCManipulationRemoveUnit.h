//
//  DSSCManipulationRemoveUnit.h
//  ReportServiceCore
//
//  Created by wzhu on 12/28/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef ReportServiceCore_DSSCManipulationRemoveUnit_h
#define ReportServiceCore_DSSCManipulationRemoveUnit_h

#import "GUID.h"
#import "DSSRWIterator.h"
#import "DSSCellHandle.h"
#include "DSSCManipulation.h"

class DSSRWTemplateNode;

class DSSCManipulationRemoveUnit : public DSSCManipulation
{
private:    
	DSSRWTemplateNode* mpTemplateNode;
	DSSTemplateUnit* mpUnit;
    DSSObjectInfo* mObject;
    
    DSSTemplate* mTemplate;
    DSSAxis* mAxis;
    DSSTemplateMetrics* mMetrics;
    int mPos;
    int mMetricsPos;
    bool mShouldRemoveControl;
    
#ifndef __ANDROID__
	void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
public:
	DSSCManipulationRemoveUnit(DSSRWTemplateNode* ipNode, DSSObjectInfo* iObject, bool iShouldRemoveControl = true);
    DSSCManipulationRemoveUnit(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSCManipulationRemoveUnit();
    
	int Execute();
    void GenerateAODeltaXML();
};

#endif
