//
//  DSSCManipulationAddControlOnTemplate.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 12/19/12.
//
//

#include "DSSCManipulationAddControlOnTemplate.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxml.h"
#else
#include "DSSSimpleXML.h"
#endif
#include "DSSBaseElementProxy.h"
#include "DSSRWControls.h"
#include "DSSDataElements.h"
#include "DSSAODeltaXML.h"
#include "DSSBaseElementsProxy.h"

DSSCManipulationAddControlOnTemplate::DSSCManipulationAddControlOnTemplate(EnumDSSRWControlType iControlType, GUID iSourceID, bool iUnitCondition, bool iShowAll, std::vector<std::string>& iTargetKeys, std::vector<PropertyInfo*>& iProperties, DSSRWTemplateNode* ipTemplateNode)
{
    DSSObjectContext* lpContext = ipTemplateNode->getObjectContext();
    mpTemplateNode = ipTemplateNode;
    /*
    DSSRWControls* lpControls = new DSSRWControls(lpContext);
    mpControlNode->setControls(lpControls);
    */
    DSSRWControls* lpControls = mpTemplateNode->getControls();
    mpControl = new DSSRWControl(lpContext);
    lpControls->Add(mpControl);
    std::string lControlKey = lpContext->createControlKey();
    mpControl->setKey(lControlKey);
    lpContext->InsertControl(lControlKey, mpControl);
    mSourceID = iSourceID;
    mShowAll = iShowAll;
    mUnitCondition = iUnitCondition;
    mTargetKeys = iTargetKeys;
    mProperties = iProperties;
    mpControl->setControlType(iControlType);
    mpControl->setSourceID(mSourceID);
    mpControl->setShowAll(mShowAll);
}

DSSCManipulationAddControlOnTemplate::DSSCManipulationAddControlOnTemplate(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    if (!ipAO->IsDeltaXML())
        return;
    
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();
    
    hParseAOXML(lXMLString, ipContext);
#endif
}

DSSCManipulationAddControlOnTemplate::~DSSCManipulationAddControlOnTemplate()
{
}

DSSRWTemplateNode* DSSCManipulationAddControlOnTemplate::getTemplateNode()
{
    return mpTemplateNode;
}

int DSSCManipulationAddControlOnTemplate::Execute()
{
    AddControl(mpTemplateNode, mpControl, mUnitCondition, &mTargetKeys, &mGroupByNodeKeys, &mProperties);
    GenerateAODeltaXML();
    
    mNodesModelChanged.push_back(mpTemplateNode);
    // mNodeMapServer[mpTemplateNode->getKey()] = mpTemplateNode;
    if(!mUnitCondition)
    {
        for(std::vector<std::string>::iterator lIter = mGroupByNodeKeys.begin(); lIter != mGroupByNodeKeys.end(); lIter++)
        {
            std::string lGroupByNodeKey = *lIter;
            DSSRWNode* lpGroupByNode = mpTemplateNode->getObjectContext()->getNode(lGroupByNodeKey);
            mNodeMapServer[lGroupByNodeKey] = lpGroupByNode;
            mNodesModelChanged.push_back(lpGroupByNode);
        }
    }
    return S_OK;
}

