//
//  DSSAttributeThresholdIterator.cpp
//  ReportServiceCore
//
//  Created by Zeng, Jinliang on 3/22/16.
//  Copyright © 2016 MicroStrategy, Inc. All rights reserved.
//

#include "DSSAttributeThresholdIterator.h"
#include "DSSXTabView.h"
#include "DSSCube.h"
#include "DSSTabularUnit.h"
#include "DSSTabularData.h"
#include "DSSXTabHeader.h"
#include "DSSSliceMapper.h"
#include "DSSCubeDataSlice.h"
#include "DSSViewDataset.h"

DSSAttributeThresholdIterator::DSSAttributeThresholdIterator(DSSXTabView* ipXTabView) : mpXTabView(ipXTabView), mCurrRow(-1), mCurrColumn(-1), mCurrAttr(GUID_NULL), mCurrThresholdId(-1), mCurrAxis(DssAxisReserved)
{
    
}

DSSAttributeThresholdIterator::DSSAttributeThresholdIterator()
: mpXTabView(NULL), mCurrRow(-1), mCurrColumn(-1), mCurrAttr(GUID_NULL), mCurrThresholdId(-1), mCurrAxis(DssAxisReserved)
, mStartRow(-1), mRowNum(-1), mbMergeRow(false), mbMergeCol(false)
{
}

DSSAttributeThresholdIterator::~DSSAttributeThresholdIterator()
{
}

int DSSAttributeThresholdIterator::setXTabView(DSSXTabView* ipXTabView)
{
    mpXTabView = ipXTabView;
    
    return S_OK;
}

int DSSAttributeThresholdIterator::Init(int iStartRow, int iNumRows, VARIANT_BOOL iMergeRow, VARIANT_BOOL iMergeCol)
{
    int hr = S_OK;
    
    //US44715: incremental fetch and merged headers
    if(iMergeRow == VARIANT_TRUE)
        mbMergeRow = true; //by default, mbMergeCol is intialized to false in contructor.
    if(iMergeCol == VARIANT_TRUE)
        mbMergeCol = true; //by default, mbMergeCol is intialized to false in contructor.
    
    //assign mStartRown and mRownum against illegal input:
    mStartRow = (iStartRow < 0) ? 0: iStartRow;
    mRowNum = ((iNumRows < 0) || (iNumRows > mpXTabView->getRowCount())) ? mpXTabView->getRowCount() : iNumRows;
    
    //retrieve the tabular data associated with mpXTabView:
    DSSCube* lpCube = NULL;
    hr = mpXTabView->GetCube(&lpCube);
    CHECK_HR_PTR(lpCube);
    
    DSSTabularData* lpTabularData = NULL;
    hr = lpCube->get_TabularData(&lpTabularData);
    if (hr != S_OK || !lpTabularData)
    {
        // sometimes, the tabular data is NULL. We try to get it from the viewDataset
        DSSViewDataSet* lpViewDataset = mpXTabView->GetViewDataSet();
        CHECK_PTR(lpViewDataset);
        lpTabularData = lpViewDataset->getTabularData();
        CHECK_PTR(lpTabularData);
    }
    
    //retrieve unit IDs of the row(0) / Column(1) header
    for (int axis = 0; axis < 2; axis++)
    {
        DSSXTabHeader * axisHeader = mpXTabView->getHeader(axis);
        if (!axisHeader)
            continue;
            
        int lNumUnits = axisHeader->getNumUnits();
        DSSTabularUnit* lpTabularDataUnit = NULL;
        for(int i = 0; i < lNumUnits; i++)
        {
            int lUnit = axisHeader->getUnit(i);
            
            hr = lpTabularData->Item(lUnit, &lpTabularDataUnit);
            CHECK_HR_PTR(lpTabularDataUnit);
            
            DSSTemplateUnit_Type lUnitType;
            hr = lpTabularDataUnit -> get_UnitType(&lUnitType);
            CHECK_HR;
            
            if(lUnitType==DssTemplateAttribute)  //to be extended to other types such as consolidation, custom group.
            {
                //check if it is a dummy unit
                DSS_ID lID = GUID_NULL;
                hr = lpTabularDataUnit->get_IDI(&lID);
                CHECK_HR;
                
                if(lID != GUID_NULL)
                {
                    if (axis == 0)
                    {
                        mvRowUnitIDs.push_back(lID);
                    }
                    else
                    {
                        mvColUnitIDs.push_back(lID);
                    }
                }
            }
        }
    }

    //starting from row header
    mCurrRow = mStartRow;
    mCurrColumn = 0;
    mCurrThresholdId = -1;
    mCurrAttr = GUID_NULL;
    mCurrAxis = DssAxisReserved;
    
    mUnitIter = mvRowUnitIDs.begin();
    if (mUnitIter != mvRowUnitIDs.end())
    {
        mCurrAxis = DssAxisRows;
    }
    else
    {
        mUnitIter = mvColUnitIDs.begin();
        if(mUnitIter == mvColUnitIDs.end())
        {
            mCurrAxis = DssAxisReserved;
            return S_FALSE;
        }
        
        mCurrAxis = DssAxisColumns;
    }
    
    mCurrAttr = *mUnitIter;
    
    return S_OK;
}

