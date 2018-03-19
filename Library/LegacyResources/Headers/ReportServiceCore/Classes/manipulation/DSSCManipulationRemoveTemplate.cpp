//
//  DSSCManipulationRemoveTemplate.cpp
//  ReportServiceCore
//
//  Created by Zhu, Wei on 4/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "DSSCManipulationRemoveTemplate.h"
#include "DSSRWTemplateNode.h"
#include "DSSAODeltaXML.h"

#ifdef __ANDROID__
#include "TinyXml/tinyxml.h"
#else
#include "DSSSimpleXML.h"
#endif

DSSCManipulationRemoveTemplate::DSSCManipulationRemoveTemplate(DSSRWNode* ipNode) : DSSCManipulationRemoveRWNode(ipNode)
{
    mType = ManipulationRemoveTemplate;
}

DSSCManipulationRemoveTemplate::~DSSCManipulationRemoveTemplate()
{
}

int DSSCManipulationRemoveTemplate::Execute()
{
    if(!mpNode || mpNode->getType() != DssRWNodeTemplate)
        return S_FALSE;
    
    DSSObjectContext* lObjectContext = mpNode->getObjectContext();
    DSSRWControls* lControls = ((DSSRWTemplateNode*)mpNode)->getControls();
    
    for (int i = 0; i < lControls->Count(); i++)
    {
        DSSRWControl* lControl = lControls->Item(i);
        MapFromTargetToControl* lMapFromTargetToControl = lObjectContext->getMapFromTargetToControl();
        int lCount = lControl->getCountTargets();
        
        for (int j = 0; j < lCount; j++)
        {
            std::string lTargetKey = lControl->getTargetKey(j);
            NodeAndControlsVector lTargetArray = (*lMapFromTargetToControl)[lTargetKey];
            
            int lTargetCount = lTargetArray.size();
            for (int k = 0; k < lTargetCount; k++)
            {
                if (lTargetArray[k]->NodeKey == mpNode->getKey())
                {
                    lTargetArray.erase(k+lTargetArray.begin());
                    
                    if (mNodeMapServer.find(lTargetKey) == mNodeMapServer.end())
                    {
                        DSSRWNode* lNode = lObjectContext->getNode(lTargetKey);
                        
                        mNodesModelChanged.push_back(lNode);
                        mNodeMapServer[lTargetKey] = lNode;
                    }
                    break;
                }
            }
            
            if (lTargetArray.size() == 0)
            {
                //Remove the target node
                DSSRWNode* lTargetNode = lObjectContext->getNode(lTargetKey);
                lTargetNode->getParentNode()->RemoveGroupByNode(lTargetNode->getIndex(), &mNodesRemoved, &mIteratorsRemoved);
                lMapFromTargetToControl->erase(lTargetKey);
            }
        }
    }
    
    // Remove the node
    DSSCManipulationRemoveRWNode::Execute();
    
    //Cao,Lei 12.14 2010
    //push the key of metric selector controlling the template node to dataRetrievalKeys
    AddMetricConditionSelectorsToMap(mpNode);
    
    GUID lID = ((DSSRWTemplateNode*)mpNode)->getTemplate()->getID();
    lObjectContext->RemoveObjectByID(lID);
    
    // persist changes
    GenerateAODeltaXML();
    
    return S_OK;
}

