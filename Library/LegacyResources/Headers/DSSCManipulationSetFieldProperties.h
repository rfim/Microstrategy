//
//  DSSCManipulationSetFieldProperties.h
//  ReportServiceCore
//
//  Created by Li, Dong on 12/27/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef CMANIPULATION_SET_FIELD_PROPERTIES_H
#define CMANIPULATION_SET_FIELD_PROPERTIES_H

#include "DSSCManipulationSetProperties.h"
#include "DSSRWFieldGroupNode.h"

class DSSCManipulationSetFieldProperties : public DSSCManipulationSetProperties
{
public:
    DSSCManipulationSetFieldProperties(DSSRWFieldGroupNode* ipFieldGroupNode, std::string& iFieldKey);
    DSSCManipulationSetFieldProperties(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    int Execute();
    
    virtual ~DSSCManipulationSetFieldProperties();
    
protected:
    
    DSSFormat* getFormat(int iIndex, bool iReadOnly = false);
    void PersistProperties();
    void UpdateModel();
    
#ifndef __ANDROID__
    std::string hBuildXML();
    void hParseXML(std::string &iXMLString, DSSObjectContext *ipContext);
#endif
    
private:
    DSSRWFieldGroupNode* mpFieldGroupNode;
    DSSField* mpField;
};

#endif
