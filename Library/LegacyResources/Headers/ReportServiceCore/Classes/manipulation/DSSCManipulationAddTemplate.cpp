//
//  DSSCManipulationAddTemplate.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 1/12/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "DSSCManipulationAddTemplate.h"
#include "DSSExpNode.h"
#include "DSSBaseElementProxy.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxm.h"
#else
    #include "DSSSimpleXML.h"
#endif
#include "DSSAODeltaXML.h"
#include "GUID.h"


DSSCManipulationAddTemplate::DSSCManipulationAddTemplate(DSSRWSectionNode* ipParentNode, EnumDSSDisplayMode iDisplayMode, GUID iDataSourceID): DSSCManipulationAddRWNode(DssRWNodeTemplate, ipParentNode)
{
    // construct from data source
    mpTemplateNode = static_cast<DSSRWTemplateNode*>(mpNode);
    mDisplayMode = iDisplayMode;
    mpGraphProperties = NULL;
	mpTemplate = NULL;
    if(!MBase::IsGUIDNULL(mDataSourceID))
        mDataSourceID = iDataSourceID;
    // else
        // mDataSourceID = mpTemplateNode->getObjectContext()->getDefaultDataSetID();
    
    mType = ManipulationAddTemplate;
}

DSSCManipulationAddTemplate::DSSCManipulationAddTemplate(DSSRWSectionNode* ipParentNode, EnumDSSDisplayMode iDisplayMode, DSSGraphProperties* ipGraphProperties, EnumGraphMajorType iMajorType): DSSCManipulationAddRWNode(DssRWNodeTemplate, ipParentNode)
{
    // construct from graph properties
    mpTemplateNode = static_cast<DSSRWTemplateNode*>(mpNode);
    mDisplayMode = iDisplayMode;
    mpGraphProperties = ipGraphProperties;
	mpTemplate = NULL;
    mGraphMajorType = iMajorType;
    mpGraphProperties->setGraphType(mGraphMajorType);
    mDataSourceID = mpTemplateNode->getObjectContext()->getDefaultDataSetID();
    
    mType = ManipulationAddTemplate;
}

DSSCManipulationAddTemplate::DSSCManipulationAddTemplate(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationAddTemplate;
    
    if (!ipAO->IsDeltaXML())
        return;
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();
    hParseAOXML(lXMLString, ipContext);
#endif
}

DSSCManipulationAddTemplate::~DSSCManipulationAddTemplate()
{
    
}

DSSRWNode* DSSCManipulationAddTemplate::getTemplateNode()
{
    return mpNode;
}

void DSSCManipulationAddTemplate::AddTemplate()
{    
    DSSCManipulationAddRWNode::AddRWNode();
    mpTemplateNode->setDataSourceID(mDataSourceID);
    DSSObjectContext* lpObjectContext = mpTemplateNode->getObjectContext();
    
    if(!mpTemplate)
    {
        GUID lTemplateID = GenerateGUID();
        mpTemplate = new DSSTemplate(lTemplateID, DssTypeTemplate, std::string(""), lpObjectContext, GUID_NULL);
        lpObjectContext->InsertObject(lTemplateID, mpTemplate);
        mpTemplateNode->setTemplateID(lTemplateID);
    }
    
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
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
    //push the key of metric selector controlling the template node to node server map
    AddMetricConditionSelectorsToMap(mpTemplateNode);
}

int DSSCManipulationAddTemplate::Execute()
{
    AddTemplate();
    
    mpTemplateNode->setDisplayMode(mDisplayMode);
    mpTemplateNode->setGraphProperties(mpGraphProperties);

    std::vector<PropertyInfo*>::iterator lIt = mProperties.begin();
	for(; lIt!=mProperties.end(); lIt++)
	{
		int lFmtInx = (*lIt)->mFormatIndex;
        std::string lPrptySetName = (*lIt)->mPropertySetName;
        std::string lPrptyName = (*lIt)->mPropertyName;
        CComVariant& lPrptyValue = (*lIt)->mPropertyValue;
        
        DSSFormat* lpFormat = mpNode->getFormat(static_cast<EnumDSSRWNodeFormatType>(lFmtInx));
        
		bool extendedProperty = false;
		
		if (lPrptySetName.compare("FormattingPosition")==0) 
		{
			if (lPrptyName.compare("Top")==0) 
			{
                mpNode->setTop(lPrptyValue.dblVal);
				extendedProperty = true;
			}
			else if (lPrptyName.compare("Left")==0)
			{
                mpNode->setLeft(lPrptyValue.dblVal);
				extendedProperty = true;
			}
		}
		else if (lPrptySetName.compare("FormattingSize")==0)
		{
			if (lPrptyName.compare("Width")==0)
			{
				mpNode->setWidth(lPrptyValue.dblVal);
                extendedProperty = true;
			}
			else if (lPrptyName.compare("Height")==0)
			{
                mpNode->setHeight(lPrptyValue.dblVal);
				extendedProperty = true;
			}
		}
		else if (lPrptySetName.compare("FormattingAppearance")==0)
		{
			if (lPrptyName.compare("Name")==0)
			{
                mpNode->setName(lPrptyValue.ToString());	
                extendedProperty = true;
			}
			else if (lPrptyName.compare("TooltipText")==0)
			{
                mpNode->setToolTipText(lPrptyValue.ToString());
				extendedProperty = true;
			}
		}
		else if (lPrptySetName.compare("FormattingOrder")==0)
		{
			if (lPrptyName.compare("ZIndex")==0)
			{
                mpNode->setZIndex(lPrptyValue.iVal);
				extendedProperty = true;
			}
		}
        else if (lPrptySetName.compare("FormattingView")==0)
        {
            if (lPrptyName.compare("Title")==0)
            {
                mpNode->setTitle(lPrptyValue.ToString());
                extendedProperty = true;
            }
        }
        
		if (!extendedProperty)
            lpFormat->setPropertyValue(lPrptySetName, lPrptyName, lPrptyValue);
    }
    GenerateAODeltaXML();
    return S_OK;
}

