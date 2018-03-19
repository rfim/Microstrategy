//
//  CDSSAttributeFormSort.cpp
//  ReportServiceCore
//
//  Created by Liao Taimin on 5/15/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#include "CDSSAttributeFormSort.h"
#import "DSSSortDefinition.h"
#import "DSSDataColumn.h"
#import "DSSCube.h"
#import "DSSTabularData.h"
#import "DSSTabularUnit.h"
#import "DSSGlobalLookUpTable.h"
#import "DSSXTabBitVector.h"
#include "DSSSort_includes.h"

CDSSAttributeFormSort::CDSSAttributeFormSort(Int32 nMetrics, Int32 nUnit)
: CDSSUnitBucketSort(nMetrics, nUnit), mfAscending(false),mfIsNullDisplayOnTop(false)
{
}


CDSSAttributeFormSort::~CDSSAttributeFormSort()
{
}



//## begin CDSSAttributeFormSort.declarations preserve=yes
int CDSSAttributeFormSort::Init(DSSSortDefinition *pDefn, Int32 *Key, Int32 nSize, DSSCube *pCube)
{
    int hr = S_OK;
    
	assert(pDefn != NULL);
	assert(pCube != NULL);
    
	Int32 lForm = 0;
	hr = pDefn -> get_AttributeAndForm(&mAttrIndex, &lForm);
	CHECK_HR;
    
	mLocale = pDefn -> mLocale;
    
    
	//use lAttr and lForm to find the column, then point to
	//the right key position
	DSSTabularData * lpTabularData = pCube -> getTabularData();
	CHECK_PTR(lpTabularData);
    
	//xhan 02/24/06 Add multi-lingual support
	DSSTabularUnit *lpUnit;
	hr = lpTabularData->Item(mAttrIndex, &lpUnit);
	CHECK_HR;
    
	DSSTabularAttribute *lpAttrPtr = dynamic_cast<DSSTabularAttribute*>(lpUnit);
	AE_ASSERT(lpAttrPtr);
    /*
	// 05/05/2008 mwang in case of DI connection mapping, the default column will be fetched
	CComPtr<ICDSSProject> lpProj;
	lpTabularData->GetSource()->get_Project(&lpProj);
	CComQIPtr<ICDSSProject6, &IID_ICDSSProject6> lpProj6(lpProj);
	EnumDSSDataMethod lDataMethod;
	lpProj6->get_DataMethodNS(&lDataMethod);
    */
	Int32 lColIndex = 0;
//	if (lDataMethod == DssDataMethodBySQL)
		hr = lpAttrPtr->FindFormWithLocale(0, lForm, mLocale, lColIndex);
//	else
//		hr = lpAttrPtr->FindFormWithLocale(0, lForm, DssLocaleNeutral, &lColIndex);
	CHECK_HR;
    
    
	hr = hInitByColunm(pDefn, Key, nSize, lpTabularData, lColIndex);
	CHECK_HR;
    
	return hr = S_OK;
}

//Used by Ana SQL
int CDSSAttributeFormSort::InitColumn(DSSSortDefinition *pDefn, Int32 *Key, Int32 nSize, DSSCube *pCube)
{
    int hr = S_OK;
    
	assert(pDefn != NULL);
	assert(pCube != NULL);
    
	//Form is interpreted as column
	Int32 lColumnIndex = 0;
	hr = pDefn -> get_AttributeAndForm(&mAttrIndex, &lColumnIndex);
	CHECK_HR;
    
	//xiyu, 349459, initialize locale
	mLocale = pDefn -> mLocale;
    
	DSSTabularData * lpTabularData = pCube -> getTabularData();
	CHECK_PTR(lpTabularData);
    
	hr = hInitByColunm(pDefn, Key, nSize, lpTabularData, lColumnIndex);
	CHECK_HR;
    
	return hr = S_OK;
    
}


bool CDSSAttributeFormSort::operator ()(Int32 Index1, Int32 Index2)
{
	// 06.26.2008 temporary fix for 301785
	// we reverse the order the NULLs in the descending case so that NullOnTop is consistent no matter ascending or descending
	Int32 result = mpCol->Compare(Index1, Index2, (mfAscending > 0) ? mfIsNullDisplayOnTop : !mfIsNullDisplayOnTop, mLocale) * mfAscending;
    
	return result < 0;
}

int CDSSAttributeFormSort::hInitByColunm(DSSSortDefinition *pDefn, Int32 *Key, Int32 nSize, DSSTabularData *ipTabularData, Int32 iColIndex)

