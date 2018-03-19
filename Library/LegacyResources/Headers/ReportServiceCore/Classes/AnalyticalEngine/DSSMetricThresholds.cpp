//
//  DSSMetricThresholds.cpp
//  ReportServiceCore
//
//  Created by Zhuo Yang on 11/2/12.
//  Copyright (c) 2012 Microstrategy. All rights reserved.
//

#include <iostream>
#include "DSSMetricThresholds.h"
#include "DSSFunctionServer.h"
#include "DSSTemplate.h"
#include "DSSCExpression.h"
#include "DSSExpNode.h"
#import "CalculationEngine/DSSCalculationPlan.h"
#include "DSSStrongPtr.h"
#include "DSSCubeDataSlice.h"
#include "DSSDataModelXTabImpl.h"
#include "DSSCube.h"
#include "DSSTabularIndex.h"
#include "DSSXTabView.h"
#include "DSSTabularData.h"
#include <algorithm>
#include "DSSConstantSlice.h" // jimzhang, Jun/17, temp code
#include "DSSDataColumn.h" //jimzhang, Jun/17, temp code
#include "DSSRWGridModel.h"
#include "DSSFunctionObject.h"
#include "DSSTabularIndexPool.h"

//#define USE_DFC_FILTER
#ifdef USE_DFC_FILTER
    #import "CDSSFilterExprEvalCube.h"
    #import "SimpleBuffer.h"
    #import "DFCWNode.h"
#else
    #include "DSSFilterExprEvalCube.h"
#endif
DSSMetricThresholds::DSSMetricThresholds(MCESchema *ipSchema): mpSchema(ipSchema)
{
}

DSSMetricThresholds::~DSSMetricThresholds()
{
    
}

int DSSMetricThresholds::calculateThreshold(IDSSTemplateModel *ipModel, DSSTabularData *ipTabularData, DSSCalculationPlan* ipPlan, DSSTemplate* ipTemplate, DSSFunctionServer *ipFunctionServer)
{
    int hr = S_OK;
    
    assert(ipModel);
    mpModel = ipModel;
    
    if(!ipTabularData)return hr;
    mpTabularData = ipTabularData;
    
    /*
    DSSXTabView* lpView = ipTabularData->getXTabView();
    CHECK_PTR(lpView);
    */
   
    DSSCube* lpCube = NULL;
    //hr = lpView->GetCube(&lpCube);
    lpCube = mpModel->getCube();
    CHECK_HR_PTR(lpCube);
    
    mpTemplate = ipTemplate;
    assert(mpTemplate);
    
    CHECK_PTR(ipPlan);
    mLevelCount = ipPlan->getNumLevels();
    
    DSSTemplateMetrics *lpTemplateMetrics = mpTemplate->getTemplateMetrics();
    if(!lpTemplateMetrics)
    {
        mMetricCount = 0;
    }
    else
    {
        mMetricCount = lpTemplateMetrics->Count();
    }
    
    mvThresholdAttrs.clear();
    if (mpTemplate->hasAttributeThreshold())
    {
        // Check if there's any attribute threshold in row(1) and column(2)
        int lnAxis = 2;
        
        for (Int32 i = 1; i <= lnAxis; i++)
        {
            DSSAxis* lAxis = mpTemplate->getAxis(i);
            int lnUnit = lAxis->Count();
            for (int j = 0; j < lnUnit; j++)
            {
                DSSTemplateUnit* lpTemplateUnit = lAxis->Item(j);
                if (lpTemplateUnit && (lpTemplateUnit->getType() == DssTemplateAttribute))
                {
                    DSSTemplateAttribute* lpTemplateAttribute = static_cast<DSSTemplateAttribute*>(lpTemplateUnit);
                    
                    DSSThresholds *lpThresholds = lpTemplateAttribute->getThresholds();
                    
                    if (lpThresholds && lpThresholds->Count() > 0)
                    {
                        mvThresholdAttrs.push_back(lpTemplateAttribute->getID());
                    }
                }
            }
        }
    }
    
    hr = hEvaluateThresholds4Template(lpTemplateMetrics, lpCube, ipPlan, ipFunctionServer);
    CHECK_HR;
  
    if (hr != S_OK)
    {
//        CDSSFilterExprEvalCube lDummy(0,0,0,0,0,0,0,0,0,0,0,0);
//        lDummy.ProcessFilter(0, 0);
    }
    
    return hr;
}

