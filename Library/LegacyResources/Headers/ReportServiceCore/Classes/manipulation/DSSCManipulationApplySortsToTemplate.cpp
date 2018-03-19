//
//  DSSCManipulationApplySortsToTemplate.cpp
//  ReportServiceCore
//
//  Created by Zhu, Wei on 12/3/12.
//
//

#include "DSSCManipulationApplySortsToTemplate.h"

#include "DSSCManipulationSortTemplate.h"
#include "DSSRWTemplateNode.h"
#include "DSSTemplateConsolidation.h"
#include "DSSConsolidation.h"
#include "DSSRWNode.h"
#include "DSSSorts.h"
#include "DSSTemplateCustomGroup.h"
#include "DSSCustomGroup.h"
#include "DSSDataModelXTabImpl.h"
#include "DSSTemplateDimension.h"
#include "DSSRWDataModel.h"

#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#include "DSSBaseElementProxy.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxml.h"
#else
#include "DSSSimpleXML.h"
#endif

DSSCManipulationApplySortsToTemplate::DSSCManipulationApplySortsToTemplate(DSSRWIterator* ipIterator, std::vector<SortInfo*>&ivSortInfo, EnumDSSSortFlag iSortFlag)
{
    mpIterator = ipIterator;
	mpTemplateNode = (DSSRWTemplateNode*)(ipIterator->getRWNode());
	mvSortInfo = ivSortInfo;
	mpOldSortsColumn = mpOldSortsRow = NULL;
    mbNeedClear = true;
    mSortFlag = iSortFlag;
    
    // for replay manipluations
    mID = mFormID = GUID_NULL;
    mAxisIndex = -1;
    mAscending = false;
    mSubtotalsPosition = -1;
    
    mType = ManipulationApplySortsToTemplate;
}

DSSCManipulationApplySortsToTemplate::DSSCManipulationApplySortsToTemplate(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationApplySortsToTemplate;
    
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
    std::string lXMLString = lpAO->getXML();
#ifndef __ANDROID__
    hParseAOXML(lXMLString, ipContext);
#endif

     mIsFromActionObject = true;
}

DSSCManipulationApplySortsToTemplate::~DSSCManipulationApplySortsToTemplate()
{
    if(mpOldSortsRow)
    {
        delete mpOldSortsRow;
        mpOldSortsRow = NULL;
    }
    
    if(mpOldSortsColumn)
    {
        delete mpOldSortsColumn;
        mpOldSortsColumn = NULL;
    }
}

#ifndef __ANDROID__
void DSSCManipulationApplySortsToTemplate::GenerateAODeltaXMLClear(int iAxisId, int iSubtotalPosition)
{
    SimpleXMLNode lAxisXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation"));
    SimpleXMLNode lAxisManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation_method"));
    SimpleXMLNode lAxisManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("27", true));
    lAxisManipulationMethodXML.AddChild(lAxisManipulationMethodXMLContent);
    
    SimpleXMLNode lAxisNameXML(SimpleXMLBuilder::CreateNodePtr("axis_name"));
    SimpleXMLNode lAxisNameXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(iAxisId).c_str(), true));
    lAxisNameXML.AddChild(lAxisNameXMLContent);
    
    lAxisXML.AddChild(lAxisManipulationMethodXML);
    lAxisXML.AddChild(lAxisNameXML);
    mvReportManipulationXMLs.push_back(lAxisXML);
    
    if(iSubtotalPosition > 0)
    {
        SimpleXMLNode lReportAxisXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation"));
        SimpleXMLNode lReportAxisManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation_method"));
        SimpleXMLNode lReportAxisManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("26", true));
        lReportAxisManipulationMethodXML.AddChild(lReportAxisManipulationMethodXMLContent);
        
        SimpleXMLNode lReportAxisNameXML(SimpleXMLBuilder::CreateNodePtr("axis_name"));
        SimpleXMLNode lReportAxisNameXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(iAxisId).c_str(), true));
        lReportAxisNameXML.AddChild(lReportAxisNameXMLContent);
        
        SimpleXMLNode lSortTypeXML(SimpleXMLBuilder::CreateNodePtr("sort_type"));
        SimpleXMLNode lSortTypeXMLContent(SimpleXMLBuilder::CreateNodePtr("4", true));
        lSortTypeXML.AddChild(lSortTypeXMLContent);
        lReportAxisXML.AddChild(lSortTypeXML);
        
        SimpleXMLNode lSubtotalXML(SimpleXMLBuilder::CreateNodePtr("subtotals_position"));
        SimpleXMLNode lSubtotalXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(iSubtotalPosition).c_str(), true));
        lSubtotalXML.AddChild(lSubtotalXMLContent);
        lReportAxisXML.AddChild(lSubtotalXML);
        
        mvReportManipulationXMLs.push_back(lReportAxisXML);
    }
}

