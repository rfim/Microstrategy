//
//  DSSCManipulationRevertDDIC.cpp
//  ReportServiceCore
//
//  Created by Zeng Jinliang on 6/25/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#ifdef __ANDROID__
#include "TinyXml/tinyxml.h"
#else
#include "DSSSimpleXML.h"
#endif
#include "DSSCManipulationRevertDDIC.h"
#include "DSSAODeltaXML.h"

DSSCManipulationRevertDDIC::DSSCManipulationRevertDDIC(DSSRWNode* ipNode, DSSRWControl* ipControl, const std::vector<std::string>& iRevertFieldVector)
{
    mType = ManipulationRevertDDIC;
    mpNode = ipNode;
    mpControl = ipControl;
    mHasUC = false;
    mIsFGNode = false;
    mDiscardTransactionFields = iRevertFieldVector;
}

DSSCManipulationRevertDDIC::DSSCManipulationRevertDDIC(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationRevertDDIC;
    mIsFromActionObject = true;

    // TODO parse from xml
}

DSSCManipulationRevertDDIC::~DSSCManipulationRevertDDIC()
{
    mProcessedDDICKeys.clear();
    mDiscardTransactionFields.clear();
}

int DSSCManipulationRevertDDIC::Execute()
{
    int hr = S_OK;
    // 1. param check
    if (!mpControl || !mpNode)
        return S_OK;
    
    if (hCheckLayoutLoaded(mpNode) == S_FALSE)
		return S_FALSE;
    
    DSSObjectContext* lpObjectContext = mpNode->getObjectContext();
    if (!lpObjectContext)
        return S_FALSE;
    
    // 2. do revert.
    DSSRWFieldGroupNode* lpFGNode = dynamic_cast<DSSRWFieldGroupNode*>(mpNode);
    if (!lpFGNode)
    {
        mIsFGNode = false;
        hr = hRevertDDIC(mpControl);
    }
    else
    {
        mIsFGNode = true;
        hr = hRevertFieldDDIC();
    }
    
    // 3. UC need node reload from server
    if (mHasUC)
    {
        mNodeMapServer[mpNode->getKey()] = mpNode;
    }
    
    if (mIsFGNode)
    {
        if (std::find(mNodesModelChanged.begin(), mNodesModelChanged.end(), mpNode) == mNodesModelChanged.end())
            mNodesModelChanged.push_back(mpNode);
    }
    else
    {
        // jzeng, 08/14/14, issue 952085, revert do not refresh the template node because maybe "automatically recalculate.." is not checked. If the setting is checked, it will refresh in Manipulation discardTransaction.
    }
    
    // 4. push xml to sync with iserver
    if (!mIsFromActionObject)
	{
        if (!mProcessedDDICKeys.empty())
        {
            mpActionObject = new DSSAODeltaXML(ManipulationRevertDDIC);
            std::string lXML = hBuildClearElementsXML();
            (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lXML);
        }
    }
   
    return hr;
}

int DSSCManipulationRevertDDIC::getRevertFieldVector(DSSRWNode* ipNode, DSSRWControl* ipControl, std::vector<std::string>& oRevertFieldVector)
{
    // 1. check input
    if ((!ipNode) || (!ipControl))
        return S_FALSE;
    // if it's not FieldGroup Node, it's Revert Field Vector is empty, end with ok
    DSSRWFieldGroupNode* lpFGNode = static_cast<DSSRWFieldGroupNode*>(ipNode);
    if (!lpFGNode)
        return S_OK;
    
    // 2. push the field which control mapping to the vector
    DSSFields* ipFields = static_cast<DSSRWFieldGroupNode*>(ipNode)->getFields();
    std::string lMappedField;
    int lFieldCnt = ipFields->Count();
    for (int i = 0; i < lFieldCnt; i++)
    {
        DSSField* lField = ipFields->Item(i);
        if ((lField) && (lField->getInputControlKey() == ipControl->getKey()))
        {
            lMappedField = lField->getKey();
            break;
        }
    }
    // if the control mapping field is not in this FieldGroup Node, end with ok
    if (lMappedField.empty())
        return S_OK;
    if (std::find(oRevertFieldVector.begin(), oRevertFieldVector.end(), lMappedField) == oRevertFieldVector.end())
    {
        oRevertFieldVector.push_back(lMappedField);
    }
    else
    {
        // if the field info has in the field vector, it means this field has visited, don't recursive
        return S_OK;
    }
    
    // 3. recursively visit it's target
    int hr = S_OK;
    int lTargetControlCount = ipControl->getCountTargetControls();
    for (int i = 0; (hr == S_OK) && (i < lTargetControlCount); i++)
    {
        DSSRWControl* lpTargetControl = ipControl->getTargetControl(i);
        if (!lpTargetControl)
            continue;
        hr = DSSCManipulationRevertDDIC::getRevertFieldVector(ipNode, lpTargetControl, oRevertFieldVector);
    }
    
    return hr;
}