int DSSAttributeThresholdIterator::getRow(EnumDSSAxisName * opAxis, int* opRow)
{
    int hr = S_OK;
    
    if (opAxis)
    {
        *opAxis = mCurrAxis;
    }
    if (opRow)
    {
        if(mCurrAxis == DssAxisRows)
            *opRow = mCurrRow;
        else if(mCurrAxis == DssAxisColumns)
            *opRow = mCurrColumn;
        else
            return S_FALSE;
    }
    
    return hr;
}


int DSSAttributeThresholdIterator::getThreshold(DSS_ID * opAttributeID, int * opThresholdID)
{
    if (opAttributeID)
    {
        *opAttributeID = mCurrAttr;
    }
    if (opThresholdID)
    {
        *opThresholdID = mCurrThresholdId;
    }
    
    return S_OK;
}

// return S_FALSE if no threshold found.
int DSSAttributeThresholdIterator::First()
{
    mCurrRow = mStartRow;
    mCurrColumn = 0;
    mCurrThresholdId = -1;
    mCurrAttr = GUID_NULL;
    mCurrAxis = DssAxisReserved;
    
    mUnitIter = mvRowUnitIDs.begin();
    if (mUnitIter != mvRowUnitIDs.end())
    {
        mCurrAxis = DssAxisRows;
    }
    else
    {
        mUnitIter = mvColUnitIDs.begin();
        if (mUnitIter == mvColUnitIDs.end())
        {
            mCurrAxis = DssAxisReserved;
            return S_FALSE;
        }
        
        mCurrAxis = DssAxisColumns;
    }
    
    mCurrAttr = *mUnitIter;
    return Next();
}

// return S_FALSE if no threshold found.
/* This function decides the traversal order. The traversal order is indexed in three dimensions: data level, axis, attributes.
 The three dimension is organized according the following hierarchy: "Axis (Row/Column) => attributes => level", with the former one in a higher hierarchy, i.e.,
 Axis (Row/Column)
 => attributes
 => level
 
 In another word, we will traverse axis by axis, i.e., we will iterate row by row and then column by column.
 We will then iterate find thresholds for all attributes in the current row/column before we proceed to the next row/column, regardless whatever level of the iterated cell.
 
 Example:
 |----------------------------------|
 |                              M	|
 |                              A3	|
 lineNum	|	A1  A2		a31  a32|
	1       |	a11 a21		 0    1	|
	2       |	a11 a22		 2    2	|
	3       |	a11 subtotal 0    3	|
	4       |	a12 a21		 1    0	|
	5       |	a12 a22		 0    0	|
	6       |	a12 subtotal 1    0	|
 |----------------------------------|
 
 Indexed as (level, row/column, attribute), our traversal order is:
 BEGIN
 row
	(0, r1, A1) (0, r1, A2)
	(0, r2, A1) (0, r2, A2)
	subtotal level (1, r3, A1)
	(0, r4, A1) (0, r4, A2)
	(0, r5, A1) (0, r5, A2)
	subtotal level (1, r6, A1)
 column
	(0, c1, A3), together with subtotal level (1, c1, A3)
	(0, c2, A3), together with subtotal level (1, c2, A3)
 END
 */
