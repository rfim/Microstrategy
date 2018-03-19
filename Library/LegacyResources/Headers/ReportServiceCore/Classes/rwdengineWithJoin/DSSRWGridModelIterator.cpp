/*
 *  DSSRWGridModelIterator.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 9/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

//#import <set>

//#include "DSSRWNodeModelIterator.h"
//#include "DSSRWGridModelIterator.h"
//#include "DSSRWSectionModelIterator.h"
//#include "DSSRWGridModel.h"
//#include "DSSXTabView2.h"
//
//
//#import "DSSAEHelper.h"
//#import "DSSTabularUnit.h"
//#import "DSSTabularAttribute.h"
//#import "DSSTabularCustomGroup.h"
//#import "DSSTabularConsolidation.h"
//#import "DSSDataColumn.h"
//#import "DSSXTabHeader.h"
//#import "DSSCellHandle.h"
//#import "DSSTabularMetrics.h"
//#import "DSSCubeDataSlice.h"
//
//
//#import "DSSRWTemplateNode.h"
//#import "DSSTemplateMetrics.h"
//#import "DSSDrillPathRefs.h"
//#import "DSSTemplateCustomGroup.h"
//#import "DSSXTabHashTable.h"
//#import "DSSSort_includes.h"
//#import "DSSDataModelTransaction.h"
//#import "DSSTransactionReport.h"
//#import "DSSDocumentDefinition.h"
//#import "DSSCalculationPlan.h"
//#import "DSSCube.h"
//
//#include "MsiCOM/MsiCOM/Msi_atlbase.h"

//DSSRWGridModelIterator::DSSRWGridModelIterator():mpGridModel(NULL), mRowOrdinal(0), mColOrdinal(0), mpXTabView(NULL)
//,mpTemplateMetrics(NULL), mbNodeHasGBUnits(false), mbHasMetricOnRow(false)
//,mnRowGBUnits(0), mnColGBUnits(0), mbPivotGB(false), mpGroupbyGridIterator(NULL), mpSliceWindowGridIterator(NULL)
//,mRowSliceWindowSize(0), mColSliceWindowSize(0), mRowSliceWindowID(0), mColSliceWindowID(0)
//,mbExcludeTotal(false), mpRowColorMap(NULL), mpColColorMap(NULL), mnColorByRows(0), mnColorByCols(0)
//,mpParentGridIterator(NULL), mCBRowStartOrdinal(0), mCBColStartOrdinal(0)
//{}
//
//DSSRWGridModelIterator::~DSSRWGridModelIterator()
//{
//	// hCleanUp();
//}

//int DSSRWGridModelIterator::Init(DSSRWGridModel *ipGridModel, DSSRWSectionModelIterator *ipParent)
//{
//    DSSRWNodeModelIterator::Init(ipGridModel->mpDataModel, ipGridModel->mpNode, ipGridModel, ipParent);
//    if(ipGridModel->mpExtRuntimeTemplate != NULL)
//		mpTemplate = ipGridModel->mpExtRuntimeTemplate;
//	else
//		mpTemplate = ipGridModel->mpRuntimeTemplate;
//    
//    mpXTabView = ipGridModel->mpXTabView;
//    
//    return ReInit();        
//}
//
//int DSSRWGridModelIterator::ReInit()
//{
//	int hr = S_OK;
//    
//	/*hr = hInitTemplateStructure(lpThisNode);
//     CHECK_HR;*/
//    
//	vector<vector<int> > lvvActualGBElements;
//	int lLocalTag = -1;
//	static_cast<DSSRWGridModel*>(mpNodeModel)->GetActualGBElements(lvvActualGBElements, lLocalTag);
//	hr = mpXTabView->ResetCurrentSelection(lvvActualGBElements, lLocalTag);
//	CHECK_HR;
//	hr = mpXTabView->GetCurrentWindow(mvRowRange, mvColumnRange);
//	CHECK_HR;
//    
//    /*
//    for (int iAxis = 0; iAxis < 2; iAxis++)
//	{
//		DSSXTabHeader *lpHeader = mpXTabView->getHeader(iAxis);
//		CHECK_PTR(lpHeader);
//        
//		//support "disable subtoal" in graph
//		if (mbExcludeTotal && lpHeader->getLevelCount() > 1)
//		{
//			int lSize = 0;
//			vector<int>::iterator start;
//			vector<int>::iterator end;
//			if (iAxis == 0)
//			{
//				start = mvRowWindow.begin();
//				end = mvRowWindow.end();
//				lSize = mvRowWindow.size();
//			}
//			else
//			{
//				start = mvColumnWindow.begin();
//				end = mvColumnWindow.end();
//				lSize = mvColumnWindow.size();
//			}
//            
//			vector<int> lvTemp;
//			lvTemp.reserve(lSize);
//			for (vector<int>::iterator it = start; it < end; it++)
//			{
//				if (lpHeader->getLevel(*it) == 0)
//					lvTemp.push_back(*it);
//			}
//            
//			if (lvTemp.size() < lSize)
//			{
//				if (iAxis == 0)
//				{
//					mvRowWindow.clear();
//					mvRowWindow.assign(lvTemp.begin(), lvTemp.end());
//				}
//				else
//				{
//					mvColumnWindow.clear();
//					mvColumnWindow.assign(lvTemp.begin(), lvTemp.end());
//				}
//			}
//		}
//        
//		int lnRows = iAxis == 0 ? mvRowWindow.size() : mvColumnWindow.size();
//		vector<int> *lpvRowRange = iAxis == 0 ? &mvRowWindow : &mvColumnWindow;
//		int i = 0;
//		int lnUnits = lpHeader->getNumUnits() - lpHeader ->getNumRuntimeUnits();
//        
//		bool *lpHierarchicalDisplayFlag = new bool[lnUnits];
//		for (i = 0; i < lnUnits; i++)
//		{
//			DSSTemplateUnit *lpTemplateUnit = mvpTemplateUnits[lpHeader->getUnit(i)];
//			AE_ASSERT(lpTemplateUnit);	//481143;
//			if (lpTemplateUnit->getType() == DssTemplateCustomGroup && !static_cast<DSSTemplateCustomGroup*> (lpTemplateUnit)->getFlatten())
//				lpHierarchicalDisplayFlag[i] = true;
//			else
//				lpHierarchicalDisplayFlag[i] = false;
//		}
//        
//		//get ordinal and element keys
//		vector<int> lvUnitID;
//		hr = lpHeader->getHeaderElements(lnRows, &(*lpvRowRange)[0], lnUnits, lpHierarchicalDisplayFlag, lvUnitID,
//										 iAxis == 0 ? mvRowEleOrdinals : mvColEleOrdinals,
//										 iAxis == 0 ? mvRowEleKeys : mvColEleKeys);
//		delete lpHierarchicalDisplayFlag;
//		CHECK_HR;
//        
//		mHeader[iAxis].mnRows = lnRows;
//		mHeader[iAxis].mnUnits = lnUnits;
//        
//		//get header width
//		int lWidth = iAxis == 0 ? mvRowEleKeys.size() : mvColEleKeys.size();
//		mHeader[iAxis].mWidth = lWidth;
//        
//		//initialize unit cursor
//		if (iAxis == 0)
//		{
//			mvRowUnitCursors.resize(lWidth);
//			for (i = 0; i < lWidth; i++)
//				mvRowUnitCursors[i] = 0;
//		}
//		else
//		{
//			mvColUnitCursors.resize(lWidth);
//			for (i = 0; i < lWidth; i++)
//				mvColUnitCursors[i] = 0;
//		}
//        
//		//calculate unit width
//		int lnDistinct = lWidth > 0 ? 1 : 0;
//		for (i = 0; i < lWidth - 1; i++)
//			if (lvUnitID[i] != lvUnitID[i + 1])
//				lnDistinct++;
//        
//		if (lnDistinct != lnUnits && lWidth != 0)
//			return E_FAIL;
//        
//		mHeader[iAxis].mvUnitWidth.resize(lnUnits);
//		for (i = 0; i < lnUnits; i++)
//		{
//			mHeader[iAxis].mvUnitWidth[i] = 0;
//		}
//        
//		int w = 1;		//unit width
//		int uid = 0;	//unit id
//		for (i = 0; i < lWidth - 1; i++)
//		{
//			mHeader[iAxis].mvDepthToUnit.push_back(uid);
//			if (lvUnitID[i] != lvUnitID[i + 1])
//			{
//				mHeader[iAxis].mvUnitWidth[uid++] = w;
//				w = 1;
//			}
//			else
//			{
//				w++;
//			}
//		}
//		if (i < lWidth)
//		{
//			mHeader[iAxis].mvUnitWidth[uid] = w;	//the last unit
//			mHeader[iAxis].mvDepthToUnit.push_back(uid);	//the last depth
//		}
//        
//		//cache xtab unit
//		for (i = 0; i < lWidth; i++)
//		{
//			DSSTabularUnit *lpUnit = NULL;
//			hr = hGetUnit(iAxis, i, &lpUnit);
//			CHECK_PTR(lpUnit);
//            
//			int lUnitIndex = mHeader[iAxis].mvDepthToUnit[i];	//map depth to unit
//            
//			if (iAxis == 0)
//			{
//				mvRowXTabUnits.push_back(lpUnit);
//				mvRowTemplateUnits.push_back(mvpTemplateUnits[lpHeader->getUnit(lUnitIndex)]);
//			}
//			else
//			{
//				mvColXTabUnits.push_back(lpUnit);
//				mvColTemplateUnits.push_back(mvpTemplateUnits[lpHeader->getUnit(lUnitIndex)]);
//			}
//		}
//        
//		//create key table
//		if (iAxis == 0)
//			hCreateKeyTable(lnRows, mvRowEleOrdinals, mvRowEleKeys, mvRowKeyTable);
//		else
//			hCreateKeyTable(lnRows, mvColEleOrdinals, mvColEleKeys, mvColKeyTable);
//	}
//    
//	//457191, don't fail during intialization
//	hr = hBuildTransactionUnitMap(lpThisNode);
//    */
//	return hr;
//}
//
//
//int DSSRWGridModelIterator::GetTemplate(DSSTemplate ** oppTemplate)
//{
//	if (!oppTemplate) return E_POINTER;
//	*oppTemplate = mpTemplate;
//    return S_OK;   
//}
//
//template<class T>
//bool BinarySearch(T val, vector<T> &rvSortedList, int start, int end, int &orOffset)
//{
//	if (start < 0 || start >= rvSortedList.size() || start > end)
//		return false;
//    
//	int *lpList = &rvSortedList[0];
//	int left = start;
//	int right = end;
//	while(left < right)
//	{
//		int mid = (right - left) / 2 + left;
//        
//		if (val >= lpList[mid] && val <= lpList[mid + 1])
//		{
//			orOffset = mid;
//			return true;
//		}
//		else if (val < lpList[mid])
//			right = mid - 1;
//		else if (val > lpList[mid + 1])
//			left = mid + 1;
//	}//end of while (left < right)
//    
//	orOffset = right;	//left >= right
//    
//	if (val < lpList[orOffset] || val > lpList[orOffset + 1])
//	{
//		_ASSERT(false);
//		return false;
//	}
//    
//	return true;
//}
//
//
//template<class T>
//class ElementSort
//{
//public:
//	ElementSort(T *ipKey, int nRows, int nUnits): mpKey(ipKey), mnRows(nRows), mnUnits(nUnits), mCur(0)
//	{}
//    
//	inline void ReInit(T *ipKey, int nRows, int nUnits)
//	{
//		mpKey = ipKey;
//		mnRows = nRows;
//		mnUnits = nUnits;
//		mCur = 0;
//	}
//    
//	inline void SetUnit(int iUnit)
//	{	if (iUnit < mnUnits)	mCur = iUnit; }
//    
//	bool operator() (int iRow1, int iRow2)
//	{
//		return mpKey[iRow1 * mnUnits + mCur] < mpKey[iRow2 * mnUnits + mCur];
//	}
//    
//private:
//	T *mpKey;
//	int mnRows;
//	int mnUnits;
//	int mCur;
//};
//
//
//
//int DSSRWGridModelIterator::Init(DSSRWNode *ipNode, DSSRWSectionModelIterator * ipParent, vector<DSSBaseElementProxy*>& ivNGBElements)
//{
//    return E_NOTIMPL;
//}
//
//void DSSRWGridModelIterator::hCleanUp(int ibFlag)
//{
//    // E_NOTIMPL
//}
//
//int DSSRWGridModelIterator::CountRows()
//{
//	return mHeader[0].mnRows;
//}
//
//int DSSRWGridModelIterator::CountColumns()
//{
//	return mHeader[1].mnRows;
//}
//
//bool DSSRWGridModelIterator::FirstRow()
//{
//	mRowOrdinal = 0;
//	for (vector<int>::iterator it = mvRowUnitCursors.begin(); it < mvRowUnitCursors.end(); it++)
//	{
//		*it = 0;
//	}
//	return mHeader[0].mnRows > 0;
//}
//
///*bool DSSRWGridModelIterator::LastRow()
// {
// if (mHeader[0].mnRows > 0)
// {
// mRowOrdinal = mHeader[0].mnRows - 1;
// vector<int>::iterator it = mvRowUnitCursors.begin();
// vector<vector<int> >::iterator itEles = mvRowEleKeys.begin();
// for (; it < mvRowUnitCursors.end(); it++, itEles++)
// {
// *it = itEles->size() - 1;
// }
// return true;
// }
// else
// return false;
// }*/
//
//bool DSSRWGridModelIterator::NextRow()
//{
//	if (mRowOrdinal < mHeader[0].mnRows - 1)
//	{
//		mRowOrdinal++;
//		hIncreaseCursors(mRowOrdinal, 1, mvRowEleOrdinals, mvRowUnitCursors);
//		return true;
//	}
//	else
//		return false;
//}
//
//bool DSSRWGridModelIterator::PreviousRow()
//{
//	if (mRowOrdinal > 0)
//	{
//		mRowOrdinal--;
//		hIncreaseCursors(mRowOrdinal, -1, mvRowEleOrdinals, mvRowUnitCursors);
//		return true;
//	}
//	else
//		return false;
//}
//
//bool DSSRWGridModelIterator::FirstColumn()
//{
//	mColOrdinal = 0;
//	for (vector<int>::iterator it = mvColUnitCursors.begin(); it < mvColUnitCursors.end(); it++)
//	{
//		*it = 0;
//	}
//	return mHeader[1].mnRows > 0;
//}
//
///*bool DSSRWGridModelIterator::LastColumn()
// {
// if (mHeader[1].mnRows > 0)
// {
// mColOrdinal = mHeader[1].mnRows - 1;
// vector<int>::iterator it = mvColUnitCursors.begin();
// vector<vector<int> >::iterator itEles = mvColEleKeys.begin();
// for (; it < mvColUnitCursors.end(); it++, itEles++)
// {
// *it = itEles->size() - 1;
// }
// return true;
// }
// else
// return false;
// }*/
//
//bool DSSRWGridModelIterator::NextColumn()
//{
//	if (mColOrdinal < mHeader[1].mnRows - 1)
//	{
//		mColOrdinal++;
//		hIncreaseCursors(mColOrdinal, 1, mvColEleOrdinals, mvColUnitCursors);
//		return true;
//	}
//	else
//		return false;
//}
//
//bool DSSRWGridModelIterator::PreviousColumn()
//{
//	if (mColOrdinal > 0)
//	{
//		mColOrdinal--;
//		hIncreaseCursors(mColOrdinal, -1, mvColEleOrdinals, mvColUnitCursors);
//		return true;
//	}
//	else
//		return false;
//}
//
//int DSSRWGridModelIterator::getNumUnits(int iAxis)
//{
//	if (iAxis == 0 || iAxis == 1)
//		return mHeader[iAxis].mnUnits;
//	else
//		return 0;
//}
//
//int DSSRWGridModelIterator::getHeaderWidth(int iAxis)
//{
//	if (iAxis == 0 || iAxis == 1)
//		return mHeader[iAxis].mWidth;
//	else
//		return 0;
//}
//
//int DSSRWGridModelIterator::getUnitWidth(int iAxis, int iUnit)
//{
//	if (iAxis == 0 || iAxis == 1)
//	{
//		if (iUnit >= 0 && iUnit < mHeader[iAxis].mvUnitWidth.size())
//			return mHeader[iAxis].mvUnitWidth[iUnit];
//	}
//	return 0;
//}
//
//int DSSRWGridModelIterator::getUnitDepth(int iAxis, int iUnits)
//{
//	int depth = 0;
//	if (iAxis == 0 || iAxis == 1)
//	{
//		for (int i = 0; i < iUnits && i < mHeader[iAxis].mvUnitWidth.size(); i++)
//		{
//			depth += mHeader[iAxis].mvUnitWidth[i];
//		}
//	}
//	return depth;
//}
//
//int DSSRWGridModelIterator::getRowOrdinal()
//{
//	return mRowOrdinal;
//}
//
//int DSSRWGridModelIterator::getColOrdinal()
//{
//	return mColOrdinal;
//}
//
//int DSSRWGridModelIterator::getRowElementRange(int iDepth, int* opStart, int* opEnd)
//{
//	if (!opStart || !opEnd)
//		return E_INVALIDARG;
//    
//	*opStart = mvRowEleOrdinals[iDepth][mvRowUnitCursors[iDepth]];
//	*opEnd = mvRowEleOrdinals[iDepth][mvRowUnitCursors[iDepth] + 1];
//    
//	return S_OK;
//}
//
//int DSSRWGridModelIterator::getColElementRange(int iDepth, int* opStart, int* opEnd)
//{
//	if (!opStart || !opEnd)
//		return E_INVALIDARG;
//    
//	*opStart = mvColEleOrdinals[iDepth][mvColUnitCursors[iDepth]];
//	*opEnd = mvColEleOrdinals[iDepth][mvColUnitCursors[iDepth] + 1];
//    
//	return S_OK;
//}
//
//int DSSRWGridModelIterator::getElementSpan(int iAxis, int iDepth)
//{
//	if (iAxis == 0)
//	{
//		int cursor = mvRowUnitCursors[iDepth];
//		return mvRowEleOrdinals[iDepth][cursor + 1] - mvRowEleOrdinals[iDepth][cursor];
//	}
//	else if (iAxis == 1)
//	{
//		int cursor = mvColUnitCursors[iDepth];
//		return mvColEleOrdinals[iDepth][cursor + 1] - mvColEleOrdinals[iDepth][cursor];
//	}
//	return 0;
//}
//
//int DSSRWGridModelIterator::getElementOffsetInSpan(int iAxis, int iDepth, int iOrdinal)
//{
//	if (iAxis == 0)
//	{
//		int cursor = mvRowUnitCursors[iDepth];
//		return iOrdinal - mvRowEleOrdinals[iDepth][cursor];
//	}
//	else if (iAxis == 1)
//	{
//		int cursor = mvColUnitCursors[iDepth];
//		return iOrdinal - mvColEleOrdinals[iDepth][cursor];
//	}
//	return 0;
//}
//
//bool DSSRWGridModelIterator::IsRowElementSubtotal(int iDepth)
//{
//	return mvRowEleKeys[iDepth][mvRowUnitCursors[iDepth]] < 0;
//}
//
//bool DSSRWGridModelIterator::IsColElementSubtotal(int iDepth)
//{
//	return mvColEleKeys[iDepth][mvColUnitCursors[iDepth]] < 0;
//}
//
//EnumDSSMetricType DSSRWGridModelIterator::getRowSubtotalType()
//{
//	for (int iDepth = mHeader[0].mWidth - 1; iDepth >= 0; iDepth--)
//	{
//		int key = mvRowEleKeys[iDepth][mvRowUnitCursors[iDepth]];
//		if (key < 0)
//		{
//			return mpGridModel->getSubtotalType(-key - 1);
//		}
//	}
//	return DssMetricReserved;
//}
//
//EnumDSSMetricType DSSRWGridModelIterator::getColSubtotalType()
//{
//	for (int iDepth = mHeader[1].mWidth - 1; iDepth >= 0; iDepth--)
//	{
//		int key = mvColEleKeys[iDepth][mvColUnitCursors[iDepth]];
//		if (key < 0)
//		{
//			return mpGridModel->getSubtotalType(-key - 1);
//		}
//	}
//	return DssMetricReserved;
//}
//
//int DSSRWGridModelIterator::getRowElementValue(int iDepth, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag)
//{
//	if (!oppData || !opDataSize || !opDataFlag || !opDataType)
//		return E_INVALIDARG;
//    
//	DSSTabularUnit *lpUnit = mvRowXTabUnits[iDepth];
//	CHECK_PTR(lpUnit);
//    
//	int lKey = mvRowEleKeys[iDepth][mvRowUnitCursors[iDepth]];
//    
//	if (lpUnit->getUnitType() == DssTemplateCustomGroup && iDepth > 0)
//	{
//		if (mvRowXTabUnits[iDepth - 1] == lpUnit)
//		{
//			int lParentKey = mvRowEleKeys[iDepth - 1][mvRowUnitCursors[iDepth - 1]];
//			if (lKey == lParentKey)
//			{
//				//lwang, 415238, skip the repeat element in hierarchical display
//				*oppData = NULL;
//				*opDataSize = 0;
//				*opDataType = DssDataTypeVarChar;
//				*opDataFlag = DssDataOk;
//				return S_OK;
//			}
//		}
//	}
//    
//	return hGetUnitValue(lpUnit, lKey, iBaseForm, oppData, opDataSize, opDataType, opDataFlag);
//}
//
//int DSSRWGridModelIterator::getRowElementValue(int iDepth, int iBaseForm, std::string &orText, DSSDataFlag *opDataFlag)
//{
//	const void *lpData;
//	int lSize = 0;
//	EnumDSSDataType lDataType = DssDataTypeReserved;
//	DSSDataFlag lFlag = DssDataOk;
//	int hr = getRowElementValue(iDepth, iBaseForm, &lpData, &lSize, &lDataType, &lFlag);
//	CHECK_HR;
//    
//	if (lFlag == S_OK)
//		return DSSAEHelper::ConvertToString(lpData, lSize, lDataType, lFlag, orText);
//	else
//		return mpGridModel->getElementNullDisplay(lFlag, orText);
//}
//
//int DSSRWGridModelIterator::getColElementValue(int iDepth, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag)
//{
//	if (!oppData || !opDataSize || !opDataFlag || !opDataType)
//		return E_INVALIDARG;
//    
//	DSSTabularUnit *lpUnit = mvColXTabUnits[iDepth];
//	CHECK_PTR(lpUnit);
//    
//	int lKey = mvColEleKeys[iDepth][mvColUnitCursors[iDepth]];
//    
//	if (lpUnit->getUnitType() == DssTemplateCustomGroup && iDepth > 0)
//	{
//		if (mvColXTabUnits[iDepth - 1] == lpUnit)
//		{
//			int lParentKey = mvColEleKeys[iDepth - 1][mvColUnitCursors[iDepth - 1]];
//			if (lKey == lParentKey)
//			{
//				//lwang, 415238, skip the repeat element in hierarchical display
//				*oppData = NULL;
//				*opDataSize = 0;
//				*opDataType = DssDataTypeVarChar;
//				*opDataFlag = DssDataOk;
//				return S_OK;
//			}
//		}
//	}
//    
//	return hGetUnitValue(lpUnit, lKey, iBaseForm, oppData, opDataSize, opDataType, opDataFlag);
//}
//
//int DSSRWGridModelIterator::getColElementValue(int iDepth, int iBaseForm, std::string &orText, DSSDataFlag *opDataFlag)
//{
//	const void *lpData;
//	int lSize = 0;
//	EnumDSSDataType lDataType = DssDataTypeReserved;
//	DSSDataFlag lFlag = DssDataOk;
//	int hr = getColElementValue(iDepth, iBaseForm, &lpData, &lSize, &lDataType, &lFlag);
//	CHECK_HR;
//    
//	if (lFlag == S_OK)
//		return DSSAEHelper::ConvertToString(lpData, lSize, lDataType, lFlag, orText);
//	else
//		return mpGridModel->getElementNullDisplay(lFlag, orText);
//}
//
//int DSSRWGridModelIterator::hGetUnitValue(DSSTabularUnit *ipUnit, int iKey, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag)
//{
//	if (iKey >= 0)
//	{
//		EnumDSSTemplateUnitType lUnitType = ipUnit->getUnitType();
//		switch (lUnitType) {
//			case DssTemplateMetrics:
//				return hGetMetricName(iKey, oppData, opDataSize, opDataType, opDataFlag);
//			default:
//				return mpXTabView->getUnitValue(ipUnit, iKey, iBaseForm, oppData, opDataSize, opDataType, opDataFlag);
//		}
//	}
//	else
//	{
//		return hGetSubtotalName(-iKey - 1, oppData, opDataSize, opDataType, opDataFlag);
//	}
//	return S_OK;
//}
//
//int DSSRWGridModelIterator::getCellValue(const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag, int *opThreshold, int *opMetric)
//{
//	if (!opDataSize || !opDataType || !opDataFlag)
//		return E_INVALIDARG;
//    
//	return mpXTabView->GetRawCellInfo(mvRowWindow[mRowOrdinal], mvColumnWindow[mColOrdinal], (const char**)oppData, *opDataSize, *opDataType, *opDataFlag, opThreshold, opMetric);
//}
//
//int DSSRWGridModelIterator::getCellValue(double *opVal, DSSDataFlag *opDataFlag, int *opThreshold, int *opMetric)
//{
//	if (!opVal || !opDataFlag)
//		return E_INVALIDARG;
//    
//	return mpXTabView->GetCellValue(mvRowWindow[mRowOrdinal], mvColumnWindow[mColOrdinal], *opVal, *opDataFlag, opThreshold, opMetric);
//}
//
//int DSSRWGridModelIterator::hGetUnit(int iAxis, int iDepth, DSSTabularUnit **oppUnit)
//{
//	DSSXTabHeader *lpHeader = mpXTabView->getHeader(iAxis);
//	CHECK_PTR(lpHeader);
//    
//	int lUnit = mHeader[iAxis].mvDepthToUnit[iDepth];	//map depth to unit
//	int lUnitID = lpHeader->getUnit(lUnit);
//    
//	if (oppUnit)
//		*oppUnit = mpGridModel->Item(lUnitID);
//	return S_OK;
//}
//
//int DSSRWGridModelIterator::hGetSubtotalName(int iSubtotalID, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag)
//{
//	*opDataFlag = DssDataOk;
//	*opDataType = DssDataTypeVarChar;
//	*oppData = mpGridModel->getSubtotalNameAndLength(iSubtotalID, *opDataSize);
//	if (!oppData)
//		return E_FAIL;
//	return S_OK;
//}
//
//int DSSRWGridModelIterator::hGetMetricName(int iMetricID, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag)
//{
//	if (!mpTemplateMetrics || iMetricID < 0 || iMetricID >= mpTemplateMetrics->Count())
//		return E_INVALIDARG;
//    
//	*opDataFlag = DssDataOk;
//	*opDataType = DssDataTypeVarChar;
//    
//	DSSTemplateMetric * lpTemplateMetric = mpTemplateMetrics->Item(iMetricID);
//	std::string lAlias = lpTemplateMetric->getAlias();
//    
//	*oppData = lAlias.c_str();
//	*opDataSize = lAlias.length();
//    
//	return S_OK;
//}
//
//void DSSRWGridModelIterator::hIncreaseCursors(int ordinal, int inc, vector<vector<int> > &irvEleOrdinals, vector<int> &rvCursors)
//{
//	int width = rvCursors.size();
//	for (int i = width - 1; i >= 0; i--)
//	{
//		if (irvEleOrdinals[i][rvCursors[i] + inc] == ordinal)
//		{
//			rvCursors[i] += inc;
//		}
//		else
//			break;
//	}
//}
//
//DSSCellHandle DSSRWGridModelIterator::getCellHandle()
//{
//	int lnCols = mpXTabView->getHeader(1)->getRowCount();
//	int iRow = mvRowWindow[mRowOrdinal];
//	int iCol = mvColumnWindow[mColOrdinal];
//	return DSSCellHandle(DssCellHandleXTabData, iRow * lnCols + iCol);
//}
//
//DSSCellHandle DSSRWGridModelIterator::getElementHandle(int iAxis, int iDepth)
//{
//	if (iAxis == 0 && iDepth >= 0 && iDepth < mvRowXTabUnits.size())
//		return DSSCellHandle(DssCellHandleXTabHeaderElement, mvRowXTabUnits[iDepth], mRowOrdinal, mvRowEleKeys[iDepth][mvRowUnitCursors[iDepth]]);
//	else if (iAxis == 1 && iDepth >= 0 && iDepth < mvColXTabUnits.size())
//		return DSSCellHandle(DssCellHandleXTabHeaderElement, mvColXTabUnits[iDepth], mColOrdinal, mvColEleKeys[iDepth][mvColUnitCursors[iDepth]]);
//	return DSSCellHandle(DssCellHandleReserved, 0);
//}
//
//DSSCellHandle DSSRWGridModelIterator::getCellHandleAt(int iRowOrdinal, int iColOrdinal)
//{
//	int lnCols = mpXTabView->getHeader(1)->getRowCount();
//	int iRow = mvRowWindow[iRowOrdinal];
//	int iCol = mvColumnWindow[iColOrdinal];
//	return DSSCellHandle(DssCellHandleXTabData, iRow * lnCols + iCol);
//}
//
//DSSCellHandle DSSRWGridModelIterator::getElementHandleAt(int iAxis, int iOrdinal, int iDepth, int iFormId)
//{
//	// Enhance this method to return the last element if the depth is set to -1
//	if (iAxis == 0)
//	{
//		if (iDepth == -1)
//			iDepth = mvRowXTabUnits.size() - 1;
//        
//		if (iDepth >= 0 && iDepth < mvRowXTabUnits.size())
//			//int lElementIndex = mvRowUnitCursors[iDepth];		//lElementOffset is the i-th element in mvRowEleKeys[iDepth]
//			return DSSCellHandle(DssCellHandleXTabHeaderElement, mvRowXTabUnits[iDepth], iOrdinal, mvRowKeyTable[iDepth * mHeader[0].mnRows + iOrdinal], iFormId);
//	}
//	else
//	{
//		if (iDepth == -1)
//			iDepth = mvColXTabUnits.size() - 1;
//        
//		if (iDepth >= 0 && iDepth < mvColXTabUnits.size())
//			//int lElementIndex = mvColEleKeys[iDepth][iOrdinal];
//			return DSSCellHandle(DssCellHandleXTabHeaderElement, mvColXTabUnits[iDepth], iOrdinal, mvColKeyTable[iDepth * mHeader[1].mnRows + iOrdinal], iFormId);
//	}
//	return DSSCellHandle(DssCellHandleReserved, 0);
//}
//
//DSSCellHandle DSSRWGridModelIterator::getUnitHandle(int iAxis, int iDepth, int iFormIndex)
//{
//	DSSTemplateUnit *lpTemplateUnit = getUnit(iAxis, iDepth);
//	if (!lpTemplateUnit)
//		return DSSCellHandle(DssCellHandleReserved, 0);
//    
//	if (lpTemplateUnit->getType() == DssTemplateAttribute)
//	{
//		if (iFormIndex >= 0)
//			return DSSCellHandle(DssCellHandleTemplateUnitAndForm, lpTemplateUnit, iFormIndex);
//		else
//			return DSSCellHandle(DssCellHandleTemplateUnit, lpTemplateUnit, 0);
//	}
//	else if (lpTemplateUnit->getType() == DssTemplateCustomGroup)
//	{
//		return DSSCellHandle(DssCellHandleCustomGroupAndDepth, lpTemplateUnit, 0);		//TODO: set depth
//	}
//	else
//	{
//		return DSSCellHandle(DssCellHandleTemplateUnit, lpTemplateUnit, 0);
//	}
//}
//
//int DSSRWGridModelIterator::getCommandList(DSSCellHandle &irHandle, vector<EnumManipulationCommandTypes> &orvCommand)
//{
//	CHECK_PTR(mpGridModel);
//	
//	int hr = S_OK;
//    
//	bool lbLinked = false;
//	lbLinked = static_cast<DSSRWTemplateNode*> (getRWNode())->getTemplateOrigin() == DssRWTemplateOriginReportView;
//    
//	bool lbOnline = true;	//TODO: offline mode?
//    
//	switch (irHandle.type) {
//		case DssCellHandleTemplateUnit:
//		case DssCellHandleTemplateUnitAndForm:
//		case DssCellHandleCustomGroupAndDepth:
//		{
//			//1. add change selection command if it has control, need check subtotal option.
//			DSSRWControl *lpControl = hGetControl(irHandle.p.unit);
//			if (lpControl)
//			{
//				if (lbOnline)
//					orvCommand.push_back(ManipulationSetSelectionFromTemplate);
//				else
//				{
//					//TODO: UC
//					//if (!lControl.hasUC())
//					orvCommand.push_back(ManipulationSetSelectionFromTemplate);
//					//else
//					//	mbUCWarning = true;
//				}
//			}
//            
//			//2. Add drill command if the unit can be drilled
//			if (lbOnline)
//			{
//				// For online mode, we support drill for both local or link template (drill out)
//				DSSTemplateUnit *lpTemplateUnit = getTemplateUnit(irHandle, true);
//                
//				// If the unit has drill path, it can be drilled
//				if (lpTemplateUnit && lpTemplateUnit->getDrillPaths() && lpTemplateUnit->getDrillPaths()->Count() > 0)
//					orvCommand.push_back(ManipulationDrill);
//			}
//            
//			//3. Add sort command if the unit is not a metrics or customgroup
//			if (!lbOnline || !lbLinked)
//			{
//				DSSTemplateUnit *lpTempalteUnit = irHandle.p.unit;
//				if (lpTempalteUnit)
//				{
//					EnumDSSTemplateUnitType lUnitType = lpTempalteUnit->getType();
//					if (lUnitType != DssTemplateMetrics
//						&& lUnitType != DssTemplateCustomGroup
//						&& lUnitType != DssTemplateConsolidation
//						//TODO: && (lInstance.hasNamedPrivilege(EnumDSSPrivilegeType.DssPrivilegesWebSort) || lInstance.hasNamedPrivilege(EnumDSSPrivilegeType.DssPrivilegesUseOfficeIntelligence))
//						)
//						orvCommand.push_back(ManipulationSortTemplate);
//				}
//				//TODO: pivot
//				//if ( (_dataView == null) || ( (_dataView as IXTabView).hasSubtotal == false) )
//				//	if (lInstance.hasNamedPrivilege(EnumDSSPrivilegeType.DssPrivilegesWebPivotReport) || lInstance.hasNamedPrivilege(EnumDSSPrivilegeType.DssPrivilegesUseOfficeIntelligence))
//				//		orvCommand.push_back(ManipulationPivotUnit);
//			}
//		}
//			break;
//            
//		case DssCellHandleXTabHeaderElement:
//		{
//			DSSTabularUnit *lpUnit = irHandle.p.xtabUnit;
//			CHECK_PTR(lpUnit);
//			//int lOrdinal = irHandle.v.ordinal;
//			int lAxis = mvUnitMapToAxis[irHandle.p.xtabUnit->getUnitIndex()];
//			_ASSERT(lAxis == 0 || lAxis == 1);
//            
//			//1. add Move DSSMetric command if it's a metric unit
//			/*if ((!lbOnline || !lbLinked) && lpUnit->getUnitType() == DssTemplateMetrics)
//			 {
//			 //TODO: if (lInstance.hasNamedPrivilege(EnumDSSPrivilegeType.DssPrivilegesWebPivotReport) || lInstance.hasNamedPrivilege(EnumDSSPrivilegeType.DssPrivilegesUseOfficeIntelligence))
//			 orvCommand.push_back(ManipulationMoveMetric);
//			 }*/
//            
//			//2. add change selection command if it has control, need check subtotal option.
//			bool lbHasInvalidTotalSelection = false;
//			DSSRWControl *lpControl = hGetControl(irHandle, lpUnit, lbHasInvalidTotalSelection);
//			if (lpControl)
//				orvCommand.push_back(ManipulationSetSelectionFromTemplate);
//            
//			//3. Add drill command if the unit can be drilled
//			if (lbOnline)
//			{
//				// We do not support drill on subtotal elements
//				int lKey = 0;
//				hr = getXTabElementKey(irHandle, irHandle.p.xtabUnit, lKey);
//				if (hr != S_OK)
//					return hr;
//                
//				// Subtotal element has nagative key
//				if (lKey >= 0)
//				{
//					// We do not allow drill on Derived Element nor Derived Element along the path
//					if (! HasDerivedElementAlongPath(irHandle) )
//					{
//						// For online mode, we support drill for both local or link template (drill out)
//						DSSTemplateUnit *lpTemplateUnit = getTemplateUnit(irHandle, true);
//                        
//						// If the unit has drill path, it can be drilled
//						if (lpTemplateUnit && lpTemplateUnit->getDrillPaths() && lpTemplateUnit->getDrillPaths()->Count() > 0)
//							orvCommand.push_back(ManipulationDrill);
//					}
//				}
//			}
//            
//			//4. Add sort command if this is the last element on column axis and there's metric
//			if (!lbOnline || !lbLinked)
//			{
//				DSSXTabHeader *lpHeader = mpXTabView->getHeader(lAxis);
//				if (lAxis == 1 && lpHeader != NULL && lpHeader->HasMetric() && lpUnit == mvColXTabUnits[mvColXTabUnits.size() - 1])
//					//&& (lInstance.hasNamedPrivilege(EnumDSSPrivilegeType.DssPrivilegesWebSort) || lInstance.hasNamedPrivilege(EnumDSSPrivilegeType.DssPrivilegesUseOfficeIntelligence)))
//				{
//					orvCommand.push_back(ManipulationSortTemplate);
//				}
//			}//end of if (!lbOnline || !lbLinked)
//            
//			//5. Add sort command if this is an attribute/consolidation/total element on row axis
//			/*if (lAxis == 0)
//			 {
//			 EnumDSSTemplateUnitType lUnitType = lpUnit->getUnitType();
//			 if (lUnitType != DssTemplateMetrics && lUnitType != DssTemplateCustomGroup)
//			 orvCommand.push_back(ManipulationSortTemplate);
//			 }*/
//            
//		}//end of case DssCellHandleXTabHeaderElement:
//			break;
//            
//		case DssCellHandleXTabData:
//		{
//			//TODO: 266608; enable the commend. Only disable in grid. GUI will take care of that
//            
//			int lnUnits = mpGridModel->Count();
//            
//			bool lbHasControl = false;
//			bool lbHasInvalidTotalSelection = false;
//			int i = 0;
//			for (i = 0; i < lnUnits; i++)
//			{
//				DSSTabularUnit *lpUnit = mpGridModel->Item(i);
//				_ASSERT(lpUnit);
//                
//				if (!lpUnit->IsRuntimeUnit())
//				{
//					DSSRWControl *lpControl = hGetControl(irHandle, lpUnit, lbHasInvalidTotalSelection);
//					if (lpControl && !lbHasControl)
//						lbHasControl = true;
//					if (lbHasInvalidTotalSelection)
//						break;
//				}
//			}
//            
//			//if the cell is at a subtoal level, and that unit has disabled Total selection, don't add command
//			if (lbHasControl && !lbHasInvalidTotalSelection)
//				orvCommand.push_back(ManipulationSetSelectionFromTemplate);
//            
//			//2. Add drill command if the unit can be drilled
//			if (lbOnline)
//			{
//				for (int i = 0; i < lnUnits; i++)
//				{
//					if (mvpTemplateUnits[i] && mvpTemplateUnits[i]->getDrillPaths() && mvpTemplateUnits[i]->getDrillPaths()->Count() > 0)
//					{
//						orvCommand.push_back(ManipulationDrill);
//						break;
//					}
//				}
//			}
//		}
//			break;
//            
//		default:
//			break;
//	}
//	// Determine whether we need to support QuickSwitch feature on the template node
//	// base on the boolean propert FormattingCrosstab -> Prefetch stored on the format
//	// object of the template node
//	/*	var lNodeFormat:IDSSFormat = _rwNode.getFormat(EnumDSSRWNodeFormatType.DssRWNodeFormatNode);
//	 if (lNodeFormat != null)
//	 {
//	 var lValue:String = lNodeFormat.getPropertyValue("FormattingCrosstab", "Prefetch" );
//	 if (lValue != null && Boolean(lValue))
//	 // ytang, 2009-10-23, tqms349706
//	 // Spec: "If the designer has chosen Quick Switch but the viewer does not have WebChangeViewMode available,
//	 // the enabler button will not be displayed for the user."
//	 if (lInstance.hasNamedPrivilege(EnumDSSPrivilegeType.DssPrivilegesWebChangeViewMode))
//	 {
//	 oCommandList.push(EnumManipulationCommandTypes.ManipulationSetTemplateDisplayMode);
//	 }
//	 }
//	 */
//	return hr;
//}
//
//DSSRWControl* DSSRWGridModelIterator::hGetControl(DSSCellHandle &irHandle, DSSTabularUnit *ipUnit, bool &orbInvalidTotalSelection)
//{
//	orbInvalidTotalSelection = false;
//    
//	if (DssCellHandleXTabHeaderElement != irHandle.type && DssCellHandleXTabData != irHandle.type)
//		return NULL;
//    
//	DSSRWControls *lpControls = static_cast<DSSRWTemplateNode*>(getRWNode())->getControls();
//	if (!lpControls)
//		return NULL;
//    
//	EnumDSSTemplateUnitType lUnitType = ipUnit->getUnitType();
//	GUID lUnitID;
//	ipUnit->getGUID(lUnitID);
//    
//	int lnControls = lpControls->Count();
//	for (int i = 0; i < lnControls; i++)
//	{
//		DSSRWControl *lpControl = lpControls->Item(i);
//		_ASSERT(lpControl);
//        
//		DSSObjectInfo *lControlObject = lpControl->getSourceObject();
//		if ((lUnitType == DssTemplateMetrics && lpControl->getType() == DssRWControlTypeMetricList)
//			|| (lControlObject && MBase::IsEqualGUID(lControlObject->getID(), lUnitID)))	//unit match
//		{
//			if (lpControl->getCountTargets() <= 0)
//				return NULL;
//			else
//			{
//				int lKey = 0;
//				int hr = getXTabElementKey(irHandle, ipUnit, lKey);
//				if (hr != S_OK)		return NULL;
//                
//				if (lKey >= 0)
//					return lpControl;
//                
//				orbInvalidTotalSelection = true;		//set to true until the subtotal is validated.
//                
//				int lnSubtotals = 0;
//				DSSDocumentSubtotalRefs * lpSubtotalsOnControl = lpControl->getSubtotals();
//				if (lpSubtotalsOnControl)
//					lnSubtotals = lpSubtotalsOnControl->Count();
//                
//				if (lnSubtotals > 0)
//				{
//					std::string lGlobalKey;
//					{
//						int lGlobalIndex = 0;
//						GUID lTemplateID = static_cast<DSSRWTemplateNode*> (getRWNode())->getTemplate()->getID();
//						hr = mpObjectContext->getDocumentLevelSubtotalIndex(-lKey, &lTemplateID, &lGlobalIndex);
//						if (hr != S_OK || lGlobalIndex == -1)		return NULL;
//                        
//						DSSDocumentSubtotals * lpDocumentSubtotals = mpObjectContext->getDocumentSubtotals();
//						if (!lpDocumentSubtotals)	return NULL;
//                        
//						DSSDocumentSubtotal *lpGlobalTotal = lpDocumentSubtotals->Item(lGlobalIndex - 1);
//						if (!lpGlobalTotal)		return NULL;
//                        
//						lGlobalKey = lpGlobalTotal->getKey();
//					}
//                    
//					for (int j = 0; j < lnSubtotals; j++)
//					{
//						DSSDocumentSubtotal *lpTotal = lpSubtotalsOnControl->Item(j);
//						if (!lpTotal)		return NULL;
//                        
//						if (lpTotal->getKey() == lGlobalKey)
//						{
//							orbInvalidTotalSelection = false;
//							return lpControl;
//						}
//					}
//				}
//				return NULL;
//			}//end of if (lnSubtotals > 0)
//		}
//	}
//    
//	return NULL;
//}
//
//DSSRWControl* DSSRWGridModelIterator::hGetControl(DSSTemplateUnit *ipTemplateUnit)
//{
//	DSSRWControls *lpControls = static_cast<DSSRWTemplateNode*>(getRWNode())->getControls();
//	if (!lpControls)
//		return NULL;
//    
//	GUID lUnitID = ipTemplateUnit->getID();
//	EnumDSSTemplateUnitType lUnitType = ipTemplateUnit->getType();
//    
//	int lnControls = lpControls->Count();
//	for (int i = 0; i < lnControls; i++)
//	{
//		DSSRWControl *lpControl = lpControls->Item(i);
//		_ASSERT(lpControl);
//        
//		DSSObjectInfo *lControlObject = lpControl->getSourceObject();
//		if ((lUnitType == DssTemplateMetrics && lpControl->getType() == DssRWControlTypeMetricList)
//			|| (lControlObject && MBase::IsEqualGUID(lControlObject->getID(), lUnitID)))	//unit match
//		{
//            
//			if (lpControl->getShowAll() && lpControl->getCountTargets() > 0)
//				return lpControl;
//			else
//				return NULL;
//		}
//	}
//    
//	return NULL;
//}
//
//DSSBaseElementProxy * DSSRWGridModelIterator::createElementFromHandle(DSSCellHandle &irHandle)
//{
//	if (irHandle.type == DssCellHandleXTabHeaderElement)
//	{
//		DSSTabularUnit *lpUnit = irHandle.p.xtabUnit;
//        
//		int lKey = 0;
//		int hr = getXTabElementKey(irHandle, lpUnit, lKey);
//		if (hr != S_OK)
//			return NULL;
//        
//		DSSBaseElementProxy *lpElement = NULL;
//		lpUnit->createElement(lKey, mpObjectContext, &lpElement);
//		return lpElement;
//	}
//	return NULL;
//}
//
//int DSSRWGridModelIterator::hFillIndex(int cellID, int iAxisIndex, vector<int> &orKeys)
//{
//	int hr = S_OK;
//	int lnCols = mpXTabView->getHeader(1)->getRowCount();
//	int row = lnCols != 0 ? cellID / lnCols : cellID;
//	int col = lnCols != 0 ? cellID % lnCols : 0;
//	
//	int lnUnits = mpGridModel->Count();
//	if (lnUnits == 0)	//no unit, no elements
//		return S_OK;
//	
//	AE_ASSERT(orKeys.size() >= lnUnits);
//	
//	int i = 0;
//	for (i = 0; i < lnUnits; i++)
//		orKeys[i] = DSSXTAB_TOTAL_KEY;
//	
//	if (iAxisIndex == DssAxisAll)
//	{
//		int lMetricKey = 0;
//		hr = mpXTabView->getHeader(0)->FillIndex(row, lnUnits, &orKeys[0], &lMetricKey);
//		CHECK_HR;
//		hr = mpXTabView->getHeader(1)->FillIndex(col, lnUnits, &orKeys[0], &lMetricKey);
//		CHECK_HR;
//		int lMetricUnitID = mpGridModel->getMetricUnitID();
//		if (lMetricUnitID >= 0 && lMetricUnitID < lnUnits)
//			orKeys[lMetricUnitID] = lMetricKey;
//	}
//	else if (iAxisIndex == DssAxisRows || iAxisIndex == DssAxisColumns)
//	{
//		DSSXTabHeader *lpXTabHeader = mpXTabView->getHeader(iAxisIndex-1);
//		CHECK_PTR(lpXTabHeader);
//		
//		int lMetricKey = 0;
//		hr = lpXTabHeader->FillIndex((iAxisIndex == DssAxisRows) ? row : col, lnUnits, &orKeys[0], &lMetricKey);
//		CHECK_HR;
//		
//		int lMetricUnitID = mpGridModel->getMetricUnitID();
//		if (lMetricUnitID >= 0 && lMetricUnitID < lnUnits)
//			orKeys[lMetricUnitID] = lMetricKey;
//	}
//	return S_OK;	
//}
//
//int DSSRWGridModelIterator::createElementsFromHandle(DSSCellHandle &irHandle, int iAxisIndex, vector<DSSBaseElementProxy *> &orvpElements)
//{
//	if (irHandle.type == DssCellHandleXTabData)
//	{
//		int hr = S_OK;
//		int lnUnits = mpGridModel->Count();
//		if (lnUnits == 0)	//no unit, no elements
//			return S_OK;
//		
//		vector<int> lvKeys(lnUnits);
//		hr = hFillIndex(irHandle.v.cell, iAxisIndex, lvKeys);
//		CHECK_HR;
//		
//		
//		int i = 0;
//		if (iAxisIndex == DssAxisAll)
//		{
//			for (i = 0; i < lnUnits; i++)
//			{
//				DSSTabularUnit *lpUnit = mpGridModel->Item(i);
//				if (!lpUnit->IsRuntimeUnit())
//				{
//					int lKey = lvKeys[i];
//					DSSBaseElementProxy *lpElement = NULL;
//					lpUnit->createElement(lKey, mpObjectContext, &lpElement);
//					if (lpElement)
//						orvpElements.push_back(lpElement);
//				}
//			}
//		}
//		else if (iAxisIndex == DssAxisRows || iAxisIndex == DssAxisColumns)
//		{
//			DSSXTabHeader *lpXTabHeader = mpXTabView->getHeader(iAxisIndex-1);
//			CHECK_PTR(lpXTabHeader);
//            
//			int lnHeaderUnits = lpXTabHeader->getNumUnits();
//			for (i = 0; i < lnHeaderUnits; i++)
//			{
//				int lUnitIndex = lpXTabHeader->getUnit(i);
//                
//				DSSTabularUnit *lpUnit = mpGridModel->Item(lUnitIndex);
//				if (!lpUnit->IsRuntimeUnit())
//				{
//					int lKey = lvKeys[lUnitIndex];
//					DSSBaseElementProxy *lpElement = NULL;
//					lpUnit->createElement(lKey, mpObjectContext, &lpElement);
//					if (lpElement)
//						orvpElements.push_back(lpElement);
//				}
//			}
//		}
//	}
//	return NULL;
//}
//
//int DSSRWGridModelIterator::createSiblingElementsFromHandle(DSSCellHandle &irHandle, bool iParentElementsOnly, vector<DSSBaseElementProxy *> &orvpElements)
//{
//	if (irHandle.type == DssCellHandleXTabHeaderElement)
//	{
//		DSSTabularUnit *lpUnit = irHandle.p.xtabUnit;
//		int lOrdinal = irHandle.v.ordinal;
//        
//		int hr = S_OK;
//        
//		int lnUnits = mpGridModel->Count();
//		if (lnUnits == 0)	//no unit, no elements
//			return S_OK;
//        
//		vector<int> lvKeys(lnUnits);
//		int i = 0;
//		for (i = 0; i < lnUnits; i++)
//			lvKeys[i] = DSSXTAB_TOTAL_KEY;
//        
//		int lUnitID = lpUnit->getUnitIndex();
//		int lAxis = mvUnitMapToAxis[lUnitID];
//		_ASSERT(lAxis != -1);
//        
//		DSSXTabHeader *lpXTabHeader = mpXTabView->getHeader(lAxis);
//		CHECK_PTR(lpXTabHeader);
//        
//		int lRowOffset = lAxis == 0 ? mvRowWindow[lOrdinal] : mvColumnWindow[lOrdinal];
//		int lMetricKey = 0;
//		hr = lpXTabHeader->FillIndex(lRowOffset, lnUnits, &lvKeys[0], &lMetricKey);
//		CHECK_HR;
//        
//		int lMetricUnitID = mpGridModel->getMetricUnitID();
//		if (lMetricUnitID >= 0 && lMetricUnitID < lnUnits)
//			lvKeys[lMetricUnitID] = lMetricKey;
//        
//		int lnHeaderUnits = lpXTabHeader->getNumUnits();
//		for (i = 0; i < lnHeaderUnits; i++)
//		{
//			int lUnitIndex = lpXTabHeader->getUnit(i);
//            
//			DSSTabularUnit *lpUnit = mpGridModel->Item(lUnitIndex);
//			if (!lpUnit->IsRuntimeUnit())
//			{
//				//TQMS 430986, add parent element for hierarchical CG
//				DSSTemplateUnit *lpTemplateUnit = mvpTemplateUnits[lUnitIndex];
//				bool lbHierarchicalDisplay = (lpTemplateUnit->getType() == DssTemplateCustomGroup
//											  && !static_cast<DSSTemplateCustomGroup*> (lpTemplateUnit)->getFlatten());
//				if (lbHierarchicalDisplay)
//				{
//					DSSTabularCustomGroup *lpCGUnit = static_cast<DSSTabularCustomGroup*> (lpUnit);
//					int lKey = lvKeys[lUnitIndex];
//					vector<int> lvParentKeys;
//                    
//					while (true)
//					{
//						XTabCustomGroupElement *lpXTabCGElement = lpCGUnit->getElement(lKey);
//						if (lpXTabCGElement && (lKey = lpXTabCGElement->parent) > 0)
//						{
//							lvParentKeys.push_back(lKey);
//						}
//						else
//							break;
//					}
//                    
//					int lnParents = lvParentKeys.size();
//					for (int j = lnParents - 1; j >= 0; j--)
//					{
//						DSSBaseElementProxy *lpElement = NULL;
//						lpUnit->createElement(lvParentKeys[j], mpObjectContext, &lpElement);
//						if (lpElement)
//							orvpElements.push_back(lpElement);
//					}
//				}
//			}
//            
//			if (iParentElementsOnly == true)
//			{
//				// If caller only asks for parent elements, break when reaching this element
//				if (lUnitIndex == lUnitID)
//					break;
//			}
//            
//			if (!lpUnit->IsRuntimeUnit())
//			{
//				int lKey = lvKeys[lUnitIndex];
//				DSSBaseElementProxy *lpElement = NULL;
//				lpUnit->createElement(lKey, mpObjectContext, &lpElement);
//				if (lpElement)
//					orvpElements.push_back(lpElement);
//			}
//		}
//	}
//	return NULL;
//}
//
//EnumDSSAxisName DSSRWGridModelIterator::getAxisFromHandle(DSSCellHandle &irHandle)
//{
//	if (irHandle.type == DssCellHandleXTabHeaderElement)
//	{
//		int lAxis = mvUnitMapToAxis[irHandle.p.xtabUnit->getUnitIndex()];
//		if (lAxis == 0)
//			return DssAxisRows;
//		else if (lAxis == 1)
//			return DssAxisColumns;
//	}
//	return DssAxisReserved;
//}
//
//
//bool DSSRWGridModelIterator::IsSubtotalAt(int iAxis, int iOrdinal, int iDepth)
//{
//	//TODO: validate iOrdinal and iDepth
//	if (iAxis == 0)
//	{
//		int offset = iDepth * mHeader[0].mnRows + iOrdinal;
//		AE_ASSERT(offset < mvRowKeyTable.size());
//		return mvRowKeyTable[offset] < 0;
//	}
//	else if (iAxis == 1)
//	{
//		int offset = iDepth * mHeader[1].mnRows + iOrdinal;
//		AE_ASSERT(offset < mvColKeyTable.size());
//		return mvColKeyTable[offset] < 0;
//	}
//	else
//		return false;
//}
//
//EnumDSSMetricType DSSRWGridModelIterator::getSubtotalTypeAt(int iAxis, int iOrdinal)
//{
//	//TODO: validate iOrdinal
//	if (iAxis == 0)
//	{
//		for (int iDepth = mHeader[0].mWidth - 1; iDepth >= 0; iDepth--)
//		{
//			int offset = iDepth * mHeader[0].mnRows + iOrdinal;
//			_ASSERT(offset < mvRowKeyTable.size());
//            
//			int key = mvRowKeyTable[offset];
//			if (key < 0)
//			{
//				return mpGridModel->getSubtotalType(-key - 1);
//			}
//		}
//	}
//	else if (iAxis == 1)
//	{
//		for (int iDepth = mHeader[1].mWidth - 1; iDepth >= 0; iDepth--)
//		{
//			int offset = iDepth * mHeader[1].mnRows + iOrdinal;
//			_ASSERT(offset < mvColKeyTable.size());
//			int key = mvColKeyTable[offset];
//			if (key < 0)
//			{
//				return mpGridModel->getSubtotalType(-key - 1);
//			}
//		}
//	}
//	return DssMetricReserved;
//}
//
//bool DSSRWGridModelIterator::IsSubtotalAtOrdinal(int iAxis, int iOrdinal)
//{
//	return getSubtotalTypeAt(iAxis, iOrdinal) != DssMetricReserved;
//}
//
//int DSSRWGridModelIterator::getElementWidth(int iAxis, int iDepth)
//{
//	int lnWidths = mHeader[iAxis].mWidth;
//	if (iAxis == 0)
//	{
//		if (mvRowXTabUnits[iDepth]->getUnitType() != DssTemplateCustomGroup)
//			return 1;
//        
//		int cursor = mvRowUnitCursors[iDepth];
//		int key = mvRowEleKeys[iDepth][cursor];
//        
//		int i = 0;
//		for (i = iDepth; i < lnWidths; i++)
//		{
//			cursor = mvRowUnitCursors[i];
//			if (mvRowXTabUnits[i] != mvRowXTabUnits[iDepth] || mvRowEleKeys[i][cursor] != key)
//				break;
//		}
//		return i - iDepth;
//        
//	}
//	else if (iAxis == 1)
//	{
//		if (mvColXTabUnits[iDepth]->getUnitType() != DssTemplateCustomGroup)
//			return 1;
//        
//		int cursor = mvColUnitCursors[iDepth];
//		int key = mvColEleKeys[iDepth][cursor];
//        
//		int i = 0;
//		for (i = iDepth; i < lnWidths; i++)
//		{
//			cursor = mvColUnitCursors[i];
//			if (mvColXTabUnits[i] != mvColXTabUnits[iDepth] || mvColEleKeys[i][cursor] != key)
//				break;
//		}
//		return i - iDepth;
//	}
//	return 0;
//}
//
//int DSSRWGridModelIterator::getElementValueAt(int iAxis, int iOrdinal, int iDepth, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag)
//{
//	if (!oppData || !opDataSize || !opDataFlag || !opDataType)
//		return E_INVALIDARG;
//    
//	//TODO[opt]: cache the unit's column info
//	DSSTabularUnit *lpUnit = NULL;
//	if (iAxis == 0)
//		lpUnit = mvRowXTabUnits[iDepth];
//	else
//		lpUnit = mvColXTabUnits[iDepth];
//	CHECK_PTR(lpUnit);
//    
//	int lKey = 0;
//	bool lbSkip = false;
//    
//	if (iAxis == 0)
//	{
//		lKey = mvRowKeyTable[iDepth * mHeader[0].mnRows + iOrdinal];
//        
//		if (lpUnit->getUnitType() == DssTemplateCustomGroup && iDepth > 0)
//		{
//			if (mvRowXTabUnits[iDepth - 1] == lpUnit)
//			{
//				//lwang, 415238, skip the repeat element in hierarchical display
//				int lParentKey =  mvRowKeyTable[(iDepth - 1) * mHeader[0].mnRows + iOrdinal];;
//				lbSkip = (lParentKey == lKey);
//			}
//		}
//	}
//	else if (iAxis == 1)
//	{
//		lKey = mvColKeyTable[iDepth * mHeader[1].mnRows + iOrdinal];
//        
//		if (lpUnit->getUnitType() == DssTemplateCustomGroup && iDepth > 0)
//		{
//			if (mvColXTabUnits[iDepth - 1] == lpUnit)
//			{
//				//lwang, 415238, skip the repeat element in hierarchical display
//				int lParentKey = mvColKeyTable[(iDepth - 1) * mHeader[1].mnRows + iOrdinal];
//				lbSkip = (lParentKey == lKey);
//			}
//		}
//	}
//	else
//		return E_INVALIDINDEX;
//    
//	if (lbSkip)
//	{
//		*oppData = NULL;
//		*opDataSize = 0;
//		*opDataType = DssDataTypeVarChar;
//		*opDataFlag = DssDataOk;
//		return S_OK;
//	}
//    
//	return hGetUnitValue(lpUnit, lKey, iBaseForm, oppData, opDataSize, opDataType, opDataFlag);
//}
//
//int DSSRWGridModelIterator::getElementValueAt(int iAxis, int iOrdinal, int iDepth, int iBaseForm, std::string &orText, DSSDataFlag *opDataFlag)
//{
//	const void *lpData;
//	int lSize = 0;
//	EnumDSSDataType lDataType = DssDataTypeReserved;
//	DSSDataFlag lFlag = DssDataOk;
//	int hr = getElementValueAt(iAxis, iOrdinal, iDepth, iBaseForm, &lpData, &lSize, &lDataType, &lFlag);
//	CHECK_HR;
//    
//	if (lFlag == S_OK)
//		return DSSAEHelper::ConvertToString(lpData, lSize, lDataType, lFlag, orText);
//	else
//		return mpGridModel->getElementNullDisplay(lFlag, orText);
//}
//
//int DSSRWGridModelIterator::getElementSpanAt(int iAxis, int iOrdinal, int iDepth)
//{
//	return E_NOTIMPL;
//	//TODO: translate iOrdinal to offset in mvRowEleOrdinals[iDepth];
//	if (iAxis == 0)
//	{
//		return mvRowEleOrdinals[iDepth][iOrdinal + 1] - mvRowEleOrdinals[iDepth][iOrdinal];
//	}
//	else if (iAxis == 1)
//	{
//		return mvColEleOrdinals[iDepth][iOrdinal + 1] - mvColEleOrdinals[iDepth][iOrdinal];
//	}
//	return 0;
//}
//
//int DSSRWGridModelIterator::getCellValueAt(int iRowOrdinal, int iColOrdinal, const void **oppData, int *opDataSize,
//                                      EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag, int *opThreshold, int *opMetric)
//{
//	if (!opDataSize || !opDataType || !opDataFlag)
//		return E_INVALIDARG;
//    
//	//TODO: validate iOrdinal
//	return mpXTabView->GetRawCellInfo(mvRowWindow[iRowOrdinal], mvColumnWindow[iColOrdinal], (const char**)oppData, *opDataSize, *opDataType, *opDataFlag, opThreshold, opMetric);
//}
//
//int DSSRWGridModelIterator::getCellValueAt(int iRowOrdinal, int iColOrdinal, double *opVal, DSSDataFlag *opDataFlag, int *opThreshold, int *opMetric)
//{
//	if (!opVal || !opDataFlag)
//		return E_INVALIDARG;
//    
//	//TODO: validate iOrdinal
//	return mpXTabView->GetCellValue(mvRowWindow[iRowOrdinal], mvColumnWindow[iColOrdinal], *opVal, *opDataFlag, opThreshold, opMetric);
//}
//
//DSSTemplateUnit * DSSRWGridModelIterator::getUnit(int iAxis, int iDepth)
//{
//	if (iAxis == 0 && iDepth >= 0 && iDepth < mvRowTemplateUnits.size())
//		return mvRowTemplateUnits[iDepth];
//	else if (iAxis == 1 && iDepth >= 0 && iDepth < mvColTemplateUnits.size())
//		return mvColTemplateUnits[iDepth];
//	return NULL;
//}
//
//int DSSRWGridModelIterator::getUnitID(int iAxis, int iDepth)
//{
//	return 0;
//}
//
//int DSSRWGridModelIterator::setGroupByUnitsForSliceWindow(int nRowUnits, int *ipRowUnits, int nColUnits, int *ipColUnits)
//{
//	if (nRowUnits < 0 || nColUnits < 0 || nRowUnits > mHeader[0].mnUnits || nColUnits > mHeader[1].mnUnits)
//		return E_FAIL;
//    
//	//validation: currenly only support the left or top units as groupby units
//	int i = 0;
//	for (i = 0; i < nRowUnits; i++)
//		if (ipRowUnits[i] >= nRowUnits)
//			return E_NOTIMPL;
//    
//	for (i = 0; i < nColUnits; i++)
//		if (ipColUnits[i] >= nColUnits)
//			return E_NOTIMPL;
//    
//	vector<int> lvRowGBUnits;
//	lvRowGBUnits.assign(ipRowUnits, ipRowUnits + nRowUnits);
//	vector<int> lvColGBUnits;
//	lvColGBUnits.assign(ipColUnits, ipColUnits + nColUnits);
//    
//	sort(lvRowGBUnits.begin(), lvRowGBUnits.end());
//	sort(lvColGBUnits.begin(), lvColGBUnits.end());
//    
//	for (i = 1; i < nRowUnits; i++)
//		if (lvRowGBUnits[i] == lvRowGBUnits[i - 1])
//			return E_INVALIDARG;			//has duplicate units
//    
//	for (i = 1; i < nColUnits; i++)
//		if (lvColGBUnits[i] == lvColGBUnits[i - 1])
//			return E_INVALIDARG;			//has duplicate units
//    
//    //	if (nRowUnits + 1 != mHeader[0].mnUnits)	//TODO: support less groupby units case
//    //		return E_INVALIDARG;
//    
//    //	if (nColUnits + 1 != mHeader[1].mnUnits)
//    //		return E_INVALIDARG;
//    
//	mnRowGBUnits = nRowUnits;
//	mnColGBUnits = nColUnits;
//    
//	//create slice window
//	int lnRows = 0;
//	int lnCols = 0;
//	if (!mbPivotGB)
//	{
//		int lnRowDepth = hGetUnitStartDepth(0, mnRowGBUnits) - 1;
//		if (lnRowDepth < 0)
//		{
//			lnRows = 1;
//		}
//		else
//		{
//			lnRows = mvRowEleOrdinals[lnRowDepth].size();
//		}
//        
//		int lnColDepth = hGetUnitStartDepth(1, mnColGBUnits) - 1;
//		if (lnColDepth < 0)
//			lnCols = 1;
//		else
//			lnCols = mvColEleOrdinals[lnColDepth].size();
//	}
//	else
//	{
//		mnColGBUnits = 1;
//        
//		int lnRowDepth = hGetUnitStartDepth(0, mnRowGBUnits - 1) - 1;
//		_ASSERT (lnRowDepth >= 0);	//there should be at least two units on row
//        
//		lnRows = mvRowEleOrdinals[lnRowDepth].size();
//        
//		vector<int> lvDistinctEle;
//		hSelectDistinct(mvRowEleKeys[lnRowDepth + 1], lvDistinctEle);
//        
//		lnCols = lvDistinctEle.size();
//        
//		//TODO: fill the holes
//	}
//    
//	mvRowSliceWindow.resize(lnRows);
//	for (i = 0; i < lnRows; i++)
//		mvRowSliceWindow[i] = i;
//    
//	mvColSliceWindow.resize(lnCols);
//	for (i = 0; i < lnCols; i++)
//		mvColSliceWindow[i] = i;
//    
//	mRowSliceWindowSize = mvRowSliceWindow.size() - 1;		//mvRowEleOrdinals contains one extra row
//	mColSliceWindowSize = mvColSliceWindow.size() - 1;
//	if (mRowSliceWindowSize <= 0)
//		mRowSliceWindowSize = 1;
//	if (mColSliceWindowSize <= 0)
//		mColSliceWindowSize = 1;
//    
//	return S_OK;
//}
//
//int DSSRWGridModelIterator::setAndPivotGroupByUnitsForSliceWindow(int iAxis, int nUnits, int *ipUnits, int nPivotUnits, int *ipPivotUnits)
//{
//	if (nUnits < 2 || nPivotUnits != 1 || !ipPivotUnits)
//		return E_NOTIMPL;
//    
//	int hr = S_OK;
//	mbPivotGB = true;
//	if (iAxis == 0)
//	{
//		hr = setGroupByUnitsForSliceWindow(nUnits, ipUnits, 0, NULL);
//		CHECK_HR;
//		_ASSERT(mnRowGBUnits - 1 == *ipPivotUnits);
//	}
//	else
//	{
//		hr = setGroupByUnitsForSliceWindow(0, NULL, nUnits, ipUnits);
//		CHECK_HR;
//		_ASSERT((mnColGBUnits - 1) == *ipPivotUnits);
//	}
//	return S_OK;
//}
//
//DSSRWGridModelIterator * DSSRWGridModelIterator::getIteratorForGroupBy()
//{
//	if (!mpGroupbyGridIterator)
//	{
//		mpGroupbyGridIterator = new DSSRWGridModelIterator();
//		int hr = mpGroupbyGridIterator->hPopulate(this, false);
//		if (hr != S_OK)
//		{
//			delete mpGroupbyGridIterator;
//			mpGroupbyGridIterator = NULL;
//		}
//	}
//	if (!mpGroupbyGridIterator)
//	{	//caller doesn't have error handling, and usually result in various un-expectedd problem. 
//		//to catch the problem sooner and ease debugging, assert here
//		_ASSERT(false);
//	}
//	return mpGroupbyGridIterator;
//}
//
//int DSSRWGridModelIterator::setSliceWindowID(int iSlice)
//{
//	if (mColSliceWindowSize == 0)
//		return E_FAIL;
//    
//	mRowSliceWindowID = iSlice / mColSliceWindowSize;
//	mColSliceWindowID = iSlice % mColSliceWindowSize;
//	_ASSERT(mRowSliceWindowID < mvRowSliceWindow.size() && mColSliceWindowID < mvColSliceWindow.size());
//    
//	return S_OK;
//}
//
//DSSRWGridModelIterator * DSSRWGridModelIterator::getIteratorForSliceWindow()
//{
//	if (!mpSliceWindowGridIterator)
//	{
//		mpSliceWindowGridIterator = new DSSRWGridModelIterator();
//		int hr = mpSliceWindowGridIterator->hPopulate(this, true);
//		if (hr != S_OK)
//		{
//			delete mpSliceWindowGridIterator;
//			mpSliceWindowGridIterator = NULL;
//		}
//	}
//    
//	//TODO: the logic is not correct: check if this's a "hole"
//	//if (mvRowSliceWindow[mRowSliceWindowID + 1] == mvRowSliceWindow[mRowSliceWindowID]
//	//	|| mvColSliceWindow[mColSliceWindowID + 1] == mvColSliceWindow[mColSliceWindowID])
//	//	return NULL;
//    
//	DSSRWGridModelIterator *lpIterator = NULL;
//	if (mpSliceWindowGridIterator && mpSliceWindowGridIterator->hUpdateSliceWindow(this) == S_OK)
//		lpIterator = mpSliceWindowGridIterator;
//	
//	if (!lpIterator)
//	{	//caller doesn't have error handling and result in un-expectedd problem. 
//		//to catch the problem earlier and ease debugging, assert here
//		_ASSERT(false);
//	}
//	
//	return lpIterator;
//}
//
//int DSSRWGridModelIterator::hPopulate(DSSRWGridModelIterator *ipSourceIterator, bool ibPartial)
//{
//	mType = DssIteratorGrid;
//	mNodeKey = ipSourceIterator->mNodeKey;
//	mpObjectContext = ipSourceIterator->mpObjectContext;
//	mpGridModel = ipSourceIterator->mpGridModel;
//	mpXTabView = ipSourceIterator->mpXTabView;
//    
//	//TODO: check metrics
//	mpTemplateMetrics = ipSourceIterator->mpTemplateMetrics;
//	mbHasMetricOnRow = ipSourceIterator->mbHasMetricOnRow;
//	mbNodeHasGBUnits = ipSourceIterator->mbNodeHasGBUnits;
//    
//	//1. copy mpUnitMapToAxis
//	mvUnitMapToAxis.assign(ipSourceIterator->mvUnitMapToAxis.begin(), ipSourceIterator->mvUnitMapToAxis.end());
//	mvpTemplateUnits.assign(ipSourceIterator->mvpTemplateUnits.begin(), ipSourceIterator->mvpTemplateUnits.end());
//	//TODO: in pivot case, still map to the original axis?
//    
//	//2. get mvRowEleOrdinals, mvColEleOrdinals, mvRowElemKeys, mvColEleKeys
//	mnRowGBUnits = ipSourceIterator->mnRowGBUnits;
//	mnColGBUnits = ipSourceIterator->mnColGBUnits;
//	mbPivotGB = ipSourceIterator->mbPivotGB;
//    
//	mpParentGridIterator = ipSourceIterator;
//	
//	int lnRowGBUnits = mnRowGBUnits;
//	int lnColGBUnits = mnColGBUnits;
//	if (mbPivotGB)
//	{
//		lnRowGBUnits --;
//	}
//    
//	int lnRowWidth = ipSourceIterator->hGetUnitStartDepth(0, lnRowGBUnits);	//map unit to depth
//	int lnColWidth = ipSourceIterator->hGetUnitStartDepth(1, lnColGBUnits);	//map unit to depth
//    
//	//3. get header width and create cursor
//	if (ibPartial)
//	{
//		//for slice window grid iterator, the other information are dynamic
//		mHeader[0].mnUnits = ipSourceIterator->mHeader[0].mnUnits - mnRowGBUnits;
//		mHeader[0].mvUnitWidth.assign(ipSourceIterator->mHeader[0].mvUnitWidth.begin() + mnRowGBUnits, ipSourceIterator->mHeader[0].mvUnitWidth.end());
//		mHeader[0].mvDepthToUnit.assign(ipSourceIterator->mHeader[0].mvDepthToUnit.begin() + lnRowWidth, ipSourceIterator->mHeader[0].mvDepthToUnit.end());
//        
//		mHeader[1].mnUnits = ipSourceIterator->mHeader[1].mnUnits - mnColGBUnits;
//		mHeader[1].mvUnitWidth.assign(ipSourceIterator->mHeader[1].mvUnitWidth.begin() + mnColGBUnits, ipSourceIterator->mHeader[1].mvUnitWidth.end());
//		mHeader[1].mvDepthToUnit.assign(ipSourceIterator->mHeader[1].mvDepthToUnit.begin() + lnColWidth, ipSourceIterator->mHeader[1].mvDepthToUnit.end());
//        
//		//mvRowXTabUnits, mvColXTabUnits
//		int lnRowWidthEnd = ipSourceIterator->mHeader[0].mWidth;	//hGetUnitStartDepth(0, lnRowGBUnits);
//		int lnColWidthEnd = ipSourceIterator->mHeader[1].mWidth;	//hGetUnitStartDepth(1, lnColGBUnits);
//		mvRowXTabUnits.assign(ipSourceIterator->mvRowXTabUnits.begin() + lnRowWidth,
//							  ipSourceIterator->mvRowXTabUnits.begin() + lnRowWidthEnd);
//		mvColXTabUnits.assign(ipSourceIterator->mvColXTabUnits.begin() + lnColWidth,
//							  ipSourceIterator->mvColXTabUnits.begin() + lnColWidthEnd);
//		mvRowTemplateUnits.assign(ipSourceIterator->mvRowTemplateUnits.begin() + lnRowWidth,
//								  ipSourceIterator->mvRowTemplateUnits.begin() + lnRowWidthEnd);
//		mvColTemplateUnits.assign(ipSourceIterator->mvColTemplateUnits.begin() + lnColWidth,
//								  ipSourceIterator->mvColTemplateUnits.begin() + lnColWidthEnd);
//        
//		return S_OK;
//	}
//	else
//	{
//		mHeader[0].mnUnits = lnRowGBUnits;
//		mHeader[0].mvUnitWidth.assign(ipSourceIterator->mHeader[0].mvUnitWidth.begin(), ipSourceIterator->mHeader[0].mvUnitWidth.begin() + lnRowGBUnits);
//		mHeader[0].mvDepthToUnit.assign(ipSourceIterator->mHeader[0].mvDepthToUnit.begin(), ipSourceIterator->mHeader[0].mvDepthToUnit.begin() + lnRowWidth);
//        
//		mHeader[1].mnUnits = lnColGBUnits;
//		mHeader[1].mvUnitWidth.assign(ipSourceIterator->mHeader[1].mvUnitWidth.begin(), ipSourceIterator->mHeader[1].mvUnitWidth.begin() + lnColGBUnits);
//		mHeader[1].mvDepthToUnit.assign(ipSourceIterator->mHeader[1].mvDepthToUnit.begin(), ipSourceIterator->mHeader[1].mvDepthToUnit.begin() + lnColWidth);
//	}
//    
//	if (!mpGridModel)
//		return S_OK;
//    
//	//4. mvRowXTabUnits, mvColXTabUnits
//	mvRowXTabUnits.assign(ipSourceIterator->mvRowXTabUnits.begin(), ipSourceIterator->mvRowXTabUnits.begin() + lnRowWidth);
//	mvColXTabUnits.assign(ipSourceIterator->mvColXTabUnits.begin(), ipSourceIterator->mvColXTabUnits.begin() + lnColWidth);
//	mvRowTemplateUnits.assign(ipSourceIterator->mvRowTemplateUnits.begin(), ipSourceIterator->mvRowTemplateUnits.begin() + lnRowWidth);
//	mvColTemplateUnits.assign(ipSourceIterator->mvColTemplateUnits.begin(), ipSourceIterator->mvColTemplateUnits.begin() + lnColWidth);
//    
//	//5. copy element keys and ordinal array
//	vector<vector<int> > lvRowOrdinal;
//	lvRowOrdinal.assign(ipSourceIterator->mvRowEleOrdinals.begin(), ipSourceIterator->mvRowEleOrdinals.begin() + lnRowWidth);
//	hAdjustOrdinal(lvRowOrdinal, mvRowEleOrdinals);
//	mvRowEleKeys.assign(ipSourceIterator->mvRowEleKeys.begin(), ipSourceIterator->mvRowEleKeys.begin() + lnRowWidth);
//    
//	//copy mvColEle***
//	if (!mbPivotGB)
//	{
//		vector<vector<int> > lvColOrdinal;
//		lvColOrdinal.assign(ipSourceIterator->mvColEleOrdinals.begin(), ipSourceIterator->mvColEleOrdinals.begin() + lnColWidth);
//		hAdjustOrdinal(lvColOrdinal, mvColEleOrdinals);
//		mvColEleKeys.assign(ipSourceIterator->mvColEleKeys.begin(), ipSourceIterator->mvColEleKeys.begin() + lnColWidth);
//	}
//	else
//	{
//		//TODO: select distinct from row and assign to mvCol;
//		lnColWidth = 1;
//		vector<vector<int> > lvColOrdinal(1);
//		hSelectDistinct(ipSourceIterator->mvRowEleOrdinals[lnRowWidth], lvColOrdinal[0]);
//		mvColEleOrdinals.resize(1);
//		mvColEleKeys.resize(1);
//		hSelectDistinct(ipSourceIterator->mvRowEleOrdinals[lnRowWidth], mvColEleKeys[0]);
//		int lnEles = mvColEleKeys[0].size();
//		for (int i = 0; i < lnEles; i++)
//			mvColEleOrdinals[0][i] = i;
//	}
//    
//	//6. lnRowWindowSize = 0;
//	int i = 0;
//	if (lnRowWidth > 0)
//	{
//		int *lpOrigRowWindow = &ipSourceIterator->mvRowWindow[0];
//		int lnRowWindowSize = ipSourceIterator->mvRowEleKeys[lnRowWidth - 1].size();
//		int *lpOrigRowOrdinal = &(ipSourceIterator->mvRowEleOrdinals[lnRowWidth - 1][0]);
//        
//		mvRowWindow.resize(lnRowWindowSize);
//		for (i = 0; i < lnRowWindowSize; i++)
//		{
//			mvRowWindow[i] = lpOrigRowWindow[lpOrigRowOrdinal[i]];
//		}
//	}
//	else if (lnRowWidth == 0 && !ipSourceIterator->mvRowEleOrdinals.empty())
//	{
//		mvRowWindow.push_back(ipSourceIterator->mvRowEleOrdinals[lnRowWidth][0]);
//	}
//    
//	if (!mbPivotGB)
//	{
//		if (lnColWidth > 0)
//		{
//			int *lpOrigColWindow = &ipSourceIterator->mvColumnWindow[0];
//			int lnColWindowSize = ipSourceIterator->mvColEleKeys[lnColWidth - 1].size();
//			int *lpOrigColOrdinal = &(ipSourceIterator->mvColEleOrdinals[lnColWidth - 1][0]);
//            
//			mvColumnWindow.resize(lnColWindowSize);
//			for (i = 0; i < lnColWindowSize; i++)
//			{
//				mvColumnWindow[i] = lpOrigColWindow[lpOrigColOrdinal[i]];
//			}
//		}
//		else if (lnColWidth == 0 && !ipSourceIterator->mvColEleOrdinals.empty())	//472099, column is empty
//		{
//			mvColumnWindow.push_back(ipSourceIterator->mvColEleOrdinals[lnColWidth][0]);
//		}
//	}
//	else {
//		//TODO: column window is the number of elements of distinct pivot unit
//	}
//    
//	mHeader[0].mnRows = mvRowWindow.size();
//	int lRowWidth = mHeader[0].mWidth = mvRowEleKeys.size();
//	mvRowUnitCursors.assign(ipSourceIterator->mvRowUnitCursors.begin(), ipSourceIterator->mvRowUnitCursors.begin() + lRowWidth);
//	for (i = 0; i < lRowWidth; i++)
//		mvRowUnitCursors[i] = 0;
//    
//    
//	mHeader[1].mnRows = mvColumnWindow.size();
//	int lColWidth = mHeader[1].mWidth = mvColEleKeys.size();
//	mvColUnitCursors.assign(ipSourceIterator->mvColUnitCursors.begin(), ipSourceIterator->mvColUnitCursors.begin() + lColWidth);
//	for (i = 0; i < lColWidth; i++)
//		mvColUnitCursors[i] = 0;
//    
//	hCreateKeyTable(mHeader[0].mnRows, mvRowEleOrdinals, mvRowEleKeys, mvRowKeyTable);
//	hCreateKeyTable(mHeader[1].mnRows, mvColEleOrdinals, mvColEleKeys, mvColKeyTable);
//    
//	return S_OK;
//}
//
//int DSSRWGridModelIterator::hUpdateSliceWindow(DSSRWGridModelIterator *ipSourceIterator)
//{
//	//clear the content:
//	hCleanUp(1);
//    
//	//1, copy mvRowEle***
//	int i = 0;
//	if (!mbPivotGB)
//	{
//		//copy mvRowEleOrdinal, mvRowEleKey and mvRowWindow
//		{
//			//1. calculate which depth data need copied over, and the range of local ordinal number
//			int lStartDepth = ipSourceIterator->hGetUnitStartDepth(0, mnRowGBUnits);
//			int lEndDepth = ipSourceIterator->mHeader[0].mWidth - 1;
//            
//			int lStartOrdinal = 0;
//			int lEndOrdinal = 0;
//			if (lStartDepth > 0)
//			{
//				int lParentDepth = lStartDepth - 1;
//				int lParentCursor = ipSourceIterator->mRowSliceWindowID;
//				AE_ASSERT(lParentCursor + 1< ipSourceIterator->mvRowEleOrdinals[lParentDepth].size());
//                
//				lStartOrdinal = ipSourceIterator->mvRowEleOrdinals[lParentDepth][lParentCursor];
//				lEndOrdinal = ipSourceIterator->mvRowEleOrdinals[lParentDepth][lParentCursor + 1];
//			}
//			else if (lEndDepth >= 0)
//			{	//no group-by
//				lEndOrdinal = ipSourceIterator->mvRowEleKeys[lEndDepth].size();
//			}
//			
//			//remember the start ordinal in source iterator
//			mCBRowStartOrdinal = lStartOrdinal;
//            
//			//2. copy element keys and element ordinals at every depth
//			for (int iDepth = lStartDepth; iDepth <= lEndDepth; iDepth++)
//			{
//				int lStartCursor = lStartOrdinal;
//				int lEndCursor = lEndOrdinal;
//                
//				if (lEndDepth - lStartDepth > 0)
//				{
//					bool lFind = DSSRWDHelper::BinarySearch(lStartOrdinal,
//                                                            ipSourceIterator->mvRowEleOrdinals[iDepth],
//                                                            0,
//                                                            ipSourceIterator->mvRowEleOrdinals[iDepth].size() - 1,
//                                                            lStartCursor);
//					AE_ASSERT (lFind && lStartCursor >= 0);
//                    
//					lFind = DSSRWDHelper::BinarySearch(lEndOrdinal,
//                                                       ipSourceIterator->mvRowEleOrdinals[iDepth],
//                                                       0,
//                                                       ipSourceIterator->mvRowEleOrdinals[iDepth].size() - 1,
//                                                       lEndCursor);
//					AE_ASSERT (lFind && lEndCursor >= 0);
//				}
//                
//				int lnRows = lEndCursor - lStartCursor;
//                
//				int loffset = mvRowEleKeys.size();
//				mvRowEleKeys.push_back(vector<int>(lnRows));
//				mvRowEleOrdinals.push_back(vector<int>(lnRows + 1));
//                
//				//ordinal and keys
//				int *lpOrigRowOrdinal = &(ipSourceIterator->mvRowEleOrdinals[iDepth][lStartCursor]);
//				int *lpOrigKeys = &ipSourceIterator->mvRowEleKeys[iDepth][lStartCursor];
//				for (i = 0; i < lnRows; i++)
//				{
//					mvRowEleOrdinals[loffset][i] = lpOrigRowOrdinal[i];
//					mvRowEleKeys[loffset][i] = lpOrigKeys[i];
//				}
//				mvRowEleOrdinals[loffset][i] = lpOrigRowOrdinal[i];
//			}
//            
//			int lMyRowWidth = mvRowEleKeys.size();
//			int lnRows = 1;		//if all units are group-by unit, we still need dispaly data
//			if (ipSourceIterator->mvRowWindow.empty())
//				lnRows = 0;		//468834
//            
//			if (lMyRowWidth > 0)
//				lnRows = mvRowEleKeys[lMyRowWidth - 1].size();
//            
//			if (lEndDepth - lStartDepth > 0)
//			{
//				vector<vector<int> > lvRowOrdinal;
//				hAdjustOrdinal(mvRowEleOrdinals, lvRowOrdinal);
//				mvRowEleOrdinals.assign(lvRowOrdinal.begin(), lvRowOrdinal.end());
//			}
//			else if (lMyRowWidth > 0)
//			{
//				for (i = 0; i < lnRows + 1; i++)
//				{
//					mvRowEleOrdinals[lMyRowWidth - 1][i] = i;
//				}
//			}
//            
//			//3. copy mvRowWindow
//			mvRowWindow.resize(lnRows);
//			int *lpOrigRowWindow = &ipSourceIterator->mvRowWindow[0];
//			int *lpOrigRowOrdinal = lEndDepth >= 0 ? &(ipSourceIterator->mvRowEleOrdinals[lEndDepth][lStartOrdinal]) : NULL;	//use the last depth to populate row window
//			if (lpOrigRowOrdinal)
//			{	//476022, original row header has nothing, but 1 row.
//				for (i = 0; i < lnRows; i++)
//				{
//					mvRowWindow[i] = lpOrigRowWindow[lpOrigRowOrdinal[i]];
//				}
//			}
//            
//			//4. set unit cursor
//			mHeader[0].mnRows = mvRowWindow.size();
//			mHeader[0].mWidth = lMyRowWidth;
//			mvRowUnitCursors.resize(lMyRowWidth);
//			for (i = 0; i < lMyRowWidth; i++)
//				mvRowUnitCursors[i] = 0;
//            
//			hCreateKeyTable(lnRows, mvRowEleOrdinals, mvRowEleKeys, mvRowKeyTable);
//		}
//        
//		//copy mvColEleOrdinal, mvColEleKey and mvColumnWindow
//		{
//			int lStartDepth = ipSourceIterator->hGetUnitStartDepth(1, mnColGBUnits);
//			int lEndDepth = ipSourceIterator->mHeader[1].mWidth - 1;
//            
//			int lStartOrdinal = 0;
//			int lEndOrdinal = 0;
//			if (lStartDepth > 0)
//			{
//				//TODO: add metrics
//				int lParentDepth = lStartDepth - 1;
//				int lParentCursor = ipSourceIterator->mColSliceWindowID;
//				AE_ASSERT(lParentCursor + 1 < ipSourceIterator->mvColEleOrdinals[lParentDepth].size());
//                
//				lStartOrdinal = ipSourceIterator->mvColEleOrdinals[lParentDepth][lParentCursor];
//				lEndOrdinal = ipSourceIterator->mvColEleOrdinals[lParentDepth][lParentCursor + 1];
//			}
//			else if (lEndDepth >= 0)	//472099, column is actually empty
//			{	//no group-by
//				lEndOrdinal = ipSourceIterator->mvColEleKeys[lEndDepth].size();
//			}
//			
//			//remember the start ordinal in source iterator
//			mCBColStartOrdinal = lStartOrdinal;
//            
//			for (int iDepth = lStartDepth; iDepth <= lEndDepth; iDepth++)
//			{
//				int lStartCursor = lStartOrdinal;
//				int lEndCursor = lEndOrdinal;
//                
//				if (lEndDepth - lStartDepth > 0)
//				{
//					bool lFind = DSSRWDHelper::BinarySearch(lStartOrdinal,
//                                                            ipSourceIterator->mvColEleOrdinals[iDepth],
//                                                            0,
//                                                            ipSourceIterator->mvColEleOrdinals[iDepth].size() - 1,
//                                                            lStartCursor);
//					AE_ASSERT (lFind && lStartCursor >= 0);
//                    
//					lFind = DSSRWDHelper::BinarySearch(lEndOrdinal,
//                                                       ipSourceIterator->mvColEleOrdinals[iDepth],
//                                                       0,
//                                                       ipSourceIterator->mvColEleOrdinals[iDepth].size() - 1,
//                                                       lEndCursor);
//					AE_ASSERT (lFind && lEndCursor >= 0);
//				}
//                
//				int lnCols = lEndCursor - lStartCursor;
//                
//				int loffset = mvColEleKeys.size();
//				mvColEleKeys.push_back(vector<int>(lnCols));
//				mvColEleOrdinals.push_back(vector<int>(lnCols + 1));
//                
//				//ordinal and keys
//				int *lpOrigColOrdinal = &(ipSourceIterator->mvColEleOrdinals[iDepth][lStartCursor]);
//				int *lpOrigKeys = &ipSourceIterator->mvColEleKeys[iDepth][lStartCursor];
//				for (i = 0; i < lnCols; i++)
//				{
//					mvColEleOrdinals[loffset][i] = lpOrigColOrdinal[i];
//					mvColEleKeys[loffset][i] = lpOrigKeys[i];
//				}
//				mvColEleOrdinals[loffset][i] = lpOrigColOrdinal[i];
//			}
//            
//			int lMyColWidth = mvColEleKeys.size();
//			int lnCols = 1;		//when all units are group-by unit, we should show some data
//			if (ipSourceIterator->mvColumnWindow.empty())
//				lnCols = 0;		//468834
//            
//			if (lMyColWidth > 0)
//				lnCols = mvColEleKeys[lMyColWidth - 1].size();
//            
//			if (lEndDepth - lStartDepth > 0)
//			{
//				vector<vector<int> > lvColOrdinal;
//				hAdjustOrdinal(mvColEleOrdinals, lvColOrdinal);
//				mvColEleOrdinals.assign(lvColOrdinal.begin(), lvColOrdinal.end());
//			}
//			else if (lMyColWidth > 0)
//			{
//				for (i = 0; i < lnCols + 1; i++)
//				{
//					mvColEleOrdinals[lMyColWidth - 1][i] = i;
//				}
//			}
//            
//			//column window
//			mvColumnWindow.resize(lnCols);
//			int *lpOrigColWindow = &ipSourceIterator->mvColumnWindow[0];
//			int *lpOrigColOrdinal = lEndDepth >= 0 ? &(ipSourceIterator->mvColEleOrdinals[lEndDepth][lStartOrdinal]) : NULL;
//			for (i = 0; i < lnCols; i++)
//			{
//				mvColumnWindow[i] = lpOrigColWindow[lpOrigColOrdinal[i]];
//			}
//            
//			//unit cursor
//			mHeader[1].mnRows = mvColumnWindow.size();
//			mHeader[1].mWidth = lMyColWidth;
//			mvColUnitCursors.resize(lMyColWidth);
//			for (i = 0; i < lMyColWidth; i++)
//				mvColUnitCursors[i] = 0;
//            
//			hCreateKeyTable(lnCols, mvColEleOrdinals, mvColEleKeys, mvColKeyTable);
//		}
//	}
//	else{
//		//TODO:
//	}
//	return S_OK;
//}
//
//void DSSRWGridModelIterator::hAdjustOrdinal(vector<vector<int> > &irvOrigOrdinal, vector<vector<int> > &orvNewOrdinal)
//{
//	// adjust the ordinal number
//	//	irvOrigOdrinal	==>  orvNewOridnal
//	//	 0	 0	 0				0	0	0
//	//			 4						1
//	//		 7	 7					2	2
//	//	10	10	10		==>		3	3	3
//	//			16						4
//	//		19	19					5	5
//	//			25						6
//	//	30	30	30				7	7	7
//	//
//    
//	orvNewOrdinal.assign(irvOrigOrdinal.begin(), irvOrigOrdinal.end());
//    
//	int lnUnits = irvOrigOrdinal.size();
//	if (lnUnits == 0)
//		return;
//	int lSize = orvNewOrdinal[lnUnits - 1].size();
//	int i = 0;
//	for (i = 0; i < lSize; i++)
//	{
//		orvNewOrdinal[lnUnits - 1][i] = i;
//	}
//    
//	for (i = lnUnits - 2; i >= 0; i--)
//	{
//		lSize = orvNewOrdinal[i].size();
//		int lChildIndex = 0;
//		int lChildSize = orvNewOrdinal[i + 1].size();
//		for (int j = 0; j < lSize; j++)
//		{
//			//check which child has the same ordinal
//			int lOrdinal = irvOrigOrdinal[i][j];
//			while (lChildIndex < lChildSize)
//			{
//				if (irvOrigOrdinal[i + 1][lChildIndex] < lOrdinal)
//					lChildIndex++;
//				else
//					break;
//			}
//			_ASSERT(lChildIndex < lChildSize);
//            
//			//copy that child's new ordinal number
//			orvNewOrdinal[i][j] = orvNewOrdinal[i + 1][lChildIndex];
//		}
//	}
//}
//
//int DSSRWGridModelIterator::hCreateKeyTable(int nRows, vector<vector<int> > &irOrdinal, vector<vector<int> > &irKeys, vector<int> &orKeyTable)
//{
//	int lnWidth = irKeys.size();
//	orKeyTable.resize(nRows * lnWidth);
//	int *lpKeyTable = &orKeyTable[0];
//	int count = 0;
//	for (int i = 0; i < lnWidth; i++)
//	{
//		int lnElements = irKeys[i].size();
//		int iRow = 0;
//		for (int j = 0; j < lnElements; j++)
//		{
//			int key = irKeys[i][j];
//			int lNextOrdinal = irOrdinal[i][j + 1];
//			_ASSERT(lNextOrdinal <= nRows);
//			while (iRow < lNextOrdinal)
//			{
//				lpKeyTable[count++] = key;
//				iRow++;
//			}
//		}
//		_ASSERT(iRow == nRows);
//	}
//	return 0;
//}
//
//void DSSRWGridModelIterator::hSelectDistinct(vector<int> &irSrc, vector<int> &orDes)
//{
//	set<int> lvSet;
//	vector<int>::iterator it = irSrc.begin();
//	for (; it < irSrc.end(); it++)
//	{
//		lvSet.insert(*it);
//	}
//    
//	set<int>::iterator it2 = lvSet.begin();
//	for (; it2 != lvSet.end(); it2++)
//	{
//		orDes.push_back(*it2);
//	}
//}
//
//DSSConsolidationElement* DSSRWGridModelIterator::getConsolidationElement(int iAxis, int iDepth)
//{
//	DSSTabularUnit *lpUnit = mvRowXTabUnits[iDepth];
//	if (!lpUnit)
//		return NULL;
//    
//	int lKey = mvRowEleKeys[iDepth][mvRowUnitCursors[iDepth]];
//    
//	if (lpUnit->getUnitType() == DssTemplateConsolidation && lKey >= 0)
//	{
//		return reinterpret_cast<DSSTabularConsolidation*> (lpUnit)->getConsolidationElement(lKey);
//	}
//	return NULL;
//}
//
//DSSConsolidationElement* DSSRWGridModelIterator::getConsolidationElementAt(int iAxis, int iOrdinal, int iDepth)
//{
//	DSSTabularUnit *lpUnit = NULL;
//	int lKey = 0;
//	if (iAxis == 0)
//	{
//		lpUnit = mvRowXTabUnits[iDepth];
//		if (!lpUnit)
//			return NULL;
//		lKey = mvRowKeyTable[iDepth * mHeader[0].mnRows + iOrdinal];
//	}
//	else if (iAxis == 1)
//	{
//		lpUnit = mvColXTabUnits[iDepth];
//		if (!lpUnit)
//			return NULL;
//		lKey = mvColKeyTable[iDepth * mHeader[1].mnRows + iOrdinal];
//	}
//    
//	if (lpUnit->getUnitType() == DssTemplateConsolidation && lKey >= 0)
//	{
//		return reinterpret_cast<DSSTabularConsolidation*> (lpUnit)->getConsolidationElement(lKey);
//	}
//	return NULL;
//}
//
//bool DSSRWGridModelIterator::MoveCursor(int iAxis, int iOrdinal)
//{
//	if (iAxis == 0)
//	{
//		mRowOrdinal = iOrdinal;
//		int width = mvRowEleOrdinals.size();
//		for (int i = 0; i < width; i++)
//		{
//			int lOffset = -1;
//			bool lFind = DSSRWDHelper::BinarySearch(iOrdinal, mvRowEleOrdinals[i], 0, mvRowEleOrdinals[i].size() - 1, lOffset);
//			if (lFind && lOffset >= 0)
//			{
//				mvRowUnitCursors[i] = lOffset;
//			}
//			else
//				return false;
//		}
//	}
//	else if (iAxis == 1)
//	{
//		mColOrdinal = iOrdinal;
//		int width = mvColEleOrdinals.size();
//		for (int i = 0; i < width; i++)
//		{
//			int lOffset = -1;
//			bool lFind = DSSRWDHelper::BinarySearch(iOrdinal, mvColEleOrdinals[i], 0, mvColEleOrdinals[i].size() - 1, lOffset);
//			if (lFind && lOffset >= 0)
//			{
//				mvColUnitCursors[i] = lOffset;
//			}
//			else
//				return false;
//		}
//	}
//	return true;
//}
//
//int DSSRWGridModelIterator::getCellNullDisplay(DSSDataFlag iDataFlag, MBase::CString &orVal)
//{
//	return mpGridModel->getDataNullDisplay(iDataFlag, orVal);
//}
//
//int DSSRWGridModelIterator::getElementNullDisplay(DSSDataFlag iDataFlag, MBase::CString &orVal)
//{
//	return mpGridModel->getElementNullDisplay(iDataFlag, orVal);
//}
//
//int DSSRWGridModelIterator::getMaxMetricValue(GUID &irMetriID, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag)
//{
//    int lMetricIndex = mpGridModel->FindMetric(irMetriID);
//	if (lMetricIndex < 0)
//		return E_FAIL;
//    
//	int lOffset = mpGridModel->getMaxDataOffset(lMetricIndex);
//    
//	DSSCubeDataSlice *lpSlice = NULL;
//	mpXTabView->getDataSlice(0, 0, lMetricIndex, &lpSlice);		//only check report level
//	CHECK_PTR(lpSlice);
//    
//	DSSDataColumn *lpColumn = lpSlice->getColumn();
//	CHECK_PTR(lpColumn);
//    
//	return lpColumn->getData(lOffset, (const char**)oppData, *opDataSize, *opDataType, *opDataFlag);
//}
//
//int DSSRWGridModelIterator::getMinMetricValue(GUID &irMetriID, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag)
//{
//	int lMetricIndex = mpGridModel->FindMetric(irMetriID);
//	if (lMetricIndex < 0)
//		return E_FAIL;
//    
//	int lOffset = mpGridModel->getMinDataOffset(lMetricIndex);
//    
//	DSSCubeDataSlice *lpSlice = NULL;
//	mpXTabView->getDataSlice(0, 0, lMetricIndex, &lpSlice);		//only check report level
//	CHECK_PTR(lpSlice);
//    
//	DSSDataColumn *lpColumn = lpSlice->getColumn();
//	CHECK_PTR(lpColumn);
//    
//	return lpColumn->getData(lOffset, (const char**)oppData, *opDataSize, *opDataType, *opDataFlag);
//}
//
//DSSTemplateUnit * DSSRWGridModelIterator::getTemplateUnit(DSSCellHandle &irHandle, bool iNeedTemplateMetric)
//{
//	if (irHandle.type == DssCellHandleTemplateUnit ||
//		irHandle.type == DssCellHandleTemplateUnitAndForm ||
//		irHandle.type == DssCellHandleCustomGroupAndDepth)
//	{
//		return irHandle.p.unit;
//	}
//	else if (irHandle.type == DssCellHandleXTabHeaderElement)
//	{
//		DSSTemplate * lpTemplate = static_cast<DSSRWTemplateNode*>(getRWNode())->getTemplate();
//		if (!lpTemplate)
//			return NULL;
//        
//		GUID lUnitID = GUID_NULL;
//        
//		DSSTabularUnit *lpXTabUnit = irHandle.p.xtabUnit;
//		if (lpXTabUnit->getUnitType() != DssTemplateMetrics)
//		{
//			lpXTabUnit->getGUID(lUnitID);
//		}
//		else
//		{
//			int lOrdinal = irHandle.v.ordinal;	// obtain the ordinal of the element
//            
//			int lKey;		// Need to convert ordinal to key
//			int lUnitIndex = lpXTabUnit->getUnitIndex();
//			int lAxisIndex = mvUnitMapToAxis[lUnitIndex];
//			if (lAxisIndex == 0)
//				lKey = mpXTabView->getHeader(lAxisIndex)->getMetricID(mvRowWindow[lOrdinal]);
//			else
//				lKey = mpXTabView->getHeader(lAxisIndex)->getMetricID(mvColumnWindow[lOrdinal]);
//            
//			static_cast<DSSTabularMetrics*>(lpXTabUnit)->getMetricID(lKey, lUnitID);
//		}
//        
//		return lpTemplate->getTemplateUnitFromAxisIndex(DssAxisAll, lUnitID, iNeedTemplateMetric);
//	}
//	else if (irHandle.type == DssCellHandleXTabData)
//	{
//		//try to find out the metric
//		DSSTemplate * lpTemplate = static_cast<DSSRWTemplateNode*>(getRWNode())->getTemplate();
//		if (!lpTemplate)
//			return NULL;
//		
//		int lnCols = mpXTabView->getHeader(1)->getRowCount();
//		int lCellID = irHandle.v.cell;
//		int row = lnCols != 0 ? lCellID / lnCols : lCellID;
//		int col = lnCols != 0 ? lCellID % lnCols : 0;
//		
//		int lMetricIndex = mpXTabView->getMetricID(row, col);
//		
//		GUID lMetricID = GUID_NULL;
//		int hr = mpGridModel->getMetricID(lMetricIndex, lMetricID);
//		if (hr != S_OK)
//			return NULL;
//		
//		return lpTemplate->getTemplateUnitFromAxisIndex(DssAxisAll, lMetricID, iNeedTemplateMetric);
//		
//	}
//	else
//		return NULL;
//}
//
//DSSBaseElementProxy* DSSRWGridModelIterator::getCustomGroupElement(int iAxis, int iDepth)
//{
//	DSSTabularUnit *lpUnit = mvRowXTabUnits[iDepth];
//	if (!lpUnit)
//		return NULL;
//    
//	int lKey = mvRowEleKeys[iDepth][mvRowUnitCursors[iDepth]];
//    
//	if (lpUnit->getUnitType() == DssTemplateCustomGroup && lKey >= 0)
//	{
//		return reinterpret_cast<DSSTabularCustomGroup*> (lpUnit)->getCustomGroupElement(lKey);
//	}
//	return NULL;
//}
//
//DSSBaseElementProxy* DSSRWGridModelIterator::getCustomGroupElementAt(int iAxis, int iOrdinal, int iDepth)
//{
//	DSSTabularUnit *lpUnit = NULL;
//	int lKey = 0;
//	if (iAxis == 0)
//	{
//		lpUnit = mvRowXTabUnits[iDepth];
//		if (!lpUnit)
//			return NULL;
//		lKey = mvRowKeyTable[iDepth * mHeader[0].mnRows + iOrdinal];
//	}
//	else if (iAxis == 1)
//	{
//		lpUnit = mvColXTabUnits[iDepth];
//		if (!lpUnit)
//			return NULL;
//		lKey = mvColKeyTable[iDepth * mHeader[1].mnRows + iOrdinal];
//	}
//    
//	if (lpUnit->getUnitType() == DssTemplateCustomGroup && lKey >= 0)
//	{
//		return reinterpret_cast<DSSTabularCustomGroup*> (lpUnit)->getCustomGroupElement(lKey);
//	}
//	return NULL;
//}
//
//int DSSRWGridModelIterator::hInitTemplateStructure(DSSRWNode * ipNode)
//{
//	if (!ipNode)
//		return E_POINTER;
//    
//	DSSTemplate *lpTemplate = static_cast<DSSRWTemplateNode*> (ipNode)->getTemplate();
//	CHECK_PTR(lpTemplate);
//    
//	//cache template metrics unit and whether metrics are on row
//	mbHasMetricOnRow = false;	// default to false
//    
//	mpTemplateMetrics = lpTemplate->getTemplateMetrics();
//	if (mpTemplateMetrics)
//	{
//		if (mpTemplateMetrics->getAxis()->getIndex() == DssAxisRows)
//		{
//			mbHasMetricOnRow = true;
//		}
//	}
//    
//	// cache whether there are GB untis on the path
//	std::vector<DSSGroupByUnit*> lvGBUnits;
//	ipNode->getGroupByPath(lvGBUnits);
//	mbNodeHasGBUnits = lvGBUnits.size() > 0;
//    
//	// cache template unit
//	int lnUnits = mpGridModel->Count();
//	mvUnitMapToAxis.clear();
//	mvpTemplateUnits.clear();
//	mvUnitMapToAxis.resize(lnUnits);
//	mvpTemplateUnits.resize(lnUnits);
//	int i = 0;
//	for (i = 0; i < lnUnits; i++)
//	{
//		mvUnitMapToAxis[i] = -1;
//		mvpTemplateUnits[i] = NULL;
//	}
//    
//	//TODO: code clean up: don't use mvpTemplateUnits
//	//		move mvXXXTabUnits and mvXXXTemplateUnits out of ReInit()
//	//		merge the common part of ReInit(), Populate(), Update()
//	for (int iAxis = 0; iAxis < 2; iAxis++)
//	{
//		DSSXTabHeader *lpHeader = mpXTabView->getHeader(iAxis);
//		CHECK_PTR(lpHeader);
//        
//		int lnHeaderUnits = lpHeader->getNumUnits();
//		for (i = 0; i < lnHeaderUnits; i++)
//		{
//			int lUnitID = lpHeader->getUnit(i);
//			_ASSERT(lUnitID < lnUnits);
//            
//			mvUnitMapToAxis[lUnitID] = iAxis;
//            
//			DSSTabularUnit *lpXTabUnit = mpGridModel->Item(lUnitID);
//			CHECK_PTR(lpXTabUnit);
//            
//			if (lpXTabUnit->getUnitType() != DssTemplateMetrics)
//			{
//				GUID lGUID;
//				lpXTabUnit->getGUID(lGUID);
//				DSSTemplateUnit *lpUnit = lpTemplate->getTemplateUnitFromAxisIndex(iAxis + 1, lGUID);
//				mvpTemplateUnits[lUnitID] = lpUnit;
//			}
//			else
//			{
//				DSSTemplateUnit *lpUnit = lpTemplate->getTemplateMetrics();
//				mvpTemplateUnits[lUnitID] = lpUnit;
//			}
//		}
//	}
//	return S_OK;
//}
//
//bool DSSRWGridModelIterator::IsDerivedElement(int iAxis, int iDepth)
//{
//	if ((iAxis == 0 && iDepth >= mvRowXTabUnits.size()) || (iAxis == 1 && iDepth >= mvColXTabUnits.size()))
//		return false;
//    
//	DSSTabularUnit *lpUnit = iAxis == 0 ? mvRowXTabUnits[iDepth] : mvColXTabUnits[iDepth];
//	if (lpUnit && lpUnit->getUnitType() == DssTemplateConsolidation)
//	{
//		return static_cast<DSSTabularConsolidation *> (lpUnit)->IsDerivedElement();
//	}
//	return false;
//}
//
//bool DSSRWGridModelIterator::IsDerivedElementAt(int iAxis, int iOrdinal, int iDepth)
//{
//	if ((iAxis == 0 && iDepth >= mvRowXTabUnits.size()) || (iAxis == 1 && iDepth >= mvColXTabUnits.size()))
//		return false;
//	
//	DSSTabularUnit *lpUnit = iAxis == 0 ? mvRowXTabUnits[iDepth] : mvColXTabUnits[iDepth];
//	if (lpUnit && lpUnit->getUnitType() == DssTemplateConsolidation)
//	{
//		DSSTabularConsolidation *lpConsolidationUnit = static_cast<DSSTabularConsolidation *> (lpUnit);
//		if (lpConsolidationUnit->IsDerivedElement())
//		{
//			int lKey = -1;
//			if (iAxis == 0)
//			{
//				lKey = mvRowKeyTable[iDepth * mHeader[0].mnRows + iOrdinal];
//			}
//			else if (iAxis == 1)
//			{
//				lKey = mvColKeyTable[iDepth * mHeader[1].mnRows + iOrdinal];
//			}
//			return (lKey >= 0 && NULL != lpConsolidationUnit->getConsolidationElement(lKey));
//		}
//	}
//	return false;
//}
//
//bool DSSRWGridModelIterator::HasDerivedElementAlongPath(DSSCellHandle &irHandle)
//{
//	// The method checks whether the elements from root to this unit (include itself) have DE or not
//	if (irHandle.type == DssCellHandleXTabHeaderElement)
//	{
//		DSSTabularUnit *lpUnit = irHandle.p.xtabUnit;
//        
//		if (lpUnit)
//		{
//			// First check whether myself is a DE
//			if ((lpUnit->getUnitType() == DssTemplateConsolidation) &&
//				static_cast<DSSTabularConsolidation *> (lpUnit)->IsDerivedElement())
//			{
//				return true;
//			}
//			else
//			{
//				// Then check whehter element on the path has DE
//				int lUnitID = lpUnit->getUnitIndex();
//				int lAxis = mvUnitMapToAxis[lUnitID];
//                
//				DSSXTabHeader *lpXTabHeader = mpXTabView->getHeader(lAxis);
//				if (lpXTabHeader)
//				{
//					int lnHeaderUnits = lpXTabHeader->getNumUnits();
//					for (int i = 0; i < lnHeaderUnits; i++)
//					{
//						int liUnitIndex = lpXTabHeader->getUnit(i);
//                        
//						// Break when reaching this element
//						if (liUnitIndex == lUnitID)
//							break;
//                        
//						DSSTabularUnit *lpiUnit = mpGridModel->Item(liUnitIndex);
//						if (lpiUnit &&
//							(lpiUnit->getUnitType() == DssTemplateConsolidation) &&
//							static_cast<DSSTabularConsolidation *> (lpiUnit)->IsDerivedElement())
//						{
//							return true;
//						}
//					}
//				}
//			}
//		}
//	}
//	return false;
//}
//
//int DSSRWGridModelIterator::hGetUnitStartDepth(int iAxis, int iUnitIndex)
//{
//	int count = 0;
//	int lnUnits = mHeader[iAxis].mnUnits;
//	for (int i = 0; i < iUnitIndex && i < lnUnits; i++)
//	{
//		count += mHeader[iAxis].mvUnitWidth[i];
//	}
//	return count;
//}
//
//int DSSRWGridModelIterator::getXTabElementKey(DSSCellHandle &irHandle, DSSTabularUnit *ipUnit, int &orKey)
//{
//	int hr = S_OK;
//    
//	int lUnitID = ipUnit->getUnitIndex();
//	int lAxis = mvUnitMapToAxis[lUnitID];
//	_ASSERT(lAxis != -1);
//    
//	int lRowOffset = 0;
//    
//	if (irHandle.type == DssCellHandleXTabHeaderElement)
//	{
//		//int lOrdinal = irHandle.v.ordinal;
//		//lRowOffset = lAxis == 0 ? mvRowWindow[lOrdinal] : mvColumnWindow[lOrdinal];
//		orKey = irHandle.x.key;
//		return S_OK;
//	}
//	else if (irHandle.type == DssCellHandleXTabData)
//	{
//		int lnCols = mpXTabView->getHeader(1)->getRowCount();
//		int lOffset = irHandle.v.cell;
//		if (lAxis == 0)
//			lRowOffset = lnCols != 0 ? lOffset / lnCols : lOffset;		//row ID
//		else
//			lRowOffset = lnCols != 0 ? lOffset % lnCols : 0;			//column ID
//	}
//	else
//	{
//		return S_FALSE;
//	}
//    
//	int lnUnits = mpGridModel->Count();
//	if (lnUnits == 0)	//no unit, no elements
//		return S_FALSE;
//    
//	vector<int> lvKeys(lnUnits);
//	int i = 0;
//	for (i = 0; i < lnUnits; i++)
//		lvKeys[i] = DSSXTAB_TOTAL_KEY;
//    
//	DSSXTabHeader *lpXTabHeader = mpXTabView->getHeader(lAxis);
//	_ASSERT(lpXTabHeader);
//    
//	int lMetricKey = 0;
//	//hr = lpXTabHeader->FillIndex(lRowOffset, lnUnits, &lvKeys[0], &lMetricKey);
//	hr = lpXTabHeader->FillIndexWithTag(lRowOffset, lnUnits, &lvKeys[0], &lMetricKey);
//	CHECK_HR;
//    
//	orKey = lvKeys[ipUnit->getUnitIndex()];
//    
//	if (ipUnit->getUnitType() == DssTemplateMetrics)	//TQMS 408066
//		orKey = lMetricKey;
//    
//	return S_OK;
//}
//
//
//bool DSSRWGridModelIterator::HasRuntimeUnit()
//{
//	for (int iAxis = 0; iAxis < 2; iAxis++)
//	{
//		DSSXTabHeader *lpHeader = mpXTabView->getHeader(iAxis);
//		CHECK_PTR(lpHeader);
//        
//		if (lpHeader ->getNumRuntimeUnits() > 0)
//			return true;
//	}
//	return false;
//}
//
//bool DSSRWGridModelIterator::HasGroupbyMetrics()
//{
//	vector<int> lvGBUnits = mpGridModel->getGroupbyUnits();
//	for (int i = 0; i < lvGBUnits.size(); i++)
//	{
//		DSSTabularUnit *lpUnit = mpGridModel->Item(lvGBUnits[i]);
//		if (lpUnit)
//		{
//			if (lpUnit->getUnitType() == DssTemplateMetrics)
//				return true;
//		}
//	}
//	return false;
//}
//
//int DSSRWGridModelIterator::getSelectedMetrics(vector<GUID> &orMetricsList)
//{
//	   
//	int lnMetrics = mpGridModel->Count();
//	if (!HasGroupbyMetrics())
//	{
//		//all metrics are selected and displayed
//		orMetricsList.resize(lnMetrics);
//		for (int i = 0; i < lnMetrics; i++)
//		{
//			int hr = mpGridModel->getMetricID(i, orMetricsList[i]);
//			CHECK_HR;
//		}
//		return S_OK;
//	}
//    
//	//if there's metric selector, check the metric unit keys
//	vector<int> *lpvKeys = NULL;
//	if (mbHasMetricOnRow)
//	{
//		for (int i = 0; i < mvRowXTabUnits.size(); i++)
//		{
//			if (mvRowXTabUnits[i] && mvRowXTabUnits[i]->getUnitType() == DssTemplateMetrics)
//			{
//				lpvKeys = &mvRowEleKeys[i];
//			}
//		}
//	}
//	else
//	{
//		for (int i = 0; i < mvColXTabUnits.size(); i++)
//		{
//			if (mvColXTabUnits[i] && mvColXTabUnits[i]->getUnitType() == DssTemplateMetrics)
//			{
//				lpvKeys = &mvColEleKeys[i];
//			}
//		}
//	}
//    
//	if (!lpvKeys)
//		return S_OK;
//    
//	//mark all the selected metrics
//	int count = 0;
//	vector<int> lvMask(lnMetrics);
//	memset(&lvMask[0], 0, sizeof(int) * lnMetrics);
//    
//	vector<int>::iterator it = lpvKeys->begin();
//	vector<int>::iterator end = lpvKeys->end();
//	for (; it < end; it++)
//	{
//		AE_ASSERT(*it >= 0 && *it < lnMetrics);
//		if (!lvMask[*it])
//		{
//			lvMask[*it] = 1;
//			count ++;
//			if (count == lnMetrics)
//				break;
//		}
//	}
//    
//	//get the selected metric GUID
//	for (int i = 0; i < lnMetrics; i++)
//	{
//		if (lvMask[i] == 1)
//		{
//			GUID lMetricID = GUID_NULL;
//			int hr = mpGridModel->getMetricID(i, lMetricID);
//			CHECK_HR;
//            
//			orMetricsList.push_back(lMetricID);
//		}
//	}
//	return S_OK;
//}
//
//void DSSRWGridModelIterator::setExcludeTotal(bool flag)
//{
//	mbExcludeTotal = flag;
//}
//
////lwang, 411897, add support to highlight current selection for template selector
//int DSSRWGridModelIterator::SearchElement(int iAxis, int iDepth, DSSBaseElementsProxy *ipElements, vector<int> &orvOrdinal, bool &orbHasALL)
//{
//	if ((iAxis == 0 && iDepth >= mvRowXTabUnits.size()) || (iAxis == 1 && iDepth >= mvColXTabUnits.size()))
//		return false;
//    
//	//1. check if there's ALL elements
//	int lnElements = ipElements->Count();
//	int i = 0;
//	for (i = 0; i < lnElements; i++)
//	{
//		if (DssElementAll == ipElements->Item(i)->getElementType())
//		{
//			orbHasALL = true;
//			return S_OK;
//		}
//	}
//    
//	orbHasALL = false;
//    
//	DSSTabularUnit *lpUnit = iAxis == 0 ? mvRowXTabUnits[iDepth] : mvColXTabUnits[iDepth];
//    
//	int lnRows = mHeader[iAxis].mnRows;
//	_ASSERT( (iAxis == 0 && (iDepth + 1) * lnRows <= mvRowKeyTable.size())
//			||(iAxis == 1 && (iDepth + 1) * lnRows <= mvColKeyTable.size()));
//	int *lpElementKeys = iAxis == 0 ? &mvRowKeyTable[iDepth * lnRows] : &mvColKeyTable[iDepth * lnRows];
//    
//	for (i = 0; i < lnElements; i++)
//	{
//		DSSBaseElementProxy *lpElement = ipElements->Item(i);
//        
//		int lKey = 0;
//		if (lpElement->getElementType() != DssElementSubtotal)
//		{
//			int hr = lpUnit->FindElement(lpElement, lKey);
//			CHECK_HR;
//		}
//		else
//		{
//			DSSRWNode *lpThisNode = getRWNode();
//			_ASSERT(lpThisNode);
//			GUID lTemplateID =  static_cast<DSSRWTemplateNode*> (lpThisNode)->getTemplate()->getID();
//            
//			//get the local subtoal index
//			int hr = mpObjectContext->getTemplateLevelSubtotalIndex(static_cast<DSSSubtotalElementProxy*>(lpElement)->getSubtotalIndex(),
//																	&lTemplateID,
//																	&lKey);
//			CHECK_HR;
//            
//			if (lKey <= 0)
//				continue;
//            
//			lKey = -lKey;
//		}
//        
//		for (int ordinal = 0; ordinal < lnRows; ordinal++, lpElementKeys++)
//		{
//			if (*lpElementKeys == lKey)
//				orvOrdinal.push_back(ordinal);
//		}
//	}
//    
//	return S_OK;
//}
//
//
//int DSSRWGridModelIterator::getGraphDrillUnitAndCellHandle(int iRowIndex, int iColumnIndex, DSSTemplateUnit* & opDrillUnit, DSSCellHandle & oCellHandle)
//{
//	CHECK_PTR(mpGridModel);
//	
//	if ((iRowIndex < 0) && (iColumnIndex < 0))
//	{
//		opDrillUnit = NULL;
//		oCellHandle = DSSCellHandle();
//	}
//	else
//	{
//		DSSRWNode *lpThisNode = getRWNode();
//		_ASSERT(lpThisNode);
//        
//		DSSTemplate *lpTemplate = static_cast<DSSRWTemplateNode*> (lpThisNode)->getTemplate();
//		CHECK_PTR(lpTemplate);
//        
//		if ((iRowIndex >= 0) && (iColumnIndex < 0))
//		{
//			// Find the last non-metric unit on row
//			opDrillUnit = lpTemplate->getInnermostUnit(DssAxisRows, false);
//			// Get the innermost element on row, getElementHandleAt uses 0-based index
//			oCellHandle = getElementHandleAt(DssAxisRows-1, iRowIndex, -1);
//		}
//		else if ((iRowIndex < 0) && (iColumnIndex >= 0))
//		{
//			// Find the last non-metric unit on column
//			opDrillUnit = lpTemplate->getInnermostUnit(DssAxisColumns, false);
//			// Get the innermost element on column, getElementHandleAt uses 0-based index
//			oCellHandle = getElementHandleAt(DssAxisColumns-1, iColumnIndex, -1);
//		}
//		else
//		{
//			// Determine the Categories axis
//			int lAxisIndex = DssAxisRows;	// By default the Categories maps to row
//			DSSGraphProperties *lpGraphProperties = static_cast<DSSRWTemplateNode*> (lpThisNode)->getGraphProperties();
//			if (lpGraphProperties)
//			{
//				if (lpGraphProperties->getSeriesByColumn())
//					lAxisIndex = DssAxisRows;
//				else
//					lAxisIndex = DssAxisColumns;
//			}
//			// Find the last non-metric unit on axis maps to categories
//			opDrillUnit = lpTemplate->getInnermostUnit(lAxisIndex, false);
//			// Get the cell
//			oCellHandle = getCellHandleAt(iRowIndex, iColumnIndex);
//		}
//	}
//	return S_OK;
//}
//
//int DSSRWGridModelIterator::LookupCellFormatID(int iFmtID, GUID &orFmtGUID)
//{
//	return mpGridModel->getCellFmtIDs(iFmtID, orFmtGUID);
//}
//
//bool DSSRWGridModelIterator::HasCellFormatData()
//{
//	GUID lFmtGUID;
//	return (S_OK == mpGridModel->getCellFmtIDs(0, lFmtGUID));
//}
//
//int DSSRWGridModelIterator::createCellHandleFromElement(int iAxis, int iDepth, DSSBaseElementProxy *ipElement, DSSCellHandle &orCellHandle)
//{
//	orCellHandle.type = DssCellHandleReserved;
//    
//	if ((iAxis == 0 && iDepth >= mvRowXTabUnits.size()) || (iAxis == 1 && iDepth >= mvColXTabUnits.size()))
//		return E_INVALIDARG;
//    
//	//check if it's ALL elements
//	if (DssElementAll == ipElement->getElementType())
//	{
//		orCellHandle.type = DssCellHandleTemplateUnit;
//		orCellHandle.p.unit = getUnit(iAxis, iDepth);
//		return S_OK;
//	}
//    
//	DSSTabularUnit *lpUnit = iAxis == 0 ? mvRowXTabUnits[iDepth] : mvColXTabUnits[iDepth];
//    
//	int lKey = 0;
//	if (ipElement->getElementType() != DssElementSubtotal)
//	{
//		int hr = lpUnit->FindElement(ipElement, lKey);
//		CHECK_HR;
//	}
//	else
//	{
//		DSSRWNode *lpThisNode = getRWNode();
//		_ASSERT(lpThisNode);
//		GUID lTemplateID =  static_cast<DSSRWTemplateNode*> (lpThisNode)->getTemplate()->getID();
//        
//		//get the local subtoal index
//		int hr = mpObjectContext->getTemplateLevelSubtotalIndex(static_cast<DSSSubtotalElementProxy*>(ipElement)->getSubtotalIndex(),
//																&lTemplateID,
//																&lKey);
//		CHECK_HR;
//        
//		if (lKey <= 0)
//			return E_FAIL;
//        
//		lKey = -lKey;
//	}
//    
//	int lnRows = mHeader[iAxis].mnRows;
//	_ASSERT( (iAxis == 0 && (iDepth + 1) * lnRows <= mvRowKeyTable.size())
//			||(iAxis == 1 && (iDepth + 1) * lnRows <= mvColKeyTable.size()));
//	int *lpElementKeys = iAxis == 0 ? &mvRowKeyTable[iDepth * lnRows] : &mvColKeyTable[iDepth * lnRows];
//    
//	for (int ordinal = 0; ordinal < lnRows; ordinal++, lpElementKeys++)
//	{
//		if (*lpElementKeys == lKey)
//		{
//			orCellHandle.type = DssCellHandleXTabHeaderElement;
//			orCellHandle.p.xtabUnit = lpUnit;
//			orCellHandle.v.ordinal = ordinal;
//			orCellHandle.x.key = lKey;
//			return S_OK;
//		}
//	}
//    
//	return S_OK;
//}
//
//// tehe: the method is already defined in old grid iterator
//int hSelectDistinct2(vector<int> &irvKeyTable, int nKeys, int nTotalKeys, vector<int> &orSortedList, vector<int> &orDistinctList)
//{
//	if (nKeys == 0 || irvKeyTable.size() == 0)
//		return 0;
//    
//	int lnRows = irvKeyTable.size() / nTotalKeys;		//nTotalKeys can be greater than nKeys
//	orSortedList.resize(lnRows);
//    
//	int i = 0;
//	for (i = 0; i < lnRows; i++)
//		orSortedList[i] = i;
//    
//	//1. sort the original rows		TODO[opt]: use bucket sort
//	int *lpKeys = &irvKeyTable[0];
//	ElementSort<int> lSorter(lpKeys, lnRows, nTotalKeys);
//	for (i = nKeys - 1; i >= 0; --i)
//	{
//		lSorter.SetUnit(i);
//		_xtab_sort(&orSortedList[0], &orSortedList[0] + lnRows, lSorter);	//merge sort
//	}
//    
//	//2. select distinct
//	orDistinctList.reserve(lnRows);
//	int *lpPre = NULL;
//	int *lpCur = NULL;
//	if (lnRows > 0)
//	{
//		lpPre = lpKeys + nTotalKeys * orSortedList[0];
//		orDistinctList.push_back(orSortedList[0]);
//	}
//    
//	for (i = 1; i < lnRows; i++)
//	{
//		lpCur = lpKeys + nTotalKeys * orSortedList[i];
//		for (int j = nKeys -1; j >= 0; --j)
//		{
//			if (lpCur[j] != lpPre[j])
//			{
//				orDistinctList.push_back(orSortedList[i]);
//				break;
//			}
//		}
//		lpPre = lpCur;
//	}
//	
//	//483233, keep the original row ordinal
//	std::sort(orDistinctList.begin(), orDistinctList.end());
//	
//	return 0;
//}
//// tehe: the similar method is already defined in old grid iterator
//int hCreateViewMap2(int nUnits, int nRows, int *ipKeys, DSSXTabHashTable **oppColorMap)
//{
//	vector<int> lvBucketSpace(nUnits);
//	vector<int> lvMaxKeys(nUnits);
//    
//	int lNumBuckets = 0;
//	int *lpUnitBucketSpace = NULL;
//	int *lpUnitMaxKey = NULL;
//    
//	if (nUnits > 0)
//	{
//		memset(&lvMaxKeys[0], 0, sizeof(int)*nUnits);
//		lpUnitBucketSpace = &lvBucketSpace[0];
//		lpUnitMaxKey = &lvMaxKeys[0];
//	}
//    
//	*oppColorMap = new DSSXTabHashTable();
//	int hr = (*oppColorMap)->CheckKeyValues(nUnits, nRows, ipKeys, lpUnitBucketSpace, lNumBuckets, lpUnitMaxKey);
//	CHECK_HR;
//    
//	hr = (*oppColorMap)->Init(lNumBuckets, nUnits, lpUnitBucketSpace);
//	CHECK_HR;
//    
//	return (*oppColorMap)->HashAll(ipKeys, nUnits, nRows);
//}
//
//int hCreateColorLookupTable2(vector<int> &irvKeyTable, int nRows, int nUnits, vector<int> &orvKeyTable, int *opDistinctRows, DSSXTabHashTable **oppColorMap)
//{
//	if(nUnits > 0)
//	{
//		if (nRows * nUnits != irvKeyTable.size())
//			return E_FAIL;
//        
//		//TODO:[perf], it may already e sorted. If so, no need to sort and use mvRowList
//		vector<int> lvSortedList;
//		vector<int> lvDistinctList;
//		hSelectDistinct2(irvKeyTable, nUnits, nUnits, lvSortedList, lvDistinctList);
//		int lnDistinctRows = lvDistinctList.size();
//        
//		orvKeyTable.resize(lnDistinctRows * nUnits);
//		int *lpKeys = &orvKeyTable[0];
//		int i = 0;
//		for (i = 0; i < lnDistinctRows; i++)
//		{
//			memcpy(lpKeys, &irvKeyTable[lvDistinctList[i] * (nUnits)], sizeof(int) * nUnits);
//			lpKeys += nUnits;
//		}
//		*opDistinctRows = lnDistinctRows;
//		return hCreateViewMap2(nUnits, lnDistinctRows, &orvKeyTable[0], oppColorMap);
//	}
//	else
//	{
//		*opDistinctRows = 1;
//		return hCreateViewMap2(nUnits, 1, NULL, oppColorMap);
//	}
//}
//
//int DSSRWGridModelIterator::setColorByUnits(vector<int> &irRowColorByUnits, vector<int> &irColColorByUnits)
//{
//	//[TODO], just use std::map as the lookup table to make it simple
//	{
//		int lnRows = mHeader[0].mnRows;
//		int lnUnits = mHeader[0].mnUnits;
//		if (lnUnits < irRowColorByUnits.size())
//			return E_FAIL;
//        
//		if (lnRows * lnUnits != mvRowKeyTable.size())
//			return E_FAIL;	//don't support hierarchical custom group unit
//        
//		int i = 0;
//		vector<int> lvKeys;
//		if (irRowColorByUnits.size() > 0)
//		{
//			for (i = 0; i < lnRows; i++)
//			{
//				for (int j = 0; j < irRowColorByUnits.size(); j++)
//				{
//					int offset = irRowColorByUnits[j] * mHeader[0].mnRows + i;	//TODO: [opt]
//					_ASSERT(offset < mvRowKeyTable.size());
//					int key = mvRowKeyTable[offset];
//					lvKeys.push_back(key);
//				}
//			}
//		}
//		mvRowColorByUnitOffset.assign(irRowColorByUnits.begin(), irRowColorByUnits.end());
//		int hr = hCreateColorLookupTable2(lvKeys, lnRows, irRowColorByUnits.size(), mvRowColorByKeys, &mnColorByRows, &mpRowColorMap);
//		CHECK_HR;
//	}
//	{
//		int lnCols = mHeader[1].mnRows;
//		int lnUnits = mHeader[1].mnUnits;
//		if (lnUnits < irColColorByUnits.size())
//			return E_FAIL;
//        
//		if (lnCols * lnUnits != mvColKeyTable.size())
//			return E_FAIL;	//don't support hierarchical custom group unit
//        
//		int i = 0;
//		vector<int> lvKeys;
//		if (irColColorByUnits.size() > 0)
//		{
//			for (i = 0; i < lnCols; i++)
//			{
//				for (int j = 0; j < irColColorByUnits.size(); j++)
//				{
//					int offset = irColColorByUnits[j] * mHeader[1].mnRows + i;
//					_ASSERT(offset < mvColKeyTable.size());
//					int key = mvColKeyTable[offset];
//					lvKeys.push_back(key);
//				}
//			}
//		}
//		mvColColorByUnitOffset.assign(irColColorByUnits.begin(), irColColorByUnits.end());
//		int hr = hCreateColorLookupTable2(lvKeys, lnCols, irColColorByUnits.size(), mvColColorByKeys, &mnColorByCols, &mpColColorMap);
//		CHECK_HR;
//	}
//	return S_OK;
//}
//int DSSRWGridModelIterator::getColorByCount()
//{
//	if (mpParentGridIterator)
//		return mpParentGridIterator->getColorByCount();
//	
//	return mnColorByRows * mnColorByCols;
//}
//
//int DSSRWGridModelIterator::getColorByIndex(int iRowOrdinal, int iColOrdinal)
//{
//	if (mpParentGridIterator)
//		return mpParentGridIterator->getColorByIndex(iRowOrdinal + mCBRowStartOrdinal, iColOrdinal + mCBColStartOrdinal);
//	
//	if (!mpRowColorMap || !mpColColorMap)
//		return 0;
//    
//	int lRowIndex = 0;
//	vector<int> lvKeys;
//	int lnUnits = mvRowColorByUnitOffset.size() ;
//	if (lnUnits > 0)
//	{
//		lvKeys.resize(lnUnits);
//		for (int i = 0; i < lnUnits; i++)
//		{
//			int offset = mvRowColorByUnitOffset[i] * mHeader[0].mnRows + iRowOrdinal;
//			_ASSERT(offset < mvRowKeyTable.size());
//			int key = mvRowKeyTable[offset];
//			lvKeys[i] = key;
//		}
//		if (!mpRowColorMap->LookUp(&lvKeys[0], &lRowIndex))
//			return 0;
//	}
//    
//	int lColIndex = 0;
//	lnUnits = mvColColorByUnitOffset.size() ;
//	if (lnUnits > 0)
//	{
//		lvKeys.resize(lnUnits);
//		for (int i = 0; i < lnUnits; i++)
//		{
//			int offset = mvColColorByUnitOffset[i] * mHeader[1].mnRows + iColOrdinal;
//			_ASSERT(offset < mvColKeyTable.size());
//			int key = mvColKeyTable[offset];
//			lvKeys[i] = key;
//		}
//		if (!mpColColorMap->LookUp(&lvKeys[0], &lColIndex))
//			return 0;
//	}
//    
//	return lRowIndex * mnColorByCols + lColIndex;
//}
//DSSDataModelTransaction* DSSRWGridModelIterator::getTransactionDataModel()
//{
//	DSSDataModelTransaction *lpTransactionModel = mpGridModel->getTransactionDataModel();
//	if (!lpTransactionModel)
//	{
//		DSSRWTemplateNode * lpThisNode = static_cast<DSSRWTemplateNode*> (getRWNode());
//		
//		if (lpThisNode->IsTransactionEnabled())	//transaction enabled
//		{
//			lpTransactionModel = new DSSDataModelTransaction();
//			mpGridModel->setTransactionDataModel(lpTransactionModel);
//		}
//	}
//	return lpTransactionModel;
//}
//
//int DSSRWGridModelIterator::hGetRowColumnOrdinal(DSSCellHandle &irHandle, int &orRow, int &orCol)
//{
//	if (irHandle.type == DssCellHandleXTabData)
//	{
//		int lnCols = mpXTabView->getHeader(1)->getRowCount();
//		int lOffset = irHandle.v.cell;
//		orRow = lnCols != 0 ? lOffset / lnCols : lOffset;
//		orCol = lnCols != 0 ? lOffset % lnCols : 0;
//	}
//	else if (irHandle.type == DssCellHandleXTabHeaderElement)
//	{
//		int lOrdinal = irHandle.v.ordinal;
//		int lAxis = mvUnitMapToAxis[irHandle.p.xtabUnit->getUnitIndex()];
//        
//		if (lAxis == 0)		//row axis
//		{
//			orRow = mvRowWindow[lOrdinal];
//		}
//		else
//		{
//			orCol = mvColumnWindow[lOrdinal];
//		}
//	}
//	else
//	{
//		return E_FAIL;
//	}
//	return S_OK;
//}
//
//int DSSRWGridModelIterator::UpdateData(DSSCellHandle &irHandle, CComVariant &irNewValue)
//{
//	DSSDataModelTransaction *lpTransactionModel = getTransactionDataModel();
//	if (!lpTransactionModel || mvXTabUnitFormToTransactionMap.empty())
//		return E_FAIL;
//	
//	int lnColumns = mapTrxColumn2XTabUnit.size();		//number of write back columns
//	if (lnColumns <= 0)
//		return S_OK;
//    
//	int hr = S_OK;
//	
//	//1. get the row and column offset in XTabView
//	int row = 0;
//	int col = 0;
//	hr = hGetRowColumnOrdinal(irHandle, row, col);
//	CHECK_HR;
//	
//	//2, get the raw data if it's the frist time this row is changed
//	vector<AERawData> lvRawData;
//	vector<AERawData> *lpOriginalData = NULL;
//	
//	if (DssTransactionDataUpdate != lpTransactionModel->getRecordeType(row, col))
//	{
//		//int hr = hGetRecordData(row, col, lvRawData);
//		int hr = mpXTabView->GenerateRecord(row, col, mapTrxColumn2XTabUnit, mapTrxColumn2BaseForm, lvRawData);
//		CHECK_HR;
//		
//		lpOriginalData = &lvRawData;
//	}
//    
//	//3, save the update in transaction data
//	VariantChangeType(&irNewValue, &irNewValue, 0, VT_BSTR);
//	std::string lText = MBase::WideCharToUTF8(irNewValue.bstrVal).c_str();
//    
//	if (irHandle.type == DssCellHandleXTabData)
//	{
//		int lMetricID = mpXTabView->getMetricID(row, col);
//		int lColumnUpdate = lMetricID >= 0 ? mvXTabMetricToTransactionMap[lMetricID] : -1;
//		if (lColumnUpdate >= 0)
//		{
//			hr = lpTransactionModel->AddRecord(row, col, lColumnUpdate, lText, lpOriginalData);
//			CHECK_HR;
//		}
//		
//		hr = lpTransactionModel->UpdateDataCell(row, col, lColumnUpdate, &lText);
//		CHECK_HR;
//        
//		//4, mark the calculation as dirty so that the related event will be re-calculated
//		hr = mpGridModel->getCalculationPlan()->MarkUnCalculated(mpXTabView->getMetricID(row, col));
//		CHECK_HR;
//	}
//	else if (irHandle.type == DssCellHandleXTabHeaderElement)
//	{
//		EnumDSSTemplateUnitType lUnitType = irHandle.p.xtabUnit->getUnitType();
//		AE_ASSERT(lUnitType != DssTemplateMetrics); //the header element of metrics should not be editable
//        
//		int lKey = irHandle.x.key; 		//462566, pass in tabular data unit.
//		if(lUnitType == DssTemplateConsolidation)
//		{
//			if (static_cast<DSSTabularConsolidation*> (irHandle.p.xtabUnit)->getChildElementKey(lKey) < 0)
//				return S_OK;			//474246, edit Derive Element's group node is not allowed
//		}
//        
//		
//		int lColumnUpdate = -1;
//		int lOffset = irHandle.p.xtabUnit->getUnitIndex() + mpGridModel->Count() * irHandle.formId;
//		if(mvXTabUnitFormToTransactionMap.find(lOffset)!=mvXTabUnitFormToTransactionMap.end())
//			lColumnUpdate = mvXTabUnitFormToTransactionMap[lOffset];
//        
//		int lBaseForm = 0;
//		if (lColumnUpdate >= 0)
//		{
//			hr = lpTransactionModel->AddRecord(row, col, lColumnUpdate, lText, lpOriginalData);
//			CHECK_HR;
//			
//			lBaseForm = mapTrxColumn2BaseForm[lColumnUpdate];
//		}
//        
//		hr = lpTransactionModel->UpdateHeaderCell(row, col, lColumnUpdate, irHandle.p.xtabUnit, lBaseForm, lKey, &lText);
//		CHECK_HR;
//		
//		//TODO: mark calculation plan dirty?
//	}
//	
//	return S_OK;
//}
//
//int DSSRWGridModelIterator::hGetRecordData(int row, int col, vector<AERawData> &irvOriginalData)
//{
//	int hr = S_OK;
//	
//	//1, reset row and column to the first metric
//	int lCellMetricID = mpXTabView->getMetricID(row, col);
//	row = mbHasMetricOnRow ? row - lCellMetricID : row;
//	col = mbHasMetricOnRow ? col : col - lCellMetricID;
//	
//	//2, fill index at [row, col] on XTabView
//	int lnCols = mpXTabView->getHeader(1)->getRowCount();
//	int lCellID = row * lnCols + col;
//	int lnUnits = mpGridModel->Count();
//	vector<int> lvKeys(lnUnits);
//	hr = hFillIndex(lCellID, DssAxisAll, lvKeys);
//	CHECK_HR;
//	
//	//3, get the raw data with unit keys
//	int lnColumns = mapTrxColumn2XTabUnit.size();
//	irvOriginalData.resize(lnColumns);
//	for (int i = 0; i < lnColumns; i++)
//	{
//		char *lpData = NULL;
//		int lSize = 0;
//		EnumDSSDataType lDataType = DssDataTypeReserved;
//		DSSDataFlag lFlag = DssDataOk;
//        
//		int lXTabUnitID = mapTrxColumn2XTabUnit[i];
//		if ( lXTabUnitID >= 0)
//		{
//			//it's not a metric column
//			DSSTabularUnit *lpUnit = mpGridModel->Item(lXTabUnitID);
//			CHECK_PTR(lpUnit);
//			
//			int lKey = lvKeys[lXTabUnitID];
//			int lBaseForm = mapTrxColumn2BaseForm[i];
//            
//			hr = hGetUnitValue(lpUnit, lKey, lBaseForm, (const void**)&lpData, &lSize, &lDataType, &lFlag);
//			CHECK_HR;
//		}
//		else
//		{
//			//metric
//			int lMetricID = mapTrxColumn2XTabUnit[i];
//			if (lMetricID >= 0)
//			{
//				//need adjust row and column to get all the other metric values
//				int lRowAdjusted = mbHasMetricOnRow ? row + lMetricID : row;
//				int lColAdjusted = mbHasMetricOnRow ? col : col + lMetricID;
//				
//				int lThreshold, lMetric;
//				hr = mpXTabView->GetRawCellInfo(lRowAdjusted, lColAdjusted,
//											(const char**)&lpData, lSize, lDataType, lFlag, &lThreshold, &lMetric);
//				CHECK_HR;
//				
//			}
//			else
//			{
//				lFlag = DssDataNull;
//			}
//		}
//		irvOriginalData[i].mValue = lpData;
//		irvOriginalData[i].mSizeInByte = lSize;
//		irvOriginalData[i].mDataType = lDataType;
//		irvOriginalData[i].mFlag = lFlag;
//	}
//	return S_OK;
//}
//
//int DSSRWGridModelIterator::hFindAttributeForm(DSSColumnMappingInfo *ipColumnInfo, int &orXTabUnitID, int &orBaseFormID)
//{
//	GUID lTargetUnitID = ipColumnInfo->getAttributeID();
//	orXTabUnitID = mpGridModel->FindUnit(lTargetUnitID, DssTemplateReserved);
//	
//	if (orXTabUnitID >= 0)	//found
//	{
//		orBaseFormID = ipColumnInfo->getBaseFormID();
//		return S_OK;
//	}
//	else
//	{
//		orXTabUnitID = -1;
//		orBaseFormID = -1;
//		return S_FALSE;
//	}
//}
//
//int DSSRWGridModelIterator::hMatchEditableTemplateUnit(DSSTemplateUnit*ipUnit, int iXTabUnitID)
//{
//	if (ipUnit->getType() == DssTemplateAttribute)
//	{
//		DSSTemplateAttribute* lpAttribute = static_cast<DSSTemplateAttribute*> (ipUnit);
//		DSSAttributeFormWrappers *lpForms = lpAttribute->getAttribute()->getFormWrappers();
//		int lnForms = lpForms->Count();
//		for (int i = 0; i < lnForms; i++)
//		{
//			DSSAttributeFormWrapper *lpForm = lpForms->Item(i);		//465385, 465131
//			int lColumnIndex = lpAttribute->getColumnIndexByForm(lpForm->getID());
//			lColumnIndex --;	//it's 1-based
//			
//			if (lColumnIndex >= 0 && lColumnIndex < mapTrxColumn2XTabUnit.size())
//			{
//				mapTrxColumn2XTabUnit[lColumnIndex] = iXTabUnitID;
//				mapTrxColumn2BaseForm[lColumnIndex] = lpForm->getBaseFormID();
//			}
//		}
//	}
//	else
//	{		       
//		DSSTemplateMetrics* lpTemplateMetrics = static_cast<DSSTemplateMetrics*> (ipUnit);
//		int lnMetrics = lpTemplateMetrics->Count();
//		for (int i = 0; i < lnMetrics; i++)
//		{
//			DSSTemplateMetric *lpMetric = lpTemplateMetrics->Item(i);
//			int lColumnIndex = lpMetric->getColumnIndex();
//			lColumnIndex --;	//it's 1-based
//			if (lColumnIndex >= 0 && lColumnIndex < mapTrxColumn2XTabUnit.size())
//			{
//				GUID lGUID = lpMetric->getID();
//				int lMetricID = mpGridModel->FindMetric(lGUID);
//				if (lMetricID >= 0)
//				{
//					mapTrxColumn2XTabUnit[lColumnIndex] = -1;
//					mapTrxColumn2BaseForm[lColumnIndex] = lMetricID;
//				}
//			}
//		}//end of for (int i = 0; i < lnMetrics; i++)
//	}//end of else if (ipUnit->getType() == DssTemplateMetrics)
//	return S_OK;
//}
//
//int DSSRWGridModelIterator::hBuildTransactionUnitMap(DSSRWNode * ipNode)
//{
//	if (!mapTrxColumn2XTabUnit.empty())		//map has been built
//		return S_OK;
//    
//	if (!ipNode)
//		return E_POINTER;
//	
//	int hr = S_OK;
//    
//	DSSTransactionReport *lpTargetReport = static_cast<DSSRWTemplateNode*>(ipNode)->getTransactionReport();
//	CHECK_PTR(lpTargetReport);
//	
//	DSSDataModelTransaction *lpTransactionModel = getTransactionDataModel();
//	if (!lpTransactionModel)
//		return S_OK;
//	
//	if (lpTransactionModel->getStatus() != XTAB_INITIALIZED)
//	{
//		hr = lpTransactionModel->Init(lpTargetReport, mpGridModel);
//		CHECK_HR;
//	}
//    
//	map<int, DSSColumnMappingInfo*>* lpColumnInfoMap = lpTargetReport->getColumnMap();
//	int lnTargetColumns = lpColumnInfoMap->size();
//	
//		
//	int lnMetrics = mpGridModel->Count();
//	
//	mapTrxColumn2XTabUnit.resize(lnTargetColumns);
//	mapTrxColumn2BaseForm.resize(lnTargetColumns);
//	//build the map from source template to transaction report
//	int i = 0;
//	for (i = 0; i < lnTargetColumns; i++)		//473622, init to -1 so that un-mapped column can be mapped
//	{
//		mapTrxColumn2XTabUnit[i] = -1;
//		mapTrxColumn2BaseForm[i] = -1;
//	}
//	
//	//1, use the document level mapping first:
//	int lnXTabUnits = mpGridModel->Count();
//	for (i = 0; i < lnXTabUnits; i++)
//	{
//		DSSTemplateUnit *lpUnit = mvpTemplateUnits[i];
//		if (lpUnit && 
//			(lpUnit->IsMappedForTransaction()
//             || lpUnit->getType() == DssTemplateMetrics))
//		{
//			hMatchEditableTemplateUnit(lpUnit, i);
//		}
//	}
//	
//	//2, map other columns use default rules:
//	for (i = 0; i < lnTargetColumns; i++)
//	{
//		if (mapTrxColumn2XTabUnit[i] != -1 || mapTrxColumn2BaseForm[i] != -1)										  
//			continue;	//already in document level mapping
//		
//		int lXTabUnitID = -1;
//		int lBaseFormID = -1;
//		DSSColumnMappingInfo *lpColumnInfo = (*lpColumnInfoMap)[i + 1];	//column mapping info is 1-based
//        
//		if (lpColumnInfo->getType() != DssTypeMetric)
//		{
//			hFindAttributeForm(lpColumnInfo, lXTabUnitID, lBaseFormID);
//		}
//		else if (lnMetrics > 0)
//		{
//			GUID lTargetMetricID = lpColumnInfo->getMetricID();
//			int lMetricID = mpGridModel->FindMetric(lTargetMetricID);
//			
//			if (lMetricID >= 0)
//				lBaseFormID = lMetricID;
//		}
//		mapTrxColumn2XTabUnit[i] = lXTabUnitID;
//		mapTrxColumn2BaseForm[i] = lBaseFormID;
//        
//	}
//	
//	//3, build a map from Tabular Unit ID to transaction column ID
//	for (i = 0; i < lnTargetColumns; i++)
//	{
//		int lOffset = mapTrxColumn2XTabUnit[i]
//        + mapTrxColumn2BaseForm[i]
//        * mpGridModel->Count();
//		
//		mvXTabUnitFormToTransactionMap[lOffset] = i;
//	}
//	
//	//build a map from metric ID to transaction column ID
//	mvXTabMetricToTransactionMap.resize(lnMetrics);
//	for (i = 0; i < lnMetrics; i++)
//	{
//		mvXTabMetricToTransactionMap[i] = -1;
//	}
//	for (i = 0; i < lnTargetColumns; i++)
//	{
//		if (mapTrxColumn2XTabUnit[i] < 0)
//		{
//			int lMetricID = mapTrxColumn2BaseForm[i];
//			if (lMetricID >= 0 && lMetricID < lnMetrics)
//				mvXTabMetricToTransactionMap[lMetricID] = i;
//		}
//	}
//	
//	lpTransactionModel->setMapTransactionColumnToMetric(mapTrxColumn2XTabUnit, mapTrxColumn2BaseForm);
//	
//	return S_OK;
//}
//
//int DSSRWGridModelIterator::UndoDataChange(DSSCellHandle &irHandle)
//{
//	//undo will not be supported in the current release (9.2.1)
//	/*
//     DSSDataModelTransaction *lpTransactionModel = getTransactionDataModel();
//     if (!lpTransactionModel || mvXTabUnitFormToTransactionMap.empty())
//     return E_FAIL;
//     
//     //1. get the row and column offset in XTabView
//     int row = 0;
//     int col = 0;
//     int hr = hGetRowColumnOrdinal(irHandle, row, col);
//     CHECK_HR;
//     
//     //2, check which column should undo change
//     if (irHandle.type == DssCellHandleXTabData)
//     {
//     int lMetricID = mpXTabView->getMetricID(row, col);
//     int lColumnUndo = mvXTabMetricToTransactionMap[lMetricID];
//     
//     if (lColumnUndo >= 0)
//     {
//     hr = lpTransactionModel->UndoChange(row, lColumnUndo);
//     CHECK_HR;
//     }
//     
//     hr = lpTransactionModel->UpdateDataCell(row, col, lColumnUndo, NULL);
//     CHECK_HR;
//     
//     hr = mpGridModel->getCalculationPlan()->MarkUnCalculated(mpXTabView->getMetricID(row, col));
//     CHECK_HR;
//     }
//     else if (irHandle.type == DssCellHandleXTabHeaderElement)
//     {
//     int lColumnUndo = -1;
//     int lOffset = irHandle.p.xtabUnit->getUnitIndex() + irHandle.formId * mpGridModel->Count();
//     if(mvXTabUnitFormToTransactionMap.find(lOffset)!=mvXTabUnitFormToTransactionMap.end())
//     lColumnUndo = mvXTabUnitFormToTransactionMap[lOffset];
//     int lBaseForm = 0;
//     
//     if (lColumnUndo >= 0)
//     {
//     int hr = lpTransactionModel->UndoChange(row, lColumnUndo);
//     CHECK_HR;
//     
//     lBaseForm = mapTrxColumn2BaseForm[lColumnUndo];
//     }
//     
//     int lKey = irHandle.x.key;
//     hr = lpTransactionModel->UpdateHeaderCell(row, lColumnUndo, irHandle.p.xtabUnit, lBaseForm, lKey, NULL);
//     CHECK_HR;
//     
//     //TODO: mark calculation plan as dirty
//     }	*/
//	return S_OK;
//}
//
//int DSSRWGridModelIterator::MarkRows(int iRowOrdinal, EnumDSSRWMarkRowAction iActionType)
//{
//	DSSDataModelTransaction *lpTransactionModel = getTransactionDataModel();
//	if (!lpTransactionModel)
//		return E_FAIL;
//	
//	if (iRowOrdinal >= mvRowWindow.size()) //472749 || mvColumnWindow.empty())
//		return E_INVALIDARG;
//	
//	//1. get the row and column offset in XTabView
//	int row = mvRowWindow[iRowOrdinal];
//	int col = !mvColumnWindow.empty() ? mvColumnWindow[0] : 0;
//    
//	if (DssRWMarkRowSelect == iActionType && DssTransactionDataUpdate == lpTransactionModel->getRecordeType(row, 0))
//	{
//		//already marked
//		return S_OK;
//	}
//	
//	// get the raw data
//	vector<AERawData> lvOriginalData;
//	//int hr = hGetRecordData(row, col, lvOriginalData);
//	int hr = mpXTabView->GenerateRecord(row, col, mapTrxColumn2XTabUnit, mapTrxColumn2BaseForm, lvOriginalData);
//	CHECK_HR;
//    
//	if (DssRWMarkRowSelect == iActionType)
//		return lpTransactionModel->MarkRecord(row, &lvOriginalData, DssTransactionDataInsert);
//	else if (DssRWMarkRowUnset == iActionType)
//		return lpTransactionModel->MarkRecord(row, &lvOriginalData, DssTransactionDataOriginal);
//	else
//		return E_INVALIDARG;
//}
//
//EnumDSSTransactionDataChangeType DSSRWGridModelIterator::getDataChangeType(DSSCellHandle &irHandle)
//{
//	DSSDataModelTransaction *lpTransactionModel = getTransactionDataModel();
//	if (!lpTransactionModel || mvXTabUnitFormToTransactionMap.empty())
//		return DssTransactionDataReserved;
//	
//	int row = 0;
//	int col = 0;
//	hGetRowColumnOrdinal(irHandle, row, col);
//    
//	int lColumnIndex = -1;
//	if (irHandle.type == DssCellHandleXTabData)
//	{
//		int lMetricID = mpXTabView->getMetricID(row, col);
//		if (lMetricID >= 0)		//475919: the report may not contain any metrics
//			lColumnIndex = mvXTabMetricToTransactionMap[lMetricID];
//	}
//	else if (irHandle.type == DssCellHandleXTabHeaderElement
//			 && irHandle.p.xtabUnit->getUnitType() != DssTemplateMetrics)	//468728, the header element of metrics should not be changed
//	{
//		int lOffset = irHandle.p.xtabUnit->getUnitIndex() + irHandle.formId * mpGridModel->Count();
//        
//		if(mvXTabUnitFormToTransactionMap.find(lOffset)!=mvXTabUnitFormToTransactionMap.end())
//		{
//			lColumnIndex = mvXTabUnitFormToTransactionMap[lOffset];
//		}
//	}
//	
//	if (lColumnIndex >= 0 && lpTransactionModel->IsDataUpdated(row, col, lColumnIndex))
//		return DssTransactionDataUpdate;
//	else
//		return DssTransactionDataOriginal;
//}
//
//EnumDSSTransactionDataChangeType DSSRWGridModelIterator::getRowChangeType(int iRowOrdinal)
//{
//	DSSDataModelTransaction *lpTransactionModel = getTransactionDataModel();
//	if (!lpTransactionModel || iRowOrdinal >= mvRowWindow.size())
//		return DssTransactionDataReserved;
//	
//	int row = mvRowWindow[iRowOrdinal];
//	
//	return lpTransactionModel->getRecordeType(row, 0);
//}
//
//int DSSRWGridModelIterator::getMetricCount(GUID &iMetricID)
//{
//	return mpXTabView->getMetricCount(iMetricID);
//}
//
//void DSSRWGridModelIterator::ReleaseUnusedMemory()
//{
//	hCleanUp();
//	if (mpGridModel)
//	{
//		mpGridModel->ReleaseUnusedMemory();
//	}
//	mpGridModel = NULL;
//	mpXTabView = NULL;
//	return;
//}

