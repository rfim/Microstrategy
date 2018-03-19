//
//  DSSCManipulationMoveMetric.h
//  ReportServiceCore
//
//  Created by wzhu on 12/30/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef ReportServiceCore_DSSCManipulationMoveMetric_h
#define ReportServiceCore_DSSCManipulationMoveMetric_h

#import "GUID.h"
#import "DSSRWIterator.h"
#import "DSSCellHandle.h"
#include "DSSCManipulation.h"
#include "DSSUnitObject.h"

class DSSRWTemplateNode;
class DSSTemplateMetric;

class DSSCManipulationMoveMetric : public DSSCManipulation
{
private:    
	DSSRWTemplateNode* mpTemplateNode;
	DSSTemplateMetric* mpUnit;
    DSSTemplate* mTemplate;
    DSSObjectInfo* mObject;
    
    int mPos;
    
#ifndef __ANDROID__
	void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
public:
	DSSCManipulationMoveMetric(DSSRWTemplateNode* ipNode, DSSObjectInfo* iObject, int idPos);
    DSSCManipulationMoveMetric(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSCManipulationMoveMetric();
    
	int Execute();
    void GenerateAODeltaXML();
};

#endif
