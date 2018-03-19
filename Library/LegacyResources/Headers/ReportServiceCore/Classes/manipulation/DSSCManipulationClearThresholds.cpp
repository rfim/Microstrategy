//
//  DSSCManipulationClearThreshods.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 3/19/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#include "DSSCManipulationClearThresholds.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif

DSSCManipulationClearThresholds::DSSCManipulationClearThresholds(DSSRWTemplateNode* ipTemplateNode, DSSTemplateMetric* ipTemplateMetric)
{
    mpTemplateNode = ipTemplateNode;
    mpTemplateMetric = ipTemplateMetric;
    
    mType = ManipulationClearThresholds;
}

DSSCManipulationClearThresholds::DSSCManipulationClearThresholds(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationClearThresholds;
    
    if(!ipAO->IsDeltaXML())
        return;

    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
    std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
    hParseAOXML(lXMLString, ipContext);
#endif
}

DSSCManipulationClearThresholds::~DSSCManipulationClearThresholds()
{
    
}

int DSSCManipulationClearThresholds::Execute()
{
    DSSThresholds* lpThresholds = mpTemplateMetric->getThresholds();
    mCount = lpThresholds->Count();
    lpThresholds->Clear();
    
    mNodesModelChanged.push_back(mpTemplateNode);
    mNodeMapServer[mpTemplateNode->getKey()] = mpTemplateNode;
    
    std::vector<std::string> lNodes = mpTemplateNode->GetServerNodeKeys();
    int lSize = lNodes.size();
    for (int i = 0; i < lSize; i++)
    {
        std::string lKey = lNodes[i];
        if (mNodeMapServer.find(lKey) != mNodeMapServer.end())
            continue;
        
        DSSRWNode* lNode = mpTemplateNode->getObjectContext()->getNode(lKey);
        
        mNodesModelChanged.push_back(lNode);
        mNodeMapServer[lKey] = lNode;
    }
    GenerateAODeltaXML();
    
    return S_OK;
}

#ifndef __ANDROID__
void DSSCManipulationClearThresholds::GenerateAODeltaXML()
{
    if(mCount <= 0)
        return;
    
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("19", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    
    int lTreeType = static_cast<int>(mpTemplateNode->getTreeType());
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    
    lXML.AddChild(lManipulationMethodXML);
    lXML.AddChild(lTreeTypeXML);
    lXML.AddChild(lNodeKeyXML);
    
    for(int i = mCount; i > 0; i--)
    {
        SimpleXMLNode lReportXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation"));
        
        SimpleXMLNode lReportManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation_method"));
        SimpleXMLNode lReportManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("60", true));
        lReportManipulationMethodXML.AddChild(lReportManipulationMethodXMLContent);
        
        char lUnitID[33];
        DSSBaseElementProxy::ConvertGUIDToString(mpTemplateMetric->getID(), lUnitID);
        SimpleXMLNode lReportUnitIDXML(SimpleXMLBuilder::CreateNodePtr("unit_id"));
        SimpleXMLNode lReportUnitIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lUnitID, true));
        lReportUnitIDXML.AddChild(lReportUnitIDXMLContent);
        
        SimpleXMLNode lReportUnitTypeXML(SimpleXMLBuilder::CreateNodePtr("unit_type"));
        SimpleXMLNode lReportUnitTypeXMLContent(SimpleXMLBuilder::CreateNodePtr("4", true));
        lReportUnitTypeXML.AddChild(lReportUnitTypeXMLContent);
        
        SimpleXMLNode lReportThresholdXML(SimpleXMLBuilder::CreateNodePtr("threshold"));
        lReportThresholdXML.AddProperty("ix", DSSExpNode::IntToString(i).c_str());
        
        lReportXML.AddChild(lReportManipulationMethodXML);
        lReportXML.AddChild(lReportUnitIDXML);
        lReportXML.AddChild(lReportUnitTypeXML);
        lReportXML.AddChild(lReportThresholdXML);
        lXML.AddChild(lReportXML);
    }
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    mpActionObject = new DSSAODeltaXML(ManipulationClearThresholds);

    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationClearThresholds::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
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
            else if(lCurrentNode.GetName().compare("report_manipulation") == 0)
            {
                for(SimpleXMLNode lCurrentReportNode = lCurrentNode.GetFirstChild(); lCurrentReportNode.IsValid(); lCurrentReportNode = lCurrentReportNode.GetNextSibling())
                {
                    if(lCurrentReportNode.IsElementNode() && lCurrentReportNode.GetName().compare("unit_id") == 0)
                    {
                        lCurrentReportNode.GetContent(lStrContent);
                        GUID lTemplateMetricGUID;
                        DSSBaseElementProxy::ConvertFromStringToGUID(lStrContent, lTemplateMetricGUID);
                        DSSTemplate* lpTemplate = mpTemplateNode->getTemplate();
                        DSSTemplateUnit* lUnit = lpTemplate->getTemplateUnitFromAxisIndex(DssAxisRows, lTemplateMetricGUID, false);
                        if (lUnit)
                            mpTemplateMetric = static_cast<DSSTemplateMetric*>(lUnit);
                        else
                            mpTemplateMetric = static_cast<DSSTemplateMetric*>(lpTemplate->getTemplateUnitFromAxisIndex(DssAxisColumns, lTemplateMetricGUID, false));
                    }
                }
            }
        }
    }
}
#endif