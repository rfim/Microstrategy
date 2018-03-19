/*
 *  DSSCManipulationSetGroupByElement.cpp
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-22.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSCManipulationSetGroupByElement.h"
#include "DSSDocumentDefinition.h"
#include "DSSAOGroupBy.h"
#include "DSSRWGroupbyIterator.h"

#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#include "DSSBaseElementProxy.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif

DSSCManipulationSetGroupByElement::DSSCManipulationSetGroupByElement(DSSRWSectionNode* ipGroupBySectionNode, DSSBaseElementProxy* ipElement, bool iAOXML, bool iAffectOtherLayout /* =false */)
{
	mpGroupBySectionNode = ipGroupBySectionNode;
    if (ipElement)
        mpElement = DSSBaseElementsProxy::Clone(ipElement, ipElement->getObjectContext());
    else
        mpElement = NULL;
	mpRedrawParentNode = NULL;
	mRedrawChildIndex = -1;
	mAffectOtherLayout = iAffectOtherLayout;
	mIsFromActionObject = false;
    mStatus = DssSetGroupByReserved;
    mAOXML = iAOXML;
    mStatus = DssSetGroupByReserved;
    
    mType = ManipulationSetGroupByElement;
}

DSSCManipulationSetGroupByElement::DSSCManipulationSetGroupByElement(DSSActionObject* ipAO, DSSObjectContext* ipContext, bool iAffectOtherLayout)
{
    mType = ManipulationSetGroupByElement;
    mStatus = DssSetGroupByReserved;
    
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
        DSSAOGroupBy* lpAO = static_cast<DSSAOGroupBy*>(ipAO);
        mpGroupBySectionNode = (DSSRWSectionNode*)(ipContext->getNode(lpAO->getNodeKey()));
        mpElement = DSSBaseElementsProxy::CreateElementFromID(lpAO->getElementID(), ipContext);
		
        mpRedrawParentNode = NULL;
        mRedrawChildIndex = -1;
        mAffectOtherLayout = iAffectOtherLayout;
        mIsFromActionObject = true;
    }
}

DSSCManipulationSetGroupByElement::~DSSCManipulationSetGroupByElement()
{
	if (mpElement)
	{
		delete mpElement;
		mpElement = NULL;
	}
}

