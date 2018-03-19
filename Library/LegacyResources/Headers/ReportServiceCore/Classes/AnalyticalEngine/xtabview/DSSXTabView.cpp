/*
 *  DSSXTabView.cpp
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import "DSSXTabView.h"
#import "DSSXTabHeader.h"
#import "DSSCubeDataSlice.h"
#import "DSSDataModelXTabImpl.h"

#import "DSSDataColumn.h"
#import "DSSCube.h"
#import "DSSTabularCustomGroup.h"
#import "DSSTabularConsolidation.h"
#import "DSSTabularAttribute.h"
#import "DSSTabularMetrics.h"
#import "DSSXTabRefLookupTable.h"
#import "DSSXTabRecursiveUnit.h"

#import "DSSRWGridModel.h"
#include "DSSTabularIndexPool.h"
#include "DSSTabularIndex.h"
#include "DSSTabularData.h"
#include "DSSViewDataSet.h"
#include "DSSXTabDispInfo.h"
#include "DSSSliceMapper.h"
#include "DSSXTabLayoutInfo.h"
#include "DSSXTabLayoutUnitInfo.h"
#import "DSSStrongPtr.h"
#include "DSSMaskInfos.h"

#include "DSSXTabBitVector.h"
#include "DSSSortConverter.h"
#include "DSSTemplate.h"
#include "DSSSortDefinition.h"
#include "DSSSorter.h"
#include "DSSNewRWDSorter.h"

#include "DSSGBTable.h"

#include "DSSAttributeThresholdIterator.h"
#include "DSSAttributeThresholdItems.h"

#include "MsiCOM/MsiCOM/Msi_atlbase.h"
#include <algorithm>
#include "DSSSort.h"
#include "DSSSorts.h"
#define LABLE_NO_METRIC -1	//definitely this can not be a right index to a metric

// tehe: we don't consider page by axis at ipad side
#define AXIS_COUNT 2

DSSXTabView::DSSXTabView(): mpKeyBuffer(NULL)
,mnLevels(0), mnRowLevels(0), mnColLevels(0), mnMetrics(0), mnTotalUnits(0)
,mpXTabModel(NULL),mpSliceMap(NULL),mpCube(NULL), mfStatus(XTAB_UNINITIALIZED),mCurIndex(NULL),
mCurMetric(LABLE_NO_METRIC), mCurTag(0), mColCount(0), mRowCount(0),mDummyUnitIndex(-1),
mHasCellFormat(0),mfInitInternal(false),mSizeSliceLevel(0),
mpSliceLevel(NULL),mbHasMetricGroupBy(false),mbHasValidGroupBy(false),mnGroupByUnits(0),
mHasInitialized(false),mpvLastGBElements(NULL),mpViewDataSet(NULL),mpGroupByUnits(NULL),mppvMapGBPos2LevelSlice(NULL),
mpGroupByUnitsPos(NULL), mpReportLevelIndex(NULL), mbConsoBVCalculated(false),mpGBTable(NULL),mpOVGBUnits(NULL), mpOVGBUnitPos(NULL), mpvLastOVGBElements(NULL),mpHideNullZeroMaskInfo(NULL),mpAttributeThresholdIterator(NULL),mpAttributeThresholdItems(NULL)
{
    // mHeaders = new DSSXTabHeader*[HEADCOUNT];
    mHeaders[0] = NULL;
    mHeaders[1] = NULL;
    
    // mHeaders[2] = NULL;
    
	mnHeaderLevels[0] = 0;
	mnHeaderLevels[1] = 0;
    
    mvRowSorts = NULL;
    mvColumnSorts = NULL;
    
    mnOVGBUnits = 0;
}

DSSXTabView::~DSSXTabView()
{
    // 5/21/2012 tehe
    mpReportLevelIndex = NULL;
    
    if (mHeaders[0])
		delete mHeaders[0];
	if (mHeaders[1])
		delete mHeaders[1];
	mHeaders[0] = mHeaders[1] = NULL;
    
	if (mpKeyBuffer)
    {
		delete [] mpKeyBuffer;
        mpKeyBuffer = NULL;
    }
    
    // tehe: here we also need release the memory cached by new xtabview
    if(mpSliceLevel)
    {
        delete [] mpSliceLevel;
        mpSliceLevel = NULL;
    }
    
    if(mpSliceMap)
    {
        delete mpSliceMap;
        mpSliceMap = NULL;
    }
    
    if(mCurIndex)
    {
        delete [] mCurIndex;
        mCurIndex = NULL;
    }
    
    if(mpGroupByUnits)
    {
        delete [] mpGroupByUnits;
        mpGroupByUnits = NULL;
    }
    
    if(mpGroupByUnitsPos)
    {
        delete [] mpGroupByUnitsPos;
        mpGroupByUnitsPos = NULL;
    }
    
    if(mpvLastGBElements)
    {
        for(int i=0;i<mnGroupByUnits;i++)
        {
            if(mpvLastGBElements[i])
                delete mpvLastGBElements[i];
        }
        if(mpvLastGBElements)
            delete []mpvLastGBElements;
        mpvLastGBElements = NULL;
        
    }
    
    if(mppvMapGBPos2LevelSlice)
    {
        int lnGBGroups = 1<<mnGroupByUnits;
        for(int i=0;i<lnGBGroups;i++)
        {
            if (mppvMapGBPos2LevelSlice[i])
            {
                delete mppvMapGBPos2LevelSlice[i];
            }
        }
        if(mppvMapGBPos2LevelSlice)
            delete []mppvMapGBPos2LevelSlice;
        mppvMapGBPos2LevelSlice = NULL;
        
    }
    
    hDeleteSortDefinition();
    
    // 7/6/2012 tehe
    for(int i=0; i<mpvConsolidationBVs.size(); i++)
    {
        if(mpvConsolidationBVs[i])
        {
            delete mpvConsolidationBVs[i];
            mpvConsolidationBVs[i] = NULL;
        }
    }
    
    if(mpOVGBUnits)
    {
        delete [] mpOVGBUnits;
        mpOVGBUnits = NULL;
    }
    
    if(mpOVGBUnitPos)
    {
        delete [] mpOVGBUnitPos;
        mpOVGBUnitPos = NULL;
    }
    
    if(mpvLastOVGBElements)
    {
        for(int i=0;i<mnOVGBUnits;i++)
        {
            if(mpvLastOVGBElements[i])
                delete mpvLastOVGBElements[i];
        }
        if(mpvLastOVGBElements)
            delete []mpvLastOVGBElements;
        mpvLastOVGBElements = NULL;
        
    }
    
    if (mpAttributeThresholdIterator)
    {
        delete mpAttributeThresholdIterator;
        mpAttributeThresholdIterator = NULL;
    }
    
    if (mpAttributeThresholdItems)
    {
        delete mpAttributeThresholdItems;
        mpAttributeThresholdItems = NULL;
    }
}

int DSSXTabView::Init(DSSDataModelXTabImpl *ipXTabModel)
{
	if (mHeaders[0] || mHeaders[1] || mpKeyBuffer || !ipXTabModel)
		return E_FAIL;
	
	mpXTabModel = ipXTabModel;
    
	mHeaders[0] = new DSSXTabHeader();
	mHeaders[1] = new DSSXTabHeader();
	
	mnTotalUnits = ipXTabModel->Count();
	mpKeyBuffer = new int[mnTotalUnits];
    
	return 0;
}

DSSXTabHeader* DSSXTabView::getHeader(int iAxis)
{
	if (iAxis == 0)
		return mHeaders[0];
	else if (iAxis == 1)
		return mHeaders[1];
	else
		return NULL;
}

int DSSXTabView::get_Header(int iAxis, DSSXTabHeader** oppHeader)
{
    if(!oppHeader)
    {
        return S_OK;
    }
    if (iAxis == 0)
    {
        (*oppHeader) = mHeaders[0];
    }
	else if (iAxis == 1)
    {
		(*oppHeader) = mHeaders[1];
    }
	else
    {
		(*oppHeader) = NULL;
    }
    return S_OK;
}
int DSSXTabView::CreateSliceMap(int nRowLevels, int nColLevels, int nMetrics)
{
	mnRowLevels = nRowLevels;
	mnColLevels = nColLevels;
	mnLevels = nRowLevels * nColLevels;
	mnMetrics = nMetrics;
	
	int lnSlices = mnLevels * nMetrics;
	mvpSliceMap.resize(lnSlices);
	for (int i = 0; i < lnSlices; i++)
	{
		mvpSliceMap[i] = -1;
	}
	return S_OK;
}

int DSSXTabView::AddSlice(int iCubeSliceID, int iLevel, int iMetric)
{
	int lOffset = iLevel * mnMetrics + iMetric;
	if (lOffset >= mvpSliceMap.size())
		return E_FAIL;
	mvpSliceMap[lOffset] = iCubeSliceID;
	return S_OK;
}

int DSSXTabView::addAttributeThresholdSlice(int iLevel, int iTag, const DSS_ID * ipAttrID, int iSliceID)
{
    if (!ipAttrID)
        return E_INVALIDARG;
    
    mAttributeThresholdSliceInfo[AttributeThresholdKey(iLevel, iTag, *ipAttrID)] = iSliceID;
    
    // when slice changed, we reset the attribute threshold item
    if (mpAttributeThresholdItems)
    {
        delete mpAttributeThresholdItems;
        mpAttributeThresholdItems = NULL;
    }
    
    return S_OK;
}

int DSSXTabView::getAttributeThresholdSliceInfo(int iLevel, int iTag, const DSS_ID* ipAttrID, int* opSliceID)
{
    if (!opSliceID)
        return E_POINTER;
    *opSliceID = -1;
    
    if (!ipAttrID)
        return E_INVALIDARG;
    
    AttributeThresholdSliceMap::iterator it = mAttributeThresholdSliceInfo.find(AttributeThresholdKey(iLevel, iTag, *ipAttrID));
    if (it != mAttributeThresholdSliceInfo.end())
        *opSliceID = it->second;
    return S_OK;
}

int DSSXTabView::getAttributeThresholdSliceByLevel(bool *iLevel, std::vector<int> &ovSliceID)
{
    if (!mpSliceMap)
    {
        return S_FALSE;
    }
    int levelIndex = -1;
    if (mpSliceMap->FindLevelInHash(iLevel, &levelIndex) != S_OK)
    {
        return S_OK;
    }
    
    for (AttributeThresholdSliceMap::const_iterator it = mAttributeThresholdSliceInfo.begin(); it != mAttributeThresholdSliceInfo.end(); it++)
    {
        if (levelIndex == it->first.mLevel)
        {
            ovSliceID.push_back(it->second);
        }
    }
    return S_OK;
}

int DSSXTabView::getDataSlice(int iRowLevel, int iColLevel, int iMetric, DSSCubeDataSlice **oppSlice)
{
	int lOffset = iRowLevel * mnColLevels * mnMetrics + iColLevel * mnMetrics + iMetric;
	if (lOffset < 0 || lOffset >= mvpSliceMap.size() || !oppSlice)
		return E_INVALIDARG;
	
	*oppSlice = mpXTabModel->getCube()->getSliceByID(mvpSliceMap[lOffset]);
	return S_OK;
}

int DSSXTabView::getCubeSliceID(int iLevel, int iMetric)
{
	int offset = iLevel * mnMetrics + iMetric;
	if (offset < mvpSliceMap.size())
		return mvpSliceMap[offset];
	else
		return -1;
}

int DSSXTabView::getLevelInfo(int iLevel, int &orTotalID, vector<int> &orvUnits)
{
	int iRowLevel = iLevel / mnColLevels;
	int iColLevel = iLevel % mnColLevels;
	
	int lTotalID = 0;
	int hr = mHeaders[0]->getSubtotalIDByLevel(iRowLevel, lTotalID);
	CHECK_HR;
	
	int lTotalID2 = 0;
	hr = mHeaders[1]->getSubtotalIDByLevel(iColLevel, lTotalID2);
	CHECK_HR;
	
	if (lTotalID > 0 && lTotalID2 > 0 && lTotalID != lTotalID2)
        return S_FALSE;
	
	orTotalID = lTotalID > 0 ? lTotalID : lTotalID2;
	
	DSSTabularIndex *lpIndex = NULL;
	hr = mHeaders[0]->getIndexByLevel(iRowLevel, &lpIndex);
	CHECK_PTR(lpIndex);
	
	lpIndex->getUnits(orvUnits);
	
	hr= mHeaders[1]->getIndexByLevel(iColLevel, &lpIndex);
	CHECK_PTR(lpIndex);
	
	lpIndex->getUnits(orvUnits);
    
	return S_OK;
}

/*
 DSSCubeDataSlice * DSSXTabView::getDataSlice2(int iSliceID)
 {
 if (iSliceID >= 0 && iSliceID < mvpSliceMap.size())
 return mvpSliceMap[iSliceID];
 else
 return NULL;
 }
 
 int DSSXTabView::setDataSlice(int iSliceID, DSSCubeDataSlice *ipDataSlice)
 {
 if (iSliceID >= 0 && iSliceID < mvpSliceMap.size())
 {
 mvpSliceMap[iSliceID] = ipDataSlice;
 return S_OK;
 }
 else
 return E_INVALIDINDEX;
 }*/

int DSSXTabView::GetCellValue(int RowIndex, int ColIndex, double &orVal, DSSDataFlag &orDataFlag, int *opThreshold, int *opMetric)
{
	if (mnMetrics == 0)
	{
		//no metrics on grid
		orDataFlag = DssDataInvalidCell;
		return S_OK;
	}
    
	int hr = S_OK;
	int lRowLevel = mHeaders[0]->getLevel(RowIndex);
	int lColLevel = mHeaders[1]->getLevel(ColIndex);
	if (lRowLevel < 0 || lColLevel < 0)
		return E_FAIL;
	
	int lMetricID = mHeaders[1]->getMetricID(ColIndex);
	if (lMetricID < 0)
		lMetricID = mHeaders[0]->getMetricID(RowIndex);
	
	if (opMetric)
	{
		if (lMetricID >= 0 && lMetricID < mnMetrics)
			*opMetric = lMetricID;
		else
			*opMetric = 0;
	}
	
	DSSCubeDataSlice *lpDataSlice = NULL;
	getDataSlice(lRowLevel, lColLevel, lMetricID, &lpDataSlice);
    
	if (lpDataSlice)
	{
        //		if (!lpDataSlice->getIndices())
		{
			int lRowOffset = mHeaders[0]->getOffsetOfIndex(RowIndex);
			int lColOffset = mHeaders[1]->getOffsetOfIndex(ColIndex);
			int lnColIndexSize = 0;
			{
				DSSTabularIndex *lpColIndex = NULL;
				hr = mHeaders[1]->getIndexByLevel(lColLevel, &lpColIndex);
				if (lpColIndex)
					lnColIndexSize = lpColIndex->Count();
			}
			long long lOffset = ((long long)lRowOffset)*lnColIndexSize + lColOffset;
			hr = lpDataSlice->getNumericByOffset(lOffset, orVal, orDataFlag, opThreshold);
			CHECK_HR;
		}
        //		else {
        //			_ASSERT(false);		//not implement yet
        //don't do anything
        //		}
	}
	else
	{
		//it's a XTab NULL
		orDataFlag = DssDataInvalidCell;
	}
	
	return S_OK;
}

int DSSXTabView::GetRawCellInfo(int RowIndex, int ColIndex, const char **oppData,
                                int &orDataSize, EnumDSSDataType &orDataType,
                                DSSDataFlag &orDataFlag, int *opThreshold,
                                int *opMetric, int *opTotalFlag, bool ibGetNullDataType)
{
	if (mnMetrics == 0)
	{
		//no metrics on grid
		orDataFlag = DssDataInvalidCell;
		return S_OK;
	}
    
	int hr = S_OK;
	int lRowLevel = mHeaders[0]->getLevel(RowIndex);
	int lColLevel = mHeaders[1]->getLevel(ColIndex);
	if (lRowLevel < 0 || lColLevel < 0)
		return E_FAIL;
	
	int lMetricID = mHeaders[1]->getMetricID(ColIndex);
	if (lMetricID < 0)
		lMetricID = mHeaders[0]->getMetricID(RowIndex);
	
	if (opMetric)
	{
		if (lMetricID >= 0 && lMetricID < mnMetrics)
			*opMetric = lMetricID;
		else
			*opMetric = 0;
	}
	
	DSSCubeDataSlice *lpDataSlice = NULL;
	getDataSlice(lRowLevel, lColLevel, lMetricID, &lpDataSlice);
    
	if (lpDataSlice)
	{
        //		if (!lpDataSlice->getIndices())
		{
			int lRowOffset = mHeaders[0]->getOffsetOfIndex(RowIndex);
			int lColOffset = mHeaders[1]->getOffsetOfIndex(ColIndex);
			int lnColIndexSize = 0;
			{
				DSSTabularIndex *lpColIndex = NULL;
				hr = mHeaders[1]->getIndexByLevel(lColLevel, &lpColIndex);
				if (lpColIndex)
					lnColIndexSize = lpColIndex->Count();
			}
            //fyang, 959716
            if(!lpDataSlice->IsConstantSlice())
            {
                DSSDataColumn* lpColumn = lpDataSlice->getColumn();
                if(lpColumn && lpColumn->getRowCount() < lRowOffset * lnColIndexSize + lColOffset)
                {
                    *oppData = NULL;
                    orDataSize = 0;
                    orDataFlag = DssDataInvalid;
                    return S_OK;
                }
            }
			hr = lpDataSlice->getValByOffset(lRowOffset * lnColIndexSize + lColOffset, oppData, orDataSize, orDataType, orDataFlag, opThreshold);
			CHECK_HR;
		}
        //		else {
        //			_ASSERT(false);		//not implement yet
        //don't do anything
        //		}
	}
	else
	{
		//it's a XTab NULL
		orDataFlag = DssDataInvalidCell;
	}
	
	//TODO:
	if (opTotalFlag)
	{}
	
	if (ibGetNullDataType)
	{}
    
    /*	for (int i = 0; i < mnTotalUnits; i++)
     mpKeyBuffer[i] = DSSXTAB_TOTAL_KEY;
     
     int lMetricID = 0;
     hr = mHeaders[0]->FillIndex(RowIndex, mnTotalUnits, mpKeyBuffer, &lMetricID);
     CHECK_HR;
     
     hr = mHeaders[1]->FillIndex(ColIndex, mnTotalUnits, mpKeyBuffer, &lMetricID);
     CHECK_HR;
     
     DSSCubeDataSlice *lpDataSlice = NULL;
     hr = getDataSlice(lRowLevel, lColLevel, lMetricID, &lpDataSlice);
     
     if (lpDataSlice)
     {
     hr = lpDataSlice->getVal(mnTotalUnits, mpKeyBuffer, oppData, orDataSize, orDataType, orDataFlag, opThreshold);
     CHECK_HR;
     }
     else
     {
     //it's a XTab NULL
     orDataFlag = DssDataInvalidCell;
     }
     
     if (opMetric)
     {
     if (lMetricID >= 0 && lMetricID < mnMetrics)
     *opMetric = lMetricID;
     else
     *opMetric = 0;
     }
     
     //TODO:
     if (opTotalFlag)
     {}
     
     if (ibGetNullDataType)
     {}
     */
	return S_OK;
}

int DSSXTabView::getOffsetByElements(int iAxis, std::map<int, int> &irKeys)
{
	if (iAxis == 0)
		return mHeaders[0]->getOffsetByElements(irKeys);
	else if (iAxis == 1)
		return mHeaders[1]->getOffsetByElements(irKeys);
	
	return -1;
}

int DSSXTabView::getSubtotalIndex(int iRow, int iCol, int &orSubtotalIndex)
{
	int lRowSubtotalID = 0;
	int hr = mHeaders[0]->getSubtotalID(iRow, lRowSubtotalID);
	CHECK_HR;
	
	int lColSubtotalID = 0;
	hr = mHeaders[1]->getSubtotalID(iCol, lColSubtotalID);
	CHECK_HR;
	
	if (lRowSubtotalID > 0 && lColSubtotalID > 0 && lRowSubtotalID != lColSubtotalID)
		return S_FALSE;			//corss tabular cell of two different totals
	
	orSubtotalIndex = lRowSubtotalID > 0 ? lRowSubtotalID : lColSubtotalID;
	return S_OK;
}

int DSSXTabView::getLevel(int iRow, int iCol)
{
	int lRowLevel = mHeaders[0]->getLevel(iRow);
	int lColLevel = mHeaders[1]->getLevel(iCol);
    
	if (lRowLevel < 0 || lColLevel < 0)
		return -1;
	
	return lRowLevel * mHeaders[1]->getLevelCount() + lColLevel;		//TODO: caller wants distinct levels
}

int DSSXTabView::get_Metric(int iRow, int iCol, int* opMetric)
{
    if(!opMetric)
    {
        return S_OK;
    }
	if (mHeaders[0]->HasMetric())
    {
        (*opMetric) = mHeaders[0]->getMetricID(iRow);
    }
	else
    {
		(*opMetric) = mHeaders[1]->getMetricID(iCol);
    }
    return S_OK;
}

int DSSXTabView::getMetricID(int iRow, int iCol)
{
	if (mHeaders[0]->HasMetric())
		return mHeaders[0]->getMetricID(iRow);
	else
		return mHeaders[1]->getMetricID(iCol);
}

int DSSXTabView::getRowCount()
{
	if (mHeaders[0]->getNumUnits() > 0 || mnMetrics > 0)
		return mHeaders[0]->getRowCount();
	else
		//if row header units is empty and xtabview doesn't have metrics, row count should be 0
		return 0;
}

int DSSXTabView::getColCount()
{
	if (mHeaders[1]->getNumUnits() > 0 || mnMetrics > 0)
		return mHeaders[1]->getRowCount();
	else
		//if column header units is empty and xtabview doesn't have metrics, row count should be 0
		return 0;
}
int DSSXTabView::get_NumRows(int* opRow)
{
    if(!opRow)
    {
        return S_OK;
    }
    if (mHeaders[0]->getNumUnits() > 0 || mnMetrics > 0)
    {
		(*opRow) = mHeaders[0]->getRowCount();
    }
	else
    {
		//if row header units is empty and xtabview doesn't have metrics, row count should be 0
		(*opRow) = 0;
    }
    return S_OK;
}


int DSSXTabView::get_NumColumns(int* opCol)
{
    if(!opCol)
    {
        return S_OK;
    }
    if (mHeaders[1]->getNumUnits() > 0 || mnMetrics > 0)
    {
		(*opCol) = mHeaders[1]->getRowCount();
    }
	else
    {
		//if column header units is empty and xtabview doesn't have metrics, row count should be 0
		(*opCol) = 0;
    }
    return S_OK;
}
int DSSXTabView::size()
{
	int lSize = sizeof(this);
	
	if (mHeaders[0])
		lSize += mHeaders[0]->size();
	
	if (mHeaders[1])
		lSize += mHeaders[1]->size();
	
	if (mpKeyBuffer)
		lSize += sizeof(int) * mnTotalUnits;
    
	return lSize;
}

int DSSXTabView::setCellValue(int RowIndex, int ColIndex, char *ipData, int iSizeInByte, DSSDataFlag iFlag, bool ibIsRawData)
{
	if (mnMetrics == 0)
		return S_OK;
    
	int hr = S_OK;
	int lRowLevel = mHeaders[0]->getLevel(RowIndex);
	int lColLevel = mHeaders[1]->getLevel(ColIndex);
	if (lRowLevel < 0 || lColLevel < 0)
		return E_FAIL;
	
	int lMetricID = mHeaders[1]->getMetricID(ColIndex);
	if (lMetricID < 0)
		lMetricID = mHeaders[0]->getMetricID(RowIndex);
	
	DSSCubeDataSlice *lpDataSlice = NULL;
	getDataSlice(lRowLevel, lColLevel, lMetricID, &lpDataSlice);
    
	if (lpDataSlice)
	{
        //		if (!lpDataSlice->getIndices())
		{
			int lRowOffset = mHeaders[0]->getOffsetOfIndex(RowIndex);
			int lColOffset = mHeaders[1]->getOffsetOfIndex(ColIndex);
			int lnColIndexSize = 0;
			{
				DSSTabularIndex *lpColIndex = NULL;
				hr = mHeaders[1]->getIndexByLevel(lColLevel, &lpColIndex);
				if (lpColIndex)
					lnColIndexSize = lpColIndex->Count();
			}
			
			if (iFlag != DssDataOk)
			{
				ipData = NULL;
				iSizeInByte = 0;	//483246; if the metric is string type, size should be 0
			}
			
			if (!ibIsRawData)
			{
				hr = lpDataSlice->updateValueWithString(lRowOffset * lnColIndexSize + lColOffset, ipData, iSizeInByte);
				CHECK_HR;
			}
			else
			{
				DSSDataColumn *lpColumn = lpDataSlice->getColumn();
				hr = lpColumn->setData(lRowOffset * lnColIndexSize + lColOffset, ipData, iSizeInByte, iFlag);
				CHECK_HR;
			}
		}
        //		else {
        //			_ASSERT(false);		//not implement yet
        //don't do anything
        //		}
	}
	return S_OK;
}

int DSSXTabView::setCellValueWithLookup(int RowIndex, int ColIndex, char *ipData, int iSizeInByte, DSSDataFlag iFlag, bool ibIsRawData)
{
	if (mnMetrics == 0)
		return S_OK;
    
	int hr = S_OK;
    
    vector<DSSXTabKey> lvIndexKeys(mnTotalUnits);
    DSSXTabKey * pIndex = &lvIndexKeys[0];
    int lMetric = 0,  lTag = 0 ;
    DSSDataFlag lFlag = DssDataOk;
    
	DSSCubeDataSlice *lpDataSlice = NULL;
	DSSTabularIndex* lpIndexObject = NULL;
	int lOffsetInIndexObject = -1;
	hr = FindIndexFromCoordinate(RowIndex, ColIndex, pIndex, &lMetric, &lTag, &lpIndexObject, &lOffsetInIndexObject);
	if(hr == S_OK)
	{
        hr = FindDataLocFromIndex(pIndex, lMetric, lTag, DssOriginalSlice, &lpDataSlice, lFlag);
    }
    
    if(lpDataSlice)
    {
		DSSTabularIndex *lpSliceIndex;
		lpSliceIndex = lpDataSlice->getIndices();
		CHECK_HR;
        
		if (!(lpIndexObject && lpSliceIndex && lpSliceIndex->IsSameOrder(lpIndexObject)))
		{
            //needs look up
            hr = lpSliceIndex->LookUp(mnTotalUnits, pIndex, &lOffsetInIndexObject);
            CHECK_HR;
		}
        
        if (iFlag != DssDataOk)
        {
            ipData = NULL;
            iSizeInByte = 0;	//483246; if the metric is string type, size should be 0
        }
        
        if (!ibIsRawData)
        {
            hr = lpDataSlice->updateValueWithString(lOffsetInIndexObject, ipData, iSizeInByte);
            CHECK_HR;
        }
        else
        {
            DSSDataColumn *lpColumn = lpDataSlice->getColumn();
            hr = lpColumn->setData(lOffsetInIndexObject, ipData, iSizeInByte, iFlag);
            CHECK_HR;
        }
	}
	return S_OK;
}

int DSSXTabView::GenerateRecord(int iRow, int iColumn, vector<int> &irvUnitID, vector<int> &irvFormID, vector<int> &irvWBFormID,vector<AERawData> &orvRecordData, vector<AERawData> &orvDisplayData)
{
	int lMetricUnitID = mpXTabModel->getMetricUnitID();
    
	int lnUnits = mpXTabModel->Count();
    vector<DSSTabularUnit*> lvTabularUnits(lnUnits);
    
    for (int i = 0; i < lnUnits; i++)
    {
        DSSTabularUnit *lpUnit = mpXTabModel->Item(i);
        CHECK_PTR(lpUnit);
        
        lvTabularUnits[i] = lpUnit;
    }
    
    return hGenerateRecord(iRow, iColumn, irvUnitID, irvFormID,irvWBFormID, lvTabularUnits, lMetricUnitID, false, orvRecordData,orvDisplayData);
}

int DSSXTabView::GenerateRecordWithLookup(int iRow, int iColumn, vector<int> &irvUnitID, vector<int> &irvFormID, vector<int> &irvWBFormID, vector<AERawData> &orvRecordData, vector<AERawData> &orvDisplayData)
{
    if (!mpViewDataSet)
        return E_FAIL;
    
    DSSTabularData *lpTabularData = mpViewDataSet->getTabularData();
    
	int lMetricUnitID = lpTabularData->getMetircPos();
    
	int lnUnits = mpViewDataSet->Count();
    vector<DSSTabularUnit*> lvTabularUnits(lnUnits);
    for (int i = 0; i < lnUnits; i++)
    {
        DSSTabularUnit *lpUnit = NULL;
        lpTabularData->Item(i, &lpUnit);
        CHECK_PTR(lpUnit);
        
        lvTabularUnits[i] = lpUnit;
    }
    
    return hGenerateRecord(iRow, iColumn, irvUnitID, irvFormID, irvWBFormID, lvTabularUnits, lMetricUnitID, true, orvRecordData, orvDisplayData);
}


