/*
 *  DSSXTabHeader.cpp
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import "DSSXTabHeader.h"
#import "DSSDataModelXTabImpl.h"
#import "DSSTabularMetrics.h"
#import "DSSTabularIndex.h"
#import "DSSTabularCustomGroup.h"
#import "DSSXTabView.h"
#import "DSSDataSource.h"

#include "DSSRWGridModel.h"
#include "DSSViewDataSet.h"

#include "DSSTabularData.h"
#include "DSSTabularConsolidation.h"
#include "DSSXTabRecursiveUnit.h"
#include "DSSMaskInfos.h"

DSSXTabHeader::DSSXTabHeader():mnDispRows(0), mnRows(0), mnLevels(0), mnUnits(0), mnTabularUnits(0)
,mbHasMetric(false),mMetricsUnitID(-1),mnMetrics(0),mbIsNewRWD(false),mHasUnqualifiedRows(false)
,mpView(NULL), mnRuntimeUnits(0), mbHasGBMetric(false), mnDispUnits(-1),mnConsolidationUnit(0)
,mDummyUnitIndex(-1), mpCurDispRowMap(NULL),mMapIndexToTarget(NULL)
//,mpConsolidationUnit(NULL),mpConsolidation(NULL)
,mpLevelID(NULL),mpMMPBuffer(NULL), mMMPBufSize(0)
{
    // 8/13/2012 tehe
    mbPreXtab = false;
    mbInitialized = false;
    mnCurRow = 0;
    mnCurDispRow = 0;
    mbOffsetFlag = true;
    mReserveMemorySize = 0;
}

DSSXTabHeader::~DSSXTabHeader()
{
	mvOffset.clear();
    
	int lDataSize = mnRows * sizeof(short);
	if (mpMMPBuffer)
    {
        DataSourceUtil::releaseMappedFileBuffer(mpMMPBuffer, mMMPBufSize);
        mpMMPBuffer = NULL;
    }
    else
    {
        //mvLevelID.clear();
        delete [] mpLevelID;
        MEM_GOVERNOR_RELEASE(lDataSize, lDataSize)
    }
	
	mvDispRows.clear();
    
    if(mReserveMemorySize > 0)
    {
        MEM_GOVERNOR_RELEASE(mReserveMemorySize, mReserveMemorySize);
        mReserveMemorySize = 0;
    }
    
    mvConsolidation.clear();
    mvConsolidationUnit.clear();
    
    mvRecursiveAttr.clear();
    mvRecursiveUnits.clear();
    
    // 8/13/2012 tehe
    if(mpCurDispRowMap)
    {
        delete [] mpCurDispRowMap;
        mpCurDispRowMap = NULL;
    }
    
    // 8/9/2012 tehe
    if(mMapIndexToTarget)
    {
        map<DSSTabularIndex*, int *>::iterator it = mMapIndexToTarget->begin();
        while (it != mMapIndexToTarget->end())
        {
            int *validSegment  = it->second;
            if(validSegment)
            {
                delete [] validSegment;
                validSegment = NULL;
            }
            it ++;
        }
        mMapIndexToTarget->clear();
        delete mMapIndexToTarget;
        mMapIndexToTarget = NULL;
    }
    
}

int DSSXTabHeader::GetCurrentGBAxisRange(vector<int>& orRows)
{
    
    int lnSize = 0;
    if (mpCurDispRowMap == NULL)
    {
        if (mbHasGBMetric && mbHasMetric)
		{
			vector<bool> lvMetricInSelector;
			lvMetricInSelector.resize(mnMetrics);
			lvMetricInSelector.assign(mnMetrics, false);
			int lnSelectedMetrics = 0;
			for(int i = 0; i < mapToMetricID.size(); i++)
			{
				if (mapToMetricID[i] < mnMetrics && mapToMetricID[i] >= 0)
				{
					lvMetricInSelector[mapToMetricID[i]] = true;
					lnSelectedMetrics++;
				}
			}
            
			if (lnSelectedMetrics > 0)
			{
                // 8/10/2012 tehe: sorted by metric index
                orRows.resize(mnRows * lnSelectedMetrics);
				for (int i = 0 ; i < mnDispRows; i++)
				{
					if (lvMetricInSelector[/*mvDispRows[i]*/i%mnMetrics])
					{
						orRows[lnSize++] = i;
					}
				}
			}
		}
		else{
            orRows.resize(mnDispRows);
            // memmove(&orRows[0], &mvDispRows[0], sizeof(int) * mnDispRows);
            // tehe
            for(int i=0;i<mnDispRows;i++)
                orRows[i] = i;
		}
    }
    else
    {
        orRows.resize(mnCurDispRow);
        memmove(&orRows[0], mpCurDispRowMap, sizeof(int) * mnCurDispRow);
        
    }
    
    return S_OK;
    
}

int DSSXTabHeader::FillIndexEx(int iRow, int nUnit, int *ipKeys, int *pMetric, DSSTabularIndex **oppIndex, int *opOffset, int *ipChildKey)
{
    int hr = S_OK;
    
    //xhan 11/04/04 make sure iRow is within the range
    AE_ASSERT(iRow < mnDispRows);
    
    if(mbHasMetric)
    {
        *pMetric = iRow % mnMetrics;
        iRow = iRow / mnMetrics;
    }
    
    int lOffset = 0;
    if(mbOffsetFlag)
    {
        lOffset = mvOffset[iRow];
    }
    else
    {
        lOffset = iRow - mvLevelOffset[mvLevelID[iRow]];
    }
    
    mvIndices[mvLevelID[iRow]]-> Item(lOffset, nUnit, ipKeys);
    // The caller is interested in the actual index object and the offset
    if (oppIndex != NULL)
    {
        //for financial report
        int lnUnits = 0;
        lnUnits = mvIndices[mvLevelID[iRow]]->getNumUnits();
        int *lpUnits = new int[lnUnits];
        hr = mvIndices[mvLevelID[iRow]]->getUnits(lnUnits, lpUnits);
        if (hr != S_OK)
        {
            delete [] lpUnits;
        }
        CHECK_HR
        
        if(mnRecursiveUnits>0 && ipChildKey)
        {
            for (int i=0; i<lnUnits; i++) {
                int j = 0;
                for (; j<mnRecursiveUnits; j++)
                {
                    if (lpUnits[i] == mvRecursiveUnits[j]) {
                        //get child unit index
                        int lUnit = mvRecursiveAttr[j]->getDataUnit();
                        //if find, use data index to replace the display index
                        mvRecursiveAttr[j]->TransferDisplay2DataKey(ipKeys[lpUnits[i]], ipChildKey[lUnit]);
                        break;
                        
                    }
                }
                
                //if not find, then it's normal attribute
                if (j>= mnRecursiveUnits) {
                    ipChildKey[lpUnits[i]] = ipKeys[lpUnits[i]];
                }
            }
            
            *oppIndex = mvIndices2[mvLevelID[iRow]];
            mvIndices2[mvLevelID[iRow]]->LookUp(nUnit, ipChildKey, opOffset);
        }
        else {
            
            if (ipChildKey) {
                for (int i =0; i<lnUnits; i++) {
                    ipChildKey[lpUnits[i]] = ipKeys[lpUnits[i]];
                }
            }
            *oppIndex = mvIndices[mvLevelID[iRow]];
            *opOffset = lOffset;
        }
        
        delete [] lpUnits;
    }
    
    return hr = S_OK;
}

/*
 DE25447: To be used in CDSSAttributeThresholdIterator.
 Input
	iRow: the header ordinal of the metric cell (or dummy metric cell if there is no metric)
 Output
	pIsFirstorNonMetricCell:  (*pIsFirstorNonMetricCell) is set to true if the current cell is not associated with any metric (i.e., a dummy cell in a grid with empty metric),
 or the cell is associated with the first metric (so we iterate metric cells with the same index only once)
 */
int DSSXTabHeader::isFirstMetricCell(int iRow, bool* pIsFirstorNonMetricCell)
{
    HRESULT hr = S_OK;
    
    //xhan 11/04/04 make sure iRow is within the range
    AE_ASSERT(iRow < mnDispRows);
    
    *pIsFirstorNonMetricCell = true;
    if (mbHasMetric)
    {
        if (iRow % mnMetrics != 0)  // the current cell is not associated with the first metric
            *pIsFirstorNonMetricCell = false;
    }
    
    return hr;
}

