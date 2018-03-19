//
//  DSSSimpleExprGetMetrics.h
//  ReportServiceCore
//
//  Created by Jun Wang on 2/25/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSSimpleExprGetMetrics_h
#define ReportServiceCore_DSSSimpleExprGetMetrics_h

#include "DSSAEHelper.h"

struct DSSSliceID;
struct METRIC_ID;
struct UnitFormInfo;
struct MetricIDCompare;
class DFCFunc;
class DFCSimpleExpr;
class DFCSimpleExprUnit;
class DFCSimpleExprConst;
class DSSCube;
class DSSCalculationPlan;
class DSSTabularData;
class DSSSimpleExprGetMetrics
{
public:
    
    DSSSimpleExprGetMetrics(MBase::Buffer		*ipBuffer, 
                             DSSCube			*ipSourceCube,
                             DSSCalculationPlan *ipCalculationPlan);
    
    ~DSSSimpleExprGetMetrics();
    
    //entry points of the class
	int GetMetrics(DFCSimpleExpr ** ipExprs,
                                         Int32 inExprs,
                                         vector<METRIC_ID, MBase::Allocator<METRIC_ID> > &oMetricIDs,	
                                         vector<Int32, MBase::Allocator<Int32> > &oMetricSlices,
                                         vector<UnitFormInfo, MBase::Allocator<UnitFormInfo> > &oSelectAttrForms);
	
private:
    
    int hGetMetrics(DFCSimpleExpr *ipExpr);
    
    int  GetMetricsFromSortDefs(DFCFunc *pfunc);
    
    int  hGetMetricFromUnit(DFCSimpleExprUnit *ipUnit);
    
    int  hGetMetricByIndex(Int32 iMetricIndex);
    
    int  hGetMetricByID(const METRIC_ID &iMetricID);
    
    void hInsertSlice(const METRIC_ID &iMetricID, Int32 iSliceID);
    
    
private:
    MBase::Buffer *mpBuffer;
    
    DSSCube *mpSourceCube;
    DSSCalculationPlan *mpCalculationPlan;
    
    DSSTabularData *mpTabularData;
    
    //to remove redundant metric slices, I use slice map	
    //typedef std::map<METRIC_ID, Int32, MetricIDCompare, MBase::Allocator<std::pair<const METRIC_ID, Int32> > >  DSSID2SliceIDMap;
    //DSSID2SliceIDMap mMetricMap; 
    
    // A vector to record the attr and forms that are used in the sort and count definition
    // For example, suppose we want to aggregate 'account balance' from day to month level using the last day's balance
    // The original tabular data contains [Month], [Day], [Balance]
    // CSI is the following
    // select [Month], First (M1) (sort by [Day@ID] descending) group by [Month]
    // In this case, in order to correctly calculate the First(...) expression, we need to 
    // record [Day@ID], although it is not in the selected list.
    
    // deliu, 472397, 07/15/11
    // Attribute Customer also should be recorded in the count metric count([Customer@ID])
    std::vector<UnitFormInfo, MBase::Allocator<UnitFormInfo> > mvSelectUnitFormInfo;
    
    
    
};

#endif
