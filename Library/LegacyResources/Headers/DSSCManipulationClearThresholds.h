//
//  DSSCManipulationClearThreshold.h
//  ReportServiceCore
//
//  Created by Li, Dong on 3/19/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CMANIPULATION_CLEAR_THRESHOLDS_H
#define CMANIPULATION_CLEAR_THRESHOLDS_H

#include "DSSCManipulation.h"
#include "DSSThresholds.h"
#include "DSSThreshold.h"
#include "DSSRWTemplateNode.h"
#include "DSSTemplateMetric.h"

class DSSCManipulationClearThresholds: public DSSCManipulation
{
public:
    DSSCManipulationClearThresholds(DSSRWTemplateNode* ipTemplateNode, DSSTemplateMetric* ipTemplateMetric);
    DSSCManipulationClearThresholds(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationClearThresholds();
    
    int Execute();
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
private:
    DSSRWTemplateNode* mpTemplateNode;
    DSSTemplateMetric* mpTemplateMetric;
    int mCount;
};

#endif
