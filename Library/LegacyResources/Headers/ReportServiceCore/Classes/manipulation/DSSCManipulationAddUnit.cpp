//
//  DSSCManipulationAddUnit.cpp
//  ReportServiceCore
//
//  Created by wzhu on 12/26/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//
#include "DSSCManipulationAddUnit.h"
#include "DSSRWTemplateNode.h"
#include "DSSTemplateAttribute.h"
#include "DSSTemplateConsolidation.h"
#include "DSSTemplateDimension.h"
#include "DSSTemplateCustomGroup.h"
#include "DSSTemplateMetrics.h"
#include "DSSDataElement.h"
#include "DSSDataElements.h"
#include "DSSAEHelper.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#include "DSSBaseElementProxy.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxm.h"
#else
#include "DSSSimpleXML.h"
#endif

#include <iostream>

DSSCManipulationAddUnit::DSSCManipulationAddUnit(DSSRWTemplateNode* ipNode, DSSObjectInfo* iObject, EnumDSSAxisName idAxis, int idPos)
{
    mpTemplateNode = ipNode;
    mTemplate = mpTemplateNode->getTemplate();
    
    mpUnit = NULL;
    
    mObject =  iObject;
    
    if(idAxis == DssAxisRows)
    {
        mAxis = mTemplate->getRow();
    }
    else if(idAxis == DssAxisColumns)
    {
        mAxis = mTemplate->getColumn();
    }
    else
    {
        mAxis = NULL;
    }
    
    mPos = idPos;
    
    mType = ManipulationAddUnitToTemplate;
}

DSSCManipulationAddUnit::DSSCManipulationAddUnit(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationAddUnitToTemplate;
    
    if (!ipAO->IsDeltaXML())
        return;
 
        
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();
    
    // parse lXMLString
    hParseAOXML(lXMLString, ipContext);
#endif
    
}

int DSSCManipulationAddUnit::Execute()
{
    if (!mpTemplateNode || !mObject)
        return S_FALSE;
    
    DSSObjectContext* lpContext = mpTemplateNode->getObjectContext();
    
    EnumDSSObjectType lObjectType = mObject->getType();
    EnumDSSTemplateUnitType lUnitType = DSSAEHelper::ConvertObject2UnitType(lObjectType);
    
    if (lUnitType == DssTemplateReserved)
        return S_FALSE;
    
    DSSDataElement* lElement = lpContext->getDataElements()->FindByID(mpTemplateNode->getDataSourceID());
    mUnitObject = lElement->getUnitObjects()->FindByID(mObject->getID());
    if (!mUnitObject)
        return S_FALSE;
    
    mpUnit = mTemplate->FindDisabledUnit(mObject); 
    bool lIsNewUnit = true;
    
    if (mpUnit) // insert unit
    {
        lIsNewUnit = false;
        mpUnit->setAxis(mAxis);
        
        EnumDSSTemplateUnitType lTempType = mpUnit->getType();
        
        mAxis->InsertUnit(mPos, mpUnit);
        mTemplate->removeDisabledUnit(mpUnit);
        
        if (lTempType == DssTemplateDimension) // replace the object with dimension's object info
            mObject = ((DSSTemplateDimension*)mpUnit)->getObject();
    }
    else // do we need to generate new unit??
    {
        mpUnit = mAxis->AddNewUnit(lUnitType, mPos);
    }
    
    if (mPos >= 0 && mPos < mAxis->Count())
        mpUnit->setIndex(mPos);
    else
        mpUnit->setIndex(mAxis->Count() - 1);
    
    GUID lID = GUID_NULL;
    if (lIsNewUnit)
    {
        switch (lObjectType)
        {
            case DssTypeAttribute:
            {
                DSSTemplateAttribute* lAttribute = (DSSTemplateAttribute*) mpUnit;
                lAttribute->setAlias(mObject->getName());
                lID = mObject->getID();
                lAttribute->setID(lID);
                lAttribute->PopulateDefaultForms();
                break;
            }
            case DssTypeConsolidation:
            {
                DSSTemplateConsolidation* lConsolidation = (DSSTemplateConsolidation*) mpUnit;
                lConsolidation->setAlias(mObject->getName());
                lID = mObject->getID();
                lConsolidation->setID(lID);
                break;
            }
            case DssTypeFilter:
            {
                DSSTemplateCustomGroup* lCustomGroup = (DSSTemplateCustomGroup*) mpUnit;
                lCustomGroup->setAlias(mObject->getName());
                lID = mObject->getID();
                lCustomGroup->setID(lID);
                lCustomGroup->setFlatten(true);
                break;
            }
            case DssTypeDimension:
                // TODO
                break;
            default:
                break;
        }
        
        mpUnit->setAlias(mUnitObject->getAlias());
        mpUnit->setAxis(mAxis);
        vector<int> lFormatRefIDs = mUnitObject->getFormatRefIDs();
        if (lFormatRefIDs.size() >= 2) // populate format from unitObject to templateMetric
        {
            mpUnit->setResolvedFormatRefID(HEADER, lFormatRefIDs[HEADER]);
            mpUnit->setResolvedFormatRefID(GRID, lFormatRefIDs[GRID]);
        }
        
        int lNum = lFormatRefIDs.size();
        for (int i = 0; i < lNum; i++)
        {
            if (lFormatRefIDs[i] <= 0 || lNum < 2)
            {
                mpUnit->SetInvalidateResolvedFormat(true);
                break;
            }
        }
    }
    
    
    mNodesModelChanged.push_back(mpTemplateNode);
    mNodeMapServer[mpTemplateNode->getKey()] = mpTemplateNode;
    
    std::vector<std::string> lNodes = mpTemplateNode->GetServerNodeKeys();
    int lSize = lNodes.size();
    for (int i = 0; i < lSize; i++)
    {
        std::string lKey = lNodes[i];
        if (mNodeMapServer.find(lKey) != mNodeMapServer.end())
            continue;
        
        DSSRWNode* lNode = lpContext->getNode(lKey);
        
        mNodesModelChanged.push_back(lNode);
        mNodeMapServer[lKey] = lNode;
    }
    
    //push the key of metric selector controlling the template node to node server map
    AddMetricConditionSelectorsToMap(mpTemplateNode);
    
    // Persist changes
    GenerateAODeltaXML();
    return  S_OK;
}