int DSSXTabView::hGenerateRecord(int iRow, int iColumn, vector<int> &irvUnitID, vector<int> &irvFormID, vector<int> &irvWBFormID,vector<DSSTabularUnit*> &irvTabularUnits, int lMetricUnitID, bool ibNeedLookup, vector<AERawData> &orvRecordData,vector<AERawData> &orvDisplayData)
{
	int hr = S_OK;
	
	//1, reset row and column to the first metric
	bool lbHasMetricOnRow = mHeaders[0]->HasMetric();
	
	int lCellMetricID = getMetricID(iRow, iColumn);
    if(lCellMetricID >= 0)
    {
        iRow = lbHasMetricOnRow ? iRow - lCellMetricID : iRow;
        iColumn = lbHasMetricOnRow ? iColumn : iColumn - lCellMetricID;
	}
	//2, fill index at [row, col] on XTabView
	int lnUnits = irvTabularUnits.size();
	vector<int> lvKeys(lnUnits);
    
	int i = 0;
	for (i = 0; i < lnUnits; i++)
		lvKeys[i] = DSSXTAB_TOTAL_KEY;
	
	int lMetricKey = 0;
	hr = mHeaders[0]->FillIndex(iRow, lnUnits, &lvKeys[0], &lMetricKey);
	CHECK_HR;
    
	hr = mHeaders[1]->FillIndex(iColumn, lnUnits, &lvKeys[0], &lMetricKey);
	CHECK_HR;
    
	if (lMetricUnitID >= 0 && lMetricUnitID < lnUnits)
		lvKeys[lMetricUnitID] = lMetricKey;
    
	//2, get the raw data with unit keys
	int lnColumns = irvUnitID.size();
	orvRecordData.resize(lnColumns);
    orvDisplayData.resize(lnColumns);
    
	for (int i = 0; i < lnColumns; i++)
	{
        orvDisplayData[i].mValue = NULL;
        orvDisplayData[i].mSizeInByte = -1;
        orvDisplayData[i].mDataType = DssDataTypeUnknown;
        orvDisplayData[i].mFlag = DssDataOk;
        
		char *lpData = NULL;
		int lSize = 0;
		EnumDSSDataType lDataType = DssDataTypeReserved;
		DSSDataFlag lFlag = DssDataNull;
		
		int lXTabUnitID = irvUnitID[i];
		if ( lXTabUnitID >= 0 && irvWBFormID[i] != -100)
		{
			//it's not a metric column
			DSSTabularUnit *lpUnit = irvTabularUnits[lXTabUnitID];
			CHECK_PTR(lpUnit);
			
			int lKey = lvKeys[lXTabUnitID];
			int lBaseForm = irvFormID[i];
			if(irvWBFormID[i] >= 0)
            {
                hr = getUnitValue(lpUnit, lKey, lBaseForm, (const void**)&lpData, &lSize, &lDataType, &lFlag);
                CHECK_HR;
                
                orvDisplayData[i].mValue = lpData;
                orvDisplayData[i].mSizeInByte = lSize;
                orvDisplayData[i].mDataType = lDataType;
                orvDisplayData[i].mFlag = lFlag;
                
                lBaseForm = irvWBFormID[i];
            }
            hr = getUnitValue(lpUnit, lKey, lBaseForm, (const void**)&lpData, &lSize, &lDataType, &lFlag);
            CHECK_HR;
		}
		else//539028: if (irvWBFormID[i] != -100)
		{
			//metric
			int lMetricID = irvFormID[i];
			if (lXTabUnitID  < 0 && lMetricID >= 0)
			{
				//need adjust row and column to get all the other metric values
				int lRowAdjusted = lbHasMetricOnRow ? iRow + lMetricID : iRow;
				int lColAdjusted = lbHasMetricOnRow ? iColumn : iColumn + lMetricID;
				
                if (ibNeedLookup)
                {
                    hr = GetRawCellInfo2(lRowAdjusted, lColAdjusted, (const char**)&lpData, lSize, lDataType, lFlag);
                    CHECK_HR;
                }
                else
                {
                    int lThreshold, lMetric;
                    hr = GetRawCellInfo(lRowAdjusted, lColAdjusted,
                                        (const char**)&lpData, lSize, lDataType, lFlag, &lThreshold, &lMetric);
                    CHECK_HR;
				}
				//DDIC defined on metric
                if(irvWBFormID[i] == -100)
                {
                    orvDisplayData[i].mValue = lpData;
                    orvDisplayData[i].mSizeInByte = lSize;
                    orvDisplayData[i].mDataType = lDataType;
                    orvDisplayData[i].mFlag = lFlag;
                }
                
			}
            //DDIC on attribute but form not match
			else
			{
                //lFlag = DssDataNull;
                //584745
                DSSTabularUnit *lpUnit = NULL;
                if (mpXTabModel)
                {
                    lpUnit = mpXTabModel->Item(lXTabUnitID);
                } else
                {
                    DSSTabularData *lpTabularData = NULL;
                    hr = mpViewDataSet->getTabularData(&lpTabularData);
                    CHECK_HR;
                    lpTabularData->Item(lXTabUnitID, &lpUnit);
                }
                if(lpUnit)
                {
                    int lKey = lvKeys[lXTabUnitID];
                    int lBaseForm = irvFormID[i];
                    hr = getUnitValue(lpUnit, lKey, lBaseForm, (const void**)&lpData, &lSize, &lDataType, &lFlag);
                    CHECK_HR;
                }
			}
        }
       	orvRecordData[i].mValue = lpData;
		orvRecordData[i].mSizeInByte = lSize;
		orvRecordData[i].mDataType = lDataType;
		orvRecordData[i].mFlag = lFlag;
        
    }
	return S_OK;
}

int DSSXTabView::getUnitValue(DSSTabularUnit *ipUnit, int iKey, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag)
{
	if (iKey >= 0)
	{
		EnumDSSTemplateUnitType lUnitType = ipUnit->getUnitType();
		switch (lUnitType) {
			case DssTemplateAttribute:
				return hGetAttributeFormValue(ipUnit, iKey, iBaseForm, oppData, opDataSize, opDataType, opDataFlag);
			case DssTemplateCustomGroup:
				return hGetCustomGroupValue(ipUnit, iKey, oppData, opDataSize, opDataType, opDataFlag);
			case DssTemplateConsolidation:
				return hGetConsolidationValue(ipUnit, iKey, iBaseForm, oppData, opDataSize, opDataType, opDataFlag);
			default:
				break;
		}
	}
	
	return E_NOTIMPL;
}

int DSSXTabView::getUnitThreshold(EnumDSSAxisName iAxis, int iOrdinal, const DSS_ID& irAttributeID, int& orThresholdID)
{
    int hr = S_OK;
    if (!mpAttributeThresholdItems)
    {
        hr = hInitAttributeThresholdItem();
        CHECK_HR;
    }
    
    if (mpAttributeThresholdItems && !mpAttributeThresholdItems->getAttributeThresholdItem(iAxis, iOrdinal, irAttributeID, orThresholdID))
    {
        // don't find the attribute threshold at this cell
        return S_OK;
    }
    return hr;
}

int DSSXTabView::getUnitMergedThreshold(EnumDSSAxisName iAxis, const vector<int>& iOrdinal, const DSS_ID& irAttributeID, int& orThresholdID)
{
    int res = -1;
    if (!mpAttributeThresholdItems)
    {
        int hr = hInitAttributeThresholdItem();
        CHECK_HR;
    }
    
    if (mpAttributeThresholdItems)
    {
        res = mpAttributeThresholdItems->getMergedAttributeThresholdItem(iAxis, iOrdinal, irAttributeID, orThresholdID);
    }
    
    return res;
}

bool DSSXTabView::hasUnitThreshold(const DSS_ID& irAttributeID)
{
    if (!mpAttributeThresholdItems)
    {
        if (hInitAttributeThresholdItem() != S_OK)
            return false;
    }
    
    return mpAttributeThresholdItems->hasThreshold(irAttributeID);
}

int DSSXTabView::hGetAttributeFormValue(DSSTabularUnit *ipUnit, int iOffset, int iBaseForm,
                                        const void **oppData, int *opDataSize,
                                        EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag)
{
	DSSTabularAttribute *lpAttribute = static_cast<DSSTabularAttribute *> (ipUnit);
	
	//TODO[opt]: cache data form
	int lColumnID = 0;
	int hr = lpAttribute->FindFormWithLocale(0, iBaseForm, 0, lColumnID);
    
    //TQMS 653168, hyan,21/11/2012
    //for extra unit, hr will be S_FALSE
    //Temporary fix to avoid crash
    if(hr!=S_OK)
    {
        *oppData = NULL;
		*opDataSize = 0;
		*opDataType = DssDataTypeVarChar;
		*opDataFlag = DssDataMissing;
		return S_OK;
    }
	//CHECK_HR;
	
	DSSDataColumn *lpColumn = NULL;
	hr = static_cast<DSSTabularAttribute *> (ipUnit)->Item(lColumnID, &lpColumn);
	CHECK_HR;
	
	return lpColumn->getData(iOffset, (const char**)oppData, *opDataSize, *opDataType, *opDataFlag);
}

int DSSXTabView::hGetCustomGroupValue(DSSTabularUnit *ipUnit, int iOffset, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag)
{
	DSSTabularCustomGroup *lpCustomGroup = static_cast<DSSTabularCustomGroup *> (ipUnit);
	
	*opDataFlag = DssDataOk;
	*opDataType = DssDataTypeVarChar;
	*oppData = lpCustomGroup->getTextAndLength(iOffset, *opDataSize);
	if (!(*oppData))
    {
        *opDataFlag = DssDataNull;
        XTabCustomGroupElement * lpCGElement = lpCustomGroup->getElement(iOffset);
        AE_ASSERT(!lpCGElement);    //check if it's join null
    }
	return S_OK;
}

int DSSXTabView::hGetConsolidationValue(DSSTabularUnit *ipUnit, int iOffset, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag)
{
	DSSTabularConsolidation *lpConsolidation = static_cast<DSSTabularConsolidation *> (ipUnit);
	
	if (lpConsolidation->IsDerivedElement() && lpConsolidation->IsLeaf(iOffset) && lpConsolidation->IsDisplayed(iOffset))
	{
		DSSTabularUnit *lpChildUnit = lpConsolidation->getChildUnit();
		CHECK_PTR(lpChildUnit);
		
		int lChildKey = lpConsolidation->getChildElementKey(iOffset);
		_ASSERT(lChildKey >= 0);
		return hGetAttributeFormValue(lpChildUnit, lChildKey, iBaseForm, oppData, opDataSize, opDataType, opDataFlag);
	}
	
	*opDataFlag = DssDataOk;
	*opDataType = DssDataTypeVarChar;
	*oppData = lpConsolidation->getTextAndLength(iOffset, *opDataSize);
    if(!*oppData)
    {
        *opDataFlag = DssDataNull; // zhyang, 816224, when oppData returns null, it should be a join null
    }
	//_ASSERT(*oppData);
	return S_OK;
}

int DSSXTabView::getMetricCount(GUID &iMetricID)
{
	DSSTabularMetrics *lpTabularMetrics = NULL;
    //701218,zhyang, deal with new rwd logic here
    if(mpXTabModel)
    {
        mpXTabModel->getMetricsUnit(&lpTabularMetrics);
        if (lpTabularMetrics)
        {
            int lMetricIndex = lpTabularMetrics->FindMetric(iMetricID);
            if (lMetricIndex >= 0)
            {
                DSSCubeDataSlice *lpSlice = NULL;
                this->getDataSlice(0, 0, lMetricIndex, &lpSlice);		//only check report level
                if (lpSlice)
                {
                    DSSDataColumn *lpColumn = lpSlice->getColumn();
                    if (lpColumn)
                        return lpColumn->getRowCount();
                }
            }
        }
    }
    else
    {
        DSSTabularData *lpTabularData = mpViewDataSet->getTabularData();
        
        lpTabularMetrics = lpTabularData->getTabularMetircs();
        if(lpTabularMetrics)
        {
            int lMetricIndex = lpTabularMetrics->FindMetric(iMetricID);
            if (lMetricIndex >= 0)
            {
                DSSCubeDataSlice *lpSlice = NULL;
                bool * pIndexLevel = new bool[mnTotalUnits];
                MBase::DSSStrongPtr<bool, MBase::DeleteArray<bool> > pIndexLevelStrongPtr(pIndexLevel);
                
                //report level
                for(int i = 0; i < mnTotalUnits; i++)
                    pIndexLevel[i] = true;
                
                int SliceID = 0;
                int hr = mpSliceMap->SearchID(pIndexLevel, 0, DssOriginalSlice, lMetricIndex, &SliceID);
                // need to convert SliceID into slice pointer
                
                //lwang, TQMS 331360; set proper data flag if data is invalid;
                //set aggregation Null if it is a subtoal; otherwise, it should be a warehouse null
                if(SliceID != -1 && hr != S_FALSE)
                {
                    lpSlice =  mpCube -> getSliceByID(SliceID);
                    if (lpSlice)
                    {
                        DSSDataColumn *lpColumn = lpSlice->getColumn();
                        if (lpColumn)
                            return lpColumn->getRowCount();
                    }
                }
            }
        }
    }
	return 0;
}

int DSSXTabView::RecycleMetricSliceMemory()
{
	DSSCube *lpCube = mpXTabModel->getCube();
	int lnSlices = mvpSliceMap.size();
	for (int i = 0; i < lnSlices; i++)
	{
		DSSCubeDataSlice *lpDataSlice = lpCube->getSliceByID(mvpSliceMap[i]);
        
		if (lpDataSlice)
			lpDataSlice->RecycleMemory();
	}
	return S_OK;
	
}

int DSSXTabView::Init (DSSXTabDispInfo *pDispInfo, DSSViewDataSet *ipViewDataSet)
{
  	AE_ASSERT(pDispInfo);
	AE_ASSERT(ipViewDataSet);
    mpViewDataSet = ipViewDataSet;
    mbOnDemandLoading = mpViewDataSet->IsOnDemandLoading();
    int hr = S_OK;
    
	if(mfStatus != XTAB_UNINITIALIZED)
		return E_FAIL;
    
    if(mfInitInternal)
        //this C++ init has been called before
		return E_FAIL;
    
	//need to remember to release
	mpCube = ipViewDataSet->GetCube();
	DSSViewDataSet *lpViewDataSet = NULL;
    lpViewDataSet = mpViewDataSet;
    CHECK_HR;
    
    
    DSSTabularData * lpTabularData = NULL;
	hr = lpViewDataSet -> getTabularData(&lpTabularData);
	CHECK_HR;
    
	mnTotalUnits = lpTabularData -> Count();
    
	pDispInfo -> get_LevelCount(&mnLevels);
	pDispInfo -> get_PositionCount(&mnMetrics);
    
    
	//xhan 01/07/05
	hr = lpTabularData->getDummyUnitIndex(&mDummyUnitIndex);
	CHECK_HR;
    
	// initialize the slice map
	mpSliceMap = new DSSSliceMapper(mnLevels, mnMetrics, mnTotalUnits);
	CHECK_NEWED_PTR(mpSliceMap);
    
	hr = mpSliceMap->Init();
	CHECK_HR;
    
	int i = 0,  j = 0 ;
    
	int * lOrgUnit = new int[mnTotalUnits];
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lOrgUnitStrongPtr(lOrgUnit);
    
	CHECK_NEWED_PTR(lOrgUnit);
	bool * llevelMap = new bool[mnTotalUnits];
    MBase::DSSStrongPtr<bool, MBase::DeleteArray<bool> > lLevelMapStrongPtr(llevelMap);
    
	CHECK_NEWED_PTR(llevelMap);
	int lcbWritten = 0;
	for(i = 0; i < mnLevels; i++)
	{
		// get level def
		hr = pDispInfo -> get_OrgUnitByLevel(i, mnTotalUnits, lOrgUnit, &lcbWritten);
		CHECK_HR;
        
		// convert it into attribute map (CDSSSliceMapper standard)
		for(j = 0; j < mnTotalUnits; j++)
			llevelMap[j] = false;
		for(j = 0; j < lcbWritten; j++)
			llevelMap[lOrgUnit[j]] = true;
        
        
        Int32 nSliceInLevel = 0;
        Int32 lSliceID = 0,  lTag = 0 ,  lMetric = 0 ;
        EnumDssSliceFlag lSliceFlag;
        EnumDSSRuntimeSubtotalType lExtremeType = DssRuntimeSubtotalDefault;
        Int32 lModelIndex = -1;
        Int32 lTRMetric = -1;
        Int32 lnxVariables = 0;
        Int32* lpxVariables = NULL;
        Int32* lpxIndexMetricSlices = NULL;
        EnumDSSObjectType *lpXVariableTypes = NULL;
        
        
		//xhan 01/07/05 put dummy unit into report level
		if (i == 0 && mDummyUnitIndex != -1)
			llevelMap[mDummyUnitIndex] = true;
        
		//always keep all the level
		int lTemp = 0;
        // 6/19/2012 tehe: we we need to first add the level, because add slice may be not called when there are no metric slices. but we still need the level info
		//very strange, Andrea use 1 to indicate Fail and 0 indicate success
        //		if(mpSliceMap -> AddLevelInHash(llevelMap, &lTemp))
        //			return hr = E_FAIL;
        mpSliceMap -> AddLevel(llevelMap, &lTemp,i);
		//keep reading the slices of this level.
        
		if(pDispInfo)
			hr = pDispInfo -> get_SliceCountByLevel2(i, &nSliceInLevel);
		CHECK_HR;
        
		for(j = 0; j < nSliceInLevel; j++)
		{
            
			if(pDispInfo)
				hr = pDispInfo->get_SliceInfo3(i, j, &lSliceID, &lMetric, &lTag, &lSliceFlag, &lExtremeType, &lModelIndex, &lTRMetric, &lnxVariables, &lpxVariables, &lpXVariableTypes, &lpxIndexMetricSlices);
			CHECK_HR;
            
			//suppose we got SliceID, metric and tag
			hr = mpSliceMap->AddSlice(llevelMap, lTag, lSliceFlag, lMetric, lSliceID, i, lExtremeType, lModelIndex, lTRMetric, lnxVariables, lpxVariables, lpXVariableTypes, lpxIndexMetricSlices);
            CHECK_HR;
			CHECK_HR;
            
			//check whether we have cell formatting object
			if(!mHasCellFormat)
			{
				DSSCubeDataSlice *lpSlice;
				lpSlice = mpCube -> getSliceByID(lSliceID);
				if(lpSlice  != NULL)
                {
                    EnumDSSDataType lDataType = DssDataTypeUnknown;
                    lDataType = lpSlice->getDataType();
                    
                    if(lDataType==DssDataTypeCellFormatData)
                        mHasCellFormat=1;
                }
			}
		}
	}
    
    // load the attribute threshold slice info to XTabView, because the pDisInfo will be released
    const DSSXTabDispInfo::AttributeThresholdSliceInfo& lpAttributeThresholdInfo = pDispInfo->getAttributeThresholdSlices();
    for (DSSXTabDispInfo::AttributeThresholdSliceInfo::const_iterator iter = lpAttributeThresholdInfo.begin(); iter != lpAttributeThresholdInfo.end(); iter++) {
        std::string lTempStr = iter->first;
        
        // parse the threshold info from Level_Tag_SliceID
        std::vector<std::string> lTokens; //we'll put all of the tokens in here
        size_t start = 0, pos = std::string::npos;
        while (start < lTempStr.length() && (pos = lTempStr.find("_", start)) != std::string::npos) {
            lTokens.push_back(lTempStr.substr(start, pos - start));
            start = pos + 1;
        }
        lTokens.push_back(lTempStr.substr(start));
        if (lTokens.size() != 3)
            continue;
        
        int lLevel = atoi(lTokens[0].c_str());
        int lTag = atoi(lTokens[1].c_str());
        int lSliceID = atoi(lTokens[2].c_str());
        
        hr = addAttributeThresholdSlice(lLevel, lTag, &iter->second, lSliceID);
        CHECK_HR;
    }
    
	//should have tag for Header;
    
	//new a header for each axis
	for(i = 0; i < HEADCOUNT; i++) {
		mHeaders[i] = new DSSXTabHeader();
		CHECK_NEWED_PTR(mHeaders[i]);
	}
    
    /*
     DSSTabularData *ipTabularData;
     mpViewDataSet->getTabularData(&ipTabularData);
     hr = ipTabularData->setXTabView(this);
     */
    
	mfInitInternal = true;
	mfStatus = XTAB_UNINITIALIZED;
	return hr = S_OK;
    
}

int DSSXTabView::Init (DSSXTabLayoutInfo *pLayout, DSSXTabView * /*pOldView*/)
{
    
    if(mfStatus != XTAB_UNINITIALIZED)
		return E_FAIL;
    
    CHECK_PTR(pLayout);
    
	//disregard the oldview for now.
	//do not read the current element for now. do not filter anything for now.
    
	//get the total number of rows in the metrictable.
    
	int i = 0,  j = 0 ,  k = 0 ;
    
   	DSSTabularIndexPool * pIndicesPool = NULL;
	int hr = mpCube -> getIndexPool(&pIndicesPool);
	CHECK_HR;
    
    
	//generate header for each axis
	int nUnitInAxis = 0;
	int * pUnitInAxis = new int[mnTotalUnits + 1];	//this may have metric unit in it
    MBase::StrongPtr<int, MBase::DeleteArray<int> > lUnitInAxisPtr(pUnitInAxis);
	CHECK_NEWED_PTR(pUnitInAxis);
	
    int nAttrUnitInAxis = 0;
	int * pAttrUnitInAxis = new int[mnTotalUnits];
    MBase::StrongPtr<int, MBase::DeleteArray<int> > lAttrUnitInAxisPtr(pAttrUnitInAxis);
	CHECK_NEWED_PTR(pAttrUnitInAxis);
    
	int lnOrgUnit = 0;
	int * lpOrgUnit = new int[mnTotalUnits];
    MBase::StrongPtr<int, MBase::DeleteArray<int> > lOrgUnitStrongPtr(lpOrgUnit);
    CHECK_NEWED_PTR(lpOrgUnit);
    int * lpChildUnits= new int[mnTotalUnits];
    MBase::StrongPtr<int, MBase::DeleteArray<int> > lChildUnitsStrongPtr(lpChildUnits);
	CHECK_NEWED_PTR(lpChildUnits);
    
	bool * lpfOrgUnit = new bool[mnTotalUnits];
    MBase::StrongPtr<bool, MBase::DeleteArray<bool> > lfOrgUnitStrongPtr(lpfOrgUnit);
	CHECK_NEWED_PTR(lpfOrgUnit);
    
    
	mCurIndex = new DSSXTabKey[mnTotalUnits];
	CHECK_NEWED_PTR(mCurIndex);
	for(i = 0; i < mnTotalUnits; i++)
		mCurIndex[i] = DSSXTabTotalKey;
    
	mCurMetric = LABLE_NO_METRIC;
	mCurTag = 0;	//original data
    
	DSSXTabLayoutUnitInfo * pUnitInfo = NULL;
	DSSTabularIndex* lpIndices = NULL;
    DSSTabularIndex* lpIndices2 = NULL;
	vector<int> lpTag;
    
	
	vector<DSSTabularIndex *> lppIndices;
    vector<DSSTabularIndex *> lppIndices2;
    vector<bool> lvHasRecursiveUnit;
    
	bool lfMetric = false;
	int lfMetric2 = 0;
	// DSSXTabKey lElementID;
	bool *AttrInAxisMap = new bool[mnTotalUnits];
    MBase::StrongPtr<bool, MBase::DeleteArray<bool> > lAttrInAxisMapStrongPtr(AttrInAxisMap);
	CHECK_NEWED_PTR(AttrInAxisMap);
    
    // tehe: support hide null or zero
    bool lbHideNullorZero = false;
    
    if(mpHideNullZeroMaskInfo)
    {
        DSSTabularIndex *lpMainIndex = mpHideNullZeroMaskInfo->GetMainIndex();
        DSSXTabBitVector *lpBitVector = NULL;
        
        if(mpHideNullZeroMaskInfo->FindIndexMask(lpMainIndex, &lpBitVector))
        {
            lbHideNullorZero = true;
        }
    }
    
    bool lvHasSubtotalInAxis[AXIS_COUNT];
    memset(&lvHasSubtotalInAxis[0], 0, sizeof(bool) * AXIS_COUNT);
    
    
    vector<set<DSSTabularIndex*> > lvvTmpIndex(AXIS_COUNT);
    
    // tehe: at client side, we will not care about the page by axis
	for(i = 0; i < AXIS_COUNT/*3*/; i++)
	{
		hr = pLayout -> get_AxisSize(i, &nUnitInAxis);
		CHECK_HR;
        
		lfMetric = false;
        
		nAttrUnitInAxis = 0;
        
        
		for(j = 0; j < nUnitInAxis; j++)
		{
			hr = pLayout -> get_UnitInfoByAxis(i, j, &pUnitInfo);
			CHECK_HR;
            
			hr = pUnitInfo -> get_UnitID(&k);
			CHECK_HR;
            
            // tehe: here we need not handling page by header
			// hr = pUnitInfo -> get_CurrentElement(&lElementID);
			CHECK_HR;
            
			pUnitInAxis[j] = k;
            
			hr = pUnitInfo -> Is_Metric(&lfMetric2);
			CHECK_HR;
            
			if(lfMetric2)
				lfMetric = true;
			else
				pAttrUnitInAxis[nAttrUnitInAxis ++] = k;
            
            //			if(i == 2)
            //			{
            //				//Page-by
            //				if(lfMetric2)
            //					mCurMetric = lElementID;
            //				else
            //				{
            //					mCurIndex[k] = lElementID;
            //				}
            //			}
            
		}
        
		// generate a boolmap of the attributes in axis
		for(k = 0; k < mnTotalUnits; k++)
            AttrInAxisMap[k] = false;
        
		for(k=0; k<nAttrUnitInAxis; k++)
			AttrInAxisMap[pAttrUnitInAxis[k]] = true;
        
        
        
		//need to go throught the mpLevel and generate the distinct for the
		//units on this axis. For any unit happen to be aggregated, need to annotated with tag
		for(j = 0; j < mnLevels; j++)
		{
			lnOrgUnit = 0;
			bool *AttrMap = NULL;
			int tags = 0,  ctag = 0 ;	// tags is # of tags, ctag is the current tag
            
			//mOrgUnitByLevel intersect pAttrUnitInAxis,
			//we need to have it in the right order {1, 2, 4} not {4, 2, 1}
			//for(k = 0; k < mnTotalUnits; k++)
			//	lpfOrgUnit[k] = false;
            
			mpSliceMap->Count(j, &tags);
			mpSliceMap->GetLevel(j, &AttrMap);
            
            
			//xhan 01/05/05 put the dummy unit on the row axis.
			for(k = 0; k < mnTotalUnits; k++)
			{
				if(AttrMap[k] && AttrInAxisMap[k])
					lpOrgUnit[lnOrgUnit++] = k;
			}
            
            bool lfHasRecursiveUnit = false;
            memmove(&lpChildUnits[0], lpOrgUnit, sizeof(int)*mnTotalUnits);
            DSSTabularData* lpTabularData = NULL;
            hr = mpViewDataSet->getTabularData(&lpTabularData);
            CHECK_HR;
            
            for (int iUnit = 0; iUnit<lnOrgUnit; iUnit++)
            {
                DSSTabularUnit* lpTabularUnit = NULL;
                hr = lpTabularData->ItemC(lpOrgUnit[iUnit], &lpTabularUnit);
                CHECK_HR;
                
                if(lpTabularUnit && lpTabularUnit->IsRecursiveUnit())
                {
                    int lDataUnitIndex = static_cast<DSSXTabRecursiveUnit*>(lpTabularUnit)->getDataUnit();
                    
                    lfHasRecursiveUnit = true;
                    lpChildUnits[iUnit] = lDataUnitIndex;
                }
                
            }
            
            
			hr = pIndicesPool -> getIndex(lnOrgUnit, lpOrgUnit, &lpIndices);
            CHECK_HR;
            
            if (lfHasRecursiveUnit)
            {
                hr = pIndicesPool -> getIndex(lnOrgUnit, lpChildUnits, &lpIndices2);
                CHECK_HR;
            }
            
			if(nAttrUnitInAxis > lnOrgUnit)
			{
				//it is a total
				for (int Itag=0; Itag<tags; Itag++)
				{
					mpSliceMap->Item(j, Itag, &AttrMap, &ctag);
                    
					lpTag.push_back(ctag);
					lppIndices.push_back(lpIndices);
                    lppIndices2.push_back(lpIndices2);
                    lvHasRecursiveUnit.push_back(lfHasRecursiveUnit);
				}
                
                lvHasSubtotalInAxis[i] = true;
			}
			else
			{ // it is not a total on this axis
				if (i == 0 && mDummyUnitIndex != -1)
				{ //if it is the row axis and there is a dummy unit, then we have to put the dummy into
					// the row axis
					lpOrgUnit[lnOrgUnit++] = mDummyUnitIndex;
                    
					sort(lpOrgUnit, lpOrgUnit + lnOrgUnit);
                    
					hr = pIndicesPool -> getIndex(lnOrgUnit, lpOrgUnit, &lpIndices);
					CHECK_HR;
                    
                    
					lppIndices.push_back(lpIndices);
					lpTag.push_back(0);	//reserved tag
                    lppIndices2.push_back(lpIndices2);
                    lvHasRecursiveUnit.push_back(lfHasRecursiveUnit);
				} else {
					lppIndices.push_back(lpIndices);
					lpTag.push_back(0);	//reserved tag
                    lppIndices2.push_back(lpIndices2);
                    lvHasRecursiveUnit.push_back(lfHasRecursiveUnit);
				}
			}
            
            if(lbHideNullorZero && nAttrUnitInAxis <= lnOrgUnit)
            {
                lvvTmpIndex[i].insert(lpIndices);
            }
		}
        
		//this axis does have metric
		//XW: TODO inside lppIndices, the elements may not be distinct. It is
		//possible the same indices appear twice, we need to eliminate that.
		hr = mHeaders[i] -> Init(this, lpTag.size(), &lpTag[0], &lppIndices[0], nUnitInAxis, pUnitInAxis,
                                 mnTotalUnits, lfMetric, mnMetrics, pLayout->getRuntimeUnitCount(i), &lppIndices2[0], lvHasRecursiveUnit);
		CHECK_HR;
        
        if (i == 0 && mDummyUnitIndex != -1)
        {
            mHeaders[i]->mDummyUnitIndex = mDummyUnitIndex;
        }
        
		lppIndices.clear();
		lpTag.clear();
        lpIndices2 = NULL;
        lppIndices2.clear();
        lvHasRecursiveUnit.clear();
	}
    
    if(lbHideNullorZero)
    {
        set<DSSTabularIndex*> lvHideNullorZeroIndex;
        
        if(!lvHasSubtotalInAxis[1])
        {
            // no subtotal in column, store indices in row
            for(set<DSSTabularIndex*>::iterator it = lvvTmpIndex[0].begin(); it != lvvTmpIndex[0].end(); it++)
            {
                lvHideNullorZeroIndex.insert(*it);
            }
        }
        
        if(!lvHasSubtotalInAxis[0])
        {
            // no subtotal in row, store indices in column
            for(set<DSSTabularIndex*>::iterator it = lvvTmpIndex[1].begin(); it != lvvTmpIndex[1].end(); it++)
            {
                lvHideNullorZeroIndex.insert(*it);
            }
        }
        
        DSSXTabBitVector *lpMask = NULL;
        
        for(set<DSSTabularIndex*>::iterator it = lvHideNullorZeroIndex.begin(); it != lvHideNullorZeroIndex.end(); it++)
        {
            mpHideNullZeroMaskInfo->GetCalculatedMask2(*it, &lpMask);
        }
    }
    
	hr = pLayout -> get_Tag(&mCurTag);
	CHECK_HR;
    
	//init pageby header
    //	hr = mHeaders[2]->init();
    //	CHECK_HR;
    
    
	mfStatus = XTAB_INITIALIZED;
    
	hr = hGenerateSliceLevelInfo();
	CHECK_HR;
    
    // update the attribute threshold items
    if (mpAttributeThresholdItems)
    {
        delete mpAttributeThresholdItems;
        mpAttributeThresholdItems = NULL;
    }
    
	return hr = S_OK;
	//## end CDSSXTabView::Init%34C57BAA02E0.body
}

