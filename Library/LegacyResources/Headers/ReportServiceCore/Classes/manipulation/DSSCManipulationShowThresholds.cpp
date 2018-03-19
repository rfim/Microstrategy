//
//  DSSCManipulationShowThresholds.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 3/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "DSSCManipulationShowThresholds.h"

#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxml.h"
#else
#include "DSSSimpleXML.h"
#endif

DSSCManipulationShowThresholds::DSSCManipulationShowThresholds(DSSRWTemplateNode* ipTemplateNode, DSSTemplateMetric* ipTemplateMetric)
{
    mpTemplateNode = ipTemplateNode;
    mpTemplateMetric = ipTemplateMetric;
    mType = ManipulationShowThresholds;
}

DSSCManipulationShowThresholds::DSSCManipulationShowThresholds(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationShowThresholds;
    
    if(!ipAO->IsDeltaXML())
        return;
    
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
    std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
    hParseAOXML(lXMLString, ipContext);
#endif
}

DSSCManipulationShowThresholds::~DSSCManipulationShowThresholds()
{
    
}

int DSSCManipulationShowThresholds::Execute()
{
    if(mEnabled == false)
        mpTemplateMetric->getThresholds()->setEnabled(true);
    GenerateAODeltaXML();
    return S_OK;
}

#ifndef __ANDROID__
void DSSCManipulationShowThresholds::GenerateAODeltaXML()
{
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("19"));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    
    int lTreeType = static_cast<int>(mpTemplateNode->getTreeType());
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    
    SimpleXMLNode lReportXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation"));
    
    SimpleXMLNode lReportManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation_method"));
    SimpleXMLNode lReportManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr(mEnabled ? "34" : "35", true));
    lReportManipulationMethodXML.AddChild(lReportManipulationMethodXMLContent);
    
    SimpleXMLNode lReportToggleSettingXML(SimpleXMLBuilder::CreateNodePtr("kt"));
    SimpleXMLNode lReportToggleSettingXMLContent(SimpleXMLBuilder::CreateNodePtr("1", true));
    lReportToggleSettingXML.AddChild(lReportToggleSettingXMLContent);
    
    lReportXML.AddChild(lReportManipulationMethodXML);
    lReportXML.AddChild(lReportToggleSettingXML);
    
    lXML.AddChild(lManipulationMethodXML);
    lXML.AddChild(lTreeTypeXML);
    lXML.AddChild(lNodeKeyXML);
    lXML.AddChild(lReportXML);
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    mpActionObject = new DSSAODeltaXML(ManipulationShowThresholds);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationShowThresholds::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
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
                    if(lCurrentReportNode.IsElementNode() && lCurrentReportNode.GetName().compare("report_manipulation_method"));
                    {
                        lCurrentReportNode.GetContent(lStrContent);
                        if(lStrContent.compare("34") == 0)
                            mEnabled = true;
                        else if(lStrContent.compare("35") == 0)
                            mEnabled = false;
                    }
                }
            }
        }
    }

}
#endif