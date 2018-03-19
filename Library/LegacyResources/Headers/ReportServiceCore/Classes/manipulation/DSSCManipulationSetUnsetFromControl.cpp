/*
 *  DSSCManipulationSetUnsetFromControl.cpp
 *  ReportServiceCore
 *
 *  Created by lcao on 5/9/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSCManipulationSetUnsetFromControl.h"
#include "DSSAOUnsetSelectorControl.h"
#include "DSSRWControlNode.h"
#include "DSSRWTemplateNode.h"
#include "DSSCExpression.h"
#include "DSSRWControl.h"
#include "DSSDocumentDefinition.h"

#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif

DSSCManipulationSetUnsetFromControl::DSSCManipulationSetUnsetFromControl(DSSRWNode* ipNode, EnumDSSXMLRWSelectorUnsetStatus iSus /*=DssXMLRWSelectorUnsetStatusDefault*/, int iSuc /*=1*/, bool iAOXML)
{
	mpNode = ipNode;
	mpControl = NULL;
	mSus = iSus;
	mSuc = iSuc;
    mpRedrawParentNode = NULL;
    mRedrawChildIndex = -1;
	
	DSSRWControls* lpControls = NULL;
    if (mpNode->getType() == DssRWNodeControl)
        lpControls = ((DSSRWControlNode*)mpNode)->getControls();
    else if (mpNode->getType() == DssRWNodeTemplate)
        lpControls = ((DSSRWTemplateNode*)mpNode)->getControls();
    
	if (lpControls && lpControls->Count()>0)
		mpControl = lpControls->Item(0);
	
	mIsFromActionObject = false;
    mAOXML = iAOXML;
    
    mType = ManipulationSetUnsetFromControl;
}

DSSCManipulationSetUnsetFromControl::DSSCManipulationSetUnsetFromControl(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationSetUnsetFromControl;
    
    if(mAOXML)
    {
        DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
        std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
        hParseAOXML(lXMLString, ipContext);
#endif
    }
    else
    {
        DSSAOUnsetSelectorControl* lpAO = static_cast<DSSAOUnsetSelectorControl*> (ipAO);
        mpNode = (DSSRWControlNode*)(ipContext->getNode(lpAO->getNodeKey()));        
		mpRedrawParentNode = NULL;
		mRedrawChildIndex = -1;
	
        mpControl = NULL;
        DSSRWControls* lpControls = NULL;
        if (mpNode->getType() == DssRWNodeControl)
            lpControls = ((DSSRWControlNode*)mpNode)->getControls();
        else if (mpNode->getType() == DssRWNodeTemplate)
            lpControls = ((DSSRWTemplateNode*)mpNode)->getControls();
        
        if (lpControls && lpControls->Count()>0)
            mpControl = lpControls->Item(0);
        
        mSuc = lpAO->getSuc();
        mSus = lpAO->getSus();
        
        mIsFromActionObject = true;	
    } 	
}