void DSSCManipulationApplySortsToTemplate::GenerateAODeltaXML(int iSortIndex, int iSortType, int iObjType, GUID& iObjID, bool iAscending, int iAxisId,  int iSubtotalsPosition,  DSSBaseElementsProxy* ipElements, GUID iFormID)
{
    DSSTemplate* lpTemplate = mpTemplateNode->getTemplate();
    DSSSorts* lSorts = NULL;
    
    if (iAxisId == 1)
        lSorts = lpTemplate->getRow()->getSorts();
    else
        lSorts = lpTemplate->getColumn()->getSorts();
    
    if (mbNeedClear && lSorts)
    {
        SimpleXMLNode lAxisXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation"));
        SimpleXMLNode lAxisManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation_method"));
        SimpleXMLNode lAxisManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("27", true));
        lAxisManipulationMethodXML.AddChild(lAxisManipulationMethodXMLContent);
        
        SimpleXMLNode lAxisNameXML(SimpleXMLBuilder::CreateNodePtr("axis_name"));
        SimpleXMLNode lAxisNameXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(iAxisId).c_str(), true));
        lAxisNameXML.AddChild(lAxisNameXMLContent);
        
        lAxisXML.AddChild(lAxisManipulationMethodXML);
        lAxisXML.AddChild(lAxisNameXML);
        //lXML.AddChild(lAxisXML);
        
        mvReportManipulationXMLs.push_back(lAxisXML);
        mbNeedClear = false;
    }
        
    SimpleXMLNode lReportAxisXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation"));
    SimpleXMLNode lReportAxisManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation_method"));
    SimpleXMLNode lReportAxisManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("26", true));
    lReportAxisManipulationMethodXML.AddChild(lReportAxisManipulationMethodXMLContent);
    
    SimpleXMLNode lReportAxisNameXML(SimpleXMLBuilder::CreateNodePtr("axis_name"));
    SimpleXMLNode lReportAxisNameXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(iAxisId).c_str(), true));
    lReportAxisNameXML.AddChild(lReportAxisNameXMLContent);
    
    lReportAxisXML.AddChild(lReportAxisManipulationMethodXML);
    lReportAxisXML.AddChild(lReportAxisNameXML);
    //lReportXML.AddChild(lReportAxisXML);
    
    SimpleXMLNode lSortTypeXML(SimpleXMLBuilder::CreateNodePtr("sort_type"));
    
    if (iSortType == 1)
    {
        SimpleXMLNode lSortTypeXMLContent(SimpleXMLBuilder::CreateNodePtr("4", true));
        lSortTypeXML.AddChild(lSortTypeXMLContent);
    } else if (iSortType == 2 || iSortType == 3)
    {
        SimpleXMLNode lSortTypeXMLContent(SimpleXMLBuilder::CreateNodePtr("2", true));
        lSortTypeXML.AddChild(lSortTypeXMLContent);
    }
    
    lReportAxisXML.AddChild(lSortTypeXML);
    
    SimpleXMLNode lAscendingXML(SimpleXMLBuilder::CreateNodePtr("is_ascending"));
    SimpleXMLNode lAscendingXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(iAscending?1:0).c_str(), true));
    lAscendingXML.AddChild(lAscendingXMLContent);
    lReportAxisXML.AddChild(lAscendingXML);
    
    SimpleXMLNode lSubtotalXML(SimpleXMLBuilder::CreateNodePtr("subtotals_position"));
    SimpleXMLNode lSubtotalXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(iSubtotalsPosition).c_str(), true));
    lSubtotalXML.AddChild(lSubtotalXMLContent);
    lReportAxisXML.AddChild(lSubtotalXML);
    
    SimpleXMLNode lUnitIDXML(SimpleXMLBuilder::CreateNodePtr("unit_id"));
    char lID[33];
    DSSBaseElementProxy::ConvertGUIDToString(iObjID, lID);
    SimpleXMLNode lUnitIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lID, true));
    lUnitIDXML.AddChild(lUnitIDXMLContent);
    lReportAxisXML.AddChild(lUnitIDXML);
    
    SimpleXMLNode lUnitTypeXML(SimpleXMLBuilder::CreateNodePtr("unit_type"));
    SimpleXMLNode lUnitTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(iObjType).c_str(), true));
    lUnitTypeXML.AddChild(lUnitTypeXMLContent);
    lReportAxisXML.AddChild(lUnitTypeXML);
    
    if (iFormID != GUID_NULL)
    {
        SimpleXMLNode lFormIDXML(SimpleXMLBuilder::CreateNodePtr("form_id"));
        char lFormID[33];
        DSSBaseElementProxy::ConvertGUIDToString(iFormID, lFormID);
        SimpleXMLNode lFormIDXMLContent(SimpleXMLBuilder::CreateNodePtr(lFormID, true));
        lFormIDXML.AddChild(lFormIDXMLContent);
        lReportAxisXML.AddChild(lFormIDXML);
    }
    
    mvReportManipulationXMLs.push_back(lReportAxisXML);
}

