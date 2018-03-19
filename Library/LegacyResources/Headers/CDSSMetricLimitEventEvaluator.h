//
//  CDSSMetricLimitEventEvaluator.h
//  ReportServiceCore
//
//  Created by Taimin Liao on 4/1/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_CDSSMetricLimitEventEvaluator_h
#define ReportServiceCore_CDSSMetricLimitEventEvaluator_h

struct DSSSliceID;

class EventContext;
class DSSCalculationPlan;

#import "CalculationEngine/DSSCalculationEvent.h"
#include "DSSCalculationEventEvaluator.h"

class CDSSMetricLimitEventEvaluator : public DSSCalculationEventEvaluator
{
public:
	CDSSMetricLimitEventEvaluator();
	~CDSSMetricLimitEventEvaluator();
	virtual int ApplyEvent(EventContext *ipCntxt);
	int GetEventType(EnumEventType *pEventType);
	int SetAbellEvalOrder();
    
private:
	int hEvaluateMetricLimitCondition(EventContext *ipCntxt,
                                      DSSCube *ipTargetCube,
                                      DSSCalculationPlan *ipCalculationPlan,
                                      Int32 iSessionId,
                                      DSSSliceID *opResultSliceID);
    
	int hRemoveDuplicateMetrics(Int32 lnMetrics, 
                                METRIC_ID* ipMetricIDs,         
                                Int32* opMetricMap, 
                                Int32* onDistinctMetrics, 
                                Int32** oppDistinctMetrics);
    
};

#endif
