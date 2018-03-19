/*
 *  DSSRWControlModelIterator.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 9/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#import "DSSViewDataSetHelper.h"
#include "DSSRWControlModelIterator.h"
#include "DSSRWControlNode.h"
#include "DSSRWGridModel.h"
#include "DSSCubeDataSlice.h"
#include "DSSSortConverter.h"
#include "DSSDataModelControlImpl.h"
#include "DSSSorter.h"
#include "DSSSortDefinition.h"
#include "DSSTabularAttribute.h"
#include "DSSStrongPtr.h"
#include "DSSConsolidation.h"
#include "DSSConsolidationElement.h"
#include "DSSTabularConsolidation.h"
#include "DSSTabularCustomGroup.h"
#include "DSSRWCompare.h"
#include "DSSSort_includes.h"
#include "DSSGUnitContainer.h"
#include "DSSRWTemplateNode.h"


#include "DSSCExpression.h"
#include "DSSExpNode.h"
#include "DSSExpNodeOperator.h"
#include "DSSExpNodeElementsObject.h"
#include "DSSUnitInfos.h"
#include "DSSUnitInfo.h"
#include "DSSAttributeListElementProxy.h"

DSSRWControlModelIterator::DSSRWControlModelIterator()
{
    mbAfterManipulation=false;
    mpControl=NULL;
    mbCache=false;
    mpObjectInfo=NULL;
    mpControlFormat=NULL;
    mpTabularAttribute=NULL;
}

DSSRWControlModelIterator::~DSSRWControlModelIterator()
{
    if(mpDataModel)
    {
        mpDataModel->removeNodeKey2Iterator(DSSRWNodeModelIterator::mNodeKey, this);
        hClearCacheValues();
    }
}

int DSSRWControlModelIterator::Init(DSSRWDataModel* pDataModel, DSSRWNodeModel *pNodeModel, DSSRWNode *pNode,DSSRWSectionModelIterator *pParentIterator,vector<Int32> & pCurrentElements, DSSTabularIndex *pGroupingIndex)

{
    int hr =DSSRWNodeModelIterator::Init(pDataModel,pNode,pNodeModel,pParentIterator);
    CHECK_HR;
    
    mpDataModel->addNodeKey2Iterator(pNode->getKey(), this);
    mGlobalLookupVer = mpDataModel->getGlobalLookupVer();
    mpGroupingIndex = pGroupingIndex;
    
    
    mvElements.assign(pCurrentElements.begin(), pCurrentElements.end());
    
    mCursor=0;
    
    mnElements = mvElements.size();
    
    
    DSSRWControl *lpControl = NULL;
	DSSRWControls *lpControls = static_cast<DSSRWControlNode*> (pNode)->getControls();
	if(lpControls)
	{
		int lnControls = lpControls->Count();
		if (lnControls > 0)
		{
			lpControl = lpControls->Item(0);
		}
	}
    
    mpControl=lpControl;
	
    DSSRWControlIterator::mControlKey = lpControl->getKey();
    
    //initialize base class DSSRWControlIterator
    DSSRWControlIterator::mType = DssIteratorControl;
    DSSRWControlIterator::mpParentIterator = dynamic_cast<DSSRWSectionIterator*>(pParentIterator);
    DSSRWControlIterator::mNodeKey = pNode->getKey();
    DSSRWControlIterator::mpObjectContext = pNode->getObjectContext();
    
    
    DSSRWNodeModelIterator::mType = DssIteratorControl;
    //DSSRWNodeModelIterator::mpParentIterator = dynamic_cast<DSSRWSectionIterator*>(pParentIterator);
    DSSRWNodeModelIterator::mNodeKey = pNode->getKey();
    DSSRWNodeModelIterator::mpObjectContext = pNode->getObjectContext();
    
    
    
    
    //hyan,2012/06/07
    //if candidte elements from server, we donot need to sort
    
    if(false==dynamic_cast<DSSRWControlModel*>(mpNodeModel)->getCandidateElementsFromServerFlag())
    {    
        if(lpControl&&lpControl->getType()==DssRWControlTypeElementList)
        {
            
            //junwang, 12/26/2011
            if (mnElements>1)
            {
                //TODO:  sorting
                DSSRWControlCompare lCMP;
                lCMP.Init(this, mpDataModel->GetDisplayNullOnTop());
                
               _xtab_sort(&mvElements[0], &mvElements[0]+mnElements, lCMP);
                
    /*
                int* lpIndex = new int[mnElements];
                int* lpTemp = new int[mnElements];
                
                for (int i=0; i<mnElements; i++)
                {
                    lpIndex[i] = i;
                }
                
                _xtab_sort(lpIndex,lpIndex+mnElements,lCMP);
                
                for (int i=0; i<mnElements; i++)
                {
                    lpTemp[i] = mvElements[lpIndex[i]];
                }
                memmove(&mvElements[0],lpTemp,sizeof(int)*mnElements);
                
                delete[] lpIndex;
                delete[] lpTemp;
    */
            }
        }
    }
    
    
    if (hShowAll(lpControl))	
	{
		mbShowAll = true;
		DSSAllElementProxy *lpAllElement = new DSSAllElementProxy(DSSRWNodeModelIterator::mpObjectContext);
		lpAllElement->Parse(MBase::CString("OA:(All)"));
		mpAllElement = lpAllElement;
	}
    
    
    DSSObjectContext *lpObjContext=mpDataModel->getObjectContext();//this will be a member
    
    GUID lControlObjectID = lpControl->getSourceID();
    
    
    DSSGUnitContainer* lpGuitContainer = NULL;
    
    mpDataModel->GetGlobalLookupContainer(&lpGuitContainer);
    DSSObjectInfo *lpObjectInfo = lpObjContext->FindObjectH(lControlObjectID);
    
    //hyan,2012/05/23
    //add for optimize
    mpObjectInfo=lpObjectInfo;
    EnumDSSObjectType lType= lpObjectInfo ? lpObjectInfo->getType() : DssTypeReserved;
    mpControlFormat=mpControl->getFormat(DssRWControlFormatControl);
    
    //xiazhou, 969920, document level attribute formatting
    CComVariant* lpVal = NULL;
    if (mpControlFormat)
    {
        lpVal = mpControlFormat->getPropertyValue("FormattingNumber", "Format");
    }
    bool lbGeneral = false;
    if(lpVal)
        lbGeneral = (lpVal->vt == VT_I2) ? lpVal->iVal==9 : lpVal->ToString()=="General";

    if((!lpVal || lbGeneral) && (lType == DssTypeAttribute || lType == DssTypeFilter|| lType == DssTypeConsolidation))
    {//use document level setting
        const DSSUnitInfos* lpUnitInfos = DSSRWControlIterator::mpObjectContext->getUnitInfos(GUID_NULL);
        if(lpUnitInfos)
        {
            const DSSUnitInfo* lpUnitInfo = lpUnitInfos->getUnitInfo(lControlObjectID);
            if(lpUnitInfo)
            {
                mpControlFormat = DSSRWControlIterator::mpObjectContext->getFormat(lpUnitInfo->formatRefID());
            }
        }
    }
    
    
    if(lpObjectInfo)
    {
        //special handle for metric condition selector
        if(lType==DssTypeMetric)
        {
            mvDisplayText.clear();
            mvElements.clear();
            mvSelectedCollection.clear();
            mnElements=0;
            mbShowAll=false;
            
            return S_OK;
        }
        
        
        
        GUID lGUID=lpObjectInfo->getID();
        
        //hyan,2012/05/28
        //cache tabularAttribute for getElementText and genereate element proxy
        mpTabularAttribute=mpDataModel->GetTabularAttributeByGUID(lGUID);
    }
    
    
    //2017/5/23, msun, for financial report
    hGenerateLevelInfo();
    
    //hyan,2012/03/30
    //for testing incremental rendering of selector
    if(lpControl->getType()==DssRWControlTypeElementList)
    {
        //maybe we can check whether to do cache according the size of mvElements
        //if(mnElements>100)
        mbCache=true;
        
        if(mbCache)
        {
            //hyan,2012/03/29, 
            //get the DisplayTextMap and ElementProxyMap from DataModel
            mpDisplayTextMap=mpDataModel->getDisplayTextMap(DSSRWNodeModelIterator::mNodeKey);
            mpElementProxyMap=mpDataModel->getElementProxyMap(DSSRWNodeModelIterator::mNodeKey);

            
            return S_OK;
        }
    }
    
    
        
    
    //ToDo:
    //get mvDisplayText
    hr=hgetDisplayText(lpControl,mvDisplayText);
    
    CHECK_HR;
    
	//initialize mpElement to contain all elements
	int lnTotalElements = mvDisplayText.size();
    
    mpElements.clear();
	mpElements.resize(lnTotalElements);
    
	for (int i = 0; i < lnTotalElements; i++)
	{
		mpElements[i] = NULL;
	}
    
    
    
    return hGenerateElements(lpControl);
}


