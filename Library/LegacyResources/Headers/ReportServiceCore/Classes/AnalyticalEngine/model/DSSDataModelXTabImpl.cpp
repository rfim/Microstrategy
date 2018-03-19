/*
 *  DSSDataModelXTabImpl.cpp
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import <set>

#import "DSSDataModelXTabImpl.h"
#import "DSSXTabView.h"
#import "DSSCube.h"
#import "DSSTabularAttribute.h"
#import "DSSTabularMetrics.h"
#import "DSSTabularConsolidation.h"
#import "DSSViewMap.h"
#import "DSSSortConverter.h"
#import "DSSSortDefinition.h"
#import "DSSSorter.h"
#import "DSSTemplate.h"
#import "DSSXTabHeader.h"
#import "DSSCubeDataSlice.h"
#import "DSSDataColumn.h"
#import "DSSDataSource.h"
#import "CalculationEngine/DSSCalculationPlan.h"
#import "CalculationEngine/DSSProcessMetrics.h"
#import "DSSDataModelTransaction.h"
#import "DSSRWTemplateNode.h"
#import "DSSTabularIndex.h"
#import "DSSMetricDefaultSorter.h"
#import "DSSSortDefinition.h"
#import "DSSMetricThresholds.h"
#import "DSSFunctionServer.h"
#import "CSITransaction/DFCCSITransactionBase.h"
#include <algorithm>

DSSDataModelXTabImpl::DSSDataModelXTabImpl():mpView(NULL), mpCube(NULL), mpMetricsUnit(NULL)
,mvRowSorts(NULL), mvColumnSorts(NULL)
,mbIsNullOnTop(false), mbMetricsMoved(false), mnSlices(0)
,mpDummyUnit(NULL), mMetricUnitID(-1), mnTotals(0), mTotalPosition(DssSubtotalsPositionReserved)
,mpTemplate(NULL), mpDataSource(NULL)//, mnLoadedSlices(0)
,mpHeaderSource(NULL),mbLoadHeaderBySlice(false)
,mpRowList(NULL),mpColList(NULL), mpRowWindowOffset(NULL),mpColWindowOffset(NULL)
,mnRowList(0),mnColList(0), mnRowWindowOffset(0),mnColWindowOffset(0)
,mpCalculationPlan(NULL)
,mpRowListPage(NULL),mpRowWindowPage(NULL),mpColListPage(NULL),mpColWindowPage(NULL)
,mRowListPageStart(0),mColListPageStart(0),mRowWindowPageStart(0),mColWindowPageStart(0)
,mRowListPageOffset(0),mRowWindowPageOffset(0),mColListPageOffset(0),mColWindowPageOffset(0)
,mbRowMapped(false),mbColMapped(false),mFilePath(""),mbDataSourceMapped(false)
{
    mDuplicatedRows.resize(2);
}

DSSDataModelXTabImpl::~DSSDataModelXTabImpl()
{
	hCleanUp();
}

void DSSDataModelXTabImpl::hCleanUp()
{
	if (mpView)
	{
		delete mpView;
		mpView = NULL;
	}
	
	if (mpCube)
	{
		delete mpCube;
		mpCube = NULL;
	}
	
	deleteSorts();
	
	if (mpDataSource)
	{
		delete mpDataSource;
		mpDataSource = NULL;
	}
    
    if(mpHeaderSource)
    {
        delete mpHeaderSource;
        mpHeaderSource = NULL;
    }
    
	if (mpCalculationPlan)
	{
		delete mpCalculationPlan;
		mpCalculationPlan = NULL;
	}
	
	mvGBUnits.clear();
	
    //clear the row list
    clearRowList(0);
    clearRowList(1);
    
	mnSlices = 0;
    
	mvTotalInfo.clear();
	mvDataNullDisplay.clear();
	mvElementNullDisplay.clear();
	mvCellFmtIDs.clear();
	mvMaxDataOffset.clear();
	mvMinDataOffset.clear();
    for(int i = 0; i < mDuplicatedRows.size();i++)
        mDuplicatedRows[i].clear();
    mFilePath = "";
}

int DSSDataModelXTabImpl::Init()
{
	mType = DssModelXTab;
	return 0;
}

int DSSDataModelXTabImpl::CreateXTabView()
{
	if (mpView != NULL)
		return E_FAIL;
	
	//TODO: make sure when xtab view is created, all units have been loaded
	mpView = new DSSXTabView();
	mpView->Init(this);
	return 0;
}

DSSXTabView* &DSSDataModelXTabImpl::getXTabView()
{
	if (mpView == NULL)
		CreateXTabView();
	
	return mpView;
}

DSSCube* &DSSDataModelXTabImpl::getCube()
{
	if (!mpCube)
	{
		mpCube = new DSSCube();
		mpCube->Init(hGetTabularData());
	}
	
	return mpCube;
}

int DSSDataModelXTabImpl::setNullOnTop(bool &irVal)
{
	mbIsNullOnTop = irVal;
	return 0;
}

int DSSDataModelXTabImpl::setNullReplacement(MBase::CString &irVal)
{
	mNullReplacement = irVal;
	return 0;
}

int DSSDataModelXTabImpl::getNullReplacement(MBase::CString &orReplacement)
{
	orReplacement = mNullReplacement;
	return 0;
}

int DSSDataModelXTabImpl::setDataNullDisplay(vector<MBase::CString> &irvVal)
{
	AE_ASSERT(irvVal.size() == 5 || irvVal.empty());	//460710, empty grid
	mvDataNullDisplay.assign(irvVal.begin(), irvVal.end());
	return 0;
}

// jimzhang nov/03/2011 TQMS 510040 change parameter's type from "char" to "DSSDataFlag"
int hConvertFlag(/*char*/DSSDataFlag iNullFlag)
{
	switch (iNullFlag) {
		case DssDataNull:		//warehouse null
			return 0;
		case DssDataInvalidCell://xtab null, caused by cross tabular (data cell "hole")
			return 1;
		case DssDataOverflow:
		case DssDataUnderflow:
		case DssDataZerodivide:
		case DssDataInvalid:	//xtab null, or calculation null
			return 2;
		case DssDataUnCalculated://aggregation null
			return 3;
		case DssDataMissing:	//data missing
			return 4;
		default:
			return 2;
	}
}

// jimzhang nov/03/2011 TQMS 510040 change type of first parameter from "char" to "DSSDataFlag"
int DSSDataModelXTabImpl::getDataNullDisplay(/*char*/DSSDataFlag iNullFlag, MBase::CString &orVal)
{
	int index = hConvertFlag(iNullFlag);
	orVal = mvDataNullDisplay[index];
	return S_OK;
}

int DSSDataModelXTabImpl::setElementNullDisplay(vector<MBase::CString> &irvVal)
{
	AE_ASSERT(irvVal.size() == 5 || irvVal.empty());	//460710, empty grid
	mvElementNullDisplay.assign(irvVal.begin(), irvVal.end());
	return 0;
}

int DSSDataModelXTabImpl::getElementNullDisplay(DSSDataFlag iNullFlag, MBase::CString &orVal)
{
	int index = hConvertFlag(iNullFlag);
	orVal = mvElementNullDisplay[index];
	return S_OK;
}

int DSSDataModelXTabImpl::setSubtotalCount(int count)
{
    /*	if (mvTotalInfo.size() > 0)		//should not set the size multiple times
     return E_FAIL;
     
     mvTotalInfo.resize(count);
     mnTotals = count;*/
	return 0;
}

int DSSDataModelXTabImpl::setSubtotalInfo(int index, int iTotalID, MBase::CString &irTotalGUID, vector<int> &irvDataTypes, MBase::CString &irTotalName, int iTotalType)
{
	//hr = ipModel->setSubtotalInfo(0, id, lguid, irTotalDatatype[i], lName, lType);
    //	if (index < 0 || index >= mnTotals)
    //		return E_INVALIDINDEX;
	if (index >= mvTotalInfo.size())
	{
		int count = mvTotalInfo.size() - 1;
		for (; count < index; count++)
		{
			mvTotalInfo.push_back(SubtotalInformation());
		}
		mnTotals = mvTotalInfo.size();
	}
	
	SubtotalInformation *lpTotal = &mvTotalInfo[index];
	lpTotal->toalID = iTotalID;
	lpTotal->mGuid = irTotalGUID;
	lpTotal->name = irTotalName;
	lpTotal->type = (EnumDSSMetricType)iTotalType;
	
	lpTotal->dataTypes.reserve(irvDataTypes.size());
	for (int i = 0; i < irvDataTypes.size(); i++)
		lpTotal->dataTypes.push_back((EnumDSSDataType)irvDataTypes[i]);
	return 0;
}

int DSSDataModelXTabImpl::getSubtotalName(int iTotalIndex, MBase::CString &orName)
{
	iTotalIndex--;	//iTotalIndex is 1-based
	
	if (iTotalIndex < 0 || iTotalIndex >= mnTotals)
		return E_INVALIDINDEX;
	orName = mvTotalInfo[iTotalIndex].name;
	return 0;
}

MBase::CString DSSDataModelXTabImpl::getSubtotalName(int iTotalIndex)
{
	if (iTotalIndex < 0 || iTotalIndex >= mnTotals)
		return NULL;
	return mvTotalInfo[iTotalIndex].name;
}

const char *DSSDataModelXTabImpl::getSubtotalNameAndLength(int iTotalIndex, int &orLength)
{
	if (iTotalIndex < 0 || iTotalIndex >= mnTotals)
		return NULL;
	
	orLength = mvTotalInfo[iTotalIndex].name.length();
	return mvTotalInfo[iTotalIndex].name.c_str();
}

EnumDSSMetricType DSSDataModelXTabImpl::getSubtotalType(int iTotalIndex, GUID iGUID)
{
	if (iTotalIndex >= 0 && iTotalIndex < mnTotals)
		return mvTotalInfo[iTotalIndex].type;
	else
		return DssMetricReserved;
}

int DSSDataModelXTabImpl::setMetricUnit(int index)
{
	mMetricUnitID = index;
	//TODO[enh]: when index is matched to mvUnit
	for (int i = 0; i < mvUnits.size(); i++)
	{
		EnumDSSTemplateUnitType lUnitType = mvUnits[i]->getUnitType();		
		if (lUnitType == DssTemplateMetrics)
			mpMetricsUnit = (DSSTabularMetrics*)mvUnits[i];		
	}
	return 0;
}

