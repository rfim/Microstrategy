/*
 *  DSSManipulationSetSelectionFromControl.cpp
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-3-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSCManipulationSetSelectionFromControl.h"
#include "DSSRWTemplateNode.h"
#include "DSSAOSelection.h"
#include "DSSAODeltaXML.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxml.h"
#else
#include "DSSSimpleXML.h"
#endif
#include "DSSCExpression.h"
#include "DSSExpNodeOperator.h"
#include "DSSExpNodeShortcut.h"
#include "DSSExpNodeConstant.h"
#include "DSSExpNodeTime.h"
#include "DSSExpNodeElementsObject.h"
#include "DSSConcreteElementProxy.h"
#include "DSSDocumentDefinition.h"
#include "DSSFunction.h"
#include "DSSCManipulationHelper.h"
#include "DSSAttributeListElementProxy.h"
DSSCManipulationSetSelectionFromControl::DSSCManipulationSetSelectionFromControl(DSSRWControlNode* ipControlNode, DSSBaseElementsProxy *ipElements, DSSCExpression* ipExpression, bool iIsNotIn)
{
    mType = ManipulationSetSelectionFromControl;
    
    mpElements = NULL;
	mpControlNode = ipControlNode;
	
    mpExpression = NULL;
    if (ipExpression) {
        mpExpression = new DSSCExpression(ipExpression->getObjectContext());
        mpExpression->Clone(ipExpression);
    }
	mIsNotIn = iIsNotIn;
    
	mpControl = NULL;
	DSSRWControls* lpControls = mpControlNode->getControls();
	if (lpControls && lpControls->Count()>0)
		mpControl = lpControls->Item(0);
    
	mIsFromActionObject = false;
	
	if (ipElements)
	{
		DSSObjectContext* lpObjectContext = mpControlNode->getObjectContext();
		mpElements = new DSSBaseElementsProxy(lpObjectContext);
		int lCount = ipElements->Count();
		for (int i=0; i<lCount; i++)
		{
			DSSBaseElementProxy* lpElement = ipElements->Item(i);
			if (lpElement->getElementType()==DssElementConcrete && lpElement->getUnitType()==DssTemplateAttribute && lpElement->getElementID()[2] - ':' != 0)
			{
				std::string lElementID = ((DSSAttributeElementProxy*)lpElement)->getCompactElementID();
				DSSAttributeElementProxy* lpElementNew = new DSSAttributeElementProxy(lpObjectContext);
				lpElementNew->Parse(lElementID);
				lpElementNew->setDisplayText(lpElement->getDisplayText());
				mpElements->AddElement(lpElementNew);
			}
			else
			{
				DSSBaseElementProxy* lpElementNew = DSSBaseElementsProxy::Clone(lpElement, lpObjectContext);
				mpElements->AddElement(lpElementNew);
			}
		}
		delete ipElements;
	}
    
    mpRedrawParentNode = NULL;
    mRedrawChildIndex = -1;
    mIsUC = false;
}

DSSCManipulationSetSelectionFromControl::DSSCManipulationSetSelectionFromControl(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationSetSelectionFromControl;
    
	if (!ipAO->IsDeltaXML())
	{
		mpExpression = NULL;
		mIsNotIn = false;
        
		DSSAOSelection* lpAO = static_cast<DSSAOSelection*> (ipAO);
		mpControlNode = (DSSRWControlNode*)(ipContext->getNode(lpAO->getNodeKey()));
		
		mpElements = new DSSBaseElementsProxy(ipContext);
		StringVector::iterator it1;
		for (it1=lpAO->getElementIDs()->begin(); it1!=lpAO->getElementIDs()->end(); it1++)
		{
			DSSBaseElementProxy* lpElement = DSSBaseElementsProxy::CreateElementFromID(*it1, ipContext);
			
			if (lpElement)
				mpElements->AddElement(lpElement);
		}
	}
	else
	{
		mpElements = NULL;
		//mIsNotIn = true;
        
		DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
		std::string lXMLString = lpAO->getXML();
		mIsNotIn = lpAO->getIsNotIn();
		
		// parse lXMLString and construct the mpControlNode and mpExpression
		hParseExpressionFromXML(lXMLString, ipContext);
        
	}
    
    
	mpPanelStack = NULL;
    
	mpControl = NULL;
	DSSRWControls* lpControls = mpControlNode->getControls();
	if (lpControls && lpControls->Count()>0)
		mpControl = lpControls->Item(0);
    
	mIsFromActionObject = true;
    
    mpRedrawParentNode = NULL;
    mRedrawChildIndex = -1;
    mIsUC = false;
}

DSSCManipulationSetSelectionFromControl::~DSSCManipulationSetSelectionFromControl()
{
	NodeAndControlsMap::iterator it;
	for ( it=mNodeMapSelectionUpdate.begin() ; it != mNodeMapSelectionUpdate.end(); it++ )
	{
		if (mpControlNode->getObjectContext()->getNode((*it).second->NodeKey)->getType()==DssRWNodeTemplate)
			delete (*it).second;
	}
    
	if (mpElements)
	{
		delete mpElements;
		mpElements = NULL;
	}
	
    if (mpExpression) {
        delete mpExpression;
        mpExpression = NULL;
    }
}

bool DSSCManipulationSetSelectionFromControl::IsNeedServer()
{
    return mIsUC || mRetrievalGlobalLookupTableHeader || (mNodeMapServer.size() > 0);
}

int DSSCManipulationSetSelectionFromControl::Execute()
{
	bool lLayoutLoaded = true;
    
	if (!mpControl)
		return S_OK;
    
    DSSObjectContext* lpObjectContext = mpControlNode->getObjectContext();
    EnumDSSRWControlType lType = mpControl->getType();
    //only support single support for attribute selector
    if (lType == DssRWControlTypeAttributeList) {
        if (mpElements && mpElements->Count()>1) {
            return S_OK;
        }
    }
    int lTargetCount;
    // bool isUC = false;
    DSSRWNode* lHighestNGB = NULL;
    if (lType == DssRWControlTypeElementList || lType == DssRWControlTypeMetricList || lType == DssRWControlTypeAttributeList)
    {
        if(mpControl->getTargetDatasetCount() > 0)
        {
            mIsUC = true;
        }
        else
        {
            if(mpControl->getCountTargets() > 0)
            {
                lTargetCount = mpControl->getCountTargets();
                for (int i=0; i<lTargetCount; i++)
                {
                    std::string lTargetKey = mpControl->getTargetKey(i);
                    DSSRWNode* lpTargetNode = lpObjectContext->getNode(lTargetKey);
                    if (!lpTargetNode || lpTargetNode->getType()!=DssRWNodeSection)
                        continue;
                    DSSRWSectionNode* lpSectionNode = (DSSRWSectionNode*)(lpTargetNode);
                    
                    if (lpSectionNode->getSectionType()==DssRWSectionUnitCondition)
                    {
                        mIsUC = true;
                        break;
                    }
                }
            }
            if(!mIsUC && mpControl->getCountTargetControls() > 0)
            {
                lTargetCount = mpControl->getCountTargetControls();
                for(int i = 0; i < lTargetCount; i++)
                {
                    std::string lTargetControlKey = mpControl->getTargetControlKey(i);
                    DSSRWControl* lpTargetControl = lpObjectContext->getControl(lTargetControlKey);
                    if(!lpTargetControl)
                        continue;
                    if(lpTargetControl->getFilterType() == EnumDSSRWControlFilterTypeUnitCondition)
                    {
                        mIsUC = true;
                        break;
                    }
                }
            }
        }
    }
    
	if (hCheckLayoutLoaded(mpControlNode)==S_FALSE)
    {
        if (mIsUC)
            return S_FALSE;
        else
            if (mIsLayoutKeyNeededInitial)//TQMS:777320 we need to wait for init layout finishing downloading, then continue the manipulation or we will hit multi-thread crash in AE
                return S_FALSE;
            else
                lLayoutLoaded = false;
    }
    
    bool isLayoutPageHeaderOrFooter = false;
    DSSRWNode* lpLayoutNode = mpControlNode->getLayout();
    if (lpLayoutNode->getType()==DssRWNodeSection && (((DSSRWSectionNode*)lpLayoutNode)->getSectionType()==DssRWSectionPageHeader || ((DSSRWSectionNode*)lpLayoutNode)->getSectionType()==DssRWSectionPageFooter))
        isLayoutPageHeaderOrFooter = true;
    
    int ltgtCnt = mpControl->getCountTargets();
    for (int i=0; i<ltgtCnt; i++)
    {
        std::string lstrTgtKey = mpControl->getTargetKey(i);
        if (isLayoutPageHeaderOrFooter && hCheckLayoutLoaded(lpObjectContext->getNode(lstrTgtKey))==S_FALSE)
            return S_FALSE;
    }
    
    int lTargetControlCount = mpControl->getCountTargetControls();
    for (int i = 0; i < lTargetControlCount; i++)
    {
        std::string lTargetControlKey = mpControl->getTargetControlKey(i);
        std::string lNodeKeyDDIC;
        if (lpObjectContext->getControl(lTargetControlKey))
            lNodeKeyDDIC = lpObjectContext->getControl(lTargetControlKey)->getNodeKeyDDIC();
        if (isLayoutPageHeaderOrFooter && hCheckLayoutLoaded(lpObjectContext->getNode(lNodeKeyDDIC))==S_FALSE)
            return S_FALSE;
    }
	
	std::vector<DSSRWNode*> lNodesModelChanged;
    
	// Set current elments on control
    
	if ((mpExpression == NULL && mIsNotIn == false) || (mIsNotIn && mpExpression == NULL && mpElements && mpElements->Count()>0 && mpElements->Item(0)->getElementType()==DssElementAll)) //Not DSSMetric Condition or NotIn, or exclude all (changing to include all)
	{
		mIsNotIn = false;
		
		DSSBaseElementsProxy* lpElements = mpControl->getCurrentElements();
		if (!lpElements)
		{
			lpElements = new DSSBaseElementsProxy(lpObjectContext);
			mpControl->setCurrentElements(lpElements);
		}
		else
			lpElements->Clear();
		int i;
		for (i=0; i<mpElements->Count(); i++)
		{
			DSSBaseElementProxy* lpElement = DSSBaseElementsProxy::Clone(mpElements->Item(i), lpObjectContext);
			if (lpElement)
				lpElements->AddElement(lpElement);
		}
		mpControl->setExpression(NULL);
	}
    
    if (mpControl->getCurrentElementsStatus() == DssCurrentElementUnset)
        mpControl->setCurrentElementsStatus(DssCurrentElementChosen);
    
	// Process for different control type
	switch (lType) {
		case DssRWControlTypeElementList:
		case DssRWControlTypeMetricList:
        case DssRWControlTypeAttributeList:
		{
			//DSSMetric Condition
			if (mpExpression && mpControl->getSourceObject() && mpControl->getSourceObject()->getType()==DssTypeMetric)
            {
                if(mpControl->getObjectContext()->IsSmartClient())
                {
                    DSSExpNode* lpRoot = mpExpression->getRoot();
                    if(lpRoot && lpRoot->getType() == DssNodeOperator)
                    {
                        DSSExpNodeOperator* lpOperator = static_cast<DSSExpNodeOperator*>(lpRoot);
                        lpOperator->setFunctionID();
                    }
                }
				mpControl->setExpression(mpExpression, true);
                setLimitExpression(mpControl);//xiazhou, dong's change.
			}
            
			//Not-In support
			if (mIsNotIn && mpExpression == NULL)
			{
				// construct the expression "not in", take the function createNotInListExpression in the file "DataGridManipulationExecuter.as" as an example
				mpExpression = new DSSCExpression(lpObjectContext);
				
				//the root node is an operator
				DSSExpNodeOperator* lpOperatorNode = new DSSExpNodeOperator(lpObjectContext, "", mpExpression);
				lpOperatorNode->setExpressionType(DssFilterListQual);
				lpOperatorNode->setOperatorType(DssFunctionNotIn);
				
				//then we have a shortcut pointing to the metric
				DSSExpNodeShortcut* lpShortCutNode = static_cast<DSSExpNodeShortcut*>(lpOperatorNode->Add(DssNodeShortcut, ""));
				lpShortCutNode->setTareget(mpControl->getSourceObject());
				
				//AND THE NODE CARRYING A LIST OF THE ELEMENTS THAT COMPRISE THE FILTER
				DSSExpNodeElementsObject* lpElementsNode = static_cast<DSSExpNodeElementsObject*>(lpOperatorNode->Add(DssNodeElementsObject, ""));
				
				//THIS ITERATES THROUGH THE LIST OF ELEMENTS, AND ADDS TO THE NODE ONLY
				//THOSE THAT WERE SET AS CHECKED ON THE MENU
				//DSSBaseElementsProxy* lpElementsProxy = new DSSBaseElementsProxy(lpObjectContext);
				
				//for (int i = 0; i < mpElements->Count(); i++)
				//	lpElementsProxy->AddElement(mpElements->Item(i));
				
				lpElementsNode->setElements(mpElements);
				
				mpExpression->setRoot(lpOperatorNode);
				mpExpression->setExpresssionType(DssFilterListQual);
				mpControl->setExpression(mpExpression, true); //NotIn Case: set expression.
				mpControl->ClearCurrentElements();
			}
			
			if (mpControl->getCountTargets() > 0 || mpControl->getCountTargetControls() > 0)
			{
				lTargetCount = mpControl->getCountTargets();

                // TQMS 529238: sort the targets in the hierachical order in content tree
                std::vector<DSSRWNode*> lTargets;
                for(int i = 0; i < lTargetCount; i++)
                {
                    std::string lTargetKey = mpControl->getTargetKey(i);
                    DSSRWNode* lpTargetNode = lpObjectContext->getNode(lTargetKey);
                    lTargets.push_back(lpTargetNode);
                }
                SortControlTarget(lTargets);

                mpControl->getHighlightedNodeKeys(mNodeKeysExclusive);
				for (int i=0; i<lTargetCount; i++)
				{
					// std::string lTargetKey = mpControl->getTargetKey(i);
					// DSSRWNode* lpTargetNode = lpObjectContext->getNode(lTargetKey);
					DSSRWNode* lpTargetNode = lTargets[i];
                    if (!lpTargetNode || lpTargetNode->getType()!=DssRWNodeSection)
						continue;
					DSSRWSectionNode* lpSectionNode = (DSSRWSectionNode*)(lpTargetNode);
					lpSectionNode->setLatestControlKey(mpControl->getKey());

                    if (DSSCManipulationHelper::setTargetGroupByUnit(lpTargetNode, mpControl, mpExpression, mpElements) == S_FALSE)
                        continue;

                    //for attribute selector
                    if (lType == DssRWControlTypeAttributeList && mpElements && mpElements->Count()>0)
                    {
                        DSSAttributeListElementProxy* lSelectedElement = (DSSAttributeListElementProxy*)(mpElements->Item(0));
                        DSSAttribute* lSelectedAttribute = lSelectedElement->getAttribute();
                        if (!lSelectedElement) {
                            return S_OK;
                        }
                        std::vector<DSSRWNode*> lpLeafNodes;
                        DSSTemplateUnit* lSelectedTemplateUnit = mpControl->getTemplateUnit(lSelectedAttribute);
                        
                        lpTargetNode->getDescendantNodes(&lpLeafNodes);
                        for (int ii=0; ii<lpLeafNodes.size(); ii++) {
                            DSSRWNode* lpLeafNode = lpLeafNodes[ii];
                            if(lpLeafNode->getType() == DssRWNodeTemplate){
                                DSSTemplate* lTemplate = ((DSSRWTemplateNode*)lpLeafNode)->getTemplate();
                                //rollback to unset status
                                for (int axisIndex = DssAxisRows; axisIndex<=DssAxisColumns; axisIndex++) {
                                    DSSAxis* lAxis = lTemplate->getAxis(axisIndex);
                                    for (int unitIndex =0; unitIndex<lAxis->Count(); unitIndex++) {
                                        DSSTemplateUnit* lUnitIter = lAxis->Item(unitIndex);
                                        lTemplate->rollbackToUnsetForAttributeSelector(lUnitIter);
                                    }
                                }
                                
                                //re-apply other controls
                                ControlMap lControlMap = lpObjectContext->getControlMap();
                                ControlMap::iterator lIter = lControlMap.begin();
                                for(; lIter != lControlMap.end(); lIter++)
                                {
                                    DSSRWControl* lpRWControl= (DSSRWControl*)(lIter->second);
                                    if (!lpRWControl || lpRWControl->getKey()== mpControl->getKey() || lpRWControl->getReplacementAttributeID()== mpControl->getReplacementAttributeID() || lpRWControl->getType()!= DssRWControlTypeAttributeList) {
                                        continue;
                                    }
                                    bool lTargetThisTemplate = false;
                                    for (int targetIndex = 0; targetIndex < lpRWControl->getCountTargets(); targetIndex++) {
                                        DSSRWNode* lpTargetNodeOther = lpRWControl->getTargetNode(targetIndex);
                                        std::vector<DSSRWNode*> lpLeafNodes2;
                                        lpTargetNodeOther->getDescendantNodes(&lpLeafNodes2);
                                        for (int leafIndex = 0; leafIndex<lpLeafNodes2.size(); leafIndex++) {
                                            DSSRWNode* lpLeafNodeOther = lpLeafNodes2[leafIndex];
                                            if(lpLeafNodeOther->getKey() == lpLeafNode->getKey())
                                            {
                                                lTargetThisTemplate = true;
                                                break;
                                            }
                                        }
                                        if (lTargetThisTemplate) {
                                            break;
                                        }
                                    }
                                    
                                    if (lTargetThisTemplate) {
                                        if (lpRWControl->getCurrentElements() && lpRWControl->getCurrentElements()->Count()>0) {
                                            DSSAttributeListElementProxy* lSelectedElement2 = (DSSAttributeListElementProxy*)(lpRWControl->getCurrentElements()->Item(0));
                                            if (!lSelectedElement2) continue;
                                            DSSAttribute* lSelectedAttribute2 = lSelectedElement2->getAttribute();
                                            if (lpObjectContext->searchControlWithReplaceByID(lSelectedAttribute2->getID()) > 0) {
                                                continue;
                                            }
                                            DSSTemplateUnit* lSelectedTemplateUnit2 = lpRWControl->getTemplateUnit(lSelectedAttribute2);
                                            DSSCManipulationHelper::replaceUnitOnTemplate(lTemplate, lpRWControl->getReplacementAttributeID(), lSelectedTemplateUnit2);
                                        }
                                    }
                                }
                                
                                //check
                                if(mpElements->Count()>1)
                                    continue;
                                if (lpObjectContext->searchControlWithReplaceByID(lSelectedAttribute->getID()) > 0) {
                                    continue;
                                }
                                //apply this control
                                DSSCManipulationHelper::replaceUnitOnTemplate(lTemplate, mpControl->getReplacementAttributeID(), lSelectedTemplateUnit);
                            }
                        }
                    }
                    
					// Add the UC node's highest CGB node into the Server node map (the root nodes needing go back to server to get partial data retrieval)
					bool isUC = false;
					if (lpSectionNode->getSectionType()==DssRWSectionUnitCondition)
					{
						isUC = true;

						// get highest CGB node if UC
						DSSRWNode* lHighestCGB = lpSectionNode->getHighestCGB();
						if (lHighestCGB)
							lpTargetNode = lHighestCGB;

						mNodeMapServer[lpTargetNode->getKey()] = lpTargetNode;
						
						lHighestNGB = lpSectionNode->getHighestNGB();
						if (lHighestNGB)
						{
							std::map<std::string, DSSRWNode*>::iterator it = mNodeMapServer.find(lHighestNGB->getKey());
							if (it == mNodeMapServer.end())
								mNodeMapServer[lHighestNGB->getKey()] = lHighestNGB;
							
							//    mNodesModelChanged.push_back(lHighestNGB);
							populateSubtreeIntoNodeModelChanged(lHighestNGB, mpControlNode);
							
							bool isNGBAll = false;
							DSSGroupByUnits* lpGBUnits = ((DSSRWSectionNode*)lHighestNGB)->getGroupByUnits();
							if (lpGBUnits && lpGBUnits->Count()>0)
							{
								DSSGroupByUnit* lpGBUnit = lpGBUnits->Item(0);
								DSSBaseElementProxy* lpReplacedElement = lpGBUnit->getReplacedElement();
								if (lpGBUnit->getIsReplaced() && (!lpReplacedElement||lpReplacedElement->getElementType()==DssElementAll))
									isNGBAll = true;
								else {
									DSSBaseElementsProxy* lpElements = lpGBUnit->getCurrentElements();
									if (lpElements && lpElements->Count()>0)
									{
										EnumDSSElementType lElementType = lpElements->Item(0)->getElementType();
										isNGBAll = (lElementType==DssElementAll);
									}
									else
									{
										CComVariant* lpVariant = lpObjectContext->getDocumentDefinition()->getPropertyValue("AnalyticalEngineProperties", "AutoSelectionForUnset");
										if ((*lpVariant)!=CComVariant(1) && (*lpVariant)!=CComVariant(2))
											isNGBAll = true;
									}
								}
							}
							if (isNGBAll)
							{
								mpRedrawParentNode = ((DSSRWSectionNode*)lHighestNGB->getParentNode());
								mRedrawChildIndex = lHighestNGB->getIndex();
							}
						}
					}
					else if (lpSectionNode->getSectionType()==DssRWSectionControlGroupBySection && !lLayoutLoaded)
					{
						mNodeMapServer[lpTargetNode->getKey()] = lpTargetNode;
					}
                
					// Add the target node and all its subtree into the modelChanged map
					lpTargetNode->AddNodeMap(lNodesModelChanged, mNodeMapServer, mNodeMapSelectionUpdate, mpControlNode->getKey(), isUC, lpTargetNode->getKey());
				}
                
                int lTargetControlCount = mpControl->getCountTargetControls();
                for (int j = 0; j < lTargetControlCount; j++)
                {
                    DSSRWControl* lpTargetControl = mpControl->getTargetControl(j);
                    if(!lpTargetControl)
                        continue;
                    DSSGroupByUnits* lpUnits = lpTargetControl->getGroupbyPath();
                    
                    if (lpUnits && lpUnits->Count() > 0)
                    {
                        DSSGroupByUnit* lpUnit = NULL;
                        GUID lControlObjectID = mpControl->getSourceID();
                        for (int iUnit = 0; iUnit < lpUnits->Count(); iUnit++)
                        {
                            DSSGroupByUnit *lpTmpGBUnit = lpUnits->Item(iUnit);
                            GUID lGBUnitID = lpTmpGBUnit->getObjectID();
                            if (lControlObjectID == lGBUnitID)
                            {
                                lpUnit = lpTmpGBUnit;
                                break;
                            }
                        }
                        
                        if (!lpUnit)
                            continue;
                        lpUnit->ClearCurrentElements();
                        
                        if(mpExpression)
                        {
                            DSSCExpression* lpExpression = new DSSCExpression(lpObjectContext);
                            lpExpression->Clone(mpExpression);  //< make a copy of mpExpression.
                            lpUnit->setExpression(lpExpression);
                        }
                        else
                        {
                            for (int j=0; j<mpElements->Count(); j++)
                            {
                                DSSBaseElementProxy* lpElement = DSSBaseElementsProxy::Clone(mpElements->Item(j), lpObjectContext);
                                if (lpElement)
                                    lpUnit->AddCurrentElement(lpElement);
                            }
                        }
                        if(lpUnit->getCurrentElementsStatus() == DssCurrentElementUnset)//xiazhou, change element status for GBUnit
                            lpUnit->setCurrentElementsStatus(DssCurrentElementChosen);
                    }
                    
                    if (lpTargetControl->getFilterType() == EnumDSSRWControlFilterTypeUnitCondition)
                        mControlsModelChanged.push_back(lpTargetControl);

                    DSSRWNode* lTargetNodeDDIC = lpObjectContext->getNode(lpTargetControl->getNodeKeyDDIC());
                    if(lTargetNodeDDIC && std::find(mNodesModelChanged.begin(), mNodesModelChanged.end(), lTargetNodeDDIC) == mNodesModelChanged.end())
                        mNodesModelChanged.push_back(lTargetNodeDDIC);
                    
                    // 932851 jzeng; for sync the DDIC, I complement the API. So it just be called here at present.
                    DSSCManipulationHelper::syncSelectionByTargetControl(lpTargetControl, mpControlNode->getKey(), lTargetNodeDDIC->getKey(), mNodeMapSelectionUpdate);
                }
			}
			if(mpControl->getTargetDatasetCount() > 0)//will be called in the following
            {
				mpControl->getObjectContext()->getDocumentDefinition()->getNodesForDatasetSelector(mpControl, &mNodeMapServer, &lNodesModelChanged);
            }
		}
			break;

		case DssRWControlTypeTabList:
            
			// Set current panel
            
			if (mpElements && mpElements->Count()>0)
			{
				DSSBaseElementProxy* lpElement = mpElements->Item(0);
				if (lpElement && lpElement->getElementType()==DssElementNode)
				{
					std::string lNewPanelKey = ((DSSNodeElementProxy*)(lpElement))->getNodeKey();
					DSSRWNode* lpNewPanelNode = lpObjectContext->getNode(lNewPanelKey);
					if (lpNewPanelNode)
					{
						DSSRWSectionNode* lpPanelStackNode = (DSSRWSectionNode*)(lpNewPanelNode->getParentNode());
						if (lpPanelStackNode && lpPanelStackNode->getCurrentNodeKey()!=lNewPanelKey)
						{
							lpPanelStackNode->setCurrentNodeKey(lNewPanelKey);
							lpPanelStackNode->setCurrentNodeIndex(lpNewPanelNode->getIndex());
							mpPanelStack = lpPanelStackNode;
							
							if (!lpNewPanelNode->isNodeLoaded())
							{
								mNodeMapServer[lNewPanelKey] = lpNewPanelNode;
								mNodesModelChanged.push_back(lpNewPanelNode);
							}
						}
					}
				}
			}
			break;
		default:
			return S_OK;
	}
    
    // jzeng; 07/31/14; changing selection need refresh target in placeholderMap
    const PlaceHolderMap& lPlaceHolderMap = mpControl->getPlaceHolderMap();
    for (PlaceHolderMap::const_iterator it = lPlaceHolderMap.begin(); it != lPlaceHolderMap.end(); it++)
    {
        const std::string& lTargetNodeKey = it->first;
        DSSRWNode* lTargetNode = lpObjectContext->getNode(lTargetNodeKey);
        DSSCManipulationHelper::setTargetGroupByUnit(lTargetNode, mpControl, mpExpression, mpElements, DSSCManipulationHelper::DssPlaceHolderTarget);
        setHasPlaceHolderTargets(true);
    }
    DSSCManipulationHelper::addPlaceHolderMapToNodeMapServer(mpControl, mNodeMapServer, mNodesModelChanged, mNodeMapSelectionUpdate);
    
    // jzeng; 09/01/14; update the target filter when current selection changed
    DSSCManipulationHelper::updateSelectionOfAffectedControl(mpControl, mNodeMapServer, mControlsModelChanged, mNodesModelChanged, mNodeMapSelectionUpdate);
    
	if (!mIsFromActionObject)
	{
		if (mpExpression) //DSSMetric Condition or NotIn.
		{
			mpActionObject = new DSSAODeltaXML(ManipulationSetSelectionFromControl);
			DSSAODeltaXML* lpAODeltaXML = new DSSAODeltaXML(ManipulationSetSelectionFromControl);
			std::string lXML = hBuildExpressionXML();
			printf("xml:%s\n", lXML.c_str());
			(static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lXML);
			lpAODeltaXML->setXMLString(lXML);
			(static_cast<DSSAODeltaXML*>(mpActionObject))->setIsNotIn(mIsNotIn);	//2011-6-2 lcao TQMS 478489
			lpAODeltaXML->setIsNotIn(mIsNotIn);
			mActionObjects.push_back(lpAODeltaXML);
		}
		else
		{
			mpActionObject = new DSSAOSelection(ManipulationSetSelectionFromControl);
			DSSAOSelection* lpActionObject = static_cast<DSSAOSelection*> (mpActionObject);
			lpActionObject->setNodeKey(mpControlNode->getKey());
			lpActionObject->setControlKey(mpControl->getKey());
			lpActionObject->setTreeType(mpControlNode->getTreeType());
			
			for (int i=0; i<mpElements->Count(); i++)
			{
				DSSBaseElementProxy* lpElement = mpElements->Item(i);
				if (lpElement->getElementType()==DssElementConcrete && lpElement->getUnitType()==DssTemplateAttribute)
					lpActionObject->addElementID(((DSSAttributeElementProxy*)lpElement)->getCompactElementIDwithDisplayText());
				else
					lpActionObject->addElementID(lpElement->getElementID());
			}
			NodeMap::iterator it;
			for ( it=mNodeMapServer.begin() ; it != mNodeMapServer.end(); it++ )
				lpActionObject->addServerKey((*it).first);
		}
	}
    //DE38064, msun, control could have RWNode and dataset as target at the same time.
	if(mpControl->getCountTargets() > 0 || mpControl->getCountTargetControls() > 0)
    {
        // put all control nodes to the end of the vector
        std::vector<DSSRWNode*> lNodesModelChangedControl;
        // put all template nodes to the end of the vector
        std::vector<DSSRWNode*> lNodesModelChangedTemplate;
        std::vector<DSSRWNode*>::iterator it1;
        for ( it1=lNodesModelChanged.begin() ; it1 != lNodesModelChanged.end(); it1++ )
        {
            DSSRWNode* lpNode = (*it1);
            if (lHighestNGB && lpNode->getHighestNGB()==lHighestNGB) //we have already pushed lpNode previously
                continue;
            
            if((*it1)->getType() == DssRWNodeControl)
                lNodesModelChangedControl.push_back(*it1);
            else if((*it1)->getType() == DssRWNodeTemplate)
                lNodesModelChangedTemplate.push_back(*it1);
            else
                mNodesModelChanged.push_back(*it1);
        }
        
        if (mIsFromActionObject)
            mNodesModelChanged.push_back(mpControlNode);
        
        std::vector<DSSRWNode*>::iterator it2 =lNodesModelChangedTemplate.begin();
        std::vector<DSSRWNode*>::iterator it3 =lNodesModelChangedTemplate.end();
        for (; it2 != it3; it2++ )
        {
            mNodesModelChanged.push_back(*it2);
        }
        
        it2 =lNodesModelChangedControl.begin();
        it3 =lNodesModelChangedControl.end();
        for (; it2 != it3; it2++ )
        {
            mNodesModelChanged.push_back(*it2);
        }
    
#ifndef	__ANDROID__
        // add metric condition selector nodes to mNodesModelChanged
        if (mpControl->getType()==DssRWControlTypeElementList && mpControl->getSourceObject() && mpControl->getSourceObject()->getType()!=DssTypeMetric && mpControl->getCountTargets()>0)
        {
            DSSRWNode* lpParentNode = mpControlNode->getParentNode();
            DSSRWNode* lpNodeToExplore = hGetLowestNonGroupBySectionNode(lpParentNode);
            if (lpNodeToExplore)
                lpParentNode = lpNodeToExplore;
            AddMetricConditionSelectorsToMap(lpParentNode, mpControl->getKey());
        }
#endif
    }
    if (mpControl->getTargetDatasetCount() > 0)
    {
        // UC on dataset
        mpControl->getObjectContext()->getDocumentDefinition()->getNodesForDatasetSelector(mpControl, &mNodeMapServer, &mNodesModelChanged, &mDDICModelChanged);
        // we only save DDIC with matched dataset but different from the datasource of the template
        for(std::vector<DSSRWNode*>::iterator lIter1 = mNodesModelChanged.begin(); lIter1 != mNodesModelChanged.end(); lIter1++)
        {
            std::string lTemplateNodeKey = (*lIter1)->getKey();
            std::map<std::string, std::vector<std::string> >::iterator lIter2 = mDDICModelChanged.find(lTemplateNodeKey);
            if(lIter2 != mDDICModelChanged.end())
                mDDICModelChanged.erase(lIter2);
        }
        mRetrievalGlobalLookupTableHeader = true;
        DSSRWNode* lpLayoutNode = mpControlNode->getLayout();
        if(lpLayoutNode->IsAboveNGBAll())
        {
            mpRedrawParentNode = static_cast<DSSRWSectionNode*>(lpLayoutNode->getParentNode());
            mRedrawChildIndex = lpLayoutNode->getIndex();
        }
        //Reset all layouts other than current layout's loaded flag
//        DSSDocumentDefinition* lpDefinition = lpObjectContext->getDocumentDefinition();
//        if(lpDefinition){
//            std::vector<std::string>* allLayouts = lpDefinition->getLayoutKeys();
//            std::string currentLayout = lpDefinition->getCurrentLayoutKey();
//            for(std::vector<std::string>::iterator it = allLayouts->begin();it<allLayouts->end();it++){
//                if((*it)!=currentLayout){
//                    DSSRWNode* lpNode = lpObjectContext->getNode(*it);
//                    if (lpNode)
//                        lpNode->setLayoutLoaded(false);
//                }
//            }
//        }
        mNeedResetAllLayouts = true;
    }
    
    return S_OK;
}

void DSSCManipulationSetSelectionFromControl::resetAllLayouts()
{
    //Reset all layouts other than current layout's loaded flag
	DSSObjectContext* lpObjectContext = mpControlNode->getObjectContext();
    
    DSSDocumentDefinition* lpDefinition = lpObjectContext->getDocumentDefinition();
    if(lpDefinition){
        std::vector<std::string>* allLayouts = lpDefinition->getLayoutKeys();
        std::string currentLayout = lpDefinition->getCurrentLayoutKey();
        for(std::vector<std::string>::iterator it = allLayouts->begin();it<allLayouts->end();it++){
            if((*it)!=currentLayout){
                DSSRWNode* lpNode = lpObjectContext->getNode(*it);
                if (lpNode)
                    lpNode->setLayoutLoaded(false);
            }
        }
    }
}

#ifdef __ANDROID__
std::string DSSCManipulationSetSelectionFromControl::hBuildExpressionXML()
{
	TiXmlDocument lDoc;
	TiXmlElement* lpRootElement = new TiXmlElement("rw_manipulation");
    
	TiXmlElement* lpManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
	TiXmlText* lpManipulationMethodText = new TiXmlText("99");
	lpManipulationMethodElement->LinkEndChild(lpManipulationMethodText);
	lpRootElement->LinkEndChild(lpManipulationMethodElement);
    
	TiXmlElement* lpTreeTypeElement = new TiXmlElement("rw_tree_type");
	TiXmlText* lpTreeTypeText = new TiXmlText("1");
	lpTreeTypeElement->LinkEndChild(lpTreeTypeText);
	lpRootElement->LinkEndChild(lpTreeTypeElement);
    
	std::string lstrNodeKey = mpControlNode->getKey();
	TiXmlElement* lpNodeKeyElement = new TiXmlElement("rw_node_key");
	TiXmlText* lpNodeKeyText = new TiXmlText(lstrNodeKey);
	lpNodeKeyElement->LinkEndChild(lpNodeKeyText);
	lpRootElement->LinkEndChild(lpNodeKeyElement);
    
	lstrNodeKey = mpControl->getKey();
	TiXmlElement* lpControlKeyElement = new TiXmlElement("rw_control_key");
	TiXmlText* lpControlKeyText = new TiXmlText(lstrNodeKey);
	lpControlKeyElement->LinkEndChild(lpControlKeyText);
	lpRootElement->LinkEndChild(lpControlKeyElement);
    
	mpExpression->getExpressionXML(lpRootElement);
	lDoc.LinkEndChild(lpRootElement);
    
	TiXmlPrinter printer;
	printer.SetIndent(NULL);
	printer.SetLineBreak(NULL);
    
	lDoc.Accept( &printer );
	return printer.Str();
}


void DSSCManipulationSetSelectionFromControl::hParseExpressionFromXML(std::string& iXMLString, DSSObjectContext* ipContext)
{
	TiXmlDocument doc;
	doc.Parse(iXMLString.c_str());
	if (doc.Error())
		return;
    
	const TiXmlElement* lpCurrentNode = doc.RootElement();
	if (!lpCurrentNode || lpCurrentNode->ValueStr() != "rw_manipulation")
		return;
    
	const TiXmlElement* lpMiNode = NULL;
	std::string lstrContent;
    
	for (lpCurrentNode = lpCurrentNode->FirstChildElement(); lpCurrentNode != NULL; lpCurrentNode = lpCurrentNode->NextSiblingElement())
	{
		if (lpCurrentNode->ValueStr() == "rw_node_key")
		{
			// Found the control node key
			const char* lpstrText = lpCurrentNode->GetText();
			if (lpstrText)
				lstrContent = lpstrText;
			mpControlNode = (DSSRWControlNode*)ipContext->getNode(lstrContent);
		}
		else if (lpCurrentNode->ValueStr() == "rw_control_key")
		{
			// Found the control key
			const char* lpstrText = lpCurrentNode->GetText();
			if (lpstrText)
				lstrContent = lpstrText;
			mpControl = (DSSRWControl*)ipContext->getControl(lstrContent);
		}
		else if (lpCurrentNode->ValueStr() == "mi")
		{
			// Found the expression
			lpMiNode = lpCurrentNode;
		}
	}
    
	if (!lpMiNode)
		return;
    
	// Parse the mi xml to get root node of the expression
	mpExpression = new DSSCExpression(ipContext);
	const TiXmlElement* lpRootNode = NULL;
	for (lpCurrentNode = lpMiNode->FirstChildElement(); lpCurrentNode != NULL; lpCurrentNode = lpCurrentNode->NextSiblingElement())
	{
		if (lpCurrentNode->ValueStr() == "exp")
		{
			// Found the exp node
			for (lpCurrentNode = lpCurrentNode->FirstChildElement(); lpCurrentNode != NULL; lpCurrentNode = lpCurrentNode->NextSiblingElement())
			{
				if (lpCurrentNode->ValueStr() == "nd")
				{
					// Found the root node
					lpRootNode = lpCurrentNode;
					break;
				}
			}
			break;
		}
	}
    
	if (!lpRootNode)
		return;
	
	DSSExpNode* lpExpNode = hBuildExpNode(lpRootNode, NULL, ipContext);
	mpExpression->setRoot(lpExpNode);
	if (lpExpNode)
		mpExpression->setExpresssionType(lpExpNode->getExpressionType());
}

DSSExpNode* DSSCManipulationSetSelectionFromControl::hBuildExpNode(const TiXmlElement* ipXMLNode, DSSExpNode* ipParentExpNode, DSSObjectContext* ipContext)
{
	int et, nt;
	if (ipXMLNode->QueryIntAttribute("et", &et) != TIXML_SUCCESS || ipXMLNode->QueryIntAttribute("nt", &nt) != TIXML_SUCCESS)
		return NULL;
	switch (nt)
	{
		case DssNodeElementsObject:
		{
			DSSExpNodeElementsObject* lpElementsNode;
			if (ipParentExpNode)
				lpElementsNode = static_cast<DSSExpNodeElementsObject*>(ipParentExpNode->Add(DssNodeElementsObject, ""));
			else
				lpElementsNode = new DSSExpNodeElementsObject(ipContext, "", mpExpression);
			
			lpElementsNode->setExpressionType((EnumDSSExpressionType)et);
			
			DSSBaseElementsProxy* lpElements = new DSSBaseElementsProxy(ipContext);
			int type;
			
			for (const TiXmlElement* lpChildNode = ipXMLNode->FirstChildElement(); lpChildNode != NULL; lpChildNode = lpChildNode->NextSiblingElement())
			{
				if (lpChildNode->ValueStr() == "mi")
				{
					// found mi node
					for (lpChildNode = lpChildNode->FirstChildElement(); lpChildNode != NULL; lpChildNode = lpChildNode->NextSiblingElement())
					{
						if (lpChildNode->ValueStr() == "in")
						{
							// found in node
							for (const TiXmlElement* lpOiNode = lpChildNode->FirstChildElement(); lpOiNode != NULL; lpOiNode = lpOiNode->NextSiblingElement())
							{
								if (lpOiNode->ValueStr() == "oi")
								{
									// found oi node
									lpOiNode->QueryIntAttribute("tp", &type);
									break;
								}
							}
						}
						else if (lpChildNode->ValueStr() == "es")
						{
							// found es node
							for (lpChildNode = lpChildNode->FirstChildElement(); lpChildNode != NULL; lpChildNode = lpChildNode->NextSiblingElement())
							{
								if (lpChildNode->ValueStr() == "e")
								{
									// found e node
									const char* lpstrText = lpChildNode->Attribute("ei");
									std::string lstrProp;
									if (lpstrText)
										lstrProp = lpstrText;
									DSSBaseElementProxy* lpElement = NULL;
									switch (type) {
										case DssTypeAttribute:
											lpElement = new DSSAttributeElementProxy(ipContext);
											((DSSAttributeElementProxy*)(lpElement))->Parse(lstrProp);
											break;
										default:
											// TODO check other cases
											break;
									}
									
									if (lpElement)
										lpElements->AddElement(lpElement);;
								}
							}
							break;
						}
					}
					break;
				}
			}
			lpElementsNode->setElements(lpElements);
			return lpElementsNode;
		}
			break;
		case DssNodeConstant:
		{
			DSSExpNodeConstant* lpConstantNode;
			if (ipParentExpNode)
				lpConstantNode = static_cast<DSSExpNodeConstant*>(ipParentExpNode->Add(DssNodeConstant, ""));
			else
				lpConstantNode = new DSSExpNodeConstant(ipContext, DssNodeConstant, "", mpExpression);
			
			lpConstantNode->setExpressionType((EnumDSSExpressionType)et);
            
			for (const TiXmlElement* lpChildNode = ipXMLNode->FirstChildElement(); lpChildNode != NULL; lpChildNode = lpChildNode->NextSiblingElement())
			{
				if (lpChildNode->ValueStr() == "cst")
				{
					// constant's data type and value
					int ddt = 0;
					lpChildNode->QueryIntAttribute("ddt", &ddt);
					lpConstantNode->setDataType(ddt);
					CComVariant value;
					const char* lpstrText = lpChildNode->GetText();
					std::string lstrProp;
					if (lpstrText)
						lstrProp = lpstrText;
					DSSExpNode::StringToVariant(lstrProp, ddt, value);
					lpConstantNode->setValue(value);
				}
			}
			return lpConstantNode;
		}
			break;
		case DssNodeTime:
		{
			DSSExpNodeConstant* lpTimeNode;
			if (ipParentExpNode)
				lpTimeNode = static_cast<DSSExpNodeConstant*>(ipParentExpNode->Add(DssNodeTime, ""));
			else
				lpTimeNode = new DSSExpNodeConstant(ipContext, DssNodeTime, "", mpExpression);
			lpTimeNode->setExpressionType((EnumDSSExpressionType)et);
			
			for (const TiXmlElement* lpChildNode = ipXMLNode->FirstChildElement(); lpChildNode != NULL; lpChildNode = lpChildNode->NextSiblingElement())
			{
				if (lpChildNode->ValueStr() == "cst")
				{
					// constant's data type and value
					int ddt = 0;
					lpChildNode->QueryIntAttribute("ddt", &ddt);
					lpTimeNode->setDataType(ddt);
					CComVariant value;
					const char* lpstrText = lpChildNode->GetText();
					std::string lstrProp;
					if (lpstrText)
						lstrProp = lpstrText;
					DSSExpNode::StringToVariant(lstrProp, ddt, value);
					lpTimeNode->setValue(value);
				}
			}
			return lpTimeNode;
		}
			break;
		case DssNodeOperator:
		{
			DSSExpNodeOperator* lpOperatorNode;
			if (ipParentExpNode)
				lpOperatorNode = static_cast<DSSExpNodeOperator*>(ipParentExpNode->Add(DssNodeOperator, ""));
			else
				lpOperatorNode = new DSSExpNodeOperator(ipContext, "", mpExpression);
			
			lpOperatorNode->setExpressionType((EnumDSSExpressionType)et);
            
			for (const TiXmlElement* lpChildNode = ipXMLNode->FirstChildElement(); lpChildNode != NULL; lpChildNode = lpChildNode->NextSiblingElement())
			{
				if (lpChildNode->ValueStr() == "nd")
				{
					// Found a child exp node
					hBuildExpNode(lpChildNode, lpOperatorNode, ipContext);
				}
				else if (lpChildNode->ValueStr() == "op")
				{
					// operator's function type
					int fnt = 0;
					lpChildNode->QueryIntAttribute("fnt", &fnt);
					lpOperatorNode->setOperatorType(fnt);
				}
			}
			return lpOperatorNode;
		}
			break;
		case DssNodeShortcut:
		{
			DSSExpNodeShortcut* lpShortCutNode;
			if (ipParentExpNode)
				lpShortCutNode = static_cast<DSSExpNodeShortcut*>(ipParentExpNode->Add(DssNodeShortcut, ""));
			else
				lpShortCutNode = new DSSExpNodeShortcut(ipContext, "", mpExpression);
			
			lpShortCutNode->setExpressionType((EnumDSSExpressionType)et);
			/*for (xmlNode* lpChildNode = ipXMLNode->children; lpChildNode; lpChildNode = lpChildNode->next)
			 {
			 if (lpChildNode->type == XML_ELEMENT_NODE)
			 {
			 if (strcmp(lpChildNode->name, "at") == 0)
			 {
			 lstrProp = xmlGetProp(ipXMLNode, "rfd");
			 int rfd = DSSExpNode::StringToInt(lstrProp);
			 }
			 }
			 }*/
			if (mpControl)
				lpShortCutNode->setTareget(mpControl->getSourceObject());
			
			return lpShortCutNode;
		}
			break;
		default:
		{
			DSSExpNode* lpNode;
			if (ipParentExpNode)
				lpNode = ipParentExpNode->Add(et, "");
			else
				lpNode = new DSSExpNode(ipContext, DssNodeReserved, "", mpExpression);
			lpNode->setExpressionType((EnumDSSExpressionType)et);
			return lpNode;
		}
			break;
	}
	return NULL;
}

