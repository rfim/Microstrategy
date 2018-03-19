//
//  DSSCManipulationRemoveUnit.cpp
//  ReportServiceCore
//
//  Created by wzhu on 12/28/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

//
//  DSSCManipulationAddUnit.cpp
//  ReportServiceCore
//
//  Created by wzhu on 12/26/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//
#include "DSSCManipulationRemoveUnit.h"
#include "DSSRWTemplateNode.h"
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

DSSCManipulationRemoveUnit::DSSCManipulationRemoveUnit(DSSRWTemplateNode* ipNode, DSSObjectInfo* iObject, bool iShouldRemoveControl)
{
    mpTemplateNode = ipNode;
    mTemplate = mpTemplateNode->getTemplate();
    
    mpUnit = NULL;
    mMetrics = NULL;
    mObject =  iObject;
    mShouldRemoveControl = iShouldRemoveControl;
    
    // initialize the mpunit
    if(mObject)
    {
        GUID lTempID = mObject->getID();
        DSSTemplateUnit* lUnit = NULL;
        
        lUnit = mTemplate->getTemplateUnitFromAxisIndex(DssAxisRows, lTempID, true); // include metric
        /*char lIDStr[33];
         DSSBaseElementProxy::ConvertGUIDToString(lTempID, lIDStr);*/
        if (lUnit)
            mpUnit = lUnit;
        else
            mpUnit = mTemplate->getTemplateUnitFromAxisIndex(DssAxisColumns, lTempID, true); // include metric
        
        if (mpUnit)
        {
            mAxis = mpUnit->getAxis();
            
            EnumDSSTemplateUnitType lUnitType = mpUnit->getType();
            if (lUnitType == DssTemplateMetric)
            {
                mMetrics = (DSSTemplateMetrics*)mTemplate->getTemplateUnitFromAxisIndex(DssAxisAll, lTempID, false);
            }
        }
    }
    
    mType = ManipulationRemoveUnit;
}

DSSCManipulationRemoveUnit::DSSCManipulationRemoveUnit(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationRemoveUnit;
    
    if (!ipAO->IsDeltaXML())
        return;
    
    
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();
    
    // parse lXMLString
    hParseAOXML(lXMLString, ipContext);
#endif
    
}

DSSCManipulationRemoveUnit::~DSSCManipulationRemoveUnit()
{
    int i;
    DSSObjectContext* lpObjectContext = mpTemplateNode->getObjectContext();
    for (i=0; i<mNodesRemoved.size(); i++)
    {
        DSSRWNode* lpNode = mNodesRemoved[i];
        lpObjectContext->RemoveNode(lpNode->getKey());
        delete lpNode;  //delete node
    }
    
    for (i=0; i<mIteratorsRemoved.size(); i++)
    {
        DSSRWIterator* lpIterator = mIteratorsRemoved[i];
        delete lpIterator;
    }
}