int DSSXTabView::hInitAttributeThresholdItem()
{
    if (!mpViewDataSet)
    {
        // status is incorrect
        return S_FALSE;
    }
    
    // release the old items info
    if (mpAttributeThresholdItems)
    {
        delete mpAttributeThresholdItems;
        mpAttributeThresholdItems = NULL;
    }
    
    mpAttributeThresholdItems = new DSSAttributeThresholdItems();
    // if there is no attribute threshold, return YES
    if (!mpViewDataSet->getCalculationPlan() || (mpViewDataSet->getCalculationPlan()->getAttriID(0) == GUID_NULL)) {
        return S_OK;
    }
    
    return mpAttributeThresholdItems->init(this);
}

int DSSXTabView::SetGBUnits(const vector<GUID>& ivGroupByUnits, DSSGBTable* ipGBTable)
{
    int hr = S_OK;
    
    mpGBTable = ipGBTable;
    mbHasValidGroupBy = false;
    
    if(ivGroupByUnits.size() > 0)
    {
        vector<int> lvGroupByUnitIDs;
        
        hr = ConvertGUIDToLocalUnitID(ivGroupByUnits, lvGroupByUnitIDs);
        hr = SetGBUnits(lvGroupByUnitIDs);
    }
    
	return hr;
    
}

int DSSXTabView::SetGBUnits(const vector<int>& ivGroupByUnits)
{
	int hr = S_OK;
	if(ivGroupByUnits.size() > 0)
    {
        // get metric unit id
        DSSViewDataSet *lpViewDataSet = mpViewDataSet;
        CHECK_HR;
        
        DSSTabularData *lpTabularData = NULL;
        hr = lpViewDataSet->getTabularData(&lpTabularData);
        CHECK_HR;
        
        
        int lMetricUnit = -1;
        DSSTabularMetrics *lpXTabMetrics = NULL;
        hr = lpTabularData -> getMetrics(&lpXTabMetrics);
        CHECK_HR;
        
        lMetricUnit = lpXTabMetrics->getUnitIndex();
        
        
        // check metrics unit is contained in GB units
        int lnGroupByUnits = ivGroupByUnits.size();
        mbHasMetricGroupBy = false;
        mGBMetricPos = -1;
        mnGroupByUnits = 0;
        
        for (int i = 0; i < lnGroupByUnits; i++)
        {
            if (ivGroupByUnits[i] == lMetricUnit)
            {
                // 3/28/2012 tehe: #569925
                if(!mbHasMetricGroupBy)
                {
                    mbHasMetricGroupBy = true;
                    mGBMetricPos = i;
                }
                // break;
            }
            else if(ivGroupByUnits[i]>-1)
            {
                mnGroupByUnits++;
                mbHasValidGroupBy = true;
            }
        }
        
        //
        //        for (int i = 0; i < lnGroupByUnits; i++)
        //        {
        //            if (ivGroupByUnits[i] > -1)
        //            {
        //                mbHasValidGroupBy = true;
        //                break;
        //            }
        //        }
        //
        //        mnGroupByUnits = mbHasMetricGroupBy ? (lnGroupByUnits - 1) : lnGroupByUnits;
        if (mbHasValidGroupBy && mnGroupByUnits >0)
        {
            mpGroupByUnits = new int[mnGroupByUnits];
            mpGroupByUnitsPos = new int[mnGroupByUnits];
            Int32 iGroupByUnits = 0;
            for(Int32 i = 0; i < lnGroupByUnits; i++)
            {
                if (ivGroupByUnits[i] != lMetricUnit && ivGroupByUnits[i] > -1)
                {
                    mpGroupByUnits[iGroupByUnits] = ivGroupByUnits[i];
                    mpGroupByUnitsPos[iGroupByUnits] = i;
                    iGroupByUnits++;
                }
            }
        }
        //        mpGroupByUnits = new int[mnGroupByUnits];
        //
        //        CHECK_NEWED_PTR(mpGroupByUnits);
        //
        //        if (mbHasMetricGroupBy)
        //        {
        //            if (mGBMetricPos > 0)
        //                memmove(mpGroupByUnits, &ivGroupByUnits[0], mGBMetricPos * sizeof(int));
        //            if(mnGroupByUnits - mGBMetricPos > 0)
        //                memmove(mpGroupByUnits + mGBMetricPos, &ivGroupByUnits[mGBMetricPos + 1], (mnGroupByUnits - mGBMetricPos) * sizeof(int));
        //        }
        //        else
        //        {
        //            if (mnGroupByUnits > 0)
        //                memmove(mpGroupByUnits, &ivGroupByUnits[0], mnGroupByUnits * sizeof(int));
        //        }
        
        int lnGBGroups = 1<<mnGroupByUnits;
        mppvMapGBPos2LevelSlice = new vector<int, MBase::Allocator<int> >*[lnGBGroups];
        CHECK_NEWED_PTR(mppvMapGBPos2LevelSlice);
        
	    
        for(int i = 0; i < lnGBGroups; i++)
        {
            mppvMapGBPos2LevelSlice[i] = new vector<int, MBase::Allocator<int> >();
            CHECK_NEWED_PTR(mppvMapGBPos2LevelSlice[i]);
        }
        
        int lnUnits;
        vector<int> lvUnits(mnTotalUnits);
        
        for (int i = 0; i < mSizeSliceLevel; i++)
        {
            DSSTabularIndex* lpCellIndex = mpSliceLevel[i].mpCellIndices;
            if(lpCellIndex == NULL)
                continue;
            
            lnUnits = lpCellIndex->getNumUnits();
            if(lnUnits > 0)
                lpCellIndex->getUnits(lnUnits, &lvUnits[0]);
            
            int iGroup = 0;
            
            for (int j = mnGroupByUnits-1; j >=0; j--)
            {
                bool match = false;
                for (int k = 0; k < lnUnits; k++)
                {
                    if(lvUnits[k] == mpGroupByUnits[j])
                    {
                        match = true;
                        break;
                    }
                }
                iGroup = (iGroup<<1) + (match? 1: 0);
            }
            mppvMapGBPos2LevelSlice[iGroup]->push_back(i);
        }
    }
	return hr;
}

int DSSXTabView::UpdateNumRowAndColumn()
{
    //depending where the metrics is. sometime we will get 0 from
	//header, but we have to claim to have at least one row
	int hr = S_OK;
    
	mRowCount = mHeaders[0] -> getRowCount();
    
	mColCount = mHeaders[1] -> getRowCount();
    
	//if there is no unit (metric or attribute) on row and there is no metric at all
	//we need to set mRowCount to 0
    
	//if there is no unit (metric or attribute) on column and there is no metric at all
	//we need to set mColCount to 0
    
	if(mnMetrics == 0)
	{
        
		if((mHeaders[0] -> mnUnits - mHeaders[0] -> mbHasMetric) == 0)
            
			mRowCount = 0;
        
		if((mHeaders[1] -> mnUnits - mHeaders[1] -> mbHasMetric) == 0)
			mColCount = 0;
	}
	else if (!mRowCount || !mColCount)
	{
		mRowCount = mColCount = 0;
	}
	return hr;
	
}

//int DSSXTabView::hSortHeaders()
//{
//    int hr = S_OK;
//	for (Int32 i = 0; i < 2; i++)
//	{
//        hr = mHeaders[i]->sort();
//		CHECK_HR
//	}
//	return hr;
//}

bool DSSXTabView::isNonMetricsSelectionChanged(const vector<vector<int> >& ivvGBElements)
{
	if ( (mpvLastGBElements == NULL && mpvLastOVGBElements == NULL) && ivvGBElements.size() == 0)
		return false;
	else if ((mpvLastGBElements == NULL && mpvLastOVGBElements == NULL) || ivvGBElements.size() == 0)
		return true;
	else if (mnGroupByUnits > 0 && !mpvLastGBElements)
		return true;
	else if (mnOVGBUnits > 0 && !mpvLastOVGBElements)
		return true;
	
	Int32 lMappedPos = -1;
	for (Int32 i = 0; i < mnGroupByUnits; i++)
	{
		lMappedPos = mpGroupByUnitsPos[i];
        
		if (mpvLastGBElements[i]->size() != ivvGBElements[lMappedPos].size())
		{
			return true;
		}
        
		for (Int32 j = 0; j < mpvLastGBElements[i]->size(); j++)
		{
			if ((*mpvLastGBElements[i])[j] != ivvGBElements[lMappedPos][j])
			{
				return true;
			}
		}
	}
    
	for (Int32 i = 0; i < mnOVGBUnits; i++)
	{
		lMappedPos = mpOVGBUnitPos[i];
		if (mpvLastOVGBElements[i]->size() != ivvGBElements[lMappedPos].size())
		{
			return true;
		}
        
		for (Int32 j = 0; j < mpvLastOVGBElements[i]->size(); j++)
		{
			if ((*mpvLastOVGBElements[i])[j] != ivvGBElements[lMappedPos][j])
			{
				return true;
			}
		}
	}
	return false;
}

int DSSXTabView::ResetCurrentSelection(const vector<vector<int> >& ivvGBElements, int iTag, bool ibGBElementsChanged)
{
    // 8/13/2012 tehe: we need always do pre-xtab now
    //	if (mDoPreXTab)
    //	{
    //		return S_OK;
    //	}
	int hr = S_OK;
	
	vector<int> lvMetrics;
    vector<int> lvMetricsNeetToLoad; //zhyang, 825456, when there is only metric group-by, we need to load those metric slice that used to do sorting
    
    // tehe 1/16/2012 GBElements may be empty
	if (mbHasMetricGroupBy && mGBMetricPos < ivvGBElements.size())
    {
        lvMetrics.assign(ivvGBElements[mGBMetricPos].begin(), ivvGBElements[mGBMetricPos].end());
        lvMetricsNeetToLoad = lvMetrics;
        
        DSSTemplate *lpTemplate;
        hr = mpViewDataSet->getTemplate(&lpTemplate);
        CHECK_HR;
        for(int iAxis = 0; iAxis < 2; ++iAxis)
        {
            DSSAxis* lpAxis = lpTemplate->getAxis(iAxis+1);
            if(lpAxis)
            {
                DSSSorts* lpsorts=lpAxis->getSorts();
                for(int iSort=0;iSort<lpsorts->Count();++iSort)
                {
                    bool *lpLevel = new bool[mnTotalUnits];
                    MBase::StrongPtr<bool, MBase::DeleteArray<bool> > pLevel(lpLevel);
                    fill(lpLevel, lpLevel+mnTotalUnits, false);
                    
                    DSSSort* lpSort = lpsorts->Item(iSort);
                    if(lpSort->getSortType() == DssSortMetric)
                    {
                        DSSTabularMetrics* metrics = mpViewDataSet->getTabularData()->getTabularMetircs();
                        int metricID;
                        GUID guid=lpSort->getObjectID();
                        if(metrics==NULL || (metricID=metrics->FindMetric(guid))<0)
                            continue;
                        
                        bool ok = true;
                        for(int i = 0; i < lvMetricsNeetToLoad.size(); ++i)
                            if(metricID == lvMetricsNeetToLoad[i])
                                ok = false;
                        if(ok)
                            lvMetricsNeetToLoad.push_back(metricID);
                    }
                }
            }
        }
    }
    
	if ((mnGroupByUnits == 0 && mnOVGBUnits == 0) || !mbHasValidGroupBy )
	{ // if it has no CGB or only contains CGB over metrics
        // 5/24/2012 tehe: if there is no GB units, we should load all the slice
		hr = S_OK;
        // 6/20/2012 tehe: optimization for metric selector,only load the metric slice
        if (mbHasMetricGroupBy && mGBMetricPos < ivvGBElements.size())
        {
            //hr = hLoadSliceByMetrics(lvMetrics);
            hr = hLoadSliceByMetrics(lvMetricsNeetToLoad);
            CHECK_HR;
            
            // 6/29/2012 tehe
            mvGBMetrics.assign(lvMetrics.begin(), lvMetrics.end());
        }
        else
        {
            hr = LoadAllSliceData();
            CHECK_HR;
        }
        
		// bool lfDoSort = false;
		if(!mHasInitialized)
		{
			// lfDoSort = true;
			hr = mHeaders[0]->init();
			if (FAILED(hr)) return hr;
            
			hr = mHeaders[1]->init();
			if (FAILED(hr)) return hr;
            
            // 6/29/2012 tehe
            //			hr = mHeaders[0]->InitMetricGroupBy(mbHasMetricGroupBy, lvMetrics);
            //			if (FAILED(hr)) return hr;
            //
            //			hr = mHeaders[1]->InitMetricGroupBy(mbHasMetricGroupBy, lvMetrics);
            //			if (FAILED(hr)) return hr;
            
			mHasInitialized = true;
		}
		
		if (mbHasMetricGroupBy && mHasInitialized)
		{
			hr = mHeaders[0]->InitMetricGroupBy(mbHasMetricGroupBy, lvMetrics);
			if (FAILED(hr)) return hr;
            
			hr = mHeaders[1]->InitMetricGroupBy(mbHasMetricGroupBy, lvMetrics);
			if (FAILED(hr)) return hr;
		}
        
		hr = UpdateNumRowAndColumn();
        
        // 8/28/2012 tehe: we have did pre-sort in init(xtabview,....) method of xtabheader
        //		if(lfDoSort)
        //			hr = hSortHeaders();
        
		return hr;
	}
	
	// check if it's same with last selection
	if(!ibGBElementsChanged)
	{
        int hr = hLoadSliceByMetrics(lvMetrics);
        CHECK_HR;
        
        // 6/29/2012 tehe
        mvGBMetrics.assign(lvMetrics.begin(), lvMetrics.end());
        
		// only Metrics selection is changed. Need to further check if they are also the same
		// TODO
		hr = mHeaders[0]->InitMetricGroupBy(mbHasMetricGroupBy, lvMetrics);
		if (FAILED(hr)) return hr;
        
		hr = mHeaders[1]->InitMetricGroupBy(mbHasMetricGroupBy, lvMetrics);
		if (FAILED(hr)) return hr;
		
		hr = UpdateNumRowAndColumn();
		return hr;
	}
	else // save current selection to mpvLastGBElements
	{
        // 6/29/2012 tehe
        if(mbHasMetricGroupBy)
        {
            mvGBMetrics.assign(lvMetrics.begin(), lvMetrics.end());
        }
        
        // tehe 4/19/2012:avoid memory leak
        if(!mpvLastGBElements && mnGroupByUnits >0)
        {
            mpvLastGBElements = new vector<int>*[mnGroupByUnits];
            CHECK_NEWED_PTR(mpvLastGBElements);
            
            for(int i = 0; i < mnGroupByUnits; i++)
            {
                mpvLastGBElements[i] = new vector<int>();
                CHECK_NEWED_PTR(mpvLastGBElements[i]);
            }
        }
        
        // 9/10/2012 tehe
        if(!mpvLastOVGBElements && mnOVGBUnits > 0)
        {
            mpvLastOVGBElements = new vector<int>*[mnOVGBUnits];
            CHECK_NEWED_PTR(mpvLastOVGBElements);
            
            for(int i = 0; i < mnOVGBUnits; i++)
            {
                mpvLastOVGBElements[i] = new vector<int>();
                CHECK_NEWED_PTR(mpvLastOVGBElements[i]);
            }
            
        }
        
		for(int i = 0; i < mnGroupByUnits; i++)
		{
			if(mpGroupByUnitsPos[i] < ivvGBElements.size())
                mpvLastGBElements[i]->assign(ivvGBElements[mpGroupByUnitsPos[i]].begin(), ivvGBElements[mpGroupByUnitsPos[i]].end());
		}
        
        for(int i=0; i< mnOVGBUnits; i++)
        {
			if(mpOVGBUnitPos[i] < ivvGBElements.size())
                mpvLastOVGBElements[i]->assign(ivvGBElements[mpOVGBUnitPos[i]].begin(), ivvGBElements[mpOVGBUnitPos[i]].end());
        }
        
        // delete the attribute threshold to force it refresh after group by unit changed
        if (mpAttributeThresholdItems)
        {
            delete mpAttributeThresholdItems;
            mpAttributeThresholdItems = NULL;
        }
	}
    
	vector<vector<int> > lvvGBElements(mnGroupByUnits);
	for (int i = 0; i < mnGroupByUnits; i++)
	{
		lvvGBElements[i].assign(mpvLastGBElements[i]->begin(), mpvLastGBElements[i]->end());
	}
    
	vector<vector<int> > lvvValidGBUnitsPos;
	vector<vector<DSSTabularIndex*> > lvvCellIndices;
	vector<vector<DSSTabularIndex*> > lvvRowIndices;
	vector<vector<DSSTabularIndex*> > lvvColumnIndices;
    
	int i;
	int j;
	int k;
    
	//SetGBUnits(iGroupByUnitSize, ipGroupByUnits, iHasMetricGroupBy);
    
	// check if each Groupby units contains Non-Total elements and Total elements
	vector<bool> hasNonTotalElems(mnGroupByUnits);
	vector<bool> hasTotalElems(mnGroupByUnits);
    
	for ( i = 0; i < mnGroupByUnits; i++)
	{
		hasNonTotalElems[i] = false;
		hasTotalElems[i] = false;
		for (j = 0; j < lvvGBElements[i].size(); j++)
		{
			if (!hasNonTotalElems[i] && (lvvGBElements[i][j] >= 0 || lvvGBElements[i][j] == -1))// -1 for "ALL" element
			{
				hasNonTotalElems[i] = true;
			}
			if(!hasTotalElems[i] && (lvvGBElements[i][j] == DSSXTAB_TOTAL_KEY || lvvGBElements[i][j] == -1))// -1 for "All" element, so "Total" is contained.
			{
				hasTotalElems[i] = true;
			}
			if( hasNonTotalElems[i] && hasTotalElems[i]) break;
		}
	}
    
	// find indices group with valid GB units
	map<DSSTabularIndex*, int > lMapRowIndex2GBUnits;
	map<DSSTabularIndex*, int >::iterator itRow;
	map<DSSTabularIndex*, vector<int> > lMapRowIndex2Tags;
	map<DSSTabularIndex*, int > lMapColumnIndex2GBUnits;
	map<DSSTabularIndex*, int >::iterator itCol;
	map<DSSTabularIndex*, vector<int> > lMapColumnIndex2Tags;
    
	int lnGBGroups = 1<<mnGroupByUnits;
	for (i = 0; i < lnGBGroups; i++)
	{
		if(mppvMapGBPos2LevelSlice[i] == NULL || mppvMapGBPos2LevelSlice[i]->size() == 0)
		{
			continue;
		}
		
		bool lIsValid = true;
		bool lHasTotal = false;
		vector<int> lvGBUnitsPos;
		
		// if an index is valid for current selection, for each GB unit, one of following condition should be true
		//    1. if it is conatined in the index, at least a non-total element must be selected.
		//    2. else, total element must selected
		for (j = 0; j < mnGroupByUnits; j++)
		{
			if (mpGroupByUnits[j] < 0) // if the unit is not include, ignore.
			{
				continue;
			}
			else if(i&(0x1<<j)) // if j-th GB units is contained in units list of cell index of this group
			{
				lvGBUnitsPos.push_back(j);
                
				if (!hasNonTotalElems[j])
				{//there is no non-total element, this index is not valid for current selection
					lIsValid = false;
				}
			}
			else
			{// j-th GB units is not in the unit list of index
				if (!hasTotalElems[j])
				{//there is no total element, this index is not valid for current selection
					lIsValid = false;
					break;
				}
				lHasTotal = true;
			}
		}
        
		if(lIsValid)
		{
			lvvValidGBUnitsPos.push_back(lvGBUnitsPos);
			int lPos = lvvValidGBUnitsPos.size() - 1;
            
			int lnSliceLevels = mppvMapGBPos2LevelSlice[i]->size();
			vector<DSSTabularIndex *> lvCellIndices;
			for (k = 0; k < lnSliceLevels; k++)
			{
				DSSSliceLevel lSliceLevel = mpSliceLevel[(*mppvMapGBPos2LevelSlice[i])[k]];
				DSSTabularIndex* lpCellIndices = lSliceLevel.mpCellIndices;
				if (lpCellIndices == NULL)
					continue;
				lvCellIndices.push_back(lpCellIndices);
				
				itRow = lMapRowIndex2GBUnits.find(lSliceLevel.mpRowIndices);
				if (itRow != lMapRowIndex2GBUnits.end())
				{
				    // if the row indices is already added to map, use the cell index which has less gbUnits
				    // because suppose GB=(Year,City), both (Year,Country,City,Category) and (Country,City,Category) genereate (City,Category),
					// we will choose  (Country, City, Category)  to generate (City, Category),
					// because BV( (Year, Country, City, Category) filtered by (Year=05,City="NY"))
					//		| BV((Country, City, Category) filtered by(City="NY))
					//		= BV((Country, City, Category) filtered by(City="NY))
					if(lvGBUnitsPos.size() < lvvValidGBUnitsPos[itRow->second].size())
						lMapRowIndex2GBUnits[lSliceLevel.mpRowIndices] = lPos;
				}
				else
				{
					lMapRowIndex2GBUnits[lSliceLevel.mpRowIndices] = lPos;
				}
                
				itCol = lMapColumnIndex2GBUnits.find(lSliceLevel.mpColIndices);
				if (itCol != lMapColumnIndex2GBUnits.end())
				{   // same reason to handle row indices
					if(lvGBUnitsPos.size() < lvvValidGBUnitsPos[itCol->second].size())
						lMapColumnIndex2GBUnits[lSliceLevel.mpColIndices] = lPos;
				}
				else
				{
					lMapColumnIndex2GBUnits[lSliceLevel.mpColIndices] = lPos;
				}
                
				// rzhu, 05/25/2011, process tags
				int iAxis = 0;
				do
				{
					map<DSSTabularIndex*, vector<int> >::iterator itTags;
					map<DSSTabularIndex*, vector<int> >::iterator itTags2;
					DSSTabularIndex * lpIndices;
					if (iAxis == 0)
					{
						lpIndices = lSliceLevel.mpRowIndices;
						itTags = lMapRowIndex2Tags.find(lpIndices);
						itTags2 = lMapRowIndex2Tags.end();
					}
					else
					{
						lpIndices = lSliceLevel.mpColIndices;
						itTags = lMapColumnIndex2Tags.find(lpIndices);
						itTags2 = lMapColumnIndex2Tags.end();
					}
                    
					if (itTags == itTags2)
					{
						vector<int> lTags;
						if(lHasTotal)
							lTags.push_back(iTag);
						else
							lTags.push_back(-1);
						//xzeng 2011-08-16 Change lpCellIndices to lpIndices.
						if (iAxis == 0)
							lMapRowIndex2Tags[lpIndices] = lTags;
						else
							lMapColumnIndex2Tags[lpIndices] = lTags;
					}
					else
					{
						if (!lHasTotal)
						{
							itTags->second.clear();
							itTags->second.push_back(-1);
						}
						else
						{
							int lTag = 0;
							for (lTag = 0; lTag < itTags->second.size(); lTag++)
							{
								if (itTags->second[lTag] == iTag)
									break;
							}
							if (lTag == itTags->second.size())
								itTags->second.push_back(iTag);
						}
					}
					iAxis++;
				}while(iAxis < 2);
                
			}// end of for each slice level with same gbUnits
			lvvCellIndices.push_back(lvCellIndices);
		}
		
	}
	
	lvvRowIndices.resize(lvvValidGBUnitsPos.size());
	lvvColumnIndices.resize(lvvValidGBUnitsPos.size());
	
	itRow = lMapRowIndex2GBUnits.begin();
	while(itRow != lMapRowIndex2GBUnits.end())
	{
		lvvRowIndices[itRow->second].push_back(itRow->first);
		itRow++;
	}
    
	itCol = lMapColumnIndex2GBUnits.begin();
	while(itCol != lMapColumnIndex2GBUnits.end())
	{
		lvvColumnIndices[itCol->second].push_back(itCol->first);
		itCol++;
	}
    
    // 5/21/2012 tehe: get report level index
    if(!mpReportLevelIndex)
    {
        hr = hGetReportLevelIndex(&mpReportLevelIndex);
        CHECK_HR;
    }
    
    int lnRow;
    lnRow = mpReportLevelIndex->Count();
    
	if (lnRow == 0)
		return S_OK;
    
    DSSXTabBitVector lMaskForOVGBUnits;
	lMaskForOVGBUnits.Init(lnRow, false);
	if (lnRow > 0)
		lMaskForOVGBUnits.BulkSet(0, lnRow - 1, false);
    
	if (mpGBTable)
	{
		hr = hGetMaskByOVGBUnits(ivvGBElements, lMaskForOVGBUnits);
		CHECK_HR;
	}
    
    // get bitvector for each gbunits.
	// need to avoid get the unnecessary bitvector later
	// jxie, 2012-5-20, only use BitVector when the selection has several levels
	vector<DSSXTabBitVector> lpResultBVs(mnGroupByUnits);
	if (lvvValidGBUnitsPos.size() > 1)
	{
		for (i = 0; i < mnGroupByUnits; i++)
		{
			mpReportLevelIndex->CalculateMask(1, &mpGroupByUnits[i], &lvvGBElements[i], lpResultBVs[i]);
		}
	}
    
    // 5/21/2012 tehe
    // vector<DSSTabularIndex*> lvCellInices;
    vector<DSSTabularIndex*> lvSliceInices;
    // vector<DSSXTabBitVector *> lvCellBitVector;
    vector<DSSXTabBitVector*> lvSliceBitVector;
    // vector<vector<int> > lvvSliceID;
    vector<int> lvSliceID;
    
	vector<DSSTabularIndex* > lvRowInices;
	vector<DSSXTabBitVector *> lvRowBitVector;
	vector<DSSTabularIndex* > lvColumnIndices;
	vector<DSSXTabBitVector *> lvColumnBitVector;
	vector<vector<int> > lvRowTags;
	vector<vector<int> > lvColumnTags;
	
    // 6/1/2012 tehe
    vector<int> lvTempMetrics;
    if(mbHasMetricGroupBy)
    {
        lvTempMetrics = lvMetrics;
    }
    else
    {
        for(int i=0;i<mnMetrics;i++)
        {
            lvTempMetrics.push_back(i);
        }
    }
    
    vector<DSSMaskInfos> lvMaskInfos(lvvValidGBUnitsPos.size());
    
	for (i = 0; i < lvvValidGBUnitsPos.size(); i++)
	{
		lvMaskInfos[i].Init(mpReportLevelIndex, mpCube /*ipCondition*/);
		bool lCalculated = false;
		DSSXTabBitVector* lpMask = NULL;
		lvMaskInfos[i].GetMask(mpReportLevelIndex, &lpMask, &lCalculated);
		lpMask->BulkSet(0, lnRow - 1, true);
		Int32 lnValidGBUnits = lvvValidGBUnitsPos[i].size();
        
		if(lnValidGBUnits == 0)
		{ // in this case, non of the bg units is valid
            
		}
		else
		{
			if (lvvValidGBUnitsPos.size() > 1)
			{
				for (j = 0; j < lnValidGBUnits; j++)
					lpMask->CalculateAnd(&lpResultBVs[lvvValidGBUnitsPos[i][j]]);
			}
			else
			{
				// jxie, 2012-5-20, calculate mask directly from all the valid GB units
				vector<Int32> lvValidUnits(lnValidGBUnits);
				vector<vector<Int32> > lvvValidElements;
				for (j = 0; j < lnValidGBUnits; j++)
				{
					lvValidUnits[j] = mpGroupByUnits[ lvvValidGBUnitsPos[i][j] ];
					lvvValidElements.push_back(lvvGBElements[ lvvValidGBUnitsPos[i][j] ]);
				}
                
				mpReportLevelIndex->CalculateMask(lnValidGBUnits, &lvValidUnits[0], &lvvValidElements[0], *lpMask);
			}
		}
        if(mpGBTable)
            lpMask->CalculateAnd(&lMaskForOVGBUnits);
		// sorting lvvCellIndices, lvvrowIndices, lvvColumnIndices descending is to guarantee that
		// BitVector of lower level indices will be generated before BitVector of higher level indices
        // 6/1/2012 tehe
        sort(lvvCellIndices[i].begin(), lvvCellIndices[i].end(), mIndexUnitComparator);
		sort(lvvRowIndices[i].begin(), lvvRowIndices[i].end(), mIndexUnitComparator);
		sort(lvvColumnIndices[i].begin(), lvvColumnIndices[i].end(), mIndexUnitComparator);
        
        // 6/1/2012 tehe
        for(j=0;j <lvvCellIndices[i].size();j++)
        {
            // 6/18/2012 tehe: skip same index
            if(j < lvvCellIndices[i].size()-1 && lvvCellIndices[i][j] == lvvCellIndices[i][j+1])
                continue;
            
            // lvCellInices.push_back(lvvCellIndices[i][j]);
            bool *lpLevel = new bool[mnTotalUnits];
            MBase::StrongPtr<bool, MBase::DeleteArray<bool> > pLevel(lpLevel);
            fill(lpLevel, lpLevel+mnTotalUnits, false);
            vector<int> lvUnits;
            lvvCellIndices[i][j]->getUnits(lvUnits);
            for(int k=0;k<lvUnits.size(); k++)
            {
                lpLevel[lvUnits[k]] = true;
            }
            vector<int> lvTmpSliceID;
            int ret = mpSliceMap->getSlicesByLevel(lpLevel,lvTempMetrics,lvTmpSliceID);
            if(ret != S_OK)
            {
                lvTmpSliceID.clear();
            }
            
            // jzeng 04/08/16 load attribute threshold slice
            getAttributeThresholdSliceByLevel(lpLevel, lvTmpSliceID);
            
            // 6/4/2012 tehe
            if(lvTmpSliceID.size() >0)
            {
                // DSSXTabBitVector *lpBV = NULL;
                // lvMaskInfos[i].GetCalculatedMask2(lvvCellIndices[i][j], &lpBV);
                for(int k=0; k<lvTmpSliceID.size(); k ++)
                {
                    vector<int>::iterator result = find(lvSliceID.begin(), lvSliceID.end(), lvTmpSliceID[k]);
                    if(result == lvSliceID.end())
                    {
                        DSSCubeDataSlice *lpCubeSlice = mpCube->getSliceByID(lvTmpSliceID[k]);
                        if (lpCubeSlice == NULL)
                            continue;
                        lvSliceInices.push_back(lpCubeSlice->getIndices());
                        lvSliceID.push_back(lvTmpSliceID[k]);
                        DSSXTabBitVector *lpBV = NULL;
                        
                        if (lpCubeSlice && lpCubeSlice->getIndices())
                        {
                            lvMaskInfos[i].GetCalculatedMask2(lpCubeSlice->getIndices(), &lpBV);
                            lvSliceBitVector.push_back(lpBV);
                        }
                    }
                }
            }
        }
		
		map<DSSTabularIndex*, vector<int> >::iterator itTags;
		for (j = 0; j < lvvRowIndices[i].size(); j++)
		{
			lvRowInices.push_back(lvvRowIndices[i][j]);
			DSSXTabBitVector * lpBV = NULL;
			lvMaskInfos[i].GetCalculatedMask2(lvvRowIndices[i][j], &lpBV);
            
			lvRowBitVector.push_back(lpBV);
			itTags = lMapRowIndex2Tags.find(lvvRowIndices[i][j]);
			if (itTags == lMapRowIndex2Tags.end())
			{
				// shouldn't enter this section
				vector<Int32> lvTags(1, -1);
				lvRowTags.push_back(lvTags);
			}
			else
			{
				lvRowTags.push_back(itTags->second);
			}
		}
		for (j = 0; j < lvvColumnIndices[i].size(); j++)
		{
			lvColumnIndices.push_back(lvvColumnIndices[i][j]);
			DSSXTabBitVector * lpBV = NULL;
			lvMaskInfos[i].GetCalculatedMask2(lvvColumnIndices[i][j], &lpBV);
			lvColumnBitVector.push_back(lpBV);
			itTags = lMapColumnIndex2Tags.find(lvvColumnIndices[i][j]);
			if (itTags == lMapColumnIndex2Tags.end())
			{
				// shouldn't enter this section
				vector<Int32> lvTags(1, -1);
				lvColumnTags.push_back(lvTags);
			}
			else
			{
				lvColumnTags.push_back(itTags->second);
			}
		}
	}
    
    
    //zhyang,  733189
    DSSTemplate *lpTemplate;
    hr = mpViewDataSet->getTemplate(&lpTemplate);
    CHECK_HR;
    for(int iAxis = 0; iAxis < 2; ++iAxis)
    {
        DSSAxis* lpAxis = lpTemplate->getAxis(iAxis+1);
        if(lpAxis)
        {
            DSSSorts* lpsorts=lpAxis->getSorts();
            if (!lpsorts) continue;
            for(int iSort=0;iSort<lpsorts->Count();++iSort)
            {
                bool *lpLevel = new bool[mnTotalUnits];
                MBase::StrongPtr<bool, MBase::DeleteArray<bool> > pLevel(lpLevel);
                fill(lpLevel, lpLevel+mnTotalUnits, false);
                
                DSSSort* lpSort = lpsorts->Item(iSort);
                if(lpSort->getSortType() == DssSortMetric)
                {
                    DSSTabularMetrics* metrics = mpViewDataSet->getTabularData()->getTabularMetircs();
                    int metricID;
                    GUID guid=lpSort->getObjectID();
                    if(metrics==NULL || (metricID=metrics->FindMetric(guid))<0)
                        continue;
                    
                    bool ok = true;
                    DSSBaseElementsProxy* elements=lpSort->getElements();
                    if(elements!=NULL)
                    {
                        for(int iElement=0;iElement<elements->Count();++iElement)
                        {
                            DSSBaseElementProxy* element=elements->Item(iElement);
                            int unit, key;
                            if(element->getElementType() != DssElementMetric)
                            {
                                if(mpViewDataSet->getTabularData()->FindElement(element, unit, key)==S_OK)
                                    lpLevel[unit] = true;
                                else
                                {
                                    ok = false;
                                    break;
                                }
                            }
                        }
                    }
                    if(ok)
                    {
                        DSSXTabHeader *lpHeader = getHeader(iAxis);
                        for (int i = 0; i < lpHeader->mvUnits.size(); ++i)
                        {
                            lpLevel[lpHeader->mvUnits[i]] = true;
                        }
                        
                        vector<int> lvMetricID;
                        lvMetricID.push_back(metricID);
                        vector<int> lvTmpSliceID;
                        int ret = mpSliceMap->getSlicesByLevel(lpLevel,lvMetricID,lvTmpSliceID);
                        if(ret != S_OK)
                        {
                            continue;
                        }
                        
                        if(lvTmpSliceID.size() > 0)
                        {
                            //zhyang, if we go into here, there is only one element in lvTmpSliceID;
                            int i = -1;
                            for (i = 0; i < lvSliceID.size(); ++i)
                            {
                                if(lvTmpSliceID[0] == lvSliceID[i])
                                {
                                    if(lvSliceBitVector[i] != NULL)
                                    {
                                        lvSliceBitVector[i] = NULL;
                                    }
                                    break;
                                }
                            }
                            if(i == lvSliceID.size())
                            {
                                //zhyang, not found, the slice used to sort is not displayed
                                DSSCubeDataSlice *lpCubeSlice = mpCube->getSliceByID(lvTmpSliceID[0]);
                                lvSliceInices.push_back(lpCubeSlice->getIndices());
                                lvSliceID.push_back(lvTmpSliceID[0]);
                                lvSliceBitVector.push_back(NULL);
                            }
                        }
                    }
                }
            }
        }
    }
    
    
    if(mbOnDemandLoading)
    {
        if(lvSliceInices.size() >0 )
        {
            hr = mpViewDataSet->LoadSliceDataFromDataSource(lvSliceInices.size(), &(lvSliceBitVector[0]), &(lvSliceID[0]));
            CHECK_HR;
        }
    }
    
    if(lvRowInices.size() > 0)
	{
		hr = mHeaders[0]->InitBySelection(lvRowInices.size(), &lvRowInices[0], &lvRowBitVector[0], mbHasMetricGroupBy, lvMetrics, &lvRowTags[0]);
		CHECK_HR;
	}
	else
	{
		hr = mHeaders[0]->InitBySelection(0, NULL, NULL, mbHasMetricGroupBy, lvMetrics, NULL);
		CHECK_HR;
	}
    
	if(lvColumnIndices.size() > 0)
	{
		hr = mHeaders[1]->InitBySelection(lvColumnIndices.size(), &lvColumnIndices[0], &lvColumnBitVector[0], mbHasMetricGroupBy, lvMetrics, &lvColumnTags[0]);
		CHECK_HR;
	}
	else
	{
		hr = mHeaders[1]->InitBySelection(0, NULL, NULL, mbHasMetricGroupBy, lvMetrics, NULL);
		CHECK_HR;
	}
	
	hr = UpdateNumRowAndColumn();

    // 8/28/2012 tehe: we have did pre-sort in init(xtabview,....) method of xtabheader
	// hr = hSortHeaders();
	
	return hr;
    
}