int DSSXTabHeader::InitBySelection(int nIndices, DSSTabularIndex** pIndices, DSSXTabBitVector** pBitVectors, int iHasMetricGroupBy, vector<int> ivMetrics, vector<int> *pTags)
{
    int hr = S_OK;
    if(!mbInitialized)
    {
        hr = init();
        CHECK_HR;
    }
    
	int iLevel = 0,  lnRowInLevel = 0 ;
	int iRow = 0;
    
    
	hr = hCountConsolidationAndRecursiveUnits();
	CHECK_HR;
    
	bool lDisplayHiddenRows = false;
    
    hr = hInitMetrics(iHasMetricGroupBy, ivMetrics);
	CHECK_HR;
    
	hr = hResetSlicingBuffer();
    CHECK_HR;
    
	vector<int> lvStartRow(mnLevels + 1, 0);
	int lnMatchCount = 0;
    
	vector<int> lvMatchIndices;
	vector<int> lvMatchLevel;
	int lEstimatedCount = 0;
	int lIndex = -1;
	for(iLevel = 0; iLevel < mnLevels; iLevel ++)
	{
		lnRowInLevel = mvIndices[iLevel] -> Count();
        
		int* pMap = NULL;
        // 8/9/2012 tehe: if the index has unqulified rows or undisplayable rows, we should get the actual valid row count from mapindextotarget
		if(mMapIndexToTarget->find(mvIndices[iLevel]) != mMapIndexToTarget->end())
		{
			pMap = mMapIndexToTarget->find(mvIndices[iLevel])->second;
			lnRowInLevel = pMap[1];
		}
		lvStartRow[iLevel + 1] = lvStartRow[iLevel] + lnRowInLevel;
        
		lIndex = -1;
        
        // 8/9/2012 tehe: according the input index + pTags info, find the coresponding level. keep in mind that level means index + tag
		for (int j = 0; j < nIndices; j++)
		{
			if (mvIndices[iLevel] == pIndices[j])
			{
				if( (pTags[j].size() == 1 && pTags[j][0] == -1) || mvSubtotalID[iLevel] == 0)
				{
					lIndex = j;
					break;
				}
				else
				{
					int iTag = 0;
					for (iTag = 0; iTag < pTags[j].size(); iTag++)
					{
						if (mvSubtotalID[iLevel] == pTags[j][iTag])
							break;
					}
					if(iTag < pTags[j].size())
					{
						lIndex = j;
						break;
					}
				}
                
			}
		}
        
		if (lIndex < 0 || pBitVectors[lIndex] == NULL) continue;
		int lSetSize = pBitVectors[lIndex]->GetSetSize();
		if (lSetSize > 0)
		{
			lvMatchLevel.push_back(iLevel);
			lvMatchIndices.push_back(lIndex);
			lnMatchCount++;
			lEstimatedCount += lSetSize;
		}
	}
    
	// reserve necessary size for vectors;
	if (lEstimatedCount > 0)
	{
        mvCurRow.resize(lEstimatedCount);
	}
	int iStartRow = 0;
	int iMetric = 0;
	int lTargetPos = 0;
	
	int lnTotalRow = mnDispRows;    // 8/10/2012 tehe: I think we should this value when gb metrics are changed.
	DSSXTabBitVector lSelectedBV(lnTotalRow);
	
	int lCurRow = 0;
	int lnMetrics = (mbHasGBMetric && iHasMetricGroupBy) ? mapToMetricID.size() : mnMetrics;
	
    DSSMaskInfos *lpMaskInfo = mpView->getHideNullZeroMaskInfo();
    
	for (int i = 0; i < lnMatchCount; i++)
	{
		iLevel = lvMatchLevel[i];
		lIndex = lvMatchIndices[i];
		pBitVectors[lIndex]->SetSeekPosition(0);
		iRow = pBitVectors[lIndex]->GetNextSetPosition();
		int lnIndexRow = mvIndices[iLevel]->Count();
        
        DSSXTabBitVector *lpBitVector = NULL;
        if(lpMaskInfo && lpMaskInfo->GetMainIndexMask() != NULL && iLevel == 0)
        {
            hr = lpMaskInfo->GetCalculatedMask2(mvIndices[iLevel], &lpBitVector);
            CHECK_HR;
        }
        
		// rzhu, find the mapping targets for the index contains hidden rows
		int* pMap = NULL;
		if(mMapIndexToTarget->find(mvIndices[iLevel]) != mMapIndexToTarget->end())
		{
			pMap = mMapIndexToTarget->find(mvIndices[iLevel])->second;
		}
		int lnSeg = pMap == NULL ? 0 : pMap[0];
		int lnTotalCount = pMap == NULL ? 0 : pMap[1];
		int lCurSeg = 0;
		int lnDispRows = 0;
		int lDispRow = 0;
		int iSeg = 0;
        
		iStartRow = lvStartRow[iLevel];
		while(iRow > -1 && iRow < lnIndexRow)
		{
			bool lfDisplay = true;
			if ((mnConsolidationUnit > 0 || mHasUnqualifiedRows || lpBitVector) && !lDisplayHiddenRows)
			{
                if(lpBitVector && !lpBitVector->Get(iRow))
                {
                    lfDisplay = false;
                }
                
                if(lfDisplay && (mnConsolidationUnit > 0 || mHasUnqualifiedRows))
                {
                    //the complexity caused by redundant consolidation index
                    hr = hCheckDisplayable(iRow, iLevel, lfDisplay);
                    CHECK_HR;
                }
			}
            
			if(lfDisplay) {
                
                if (pMap == NULL)
				{
					lDispRow = iRow;
				}
				else
				{
                    // 8/13/2012 tehe: find related index in the segment
					if (lnSeg > 0)
					{
						for(iSeg = lCurSeg; iSeg < lnSeg; iSeg++)
						{
							if(iRow > pMap[iSeg * 2 + 2] + pMap[iSeg*2 + 3] -1)
							{
								lnDispRows += pMap[iSeg*2 + 3];
								continue;
							}
							else if(iRow < pMap[iSeg*2 + 2] + pMap[iSeg*2 + 3])
							{
								lCurSeg = iSeg;
								lDispRow = lnDispRows + iRow - pMap[iSeg*2 + 2];
								break;
							}
						}
					}
					else
					{
						for(iSeg = lCurSeg; iSeg < lnTotalCount; iSeg++)
						{
							if(iRow == pMap[iSeg + 2])
							{
								lCurSeg = iSeg;
								lDispRow = iSeg;
							}
						}
					}
				}
				mvCurRow[lCurRow++] = iStartRow + lDispRow;
                
                if (!mbHasMetric)
                {
                    lTargetPos = iStartRow + lDispRow; /*mvDispRows[iStartRow + lDispRow];*/
                    lSelectedBV.Set(lTargetPos, true);
                }
                else if (lnMetrics == mnMetrics)
                {
                    for(iMetric = 0; iMetric < mnMetrics; iMetric++)
                    {
                        lTargetPos = (iStartRow + lDispRow) * mnMetrics + iMetric; /*mvDispRows[(iStartRow + lDispRow) * mnMetrics + iMetric];*/
                        lSelectedBV.Set(lTargetPos, true);
                    }
                }
                else
                {
                    for(iMetric = 0; iMetric < lnMetrics; iMetric++)
                    {
                        lTargetPos = (iStartRow + lDispRow) * mnMetrics + mapToMetricID[iMetric]; /*mvDispRows[(iStartRow + lDispRow) * mnMetrics + mapToMetricID[iMetric]];*/
                        lSelectedBV.Set(lTargetPos, true);
                    }
                }
			}
            
			iRow = pBitVectors[lIndex]->GetNextSetPosition();
		}// end row
        
	} //level
    
	mnCurRow = lCurRow;
    
    
    // 8/10/2012 tehe: we don't need pre-sort here
    if(!mpCurDispRowMap)
    {
        //it is called from Init(), we need to the following processing
        if(!mbHasMetric)
            mnCurDispRow = mnCurRow;	//all the row will be displayed
        else
            mnCurDispRow = mnCurRow * lnMetrics;
        
        // a special case, has metric lable in this axis, but there is no metric in the data
        if(mbHasMetric == true && mnMetrics == 0)
        {
            mbHasMetric = false;
            mnCurDispRow = mnCurRow;
        }
        
        mpCurDispRowMap = new int[mnCurDispRow];
        
        int lNewTarget = 0;
        lSelectedBV.SetSeekPosition(0);
        iRow = lSelectedBV.GetNextSetPosition();
        
        while(iRow > -1 && iRow < lnTotalRow)
        {
            mpCurDispRowMap[lNewTarget++] = iRow;
            iRow = lSelectedBV.GetNextSetPosition();
        }
    }
    
	hr = CreateUnitMappingTable();
	CHECK_HR;
    
	return hr = S_OK;
}

int DSSXTabHeader::InitMetricGroupBy(int iHasMetricGroupBy, vector<int> ivMetrics)
{
    
	if (!iHasMetricGroupBy || !mbHasMetric)
		return S_OK;
    
    //no attribute selector, has metric selector only
	if (mnCurRow == 0)
    {
		int hr = hResetSlicingBuffer();
		CHECK_HR;
        
		// 8/10/2012 tehe
        if(!mbInitialized)
        {
            hr = init();
            CHECK_HR;
        }
		
        hr = hInitMetrics(iHasMetricGroupBy, ivMetrics);
		CHECK_HR;
        
		int lnTotalRow = mnDispRows;
		DSSXTabBitVector lSelectedBV(lnTotalRow);
		int lnMetrics = mapToMetricID.size();
		int i = 0, iMetric = 0;
		int lTargetPos;
        
		for(i = 0; i < mnRows; i++)
		{
			if (!mbHasMetric)
			{
				lSelectedBV.Set(/*mvDispRows[i]*/i, true);
			}
			else if (lnMetrics == mnMetrics)
			{
				lTargetPos = i * mnMetrics;
				for(iMetric = 0; iMetric < mnMetrics; iMetric++)
				{
					lSelectedBV.Set(/*mvDispRows[lTargetPos + iMetric]*/lTargetPos + iMetric, true);
				}
			}
			else
			{
				lTargetPos = i * mnMetrics;
				for(iMetric = 0; iMetric < lnMetrics; iMetric++)
				{
					lSelectedBV.Set(/*mvDispRows[lTargetPos + mapToMetricID[iMetric]]*/lTargetPos + mapToMetricID[iMetric], true);
				}
			}
		}
		if(!mbHasMetric)
			mnCurDispRow = mnRows;	//all the row will be displayed
		else
			mnCurDispRow = mnRows * lnMetrics;
        
		//a special case, has metric lable in this axis, but there is no metric in the data
		if(mbHasMetric == true && mnMetrics == 0)
		{
			mbHasMetric = false;
			mnCurDispRow = mnRows;
		}
		
        mpCurDispRowMap = new int [mnCurDispRow];
		int iRow = 0;
        
        int lNewTarget = 0;
        lSelectedBV.SetSeekPosition(0);
        iRow = lSelectedBV.GetNextSetPosition();
        
        while(iRow > -1 && iRow < lnTotalRow)
        {
            mpCurDispRowMap[lNewTarget++] = iRow;
            iRow = lSelectedBV.GetNextSetPosition();
        }
        
        return S_OK;
	}
    // 8/10/2012 tehe: there is attribute selector and metric selector, and only metric value is changed
    
    // tehe: here we needn't checking initialize because it must be alreay initialized, and we can't resetslicebuffer, we need use it
    int hr = hInitMetrics(iHasMetricGroupBy, ivMetrics);
    CHECK_HR;
    
    int lnTotalRow = mnDispRows;
    DSSXTabBitVector lSelectedBV(lnTotalRow);
    int lnMetrics = mapToMetricID.size();
    int i = 0, iMetric = 0;
    int lTargetPos;
    for(i = 0; i < mnCurRow; i++)
    {
        if (!mbHasMetric)
        {
            lSelectedBV.Set(/*mvDispRows[mvCurRow[i]]*/mvCurRow[i], true);
        }
        else if (lnMetrics == mnMetrics)
        {
            lTargetPos = mvCurRow[i] * mnMetrics;
            for(iMetric = 0; iMetric < mnMetrics; iMetric++)
            {
                lSelectedBV.Set(/*mvDispRows[lTargetPos + iMetric]*/lTargetPos + iMetric, true);
            }
        }
        else
        {
            lTargetPos = mvCurRow[i] * mnMetrics;
            for(iMetric = 0; iMetric < lnMetrics; iMetric++)
            {
                lSelectedBV.Set(/*mvDispRows[lTargetPos + mapToMetricID[iMetric]]*/lTargetPos + mapToMetricID[iMetric], true);
            }
        }
    }
    if(!mbHasMetric)
        mnCurDispRow = mnCurRow;	//all the row will be displayed
    else
        mnCurDispRow = mnCurRow * lnMetrics;
    
    //a special case, has metric lable in this axis, but there is no metric in the data
    if(mbHasMetric == true && mnMetrics == 0)
    {
        mbHasMetric = false;
        mnCurDispRow = mnCurRow;
    }
    
    // 8/10/2012 tehe: we need clear mvCurDispRowMap here
    if(mpCurDispRowMap)
    {
        delete [] mpCurDispRowMap;
        mpCurDispRowMap = NULL;
    }
    mpCurDispRowMap  = new int[mnCurDispRow];
    int iRow = 0;
    int lNewTarget = 0;
    lSelectedBV.SetSeekPosition(0);
    iRow = lSelectedBV.GetNextSetPosition();
    
    while(iRow > -1 && iRow < lnTotalRow)
    {
        mpCurDispRowMap[lNewTarget++] = iRow;
        iRow = lSelectedBV.GetNextSetPosition();
    }
    
    return S_OK;
	
}

