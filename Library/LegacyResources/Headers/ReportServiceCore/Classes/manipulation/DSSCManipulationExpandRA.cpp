//
//  DSSCManipulationExpandRA.cpp
//  ReportServiceCore
//
//  Created by Sun, Mingmin on 5/9/16.
//  Copyright © 2016 MicroStrategy, Inc. All rights reserved.
//

#include "DSSCManipulationExpandRA.h"
#include "DSSAODeltaXML.h"
#include "DSSRWDEngine.h"
#include "DSSXTabRecursiveUnit.h"
#include "DSSExpNode.h"
#include "DSSRWDataModel.h"
#ifndef __ANDROID__
#include "DSSSimpleXML.h"
#else
#include "TinyXml/tinyxml.h"
#endif

#define DebugAO false

DSSCManipulationExpandRA::DSSCManipulationExpandRA(DSSRWIterator* ipIterator, int iAxis, int iDepth, DSSCellHandle* iCellHandle, bool iExpand, bool iApplyAll)
{
    mpIterator = ipIterator;
    mpTemplateNode = (DSSRWTemplateNode*)(ipIterator->getRWNode());
    mAxis = iAxis;
    mDepth = iDepth;
    mCellHandle = *iCellHandle;
    mbExpand = iExpand;
    mbApplyAll = iApplyAll;
    mAOXML = true;
    mIsFromActionObject = false;
    mType = ManipulationExpandRecursiveAttribute;
}

DSSCManipulationExpandRA::DSSCManipulationExpandRA(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    if(ipAO && ipAO->IsDeltaXML())
    {
        DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
        std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
        hParseAOXML(lXMLString, ipContext);
#endif
    }
    else
    {
        assert(false);
    }
    
    mIsFromActionObject = true;
    mAOXML = true;
    mType = ManipulationExpandRecursiveAttribute;
}

DSSCManipulationExpandRA::~DSSCManipulationExpandRA()
{
    for (int i=0; i<mSiblingElementsProxy.size(); i++) {
        delete mSiblingElementsProxy[i];
    }
    mSiblingElementsProxy.clear();
}

int DSSCManipulationExpandRA::Execute()
{
    if (!mpIterator) {
        return S_FALSE;
    }
    DSSRWGridIterator* lGridIterator = dynamic_cast<DSSRWGridIterator*>(mpIterator);
    if (!lGridIterator) {
        return S_FALSE;
    }
    
    //mSiblingElementsProxy for generating xml
    lGridIterator->createSiblingElementsFromHandle(mCellHandle, false, mSiblingElementsProxy);
    
    DSSTabularUnit* lpTabularUnit = lGridIterator->getLatestTabularUnit(mAxis, mDepth);
    if (!lpTabularUnit) {
        return S_FALSE;
    }
    
    //after update model, the previous reference is abandoned.
    mCellHandle.p.xtabUnit = lpTabularUnit;
    
    mID = lpTabularUnit->getGUIDStr();
    mRowOrdinal = mCellHandle.v.ordinal;
    
    if (!DebugAO) {
        mpTemplateNode = (DSSRWTemplateNode*)(lGridIterator->getRWNode());
        DSSXTabRecursiveUnit* lpRAUnit = dynamic_cast<DSSXTabRecursiveUnit*>(lpTabularUnit);
        if (lpRAUnit) {
            int lKey;
            int hr =lGridIterator->getXTabElementKey(mCellHandle,lpTabularUnit,lKey);
            if (hr == S_OK || mbApplyAll) {
                lpRAUnit->updateExpandStatus(lKey, mbExpand, mbApplyAll);
                
                DSSRWDataModel* lpDataModel = lGridIterator->getDataModel();
                if (lpDataModel) {
                    std::string lNodeKey = mpTemplateNode->getKey();
                    lpDataModel->ReevaluateViewForFinancialReport(lNodeKey);
                }
            }
        }
    }
    else
    {

        DSSObjectContext* lpContext = mpTemplateNode->getObjectContext();
        
        mNodesModelChanged.push_back(mpTemplateNode);
        mNodeMapServer[mpTemplateNode->getKey()] = mpTemplateNode;
        
        std::vector<std::string> lNodes = mpTemplateNode->GetServerNodeKeys();
        int lSize = lNodes.size();
        for (int i = 0; i < lSize; i++)
        {
            std::string lKey = lNodes[i];
            if (mNodeMapServer.find(lKey) != mNodeMapServer.end())
                continue;
            
            DSSRWNode* lNode = lpContext->getNode(lKey);
            
            mNodesModelChanged.push_back(lNode);
            mNodeMapServer[lKey] = lNode;
        }
    }
    vector<int> lGBPath;
    mpIterator->getNGBPath(lGBPath);
    if (!mIsFromActionObject)
    {
        if(mAOXML)
        {
            GenerateAODeltaXML();
        }
    }
    return S_OK;
}

DSSRWTemplateNode* DSSCManipulationExpandRA::getTemplateNode()
{
    return mpTemplateNode;
}

