//
//  DSSCManipulationInsertLayout.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 2/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "DSSCManipulationInsertLayout.h"

#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxml.h"
#else
#include "DSSSimpleXML.h"
#endif

DSSCManipulationInsertLayout::DSSCManipulationInsertLayout(DSSRWSectionNode* ipRootNode, std::string iDocID, int iFlag)
{
    mpParentNode = ipRootNode;
    mDocID = iDocID;
    mFlag = iFlag;
    mLayoutIndex = ipRootNode->Count() + 1;
    mpNode = new DSSRWSectionNode(ipRootNode->getObjectContext());
    mpNode->setKey(ipRootNode->getObjectContext()->createNodeKey());
    (static_cast<DSSRWSectionNode*>(mpNode))->setSectionType(DssRWSectionGeneric);
    
    mType = ManipulationInsertLayout;
}

DSSCManipulationInsertLayout::DSSCManipulationInsertLayout(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationInsertLayout;
    
    if (!ipAO->IsDeltaXML())
        return;
    
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();

    hParseAOXML(lXMLString, ipContext);
#endif
}

DSSCManipulationInsertLayout::~DSSCManipulationInsertLayout()
{
    
}

int DSSCManipulationInsertLayout::Execute()
{
    AddRWNode();
    GenerateAODeltaXML();
    
    mNodesModelChanged.push_back(mpNode);
    mNodeMapServer[mpNode->getKey()] = mpNode;
    
    std::vector<std::string> lNodes = mpNode->GetServerNodeKeys();
    int lSize = lNodes.size();
    for (int i = 0; i < lSize; i++)
    {
        std::string lKey = lNodes[i];
        if (mNodeMapServer.find(lKey) != mNodeMapServer.end())
            continue;
        
        DSSRWNode* lNode = mpNode->getObjectContext()->getNode(lKey);
        
        mNodesModelChanged.push_back(lNode);
        mNodeMapServer[lKey] = lNode;
    }
    
    return S_OK;
}

DSSRWNode* DSSCManipulationInsertLayout::getLayoutNode()
{
    return mpNode;
}

#ifndef __ANDROID__
void DSSCManipulationInsertLayout::GenerateAODeltaXML()
{
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("52", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    
    SimpleXMLNode lDocIDXML(SimpleXMLBuilder::CreateNodePtr("doid"));
    SimpleXMLNode lDocIDXMLContent(SimpleXMLBuilder::CreateNodePtr(mDocID.c_str(), true));
    lDocIDXML.AddChild(lDocIDXMLContent);
    
    SimpleXMLNode lIndexXML(SimpleXMLBuilder::CreateNodePtr("index"));
    SimpleXMLNode lIndexXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(mLayoutIndex).c_str(), true));
    lIndexXML.AddChild(lIndexXMLContent);
    
    SimpleXMLNode lFlagXML(SimpleXMLBuilder::CreateNodePtr("import_rwd_options"));
    SimpleXMLNode lFlagXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(mFlag).c_str(), true));
    lFlagXML.AddChild(lFlagXMLContent);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpLayoutNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    
    lXML.AddChild(lManipulationMethodXML);
    lXML.AddChild(lIndexXML);
    lXML.AddChild(lFlagXML);
    lXML.AddChild(lNodeKeyXML);
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    
    mpActionObject = new DSSAODeltaXML(ManipulationInsertLayout);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}
void DSSCManipulationInsertLayout::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser(iXMLString);
    if(!lParser.IsValid())
        return;
    SimpleXMLNode lCurrentNode(lParser.GetRoot());
    if(!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
        return;
    
    std::string lStrContent;
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
                mLayoutIndex = DSSExpNode::StringToInt(lStrContent);
            }
            else if(lCurrentNode.GetName().compare("doid") == 0)
            {
                lCurrentNode.GetContent(lStrContent);
                mDocID = lStrContent;
            }
            else if(lCurrentNode.GetName().compare("import_rwd_options") == 0)
            {
                lCurrentNode.GetContent(lStrContent);
                mFlag = DSSExpNode::StringToInt(lStrContent);
            }
        }
    }
}
#endif