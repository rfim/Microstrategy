//
//  DSSSimpleExprEvalSubQuery.cpp
//  ReportServiceCore
//
//  Created by Taimin Liao on 2/4/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#include "DSSSimpleExprEvalSubQuery.h"
#include "DSSTabularIndex.h"
#include "DSSTabularIndexPool.h"
#include "DSSSliceID.h"
#include "AEFilterExpr.h"
#include "DSSCube.h"
#include "DSSViewDataSet.h"
#include "DSSTabularData.h"
#include "DSSTabularUnit.h"
#include "DSSGUnitContainer.h"
#include "DSSCalculationHelper.h"
#include "DSSFunction.h"
#include "DSSFunctionServer.h"

#include "SimpleExpr/DFCSimpleExpr.h"
#include "SimpleExpr/DFCSimpleExprUnit.h"
#include "SimpleExpr/DFCSimpleExprUnitList.h"

#include "SimpleExpr/DFCSimpleExprBranch.h"

#include <algorithm>

//typedef map<Int32, Int32, less<Int32>, MBase::Allocator<pair<Int32 const, Int32> > > IntToIntMapType;
typedef std::map<int, int, less<int> > IntToIntMapType;
DSSSimpleExprEvalSubQuery::DSSSimpleExprEvalSubQuery(DSSGUnitContainer* ipContainer, 
                                                     DSSFunctionServer* ipFunction, 
                                                     DSSObjectContext* ipSource, 
                                                     DSSCube* ipSourceCube, 
                                                     DSSCube* ipTargetCube) : mpGUnitContainer(ipContainer),mpTargetCube(ipTargetCube), mpSourceCube(ipSourceCube), mpFuncServer(ipFunction), mpSource(ipSource)
{
}

DSSSimpleExprEvalSubQuery::~DSSSimpleExprEvalSubQuery()
{
}

int DSSSimpleExprEvalSubQuery::EvalExpr(DFCSimpleExpr *ipExpr, DSSSliceID * opResultID, AEFilterExprNode **oppNode)
{
	if (!ipExpr || !opResultID)
		return E_INVALIDARG;
    
	if (ipExpr->GetExprType() != DFCSimpleExpr::SIMPLE_EXPR_BRANCH)
		return E_INVALIDARG;
    
	DFCSimpleExprBranch* lpBranchExpr = static_cast<DFCSimpleExprBranch *>(ipExpr);
	if (lpBranchExpr->GetBranchType() != BRANCH_SUBQUERY)
		return E_FAIL;
    
	return hCalculateExpr(ipExpr, opResultID, oppNode);
}