#ifndef __ANDROID__
void DSSCManipulationExpandRA::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
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
            mpTemplateNode = static_cast<DSSRWTemplateNode*>(ipContext->getNode(lStrContent));
        }
        else if(lCurrentNode.IsElementNode() && lCurrentNode.GetName().compare("report_manipulation") == 0)
        {
            SimpleXMLNode lReportNode = lCurrentNode.GetFirstChild();
            for (; lReportNode.IsValid(); lReportNode = lReportNode.GetNextSibling()) {
                if(lReportNode.IsElementNode() && lReportNode.GetName().compare("attribute_id") == 0){
                    std::string lStrContent;
                    lReportNode.GetContent(lStrContent);
                    mID = lStrContent;
                }
                else if(lReportNode.IsElementNode() && lReportNode.GetName().compare("is_expand_all") == 0){
                    mbApplyAll = true;
                    mbExpand = true;
                }
                else if(lReportNode.IsElementNode() && lReportNode.GetName().compare("is_collapse_all") == 0){
                    mbApplyAll = true;
                    mbExpand = false;
                }
                else if(lReportNode.IsElementNode() && lReportNode.GetName().compare("is_expand") == 0){
                    mbApplyAll = false;
                    std::string lStrContent;
                    lReportNode.GetContent(lStrContent);
                    if (lStrContent == "1") {
                        mbExpand = true;
                    }
                    else
                    {
                        mbExpand = false;
                    }
                }
                else if(lReportNode.IsElementNode() && lReportNode.GetName().compare("depth") == 0){
                    std::string lStrContent;
                    lReportNode.GetContent(lStrContent);
                    mDepth = DSSExpNode::StringToInt(lStrContent) -1;
                }
                else if(lReportNode.IsElementNode() && lReportNode.GetName().compare("element_ordinals_collection") == 0){
                    SimpleXMLNode lElementNode = lReportNode.GetFirstChild();
                    std::string lStrContent;
                    lElementNode.GetContent(lStrContent);
                    mRowOrdinal = DSSExpNode::StringToInt(lStrContent);
                }
                else if(lReportNode.IsElementNode() && lReportNode.GetName().compare("slice_id") == 0){
                    std::string lStrContent;
                    lReportNode.GetContent(lStrContent);
                    mSliceID = DSSExpNode::StringToInt(lStrContent);
                    //TODO: how to convert between slice id and ngbpath
                    if (mpTemplateNode && mSliceID == 1) {
                        std::string lKey = mpTemplateNode->getKey();
                        std::vector<int> lNGBPath;
                        if (ipContext->getRWDEngine()) {
                            DSSRWIterator* lpIterator;
                            ipContext->getRWDEngine()->FindIterator(lKey, lNGBPath, &lpIterator);
                            mpIterator = lpIterator;
                        }
                        
                    }
                    
                }
            }
        }
    }
}

