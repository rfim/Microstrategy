//
//  CDSSMetricLimitEventEvaluator.cpp
//  ReportServiceCore
//
//  Created by Taimin Liao on 4/1/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#include "CDSSMetricLimitEventEvaluator.h"
#import "DSSCalculationPlan.h"
#import "DSSTabularData.h"
#import "DSSTabularMetrics.h"
#import "DSSTemplateMetrics.h"
#import "DSSTemplate.h"
#import "DSSCExpression.h"
#import "DSSFunctionServer.h"
#import "DSSRWGridModel.h"
#import "DSSViewDataSet.h"
#import "DSSCubeDataSlice.h"

//#define USE_DFC_FILTER
#ifdef USE_DFC_FILTER
#import "CDSSFilterExprEvalCube.h"
#import "SimpleBuffer.h"
#import "DFCWNode.h"
#else
#import "DSSFilterExprEvalCube.h"
#endif

CDSSMetricLimitEventEvaluator::CDSSMetricLimitEventEvaluator()
{
}

CDSSMetricLimitEventEvaluator::~CDSSMetricLimitEventEvaluator()
{
}

// Information needed in CalcCntxt:
// mpCurLevel, mpCurEvent, pDataSet, pViewDataSet
int CDSSMetricLimitEventEvaluator::ApplyEvent(EventContext *ipCntxt)
{
    
    //In fact, this call will likely result in pXTab being cleaned and reconstructed
    //with a new TabularData, thus caller are advised to obtain
    //pointer to Cube again
    int hr = S_OK;
    DSSCalculationPlan *lpCalculationPlan = ipCntxt->mpCalculationPlan;
    ipCntxt->mpTemplate = (dynamic_cast<DSSRWGridModel*>(ipCntxt->mpDataModel))->getTemplate();
    Int32 lNumMetrics = lpCalculationPlan -> getNumMetrics();
    //	int iLevel = ipCntxt->mLevel;
    int iEvent = ipCntxt->mpEvent->mEventIndex;
    
    EnumOrderingType lOrderingRule = lpCalculationPlan->getOrderingRule();
    
    //	Int32 lIntermediateFlag = 0;
    
    if (ipCntxt -> mLevel > 0 && (lOrderingRule == DssDefaultOrdering || lOrderingRule == DssOrionOrdering))
    {
        Int32 lSliceID = 0;
        bool lfIsCalculated = false;
        for(int iMetric = 0; iMetric < lNumMetrics; iMetric++)
        {
            hr = lpCalculationPlan->getMetricSliceID(ipCntxt->mLevel,
                                                     iEvent - 1,
                                                     iMetric,
                                                     &lfIsCalculated,
                                                     &lSliceID);
            
            if(hr != S_OK) return hr;
            
            hr = lpCalculationPlan->setMetricSliceID(ipCntxt->mLevel,
                                                     iEvent,
                                                     iMetric,
                                                     lfIsCalculated,
                                                     lSliceID);
            if(hr != S_OK) return hr;
        }
        
        return hr;
    }
    
    
    
    Int32 lNumLevels = 0,  lTotalSlices = 0 ;
    lNumLevels = lpCalculationPlan->getNumLevels();
    
    // lweng, 2011-12-21, Hold space for generated SFB slices
    lTotalSlices = lNumMetrics * lNumLevels * 2;
    
    DSSCube *lpCube = ipCntxt->mpCube;
    
    //xhan 11/21/05 Metric limit evaluation needs to include the conditions defined in the individual metrics.
    DSSSliceID lResultSliceID;
    Int32 lSessionId = 0L;
    hr = hEvaluateMetricLimitCondition(ipCntxt, lpCube, lpCalculationPlan, lSessionId, &lResultSliceID);
    
    //then subsetting
    if(hr != S_OK && hr != S_FALSE) return hr;
    
    if(hr == S_FALSE)
    {
        //I guess do not need to do the filtering
        //while we should just copy every single ID to next event
        
        // aye, 255303, 3/13/2008
        // in case the metric limit turns out to be empty, we still need to copy over
        // the slice IDs.
        Int32 lSliceID = 0;
        bool lfIsCalculated = false;
        
        // qjiang 456435, 02/12/2011
        // we should copy over all the levels
        for (Int32 iLevel = 0; iLevel < lNumLevels; iLevel++)
        {
            for(Int32 iMetric = 0; iMetric < lNumMetrics; iMetric++)
            {
                hr = lpCalculationPlan->getMetricSliceID(iLevel,
                                                         iEvent - 1,
                                                         iMetric,
                                                         &lfIsCalculated,
                                                         &lSliceID);
                
                if(hr != S_OK) return hr;
                
                hr = lpCalculationPlan->setMetricSliceID(iLevel,
                                                         iEvent,
                                                         iMetric,
                                                         lfIsCalculated,
                                                         lSliceID);
                if(hr != S_OK) return hr;
            }
        }
        
        return S_OK;
    }
    
    
    //that is the object ID
    //I will gather a list of slices first
    Int32 * lpMetricSliceIDs = new Int32[lTotalSlices];
    CHECK_NEWED_PTR(lpMetricSliceIDs);
    
    METRIC_ID * lpMetricIDs = new METRIC_ID[lTotalSlices];
    CHECK_NEWED_PTR(lpMetricIDs);
    
    Int32 * lpMetricSlicePos = new Int32[lTotalSlices];
    CHECK_NEWED_PTR(lpMetricSlicePos);
    
    Int32 * lpMetricSliceLevel = new Int32[lTotalSlices];
    CHECK_NEWED_PTR(lpMetricSliceLevel);
    
    // lweng, 2011-9-23: Memory optimization
    // use to indicate the cube of each slice
    /*
     Int32 * lpIntermediateFlags = new Int32[lTotalSlices];
     CHECK_NEWED_PTR(lpMetricSliceIDs);
     memset(lpIntermediateFlags, 0, sizeof(Int32) * lTotalSlices);
     */
    Int32 lNumCalculatedMetrics = 0;
    
    //get the previous event's sliceID, also, it is actually OK
    //if there are some slice that are not calculated yet (think about the case
    //that metric limit is the first action), after copying it is just
    for(int iLevel = 0; iLevel < lNumLevels; iLevel++)
    {
        for(int iMetric = 0; iMetric < lNumMetrics; iMetric ++)
        {
            bool lfIsCalculated = false;
            Int32 lSliceID = 0;
            
            hr = lpCalculationPlan -> getMetricSliceID(iLevel,
                                                       iEvent - 1,
                                                       iMetric,
                                                       &lfIsCalculated,
                                                       &lSliceID);
            if(hr != S_OK) return hr;
            
            if(lfIsCalculated)
            {
                lpMetricSliceIDs[lNumCalculatedMetrics] = lSliceID;
                lpMetricSlicePos[lNumCalculatedMetrics] = iMetric;
                lpMetricSliceLevel[lNumCalculatedMetrics] = iLevel;
                //				lpIntermediateFlags[lNumCalculatedMetrics] = lIntermediateFlag;
                if(iLevel == 0)
                {
                    lpMetricIDs[lNumCalculatedMetrics].mMetricID = lpCalculationPlan -> getMetricID(iMetric);
                }
                else
                    lpMetricIDs[lNumCalculatedMetrics].mMetricID = GUID_NULL;
                
                lpMetricIDs[lNumCalculatedMetrics].mDatasetID = GUID_NULL;
                lNumCalculatedMetrics ++;
            }
            
        }
    }
    
    // lweng, 2011-12-20, Store the generated SFB slices after the calculated slices.
    
    Int32 lnGeneratedSlices = 0;
    vector<pair<DSS_ID, Int32> > lvOrigMetricID2Level;
    for(int iLevel = 0; iLevel < lNumLevels; iLevel++)
    {
        for(int iMetric = 0; iMetric < lNumMetrics; iMetric ++)
        {
            DSS_ID lOriginalMetricID = GUID_NULL;
            Int32 lGeneratedSliceID = -1;
            
            lOriginalMetricID = lpCalculationPlan->getMetricID2(iMetric);
            hr = lpCube->get_GeneratedMetricID(lOriginalMetricID, iLevel, &lGeneratedSliceID);
            /*
             hr = lpCalculationPlan->get_SFBSliceID(lOriginalMetricID, iLevel, &lGeneratedSliceID);
             */
            CHECK_HR;
            if (lGeneratedSliceID != -1)
            {
                lpMetricSliceIDs[lNumCalculatedMetrics + lnGeneratedSlices] = lGeneratedSliceID;
                
                //DSS_ID lGeneratedMetricID;
                //hr = ipCntxt->mpViewDataSet->get_GeneratedMetricID(lOriginalMetricID, iLevel, &lGeneratedMetricID);
                lpMetricIDs[lNumCalculatedMetrics + lnGeneratedSlices].mMetricID = GUID_NULL;
                
                lvOrigMetricID2Level.push_back(make_pair(lOriginalMetricID, iLevel));
                
                lnGeneratedSlices++;
            }
        }
    }
    
    DSSViewDataSet *lpViewDataset = NULL;
    hr = (dynamic_cast<DSSRWGridModel*>(ipCntxt->mpDataModel))->GetViewDataSet(&lpViewDataset);
    CHECK_HR;
    DSSGUnitContainer *lpGUnitContainer = lpViewDataset->getGUnitContainer();
    // xhan 03/22/05 remove the old tabulardata from the view dataset
    DSSTabularData *lpOriginalTabularData = lpViewDataset->getTabularData();
    hr = lpViewDataset->setTabularData(NULL);//memory leak?
    CHECK_HR;
    
    //regenerate another tabular data
    DSSTabularData *lpTabularData = new DSSTabularData();
    CHECK_NEWED_PTR(lpTabularData);
    lpTabularData->setObjectContext(lpViewDataset->getObjectContext());
    lpViewDataset->setTabularData(lpTabularData);
    
    vector<int> lvUnits;
    //tliao, 09/24/2013, the mpTemplate doesn't contain all the runtime units, for example,
    // Base: Year, Quarter, Month, Revenue
    // View: Quart, Month, Revenue
    // NGB: Year        Metric Limit: Revenue > 500k
    // The runtime units contain all units(Year,Quarter,Month), but there are only Quarter and Month in mpTemplate
    // So I choose to use original tabular data to get all the runtime units
    //hr = DSSAEHelper::FindOriginalLevel(iEvent, ipCntxt->mpTemplate, lpCube, lpCalculationPlan, &lvUnits);
    hr = lpOriginalTabularData->getLevels(lvUnits);
    CHECK_HR;
    
    DSSCubeDataSlice *lpFilterSlice;
    hr = lResultSliceID.GetSlice(&lpFilterSlice);
    CHECK_HR_PTR(lpFilterSlice);
    //02/01/06 nprabhudessai: VC 8 requirement
    Int32 *lpInt = 0; // dummy null pointer
    
    // aye, 12/12/07
    // changed FilterPopulate signature
    
    // qjiang 11/3/2009 deal with duplicated metrics in metric limit event
    Int32 lnDistinct = 0;
    Int32* lpDistinct = NULL;
    
    Int32* lpMetricMap = new Int32[lTotalSlices];
    CHECK_NEWED_PTR(lpMetricMap);
    
    hr = hRemoveDuplicateMetrics(lNumCalculatedMetrics + lnGeneratedSlices, lpMetricIDs, lpMetricMap, &lnDistinct, &lpDistinct);
    CHECK_HR;
    
    // lweng, 2011-12-20, Include generated SFB slices
    vector<DSSSliceID> lvMetricSliceID(lnDistinct);
    for (Int32 iMetric = 0; iMetric < lnDistinct; iMetric++)
    {
        lvMetricSliceID[iMetric].mpCube = lpCube;//lpIntermediateFlags[lpDistinct[iMetric]]?  ipCntxt->mpIntermediateCube: lpCube.p;
        lvMetricSliceID[iMetric].mSliceID = lpMetricSliceIDs[lpDistinct[iMetric]];
    }
    
    // dataset.MetricID
    //	Int32* lpDataSource = NULL;
    
    //do the copying, all the attributes and the slices
    hr = lpTabularData -> filterPopulate(lpCube,
                                         lvUnits.size(), (lvUnits.size()>0)?&lvUnits[0]:lpInt,
                                         lvMetricSliceID, lpMetricIDs,
                                         lpFilterSlice, NULL, -1, NULL, 0 , false, lpGUnitContainer);
    
    CHECK_HR;
    
    //lwang, 10/12/2009, 379322, set the Consolidation Subtotal Property to generate the correct main index.
    DSSTabularData *lpSrcTabularData = lpCube->getTabularData();
    CHECK_PTR(lpSrcTabularData);
    
    //tliao, 951691, 2014/08/14, copy dynamic dimty info
    vector<map<GUID, vector<int> > >* lpvDD = lpSrcTabularData->getDynamicDimty();
    lpTabularData->setDynamicDimty(lpvDD);
    
    //tliao, 814212, 11/20/2013, it is possible that the metric limit event is before all compound metric event,
    //       in that case, there is no compound metric in the lpTabularData, we need to add it back from original tabular data
    DSSTabularMetrics* lpOriginalMetrics = lpSrcTabularData->getTabularMetircs();
    DSSTabularMetrics* lpNewMetrics = lpTabularData->getTabularMetircs();
    lpOriginalMetrics->AddMetricToAnotherMetrics(lpNewMetrics);
    
    /*
     CComQIPtr<ICDSSTabularData3, &IID_ICDSSTabularData3> lpSrcTabularData3(lpSrcTabularData);
     CHECK_PTR(lpSrcTabularData3);
     
     VARIANT_BOOL lEnableCSTotal = VARIANT_FALSE;
     hr = lpSrcTabularData->GetConsolidationSubtotalProperty(&lEnableCSTotal);
     CHECK_HR;
     
     CComQIPtr<ICDSSTabularData3, &IID_ICDSSTabularData3> lpCDSSTabularData3(lpcTabularData);
     CHECK_PTR(lpCDSSTabularData3);
     
     hr = lpCDSSTabularData3->SetConsolidationSubtotalProperty(lEnableCSTotal);
     CHECK_HR;
     */
    
    Int32 lUnitCount1 = 0;
    hr = lpTabularData->Count(&lUnitCount1);
    CHECK_HR;
    /*
     //maybe the consolidation is yet to be created
     hr = CDSSCalculationHelper::PopulateConsolidationUnit(mpObjContext, ipCntxt->mpTemplate, lpTabularData, mpBuffer);
     CHECK_HR;
     
     //yma 11/12/07 need to populate DE if it is not in TabularData
     hr = CDSSCalculationHelper::PopulateTemplateDEUnit(ipCntxt->mpTemplate, lpTabularData, mpFunctionServer,mpBuffer, false);
     CHECK_HR;
     
     //ctang: replace the DE unit from global lookup table, for New RWD case
     //get container first
     CComPtr<ICDSSTabularData> lpOrignTabularData;
     hr = lpCube->get_TabularData(&lpOrignTabularData);
     CHECK_HR_PTR(lpOrignTabularData);
     
     CDSSGUnitContainer* lpContainerC = NULL;
     hr = static_cast<CDSSTabularData*>(lpOrignTabularData.p)->GetGUnitContainerC(&lpContainerC);
     CHECK_HR;
     
     if (lpContainerC)
     {
     DSS_ID lID;
     hr = (ipCntxt->mpViewDataSet)->get_TemplateID(&lID);
     CHECK_HR;
     
     hr = static_cast <CDSSTabularData *> (lpTabularData.p)->ReplaceDEUnit(lpContainerC, lID, mpReportInstance);
     CHECK_HR;
     
     hr = static_cast <CDSSTabularData *> (lpTabularData.p)->SetGUnitContainer(lpContainerC);
     CHECK_HR;
     }
     
     Int32 lUnitCount2 = 0;
     hr = lpTabularData->Count(&lUnitCount2);
     CHECK_HR;
     
     if (lUnitCount1 < lUnitCount2)
     {	//It happens when metric limit is evaluated before some consolidation events
     //some consolidation unit(s) is added to tabular data, then the index pool in the tabular data needs to be rebuilt.
     // The workflow is not optimial, but this is easiest way now.
     CDSSXTabIndexPool *lpIndexPool = NULL;
     hr = (static_cast<CDSSTabularData *> (lpTabularData.p) )->get_IndicesPoolC(&lpIndexPool);
     CHECK_HR_PTR(lpIndexPool);
     
     hr = lpIndexPool->ReInit();
     CHECK_HR;
     }
     
     //after we copied them out
     //we will erase the plan
     hr = lpCalcPlan6 -> EraseEvents( mIdxDoable );
     */
    
    // put the new cube into the ViewDataSet
    DSSCube *lpNewCube = new DSSCube();
    hr = lpNewCube->Init(lpTabularData);
    if(hr != S_OK) return hr;
    lpViewDataset->setCube(lpNewCube);
    
    DSSTabularMetrics *lpMetrics = lpTabularData->getTabularMetircs();
    vector<DSSCubeDataSlice *> lvSlices = lpMetrics->getSliceList();
    
    Int32 lMetricSliceID = 0;
    Int32 lSFBSlice = 0;
    
    for (int i = 0; i < lvSlices.size(); i++)
    {
        DSSCubeDataSlice* lpNewSlice = NULL;
        hr = lpNewCube -> duplicateSlice(lvSlices[i], &lpNewSlice, false);
        CHECK_HR;
        
        hr = lpNewCube->InsertDataSlice(lpNewSlice, lMetricSliceID);
        CHECK_HR;
        
        if (lvSlices[i]->IsSFBSlice())
        {
            lpNewCube->put_GeneratedMetricID(lvOrigMetricID2Level[lSFBSlice].first, lvOrigMetricID2Level[lSFBSlice].second, lMetricSliceID);
            lSFBSlice++;
        }
        else
        {
            GUID lMetricGUID = lpMetrics->getMetricID(i);
            if(lpCalculationPlan)
            {
                int lMetricID = lpCalculationPlan->getMetricIndex(lMetricGUID);
                hr = lpCalculationPlan->setMetricSliceID2(lMetricID, lMetricSliceID);
                CHECK_HR;
            }
        }
    }
    
    //get a slice for each metric from the new cube and put into plan
    for(int iMetric = 0; iMetric < lNumCalculatedMetrics; iMetric ++)
    {
        lMetricSliceID = lpMetricMap[iMetric];
        
        hr = lpCalculationPlan -> setMetricSliceID(lpMetricSliceLevel[iMetric],
                                                   iEvent,
                                                   lpMetricSlicePos[iMetric],
                                                   1,
                                                   lMetricSliceID);
        CHECK_HR;
    }
    /*
     // lweng, 2011-12-20, Reset the SFB slice map
     hr = lpCalPlan6->ClearSFBSliceMap();
     CHECK_HR;
     
     // lweng, 2012-1-6, The last lnGeneratedSlices number of metrics should be generated total subtotal metrics
     for (iMetric = 0; iMetric < lnGeneratedSlices ; iMetric++)
     {
     hr = lpNewCube -> get_MetricSlice(
     lnDistinct -lnGeneratedSlices + iMetric,
     &lMetricSliceID);
     if(hr != S_OK) return hr;
     
     hr = lpCalPlan6->put_SFBSliceID(lvOrigMetricID2Level[iMetric].first, lvOrigMetricID2Level[iMetric].second, lMetricSliceID);
     }
     */
    return S_OK;
    
    
}