void DSSCManipulationApplySortsToTemplate::GenerateAODeltaXML()
{
    mpActionObject = new DSSAODeltaXML(ManipulationApplySortsToTemplate);
    
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("19", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    
    int lTreeType = static_cast<int>(mpTemplateNode->getTreeType());
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    
    if (mvReportManipulationXMLs.size() > mvSortInfo.size())
        lXML.AddChild(mvReportManipulationXMLs[0]);
    
    for (int i = mvReportManipulationXMLs.size() - 1; i >= 1; i--)
        lXML.AddChild(mvReportManipulationXMLs[i]);
    
    lXML.AddChild(lManipulationMethodXML);
    lXML.AddChild(lTreeTypeXML);
    lXML.AddChild(lNodeKeyXML);
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationApplySortsToTemplate::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
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
                mpTemplateNode = static_cast<DSSRWTemplateNode*>(ipContext->getNode(lStrContent));
            }
            else if(lCurrentNode.GetName().compare("report_manipulation") == 0)
            {
                for(SimpleXMLNode lSortNode = lCurrentNode.GetFirstChild(); lSortNode.IsValid(); lSortNode = lSortNode.GetNextSibling())
                {
                    if (lSortNode.GetName().compare("rw_manipulation_method") == 0)
                    {
                        lSortNode.GetContent(lStrContent);
                        if (lStrContent.compare("26") != 0)
                            continue;
                    }
                    else if(lSortNode.GetName().compare("axis_name") == 0)
                    {
                        lSortNode.GetContent(lStrContent);
                        mAxisIndex = static_cast<EnumDSSAxisName>(DSSExpNode::StringToInt(lStrContent));
                    }
                    else if(lSortNode.GetName().compare("unit_id") == 0)
                    {
                        lSortNode.GetContent(lStrContent);
                        DSSBaseElementProxy::ConvertFromStringToGUID(lStrContent, mID);
                    }
                    else if(lSortNode.GetName().compare("form_id") == 0)
                    {
                        lSortNode.GetContent(lStrContent);
                        DSSBaseElementProxy::ConvertFromStringToGUID(lStrContent, mFormID);
                    } else if (lSortNode.GetName().compare("is_ascending") == 0)
                    {
                        lSortNode.GetContent(lStrContent);
                        mAscending = (lStrContent.compare("1") == 0);
                    } else if (lSortNode.GetName().compare("subtotals_position") == 0)
                    {
                        lSortNode.GetContent(lStrContent);
                        mSubtotalsPosition = DSSExpNode::StringToInt(lStrContent);
                    }
                }
            }
        }
    }
     
}
#else
// TODO - For Android
#endif

