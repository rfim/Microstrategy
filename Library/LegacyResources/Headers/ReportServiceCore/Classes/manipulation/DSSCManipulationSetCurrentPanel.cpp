/*
 *  DSSCManipulationSetCurrentPanel.cpp
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-11-3.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSCManipulationSetCurrentPanel.h"
#include "DSSDocumentDefinition.h"
#include "DSSRWSectionNode.h"
#include "DSSAOPanel.h"

#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxml.h"
#else
#include "DSSSimpleXML.h"
#endif

DSSCManipulationSetCurrentPanel::DSSCManipulationSetCurrentPanel(DSSRWSectionNode* ipPanel, bool iAOXML)
{
	mpPanel = ipPanel;
    mAOXML = iAOXML;
	mpPanelStack = (DSSRWSectionNode*)(mpPanel->getParentNode()); // 489854 Initialize mpPanelStack in every constructor.
	mIsFromActionObject = false;
    
    mType = ManipulationSetCurrentPanel;
}

DSSCManipulationSetCurrentPanel::DSSCManipulationSetCurrentPanel(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationSetCurrentPanel;
    
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
        DSSAOPanel* lpAO = static_cast<DSSAOPanel*>(ipAO);
        mpPanel = (DSSRWSectionNode*)(ipContext->getNode(lpAO->getPanelKey()));
        mpPanelStack = (DSSRWSectionNode*)(mpPanel->getParentNode());
        mpActionObject = NULL;
        mIsFromActionObject = true;
    }
}

DSSCManipulationSetCurrentPanel::~DSSCManipulationSetCurrentPanel()
{}

int DSSCManipulationSetCurrentPanel::Execute()
{
	if (hCheckLayoutLoaded(mpPanel)==S_FALSE)
		return S_FALSE;
	
	std::string lKey = mpPanel->getKey();
	DSSRWSectionNode* lpPanelStackNode = (DSSRWSectionNode*)(mpPanel->getParentNode());
	if (lpPanelStackNode && lpPanelStackNode->getCurrentNodeKey()!=lKey)
	{
		lpPanelStackNode->setCurrentNodeKey(lKey);
		lpPanelStackNode->setCurrentNodeIndex(mpPanel->getIndex());
		mpPanelStack = lpPanelStackNode;
        
        mNodesModelChanged.push_back(lpPanelStackNode);
        if(!mpPanel->isNodeLoaded() && mpPanel->CanHaveDataModel())
        {
            mNodeMapServer[lKey] = mpPanel;
            mNodesModelChanged.push_back(mpPanel);
        }
		
		if (!mIsFromActionObject)
		{
            if(mAOXML == true)
            {
                GenerateAODeltaXML();
            }
			else
            {   mpActionObject = new DSSAOPanel(ManipulationSetCurrentPanel);
                ((DSSAOPanel*)(mpActionObject))->setPanelKey(lKey);
                ((DSSAOPanel*)(mpActionObject))->setPanelStackKey(mpPanelStack->getKey());
                std::string lUnitKeyContext = DSSExpNode::IntToString(static_cast<int>(mpPanel->getTreeType()));
                lUnitKeyContext += "\x1E";
                lUnitKeyContext += mpPanelStack->getKey();
                ((DSSAOPanel*)(mpActionObject))->setUnitContextKey(lUnitKeyContext);
            }
		}
	}
	
	return S_OK;
}

DSSRWNode* DSSCManipulationSetCurrentPanel::getPanelStack()
{
	return mpPanelStack;
}

#ifndef __ANDROID__
void DSSCManipulationSetCurrentPanel::GenerateAODeltaXML()
{
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("72", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    
    int lTreeType = static_cast<int>(mpPanel->getTreeType());
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpPanel->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    
    lXML.AddChild(lManipulationMethodXML);
    lXML.AddChild(lTreeTypeXML);
    lXML.AddChild(lNodeKeyXML);
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    
    mpActionObject = new DSSAODeltaXML();
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationSetCurrentPanel::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
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
                mpPanel = static_cast<DSSRWSectionNode*>(ipContext->getNode(lStrContent));
                mpPanelStack = (DSSRWSectionNode*)(mpPanel->getParentNode()); 
            }
        }
    }
}
#else
void DSSCManipulationSetCurrentPanel::GenerateAODeltaXML()
{
    TiXmlDocument lDoc;
    TiXmlElement* lpRootElement = new TiXmlElement("rw_manipulation");
    
    TiXmlElement* lpManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
    TiXmlText* lpManipulationMethodText =  new TiXmlText("72");
    lpManipulationMethodElement->LinkEndChild(lpManipulationMethodText);
    
    int lTreeType = static_cast<int>(mpPanel->getTreeType());
    TiXmlElement* lpTreeTypeElement = new TiXmlElement("rw_tree_type");
    TiXmlText* lpTreeTypeText = new TiXmlText(DSSExpNode::IntToString(lTreeType).c_str());
    lpTreeTypeElement->LinkEndChild(lpTreeTypeText);
    
    TiXmlElement* lpNodeKeyElement = new TiXmlElement("rw_node_key");
    TiXmlText* lpNodeKeyText = new TiXmlText(mpPanel->getKey().c_str());
    lpNodeKeyElement->LinkEndChild(lpNodeKeyText);
    
    lpRootElement->LinkEndChild(lpManipulationMethodElement);
    lpRootElement->LinkEndChild(lpTreeTypeElement);
    lpRootElement->LinkEndChild(lpNodeKeyElement);
    
    TiXmlPrinter lPrinter;
    lPrinter.SetIndent(NULL);
    lPrinter.SetLineBreak(NULL);
    lDoc.Accept(&lPrinter);
    
    mpActionObject = new DSSAODeltaXML();
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lPrinter.Str());
}
#endif
