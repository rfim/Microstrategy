//
//  DSSCManipulationAddTemplate.h
//  ReportServiceCore
//
//  Created by Li, Dong on 1/12/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CMANIPULATION_ADD_TEMPLATE_H
#define CMANIPULATION_ADD_TEMPLATE_H

#include "DSSCManipulationAddRWNode.h"
#include "DSSRWTemplateNode.h"
#include "DSSRWSectionNode.h"
#include "DSSGraphProperties.h"
#include <set>

class DSSCManipulationAddTemplate: public DSSCManipulationAddRWNode
{
public:
    DSSCManipulationAddTemplate(DSSRWSectionNode* ipParentNode, EnumDSSDisplayMode iDisplayMode, GUID iDataSourceID = GUID_NULL);
    DSSCManipulationAddTemplate(DSSRWSectionNode* ipParentNode, EnumDSSDisplayMode iDisplayMode, DSSGraphProperties* ipGraphProperties, EnumGraphMajorType iMajorType = MajorTypeVerticalBar);
    DSSCManipulationAddTemplate(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    
    ~DSSCManipulationAddTemplate();
    int Execute();
    
    DSSRWNode* getTemplateNode();
    void SetProperties(std::vector<PropertyInfo*> iProperties);
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
protected:
    DSSRWTemplateNode* mpTemplateNode;
    DSSTemplate* mpTemplate;
    EnumDSSDisplayMode mDisplayMode;
    GUID mDataSourceID;
    DSSGraphProperties* mpGraphProperties;
    EnumGraphMajorType mGraphMajorType;
    
    std::vector<PropertyInfo*> mProperties;
    
    void AddTemplate();
    GUID GenerateGUID();
};
#endif
