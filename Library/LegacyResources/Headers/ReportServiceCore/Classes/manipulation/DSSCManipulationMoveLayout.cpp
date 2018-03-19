//
//  DSSCManipulationMoveLayout.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 4/24/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#include "DSSCManipulationMoveLayout.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif
#include "DSSBaseElementProxy.h"

DSSCManipulationMoveLayout::DSSCManipulationMoveLayout(DSSRWSectionNode* ipLayoutNode, bool iIsMoveRight, int iNewIndex)
{
    mpLayoutNode = ipLayoutNode;
    DSSRWNode* lpParentNode = ipLayoutNode->getParentNode();
    int lLayoutCount = lpParentNode->Count();
    if(iNewIndex >= 0)
    {
        mNewIndex = iNewIndex;
    }
    else
    {
        if(iIsMoveRight)
            mNewIndex = (iNewIndex + 1) % lLayoutCount;
        else
            mNewIndex = (iNewIndex + lLayoutCount - 1) % lLayoutCount;
    }
    
    mType = ManipulationMoveLayout;
}

DSSCManipulationMoveLayout::DSSCManipulationMoveLayout(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationMoveLayout;
    
    if(!ipAO->IsDeltaXML())
        return;
    
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
    std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
    hParseAOXML(lXMLString, ipContext);
#endif
}

DSSCManipulationMoveLayout::~DSSCManipulationMoveLayout()
{
    
}

int DSSCManipulationMoveLayout::Execute()
{
    UpdateModel();
    GenerateAODeltaXML();
    mNodesModelChanged.push_back(mpLayoutNode);
    return S_OK;
}

void DSSCManipulationMoveLayout::UpdateModel()
{
    DSSRWNode* lpParentNode = mpLayoutNode->getParentNode();
    int lLayoutCount = lpParentNode->Count();
    if(lLayoutCount <= 0 || mNewIndex < 0 || mNewIndex >= lLayoutCount)
        return;
    
    int lIndex = mpLayoutNode->getIndex();
    lpParentNode->MoveChild(mNewIndex, lIndex);
}

DSSRWSectionNode* DSSCManipulationMoveLayout::getLayoutNode()
{
    return mpLayoutNode;
}

#ifndef __ANDROID__
void DSSCManipulationMoveLayout::GenerateAODeltaXML()
{
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("30", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr("1", true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpLayoutNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    
    SimpleXMLNode lTreeTypeDestXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type_dest"));
    SimpleXMLNode lTreeTypeDestXMLContent(SimpleXMLBuilder::CreateNodePtr("1", true));
    lTreeTypeDestXML.AddChild(lTreeTypeDestXMLContent);
    
    SimpleXMLNode lNodeKeyParentXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key_parent"));
    SimpleXMLNode lNodeKeyParentXMLContent(SimpleXMLBuilder::CreateNodePtr(mpLayoutNode->getParentNode()->getKey().c_str(), true));
    lNodeKeyParentXML.AddChild(lNodeKeyParentXMLContent);
    
    SimpleXMLNode lIndexXML(SimpleXMLBuilder::CreateNodePtr("index"));
    SimpleXMLNode lIndexXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(mNewIndex + 1).c_str(), true));
    lIndexXML.AddChild(lIndexXMLContent);
    
    lXML.AddChild(lManipulationMethodXML);
    lXML.AddChild(lTreeTypeXML);
    lXML.AddChild(lNodeKeyXML);
    lXML.AddChild(lTreeTypeDestXML);
    lXML.AddChild(lNodeKeyParentXML);
    lXML.AddChild(lIndexXML);
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    
    mpActionObject = new DSSAODeltaXML(ManipulationMoveLayout);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationMoveLayout::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser(iXMLString);
    if(!lParser.IsValid())
        return;
    SimpleXMLNode lCurrentNode(lParser.GetRoot());
    if(!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
        return;
    
    std::string lStrContent;
    SimpleXMLNode lMiNode;
    for(lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if(lCurrentNode.IsElementNode())
        {
            if(lCurrentNode.GetName().compare("rw_node_key") == 0)
            {
                lCurrentNode.GetContent(lStrContent);
                mpLayoutNode = static_cast<DSSRWSectionNode*>(ipContext->getNode(lStrContent));
            }
            else if(lCurrentNode.GetName().compare("index") == 0)
            {
                lCurrentNode.GetContent(lStrContent);
                mNewIndex = DSSExpNode::StringToInt(lStrContent)-1;
            }
        }
    }
}
#endif