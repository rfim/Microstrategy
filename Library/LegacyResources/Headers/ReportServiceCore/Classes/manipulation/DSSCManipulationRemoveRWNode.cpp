//
//  DSSCManipulationRemonvRWNode.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 1/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "DSSCManipulationRemoveRWNode.h"
#include "DSSAODeltaXML.h"
#include "DSSRWSectionNode.h"
#include "DSSRWControls.h"
#include "DSSRWControl.h"
#include "DSSRWControlNode.h"
#include "DSSRWTemplateNode.h"
#include "DSSExpNode.h"
#include "DSSDocumentDefinition.h"
#include "DSSRWIterator.h"
#include <string>
#include <sstream>
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif

DSSCManipulationRemoveRWNode::DSSCManipulationRemoveRWNode(DSSRWNode* ipNode)
{
    mpNode = ipNode;
    
    mType = ManipulationRemoveRWNode;
}

DSSCManipulationRemoveRWNode::DSSCManipulationRemoveRWNode(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationRemoveRWNode;
    
    if(!ipAO->IsDeltaXML())
    {
        return;
    }
#ifndef __ANDROID__
    std::string lXMLString = (static_cast<DSSAODeltaXML*>(ipAO))->getXML();
    hParseAOXML(lXMLString, ipContext);
#endif
}

DSSCManipulationRemoveRWNode::~DSSCManipulationRemoveRWNode()
{
    int i;
    DSSObjectContext* lpObjectContext = mpNode->getObjectContext();
    for (i=0; i<mNodesRemoved.size(); i++)
    {
        DSSRWNode* lpNode = mNodesRemoved[i];
        lpObjectContext->RemoveNode(lpNode->getKey());
        delete lpNode;  //delete node
    }
    
    for (i=0; i<mIteratorsRemoved.size(); i++)
    {
        DSSRWIterator* lpIterator = mIteratorsRemoved[i];
        delete lpIterator;
    }
}
void DSSCManipulationRemoveRWNode::RemoveRWNode()
{
    DSSRWNode* lpNode = mpNode;
    DSSRWNode* lpParentNode;
    do {
//        mpNode->getObjectContext()->UpdateIteratorRemoveNode(mpNode);
        
        lpParentNode = mpNode->getParentNode();
        if(lpParentNode)
        {
            int lIndex = lpNode->getIndex();
            lpParentNode->RemoveChild(lIndex, &mNodesRemoved, &mIteratorsRemoved);
            
            int lNewIndex = lIndex;
            if(lNewIndex > lpParentNode->Count())
                lNewIndex--;
            
            if(lNewIndex >= 0)
            {
                (static_cast<DSSRWSectionNode*>(lpParentNode))->setCurrentNodeIndex(lNewIndex);
                DSSRWNode* lpNewNode = lpParentNode->Item(lNewIndex);
                (static_cast<DSSRWSectionNode*>(lpParentNode))->setCurrentNodeKey(lpNewNode->getKey());
            }
        }
        else
        {
            // root node
            DSSDocumentDefinition* lpDoc = lpNode->getObjectContext()->getDocumentDefinition();
            for(int i = 0; i < DssRWTreeTypeLastOne; i++)
            {
                if(lpNode == lpDoc->getRootNode(static_cast<EnumDSSRWTreeType>(i)))
                {
                    lpDoc->ClearRootNode(static_cast<EnumDSSRWTreeType>(i), &mNodesRemoved, &mIteratorsRemoved);
                }
            }
        }
        
        // if mpNode is UC or CGB, we should remove the selector targeted on it
        if(lpNode->isUCorCGB())
        {
            DSSObjectContext* lpObjectContext = lpNode->getObjectContext();
            MapFromTargetToControl* lMapFromTargetToControl = lpObjectContext->getMapFromTargetToControl();
            NodeAndControlsVector lNodeAndControlsVector = (*lMapFromTargetToControl)[lpNode->getKey()];
            for(NodeAndControlsVector::iterator lIter = lNodeAndControlsVector.begin(); lIter != lNodeAndControlsVector.end(); lIter++)
            {
                NodeAndControls* lNodeAndControls = *lIter;
                std::vector<std::string> lControlKeys = lNodeAndControls->ControlKeys;
                for(std::vector<std::string>::iterator lControlIter = lControlKeys.begin(); lControlIter != lControlKeys.end(); lControlIter++)
                {
                    DSSRWControl* lpControl = lpObjectContext->getControl(*lControlIter);
                    for(int i = 0; i < lpControl->getCountTargets(); i++)
                    {
                        if(lpControl->getTargetKey(i) == lpNode->getKey())
                            lpControl->removeTarget(i);
                    }
                }
            }
            lpObjectContext->RemoveTargetToNodeAndControls(lpNode->getKey());
        }
    } while (lpParentNode == searchGroupByNodes(lpParentNode));
    
}

