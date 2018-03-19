//
//  DSSManipulationRemoveDerivedMetricFromDataset.cpp
//  ReportServiceCore
//
//  Created by Zhu, Wei on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "DSSManipulationRemoveDerivedMetricFromDataset.h"
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


DSSManipulationRemoveDerivedMetricFromDataset::DSSManipulationRemoveDerivedMetricFromDataset(DSSDataElement* iDataElement, std::string iMetricID)
{
    mDataElement = iDataElement;
    mMetricID = iMetricID;
    
    GUID lTempID = GUID_NULL;
    DSSBaseElementProxy::ConvertFromStringToGUID(iMetricID, lTempID);
    mMetric = mDataElement->getMetrics()->FindByID(lTempID);
    
    mTemplateNodes.clear();
    
    mType = ManipulationRemoveDerivedMetricFromDataset;
}

DSSManipulationRemoveDerivedMetricFromDataset::DSSManipulationRemoveDerivedMetricFromDataset(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationRemoveDerivedMetricFromDataset;
    
    if (!ipAO->IsDeltaXML())
        return;
    
    
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();
    
    // parse lXMLString
    hParseAOXML(lXMLString, ipContext);
#endif
    
}

int DSSManipulationRemoveDerivedMetricFromDataset::Execute()
{
    if (!mDataElement || !mMetric)
        return S_FALSE;
    
    DSSObjectContext* lpContext = mDataElement->getObjectContext();
    
    // removing templateMetric from template
    DSSDocumentDefinition* lDocDef = lpContext->getDocumentDefinition();
    for (int i = 1; i <= 3; i++)
    {
        DSSRWNode* lRoot = lDocDef->getRootNode((EnumDSSRWTreeType)i);
        RemoveMetricFromTemplateNodes(lRoot);
    }
    
    // delete element from _dataElement. New function removeDerivedMetric() called
    if (mDataElement)
        mDataElement->getMetrics()->RemoveByID(mMetric->getID());
    
    // Set dataRetrievalKeys
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
    
    // Persist changes
    GenerateAODeltaXML();
    return  S_OK;
}

void DSSManipulationRemoveDerivedMetricFromDataset::RemoveMetricFromTemplateNodes(DSSRWNode* iCurrentNode)
{
    if  (iCurrentNode->getType() == DssRWNodeTemplate)
    {
        DSSTemplate* lTemplate = ((DSSRWTemplateNode*)iCurrentNode)->getTemplate();
        DSSTemplateMetrics* lTemplateMetrics = lTemplate->getTemplateMetrics();
        bool lTemplateChanged = false;
        
        if (lTemplateMetrics)
        {
            for (int i = 0; i < lTemplateMetrics->Count(); i++)
            {
                DSSTemplateMetric* lMetric = lTemplateMetrics->Item(i);
                if (MBase::IsEqualGUID(lMetric->getMetric()->getID() , mMetric->getID()))
                {
                    lTemplateMetrics->Remove(lMetric->getIndex());
                    lTemplateChanged = true;
                }
                
            }
        }
        
        if (lTemplateChanged)
            mTemplateNodes.push_back((DSSRWTemplateNode*)iCurrentNode);
    }
    
    int lChildCount = iCurrentNode->Count();
    for (int i = 0; i < lChildCount; i++)
        RemoveMetricFromTemplateNodes(iCurrentNode->Item(i));
}

DSSManipulationRemoveDerivedMetricFromDataset::~DSSManipulationRemoveDerivedMetricFromDataset()
{}

#ifndef __ANDROID__
void DSSManipulationRemoveDerivedMetricFromDataset::GenerateAODeltaXML()
{   
    mpActionObject = new DSSAODeltaXML(ManipulationUpdateDerivedMetric);
    
    // tree type string
    SimpleXMLBuilder lBuilder;
    
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
	SimpleXMLNode lManipulationXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
	SimpleXMLNode lManipulationXMLContent(SimpleXMLBuilder::CreateNodePtr("42", true));
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
    
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
	lRoot.AddChild(lXML);
    
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSManipulationRemoveDerivedMetricFromDataset::hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext)
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
            if (lCurrentNode.GetName().compare("metric_id") == 0)
            {
                lCurrentNode.GetContent(lstrContent);
                mMetricID = lstrContent;
                GUID lTempID = GUID_NULL;
                DSSBaseElementProxy::ConvertFromStringToGUID(lstrContent, lTempID);
                mMetric = mDataElement->getMetrics()->FindByID(lTempID);
                
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
void DSSManipulationRemoveDerivedMetricFromDataset::GenerateAODeltaXML()
{
    // TODO
}
#endif

