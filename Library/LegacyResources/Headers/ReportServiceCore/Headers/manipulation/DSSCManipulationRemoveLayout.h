//
//  DSSCManipulationRemoveLayout.h
//  ReportServiceCore
//
//  Created by Li, Dong on 4/24/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#ifndef CMANIPULATION_REMOVE_LAYOUT_H
#define CMANIPULATION_REMOVE_LAYOUT_H

#include "DSSCManipulationRemoveRWNode.h"
#include "DSSRWSectionNode.h"

class DSSCManipulationRemoveLayout: public DSSCManipulationRemoveRWNode
{
public:
    DSSCManipulationRemoveLayout(DSSRWNode* ipNode);
    DSSCManipulationRemoveLayout(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationRemoveLayout();
    
    DSSRWSectionNode* getRootNode();
    
    int Execute();
    
private:
    DSSRWSectionNode* mpRootNode;
};

#endif
