//
//  DSSCManipulationAddRWNode.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 1/5/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#include <iostream>
#include "DSSCManipulationAddRWNode.h"
#include "DSSRWTemplateNode.h"

DSSCManipulationAddRWNode::DSSCManipulationAddRWNode(): mpNode(NULL), mpParentNode(NULL)
{
    mpNode = NULL;
    mpParentNode = NULL;
}

DSSCManipulationAddRWNode::DSSCManipulationAddRWNode(EnumDSSRWNodeType iNodeType, DSSRWSectionNode* ipParentNode)
{
    DSSObjectContext* lpContext = ipParentNode->getObjectContext();
    switch (iNodeType)
    {
        case DssRWNodeControl:
            mpNode = new DSSRWControlNode(lpContext);
            break;
        case DssRWNodeTemplate:
            mpNode = new DSSRWTemplateNode(lpContext);
            break;
        case DssRWNodeFieldGroup:
            mpNode = new DSSRWFieldGroupNode(lpContext);
            break;
        case DssRWNodeSection:
            mpNode = new DSSRWSectionNode(lpContext);
            break;
        default:
            break;
    }
    std::string lstrNewNdKey = lpContext->createNodeKey();
    mpNode->Init(ipParentNode, lstrNewNdKey, 0, DssRWTreeTypeContent, iNodeType);
    
   	mpParentNode = ipParentNode;
}

DSSCManipulationAddRWNode::~DSSCManipulationAddRWNode()
{

}

int DSSCManipulationAddRWNode::Execute()
{
    AddRWNode();
    return S_OK;
}

void DSSCManipulationAddRWNode::AddRWNode()
{
    mpParentNode->AddChild(mpNode);
    mpNode->setIndex(mpParentNode->Count() - 1);
    //mpNode->setParentNode(mpParentNode);
    mNodesModelChanged.push_back(mpParentNode);
    //mNodesModelChanged.push_back(mpNode);
    
    mpNode->getObjectContext()->UpdateIteratorAddNode(mpNode);
}

void DSSCManipulationAddRWNode::setName(std::string iName)
{
    mpNode->setName(iName);
    mpNode->getFormat(DssRWNodeFormatNode)->setPropertyValue("FormattingAppearance", "Name", iName);
}

void DSSCManipulationAddRWNode::setTitle(std::string iTitle)
{
    mpNode->getFormat(DssRWNodeFormatNode)->setPropertyValue("FormattingView", "Title", iTitle);
}