int DSSXTabHeader::init()
{
    if (mbInitialized)
    {
        return S_OK;
    }
    
    mbIsNewRWD = true;
    
   	int iLevel = 0,  lnRowInLevel = 0 ;
	int iRow = 0;
    
    // tehe 2011-11-21 special handing for consolidation
    int hr = hCountConsolidationAndRecursiveUnits();
    CHECK_HR;
    
    
    // tehe 2011-11-21 I am still not clear about meaning of hidden row, so here I will suppose it not display hidden rows
	bool lDisplayHiddenRows = false;
    
	// tehe: clear the used value
	hr = hResetXTabBuffer();
	CHECK_HR;
    
    // 8/9/2012 tehe: enable pre xtab
	bool lbDoPreXtab = mpView->IsDoPreXtab();
    
	bool lbBuildMap = false;
    
    mbOffsetFlag = hGetOffsetFlag();
    
    DSSMaskInfos *lpMaskInfo = mpView->getHideNullZeroMaskInfo();
    
    int lReserveSize = 0;
    for(iLevel = 0; iLevel < mnLevels; iLevel ++)
    {
        lReserveSize += mvIndices[iLevel]->Count();
    }
    if(mbOffsetFlag)
        lReserveSize = lReserveSize *(sizeof(short)+sizeof(int));
    else
        lReserveSize = lReserveSize * sizeof(short);
    
    mReserveMemorySize = lReserveSize;
    
    MEM_GOVERNOR_RESERVE(lReserveSize, lReserveSize)
    MEM_GOVERNOR_COMMIT(lReserveSize);
    
    //Financial report, to count the displaying rows for each level
    //Risks?
    vector<int> lDispRowsInLevel;
    lDispRowsInLevel.resize(mnLevels);
    
	for(iLevel = 0; iLevel < mnLevels; iLevel ++)
	{
        
		lbBuildMap = false;
        // 8/9/2012 tehe: if the index contains consolidation unit or unqulified rows, we need build index map for it
		if( lbDoPreXtab && !lDisplayHiddenRows)
		{
			lbBuildMap = mHasUnqualifiedRows;
			if (!lbBuildMap)
			{
				// if contains consolidation unit
				vector<int> lvUnits;
				int lnUnits = 0;
				lnUnits = mvIndices[iLevel]->getNumUnits();
				if(lnUnits > 0)
				{
					lvUnits.resize(lnUnits);
					hr = mvIndices[iLevel]->getUnits(lnUnits, &lvUnits[0]);
					CHECK_HR;
				}
				for(int iUnit2 = 0; iUnit2 < mnConsolidationUnit; iUnit2++)
				{
					for(int iUnit = 0; iUnit < lnUnits; iUnit++)
					{
						if(lvUnits[iUnit] == mvConsolidationUnit[iUnit2])
						{
							lbBuildMap = true;
							break;
						}
					}
					if(lbBuildMap)
						break;
				}
				
			}
		}
        
        // tehe:support hide null or zero feature
        DSSXTabBitVector *lpBitVector = NULL;
        
        if(lpMaskInfo && lpMaskInfo->GetMainIndexMask() != NULL && iLevel == 0)
        {
            hr = lpMaskInfo->GetCalculatedMask2(mvIndices[iLevel], &lpBitVector);
            CHECK_HR;
            
            lbBuildMap = true;
        }
        
		if(lbBuildMap && mMapIndexToTarget->find(mvIndices[iLevel]) != mMapIndexToTarget->end())
		{
			lbBuildMap = false;
		}
        
        // 8/9/2012 tehe: build index map for index which has unqulified rows or undisplay rows, use lvmap remember the position of segment, each row in this segment is valid and can be display  (if this row is not filted by slice)
		vector<int> lvMap;
		lvMap.push_back(0); // tehe: reserve the first two position of vector
		lvMap.push_back(0);
		int lnSeg = 0;
		int lnLast = -1;
		int lnCount = 0;
		int lnTotalCount = 0;
        
		lnRowInLevel = mvIndices[iLevel] -> Count();
        
        bool lfHasRecursiveUnit = (mvHasRecursiveUnit.size()>iLevel) && mvHasRecursiveUnit[iLevel];
        
        // 9/5/2012 tehe: optimize if there's no consolidation units or unqulified rows
        if((mnConsolidationUnit > 0 || mnRecursiveUnits>0 ||mHasUnqualifiedRows || lpBitVector) && !lDisplayHiddenRows)
        {
            for(iRow = 0; iRow < lnRowInLevel; iRow ++)
            {
                bool lfDisplay = true;
                
                if(lpBitVector && !lpBitVector->Get(iRow))
                {
                    lfDisplay = false;
                }
                
                //de48140, msun, add a new mask on display index.
                if(mnRecursiveUnits > 0 && iLevel == 0)
                {
                    DSSXTabBitVector *lpHiddenRows = mvIndices[iLevel]->GetHiddenRows();
                    if (lpHiddenRows && !lpHiddenRows->Get(iRow)) {
                        lfDisplay = false;
                    }
                }
                
                if(lfDisplay && (mnConsolidationUnit > 0 || mHasUnqualifiedRows || lfHasRecursiveUnit))
                {
                    hr = hCheckDisplayable(iRow, iLevel, lfDisplay);
                    CHECK_HR;
                }
                
                if(lfDisplay) {
                    if(lbBuildMap)
                    {
                        // 8/9/2012 tehe: new segment
                        if (lnLast == -1 || lnLast +1 != iRow)
                        {
                            // 8/10/2012 tehe: a new level begins,and the level has unqulified rows or undisplay rows
                            if(lnLast == -1)
                            {
                                lvMap.push_back(iRow); // tehe: iRow the first valid row in the level
                            }
                            else
                            {
                                lnTotalCount += lnCount;
                                lvMap.push_back(lnCount);
                                lvMap.push_back(iRow);
                            }
                            lnSeg++;
                            lnCount = 0;
                        }
                        lnLast = iRow;
                        lnCount++;
                    }
                    // 8/13/2012 tehe
                    mvOffset.push_back(iRow);
                    mvLevelID.push_back(iLevel);
                    lDispRowsInLevel[iLevel]++;
                }
            }
        }
        else
        {
            if(lnRowInLevel > 0 && mbOffsetFlag)
            {
                int lCount = mvLevelID.size();
                
                mvLevelID.resize(lCount + lnRowInLevel, iLevel);
                
                
                mvOffset.resize(lCount+lnRowInLevel);
                for(int i=0 ; i < lnRowInLevel; i++)
                {
                    mvOffset[lCount++] = i;
                }
            }
            lDispRowsInLevel[iLevel] = lnRowInLevel;
        }
        
		if(lbBuildMap)
		{
			int *pMap = NULL;
            vector<int> lvResultMap;
			lnTotalCount += lnCount;
			if (lnCount > 0)
				lvMap.push_back(lnCount);
			if(lnTotalCount > lnSeg * 2)
			{
				lvMap[0] = lnSeg;
				lvMap[1] = lnTotalCount;
                
				pMap = new int[lnSeg * 2 + 2];
				memmove(pMap, &lvMap[0], sizeof(int) * (lnSeg*2 + 2));
			}
			else
			{
				pMap = new int[lnTotalCount + 2];
				pMap[0] = 0;
				pMap[1] = lnTotalCount;
				int nCount = lvMap.size();
				int iRow = 2;
				for(int i = 2; i < nCount; i+=2)
				{
					for(int j = 0; j < lvMap[i+1]; j++)
						pMap[iRow++] = lvMap[i] + j;
				}
			}
			mMapIndexToTarget->insert(pair<DSSTabularIndex*, int*>(mvIndices[iLevel], pMap));
		}
        
	} //level
    
    if(!mbOffsetFlag)
    {
        mvLevelOffset.resize(mnLevels);
        
        int lCount = 0;
        for (int i = 0; i < mnLevels; i++)
        {
            mvLevelOffset[i] = lCount;
            
            //int lnRows = mvIndices[i]->Count();
            int lnRows = lDispRowsInLevel[i];
            mvLevelID.resize(lCount+lnRows, i);
            
            lCount += lnRows;
        }
    }
    
	mnRows = mvLevelID.size();
    
    if(!mbHasMetric)
        mnDispRows = mnRows;	//all the row will be displayed
    else
        mnDispRows = mnRows * mnMetrics;
    
    //a special case, has metric lable in this axis, but there is no metric in the data
    if(mbHasMetric == true && mnMetrics == 0)
    {
        mbHasMetric = false;
        mnDispRows = mnRows;
    }
    
	hr = CreateUnitMappingTable();
	CHECK_HR;
    
	mbInitialized = true;
    
	return hr = S_OK;
}


int DSSXTabHeader::CreateUnitMappingTable()
{
	int iUnit;
    int hr = S_OK;
    
    DSSViewDataSet *lpViewDataSet = mpView->GetViewDataSet();
    CHECK_PTR(lpViewDataSet);
    
    
	DSSTabularData * lpTabularData = NULL;
	hr = lpViewDataSet -> getTabularData(&lpTabularData);
	CHECK_HR;
    
    
	EnumDSSTemplateUnitType lUnitType;
	DSSTabularUnit * lpTabularUnit = NULL;
    
    // tehe 4/17/2012: avoid memory leak
    mvUnitDepth.clear();
    
	int lBeginDepth = 0;
    int lStartDepth = 0;
    int lCount;
	int lMaxDepth;
	
	int lnDispUnit = mnDispUnits > -1 ? mnDispUnits : mnUnits;
	vector<int> lvDispUnit = mnDispUnits > -1 ? mvDispUnits : mvUnits;
	for (iUnit = 0; iUnit < lnDispUnit; iUnit++)
	{
		hr = lpTabularData -> Item(lvDispUnit[iUnit], &lpTabularUnit);
		CHECK_HR;
        
		lUnitType = lpTabularUnit -> getUnitType();
		CHECK_HR;
        
        
		if(lUnitType == DssTemplateCustomGroup)
		{
			DSSTabularCustomGroup *lpCustomGroup;
			lpCustomGroup = static_cast<DSSTabularCustomGroup *>(lpTabularUnit);
			lMaxDepth = lpCustomGroup -> getMaxDepth();
			CHECK_HR;
			// The header node of a custom group structure will not be displayed
			// So the Actual depth of displayed elements is lMaxDepth - 1
			lMaxDepth --;
		}
		else	// Non-custom group
			lMaxDepth = 0;
        
        lCount = lMaxDepth + 1;
        lStartDepth = lBeginDepth;
        mvUnitDepth.push_back(lStartDepth);
		lBeginDepth = lBeginDepth + lCount;
	}
    mvUnitDepth.push_back(lBeginDepth);
	
	mHeaderWidth = lBeginDepth;
	
    return S_OK;
}

int DSSXTabHeader::hResetXTabBuffer()
{
    mvOffset.clear();
    mvLevelID.clear();
    
    if(mReserveMemorySize > 0)
    {
        MEM_GOVERNOR_RELEASE(mReserveMemorySize, mReserveMemorySize);
        mReserveMemorySize = 0;
    }
    
	mnDispRows = 0;
	// mvDispRows.clear();
    
    mvUnitDepth.clear();
    
    // 8/9/2012 tehe
    if(mMapIndexToTarget)
    {
        map<DSSTabularIndex*, int *>::iterator it = mMapIndexToTarget->begin();
        while (it != mMapIndexToTarget->end())
        {
            int *validSegment  = it->second;
            if(validSegment)
            {
                delete [] validSegment;
                validSegment = NULL;
            }
            it ++;
        }
        mMapIndexToTarget->clear();
        delete mMapIndexToTarget;
        mMapIndexToTarget = NULL;
    }
    
    mMapIndexToTarget = new map<DSSTabularIndex*, int *>();
    
    // 3/22/2012 tehe
    if(mnConsolidationUnit >0)
    {
        mvEnableTotalForCS.clear();
        mvEnableTotalForCS.assign(mnConsolidationUnit,false);
        
        mvTotalStateForCS.clear();
        mvTotalStateForCS.assign(mnConsolidationUnit, false);
    }
    
	return S_OK;
}

