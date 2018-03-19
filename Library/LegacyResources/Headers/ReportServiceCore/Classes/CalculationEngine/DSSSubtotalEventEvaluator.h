/*
 *  DSSSubtotalEventEvaluator.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 1/12/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef SubtotalEventEvaluator_h
#define SubtotalEventEvaluator_h 1

#import "CalculationEngine/DSSCalculationEventEvaluator.h"

class DSSSubtotalEventEvaluator : public DSSCalculationEventEvaluator
{
public:
	DSSSubtotalEventEvaluator();
	
	~DSSSubtotalEventEvaluator();
	
	int ApplyEvent(EventContext *ipCntxt);
    
    static int InitReportBaseLevel(DSSTemplate *ipTemplate,
                                   DSSTabularData *ipTabularData,
                                   //MBase::Buffer *ipBuffer,
                                   std::vector<Int32 *>  *oppvAttrInAxis,
                                   std::vector<Int32> *opvAttrInAxis,
                                   std::vector<Int32> *opAxisIndex);
	
private:
	int hEvaluateSubtotal(EventContext *ipCntxt,
						  EnumDSSMetricType iTotalImpl,
						  int iMetricID,
						  vector<int> &irvUnits,
                          bool iUseContainer,
						  DSSSliceID *opSliceID);
    int ExpandSlice(DSSCube * ipCube, int iSliceID, DSSSliceID *opSliceID);

	std::vector<Int32 *> mvpAttrInAxis;
	std::vector<Int32> mvnAttrInAxis;
	std::vector<Int32> mvnAxisIndex;
};

#endif // CDSSSubtotalEventEvaluator_h
