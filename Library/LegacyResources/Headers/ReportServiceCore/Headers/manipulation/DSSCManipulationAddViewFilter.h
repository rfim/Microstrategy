//
//  DSSCManipulationAddViewFilter.h
//  ReportServiceCore
//
//  Created by Li, Dong on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CMANIPULATION_ADD_VIEW_FILTER_H
#define CMANIPULATION_ADD_VIEW_FILTER_H

#include "DSSCManipulation.h"
#include "DSSRWTemplateNode.h"
#include "DSSCExpression.h"
#ifndef __ANDROID__
    #include "DSSSimpleXML.h"
#endif

class DSSCManipulationAddViewFilter: public DSSCManipulation
{
public:
    DSSCManipulationAddViewFilter(DSSRWTemplateNode* ipTemplateNode, DSSCExpression* ipExpression);
    DSSCManipulationAddViewFilter(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationAddViewFilter();
    
    int Execute();
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
    DSSExpNode* hBuildExpNode(SimpleXMLNode& iXMLNode, DSSExpNode* ipParentExpNode, DSSObjectContext* ipContext);
#endif
private:
    DSSRWTemplateNode* mpTemplateNode;
    DSSCExpression* mpExpression;
};

#endif