#ifndef __ANDROID__
void DSSCManipulationRemoveRWNode::GenerateAODeltaXML()
{
    // DSSObjectContext* lContext = mpNode->getObjectContext();

    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("5", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    
    int lTreeType = static_cast<int>(mpNode->getTreeType());
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    
    lXML.AddChild(lManipulationMethodXML);
    lXML.AddChild(lTreeTypeXML);
    lXML.AddChild(lNodeKeyXML);
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    
    std::string lstrXML = lBuilder.ToString(false);
    if (!mpActionObject)
        mpActionObject = new DSSAODeltaXML(ManipulationRemoveRWNode);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->appendXMLString(lstrXML);
    
    //mpActionObject = new DSSAODeltaXML(ManipulationRemoveRWNode);
    //(static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}
void DSSCManipulationRemoveRWNode::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser(iXMLString);
    if(!lParser.IsValid())
    {
        return;
    }
    SimpleXMLNode lCurrentNode = lParser.GetRoot();
    if (!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0) 
    {
        return;
    }
    
    std::string lStrContent;
    for(lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if(lCurrentNode.IsElementNode())
        {
            if(lCurrentNode.GetName().compare("rw_node_key") == 0)
            {
                lCurrentNode.GetContent(lStrContent);
                mpNode = ipContext->getNode(lStrContent);
            }
        }
    }
}
#else
void DSSCManipulationRemoveRWNode::GenerateAODeltaXML()
{
    DSSObjectContext* lContext = mpNode->getObjectContext();
    TiXmlDocument lDoc;
    TiXmlElement* lpRootElement = new TiXmlElement("rw_manipulation");
    
    TiXmlElement* lpManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
    TiXmlText* lpManipulationMethodText = new TiXmlText("5");
    lpManipulationMethodElement->LinkEndChild(TiXmlText);
    
    std::stringstream lStream;
    int lTreeType = static_cast<int>(mpNode->getTreeType());
    lStream >> lTreeType;
    std::string lStrTreeType = lStream.str();
    lStream.clear();
    lStream.str(std::string());
    TiXmlElement* lpTreeTypeElement = new TiXmlElement("rw_tree_type");
    TiXmlText* lpTreeTypeText = new TiXmlText(lStrTreeType.c_str());
    lpTreeTypeElement->LinkEndChild(lpTreeTypeText);
    
    TiXmlElement* lNodeKeyElement = new TiXmlElement("rw_node_key");
    TiXmlText* lNodeKeyText = new TiXmlText(mpNode->getKey().c_str());
    lNodeKeyElement->LinkEndChild(lNodeKeyText);
    
    TiXmlPrinter lPrinter;
    lPrinter.SetIndent(NULL);
    lPrinter.SetLineBreak(NULL);
    lDoc.Accept(&lPrinter);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lPrinter.Str());
}
#endif

int DSSCManipulationRemoveRWNode::Execute()
{
    GenerateAODeltaXML();
    //mNodesModelChanged.push_back(mpNode->getParentNode());
    RemoveRWNode();
    return S_OK;
}



