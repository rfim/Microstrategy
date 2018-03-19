//
//  DSSSubsetEngine.h
//  ReportServiceCore
//
//  Created by Taimin Liao on 2/21/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSSubsetEngine_h
#define ReportServiceCore_DSSSubsetEngine_h

#include "DSSAEHelper.h"

class DSSFunctionServer;
class DSSGUnitContainer;
class DSSViewDataSet;
class DSSTabularIndex;
class DSSCalculationPlan;
class DSSMaskInfos;
class DSSCubeDataSlice;
class DSSCube;
struct METRIC_ID;
class DSSTabularIndexPool;
class DSSTabularUnit;
struct DSSSliceID;
class DSSXTabBitVector;
class DSSTabularData;
class DSSObjectContext;
class MCESchema;
class DFCCSI;
class DFCSimpleExpr;

class DSSGroupByUnit;//xiazhou, offline UC

class DSSSubsetEngine
{
public:
    DSSSubsetEngine(MBase::Buffer *ipBuffer, DSSGUnitContainer *ipGlobalLookupContainer, DSSObjectContext  *ipSource, MCESchema *ipSchema);
    ~DSSSubsetEngine();
    
    int SubsetCSI(DSSViewDataSet *ipBaseDataset, DFCCSI *ipCSI, map<DSS_ID, DSSGroupByUnit *> &iGBUnitMap, DSSTabularData *opTabularData, DSSCube *opCube,DSSCalculationPlan* ipCalcPlan, bool IsTransactionEnabled, bool IsOfflineUC = false, bool IsOfflineDDIC = false);
    
    
private:
    int hRemoveDuplicateMetrics(int lnMetrics, METRIC_ID* ipMetricIDs, DFCSimpleExpr** ipMetricExprs, int* onDistinct, bool* ipConflictFlag);
    int hCalculateMask(DSSCube *ipCube, DFCSimpleExpr* ipFilter, DSSCalculationPlan* ipCalPlan, DSSMaskInfos &irMaskInfos, map<DSS_ID, DSSGroupByUnit *> &iGBUnitMap, bool ibFilterWithHiddenDE = false, bool ibEvaluateConditionalMetric = false, bool IsOfflineUC = false);
    int hAdjustSliceLevel(DSSCube *ipCube, Int32 nTargetLevel, Int32 *pTargetLevel, Int32 nMetric, DSSCubeDataSlice **ippSlices);
    int hProcessSFBMetrics(DSSCube* ipCube, Int32 nSlice, DSS_ID* pOriginalMetricID, Int32* pMetricLevel, Int32* pGeneratedMetricID);
    int hFilterTabularData( DSSCube*	ipCube,					// the cube that contains the data to be populated
                            DFCSimpleExpr* ipFilterExpr,		// the filter to be applied
                            DSSCubeDataSlice *ipFilterSlice,	// the filter slice to be applied
                            DSSXTabBitVector *ipMask,			// if passed in, it is a bit vector at the original tabulardata main index level
                            DSSCalculationPlan* ipCalPlan,      // the calculation plan so that we can translate Metric to slice
                            vector<UnitFormInfo> &ivUnits,		// the units (attribute in our case)to be populated
                            vector<DSSSliceID>	 &ivSlices,		// the slices to be populated
                            DSSTabularData** oppResult,         // the filtered tabular data
                            vector<UnitFormInfo, MBase::Allocator<UnitFormInfo> > &lvSortAttrForm,
                            int &lnAggLevel,
                            vector<METRIC_ID, MBase::Allocator<METRIC_ID> > &lInputMetricIDs,
                            bool ibFilterWithHiddenDE,
                            Int32 NumDerivedElements,
                            Int32 *ipDerivedElementUnits);
    int hGetFilterSlice(DSSCube* ipCube,					// the cube to be filtered
                        DFCSimpleExpr* ipFilter,			// the filter to be applied
                        DSSCalculationPlan* ipCalPlan,		// the calculation plan
                        DSSCubeDataSlice** oppFilterSlice,	// the output
                        DSSCube** oppTempCube,				// the TempCube that contains the FilterSlice
                        bool ibFilterWithHiddenDE);
    
    MBase::Buffer *mpBuffer;
    DSSFunctionServer *mpFunctionServer;
    DSSGUnitContainer *mpGlobalLookupContainer;
    DSSObjectContext  *mpSource;
    
    MCESchema *mpSchema;
};

#endif
