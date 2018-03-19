//
//  DSSCManipulationSetNodeProperties.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 3/2/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#import "DSSCManipulationSetNodeProperties.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifndef __ANDROID__
    #include "DSSSimpleXML.h"
    #include "DSSExpNode.h"
#endif
#include <set>

DSSCManipulationSetNodeProperties::DSSCManipulationSetNodeProperties(DSSRWNode* ipNode, DSSCellHandle* ipCellHandle) :DSSCManipulationSetProperties(ipNode, ipCellHandle)
{
    mType = ManipulationSetNodeProperties;
}

DSSCManipulationSetNodeProperties::DSSCManipulationSetNodeProperties(DSSActionObject* ipAO, DSSObjectContext* ipContext) : DSSCManipulationSetProperties(ipAO, ipContext)
{
    mType = ManipulationSetNodeProperties;
    
    mpCellHandle = NULL;
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();
    hParseXML(lXMLString, ipContext);
#endif
}

DSSCManipulationSetNodeProperties::~DSSCManipulationSetNodeProperties()
{}

void DSSCManipulationSetNodeProperties::UpdateModel()
{
    if(mpNode == NULL)
    {
        return;
    }
	
    std::vector<PropertyInfo*>::iterator lIt = mProperties.begin();
	for(; lIt!=mProperties.end(); lIt++)
	{
		int lFmtInx = (*lIt)->mFormatIndex;
        std::string lPrptySetName = (*lIt)->mPropertySetName;
        std::string lPrptyName = (*lIt)->mPropertyName;
        CComVariant& lPrptyValue = (*lIt)->mPropertyValue;
        
		bool extendedProperty = false;
		
		if (lPrptySetName.compare("FormattingPosition")==0) 
		{
			if (lPrptyName.compare("Top")==0) 
			{
                mpNode->setTop(lPrptyValue.dblVal);
				extendedProperty = true;
			}
			else if (lPrptyName.compare("Left")==0)
			{
                mpNode->setLeft(lPrptyValue.dblVal);
				extendedProperty = true;
			}
		}
		else if (lPrptySetName.compare("FormattingSize")==0)
		{
			if (lPrptyName.compare("Width")==0)
			{
				mpNode->setWidth(lPrptyValue.dblVal);
                extendedProperty = true;
			}
			else if (lPrptyName.compare("Height")==0)
			{
                mpNode->setHeight(lPrptyValue.dblVal);
				extendedProperty = true;
			}
		}
		else if (lPrptySetName.compare("FormattingAppearance")==0)
		{
			if (lPrptyName.compare("Name")==0)
			{
                mpNode->setName(lPrptyValue.ToString());	
                extendedProperty = true;
			}
			else if (lPrptyName.compare("TooltipText")==0)
			{
                mpNode->setToolTipText(lPrptyValue.ToString());
				extendedProperty = true;
			}
		}
		else if (lPrptySetName.compare("FormattingOrder")==0)
		{
			if (lPrptyName.compare("ZIndex")==0)
			{
                mpNode->setZIndex(lPrptyValue.iVal);
				extendedProperty = true;
			}
		}
        else if (lPrptySetName.compare("FormattingView")==0)
        {
            if (lPrptyName.compare("Title")==0)
            {
                mpNode->setTitle(lPrptyValue.ToString());
                extendedProperty = true;
            }
        }
        
		if (!extendedProperty)
        {
            DSSFormat* lpFormat = getFormat(lFmtInx);
            lpFormat->setPropertyValue(lPrptySetName, lPrptyName, lPrptyValue);
        }
    }
}

void DSSCManipulationSetNodeProperties::PersistProperties()
{
    GenerateAODeltaXML();
}

int DSSCManipulationSetNodeProperties::Execute()
{
    if (hCheckLayoutLoaded(mpNode)==S_FALSE)
		return S_FALSE;
    
    UpdateModel();
    if (!mIsFromActionObject)
		PersistProperties();
    
	mNodesModelChanged.push_back(mpNode);
    return S_OK;
}

#ifndef __ANDROID__
void DSSCManipulationSetNodeProperties::GenerateAODeltaXML()
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
        SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("7", true));
        lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
        
        int lTreeType = static_cast<int>(mpNode->getTreeType());
        SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
        lTreeTypeXML.AddChild(lTreeTypeXMLContent);
        
        SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpNode->getKey().c_str(), true));
        lNodeKeyXML.AddChild(lNodeKeyXMLContent);
        
        SimpleXMLNode lFormatTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_format_type"));
        SimpleXMLNode lFormatTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(*lIter).c_str(), true));
        lFormatTypeXML.AddChild(lFormatTypeXMLContent);
        
        DSSFormat* lpFormat = new DSSFormat(*lIter, &mProperties, lpContext);
                
        xmlNodePtr lpFormatNode = static_cast<xmlNodePtr> (lpFormat->GenerateXMLNode());
        SimpleXMLNode formatNode(lpFormatNode);
        
        lXML.AddChild(lManipulationMethodXML);
        lXML.AddChild(lTreeTypeXML);
        lXML.AddChild(lNodeKeyXML);
        lXML.AddChild(lFormatTypeXML);
        lXML.AddChild(formatNode);
        
        SimpleXMLNode lRoot(lBuilder.GetRoot());
        lRoot.AddChild(lXML);
        std::string tmpString = lBuilder.ToString(false);
        lXMLStr = lXMLStr + tmpString;
        //mActionObjects.push_back(lpAO);
        delete lpFormat;
    }
    
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lXMLStr);
}

void DSSCManipulationSetNodeProperties::hParseXML(std::string& iXMLString, DSSObjectContext* ipContext)
{
    int lnFormatIndex = -1;
    
    SimpleXMLParser lParser(iXMLString);
	if (!lParser.IsValid())
		return;
	
	SimpleXMLNode lCurrentNode = lParser.GetRoot();
	
	if (!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
		return;
	
	SimpleXMLNode lMiNode;
	std::string lstrContent;
	for (lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
	{
		if (lCurrentNode.IsElementNode())
		{
			if (lCurrentNode.GetName().compare("rw_node_key") == 0)
			{
				// Found the node key
				lCurrentNode.GetContent(lstrContent);
				mpNode = ipContext->getNode(lstrContent);
			}
            else if (lCurrentNode.GetName().compare("rw_format_type") == 0)
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
	
	if (!lMiNode.IsValid())
		return;
	
    std::string lstrPropertySetName;
    std::string lstrPropertyName;
    EnumVariantType lValueType;
    CComVariant lValue;
    SimpleXMLNode lSubNode;
    for (lCurrentNode = lMiNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if (lCurrentNode.IsElementNode())
        {
            if (lCurrentNode.GetName().compare("prs") == 0)
            {
                lCurrentNode.GetProperty("n", lstrPropertySetName);
                for (lSubNode=lCurrentNode.GetFirstChild(); lSubNode.IsValid(); lSubNode = lSubNode.GetNextSibling())
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