int CDSSMetricLimitEventEvaluator::hEvaluateMetricLimitCondition(EventContext *ipCntxt,
                                                                 DSSCube *ipTargetCube,
                                                                 DSSCalculationPlan *lpCalculationPlan,
                                                                 Int32 iSessionId,
                                                                 DSSSliceID *opResultSliceID)
{
    int hr = S_OK;
    
    //just go straight to template and find the expression
    DSSTemplate *lpTemplate = ipCntxt->mpTemplate;
    DSSTemplateMetrics *lpTemplateMetrics = lpTemplate -> getTemplateMetrics();
    
    //lwang, 01/11/2008, TQMS 295748
    //It is possible for XDA report that no metrics unit is on the template,
    //but metric limit is defined on a disabled metric
    Int32 lMetricCount = 0;
    if (lpTemplateMetrics)
    {
        //fyang, 03/21/2011, skill metric limit evaluation for transaction reports
        VARIANT_BOOL lbEvaluate = VARIANT_TRUE;
        //		hr = licpTemplateMetrics->get_EvaluateMetricLimit(&lbEvaluate);
        CHECK_HR;
        
        if(lbEvaluate == VARIANT_FALSE)
            return S_FALSE;
        
        lMetricCount = lpTemplateMetrics->Count();
    }
    
    Int32 lDisabledMetricCount = 0;
    /*
     DSSTemplateMetrics *lpDisabledTemplateMetrics;
     hr = lpTemplate->getDisabledMetrics(&lpDisabledTemplateMetrics);
     CHECK_HR;
     
     if (lpDisabledTemplateMetrics != NULL)
     {
     //fyang, 03/21/2011, skill metric limit evaluation for transaction reports
     CComQIPtr<ICDSSTemplateMetrics2, &IID_ICDSSTemplateMetrics2> licpDisabledTemplateMetrics(lpDisabledTemplateMetrics);
     CHECK_PTR(licpDisabledTemplateMetrics);
     
     VARIANT_BOOL lbEvaluate = VARIANT_TRUE;
     hr = licpDisabledTemplateMetrics->get_EvaluateMetricLimit(&lbEvaluate);
     CHECK_HR;
     
     if(lbEvaluate == VARIANT_FALSE)
     return S_FALSE;
     
     hr = lpDisabledTemplateMetrics->DisabledCount(&lDisabledMetricCount);
     CHECK_HR;
     }
     */
    std::vector<DSSTemplateMetric*> *lpDisabledTemplateMetrics = lpTemplate->getDisabledMetrics();
    if (lpDisabledTemplateMetrics != NULL)
    {
        lDisabledMetricCount = lpDisabledTemplateMetrics->size();
    }
    DSSSliceID *lpLimitSlices = new DSSSliceID [lMetricCount + 1 + lDisabledMetricCount];
    CHECK_NEWED_PTR(lpLimitSlices);
    
    // Initialize the slice id to -1
    Int32 i = 0;
    for(i = 0; i <= lMetricCount + lDisabledMetricCount; i++)
        lpLimitSlices[i].mSliceID = -1;
    
    
    //Evaluate the limit associated with templateMetrics first and store the result to lpLimitSlices[0]
    
    //lwang, 01/11/2008, TQMS 295748
    //It is possible for XDA report that no metrics unit is on the template,
    //but metric limit is defined on a disabled metric
    /*
     if (lpTemplateMetrics)
     {
     CComPtr<ICDSSExpression> lpMetricLimit;
     hr = lpTemplateMetrics -> get_MetricLimit(&lpMetricLimit);
     if(hr != S_OK) return hr;
     
     if (!lpMetricLimit)
     // should not be NULL, an empty filter will have NULL at the root of the expr
     return E_FAIL;
     
     CComPtr<ICDSSNode> lpRoot;
     hr = lpMetricLimit -> get_Root(&lpRoot);
     if(hr != S_OK) return hr;
     
     if (lpRoot)
     {
     //xhan 01/30/04
     CDSSFilterExprEvalCube lFilterEval(mpBuffer,
     mpObjContext,
     ipTargetCube,
     ipTargetCube,
     lpCalculationPlan,
     ipCntxt->mLevel,
     mIdxDoable,
     mpSource,
     mpFunctionServer,
     mpReportInstance,
     ipCntxt->mpTemplate,
     mpLocalSchema);
     
     hr = lFilterEval.ProcessFilterUnit(
     lpRoot,
     &lpLimitSlices[0]);
     if(hr != S_OK)
     return hr;
     }
     
     }
     */
    // record the metrics that needs to calculate range/count
    std::set<DSS_ID> lRuntimeMLMetrics;
    DSSFunctionServer *lpFunctionServer = new DSSFunctionServer();
    //for each template metric, evaluate the metric limit expression
    for ( i = 0; i < lMetricCount; i ++)
    {
        DSSTemplateMetric *lpTemplateMetric = lpTemplateMetrics->Item(i);
        CHECK_HR_PTR(lpTemplateMetric);
        /*
         // jinzhang 08/09/2009 Check whether use runtime metric limit is set
         SQIPtr(ICDSSTemplateMetric6) licpTemplateMetric6(lpTemplateMetric);
         if (!licpTemplateMetric6)
         return E_NOINTERFACE;
         
         VARIANT_BOOL lUseRuntimeML = VARIANT_FALSE;
         hr = licpTemplateMetric6->get_UseRuntimeMetricLimit(&lUseRuntimeML);
         CHECK_HR;
         */
        DSSCExpression *lpLimit = NULL;
        //if (lUseRuntimeML == VARIANT_FALSE)
        {
            //tliao, 935325, 2014/07/21, use runtimeLimitExpression for metric limit event
            lpLimit = lpTemplateMetric->getRuntimeLimitExpression();
            //tliao, 937312, 2014/07/21, use original limit expression if no runtimeLimitExpression
            if (lpLimit == NULL)
            {
                lpLimit = lpTemplateMetric->getLimitExpression();
            }
            lRuntimeMLMetrics.insert(lpTemplateMetric->getID());//xiazhou
        }
        /*
         else
         {
         hr = licpTemplateMetric6->get_RuntimeMetricLimit(&lpLimit);
         CHECK_HR;
         
         DSS_ID lID = GUID_NULL;
         hr = licpTemplateMetric6->get_MetricID(&lID);
         CHECK_HR;
         
         lRuntimeMLMetrics.insert(lID);
         }
         */
        
        if (lpLimit)
        {
            DSSExpNode *lpRoot = lpLimit->getRoot();
            
            if(lpRoot) //make sure the expression is not empty
            {
#ifndef USE_DFC_FILTER
                
                DSSFilterExprEvalCube lFilterEval(//mpBuffer,
                                                  //mpObjContext,
                                                  ipTargetCube,
                                                  ipTargetCube,
                                                  lpCalculationPlan,
                                                  ipCntxt->mLevel,
                                                  ipCntxt->mpEvent->mEventIndex,
                                                  //mpSource,
                                                  lpFunctionServer,
                                                  ipCntxt);
                //mpReportInstance,
                //ipCntxt->mpTemplate,
                //mpLocalSchema);
                
                hr = lFilterEval.processFilterUnit(lpRoot, &(lpLimitSlices[i + 1]));
#else
                MBase::SimpleBuffer lSimpleBuffer;
                MBase::BufferPtr lBufferPtr(&lSimpleBuffer);
                MBase::Buffer *lpBuffer = lBufferPtr.Get();
                CHECK_NEWED_PTR(lpBuffer);
                
                CDSSFilterExprEvalCube lFilterEval(lpBuffer, ipTargetCube, lpFunctionServer, ipCntxt);
                
                DFCWNode *lpDFCWRoot = DFCWNode::getDFCWNode(lpRoot, lpBuffer);
                //               MBase::StrongPtr<DFCNode> lpHolder(lpDFCRoot);
                hr = lFilterEval.ProcessFilterUnit(lpDFCWRoot, &(lpLimitSlices[i + 1]));
#endif
            }
        }
    }
    
    //evaluate the metric limit expression for disabled metric
    if (lpDisabledTemplateMetrics != NULL)
    {
        for ( i = 0; i < lDisabledMetricCount; i ++)
        {
            DSSTemplateMetric *ldpMetric = (*lpDisabledTemplateMetrics)[i];
            CHECK_PTR(ldpMetric);
            /*
             // jinzhang 08/09/2009 Check whether use runtime metric limit is set
             VARIANT_BOOL lUseRuntimeML = VARIANT_FALSE;
             hr = ldpMetric->get_UseRuntimeMetricLimit(&lUseRuntimeML);
             CHECK_HR;
             */
            DSSCExpression *lpLimit;
            //			if (lUseRuntimeML == VARIANT_FALSE)
            {
                //tliao, 935325, 2014/07/21, use runtimeLimitExpression for metric limit event
                lpLimit = ldpMetric->getRuntimeLimitExpression();
                //tliao, 937312, 2014/07/21, use original limit expression if no runtimeLimitExpression
                if (lpLimit == NULL)
                {
                    lpLimit = ldpMetric->getLimitExpression();
                }
                lRuntimeMLMetrics.insert(ldpMetric->getID());//xiazhou
            }
            /*
             else
             {
             hr = licpDisabledMetric6->get_RuntimeMetricLimit(&lpLimit);
             CHECK_HR;
             
             DSS_ID lID = GUID_NULL;
             hr = licpDisabledMetric6->get_MetricID(&lID);
             CHECK_HR;
             
             lRuntimeMLMetrics.insert(lID);
             }
             */
            if (lpLimit)
            {
                DSSExpNode *lpRoot = lpLimit->getRoot();
                
                if(lpRoot) //make sure the expression is not empty
                {
#ifndef USE_DFC_FILTER
                    DSSFilterExprEvalCube lFilterEval(//mpBuffer,
                                                      //mpObjContext,
                                                      ipTargetCube,
                                                      ipTargetCube,
                                                      lpCalculationPlan,
                                                      ipCntxt->mLevel,
                                                      ipCntxt->mpEvent->mEventIndex,
                                                      //mpSource,
                                                      lpFunctionServer,
                                                      ipCntxt);
                    //mpReportInstance,
                    //ipCntxt->mpTemplate,
                    //mpLocalSchema);
                    
                    
                    hr = lFilterEval.processFilterUnit(lpRoot,
                                                       &(lpLimitSlices[i + lMetricCount + 1]));
#else
                    MBase::SimpleBuffer lSimpleBuffer;
                    MBase::BufferPtr lBufferPtr(&lSimpleBuffer);
                    MBase::Buffer *lpBuffer = lBufferPtr.Get();
                    CHECK_NEWED_PTR(lpBuffer);
                    
                    CDSSFilterExprEvalCube lFilterEval(lpBuffer, ipTargetCube, lpFunctionServer, ipCntxt);
                    
                    DFCWNode *lpDFCWRoot = DFCWNode::getDFCWNode(lpRoot, lpBuffer);
                    //                  MBase::StrongPtr<DFCNode> lpHolder(lpDFCRoot);
                    
                    hr = lFilterEval.ProcessFilterUnit(lpDFCWRoot,
                                                       &(lpLimitSlices[i + lMetricCount + 1]));
#endif
                    
                }
            }//end of if (lpLimit)
        }//end of for ( i = 0; i < lDisabledMetricCount; i ++)
    }//end of if (lpDisabledTemplateMetrics != NULL)
    
    //Combine all limit slices together by And operator to generate a single result slice
    
    // qjiang 11/3/2010
    // calculate the range/count for metrics that have runtime metric limit defined
    
    //xiazhou, start calculating metric range
    std::set<DSS_ID>::iterator lEnd(lRuntimeMLMetrics.end());
    for (std::set<DSS_ID>::iterator lIt(lRuntimeMLMetrics.begin()); lIt != lEnd; ++lIt)
    {
        Int32 lMetric = -1;
        DSS_ID lMetricID = *lIt;
        lMetric = lpCalculationPlan->getMetricIndex(lMetricID);
        //tliao, 960882, 2014/08/28, we can't find the metric in calculation plan in some case
        // not sure if it is expected, skip it for now
        if (lMetric < 0)
        {
            continue;
        }
        bool lIsCalculated = false;
        Int32 lSliceID = -1;
        int iEvent = ipCntxt->mpEvent->mEventIndex;
        
        hr = lpCalculationPlan->getMetricSliceID(0, iEvent - 1, lMetric, &lIsCalculated, &lSliceID);
        CHECK_HR;
        
        if (lIsCalculated)
        {
            DSSCubeDataSlice *lpSlice = ipTargetCube->getSliceByID(lSliceID);
            if(lpSlice)
            {
                hr = lpSlice->CalculateRange();
                CHECK_HR;
            }
            
        }
    }
    //xiazhou, end calculating metric range
    
    //Count the limit slices first
    Int32 lLimitSliceCount = 0;
    for (i = 0; i < lMetricCount + 1 + lDisabledMetricCount; i ++)
    {
        if (lpLimitSlices[i].mSliceID != -1)
        {
            lpLimitSlices[lLimitSliceCount] = lpLimitSlices[i]; //condense the limit array
            lLimitSliceCount ++;
        }
    }
    
    if (lLimitSliceCount == 0)
        return S_FALSE; //No metric limit anywhere
    
    
    GUID lBuildInPackageGUID = {0xACC5BFA0,0x11D3,0xDD9C,{0xC0,0x00,0x81,0xB9,0xEA,0x33,0x22,0x4F}};//GUID_DSSBuiltInPackage;
    DSSFunctionObject *lpAndFunc = lpFunctionServer->FindFunctionObject(DssFuncAnd, &lBuildInPackageGUID);
    EnumNullCheckingOption lNullCheckingOption = DssNumericErrorSkip;
    /*
     hr = CDSSAEHelper::FindNullCheckingOption(mpReportInstance,
     &lNullCheckingOption);
     CHECK_HR;
     */
    hr = DSSCalculationHelper::CalculateAndOR(lpAndFunc, lLimitSliceCount, lpLimitSlices, ipTargetCube,
                                              lNullCheckingOption, false, opResultSliceID, ipCntxt->mpSchema);
    CHECK_HR;
    
    
    return S_OK;
    
}

