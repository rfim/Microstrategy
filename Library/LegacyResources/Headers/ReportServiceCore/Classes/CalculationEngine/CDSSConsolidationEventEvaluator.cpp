//
//  CDSSConsolidationEventEvaluator.cpp
//  ReportServiceCore
//
//  Created by Liao Taimin on 4/17/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#include "CDSSConsolidationEventEvaluator.h"
#import "DSSCalculationPlan.h"
#import "DSSTabularData.h"
#import "CDSSConsolidationEvaluators.h"
#import "CDSSConsolidationEvaluator.h"
#import "DSSTabularConsolidation.h"
#import "DSSFunctionServer.h"
#import "DSSCubeDataSlice.h"
#import "DSSTabularIndex.h"

CDSSConsolidationEventEvaluator::CDSSConsolidationEventEvaluator() : mEventType(DssEventConsolidation)
{
}

CDSSConsolidationEventEvaluator::CDSSConsolidationEventEvaluator(EnumEventType iEventType)
: mEventType(iEventType)
{
}

CDSSConsolidationEventEvaluator::~CDSSConsolidationEventEvaluator()
{
}


// Information needed in CalcCntxt: mLevel, mpCube
int CDSSConsolidationEventEvaluator::ApplyEvent(EventContext *ipCntxt)
{
    
    int hr = S_OK;
    DSSCalculationPlan *ipCalculationPlan = ipCntxt->mpCalculationPlan;
    int iLevel = ipCntxt->mLevel;
    int iEvent = ipCntxt->mpEvent->mEventIndex;
    GUID iID = ipCntxt->mpEvent->mID;
    Int32 iMetric = 0,  lNumMetrics = 0 ;
    lNumMetrics = ipCalculationPlan -> getNumMetrics();
    
    EnumOrderingType lOrderingRule = ipCalculationPlan->getOrderingRule();
    
    //	Int32 lIntermediateFlag = 0;
    
    // yma 7/20/07 DE will continue evaluate since for smart element we always recalculate,
    // and will ignore the following condition checking
    if(mEventType == DssEventConsolidation)
    {
        if (ipCntxt->mLevel > 0 && (lOrderingRule == DssDefaultOrdering || lOrderingRule == DssOrionOrdering))
        {
            Int32 lSliceID = 0;
            bool lfIsCalculated = false;
            for(iMetric = 0; iMetric < lNumMetrics; iMetric++)
            {
                hr = ipCalculationPlan->getMetricSliceID(iLevel,
                                                         iEvent-1,
                                                         iMetric,
                                                         &lfIsCalculated,
                                                         &lSliceID);
                if(hr != S_OK) return hr;
                
                hr = ipCalculationPlan->setMetricSliceID(iLevel,
                                                         iEvent,
                                                         iMetric,
                                                         lfIsCalculated,
                                                         lSliceID);
                if(hr != S_OK) return hr;
            }
            
            return S_OK;
            
        }
    }
    
    
    //also need to find the consolidation unit
    Int32 lConsolidationUnit = 0;
    DSSTabularData *lpTabularData = ipCntxt->mpCube->getTabularData();
    CHECK_PTR(lpTabularData);
    
    hr = lpTabularData -> FindUnitI(iID, DssTemplateConsolidation, lConsolidationUnit);
    if(hr != S_OK) return hr;
    
    //create the evaluator
    //Note did not use buffer
    CDSSConsolidationEvaluators * lpEvaluators = new CDSSConsolidationEvaluators();
    //guarantee the release
    //	DSSFunctionPackage *lpEvaluators2 = lpEvaluators;
    
    DSSTabularUnit *lpTabularDataUnit;
    hr = lpTabularData -> Item(lConsolidationUnit, &lpTabularDataUnit);
    if(hr != S_OK) return hr;
    
    DSSTabularConsolidation *lpXTabConsolidation = dynamic_cast<DSSTabularConsolidation*>(lpTabularDataUnit);
    
    DSSFunctionServer *lpFunctionServer = new DSSFunctionServer();
    hr = lpEvaluators -> init(ipCntxt->mpObjectContext, lpFunctionServer, lpXTabConsolidation);
    if(hr != S_OK) return hr;
    
    // aye, 290304, 12/1/2007
    // if metric number is 0, we still need to get the consolidation index
    if (lNumMetrics == 0)
    {
        /*Int32 lSliceID = 0;
         
         hr = (ipCntxt->mpTargetCube) -> Consolidate(lpEvaluators,
         lConsolidationUnit,
         -1,
         DssNumericErrorSkip,
         &lSliceID);
         */
        CHECK_HR;
    }
    
    bool lfConsolidate = false;
    
    for(iMetric = 0; iMetric < lNumMetrics; iMetric ++)
    {
        bool lfIsCalculated = false,  lfIsCalculated2 = false ;
        Int32 lSliceID = 0,  lSliceID2 = 0 ;
        hr = ipCalculationPlan -> getMetricSliceID(
                                                   iLevel,
                                                   iEvent,
                                                   iMetric,
                                                   &lfIsCalculated,
                                                   &lSliceID);
        if(hr != S_OK) return hr;
        
        if(lfIsCalculated)
            //well, this is actually very strange
            //maybe should return fail, ignore first
            continue;
        
        
        //need to calculate this one
        //action is the same for all metrics (original or compound)
        hr = ipCalculationPlan -> getMetricSliceID(iLevel,
                                                   iEvent - 1,
                                                   iMetric,
                                                   &lfIsCalculated2,
                                                   &lSliceID2
                                                   );
        if(!lfIsCalculated2)
            //something is wrong
            continue;
        
        //04/04/03 xhan if report is subset report, skip the consolidation
        bool lfViewTemplate;
        hr = hCheckViewTemplate(&lfViewTemplate);
        CHECK_HR;
        
        // lweng, 2012-3-16, TQMS 537312, check if it's a SFB slice on DE
        bool lfSFBSliceOnDE = false;
        
        DSSCubeDataSlice *lpSlice;
        //		hr = (lIntermediateFlag ? mpCntxt->mpIntermediateCube: mpCntxt->mpCube)->get_SliceByID(lSliceID2, &lpSlice);
        lpSlice = ipCntxt->mpCube->getSliceByID(lSliceID2);
        CHECK_PTR(lpSlice);
        
        bool lfSFBSlice = false;
        //		lfSFBSlice = static_cast<CDSSCubeSlice*>(lpSlice.p)->GetSFBFlag();
        
        if (lfSFBSlice)
        {
            DSSTabularIndex *lpIndices = lpSlice->getIndices();
            CHECK_HR;
            
            Int32 lnUnit = lpIndices->getNumUnits();
            
            Int32 *lpUnit = new Int32[lnUnit];
            CHECK_NEWED_PTR(lpUnit);
            
            hr = lpIndices->getUnits(lnUnit, lpUnit);
            CHECK_HR;
            
            for (Int32 iUnit = 0; iUnit < lnUnit; iUnit++)
            {
                if (lpUnit[iUnit] == lConsolidationUnit)
                {
                    lfSFBSliceOnDE = true;
                    break;
                }
            }
        }
        
        if ( (mEventType == DssEventConsolidation) && lfViewTemplate) {
            //because of subsetting, there is a chance that it is already consolidated
            lSliceID = lSliceID2;
        }
        else if (lfSFBSliceOnDE)	//SFB slice on DE, skip consolidate
            lSliceID = lSliceID2;
        else	//otherwise, consolidate this slice
        {
            
            //Add NULL checking for consolidation, xhan
            METRIC_ID lMetricID;
            //			hr = lpCalcPlan6->get_MetricIDEx(iMetric, &lMetricID.mDatasetID, &lMetricID.mMetricID);
            lMetricID.mMetricID = ipCalculationPlan->getMetricID(iMetric);
            //			CHECK_HR;
            /*
             CComPtr<ICDSSObjectInfo> licpViewInfo;
             hr = mpCntxt->mpTemplate->get_Info(&licpViewInfo);
             CHECK_HR;
             
             DSS_ID lViewID = GUID_NULL;
             hr = licpViewInfo->get_ID(&lViewID);
             CHECK_HR;
             
             EnumNullCheckingOption lNullCheckingOption = DssNumericErrorSkip;
             
             //ctang 2009-11-30 using CalculatPlan's method to get null checking option
             
             hr = lpCalcPlan6->get_NullCheckingOptionWithObjContext(
             iMetric,
             mpReportInstance,
             mpObjContext,
             lNullCheckingOption,
             lMetricID.mMetricID,
             DssTypeMetric,
             &lNullCheckingOption,
             &lViewID);
             CHECK_HR;
             
             */
            EnumNullCheckingOption lNullCheckingOption = DssNumericErrorSkip;//hard code ltm
            Int32 lFuncIndex = DssFuncUnknown;
            //lwang, 12/8/2008,
            //Use First/Last function if it is non-aggregatable metric, otherwise use the dynamic function
            /*
             bool lbHasNonAggFunction = false;
             hr = hCheckNonAggMetric(licpViewInfo, lMetricID, lFuncIndex, lbHasNonAggFunction);
             CHECK_HR;
             */
            hr = lpEvaluators->put_NonAggFunc(lFuncIndex);
            CHECK_HR;
            
            // yma 03/02/06
            // Find the DynamicAggFunction and put into Evaluators
            // TODO
            
            DSSExpNode *lpExprRoot;
            DSSFunctionObject *lpFunctionObject;
            VARIANT_BOOL lfOrderSignificant = VARIANT_FALSE;
            lFuncIndex = DssFuncUnknown;
            DSSFunctionServer *lpFunctionServer = new DSSFunctionServer();
            hr = DSSCalculationHelper::ChooseAggregationFunction(ipCntxt->mpObjectContext,
                                                                 lpFunctionServer,
                                                                 lMetricID,
                                                                 &lpFunctionObject,
                                                                 &lFuncIndex,
                                                                 &lfOrderSignificant,
                                                                 &lpExprRoot);
            
            hr = lpEvaluators->put_DynamicAggFunc(lFuncIndex);
            CHECK_HR;
            
            // lweng, 2012-7-2, this flag indicate that we perform the consolidate
            if (!lfConsolidate)
                lfConsolidate = true;
            /*
             hr = (lIntermediateFlag ? mpCntxt->mpIntermediateCube: mpCntxt->mpCube) -> Consolidate(lpEvaluators,
             lConsolidationUnit,
             lSliceID2,
             lNullCheckingOption,
             &lSliceID);
             */
            ipCntxt->mpCube-> Consolidate(lpEvaluators,
                                          lConsolidationUnit,
                                          lSliceID2,
                                          lNullCheckingOption,
                                          &lSliceID);
            if(hr != S_OK) {
                if (ipCntxt->mLevel == 0)
                    return hr = E_FAIL;
                else { //xhan try not fail the report, conslidation could not be applied to subtotal
                    //so just copy the subtotal result
                    hr = ipCalculationPlan->getMetricSliceID(iLevel,
                                                             iEvent-1,
                                                             iMetric,
                                                             &lfIsCalculated,
                                                             &lSliceID
                                                             );
                    if(hr != S_OK) return hr;
                    
                }
            }
        }
        
        
        hr = ipCalculationPlan -> setMetricSliceID(iLevel,
                                                   iEvent,
                                                   iMetric,
                                                   true,
                                                   lSliceID);
        
        if(hr != S_OK) return hr;
    }
    
    if (iLevel == 0)
    {
        // lweng, 2012-3-16, TQMS 537312, process the SFB slices, convert CG to CS
        /*
         Int32 iLevel = 0,  lNumLevels = 0 ;
         lNumLevels = ipCalculationPlan->getNumLevels();
         
         for (iLevel = 1; iLevel < lNumLevels; iLevel++)
         {
         for	(iMetric = 0; iMetric < lNumMetrics; iMetric++)
         {
         // 1. Get the SFB slice ID.
         DSS_ID lMetricID;
         hr = ipCalculationPlan->get_MetricID(iMetric, &lMetricID);
         CHECK_HR;
         
         Int32 lSFBSliceID = -1;
         hr = ipCalculationPlan->get_SFBSliceID(lMetricID, iLevel, &lSFBSliceID);
         CHECK_HR;
         
         // 2. Convert CG to CS.
         if (lSFBSliceID != -1)
         {
         // 2-a. Get the CG unit.
         Int32 lCustomGroupUnit = -1;
         hr = lpXTabConsolidation->get_Units(1, &lCustomGroupUnit);
         CHECK_HR;
         
         // 2-b. Check if the level include this CG.
         CComPtr<ICDSSCubeSlice> lpSFBSlice;
         hr = (mpCntxt->mpCube)->get_SliceByID(lSFBSliceID, &lpSFBSlice);
         CHECK_HR;
         
         CComPtr<ICDSSXTabIndices> lpIndices;
         lpSFBSlice->get_Indices(&lpIndices);
         CHECK_HR;
         
         Int32 lnUnit = 0;
         hr = lpIndices->get_NumUnits(&lnUnit);
         CHECK_HR;
         
         Int32 *lpUnit = new (mpBuffer) Int32[lnUnit];
         CHECK_NEWED_PTR(lpUnit);
         
         hr = lpIndices->get_Units(lnUnit, lpUnit);
         CHECK_HR;
         
         bool lfConvert = false;
         for (Int32 iUnit = 0; iUnit < lnUnit; iUnit++)
         {
         if (lpUnit[iUnit] == lCustomGroupUnit)
         {
         lpUnit[iUnit] = lConsolidationUnit;
         lfConvert = true;
         break;
         }
         // lweng, 2012-6-29, TQMS 619397, if the DE elements are re-populated, regenerated the SFB slice
         else if (lpUnit[iUnit] == lConsolidationUnit && lfConsolidate)
         {
         lfConvert = true;
         break;
         }
         }
         
         CDSSSliceID lResultSliceID;
         
         if (lfConvert)
         {
         // 2-c. Generate the slice at new level.
         double lVal = 0;
         // make sure unit level is sorted
         sort(lpUnit, lpUnit + lnUnit);
         
         hr = CDSSCalculationHelper::GetConstantSlice(mpCntxt->mpCube,
         lnUnit, lpUnit,
         reinterpret_cast<unsigned char *>(&lVal),
         sizeof(double),
         DssDataTypeDouble,
         DssDataUnCalculated,
         &lResultSliceID);
         CHECK_HR;
         
         // lweng, 2012-5-17, TQMS 592525
         // 2-d. Set the SFB flag.
         CComPtr<ICDSSCubeSlice> lpResultSlice;
         hr = (mpCntxt->mpCube)->get_SliceByID(lResultSliceID.mSliceID, &lpResultSlice);
         CHECK_HR;
         
         hr = static_cast<CDSSMetricSlice*>(lpResultSlice.p)->SetSFBFlag(true);
         CHECK_HR;
         
         // 3. Updata the SFB slice map
         hr = lpCalculationPlan6->put_SFBSliceID(lMetricID, iLevel, lResultSliceID.mSliceID);
         CHECK_HR;
         }
         }
         }
         }
         */
    }
    
    return S_OK;
}