int DSSDataModelXTabImpl::CreateViewMap(int nSlices, vector<int> &irvGBUnits, vector<int> &irvGBPaths,
                                        vector<int>& irvRowWindowSize, vector<int>& irvRows,
                                        vector<int>& irvColWindowSize, vector<int>& irvCols)
{
	if (nSlices * irvGBUnits.size() != irvGBPaths.size() || irvRowWindowSize.size () < nSlices || irvColWindowSize.size() < nSlices)
		return E_INVALIDARG;
    
	mvGBUnits.assign(irvGBUnits.begin(), irvGBUnits.end());
	
	int hr = S_OK;
	hr = hCreateViewMap(irvGBUnits.size(), &irvGBUnits[0], nSlices, &irvGBPaths[0]);
	CHECK_HR;
	
	mnSlices = nSlices;
    if(mpRowWindowOffset)
    {
        delete mpRowWindowOffset;
        mpRowWindowOffset = NULL;
    }
    mnRowWindowOffset = irvRowWindowSize.size();
    mpRowWindowOffset = new int[mnRowWindowOffset];
    memmove(mpRowWindowOffset, &irvRowWindowSize[0], sizeof(int) * mnRowWindowOffset);
    if(mpColWindowOffset)
    {
        delete mpColWindowOffset;
        mpColWindowOffset = NULL;
    }
    mnColWindowOffset = irvColWindowSize.size();
    mpColWindowOffset = new int[mnColWindowOffset];
    memmove(mpColWindowOffset, &irvColWindowSize[0], sizeof(int) * mnColWindowOffset);
    
    if(mpRowList)
    {
        //        int lDataSize = mnRowList * sizeof(int);
        delete mpRowList;
        //MEM_GOVERNOR_RELEASE(lDataSize, lDataSize)
        mpRowList = NULL;
    }
    mnRowList = irvRows.size();
    //    int lReservedSize = mnRowList * sizeof(int);
    if(mnRowList > 0)
    {
        //MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)		
        mpRowList = new int[mnRowList];
        //MEM_GOVERNOR_COMMIT(lReservedSize)
        memmove(mpRowList, &irvRows[0], sizeof(int) * mnRowList);
    }
    if(mpColList)
    {
        //        int lDataSize = mnColList * sizeof(int);
        delete mpColList;
        //MEM_GOVERNOR_RELEASE(lDataSize, lDataSize)
        mpColList = NULL;
    }
    mnColList = irvCols.size();
    //    lReservedSize = mnColList * sizeof(int);
    if(mnColList > 0)
    {
        //MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)		
        mpColList = new int[mnColList];
        //MEM_GOVERNOR_COMMIT(lReservedSize)
        memmove(mpColList, &irvCols[0], sizeof(int) * mnColList);
    }
	//NOTICE: for non-sliced view
	//in these cases some rows and columns are shared by different slices
	//TODO: here we assume the display rows of the slices always appear in ascending order, is this assumption the truth?
    if(nSlices > 0)
    {
        if(mpRowWindowOffset[nSlices] != mnRowList && mnRowList != 0)
        {
            int* temp = mpRowList;
            int nRepeated = mpRowWindowOffset[nSlices] / mnRowList;
            //            int lReservedSize = mnRowList * nRepeated * sizeof(int);
            //MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)		
            mpRowList = new int[mnRowList * nRepeated];
            //MEM_GOVERNOR_COMMIT(lReservedSize)
            
            for(int i = 0; i < nRepeated; i++)
            {
                memmove(mpRowList + i * mnRowList, temp, sizeof(int) * mnRowList);
            }
            //           int lDataSize = mnRowList * sizeof(int);
            mnRowList *= nRepeated;
            delete temp;
            //MEM_GOVERNOR_RELEASE(lDataSize, lDataSize)
        }
        
        if(mpColWindowOffset[nSlices] != mnColList && mnColList != 0)
        {
            int* temp = mpColList;
            int nRepeated = mpColWindowOffset[nSlices] / mnColList;
            //           int lReservedSize = mnColList * nRepeated * sizeof(int);
            //MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)		
            mpColList = new int[mnColList * nRepeated];
            //MEM_GOVERNOR_COMMIT(lReservedSize)
            
            for(int i = 0; i < nRepeated; i++)
            {
                memmove(mpColList + i * mnColList, temp, sizeof(int) * mnColList);
            }
            //           int lDataSize = mnColList * sizeof(int);
            mnColList *= nRepeated;
            delete temp;
            //MEM_GOVERNOR_RELEASE(lDataSize, lDataSize)
        }
    }
	
	DSSXTabHeader *lpRowHeader = mpView->getHeader(0);
	DSSXTabHeader *lpColHeader = mpView->getHeader(1);
	if (lpRowHeader && lpColHeader)
	{
		//424514, 07/30/2010, if xtabview only contains runtime units, it should be treated as empty.
		if (lpRowHeader->getNumRuntimeUnits() == lpRowHeader->getNumUnits()
			&& lpColHeader->getNumRuntimeUnits() == lpColHeader->getNumUnits())
		{
			memset(mpRowWindowOffset, 0, sizeof(int) * mnRowWindowOffset);
			memset(mpColWindowOffset, 0, sizeof(int) * mnColWindowOffset);
		}
	}
	
    //	mvbIsSliceLoaded.resize(nSlices);
    //	for (int i = 0; i < nSlices; i++)
    //		mvbIsSliceLoaded[i] = false;
	
	return 0;
}

int DSSDataModelXTabImpl::CreateViewMap2(int nSlices, vector<int> &irvGBUnits, vector<int> &irvGBPaths,vector<int*>& rvList, vector<int>& rvnList, vector<int*> &rvOffset, vector<int>& rvnOffset)
{
    int hr = S_OK;
    mnSlices = nSlices;
    if( rvList.size() == 0)//empty grid
    {
        hr = hCreateViewMap(0, NULL, nSlices, NULL);
        CHECK_HR;
        
        return hr;
    }
	if (nSlices * irvGBUnits.size() != irvGBPaths.size() || rvnOffset[0] < nSlices || rvnOffset[1] < nSlices)
		return E_INVALIDARG;
    
	mvGBUnits.assign(irvGBUnits.begin(), irvGBUnits.end());
	
    
	hr = hCreateViewMap(irvGBUnits.size(), &irvGBUnits[0], nSlices, &irvGBPaths[0]);
	CHECK_HR;
#ifndef __ANDROID__
    hr = hCreateMappedFile();
    CHECK_HR;
#endif
    mnRowWindowOffset = rvnOffset[0];
    mpRowWindowOffset = rvOffset[0];
    
    mnColWindowOffset = rvnOffset[1];
    mpColWindowOffset = rvOffset[1];
    
    mnRowList = rvnList[0];		
    mpRowList = rvList[0];
    
    mnColList = rvnList[1];
    mpColList = rvList[1];
	//NOTICE: for non-sliced view
	//in these cases some rows and columns are shared by different slices
	//TODO: here we assume the display rows of the slices always appear in ascending order, is this assumption the truth?
    if(nSlices > 0)
    {
        if(mpRowWindowOffset[nSlices] != mnRowList && mnRowList != 0)
        {
            int* temp = mpRowList;
            int nRepeated = mpRowWindowOffset[nSlices] / mnRowList;
            //            int lReservedSize = mnRowList * nRepeated * sizeof(int);
            //MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)		
            mpRowList = new int[mnRowList * nRepeated];
            //MEM_GOVERNOR_COMMIT(lReservedSize)
            
            for(int i = 0; i < nRepeated; i++)
            {
                memmove(mpRowList + i * mnRowList, temp, sizeof(int) * mnRowList);
            }
            //            int lDataSize = mnRowList * sizeof(int);
            mnRowList *= nRepeated;
            delete temp;
            //MEM_GOVERNOR_RELEASE(lDataSize, lDataSize)
        }
        
        if(mpColWindowOffset[nSlices] != mnColList && mnColList != 0)
        {
            int* temp = mpColList;
            int nRepeated = mpColWindowOffset[nSlices] / mnColList;
            //           int lReservedSize = mnColList * nRepeated * sizeof(int);
            //MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)		
            mpColList = new int[mnColList * nRepeated];
            //MEM_GOVERNOR_COMMIT(lReservedSize)
            
            for(int i = 0; i < nRepeated; i++)
            {
                memmove(mpColList + i * mnColList, temp, sizeof(int) * mnColList);
            }
            //            int lDataSize = mnColList * sizeof(int);
            mnColList *= nRepeated;
            //            MEM_GOVERNOR_RELEASE(lDataSize, lDataSize)
            delete temp;
        }
    }
	
	DSSXTabHeader *lpRowHeader = mpView->getHeader(0);
	DSSXTabHeader *lpColHeader = mpView->getHeader(1);
	if (lpRowHeader && lpColHeader)
	{
		//424514, 07/30/2010, if xtabview only contains runtime units, it should be treated as empty.
		if (lpRowHeader->getNumRuntimeUnits() == lpRowHeader->getNumUnits()
			&& lpColHeader->getNumRuntimeUnits() == lpColHeader->getNumUnits())
		{
			memset(mpRowWindowOffset, 0, sizeof(int) * mnRowWindowOffset);
			memset(mpColWindowOffset, 0, sizeof(int) * mnColWindowOffset);
		}
	}
	
    //	mvbIsSliceLoaded.resize(nSlices);
    //	for (int i = 0; i < nSlices; i++)
    //		mvbIsSliceLoaded[i] = false;
	
	return 0;
}
int DSSDataModelXTabImpl::CreateViewMap3(int nSlices, vector<int> &irvGBUnits, int* pGBPath, char* pGBPage, int nGBPageSize, int GBPagePosInFile, vector<int*>& rvList, vector<int>& rvnList, vector<int*> &rvOffset, vector<int>& rvnOffset)
{
    int hr = S_OK;
    mnSlices = nSlices;
    if( rvList.size() == 0)//empty grid
    {
        hr = hCreateViewMap(0, NULL, nSlices, NULL);
        CHECK_HR;
        
        return hr;
    }    
	mvGBUnits.assign(irvGBUnits.begin(), irvGBUnits.end());
	
	hr = hCreateViewMap(irvGBUnits.size(), &irvGBUnits[0], nSlices, pGBPath, mFilePath, pGBPage, nGBPageSize, GBPagePosInFile);
	CHECK_HR;
#ifndef __ANDROID__
    //TODO: CHECK SECURITY MODE
    if(DataSourceUtil::isMappedFileSupported())
    {
        hr = hCreateMappedFile();
        CHECK_HR;
    }
#endif
    mnRowWindowOffset = rvnOffset[0];
    mpRowWindowOffset = rvOffset[0];
    
    mnColWindowOffset = rvnOffset[1];
    mpColWindowOffset = rvOffset[1];
    
    mnRowList = rvnList[0];	
    mpRowList = rvList[0];
    mnColList = rvnList[1];
    mpColList = rvList[1];
	//NOTICE: for non-sliced view
	//in these cases some rows and columns are shared by different slices
	//TODO: here we assume the display rows of the slices always appear in ascending order, is this assumption the truth?
    if(nSlices > 0)
    {
        if(mpRowWindowOffset[nSlices] != mnRowList && mnRowList != 0)
        {
            int nRepeated = mpRowWindowOffset[nSlices] / mnRowList;
            
            //            int lReservedSize = mnRowList * nRepeated * sizeof(int) + mnRowWindowOffset * sizeof(int);
            //MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)	
            int* lpTempRowWindowOffset = new int[mnRowWindowOffset];
            int* lpTempRowList = new int[mnRowList * nRepeated];
            //MEM_GOVERNOR_COMMIT(lReservedSize)
            int lnRowList = mnRowList;
            int lnRowWindowOffset = mnRowWindowOffset;
            memmove(lpTempRowWindowOffset, mpRowWindowOffset, mnRowWindowOffset * sizeof(int));   
            
            for(int i = 0; i < nRepeated; i++)
            {
                memmove(lpTempRowList + i * lnRowList, mpRowList, sizeof(int) * mnRowList);
            }
            lnRowList *= nRepeated;
            
            clearRowList(0);
            
            mnRowList = lnRowList;
            mnRowWindowOffset = lnRowWindowOffset;
            mpRowWindowOffset = lpTempRowWindowOffset;
            mpRowList = lpTempRowList;
            mbRowMapped = false;
        }
        
        if(mpColWindowOffset[nSlices] != mnColList && mnColList != 0)
        {
            int nRepeated = mpColWindowOffset[nSlices] / mnColList;
            
            //            int lReservedSize = mnColList * nRepeated * sizeof(int) + mnColWindowOffset * sizeof(int);
            //MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)	
            int* lpTempColWindowOffset = new int[mnColWindowOffset];
            int* lpTempColList = new int[mnColList * nRepeated];
            //MEM_GOVERNOR_COMMIT(lReservedSize)
            int lnColList = mnColList;
            int lnColWindowOffset = mnColWindowOffset;
            memmove(lpTempColWindowOffset, mpColWindowOffset, mnColWindowOffset * sizeof(int));   
            
            for(int i = 0; i < nRepeated; i++)
            {
                memmove(lpTempColList + i * lnColList, mpColList, sizeof(int) * mnColList);
            }
            lnColList *= nRepeated;
            
            clearRowList(1);
            
            mnColList = lnColList;
            mnColWindowOffset = lnColWindowOffset;
            mpColWindowOffset = lpTempColWindowOffset;
            mpColList = lpTempColList;
            mbColMapped = false;
        }
    }
	
	DSSXTabHeader *lpRowHeader = mpView->getHeader(0);
	DSSXTabHeader *lpColHeader = mpView->getHeader(1);
	if (lpRowHeader && lpColHeader)
	{
		//424514, 07/30/2010, if xtabview only contains runtime units, it should be treated as empty.
		if (lpRowHeader->getNumRuntimeUnits() == lpRowHeader->getNumUnits()
			&& lpColHeader->getNumRuntimeUnits() == lpColHeader->getNumUnits())
		{
            //         int lReservedSize = (mnRowList + mnColList + mnRowWindowOffset + mnColWindowOffset) * sizeof(int);
            //MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)
            int* lpTempRowWindowOffset = mnRowWindowOffset > 0 ? new int[mnRowWindowOffset] : NULL;
            int* lpTempRowList = mnRowList > 0 ? new int[mnRowList] : NULL;
            int* lpTempColWindowOffset = mnColWindowOffset > 0 ? new int[mnColWindowOffset] : NULL;
            int* lpTempColList = mnColList > 0 ? new int[mnColList] : NULL;
            //MEM_GOVERNOR_COMMIT(lReservedSize)
            if(mnRowList > 0)
                memmove(lpTempRowList, mpRowList, sizeof(int) * mnRowList);
            if(mnColList > 0)
                memmove(lpTempColList, mpColList, sizeof(int) * mnColList);
			if(mnRowWindowOffset > 0)
                memset(lpTempRowWindowOffset, 0, sizeof(int) * mnRowWindowOffset);
			if(mnColWindowOffset > 0)
                memset(lpTempColWindowOffset, 0, sizeof(int) * mnColWindowOffset);
            int lnRowWindowOffset = mnRowWindowOffset;
            int lnRowList = mnRowList;
            int lnColWindowOffset = mnColWindowOffset;
            int lnColList = mnColList;
            clearRowList(0);
            clearRowList(1);
            mpRowWindowOffset = lpTempRowWindowOffset;
            mpColWindowOffset = lpTempColWindowOffset;
            mpRowList = lpTempRowList;
            mpColList = lpTempColList;
            mnRowList = lnRowList;
            mnColList = lnColList;
            mnRowWindowOffset = lnRowWindowOffset;
            mnColWindowOffset = lnColWindowOffset;
            mbRowMapped = false;
            mbColMapped = false;
		}
	}
    
	return 0;
}

