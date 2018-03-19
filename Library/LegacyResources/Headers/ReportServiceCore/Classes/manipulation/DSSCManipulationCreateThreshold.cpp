//
//  DSSCManipulationCreateThreshold.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 3/19/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#include "DSSCManipulationCreateThreshold.h"

#include "DSSCManipulationSetSelectionFromControl.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif

DSSCManipulationCreateThreshold::DSSCManipulationCreateThreshold(DSSThreshold* ipThreshold, DSSRWTemplateNode* ipTemplateNode, DSSTemplateMetric* ipTemplateMetric)
{
    mpThreshold = ipThreshold;
    mpTemplateNode = ipTemplateNode;
    mpTemplateMetric = ipTemplateMetric;
    
    mType = ManipulationCreateThreshold;
}

DSSCManipulationCreateThreshold::DSSCManipulationCreateThreshold(std::string iName, std::string iReplaceText, int iSemantics, DSSCExpression* ipExpression, std::vector<PropertyInfo*> iProperties, DSSRWTemplateNode* ipTemplateNode, DSSTemplateMetric* ipTemplateMetric, bool iIsEnabled, EnumDSSThresholdScope iThresholdScope)
{
    DSSObjectContext* lObjectContext = ipTemplateNode->getObjectContext();
    
    mpThreshold = new DSSThreshold(lObjectContext);
    mpThreshold->setName(iName);
    mpThreshold->setReplaceText(iReplaceText);
    mpThreshold->setSemantics(iSemantics);
    mpThreshold->setExpression(ipExpression);
    mpThreshold->setScope(iThresholdScope);
    mpThreshold->setIsEnabled(iIsEnabled);
    mpTemplateNode = ipTemplateNode;
    mpTemplateMetric = ipTemplateMetric;
    
    mProperties = iProperties;
    
    mType = ManipulationCreateThreshold;
}

DSSCManipulationCreateThreshold::DSSCManipulationCreateThreshold(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationCreateThreshold;
    
    if(ipAO->IsDeltaXML() == false)
        return;

    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
    std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
    hParseAOXML(lXMLString, ipContext);
#endif
}

DSSCManipulationCreateThreshold::~DSSCManipulationCreateThreshold()
{
    
}

int DSSCManipulationCreateThreshold::Execute()
{
    if (!mpTemplateNode)
        return S_FALSE;
    
    if (!mpTemplateMetric)
        return S_FALSE;
    
    DSSObjectContext* lObjectContext = mpTemplateNode->getObjectContext();
    if (!lObjectContext)
        return S_FALSE;
    
    DSSThresholds* lpThresholds = mpTemplateMetric->getThresholds();
    if(!lpThresholds)
    {
        lpThresholds = new DSSThresholds(lObjectContext);
        mpTemplateMetric->setThreholds(lpThresholds);
    }

    lpThresholds->Add(mpThreshold);
    mThresholdIndex = lpThresholds->Count();
    
    if(!mpThreshold->getFormat())
    {
        DSSFormat* lpFormat = new DSSFormat(1, &mProperties, mpTemplateNode->getObjectContext());
        lpFormat->setIndex(1);
        mpThreshold->setFormatRefId(lObjectContext->InsertFormat(lpFormat));
    }
    
    mIsEnabled = lpThresholds->getEnabled();
    
    mNodesModelChanged.push_back(mpTemplateNode);
    mNodeMapServer[mpTemplateNode->getKey()] = mpTemplateNode;
    
    std::vector<std::string> lNodes = mpTemplateNode->GetServerNodeKeys();
    int lSize = lNodes.size();
    for (int i = 0; i < lSize; i++)
    {
        std::string lKey = lNodes[i];
        if (mNodeMapServer.find(lKey) != mNodeMapServer.end())
            continue;
        
        DSSRWNode* lNode = mpTemplateNode->getObjectContext()->getNode(lKey);
        
        mNodesModelChanged.push_back(lNode);
        mNodeMapServer[lKey] = lNode;
    }
    
    GenerateAODeltaXML();
    return S_OK;
}

