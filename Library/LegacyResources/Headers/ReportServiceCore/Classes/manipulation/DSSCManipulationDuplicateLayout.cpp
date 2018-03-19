//
//  DSSCManipulationDuplicateLayout.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 5/15/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#include "DSSCManipulationDuplicateLayout.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#include "DSSBaseElementProxy.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxml.h"
#else
#include "DSSSimpleXML.h"
#endif


DSSCManipulationDuplicateLayout::DSSCManipulationDuplicateLayout(DSSRWSectionNode* ipSrcLayoutNode)
{
    mpSrcLayoutNode = ipSrcLayoutNode;
    mpDstLayoutNode = NULL;
    mpRootNode = mpSrcLayoutNode->getParentNode();
    
    mType = ManipulationDuplicateLayout;
}

DSSCManipulationDuplicateLayout::DSSCManipulationDuplicateLayout(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationDuplicateLayout;
    
    if(!ipAO->IsDeltaXML())
        return;
    
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
    std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
    hParseAOXML(lXMLString, ipContext);
#endif
}

DSSCManipulationDuplicateLayout::~DSSCManipulationDuplicateLayout()
{
    
}

DSSRWSectionNode* DSSCManipulationDuplicateLayout::getLayoutNode()
{
    return mpDstLayoutNode;
}

void DSSCManipulationDuplicateLayout::UpdateModel()
{
    /*
     if(mpSrcLayoutNode->getSectionType() != DssRWSectionGeneric)
     return;
     */
    
    if(mpDstLayoutNode)
        delete mpDstLayoutNode;
    mpDstLayoutNode = static_cast<DSSRWSectionNode*>(hUpdateTree(mpSrcLayoutNode, mpRootNode));
    
    hCheckTargetKeys(mpDstLayoutNode);
}

DSSRWNode* DSSCManipulationDuplicateLayout::hUpdateTree(DSSRWNode *ipNode, DSSRWNode *ipParentNode)
{
    DSSRWNode* lpNode = new DSSRWNode();
    lpNode->Duplicate(ipNode);
    lpNode->setParentNode(ipParentNode);
    ipParentNode->AddChild(lpNode);
    for(int i = 0; i < ipNode->Count(); i++)
    {
        hUpdateTree(ipNode->Item(i), lpNode);
    }
    mKeyMap[ipNode->getKey()] = lpNode->getKey();
    return lpNode;
}

void DSSCManipulationDuplicateLayout::hCheckTargetKeys(DSSRWNode* ipNode)
{
    DSSRWControls* lpControls;
    if(ipNode->getType() == DssRWNodeTemplate)
        lpControls = (static_cast<DSSRWTemplateNode*>(ipNode))->getControls();
    else if(ipNode->getType() == DssRWNodeControl)
        lpControls = (static_cast<DSSRWControlNode*>(ipNode))->getControls();
    else
        lpControls = NULL;
    
    if(lpControls)
    {
        for(int i = 0; i < lpControls->Count(); i++)
        {
            DSSRWControl* lpControl = lpControls->Item(i);
            lpControl->UpdateTargets(NULL, &mKeyMap);
        }
    }
    
    for (int i = 0; i < ipNode->Count(); i++)
    {
        hCheckTargetKeys(ipNode->Item(i));
    }
}

int DSSCManipulationDuplicateLayout::Execute()
{
    UpdateModel();
    
    // node change
    hAddNodesChanged(mpDstLayoutNode);
    
    return S_OK;
}

void DSSCManipulationDuplicateLayout::hAddNodesChanged(DSSRWNode *ipNode)
{
    mNodesModelChanged.push_back(ipNode);
    
    mNodeMapServer[ipNode->getKey()] = ipNode;
    
    std::vector<std::string> lNodes = ipNode->GetServerNodeKeys();
    int lSize = lNodes.size();
    for(int i = 0; i < lSize; i++)
    {
        std::string lKey = lNodes[i];
        if(mNodeMapServer.find(lKey) != mNodeMapServer.end())
            continue;
        DSSRWNode* lNode = ipNode->getObjectContext()->getNode(lKey);
        
        mNodesModelChanged.push_back(lNode);
        mNodeMapServer[lKey] = lNode;
    }
    
    for(int i = 0; i < ipNode->Count(); i++)
    {
        hAddNodesChanged(ipNode->Item(i));
    }
}


void DSSCManipulationDuplicateLayout::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    
}

void DSSCManipulationDuplicateLayout::GenerateAODeltaXML()
{
    
}