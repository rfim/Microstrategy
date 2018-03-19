//
//  DSSRWDDICInputControlIterator.cpp
//  ReportServiceCore
//
//  Created by Taimin Liao on 11/23/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#import "DSSRWDDICInputControlIterator.h"
#import "DSSObjectContext.h"
#import "DSSRWControl.h"
#import "DSSTabularAttribute.h"
#import "DSSTabularCustomGroup.h"
#import "DSSRWNodeModel.h"
#import "DSSRWGridModel.h"
#import "DSSRWFieldGroupModel.h"
#import "DSSGlobalLookUpTable.h"
#import "DSSDataColumn.h"
#import "DSSRWDataModel.h"
#import "DSSRWCompare.h"
#include "DSSSort_includes.h"

#import "DSSCExpression.h"
#import "DSSExpNode.h"
#import "DSSExpNodeElementsObject.h"
#import "DSSExpNodeOperator.h"

#include "DSSGUnitContainer.h"

DSSRWDDICInputControlIterator::DSSRWDDICInputControlIterator() 
{
    mGlobalLookupVer = -1;
}

DSSRWDDICInputControlIterator::~DSSRWDDICInputControlIterator()
{
    std::map<GUID, DSSBaseElementsProxy *>::iterator ite = mReplaceElementsMap.begin();
    while(ite != mReplaceElementsMap.end())
    {
        delete ite->second;
        ite->second = NULL;
        ite++;
    }
    mReplaceElementsMap.clear();
}

int DSSRWDDICInputControlIterator::Init(std::string iKey, DSSObjectContext* ipObjectContext, DSSRWNodeModel* ipNodeModel, DSSRWDataModel* ipDataModel)
{
    CHECK_PTR(ipObjectContext);
    
    DSSRWControl *lpControl = ipObjectContext->getControl(iKey);
    CHECK_PTR(lpControl);
	mType = DssIteratorControl;
    
    CHECK_PTR(ipNodeModel)
    mpNodeModel = ipNodeModel;
    mpDataModel = ipDataModel;
    mContainerKey = mpNodeModel->getNodeKey();
    mGlobalLookupVer = ipDataModel->getGlobalLookupVer();
	mControlKey = iKey;
	mpObjectContext = ipObjectContext;
    GUID lControlObjectID = lpControl->getSourceID();
    DSSObjectInfo *lpObjectInfo=mpObjectContext->FindObjectH(lControlObjectID);
    //tliao, 697153
    if (lpObjectInfo)
        mGUID=lpObjectInfo->getID();
    
    
    mpDisplayTextMap = mpDataModel->getDisplayTextMap(iKey);
    mpElementProxyMap = mpDataModel->getElementProxyMap(iKey);
    
    return hDoInit(lpControl);
}

int DSSRWDDICInputControlIterator::hGetDisplayText(DSSRWControl *ipControl, vector<int>* iGlobalKeys)
{
    if(!ipControl)
        return E_POINTER;
    
    DSSGUnitContainer* lpGuitContainer = NULL;
    mpDataModel->GetGlobalLookupContainer(&lpGuitContainer);
    
    GUID lControlObjectID = ipControl->getSourceID();
    
    DSSObjectInfo *lpObjectInfo=mpObjectContext->FindObjectH(lControlObjectID);
    
    mvDisplayText.clear();
    mvDisplayText.resize(mnElements);
    
    vector<int> lEmptyVector;
    string lAttributeFormSeperator=":";
    
    /*DSSAEHelper::GetDisplayTextsForTabularUnit(lpGuitContainer,lpObjectInfo,mGUID,mvElements,mvDisplayText,lEmptyVector,lAttributeFormSeperator,NULL,NULL,mpDataModel);*/
    DSSTabularUnit *lpUnit;
    int hr = getTabularUnit(mGUID, &lpUnit);
    CHECK_HR;
    
    DSSRWControl* lpControl = mpObjectContext->getControl(mControlKey);
    if(lpControl)
    {
        DSSAttributeFormWrappers* lpDisForms = lpControl->getCustomAttributeForms();
        if(lpDisForms)
        {
            for(int i = 0; i < lpDisForms->Count(); i++)
            {
                DSSAttributeFormWrapper* lpForm = lpDisForms->Item(0);
                if(lpForm)
                {
                    lEmptyVector.push_back(lpForm->getBaseFormID());
                }
            }
        }
    }
    if (!iGlobalKeys) {
        iGlobalKeys = &mvElements;
    }
    hr = DSSAEHelper::GetDisplayTextsForTabularUnitForDDIC(static_cast<DSSTabularAttribute *>(lpUnit),lpObjectInfo,mGUID,*iGlobalKeys,mvDisplayText,lEmptyVector,lAttributeFormSeperator,NULL,mpDataModel, lpGuitContainer);
    CHECK_HR;
    return S_OK;
}


