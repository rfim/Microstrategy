//
//  CDSSDoSort.cpp
//  ReportServiceCore
//
//  Created by Liao Taimin on 5/15/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#include "CDSSDoSort.h"
#import "DSSSortDefinition.h"
#import "DSSCube.h"
#import "DSSCubeDataSlice.h"
#import "DSSTabularIndex.h"
#import "DSSSliceCorrelator.h"
#import "CDSSBucketSort.h"
#import "DSSTabularData.h"
#import "CDSSSingleSliceSort.h"
#import "CDSSAttributeFormSort.h"
#import "DSSStrongPtr.h"

CDSSDoSort::CDSSDoSort(vector<DSSSortDefinition *> *ivpSortDefns) : mvpSortDefns(ivpSortDefns), mpCube(NULL), mnUnit(0), mnRow(0), mpKey(NULL),
/*mpView(NULL),*/ mnMetrics(0),	mnSubtotalTags(0), mpSubtotalTags(NULL), mnUnitRelevant(0), mpUnitRelevant(NULL),
mMetricUnitIndex(0), mpMetricSlice(NULL), mpUnitSubtotalTags(NULL)
{
}

CDSSDoSort::~CDSSDoSort()
{
	for (Int32 i = 0; i < mvSliceCorrelator.size(); i++)
		if (mvSliceCorrelator[i] != NULL)
		{
			delete mvSliceCorrelator[i];
			mvSliceCorrelator[i] = NULL;
		}
    //tliao, 912747, 2014/08/04, fix memory leak
    if (mpKey)
    {
        delete [] mpKey;
    }
    if (mpSubtotalTags)
    {
        delete [] mpSubtotalTags;
    }
    if (mpUnitRelevant)
    {
        delete [] mpUnitRelevant;
    }
    if (mpUnitSubtotalTags)
    {
        delete [] mpUnitSubtotalTags;
    }
}
/*
//Initialization function for Display sorting
HRESULT CDSSDoSort::DisplaySortInit(CDSSCube *ipCube, CDSSXTabView *ipView,
									Int32 nUnit, Int32 nRow, Int32 *pKey,
									Int32 nMetrics,
									Int32 nSubtotalTags, Int32 *pSubtotalTags,
									Int32 nUnitRelevant, Int32 *pUnitRelevant,
									Int32 MetricUnitIndex)
{
    
	mpCube = ipCube;
	mpView = ipView;
    
    
	mnUnit = nUnit;
	//Assert: mnUnit is number of units in the TabularData
	mnRow = nRow;
	mpKey = pKey;
    
	mnMetrics = nMetrics;  // the row number of mpKey = mnMetrics ?  mnRow / mnMetrics : mnRow;
	
	mnSubtotalTags = nSubtotalTags;
	mpSubtotalTags = pSubtotalTags;
	mnUnitRelevant = nUnitRelevant;
	mpUnitRelevant = pUnitRelevant;
	mMetricUnitIndex = MetricUnitIndex;
    
	return S_OK;
    
}
 */

//Initialization function for OLAP sorting, there is no need to consider subtotals, simpler than display sort
Int32 CDSSDoSort::OLAPSortInit(DSSCube *ipCube, DSSTabularIndex *ipIndex, DSSCubeDataSlice *ipMetricSlice)
{
	HRESULT hr = S_OK;
    
	mpCube = ipCube;
    
	DSSTabularData *lpTabularData = NULL;
	lpTabularData = mpCube->getTabularData();
	CHECK_PTR(lpTabularData);
	
	hr = lpTabularData->Count(&mnUnit);
	CHECK_HR;
    
	mnRow = ipIndex->Count();
    
	mpIndex = ipIndex;
    
	mpKey = new Int32 [mnRow * (size_t)mnUnit];
	CHECK_NEWED_PTR(mpKey);
	
	Int32 i = 0;
	for(i = 0; i < mnRow; i++)
	{
		Int32 *lpIndex = mpKey + i * (size_t)mnUnit;
		for(Int32 iUnit = 0; iUnit < mnUnit; iUnit ++)
			lpIndex[iUnit] = DSSXTAB_TOTAL_KEY;
        
		hr = ipIndex->Item(i, mnUnit, lpIndex);
		CHECK_HR;
        
	}
    
	mpMetricSlice = ipMetricSlice;
    
	return S_OK;
    
}