GUID DSSCManipulationAddTemplate::GenerateGUID()
{
    return DSSObjectContext::createGUID();
}

#ifndef __ANDROID__
void DSSCManipulationAddTemplate::GenerateAODeltaXML()
{
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    
    // add the node
    {
        SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
        
        SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
        SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("4", true));
        lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
        
        int lTreeType = static_cast<int>(mpTemplateNode->getTreeType());
        SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
        lTreeTypeXML.AddChild(lTreeTypeXMLContent);
        
        SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
        lNodeKeyXML.AddChild(lNodeKeyXMLContent);
        
        SimpleXMLNode lNodeKeyParentXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key_parent"));
        SimpleXMLNode lNodeKeyParentXMLContent(SimpleXMLBuilder::CreateNodePtr(mpParentNode->getKey().c_str(), true));
        lNodeKeyParentXML.AddChild(lNodeKeyParentXMLContent);
        
        int lChildIndex = mpTemplateNode->getIndex();
        SimpleXMLNode lChildIndexXML(SimpleXMLBuilder::CreateNodePtr("index"));
        SimpleXMLNode lChildIndexXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lChildIndex).c_str(), true));
        lChildIndexXML.AddChild(lChildIndexXMLContent);
        
        lXML.AddChild(lManipulationMethodXML);
        lXML.AddChild(lTreeTypeXML);
        lXML.AddChild(lNodeKeyXML);
        lXML.AddChild(lNodeKeyParentXML);
        lXML.AddChild(lChildIndexXML);
        lRoot.AddChild(lXML);
    }
    
    // set dataset id
    {
        SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
        
        SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
        SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("8", true));
        lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
        
        int lTreeType = static_cast<int>(mpTemplateNode->getTreeType());
        SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
        lTreeTypeXML.AddChild(lTreeTypeXMLContent);
        
        SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
        lNodeKeyXML.AddChild(lNodeKeyXMLContent);
        
        char lDataSetID[33];
        DSSBaseElementProxy::ConvertGUIDToString(mDataSourceID, lDataSetID);
        SimpleXMLNode lDataSetIDXML(SimpleXMLBuilder::CreateNodePtr("rw_data_set_id"));
        SimpleXMLNode lDataSetIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lDataSetID, true));
        lDataSetIDXML.AddChild(lDataSetIDXMLContent);
        
        int lObjectType = static_cast<EnumDSSObjectType>(mpTemplateNode->getObjectContext()->FindObjectH(mDataSourceID)->getType());
        SimpleXMLNode lDataSetTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_data_set_type"));
        SimpleXMLNode lDataSetTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lObjectType).c_str(), true));
        lDataSetTypeXML.AddChild(lDataSetTypeXMLContent);
        
        lXML.AddChild(lManipulationMethodXML);
        lXML.AddChild(lTreeTypeXML);
        lXML.AddChild(lNodeKeyXML);
        lXML.AddChild(lDataSetIDXML);
        lXML.AddChild(lDataSetTypeXML);
        // lManipulationsXML.AddChild(lXML);
        lRoot.AddChild(lXML);
    }
    
    // set grid display mode
    if(mDisplayMode >= 1)
    {
        SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
        
        SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
        SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("32", true));
        lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
        
        int lTreeType = static_cast<int>(mpTemplateNode->getTreeType());
        SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
        lTreeTypeXML.AddChild(lTreeTypeXMLContent);
        
        SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
        lNodeKeyXML.AddChild(lNodeKeyXMLContent);
        
        int lDisplayMode = static_cast<int>(mDisplayMode);
        SimpleXMLNode lDisplayModeXML(SimpleXMLBuilder::CreateNodePtr("rw_grid_display_mode"));
        SimpleXMLNode lDisplayModeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lDisplayMode).c_str(), true));
        lDisplayModeXML.AddChild(lDisplayModeXMLContent);
        
        lXML.AddChild(lManipulationMethodXML);
        lXML.AddChild(lTreeTypeXML);
        lXML.AddChild(lNodeKeyXML);
        lXML.AddChild(lDisplayModeXML);
        // lManipulationsXML.AddChild(lXML);
        lRoot.AddChild(lXML);
    }
    
    // set properties
    if(mProperties.size())
    {
        std::set<int> lIndexes;
        std::vector<PropertyInfo*>::iterator lIt = mProperties.begin();
        for(; lIt!=mProperties.end(); lIt++)
        {
            int lFmtInx = (*lIt)->mFormatIndex;
            
            if(lIndexes.find(lFmtInx) == lIndexes.end())
                lIndexes.insert(lFmtInx);
        }
        
        for(std::set<int>::iterator lIter = lIndexes.begin(); lIter != lIndexes.end(); lIter++)
        {
            SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
            
            SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
            SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("7", true));
            lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
            
            int lTreeType = static_cast<int>(mpNode->getTreeType());
            SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
            SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
            lTreeTypeXML.AddChild(lTreeTypeXMLContent);
            
            SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
            SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpNode->getKey().c_str(), true));
            lNodeKeyXML.AddChild(lNodeKeyXMLContent);
            
            SimpleXMLNode lFormatTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_format_type"));
            SimpleXMLNode lFormatTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(*lIter).c_str(), true));
            lFormatTypeXML.AddChild(lFormatTypeXMLContent);
            
            DSSFormat* lpFormat = new DSSFormat(*lIter, &mProperties, mpTemplateNode->getObjectContext());
            
            xmlNodePtr lpFormatNode = static_cast<xmlNodePtr> (lpFormat->GenerateXMLNode());
            SimpleXMLNode formatNode(lpFormatNode);
            
            lXML.AddChild(lManipulationMethodXML);
            lXML.AddChild(lTreeTypeXML);
            lXML.AddChild(lNodeKeyXML);
            lXML.AddChild(lFormatTypeXML);
            lXML.AddChild(formatNode);
            
            lRoot.AddChild(lXML);
            delete lpFormat;
        }
    }
    
    // set graph binary
    if(mpGraphProperties)
    {
        SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
        
        SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
        SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("37", true));
        lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
        
        int lTreeType = static_cast<int>(mpTemplateNode->getTreeType());
        SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
        lTreeTypeXML.AddChild(lTreeTypeXMLContent);
        
        SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
        lNodeKeyXML.AddChild(lNodeKeyXMLContent);
        
        int lgat = static_cast<int>(mpGraphProperties->getGraphType());
        SimpleXMLNode lGraphTypeXML(SimpleXMLBuilder::CreateNodePtr("gpp"));
        lGraphTypeXML.AddProperty("gat", DSSExpNode::IntToString(lgat).c_str());
        
        lXML.AddChild(lManipulationMethodXML);
        lXML.AddChild(lTreeTypeXML);
        lXML.AddChild(lNodeKeyXML);
        lXML.AddChild(lGraphTypeXML);
        lRoot.AddChild(lXML);
    }
        
    mpActionObject = new DSSAODeltaXML(ManipulationAddTemplate);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationAddTemplate::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
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
            for(SimpleXMLNode lCurrentSubNode = lCurrentNode.GetFirstChild(); lCurrentSubNode.IsValid(); lCurrentSubNode = lCurrentSubNode.GetNextSibling())
            {
                if(lCurrentSubNode.GetName().compare("rw_node_key") == 0)
                {
                    lCurrentSubNode.GetContent(lStrContent);
                    mpNode = ipContext->getNode(lStrContent);
                    mpTemplateNode = static_cast<DSSRWTemplateNode*>(mpNode);
                }
                else if(lCurrentSubNode.GetName().compare("rw_data_set_id") == 0)
                {
                    lCurrentSubNode.GetContent(lStrContent);
                    DSSBaseElementProxy::ConvertFromStringToGUID(lStrContent, mDataSourceID);
                }
                else if(lCurrentSubNode.GetName().compare("rw_grid_display_mode") == 0)
                {
                    lCurrentSubNode.GetContent(lStrContent);
                    mDisplayMode = static_cast<EnumDSSDisplayMode>(DSSExpNode::StringToInt(lStrContent));
                }
                else if(lCurrentSubNode.GetName().compare("rwfmt") == 0)
                {
                    lMiNode = lCurrentSubNode;
                }
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
#else
void DSSCManipulationAddTemplate::GenerateAODeltaXML()
{
    // TODO 
}
#endif