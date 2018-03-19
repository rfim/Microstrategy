//
//  DSSCManipulationRemovePanel.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 1/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "DSSCManipulationRemovePanel.h"

DSSCManipulationRemovePanel::DSSCManipulationRemovePanel(DSSRWNode* ipNode) : DSSCManipulationRemoveRWNode(ipNode)
{
    mType = ManipulationRemovePanel;
}

DSSCManipulationRemovePanel::~DSSCManipulationRemovePanel()
{
    
}

int DSSCManipulationRemovePanel::Execute()
{
    if(mpNode->getType() != DssRWNodeSection)
    {
        return S_FALSE;
    }
    return DSSCManipulationRemoveRWNode::Execute();
}