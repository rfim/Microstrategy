/*
 *  DSSCManipulationSortTemplate.cpp
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-21.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSCManipulationSortTemplate.h"
#include "DSSRWTemplateNode.h"
#include "DSSDataModelXTabImpl.h"
#include "DSSTemplateDimension.h"

#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#include "DSSBaseElementProxy.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif
#include "DSSConsolidation.h"
#include "DSSCManipulationHelper.h"
#include "DSSRWGridModel.h"
#include "DSSRWDataModel.h"

DSSCManipulationSortTemplate::DSSCManipulationSortTemplate(DSSRWIterator* ipIterator, DSSCellHandle* ipCellHandle, bool iAscending, bool iAOXML, EnumDSSSortFlag iSortFlag)
{
	mpTemplateNode = (DSSRWTemplateNode*)(ipIterator->getRWNode());
	mpCellHandle = ipCellHandle;
	mAscending = iAscending;
	mpIterator = ipIterator;
	mIsFromActionObject = false;
    mAOXML = iAOXML;
    mSortFlag = iSortFlag;
    mAxisIndex = DssAxisReserved;
    
    mType = ManipulationSortTemplate;
    mObjectType = DssTypeReserved;
    mpElementIDs = NULL;
    mIsClearSort = false;
    mBaseFormID = -1;
    mSortType = DssSortReserved;
}

DSSCManipulationSortTemplate::DSSCManipulationSortTemplate(DSSRWIterator* ipIterator, DSSCellHandle* ipCellHandle)
{
    mpIterator = ipIterator;
    mpTemplateNode = (DSSRWTemplateNode*)(ipIterator->getRWNode());
    mpCellHandle = ipCellHandle;
    mAscending = false;
    mIsFromActionObject = false;
    mAOXML = true;
    mSortFlag = DssSortFlagDefault;
    mAxisIndex = DssAxisReserved;
    
    mType = ManipulationSortTemplate;
    mObjectType = DssTypeReserved;
    mpElementIDs = NULL;
    mIsClearSort = true;
    mBaseFormID = -1;
    mSortType = DssSortReserved;
}

DSSCManipulationSortTemplate::DSSCManipulationSortTemplate(DSSRWIterator* ipIterator, int iAxisIndex)
{
    mpIterator = ipIterator;
    mpTemplateNode = (DSSRWTemplateNode*)(ipIterator->getRWNode());
    mpCellHandle = NULL;
    mAscending = false;
    mIsFromActionObject = false;
    mAOXML = true;
    mSortFlag = DssSortFlagDefault;
    mAxisIndex = EnumDSSAxisName(iAxisIndex);

    mType = ManipulationSortTemplate;
    mObjectType = DssTypeReserved;
    mpElementIDs = NULL;
    mIsClearSort = true;
    mBaseFormID = -1;
    mSortType = DssSortReserved;
}

DSSCManipulationSortTemplate::DSSCManipulationSortTemplate(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationSortTemplate;
    mpElementIDs = NULL;
    mBaseFormID = -1;
    mSortType = DssSortReserved;
    if(ipAO->IsDeltaXML())
    {
        DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*>(ipAO);
        std::string lXMLString = lpAO->getXML();

        hParseAOXML(lXMLString, ipContext);
    }
    else
    {
        DSSAOSort* lpAO = static_cast<DSSAOSort*>(ipAO);
        mpTemplateNode = (DSSRWTemplateNode*)(ipContext->getNode(lpAO->getNodeKey()));
        mAscending = lpAO->getAscending();
        
        mAxisIndex = lpAO->getAxisIndex();
        mID = lpAO->getObjectID();
        mFormID = lpAO->getFormID();

        mpElementIDs = new StringVector;
        for (StringVector::iterator it = lpAO->getElementIDs()->begin() ; it != lpAO->getElementIDs()->end(); it++)
            mpElementIDs->push_back(*it);
    }
    
    mIsFromActionObject = true;
    mIsClearSort = false;
}

DSSCManipulationSortTemplate::~DSSCManipulationSortTemplate()
{
    if (mpElementIDs)
    {
        delete mpElementIDs;
        mpElementIDs = NULL;
    }
}

bool DSSCManipulationSortTemplate::hInit()
{
    // check ActionObject's status of member variant to ensure it's not clear sort
    if (!mIsClearSort)
    {
        if (mIsFromActionObject && (mID == GUID_NULL))
            mIsClearSort = true;
    }
    
    DSSObjectContext* lpObjectContext = mpTemplateNode->getObjectContext();
    DSSTemplate* lpTemplate = mpTemplateNode->getTemplate();
    int lSubtotalsPositionRow = -1;
    int lSubtotalsPositionColumn = -1;
    
    DSSSorts* lpSortsRow = lpTemplate->getRow()->getSorts();
    DSSSorts* lpSortsColumn = lpTemplate->getColumn()->getSorts();
    
    if (lpSortsRow && lpSortsRow->Count()>0)
    {
        lSubtotalsPositionRow = lpSortsRow->Item(0)->getSubtotalPosition();
    }
    
    if (lpSortsColumn && lpSortsColumn->Count()>0)
    {
        lSubtotalsPositionColumn = lpSortsColumn->Item(0)->getSubtotalPosition();
    }
    
    // set axis / objectType / elementIDs / mID / sortType / subtotalPosition
    if ((!mIsFromActionObject) && (mpCellHandle)) {
        switch (mpCellHandle->type)
        {
            case DssCellHandleXTabHeaderElement: //metric sort or attribute sort
            {
                {
                    // metric sort
                    mObjectType = DssTypeMetric;
                    mSortType = DssSortMetric;
                    
                    std::vector<DSSBaseElementProxy*> lElements;
                    int hr = mpIterator->createSiblingElementsFromHandle(*mpCellHandle, false, lElements);
                    if (hr != S_OK)
                        break;
                    
                    DSSMetric* lpMetric = NULL;
                    std::vector<DSSBaseElementProxy*>::iterator it;
                    for ( it=lElements.begin() ; it != lElements.end(); it++ )
                    {
                        if ((*it)->getElementType()==DssElementMetric)
                        {
                            lpMetric = ((DSSMetricElementProxy*)(*it))->getMetric();
                            mID = lpMetric->getID();
                            break;
                        }
                    }
                    
                    // set elementIDs and release the point
                    mpElementIDs = new StringVector;
                    for (it=lElements.begin() ; it != lElements.end(); it++)
                        mpElementIDs->push_back((*it)->getElementID());
                    
                    for ( it=lElements.begin() ; it != lElements.end(); it++ )
                    {
                        if (*it)
                            delete *it;
                    }
                    
                    if (lpMetric)
                    {
                        if (mpIterator->getAxisFromHandle(*mpCellHandle)==DssAxisRows)
                        {
                            mAxisIndex = DssAxisColumns;
                        }
                        else if (mpIterator->getAxisFromHandle(*mpCellHandle)==DssAxisColumns)
                        {
                            mAxisIndex = DssAxisRows;
                        }
                        else
                            break;
                    }
                }
                break;
            }
                
            case DssCellHandleTemplateUnit:
            case DssCellHandleCustomGroupAndDepth:
            {
                DSSTemplateUnit* lpUnit = mpCellHandle->p.unit;
                DSSAxis* lpAxis = lpUnit->getAxis();
                
                if (!lpAxis)
                    break;
                
                mAxisIndex = EnumDSSAxisName(lpAxis->getIndex());
                
                if(lpUnit->getType() == DssTemplateAttribute)
                {
                    mObjectType = DssTypeAttribute;
                    mID = lpUnit->getID();
                    mSortType = DssSortAttributeDefault;
                    mpIterator->getFormFromHandle(*mpCellHandle, mFormID);
                    mBaseFormID = hFindMatchedBaseFormID((DSSTemplateAttribute*)(lpUnit), mFormID);
                }
                else if(lpUnit->getType() == DssTemplateConsolidation)
                {
                    mObjectType = DssTypeConsolidation;
                    mID = lpUnit->getID();
                    // jzeng; 08/07/14; new DE need set sort form id
                    DSSObjectInfo* lpObjectInfo = lpObjectContext->FindObjectH(mID);
                    DSSConsolidation* lpConsolidation = static_cast<DSSConsolidation*>(lpObjectInfo);
                    if (lpConsolidation && (lpConsolidation->getConsolidationType() == DssConsolidationTypeDerivedElement))
                    {
                        mSortType = DssSortConsolidationForm;
                        DSSAttributeFormWrappers* lpAttributeFormWrappers = lpConsolidation->getFormWrappers();
                        // sort by first form at present, maybe prompting in furture
                        if (lpAttributeFormWrappers->Count() > 0)
                        {
                            mFormID = lpAttributeFormWrappers->Item(0)->getID();
                        }
                    }
                    //sort on recursive NDE, use attribute form sorter
                    else if (lpConsolidation && lpConsolidation->getConsolidationType() == DssConsolidationTypeRecursiveDerivedElement)
                    {
                        mObjectType = DssTypeConsolidation;
                        mID = lpUnit->getID();
                        mSortType = DssSortConsolidationForm;
                        DSSAttributeFormWrappers* lpAttributeFormWrappers = lpConsolidation->getFormWrappers();
                        // sort by first form .at present, maybe prompting in furture
                        if (lpAttributeFormWrappers->Count() > 0)
                        {
                            mFormID = lpAttributeFormWrappers->Item(0)->getID();
                            mBaseFormID = lpAttributeFormWrappers->Item(0)->getBaseFormID();
                        }
                    }
                    else
                    {
                        mSortType = DssSortConsolidationDefault;
                    }
                    
                }
                else if(lpUnit->getType() == DssTemplateCustomGroup)
                {
                    mObjectType = DssTypeFilter;
                    mID = lpUnit->getID();
                    mSortType = DssSortCustomGroupDefault;
                }
                else if(lpUnit->getType() == DssTemplateDimension)
                {
                    mObjectType = DssTypeAttribute;
                    mpIterator->getFormFromHandle(*mpCellHandle, mFormID);
                    DSSTemplateDimension* lpDimension = (DSSTemplateDimension*)lpUnit;
                    for (int j=0; j<lpDimension->Count(); j++)
                    {
                        mBaseFormID = hFindMatchedBaseFormID(lpDimension->Item(j), mFormID);
                        if (mBaseFormID>=0)
                        {
                            mID = lpDimension->Item(j)->getID();
                            break;
                        }
                    }
                    mSortType = DssSortAttributeForm;
                }
                else if(lpUnit->getType() == DssTemplateMetric)
                {
                    mObjectType = DssTypeMetric;
                    mID = lpUnit->getID();
                    mSortType = DssSortMetric;
                }
                
                // set subtoalsPostion by id / form id
                if(mAxisIndex == DssAxisRows)
                    lSubtotalsPositionRow = getSubtotalPosition(lpAxis->getSorts(), mID, mFormID);
                else
                    lSubtotalsPositionColumn = getSubtotalPosition(lpAxis->getSorts(), mID, mFormID);
                break;
            }
            case DssCellHandleTemplateUnitAndForm:
            {
                // DSSAttribute form sort
                
                mObjectType = DssTypeAttribute;
                mSortType = DssSortAttributeForm;
                DSSTemplateUnit* lpUnit = mpCellHandle->p.unit;
                
                DSSAxis* lpAxis = lpUnit->getAxis();
                
                if (!lpAxis)
                    break;
                
                mAxisIndex = EnumDSSAxisName(lpAxis->getIndex());
                mpIterator->getFormFromHandle(*mpCellHandle, mFormID);
                if  (lpUnit->getType()==DssTemplateAttribute)
                {
                    mID = lpUnit->getID();
                    mBaseFormID = hFindMatchedBaseFormID((DSSTemplateAttribute*)(lpUnit), mFormID);
                }
                else if (lpUnit->getType()==DssTemplateDimension)
                {
                    DSSTemplateDimension* lpDimension = (DSSTemplateDimension*)lpUnit;
                    for (int j=0; j<lpDimension->Count(); j++)
                    {
                        mBaseFormID = hFindMatchedBaseFormID(lpDimension->Item(j), mFormID);
                        if (mBaseFormID >= 0)
                        {
                            mID = lpDimension->Item(j)->getID();
                            break;
                        }
                    }
                }
                
                break;
            }
            default:
                break;
        }
        
        if (mAxisIndex==DssAxisRows)
            mSubtotalPosition = lSubtotalsPositionRow;
        else if (mAxisIndex==DssAxisColumns)
            mSubtotalPosition = lSubtotalsPositionColumn;
    }
    else if (mIsFromActionObject)
    {
        DSSObjectInfo* lpObject = lpObjectContext->FindObjectH(mID);
        if (!lpObject)
            return S_OK;
        
        mObjectType = lpObject->getType();
        if (mObjectType==DssTypeAttribute)
        {
            if (MBase::IsEqualGUID(mFormID,GUID_NULL))
                mSortType = DssSortAttributeDefault;
            else
                mSortType = DssSortAttributeForm;
        }
        else if (mObjectType==DssTypeMetric)
            mSortType = DssSortMetric;
        else if (mObjectType==DssTypeConsolidation)
            mSortType = DssSortConsolidationDefault;
        else
            return S_OK;
    }
    
    if (mIsClearSort)
    {
        DSSTemplate* lpTemplate = mpTemplateNode->getTemplate();
        if (mAxisIndex == DssAxisReserved) {
            // set axisIndex if cellHandle don't input; try row at first
            DSSSorts* lpSortsRow = lpTemplate->getRow()->getSorts();
            bool lbSortOnRow = (lpSortsRow && lpSortsRow->Count()>0);
            DSSSorts* lpSortsColumn = lpTemplate->getColumn()->getSorts();
            bool lbSortOnColumn = (lpSortsColumn && lpSortsColumn->Count()>0);
            if (lbSortOnRow && lbSortOnColumn)
            {
                DSSSorts* lpSortsInitialRow = lpTemplate->getRow()->getInitialSorts();
                bool lbSortOnInitialRow = (lpSortsInitialRow && lpSortsInitialRow->Count()>0);
                DSSSorts* lpSortsInitialColumn = lpTemplate->getColumn()->getInitialSorts();
                bool lbSortOnInitialColumn = (lpSortsInitialColumn && lpSortsInitialColumn->Count()>0);
                if (lbSortOnInitialRow != lbSortOnRow)
                    mAxisIndex = DssAxisRows;
                else if (lbSortOnInitialColumn != lbSortOnColumn)
                    mAxisIndex = DssAxisColumns;
                else
                    mAxisIndex = DssAxisAll;
            }
            else if (lbSortOnRow)
            {
                mAxisIndex = DssAxisRows;
            }
            else if (lbSortOnColumn)
            {
                mAxisIndex = DssAxisColumns;
            }
        }
        
        // set ascend, used for generate XML
        DSSAxis* lpAxis = lpTemplate->getRow();
        if (mAxisIndex == DssAxisColumns) {
            lpAxis = lpTemplate->getColumn();
        }
        DSSSorts* lpSorts = lpAxis->getInitialSorts();
        if (lpSorts && lpSorts->Count() > 0)
            mAscending = lpSorts->Item(0)->getAscending();
    }
    
    return true;
}

int DSSCManipulationSortTemplate::Execute()
{
	/*if (hCheckLayoutLoaded(mpTemplateNode)==S_FALSE)
		return S_FALSE;*/
	
    if (!hInit())
        return S_FALSE;
    
	int hr;
	DSSObjectContext* lpObjectContext = mpTemplateNode->getObjectContext();
	DSSTemplate* lpTemplate = mpTemplateNode->getTemplate();
    DSSAxis* lpAxis;
    if (!mIsClearSort)
    {
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
        {
            lpSorts->Clear();
        }
        
        DSSSort* lpNewSort = new DSSSort(lpObjectContext);
        lpNewSort->setSortType(mSortType);
        lpNewSort->setAscending(mAscending);
        lpNewSort->setObjectID(mID);
        lpNewSort->setFormID(mFormID);
        if (mBaseFormID >= 0)
            lpNewSort->setBaseFormID(mBaseFormID);
        if (mpElementIDs)
        {
            DSSBaseElementsProxy* lpElements = new DSSBaseElementsProxy(lpObjectContext);
            StringVector::iterator it1;
            for (it1 = mpElementIDs->begin(); it1 != mpElementIDs->end(); it1++)
            {
                DSSBaseElementProxy* lpElement = DSSBaseElementsProxy::CreateElementFromID(*it1, lpObjectContext);
                
                if (lpElement)
                    lpElements->AddElement(lpElement);
            }
            lpNewSort->setElements(lpElements);
        }
        lpNewSort->setSubtotalPosition(mSubtotalPosition);
        lpSorts->Add(lpNewSort);
    }
    else
    {
        if (mAxisIndex == DssAxisColumns)
        {
            lpAxis = lpTemplate->getColumn();
        }
        else if (mAxisIndex == DssAxisRows)
        {
            lpAxis = lpTemplate->getRow();
        }
        else
        {
            // There is no initial sort or both axis sorts, then reset column sort at first
            lpTemplate->getColumn()->setSorts(DSSSorts::DuplicateSorts(lpTemplate->getColumn()->getInitialSorts()));
            
            // set axis to row to following logic
            lpAxis = lpTemplate->getRow();
        }
        
        lpAxis->setSorts(DSSSorts::DuplicateSorts(lpAxis->getInitialSorts()));
    }
    
    //zhyang, 539223
    if(lpTemplate->getTemplateModelInterface())
        lpTemplate->getTemplateModelInterface()->Refresh();
    
	if (!mIsFromActionObject)
	{
        if(mAOXML == true)
        {
            // TODO, just save no sort at present while clear sort, actually it's expected extract initial sort info to generate xml
            GenerateAODeltaXML(mIsClearSort);
        }
        else
        {
            mpActionObject = new DSSAOSort();
            DSSAOSort* mpSortAO = static_cast<DSSAOSort*>(mpActionObject);
            mpSortAO->setObjectType(mObjectType);
            mpSortAO->setObjectID(mID);
            mpSortAO->setFormID(mFormID);
            mpSortAO->setFormType(DssTypeAttributeForm);
            mpSortAO->setClearSorts(true);
            mpSortAO->setAscending(mAscending);
            mpSortAO->setNodeKey(mpTemplateNode->getKey());
            mpSortAO->setAxisIndex(mAxisIndex);
            
			mpSortAO->setSubtotalPosition(mSubtotalPosition);
			
            if (mpElementIDs) {
                for (StringVector::iterator it = mpElementIDs->begin() ; it != mpElementIDs->end(); it++)
                    mpSortAO->addElementID(*it);
                
            }
        }
	}
    
    bool lNeedServer = false;
    bool lSubsetNeed = false;
    hr = DSSCManipulationHelper::checkDynamicMetricInVT(lpTemplate, lNeedServer, lSubsetNeed);
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
    
	return S_OK;
}

