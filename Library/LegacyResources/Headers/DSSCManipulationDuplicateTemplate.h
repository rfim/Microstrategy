//
//  DSSCManipulationDuplicateTemplate.h
//  ReportServiceCore
//
//  Created by Li, Dong on 2/15/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CMANIPULATION_DUPLICATE_TEMPLATE_H
#define CMANIPULATION_DUPLICATE_TEMPLATE_H

#include "DSSCManipulationAddTemplate.h"

class DSSCManipulationDuplicateTemplate : public DSSCManipulationAddTemplate
{
public:
    DSSCManipulationDuplicateTemplate(DSSRWSectionNode* ipParentNode, DSSRWTemplateNode* ipSourceTemplateNode, std::string iName = "", std::string iTitle = "", EnumDSSDisplayMode iDisplayMode = DssDisplayModeReserved, DSSGraphProperties* ipGraphProperties = NULL, EnumGraphMajorType iGraphMajorType = MajorTypeVerticalArea, int iGraphMinorType = 1, GUID iDataSourceID = GUID_NULL);
    DSSCManipulationDuplicateTemplate(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationDuplicateTemplate();
    
    int Execute();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
protected:
    DSSRWTemplateNode* mpSourceTemplateNode;
    DSSTemplate* mpSourceTemplate;
    
    // derived from DSSCManipulationAddTemplate
    // DSSRWTemplateNode* mpTemplateNode;
    // DSSTemplate* mpTemplate;
    std::string mName;
    std::string mTitle;
    // EnumDSSDisplayMode mDisplayMode;
    // DSSGraphProperties* mpGraphProperties;
    // EnumGraphMajorType mGraphMajorType;
    int mGraphMinorType;
    // GUID mDataSourceID;
    // std::vector<PropertyInfo*> mProperties;
    void GenerateAOXML();
    bool mUnderSamePanel;
    bool mNeedAddTarget;
};

#endif