int DSSRWDDICInputControlIterator::hGenerateElements(DSSRWControl *ipControl)
{
    if(!ipControl)
        return E_POINTER;
    
    int hr=S_OK;
    
    GUID lControlObjectID = ipControl->getSourceID();
    
    DSSObjectInfo *lpObjectInfo=mpObjectContext->FindObjectH(lControlObjectID);
    
    EnumDSSObjectType lType=lpObjectInfo->getType();
    
    switch (lType) {
        case DssTypeAttribute:
        {
            
            DSSTabularUnit *lpUnit = NULL;
            hr = getTabularUnit(mGUID, &lpUnit);
            CHECK_HR;
            DSSTabularAttribute *lpTabularAttribute=static_cast<DSSTabularAttribute *>(lpUnit);
            
            //lpTabularAttribute=mpDataModel->GetTabularAttributeByGUID(mGUID);
            
            //DSSTabularUnit* lpUnit = NULL;
            
            //DSSGlobalLookUpTable* lpLookUp = dynamic_cast<DSSGlobalLookUpTable*>(lpTabularAttribute);
            
            for (int i = 0; i < mnElements; i++)
            {
                int ei = mvElements[i];
                DSSBaseElementProxy *lpElement = NULL;            
//                int lLocalKey = -1;
                
                if(NULL==lpTabularAttribute)
                {
                    lpElement = new DSSNullElementProxy(mpObjectContext,mGUID);
                    setElementAt(i, lpElement);
                    continue;
                }
                if(ei<0)
                {
                    hr = lpTabularAttribute->createElement(ei, mpObjectContext, &lpElement);
                    CHECK_PTR(lpElement);
                }
                else
                {
                    //lpUnit = NULL;
                    //bool lfDE=false;
                    //DE
                    //lpLookUp->GetGlobalUnit(ei, &lpUnit, &lLocalKey, &lfDE);
                    
                   
                    int lMaxKey = lpTabularAttribute->CountRows();
                    //hr = lpLookUp->GetMaxGlobalKey(&lMaxKey);
                    CHECK_HR;
                    
                    if (ei==lMaxKey+1)
                    {
                        lpElement = new DSSNullElementProxy(mpObjectContext,mGUID);
                    }
                    else                    
                    {
                        
                        hr = lpTabularAttribute->createElement(ei, mpObjectContext, &lpElement, ei,mGlobalLookupVer);
                        //hr = lpTabularAttribute->createElement(ei, mpObjectContext, &lpElement, 2);
                        CHECK_HR;
                        
                        //xiazhou, issue 865553,correct global key
                        int lGlobalKey = -1;
                        GUID lUnitID = lpTabularAttribute->getGUID();
                        int lUnitGlobal = -1;
                        DSSTabularAttribute *lpGlobalLookUp = NULL;
                        DSSGUnitContainer *lpGlobalLookupContainer = NULL;
                        mpDataModel->GetGlobalLookupContainer(&lpGlobalLookupContainer);
                        if(lpGlobalLookupContainer)
                        {
                            hr = lpGlobalLookupContainer->getGlobalLookupByID(&lUnitID, &lUnitGlobal, &lpGlobalLookUp);
                            CHECK_HR_PTR(lpGlobalLookUp);
                            
                            hr = lpGlobalLookUp->FindElement(lpElement, lGlobalKey);
                            if(hr == S_OK)
                                lpElement->setGlobalKey(lGlobalKey);
                            else
                                hr = S_OK;
                        }
                    }
                }
                setElementAt(i, lpElement);
            }//end of for (i = 0; i < mnElements; i++)
        }
            break;
            
        case DssTypeConsolidation:
        {
            for (int i = 0; i < mnElements; i++)
            {
                int ei = mvElements[i];

                //special handle for total
                if(ei==DSSXTAB_TOTAL_KEY)
                {
                    //subtotal index for total will be 1,hyan, 11/12/2012
                    DSSBaseElementProxy* lpSubTotalElement=new DSSSubtotalElementProxy(mpObjectContext,mGUID,1);
                    setElementAt(i, lpSubTotalElement);
                }
                else
                {
                    //junwang, 2012/02/21, null element
                    bool lfNULL = false;
                    hr = mpDataModel->IsCompoundNull(mGUID, ei, &lfNULL);
                    CHECK_HR;
                    if(lfNULL)
                    {
                        DSSBaseElementProxy* lpElement = new DSSNullElementProxy(mpObjectContext,mGUID);
                        setElementAt(i, lpElement);
                    }
                    else
                    {
                        DSSTabularUnit *lpTabularUnit=NULL;
                        int lLocalKey=-1;
                        hr=mpDataModel->GetTabularUnit(mGUID, ei, &lpTabularUnit,&lLocalKey);
                        CHECK_HR;
                        
                        if(lpTabularUnit)
                        {
                            DSSTabularConsolidation *lpTabularConsolidation=static_cast<DSSTabularConsolidation*>(lpTabularUnit);
                            
                            if(lpTabularConsolidation)
                            {
                                DSSBaseElementProxy *lpElement = NULL;
                                hr = lpTabularConsolidation->createElement(lLocalKey, mpObjectContext, &lpElement);
                                CHECK_PTR(lpElement);
                                setElementAt(i, lpElement);
                            }
                        }
                    }
                }
                
            }//end of for (i = 0; i < mnElements; i++)
        }
            break;
            
        case DssTypeFilter:
        {
            //only create the ones currently used
            for (int i = 0; i < mnElements; i++)
            {
                int ei = mvElements[i];
                //special handle for total
                if(ei==DSSXTAB_TOTAL_KEY)
                {
                     //subtotal index for total will be 1,hyan, 11/12/2012
                    DSSBaseElementProxy* lpSubTotalElement=new DSSSubtotalElementProxy(mpObjectContext,mGUID,1);
                    setElementAt(i, lpSubTotalElement);
                }
                else
                {
                    bool lfNULL = false;
                    hr = mpDataModel->IsCompoundNull(mGUID, ei, &lfNULL);
                    CHECK_HR;
                    if(lfNULL)
                    {
                        DSSBaseElementProxy* lpElement = new DSSNullElementProxy(mpObjectContext,mGUID);
                        setElementAt(i, lpElement);
                        continue;
                    }
                    else
                    {
                        DSSTabularUnit *lpTabularUnit=NULL;
                        int lLocalKey=-1;
                        hr=mpDataModel->GetTabularUnit(mGUID, ei, &lpTabularUnit,&lLocalKey);
                        CHECK_HR;
                        
                        if(lpTabularUnit)
                        {
                            DSSTabularCustomGroup *lpTabularCustomGroup=dynamic_cast<DSSTabularCustomGroup*>(lpTabularUnit);
                            if(lpTabularCustomGroup)
                            {
                                DSSBaseElementProxy *lpElement = NULL;
                                hr = lpTabularCustomGroup->createElement(lLocalKey, mpObjectContext, &lpElement);
                                CHECK_PTR(lpElement);
                                
                                setElementAt(i, lpElement);
                            }
                        }
                    }
                }
            }
            
        }
            break;
            
        default:
            break;
    }
    return S_OK;
}

