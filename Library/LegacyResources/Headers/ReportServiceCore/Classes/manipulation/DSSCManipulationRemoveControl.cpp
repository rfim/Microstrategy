//
//  DSSCManipulationRemoveControl.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 1/12/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "DSSCManipulationRemoveControl.h"
#include "DSSAODeltaXML.h"
#include "DSSRWTemplateNode.h"
#include "DSSRWControlNode.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif
class DSSRWIterator;

DSSCManipulationRemoveControl::DSSCManipulationRemoveControl(DSSRWNode* ipNode) : DSSCManipulationRemoveRWNode(ipNode)
{
    assert(ipNode->getType() == DssRWNodeControl);
    DSSRWControlNode* lpNode = static_cast<DSSRWControlNode*>(ipNode);
    DSSRWControls* lpControls = lpNode->getControls();
    if(lpControls && lpControls->Count() > 0)
    {
        mpControl = lpControls->Item(0);
    }
    
    for(int i = 0; i < mpControl->getCountTargets(); i++)
        mTargetKeys.push_back(mpControl->getTargetKey(i));
    
    mType = ManipulationRemoveControl;
}

DSSCManipulationRemoveControl::DSSCManipulationRemoveControl(DSSActionObject* ipAO, DSSObjectContext* ipContext) : DSSCManipulationRemoveRWNode(NULL, NULL)
{
    mType = ManipulationRemoveControl;
    
    if(!ipAO->IsDeltaXML())
    {
        return;
    }
#ifndef __ANDROID__
    std::string lXMLString = (static_cast<DSSAODeltaXML*>(ipAO))->getXML();
    hParseAOXML(lXMLString, ipContext);
#endif
}

DSSCManipulationRemoveControl::~DSSCManipulationRemoveControl()
{
    //delete the removed nodes and iterators in its base class. lcao,20130109
}

int DSSCManipulationRemoveControl::Execute()
{
    if(!mpControl)
        return S_FALSE;
    //debug
    /*DSSObjectContext* lpObjectContext = mpNode->getObjectContext();
    DSSRWNode* lpNode = lpObjectContext->getNode("K33");
    mNodesModelChanged.push_back(lpNode);
    DSSRWNode* lpNode2 = lpObjectContext->getNode("K46");
    mNodesModelChanged.push_back(lpNode2); */
    
    /*DSSObjectContext* lpObjectContext = mpNode->getObjectContext();
    mNodesModelChanged.push_back(mpNode->getParentNode());
    mNodesModelChanged.push_back(mpNode->getParentNode()->getParentNode()); //K46
    std::string lNodeKey = mpControl->getTargetKey(0);
    DSSRWNode* lpNode = lpObjectContext->getNode(lNodeKey);
    while (lpNode->getType()==DssRWNodeSection)
    {
        DSSRWSectionNode* lpSecNode = static_cast<DSSRWSectionNode*>(lpNode);
        if (lpSecNode->isUCorCGB())
            lpNode = lpSecNode->Item(0);
        else
        {
            mNodesModelChanged.push_back(lpSecNode);
            break;
        }
    }*/ //the code above push filter panel stack node key and visulization panel stack key into mNodesModelChanged
    //enddebug
    
    mNodesModelChanged.push_back(mpNode->getParentNode()->getParentNode()); //filter panel stack node
    std::string lNodeKey = mpControl->getTargetKey(0);
    DSSRWNode* lpNode = mpControl->getObjectContext()->getNode(lNodeKey);
    while (lpNode->getType() == DssRWNodeSection)
    {
        DSSRWSectionNode* lpSecNode = static_cast<DSSRWSectionNode*>(lpNode);
        if (lpSecNode->isUCorCGB())
            lpNode = lpSecNode->Item(0);
        else
        {
            populateSubtreeIntoNodeModelChanged(lpNode);
            break;
        }
    }
    GenerateAODeltaXML();
    RemoveControl();
    DSSCManipulationRemoveRWNode::Execute();
    return S_OK;
}

void DSSCManipulationRemoveControl::RemoveControl()
{    
    int i;
    
    //mNodesRemoved.push_back(mpNode);
    for (i=0; i<mTargetKeys.size(); i++)
        RemoveControlTarget(mpNode, mTargetKeys[i]);
    
    //Add Control node key whose source is metric to mNodeMapServer
    DSSRWNode* lpParentNode = mpNode->getParentNode();
    for (i=0; i<lpParentNode->Count(); i++)
    {
        DSSRWControlNode* lpCtrlNode = getChildControlNode(lpParentNode->Item(i));
        if (lpCtrlNode && lpCtrlNode->getKey()!=mpNode->getKey())
        {
            DSSRWControls* lpCtrls = lpCtrlNode->getControls();
            DSSRWControl* lpCtrl = lpCtrls->Item(0);
            DSSMetric* lpMetric = dynamic_cast<DSSMetric*>(lpCtrl->getSourceObject());
            if (lpMetric)
                mNodeMapServer[lpCtrlNode->getKey()] = lpCtrlNode;
        }
    }
    
}