int DSSCManipulationSetUnsetFromControl::Execute()
{
	if (!mpNode)
		return S_OK;
		
	if (hCheckLayoutLoaded(mpNode)==S_FALSE)
		return S_FALSE;
    
    bool isLayoutPageHeaderOrFooter = false;
    DSSRWNode* lpLayoutNode = mpNode->getLayout();
    if (lpLayoutNode->getType()==DssRWNodeSection && (((DSSRWSectionNode*)lpLayoutNode)->getSectionType()==DssRWSectionPageHeader || ((DSSRWSectionNode*)lpLayoutNode)->getSectionType()==DssRWSectionPageFooter))
        isLayoutPageHeaderOrFooter = true;
    
    int ltgtCnt = mpControl->getCountTargets();
    for (int i=0; i<ltgtCnt; i++)
    {
        std::string lstrTgtKey = mpControl->getTargetKey(i);
        DSSObjectContext* lpObjCntxt = mpNode->getObjectContext();
        if (isLayoutPageHeaderOrFooter && hCheckLayoutLoaded(lpObjCntxt->getNode(lstrTgtKey))==S_FALSE)
            return S_FALSE;
    }
	
	std::vector<DSSRWNode*> lNodesModelChanged;
	DSSObjectContext* lpObjectContext = mpNode->getObjectContext();
	
	mpControl->ClearCurrentElements();
	// we should not set unset on control, should be on target only
    //mpControl->setCurrentElementsStatus(DssCurrentElementUnset);
	mpControl->setUnsetStatus(mSus);
	mpControl->setUnsetElementNumber(mSuc);
	if (mpControl->getExpression())
		mpControl->getExpression()->Clear();
	
	EnumDSSRWControlType lType = mpControl->getType();
	int lTargetCount;
	switch (lType) 
	{
		case DssRWControlTypeElementList:
		case DssRWControlTypeMetricList:
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

			for (int i=0; i<lTargetCount; i++)
			{
				std::string lTargetKey = mpControl->getTargetKey(i);
				DSSRWNode* lpTargetNode = lpObjectContext->getNode(lTargetKey);
				if (!lpTargetNode || lpTargetNode->getType()!=DssRWNodeSection)
					continue;
				DSSRWSectionNode* lpSectionNode = (DSSRWSectionNode*)(lpTargetNode);
				
				if (lType == DssRWControlTypeElementList)
					lpSectionNode->DiscardTransaction();
				lpSectionNode->setLatestControlKey(mpControl->getKey());
				
				if (lpSectionNode->getSectionType()!=DssRWSectionControlGroupBySection && lpSectionNode->getSectionType()!=DssRWSectionUnitCondition)
					continue;
				DSSGroupByUnits* lpUnits = lpSectionNode->getGroupByUnits();
				if (!lpUnits || lpUnits->Count()==0)
					continue;
				DSSGroupByUnit* lpUnit = lpUnits->Item(0);
				if (!lpUnit)
					continue;
				
				lpUnit->ClearCurrentElements();
				lpUnit->setCurrentElementsStatus(DssCurrentElementUnset);
				lpUnit->setUnsetStatus(mSus);
				lpUnit->setUnsetElementNumber(mSuc);
				if (lpUnit->getExpression())
					lpUnit->getExpression()->Clear();
				
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
                    
                    DSSRWNode* lHighestNGB = lpSectionNode->getHighestNGB();
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
				lpTargetNode->AddNodeMap(lNodesModelChanged, mNodeMapServer, mNodeMapSelectionUpdate, mpNode->getKey(), isUC, lpTargetNode->getKey());
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
                    if (lpUnit->getExpression())
                        lpUnit->getExpression()->Clear();
                    lpUnit->setCurrentElementsStatus(DssCurrentElementUnset);
                    lpUnit->setUnsetStatus(mSus);
                    lpUnit->setUnsetElementNumber(mSuc);

                    DSSRWNode* lTargetNodeDDIC = lpObjectContext->getNode(lpTargetControl->getNodeKeyDDIC());
                    if(lTargetNodeDDIC)
                        mNodesModelChanged.push_back(lTargetNodeDDIC);
                }
                
                if (lpTargetControl->getFilterType() == EnumDSSRWControlFilterTypeUnitCondition)
                    mControlsModelChanged.push_back(lpTargetControl);
            }
			break;
		}
		default:
			break;
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
	
	// add metric condition selector nodes to mNodesModelChanged
	if (mpControl->getType()==DssRWControlTypeElementList && mpControl->getSourceObject() && mpControl->getSourceObject()->getType()!=DssTypeMetric)
	{
		DSSRWNode* lpParentNode = mpNode->getParentNode();
		DSSRWNode* lpNodeToExplore = hGetLowestNonGroupBySectionNode(lpParentNode);
		if (lpNodeToExplore)
			lpParentNode = lpNodeToExplore;
		AddMetricConditionSelectorsToMap(lpParentNode, mpControl->getKey());
	}	
		
	if (!mIsFromActionObject)
    {
        if(mAOXML == true)
        {
            GenerateAODeltaXML();
        }
        else
        {
            Persist();
        }
    }

	return S_OK;
}

void DSSCManipulationSetUnsetFromControl::Persist()
{
	mpActionObject = new DSSAOUnsetSelectorControl();
	((DSSAOUnsetSelectorControl*)mpActionObject)->setCtrlKey(mpControl->getKey());
	((DSSAOUnsetSelectorControl*)mpActionObject)->setNodeKey(mpNode->getKey());
	((DSSAOUnsetSelectorControl*)mpActionObject)->setSuc(mSuc);
	((DSSAOUnsetSelectorControl*)mpActionObject)->setSus(mSus);
}

