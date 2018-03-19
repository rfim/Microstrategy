//
//  DSSCManipulationMoveMetric.cpp
//  ReportServiceCore
//
//  Created by wzhu on 12/30/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "DSSCManipulationMoveMetric.h"
#include "DSSRWTemplateNode.h"
#include "DSSDataModelXTabImpl.h"
#include "DSSTemplateConsolidation.h"
#include "DSSTemplateDimension.h"
#include "DSSTemplateCustomGroup.h"
#include "DSSTemplateMetrics.h"
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

DSSCManipulationMoveMetric::DSSCManipulationMoveMetric(DSSRWTemplateNode* ipNode,
                                                                         DSSObjectInfo* iObject, int idPos)
{
    mpTemplateNode = ipNode;
    mTemplate = mpTemplateNode->getTemplate();
    
    mpUnit = NULL;
    
    mObject =  iObject;
    mPos = idPos;
    
    mType = ManipulationMoveMetric;
}

DSSCManipulationMoveMetric::DSSCManipulationMoveMetric(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationMoveMetric;
    
    if (!ipAO->IsDeltaXML())
        return;
    
    
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();
    
    // parse lXMLString
    hParseAOXML(lXMLString, ipContext);
#endif
    
}

int DSSCManipulationMoveMetric::Execute()
{
    if (!mpTemplateNode || !mObject || !mTemplate)
        return S_FALSE;
    
    if (mPos < 0)
        mPos = 0;
    
    //DSSObjectContext* lpContext = mpTemplateNode->getObjectContext();
    DSSTemplateMetrics* lMetrics = mTemplate->getTemplateMetrics();
    
    GUID lTempID = mObject->getID();
    mpUnit = (DSSTemplateMetric*) mTemplate->getTemplateUnitFromAxisIndex(DssAxisAll, lTempID, true); // include metric
    
    if (!mpUnit)
        return S_FALSE;
    
    if (lMetrics)
        lMetrics->Move(mpUnit, mPos);
    
    // refresh xtabmodel
    //mTemplate->getXTabModel()->MoveMetric(lMetrics);
    //mTemplate->getXTabModel()->Refresh();
    
    // record changed node
    mNodesModelChanged.push_back(mpTemplateNode);
    mNodeMapServer[mpTemplateNode->getKey()]=mpTemplateNode;
    
    // Persist changes
    GenerateAODeltaXML();
    return S_OK;
}

DSSCManipulationMoveMetric::~DSSCManipulationMoveMetric()
{}

#ifndef __ANDROID__
void DSSCManipulationMoveMetric::GenerateAODeltaXML()
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
	SimpleXMLNode lRptPivotUnitXMContent(SimpleXMLBuilder::CreateNodePtr("2", true));
    lRptPivotUnitXML.AddChild(lRptPivotUnitXMContent);
    lRptManipulationXML.AddChild(lRptPivotUnitXML);
    
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
    
    SimpleXMLNode lUnitPosXML(SimpleXMLBuilder::CreateNodePtr("unit_position"));
    SimpleXMLNode lUnitPosContentXML(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(mpUnit->getIndex()+1).c_str(), true));
    lUnitPosXML.AddChild(lUnitPosContentXML);
    lRptManipulationXML.AddChild(lUnitPosXML);
    
    SimpleXMLNode lDisplayNameXML(SimpleXMLBuilder::CreateNodePtr("unit_display_name"));
    SimpleXMLNode lDisplayNameContentXML(SimpleXMLBuilder::CreateNodePtr(mpUnit->getAlias().c_str(), true));
    lDisplayNameXML.AddChild(lDisplayNameContentXML);
    lRptManipulationXML.AddChild(lDisplayNameXML);
    
	lXML.AddChild(lRptManipulationXML);
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
	lRoot.AddChild(lXML);
    
    std::cout << lBuilder.ToString(false) << endl;
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationMoveMetric::hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext)
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
            else if (lCurrentNode.GetName().compare("unit_position") == 0)
			{
                lCurrentNode.GetContent(lstrContent);
                mPos = atoi(lstrContent.c_str()) - 1;
			}
		}
    }
    
}

#else
void DSSCManipulationMoveMetric::GenerateAODeltaXML()
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

    lDoc.Accept( &printer );
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(printer.Str());
}
#endif
