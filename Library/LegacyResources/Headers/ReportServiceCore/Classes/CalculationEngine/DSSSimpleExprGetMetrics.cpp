//
//  DSSSimpleExprGetMetrics.cpp
//  ReportServiceCore
//
//  Created by Jun Wang on 2/25/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#include "DSSSimpleExprGetMetrics.h"

#import "DSSCube.h"
#import "DSSViewDataSet.h"
#import "DSSTabularData.h"
#import "DSSTabularUnit.h"

#include "SimpleExpr/DFCSimpleExpr.h"
#include "SimpleExpr/DFCSimpleExprUnit.h"
#include "SimpleExpr/DFCSimpleExprRelation.h"
#include "SimpleExpr/DFCSimpleExprPos.h"
#include "SimpleExpr/DFCSimpleExprBranch.h"


DSSSimpleExprGetMetrics::DSSSimpleExprGetMetrics(MBase::Buffer		*ipBuffer, 
                                                  DSSCube			*ipSourceCube,
                                                  DSSCalculationPlan *ipCalculationPlan)
: mpBuffer(ipBuffer),  mpSourceCube(ipSourceCube), mpCalculationPlan(ipCalculationPlan)
//, mMetricMap(MetricIDCompare(), MBase::Allocator<std::pair<const METRIC_ID, Int32> > (ipBuffer) )
{
}


DSSSimpleExprGetMetrics::~DSSSimpleExprGetMetrics()
{
}


//The main calculation logic, refactored from DFCCalculationServer, Ana SQL evaluation.
int DSSSimpleExprGetMetrics::GetMetrics(
                                                               DFCSimpleExpr ** ipExprs, Int32 inExprs,
                                                               vector<METRIC_ID, MBase::Allocator<METRIC_ID> > &oMetricIDs,					   
                                                               vector<Int32, MBase::Allocator<Int32> > &oMetricSlices,
                                                               vector<UnitFormInfo, MBase::Allocator<UnitFormInfo> > &oSelectAttrForms)
{
    
	//Initialize first
    mpTabularData = mpSourceCube->getTabularData();
    CHECK_PTR(mpTabularData);
    
	//Get input metric inputs
	Int32 i = 0;
	for (i = 0; i < inExprs; i ++) 
	{
		int hr = hGetMetrics(ipExprs[i]);
		CHECK_HR;
	}
    /*
	for (DSSID2SliceIDMap::iterator lMetricIterator = mMetricMap.begin(); 
         lMetricIterator != mMetricMap.end(); lMetricIterator ++){
		oMetricIDs.push_back((*lMetricIterator).first);
		oMetricSlices.push_back((*lMetricIterator).second);
	}
    */
	oSelectAttrForms = mvSelectUnitFormInfo;
	return S_OK;
}