// tehe: notice that nAttrUnit means the total unit number in tabular data
int DSSXTabHeader::Init(DSSXTabView *pView, int nLevel, int *pTag, DSSTabularIndex **ppIndices, int nColUnit, int *pColUnit, int nAttrUnit, bool fMetric, int nMetric, int iRumtimeUnitCount, DSSTabularIndex **ppChildIndices, vector<bool> & irvHasRecursiveUnit)
{
    mnRuntimeUnits = iRumtimeUnitCount;
    
    mbIsNewRWD = true;
    
	int * lpUnique = new int[nLevel];
    MBase::StrongPtr<int,MBase::DeleteArray<int> > lUniqueStrongPtr(lpUnique);
    
	int lnUnique = 0;
	bool lfMatch = false;
    
	if (nLevel > 0){
		AE_ASSERT(pTag != NULL);
		AE_ASSERT(ppIndices != NULL);
	}
    
	if (nColUnit > 0)
		AE_ASSERT(pColUnit != NULL);
    
	int i, j;
	for(i = 0; i < nLevel; i++)
	{
		lfMatch = false;
        
		for(j = 0; j < lnUnique; j ++)
			if(pTag[i] == pTag[lpUnique[j]] &&
               ppIndices[i] == ppIndices[lpUnique[j]])
			{
				lfMatch = true;
				break;
			}
        
		if(!lfMatch)
			lpUnique[lnUnique ++] = i;
	}
    
    
    
	mnLevels = lnUnique;
    // mvLevelID.resize(mnLevels);
    // mvLevelID.assign(lpUnique, lpUnique+lnUnique);
    
    // tehe 4/17/2012: avoid memory leak
    mvSubtotalID.clear();
    mvIndices.clear();
    mvUnits.clear();
    mvIndices2.clear();
    mvHasRecursiveUnit.clear();
    
	mvSubtotalID.resize(lnUnique);
	
    mvIndices.resize(lnUnique);
    mvIndices2.resize(lnUnique);
    mvHasRecursiveUnit.resize(lnUnique);
    
	for(i = 0; i < lnUnique; i++)
	{
		mvSubtotalID[i] = pTag[lpUnique[i]];
		mvIndices[i] = ppIndices[lpUnique[i]];
        mvIndices2[i]= ppChildIndices[lpUnique[i]];
        mvHasRecursiveUnit[i] = irvHasRecursiveUnit[lpUnique[i]];
		if (!mvIndices[i]->GetUnqualifiedRows()->IsEmpty()) //note: GetUnqualifiedRows always return something
			mHasUnqualifiedRows = true;
	}
    
    // 8/28/2012 tehe pre-sort subtotal values
    int lMaxSubtotalID = 0;
    for(int i=0 ;i< mvSubtotalID.size(); i++)
    {
        if(mvSubtotalID[i] > lMaxSubtotalID)
            lMaxSubtotalID = mvSubtotalID[i];
    }
    
    if(lMaxSubtotalID > 0)
    {
        vector<vector<DSSTabularIndex *>* > lvvResult;
        lvvResult.resize(lMaxSubtotalID+1);
        for (int i= 0; i<lMaxSubtotalID+1; i++)
        {
            lvvResult[i] = NULL;
        }
        for(int i= 0;i<mvSubtotalID.size(); i++)
        {
            if(lvvResult[mvSubtotalID[i]] == NULL)
            {
                lvvResult[mvSubtotalID[i]] = new vector<DSSTabularIndex *>();
            }
            lvvResult[mvSubtotalID[i]]->push_back(mvIndices[i]);
        }
        mvSubtotalID.clear();
        mvIndices.clear();
        
        for(int i= 0; i<lMaxSubtotalID+1; i++)
        {
            if(lvvResult[i])
            {
                for(int j = 0;j < lvvResult[i]->size(); j++)
                {
                    mvIndices.push_back((*lvvResult[i])[j]);
                    mvSubtotalID.push_back(i);
                }
            }
        }
        for(int i=0;i<lMaxSubtotalID+1; i++)
        {
            if(lvvResult[i])
            {
                delete lvvResult[i];
            }
        }
        lvvResult.clear();
    }
    
    
	mnUnits = nColUnit;
    mvUnits.resize(nColUnit);
    mvUnits.assign(pColUnit, pColUnit+nColUnit);
    
	mnTabularUnits = nAttrUnit;
	mbHasMetric = fMetric;
	mnMetrics = nMetric;
	mpView = pView;
	AE_ASSERT(mpView != NULL);
    
    mbPreXtab = mpView->IsDoPreXtab();
    
	int hr = S_OK;
    DSSViewDataSet *lpViewDataSet = mpView->GetViewDataSet();
    CHECK_PTR(lpViewDataSet);
    
    DSSTabularData *lpTabularData = NULL;
    hr = lpViewDataSet->getTabularData(&lpTabularData);
    CHECK_HR;
    DSSTabularMetrics *lpMetrics = NULL;
    hr = lpTabularData->getMetrics(&lpMetrics);
    CHECK_HR;
	
    mMetricsUnitID = lpMetrics -> getUnitIndex();
    
    // mvDispRows.clear();
    
    // 8/10/2012 tehe: create unit map index
    mvUnitMapToIndex.resize(mnLevels * mnUnits);
    for (int i = 0; i < mnLevels; i++)
	{
		DSSTabularIndex *lpIndex = mvIndices[i];
        
		//build unit map to index
		int lnIndexUnits = lpIndex->getNumUnits();
		vector<int> lvUnits(lnIndexUnits);
		hr = lpIndex->getUnits(lnIndexUnits, lnIndexUnits > 0 ? &lvUnits[0] : NULL);
		CHECK_HR;
		
		for (j = 0; j < mnUnits; j++)
		{
			mvUnitMapToIndex[i * mnUnits + j] = -1;
			for (int k = 0; k < lnIndexUnits; k++)
			{
				if (mvUnits[j] == lvUnits[k])
				{
					mvUnitMapToIndex[i * mnUnits + j] = k;
				}
			}
		}
	}
    
    // 8/30/2012 tehe
    hr = init();
    CHECK_HR;
    
	return S_OK;
    
}

int DSSXTabHeader::Init(vector<int> &irvUnits, int nRuntimeUnits, int iMetricsID, int nRows, //vector<int> &irvOffset, vector<short> &irvLevelID,
                        vector<DSSTabularIndex*> &irvpIndice, vector<int> &irvSubtotalID, DSSDataModelXTabImpl *ipXTabModel)
{
	int hr = S_OK;
	
	if (irvSubtotalID.size() != irvpIndice.size())
		return E_INVALIDARG;
	
	mnLevels = irvpIndice.size();
	mvIndices.assign(irvpIndice.begin(), irvpIndice.end());
	mvSubtotalID.assign(irvSubtotalID.begin(), irvSubtotalID.end());
	
	mnUnits = irvUnits.size();
	mvUnits.assign(irvUnits.begin(), irvUnits.end());
	mnRuntimeUnits = nRuntimeUnits;
    
	mnTabularUnits = ipXTabModel->Count();
    
	mMetricsUnitID = iMetricsID;
	mbHasMetric = iMetricsID >= 0;
	mnMetrics = 0;
	if (mbHasMetric)
	{
		DSSTabularMetrics *lpMetrics = NULL;
		ipXTabModel->getMetricsUnit(&lpMetrics);
		CHECK_PTR(lpMetrics);
		mnMetrics = lpMetrics->Count();
	}
	
	if(mbHasMetric && mnMetrics == 0)
		mbHasMetric = false;
    
	mpView = ipXTabModel->getXTabView();
	CHECK_PTR(mpView);
    
	mnRows = nRows;
    //526227, performance, replace mvOffset with mvLevelOffset
    /*	int lReservedSize = mnRows * sizeof(int);
     {
     MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)
     mvOffset.resize(mnRows);
     MEM_GOVERNOR_COMMIT(lReservedSize)
     }*/
	
	int lReservedSize = mnRows * sizeof(short);
	{
        MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)
        //	mvLevelID.resize(mnRows);
        mpLevelID = new short[mnRows];
        MEM_GOVERNOR_COMMIT(lReservedSize)
	}
	
	mvUnitMapToIndex.resize(mnLevels * mnUnits);
	int count = 0;
    
    //	int *lpOffset = nRows > 0 ? &mvOffset[0] : NULL;
    mvLevelOffset.resize(mnLevels);
	short *lpLevelID = nRows > 0 ? &mpLevelID[0] : NULL;
	for (int i = 0; i < mnLevels; i++)
	{
        mvLevelOffset[i] = count;
		DSSTabularIndex *lpIndex = irvpIndice[i];
		int lnRows = lpIndex->Count();
		count += lnRows;
		if (count > nRows)
			return E_FAIL;
		
		int j = 0;
		for (j = 0; j < lnRows; j++)
		{
            //			*lpOffset++ = j;
            if(lpLevelID)
                *lpLevelID++ = i;
		}
		
		//build unit map to index
		int lnIndexUnits = lpIndex->getNumUnits();
		vector<int> lvUnits(lnIndexUnits);
		hr = lpIndex->getUnits(lnIndexUnits, lnIndexUnits > 0 ? &lvUnits[0] : NULL);
		CHECK_HR;
		
		for (j = 0; j < mnUnits; j++)
		{
			mvUnitMapToIndex[i * mnUnits + j] = -1;
			for (int k = 0; k < lnIndexUnits; k++)
			{
				if (mvUnits[j] == lvUnits[k])
				{
					mvUnitMapToIndex[i * mnUnits + j] = k;
				}
			}
		}
	}
	
	if(!mbHasMetric)
		mnDispRows = mnRows;	//all the row will be displayed
	else
		mnDispRows = mnRows * mnMetrics;
    
    //526227, performance, remove mvDispRows
    /*	lReservedSize = mnDispRows * sizeof(int);
     {
     MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)
     mvDispRows.resize(mnDispRows);
     MEM_GOVERNOR_COMMIT(lReservedSize)
     }
     for(int iRow = 0; iRow < mnDispRows; ++iRow)
     mvDispRows[iRow] = iRow;
     */
	hr = hCreateUnitMappingTable(ipXTabModel);
	CHECK_HR;
	
	return S_OK;
}

int DSSXTabHeader::getNumUnits()
{
	return mnUnits;
}

int DSSXTabHeader::getUnits(int nUnitSize, int *opUnits)
{
	if (nUnitSize < mnUnits || !opUnits)
		return E_INVALIDARG;
	
	if (mnUnits > 0)
	{
		memmove(opUnits, &mvUnits[0], mnUnits * sizeof(int));
	}
	return S_OK;
}

int DSSXTabHeader::getUnit(int index)
{
	if (index < 0 || index >= mnUnits)
		return E_INVALIDINDEX;
	
	return mvUnits[index];
}


int DSSXTabHeader::getLevel(int iRow)
{
	if (iRow < 0 || iRow >= mnDispRows)
		return -1;
	
	// iRow = mvDispRows[iRow];
	
	if(mbHasMetric)
	{
		iRow = iRow / mnMetrics;
	}
	
	return mpLevelID ? mpLevelID[iRow] : mvLevelID[iRow];
}



int DSSXTabHeader::getIndex(int iRow, DSSTabularIndex **oppIndex)
{
	if (iRow < 0 || iRow >= mnDispRows)
		return E_INVALIDINDEX;
	
	if (!oppIndex)
		return E_INVALIDARG;
	
	// iRow = mvDispRows[iRow];
	
	if(mbHasMetric)
	{
		iRow = iRow / mnMetrics;
	}
	*oppIndex = mpLevelID ? mvIndices[mpLevelID[iRow]] : mvIndices[mvLevelID[iRow]];
	return S_OK;
}