// qjiang 11/3/2009
// the input MetricIDs may have duplicated GUIDs, we need to get all the distincts, here we treat GUID_NULL as distinct
// e.g. ipMetricIDs = {A, B, A, B, C, GUID_NULL, GUID_NULL, D}, then after calling this function, the parameters may look like:
// ipMetricIDs = {A, B, C, GUID_NULL, GUID_NULL, D, GUID_NULL, D}
// opMetricMap = {0, 1, 0, 1, 2, 3, 4, 5}
// onDistinct = 6
// oppDistinct = {0, 1, 4, 5, 6, 7}
int CDSSMetricLimitEventEvaluator::hRemoveDuplicateMetrics(Int32 lnMetrics, METRIC_ID* ipMetricIDs,
                                                           Int32* opMetricMap, Int32* onDistinct, Int32** oppDistinct)
{
    if (opMetricMap == NULL || oppDistinct == NULL || onDistinct == NULL)
        return E_POINTER;
    
    // the length of lpDistinct should be the distinct number, but we cannot pre-know it, allocate a big-enough array here
    Int32* lpDistinct = new Int32 [lnMetrics];
    CHECK_NEWED_PTR(lpDistinct);
    
    Int32 lnNulls = 0;
    
    std::map<METRIC_ID, Int32, MetricIDCompare > lMetricsMap;
    
    Int32 lPos = 0;
    
    for (Int32 iMetric = 0; iMetric < lnMetrics; iMetric++)
    {
        METRIC_ID& lID = ipMetricIDs[iMetric];
        
        if (MBase::IsGUIDNULL(lID.mMetricID))
        {	// treat GUID_NULL as distinct
            lnNulls ++;
        }
        else
        {
            std::map<METRIC_ID, Int32, MetricIDCompare >::iterator lIt = lMetricsMap.find(lID);
            if (lIt == lMetricsMap.end())
            {// not find, add to map
                lMetricsMap[lID] = lPos;
            }
            else
            {// appear before, not distinct
                opMetricMap[iMetric] = (*lIt).second;
                continue;
            }
        }
        
        // comes here, meaning current GUID is distinct
        lpDistinct[lPos] = iMetric;
        opMetricMap[iMetric] = lPos;
        
        if (lPos < iMetric)
        {
            ipMetricIDs[lPos] = lID;
        }
        lPos ++;
        
    }
    
    *oppDistinct = lpDistinct;
    *onDistinct = lMetricsMap.size() + lnNulls;
    
    return S_OK;
}