int DSSDataModelXTabImpl::getRowRange(int iSlice, const int *&orpRows, int &ornRows)
{
	if (iSlice < 0 || iSlice >= mnSlices)
		return E_INVALIDINDEX;
	
	orpRows = mpRowList ? mpRowList + mpRowWindowOffset[iSlice] : NULL;
	ornRows = mpRowWindowOffset ? mpRowWindowOffset[iSlice + 1] - mpRowWindowOffset[iSlice] : 0;
    
	return S_OK;
}


int DSSDataModelXTabImpl::getColRange(int iSlice, const int *&orpCols, int &ornCols)
{
	if (iSlice < 0 || iSlice >= mnSlices)
		return E_INVALIDINDEX;
    
	orpCols = mpColList ? mpColList + mpColWindowOffset[iSlice] : NULL;
	ornCols = mpColWindowOffset ? mpColWindowOffset[iSlice + 1] - mpColWindowOffset[iSlice] : 0;
    
	return S_OK;
}

void ExcludeFrom(vector<int> &irvExcludeList, vector<int> &rvOriginal)
{
	vector<int> lvTemp;
	lvTemp.assign(rvOriginal.begin(), rvOriginal.end());
    
	std::sort(irvExcludeList.begin(), irvExcludeList.end());
	std::sort(lvTemp.begin(), lvTemp.end());
	
	int lnTempRows = lvTemp.size();
	int lnExcludeRows = irvExcludeList.size();
	
	rvOriginal.clear();
	
	int itemp = 0, iexc = 0;
	for (; itemp < lnTempRows && iexc < lnExcludeRows; )
	{
		if (lvTemp[itemp] == irvExcludeList[iexc])
		{
			itemp++;
			iexc++;
		}
		else if (lvTemp[itemp] < irvExcludeList[iexc])
		{
			rvOriginal.push_back(lvTemp[itemp]);
			itemp++;
		}
		else	//(lvTemp[itemp] > irvExcludeList[iexc])
		{
			iexc++;
		}
	}
	for (; itemp < lnTempRows; itemp++)
		rvOriginal.push_back(lvTemp[itemp]);
}

int DSSDataModelXTabImpl::Select(int nPaths, int nGBUnits, vector<GUID> &irGBUnitID, int *ipGBPaths,
                                 vector<int> &orvRowWindow, vector<int> &orvColWindow,
                                 int nExcludeGBUnits, vector<GUID> *ipvExcludeGBUnitID, int nExcludePaths, int *ipEXcludeGBPaths,
                                 bool ibGBAll)//522608, when GB all, multiple pages can be shown in the same screen
{ 
	int hr = S_OK;
#ifdef _DEBUG	
	if (mpCalculationPlan && mpCalculationPlan->getStatus() == DssCalcStatusUnCalculated && hr != S_OK)		//for DEBUG only
	{
		mpCalculationPlan->MarkUnCalculated(0);
		hr = ReCalculate();
		CHECK_HR;
	}
#endif
    
	if (mpTransactionModel)
	{
		//474376, if there's metric selector, load all slices into memory
		int i = 0;
		for (i = 0; i < mvGBUnits.size(); i++)
		{
			if (mvGBUnits[i] == mMetricUnitID)
				break;
		}
		if (i < mvGBUnits.size())
		{
            hr = mpDataSource->MapFileBuffer();
            CHECK_HR;
			hr = hLoadAllFromDataSource();
            CHECK_HR;
            hr = mpDataSource->UnMapFileBuffer();
			CHECK_HR;
		}
	}
	else if (mpDataSource && !mpTransactionModel 
             && !ibGBAll)//522608
	{	//there's data source, and transaction is not enabled
		//TODO: depends on the available memory, release previous loaded data slices
		UnLoadFromDataSource();
	}
    
    hr = MapSliceWindow(0);
    CHECK_HR;
    
    hr = MapSliceWindow(1);
    CHECK_HR;
    
    if(mpDataSource)
    {
        hr = mpDataSource->MapFileBuffer();
        CHECK_HR;
    }
    
	if (mpViewMap->getUnitCount() == 0)
	{
		if (mnSlices <= 0)		//for empty grid, //TODO: 409716 is broken, after UC selection, model may be empty
			return S_OK;
        
		_ASSERT(mnSlices == 1);
		
		int hr = LoadFromDataSource(0);
		CHECK_HR;
		
        if(mpRowWindowOffset)
            for(int i = mpRowWindowOffset[0]; i < mpRowWindowOffset[1]; i++)
            {
                if(mpRowList)
                    orvRowWindow.push_back(mpRowList[i]);
            }
        if(mpColWindowOffset)
            for(int i = mpColWindowOffset[0]; i < mpColWindowOffset[1]; i++)
            {
                if(mpColList)
                    orvColWindow.push_back(mpColList[i]);
            }
        
		if(mvRowSorts==NULL)
        {
			Refresh();
        }
		int i;
		for (i=mvRowSorts->size()-1; i>=0; --i)
		{
			hr = (*mvRowSorts)[i]->getSorter()->sort(mpView->getHeader(0), orvRowWindow);
			CHECK_HR;
		}
		for (i=mvColumnSorts->size()-1; i>=0; --i)
		{
            hr = (*mvColumnSorts)[i]->getSorter()->sort(mpView->getHeader(1), orvColWindow);
            CHECK_HR;
		}
        
        //if no GBunit, all header keys have been loaded
        //Load the keytables of the indices of the row & column headers
        //hr = LoadAllFromHeaderSource();
        //CHECK_HR;
	}
	else
	{
		vector<int> lvWindowID;
		int hr = mpViewMap->MapGBKeys();
        CHECK_HR;
        hr = hSelectWindows(nPaths, nGBUnits, irGBUnitID, ipGBPaths, lvWindowID);
		CHECK_HR;
		
		if (nExcludePaths > 0)
		{
			vector<int> lvExcludeWindowID;
			int hr = hSelectWindows(nExcludePaths, nExcludeGBUnits, *ipvExcludeGBUnitID, ipEXcludeGBPaths, lvExcludeWindowID);
			CHECK_HR;
			
			ExcludeFrom(lvExcludeWindowID, lvWindowID);
		}
		
        hr = mpViewMap->UnMapGBKeys();
        CHECK_HR;
        
		int lnViews = lvWindowID.size();
		int i = 0;
		for (i = 0; i < lnViews; i++)
		{
			hr = LoadFromDataSource(lvWindowID[i]);
			CHECK_HR;
		}
        
        //TQMS 498647, the subtotal display order shouldn't be changed
        set<int> lsList;
        for(i = 0; i < lnViews; i++)
        {
            int offset = lvWindowID[i];
            if(offset < mnSlices)
            {
                if(mpRowWindowOffset)
                    for(int lRow = mpRowWindowOffset[offset]; lRow < mpRowWindowOffset[offset + 1]; lRow ++)
                    {
                        if(mpRowList && lsList.find(mpRowList[lRow]) == lsList.end())
                        {
                            lsList.insert(mpRowList[lRow]);
                            orvRowWindow.push_back(mpRowList[lRow]);
                        }
                    }
            }
        }
        
		lsList.clear();
        for(i = 0; i < lnViews; i++)
        {
            int offset = lvWindowID[i];
            if(offset < mnSlices)
            {
                if(mpColWindowOffset)
                {
                    for(int lRow = mpColWindowOffset[offset]; lRow < mpColWindowOffset[offset + 1]; lRow ++)
                    {
                        if(mpColList && lsList.find(mpColList[lRow]) == lsList.end())
                        {
                            lsList.insert(mpColList[lRow]);
                            orvColWindow.push_back(mpColList[lRow]);
                        }
                    }
                }
            }
        }
        
        //Load the keytables of the indices of the row & column headers
        if(mbLoadHeaderBySlice)
        {
            hr = LoadFromHeaderSource(lvWindowID);
            CHECK_HR;
        }
        
		if(mvRowSorts==NULL)
			Refresh();
        
		for (i=mvRowSorts->size()-1; i>=0; --i)
		{
			hr = (*mvRowSorts)[i]->getSorter()->sort(mpView->getHeader(0), orvRowWindow);
			CHECK_HR;
		}
        
		for (i=mvColumnSorts->size()-1; i>=0; --i)
		{
			hr = (*mvColumnSorts)[i]->getSorter()->sort(mpView->getHeader(1), orvColWindow);
			CHECK_HR;
		}
	}
    
    if(mpDataSource)
    {
        hr = mpDataSource->UnMapFileBuffer();
        CHECK_HR;
    }
    
    hr = UnMapSliceWindow(0);
    CHECK_HR;
    
    hr = UnMapSliceWindow(1);
    CHECK_HR;
    
	return S_OK;
}

int DSSDataModelXTabImpl::setIsRunTimeUnit(int index, bool val)
{
	if (index >= 0 && index < mvUnits.size())
	{
		mvUnits[index]->setIsRuntimeUnit(val);
		return S_OK;
	}
	else
		return E_INVALIDINDEX;
}

int DSSDataModelXTabImpl::Refresh()
{
	int hr = EnsureModelData();
	CHECK_HR;
	
	deleteSorts();
	DSSSortConverter converter(this, hGetTabularData());
	vector<int>& rowUnits=mpView->getHeader(0)->getUnits();
	vector<int>& columnUnits=mpView->getHeader(1)->getUnits();
	mvRowSorts=converter.convert(mpTemplate->getRow(), rowUnits);
	mvColumnSorts=converter.convert(mpTemplate->getColumn(), columnUnits);
	return 0;
}

void DSSDataModelXTabImpl::deleteSorts()
{
	int i=0;
	if(mvRowSorts)
	{
		for(i=0;i<mvRowSorts->size();++i)
			delete (*mvRowSorts)[i];
		delete mvRowSorts;
		mvRowSorts=NULL;
	}
	if(mvColumnSorts)
	{
		for(i=0;i<mvColumnSorts->size();++i)
			delete (*mvColumnSorts)[i];
		delete mvColumnSorts;
		mvColumnSorts=NULL;
	}
}

