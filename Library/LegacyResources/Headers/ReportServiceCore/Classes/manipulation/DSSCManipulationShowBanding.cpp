//
//  DSSCManipulationShowBanding.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 4/15/13.
//
//

#include "DSSCManipulationShowBanding.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif
#include "DSSAODeltaXML.h"

DSSCManipulationShowBanding::DSSCManipulationShowBanding(DSSRWTemplateNode* ipTemplateNode, bool iBandingEnabled): DSSCManipulation(), mpTemplateNode(ipTemplateNode), mBandingEnabled(iBandingEnabled)
{
    mType = ManipulationShowBanding;
}

DSSCManipulationShowBanding::DSSCManipulationShowBanding(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    if (!ipAO->IsDeltaXML())
        return;
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();
    hParseAOXML(lXMLString, ipContext);
#endif
    mType = ManipulationShowBanding;
}

DSSCManipulationShowBanding::~DSSCManipulationShowBanding()
{
    
}

int DSSCManipulationShowBanding::Execute()
{
    if(mpTemplateNode && mpTemplateNode->getTemplate())
        mpTemplateNode->getTemplate()->setBandingHidden(!mBandingEnabled);
    GenerateAODeltaXML();
    mNodesModelChanged.push_back(mpTemplateNode);
    return S_OK;
}

DSSRWTemplateNode* DSSCManipulationShowBanding::getTemplateNode()
{
    return mpTemplateNode;
}

#ifndef __ANDROID__
void DSSCManipulationShowBanding::GenerateAODeltaXML()
{
    mpActionObject = new DSSAODeltaXML(ManipulationShowBanding);
    
    SimpleXMLBuilder lBuilder;
    
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationXMLContent(SimpleXMLBuilder::CreateNodePtr("19", true));
    lManipulationXML.AddChild(lManipulationXMLContent);
    lXML.AddChild(lManipulationXML);
    
    EnumDSSRWTreeType lTreeType = mpTemplateNode->getTreeType();
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    lXML.AddChild(lTreeTypeXML);
    
    std::string lStr = mpTemplateNode->getKey();
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lStr.c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    lXML.AddChild(lNodeKeyXML);
    
    std::string lShowBanding = mBandingEnabled ? "33" : "32";
    SimpleXMLNode lRptManipulationXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation"));
    SimpleXMLNode lRptShowBandingXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation_method"));
    SimpleXMLNode lRptShowBandingXMContent(SimpleXMLBuilder::CreateNodePtr(lShowBanding.c_str(), true));
    lRptShowBandingXML.AddChild(lRptShowBandingXMContent);
    lRptManipulationXML.AddChild(lRptShowBandingXML);
    lXML.AddChild(lRptShowBandingXML);
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    std::string lXMLString = lBuilder.ToString(false);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationShowBanding::hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext)
{
    SimpleXMLParser lParser(iXMLString);
	if (!lParser.IsValid())
		return;
	
	SimpleXMLNode lCurrentNode = lParser.GetRoot();
	
	if (!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
		return;
	
	SimpleXMLNode lRptManipulation;
	std::string lstrContent;
	for (lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
	{
		if (lCurrentNode.IsElementNode())
		{
			if (lCurrentNode.GetName().compare("rw_node_key") == 0) // template node
			{
				lCurrentNode.GetContent(lstrContent);
                mpTemplateNode = (DSSRWTemplateNode*)(ipContext->getNode(lstrContent));
			}
			else if (lCurrentNode.GetName().compare("report_manipulation") == 0) // report manipulation root
			{
                lRptManipulation = lCurrentNode;
			}
		}
	}
    
    // parse report manipulation
    GUID lTempID;
    for (lCurrentNode = lRptManipulation.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if (lCurrentNode.IsElementNode())
		{
			if (lCurrentNode.GetName().compare("report_manipulation_method") == 0)
			{
				lCurrentNode.GetContent(lstrContent);
                DSSBaseElementProxy::ConvertFromStringToGUID(lstrContent, lTempID);
                
                if(lstrContent.compare("32") == 0)
                    mBandingEnabled = true;
                else
                    mBandingEnabled = false;
			}
		}
    }
}
#endif