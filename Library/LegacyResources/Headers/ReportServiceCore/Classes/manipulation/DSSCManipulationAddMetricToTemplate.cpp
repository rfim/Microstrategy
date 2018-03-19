//
//  DSSCManipulationAddMetricToTemplate.cpp
//  ReportServiceCore
//
//  Created by wzhu on 12/29/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "DSSCManipulationAddMetricToTemplate.h"
#include "DSSRWTemplateNode.h"
#include "DSSTemplateConsolidation.h"
#include "DSSTemplateDimension.h"
#include "DSSTemplateCustomGroup.h"
#include "DSSTemplateMetrics.h"
#include "DSSAEHelper.h"
#include "DSSDataElements.h"
#include "DSSDataElement.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#include "DSSBaseElementProxy.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxm.h"
#else
#include "DSSSimpleXML.h"
#endif

#include <iostream>

DSSCManipulationAddMetricToTemplate::DSSCManipulationAddMetricToTemplate(DSSRWTemplateNode* ipNode,
                                                                           DSSObjectInfo* iObject, EnumDSSAxisName idAxis, int idPos)
{
    mpTemplateNode = ipNode;
    mTemplate = mpTemplateNode->getTemplate();
    
    mpUnit = NULL;
    mUnitObject = NULL;
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
    mHasTemplateMetrics = true;
    mType = ManipulationAddMetricToTemplate;
}

DSSCManipulationAddMetricToTemplate::DSSCManipulationAddMetricToTemplate(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationAddMetricToTemplate;
    
    if (!ipAO->IsDeltaXML())
        return;
    
    
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();
    
    // parse lXMLString
    hParseAOXML(lXMLString, ipContext);
#endif
    
}

