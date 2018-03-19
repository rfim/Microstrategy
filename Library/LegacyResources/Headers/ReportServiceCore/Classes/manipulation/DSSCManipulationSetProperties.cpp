//
//  DSSCManipulationSetProperties.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 3/2/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#include "DSSCManipulationSetProperties.h"
#include "DSSAODeltaXML.h"
#include "DSSBaseElementProxy.h"
#include "DSSExpNode.h"
#include "DSSRWTemplateNode.h"
#ifndef __ANDROID__
    #include "DSSSimpleXML.h"
#endif
#include <sstream>
#include <string>
#include <iostream>
#include <set>


DSSCManipulationSetProperties::DSSCManipulationSetProperties(DSSRWNode* ipNode, DSSCellHandle* ipCellHandle) : mpNode(ipNode), mpCellHandle(ipCellHandle)
{
    mType = ManipulationSetProperties;
}

DSSCManipulationSetProperties::~DSSCManipulationSetProperties()
{
    std::vector<PropertyInfo*>::iterator lIt = mProperties.begin();
	for(; lIt!=mProperties.end(); lIt++)
	{
        if (*lIt)
            delete *lIt;
    }
}

DSSCManipulationSetProperties::DSSCManipulationSetProperties(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationSetProperties;
    
    if(ipAO->IsDeltaXML() == false)
    {
        mpNode = NULL;
        mpCellHandle = NULL;
    }
    else
    {
        DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
        std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
        hParseXML(lXMLString, ipContext);
#endif
    }
}

void DSSCManipulationSetProperties::AddProperty(PropertyInfo* ipPropertyInfo)
{
    if (ipPropertyInfo)
        mProperties.push_back(ipPropertyInfo);
}

DSSRWTemplateNode* DSSCManipulationSetProperties::getTemplateNode()
{
    return (DSSRWTemplateNode*) mpNode;
}

DSSFormat* DSSCManipulationSetProperties::getFormat(int iIndex, bool iReadOnly)
{
    if(!mpCellHandle)
    {
        if(!mpNode)
            return NULL;
        DSSFormat* lpFormat = mpNode->getFormat((EnumDSSRWNodeFormatType)iIndex);
        if(lpFormat)
        {
            if(iReadOnly || mpNode->getObjectContext()->getFormatRefCount(lpFormat->getIndex()) <= 1)
                return lpFormat;
            DSSFormat* lpNewFormat = new DSSFormat();
            lpNewFormat->Duplicate(lpFormat);
            // mpNode->setFormatRefID(iIndex, mpNode->getObjectContext()->InsertFormat(lpNewFormat, lpFormat->getIndex()));
            mpNode->setFormatRefID(iIndex - 1, lpNewFormat->getIndex());
            return lpNewFormat;
        }
        else
        {
            DSSFormat* lpNewFormat = new DSSFormat(1, NULL, mpNode->getObjectContext());
            lpNewFormat->setIndex(mpNode->getObjectContext()->InsertFormat(lpNewFormat));
            mpNode->setFormatRefID(iIndex - 1, lpNewFormat->getIndex());
        }
        return lpFormat;
    }
    else if(mpCellHandle->type == DssCellHandleTemplateUnit)
    {
        // template unit or template metric
        DSSObjectContext* lpContext = mpNode->getObjectContext();
        return lpContext->getFormat(iIndex);
    }
    else
    {
        return NULL;
    }
}

void DSSCManipulationSetProperties::UpdateModel()
{
    std::vector<PropertyInfo*>::iterator lIt = mProperties.begin();
	for(; lIt!=mProperties.end(); lIt++)
	{
		int lFmtInx = (*lIt)->mFormatIndex;
        std::string lPrptySetName = (*lIt)->mPropertySetName;
        std::string lPrptyName = (*lIt)->mPropertyName;
        CComVariant& lPrptyValue = (*lIt)->mPropertyValue;
        
        DSSFormat* lpFormat = getFormat(lFmtInx);
        lpFormat->setPropertyValue(lPrptySetName, lPrptyName, lPrptyValue);
    }
}

void DSSCManipulationSetProperties::PersistProperties()
{
    mpActionObject = new DSSAODeltaXML(ManipulationSetProperties);
    
    std::string lAOXml = hBuildXML();
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lAOXml);
}

