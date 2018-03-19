//
//  DSSManipulationAddDerivedMetricToGrid.cpp
//  ReportServiceCore
//
//  Created by Zhu, Wei on 3/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "DSSManipulationAddDerivedMetricToGrid.h"
#include "DSSDataElements.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
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


DSSManipulationAddDerivedMetricToGrid::DSSManipulationAddDerivedMetricToGrid(DSSRWTemplateNode* ipNode, int idPos, std::string iFormula, std::string iName, std::vector<PropertyInfo*> iProperties, bool iAggregateFromBase)
{
    mpTemplateNode = ipNode;
    mNewMetric = NULL;
    mPos = idPos;
    mFormula = iFormula;
    mName = iName;
    mProperties = iProperties;
    mAggregateFromBase = iAggregateFromBase;
    
    mNeedNewObjects = true;
    mDataElement = NULL;
    
    mType = ManipulationAddDerivedMetricToGrid;
}

DSSManipulationAddDerivedMetricToGrid::DSSManipulationAddDerivedMetricToGrid(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationAddDerivedMetricToGrid;
    
    if (!ipAO->IsDeltaXML())
        return;
    
    
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();
    
    // parse lXMLString
    hParseAOXML(lXMLString, ipContext);
#endif
    
}

int DSSManipulationAddDerivedMetricToGrid::Execute()
{
    if (!mpTemplateNode)
        return S_FALSE;
    
    DSSObjectContext* lpContext = mpTemplateNode->getObjectContext();
    DSSTemplate* lTemplate = mpTemplateNode->getTemplate();
    
    mDataElement = lpContext->getDataElements()->FindByID(mpTemplateNode->getDataSourceID());
    
    mName = hGetUniqueName(mName);
    
    GUID lNewID = ManipulationHelper::createUID();
    mNewMetric = new DSSMetric(lNewID, DssTypeMetric, mName, lpContext, lTemplate->getID());
    lpContext->InsertObject(lNewID, mNewMetric);
    
    // Get the template metrics unit
    DSSTemplateMetrics* lTemplateMetrics = lTemplate->getTemplateMetrics();
    if (lTemplateMetrics == NULL)
    {
        DSSTemplateUnit* lUnit = lTemplate->getColumn()->AddNewUnit(DssTemplateMetrics, -1);
        lTemplateMetrics = (DSSTemplateMetrics*) lUnit;
    }
    
     // search it in disabled template metrics
    DSSTemplateMetric* lTemplateMetric = lTemplate->FindDisabledMetric(mNewMetric);
    if (lTemplateMetric)
    {
        lTemplateMetrics->Add(lTemplateMetric, mPos);
        lTemplate->removeDisabledMetric(lTemplateMetric);
    }
    else
    {
        // Create and add a template metric
        lTemplateMetric = new DSSTemplateMetric(lpContext);
        lTemplateMetric->setID(lNewID);
        lTemplateMetric->setAlias(mName);
        
        CComVariant* lColumnWidth = lTemplate->getPropertyValue("ColumnWidthProperties", "HeaderWidth");
        if (lColumnWidth)
            lTemplateMetric->setColumnWidths(0, lColumnWidth->intVal);
        
        lTemplateMetrics->Add(lTemplateMetric, mPos);
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

DSSManipulationAddDerivedMetricToGrid::~DSSManipulationAddDerivedMetricToGrid()
{}

std::string DSSManipulationAddDerivedMetricToGrid::hGetUniqueName(std::string iName) 
{
    bool lIsUniqueName = false;
    std::string lAlias;
    std::string sName = iName;
    int lCounter = 0;
    int i = 0;
    
    while (!lIsUniqueName)
    {
        lIsUniqueName = true;
        DSSMetrics* lMetrics = mDataElement->getMetrics();
        
        for (i = 0; i < lMetrics->Count(); i++)
        {
            DSSMetric* lDerivedMetric = lMetrics->Item(i);
            lAlias = lDerivedMetric->getName();
            if (lAlias == iName)
            {
                lIsUniqueName = false;
                lCounter++;
                iName = sName + " (" + DSSExpNode::IntToString(lCounter) + ")";
                break;
            }
        }
    }
    
    return iName;
}

#ifndef __ANDROID__
void DSSManipulationAddDerivedMetricToGrid::GenerateAODeltaXML()
{       
    DSSObjectContext* lObjectContext = mpTemplateNode->getObjectContext();
    mpActionObject = new DSSAODeltaXML(ManipulationAddDerivedMetricToGrid);
    
    // tree type string
    SimpleXMLBuilder lBuilder;
    
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
	SimpleXMLNode lManipulationXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
	SimpleXMLNode lManipulationXMLContent(SimpleXMLBuilder::CreateNodePtr("40", true));
	lManipulationXML.AddChild(lManipulationXMLContent);
	lXML.AddChild(lManipulationXML);
    
    std::string lDatasetID;
    char lIDString[33];
	DSSBaseElementProxy::ConvertGUIDToString(mDataElement->getDatasetID(), lIDString);
	lDatasetID = (std::string)lIDString;
    
    SimpleXMLNode lDatasetIDXML(SimpleXMLBuilder::CreateNodePtr("rw_data_set_id"));
	SimpleXMLNode lDatasetIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lDatasetID.c_str(), true));
	lDatasetIDXML.AddChild(lDatasetIDXMLContent);
	lXML.AddChild(lDatasetIDXML);
    
    SimpleXMLNode lDatasetTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_data_set_type"));
	SimpleXMLNode lDatasetTypeXMLContent(SimpleXMLBuilder::CreateNodePtr("3", true));
	lDatasetTypeXML.AddChild(lDatasetTypeXMLContent);
	lXML.AddChild(lDatasetTypeXML);
    
    SimpleXMLNode lFormulaXML(SimpleXMLBuilder::CreateNodePtr("derived_metric_formula"));
	SimpleXMLNode lFormulaXMLContent(SimpleXMLBuilder::CreateNodePtr(mFormula.c_str(), true));
	lFormulaXML.AddChild(lFormulaXMLContent);
	lXML.AddChild(lFormulaXML);
    
    SimpleXMLNode lNameXML(SimpleXMLBuilder::CreateNodePtr("name"));
	SimpleXMLNode lNameXMLContent(SimpleXMLBuilder::CreateNodePtr(mName.c_str(), true));
	lNameXML.AddChild(lNameXMLContent);
	lXML.AddChild(lNameXML);
    
    int lAggregateFromBaseInt = 0;
    if (mAggregateFromBase)
        lAggregateFromBaseInt = 1;
    else
        lAggregateFromBaseInt = 0;
    SimpleXMLNode lAggregateFromBaseXML(SimpleXMLBuilder::CreateNodePtr("aggregate_from_base"));
	SimpleXMLNode lAggregateFromBaseXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lAggregateFromBaseInt).c_str(), true));
	lAggregateFromBaseXML.AddChild(lAggregateFromBaseXMLContent);
	lXML.AddChild(lAggregateFromBaseXML);
    
    std::string lNewID;
    DSSBaseElementProxy::ConvertGUIDToString(mNewMetric->getID(), lIDString);
	lNewID = (std::string)lIDString;
    
    SimpleXMLNode lNewObjectsXML(SimpleXMLBuilder::CreateNodePtr("new_object_ids"));
    SimpleXMLNode lNewObjectXML(SimpleXMLBuilder::CreateNodePtr("new_object_id"));
    lNewObjectsXML.AddChild(lNewObjectXML);
	SimpleXMLNode lNewObjectXMLContent(SimpleXMLBuilder::CreateNodePtr(lNewID.c_str(), true));
	lNewObjectXML.AddChild(lNewObjectXMLContent);
	lXML.AddChild(lNewObjectsXML);
    
    // DSSXmlRWManipulationFormatDerivedMetric
    SimpleXMLNode lXML2(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
	SimpleXMLNode lManipulationXML2(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
	SimpleXMLNode lManipulationXML2Content(SimpleXMLBuilder::CreateNodePtr("46", true));
	lManipulationXML2.AddChild(lManipulationXML2Content);
	lXML2.AddChild(lManipulationXML2);
    
    
    SimpleXMLNode lDatasetIDXML2(SimpleXMLBuilder::CreateNodePtr("rw_data_set_id"));
	SimpleXMLNode lDatasetIDXML2Content(SimpleXMLBuilder::CreateNodePtr(lDatasetID.c_str(), true));
	lDatasetIDXML2.AddChild(lDatasetIDXML2Content);
	lXML2.AddChild(lDatasetIDXML2);
    
    SimpleXMLNode lDatasetTypeXML2(SimpleXMLBuilder::CreateNodePtr("rw_data_set_type"));
	SimpleXMLNode lDatasetTypeXML2Content(SimpleXMLBuilder::CreateNodePtr("3", true));
	lDatasetTypeXML2.AddChild(lDatasetTypeXML2Content);
	lXML2.AddChild(lDatasetTypeXML2);
    
    SimpleXMLNode lMetricXML(SimpleXMLBuilder::CreateNodePtr("metric_id"));
	SimpleXMLNode lMetricXMLContent(SimpleXMLBuilder::CreateNodePtr(lNewID.c_str(), true));
	lMetricXML.AddChild(lMetricXMLContent);
	lXML2.AddChild(lMetricXML);
    
    // For Test
    //<rwfmt><prs n="FormattingNumber"><pr pri="4" v="$"/><pr pri="6" v="#,##0;(#,##0)"/></prs></rwfmt>
    //std::string lFormatStr = "<rwfmt><prs n=\"FormattingNumber\"><pr pri=\"4\" v=\"$\"/><pr pri=\"6\" v=\"#,##0;(#,##0)\"/></prs></rwfmt>";
    // SimpleXMLParser lParser(lFormatStr);
    
    DSSFormat* lFormat = new DSSFormat(1, &mProperties, lObjectContext);
    /*
    SimpleXMLParser lParser(lFormat->GenerateXML());
    SimpleXMLNode lSCFFormatXML = lParser.GetRoot();
    lXML2.AddChild(lSCFFormatXML);
    */
    xmlNodePtr lpFormatNode = static_cast<xmlNodePtr> (lFormat->GenerateXMLNode());
    SimpleXMLNode formatNode(lpFormatNode);
    lXML2.AddChild(formatNode);
    
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
	SimpleXMLNode lRptPivotUnitXMContent(SimpleXMLBuilder::CreateNodePtr("2", true));
    lRptPivotUnitXML.AddChild(lRptPivotUnitXMContent);
    lRptManipulationXML.AddChild(lRptPivotUnitXML);
    
    SimpleXMLNode lUnitIDXML(SimpleXMLBuilder::CreateNodePtr("unit_id"));
    SimpleXMLNode lUnitIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lNewID.c_str(), true));
    lUnitIDXML.AddChild(lUnitIDXMLContent);
    lRptManipulationXML.AddChild(lUnitIDXML);
    
    SimpleXMLNode lUnitTypeXML(SimpleXMLBuilder::CreateNodePtr("unit_type"));
    SimpleXMLNode lUnitTypeContentXML(SimpleXMLBuilder::CreateNodePtr("4", true));
    lUnitTypeXML.AddChild(lUnitTypeContentXML);
    lRptManipulationXML.AddChild(lUnitTypeXML);
    
    SimpleXMLNode lUnitPosXML(SimpleXMLBuilder::CreateNodePtr("unit_position"));
    SimpleXMLNode lUnitPosContentXML(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(mPos+1).c_str(), true));
    lUnitPosXML.AddChild(lUnitPosContentXML);
    lRptManipulationXML.AddChild(lUnitPosXML);
    
    SimpleXMLNode lDisplayNameXML(SimpleXMLBuilder::CreateNodePtr("unit_display_name"));
    SimpleXMLNode lDisplayNameContentXML(SimpleXMLBuilder::CreateNodePtr(mName.c_str(), true));
    lDisplayNameXML.AddChild(lDisplayNameContentXML);
    lRptManipulationXML.AddChild(lDisplayNameXML);
	lXML3.AddChild(lRptManipulationXML);
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
	lRoot.AddChild(lXML);
    lRoot.AddChild(lXML2);
    lRoot.AddChild(lXML3);
    
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSManipulationAddDerivedMetricToGrid::hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext)
{
     SimpleXMLParser lParser("<rw_manipulations>" + iXMLString + "</rw_manipulations>");
	if (!lParser.IsValid())
		return;
	
	SimpleXMLNode lCurrentNode = lParser.GetRoot();
	
    std::string lTemp = lCurrentNode.GetName();
    
	if (!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulations") != 0)
		return;
	
	SimpleXMLNode lRWManipulation1;
    SimpleXMLNode lRWManipulation2;
    SimpleXMLNode lRWManipulation3;
    int lIndex = 1;
    
    for (lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if (lCurrentNode.IsElementNode())
        {
            if (lCurrentNode.GetName().compare("rw_manipulation") == 0)
            {
                if (lIndex == 1)
                    lRWManipulation1 = lCurrentNode;
                else if (lIndex == 2)
                    lRWManipulation2 = lCurrentNode;
                else if (lIndex == 3)
                    lRWManipulation3 = lCurrentNode;
                    
                lIndex++;
                
            }
                
        }
    }
    
	std::string lstrContent;
	for (lCurrentNode = lRWManipulation1.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
	{
		if (lCurrentNode.IsElementNode())
		{
			if (lCurrentNode.GetName().compare("derived_metric_formula") == 0) // derived metric formula
			{
				lCurrentNode.GetContent(lstrContent);
                mFormula = lstrContent;
			}
			else if (lCurrentNode.GetName().compare("name") == 0) // derived metric name
			{
                lCurrentNode.GetContent(lstrContent);
                 mName = lstrContent;
                
			} else if (lCurrentNode.GetName().compare("aggregate_from_base") == 0)
            {
                lCurrentNode.GetContent(lstrContent);
                mAggregateFromBase = (lstrContent == "1");
            }
		}
	}
    
    SimpleXMLNode lFormatNode;
    for (lCurrentNode = lRWManipulation2.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
	{
		if (lCurrentNode.IsElementNode())
		{
			if (lCurrentNode.GetName().compare("rwfmt") == 0) // format node
			{
				lFormatNode = lCurrentNode;
			}
		}
	}
    
    std::string lstrPropertySetName;
    std::string lstrPropertyName;
    EnumVariantType lValueType;
    CComVariant lValue;
    SimpleXMLNode lSubNode;
    for (lCurrentNode = lFormatNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode.GetNextSibling())
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
                            PropertyInfo* lPropertyInfo = new PropertyInfo(1, lstrPropertySetName, lstrPropertyName, lValue);
                            mProperties.push_back(lPropertyInfo);
                        }
                    }
                }
            }
        }
    }

    SimpleXMLNode lRptManipulation;
	for (lCurrentNode = lRWManipulation3.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
	{
		if (lCurrentNode.IsElementNode())
		{
			if (lCurrentNode.GetName().compare("rw_node_key") == 0) // template node
			{
				lCurrentNode.GetContent(lstrContent);
                mpTemplateNode = (DSSRWTemplateNode*)(ipContext->getNode(lstrContent));
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
            if (lCurrentNode.GetName().compare("unit_position") == 0)
			{
                lCurrentNode.GetContent(lstrContent);
                mPos = atoi(lstrContent.c_str()) - 1;
			}
		}
    }
    
    mDataElement = NULL;
    mNewMetric = NULL;
    mNeedNewObjects = true;
}
#else
void DSSManipulationAddDerivedMetricToGrid::GenerateAODeltaXML()
{
    // TODO
}
#endif