int DSSCManipulationSortTemplate::hFindMatchedBaseFormID(DSSTemplateAttribute* ipAttribute, GUID iFormID)
{
	DSSAttributeFormWrappers* lpForms = ipAttribute->getFormWrappers();
	for (int i=0; i<lpForms->Count(); i++)
		if(MBase::IsEqualGUID(lpForms->Item(i)->getID(), iFormID))
			return lpForms->Item(i)->getBaseFormID();
	return -1;
}

DSSRWTemplateNode* DSSCManipulationSortTemplate::getTemplateNode()
{
	return mpTemplateNode;
}

#ifndef __ANDROID__
void DSSCManipulationSortTemplate::GenerateAODeltaXML(bool isRemoveSort)
{
    mpActionObject = new DSSAODeltaXML(ManipulationSortTemplate);
    
    SimpleXMLBuilder lBuilder;
    SimpleXMLNode lXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation"));
    
    SimpleXMLNode lManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("rw_manipulation_method"));
    SimpleXMLNode lManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("19", true));
    lManipulationMethodXML.AddChild(lManipulationMethodXMLContent);
    lXML.AddChild(lManipulationMethodXML);
    
    int lTreeType = static_cast<int>(mpTemplateNode->getTreeType());
    SimpleXMLNode lTreeTypeXML(SimpleXMLBuilder::CreateNodePtr("rw_tree_type"));
    SimpleXMLNode lTreeTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lTreeType).c_str(), true));
    lTreeTypeXML.AddChild(lTreeTypeXMLContent);
    lXML.AddChild(lTreeTypeXML);
    
    SimpleXMLNode lNodeKeyXML(SimpleXMLBuilder::CreateNodePtr("rw_node_key"));
    SimpleXMLNode lNodeKeyXMLContent(SimpleXMLBuilder::CreateNodePtr(mpTemplateNode->getKey().c_str(), true));
    lNodeKeyXML.AddChild(lNodeKeyXMLContent);
    lXML.AddChild(lNodeKeyXML);
    
    SimpleXMLNode lAxisXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation"));
    {
        SimpleXMLNode lAxisManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation_method"));
        SimpleXMLNode lAxisManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("27", true));
        lAxisManipulationMethodXML.AddChild(lAxisManipulationMethodXMLContent);
        
        int lAxisName = static_cast<int>(mAxisIndex);
        SimpleXMLNode lAxisNameXML(SimpleXMLBuilder::CreateNodePtr("axis_name"));
        SimpleXMLNode lAxisNameXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lAxisName).c_str(), true));
        lAxisNameXML.AddChild(lAxisNameXMLContent);
        
        lAxisXML.AddChild(lAxisManipulationMethodXML);
        lAxisXML.AddChild(lAxisNameXML);
    }
    lXML.AddChild(lAxisXML);
    
    if (!isRemoveSort)
    {
        SimpleXMLNode lSortXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation"));
        {
            SimpleXMLNode lSortManipulationMethodXML(SimpleXMLBuilder::CreateNodePtr("report_manipulation_method"));
            SimpleXMLNode lSortManipulationMethodXMLContent(SimpleXMLBuilder::CreateNodePtr("26", true));
            lSortManipulationMethodXML.AddChild(lSortManipulationMethodXMLContent);
            lSortXML.AddChild(lSortManipulationMethodXML);
            
            SimpleXMLNode lAxisNameXML(SimpleXMLBuilder::CreateNodePtr("axis_name"));
            int lAxisName = static_cast<int>(mAxisIndex);
            SimpleXMLNode lAxisNameXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lAxisName).c_str(), true));
            lAxisNameXML.AddChild(lAxisNameXMLContent);
            lSortXML.AddChild(lAxisNameXML);
            
            int lIsAscending = static_cast<int>(mAscending);
            SimpleXMLNode lSortAscendingXML(SimpleXMLBuilder::CreateNodePtr("is_ascending"));
            SimpleXMLNode lSortAscendingXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lIsAscending).c_str(), true));
            lSortAscendingXML.AddChild(lSortAscendingXMLContent);
            lSortXML.AddChild(lSortAscendingXML);
            
            SimpleXMLNode lSortSubtotalsPositionXML(SimpleXMLBuilder::CreateNodePtr("subtotals_position"));
            SimpleXMLNode lSortSubtotalsPositionXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(mSubtotalPosition).c_str(), true));
            lSortSubtotalsPositionXML.AddChild(lSortSubtotalsPositionXMLContent);
            lSortXML.AddChild(lSortSubtotalsPositionXML);
            
            SimpleXMLNode lUnitIdXML(SimpleXMLBuilder::CreateNodePtr("unit_id"));
            char lID[33];
            DSSBaseElementProxy::ConvertGUIDToString(mID, lID);
            SimpleXMLNode lUnitIdXMLContent(SimpleXMLBuilder::CreateNodePtr(lID, true));
            lUnitIdXML.AddChild(lUnitIdXMLContent);
            lSortXML.AddChild(lUnitIdXML);
            
            SimpleXMLNode lUnitTypeXML(SimpleXMLBuilder::CreateNodePtr("unit_type"));
            SimpleXMLNode lUnitTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(mObjectType).c_str(), true));
            lUnitTypeXML.AddChild(lUnitTypeXMLContent);
            lSortXML.AddChild(lUnitTypeXML);
            
            if (mpElementIDs && mpElementIDs->size() > 0)
            {
                SimpleXMLNode lElementIDsXML(SimpleXMLBuilder::CreateNodePtr("element_ids_collection"));
                for (StringVector::iterator it = mpElementIDs->begin(); it != mpElementIDs->end(); it++)
                {
                    SimpleXMLNode lElementIDXML(SimpleXMLBuilder::CreateNodePtr("element_id"));
                    SimpleXMLNode lElementIDContent(SimpleXMLBuilder::CreateNodePtr(it->c_str(), true));
                    lElementIDXML.AddChild(lElementIDContent);
                    lElementIDsXML.AddChild(lElementIDXML);
                }
                lSortXML.AddChild(lElementIDsXML);
            }
            
            EnumDSSSortType lSortType;
            switch (mpCellHandle->type)
            {
                case DssCellHandleXTabHeaderElement:
                    lSortType = DssSortMetric;
                    break;
                case DssCellHandleTemplateUnit:
                {
                    DSSTemplateUnit* lpTemplateUnit = mpCellHandle->p.unit;
                    if(lpTemplateUnit->getType() == DssTemplateAttribute)
                        lSortType = DssSortAttributeDefault;
                    else if(lpTemplateUnit->getType() == DssTemplateConsolidation)
                        lSortType = DssSortConsolidationDefault;
                    break;
                }
                case DssCellHandleTemplateUnitAndForm:
                    lSortType = DssSortAttributeForm;
                    break;
                default:
                    break;
            }

            int lSortTypeInt = static_cast<int>(lSortType);
            SimpleXMLNode lSortTypeXML(SimpleXMLBuilder::CreateNodePtr("sort_type"));
            SimpleXMLNode lSortTypeXMLContent(SimpleXMLBuilder::CreateNodePtr(DSSExpNode::IntToString(lSortTypeInt).c_str(), true));
            lSortTypeXML.AddChild(lSortTypeXMLContent);
            lSortXML.AddChild(lSortTypeXML);
            
            if(lSortTypeInt == 2 || (lSortTypeInt == 3 && mpCellHandle->p.unit->getType() == DssTemplateAttribute))
            {
                char lAttributeForm[33];
                DSSBaseElementProxy::ConvertGUIDToString(mFormID, lAttributeForm);
                SimpleXMLNode lAttributeFormXML(SimpleXMLBuilder::CreateNodePtr("form_id"));
                SimpleXMLNode lAttributeFormXMLContent(SimpleXMLBuilder::CreateNodePtr(lAttributeForm, true));
                lAttributeFormXML.AddChild(lAttributeFormXMLContent);
                lSortXML.AddChild(lAttributeFormXML);
            }
        }
        lXML.AddChild(lSortXML);
    }
    
    SimpleXMLNode lRoot(lBuilder.GetRoot());
    lRoot.AddChild(lXML);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lBuilder.ToString(false));
}