int DSSSimpleExprGetMetrics::hGetMetrics(DFCSimpleExpr *ipExpr)
{
    
	AE_ASSERT(ipExpr);
    int hr = S_OK;
	switch(ipExpr->GetExprType())
	{
        case DFCSimpleExpr::SIMPLE_EXPR_BRANCH:	//DFCSimpleExprBranch
            
		{
			DFCSimpleExprBranch * lpBranch = static_cast<DFCSimpleExprBranch *>(ipExpr);
			DFCFunc * lpFunc = lpBranch->GetFunc();
			hr = GetMetricsFromSortDefs(lpFunc);
			CHECK_HR;
            
            
			//here should recursive call its children
			vector<DFCSimpleExpr *, MBase::Allocator<DFCSimpleExpr *> > lChildren(lpBranch -> GetArgs());
            
			Int32 i = 0;
			Int32 lnChild = lChildren.size();
			for(i = 0; i < lnChild; i++)
			{
				hr = hGetMetrics(lChildren[i]);
				CHECK_HR;
			}
            
		}
            
            break;
            
            
        case DFCSimpleExpr::SIMPLE_EXPR_POS:	//DFCSimpleExprPos
		{
			DFCSimpleExprPos * lpPos = static_cast<DFCSimpleExprPos *>(ipExpr);
            
			Int32 iSource = 0,  iUnit = 0 ,  iForm = 0 ;
			lpPos -> GetPos(iSource, iUnit, iForm);
            
			// xhan iForm is the metric ID in tabular data
			hr = hGetMetricByIndex(iForm);
			CHECK_HR;
            
		}
            
            break;
            
        case DFCSimpleExpr::SIMPLE_EXPR_CONST:	//DFCSimpleExprConst
            break;
            
        case DFCSimpleExpr::SIMPLE_EXPR_UNIT:
		{
			DFCSimpleExprUnit * lpUnit = static_cast<DFCSimpleExprUnit *>(ipExpr);
            
			//The unit can be a metric or a attribute unit 
			bool lIsMetric = lpUnit->IsMetric();
			if (lIsMetric)
			{
				hr = hGetMetricFromUnit(lpUnit);
				CHECK_HR;
                
			} 
			else	// deliu, 472397, 07/15/11
			{
				// record the attribute and form
				DSS_ID lAttrID = GUID_NULL;
				MBase::Allocator<Int32> lAlloc(mpBuffer);
				vector<Int32, MBase::Allocator<Int32> > lAttrForms(lAlloc);
				vector<Int32, MBase::Allocator<Int32> > lLocales(lAlloc);
				Int32 lSourceID = 0;
                
				lpUnit->GetAttrUnit(lSourceID, lAttrID, lAttrForms, lLocales);
                
				Int32 lUnitIndex = 0;
				hr = mpTabularData->FindUnitI(lAttrID, DssTemplateReserved, lUnitIndex);
				CHECK_HR;
                
				UnitFormInfo lUnitFormInfo;
				lUnitFormInfo.UnitID = lUnitIndex;
				lUnitFormInfo.nForms = lAttrForms.size();
				lUnitFormInfo.Forms = &lAttrForms[0];
				lUnitFormInfo.Locales = &lLocales[0];
                
				mvSelectUnitFormInfo.push_back(lUnitFormInfo);
			}
		}
            
            break;
        case DFCSimpleExpr::SIMPLE_EXPR_RELATION:	//DFCSimpleExprRelation
		{
			DFCSimpleExprRelation * lpBranch = static_cast<DFCSimpleExprRelation *>(ipExpr);
			//here should recursive call its children
			hr = hGetMetrics(lpBranch->GetAttributes());
			CHECK_HR;
			hr = hGetMetrics(lpBranch->GetFilter());
			CHECK_HR;
            
		}
            
            break;
        default:
            _ASSERTE(false);
            return hr =  E_FAIL;
	}
    
    
	return hr;
    //## end CDSSSimpleExprGetMetrics::EvalExprOneCube%351AA4B8036F.body
}


// Additional Declarations
//## begin CDSSSimpleExprGetMetrics%35156D5A01F5.declarations preserve=yes
int DSSSimpleExprGetMetrics::GetMetricsFromSortDefs(DFCFunc *pfunc)
{
    AE_ASSERT(pfunc);
    int hr = S_OK;
    if ( pfunc->IsSortDependent() )
    {
        // get sort definition
        vector<DFCSimpleExpr*,MBase::Allocator<DFCSimpleExpr*> > &SortLevel = pfunc->GetSortLevel();
        vector<bool, MBase::Allocator<bool> > &SortOrder = pfunc->GetSortOrder();
        _ASSERT( SortLevel.size() == SortOrder.size() );
        
        Int32 lLocale = 0;
        /*
        lLocale = MFormat::GetUserDefaultLCID();
        */
        
        Int32 i = 0;
        for (i = 0; i < SortLevel.size(); i++)
        {
            
            if (i == 0)
                lLocale = SortLevel[i] -> GetLocale();
            
            switch(SortLevel[i] -> GetExprType())
            {
                case DFCSimpleExpr::SIMPLE_EXPR_BRANCH:
                {
                    hr = hGetMetrics(SortLevel[i]);
                    CHECK_HR;
                    
                }
                    break;
                    
                case DFCSimpleExpr::SIMPLE_EXPR_POS:
                {
                    
                    //now it has to be
                    Int32 Source = 0,  Unit = 0 ,  Form = 0 ;
                    static_cast<DFCSimpleExprPos *>(SortLevel[i])->GetPos(Source, Unit, Form);
                    
                    
                    DSSTabularUnit* pUnit = NULL;
                    hr = mpTabularData->Item(Unit, &pUnit);
                    CHECK_HR_PTR(pUnit);
                    
                    DSSTemplateUnit_Type pType = pUnit->getUnitType();
                    CHECK_HR;
                    
                    switch (pType)
                    {
                        case	DssTemplateMetrics:
                        {
                            hr = hGetMetricByIndex(Form);
                            CHECK_HR;
                        }
                            break;
                            
                        case	DssTemplateAttribute:
                            
                            break;
                            
                        case	DssTemplateConsolidation:
                        case	DssTemplateCustomGroup:	// not handled yet
                        case	DssTemplateDimension:	// should never happen
                        default :
                            _ASSERTE(false);
							return hr =  E_FAIL;
                    }
                }
                    break;
                    
                case DFCSimpleExpr::SIMPLE_EXPR_UNIT:
                {
                    DFCSimpleExprUnit * lpUnit = static_cast<DFCSimpleExprUnit *>(SortLevel[i]);
                    
                    //The unit can be a metric or a attribute unit 
                    bool lIsMetric = lpUnit->IsMetric();
                    if (lIsMetric)
                    {
                        hr = hGetMetricFromUnit(lpUnit);
                        CHECK_HR;
                    } else
                    {
                        // record the attribute and form
                        DSS_ID lAttrID = GUID_NULL;
                        MBase::Allocator<Int32> lAlloc(mpBuffer);
                        vector<Int32, MBase::Allocator<Int32> > lAttrForms(lAlloc);
                        vector<Int32, MBase::Allocator<Int32> > lLocales(lAlloc);
                        Int32 lSourceID = 0;
                        
                        lpUnit->GetAttrUnit(lSourceID, lAttrID, lAttrForms, lLocales);
                        
                        Int32 lUnitIndex = 0;
                        hr = mpTabularData->FindUnitI(lAttrID, DssTemplateReserved, lUnitIndex);
                        CHECK_HR;
                        
                        UnitFormInfo lUnitFormInfo;
                        lUnitFormInfo.UnitID = lUnitIndex;
                        lUnitFormInfo.nForms = lAttrForms.size();
                        lUnitFormInfo.Forms = &lAttrForms[0];
                        lUnitFormInfo.Locales = &lLocales[0];
                        
                        mvSelectUnitFormInfo.push_back(lUnitFormInfo);
                    }
                    
					break;
                }
                    
                default:
                    return hr =  E_FAIL;
            }
        }
        
    }
    
    return hr = S_OK;
}