int  DSSRWControlModelIterator::ReInit()
{
    //TQMS 687552, hyan, 28/11/2012
    //we need to update node model at the beginning
    int hr=0;
    
    //zhyang, 01/11/2012
    bool lbUpdated = false;
    hr = mpDataModel->updateNodeModel(DSSRWNodeModelIterator::mNodeKey, lbUpdated);
    CHECK_HR;
    
    //hyan,2011/12/2
    //Bind new NodeModel for UC
    DSSRWNodeModel* lpChildNodeModel = mpDataModel->getNodeModel(DSSRWNodeModelIterator::mNodeKey);
    mpNodeModel=lpChildNodeModel;

    
    
    
    DSSRWControl *lpControl = NULL;
	DSSRWControls *lpControls = static_cast<DSSRWControlNode*> (mpNode)->getControls();
	if(lpControls)
	{
		int lnControls = lpControls->Count();
		if (lnControls > 0)
		{
			lpControl = lpControls->Item(0);
		}
	}
    
    
    DSSObjectContext *lpObjContext=mpDataModel->getObjectContext();//this will be a member
    
    GUID lControlObjectID = lpControl->getSourceID();
    
    
    DSSGUnitContainer* lpGuitContainer = NULL;
    
    mpDataModel->GetGlobalLookupContainer(&lpGuitContainer);
    
    DSSObjectInfo *lpObjectInfo=lpObjContext->FindObjectH(lControlObjectID);
	
    if(lpObjectInfo)
    {
        EnumDSSObjectType lType=lpObjectInfo->getType();
        
        //special handle for metric condition selector
        if(lType==DssTypeMetric)
        {
            mvDisplayText.clear();
            mvElements.clear();
            mvSelectedCollection.clear();
            mnElements=0;
            mbShowAll=false;
            
            return S_OK;
        }
        
        GUID lGUID=lpObjectInfo->getID();
        
        
        //hyan,2012/05/28
        //cache tabularAttribute for getElementText and genereate element proxy
        mpTabularAttribute=mpDataModel->GetTabularAttributeByGUID(lGUID);

    }
    
    
    mCursor=0;
    mnElements=0;
    mvElements.clear();
    mvDisplayText.clear();
    mvTemplateMetricsGUID.clear();
    mvTemplateAttributesGUID.clear();
    mvPanelNodeKeys.clear();
    hClearCacheValues();
    mvElementInfo.clear();
    mvSelectedCollection.clear();
        
    //hyan,2012/02/15
    //TQMS 551064
    //TQMS 584194 ,hyan,2012/04/28
    //we need to do sepecial handle for unselect all
    
    //TQMS671256,hyan,2012/10/23
    //in 930 DHTML mode, for empty selection, we will show All
//    if(hCheckParentSelectorEmptyChoose())
//    {
//        mbShowAll=false;
//        return S_OK;
//    }
    
    
    vector<Int32> lvCurrentElements;
    
    vector<vector<Int32> > lvvGroupByElements;
    

    DSSRWNodeModelIterator::mpParentIterator->getGroupByElements(lvvGroupByElements);
    
    hr = dynamic_cast<DSSRWControlModel*>(mpNodeModel)->getCandidateElementsForRefresh(lvvGroupByElements,lvCurrentElements,DSSRWNodeModelIterator::mpParentIterator);
    CHECK_HR;
    
    mvElements.assign(lvCurrentElements.begin(), lvCurrentElements.end());
    
    mnElements=mvElements.size();
    
    //hyan,2012/06/07
    //if candidte elements from server, we donot need to sort
    if(false==dynamic_cast<DSSRWControlModel*>(mpNodeModel)->getCandidateElementsFromServerFlag())    
    {    
        if(lpControl&&lpControl->getType()==DssRWControlTypeElementList)
        {
            //junwang, 12/26/2011
            if (mnElements>1)
            {
                //TODO:  sorting
                DSSRWControlCompare lCMP;
                hr = lCMP.Init(this, mpDataModel->GetDisplayNullOnTop());
                CHECK_HR;
                
                bool sorted = false;
                
                int lUnit;
                vector<int> lvFroms;
                bool *lpAscs = NULL;
                hr = lCMP.getSortInfo(lUnit, lvFroms, &lpAscs);
                MBase::StrongPtr<bool, MBase::DeleteArray<bool> > lpAscsPtr(lpAscs);
                CHECK_HR
                
                
                DSSGUnitContainer *lGlobalLookupContianer;
                hr = mpDataModel->GetGlobalLookupContainer(&lGlobalLookupContianer);
                CHECK_HR;
                
                EnumDSSTemplateUnitType lUnitType;
                bool lfDE = false;
                hr = lGlobalLookupContianer->GetUnitType(lUnit, &lUnitType, &lfDE);
                CHECK_HR;
                
                if(lUnitType == DssTemplateAttribute && !lfDE)
                {
                    int lBucketID = lGlobalLookupContianer->getBucketID(lUnit, lvFroms, lpAscs, mpDataModel->GetDisplayNullOnTop());
                    if(true  &&  lBucketID != -1)
                    {
                        int lnBuckets;
                        int *lpBuckets;
                        hr = lGlobalLookupContianer->getSortResult(lUnit, lBucketID, &lpBuckets, &lnBuckets);
                        CHECK_HR;
                        
                        //zhyang, 875892, the element in lpBuckets may be same if we sort a string column in client side.
                        vector<vector<int> > lvvAttrBucket;
                        lvvAttrBucket.resize(lnBuckets);
                        
                        bool lbHasTotal = false;
                        for(int i = 0; i < mvElements.size(); ++i)
                        {
                            if(mvElements[i] == DSSXTAB_TOTAL_KEY)
                                lbHasTotal = true;
                            else
                                lvvAttrBucket[lpBuckets[mvElements[i]]].push_back(mvElements[i]);
                        }
                        
                        mvElements.clear();
                        for(int i = 0; i < lnBuckets; ++i)
                        {
                            for(int j = 0; j < lvvAttrBucket[i].size(); ++j)
                                mvElements.push_back(lvvAttrBucket[i][j]);
                        }
                        if(lbHasTotal)
                            mvElements.push_back(DSSXTAB_TOTAL_KEY);
                        sorted = true;
                    }
                }

                if(!sorted)
                    _xtab_sort(&mvElements[0], &mvElements[0]+mnElements, lCMP);
    /*
                int* lpIndex = new int[mnElements];
                int* lpTemp = new int[mnElements];
                
                for (int i=0; i<mnElements; i++)
                {
                    lpIndex[i] = i;
                }
                
                _xtab_sort(lpIndex,lpIndex+mnElements,lCMP);
                
                for (int i=0; i<mnElements; i++)
                {
                    lpTemp[i] = mvElements[lpIndex[i]];
                }
                memmove(&mvElements[0],lpTemp,sizeof(int)*mnElements);
                
                delete[] lpIndex;
                delete[] lpTemp;
     */
            }
        }
    }
    
    if (hShowAll(lpControl))	//for metric selector, "ALL" is included in mpElements
	{
		mbShowAll = true;
	}
    
    
    if(!mbAfterManipulation)
        mbAfterManipulation=true;
    
    //2017/5/23, msun, for financial report
    hGenerateLevelInfo();
    
    //hyan,2012/03/30
    //for testing incremental rendering of selector
    if(lpControl->getType()==DssRWControlTypeElementList)
    {
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
            /*
            //777901, we need to adjust the current selected element's global key after UC on DS
            if(mpControl && mpControl->getCurrentElementsStatus() == DssCurrentElementChosen)
            {
                DSSBaseElementsProxy* lpCurrentElements = mpControl->getCurrentElements();
                if(lpCurrentElements->Count() > 0)
                {
                    for(int i = 0; i < lpCurrentElements->Count(); i++)
                    {
                        DSSBaseElementProxy* lpElement = lpCurrentElements->Item(i);
                        if(lpElement)
                        {
                            lpElement->setGlobalKey(-1);
                        }
                    }
                }
            }*/
            mGlobalLookupVer = mpDataModel->getGlobalLookupVer();
        }
        if(mbCache)
            return S_OK;
    }
    
    //ToDo:
    //get mvDisplayText
    hr=hgetDisplayText(lpControl,mvDisplayText);
    
    CHECK_HR;
    
    
    
    for(int i=0;i<mpElements.size();i++)
    {
        delete mpElements[i];
                
        mpElements[i]=NULL;
    }
    
	//initialize mpElement to contain all elements
	int lnTotalElements = mvDisplayText.size();
    
    
    mpElements.clear();
	mpElements.resize(lnTotalElements);
    
	for (int i = 0; i < lnTotalElements; i++)
	{
		mpElements[i] = NULL;
	}
    
    
    
    return hGenerateElements(lpControl);
    
}