//xiazhou, 05/06/2014. Create the ith element on display. Called in incremental fetch.
//@param index
//       Offset of the available elements
//@param oppElementProxy
//       Output, generated element.
int DSSRWDDICInputControlIterator::hGenerateSingleElement(int index, DSSBaseElementProxy **oppElementProxy)
{
    int hr=S_OK;
    DSSRWControl *lpControl = mpObjectContext->getControl(mControlKey);
    CHECK_PTR(lpControl);
    GUID lControlObjectID = lpControl->getSourceID();
    DSSObjectInfo *lpObjectInfo=mpObjectContext->FindObjectH(lControlObjectID);
    CHECK_PTR(lpObjectInfo);
    EnumDSSObjectType lType=lpObjectInfo->getType();
    
    int ei = -1;
    DSSBaseElementProxy *lpElement = NULL;
    
    if(index >= 0 && index < mvElements.size())
        ei = mvElements[index];
    
    switch (lType) {
        case DssTypeAttribute:
        {
            
            DSSTabularUnit *lpUnit = NULL;
            hr = getTabularUnit(mGUID, &lpUnit);
            CHECK_HR_PTR(lpUnit);
            DSSTabularAttribute *lpTabularAttribute=static_cast<DSSTabularAttribute *>(lpUnit);
            
            if(NULL==lpTabularAttribute)
            {
                lpElement = new DSSNullElementProxy(mpObjectContext,mGUID);
                break;
            }
            
            if(ei<0)
            {
                hr = lpTabularAttribute->createElement(ei, mpObjectContext, &lpElement);
                CHECK_PTR(lpElement);
            }
            else
            {
                int lMaxKey = lpTabularAttribute->CountRows();
                CHECK_HR;
                
                if (ei==lMaxKey+1)
                {
                    lpElement = new DSSNullElementProxy(mpObjectContext,mGUID);
                }
                else
                {
                    
                    hr = lpTabularAttribute->createElement(ei, mpObjectContext, &lpElement, ei,mGlobalLookupVer);
                    CHECK_HR;
                    int lGlobalKey = -1;
                    GUID lUnitID = lpTabularAttribute->getGUID();
                    int lUnitGlobal = -1;
                    
                    DSSTabularAttribute *lpGlobalLookUp = NULL;
                    DSSGUnitContainer *lpGlobalLookupContainer = NULL;
                    mpDataModel->GetGlobalLookupContainer(&lpGlobalLookupContainer);
                    if(lpGlobalLookupContainer)
                    {
                        hr = lpGlobalLookupContainer->getGlobalLookupByID(&lUnitID, &lUnitGlobal, &lpGlobalLookUp);
                        CHECK_HR_PTR(lpGlobalLookUp);
                        
                        hr = lpGlobalLookUp->FindElement(lpElement, lGlobalKey);
                        if(hr == S_OK)
                            lpElement->setGlobalKey(lGlobalKey);
                        else
                            hr = S_OK;
                    }
                    

                    

                }
            }
        }
            break;
//        DDIC doesn't support consolidation yet
//        case DssTypeConsolidation:
//            break;
//            
//        case DssTypeFilter:
//            break;
        default:
            break;
    }
    
    if(lpElement==NULL)
        lpElement = new DSSNullElementProxy(mpObjectContext, lControlObjectID);
    
    *oppElementProxy=lpElement;
    
    return S_OK;

}

int DSSRWDDICInputControlIterator::ReInit(bool ibRefresh, bool ibDDICOnly)
{
    // May/16/2013, TQMS 748597, do not clear mvElements when false == ibRefresh, since mvElements[i] is used by viewer layer now.
    if (false == ibRefresh)
        return S_OK;
    
    DSSRWControl *lpControl = mpObjectContext->getControl(mControlKey);
    CHECK_PTR(lpControl);
    
    mnElements=0;
    mvElements.clear();
    mvSelectedCollection.clear();
    
    std::map<GUID, DSSBaseElementsProxy *>::iterator ite = mReplaceElementsMap.begin();
    while(ite != mReplaceElementsMap.end())
    {
        delete ite->second;
        ite->second = NULL;
        ite++;
    }
    mReplaceElementsMap.clear();
    
    mvMatchText.clear();
    
    //clear cache when it's invalid
    if(mpDataModel->getGlobalLookupVer() != mGlobalLookupVer)
    {
        if(mpDisplayTextMap)
        {
            mpDisplayTextMap->clear();
        }
        if(mpElementProxyMap)
        {
            map<int,DSSBaseElementProxy*>::iterator lIt=mpElementProxyMap->begin();
            
            while(lIt!=mpElementProxyMap->end())
            {
                delete lIt->second;
                
                lIt->second=NULL;
                
                lIt++;
            }
            
            mpElementProxyMap->clear();
        }
        mGlobalLookupVer = mpDataModel->getGlobalLookupVer();
    }
    
    mvWritebackText.clear();
    mvDisplayText.clear();
    
    return hDoInit(lpControl);
}

