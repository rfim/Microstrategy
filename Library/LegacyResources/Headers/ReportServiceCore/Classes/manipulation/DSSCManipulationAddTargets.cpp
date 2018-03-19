//
//  DSSCManipulationAddTargets.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 3/26/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#include "DSSCManipulationAddTargets.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxml.h"
#else
#include "DSSSimpleXML.h"
#endif
#include "DSSBaseElementProxy.h"

DSSCManipulationAddTargets::DSSCManipulationAddTargets(DSSRWControlNode* ipControlNode, DSSRWControl* ipControl, std::vector<std::string> iTargetKeys): DSSCManipulationAddControl(ipControlNode, ipControl, iTargetKeys)
{
    mpControlNode = ipControlNode;
    mpNode = static_cast<DSSRWNode*>(mpControlNode);
    mpControl = ipControl;
    if(static_cast<DSSRWSectionNode*>(ipControlNode->getParentNode())->getSectionType() == DssRWSectionUnitCondition)
        mUnitCondition = true;
    else
        mUnitCondition = false;
    
    mType = ManipulationAddTargets;
}

DSSCManipulationAddTargets::DSSCManipulationAddTargets(DSSActionObject* ipAO, DSSObjectContext* ipContext): DSSCManipulationAddControl(ipAO, ipContext)
{
    mType = ManipulationAddTargets;
    
    if (!ipAO->IsDeltaXML())
        return;
    
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();
    
    hParseAOXML(lXMLString, ipContext);
#endif
}

DSSCManipulationAddTargets::~DSSCManipulationAddTargets()
{
    
}

