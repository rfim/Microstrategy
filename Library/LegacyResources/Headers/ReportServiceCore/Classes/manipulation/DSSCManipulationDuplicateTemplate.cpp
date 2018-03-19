//
//  DSSCManipulationDuplicateTemplate.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 2/16/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "DSSCManipulationDuplicateTemplate.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#include "DSSBaseElementProxy.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxml.h"
#else
#include "DSSSimpleXML.h"
#endif

DSSCManipulationDuplicateTemplate::DSSCManipulationDuplicateTemplate(DSSRWSectionNode* ipParentNode, DSSRWTemplateNode* ipSourceTemplateNode, std::string iName, std::string iTitle, EnumDSSDisplayMode iDisplayMode, DSSGraphProperties* ipGraphProperties, EnumGraphMajorType iGraphMajorType, int iGraphMinorType, GUID iDataSourceID): DSSCManipulationAddTemplate(ipParentNode, iDisplayMode, iDataSourceID), mpSourceTemplateNode(ipSourceTemplateNode), mName(iName), mTitle(iTitle)
{
    mpGraphProperties = ipGraphProperties;
    mGraphMajorType = iGraphMajorType;
    mGraphMinorType = iGraphMinorType;
    if(mpSourceTemplateNode && mpSourceTemplateNode->getTemplate())
        mpSourceTemplate = mpSourceTemplateNode->getTemplate();
    mUnderSamePanel = false;
    mNeedAddTarget = false;
}
DSSCManipulationDuplicateTemplate::DSSCManipulationDuplicateTemplate(DSSActionObject* ipAO, DSSObjectContext* ipContext): DSSCManipulationAddTemplate(NULL, NULL)
{
    mType = ManipulationDuplicateTemplate;
    mUnderSamePanel = false;
    mNeedAddTarget = false;
    
    if (!ipAO->IsDeltaXML())
        return;
    
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();
    
    // parse lXMLString
    hParseAOXML(lXMLString, ipContext);
#endif
}

DSSCManipulationDuplicateTemplate::~DSSCManipulationDuplicateTemplate()
{
    
}

