//
//  DSSSliceCorrelator.cpp
//  ReportServiceCore
//
//  Created by Taimin Liao on 2/27/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#include "DSSSliceCorrelator.h"

#import "DSSTabularData.h"
#import "DSSCube.h"
#import "DSSViewDataSet.h"
#import "DSSDataColumn.h"
#import "DSSCubeDataSlice.h"

DSSSliceCorrelator::DSSSliceCorrelator()
:mCurrentRow(-1), mpTempKeys(NULL), mpCurrentKeys(NULL)
{}

DSSSliceCorrelator::~DSSSliceCorrelator()
{
	if (mpTempKeys)
		delete mpTempKeys;
	mpTempKeys = NULL;
    
	if (mpCurrentKeys)
		delete mpCurrentKeys;
	mpCurrentKeys = NULL;
}

// Setup the rowmaps of the lookup, input and output
int DSSSliceCorrelator::Init(DSSTabularIndex* ipLookupIndex, Int32 inInputs, DSSCubeDataSlice **ippInputSlices, DSSCubeDataSlice *ipOutputSlice, DSSCube* ipCube, bool iEnsureRowMap)//, MBase::Buffer *ipBuffer)
{
	Int32 i = 0;
	int hr = S_OK;
    
	mpLookupIndex = ipLookupIndex;
    
	DSSTabularData* lpTabularData = ipCube->getTabularData();
	CHECK_PTR(lpTabularData);
    
	hr = lpTabularData->Count(&mnTotalUnits);
	CHECK_HR;
    
	mpTempKeys = (Int32*) new Int32[mnTotalUnits];
	mpCurrentKeys = (Int32*) new Int32[mnTotalUnits];
	mCurrentRow = -1;
    
	// weiwang 2009-10-14 TQMS363750 support output slice as NULL
	Int32 lnSliceCount = (ipOutputSlice) ? inInputs + 1 : inInputs;
    
	for (i = 0; i < lnSliceCount; i++)
	{		
		DSSDataColumn* lpColumn = NULL;
		if (i == inInputs)
		{
			lpColumn = ipOutputSlice->getColumn();
			Int32 lRefUnit = -1;
			mvRefUnit.push_back(lRefUnit);	// the output cannot be a ref column
		}
		else
		{
            lpColumn = ippInputSlices[i]->getColumn();
            Int32 lRefUnit = -1;
			mvRefUnit.push_back(lRefUnit);
            /*
			DSSCubeSliceType lType;
			hr = ippInputSlices[i]->get_SliceType(&lType);
			CHECK_HR;
            
			// qjiang 06/08/2009
			// CDSSCubeSlice->get_Column may cause unnecessary data copy
			// here, if it is metric slice, we us get_ColumnC to try to get the column, if the column is null, don't use it.
			// use the slice to get data.
			bool lIsRefColumn = false;
			Int32 lRefUnit = -1;
            
			if(lType == DSS_METRIC_SLICE)
			{
				CDSSXTabColumn* lpColumnC = NULL;
				hr = static_cast<CDSSMetricSlice*>(ippInputSlices[i])->get_ColumnC(&lpColumnC, false);
				if(lpColumnC)
				{
					lpColumn = lpColumnC;
				}
				else
				{
					hr = static_cast<CDSSMetricSlice*>(ippInputSlices[i])->get_RefColumnC(&lpColumnC);
					CHECK_HR;
                    
					lpColumn = lpColumnC;
					lRefUnit = static_cast<CDSSMetricSlice*>(ippInputSlices[i])->GetRefUnit();
				}
			}
			else
			{
				hr = ippInputSlices[i]->get_Column(&lpColumn);
                
				// weiwang 2009-10-14 TQMS363750 column of ProperSlice is treated as RefColumn
				if (lType == DSS_PROPERTY_SLICE)
				{
					lRefUnit = static_cast<CDSSPropertySlice*>(ippInputSlices[i])->GetRefUnit();
				}
			}
            
			mvRefUnit.push_back(lRefUnit);
             */
		}
		CHECK_HR;
        
		//Record the column
		DSSDataColumn *lpCol = lpColumn;
		mvColumn.push_back(lpCol);
        
		DSSTabularIndex* lpSliceIndex;
		if (i == inInputs)
			lpSliceIndex = ipOutputSlice->getIndices();
		else
			lpSliceIndex = ippInputSlices[i]->getIndices();
		CHECK_HR;
        
		DSSTabularIndex *lpIndex = lpSliceIndex;
		mvIndex.push_back(lpIndex);
        
		Int32 *lpRowMap = NULL;
		if (mvRefUnit[i] != -1)		// if we are not using ref unit, setup the rowmap
			mvRowMap.push_back(lpRowMap);
		else
		{				
			ipCube->GetIndexRowMap(ipLookupIndex, lpSliceIndex, &lpRowMap);
			if (lpRowMap == NULL && iEnsureRowMap)
			{
				// yma 5/27/09, TQMS 361040
				Int32 lnRows = mpLookupIndex->Count();
                
				lpRowMap = new Int32[lnRows];
				CHECK_NEWED_PTR(lpRowMap);
                
				mvRowMap.push_back(lpRowMap);	// set it here to avoid memory leak
                
				for (Int32 lRow = 0; lRow < lnRows; lRow++)
				{
					hr = mpLookupIndex->ItemEx(lRow, mnTotalUnits, mpCurrentKeys);
					CHECK_HR;
                    
					hr = lpIndex->LookUpEx(mnTotalUnits, mpCurrentKeys, mpTempKeys, &lpRowMap[lRow]);
					CHECK_HR;
				}
			}
			else
				mvRowMap.push_back(lpRowMap);
		}			
	}
    
	// weiwang 2009-10-14 TQMS363750 support output slice as NULL
	mOutputSlicePos = (ipOutputSlice) ? inInputs : -1;
    
	return S_OK;
}

