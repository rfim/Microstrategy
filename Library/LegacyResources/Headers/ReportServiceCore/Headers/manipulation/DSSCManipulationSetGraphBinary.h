//
//  DSSCManipulationSetGraphBinary.h
//  ReportServiceCore
//
//  Created by Li, Dong on 5/18/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#ifndef CMANIPULATION_SET_GRAPH_BINARY_H
#define CMANIPULATION_SET_GRAPH_BINARY_H

#include "DSSCManipulation.h"
#include "DSSRWTemplateNode.h"
#include "DSSGraphProperties.h"

class DSSCManipulationSetGraphBinary: public DSSCManipulation
{
public:
    DSSCManipulationSetGraphBinary(DSSRWTemplateNode* ipTemplateNode, DSSGraphProperties* ipGraphProperties = NULL, EnumGraphMajorType iGraphMajorType = MajorTypeVerticalArea, int iGraphSubtype = 1, EnumDSSGraphType iGraphType = DssGraphTypeReserved);
    DSSCManipulationSetGraphBinary(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationSetGraphBinary();
    
    int Execute();
    
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    virtual void hParseXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
    DSSRWTemplateNode* getTemplateNode();
    
private:
    DSSRWTemplateNode* mpTemplateNode;
    DSSGraphProperties* mpGraphProperties;
    EnumGraphMajorType mGraphMajorType;
    int mGraphSubtype;
    EnumDSSGraphType mGraphType;
};

#endif
