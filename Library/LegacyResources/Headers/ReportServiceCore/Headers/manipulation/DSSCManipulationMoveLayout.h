//
//  DSSCManipulationMoveLayout.h
//  ReportServiceCore
//
//  Created by Li, Dong on 4/24/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#ifndef CMANIPULATION_MOVE_LAYOUT_H
#define CMANIPULATION_MOVE_LAYOUT_H

#include "DSSCManipulation.h"
#include "DSSRWSectionNode.h"

class DSSCManipulationMoveLayout: public DSSCManipulation
{
public:
    DSSCManipulationMoveLayout(DSSRWSectionNode* ipLayoutNode, bool iIsMoveRight, int iNewIndex = -1);
    DSSCManipulationMoveLayout(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationMoveLayout();
    
    int Execute();
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
    DSSRWSectionNode* getLayoutNode();
    
private:
    DSSRWSectionNode* mpLayoutNode;
    int mNewIndex;
    
    void UpdateModel();
};
#endif