int DSSRWDDICInputControlIterator::hDoInit(DSSRWControl *ipControl)
{
    int hr = S_OK;
    EnsureContainerModelData();
    EnumDSSRWNodeType lNodeType = mpNodeModel->getNodeType();
    DSSRWDDICModel *lpDDICModel = NULL;
    if(lNodeType == DssRWNodeTemplate)
    {
        DSSRWGridModel *lpGridModel = static_cast<DSSRWGridModel*>(mpNodeModel);
        CHECK_PTR(lpGridModel);
        
        lpDDICModel = static_cast<DSSRWDDICModel *>(lpGridModel);
              
    }
    else if (lNodeType == DssRWNodeFieldGroup)
    {
        DSSRWFieldGroupModel *lpFGModel = static_cast<DSSRWFieldGroupModel*>(mpNodeModel);
        CHECK_PTR(lpFGModel);
        
        lpDDICModel = static_cast<DSSRWDDICModel *>(lpFGModel);
    }
    else
    {
        _ASSERT(false);
    }
    
    CHECK_PTR(lpDDICModel);
    
    EnumDSSRWControlFilterType lFilterType = ipControl->getFilterType();
    
    
    if(lFilterType != EnumDSSRWControlFilterTypeUnitCondition)
    {
        //xiazhou, do slicing
        std::vector<DSSGroupByUnit*> lvGBPath;
        ipControl->getGroupByPath(lvGBPath);
        int lGBUnitsNum = 0;
        vector<GUID> lvGBUnits;
        vector< vector<int> > lvvSelectedElements;
        if(!lvGBPath.empty())
        {
            lGBUnitsNum = lvGBPath.size();
            for(int i = 0; i < lGBUnitsNum; i++)
            {
                DSSGroupByUnit * gbUnit = lvGBPath[i];
                GUID lUnitID = gbUnit->getObjectID();
               
                DSSTabularUnit *lpUnit;
                hr = lpDDICModel->getAttrForDDIC(mControlKey, lUnitID, &lpUnit);
                if(hr != S_OK)//can't find GB unit in local view dataset, continue;
                    continue;
                
                vector<int> lvSelectedElements;
                
                DSSBaseElementsProxy *elements = NULL;
                int lElementNum = 0;
                
                EnumDSSCurrentElementStatus lStatus = gbUnit->getCurrentElementsStatus();


                //xiazhou, 03/05/2014, support Not in.
                bool lbExcludeAll = false;
                bool lbExclude = false;
                DSSCExpression *lpExpression = gbUnit->getExpression();
                if(lpExpression && lpExpression->Root())
                    lbExclude = true;
                //(1) get candidates
                vector<int> lvCandidates;
                set<int> lsCandidates;
                //candidates will be needed in 2 occasions: when auto-pick is necessary, or in exclude mode
                if(lStatus != DssCurrentElementChosen || lbExclude)
                {
                    hr = DSSRWNodeModel::getCandidateElementsDDIC(lvGBUnits, lvvSelectedElements, lUnitID, ipControl, mpNodeModel, false/*iGlobalFlag*/, lvCandidates);
                    CHECK_HR;
                    
                    lsCandidates.insert(lvCandidates.begin(), lvCandidates.end());
                }
                
                //(2) get current selections
                //BEHAVIOR: "Total" is handled differently for include and exclude. This behavior only applys to DDIC, not normal target.
                //--Include mode: total is treated as ALL
                //--Exclude mode: total is ignored.
                if(lStatus != DssCurrentElementUnset)
                {
                    if(lbExclude)//Not in case.
                    {
                        elements = static_cast<DSSExpNodeElementsObject*>(lpExpression->Root()->getChildNode()->getLastSibling())->getElements();
                        CHECK_PTR(elements);
                        lElementNum = elements->Count();
                        set<int> lExcludeSet;
                        //collect all the excluded elements.
                        for(int j = 0; j < lElementNum; j++)
                        {
                            DSSBaseElementProxy *element = elements->Item(j);
                            EnumDSSElementType lType = element->getElementType();
                            if(DssElementAll == lType)
                            {
                                lbExcludeAll = true;
                                break;
                            }
                            
                            if(DssElementSubtotal == lType)//exclude subtotal, ignore it.
                                continue;
                            int lKey = -1;
                            hr = hFindElementInLocal(element, lpUnit, lKey);
                            if(FAILED(hr))
                                return hr;
                            if(hr != S_OK)
                                continue;
                            
                            lExcludeSet.insert(lKey);
                        }
                        //reverse selections
                        if(!lbExcludeAll)
                        {
                            for(int j = 0; j < lvCandidates.size(); j++)
                            {
                                if(lExcludeSet.find(lvCandidates[j]) == lExcludeSet.end())
                                {
                                    lvSelectedElements.push_back(lvCandidates[j]);
                                }
                            }
                        }
                        
                    }
                    else
                    {
                        elements = gbUnit->getCurrentElements();
                        lElementNum = elements->Count();
                        for(int j = 0; j < lElementNum; j++)
                        {
                            DSSBaseElementProxy *element = elements->Item(j);
                            EnumDSSElementType lType = element->getElementType();
                            if(DssElementAll == lType || DssElementSubtotal == lType)//for issue 862147
                            {
                                int lnRow = lpUnit->CountRows();
                                lvSelectedElements.clear();
                                for( int i = 0; i < lnRow; i++)
                                    lvSelectedElements.push_back(i);
                                break;
                            }
                            
                            int lKey = -1;
                            hr = hFindElementInLocal(element, lpUnit, lKey);
                            if(FAILED(hr))
                                return hr;
                            //927956, If can't find selected element in local lookup, just ignore this selection.
                            if(hr != S_OK)
                                continue;
                            //invalid user selection
                            if(lStatus == DssCurrentElementDesired && lsCandidates.find(lKey) == lsCandidates.end())
                                continue;
                            //lKey is found from DDIC local lookup, so they should be local keys
                            //it's ok to use local keys here, because finally the display text is getting from local lookup.
                            lvSelectedElements.push_back(lKey);
                        }
                    }
                }
                
                //(3) do auto-pick if it's needed
                if(lStatus != DssCurrentElementChosen && lvSelectedElements.empty())
                {
                    //first of all, sort candidates.
                    if(!lvCandidates.empty())
                    {
                        DSSRWDDICCompare lCMP;
                        hr = lCMP.Init(this, mpDataModel->GetDisplayNullOnTop(), gbUnit->getObject());
                        CHECK_HR;
                        _xtab_sort(&lvCandidates[0], &lvCandidates[0]+lvCandidates.size(), lCMP);
                    }
                    
                    //then pick according status.
                    EnumDSSXMLRWSelectorUnsetStatus lUnsetStatus = gbUnit->getUnsetStatus();
                    int lUnsetNumber = gbUnit->getUnsetElementNumber();
                    
                    if(lUnsetStatus == DssXMLRWSelectorUnsetStatusDefault)
                    {
                        lUnsetStatus = DssXMLRWSelectorUnsetStatusFirst;
                        lUnsetNumber = 1;
                    }
                    
                    int lSeletedRow = lUnsetStatus == DssXMLRWSelectorUnsetStatusFirst? 0 : lvCandidates.size() - 1;
                    int lnSelected = 0;
                    MBase::StrongPtr<DSSBaseElementsProxy>  lpReplacedElements(new DSSBaseElementsProxy(mpObjectContext));
                    while(lnSelected < lUnsetNumber && lSeletedRow >= 0 && lSeletedRow < lvCandidates.size())
                    {
                        if(lvCandidates[lSeletedRow] != DSSXTAB_TOTAL_KEY)
                        {
                            lvSelectedElements.push_back(lvCandidates[lSeletedRow]);
                            lnSelected++;
//                            DSSBaseElementProxy* lpElement = NULL;
                            MBase::StrongPtr<DSSBaseElementProxy> lpElement;
                            hr = lpUnit->createElement(lvCandidates[lSeletedRow], mpDataModel->getObjectContext(), &lpElement);
                            CHECK_HR;
                            
                            //Correct GlobalKey
                            int lGlobalKey = -1;
                            int lUnitGlobal = -1;
                            DSSTabularAttribute *lpGlobalLookUp = NULL;

                            DSSGUnitContainer *lpGlobalLookupContainer = NULL;
                            mpDataModel->GetGlobalLookupContainer(&lpGlobalLookupContainer);
                            if(lpGlobalLookupContainer)
                            {
                                hr = lpGlobalLookupContainer->getGlobalLookupByID(&lUnitID, &lUnitGlobal, &lpGlobalLookUp);
                                CHECK_HR_PTR(lpGlobalLookUp);
                                
                                hr = lpGlobalLookUp->FindElement(lpElement.Get(), lGlobalKey);
                                if(hr == S_OK)
                                    lpElement->setGlobalKey(lGlobalKey);
                                else
                                    hr = S_OK;
                            }
//                            lvReplacedElements->push_back(lpElement);
                            lpReplacedElements->AddElement(lpElement.GiveUp());
                        }
                        if(lUnsetStatus == DssXMLRWSelectorUnsetStatusFirst)
                            lSeletedRow++;
                        else
                            lSeletedRow--;
                        
                    }
                    mReplaceElementsMap[lUnitID] = lpReplacedElements.GiveUp();
                }//end for (3)
                
                lvGBUnits.push_back(lUnitID);
                lvvSelectedElements.push_back(lvSelectedElements);
            }
        }
        hr = DSSRWNodeModel::getCandidateElementsDDIC(lvGBUnits, lvvSelectedElements, ipControl->getSourceID(), ipControl, mpNodeModel, false, mvElements);
    }
    else//UC, Just get elements from index.
    {
        DSSTabularIndex *lpIndex;
        int lLocalUnitID = -1;
        
        hr = lpDDICModel->getDDICIndex(mControlKey, &lpIndex);
        CHECK_HR;
        if(lpIndex)
        {
            lpDDICModel->getAttrIDForDDIC(mControlKey, ipControl->getSourceID(), lLocalUnitID);
        }
        
        if(lpIndex && lLocalUnitID > -1)
        {
            int lnRow = 0;
            int *lpElements = NULL;
            hr = lpIndex->GetElementsForUnit(lLocalUnitID, &lnRow, &lpElements);
            CHECK_HR
            if(lnRow > 0 && lpElements)
                mvElements.insert(mvElements.end(), lpElements, lpElements + lnRow);
        }

    }
    

    mnElements = mvElements.size();
    //now sort
    if(mnElements > 1)
    {
        DSSRWDDICCompare lCMP;
        hr = lCMP.Init(this, mpDataModel->GetDisplayNullOnTop(), NULL, false);
        CHECK_HR;
        
        _xtab_sort(&mvElements[0], &mvElements[0]+mnElements, lCMP);
    }
    
    
    mCursor = 0;
    
//    xiazhou, 05/06/2014. mpElements will not be used in incremental fetch
/*    for(int i=0;i<mpElements.size();i++)
    {
        delete mpElements[i];
        
        mpElements[i]=NULL;
    }
    mpElements.clear();
    //tliao, 697153
    if (mnElements == 0) return S_OK;

    hr=hGetDisplayText(ipControl);
    CHECK_HR;

	mpElements.resize(mnElements);
	for (int i = 0; i < mnElements; i++)
	{
		mpElements[i] = NULL;
	}


	return hGenerateElements(ipControl);*/
    return S_OK;
}