int DSSAttributeThresholdIterator::Next()
{
    int hr = S_OK;
    
    if (mCurrAxis == DssAxisReserved)
        return S_FALSE;
    
    if ((mCurrAxis == DssAxisColumns) && (mUnitIter == mvColUnitIDs.end()) && mCurrColumn >= mpXTabView->getColCount())
        return S_FALSE;
    
    DSSCube* lpCube;
    hr = mpXTabView->get_Cube(&lpCube);
    CHECK_HR_PTR(lpCube);
    
    //DE37204, msun, to use local array directly to avoid memory leak. Buffer is not totally implemented. Guess only good for char*.
    //MBase::BufferPtr lBufferPtr(MBase::CreateBuffer(1, 1));
    //bool * bIndex = new (lBufferPtr.Get()) bool[mpXTabView->mnTotalUnits];
    //CHECK_NEWED_PTR(bIndex);
    //DSSXTabKey *pIndex = new (lBufferPtr.Get()) DSSXTabKey[mpXTabView->mnTotalUnits];
    //CHECK_NEWED_PTR(pIndex);
    
    //C99, Variable Length array, can also use vector here if VLA is disabled.
    bool bIndex[mpXTabView->mnTotalUnits];
    DSSXTabKey pIndex[mpXTabView->mnTotalUnits];
    
    int lThresholdId = -1;
    bool bFindThreshold = false;
    if (mCurrAxis == DssAxisRows)
    {
        while (mCurrRow < mStartRow + mRowNum)
        {
            //Select the current attribute. If all attribute units in the current row has been checked, proceed to the next row.
            if (mUnitIter == mvRowUnitIDs.end())
            {
                mUnitIter = mvRowUnitIDs.begin();
                mCurrRow++;
            }
            mCurrAttr = *mUnitIter;
            
            if (mCurrRow >= mStartRow + mRowNum)
            {
                mCurrAxis = DssAxisColumns;
                mCurrColumn = 0;  //switch to column
                
                mUnitIter = mvColUnitIDs.begin();
                
                return Next();
            }
            
            //DE25447: In the following iteration, we previously iterated each metric cell in the current row and retrieved
            //its index. We then got attribute thresholds coresponding to that index. This will cause two problems:
            //1. when there was no metric cell, we wouldn't go into the iteration and no index would be retrieved in the row.
            //So we now force iEndingCol=1 in this case.
            //2. when there were multiple metrics, we would loop through all metrics for the same header to get indices. This
            //is unnecessary. So we add a bool variable "isFirstOrNonMetricCell" to indicate that a metric cell is associated
            //with the first metric in the XTab table and we will only retrieve indices for this kind of metric cells.
            int iEndingCol = mpXTabView->getColCount();
            if (iEndingCol == 0)
                iEndingCol = 1;
            bool isFirstOrNonMetricCell = true;
            for (int iCol = 0; iCol < iEndingCol; iCol++)
            {
                mpXTabView->getHeader(1)->isFirstMetricCell(iCol, &isFirstOrNonMetricCell);
                if (!isFirstOrNonMetricCell)
                    continue;
                
                hr = hgetCellThresholdId(mCurrRow, iCol, &mCurrAttr, lpCube, bIndex, pIndex, &lThresholdId);
                if (FAILED(hr))
                    return hr;
                if (hr != S_OK)
                    continue;
                
                if (lThresholdId > 0)
                {
                    if (!bFindThreshold) //it is the first threshold id found in the current row.
                    {
                        bFindThreshold = true;
                        mCurrThresholdId = lThresholdId;
                    }
                    else
                        mCurrThresholdId = (mCurrThresholdId < lThresholdId) ? mCurrThresholdId : lThresholdId; //update to the threshold with smallest id.
                }
            } //end for
            
            mUnitIter++;
            
            if (bFindThreshold)
            {
                return S_OK;
            }
            
        } //end while
    }
    else if ((mCurrAxis == DssAxisColumns) && !mvColUnitIDs.empty())
    {
        while (mCurrColumn < mpXTabView->getColCount())
        {
            //Select the current attribute. If all attribute units in the current column has been checked, proceed to the next column.
            if (mUnitIter == mvColUnitIDs.end())
            {
                mUnitIter = mvColUnitIDs.begin();
                mCurrColumn++;
            }
            mCurrAttr = *mUnitIter;
            
            if (mCurrColumn >= mpXTabView->getColCount()) //finish iterating through all cells
            {
                return S_FALSE;
            }
            
            int iEndingRow = mStartRow + mRowNum;
            if (iEndingRow == 0) //DE25447: even when there is no row, we still need to iterate through cells in a dummy row to retrieve attribute threshold.
                iEndingRow = 1;
            for (int iRow = mStartRow; iRow < iEndingRow; iRow++)  //incremental fetch, might affects correctness of the results here.
            {
                hr = hgetCellThresholdId(iRow, mCurrColumn, &mCurrAttr, lpCube, bIndex, pIndex, &lThresholdId);
                if (FAILED(hr))
                    return hr;
                if (hr != S_OK)
                    continue;
                
                if (lThresholdId > 0)
                {
                    if (!bFindThreshold)
                    {
                        bFindThreshold = true;
                        mCurrThresholdId = lThresholdId;
                    }
                    else
                        mCurrThresholdId = mCurrThresholdId < lThresholdId ? mCurrThresholdId :lThresholdId; //update to the threshold with smallest id.
                }
            } //end for
            
            mUnitIter++;
            
            if (bFindThreshold)
            {
                return S_OK;
            }
            
        } //end while
    }
    
    return S_FALSE;
}


