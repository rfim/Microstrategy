/*
 *  DSSCManipulationSetSelectionFromTemplate.cpp
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-15.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <sstream>
#include "DSSCManipulationSetSelectionFromTemplate.h"
#include "DSSRWTemplateNode.h"
#include "DSSAOSelection.h"
#include "DSSDocumentDefinition.h"
#include "DSSCManipulationHelper.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif

DSSCManipulationSetSelectionFromTemplate::DSSCManipulationSetSelectionFromTemplate(DSSRWIterator* ipIterator, DSSCellHandle* ipCellHandle, std::string iDisplayText)
{
	mpTemplateNode = (DSSRWTemplateNode*)(ipIterator->getRWNode());
	mpElements = new DSSBaseElementsProxy(mpTemplateNode->getObjectContext());
    mIsNewBehavior = false;
    mpRedrawParentNode = NULL;
    mRedrawChildIndex = -1;
    
    mpIterator = ipIterator;
    mpCellHandles.push_back(*ipCellHandle);
    
	hInit(ipIterator, ipCellHandle, iDisplayText);
    
    mType = ManipulationSetSelectionFromTemplate;
    mIsUC = false;
}


DSSCManipulationSetSelectionFromTemplate::DSSCManipulationSetSelectionFromTemplate(DSSRWIterator* ipIterator, DSSCellHandle* ipCellHandle, bool iAOXML)
{
	mpTemplateNode = (DSSRWTemplateNode*)(ipIterator->getRWNode());
	mpElements = new DSSBaseElementsProxy(mpTemplateNode->getObjectContext());
    mIsNewBehavior = false;
    hInit(ipIterator, ipCellHandle);
    mpRedrawParentNode = NULL;
    mRedrawChildIndex = -1;

    mpIterator = ipIterator;
    mpCellHandles.push_back(*ipCellHandle);
    
    mAOXML = iAOXML;
    mType = ManipulationSetSelectionFromTemplate;
    mIsUC = false;
}

DSSCManipulationSetSelectionFromTemplate::DSSCManipulationSetSelectionFromTemplate(DSSRWIterator* ipIterator, std::vector<DSSCellHandle*> iCellHandleVector, bool iAOXML)
{
	mpTemplateNode = (DSSRWTemplateNode*)(ipIterator->getRWNode());
	mpElements = new DSSBaseElementsProxy(mpTemplateNode->getObjectContext());
    mIsNewBehavior = false;
    mpRedrawParentNode = NULL;
    mRedrawChildIndex = -1;

    mpIterator = ipIterator;

	std::vector<DSSCellHandle*>::iterator lIt;
	for (lIt=iCellHandleVector.begin(); lIt!=iCellHandleVector.end(); lIt++){
		hInit(ipIterator, *lIt);
        mpCellHandles.push_back(**lIt);

    }
    
    mAOXML = iAOXML;
    mType = ManipulationSetSelectionFromTemplate;
    mIsUC = false;
}

// clark, new requirement from frontend - create a DSSCManipulationSetSelectionFromTemplate for each RWControl on the TemplateNode
DSSCManipulationSetSelectionFromTemplate::DSSCManipulationSetSelectionFromTemplate(DSSRWTemplateNode* ipTemplateNode, GUID iSourceID, DSSBaseElementsProxy* ipElements, bool iAOXML)
{
    mpTemplateNode = ipTemplateNode;
    mpElements = ipElements;
    mSourceID = iSourceID;
    mIsNewBehavior = true;
    mCase = 1;
    mpRedrawParentNode = NULL;
    mRedrawChildIndex = -1;
    mAOXML = iAOXML;
    mType = ManipulationSetSelectionFromTemplate;
    mIsUC = false;
    mpIterator = NULL;

}

void DSSCManipulationSetSelectionFromTemplate::hInit(DSSRWIterator* ipIterator, DSSCellHandle* ipCellHandle, std::string iDisplayText)
{
	DSSAllElementProxy* lpAllElement;
	std::vector<DSSBaseElementProxy*> lElements;
	int hr;
	std::vector<DSSBaseElementProxy*>::iterator it;
	DSSBaseElementProxy* lpElement;
	
	switch (ipCellHandle->type) {
		
		case DssCellHandleXTabHeaderElement:
			// single element
			lpElement = ipIterator->createElementFromHandle(*ipCellHandle);
			if (lpElement)
            {
                lpElement->setDisplayText(iDisplayText);
                mpElements->AddElement(lpElement);
            }
			mCase = 1;
			break;
		
		case DssCellHandleTemplateUnit:
		case DssCellHandleTemplateUnitAndForm:
		case DssCellHandleCustomGroupAndDepth:
			// single attribute (select all)
			mpTemplateUnit = ipCellHandle->p.unit;
			lpAllElement = new DSSAllElementProxy(mpTemplateNode->getObjectContext());
			lpAllElement->Parse("OA:(All)");
            lpAllElement->setDisplayText(iDisplayText);
			mpElements->AddElement(lpAllElement);
			mCase = 2;
			break;
		
		case DssCellHandleXTabData:
			// single cell (could be multiple elements)
			hr = ipIterator->createElementsFromHandle(*ipCellHandle, DssAxisAll, lElements);
			if (hr != S_OK)
				return;
			
			for ( it=lElements.begin() ; it != lElements.end(); it++ )
				mpElements->AddElement(*it);
			
			mCase = 3;
			break;
		
		default:
			break;
	}
	
	mIsFromActionObject = false;
}

DSSCManipulationSetSelectionFromTemplate::DSSCManipulationSetSelectionFromTemplate(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationSetSelectionFromTemplate;
    mIsUC = false;
    mpIterator = NULL;

    if(ipAO->IsDeltaXML())
    {
        DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
        std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
        hParseAOXML(lXMLString, ipContext);
#endif
    }
    else
    {
        DSSAOSelection* lpAO = static_cast<DSSAOSelection*>(ipAO);
        mpTemplateNode = (DSSRWTemplateNode*)(ipContext->getNode(lpAO->getNodeKey()));
        mpElements = new DSSBaseElementsProxy(ipContext);
        mIsNewBehavior = false;
        mCase = 1;        
		mpRedrawParentNode = NULL;
		mRedrawChildIndex = -1;
		
        StringVector::iterator it1;
        for (it1=lpAO->getElementIDs()->begin(); it1!=lpAO->getElementIDs()->end(); it1++)
        {
            DSSBaseElementProxy* lpElement = DSSBaseElementsProxy::CreateElementFromID(*it1, ipContext);
            
            if (lpElement)
            {
                mpElements->AddElement(lpElement);
                
                if (lpElement->getElementType()==DssElementAll)
                {
                    mCase = 2;
                    DSSTemplate* lpTemplate = mpTemplateNode->getTemplate();
                    if (lpTemplate)
                    {
                        DSSRWControl* lpControl = mpTemplateNode->getObjectContext()->getControl(lpAO->getControlKey());
                        if (lpControl)
                        {
                            GUID lID = lpControl->getSourceID();
                            if (lID != GUID_NULL)
                                mpTemplateUnit = lpTemplate->getTemplateUnitFromAxisIndex(int(DssAxisAll), lID);
                            else
                                mpTemplateUnit = lpTemplate->getTemplateMetrics();
                        }
                    }
                }
            }
        }
        
        mIsFromActionObject = true;
    }
}

DSSCManipulationSetSelectionFromTemplate::~DSSCManipulationSetSelectionFromTemplate()
{
	if (mpElements)
	{
		delete mpElements;
		mpElements = NULL;
	}
	
	NodeAndControlsMap::iterator it;
	for ( it=mNodeMapSelectionUpdate.begin() ; it != mNodeMapSelectionUpdate.end(); it++ )
	{
		if (mpTemplateNode->getObjectContext()->getNode((*it).second->NodeKey)->getType()==DssRWNodeTemplate)
			delete (*it).second;
	}
    mpCellHandles.clear();
}

bool DSSCManipulationSetSelectionFromTemplate::IsNeedServer()
{
    DSSRWControls* lpControls = mpTemplateNode->getControls();
    for(int i = 0; i < lpControls->Count(); i++)
    {
        DSSRWControl* lpControl = lpControls->Item(i);
        for(int j = 0; j < lpControl->getCountTargetControls(); j++)
        {
            DSSRWControl* lpTargetControl = lpControl->getTargetControl(j);
            if(lpTargetControl->getFilterType() == EnumDSSRWControlFilterTypeUnitCondition)
                return true;
        }
    }
    return mRetrievalGlobalLookupTableHeader || mNodeMapServer.size() > 0;
}

int DSSCManipulationSetSelectionFromTemplate::Execute()
{
	if (mpElements->Count()<1)
		return S_OK;
    
    bool isLayoutPageHeaderOrFooter = false;
    DSSRWNode* lpLayoutNode = mpTemplateNode->getLayout();
    if (lpLayoutNode->getType()==DssRWNodeSection && (((DSSRWSectionNode*)lpLayoutNode)->getSectionType()==DssRWSectionPageHeader || ((DSSRWSectionNode*)lpLayoutNode)->getSectionType()==DssRWSectionPageFooter))
        isLayoutPageHeaderOrFooter = true;
    
    DSSRWControl* lpCtrl = mpTemplateNode->getControls()->Item(0);
    int ltgtCnt = lpCtrl->getCountTargets();
    for (int i=0; i<ltgtCnt; i++)
    {
        std::string lstrTgtKey = lpCtrl->getTargetKey(i);
        DSSObjectContext* lpObjCntxt = mpTemplateNode->getObjectContext();
        if (isLayoutPageHeaderOrFooter && hCheckLayoutLoaded(lpObjCntxt->getNode(lstrTgtKey))==S_FALSE)
            return S_FALSE;
    }

	std::vector<DSSRWNode*> lNodesModelChanged;
	
	DSSRWControls* lpControls = mpTemplateNode->getControls();
	DSSObjectContext* lpObjectContext = mpTemplateNode->getObjectContext();
    DSSRWNode* lHighestNGB = NULL;
	
    // clark, new requirement from frontend - create a DSSCManipulationSetSelectionFromTemplate for each RWControl on the TemplateNode
    if (mIsNewBehavior)
    {
        for (int j=0; j<mpElements->Count(); j++)
        {
            DSSBaseElementProxy* lpElement = mpElements->Item(j);
            
            if (lpElement->getElementType()==DssElementAll)
            {
                mCase = 2;
                DSSTemplate* lpTemplate = mpTemplateNode->getTemplate();
                if (lpTemplate)
                {
                    DSSRWControl* lpControl = NULL;
                    bool lFound = false;
                    for (int i=0; i<lpControls->Count(); i++)
                    {
                        lpControl = lpControls->Item(i);
                        if (mIsNewBehavior && MBase::IsEqualGUID(lpControl->getSourceID(), mSourceID))
                        {
                            lFound = true;
                            break;
                        }
                    }
                    
                    if (lpControl && lFound)
                    {
                        GUID lID = lpControl->getSourceID();
                        if (lID != GUID_NULL)
                            mpTemplateUnit = lpTemplate->getTemplateUnitFromAxisIndex(int(DssAxisAll), lID);
                        else
                            mpTemplateUnit = lpTemplate->getTemplateMetrics();
                    }
                }
            }
        }
    }
    
    // new change to handle on-demand slicing
    
    bool lLayoutLoaded = true;
    
    for (int i=0; i<lpControls->Count(); i++)
	{
		DSSRWControl* lpControl = lpControls->Item(i);
        if (mIsNewBehavior && !MBase::IsEqualGUID(lpControl->getSourceID(), mSourceID))
            continue;
        
        for(int j = 0; j < lpControl->getCountTargets(); j++)
        {
            std::string lTargetKey = lpControl->getTargetKey(j);
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
        
		std::vector<DSSBaseElementProxy*> lMatchedElements;
		for (int j=0; j<mpElements->Count(); j++)
		{
			DSSBaseElementProxy* lpElement = mpElements->Item(j);
			if (mCase==2)
			{
				if (hMatchAllElementToControl(lpControl, mpTemplateUnit))
					lMatchedElements.push_back(lpElement);
			}
			else //1 or 3
			{
				if (hMatchElementToControl(lpControl, lpElement))
					lMatchedElements.push_back(lpElement);
			}
		}
		
		if (lMatchedElements.size()>0)
		{
            int lTargetCount = lpControl->getCountTargets();
			// TQMS 529238: sort the targets in the hierachical order in content tree
            std::vector<DSSRWNode*> lTargets;
            for(int i = 0; i < lTargetCount; i++)
            {
                std::string lTargetKey = lpControl->getTargetKey(i);
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
            if (mIsUC)
                break;
            
        }
    }
    
    if (hCheckLayoutLoaded(mpTemplateNode)==S_FALSE)
    {
        if (mIsUC)
            return S_FALSE;
        else
            lLayoutLoaded = false;
    }
    
	for (int i=0; i<lpControls->Count(); i++)
	{
		DSSRWControl* lpControl = lpControls->Item(i);
        if (mIsNewBehavior && !MBase::IsEqualGUID(lpControl->getSourceID(), mSourceID))
            continue;
		
		std::vector<DSSBaseElementProxy*> lMatchedElements;
		
		for (int j=0; j<mpElements->Count(); j++)
		{
			DSSBaseElementProxy* lpElement = mpElements->Item(j);
			if (mCase==2)
			{
				if (hMatchAllElementToControl(lpControl, mpTemplateUnit))
					lMatchedElements.push_back(lpElement);
			}
			else //1 or 3
			{
				if (hMatchElementToControl(lpControl, lpElement))
					lMatchedElements.push_back(lpElement);
			}
		}
		
		DSSAOSelection* lpActionObject;
		
		if (lMatchedElements.size()>0)
		{
			if (!mIsFromActionObject)
			{
				// create ao
				lpActionObject = new DSSAOSelection(ManipulationSetSelectionFromTemplate);
				lpActionObject->setNodeKey(mpTemplateNode->getKey());
				lpActionObject->setControlKey(lpControl->getKey());
				lpActionObject->setTreeType(mpTemplateNode->getTreeType());
				mActionObjects.push_back(lpActionObject);
			}
				
			// set current elements on the control
			
			DSSBaseElementsProxy* lpElements = lpControl->getCurrentElements();
			if (!lpElements)
				lpElements = new DSSBaseElementsProxy(lpObjectContext);
			else
				lpElements->Clear();
	
			//for each element in lMatchedElements, if it's not subtotal, add it otherwise, transfer the subtotoal index to global index and add it
			
			std::vector<DSSBaseElementProxy*>::iterator it;
			for ( it=lMatchedElements.begin() ; it != lMatchedElements.end(); it++ )
			{
				DSSBaseElementProxy* lpMatchedElement = *it;
				
				if (!mIsFromActionObject)
				{
					// populate ao elements
					if (lpMatchedElement->getElementType()==DssElementConcrete && lpMatchedElement->getUnitType()==DssTemplateAttribute)
                        lpActionObject->addElementID(((DSSAttributeElementProxy*)lpMatchedElement)->getCompactElementID());
                    else
                        lpActionObject->addElementID(lpMatchedElement->getElementID());
				}
				
				if (lpMatchedElement->getElementType()!=DssElementSubtotal)
				{
					DSSBaseElementProxy* lpElement = DSSBaseElementsProxy::Clone(lpMatchedElement, lpObjectContext);
					if (lpElement)
						lpElements->AddElement(lpElement);
				}
				else
				{
					int lLocalSubtotalIndex = ((DSSSubtotalElementProxy*)(lpMatchedElement))->getSubtotalIndex();
					int lGlobalSubtotalIndex;
					GUID lTemplateID = mpTemplateNode->getTemplate()->getID();
					int hr = lpObjectContext->getDocumentLevelSubtotalIndex(lLocalSubtotalIndex, &lTemplateID, &lGlobalSubtotalIndex);
					if (hr!=S_OK)
						continue;
					
					std::string lElementIDLocal = lpMatchedElement->getElementID();
					std::string lElementIDGlobal = lElementIDLocal.substr(0, 36);
					std::ostringstream oss;
					oss << lGlobalSubtotalIndex;
					lElementIDGlobal.append(oss.str());
					DSSSubtotalElementProxy* lpMatchedElementGlobal = new DSSSubtotalElementProxy(lpObjectContext);
					lpMatchedElementGlobal->Parse(lElementIDGlobal);
					
					DSSBaseElementProxy* lpElement = DSSBaseElementsProxy::Clone(lpMatchedElementGlobal, lpObjectContext);
					if (lpElement)
						lpElements->AddElement(lpElement);
				}
			}
            
            // UC on dataset
            if(lpControl->getTargetDatasetCount() > 0)
            {
                /*
                if(lpControl->getCountTargets() > 0)
                {
                    for(int i = 0; i < lpControl->getCountTargets(); i++)
                        mNodeMapServer[lpControl->getTargetKey(i)] = lpControl->getTargetNode(i);
                }
                else
                 */
                    lpControl->getObjectContext()->getDocumentDefinition()->getNodesForDatasetSelector(lpControl, &mNodeMapServer, &mNodesModelChanged, &mDDICModelChanged);
                mRetrievalGlobalLookupTableHeader = true;
                // if uc on dataset && layout contains ngb all, we need to redraw the layout
                DSSRWNode* lpLayoutNode = mpTemplateNode->getLayout();
                if(lpLayoutNode->IsAboveNGBAll())
                {
                    mpRedrawParentNode = static_cast<DSSRWSectionNode*>(lpLayoutNode->getParentNode());
                    mRedrawChildIndex = lpLayoutNode->getIndex();
                }
                
                //Reset all layouts other than current layout's loaded flag
