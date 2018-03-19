//
//  DSSCManipulationSetGraphProperties.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 4/18/13.
//
//

#include "DSSCManipulationSetGraphProperties.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxml.h"
#else
#include "DSSSimpleXML.h"
#endif
#include "DSSAODeltaXML.h"

DSSCManipulationSetGraphProperties::DSSCManipulationSetGraphProperties(DSSRWTemplateNode* ipTemplateNode, int iMaxCategories, bool iIsPieGraph): mpTemplateNode(ipTemplateNode), mMaxCategories(iMaxCategories), mIsPieGraph(iIsPieGraph)
{
    mType = ManipulationSetGraphProperties;
}

DSSCManipulationSetGraphProperties::DSSCManipulationSetGraphProperties(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    if (!ipAO->IsDeltaXML())
        return;
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();
    hParseAOXML(lXMLString, ipContext);
#endif
    mType = ManipulationSetGraphProperties;
}

DSSCManipulationSetGraphProperties::~DSSCManipulationSetGraphProperties()
{
    
}

int DSSCManipulationSetGraphProperties::Execute()
{
    if(mpTemplateNode)
    {
        if(mIsPieGraph)
            mpTemplateNode->setMaxSeries(mMaxCategories);
        else
            mpTemplateNode->setMaxCategories(mMaxCategories);
    }
    GenerateAODeltaXML();
    mNodesModelChanged.push_back(mpTemplateNode);
    return S_OK;
}

DSSRWTemplateNode* DSSCManipulationSetGraphProperties::getTemplateNode()
{
    return mpTemplateNode;
}

#ifndef __ANDROID__
void DSSCManipulationSetGraphProperties::GenerateAODeltaXML()
{
    mpActionObject = new DSSAODeltaXML(ManipulationSetGraphProperties);
    
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
    
    SimpleXMLNode lRptManipulationXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation"));
    SimpleXMLNode lRptSetGraphPropertiesXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation_method"));
    SimpleXMLNode lRptSetGraphPropertiesXMContent(SimpleXMLBuilder::CreateNodePtr("38", true));
    lRptSetGraphPropertiesXML.AddChild(lRptSetGraphPropertiesXMContent);
    lRptManipulationXML.AddChild(lRptSetGraphPropertiesXML);
    
    SimpleXMLNode lReportPropertiesXML(SimpleXMLBuilder::CreateNodePtr("properties_xml"));
    
    SimpleXMLNode lPrsXML(SimpleXMLBuilder::CreateNodePtr("prs"));
    lPrsXML.AddProperty("did", "3226ED12769B11D3B91E00C04F2233EA");
    lPrsXML.AddProperty("n", "GraphInfo");
    lPrsXML.AddProperty("dirty", "1");
    
    SimpleXMLNode lPrXML1(SimpleXMLBuilder::CreateNodePtr("pr"));
    lPrXML1.AddProperty("n", "MaxSeries");
    lPrXML1.AddProperty("dt", "3");
    lPrXML1.AddProperty("pdv", "5");
    lPrXML1.AddProperty("pri", "3");
    lPrXML1.AddProperty("pru", "0");
    lPrXML1.AddProperty("dirty", "1");
    lPrXML1.AddProperty("v", "30");
    
    SimpleXMLNode lPrXML2(SimpleXMLBuilder::CreateNodePtr("pr"));
    lPrXML2.AddProperty("n", "MaxCategories");
    lPrXML2.AddProperty("dt", "3");
    lPrXML2.AddProperty("pdv", "6");
    lPrXML2.AddProperty("pri", "4");
    lPrXML2.AddProperty("pru", "0");
    lPrXML2.AddProperty("dirty", "1");
    lPrXML2.AddProperty("v", "30");
    
    if(mIsPieGraph)
        lPrsXML.AddChild(lPrXML1);
    lPrsXML.AddChild(lPrXML2);
    lReportPropertiesXML.AddChild(lPrsXML);    
    lRptManipulationXML.AddChild(lReportPropertiesXML);
    lXML.AddChild(lRptManipulationXML);
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    std::string lXMLString = lBuilder.ToString(false);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationSetGraphProperties::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
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
    SimpleXMLNode lMiNode;
    for (lCurrentNode = lRptManipulation.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if (lCurrentNode.IsElementNode())
		{
			if (lCurrentNode.GetName().compare("properties_xml") == 0)
			{
                lMiNode = lCurrentNode;
			}
		}
    }
    
    std::string lstrPropertySetName;
    std::string lstrPropertyName;
    EnumVariantType lValueType;
    CComVariant lValue;
    SimpleXMLNode lSubNode;
    for (lCurrentNode = lMiNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode.GetNextSibling())
    {
        if (lCurrentNode.IsElementNode())
        {
            if (lCurrentNode.GetName().compare("prs") == 0)
            {
                for (lSubNode=lCurrentNode.GetFirstChild(); lSubNode.IsValid(); lSubNode.GetNextSibling())
                {
                    if (lSubNode.IsElementNode())
                    {
                        if (lSubNode.GetName().compare("pr") == 0)
                        {
                            lSubNode.GetProperty("pri", lstrContent);
                            int lnPri = atoi(lstrContent.c_str());
                            lstrPropertyName = ipContext->getPropertyNameFromID(lstrPropertySetName, lnPri);
                            if(lstrPropertyName.compare("MaxSeries") == 0)
                                mIsPieGraph = true;
                            else
                                mIsPieGraph = false;
                            lValueType = ipContext->getPropertyTypeFromID(lstrPropertySetName, lnPri);
                            lSubNode.GetProperty("v", lstrContent);
                            DSSExpNode::StringToVariant(lstrContent, lValueType, lValue);
                            // for now only maxseries/maxcategories
                            mMaxCategories = lValue.lVal;
                        }
                    }
                }
            }
        }
    }
}
#endif