int DSSRWControlModelIterator::hgetDisplayText(DSSRWControl *ipControl,vector<MBase::CString> &ivDisplayText)
{
    if(!ipControl)
        return E_POINTER;
    
	if (ipControl)
	{
        if(ipControl->getType()==DssRWControlTypeMetricList)
        {
            map<GUID, DSSTemplateMetric*> lvTemplateMetricsMap;
            
            //for metircs selector, we need to get metrics from the targets
            int lnTarget=ipControl->getCountTargets();
            
            for(int i=0;i<lnTarget;i++)
            {
                DSSRWNode* lpRWNode=ipControl->getTargetNode(i);
                
                if(lpRWNode)
                {
                    if(lpRWNode->getType()==DssRWNodeSection)
                    {
                        DSSRWSectionNode* lpSectionNode=dynamic_cast<DSSRWSectionNode*>(lpRWNode);
                        
                        if(lpSectionNode)
                        {
                            vector<DSSTemplateMetric*> lvTemplateMetrics;
                            
                            lpSectionNode->getControlMetrics(lvTemplateMetrics);
                            
                            int lnMetrics=lvTemplateMetrics.size();
                            
                            for(int j=0;j<lnMetrics;j++)
                            {
                                DSSTemplateMetric* lpTemplateMetric=lvTemplateMetrics.at(j);
                                
                                string lAlias=lpTemplateMetric->getAlias();
                                
                                GUID lTemplateMetricGUID=lpTemplateMetric->getID();
                                
                                map<GUID, DSSTemplateMetric*>::iterator lIter=lvTemplateMetricsMap.find(lTemplateMetricGUID);
                                
                                if(lIter==lvTemplateMetricsMap.end())
                                {
                                    lvTemplateMetricsMap.insert(map<GUID, DSSTemplateMetric*>::value_type(lTemplateMetricGUID,lpTemplateMetric));
                                    
                                    mvTemplateMetricsGUID.push_back(lTemplateMetricGUID);
                                    
                                    ivDisplayText.push_back(lAlias);
                                }
                                
                            }
                        }
                        
                    }
                    
                }
            }
            return S_OK;
            
        }
        if (ipControl->getType() == DssRWControlTypeAttributeList) {
            DSSAxis* lCandidatesTemplateUnits = ipControl->getCandidateAttributeElements();
            if(lCandidatesTemplateUnits)
            {
                int lnUnits = lCandidatesTemplateUnits->Count();
                for (Int32 i=0; i<lnUnits; i++) {
                    DSSTemplateUnit* lUnit = lCandidatesTemplateUnits->Item(i);
                    if(lUnit->getType() == DssTemplateAttribute)
                    {
                        mvTemplateAttributesGUID.push_back(lUnit->getID());
                        DSSTemplate* lTemplate = NULL;
                        for (int i=0; i<ipControl->getCountTargets(); i++) {
                            DSSRWNode* lTargetNode = ipControl->getTargetNode(i);
                            vector<DSSRWNode*> lLeafNodes;
                            lTargetNode->getDescendantNodes(&lLeafNodes);
                            for (int j=0; j<lLeafNodes.size(); j++) {
                                DSSRWNode* lTempNode = lLeafNodes[j];
                                if (lTempNode->getType() == DssRWNodeTemplate) {
                                    lTemplate = ((DSSRWTemplateNode*)lTempNode)->getTemplate();
                                    break;
                                }
                            }
                            if (lTemplate) {
                                break;
                            }
                        }
                        lCandidatesTemplateUnits->setTemplate(lTemplate);
                        ivDisplayText.push_back(lUnit->getAlias());
                    }
                }
            }
            return S_OK;
        }
        //panel selector
        else if(ipControl->getType()==DssRWControlTypeTabList)
        {
            int lnTarget=ipControl->getCountTargets();
            
            for(int i=0;i<lnTarget;i++)
            {
                DSSRWNode* lpRWNode=ipControl->getTargetNode(i);
                
                if(lpRWNode)
                {
                    if(lpRWNode->getType()==DssRWNodeSection)
                    {
                        DSSRWSectionNode* lpSectionNode=dynamic_cast<DSSRWSectionNode*>(lpRWNode);
                        
                        if(lpSectionNode&&lpSectionNode->getSectionType()==DssRWSectionContainer)
                        {
                            int lnChild=lpSectionNode->Count();
                            
                            for(int j=0;j<lnChild;j++)
                            {
                                DSSRWNode *lpChildNode=lpSectionNode->Item(j);
                                
                                if(lpChildNode->getType()==DssRWNodeSection)
                                {
                                    DSSRWSectionNode* lpChildSectionNode=dynamic_cast<DSSRWSectionNode*>(lpChildNode);
                                    
                                    if(lpChildSectionNode->getSectionType()==DssRWSectionPanel)
                                    {
                                        ivDisplayText.push_back(lpChildSectionNode->getTitle());
                                        
                                        mvPanelNodeKeys.push_back(lpChildSectionNode->getKey());
                                    }
                                }
                            }
                        }
                    }
                    
                }
            }
            
            return S_OK;
        }
        
        
    
        GUID lControlObjectID = ipControl->getSourceID();

        //junwang,02/08/2012, #547222
        if(MBase::IsGUIDNULL(lControlObjectID))
            return S_OK;
        
        DSSGUnitContainer* lpGuitContainer = NULL;
        mpDataModel->GetGlobalLookupContainer(&lpGuitContainer);
             
        GUID lGUID=mpObjectInfo->getID();
        
        
        int lnElements=mvElements.size();
        
        ivDisplayText.clear();
        
        ivDisplayText.resize(lnElements);
        
        vector<int> lvDisplayForms;
        dynamic_cast<DSSRWControlModel*>(mpNodeModel)->getDisplayForms(lvDisplayForms);
        
        MBase::CString lDelimeter;
        dynamic_cast<DSSRWControlModel*>(mpNodeModel)->getDelimeter(lDelimeter);
        string lAttributeFormSeperator=lDelimeter;

        DSSAEHelper::GetDisplayTextsForTabularUnit(lpGuitContainer,mpObjectInfo,lGUID,mvElements,ivDisplayText,lvDisplayForms,lAttributeFormSeperator,
                                                   mpControlFormat,mpTabularAttribute,mpDataModel);
    }
        
    return S_OK;
}