int DSSXTabHeader::getSubtotalID(int iRow, int &orSutotalID)
{
	if (iRow < 0 || iRow >= mnDispRows)
		return E_INVALIDINDEX;
	
	// iRow = mvDispRows[iRow];
	
	if(mbHasMetric)
	{
		iRow = iRow / mnMetrics;
	}
    
	orSutotalID = mpLevelID ? mvSubtotalID[mpLevelID[iRow]] : mvSubtotalID[mvLevelID[iRow]];;
	return S_OK;
}

int DSSXTabHeader::getIndexByLevel(int level, DSSTabularIndex **oppIndex)
{
	if (level < 0 || level >= mnLevels)
		return E_INVALIDINDEX;
	if (oppIndex)
		*oppIndex = mvIndices[level];
	return S_OK;
}

int DSSXTabHeader::getSubtotalIDByLevel(int level, int &orSutotalID)
{
	if (level < 0 || level >= mnLevels)
		return E_INVALIDINDEX;
	orSutotalID = mvSubtotalID[level];
	return S_OK;
}

int DSSXTabHeader::getOffsetOfIndex(int iRow)
{
	if (iRow < 0 || iRow >= mnDispRows)
		return -1;
	
	if(mbHasMetric)
	{
		iRow = iRow / mnMetrics;
	}
	
    //mvLevelOffset is used for optimization, to replace mvOffset.
    //instead of saving the offset for each row, we only save the offset of the first row of each level
    //to get the index offset for iRow, we use the formula:
    //  offset = iRow - mvLevelOffset[mvLevel[iRow]];
    int lOffset = 0;
    if(mpLevelID)
    {
        lOffset = iRow - mvLevelOffset[mpLevelID[iRow]];
    }
    else if(mbOffsetFlag)
    {
        lOffset = mvOffset[iRow];
    }
    else
        lOffset = iRow - mvLevelOffset[mvLevelID[iRow]];
    
    return lOffset;
}

int DSSXTabHeader::FillIndex(int iRow, int nUnits, int *pIndex, int *pMetric)
{
	if (!pIndex || !pMetric)
		return E_INVALIDARG;
	
	if (mnDispRows == 0 	//466729, header is empty, don't fail
        || mnUnits == 0)    //640685, also check mnUnits.
		return S_OK;
	
	if (iRow < 0 || iRow >= mnDispRows)
		return E_INVALIDINDEX;
	
	// iRow = mvDispRows[iRow];
	
	if(mbHasMetric)
	{
		*pMetric = iRow % mnMetrics;
		if (mbHasGBMetric)
		{
			*pMetric = mapToMetricID[*pMetric];
		}
		iRow = iRow / mnMetrics;
	}
    
    if (iRow < 0 || iRow >= mnDispRows)
		return E_INVALIDINDEX;
    
    int lOffset = 0;
    if(mpLevelID)
    {
        lOffset = iRow - mvLevelOffset[mpLevelID[iRow]];
        
        return mvIndices[mpLevelID[iRow]]->Item(lOffset, nUnits, pIndex);
    }
    else
    {
        if(mbOffsetFlag)
            lOffset = mvOffset[iRow];
        else
            lOffset = iRow - mvLevelOffset[mvLevelID[iRow]];
        
        return mvIndices[mvLevelID[iRow]]->Item(lOffset, nUnits, pIndex);
    }
}

int DSSXTabHeader::Fill_Index(int iRow, int nUnit, int *pIndex, int *pMetric)
{
    return FillIndex(iRow, nUnit, pIndex, pMetric);
}

int DSSXTabHeader::FillIndexWithTag(int iRow, int nUnits, int *pIndex, int *pMetric)
{
	if (!pIndex || !pMetric)
		return E_INVALIDARG;
	
	if (iRow < 0 || iRow >= mnDispRows)
		return E_INVALIDINDEX;
    
	// tehe 1/13/2012 do very special handling for GB metrics
	if(mbHasMetric)
	{
		*pMetric = iRow % mnMetrics;
		if (mbHasGBMetric)
		{
            if(!mbIsNewRWD)
                *pMetric = mapToMetricID[*pMetric];
		}
		iRow = iRow / mnMetrics;
	}
    
    if (iRow < 0 || iRow >= mnDispRows)
		return E_INVALIDINDEX;
    
	int lSubtotalFlag = mpLevelID ? mvSubtotalID[mpLevelID[iRow]] : mvSubtotalID[mvLevelID[iRow]];
	//501925, only fill the units on this header
	int *lpUnits = mnUnits > 0 ? &mvUnits[0]: NULL;
	for (int i = 0; i < mnUnits; i++)
	{
		if (*lpUnits < nUnits)
			pIndex[*lpUnits] = -lSubtotalFlag;
		
		lpUnits++;
	}
	
    int lOffset = 0;
    if(mpLevelID)
    {
        lOffset = iRow - mvLevelOffset[mpLevelID[iRow]];
        
        return mvIndices[mpLevelID[iRow]]->Item(lOffset, nUnits, pIndex);
    }
    else
    {
        if(mbOffsetFlag)
            lOffset = mvOffset[iRow];
        else
            lOffset = iRow - mvLevelOffset[mvLevelID[iRow]];
        
        return mvIndices[mvLevelID[iRow]]->Item(lOffset, nUnits, pIndex);
    }
    
}


//	@param ipRowList : row list, has to be sorted.
//	@param iRowCount : number of rows
//
// This method returns the header elements whithin the row range in a block/group structure.
// Every unit may occupy one or multiple(if custom group unit enables hierarchical display) columns.
// The header elements are saved column by column. In each column, if an element appears in consecurtive rows
// its start row number and end row number will be saved.

// Example, Row Header has one attribute and one custom group(Year, Quarter_CG)
//		-------------------------
//row#	  Year	|   Qaurter_CG	|
//		+-------+-------+-------+
//	0	  2000	   CG1	  (CG1)
//	1	 (2000)   (CG1)	  Q100
//	2	 (2000)	   CG2	  (CG2)
//	3	 (2000)	  (CG2)	  Q200
//	4	  2001	   CG2	  (CG2)
//	5	 (2001)	  (CG2)	  Q201
//	6	 (2001)    CG3	  (CG3)
//	7	 (2001)   (CG3)	  Q301
//		-------------------------
// the customer
// The result will look like:
//	Column1: (0,4,8), which means 2000's block range is [0,3); 2001's block range is [4,8)
//	Column2: (0,2,4,6,8)
//	Column3: (0,1,2,3,4,5,6,7,8)
int DSSXTabHeader::getHeaderElements(int iRowCount, const int *iRowList, int nUnits, bool *ipbHierDisp,
                                     vector<int> &orUnitID, vector<vector<int> > &orBlocks, vector<vector<int> > &orEleKeys)
{
	if (iRowCount == 0)
		return S_OK;
	
	int hr = S_OK;
	
	int lMetric1 = 0;
	vector<int> lvKey1(mnTabularUnits);
	int *lKey1 = mnTabularUnits > 0 ? &lvKey1[0] : NULL;
	int lMetric2 = 0;
	vector<int> lvKey2(mnTabularUnits);
	int *lKey2 = mnTabularUnits > 0 ? &lvKey2[0] : NULL;
	
	bool lfDistinct = false;
	
	int lnTotalUnits = mnTabularUnits;
	int lnUnits = nUnits;
	const int* lpUnitsHead = mnUnits > 0 ? &mvUnits[0] : NULL;
	int lMetricsID = mMetricsUnitID;
	
	bool* lpbIsHierarchicalDisplay = ipbHierDisp;
	vector<DSSTabularCustomGroup*> lvCustomGroup(lnUnits);
	DSSTabularCustomGroup** lpCG = &lvCustomGroup[0];
	
	hr = hGetCustomGroup(&mvUnits[0], lnUnits, lpCG);
	CHECK_HR;
	
	int* lpUnitDepth = &mvUnitDepth[0];
	
	//need adjust unit depth by hierarchical display flag if there's custom group unit
	vector<int> lvUnitDepth;
	if (mvUnitDepth[mnUnits] > mnUnits)
	{
		lvUnitDepth.resize(mnUnits + 1);
		lpUnitDepth = &lvUnitDepth[0];
		
		lpUnitDepth[0] = 0;
		int lDepth = 0;
		for (int i = 1; i < lnUnits + 1; i++)
		{
			if (!lpbIsHierarchicalDisplay[i - 1])
				lDepth++;
			else
				lDepth += mvUnitDepth[i] - mvUnitDepth[i - 1];
			
			lpUnitDepth[i] = lDepth;
		}
	}
	
	int lnColumns = lpUnitDepth[lnUnits];
	orEleKeys.resize(lnColumns);
	orBlocks.resize(lnColumns);
	
	hr = FillIndexWithTag(iRowList[0], lnTotalUnits, lKey1, &lMetric1);
	CHECK_HR;
	
	orUnitID.resize(lnColumns);
	int i = 0;
	for (i = 0; i < lnUnits; i++)		//fill the first row
	{
		int key = 0;
        if(lKey1)
            key = lKey1[lpUnitsHead[i]];
		if (lpUnitsHead && lpUnitsHead[i] == lMetricsID)
			key = lMetric1;
		int j = lpUnitDepth[i];
		for (; j < lpUnitDepth[i + 1]; j++)
		{
			orEleKeys[j].push_back(key);
			orBlocks[j].push_back(0);
			orUnitID[j] = i;
		}
		
		//fill the parent key
		if (lpbIsHierarchicalDisplay[i] && key >= 0)
		{
			XTabCustomGroupElement *lpXTabCGElement = lpCG[i]->getElement(key);
            //			AE_ASSERT(lpXTabCGElement);
			
            //			int depth = lpXTabCGElement->depth;
			int depth = lpXTabCGElement ? lpXTabCGElement->depth : 1;     //join null will have depth = 1
            
			depth--;	 //the root node of custom group is not displayed
			
			j = depth + lpUnitDepth[i] - 1;
			while (j >= lpUnitDepth[i] && j < lpUnitDepth[i + 1])
			{
                //zhyang, 556236, we should use the getParentKey function to get parent of CG
				//key = lpCG[i]->getElement(key)->parent;
                hr = lpCG[i]->getParentKey(key, &key);
                CHECK_HR;
				orEleKeys[j][0] = key;
				j--;
			}
		}
	}
	
	int iRow = 0;
	for(iRow = 1; iRow < iRowCount; iRow++)
	{
		hr = FillIndexWithTag(iRowList[iRow], lnTotalUnits, lKey2, &lMetric2);
		CHECK_HR;
		
		lfDistinct = false;
		
		const int *p = lpUnitsHead;	//units that need to be checked
		int j = 0;
		for (j = 0; j < lnUnits; j++)
		{
			if (p && *p == lMetricsID)
			{
				if (!lfDistinct)
					lfDistinct = (lMetric1 != lMetric2);
				
				if (lfDistinct)
				{
					orEleKeys[lpUnitDepth[j]].push_back(lMetric2);
					orBlocks[lpUnitDepth[j]].push_back(iRow);
				}
			}
			else
			{
				bool lfDistinctPrev = lfDistinct;
				if (!lfDistinct)
				{
                    if(lKey1)
                        lfDistinct = (lKey1[*p] != lKey2[*p]);
				}
				
				if (lfDistinct)
				{
					if (!lpbIsHierarchicalDisplay[j])
					{
						orEleKeys[lpUnitDepth[j]].push_back(lKey2[*p]);
						orBlocks[lpUnitDepth[j]].push_back(iRow);
					}
					else		//for custom group hierarchical display
					{
                        if(lKey1 && lKey2)
                            hr = hGetHeaderElementsOfCG(iRow, lpCG[j], lKey1[*p], lKey2[*p], lpUnitDepth[j], lpUnitDepth[j + 1],
                                                        lfDistinctPrev, orBlocks, orEleKeys);
						CHECK_HR;
					}
				}
			}
			p++;
		}
		
		//switch keys
		int * lpTemp = lKey2;
		lKey2 = lKey1;
		lKey1 = lpTemp;
		
		int lTemp = lMetric1; lMetric1 = lMetric2; lMetric2 = lTemp;
	}
	
	for (i = 0; i < lnColumns; i++)
	{
		orBlocks[i].push_back(iRowCount);			//fill the end of block
	}
	
	return S_OK;
}

