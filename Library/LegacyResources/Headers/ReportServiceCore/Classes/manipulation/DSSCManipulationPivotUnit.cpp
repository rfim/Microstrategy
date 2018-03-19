/*
 *  DSSCManipulationPivotUnit.cpp
 *  ReportServiceCore
 * 
 *  Modified by wzhu on 12/22/11.
 *  Created by admin on 10/20/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSCManipulationPivotUnit.h"
#include "DSSRWTemplateNode.h"
#include "DSSTemplateUnit.h"
#include "DSSDataModelXTabImpl.h"
#include "DSSTemplateDimension.h"
#include "DSSSort.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#include "DSSBaseElementProxy.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxm.h"
#else
#include "DSSSimpleXML.h"
#endif

DSSCManipulationPivotUnit::DSSCManipulationPivotUnit(DSSRWTemplateNode* ipNode, DSSTemplateUnit* ipUnit, int idAxis, int idPos)
{
	mpTemplateNode = ipNode;
	mpUnit = ipUnit;
	mDstAxisIndex = idAxis;
	mDstUnitPosition = idPos;
	
	mRefreshView = false;
	mEnableDataRetrieval = false;
	mIsFromActionObject = false;
    
    mType = ManipulationPivotUnit;
}

DSSCManipulationPivotUnit::DSSCManipulationPivotUnit(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationPivotUnit;
    
    if (!ipAO->IsDeltaXML())
	{
        DSSAOPivotUnit* lPivotUnit = (DSSAOPivotUnit*)ipAO;
        mpTemplateNode = (DSSRWTemplateNode*)(ipContext->getNode(lPivotUnit->getNodeKey()));
        //mpUnit = ?
        mDstAxisIndex = lPivotUnit->getdAxis();
        mDstUnitPosition = lPivotUnit->getdPos();
        mSrcAxisIndex = lPivotUnit->getoAxis();
        mSrcUnitPosition = lPivotUnit->getoPos();
	}
	else 
	{

#ifndef __ANDROID__
		DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
		std::string lXMLString = lpAO->getXML();
		
		// parse lXMLString
		hParseAOXML(lXMLString, ipContext);
#endif
	}
    
    mRefreshView = false;
    mEnableDataRetrieval = false;
    mIsFromActionObject = true;
}

DSSCManipulationPivotUnit::~DSSCManipulationPivotUnit()
{}

int DSSCManipulationPivotUnit::Execute()
{
	if (!mpUnit || !mpTemplateNode)
		return S_FALSE;
	DSSAxis* lpAxis = mpUnit->getAxis();
    DSSObjectContext* lpObjectContext = mpUnit->getObjectContext();
    
	mSrcAxisIndex = lpAxis->getIndex();
	mSrcUnitPosition = mpUnit->getIndex();
	//std::vector<DSSRWNode*> lNodesModelChanged;a
	DSSTemplate* lpTemplate = mpTemplateNode->getTemplate();
	bool lValue = lpTemplate->MoveUnit(mSrcAxisIndex,mSrcUnitPosition,mDstAxisIndex,mDstUnitPosition); //mpTemplateNode, dst, src
	if (!lValue) {
		return S_FALSE;
	}
	// depends on whether there is subtotal
	mEnableDataRetrieval = (lpTemplate->getXTabModel()->getSubtotalCount() > 0);  // 12/22/2011 clark, enable data retrieval only when there is subtotals
	
	// Add the template node into the Server node map (the root nodes needing go back to server to get partial data retrieval)
    // clark, always use the partial data retrieval
	//if (mEnableDataRetrieval) {
	
		mNodeMapServer[mpTemplateNode->getKey()]=mpTemplateNode;
	//}

	mNodesModelChanged.push_back(mpTemplateNode); //??S
    lpTemplate->getXTabModel()->Refresh();
	
    DSSAxis* lDestAxis = NULL;
	if (mDstAxisIndex == 1)
        lDestAxis = lpTemplate->getRow();
    else
        lDestAxis = lpTemplate->getColumn();
    
    // 12/22/2011 clark, move sorts to the corresponding axis
    if (mSrcAxisIndex != mDstAxisIndex)
    {
        std::vector<DSSSort*> lSortsToMove;
        
        DSSSorts* lSrcSorts = lpAxis->getSorts();
        GUID lUnitId = mpUnit->getID();
        EnumDSSTemplateUnitType lUnitType = mpUnit->getType();
        
        if (lUnitType != DssTemplateMetrics)
        {
            if (lUnitType == DssTemplateDimension)
            {
                DSSTemplateDimension* lpDimension = (DSSTemplateDimension*)(mpUnit);
                int lCount = lpDimension->Count();
                for (int i = 0; i < lCount; i++)
                {
                    GUID lGuid = lpDimension->Item(i)->getAttribute()->getID();
                    std::vector<DSSSort*> lSortsToMovePart;
                    lSrcSorts->Find(lGuid, lSortsToMovePart);
                    
                    for (int j = 0; j < lSortsToMovePart.size(); j++)
                        lSortsToMove.push_back(lSortsToMovePart[j]);
                }
            }
            else
                lSrcSorts->Find(lUnitId, lSortsToMove);
            
            if (lSortsToMove.size() > 0)
            {
                DSSSorts* lDestSorts = lDestAxis->getSorts();
                if (!lDestSorts)
                {
                    lDestSorts = new DSSSorts(lpObjectContext);
                    lDestAxis->setSorts(lDestSorts);
                }
                else
                {
                    for (int i = 0; i < lSortsToMove.size(); i++)
                        lDestSorts->removeSort(lSortsToMove[i]);						
                }
                
                for (int i = 0; i < lSortsToMove.size(); i++)
                {
                    DSSSort* lSortToMove = lSortsToMove[i];
                    
                    //Find the correct position to insert the new sort
                    bool lInserted = false;
                    
                    for (int j = 0; j < lDestSorts->Count(); j++)
                    {
                        GUID lSortID = lDestSorts->Item(j)->getObjectID();
                        DSSTemplateUnit* lTemplateUnit = lpTemplate->getTemplateUnitFromAxis(lDestAxis, lSortID);

                        if (lTemplateUnit && lTemplateUnit->getIndex() > mDstUnitPosition)
                        {
                            lDestSorts->addSort(lSortToMove, j);
                            lInserted = true;
                            break;
                        }
                    }
                    if (!lInserted)
                        lDestSorts->addSort(lSortToMove);
                }
                for (int i = 0; i < lSortsToMove.size(); i++)
                    lSrcSorts->removeSort(lSortsToMove[i]);
            }
        }
    }

    // use with generic XML AO to the do the resync
    GenerateAODeltaXML();
	
	return S_OK;
}

#ifndef __ANDROID__
void DSSCManipulationPivotUnit::GenerateAODeltaXML()
{   
    DSSObjectContext* lObjectContext = mpTemplateNode->getObjectContext();
    mpActionObject = new DSSAODeltaXML(ManipulationPivotUnit);
    
    // tree type string
    SimpleXMLBuilder lBuilder;
        
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
	SimpleXMLNode lManipulationXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
	SimpleXMLNode lManipulationXMLContent(SimpleXMLBuilder::CreateNodePtr("19", true));
	lManipulationXML.AddChild(lManipulationXMLContent);
	lXML.AddChild(lManipulationXML);
    
    EnumDSSRWTreeType lTreeType = mpTemplateNode->getTreeType();
	SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
	SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
	lTreeTypeXML.AddChild(lTreeTypeXMLContent);
	lXML.AddChild(lTreeTypeXML);
	
	std::string lStr = mpTemplateNode->getKey();
	SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
	SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(lStr.c_str(), true));
	lNodeKeyXML.AddChild(lNodeKeyXMLContent);
	lXML.AddChild(lNodeKeyXML);
	
    SimpleXMLNode lRptManipulationXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation"));
    SimpleXMLNode lRptPivotUnitXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation_method"));
	SimpleXMLNode lRptPivotUnitXMContent(SimpleXMLBuilder::CreateNodePtr("7", true));
    lRptPivotUnitXML.AddChild(lRptPivotUnitXMContent);
    lRptManipulationXML.AddChild(lRptPivotUnitXML);
    
    EnumDSSTemplateUnitType lUnitType = mpUnit->getType();
    if (lUnitType != DssTemplateMetrics)
    {
        SimpleXMLNode lUnitIDXML(SimpleXMLBuilder::CreateNodePtr("unit_id"));
        char lID[33];
        GUID lGUID = mpUnit->getID();
        
        DSSBaseElementProxy::ConvertGUIDToString(lGUID, lID);
        lStr = lID;
        SimpleXMLNode lUnitIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lStr.c_str(), true));
        lUnitIDXML.AddChild(lUnitIDXMLContent);
        lRptManipulationXML.AddChild(lUnitIDXML);
        
        EnumDSSObjectType lObjecType = lObjectContext->FindObjectH(lGUID)->getType();
        SimpleXMLNode lUnitTypeXML(SimpleXMLBuilder::CreateNodePtr("unit_type"));
        SimpleXMLNode lUnitTypeContentXML(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lObjecType).c_str(), true));
        lUnitTypeXML.AddChild(lUnitTypeContentXML);
        lRptManipulationXML.AddChild(lUnitTypeXML);
    }
    
    SimpleXMLNode lAxisNameXML(SimpleXMLBuilder::CreateNodePtr("axis_name"));
    SimpleXMLNode lAxisNameContentXML(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(mDstAxisIndex).c_str(), true));
    lAxisNameXML.AddChild(lAxisNameContentXML);
    lRptManipulationXML.AddChild(lAxisNameXML);
    
    SimpleXMLNode lUnitPosXML(SimpleXMLBuilder::CreateNodePtr("unit_position"));
    SimpleXMLNode lUnitPosContentXML(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(mDstUnitPosition+1).c_str(), true));
    lUnitPosXML.AddChild(lUnitPosContentXML);
    lRptManipulationXML.AddChild(lUnitPosXML);
    
	lXML.AddChild(lRptManipulationXML);

    SimpleXMLNode lRoot(lBuilder.GetRoot());
	lRoot.AddChild(lXML);
	
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationPivotUnit::hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext)
{
	SimpleXMLParser lParser(iXMLString);
	if (!lParser.IsValid())
		return;
	
	SimpleXMLNode lCurrentNode = lParser.GetRoot();
	
	if (!lCurrentNode.IsElementNode() || lCurrentNode.GetName().compare("rw_manipulation") != 0)
		return;
	
	SimpleXMLNode lRptManipulation;
	std::string lstrContent;
	for (lCurrentNode = lCurrentNode.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
	{
		if (lCurrentNode.IsElementNode())
		{
			if (lCurrentNode.GetName().compare("rw_node_key") == 0) // template node
			{
				lCurrentNode.GetContent(lstrContent);
                mpTemplateNode = (DSSRWTemplateNode*)(ipContext->getNode(lstrContent));
			}
			else if (lCurrentNode.GetName().compare("report_manipulation") == 0) // report manipulation root
			{
                lRptManipulation = lCurrentNode;
			}
		}
	}
    
    // parse report manipulation
    DSSTemplate* lpTemplate = mpTemplateNode->getTemplate();
    GUID lTempID;
    for (lCurrentNode = lRptManipulation.GetFirstChild(); lCurrentNode.IsValid(); lCurrentNode = lCurrentNode.GetNextSibling())
    {
        if (lCurrentNode.IsElementNode())
		{
			if (lCurrentNode.GetName().compare("unit_id") == 0)
			{
				lCurrentNode.GetContent(lstrContent);
                DSSBaseElementProxy::ConvertFromStringToGUID(lstrContent, lTempID);
                
                DSSTemplateUnit* lUnit = NULL;
                
                lUnit = lpTemplate->getTemplateUnitFromAxisIndex(DssAxisRows, lTempID, false);
                if (lUnit)
                    mpUnit = lUnit;
                else
                    mpUnit = lpTemplate->getTemplateUnitFromAxisIndex(DssAxisColumns, lTempID, false);
                
                if (mpUnit)
                {
                    DSSAxis* lpAxis = mpUnit->getAxis();
                    
                    mSrcAxisIndex = lpAxis->getIndex();
                    mSrcUnitPosition = mpUnit->getIndex();
                }
			}
			else if (lCurrentNode.GetName().compare("unit_type") == 0)
			{
                lCurrentNode.GetContent(lstrContent);
                lRptManipulation = lCurrentNode;
			}
            else if (lCurrentNode.GetName().compare("axis_name") == 0)
			{
                lCurrentNode.GetContent(lstrContent);
                mDstAxisIndex = atoi(lstrContent.c_str());
			}
            else if (lCurrentNode.GetName().compare("unit_position") == 0)
			{
                lCurrentNode.GetContent(lstrContent);
                mDstUnitPosition = atoi(lstrContent.c_str()) - 1;
			}
		}
    }
    
}

#else
void DSSCManipulationPivotUnit::GenerateAODeltaXML()
{
    DSSObjectContext* lObjectContext = mpTemplateNode->getObjectContext();
    
   	TiXmlDocument lDoc;
	TiXmlElement* lpRootElement = new TiXmlElement("rw_manipulation");
    
	TiXmlElement* lpManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
	TiXmlText* lpManipulationMethodText = new TiXmlText("19");
	lpManipulationMethodElement->LinkEndChild(lpManipulationMethodText);
	lpRootElement->LinkEndChild(lpManipulationMethodElement);
    
    EnumDSSRWTreeType lTreeType = mpTemplateNode->getTreeType();
	TiXmlElement* lpTreeTypeElement = new TiXmlElement("rw_tree_type");
	TiXmlText* lpTreeTypeText = new TiXmlText(DSSExpNode::IntToString(lTreeType).c_str());
	lpTreeTypeElement->LinkEndChild(lpTreeTypeText);
	lpRootElement->LinkEndChild(lpTreeTypeElement);
    
	std::string lStr = mpTemplateNode->getKey();
	TiXmlElement* lpNodeKeyElement = new TiXmlElement("rw_node_key");
	TiXmlText* lpNodeKeyText = new TiXmlText(lStr);
	lpNodeKeyElement->LinkEndChild(lpNodeKeyText);
	lpRootElement->LinkEndChild(lpNodeKeyElement);
    
	TiXmlElement* lRptManipulationElement = new TiXmlElement("report_manipulation");
    TiXmlElement* lRptManipulationMethodElement = new TiXmlElement("report_manipulation_method");
	TiXmlText* lValueText = new TiXmlText("7");
	lRptManipulationMethodElement->LinkEndChild(lValueText);
	lRptManipulationElement->LinkEndChild(lRptManipulationMethodElement);
    
    EnumDSSTemplateUnitType lUnitType = mpUnit->getType();
    if (lUnitType != DssTemplateMetrics)
    {
        TiXmlElement* lUnitIDXML = new TiXmlElement("unit_id");
        char lID[33];
        GUID lGUID = mpUnit->getID();
        
        DSSBaseElementProxy::ConvertGUIDToString(lGUID, lID);
        lStr = lID;
        TiXmlText* lUnitIDXMLContent = new TiXmlText(lStr);
        lUnitIDXML->LinkEndChild(lUnitIDXMLContent);
        lRptManipulationElement->LinkEndChild(lUnitIDXML);
        
        EnumDSSObjectType lObjecType = lObjectContext->FindObjectH(lGUID)->getType();
        TiXmlElement* lUnitTypeXML = new TiXmlElement("unit_type");
        TiXmlText* lUnitTypeXMLContent = new TiXmlText(DSSExpNode::IntToString(lObjecType).c_str());
        lUnitTypeXML->LinkEndChild(lUnitTypeXMLContent);
        lRptManipulationElement->LinkEndChild(lUnitTypeXML);
    }
    
    TiXmlElement* lAxisNameXML = new TiXmlElement("axis_name");
    TiXmlText* lAxisNameContentXML = new TiXmlText(DSSExpNode::IntToString(mDstAxisIndex).c_str());
    lAxisNameXML->LinkEndChild(lAxisNameContentXML);
    lRptManipulationElement->LinkEndChild(lAxisNameXML);
    
    TiXmlElement* lUnitPosXML = new TiXmlElement("unit_position");
     TiXmlText* lAxisNameContentXML = new TiXmlText(DSSExpNode::IntToString(mDstUnitPosition+1).c_str());
    lUnitPosXML->LinkEndChild(lUnitPosContentXML);
    lRptManipulationElement->LinkEndChild(lUnitPosXML);
    
    lpRootElement->LinkEndChild(lRptManipulationElement);
    
	TiXmlPrinter printer;
	printer.SetIndent(NULL);
	printer.SetLineBreak(NULL);
    lDoc.LinkEndChild(lpRootElement);
	lDoc.Accept( &printer );
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(printer.Str());
}
#endif

DSSRWTemplateNode* DSSCManipulationPivotUnit::getTemplateNode()
{
	return mpTemplateNode;
}