int DSSSimpleExprEvalSubQuery::hEvalExpr(DFCSimpleExpr *ipExpr, DSSTabularIndex** oppIndices, DSSSliceID* opResultID, DSSTabularData** oppTabularData)
{
	AE_ASSERT(ipExpr);
    
	HRESULT hr = S_OK;
    //MCEExecutor is not ready
    
	switch(ipExpr->GetExprType())
	{
            /*
        case DFCSimpleExpr::SIMPLE_EXPR_DATASET_ID:
		{
			DFCSimpleExprDatasetID* lpDataSetID = static_cast<DFCSimpleExprDatasetID*>(ipExpr);
			DFCSimpleExprDatasetID::DATASET_TYPE lType;
			Int32 lDataSetID;
			hr = lpDataSetID->GetDatasetID(lType, lDataSetID);
			CHECK_HR;
            
			//AE_ASSERT(mpMCEDataContainer);
            
			if (lType == DFCSimpleExprDatasetID::BASE_REPORT_INSTANCE)
			{
				CComPtr<ICDSSReportInstance10> lpBaseReportInstance;
				hr = mpMCEDataContainer->GetBaseRIbyID(lDataSetID, &lpBaseReportInstance);
				CHECK_HR_PTR(lpBaseReportInstance);
                
				CComPtr<ICDSSTemplate> lpBaseTemplate;
				hr = lpBaseReportInstance->get_SourceTemplate(&lpBaseTemplate);
				CHECK_HR_PTR(lpBaseTemplate);
                
				CComPtr<ICDSSDataSet> lpBaseDataSet;
				hr = lpBaseReportInstance->get_DataSet(&lpBaseDataSet);
				CHECK_HR_PTR(lpBaseDataSet);
                
				CComPtr<ICDSSViewDataSet> lpBaseViewDataSet;
				hr = CDSSCalculationHelper::GetViewDataSet(lpBaseDataSet, lpBaseTemplate, &lpBaseViewDataSet);
				CHECK_HR_PTR(lpBaseViewDataSet);
                
				CComPtr<ICDSSCube> lpBaseCube;
				hr = lpBaseViewDataSet->get_Cube(&lpBaseCube);
				CHECK_HR_PTR(lpBaseCube);
                
				CComPtr<ICDSSTabularData> lpBaseTabularData;
				hr = lpBaseCube->get_TabularData(&lpBaseTabularData);
				CHECK_HR_PTR(lpBaseTabularData);
                
				if (!oppTabularData)
					return E_FAIL;
                
				*oppTabularData = static_cast<CDSSTabularData*>(lpBaseTabularData.p);
			}
			else if (lType == DFCSimpleExprDatasetID::TEMP_CUBE)
			{
				CComPtr<ICDSSCube> lpBaseCube;
				hr = mpMCEDataContainer->GetTempCubebyID(lDataSetID, &lpBaseCube);
				CHECK_HR_PTR(lpBaseCube);
                
				CComPtr<ICDSSTabularData> lpBaseTabularData;
				hr = lpBaseCube->get_TabularData(&lpBaseTabularData);
				CHECK_HR_PTR(lpBaseTabularData);
                
				if (!oppTabularData)
					return E_FAIL;
                
				*oppTabularData = static_cast<CDSSTabularData*>(lpBaseTabularData.p);
			}
			else
			{
				_ASSERT(false);
				return E_NOTIMPL;
			}
		}
            break;*/
        case DFCSimpleExpr::SIMPLE_EXPR_UNIT_LIST:
		{
			DFCSimpleExprUnitList* lpUnitListExpr = static_cast<DFCSimpleExprUnitList*>(ipExpr);
			std::vector<DSS_ID> lvUnitID;
			lpUnitListExpr->GetAttrIDs(lvUnitID);
            
			//gen index from mpcube
			DSSTabularData *lpTabularData = mpSourceCube->getTabularData();
			CHECK_HR;
            
			std::vector<Int32> lvUnit(lvUnitID.size());
            
			for (Int32 iUnit = 0; iUnit < lvUnitID.size(); iUnit++)
			{
				hr = lpTabularData->FindUnitI(lvUnitID[iUnit], DssTemplateReserved, lvUnit[iUnit]);
				CHECK_HR;
			}
            
			sort(lvUnit.begin(), lvUnit.end());
            
			DSSTabularIndexPool* lpIndexPool = lpTabularData->getIndexPool();
			CHECK_PTR(lpIndexPool);
            
			if (!oppIndices)
				return E_FAIL;
            
			hr = lpIndexPool->getIndex(lvUnit.size(), lvUnit.size() > 0 ? &lvUnit[0] : NULL, oppIndices);
			CHECK_HR;
		}
            break;
        case DFCSimpleExpr::SIMPLE_EXPR_BRANCH:
		{
            _ASSERT(false);
            return E_NOTIMPL;
		}
            break;
        case DFCSimpleExpr::SIMPLE_EXPR_POS:
		{
		}
            break;
        case DFCSimpleExpr::SIMPLE_EXPR_CONST:
		{
		}
            break;
        case DFCSimpleExpr::SIMPLE_EXPR_UNIT:
		{
		}
            break;
        case DFCSimpleExpr::SIMPLE_EXPR_RELATION:
		{
		}
            break;
        default:
            _ASSERTE(false);
            return hr = E_FAIL;
	}
    
	return S_OK;
}
//ydong
#import <SimpleBuffer.h>