void DSSCManipulationSortTemplate::hParseAOXML(std::string &iXMLString, DSSObjectContext *ipContext)
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
                    if(lSortNode.GetName().compare("axis_name") == 0)
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
                    }
                    else if (lSortNode.GetName().compare("is_ascending") == 0)
                    {
                        lSortNode.GetContent(lStrContent);
                        mAscending = DSSExpNode::StringToInt(lStrContent);
                    }
                    else if (lSortNode.GetName().compare("element_ids_collection") == 0)
                    {
                        mpElementIDs = new StringVector;
                        for(SimpleXMLNode lElementNode = lSortNode.GetFirstChild(); lElementNode.IsValid(); lElementNode = lElementNode.GetNextSibling())
                        {
                            lElementNode.GetContent(lStrContent);
                            mpElementIDs->push_back(lStrContent);
                        }
                    }
                    else if (lSortNode.GetName().compare("subtotals_position") == 0)
                    {
                        lSortNode.GetContent(lStrContent);
                        mSubtotalPosition = DSSExpNode::StringToInt(lStrContent);
                    }
                    else if (lSortNode.GetName().compare("unit_type") == 0)
                    {
                        lSortNode.GetContent(lStrContent);
                        mObjectType = static_cast<EnumDSSObjectType>(DSSExpNode::StringToInt(lStrContent));
                    }
                }
            }
        }
    }
}
#else
void DSSCManipulationSortTemplate::GenerateAODeltaXML(bool isRemoveSort)
{
    TiXmlDocument lDoc;
    TiXmlElement* lpRootElement = new TiXmlElement("rw_manipulation");
    
    TiXmlElement* lpManipulationMethodElement = new TiXmlElement("rw_manipulation_method");
    TiXmlText* lpManipulationMethodText = new TiXmlText("19");
    lpManipulationMethodElement->LinkEndChild(lpManipulationMethodText);
    lpRootElement->LinkEndChild(lpManipulationMethodElement);
    
    int lTreeType = static_cast<int>(mpTemplateNode->getTreeType());
    TiXmlElement* lpTreeTypeElement =  new TiXmlElement("rw_tree_type");
    TiXmlText* lpTreeTypeText = new TiXmlText(DSSExpNode::IntToString(lTreeType).c_str());
    lpTreeTypeElement->LinkEndChild(lpTreeTypeText);
    lpRootElement->LinkEndChild(lpTreeTypeElement);
    
    TiXmlElement* lpNodeKeyElement = new TiXmlElement("rw_node_key");
    TiXmlText* lpNodeKeyText = new TiXmlText(mpTemplateNode->getKey().c_str());
    lpNodeKeyElement->LinkEndChild(lpNodeKeyText);
    lpRootElement->LinkEndChild(lpNodeKeyElement);
    
    TiXmlElement* lpAxisElement = new TiXmlElement("report_manipulation");
    {
		TiXmlElement* lpAxisManipulationMethodElement = new TiXmlElement("report_manipulation_method");
		TiXmlText* lpAxisManipulationMethodText = new TiXmlText("27");
		lpAxisManipulationMethodElement->LinkEndChild(lpAxisManipulationMethodText);
		lpAxisElement->LinkEndChild(lpAxisManipulationMethodElement);

		int lAxisName = static_cast<int>(mAxisIndex);
		TiXmlElement* lpAxisNameElement = new TiXmlElement("axis_name");
		TiXmlText* lpAxisText = new TiXmlText(DSSExpNode::IntToString(lAxisName).c_str());
		lpAxisNameElement->LinkEndChild(lpAxisText);
		lpAxisElement->LinkEndChild(lpAxisNameElement);
    }
	lpRootElement->LinkEndChild(lpAxisElement);

	if (!isRemoveSort)
	{
		TiXmlElement* lpSortElement = new TiXmlElement("report_manipulation");

		TiXmlElement* lpSortManipulationMethodElement = new TiXmlElement("report_manipulation_method");
		TiXmlText* lpSortManipulationMethodText = new TiXmlText("26");
		lpSortManipulationMethodElement->LinkEndChild(lpSortManipulationMethodText);
		lpSortElement->LinkEndChild(lpSortManipulationMethodElement);

		int lAxisName = static_cast<int>(mAxisIndex);
		TiXmlElement* lpAxisNameElement = new TiXmlElement("axis_name");
		TiXmlText* lpAxisText = new TiXmlText(DSSExpNode::IntToString(lAxisName).c_str());
		lpAxisNameElement->LinkEndChild(lpAxisText);
		lpSortElement->LinkEndChild(lpAxisNameElement);

		int lIsAscending = static_cast<int>(mAscending);
		TiXmlElement* lpSortAscendingElement = new TiXmlElement("is_ascending");
		TiXmlText* lpSortAscendingText = new TiXmlText(DSSExpNode::IntToString(lIsAscending).c_str());
		lpSortAscendingElement->LinkEndChild(lpSortAscendingText);
		lpSortElement->LinkEndChild(lpSortAscendingElement);

		TiXmlElement* lSortSubtotalsPositionElement = new TiXmlElement("subtotals_position");
		TiXmlText* lSortSubtotalsPositionText = new TiXmlText(DSSExpNode::IntToString(mSubtotalPosition).c_str());
		lSortSubtotalsPositionElement->LinkEndChild(lSortSubtotalsPositionText);
		lpSortElement->LinkEndChild(lSortSubtotalsPositionElement);

		TiXmlElement* lUnitIdXML = new TiXmlElement("unit_id");
        char lID[33];
        DSSBaseElementProxy::ConvertGUIDToString(mID, lID);
        TiXmlText* lUnitIdXMLContent = new TiXmlText(lID);
        lUnitIdXML->LinkEndChild(lUnitIdXMLContent);
        lpSortElement->LinkEndChild(lUnitIdXML);

		TiXmlElement* lUnitTypeXML = new TiXmlElement("unit_type");
		TiXmlText* lUnitTypeXMLContent = new TiXmlText(DSSExpNode::IntToString(mObjectType).c_str());
		lUnitTypeXML->LinkEndChild(lUnitTypeXMLContent);
		lpSortElement->LinkEndChild(lUnitTypeXML);

        if (mpElementIDs && mpElementIDs->size() > 0)
        {
    		TiXmlElement* lElementIDsXML = new TiXmlElement("element_ids_collection");
            for (StringVector::iterator it = mpElementIDs->begin(); it != mpElementIDs->end(); it++)
            {
        		TiXmlElement* lElementIDXML = new TiXmlElement("element_id");
        		TiXmlText* lElementIDContent = new TiXmlText(it->c_str());
        		lElementIDXML->LinkEndChild(lElementIDContent);
        		lElementIDsXML->LinkEndChild(lElementIDXML);
            }
            lpSortElement->LinkEndChild(lElementIDsXML);
        }

		TiXmlElement* lSortTypeElement = new TiXmlElement("sort_type");
		TiXmlText* lSortTypeText = new TiXmlText(DSSExpNode::IntToString(mSortType).c_str());
		lSortTypeElement->LinkEndChild(lSortTypeText);
		lpSortElement->LinkEndChild(lSortTypeElement);

		if(mSortType == DssSortAttributeForm || (mSortType == DssSortAttributeDefault && mpCellHandle->p.unit->getType() == DssTemplateAttribute))
		{
			char lAttributeForm[33];
			DSSBaseElementProxy::ConvertGUIDToString(mFormID, lAttributeForm);
			TiXmlElement* lAttributeFormElement = new TiXmlElement("form_id");
			TiXmlText* lAttributeFormText = new TiXmlText(lAttributeForm);
			lAttributeFormElement->LinkEndChild(lAttributeFormText);
			lpSortElement->LinkEndChild(lAttributeFormElement);
		}
		lpRootElement->LinkEndChild(lpSortElement);
	}
	lDoc.LinkEndChild(lpRootElement);
    
    TiXmlPrinter lPrinter;
    lPrinter.SetIndent(NULL);
    lPrinter.SetLineBreak(NULL);
    lDoc.Accept(&lPrinter);
    mpActionObject = new DSSAODeltaXML(ManipulationSortTemplate);
    (static_cast<DSSAODeltaXML*>(mpActionObject))->setXMLString(lPrinter.Str());
}