//                DSSDocumentDefinition* lpDefinition = lpObjectContext->getDocumentDefinition();
//                if(lpDefinition){
//                    std::vector<std::string>* allLayouts = lpDefinition->getLayoutKeys();
//                    std::string currentLayout = lpDefinition->getCurrentLayoutKey();
//                    for(std::vector<std::string>::iterator it = allLayouts->begin();it<allLayouts->end();it++){
//                        if((*it)!=currentLayout){
//                            DSSRWNode* lpNode = lpObjectContext->getNode(*it);
//                            if (lpNode)
//                                lpNode->setLayoutLoaded(false);
//                        }
//                    }
//                }
                mNeedResetAllLayouts = true;
            }
			
			// Set current elements on groupby
			int lTargetCount = lpControl->getCountTargets();
			// TQMS 529238: sort the targets in the hierachical order in content tree
            std::vector<DSSRWNode*> lTargets;
            for(int i = 0; i < lTargetCount; i++)
            {
                std::string lTargetKey = lpControl->getTargetKey(i);
                DSSRWNode* lpTargetNode = lpObjectContext->getNode(lTargetKey);
                lTargets.push_back(lpTargetNode);
            }
            SortControlTarget(lTargets);

            lpControl->getHighlightedNodeKeys(mNodeKeysExclusive);
			for (int j=0; j<lTargetCount; j++)
			{
				// std::string lTargetKey = lpControl->getTargetKey(j);
				// DSSRWNode* lpTargetNode = lpObjectContext->getNode(lTargetKey);
				DSSRWNode* lpTargetNode = lTargets[j];
				if (!lpTargetNode || lpTargetNode->getType()!=DssRWNodeSection)
					continue;
				DSSRWSectionNode* lpSectionNode = (DSSRWSectionNode*)(lpTargetNode);
				lpSectionNode->setLatestControlKey(lpControl->getKey());
				
				if (DSSCManipulationHelper::setTargetGroupByUnit(lpTargetNode, lpControl, NULL, lpElements) == S_FALSE)
                    continue;
				
				// Add the UC node into the Server node map (the root nodes needing go back to server to get partial data retrieval)
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
                        populateSubtreeIntoNodeModelChanged(lHighestNGB, mpTemplateNode);
                        
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
				lpTargetNode->AddNodeMap(lNodesModelChanged, mNodeMapServer, mNodeMapSelectionUpdate, mpTemplateNode->getKey(), isUC, lpTargetNode->getKey(), true);
			}
            
            // jzeng; 07/31/14; changing selection need refresh target in placeholderMap
            const PlaceHolderMap& lPlaceHolderMap = lpControl->getPlaceHolderMap();
            for (PlaceHolderMap::const_iterator it = lPlaceHolderMap.begin(); it != lPlaceHolderMap.end(); it++)
            {
                const std::string& lTargetNodeKey = it->first;
                DSSRWNode* lTargetNode = lpObjectContext->getNode(lTargetNodeKey);
                DSSCManipulationHelper::setTargetGroupByUnit(lTargetNode, lpControl, NULL, lpElements, DSSCManipulationHelper::DssPlaceHolderTarget);
                setHasPlaceHolderTargets(true);
            }
            DSSCManipulationHelper::addPlaceHolderMapToNodeMapServer(lpControl, mNodeMapServer, mNodesModelChanged, mNodeMapSelectionUpdate);
            // jzeng; 09/02/14; update the target filter when current selection changed
            DSSCManipulationHelper::updateSelectionOfAffectedControl(lpControl, mNodeMapServer, mControlsModelChanged, mNodesModelChanged, mNodeMapSelectionUpdate);
            
            int lTargetControlCount = lpControl->getCountTargetControls();
            for (int j = 0; j < lTargetControlCount; j++)
            {
                DSSRWControl* lpTargetControl = lpControl->getTargetControl(j);
                if(!lpTargetControl)
                    continue;

                DSSGroupByUnits* lpUnits = lpTargetControl->getGroupbyPath();
                if (lpUnits && lpUnits->Count() > 0)
                {
                    DSSGroupByUnit* lpUnit = NULL;
                    GUID lControlObjectID = lpControl->getSourceID();
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
                    
                    for (int j=0; j<mpElements->Count(); j++)
                    {
                        DSSBaseElementProxy* lpElement = DSSBaseElementsProxy::Clone(mpElements->Item(j), lpObjectContext);
                        if (lpElement)
                            lpUnit->AddCurrentElement(lpElement);
                    }
                    
                    if(lpUnit->getCurrentElementsStatus() == DssCurrentElementUnset)//xiazhou, change element status for GBUnit
                        lpUnit->setCurrentElementsStatus(DssCurrentElementChosen);
                }

                if (lpTargetControl->getFilterType() == EnumDSSRWControlFilterTypeUnitCondition)
                    mControlsModelChanged.push_back(lpTargetControl);

                DSSRWNode* lTargetNodeDDIC = lpObjectContext->getNode(lpTargetControl->getNodeKeyDDIC());
                if(lTargetNodeDDIC && std::find(mNodesModelChanged.begin(), mNodesModelChanged.end(), lTargetNodeDDIC) == mNodesModelChanged.end())
                    mNodesModelChanged.push_back(lTargetNodeDDIC);
                
                DSSCManipulationHelper::syncSelectionByTargetControl(lpTargetControl, mpTemplateNode->getKey(), lTargetNodeDDIC->getKey(), mNodeMapSelectionUpdate);
            }
			mControls.push_back(lpControl);
		}
        if (mIsNewBehavior)
             break;
	}
	
    std::vector<DSSActionObject*>::iterator it;
    for ( it=mActionObjects.begin() ; it != mActionObjects.end(); it++ )
    {
        NodeMap::iterator it2;
        for ( it2=mNodeMapServer.begin() ; it2 != mNodeMapServer.end(); it2++ )
            static_cast<DSSAOSelection*>((*it))->addServerKey((*it2).first);
    }
    
    // put all control nodes to the end of the vector
    std::vector<DSSRWNode*> lNodesModelChangedControl;
    std::vector<DSSRWNode*>::iterator it1;
    for ( it1=lNodesModelChanged.begin() ; it1 != lNodesModelChanged.end(); it1++ )
    {
        if ((*it1)->getType()!=DssRWNodeControl)
            mNodesModelChanged.push_back(*it1);
        else
            lNodesModelChangedControl.push_back(*it1);
    }
    
    if (mIsFromActionObject)
        mNodesModelChanged.push_back(mpTemplateNode);
    
    // sort the changed node in order, to sure the node model reinit correctly
    if (lpControls->Count() > 1) {
        SortNodeOrder(mNodesModelChanged);
    }
    
    std::vector<DSSRWNode*>::iterator it2 =lNodesModelChangedControl.begin();
    std::vector<DSSRWNode*>::iterator it3 =lNodesModelChangedControl.end();
    for (; it2 != it3; it2++ )
    {
        mNodesModelChanged.push_back(*it2);
    }
    
    if(mDDICModelChanged.size())
    {
        // we only save DDIC with matched dataset but different from the datasource of the template
        for(std::vector<DSSRWNode*>::iterator lIter1 = mNodesModelChanged.begin(); lIter1 != mNodesModelChanged.end(); lIter1++)
        {
            if((*lIter1)->getType() != DssRWNodeTemplate)
                continue;
            std::string lTemplateNodeKey = (*lIter1)->getKey();
            std::map<std::string, std::vector<std::string> >::iterator lIter2 = mDDICModelChanged.find(lTemplateNodeKey);
            if(lIter2 != mDDICModelChanged.end())
                mDDICModelChanged.erase(lIter2);
        }
    }
    
	return S_OK;
}