Int32 CDSSDoSort::Sort(Int32 *&opResult)
{
	HRESULT hr = S_OK;
    
	//Initialize sort input
	Int32 *lpInputArray = new Int32 [mnRow];
    
	if (mnRow == 0)		// I don't think we need to do anything if...
	{
		opResult = lpInputArray;
		return S_OK;
	}
    
	Int32 i = 0;
	for (i = 0; i < mnRow; i ++)
	{
		lpInputArray[i] = i;
	}
    
	AE_ASSERT(mvpSortDefns)
	//Iterate through sort defintions. For each sort defintion, construct corresponding
	// bucket sort class, and sort the display rows.
	//Start from the least significant sort defintion
	Int32 lnSort = mvpSortDefns->size();
    
	//pre-allocate the memory space used inside sort
    //tliao, 912747, 2014/08/04, fix memory leak
	Int32 *lpBucketLinkList = new Int32 [mnRow];
    MBase::DSSStrongPtr<Int32, MBase::DeleteArray<Int32> > lBucketLinkListPtr(lpBucketLinkList);
	Int32 *lpAuxArray = new Int32 [mnRow]; //Auxilliary array
    
	for (i = lnSort - 1; i >= 0; i --){
		
		DSSSortDefinition *lpDefn = (*mvpSortDefns)[i];
		CHECK_PTR(lpDefn);
        
		CDSSBucketSort *lpSort = NULL;
		hr = ConstructBucketSort(lpDefn, &lpSort);
		CHECK_HR;
        
		//To save space and time, lpInputArray and lpAuxArray will be switched back
		// and forth inside sort, the sort result is always in lpInputArray
		if (lpSort)
		{
			hr = lpSort->Sort(&lpInputArray, &lpAuxArray, lpBucketLinkList, mnRow);
            //tliao, 912747, 2014/08/04, fix memory leak
            delete lpSort;
			CHECK_HR;
		}
	}
    
	opResult = lpInputArray;
    //tliao, 912747, 2014/08/04, fix memory leak
    delete [] lpAuxArray;
    
	return S_OK;
}

