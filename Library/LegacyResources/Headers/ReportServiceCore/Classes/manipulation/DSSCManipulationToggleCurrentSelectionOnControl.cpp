/*
 *  DSSCManipulationToggleCurrentSelectionOnControl.cpp
 *  ReportServiceCore
 *
 *  Created by lcao on 3/28/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSCManipulationToggleCurrentSelectionOnControl.h"
#include "DSSRWControlNode.h"
#include "DSSAOSelection.h"
#include "DSSAODeltaXML.h"
#include "DSSSimpleXML.h"
#include "DSSCExpression.h"
#include "DSSExpNodeOperator.h"
#include "DSSExpNodeShortcut.h"
#include "DSSExpNodeConstant.h"
#include "DSSExpNodeTime.h"
#include "DSSExpNodeElementsObject.h"
#include "DSSConcreteElementProxy.h"
#include "DSSDocumentDefinition.h"

DSSCManipulationToggleCurrentSelectionOnControl::DSSCManipulationToggleCurrentSelectionOnControl(DSSRWControlNode* ipControlNode)
{
	mpControlNode = ipControlNode;
	mpControl = NULL;
	mpElements = NULL;
	mpExpression = NULL;
    mpRedrawParentNode = NULL;
    mRedrawChildIndex = -1;
	
	DSSRWControls* lpControls = mpControlNode->getControls();
	if (lpControls && lpControls->Count()>0)
		mpControl = lpControls->Item(0);
	
	mIsFromActionObject = false;
    
    mType = ManipulationToggleCurrentSelectionOnControl;
}

DSSCManipulationToggleCurrentSelectionOnControl::DSSCManipulationToggleCurrentSelectionOnControl(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationToggleCurrentSelectionOnControl;
	mpElements = NULL;
	mpExpression = NULL;
    mpRedrawParentNode = NULL;
    mRedrawChildIndex = -1;
	
	DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
	std::string lXMLString = lpAO->getXML();
	
	// parse lXMLString and construct the mpControlNode and mpControl
	hParseExpressionFromXML(lXMLString, ipContext);
	
	mIsFromActionObject = true;
}

void DSSCManipulationToggleCurrentSelectionOnControl::hParseExpressionFromXML(std::string& iXMLString, DSSObjectContext* ipContext)
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
		}
	}
}

int DSSCManipulationToggleCurrentSelectionOnControl::Execute()
{
	if (hCheckLayoutLoaded(mpControlNode)==S_FALSE)
		return S_FALSE;

	int i;
	if (!mpControl)
		return S_OK;
    
    bool isLayoutPageHeaderOrFooter = false;
    DSSRWNode* lpLayoutNode = mpControlNode->getLayout();
    if (lpLayoutNode->getType()==DssRWNodeSection && (((DSSRWSectionNode*)lpLayoutNode)->getSectionType()==DssRWSectionPageHeader || ((DSSRWSectionNode*)lpLayoutNode)->getSectionType()==DssRWSectionPageFooter))
        isLayoutPageHeaderOrFooter = true;
    
    int ltgtCnt = mpControl->getCountTargets();
    for (i=0; i<ltgtCnt; i++)
    {
        std::string lstrTgtKey = mpControl->getTargetKey(i);
        DSSObjectContext* lpObjCntxt = mpControlNode->getObjectContext();
        if (isLayoutPageHeaderOrFooter && hCheckLayoutLoaded(lpObjCntxt->getNode(lstrTgtKey))==S_FALSE)
            return S_FALSE;
    }
	
	std::vector<DSSRWNode*> lNodesModelChanged;
	DSSObjectContext* lpObjectContext = mpControlNode->getObjectContext();
	
	if (mpControl->getExpression()==NULL || mpControl->getExpression()->Root()==NULL)
		mpExpression = NULL;
	else 
		mpExpression = mpControl->getExpression();
	
	if (mpControl->getCurrentElements()==NULL || (mpExpression && mpControl->getCurrentElements()->Count()==0))
		mpElements = NULL;
	else 
		mpElements = mpControl->getCurrentElements();
	/*{
		if(!mpElements)
			mpElements = new DSSBaseElementsProxy ();
		for (i=0; i<mpControl->getCurrentElements()->Count(); i++) 
		{
			mpElements->AddElement(mpControl->getCurrentElements()->Item(i));
		}
	}*/
	
	if (!mpElements && !mpExpression)
		return S_OK;
	
	if (mpElements) //toggle elements to expression
	{
		if (mpElements->Count()>0 && mpElements->Item(0)->getElementType()==DssElementAll) 
			return S_OK;
		else 
		{
			mpExpression = new DSSCExpression (lpObjectContext);
			DSSExpNodeOperator* lpOperatorNode = new DSSExpNodeOperator(lpObjectContext, "", mpExpression);
			lpOperatorNode->setExpressionType(DssFilterListQual); 
			lpOperatorNode->setOperatorType(DssFunctionNotIn);
			
			DSSExpNodeShortcut* lpShortcutNode = (DSSExpNodeShortcut*)lpOperatorNode->Add(DssNodeShortcut,"");
			lpShortcutNode->setTareget(mpControl->getSourceObject());
			
			DSSExpNodeElementsObject* elementsNode = (DSSExpNodeElementsObject*)lpOperatorNode->Add(DssNodeElementsObject, "");
			DSSBaseElementsProxy* lpElements = new DSSBaseElementsProxy(lpObjectContext);
			for (i=0; i<mpElements->Count(); i++) 
			{
				DSSBaseElementProxy* lpBaseElement = DSSBaseElementsProxy::Clone(mpElements->Item(i), lpObjectContext);
				if (lpBaseElement) 
					lpElements->AddElement(lpBaseElement);
			}
			elementsNode->setElements(lpElements);
			
			mpExpression->setRoot(lpOperatorNode);
			mpExpression->setExpresssionType(DssFilterListQual);
			
			mpControl->ClearCurrentElements();
			mpControl->setExpression(mpExpression);
			mpElements = NULL;
		}
	}
	else if (mpExpression)	//toggle expression to elements
	{
		DSSExpNode* rootExpNode = mpExpression->Root();
		if (rootExpNode->getType() == DssNodeOperator)
		{
			if ( ((DSSExpNodeOperator*)(rootExpNode))->getOperatorType() == DssFunctionIn ) 
				((DSSExpNodeOperator*)(rootExpNode))->setOperatorType(DssFunctionNotIn);
			else if (((DSSExpNodeOperator*)(rootExpNode))->getOperatorType() == DssFunctionNotIn)
			{
				mpElements = mpControl->getCurrentElements();
				if (!mpElements)
					mpElements = new DSSBaseElementsProxy(lpObjectContext);
				else
					mpElements->Clear();

				DSSExpNodeElementsObject* elementNode = (DSSExpNodeElementsObject*)rootExpNode->Item(1);	//get the elementsNode
				DSSBaseElementsProxy* lpInputElements = elementNode->getElements();
				for (i=0; i<lpInputElements->Count(); i++) 
				{
					DSSBaseElementProxy* lpBaseElement = DSSBaseElementsProxy::Clone(lpInputElements->Item(i), lpObjectContext);
					if (lpBaseElement) 
						mpElements->AddElement(lpBaseElement);
				}
				mpControl->setCurrentElements(mpElements);
				mpControl->setExpression(NULL);
				mpExpression = NULL;
			}
		}
	}
	
	// Process for different control type
	EnumDSSRWControlType lType = mpControl->getType();
	int lTargetCount;
	
	switch (lType) {
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
				DSSRWNode* lpTargetNode = lTargets[i];
				if (!lpTargetNode || lpTargetNode->getType()!=DssRWNodeSection)
					continue;
				DSSRWSectionNode* lpSectionNode = (DSSRWSectionNode*)(lpTargetNode);
				
				// 4/27/2011 For element list selector change, we need to drop transaction data if any for all the targets
				if (lType == DssRWControlTypeElementList)
				{
					lpSectionNode->DiscardTransaction();
				}
				
				lpSectionNode->setLatestControlKey(mpControl->getKey());
				
				if (lpSectionNode->getSectionType()!=DssRWSectionControlGroupBySection && lpSectionNode->getSectionType()!=DssRWSectionUnitCondition)
					continue;
				DSSGroupByUnits* lpUnits = lpSectionNode->getGroupByUnits();
				if (!lpUnits || lpUnits->Count()==0)
					continue;
				DSSGroupByUnit* lpUnit = lpUnits->Item(0);
				if (!lpUnit)
					continue;
				
				if (mpExpression) //NotIn or DSSMetric Condition.
				{
					DSSCExpression* lpExpression = new DSSCExpression(lpObjectContext);
					lpExpression->Clone(mpExpression);  //< make a copy of mpExpression.
					lpUnit->setExpression(lpExpression);
					lpUnit->ClearCurrentElements();
				}
				else 
				{
					lpUnit->ClearCurrentElements();
					for (int j=0; j<mpElements->Count(); j++)
					{
						DSSBaseElementProxy* lpElement = DSSBaseElementsProxy::Clone(mpElements->Item(j), lpObjectContext);
						if (lpElement)
							lpUnit->AddCurrentElement(lpElement);
					}
					lpUnit->setExpression(NULL);
				}
				
				lpUnit->setCurrentElementsStatus(mpControl->getCurrentElementsStatus());
				
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
                    
                    if (mpExpression) //NotIn or DSSMetric Condition.
                    {
                        DSSCExpression* lpExpression = new DSSCExpression(lpObjectContext);
                        lpExpression->Clone(mpExpression);  //< make a copy of mpExpression.
                        lpUnit->setExpression(lpExpression);
                        lpUnit->ClearCurrentElements();
                    }
                    else
                    {
                        lpUnit->ClearCurrentElements();
                        for (int j=0; j<mpElements->Count(); j++)
                        {
                            DSSBaseElementProxy* lpElement = DSSBaseElementsProxy::Clone(mpElements->Item(j), lpObjectContext);
                            if (lpElement)
                                lpUnit->AddCurrentElement(lpElement);
                        }
                        lpUnit->setExpression(NULL);
                    }
                    lpUnit->setCurrentElementsStatus(mpControl->getCurrentElementsStatus());
                    
                    DSSRWNode* lTargetNodeDDIC = lpObjectContext->getNode(lpTargetControl->getNodeKeyDDIC());
                    if(lTargetNodeDDIC)
                        mNodesModelChanged.push_back(lTargetNodeDDIC);
                }
                
                if (lpTargetControl->getFilterType() == EnumDSSRWControlFilterTypeUnitCondition)
                    mControlsModelChanged.push_back(lpTargetControl);
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
						}
					}
				}
			}
			break;
		default:
			return S_OK;
	}
	
	if (!mIsFromActionObject)
	{
		mpActionObject = new DSSAODeltaXML(ManipulationToggleCurrentSelectionOnControl);
		std::string lXML = hBuildExpressionXML();	
		printf("xml:%s\n", lXML.c_str());
		(static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lXML);
		
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
		mNodesModelChanged.push_back(mpControlNode);
	
	std::vector<DSSRWNode*>::iterator it2 =lNodesModelChangedControl.begin();
	std::vector<DSSRWNode*>::iterator it3 =lNodesModelChangedControl.end();
	for (; it2 != it3; it2++ )
	{
		mNodesModelChanged.push_back(*it2);
	}
	
	// add metric condition selector nodes to mNodesModelChanged
	if (mpControl->getType()==DssRWControlTypeElementList && mpControl->getSourceObject() && mpControl->getSourceObject()->getType()!=DssTypeMetric)
	{
		DSSRWNode* lpParentNode = mpControlNode->getParentNode();
		DSSRWNode* lpNodeToExplore = hGetLowestNonGroupBySectionNode(lpParentNode);
		if (lpNodeToExplore)
			lpParentNode = lpNodeToExplore;
		AddMetricConditionSelectorsToMap(lpParentNode, mpControl->getKey());
	}
	
	return S_OK;
}