/*void DSSCManipulationAddControlOnTemplate::AddControl()
{
    mpControl->setCurrentElementsStatus(DssCurrentElementUnset);
    mpControl->setUnsetStatus(DssXMLRWSelectorUnsetStatusDefault);
    
    for(std::vector<std::string>::iterator lIter = mTargetKeys.begin(); lIter != mTargetKeys.end(); lIter++)
    {
        AddControlTarget(*lIter);
    }
    
    std::vector<PropertyInfo*>::iterator lIt = mProperties.begin();
	for(; lIt!=mProperties.end(); lIt++)
	{
		int lFmtInx = (*lIt)->mFormatIndex;
        std::string lPrptySetName = (*lIt)->mPropertySetName;
        std::string lPrptyName = (*lIt)->mPropertyName;
        CComVariant& lPrptyValue = (*lIt)->mPropertyValue;
        
		bool extendedProperty = false;
		if (lPrptySetName.compare("FormattingPosition")==0)
		{
			if (lPrptyName.compare("Top")==0)
			{
                mpControl->setTop(lPrptyValue.dblVal);
				extendedProperty = true;
			}
			else if (lPrptyName.compare("Left")==0)
			{
                mpControl->setLeft(lPrptyValue.dblVal);
				extendedProperty = true;
			}
		}
		else if (lPrptySetName.compare("FormattingSize")==0)
		{
			if (lPrptyName.compare("Width")==0)
			{
				mpControl->setWidth(lPrptyValue.dblVal);
                extendedProperty = true;
			}
			else if (lPrptyName.compare("Height")==0)
			{
                mpControl->setHeight(lPrptyValue.dblVal);
				extendedProperty = true;
			}
		}
		else if (lPrptySetName.compare("FormattingAppearance")==0)
		{
			if (lPrptyName.compare("Name")==0)
			{
                mpControl->setName(lPrptyValue.ToString());
				extendedProperty = true;
			}
			else if (lPrptyName.compare("TooltipText")==0)
			{
                mpControl->setToolTipText(lPrptyValue.ToString());
				extendedProperty = true;
			}
		}
		else if (lPrptySetName.compare("FormattingOrder")==0)
		{
			if (lPrptyName.compare("ZIndex")==0)
			{
                mpControl->setZIndex(lPrptyValue.iVal);
				extendedProperty = true;
			}
		}
        
		if (!extendedProperty)
        {
            DSSFormat* lpFormat = new DSSFormat(lFmtInx, &mProperties, mpControl->getObjectContext());
            lpFormat->setPropertyValue(lPrptySetName, lPrptyName, lPrptyValue);
        }
    }
}

void DSSCManipulationAddControlOnTemplate::AddControlTarget(std::string iTargetKey)
{
    DSSObjectContext* lpContext = mpControl->getObjectContext();
    DSSRWNode* lpTargetNode = lpContext->getNode(iTargetKey);
    if(!lpTargetNode)
        return;
    DSSRWSectionNode* lpParentNode = static_cast<DSSRWSectionNode*>(lpTargetNode->getParentNode());
    if(!lpParentNode)
        return;
    DSSRWSectionNode* lpTargetGroupByNode;
    if(lpTargetNode->isUCorCGB())
        lpTargetGroupByNode = searchGroupByNodes2(lpTargetNode, mSourceID);
    else
        lpTargetGroupByNode = searchGroupByNodes2(lpParentNode, mSourceID);
    EnumDSSRWSectionType lSectionType = DssRWSectionTypeReserved;
    if(lpTargetGroupByNode)
        lSectionType = lpTargetGroupByNode->getSectionType();
    
    if(lSectionType != DssRWSectionControlGroupBySection && lSectionType != DssRWSectionUnitCondition) // there is no CGB or UC on this source object currently
    {
        // insert the new group by section node into target node and its parent
        lpParentNode->RemoveGroupByNode(lpTargetNode->getIndex(), &mNodesRemoved, &mIteratorsRemoved);
        DSSRWSectionNode* lpSectionNode = new DSSRWSectionNode(lpContext);
        lpSectionNode->setKey(lpContext->createNodeKey());
        lpSectionNode->setSectionType(mUnitCondition ? DssRWSectionUnitCondition : DssRWSectionGroupBySection);
        lpParentNode->AddChild(lpSectionNode);
        lpSectionNode->AddChild(lpTargetNode);
        DSSGroupByUnits* lpGroupByUnits = new DSSGroupByUnits(lpContext);
        lpSectionNode->setGroupByUnits(lpGroupByUnits);
        DSSGroupByUnit* lpGroupByUnit = new DSSGroupByUnit(lpContext);
        lpGroupByUnit->setObjectID(mSourceID);
        lpGroupByUnit->setCurrentElementsStatus(DssCurrentElementUnset);
        lpGroupByUnit->setUnsetStatus(DssXMLRWSelectorUnsetStatusDefault);
        lpGroupByUnits->Add(lpGroupByUnit);
        
        mpControl->addTarget(lpSectionNode->getKey());
        lpContext->AddTargetToNodeAndControls(lpSectionNode->getKey(), mpTemplateNode->getKey(), mpControl->getKey());
        
        // update the group-by unit status according to the control, ported from flash 931
        DSSBaseElementsProxy* lpElements = mpControl->getCurrentElements();
        if(lpElements)
        {
            for(int i = 0; i < lpElements->Count(); i++)
            {
                lpGroupByUnit->AddCurrentElement(lpElements->Item(i));
                if(!mpControl->isUnset())
                    lpGroupByUnit->setCurrentElementsStatus(mpControl->getCurrentElementsStatus());
                
                lpGroupByUnit->setExpression(mpControl->getExpression());
                lpSectionNode->setLatestControlKey(mpControl->getKey());
            }
        }
        
        lpContext->UpdateIteratorAddNode(lpSectionNode);
        
        mGroupByNodeKeys.push_back(lpSectionNode->getKey());
    }
    else
    {
        mpControl->addTarget(lpTargetGroupByNode->getKey());
        lpContext->AddTargetToNodeAndControls(lpTargetGroupByNode->getKey(), mpTemplateNode->getKey(), mpControl->getKey());
    }
}*/

