//
//  DSSCManipulationSetGraphBinary.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 5/18/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#include "DSSCManipulationSetGraphBinary.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif

DSSCManipulationSetGraphBinary::DSSCManipulationSetGraphBinary(DSSRWTemplateNode* ipTemplateNode, DSSGraphProperties* ipGraphProperties, EnumGraphMajorType iGraphMajorType, int iGraphSubtype, EnumDSSGraphType iGraphType)
{
    mpTemplateNode = ipTemplateNode;
    mpGraphProperties = ipGraphProperties;
    mGraphMajorType = iGraphMajorType;
    mGraphSubtype = iGraphSubtype;
    mGraphType = iGraphType;
    
    mType = ManipulationSetGraphBinary;
}

DSSCManipulationSetGraphBinary::DSSCManipulationSetGraphBinary(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationSetGraphBinary;
    
    if(!ipAO->IsDeltaXML())
        return;
    
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
    std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
    hParseXML(lXMLString, ipContext);
#endif
}

DSSCManipulationSetGraphBinary::~DSSCManipulationSetGraphBinary()
{

}

int DSSCManipulationSetGraphBinary::Execute()
{
    DSSGraphProperties* lpGraphProperties = mpTemplateNode->getGraphProperties();
    if(mpGraphProperties)
    {
        if(lpGraphProperties && lpGraphProperties != mpGraphProperties)
            delete lpGraphProperties;
    }
    else
    {
        if(!lpGraphProperties)
            mpGraphProperties = new DSSGraphProperties();
        else
            mpGraphProperties = lpGraphProperties;
    }
    
    mpGraphProperties->setGraphType(mGraphMajorType);
    mpGraphProperties->setGraphSubType(mGraphSubtype);
    mpTemplateNode->setGraphProperties(mpGraphProperties);

    DSSTemplate* lpTemplate = mpTemplateNode->getTemplate();
    CComVariant lGraphType = CComVariant(static_cast<int>(mGraphType));
    lpTemplate->setPropertyValue("GraphInfo", "GraphType", lGraphType);
    
    mNodesModelChanged.push_back(mpTemplateNode);
    
    GenerateAODeltaXML();
    return S_OK;
}

DSSRWTemplateNode* DSSCManipulationSetGraphBinary::getTemplateNode()
{
    return mpTemplateNode;
}

void DSSCManipulationSetGraphBinary::GenerateAODeltaXML()
{
    SimpleXMLBuilder lBuilder;
    
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("37", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    lXML.AddChild(lManipulationMethodXML);
    
    int lTreeType = static_cast<EnumDSSRWTreeType>(mpTemplateNode->getTreeType());
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    lXML.AddChild(lTreeTypeXML);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    lXML.AddChild(lNodeKeyXML);
    
    SimpleXMLNode lGraphPropertiesXML(SimpleXMLBuilder::CreateNodePtr("gpp"));
    lGraphPropertiesXML.SetProperty("gat", DSSExpNode::IntToString(static_cast<int>(mGraphType)).c_str());
    lGraphPropertiesXML.SetProperty("git", DSSExpNode::IntToString(mGraphSubtype).c_str());
    lXML.AddChild(lGraphPropertiesXML);
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    mpActionObject = new DSSAODeltaXML(ManipulationSetGraphBinary);
    
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}
#ifndef __ANDROID__
void DSSCManipulationSetGraphBinary::hParseXML(std::string &iXMLString, DSSObjectContext *ipContext)
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
                mpTemplateNode = static_cast<DSSRWTemplateNode*>(ipContext->getNode(lStrContent));
            }
            else if(lCurrentNode.GetName().compare("gpp") == 0)
            {
                lCurrentNode.GetProperty("gat", lStrContent);
                mGraphMajorType = static_cast<EnumGraphMajorType>(DSSExpNode::StringToInt(lStrContent));;
                lCurrentNode.GetProperty("git", lStrContent);
                mGraphSubtype = DSSExpNode::StringToInt(lStrContent);
            }
        }
    }
}
#endif