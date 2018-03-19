//
//  DSSCManipulationRemonvRWNode.h
//  ReportServiceCore
//
//  Created by Li, Dong on 1/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CMANIPULATION_REMOVE_RWNODE_H
#define CMANIPULATION_REMOVE_RWNODE_H

#include "DSSCManipulation.h"

class DSSCManipulationRemoveRWNode : public DSSCManipulation
{
public:
    DSSCManipulationRemoveRWNode(DSSRWNode* ipNode);
    DSSCManipulationRemoveRWNode(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationRemoveRWNode();
    
    virtual int Execute();
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
protected:
    void RemoveRWNode();
    DSSRWNode* mpNode;
};

#endif