int  DSSSimpleExprGetMetrics::hGetMetricFromUnit(DFCSimpleExprUnit *ipUnit)
{
    //If the unit is a metric, then get metric slice
    METRIC_ID lMetricID;
    lMetricID.mDatasetID = GUID_NULL;
    // TODO dataset.MetricID
    Int32 lSourceID = 0;
    ipUnit->GetMetricUnit(lSourceID, lMetricID.mMetricID);
    //ipUnit->GetSourceInfo(lSourceID, lMetricID.mDatasetID, NULL);
    
    return hGetMetricByID(lMetricID);
}

int  DSSSimpleExprGetMetrics::hGetMetricByIndex(Int32 iMetricIndex)
{
	int hr = S_OK;
    
	DSSTabularMetrics* lpMetrics = NULL;
	hr = mpTabularData->getMetrics(&lpMetrics);
	CHECK_HR_PTR(lpMetrics);
    
	METRIC_ID lMetricID;
	/*
    hr = lpMetrics->get_MetricIDIEx(iMetricIndex, &lMetricID.mDatasetID, &lMetricID.mMetricID);
     */
	CHECK_HR;
    
	hr = hGetMetricByID(lMetricID);
	CHECK_HR;
    
	return hr;
    
}

int  DSSSimpleExprGetMetrics::hGetMetricByID(const METRIC_ID &iMetricID)
{
    /*
	int hr = S_OK;
	
	//Suppose SIMPE_EXPR_UNIT is from CSI, mpCalculationPlan must be initialized
    
	AE_ASSERT(mpCalculationPlan);
	SQIPtr(ICDSSCalculationPlan6) lpCalcPlan6(mpCalculationPlan);
    
	Int32 lMetricIndex = 0;
	Int32 lSliceID = -1;
	hr = lpCalcPlan6->get_MetricIndexEx(iMetricID.mDatasetID, iMetricID.mMetricID, &lMetricIndex);
	if (hr == S_OK) 
	{
        
		Int32 lBaseNumEvents = 0;
		hr = lpCalcPlan6->get_NumEvents(&lBaseNumEvents);
		CHECK_HR;
        
		Int32 lfIsCalculated = 0;
        
		//last event is considered as result
		hr = lpCalcPlan6->get_MetricSliceID(
                                            0,
                                            lBaseNumEvents - 1,
                                            lMetricIndex,
                                            &lfIsCalculated,
                                            &lSliceID);
        
        
		// aye, 7/8/2009
		// if the slice is not calculated, we should treat it as if it's not there in the
		// calculation plan, instead of failing the report execution.
		if (lfIsCalculated)
			hInsertSlice(iMetricID, lSliceID);
	}
    //else { metric is not found in the calculation plan, skip it;}
	
	*/
    
	return S_OK;
}

void DSSSimpleExprGetMetrics::hInsertSlice(const METRIC_ID &iMetricID, Int32 iSliceID)
{
    /*
	DSSID2SliceIDMap::iterator lMetricIterator = mMetricMap.find(iMetricID);
	if (lMetricIterator == mMetricMap.end())
		//slice id not in map yet, insert it
		mMetricMap.insert(std::pair<METRIC_ID, Int32> (iMetricID, iSliceID) );
     */
    return;
    
}