//ported from ManipulationHelper::removeControlTarget() in 931 flash
DSSRWNode* DSSCManipulationRemoveControl::RemoveControlTarget(DSSRWNode* ipNode, std::string iTargetKey)
{
    DSSObjectContext* lpContext = mpNode->getObjectContext();
    MapFromTargetToControl* lMapFromTargetToControl = lpContext->getMapFromTargetToControl();
    DSSRWNode* lpTargetNode = lpContext->getNode(iTargetKey);
    bool flag = true;
    int i;
    DSSRWNode* lpMatchedGroupByNode = NULL;
    
    do {
        lpMatchedGroupByNode = searchGroupByNodes(lpTargetNode, mpControl->getSourceID());
        if(!lpMatchedGroupByNode)
            return NULL;
        NodeAndControlsVector lNodeAndControlsVector = (*lMapFromTargetToControl)[lpMatchedGroupByNode->getKey()];
        if(!lNodeAndControlsVector.size())
            return lpMatchedGroupByNode;
        
        for(NodeAndControlsVector::iterator lIter = lNodeAndControlsVector.begin(); lIter != lNodeAndControlsVector.end(); lIter++)
        {
            if((*lIter)->NodeKey == ipNode->getKey())
            {
                lNodeAndControlsVector.erase(lIter);
                // remove control target here ?
                flag = false;
                break;
            }
        }
        
        if(!lNodeAndControlsVector.size())
        {
            //mNodesModelChanged.push_back(lpMatchedGroupByNode->getParentNode());
            
            for (i=0; i<lpMatchedGroupByNode->Count(); i++)
            {
                DSSRWNode* lpChildNode = lpMatchedGroupByNode->Item(i);
                mNodeMapServer[lpChildNode->getKey()] = lpChildNode;
            //    populateSubtreeIntoNodeModelChanged(lpChildNode);
            }

            DSSRWNode* lpChildNode = lpMatchedGroupByNode->Item(0);
            //mNodesRemoved.push_back(lpMatchedGroupByNode);
            lpMatchedGroupByNode->getParentNode()->RemoveGroupByNode(lpMatchedGroupByNode->getIndex(), &mNodesRemoved, &mIteratorsRemoved);
            lMapFromTargetToControl->erase(lpMatchedGroupByNode->getKey());
            lpMatchedGroupByNode = lpChildNode;
        }
        else{   //still targeted by other controls
            mNodeMapServer[lpMatchedGroupByNode->getKey()] = lpMatchedGroupByNode;
            //populateSubtreeIntoNodeModelChanged(lpMatchedGroupByNode);
        }
        lpTargetNode = lpMatchedGroupByNode->getParentNode();
    } while (flag);
    
    return lpMatchedGroupByNode;
}

//ported from ManipulationHelper::getChildControlNode() in 931 flash
DSSRWControlNode* DSSCManipulationRemoveControl::getChildControlNode(DSSRWNode* ipNode)
{
    if (!ipNode)
        return NULL;
    
    EnumDSSRWNodeType lNodeType = ipNode->getType();
    if (lNodeType == DssRWNodeControl)
        return (DSSRWControlNode*)ipNode;
    else
        return getChildControlNode(ipNode->Item(0));
}