int DSSCManipulationAddTargets::Execute()
{
    for(std::vector<std::string>::iterator lIter = mTargetKeys.begin(); lIter != mTargetKeys.end(); lIter++)
    {
    //    AddControlTarget(*lIter);
    }
    mNodesModelChanged.push_back(mpNode);
    if(!mUnitCondition)
    {
        for(std::vector<std::string>::iterator lIter = mGroupByNodeKeys.begin(); lIter != mGroupByNodeKeys.end(); lIter++)
            mNodesModelChanged.push_back(mpControl->getObjectContext()->getNode(*lIter));
    }
    GenerateAODeltaXML();
    return S_OK;
}
#ifndef __ANDROID__
void DSSCManipulationAddTargets::GenerateAODeltaXML()
{
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    // rw_manipulations?
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulations"));
    
    for(int i = 0; i < mTargetKeys.size(); i++)
    {
        // not sure
        std::string lGroupByKey = mGroupByNodeKeys[i];
        std::string lTargetKey = mTargetKeys[i];
        if(lGroupByKey.compare("") != 0)
        {
            // insert CGB
            SimpleXMLNode lInsertCGBXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
            
            SimpleXMLNode lICGBManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
            SimpleXMLNode lICGBManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("64", true));
            lICGBManipulationMethodXML.AddChild(lICGBManipulationMethodXMLContent);
            
            EnumDSSRWTreeType lICGBTreeType = mpNode->getTreeType();
            SimpleXMLNode lICGBTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
            SimpleXMLNode lICGBTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lICGBTreeType).c_str(), true));
            lICGBTreeTypeXML.AddChild(lICGBTreeTypeXMLContent);
            
            SimpleXMLNode lICGBNodeKeyChildXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key_child"));
            SimpleXMLNode lICGBNodeKeyChildXMLContent(SimpleXMLBuilder::CreateNodePtr(lTargetKey.c_str(), true));
            lICGBNodeKeyChildXML.AddChild(lICGBNodeKeyChildXMLContent);
            
            SimpleXMLNode lICGBNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
            SimpleXMLNode lICGBNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lGroupByKey.c_str(), true));
            lICGBNodeKeyXML.AddChild(lICGBNodeKeyXMLContent);
            
            int lControlType = static_cast<int>(mpControl->getType());
            SimpleXMLNode lICGBGroupByUnitType(SimpleXMLBuilder::CreateNodePtr("rw_group_by_type"));
            SimpleXMLNode lICGBGroupByUnitTypeXML(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lControlType).c_str(), true));
            lICGBGroupByUnitType.AddChild(lICGBNodeKeyXMLContent);
            
            lInsertCGBXML.AddChild(lICGBManipulationMethodXML);
            lInsertCGBXML.AddChild(lICGBTreeTypeXML);
            lInsertCGBXML.AddChild(lICGBNodeKeyXML);
            lInsertCGBXML.AddChild(lICGBGroupByUnitTypeXML);
            
            char lSourceIDString[33];
            DSSBaseElementProxy::ConvertGUIDToString(mSourceID, lSourceIDString);
            if(lSourceIDString)
            {
                int lSourceType = static_cast<int>(mpControl->getObjectContext()->FindObjectH(mSourceID)->getType());
                SimpleXMLNode lSCSSourceTypeXML(SimpleXMLBuilder::CreateNodePtr("unit_type"));
                SimpleXMLNode lSCSSourceTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lSourceType).c_str(), true));
                lSCSSourceTypeXML.AddChild(lSCSSourceTypeXMLContent);
                
                char lSourceIDString[33];
                DSSBaseElementProxy::ConvertGUIDToString(mSourceID, lSourceIDString);
                SimpleXMLNode lSCSSourceIDXML(SimpleXMLBuilder::CreateNodePtr("unit_id"));
                SimpleXMLNode lSCSSourceIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lSourceIDString));
                lSCSSourceTypeXML.AddChild(lSCSSourceIDXMLContent);
                
                lInsertCGBXML.AddChild(lSCSSourceTypeXML);
                lInsertCGBXML.AddChild(lSCSSourceIDXML);
            }
            lXML.AddChild(lInsertCGBXML);
            
            if(mUnitCondition)
            {
                SimpleXMLNode lCGB2UCXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
                
                SimpleXMLNode lCUManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
                SimpleXMLNode lCUManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("94", true));
                lCUManipulationMethodXML.AddChild(lCUManipulationMethodXMLContent);
                
                SimpleXMLNode lCUNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
                SimpleXMLNode lCUNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lGroupByKey.c_str(), true));
                lCUNodeKeyXML.AddChild(lCUNodeKeyXMLContent);
                
                lCGB2UCXML.AddChild(lCUManipulationMethodXML);
                lCGB2UCXML.AddChild(lICGBTreeTypeXML);
                lCGB2UCXML.AddChild(lCUNodeKeyXML);
                lXML.AddChild(lCGB2UCXML);
            }
        }
        // add the control target
        SimpleXMLNode lAddControlTargetXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
        
        SimpleXMLNode lACTManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
        SimpleXMLNode lACTManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("67", true));
        lACTManipulationMethodXML.AddChild(lACTManipulationMethodXMLContent);
        
        EnumDSSRWTreeType lACTTreeType = mpNode->getTreeType();
        SimpleXMLNode lACTTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lACTTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lACTTreeType).c_str(), true));
        lACTTreeTypeXML.AddChild(lACTTreeTypeXMLContent);
        
        SimpleXMLNode lACTNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lACTNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lGroupByKey.c_str(), true));
        lACTNodeKeyXML.AddChild(lACTNodeKeyXMLContent);
        
        SimpleXMLNode lACTControlKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_control_key"));
        SimpleXMLNode lACTControlKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpControl->getKey().c_str(), true));
        lACTControlKeyXML.AddChild(lACTControlKeyXMLContent);
        
        SimpleXMLNode lACTTargetKeyXML(SimpleXMLBuilder::CreateNodePtr("target_key"));
        SimpleXMLNode lACTTargetKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lTargetKey.c_str(), true));
        lACTTargetKeyXML.AddChild(lACTTargetKeyXMLContent);
        
        lAddControlTargetXML.AddChild(lACTManipulationMethodXML);
        lAddControlTargetXML.AddChild(lACTTreeTypeXML);
        lAddControlTargetXML.AddChild(lACTNodeKeyXML);
        lAddControlTargetXML.AddChild(lACTControlKeyXML);
        lAddControlTargetXML.AddChild(lACTTargetKeyXML);
        lXML.AddChild(lAddControlTargetXML);
    }
    lRoot.AddChild(lXML);
    mpActionObject = new DSSAODeltaXML(ManipulationAddTargets);
    static_cast<DSSAODeltaXML*>(mpActionObject)->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationAddTargets::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser(iXMLString);
    if(!lParser.IsValid())
        return;
    
    SimpleXMLNode lCurrentNode = lParser.GetRoot();
    if(!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulations") != 0)
        return;
    
    std::string lStrContent;
    std::vector<std::string> lTargetKeys;
    for(lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        // rw_manipulation
        if(lCurrentNode.IsElementNode())
        {
            SimpleXMLNode lManipulationNode = lCurrentNode.GetFirstChild();
            if(lManipulationNode.GetName().compare("rw_manipulation_method") != 0)
                return;
            
            lManipulationNode.GetContent(lStrContent);
            // if add node
            if(lStrContent.compare("67") == 0)
            {
                for(lManipulationNode = lCurrentNode.GetFirstChild(); lManipulationNode.IsValid(); lManipulationNode = lManipulationNode.GetNextSibling())
                {
                    if(lManipulationNode.GetName().compare("rw_node_key") == 0)
                    {
                        lManipulationNode.GetContent(lStrContent);
                        mpNode = ipContext->getNode(lStrContent);
                        mpControlNode = static_cast<DSSRWControlNode*>(mpNode);
                    }
                    else if(lManipulationNode.GetName().compare("rw_control_key") == 0)
                    {
                        lManipulationNode.GetContent(lStrContent);
                        mpControl = ipContext->getControl(lStrContent);
                    }
                }
            }
            else if(lStrContent.compare("64") == 0)
            {
                for(lManipulationNode = lCurrentNode.GetFirstChild(); lManipulationNode.IsValid(); lManipulationNode = lManipulationNode.GetNextSibling())
                {
                    if(lManipulationNode.GetName().compare("rw_node_key_child") == 0)
                    {
                        lManipulationNode.GetContent(lStrContent);
                        lTargetKeys.push_back(lStrContent);
                    }
                }
            }
        }
    }
    mTargetKeys = lTargetKeys;
}
#endif