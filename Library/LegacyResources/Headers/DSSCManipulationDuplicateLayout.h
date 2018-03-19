//
//  DSSCManipulationDuplicateLayout.h
//  ReportServiceCore
//
//  Created by Li, Dong on 5/15/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#ifndef CMANIPULATION_DUPLICATE_LAYOUT_H
#define CMANIPULATION_DUPLICATE_LAYOUT_H

#include "DSSCManipulation.h"
#include "DSSRWSectionNode.h"
#include "DSSRWTemplateNode.h"

class DSSCManipulationDuplicateLayout: public DSSCManipulation
{
public:
    DSSCManipulationDuplicateLayout(DSSRWSectionNode* ipSrcLayoutNode);
    DSSCManipulationDuplicateLayout(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationDuplicateLayout();
    
    int Execute();
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
    DSSRWSectionNode* getLayoutNode();
private:
    DSSRWSectionNode* mpSrcLayoutNode;
    DSSRWSectionNode* mpDstLayoutNode;
    DSSRWNode* mpRootNode;
    
    std::map<std::string, std::string> mKeyMap;
    void UpdateModel();
    
    DSSRWNode* hUpdateTree(DSSRWNode* ipNode, DSSRWNode* ipParentNode);
    void hCheckTargetKeys(DSSRWNode* ipNode);
    
    void hAddNodesChanged(DSSRWNode* ipNode);
};

#endif