#ifndef __ANDROID__
void DSSCManipulationRemoveControl::GenerateAODeltaXML()
{
    mpActionObject = new DSSAODeltaXML(ManipulationRemoveControl);
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    DSSObjectContext* lpContext = mpNode->getObjectContext();
    MapFromTargetToControl* lMapFromTargetToControl = lpContext->getMapFromTargetToControl();
    EnumDSSRWTreeType lTreeType = mpNode->getTreeType();
    
    for(int i = 0; i < mpControl->getCountTargets(); i++)
    {
        SimpleXMLNode lRemoveTargetXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
        SimpleXMLNode lRTManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
        SimpleXMLNode lRTManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("68", true));     //RemoveControlTarget
        lRTManipulationMethodXML.AddChild(lRTManipulationMethodXMLContent);
        
        SimpleXMLNode lRTTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
        SimpleXMLNode lRTTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
        lRTTreeTypeXML.AddChild(lRTTreeTypeXMLContent);
        
        SimpleXMLNode lRTNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
        SimpleXMLNode lRTNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpNode->getKey().c_str(), true));
        lRTNodeKeyXML.AddChild(lRTNodeKeyXMLContent);
        
        SimpleXMLNode lRTControlKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_control_key"));
        SimpleXMLNode lRTControlKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpControl->getKey().c_str(), true));
        lRTControlKeyXML.AddChild(lRTControlKeyXMLContent);
        
        std::string lTargetKey = mpControl->getTargetKey(i);
        SimpleXMLNode lRTTargetKeyXML(SimpleXMLBuilder::CreateNodePtr("target_key"));
        SimpleXMLNode lRTTargetKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lTargetKey.c_str(), true));
        lRTTargetKeyXML.AddChild(lRTTargetKeyXMLContent);
        
        lRemoveTargetXML.AddChild(lRTManipulationMethodXML);
        lRemoveTargetXML.AddChild(lRTTreeTypeXML);
        lRemoveTargetXML.AddChild(lRTNodeKeyXML);
        lRemoveTargetXML.AddChild(lRTControlKeyXML);
        lRemoveTargetXML.AddChild(lRTTargetKeyXML);
        lRoot.AddChild(lRemoveTargetXML);
        
        //DssRWManipulationRemoveControlGroupbySection
        NodeAndControlsVector lNodeAndControlsVector = (*lMapFromTargetToControl)[lTargetKey];
        if (lNodeAndControlsVector.size())
        {
            SimpleXMLNode lRmvCtrlGBSecXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
            SimpleXMLNode lRmvCtrlGBSecMthdXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
            SimpleXMLNode lRmvCtrlGBSecCtnt(SimpleXMLBuilder::CreateNodePtr("66", true));
            lRmvCtrlGBSecMthdXML.AddChild(lRmvCtrlGBSecCtnt);
            
            SimpleXMLNode lRTTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
            SimpleXMLNode lRTTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
            lRTTreeTypeXML.AddChild(lRTTreeTypeXMLContent);
            
            SimpleXMLNode lRTNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
            SimpleXMLNode lRTNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lTargetKey.c_str(), true));
            lRTNodeKeyXML.AddChild(lRTNodeKeyXMLContent);
            
            lRmvCtrlGBSecXML.AddChild(lRmvCtrlGBSecMthdXML);
            lRmvCtrlGBSecXML.AddChild(lRTTreeTypeXML);
            lRmvCtrlGBSecXML.AddChild(lRTNodeKeyXML);
            lRoot.AddChild(lRmvCtrlGBSecXML);
        }
    }
    
    //DssRWManipulationRemoveControl
    SimpleXMLNode lRmvCtrlXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    SimpleXMLNode lRmvCtrlMthdXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lRmvCtrlCtntCXML(SimpleXMLBuilder::CreateNodePtr("70", true));
    lRmvCtrlMthdXML.AddChild(lRmvCtrlCtntCXML);
    
    SimpleXMLNode lTreeTypeXML2(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent2(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML2.AddChild(lTreeTypeXMLContent2);
    
    SimpleXMLNode lNodeKeyXML2(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent2(SimpleXMLBuilder::CreateNodePtr(mpNode->getKey().c_str(), true));
    lNodeKeyXML2.AddChild(lNodeKeyXMLContent2);
    
    SimpleXMLNode lCtrlKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_control_key"));
    SimpleXMLNode lCtrlKeyXMLCtntXML(SimpleXMLBuilder::CreateNodePtr(mpControl->getKey().c_str(), true));
    lCtrlKeyXML.AddChild(lCtrlKeyXMLCtntXML);
    
    lRmvCtrlXML.AddChild(lRmvCtrlMthdXML);
    lRmvCtrlXML.AddChild(lTreeTypeXML2);
    lRmvCtrlXML.AddChild(lNodeKeyXML2);
    lRmvCtrlXML.AddChild(lCtrlKeyXML);
    lRoot.AddChild(lRmvCtrlXML);
    
    //DssRWManipulationRemoveNode, we persist the remove node manipulation in its base class. lcao 20130114
    /*SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("5", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent); 
    
    lXML.AddChild(lManipulationMethodXML);
    lXML.AddChild(lTreeTypeXML);
    lXML.AddChild(lNodeKeyXML);
    lRoot.AddChild(lXML); */
    
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}
void DSSCManipulationRemoveControl::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
{
    SimpleXMLParser lParser(iXMLString);
    if(!lParser.IsValid())
    {
        return;
    }
    SimpleXMLNode lRootNode = lParser.GetRoot();
    SimpleXMLNode lCurrentNode;
    if(!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
    {
        return ;
    }
    // only parse one?
    std::string lCurrentContent;
    for(lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if(lCurrentNode.IsElementNode())
        {
            if(lCurrentNode.GetName().compare("rw_node_key") == 0)
            {
                lCurrentNode.GetContent(lCurrentContent);
                mpNode = ipContext->getNode(lCurrentContent);
            }
            else if(lCurrentNode.GetName().compare("rw_control_key") == 0)
            {
                lCurrentNode.GetContent(lCurrentContent);
                mpControl = ipContext->getControl(lCurrentContent);
            }
        }
    }
}
#else
void DSSCManipulationRemoveControl::GenerateAODeltaXML()
{
    TiXmlDocument lDoc;
    TiXmlElement* lRootElement;
    
    for(int i = 0; i < mpControl->getCountTargets(); i++)
    {
        TiXmlElement* lRemoveTargetElement = new TiXmlElement("rw_manipulation");
        
        TiXmlElement* lRTManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
        TiXmlText* lRTManipulationMethodText = new TiXmlText("68");
        lRTManipulationMethodElement->LinkEndChild(lRTManipulationMethodeText);
        
        EnumDSSRWTreeType lRTTreeType = mpNode->getTreeType();
        TiXmlElement* lRTTreeTypeElement = new TiXmlElement("rw_tree_type");
        TiXmlText* lRTTreeTypeText = new TiXmlText(DSSExpNode::IntToString(lRTTreeType).c_str());
        lRTTreeTypeElement->LinkEndChild(lRTTreeTypeText);
        
        TiXmlElement* lRTNodeKeyElement = new TiXmlElement("rw_node_key");
        TiXmlText* lRTNodeKeyText = new TiXmlText(mpNode->getKey().c_str());
        lRTNodeKeyElement->LinkEndChild(lRTNodeKeyText);
        
        TiXmlElement* lRTControlKeyElement = new TiXmlElement("rw_control_key");
        TiXmlText* lRTControlKeyText = new TiXmlText(mpControl->getKey().c_str());
        lRTControlKeyElement->LinkEndChild(lRTControlKeyText);
        
        std::string lTargetKey = mpControl->getTargetKey(i);
        TiXmlElement* lRTTargetKeyElement = new TiXmlElement("target_key");
        TiXmlText* lRTControlKeyText = new TiXmlText(lTargetKey.c_str());
        
        lRemoveTargetElement->LinkEndChild(lRTManipulationMethodElement);
        lRemoveTargetElement->LinkEndChild(lRTTreeTypeElement);
        lRemoveTargetElement->LinkEndChild(lRTNodeKeyElement);
        lRemoveTargetElement->LinkEndChild(lRTControlKeyElement);
        lRemoveTargetElement->LinkEndChild(lRTTargetKeyElement);
        
        lRootElement->LinkEndChild(lRemoveTargetElement);
    }
    
    TiXmlElement* lRemoveControlElement = new TiXmlElement("rw_manipulation");
    
    TiXmlElement* lManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
    TiXmlText* lManipulationMethodText = new TiXmlText("5");
    lManipulationMethodElement->LinkEndChild(lManipulationMethodeText);
    
    EnumDSSRWTreeType lTreeType = mpNode->getTreeType();
    TiXmlElement* lTreeTypeElement = new TiXmlElement("rw_tree_type");
    TiXmlText* lTreeTypeText = new TiXmlText(DSSExpNode::IntToString(lTreeType).c_str());
    lTreeTypeElement->LinkEndChild(lTreeTypeText);
    
    TiXmlElement* lNodeKeyElement = new TiXmlElement("rw_node_key");
    TiXmlText* lNodeKeyText = new TiXmlText(mpNode->getKey().c_str());
    lNodeKeyElement->LinkEndChild(lNodeKeyText);
    
    lRemoveControlElement->LinkEndChild(lManipulationMethodElement);
    lRemoveControlElement->LinkEndChild(lTreeTypeElement);
    lRemoveControlElement->LinkEndChild(lNodeKeyElement);
    lRootElement->LinkEndChild(lRemoveControlElement);
    
    TiXmlPrinter lPrinter;
    lPrinter.SetIndent(NULL);
    lPrinter.SetLineBreak(NULL);
    lDoc.Accept(&lPrinter);
    
    mpActionObject = new DSSAODeltaXML(ManipulationRemoveControl);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lPrinter.Str());
}
#endif









