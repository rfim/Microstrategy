//
//  DSSManipulationUpdateDerivedMetric.cpp
//  ReportServiceCore
//
//  Created by Zhu, Wei on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "DSSManipulationUpdateDerivedMetric.h"
#include "DSSDataElements.h"
#include "DSSDocumentDefinition.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#include "DSSMetric.h"
#include "DSSFormat.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxm.h"
#else
#include "DSSSimpleXML.h"
#endif

#include <iostream>


DSSManipulationUpdateDerivedMetric::DSSManipulationUpdateDerivedMetric(DSSDataElement* iDataElement, std::string iMetricID, std::string iFormula, std::string iName)
{
    mDataElement = iDataElement;
    GUID lTempID = GUID_NULL;
    DSSBaseElementProxy::ConvertFromStringToGUID(iMetricID, lTempID);
    mMetric = mDataElement->getMetrics()->FindByID(lTempID);
    if (mMetric)
        mOldName = mMetric->getName();
    else
        _ASSERT(false); // input metric is null
    
    mNewFormula = iFormula;
    mNewName = iName;
    
    mTemplateNodes.clear();
    
    mType = ManipulationUpdateDerivedMetric;
}

DSSManipulationUpdateDerivedMetric::DSSManipulationUpdateDerivedMetric(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationUpdateDerivedMetric;
    
    if (!ipAO->IsDeltaXML())
        return;
    
    
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();
    
    // parse lXMLString
    hParseAOXML(lXMLString, ipContext);
#endif
    
}

int DSSManipulationUpdateDerivedMetric::Execute()
{
    if (!mDataElement || !mMetric)
        return S_FALSE;
    
    DSSObjectContext* lpContext = mDataElement->getObjectContext();
    bool lDataRetrieval = true;
    if (mNewFormula == ""|| mOldFormula == mNewFormula) 
    {
        lDataRetrieval = false;
        
        if (mOldName == mNewName)
            return S_OK;
    }
    
     // removing templateMetric from template
    DSSDocumentDefinition* lDocDef = lpContext->getDocumentDefinition();
    for (int i = 1; i <= 3; i++)
    {
        DSSRWNode* lRoot = lDocDef->getRootNode((EnumDSSRWTreeType)i);
        GetAffectedTemplateNodes(lRoot);
    }
    
    // update metric name and formula
    if (mNewFormula.size() > 0)
        mMetric->setFormula(mNewFormula);
    if (mNewName.size() > 0)
        mMetric->setName(mNewName);
    
    // Set dataRetrievalKeys
    if (lDataRetrieval)
    {
        for (int j = 0; j < mTemplateNodes.size(); j++)
        {
            std::vector<std::string> lNodes = mTemplateNodes[j]->GetServerNodeKeys();
            
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
        }
    }
    
    // Persist changes
    GenerateAODeltaXML();
    return  S_OK;
}

void DSSManipulationUpdateDerivedMetric::GetAffectedTemplateNodes(DSSRWNode* iCurrentNode)
{
    if  (iCurrentNode->getType() == DssRWNodeTemplate)
    {
        DSSTemplate* lTemplate = ((DSSRWTemplateNode*)iCurrentNode)->getTemplate();
        DSSTemplateMetrics* lTemplateMetrics = lTemplate->getTemplateMetrics();
        
        if (lTemplateMetrics)
        {
            for (int i = 0; i < lTemplateMetrics->Count(); i++)
            {
                DSSTemplateMetric* lMetric = lTemplateMetrics->Item(i);
                if (MBase::IsEqualGUID(lMetric->getMetric()->getID() , mMetric->getID()))
                {
                    mTemplateNodes.push_back((DSSRWTemplateNode*)iCurrentNode);
                    break;
                }
                
            }
        }
    }
    
    int lChildCount = iCurrentNode->Count();
    for (int i = 0; i < lChildCount; i++)
        GetAffectedTemplateNodes(iCurrentNode->Item(i));
}