int DSSRWDDICInputControlIterator::getElementFormValue(DSSBaseElementProxy* ipElement, int iBaseForm, std::string& orValue)
{
    int hr = S_OK;
    
    const char *lpData;
    int lSize;
    EnumDSSDataType lType;
    DSSDataFlag lFlag;
    hr = getElementFormRawValue(ipElement, iBaseForm, &lpData, &lSize, &lType, &lFlag);
    CHECK_HR;
    
    hr = DSSAEHelper::ConvertToString(lpData, lSize, lType, lFlag, orValue);
    CHECK_HR;
    
    return S_OK;
}

int DSSRWDDICInputControlIterator::getAllRawValuesForColumn(GUID iObjID, int iBaseForm, vector<string>* opValues)
{
    if (opValues) {
        DSSTabularUnit *lpTabularUnit=NULL;
        int hr = getTabularUnit(iObjID, &lpTabularUnit);
        CHECK_HR_PTR(lpTabularUnit);
        EnumDSSTemplateUnitType lType = lpTabularUnit->getUnitType();
        if (lType == DssTemplateAttribute) {
            DSSTabularAttribute* lpTabularAttribute = static_cast<DSSTabularAttribute*>(lpTabularUnit);
            int lColumn = -1;
            hr = lpTabularAttribute->FindFormWithLocale(0, iBaseForm, 0, lColumn);
            CHECK_HR;
            
            DSSDataColumn* lpColumn = NULL;
            hr = lpTabularAttribute->Item(lColumn, &lpColumn);
            CHECK_HR;

            for (long long i=0; i<lpColumn->getRowCount(); i++) {
                
                const char* lpData = NULL;
                int lSize = 0;
                EnumDSSDataType lType = DssDataTypeReserved;
                DSSDataFlag lFlag = DssDataOk;
                
                hr = lpColumn->getData(i, &lpData, lSize, lType, lFlag);
                CHECK_HR;
                string lValue;
                hr = DSSAEHelper::ConvertToString(lpData, lSize, lType, lFlag, lValue);
                CHECK_HR;
                opValues->push_back(lValue);
            }
        }
    }
    return S_OK;
}

