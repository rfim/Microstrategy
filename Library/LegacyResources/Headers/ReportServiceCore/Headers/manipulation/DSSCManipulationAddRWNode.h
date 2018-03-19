//
//  DSSCManipulationAddRWNode.h
//  ReportServiceCore
//
//  Created by Li, Dong on 1/5/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//
#ifndef CMANIPULATION_ADD_RWNODE_H
#define CMANIPULATION_ADD_RWNODE_H

#include "DSSCManipulation.h"

class DSSCManipulationAddRWNode : public DSSCManipulation
{
public:
    DSSCManipulationAddRWNode();
    DSSCManipulationAddRWNode(EnumDSSRWNodeType iNodeType, DSSRWSectionNode* ipParentNode);
    ~DSSCManipulationAddRWNode();
    
    virtual int Execute();
    void setName(std::string iName);
    void setTitle(std::string iTitle);
protected:
    DSSRWNode* mpNode;
    DSSRWSectionNode* mpParentNode;
    void AddRWNode();
};

#endif