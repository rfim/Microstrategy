//
//  DSSManipulationAddPercentToTotalMetric.cpp
//  ReportServiceCore
//
//  Created by Zhu, Wei on 3/29/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "DSSManipulationAddPercentToTotalMetric.h"
#include "DSSDataElements.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#include "DSSAEHelper.h"
#include "ManipulationHelper.h"
#include "DSSMetric.h"
#include "DSSFormat.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxm.h"
#else
#include "DSSSimpleXML.h"
#endif

#include <iostream>


DSSManipulationAddPercentToTotalMetric::DSSManipulationAddPercentToTotalMetric(DSSRWTemplateNode* iNode, DSSCellHandle* iCellHandle, int iAxesBitMap,  std::vector<DSSTemplateUnit*> iUnits, bool iAggregateFromBase)
{
    mpTemplateNode = iNode;
    mTemplateMetric = NULL;
    
    if (iCellHandle->p.unit->getType() == DssTemplateMetric)
        mTemplateMetric = (DSSTemplateMetric*)(iCellHandle->p.unit);
    
    if (!mTemplateMetric)
        _ASSERT(false);
    
    mAxesBitMap = iAxesBitMap;
    mUnits = iUnits;
    mAggregateFromBase = iAggregateFromBase;
    mNeedNewObjects = true;
    
    mType = ManipulationAddPercentToTotalMetric;
}

DSSManipulationAddPercentToTotalMetric::DSSManipulationAddPercentToTotalMetric(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationAddPercentToTotalMetric;
    
    if (!ipAO->IsDeltaXML())
        return;
    
    
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();
    
    // parse lXMLString
    hParseAOXML(lXMLString, ipContext);
#endif
    
}

int DSSManipulationAddPercentToTotalMetric::Execute()
{
    if (!mpTemplateNode)
        return S_FALSE;
    
    DSSObjectContext* lpContext = mpTemplateNode->getObjectContext();
    DSSTemplate* lTemplate = mpTemplateNode->getTemplate();
    
    mDataElement = lpContext->getDataElements()->FindByID(mpTemplateNode->getDataSourceID());
    
    
    GUID lNewID = ManipulationHelper::createUID();
    mNewMetric = new DSSMetric(lNewID, DssTypeMetric, "", lpContext, lTemplate->getID());
    lpContext->InsertObject(lNewID, mNewMetric);
    
    // Get the template metrics unit
    DSSTemplateMetrics* lTemplateMetrics = lTemplate->getTemplateMetrics();
    if (lTemplateMetrics == NULL)
        return S_FALSE;
    
    // search it in disabled template metrics
    DSSTemplateMetric* lTemplateMetric = lTemplate->FindDisabledMetric(mNewMetric);
    if (lTemplateMetric)
    {
        lTemplateMetrics->Add(lTemplateMetric, mTemplateMetric->getIndex()+1);
        lTemplate->removeDisabledMetric(lTemplateMetric);
    }
    else
    {
        // Create and add a template metric
        lTemplateMetric = new DSSTemplateMetric(lpContext);
        lTemplateMetric->setID(lNewID);
        lTemplateMetric->setAlias(mNewMetric->getName());
        
        CComVariant* lColumnWidth = lTemplate->getPropertyValue("ColumnWidthProperties", "HeaderWidth");
        if (lColumnWidth)
            lTemplateMetric->setColumnWidths(0, lColumnWidth->intVal);
        
        lTemplateMetrics->Add(lTemplateMetric, mTemplateMetric->getIndex()+1);
    }
    
    // add derived metric to data element
    if (mDataElement)
        mDataElement->getMetrics()->Add(mNewMetric->getID());
    
    // TODO: lTemplate->invalidateStyleMap();
    
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
    
    mNewObjects.push_back(mNewMetric);
    
    // Persist changes
    GenerateAODeltaXML();
    return  S_OK;
}

DSSManipulationAddPercentToTotalMetric::~DSSManipulationAddPercentToTotalMetric()
{}