int DSSDataModelXTabImpl::getMaxDataOffset(int iMetricIndex, bool ibSkipNull)
{
	if (!mpMetricsUnit)
		return E_FAIL;
	
	int lnMetrics = lnMetrics = mpMetricsUnit->Count();
	
	if (iMetricIndex < 0 || iMetricIndex >= lnMetrics)
		return E_INVALIDINDEX;
	
	if (mvMaxDataOffset.empty())
	{
		int lnMetrics = mpMetricsUnit->Count();
		mvMaxDataOffset.resize(lnMetrics);
        
		for (int i = 0; i < lnMetrics; i++)
			mvMaxDataOffset[i] = -1;
	}
	
	if (mvMaxDataOffset[iMetricIndex] == -1)
	{
		DSSCubeDataSlice *lpSlice = NULL;
		mpView->getDataSlice(0, 0, iMetricIndex, &lpSlice);		//only check report level
		CHECK_PTR(lpSlice);
		
		DSSDataColumn *lpColumn = lpSlice->getColumn();
		CHECK_PTR(lpColumn);
		
		int lnRows = lpColumn->getRowCount();
		int max = 0;
		DSSDataFlag lFlag = DssDataOk;
		lpColumn->getFlag(max, lFlag);
		while(lFlag != DssDataOk && ibSkipNull)
		{
			max ++;
			if(max >= lnRows)
				return -1;
			lpColumn->getFlag(max, lFlag);
		}
		for (int j = max + 1; j < lnRows; j++)
		{
			lpColumn->getFlag(j,lFlag);
			if(lFlag != DssDataOk && ibSkipNull)
				continue;
			if (lpColumn->Order(max, j) < 0)
				max = j;
		}
		mvMaxDataOffset[iMetricIndex] = max;
	}
	
	return mvMaxDataOffset[iMetricIndex];
}

int DSSDataModelXTabImpl::getMinDataOffset(int iMetricIndex, bool ibSkipNull)
{
	if (!mpMetricsUnit)
		return E_FAIL;
	
	int lnMetrics = lnMetrics = mpMetricsUnit->Count();
	
	if (iMetricIndex < 0 || iMetricIndex >= lnMetrics)
		return E_INVALIDINDEX;
	
	if (mvMinDataOffset.empty())
	{
		int lnMetrics = mpMetricsUnit->Count();
		mvMinDataOffset.resize(lnMetrics);
		
		for (int i = 0; i < lnMetrics; i++)
			mvMinDataOffset[i] = -1;
	}
	
	if (mvMinDataOffset[iMetricIndex] == -1)
	{
		DSSCubeDataSlice *lpSlice = NULL;
		mpView->getDataSlice(0, 0, iMetricIndex, &lpSlice);		//only check report level
		CHECK_PTR(lpSlice);
		
		DSSDataColumn *lpColumn = lpSlice->getColumn();
		CHECK_PTR(lpColumn);
		
		int lnRows = lpColumn->getRowCount();
		int min = 0;
		DSSDataFlag lFlag = DssDataOk;
		lpColumn->getFlag(min, lFlag);
		while(lFlag !=  DssDataOk && ibSkipNull)
		{
			min ++;
			if(min >= lnRows)
				return -1;
			lpColumn->getFlag(min,lFlag);
		}
		for (int j = min + 1; j < lnRows; j++)
		{
			lpColumn->getFlag(j, lFlag);
			if(lFlag !=  DssDataOk && ibSkipNull)
				continue;
			if (lpColumn->Order(min, j) > 0)
				min = j;
		}
		mvMinDataOffset[iMetricIndex] = min;
	}
	
	return mvMinDataOffset[iMetricIndex];
}

int DSSDataModelXTabImpl::FindElement(DSSBaseElementProxy *ipElement, int &orUnit, int &orKey)
{
	int result = DSSDataModelBase::FindElement(ipElement, orUnit, orKey);
	if (result == S_FALSE)  // subtotal element
	{	
		if(ipElement->getElementType() == DssElementSubtotal)
		{
			orKey = - static_cast<DSSSubtotalElementProxy*> (ipElement)->getSubtotalIndex();
			return S_OK;
		}
		return S_FALSE;
	}
	else
		return result;
}

int DSSDataModelXTabImpl::size()
{
	int lSize = sizeof(this);
	
	lSize += hCalculateSizeOfBase();
	
	lSize += (sizeof(SubtotalInformation) + 32) * mvTotalInfo.size();		//we use the fixed size of subtoal info, ignore total name
    
	lSize += DSSAEHelper::SizeOfStrings(mvDataNullDisplay);
	
	lSize += sizeof(char) * mNullReplacement.size();
    
	lSize += DSSAEHelper::SizeOfStrings(mvElementNullDisplay);
    
	if (mpCube)
		lSize += mpCube->size();
	
	if (mpView)
		lSize += mpView->size();
	
	lSize += sizeof(int) * (mnRowList + mnRowWindowOffset + mnColList + mnColWindowOffset);
	
	if (mvRowSorts)
		lSize += (sizeof(DSSSortDefinition*) + sizeof(DSSSortDefinition))* mvRowSorts->size();
    
	if (mvColumnSorts)
		lSize += (sizeof(DSSSortDefinition*) + sizeof(DSSSortDefinition))* mvColumnSorts->size();
	
	lSize += mvCellFmtIDs.size() * sizeof(GUID);
    
	lSize += sizeof(int) * (mvMaxDataOffset.size() + mvMinDataOffset.size() + mvGBUnits.size());
    
	if (mpDataSource)
		lSize += mpDataSource->size();
    
	return lSize;
}

int DSSDataModelXTabImpl::setDataSource(DSSDataSource *ipDataSource)
{
	mpDataSource = ipDataSource;
	return S_OK;
}

int DSSDataModelXTabImpl::LoadFromDataSource(int iSlice)
{
	if (mpDataSource)
	{
		bool lbNewSlice = false;
		int hr = mpDataSource->LoadData(this, iSlice, lbNewSlice);
		CHECK_HR;
		
		if (lbNewSlice)
		{
			//TQMS 444532, reset Min/Max;
			int lnMetrics = mpMetricsUnit->Count();
			if (!mvMaxDataOffset.empty())
			{
				for (int i = 0; i < lnMetrics; i++)
					mvMaxDataOffset[i] = -1;
			}
			if (!mvMinDataOffset.empty())
			{
				for (int i = 0; i < lnMetrics; i++)
					mvMinDataOffset[i] = -1;
			}
		}
		
		if (mpDataSource->IsAllLoaded())
		{
            //			delete mpDataSource;
            //			mpDataSource = NULL;
		}
	}
	return S_OK;
}

void DSSDataModelXTabImpl::UnLoadFromDataSource()
{
	//TODO: for now, we always release the old data slices before load the new one
	bool lbHasEnoughMemory = false;
	if (!lbHasEnoughMemory)
	{
		mpView->RecycleMetricSliceMemory();
		
		if (mpDataSource)
			mpDataSource->Reset();
	}
	return;
}

EnumDSSSubtotalsPosition DSSDataModelXTabImpl::getRowSubtotalPosition()
{
	DSSSorts* sorts = mpTemplate->getRow()->getSorts();
	if(sorts == NULL || sorts->Count() == 0 || mvRowSorts == NULL || mvRowSorts->size() == 0)
		return DssSubtotalsPositionInherit;
	return (*mvRowSorts)[0]->getSubtotalPosition();
}

EnumDSSSubtotalsPosition DSSDataModelXTabImpl::getColSubtotalPosition()
{
	DSSSorts* sorts = mpTemplate->getColumn()->getSorts();
	if(sorts == NULL || sorts->Count() == 0 || mvColumnSorts == NULL || mvColumnSorts->size() == 0)
		return DssSubtotalsPositionInherit;
	return (*mvColumnSorts)[0]->getSubtotalPosition();
}
DSSCalculationPlan * DSSDataModelXTabImpl::getCalculationPlan()
{
	if (!mpCalculationPlan)
	{
		DSSCalculationPlan *lpCalculationPlan = new DSSCalculationPlan();
		int hr = lpCalculationPlan->Init(this);
		if (hr != S_OK)
		{
			_ASSERT(false);
			delete lpCalculationPlan;
			lpCalculationPlan = NULL;
		}
		mpCalculationPlan = lpCalculationPlan;
	}
	return mpCalculationPlan;
}

int DSSDataModelXTabImpl::hLoadAllFromDataSource()
{
	if (mpDataSource && !mpDataSource->IsAllLoaded())
	{
		int lnViews = mpViewMap->CountRows();
		int i = 0;
		for (i = 0; i < lnViews; i++)
		{
			int hr = LoadFromDataSource(i);
			CHECK_HR;
		}
	}
	return S_OK;
}

int DSSDataModelXTabImpl::ReCalculate()
{
	int hr = EnsureModelData();
	CHECK_HR;
	
	if (mpCalculationPlan->getStatus() != DssCalcStatusUnCalculated && mpCalculationPlan->getStatus() != DssCalcStatusNeedEvalThres)
		return S_OK;	//472504, no action if nothing is changed
    
	hr = hLoadAllFromDataSource();
	CHECK_HR;
	
    if(mpCalculationPlan->getStatus() != DssCalcStatusNeedEvalThres)
    {
        if (!mpCalculationPlan)
        {
            getCalculationPlan();
        }
        CHECK_PTR(mpCalculationPlan);

        if(!mpTabularData)
            hGetTabularData();
        
        if(!mpView)
            CreateXTabView();
        
        DSSCube* lpCube = NULL;
        hr = mpView->GetCube(&lpCube);
        CHECK_HR;
        
        if(!lpCube)
        {
            mpView->setCube(getCube());
        }

        DSSProcessMetrics lProcess(mpCalculationPlan, this);
        hr = lProcess.CalculateMetrics();
        CHECK_HR;
    }
    
    DSSFunctionServer lpFunctionServer;
    DSSMetricThresholds lMetricThresholds;
    hr = lMetricThresholds.calculateThreshold(this, mpTabularData, mpCalculationPlan, mpTemplate,&lpFunctionServer);
    CHECK_HR;
    
    //hr = mpCalculationPlan->FinalizeEvent(getCube());
	//CHECK_HR;
	
	
	return S_OK;
}

void DSSDataModelXTabImpl::ReleaseUnusedMemory()
{
	if (!mpTransactionModel)
	{
		hCleanUp();		//clean up itself
		DSSDataModelBase::ReleaseUnusedMemory();
	}
	return;
}

int DSSDataModelXTabImpl::setDummyUnit(int iUnitID)
{
	if(iUnitID < 0 || iUnitID >= mvUnits.size())
		return E_INVALIDINDEX;
	
	mpDummyUnit = mvUnits[iUnitID];
	
	return S_OK;
}

int DSSDataModelXTabImpl::getDummyUnitIndex(int *oDummyUnitIdx)
{
    if(!oDummyUnitIdx)
        return E_INVALIDARG;
    
    *oDummyUnitIdx = -1;
    for(int i = 0; i < mvUnits.size(); ++i)
    {
        if(mpDummyUnit == mvUnits[i])
        {
            *oDummyUnitIdx = i;
            return S_OK;
        }
    }
    
    return S_OK;
}


int DSSDataModelXTabImpl::LoadAllDataSlices()
{
	if (mpTransactionModel)
	{
		return hLoadAllFromDataSource();
	}
	return S_OK;
}

int DSSDataModelXTabImpl::getBinaryTransactionModel (bool includeCommitted, unsigned char** oppBinaryModel, int* opSize)
{
	int hr = S_OK;
	if(mpTransactionModel)
	{	
		hr = mpTransactionModel->SaveToBinary(includeCommitted, oppBinaryModel, opSize);
		CHECK_HR;
	}
	return S_OK;
}


int DSSDataModelXTabImpl::setBinaryTransactionModel (unsigned char * ipBinaryModel, int iSize)
{
	int hr = S_OK;
	hr = this->EnsureModelData();
	CHECK_HR;
    
    hr = EnsureTransactionData();
    CHECK_HR;
    
    return LoadBinaryTransactionModel(ipBinaryModel, iSize, mpTransactionModel, this);
}