DSSManipulationUpdateDerivedMetric::~DSSManipulationUpdateDerivedMetric()
{}

#ifndef __ANDROID__
void DSSManipulationUpdateDerivedMetric::GenerateAODeltaXML()
{   
    mpActionObject = new DSSAODeltaXML(ManipulationUpdateDerivedMetric);
    
    // tree type string
    SimpleXMLBuilder lBuilder;
    
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
	SimpleXMLNode lManipulationXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
	SimpleXMLNode lManipulationXMLContent(SimpleXMLBuilder::CreateNodePtr("41", true));
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
    
    std::string lNewID;
    DSSBaseElementProxy::ConvertGUIDToString(mMetric->getID(), lIDString);
	lNewID = (std::string)lIDString;
    
    SimpleXMLNode lMetricXML(SimpleXMLBuilder::CreateNodePtr("metric_id"));
	SimpleXMLNode lMetricXMLContent(SimpleXMLBuilder::CreateNodePtr(lNewID.c_str(), true));
	lMetricXML.AddChild(lMetricXMLContent);
	lXML.AddChild(lMetricXML);
    
    if (mNewFormula != mOldFormula)
    {
        SimpleXMLNode lFormulaXML(SimpleXMLBuilder::CreateNodePtr("derived_metric_formula"));
        SimpleXMLNode lFormulaXMLContent(SimpleXMLBuilder::CreateNodePtr(mNewFormula.c_str(), true));
        lFormulaXML.AddChild(lFormulaXMLContent);
        lXML.AddChild(lFormulaXML);
    }
    
    if (mNewName != mOldName)
    {
        SimpleXMLNode lNameXML(SimpleXMLBuilder::CreateNodePtr("name"));
        SimpleXMLNode lNameXMLContent(SimpleXMLBuilder::CreateNodePtr(mNewName.c_str(), true));
        lNameXML.AddChild(lNameXMLContent);
        lXML.AddChild(lNameXML);
    }
    

    SimpleXMLNode lRoot(lBuilder.GetRoot());
	lRoot.AddChild(lXML);
    
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSManipulationUpdateDerivedMetric::hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext)
{
    SimpleXMLParser lParser(iXMLString);
	if (!lParser.IsValid())
		return;
	
	SimpleXMLNode lCurrentNode = lParser.GetRoot();
	
    std::string lTemp = lCurrentNode.GetName();
    
	if (!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
		return;
	
	std::string lstrContent;
	for (lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
	{
		if (lCurrentNode.IsElementNode())
		{
			if (lCurrentNode.GetName().compare("derived_metric_formula") == 0) // derived metric formula
			{
				lCurrentNode.GetContent(lstrContent);
                mNewFormula = lstrContent;
			}
			else if (lCurrentNode.GetName().compare("name") == 0) // derived metric name
			{
                lCurrentNode.GetContent(lstrContent);
                mNewName = lstrContent;
                
			} else if (lCurrentNode.GetName().compare("metric_id") == 0)
            {
                lCurrentNode.GetContent(lstrContent);
                GUID lTempID = GUID_NULL;
                DSSBaseElementProxy::ConvertFromStringToGUID(lstrContent, lTempID);
                mMetric = mDataElement->getMetrics()->FindByID(lTempID);
                
                mOldName = mMetric->getName();
//                mOldFormula = mMetric->getFormula();
            } else if (lCurrentNode.GetName().compare("rw_data_set_id") == 0)
            {
                lCurrentNode.GetContent(lstrContent);
                GUID lDataElementID = GUID_NULL;
                DSSBaseElementProxy::ConvertFromStringToGUID(lstrContent, lDataElementID);
                mDataElement = ipContext->getDataElements()->FindByID(lDataElementID);
            }
		}
	}
    
    mTemplateNodes.clear();
}
#else
void DSSManipulationUpdateDerivedMetric::GenerateAODeltaXML()
{
    // TODO
}
#endif

