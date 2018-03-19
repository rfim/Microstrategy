//
//  DSSCManipulationShowThresholds.h
//  ReportServiceCore
//
//  Created by Li, Dong on 3/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CMANIPULATION_SHOW_THRESHOLDS_H
#define CMANIPULATION_SHOW_THRESHOLDS_H

#include "DSSCManipulation.h"
#include "DSSThresholds.h"
#include "DSSRWTemplateNode.h"
#include "DSSTemplateMetric.h"

class DSSCManipulationShowThresholds: public DSSCManipulation
{
public:
    DSSCManipulationShowThresholds(DSSRWTemplateNode* ipTemplateNode, DSSTemplateMetric* ipTemplateMetric);
    DSSCManipulationShowThresholds(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationShowThresholds();
    
    int Execute();
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
private:
    DSSRWTemplateNode* mpTemplateNode;
    DSSTemplateMetric* mpTemplateMetric;
    int mEnabled;
};

#endif