//make LoadBinaryTransactionModel as static method so that the code can be shared by new rwd engine
int DSSDataModelXTabImpl::LoadBinaryTransactionModel (unsigned char * ipBinaryModel, int iSize,
                                                      DSSDataModelTransaction *ipTransactionModel,
                                                      IDSSTemplateModel *ipTemplateNodeModel)
{
    if (!ipTransactionModel || !ipTemplateNodeModel)
        return E_INVALIDARG;
    
	int hr = S_OK;
    
    DSSXTabView *lpView = ipTemplateNodeModel->getXTabView();
    
	if(ipTransactionModel && ipBinaryModel)
	{
        //		vector<int>* lpMapTrxColumn2XTabUnit = NULL;
		vector<int>* lpMapTrxColumn2BaseForm = NULL;
        //      vector<int>* lpMapTrxColumn2WBForm = NULL;
		hr = ipTransactionModel->getColumnMappingInfo(NULL, &lpMapTrxColumn2BaseForm, NULL);
		CHECK_HR;
		
        vector<int>* lpMapOfflineTrxColumn2BaseForm = NULL;
        hr = ipTransactionModel->getOfflineColumnMappingInfo(NULL, &lpMapOfflineTrxColumn2BaseForm, NULL);
        CHECK_HR;
        
		DSSBinaryDataReader lBinaryReader;
		
		hr = lBinaryReader.Init(ipBinaryModel, iSize, false);
		CHECK_HR;
		
		//TODO: how to use current slice flag?
		int currentSliceFlag = -1;
		hr = lBinaryReader.ReadInt(&currentSliceFlag);
		CHECK_HR;
		
		int lModelType = -1;
		hr = lBinaryReader.ReadInt(&lModelType);
		CHECK_HR;
        
		if(lModelType != DssModelXTab)
			return E_UNEXPECTED;
		
		//load the column header info
		int lnHeaderColumn = 0;
		hr = lBinaryReader.ReadInt(&lnHeaderColumn);
		CHECK_HR;
        
		for(int i = 0; i < lnHeaderColumn; i++)
		{
			int lColumnIndex = -1;
			hr = lBinaryReader.ReadInt(&lColumnIndex);
			CHECK_HR;
			
			GUID lUnitGUID = GUID_NULL;
			char* lpChar = (char*)(&lUnitGUID);
			hr = lBinaryReader.ReadByteArray(lpChar, sizeof(GUID));
			CHECK_HR;
			
			//find the tabular unit
            //			int lUnit = FindUnit(lUnitGUID, DssTemplateReserved);
            //			DSSTabularUnit* lpUnit = Item(lUnit);	
            DSSTabularUnit* lpUnit = ipTemplateNodeModel->FindTabularUnit(lUnitGUID, DssTemplateReserved);
			CHECK_PTR(lpUnit);
			
			int lBaseForm = -1;
			hr = lBinaryReader.ReadInt(&lBaseForm);
			CHECK_HR;
			
			hr = ipTransactionModel->setColumnInfo(lColumnIndex, lpUnit, lBaseForm);
			CHECK_HR;
		}
        
        //load offline column data type info, not used by grid model
        int lnOfflineColumn = 0;
        hr = lBinaryReader.ReadInt(&lnOfflineColumn);
        CHECK_HR;
        
        for(int i = 0; i < lnOfflineColumn; i++)
        {
            int lDataType = 0;
            hr = lBinaryReader.ReadInt(&lDataType);
            CHECK_HR;
        }
        
		//populate the transaction records
		int lnRecord = 0;
		hr = lBinaryReader.ReadInt(&lnRecord);
		CHECK_HR;
        
        if(lnRecord > 0)
            ipTemplateNodeModel->LoadAllDataSlices();
        bool lbSetCommit = false;
        EnumDSSTransactionDataChangeType lFlag = DssTransactionDataReserved;
		for(int i = 0; i < lnRecord; i++)
		{
			lFlag = DssTransactionDataReserved;
			hr = lBinaryReader.ReadInt((int*)(&lFlag));
			CHECK_HR;
			
            //when saving the transaction records, the committed records will be saved first
            //so when we meet a Update  type records, we need to set commit for all the previous records
            if(lFlag == DssTransactionDataUpdate || lFlag == DssTransactionDataOriginal)
            {
                if(i > 0 && !lbSetCommit)
                {
                    hr = ipTransactionModel->setAllCommit();
                    CHECK_HR;
                }
                lbSetCommit = true;
            }
            
			int lOriginalRow = 0;
			hr = lBinaryReader.ReadInt(&lOriginalRow);
			CHECK_HR;
            
			int lOriginalCol = 0;
			hr = lBinaryReader.ReadInt(&lOriginalCol);
			CHECK_HR;
			
            int lbMarked = 0;
            hr = lBinaryReader.ReadInt(&lbMarked);
            CHECK_HR;
            
            int lbValidate = 1;
            hr = lBinaryReader.ReadInt(&lbValidate);
            CHECK_HR;
            
			int lRow = lOriginalRow;
			int lCol = lOriginalCol;
			
			//load the changed cells
			int lnChangedCell = 0;
			hr = lBinaryReader.ReadInt(&lnChangedCell);
			CHECK_HR;
            
            vector<AERawData> lvOriginalData;
            vector<AERawData> lvDisplayData;	
			if(lnChangedCell == 0)//row marked but not changed
			{
                
				hr = ipTransactionModel->GenerateRecordFromXTabView(lpView, lRow, lCol, lvOriginalData, lvDisplayData);
				CHECK_HR;
				
				//no need to adjust row ordinal since mark row is only enabled for tabular grids with metrics on column
				hr = ipTransactionModel->MarkRecord(lRow, &lvOriginalData,&lvDisplayData, DssTransactionDataInsert);
				CHECK_HR;
				
			}
			else
			{
				vector<AERawData> *lpOriginalData = NULL;
                vector<AERawData> *lpDisplayData = NULL;
				int hr = ipTransactionModel->GenerateRecordFromXTabView(lpView, lRow, lCol, lvOriginalData, lvDisplayData);
				CHECK_HR;
				
				lpOriginalData = &lvOriginalData;
                lpDisplayData = &lvDisplayData;
				
				for(int j = 0; j < lnChangedCell; j++)
				{
					int lColumnIndex = -1;
					hr = lBinaryReader.ReadInt(&lColumnIndex);
					CHECK_HR;
					
					int lStrLength = 0;
					hr = lBinaryReader.ReadInt(&lStrLength);
					CHECK_HR;
					
                    if (lStrLength+1 == 0) //check if integer overflow, resulting in buffer overflow
                        return DISP_E_OVERFLOW;
					char* lpText = new char[lStrLength + 1];
					memset(lpText,0,lStrLength+1);
					hr = lBinaryReader.ReadByteArray(lpText, lStrLength);
					CHECK_HR;
                    
					std::string lText = lpText;
					delete[] lpText;
					
                    // load display text for DIC                
                    lStrLength = 0;
                    hr = lBinaryReader.ReadInt(&lStrLength);
                    CHECK_HR;
                    
                    char* lpDisplayText = NULL;
                    std::string lDisplayText = lText;
                    if(lStrLength >=0 )
                    {
                        if (lStrLength+1 == 0) //check if integer overflow, resulting in buffer overflow
                            return DISP_E_OVERFLOW;
                        lpDisplayText = new char[lStrLength + 1];
                        memset(lpDisplayText,0,lStrLength+1);
                        hr = lBinaryReader.ReadByteArray(lpDisplayText, lStrLength);
                        CHECK_HR;
                        
                        lDisplayText = lpDisplayText;
                        delete lpDisplayText;
                    }
                    
                    
					int lKey = 0;
					hr = lBinaryReader.ReadInt(&lKey);
					CHECK_HR;
                    
					AERawData lRawData;
					
					hr = lBinaryReader.ReadInt((int*)(&lRawData.mFlag));
					CHECK_HR;
					
					hr = lBinaryReader.ReadInt((int*)(&lRawData.mDataType));
					CHECK_HR;
					
					hr = lBinaryReader.ReadInt(&lRawData.mSizeInByte);
					CHECK_HR;
                    
					lRawData.mValue = new char[lRawData.mSizeInByte];
					hr = lBinaryReader.ReadByteArray(lRawData.mValue, lRawData.mSizeInByte);
					CHECK_HR;	
					
					(*lpOriginalData)[lColumnIndex] = lRawData;
					
					
					DSSTabularUnit* lpUnit = NULL;
					int lBaseForm = -1;
					hr = ipTransactionModel->getColumnInfo(lColumnIndex, &lpUnit, &lBaseForm);
					CHECK_HR;
					
					if (!lpUnit)//data cell
					{
						//need to adjust the row/col ordinal
						int lMetricID = -1;
                        if(lpMapTrxColumn2BaseForm && lColumnIndex < lpMapTrxColumn2BaseForm->size())
                        {
                            lMetricID = (*lpMapTrxColumn2BaseForm)[lColumnIndex];
                        }else if(lpMapOfflineTrxColumn2BaseForm && lpMapTrxColumn2BaseForm && lColumnIndex < lpMapOfflineTrxColumn2BaseForm->size() + lpMapTrxColumn2BaseForm->size())
                        {
                            lMetricID = (*lpMapOfflineTrxColumn2BaseForm)[lColumnIndex - lpMapTrxColumn2BaseForm->size()];
                        }else{
                            return E_INVALIDINDEX;
                        }
                        
                        DSSTabularMetrics *lpMetricsUnit = NULL;
                        hr = ipTemplateNodeModel->getMetricsUnit(&lpMetricsUnit);
						CHECK_PTR(lpMetricsUnit);
                        
						if(lpView->getHeader(0)->HasMetric())//metric on row
							lRow = lOriginalRow + lMetricID;
						else if(lpView->getHeader(1)->HasMetric())//metric on column
							lCol = lOriginalCol + lMetricID;
						else 
							return E_UNEXPECTED;
						
						hr = ipTransactionModel->AddRecord(lRow, lCol, lColumnIndex, lText, lpOriginalData, lpDisplayData);
						CHECK_HR;
						
						hr = ipTransactionModel->UpdateDataCell(lRow, lCol, lColumnIndex, &lText);
						CHECK_HR;
						
						//537872, mark the calculation as dirty so that the related event will be re-calculated
						hr = ipTemplateNodeModel->getCalculationPlan()->MarkUnCalculated(lpView->getMetricID(lRow, lCol));
						CHECK_HR;
                        
					}
					else //header element cell
					{
						EnumDSSTemplateUnitType lUnitType = lpUnit->getUnitType();
						AE_ASSERT(lUnitType != DssTemplateMetrics); //the header element of metrics should not be editable
						
						if(lUnitType == DssTemplateConsolidation)
						{
							if (static_cast<DSSTabularConsolidation*> (lpUnit)->getChildElementKey(lKey) < 0)
								return S_OK;			//474246, edit Derive Element's group node is not allowed
						}
						
						//the col oridinal is always 0, and row ordinal doesn't need to adjust 
						//because we only allow user to edit the header cells on row and don't allow metric appear on row at the same time
						hr = ipTransactionModel->AddRecord(lRow, lCol, lColumnIndex, lText, lpOriginalData, lpDisplayData);
						CHECK_HR;
						
                        //DIC displaytext
                        if(lpDisplayText)
                        {
                            hr = ipTransactionModel->UpdateHeaderCell(lRow, lCol, lColumnIndex, lpUnit, lBaseForm, lKey, &lDisplayText,true);
                            CHECK_HR;
                        }
                        else
                        {
                            hr = ipTransactionModel->UpdateHeaderCell(lRow, lCol, lColumnIndex, lpUnit, lBaseForm, lKey, &lText);
                            CHECK_HR;
                        }
					}
				}
			}
            if(lbMarked != 0)
            {
                hr = ipTransactionModel->markRow(lRow,0);
                CHECK_HR;
            }
            if(lbValidate == 0)
            {
                hr = ipTransactionModel->setValidationFailedFlag(lRow,lCol);
                CHECK_HR;
            }
            if(lFlag == DssTransactionDataOriginal)
            {
                hr = ipTransactionModel->MarkRecord(lRow, &lvOriginalData, &lvDisplayData, lFlag);
            }
			hr = ipTransactionModel->setRecordFlag(lOriginalRow, lOriginalCol, lFlag);
			CHECK_HR;
        }
        if(lnRecord > 0 && lFlag == DssTransactionDataCommit && !lbSetCommit)
        {
            hr = ipTransactionModel->setAllCommit();
            CHECK_HR;
            // lbSetCommit = true;
        }
        if(lnRecord > 0)
        {
            ipTemplateNodeModel->getCalculationPlan()->setStatus(DssCalcStatusUnCalculated);
            
            //don't need to recalculate here since in GridIterator::reinit we will always refresh xtabview for new rwd
            if(ipTemplateNodeModel->getGridModelVersion() != 1)
                ipTemplateNodeModel->ReCalculate();
        }
	}
	return S_OK;
}