#else
std::string DSSCManipulationSetSelectionFromControl::hBuildExpressionXML()
{
	SimpleXMLBuilder lBuilder;
	SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
	SimpleXMLNode lManipulationXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
	SimpleXMLNode lManipulationXMLContent(SimpleXMLBuilder::CreateNodePtr("99", true));
	lManipulationXML.AddChild(lManipulationXMLContent);
	lXML.AddChild(lManipulationXML);
    
	SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
	SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr("1", true));
	lTreeTypeXML.AddChild(lTreeTypeXMLContent);
	lXML.AddChild(lTreeTypeXML);
	
	std::string lStr = mpControlNode->getKey();
	SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
	SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lStr.c_str(), true));
	lNodeKeyXML.AddChild(lNodeKeyXMLContent);
	lXML.AddChild(lNodeKeyXML);
	
	lStr = mpControl->getKey();
	SimpleXMLNode lControlKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_control_key"));
    SimpleXMLNode lControlKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lStr.c_str(), true));
	lControlKeyXML.AddChild(lControlKeyXMLContent);
	lXML.AddChild(lControlKeyXML);
    
    mpExpression->getExpressionXML(lXML.GetNode());
	SimpleXMLNode lRoot(lBuilder.GetRoot());
	lRoot.AddChild(lXML);
	
	return lBuilder.ToString(false);
}