{
	int hr = S_OK;

	//xhan 02/24/06 Add multi-lingual support
	DSSTabularUnit *lpUnit;
	hr = ipTabularData->Item(mAttrIndex, &lpUnit);
	CHECK_HR;
    
	DSSTabularAttribute *lpAttrPtr = dynamic_cast<DSSTabularAttribute*>(lpUnit);
	AE_ASSERT(lpAttrPtr);
    
 	hr = lpAttrPtr -> Item(iColIndex, &mpCol);
	CHECK_HR_PTR(mpCol);
    
	// get order direction
	mfAscending = (pDefn->isAscending) ? 1 : - 1;
	
	// get subtotal position
	mSubtotalPos = pDefn -> subtotalPosition;
    
	mfIsNullDisplayOnTop = pDefn->isNullOnTop;
	
	Int32 nRow = mpCol->getRowCount();
    
	if (nRow == 0)
	{
		mnBucket = 1;
		mpBucketLookupTable = new Int32 [1];
		mpBucketLookupTable[0] = 0;
		mKey = Key;
		return S_OK;
	}
    
	// jxie, 2012-6-18, TQMS 613738, handle Join NULL element in another way
	// rzhu, 01/07/2011, increment nRow by 1 to handle Join NULL element
	//nRow++;
    
	Int32 lnDisplayRow = 0;
	Int32* lpDisplayMap = NULL;
    
	// weiwang 2009-06-08 For BigDec type, the XTabColumnSorted is means sorted using simple comparison.
	DSSDataType_Type lColType = mpCol->getDataType();
	bool lbStringColumn = false;
	if (lColType == DssDataTypeVarChar || lColType == DssDataTypeUTF8Char)
		lbStringColumn = true;
    
	//sort according to the column contents
	bool lfColumnSorted = false;
	Int32* lpInputOrder = mpCol->GetInputOrder();
	if (lpInputOrder)
	{
		lfColumnSorted = true;
	}
    /*
	else if (mpCol->GetSortStatus() == XTabColumnSorted && mfAscending == 1 && mfIsNullDisplayOnTop
             && !(lbStringColumn && mLocale != 0))
	{
		lfColumnSorted = true;
	}*/
    
	if (lfColumnSorted && !(/*mpCol->IsMergedColumn()*/false && nSize == 0))
	{
		// ldong 06/17/2011, Red Rocket, UseInputOrder
		// if UseInputOrder, we directly return the original InputOrder as the display order
		Int32* lpInputOrder = mpCol->GetInputOrder();
		if (lpInputOrder)
		{// use InputOrder
			lnDisplayRow = nRow;
			if (mfAscending == 1)
				lpDisplayMap = lpInputOrder;
			else
			{// descending
				lpDisplayMap = new Int32[lnDisplayRow];
				CHECK_NEWED_PTR(lpDisplayMap);
                
				for (Int32 iRow = 0;iRow < nRow; iRow ++)
					lpDisplayMap[iRow] = lpInputOrder[nRow-1-iRow];
			}
		}else
		{// normal logic
            //if (mpCol->GetSortStatus() == XTabColumnSorted && mfAscending == 1 && mfIsNullDisplayOnTop
            //	&& !(lbStringColumn && mLocale != 0))
            
			// aye, 2/11/2009, TQMS 348157
			// if the column is already sorted, we can skip the _xtab_sort
			// A XTabColumnSorted means that the values are unique (i.e., each element is less than the next one),
			// and, if any NULL exists, it is the first one.
            
			Int32* lpSortOrder = mpCol->GetSortOrder();
			if (lpSortOrder)
			{
				// jxie, 2012-6-19, the join null element is handled alone, no need to call nRow-- now
				lnDisplayRow = nRow; // ldong 6/15/2012 TQMS 542000 nRow++ is called before, we need to do nRow-- here
				lpDisplayMap = lpSortOrder;
			}
			else
			{
				lnDisplayRow = nRow;
				lpDisplayMap = new Int32[lnDisplayRow];
				CHECK_NEWED_PTR(lpDisplayMap);
                
				for (Int32 i = 0; i < nRow; i++)
					lpDisplayMap[i] = i;
			}
		}
	}
	// jxie, 2012-9-12, TQMS 657037, change the threshold
    // hard code ltm
	else if (/*mpCol->IsMergedColumn()*/ false && /*ctang: threshold of sort the whole row or selected part*/nSize < nRow)
	{
		DSSXTabBitVector lSelectedKeyBV(nRow);
        
		Int32 lnSelectedRow = 0;
		Int32* lpKey = Key + mAttrIndex;
		bool lfJoinNullIncluded = false;
		for (Int32 i = 0; i < nSize; i++)
		{
			Int32 lKey = *lpKey;
			if (lKey >= 0 && lKey < nRow && !lSelectedKeyBV.Get(lKey) )	// lKey == nRow means it is Join NULL element
			{
				lSelectedKeyBV.Set(lKey, true);
				lnSelectedRow++;
			}
			else if (lKey == nRow)
				lfJoinNullIncluded = true;
			lpKey += mnUnit;
		}
        
		//rzhu, 01/18/2011, when it only contains Total element
		if (lnSelectedRow == 0)
		{
			// rzhu, 06/27/2012, may contain Join NULL
			if (lfJoinNullIncluded)
				mnBucket = nRow + 1;
			else
				mnBucket = 1;
            
			mpBucketLookupTable = new Int32 [mnBucket];
			memset(mpBucketLookupTable, 0, sizeof(Int32) * (mnBucket));
			mKey = Key;
			return S_OK;
		}
        
		lnDisplayRow = lnSelectedRow;
		lpDisplayMap = new Int32[lnDisplayRow];
		CHECK_NEWED_PTR(lpDisplayMap);
        
		lnSelectedRow = 0;
		// rzhu, 05/02/2012, 587599, the position of Join Null is incorrect. default to Top
		//if (lSelectedKeyBV.Get(nRow - 1))
        //lpDisplayMap[lnSelectedRow++] = nRow - 1;
        
		lSelectedKeyBV.SetSeekPosition(0);
		Int32 lRow = lSelectedKeyBV.GetNextSetPosition();
		while (lRow >= 0 && lRow < nRow)
		{
			lpDisplayMap[lnSelectedRow++] = lRow;
			lRow =lSelectedKeyBV.GetNextSetPosition();
		}
	}
	else
	{
		lnDisplayRow = nRow;
		lpDisplayMap = new Int32[lnDisplayRow];
		CHECK_NEWED_PTR(lpDisplayMap);
        
		// rzhu, 05/02/2012, 587599, the position of Join Null is incorrect. default to Top
		//lpDisplayMap[0] = nRow - 1;
		for (Int32 i = 0; i < nRow; i++)
			lpDisplayMap[i] = i;
		
	}
    
	if (!lfColumnSorted)
	{
		hr = _xtab_sort(lpDisplayMap, lpDisplayMap + lnDisplayRow, *this);
		//hr = _xtab_sort(lpDisplayMap, lpDisplayMap + nRow, *this);
		CHECK_HR;
        
		// jxie, 2012-10-10, TQMS 663232, not store the sort order since there may be some bugs if the data isn't strictly increasing
		//// jxie, 2012-9-12, TQMS 657037, store the sort order if the whole table is just sorted
		//if (lnDisplayRow == nRow && mpCol->GetSortStatus() != XTabColumnSorted && mfAscending == 1 && mfIsNullDisplayOnTop
		//	&& !(lbStringColumn && mLocale != 0) && mpCol->GetSortOrder() == NULL)
		//{
		//	mpCol->SetSortStatus(XTabColumnSorted);
		//	hr = mpCol->SetSortOrder(lnDisplayRow, lpDisplayMap);
		//	CHECK_HR;
		//}
	}
    
	mpBucketLookupTable = new Int32 [nRow + 1];
	CHECK_NEWED_PTR(mpBucketLookupTable);
    
	//this logic is same as rank.
	Int32 prevRank = 0;
	// jxie, 2012-6-18, put Join NULL element on top
	if (mfIsNullDisplayOnTop)
	{
		mpBucketLookupTable[nRow] = prevRank;
		prevRank++;
	}
	mpBucketLookupTable[lpDisplayMap[0]] = prevRank;
    
	for (Int32 iRow = 1; iRow < lnDisplayRow; iRow++)
	{
		//to see if the values are equal, if they are equal, they belong to same rank
		Int32 lResult = 0;
		if (lfColumnSorted)
			lResult = 1;
		else
			lResult = mpCol->Compare(lpDisplayMap[iRow - 1], lpDisplayMap[iRow], mfIsNullDisplayOnTop, mLocale);
        
		if (lResult == 0) {
			mpBucketLookupTable[lpDisplayMap[iRow]] = prevRank;
		} else {
			prevRank ++;
			mpBucketLookupTable[lpDisplayMap[iRow]] = prevRank;
		}
	}
    
	// jxie, 2012-6-18, put Join NULL element on bottom
	if (!mfIsNullDisplayOnTop)
	{
		prevRank++;
		mpBucketLookupTable[nRow] = prevRank;
	}
	
	mnBucket = prevRank + 1;
    
	mKey = Key;
    
	return S_OK;
    
}