int DSSXTabView::ConvertGUIDToLocalUnitID(const vector<GUID>& irvGUIDs, vector<int>& orvIDs)
{
    int hr = S_OK;
	// get metric unit id
	DSSViewDataSet *lpViewDataSet = mpViewDataSet;
    CHECK_HR;
    
    DSSTabularData *lpTabularData = NULL;
    hr = lpViewDataSet->getTabularData(&lpTabularData);
	CHECK_HR;
    
	int lnGBUnits = irvGUIDs.size();
    
    vector<Int32> lvOVGBUnits;
	vector<Int32> lvOVGBUnitsPos;
    
	for (int i = 0; i < lnGBUnits; i++)
	{
		int lIndex = -1;
		if (ATL::InlineIsGUIDNULL(irvGUIDs[i]))// Metrics Unit
		{
			DSSTabularMetrics *lpXTabMetrics = NULL;
			int hr = lpTabularData -> getMetrics(&lpXTabMetrics);
			CHECK_HR;
			
            lIndex = lpXTabMetrics->getUnitIndex();
            
			orvIDs.push_back(lIndex);
			continue;
		}
		hr = lpTabularData->FindUnitI(irvGUIDs[i], DssTemplateReserved, lIndex);
		if (hr != S_OK) // can't find unit.
		{
			lIndex = -1;
			hr = S_OK;
            
            //if not found in the view, check GBTable
			if (mpGBTable)
			{
				Int32 lUnit;
				GUID lID = irvGUIDs[i];
				hr = mpGBTable->FindUnitI(&lID, &lUnit);
				if (hr == S_OK)
				{
					lvOVGBUnits.push_back(lUnit);
					lvOVGBUnitsPos.push_back(i);
					mbHasValidGroupBy = true;
				}
			}
		}
        
		orvIDs.push_back(lIndex);
	}
    
    if (mpGBTable && lvOVGBUnits.size() > 0)
	{
		AE_ASSERT(lvOVGBUnits.size() == lvOVGBUnitsPos.size());
        
		mnOVGBUnits = lvOVGBUnits.size();
		mpOVGBUnits = new Int32[mnOVGBUnits];
		mpOVGBUnitPos = new Int32[mnOVGBUnits];
		CHECK_NEWED_PTR(mpOVGBUnitPos && mpOVGBUnits);
		memmove(mpOVGBUnits, &lvOVGBUnits[0], sizeof(Int32) * mnOVGBUnits);
		memmove(mpOVGBUnitPos, &lvOVGBUnitsPos[0], sizeof(Int32) * mnOVGBUnits);
	}
   	return hr;
}


int DSSXTabView::GetAvailableElements(vector<GUID>& irSelectedUnits,
                                      vector<vector<int> >& irSelectedElements,
                                      GUID iTargetUnit,
                                      vector<int>& orElements)
{
	int hr = S_OK;
	vector<int> lvSelectedUnits;
	hr = ConvertGUIDToLocalUnitID(irSelectedUnits, lvSelectedUnits);
	CHECK_HR;
    
	vector<GUID> lvTargetUnitGUIDs(1, iTargetUnit);
	vector<int> lvTargetUnits;
	hr = ConvertGUIDToLocalUnitID(lvTargetUnitGUIDs, lvTargetUnits);
	CHECK_HR;
    
	hr = GetAvailableElements(lvSelectedUnits, irSelectedElements, lvTargetUnits, &orElements);
	return hr;
}

int DSSXTabView::GetAvailableElements(vector<GUID>& irSelectedUnits,
                                      vector<vector<int> >& irSelectedElements,
                                      vector<GUID>& irTargetUnits,
                                      vector<int>* opvElements)
{
	int hr = S_OK;
	vector<int> lvSelectedUnits;
	hr = ConvertGUIDToLocalUnitID(irSelectedUnits, lvSelectedUnits);
	CHECK_HR;
    
	vector<int> lvTargetUnits;
	hr = ConvertGUIDToLocalUnitID(irTargetUnits, lvTargetUnits);
	CHECK_HR;
    
	hr = GetAvailableElements(lvSelectedUnits, irSelectedElements, lvTargetUnits, opvElements);
	return hr;
}

int DSSXTabView::GetAvailableElements(vector<int>& irSelectedUnits,
                                      vector<vector<int> >& irSelectedElements,
                                      vector<int>& irTargetUnits,
                                      vector<int>* opvElements)
{
	int hr = S_OK;
    
	// get main index
    if(!mpReportLevelIndex)
        hGetReportLevelIndex(&mpReportLevelIndex);
    
    DSSTabularIndex * lIndex = mpReportLevelIndex;
	int lnRow;
	lnRow = lIndex->Count();
	
	if (lnRow == 0)
	{// if the index contains no row, return no element.
		return S_OK;
	}
    
	int lnUnits = 0;
	vector<int> lvUnits;
	lnUnits = lIndex->getNumUnits();
	CHECK_HR;
    
	if (lnUnits > 0){
		lvUnits.resize(lnUnits);
		hr = lIndex->getUnits(lnUnits, &lvUnits[0]);
		CHECK_HR;
	}
    
	// check each target unit is included in main index. if not, return -1 to indicate
	vector<int> lvContainedTargetUnitPos;
	vector<int> lvTargetUnitPosInIndex;
	int lnTargetUnits = irTargetUnits.size();
    
    // DSSViewDataSet *lpViewDataSet = mpViewDataSet;
    DSSTabularData *lpTabularData = NULL;
    hr = mpViewDataSet->getTabularData(&lpTabularData);
    CHECK_HR;
    
	DSSTabularMetrics* lpMetrics = NULL;
	hr = lpTabularData->getMetrics(&lpMetrics);
	CHECK_HR;
    
	int lMetricIndex = -1;
	if (lpMetrics!= NULL)
	{
		lMetricIndex = lpMetrics->getUnitIndex();
	}
    
	for (int i = 0; i < lnTargetUnits; i++)
	{
		int j = 0;
		if (lMetricIndex == irTargetUnits[i] && irTargetUnits[i] != -1)
		{
			for (; j < mnMetrics; j++)
			{
				opvElements[i].push_back(j);
			}
			continue;
		}
		for (; j < lnUnits; j++)
		{
			if (lvUnits[j] == irTargetUnits[i]) // find it in index
			{
				lvContainedTargetUnitPos.push_back(i);
				lvTargetUnitPosInIndex.push_back(lvUnits[j]);
				break;
			}
		}
		// the target unit is not include in main index, do dothing
		//if (j == lnUnits)
        
	}
    
    
	int lnSelectedUnits = irSelectedUnits.size();
	int lnEffectiveUnits = 0;
	vector<int> lvEffectiveUnits(lnSelectedUnits);
	vector<vector<int> > lvEffectiveElements(lnSelectedUnits);
    
	// TQMS, 747592, tehe: gb elements get from parent may be empty
	for (int i = 0; i < lnSelectedUnits && i < irSelectedElements.size(); i++)
	{
		bool lfContainTotal = false;
		int lnElements = irSelectedElements[i].size();
		for (int j = 0; j < lnElements; j++)
		{
			if (irSelectedElements[i][j] == DSSXTAB_TOTAL_KEY)
			{
				lfContainTotal = true;
				break;
			}
		}
		if (lfContainTotal)
		{
			continue;
		}
		else
		{
			// check if duplicate units
			int k = 0;
			for (k = 0; k < lnEffectiveUnits; k++)
			{
				if (lvEffectiveUnits[k] == irSelectedUnits[i])
					break;
			}
			if (k == lnEffectiveUnits)
			{
				lvEffectiveUnits[lnEffectiveUnits] = irSelectedUnits[i];
				lvEffectiveElements[lnEffectiveUnits] = irSelectedElements[i];
				lnEffectiveUnits++;
			}
		}
	}
    
    
	DSSXTabBitVector lFilteredMaskInfo;
	if(lnEffectiveUnits > 0)
	{
		lIndex->CalculateMask(lnEffectiveUnits, &lvEffectiveUnits[0], &lvEffectiveElements[0], lFilteredMaskInfo);
	}
	else
	{
		lFilteredMaskInfo.Init(lnRow, false);
		lFilteredMaskInfo.BulkSet(0, lnRow - 1, true);
	}
	
	// check if contain Consolidation Unit
	hr = CalculateMask4Consolidation();
	CHECK_HR;
    
	DSSXTabBitVector lConsoMaskInfo;
	// Int32 lnConsolidation = 0;
	vector<DSSTabularConsolidation*> lvConsolidation;
	vector<Int32> lvConsolidationIndex;
	for (Int32 i = 0; i < lnUnits; ++i)
	{
		DSSTabularUnit* lpUnit = NULL;
		lpTabularData->Item(lvUnits[i], &lpUnit);
		CHECK_HR_PTR(lpUnit);
        
		EnumDSSTemplateUnitType lUnitType;
		lUnitType = lpUnit->getUnitType();
		if (DssTemplateConsolidation == lUnitType)
		{
			if (mpvConsolidationBVs[lvUnits[i]] != NULL)
				lFilteredMaskInfo.CalculateAnd((mpvConsolidationBVs[lvUnits[i]]));
		}
	}
    
	// map selected rows to selected element of each contained units
	int lnContainedTargetUnits = lvContainedTargetUnitPos.size();
	vector<DSSXTabBitVector> lvResultBVs(lnContainedTargetUnits);
    
	for (int i = 0; i < lnContainedTargetUnits; i++)
	{
		DSSTabularUnit* lpUnit = NULL;
		hr = lpTabularData->Item(irTargetUnits[lvContainedTargetUnitPos[i]], &lpUnit);
		CHECK_PTR(lpUnit);
		
		int lnElementCount = 0;
		EnumDSSTemplateUnitType lUnitType;
		lUnitType = lpUnit->getUnitType();
		if(DssTemplateConsolidation == lUnitType)
		{
			// need to optimize this part
			int nRow = 0;
            
			nRow = static_cast<DSSTabularConsolidation*>(lpUnit)->getNumAllElements();
            
			for (int iRow = 0; iRow < nRow; iRow++)
			{
				int lTemp = static_cast<DSSTabularConsolidation*>(lpUnit)->TranslateLocalToGlobal(iRow);
				if (lTemp > lnElementCount)
					lnElementCount = lTemp;
			}
			lnElementCount += 1;
		}
		else
        {
            //hyan, 1/31/2013, follow server side's logic, add new API to get element count to avoid crash
            DSSXTabRefLookupTable *lpXTabRefLookUpTable=dynamic_cast<DSSXTabRefLookupTable*>(lpUnit);
            
            if(lpXTabRefLookUpTable)
                lnElementCount=lpXTabRefLookUpTable->getElementCount();
            else
                lnElementCount = lpUnit->CountRows();
        }
        
		// rzhu, 01/05/2011, reserve position for Join NULL element
		lvResultBVs[i].Init(lnElementCount + 1, false);
	}
    
	// const DSSXTabKey* pKey = NULL;
	// bool lDisplayable = true;
	
	if (lFilteredMaskInfo.IsAllTrue())
	{
		const DSSXTabKey* lpKeys = lIndex->getKeyTable();
        
        if (lpKeys == NULL)
        {
            if (lnUnits == 0)
            {
                return S_OK;
            }
            assert(false);
            return S_FALSE;
        }
		else // normal / sr report without consolidation / DE
        {
            for (Int32 i = 0; i < lnContainedTargetUnits; i++)
			{
				Int32 lnElements = 0;
				Int32 *lpElements = NULL;
				hr = lIndex->GetElementsForUnit(irTargetUnits[lvContainedTargetUnitPos[i]], &lnElements, &lpElements);
				CHECK_HR;
                
				for (Int32 j = 0; j < lnElements; j++)
				{
					lvResultBVs[i].Set(lpElements[j], true);
				}
			}
        }
        
	}
	else
	{
		int *lpKey = new int [sizeof(DSSXTabKey) * mnTotalUnits];
        MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lKeyStrongPtr(lpKey);
        
		lFilteredMaskInfo.SetSeekPosition(0);
		int lRow = lFilteredMaskInfo.GetNextSetPosition();
		int lnIndexRow = 0;
		lnIndexRow = lIndex->Count();
        
        // 5/8/2013 tehe: for performance consider, valid item first. then call ItemEx instead of Item
        hr = lIndex->ValidateItemEx(mnTotalUnits);
        CHECK_HR;
        
		while (lRow > -1 && lRow < lnIndexRow)
		{
			lIndex->ItemEx(lRow, mnTotalUnits, lpKey);
            
            Int32 lKey = -1;
			for (Int32 j = 0; j < lnContainedTargetUnits; j++)
			{
				lKey = lpKey[lvTargetUnitPosInIndex[j]];
				if (!lvResultBVs[j].Get(lKey))
					lvResultBVs[j].Set(lKey, true);
			}
			lRow = lFilteredMaskInfo.GetNextSetPosition();
		}
	}
    
    // rzhu, 03/22/2012, 532630, for target unit is in selected units list case
	// For example, Region Selector1 controls Grid1, Region Selector2 controls Region Selector1 and Grid1
	// if Region Selector2 selector Total, should Region Selector1 returns Total only
    
    // 10/22/2012 tehe: don't add total value when all data is filtered
    vector<bool> lvEmpty(lnContainedTargetUnits,false);
    vector<bool> lvHasTotal(lnContainedTargetUnits,true);
    
	// collect selected elements
	for (Int32 i = 0; i < lnContainedTargetUnits; i++)
	{
        if(lvResultBVs[i].IsEmpty() || lvResultBVs[i].GetLength() == 0 || !lvResultBVs[i].IsAnyTrue(0, lvResultBVs[i].GetLength()-1))
            lvEmpty[i] = true;
        
		Int32 lSelectedIndex = -1;
		for (Int32 j = 0; j < irSelectedUnits.size(); j++)
		{
			if (irTargetUnits[lvContainedTargetUnitPos[i]] == irSelectedUnits[j])
			{
				lSelectedIndex = j;
				break;
			}
		}
		if (lSelectedIndex > -1)
		{
			// the available elements should have no more than the passed in selected elements
            lvHasTotal[i] = false;
			DSSXTabBitVector lSelectElementBV(lvResultBVs[i].GetLength());
			bool lfAllContained = false;
            
			// TQMS, 747592, tehe: gb elements get from parent may be empty
			if(lSelectedIndex < irSelectedElements.size())
			{
				for (Int32 j = 0; j < irSelectedElements[lSelectedIndex].size(); j++)
				{
					if (irSelectedElements[lSelectedIndex][j] > -1)
						lSelectElementBV.Set(irSelectedElements[lSelectedIndex][j], true);
					else if (irSelectedElements[lSelectedIndex][j] == -1) // all elements are contained
					{
						lfAllContained = true;
						break;
					}
					else if(irSelectedElements[lSelectedIndex][j] == -3 || irSelectedElements[lSelectedIndex][j] == DSSXTAB_TOTAL_KEY)
					{
						lvHasTotal[i] = true;
					}
				}
			}
            
			if (!lfAllContained)
				lvResultBVs[i].CalculateAnd(&lSelectElementBV);
		}
		lvResultBVs[i].SetSeekPosition(0);
		Int32 lElement = lvResultBVs[i].GetNextSetPosition();
		Int32 lnRow = lvResultBVs[i].GetLength();
		while (lElement > -1 && lElement < lnRow)
		{
			opvElements[lvContainedTargetUnitPos[i]].push_back(lElement);
			lElement = lvResultBVs[i].GetNextSetPosition();
		}
	}
    
	// check if we should add subtotal element   mppvMapGBPos2LevelSlice
	vector<int> lLevelUnits;
	vector<bool> lLevelFlagUnits;
	for (int i = 0; i < lnSelectedUnits; i++)
	{
		lLevelUnits.push_back(irSelectedUnits[i]);
		lLevelFlagUnits.push_back(true);
	}
    
	for (int i = 0; i < lnContainedTargetUnits; i++)
	{
        // tehe: unit has value that is not sliced
        if(lvEmpty[i])
            continue;
        else if(lvHasTotal[i])
        {
            // rzhu: when no subtotal slice is available, don't add total
            // opvElements[lvContainedTargetUnitPos[i]].push_back(DSSXTAB_TOTAL_KEY);
            vector<int> lTempLevelUnits = lLevelUnits;
            vector<bool> lTempLevelFlagUnits = lLevelFlagUnits;
            int lnLevelUnits = lTempLevelUnits.size();
            int j= 0;
            for (j=0; j<lnLevelUnits; j++) {
                if(lTempLevelUnits[j] == irTargetUnits[lvContainedTargetUnitPos[i]])
                    break;
            }
            if(j<lnLevelUnits)
            {
                lTempLevelUnits.push_back(irTargetUnits[lvContainedTargetUnitPos[i]]);
                lTempLevelFlagUnits.push_back(false);
            }
            bool lFind = false;
            hr = CheckSlice(lTempLevelUnits, lTempLevelFlagUnits, &lFind);
            if (lFind)
                opvElements[lvContainedTargetUnitPos[i]].push_back(DSSXTAB_TOTAL_KEY);
        }
	}
	return hr;
}

int DSSXTabView::CheckSlice(vector<GUID>& irUnits, vector<bool>& irSelected, bool* isAvailable)
{
	int hr = S_OK;
	vector<int> lvUnits;
	hr = ConvertGUIDToLocalUnitID(irUnits, lvUnits);
	CHECK_HR;
    
	hr = CheckSlice(lvUnits, irSelected, isAvailable);
	CHECK_HR;
    
	return S_OK;
}

int DSSXTabView::CheckSlice(vector<int>& irUnits, vector<bool>& irSelected, bool* isAvailable)
{
	int lnSelectedUnits = irUnits.size();
	int lTrueGroup = 0;
	int lFalseGroup = 0;
	bool match = false;
	bool matchTrue = false;
	for (int j = mnGroupByUnits-1; j >=0; j--)
	{
		match = false;
		for (int k = 0; k < lnSelectedUnits; k++)
		{
			if(irUnits[k] == mpGroupByUnits[j])
			{
				match = true;
				matchTrue = irSelected[k];
				break;
			}
		}
		lTrueGroup = (lTrueGroup<<1) + ((match&& matchTrue)? 1: 0);
		lFalseGroup = (lFalseGroup<<1) + ((match&&(!matchTrue))? 1: 0);
	}
    
	int lnGBGroups = 1<<mnGroupByUnits;
    
	for (int i = 0; i < lnGBGroups; i++)
	{
        // 3/28/2012 tehe #572592: mppvMapGBPos2LevelSlice may be null
		if(!mppvMapGBPos2LevelSlice || mppvMapGBPos2LevelSlice[i] == NULL || mppvMapGBPos2LevelSlice[i]->size() == 0)
		{
			continue;
		}
        
		// (lTrueGroup^i)&lTrueGroup == 0, means the GB units in the slices covers all selected units
		// i&lFalseGroup == 0 means the false unit is not included
		if (((lTrueGroup^i)&lTrueGroup) == 0 && (i&lFalseGroup)== 0)
		{
			*isAvailable = true;
			return S_OK;
		}
	}
	*isAvailable = false;
	return S_OK;
    
}


int DSSXTabView::GetCube(DSSCube **oppCube)
{
    *oppCube = mpCube ;
    return S_OK;
}

int DSSXTabView::get_Cube(DSSCube **oppCube)
{
    *oppCube = mpCube ;
    return S_OK;
}

int DSSXTabView::GetCurrentWindow(vector<int> &orRows, vector<int> &orColumns)
{
    
	int hr = S_OK;
    
    
	// get row range
	hr = mHeaders[0]->GetCurrentGBAxisRange(orRows);
	if (FAILED(hr)) return hr;
    
	if (orRows.size() == 0)
	{
		orColumns.clear();
		return S_OK;
	}
	else if (mHeaders[0]->mnMetrics == 0 && (mHeaders[0]->mnUnits - mHeaders[0]->mbHasMetric == 0))
	{
		//if there is no unit (metric or attribute) on row and there is no metric at all
		//we need to set mRowCount to 0
		orRows.clear();
	}
    
    if(mvRowSorts == NULL)
    {
        hr = RefreshSortDefinition();
        CHECK_HR;
    }
    
    for (int i=mvRowSorts->size()-1; i>=0; --i)
    {
        hr = (*mvRowSorts)[i]->getSorter()->sort(getHeader(0), orRows);
        CHECK_HR;
    }
    //DSSNewRWDSorter lNewRWDRowSorter(getHeader(0), mvRowSorts);
    //lNewRWDRowSorter.sort(NULL, orRows);
    
	// get column range
	hr = mHeaders[1]->GetCurrentGBAxisRange(orColumns);
	if (FAILED(hr)) return hr;
    
	if (orColumns.size() == 0)
	{
		orRows.clear();
		return S_OK;
	}
	else if (mHeaders[1]->mnMetrics == 0 && (mHeaders[1]->mnUnits - mHeaders[1]->mbHasMetric == 0))
	{
		//if there is no unit (metric or attribute) on column and there is no metric at all
		//we need to set mColCount to 0
		orColumns.clear();
	}
    
    
    for (int i=mvColumnSorts->size()-1; i>=0; --i)
    {
        hr = (*mvColumnSorts)[i]->getSorter()->sort(getHeader(1), orColumns);
        CHECK_HR;
    }
    //DSSNewRWDSorter lNewRWDColSorter(getHeader(1), mvColumnSorts);
    //lNewRWDColSorter.sort(NULL, orColumns);
    
    //zhyang, 810346
    //if there is no metric in grid and there is no unit in row, set orRows to empty, same thing for column.
    if(!mHeaders[0]->HasMetric() && !mHeaders[1]->HasMetric())
    {
        if(mHeaders[0]->mnUnits - mHeaders[0]->mnRuntimeUnits == 0)
        {
            orRows.clear();
        }
        if(mHeaders[1]->mnUnits - mHeaders[1]->mnRuntimeUnits == 0)
        {
            orColumns.clear();
        }
    }
    
	return hr;
}

