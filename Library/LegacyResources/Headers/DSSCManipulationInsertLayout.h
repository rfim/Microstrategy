//
//  DSSCManipulationInsertLayout.h
//  ReportServiceCore
//
//  Created by Li, Dong on 2/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CMANIPULATION_INSERT_LAYOUT_H
#define CMANIPULATION_INSERT_LAYOUT_H

#include "DSSCManipulationAddRWNode.h"
#include "DSSRWSectionNode.h"

class DSSCManipulationInsertLayout : public DSSCManipulationAddRWNode
{
public:
    /*
    DSSCManipulationInsertLayout(DSSRWSectionNode* ipRootNode, DSSRWSectionNode* ipLayoutNode);
    DSSCManipulationInsertLayout(DSSRWSectionNode* ipRootNode, int iLayoutIndex, int iFlag);
    */
    DSSCManipulationInsertLayout(DSSRWSectionNode* ipRootNode, std::string iDocID, int mFlag);
    DSSCManipulationInsertLayout(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationInsertLayout();
    
    DSSRWNode* getLayoutNode();
    int Execute();
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif

private:
    // mpParentNode = ipRootNode;
    DSSRWSectionNode* mpLayoutNode;
    std::string mDocID;
    int mLayoutIndex;
    int mFlag;
};

#endif
