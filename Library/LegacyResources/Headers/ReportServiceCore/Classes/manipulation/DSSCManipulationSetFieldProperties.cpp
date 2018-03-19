//
//  DSSCManipulationSetFieldProperties.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 12/27/11.
//  Modified by Zhu, Wei on 12/12/12.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "DSSCManipulationSetFieldProperties.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#import <math.h>

#ifdef __ANDROID__
#include "TinyXml/tinyxm.h"
#else
#include "DSSSimpleXML.h"
#endif

DSSCManipulationSetFieldProperties::DSSCManipulationSetFieldProperties(DSSRWFieldGroupNode* ipFieldGroupNode, std::string& iFieldKey) : DSSCManipulationSetProperties(ipFieldGroupNode), mpFieldGroupNode(ipFieldGroupNode)
{
    mpNode = ipFieldGroupNode;
    mpField = mpFieldGroupNode->getFields()->FindByKey(iFieldKey);
    
    mType = ManipulationSetFieldProperties;
}

DSSCManipulationSetFieldProperties::DSSCManipulationSetFieldProperties(DSSActionObject* ipAO, DSSObjectContext* ipContext) : DSSCManipulationSetProperties(ipAO, ipContext)
{
    mType = ManipulationSetFieldProperties;
    
    if(!ipAO->IsDeltaXML())
        return;
    
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
    std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
    hParseXML(lXMLString, ipContext);
#endif
}

DSSCManipulationSetFieldProperties::~DSSCManipulationSetFieldProperties()
{
    
}

void DSSCManipulationSetFieldProperties::UpdateModel()
{
    if (mProperties.size() == 0)
        return;
    
    if (!mpNode)
        return;
    
    std::vector<PropertyInfo*>::iterator lIt = mProperties.begin();
	for(; lIt!=mProperties.end(); lIt++)
	{
		int lFmtInx = (*lIt)->mFormatIndex;
        std::string lPrptySetName = (*lIt)->mPropertySetName;
        std::string lPrptyName = (*lIt)->mPropertyName;
        CComVariant& lPrptyValue = (*lIt)->mPropertyValue;
        
        if (lPrptySetName == "FormattingPosition")
        {
            if (lPrptyName == "Top")
            {
                if (lPrptyValue.vt != VT_R8)
                    _ASSERT(false);
                
                if (!(fabs(mpField->getTop() - lPrptyValue.dblVal ) < 1e-6))
                {
                    mpField->setTop(lPrptyValue.dblVal);
                }
            }
            else if (lPrptyName == "Left")
            {
                if (lPrptyValue.vt != VT_R8)
                    _ASSERT(false);
                
                if (!(fabs(mpField->getLeft() - lPrptyValue.dblVal ) < 1e-6))
                {
                    mpField->setLeft(lPrptyValue.dblVal);
                }
            }
        }
        else if (lPrptySetName == "FormattingSize")
        {
            if (lPrptyName == "Width")
            {
                if (lPrptyValue.vt != VT_R8)
                    _ASSERT(false);
                
                if (!(fabs(mpField->getWidth() - lPrptyValue.dblVal ) < 1e-6))
                {
                    mpField->setWidth(lPrptyValue.dblVal);
                }
            }
            else if (lPrptyName == "Height")
            {
                if (lPrptyValue.vt != VT_R8)
                    _ASSERT(false);
                
                if (!(fabs(mpField->getHeight() - lPrptyValue.dblVal ) < 1e-6))
                {
                    mpField->setHeight(lPrptyValue.dblVal);
                }
            }
        }
        else if (lPrptySetName == "FormattingAppearance")
        {
            if (lPrptyName == "Name")
            {
                if (lPrptyValue.vt != VT_BSTR)
                    _ASSERT(false);
                
                std::string lName = MBase::WideCharToUTF8(lPrptyValue.bstrVal).c_str();
                if (mpField->getName() != lName)
                {
                    mpField->setName(lName);
                }
            }
            else if (lPrptyName == "TooltipText")
            {
                if (lPrptyValue.vt != VT_BSTR)
                    _ASSERT(false);
                
                std::string lToolipText = MBase::WideCharToUTF8(lPrptyValue.bstrVal).c_str();
                if (mpField->getToolTipText() != lToolipText)
                {
                    mpField->setTooltipText(lToolipText);
                }
            }
        }
        else if (lPrptySetName == "FormattingOrder")
        {
            if (lPrptyName != "ZIndex")
            {
                if (lPrptyValue.vt != VT_I4)
                    _ASSERT(false);
                
                if (mpField->getZIndex() != lPrptyValue.iVal)
                {
                    mpField->setZIndex(lPrptyValue.iVal);
                }
            }
        }
        
        CComVariant* lpOldVal = NULL;
        lpOldVal = getFormat(lFmtInx, true)->getPropertyValue(lPrptySetName, lPrptyName);
        
        if (!lpOldVal || *lpOldVal != lPrptyValue)
        {
            getFormat(lFmtInx)->setPropertyValue(lPrptySetName, lPrptyName, lPrptyValue);
        }
    }
}

void DSSCManipulationSetFieldProperties::PersistProperties()
{
    mpActionObject = new DSSAODeltaXML(ManipulationSetFieldFormat);
    
    std::string lAOXml = hBuildXML();
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lAOXml);
}

int DSSCManipulationSetFieldProperties::Execute()
{
    if (!mpNode || !mpField)
        return S_FALSE;
    
    return DSSCManipulationSetProperties::Execute();
}


