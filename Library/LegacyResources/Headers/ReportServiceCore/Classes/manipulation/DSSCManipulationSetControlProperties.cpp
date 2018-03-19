/*
 *  DSSCManipulationSetControlProperties.cpp
 *  ReportServiceCore
 *
 *  Created by lcao on 3/30/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSCManipulationSetControlProperties.h"
#include "DSSRWControls.h"
#include "DSSAOSetObjectProperties.h"
#include "DSSAODeltaXML.h"
#include <sstream>
#include <vector>

#include "DSSExpNode.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxml.h"
#else
#include "DSSSimpleXML.h"
#endif
#include <set>

DSSCManipulationSetControlProperties::DSSCManipulationSetControlProperties(DSSRWControlNode* ipControlNode, bool iAOXML) : DSSCManipulationSetProperties(ipControlNode)
{
    mpControlNode = ipControlNode;
    mpControl = NULL;
    DSSRWControls* lpControls = mpControlNode->getControls();
	if (lpControls && lpControls->Count()>0)
		mpControl = lpControls->Item(0);
	
	mIsFromActionObject = false;
    mAOXML = iAOXML;
    
    mType = ManipulationSetControlProperties;
}

DSSCManipulationSetControlProperties::DSSCManipulationSetControlProperties(DSSActionObject* ipAO, DSSObjectContext* ipContext) : DSSCManipulationSetProperties(ipAO, ipContext)
{
    if (ipAO->IsDeltaXML())
    {
        DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
        std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
        hParseXML(lXMLString, ipContext);
#endif
        return;
    }
        
	DSSAOSetObjectProperties* lpAO = dynamic_cast<DSSAOSetObjectProperties*> (ipAO);
    if (!lpAO)
        return;
	mpNode = (DSSRWControlNode*)(ipContext->getNode(lpAO->getNodeKey()));
	mpControl = NULL;
	DSSRWControls* lpControls = ((DSSRWControlNode*)mpNode)->getControls();
	if (lpControls && lpControls->Count()>0)
		mpControl = lpControls->Item(0);
	
	hParsePropertiesFromActionObject(lpAO);
	mIsFromActionObject = true;
    
    mType = ManipulationSetControlProperties;
}

DSSCManipulationSetControlProperties::~DSSCManipulationSetControlProperties()
{}

int DSSCManipulationSetControlProperties::Execute()
{
	if (hCheckLayoutLoaded(mpNode)==S_FALSE)
		return S_FALSE;
    
	UpdateModel();
	
	if (!mIsFromActionObject)
		PersistProperties();
    
    mNodesControlPropertiesChanged.push_back(mpControlNode);
	
	return S_OK;
}

DSSFormat* DSSCManipulationSetControlProperties::getFormat(int iIndex, bool iReadOnly)
{
    if (!mpControl)
        return NULL;
    DSSFormat* lpFormat = mpControl->getFormat((EnumDSSRWControlFormatType)iIndex);
    if(lpFormat)
    {
        if(iReadOnly || mpControl->getObjectContext()->getFormatRefCount(lpFormat->getIndex()) <= 1)
            return lpFormat;
        DSSFormat* lpNewFormat = new DSSFormat();
        lpNewFormat->Duplicate(lpFormat);
        mpControl->setFormatRefID(iIndex - 1, lpNewFormat->getIndex());
        return lpNewFormat;
    }
    else
    {
        DSSFormat* lpNewFormat = new DSSFormat(1, NULL, mpControl->getObjectContext());
        lpNewFormat->setIndex(mpControl->getObjectContext()->InsertFormat(lpNewFormat));
        mpControl->setFormatRefID(iIndex - 1, lpNewFormat->getIndex());
    }
    return NULL;
}

void DSSCManipulationSetControlProperties::UpdateModel()
{
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
                mpControl->setTop(lPrptyValue.dblVal);
				extendedProperty = true;
			}
			else if (lPrptyName.compare("Left")==0)
			{
                mpControl->setLeft(lPrptyValue.dblVal);
				extendedProperty = true;
			}
		}
		else if (lPrptySetName.compare("FormattingSize")==0)
		{
			if (lPrptyName.compare("Width")==0)
			{
				mpControl->setWidth(lPrptyValue.dblVal);
                extendedProperty = true;
			}
			else if (lPrptyName.compare("Height")==0)
			{
                mpControl->setHeight(lPrptyValue.dblVal);
				extendedProperty = true;
			}
		}
		else if (lPrptySetName.compare("FormattingAppearance")==0)
		{
			if (lPrptyName.compare("Name")==0)
			{
                mpControl->setName(lPrptyValue.ToString());
				extendedProperty = true;
			}
			else if (lPrptyName.compare("TooltipText")==0)
			{
                mpControl->setToolTipText(lPrptyValue.ToString());
				extendedProperty = true;
			}
		}
		else if (lPrptySetName.compare("FormattingOrder")==0)
		{
			if (lPrptyName.compare("ZIndex")==0)
			{
                mpControl->setZIndex(lPrptyValue.iVal);
				extendedProperty = true;
			}
		}
        else if (lPrptySetName.compare("FormattingSelector") == 0)
        {
            if(lPrptyName.compare("SearchOnServer") == 0)
            {
                mNodeMapServer[mpControlNode->getKey()] = mpControlNode;
            }
            else if(lPrptyName.compare("Style") == 0)
            {
                DSSFormat* lpFormat = mpControl->getFormat(DssRWControlFormatControl);
                if(lpFormat)
                {
                    CComVariant* lpValue1 = lpFormat->getPropertyValue("FormattingSelector", "Style");
                    CComVariant* lpValue2 = lpFormat->getPropertyValue("FormattingSelector", "SearchOnServer");
                    // if(*lpValue1 == CComVariant(9) && lPrptyValue != CComVariant(9) && *lpValue2 == CComVariant(-1))
                    if(lpValue1->iVal == 9 && lPrptyValue.iVal != 9 && lpValue2->boolVal == -1)
                        mNodeMapServer[mpControlNode->getKey()] = mpControlNode;
                 }
            }
        }
        
		if (!extendedProperty)
        {
            DSSFormat* lpFormat = getFormat(lFmtInx);
            lpFormat->setPropertyValue(lPrptySetName, lPrptyName, lPrptyValue);
        }
    }
}

void DSSCManipulationSetControlProperties::PersistProperties()
{
    if(mAOXML == true)
    {
        GenerateAODeltaXML();
        //(static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(hBuildXML());
        return;
    }
    
	std::string persistKey = mpControl->getKey();
	std::map<int, std::string> dataArray;
	char ITEM_SEPARATOR = 30;
	char UNIT_SEPARATOR = 31;

	std::vector<PropertyInfo*>::iterator lIt = mProperties.begin();
	for(; lIt!=mProperties.end(); lIt++)
	{
		int lFmtInx = (*lIt)->mFormatIndex;
        std::string lPrptySetName = (*lIt)->mPropertySetName;
        std::string lPrptyName = (*lIt)->mPropertyName;
        CComVariant& lPrptyValue = (*lIt)->mPropertyValue;
		
		dataArray[lFmtInx] += persistKey;
		dataArray[lFmtInx] += UNIT_SEPARATOR;
		dataArray[lFmtInx] += lPrptySetName;
		dataArray[lFmtInx] += UNIT_SEPARATOR;
		dataArray[lFmtInx] += lPrptyName;
		dataArray[lFmtInx] += UNIT_SEPARATOR;
		dataArray[lFmtInx] += lPrptyValue.ToString();
		dataArray[lFmtInx] += ITEM_SEPARATOR;
	}
	
	std::map<int, std::string>::iterator it;
	DSSAOSetObjectProperties* lpAOSetObjectProperties;
	for (it=dataArray.begin(); it!=dataArray.end(); it++)
	{
		lpAOSetObjectProperties = new DSSAOSetObjectProperties(ManipulationSetControlProperties);
		lpAOSetObjectProperties->setNodeKey(mpNode->getKey());
		lpAOSetObjectProperties->setFmtType((*it).first);
		lpAOSetObjectProperties->setData((*it).second);
		mActionObjects.push_back(lpAOSetObjectProperties);
	}
}

void DSSCManipulationSetControlProperties::hParsePropertiesFromActionObject(DSSAOSetObjectProperties* ipAO)
{
	char ITEM_SEPARATOR = 30;
	char UNIT_SEPARATOR = 31;
	size_t idx;
	int type=0;
	std::string itemString;
	std::stringstream oss;
	
	std::string dataString = ipAO->getData();
    
	while (1) 
	{
		idx = dataString.find(ITEM_SEPARATOR);
		if (idx==std::string::npos)
			break;
		
        PropertyInfo* lpPropertyInfo = new PropertyInfo();
        lpPropertyInfo->mFormatIndex = ipAO->getFmtType();
		itemString = dataString.substr(0,idx);
		dataString = dataString.substr(idx+1);
		type = 0;
		while (1) 
		{
			idx = itemString.find(UNIT_SEPARATOR);
			if (idx == std::string::npos)
			{
				lpPropertyInfo->mPropertyValue = itemString.c_str();
				break;
			}
			type++;
			if (type == 2)
				lpPropertyInfo->mPropertySetName = itemString.substr(0,idx);
			else if(type == 3)
				lpPropertyInfo->mPropertyName = itemString.substr(0,idx);
			itemString = itemString.substr(idx+1);
		}
		mProperties.push_back(lpPropertyInfo);
	}
}
#ifndef __ANDROID__
void DSSCManipulationSetControlProperties::GenerateAODeltaXML()
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
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    for(std::set<int>::iterator lIter = lIndexes.begin(); lIter != lIndexes.end(); lIter++)
    {
        SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
        
        SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
        SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("62", true));
        lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
        
        int lTreeType = static_cast<int>(mpNode->getTreeType());
        SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
        lTreeTypeXML.AddChild(lTreeTypeXMLContent);
        
        SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpNode->getKey().c_str(), true));
        lNodeKeyXML.AddChild(lNodeKeyXMLContent);
        
        SimpleXMLNode lControlKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_control_key"));
        SimpleXMLNode lControlKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpControl->getKey().c_str(), true));
        lControlKeyXML.AddChild(lControlKeyXMLContent);

        SimpleXMLNode lFormatTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_format_type"));
        SimpleXMLNode lFormatTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(*lIter).c_str(), true));
        lFormatTypeXML.AddChild(lFormatTypeXMLContent);
        
        DSSFormat* lpFormat = new DSSFormat(*lIter, &mProperties, lpContext);
        
        xmlNodePtr lpFormatNode = static_cast<xmlNodePtr> (lpFormat->GenerateXMLNode());
        SimpleXMLNode formatNode(lpFormatNode);
        
        lXML.AddChild(lManipulationMethodXML);
        lXML.AddChild(lTreeTypeXML);
        lXML.AddChild(lNodeKeyXML);
        lXML.AddChild(lControlKeyXML);
        // lXML.AddChild(lShowAllXML);
        lXML.AddChild(lFormatTypeXML);
        lXML.AddChild(formatNode);
        
        lRoot.AddChild(lXML);
        delete lpFormat;
    }
    
    {
        // show_all
        SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
        
        SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
        SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("78", true));
        lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
        
        int lTreeType = static_cast<int>(mpNode->getTreeType());
        SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
        lTreeTypeXML.AddChild(lTreeTypeXMLContent);
        
        SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpNode->getKey().c_str(), true));
        lNodeKeyXML.AddChild(lNodeKeyXMLContent);
        
        SimpleXMLNode lControlKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_control_key"));
        SimpleXMLNode lControlKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpControl->getKey().c_str(), true));
        lControlKeyXML.AddChild(lControlKeyXMLContent);
        
        int lShowAllInt = static_cast<int>(mpControl->getShowAll());
        SimpleXMLNode lShowAllXML(SimpleXMLBuilder::CreateNodePtr("show_all"));
        SimpleXMLNode lShowAllXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lShowAllInt).c_str(), true));
        lShowAllXML.AddChild(lShowAllXMLContent);
        
        lXML.AddChild(lManipulationMethodXML);
        lXML.AddChild(lTreeTypeXML);
        lXML.AddChild(lNodeKeyXML);
        lXML.AddChild(lControlKeyXML);
        lXML.AddChild(lShowAllXML);
        lRoot.AddChild(lXML);
    }
    mpActionObject = new DSSAODeltaXML(ManipulationSetControlProperties);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}


void DSSCManipulationSetControlProperties::hParseXML(std::string& iXMLString, DSSObjectContext* ipContext)
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
            else if (lCurrentNode.GetName().compare("rw_control_key") == 0)
            {
                lCurrentNode.GetContent(lstrContent);
                mpControl = ipContext->getControl(lstrContent);
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

#else
std::string DSSCManipulationSetControlProperties::hBuildXML()
{
    TiXmlDocument lDoc;
    
    TiXmlElement* lpShowAllElement = new TiXmlElement("rw_manipulation");
    
    TiXmlElement* lpSAManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
    TiXmlText* lpManipulationMethodText = new TiXmlText("78");
    lpSAManipulationMethodElement->LinkEndChild(lpManipulationMethodText);
    
    int lTreeType = static_cast<int>(mpNode->getTreeType());
    TiXmlElement* lpSATreeTypeElement = new TiXmlElement("rw_tree_type");
    TiXmlText* lpSATreeTypeText =  new TiXmlText(DSSExpNode::IntToString(lTreeType).c_str());
    lpSATreeTypeElement->LinkEndChild(lSATreeTypeText);
    
    TiXmlElement* lpSANodeKeyElement = new TiXmlElement("rw_node_key");
    TiXmlText* lpSANodeKeyText = new TiXmlElement(mpNode->getKey().c_str());
    lpSANodeKeyElement->LinkEndChild(lpSANodeKeyText);
    
    TiXmlElement* lpSAControlKeyElement = new TiXmlElement("rw_control_key");
    TiXmlText* lpSAControlKeyText = new TiXmlText(mpControl->getKey().c_str());
    lpSAControlKeyElement->LinkEndChild(lpSAControlKeyText);
    
    int lShowAllInt = static_cast<int>(mpControl->getShowAll());
    TiXmlElement* lpSAShowAllElement = new TiXmlElement("show_all");
    TiXmlText* lpSAShowAllText = new TiXmlText(DSSExpNode::IntToString(lShowAllInt).c_str());
    lpSAShowAllElement->LinkEndChild(lpSAShowAllText);
    
    lpShowAllElement->LinkEndChild(lpSAManipulationMethodElement);
    lpShowAllElement->LinkEndChild(lpSATreeTypeElement);
    lpShowAllElement->LinkEndChild(lpSANodeKeyElement);
    lpShowAllElement->LinkEndChild(lpSAControlKeyElement);
    lpShowAllElement->LinkEndChild(lpSAShowAllElement);

    TiXmlPrinter lPrinter;
    lPrinter.SetIndent(NULL);
    lPrinter.SetLineBreak(NULL);
    lDoc.LinkEndChild(lpShowAllElement);
    lDoc.Accept(&lPrinter);
    
    return DSSCManipulationSetProperties::hBuildXML() + printer.Str();
}
#endif