int DSSCManipulationSetProperties::Execute()
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
std::string DSSCManipulationSetProperties::hBuildXML()
{
    DSSObjectContext* lpContext = mpNode->getObjectContext();
    
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    int lTreeType = static_cast<int>(mpNode->getTreeType());
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    if(mpCellHandle == NULL)
    {
   /*     if(mpNode->getType() == DssRWNodeControl)
        {
            SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("62", true));
            lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
            
            DSSRWControlNode* lpNode = (DSSRWControlNode*)mpNode;
            DSSRWControl* lpControl = lpNode->getControls()->Item(0);
            std::string lControlKey = lpControl->getKey();
            SimpleXMLNode lControlKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_control_key"));
            SimpleXMLNode lControlKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lControlKey.c_str()));
            
            lXML.AddChild(lManipulationMethodXML);
            lXML.AddChild(lTreeTypeXML);
            lXML.AddChild(lNodeKeyXML);
            lXML.AddChild(lControlKeyXML);
        }
        else if(mpNode->getType() == DssRWNodeFieldGroup)
        {
            SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("17", true));
            lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
            
            DSSRWFieldGroupNode* lpNode = (DSSRWFieldGroupNode*)mpNode;
            DSSField* lpField = lpNode->getFields()->Item(0);
            std::string lFieldKey = lpField->getKey();
            SimpleXMLNode lFieldKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_field"));
            SimpleXMLNode lFieldKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lFieldKey.c_str()));
            
            lXML.AddChild(lManipulationMethodXML);
            lXML.AddChild(lTreeTypeXML);
            lXML.AddChild(lNodeKeyXML);
            lXML.AddChild(lFieldKeyXML);
        }
        else
        {
            SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("7"));
            lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
            
            lXML.AddChild(lManipulationMethodXML);
            lXML.AddChild(lTreeTypeXML);
            lXML.AddChild(lNodeKeyXML);
        }
        SimpleXMLNode lFormatXML(SimpleXMLBuilder::CreateNodePtr("rwfmt"));
        DSSFormat* lpFormat = new DSSFormat(1, mpProperties, lpContext);
        //DSSFormat* lpFormat = getFormat(1);
        SimpleXMLParser lParser(lpFormat->GenerateXML());
        SimpleXMLNode lFormatXMLContent = lParser.GetRoot();
        lFormatXML.AddChild(lFormatXMLContent);
        lXML.AddChild(lFormatXML);*/
    }
    else
    {
        SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("19", true));
        lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
        
        lXML.AddChild(lManipulationMethodXML);
        lXML.AddChild(lTreeTypeXML);
        lXML.AddChild(lNodeKeyXML);
        
        SimpleXMLNode lReportManipulationXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation"));
        SimpleXMLNode lReportManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation_method"));
        SimpleXMLNode lReportManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("11", true));
        lReportManipulationMethodXML.AddChild(lReportManipulationMethodXMLContent);
        lReportManipulationXML.AddChild(lReportManipulationMethodXML);
        
        if(mpCellHandle->type == DssCellHandleTemplateUnit)
        {
            DSSTemplateUnit* lTemplateUnit = mpCellHandle->p.unit;
            GUID lUnitID = lTemplateUnit->getID();
            EnumDSSObjectType lUnitType = lTemplateUnit->getObjectContext()->FindObjectH(lUnitID)->getType();
            
            SimpleXMLNode lUnitIDXml(SimpleXMLBuilder::CreateNodePtr("unit_id"));
            char lUnitIDString[33];
            DSSBaseElementProxy::ConvertGUIDToString(lUnitID, lUnitIDString);
            SimpleXMLNode lUnitIDXmlContent(SimpleXMLBuilder::CreateNodePtr(lUnitIDString, true));
            lUnitIDXml.AddChild(lUnitIDXmlContent);
            
            SimpleXMLNode lUnitTypeXml(SimpleXMLBuilder::CreateNodePtr("unit_type"));
            int lUnitTypeInt = static_cast<int>(lUnitType);
            SimpleXMLNode lUnitTypeXmlContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lUnitTypeInt).c_str(), true));
            lUnitTypeXml.AddChild(lUnitTypeXmlContent);
            
            lReportManipulationXML.AddChild(lUnitIDXml);
            lReportManipulationXML.AddChild(lUnitTypeXml);
        }
        
        SimpleXMLNode lFormatXML(SimpleXMLBuilder::CreateNodePtr("format_xml"));
        SimpleXMLNode lGridFormatXML(SimpleXMLBuilder::CreateNodePtr("grid_format"));
        
        std::set<int> lIndexes;
        std::vector<PropertyInfo*>::iterator lIt = mProperties.begin();
        for(; lIt!=mProperties.end(); lIt++)
        {
            int lFmtInx = (*lIt)->mFormatIndex;
            
            if(lIndexes.find(lFmtInx) == lIndexes.end())
                lIndexes.insert(lFmtInx);
        }
        for(std::set<int>::iterator lIter = lIndexes.begin(); lIter != lIndexes.end(); lIter++)
        {
            DSSFormat* lpFormat = new DSSFormat(*lIter, &mProperties, lpContext);
            PropertySetsMap lPropertySetsMap = lpFormat->getPropertySetsMap();
            for(PropertySetsMap::iterator lIter = lPropertySetsMap.begin(); lIter != lPropertySetsMap.end(); lIter++)
            {
                DSSFormatPropertySet* lFormatPropertySet = static_cast<DSSFormatPropertySet*>(lIter->second);
                xmlNodePtr lpPropertyNode = static_cast<xmlNodePtr> (lFormatPropertySet->GenerateXMLNode());
                SimpleXMLNode propertyNode(lpPropertyNode);
                lGridFormatXML.AddChild(propertyNode);
            }
        }
        lFormatXML.AddChild(lGridFormatXML);
        lReportManipulationXML.AddChild(lFormatXML);
        
        lXML.AddChild(lReportManipulationXML);
    }
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
	lRoot.AddChild(lXML);
	
	return lBuilder.ToString(false);
}