void DSSCManipulationExpandRA::GenerateAODeltaXML()
{
    mpActionObject = new DSSAODeltaXML(ManipulationExpandRecursiveAttribute);
    
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("19", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    lXML.AddChild(lManipulationMethodXML);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    lXML.AddChild(lNodeKeyXML);
    
    SimpleXMLNode lReportXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation"));
    {
        SimpleXMLNode lReportManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation_method"));
        SimpleXMLNode lReportManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("174", true));
        lReportManipulationMethodXML.AddChild(lReportManipulationMethodXMLContent);
        lReportXML.AddChild(lReportManipulationMethodXML);
        
        SimpleXMLNode lAttrIDXML(SimpleXMLBuilder::CreateNodePtr("attribute_id"));
        SimpleXMLNode lAttrIDXMLContent(SimpleXMLBuilder::CreateNodePtr(mID.c_str(), true));
        lAttrIDXML.AddChild(lAttrIDXMLContent);
        lReportXML.AddChild(lAttrIDXML);
        
        if (mbApplyAll) {
            
            
            if (mbExpand) {
                SimpleXMLNode lApplyAllXML(SimpleXMLBuilder::CreateNodePtr("is_expand_all"));
                SimpleXMLNode lApplyAllXMLContent(SimpleXMLBuilder::CreateNodePtr("1", true));
                lApplyAllXML.AddChild(lApplyAllXMLContent);
                lReportXML.AddChild(lApplyAllXML);
            }
            else
            {
                SimpleXMLNode lApplyAllXML(SimpleXMLBuilder::CreateNodePtr("is_collapse_all"));
                SimpleXMLNode lApplyAllXMLContent(SimpleXMLBuilder::CreateNodePtr("1", true));
                lApplyAllXML.AddChild(lApplyAllXMLContent);
                lReportXML.AddChild(lApplyAllXML);
            }
            
        }
        else
        {
            /*
             <attribute_id>**</attribute_id>
             <slice_id>1</slice_id>
             <depth>**</depth>
             <element_ordinals_collection>
             <ordinal>**</ordinal>
             <ordinal>**</ordinal>
             </element_ordinals_collection>
             <is_expand>1/0</is_expand>
             */
            //deprecated API.
            if (false) {
                SimpleXMLNode lSliceIDXML(SimpleXMLBuilder::CreateNodePtr("slice_id"));
                SimpleXMLNode lSliceIDXMLContent(SimpleXMLBuilder::CreateNodePtr("1", true));
                lSliceIDXML.AddChild(lSliceIDXMLContent);
                lReportXML.AddChild(lSliceIDXML);
                
                SimpleXMLNode lDepthXML(SimpleXMLBuilder::CreateNodePtr("depth"));
                SimpleXMLNode lDepthXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(mDepth+1).c_str(), true));//TODO
                lDepthXML.AddChild(lDepthXMLContent);
                lReportXML.AddChild(lDepthXML);
                
                SimpleXMLNode lOrdiColXML(SimpleXMLBuilder::CreateNodePtr("element_ordinals_collection"));
                SimpleXMLNode lOrdiXML(SimpleXMLBuilder::CreateNodePtr("ordinal"));
                SimpleXMLNode lOrdiXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(mRowOrdinal).c_str(),true));
                lOrdiXML.AddChild(lOrdiXMLContent);
                lOrdiColXML.AddChild(lOrdiXML);
                lReportXML.AddChild(lOrdiColXML);
            }
            else
            {
                SimpleXMLNode lOrdiColXML(SimpleXMLBuilder::CreateNodePtr("element_ordinals_collection"));
                lReportXML.AddChild(lOrdiColXML);
            }
            SimpleXMLNode lExpandXML(SimpleXMLBuilder::CreateNodePtr("is_expand"));
            SimpleXMLNode lExpandXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(mbExpand?1:0).c_str(), true));//TODO
            lExpandXML.AddChild(lExpandXMLContent);
            lReportXML.AddChild(lExpandXML);
        }
    
    }
    lXML.AddChild(lReportXML);
    
    //New way to indicate the elements, avoid the ordinal logic on server side.
    if (true)
    {
        SimpleXMLNode lEleIDs(SimpleXMLBuilder::CreateNodePtr("new_element_ids"));
        
        DSSRWGridIterator* lpGridIterator = dynamic_cast<DSSRWGridIterator*>(mpIterator);
        if (!lpGridIterator) {
            assert(false);
            return;
        }
        vector<DSSBaseElementProxy*> lvElementPath;
        lpGridIterator->createElementsFromRAHandle(mCellHandle, lvElementPath);
        for (int i = 0; i < lvElementPath.size(); i++) {
            SimpleXMLNode lNewEleID(SimpleXMLBuilder::CreateNodePtr("new_element_id"));
            DSSAttributeElementProxy* lpTempID = dynamic_cast<DSSAttributeElementProxy*>(lvElementPath[i]);
            if (lpTempID) {
                SimpleXMLNode lTempEleIDContent(SimpleXMLBuilder::CreateNodePtr(lpTempID->getCompactElementID().c_str(), true));
                lNewEleID.AddChild(lTempEleIDContent);
            }
            lEleIDs.AddChild(lNewEleID);
        }
        lXML.AddChild(lEleIDs);
        
        if (mSiblingElementsProxy.size() > 0) {
            SimpleXMLNode lElePath(SimpleXMLBuilder::CreateNodePtr("element_path"));
            for (int i =0; i<mSiblingElementsProxy.size(); i++) {
                SimpleXMLNode lEleID(SimpleXMLBuilder::CreateNodePtr("element_id"));
                DSSAttributeElementProxy* lpTempID = dynamic_cast<DSSAttributeElementProxy*>(mSiblingElementsProxy[i]);
                if (lpTempID) {
                    SimpleXMLNode lTempEleIDContent(SimpleXMLBuilder::CreateNodePtr(lpTempID->getCompactElementID().c_str(), true));
                    lEleID.AddChild(lTempEleIDContent);
                }
                lElePath.AddChild(lEleID);
            }
            lXML.AddChild(lElePath);
        }
    }
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}
#else
void DSSCManipulationExpandRA::GenerateAODeltaXML()
{
    TiXmlDocument lDoc;
    TiXmlElement* lpRootElement = new TiXmlElement("rw_manipulation");
    
    TiXmlElement* lpManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
    TiXmlText* lpManipulationMethodText = new TiXmlText("19");
    lpManipulationMethodElement->LinkEndChild(lpManipulationMethodText);
    lpRootElement->LinkEndChild(lpManipulationMethodElement);
    
    
    
    
    lDoc.LinkEndChild(lpRootElement);
    TiXmlPrinter lPrinter;
    lPrinter.SetIndent(NULL);
    lPrinter.SetLineBreak(NULL);
    lDoc.Accept(&lPrinter);
    mpActionObject = new DSSAODeltaXML(ManipulationSortTemplate);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lPrinter.Str());
}
#endif



