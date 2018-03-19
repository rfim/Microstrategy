//
//  DSSCManipulationSetPanelVisible.h
//  ReportServiceCore
//
//  Created by Li, Dong on 3/12/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#ifndef CMANIPULATION_SET_PANEL_VISIBLE_H
#define CMANIPULATION_SET_PANEL_VISIBLE_H

#include "DSSCManipulationSetNodeProperties.h"
#include "DSSRWSectionNode.h"

class DSSCManipulationSetPanelVisible: public DSSCManipulationSetNodeProperties
{
public:
    DSSCManipulationSetPanelVisible(DSSRWNode* ipPanel);
    DSSCManipulationSetPanelVisible(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    virtual ~DSSCManipulationSetPanelVisible();
    int Execute();
};
#endif
