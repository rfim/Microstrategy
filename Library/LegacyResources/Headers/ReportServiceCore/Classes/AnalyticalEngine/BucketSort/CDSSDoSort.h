//
//  CDSSDoSort.h
//  ReportServiceCore
//
//  Created by Liao Taimin on 5/15/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_CDSSDoSort_h
#define ReportServiceCore_CDSSDoSort_h

#include "DSSAEHelper.h"

class DSSSortDefinition;
class DSSCube;
class DSSTabularIndex;
class DSSCubeDataSlice;
class CDSSBucketSort;
class DSSSliceCorrelator;

class  CDSSDoSort
{
    
public:
	//Constructor
	CDSSDoSort(vector<DSSSortDefinition *> *ivpSortDefns);
	//Destructor
	~CDSSDoSort();
    /*
	HRESULT DisplaySortInit(CDSSCube *ipCube, CDSSXTabView *ipView,
							Int32 nUnit, Int32 nRow, Int32 *pKey, Int32 nMetrics,
							Int32 nSubtotalTags, Int32 *pSubtotalTags,
							Int32 nUnitRelevant, Int32 *pUnitRelevant,
							Int32 MetricUnitIndex);
    */
	Int32 OLAPSortInit(DSSCube *ipCube, DSSTabularIndex *ipIndex, DSSCubeDataSlice *ipMetricSlice);
    
	Int32 Sort(Int32 *&opResult);
    
	Int32 ConstructBucketSort(DSSSortDefinition *ipSortDef, CDSSBucketSort **opSort);

private:
    
	vector<DSSSortDefinition *> *mvpSortDefns;
    
	DSSCube *mpCube;
    
    
	Int32 mnUnit;
	Int32 mnRow;
	Int32 *mpKey;
    
    
	//For display sort only
//	DSSXTabView *mpView;
	Int32 mnMetrics;
	Int32 mnSubtotalTags;
	Int32 *mpSubtotalTags;
	Int32 mnUnitRelevant;
	Int32 *mpUnitRelevant;
	Int32 mMetricUnitIndex;
    
	//For OALP sort only
	DSSCubeDataSlice *mpMetricSlice;
	DSSTabularIndex *mpIndex;
    
	//For page-by axis cube total sort
	Int32 *mpUnitSubtotalTags;
    
	vector<DSSSliceCorrelator*> mvSliceCorrelator;
    
};

#endif
