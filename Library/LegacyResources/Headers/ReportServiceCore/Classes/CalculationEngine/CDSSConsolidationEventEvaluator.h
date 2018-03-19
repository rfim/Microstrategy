//
//  CDSSConsolidationEventEvaluator.h
//  ReportServiceCore
//
//  Created by Liao Taimin on 4/17/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_CDSSConsolidationEventEvaluator_h
#define ReportServiceCore_CDSSConsolidationEventEvaluator_h

#include "DSSCalculationEventEvaluator.h"

class CDSSConsolidationEventEvaluator : public DSSCalculationEventEvaluator
{
public:
	CDSSConsolidationEventEvaluator();
	CDSSConsolidationEventEvaluator(EnumEventType iEventType);
	~CDSSConsolidationEventEvaluator();
	virtual int ApplyEvent(EventContext *ipCntxt);
    
private:
	int hCheckViewTemplate(bool *pfViewTemplate);
    
//	int hCheckNonAggMetric(ICDSSObjectInfo* lpViewInfo, METRIC_ID &irMetricID, Int32 &orFunctionIndex, bool &obHasNonAggFunction);
    
	EnumEventType mEventType;
};

#endif