DSSCManipulationAddUnit::~DSSCManipulationAddUnit()
{}

#ifndef __ANDROID__
void DSSCManipulationAddUnit::GenerateAODeltaXML()
{   
    DSSObjectContext* lObjectContext = mpTemplateNode->getObjectContext();
    mpActionObject = new DSSAODeltaXML(ManipulationPivotUnit);
    
    // tree type string
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
    SimpleXMLNode lRptPivotUnitXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation_method"));
	SimpleXMLNode lRptPivotUnitXMContent(SimpleXMLBuilder::CreateNodePtr("1", true));
    lRptPivotUnitXML.AddChild(lRptPivotUnitXMContent);
    lRptManipulationXML.AddChild(lRptPivotUnitXML);
    
    SimpleXMLNode lDatasetIDXML(SimpleXMLBuilder::CreateNodePtr("rw_data_set_id"));
    char lDatasetID[33];
    DSSBaseElementProxy::ConvertGUIDToString(mpTemplateNode->getDataSourceID(), lDatasetID);
    lStr = lDatasetID;
	SimpleXMLNode lDatasetIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lStr.c_str(), true));
    lDatasetIDXML.AddChild(lDatasetIDXMLContent);
    lRptManipulationXML.AddChild(lDatasetIDXML);
    
    SimpleXMLNode lDatasetTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_data_set_type"));
	SimpleXMLNode lDatasetTypeXMLContent(SimpleXMLBuilder::CreateNodePtr("3", true));
    lDatasetTypeXML.AddChild(lDatasetTypeXMLContent);
    lRptManipulationXML.AddChild(lDatasetTypeXML);
    
    SimpleXMLNode lUnitIDXML(SimpleXMLBuilder::CreateNodePtr("unit_id"));
    char lID[33];
    GUID lGUID = mpUnit->getID();
    
    DSSBaseElementProxy::ConvertGUIDToString(lGUID, lID);
    lStr = lID;
    SimpleXMLNode lUnitIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lStr.c_str(), true));
    lUnitIDXML.AddChild(lUnitIDXMLContent);
    lRptManipulationXML.AddChild(lUnitIDXML);
    
    EnumDSSObjectType lObjecType = DssTypeReserved;
    DSSObjectInfo* lpObjectInfo = lObjectContext->FindObjectH(lGUID);
    if (lpObjectInfo)
        lObjecType = lpObjectInfo->getType();
    
    SimpleXMLNode lUnitTypeXML(SimpleXMLBuilder::CreateNodePtr("unit_type"));
    SimpleXMLNode lUnitTypeContentXML(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lObjecType).c_str(), true));
    lUnitTypeXML.AddChild(lUnitTypeContentXML);
    lRptManipulationXML.AddChild(lUnitTypeXML);
    
    SimpleXMLNode lAxisNameXML(SimpleXMLBuilder::CreateNodePtr("axis_name"));
    SimpleXMLNode lAxisNameContentXML(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(mAxis->getIndex()).c_str(), true));
    lAxisNameXML.AddChild(lAxisNameContentXML);
    lRptManipulationXML.AddChild(lAxisNameXML);
    
    SimpleXMLNode lUnitPosXML(SimpleXMLBuilder::CreateNodePtr("unit_position"));
    SimpleXMLNode lUnitPosContentXML(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(mPos+1).c_str(), true));
    lUnitPosXML.AddChild(lUnitPosContentXML);
    lRptManipulationXML.AddChild(lUnitPosXML);
    
    SimpleXMLNode lDisplayNameXML(SimpleXMLBuilder::CreateNodePtr("unit_display_name"));
    SimpleXMLNode lDisplayNameContentXML(SimpleXMLBuilder::CreateNodePtr(mpUnit->getAlias().c_str(), true));
    lDisplayNameXML.AddChild(lDisplayNameContentXML);
    lRptManipulationXML.AddChild(lDisplayNameXML);
    
	lXML.AddChild(lRptManipulationXML);
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
	lRoot.AddChild(lXML);
    
    // add forms
    if(mObject->getType() == DssTypeAttribute)
    {
        DSSTemplateAttribute* lpAttribute = static_cast<DSSTemplateAttribute*>(mpUnit);
        DSSAttributeFormWrappers* lpForms = lpAttribute->getFormWrappers();
        for(int i = 0; i < lpForms->Count(); i++)
        {
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
            
            SimpleXMLNode lAddFormXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation_method"));
            SimpleXMLNode lAddFormXMContent(SimpleXMLBuilder::CreateNodePtr("46", true));
            lAddFormXML.AddChild(lAddFormXMContent);
            lRptManipulationXML.AddChild(lAddFormXML);
            
            char lAttributeID[33];
            DSSBaseElementProxy::ConvertGUIDToString(mObject->getID(), lAttributeID);
            SimpleXMLNode lAttributeIDXML(SimpleXMLBuilder::CreateNodePtr("attribute_id"));
            SimpleXMLNode lAttributeIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lAttributeID, true));
            lAttributeIDXML.AddChild(lAttributeIDXMLContent);
            lRptManipulationXML.AddChild(lAttributeIDXML);
            
            char lFormID[33];
            DSSBaseElementProxy::ConvertGUIDToString(lpForms->Item(i)->getID(), lFormID);
            SimpleXMLNode lFormIDXML(SimpleXMLBuilder::CreateNodePtr("form_id"));
            SimpleXMLNode lFormIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lFormID, true));
            lFormIDXML.AddChild(lFormIDXMLContent);
            lRptManipulationXML.AddChild(lFormIDXML);
            
            SimpleXMLNode lFormIndexXML(SimpleXMLBuilder::CreateNodePtr("index"));
            SimpleXMLNode lFormIndexXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(i+1).c_str(), true));
            lFormIndexXML.AddChild(lFormIndexXMLContent);
            lRptManipulationXML.AddChild(lFormIndexXML);
            
            lXML.AddChild(lRptManipulationXML);
            lRoot.AddChild(lXML);
        }
    }
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationAddUnit::hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext)
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
                if (mpTemplateNode)
                    mTemplate = mpTemplateNode->getTemplate();
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
			if (lCurrentNode.GetName().compare("unit_id") == 0)
			{
				lCurrentNode.GetContent(lstrContent);
                DSSBaseElementProxy::ConvertFromStringToGUID(lstrContent, lTempID);
                
                mObject = ipContext->FindObjectH(lTempID);
			}
			else if (lCurrentNode.GetName().compare("unit_type") == 0)
			{
                lCurrentNode.GetContent(lstrContent);
                lRptManipulation = lCurrentNode;
			}
            else if (lCurrentNode.GetName().compare("axis_name") == 0)
			{
                lCurrentNode.GetContent(lstrContent);
                int lAxis = atoi(lstrContent.c_str());
                if (lAxis == 0)
                    mAxis = mTemplate->getRow();
                else
                    mAxis = mTemplate->getColumn();
			}
            else if (lCurrentNode.GetName().compare("unit_position") == 0)
			{
                lCurrentNode.GetContent(lstrContent);
                mPos = atoi(lstrContent.c_str()) - 1;
			}
		}
    }
    
}

