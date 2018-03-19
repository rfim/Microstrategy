/*
 *  DSSCManipulationSetLayout.cpp
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-19.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSCManipulationSetLayout.h"
#include "DSSDocumentDefinition.h"
#include "DSSRWSectionNode.h"
#include "DSSAOLayout.h"

#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif

DSSCManipulationSetLayout::DSSCManipulationSetLayout(DSSRWSectionNode* ipRootNode, DSSRWSectionNode* ipLayoutNode, bool iAOXML)
{
	mpRootNode = ipRootNode;
	mpLayoutNode = ipLayoutNode;
	mNeedRedraw = false;
	mIsFromActionObject = false;
    mAOXML = iAOXML;
    
    mType = ManipulationSetLayout;
}

DSSCManipulationSetLayout::DSSCManipulationSetLayout(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationSetLayout;
    
    if(ipAO->IsDeltaXML())
    {
        DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
        std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
        hParseAOXML(lXMLString, ipContext);
        mpRootNode = static_cast<DSSRWSectionNode*>(mpLayoutNode->getParentNode());
#endif
    }
    else
    {
        DSSAOLayout* lpAO = static_cast<DSSAOLayout*>(ipAO);
        mpRootNode = (DSSRWSectionNode*)(ipContext->getNode(lpAO->getLayoutNodeKey())->getParentNode());
        mpLayoutNode = (DSSRWSectionNode*)(ipContext->getNode(lpAO->getLayoutNodeKey()));
        mNeedRedraw = false;
        mIsFromActionObject = true;
	}
}

DSSCManipulationSetLayout::~DSSCManipulationSetLayout()
{}

int DSSCManipulationSetLayout::Execute()
{
	if (hCheckLayoutLoaded(mpLayoutNode)==S_FALSE)
		return S_FALSE;
	
	std::string lNewKey = mpLayoutNode->getKey();
	DSSDocumentDefinition* lpDefn = mpRootNode->getObjectContext()->getDocumentDefinition();
	// 475914 In replayManipulation, only the last set layout manipulation will be applied. 
	// So the set layout manipulation should always be applied here.
	// if (lpDefn->getCurrentLayoutKey()!=lNewKey)
	{
		mpRootNode->setCurrentNodeKey(lNewKey);
		lpDefn->setCurrentLayoutKey(lNewKey);
		mNeedRedraw = true;
		
		if (!mIsFromActionObject)
		{
            if (mAOXML == true)
            {
                GenerateAODeltaXML();
            }
            else
            {
                mpActionObject = new DSSAOLayout();
                ((DSSAOLayout*)(mpActionObject))->setLayoutNodeKey(lNewKey);
                ((DSSAOLayout*)(mpActionObject))->setRootNodeKey(mpRootNode->getKey());
            }
		}
	}
	
	return S_OK;
}

DSSRWNode* DSSCManipulationSetLayout::getRootNode()
{
	if (mNeedRedraw)
		return mpRootNode;
	else 
		return NULL;
}

DSSRWNode* DSSCManipulationSetLayout::getSectionObject()
{
	if (mNeedRedraw) 
	{
		return mpLayoutNode;
	} 
	else 
	{
		return NULL;
	}
}

#ifndef __ANDROID__
void DSSCManipulationSetLayout::GenerateAODeltaXML()
{
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("51", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodekeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpLayoutNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodekeyXMLContent);
    
    lXML.AddChild(lManipulationMethodXML);
    lXML.AddChild(lNodeKeyXML);
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    
    mpActionObject = new DSSAODeltaXML(ManipulationSetLayout);
    // JJ, TQMS 803751, we need to applynow for change layout so that web will have the new layout set
    // dli, another fix is provided which change the manipulation of setting unset from generating web ao into xml
    // mpActionObject->setApplyNow(true);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationSetLayout::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser(iXMLString);
    if(!lParser.IsValid())
        return;
    
    SimpleXMLNode lCurrentNode = lParser.GetRoot();
    if(!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
        return;
    
    for(lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if(lCurrentNode.IsElementNode() && lCurrentNode.GetName().compare("rw_node_key") == 0)
        {
            std::string lStrContent;
            lCurrentNode.GetContent(lStrContent);   
            mpLayoutNode = static_cast<DSSRWSectionNode*>(ipContext->getNode(lStrContent));
        }
    }
}
#else
void DSSCManipulationSetLayout::GenerateAODeltaXML()
{
    TiXmlDocument lDoc;
    TiXmlElement* lpRootElement = new TiXmlElement("rw_manipulation");
    
    TiXmlElement* lManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
    TiXmlText* lManipulationMethodText = new TiXmlText("51");
    lManipulationMethodElement->LinkEndChild(lManipulationMethodText);
    
    TiXmlElement* lNodeKeyElement = new TiXmlElement("rw_node_key");
    TiXmlText* lNodeKeyText = new TiXmlText(mpLayoutNode->getKey().c_str());
    lNodeKeyElement->LinkEndChild(lNodeKeyText);
    
    lpRootElement->LinkEndChild(lManipulationMethodElement);
    lpRootElement->LinkEndChild(lNodeKeyElement);
    
    TiXmlPrinter lPrinter;
    lPrinter.SetIndent(NULL);
    lPrinter.SetLineBreak(NULL);
    lDoc.Accept(&lPrinter);
    
    mpActionObject = new DSSAODeltaXML();
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lPrinter.Str());
}
#endif