void DSSCManipulationSetProperties::hParseXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser(iXMLString);
    if(lParser.IsValid() == false)
    {
        return;
    }
    
    SimpleXMLNode lCurrentNode = lParser.GetRoot();
	if (!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
    {
		return;
    }
    
    std::string lstrContent;
    SimpleXMLNode lReportXMLNode;
    SimpleXMLNode lGridFmtXMLNode;
    SimpleXMLNode lFmtXMLNode;
	for (lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if (lCurrentNode.IsElementNode())
		{
            if (lCurrentNode.GetName().compare("rw_node_key") == 0)
            {
                lCurrentNode.GetContent(lstrContent);
                mpNode = ipContext->getNode(lstrContent);
            }
            else if (lCurrentNode.GetName().compare("report_manipulation") == 0)
            {
                lReportXMLNode = lCurrentNode;
            }
		}
    }
    
    if (!lReportXMLNode.IsValid())
        return;
    
    for (lCurrentNode=lReportXMLNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if (lCurrentNode.IsElementNode())
        {
            if (lCurrentNode.GetName().compare("unit_id")==0)
            {
                GUID lTemplateUnitGUID;
                lCurrentNode.GetContent(lstrContent);
                DSSBaseElementProxy::ConvertFromStringToGUID(lstrContent, lTemplateUnitGUID);
                DSSRWTemplateNode* lpTemplateNode = static_cast<DSSRWTemplateNode*>(mpNode);
                DSSTemplate* lpTemplate = lpTemplateNode->getTemplate();
                DSSTemplateUnit* lpTemplateUnit = lpTemplate->getTemplateUnitFromAxisIndex(DssAxisRows, lTemplateUnitGUID, true);
                if(!lpTemplateUnit)
                {
                    lpTemplateUnit = lpTemplate->getTemplateUnitFromAxisIndex(DssAxisColumns, lTemplateUnitGUID, true);
                }
                mpCellHandle = new DSSCellHandle(DssCellHandleTemplateUnit, lpTemplateUnit, 0);
            }
            else if (lCurrentNode.GetName().compare("format_xml")==0)
            {
                for (lGridFmtXMLNode=lCurrentNode.GetFirstChild(); lGridFmtXMLNode.IsValid(); lGridFmtXMLNode=lGridFmtXMLNode.GetNextSibling())
                {
                    if (lGridFmtXMLNode.IsElementNode())
                    {
                        if (lGridFmtXMLNode.GetName().compare("grid_format") == 0)
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
    
    if (!lGridFmtXMLNode.IsValid())
        return;
    
    std::string lstrPropertySetName;
    std::string lstrPropertyName;
    EnumVariantType lValueType;
    CComVariant lValue;
    SimpleXMLNode lSubNode;
    int lnFormatIndex = 1;
    for (lFmtXMLNode=lGridFmtXMLNode.GetFirstChild(); lFmtXMLNode.IsValid(); lFmtXMLNode=lFmtXMLNode.GetNextSibling())
    {
        if (lFmtXMLNode.IsElementNode())
        {
            if (lFmtXMLNode.GetName().compare("rw_format_type") == 0)
            {
                lFmtXMLNode.GetContent(lstrContent);
                lnFormatIndex = atoi(lstrContent.c_str());
            }
            else if (lFmtXMLNode.GetName().compare("rwfmt") == 0)
            {
                for (lCurrentNode=lFmtXMLNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode=lCurrentNode.GetNextSibling())
                {
                    if (lCurrentNode.IsElementNode())
                    {
                        if (lCurrentNode.GetName().compare("prs")==0)
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
        }
    }
}
#endif