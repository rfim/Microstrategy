//
//  DSSCManipulationEditTemplate.h
//  ReportServiceCore
//
//  Created by Li, Dong on 5/8/13.
//
//

#ifndef CMANIPULATION_EDIT_TEMPLATE_H
#define CMANIPULATION_EDIT_TEMPLATE_H

#include "DSSCManipulation.h"
#include "DSSRWTemplateNode.h"

class DSSCManipulationEditTemplate: public DSSCManipulation
{
public:
    DSSCManipulationEditTemplate(DSSRWTemplateNode* ipTemplateNode, GUID iDataSourceID);
    DSSCManipulationEditTemplate(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationEditTemplate();
    
    int Execute();
    void GenerateAODeltaXML();
    DSSRWTemplateNode* getTemplateNode();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
private:
    DSSRWTemplateNode* mpTemplateNode;
    GUID mDataSourceID;
};

#endif