#ifndef __ANDROID__
void DSSCManipulationCreateThreshold::GenerateAODeltaXML()
{
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("19", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    
    int lTreeType = static_cast<int>(mpTemplateNode->getTreeType());
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    
    SimpleXMLNode lReportXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation"));
    
    SimpleXMLNode lReportManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation_method"));
    SimpleXMLNode lReportManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("58", true));
    lReportManipulationMethodXML.AddChild(lReportManipulationMethodXMLContent);
    
    char lReportUnitID[33];
    DSSBaseElementProxy::ConvertGUIDToString(mpTemplateMetric->getID(), lReportUnitID);
    SimpleXMLNode lReportUnitIDXML(SimpleXMLBuilder::CreateNodePtr("unit_id"));
    SimpleXMLNode lReportUnitIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lReportUnitID, true));
    lReportUnitIDXML.AddChild(lReportUnitIDXMLContent);
    
    SimpleXMLNode lReportUnitTypeXML(SimpleXMLBuilder::CreateNodePtr("unit_type"));
    SimpleXMLNode lReportUnitTypeXMLContent(SimpleXMLBuilder::CreateNodePtr("4", true));
    lReportUnitTypeXML.AddChild(lReportUnitTypeXMLContent);
    
    SimpleXMLNode lReportThresholdXML(SimpleXMLBuilder::CreateNodePtr("threshold"));
    lReportThresholdXML.AddProperty("ix", DSSExpNode::IntToString(mThresholdIndex).c_str());
    lReportThresholdXML.AddProperty("enabled", mIsEnabled ? "1" : "0");
    
    SimpleXMLNode lReportPropertiesXML(SimpleXMLBuilder::CreateNodePtr("threshold_properties"));
    
    
    SimpleXMLNode lReportPropertiesNameXML(SimpleXMLBuilder::CreateNodePtr("name"));
    SimpleXMLNode lReportPropertiesNameXMLContent(SimpleXMLBuilder::CreateNodePtr(mpThreshold->getName().c_str(), true));
    lReportPropertiesNameXML.AddChild(lReportPropertiesNameXMLContent);
    lReportPropertiesXML.AddChild(lReportPropertiesNameXML);
    
    if(mpThreshold->getScope() != DssThresholdScopeReserved)
    {
        int lThresholdScope = static_cast<int>(mpThreshold->getScope());
        SimpleXMLNode lReportPropertiesScopeXML(SimpleXMLBuilder::CreateNodePtr("scope"));
        SimpleXMLNode lReportPropertiesScopeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lThresholdScope).c_str(), true));
        lReportPropertiesScopeXML.AddChild(lReportPropertiesScopeXMLContent);
        lReportPropertiesXML.AddChild(lReportPropertiesScopeXML);
    }
    
    if(mpThreshold->getReplaceText().size())
    {
        SimpleXMLNode lReportPropertiesReplaceTextXML(SimpleXMLBuilder::CreateNodePtr("replace_text"));
        SimpleXMLNode lReportPropertiesReplaceTextXMLContent(SimpleXMLBuilder::CreateNodePtr(mpThreshold->getReplaceText().c_str(), true));
        lReportPropertiesReplaceTextXML.AddChild(lReportPropertiesReplaceTextXMLContent);
        lReportPropertiesXML.AddChild(lReportPropertiesReplaceTextXML);
    }
    
    if(mpThreshold->getSemantics() > DssBaseFormReserved)
    {
        SimpleXMLNode lReportPropertiesSemanticsXML(SimpleXMLBuilder::CreateNodePtr("text_semantics"));
        SimpleXMLNode lReportPropertiesSemanticsXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(mpThreshold->getSemantics()).c_str(), true));
        lReportPropertiesSemanticsXML.AddChild(lReportPropertiesSemanticsXMLContent);
        lReportPropertiesXML.AddChild(lReportPropertiesSemanticsXML);
    }
    
    lReportThresholdXML.AddChild(lReportPropertiesXML);
    

    SimpleXMLNode lReportThresholdFormatXML(SimpleXMLBuilder::CreateNodePtr("grid_format"));
    DSSFormat* lpFormat = new DSSFormat(1, &mProperties, mpTemplateNode->getObjectContext());
    xmlNodePtr lpFormatNode = static_cast<xmlNodePtr> (lpFormat->GenerateXMLNode());
    SimpleXMLNode lFormatNode(lpFormatNode);

    // grid format, not rwfmt
    for(SimpleXMLNode lPrsNode = lFormatNode.GetFirstChild(); lPrsNode.IsValid(); lPrsNode = lPrsNode.GetNextSibling())
        lReportThresholdFormatXML.AddChild(lPrsNode);
    lReportThresholdXML.AddChild(lReportThresholdFormatXML);
    
    if(mpThreshold->getExpression())
        mpThreshold->getExpression()->getExpressionXML(lReportThresholdXML.GetNode());
    
    lReportXML.AddChild(lReportManipulationMethodXML);
    lReportXML.AddChild(lReportUnitIDXML);
    lReportXML.AddChild(lReportUnitTypeXML);
    lReportXML.AddChild(lReportThresholdXML);
    
    lXML.AddChild(lManipulationMethodXML);
    lXML.AddChild(lTreeTypeXML);
    lXML.AddChild(lNodeKeyXML);
    lXML.AddChild(lReportXML);
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    
    mpActionObject = new DSSAODeltaXML(ManipulationCreateThreshold);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationCreateThreshold::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser(iXMLString);
    if(!lParser.IsValid())
        return;
    SimpleXMLNode lCurrentNode(lParser.GetRoot());
    if(!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
        return;
    
    SimpleXMLNode lFormatNode;
    SimpleXMLNode lExpressionNode;
    std::string lStrContent;
    for(lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if(lCurrentNode.IsElementNode())
        {
            if(lCurrentNode.GetName().compare("rw_node_key") == 0)
            {
                lCurrentNode.GetContent(lStrContent);
                mpTemplateNode = static_cast<DSSRWTemplateNode*>(ipContext->getNode(lStrContent));
            }
            else if(lCurrentNode.GetName().compare("report_manipulation") == 0)
            {
                for(SimpleXMLNode lCurrentReportNode = lCurrentNode.GetFirstChild(); lCurrentReportNode.IsValid(); lCurrentReportNode = lCurrentReportNode.GetNextSibling())
                {
                    if(lCurrentReportNode.IsElementNode())
                    {
                        if(lCurrentReportNode.GetName().compare("unit_id") == 0)
                        {
                            lCurrentReportNode.GetContent(lStrContent);
                            GUID lTemplateMetricGUID;
                            DSSBaseElementProxy::ConvertFromStringToGUID(lStrContent, lTemplateMetricGUID);
                            DSSTemplate* lpTemplate = mpTemplateNode->getTemplate();
                            DSSTemplateUnit* lUnit = lpTemplate->getTemplateUnitFromAxisIndex(DssAxisRows, lTemplateMetricGUID, false);
                            if (lUnit)
                                mpTemplateMetric = static_cast<DSSTemplateMetric*>(lUnit);
                            else
                                mpTemplateMetric = static_cast<DSSTemplateMetric*>(lpTemplate->getTemplateUnitFromAxisIndex(DssAxisColumns, lTemplateMetricGUID, false));
                        }
                        else if(lCurrentReportNode.GetName().compare("threshold") == 0)
                        {
                            lCurrentReportNode.GetProperty("enabled", lStrContent);
                            mIsEnabled = (lStrContent.compare("1") == 0 ? true : false);
                            mpThreshold->setIsEnabled(mIsEnabled);
                            for(SimpleXMLNode lCurrentThresholdNode = lCurrentReportNode.GetFirstChild(); lCurrentThresholdNode.IsValid(); lCurrentThresholdNode = lCurrentThresholdNode.GetNextSibling())
                            {
                                if(lCurrentThresholdNode.GetName().compare("threshold_properties") == 0)
                                {
                                    for(SimpleXMLNode lCurrentPropertiesNode = lCurrentThresholdNode.GetFirstChild(); lCurrentPropertiesNode.IsValid(); lCurrentPropertiesNode = lCurrentPropertiesNode.GetNextSibling())
                                    {
                                        if(lCurrentPropertiesNode.GetName().compare("name") == 0)
                                        {
                                            lCurrentPropertiesNode.GetContent(lStrContent);
                                            mpThreshold->setName(lStrContent);
                                        }
                                        else if(lCurrentPropertiesNode.GetName().compare("scope") == 0)
                                        {
                                            lCurrentPropertiesNode.GetContent(lStrContent);
                                            mpThreshold->setScope(static_cast<EnumDSSThresholdScope>(DSSExpNode::StringToInt(lStrContent)));
                                        }
                                        else if(lCurrentPropertiesNode.GetName().compare("replace_text") == 0)
                                        {
                                            lCurrentPropertiesNode.GetContent(lStrContent);
                                            mpThreshold->setReplaceText(lStrContent);
                                        }
                                        else if(lCurrentPropertiesNode.GetName().compare("text_semantics") == 0)
                                        {
                                            lCurrentPropertiesNode.GetContent(lStrContent);
                                            mpThreshold->setSemantics(DSSExpNode::StringToInt(lStrContent));
                                        }
                                    }
                                }
                                else if(lCurrentThresholdNode.GetName().compare("grid_format") == 0)
                                {
                                    lFormatNode = lCurrentThresholdNode;
                                }
                                else if(lCurrentThresholdNode.GetName().compare("mi") == 0)
                                {
                                    lExpressionNode = lCurrentThresholdNode;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    // format
    if (!lFormatNode.IsValid())
		return;
	
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

    // expression
    if (!lExpressionNode.IsValid())
		return;
	
	// Parse the mi xml to get root node of the expression
    DSSCExpression* lpExpression = new DSSCExpression(ipContext);
	SimpleXMLNode lRootNode = NULL;
	for (lCurrentNode = lExpressionNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
	{
		if (lCurrentNode.IsElementNode() && lCurrentNode.GetName().compare("exp") == 0)
		{
			// Found the exp node
			for (lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
			{
				if (lCurrentNode.IsElementNode() && lCurrentNode.GetName().compare("nd") == 0)
				{
					// Found the root node
					lRootNode = lCurrentNode;
					break;
				}
			}
			break;
		}
	}
	
	if (!lRootNode.IsValid())
		return;

	DSSExpNode* lpExpNode = DSSCManipulationSetSelectionFromControl::hBuildExpNode(lpExpression, lRootNode, NULL, ipContext);
	lpExpression->setRoot(lpExpNode);
	lpExpression->setExpresssionType(lpExpNode->getExpressionType());
}
#endif