DSSCManipulationSetUnsetFromControl::~DSSCManipulationSetUnsetFromControl()
{
	NodeAndControlsMap::iterator it;
	for ( it=mNodeMapSelectionUpdate.begin() ; it != mNodeMapSelectionUpdate.end(); it++ )
	{
		if (mpNode->getObjectContext()->getNode((*it).second->NodeKey)->getType()==DssRWNodeTemplate)
			delete (*it).second;
	}
}
#ifndef __ANDROID__
void DSSCManipulationSetUnsetFromControl::GenerateAODeltaXML()
{
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("82", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    
    int lTreeType = static_cast<int>(mpNode->getTreeType()); 
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    
    lXML.AddChild(lManipulationMethodXML);
    lXML.AddChild(lTreeTypeXML);
    lXML.AddChild(lNodeKeyXML);
    
    DSSRWControls* lControls = NULL;
    if (mpNode->getType() == DssRWNodeControl)
        lControls = ((DSSRWControlNode*)mpNode)->getControls();
    else if (mpNode->getType() == DssRWNodeTemplate)
        lControls = ((DSSRWTemplateNode*)mpNode)->getControls();
    
    int lCount = lControls?lControls->Count():0;
    
    for(int i = 0; i < lCount; i++)
    {
        DSSRWControl* lControl = lControls->Item(i);
        SimpleXMLNode lControlKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_control_key"));
        SimpleXMLNode lControlKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lControl->getKey().c_str(), true));
        lControlKeyXML.AddChild(lControlKeyXMLContent);
        lXML.AddChild(lControlKeyXML);
        
        // flash code: uc keys?
        SimpleXMLNode lElementStatusXML(SimpleXMLBuilder::CreateNodePtr("element_status"));
        SimpleXMLNode lElementStatusXMLContent(SimpleXMLBuilder::CreateNodePtr("1", true));
        lElementStatusXML.AddChild(lElementStatusXMLContent);
        
        lXML.AddChild(lElementStatusXML);
    }
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    
    mpActionObject = new DSSAODeltaXML();
    std::string lXMLString = lBuilder.ToString(false);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationSetUnsetFromControl::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser(iXMLString);
    if(!lParser.IsValid())
        return;
    
    SimpleXMLNode lCurrentNode = lParser.GetRoot();
    if(!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
        return;
    
    for(lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if(lCurrentNode.IsElementNode() && lCurrentNode.GetName().compare("rw_node_key"))
        {
            std::string lStrContent;
            lCurrentNode.GetContent(lStrContent);
            mpNode = ipContext->getNode(lStrContent);
            DSSRWControls* lpControls = NULL;
            if (mpNode->getType() == DssRWNodeControl)
                lpControls = ((DSSRWControlNode*)mpNode)->getControls();
            else if (mpNode->getType() == DssRWNodeTemplate)
                lpControls = ((DSSRWTemplateNode*)mpNode)->getControls();
            
            if (lpControls && lpControls->Count()>0)
                mpControl = lpControls->Item(0);
        }
    }
}
#else
void DSSCManipulationSetUnsetFromControl::GenerateAODeltaXML()
{
    /*
    TiXmlDocument lDoc;
    TiXmlElement* lpRootElement = new TiXmlElement("rw_manipulation");
    
    TiXmlElement* lpManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
    TiXmlText* lPManipulationMethodText = new TiXmlText("82");
    lpManipulationMethodElement->LinkEndChild(lpManipulationMethodText);
    
    int lTreeType = static_cast<int>(mpNode->getTreeType());
    TiXmlElement* lpTreeTypeElement = new TiXmlElement("rw_tree_type");
    TiXmlText* lpTreeTypeText = new TiXmlText(DSSExpNode::IntToString(lTreeType).c_str());
    lpTreeTypeElement->LinkEndChild(lpTreeTypeText);
    
    TiXmlElement* lpNodeKeyElement = new TiXmlElement("rw_node_key");
    TiXmlText* lpNodeKeyText = new TiXmlText(mpNode->getKey().c_str());
    lpNodeKeyElement->LinkEndChild(lpNodeKeyText);
    
    lpRootElement->LinkEndChild(lpManipulationMethodElement);
    lpRootElement->LinkEndChild(lpTreeTypeElement);
    lpRootElement->LinkEndChild(lpNodeKeyElement);
    
    DSSRWControls* lpControls = mpNode->getControls();
    int lCount = lControls->Count();
    
    for(int i = 0; i < lCount; i++)
    {
        DSSRWControl* lpControl = lpControls->Item(i);
        TiXmlElement* lpControlKeyElement = new TiXmlElement("rw_control_key");
        TiXmlText* lpControlKeyText = new TiXmlText(lpControl->getKey().c_str());
        lpControlKeyElement->LinkEndChild(lpControlKeyText);
        lpRootElement->LInkEndChild(lpControlKeyElement);
    }
    TiXmlPrinter lPrinter;
    lPrinter.SetIndent(NULL);
    lPrinter.SetLineBreak(NULL);
    lDoc.Accept(&lPrinter);
    
    mpActionObject = new DSSAODeltaXML();
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lPrinter.Str());*/
}
#endif
