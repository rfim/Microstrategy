//
//  DSSCManipulationSetTemplateProperties.h
//  ReportServiceCore
//
//  Created by Li, Dong on 4/15/13.
//
//
#ifndef CMANIPULATION_SET_TEMPLATE_PROPERTIES_H
#define CMANIPULATION_SET_TEMPLATE_PROPERTIES_H

#include "DSSCManipulationSetProperties.h"
#include "DSSTemplate.h"

class DSSCManipulationSetTemplateProperties: public DSSCManipulationSetProperties
{
public:
    DSSCManipulationSetTemplateProperties(DSSRWTemplateNode* ipNode);
    DSSCManipulationSetTemplateProperties(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    virtual ~DSSCManipulationSetTemplateProperties();
    int Execute();
private:
    void UpdateModel();
    void PersistProperties();
    DSSFormat* getFormat(int iIndex);
#ifndef __ANDROID__
    void GenerateAODeltaXML();
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    DSSTemplate* mpTemplate;
};
/*
#include "DSSCManipulationSetProperties.h"
#include "DSSAODeltaXML.h"

class DSSCManipulationSetNodeProperties : public DSSCManipulationSetProperties
{
public:
    DSSCManipulationSetNodeProperties(DSSRWNode* ipNode, DSSCellHandle* ipCellHandle = NULL);
    DSSCManipulationSetNodeProperties(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    virtual ~DSSCManipulationSetNodeProperties();
    int Execute();
protected:
    virtual void UpdateModel();
    void PersistProperties();
#ifndef __ANDROID__
    void GenerateAODeltaXML();
    virtual void hParseXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
};*/

#endif
