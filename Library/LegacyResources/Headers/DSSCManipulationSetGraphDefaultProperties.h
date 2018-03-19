//
//  DSSCManipulationSetGraphDefaultProperties.h
//  ReportServiceCore
//
//  Created by Li, Dong on 5/22/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#ifndef CMANIPULATION_SET_GRAPH_DEFAULT_PROPERITES_H
#define CMANIPULATION_SET_GRAPH_DEFAULT_PROPERITES_H

#include "DSSCManipulation.h"
#include "DSSRWTemplateNode.h"

class DSSCManipulationSetGraphDefaultProperties: public DSSCManipulation
{
public:
    DSSCManipulationSetGraphDefaultProperties(DSSRWTemplateNode* ipTemplateNode, EnumGraphMajorType iGraphType =MajorTypeVerticalArea, int iGraphSubtype = 1,  bool isForChangeVizType = false);
    // DSSCManipulationSetGraphDefaultProperties(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationSetGraphDefaultProperties();
    
    int Execute();

    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
private:
    DSSRWTemplateNode* mpTemplateNode;
    EnumGraphMajorType mGraphType;
    int mGraphSubtype;
    bool  mIsForChangeVizType;
    
    void* GenerateGraphPropertyXML(EnumMFGraphAttributeID iPropertyID, EnumMFGraphObjectID iObjectID, std::string iPropertyValue);
    void* GenerateGraphPropertyXML(EnumMFGraphAttributeID iPropertyID, EnumMFGraphObjectID iObjectID, int iSeriesID, std::string iPropertyValue);
};

#endif
