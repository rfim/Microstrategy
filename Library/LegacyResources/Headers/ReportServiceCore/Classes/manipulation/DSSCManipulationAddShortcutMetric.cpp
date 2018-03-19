//
//  DSSCManipulationAddShortcutMetric.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 3/28/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#include "DSSCManipulationAddShortcutMetric.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#include "DSSRWTemplateNode.h"
#include "DSSTemplateUnit.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif
#include "DSSBaseElementProxy.h"
#include "ManipulationHelper.h"

DSSCManipulationAddShortcutMetric::DSSCManipulationAddShortcutMetric(DSSRWTemplateNode* ipTemplateNode, DSSDataElement* ipDataElement, std::vector<DSSTemplateUnit*> iUnits, std::vector<std::string> iFunctionParameters, EnumDSSFunctionType iFunctionType1, EnumDSSFunctionType iFunctionType2, std::vector<DSSSort*> iSortByUnits, std::vector<DSSObjectInfo*> iBreakByUnits, std::vector<PropertyInfo*> iProperties)
{
    mpTemplateNode = ipTemplateNode;
    mpDataElement = ipDataElement;
    mUnits = iUnits;
    mFunctionParameters = iFunctionParameters;
    mFunctionType1 = iFunctionType1;
    mFunctionType2 = iFunctionType2;
    mSortByUnits = iSortByUnits;
    mBreakByUnits = iBreakByUnits;
    mProperties = iProperties;
    
    mType = ManipulationAddShortcutMetric;
}

DSSCManipulationAddShortcutMetric::DSSCManipulationAddShortcutMetric(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationAddShortcutMetric;
    
    if(!ipAO->IsDeltaXML())
        return;
    
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
    std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
    hParseAOXML(lXMLString, ipContext);
#endif
}

DSSCManipulationAddShortcutMetric::~DSSCManipulationAddShortcutMetric()
{
    
}

int DSSCManipulationAddShortcutMetric::Execute()
{
    if (!mpTemplateNode)
        return S_FALSE;
    
    DSSObjectContext* lpContext = mpTemplateNode->getObjectContext();
    DSSTemplate* lTemplate = mpTemplateNode->getTemplate();
    
    mpDataElement = lpContext->getDataElements()->FindByID(mpTemplateNode->getDataSourceID());
    
    mName = hGetUniqueName(mName);
    
    GUID lNewID = ManipulationHelper::createUID();
    mpNewMetric = new DSSMetric(lNewID, DssTypeMetric, mName, lpContext, lTemplate->getID());

    if (mpDataElement)
        mpDataElement->getMetrics()->Add(mpNewMetric->getID());
    
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

    GenerateAODeltaXML();
    mNeedNewObjects = true;
    mNewObjects.push_back(mpNewMetric);
    
    return S_OK;
}