int DSSSimpleExprEvalSubQuery::hCalculateExpr(DFCSimpleExpr *ipExpr, DSSSliceID * opResultID, AEFilterExprNode **oppNode)
{
	HRESULT hr = S_OK;
    
	DFCSimpleExprBranch* lpBranch = static_cast<DFCSimpleExprBranch *>(ipExpr);
	
    
	DFCFunc * lpFunc = lpBranch->GetFunc();
	Int32 i = 0;
    
	// qjiang 11/15/2010
	// re-organize the following code
	DSS_ID lFuncID = lpFunc -> GetGUID();
	if (lFuncID == GUID_NULL)
	{
		_ASSERT(false);
		return E_NOTIMPL;
	}
    
	// qjiang 11/12/2010
	// get the function object before preparing the slices, as some function property could affect the slices    
    DSSObjectInfo *lpInfo = mpSource->FindObjectH(lFuncID);
    CHECK_HR_PTR(lpInfo);
    
    DSSFunction *lpFunction = dynamic_cast<DSSFunction*>(lpInfo);
    CHECK_HR_PTR(lpFunction);
    
	EnumDSSFunction lFuncTypeNS = lpFunction->getFunctionType();
    GUID lPackageGUID = lpFunction->getPackageID();
    DSSFunctionObject *lpFuncObj = NULL;
    lpFuncObj = mpFuncServer->FindFunctionObject(lpFunction->getFunctionIndex(), &lPackageGUID);
    
	//here should recursive call its children
	vector<DFCSimpleExpr *, MBase::Allocator<DFCSimpleExpr *> > lChildren(lpBranch -> GetArgs());
    
	Int32 lnChild = lChildren.size();
	
    MBase::SimpleBuffer lSimpleBuffer;
	MBase::BufferPtr lBufferPtr(&lSimpleBuffer);
	MBase::Buffer *lpBuffer = lBufferPtr.Get();
	CHECK_NEWED_PTR(lpBuffer);
    
	//for the sub-query CSI, the last child should be a temp cube
	//the 0 t0 n-1 child may be a indices, maybe a slice
	DSSTabularIndex** lpIndices = new (lpBuffer) DSSTabularIndex*[lnChild - 1];
	CHECK_NEWED_PTR(lpIndices);
    
	DSSSliceID * lpTempSliceID = new (lpBuffer) DSSSliceID[lnChild - 1];
	CHECK_NEWED_PTR(lpTempSliceID);
    
	memset(lpIndices, 0, sizeof(DSSTabularIndex*) * (lnChild - 1));
	memset(lpTempSliceID, 0, sizeof(DSSSliceID) * (lnChild - 1));
    
	DSSTabularData* lpTargetTabularData = NULL;
    
	DFCSimpleExprDatasetID *lpDataSetID  = lpBranch->GetRelationID();
	CHECK_HR;
    
	if (lpDataSetID)
	{
		_ASSERT(false);
		return E_NOTIMPL;
	}
    
	for(i = 0; i < lnChild; i++)
	{
		hr = hEvalExpr(lChildren[i], &lpIndices[i], &lpTempSliceID[i], &lpTargetTabularData);
		CHECK_HR;
	}
    
	AE_ASSERT(lpTargetTabularData);
    
	//calculate the function
	if (lFuncTypeNS != DssFunctionIn && lFuncTypeNS != DssFunctionNotIn && lFuncTypeNS != DssFunctionEquals && lFuncTypeNS != DssFunctionNotEqual)
	{	//now we only support the In
		_ASSERTE(false);
		return E_NOTIMPL;	
	}
    
	if (!mpGUnitContainer)
	{
		//now we only support this kind of CSI in new rwd
		_ASSERTE(false);
		return E_NOTIMPL;
	}
    
	bool lfReverse = lFuncTypeNS == DssFunctionNotIn || lFuncTypeNS == DssFunctionNotEqual;
    
	//create result slice
	DSSCubeDataSlice* lpResultSlice;
    
	Int32 lnUnits = lpIndices[0]->getNumUnits();
	CHECK_HR;
    
	DSSTabularIndex* lpIndex = lpIndices[0];
	AE_ASSERT(lpIndex);//the first input should be a index
    
	vector<Int32> lvUnits(lnUnits);
	hr = lpIndex->getUnits(lnUnits, &lvUnits[0]);
	CHECK_HR;
    
	Int32 lnTotalUnits = 0;
	DSSTabularData* lpTabularData = mpSourceCube->getTabularData();
	CHECK_PTR(lpTabularData);
    
	hr = lpTabularData->Count(&lnTotalUnits);
	CHECK_HR;
    
	hr = mpTargetCube->CreateDataSlice(lnUnits, &lvUnits[0], DssDataTypeBool, &lpResultSlice);
	CHECK_HR;
	
	DSSDataColumn* lpResultColumn = lpResultSlice->getColumn();
    
	Int32 lnRows = lpResultColumn->getRowCount();
    
	for (i = 0; i < lnRows; i++)
	{
		bool lVal = false;
		hr = lpResultColumn->setData(i, (char*)(&lVal), sizeof(bool), DssDataOk);
		CHECK_HR;
	}
    
	bool lfCheckUnQualify = false;
	for (Int32 i = 0; i < lnUnits; i++)
	{
		DSSTabularUnit* lpUnit = lpTabularData->Item(lvUnits[i]);
		CHECK_HR_PTR(lpUnit);
        
		EnumDSSTemplateUnitType lType = lpUnit->getUnitType();
		CHECK_HR;
        
		if (lType == DssTemplateConsolidation)
		{
			lfCheckUnQualify = true;//!(static_cast<DSSTabularConsolidation*>(lpUnit))->NeedUpdateDEIndex();
		}
	}
    
	//for in-list, there should be only two input
	//for each child, try to calculate the result
	for (i = 0; i < lnChild - 1; i++)
	{
		if(lpIndices[i])
		{
			//get the corresponding index in target tabulardata
			Int32 lnUnits = lpIndices[i]->getNumUnits();
			CHECK_HR;
            
			vector<Int32> lvUnits(lnUnits);
			hr = lpIndices[i]->getUnits(lnUnits, &lvUnits[0]);
			CHECK_HR;
            
			//prepare the maps of the elements
			Int32* lpLToGMap[lnUnits];
			Int32* lGToLMap[lnUnits];
			IntToIntMapType* lGToLMap2[lnUnits];
			memset(lGToLMap, 0, sizeof(Int32*) * lnUnits);
			memset(lGToLMap2, 0, sizeof(IntToIntMapType*) * lnUnits);
			memset(lpLToGMap, 0, sizeof(Int32*) * lnUnits);
            
			std::vector<GUID> lvUnitIDs(lnUnits);
			for (Int32 j = 0; j < lnUnits; j++)
			{
				DSSTabularUnit* lpUnit = lpTabularData->Item(lvUnits[j]);
				CHECK_HR_PTR(lpUnit);
                
				hr = lpUnit->getGUID(lvUnitIDs[j]);
				CHECK_HR;
                
                vector<int>* lpMap = mpGUnitContainer->getLocalToGlobalMap(lpTabularData->getDataSetGUID(),lpUnit);
                if (lpMap != NULL && lpMap->size()>0 )
                {
                    lpLToGMap[j] = &(*lpMap)[0];
                }
				if (FAILED(hr))
					return hr;
			}
            
			std::vector<Int32> lvUnitsInTarget(lnUnits);
			for (Int32 j = 0; j < lnUnits; j++)
			{
				hr = lpTargetTabularData->FindUnitI(lvUnitIDs[j], DssTemplateReserved, lvUnitsInTarget[j]);
				CHECK_HR;
                
				DSSTabularUnit* lpUnit;
				hr = lpTargetTabularData->Item(lvUnitsInTarget[j], &lpUnit);
				CHECK_HR_PTR(lpUnit);
                
                vector<int>* lpMap = mpGUnitContainer->getGlobalToLocalMap(lpTargetTabularData->getDataSetGUID(),lpUnit);
                if (lpMap != NULL && lpMap->size()>0 )
                {
                    lpLToGMap[j] = &(*lpMap)[0];
                }
				if (FAILED(hr))
					return hr;
			}
            
			std::vector<Int32> lvTemp;
			lvTemp.assign(lvUnitsInTarget.begin(), lvUnitsInTarget.end());
            
			sort(lvUnitsInTarget.begin(), lvUnitsInTarget.end());
			
			Int32* lpUnitMap = new (lpBuffer) Int32[lnUnits];
			for (Int32 j = 0; j < lnUnits; j++)
			{
				for (Int32 k = 0; k < lnUnits; k++)
					if (lvTemp[j] == lvUnitsInTarget[k])
					{
						lpUnitMap[j] = k;
						break;
					}
			}
            
			DSSTabularIndex* lpTargetIndices = NULL;
			
			DSSTabularIndexPool* lpTargetIndexPool = lpTargetTabularData->getIndexPool();
			CHECK_PTR(lpTargetIndexPool);
            
			hr = lpTargetIndexPool->getIndex(lvUnitsInTarget.size(), lvUnitsInTarget.size() > 0 ? &lvUnitsInTarget[0] : NULL, &lpTargetIndices);
			CHECK_HR_PTR(lpTargetIndices);
            
			Int32 lnRowInTarget = lpTargetIndices->Count();
            
			//currently for new rwd, we can compare the element key based on global lookup table
			Int32 lnRow = lpIndices[i]->Count();
            
			//for current function, the row count of result slice should be the same as the input slice
			AE_ASSERT(lnRow == lnRows);
			//prepare the key for target tabular data
			Int32 lpKey[lnUnits];
			DSSXTabKeyIterator lIt(lpIndices[i]);
			for (Int32 iRow = 0; iRow < lnRow; iRow++)
			{
				lIt.SetRow(iRow);
				for (Int32 iColumn = 0; iColumn < lnUnits; iColumn++)
				{
					Int32 lKey = lIt.GetCol(iColumn);
					Int32 lGlobalKey = lpLToGMap[iColumn] ? lpLToGMap[iColumn][lKey] : lKey;
					Int32 lTargetKey;
					if (lGToLMap[iColumn])
						lTargetKey = lGToLMap[iColumn][lGlobalKey];
					else if (lGToLMap2[iColumn])
					{
						IntToIntMapType::iterator lIt = lGToLMap2[iColumn]->find(lGlobalKey);
						if (lIt !=  lGToLMap2[iColumn]->end())
							lTargetKey = lIt->second;
						else
							return E_FAIL;//something must be wrong
					}
					else
						lTargetKey = lGlobalKey;
					lpKey[lpUnitMap[iColumn]] = lTargetKey;
				}
                
				//lookup in target
				Int32 lRow;
				hr = lpTargetIndices->LookupActualKey(lpKey, &lRow);
				if (FAILED(hr))
                    return hr;
				//ctang: 695189
				bool lVal = (hr == S_OK && lRow < lnRowInTarget);
				if (lVal && lfCheckUnQualify)
					lVal =  !lpTargetIndices->IsUnqualified(lRow);
				if (lfReverse)
					lVal = !lVal;
				hr = lpResultColumn->setData(iRow, (char*)&lVal, sizeof(bool), DssDataOk);
				CHECK_HR;
			}
            
		}
		else
		{
			_ASSERTE(false);
			return E_NOTIMPL;
		}
	}
    
	hr = mpTargetCube->InsertDataSlice(lpResultSlice, opResultID->mSliceID);
	CHECK_HR;
    
	opResultID->mpCube = mpTargetCube;
    
	return S_OK;
}