DSSFormat* DSSCManipulationSetFieldProperties::getFormat(int iIndex, bool iReadOnly)
{
    if (iIndex != 0 || !mpField) //The given index for getFormat in ManipulationSetFieldProperties should be 0
    {
        _ASSERT(false);
        return NULL;
    }
    DSSFormat* lpFormat = mpField->getFormat();
    if(lpFormat)
    {
        if(iReadOnly || mpField->getObjectContext()->getFormatRefCount(lpFormat->getIndex()) <= 1)
            return lpFormat;
        DSSFormat* lpNewFormat = new DSSFormat();
        lpNewFormat->Duplicate(lpFormat);
        lpNewFormat->setIndex(mpField->getObjectContext()->InsertFormat(lpNewFormat, lpFormat->getIndex()));
        mpField->setFormatRefId(lpNewFormat->getIndex());
    }
    else
    {
        DSSFormat* lpNewFormat = new DSSFormat(1, NULL, mpField->getObjectContext());
        lpNewFormat->setIndex(mpField->getObjectContext()->InsertFormat(lpNewFormat));
        mpField->setFormatRefId(lpNewFormat->getIndex());
    }
    return NULL;
}

#ifndef __ANDROID__
std::string DSSCManipulationSetFieldProperties::hBuildXML()
{
    std::string lResult = "";
    DSSObjectContext* lpContext = mpNode->getObjectContext();
    
    std::vector<PropertyInfo*>::iterator lIt = mProperties.begin();
	for(; lIt!=mProperties.end(); lIt++)
    {
        SimpleXMLBuilder lBuilder;
        
        int lTreeType = static_cast<int>(mpNode->getTreeType());
        SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
        lTreeTypeXML.AddChild(lTreeTypeXMLContent);
        
        SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpNode->getKey().c_str(), true));
        lNodeKeyXML.AddChild(lNodeKeyXMLContent);
        
		int lFormatIndex = (*lIt)->mFormatIndex;
        
        SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
        SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
        SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("17", true));
        lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
        
        SimpleXMLNode lFieldKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_field_key"));
        SimpleXMLNode lFieldKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpField->getKey().c_str(), true));
        lFieldKeyXML.AddChild(lFieldKeyXMLContent);
        
        SimpleXMLNode lFormatTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_format_type"));
        SimpleXMLNode lFormatTypeXMLLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lFormatIndex).c_str(), true));
        lFormatTypeXML.AddChild(lFormatTypeXMLLContent);
        
        DSSFormat* lFormat = new DSSFormat(lFormatIndex, &mProperties, lpContext);
        xmlNodePtr lpFormatNode = static_cast<xmlNodePtr> (lFormat->GenerateXMLNode());
        SimpleXMLNode formatNode(lpFormatNode);
        
        lXML.AddChild(lManipulationMethodXML);
        lXML.AddChild(lTreeTypeXML);
        lXML.AddChild(lNodeKeyXML);
        lXML.AddChild(lFieldKeyXML);
        lXML.AddChild(lFormatTypeXML);
        lXML.AddChild(formatNode);
        
        SimpleXMLNode lRoot(lBuilder.GetRoot());
        lRoot.AddChild(lXML);
        
        lResult += lBuilder.ToString(false);
    }
	
	return lResult;
}

void DSSCManipulationSetFieldProperties::hParseXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser("<rw_manipulations>" + iXMLString + "</rw_manipulations>");
	if (!lParser.IsValid())
		return;
	
	SimpleXMLNode lCurrentNode = lParser.GetRoot();
    SimpleXMLNode lMiNode; // rwfmt element
    
    std::string lStrContent;
    std::vector<int> lvFormatIndex;
    
	if (!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulations") != 0)
		return;
    
    for (lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if (lCurrentNode.IsElementNode())
        {
            if (lCurrentNode.GetName().compare("rw_manipulation") == 0)
            {
                std::string lStrContent;
                
                for(SimpleXMLNode lCurrentNode2 = lCurrentNode.GetFirstChild(); lCurrentNode2.IsValid(); lCurrentNode2 = lCurrentNode2.GetNextSibling())
                {
                    if(lCurrentNode2.IsElementNode())
                    {
                        if(lCurrentNode2.GetName().compare("rw_node_key") == 0)
                        {
                            lCurrentNode2.GetContent(lStrContent);
                            mpNode = ipContext->getNode(lStrContent);
                            mpFieldGroupNode = static_cast<DSSRWFieldGroupNode*>(mpNode);
                        }
                        else if(lCurrentNode2.GetName().compare("rw_field_key") == 0)
                        {
                            lCurrentNode2.GetContent(lStrContent);
                            DSSFields* mpFields = mpFieldGroupNode->getFields();
                            mpField = mpFields->FindByKey(lStrContent);
                        }
                        else if(lCurrentNode2.GetName().compare("rwfmt") == 0)
                        {
                            lMiNode = lCurrentNode2;
                        }
                        else if (lCurrentNode2.GetName().compare("rw_format_type") == 0)
                        {
                            lCurrentNode2.GetContent(lStrContent);
                            lvFormatIndex.push_back(DSSExpNode::StringToInt(lStrContent));
                        }
                    }
                }
                
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
                            lSubNode.GetProperty("pri", lStrContent);
                            int lnPri = atoi(lStrContent.c_str());
                            lstrPropertyName = ipContext->getPropertyNameFromID(lstrPropertySetName, lnPri);
                            lValueType = ipContext->getPropertyTypeFromID(lstrPropertySetName, lnPri);
                            lSubNode.GetProperty("v", lStrContent);
                            DSSExpNode::StringToVariant(lStrContent, lValueType, lValue);
                            
                            PropertyInfo* lPropertyInfo = new PropertyInfo(1, lstrPropertySetName, lstrPropertyName, lValue);
                            mProperties.push_back(lPropertyInfo);
                        }
                    }
                }
            }
        }
    }
}
#endif