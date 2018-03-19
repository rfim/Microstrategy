/*
 *  DSSCManipulationSetTemplateDisplayMode.cpp
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-19.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSCManipulationSetTemplateDisplayMode.h"
#include "DSSRWTemplateNode.h"
#include "DSSAODisplayMode.h"

#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif

DSSCManipulationSetTemplateDisplayMode::DSSCManipulationSetTemplateDisplayMode(DSSRWTemplateNode* ipTemplateNode, EnumDSSDisplayMode iDisplayNode, bool iAOXML)
{
	mpTemplateNode = ipTemplateNode;
	mDisplayNode = iDisplayNode;
	mNeedUpdate = false;
	mIsFromActionObject = false;
    mAOXML = iAOXML;
    
    mType = ManipulationSetTemplateDisplayMode;
}

DSSCManipulationSetTemplateDisplayMode::DSSCManipulationSetTemplateDisplayMode(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationSetTemplateDisplayMode;
    
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
        DSSAODisplayMode* lpAO = static_cast<DSSAODisplayMode*>(ipAO);
        mpTemplateNode = (DSSRWTemplateNode*)(ipContext->getNode(lpAO->getNodeKey()));
        mDisplayNode = lpAO->getDisplayMode();
        mNeedUpdate = false;
        mIsFromActionObject = true;
    }
}

DSSCManipulationSetTemplateDisplayMode::~DSSCManipulationSetTemplateDisplayMode()
{}

int DSSCManipulationSetTemplateDisplayMode::Execute()
{
	mpTemplateNode->setDisplayMode(mDisplayNode);
	mNeedUpdate = true;
	
	if (!mIsFromActionObject)
	{
        if(mAOXML == true)
        {
            GenerateAODeltaXML();
        }
        else
        {
            mpActionObject = new DSSAODisplayMode();
            ((DSSAODisplayMode*)(mpActionObject))->setNodeKey(mpTemplateNode->getKey());
            ((DSSAODisplayMode*)(mpActionObject))->setDisplayMode(mDisplayNode);
        }
	}
		
	return S_OK;
}

DSSRWTemplateNode* DSSCManipulationSetTemplateDisplayMode::getTemplateNode()
{
	if (mNeedUpdate)
		return mpTemplateNode;
	else 
		return NULL;
}
#ifndef __ANDROID__
void DSSCManipulationSetTemplateDisplayMode::GenerateAODeltaXML()
{
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("32"));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    
    int lTreeType =  static_cast<int>(mpTemplateNode->getTreeType());
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    
    int lDisplayMode = static_cast<int>(mDisplayNode);
    SimpleXMLNode lDisplayModeXML(SimpleXMLBuilder::CreateNodePtr("rw_grid_display_mode"));
    SimpleXMLNode lDisplayModeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lDisplayMode).c_str(), true));
    lDisplayModeXML.AddChild(lDisplayModeXMLContent);
    
    lXML.AddChild(lManipulationMethodXML);
    lXML.AddChild(lTreeTypeXML);
    lXML.AddChild(lNodeKeyXML);
    lXML.AddChild(lDisplayModeXML);
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    
    mpActionObject = new DSSAODeltaXML();
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationSetTemplateDisplayMode::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser(iXMLString);
    if(!lParser.IsValid())
        return;
    
    SimpleXMLNode lCurrentNode = lParser.GetRoot();
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
            else if(lCurrentNode.GetName().compare("rw_grid_display_mode"))
            {
                lCurrentNode.GetContent(lStrContent);
                int lDisplymode = atoi(lStrContent.c_str());
                mDisplayNode = static_cast<EnumDSSDisplayMode>(lDisplymode);
            }
        }
    }
}
#else
void DSSCManipulationSetTemplateDisplayMode::GenerateAODeltaXML()
{
    TiXmlDocument lDoc;
    TiXmlElement* lpRootElement = new TiXmlElement("rw_manipulation");
    
    TiXmlElement* lpManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
    TiXmlText* lpManipulationMethodText = new TiXmlText("32");
    lpManipulationMethodElement->LinkEndChild(lpManipulationMethodText);
    
    int lTreeType = static_cast<int>(mpTemplateNode->getTreeType());
    TiXmlElement* lpTreeTypeElement = new TiXmlElement("rw_tree_type");
    TiXmlText* lpTreeTypeText = new TiXmlText(DSSExpNode::IntToString(lTreeType).c_str());
    lpTreeTypeElement->LinkEndChild(lpTreeTypeText);
    
    TiXmlElement* lpNodeKeyElement = new TiXmlElement("rw_node_key");
    TiXmlText* lpNodeKeyText = new TiXmlText(mpTemplateNode->getKey().c_str());
    lpNodeKeyElement->LinkEndChild(lpNodeKeyText);
    
    int lDisplayMode = static_cast<int>(mDisplayNode);
    TiXmlElement* lpDisplayModeElement = new TiXmlElement("rw_grid_display_mode");
    TiXmlText* lpDisplayModeText = new TiXmlText(DSSExpNode::IntToString(lDisplayMode).c_str());
    lpDisplayModeElement->LinkEndChild(lpDisplayModeText);
    
    lpRootElement->LinkEndChild(lpManipulationMethodElement);
    lpRootElement->LinkEndChild(lpTreeTypeElement);
    lpRootElement->LinkEndChild(lpNodeKeyElement);
    lpRootElement->LinkEndChild(lpDisplayModeElement);
    
    TiXmlPrinter lPrinter;
    lPrinter.SetIndent(NULL);
    lPrinter.SetLineBreak(NULL);
    lDoc.Accept(&lPrinter);
                           
    mpActionObject = new DSSAODeltaXML();
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lPrinter.Str());
}
#endif