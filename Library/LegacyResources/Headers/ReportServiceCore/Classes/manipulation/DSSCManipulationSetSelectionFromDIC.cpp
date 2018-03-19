/*
 *  DSSManipulationSetSelectionFromDIC.cpp
 *  ReportServiceCore
 *
 *  Created by Cao Lei on 11-9-21.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSCManipulationSetSelectionFromDIC.h"
#include "DSSRWControlNode.h"
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
#include "DSSRWGridIterator.h"
#include "DSSDocumentDefinition.h"
#include "DSSCManipulationHelper.h"
#include "DSSRWDataModel.h"

DSSCManipulationSetSelectionFromDIC::DSSCManipulationSetSelectionFromDIC(DSSRWNode* ipNode, DSSRWControl* ipControl, DSSBaseElementsProxy *ipElements, DSSCExpression* ipExpression, bool iIsNotIn)
{
	mpNode = ipNode;
	mpElements = ipElements;
	mpExpression = ipExpression;
	mIsNotIn = iIsNotIn;
	mpControl = ipControl;
	mIsFromActionObject = false;
    
    mType = ManipulationSetSelectionFromDIC;
    mpRedrawParentNode = NULL;
    mRedrawChildIndex = -1;
    mpGridIterator = NULL;
    mIsUC = false;
}

DSSCManipulationSetSelectionFromDIC::~DSSCManipulationSetSelectionFromDIC()
{
	NodeAndControlsMap::iterator it;
	for ( it=mNodeMapSelectionUpdate.begin() ; it != mNodeMapSelectionUpdate.end(); it++ )
	{
		if (mpNode->getObjectContext()->getNode((*it).second->NodeKey)->getType()==DssRWNodeTemplate)
			delete (*it).second;
	}

	if (mpElements)
	{
		delete mpElements;
		mpElements = NULL;
	}
	
	/*if (mpExpression && mIsNotIn) //In such case, expression is allocated internally.
	{
		delete mpExpression;
		mpExpression = NULL;
	}*/
    mpRedrawParentNode = NULL;
    mRedrawChildIndex = -1;
    mpGridIterator = NULL;
}

DSSCManipulationSetSelectionFromDIC::DSSCManipulationSetSelectionFromDIC(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationSetSelectionFromDIC;
    
    mpExpression = NULL;
    if(ipAO->IsDeltaXML())
    {
        DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
        std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
        hParseExpressionFromXML(lXMLString, ipContext);
#endif
        mpElements = NULL;
    }
    else
    {
        DSSAOSelection* lpAO = static_cast<DSSAOSelection*>(ipAO);
        mpNode = ipContext->getNode(lpAO->getNodeKey());
        mpControl = ipContext->getControl(lpAO->getControlKey());
        mpElements = new DSSBaseElementsProxy(ipContext);
        
        StringVector::iterator it1;
        for (it1=lpAO->getElementIDs()->begin(); it1!=lpAO->getElementIDs()->end(); it1++)
        {
            DSSBaseElementProxy* lpElement = DSSBaseElementsProxy::CreateElementFromID(*it1, ipContext);
            if (lpElement)
                mpElements->AddElement(lpElement);
        }
    }
    mIsFromActionObject = true;
    mpRedrawParentNode = NULL;
    mRedrawChildIndex = -1;
    mpGridIterator = NULL;
}