int DSSCManipulationRevertDDIC::hRevertFieldDDIC()
{
    int hr = S_OK;
    // visit every field, and do the revert on its control
    int lFieldCnt = mDiscardTransactionFields.size();
    for (int i = 0; (hr == S_OK) && (i < lFieldCnt); i++)
    {
        DSSField* lField = mpNode->getObjectContext()->getField(mDiscardTransactionFields[i]);
        if (!lField)
            continue;
        
        DSSRWControl* lpControl = lField->getInputControl();
        if (!lpControl)
            continue;
        
        hr = hRevertDDIC(lpControl);
    }
    
    return hr;
}

int DSSCManipulationRevertDDIC::hRevertDDIC(DSSRWControl* ipControl)
{
    // 1. clear control elements
    ipControl->ClearCurrentElements();

    // 2. save current control to processed list
    mProcessedDDICKeys.push_back(ipControl->getKey());
 
    // 3. revert the target control, but don't need recursively revert
    int hr = S_OK;
    int lTargetControlCount = ipControl->getCountTargetControls();
    for (int i = 0; (hr == S_OK) && (i < lTargetControlCount); i++)
    {
        DSSRWControl* lpTargetControl = ipControl->getTargetControl(i);
        if (!lpTargetControl)
            continue;
        hr = hRevertDDICTargetControl(lpTargetControl);
    }
    
    return hr;
}

int DSSCManipulationRevertDDIC::hRevertDDICTargetControl(DSSRWControl* ipTargetControl)
{
    if (!ipTargetControl)
        return S_FALSE;

    // 1. set Groupby Units
    DSSGroupByUnits* lpUnits = ipTargetControl->getGroupbyPath();
    if (lpUnits && lpUnits->Count() > 0)
    {
        DSSGroupByUnit* lpUnit = NULL;
        GUID lControlObjectID = mpControl->getSourceID();
        for (int i = 0; i < lpUnits->Count(); i++)
        {
            DSSGroupByUnit *lpTmpGBUnit = lpUnits->Item(i);
            GUID lGBUnitID = lpTmpGBUnit->getObjectID();
            if (lControlObjectID == lGBUnitID)
            {
                lpUnit = lpTmpGBUnit;
                break;
            }
        }
        
        if (lpUnit)
        {
            lpUnit->ClearCurrentElements();
            lpUnit->setCurrentElementsStatus(mpControl->getCurrentElementsStatus());
        }
    }
    
    // 2. set control model change and node model change
    if (ipTargetControl->getFilterType() == EnumDSSRWControlFilterTypeUnitCondition)
    {
        mHasUC = true;
        mControlsModelChanged.push_back(ipTargetControl);
    }
    DSSRWNode* lpNodeDDIC = mpNode->getObjectContext()->getNode(ipTargetControl->getNodeKeyDDIC());
    if (lpNodeDDIC && (std::find(mNodesModelChanged.begin(), mNodesModelChanged.end(), lpNodeDDIC) == mNodesModelChanged.end()))
        mNodesModelChanged.push_back(lpNodeDDIC);

    return S_OK;
}

#ifdef __ANDROID__
std::string DSSCManipulationRevertDDIC::hBuildClearElementsXML()
{
	return "";
}
#else
std::string DSSCManipulationRevertDDIC::hBuildClearElementsXML()
{
	SimpleXMLBuilder lBuilder;
    for (std::vector<std::string>::iterator it = mProcessedDDICKeys.begin(); it != mProcessedDDICKeys.end(); it++)
    {
        SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
        
        SimpleXMLNode lManipulationXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
        SimpleXMLNode lManipulationXMLContent(SimpleXMLBuilder::CreateNodePtr("63", true));
        lManipulationXML.AddChild(lManipulationXMLContent);
        lXML.AddChild(lManipulationXML);
        
        SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr("1", true));
        lTreeTypeXML.AddChild(lTreeTypeXMLContent);
        lXML.AddChild(lTreeTypeXML);
        
        std::string lStr = mpNode->getKey();
        SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lStr.c_str(), true));
        lNodeKeyXML.AddChild(lNodeKeyXMLContent);
        lXML.AddChild(lNodeKeyXML);

        lStr = *it;
        SimpleXMLNode lControlKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_control_key"));
        SimpleXMLNode lControlKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lStr.c_str(), true));
        lControlKeyXML.AddChild(lControlKeyXMLContent);
        lXML.AddChild(lControlKeyXML);
        
        SimpleXMLNode lElementsXML(SimpleXMLBuilder::CreateNodePtr("element_ids_collection"));
        SimpleXMLNode lELementXML(SimpleXMLBuilder::CreateNodePtr("element_id"));
        lElementsXML.AddChild(lELementXML);
        lXML.AddChild(lElementsXML);
        
        SimpleXMLNode lRoot(lBuilder.GetRoot());
        lRoot.AddChild(lXML);
    }
	return lBuilder.ToString(false);
}
#endif