int DSSCManipulationDuplicateTemplate::Execute()
{
    DSSObjectContext* lpObjectContext = mpTemplateNode->getObjectContext();
    
    AddTemplate();
    mpTemplateNode->Duplicate(mpSourceTemplateNode);
    mDataSourceID = mpTemplateNode->getDataSourceID();
    if(!MBase::IsGUIDNULL(mDataSourceID))
        mpTemplateNode->setDataSourceID(mDataSourceID);
    if(mDisplayMode != DssDisplayModeReserved)
        mpTemplateNode->setDisplayMode(mDisplayMode);
    if(mpGraphProperties)
    {
        mpTemplateNode->setGraphProperties(mpGraphProperties);
        mpGraphProperties->setGraphType(mGraphMajorType);
        mpGraphProperties->setGraphSubType(mGraphMinorType);
    }
    
    if(mName.size())
    {
        mpTemplateNode->setName(mName);
        PropertyInfo* lPropertyName = new PropertyInfo();
        lPropertyName->mFormatIndex = 1;
        lPropertyName->mPropertySetName = "FormattingAppearance";
        lPropertyName->mPropertyName = "Name";
        lPropertyName->mPropertyValue = mName.c_str();
        mProperties.push_back(lPropertyName);
    }
    if(mTitle.size())
    {
        mpTemplateNode->setTitle(mTitle);
        PropertyInfo* lPropertyTitle = new PropertyInfo();
        lPropertyTitle->mFormatIndex = 1;
        lPropertyTitle->mPropertySetName = "FormattingView";
        lPropertyTitle->mPropertyName = "Title";
        lPropertyTitle->mPropertyValue = mTitle.c_str();
        mProperties.push_back(lPropertyTitle);
    }
    
    if(mProperties.size())
    {
        DSSFormat* lpFormat = mpTemplateNode->getFormat(DssRWNodeFormatNode);
        if(!lpFormat)
        {
            DSSObjectContext* lpObjectContext = mpTemplateNode->getObjectContext();
            lpFormat = new DSSFormat(1, &mProperties, lpObjectContext);
            mpTemplateNode->setFormatRefID(DssRWNodeFormatNode, lpObjectContext->InsertFormat(lpFormat));
        }
        else
        {
            for(std::vector<PropertyInfo*>::iterator lIter = mProperties.begin(); lIter != mProperties.end(); lIter++)
            {
                std::string lPropertySetName = (*lIter)->mPropertySetName;
                std::string lPropertyName = (*lIter)->mPropertyName;
                CComVariant lPropertyValue = (*lIter)->mPropertyValue;
                lpFormat->setPropertyValue(lPropertySetName, lPropertyName, lPropertyValue);
            }
        }
    }
    
    std::vector<std::string> lvCGBorUCKeys;
    DSSRWNode* lpPanelNode = mpTemplateNode->GetCGBorUCNodesUnderPanel(lvCGBorUCKeys);
    if(lpPanelNode && mpTemplateNode->getParentNode() == mpSourceTemplateNode->getParentNode())
    {
        mUnderSamePanel = true;
        if(lpPanelNode != mpTemplateNode->getParentNode())
            mNeedAddTarget = true;
    }
    
    if(mUnderSamePanel)
    {
        if(mNeedAddTarget)
        {
            MapFromTargetToControl* lpMapFromTargetToControl = lpObjectContext->getMapFromTargetToControl();
            // for each target node, find the source control, and add the new node as its target
            for(std::vector<std::string>::iterator lTargetIter = lvCGBorUCKeys.begin(); lTargetIter != lvCGBorUCKeys.end(); lTargetIter++)
            {
                std::string lTargetKey = *lTargetIter;
                NodeAndControlsVector lNodeAndControlsVector = (*lpMapFromTargetToControl)[lTargetKey];
                for(NodeAndControlsVector::iterator lNodeAndControlsIter = lNodeAndControlsVector.begin(); lNodeAndControlsIter != lNodeAndControlsVector.end(); lNodeAndControlsIter++)
                {
                    DSSRWNode* lpControlNode = lpObjectContext->getNode((*lNodeAndControlsIter)->NodeKey);
                    DSSRWControls* lpControls;
                    if(lpControlNode->getType() == DssRWNodeTemplate)
                        lpControls = (static_cast<DSSRWTemplateNode*>(lpControlNode))->getControls();
                    else
                        lpControls = (static_cast<DSSRWControlNode*>(lpControlNode))->getControls();
                    for(int i = 0; i < lpControls->Count(); i++)
                    {
                        DSSRWControl* lpControl = lpControls->Item(i);
                        for(int j = 0; j < lpControl->getCountTargets(); j++)
                        {
                            if(lpControl->getTargetKey(j) == lTargetKey)
                            {
                                /*
                                 void AddControlTarget(DSSRWNode* ipNode, DSSRWControl* ipControl, std::string iTargetKey, bool ibisUC, std::vector<std::string>* ipGroupbyNodeKeys);
                                 */
                                bool lbIsUC = true;
                                if(static_cast<DSSRWSectionNode*>(lpObjectContext->getNode(lTargetKey))->getSectionType() == DssRWSectionControlGroupBySection)
                                    lbIsUC = false;
                                std::vector<std::string> lGroupByNodeKeys;
                                AddControlTarget(lpControlNode, lpControl, mpTemplateNode->getKey(), lbIsUC, &lGroupByNodeKeys);
                                // update the filter?
                                mNodesModelChanged.push_back(lpControlNode);
                            }
                        }
                    }
                }
            }
        }
        std::vector<DSSRWNode*> lpSourceNodes;
        searchAssociatedSourceNodes(mpSourceTemplateNode, lpPanelNode, &lpSourceNodes);
        for(std::vector<DSSRWNode*>::iterator lSourceNodeIter = lpSourceNodes.begin(); lSourceNodeIter != lpSourceNodes.end(); lSourceNodeIter++)
        {
            DSSRWTemplateNode* lpSourceNode = static_cast<DSSRWTemplateNode*>(*lSourceNodeIter);
            std::map<std::string, int> lSourceAssociateInfos = lpSourceNode->getAssociatedNodeInfos();
            lSourceAssociateInfos[mpTemplateNode->getKey()] = lSourceAssociateInfos[mpSourceTemplateNode->getKey()];
        }
    }
    else
    {
        // not under the same panel, clear the controls & associated infos
        // mpTemplateNode->clearControls(); // debug
         mpTemplateNode->clearAssociatedInfos();
    }
    
    GenerateAOXML();
    return S_OK;
}