#ifndef __ANDROID__
void DSSCManipulationAddControlOnTemplate::GenerateAODeltaXML()
{
    // add the node
    mpActionObject = new DSSAODeltaXML(ManipulationAddControl);
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    
    // add the control
    SimpleXMLNode lAddControlXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lACManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lACManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("61", true));
    lACManipulationMethodXML.AddChild(lACManipulationMethodXMLContent);
    
    EnumDSSRWTreeType lACTreeType = mpTemplateNode->getTreeType();
    SimpleXMLNode lACTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lACTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lACTreeType).c_str(), true));
    lACTreeTypeXML.AddChild(lACTreeTypeXMLContent);
    
    SimpleXMLNode lACNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lACNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
    lACNodeKeyXML.AddChild(lACNodeKeyXMLContent);
    
    EnumDSSRWNodeType lACNodeType = mpTemplateNode->getType();
    SimpleXMLNode lACNodeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_node_type"));
    SimpleXMLNode lACNodeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lACNodeType).c_str(), true));
    lACNodeTypeXML.AddChild(lACNodeTypeXMLContent);
    
    EnumDSSRWControlType lACControlType = mpControl->getType();
    SimpleXMLNode lACControlTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_control_type"));
    SimpleXMLNode lACControlTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lACControlType).c_str(), true));
    lACControlTypeXML.AddChild(lACControlTypeXMLContent);
    
    SimpleXMLNode lACControlKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_control_key"));
    SimpleXMLNode lACControlKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpControl->getKey().c_str(), true));
    lACControlKeyXML.AddChild(lACControlKeyXMLContent);
    
    SimpleXMLNode lACShowAllXML(SimpleXMLBuilder::CreateNodePtr("show_all"));
    SimpleXMLNode lACShowAllXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(static_cast<int>(mpControl->getShowAll())).c_str() , true));
    lACShowAllXML.AddChild(lACShowAllXMLContent);
    
    lAddControlXML.AddChild(lACManipulationMethodXML);
    lAddControlXML.AddChild(lACTreeTypeXML);
    lAddControlXML.AddChild(lACNodeKeyXML);
    lAddControlXML.AddChild(lACNodeTypeXML);
    lAddControlXML.AddChild(lACControlTypeXML);
    lAddControlXML.AddChild(lACControlKeyXML);
    lAddControlXML.AddChild(lACShowAllXML);
    lRoot.AddChild(lAddControlXML);
    
    // set the control source
    SimpleXMLNode lSetControlSourceXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    SimpleXMLNode lSCSManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lSCSManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("73", true));
    lSCSManipulationMethodXML.AddChild(lSCSManipulationMethodXMLContent);
    
    EnumDSSRWTreeType lSCSTreeType = mpTemplateNode->getTreeType();
    SimpleXMLNode lSCSTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lSCSTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lSCSTreeType).c_str(), true));
    lSCSTreeTypeXML.AddChild(lSCSTreeTypeXMLContent);
    
    SimpleXMLNode lSCSNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lSCSNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
    lSCSNodeKeyXML.AddChild(lSCSNodeKeyXMLContent);
    
    EnumDSSRWControlType lSCSControlType = mpControl->getType();
    SimpleXMLNode lSCSControlTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_control_type"));
    SimpleXMLNode lSCSControlTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lSCSControlType).c_str(), true));
    lSCSControlTypeXML.AddChild(lSCSControlTypeXMLContent);
    
    SimpleXMLNode lSCSControlKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_control_key"));
    SimpleXMLNode lSCSControlKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpControl->getKey().c_str(), true));
    lSCSControlKeyXML.AddChild(lSCSControlKeyXMLContent);
        
    lSetControlSourceXML.AddChild(lSCSManipulationMethodXML);
    lSetControlSourceXML.AddChild(lSCSTreeTypeXML);
    lSetControlSourceXML.AddChild(lSCSNodeKeyXML);
    lSetControlSourceXML.AddChild(lSCSControlKeyXML);
    lSetControlSourceXML.AddChild(lSCSControlTypeXML);
    
    if(mSourceID != GUID_NULL)
    {
        int lSourceType = static_cast<int>(mpControl->getObjectContext()->FindObjectH(mSourceID)->getType());
        SimpleXMLNode lSCSSourceTypeXML(SimpleXMLBuilder::CreateNodePtr("unit_type"));
        SimpleXMLNode lSCSSourceTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lSourceType).c_str(), true));
        lSCSSourceTypeXML.AddChild(lSCSSourceTypeXMLContent);
        
        char lSourceIDString[33];
        DSSBaseElementProxy::ConvertGUIDToString(mSourceID, lSourceIDString);
        SimpleXMLNode lSCSSourceIDXML(SimpleXMLBuilder::CreateNodePtr("unit_id"));
        SimpleXMLNode lSCSSourceIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lSourceIDString, true));
        lSCSSourceIDXML.AddChild(lSCSSourceIDXMLContent);
        
        lSetControlSourceXML.AddChild(lSCSSourceTypeXML);
        lSetControlSourceXML.AddChild(lSCSSourceIDXML);
    }
    lRoot.AddChild(lSetControlSourceXML);
    
    if(mShowAll)
    {
        // show all
        SimpleXMLNode lShowAllXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
        
        SimpleXMLNode lSAManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
        SimpleXMLNode lSAManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("78", true));
        lSAManipulationMethodXML.AddChild(lSAManipulationMethodXMLContent);
        
        EnumDSSRWTreeType lSATreeType = mpTemplateNode->getTreeType();
        SimpleXMLNode lSATreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lSATreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lSATreeType).c_str(), true));
        lSATreeTypeXML.AddChild(lSATreeTypeXMLContent);
        
        SimpleXMLNode lSANodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lSANodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
        lSANodeKeyXML.AddChild(lSANodeKeyXMLContent);
        
        int lShowAll = static_cast<int>(mShowAll);
        SimpleXMLNode lSAShowAllXML(SimpleXMLBuilder::CreateNodePtr("show_all"));
        SimpleXMLNode lSAShowAllXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lShowAll).c_str(), true));
        lSAShowAllXML.AddChild(lSAShowAllXMLContent);
        
        lShowAllXML.AddChild(lSAManipulationMethodXML);
        lShowAllXML.AddChild(lSATreeTypeXML);
        lShowAllXML.AddChild(lSANodeKeyXML);
        lShowAllXML.AddChild(lACControlKeyXML);
        lShowAllXML.AddChild(lSAShowAllXML);
        lRoot.AddChild(lShowAllXML);
    }

    // group by
    for(int i = 0; i < mGroupByNodeKeys.size(); i++)
    {
        std::string lGroupByKey = mGroupByNodeKeys[i];
        std::string lTargetKey = mTargetKeys[i];
        if(lGroupByKey.compare("") != 0)
        {
            // insert CGB
            SimpleXMLNode lInsertCGBXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
            
            SimpleXMLNode lICGBManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
            SimpleXMLNode lICGBManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("64", true));
            lICGBManipulationMethodXML.AddChild(lICGBManipulationMethodXMLContent);
            
            EnumDSSRWTreeType lICGBTreeType = mpTemplateNode->getTreeType();
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
            SimpleXMLNode lICGBGroupByUnitTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_group_by_unit_type"));
            SimpleXMLNode lICGBGroupByUnitTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lControlType).c_str(), true));
            lICGBGroupByUnitTypeXML.AddChild(lICGBGroupByUnitTypeXMLContent);
            
            lInsertCGBXML.AddChild(lICGBManipulationMethodXML);
            lInsertCGBXML.AddChild(lICGBTreeTypeXML);
            lInsertCGBXML.AddChild(lICGBNodeKeyChildXML);
            lInsertCGBXML.AddChild(lICGBNodeKeyXML);
            lInsertCGBXML.AddChild(lICGBGroupByUnitTypeXML);
            
            char lSourceIDString[33];
            DSSBaseElementProxy::ConvertGUIDToString(mSourceID, lSourceIDString);
            if(mSourceID != GUID_NULL)
            {
                int lSourceType = static_cast<int>(mpControl->getObjectContext()->FindObjectH(mSourceID)->getType());
                SimpleXMLNode lICGBSourceTypeXML(SimpleXMLBuilder::CreateNodePtr("unit_type"));
                SimpleXMLNode lICGBSourceTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lSourceType).c_str(), true));
                lICGBSourceTypeXML.AddChild(lICGBSourceTypeXMLContent);
                
                char lSourceIDString[33];
                DSSBaseElementProxy::ConvertGUIDToString(mSourceID, lSourceIDString);
                SimpleXMLNode lICGBSourceIDXML(SimpleXMLBuilder::CreateNodePtr("unit_id"));
                SimpleXMLNode lICGBSourceIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lSourceIDString, true));
                lICGBSourceIDXML.AddChild(lICGBSourceIDXMLContent);
                
                lInsertCGBXML.AddChild(lICGBSourceTypeXML);
                lInsertCGBXML.AddChild(lICGBSourceIDXML);
            }
            lRoot.AddChild(lInsertCGBXML);
            
            if(mUnitCondition)
            {
                SimpleXMLNode lCGB2UCXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
                
                SimpleXMLNode lCUManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
                SimpleXMLNode lCUManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("94", true));
                lCUManipulationMethodXML.AddChild(lCUManipulationMethodXMLContent);
                
                EnumDSSRWTreeType lCUTreeType = mpTemplateNode->getTreeType();
                SimpleXMLNode lCUTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
                SimpleXMLNode lCUTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lCUTreeType).c_str(), true));
                lCUTreeTypeXML.AddChild(lCUTreeTypeXMLContent);
                
                SimpleXMLNode lCUNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
                SimpleXMLNode lCUNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lGroupByKey.c_str(), true));
                lCUNodeKeyXML.AddChild(lCUNodeKeyXMLContent);
                
                lCGB2UCXML.AddChild(lCUManipulationMethodXML);
                lCGB2UCXML.AddChild(lCUTreeTypeXML);
                lCGB2UCXML.AddChild(lCUNodeKeyXML);
                lRoot.AddChild(lCGB2UCXML);
            }
        }
        // add the control target
        SimpleXMLNode lAddControlTargetXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
        
        SimpleXMLNode lACTManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
        SimpleXMLNode lACTManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("67", true));
        lACTManipulationMethodXML.AddChild(lACTManipulationMethodXMLContent);
        
        EnumDSSRWTreeType lACTTreeType = mpTemplateNode->getTreeType();
        SimpleXMLNode lACTTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lACTTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lACTTreeType).c_str(), true));
        lACTTreeTypeXML.AddChild(lACTTreeTypeXMLContent);
        
        SimpleXMLNode lACTNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lACTNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
        lACTNodeKeyXML.AddChild(lACTNodeKeyXMLContent);
        
        SimpleXMLNode lACTControlKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_control_key"));
        SimpleXMLNode lACTControlKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpControl->getKey().c_str(), true));
        lACTControlKeyXML.AddChild(lACTControlKeyXMLContent);
        
        SimpleXMLNode lACTTargetKeyXML(SimpleXMLBuilder::CreateNodePtr("target_key"));
        SimpleXMLNode lACTTargetKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpControl->getTargetKey(i).c_str(), true));
        lACTTargetKeyXML.AddChild(lACTTargetKeyXMLContent);
        
        lAddControlTargetXML.AddChild(lACTManipulationMethodXML);
        lAddControlTargetXML.AddChild(lACTTreeTypeXML);
        lAddControlTargetXML.AddChild(lACTNodeKeyXML);
        lAddControlTargetXML.AddChild(lACTControlKeyXML);
        lAddControlTargetXML.AddChild(lACTTargetKeyXML);
        lRoot.AddChild(lAddControlTargetXML);
    }
    // set control element format
    SimpleXMLNode lSetControlElementStatusXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lSSManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lSSManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("82", true));
    lSSManipulationMethodXML.AddChild(lSSManipulationMethodXMLContent);
    
    EnumDSSRWTreeType lSSTreeType = mpTemplateNode->getTreeType();
    SimpleXMLNode lSSTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lSSTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lSSTreeType).c_str(), true));
    lSSTreeTypeXML.AddChild(lSSTreeTypeXMLContent);
    
    SimpleXMLNode lSSNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lSSNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
    lSSNodeKeyXML.AddChild(lSSNodeKeyXMLContent);
    
    SimpleXMLNode lSSControlKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_control_key"));
    SimpleXMLNode lSSControlKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpControl->getKey().c_str(), true));
    lSSControlKeyXML.AddChild(lSSControlKeyXMLContent);
    
    int lElementStatus = static_cast<int>(mpControl->getCurrentElementsStatus());
    SimpleXMLNode lSSElementStatusXML(SimpleXMLBuilder::CreateNodePtr("element_status"));
    SimpleXMLNode lSSElementStatusXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lElementStatus).c_str(), true));
    lSSElementStatusXML.AddChild(lSSElementStatusXMLContent);
    
    lSetControlElementStatusXML.AddChild(lSSManipulationMethodXML);
    lSetControlElementStatusXML.AddChild(lSSTreeTypeXML);
    lSetControlElementStatusXML.AddChild(lSSNodeKeyXML);
    lSetControlElementStatusXML.AddChild(lSSControlKeyXML);
    lSetControlElementStatusXML.AddChild(lSSElementStatusXML);
    lRoot.AddChild(lSetControlElementStatusXML);
    
    // set the format of control
    SimpleXMLNode lSetControlFormatXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lSCFManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lSCFManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("62", true));
    lSCFManipulationMethodXML.AddChild(lSCFManipulationMethodXMLContent);
    
    EnumDSSRWTreeType lSCFTreeType = mpTemplateNode->getTreeType();
    SimpleXMLNode lSCFTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lSCFTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lSCFTreeType).c_str(), true));
    lSCFTreeTypeXML.AddChild(lSCFTreeTypeXMLContent);
    
    SimpleXMLNode lSCFNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lSCFNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
    lSCFNodeKeyXML.AddChild(lSCFNodeKeyXMLContent);
    
    SimpleXMLNode lSCFControlKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_control_key"));
    SimpleXMLNode lSCFControlKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpControl->getKey().c_str(), true));
    lSCFControlKeyXML.AddChild(lSCFControlKeyXMLContent);
    
    DSSFormat* lpFormat = new DSSFormat(1, &mProperties, mpControl->getObjectContext());
    xmlNodePtr lpFormatNode = static_cast<xmlNodePtr> (lpFormat->GenerateXMLNode());
    SimpleXMLNode lSCFFormatXML(lpFormatNode);
    
    lSetControlFormatXML.AddChild(lSCFManipulationMethodXML);
    lSetControlFormatXML.AddChild(lSCFTreeTypeXML);
    lSetControlFormatXML.AddChild(lSCFNodeKeyXML);
    lSetControlFormatXML.AddChild(lSCFControlKeyXML);
    lSetControlFormatXML.AddChild(lSCFFormatXML);
    lRoot.AddChild(lSetControlFormatXML);
    
    // debug
    std::string lXMLString = lBuilder.ToString(false);
    // enddebug
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationAddControlOnTemplate::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser(iXMLString);
    if(!lParser.IsValid())
        return;
    
    SimpleXMLNode lCurrentNode = lParser.GetRoot();
    
    std::string lStrContent;
    for(lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        // rw_manipulation
        if(lCurrentNode.IsElementNode())
        {
            SimpleXMLNode lManipulationNode = lCurrentNode.GetFirstChild();
            if(lManipulationNode.GetName().compare("rw_manipulation_method") != 0)
                return;
            
            lManipulationNode.GetContent(lStrContent);
            // if add control
            if(lStrContent.compare("61") == 0)
            {
                std::string lACStrContent;
                for(lManipulationNode = lCurrentNode.GetFirstChild(); lManipulationNode.IsValid(); lManipulationNode = lManipulationNode.GetNextSibling())
                {
                    if(lManipulationNode.GetName().compare("rw_node_key") == 0)
                    {
                        lManipulationNode.GetContent(lACStrContent);
                        mpTemplateNode = static_cast<DSSRWTemplateNode*>(ipContext->getNode(lACStrContent));
                    }
                    if(lManipulationNode.GetName().compare("rw_control_key") == 0)
                    {
                        lManipulationNode.GetContent(lACStrContent);
                        mpControl = ipContext->getControl(lACStrContent);
                    }
                    else if(lManipulationNode.GetName().compare("show_all") == 0)
                    {
                        lManipulationNode.GetContent(lACStrContent);
                        mShowAll = static_cast<bool>(atoi(lACStrContent.c_str()));
                    }
                }
            }
            // if set source
            else if(lStrContent.compare("73") == 0)
            {
                std::string lSCStrContent;
                for(lManipulationNode = lCurrentNode.GetFirstChild(); lManipulationNode.IsValid(); lManipulationNode = lManipulationNode.GetNextSibling())
                {
                    if(lManipulationNode.GetName().compare("unit_id") == 0)
                    {
                        lManipulationNode.GetContent(lSCStrContent);
                        DSSBaseElementProxy::ConvertFromStringToGUID(lSCStrContent, mSourceID);
                    }
                }
            }
            // if insert cgb
            else if(lStrContent.compare("64") == 0)
            {
                std::string lICGBStrContent;
                for(lManipulationNode = lCurrentNode.GetFirstChild(); lManipulationNode.IsValid(); lManipulationNode = lManipulationNode.GetNextSibling())
                {
                    if(lManipulationNode.GetName().compare("rw_node_key_child") == 0)
                    {
                        lManipulationNode.GetContent(lICGBStrContent);
                        mTargetKeys.push_back(lICGBStrContent);
                    }
                    else if(lManipulationNode.GetName().compare("rw_node_key") == 0)
                    {
                        lManipulationNode.GetContent(lICGBStrContent);
                        mGroupByNodeKeys.push_back(lICGBStrContent);
                    }
                }
            }
            // format
            else if(lStrContent.compare("62") == 0)
            {
                std::string lstrContent;
                std::string lstrPropertySetName;
                std::string lstrPropertyName;
                EnumVariantType lValueType;
                CComVariant lValue;
                SimpleXMLNode lSubNode;
                for (SimpleXMLNode lFormatNode = lCurrentNode.GetFirstChild(); lFormatNode.IsValid(); lFormatNode.GetNextSibling())
                {
                    if (lFormatNode.IsElementNode())
                    {
                        if (lFormatNode.GetName().compare("prs") == 0)
                        {
                            lFormatNode.GetProperty("n", lstrPropertySetName);
                            for (lSubNode=lFormatNode.GetFirstChild(); lSubNode.IsValid(); lSubNode.GetNextSibling())
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
                
            }
        }
    }
}
#endif