int DSSCManipulationSetGroupByElement::Execute()
{
	if (!mpElement || mpGroupBySectionNode==0)
		return S_OK;
		
	if (hCheckLayoutLoaded(mpGroupBySectionNode)==S_FALSE)
		return S_FALSE;
	
	// 4/27/2011 For GB change, we need to drop transaction data if any
	mpGroupBySectionNode->DiscardTransaction();
	
	DSSObjectContext* lpObjectContext = mpGroupBySectionNode->getObjectContext();
	DSSGroupByUnits* lpGroupByUnits = mpGroupBySectionNode->getGroupByUnits();
	
	bool lAll = true;
	if (mpElement->getElementType()!=DssElementAll && mpElement->getElementType()!=DssElementReserved)
	{
		// check old element
		
		if (lpGroupByUnits && lpGroupByUnits->Count()>0)
		{
			DSSGroupByUnit* lpUnit = lpGroupByUnits->Item(0);
			if (lpUnit)
			{
				if (lpUnit->getIsReplaced())
				{
					// get element from AE
					
					DSSBaseElementProxy* lpOldElement = lpUnit->getReplacedElement();
					if (lpOldElement)
					{
						EnumDSSElementType lElementType = lpOldElement->getElementType();
						if (lElementType!=DssElementAll && lElementType!=DssElementReserved)
							lAll = false;
					}
				}
				else
				{
					// get element from definition
					
					DSSBaseElementsProxy* lpOldElements = lpGroupByUnits->Item(0)->getCurrentElements();
					if (lpOldElements && lpOldElements->Count()>0)
					{
						DSSBaseElementProxy* lpOldElement = lpOldElements->Item(0);
						if (lpOldElement)
						{
							EnumDSSElementType lElementType = lpOldElement->getElementType();
							if (lElementType!=DssElementAll && lElementType!=DssElementReserved)
								lAll = false;
						}
					}
					else
					{
						// Disable all, so we will always take the first non-all single element when unset
						CComVariant* lpVariant = lpObjectContext->getDocumentDefinition()->getPropertyValue("AnalyticalEngineProperties", "AutoSelectionForUnset");
						if (lpVariant && (*lpVariant==CComVariant(1) || *lpVariant==CComVariant(2)))
							lAll = false;
					}					
				}
			}
		}
	}
    else
        mStatus = DssSetGroupBySingleToAll;
    
    if(!lAll)
        mStatus = DssSetGroupBySingleToSingle;
    else if(mStatus == DssSetGroupByReserved) // not involved in the two conditions above
        mStatus = DssSetGroupByAllToSingle;
	
	if (!lpGroupByUnits)
		lpGroupByUnits = new DSSGroupByUnits(lpObjectContext);
	
	DSSGroupByUnit* lpGroupByUnit;	
	if (lpGroupByUnits->Count()>0)
		lpGroupByUnit = lpGroupByUnits->Item(0);
	else
	{
		lpGroupByUnit = new DSSGroupByUnit(lpObjectContext);
		lpGroupByUnits->Add(lpGroupByUnit);
	}
	
	DSSBaseElementProxy* lpElement = DSSBaseElementsProxy::Clone(mpElement, lpObjectContext);
	if (lpElement)
		lpGroupByUnit->SetCurrentElement(lpElement);
		
	lpGroupByUnit->setCurrentElementsStatus(DssCurrentElementDesired);
	lpGroupByUnit->setIsReplaced(false);
	
	hAddGroupBySectionNode(mpGroupBySectionNode);
	
	if (lAll) // all to single or single to all
	{
		mpRedrawParentNode = (DSSRWSectionNode*)(mpGroupBySectionNode->getParentNode());
		mRedrawChildIndex = mpGroupBySectionNode->getIndex();
	}
	else // single to single
	{
		mNodesModelChanged.push_back(mpGroupBySectionNode);
		for (int i=0; i<mpGroupBySectionNode->Count(); i++)
			hAddNodeModelChanged(mpGroupBySectionNode->Item(i), mNodeMapSelectionUpdate);
	}
	
	//2011-5-19 lcao for TQMS 477575, search the groupbySectionNode in all other layouts
	if (mAffectOtherLayout)
	{
		DSSRWSectionNode* pLayoutNode = mpGroupBySectionNode->getLayoutNode();
		DSSRWNode* pLayoutParent;
		if ( pLayoutNode && (pLayoutParent=pLayoutNode->getParentNode()) ) 
		{
			for (int i=0; i<pLayoutParent->Count(); i++) 
			{
				DSSRWNode* pChildNode = pLayoutParent->Item(i);
				if (pChildNode->getType()==DssRWNodeSection && ((DSSRWSectionNode*)pChildNode)->getSectionType()==DssRWSectionGeneric && pChildNode->getKey()!=pLayoutNode->getKey())
					FindSameGroupByUnit(pChildNode, lpGroupByUnit);
			}
		}
	}
	
	if (!mIsFromActionObject)
	{
        if(mAOXML == true)
        {
            GenerateAODeltaXML();
        }
        else
        {
            mpActionObject = new DSSAOGroupBy();
            ((DSSAOGroupBy*)(mpActionObject))->setNodeKey(mpGroupBySectionNode->getKey());
            ((DSSAOGroupBy*)(mpActionObject))->setElementID(mpElement->getElementID());
        }
	}
	
	return S_OK;
}

void DSSCManipulationSetGroupByElement::hAddGroupBySectionNode(DSSRWSectionNode* ipGroupBySectionNode)
{
	mGroupBySectionNodes.push_back(ipGroupBySectionNode);
	for (int i=0; i<ipGroupBySectionNode->Count(); i++)
	{
		DSSRWNode* lpChildNode = ipGroupBySectionNode->Item(i);
		if (lpChildNode->getType()==DssRWNodeSection)
		{
			DSSRWSectionNode* lpChildNodeSection = static_cast<DSSRWSectionNode*>(lpChildNode);
			if (lpChildNodeSection->getSectionType()==DssRWSectionGroupBySection)
				hAddGroupBySectionNode(lpChildNodeSection);
		}
	}
}