int DSSRWDDICInputControlIterator::getCachedWriteBackValues(vector<string>** opWritebackValues)
{
    GUID lObjID = getControl()->getSourceID();
    int lWriteBackBaseFormId = getControl()->getWriteBackForms()->Item(0)->getBaseFormID();
    if (mvWritebackText.size()==0)
    {
        int hr = getAllRawValuesForColumn(lObjID, lWriteBackBaseFormId, &mvWritebackText);
        CHECK_HR;
    }
    if (opWritebackValues && mvWritebackText.size()>0) {
        *opWritebackValues = &mvWritebackText;
    }
    return S_OK;
}

int DSSRWDDICInputControlIterator::getElementFormRawValue(DSSBaseElementProxy* ipElement, int iBaseForm, const char **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag)
{
    int hr = S_OK;
    GUID lUnitGUID = ipElement->getObjectID();
    
    DSSTabularUnit *lpTabularUnit=NULL;
    //int lLocalKey=-1;
    //int lGlobalKey =  ipElement->getGlobalKey();
	/*
#ifdef __ANDROID__
    //Android, ipElement doesn't have a valid globalKey
    if(lGlobalKey == -1)
    {
        for(int ei = 0; ei < mpElements.size(); ei++)
            if(mpElements[ei]->IsEqual(ipElement))
            {
                lGlobalKey = mpElements[ei]->getGlobalKey();
                break;
            }
    }
#endif
	*/
    //zhyang, 740455
    hr = getTabularUnit(lUnitGUID, &lpTabularUnit);
    //hr=mpDataModel->GetTabularUnit(lUnitGUID, lGlobalKey, &lpTabularUnit,&lLocalKey);
    CHECK_HR_PTR(lpTabularUnit);
    
    int lKey = -1;
    hr = lpTabularUnit->FindElement(ipElement, lKey);
    CHECK_HR;
    
    EnumDSSTemplateUnitType lType = lpTabularUnit->getUnitType();
    if(lType == DssTemplateAttribute)
    {
        DSSTabularAttribute* lpTabularAttribute = static_cast<DSSTabularAttribute*>(lpTabularUnit);
        
        int lColumn = -1;
        hr = lpTabularAttribute->FindFormWithLocale(0, iBaseForm, 0, lColumn);
        CHECK_HR;
        
        DSSDataColumn* lpColumn = NULL;
        hr = lpTabularAttribute->Item(lColumn, &lpColumn);
        CHECK_HR;
        
        const char* lpData = NULL;
        int lSize = 0;
        EnumDSSDataType lType = DssDataTypeReserved;
        DSSDataFlag lFlag = DssDataOk;
        
        hr = lpColumn->getData(lKey, &lpData, lSize, lType, lFlag);
        CHECK_HR;
        
        *oppData = lpData;
        *opDataSize = lSize;
        *opDataType = lType;
        *opDataFlag = lFlag;
    }
    else
        return E_UNEXPECTED;
    
    return S_OK;
}

int DSSRWDDICInputControlIterator::getElementText(int iIndex,std::string &orText)
{        
    if(iIndex>=0&&iIndex<mvElements.size())
    {
        if(mpDisplayTextMap)
        {
            int ei=mvElements[iIndex];
            
            map<int,string>::iterator it=mpDisplayTextMap->find(ei);
            
            if(it!=mpDisplayTextMap->end())
            {
                orText=it->second;
                
                return S_OK;
            }
            
            
            DSSGUnitContainer* lpGuitContainer = NULL;
            mpDataModel->GetGlobalLookupContainer(&lpGuitContainer);
            
            DSSRWControl *lpControl = this->mpObjectContext->getControl(mControlKey);
            GUID lControlObjectID = lpControl->getSourceID();
            
            DSSObjectInfo *lpObjectInfo=mpObjectContext->FindObjectH(lControlObjectID);
            
            vector<int> lEmptyVector;
            string lAttributeFormSeperator=":";
            
            DSSTabularUnit *lpUnit;
            int hr = getTabularUnit(mGUID, &lpUnit);
            CHECK_HR;
            
            if(lpControl)
            {
                DSSAttributeFormWrappers* lpDisForms = lpControl->getCustomAttributeForms();
                if(lpDisForms)
                {
                    for(int i = 0; i < lpDisForms->Count(); i++)
                    {
                        DSSAttributeFormWrapper* lpForm = lpDisForms->Item(0);
                        if(lpForm)
                        {
                            lEmptyVector.push_back(lpForm->getBaseFormID());
                        }
                    }
                }
            }
            
            vector<int> lvNewElement;
            lvNewElement.push_back(ei);
            
            vector<string> lvDisplayText;
            
            hr = DSSAEHelper::GetDisplayTextsForTabularUnitForDDIC(static_cast<DSSTabularAttribute *>(lpUnit),lpObjectInfo,mGUID,lvNewElement,lvDisplayText,lEmptyVector,lAttributeFormSeperator,NULL,mpDataModel, lpGuitContainer);
            CHECK_HR;
            
            orText=lvDisplayText[0];
        
            //add to cache map
            mpDisplayTextMap->insert(map<int,string>::value_type(ei,orText));
        }
    }
    
    return S_OK;
}

