/*
 *  DSSManipulationSetActionOnControl.cpp
 *  ReportServiceCore
 *
 *  Created by Li Jing on 11-3-07.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSCManipulationSetActionOnControl.h"
#include "DSSRWControlNode.h"
#include "DSSRWControl.h"
#include "DSSRWSectionNode.h"
#include "DSSRWTemplateNode.h"
#include "DSSRWFieldGroupNode.h"
#include "DSSRWNode.h"
#include "DSSDataModelXTabImpl.h"
#include "DSSDataModelFieldGroupImpl.h"
#include "DSSDataModelTransaction.h"
#import "DSSRWGridModel.h"
#include "DSSAOSetActionOnControl.h"

#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#include "DSSBaseElementProxy.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif

DSSCManipulationSetActionOnControl::DSSCManipulationSetActionOnControl(DSSRWControlNode* ipControlNode, DSSRWControl* ipControl, EnumDSSRWControlActionType iAction)
{
	mpControlNode = ipControlNode;
	mpControl = ipControl;
	mAction = iAction;
	mIsFromActionObject = false;
    
    mType = ManipulationSetActionOnControl;
}

DSSCManipulationSetActionOnControl::DSSCManipulationSetActionOnControl(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationSetActionOnControl;
    
	DSSAOSetActionOnControl* lpAO = static_cast<DSSAOSetActionOnControl*> (ipAO);
	mpControlNode = (DSSRWControlNode*)(ipContext->getNode(lpAO->getNodeKey()));
	mpControl = ipContext->getControl(lpAO->getCtrlKey());
	mAction = lpAO->getActionType();
	mIsFromActionObject = true;
}

DSSCManipulationSetActionOnControl::~DSSCManipulationSetActionOnControl()
{
}

int DSSCManipulationSetActionOnControl::Execute()
{
    if (hCheckLayoutLoaded(mpControlNode)==S_FALSE)
		return S_FALSE;

	if (!mpControl)
		return S_OK;

	EnumDSSRWControlType lType = mpControl->getType();
	if (lType != DssRWControlTypeAction)
		return S_OK;
	
	// We only handle Discard and Recalcuate
	if (! ((mAction & DssRWControlActionTypeDiscard) || (mAction & DssRWControlActionTypeRecalculate)))
		return S_OK;
	
	DSSObjectContext* lpObjectContext = mpControlNode->getObjectContext();

	int lTargetCount = mpControl->getCountTargets();
	for (int i=0; i<lTargetCount; i++)
	{
		std::string lTargetKey = mpControl->getTargetKey(i);
		DSSRWNode* lpTargetNode = lpObjectContext->getNode(lTargetKey);
		
		if (!lpTargetNode)
			continue;	// Not find, skip
		
		hSetActionOnNode(lpTargetNode);
	}
	
	if (!mIsFromActionObject)
	{
		mpActionObject = new DSSAOSetActionOnControl();
		((DSSAOSetActionOnControl*)mpActionObject)->setCtrlKey(mpControl->getKey());
		((DSSAOSetActionOnControl*)mpActionObject)->setNodeKey(mpControlNode->getKey());
		((DSSAOSetActionOnControl*)mpActionObject)->setActionType(mAction);
	}
	
	return S_OK;
}

void DSSCManipulationSetActionOnControl::hSetActionOnNode(DSSRWNode *ipRWNode)
{
	if (! ipRWNode)
		return;
	
	EnumDSSRWNodeType lNodeType = ipRWNode->getType();
	
	switch (lNodeType) {
		case DssRWNodeSection:
		{
			int lChildCount = ipRWNode->Count();
			for (int i=0; i<lChildCount; i++)
			{
				DSSRWNode * lpChildNode = ipRWNode->Item(i);
				
				hSetActionOnNode(lpChildNode);
			}
		}
		break;
			
		case DssRWNodeTemplate:
		{
			DSSRWTemplateNode * lpRWTemplateNode = (DSSRWTemplateNode*)(ipRWNode);
			if (lpRWTemplateNode->IsTransactionEnabled())
			{
				DSSTemplate * lpTemplate = lpRWTemplateNode->getTemplate();
				IDSSTemplateModel *lpTemplateDataModel = lpTemplate->getTemplateModelInterface();
				
                DSSDataModelTransaction *lpTransactionModel = lpTemplate->getTransactionDataModel();
				if (lpTemplateDataModel)
				{
					if (mAction & DssRWControlActionTypeDiscard)
					{
						if (lpTransactionModel)
							lpTransactionModel->Discard();	// discard the tranaction data change
					}
					
					lpTemplateDataModel->ReCalculate();
					
					mNodesModelChanged.push_back(ipRWNode);
				}
			}
			
			break;
		}
			
		case DssRWNodeFieldGroup:
		{
			DSSRWFieldGroupNode * lpRWFieldGroupNode = (DSSRWFieldGroupNode*)(ipRWNode);
			if (lpRWFieldGroupNode->IsTransactionEnabled())
			{
				{
					if (mAction & DssRWControlActionTypeDiscard)
					{
						DSSDataModelTransaction *lpTransactionModel = lpRWFieldGroupNode->getTransactionDataModel();

						if (lpTransactionModel)
							lpTransactionModel->Discard();	// discard the tranaction data change
					}
					
					mNodesModelChanged.push_back(ipRWNode);
				}
			}
			
			break;
		}
			
		default:
			break;
	}
	
	return;
}

DSSRWControlNode* DSSCManipulationSetActionOnControl::getControlNode()
{
	return mpControlNode;
}

DSSRWControl* DSSCManipulationSetActionOnControl::getControl()
{
	return mpControl;
}

EnumDSSRWControlActionType DSSCManipulationSetActionOnControl::getAction()
{
	return mAction;
}
#ifndef __ANDROID__
void DSSCManipulationSetActionOnControl::GenerateAODeltaXML()
{
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("108"));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    
    int lTreeType = static_cast<int>(mpControlNode->getTreeType());
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpControlNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    
    SimpleXMLNode lControlKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_control_key"));
    SimpleXMLNode lControlKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpControl->getKey().c_str(), true));
    lControlKeyXML.AddChild(lControlKeyXMLContent);
    
    int lAction = static_cast<int>(mAction);
    SimpleXMLNode lActionXML(SimpleXMLBuilder::CreateNodePtr("flags"));
    SimpleXMLNode lActionXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lAction).c_str(), true));
    lActionXML.AddChild(lActionXMLContent);
    
    lXML.AddChild(lManipulationMethodXML);
    lXML.AddChild(lTreeTypeXML);
    lXML.AddChild(lNodeKeyXML);
    lXML.AddChild(lControlKeyXML);
    lXML.AddChild(lActionXML);
    
    SimpleXMLNode lRoot = lBuilder.GetRoot();
    lRoot.AddChild(lXML);
    mpActionObject = new DSSAODeltaXML();
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationSetActionOnControl::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser;
    if(!lParser.IsValid())
        return;
    
    SimpleXMLNode lCurrentNode = lParser.GetRoot();
    if(!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation"))
        return;
    
    for(lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if(lCurrentNode.IsElementNode())
        {
            std::string lStrContent;
            if(lCurrentNode.GetName().compare("rw_node_key") == 0)
            {
                lCurrentNode.GetContent(lStrContent);
                mpControlNode = static_cast<DSSRWControlNode*>(ipContext->getNode(lStrContent));
            }
            else if(lCurrentNode.GetName().compare("rw_control_key") == 0)
            {
                lCurrentNode.GetContent(lStrContent);
                mpControl = ipContext->getControl(lStrContent);
            }
            else if(lCurrentNode.GetName().compare("flags") == 0)
            {
                lCurrentNode.GetContent(lStrContent);
                mAction = static_cast<EnumDSSRWControlActionType>(atoi(lStrContent.c_str()));
            }
        }
    }
}
#else
void DSSCManipulationSetActionOnControl::GenerateAODeltaXML()
{
    TiXmlDocument lDoc;
    TiXmlElement* lpRootElement = new TiXmlElement("rw_manipulation");
    
    TiXmlElement* lpManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
    TiXmlText* lpManipulationMethodText = new TiXmlText("108");
    lpManipulationMethodElement->LinkEndChild(lpManipulationMethodText);
    
    int lTreeType = static_cast<int>(mpControlNode->getTreeType());
    TiXmlElement* lpTreeTypeElement = new TiXmlElement("rw_tree_type");
    TiXmlText* lpTreeTypeText = new TiXmlText(DSSExpNode::IntToString(lTreeType).c_str());
    lpTreeTypeElement->LinkEndChild(lpTreeTypeText);
    
    TiXmlElement* lpNodeKeyElement = new TiXmlElement("rw_node_key");
    TiXmlText* lpNodeKeyText = new TiXmlText(mpControlNode->getKey().c_str());
    lpNodeKeyElement->LinkEndChild(lpNodeKeyText);
    
    TiXmlElement* lpControlKeyElement = new TiXmlElement("rw_control_key");
    TiXmlText* lpControlKeyText = new TiXmlText(mpControl->getKey().c_str());
    lpControlKeyElement->LinkEndChild(lpControlKeyText);
    
    int lAction = static_cast<int>(mAction);
    TiXmlElement* lpActionElement = new TiXmlElement("flags");
    TiXmlText* lpActionText = new TiXmlText(DSSExpNode::IntToString(lAction).c_str());
    lpActionElement->LinkEndChild(lpActionText);
    
    lpRootElement->LinkEndChild(lpManipulationMethodElement);
    lpRootElement->LinkEndChild(lpTreeTypeElement);
    lpRootElement->LinkEndChild(lpNodeKeyElement);
    lpRootElement->LinkEndChild(lpControlKeyElement);
    lpRootElement->LinkEndChild(lpActionElement);
    
    lDoc.LinkEndChild(lpRootElement);
    TiXmlPrinter lPrinter;
    lPrinter.SetIndent(NULL);
    lPrinter.SetLineBreak(NULL);
    lDoc.Accept(&lPrinter);
    
    mpActionObject = new DSSAODeltaXML();
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lPrinter.Str()); 
}
#endif
