//
//  DSSCManipulationEditTemplate.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 5/8/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#include "DSSCManipulationEditTemplate.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxml.h"
#else
#include "DSSSimpleXML.h"
#endif
#include "DSSAODeltaXML.h"

DSSCManipulationEditTemplate::DSSCManipulationEditTemplate(DSSRWTemplateNode* ipTemplateNode, GUID iDataSourceID): mpTemplateNode(ipTemplateNode), mDataSourceID(iDataSourceID)
{
    mType = ManipulationEditTemplateNode;
}

DSSCManipulationEditTemplate::DSSCManipulationEditTemplate(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    if (!ipAO->IsDeltaXML())
        return;
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();
    hParseAOXML(lXMLString, ipContext);
#endif
    mType = ManipulationEditTemplateNode;
}

DSSCManipulationEditTemplate::~DSSCManipulationEditTemplate()
{
    
}

int DSSCManipulationEditTemplate::Execute()
{
    if(mpTemplateNode)
    {
        mpTemplateNode->setDataSourceID(mDataSourceID);
    }
    GenerateAODeltaXML();
    mNodesModelChanged.push_back(mpTemplateNode);
    return S_OK;
}

DSSRWTemplateNode* DSSCManipulationEditTemplate::getTemplateNode()
{
    return mpTemplateNode;
}

#ifndef __ANDROID__
void DSSCManipulationEditTemplate::GenerateAODeltaXML()
{
    mpActionObject = new DSSAODeltaXML(ManipulationEditTemplateNode);
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("8", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    
    int lTreeType = static_cast<int>(mpTemplateNode->getTreeType());
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    
    char lDataSetID[33];
    DSSBaseElementProxy::ConvertGUIDToString(mDataSourceID, lDataSetID);
    SimpleXMLNode lDataSetIDXML(SimpleXMLBuilder::CreateNodePtr("rw_data_set_id"));
    SimpleXMLNode lDataSetIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lDataSetID, true));
    lDataSetIDXML.AddChild(lDataSetIDXMLContent);
    
    int lObjectType = static_cast<EnumDSSObjectType>(mpTemplateNode->getObjectContext()->FindObjectH(mDataSourceID)->getType());
    SimpleXMLNode lDataSetTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_data_set_type"));
    SimpleXMLNode lDataSetTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lObjectType).c_str(), true));
    lDataSetTypeXML.AddChild(lDataSetTypeXMLContent);
    
    lXML.AddChild(lManipulationMethodXML);
    lXML.AddChild(lTreeTypeXML);
    lXML.AddChild(lNodeKeyXML);
    lXML.AddChild(lDataSetIDXML);
    lXML.AddChild(lDataSetTypeXML);
    // lManipulationsXML.AddChild(lXML);
    lRoot.AddChild(lXML);
    
    
    lRoot.AddChild(lXML);
    std::string lXMLString = lBuilder.ToString(false);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationEditTemplate::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
     // not implemented
}
#endif