int DSSMetricThresholds::hEvaluateThresholds4Template(DSSTemplateMetrics *ipTemplateMetrics, DSSCube *ipCube, 
                                                      DSSCalculationPlan *ipCalculationPlan, DSSFunctionServer *ipFunctionServer)
{
    int hr = S_OK;
    
    DSSRWGridModel* lpGridModel = dynamic_cast<DSSRWGridModel*>(mpModel);
    if(!lpGridModel)//931 i-server
    {
        //fali, 962622, set tabularData for ipCube
        if (!ipCube->getTabularData())
        {
            DSSDataModelXTabImpl* lpXTabModel = dynamic_cast<DSSDataModelXTabImpl*>(mpModel);
            CHECK_PTR(lpXTabModel);
            
            DSSTabularData* lpTabularData = lpXTabModel->getTabularData();
            CHECK_PTR(lpTabularData);
            
            DSSTabularIndexPool* lpIndexPool = NULL;
            hr = ipCube->getIndexPool(&lpIndexPool);
            CHECK_HR_PTR(lpIndexPool);
            
            hr = lpIndexPool->Init(lpXTabModel);
            CHECK_HR;
            
            hr = lpTabularData->setIndexPool(lpIndexPool);
            CHECK_HR;
            
            ipCube->setTabularData(lpTabularData);
            
        }
        int lCurLevel = 0, lMetricIndex = 0, lCurEvent = 0;
        lCurEvent = ipCalculationPlan->getNumEvents();
        
        for(lCurLevel = 0; lCurLevel < mLevelCount; ++lCurLevel)
        {
            for(lMetricIndex = 0; lMetricIndex < mMetricCount; ++lMetricIndex)
            {
                DSSTemplateMetric *lpTemplateMetric = ipTemplateMetrics->Item(lMetricIndex);
                DSSThresholds *lpThresholds = lpTemplateMetric->getThresholds();
                if(!lpThresholds)
                    continue;
                
                int lNumThresholds = lpThresholds->Count();
                if(lNumThresholds <= 0)
                    continue;
                
                int lResultSliceID = -1;
                hr = hEvaluateThreshods(lpThresholds,
                                        ipCube,
                                        lCurLevel,
                                        lCurEvent,
                                        ipCalculationPlan,
                                        ipFunctionServer,
                                        &lResultSliceID);
                CHECK_HR;
                
                if(lResultSliceID != -1)
                {
                    GUID lID = lpTemplateMetric->getID();
                    int lMetricIndexInCalPlan = ipCalculationPlan->getMetricIndex(lID);
                    //hr = ipCalculationPlan->setThresholdSliceIDForNewRWD(lCurLevel, lCurEvent, lMetricIndex, lResultSliceID);
                    bool lfCalculated = false;
                    int lDataSliceID = -1;
                    hr = ipCalculationPlan->getMetricSliceID(lCurLevel, lCurEvent-1, lMetricIndexInCalPlan, &lfCalculated, &lDataSliceID);
                    CHECK_HR;
                    
                    if(lfCalculated)
                    {
                        DSSCubeDataSlice *lpDataSlice = ipCube->getSliceByID(lDataSliceID);
                        DSSCubeDataSlice *lpThreshholdSlice = ipCube->getSliceByID(lResultSliceID);
                        if(lpDataSlice)
                        {
                            hr = lpDataSlice->setThresholdSlice(lpThreshholdSlice);
                            hr = ipCalculationPlan->setThresholdSliceID(lCurLevel, lMetricIndexInCalPlan,lResultSliceID);
                        }
                    }
                    
                    CHECK_HR;
                }
            }
        }
        return S_OK;
    }
    bool lbIsSmartClient = lpGridModel->IsEvaluatedOnClient();
    int lCurLevel = 0, lMetricIndex = 0;
    vector<int> lvOriginalID;
	vector<int> lvUpdatedID;
    for(lCurLevel = 0; lCurLevel < mLevelCount; ++lCurLevel)
    {
        for(lMetricIndex = 0; lMetricIndex < mMetricCount; ++lMetricIndex)
        {
            DSSTemplateMetric *lpTemplateMetric = ipTemplateMetrics->Item(lMetricIndex);
            DSSThresholds *lpThresholds = lpTemplateMetric->getThresholds();
            if(!lpThresholds)
                continue;
            
            int lNumThresholds = lpThresholds->Count();
            if(lNumThresholds <= 0)
                continue;
            
            int lResultSliceID = -1;
            hr = hEvaluateThreshods(lpThresholds, 
                                    ipCube, 
                                    lCurLevel,
                                    ipCalculationPlan->getNumEvents(),
                                    ipCalculationPlan,
                                    ipFunctionServer,
                                    &lResultSliceID);
            CHECK_HR;
            
            if(lResultSliceID != -1)
            {
                GUID lID = lpTemplateMetric->getID();
                int lMetricIndexInCalPlan = ipCalculationPlan->getMetricIndex(lID);
                //hr = ipCalculationPlan->setThresholdSliceIDForNewRWD(lCurLevel, lCurEvent, lMetricIndex, lResultSliceID);
                bool lfCalculated = false;
                int lDataSliceID = -1;
                hr = ipCalculationPlan->getMetricSliceID(lCurLevel, ipCalculationPlan->getNumEvents() - 1, lMetricIndexInCalPlan, &lfCalculated, &lDataSliceID);
                CHECK_HR;
                
                if(lfCalculated)
                {
                    DSSCubeDataSlice *lpDataSlice = ipCube->getSliceByID(lDataSliceID);
                    DSSCubeDataSlice *lpThreshholdSlice = ipCube->getSliceByID(lResultSliceID);
                    hr = lpDataSlice->setThresholdSlice(lpThreshholdSlice);
                    if (!lbIsSmartClient)
                    {
                        int lOrginalThresholdSliceID;
                        hr = ipCalculationPlan->getMetricThresholdSliceID(lCurLevel, lID, &lOrginalThresholdSliceID);
                        CHECK_HR;
                        if (lOrginalThresholdSliceID >= 0)
                        {
                            lvOriginalID.push_back(lOrginalThresholdSliceID);
                            lvUpdatedID.push_back(lResultSliceID);
                        }
                    }
                    else
                    {
                        hr = ipCalculationPlan->setThresholdSliceID(lCurLevel, lMetricIndexInCalPlan,lResultSliceID);
                    }
                }
                
                CHECK_HR;
            }
        }
        
        // calculate the attribute threshold
        size_t lnThresholdAttribute = mvThresholdAttrs.size();
        for (int lAttriIndex = 0; lAttriIndex < lnThresholdAttribute; ++lAttriIndex)
        {
            DSSTemplateUnit *lpTemplateUnit = mpTemplate->searchTemplateUnit(mvThresholdAttrs[lAttriIndex], DssTemplateAttribute);
            if (!lpTemplateUnit || (lpTemplateUnit->getType() != DssTemplateAttribute))
            {
                continue;
            }
            DSSTemplateAttribute* lpTemplateAttribute = static_cast<DSSTemplateAttribute*>(lpTemplateUnit);
            DSSThresholds *lpThresholds = lpTemplateAttribute->getThresholds();
            if (!lpThresholds || lpThresholds->Count() <= 0)
                continue;
            
            int lResultSliceID = -1;
            hr = hEvaluateThreshods(lpThresholds,
                                    ipCube,
                                    lCurLevel,
                                    ipCalculationPlan->getNumEvents(),
                                    ipCalculationPlan,
                                    ipFunctionServer,
                                    &lResultSliceID);
            CHECK_HR;
            
            if (lResultSliceID != -1)
            {
                const GUID& lID = lpTemplateAttribute->getID();
                int lDataSliceID = -1;
                hr = ipCalculationPlan->getAttriThresholdSliceID(lCurLevel, lID, &lDataSliceID, NULL);
                CHECK_HR;
                
                // DSSCubeDataSlice *lpDataSlice = ipCube->getSliceByID(lDataSliceID);
                DSSCubeDataSlice *lpThreshholdSlice = ipCube->getSliceByID(lResultSliceID);
                hr = lpThreshholdSlice->setThresholdSlice(lpThreshholdSlice);
                if (!lbIsSmartClient)
                {
                    int lOrginalThresholdSliceID = lDataSliceID;
                    if (lOrginalThresholdSliceID >= 0)
                    {
                        lvOriginalID.push_back(lOrginalThresholdSliceID);
                        lvUpdatedID.push_back(lResultSliceID);
                    }
                }
                else
                {
                    hr = ipCalculationPlan->setAttriThresholdSliceID(lCurLevel, lID, lResultSliceID);
                }
                
                CHECK_HR;
            }
        }
    }
    
    if (!lvOriginalID.empty())
    {
        ipCube->UpdateCalculateResult(lvOriginalID, lvUpdatedID);
    }
    
    return hr;
}

