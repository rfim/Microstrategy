//
//  DSSCManipulationAddMetricToTemplate.h
//  ReportServiceCore
//
//  Created by wzhu on 12/29/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef ReportServiceCore_DSSCManipulationAddMetricToTemplate_h
#define ReportServiceCore_DSSCManipulationAddMetricToTemplate_h

#import "GUID.h"
#import "DSSRWIterator.h"
#import "DSSCellHandle.h"
#include "DSSCManipulation.h"
#include "DSSUnitObject.h"
#include "DSSUnitObject.h"

class DSSRWTemplateNode;
class DSSTemplateMetric;

class DSSCManipulationAddMetricToTemplate : public DSSCManipulation
{
private:    
	DSSRWTemplateNode* mpTemplateNode;
	DSSTemplateMetric* mpUnit;
    DSSTemplate* mTemplate;
    DSSObjectInfo* mObject;
    DSSUnitObject* mUnitObject;
    
    DSSAxis* mAxis;
    int mPos;
    bool mHasTemplateMetrics;
    
#ifndef __ANDROID__
	void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
public:
	DSSCManipulationAddMetricToTemplate(DSSRWTemplateNode* ipNode, DSSObjectInfo* iObject, EnumDSSAxisName idAxis, int idPos);
    DSSCManipulationAddMetricToTemplate(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSCManipulationAddMetricToTemplate();
    
	int Execute();
    void GenerateAODeltaXML();
};

#endif