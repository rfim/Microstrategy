/*
 *  DSSCalculationEventEvaluator.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 1/12/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef CalculationEventEvaluator_h
#define CalculationEventEvaluator_h 1

#import "CalculationEngine/DSSCalculationEvent.h"


class EventContext;
class DSSCalculationPlan;

class DSSCalculationEventEvaluator
{
public:
	DSSCalculationEventEvaluator();
	
	virtual ~DSSCalculationEventEvaluator();
	
/*	int SetEnv(ICDSSReportInstance *ipReportInstance, 
									 ICDSSBindingTable *ipBindingTable,
									 ICDSSSource *ipSource,
									 ICDSSFunctionServer *ipFuncServer,
									 VARIANT_BOOL ifBLInitialized);
*/	
	
	//	Following functions are required in the the definition of ICDSSCalculationEventEvaluator
	//	and they will be implemented by the seperate event classes
	virtual int ApplyEvent(EventContext *ipCntxt) = 0;
	
	
/*	int SetEventContext(	Int32 lLevel, 
										   ICDSSTemplate *lpCDSSTemplate, 
										   ICDSSCube *lpCDSSCube,
										   ICDSSDataSet *pCDSSDataSet,
										   ICDSSViewDataSet *pCDSSViewDataSet );
*/	
protected:
	
//	EventContext *mpCntxt;
	
	
	/*(
	// When used by TemplateSubtotal type of events, remember the index of the 
	// template subtotal
	// When used by CompoundMetric type of events, to indicate whether the compound metric
	// has been searched for component or not. (possible value: SEARCHED or UNSEARCHED)
	MBase::Buffer *mpBuffer;	// needs to be initialized
	
	// Environment varialbes
	bool mbEnvSet;		// true, set; false, not set
	ICDSSSource *mpSource;
	ICDSSBindingTable *mpBindingTable;
	ICDSSReportInstance *mpReportInstance;
	ICDSSFunctionServer *mpFunctionServer;
	// flag to identify if the report base level is initialized
	bool mfBLInitialized;
	
	
	//need to populate the following information
	DSS_ID mID;
	Int32 mEvalOrder;
	
	// When used by TemplateSubtotal type of events, remember the index of the 
	// template subtotal
	// When used by CompoundMetric type of events, to indicate whether the compound metric
	// has been searched for component or not. (possible value: SEARCHED or UNSEARCHED)
	Int32 mEventTag;
	
	Int32 mIdxDoable;		// index-1 in doable event list
	*/
};





#endif