int DSSMetricThresholds::hEvaluateThreshods(DSSThresholds *ipThresholds, DSSCube *ipCube, int iCurLevel, int iCurEvent,
                                            DSSCalculationPlan *ipCalculationPlan, DSSFunctionServer *ipFunctionServer,
                                            int *oSliceID)
{
    int hr = S_OK;
    
    int lNumThresholds = ipThresholds->Count();
    int i = 0;
    
    int *lpThresholdSliceID = new int[lNumThresholds];
    CHECK_NEWED_PTR(lpThresholdSliceID);
    
    // jimzhang, Nov/30/2012, TQMS 689420, initialize threshold sliceID to -1, since following logic will use -1 to check if this slice is skipped.
    for (int i = 0; i < lNumThresholds; ++i)
        lpThresholdSliceID[i] = -1;
    
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lpThresholdSliceIDPtr(lpThresholdSliceID);
    
    vector<int> lvTargetUnits;
    int lnTargetUnits = 0;
    int *lpTargetUnits = NULL;
    
    if(iCurLevel > 0)
    {
        //hr = DSSAEHelper::ConvertTargetLevel(iCurLevel, mpTemplate, mpModel->getCube()->getTabularData(), ipCalculationPlan, false, &lvTargetUnits, iCurEvent);
        //if(hr != S_OK)
        //    return S_OK;
        lvTargetUnits = ipCalculationPlan->getSubtotalLevel(iCurLevel);
        lnTargetUnits = lvTargetUnits.size();
        lpTargetUnits = (lnTargetUnits>0) ? &lvTargetUnits[0] : NULL;
        
    }
    
    sort(lpTargetUnits, lpTargetUnits + lnTargetUnits);
    
    bool lfSkip = false;
    bool ifHasTrue = false;
    
    EventContext lContext;
    lContext.mLevel = iCurLevel;
    lContext.mEvent = iCurEvent;
    lContext.mpCalculationPlan = ipCalculationPlan;
    lContext.mpEvent = NULL;
    lContext.mpCube = ipCube;
    lContext.mpDataModel = mpModel;
    lContext.mpObjectContext = mpModel->getObjectContext();
    lContext.mpSchema = mpSchema;
    lContext.mpTabularData = mpTabularData;
    lContext.mpTemplate = mpTemplate;
    for(i = 0; i < lNumThresholds; ++i)
    {
        DSSThreshold *lpThreshold = ipThresholds->Item(i);
        if(lpThreshold == NULL)
        {
            lfSkip = true;
            continue;
        }
        
        EnumDSSThresholdScope lScope = (EnumDSSThresholdScope)(lpThreshold->getScope());
        if((lScope == DssThresholdScopeReserved)
           || (lScope == DssThresholdScopeMetricOnly && iCurLevel > 0)
           || (lScope == DssThresholdScopeSubtotalOnly && iCurLevel == 0))
        {
            lfSkip = true;
            continue;
        }
        
        if(iCurLevel > 0)
        {
            bool lNeedCauculate = false;
            //zhyang, need implement this function
            //hr = hCompareLevel();
            lNeedCauculate = true;
            if(!lNeedCauculate)
            {
                lfSkip = true;
                continue;
            }
        }
        
        int lSliceID = -1;
        hr = hEvaluateThresholdCondition(lpThreshold,
                                         ipCube,
                                         iCurLevel,
                                         iCurEvent,
                                         ipCalculationPlan,
                                         ipFunctionServer,
                                         &lContext,
                                         &lSliceID);
        if(hr != S_OK)
        {
            lfSkip = true;
            continue;
        }
        else
        {
            DSSCubeDataSlice *lpSlice = ipCube->getSliceByID(lSliceID);
            if(!lpSlice)
            {
                lfSkip = true;
                continue;
            }
            
            EnumDSSDataType lSliceDataType = lpSlice->getDataType();
            if(lSliceDataType == DssDataTypeMissing)
            {
                lfSkip = true;
                continue;
            }
            
            if(iCurLevel > 0)
            {
                DSSTabularIndex *lpIndex = lpSlice->getIndices();
                if(!lpIndex)
                {
                    lfSkip = true;
                    continue;
                }
                
                int lnUnits = lpIndex->getNumUnits();
                int *lpUnits = new int[lnUnits];
                MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lpUnitsPtr(lpUnits);
                hr = lpIndex->getUnits(lnUnits, lpUnits);
                if(hr != S_OK)
                {
                    lfSkip = true;
                    continue;
                }
                
                if(!std::includes(lpTargetUnits, lpTargetUnits+lnTargetUnits, lpUnits, lpUnits+lnUnits))
                {
                    lfSkip = true;
                    continue;
                }
            }
        }
        
        ifHasTrue = true;
        lpThresholdSliceID[i] = lSliceID;
    }
    
//    if(!ifHasTrue)
//        return S_OK;
    
    if(lfSkip)
    {
        double lVal = 0;
        DSSSliceID lSliceID;
        
        hr = DSSCalculationHelper::GetConstantSlice(ipCube,
                                                    0, 0,
                                                    reinterpret_cast<char *>(&lVal),
                                                    sizeof(double),
                                                    DssDataTypeDouble,
                                                    DssDataOk,
                                                    &lSliceID);
        CHECK_HR;
        
        for(i = 0; i <lNumThresholds; ++i)
        {
            if(lpThresholdSliceID[i] == -1)
            {
                lpThresholdSliceID[i] = lSliceID.mSliceID;
            }
        }
    }
    /* tliao, 09/25/2013, we support it in this smart client branch
    // jimzhang, Jul/17/2013, TQMS 770823, if row count of non-constant slices are not same, just return S_OK instead of calculating Fisrt True function. Since this is not supported in current slice calculator.
    long long lFirstNonConstSliceRowCnt = -1;
    bool lbIsFirst = true;
    for (int i = 0; i < lNumThresholds; ++i)
    {
        DSSCubeDataSlice *lpSlice = lContext.mpCube->getSliceByID(lpThresholdSliceID[i]);
        if (! lpSlice)
            continue;
        
        DSSConstantSlice *lpConstSlice = dynamic_cast<DSSConstantSlice *>(lpSlice);
        if (lpConstSlice)
            continue;
     
        DSSDataColumn *lpCol = lpSlice->getColumn();
        if (! lpCol)
            continue;
     
        if (lbIsFirst)
        {
            lFirstNonConstSliceRowCnt = lpCol->getRowCount();
            lbIsFirst = false;
        }
        else
        {
            if (lpCol->getRowCount() != lFirstNonConstSliceRowCnt)
                return S_OK;            
        }
    }
     */
    vector<DSSSliceID*> lvSliceID;
    for (int i = 0; i < lNumThresholds; i++)
    {
        DSSSliceID* lpSliceID = new DSSSliceID();
        lpSliceID->mpCube = lContext.mpCube;
        lpSliceID->mSliceID = lpThresholdSliceID[i];
        lvSliceID.push_back(lpSliceID);
    }
    
    CalculationContext *lCalcContext = DSSCalculationHelper::getCalculationContext(NULL,
                                                                                   (EnumSimpleOperatorType)DssFuncFirstTrue,
                                                                                   lContext.mpCube,
                                                                                   NULL,
                                                                                   &lvSliceID,
                                                                                   DssNumericErrorSkip,
                                                                                   mpSchema);
    
    MBase::DSSStrongPtr<CalculationContext> lCalcContextPtr(lCalcContext);
    
    DSSSliceID lResultSliceID;
    hr = DSSCalculationHelper::CalculateNewSlice(lCalcContext, &lResultSliceID);
    if (lCalcContext->mpFuncObject)
    {
        //delete lCalcContext->mpFuncObject;
        MBase::DSSStrongPtr<DSSFunctionObject> lFunctionObjectPtr(lCalcContext->mpFuncObject);
    }
    CHECK_HR;
    
    *oSliceID = lResultSliceID.mSliceID;
    
    return hr;
}