#ifndef __ANDROID__
void DSSCManipulationRemoveTemplate::GenerateAODeltaXML()
{
    DSSRWControls* lControls = ((DSSRWTemplateNode*)mpNode)->getControls();
    
    for (int j = 0; j < lControls->Count(); j++)
    {
        DSSRWControl* lControl = lControls->Item(j);
        for (int i = 0; i < lControl->getCountTargets(); i++)
        {
            std::string lTargetKey = lControl->getTargetKey(i);
            
            SimpleXMLNode lXML1(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
            SimpleXMLNode lManipulationMethodXML1(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
            SimpleXMLNode lManipulationMethodXMLContent1(SimpleXMLBuilder::CreateNodePtr("68", true));
            lManipulationMethodXML1.AddChild(lManipulationMethodXMLContent1);
            
            std::stringstream lStream;
            int lTreeType1 = static_cast<int>(mpNode->getTreeType());
            lStream >> lTreeType1;
            std::string lStrTreeType1 = lStream.str();
            lStream.clear();
            lStream.str(std::string());
            SimpleXMLNode lTreeTypeXML1(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
            SimpleXMLNode lTreeTypeXMLContent1(SimpleXMLBuilder::CreateNodePtr(lStrTreeType1.c_str(), true));
            lTreeTypeXML1.AddChild(lTreeTypeXMLContent1);
            
            SimpleXMLNode lNodeKeyXML1(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
            SimpleXMLNode lNodeKeyXMLContent1(SimpleXMLBuilder::CreateNodePtr(mpNode->getKey().c_str(), true));
            lNodeKeyXML1.AddChild(lNodeKeyXMLContent1);
            
            SimpleXMLNode lControlKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_control_key"));
            SimpleXMLNode lControlKeyContent(SimpleXMLBuilder::CreateNodePtr(lControl->getKey().c_str(), true));
            lControlKeyXML.AddChild(lControlKeyContent);
            
            SimpleXMLNode lTargetKeyXML(SimpleXMLBuilder::CreateNodePtr("target_key"));
            SimpleXMLNode lTargetKeyContent(SimpleXMLBuilder::CreateNodePtr(lTargetKey.c_str(), true));
            lTargetKeyXML.AddChild(lTargetKeyContent);
            
            lXML1.AddChild(lManipulationMethodXML1);
            lXML1.AddChild(lTreeTypeXML1);
            lXML1.AddChild(lNodeKeyXML1);
            lXML1.AddChild(lControlKeyXML);
            lXML1.AddChild(lTargetKeyXML);
            
            DSSAODeltaXML* lpActionObject1 = new DSSAODeltaXML(ManipulationRemoveTemplate);
            SimpleXMLBuilder lBuilder1;
            SimpleXMLNode lRoot1(lBuilder1.GetRoot());
            lRoot1.AddChild(lXML1);
            lpActionObject1->setXMLString(lBuilder1.ToString(false));
            
            mActionObjects.push_back(lpActionObject1);
            
            
            SimpleXMLNode lXML2(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
            SimpleXMLNode lManipulationMethodXML2(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
            SimpleXMLNode lManipulationMethodXMLContent2(SimpleXMLBuilder::CreateNodePtr("66", true));
            lManipulationMethodXML2.AddChild(lManipulationMethodXMLContent2);
            
            SimpleXMLNode lTreeTypeXML2(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
            SimpleXMLNode lTreeTypeXMLContent2(SimpleXMLBuilder::CreateNodePtr(lStrTreeType1.c_str(), true));
            lTreeTypeXML2.AddChild(lTreeTypeXMLContent2);
            
            SimpleXMLNode lNodeKeyXML2(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
            SimpleXMLNode lNodeKeyXMLContent2(SimpleXMLBuilder::CreateNodePtr(lTargetKey.c_str(), true));
            lNodeKeyXML2.AddChild(lNodeKeyXMLContent2);
            
            DSSAODeltaXML* lpActionObject2 = new DSSAODeltaXML(ManipulationRemoveTemplate);
            SimpleXMLBuilder lBuilder2;
            SimpleXMLNode lRoot2(lBuilder2.GetRoot());
            lRoot2.AddChild(lXML2);
            lpActionObject2->setXMLString(lBuilder2.ToString(false));
            
            mActionObjects.push_back(lpActionObject2);
        }
    }
    
    SimpleXMLNode lXML3(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    SimpleXMLNode lManipulationMethodXML3(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent3(SimpleXMLBuilder::CreateNodePtr("5", true));
    lManipulationMethodXML3.AddChild(lManipulationMethodXMLContent3);
    
    SimpleXMLNode lTreeTypeXML3(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent3(SimpleXMLBuilder::CreateNodePtr("1", true));
    lTreeTypeXML3.AddChild(lTreeTypeXMLContent3);
    
    SimpleXMLNode lNodeKeyXML3(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent3(SimpleXMLBuilder::CreateNodePtr(mpNode->getKey().c_str(), true));
    lNodeKeyXML3.AddChild(lNodeKeyXMLContent3);
    
    DSSAODeltaXML* lpActionObject3 = new DSSAODeltaXML(ManipulationRemoveTemplate);
    SimpleXMLBuilder lBuilder3;
    SimpleXMLNode lRoot3(lBuilder3.GetRoot());
    lRoot3.AddChild(lXML3);
    lpActionObject3->setXMLString(lBuilder3.ToString(false));
    
    mActionObjects.push_back(lpActionObject3);
}

void DSSCManipulationRemoveTemplate::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    // replay manipulation - not supported
    _ASSERT(false);
}
#endif