int CDSSConsolidationEventEvaluator::hCheckViewTemplate(bool *pfViewTemplate)
{
    *pfViewTemplate = true;
    /*
     HRESULT hr = S_OK;
     *pfViewTemplate = false;
     
     DSS_ID lTemplateID, lSourceTemplateID;
     mpCntxt -> mpViewDataSet -> get_TemplateID(&lTemplateID);
     
     CComPtr<ICDSSTemplate> lpSourceTemplate;
     hr = SQIPtr(ICDSSReportInstance10)(mpReportInstance) -> get_SourceTemplate(&lpSourceTemplate);
     CHECK_HR;
     
     // yma 1/19/06, add for view report that has Derived Element defined
     if(lpSourceTemplate == NULL)
     {
     EnumDSSReportType lReportType;
     hr = mpReportInstance->get_ReportType(&lReportType);
     CHECK_HR;
     
     if(lReportType == DssReportTypeSubset)
     {
     *pfViewTemplate = false;
     }
     }
     else
     {
     CComPtr<ICDSSObjectInfo> lpObjectInfo;
     hr = lpSourceTemplate -> QueryInterface(IID_ICDSSObjectInfo, (LPVOID *)&lpObjectInfo);
     CHECK_HR_PTR(lpObjectInfo);
     
     hr = lpObjectInfo -> get_ID(&lSourceTemplateID);
     CHECK_HR;
     
     if(lTemplateID == lSourceTemplateID)
     *pfViewTemplate = false;
     else
     *pfViewTemplate = true;
     }
     */
    return S_OK;
}
/*
 int CDSSConsolidationEventEvaluator::hCheckNonAggMetric(ICDSSObjectInfo* ipViewInfo,
 METRIC_ID &irMetricID,
 Int32 &orFunctionIndex,
 bool &orbHasNonAggFunction)
 {
	HRESULT hr = S_OK;
	orbHasNonAggFunction = false;
 
	DSS_ID lViewID = GUID_NULL;
	hr= ipViewInfo->get_ID(&lViewID);
	CHECK_HR;
 
	CComVariant lVariant;
	CDSSAEHelper::readProperty(mpReportInstance, "SubtotalDimensionalityAware",
 DssPropertyIDSubtotalDimensionalityAware, irMetricID.mMetricID, DssTypeMetric, ipViewInfo, lVariant, &lViewID);
 
	VariantChangeType(&lVariant, &lVariant, 0, VT_BOOL);
 
	if(lVariant.boolVal == VARIANT_TRUE)
	{
 //get EngineLite
 CComPtr<ICDSSSession> lpSession;
 hr = mpSource->get_Session(&lpSession);
 CHECK_HR_PTR(lpSession);
 
 CComPtr<ICDSSComponent> lpDFCEngineLite;
 hr = lpSession->get_Component(DssRoleEngineLite, &lpDFCEngineLite);
 CHECK_HR_PTR(lpDFCEngineLite);
 
 CComQIPtr<ICDSSEngineLite5, &IID_ICDSSEngineLite5> lpEngineLite5(lpDFCEngineLite);
 CHECK_PTR(lpEngineLite5);
 
 //get metric aggregation info
 CComPtr<ICDSSTabularData> lpTabularData;
 hr = (mpCntxt->mpCube) -> get_TabularData(&lpTabularData);
 if(hr != S_OK) return hr;
 
 MAE::Vector<DSS_ID> lvNonAggAttr;
 MAE::Vector<EnumDSSAggregation> lvAggInfo;
 hr = static_cast<CDSSTabularData*> (lpTabularData.p)->GetAggregationInfo(irMetricID, lvNonAggAttr, lvAggInfo);
 CHECK_HR;
 
 //check if the DE is defined on the non-aggregatable dimension
 Int32 lOrgLevel = 1;	//input: original level is {A}
 Int32 lWanted = 0;		//input: target level is {}
 Int32 lResult = 0;		//output: whether "A" is in the final aggregation level, should be 0;
 EnumDSSAggregation lIntermediateAgg = DssAggregationReserved;	//output: aggregation property for "A"
 
 hr = lpEngineLite5->ResolveAggLevelEx3(1, &mID, &lOrgLevel, &lWanted,
 lvNonAggAttr.size(),
 lvNonAggAttr.size() > 0 ? &lvNonAggAttr[0] : NULL,
 lvNonAggAttr.size() > 0 ? &lvAggInfo[0] : NULL,
 (void*)mpLocalSchema,
 &lResult,
 &lIntermediateAgg);
 CHECK_HR;
 
 if (lIntermediateAgg == DssAggregationFirstInFact || lIntermediateAgg == DssAggregationFirstInRelationship)
 {
 orFunctionIndex = DssFuncFirst;
 orbHasNonAggFunction = true;
 }
 else if(lIntermediateAgg == DssAggregationLastInRelationship || lIntermediateAgg == DssAggregationLastInFact)
 {
 orFunctionIndex = DssFuncLast;
 orbHasNonAggFunction = true;
 }
	}
	return S_OK;
 }
 */