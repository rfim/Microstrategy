//
//  DSSMetricThresholds.h
//  ReportServiceCore
//
//  Created by Zhuo Yang on 11/2/12.
//  Copyright (c) 2012 Microstrategy. All rights reserved.
//

#ifndef ReportServiceCore_DSSMetricThresholds_h
#define ReportServiceCore_DSSMetricThresholds_h

class DSSFunctionServer;
class DSSTemplateMetrics;
class DSSCube;
class DSSCalculationPlan;
class DSSThresholds;
class DSSThreshold;
class EventContext;
class DSSTemplate;
class MCESchema;
class DSSTabularData;
#include "DSSCalculationHelper.h"

class DSSMetricThresholds
{
public:
    DSSMetricThresholds(MCESchema *ipSchema = NULL);
    virtual ~DSSMetricThresholds();
    
    int calculateThreshold(IDSSTemplateModel *ipModel, DSSTabularData *ipTabularData,  DSSCalculationPlan* ipPlan, DSSTemplate* ipTemplate, DSSFunctionServer *ipFunctionServer);
    
private:
    int hEvaluateThresholds4Template(DSSTemplateMetrics *ipTemplateMetrics, DSSCube *ipCube, 
                                     DSSCalculationPlan *ipCalculationPlan, DSSFunctionServer *ipFunctionServer);
    int hEvaluateAttributeThresholds4Template(DSSCube *ipCube, DSSCalculationPlan *ipCalculationPlan, DSSFunctionServer *ipFunctionServer);
    int hEvaluateThreshods(DSSThresholds *ipThresholds, DSSCube *ipCube, int iCurLevel, int iCurEvent, 
                           DSSCalculationPlan *ipCalculationPlan, DSSFunctionServer *ipFunctionServer, int *oSliceID);
    int hEvaluateThresholdCondition(DSSThreshold *ipThreshold, DSSCube *ipCube, int iCurLevel, int iCurEvent, 
                                    DSSCalculationPlan *ipCalculationPlan, 
                                    DSSFunctionServer *ipFunctionServer, EventContext *ipEventContext,
                                    int *opSliceID);
    int hCompareLevel(DSSThreshold *lpThreahold, int iCurLevel, /*object context*/ bool *opNeedCalculation, int ipAutoSubtotal);
    
    int mLevelCount;
    int mMetricCount;

    IDSSTemplateModel *mpModel;
    DSSTabularData* mpTabularData;
    DSSTemplate *mpTemplate;
    MCESchema *mpSchema;
    
    std::vector<DSS_ID> mvThresholdAttrs;
};

#endif