int DSSXTabView::sortForMicroChart(vector<int> &orRows, GUID &iMetricGUID, bool ibAsc, bool isTreeMode, int iSubtotalID)
{
    int hr = S_OK;
    
    vector<DSSSortDefinition*>* lpvMicroChartSorts = NULL;
    DSSViewDataSet *lpViewDataSet = this->GetViewDataSet();
    CHECK_PTR(lpViewDataSet);
    
    
    DSSTemplate *lpTemplate;
    hr = lpViewDataSet->getTemplate(&lpTemplate);
    CHECK_HR;
    
    DSSSortConverter converter(lpViewDataSet, this);
	vector<int>& rowUnits = getHeader(0)->getUnits();
    
    lpvMicroChartSorts = converter.convertForMicroChart(lpTemplate->getRow(), rowUnits, iMetricGUID, ibAsc, isTreeMode, iSubtotalID);
    
    assert(lpvMicroChartSorts);
    for (int i=lpvMicroChartSorts->size()-1; i>=0; --i)
    {
        hr = (*lpvMicroChartSorts)[i]->getSorter()->sort(getHeader(0), orRows);
        CHECK_HR;
    }
    
    for(int i = 0; i < lpvMicroChartSorts->size(); ++i)
    {
        delete (*lpvMicroChartSorts)[i];
    }
    
    delete lpvMicroChartSorts;
    
    return hr;
}


int DSSXTabView::hGenerateSliceLevelInfo()
{
	int hr = S_OK;
    DSSTabularIndexPool *pIndicesPool = NULL;
    hr = mpCube -> getIndexPool(&pIndicesPool);
	CHECK_HR;
    
	//get header units
	int lnHeaderAxes = AXIS_COUNT;
	vector<vector<int> > lvHeaderUnits;
	lvHeaderUnits.resize(lnHeaderAxes);
	int i = 0;
	for(i = 0; i < lnHeaderAxes; i++)
	{
		if (mHeaders[i])
		{
			int lnUnits = 0;
			lnUnits = mHeaders[i]->getNumUnits();
			CHECK_HR;
            
			if (lnUnits > 0)
			{
				lvHeaderUnits[i].resize(lnUnits);
				hr = mHeaders[i]->getUnits(lnUnits, &lvHeaderUnits[i][0]);
				CHECK_HR;
			}
		}
	}
    
	int lMetricsOnRow = mHeaders[0]->mbHasMetric ? 1 : 0;
    
    
	//step1, build map from {row_index, col_index, pageby_index} to slice id
	map<vector<DSSTabularIndex*>, int> lMapIndices2Slice;
	vector<DSSTabularIndex*> lvAxisIndices;
	lvAxisIndices.resize(lnHeaderAxes);
    
	int lnOrgUnit = 0;
	vector<int> lvOrgUnit;
	lvOrgUnit.resize(mnTotalUnits + 1);	//reserved for dummy unit
    
	for(i = 0; i < mnLevels; i++)
	{
		bool *AttrMap = NULL;
		hr = mpSliceMap->GetLevel(i, &AttrMap);
		CHECK_HR;
        
		int j = 0;
		for (j = 0; j < lnHeaderAxes; j++)
		{
			lnOrgUnit = 0;
			vector<int>::iterator it = lvHeaderUnits[j].begin();
			for (; it != lvHeaderUnits[j].end(); it++)
			{
				if(AttrMap[*it])
					lvOrgUnit[lnOrgUnit++] = *it;
			}
            
			if (mDummyUnitIndex != -1 && j == 0 && lnOrgUnit + lMetricsOnRow == lvHeaderUnits[j].size())
			{
				lvOrgUnit[lnOrgUnit++] = mDummyUnitIndex;
				sort(&lvOrgUnit[0], &lvOrgUnit[lnOrgUnit - 1]);
			}
            
			hr = pIndicesPool -> getIndex(lnOrgUnit, &lvOrgUnit[0], &lvAxisIndices[j]);
			CHECK_HR;
		}
		lMapIndices2Slice[lvAxisIndices] = i;
        
	}
    
	//step2, build the lookup table of metric slices, given the row and column level ID
	int lRowCount = mHeaders[0] -> getLevelCount();
	int lColCount = mHeaders[1] -> getLevelCount();
    
    
	mnHeaderLevels[0] = lRowCount;
	mnHeaderLevels[1] = lColCount;
	int lTotalSlices = lRowCount * lColCount;
    
	mSizeSliceLevel = lTotalSlices;
	mpSliceLevel = new DSSSliceLevel [lTotalSlices];
	CHECK_NEWED_PTR(mpSliceLevel);
    
    //    // 5/24/2012 tehe: we use mvpslicemap to cache which slice is loaded
    //    mvpSliceMap.resize(mSizeSliceLevel);
    //    fill(mvpSliceMap.begin(), mvpSliceMap.end(), -1);
    
	for (i = 0; i < lTotalSlices; i++)
	{
		mpSliceLevel[i].mpCellIndices = mpSliceLevel[i].mpRowIndices = mpSliceLevel[i].mpColIndices = NULL;
	}
    
    //	lvAxisIndices[2] = NULL;
    //	if (mHeaders[2])
    //	{
    //		int lPageByLevel = -1;
    //		hr = mHeaders[2]->get_RowSubtotalLevel(mPageOrdinal >= 0 ? mPageOrdinal : 0, lPageByLevel, &lvAxisIndices[2]);
    //		CHECK_HR;
    //	}
    
	DSSTabularIndex* lpCellIndices = NULL;
	for (i = 0; i < lRowCount; i++)
	{
		lvAxisIndices[0] = NULL;
		hr = mHeaders[0]->getIndexByLevel(i, &lvAxisIndices[0]);
		CHECK_HR;
        
		int j = 0;
		for (j = 0; j < lColCount; j++)
		{
			lvAxisIndices[1] = NULL;
			hr = mHeaders[1]->getIndexByLevel(j, &lvAxisIndices[1]);
			CHECK_HR;
            
			map<vector<DSSTabularIndex*>, int>::iterator it = lMapIndices2Slice.find(lvAxisIndices);
			if (it != lMapIndices2Slice.end())
			{
				DSSSliceLevel *lpSliceLevelLookup = &mpSliceLevel[i * lColCount + j];
				bool *lpbRowUnit = NULL;
				hr = mpSliceMap->GetLevel(it->second, &lpbRowUnit);
				CHECK_HR;
                
				lnOrgUnit = 0;
				int k = 0;
				for(k = 0; k < mnTotalUnits; k++)
				{
					if(lpbRowUnit[k])
						lvOrgUnit[lnOrgUnit++] = k;
				}
                
				hr = pIndicesPool -> getIndex(lnOrgUnit, &lvOrgUnit[0], &lpCellIndices);
				CHECK_HR;
                
				lpSliceLevelLookup->mpCellIndices = lpCellIndices;
				lpSliceLevelLookup->mpRowIndices = lvAxisIndices[0];
				lpSliceLevelLookup->mpColIndices = lvAxisIndices[1];
			}
			//else we don't the data slice at this levle {lpRowIndices+lpColIndices}
		}
	}
	return S_OK;
}

int DSSXTabView::Set_DoPreXTab(int iDoPreXTab)
{
	mDoPreXTab = (bool)iDoPreXTab;
	return S_OK;
}

//	This interface function provides the access to the data.
//	Given an coordinate, a value will be returned.
int DSSXTabView::GetCell (int iRow, int iCol, EnumDssSliceFlag iSliceFlag, double & orVal, DSSDataFlag & pFlags, int* opThresholdID)
{
    if(mfStatus != XTAB_INITIALIZED)
		return E_FAIL;
    
	int hr;
    
	pFlags = DssDataUnknown;
    
    DSSXTabKey * pIndex = new DSSXTabKey[sizeof(DSSXTabKey) * mnTotalUnits];
    MBase::DSSStrongPtr<DSSXTabKey, MBase::DeleteArray<DSSXTabKey> > pIndexStrongPtr(pIndex);
    
    
    DSSXTabKey* pChildIndex = new DSSXTabKey[mnTotalUnits];
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lStrongPtr2(pChildIndex);
    for (int i=0; i<mnTotalUnits; i++) {
        pChildIndex[i] = DSSXTAB_TOTAL_KEY;
    }
    
	int lMetric = 0,  lTag = 0 ;
    
	DSSTabularIndex* lpIndexObject = NULL;
	int lOffsetInIndexObject = -1;
	hr = FindIndexFromCoordinate(iRow, iCol, pIndex, &lMetric, &lTag, &lpIndexObject, &lOffsetInIndexObject, pChildIndex);
	if(hr != S_OK && hr != S_FALSE) return hr;
    
	if(hr == S_FALSE)
	{
		pFlags = DssDataInvalidCell;
		return S_OK;		//we just did not find the data
	}
    
	//check if it is an xtab cell, remove it if necessary
	int * lpUnit = new int [sizeof(int) * mnTotalUnits];
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lpUnitStrongPtr(lpUnit);
    
	int lnUnit = 0;
	for(int i = 0; i < mnTotalUnits; i ++)
        if(pIndex[i] != DSSXTAB_TOTAL_KEY)
            lpUnit[lnUnit ++] = i;
    
	DSSTabularIndex *lpIndices;
    DSSTabularIndexPool *lpIndexPool = NULL;
    hr = mpCube->getIndexPool(&lpIndexPool);
    CHECK_PTR(lpIndexPool);
    
	hr = lpIndexPool -> getIndex(lnUnit, lpUnit, &lpIndices);
	CHECK_PTR(lpIndices);
    
	int lOffset = 0;
    
	if (lpIndexObject == lpIndices)
	{
		lOffset = lOffsetInIndexObject;
	}
	else
	{
        hr = lpIndices -> LookUp(mnTotalUnits, pIndex, &lOffset);
        if(hr != S_OK && hr != S_FALSE)
            return E_FAIL;
        
        // weiwang 2009-09-10 TQMS358035 do not display unqualified data
        if(hr == S_FALSE || lpIndices->IsUnqualified(lOffset))
        {
            //look up failed
            pFlags = DssDataInvalidCell;
            return S_OK;
        }
	}
    
	if(mnMetrics == 0)
	{
		pFlags = DssDataNull;
		return S_OK;	//no further processing
	}
    
	//	XW: now do not do assert	_ASSERTE(lMetric < mnMetric && lMetric >= 0);
	if(lMetric >= mnMetrics || lMetric < 0)
	{
		pFlags = DssDataInvalidCell;
		return S_OK;
	}
	//now we have lMetric, pIndex and lTag
	DSSCubeDataSlice * lSlice = NULL;
	hr = FindDataLocFromIndex(pIndex, lMetric, lTag, iSliceFlag, &lSlice, pFlags);
	CHECK_HR;
    
	if(!lSlice)
		return S_OK;
    
	//may be S_FALSE
	DSSTabularIndex *lpSliceIndex;
	lpSliceIndex = lSlice->getIndices();
	CHECK_PTR(lpSliceIndex);
    
	// If the slice has the same index as the index we just checked, we will use the offset directly
	if (lpSliceIndex && lpSliceIndex->IsSameOrder(lpIndices))
	{
		hr = lSlice->getNumericByOffset(lOffset, orVal, pFlags, NULL);
	}
	else
	{
		hr = lSlice -> getNumeric(mnTotalUnits, pIndex, orVal, pFlags, NULL);
	}
    
    
	//if S_FALSE, then we will have InvalidCell
    //tliao, 1/10/2014, 831126, for DateTime type, we will return E_UNEXPECTED_DATATYPE
	if(hr != S_OK && hr != S_FALSE && hr != E_NaN && hr != E_UNEXPECTED_DATATYPE) return hr;
    
    int lDataHr = S_OK;
    //zhyang, 819769
    if(hr == E_NaN || hr == E_UNEXPECTED_DATATYPE)
    {
        // 12/4/2012 tehe #688700: the data type of the cell is not a valid number string, which means it contains value other than [+,-,0~9],like "you gain598712.24million dollars", however we don't want it return hr directly, because view may call this method to get the value of threshold id.
        lDataHr = hr;
    }
    
	if(hr == S_FALSE)
	{
		pFlags = DssDataInvalidCell;
		return S_OK;		//not on the index
	}
    
    
    //zhyang, for threshold
    unsigned char * lpData = 0;
	int lWritten = 0;
	EnumDSSDataType lType = DssDataTypeUnknown;
	iSliceFlag = DssDisplayIndexSlice;
    DSSDataFlag lFlags;
	//lMetric = 0;
	//lTag = 0 ;
    
	if(opThresholdID)
	{
		*opThresholdID = 0;
		DSSCubeDataSlice * lSlice = NULL;
        hr = FindDataLocFromIndex(pIndex, lMetric, lTag, iSliceFlag, &lSlice, lFlags);
        CHECK_HR;
        
		if(!lSlice)
		{
			*opThresholdID = 0;	//no data
			//return S_OK;
            return lDataHr;
		}
        
		DSSTabularIndex *lpSliceIndex;
		lpSliceIndex = lSlice->getIndices();
        
		// If the slice has the same index as the index we just checked, we will use the offset directly
        //zhyang, just omit the Threshold
        int lThreshhold;
        //for 744856,  lpSliceIndex may be null for calculated threshold slice
		if (lpIndexObject && ((!lpSliceIndex) || lpSliceIndex->IsSameOrder(lpIndexObject)))
		{
			hr = lSlice->getValByOffset(lOffsetInIndexObject, (const char **)&lpData, lWritten, lType, lFlags, &lThreshhold);
		}
		else
		{
			//may be S_FALSE
			hr = lSlice -> getVal(mnTotalUnits, pIndex, (const char**)&lpData, lWritten, lType, lFlags, &lThreshhold);
		}
        
		//if S_FALSE, then we will have InvalidCell
		if(hr != S_OK && hr != S_FALSE) return hr;
        
		if(hr == S_FALSE)
		{
			lFlags = DssDataInvalidCell;
			*opThresholdID = 0;
			//return S_OK;		//not on the index//
            return lDataHr;
		}
        
		if(hr == S_OK && lFlags == DssDataOk)
		{
			if(lType == DssDataTypeLong)
			{
				* opThresholdID = *lpData;
			}
			else if(lType == DssDataTypeInteger)
			{
				* opThresholdID = (int)(*((int *)lpData));
			}
			else if(lType == DssDataTypeShort)
			{
				* opThresholdID = (int)(*((short *)lpData));
			}
			else if(lType == DssDataTypeDouble)
			{
				* opThresholdID = (int) (*((double *)lpData));
			}else
				return E_FAIL;
		}
		if(hr != S_OK)
			return hr;
	}
    
    return lDataHr;
	//return S_OK;
}

//	This private function will find the slice  that contains
//	the data indexed by index.
int DSSXTabView::FindDataLocFromIndex (DSSXTabKey *pIndex, int iMetric, int iTag, EnumDssSliceFlag iSliceFlag, DSSCubeDataSlice **ppSlice, DSSDataFlag & pFlags)
{
   	bool * pIndexLevel = new bool[mnTotalUnits];
    MBase::DSSStrongPtr<bool, MBase::DeleteArray<bool> > pIndexLevelStrongPtr(pIndexLevel);
    
	//first have to decide which level the mIndex mapps to
	for(int i = 0; i < mnTotalUnits; i++)
		if(pIndex[i] != DSSXTAB_TOTAL_KEY)
			pIndexLevel[i] = true;
		else
			pIndexLevel[i] = false;
    
	int SliceID = 0;
	int hr = mpSliceMap->SearchID(pIndexLevel, iTag, iSliceFlag, iMetric, &SliceID);
	// need to convert SliceID into slice pointer
    
	//lwang, TQMS 331360; set proper data flag if data is invalid;
	//set aggregation Null if it is a subtoal; otherwise, it should be a warehouse null
	if(SliceID != -1 && hr != S_FALSE)
	{
		*ppSlice =  mpCube -> getSliceByID(SliceID);
		if(hr != S_OK && hr != S_FALSE) return hr;
        
		if (hr == S_OK && *ppSlice)
			return S_OK;
	}
	if (iTag != 0)
		pFlags = DssDataUnCalculated;
	else  // qxue: suggest to check if SliceID==-1, and if so, set the flag is DssDataInvalid
		pFlags = DssDataNull;
    
	return S_OK;
    
}

int DSSXTabView::FindIndexFromCoordinate (int iRow, int iCol, DSSXTabKey *pIndex, int *pMetric, int *pTag, DSSTabularIndex **oppIndex, int *opOffset, DSSXTabKey* pChildIndex)
{
   	int lRowTag = 0,  lColTag = 0 ;
	int hr;
    
    hr = mHeaders[0] -> getSubtotalID(iRow, lRowTag);
	if(hr != S_OK) return hr;
    
	/*
     The following condition results in empty cell
     mCurTag = 0:
     lRowTag = Others && lColTag = Others && lRowTag <> lColTag
     
     mCurTag = Others:
     lRowTag = Others && lColTag = 0 && mCurTag <> lRowTag
     lColTag = Others && lRowTag = 0 && mCurTag <> lColTag
     lColTag = Others && lRowTag = Others && Any of (mCurTag, lRowTag, lColTag) is different
     
     
     Thus, basically, all three tag need to be the same unless it is 0.
     */
    //	if(lRowTag != mCurTag && mCurTag != 0 && lRowTag != 0)	//tag not match and it is not the special tag
    //		return S_FALSE;	//that is fine
    
    
    hr = mHeaders[1] -> getSubtotalID(iCol, lColTag);
	
	if(hr != S_OK) return hr;
    
    //	if(lRowTag != lColTag && lRowTag != 0 && lColTag != 0)
    //		return S_FALSE;	//that is fine, this intersection can not have data
    
	//temporary placement
	int lTag1 = 0,  lTag2 = 0 ;
	lTag1 = (lRowTag) ? lRowTag : mCurTag;
	lTag2 = (lColTag) ? lColTag : mCurTag;
    
	if(mCurTag)
	{
		if((mCurTag != lTag1) || (mCurTag != lTag2))
			return S_FALSE;
        
		*pTag = mCurTag;
	}
	else
	{
		if(lRowTag != lColTag && lColTag != 0 && lRowTag != 0)
			return S_FALSE;
        
		if(lRowTag)
			*pTag = lRowTag;
		else
			*pTag = lColTag;
	}
    
	//now we are going to actually complete the index and lookup the
	//data
	bool lHasPageBy = false;
	int i = 0;
	for(i = 0; i < mnTotalUnits; i++)
	{
		pIndex[i] = mCurIndex[i];	//current page
		if (pIndex[i] >= 0)
			lHasPageBy = true;
	}
    
    
	*pMetric = mCurMetric;
    
	DSSTabularIndex *lpIndex1 = NULL;
	DSSTabularIndex *lpIndex2 = NULL;
	int lOffset1 = 0;
	int lOffset2 = 0;
    
	if (oppIndex)
	{
		*oppIndex = NULL;
		*opOffset = 0;
	}
    
    /*
     if(ibUseOriginalRow)
     {
     hr = mHeaders[0] -> FillIndexExByOriginalRow(iRow, mnTotalUnits, pIndex, pMetric, &lpIndex1, &lOffset1);
     }
     else
     {
     hr = mHeaders[0] -> FillIndexEx(iRow, mnTotalUnits, pIndex, pMetric, &lpIndex1, &lOffset1);
     }*/
	
    hr = mHeaders[0] -> FillIndexEx(iRow, mnTotalUnits, pIndex, pMetric, &lpIndex1, &lOffset1, pChildIndex);
	if(hr != S_OK) return hr;
    
    hr = mHeaders[1] -> FillIndexEx(iCol, mnTotalUnits, pIndex, pMetric, &lpIndex2, &lOffset2, pChildIndex);
	if(hr != S_OK) return hr;
    
	//xhan 01/07/05 it is a subtotal cell, we need to clean the dummy unit index from row if there is any
	if (*pTag != 0 && mDummyUnitIndex != -1)
		pIndex[mDummyUnitIndex]	= DSSXTAB_TOTAL_KEY;
    
	// aye, 6/8/09, if one of the index is empty, we can directly use the other lpIndex object and the row offset
	if (!lHasPageBy && oppIndex)
	{
		int lnUnits = 0;
		lnUnits = lpIndex2->getNumUnits();
		if (lnUnits == 0)
		{
			*oppIndex = lpIndex1;
			*opOffset = lOffset1;
		}
		else
		{
			lnUnits = lpIndex1->getNumUnits();
			if (lnUnits == 0)
			{
				*oppIndex = lpIndex2;
				*opOffset = lOffset2;
			}
		}
	}
    
	return S_OK;
    
}

int DSSXTabView::GetCellValue2(int RowIndex, int ColIndex, double &orVal, DSSDataFlag &orDataFlag,
                               int *opThreshold, int *opMetric)
{
    int lMetricID = mHeaders[1]->getMetricID(ColIndex);
	if (lMetricID < 0)
		lMetricID = mHeaders[0]->getMetricID(RowIndex);
	
	if (opMetric)
	{
		if (lMetricID >= 0 && lMetricID < mnMetrics)
			*opMetric = lMetricID;
		else
			*opMetric = 0;
	}
    
    // tehe: here I set the value of slice flag as the default value DssOriginalSlice, maybe in some special case, it will wrong
    int hr = GetCell(RowIndex, ColIndex, DssOriginalSlice, orVal, orDataFlag, opThreshold);
    CHECK_HR;
    
    return S_OK;
}

int DSSXTabView::GetRawCellInfo2(int RowIndex, int ColIndex, const char **oppData,
                                 int &orDataSize, EnumDSSDataType &orDataType,
                                 DSSDataFlag &orDataFlag, int *opThreshold,
                                 int *opMetric, int *opTotalFlag, bool ibGetNullDataType)
{
    
    double lData = 0;
	int hr = S_OK;
	hr = hGetRawCellInfo2(RowIndex, ColIndex, (char **)oppData, &orDataSize, &orDataType, &orDataFlag, opThreshold, opMetric, opTotalFlag, &lData, ibGetNullDataType);
	
	// qjiang 06/06/2009
	// if ppData points to lData, then we have to allocate it
	if ((double*)(*oppData) == &lData)
	{
		double* pData = new double(lData);
		CHECK_NEWED_PTR(pData);
		*oppData = reinterpret_cast<char*>(pData);
	}
    
	return hr;
}

int DSSXTabView::GetRawCellInfoFromMetricIndex(int RowIndex, int ColIndex, int iMetric, const char **oppData,
                                 int &orDataSize, EnumDSSDataType &orDataType,
                                 DSSDataFlag &orDataFlag, int *opThreshold,
                                int *opTotalFlag, bool ibGetNullDataType)
{
    
    double lData = 0;
	int hr = S_OK;
    bool lbMetricAsInput = true;
	hr = hGetRawCellInfo2(RowIndex, ColIndex, (char **)oppData, &orDataSize, &orDataType, &orDataFlag, opThreshold, &iMetric, opTotalFlag, &lData, ibGetNullDataType, lbMetricAsInput);
	
	// qjiang 06/06/2009
	// if ppData points to lData, then we have to allocate it
	if ((double*)(*oppData) == &lData)
	{
		double* pData = new double(lData);
		CHECK_NEWED_PTR(pData);
		*oppData = reinterpret_cast<char*>(pData);
	}
    
	return hr;
}

int DSSXTabView::hGetRawCellInfo2(int RowIndex, int ColIndex, char **ppData,
                                  int *pcbWritten, EnumDSSDataType *pType,
                                  DSSDataFlag * pCellFlag, int *pThreshold, int * pMetric, int *opTotalFlag,
                                  double* opData,
                                  bool ibGetNullDataType,
                                  bool iMetricAsInput)
{
    int hr = S_OK;
    
	CHECK_PTR(pCellFlag);
	*pCellFlag = DssDataUnknown;
    
	//lwang, 337697, 12/2/2008; init threshold id to 0
	if (pThreshold)
		*pThreshold = 0;
    
	EnumDssSliceFlag iSliceFlag = DssOriginalSlice;
    
    // 3/31/2012 tehe: memory leak
	DSSXTabKey * pIndex = new DSSXTabKey[mnTotalUnits];
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lStrongPtr(pIndex);
    
	int lMetric = 0,  lTag = 0 ;
    
    DSSXTabKey* pChildIndex = new DSSXTabKey[mnTotalUnits];
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lStrongPtr2(pChildIndex);
    for (int i=0; i<mnTotalUnits; i++) {
        pChildIndex[i] = DSSXTAB_TOTAL_KEY;
    }
    
	DSSTabularIndex *lpIndices2 = NULL;
	int lMetric2 = 0,  lTag2 = 0 ;
	//the initial goal is replacing FindIndexFromCoordinate with hFindCellLevelIndex to avoid
	//Fill_Index(pIndex) and speed up index lookup (without copying keys). But it's hard to achieve because:
	//1, FindDataLocFromIndex needs pIndex to find out the slice (can be enhanced)
	//2, GetValueByIndex needs pIndex to lookup (may be optimized in special case that all units are on one axis)
	hr = hFindCellLevelIndex(RowIndex, ColIndex, &lpIndices2, &lMetric2, &lTag2, 0, 0, 0/*, &lOffset2, &lSlice2, &lOffsetMetric2*/);
	if(hr != S_OK && hr != S_FALSE) return hr;
    
	DSSTabularIndex* lpIndexObject = NULL;
	int lOffsetInIndexObject = -1;
    
	hr = FindIndexFromCoordinate(RowIndex, ColIndex, pIndex, &lMetric, &lTag, &lpIndexObject, &lOffsetInIndexObject,pChildIndex);
	if(hr != S_OK && hr != S_FALSE) return hr;
    
	//lwang;03/03/2008;TQMS 302944; need return valid metric index even if data is invalid or NULL
    //******************************************************
	//if pMetric is passed in
	if(pMetric)
	{
        if(iMetricAsInput && (*pMetric) >= 0)
        {
            lMetric = *pMetric;
        }else
        {
            if(lMetric >= mnMetrics || lMetric < 0)
            {
                *pMetric = 0;
            }
            else
                *pMetric = lMetric;
        }
	}
    //******************************************************
    
	if(hr == S_FALSE)
	{
		*pCellFlag = DssDataInvalidCell;
		return S_OK;		//we just did not find the data
	}
    
	if (opTotalFlag)
		*opTotalFlag = lTag;
    
	int lOffset = 0;
    // 3/31/2012 tehe: partial look up for index, since we have push down actual gb units, here I think we may don't need partial lookup now
	if (lpIndices2)
	{
        // tehe : avoid doing lookup if lpIndexObject and lpIndices is same order
        if(lpIndices2->IsSameOrder(lpIndexObject))
        {
            lOffset = lOffsetInIndexObject;
        }
        else
        {
            // tehe: e.g. row = {quarter}, column = {year}, cell = {year,quarter}, we need lookup to find whether {20051, 2006} is valid
            hr = lpIndices2 -> LookUp(mnTotalUnits, pIndex, &lOffset);
            
            if(hr != S_OK && hr != S_FALSE)
                return E_FAIL;
        }
        
        // weiwang 2009-09-10 TQMS358035 do not display unqualified data
        if (hr == S_OK && lpIndices2->IsUnqualified(lOffset))
            hr = S_FALSE;
	}
	else
	{
		// the slice doesn't exist
		if (lTag != 0)
			*pCellFlag = DssDataUnCalculated;
		else  // qxue: suggest to check if SliceID==-1, and if so, set the flag is DssDataInvalid
			*pCellFlag = DssDataNull;
		return S_OK;
	}
    
	if(hr == S_FALSE)
	{
		//look up failed
		*pCellFlag = DssDataInvalidCell;
        
		if (ibGetNullDataType)
		{
			DSSDataFlag lTemp = DssDataUnknown;
			DSSCubeDataSlice * lSlice = NULL;
			hr = FindDataLocFromIndex(pIndex, lMetric, lTag, iSliceFlag, &lSlice, lTemp);
			CHECK_HR;
            
			if(!lSlice)
				return S_OK;
            
			*pType = lSlice->getDataType();
		}
        
		return S_OK;
	}
    
	if(mnMetrics == 0)
	{
		*pCellFlag = DssDataNull;
		return S_OK;	//no further processing
	}
    
	//	XW: now do not do assert	_ASSERTE(lMetric < mnMetric && lMetric >= 0);
	if(lMetric >= mnMetrics || lMetric < 0)
	{
		*pCellFlag = DssDataInvalidCell;
		return S_OK;
	}
	//now we have lMetric, pIndex and lTag
    //*******************************************************
	DSSDataFlag lFlags = DssDataUnknown;
	if(ppData)
	{
		*pcbWritten = 0;
		*pType = DssDataTypeUnknown;
		*ppData = 0;
        
		DSSCubeDataSlice * lSlice = NULL;
        
		hr = FindDataLocFromIndex(pIndex, lMetric, lTag, iSliceFlag, &lSlice, *pCellFlag);
		CHECK_HR;
        
		if(!lSlice)
			return S_OK;
        
		DSSTabularIndex *lpSliceIndex;
		lpSliceIndex = lSlice->getIndices();
		CHECK_HR;
        
		// If the slice has the same index as the index we just checked, we will use the offset directly
        //zhyang, just omit the Threshold
        
		if (lpIndexObject && lpSliceIndex && lpSliceIndex->IsSameOrder(lpIndexObject))
		{
			hr = lSlice->getValByOffset(lOffsetInIndexObject, (const char **)ppData,
                                        *pcbWritten, *pType, lFlags, NULL);
		}
        else if (lpIndices2 && lpSliceIndex && lpSliceIndex->IsSameOrder(lpIndices2))
        {
            hr = lSlice->getValByOffset(lOffset, (const char **)ppData,
                                        *pcbWritten, *pType, lFlags, NULL);
        }
		else
		{
			//may be S_FALSE
            //msun, financial report, the index in slice is based on data units not recursive units, so we should use pChildIndex here.
			hr = lSlice -> getVal(mnTotalUnits, pChildIndex, (const char **)ppData, *pcbWritten, *pType, lFlags, NULL);
		}
        
		//if S_FALSE, then we will have InvalidCell
		if(hr != S_OK && hr != S_FALSE) return hr;
        
		if(hr == S_FALSE)
		{
			*pCellFlag = DssDataInvalidCell;
			return S_OK;		//not on the index//
		}
	}
	//if pCellFlag is passed in
	if(lFlags != DssDataOk &&
       lFlags != DssDataNull &&
       lFlags != DssDataInvalidCell &&
       lFlags != DssDataInvalid &&
       lFlags != DssDataUnCalculated &&
       lFlags != DssDataMissing)
	{
		lFlags = DssDataInvalid;
	}
	*pCellFlag = lFlags;
    //******************************************************
    
    
    //******************************************************
	unsigned char * lpData = 0;
	int lWritten = 0;
	EnumDSSDataType lType = DssDataTypeUnknown;
	iSliceFlag = DssDisplayIndexSlice;
	//lMetric = 0;
	//lTag = 0 ;
    
	if(pThreshold)
	{
		*pThreshold = 0;
		DSSCubeDataSlice * lSlice = NULL;
        hr = FindDataLocFromIndex(pIndex, lMetric, lTag, iSliceFlag, &lSlice, lFlags);
        CHECK_HR;
        
		if(!lSlice)
		{
			*pThreshold = 0;	//no data
			return S_OK;
		}
        
		DSSTabularIndex *lpSliceIndex;
		lpSliceIndex = lSlice->getIndices();
        
		// If the slice has the same index as the index we just checked, we will use the offset directly
        int lThreshhold;
		if (lpIndexObject && lpSliceIndex && lpSliceIndex->IsSameOrder(lpIndexObject))
		{
			hr = lSlice->getValByOffset(lOffsetInIndexObject, (const char **)&lpData, lWritten, lType, lFlags, &lThreshhold);
		}
		else
		{
			//may be S_FALSE
			hr = lSlice -> getVal(mnTotalUnits, pChildIndex, (const char**)&lpData, lWritten, lType, lFlags, &lThreshhold);
		}
        
		//if S_FALSE, then we will have InvalidCell
		if(hr != S_OK && hr != S_FALSE) return hr;
        
		if(hr == S_FALSE)
		{
			lFlags = DssDataInvalidCell;
			*pThreshold = 0;
			return S_OK;		//not on the index//
		}
        
		if(hr == S_OK && lFlags == DssDataOk)
		{
			if(lType == DssDataTypeLong)
			{
				* pThreshold = *lpData;
			}
			else if(lType == DssDataTypeInteger)
			{
				* pThreshold = (int)(*((int *)lpData));
			}
			else if(lType == DssDataTypeShort)
			{
				* pThreshold = (int)(*((short *)lpData));
			}
			else if(lType == DssDataTypeDouble)
			{
				* pThreshold = (int) (*((double *)lpData));
			}else
				return E_FAIL;
		}
		if(hr != S_OK)
			return hr;
	}
    //******************************************************
	return S_OK;
}