int DSSMetricThresholds::hEvaluateThresholdCondition(DSSThreshold *ipThreshold, DSSCube *ipCube, int iCurLevel, int iCurEvent,
                                                     DSSCalculationPlan *ipCalculationPlan, DSSFunctionServer *ipFunctionServer, 
                                                     EventContext *ipEventContext, int *opSliceID)
{
    int hr = S_OK;
    
    bool lfEnabled = ipThreshold->getIsEnabled();
    if(!lfEnabled)
        return E_FAIL;
    
    DSSCExpression *lpCondition = ipThreshold->getExpression();
    if(!lpCondition)
        return E_FAIL;
    
    DSSExpNode *lpRoot = lpCondition->Root();
    if(!lpRoot)
        return E_FAIL;
    
    DSSCube* lpTempCube = new DSSCube();
    lpTempCube->Init(ipCube->getTabularData());

    DSSSliceID lSliceID;
#ifndef USE_DFC_FILTER
    DSSFilterExprEvalCube lFilterEval(ipCube, lpTempCube, ipCalculationPlan, iCurLevel, iCurEvent, ipFunctionServer, ipEventContext);
    hr = lFilterEval.processFilterUnit(lpRoot, &lSliceID);
#else
    MBase::SimpleBuffer lSimpleBuffer;
    MBase::BufferPtr lBufferPtr(&lSimpleBuffer);
    MBase::Buffer *lpBuffer = lBufferPtr.Get();
    CHECK_NEWED_PTR(lpBuffer);
    
    CDSSFilterExprEvalCube lFilterEval(lpBuffer, lpTempCube, ipFunctionServer, ipEventContext);
    
    DFCWNode *lpDFCWRoot = DFCWNode::getDFCWNode(lpRoot, lpBuffer);
//    MBase::StrongPtr<DFCNode> lpHolder(lpDFCRoot);
    hr = lFilterEval.ProcessFilterUnit(lpDFCWRoot, &lSliceID);
#endif
    CHECK_HR;
    
    DSSCubeDataSlice* lpSlice = NULL;
    hr = lSliceID.GetSlice(&lpSlice);
    CHECK_HR_PTR(lpSlice);
    
    int lFinalSliceID = -1;
    ipCube->InsertDataSlice(lpSlice, lFinalSliceID);
    *opSliceID = lFinalSliceID;
    
    return hr;
}

int DSSMetricThresholds::hCompareLevel(DSSThreshold *ipThreahold, int iCurLevel, /*object context*/ bool *opNeedCalculation, int ipAutoSubtotal)
{
    assert(ipThreahold && opNeedCalculation);
    *opNeedCalculation = false;
    
    // int lAutoSubtotalKey = 0;
    //get current level's subtotal
    
    //int lThresholSubtotalKey = ipThreahold->
    
    return S_OK;
}