int DSSCManipulationSetSelectionFromDIC::Execute()
{
	
	if (!mpControl)
		return S_OK;
	
	if (hCheckLayoutLoaded(mpNode)==S_FALSE)
		return S_FALSE;
	
	std::vector<DSSRWNode*> lNodesModelChanged;

	// Set current elments on control

	DSSObjectContext* lpObjectContext = mpNode->getObjectContext();

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
	// Process for different control type

	EnumDSSRWControlType lType = mpControl->getType();
	int lTargetCount;

	switch (lType) {
		case DssRWControlTypeElementList:
		case DssRWControlTypeMetricList:
        case DssRWControlTypeMultipleUnit:
		{
			//DSSMetric Condition
			if (mpExpression && mpControl->getSourceObject() && mpControl->getSourceObject()->getType()==DssTypeMetric) 
			{
				DSSCExpression* lpExpression = new DSSCExpression(lpObjectContext);
				lpExpression->Clone(mpExpression);  //< make a copy of mpExpression.
				mpControl->setExpression(lpExpression);
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
				mpControl->setExpression(mpExpression); //NotIn Case: set expression.
				mpControl->ClearCurrentElements();
			}	
			if(mpControl->getType() == DssRWControlTypeMultipleUnit)
            {
				#ifndef __ANDROID__
                mpControl->setExpression(mpExpression);
                if(mpGridIterator)

                    mpGridIterator->calculateCurrentSelection();
#endif
                if(mpControl->getTargetDatasetCount() > 0)
                {
                    /*
                     if(lpControl->getCountTargets() > 0)
                     {
                     for(int i = 0; i < lpControl->getCountTargets(); i++)
                     mNodeMapServer[lpControl->getTargetKey(i)] = lpControl->getTargetNode(i);
                     }
                     else
                     */
                    mpControl->getObjectContext()->getDocumentDefinition()->getNodesForDatasetSelector(mpControl, &mNodeMapServer, &mNodesModelChanged, &mDDICModelChanged);
                    mRetrievalGlobalLookupTableHeader = true;
                    // if uc on dataset && layout contains ngb all, we need to redraw the layout
                    DSSRWNode* lpLayoutNode = mpNode->getLayout();
                    if(lpLayoutNode->IsAboveNGBAll())
                    {
                        mpRedrawParentNode = static_cast<DSSRWSectionNode*>(lpLayoutNode->getParentNode());
                        mRedrawChildIndex = lpLayoutNode->getIndex();
                    }
                    //Reset all layouts other than current layout's loaded flag
//                    DSSDocumentDefinition* lpDefinition = lpObjectContext->getDocumentDefinition();
//                    if(lpDefinition){
//                        std::vector<std::string>* allLayouts = lpDefinition->getLayoutKeys();
//                        std::string currentLayout = lpDefinition->getCurrentLayoutKey();
//                        for(std::vector<std::string>::iterator it = allLayouts->begin();it<allLayouts->end();it++){
//                            if((*it)!=currentLayout){
//                                DSSRWNode* lpNode = lpObjectContext->getNode(*it);
//                                if (lpNode)
//                                    lpNode->setLayoutLoaded(false);
//                            }
//                        }
//                    }
                    mNeedResetAllLayouts = true;
                }
			}
			lTargetCount = mpControl->getCountTargets();
			for (int i=0; i<lTargetCount; i++)
			{
				std::string lTargetKey = mpControl->getTargetKey(i);
				DSSRWNode* lpTargetNode = lpObjectContext->getNode(lTargetKey);
				if (!lpTargetNode || lpTargetNode->getType()!=DssRWNodeSection)
					continue;
				DSSRWSectionNode* lpSectionNode = (DSSRWSectionNode*)(lpTargetNode);
				lpSectionNode->setLatestControlKey(mpControl->getKey());

				if (lpSectionNode->getSectionType()!=DssRWSectionControlGroupBySection && lpSectionNode->getSectionType()!=DssRWSectionUnitCondition)
					continue;
				if (DSSCManipulationHelper::setTargetGroupByUnit(lpTargetNode, mpControl, mpExpression, mpElements) == S_FALSE)
                    continue;

				// Add the UC node's highest CGB node into the Server node map (the root nodes needing go back to server to get partial data retrieval)
				if (lpSectionNode->getSectionType()==DssRWSectionUnitCondition)
				{
					mIsUC = true;

					// get highest CGB node if UC
					DSSRWNode* lHighestCGB = lpSectionNode->getHighestCGB();
					if (lHighestCGB)
						lpTargetNode = lHighestCGB;

					mNodeMapServer[lpTargetNode->getKey()] = lpTargetNode;
				}

				// Add the target node and all its subtree into the modelChanged map
                // 865845, if the selector is ddic, do not exclude self
                bool lbExcludeSourceNode = true;
                if (mpControl->getObjectContext()->getNode(mpControl->getNodeKeyDDIC()))
                    lbExcludeSourceNode = false;
				lpTargetNode->AddNodeMap(lNodesModelChanged, mNodeMapServer, mNodeMapSelectionUpdate, mpNode->getKey(), mIsUC, lpTargetNode->getKey(), lbExcludeSourceNode);
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
                {
                    mIsUC = true;
                    mControlsModelChanged.push_back(lpTargetControl);
                }
                
                DSSRWNode* lTargetNodeDDIC = lpObjectContext->getNode(lpTargetControl->getNodeKeyDDIC());
                if(lTargetNodeDDIC && std::find(mNodesModelChanged.begin(), mNodesModelChanged.end(), lTargetNodeDDIC) == mNodesModelChanged.end())
                    mNodesModelChanged.push_back(lTargetNodeDDIC);
                
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

    // jzeng; 09/02/14; update the target filter when current selection changed
    DSSCManipulationHelper::updateSelectionOfAffectedControl(mpControl, mNodeMapServer, mControlsModelChanged, mNodesModelChanged, mNodeMapSelectionUpdate);
    
    // jzeng; 09/22/15 reinit ddic iterator
    DSSCManipulationSetSelectionFromDIC::reinitDDICIterator(mpControl);
    
	if (!mIsFromActionObject)
	{
		if (mpExpression) //DSSMetric Condition or NotIn.
		{
			mpActionObject = new DSSAODeltaXML(ManipulationSetSelectionFromDIC);
			std::string lXML = hBuildExpressionXML();	
			printf("xml:%s\n", lXML.c_str());
			(static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lXML);
                        
            if (mpControl->getType() != DssRWControlTypeMultipleUnit)   //for multi unit control, we only set the condition in the template node.
                (static_cast<DSSAODeltaXML*>(mpActionObject))->setIsNotIn(mIsNotIn);	//2011-6-2 lcao TQMS 478489
		}
		else 
		{
			mpActionObject = new DSSAOSelection(ManipulationSetSelectionFromDIC);
			DSSAOSelection* lpActionObject = static_cast<DSSAOSelection*> (mpActionObject);
			lpActionObject->setNodeKey(mpNode->getKey());
			lpActionObject->setControlKey(mpControl->getKey());
			lpActionObject->setTreeType(mpNode->getTreeType());

			for (int i=0; i<mpElements->Count(); i++)
				lpActionObject->addElementID(mpElements->Item(i)->getElementID());

			NodeMap::iterator it;
			for ( it=mNodeMapServer.begin() ; it != mNodeMapServer.end(); it++ )
				lpActionObject->addServerKey((*it).first);
		}
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
		mNodesModelChanged.push_back(mpNode);
	
	std::vector<DSSRWNode*>::iterator it2 =lNodesModelChangedControl.begin();
	std::vector<DSSRWNode*>::iterator it3 =lNodesModelChangedControl.end();
	for (; it2 != it3; it2++ )
	{
		mNodesModelChanged.push_back(*it2);
	}
#ifndef	__ANDROID__
	// add metric condition selector nodes to mNodesModelChanged
	if (mpControl->getType()==DssRWControlTypeElementList && mpControl->getSourceObject() && mpControl->getSourceObject()->getType()!=DssTypeMetric)
	{
		DSSRWNode* lpParentNode = mpNode->getParentNode();
		DSSRWNode* lpNodeToExplore = hGetLowestNonGroupBySectionNode(lpParentNode);
		if (lpNodeToExplore)
			lpParentNode = lpNodeToExplore;
		AddMetricConditionSelectorsToMap(lpParentNode, mpControl->getKey());
	}
#endif	
	return S_OK;
}

int DSSCManipulationSetSelectionFromDIC::reinitDDICIterator(DSSRWControl * const ipControl) {
    int hr = S_OK;
    if (ipControl) {
        DSSObjectContext* lpObjectContext = ipControl->getObjectContext();
        if (!lpObjectContext) {
            return hr;
        }
        const std::string controlKey = ipControl->getKey();
        int targetCount = ipControl->getCountTargetControls();
        for (int i = 0; i < targetCount; i++) {
            // get the container node key
            std::string targetKey = ipControl->getTargetControlKey(i);
            DSSRWControl *targetControl = lpObjectContext->getControl(targetKey);
            if (!targetControl) {
                continue;
            }
            std::string containerNode = targetControl->getNodeKeyDDIC();
            
            // get the container node iterator
            DSSRWDataModel *lDataModel = lpObjectContext->getDataModel();
            if (!lDataModel) {
                return E_POINTER;
            }
            DSSRWIterator * lContainerIterator = NULL;
            hr = lDataModel->getCachedIterator(containerNode, &lContainerIterator);
            if (lContainerIterator) {
                hr = lContainerIterator->ReInitInputControlIterator(controlKey);
                CHECK_HR;
            }
        }
    }
    return hr;
}

void DSSCManipulationSetSelectionFromDIC::resetAllLayouts()
{
    //Reset all layouts other than current layout's loaded flag
	DSSObjectContext* lpObjectContext = mpNode->getObjectContext();
    
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
std::string DSSCManipulationSetSelectionFromDIC::hBuildExpressionXML()
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

	std::string lstrNodeKey = mpNode->getKey();
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
#else
std::string DSSCManipulationSetSelectionFromDIC::hBuildExpressionXML()
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
	
	std::string lStr = mpNode->getKey();
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

void DSSCManipulationSetSelectionFromDIC::hParseExpressionFromXML(std::string& iXMLString, DSSObjectContext* ipContext)
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
				mpNode = ipContext->getNode(lstrContent);
			}
			else if (lCurrentNode.GetName().compare("rw_control_key") == 0)
			{
				// Found the control key
				lCurrentNode.GetContent(lstrContent);
				mpControl = (DSSRWControl*)ipContext->getControl(lstrContent);
			}
            else if(lCurrentNode.GetName().compare("element_ids_collection") == 0)
            {
                for(SimpleXMLNode lCurrentElementNode = lCurrentNode.GetFirstChild(); lCurrentElementNode.IsValid(); lCurrentElementNode = lCurrentElementNode.GetNextSibling())
                {
                    if(lCurrentElementNode.IsElementNode() && lCurrentElementNode.GetName().compare("element_id") == 0)
                    {
                        lCurrentElementNode.GetContent(lstrContent);
                        DSSBaseElementProxy* lElement = new DSSBaseElementProxy(ipContext);
                        lElement->Parse(lstrContent);
                        mpElements->AddElement(lElement);
                    }
                }
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

DSSExpNode* DSSCManipulationSetSelectionFromDIC::hBuildExpNode(SimpleXMLNode& iXMLNode, DSSExpNode* ipParentExpNode, DSSObjectContext* ipContext)
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
			if (mpControl && mpControl->getSourceObject())
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
DSSBaseElementsProxy* DSSCManipulationSetSelectionFromDIC::getElements()
{
	return mpElements;
}

void DSSCManipulationSetSelectionFromDIC::setGridIterator(DSSRWGridIterator *ipGridIterator)
{
    mpGridIterator = ipGridIterator;
}

bool DSSCManipulationSetSelectionFromDIC::IsNeedServer()
{
    return mIsUC || mRetrievalGlobalLookupTableHeader || (mNodeMapServer.size() > 0);
}
ControlVector* DSSCManipulationSetSelectionFromDIC::getControls()
{
    ControlVector* lpControls = new ControlVector;
	if(mpNode->getType() == DssRWNodeTemplate)
    {
        DSSRWTemplateNode* lpTemplateNode = static_cast<DSSRWTemplateNode*>(mpNode);
        if(lpTemplateNode->getControls())
        {
            for(int i = 0; i < lpTemplateNode->getControls()->Count(); i++)
                lpControls->push_back(lpTemplateNode->getControls()->Item(i));
        }
    }
    if(lpControls->size())
        return lpControls;
    return NULL;
}

DSSRWNode* DSSCManipulationSetSelectionFromDIC::getNode()
{
    return mpNode;
}