// Get/Set the numeric value
int DSSSliceCorrelator::GetInputValue(Int32 iSlice, Int32 iRow, double *opValue, DSSData_Flags *opFlag)
{
    int hr = S_OK;
    int lRow = iRow;
    if(mvIndex[iSlice]!=NULL)
    {
	int hr = GetRow(iSlice, iRow, lRow);
	CHECK_HR;
    }
	hr = mvColumn[iSlice]->getNumericVal(lRow, *opValue, *opFlag);
	CHECK_HR;
    
	return S_OK;
}

int DSSSliceCorrelator::SetOutputValue(Int32 iInputRow, double iValue, DSSData_Flags iFlag)
{
	// weiwang 2009-10-14 TQMS363750 support output slice as NULL
	AE_ASSERT(mOutputSlicePos >= 0);
    
	Int32 lRow = -1;
	int hr = GetRow(mOutputSlicePos, iInputRow, lRow);
	CHECK_HR;
    
	return mvColumn[mOutputSlicePos]->setNumeric(lRow, iValue, iFlag);
}

// Get/Set the raw data value
int DSSSliceCorrelator::GetInputValue(Int32 iSlice, Int32 iRow, BYTE **pData, Int32 cbSize, Int32 *pcbWritten, DSSDataType_Type *pDataType, DSSData_Flags *pFlags)
{
	Int32 lRow = -1;
	int hr = GetRow(iSlice, iRow, lRow);
	CHECK_HR;
    
	return mvColumn[iSlice]->getData(lRow, (const char **)pData, *pcbWritten, *pDataType, *pFlags);
}

int DSSSliceCorrelator::SetOutputValue(Int32 iInputRow, BYTE *pData, Int32 cbSize, EnumDSSDataType Type, DSSData_Flags Flags)
{
	// weiwang 2009-10-14 TQMS363750 support output slice as NULL
	AE_ASSERT(mOutputSlicePos >= 0);
    
	Int32 lRow = -1;
	int hr = GetRow(mOutputSlicePos, iInputRow, lRow);
	CHECK_HR;
    
	return mvColumn[mOutputSlicePos]->setData(lRow, (char *)pData, cbSize, Flags);
}

int DSSSliceCorrelator::GetRow(Int32 iSlice, Int32 iRow, Int32 &orRow)
{
	orRow = XTabIndexRowMapLookup(mvRowMap[iSlice], iRow);
    
	if (orRow >= 0)
		return S_OK;
    
	int hr = S_OK;
	if (mCurrentRow != iRow)
	{
		hr = mpLookupIndex->ItemEx(iRow, mnTotalUnits, mpCurrentKeys);
		CHECK_HR;
        
		mCurrentRow = iRow;
	}
    
	Int32 lUnit = mvRefUnit[iSlice];
	if (lUnit != -1)
	{
		orRow = mpCurrentKeys[lUnit];
	}
	else
	{
		hr = mvIndex[iSlice]->LookUpEx(mnTotalUnits, mpCurrentKeys, mpTempKeys, &orRow);
		CHECK_HR;
	}
    
	if (orRow < 0)
    {
        orRow = iRow;
    }
    return S_OK;
}