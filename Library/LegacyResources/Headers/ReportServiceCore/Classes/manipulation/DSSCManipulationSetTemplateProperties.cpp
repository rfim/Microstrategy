//
//  DSSCManipulationSetTemplateProperties.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 4/15/13.
//
//

#include "DSSCManipulationSetTemplateProperties.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxml.h"
#else
#include "DSSSimpleXML.h"
#endif
#include "DSSAODeltaXML.h"
#include <set>

DSSCManipulationSetTemplateProperties::DSSCManipulationSetTemplateProperties(DSSRWTemplateNode* ipNode) : DSSCManipulationSetProperties(ipNode, NULL)
{
    if(mpNode && mpNode->getType() == DssRWNodeTemplate)
        mpTemplate = (static_cast<DSSRWTemplateNode*>(mpNode))->getTemplate();
    mType = ManipulationSetTemplateProperties;
}

DSSCManipulationSetTemplateProperties::DSSCManipulationSetTemplateProperties(DSSActionObject* ipAO, DSSObjectContext* ipContext) : DSSCManipulationSetProperties(ipAO, ipContext)
{
    if (!ipAO->IsDeltaXML())
        return;
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();
    hParseAOXML(lXMLString, ipContext);
#endif
    mType = ManipulationSetTemplateProperties;
}
DSSCManipulationSetTemplateProperties::~DSSCManipulationSetTemplateProperties()
{
}

int DSSCManipulationSetTemplateProperties::Execute()
{
    if (hCheckLayoutLoaded(mpNode)==S_FALSE)
		return S_FALSE;
    
    UpdateModel();
    if (!mIsFromActionObject)
		PersistProperties();
    
	mNodesModelChanged.push_back(mpNode);
    return S_OK;
    
}

void DSSCManipulationSetTemplateProperties::UpdateModel()
{
    if(mpNode == NULL || mpTemplate == NULL)
    {
        return;
    }
	
    std::vector<PropertyInfo*>::iterator lIt = mProperties.begin();
	for(; lIt!=mProperties.end(); lIt++)
	{
		// int lFmtInx = (*lIt)->mFormatIndex;
        std::string lPrptySetName = (*lIt)->mPropertySetName;
        std::string lPrptyName = (*lIt)->mPropertyName;
        CComVariant& lPrptyValue = (*lIt)->mPropertyValue;
        
        /*
        DSSFormat* lpFormat = getFormat(lFmtInx);
        if(!lpFormat)
        {
            DSSObjectContext* lpObjectContext = mpNode->getObjectContext();
            lpFormat = new DSSFormat(1,  NULL, lpObjectContext);
            mpTemplate->setFormatRefID(lFmtInx, lpObjectContext->InsertFormat(lpFormat));
        }
        lpFormat->setPropertyValue(lPrptySetName, lPrptyName, lPrptyValue);
         */
        // add the property on the template itself, not on its format
        mpTemplate->setPropertyValue(lPrptySetName, lPrptyName, lPrptyValue);
    }
}

DSSFormat* DSSCManipulationSetTemplateProperties::getFormat(int iIndex)
{
    switch (iIndex) {
        case 0:
            return mpTemplate->getHeaderFormat();
            break;
        case 1:
            return mpTemplate->getHeaderValuesFormat();
            break;
        case 2:
            return mpTemplate->getSubtotalHeadersFormat();
            break;
        case 3:
            return mpTemplate->getSubtotalValuesFormat();
            break;
        case 4:
            return mpTemplate->getDataFormat();
            break;
        case 5:
            return mpTemplate->getReportFormat();
            break;
        default:
            break;
    }
    return NULL;
}

void DSSCManipulationSetTemplateProperties::PersistProperties()
{
    GenerateAODeltaXML();
}