std::string DSSCManipulationAddShortcutMetric::hGetUniqueName(std::string iName) 
{
    bool lIsUniqueName = false;
    std::string lAlias;
    std::string sName = iName;
    int lCounter = 0;
    int i = 0;
    
    while (!lIsUniqueName)
    {
        lIsUniqueName = true;
        DSSMetrics* lMetrics = mpDataElement->getMetrics();
        
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
void DSSCManipulationAddShortcutMetric::GenerateAODeltaXML()
{
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("115", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    lXML.AddChild(lManipulationMethodXML);
    
    char lDataSetID[33];
    DSSBaseElementProxy::ConvertGUIDToString(mpDataElement->getDatasetID(), lDataSetID);
    SimpleXMLNode lDataSetIDXML(SimpleXMLBuilder::CreateNodePtr("rw_data_set_id"));
    SimpleXMLNode lDataSetIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lDataSetID, true));
    lDataSetIDXML.AddChild(lDataSetIDXMLContent);
    lXML.AddChild(lDataSetIDXML);
    
    SimpleXMLNode lDataSetTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_data_set_type"));
    SimpleXMLNode lDataSetTypexMLContent(SimpleXMLBuilder::CreateNodePtr("3", true));
    lDataSetTypeXML.AddChild(lDataSetTypexMLContent);
    lXML.AddChild(lDataSetTypeXML);
    
    int lFunctionType1 = static_cast<int>(mFunctionType1);
    SimpleXMLNode lFunctionType1XML(SimpleXMLBuilder::CreateNodePtr("function_type"));
    SimpleXMLNode lFunctionType1XMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lFunctionType1).c_str(), true));
    lFunctionType1XML.AddChild(lFunctionType1XMLContent);
    lXML.AddChild(lFunctionType1XML);
    
    if(mFunctionType2 != DssFunctionTypeReserved)
    {
        int lFunctionType2 = static_cast<int>(mFunctionType2);
        SimpleXMLNode lFunctionType2XML(SimpleXMLBuilder::CreateNodePtr("function_type_2"));
        SimpleXMLNode lFunctionType2XMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lFunctionType2).c_str(), true));
        lFunctionType2XML.AddChild(lFunctionType2XMLContent);
        lXML.AddChild(lFunctionType2XML);
    }
    
    if(mpTemplateNode)
    {
        int lTreeType = static_cast<int>(mpTemplateNode->getTreeType());
        SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
        lTreeTypeXML.AddChild(lTreeTypeXMLContent);
        lXML.AddChild(lTreeTypeXML);
        
        SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
        lNodeKeyXML.AddChild(lNodeKeyXMLContent);
        lXML.AddChild(lNodeKeyXML);
    }
    
    char lMetricID[33];
    DSSBaseElementProxy::ConvertGUIDToString(mpNewMetric->getID(), lMetricID);
    SimpleXMLNode lNewObjectIDXML(SimpleXMLBuilder::CreateNodePtr("new_object_ids"));
    SimpleXMLNode lMetricIDXML(SimpleXMLBuilder::CreateNodePtr("new_object_id"));
    SimpleXMLNode lMetricIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lMetricID, true));
    lMetricIDXML.AddChild(lMetricIDXMLContent);
    lNewObjectIDXML.AddChild(lMetricIDXML);
    lXML.AddChild(lNewObjectIDXML);
    
    if(mUnits.size() > 0)
    {
        SimpleXMLNode lUnitsXML(SimpleXMLBuilder::CreateNodePtr("units_collection"));
        for(std::vector<DSSTemplateUnit*>::iterator lUnitsIter = mUnits.begin(); lUnitsIter != mUnits.end(); lUnitsIter++)
        {
            DSSTemplateUnit* lpTemplateUnit = *lUnitsIter;
            char lUnitID[33];
            DSSBaseElementProxy::ConvertGUIDToString(lpTemplateUnit->getID(), lUnitID);
            SimpleXMLNode lUnitIDXML(SimpleXMLBuilder::CreateNodePtr("unit_id"));
            SimpleXMLNode lUnitIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lUnitID, true));
            lUnitIDXML.AddChild(lUnitIDXMLContent);
            lUnitsXML.AddChild(lUnitIDXML);
            
            int lUnitType = static_cast<int>(lpTemplateUnit->getType());
            SimpleXMLNode lUnitTypeXML(SimpleXMLBuilder::CreateNodePtr("unit_type"));
            SimpleXMLNode lUnitTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lUnitType).c_str(), true));
            lUnitTypeXML.AddChild(lUnitTypeXMLContent);
            lUnitsXML.AddChild(lUnitTypeXML);
        }
        lXML.AddChild(lUnitsXML);
    }

    SimpleXMLNode lAuxiliaryXML(SimpleXMLBuilder::CreateNodePtr("auxiliary_parameter"));
    
    SimpleXMLNode lNameXML(SimpleXMLBuilder::CreateNodePtr("name"));
    SimpleXMLNode lNameXMLContent(SimpleXMLBuilder::CreateNodePtr(mName.c_str(), true));
    lNameXML.AddChild(lNameXMLContent);
    lAuxiliaryXML.AddChild(lNameXML);
    
    if(mSortByUnits.size() > 0)
    {
        SimpleXMLNode lSortsXML(SimpleXMLBuilder::CreateNodePtr("sorts"));
        for(std::vector<DSSSort*>::iterator lSortsIter = mSortByUnits.begin(); lSortsIter != mSortByUnits.end(); lSortsIter++)
        {
            SimpleXMLNode lSortXML(SimpleXMLBuilder::CreateNodePtr("sort"));
            
            DSSSort* lpSort = *lSortsIter;
            char lSortByUnitID[33];
            DSSBaseElementProxy::ConvertGUIDToString(lpSort->getObjectID(), lSortByUnitID);
            SimpleXMLNode lSortByUnitIDXML(SimpleXMLBuilder::CreateNodePtr("unit_id"));
            SimpleXMLNode lSortByUnitIDXMLContent(SimpleXMLBuilder::CreateNodePtr("unit_type"));
            lSortByUnitIDXML.AddChild(lSortByUnitIDXMLContent);
            lSortXML.AddChild(lSortByUnitIDXML);
            
            int lSortByUnitType = static_cast<int>(lpSort->getObjectType());
            SimpleXMLNode lSortByUnitTypeXML(SimpleXMLBuilder::CreateNodePtr("unit_type"));
            SimpleXMLNode lSortByUnitTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lSortByUnitType).c_str(), true));
            lSortByUnitTypeXML.AddChild(lSortByUnitTypeXMLContent);
            lSortXML.AddChild(lSortByUnitTypeXML);
            
            int lAscending = static_cast<int>(lpSort->getAscending());
            SimpleXMLNode lAscendingXML(SimpleXMLBuilder::CreateNodePtr("is_ascending"));
            SimpleXMLNode lAscendingXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lAscending).c_str(), true));
            lAscendingXML.AddChild(lAscendingXMLContent);
            lSortXML.AddChild(lAscendingXML);
            
            if(lpSort->getObjectType() == DssTypeAttribute)
            {
                char lFormID[33];
                DSSBaseElementProxy::ConvertGUIDToString(lpSort->getFormID(), lFormID);
                SimpleXMLNode lFormIDXML(SimpleXMLBuilder::CreateNodePtr("form_id"));
                SimpleXMLNode lFormIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lFormID, true));
                lFormIDXML.AddChild(lFormIDXMLContent);
                lSortXML.AddChild(lFormIDXML);
            }
            lSortsXML.AddChild(lSortXML);
        }
        lAuxiliaryXML.AddChild(lSortsXML);
    }
    
    if(mBreakByUnits.size() > 0)
    {
        SimpleXMLNode lBreakByUnitsXML(SimpleXMLBuilder::CreateNodePtr("break_by_units"));
        for(std::vector<DSSObjectInfo*>::iterator lBreakByUnitIter = mBreakByUnits.begin(); lBreakByUnitIter != mBreakByUnits.end(); lBreakByUnitIter++)
        {
            DSSObjectInfo* lBreakByUnit = *lBreakByUnitIter;
            
            char lBreakByUnitID[33];
            DSSBaseElementProxy::ConvertGUIDToString(lBreakByUnit->getID(), lBreakByUnitID);
            SimpleXMLNode lBreakByUnitIDXML(SimpleXMLBuilder::CreateNodePtr("unit_id"));
            SimpleXMLNode lBreakByUnitIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lBreakByUnitID, true));
            lBreakByUnitIDXML.AddChild(lBreakByUnitIDXMLContent);
            lBreakByUnitsXML.AddChild(lBreakByUnitIDXML);
            
            int lBreakByUnitType = static_cast<int>(lBreakByUnit->getType());
            SimpleXMLNode lBreakByUnitTypeXML(SimpleXMLBuilder::CreateNodePtr("unit_type"));
            SimpleXMLNode lBreakByUnitTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lBreakByUnitType).c_str(), true));
            lBreakByUnitTypeXML.AddChild(lBreakByUnitTypeXMLContent);
            lBreakByUnitsXML.AddChild(lBreakByUnitTypeXML);
        }
        lAuxiliaryXML.AddChild(lBreakByUnitsXML);
    }
    
    if(mProperties.size() > 0)
    {
        DSSFormat* lpFormat = new DSSFormat(1, &mProperties, mpTemplateNode->getObjectContext());
        xmlNodePtr lpFormatNode = static_cast<xmlNodePtr> (lpFormat->GenerateXMLNode());
        SimpleXMLNode lFormatXML(lpFormatNode);
        lAuxiliaryXML.AddChild(lFormatXML);
    }
    
    if(mFunctionParameters.size() > 0)
    {
        SimpleXMLNode lfprmsXML(SimpleXMLBuilder::CreateNodePtr("fprms"));
        for(std::vector<std::string>::iterator lfprmIter = mFunctionParameters.begin(); lfprmIter != mFunctionParameters.end(); lfprmIter++)
        {
            SimpleXMLNode lfprmXML(SimpleXMLBuilder::CreateNodePtr("fprm"));
            SimpleXMLNode lfprmXMLContent(SimpleXMLBuilder::CreateNodePtr(lfprmIter->c_str(), true));
            lfprmXML.AddChild(lfprmXMLContent);
            lfprmsXML.AddChild(lfprmXML);
        }
        lAuxiliaryXML.AddChild(lfprmsXML);
    }
    lXML.AddChild(lAuxiliaryXML);
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    DSSAODeltaXML* lpAO = new DSSAODeltaXML();
    lpAO->setXMLString(lBuilder.ToString(false));
    mActionObjects.push_back(lpAO);
    
    SimpleXMLBuilder lBuilder2;
    SimpleXMLNode lXML2(SimpleXMLBuilder::CreateNodePtr("rw_manipulaiton"));
    
    SimpleXMLNode lManipulationMethodXML2(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent2(SimpleXMLBuilder::CreateNodePtr("46"));
    lManipulationMethodXML2.AddChild(lManipulationMethodXMLContent2);
    
    char lDataSetID2[33];
    DSSBaseElementProxy::ConvertGUIDToString(mpDataElement->getDatasetID(), lDataSetID2);
    SimpleXMLNode lDataSetIDXML2(SimpleXMLBuilder::CreateNodePtr("rw_data_set_id"));
    SimpleXMLNode lDataSetIDXMLContent2(SimpleXMLBuilder::CreateNodePtr(lDataSetID, true));
    lDataSetIDXML2.AddChild(lDataSetIDXMLContent2);
    lXML2.AddChild(lDataSetIDXML2);
    
    SimpleXMLNode lDataSetTypeXML2(SimpleXMLBuilder::CreateNodePtr("rw_data_set_type"));
    SimpleXMLNode lDataSetTypexMLContent2(SimpleXMLBuilder::CreateNodePtr("3", true));
    lDataSetTypeXML2.AddChild(lDataSetTypexMLContent2);
    lXML2.AddChild(lDataSetTypeXML2);
    
    char lMetricID2[33];
    DSSBaseElementProxy::ConvertGUIDToString(mpNewMetric->getID(), lMetricID2);
    SimpleXMLNode lMetricIDXML2(SimpleXMLBuilder::CreateNodePtr("metric_id"));
    SimpleXMLNode lMetricIDXMLContent2(SimpleXMLBuilder::CreateNodePtr(lMetricID2, true));
    lMetricIDXML2.AddChild(lMetricIDXMLContent2);
    lXML2.AddChild(lMetricIDXML2);
    
    if(mProperties.size() > 0)
    {
        DSSFormat* lpFormat = new DSSFormat(1, &mProperties, mpTemplateNode->getObjectContext());
        xmlNodePtr lpFormatNode = static_cast<xmlNodePtr> (lpFormat->GenerateXMLNode());
        SimpleXMLNode lFormatXML(lpFormatNode);
        lXML2.AddChild(lFormatXML);
    }
    
    SimpleXMLNode lRoot2(lBuilder.GetRoot());
    lRoot2.AddChild(lXML2);
    DSSAODeltaXML* lpAO2 = new DSSAODeltaXML();
    lpAO->setXMLString(lBuilder2.ToString(false));
    mActionObjects.push_back(lpAO2);
}