int DSSCManipulationApplySortsToTemplate::Execute()
{
    if (!mpTemplateNode)
        return S_FALSE;
    
    DSSObjectContext* lpObjectContext = mpTemplateNode->getObjectContext();
    
    if (mIsFromActionObject) // for replay manipulations
    {
        DSSTemplate* lpTemplate = mpTemplateNode->getTemplate();
        DSSAxis* lpAxis = NULL;
        
        if (mAxisIndex==DssAxisRows)
			lpAxis = lpTemplate->getRow();
		else if (mAxisIndex==DssAxisColumns)
			lpAxis = lpTemplate->getColumn();
		else
			return S_OK;
		
		DSSSorts* lpSorts = lpAxis->getSorts();
		if (!lpSorts)
		{
			lpSorts = new DSSSorts(lpObjectContext);
			lpAxis->setSorts(lpSorts);
		}
		else
			lpSorts->Clear();
		
		DSSSort* lpNewSort = new DSSSort(lpObjectContext);
		DSSObjectInfo* lpObject = lpObjectContext->FindObjectH(mID);
		if (!lpObject)
			return S_OK;

		
		EnumDSSObjectType lObjectType = lpObject->getType();
		if (lObjectType==DssTypeAttribute)
		{
			if (MBase::IsEqualGUID(mFormID,GUID_NULL))
				lpNewSort->setSortType(DssSortAttributeDefault);
			else
				lpNewSort->setSortType(DssSortAttributeForm);
		}
		else if (lObjectType==DssTypeMetric)
			lpNewSort->setSortType(DssSortMetric);
		else if (lObjectType==DssTypeConsolidation)
			lpNewSort->setSortType(DssSortConsolidationDefault);
		else
			return S_OK;
        
		lpNewSort->setAscending(mAscending);
		lpNewSort->setObjectID(mID);
		lpNewSort->setFormID(mFormID);
		
        lpNewSort->setSubtotalPosition(mSubtotalsPosition);
		
		lpSorts->Add(lpNewSort);
		
        IDSSTemplateModel *lpTemplateDataModel = lpTemplate->getTemplateModelInterface();
        if (lpTemplateDataModel)
        {
            lpTemplateDataModel->Refresh();
        }
        return S_OK;
    }
    
    HRESULT hr = S_OK;
    DSSTemplate* lpTemplate = mpTemplateNode->getTemplate();
    
    int lSubtotalsPositionRow, lSubtotalsPositionColumn;
    lSubtotalsPositionRow = lSubtotalsPositionColumn = -1;
    DSSSorts* lSortsRow = lpTemplate->getRow()->getSorts();
    DSSSorts* lSortsColumn = lpTemplate->getColumn()->getSorts();
    bool lHasOldSortsRow, lHasOldSortsColumn;
    lHasOldSortsRow = lHasOldSortsColumn = false;
    int lCount = 0;
    
    if (lSortsRow)
    {
        lCount = lSortsRow->Count();
        if (lCount > 0)
        {
            lSubtotalsPositionRow = lSortsRow->Item(0)->getSubtotalPosition();
            lHasOldSortsRow = true;
            mpOldSortsRow = new DSSSorts(lpObjectContext);
            for(int i = 0; i < lSortsRow->Count(); i++)
            {
                DSSSort* lpOldSortRow = new DSSSort(lpObjectContext);
                lpOldSortRow->Duplicate(lSortsRow->Item(i));
                mpOldSortsRow->Add(lpOldSortRow);
            }
        }
        lSortsRow = new DSSSorts(lpObjectContext);
        lpTemplate->getRow()->setSorts(lSortsRow);
    }
    
    if (lSortsColumn)
    {
        lCount = lSortsColumn->Count();
        if (lCount > 0)
        {
            lSubtotalsPositionColumn = lSortsColumn->Item(0)->getSubtotalPosition();
            lHasOldSortsColumn = true;
            mpOldSortsColumn = new DSSSorts(lpObjectContext);
            for(int i = 0; i < lSortsRow->Count(); i++)
            {
                DSSSort* lpOldSortColumn = new DSSSort(lpObjectContext);
                lpOldSortColumn->Duplicate(lSortsRow->Item(i));
                mpOldSortsRow->Add(lpOldSortColumn);
            }
        }
        lSortsColumn = new DSSSorts(lpObjectContext);
        lpTemplate->getColumn()->setSorts(lSortsColumn);
    }
    
    if (mvSortInfo.size() == 0)
    {
        if(lHasOldSortsRow)
        {
            DSSSort* lpNewSort = new DSSSort(lpObjectContext);
            lpNewSort->setSortType(DssSortSubtotalsPosition);
            lpNewSort->setSubtotalPosition(lSubtotalsPositionRow);
            lSortsRow->Add(lpNewSort);
            GenerateAODeltaXMLClear(1, lSubtotalsPositionRow);
        }
        if(lHasOldSortsColumn)
        {
            DSSSort* lpNewSort = new DSSSort(lpObjectContext);
            lpNewSort->setSortType(DssSortSubtotalsPosition);
            lpNewSort->setSubtotalPosition(lSubtotalsPositionColumn);
            lSortsRow->Add(lpNewSort);
            GenerateAODeltaXMLClear(2, lSubtotalsPositionColumn);
        }
        if(!lHasOldSortsRow && !lHasOldSortsColumn)
            GenerateAODeltaXMLClear(0, -1);
    }
    
    int lObjType = 0;
    GUID lObjectID = GUID_NULL;
    GUID lFormID = GUID_NULL;
    EnumDSSSortType lSortType;
    bool lNeedServer = false;
    DSSMetric* lpMetric = NULL;
    
    for (int i = 0; i < mvSortInfo.size(); i++)
    {
        int lAxisIndex = 0;
        DSSAxis* lpAxis = NULL;
        
        SortInfo* lSortInfo = mvSortInfo[i];
        bool lAscending = lSortInfo->mAscending;
        DSSCellHandle* lCellHandle = lSortInfo->mpCellHandle;
        
        if(lCellHandle->type == DssCellHandleXTabHeaderElement)
        {
            lObjType = DssTypeMetric;
            
            DSSBaseElementsProxy* lElements = new DSSBaseElementsProxy(lpObjectContext);
            
            if (lSortInfo->mvElements.size() == 0)
            {
                std::vector<DSSBaseElementProxy*> lvElements;
                hr = mpIterator->createSiblingElementsFromHandle(*lCellHandle, false, lvElements);
                if (hr != S_OK)
                {
                    //yimliu, 2014/06/12, 909459, Memory leak.
                    delete lElements;
                    lElements = NULL;
                    return hr;
                }
                
                for (int j = 0; j < lvElements.size(); j++)
                    if (lvElements[j])
                    lElements->AddElement(lvElements[j]);
            }
            else
            {
                for (int j = 0; j < lSortInfo->mvElements.size(); j++)
                    lElements->AddElement(lSortInfo->mvElements[j]);
            }
            
            for (int j = 0; j < lElements->Count(); j++)
            {
                if (lElements->Item(j)->getElementType() == DssElementMetric)
                {
                    lpMetric = ((DSSMetricElementProxy*)(lElements->Item(j)))->getMetric();
                    lObjectID = lpMetric->getID();
                    break;
                }
            }
            
            if (lpMetric)
            {
                lObjectID = lpMetric->getID();
                
                if (lSortInfo->mvElements.size() == 0)
                {
                    if (mpIterator->getAxisFromHandle(*lCellHandle) == DssAxisRows)
                    {
                        lAxisIndex = DssAxisColumns;
                        lpAxis = lpTemplate->getColumn();
                    }
                    else if (mpIterator->getAxisFromHandle(*lCellHandle) == DssAxisColumns)
                    {
                        lAxisIndex = DssAxisRows;
                        lpAxis = lpTemplate->getRow();
                    }
                    else
                    {
                        //yimliu, 2014/06/12, 909459, Memory leak.
                        delete lElements;
                        lElements = NULL;
                        return S_OK;
                    }
                }
                else
                {
                    lAxisIndex = mpIterator->getAxisFromHandle(*lCellHandle);
                    if (lAxisIndex == DssAxisRows)
                        lpAxis = lpTemplate->getRow();
                    else if (lAxisIndex == DssAxisColumns)
                        lpAxis = lpTemplate->getColumn();
                }
                
                DSSSorts* lpSorts = lpAxis->getSorts();
                DSSSort* lpNewSort = new DSSSort(lpObjectContext);
                lpNewSort->setSortType(DssSortMetric);
                lSortType = DssSortMetric;
                lpNewSort->setAscending(lAscending);
                lpNewSort->setObjectID(lpMetric->getID());
                DSSBaseElementsProxy* lpElements = new DSSBaseElementsProxy(lpObjectContext);
                for (int j = 0; j < lElements->Count(); j++)
                {
                    DSSBaseElementProxy* lpElement = DSSBaseElementsProxy::Clone(lElements->Item(j), lpObjectContext);
                    if (lpElement)
                        lpElements->AddElement(lpElement);
                }
                lpNewSort->setElements(lpElements);
                
                if (lAxisIndex==DssAxisRows) // && lSubtotalsPositionRow>0)
                {
                    int lOldSubtotalsPositionRow = DSSCManipulationSortTemplate::getSubtotalPosition(mpOldSortsRow, lpMetric->getID());
                    if(lOldSubtotalsPositionRow != -1)
                        lSubtotalsPositionRow = lOldSubtotalsPositionRow;
                    lpNewSort->setSubtotalPosition(lSubtotalsPositionRow);
                }
                else if (lAxisIndex==DssAxisColumns) // && lSubtotalsPositionColumn>0)
                {
                    int lOldSubtotalPositionColumn = DSSCManipulationSortTemplate::getSubtotalPosition(mpOldSortsColumn, lpMetric->getID());
                    if(lOldSubtotalPositionColumn != -1)
                        lSubtotalsPositionRow = lOldSubtotalPositionColumn;
                    lpNewSort->setSubtotalPosition(lSubtotalsPositionColumn);
                }
                lpSorts->Add(lpNewSort);
                
                GenerateAODeltaXML(i, 1, lObjType, lObjectID, lAscending, lAxisIndex, lpNewSort->getSubtotalPosition(), lpElements, GUID_NULL);
            }
            
            //yimliu, 2014/06/12, 909459, Memory leak.
            if (lElements!=NULL) {
                delete lElements;
                lElements = NULL;
            }
        }
        else if (lCellHandle->type == DssCellHandleTemplateUnit)
        {
            DSSTemplateUnit* lpUnit = lCellHandle->p.unit;
            lpAxis = lpUnit->getAxis();
            if (!lpAxis)
                return S_OK;
            
            lAxisIndex = EnumDSSAxisName(lpAxis->getIndex());
            
            int lBaseFormID = -1;
            
            if (lAxisIndex == DssAxisRows)
                lpAxis = mpTemplateNode->getTemplate()->getRow();
            else if (lAxisIndex == DssAxisColumns)
                lpAxis = mpTemplateNode->getTemplate()->getColumn();
            
            DSSSorts* lpSorts = lpAxis->getSorts();
            DSSSort* lpNewSort = new DSSSort(lpObjectContext);
            
            if(lpUnit->getType() == DssTemplateAttribute)
            {
                lSortType = DssSortAttributeDefault;
                lObjType = DssTypeAttribute;
                mpIterator->getFormFromHandle(*lCellHandle, lFormID);
                lBaseFormID = hFindMatchedBaseFormID((DSSTemplateAttribute*)(lpUnit), lFormID);
                lObjectID = lpUnit->getID();
            }
            else if(lpUnit->getType() == DssTemplateConsolidation)
            {
                lSortType = DssSortConsolidationDefault;
                lObjType = DssTypeConsolidation;
                DSSConsolidation* lpConsolidation = ((DSSTemplateConsolidation*) lpUnit)->getConsolidation();
                lObjectID = lpConsolidation->getID();
            } else if(lpUnit->getType() == DssTemplateCustomGroup)
            {
                lObjType = DssTypeFilter;
                lSortType = DssSortCustomGroupDefault;
                DSSCustomGroup* lpCustomGroup = ((DSSTemplateCustomGroup*) lpUnit)->getCustomGroup();
                lObjectID = lpCustomGroup->getID();
                lNeedServer = true;
            }
            
            lpNewSort->setSortType(lSortType);
            lpNewSort->setAscending(lAscending);
            lpNewSort->setObjectID(lObjectID);
            lpNewSort->setFormID(lFormID);
            if (lBaseFormID>=0)
                lpNewSort->setBaseFormID(lBaseFormID);
            
            if (lAxisIndex==DssAxisRows)// && lSubtotalsPositionRow>0)
            {
                int lOldSubtotalsPositionRow = DSSCManipulationSortTemplate::getSubtotalPosition(mpOldSortsRow, lObjectID, lFormID);
                if(lOldSubtotalsPositionRow != -1)
                    lSubtotalsPositionRow = lOldSubtotalsPositionRow;
                lpNewSort->setSubtotalPosition(lSubtotalsPositionRow);
            }
            else if (lAxisIndex==DssAxisColumns)// && lSubtotalsPositionColumn>0)
            {
                int lOldSubtotalPositionColumn = DSSCManipulationSortTemplate::getSubtotalPosition(mpOldSortsColumn, lObjectID, lFormID);
                if(lOldSubtotalPositionColumn != -1)
                    lSubtotalsPositionRow = lOldSubtotalPositionColumn;
                lpNewSort->setSubtotalPosition(lSubtotalsPositionColumn);
            }
            lpSorts->Add(lpNewSort);
            
            GenerateAODeltaXML(i, 2, lObjType, lObjectID, lAscending, lAxisIndex, lpNewSort->getSubtotalPosition(), NULL, lFormID);
        }
        else if (lCellHandle->type == DssCellHandleTemplateUnitAndForm) // form sort
        {
            lObjType = DssTypeAttribute;
            DSSTemplateUnit* lpUnit = lCellHandle->p.unit;
            lpAxis = lpUnit->getAxis();
            lAxisIndex = EnumDSSAxisName(lpAxis->getIndex());
            
            DSSSorts* lpSorts = lpAxis->getSorts();
            lFormID = GUID_NULL;
            mpIterator->getFormFromHandle(*lCellHandle, lFormID);
            
            int lBaseFormID = -1;
            if  (lpUnit->getType()==DssTemplateAttribute)
            {
                lObjectID = lpUnit->getID();
                lBaseFormID = hFindMatchedBaseFormID((DSSTemplateAttribute*)(lpUnit), lFormID);
            }
            else if (lpUnit->getType()==DssTemplateDimension)
            {
                DSSTemplateDimension* lpDimension = (DSSTemplateDimension*)lpUnit;
                for (int j=0; j<lpDimension->Count(); j++)
                {
                    lBaseFormID = hFindMatchedBaseFormID(lpDimension->Item(j), lFormID);
                    if (lBaseFormID>=0)
                    {
                        lObjectID = lpDimension->Item(j)->getID();
                        break;
                    }
                }
            }
            
            DSSSort* lpNewSort = new DSSSort(lpObjectContext);
            lpNewSort->setSortType(DssSortAttributeForm);
            lSortType = DssSortAttributeForm;
            lpNewSort->setObjectID(lObjectID);
            lpNewSort->setAscending(lAscending);
            lpNewSort->setFormID(lFormID);
            if (lBaseFormID>=0)
                lpNewSort->setBaseFormID(lBaseFormID);
            
            if (lAxisIndex==DssAxisRows)// && lSubtotalsPositionRow>0)
            {
                int lOldSubtotalsPositionRow = DSSCManipulationSortTemplate::getSubtotalPosition(mpOldSortsRow, lObjectID, lFormID);
                if(lOldSubtotalsPositionRow != -1)
                    lSubtotalsPositionRow = lOldSubtotalsPositionRow;
                lpNewSort->setSubtotalPosition(lSubtotalsPositionRow);
            }
            else if (lAxisIndex==DssAxisColumns)// && lSubtotalsPositionColumn>0)
            {
                int lOldSubtotalPositionColumn = DSSCManipulationSortTemplate::getSubtotalPosition(mpOldSortsColumn, lObjectID, lFormID);
                if(lOldSubtotalPositionColumn != -1)
                    lSubtotalsPositionRow = lOldSubtotalPositionColumn;
                lpNewSort->setSubtotalPosition(lSubtotalsPositionColumn);
            }
            lpSorts->Add(lpNewSort);
            
            GenerateAODeltaXML(i, 3, lObjType, lObjectID, lAscending, lAxisIndex, lpNewSort->getSubtotalPosition(), NULL, lFormID);
            
            // dli, 856514, add a ID form sort if microchart tree mode
            if(mSortFlag == DssSortFlagMicroChartTreeMode && lBaseFormID > 0)
            {
                GUID lIDFormID = GUID_NULL;
                if  (lpUnit->getType()==DssTemplateAttribute)
                {
                    DSSAttribute* lpAttribute = (static_cast<DSSTemplateAttribute*>(lpUnit))->getAttribute();
                    if(lpAttribute)
                    {
                        DSSAttributeFormWrapper* lpIDForm = lpAttribute->getIDFormWrapper();
                        if(lpIDForm)
                        {
                            lIDFormID = lpIDForm->getID();
                        }
                    }
                }
                else if (lpUnit->getType()==DssTemplateDimension)
                {
                    DSSTemplateDimension* lpDimension = (DSSTemplateDimension*)lpUnit;
                    for (int j=0; j<lpDimension->Count(); j++)
                    {
                        lBaseFormID = hFindMatchedBaseFormID(lpDimension->Item(j), lFormID);
                        if (lBaseFormID>=0)
                        {
                            DSSAttribute* lpAttribute = (static_cast<DSSTemplateAttribute*>(lpDimension->Item(j)))->getAttribute();
                            if(lpAttribute)
                            {
                                DSSAttributeFormWrapper* lpIDForm = lpAttribute->getIDFormWrapper();
                                if(lpIDForm)
                                {
                                    lIDFormID = lpIDForm->getID();
                                }
                            }
                            break;
                        }
                    }
                }
                
                if(lIDFormID != GUID_NULL)
                {
                    DSSSort* lpIDFormSort = new DSSSort(lpObjectContext);
                    lpIDFormSort->setSortType(DssSortAttributeForm);
                    lSortType = DssSortAttributeForm;
                    lpIDFormSort->setObjectID(lObjectID);
                    lpIDFormSort->setAscending(lAscending);
                    lpIDFormSort->setFormID(lIDFormID);
                    lpIDFormSort->setBaseFormID(0);
                    
                    if (lAxisIndex==DssAxisRows)// && lSubtotalsPositionRow>0)
                    {
                        int lOldSubtotalsPositionRow = DSSCManipulationSortTemplate::getSubtotalPosition(mpOldSortsRow, lObjectID, lFormID);
                        if(lOldSubtotalsPositionRow != -1)
                            lSubtotalsPositionRow = lOldSubtotalsPositionRow;
                        lpIDFormSort->setSubtotalPosition(lSubtotalsPositionRow);
                    }
                    else if (lAxisIndex==DssAxisColumns)// && lSubtotalsPositionColumn>0)
                    {
                        int lOldSubtotalPositionColumn = DSSCManipulationSortTemplate::getSubtotalPosition(mpOldSortsColumn, lObjectID, lFormID);
                        if(lOldSubtotalPositionColumn != -1)
                            lSubtotalsPositionRow = lOldSubtotalPositionColumn;
                        lpIDFormSort->setSubtotalPosition(lSubtotalsPositionColumn);
                    }
                    lpSorts->Add(lpIDFormSort);
                    
                    GenerateAODeltaXML(i, 3, lObjType, lObjectID, lAscending, lAxisIndex, lpIDFormSort->getSubtotalPosition(), NULL, lIDFormID);
                }
            }
        }
        else
        {
            _ASSERT(false);
        }
    }
    
#ifndef __ANDROID__
    GenerateAODeltaXML();
#endif
    
    if (lNeedServer)
    {
        std::vector<std::string> lNodes = mpTemplateNode->GetServerNodeKeys();
        int lSize = lNodes.size();
        for (int i = 0; i < lSize; i++)
        {
            std::string lKey = lNodes[i];
            if (mNodeMapServer.find(lKey) != mNodeMapServer.end())
                continue;
            
            DSSRWNode* lNode = lpObjectContext->getNode(lKey);
            
            mNodesModelChanged.push_back(lNode);
            mNodeMapServer[lKey] = lNode;
        }
    }
    else
    {
        IDSSTemplateModel *lpTemplateDataModel = lpTemplate->getTemplateModelInterface();
        if (lpTemplateDataModel)
        {
            lpTemplateDataModel->Refresh();
        }
    }

	return S_OK;
}

int DSSCManipulationApplySortsToTemplate::hFindMatchedBaseFormID(DSSTemplateAttribute* ipAttribute, GUID iFormID)
{
	DSSAttributeFormWrappers* lpForms = ipAttribute->getFormWrappers();
	for (int i=0; i<lpForms->Count(); i++)
		if(MBase::IsEqualGUID(lpForms->Item(i)->getID(), iFormID))
			return lpForms->Item(i)->getBaseFormID();
	return -1;
}