void DSSCManipulationSortTemplate::hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext)
{
	TiXmlDocument doc;
	doc.Parse(iXMLString.c_str());
	if (doc.Error())
		return;

	const TiXmlElement* lpCurrentNode = doc.RootElement();
	if (!lpCurrentNode || lpCurrentNode->ValueStr() != "rw_manipulation")
		return;

	std::string lstrContent;

	for (lpCurrentNode = lpCurrentNode->FirstChildElement(); lpCurrentNode != NULL; lpCurrentNode = lpCurrentNode->NextSiblingElement())
	{
		if (lpCurrentNode->ValueStr() == "rw_node_key")
		{
			// Found the template node key
			const char* lpstrText = lpCurrentNode->GetText();
			if (lpstrText)
			{
				lstrContent = lpstrText;
				mpTemplateNode = static_cast<DSSRWTemplateNode*>(ipContext->getNode(lstrContent));
			}
		}
		else if (lpCurrentNode->ValueStr() == "report_manipulation")
		{
			for (const TiXmlElement* lSortNode = lpCurrentNode->FirstChildElement(); lSortNode != NULL; lSortNode = lSortNode->NextSiblingElement())
			{
				if (lSortNode->ValueStr() == "axis_name")
				{
					const char* lpstrText = lSortNode->GetText();
					if (lpstrText)
					{
						lstrContent = lpstrText;
						mAxisIndex = static_cast<EnumDSSAxisName>(DSSExpNode::StringToInt(lstrContent));
					}
				}
				else if (lSortNode->ValueStr() == "unit_id")
				{
					const char* lpstrText = lSortNode->GetText();
					if (lpstrText)
					{
						lstrContent = lpstrText;
						DSSBaseElementProxy::ConvertFromStringToGUID(lstrContent, mID);
					}
				}
				else if (lSortNode->ValueStr() == "form_id")
				{
					const char* lpstrText = lSortNode->GetText();
					if (lpstrText)
					{
						lstrContent = lpstrText;
						DSSBaseElementProxy::ConvertFromStringToGUID(lstrContent, mFormID);
					}
				}
				else if (lSortNode->ValueStr() == "is_ascending")
				{
					const char* lpstrText = lSortNode->GetText();
					if (lpstrText)
					{
						lstrContent = lpstrText;
						mAscending = DSSExpNode::StringToInt(lstrContent);
					}
				}
				else if (lSortNode->ValueStr() == "element_ids_collection")
				{
					mpElementIDs = new StringVector;
					for (const TiXmlElement* lElementNode = lSortNode->FirstChildElement(); lElementNode != NULL; lElementNode = lElementNode->NextSiblingElement())
					{
						const char* lpstrText = lElementNode->GetText();
						if (lpstrText)
						{
							lstrContent = lpstrText;
							mpElementIDs->push_back(lstrContent);
						}
					}
				}
                else if (lSortNode->ValueStr() == "subtotals_position")
                {
					const char* lpstrText = lSortNode->GetText();
					if (lpstrText)
					{
						lstrContent = lpstrText;
	                    mSubtotalPosition = DSSExpNode::StringToInt(lstrContent);
					}
                }
                else if (lSortNode->ValueStr() == "unit_type")
                {
					const char* lpstrText = lSortNode->GetText();
					if (lpstrText)
					{
						lstrContent = lpstrText;
	                    mObjectType = static_cast<EnumDSSObjectType>(DSSExpNode::StringToInt(lstrContent));
					}
                }
			}
		}
	}
}

#endif

int DSSCManipulationSortTemplate::getSubtotalPosition(DSSSorts* ipSorts, GUID iObjectID, GUID iFormID)
{
    if(!ipSorts || ipSorts->Count() <= 0)
        return -1;
    
    for (int i = 0; i < ipSorts->Count(); i++)
    {
        DSSSort* lpSort = ipSorts->Item(i);
        if(iObjectID == lpSort->getObjectID() && (iFormID == GUID_NULL || iFormID == lpSort->getFormID()))
            return lpSort->getSubtotalPosition();
    }
    return -1;
}

bool DSSCManipulationSortTemplate::getIsAscending()
{
    return mAscending;
}