int DSSCManipulationRemoveUnit::Execute()
{
    if (!mpTemplateNode || !mObject || !mpUnit)
        return S_FALSE;
    
    mPos = mpUnit->getIndex();
    if (mMetrics)
        mMetricsPos = mMetrics->getIndex();
    DSSObjectContext* lpContext = mpTemplateNode->getObjectContext();
    
    // different logic for metric and other units
    EnumDSSTemplateUnitType lUnitType = mpUnit->getType();
    if (lUnitType != DssTemplateMetric)
    {	
        // remove the unit from the axis
        mAxis->RemoveUnit(mPos, &mpUnit);
        
        // put it into disabled units
        mTemplate->mDisabledUnits.push_back(mpUnit);
    }
    else
    {
        // remove the unit from the axis
        mMetrics->Remove(mPos);
        if (mMetrics->Count() == 0)
            mAxis->RemoveUnit(mMetricsPos, (DSSTemplateUnit**)(&mMetrics));
    
        // put it into disabled metrics
        mTemplate->mDisabledMetrics.push_back((DSSTemplateMetric*)mpUnit);
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
    
    // find control on template with this object
    DSSRWControls* controls = mpTemplateNode->getControls();
    if (controls && mShouldRemoveControl)
    {
        lSize = controls->Count();
        for (int k = 0; k < lSize; k++)
        {
            DSSRWControl* control = controls->Item(k);
            if (control && control->getType()==DssRWControlTypeElementList && control->getSourceObject()->getID() == mpUnit->getID())
            {
                // remove control
                controls->RemoveControlChild(k);
                
                MapFromTargetToControl* lNodesAndControlsMap =  lpContext->getMapFromTargetToControl();
                int lCount = control->getCountTargets();
                for (int i = 0; i < lCount; i++)
                {
                    std::string targetKey = control->getTargetKey(i);
                    
                    // push the target node into node server map
                    if (mNodeMapServer.find(targetKey) == mNodeMapServer.end())
                    {
                        DSSRWNode* lTemp = lpContext->getNode(targetKey);
                        mNodesModelChanged.push_back(lTemp);
                        mNodeMapServer[targetKey] = lTemp;
                    }
                    
                    NodeAndControlsVector sourceArray = (*lNodesAndControlsMap)[targetKey];
                    
                    for (int j = 0; j < sourceArray.size(); j++)
                    {
                        if (sourceArray[j]->NodeKey == mpTemplateNode->getKey())
                        {
                            std::vector<std::string> matchedControls = sourceArray[j]->ControlKeys;
                            for (int l = 0; l < matchedControls.size(); l++)
                            {
                                if (matchedControls[l] == control->getKey())
                                {
                                    matchedControls.erase(matchedControls.begin()+l);
                                    break;
                                }
                            }
                            if (matchedControls.size()==0)
                                sourceArray.erase(sourceArray.begin()+j);
                            
                            break;
                        }
                    }
                    
                    if (sourceArray.size() == 0)
                    {
                        //Remove the target node
                        DSSRWNode* targetNode = lpContext->getNode(targetKey);
                        mNodesModelChanged[mNodesModelChanged.size()-1] = targetNode->Item(0);
                        NodeMap::iterator lBeg = mNodeMapServer.find(targetKey);
                        if(lBeg != mNodeMapServer.end())
                        {
                            mNodeMapServer.erase(lBeg);
                            mNodeMapServer[targetKey] = targetNode->Item(0);
                        }
                        
                        DSSRWNode* lParentNode = targetNode->getParentNode();
                        lParentNode->RemoveGroupByNode(targetNode->getIndex(), &mNodesRemoved, &mIteratorsRemoved);
                        
                        MapFromTargetToControl::iterator lTTC = (*lNodesAndControlsMap).find(targetKey);
                        if (lTTC != (*lNodesAndControlsMap).end())
                            lNodesAndControlsMap->erase(lTTC);
                    }
                }							
                
                break;
            }
        }
    }
    
    // Persist changes
    GenerateAODeltaXML();
    return S_OK;
}

#ifndef __ANDROID__
void DSSCManipulationRemoveUnit::GenerateAODeltaXML()
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
	SimpleXMLNode lRptPivotUnitXMContent(SimpleXMLBuilder::CreateNodePtr("3", true));
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
    
    SimpleXMLNode lDisabledXML(SimpleXMLBuilder::CreateNodePtr("is_disableonly"));
    SimpleXMLNode lDisabledContentXML(SimpleXMLBuilder::CreateNodePtr("1", true));
    lDisabledXML.AddChild(lDisabledContentXML);
    lRptManipulationXML.AddChild(lDisabledXML);
    
	lXML.AddChild(lRptManipulationXML);
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
	lRoot.AddChild(lXML);
    
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationRemoveUnit::hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext)
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
                
                // initialize the mpunit
                GUID lTempID = mObject->getID();
                DSSTemplateUnit* lUnit = NULL;
                
                lUnit = mTemplate->getTemplateUnitFromAxisIndex(DssAxisRows, lTempID, true); // include metric
                if (lUnit)
                    mpUnit = lUnit;
                else
                    mpUnit = mTemplate->getTemplateUnitFromAxisIndex(DssAxisColumns, lTempID, true); // include metric
                
                if (mpUnit)
                {
                    mAxis = mpUnit->getAxis();
                    mPos = mpUnit->getIndex();
                }
			}
		}
    }
}

#else
void DSSCManipulationRemoveUnit::GenerateAODeltaXML()
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
	TiXmlText* lValueText = new TiXmlText("3");
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
    
    TiXmlElement* lDisabledXML = new TiXmlElement("is_disableonly");
    TiXmlText* lDisabledContentXML = new TiXmlText("1");
    lDisabledXML->LinkEndChild(lDisabledContentXML);
    lRptManipulationElement->LinkEndChild(lDisabledXML);
  
    lpRootElement->LinkEndChild(lRptManipulationElement);
  
    TiXmlPrinter printer;
    printer.SetIndent(NULL);
    printer.SetLineBreak(NULL);
  
  lDoc.Accept( &printer );
  (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(printer.Str());
}
#endif