int DSSRWControlModelIterator::hGenerateElements(DSSRWControl *ipControl)
{
    if(!ipControl)
        return E_POINTER;
    
    int hr=S_OK;
    
    
    if(ipControl->getType()==DssRWControlTypeMetricList)
    {
        int lnMetrics=mvTemplateMetricsGUID.size();
        
        mvElements.clear();
        
        mvElements.resize(lnMetrics);
        
        mnElements=lnMetrics;
        
        
        for(int i=0;i<lnMetrics;i++)
        {
            mvElements[i]=i;
            
            if(mpElements[i])
                continue;
            
            GUID lGUID=mvTemplateMetricsGUID[i];
            
            DSSMetricElementProxy *lpElement=new DSSMetricElementProxy(DSSRWNodeModelIterator::mpObjectContext,lGUID);
            
            
            setElementAt(i, lpElement);
            
            
        }
        
        return S_OK;
    }
    else if (ipControl->getType() == DssRWControlTypeAttributeList)
    {
        int lnAttributes=mvTemplateAttributesGUID.size();
        mvElements.clear();
        mvElements.resize(lnAttributes);
        mnElements = lnAttributes;

        vector<MBase::CString> lElementIDs = dynamic_cast<DSSRWControlModel*>(mpNodeModel)->getCandidateAttributeElement();
       // assert(lElementIDs.size() == lnAttributes || (lElementIDs.size()==0));
        for (int i = 0; i<lElementIDs.size(); i++) {
            mvElements[i]=i;
            
            if(mpElements[i])
                continue;
            
           // GUID lGUID=mvTemplateAttributesGUID[i];
            
            DSSAttributeListElementProxy *lpElement=new DSSAttributeListElementProxy(DSSRWNodeModelIterator::mpObjectContext);
            lpElement->Parse(lElementIDs[i]);
            
            setElementAt(i, lpElement);
        }
        return S_OK;
    }
    else if(ipControl->getType()==DssRWControlTypeTabList)
    {
        int lnPanels=mvPanelNodeKeys.size();
    
        mvElements.clear();
        
        mvElements.resize(lnPanels);
        
        mnElements=lnPanels;
        
        
        for(int i=0;i<lnPanels;i++)
        {
            mvElements[i]=i;
            
            if(mpElements[i])
                continue;
            
            string lnodekey=mvPanelNodeKeys[i];
            
            DSSNodeElementProxy *lpElement=new DSSNodeElementProxy(DSSRWNodeModelIterator::mpObjectContext,lnodekey);
            
            string lPrefix="PA:";
            string lElementID=lPrefix.append(lnodekey);
            
            lpElement->Parse(lElementID);
            
            setElementAt(i, lpElement);
        }
        
        return S_OK;
        
    }
    
    
    DSSObjectContext *lpObjContext=mpDataModel->getObjectContext();//this will be a member
    
    GUID lControlObjectID = ipControl->getSourceID();
    //junwang, 02/08/2012, 547222
    if(MBase::IsGUIDNULL(lControlObjectID))
        return S_OK;
    

    
    DSSObjectInfo *lpObjectInfo=lpObjContext->FindObjectH(lControlObjectID);
    
    GUID lGUID=lpObjectInfo->getID();
    
    EnumDSSObjectType lType=lpObjectInfo->getType();
    
    switch (lType) {
        case DssTypeAttribute:
        {
            
            DSSTabularAttribute *lpTabularAttribute=NULL;
            
            if(mpTabularAttribute)
                lpTabularAttribute=mpTabularAttribute;
            else
               lpTabularAttribute=mpDataModel->GetTabularAttributeByGUID(lGUID);
            
            DSSTabularUnit* lpUnit = NULL;
            
            DSSGlobalLookUpTable* lpLookUp = dynamic_cast<DSSGlobalLookUpTable*>(lpTabularAttribute);
            
            
            int lnElements = mvElements.size();
            
            //only create the ones currently used
            for (int i = 0; i < lnElements; i++)
            {
                int ei = mvElements[i];
                
                
                //hyan,2012/03/28
                //maybe already generated
                DSSBaseElementProxy *lpElement = NULL;
                
                int lLocalKey = -1;
                
                if(NULL==lpTabularAttribute)
                {
                    
                    lpElement = new DSSNullElementProxy(DSSRWNodeModelIterator::mpObjectContext,lGUID);
                    
                    setElementAt(i, lpElement);
                    
                    continue;
                }
                
                
                
                if(ei<0)
                {
                    hr = lpTabularAttribute->createElement(ei, DSSRWNodeModelIterator::mpObjectContext, &lpElement);
                    CHECK_PTR(lpElement);
                }
                else
                {
                    lpUnit = NULL;
                    
                    bool lfDE=false;
                    //DE
                    lpLookUp->GetGlobalUnit(ei, &lpUnit, &lLocalKey, &lfDE);
                    
                   
                    int lMaxKey = -1;
                    hr = lpLookUp->GetMaxGlobalKey(&lMaxKey);
                    CHECK_HR;
                    if (ei==lMaxKey+1)
                    {
                        lpElement = new DSSNullElementProxy(DSSRWNodeModelIterator::mpObjectContext,lGUID);
                    }
                    else                    
                    {
                        hr = lpUnit->createElement(lLocalKey, DSSRWNodeModelIterator::mpObjectContext, &lpElement);
                        CHECK_HR;   
                    }
                }
                
                setElementAt(i, lpElement);
                
                
                
            }//end of for (i = 0; i < lnElements; i++)
            
        }
            break;
            
        case DssTypeConsolidation:
        {
            int lnElements = mvElements.size();
            
            //only create the ones currently used
            for (int i = 0; i < lnElements; i++)
            {
                int ei = mvElements[i];
                
                //special handle for total
                if(ei==DSSXTAB_TOTAL_KEY)
                {
                    //subtotal index for total will be 1,hyan, 11/12/2012
                    DSSBaseElementProxy* lpSubTotalElement=new DSSSubtotalElementProxy(DSSRWNodeModelIterator::mpObjectContext,lGUID,1);
                    
                    setElementAt(i, lpSubTotalElement);
                    
                }
                else
                {
                    //junwang, 2012/02/21, null element
                    bool lfNULL = false;
                    hr = mpDataModel->IsCompoundNull(lGUID, ei, &lfNULL);
                    CHECK_HR;
                    if(lfNULL)
                    {
                        DSSBaseElementProxy* lpElement = new DSSNullElementProxy(DSSRWNodeModelIterator::mpObjectContext,lGUID);
                        setElementAt(i, lpElement);
                    }
                    else
                    {
                        DSSTabularUnit *lpTabularUnit=NULL;
                        int lLocalKey=-1;
                        hr=mpDataModel->GetTabularUnit(lGUID, ei, &lpTabularUnit,&lLocalKey);
                        CHECK_HR;
                        
                        if(lpTabularUnit)
                        {
                            DSSTabularConsolidation *lpTabularConsolidation=static_cast<DSSTabularConsolidation*>(lpTabularUnit);
                            
                            if(lpTabularConsolidation)
                            {
                                DSSBaseElementProxy *lpElement = NULL;
                                hr = lpTabularConsolidation->createElement(lLocalKey, DSSRWNodeModelIterator::mpObjectContext, &lpElement);
                                CHECK_PTR(lpElement);
                                
                                setElementAt(i, lpElement);
                            }
                        }
                    }
                }
                
            }//end of for (i = 0; i < lnElements; i++)
            
        }
            break;
            
        case DssTypeFilter:
        {
            int lnElements = mvElements.size();
            
            //only create the ones currently used
            for (int i = 0; i < lnElements; i++)
            {
                int ei = mvElements[i];
                
                //special handle for total
                if(ei==DSSXTAB_TOTAL_KEY)
                {
                    //subtotal index for total will be 1,hyan, 11/12/2012
                    DSSBaseElementProxy* lpSubTotalElement=new DSSSubtotalElementProxy(DSSRWNodeModelIterator::mpObjectContext,lGUID,1);
                    
                    setElementAt(i, lpSubTotalElement);
                    
                }
                else
                {
                    //junwang, 2012/02/21, null element
                    bool lfNULL = false;
                    hr = mpDataModel->IsCompoundNull(lGUID, ei, &lfNULL);
                    CHECK_HR;
                    if(lfNULL)
                    {
                        DSSBaseElementProxy* lpElement = new DSSNullElementProxy(DSSRWNodeModelIterator::mpObjectContext,lGUID);
                        setElementAt(i, lpElement);
                    }
                    else
                    {
                        DSSTabularUnit *lpTabularUnit=NULL;
                        int lLocalKey=-1;
                        hr=mpDataModel->GetTabularUnit(lGUID, ei, &lpTabularUnit,&lLocalKey);
                        CHECK_HR;
                        
                        if(lpTabularUnit)
                        {
                            DSSTabularCustomGroup *lpTabularCustomGroup=dynamic_cast<DSSTabularCustomGroup*>(lpTabularUnit);
                            
                            if(lpTabularCustomGroup)
                            {
                                DSSBaseElementProxy *lpElement = NULL;
                                hr = lpTabularCustomGroup->createElement(lLocalKey, DSSRWNodeModelIterator::mpObjectContext, &lpElement);
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

DSSBaseElementProxy* DSSRWControlModelIterator::CreateElementFromElementID(MBase::CString &irElementID)
{
	EnumDSSElementType lType = DSSBaseElementProxy::ParseElementType(irElementID);
	switch (lType) {
		case DssElementMetric:
		{
            DSSMetricElementProxy *lpMetricElement = new DSSMetricElementProxy(DSSRWNodeModelIterator::mpObjectContext);
            lpMetricElement->Parse(irElementID);
            return lpMetricElement;
		}
		case DssElementSubtotal:
		{
			DSSSubtotalElementProxy *lpSubtotalElement = new DSSSubtotalElementProxy(DSSRWNodeModelIterator::mpObjectContext);
			lpSubtotalElement->Parse(irElementID);
			return lpSubtotalElement;
		}
		case DssElementNULL:
		{
			DSSNullElementProxy *lpNullElement = new DSSNullElementProxy(DSSRWNodeModelIterator::mpObjectContext);
			lpNullElement->Parse(irElementID);
			return lpNullElement;
		}
		case DssElementAll:
		{
            DSSAllElementProxy *lpAllElement = new DSSAllElementProxy(DSSRWNodeModelIterator::mpObjectContext);
            lpAllElement->Parse(irElementID);
            return lpAllElement;
		}
		case DssElementNode:
		{
            DSSNodeElementProxy *lpNodeElement = new DSSNodeElementProxy(DSSRWNodeModelIterator::mpObjectContext);
            lpNodeElement->Parse(irElementID);
            return lpNodeElement;
		}
		default:
			break;
	}
	return NULL;
}

//hyan,16/11/2011, new RWD
int DSSRWControlModelIterator::getElementText(int iIndex,std::string &orText)
{
    if(mbCache)
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
            
                
                GUID lGUID=mpObjectInfo->getID();
                
        
                
                vector<int> lvNewElement;
                
                lvNewElement.push_back(ei);
                
                
                vector<MBase::CString> lvNewElementDisplayText;
                vector<int> lvDisplayForms;
                dynamic_cast<DSSRWControlModel*>(mpNodeModel)->getDisplayForms(lvDisplayForms);
                
                MBase::CString lDelimeter;
                dynamic_cast<DSSRWControlModel*>(mpNodeModel)->getDelimeter(lDelimeter);
                string lAttributeFormSeperator=lDelimeter;
                
                DSSAEHelper::GetDisplayTextsForTabularUnit(lpGuitContainer,mpObjectInfo,lGUID,lvNewElement,lvNewElementDisplayText,
                                                           lvDisplayForms,lAttributeFormSeperator,mpControlFormat,mpTabularAttribute,mpDataModel);
                
                
                orText=lvNewElementDisplayText[0];
                
                
                //add the cache map
                mpDisplayTextMap->insert(map<int,string>::value_type(ei,orText));
            }
        }
    }
    else
    {

        if(iIndex>=0&&iIndex<mvDisplayText.size())
        {
                orText=mvDisplayText[iIndex];
        }
    }
    
    
    return S_OK;
}


int DSSRWControlModelIterator::hget_ElementIndex(int iRow, int iUnit, int *pElementIndex)
{
    *pElementIndex = iRow;
    return S_OK;
}


int DSSRWControlModelIterator::getCommandList(DSSCellHandle &irHandle, vector<EnumManipulationCommandTypes> &orvCommand)
{
    return DSSRWControlIterator::getCommandList(irHandle, orvCommand);
}




int DSSRWControlModelIterator::get_Type(EnumDSSIteratorType * pType)
{
    
    if (!pType) return E_POINTER;
    
    *pType=DssIteratorControl;
    
    return S_OK;
}

int DSSRWControlModelIterator::FindUnit(GUID UnitID, Int32 * pUnit)
{
    return mpDataModel->FindUnit(UnitID);
}

int DSSRWControlModelIterator::get_ElementIndexByRowNum(Int32 iRow, Int32 iUnit, Int32 * pElementIndex)

{
    
    *pElementIndex = iRow;
    
    return S_OK;
    
}


int DSSRWControlModelIterator::get_Tag(Int32 * pTag)

{
    
    if (mnElements<=0 || mCursor >= mnElements)
        
        return E_UNEXPECTED;
    
    *pTag = mvElements[mCursor];
    
    return S_OK;
    
}


int DSSRWControlModelIterator::getMetricValueMin(const CComVariant ** oppVal)
{
	if (!mpNodeModel || !oppVal)
		return E_FAIL;
    
    DSSRWControlModel *lpControlModel=dynamic_cast<DSSRWControlModel*>(mpNodeModel);
    
	
	*oppVal = lpControlModel->getMetricRange(0);
	CHECK_PTR(*oppVal);
	return S_OK;
}

int DSSRWControlModelIterator::getMetricValueMax(const CComVariant ** oppVal)
{
	if (!mpNodeModel || !oppVal)
		return E_FAIL;
    
    DSSRWControlModel *lpControlModel=dynamic_cast<DSSRWControlModel*>(mpNodeModel);
    
	
	*oppVal = lpControlModel->getMetricRange(1);
	CHECK_PTR(*oppVal);
	return S_OK;
}

int DSSRWControlModelIterator::getMetricValueCount(const CComVariant ** oppVal)
{
	if (!mpNodeModel || !oppVal)
		return E_FAIL;
    
    DSSRWControlModel *lpControlModel=dynamic_cast<DSSRWControlModel*>(mpNodeModel);
    
	
	*oppVal = lpControlModel->getMetricRange(2);
	CHECK_PTR(*oppVal);
	return S_OK;
}

ControlCell DSSRWControlModelIterator::getControlCell()
{
    return ControlCell(mCursor,this);
}


ControlCell DSSRWControlModelIterator::getControlCellAt(int index)
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

//hyan,2012/07/12,since now we don't need to support remove null element feature
//comment these APIs

//ControlCell DSSRWControlModelIterator::getNonNullControlCellAt(int index)
//{
//    //index range from [0, N), where N is the value returned by CountRows
//    
//    if(mvNonNullIndex.size()==0)
//    {
//        if (mbShowAll)
//        {
//            if (index > 0)
//                return ControlCell(index - 1, this);
//            else
//                return ControlCell(-1, this);
//        }
//        else
//        {
//            return ControlCell(index, this);
//        }
//    }
//    else
//    {
//    
//        if (mbShowAll)
//        {
//            if (index > 0)
//                return ControlCell(mvNonNullIndex[index - 1], this);
//            else
//                return ControlCell(-1, this);
//        }
//        else
//        {
//            return ControlCell(mvNonNullIndex[index], this);
//        }
//    }
//    
//
//}
//
//int DSSRWControlModelIterator::CountNonNullRows()
//{
//    
//    //hyan,2012/07/06,temporary implement
//    
//    if(mvNonNullIndex.size()==0)
//    {
//        if(mbShowAll)
//            return mvElements.size()+1;
//        else
//            return mvElements.size();
//    }
//    else
//    {
//        if(mbShowAll)
//            return mvNonNullIndex.size()+1;
//        else
//            return mvNonNullIndex.size();
//    }
//}
//
//int DSSRWControlModelIterator::hGenerateNonNullIndexArray()
//{
//    int hr=S_OK;
//    
//    DSSObjectContext *lpObjContext=mpDataModel->getObjectContext();//this will be a member
//    
//    GUID lControlObjectID = mpControl->getSourceID();
//    
//    //junwang, 02/08/2012, 547222
//    if(MBase::IsGUIDNULL(lControlObjectID))
//        return S_OK;
//    
//    
//    
//    DSSObjectInfo *lpObjectInfo=lpObjContext->FindObjectH(lControlObjectID);
//    
//    GUID lGUID=lpObjectInfo->getID();
//    
//    
//    
//    DSSTabularAttribute *lpTabularAttribute=NULL;
//    
//    if(mpTabularAttribute)
//        lpTabularAttribute=mpTabularAttribute;
//    else
//        lpTabularAttribute=mpDataModel->GetTabularAttributeByGUID(lGUID);
//    
//    
//    DSSGlobalLookUpTable* lpLookUp = dynamic_cast<DSSGlobalLookUpTable*>(lpTabularAttribute);
//    
//    if(lpLookUp==NULL)
//        return S_OK;
//
//    // set<int> lsetForms;
//    vector<int> lvUnionForms;
//    
//    hr=lpLookUp->GetDisplayFormUnionForControl(lvUnionForms);
//    
//    CHECK_HR;
//    
//    bool isNull=false;
//    
//    mvNonNullIndex.clear();
//    
//    int lnsize=mvElements.size();
//    
//    for (int i=0; i<lnsize; i++)
//    {
//        isNull=false;
//        
//        int ei = mvElements[i];
//        
//        if(ei != DSSXTabTotalKey && ei!=DSSXTabAnyKey)
//        {
//            for(int j=0;j<lvUnionForms.size();j++)
//            {
//                int lIndex=lvUnionForms[j];
//                
//                
//                //TQMS 566888,567149
//                //hyan,2012/03/20
//                //The lIndex is just the BaseFormID
//                int lBaseFormID=lIndex;
//                
//                //TODO[opt]: cache data form
//                int lColumnID = 0;
//                int hr = lpLookUp->FindFormWithLocale(0, lBaseFormID, 0, lColumnID);
//                
//                if(hr!=S_OK)
//                    continue;            
//                
//                //we can use the ColumnID as the index to get FormType
//                int lFormType=lpLookUp->getBaseFormType(lColumnID);
//                
//                //CHECK_HR;
//                
//                DSSDataColumn *lpColumn = NULL;
//                hr = lpLookUp->Item(lColumnID, &lpColumn);
//                CHECK_HR;
//                
//                //how to get offset?
//                int lOffset=0;
//                
//                const void* lCellData=NULL;
//                int lCellDataSize=0;
//                EnumDSSDataType lCellDataType;
//                DSSDataFlag lCellDataFlag;
//                
//                lpColumn->getData(ei, (const char**)(&lCellData), lCellDataSize, lCellDataType,lCellDataFlag);
//                
//                
//                if(lCellDataFlag==1)
//                {
//                    isNull=true;
//                    break;
//                }
//            }
//        }
//        
//        if(false==isNull)
//        {
//            mvNonNullIndex.push_back(i);
//        }
//    }
//    
//    return S_OK;
//}




int DSSRWControlModelIterator::FindElement(DSSBaseElementProxy* ipElement)
{
	int lnElements = mvElements.size();
    
    DSSObjectContext *lpObjContext=mpDataModel->getObjectContext();
    
    GUID lControlObjectID = mpControl->getSourceID();

    DSSObjectInfo *lpObjectInfo=lpObjContext->FindObjectH(lControlObjectID);
    
    if(lpObjectInfo && mpControl->getType() != DssRWControlTypeMetricList)  //746427, check if it's metric selector
    {
    
        GUID lGUID=lpObjectInfo->getID();
        
        EnumDSSObjectType lType=lpObjectInfo->getType();
        
        
        //EnumDSSElementType lElementType = ipElement->getElementType();

        int lLocalIndex = -1;
        int hr = S_OK;
        hr = mpDataModel->LookForElement(ipElement, lGUID, lType, mvElements, &lLocalIndex);
        
        if(hr == S_OK)
        {
            return mbShowAll ? lLocalIndex+1:lLocalIndex;
        }
    }
    //TQMS 586406, for metric selector, the ObjectInfo will be NULL
    else
    {
        for (int i = 0; i < lnElements; i++)
        {
            int ei = mvElements[i];
            
            if(ei==DSSXTAB_TOTAL_KEY)
            {
                if(ipElement->getElementType()==DssElementSubtotal)
                    return mbShowAll ? i + 1 : i;
                else
                    continue;
            }
            
            if(mbCache)
            {
                map<int,DSSBaseElementProxy*>::iterator it=mpElementProxyMap->find(ei);
                
                if(it!=mpElementProxyMap->end())
                {
                    DSSBaseElementProxy* lpElement=it->second;
                    
                    if(lpElement&&lpElement->IsEqual(ipElement))
                        return mbShowAll ? i + 1 : i;
                }
            }
            else
            {
                //   continue;
                //TODO:[opt], don't compare element
                if (mpElements[i])
                {
                    if (mpElements[i]->IsEqual(ipElement))
                        return mbShowAll ? i + 1 : i;
                }
            }
        }
    }
    
    return -1;
}


bool DSSRWControlModelIterator::hCheckHasParentSelector()
{
    if(!mpNode)
        return false;
    
    DSSRWSectionNode *lpParentSelectorSectionNode=NULL;
    
    DSSRWNode* lpParentNode=mpNode->getParentNode();
    
    while(lpParentNode)
    {
        DSSRWSectionNode *lpSectionNode=dynamic_cast<DSSRWSectionNode*>(lpParentNode);
        
        if(lpSectionNode)
        {
            EnumDSSRWSectionType lSectionType=lpSectionNode->getSectionType();
            
            //up to the content root node
            if(lSectionType==DssRWSectionContent)
                break;
            
            if(lSectionType==DssRWSectionControlGroupBySection || lSectionType==DssRWSectionUnitCondition)
            {
                lpParentSelectorSectionNode=lpSectionNode;
                break;
            }
        }
        
        lpParentNode=lpParentNode->getParentNode();
    }
    
    if(lpParentSelectorSectionNode)
        return true;
    
    return false;
}



bool DSSRWControlModelIterator::hCheckParentSelectorEmptyChoose()
{
    if(!mpNode)
        return false;
    
    DSSRWSectionNode *lpParentSelectorSectionNode=NULL;
    
    DSSRWNode* lpParentNode=mpNode->getParentNode();
    
    while(lpParentNode)
    {
        DSSRWSectionNode *lpSectionNode=dynamic_cast<DSSRWSectionNode*>(lpParentNode);
        
        if(lpSectionNode)
        {
            EnumDSSRWSectionType lSectionType=lpSectionNode->getSectionType();
            
            //up to the content root node
            if(lSectionType==DssRWSectionContent)
                break;
            
            if(lSectionType==DssRWSectionControlGroupBySection || lSectionType==DssRWSectionUnitCondition)
            {
                lpParentSelectorSectionNode=lpSectionNode;
                break;
            }
        }
        
        lpParentNode=lpParentNode->getParentNode();
    }
    
    
    if(lpParentSelectorSectionNode)
    {
        DSSGroupByUnits* lpGroupByUnits = lpParentSelectorSectionNode->getGroupByUnits();
        
        DSSGroupByUnit* lpUnit = lpGroupByUnits->Item(0);
        
        EnumDSSCurrentElementStatus lStatus = DssCurrentElementReserved;
        
        lStatus = lpUnit->getCurrentElementsStatus();
        
        if(lStatus==DssCurrentElementUnset)
            return false;
        
        
        //hyan,2012/04/28
        //we need to check Metric Condition Selector
        GUID lGUID=lpUnit->getObjectID();
        
        int lGlobalUnitID = mpDataModel->FindUnit(lGUID);
        
        if (-1==lGlobalUnitID)// it's an metric condition selector
        { 
            return false;
        }

        
        
        
        bool lbInclude=true;
        
        DSSBaseElementsProxy *lpElements=NULL;
        
        DSSCExpression* lpExpression = lpUnit->getExpression();
        
        if(lpExpression)
        {
            DSSExpNode* lpExpRoot = lpExpression->Root();
            
            if(lpExpRoot!=NULL&&lpExpRoot->getType() == DssNodeOperator
               &&(static_cast<DSSExpNodeOperator*>(lpExpRoot)->getOperatorType()==DssFunctionNotIn))
            {
                lbInclude=false;
                
                DSSBaseElementsProxy *lpExcludedElements = static_cast<DSSExpNodeElementsObject*> (lpExpRoot->getChildNode()->getLastSibling())->getElements();
                
                lpElements = lpExcludedElements;
            }
            //hack code, for test
            else
            {
                lpElements = lpUnit->getCurrentElements();
            }
        }
        else
        {
            lpElements = lpUnit->getCurrentElements();
        }
        
        int lCountElements = 0;
        
        lCountElements=lpElements->Count();
        
        if(lbInclude&&lCountElements==0)
            return true;
    }
    
    
    return false;
}


void DSSRWControlModelIterator::setElementAt(int iIndex, DSSBaseElementProxy* ipElement) {
    if (iIndex >= 0 && iIndex < (int) mpElements.size()) {
        delete mpElements[iIndex];
        mpElements[iIndex] = ipElement;
    }
}

ControlElementInfo* DSSRWControlModelIterator::getElementInfoAt(int iRow)
{
    if (iRow >= mnElements || !mpNodeModel) {
        return NULL;
    }
    if (mvElementInfo.size() < mnElements) {
        mvElementInfo.resize(mnElements);
        memset(&mvElementInfo[0], NULL, mnElements);
    }
    
    if (mvElementInfo[iRow]) {
        return mvElementInfo[iRow];
    }
    
    int lParent = -1;
    int lLevel = 0;
    bool lIsExpand = false;
    bool lHasChildren = false;
    int hr = (static_cast<DSSRWControlModel*>(mpNodeModel))->getRelationInfo(iRow, &lParent, &lLevel, &lIsExpand, &lHasChildren);
    if (hr != S_OK) {
        return NULL;
    }
    ControlElementInfo* lInfo = new ControlElementInfo;
    lInfo->parentIndex = lParent;
    lInfo->level = lLevel;
    lInfo->isExpand = lIsExpand;
    lInfo->hasChildren = lHasChildren;
    mvElementInfo[iRow] = lInfo;
    return lInfo;
}

int DSSRWControlModelIterator::hGenerateLevelInfo()
{
    if (!mpControl || !mpNodeModel) {
        return -1;
    }
    
    //only for recursive attribute selector now.
    if (mpControl->getType() != DssRWControlTypeElementList || !isRecursiveUnit()) {
        return 0;
    }
    
    for (int i=0; i < mnElements; i++) {
        ControlElementInfo* lpInfo = getElementInfoAt(i);
        if (lpInfo) {
            map<int, vector<int>* >::iterator lIt = mvLevelMap.find(lpInfo->level);
            if (lIt == mvLevelMap.end()) {
                vector<int> *lListForLevel = new vector<int>;
                lListForLevel->push_back(i);
                mvLevelMap.insert(map<int,vector<int>* >::value_type(lpInfo->level,lListForLevel));
            }
            else
            {
                lIt->second->push_back(i);
            }
        }
        else{
            return -1;
        }
    }
    
    return 0;
}

int DSSRWControlModelIterator::hClearCacheValues()
{
    map<int,vector<int>*>::iterator lIt=mvLevelMap.begin();
    
    while(lIt!=mvLevelMap.end())
    {
        delete lIt->second;
        
        lIt->second=NULL;
        
        lIt++;
    }
    
    mvLevelMap.clear();
}

int DSSRWControlModelIterator::getLevelInfo(int iLevel, int& oCount, int& oSelected, string& oName)
{
    map<int, vector<int>* >::iterator lIt = mvLevelMap.find(iLevel);
    if (lIt != mvLevelMap.end()) {
        vector<int>* lpList = lIt->second;
        if (!lpList) {
            return S_FALSE;
        }
        int lnSelected = 0;
        for (int i=0; i < lpList->size(); i++) {
            if (IsSelectedAt((*lpList)[i])) {
                lnSelected++;
            }
        }
        oCount = lpList->size();
        oSelected = lnSelected;
        
        //TODO: level name
        std::stringstream lNameStream;
        lNameStream<<"Level "<<(iLevel+1);
        oName = lNameStream.str();
        return S_OK;
    }
    
    return S_FALSE;
}

int DSSRWControlModelIterator::getTotalLevels()
{
    return mvLevelMap.size();
}

bool DSSRWControlModelIterator::isRecursiveUnit()
{
    if (mpNodeModel)
    {
        return (static_cast<DSSRWControlModel*>(mpNodeModel))->isRecursiveUnit();
    }
    return false;
}

DSSBaseElementProxy* DSSRWControlModelIterator::getElementAt(int index)
{
    if (index == -1)
		return mpAllElement;
    
    
    if(mbCache)
    {
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
    }//end if(mbCache)
    else
    {
        if(index>=0&&index<mpElements.size())
        {
            return mpElements[index];
        }
    }
        
    return NULL;
}


int DSSRWControlModelIterator::hGenerateSingleElement(int index,DSSBaseElementProxy** ippElementProxy)
{
    if(!ippElementProxy)
        return E_INVALIDARG;
    
    
    int hr=S_OK;
    
    DSSBaseElementProxy *lpElement = NULL;

    
    DSSObjectContext *lpObjContext=mpDataModel->getObjectContext();//this will be a member
    
    GUID lControlObjectID = mpControl->getSourceID();
    
    //junwang, 02/08/2012, 547222
    if(MBase::IsGUIDNULL(lControlObjectID))
        return S_OK;
    
    DSSObjectInfo *lpObjectInfo=lpObjContext->FindObjectH(lControlObjectID);
    
    GUID lGUID=lpObjectInfo->getID();
    
    EnumDSSObjectType lType=lpObjectInfo->getType();
    
    switch (lType) {
        case DssTypeAttribute:
        {
            
            int ei = mvElements[index];

            if(ei==DSSXTAB_TOTAL_KEY)
            {
                //subtotal index for total will be 1,hyan, 11/12/2012
                lpElement=new DSSSubtotalElementProxy(DSSRWNodeModelIterator::mpObjectContext,lGUID,1);
            }
            else
            {
                DSSTabularAttribute *lpTabularAttribute=NULL;
                
                if(mpTabularAttribute)
                    lpTabularAttribute=mpTabularAttribute;
                else
                    lpTabularAttribute=mpDataModel->GetTabularAttributeByGUID(lGUID);
                
                if(NULL==lpTabularAttribute)
                {
                    lpElement = new DSSNullElementProxy(DSSRWNodeModelIterator::mpObjectContext,lGUID);
                }
                else
                {
                   
                    DSSTabularUnit* lpUnit = NULL;
                    
                    DSSGlobalLookUpTable* lpLookUp = dynamic_cast<DSSGlobalLookUpTable*>(lpTabularAttribute);
                    
                    bool lfDE=false;
                    int lLocalKey=-1;
                    //DE
                    lpLookUp->GetGlobalUnit(ei, &lpUnit, &lLocalKey, &lfDE);
                    
                    //junwang, 02/21/2012, special handle for NULL element
                    //if(lpUnit==NULL)
                    //TQMS653125 hyan, 09/12/2012
                    //for Join NULL element, the lpUnit won't be NULL
                    int lMaxKey = -1;
                    hr = lpLookUp->GetMaxGlobalKey(&lMaxKey);
                    CHECK_HR;
                
                    if (ei==lMaxKey+1)
                    {
                        lpElement = new DSSNullElementProxy(DSSRWNodeModelIterator::mpObjectContext,lGUID);
                    }
                    else                    
                    {
                        if(lpUnit)
                        {
                            hr = lpUnit->createElement(lLocalKey, DSSRWNodeModelIterator::mpObjectContext, &lpElement,ei,mGlobalLookupVer);
                            
                            CHECK_HR; 
                        }
                    }
                }
            }
        }
            break;
            
        case DssTypeConsolidation:
        {
            int ei = mvElements[index];
        
            //special handle for total
            if(ei==DSSXTAB_TOTAL_KEY)
            {
                lpElement=new DSSSubtotalElementProxy(DSSRWNodeModelIterator::mpObjectContext,lGUID,1);
            }
            else
            {
                //junwang, 2012/02/21, null element
                bool lfNULL = false;
                hr = mpDataModel->IsCompoundNull(lGUID, ei, &lfNULL);
                CHECK_HR;
                if(lfNULL)
                {
                    lpElement = new DSSNullElementProxy(DSSRWNodeModelIterator::mpObjectContext,lGUID);
                }
                else
                {
                    DSSTabularUnit *lpTabularUnit=NULL;
                    int lLocalKey=-1;
                    hr=mpDataModel->GetTabularUnit(lGUID, ei, &lpTabularUnit,&lLocalKey);
                    CHECK_HR;
                    
                    if(lpTabularUnit)
                    {
                        DSSTabularConsolidation *lpTabularConsolidation=static_cast<DSSTabularConsolidation*>(lpTabularUnit);
                        
                        if(lpTabularConsolidation)
                        {
                            hr = lpTabularConsolidation->createElement(lLocalKey, DSSRWNodeModelIterator::mpObjectContext, &lpElement,ei,mGlobalLookupVer);
                            CHECK_PTR(lpElement);
                        }
                    }
                }
            }
        }
            break;
            
        case DssTypeFilter:
        {
            int ei = mvElements[index];
            
            
            //special handle for total
            if(ei==DSSXTAB_TOTAL_KEY)
            {
                lpElement=new DSSSubtotalElementProxy(DSSRWNodeModelIterator::mpObjectContext,lGUID,1);
                
            }
            else
            {
                //junwang, 2012/02/21, null element
                bool lfNULL = false;
                hr = mpDataModel->IsCompoundNull(lGUID, ei, &lfNULL);
                CHECK_HR;
                if(lfNULL)
                {
                    lpElement = new DSSNullElementProxy(DSSRWNodeModelIterator::mpObjectContext,lGUID);
                }
                else
                {
                    DSSTabularUnit *lpTabularUnit=NULL;
                    int lLocalKey=-1;
                    hr=mpDataModel->GetTabularUnit(lGUID, ei, &lpTabularUnit,&lLocalKey);
                    CHECK_HR;
                    
                    if(lpTabularUnit)
                    {
                        DSSTabularCustomGroup *lpTabularCustomGroup=dynamic_cast<DSSTabularCustomGroup*>(lpTabularUnit);
                        
                        if(lpTabularCustomGroup)
                        {
                            hr = lpTabularCustomGroup->createElement(lLocalKey, DSSRWNodeModelIterator::mpObjectContext, &lpElement,ei,mGlobalLookupVer);
                            CHECK_PTR(lpElement);
                        }
                    }
                }
            }
        }
            break;
            
        default:
            break;
    }
    
    
    //if there some errors, to avoid crash, we create a null element
    if(lpElement==NULL)
        lpElement = new DSSNullElementProxy(DSSRWNodeModelIterator::mpObjectContext,lGUID);
    
    *ippElementProxy=lpElement;
    
    return S_OK;

}

int DSSRWControlModelIterator::getElementDisplayText(DSSBaseElementProxy* ipElement, std::string &orText)
{
    orText = "";
    int index = FindElement(ipElement);
    if(index < 0)
        return S_FALSE;
    
    if(mbShowAll)
        index--;
    
    //xiazhou, 2013-07-16. Fix display text error. Should use index as getElementText's input.
    //int elementID = mvElements[index];
    int elementID = index;
    
    if (elementID != -1)
	{
		return getElementText(elementID, orText);
	}
	else
	{	//-1 means the "ALL" element
		orText = getControl()->getAllAlias();
		return S_OK;
	}
}
int DSSRWControlModelIterator::AnalyzeTransactionThreshold(DSSThreshold* ipThreshold, bool* opResult)
{
    int hr = S_OK;
    if(ipThreshold && mpDataModel)
    {
        hr = mpDataModel->AnalyzeTransactionThreshold(ipThreshold, opResult);
        CHECK_HR;
    }
    return hr;
}
