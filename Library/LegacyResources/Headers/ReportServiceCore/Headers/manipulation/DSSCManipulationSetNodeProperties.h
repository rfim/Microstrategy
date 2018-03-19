//
//  DSSCManipulationSetNodeProperties.h
//  ReportServiceCore
//
//  Created by Li, Dong on 3/2/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#ifndef CMANIPULATION_SET_NODE_PROPERTIES_H
#define CMANIPULATION_SET_NODE_PROPERTIES_H

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
};
#endif