int DSSDataModelXTabImpl::EnsureTransactionData()
{
    int hr = S_OK;
    
    if(!mpObjContext)
        return E_FAIL;
    
    DSSRWNode* lpNode = mpObjContext->getNode(mNodeKey);
    CHECK_PTR(lpNode);
    
    AE_ASSERT(lpNode->getType() == DssRWNodeTemplate);
    
    DSSRWTemplateNode * lpThisNode = static_cast<DSSRWTemplateNode*> (lpNode);
    
    if(!mpTransactionModel)
    {
        if (lpThisNode->IsTransactionEnabled())	//transaction enabled
        {
            mpTransactionModel = new DSSDataModelTransaction();
        }
    }
    
    //return OK if not a transaction enabled xtab model
	if (!mpTransactionModel)
		return S_OK;
    
    DSSTransactionReport *lpTargetReport = lpThisNode->getTransactionReport();
	CHECK_PTR(lpTargetReport);
    
	if (mpTransactionModel->getStatus() != XTAB_INITIALIZED)
	{
		hr = mpTransactionModel->Init(lpTargetReport, this);
		CHECK_HR;
	}
    
    if(!mpTemplate)
        mpTemplate = lpThisNode->getTemplate();
    CHECK_PTR(mpTemplate);
    
    return EnsureTransactionData(mpTransactionModel, this, lpTargetReport, &mvUnits);
}

//make EnsureTransactionData as static method so that the code can be shared by new rwd engine
int DSSDataModelXTabImpl::EnsureTransactionData(DSSDataModelTransaction *ipTransactionModel,
                                                IDSSTemplateModel *ipTemplateNodeModel,
                                                DSSTransactionReport *ipTargetReport,
                                                const vector<DSSTabularUnit *> *ipvTabularUnits,
                                                DFCCSITransactionBase* ipCSI)
{
    int hr = S_OK;
    
    
    DSSTemplate *lpTemplate = ipTemplateNodeModel->getTemplate();
    DSSXTabView *lpView = ipTemplateNodeModel->getXTabView();
    
    vector<int>* lpMapTrxColumn = NULL;
    hr = ipTransactionModel->getColumnMappingInfo(&lpMapTrxColumn, NULL, NULL);
    CHECK_HR;
    
    //return if the maps are already set
    if(lpMapTrxColumn && lpMapTrxColumn->size() > 0)
        return S_OK;
    
    vector<int> mapTrxColumn2XTabUnit ;
    vector<int> mapTrxColumn2BaseForm ;
    vector<int> mapTrxColumn2WBForm ;
    
	map<int, DSSColumnMappingInfo*>* lpColumnInfoMap = ipTargetReport->getColumnMap();
    int lnTargetColumns = 0;
    //782890
    map<int, DSSColumnMappingInfo*>::iterator lIter;
	for (lIter = lpColumnInfoMap->begin(); lIter != lpColumnInfoMap->end(); lIter++)
	{
        if(lIter->first > lnTargetColumns)
            lnTargetColumns = lIter->first;
    }
	
	DSSTabularMetrics *lpTabularMetrics = NULL;
	hr = ipTemplateNodeModel->getMetricsUnit(&lpTabularMetrics);
	
	int lnMetrics = lpTabularMetrics ? lpTabularMetrics->Count() : 0;
	
	mapTrxColumn2XTabUnit.resize(lnTargetColumns);
	mapTrxColumn2BaseForm.resize(lnTargetColumns);
    mapTrxColumn2WBForm.resize(lnTargetColumns);
	//build the map from source template to transaction report
	int i = 0;
	for (i = 0; i < lnTargetColumns; i++)		//473622, init to -1 so that un-mapped column can be mapped
	{
		mapTrxColumn2XTabUnit[i] = -1;
		mapTrxColumn2BaseForm[i] = -1;
        mapTrxColumn2WBForm[i] = -1;
	}
	
	//1, use the document level mapping first:
    //    int lnUnits = mvUnits.size();
    int lnUnits = ipvTabularUnits->size();
    
	for (int iAxis = 0; iAxis < 2; iAxis++)
	{
		DSSXTabHeader *lpHeader = lpView->getHeader(iAxis);
		CHECK_PTR(lpHeader);
        
		int lnHeaderUnits = lpHeader->getNumUnits();
		for (i = 0; i < lnHeaderUnits; i++)
		{
			int lUnitID = lpHeader->getUnit(i);
			AE_ASSERT(lUnitID < lnUnits);
            
            //            DSSTabularUnit *lpXTabUnit = this->Item(lUnitID);
            DSSTabularUnit *lpXTabUnit = (*ipvTabularUnits)[lUnitID];
			CHECK_PTR(lpXTabUnit);
            
			if (lpXTabUnit->getUnitType() == DssTemplateAttribute)
			{
				GUID lGUID;
				lpXTabUnit->getGUID(lGUID);
				DSSTemplateUnit *lpUnit = lpTemplate->getTemplateUnitFromAxisIndex(iAxis + 1, lGUID);
                if(!lpUnit)
                    continue;
				DSSTemplateAttribute* lpAttribute = static_cast<DSSTemplateAttribute*> (lpUnit);
                DSSAttributeFormWrappers *lpForms = lpAttribute->getAttribute()->getFormWrappers();
                int lnForms = lpForms->Count();
                for (int i = 0; i < lnForms; i++)
                {
                    DSSAttributeFormWrapper *lpForm = lpForms->Item(i);		//465385, 465131
                    int lColumnIndex = lpAttribute->getColumnIndexByForm(lpForm->getID());
                    lColumnIndex --;	//it's 1-based
                    
                    if (lColumnIndex >= 0 && lColumnIndex < mapTrxColumn2XTabUnit.size())
                    {
                        mapTrxColumn2XTabUnit[lColumnIndex] = lUnitID;
                        mapTrxColumn2BaseForm[lColumnIndex] = lpForm->getBaseFormID();
                    }
                }
                
			}
			else if(lpXTabUnit->getUnitType() == DssTemplateMetrics)
			{
				DSSTemplateUnit *lpUnit = lpTemplate->getTemplateMetrics();
                
                DSSTemplateMetrics* lpTemplateMetrics = static_cast<DSSTemplateMetrics*> (lpUnit);
                int lnMetrics = lpTemplateMetrics->Count();
                for (int i = 0; i < lnMetrics; i++)
                {
                    DSSTemplateMetric *lpMetric = lpTemplateMetrics->Item(i);
                    int lColumnIndex = lpMetric->getColumnIndex();
                    lColumnIndex --;	//it's 1-based
                    if (lColumnIndex >= 0 && lColumnIndex < mapTrxColumn2XTabUnit.size())
                    {
                        GUID lGUID = lpMetric->getID();
                        int lMetricID = lpTabularMetrics->FindDisplayedMetric(lGUID);
                        if (lMetricID >= 0)
                        {
                            mapTrxColumn2XTabUnit[lColumnIndex] = -1;
                            mapTrxColumn2BaseForm[lColumnIndex] = lMetricID;
                        }
                    }
                }
			}
		}
	}
	
    //DDIC definition
    //    DSSTemplate* lpTemplate = getTemplate();
    for (i = 0; lpTemplate && i < lnTargetColumns; i++)
	{
        if(lpColumnInfoMap->find(i+1) == lpColumnInfoMap->end())
            continue;
		DSSColumnMappingInfo *lpColumnInfo = (*lpColumnInfoMap)[i + 1];	//column mapping info is 1-based
        
		if (lpColumnInfo->getType() != DssTypeMetric 
            && !(mapTrxColumn2XTabUnit[i] == -1 && mapTrxColumn2BaseForm[i]>=0))//a metric in the xtab  may be mapped to a attribute in the transaction report
		{
            int lUnit = mapTrxColumn2XTabUnit[i];
            int lBaseFormID = mapTrxColumn2BaseForm[i];
            //            DSSTabularUnit* lpTabularUnit = Item(lUnit);
            DSSTabularUnit* lpTabularUnit = NULL;
            if(lUnit >= 0 && lUnit < ipvTabularUnits->size())
                lpTabularUnit = (*ipvTabularUnits)[lUnit];
            if(!lpTabularUnit)
                continue;
            GUID lUnitID = lpTabularUnit->getGUID();
            //build a map for DDIC write back form
            if(lpTemplate)
            {
                //getAxis is 1 based
                DSSAxis* lpRowAxis = lpTemplate->getAxis(1);
                if(lpRowAxis)
                {
                    DSSTemplateUnit* lpTemplateUnit = lpTemplate->getTemplateUnitFromAxis(lpRowAxis, lUnitID);
                    if(lpTemplateUnit && lpTemplateUnit->getType() == DssTemplateAttribute)
                    {
                        DSSTemplateAttribute* lpTemplateAttribute = static_cast<DSSTemplateAttribute*>(lpTemplateUnit);
                        DSSAttributeFormWrappers* lpForms = lpTemplateAttribute->getFormWrappers();
                        if(lpForms)
                        {
                            int lnForm = lpForms->Count();
                            for(int j = 0;j< lnForm;j++)
                            {
                                DSSAttributeFormWrapper* lpForm = lpForms->Item(j);
                                if(lpForm && lBaseFormID == lpForm->getBaseFormID())    //that's the form where ColumnMappingInfo is defined
                                {
                                    GUID lGUID = lpForm->getID();
                                    DSSRWControl* lpControl = lpTemplateAttribute->getInputControlByForm(lGUID);
                                    if(lpControl && lpControl->getSourceID() != GUID_NULL)       //there's DDIC defined
                                    {
                                        //we don't have a match, then use a value like -100 to indicate
                                        mapTrxColumn2WBForm[i] = -100;
                                        
                                        if(lpControl->getSourceID() == lUnitID)
                                        {
                                            DSSAttributeFormWrappers* lpWBForms = lpControl->getWriteBackForms();
                                            if(lpWBForms && lpWBForms->Count()>0)
                                            {
                                                DSSAttributeFormWrapper* lpWBForm = lpWBForms->Item(0);
                                                if(lpWBForm)
                                                {
                                                    int lWBBaseFormID = lpWBForm->getBaseFormID();
                                                    DSSTabularAttribute * lpTabularAttribute = dynamic_cast<DSSTabularAttribute *>(lpTabularUnit);
                                                    
                                                    int lColumnID = -1;
                                                    if (lpTabularAttribute && (S_OK == lpTabularAttribute->FindFormWithLocale(0, lWBBaseFormID, 0, lColumnID)))
                                                    {
                                                        mapTrxColumn2WBForm[i] = lpWBForm->getBaseFormID();
                                                    }
                                                }//end of if(lpWBForm)
                                            }
                                        }
                                    }//end of if(lpControl && lpControl->getSourceID() != GUID_NULL) 
                                    break;  //check the next column
                                }
                            }//end of for(int j = 0;j< lnForm;j++)
                        }//end of if(lpForms)
                    }
                }//end of if(lpRowAxis)
            }
		}
		else if (lnMetrics > 0)
		{
            CHECK_PTR(lpTabularMetrics);
            
            DSSTemplateMetrics* lpTemplateMetrics = lpTemplate->getTemplateMetrics();
            
            GUID lMetricID = GUID_NULL;
            lpTabularMetrics->getMetricID(mapTrxColumn2BaseForm[i], lMetricID);
            
            DSSTemplateMetric *lpMetric = lpTemplateMetrics->ItemByGUID(lMetricID);
            if (lpMetric)
            {
                //check DDIC, Data Driven Input Control
                DSSRWControl* lpControl = lpMetric->getInputControl();
                if (lpControl)
                {
                    DSSAttributeFormWrappers* lpWBForms = lpControl->getWriteBackForms();
                    if(lpWBForms && lpWBForms->Count()>0)
                    {
                        //we can't match DDIC for metrics, use a value like -100 to indicate
                        mapTrxColumn2WBForm[i] = -100;
                    }
                }
            }
		}
        
	}
    
	//2, map other columns use default rules:
	for (i = 0; i < lnTargetColumns; i++)
	{
		if (mapTrxColumn2XTabUnit[i] != -1 || mapTrxColumn2BaseForm[i] != -1)										  
			continue;	//already in document level mapping
		
		int lXTabUnitID = -1;
		int lBaseFormID = -1;
        if(lpColumnInfoMap->find(i+1) != lpColumnInfoMap->end())
        {
            DSSColumnMappingInfo *lpColumnInfo = (*lpColumnInfoMap)[i + 1];	//column mapping info is 1-based
            
            if (lpColumnInfo->getType() != DssTypeMetric)
            {
                GUID lUnitID = lpColumnInfo->getAttributeID();
                //lXTabUnitID = this->FindUnit(lUnitID, DssTemplateReserved);
                DSSTabularUnit *lpUnit = ipTemplateNodeModel->FindTabularUnit(lUnitID, DssTemplateReserved);
                if (lpUnit)
                    lXTabUnitID = lpUnit->getUnitIndex();
                
                if (lXTabUnitID >= 0)	//found
                {
                    lBaseFormID = lpColumnInfo->getBaseFormID();
                }
                else
                {
                    lXTabUnitID = -1;
                    lBaseFormID = -1;
                }
                
            }
            else if (lnMetrics > 0)
            {
                GUID lTargetMetricID = lpColumnInfo->getMetricID();
                int lMetricID = lpTabularMetrics->FindDisplayedMetric(lTargetMetricID);
                
                if (lMetricID >= 0)
                    lBaseFormID = lMetricID;
            }
        }
		mapTrxColumn2XTabUnit[i] = lXTabUnitID;
		mapTrxColumn2BaseForm[i] = lBaseFormID;
        
	}
	
	ipTransactionModel->setMapTransactionColumnToMetric(mapTrxColumn2XTabUnit, mapTrxColumn2BaseForm, mapTrxColumn2WBForm);
    
    
    //offline transaction part
    if(!ipCSI)
    {
        return S_OK;
    }
    set<Int32, less<Int32>, MBase::Allocator<Int32> >* lpPlaceHolders = ipCSI->GetPlaceHolderSet();
    if(!lpPlaceHolders)
    {
        return S_OK;
    }
    
    int lnOfflineTargetColumns = lpPlaceHolders->size();
    /*int lnOfflineTargetColumns = ipCSI->GetPlaceHolderCount();*/
    if(lnOfflineTargetColumns <= 0)
    {
        return S_OK;
    }
		
	vector<int> mapOfflineTrxColumn2XTabUnit(lnOfflineTargetColumns, -1);
	vector<int> mapOfflineTrxColumn2BaseForm(lnOfflineTargetColumns, -1);
    vector<int> mapOfflineTrxColumn2WriteBackForm(lnOfflineTargetColumns, -1);
    vector<EnumDSSDataType> mvOfflineTrxDataType(lnOfflineTargetColumns, DssDataTypeReserved);
    vector<int> mapOfflineTrxColumn2TrxColumn(lnOfflineTargetColumns, -1);

	//1, use the document level mapping first:
	for (int iAxis = 0; iAxis < 2; iAxis++)
	{
		DSSXTabHeader *lpHeader = lpView->getHeader(iAxis);
		CHECK_PTR(lpHeader);
        
		int lnHeaderUnits = lpHeader->getNumUnits();
		for (i = 0; i < lnHeaderUnits; i++)
		{
			int lUnitID = lpHeader->getUnit(i);
			AE_ASSERT(lUnitID < lnUnits);
            
            DSSTabularUnit *lpXTabUnit = (*ipvTabularUnits)[lUnitID];
			CHECK_PTR(lpXTabUnit);
            
			if (lpXTabUnit->getUnitType() == DssTemplateAttribute)
			{
				GUID lGUID;
				lpXTabUnit->getGUID(lGUID);
				DSSTemplateUnit *lpUnit = lpTemplate->getTemplateUnitFromAxisIndex(iAxis + 1, lGUID);
                if(!lpUnit)
                    continue;
				DSSTemplateAttribute* lpAttribute = static_cast<DSSTemplateAttribute*> (lpUnit);
                DSSAttributeFormWrappers *lpForms = lpAttribute->getAttribute()->getFormWrappers();
                DSSTabularAttribute* lpTabularAttribute = dynamic_cast<DSSTabularAttribute *>(lpXTabUnit);
                int lnForms = lpForms->Count();
                for (int i = 0; i < lnForms; i++)
                {
                    DSSAttributeFormWrapper *lpForm = lpForms->Item(i);		//465385, 465131
                    int lColumnIndex = lpAttribute->getOfflineColumnIndexByForm(lpForm->getID());
                    lColumnIndex --;	//it's 1-based
                    
                    if (lColumnIndex >= 0 && lColumnIndex < mapOfflineTrxColumn2XTabUnit.size())
                    {
                        mapOfflineTrxColumn2XTabUnit[lColumnIndex] = lUnitID;
                        mapOfflineTrxColumn2BaseForm[lColumnIndex] = lpForm->getBaseFormID();
                        int lDataColumnID = -1;
                        int hr = lpTabularAttribute->FindFormWithLocale(0, lpForm->getBaseFormID(), 0, lDataColumnID);
                        if(hr == S_OK)
                        {
                            DSSDataColumn* lpColumn = NULL;
                            hr = lpTabularAttribute->Item(lDataColumnID, &lpColumn);
                            CHECK_HR;
                            
                            mvOfflineTrxDataType[lColumnIndex] = lpColumn->getDataType();
                        }
                        //check DDIC, Data Driven Input Control
                        DSSRWControl* lpControl = lpAttribute->getInputControlByForm(lpForm->getID());
                        if(lpControl && lpControl->getSourceID() != GUID_NULL)       //there's DDIC defined
                        {
                            //we don't have a match, then use a value like -100 to indicate
                            mapOfflineTrxColumn2WriteBackForm[lColumnIndex] = -100;
                            
                            if(lpControl->getSourceID() == lpUnit->getID())
                            {
                                DSSAttributeFormWrappers* lpWBForms = lpControl->getWriteBackForms();
                                if(lpWBForms && lpWBForms->Count()>0)
                                {
                                    DSSAttributeFormWrapper* lpWBForm = lpWBForms->Item(0);
                                    if(lpWBForm)
                                    {
                                        int lWBBaseFormID = lpWBForm->getBaseFormID();
                                        int lColumnID = -1;
                                        if (lpTabularAttribute && (S_OK == lpTabularAttribute->FindFormWithLocale(0, lWBBaseFormID, 0, lColumnID)))
                                        {
                                            //we got a match, same attribute and same base form
                                            mapOfflineTrxColumn2WriteBackForm[lColumnIndex] = lWBBaseFormID;
                                        }
                                    }//end of if(lpWBForm)
                                }
                            }
                        }//end of if(lpControl)
                    }
                }
                
			}
			else if(lpXTabUnit->getUnitType() == DssTemplateMetrics)
			{
				DSSTemplateUnit *lpUnit = lpTemplate->getTemplateMetrics();
                
                DSSTemplateMetrics* lpTemplateMetrics = static_cast<DSSTemplateMetrics*> (lpUnit);
                int lnMetrics = lpTemplateMetrics->Count();
                for (int i = 0; i < lnMetrics; i++)
                {
                    DSSTemplateMetric *lpMetric = lpTemplateMetrics->Item(i);
                    int lColumnIndex = lpMetric->getOfflineColumnIndex();
                    lColumnIndex --;	//it's 1-based
                    if (lColumnIndex >= 0 && lColumnIndex < mapOfflineTrxColumn2XTabUnit.size())
                    {
                        GUID lGUID = lpMetric->getID();
                        int lMetricID = lpTabularMetrics->FindDisplayedMetric(lGUID);
                        if (lMetricID >= 0)
                        {
                            mapOfflineTrxColumn2XTabUnit[lColumnIndex] = -1;
                            mapOfflineTrxColumn2BaseForm[lColumnIndex] = lMetricID;
                            mvOfflineTrxDataType[lColumnIndex] = lpTabularMetrics->getMetricDataType(lMetricID);
                            //check DDIC, Data Driven Input Control
                            DSSRWControl* lpControl = lpMetric->getInputControl();
                            if (lpControl)
                            {
                                DSSAttributeFormWrappers* lpWBForms = lpControl->getWriteBackForms();
                                if(lpWBForms && lpWBForms->Count()>0)
                                {
                                    //we can't match DDIC for metrics, use a value like -100 to indicate
                                    mapOfflineTrxColumn2WriteBackForm[lColumnIndex] = -100;
                                }
                            }

                        }
                    }
                }
			}
		}
	}
    
   	//2, map other columns use default rules:
	for (i = 0; i < lnOfflineTargetColumns; i++)
	{
		if (mapOfflineTrxColumn2XTabUnit[i] != -1 || mapOfflineTrxColumn2BaseForm[i] != -1)
			continue;	//already in document level mapping
        
        //offline transaction columns must be mapped in hMatchEditableTemplateUnitOfflineTrx
        mapOfflineTrxColumn2XTabUnit.clear();
        mapOfflineTrxColumn2BaseForm.clear();
        mapOfflineTrxColumn2WriteBackForm.clear();
        mvOfflineTrxDataType.clear();
        mapOfflineTrxColumn2TrxColumn.clear();
		return S_OK;
	}
	
    for(i = 0; i < lnOfflineTargetColumns; i++)
    {
        for(int j = 0; j < mapOfflineTrxColumn2XTabUnit.size();j++)
        {
            if(mapOfflineTrxColumn2XTabUnit[i] == mapTrxColumn2XTabUnit[j] && mapOfflineTrxColumn2BaseForm[i] == mapTrxColumn2BaseForm[j])
            {
                mapOfflineTrxColumn2TrxColumn[i] = j;
                break;
            }
        }
    }
	ipTransactionModel->setOfflineTransaction(mapOfflineTrxColumn2XTabUnit, mapOfflineTrxColumn2BaseForm, mapOfflineTrxColumn2WriteBackForm, mvOfflineTrxDataType, mapOfflineTrxColumn2TrxColumn);
    
    return S_OK;
}  
int DSSDataModelXTabImpl::setHeaderSource(DSSHeaderSource* ipSource)
{
    if(mpHeaderSource)
    {
        delete mpHeaderSource;
        mpHeaderSource = NULL;
    }
    mpHeaderSource = ipSource;
    return 0;
}

