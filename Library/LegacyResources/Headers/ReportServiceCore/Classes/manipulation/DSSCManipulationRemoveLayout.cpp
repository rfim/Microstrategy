//
//  DSSCManipulationRemoveLayout.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 4/24/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#include "DSSCManipulationRemoveLayout.h"

DSSCManipulationRemoveLayout::DSSCManipulationRemoveLayout(DSSRWNode* ipNode): DSSCManipulationRemoveRWNode(ipNode)
{
    mpRootNode = static_cast<DSSRWSectionNode*>(mpNode->getParentNode());
    
    mType = ManipulationRemoveLayout;
}

DSSCManipulationRemoveLayout::DSSCManipulationRemoveLayout(DSSActionObject* ipAO, DSSObjectContext* ipContext): DSSCManipulationRemoveRWNode(ipAO, ipContext)
{
    mType = ManipulationRemoveLayout;
}

DSSCManipulationRemoveLayout::~DSSCManipulationRemoveLayout()
{
    
}

DSSRWSectionNode* DSSCManipulationRemoveLayout::getRootNode()
{
    return mpRootNode;
}

int DSSCManipulationRemoveLayout::Execute()
{
    if(mpNode->getType() != DssRWNodeSection || (static_cast<DSSRWSectionNode*>(mpNode))->getSectionType() != DssRWSectionGeneric)
        return S_FALSE;
    return DSSCManipulationRemoveRWNode::Execute();
}