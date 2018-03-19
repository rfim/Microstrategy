//
//  DSSCManipulationAddField.h
//  ReportServiceCore
//
//  Created by Li, Dong on 4/25/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#ifndef CMANIPULATION_ADD_FIELD_H
#define CMANIPULATION_ADD_FIELD_H

#include "DSSCManipulationAddRWNode.h"
#include "DSSRWFieldGroupNode.h"

class DSSCManipulationAddField: public DSSCManipulationAddRWNode
{
public:
    DSSCManipulationAddField(DSSRWNode* ipNode, std::string iFieldName, EnumDSSRWFieldType iFieldType, std::string iFieldText, EnumDSSObjectType iFieldObjectType, GUID iFieldObjectID, double iFieldLeft, double iFieldTop, double iFieldWidth, double iFieldHeight, std::vector<PropertyInfo*>& iProperties);
    DSSCManipulationAddField(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationAddField();
    
    int Execute();
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
private:
    // DSSRWNode* mpNode;
    DSSRWFieldGroupNode* mpFieldGroupNode;
    DSSField* mpField;
    std::vector<PropertyInfo*> mProperties;
    
};
#endif
