/*
 *  DSSCompoundMetricEventEvaluator.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 1/12/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#include "DSSCompoundMetricEventEvaluator.h"
#import "CalculationEngine/DSSCalculationPlan.h"
#import "CalculationEngine/DSSDimMetricEvaluator.h"

DSSCompoundMetricEventEvaluator::DSSCompoundMetricEventEvaluator()
{
}

DSSCompoundMetricEventEvaluator::~DSSCompoundMetricEventEvaluator()
{
}

int DSSCompoundMetricEventEvaluator::ApplyEvent(EventContext *ipCntxt)
{
	int hr = S_OK;
	
	DSSCalculationPlan *lpCalcPlan = ipCntxt->mpCalculationPlan;
	int iLevel = ipCntxt->mLevel;
	int iEvent = ipCntxt->mpEvent->mEventIndex;
	
	int lnNumMetrics = lpCalcPlan->getNumMetrics();
	CHECK_HR;
	
	int  iMetric = 0;
	GUID lMetricID = GUID_NULL;
	
	for(iMetric = 0; iMetric < lnNumMetrics; iMetric ++)
	{
		bool  lfIsCalculated = 0;
		DSSSliceID lSliceID;
		hr = lpCalcPlan->getMetricSliceID(iLevel,
										  iEvent,
										  iMetric,
										  &lfIsCalculated,
										  &(lSliceID.mSliceID));
		CHECK_HR;
		
		if(lfIsCalculated)
			continue;
		
		//need to get this metric and look at if the metric is the same or not
		lMetricID = lpCalcPlan->getMetricID(iMetric);
		
		if (!MBase::IsEqualGUID(ipCntxt->mpEvent->mID, lMetricID))
		{
			//this metric does not need to be evaluate for this event
			//just write the slice ID down
			
			hr = lpCalcPlan->getMetricSliceID(iLevel,
											   iEvent - 1,
											   iMetric,
											   &lfIsCalculated,
											   &(lSliceID.mSliceID));
			CHECK_HR;
		}
		else
		{
			DSSDimMetricEvaluator lDimMetricEvaluator(ipCntxt, lpCalcPlan->getNullCheckingOption(iMetric));
			
            lSliceID.mSliceID = -1;
			hr = lDimMetricEvaluator.CalculateDimMetric(lMetricID, &lSliceID);
			lfIsCalculated = (hr == S_OK && lSliceID.mSliceID >= 0);
            if (!lfIsCalculated)
                lSliceID.mSliceID = -1;
		}
		
		hr = lpCalcPlan->setMetricSliceID(iLevel,
										  iEvent,
										  iMetric,
										  lfIsCalculated,
										  lSliceID.mSliceID);
		CHECK_HR;
		
	}
	return S_OK;
}