#ifndef __ANDROID__
void DSSCManipulationSetTemplateProperties::GenerateAODeltaXML()
{
    DSSObjectContext* lpContext = mpNode->getObjectContext();
    std::set<int> lIndexes;
    std::vector<PropertyInfo*>::iterator lIt = mProperties.begin();
	for(; lIt!=mProperties.end(); lIt++)
	{
		int lFmtInx = (*lIt)->mFormatIndex;
        
        if(lIndexes.find(lFmtInx) == lIndexes.end())
            lIndexes.insert(lFmtInx);
    }
    
    SimpleXMLBuilder lBuilder;
    
    mpActionObject = new DSSAODeltaXML(ManipulationSetNodeProperties);
    std::string lXMLStr = "";
    
    for(std::set<int>::iterator lIter = lIndexes.begin(); lIter != lIndexes.end(); lIter++)
    {
        SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
        
        SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
        SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("19", true));
        lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
        
        int lTreeType = static_cast<int>(mpNode->getTreeType());
        SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
        lTreeTypeXML.AddChild(lTreeTypeXMLContent);
        
        SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpNode->getKey().c_str(), true));
        lNodeKeyXML.AddChild(lNodeKeyXMLContent);
        
        SimpleXMLNode lRptManipulationXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation"));
        SimpleXMLNode lRptSetPropertiesXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation_method"));
        SimpleXMLNode lRptSetPropertiesXMLContent(SimpleXMLBuilder::CreateNodePtr("38", true));
        lRptSetPropertiesXML.AddChild(lRptSetPropertiesXMLContent);
        lRptManipulationXML.AddChild(lRptSetPropertiesXML);
        
        DSSFormat* lpFormat = new DSSFormat(*lIter, &mProperties, lpContext);
        
        xmlNodePtr lpFormatNode = static_cast<xmlNodePtr> (lpFormat->GenerateXMLNode());
        SimpleXMLNode formatNode(lpFormatNode);
        SimpleXMLNode lpPropertiesXML(SimpleXMLBuilder::CreateNodePtr("properties_xml"));
        SimpleXMLNode lpPropertiesXMLContent = formatNode.GetFirstChild();
        lpPropertiesXML.AddChild(lpPropertiesXMLContent);
        
        lRptManipulationXML.AddChild(lpPropertiesXML);
        
        lXML.AddChild(lManipulationMethodXML);
        lXML.AddChild(lTreeTypeXML);
        lXML.AddChild(lNodeKeyXML);
        lXML.AddChild(lRptManipulationXML);
        
        SimpleXMLNode lRoot(lBuilder.GetRoot());
        lRoot.AddChild(lXML);
        std::string tmpString = lBuilder.ToString(false);
        lXMLStr = lXMLStr + tmpString;
        //mActionObjects.push_back(lpAO);
        delete lpFormat;
    }
    
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lXMLStr);
}

void DSSCManipulationSetTemplateProperties::hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext)
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
                // mpTemplateNode = (DSSRWTemplateNode*)(ipContext->getNode(lstrContent));
                mpNode = ipContext->getNode(lstrContent);
			}
			else if (lCurrentNode.GetName().compare("report_manipulation") == 0) // report manipulation root
			{
                lRptManipulation = lCurrentNode;
			}
		}
	}
    
    SimpleXMLNode lMiNode;
    int lnFormatIndex = -1;
    for(lCurrentNode = lRptManipulation.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode.GetNextSibling())
    {
        if (lCurrentNode.IsElementNode())
		{
			if(lCurrentNode.GetName().compare("rw_format_type") == 0)
            {
                lCurrentNode.GetContent(lstrContent);
                lnFormatIndex = atoi(lstrContent.c_str());
            }
            else if (lCurrentNode.GetName().compare("rwfmt") == 0)
			{
				lMiNode = lCurrentNode;
			}
		}
    }
    
    // parse report manipulation
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
                lCurrentNode.GetProperty("n", lstrPropertySetName);
                for (lSubNode=lCurrentNode.GetFirstChild(); lSubNode.IsValid(); lSubNode.GetNextSibling())
                {
                    if (lSubNode.IsElementNode())
                    {
                        if (lSubNode.GetName().compare("pr") == 0)
                        {
                            lSubNode.GetProperty("pri", lstrContent);
                            int lnPri = atoi(lstrContent.c_str());
                            lstrPropertyName = ipContext->getPropertyNameFromID(lstrPropertySetName, lnPri);
                            lValueType = ipContext->getPropertyTypeFromID(lstrPropertySetName, lnPri);
                            lSubNode.GetProperty("v", lstrContent);
                            DSSExpNode::StringToVariant(lstrContent, lValueType, lValue);
                            PropertyInfo* lPropertyInfo = new PropertyInfo(lnFormatIndex, lstrPropertySetName, lstrPropertyName, lValue);
                            mProperties.push_back(lPropertyInfo);
                        }
                    }
                }
            }
        }
    }

}
#endif