#else
void DSSCManipulationAddUnit::GenerateAODeltaXML()
{
    DSSObjectContext* lObjectContext = mpTemplateNode->getObjectContext();
    
   	TiXmlDocument lDoc;
	TiXmlElement* lpRootElement = new TiXmlElement("rw_manipulation");
    
	TiXmlElement* lpManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
	TiXmlText* lpManipulationMethodText = new TiXmlText("19");
	lpManipulationMethodElement->LinkEndChild(lpManipulationMethodText);
	lpRootElement->LinkEndChild(lpManipulationMethodElement);
    
    EnumDSSRWTreeType lTreeType = mpTemplateNode->getTreeType();
	TiXmlElement* lpTreeTypeElement = new TiXmlElement("rw_tree_type");
	TiXmlText* lpTreeTypeText = new TiXmlText(DSSExpNode::IntToString(lTreeType).c_str());
	lpTreeTypeElement->LinkEndChild(lpTreeTypeText);
	lpRootElement->LinkEndChild(lpTreeTypeElement);
    
	std::string lStr = mpTemplateNode->getKey();
	TiXmlElement* lpNodeKeyElement = new TiXmlElement("rw_node_key");
	TiXmlText* lpNodeKeyText = new TiXmlText(lStr);
	lpNodeKeyElement->LinkEndChild(lpNodeKeyText);
	lpRootElement->LinkEndChild(lpNodeKeyElement);
    
	TiXmlElement* lRptManipulationElement = new TiXmlElement("report_manipulation");
    TiXmlElement* lRptManipulationMethodElement = new TiXmlElement("report_manipulation_method");
	TiXmlText* lValueText = new TiXmlText("1");
	lRptManipulationMethodElement->LinkEndChild(lValueText);
	lRptManipulationElement->LinkEndChild(lRptManipulationMethodElement);
    
    EnumDSSTemplateUnitType lUnitType = mpUnit->getType();

    TiXmlElement* lUnitIDXML = new TiXmlElement("unit_id");
    char lID[33];
    GUID lGUID = mpUnit->getID();
    
    DSSBaseElementProxy::ConvertGUIDToString(lGUID, lID);
    lStr = lID;
    TiXmlText* lUnitIDXMLContent = new TiXmlText(lStr);
    lUnitIDXML->LinkEndChild(lUnitIDXMLContent);
    lRptManipulationElement->LinkEndChild(lUnitIDXML);
    
    EnumDSSObjectType lObjecType = lObjectContext->FindObjectH(lGUID)->getType();
    TiXmlElement* lUnitTypeXML = new TiXmlElement("unit_type");
    TiXmlText* lUnitTypeXMLContent = new TiXmlText(DSSExpNode::IntToString(lObjecType).c_str());
    lUnitTypeXML->LinkEndChild(lUnitTypeXMLContent);
    lRptManipulationElement->LinkEndChild(lUnitTypeXML);
    
    TiXmlElement* lAxisNameXML = new TiXmlElement("axis_name");
    TiXmlText* lAxisNameContentXML = new TiXmlText(DSSExpNode::IntToString(mAxis->getIndex()).c_str());
    lAxisNameXML->LinkEndChild(lAxisNameContentXML);
    lRptManipulationElement->LinkEndChild(lAxisNameXML);
    
    TiXmlElement* lUnitPosXML = new TiXmlElement("unit_position");
    TiXmlText* lAxisNameContentXML = new TiXmlText(DSSExpNode::IntToString(mPos+1).c_str());
    lUnitPosXML->LinkEndChild(lUnitPosContentXML);
    lRptManipulationElement->LinkEndChild(lUnitPosXML);
    
    TiXmlElement* lDisplayNameXML = new TiXmlElement("unit_display_name");
    TiXmlText* lDisplayNameContentXML = new TiXmlText((mpUnit->getAlias().c_str());
    lDisplayNameXML->LinkEndChild(lDisplayNameContentXML);
    lRptManipulationElement->LinkEndChild(lDisplayNameXML);
    
    lpRootElement->LinkEndChild(lRptManipulationElement);
    
	TiXmlPrinter printer;
	printer.SetIndent(NULL);
	printer.SetLineBreak(NULL);
    
    lDoc.LinkEndChild(lpRootElement);
	lDoc.Accept( &printer );
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(printer.Str());
}
#endif