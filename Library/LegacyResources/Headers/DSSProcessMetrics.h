/*
 *  DSSProcessMetrics.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 1/8/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef ProcessMetrics_h
#define ProcessMetrics_h 1

#include <cstddef>

class DSSCalculationPlan;
class DSSCalculationEvent;
class DSSCalculationEventEvaluator;
class IDSSTemplateModel;
class MCESchema;
class DSSViewDataSet;

class DSSProcessMetrics
{
public:
	DSSProcessMetrics(DSSCalculationPlan *ipPlan, IDSSTemplateModel *ipModel = NULL, MCESchema *ipLocalSchema = NULL, DSSViewDataSet* ipCube = NULL);
	
	~DSSProcessMetrics();
	//xiazhou, 04/23/2014, to support offline UC.
	int CalculateMetrics(bool isSmartClient = false, bool isOfflineUC = false);
	
private:
	int hCalcMetricsInDeftOrder(bool isSmartClient, bool isOfflineUC);
	
	int hCalcMetricsInExtdOrder(bool isSmartClient, bool isOfflineUC);
	
	int hSkipEvent(int iEvent, int iLevel);
	
	DSSCalculationEventEvaluator* hMakeCalculationEventEvaluator(DSSCalculationEvent *pEvent);
	
	void DestroyCalculationEventEvaluator(DSSCalculationEventEvaluator * pEventEvaluator);
	
	DSSCalculationPlan *mpCalculationPlan;
    
	IDSSTemplateModel *mpDataModel;
    MCESchema *mpLocalSchema;
    DSSViewDataSet* mpViewDataset;
};

#endif