int DSSDataModelXTabImpl::LoadFromHeaderSource(vector<int>& rvWindowID)
{
	if (mpHeaderSource)
	{
		int hr = mpHeaderSource->LoadHeaderKeys(this, rvWindowID);
		CHECK_HR;
		
	}
	return S_OK;
}
int DSSDataModelXTabImpl::LoadHeaderKeys(DSSBinaryDataReader* ipBinaryReader, int lnKeys, int iAxis, vector<int>& rvWindowID, vector<int>& GBPath, int nRuntimeUnit, vector<int>& MapRuntimeUnit2GBUnit)
{
    int hr = S_OK;
    if(rvWindowID.size() <= 0)
        return S_OK;
    
    DSSXTabHeader* lpHeader = mpView->getHeader(iAxis);
    CHECK_PTR(lpHeader);
    
    int nLevels = lpHeader->getLevelCount();
    if(nLevels == 0)
        return S_OK;
    vector<int> lvRowUnits = lpHeader->getUnits();
    int nGBUnits = mvGBUnits.size();
    int nUnits = lvRowUnits.size();
    
    //first create index unit map
    vector<vector<int> > lvIndexUnitsMap(nLevels);
    
    for (int i = 0; i < nLevels; i++)
	{
        DSSTabularIndex* lpIndex = NULL;
        hr = lpHeader->getIndexByLevel(i, &lpIndex);
        CHECK_HR;
        
        vector<int> rvLevelUnits;
        hr = lpIndex->getUnits(rvLevelUnits);
        CHECK_HR;
        
		int lnIndexUnits = rvLevelUnits.size();	
        
        lvIndexUnitsMap[i].resize(lnIndexUnits);
        
        for (int k = 0; k < lnIndexUnits; k++) {
            int j = 0;
            for (j = 0; j < lvRowUnits.size(); j++) {
                if (rvLevelUnits[k] == lvRowUnits[j])
                {
                    lvIndexUnitsMap[i][k] = j;
                    break;
                }
            }
            if (j >= lvRowUnits.size())
                return E_INVALIDINDEX;
        }
    }
    
    int lStartPos = ipBinaryReader->GetPosition();
    
    vector<vector<int> > lvRowKeys(nLevels);
    vector<vector<int> > lvRowList(nLevels);
    int lnStaticUnits = nUnits - nRuntimeUnit;
    vector<int> lvKeys(nUnits);
    int* lpRowList = iAxis == 0 ? mpRowList : mpColList;
    int* lpRowWindowOffset = iAxis == 0 ? mpRowWindowOffset : mpColWindowOffset;
    set<int> lRowSet;
    
	for(int k = 0; k < rvWindowID.size(); k++)
    {
        //int* lpGBPath = &GBPath[rvWindowID[k] * nGBUnits];
        int * lpGBPath = &GBPath[k * nGBUnits];
        for(int i = lpRowWindowOffset[rvWindowID[k]]; i < lpRowWindowOffset[rvWindowID[k]+1]; i++)
        {
            int lRow = lpRowList[i];
            //handle duplicated rows caused by dummy unit
            if(iAxis == 0 && mDuplicatedRows[iAxis].size() > 0 && mDuplicatedRows[iAxis].find(lRow)!=mDuplicatedRows[iAxis].end())
            {
                if(lRowSet.find(mDuplicatedRows[iAxis][lRow]) == lRowSet.end())
                    lRow = mDuplicatedRows[iAxis][lRow];
                else
                    continue;
            }
            if(lRowSet.find(lRow) != lRowSet.end())
                continue;
            
            lRowSet.insert(lRow);
            
            int lLevel = lpHeader->getLevel(lRow);
            
            if(lnStaticUnits > 0)
            {
                hr = ipBinaryReader->SetPosition(lStartPos + lRow * lnStaticUnits * sizeof(int));
                CHECK_HR;
                
                hr = ipBinaryReader->ReadIntArray(&lvKeys[0], lnStaticUnits); 
                CHECK_HR;
            }
            int* lpDes = &lvKeys[lnStaticUnits];
            for(int j = 0; j < nRuntimeUnit; j++)
            {
                *lpDes = lpGBPath[MapRuntimeUnit2GBUnit[j]];
                if (*lpDes < 0)
                    *lpDes += 10;	//total in group by header start from -11, -12, -13 ... 
                lpDes++;
            }
            
            
            for (int j = 0; j < lvIndexUnitsMap[lLevel].size(); j++)
            {
                lvRowKeys[lLevel].push_back(lvKeys[lvIndexUnitsMap[lLevel][j]]);
            }
            lvRowList[lLevel].push_back(lRow);
        }
    }   
    
    
    //set the keys to their corresponding indices
    for (int i = 0; i < nLevels; i++)
	{
        DSSTabularIndex* lpIndex = NULL;
        hr = mpView->getHeader(iAxis)->getIndexByLevel(i, &lpIndex);
        CHECK_HR;
        
        DSSTabularIndex* lpNextIndex = NULL;
        if(i < nLevels - 1)
        {
            hr = mpView->getHeader(iAxis)->getIndexByLevel(i+1, &lpNextIndex);
            CHECK_HR;
        }
        //504429
        if(lpNextIndex == lpIndex)
        {
            for(int j = 0; j < lvRowList[i].size(); j++)
                lvRowList[i+1].push_back(lvRowList[i][j]);
            
            for(int j = 0; j < lvRowKeys[i].size(); j++)
                lvRowKeys[i+1].push_back(lvRowKeys[i][j]);
        }
        else
        {
            for(int j = 0; j < lvRowList[i].size(); j++)
                lvRowList[i][j] = lpHeader->getOffsetOfIndex(lvRowList[i][j]);
            hr = lpIndex->setHeaderKeys(lvRowKeys[i], lvRowList[i]);
        }
        CHECK_HR;
    }
    return S_OK;
}

