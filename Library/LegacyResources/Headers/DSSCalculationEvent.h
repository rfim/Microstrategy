/*
 *  DSSCalculationEvent.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 1/12/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef CalculationEvent_h
#define CalculationEvent_h

#define AbellOriginalMetricOrder -4
#define AbellSubtotalOrder       -3
#define AbellCompoundMetricOrder -2
#define AbellMetricLimitOrder    INT_MAX

#import "CalculationEngine/DSSCalculationHelper.h"

typedef enum EnumEventType
{
	DssEventTypeReserved	= 0,
    DssEventOriginalMetric	= 1,
    DssEventCompoundMetric	= 2,
    DssEventConsolidation	= 3,
    DssEventMetricLimit		= 4,
    DssEventTemplateSubtotal= 5,
    DssEventDerivedElement	= 6,
    DssEventLastOne
} EnumEventType;

class DSSCalculationEvent
{
public:
	DSSCalculationEvent(){};
	
	virtual ~DSSCalculationEvent(){};
	
//TODO: private:
	EnumEventType mEventType;
	GUID mID;
//	int mEvalOrder;
	int mEventTag;		//subtoal Index for Subtotal event, and a flag in compound metric event
	int mEventIndex;	//mIdxDoable, doable event id
};
/*
class CompoundMetricEvent : public DSSCalculationEvent
{
	
};

class SubtotalEvent : public DSSCalculationEvent
{
	
};*/

#endif