int DSSXTabView::hFindCellLevelIndex(Int32 iRow, Int32 iCol, DSSTabularIndex **oppIndex, Int32 *pMetric, Int32 *pTag, Int32 *opOffset, DSSCubeDataSlice **oppSlice, Int32 *opOffsetSlice, Int32 nExcludeUnits, Int32* pExcludeUnits)
{
	int lRowLevel = 0;
    lRowLevel = mHeaders[0]->getLevel(iRow);
    
	int lColLevel = 0;
    lColLevel = mHeaders[1]->getLevel(iCol);
	
	if (oppIndex)
	{
        *oppIndex = NULL;
        DSSTabularIndex* lpIndicesC = NULL;
		if (mpSliceLevel)
		{
			DSSSliceLevel *lpSliceLevelLookup = &mpSliceLevel[lRowLevel * mnHeaderLevels[1] + lColLevel];
			lpIndicesC = lpSliceLevelLookup->mpCellIndices;
		}
		else
			return E_FAIL;
        
        // jzeng 15/06/02 sync with iserver
        if (nExcludeUnits > 0 && pExcludeUnits != NULL)
        {
            int lnUnits = lpIndicesC->getNumUnits();
            vector<int> lvUnits;
            lpIndicesC->getUnits(lvUnits);
            vector<int> lvResultUnits;
            for (int i = 0; i < lnUnits; i++)
            {
                bool lFound = false;
                for (int j = 0; (j < nExcludeUnits) && (!lFound); j++)
                {
                    if (lvUnits[i] == pExcludeUnits[j])
                        lFound = true;
                }
                if (!lFound)
                    lvResultUnits.push_back(lvUnits[i]);
            }
            int hr = mpCube->getIndex(lvResultUnits.size(), &lvResultUnits[0], &lpIndicesC);
            CHECK_HR;
        }
        *oppIndex = lpIndicesC;
	}
	return S_OK;
}

int DSSXTabView::get_MetricValue2(int nKeys,
                                  DSSXTabKey *pIndex,
                                  int iMetric,
                                  int iDummyIndex,
                                  EnumDssSliceFlag iSliceFlag,
                                  int iTag,
                                  const char **oppData,
                                  int *orSize,
                                  EnumDSSDataType *opType,
                                  DSSDataFlag *opFlags)
{
	int hr = S_OK;
    
	hr = hGetMetricValue(pIndex,
						 iMetric,
						 iDummyIndex,
						 iSliceFlag,
						 iTag,
						 oppData,
						 orSize,
						 opType,
						 opFlags
                         );
    
	
	return hr;
}

int DSSXTabView::hGetMetricValue(DSSXTabKey *pIndex,
                                 int iMetric,
                                 int iDummyIndex,
                                 EnumDssSliceFlag iSliceFlag,
                                 int iTag,
                                 const char **ppData,
                                 int *orSize,
                                 EnumDSSDataType *pType,
                                 DSSDataFlag *pFlags
                                 )
{
	int hr = S_OK;
	//now we have lMetric, pIndex and lTag
    
	//XW: 1/14/2005
	//we don't reinterpert the lTag any more, because we need it for subtotal
	//essentially what we have here is only used by RW document
	//thus we want to use tag to differentiate the duplicate rows
    
    
	//if(mDummyUnitIndex != -1)
	//	pIndex[mDummyUnitIndex] = iDummyIndex;
    
	DSSCubeDataSlice * lSlice = NULL;
	hr = FindDataLocFromIndex(pIndex, iMetric, iTag, iSliceFlag, &lSlice, *pFlags);
	CHECK_HR;
    
	if(!lSlice)
		return S_OK;
    
	//may be S_FALSE
    hr = lSlice->getVal(mnTotalUnits, pIndex, ppData, *orSize, *pType, *pFlags);
	//if S_FALSE, then we will have InvalidCell
	if(hr != S_OK && hr != S_FALSE) return hr;
    
	if(hr == S_FALSE && mDummyUnitIndex != -1 && pIndex[mDummyUnitIndex] != 0)
	{
		//XW: 1/14/2005
		//not so fast, maybe we just need to pass the original tag which is 0
        //		pIndex[mDummyUnitIndex] = 0;
        
        //		hr = lSlice -> GetValueByIndex(mnAttrUnit, pIndex, ppData, pcbWritten, pType, pFlags);
        //		if(hr != S_OK && hr != S_FALSE) return hr;
	}
    
	if(hr == S_FALSE)
	{
		*pFlags = DssDataInvalidCell;
		return S_OK;		//not on the index
	}
    
	return S_OK;
    
}

int DSSXTabView::getRawCellInfo2(map<int, int> imElementIDs,
                                 int iNumber,
                                 bool ibRowKey,
                                 vector<int> &ivMask,
                                 int iMetric,
                                 int iDummyIndex,
                                 int iSubtotalID,
                                 const char **oppData,
                                 int *orSize,
                                 EnumDSSDataType *opType,
                                 DSSDataFlag *opFlags,
                                 int &actualUnits)
{
    int hr = S_OK;
    
    iDummyIndex = 0;
    int lRowLevel, lColumnLevel;
    
    DSSXTabKey *lpKeys = new DSSXTabKey[mnTotalUnits];
    CHECK_NEWED_PTR(lpKeys);
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lpKeysPtr(lpKeys);
    
    DSSXTabKey* lpChildKeys = new DSSXTabKey[mnTotalUnits];
    CHECK_NEWED_PTR(lpChildKeys)
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lStrongPtr2(lpChildKeys);
    
    int lTag1 = 0, lTag2 = 0;
    actualUnits = 0;
    
    for(int i = 0; i < mnTotalUnits; ++i)
    {
        lpKeys[i] = DSSXTAB_TOTAL_KEY;
        lpChildKeys[i] = DSSXTAB_TOTAL_KEY;
    }
    
    // tehe: try to avoid looking up on cell indices
    DSSTabularIndex* lpIndexObject = NULL;
    int lIndexObjectOffset = -1;
    
    if(ibRowKey)
    {
        hr = mHeaders[1]->FillIndexEx(iNumber, mnTotalUnits, lpKeys, &iMetric, &lpIndexObject, &lIndexObjectOffset, lpChildKeys);
        CHECK_HR;
        
        vector<int> lvUnits;
        for(int i = 0; i < ivMask.size(); ++i)
        {
            lpKeys[ivMask[i]] = DSSXTAB_TOTAL_KEY;
            if(-1 != mDummyUnitIndex)
            {
                lpKeys[mDummyUnitIndex] = DSSXTAB_TOTAL_KEY;
                iDummyIndex = mDummyUnitIndex;
            }
        }
        
        for(int i = 0; i < mnTotalUnits; ++i)
        {
            if(DSSXTAB_TOTAL_KEY != lpKeys[i])
            {
                lvUnits.push_back(i);
                actualUnits++;
            }
        }
        
        if(-1 != mDummyUnitIndex && lpKeys[mDummyUnitIndex] != DSSXTAB_TOTAL_KEY)
        {
            actualUnits--;
        }
        
        hr = mHeaders[1]->getSubtotalID(iNumber, lTag2);
        CHECK_HR;
        
        if(iSubtotalID != -1)
        {
            lTag2 = iSubtotalID;
        }
        
        lColumnLevel = mHeaders[1]->getLevelByUnits(lvUnits, lTag2);
        //zhyang, column level can be -1 when HS using none, this should be data invalid
        if(lColumnLevel == -1)
        {
            *opFlags = DssDataInvalidCell;
            return S_OK;
        }
        
        hr = mHeaders[0]->fillIndexUsingElements(lpKeys, imElementIDs, lTag1, lpChildKeys);
        CHECK_HR;
        
        lRowLevel = mHeaders[0]->getLevelByElements(imElementIDs);
        if(lRowLevel == -1)
        {
            *opFlags = DssDataInvalidCell;
            return S_OK;
        }
    }
    else
    {
        hr = mHeaders[0]->FillIndexEx(iNumber, mnTotalUnits, lpKeys, &iMetric, &lpIndexObject, &lIndexObjectOffset, lpChildKeys);
        CHECK_HR;
        
        vector<int> lvUnits;
        for(int i = 0; i < ivMask.size(); ++i)
        {
            lpKeys[ivMask[i]] = DSSXTAB_TOTAL_KEY;
            if(-1 != mDummyUnitIndex)
            {
                lpKeys[mDummyUnitIndex] = DSSXTAB_TOTAL_KEY;
                iDummyIndex = mDummyUnitIndex;
            }
        }
        
        
        for(int i = 0; i < mnTotalUnits; ++i)
        {
            if(DSSXTAB_TOTAL_KEY != lpKeys[i])
            {
                lvUnits.push_back(i);
                actualUnits++;
            }
        }
        
        
        if(-1 != mDummyUnitIndex && lpKeys[mDummyUnitIndex] != DSSXTAB_TOTAL_KEY)
        {
            actualUnits--;
        }
        
        
        hr = mHeaders[0]->getSubtotalID(iNumber, lTag2);
        CHECK_HR;
        
        if(iSubtotalID != -1)
        {
            lTag2 = iSubtotalID;
        }
        
        lRowLevel = mHeaders[0]->getLevelByUnits(lvUnits, lTag2);
        if(lRowLevel == -1)
        {
            *opFlags = DssDataInvalidCell;
            return S_OK;
        }
        
        hr = mHeaders[1]->fillIndexUsingElements(lpKeys, imElementIDs, lTag1, lpChildKeys);
        CHECK_HR;
        
        lColumnLevel = mHeaders[1]->getLevelByElements(imElementIDs);
        if(lColumnLevel == -1)
        {
            *opFlags = DssDataInvalidCell;
            return S_OK;
        }
    }
    
    int lTag;
    if(iSubtotalID != -1)
    {
        lTag = iSubtotalID;
    }
    else
    {
        lTag1 = (lTag1) ? lTag1 : mCurTag;
        lTag2 = (lTag2) ? lTag2 : mCurTag;
        
        if(mCurTag)
        {
            if((mCurTag != lTag1) || (mCurTag != lTag2))
                return S_FALSE;
            
            lTag = mCurTag;
        }
        else
        {
            if(lTag1 != lTag2 && lTag2 != 0 && lTag1 != 0)
                return S_FALSE;
            
            if(lTag1)
                lTag = lTag1;
            else
                lTag = lTag2;
        }
    }
    
    DSSTabularIndex *lpIndices2 = NULL;
    
    if (mpSliceLevel)
    {
        DSSSliceLevel *lpSliceLevelLookup = &mpSliceLevel[lRowLevel * mnHeaderLevels[1] + lColumnLevel];
        lpIndices2 = lpSliceLevelLookup->mpCellIndices;
    }
    else
        return E_FAIL;
	
    // 3/31/2012 tehe: avoid do lookup
    if (lpIndices2)
	{
        int lOffset = -1;
        //5/2/2013 fyang: we have to do lookup to check whether the cell is a xtab null
        //TQMS: 734324,751938
        //TODO: we need to figure out a way to opt this
        if(lpIndices2->IsSameOrder(lpIndexObject) &&
           (!ibRowKey && mHeaders[1]->getNumUnits() == 1 && mHeaders[1]->HasMetric()/*751938*/))//only metric in column  so no xtab null
        {
            lOffset = lIndexObjectOffset;
        }
        else
        {
            hr = lpIndices2 -> LookUp(mnTotalUnits, lpKeys, &lOffset);
        }
        
        if(hr != S_OK && hr != S_FALSE)
            return E_FAIL;
        
        // weiwang 2009-09-10 TQMS358035 do not display unqualified data
        if (hr == S_OK && lpIndices2->IsUnqualified(lOffset))
            hr = S_FALSE;
	}
	else
	{
		// the slice doesn't exist
		if (lTag != 0)
			*opFlags = DssDataUnCalculated;
		else  // qxue: suggest to check if SliceID==-1, and if so, set the flag is DssDataInvalid
			*opFlags = DssDataNull;
		return S_OK;
	}
    
	if(hr == S_FALSE)
	{
		//look up failed
		*opFlags = DssDataInvalidCell;
        
		//if (ibGetNullDataType)
		{
            //zhyang, 787072, according to xiaonan, it is a subtotal cell, we need to clean the dummy unit index from row if there is any
            if (lTag != 0 && mDummyUnitIndex != -1)
                lpKeys[mDummyUnitIndex]	= DSSXTAB_TOTAL_KEY;
            
			DSSDataFlag lTemp = DssDataUnknown;
			DSSCubeDataSlice * lSlice = NULL;
			hr = FindDataLocFromIndex(lpKeys, iMetric, lTag, DssOriginalSlice, &lSlice, lTemp);
			CHECK_HR;
            
			if(!lSlice)
				return S_OK;
            
			*opType = lSlice->getDataType();
		}
        
		return S_OK;
	}
    
    //zhyang, 787072, according to xiaonan, it is a subtotal cell, we need to clean the dummy unit index from row if there is any
    if (lTag != 0 && mDummyUnitIndex != -1)
		lpKeys[mDummyUnitIndex]	= DSSXTAB_TOTAL_KEY;
    
    DSSCubeDataSlice * lSlice = NULL;
	hr = FindDataLocFromIndex(lpKeys, iMetric, lTag, DssOriginalSlice, &lSlice, *opFlags);
	CHECK_HR;
    
	if(!lSlice)
		return S_OK;
    
    // 3/12/2013 tehe: avoid do lookup
	DSSTabularIndex *lpSliceIndex = NULL;
    lpSliceIndex = lSlice->getIndices();
    CHECK_PTR(lpSliceIndex);
    
    if(lpSliceIndex->IsSameOrder(lpIndexObject))
    {
        hr = lSlice->getValByOffset(lIndexObjectOffset, oppData,
                                    *orSize, *opType, *opFlags, NULL);
    }
    else
    {
        hr = lSlice->getVal(mnTotalUnits, lpChildKeys, oppData, *orSize, *opType, *opFlags);
    }
    
	if(hr != S_OK && hr != S_FALSE) return hr;
    
    
	if(hr == S_FALSE)
	{
		*opFlags = DssDataInvalidCell;
		return S_OK;		//not on the index
	}
    
    
    //    return get_MetricValue2(mnTotalUnits,
    //                            lpKeys,
    //                            iMetric,
    //                            iDummyIndex,
    //                            DssOriginalSlice,
    //                            lTag,
    //                            oppData,
    //                            orSize,
    //                            opType,
    //                            opFlags);
    return S_OK;
}

int DSSXTabView::RefreshSortDefinition()
{
    hDeleteSortDefinition();
    
    DSSViewDataSet *lpViewDataSet = this->GetViewDataSet();
    CHECK_PTR(lpViewDataSet);
    
    
    DSSTemplate *lpTemplate;
    int hr = lpViewDataSet->getTemplate(&lpTemplate);
    CHECK_HR;
    
    DSSSortConverter converter(lpViewDataSet, this);
	vector<int>& rowUnits = getHeader(0)->getUnits();
	vector<int>& columnUnits = getHeader(1)->getUnits();
    
    mvRowSorts = converter.convert(lpTemplate->getRow(), rowUnits);
    mvColumnSorts=converter.convert(lpTemplate->getColumn(), columnUnits);
    
    return hr;
}

int DSSXTabView::hDeleteSortDefinition()
{
    //tehe 4/16/2012: memory leak
    if(mvRowSorts)
    {
        for(int i=0;i<mvRowSorts->size(); i++)
        {
            delete (*mvRowSorts)[i];
            (*mvRowSorts)[i] = NULL;
        }
        delete mvRowSorts;
        mvRowSorts = NULL;
    }
    
    //tehe 4/16/2012: memory leak
    if(mvColumnSorts)
    {
        for(int i=0;i<mvColumnSorts->size(); i++)
        {
            delete (*mvColumnSorts)[i];
            (*mvColumnSorts)[i] = NULL;
        }
        delete mvColumnSorts;
        mvColumnSorts = NULL;
    }
    
    return S_OK;
}

// 5/21/2012 tehe: get report level index
int DSSXTabView::hGetReportLevelIndex(DSSTabularIndex **opReportLevelIndex)
{
    return mpViewDataSet->GetReportLevelIndex(opReportLevelIndex);
    
}

// 5/24/2012 tehe: if there is no valid gb units, we will load all the slice
int DSSXTabView::LoadAllSliceData()
{
    if(!mbOnDemandLoading)
        return S_OK;
    vector<int> lvMetrics;
    for(int i=0;i<mnMetrics; i++)
    {
        lvMetrics.push_back(i);
    }
    
    return hLoadSliceByMetrics(lvMetrics);
}

int DSSXTabView::hLoadSliceByMetrics(vector<int> &ivMetrics)
{
    
    if(!mbOnDemandLoading)
        return S_OK;
    
    // 6/28/2012 tehe
    vector<int> lvDisableMetrics;
    for(int i=0;i<mvGBMetrics.size();i++)
    {
        vector<int>::iterator it = find(ivMetrics.begin(), ivMetrics.end(), mvGBMetrics[i]);
        if(it == ivMetrics.end())
        {
            lvDisableMetrics.push_back(mvGBMetrics[i]);
        }
    }
    
    int hr = S_OK;
    
    bool *lpLevel = new bool[mnTotalUnits];
    MBase::StrongPtr<bool, MBase::DeleteArray<bool> > pLevel(lpLevel);
    
    // int lSize = 0;
    //vector<vector<int> > lvvSliceID;
    vector<int> lvSliceID;
    
    vector<int> lvDisableSliceID;
    
    for(int i=0; i<mSizeSliceLevel; i++)
    {
        DSSTabularIndex *lpCellIndex = mpSliceLevel[i].mpCellIndices;
        if(!lpCellIndex)
            continue;
        fill(lpLevel, lpLevel+mnTotalUnits, false);
        vector<int> lvUnits;
        lpCellIndex->getUnits(lvUnits);
        for(int j=0;j<lvUnits.size(); j++)
        {
            lpLevel[lvUnits[j]] = true;
        }
        vector<int> lvTmpSliceID;
        int ret = mpSliceMap->getSlicesByLevel(lpLevel, ivMetrics, lvTmpSliceID);
        if(ret == S_OK)
        {
            // lSize ++;
            lvSliceID.insert(lvSliceID.end(),lvTmpSliceID.begin(), lvTmpSliceID.end());
        }
        
        // 6/28/2012 tehe
        lvTmpSliceID.clear();
        ret = mpSliceMap->getSlicesByLevel(lpLevel, lvDisableMetrics, lvTmpSliceID);
        if(ret == S_OK)
        {
            lvDisableSliceID.insert(lvDisableSliceID.end(), lvTmpSliceID.begin(), lvTmpSliceID.end());
        }
        
    }
    
    vector<int> lvExtremeSliceID;
    hr = mpViewDataSet->getCalculationPlan()->getAllExtremeSliceID(lvExtremeSliceID);
    CHECK_HR;
    
    for(int i = 0; i < lvExtremeSliceID.size(); ++i)
        lvSliceID.push_back(lvExtremeSliceID[i]);
    
    // 03/23/16 jzeng attributet threshold
    vector<int> lvAttributeThresholdSliceID;
    hr = mpViewDataSet->getCalculationPlan()->getAllAttriThresholdSliceID(lvAttributeThresholdSliceID);
    CHECK_HR;
    
    for(int i = 0; i < lvAttributeThresholdSliceID.size(); ++i)
    {
        if (lvAttributeThresholdSliceID[i] != -1)
        {
            lvSliceID.push_back(lvAttributeThresholdSliceID[i]);
        }
    }
    
    sort(lvSliceID.begin(), lvSliceID.end());
    sort(lvDisableSliceID.begin(), lvDisableSliceID.end());
    
    vector<int>::iterator it = unique(lvSliceID.begin(), lvSliceID.end());
    lvSliceID.erase(it,lvSliceID.end());
    
    it = unique(lvDisableSliceID.begin(), lvDisableSliceID.end());
    lvDisableSliceID.erase(it,lvDisableSliceID.end());
    
    // 6/28/2012 tehe
    if(lvDisableSliceID.size() > 0)
    {
        hr = mpViewDataSet->RecycleMetricSliceMemoryForNewRWD(false, lvDisableSliceID.size(), (int*)&(lvDisableSliceID[0]));
        CHECK_HR;
    }
    
    hr = mpViewDataSet->LoadSliceDataFromDataSource(lvSliceID.size(), NULL, &(lvSliceID[0]));
    CHECK_HR;
    
    return S_OK;
}

int DSSXTabView::CalculateMask4Consolidation()
{
    if (mbConsoBVCalculated)
		return S_OK;
    
    int hr = S_OK;
	mbConsoBVCalculated = true;
	
	mpvConsolidationBVs.resize(mpViewDataSet->getTabularData()->Count());
    for(int i=0; i<mpViewDataSet->getTabularData()->Count(); i++)
    {
        mpvConsolidationBVs[i] = NULL;
    }
    
	DSSTabularIndex *lIndex = mpReportLevelIndex;
	Int32 lnRowCount = 0;
	lnRowCount = lIndex->Count();
    
	if (lnRowCount == 0)
		return S_OK;
    
	Int32 lnUnits = 0;
	vector<Int32> lvUnits;
	lnUnits = lIndex->getNumUnits();
    
	if (lnUnits > 0){
		lvUnits.resize(lnUnits);
		hr = lIndex->getUnits(lnUnits, &lvUnits[0]);
		CHECK_HR;
	}
    
	DSSXTabBitVector lConsoMaskInfo;
	vector<DSSTabularConsolidation*> lvConsolidation;
	vector<Int32> lvConsolidationIndex;
	DSSTabularConsolidation *lpConsoUnit;
    
    DSSTabularData *lpTabularData = mpViewDataSet->getTabularData();
	for (Int32 i = 0; i < lnUnits; ++i)
	{
		DSSTabularUnit* lpUnit = NULL;
		lpTabularData->Item(lvUnits[i], &lpUnit);
		CHECK_HR_PTR(lpUnit);
        
		EnumDSSTemplateUnitType lUnitType;
		lUnitType = lpUnit->getUnitType();
		if (DssTemplateConsolidation == lUnitType)
		{
			DSSXTabBitVector *lpBV = new DSSXTabBitVector(lnRowCount);
			
			mpvConsolidationBVs[lvUnits[i]] = lpBV;
            
			lpConsoUnit = static_cast<DSSTabularConsolidation*>(lpUnit);
			Int32 lnCount = 0;
			lnCount = lpConsoUnit->GetMaxKey();
			vector<Int32> lvElements;
			for (Int32 j = 0; j < lnCount; j++)
			{
                if(lpConsoUnit->IsDisplayed(j))
                    lvElements.push_back(j);
			}
			// treat Join NULL as displayable
			lvElements.push_back(lnCount);
			hr = lIndex->CalculateMask(1, &lvUnits[i], &lvElements, *lpBV);
			CHECK_HR;
		}
	}
	
	return S_OK;
}


int DSSXTabView::hGetMaskByOVGBUnits(const vector<vector<Int32> >& ivvGBElements, DSSXTabBitVector& orMask)
{
	if (!mpGBTable)
		return S_OK;
    
	AE_ASSERT(mpGBTable->GetTargetRowCount() ==  orMask.GetLength() || mpGBTable->getType() == 0);
    
	HRESULT hr = S_OK;
    
	vector<GUID> lvUnitID;
	vector<vector<Int32> > lvvSelectElement;
	for (Int32 i = 0; i < mnOVGBUnits; i++)
	{
		GUID lID;
		hr = mpGBTable->GetUnitID(mpOVGBUnits[i], &lID);
		CHECK_HR;
        
		bool lfHasTotalAll = false;
		for (Int32 j = 0; j < ivvGBElements[mpOVGBUnitPos[i]].size(); j++)
			if (ivvGBElements[mpOVGBUnitPos[i]][j] == DSSXTAB_TOTAL_KEY || ivvGBElements[mpOVGBUnitPos[i]][j] == -1)
			{
				lfHasTotalAll = true;
				break;
			}
		if (!lfHasTotalAll)
		{
			lvUnitID.push_back(lID);
			vector<Int32> lvElement;
			lvElement.assign(ivvGBElements[mpOVGBUnitPos[i]].begin(), ivvGBElements[mpOVGBUnitPos[i]].end());
			lvvSelectElement.push_back(lvElement);
		}
	}
    
	AE_ASSERT(lvUnitID.size() == lvvSelectElement.size());
    
	return mpGBTable->GetSelectedTargetRow(lvUnitID, lvvSelectElement, orMask);
}


int DSSXTabView::getReportLevelSlice(int iMetric, DSSCubeDataSlice **oppSlice)
{
    DSSCubeDataSlice *lpSlice = NULL;
    
    DSSTabularIndex *lpIndex = NULL;
    int hr = hGetReportLevelIndex(&lpIndex);
    CHECK_HR;
    
    vector<int> lvUnits;
    hr = lpIndex->getUnits(lvUnits);
    CHECK_HR;
    
    DSSXTabKey *lpKeys = new int[mnTotalUnits];
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lpKeysStrongPtr(lpKeys);
    
	//first have to decide which level the mIndex mapps to
	for(int i = 0; i < mnTotalUnits; i++)
        lpKeys[i] = DSSXTAB_TOTAL_KEY;
    
    for(int i = 0; i < lvUnits.size(); ++i)
        lpKeys[lvUnits[i]] = 0;
    
    DSSDataFlag lFlag;
    hr = FindDataLocFromIndex(lpKeys, iMetric, 0, DssOriginalSlice, &lpSlice, lFlag);
    CHECK_HR;
    
    *oppSlice = lpSlice;
    return S_OK;
}

int DSSXTabView::setCube(DSSCube* ipCube)
{
    if(ipCube)
        mpCube = ipCube;
    return S_OK;
}

//imihai 06-10-2013
int DSSXTabView::getOffsetForCoordinates(int iRow, int iCol, int &lOffset, DSSDataFlag &pFlags)
{
    if(mfStatus != XTAB_INITIALIZED)
		return E_FAIL;
    
	int hr;
    
    pFlags = DssDataUnknown;
    
    DSSXTabKey * pIndex = new DSSXTabKey[sizeof(DSSXTabKey) * mnTotalUnits];
    MBase::DSSStrongPtr<DSSXTabKey, MBase::DeleteArray<DSSXTabKey> > pIndexStrongPtr(pIndex);
    
	int lMetric = 0,  lTag = 0 ;
    
	DSSTabularIndex* lpIndexObject = NULL;
	int lOffsetInIndexObject = -1;
	hr = FindIndexFromCoordinate(iRow, iCol, pIndex, &lMetric, &lTag, &lpIndexObject, &lOffsetInIndexObject);
	if(hr != S_OK && hr != S_FALSE) return hr;
    
    if(hr == S_FALSE)
	{
		pFlags = DssDataInvalidCell;
		return S_OK;		//we just did not find the data
	}
    
	int * lpUnit = new int [sizeof(int) * mnTotalUnits];
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lpUnitStrongPtr(lpUnit);
    
	int lnUnit = 0;
	for(int i = 0; i < mnTotalUnits; i ++)
        if(pIndex[i] != DSSXTAB_TOTAL_KEY)
            lpUnit[lnUnit ++] = i;
    
	DSSTabularIndex *lpIndices;
    DSSTabularIndexPool *lpIndexPool = NULL;
    hr = mpCube->getIndexPool(&lpIndexPool);
    CHECK_PTR(lpIndexPool);
    
	hr = lpIndexPool -> getIndex(lnUnit, lpUnit, &lpIndices);
	CHECK_PTR(lpIndices);
    
	lOffset = 0;
    
	if (lpIndexObject == lpIndices)
	{
		lOffset = lOffsetInIndexObject;
	}
	else
	{
        hr = lpIndices -> LookUp(mnTotalUnits, pIndex, &lOffset);
        if(hr != S_OK && hr != S_FALSE)
            return E_FAIL;
        
        if(hr == S_FALSE || lpIndices->IsUnqualified(lOffset))
        {
            //look up failed
            pFlags = DssDataInvalidCell;
            return S_OK;
        }
	}
    
	return S_OK;
}