int DSSXTabHeader::getRowCount()
{
	return mnDispRows;
}

bool DSSXTabHeader::HasMetric()
{
	return mbHasMetric;
}

int DSSXTabHeader::getNumRuntimeUnits()
{
	return mnRuntimeUnits;
}

int DSSXTabHeader::getUnitKey(int iRow, int iUnit)
{
	
	if (iRow >= mnDispRows || iUnit >= mnUnits)
	{
		_ASSERT(false);
		return DSSXTAB_TOTAL_KEY;		//negative value means total ID, -100 should be big enough.
	}
    
	if (mbHasMetric)
	{
		if (mvUnits[iUnit] == mMetricsUnitID)
		{
			int lMetricID = iRow % mnMetrics;
			if (mbHasGBMetric)
			{
				lMetricID = mapToMetricID[lMetricID];
			}
			return lMetricID;
		}
		iRow = iRow / mnMetrics;
	}
	int level = mpLevelID ? mpLevelID[iRow] : mvLevelID[iRow];
	int lIndexUnit = mvUnitMapToIndex[level * mnUnits + iUnit];
	if (lIndexUnit > -1)
	{
        int lOffset = 0;
        if(mpLevelID)
        {
            lOffset = iRow - mvLevelOffset[mpLevelID[iRow]];
        }
        else if(mbOffsetFlag)
        {
            lOffset = mvOffset[iRow];
        }
        else
        {
            lOffset = iRow - mvLevelOffset[mvLevelID[iRow]];
        }
        
		return mvIndices[level]->getKey(lOffset, lIndexUnit);
	}
	else
	{
		return -mvSubtotalID[level];
	}
}


vector<int>& DSSXTabHeader::getUnits()
{
	return mvUnits;
}

int DSSXTabHeader::getOffsetByElements(std::map<int, int> &irKeys)
{
	vector<int> lvKeys(mnTabularUnits);
	int i = 0;
    //zhyang, 567179, we should init all units in lvKeys
	for (i = 0; i < mnTabularUnits; i++)
	{
		lvKeys[i] = DSSXTAB_TOTAL_KEY;
	}
	
	for (int i = 0; i < mnUnits; i++)
	{
		int lUnitID = mvUnits[i];
		map<int, int>::iterator it = irKeys.find(lUnitID);
		if (it != irKeys.end())
		{
			int key = it->second;
			lvKeys[lUnitID] = key;
		}
	}
	
	return getOffsetByElements(lvKeys);
}

int DSSXTabHeader::getOffsetByElements(vector<int> &irKeys)
{
	AE_ASSERT(irKeys.size() == mnTabularUnits);
	
	vector<int> lvLevelUnits;
	int lSubtotalID = 0;
	int lMetricKey = -1;
	for (int i = 0; i < mnUnits; i++)
	{
		int lUnitID = mvUnits[i];
		int key = irKeys[lUnitID];
		
		if (mbHasMetric && lUnitID == mMetricsUnitID)
			lMetricKey = key;
		else
		{
			if (key >= 0)
				lvLevelUnits.push_back(lUnitID);
			
			if (lSubtotalID == 0 && key < 0)
				lSubtotalID = -key;
		}
	}
    
    if (mDummyUnitIndex >= 0 && irKeys[mDummyUnitIndex] >= 0)
    {
        lvLevelUnits.push_back(mDummyUnitIndex);
    }
	
	int lLevel = getLevelByUnits(lvLevelUnits, lSubtotalID);
	if (lLevel < 0 || lLevel >= mnLevels)
		return -1;
	
	int offset = -1;
	mvIndices[lLevel]->LookUp(mnTabularUnits, &irKeys[0], &offset);
	//don't check hr, caller should handle the error
	
	//adjust offset
	if (offset >= 0 && lLevel > 0)
	{
		for (int i = 0; i < lLevel; i++)
		{
			offset += mvIndices[i]->Count();
		}
	}
	
	if (offset >= 0 && lMetricKey >= 0)
	{
		offset = offset * mnMetrics + lMetricKey;
    }
    else if (lMetricKey >= 0 && mvIndices[lLevel]->getNumUnits() == 0)
    {
        //DE9652, backward compatibility. offset may be invalid for 931 I-Server.
        offset = lMetricKey;
    }
	
	return offset;
}

int DSSXTabHeader::getMetricID(int iRow)
{
    // tehe 1/13/2012 do very special handling for GB metrics
	if (mbHasMetric && iRow < mnDispRows)
	{
		int lMetricID = iRow % mnMetrics;
		if (mbHasGBMetric)
		{
            if(!mbIsNewRWD)
                lMetricID = mapToMetricID[lMetricID];
		}
		return lMetricID;
	}
	else
		return -1;
}

//imihai# - modified: added subtotalIndex == -1
int DSSXTabHeader::getLevelByUnits(vector<int> &irvUnits, int iSubtoalIndex)
{
	int lnUnits = irvUnits.size();
	vector<int> lvIndexUnits(lnUnits);
	
	int lnLevels = mnLevels;
	for (int i = 0; i < lnLevels; i++)
	{
		if ((iSubtoalIndex == -1 || iSubtoalIndex == mvSubtotalID[i]) && mvIndices[i]->getNumUnits() == lnUnits)
		{
			//compare the unit list
			if (lnUnits > 0)
			{
				mvIndices[i]->getUnits(lnUnits, &lvIndexUnits[0]);
				int j = 0;
				for (j = 0; j < lnUnits; j++)
				{
					int k = 0;
					for (k = 0; k < lnUnits; k++)
					{
						if (lvIndexUnits[j] == irvUnits[k])
							break;
					}
					if (k >= lnUnits)	//not matched
						break;
				}
				if (j >= lnUnits)	//matched
					return i;
			}
			else
				return i;
		}
	}
	return -1;
}

//compare the current element(iKey2) with element(iKey1) for hierarchical display, it assums iKey1 != iKey2
//for level after iKey2, its merge header is set to true
//for level before iKey2, if it has different parent from iKey1 at that level, add iCurrentRow to orvColumnBlocks
int DSSXTabHeader::hGetHeaderElementsOfCG(int iCurrentRow, DSSTabularCustomGroup *ipCustomGroup,
                                          int iKey1, int iKey2, int iUnitStartDepth, int iUnitEndDepth,
                                          bool ibfDistinctPrev, vector<vector<int> > &orvColumnBlocks, vector<vector<int> > &orvColumnElements)
{
	int hr = S_OK;
	
	if (iKey1 >= 0 && iKey2 >= 0)
	{
		{
            //			int lDepth1 = ipCustomGroup->getElement(iKey1)->depth;
            //			int lDepth2 = ipCustomGroup->getElement(iKey2)->depth;
            XTabCustomGroupElement *lpElement1 = ipCustomGroup->getElement(iKey1);
            XTabCustomGroupElement *lpElement2 = ipCustomGroup->getElement(iKey2);
			int lDepth1 = lpElement1 ? lpElement1->depth : 1;
			int lDepth2 = lpElement2 ? lpElement2->depth : 1;
			CHECK_HR;
			
			lDepth1 += iUnitStartDepth - 1;
			lDepth2 += iUnitStartDepth - 1; //depth - 1 is the real depth because
			//the root node of custom group is not displayed
			
			int i = iUnitEndDepth - 1;		//start from the end
			//step 1. fill orvColumnElements for iKey2;
			while (lDepth2 < i && i >= iUnitStartDepth)
			{
				orvColumnBlocks[i].push_back(iCurrentRow);
				orvColumnElements[i].push_back(iKey2);
				i--;
			}
			//i == lDepth2
			
			//step 2. if lDepth1 > lDepth2;
			int lParent1 = iKey1;
			while (lDepth1 > i && lDepth1 >= iUnitStartDepth)
			{
                //zhyang, TQMS 540004, we need to get the parent key and translate it
                hr = ipCustomGroup->getParentKey(lParent1, &lParent1);
                CHECK_HR;
				//lParent1 = ipCustomGroup->getElement(lParent1)->parent;
				lDepth1--;
			}
			
			//step 3. else if lDepth1 < lDepth2
			int lParent2 = iKey2;
			while (lDepth1 < i && i >= iUnitStartDepth)
			{
				orvColumnBlocks[i].push_back(iCurrentRow);
				orvColumnElements[i].push_back(lParent2);
				
                hr = ipCustomGroup->getParentKey(lParent2, &lParent2);
                CHECK_HR;
				//lParent2 = ipCustomGroup->getElement(lParent2)->parent;
				i--;
			}
			
			//step 4. lDepth1 == lDepth2
			while ((ibfDistinctPrev || lParent1 != lParent2)//if previous units are different, always add current row
				   && i >= iUnitStartDepth)
			{
				orvColumnBlocks[i].push_back(iCurrentRow);
				orvColumnElements[i].push_back(lParent2);
				
				//lParent1 = ipCustomGroup->getElement(lParent1)->parent;
				//lParent2 = ipCustomGroup->getElement(lParent2)->parent;
                hr = ipCustomGroup->getParentKey(lParent1, &lParent1);
                CHECK_HR;
                
                hr = ipCustomGroup->getParentKey(lParent2, &lParent2);
                CHECK_HR;
                
				i--;
			}
			
			//step 5. lParent1 == lParent2
		}
	}else if (iKey1 != iKey2 || ibfDistinctPrev)
	{
		//a. iKey1 == subtoal, iKey2 == CG;
		//b. iKey1 == CG, iKey2 == subtotal;
		//c. iKey1 == subtotal1, iKey2 == subtotal2;
		//d. iKey1 == iKey2 == subtotal1 && ibfDistinctPrev == true
		int i = 0;
		for (i = iUnitStartDepth; i < iUnitEndDepth; i++)
		{
			orvColumnBlocks[i].push_back(iCurrentRow);
			orvColumnElements[i].push_back(iKey2);
		}
		
		if (iKey2 > 0)
		{
            //			int lDepth2 = ipCustomGroup->getElement(iKey2)->depth;
            XTabCustomGroupElement *lpElement2 = ipCustomGroup->getElement(iKey2);
			int lDepth2 = lpElement2 ? lpElement2->depth : 1;
			CHECK_HR;
			lDepth2--;	//the root node of custom group is not displayed
			
			i = lDepth2 + iUnitStartDepth - 1;
			
			while (i >= iUnitStartDepth && i < iUnitEndDepth)
			{
                hr = ipCustomGroup->getParentKey(iKey2, &iKey2);
                CHECK_HR;
                
				//iKey2 = ipCustomGroup->getElement(iKey2)->parent;
				
				int size = orvColumnElements[i].size();
				orvColumnElements[i][size - 1] = iKey2;
				i--;
			}
		}//end of if (iKey2 > 0)
	}
	else
		//caller should not call this function
		return S_FALSE;
	
	return S_OK;
}