DSSBaseElementProxy *DSSRWDDICInputControlIterator::getElementAt(int index)
{
    if (index == -1)
		return mpAllElement;
    
    
    if(index>=0&&index<mvElements.size())
    {
        if(mpElementProxyMap)
        {
            int ei=mvElements[index];
            
            map<int,DSSBaseElementProxy*>::iterator it=mpElementProxyMap->find(ei);
            
            if(it!=mpElementProxyMap->end())
            {
                return it->second;
            }
            
            
            DSSBaseElementProxy *lpElement=NULL;
            
            hGenerateSingleElement(index, &lpElement);
            
            map<int,DSSBaseElementProxy*>::iterator oldIte = mpElementProxyMap->find(ei);
            if(oldIte != mpElementProxyMap->end())
            {
                delete oldIte->second;
                mpElementProxyMap->erase(ei);
            }
            
            mpElementProxyMap->insert(map<int,DSSBaseElementProxy*>::value_type(ei,lpElement));
            
            return lpElement;
        }
    }
    
    return NULL;
}

//xiazhou, be aware calling this method may result in generating all elements in DDIC, which is time consuming.
int DSSRWDDICInputControlIterator::FindElement(DSSBaseElementProxy *ipElement)
{
    
    int lnElements = mvElements.size();
	for (int i = 0; i < lnElements; i++)
	{
		int ei = mvElements[i];
        
        if(ei == DSSXTAB_TOTAL_KEY)
        {
            if(ipElement->getElementType()==DssElementSubtotal)
                return mbShowAll ? i + 1 : i;
            else
                continue;
        }

        DSSBaseElementProxy *lpElement = getElementAt(i);
        
		if (lpElement)
		{
			if (lpElement->IsEqual(ipElement))
				return mbShowAll ? i + 1 : i;
		}
	}
	return -1;
}

int DSSRWDDICInputControlIterator::FindElementDisplayIndex(DSSBaseElementProxy *ipElement)
{
    return FindElement(ipElement);
}

int DSSRWDDICInputControlIterator::getElementMatchText(DSSBaseElementProxy* ipElement, std::string &orText)
{
    //TODO: support this in new rwd
    DSSRWControl *lpControl = mpObjectContext->getControl(mControlKey);
    CHECK_PTR(lpControl);
    
    DSSAttributeFormWrappers* lpForms = lpControl->getMatchForms();
    CHECK_PTR(lpForms);
    
    if(lpForms->Count()<1)
        return S_FALSE;
    
    DSSAttributeFormWrapper* lpForm = lpForms->Item(0);
    CHECK_PTR(lpForm);
    
    int lBaseFormID = lpForm->getBaseFormID();
    return getElementFormValue(ipElement, lBaseFormID, orText);
}

ControlCell DSSRWDDICInputControlIterator::getControlCell()
{
    return ControlCell(mCursor,this);
}


ControlCell DSSRWDDICInputControlIterator::getControlCellAt(int index)
{
	//index range from [0, N), where N is the value returned by CountRows
	if (mbShowAll)
	{
		if (index > 0)
			return ControlCell(index - 1, this);
		else
			return ControlCell(-1, this);
	}
	else
	{
		return ControlCell(index, this);
	}
}

int DSSRWDDICInputControlIterator::getTabularUnit(GUID &iGUID, DSSTabularUnit **oppUnit)
{
    int hr = S_OK;
    DSSTabularUnit *lpUnit;
    EnsureContainerModelData();
    EnumDSSRWNodeType lNodeType = mpNodeModel->getNodeType();
    if(lNodeType == DssRWNodeTemplate)
    {
        DSSRWGridModel* lpGridModel = static_cast<DSSRWGridModel*>(mpNodeModel);
        hr = lpGridModel->getAttrForDDIC(mControlKey, iGUID, &lpUnit);
        CHECK_HR;
    } else if (lNodeType == DssRWNodeFieldGroup)
    {
        DSSRWFieldGroupModel* lpFGModel = static_cast<DSSRWFieldGroupModel*>(mpNodeModel);
        //hr = lpFGModel->getDDICIndexByControlKey(mControlKey, mvElements);
        hr = lpFGModel->getAttrForDDIC(mControlKey, iGUID, &lpUnit);
        CHECK_HR;
    } else
    {
        _ASSERT(false);
    }
    
    *oppUnit = lpUnit;
    return S_OK;
}

int DSSRWDDICInputControlIterator::generateMatchFormTexts()
{
    int hr = S_OK;
    //1. get match text form
    DSSRWControl *lpControl = mpObjectContext->getControl(mControlKey);
    CHECK_PTR(lpControl);
    
    DSSAttributeFormWrappers* lpForms = lpControl->getMatchForms();
    CHECK_PTR(lpForms);
    
    if(lpForms->Count()<1)
        return S_FALSE;
    
    DSSAttributeFormWrapper* lpForm = lpForms->Item(0);
    CHECK_PTR(lpForm);
    
    int lMatchTextFormID = lpForm->getBaseFormID();
    
    //2. get the lookup column
    DSSTabularUnit* lpTabularUnit = NULL;
    hr = getTabularUnit(mGUID, &lpTabularUnit);
    //hr=mpDataModel->GetTabularUnit(lUnitGUID, lGlobalKey, &lpTabularUnit,&lLocalKey);
    CHECK_HR_PTR(lpTabularUnit);
    
    EnumDSSTemplateUnitType lType = lpTabularUnit->getUnitType();
    if(lType != DssTemplateAttribute)
        return S_FALSE;
    
    DSSTabularAttribute* lpTabularAttribute = static_cast<DSSTabularAttribute*>(lpTabularUnit);
        
    int lColumn = -1;
    hr = lpTabularAttribute->FindFormWithLocale(0, lMatchTextFormID, 0, lColumn);
    CHECK_HR;
        
    DSSDataColumn* lpColumn = NULL;
    hr = lpTabularAttribute->Item(lColumn, &lpColumn);
    CHECK_HR;
    
    mvMatchText.clear();
    //3. generate the texts from the element keys
    for(int i = 0; i < mvElements.size(); i++)
    {
        int lKey = mvElements[i];
        
        const char* lpData = NULL;
        int lSize = 0;
        EnumDSSDataType lType = DssDataTypeReserved;
        DSSDataFlag lFlag = DssDataOk;
        
        hr = lpColumn->getData(lKey, &lpData, lSize, lType, lFlag);
        CHECK_HR;
        
        std::string lValue;
        hr = DSSAEHelper::ConvertToString(lpData, lSize, lType, lFlag, lValue);
        CHECK_HR;
        
        mvMatchText.push_back(lValue);
    }
    
    return hr;
}