bool DSSXTabView::isInitialized()
{
    if(!mHeaders[0] && !mHeaders[1])
        return false;
    return true;
}

int DSSXTabView::ResetThresholdSlice(vector<int>& ivUnits, int  iSliceID, int  iMetric, int  iTag, EnumDssSliceFlag iSliceFlag)
{
    int hr = S_OK;
    
    if(!mpSliceMap)
    {
        return S_OK;
    }

	bool * llevelMap = new bool[mnTotalUnits];
    MBase::DSSStrongPtr<bool, MBase::DeleteArray<bool> > lLevelMapStrongPtr(llevelMap);
    
	CHECK_NEWED_PTR(llevelMap);

    // convert it into attribute map (CDSSSliceMapper standard)
    for(int j = 0; j < mnTotalUnits; j++)
        llevelMap[j] = false;
    for(int j = 0; j < ivUnits.size(); j++)
        llevelMap[ivUnits[j]] = true;
        
    
    //TODO: dummy unit
    //xhan 01/07/05 put dummy unit into report level
    //if (i == 0 mDummyUnitIndex != -1)
    //    llevelMap[mDummyUnitIndex] = true;
        
    hr = mpSliceMap->ReplaceSliceByLevel(llevelMap, iSliceID, iMetric, iTag, iSliceFlag);
    CHECK_HR;
        
    return hr;
}



int DSSXTabView::getMetricSliceOffset(int iRow, int iMetric, int* opOffset, bool ibSkipSubTotal)
{
    if(mfStatus != XTAB_INITIALIZED)
		return E_FAIL;
    
	int hr = S_OK;
    CHECK_PTR(opOffset);
    
    if(!mHeaders[0] && !mHeaders[1])
        return hr;
    
    bool lbMetricOnColumn = true;
    if(mHeaders[0] && mHeaders[0]->HasMetric())
    {
        lbMetricOnColumn = false;
    }
    
    DSSXTabKey * pIndex = new DSSXTabKey[sizeof(DSSXTabKey) * mnTotalUnits];
    MBase::DSSStrongPtr<DSSXTabKey, MBase::DeleteArray<DSSXTabKey> > pIndexStrongPtr(pIndex);
    for(int i = 0; i < mnTotalUnits; i++)
        pIndex[i] = DSSXTAB_TOTAL_KEY;
    
    int lTag = 0 ;
    if(mHeaders[0])
    {
        hr = mHeaders[0] -> getSubtotalID(iRow, lTag);
        if(hr != S_OK) return hr;
    }
    
    if(ibSkipSubTotal && lTag != 0)
    {
        *opOffset = -1;
        return hr;
    }
	
    int lnCol = 0;
    if(mHeaders[1])
    {
        lnCol = mHeaders[1]->getRowCount();
    }
    
	int * lpUnit = new int [sizeof(int) * mnTotalUnits];
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lpUnitStrongPtr(lpUnit);

    DSSTabularIndex *lpIndices = NULL;
    int lOffset = -1;
    for(int i = 0; i < lnCol; i++)
    {
        int lMetric = getMetricID(iRow, i);
        if(lMetric == iMetric)
        {
            DSSTabularIndex *lpIndex1 = NULL;
            int lOffset1 = -1;
            hr = this->FindIndexFromCoordinate(iRow, i, pIndex, &lMetric, &lTag, &lpIndex1, &lOffset1);
            if(hr != S_OK && hr != S_FALSE)
                return hr;
            
            int lnUnit = 0;
            for(int i = 0; i < mnTotalUnits; i ++)
                if(pIndex[i] != DSSXTAB_TOTAL_KEY)
                    lpUnit[lnUnit ++] = i;
            
  
            DSSTabularIndexPool *lpIndexPool = NULL;
            hr = mpCube->getIndexPool(&lpIndexPool);
            CHECK_PTR(lpIndexPool);
            
            hr = lpIndexPool -> getIndex(lnUnit, lpUnit, &lpIndices);
            CHECK_HR_PTR(lpIndices);
            
            if (lpIndex1 == lpIndices)
            {
                lOffset = lOffset1;
            }
            else
            {
                hr = lpIndices -> LookUp(mnTotalUnits, pIndex, &lOffset);
                if(hr == S_FALSE)
                {
                    //it is an xtab cell
                    lOffset = -1;
                    lpIndices = NULL;
                    continue;
                }else if(hr == S_OK)
                {
                    break;
                }else
                {
                    return hr;
                }
            }

        }
    }
    
    if(lpIndices == NULL)
    {
        return S_FALSE;
    }

	if(mnMetrics == 0)
	{
		return S_OK;	//no further processing
	}
    
	//	XW: now do not do assert	_ASSERTE(lMetric < mnMetric && lMetric >= 0);
	if(iMetric >= mnMetrics || iMetric < 0)
	{
		(*opOffset) = -1;
		return S_OK;
	}
	//now we have lMetric, pIndex and lTag
	DSSCubeDataSlice * lSlice = NULL;
    EnumDssSliceFlag lSliceFlag = DssOriginalSlice;
    DSSDataFlag lFlag = DssDataOk;
	hr = FindDataLocFromIndex(pIndex, iMetric, lTag, lSliceFlag, &lSlice, lFlag);
	CHECK_HR;
    
	if(!lSlice)
		return S_OK;
    
	//may be S_FALSE
	DSSTabularIndex *lpSliceIndex;
	lpSliceIndex = lSlice->getIndices();
	CHECK_PTR(lpSliceIndex);
    
	// If the slice has the same index as the index we just checked, we will use the offset directly
	if (lpSliceIndex && lpSliceIndex->IsSameOrder(lpIndices))
	{
		(*opOffset) = lOffset;
	}
	else
	{
        hr = lpSliceIndex->LookUp(mnTotalUnits, pIndex, opOffset);
        CHECK_HR;
	}
    return hr;
}



//zhyang, the following two funtions are copied from server side and related to extrem/ reference line/ trend line.

// This method is trying to get the Extremes/Reference Line/Trend Line
// iExtremeType      EnumDSSRuntimeSubtotalType could be DssRuntimeSubtotalGlobal, DssRuntimeSubtotalPerRow, DssRuntimeSubtotalPerColumn, DssRuntimeSubtotalPerCell
// ipRows: first row indices of each chart
// ipColumns: first column indices of each chart
// ipBuffer: of type MBase:Buffer
// onMetricIDs       count of metric id
// oppMetricIDs      metric IDs
// oppDataTypes      Data type of metric
// oppExtremeType EnumDSSRuntimeSubtotalType, indicate Extreme type: Scale Extreme, Sizeby, Colorby
// onGroupExtremeInfos     number of Extreme Info groups, each group contains *onMetricIDs number of ExtremeInfo
// oppExtremeInfos   Extreme Infos, of length (*onMetricIDs) * (onExtremeInfos). We should cast it to (ExtremeInfo **) type
int DSSXTabView::getExtremes(EnumDSSRuntimeSubtotalType iExtremeType,
                             Int32 nRows,
                             Int32 *ipRows,
                             Int32 nColumns,
                             Int32 *ipColumns,
                             Int32 *onMetricIDs,
                             Int32 **oppMetricIDs,
                             EnumDSSDataType **oppDataTypes,
                             EnumDSSRuntimeSubtotalType **oppExtremeTypes,
                             Int32 *onGroupExtremeInfos,
                             void ** oppExtremeInfos)
{
    HRESULT hr = S_OK;
    
    //TODO, validate ipRows and ipColumns according to iBlocks
    if ((iExtremeType == DssRuntimeSubtotalPerRow && nRows == 0)
        || (iExtremeType == DssRuntimeSubtotalPerColumn && nColumns == 0)
        || (iExtremeType == DssRuntimeSubtotalPerCell && (nRows == 0 || nColumns == 0)))
        return S_OK;
    
    if (!onMetricIDs || !oppMetricIDs || !oppDataTypes || !onGroupExtremeInfos || !oppExtremeInfos)
        return E_POINTER;
    
    if (iExtremeType == DssRuntimeSubtotalGlobal) {
        for (Int32 unit = 0; unit < mnGroupByUnits; unit++) {
            if (mpvLastGBElements[unit]->size() <= 0) {
                return S_OK; // no extreme will be returned because no valid elements are selected (the selected elements result in no data)
            }
        }
    }
    DSSTabularData *lpTabularData = mpViewDataSet->getTabularData();
    CHECK_HR_PTR(lpTabularData);
    
    Int32 nLevels;
    hr = mpSliceMap->CountLevels(&nLevels);
    CHECK_HR;
    
    Int32 lMetricCount = 0;
    vector<Int32> lvMetricIDs(mnMetrics);
    vector<EnumDSSDataType> lvDataTypes(mnMetrics);
    map<Int32, map<Int32, vector<Int32>, less<Int32> >, less<Int32> > lMetricIDToSliceIDMap; //<metric id,<subtotal type, sliceID> >
    map<Int32, map<Int32, vector<DSSModelInfo*>, less<Int32> >, less<Int32> > lmapMetricIDToModelInfo;
    
    hr = hGetMetricAndSlice(EnumDSSRuntimeSubtotalType(DssRuntimeSubtotalExtremeMin|DssRuntimeSubtotalExtremeMax|DssRuntimeSubtotalExtremeSizeBy|DssRuntimeSubtotalExtremeColorBy), iExtremeType, &lMetricCount, lvMetricIDs, lvDataTypes, lMetricIDToSliceIDMap, lmapMetricIDToModelInfo);
    CHECK_HR;
    
    // rzhu, 832540, sizeby/colorby and scale extreme may have different values
    vector<Int32> lvRsltMetricIDs;
    vector<EnumDSSDataType> lvRsltDataTypes;
    vector<EnumDSSRuntimeSubtotalType> lvRsltExtremeType;
    vector<Int32> lvSliceIDs;
    vector<EnumDSSRuntimeSubtotalType> lvExtremes;
    // groupby extreme type
    
    map<Int32, map<Int32, vector<Int32>, less<Int32> >, less<Int32> >::iterator it;
    for (Int32 lMetricIndex = 0; lMetricIndex < lMetricCount; lMetricIndex++)
    {
        map<EnumDSSRuntimeSubtotalType, Int32> lmapExtremeType2Index;
        it = lMetricIDToSliceIDMap.find(lvMetricIDs[lMetricIndex]);
        if(it != lMetricIDToSliceIDMap.end())
        {
            map<Int32, vector<Int32>, less<Int32> >::iterator it2 = it->second.begin();
            while(it2 != it->second.end())
            {
                for (Int32 iSlice = 0; iSlice < it2->second.size(); iSlice++)
                {
                    EnumDSSRuntimeSubtotalType lExtremeType = EnumDSSRuntimeSubtotalType(it2->first & 0x00010);
                    if (lmapExtremeType2Index.find(lExtremeType) == lmapExtremeType2Index.end())
                    {
                        lmapExtremeType2Index[lExtremeType] = lvRsltMetricIDs.size();
                        lvRsltMetricIDs.push_back(lvMetricIDs[lMetricIndex]);
                        lvRsltDataTypes.push_back(lvDataTypes[lMetricIndex]);
                        lvRsltExtremeType.push_back(lExtremeType);
                        lvSliceIDs.push_back(-1);
                        lvSliceIDs.push_back(-1);
                        lvExtremes.push_back(DssRuntimeSubtotalDefault);
                        lvExtremes.push_back(DssRuntimeSubtotalDefault);
                    }
                    
                    Int32 lIndex = lmapExtremeType2Index.find(lExtremeType)->second;
                    if (it2->first & DssRuntimeSubtotalExtremeMin)
                    {
                        lvSliceIDs[lIndex * 2] = it2->second[iSlice];
                        lvExtremes[lIndex * 2] = EnumDSSRuntimeSubtotalType(it2->first);
                    }
                    else
                    {
                        lvSliceIDs[lIndex * 2 + 1] = it2->second[iSlice];
                        lvExtremes[lIndex * 2 + 1] = EnumDSSRuntimeSubtotalType(it2->first);
                    }
                }
                it2++;
            }
        }
    }
    
    lMetricCount = lvRsltMetricIDs.size();
    Int32 *lpMetricIDs = new Int32[lMetricCount];
    CHECK_NEWED_PTR(lpMetricIDs);
    EnumDSSDataType *lpDataTypes = new EnumDSSDataType[lMetricCount];
    EnumDSSRuntimeSubtotalType *lpExtremeType = new EnumDSSRuntimeSubtotalType[lMetricCount];
    CHECK_NEWED_PTR(lpExtremeType);
    if (lMetricCount > 0)
    {
        memmove(lpMetricIDs, &lvRsltMetricIDs[0], sizeof(Int32) * lMetricCount);
        memmove(lpDataTypes, &lvRsltDataTypes[0], sizeof(EnumDSSDataType) * lMetricCount);
        memmove(lpExtremeType, &lvRsltExtremeType[0], sizeof(EnumDSSRuntimeSubtotalType) * lMetricCount);
    }
    
    ExtremeInfo *lpExtremeInfos = NULL;
    Int32 lnGroupExtremeInfos = 0;
    
    DSSXTabKey pIndex[mnTotalUnits];
    Int32 lMetric = 0,  lTag = 0 ;
    DSSTabularIndex* lpIndexObject = NULL;
    Int32 lOffsetInIndexObject = -1;
    
    // get Global extremes
    if (iExtremeType == DssRuntimeSubtotalGlobal)
    {
        hr = S_OK;
        lnGroupExtremeInfos = 1;
        lpExtremeInfos = new ExtremeInfo[lnGroupExtremeInfos * lMetricCount];
        CHECK_NEWED_PTR(lpExtremeInfos);
        CHECK_HR_PTR(lpExtremeInfos);
        
        hr = FindIndexFromCoordinate(0, 0, pIndex, &lMetric, &lTag, &lpIndexObject, &lOffsetInIndexObject);
        CHECK_HR;
        
        for (Int32 lMetricIndex = 0; lMetricIndex < lMetricCount; lMetricIndex++)
        {
            Int32 lIndex = lMetricIndex * 2;
            if (lvSliceIDs[lIndex] > -1)
            {
                hr = hgetValueFromSlice(lvSliceIDs[lIndex], pIndex, lpIndexObject, lOffsetInIndexObject, lvExtremes[lIndex], lpExtremeInfos + lMetricIndex);
                CHECK_HR;
            } else {
                (lpExtremeInfos + lMetricIndex)->mDataFlag = DssDataMissing;
            }
            if (lvSliceIDs[lIndex + 1] > -1)
            {
                hr = hgetValueFromSlice(lvSliceIDs[lIndex + 1], pIndex, lpIndexObject, lOffsetInIndexObject, lvExtremes[lIndex + 1], lpExtremeInfos + lMetricIndex);
                CHECK_HR;
            } else {
                (lpExtremeInfos + lMetricIndex)->mDataFlag = DssDataMissing;
            }
        }
        
    }
    else if (iExtremeType == DssRuntimeSubtotalPerRow || iExtremeType == DssRuntimeSubtotalPerColumn)
    {
        Int32 lnCount = 0;
        Int32 *lpIndices = NULL;
        if (iExtremeType == DssRuntimeSubtotalPerRow )
        {
            lnCount = nRows;
            lpIndices = ipRows;
        }
        else
        {
            lnCount = nColumns;
            lpIndices = ipColumns;
        }
        
        hr = S_OK;
        lnGroupExtremeInfos = lnCount;
        lpExtremeInfos = new ExtremeInfo[lnGroupExtremeInfos * lMetricCount];
        CHECK_NEWED_PTR(lpExtremeInfos);
        CHECK_HR_PTR(lpExtremeInfos);
        Int32 lCount = 0;
        for (Int32 iRow = 0; iRow < lnCount; iRow++)
        {
            if (iExtremeType == DssRuntimeSubtotalPerRow )
            {
                hr = FindIndexFromCoordinate(lpIndices[iRow], 0, pIndex, &lMetric, &lTag, &lpIndexObject, &lOffsetInIndexObject);
            }
            else
            {
                hr = FindIndexFromCoordinate(0, lpIndices[iRow], pIndex, &lMetric, &lTag, &lpIndexObject, &lOffsetInIndexObject);
            }
            CHECK_HR;
            for (Int32 lMetricIndex = 0; lMetricIndex < lMetricCount; lMetricIndex++)
            {
                Int32 lIndex = lMetricIndex * 2;
                if (lvSliceIDs[lIndex] > -1)
                {
                    hr = hgetValueFromSlice(lvSliceIDs[lIndex], pIndex, lpIndexObject, lOffsetInIndexObject, lvExtremes[lIndex], lpExtremeInfos + lCount * lMetricCount + lMetricIndex);
                    CHECK_HR;
                } else {
                    (lpExtremeInfos + lCount * lMetricCount + lMetricIndex)->mDataFlag = DssDataMissing;
                }
                if (lvSliceIDs[lIndex + 1] > -1)
                {
                    hr = hgetValueFromSlice(lvSliceIDs[lIndex + 1], pIndex, lpIndexObject, lOffsetInIndexObject, lvExtremes[lIndex + 1], lpExtremeInfos + lCount * lMetricCount + lMetricIndex);
                    CHECK_HR;
                } else {
                    (lpExtremeInfos + lCount * lMetricCount + lMetricIndex)->mDataFlag = DssDataMissing;
                }
            }
            lCount++;
        }
    }
    else if (iExtremeType == DssRuntimeSubtotalPerCell)
    {
        hr = S_OK;
        lnGroupExtremeInfos = nRows * nColumns;
        lpExtremeInfos = new ExtremeInfo[lnGroupExtremeInfos * lMetricCount];
        CHECK_NEWED_PTR(lpExtremeInfos);
        CHECK_HR_PTR(lpExtremeInfos);
        
        Int32 lCount = 0;
        for (Int32 iRow = 0; iRow < nRows; iRow++)
        {
            for (Int32 iCol = 0; iCol < nColumns; iCol++)
            {
                hr = FindIndexFromCoordinate(ipRows[iRow], ipColumns[iCol], pIndex, &lMetric, &lTag, &lpIndexObject, &lOffsetInIndexObject);
                CHECK_HR;
                
                for (Int32 lMetricIndex = 0; lMetricIndex < lMetricCount; lMetricIndex++)
                {
                    Int32 lIndex = lMetricIndex * 2;
                    if (lvSliceIDs[lIndex] > -1)
                    {
                        hr = hgetValueFromSlice(lvSliceIDs[lIndex], pIndex, lpIndexObject, lOffsetInIndexObject, lvExtremes[lIndex], lpExtremeInfos + lCount * lMetricCount + lMetricIndex);
                        CHECK_HR;
                    } else {
                        (lpExtremeInfos + lCount * lMetricCount + lMetricIndex)->mDataFlag = DssDataMissing;
                    }
                    if (lvSliceIDs[lIndex + 1] > -1)
                    {
                        hr = hgetValueFromSlice(lvSliceIDs[lIndex + 1], pIndex, lpIndexObject, lOffsetInIndexObject, lvExtremes[lIndex + 1], lpExtremeInfos + lCount * lMetricCount + lMetricIndex);
                        CHECK_HR;
                    } else {
                        (lpExtremeInfos + lCount * lMetricCount + lMetricIndex)->mDataFlag = DssDataMissing;
                    }
                }
                lCount++;
            }
        }
    }
    else
    {
        return S_FALSE;
    }
    
    *onMetricIDs = lMetricCount;
    *oppMetricIDs = lpMetricIDs;
    *oppDataTypes = lpDataTypes;
    *oppExtremeTypes = lpExtremeType;
    *onGroupExtremeInfos = lnGroupExtremeInfos;
    *oppExtremeInfos = (void*)lpExtremeInfos;
    
    return S_OK;
}

