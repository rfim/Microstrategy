//
//  DSSCManipulationSetPanelVisible.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 3/12/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#include "DSSCManipulationSetPanelVisible.h"

DSSCManipulationSetPanelVisible::DSSCManipulationSetPanelVisible(DSSRWNode* ipPanel):DSSCManipulationSetNodeProperties(ipPanel)
{
    mType = ManipulationSetPanelVisible;
    
    if(ipPanel->getType() == DssRWNodeSection && (static_cast<DSSRWSectionNode*>(ipPanel))->getSectionType() == DssRWSectionPanel)
        mpNode = ipPanel;
}

DSSCManipulationSetPanelVisible::DSSCManipulationSetPanelVisible(DSSActionObject* ipAO, DSSObjectContext* ipContext): DSSCManipulationSetNodeProperties(ipAO, ipContext)
{
    mType = ManipulationSetPanelVisible;
}

DSSCManipulationSetPanelVisible::~DSSCManipulationSetPanelVisible()
{}

int DSSCManipulationSetPanelVisible::Execute()
{   
    mNodesModelChanged.push_back(mpNode->getParentNode());
    
    MapFromTargetToControl* lMapFromTargetToControl = mpNode->getObjectContext()->getMapFromTargetToControl();
    NodeAndControlsVector lVector = (*lMapFromTargetToControl)[mpNode->getParentNode()->getKey()];
    for(NodeAndControlsVector::iterator lIter = lVector.begin(); lIter != lVector.end(); lIter++)
    {
        std::string lNodeKey = (*lIter)->NodeKey;
        DSSRWNode* lNode = mpNode->getObjectContext()->getNode(lNodeKey);
        mNodesModelChanged.push_back(lNode);
    }
    
    return DSSCManipulationSetNodeProperties::Execute();
}