int DSSCManipulationAddMetricToTemplate::Execute()
{
    if (!mpTemplateNode || !mObject)
        return S_FALSE;
    
    DSSObjectContext* lpContext = mpTemplateNode->getObjectContext();
    DSSTemplateMetrics* lMetrics = mTemplate->getTemplateMetrics();
    if (lMetrics == NULL)
    {
        lMetrics = (DSSTemplateMetrics*)mAxis->AddNewUnit(DssTemplateMetrics, -1);
        lMetrics->setAxis(mAxis);
        mHasTemplateMetrics = false;
    }
    
    bool lNewObject = false;
    mpUnit = mTemplate->FindDisabledMetric(mObject);
    if (mpUnit)
    {
        lMetrics->Add(mpUnit, mPos);
        mTemplate->removeDisabledMetric(mpUnit);
        mpUnit->setAxis(mAxis);
    }
    else // do we really need to generate new unit??
    {
        lNewObject = true;
        mpUnit = new DSSTemplateMetric(lpContext);
        mpUnit->setAlias(mObject->getName());
        mpUnit->setAxis(mAxis);
        GUID lTempID = mObject->getID();
        mpUnit->setID(lTempID);
        
        CComVariant* lColumnWidth = mTemplate->getPropertyValue("ColumnWidthProperties", "HeaderWidth");
        if (lColumnWidth && lColumnWidth->intVal != -1)
            mpUnit->setColumnWidths(0, lColumnWidth->intVal);
        
        lMetrics->Add(mpUnit, mPos);
    }
    
    DSSDataElement* lElement = lpContext->getDataElements()->FindByID(mpTemplateNode->getDataSourceID());
    mUnitObject = lElement->getUnitObjects()->FindByID(mObject->getID());
    
    vector<int> lFormatRefIDs;
    
    if (mUnitObject)	// if lUnitObject is null, it means the _metric is in dataElement.derivedMetrics
    {
        mpUnit->setAlias(mUnitObject->getAlias());
        
        lFormatRefIDs = mUnitObject->getFormatRefIDs();
        if (lFormatRefIDs.size() >= 2) // populate format from unitObject to templateMetric
        {
            DSSTemplateMetric* lTemplateMetric = (DSSTemplateMetric*)mpUnit;
            lTemplateMetric->setFormatRefID(HEADER, lFormatRefIDs[HEADER]);
            lTemplateMetric->setFormatRefID(GRID, lFormatRefIDs[GRID]);
            lTemplateMetric->setResolvedFormatRefID(HEADER, lFormatRefIDs[HEADER]);
            lTemplateMetric->setResolvedFormatRefID(GRID, lFormatRefIDs[GRID]);
        }
    }	
    
    if (lNewObject)
    {
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
    return S_OK;
}

DSSCManipulationAddMetricToTemplate::~DSSCManipulationAddMetricToTemplate()
{}

#ifndef __ANDROID__
void DSSCManipulationAddMetricToTemplate::GenerateAODeltaXML()
{   
    DSSObjectContext* lObjectContext = mpTemplateNode->getObjectContext();
    mpActionObject = new DSSAODeltaXML(ManipulationAddMetricToTemplate);
    
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
    
    if (mHasTemplateMetrics)
    {
        SimpleXMLNode lRptPivotUnitXMContent(SimpleXMLBuilder::CreateNodePtr("2", true));
        lRptPivotUnitXML.AddChild(lRptPivotUnitXMContent);
    }
    else // clark, if there is no template metrics, use addunit instead of addmetric
    {
        SimpleXMLNode lRptPivotUnitXMContent(SimpleXMLBuilder::CreateNodePtr("1", true));
        lRptPivotUnitXML.AddChild(lRptPivotUnitXMContent);
    }
    
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
    
    EnumDSSObjectType lObjecType = lObjectContext->FindObjectH(lGUID)->getType();
    SimpleXMLNode lUnitTypeXML(SimpleXMLBuilder::CreateNodePtr("unit_type"));
    SimpleXMLNode lUnitTypeContentXML(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lObjecType).c_str(), true));
    lUnitTypeXML.AddChild(lUnitTypeContentXML);
    lRptManipulationXML.AddChild(lUnitTypeXML);
    
    if (!mHasTemplateMetrics) // clark, if there is no template metrics, tell server which axis the metric is added to
    {
        SimpleXMLNode lAxisNameXML(SimpleXMLBuilder::CreateNodePtr("axis_name"));
        SimpleXMLNode lAxisNameContentXML(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(mAxis->getIndex()).c_str(), true));
        lAxisNameXML.AddChild(lAxisNameContentXML);
        lRptManipulationXML.AddChild(lAxisNameXML);
    }
    
    SimpleXMLNode lUnitPosXML(SimpleXMLBuilder::CreateNodePtr("unit_position"));
    if (!mHasTemplateMetrics)   //if no template metrics, we add the index of the templateMetrics in the grid
    {
        int lPos = mTemplate->getTemplateMetrics()->getIndex();
        SimpleXMLNode lUnitPosXML1(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lPos+1).c_str(), true));
        lUnitPosXML.AddChild(lUnitPosXML1);
    }
    else    //else if we have template metrics, add the index of the template metric in the template metrics
    {
        SimpleXMLNode lUnitPosContentXML(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(mPos+1).c_str(), true));
        lUnitPosXML.AddChild(lUnitPosContentXML);
    }
    //SimpleXMLNode lUnitPosContentXML(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(mPos+1).c_str(), true));
    //lUnitPosXML.AddChild(lUnitPosContentXML);
    lRptManipulationXML.AddChild(lUnitPosXML);
    
    SimpleXMLNode lDisplayNameXML(SimpleXMLBuilder::CreateNodePtr("unit_display_name"));
    SimpleXMLNode lDisplayNameContentXML(SimpleXMLBuilder::CreateNodePtr(mpUnit->getAlias().c_str(), true));
    lDisplayNameXML.AddChild(lDisplayNameContentXML);
    lRptManipulationXML.AddChild(lDisplayNameXML);
    
	lXML.AddChild(lRptManipulationXML);
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
	lRoot.AddChild(lXML);
    
   (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationAddMetricToTemplate::hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext)
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
void DSSCManipulationAddMetricToTemplate::GenerateAODeltaXML()
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
	TiXmlText* lValueText = new TiXmlText("2");
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
    
    //TiXmlElement* lAxisNameXML = new TiXmlElement("axis_name");
    //TiXmlText* lAxisNameContentXML = new TiXmlText(DSSExpNode::IntToString(mAxis->getIndex()).c_str());
    //lAxisNameXML->LinkEndChild(lAxisNameContentXML);
    //lRptManipulationElement->LinkEndChild(lAxisNameXML);
    
    TiXmlElement* lUnitPosXML = new TiXmlElement("unit_position");
    //TODO: this logic is wrong!! The iOS code is correct
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
  
    lDoc.Accept( &printer );
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(printer.Str());
}
#endif