#ifndef __ANDROID__
void DSSManipulationAddPercentToTotalMetric::GenerateAODeltaXML()
{   
    mpActionObject = new DSSAODeltaXML(ManipulationAddPercentToTotalMetric);
    DSSObjectContext* lObjectContext = mpTemplateNode->getObjectContext();
    
    // tree type string
    SimpleXMLBuilder lBuilder;
    
    //DSSXmlRWManipulationReportManipulation
    SimpleXMLNode lXML3(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationXML3(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
	SimpleXMLNode lManipulationXML3Content(SimpleXMLBuilder::CreateNodePtr("19", true));
	lManipulationXML3.AddChild(lManipulationXML3Content);
	lXML3.AddChild(lManipulationXML3);
    
    EnumDSSRWTreeType lTreeType = mpTemplateNode->getTreeType();
	SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
	SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
	lTreeTypeXML.AddChild(lTreeTypeXMLContent);
	lXML3.AddChild(lTreeTypeXML);
	
	std::string lStr = mpTemplateNode->getKey();
	SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
	SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lStr.c_str(), true));
	lNodeKeyXML.AddChild(lNodeKeyXMLContent);
	lXML3.AddChild(lNodeKeyXML);
    
    SimpleXMLNode lRptManipulationXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation"));
    SimpleXMLNode lRptPivotUnitXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation_method"));
	SimpleXMLNode lRptPivotUnitXMContent(SimpleXMLBuilder::CreateNodePtr("23", true));
    lRptPivotUnitXML.AddChild(lRptPivotUnitXMContent);
    lRptManipulationXML.AddChild(lRptPivotUnitXML);
    
    std::string lNewID;
    char lIDString[33];
    DSSBaseElementProxy::ConvertGUIDToString(mTemplateMetric->getID(), lIDString);
	lNewID = (std::string)lIDString;
    
    SimpleXMLNode lMetricXML(SimpleXMLBuilder::CreateNodePtr("metric_id"));
	SimpleXMLNode lMetricXMLContent(SimpleXMLBuilder::CreateNodePtr(lNewID.c_str(), true));
    lMetricXML.AddChild(lMetricXMLContent);
    lRptManipulationXML.AddChild(lMetricXML);
    
    SimpleXMLNode lAxisMapXML(SimpleXMLBuilder::CreateNodePtr("axes_bitmap"));
	SimpleXMLNode lAxisMapXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(mAxesBitMap).c_str(), true));
    lAxisMapXML.AddChild(lAxisMapXMLContent);
    lRptManipulationXML.AddChild(lAxisMapXML);
    
    if (mAxesBitMap == 0)
    {
        SimpleXMLNode lUnitsXML(SimpleXMLBuilder::CreateNodePtr("units_collection"));
        lRptManipulationXML.AddChild(lUnitsXML);
        
        for (int i = 0; i < mUnits.size(); i++)
        {
            std::string lIDStr;
            GUID lUnitID = mUnits[i]->getID();
            DSSBaseElementProxy::ConvertGUIDToString(lUnitID, lIDString);
            lIDStr = (std::string) lIDString;
            
            
            SimpleXMLNode lUnitIDXML(SimpleXMLBuilder::CreateNodePtr("unit_id"));
            SimpleXMLNode lUnitIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lIDStr.c_str(), true));
            lUnitIDXML.AddChild(lUnitIDXMLContent);
            lRptManipulationXML.AddChild(lUnitIDXML);
            
            EnumDSSObjectType lObjecType = lObjectContext->FindObjectH(lUnitID)->getType(); 
            
            SimpleXMLNode lUnitTypeXML(SimpleXMLBuilder::CreateNodePtr("unit_type"));
            SimpleXMLNode lUnitTypeContentXML(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lObjecType).c_str(), true));
            lUnitTypeXML.AddChild(lUnitTypeContentXML);
            lRptManipulationXML.AddChild(lUnitTypeXML);
        }
    }
    
    
	lXML3.AddChild(lRptManipulationXML);
    
    SimpleXMLNode lNewObjectsXML(SimpleXMLBuilder::CreateNodePtr("new_object_ids"));
    SimpleXMLNode lNewObjectXML(SimpleXMLBuilder::CreateNodePtr("new_object_id"));
    lNewObjectsXML.AddChild(lNewObjectXML);
	SimpleXMLNode lNewObjectXMLContent(SimpleXMLBuilder::CreateNodePtr(lNewID.c_str(), true));
	lNewObjectXML.AddChild(lNewObjectXMLContent);
	lXML3.AddChild(lNewObjectsXML);
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML3);
    
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSManipulationAddPercentToTotalMetric::hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext)
{
    SimpleXMLParser lParser(iXMLString);
	if (!lParser.IsValid())
		return;
	
	SimpleXMLNode lCurrentNode = lParser.GetRoot();
	
    std::string lTemp = lCurrentNode.GetName();
    
	if (!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
		return;
    
    mTemplateMetric = NULL;
    
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
                mDataElement = ipContext->getDataElements()->FindByID(mpTemplateNode->getDataSourceID());
			} else if (lCurrentNode.GetName().compare("report_manipulation") == 0) // report manipulation root
			{
                lRptManipulation = lCurrentNode;
			}
		}
	}
    
    // parse report manipulation
    for (lCurrentNode = lRptManipulation.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if (lCurrentNode.IsElementNode())
		{
            if (lCurrentNode.GetName().compare("metric_id") == 0)
			{
                lCurrentNode.GetContent(lstrContent);
                GUID lTempID = GUID_NULL;
                DSSBaseElementProxy::ConvertFromStringToGUID(lstrContent, lTempID);
                
                DSSTemplate* lTemplate = mpTemplateNode->getTemplate();
                DSSTemplateMetrics* lTemplateMetrics = lTemplate->getTemplateMetrics();
                
                if (lTemplateMetrics)
                {
                    for (int i = 0; i < lTemplateMetrics->Count(); i++)
                    {
                        DSSTemplateMetric* lMetric = lTemplateMetrics->Item(i);
                        if (MBase::IsEqualGUID(lMetric->getMetric()->getID() , lTempID))
                        {
                            mTemplateMetric = lMetric;
                            break;
                        }
                        
                    }
                }
			} else if (lCurrentNode.GetName().compare("unit_id") == 0)
            {
                lCurrentNode.GetContent(lstrContent);
                GUID lTempID = GUID_NULL;
                DSSBaseElementProxy::ConvertFromStringToGUID(lstrContent, lTempID);
                DSSTemplate* lTemplate = mpTemplateNode->getTemplate();
                
                if (lTemplate)
                {
                    DSSTemplateUnit* lUnit = lTemplate->getTemplateUnitFromAxisIndex(DssAxisAll, lTempID, true);
                    mUnits.push_back(lUnit);
                }
                
                //mBreakByObjects.push_back(ipContext->FindObjectH(lTempID));
            } else if (lCurrentNode.GetName().compare("axes_bitmap") == 0)
            {
                lCurrentNode.GetContent(lstrContent);
                mAxesBitMap = atoi(lstrContent.c_str());
            }
		}
    }
    
   mAggregateFromBase = false;
   mNeedNewObjects = true;
}
#else
void DSSManipulationAddPercentToTotalMetric::GenerateAODeltaXML()
{
    // TODO
}
#endif