void DSSCManipulationSetSelectionFromControl::hParseExpressionFromXML(std::string& iXMLString, DSSObjectContext* ipContext)
{
	SimpleXMLParser lParser(iXMLString);
	if (!lParser.IsValid())
		return;
	
	SimpleXMLNode lCurrentNode = lParser.GetRoot();
	
	if (!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
		return;
	
	SimpleXMLNode lMiNode;
	std::string lstrContent;
	for (lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
	{
		if (lCurrentNode.IsElementNode())
		{
			if (lCurrentNode.GetName().compare("rw_node_key") == 0)
			{
				// Found the control node key
				lCurrentNode.GetContent(lstrContent);
				mpControlNode = (DSSRWControlNode*)ipContext->getNode(lstrContent);
			}
			else if (lCurrentNode.GetName().compare("rw_control_key") == 0)
			{
				// Found the control key
				lCurrentNode.GetContent(lstrContent);
				mpControl = (DSSRWControl*)ipContext->getControl(lstrContent);
			}
			else if (lCurrentNode.GetName().compare("mi") == 0)
			{
				// Found the expression
				lMiNode = lCurrentNode;
			}
		}
	}
	
	if (!lMiNode.IsValid())
		return;
	
	// Parse the mi xml to get root node of the expression
	mpExpression = new DSSCExpression(ipContext);
	SimpleXMLNode lRootNode = NULL;
	for (lCurrentNode = lMiNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
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
	
	DSSExpNode* lpExpNode = hBuildExpNode(lRootNode, NULL, ipContext);
	mpExpression->setRoot(lpExpNode);
	mpExpression->setExpresssionType(lpExpNode->getExpressionType());
}

DSSExpNode* DSSCManipulationSetSelectionFromControl::hBuildExpNode(SimpleXMLNode& iXMLNode, DSSExpNode* ipParentExpNode, DSSObjectContext* ipContext)
{
	std::string lstrProp;
	iXMLNode.GetProperty("et", lstrProp);
	int et = DSSExpNode::StringToInt(lstrProp);
	iXMLNode.GetProperty("nt", lstrProp);
	int nt = DSSExpNode::StringToInt(lstrProp);
	switch (nt)
	{
		case DssNodeElementsObject:
		{
			DSSExpNodeElementsObject* lpElementsNode;
			if (ipParentExpNode)
				lpElementsNode = static_cast<DSSExpNodeElementsObject*>(ipParentExpNode->Add(DssNodeElementsObject, ""));
			else
				lpElementsNode = new DSSExpNodeElementsObject(ipContext, "", mpExpression);
			
			lpElementsNode->setExpressionType((EnumDSSExpressionType)et);
			
			DSSBaseElementsProxy* lpElements = new DSSBaseElementsProxy(ipContext);
			int type;
			
			for (SimpleXMLNode lChildNode = iXMLNode.GetFirstChild(); lChildNode.IsValid(); lChildNode = lChildNode.GetNextSibling())
			{
				if (lChildNode.IsElementNode() && lChildNode.GetName().compare("mi") == 0)
				{
					// found mi node
					for (lChildNode = lChildNode.GetFirstChild(); lChildNode.IsValid(); lChildNode = lChildNode.GetNextSibling())
					{
						if (lChildNode.IsElementNode() && lChildNode.GetName().compare("in") == 0)
						{
							// found in node
							for (SimpleXMLNode lOiNode = lChildNode.GetFirstChild(); lOiNode.IsValid(); lOiNode = lOiNode.GetNextSibling())
							{
								if (lOiNode.IsElementNode() && lOiNode.GetName().compare("oi") == 0)
								{
									// found oi node
									lOiNode.GetProperty("tp", lstrProp);
									type = DSSExpNode::StringToInt(lstrProp);
									break;
								}
							}
						}
						else if (lChildNode.IsElementNode() && lChildNode.GetName().compare("es") == 0)
						{
							// found es node
							for (lChildNode = lChildNode.GetFirstChild(); lChildNode.IsValid(); lChildNode = lChildNode.GetNextSibling())
							{
								if (lChildNode.IsElementNode() && lChildNode.GetName().compare("e") == 0)
								{
									// found e node
									lChildNode.GetProperty("ei", lstrProp);
									DSSBaseElementProxy* lpElement = NULL;
									switch (type) {
										case DssTypeAttribute:
											lpElement = new DSSAttributeElementProxy(ipContext);
											((DSSAttributeElementProxy*)(lpElement))->Parse(lstrProp);
											break;
										default:
											// TODO check other cases
											break;
									}
									
									if (lpElement)
										lpElements->AddElement(lpElement);;
								}
							}
							break;
						}
					}
					break;
				}
			}
			lpElementsNode->setElements(lpElements);
			return lpElementsNode;
		}
			break;
		case DssNodeConstant:
		{
			DSSExpNodeConstant* lpConstantNode;
			if (ipParentExpNode)
				lpConstantNode = static_cast<DSSExpNodeConstant*>(ipParentExpNode->Add(DssNodeConstant, ""));
			else
				lpConstantNode = new DSSExpNodeConstant(ipContext, DssNodeConstant, "", mpExpression);
			
			lpConstantNode->setExpressionType((EnumDSSExpressionType)et);
			
			for (SimpleXMLNode lChildNode = iXMLNode.GetFirstChild(); lChildNode.IsValid(); lChildNode = lChildNode.GetNextSibling())
			{
				if (lChildNode.IsElementNode())
				{
					if (lChildNode.GetName().compare("cst") == 0)
					{
						// constant's data type and value
						lChildNode.GetProperty("ddt", lstrProp);
						int ddt = DSSExpNode::StringToInt(lstrProp);
						lpConstantNode->setDataType(ddt);
						CComVariant value;
						lChildNode.GetContent(lstrProp);
						DSSExpNode::StringToVariant(lstrProp, ddt, value);
						lpConstantNode->setValue(value);
					}
				}
			}
			return lpConstantNode;
		}
			break;
		case DssNodeTime:
		{
			//DSSExpNodeConstant* lpTimeNode;
            DSSExpNodeTime* lpTimeNode;
			if (ipParentExpNode)
				lpTimeNode = static_cast<DSSExpNodeTime*>(ipParentExpNode->Add(DssNodeTime, ""));
			else
				lpTimeNode = new DSSExpNodeTime(ipContext, DssNodeTime, "", mpExpression);
			lpTimeNode->setExpressionType((EnumDSSExpressionType)et);
			
			for (SimpleXMLNode lChildNode = iXMLNode.GetFirstChild(); lChildNode.IsValid(); lChildNode = lChildNode.GetNextSibling())
			{
				if (lChildNode.IsElementNode())
				{
					if (lChildNode.GetName().compare("cst") == 0)
					{
						// constant's data type and value
						lChildNode.GetProperty("ddt", lstrProp);
						int ddt = DSSExpNode::StringToInt(lstrProp);
						lpTimeNode->setDataType(ddt);
						CComVariant value;
						lChildNode.GetContent(lstrProp);
						DSSExpNode::StringToVariant(lstrProp, ddt, value);
						lpTimeNode->setFormattedValue(value);
					}
				}
			}
			return lpTimeNode;
		}
			break;
		case DssNodeOperator:
		{
			DSSExpNodeOperator* lpOperatorNode;
			if (ipParentExpNode)
				lpOperatorNode = static_cast<DSSExpNodeOperator*>(ipParentExpNode->Add(DssNodeOperator, ""));
			else
				lpOperatorNode = new DSSExpNodeOperator(ipContext, "", mpExpression);
			
			lpOperatorNode->setExpressionType((EnumDSSExpressionType)et);
			
			for (SimpleXMLNode lChildNode = iXMLNode.GetFirstChild(); lChildNode.IsValid(); lChildNode = lChildNode.GetNextSibling())
			{
				if (lChildNode.IsElementNode())
				{
					if (lChildNode.GetName().compare("nd") == 0)
					{
						// Found a child exp node
						hBuildExpNode(lChildNode, lpOperatorNode, ipContext);
					}
					else if (lChildNode.GetName().compare("op") == 0)
					{
						// operator's function type
						lChildNode.GetProperty("fnt", lstrProp);
						int fnt = DSSExpNode::StringToInt(lstrProp);
						lpOperatorNode->setOperatorType(fnt);
					}
                    else if (lChildNode.GetName().compare("prs") == 0)
                    {
                        for (SimpleXMLNode lPrpNode=lChildNode.GetFirstChild(); lPrpNode.IsValid(); lPrpNode=lPrpNode.GetNextSibling())
                        {
                            if (lPrpNode.IsElementNode())
                            {
                                if (lPrpNode.GetName().compare("pr") == 0)
                                {
                                    std::string lstrPrpValue;
                                    lPrpNode.GetProperty("ix", lstrProp);
                                    lPrpNode.GetProperty("v", lstrPrpValue);
                                    if (lstrProp=="4")  //mIsAscending
                                    {
                                        lstrPrpValue=="-1" ? lpOperatorNode->setAscending(true) : lpOperatorNode->setAscending(false);
                                    }
                                    else if (lstrProp == "5")   //mIsByValue
                                    {
                                        lstrPrpValue=="-1" ? lpOperatorNode->setByValue(true) : lpOperatorNode->setByValue(false);
                                    }
                                }
                            }
                        }
                    }
				}
			}
			return lpOperatorNode;
		}
			break;
		case DssNodeShortcut:
		{
			DSSExpNodeShortcut* lpShortCutNode;
			if (ipParentExpNode)
				lpShortCutNode = static_cast<DSSExpNodeShortcut*>(ipParentExpNode->Add(DssNodeShortcut, ""));
			else
				lpShortCutNode = new DSSExpNodeShortcut(ipContext, "", mpExpression);
			
			lpShortCutNode->setExpressionType((EnumDSSExpressionType)et);
			/*for (xmlNode* lpChildNode = ipXMLNode->children; lpChildNode; lpChildNode = lpChildNode->next)
			 {
			 if (lpChildNode->type == XML_ELEMENT_NODE)
			 {
			 if (strcmp(lpChildNode->name, "at") == 0)
			 {
			 lstrProp = xmlGetProp(ipXMLNode, "rfd");
			 int rfd = DSSExpNode::StringToInt(lstrProp);
			 }
			 }
			 }*/
			if (mpControl)
				lpShortCutNode->setTareget(mpControl->getSourceObject());
			
			return lpShortCutNode;
		}
			break;
		default:
		{
			DSSExpNode* lpNode;
			if (ipParentExpNode)
				lpNode = ipParentExpNode->Add(et, "");
			else
				lpNode = new DSSExpNode(ipContext, DssNodeReserved, "", mpExpression);
			lpNode->setExpressionType((EnumDSSExpressionType)et);
			return lpNode;
		}
			break;
	}
	return NULL;
}
#endif

DSSBaseElementsProxy* DSSCManipulationSetSelectionFromControl::getElements()
{
	return mpElements;
}

DSSRWNode* DSSCManipulationSetSelectionFromControl::getSelectionNode()
{
	return mpControlNode;
}

void DSSCManipulationSetSelectionFromControl::setLimitExpression(DSSRWControl *ipControl)
{
    if(!ipControl)
        return;
    
    DSSCExpression* lpExpression = ipControl->getExpression();
    if(!lpExpression)
        return;
    
    GUID lSourceID = ipControl->getSourceID();
    for (int i = 0; i < ipControl->getCountTargets(); i++)
    {
        DSSRWTemplateNode* lpTargetNode = dynamic_cast<DSSRWTemplateNode*>(ipControl->getTargetNode(i));
        if(lpTargetNode)
        {
            DSSTemplate* lpTemplate = lpTargetNode->getTemplate();
            DSSAxis* lpAxis = lpTemplate->getRow();
            for(int j = 0; j < lpAxis->Count(); j++)
            {
                DSSTemplateMetrics* lpMetrics = dynamic_cast<DSSTemplateMetrics*>(lpAxis->Item(j));
                if(lpMetrics)
                {
                    for(int k = 0; k < lpMetrics->Count(); k++)
                    {
                        DSSTemplateMetric* lpMetric = lpMetrics->Item(k);
                        if(lpMetric->getMetric()->getID() == lSourceID)
                        {
                            DSSCExpression* lpLimitExpression = new DSSCExpression(ipControl->getObjectContext());
                            lpLimitExpression->Duplicate(lpExpression);
                        }
                    }
                }
            }
            lpAxis = lpTemplate->getColumn();
            for(int j = 0; j < lpAxis->Count(); j++)
            {
                DSSTemplateMetrics* lpMetrics = dynamic_cast<DSSTemplateMetrics*>(lpAxis->Item(j));
                if(lpMetrics)
                {
                    for(int k = 0; k < lpMetrics->Count(); k++)
                    {
                        DSSTemplateMetric* lpMetric = lpMetrics->Item(k);
                        if(lpMetric->getMetric()->getID() == lSourceID)
                        {
                            DSSCExpression* lpLimitExpression = new DSSCExpression(ipControl->getObjectContext());
                            lpLimitExpression->Duplicate(lpExpression);
                        }
                    }
                }
            }
            DSSTemplateMetric* lpMetric = lpTemplate->FindDisabledMetric(ipControl->getObjectContext()->FindObjectH(lSourceID));
            if(lpMetric)
            {
                DSSCExpression* lpLimitExpression = new DSSCExpression(ipControl->getObjectContext());
                lpLimitExpression->Duplicate(lpExpression);
            }
        }
    }
}

#ifndef __ANDROID__
DSSExpNode* DSSCManipulationSetSelectionFromControl::hBuildExpNode(DSSCExpression *&ipExpression, SimpleXMLNode &iXMLNode, DSSExpNode *ipParentExpNode, DSSObjectContext *ipContext)
{
    std::string lstrProp;
	iXMLNode.GetProperty("et", lstrProp);
	int et = DSSExpNode::StringToInt(lstrProp);
	iXMLNode.GetProperty("nt", lstrProp);
	int nt = DSSExpNode::StringToInt(lstrProp);
	switch (nt)
	{
		case DssNodeElementsObject:
		{
			DSSExpNodeElementsObject* lpElementsNode;
			if (ipParentExpNode)
				lpElementsNode = static_cast<DSSExpNodeElementsObject*>(ipParentExpNode->Add(DssNodeElementsObject, ""));
			else
				lpElementsNode = new DSSExpNodeElementsObject(ipContext, "", ipExpression);
			
			lpElementsNode->setExpressionType((EnumDSSExpressionType)et);
			
			DSSBaseElementsProxy* lpElements = new DSSBaseElementsProxy(ipContext);
			int type;
			
			for (SimpleXMLNode lChildNode = iXMLNode.GetFirstChild(); lChildNode.IsValid(); lChildNode = lChildNode.GetNextSibling())
			{
				if (lChildNode.IsElementNode() && lChildNode.GetName().compare("mi") == 0)
				{
					// found mi node
					for (lChildNode = lChildNode.GetFirstChild(); lChildNode.IsValid(); lChildNode = lChildNode.GetNextSibling())
					{
						if (lChildNode.IsElementNode() && lChildNode.GetName().compare("in") == 0)
						{
							// found in node
							for (SimpleXMLNode lOiNode = lChildNode.GetFirstChild(); lOiNode.IsValid(); lOiNode = lOiNode.GetNextSibling())
							{
								if (lOiNode.IsElementNode() && lOiNode.GetName().compare("oi") == 0)
								{
									// found oi node
									lOiNode.GetProperty("tp", lstrProp);
									type = DSSExpNode::StringToInt(lstrProp);
									break;
								}
							}
						}								 
						else if (lChildNode.IsElementNode() && lChildNode.GetName().compare("es") == 0)
						{
							// found es node
							for (lChildNode = lChildNode.GetFirstChild(); lChildNode.IsValid(); lChildNode = lChildNode.GetNextSibling())
							{
								if (lChildNode.IsElementNode() && lChildNode.GetName().compare("e") == 0)
								{
									// found e node
									lChildNode.GetProperty("ei", lstrProp);
									DSSBaseElementProxy* lpElement = NULL;
									switch (type) {
										case DssTypeAttribute:
											lpElement = new DSSAttributeElementProxy(ipContext);
											((DSSAttributeElementProxy*)(lpElement))->Parse(lstrProp);
											break;
										default:
											// TODO check other cases
											break;
									}
									
									if (lpElement)
										lpElements->AddElement(lpElement);;
								}
							}
							break;
						}
					}
					break;
				}
			}
			lpElementsNode->setElements(lpElements);
			return lpElementsNode;
		}
			break;
		case DssNodeConstant:
		{
			DSSExpNodeConstant* lpConstantNode;
			if (ipParentExpNode)
				lpConstantNode = static_cast<DSSExpNodeConstant*>(ipParentExpNode->Add(DssNodeConstant, ""));
			else
				lpConstantNode = new DSSExpNodeConstant(ipContext, DssNodeConstant, "", ipExpression);
			
			lpConstantNode->setExpressionType((EnumDSSExpressionType)et);
			
			for (SimpleXMLNode lChildNode = iXMLNode.GetFirstChild(); lChildNode.IsValid(); lChildNode = lChildNode.GetNextSibling())
			{
				if (lChildNode.IsElementNode())
				{
					if (lChildNode.GetName().compare("cst") == 0)
					{
						// constant's data type and value
						lChildNode.GetProperty("ddt", lstrProp);
						int ddt = DSSExpNode::StringToInt(lstrProp);
						lpConstantNode->setDataType(ddt);
						CComVariant value;
						lChildNode.GetContent(lstrProp);
						DSSExpNode::StringToVariant(lstrProp, ddt, value);
						lpConstantNode->setValue(value);
					}
				}
			}
			return lpConstantNode;
		}
			break;
		case DssNodeTime:
		{
			DSSExpNodeConstant* lpTimeNode;
			if (ipParentExpNode)
				lpTimeNode = static_cast<DSSExpNodeConstant*>(ipParentExpNode->Add(DssNodeTime, ""));
			else
				lpTimeNode = new DSSExpNodeConstant(ipContext, DssNodeTime, "", ipExpression);
			lpTimeNode->setExpressionType((EnumDSSExpressionType)et);
			
			for (SimpleXMLNode lChildNode = iXMLNode.GetFirstChild(); lChildNode.IsValid(); lChildNode = lChildNode.GetNextSibling())
			{
				if (lChildNode.IsElementNode())
				{
					if (lChildNode.GetName().compare("cst") == 0)
					{
						// constant's data type and value
						lChildNode.GetProperty("ddt", lstrProp);
						int ddt = DSSExpNode::StringToInt(lstrProp);
						lpTimeNode->setDataType(ddt);
						CComVariant value;
						lChildNode.GetContent(lstrProp);
						DSSExpNode::StringToVariant(lstrProp, ddt, value);
						lpTimeNode->setValue(value);
					}
				}
			}
			return lpTimeNode;
		}
			break;
		case DssNodeOperator:
		{
			DSSExpNodeOperator* lpOperatorNode;
			if (ipParentExpNode)
				lpOperatorNode = static_cast<DSSExpNodeOperator*>(ipParentExpNode->Add(DssNodeOperator, ""));
			else
				lpOperatorNode = new DSSExpNodeOperator(ipContext, "", ipExpression);
			
			lpOperatorNode->setExpressionType((EnumDSSExpressionType)et);
			
			for (SimpleXMLNode lChildNode = iXMLNode.GetFirstChild(); lChildNode.IsValid(); lChildNode = lChildNode.GetNextSibling())
			{
				if (lChildNode.IsElementNode())
				{
					if (lChildNode.GetName().compare("nd") == 0)
					{
						// Found a child exp node
						hBuildExpNode(ipExpression, lChildNode, lpOperatorNode, ipContext);
					}
					else if (lChildNode.GetName().compare("op") == 0)
					{
						// operator's function type
						lChildNode.GetProperty("fnt", lstrProp);
						int fnt = DSSExpNode::StringToInt(lstrProp);
						lpOperatorNode->setOperatorType(fnt);
					}
				}
			}
			return lpOperatorNode;
		}
			break;
		case DssNodeShortcut:
		{
			DSSExpNodeShortcut* lpShortCutNode;
			if (ipParentExpNode)
				lpShortCutNode = static_cast<DSSExpNodeShortcut*>(ipParentExpNode->Add(DssNodeShortcut, ""));
			else
				lpShortCutNode = new DSSExpNodeShortcut(ipContext, "", ipExpression);
			
			lpShortCutNode->setExpressionType((EnumDSSExpressionType)et);
			/*for (xmlNode* lpChildNode = ipXMLNode->children; lpChildNode; lpChildNode = lpChildNode->next)
			 {
			 if (lpChildNode->type == XML_ELEMENT_NODE)
			 {
			 if (strcmp(lpChildNode->name, "at") == 0)
			 {
			 lstrProp = xmlGetProp(ipXMLNode, "rfd");
			 int rfd = DSSExpNode::StringToInt(lstrProp);
			 }
			 }
			 }*/
            
            // unsure about the logic of mpControl
            /*
			if (mpControl)
				lpShortCutNode->setTareget(mpControl->getSourceObject());
			*/
			return lpShortCutNode;
		}
			break;
		default:
		{
			DSSExpNode* lpNode;
			if (ipParentExpNode)
				lpNode = ipParentExpNode->Add(et, "");
			else
				lpNode = new DSSExpNode(ipContext, DssNodeReserved, "", ipExpression);
			lpNode->setExpressionType((EnumDSSExpressionType)et);
			return lpNode;
		}
			break;
	}
	return NULL;
}
#endif

DSSCExpression* DSSCManipulationSetSelectionFromControl::getExpression()
{
    return mpExpression;
}