void DSSCManipulationSetGroupByElement::hAddNodeModelChanged(DSSRWNode* ipNode, NodeAndControlsMap& ioSelectionUpdateNodeMap)
{
	if (ipNode->getType()==DssRWNodeSection)
	{
		DSSRWSectionNode* lpSectionNode = (DSSRWSectionNode*)ipNode;
		bool lAll = false;
		
		if (lpSectionNode->getSectionType()==DssRWSectionGroupBySection)
		{
			lAll = true; 
			//always redraw 
			DSSGroupByUnits* lpGroupByUnits = ((DSSRWSectionNode*)(ipNode))->getGroupByUnits();
			
			if (lpGroupByUnits && lpGroupByUnits->Count()>0)
			{
				DSSGroupByUnit* lpUnit = lpGroupByUnits->Item(0);
				if (lpUnit)
				{
					if (lpUnit->getIsReplaced())
					{
						// get element from AE
						
						DSSBaseElementProxy* lpOldElement = lpUnit->getReplacedElement();
						if (lpOldElement)
						{
							EnumDSSElementType lElementType = lpOldElement->getElementType();
							if (lElementType!=DssElementAll && lElementType!=DssElementReserved)
								lAll = false;
						}
					}
					else
					{
						// get element from definition
						
						DSSBaseElementsProxy* lpOldElements = lpGroupByUnits->Item(0)->getCurrentElements();
						if (lpOldElements && lpOldElements->Count()>0)
						{
							DSSBaseElementProxy* lpOldElement = lpOldElements->Item(0);
							if (lpOldElement)
							{
								EnumDSSElementType lElementType = lpOldElement->getElementType();
								if (lElementType!=DssElementAll && lElementType!=DssElementReserved)
									lAll = false;
							}
						}
						else
						{
							CComVariant* lpVariant = ipNode->getObjectContext()->getDocumentDefinition()->getPropertyValue("AnalyticalEngineProperties", "AutoSelectionForUnset");
							if (lpVariant && (*lpVariant==CComVariant(1) || *lpVariant==CComVariant(2)))
								lAll = false;
						}					
					}
				}
			}
		}
			
		if (lAll) // NGB all to single, need redraw
		{
			mpRedrawParentNode = (DSSRWSectionNode*)(ipNode->getParentNode());
			mRedrawChildIndex = ipNode->getIndex();
		}
		else
		{
			// single to single, don't need redraw, need model changed
			
			mNodesModelChanged.push_back(ipNode);
					
			if (lpSectionNode->getSectionType()==DssRWSectionControlGroupBySection || lpSectionNode->getSectionType()==DssRWSectionUnitCondition)
			{
				if (lpSectionNode->getGroupByUnits() && lpSectionNode->getGroupByUnits()->Count()>0 && lpSectionNode->getGroupByUnits()->Item(0)->getCurrentElementsStatus()!=DssCurrentElementChosen)
				{
					// need selection changed on possible control node or template node
					
					MapFromTargetToControl::iterator iter = ipNode->getObjectContext()->getMapFromTargetToControl()->find(ipNode->getKey());
		
					if (iter!=ipNode->getObjectContext()->getMapFromTargetToControl()->end())
					{
						
						// (*iter).second is a NodeAndControlsVector
						
						NodeAndControlsVector::iterator iter2;
						
						for ( iter2=(*iter).second.begin() ; iter2 != (*iter).second.end(); iter2++ )
						{
							DSSRWNode* lpSourceNode = ipNode->getObjectContext()->getNode((*iter2)->NodeKey);
							
							// add control node or template node into ioSelectionUpdateNodeMap
							if (lpSourceNode->getType()==DssRWNodeControl)
							{
								// control node
								ioSelectionUpdateNodeMap[(*iter2)->NodeKey] = *iter2;
							}
							else if (lpSourceNode->getType()==DssRWNodeTemplate)
							{
								// template node
							
								NodeAndControlsMap::iterator iter3 = ioSelectionUpdateNodeMap.find((*iter2)->NodeKey);
								if (iter3==ioSelectionUpdateNodeMap.end())
								{
									NodeAndControls* lpNodeAndControls = new NodeAndControls();
									lpNodeAndControls->NodeKey = (*iter2)->NodeKey;
									lpNodeAndControls->ControlKeys = (*iter2)->ControlKeys;
									ioSelectionUpdateNodeMap[(*iter2)->NodeKey] = lpNodeAndControls;
								}
								else
								{
									std::vector<std::string>::iterator iter4;
									for ( iter4=(*iter2)->ControlKeys.begin() ; iter4 != (*iter2)->ControlKeys.end(); iter4++ )
									{
										std::vector<std::string>::iterator iter5;
										for ( iter5=(*iter3).second->ControlKeys.begin() ; iter5 != (*iter3).second->ControlKeys.end(); iter5++ )
											if (*iter4==*iter5)
												break;
										if (iter5==(*iter3).second->ControlKeys.end())
											(*iter3).second->ControlKeys.push_back(*iter4);
									}
								}
							}
						}
					}
				}
			}
			
			for (int i=0; i<ipNode->Count(); i++)
				hAddNodeModelChanged(ipNode->Item(i), ioSelectionUpdateNodeMap);
		}
	}	
	else
		mNodesModelChanged.push_back(ipNode);
}