#ifndef __ANDORID__
void DSSCManipulationDuplicateTemplate::GenerateAOXML()
{
    
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    
    // copy template
    {
        SimpleXMLNode lCopyTemplateXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
        
        SimpleXMLNode lCTManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
        SimpleXMLNode lCTManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("35", true));
        lCTManipulationMethodXML.AddChild(lCTManipulationMethodXMLContent);
        
        int lCTTreeType = static_cast<int>(mpSourceTemplateNode->getTreeType());
        SimpleXMLNode lCTTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lCTTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lCTTreeType).c_str(), true));
        lCTTreeTypeXML.AddChild(lCTTreeTypeXMLContent);
        
        SimpleXMLNode lCTNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lCTNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpSourceTemplateNode->getKey().c_str(), true));
        lCTNodeKeyXML.AddChild(lCTNodeKeyXMLContent);
        
        int lCTDestTreeType = static_cast<int>(mpParentNode->getTreeType());
        SimpleXMLNode lCTDestTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type_dest"));
        SimpleXMLNode lCTDestTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lCTDestTreeType).c_str(), true));
        lCTDestTreeTypeXML.AddChild(lCTDestTreeTypeXMLContent);
        
        SimpleXMLNode lCTNodeKeyParentXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key_parent"));
        SimpleXMLNode lCTNodeKeyParentXMLContent(SimpleXMLBuilder::CreateNodePtr(mpParentNode->getKey().c_str(), true));
        lCTNodeKeyParentXML.AddChild(lCTNodeKeyParentXMLContent);
        
        SimpleXMLNode lCTDestNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key_dest"));
        SimpleXMLNode lCTDestNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
        lCTDestNodeKeyXML.AddChild(lCTDestNodeKeyXMLContent);
        
        lCopyTemplateXML.AddChild(lCTManipulationMethodXML);
        lCopyTemplateXML.AddChild(lCTTreeTypeXML);
        lCopyTemplateXML.AddChild(lCTNodeKeyXML);
        lCopyTemplateXML.AddChild(lCTDestTreeTypeXML);
        lCopyTemplateXML.AddChild(lCTNodeKeyParentXML);
        lCopyTemplateXML.AddChild(lCTDestNodeKeyXML);
        lRoot.AddChild(lCopyTemplateXML);
    }
    
    // set datasource id
    if(!MBase::IsGUIDNULL(mDataSourceID) && !MBase::IsEqualGUID(mDataSourceID, mpSourceTemplateNode->getDataSourceID()))
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
    
    // set display mode
    int lDisplayMode = static_cast<int>(mDisplayMode);
    if(lDisplayMode >= 1)
    {
        SimpleXMLNode lSetDisplayModeXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
        
        SimpleXMLNode lSDMManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
        SimpleXMLNode lSDMManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("32", true));
        lSDMManipulationMethodXML.AddChild(lSDMManipulationMethodXMLContent);
        
        int lSDMTreeType = static_cast<int>(mpParentNode->getTreeType());
        SimpleXMLNode lSDMParentTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lSDMParentTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lSDMTreeType).c_str(), true));
        lSDMParentTreeTypeXML.AddChild(lSDMParentTreeTypeXMLContent);
        
        SimpleXMLNode lSDMNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lSDMNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
        lSDMNodeKeyXML.AddChild(lSDMNodeKeyXMLContent);
        
        SimpleXMLNode lSDMGridDisplayModeXML(SimpleXMLBuilder::CreateNodePtr("rw_grid_display_mode"));
        SimpleXMLNode lSDMGridDisplayModeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lDisplayMode).c_str(), true));
        //lSDMManipulationMethodXML.AddChild(lSDMManipulationMethodXMLContent);
        lSDMGridDisplayModeXML.AddChild(lSDMGridDisplayModeXMLContent);
        
        lSetDisplayModeXML.AddChild(lSDMManipulationMethodXML);
        lSetDisplayModeXML.AddChild(lSDMParentTreeTypeXML);
        lSetDisplayModeXML.AddChild(lSDMNodeKeyXML);
        lSetDisplayModeXML.AddChild(lSDMGridDisplayModeXML);
        lRoot.AddChild(lSetDisplayModeXML);
    }
    
    // set the format
    if(mProperties.size() > 0)
    {
        DSSObjectContext* lpContext = mpNode->getObjectContext();
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
            
            DSSFormat* lpFormat = new DSSFormat(*lIter, &mProperties, lpContext);
            
            xmlNodePtr lpFormatNode = static_cast<xmlNodePtr> (lpFormat->GenerateXMLNode());
            SimpleXMLNode formatNode(lpFormatNode);
            
            lXML.AddChild(lManipulationMethodXML);
            lXML.AddChild(lTreeTypeXML);
            lXML.AddChild(lNodeKeyXML);
            lXML.AddChild(lFormatTypeXML);
            lXML.AddChild(formatNode);
            
            // SimpleXMLNode lRoot(lBuilder.GetRoot());
            lRoot.AddChild(lXML);
            delete lpFormat;
        }
        /*
        SimpleXMLNode lSetPropertiesXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
        
        SimpleXMLNode lSPManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
        SimpleXMLNode lSPManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("7", true));
        lSPManipulationMethodXML.AddChild(lSPManipulationMethodXMLContent);
        
        int lTreeType = static_cast<int>(mpTemplateNode->getTreeType());
        SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
        lTreeTypeXML.AddChild(lTreeTypeXMLContent);
        
        SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
        lNodeKeyXML.AddChild(lNodeKeyXMLContent);
        
        SimpleXMLNode lFormatTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_format_type"));
        SimpleXMLNode lFormatTypeXMLContent(SimpleXMLBuilder::CreateNodePtr("1", true));
        lFormatTypeXML.AddChild(lFormatTypeXMLContent);
        
        DSSObjectContext* lpContext = mpParentNode->getObjectContext();
        
        // DSSFormat* lpFormat = new DSSFormat(1, mpProperties, mNumProperties, lpContext);
         // need to parse
        // SimpleXMLNode lSPPropertiesXML(SimpleXMLBuilder::CreateNodePtr(lpFormat->GenerateXML().c_str()));
        
        DSSFormat* lpFormat = new DSSFormat(1, &mProperties, lpContext);
        
        xmlNodePtr lpFormatNode = static_cast<xmlNodePtr> (lpFormat->GenerateXMLNode());
        SimpleXMLNode lSPPropertiesXML(lpFormatNode);
        
        
        lSetPropertiesXML.AddChild(lSPManipulationMethodXML);
        lSetPropertiesXML.AddChild(lTreeTypeXML);
        lSetPropertiesXML.AddChild(lNodeKeyXML);
        lSetPropertiesXML.AddChild(lSPPropertiesXML);
        lRoot.AddChild(lSetPropertiesXML);*/
    }
    
    // add from graph
    if(mpGraphProperties)
    {
        SimpleXMLNode lSetGraphTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
        
        SimpleXMLNode lSGTManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
        SimpleXMLNode lSGTManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("37", true));
        lSGTManipulationMethodXML.AddChild(lSGTManipulationMethodXMLContent);
        
        int lSGTTreeType = static_cast<int>(mpParentNode->getTreeType());
        SimpleXMLNode lSGTTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lSGTTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lSGTTreeType).c_str(), true));
        lSGTTreeTypeXML.AddChild(lSGTTreeTypeXMLContent);
        
        SimpleXMLNode lSGTNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lSGTNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
        lSGTNodeKeyXML.AddChild(lSGTNodeKeyXMLContent);
        
        int lGraphMajorType = static_cast<int>(mpGraphProperties->getGraphType());
        int lGraphMinorType = static_cast<int>(mpGraphProperties->getGraphSubType());
        SimpleXMLNode lSGTGraphTypeXML(SimpleXMLBuilder::CreateNodePtr("gpp"));
        lSGTGraphTypeXML.AddProperty("gat", DSSExpNode::IntToString(lGraphMajorType).c_str());
        lSGTGraphTypeXML.AddProperty("git", DSSExpNode::IntToString(lGraphMinorType).c_str());
        
        lSetGraphTypeXML.AddChild(lSGTManipulationMethodXML);
        lSetGraphTypeXML.AddChild(lSGTTreeTypeXML);
        lSetGraphTypeXML.AddChild(lSGTNodeKeyXML);
        lSetGraphTypeXML.AddChild(lSGTGraphTypeXML);
        lRoot.AddChild(lSetGraphTypeXML);
    }
    
    mpActionObject = new DSSAODeltaXML(ManipulationDuplicateTemplate);
    // debug
    std::string lXMLString = lBuilder.ToString(false);
    // enddebug
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationDuplicateTemplate::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    // not implemented
}
#else
void DSSCManipulationDuplicateTemplate::GenerateAOXML()
{
    // not updated
    
    TiXmlDocument lDoc;
    TiXmlElement* lpRootElement = new TiXmlElement("rw_manipulations");
    
    TiXmlElement* lpCopyTemplateElement = new TiXmlElement("rw_manipulation");
    
    TiXmlElement* lpCTManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
    TiXmlText* lpCTManipulationMethodText = new TiXmlText("35");
    lpCTManipulationMethodElement->LinkEndChild(lpCTManipulationMethodText);
    
    int lCTTreeType = static_cast<int>(mpSourceTemplateNode->getTreeType());
    TiXmlElement* lpCTTreeTypeElement = new TiXmlElement("rw_tree_type");
    TiXmlText* lpCTTreeTypeText = new TiXmlText(DSSExpNode::IntToString(lCTTreeType).c_str());
    lpCTTreeTypeElement->LinkEndChild(lpCTTreeTypeText);
    
    TiXmlElement* lpCTNodeKeyElement = new TiXmlElement("rw_node_key");
    TiXmlText* lpCTNodeKeyText = new TiXmlText(mpSourceTemplateNode->getKey().c_str());
    lpCTNodeKeyElement->LinkEndChild(lpCTNodeKeyText);
    
    int lCTDestTreeType = static_cast<int>(mpParentNode->getTreeType());
    TiXmlElement* lpCTDestTreeTypeElement = new TiXmlElement("rw_tree_type_dest");
    TiXmlText* lpCTDestTreeTypeText = new TiXmlText(DSSExpNode::IntToString(lCTDestTreeType).c_str());
    lpCTDestTreeTypeElement->LinkEndChild(lpCTDestTreeTypeText);
    
    TiXmlElement* lpCTParentNodeKeyElement = new TiXmlElement("rw_node_key_parent");
    TiXmlElement* lpCTParentNodeKeyText = new TiXmlText(mpParentNode->getKey().c_str());
    lpCTParentNodeKeyElement->LinkEndChild(lpCTParentNodeKeyText);
    
    TiXmlElment* lpCTDestNodeKeyElement = new TiXmlElement("rw_node_key_dest");
    TiXmlText* lpCTDestNodeKeyText = new TiXmlText(mpTemplateNode->getKey().c_str());
    lpCTDestNodeKeyElement->LinkEndChild(lpCTDestNodeKeyText);
    
    lpCopyTemplateElement->LinkEndChild(lpCTManipulationMethodElement);
    lpCopyTemplateElement->LinkEndChild(lpCTTreeTypeElement);
    lpCopyTemplateElement->LinkEndChild(lpCTNodeKeyElement);
    lpCopyTemplateElement->LinkEndChild(lpCTDestTreeTypeElement);
    lpCopyTemplateElement->LinkEndChild(lpCTParentNodeKeyElement);
    lpCopyTemplateElement->LinkEndChild(lpCTDestNodeKeyElement);
    lpRootElement->LinkEndChild(lpCopyTemplateElement);
    
    // set display mode
    if(mDisplayMode >= 1)
    {
        TiXmlElement* lpSetDisplayModeElement =  new TiXmlElement("rw_manipulation");
        
        TiXmlElement* lpSDMManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
        TiXmlText* lpSDMManipulationMethodText = new TiXmlText("32");
        lpSDMManipulationMethodElement->LinkEndChild(lpSDMManipulationMethodText);
        
        int lSDMTreeType = static_cast<int>(mpParentNode->getTreeType());
        TiXmlElement* lpSDMTreeTypeElement = new TiXmlElement("rw_tree_type");
        TiXmlText* lpSDMTreeTypeText = new TiXmlText(DSSExpNode::IntToString(lSDMTreeType).c_str());
        lpSDMTreeTypeElement->LinkEndChild(lpSDMTreeTypeText);
        
        TiXmlElement* lpSDMNodeKeyElement = new TiXmlElement("rw_node_key");
        TiXmlText* lpSDMNodeKeyText = new TiXmlText(mpTemplateNode->getKey());
        lpSDMNodeKeyElement->LinkEndChild(lpSDMNodeKeyText);
        
        int lpSDMGridDisplayMode = static_cast<int>(mDisplayMode);
        TiXmlElement* lpSDMGridDisplayModeElement = new TiXmlElement("rw_grid_display_mode");
        TiXmlText* lpSDMGridDisplayModeText = new TiXmlText(DSSExpNode::IntToString(lpSDMGridDisplayMode).c_str());
        lpSDMGridDisplayModeElement->LinkEndChild(lpSDMGridDisplayModeText);
        
        lpSetDisplayModeElement->LinkEndChild(lpSDMManipulationMethodElement);
        lpSetDisplayModeElement->LinkEndChild(lpSDMTreeTypeElement);
        lpSetDisplayModeElement->LinkEndChild(lpSDMNodeKeyElement);
        lpSetDisplayModeElement->LinkEndChild(lpSDMGridDisplayModeElement);
        lpRootElement->LinkEndChild(lpSetDisplayModeElement);
    }
    
    // set properties
    if(mpProperties)
    {
        TiXmlElement* lpSetPropertiesElement = new TiXmlElement("rw_manipulation");
        
        TiXmlElement* lpSPManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
        TiXmlText* lpSPManipulationMethodText = new TiXmlText("7");
        lpSPManipulationMethodElement->LinkEndChild(lpSPManipulationMethodText);
        
        int lpSPTreeType = static_cast<int>(mpParentNode->getTreeType());
        TiXmlElement* lpSPTreeTypeElement = new TiXmlElement("rw_tree_type");
        TiXmlText* lpSPTreeTypeText = new TiXmlText(DSSExpNode::IntToString(lpSPTreeType).c_str());
        lpSPTreeTypeElement->LinkEndChild(lpSPTreeTypeText);
        
        TiXmlElement* lpSPNodeKeyElement = new TiXmlElement("rw_node_key");
        TiXmlText* lpSPNodeKeyText = new TiXmlText(mpTemplateNode->getKey().c_str());
        lpSPNodeKeyElement->LinkEndChild(lpSPNodeKeyText);
        
        DSSObjectContext* lpContext = mpTemplateNode->getObjectContext();
        DSSFormat* lpFormat = new DSSFormat(1, mpProperties, mNumProperties, lpContext);
        // to be parsed
        TiXmlElement* lpFormatElement = new TiXmlElement(lpFormat->GenerateXML());
        
        lpSetPropertiesElement->LinkEndChild(lpSPManipulationMethodElement);
        lpSetPropertiesElement->LinkEndChild(lpSPTreeTypeElement);
        lpSetPropertiesElement->LinkEndChild(lpSPNodeKeyElement);
        lpSetPropertiesElement->LinkEndChild(lpFormatElement);
        lpRootElement->LinkEndChild(lpSetPropertiesElement);
    }
    
    // set graph type
    if(mpGraphProperties)
    {
        TiXmlElement* lpSetGraphTypeElement = new TiXmlElement("rw_manipulation");
        
        TiXmlElement* lpSGTManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
        TiXmlText* lpSGTManipulationMethodText = new TiXmlText("37");
        lpSGTManipulationMethodElement->LinkEndChild(lpSGTManipulationMethodText);
        
        int lSGTTreeType = static_cast<int>(mpParentNode->getTreeType());
        TiXmlElement* lpSGTTreeTypeElement = new TiXmlElement("rw_tree_type");
        TiXmlText* lpSGTTreeTypeText = new TiXmlText(DSSExpNode::IntToString(lSGTTreeType).c_str());
        lpSGTTreeTypeElement->LinkEndChild(lpSGTTreeTypeText);
        
        TiXmlElement* lpSGTNodeKeyElement = new TiXmlElement("rw_node_key");
        TiXmlText* lpSGTNodeKeyText = new TiXmlText(mpTemplateNode->getKey().c_str());
        lpSGTNodeKeyElement->LinkEndChild(lpSGTNodeKeyText);
        
        int lSGTMajorType = static_cast<int>(mpGraphProperties->getGraphType());
        int lSGTMinorType = static_cast<int>(mpGraphProperties->getGraphSubType());
        TiXmlElement* lpSGTGraphTypeElement = new TiXmlElement("gpp");
        lpSGTGraphTypeElement->SetAttribute("gat", DSSExpNode::IntToString(lSGTMajorType).c_str());
        lpSGTGraphTypeElement->SetAttribute("git", DSSExpNode::IntToString(lSGMinorTyp).c_str());
        
        lpSetGraphTypeElement->LinkEndChild(lpSGTManipulationMethodElement);
        lpSetGraphTypeElement->LinkEndChild(lpSGTTreeTypeElement);
        lpSetGraphTypeElement->LinkEndChild(lpSGTNodeKeyElement);
        lpSetGraphTypeElement->LinkEndChild(lpSGTGraphTypeElement);
        lpRootElement->LinkEndChild(lpSetGraphTypeElement);
    }
    
    lDoc.LinkEndChild(lpRootElement);
    TiXmlPrinter lPrinter;
    lPrinter.SetIndent(NULL);
    lPrinter.SetLineBreak(NULL);
    lDoc.Accept(&lPrinter);
    mpActionObject = new DSSAODeltaXML();
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lPrinter.Str());
}
#endif