int DSSDataModelXTabImpl::setDuplicatedRows(int iAxis, vector<int>& ivDuplicatedRows, vector<int>& ivOrignalRows)
{
    AE_ASSERT(iAxis == 0 || iAxis == 1);
    AE_ASSERT(ivDuplicatedRows.size() == ivOrignalRows.size());
    
    mDuplicatedRows[iAxis].clear();
    for(int i = 0;i < ivDuplicatedRows.size();i++)
        mDuplicatedRows[iAxis][ivDuplicatedRows[i]] = ivOrignalRows[i];
    return S_OK;
}
void DSSDataModelXTabImpl::setMemPageInfo(char* rowList, char* rowWindow, char* colList, char* colWindow, 
                                          int rowListSize, int rowWindowSize, int colListSize, int colWindowSize,
                                          int rowListStart, int rowWindowStart, int colListStart, int colWindowStart,
                                          int rowListOffset, int rowWindowOffset, int colListOffset, int colWindowOffset)
{
    mbRowMapped = true;
    mbColMapped = true;
    mpRowListPage = rowList;
    mpRowWindowPage = rowWindow;
    mpColListPage = colList;
    mpColWindowPage = colWindow;
    mnRowListPage = rowListSize;
    mnRowWindowPage = rowWindowSize;
    mnColListPage = colListSize;
    mnColWindowPage = colWindowSize;
    mRowListPageStart = rowListStart;
    mRowWindowPageStart = rowWindowStart;
    mColListPageStart = colListStart;
    mColWindowPageStart = colWindowStart;
    mRowListPageOffset = rowListOffset;
    mRowWindowPageOffset = rowWindowOffset;
    mColListPageOffset = colListOffset;
    mColWindowPageOffset = colWindowOffset;
}
int DSSDataModelXTabImpl::CreateMappedFileForDataSource()
{
    int hr = S_OK;
    mFilePathMMP = mDataSourcePath + mNodeKey.c_str() + "mmp";
    bool lbCreateNewFile = true;
    if (mpDataSource)
    {
        hr = mpDataSource->createMMapMemory(mFilePathMMP.c_str(), lbCreateNewFile);
        CHECK_HR;
        
        mbDataSourceMapped = !lbCreateNewFile;
    } 
    return hr;
}

int DSSDataModelXTabImpl::hCreateMappedFile()
{
    int hr = S_OK;
    
    mFilePathMMP = mDataSourcePath + mNodeKey.c_str() + "mmp";
    
    bool lbCreateNewFile = !mbDataSourceMapped;
    
    if (mpViewMap)
    {
        hr = mpViewMap->createMMapMemory(mFilePathMMP.c_str(), lbCreateNewFile);
        CHECK_HR;
        
        //lbCreateNewFile = false;
    }
    
    if (mpView)
    {
        hr = mpView->getHeader(0)->createMMapMemory(mFilePathMMP.c_str(), lbCreateNewFile);
        CHECK_HR;
        
        //lbCreateNewFile = false;
        
        hr = mpView->getHeader(1)->createMMapMemory(mFilePathMMP.c_str(), lbCreateNewFile);
        CHECK_HR;
        
    }
    /* map tabular units
     for(int i = 0; i < mvUnits.size(); i++)
     {
     if(mvUnits[i]->getUnitType() == DssTemplateAttribute)
     {
     hr = ((DSSTabularAttribute*)mvUnits[i])->createMMapMemory(mFilePathMMP.c_str(), lbCreateNewFile);
     CHECK_HR;
     
     lbCreateNewFile = false;
     }
     }
     */
    return S_OK;
}

int DSSDataModelXTabImpl::clearRowList(int iAxis)
{
    int hr = 0;
    if(iAxis == 0)
    {
        if(mbRowMapped)
        {
            hr = UnMapSliceWindow(iAxis);
            CHECK_HR;
        }
        else
        {
            if(mpRowList)
            {
                delete mpRowList;
                mpRowList = NULL;
            }
            if(mpRowWindowOffset)
            {
                delete mpRowWindowOffset;
                mpRowWindowOffset = NULL;
            }
            //         int lDataSize = (mnRowList + mnRowWindowOffset) * sizeof(int);
            //         MEM_GOVERNOR_RELEASE(lDataSize, lDataSize)
            mpRowListPage = NULL;
            mpRowWindowPage = NULL;
        }
        mnRowList = 0;
        mnRowListPage = 0;
        mnRowWindowOffset = 0;
        mnRowWindowPage = 0;
    }
    else
    {
        if(mbColMapped)
        {
            hr = UnMapSliceWindow(iAxis);
            CHECK_HR;
        }
        else
        {
            if(mpColList)
            {
                delete mpColList;
                mpColList = NULL;
            }
            if(mpColWindowOffset)
            {
                delete mpColWindowOffset;
                mpColWindowOffset = NULL;
            }
            //        int lDataSize = (mnColList + mnColWindowOffset) * sizeof(int);
            //        MEM_GOVERNOR_RELEASE(lDataSize, lDataSize)
            mpColListPage = NULL;
            mpColWindowPage = NULL;
        }
        mnColList = 0;
        mnColListPage = 0;
        mnColWindowOffset = 0;
        mnColWindowPage = 0;
    }
    return S_OK;
}
int DSSDataModelXTabImpl::setRowList(int iAxis, int* rowList, int nRowList, int* rowOffset, int nRowOffset, bool ibMapped)
{
    if(iAxis == 0)//row axis
    {
        if(ibMapped)
        {
            //TODO: implement
            mbRowMapped = true;
        }
        else
        {
            mnRowList = nRowList;
            mnRowWindowOffset = nRowOffset;
            mpRowList = rowList;
            mpRowWindowOffset = rowOffset;
            mbRowMapped = false;
            mpRowWindowPage = NULL;
            mpRowListPage = NULL;
        }
    }
    else//col axis
    {
        if(ibMapped)
        {
            //TODO: implement
            mbColMapped = true;
        }
        else
        {
            mnColList = nRowList;
            mnColWindowOffset = nRowOffset;
            mpColList = rowList;
            mpColWindowOffset = rowOffset;
            mbColMapped = false;
            mpColWindowPage = NULL;
            mpColListPage = NULL;
        }        
    }
    return S_OK;
}

int DSSDataModelXTabImpl::UnMapSliceWindow(int iAxis)
{
    int hr = S_OK;
    if(iAxis == 0 && mbRowMapped)
    {
        if(mpRowListPage)
        {
            hr = DataSourceUtil::releaseMappedFileBuffer(mpRowListPage, mnRowListPage);
            CHECK_HR;
        }
        
        if(mpRowWindowPage)
        {
            hr = DataSourceUtil::releaseMappedFileBuffer(mpRowWindowPage, mnRowWindowPage);
            CHECK_HR;
        }
        
        mpRowListPage = NULL;
        mpRowList = NULL; 
        mpRowWindowOffset= NULL;
        mpRowWindowPage = NULL;
    }
    else if(iAxis == 1 && mbColMapped)
    {
        if(mpColListPage)
        {
            hr = DataSourceUtil::releaseMappedFileBuffer(mpColListPage, mnColListPage);
            CHECK_HR;
        }
        
        if(mpColWindowPage)
        {
            hr = DataSourceUtil::releaseMappedFileBuffer(mpColWindowPage, mnColWindowPage);
            CHECK_HR;
        }
        
        mpColListPage = NULL;
        mpColList = NULL; 
        mpColWindowPage= NULL;
        mpColWindowOffset = NULL;
    }
    return hr;
}

int DSSDataModelXTabImpl::MapSliceWindow(int iAxis)
{
    int hr = S_OK;
    if(iAxis == 0 && mbRowMapped && mFilePath != "")
    {
        hr = DataSourceUtil::reloadMappedFileBuffer(mFilePath.c_str(), mnRowListPage, mRowListPageOffset, mRowListPageStart, 
                                                    (void**)&mpRowListPage,(void**)&mpRowList);
        CHECK_HR;
        
        hr = DataSourceUtil::reloadMappedFileBuffer(mFilePath.c_str(), mnRowWindowPage, mRowWindowPageOffset, mRowWindowPageStart, 
                                                    (void**)&mpRowWindowPage, (void**)&mpRowWindowOffset); 
        CHECK_HR;
    }
    else if(iAxis == 1 && mbColMapped && mFilePath != "")
    {
        hr = DataSourceUtil::reloadMappedFileBuffer(mFilePath.c_str(), mnColListPage, mColListPageOffset, mColListPageStart, 
                                                    (void**)&mpColListPage,(void**)&mpColList);
        CHECK_HR;
        
        hr = DataSourceUtil::reloadMappedFileBuffer(mFilePath.c_str(), mnColWindowPage, mColWindowPageOffset, mColWindowPageStart, 
                                                    (void**)&mpColWindowPage, (void**)&mpColWindowOffset);
        CHECK_HR;
        
    }
    return hr;
}

int DSSDataModelXTabImpl::LoadInputControlModel(std::string iKey, DSSDataModelControlImpl** oppControl)
{
    return  mpModelDataSource->LoadInputControlModel(this, iKey, oppControl);
}

DSSTabularUnit* DSSDataModelXTabImpl::FindTabularUnit(const GUID &irGUID, EnumDSSTemplateUnitType iType)
{
    int lUnitIndex = FindUnit(irGUID, iType);
    DSSTabularUnit* lpUnit = Item(lUnitIndex);	
    return lpUnit;
}

DSSTemplate *DSSDataModelXTabImpl::getTemplate()
{
    return mpTemplate;
}