/*Return S_FALSE if no threshold found in the specific position. The current status will not be updated with this function.
	(i.e., mCurrAttr, mCurrRow, mCurrColumn, mCurrThresholdId, mCurrAxis etc. will not be updated. They will only be updated with First() and Next().)
 
 The first 3 parameters are the input parameters, the last one is the output parameter.
	iOrdinal is the specified row number if iAxis==DssAxisRows, or the specificed column number if iAxis==DssAxisColumns.
 */
int DSSAttributeThresholdIterator::getSpecificThreshold(EnumDSSAxisName iAxis, int iOrdinal, DSS_ID* ipAttrID, int* opThresholdId)
{
    int hr = S_FALSE;
    if (iAxis != DssAxisRows && iAxis != DssAxisColumns)
    {
        return hr;
    }
    
    if (!opThresholdId)
        return E_POINTER;
    
    //DE37204, msun, to use local array directly to avoid memory leak. Buffer is not totally implemented. Guess only good for char*.
    //MBase::BufferPtr lBufferPtr(MBase::CreateBuffer(1, 1));
    //bool * bDummyIndex = new (lBufferPtr.Get()) bool[mpXTabView->mnTotalUnits];
    //CHECK_NEWED_PTR(bDummyIndex);
    //DSSXTabKey *pDummyIndex = new (lBufferPtr.Get()) DSSXTabKey[mpXTabView->mnTotalUnits];
    //CHECK_NEWED_PTR(pDummyIndex);
    //C99 feature, VLA.
    bool bDummyIndex[mpXTabView->mnTotalUnits];
    DSSXTabKey pDummyIndex[mpXTabView->mnTotalUnits];
    
    DSSCube* lpCube = NULL;
    hr = mpXTabView->get_Cube(&lpCube);
    CHECK_HR_PTR(lpCube);
    
    bool lbFindThreshold = false;
    int lThresholdId = -1;
    int lRow = 0, lCol = 0;
    
    *opThresholdId = -1;
    if (iAxis == DssAxisRows)
    {
        lRow = iOrdinal;
        //going through all columns.
        for (lCol = 0; lCol < mpXTabView->getColCount(); lCol++)
        {
            hr = hgetCellThresholdId(lRow, lCol, ipAttrID, lpCube, bDummyIndex, pDummyIndex, &lThresholdId);
            if(FAILED(hr))
                return hr;
            if(hr!=S_OK)
                continue;
            
            if(lThresholdId>0)
            {
                if(!lbFindThreshold) //it is the first threshold id found in the current row.
                {
                    lbFindThreshold=true;
                    *opThresholdId=lThresholdId;
                }
                else
                    *opThresholdId = (*opThresholdId<lThresholdId) ? *opThresholdId:lThresholdId; //update the opThresholdId with smallest id when there are multiple thresholds in the current row.
            }
        }//end for
    }
    else if(iAxis == DssAxisColumns)
    {
        lCol=iOrdinal;
        //going through all rows within the incremental fetch range.
        for(lRow=mStartRow; lRow < mStartRow+mRowNum; lRow++)
        {
            hr = hgetCellThresholdId(lRow, lCol, ipAttrID, lpCube, bDummyIndex, pDummyIndex, &lThresholdId);
            if(FAILED(hr))
                return hr;
            if(hr!=S_OK)
                continue;
            
            if(lThresholdId>0)
            {
                if(!lbFindThreshold) //it is the first threshold id found in the current row.
                {
                    lbFindThreshold=true;							
                    *opThresholdId=lThresholdId;
                }						
                else
                    *opThresholdId = (*opThresholdId<lThresholdId) ? *opThresholdId:lThresholdId; //update the opThresholdId with smallest id.
            }
        }//end for
    }	
    
    if(lbFindThreshold)
        return S_OK;
    else
        return S_FALSE;
}