int DSSXTabView::getRefOrTrendInfo(EnumDSSRuntimeSubtotalType iLineType,
                                   EnumDSSRuntimeSubtotalType iLevel,
                                   Int32 nRows,
                                   Int32 *ipRows,
                                   Int32 nColumns,
                                   Int32 *ipColumns,
                                   Int32 nBreakbyUnits,
                                   Int32 *ipBreakbyUnits,
                                   Int32 *ipRowWindow,
                                   Int32 *ipColumnWindow,
                                   Int32 *onMetricIDs,
                                   Int32 **oppMetricIDs,                                 // length of *onMetricIDs
                                   Int32 **oppModelIndices,                       // length of *onMetricIDs
                                   Int32 **oppTRMetricIDs,                        // length of *onMetricIDs, only for trendline over metrics case
                                   EnumDSSDataType **oppDataTypes,          // length of *onMetricIDs
                                   Int32 **oppnVariables,
                                   Int32 ***opppVariables,
                                   EnumDSSObjectType ***opppVariableTypes,
                                   Int32 **opIsIncludeBreakBy,                    // length of *onMetricIDs, only for trendline over metrics case
                                   Int32 **oppnBreakbyElements,                   // length of (nRows * nColumns) depends on iLevel
                                   DSSXTabKey **oppBreanbyKeys,                    // length of (nRows * nColumns) * (*opnBreakElements) * nBreakbyUnits
                                   Int32 ***oppnSize,                             // length of (nRows * nColumns) * (*opnBreakElements),
                                   unsigned char ****oppInfo,                 // length of (nRows * nColumns) * (*opnBreakElements),
                                   DSSDataFlag ***oppDataFlags,           // length of (nRows * nColumns) * (*opnBreakElements),
                                   void ****oppForecastInfo                      // type fo ForecastInfo ***,length of (nRows * nColumns) * (*opnBreakElements),
)
{
    HRESULT hr = S_OK;
    
    //TODO, validate ipRows and ipColumns according to iBlocks
    if ((iLevel == DssRuntimeSubtotalPerRow && nRows == 0)
        || (iLevel == DssRuntimeSubtotalPerColumn && nColumns == 0)
        || (iLevel == DssRuntimeSubtotalPerCell && (nRows == 0 || nColumns == 0)))
        return S_OK;
    
    if (!onMetricIDs || !oppMetricIDs || !oppDataTypes || !oppModelIndices || !oppTRMetricIDs || !oppnBreakbyElements || !oppnSize || !oppInfo || !oppDataFlags || !oppBreanbyKeys)
        return E_POINTER;
    
    DSSTabularData *lTabularDataPtr = mpViewDataSet->getTabularData();
    CHECK_HR_PTR(lTabularDataPtr);
    
    vector<EnumDSSRuntimeSubtotalType> lvExtremeTypes;
    vector<Int32> lvSliceIDs;
    
    Int32 nLevels;
    hr = mpSliceMap->CountLevels(&nLevels);
    
    Int32 lMetricCount = 0;
    vector<Int32> lvMetricIDs(mnMetrics);
    vector<EnumDSSDataType> lvDataTypes(mnMetrics);
    map<Int32, map<Int32, vector<Int32>, less<Int32> >, less<Int32> > lMetricIDToSliceIDMap; //<metric id,<subtotal type, sliceID> >
    map<Int32, map<Int32, vector<DSSModelInfo*>, less<Int32> >, less<Int32> > lmapMetricIDToModelInfo;
    hr = hGetMetricAndSlice(iLineType, iLevel, &lMetricCount, lvMetricIDs, lvDataTypes, lMetricIDToSliceIDMap, lmapMetricIDToModelInfo);
    CHECK_HR;
    
    vector<Int32> lvResultMetricIDs;
    vector<Int32> lvResultModelIndices;
    vector<Int32> lvTRMetricIDs;
    vector<Int32> lvnxVariables;
    vector<Int32*> lvpxVariables;
    vector<EnumDSSObjectType *> lvpVariableTypes;
    vector<EnumDSSDataType> lvResultDataTypes;
    vector<bool> includeBreakBy;
    //vector<CDSSCubeSlice*> lvCubeSlices;
    //vector<Int32> lvSlices;
    vector<vector<Int32, MBase::Allocator<Int32> >* > lvpnSize;
    vector<vector<unsigned char*, MBase::Allocator<unsigned char*> >*> lvpvResultInfo;
    vector<vector<DSSDataFlag, MBase::Allocator<DSSDataFlag> >*> lvpvResultDataFlags;
    vector<Int32, MBase::Allocator<Int32> >* lpvnBreakbyElements;
    vector<Int32, MBase::Allocator<Int32> >* lpvpBreakByKeys;
    vector<vector<ForecastInfo*, MBase::Allocator<ForecastInfo*> > * > lvpvForecastInfos;
    bool hasBreakBy = false;
    
    map<Int32, map<Int32, vector<Int32>, less<Int32> >, less<Int32> >::iterator it = lMetricIDToSliceIDMap.begin();
    map<Int32, map<Int32, vector<DSSModelInfo*>, less<Int32> >, less<Int32> >::iterator lModelIt;
    map<Int32, vector<DSSModelInfo*>, less<Int32> >::iterator lModelIt2;
    
    Int32 lMetricIndex = 0;
    Int32 lFirstMetricWithBreakBy = -1;
    while(it != lMetricIDToSliceIDMap.end())
    {
        map<Int32, vector<Int32>, less<Int32> >::iterator it2 = it->second.begin();
        lModelIt = lmapMetricIDToModelInfo.find(it->first);
        while(it2 != it->second.end())
        {
            for (Int32 iSlice = 0; iSlice < it2->second.size(); iSlice++)
            {
                Int32 lSliceID = it2->second[iSlice];
                lvSliceIDs.push_back(lSliceID);
                lvResultMetricIDs.push_back(it->first);
                DSSModelInfo *lpModelInfo = NULL;
                if (lModelIt != lmapMetricIDToModelInfo.end())
                {
                    lModelIt2 = lModelIt->second.find(it2->first);
                    if (lModelIt2 != lModelIt->second.end())
                    {
                        lpModelInfo = lModelIt2->second[iSlice];
                    }
                }
                
                if (lpModelInfo)
                {
                    lvResultModelIndices.push_back(lpModelInfo->mModelIndex);
                    lvTRMetricIDs.push_back(lpModelInfo->mTRMetricID);
                    lvnxVariables.push_back(lpModelInfo->mnVariable);
                    lvpxVariables.push_back(lpModelInfo->mpVariables);
                    lvpVariableTypes.push_back(lpModelInfo->mpVariableTypes);
                }
                else
                {
                    lvResultModelIndices.push_back(-1);
                    lvTRMetricIDs.push_back(-1);
                    lvnxVariables.push_back(0);
                    lvpxVariables.push_back(NULL);
                    lvpVariableTypes.push_back(NULL);
                }
                
                DSSCubeDataSlice* lpSlice = mpCube->getSliceByID(lSliceID);
                CHECK_HR_PTR(lpSlice);
                
                EnumDSSDataType lDataType = lpSlice->getDataType();
                CHECK_HR;
                
                lvResultDataTypes.push_back(lDataType);
                
                if (nBreakbyUnits  > 0)
                {
                    if (mmapIncludeBreakBy.find(lSliceID) != mmapIncludeBreakBy.end())
                    {
                        includeBreakBy.push_back(true);
                        if (!hasBreakBy)
                        {
                            lFirstMetricWithBreakBy = lMetricIndex;
                            hasBreakBy = true;
                        }
                    }
                    else
                    {
                        includeBreakBy.push_back(false);
                    }
                }
                lMetricIndex++;
            }
            
            it2++;
        }
        it++;
    }
    
    *onMetricIDs = lMetricIndex;
    if (lMetricIndex == 0)
    {
        return S_OK;
    }
    
    *oppMetricIDs = new Int32[lMetricIndex];
    CHECK_NEWED_PTR(*oppMetricIDs)
    memmove(*oppMetricIDs, &lvResultMetricIDs[0], sizeof(Int32) * lMetricIndex);
    
    *oppModelIndices = new Int32[lMetricIndex];
    CHECK_NEWED_PTR(*oppModelIndices)
    memmove(*oppModelIndices, &lvResultModelIndices[0], sizeof(Int32) * lMetricIndex);
    
    *oppTRMetricIDs = new Int32[lMetricIndex];
    CHECK_NEWED_PTR(oppTRMetricIDs);
    memmove(*oppTRMetricIDs, &lvTRMetricIDs[0], sizeof(Int32) * lMetricIndex);
    
    *oppDataTypes = new EnumDSSDataType[lMetricIndex];
    CHECK_NEWED_PTR(*oppDataTypes)
    memmove(*oppDataTypes, &lvResultDataTypes[0], sizeof(EnumDSSDataType) * lMetricIndex);
    
    *oppnVariables = new Int32[lMetricIndex];
    CHECK_NEWED_PTR(*oppnVariables);
    memmove(*oppnVariables, &lvnxVariables[0], sizeof(Int32) * lMetricIndex);
    
    *opppVariables = new Int32*[lMetricIndex];
    CHECK_NEWED_PTR(*opppVariables);
    memmove(*opppVariables, &lvpxVariables[0], sizeof(Int32*) * lMetricIndex);
    
    *opppVariableTypes = new EnumDSSObjectType *[lMetricIndex];
    CHECK_NEWED_PTR(*opppVariableTypes);
    memmove(*opppVariableTypes, &lvpVariableTypes[0], sizeof(EnumDSSObjectType*) * lMetricIndex);
    
    lvpnSize.resize(lMetricIndex);
    lvpvResultInfo.resize(lMetricIndex);
    lvpvResultDataFlags.resize(lMetricIndex);
    lvpvForecastInfos.resize(lMetricIndex, NULL);
    
    if (hasBreakBy)
    {
        lpvnBreakbyElements = new vector<Int32, MBase::Allocator<Int32> >;
        CHECK_NEWED_PTR(lpvnBreakbyElements);
        lpvpBreakByKeys = new vector<Int32, MBase::Allocator<Int32> >;
        CHECK_NEWED_PTR(lpvpBreakByKeys);
    }
    
    vector<ForecastInfo*> lvSavedForecastInfo;
    for (lMetricIndex = 0; lMetricIndex < *onMetricIDs; lMetricIndex++)
    {
        vector<Int32, MBase::Allocator<Int32> >* lpvnSize = new vector<Int32, MBase::Allocator<Int32> >;
        CHECK_NEWED_PTR(lpvnSize);
        lvpnSize[lMetricIndex] = lpvnSize;
        vector<unsigned char*, MBase::Allocator<unsigned char*> >* lpvResultInfo =
        new vector<unsigned char*, MBase::Allocator<unsigned char*> >;
        CHECK_NEWED_PTR(lpvResultInfo);
        lvpvResultInfo[lMetricIndex] = lpvResultInfo;
        vector<DSSDataFlag, MBase::Allocator<DSSDataFlag> >* lpvResultDataFlags=
        new  vector<DSSDataFlag, MBase::Allocator<DSSDataFlag> >;
        CHECK_NEWED_PTR(lpvResultDataFlags);
        lvpvResultDataFlags[lMetricIndex] = lpvResultDataFlags;
        
        Int32 lSliceID = lvSliceIDs[lMetricIndex];
        
        ForecastInfo *lpForecastInfo = NULL;
        if (mmapForecastInfos.find(lSliceID) != mmapForecastInfos.end())
        {
            lpForecastInfo = mmapForecastInfos.find(lSliceID)->second;
            vector<ForecastInfo*, MBase::Allocator<ForecastInfo*> >* lpvForecastInfo = new vector<ForecastInfo*, MBase::Allocator<ForecastInfo*> >;
            CHECK_NEWED_PTR(lpvForecastInfo);
            lvpvForecastInfos[lMetricIndex] = lpvForecastInfo;
        }
        
        lvSavedForecastInfo.push_back(lpForecastInfo);
    }
    
    DSSXTabKey pIndex[mnTotalUnits];
    Int32 lMetric = 0,  lTag = 0 ;
    DSSTabularIndex* lpIndexObject = NULL;
    Int32 lOffsetInIndexObject = -1;
    
    Int32 nResRows = 0;
    Int32 *lpResRows = NULL;
    Int32 nResCols = 0;
    Int32 *lpResCols = NULL;
    
    Int32 nRowsBb = 0;
    Int32 *lpRowsBb = NULL;
    Int32 nColsBb = 0;
    Int32 *lpColsBb = NULL;
    Int32 lBreakByAxis = -1;
    
    vector<Int32> lvRows(1, 0);
    vector<Int32> lvCols(1, 0);
    
    Int32 lnBreakByCount;
    
    
    Int32 lnOrigRow = 0;
    Int32 *lpOrigRow = NULL;
    
    Int32 lnOrigCol = 0;
    Int32 *lpOrigCol = NULL;
    
    if (iLevel == DssRuntimeSubtotalGlobal)
    {
        lnOrigRow = 1;
        lpOrigRow = new Int32[1];
        CHECK_NEWED_PTR(lpOrigRow);
        lpOrigRow[0] = 0;
        
        lnOrigCol = 1;
        lpOrigCol = lpOrigRow;
    }
    else if (iLevel == DssRuntimeSubtotalPerRow)
    {
        lnOrigRow = nRows;
        lpOrigRow = ipRows;
        
        lnOrigCol = 1;
        lpOrigCol = new Int32[1];
        CHECK_NEWED_PTR(lpOrigCol);
        lpOrigCol[0] = 0;
    }
    else if (iLevel == DssRuntimeSubtotalPerColumn)
    {
        lnOrigRow = 1;
        lpOrigRow = new Int32[1];
        CHECK_NEWED_PTR(lpOrigRow);
        lpOrigRow[0] = 0;
        
        lnOrigCol = nColumns;
        lpOrigCol = ipColumns;
    }
    else if (iLevel == DssRuntimeSubtotalPerCell)
    {
        lnOrigRow = nRows;
        lpOrigRow = ipRows;
        
        lnOrigCol = nColumns;
        lpOrigCol = ipColumns;
    }
    
    DSSTabularIndex* lpCellLevelIndices = NULL;
    Int32 lnTotal = 0;
    Int32 lnBreakBlock = 0;
    for (Int32 iRow = 0; iRow < lnOrigRow; iRow++)
    {
        for (Int32 iCol = 0; iCol < lnOrigCol; iCol++)
        {
            if (hasBreakBy)
            {
                hr = hGetRowAndColWithBreakBy(lpOrigRow[iRow], iRow < lnOrigRow - 1 ? lpOrigRow[iRow + 1] : mRowCount, lpOrigCol[iCol], iCol < lnOrigCol - 1 ? lpOrigCol[iCol + 1] : mColCount, nBreakbyUnits, ipBreakbyUnits, &nRowsBb, &lpRowsBb, &nColsBb, &lpColsBb, &lBreakByAxis);
                CHECK_HR;
                if (lBreakByAxis == 0)
                {
                    nColsBb = 1;
                }
                else
                {
                    nRowsBb = 1;
                }
            }
            
            for (lMetricIndex = 0; lMetricIndex < *onMetricIDs; lMetricIndex++)
            {
                if (hasBreakBy && includeBreakBy[lMetricIndex])
                {
                    nResRows = nRowsBb;
                    nResCols = nColsBb;
                    lpResRows = lpRowsBb;
                    lpResCols = lpColsBb;
                }
                else
                {
                    nResRows = 1;
                    nResCols = 1;
                    lpResRows = lpOrigRow + iRow;
                    lpResCols = lpOrigCol + iCol;
                }
                vector<Int32, MBase::Allocator<Int32> >* lpvnSize = lvpnSize[lMetricIndex];
                vector<unsigned char*, MBase::Allocator<unsigned char*> >* lpvResultInfo = lvpvResultInfo[lMetricIndex];
                vector<DSSDataFlag, MBase::Allocator<DSSDataFlag> >* lpvResultDataFlags = lvpvResultDataFlags[lMetricIndex];
                vector<ForecastInfo*, MBase::Allocator<ForecastInfo*> >* lpvForecastInfo = lvpvForecastInfos[lMetricIndex];
                
                
                Int32 lSliceID = lvSliceIDs[lMetricIndex];
                DSSCubeDataSlice *lpSlice = mpCube->getSliceByID(lSliceID);
                CHECK_HR;
                
                ForecastInfo *lpSavedForecastInfo = lvSavedForecastInfo[lMetricIndex];
                
                lnBreakByCount = 0;
                for (Int32 iResRow = 0; iResRow < nResRows; iResRow++)
                {
                    for (Int32 iResCol = 0; iResCol < nResCols; iResCol++)
                    {
                        // jzeng, sync with iserver, check whether cell exists for cell level
                        if ((iLevel ==  DssRuntimeSubtotalPerCell) && (lpCellLevelIndices == NULL) && (lvnxVariables[lMetricIndex] > 0))
                        {
                            vector<Int32> lvExcludeUnits;
                            for (Int32 i = 0; i < nBreakbyUnits; i++)
                                lvExcludeUnits.push_back(ipBreakbyUnits[i]);
                            
                            for (Int32 i = 0; i < lvnxVariables[lMetricIndex]; i++)
                                lvExcludeUnits.push_back(lvpxVariables[lMetricIndex][i]);
                            
                            int lMetric2, lTag2;
                            hr = hFindCellLevelIndex(lpResRows[iResRow], lpResCols[iResCol], &lpCellLevelIndices, &lMetric2, &lTag2, NULL, NULL, NULL, lvExcludeUnits.size(), (lvExcludeUnits.empty() ? NULL : &lvExcludeUnits[0]));
                            CHECK_HR;
                        }
                        hr = FindIndexFromCoordinate(ipRowWindow[lpResRows[iResRow]], ipColumnWindow[lpResCols[iResCol]], pIndex, &lMetric, &lTag, &lpIndexObject, &lOffsetInIndexObject);
                        CHECK_HR;
                        
                        if (lpCellLevelIndices)
                        {
                            int lOffset1 = 0;
                            hr = lpCellLevelIndices->LookUp(mnTotalUnits, pIndex, &lOffset1);
                            CHECK_HR;
                        }
                        unsigned char *lpData = NULL;
                        Int32 lcbWritten;
                        EnumDSSDataType lType;
                        DSSDataFlag lFlags;
                        
                        // rzhu, potential performance optimization: reuse offset
                        Int32 lOffset;
                        hr = hgetValueFromSlice2(lpSlice, pIndex, lpIndexObject, lOffsetInIndexObject, &lpData, &lcbWritten, &lType, &lFlags, &lOffset);
                        CHECK_HR;
                        lpvnSize->push_back(lcbWritten);
                        lpvResultInfo->push_back(lpData);
                        lpvResultDataFlags->push_back(lFlags);
                        if (!hasBreakBy && lMetricIndex == 0)
                        {
                            lnBreakByCount++;
                        }
                        else if (lFirstMetricWithBreakBy == lMetricIndex)
                        {
                            // jzeng 15/06/02 sync with iserver, the key retrieved from the header already contains the breakby info
                            lnBreakByCount++;
                            for (Int32 iUnit = 0; iUnit < nBreakbyUnits;iUnit++)
                                lpvpBreakByKeys->push_back(pIndex[ipBreakbyUnits[iUnit]]);
                        }
                        
                        if (lpSavedForecastInfo)
                        {
                            lpvForecastInfo->push_back(lpSavedForecastInfo + lOffset);
                        }
                    }
                }
                if (!hasBreakBy && lMetricIndex == 0)
                {
                    lnTotal += lnBreakByCount;
                }
                else if (lFirstMetricWithBreakBy == lMetricIndex)
                {
                    lnBreakBlock++;
                    lpvnBreakbyElements->push_back(lnBreakByCount);
                    lnTotal += lnBreakByCount;
                }
                
            }
        }
    }
    
    Int32* lpnBreakbyElements = NULL;
    Int32* lpFinalBreakByKeys = NULL;
    Int32 lnMetric = *onMetricIDs;
    Int32** lppnSize = new Int32*[lnMetric];
    CHECK_NEWED_PTR(lppnSize);
    unsigned char*** lppInfo = new unsigned char**[lnMetric];
    CHECK_NEWED_PTR(lppInfo);
    DSSDataFlag **lppFlags = new DSSDataFlag*[lnMetric];
    CHECK_NEWED_PTR(lppFlags);
    Int32* lpIncludeBreakBy = new Int32[lnMetric];
    CHECK_NEWED_PTR(lpIncludeBreakBy);
    ForecastInfo ***lppForecastInfo = new ForecastInfo**[lnMetric];
    CHECK_NEWED_PTR(lppForecastInfo);
    
    if (hasBreakBy)
    {
        lpnBreakbyElements = &(*lpvnBreakbyElements)[0];
        lpFinalBreakByKeys = &(*lpvpBreakByKeys)[0];
    }
    
    for (lMetricIndex = 0; lMetricIndex < lnMetric; lMetricIndex++)
    {
        if (hasBreakBy && includeBreakBy[lMetricIndex])
        {
            lpIncludeBreakBy[lMetricIndex] = true;
        }
        else
        {
            lpIncludeBreakBy[lMetricIndex] = false;
        }
        
        vector<Int32, MBase::Allocator<Int32> >* lpvnSize = lvpnSize[lMetricIndex];
        vector<unsigned char*, MBase::Allocator<unsigned char*> >* lpvResultInfo = lvpvResultInfo[lMetricIndex];
        vector<DSSDataFlag, MBase::Allocator<DSSDataFlag> >* lpvResultDataFlags = lvpvResultDataFlags[lMetricIndex];
        vector<ForecastInfo*, MBase::Allocator<ForecastInfo*> >* lpvForecastInfo = lvpvForecastInfos[lMetricIndex];
        Int32* lpnSize = NULL;
        unsigned char** lpInfo = NULL;
        DSSDataFlag* lpFlags = NULL;
        
        Int32 lnCount = lpvnSize->size();
        if (lnCount > 0)
        {
            lpnSize = new Int32[lnCount];
            CHECK_NEWED_PTR(lpnSize);
            memmove(lpnSize, &(*lpvnSize)[0], lnCount * sizeof(Int32));
            lpInfo = new unsigned char*[lnCount];
            CHECK_NEWED_PTR(lpInfo);
            memmove(lpInfo, &(*lpvResultInfo)[0], lnCount * sizeof(unsigned char*));
            lpFlags = new DSSDataFlag[lnCount];
            CHECK_NEWED_PTR(lpFlags);
            memmove(lpFlags, &(*lpvResultDataFlags)[0], lnCount * sizeof(DSSDataFlag));
        }
        lppnSize[lMetricIndex] = lpnSize;
        lppInfo[lMetricIndex] = lpInfo;
        lppFlags[lMetricIndex] = lpFlags;
        if (lpvForecastInfo != NULL && lpvForecastInfo->size() > 0)
            lppForecastInfo[lMetricIndex] = &(*lpvForecastInfo)[0];
        else
            lppForecastInfo[lMetricIndex] = NULL;
    }
    *opIsIncludeBreakBy = lpIncludeBreakBy;
    *oppnBreakbyElements = lpnBreakbyElements;
    *oppBreanbyKeys = lpFinalBreakByKeys;
    *oppnSize = lppnSize;
    *oppInfo = lppInfo;
    *oppDataFlags = lppFlags;
    *oppForecastInfo = (void***)lppForecastInfo;
    return S_OK;
}

DSSAttributeThresholdIterator* DSSXTabView::getAttributeThresholdIterator()
{
    if (!mpAttributeThresholdIterator) {
        mpAttributeThresholdIterator = new DSSAttributeThresholdIterator(this);
    }
    return mpAttributeThresholdIterator;
}

int DSSXTabView::hGetMetricAndSlice(EnumDSSRuntimeSubtotalType iSubtotalType,
                                    EnumDSSRuntimeSubtotalType iSubtotalLevel,
                                    Int32 *opMetricCount,
                                    vector<Int32>& orvMetricIDs,
                                    vector<EnumDSSDataType>& orvDataTypes,
                                    map<Int32, map<Int32, vector<Int32>, less<Int32> >, less<Int32> >& ormapMetricIDToSliceID/*metricID->(SubtotalType->sliceID)*/,
                                    map<Int32, map<Int32, vector<DSSModelInfo*>, less<Int32> >, less<Int32> > &ormapMetricIDToModelInfo)
{
    if (!opMetricCount)
        return E_POINTER;
    
    int hr = S_OK;
    
    *opMetricCount = 0;
    DSSTabularData *lTabularDataPtr = mpViewDataSet->getTabularData();
    CHECK_HR_PTR(lTabularDataPtr);
    
    map<Int32, map<Int32, vector<Int32>, less<Int32> >, less<Int32> >::iterator it;
    map<Int32, map<Int32, vector<DSSModelInfo*> >, less<Int32> >::iterator lModelIt;
    orvMetricIDs.resize(mnMetrics);
    orvDataTypes.resize(mnMetrics);
    
    Int32 nLevels;
    hr = mpSliceMap->CountLevels(&nLevels);
    Int32 lMetricCount = 0;
    
    for (Int32 iLevel = 0; iLevel < nLevels; iLevel++)
    {
        Int32 lnCount;
        hr = mpSliceMap->Count(iLevel, &lnCount, DssDisplayExtremeSlice);
        for (Int32 jTag = 0; jTag < lnCount; jTag++)
        {
            bool *AttrMap = NULL;
            Int32 lTag = 0;
            DSSModelInfo *lpModelInfo = NULL;
            hr = mpSliceMap->Item(iLevel, jTag, &AttrMap, &lTag, DssDisplayExtremeSlice, &lpModelInfo);
            CHECK_HR;
            
            for (Int32 iMetric = 0; iMetric < mnMetrics; iMetric++)
            {
                Int32 lSliceID = -1;
                EnumDSSRuntimeSubtotalType lExtremeType;
                hr = mpSliceMap->SearchID(AttrMap, lTag, DssDisplayExtremeSlice, iMetric, &lSliceID, &lExtremeType);
                
                if (lSliceID >= 0 && hr == S_OK)
                {
                    if ((lExtremeType & iSubtotalType) && (lExtremeType & iSubtotalLevel))
                    {
                        Int32 lMetric = iMetric;
                        if (lExtremeType & 0xF0000)
                        {
                            lMetric = -(lExtremeType & 0xFF0000);
                        }
                        it = ormapMetricIDToSliceID.find(lMetric);
                        if (it == ormapMetricIDToSliceID.end())
                        {
                            vector<Int32> lvSlices;
                            lvSlices.push_back(lSliceID);
                            map<Int32, vector<Int32>, less<Int32> > lMap;
                            lMap[lExtremeType] = lvSlices;
                            ormapMetricIDToSliceID[lMetric] = lMap;
                            orvMetricIDs[lMetricCount] = lMetric;
                            DSSCubeDataSlice* lpSlice = mpCube->getSliceByID(lSliceID);
                            CHECK_HR_PTR(lpSlice);
                            
                            orvDataTypes[lMetricCount] = lpSlice->getDataType();
                            lMetricCount++;
                        }
                        else
                        {
                            it->second[lExtremeType].push_back(lSliceID);
                        }
                        
                        if (lpModelInfo)
                        {
                            lModelIt = ormapMetricIDToModelInfo.find(lMetric);
                            if (lModelIt == ormapMetricIDToModelInfo.end())
                            {
                                vector<DSSModelInfo *> lvpModels;
                                lvpModels.push_back(lpModelInfo);
                                map<Int32, vector< DSSModelInfo* >, less<Int32> > lMap;
                                lMap[lExtremeType] = lvpModels;
                                ormapMetricIDToModelInfo[lMetric] = lMap;
                            }
                            else
                            {
                                lModelIt->second[lExtremeType].push_back(lpModelInfo);
                            }
                        }
                    }
                    
                }
            }
        }
    }
    
    *opMetricCount = lMetricCount;
    
    return S_OK;
}

int DSSXTabView::hgetValueFromSlice(Int32 iSliceID,
                                    DSSXTabKey * pIndex,
                                    DSSTabularIndex *ipIndexObject,
                                    Int32 iOffsetInIndexObject,
                                    EnumDSSRuntimeSubtotalType iExtremeType,
                                    ExtremeInfo *iopExtremeInfo)
{
    if (!iopExtremeInfo)
        return E_POINTER;
    
    unsigned char *lpData = NULL;
    Int32 lcbWritten;
    EnumDSSDataType lType;
    DSSDataFlag lFlags;
    //double lData;
    HRESULT hr = S_OK;
    DSSCubeDataSlice* lpSlice = NULL;
    //zhyang, TODO
    
    map<Int32, int, less<Int32> >::iterator it = mmapNewExtremes.find(iSliceID);
    if (it == mmapNewExtremes.end())
    {
        //zhyang, we will not go there.
        lpSlice = mpCube->getSliceByID(iSliceID);
        CHECK_HR_PTR(lpSlice);
    }
    else
    {
        //hr = it->second.GetSliceC(&lpSlice);
        lpSlice = mpCube->getSliceByID(it->second);
        CHECK_HR_PTR(lpSlice);
    }
    
    hr = hgetValueFromSlice2(lpSlice, pIndex, ipIndexObject, iOffsetInIndexObject, &lpData, &lcbWritten, &lType, &lFlags);
    CHECK_HR;
    
    if (lFlags != DssDataOk)
        iopExtremeInfo->mDataFlag = lFlags;
    
    if ((iExtremeType & DssRuntimeSubtotalExtremeMin))
    {
        iopExtremeInfo->mpMin = lpData;
        iopExtremeInfo->mMinSize = lcbWritten;
    }
    else
    {
        iopExtremeInfo->mpMax = lpData;
        iopExtremeInfo->mMaxSize = lcbWritten;
    }
    
    return S_OK;
}

int DSSXTabView::hgetValueFromSlice2(DSSCubeDataSlice* ipSlice,
                                     DSSXTabKey * pIndex,
                                     DSSTabularIndex *ipIndexObject,
                                     Int32 iOffsetInIndexObject,
                                     unsigned char **ppData,
                                     Int32 *pcbWritten,
                                     EnumDSSDataType *pDataType,
                                     DSSDataFlag *pFlags,
                                     Int32 *opOffset)
{
    // Shall we validate the keys?
    int hr = S_OK;
    
    DSSTabularIndex *lpSliceIndices = ipSlice->getIndices();
    CHECK_HR_PTR(lpSliceIndices);
    
    if (ipIndexObject && lpSliceIndices->IsSameOrder(ipIndexObject))
    {
        hr = ipSlice->getValByOffset(iOffsetInIndexObject, (const char **)ppData, *pcbWritten, *pDataType, *pFlags, NULL);
        if (opOffset)
            *opOffset = iOffsetInIndexObject;
    }
    else
    {
        int lOffset = -1;
        hr = lpSliceIndices->LookUp(mnTotalUnits, pIndex, &lOffset);
        CHECK_HR;
        
        if (opOffset)
        {
            *opOffset = lOffset;
        }
        
        hr = ipSlice->getValByOffset(lOffset,(const char **)ppData, *pcbWritten, *pDataType, *pFlags, NULL);
    }
    
    if (hr != S_OK)
        *pFlags = DssDataInvalidCell;
    
    return S_OK;
    
}

/*
 int DSSXTabView::GetTrendModelCube(DSSCube **opCube)
 {
 int hr = S_OK;
 if (!mTrendModelCubePtr)
 {
 DSSCube* lpTrendModelCube = new DSSCube;
 CHECK_NEWED_PTR(lpTrendModelCube);
 
 mTrendModelCubePtr = lpTrendModelCube;
 }
 
 *opCube = mTrendModelCubePtr;
 
 return hr;
 }
 
 int DSSXTabView::ResetTrendModelInfo()
 {
 mmapIncludeBreakBy.clear();
 mmapElementKeys.clear();
 mmapLabels.clear();
 mmapNewExtremes.clear();
 mmapForecastInfos.clear();
 if (mTrendModelCubePtr)
 {
 delete mTrendModelCubePtr;
 mTrendModelCubePtr = NULL;
 }
 return S_OK;
 }
 
 int DSSXTabView::GetBreakByMap(map<Int32, bool, less<Int32> > * &opmapInclueBreakBy)
 {
 opmapInclueBreakBy = &mmapIncludeBreakBy;
 return S_OK;
 }
 int DSSXTabView::GetIndexValueToElementKeyMap(map<Int32, map<double, Int32, less<double>, MBase::Allocator<pair<double const, Int32> > >*, less<Int32> > * &opmapElementKeys)
 {
 opmapElementKeys = &mmapElementKeys;
 return S_OK;
 }
 
 int DSSXTabView::GetIndexValueToLabelsMap(map<Int32, map<double, BSTR, less<double>, MBase::Allocator<pair<double const, BSTR> > >*, less<Int32> > * &opmapLabels)
 {
 opmapLabels = &mmapLabels;
 return S_OK;
 }
 
 int DSSXTabView::GetNewExtremeMap(map<Int32, DSSSliceID, less<Int32> > * &opmapNewExtremes)
 {
 opmapNewExtremes = &mmapNewExtremes;
 return S_OK;
 }
 
 int DSSXTabView::GetForecastInfoMap(map<Int32, ForecastInfo*, less<Int32> > * &opmapForecastInfos)
 {
 opmapForecastInfos = &mmapForecastInfos;
 return S_OK;
 }*/

// jzeng 15/06/02 DE13877, sync with iserver, don't return breakby key, which can be retrieved from the header
int DSSXTabView::hGetRowAndColWithBreakBy(Int32 iRowStart, Int32 iRowEnd,
                                          Int32 iColStart, Int32 iColEnd,
                                          Int32 nBreakbyUnits, Int32 *ipBreakbyUnits,
                                          Int32* opnRows, Int32** oppRows,
                                          Int32 *opnCols, Int32** oppCols,
                                          Int32 *opBreakByAxis)
{
    if (iRowStart > iRowEnd || iColStart > iColEnd)
        return E_FAIL;
    
    HRESULT hr = S_OK;
    
    Int32 lBreakByAxis = -1;
    for(Int32 iAxis = 0; iAxis < 2; iAxis++)
    {
        Int32 lnHeaderUnits = mHeaders[iAxis]->getNumUnits();
        CHECK_HR;
        vector<Int32> lvHeaderUnits(lnHeaderUnits);
        if(lnHeaderUnits > 0)
        {
            hr = mHeaders[iAxis]->getUnits(lnHeaderUnits, &lvHeaderUnits[0]);
            CHECK_HR;
        }
        
        bool lbFound;
        for(Int32 iUnit2 = 0; iUnit2 < nBreakbyUnits; iUnit2++)
        {
            lbFound = false;
            for (Int32 iUnit = 0; iUnit < lnHeaderUnits; iUnit++)
            {
                if (lvHeaderUnits[iUnit] == ipBreakbyUnits[iUnit2])
                {
                    lbFound = true;
                    break;
                }
            }
            if (!lbFound)
                break;
        }
        if (lbFound)
        {
            lBreakByAxis = iAxis;
            break;
        }
    }
    
    if (lBreakByAxis < 0)
        return E_FAIL;
    
    Int32 i, j;
    Int32 lRowStart = -1;
    Int32 lRowEnd = -1;
    *opBreakByAxis = lBreakByAxis;
    if (lBreakByAxis == 0 && iRowEnd > iRowStart)
    {
        lRowStart =  iRowStart;
        lRowEnd = iRowEnd;
    }
    else if (lBreakByAxis == 1 && iColEnd > iColStart)
    {
        lRowStart =  iColStart;
        lRowEnd = iColEnd;
    }
    
    if (lRowStart > -1)
    {
        DSSTabularData * lTabularData = mpViewDataSet->getTabularData();
        CHECK_HR_PTR(lTabularData);
        
        DSSTabularIndexPool * lIndicesPool = NULL;
        hr = mpCube -> getIndexPool(&lIndicesPool);
        CHECK_HR_PTR(lIndicesPool);
        
        DSSTabularIndex* lpBreakbyIndices = NULL;
        hr = lIndicesPool->getIndex(nBreakbyUnits, ipBreakbyUnits, &lpBreakbyIndices);
        CHECK_HR;
        
        Int32 lnRow = lpBreakbyIndices->Count();
        
        DSSXTabKey pIndex[mnTotalUnits];
        Int32 lMetric = 0 ;
        Int32 lRow = -1;
        
        DSSXTabBitVector lBV;
        lBV.Init(lnRow, false);
        vector<Int32> lvRows;
        lvRows.reserve(lRowStart - lRowStart);
        
        for (i = lRowStart; i < lRowEnd; i++)
        {
            hr = mHeaders[lBreakByAxis]->FillIndex(i, mnTotalUnits, pIndex, &lMetric);
            CHECK_HR;
            
            hr = lpBreakbyIndices->LookUp(mnTotalUnits, pIndex, &lRow);
            CHECK_HR;
            
            if (!lBV.Get(lRow))
            {
                lvRows.push_back(i);
                lBV.Set(lRow, true);
            }
        }
        
        Int32 lpPosition[nBreakbyUnits];
        Int32 lpIndexUnits[nBreakbyUnits];
        hr = lpBreakbyIndices->getUnits(nBreakbyUnits, lpIndexUnits);
        CHECK_HR;
        for(i = 0; i < nBreakbyUnits; i ++)
        {
            for (j = 0; j < nBreakbyUnits; j++)
            {
                if (ipBreakbyUnits[i] == lpIndexUnits[j])
                {
                    lpPosition[i] = j;
                    break;
                }
            }
        }
        
        Int32 lnRows = lvRows.size();
        Int32 *lpRows = new Int32[lnRows];
        CHECK_NEWED_PTR(lpRows);
        memmove(lpRows, &lvRows[0], sizeof(Int32) * lnRows);
        
        if (lBreakByAxis == 0)
        {
            *opnRows = lnRows;
            *oppRows = lpRows;
            
            if (iColEnd > iColStart)
            {
                Int32 *lpRows2 = new Int32[iColEnd - iColStart];
                CHECK_NEWED_PTR(lpRows2);
                for (i = iColStart, j = 0; i< iColEnd; i++, j++)
                {
                    lpRows2[j] = i;
                }
                *opnCols = iColEnd - iColStart;
                *oppCols = lpRows2;
            }
            else
            {
                Int32 *lpRows2 = new Int32[1];
                CHECK_NEWED_PTR(lpRows2);
                *opnCols = 1;
                *oppCols = lpRows2;
            }
        }
        else
        {
            *opnCols = lnRows;
            *oppCols = lpRows;
            
            if (iRowEnd > iRowStart)
            {
                Int32 *lpRows2 = new Int32[iRowEnd - iRowStart];
                CHECK_NEWED_PTR(lpRows2);
                for (i = iRowStart, j = 0; i< iRowEnd; i++, j++)
                {
                    lpRows2[j] = i;
                }
                *opnRows = iRowEnd - iRowStart;
                *oppRows = lpRows2;
            }
            else
            {
                Int32 *lpRows2 = new Int32[1];
                CHECK_NEWED_PTR(lpRows2);
                *opnRows = 1;
                *oppRows = lpRows2;
            }
        }
    }
    else
    {
        if (iRowEnd > iRowStart)
        {
            Int32 *lpRows2 = new Int32[iRowEnd - iRowStart];
            CHECK_NEWED_PTR(lpRows2);
            for (i = iRowStart, j = 0; i< iRowEnd; i++, j++)
            {
                lpRows2[j] = i;
            }
            *opnRows = iRowEnd - iRowStart;
            *oppRows = lpRows2;
        }
        else
        {
            Int32 *lpRows2 = new Int32[1];
            CHECK_NEWED_PTR(lpRows2);
            *opnRows = 1;
            *oppRows = lpRows2;
        }
        if (iColEnd > iColStart)
        {
            Int32 *lpRows2 = new Int32[iColEnd - iColStart];
            CHECK_NEWED_PTR(lpRows2);
            for (i = iColStart, j = 0; i< iColEnd; i++, j++)
            {
                lpRows2[j] = i;
            }
            *opnCols = iColEnd - iColStart;
            *oppCols = lpRows2;
        }
        else
        {
            Int32 *lpRows2 = new Int32[1];
            CHECK_NEWED_PTR(lpRows2);
            *opnCols = 1;
            *oppCols = lpRows2;
        }
    }
    
    return S_OK;
}

