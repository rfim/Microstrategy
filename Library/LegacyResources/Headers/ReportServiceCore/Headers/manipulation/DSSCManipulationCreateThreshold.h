//
//  DSSCManipulationCreateThreshold.h
//  ReportServiceCore
//
//  Created by Li, Dong on 3/19/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CMANIPULATION_CREATE_THRESHOLD_H
#define CMANIPULATION_CREATE_THRESHOLD_H

#include "DSSCManipulation.h"
#include "DSSThreshold.h"
#include "DSSThresholds.h"
#include "DSSRWTemplateNode.h"
#include "DSSTemplateMetric.h"
#include "DSSCExpression.h"
#include "DSSFormat.h"

class DSSCManipulationCreateThreshold: public DSSCManipulation
{
public:
    DSSCManipulationCreateThreshold(DSSThreshold* ipThreshold, DSSRWTemplateNode* ipTemplateNode, DSSTemplateMetric* ipTemplateMetric);
    DSSCManipulationCreateThreshold(std::string iName, std::string iReplaceText, int iSemantics, DSSCExpression* ipExpression, std::vector<PropertyInfo*> iProperties, DSSRWTemplateNode* ipTemplateNode, DSSTemplateMetric* ipTemplateMetric, bool iIsEnabled = true, EnumDSSThresholdScope = DssThresholdScopeMetricOnly);
    DSSCManipulationCreateThreshold(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationCreateThreshold();
    
    int Execute();
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
private:
    DSSThreshold* mpThreshold;
    DSSRWTemplateNode* mpTemplateNode;
    DSSTemplateMetric* mpTemplateMetric;
    std::vector<PropertyInfo*> mProperties;
    int mThresholdIndex;
    bool mIsEnabled;
};

#endif
