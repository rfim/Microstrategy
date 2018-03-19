/*
 *  DSSCompoundMetricEventEvaluator.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 1/12/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef CompoundMetricEventEvaluator_h
#define CompoundMetricEventEvaluator_h 1

#import "CalculationEngine/DSSCalculationEventEvaluator.h"

class DSSCompoundMetricEventEvaluator : public DSSCalculationEventEvaluator
{
public:
	DSSCompoundMetricEventEvaluator();
	~DSSCompoundMetricEventEvaluator();
	//virtual functions
	virtual int ApplyEvent(EventContext *ipCntxt);
};

#endif // CompoundMetricEventEvaluator_h