void DSSCManipulationAddShortcutMetric::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser(iXMLString);
    if(!lParser.IsValid())
        return;
    SimpleXMLNode lCurrentNode(lParser.GetRoot());
    if(!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
        return;
    
    std::string lStrContent;
    SimpleXMLNode lMiNode;
    for(lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if(lCurrentNode.IsElementNode())
        {
            if(lCurrentNode.GetName().compare("function_type") == 0)
            {
                lCurrentNode.GetContent(lStrContent);
                mFunctionType1 = static_cast<EnumDSSFunctionType>(DSSExpNode::StringToInt(lStrContent));
            }
            else if(lCurrentNode.GetName().compare("function_type_2") == 0)
            {
                lCurrentNode.GetContent(lStrContent);
                mFunctionType2 = static_cast<EnumDSSFunctionType>(DSSExpNode::StringToInt(lStrContent));
            }
            else if(lCurrentNode.GetName().compare("rw_data_set_id") == 0)
            {
                GUID lDataSetID;
                lCurrentNode.GetContent(lStrContent);
                DSSBaseElementProxy::ConvertFromStringToGUID(lStrContent, lDataSetID);
                if(!mpDataElement)
                    mpDataElement = new DSSDataElement(lDataSetID, ipContext);
            }
            else if(lCurrentNode.GetName().compare("rw_node_key") == 0)
            {
                lCurrentNode.GetContent(lStrContent);
                mpTemplateNode = static_cast<DSSRWTemplateNode*>(ipContext->getNode(lStrContent));
            }
            else if(lCurrentNode.GetName().compare("units_collection") == 0)
            {
                for(SimpleXMLNode lCurrentUnitNode = lCurrentNode.GetFirstChild(); lCurrentUnitNode.IsValid(); lCurrentUnitNode = lCurrentUnitNode.GetNextSibling())
                {
                    if(lCurrentUnitNode.GetName().compare("unit_id") == 0)
                    {
                        lCurrentUnitNode.GetContent(lStrContent);
                        GUID lObjectID;
                        DSSBaseElementProxy::ConvertFromStringToGUID(lStrContent, lObjectID);
                        DSSTemplateUnit* lpUnit = new DSSTemplateUnit(ipContext);
                        lpUnit->setID(lObjectID);
                        mUnits.push_back(lpUnit);
                    }
                }
            }
            else if(lCurrentNode.GetName().compare("auxiliary_parameter") == 0)
            {
                for(SimpleXMLNode lCurrentAuxNode = lCurrentNode.GetFirstChild(); lCurrentAuxNode.IsValid(); lCurrentAuxNode = lCurrentAuxNode.GetNextSibling())
                {
                    if(lCurrentAuxNode.GetName().compare("name") == 0)
                    {
                        lCurrentAuxNode.GetContent(lStrContent);
                        mName = lStrContent;
                    }
                    else if(lCurrentAuxNode.GetName().compare("sorts") == 0)
                    {
                        for(SimpleXMLNode lCurrentSortNode = lCurrentAuxNode.GetFirstChild().GetFirstChild(); lCurrentSortNode.IsValid(); lCurrentSortNode = lCurrentSortNode.GetNextSibling())
                        {
                            DSSSort* lpSort = new DSSSort(ipContext);
                            if(lCurrentSortNode.GetName().compare("unit_id") == 0)
                            {
                                lCurrentSortNode.GetContent(lStrContent);
                                GUID lObjectID;
                                DSSBaseElementProxy::ConvertFromStringToGUID(lStrContent, lObjectID);
                                lpSort->setObjectID(lObjectID);
                                mSortByUnits.push_back(lpSort);
                            }
                            else if(lCurrentAuxNode.GetName().compare("ascending") == 0)
                            {
                                lCurrentSortNode.GetContent(lStrContent);
                                if(lStrContent.compare("1") == 0)
                                    lpSort->setAscending(true);
                                else
                                    lpSort->setAscending(false);
                            }
                            else if(lCurrentAuxNode.GetName().compare("form_id") == 0)
                            {
                                lCurrentSortNode.GetContent(lStrContent);
                                GUID lFormID;
                                DSSBaseElementProxy::ConvertFromStringToGUID(lStrContent, lFormID);
                                lpSort->setFormID(lFormID);
                            }
                        }
                    }
                    else if(lCurrentAuxNode.GetName().compare("break_by_units") == 0)
                    {
                        for(SimpleXMLNode lCurrentBreakByNode = lCurrentAuxNode.GetFirstChild(); lCurrentBreakByNode.IsValid(); lCurrentBreakByNode = lCurrentBreakByNode.GetNextSibling())
                        {
                            if(lCurrentBreakByNode.GetName().compare("unit_id") == 0)
                            {
                                GUID lObjectID;
                                DSSBaseElementProxy::ConvertFromStringToGUID(lStrContent, lObjectID);
                                mBreakByUnits.push_back(ipContext->FindObjectH(lObjectID));
                            }
                        }
                    }
                    else if(lCurrentAuxNode.GetName().compare("rw_fmt") == 0)
                    {
                        lMiNode = lCurrentAuxNode;
                    }
                    else if(lCurrentAuxNode.GetName().compare("fprms") == 0)
                    {
                        for(SimpleXMLNode lfprmNode = lCurrentAuxNode.GetFirstChild(); lfprmNode.IsValid(); lfprmNode = lfprmNode.GetNextSibling())
                        {
                            lfprmNode.GetContent(lStrContent);
                            mFunctionParameters.push_back(lStrContent);
                        }
                    }
                }
            }
            else if(lCurrentNode.GetName().compare("metric_id") == 0)
            {
                lCurrentNode.GetContent(lStrContent);
                GUID lMetricID;
                DSSBaseElementProxy::ConvertFromStringToGUID(lStrContent, lMetricID);
                mpNewMetric = static_cast<DSSMetric*>(ipContext->FindObjectH(lMetricID));
            }
            else if(lCurrentNode.GetName().compare("rw_fmt") == 0)
            {
                lMiNode = lCurrentNode;
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