//
//  DSSCManipulationSetProperties.h
//  ReportServiceCore
//
//  Created by Li, Dong on 3/2/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#ifndef CMANIPULATION_SET_PROPERTIES_H
#define CMANIPULATION_SET_PROPERTIES_H

#include "DSSCManipulation.h"
#include "DSSCellHandle.h"
#include "DSSRWNode.h"
#include "DSSRWTemplateNode.h"

class DSSCManipulationSetProperties : public DSSCManipulation
{
public:
    DSSCManipulationSetProperties(DSSRWNode* ipNode, DSSCellHandle* ipCellHandle = NULL);
    DSSCManipulationSetProperties(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    virtual ~DSSCManipulationSetProperties();
    virtual int Execute();
    DSSRWTemplateNode* getTemplateNode();    
    void AddProperty(PropertyInfo* ipPropertyInfo);
    
protected:
    DSSRWNode* mpNode;
    DSSCellHandle* mpCellHandle;
    std::vector<PropertyInfo*> mProperties;
       
    virtual void UpdateModel();
    virtual void PersistProperties();
    virtual DSSFormat* getFormat(int iIndex, bool iReadOnly = false);
#ifndef __ANDROID__
    virtual std::string hBuildXML();
    virtual void hParseXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
};
#endif