int DSSRWDDICInputControlIterator::getElementMatchTexts(vector<string>** oppTexts)
{
    int hr = S_OK;
    if(mvMatchText.size() == 0)
    {
        hr = generateMatchFormTexts();
        CHECK_HR;
    }
    if(oppTexts)
        (*oppTexts) = &mvMatchText;
    return S_OK;
    
}

//This is used for barcode style input. In this case, all the display texts are needed.
int DSSRWDDICInputControlIterator::getElementDisplayTexts(vector<string>** oppTexts)
{
    int hr = S_OK;
    if(oppTexts)
    {
        hr = hGetDisplayText(mpObjectContext->getControl(mControlKey));
        CHECK_HR;
        (*oppTexts) = &mvDisplayText;
    }
    return S_OK;
    
}

int DSSRWDDICInputControlIterator::getCachedElementDisplayTexts(vector<string>** oppTexts)
{
    int hr = S_OK;
    if (mvDisplayText.size() ==0) {
        //de39486, msun, get the unordered list, to be consist with mvWritebackText
        vector<int> lvElements;
        for (int i = 0; i < mvElements.size(); i++) {
            lvElements.push_back(i);
        }
        
        hr = hGetDisplayText(mpObjectContext->getControl(mControlKey), &lvElements);
        CHECK_HR;
    }
    if (oppTexts) {
        (*oppTexts) = &mvDisplayText;
    }
    return hr;
}

//xiazhou, find unit's position in DDIC's index
int DSSRWDDICInputControlIterator::findLocalUnitKey(GUID &iGUID, int &oKey)
{
    int hr = S_OK;
    EnsureContainerModelData();
    EnumDSSRWNodeType lNodeType = mpNodeModel->getNodeType();
    if(lNodeType == DssRWNodeTemplate)
    {
        DSSRWGridModel* lpGridModel = static_cast<DSSRWGridModel*>(mpNodeModel);
        hr = lpGridModel->getAttrIDForDDIC(mControlKey, iGUID, oKey);
        CHECK_HR;
    }
    else if (lNodeType == DssRWNodeFieldGroup)
    {
        DSSRWFieldGroupModel* lpFGModel = static_cast<DSSRWFieldGroupModel*>(mpNodeModel);
        hr = lpFGModel->getAttrIDForDDIC(mControlKey, iGUID, oKey);
        CHECK_HR;
    }
    else
    {
        _ASSERT(false);
    }
    
    return S_OK;
}

//xiazhou, ensure contianer's model
int DSSRWDDICInputControlIterator::EnsureContainerModelData()
{
    int hr = S_OK;
    
    DSSRWNodeModel *lpContainerModel = mpDataModel->getLatestNodeModel(mContainerKey);
    
    if(lpContainerModel && lpContainerModel != mpNodeModel)
        mpNodeModel = lpContainerModel;
    
    EnumDSSRWNodeType lNodeType = mpNodeModel->getNodeType();
    if(lNodeType == DssRWNodeTemplate)
    {
        hr = static_cast<DSSRWGridModel *>(mpNodeModel)->updateDDIC(mControlKey);
    }
    else if(lNodeType == DssRWNodeFieldGroup)
    {
        hr = static_cast<DSSRWFieldGroupModel *>(mpNodeModel)->updateDDIC(mControlKey);
    }
    
    return hr;
}

DSSBaseElementsProxy *DSSRWDDICInputControlIterator::getReplacedElementsForGB(GUID &iGUID)
{
    if(mReplaceElementsMap.find(iGUID) == mReplaceElementsMap.end())
        return NULL;
    else
        return mReplaceElementsMap[iGUID];
}

int DSSRWDDICInputControlIterator::hFindElementInLocal(DSSBaseElementProxy *ipElement, DSSTabularUnit *ipUnit, int &oLocalKey)
{
    int hr = S_OK;
    oLocalKey = -1;
    
    if(!ipElement || !ipUnit)
        return E_POINTER;
    
    hr = ipUnit->FindElement(ipElement, oLocalKey);
    if(hr == S_OK)
        return hr;
    else if(ipElement ->getElementType() != DssElementNULL)
        return hr;
    
    //Now, handle join null element in local lookup table.
    //Currently DDIC local lookup only contain attribute.
    DSSTabularAttribute *lpAttr = static_cast<DSSTabularAttribute *>(ipUnit);
    if(!lpAttr)
        return S_FALSE;
    
    if(lpAttr->Count() == 0)
        return S_FALSE;
    
    DSSDataColumn *lpColumn = NULL;
    hr = lpAttr->Item(0, &lpColumn);
    CHECK_HR_PTR(lpColumn);
    
    int lnRowCount = lpColumn->getRowCount();
    for(int i = lnRowCount - 1; i >= 0; i--)
    {
        DSSDataFlag lFlag;
        hr = lpColumn->getFlag(i, lFlag);
        CHECK_HR;
        
        if(lFlag == DssDataNull)
        {
            oLocalKey = i;
            return S_OK;
        }
    }
    
    return S_FALSE;
    
}