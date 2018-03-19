//
//  DSSCManipulationRemovePanel.h
//  ReportServiceCore
//
//  Created by Li, Dong on 1/9/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#ifndef CMANIPULATION_REMOVE_PANEL
#define CMANIPULATION_REMOVE_PANEL

#include "DSSCManipulationRemoveRWNode.h"

class DSSCManipulationRemovePanel : public DSSCManipulationRemoveRWNode
{
public:
    DSSCManipulationRemovePanel(DSSRWNode* ipNode);
    virtual ~DSSCManipulationRemovePanel();
    
    int Execute();
};

#endif