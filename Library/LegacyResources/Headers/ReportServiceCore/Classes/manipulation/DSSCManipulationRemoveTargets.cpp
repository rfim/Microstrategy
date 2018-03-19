//
//  DSSCManipulationRemoveTargets.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "DSSCManipulationRemoveTargets.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxml.h"
#else
#include "DSSSimpleXML.h"
#endif
#include "DSSBaseElementProxy.h"

DSSCManipulationRemoveTargets::DSSCManipulationRemoveTargets(DSSRWControlNode* ipControlNode, DSSRWControl* ipControl, std::vector<std::string> iTargetKeys): DSSCManipulationRemoveControl(static_cast<DSSRWNode*>(ipControlNode))
{
    mpControl = ipControl;
    mTargetKeys = iTargetKeys;
    
    mType = ManipulationRemoveTargets;
}

DSSCManipulationRemoveTargets::DSSCManipulationRemoveTargets(DSSActionObject* ipAO, DSSObjectContext* ipContext): DSSCManipulationRemoveControl(ipAO, ipContext)
{
    mType = ManipulationRemoveTargets;
    
    if (!ipAO->IsDeltaXML())
        return;
    
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();
    
    hParseAOXML(lXMLString, ipContext);
#endif
}

DSSCManipulationRemoveTargets::~DSSCManipulationRemoveTargets()
{
    
}

int DSSCManipulationRemoveTargets::Execute()
{
    for(std::vector<std::string>::iterator lIter = mTargetKeys.begin(); lIter != mTargetKeys.end(); lIter++)
    {
    //    RemoveControlTarget(*lIter);
    }
    mNodesModelChanged.push_back(mpNode);
    GenerateAODeltaXML();
    return S_OK;
}
#ifndef __ANDROID__
void DSSCManipulationRemoveTargets::GenerateAODeltaXML()
{
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulations"));
    
    for(int i = 0; i < mpControl->getCountTargets(); i++)
    {
        SimpleXMLNode lRemoveTargetXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
        SimpleXMLNode lRTManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
        SimpleXMLNode lRTManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("68", true));
        lRTManipulationMethodXML.AddChild(lRTManipulationMethodXML);
        
        EnumDSSRWTreeType lRTTreeType = mpNode->getTreeType();
        SimpleXMLNode lRTTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lRTTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lRTTreeType).c_str(), true));
        lRTTreeTypeXML.AddChild(lRTTreeTypeXMLContent);
        
        SimpleXMLNode lRTNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lRTNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpNode->getKey().c_str(), true));
        lRTNodeKeyXML.AddChild(lRTNodeKeyXMLContent);
        
        SimpleXMLNode lRTControlKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_control_key"));
        SimpleXMLNode lRTControlKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpControl->getKey().c_str(), true));
        lRTControlKeyXML.AddChild(lRTControlKeyXMLContent);
        
        std::string lTargetKey = mpControl->getTargetKey(i);
        SimpleXMLNode lRTTargetKeyXML(SimpleXMLBuilder::CreateNodePtr("target_key"));
        SimpleXMLNode lRTTargetKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lTargetKey.c_str(), true));
        
        lRemoveTargetXML.AddChild(lRTManipulationMethodXML);
        lRemoveTargetXML.AddChild(lRTTreeTypeXML);
        lRemoveTargetXML.AddChild(lRTNodeKeyXML);
        lRemoveTargetXML.AddChild(lRTControlKeyXML);
        lRemoveTargetXML.AddChild(lRTTargetKeyXML);
        lXML.AddChild(lRemoveTargetXML);
        
        
        // remove cgb?
    }
    mpActionObject = new DSSAODeltaXML(ManipulationRemoveTargets);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationRemoveTargets::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser(iXMLString);
    if(!lParser.IsValid())
    {
        return;
    }
    SimpleXMLNode lRootNode = lParser.GetRoot();
    SimpleXMLNode lCurrentNode;
    if(!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
    {
        return ;
    }
    std::string lCurrentContent;
    for(lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if(lCurrentNode.IsElementNode())
        {
            if(lCurrentNode.GetName().compare("rw_node_key") == 0)
            {
                lCurrentNode.GetContent(lCurrentContent);
                mpNode = ipContext->getNode(lCurrentContent);
            }
            else if(lCurrentNode.GetName().compare("rw_control_key") == 0)
            {
                lCurrentNode.GetContent(lCurrentContent);
                mpControl = ipContext->getControl(lCurrentContent);
            }
        }
    }
}
#else
void DSSCManipulationRemoveTargets::GenerateAODeltaXML()
{
    TiXmlDocument lDoc;
    TiXmlElement* lRootElement;
    
    for(int i = 0; i < mpControl->getCountTargets(); i++)
    {
        TiXmlElement* lRemoveTargetElement = new TiXmlElement("rw_manipulation");
        
        TiXmlElement* lRTManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
        TiXmlText* lRTManipulationMethodText = new TiXmlText("68");
        lRTManipulationMethodElement->LinkEndChild(lRTManipulationMethodeText);
        
        EnumDSSRWTreeType lRTTreeType = mpNode->getTreeType();
        TiXmlElement* lRTTreeTypeElement = new TiXmlElement("rw_tree_type");
        TiXmlText* lRTTreeTypeText = new TiXmlText(DSSExpNode::IntToString(lRTTreeType).c_str());
        lRTTreeTypeElement->LinkEndChild(lRTTreeTypeText);
        
        TiXmlElement* lRTNodeKeyElement = new TiXmlElement("rw_node_key");
        TiXmlText* lRTNodeKeyText = new TiXmlText(mpNode->getKey().c_str());
        lRTNodeKeyElement->LinkEndChild(lRTNodeKeyText);
        
        TiXmlElement* lRTControlKeyElement = new TiXmlElement("rw_control_key");
        TiXmlText* lRTControlKeyText = new TiXmlText(mpControl->getKey().c_str());
        lRTControlKeyElement->LinkEndChild(lRTControlKeyText);
        
        std::string lTargetKey = mpControl->getTargetKey(i);
        TiXmlElement* lRTTargetKeyElement = new TiXmlElement("target_key");
        TiXmlText* lRTControlKeyText = new TiXmlText(lTargetKey.c_str());
        
        lRemoveTargetElement->LinkEndChild(lRTManipulationMethodElement);
        lRemoveTargetElement->LinkEndChild(lRTTreeTypeElement);
        lRemoveTargetElement->LinkEndChild(lRTNodeKeyElement);
        lRemoveTargetElement->LinkEndChild(lRTControlKeyElement);
        lRemoveTargetElement->LinkEndChild(lRTTargetKeyElement);
        
        lRootElement->LinkEndChild(lRemoveTargetElement);
    }
    TiXmlPrinter lPrinter;
    lPrinter.SetIndent(NULL);
    lPrinter.SetLineBreak(NULL);
    lDoc.Accept(&lPrinter);
    
    mpActionObject = new DSSAODeltaXML(ManipulationRemoveTargets);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lPrinter.Str());
}
#endif