NodeVectorGroupBy* DSSCManipulationSetGroupByElement::getGroupBySectionNodes()
{
	return &mGroupBySectionNodes;
}

DSSRWSectionNode* DSSCManipulationSetGroupByElement::getRedrawParentNode()
{
	return mpRedrawParentNode;
}

int DSSCManipulationSetGroupByElement::getRedrawChildIndex()
{
	return mRedrawChildIndex;
}

NodeVectorModelChanged* DSSCManipulationSetGroupByElement::getNodesModelChanged()
{
	return &mNodesModelChanged;
}

NodeAndControlsMap* DSSCManipulationSetGroupByElement::getNodeMapSelectionUpdate()
{
	return &mNodeMapSelectionUpdate;
}

std::string DSSCManipulationSetGroupByElement::getGroupByNodeKey()
{
	return mpGroupBySectionNode->getKey();
}

void DSSCManipulationSetGroupByElement::FindSameGroupByUnit(DSSRWNode* ipNode, DSSGroupByUnit* ipGroupByUnit)
{
	bool lSet = false;
	if (ipNode->getType() == DssRWNodeSection && ((DSSRWSectionNode*)ipNode)->getSectionType()==DssRWSectionGroupBySection) 
	{
		DSSGroupByUnits* lpGroupByUnits = ((DSSRWSectionNode*)ipNode)->getGroupByUnits();
		if (!lpGroupByUnits || lpGroupByUnits->Count()<=0)
			return;
		DSSGroupByUnit* lpGroupByUnit = lpGroupByUnits->Item(0);
		if (lpGroupByUnit->getObjectID() == ipGroupByUnit->getObjectID())
		{
			DSSBaseElementsProxy* lpElements = ipGroupByUnit->getCurrentElements();
			for (int i=0; i<lpElements->Count(); i++) 
			{
				if (i==0)
					lpGroupByUnit->SetCurrentElement(lpElements->Item(i));
				else 
					lpGroupByUnit->AddCurrentElement(lpElements->Item(i));
			}
			lpGroupByUnit->setCurrentElementsStatus(ipGroupByUnit->getCurrentElementsStatus());
			lpGroupByUnit->setIsReplaced(ipGroupByUnit->getIsReplaced());
			lSet = true;
		}
	}
	
	if (!lSet)
	{
		for (int i=0; i<ipNode->Count(); i++) 
			FindSameGroupByUnit(ipNode->Item(i), ipGroupByUnit);
	}
}
#ifndef __ANDROID__
void DSSCManipulationSetGroupByElement::GenerateAODeltaXML()
{
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("44", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    
    int lTreeType = static_cast<int>(mpGroupBySectionNode->getTreeType());
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpGroupBySectionNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    
    int lElementStatus = static_cast<int>(mpGroupBySectionNode->getGroupByUnits()->Item(0)->getCurrentElementsStatus());
    SimpleXMLNode lElementStatusXML(SimpleXMLBuilder::CreateNodePtr("element_status"));
    SimpleXMLNode lElementStatusXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lElementStatus).c_str(), true));
    lElementStatusXML.AddChild(lElementStatusXMLContent);
    
    char lElementID[33];
    DSSBaseElementProxy::ConvertGUIDToString(mpElement->getObjectID(), lElementID);
    SimpleXMLNode lElementIDXML(SimpleXMLBuilder::CreateNodePtr("element_id"));
    SimpleXMLNode lElementIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lElementID, true));
    lElementIDXML.AddChild(lElementIDXMLContent);
    
    lXML.AddChild(lManipulationMethodXML);
    lXML.AddChild(lTreeTypeXML);
    lXML.AddChild(lNodeKeyXML);
    lXML.AddChild(lElementStatusXML);
    lXML.AddChild(lElementIDXML);
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    
    mpActionObject = new DSSAODeltaXML();
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationSetGroupByElement::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
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
                mpGroupBySectionNode = static_cast<DSSRWSectionNode*>(ipContext->getNode(lStrContent));
            }
            else if(lCurrentNode.GetName().compare("element_id") == 0)
            {
                lCurrentNode.GetContent(lStrContent);
                mpElement = new DSSBaseElementProxy();
                mpElement->Parse(lStrContent);
            }
        }
    }
}
#else
void DSSCManipulationSetGroupByElement::GenerateAODeltaXML()
{
    // remove it temporarily to avoid build break - dli, 13/08/20
    /*
    TiXmlDocument lDoc;
    TiXmlElement* lpRootElement = new TiXmlElement("rw_manipulation");
    
    TiXmlElement* lpManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
    TiXmlText* lpManipulationMethodText =  new TiXmlText("44");
    lpManipulationMethodElement->LinkEndChild(lpManipulationMethodText);
    
    int lTreeType = static_cast<int>(mpPanel->getTreeType());
    TiXmlElement* lpTreeTypeElement = new TiXmlElement("rw_tree_type");
    TiXmlText* lpTreeTypeText = new TiXmlText(DSSExpNode::IntToString(lTreeType).c_str());
    lpTreeTypeElement->LinkEndChild(lpTreeTypeText);
    
    TiXmlElement* lpNodeKeyElement = new TiXmlElement("rw_node_key");
    TiXmlText* lpNodeKeyText = new TiXmlText(mpGroupBySectionNode->getKey().c_str());
    lpNodeKeyElement->LinkEndChild(lpNodeKeyText);
    
    int lElementStatus = static_cast<int>(mpGroupBySectionNode->getGroupByUnits->Item(0)->getCurrentElementStatus());
    TiXmlElement* lpElementStatusElement = new TiXmlElement("element_status");
    TiXmlText* lpElementStatusText = new TiXmlText(DSSExpNode::IntToString(lElementStatus).c_str());
    lpElementStatusElement->LinkEndChild(lpElementStatusText);
    
    char lElementID[33];
    DSSBaseElementProxy::ConvertGUIDToString(mpElement->getObjectID(), lElementID);
    TiXmlElement* lpElementIDElement = new TiXmlElement("element_id");
    TiXmlText* lpElementIDText = new TiXmlText(lElementID);
    lpElementIDElement->LinkEndChild(lpElementIDText);
    
    lpRootElement->LinkEndChild(lpManipulationMethodElement);
    lpRootElement->LinkEndChild(lpTreeTypeElement);
    lpRootElement->LinkEndChild(lpNodeKeyElement);
    lpRootElement->LinkEndChild(lpElementStatusElement);
    lpRootElement->LinkEndChild(lpElementIDElement);
    
    TiXmlPrinter lPrinter;
    lPrinter.SetIndent(NULL);
    lPrinter.SetLineBreak(NULL);
    lDoc.Accept(&lPrinter);
    
    mpActionObject = new DSSAODeltaXML();
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lPrinter.Str());
     */
}
#endif

EnumDSSSetGroupbyStatus DSSCManipulationSetGroupByElement::getStatus()
{
    return mStatus;
}

// lishan manipulation
DSSRWSectionNode* DSSCManipulationSetGroupByElement::getGroupBySectionNode()
{
    return mpGroupBySectionNode;
}

DSSBaseElementProxy* DSSCManipulationSetGroupByElement::getElement()
{
    return mpElement;
}