int DSSXTabHeader::hCreateUnitMappingTable(DSSDataModelXTabImpl *ipModel)
{
	mvUnitDepth.resize(mnUnits + 1);
	
	int depth = 0;
	int i = 0;
	for (i = 0; i < mnUnits; i++)
	{
		DSSTabularUnit *lpUnit = ipModel->Item(mvUnits[i]);
		CHECK_PTR(lpUnit);
		
		int width = 1;
		if (lpUnit->getUnitType() == DssTemplateCustomGroup)
		{
			width = static_cast<DSSTabularCustomGroup*> (lpUnit)->getMaxDepth();
		}
		mvUnitDepth[i] = depth;
		depth += width;
	}
	mvUnitDepth[i] = depth;
	return S_OK;
}

int DSSXTabHeader::hGetCustomGroup(int *ipColUnit, int iSize, DSSTabularCustomGroup ** oppCG)
{
	if(!mbIsNewRWD)
    {
        DSSDataModelXTabImpl *lpModel = mpView->getXTabModel();
        CHECK_PTR(lpModel);
        
        for (int i = 0; i < iSize; i++)
        {
            DSSTabularUnit *lpUnit = lpModel->Item(ipColUnit[i]);
            CHECK_PTR(lpUnit);
            
            if (lpUnit->getUnitType() == DssTemplateCustomGroup)
                oppCG[i] = static_cast<DSSTabularCustomGroup *>(lpUnit);
            else
                oppCG[i] = NULL;
        }
    }
    else
    {
        DSSViewDataSet *lpViewDataSet = mpView->GetViewDataSet();
        CHECK_PTR(lpViewDataSet);
        
        
        DSSTabularData *lpTabularData = NULL;
        lpViewDataSet->getTabularData(&lpTabularData);
        
        
        for(int i=0; i< iSize; i++)
        {
            DSSTabularUnit *lpUnit = NULL;
            int hr = lpTabularData->Item(ipColUnit[i], &lpUnit);
            CHECK_HR;
            
            if (lpUnit->getUnitType() == DssTemplateCustomGroup)
                oppCG[i] = static_cast<DSSTabularCustomGroup *>(lpUnit);
            else
                oppCG[i] = NULL;
        }
        
    }
    
    return S_OK;
}

int DSSXTabHeader::size()
{
	int lSize = sizeof(this);
	
    //	lSize += DSSAEHelper::SizeOfVector(mvDispRows);
	
    //	lSize += DSSAEHelper::SizeOfVector(mvOffset);
	
    //	lSize += DSSAEHelper::SizeOfVector(mvLevelID);
    
	lSize += DSSAEHelper::SizeOfVector(mvSubtotalID);
	
	lSize += DSSAEHelper::SizeOfVector(mvUnitMapToIndex);
	
	lSize += DSSAEHelper::SizeOfVector(mvUnits);
	
	lSize += DSSAEHelper::SizeOfVector(mvUnitDepth);
	
	return lSize;
}

int DSSXTabHeader::setGroupbyMetric(int nSelectedMetrics, vector<bool> &irvSelectedMetrics)
{
	AE_ASSERT(irvSelectedMetrics.size() == mnMetrics && mbHasMetric);
	
	mbHasGBMetric = true;
	mnMetrics = nSelectedMetrics;
	
	mnDispRows = mnRows * mnMetrics;
    /*	mvDispRows.resize(mnDispRows);
     for(int iRow = 0; iRow < mnDispRows; ++iRow)
     mvDispRows[iRow] = iRow;
     */
	mapToMetricID.resize(mnMetrics);
	int count = 0;
	for (int i = 0; i < irvSelectedMetrics.size(); i++)
	{
		if (irvSelectedMetrics[i])
		{
			mapToMetricID[count++] = i;
		}
	}
	AE_ASSERT(count == nSelectedMetrics);
	
	return S_OK;
	
}

int DSSXTabHeader::hCountConsolidationAndRecursiveUnits()
{
    DSSViewDataSet *lpViewDataSet = mpView->GetViewDataSet();
    CHECK_PTR(lpViewDataSet);
    
    DSSTabularData *lpTabularData = lpViewDataSet->getTabularData();
    CHECK_PTR(lpTabularData);
    
    // 3/26/2012 tehe: memory leak
	mnConsolidationUnit = 0;
    mvConsolidationUnit.clear();
    mvConsolidationUnit.resize(mnUnits);
	// mpConsolidationUnit = new int[mnUnits];
    // mpConsolidation = new DSSTabularConsolidation * [mnUnits];
    mvConsolidation.clear();
    mvConsolidation.resize(mnUnits);
    
    mnRecursiveUnits = 0;
    mvRecursiveUnits.clear();
    mvRecursiveUnits.resize(mnUnits);
    mvRecursiveAttr.clear();
    mvRecursiveAttr.resize(mnUnits);
    
	for(int iUnit = 0; iUnit < mnUnits; iUnit ++)
	{
		DSSTabularUnit * lpTabularDataUnit = NULL;
		int hr = lpTabularData -> Item(mvUnits[iUnit], &lpTabularDataUnit);
		CHECK_HR_PTR(lpTabularDataUnit);
        
		EnumDSSTabularDataUnitType lUnitType;
		lUnitType = (EnumDSSTabularDataUnitType)lpTabularDataUnit -> getUnitType();
        
		if(lUnitType == DssUnitConsolidation)
		{
			//need to remember
			mvConsolidation[mnConsolidationUnit] = dynamic_cast<DSSTabularConsolidation *>(lpTabularDataUnit);
			mvConsolidationUnit[mnConsolidationUnit] = mvUnits[iUnit];
			mnConsolidationUnit ++;
		}
        else if(lUnitType == DssUnitRecursiveAttribute || lUnitType == DssUnitRecursiveConsolidation || (lUnitType == DssUnitAttribute && lpTabularDataUnit->IsRecursiveUnit()))
        {
            mvRecursiveAttr[mnRecursiveUnits] = dynamic_cast<DSSXTabRecursiveUnit*>(lpTabularDataUnit);
            mvRecursiveUnits[mnRecursiveUnits] = mvUnits[iUnit];
            mnRecursiveUnits++;
        }
	}
    
	return S_OK;
}

int DSSXTabHeader::hCheckDisplayable(int iRow, int iLevel, bool &ofDisplay)
{
    // with DerivedElement, some unqualified rows may exist in the index. Don't display them
	if (mnRecursiveUnits<0 && mvIndices[iLevel]->IsUnqualified(iRow))
	{
		ofDisplay = false;
		return S_OK;
	}
	//we need to clear lpIndex each time before we use it
	int * lpIndex = new int [sizeof(int) * mnTabularUnits];
    MBase::StrongPtr<int,MBase::DeleteArray<int> > lIndexStrongPtr(lpIndex);
    
	int iUnit = 0;
	for(; iUnit < mnTabularUnits; iUnit ++)
		lpIndex[iUnit] = DSSXTAB_TOTAL_KEY;
    
	int hr = mvIndices[iLevel] -> Item(iRow, mnTabularUnits, lpIndex);
	CHECK_HR;
    
	for(iUnit = 0; iUnit < mnConsolidationUnit; iUnit ++){
		if(lpIndex[mvConsolidationUnit[iUnit]] != DSSXTAB_TOTAL_KEY){
            
            ofDisplay = mvConsolidation[iUnit]->IsDisplayed(lpIndex[mvConsolidationUnit[iUnit]]);
            
		}
        // 3/22/2012 tehe: I think the bese way may put the judging logic during the loading Consolidation, it more effective
        else if(lpIndex[mvConsolidationUnit[iUnit]] == DSSXTAB_TOTAL_KEY)
        {
            if(!mvTotalStateForCS[iUnit])
            {
                mvEnableTotalForCS[iUnit] = mvConsolidation[iUnit]->hasElementDisplayable();
            }
            
            ofDisplay = mvEnableTotalForCS[iUnit];
            mvTotalStateForCS[iUnit] = true;
        }
        
        if(!ofDisplay)
            break;
	}
    
    for(iUnit = 0; iUnit < mnRecursiveUnits; iUnit ++){
        if(lpIndex[mvRecursiveUnits[iUnit]] != DSSXTAB_TOTAL_KEY){
            ofDisplay =  mvRecursiveAttr[iUnit]->IsDisplayed(lpIndex[mvRecursiveUnits[iUnit]]);
        }
        
        if(!ofDisplay)
            break;
    }
    
	return S_OK;
}

//int DSSXTabHeader::sort()
//{
//    int hr = S_OK;
//    int lnBucketRowCount = 0;
//    for(int i = 0; i < mvSubtotalID.size(); ++i)
//    {
//        if(lnBucketRowCount < mvSubtotalID[i])
//            lnBucketRowCount = mvSubtotalID[i];
//    }
//    lnBucketRowCount ++;
//    int nSize = mvDispRows.size();
//
//    int *lpBucketLinkList = new int[mvDispRows.size()];
//    CHECK_NEWED_PTR(lpBucketLinkList);
//    MBase::StrongPtr<int> lpBucketLinkListPtr(lpBucketLinkList);
//
//	int *lpStart = new int[lnBucketRowCount];
//	CHECK_NEWED_PTR(lpStart);
//    MBase::StrongPtr<int> lpStratPtr(lpStart);
//
//	int *lpEnd = new int[lnBucketRowCount];
//	CHECK_NEWED_PTR(lpEnd);
//    MBase::StrongPtr<int> lpEndPtr(lpEnd);
//
//	int iRow = 0;
//	for (iRow = 0;  iRow < lnBucketRowCount; iRow ++)
//		lpStart[iRow] = lpEnd[iRow] = -1;
//
//	for (iRow = 0;  iRow < nSize; iRow ++)
//		lpBucketLinkList[iRow] = -1;
//
//
//	//distribute the rows among the buckets
//	for(iRow = 0; iRow < nSize; iRow++)	{
//		//int lSubtotalGroup = mvSubtotalID[hMapRow(mvDispRows[iRow])];
//        //int lSubtotalGroup = mvSubtotalID[mvLevelID[mvDispRows[iRow]]];
//        int iTempRow = mvDispRows[iRow];
//
//        if(mbHasMetric)
//        {
//            iTempRow = iTempRow / mnMetrics;
//        }
//
//        int lSubtotalGroup = mvSubtotalID[mvLevelID[iTempRow]];
//
//		if(lpStart[lSubtotalGroup] == -1) { // if the list is empty
//			lpStart[lSubtotalGroup] = iRow;
//			lpEnd[lSubtotalGroup] = iRow;
//		} else {
//			lpBucketLinkList[lpEnd[lSubtotalGroup]] = iRow;
//			lpEnd[lSubtotalGroup] = iRow;
//		}
//
//	} //for
//
//    //zhyang, 12/30/2011, I think it is we should not consider where subtatal position is first or last,
//    //attr sorter will do it for us, we only need to sort subtoal.
//    vector<int> lvResult;
//    for (int i = 0; i < lnBucketRowCount; ++i)
//    {
//        int lBucketRow = lpStart[i];
//        while (lBucketRow != -1) {
//            lvResult.push_back(mvDispRows[lBucketRow]);
//            lBucketRow = lpBucketLinkList[lBucketRow];
//        }
//    }
//
//    mvDispRows.clear();
//    mvDispRows.assign(lvResult.begin(), lvResult.end());
//
//	return hr = S_OK;
//}