//iobIndex stores intermediate results only (transferred in for performance consideration). It is not input parameter nor output parameter.
int DSSAttributeThresholdIterator::hgetCellThresholdId(int iRow, int iCol, DSS_ID* ipAttrID, DSSCube* ipCube, bool* iobIndex, DSSXTabKey* iopIndex, int* opThresholdId)
{
    int hr = S_OK;
    AE_ASSERT(opThresholdId != NULL);
    
    int lMetric = 0, lTag = 0;
    DSSXTabKey lpChildIndex[mpXTabView->mnTotalUnits];
    for (int i=0; i<mpXTabView->mnTotalUnits; i++)
    {
        lpChildIndex[i] = -100;
    }
    
    hr = mpXTabView->FindIndexFromCoordinate(iRow, iCol, iopIndex, &lMetric, &lTag, NULL, NULL, lpChildIndex);
    CHECK_HR;
				
    //get the current level
    int lnCurrLevel=0;	
    for(int k = 0; k < mpXTabView->mnTotalUnits; k++)  //double check...
    {
        if (iopIndex[k] < 0)
            iobIndex[k] = false;
        else
            iobIndex[k] = true;
    }					
    mpXTabView->mpSliceMap->FindLevelInHash(iobIndex, &lnCurrLevel);					
    
    //here get slice lpThesholdSlice			
    int lSliceId;
    mpXTabView->getAttributeThresholdSliceInfo(lnCurrLevel, lTag, &mCurrAttr, &lSliceId);
    if (lSliceId == -1)
        return S_FALSE;										
    
    DSSCubeDataSlice* lpThesholdSlice = NULL; //get metric slice based on slice id;
    ipCube->get_SliceByID(lSliceId, &lpThesholdSlice);
    CHECK_PTR(lpThesholdSlice);		
    
    //parameters used in lpThesholdSlice->GetValueByIndex()
    const char *lpData = NULL;
    int lcbWritten = 0;
    DSSDataType_Type lDataType = DssDataTypeUnknown;
    DSSData_Flags lFlags = DssDataUnknown;
    //double lDummyDoubleValue = 0.0;
    //DE51599, use data index instead of display index
    hr = lpThesholdSlice->getVal(mpXTabView->mnTotalUnits, lpChildIndex, &lpData, lcbWritten, lDataType, lFlags, NULL);
    if (lpData && lDataType == DssDataTypeDouble)
    {
        *opThresholdId = *reinterpret_cast<const double *>(lpData);
    }
    return hr;
}