std::string DSSCManipulationToggleCurrentSelectionOnControl::hBuildExpressionXML()
{
	SimpleXMLBuilder lBuilder;
	SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
	
	SimpleXMLNode lManipulationXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
	SimpleXMLNode lManipulationXMLContent(SimpleXMLBuilder::CreateNodePtr("100", true));
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
	
	SimpleXMLNode lRoot(lBuilder.GetRoot());
	lRoot.AddChild(lXML);
	
	return lBuilder.ToString(false);
}

DSSExpNode* DSSCManipulationToggleCurrentSelectionOnControl::hBuildExpNode(SimpleXMLNode& iXMLNode, DSSExpNode* ipParentExpNode, DSSObjectContext* ipContext)
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

DSSCManipulationToggleCurrentSelectionOnControl::~DSSCManipulationToggleCurrentSelectionOnControl()
{
	NodeAndControlsMap::iterator it;
	for ( it=mNodeMapSelectionUpdate.begin() ; it != mNodeMapSelectionUpdate.end(); it++ )
	{
		if (mpControlNode->getObjectContext()->getNode((*it).second->NodeKey)->getType()==DssRWNodeTemplate)
			delete (*it).second;
	}
	
	if (mIsFromActionObject && mpExpression)	//2011-5-19 lcao change mpElements with mpExpression
	{
		delete mpExpression;
		mpExpression = NULL;
	}
}