int DSSXTabHeader::fillIndexUsingElements(int *ipKeys, map<int, int> &imElementIDs, int &iTag, int *ipChildKey)
{
    for (int i = 0; i < mnUnits; i++)
	{
		int lUnitID = mvUnits[i];
        if(lUnitID == mMetricsUnitID)
            continue;
        
		map<int, int>::iterator it = imElementIDs.find(lUnitID);
		if (it != imElementIDs.end())
		{
			int key = it->second;
            if(key >= 0){
                ipKeys[lUnitID] = key;
                
                if(mnRecursiveUnits>0 && ipChildKey)
                {
                    int j = 0;
                    for (; j<mnRecursiveUnits; j++)
                    {
                        if (lUnitID == mvRecursiveUnits[j]) {
                            //get child unit index
                            int lUnit = mvRecursiveAttr[j]->getDataUnit();
                            //if find, use data index to replace the display index
                            mvRecursiveAttr[j]->TransferDisplay2DataKey(ipKeys[lUnitID], ipChildKey[lUnit]);
                            break;
                            
                        }
                    }
                    
                    //if not find, then it's normal attribute
                    if (j>= mnRecursiveUnits) {
                        ipChildKey[lUnitID] = ipKeys[lUnitID];
                    }
                }
                else {
                    if (ipChildKey) {
                        ipChildKey[lUnitID] = ipKeys[lUnitID];
                    }
                }
            }
            else
                iTag = -key;
		}
	}
    
    return S_OK;
}

int DSSXTabHeader::getLevelByElements(std::map<int, int> &irKeys)
{
    vector<int> lvKeys(mnTabularUnits);
	int i = 0;
    //zhyang, 567179, we should init all units in lvKeys
	for (i = 0; i < mnTabularUnits; i++)
	{
		lvKeys[i] = DSSXTAB_TOTAL_KEY;
	}
	
	for (int i = 0; i < mnUnits; i++)
	{
		int lUnitID = mvUnits[i];
        if(lUnitID == mMetricsUnitID)
            continue;
        
		map<int, int>::iterator it = irKeys.find(lUnitID);
		if (it != irKeys.end())
		{
			int key = it->second;
            lvKeys[lUnitID] = key;
		}
	}
	
    if (mDummyUnitIndex >= 0)
    {
		map<int, int>::iterator it = irKeys.find(mDummyUnitIndex);
		if (it != irKeys.end())
		{
			int key = it->second;
            lvKeys[mDummyUnitIndex] = key;
		}
    }
	
	vector<int> lvLevelUnits;
	int lSubtotalID = 0;
	int lMetricKey = -1;
	for (int i = 0; i < mnUnits; i++)
	{
		int lUnitID = mvUnits[i];
		int key = lvKeys[lUnitID];
		
		if (mbHasMetric && lUnitID == mMetricsUnitID)
			lMetricKey = key;
		else
		{
			if (key >= 0)
				lvLevelUnits.push_back(lUnitID);
			
			if (lSubtotalID == 0 && key < 0)
				lSubtotalID = -key;
		}
	}
	
    if (mDummyUnitIndex >= 0 && lvKeys[mDummyUnitIndex] >= 0)
    {
        lvLevelUnits.push_back(mDummyUnitIndex);
    }
	
	int lLevel = getLevelByUnits(lvLevelUnits, lSubtotalID);
	if (lLevel < 0 || lLevel >= mnLevels)
		return -1;
    return lLevel;
}

int DSSXTabHeader::hResetSlicingBuffer()
{
    // tehe: release memory for xtab related variables
    mvCurRow.clear();
    if(mpCurDispRowMap)
    {
        delete [] mpCurDispRowMap;
        mpCurDispRowMap = NULL;
    }
    
	mnCurRow = 0;
	mnCurDispRow = 0;
    
    return S_OK;
}

int DSSXTabHeader::hInitMetrics(int iHasMetricGroupBy, vector<int> ivMetrics)
{
    if(mbHasMetric && iHasMetricGroupBy)
    {
        mbHasGBMetric = true;
        if(ivMetrics.size() > 0 && ivMetrics[0] == -1) // select All metrics
        {
            mapToMetricID.resize(mnMetrics);
            for (int i = 0; i < mnMetrics; i++)
                mapToMetricID[i] = i;
        }
        else{
            mapToMetricID.assign(ivMetrics.begin(), ivMetrics.end());
        }
    }
	return S_OK;
}


int DSSXTabHeader::createMMapMemory(const char * ipFilePath, bool& ibUseNewFile)
{
 	int lDataSize = mnRows * sizeof(short);
    if (lDataSize < MEM_GOVERNOR_SIZE)
        return S_OK;
    
    void *lpBuffer = NULL;
    const void *lpData = mpLevelID;
    int hr = DataSourceUtil::createMappedFileBuffer(1, &lpData, &lDataSize, ipFilePath, &ibUseNewFile, &mpMMPBuffer, &mMMPBufSize, &lpBuffer);
    CHECK_HR;
    
    delete [] mpLevelID;
    MEM_GOVERNOR_RELEASE(lDataSize, lDataSize)
    mpLevelID = (short*)lpBuffer;
    
    return S_OK;
}

int DSSXTabHeader::replaceRowByCertainSubtotal(int iRowID, int iLevel, int iSubtotalID, int iMetricID)
{
    
    int lOrgRowID = iRowID;
    if(mbHasMetric)
    {
        iRowID = iRowID / mnMetrics;
    }
    
    //HS, using none
    if(iLevel == -1)
    {
        int lRowID = iRowID;
        if(mbHasMetric)
        {
            lRowID = lRowID / mnMetrics;
        }
        if(mpLevelID[lRowID] == 0)
            return -1;
        else
            return lOrgRowID;
    }
    
    DSSTabularIndex *lInputLevelIndex = mvIndices[iLevel];
    int lInputLevelUnitNum = lInputLevelIndex->getNumUnits();
    vector<int> lInputLevelUnits;
    lInputLevelIndex->getUnits(lInputLevelUnits);
    
    DSSTabularIndex *lInputRowIndex = mvIndices[mpLevelID[iRowID]];
    vector<int> lInputRowUnits;
    lInputRowIndex->getUnits(lInputRowUnits);
    int lInputOffset = getOffsetOfIndex(lOrgRowID);
    
    //zhyang, it should be total of current level
    if(lInputRowUnits.size() < lInputLevelUnits.size())
        return lOrgRowID;
    
    vector<int> lTargetKeys;
    for(int i = 0; i < lInputLevelUnitNum; ++i)
    {
        int lIndex = 0;
        for(; lIndex < lInputRowUnits.size(); ++lIndex)
        {
            if(lInputLevelUnits[i] == lInputRowUnits[lIndex])
                break;
        }
        assert(lIndex != lInputRowUnits.size());
        lTargetKeys.push_back(lInputRowIndex->getKey(lInputOffset, lIndex));
    }
    
    for(int i = 0; i < mnDispRows; ++i)
    {
        int lSubTotalID;
        
        int lAdjustRow = AdjustRowByMetricID(i, iMetricID);
        int iCurRow = lAdjustRow;
        if(mbHasMetric)
        {
            iCurRow = iCurRow / mnMetrics;
        }
        
        getSubtotalID(lAdjustRow, lSubTotalID);
        if(iSubtotalID == lSubTotalID)
        {
            int lOffset = getOffsetOfIndex(lAdjustRow);
            DSSTabularIndex *lIndex = mvIndices[mpLevelID[iCurRow]];
            if(lIndex->getNumUnits() == lInputLevelUnitNum)
            {
                int lKeyIndex = 0;
                for(; lKeyIndex < lInputLevelUnitNum; ++lKeyIndex)
                {
                    if(lIndex->getKey(lOffset, lKeyIndex) != lTargetKeys[lKeyIndex])
                        break;
                }
                if(lKeyIndex == lInputLevelUnitNum)
                {
                    return lAdjustRow;
                }
            }
        }
    }
    
    return -1;
}

int DSSXTabHeader::getNumUnitsForCertainRow(int iRowID, vector<int> &ivTargetUnits)
{
    if(mbHasMetric)
	{
		iRowID = iRowID / mnMetrics;
	}
    if (iRowID < 0 || iRowID >= mnDispRows)
		return -1;
	
    //	iRow = mvDispRows[iRow];
	
	if(mbHasMetric)
	{
		iRowID = iRowID / mnMetrics;
	}
    
    //DSSTabularIndex *lInputRowIndex = mvIndices[mpLevelID[iRowID]];
    DSSTabularIndex *lInputRowIndex = mvIndices[mvLevelID[iRowID]];
    vector<int> lInputRowUnits;
    lInputRowIndex->getUnits(lInputRowUnits);
    
    //zhyang, report level
    if (ivTargetUnits.size() == 0) {
        return lInputRowUnits.size();
    }
    
    int lnUnits = 0;
    for(int i = 0; i < lInputRowUnits.size(); ++i)
        for(int j = 0; j < ivTargetUnits.size(); ++j)
        {
            if(lInputRowUnits[i] == ivTargetUnits[j])
                lnUnits++;
        }
    return lnUnits;
}


bool DSSXTabHeader::hGetOffsetFlag()
{
    if(mHasUnqualifiedRows || mnConsolidationUnit > 0 || mnRecursiveUnits > 0)
        return true;
    
    DSSMaskInfos *lpMaskInfo = mpView->getHideNullZeroMaskInfo();
    
    int lLevelID = 0;
    if(lpMaskInfo && lpMaskInfo->GetMainIndexMask() != NULL && lLevelID == 0)
    {
        return true;
    }
    
    return false;
}

//fali, 939000, not all index rows is displayed. Locate the displayed offset using mvoffset vector
int DSSXTabHeader::getDisplayOffsetByElements(vector<int> &irKeys)
{
    AE_ASSERT(irKeys.size() == mnTabularUnits);
	
	vector<int> lvLevelUnits;
	int lSubtotalID = 0;
	int lMetricKey = -1;
	for (int i = 0; i < mnUnits; i++)
	{
		int lUnitID = mvUnits[i];
		int key = irKeys[lUnitID];
		
		if (mbHasMetric && lUnitID == mMetricsUnitID)
			lMetricKey = key;
		else
		{
			if (key >= 0)
				lvLevelUnits.push_back(lUnitID);
			
			if (lSubtotalID == 0 && key < 0)
				lSubtotalID = -key;
		}
	}
    
    if (mDummyUnitIndex >= 0 && irKeys[mDummyUnitIndex] >= 0)
    {
        lvLevelUnits.push_back(mDummyUnitIndex);
    }
	
	int lLevel = getLevelByUnits(lvLevelUnits, lSubtotalID);
	if (lLevel < 0 || lLevel >= mnLevels)
		return -1;
	
	int offset = -1;
	mvIndices[lLevel]->LookUp(mnTabularUnits, &irKeys[0], &offset);
	//don't check hr, caller should handle the error
	
    int displayOffset = offset;
    if (mvConsolidationUnit.size() > 0)
    {
        if(mbOffsetFlag)
        {
            for (int i=0; i<mvOffset.size(); i++)
            {
                if (mvOffset[i] == offset)
                {
                    displayOffset = i;
                    break;
                }
            }
        }
    }
    
	//adjust offset
	if (offset >= 0 && lLevel > 0)
	{
		for (int i = 0; i < lLevel; i++)
		{
			displayOffset += mvIndices[i]->Count();
		}
	}
	
	if (offset >= 0 && lMetricKey >= 0)
	{
		displayOffset = displayOffset * mnMetrics + lMetricKey;
	}
	
	return displayOffset;
    
}