Int32 CDSSDoSort::ConstructBucketSort(DSSSortDefinition *ipSortDef, CDSSBucketSort **opSort)
{
	HRESULT hr = S_OK;
    
	DSSSort_Type lSortType = ipSortDef->sortType;
    /*
	CDSSSliceMapper *lpSliceMap = NULL;
	if (mpView)
	{
		hr = mpView->get_SliceMapperC(&lpSliceMap);
		CHECK_HR_PTR(lpSliceMap);
	}
    */
	//if there is metric unit on this axis, we need think about the number of metrics
	switch(lSortType) {
            
		case DssSortAttributeForm:	//attribute form sort
        {
            CDSSAttributeFormSort *lpAttributeFormSort = new CDSSAttributeFormSort(mnMetrics, mnUnit);
            CHECK_NEWED_PTR(lpAttributeFormSort);
            Int32 lnRow = mnMetrics ?  mnRow / mnMetrics : mnRow;
            hr = lpAttributeFormSort->Init(ipSortDef, mpKey, lnRow, mpCube);
            CHECK_HR;
            
            *opSort = lpAttributeFormSort;
            
        }
            
			break;
            /*
		case DssSortConsolidationDefault:
        {
            CDSSConsolidationSort *lpConsolidationSort = new (mpBuffer) CDSSConsolidationSort(mpBuffer, mnMetrics, mnUnit);
            CHECK_NEWED_PTR(lpConsolidationSort);
            hr = lpConsolidationSort->Init(ipSortDef, mpKey, mpCube);
            CHECK_HR;
            
            *opSort = lpConsolidationSort;
        }
            
			break;
            
		case DssSortCustomGroupDefault:
        {
            CDSSCustomGroupSortDefault *lpCustomGroupSort = new (mpBuffer) CDSSCustomGroupSortDefault(mpBuffer, mnMetrics, mnUnit);
            CHECK_NEWED_PTR(lpCustomGroupSort);
            hr = lpCustomGroupSort->Init(ipSortDef, mpKey, mpCube);
            CHECK_HR;
            
            *opSort = lpCustomGroupSort;
        }
            
			break;
			
		case DssSortGroupByMetric:	//group by metric label
        {
            CDSSMetricLabelSort *lpMetricLabelSort = new (mpBuffer) CDSSMetricLabelSort(mpBuffer, mnMetrics, mnUnit);
            CHECK_NEWED_PTR(lpMetricLabelSort);
            hr = lpMetricLabelSort->Init(ipSortDef);
            CHECK_HR;
            
            *opSort = lpMetricLabelSort;
        }
            
			break;
            
		case DssSortGroupBySubtotal:	//group by subtotal.
        {
            
            CDSSGroupSubtotalSort *lpGroupSubtotalSort = new (mpBuffer) CDSSGroupSubtotalSort(mpBuffer, mnMetrics, mnUnit);
            CHECK_NEWED_PTR(lpGroupSubtotalSort);
            hr = lpGroupSubtotalSort->Init(ipSortDef, mpSubtotalTags, mnSubtotalTags);
            CHECK_HR;
            
            *opSort = lpGroupSubtotalSort;
            
        }
			break;
            */
		case DssSortMetric:	//metric sort
        {
            /*
            VARIANT_BOOL fDispSort;
            hr = ipSortDef -> IsDispSort(&fDispSort);
            if (hr != S_OK)
                return hr;
            if (fDispSort == VARIANT_TRUE)
            {// Display Sort
				
                AE_ASSERT(mpUnitRelevant);
                CDSSMetricValueSort *lpMetricValueSort = new (mpBuffer) CDSSMetricValueSort(mpBuffer, mnMetrics, mnUnit);
                CHECK_NEWED_PTR(lpMetricValueSort);
                
                bool * lpAttrInAxis = new (mpBuffer) bool[mnUnit];
                CHECK_NEWED_PTR(lpAttrInAxis);
                memset(lpAttrInAxis, 0, sizeof(bool) * mnUnit);
                
                for (Int32 i=0; i < mnUnitRelevant; i++){
                    lpAttrInAxis[mpUnitRelevant[i]] = true;
                }
                
                lpAttrInAxis[mMetricUnitIndex] = false;
                
                hr = lpMetricValueSort->Init(ipSortDef, mpKey, mnSubtotalTags, mpCube,
                                             lpSliceMap, lpAttrInAxis, mpSubtotalTags);
                CHECK_HR;
                
                *opSort = lpMetricValueSort;
            } else*/
            { //OLAP sort
                CDSSSingleSliceSort *lpSliceSort = new CDSSSingleSliceSort(mnMetrics, mnUnit);
                CHECK_NEWED_PTR(lpSliceSort);
                
                DSSSliceCorrelator* lpSliceCorrelator = new DSSSliceCorrelator;
                mvSliceCorrelator.push_back(lpSliceCorrelator);
                
                hr = lpSliceSort->Init(ipSortDef, mpIndex, lpSliceCorrelator, NULL, mpCube);
                CHECK_HR;
                
                *opSort = lpSliceSort;
            }
            
        }
            
			break;
            
		case DssSortValue:
        {
            //Come from OALP sort
            if (mpMetricSlice)
            {
                
                CDSSSingleSliceSort *lpSliceSort = new CDSSSingleSliceSort(mnMetrics, mnUnit);
                CHECK_NEWED_PTR(lpSliceSort);
                
                DSSSliceCorrelator *lpSliceCorrelator = new DSSSliceCorrelator;
                mvSliceCorrelator.push_back(lpSliceCorrelator);
                hr = lpSliceSort->Init(ipSortDef, mpIndex, lpSliceCorrelator, mpMetricSlice, mpCube);
                CHECK_HR;
                
                *opSort = lpSliceSort;
                
            } else
            {
                
                *opSort = NULL;
            }
        }
            
			break;
            /*
		case DssSortCustomGroupForm:
        {
            CDSSCustomGroupSort *lpCustomGroupSort = new (mpBuffer) CDSSCustomGroupSort(mpBuffer, mnMetrics, mnUnit);
            CHECK_NEWED_PTR(lpCustomGroupSort);
            hr = lpCustomGroupSort->Init(ipSortDef, mpKey, mpCube);
            CHECK_HR;
            
            *opSort = lpCustomGroupSort;
        }
            
			break;
            */
		case DssSortAttributeColumn:	//From ANA SQL, sort by attribute table column
        {
            CDSSAttributeFormSort *lpAttributeFormSort = new CDSSAttributeFormSort(mnMetrics, mnUnit);
//            CDSSAttributeFormSort *lpAttributeFormSort = new (mpBuffer) CDSSAttributeFormSort(mpBuffer, mnMetrics, mnUnit);
            CHECK_NEWED_PTR(lpAttributeFormSort);
            Int32 lnRow = mnMetrics ?  mnRow / mnMetrics : mnRow;
            hr = lpAttributeFormSort->InitColumn(ipSortDef, mpKey, lnRow, mpCube);
            CHECK_HR;
            
            *opSort = lpAttributeFormSort;
            
        }
            
			break;
            /*
		case DssSortUnitSubtotal: //Page-by axis cube total sort
        {
            
            if (mpUnitSubtotalTags == NULL)
                mpUnitSubtotalTags = new (mpBuffer) Int32[mnSubtotalTags];
            
            Int32 lUnitIndex = ipSortDef->UnitIndex();
            AE_ASSERT(lUnitIndex >= 0);
            
            for (Int32 i = 0; i < mnSubtotalTags; i ++)
            {
                if (mpSubtotalTags[i] == 0)
                    mpUnitSubtotalTags[i] = 0; //no subtotal
                else {
                    //need to check whether it is a subtotal for this unit
                    if (mpKey[i * mnUnit + lUnitIndex] == DSSXTAB_TOTAL_KEY)
                        mpUnitSubtotalTags[i] = mpSubtotalTags[i]; // it is a total for this unit element
                    else 
                        mpUnitSubtotalTags[i] = 0; //no subtotal
                    
                }
            }
            
            CDSSGroupSubtotalSort *lpGroupSubtotalSort = new (mpBuffer) CDSSGroupSubtotalSort(mpBuffer, mnMetrics, mnUnit);
            CHECK_NEWED_PTR(lpGroupSubtotalSort);
            hr = lpGroupSubtotalSort->Init(ipSortDef, mpUnitSubtotalTags, mnSubtotalTags);
            CHECK_HR;
            
            *opSort = lpGroupSubtotalSort;
        }
            
			break;
            
		case DssSortDerivedElementDefault:
		case DssSortDerivedElementForm:
        {
            CDSSDerivedElementSort *lpDESort = new (mpBuffer) CDSSDerivedElementSort(mpBuffer, mnMetrics, mnUnit);
            CHECK_NEWED_PTR(lpDESort);
            hr = lpDESort->Init(ipSortDef, mpKey, mpCube);
            CHECK_HR;
            
            *opSort = lpDESort;
        }
            
			break;
            */
            
		default:
            AE_ASSERT(false);	// something wrong here
            break;
    }
    
	return hr = S_OK;
    
}