void DSSCManipulationSetSelectionFromTemplate::resetAllLayouts()
{
    //Reset all layouts other than current layout's loaded flag
	DSSObjectContext* lpObjectContext = mpTemplateNode->getObjectContext();

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

bool DSSCManipulationSetSelectionFromTemplate::hMatchAllElementToControl(DSSRWControl* ipControl, DSSTemplateUnit* ipTemplateUnit)
{
	EnumDSSRWControlType lControlType = ipControl->getType();
	if (ipTemplateUnit->getType()==DssTemplateMetrics)
	{
		if (lControlType==DssRWControlTypeMetricList)
			return true;
	}
	else {
		if (lControlType==DssRWControlTypeElementList)
		{
			GUID lIDFromUnit = ipTemplateUnit->getID();
			GUID lIDFromControl = ipControl->getSourceID();
			if (MBase::IsEqualGUID(lIDFromUnit, lIDFromControl))
				return true;
		}
	}
	return false;
}

bool DSSCManipulationSetSelectionFromTemplate::hMatchElementToControl(DSSRWControl* ipControl, DSSBaseElementProxy* ipElement)
{
	EnumDSSElementType lElementType = ipElement->getElementType();
	EnumDSSTemplateUnitType lUnitType = ipElement->getUnitType();
	GUID lIDFromElement = ipElement->getObjectID();
	EnumDSSRWControlType lControlType = ipControl->getType();
	
	bool lMatched = false;
	
	if (lControlType==DssRWControlTypeElementList)
	{
		GUID lSourceID = ipControl->getSourceID();
		
		if (  ( lUnitType == DssTemplateAttribute ||
			   lUnitType == DssTemplateConsolidation ||
			   lUnitType == DssTemplateCustomGroup ||
               lElementType == DssElementNULL
               ) &&
			MBase::IsEqualGUID(lIDFromElement, lSourceID ))
		{
			if (lElementType!=DssElementSubtotal)
				lMatched = true;
			else 
			{
				DSSDocumentSubtotalRefs* lpSubtotalsOnControl = ipControl->getSubtotals();
				if (!lpSubtotalsOnControl || lpSubtotalsOnControl->Count()==0)
					lMatched = true;
				else 
				{
					int lLocalSubtotalIndexOnInput = ((DSSSubtotalElementProxy*)(ipElement))->getSubtotalIndex();
					int lGlobalSubtotalIndexOnInput;
					GUID lTemplateID = mpTemplateNode->getTemplate()->getID();
					int hr = mpTemplateNode->getObjectContext()->getDocumentLevelSubtotalIndex(lLocalSubtotalIndexOnInput, &lTemplateID, &lGlobalSubtotalIndexOnInput);
					if (hr!=S_OK)
						return lMatched;
					if (lGlobalSubtotalIndexOnInput>=1)
					{
						DSSDocumentSubtotals* lpDocumentSubtotals = mpTemplateNode->getObjectContext()->getDocumentSubtotals();
						if (lpDocumentSubtotals && lpDocumentSubtotals->Count()>0)
						{
							std::string lSubtotalKeyOnInput = lpDocumentSubtotals->Item(lGlobalSubtotalIndexOnInput-1)->getKey();
							for (int i=0; i<lpSubtotalsOnControl->Count(); i++)
							{
								if (lpSubtotalsOnControl->Item(i)->getKey()==lSubtotalKeyOnInput)
								{
									lMatched = true;
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	else if (lControlType==DssRWControlTypeMetricList)
	{
		if (lUnitType==DssTemplateMetrics)
			lMatched = true;
	}
	return lMatched;
}

DSSRWTemplateNode* DSSCManipulationSetSelectionFromTemplate::getTemplateNode()
{
	return mpTemplateNode;
}

ControlVector* DSSCManipulationSetSelectionFromTemplate::getControls()
{
	return &mControls;
}

DSSRWNode* DSSCManipulationSetSelectionFromTemplate::getSelectionNode()
{
	return mpTemplateNode;
}

#ifndef __ANDROID__
void DSSCManipulationSetSelectionFromTemplate::GenerateAODeltaXML()
{
    for(int i = 0; i < mControls.size(); i++)
    {
        SimpleXMLBuilder lBuilder;
        SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
        
        SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
        SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("63", true));
        lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
        
        int lTreeType = static_cast<int>(mpTemplateNode->getTreeType());
        SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
        lTreeTypeXML.AddChild(lTreeTypeXMLContent);
        
        SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
        lNodeKeyXML.AddChild(lNodeKeyXMLContent);
        
        SimpleXMLNode lControlKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_control_key"));
        SimpleXMLNode lControlKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mControls[i]->getKey().c_str(), true));
        lControlKeyXML.AddChild(lControlKeyXMLContent);
        
        SimpleXMLNode lElementsIDCollectionXML(SimpleXMLBuilder::CreateNodePtr("element_ids_collection"));
        for(int j = 0; j < mpElements->Count(); j++)
        {
            char lElementID[33];
            DSSBaseElementProxy::ConvertGUIDToString(mpElements->Item(j)->getObjectID(), lElementID);
            SimpleXMLNode lElementXML(SimpleXMLBuilder::CreateNodePtr("element_id"));
            SimpleXMLNode lElementXMLContent(SimpleXMLBuilder::CreateNodePtr(lElementID, true));
            lElementXML.AddChild(lElementXMLContent);
            lElementsIDCollectionXML.AddChild(lElementXML);
        }
        
        lXML.AddChild(lManipulationMethodXML);
        lXML.AddChild(lTreeTypeXML);
        lXML.AddChild(lNodeKeyXML);
        lXML.AddChild(lControlKeyXML);
        lXML.AddChild(lElementsIDCollectionXML);
        SimpleXMLNode lRoot(lBuilder.GetRoot());
        lRoot.AddChild(lXML);
        
        DSSActionObject* lpActionObject = new DSSAODeltaXML();
        (static_cast<DSSAODeltaXML*>(lpActionObject))->setXMLString(lBuilder.ToString(false));
        mActionObjects.push_back(lpActionObject);
    }
}

void DSSCManipulationSetSelectionFromTemplate::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser(iXMLString);
    if(!lParser.IsValid())
        return;
    SimpleXMLNode lCurrentNode(lParser.GetRoot());
    if(!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
        return;
    
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
            else if(lCurrentNode.GetName().compare("rw_control_key") == 0)
            {
                lCurrentNode.GetContent(lStrContent);
                DSSRWControl* lpControl = ipContext->getControl(lStrContent);
                mControls.push_back(lpControl);
            }
            else if(lCurrentNode.GetName().compare("element_ids_collection") == 0)
            {
                for(SimpleXMLNode lCurrentElementNode = lCurrentNode.GetFirstChild(); lCurrentElementNode.IsValid(); lCurrentElementNode = lCurrentElementNode.GetNextSibling())
                {
                    if(lCurrentElementNode.IsElementNode() && lCurrentElementNode.GetName().compare("element_id") == 0)
                    {
                        lCurrentElementNode.GetContent(lStrContent);
                        DSSBaseElementProxy* lElement = new DSSBaseElementProxy(ipContext);
                        lElement->Parse(lStrContent);
                        mpElements->AddElement(lElement);
                    }
                }
            }
        }
    }
}
#else
void DSSCManipulationSetSelectionFromTemplate::GenerateAODeltaXML()
{
    /*
    for(int i = 0; i < mControls.size(); i++)
    {
        TiXmlDocument lDoc;
        TiXmlElement* lpRootElement = new TiXmlElement("rw_manipulation");
        
        TiXmlElement* lpManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
        TiXmlText* lpManipulationMethodText =  new TiXmlText("63");
        lpManipulationMethodElement->LinkEndChild(lpManipulationMethodText);
        
        int lTreeType = static_cast<int>(mpPanel->getTreeType());
        TiXmlElement* lpTreeTypeElement = new TiXmlElement("rw_tree_type");
        TiXmlText* lpTreeTypeText = new TiXmlText(DSSExpNode::IntToString(lTreeType).c_str());
        lpTreeTypeElement->LinkEndChild(lpTreeTypeText);
        
        TiXmlElement* lpNodeKeyElement = new TiXmlElement("rw_node_key");
        TiXmlText* lpNodeKeyText = new TiXmlText(mpPanel->getKey().c_str());
        lpNodeKeyElement->LinkEndChild(lpNodeKeyText);
        
        TiXmlElement* lpControlKeyElement = new TiXmlElement("rw_control_key");
        TiXmlText* lpControlKeyText = new TiXmlText(mControls[i]->getKey().c_str());
        lpControlKeyElement->LinkEndChild(lpControlKeyElement);
        
        TiXmlElement* lpElementsIDCollection = new TiXmlElement("elements_id_collection");
        for(int j = 0; j < mpElements->Count(); j++)
        {
            char lElementID[33];
            DSSBaseElementProxy::ConvertGUIDToString(mpElements->Item(j)->getObjectID(), lElementID);
            TiXmlElement* lpElementIDElement = new TiXmlElement("element_id");
            TiXmlText* lpElementIDText = new TiXmlText(lElementID);
            lpElementIDElement->LinkEndChild(lpElementIDText);
            lpElementsIDCollection->LinkEndChild(lpElementIDElement);
        }
        
        lpRootElement->LinkEndChild(lpManipulationMethodElement);
        lpRootElement->LinkEndChild(lpTreeTypeElement);
        lpRootElement->LinkEndChild(lpNodeKeyElement);
        lpRootElement->LinkEndChild(lpControlKeyElement);
        lpRootElement->LinkEndChild(lpElementsIDCollection);
        
        TiXmlPrinter lPrinter;
        lPrinter.SetIndent(NULL);
        lPrinter.SetLineBreak(NULL);
        lDoc.Accept(&lPrinter);
        
        mpActionObject = new DSSAODeltaXML();
        (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lPrinter.Str());
    }*/
}
#endif

//lishan manipulation
DSSRWIterator * DSSCManipulationSetSelectionFromTemplate::getIterator()
{
    return mpIterator;
}

std::vector<DSSCellHandle> DSSCManipulationSetSelectionFromTemplate::getCellHandles()
{
    return mpCellHandles;
}
