/*
 *  DSSSliceCalculator.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 1/25/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#include "DSSSliceCalculator.h"
#import "DSSCubeDataSlice.h"
#import "DSSTabularIndex.h"
#import "DSSCube.h"
#import "DSSDataColumn.h"
#import "DSSTabularUnit.h"
#import "DSSFunctionServer.h"
#import "DSSFunctionObject.h"
#import "DSSSimpleFunction.h"
#import "DSSNNSimpleFunction.h"
#import "DSSAggregationFunction.h"
#import "DSSTabularIndexPool.h"
#import "DSSBuiltInPackage.h"
#import "DSSGenericFunction.h"
#import "DSSNNGenericFunction.h"
#import "DSSStrongPtr.h"
#import "DSSTabularIndexPool.h"
#import "DSSDataModelXTabImpl.h"
#import "DSSTabularUnit.h"
#import "DSSXTabLayoutUnitInfo.h"
#import "DFCDataConversion.h"
#import "DSSTabularData.h"
#import <algorithm>
#import "CalculationEngine/DSSNNSimpleFunction.h"
#import "CalculationEngine/DSSNNAggregationFunction.h"
#import "CalculationEngine/DSSGenericFunction.h"
#import "CalculationEngine/DSSNNGenericFunction.h"
#import "DFCDataConversion.h"
#import "DSSSortDefinition.h"
#import "DSSSorter.h"
#import "DSSSort.h"
#import "DSSXTabView.h"
#import "DSSTabularMetrics.h"
#import "DSSSorts.h"
#import "DSSSortConverter.h"
#import "DSSXTabHeader.h"
#import "DSSTemplate.h"
#import "DSSStrongPtr.h"
#import "DSSMetricSorter.h"
#import "DSSViewDataSet.h"
#import "DSSTabularData.h"
#import "DSSSliceCorrelator.h"
#import "DSSCellFmtData.h"
#import "DSSAggregateGeneralFunc.h"
#import "DSSMaskInfos.h"
#import "Base/Base/MultiByteConverter.h"
#import "Base/Base/UTFConverter.h"
#import "CDSSDoSort.h"
#import "CDSSBucketSort.h"
#import "DSSSimpleOperatorFunc.h"
#import <SimpleBuffer.h>

DSSSliceCalculator::DSSSliceCalculator(CalculationContext *ipContext): mpContext(ipContext),
mLevelResolver(ipContext->mpCube, ipContext->mpSchema), mpSortIndex(NULL),mbOldRWD(0),mbDSTrx(ipContext->mbDSTrx)
{
}

DSSSliceCalculator::~DSSSliceCalculator()
{
    //tliao, 912747, 2014/08/04, fix memory leak
    if (mpSortIndex)
    {
        delete [] mpSortIndex;
        mpSortIndex = NULL;
    }
}

int DSSSliceCalculator::hLoadInputDataSlices()
{
	int hr = S_OK;
    
	int lnSlices = mpContext->mvSliceID.size();
    
	mvSlice.resize(lnSlices);
	
	for (int i = 0; i < lnSlices; i++)
	{
		DSSCubeDataSlice *lpSlice = NULL;
        hr = mpContext->mvSliceID[i]->GetSlice(&lpSlice);
        CHECK_HR_PTR(lpSlice);
		
		mvSlice[i] = lpSlice;
	}

	return S_OK;
}

int DSSSliceCalculator::CalculateNewSlice(int *opResultSliceID)
{
    // comment out checking of input slice count, since some functions has no input slice, e.g. CurrentDate()
	if (!mpContext || /*mvSliceIDIn.empty() ||*/ !opResultSliceID)
		return E_INVALIDARG;
    
	int hr = S_OK;
	
	hr = hLoadInputDataSlices();
	CHECK_HR;
	
    if(mLevelResolver.getMCESchema())
    {
        hr = mLevelResolver.Init();
        CHECK_HR;
	}
    //	*opResultSliceID = NULL;
    
    if (!mpContext->mpFuncObject)
    {
        //prevent memory leak
        mpContext->mpFuncObject = DSSBuiltInPackage::getFunctionByIndex(mpContext->mFuncIndex);
    }
	CHECK_PTR(mpContext->mpFuncObject);
	
	EnumDSSFunctionType lFuncType = mpContext->mFuncType = mpContext->mpFuncObject->getFunctionType();
	
	if (lFuncType == DssFunctionTypeAggregation)
//		AE_ASSERT(mvSlice.size() == 1);
	
	//need to sort the break-by units
	std::sort(mpContext->mvBreakbyUnits.begin(), mpContext->mvBreakbyUnits.end());
	
	if (mpContext->mNullCheckingOption == DssNumericErrorCalcOnly)
	{
		if (lFuncType == DssFunctionTypeAggregation)
			mpContext->mNullCheckingOption = DssNumericErrorSkip;
		else
			mpContext->mNullCheckingOption = DssNumericErrorTreatAs;
	}
	
	switch(lFuncType)
	{
		case DssFunctionTypeSimple:
		{
			hr = hApplySimpleFunc(opResultSliceID);
		}
			break;
		case DssFunctionTypeGeneric:
		{
			hr = hApplyGenericFunc(opResultSliceID);
		}
			break;
		case DssFunctionTypeAggregation:
		{
			hr = hApplyAggregationFunc(opResultSliceID);
		}
			break;
		default:
			AE_ASSERT(false);
	}
	CHECK_HR;
		
	return S_OK;
}

int MaxRowCount(vector<DSSCubeDataSlice*> &irvDataSlices)
{
	int max = -1;
	for (int i = 0; i < irvDataSlices.size(); i++)
	{
		DSSDataColumn *lpColumn = irvDataSlices[i]->getColumn();
		int lnRows = 1;
		
		if (lpColumn)	//it may ba a constant slice
			lnRows = lpColumn->getRowCount();
		if (max < lnRows)
			max = lnRows;
	}
	return max;
}

DSSTabularIndex* MaxRowIndex(vector<DSSCubeDataSlice*> &irvDataSlices)
{
	int max = -1;
    DSSTabularIndex *lpIndex = NULL;
	for (int i = 0; i < irvDataSlices.size(); i++)
	{
		DSSDataColumn *lpColumn = irvDataSlices[i]->getColumn();
		int lnRows = 1;
		
		if (lpColumn)	//it may ba a constant slice
			lnRows = lpColumn->getRowCount();
		if (max < lnRows)
        {
			max = lnRows;
            lpIndex = irvDataSlices[i]->getIndices();
        }
	}
	return lpIndex;
}

int DSSSliceCalculator::hApplySimpleFunc(int *opResultSliceID)
{
	int hr = S_OK;
    
    hr = hProcessConditionalMetric(mvSlice.size(), &mvSlice[0]);
	CHECK_HR;
	
	vector<int > lvUnionLevel;
	hr = DSSCalculationHelper::GetUnionLevel(mvSlice, lvUnionLevel);
	CHECK_HR;
	
    Int32 lnUnionUnitSpace	= lvUnionLevel.size();
	Int32 *lpUnionUnitSpace	= (lnUnionUnitSpace>0)?&lvUnionLevel[0]:NULL;
    
	vector<int> lvTrueLevel;

    if(mLevelResolver.getMCESchema() != NULL)
    {
        hr = mLevelResolver.ResolveTrueLevel(lvUnionLevel, lvTrueLevel);
        CHECK_HR;
    }else{
        //[conditional logic]] will enter this "else" when trx condition evaluation for initial slice
        Int32 lnRow = 0;
        for(Int32 i = 0; i < mvSlice.size(); i++)
        {
            DSSTabularIndex* lpIndex = mvSlice[0]->getIndices();
            if(lpIndex)
            {
                if(lnRow == 0)
                {
                    lnRow = lpIndex->Count();
                }else if(lnRow != lpIndex->Count())
                {
                    lnRow = -1;
                    break;
                }
                    
            }
        }
        if(lnRow > 1)
        {
            //conditional logic
            lvTrueLevel = lvUnionLevel;
        }
        
    }
    
    Int32 lnTrueLevel	= lvTrueLevel.size();
	Int32 *lpTrueLevel	= (lnTrueLevel>0)?&lvTrueLevel[0]:NULL;
	
    // Check if there is any missing input slice.
    // If there is any, propogate it to the output slice
    // 814074, fali
    bool lFoundMissing = false;
    DSSCubeDataSlice *lpMissingOutputSlice = NULL;
    hr = hPropogateMissing(mvSlice.size(), &mvSlice[0], lnTrueLevel, lpTrueLevel, &lpMissingOutputSlice, lFoundMissing);
    CHECK_HR;
    
    if (lFoundMissing)
    {
        mpContext->mpCube->InsertDataSlice(lpMissingOutputSlice, *opResultSliceID);
        return hr = S_OK;
    }
    
    DSSTabularIndex* lpLookupIndex = NULL;
    DSSTabularIndex *lpOutputIndex = NULL;
	
    DSSTabularIndex lIndex1;//fake index for 931 compatiblity
    if(!mLevelResolver.getMCESchema() && lnTrueLevel == 0)//931 i-server
    {
        mbOldRWD = true;
        if (0 == mpContext->mvSliceID.size())
        {
            lIndex1.Init(0, 1, 0, 0, NULL);
            lpLookupIndex = &lIndex1;
            
            lpOutputIndex = new DSSTabularIndex();
            CHECK_NEWED_PTR(lpOutputIndex);
            lpOutputIndex->Init(0, 1, 0, 0, NULL);
        }
        else
        {
            DSSTabularIndexPool *lpIndexPool = NULL;
            hr = mpContext->mpCube->getIndexPool(&lpIndexPool);
            CHECK_HR;
            
            DSSTabularIndex *lpMainIndex = NULL;
            hr = lpIndexPool->getMainIndex(&lpMainIndex);
            CHECK_HR;
            
            if (NULL == lpMainIndex) //in old RWD, main index of index pool is NULL
            {
                int lnMaxRowCount = 0;
                for(int i = 0; i < mpContext->mvSliceID.size(); i++)
                {
                    DSSCubeDataSlice* lpSlice = NULL;
                    if(mpContext->mvSliceID[i])
                    {
                        hr = mpContext->mvSliceID[i]->GetSlice(&lpSlice);
                        if(lpSlice)
                        {
                            DSSDataColumn* lpColumn = lpSlice->getColumn();
                            if(lpColumn)
                            {
                                if(lnMaxRowCount < lpColumn->getRowCount())
                                {
                                    lnMaxRowCount = lpColumn->getRowCount();
                                }
                            }
                        }
                    }
                }
                lIndex1.Init(0, lnMaxRowCount, 0, 0, NULL);
                lpLookupIndex = &lIndex1;
   
                // Jun/19/2013, TQMS 759322, in old RWD, "mpIndex" of cube slice is not used and is populated to NULL, while size of input slice is always greater than 0. This if-block is to handle this case.
                lpOutputIndex = new DSSTabularIndex();
                lpOutputIndex->Init(0, lIndex1.Count(), 0, 0, NULL);
            }
        }
    }
    
    if(!lpLookupIndex)
    {
        hr = mpContext->mpCube->getIndex(lnUnionUnitSpace, lpUnionUnitSpace, &lpLookupIndex);
        CHECK_HR_PTR(lpLookupIndex);
    }
    
    if(!lpOutputIndex)
    {
        hr = mpContext->mpCube->getIndex(lnTrueLevel, lpTrueLevel, &lpOutputIndex);
        CHECK_HR_PTR(lpOutputIndex);
    }
    // jxie, 2012-7-31, TQMS 628857, merge Linjun's check-in to new rwd
	// lweng 2011-07-10 TQMS475241
	// if the output index which is at TrueLevel is incompatible with lookup index,
	// use lookup index as output index.
    
	Int32 lOutputIndexRowCount = lpOutputIndex->Count();
	Int32 lLookupIndexRowCount = lpLookupIndex->Count();
    
	if(lOutputIndexRowCount != lLookupIndexRowCount)
	{
		lnTrueLevel = lnUnionUnitSpace;
		lpTrueLevel = lpUnionUnitSpace;
	}
	
	bool bAllNumerical = true;
    //tliao, 822148, 2014/09/23, support cell format data calculation
    bool lbHasCellFormat = false;
    if (mvSlice.size())
    {
        for (int  i = 0; i < mvSlice.size(); i ++)
        {
            bool bNumericalSlice;
            bool bCellFormatSlice;
            hr = hIsNumericalSlice(mvSlice[i], bNumericalSlice, bCellFormatSlice);
            CHECK_HR;
            
            if (!bNumericalSlice)
                bAllNumerical = false;
            if (bCellFormatSlice)
                lbHasCellFormat = true;
        }
    }
    else
    {
        bAllNumerical = false;
    }
    //tliao, 822148, 2014/09/23, support cell format data calculation
    bool lbSupportCellFormat = mpContext->mpFuncObject->SupportCellFormat();
    if (lbSupportCellFormat && lbHasCellFormat)
    {
        hr = hApplyNonNumericalSimpleFunction(NULL, lpLookupIndex, lpOutputIndex, opResultSliceID, mpContext->mpFuncObject);
        CHECK_HR;
        
        return S_OK;
    }
    
    DSSSimpleOperatorFunc* lpSimpleOperatorFunc = NULL;
    lpSimpleOperatorFunc = dynamic_cast<DSSSimpleOperatorFunc*>(mpContext->mpFuncObject);

    
	// apply numerical function if the data are all numerical
    DSSNNSimpleFunction *lpNNSimpleFunction = NULL;
    lpNNSimpleFunction = dynamic_cast<DSSNNSimpleFunction *>(mpContext->mpFuncObject);

    bool bIsNNFunctionOnly = lpNNSimpleFunction != NULL ? lpNNSimpleFunction->IsNNFunctionOnly() : false;
    //fali, 870518, follow server side logic, only if it's lpNNSimpleFunction but not lpSimpleOperatorFunc, check big decimal
    if (lpNNSimpleFunction != NULL && lpSimpleOperatorFunc == NULL)
    {
        for (int  i = 0; i < mvSlice.size(); i ++)
        {
            DSSDataType_Type inputType = mvSlice[i]->getDataType();
            if (inputType == DssDataTypeBigDecimal)
            {
                bAllNumerical = false;
                break;
            }
        }
    }
	if (bAllNumerical && !bIsNNFunctionOnly)
	{
		DSSSimpleFunction *lpSimpleFunction = dynamic_cast<DSSSimpleFunction*> (mpContext->mpFuncObject);
		CHECK_PTR(lpSimpleFunction);
		
		hr = hApplyNumericalSimpleFunction(lpSimpleFunction, lpLookupIndex,
                                           lpOutputIndex, opResultSliceID);
		CHECK_HR;
	}
	else
	{
		hr = hApplyNonNumericalSimpleFunction(lpNNSimpleFunction, lpLookupIndex,
                                              lpOutputIndex, opResultSliceID);
		CHECK_HR;
	}
    
	return S_OK;
}

int DSSSliceCalculator::hApplyAggregationFunc(int *opResultSliceID)
{
	int hr = S_OK;
	
    DSSTabularIndex *lpIndex = NULL;
    DSSCubeDataSlice *lpSliceIn = mvSlice[0];
    // qjiang
	// if the function is count, check its type
	CountType lCountType = eCountUnknown;
	hr = hGetCountType(lCountType);
	CHECK_HR;
    
	// for count(*), use the main index to calculate
	if (lCountType == eCountStar)
	{
		DSSTabularData *lpTabularData = mpContext->mpCube->getTabularData();
		CHECK_PTR(lpTabularData);
        
		DSSTabularIndexPool *lpIndexPool = lpTabularData->getIndexPool();
        CHECK_PTR(lpIndexPool);
    
		hr = lpIndexPool->getMainIndex(&lpIndex);
		CHECK_HR_PTR(lpIndex);
	}
	else
	{
		lpIndex = lpSliceIn->getIndices();
        if(!lpIndex)//931 i-server, compatiblity
        {
            DSSTabularIndex lIndex1;
            
            int lnMaxRowCount = 0;
            for(int i = 0; i < mpContext->mvSliceID.size(); i++)
            {
                DSSCubeDataSlice* lpSlice = NULL;
                if(mpContext->mvSliceID[i])
                {
                    hr = mpContext->mvSliceID[i]->GetSlice(&lpSlice);
                    if(lpSlice)
                    {
                        DSSDataColumn* lpColumn = lpSlice->getColumn();
                        if(lpColumn)
                        {
                            if(lnMaxRowCount < lpColumn->getRowCount())
                            {
                                lnMaxRowCount = lpColumn->getRowCount();
                            }
                        }
                    }
                }
            }

            lIndex1.Init(0, lnMaxRowCount, 0, 0, NULL);
            
            DSSTabularIndex *lpLookupIndex = &lIndex1;
            
            DSSTabularIndexPool *lpIndexPool = NULL;
            mpContext->mpCube->getIndexPool(&lpIndexPool);
            CHECK_PTR(lpIndexPool);
            
            DSSTabularIndex *lpOutputIndex = NULL;
            if (lpIndexPool->getMainIndex() != NULL)
            {
                hr = lpIndexPool->getIndex(0, NULL, &lpOutputIndex);
                CHECK_PTR(lpOutputIndex);
            }
            else
            {
                //this is for old rwd engine
                lpOutputIndex = lpIndexPool->getSpecialIndex();
                CHECK_PTR(lpOutputIndex);
            }
            
            DSSAggregationFunction *lpAggFunction = static_cast<DSSAggregationFunction*> (mpContext->mpFuncObject);
            CHECK_PTR(lpAggFunction);
            
            hr = hApplyNumericalAggregation(lpAggFunction, lpLookupIndex,
                                            lpOutputIndex, mpContext->mNullCheckingOption, opResultSliceID, lCountType);
            CHECK_HR;
            
            return S_OK;
        }
	}
    
    Int32 lnUnit = lpIndex->getNumUnits();
    Int32 *lpUnit = new Int32[lnUnit];
    CHECK_NEWED_PTR(lpUnit);
    MBase::DSSStrongPtr<Int32, MBase::DeleteArray<Int32> > lModelPtr(lpUnit);
    
    hr = lpIndex->getUnits(lnUnit, lpUnit);
    CHECK_HR;
    vector<Int32> lvInputLevel;
    for(Int32 i = 0; i <lnUnit; ++i)
    {
        lvInputLevel.push_back(lpUnit[i]);
    }
    
    vector<Int32> lvAggLevel;
    vector<Int32> lvFinalLevel;  
    //something to do with dimty
    if(mLevelResolver.getMCESchema() != NULL)
    {
        hr = mLevelResolver.ResolveAggLevel(lvInputLevel, mpContext->mvBreakbyUnits, lvAggLevel);
        CHECK_HR;
        

        //something to do with dimty
        hr = mLevelResolver.ResolveDownwardLevel(lvAggLevel, mpContext->mvBreakbyUnits, lvFinalLevel);
        CHECK_HR;
    }
    //fali, 948719, backward compatibility with 941 server, only support report level subtotal, mpContext->mvBreakbyUnits save the subtotal level
    else if (mpContext->mvBreakbyUnits.size() > 0)
    {
        return S_FALSE;
    }
        
    
    bool lbNoAgg = (NULL == mpContext->mpFuncObject) && (lvInputLevel == lvAggLevel);
    bool lbNoAdjust = (lvAggLevel == lvFinalLevel);
    //something to do with noAgg and noAdjust, like duplicate slice
    if (lbNoAgg)
    {
        if (lbNoAdjust)
        {
            //hr =  mpTargetCube->duplicateSlice(lpSliceIn, ppSliceOut, false);
            CHECK_HR;
        } else {
            //Strech slice to final level
            DSSCubeDataSlice *lpOutputSlice = NULL;
            hr = DSSCalculationHelper::StrechSliceDownward(mpContext->mpCube,
                                                           lpSliceIn,
                                                           lnUnit, lpUnit,
                                                           lvFinalLevel.size(), &lvFinalLevel[0],
                                                           &lpOutputSlice);
            CHECK_HR;
            mpContext->mpCube->InsertDataSlice(lpOutputSlice, *opResultSliceID);
        }
        return hr; //Return here, no aggregation
    }

    if(NULL == mpContext->mpFuncObject)
    {
        return E_FAIL;
    }
    // Check if there is any missing input slice.
    // If there is any, propogate it to the output slice
    // 814074, fali
    bool lFoundMissing = false;
    DSSCubeDataSlice *lpMissingOutputSlice = NULL;
    hr = hPropogateMissing(1, &lpSliceIn, lvFinalLevel.size(), &lvFinalLevel[0], &lpMissingOutputSlice, lFoundMissing);
    CHECK_HR;
    
    if (lFoundMissing)
    {
        mpContext->mpCube->InsertDataSlice(lpMissingOutputSlice, *opResultSliceID);
        return hr = S_OK;
    }

    bool lbHasSort = (mpContext->mvSortbyDefn).size() == 0 ? false: true;
    vector<Int32> lvLookupUnit;
    hr = hResolveLookupUnits(lvInputLevel, lvAggLevel, lbHasSort, lvLookupUnit);
    CHECK_HR;
    
    DSSTabularIndex *lpLookupIndex = NULL;
    DSSTabularIndexPool *lpIndexPool = NULL;
    mpContext->mpCube->getIndexPool(&lpIndexPool);
    hr = lpIndexPool->getIndex(lvLookupUnit.size(), &lvLookupUnit[0], &lpLookupIndex);
    CHECK_HR;
    
    DSSTabularIndex *lpOutputIndex = NULL;
    hr = lpIndexPool->getIndex(lvAggLevel.size(), &lvAggLevel[0], &lpOutputIndex);
    CHECK_HR;
    
    bool lfIsNumericalSlice = false;
    bool lbCellFormatSlice;
    // treat count(*) as numeric slice calculation
	if (lCountType == eCountStar)
	{
		lfIsNumericalSlice = true;
        lbCellFormatSlice = false;
	}
	else
	{
        hr = hIsNumericalSlice(lpSliceIn, lfIsNumericalSlice, lbCellFormatSlice);
        CHECK_HR;
    }
    // TQMS553309 for count on non-numeric ID, enable the optimization, but skip the nulls for now
	// count(offsets) can be also done with numeric interface
	EnumNullCheckingOption lNullCheckOption = mpContext->mNullCheckingOption;
	if ((lCountType == eCountID  || lCountType == eCountOffsets) && lfIsNumericalSlice == false)
	{
		lfIsNumericalSlice = true;
        lbCellFormatSlice = false;
		lNullCheckOption = DssNumericErrorSkip;
	}
    //tliao, 822148, 2014/09/23, support cell format data calculation
    bool lbSupportCellFormat = mpContext->mpFuncObject->SupportCellFormat();
    if (lbSupportCellFormat && lbCellFormatSlice)
    {
        hr = hApplyNonNumericalAggregation(NULL, lpLookupIndex,lpOutputIndex, lNullCheckOption, opResultSliceID, lCountType, mpContext->mpFuncObject);
        CHECK_HR;
    }
    else
    {
        if(lfIsNumericalSlice)
        {
            DSSAggregationFunction *lpAggFunction = NULL;
            lpAggFunction = dynamic_cast<DSSAggregationFunction*> (mpContext->mpFuncObject);
            CHECK_PTR(lpAggFunction);
            
            hr = hApplyNumericalAggregation(lpAggFunction, lpLookupIndex,
                                            lpOutputIndex, lNullCheckOption, opResultSliceID, lCountType);
            CHECK_HR;
        }
        else
        {
            DSSNNAggregationFunction *lpNNAggFunction = NULL;
            lpNNAggFunction = dynamic_cast<DSSNNAggregationFunction*> (mpContext->mpFuncObject);
            CHECK_PTR(lpNNAggFunction);
            
            hr = hApplyNonNumericalAggregation(lpNNAggFunction, lpLookupIndex,
                                               lpOutputIndex, lNullCheckOption, opResultSliceID, lCountType);
            CHECK_HR;
        }
    }
    
    if (!lbNoAdjust)
    {
        DSSCubeDataSlice *lpAggResultSlice = mpContext->mpCube->getSliceByID(*opResultSliceID);
        DSSCubeDataSlice *lpOutputSlice = NULL;
        hr = DSSCalculationHelper::StrechSliceDownward(mpContext->mpCube, //mpBuffer,
                                                       lpAggResultSlice,
                                                       lvAggLevel.size(), &lvAggLevel[0],
                                                       lvFinalLevel.size(), &lvFinalLevel[0],
                                                       &lpOutputSlice);
        CHECK_HR;
        mpContext->mpCube->InsertDataSlice(lpOutputSlice, *opResultSliceID);
    }

	return S_OK;
}

int DSSSliceCalculator::hApplyGenericFunc(int *opResultSliceID)
{
    int hr = S_OK;
    
    hr = hProcessConditionalMetric(mvSlice.size(), &mvSlice[0]);
	CHECK_HR;
    
    bool bAllNumerical = true;
    bool lbHasCellFormat = false;
	for (int i = 0; i < mvSlice.size(); i ++)
	{
		bool bNumericalSlice;
        bool bCellFormatSlice;
        hr = hIsNumericalSlice(mvSlice[i], bNumericalSlice, bCellFormatSlice);
        CHECK_HR;
        
        if (!bNumericalSlice)
            bAllNumerical = false;
        if (bCellFormatSlice)
            lbHasCellFormat = true;
	}
    
    DSSGenericFunction *lpFunc = NULL;
    DSSNNGenericFunction *lpNNFunc = NULL;
    lpFunc = dynamic_cast<DSSGenericFunction *>(mpContext->mpFuncObject);
    lpNNFunc = dynamic_cast<DSSNNGenericFunction *>(mpContext->mpFuncObject);
    
    //fali, 861201, validate olap pattern as server side did
    CComVariant lVar;
    if (lpFunc)
    {
        hr = lpFunc->get_PropertyInfo(-10, NULL, NULL, &lVar); // only new olap functions will succeed this call
    }
    if (SUCCEEDED(hr))
    {
        switch (lVar.intVal)
        {
            case DssFuncOLAPSum:
            case DssFuncOLAPAvg:
            case DssFuncOLAPCount:
            case DssFuncOLAPMax:
            case DssFuncOLAPMin:
            case DssFuncLag:
            case DssFuncLead:
            {
                vector<int> lvBreakByUnits = mpContext->mvBreakbyUnits;
                int lnBreakByUnit = lvBreakByUnits.size();                
                if (lpFunc)
                {
                    hr = DSSCalculationHelper::validateOlapPattern(lpFunc, mpContext->mvSortbyDefn.size(), lnBreakByUnit, &lvBreakByUnits[0]);
                }
            }
                break;
            default:
                hr = S_OK;
        }
        
        if (FAILED(hr)) // verification fails
            return hr;
    }
    vector<int> lvBreakBy;
	vector<int> lvLookupUnits;
	GenericFunctionType lFuncType = eGenericUnknown;
	bool *lpfIsVector = NULL;
	EnumDSSDataType lOutputDataType = DssDataTypeUnknown;

    hr = hGetGenericLevels(lpFunc, lpNNFunc, lvBreakBy, lvLookupUnits, lFuncType, lpfIsVector, lOutputDataType);
    CHECK_HR;

    MBase::DSSStrongPtr<bool, MBase::DeleteArray<bool> > lIsVectorPtr(lpfIsVector);
    
    vector<int> lvResSliceLevel;
	if (lFuncType == eGenericType2 && mLevelResolver.getMCESchema())
    {
		//Type 2: ResultLevel = TrueLevel(Union(Lv1, Lv2, Ls))
        hr = mLevelResolver.ResolveTrueLevel(lvLookupUnits, lvResSliceLevel);
		CHECK_HR;
	}
    else
    {
		//AE_ASSERT (lFuncType == eGenericType1)
		//Type 1: may need to ajust the level later
        
        //941 hard code to support backward compatibility, shouldn't be called when connected to polaris server
        if(lFuncType == eGenericType2 && !mLevelResolver.getMCESchema())
        {
            //fali, 946315, for threshold calculation which uses FirstTrue function, the level is grid level, the following calculation can get correct result
            if (lpFunc->getOperatorType() == DssFuncFirstTrue)
            {
                lvBreakBy.clear();
            }
            else
            {
                //fali, 928766, if it's 941 server, it can't get correct breakby info, return.
                if(lpFunc->getOperatorType() == DssFuncRank || lpFunc->getOperatorType() == DssFuncRunningSum)
                    return S_FALSE;
            }

        }
        lvResSliceLevel = lvBreakBy;
	}
    
    vector<int> lvFinalLevel;
	if (lFuncType == eGenericType2)
    {
        lvFinalLevel = lvResSliceLevel;
	}
    else
    {
		//ResultLevel =  AdjustDownward(BreakbyLevel, L)
        if(mLevelResolver.getMCESchema())
        {
            hr = mLevelResolver.ResolveDownwardLevel(lvBreakBy, mpContext->mvBreakbyUnits, lvFinalLevel);
            CHECK_HR;
        }else//941 backward compatibility to support "if" function
        {
            lvFinalLevel = lvResSliceLevel;
        }
	}
    
    // Check if there is any missing input slice.
    // If there is any, propogate it to the output slice
    // 814074, fali
    bool lFoundMissing = false;
    DSSCubeDataSlice *lpMissingOutputSlice = NULL;
    hr = hPropogateMissing(mvSlice.size(), &mvSlice[0], lvFinalLevel.size(), &lvFinalLevel[0], &lpMissingOutputSlice, lFoundMissing);
    CHECK_HR;
    
    if (lFoundMissing)
    {
        mpContext->mpCube->InsertDataSlice(lpMissingOutputSlice, *opResultSliceID);
        return hr = S_OK;
    }

    DSSCube *lpCube = mpContext->mpCube;
    CHECK_PTR(lpCube);
    DSSTabularIndexPool *lpTabularIndexPool = NULL;
    hr = lpCube->getIndexPool(&lpTabularIndexPool);
    CHECK_HR;
    
    DSSTabularIndex *lpLookupIndex = NULL;
    hr = lpTabularIndexPool->getIndex(lvLookupUnits.size(), lvLookupUnits.size() == 0 ? NULL : &lvLookupUnits[0], &lpLookupIndex);
    CHECK_HR;
    
    DSSTabularIndex *lpBreakbyIndices = NULL;
    hr = lpTabularIndexPool->getIndex(lvBreakBy.size(), lvBreakBy.size() == 0 ? NULL : &lvBreakBy[0], &lpBreakbyIndices);
    CHECK_HR;
	
    DSSTabularIndex *lpResIndices;
	if (lFuncType == eGenericType2)
	{
		// qjiang TQMS547994 prepare for the data-schema mismatch case
		// check if the result index has different row # than the lookup index
		// if it does mismatch, don't do the reduce here
		hr = lpTabularIndexPool->getIndex(lvResSliceLevel.size(), lvResSliceLevel.size() == 0 ? NULL : &lvResSliceLevel[0], &lpResIndices);
		CHECK_HR;
        
		Int32 lnRow1 = lpLookupIndex->Count();
        
		Int32 lnRow2 = lpResIndices->Count();
        
		if (lnRow1 != lnRow2)
		{
            lvResSliceLevel = lvLookupUnits;
		}
	}
    
    //931 i-server compatibility
    if((mpContext->mFuncIndex == DssFuncIF || mpContext->mFuncIndex == DssFuncFirstTrue) && lpTabularIndexPool->getMainIndex()== NULL/*for old RWD, the main index is NULL*/)
    {
        int lnMaxRowCount = 0;
        for(int i = 0; i < mpContext->mvSliceID.size(); i++)
        {
            DSSCubeDataSlice* lpSlice = NULL;
            if(mpContext->mvSliceID[i])
            {
                hr = mpContext->mvSliceID[i]->GetSlice(&lpSlice);
                if(lpSlice)
                {
                    DSSDataColumn* lpColumn = lpSlice->getColumn();
                    if(lpColumn)
                    {
                        if(lnMaxRowCount < lpColumn->getRowCount())
                        {
                            lnMaxRowCount = lpColumn->getRowCount();
                        }
                    }
                }
            }
        }
        if(lnMaxRowCount != lpLookupIndex->Count())
        {
            mbOldRWD = true;
            
            lpLookupIndex = new DSSTabularIndex();
            lpLookupIndex->Init(0, lnMaxRowCount, 0, 0, NULL);
            
            lpBreakbyIndices = lpLookupIndex;
        }
    }
    bool bIsNNFunctionOnly = lpNNFunc != NULL ? lpNNFunc->IsNNFunctionOnly() : false;
    //tliao, 822148, 2014/09/23, support cell format data calculation
    bool bSupportCellFormat = mpContext->mpFuncObject->SupportCellFormat();
    if (bAllNumerical && !bIsNNFunctionOnly && (!lbHasCellFormat || !bSupportCellFormat))
    {
        hr = hApplyNumericalGenericFunction(lpFunc, lFuncType,
                                            lpLookupIndex, lpBreakbyIndices,
                                            lpfIsVector,
                                            lvResSliceLevel,
                                            lOutputDataType,
                                            opResultSliceID);
        CHECK_HR;
    }
    else if (lpNNFunc)
    {
        hr = hApplyNonNumericalGenericFunction(lpNNFunc, lFuncType,
                                               lpLookupIndex, lpBreakbyIndices,
                                               lpfIsVector,
                                               lvResSliceLevel,
                                               lOutputDataType,
                                               opResultSliceID,
                                               bSupportCellFormat);
        
        CHECK_HR;
    }
    else
        return E_FAIL;
    
    if (lFuncType == eGenericType1)
    {
        //may need to adjust result result to breakby level
        bool lbNoAdjust = (lvFinalLevel.size() == lvResSliceLevel.size()) && lvFinalLevel == lvResSliceLevel;
     
        if (!lbNoAdjust)
        {
            DSSCubeDataSlice *lpResultSlice = mpContext->mpCube->getSliceByID(*opResultSliceID);
            DSSCubeDataSlice *lpOutputSlice = NULL;
            hr = DSSCalculationHelper::StrechSliceDownward(mpContext->mpCube,
                                                            lpResultSlice,
                                                            lvResSliceLevel.size(), &lvResSliceLevel[0],
                                                            lvFinalLevel.size(), &lvFinalLevel[0],
                                                            &lpOutputSlice);
             CHECK_HR;
             mpContext->mpCube->InsertDataSlice(lpOutputSlice, *opResultSliceID);
        }
     }
    
	return S_OK;
}

int DSSSliceCalculator::hApplyNumericalSimpleFunction(DSSSimpleFunction* pFunc, DSSTabularIndex* ipLookupIndex,
                                                      DSSTabularIndex *ipOutputIndex, int *opResultSliceID)
{
	int hr = S_OK;
	
	int lnUnit = ipOutputIndex->getNumUnits();
    int *lpUnit = new int[lnUnit];
    MBase::DSSStrongPtr<Int32, MBase::DeleteArray<Int32> > lUnit1Ptr(lpUnit);
    hr = ipOutputIndex->getUnits(lnUnit, lpUnit);
    CHECK_HR;
    
	//initialize a slice, which is always double now.
	DSSCubeDataSlice *lpSliceOut = NULL;
    if(mbOldRWD)
    {
        DSSDataColumn *lpColumn = NULL;
        int hr = DSSAEHelper::GenXTabCol(DssDataTypeDouble, ipOutputIndex->Count(), &lpColumn);
        CHECK_PTR(lpColumn);
        
        hr = mpContext->mpCube->CreateDataSlice(ipOutputIndex, lpColumn, &lpSliceOut);
        CHECK_PTR(lpSliceOut);
    }else{
        hr = mpContext->mpCube->CreateDataSlice(lnUnit, lpUnit, DssDataTypeDouble, &lpSliceOut);
        CHECK_HR_PTR(lpSliceOut);
       
    }
 
    mpContext->mpCube->InsertDataSlice(lpSliceOut, *opResultSliceID);
    
	//preparetion of the data to be passed into function
	int lnResRow = ipLookupIndex->Count();
	
    // nothing to do
	if (lnResRow <= 0)
		return S_OK;
    
	//now going to set the data
	DSSDataColumn *lpResCol = lpSliceOut->getColumn();
	CHECK_PTR(lpResCol);
	
    DSSXTabBitVector* lpMask = NULL;
	if (mpContext->mpMaskInfos && !mpContext->mpMaskInfos->IsEmpty())
	{
		hr = mpContext->mpMaskInfos->GetCalculatedMask(ipLookupIndex, &lpMask);
		CHECK_HR;
	}
    
    
    // qjiang 08/12/2009
	// check if we can use direct calculate
	bool lfDirectCalculate = false;
	hr = hCheckDirectCalculate(ipLookupIndex, lfDirectCalculate, pFunc);
	CHECK_HR;
    
	if (lfDirectCalculate && !mbOldRWD)
	{
		hr = hDirectCalculateForSimpleFunction(ipLookupIndex, pFunc, lpSliceOut, lpMask, mpContext->mNullCheckingOption);
		if (hr == S_OK)
			return S_OK;
		// if failed to calculate, let it go to the old approach
	}
    
    MBase::SimpleBuffer lSimpleBuffer;
	MBase::BufferPtr lBufferPtr(&lSimpleBuffer);
	MBase::Buffer *lpBuffer = lBufferPtr.Get();
	CHECK_NEWED_PTR(lpBuffer);
    
    double ** lppData = NULL;
    // Veracode: CWE ID 190: Integer Overflow or Wraparound
    signed long long tmp = (signed long long) lnResRow;
	if (tmp > 0 && tmp <= INT_MAX/sizeof(double))
    {
        lppData = new (lpBuffer) double*[lnResRow];
	CHECK_NEWED_PTR(lppData);
    }
    // Veracode
	
	//store the result
	double * lpResult = NULL;
    if (tmp > 0 && tmp <= INT_MAX/sizeof(double))
    {
        lpResult = new double[lnResRow];
	CHECK_NEWED_PTR(lpResult);
    }
    MBase::DSSStrongPtr<double, MBase::DeleteArray<double> > lResultPtr(lpResult);
    
	
    DSSDataFlag * lpFlag = NULL;
    if (tmp > 0 && tmp <= INT_MAX/sizeof(double))
    {
        lpFlag = new DSSDataFlag[lnResRow];
        CHECK_NEWED_PTR(lpFlag);
    }
    MBase::DSSStrongPtr<DSSDataFlag, MBase::DeleteArray<DSSDataFlag> > lFlagPtr(lpFlag);
	
    bool *lpfRowCalculated = NULL;
    if (tmp > 0 && tmp <= INT_MAX/sizeof(double))
    {
        lpfRowCalculated = new bool[lnResRow];
        CHECK_NEWED_PTR(lpfRowCalculated);
	}
    MBase::DSSStrongPtr<bool, MBase::DeleteArray<bool> > lfRowCalculatedPtr(lpfRowCalculated);
	
	DSSCubeDataSlice **lpSliceIn = &mvSlice[0];
	int nSliceIn = mvSlice.size();
	EnumNullCheckingOption iNullCheckingOption = mpContext->mNullCheckingOption;
    
    DSSTabularIndexPool *lpIndexPool = NULL;
    DSSTabularIndex *lpMainIndex = NULL;
    if (0 != mvSlice.size())
    {
        hr = mpContext->mpCube->getIndexPool(&lpIndexPool);
        CHECK_HR;
        
        //in old RWD, main index of index pool is NULL
        hr = lpIndexPool->getMainIndex(&lpMainIndex);
        CHECK_HR;
    }
	
    // Setup the direct pointers so that the coming loop can fast access
	DSSSliceCorrelator lSliceCorrelator;
	hr = lSliceCorrelator.Init(ipLookupIndex, nSliceIn, lpSliceIn, lpSliceOut, mpContext->mpCube);
	CHECK_HR;
    
	int lnRowCalculated = 0,  iRow = 0 ;
	for(iRow = 0; iRow < lnResRow; iRow ++)
	{
        if (lpMask && !lpMask->Get(iRow))
		{
			lpfRowCalculated[iRow] = false;
			continue;
		}
        
		lpfRowCalculated[iRow] = true;
		signed long long temp = (signed long long)nSliceIn;
        if (temp > 0 && temp <= INT_MAX/sizeof(double))
        {
		lppData[lnRowCalculated] = new (lpBuffer) double[nSliceIn];
		CHECK_NEWED_PTR(lppData[lnRowCalculated]);
        }
		
		int iSlice = 0;
		for(; iSlice < nSliceIn; iSlice ++)
		{
			DSSDataFlag lFlag = DssDataUnknown;
            
            if(mbOldRWD)
            {
                int lRowInSliceIndex = iRow;
                if (lpMainIndex)
                {
                    int lnTotalUnits = 0;
                    hr = hGetTotalUnits(lnTotalUnits);
                    CHECK_HR
                    
                    vector<int> lvKeys(lnTotalUnits);
                    hr = ipLookupIndex->Item(iRow, lvKeys.size(), 0 == lvKeys.size() ? NULL : &lvKeys[0]);
                    CHECK_HR;
                    
                    DSSTabularIndex *lpSliceIndex = lpSliceIn[iSlice]->getIndices();
                    if (lpSliceIndex)
                    {
                        hr = lpSliceIndex->LookUp(lvKeys.size(), 0 == lvKeys.size() ? NULL : &lvKeys[0], &lRowInSliceIndex);
                        CHECK_HR;
                    }
                }
                
                //TODO: lookup from input slice index
                if(lpSliceIn[iSlice]->getColumn() && lpSliceIn[iSlice]->getColumn()->getRowCount() == 1)
                {
                    //for 931 i-server, this means a constant slice
                    lRowInSliceIndex = 0;
                }
                hr = lpSliceIn[iSlice]->getNumericByOffset(lRowInSliceIndex, lppData[lnRowCalculated][iSlice], lFlag, NULL);
                CHECK_HR;
                
                if(lFlag != DssDataOk)
                {
                    if( iNullCheckingOption == DssNumericErrorSkip)
                    {
                        //472498, inheritate the null flag from input
                        hr = lpSliceOut->setNumericByOffset(iRow, lpResult[iRow], lFlag);
                        CHECK_HR;
                        
                        lpFlag[iRow] = lFlag;
                        lpfRowCalculated[iRow] = false;
                    }
                    else
                    {
                        //it is treat as, we just need to replace the value used,
                        
                        //now use 0
                        lppData[lnRowCalculated][iSlice] = 0;
                        lpFlag[iRow] = DssDataOk;
                    }
                }//end of if(lFlag != DssDataOk)
                
            }else
            {
                hr = lSliceCorrelator.GetInputValue(iSlice, iRow, &lppData[lnRowCalculated][iSlice], &lFlag);
                CHECK_HR;
                
                if(lFlag != DssDataOk)
                {
                    if( iNullCheckingOption == DssNumericErrorSkip)
                    {
                        //tliao, 12/12/2013, 822594
                        if(lpfRowCalculated[iRow])
                        {
                            hr = lSliceCorrelator.SetOutputValue(iRow, lpResult[iRow], lFlag);
                            CHECK_HR;
                        }
                        lpFlag[iRow] = DssDataNull;
                        lpfRowCalculated[iRow] = false;
                    }
                    else
                    {
                        //it is treat as, we just need to replace the value used,
                        
                        //now use 0
                        lppData[lnRowCalculated][iSlice] = 0;
                        lpFlag[iRow] = DssDataOk;
                    }
                }//end of if(lFlag != DssDataOk)
            }
		}//end of for(; iSlice < nSliceIn; iSlice ++)
		
        if(lpfRowCalculated[iRow])
		{
            //ok
            lnRowCalculated ++;
        }
	}//end of for(iRow = 0; iRow < lnResRow; iRow ++)
    
	//now going to do the calculation
	hr = pFunc -> CalculateRepeat(nSliceIn,
								  lnRowCalculated,
								  lppData,
								  lpResult,
								  lpFlag);
	CHECK_HR;
	
    lnRowCalculated = 0;
	for (iRow = 0; iRow < lnResRow; iRow ++)
	{
		if (lpfRowCalculated[iRow])
		{
            if(mbOldRWD)
            {
                hr = lpSliceOut->setNumericByOffset(iRow, lpResult[iRow], lpFlag[iRow]);
                CHECK_HR;
            }else{
            //tliao, 12/12/2013, 822594
                hr = lSliceCorrelator.SetOutputValue(iRow, lpResult[lnRowCalculated], lpFlag[lnRowCalculated]);
                CHECK_HR;
            }
            lnRowCalculated++;
		}
	}
	
	return S_OK;
}

int DSSSliceCalculator::hApplyNonNumericalSimpleFunction(DSSNNSimpleFunction* pFunc, DSSTabularIndex* ipLookupIndex,
                                                         DSSTabularIndex *ipOutputIndex,int *opResultSliceID, DSSFunctionObject* pCellFmtFunc)
{
    int hr = S_OK;
    
    EnumSimpleOperatorType lFunctionType = DssFuncUnknown;
    if (pFunc)
    {
        lFunctionType = pFunc->getOperatorType();
    }
    
    Int32 lnTotalUnits = 0;
	hr = hGetTotalUnits(lnTotalUnits);
	CHECK_HR;
    
    DSSXTabKey *lKeyUnits = new DSSXTabKey[lnTotalUnits];
    CHECK_NEWED_PTR(lKeyUnits);
    MBase::DSSStrongPtr<DSSXTabKey, MBase::DeleteArray<DSSXTabKey> > lKeyUnitsPtr(lKeyUnits);
    
    DSSCubeDataSlice **lppSliceIn = &mvSlice[0];
	int nSliceIn = mvSlice.size();
	EnumNullCheckingOption iNullCheckingOption = mpContext->mNullCheckingOption;
	
    EnumDSSDataType *inputType = new EnumDSSDataType[nSliceIn];
	CHECK_NEWED_PTR(inputType);
    MBase::DSSStrongPtr<EnumDSSDataType, MBase::DeleteArray<EnumDSSDataType> > lInputTypePtr(inputType);
    
    LONG iSlice = 0;
	for(iSlice = 0; iSlice < nSliceIn; iSlice ++)
    {
        inputType[iSlice] = lppSliceIn[iSlice]->getDataType();
		CHECK_HR;
	}
    
    EnumDSSDataType outputType = DssDataTypeUnknown;
	if (pFunc != NULL)
	{
		hr = pFunc->GetOutputType(nSliceIn, inputType, &outputType);
		CHECK_HR;
	}
	else
    {
		outputType = DssDataTypeCellFormatData;
    }
    
    if ((outputType == DssDataTypeBigDecimal && lFunctionType != DssFuncMax2 && lFunctionType != DssFuncMin2)
         || (pCellFmtFunc == NULL && outputType == DssDataTypeCellFormatData))
    {
		outputType = DssDataTypeDouble;
	}
    
    int lByte = 0;
	hr = DSSAEHelper::SizeOfType(outputType, &lByte);
	CHECK_HR;
    
    int lnUnit = ipOutputIndex->getNumUnits();
    int *lpUnit = new int[lnUnit];
    MBase::DSSStrongPtr<Int32, MBase::DeleteArray<Int32> > lUnit1Ptr(lpUnit);
    hr = ipOutputIndex->getUnits(lnUnit, lpUnit);
    CHECK_HR;
    
	//initialize a slice, which is always double now.
	DSSCubeDataSlice *lpSliceOut = NULL;
    if(mbOldRWD)
    {
        DSSDataColumn *lpColumn = NULL;
        int hr = DSSAEHelper::GenXTabCol(outputType, ipOutputIndex->Count(), &lpColumn);
        CHECK_PTR(lpColumn);
        
        hr = mpContext->mpCube->CreateDataSlice(ipOutputIndex, lpColumn, &lpSliceOut);
        CHECK_PTR(lpSliceOut);
    }else{
        hr = mpContext->mpCube->CreateDataSlice(lnUnit, lpUnit, outputType, &lpSliceOut);
        CHECK_PTR(lpSliceOut);
    }

    mpContext->mpCube->InsertDataSlice(lpSliceOut, *opResultSliceID);
    
    DSSSliceCorrelator lSliceCorrelator;
	hr = lSliceCorrelator.Init(ipLookupIndex, nSliceIn, lppSliceIn, lpSliceOut, mpContext->mpCube);
	CHECK_HR;
    
    //preparetion of the data to be passed into function
	int lnResRow = ipLookupIndex->Count();
    
    //make a dummy result for fail case
	BYTE *lpDummyResult = new BYTE [lByte];
	CHECK_NEWED_PTR(lpDummyResult);
    MBase::DSSStrongPtr<BYTE, MBase::DeleteArray<BYTE> > lDummyResultPtr(lpDummyResult);
	memset(lpDummyResult, 0, lByte);
    
    //Initilize raw data space
	BYTE **ppRawData = new BYTE *[nSliceIn];
	CHECK_NEWED_PTR(ppRawData);
    MBase::DSSStrongPtr<BYTE*, MBase::DeleteArray<BYTE*> > lRawDataPtr(ppRawData);
    
	Int32 *pCbSize = new Int32 [nSliceIn];
	CHECK_NEWED_PTR(pCbSize);
    MBase::DSSStrongPtr<Int32, MBase::DeleteArray<Int32> > lCbSizePtr(pCbSize);
    
    for (iSlice = 0; iSlice < nSliceIn; iSlice++)
    {
        hr = lppSliceIn[iSlice]->getSizeOfData(pCbSize[iSlice]);
        CHECK_HR;
        
        ppRawData[iSlice] = NULL;
    }
    
    // temp buffer to store result data
	int lResultBufLen = lByte;
	BYTE *lpResultData = new BYTE[lResultBufLen];
	CHECK_NEWED_PTR(lpResultData);
    MBase::DSSStrongPtr<BYTE, MBase::DeleteArray<BYTE> > lResultDataPtr(lpResultData);
    /*
     DSSXTabBitVector* lpMask = NULL;
     if (mpMaskInfos && !mpMaskInfos->IsEmpty())
     {
     hr = mpMaskInfos->GetCalculatedMask(ipLookupIndex, &lpMask);
     CHECK_HR;
     }
     */
	bool lfStringFunc = DSSAEHelper::IsStringFunction(lFunctionType);
    
    if (nSliceIn < 0) {
        return E_INVALIDARG;
    }
	
    // Veracode: CWE ID 190: Integer Overflow or Wraparound
//    signed long long tmp = (signed long long) nSliceIn;
//    if (tmp > 0 && tmp <= INT_MAX/sizeof(int))
//    {
//        inputType = new EnumDSSDataType[nSliceIn];
//    }
    // Veracode
    
    int iRow = 0;
	for(iRow = 0; iRow < lnResRow; iRow ++)
	{
		//Create a one-dimensional Safearray for input paramter[nSlice];
		SAFEARRAYBOUND lBound;
		lBound.lLbound = 0;
		lBound.cElements = (lfStringFunc ? (2 * nSliceIn) : nSliceIn);
		CComVariant lInVariant;
		lInVariant.vt = VT_ARRAY|VT_VARIANT;
		lInVariant.parray = SafeArrayCreate(VT_VARIANT, 1, &lBound);
        
		bool lCalculate = true;
        
		// pack the input parameters into the input variant array
		for(iSlice = 0; iSlice < nSliceIn; iSlice ++)
		{
			EnumDSSDataType valueType = DssDataTypeUnknown;
			DSSDataFlag lFlag = DssDataUnknown;
            
            if(mbOldRWD)
            {
                int lCurrentRow = iRow;
                if(lppSliceIn[iSlice]->getColumn()&& lppSliceIn[iSlice]->getColumn()->getRowCount() == 1)
                {
                    //in 931 this mean a constant slice
                    lCurrentRow = 0;
                }
                //TODO: lookup from input slice index
                hr = lppSliceIn[iSlice]->getValByOffset(lCurrentRow, (const char**)(&ppRawData[iSlice]), pCbSize[iSlice], valueType, lFlag, NULL);
                CHECK_HR;
                
                if(lFlag != DssDataOk)
                {
                    if (iNullCheckingOption == DssNumericErrorTreatAs) {
                        // weiwang 2010-07-08 TQMS417837 need to handle NULL cell format data
                        if(DSSAEHelper::IsNumeric(valueType) || valueType == DssDataTypeCellFormatData) //set input to 0
                        {
                            CComVariant lData;
                            lData.vt = VT_I2;
                            lData.iVal = 0;
                            
                            hr = SafeArrayPutElement(lInVariant.parray, (LONG*)&iSlice, &lData);
                            CHECK_HR;
                        }
                        else
                        {
                            double lValue = 0;
                            // jxie, 2011-3-3, TQMS 450450
                            // Don't set DateTime to 0, because DateTime has been initialized to 1899/12/30
                            if (outputType == DssDataTypeDate || outputType == DssDataTypeTimeStamp || outputType == DssDataTypeTime)
                                hr = lpSliceOut->setValByOffset(iRow, (char*) &lValue, sizeof (double), outputType, lFlag);
                            else
                                hr = lpSliceOut->setValByOffset(iRow, (char*) &lValue, sizeof (double), outputType, DssDataOk);
                            CHECK_HR;
                            
                            lCalculate = false;
                        }
                    }
                    else
                    {
                        // skip this row and set the result flag directly
                        
                        // aye, in case the OpType is Equals or NotEquals, we can do the comparison now
                        if (lFunctionType == DssFunctionEquals || lFunctionType == DssFunctionNotEqual)
                        {
                            AE_ASSERT(nSliceIn == 2);
                            
                            DSSDataFlag lFlag0 = DssDataOk;
                            DSSDataFlag lFlag1 = DssDataOk;
                            
                            hr = lppSliceIn[0]->getValByOffset(iRow, (const char**)(&ppRawData[0]), pCbSize[0], valueType, lFlag, NULL);
                            CHECK_HR;
                            
                            hr = lppSliceIn[1]->getValByOffset(iRow, (const char**)(&ppRawData[1]), pCbSize[1], valueType, lFlag, NULL);
                            CHECK_HR;
                            
                            // mpittkin, 325942 : handle null metric value and valid comparison value
                            double lValue = 0;
                            DSSDataFlag lFlagFinal = DssDataOk;
                            if(lFlag0 != DssDataOk && lFlag1 == DssDataOk)
                            {
                                lFlagFinal = lFlag0;
                            }
                            else if ((lFlag0 == lFlag1 && lFunctionType == DssFunctionEquals) ||
                                     (lFlag0 != lFlag1 && lFunctionType == DssFunctionNotEqual))
                                lValue = 1;
                            
                            hr = lpSliceOut->setValByOffset(iRow, (char*) &lValue, sizeof (double), outputType, lFlagFinal);
                            CHECK_HR;
                        }
                        else
                        {
                            hr = lpSliceOut->setValByOffset(iRow, (char*)lpDummyResult, lByte, outputType, lFlag);
                            CHECK_HR;
                        }
                        
                        lCalculate = false;
                        
                        break;
                    }
                }
                else
                {   //put the input parameter in the variant array
                    CComVariant lData;
                    
                    hr = DFCDataConversion::ConvertToVariant(ppRawData[iSlice], valueType, pCbSize[iSlice], &lData);
                    CHECK_HR;
                    
                    hr = SafeArrayPutElement(lInVariant.parray, (LONG*)&iSlice, &lData);
                    AE_ASSERT(hr == S_OK);
                }
            }else
            {
                Int32 retSize = 0;
                hr = lSliceCorrelator.GetInputValue(iSlice, iRow, &ppRawData[iSlice], pCbSize[iSlice], &retSize, &valueType, &lFlag);
                CHECK_HR;
            
                if(lFlag != DssDataOk)
                {
                    // one input parameter is not valid
                    // deliu, 2012-05-17, TQMS 589476, process null in string func in future event
                    // fali, 814085, for string function, if there is one invalid slice, set dataValue as dummyResult 0 and keep flag.
                    if (lfStringFunc) { // qjiang Add support for new falg
                        
                        hr = lSliceCorrelator.SetOutputValue(iRow, lpDummyResult, lByte, outputType, lFlag);
                        CHECK_HR;
                        
                        lCalculate = false;
                        break;
                        
                    }
                    else if (iNullCheckingOption == DssNumericErrorTreatAs) {
                        // weiwang 2010-07-08 TQMS417837 need to handle NULL cell format data
                        if(DSSAEHelper::IsNumeric(valueType) || valueType == DssDataTypeCellFormatData) //set input to 0
                        {
                            CComVariant lData;
                            lData.vt = VT_I2;
                            lData.iVal = 0;
                            
                            hr = SafeArrayPutElement(lInVariant.parray, (LONG*)&iSlice, &lData);
                            CHECK_HR;
                        }
                        else if (valueType == DssDataTypeBigDecimal)
                        {
                            double lValue = 0;
                            MDataType::DSSBigDecimal *lpBD = new MDataType::DSSBigDecimal(lValue);
                            DSSBigDecimalWrapper *lpBDWrapper = DSSBigDecimalWrapper::CreateInstance(lpBD);
                            CHECK_PTR(lpBDWrapper);
                            
                            CComVariant lData;
                            lData.vt = VT_DISPATCH;
                            lData.pdispVal = (IDispatch*)lpBDWrapper;
                            lData.pdispVal->AddRef();
                            hr = SafeArrayPutElement(lInVariant.parray, (LONG*)&iSlice, &lData);
                            CHECK_HR;
                        }
                        else
                        {
                            double lValue = 0;
                            // jxie, 2011-3-3, TQMS 450450
                            // Don't set DateTime to 0, because DateTime has been initialized to 1899/12/30
                            if (outputType == DssDataTypeDate || outputType == DssDataTypeTimeStamp || outputType == DssDataTypeTime)
                            {
                                hr = lSliceCorrelator.SetOutputValue(iRow, (BYTE*) &lValue, sizeof (double), outputType, lFlag);
                            }
                            else if (outputType == DssDataTypeBigDecimal)
                            {
                                MDataType::DSSBigDecimal lBD(lValue);
                                hr = lSliceCorrelator.SetOutputValue(iRow, (BYTE*)&lBD, sizeof(MDataType::DSSBigDecimal), outputType, DssDataOk);
                            }
                            else
                            {
                                hr = lSliceCorrelator.SetOutputValue(iRow, (BYTE*) &lValue, sizeof (double), outputType, DssDataOk);
                            }
                            
                            CHECK_HR;
                            
                            lCalculate = false;
                        }
                    }
                    else
                    {
                        // skip this row and set the result flag directly
                        
                        // aye, in case the OpType is Equals or NotEquals, we can do the comparison now
                        if (lFunctionType == DssFunctionEquals || lFunctionType == DssFunctionNotEqual)
                        {
                            AE_ASSERT(nSliceIn == 2);
                            
                            DSSDataFlag lFlag0 = DssDataOk;
                            DSSDataFlag lFlag1 = DssDataOk;
                            
                            hr = lSliceCorrelator.GetInputValue(0, iRow, &ppRawData[0], pCbSize[0], &retSize, &valueType, &lFlag0);
                            CHECK_HR;
                            
                            hr = lSliceCorrelator.GetInputValue(1, iRow, &ppRawData[1], pCbSize[1], &retSize, &valueType, &lFlag1);
                            CHECK_HR;
                            
                            // mpittkin, 325942 : handle null metric value and valid comparison value
                            double lValue = 0;
                            DSSDataFlag lFlagFinal = DssDataOk;
                            if(lFlag0 != DssDataOk && lFlag1 == DssDataOk)
                            {
                                lFlagFinal = lFlag0;
                            }
                            else if ((lFlag0 == lFlag1 && lFunctionType == DssFunctionEquals) ||
                                     (lFlag0 != lFlag1 && lFunctionType == DssFunctionNotEqual))
                            {
                                if(lFunctionType == DssFunctionEquals && lFlag0 == DssDataNull && mbDSTrx)
                                {
                                    //982518
                                    lValue = 0;
                                }else{
                                    lValue = 1;
                                }
                            }
                            hr = lSliceCorrelator.SetOutputValue(iRow, (BYTE*) &lValue, sizeof (double), outputType, lFlagFinal);
                            CHECK_HR;
                        }
                        else
                        {
                            hr = lSliceCorrelator.SetOutputValue(iRow, lpDummyResult, lByte, outputType, lFlag);
                            CHECK_HR;
                        }
                        
                        lCalculate = false;
                        
                        break;
                    }
                }
                else
                {   //put the input parameter in the variant array
                    CComVariant lData;
                    
                    if (valueType == DssDataTypeCellFormatData && pCellFmtFunc == NULL) // convert cell format data to double for non-cell format funcs
                    {
                        double cellValue = ((MDataType::DSSCellFmtData *)ppRawData[iSlice])->GetValue();
                        hr = DFCDataConversion::ConvertToVariant((BYTE*)&cellValue, DssDataTypeDouble, retSize, &lData);
                    }
                    else
                        hr = DFCDataConversion::ConvertToVariant(ppRawData[iSlice], valueType, pCbSize[iSlice], &lData);
                    CHECK_HR;
                    
                    if (hIsNumerical(lData.vt))	// jxie, 2011-1-24, TQMS 453258
                    {
                        lData.ChangeType(VT_R8);
                    }
                    
                    if (lfStringFunc)		// in string function, first type then value
                    {
                        VARTYPE lInputDataType = lData.vt;
                        
                        EnumDSSDataType lValueType = DssDataTypeUnknown;
                        
                        switch(lInputDataType)
                        {
                            case VT_I2:
                                lValueType = DssDataTypeShort;
                                break;
                                //case VT_I4:		// can't process VT_I4 and VT_DISPATCH
                                //	break;
                                
                            case VT_R4:
                                lValueType = DssDataTypeFloat;
                                break;
                                
                            case VT_R8:
                                lValueType = DssDataTypeDouble;
                                break;
                                
                            case VT_BOOL:
                                lValueType = DssDataTypeBool;
                                break;
                                
                            case VT_I8:
                                lValueType = DssDataTypeInt64;
                                break;
                                
                            case VT_DATE:
                                lValueType = valueType;
                                
                            default:
                                break;
                        }

                        LONG lIndex = 2 * iSlice;
                        
                        CComVariant v;
                        hr = DFCDataConversion::ConvertToVariant((BYTE *)&lValueType, DssDataTypeInteger, 0, &v);
                        CHECK_HR;
                         
                        hr = SafeArrayPutElement(lInVariant.parray, &lIndex, &v);
                        CHECK_HR;
                        
                        lIndex = 2 * iSlice + 1;
                        hr = SafeArrayPutElement(lInVariant.parray, &lIndex, &lData);
                        CHECK_HR;
                    }
                    else
                    {
                        hr = SafeArrayPutElement(lInVariant.parray, (LONG*)&iSlice, &lData);
                        AE_ASSERT(hr == S_OK);
                    }
                }
			}
		} // end inner for loop
        
		// the input parameter array is ready.
		if (lCalculate)
        {
			CComVariant lResult;
			DSSDataFlag lFlag = DssDataUnknown;
			if (pFunc != NULL)
            {
				hr = pFunc->Calculate(nSliceIn, &lInVariant, &lResult, &lFlag);
            }
            else
                hr = pCellFmtFunc->CalculateCellData(nSliceIn, &lInVariant, &lResult, &lFlag);
            
			if (hr != S_OK){
				return	hr = E_FAIL;
                
			}	//AE_MSG xhan caculate fails
            
			//put result into the output slice
			if (lFlag == DssDataOk && outputType == DssDataTypeBigDecimal)
            {
                DSSBigDecimalWrapper *lpBDWrapper = dynamic_cast<DSSBigDecimalWrapper*>(lResult.pdispVal);
                CHECK_PTR(lpBDWrapper);
                
                const MDataType::DSSBigDecimal *lpBD = lpBDWrapper->getBigDecimal();
                CHECK_PTR(lpBD);
                
                if(mbOldRWD)
                {
                    hr = lpSliceOut->setValByOffset(iRow, (char *)lpBD, sizeof(MDataType::DSSBigDecimal), outputType, lFlag);
                    CHECK_HR;
                }else{
                    hr = lSliceCorrelator.SetOutputValue(iRow, (BYTE*)lpBD, sizeof(MDataType::DSSBigDecimal), outputType, lFlag);
                    CHECK_HR;
                }
                
                continue;
            }
            else if (lFlag == DssDataOk)
            {
				hr = hConvertFromVariant(lResult, outputType, lpResultData, lResultBufLen);
                CHECK_HR;
                lResultDataPtr.Reset(lpResultData);
			}
            
			//ok
			//set it back to the column, with the value and status
            if(mbOldRWD)
            {
                hr = lpSliceOut->setValByOffset(iRow, (char *)lpResultData, lResultBufLen, outputType, lFlag);
                CHECK_HR;
            }else{
                hr = lSliceCorrelator.SetOutputValue(iRow, lpResultData, lResultBufLen, outputType, lFlag);
                CHECK_HR;
            }
		} //if
	} //end for loop
    
    return hr = S_OK;
} // new

int DSSSliceCalculator::hConvertFromVariant(CComVariant &iVariant, EnumDSSDataType iDataType,
                                            BYTE* &opResultBuf, int &oResultLen)
{
	int hr = S_OK;
    
	// weiwang 2009-06-05 TQMS352644 UTF8 support
	if (iDataType == DssDataTypeVarChar || iDataType == DssDataTypeUTF8Char || iDataType == DssDataTypeBigDecimal)
    {
		// get string length from variant
		int sLen;
        DFCDataConversion::VariantSize(&iVariant, iDataType, &sLen);
		CHECK_HR;
        
		if (iDataType == DssDataTypeBigDecimal)
        {
			sLen = sLen+1;
		}
        
        //tliao, 12/25/2013, 826712 support datetime-to-BSTR
        if (iDataType == DssDataTypeVarChar && iVariant.vt == VT_DATE)
        {
            //DateTime "mm/dd/yyyy hh:mm:ssXM" has fixed length 22
            //tliao, 971805, the datetime format has been changed somehow...
            sLen = 22;
        }
        
		if (sLen > oResultLen)
        {   //reallocate result buffer if necessary
			oResultLen = sLen;
			opResultBuf = new BYTE[sLen];   // Old buffer which "opResultBuf" points to will be deleted by StrongPtr in the caller.
			CHECK_NEWED_PTR(opResultBuf);
		}
	}
    
	hr = DFCDataConversion::ConvertFromVariant(&iVariant, iDataType, opResultBuf, &oResultLen);
	CHECK_HR;
    
	return S_OK;
}

int DSSSliceCalculator::hApplyNumericalAggregation(DSSAggregationFunction* ipFunc,
                                                   DSSTabularIndex* ipLookupIndex,
                                                   DSSTabularIndex *ipOutputIndex,
                                                   EnumNullCheckingOption iNullCheckingOption,
                                                   int *opResultSliceID,
                                                   CountType iCountType)
{
	int hr = S_OK;
	
    int lnUnit = ipOutputIndex->getNumUnits();
    int *lpUnit = new int[lnUnit];
    MBase::DSSStrongPtr<Int32, MBase::DeleteArray<Int32> > lUnit1Ptr(lpUnit);
    hr = ipOutputIndex->getUnits(lnUnit, lpUnit);
    CHECK_HR;
    
	//initialize a slice, which is always double now.
	DSSCubeDataSlice *lpSliceOut = NULL;
	hr = mpContext->mpCube->CreateDataSlice(lnUnit, lpUnit, DssDataTypeDouble, &lpSliceOut);
    CHECK_HR;
	CHECK_PTR(lpSliceOut);
    mpContext->mpCube->InsertDataSlice(lpSliceOut, *opResultSliceID);
	
	DSSCubeDataSlice *lpSliceIn = mvSlice[0];
	DSSDataColumn * lpInCol = NULL;//lpSliceIn->getColumn();
	//CHECK_PTR(lpInCol);
	
	// Link list of groups
	// lpStart: the start of each group;
	// lpGroupBound: the start of each group / the end of the previous group
	// lpBlockList: the list of all the groups combined together
	// lpBlockAddress: the start address of each block
	
	int *lpGroupBound = NULL;
	int *lpBlockList = NULL;
	int *lpBlockAddress = NULL;
	
	int lnResRow;	// number of output rows
	int lnRow;		// number of input rows
    
    DSSDataColumn* lpResCol = NULL;
	lpResCol = lpSliceOut->getColumn();
	CHECK_PTR(lpResCol);
	
    EnumDSSDataType lDataType = DssDataTypeReserved;
    
	// qjiang 04/16/2009
	// original data type can be integer/short as well
	void* lpData = NULL;
	DSSDataFlag* lpDataFlags = NULL;
	bool lAllDataValid = false;
    double lDivider = 1;//lpInCol->getDivider();
	
	bool lfUseUnionLevel = false;
    
	// according to the type of function, prepare the input data
	switch (iCountType)
	{
        case eCountStar:
		{
			lDataType = DssDataTypeInteger;
			lAllDataValid = true;
			
			// indtend to set the data and flag to be NULL
		}
            
            break;
            
        case eCountID:
		{
			// count on ID is used much frequently than on other forms,
			// we do aggressive optimization here
			// 1. use key column as input data
			// 2. if key is already distinct, count the row is enough
			// 3. if all data valid, skip the data flag
			// 4. pre-allocate enough memory for sharing between groups
            
			DSSTabularIndex *lpIndices = lpSliceIn->getIndices();
			CHECK_PTR(lpIndices);
            
            Int32 lRefUnitPos = lpSliceIn->GetRefUnitPos();
            AE_ASSERT(lRefUnitPos >= 0);
            
			DSSAggregateGeneralFunc* lpFunc = static_cast<DSSAggregateGeneralFunc*>(mpContext->mpFuncObject);
			if (lpFunc->GetDistinctProperty() && lpIndices->hasElementBlocks())
			{
				// qjiang 02/10/2011 TQMS430713 further optimization for AFB-CountDistinct in Cube
				// we can use Union(attribute, output level) as input
				Int32 lnUnits = lpIndices->getNumUnits();
                
				Int32 lpUnits[lnUnits];
				CHECK_NEWED_PTR(lpUnits);
                
				hr = lpIndices->getUnits(lnUnits, lpUnits);
				CHECK_HR;
                /*
                 Int32 lUnit = lpUnits[lRefUnitPos];
                 
                 // union the attribute and the output
                 Int32* lpUnionLast = std::set_union(&lUnit, &lUnit+1, pGroupByUnit, pGroupByUnit+nGroupByUnit, lpUnits);
                 Int32 lnUnion = lpUnionLast - lpUnits;
                 
                 // RefUnitPos should be changed accordingly
                 for (Int32 i = 0; i < lnUnion; i++)
                 {
                 if (lpUnits[i] == lUnit)
                 {
                 lRefUnitPos = i;
                 break;
                 }
                 }
                 
                 CComPtr<ICDSSXTabIndices> lpUnionIndices;
                 hr = mpTargetCube->get_Indices(lnUnion, lpUnits, &lpUnionIndices);
                 CHECK_HR_PTR(lpUnionIndices);
                 
                 // from now on, we should use this index as input
                 lpIndicesC = static_cast<CDSSXTabIndices*>(lpUnionIndices.p);
                 
                 // reset the lookup index
                 lpLookupIndex = lpUnionIndices;
                 
                 // in this case, we can skip the distinct operation
                 lfUseUnionLevel = true;
                 */
			}
            
			Int32 lnRow = lpIndices->Count();
            
			vector<Int32*> lvKey;
			hr = lpIndices->DecompressKeys(NULL, 1, &lRefUnitPos, NULL, NULL, lnRow, lvKey);
			CHECK_HR;
            
			// use this key column for calculation
			lpData = lvKey[0];
			
			Int32 lnUnit = lpIndices->getNumUnits();
            
			// qjiang 11/25/2010
			// for single unit index, if it is distinct, we can directly set the input as distinct
			if ((lpIndices->IsDistinct() && lnUnit == 1) || lfUseUnionLevel)
				lpFunc->SetInputDistinct(true);
			else
			{
				Int32 lMaxKey = lpIndices->getMaxKey(lRefUnitPos);
				
				// try to allocate some memory first for share
                hr = lpFunc->SetWorkingBuffer(lMaxKey);
                CHECK_HR;
			}
            
            //tliao, 4/21/2014, 869730, use get_ColumnC and get_RefColumnC instead of getColumn()
			DSSDataColumn * lpColumn = NULL;
            hr = lpSliceIn->get_ColumnC(&lpColumn, false);
            if (hr != S_OK || !lpColumn)
            {
                hr = lpSliceIn->get_RefColumnC(&lpColumn);
            }
            CHECK_HR_PTR(lpColumn);
            
			bool lAllValid = true;
            
			hr = lpColumn->getAllDataValidFlag(lAllDataValid);
			CHECK_HR;
            
			// if all data are valid, no need to pass the flag array
			if (!lAllDataValid)
			{
				lpDataFlags = new DSSDataFlag[lnRow];
				CHECK_NEWED_PTR(lpDataFlags);
                
				Int32* lpKey = lvKey[0];
				for (Int32 i = 0; i < lnRow; i++)
				{
					Int32 lOffset = *lpKey++;
					lpColumn->getFlag(lOffset, lpDataFlags[i]);
					CHECK_HR;
					if (lpDataFlags[i] != DssDataOk)
						lAllValid = false;
				}
                
				lAllDataValid = lAllValid ? true : false;
			}
			
			lDataType = DssDataTypeInteger;
		}
            
            break;
            
        case eCountOffsets:
		{
			// this must be metric column
            
			DSSDataColumn * lpColumn = NULL;
            hr = lpSliceIn->get_ColumnC(&lpColumn, false);
			CHECK_HR_PTR(lpColumn);
            
			lpColumn->getRawDataAndFlags(&lpData, &lpDataFlags);
			// don't check hr here
            
			hr = lpColumn->getAllDataValidFlag(lAllDataValid);
			CHECK_HR;
            
			lDataType = lpColumn->getDataType();
            /*
             lpOffsets = lpColumn->GetOffsets();
             
             hr = lpColumn->GetCountDistinct(lnDistinct);
             CHECK_HR;
             */
		}
            break;
            
        default:
		{
			// this is for normal aggregation
            lpInCol = lpSliceIn->getColumn();
            CHECK_PTR(lpInCol);
			
            lDataType = lpInCol->getDataType();
            
			// qjiang 04/16/2009
			// after add support for double to integer conversion, we also support integer aggregation in DirectCalculate
            if(lpInCol && (DssDataTypeDouble == lDataType || DssDataTypeNumeric == lDataType || DssDataTypeDecimal == lDataType ||
                           DssDataTypeInteger == lDataType || DssDataTypeShort == lDataType))
            {
                lpInCol->getRawDataAndFlags((void**)&lpData, &lpDataFlags);
                
                lpInCol->getAllDataValidFlag(lAllDataValid);
                
                
                 lDivider = lpInCol->getDivider();
                /*
                 lpOffsets = lpInCol->GetOffsets();
                 */
			}
		}
            break;
	}
    
    EnumSimpleOperatorType lType = mpContext->mpFuncObject->getOperatorType();
    
	// aye, 4/1/2009, streaming aggregation support
	bool lUseStreamAgg = false;
	if (lType == DssFuncSum && lpData != NULL)
	{
		DSSTabularIndex *lpOutputIndex = lpSliceOut->getIndices();
		CHECK_PTR(lpOutputIndex);
        
        //		hr = hCheckStreamAggregation(ipLookupIndex, lpOutputIndex, lUseStreamAgg);
		CHECK_HR;
        
	}
    DSSXTabBitVector *lpMask = NULL;	// mask that applies to the result. Only the groups with 'true' will be calculated
	hr = hGroupData(lpSliceIn, ipLookupIndex, lpSliceOut, lUseStreamAgg,
					lpBlockList, lnRow, lpGroupBound, lnResRow, lpBlockAddress,lpMask,(iCountType == eCountStar || lfUseUnionLevel));
	CHECK_HR;
	
	if (lnResRow <= 0)
		return hr = S_OK;
    
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lBlockListPtr(lpBlockList);
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lGroupBoundPtr(lpGroupBound);
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lBlockAddressPtr(lpBlockAddress);
	
    bool lfConditionalMetric = false;
	if (lpSliceIn->GetCondition() != NULL && lpMask && !lpMask->IsAllTrue())
	{
		lfConditionalMetric = true;
	}
    
	bool lUseDirectCalculate = ipFunc->SupportDirectCalculate();

	if( lUseDirectCalculate &&
	   (lpData != NULL || iCountType == eCountStar|| iCountType == eCountOffsets) &&
	   (iNullCheckingOption == DssNumericErrorSkip || iNullCheckingOption == DssNumericErrorTreatAs)
	   )
	{
		// We use top-down approach
		// For each output row, iterate its input rows.
		for(int iRow = 0; iRow < lnResRow; iRow++)
		{
            // aye, 12/22/2007, new filter algorithm
			// skip the group if it's not selected
			if (lpMask && lpMask->Get(iRow) == false)
			{
				if (lfConditionalMetric)
				{
					hr =lpResCol->setFlag(iRow, DssDataIgnoreCheckNull);
					CHECK_HR;
				}
                
				continue;
			}
            
			int *lpStart = &lpBlockList[lpGroupBound[iRow]];
			int lGroupSize = lpGroupBound[iRow + 1] - lpGroupBound[iRow];
			
			DSSDataFlag lGroupValid = DssDataOk;
			double lData = 0;
			
			ipFunc->DirectCalculate(lGroupSize, lpStart, lpBlockAddress, lpData, lDivider, lDataType, lpDataFlags, lAllDataValid, iNullCheckingOption, &lData, &lGroupValid);
			
			if(lGroupValid != DssDataOk)
			{
				lData = 0;
			}
			
			hr = lpSliceOut->setNumericByOffset(iRow, lData, lGroupValid);
			CHECK_HR;
		}
	}
	else
	{
		double *lpInput = new double[lnRow];
        CHECK_NEWED_PTR(lpInput);
        MBase::DSSStrongPtr<double, MBase::DeleteArray<double> > lInputPtr(lpInput);
        
        Int32 lpGroupPtr = 0;
        Int32 lpGroupEnd = 0;
        
        for(Int32 iRow = 0; iRow < lnResRow; ++iRow)
        {
            bool lIfAllIgnoreCheckNull = true;
            Int32 size = 0;
            lpGroupPtr = lpGroupBound[iRow];
            lpGroupEnd = lpGroupBound[iRow + 1];
            DSSDataFlag lGroupValid = DssDataOk;
            
            while (lpGroupPtr < lpGroupEnd && DssDataOk == lGroupValid)
            {
                Int32 lInputBlock = lpBlockList[lpGroupPtr];
                Int32 lStart = lpBlockAddress? lpBlockAddress[lInputBlock] : lInputBlock;
				Int32 lEnd = lpBlockAddress ? lpBlockAddress[lInputBlock + 1] : lInputBlock + 1;
				
                for (Int32 lInputRow = lStart; lInputRow < lEnd && lGroupValid == DssDataOk; lInputRow++)
				{
					double lData;
					DSSDataFlag lFlag;
					hr = lpInCol -> getNumericVal(lInputRow, lData, lFlag);
					CHECK_HR;
                    
					/*!!!!
                     if (lFlag == DssDataIgnoreCheckNull)
                     continue;
                     */
					lIfAllIgnoreCheckNull = false;
                    
					if(lFlag != DssDataOk)
					{
						switch(iNullCheckingOption)
						{
                            case DssNumericErrorSkip:
                                continue;
                                break;
                                
                            case DssNumericErrorPropagate:
                                if(lGroupValid == DssDataOk)
                                    lGroupValid = lFlag;
                                continue;
                                break;
                                
                            case DssNumericErrorTreatAs:
                                
                                lData = 0;			//now just 0
                                break;
                            default:
                                AE_ASSERT(false);
                        }
                    }
                    lpInput[size++] = lData;
                }
                
                lpGroupPtr++;
            }//while
            
            if (lGroupValid == DssDataOk && size <= 0)
			{
				/*if (lfAllIgnoreCheckNull)
                 lGroupValid = DssDataIgnoreCheckNull;
                 else*/
                lGroupValid = DssDataInvalid;
			}
            
			if(lGroupValid == DssDataOk) { // caculate this aggregation
				double lResult = 0;
				DSSDataFlag lFlag = DssDataTypeUnknown;
                
				hr = ipFunc->Calculate(size, lpInput, &lResult, &lFlag);
				if (hr != S_OK)
                {
                    return E_FAIL;
                }
				hr = lpSliceOut->setNumericByOffset(iRow, lResult, lFlag);
				CHECK_HR;
			} else {
                double lResult = 0;
				hr = lpSliceOut->setNumericByOffset(iRow, lResult, lGroupValid);
				CHECK_HR;
			}
            
        }
	}
    
	return S_OK;
}

int DSSSliceCalculator::hApplyNonNumericalAggregation(DSSNNAggregationFunction* ipFunc,
                                                      DSSTabularIndex* ipLookupIndex,
                                                      DSSTabularIndex *ipOutputIndex,
                                                      EnumNullCheckingOption iNullCheckingOption,
                                                      int *opResultSliceID,
                                                      CountType iCountType,
                                                      DSSFunctionObject* pCellFmtFunc)
{
    int hr = S_OK;
    DSSNNAggregationFunction* pFunc = ipFunc;
    
    EnumDSSDataType lInputType, lOutputType;
    DSSCubeDataSlice *lpSliceIn = mvSlice[0];
    lInputType = lpSliceIn->getDataType();
    if(lInputType == DssDataTypeBigDecimal || (pCellFmtFunc == NULL && lInputType == DssDataTypeCellFormatData))
    {
        lOutputType = DssDataTypeDouble;
    }
    else if (pCellFmtFunc != NULL)
    {
		lOutputType = DssDataTypeCellFormatData;
	}
    else
    {
        pFunc->getOutputType(lInputType, &lOutputType);
    }
    
    //!!!!Do we need the column length to create the output slice?
    int lnUnit = ipOutputIndex->getNumUnits();
    int *lpUnit = new int[lnUnit];
    MBase::DSSStrongPtr<Int32, MBase::DeleteArray<Int32> > lUnit1Ptr(lpUnit);
    hr = ipOutputIndex->getUnits(lnUnit, lpUnit);
    CHECK_HR;
    
	DSSCubeDataSlice *lpSliceOut = NULL;
	hr = mpContext->mpCube->CreateDataSlice(lnUnit, lpUnit, lOutputType, &lpSliceOut);
	CHECK_PTR(lpSliceOut);
    mpContext->mpCube->InsertDataSlice(lpSliceOut, *opResultSliceID);
    
    int *lpGroupBound = NULL;
    int *lpBlockList = NULL;
    int *lpBlockAddress = NULL;
    
    int lnRow;
    int lnResRow;
    bool lbUseStreamAgg = false;
    DSSXTabBitVector* lpMask = NULL;
	hr = hGroupData(lpSliceIn, ipLookupIndex, lpSliceOut, lbUseStreamAgg,
					lpBlockList, lnRow, lpGroupBound, lnResRow, lpBlockAddress, lpMask);
    CHECK_HR;
    
    if(lnResRow <= 0)
        return S_OK;
    
    //zhyang, 08/31/2011  We wrap lpBlock, lpGroupBound and lpBlockAddress here
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lBlockListPtr(lpBlockList);
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lGroupBoundPtr(lpGroupBound);
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lBlockAddressPtr(lpBlockAddress);
    
    
    
    DSSDataColumn* lDataCol = lpSliceIn->getColumn();
    CHECK_PTR(lDataCol);
    DSSDataColumn* lOutputCol = lpSliceOut -> getColumn();
    CHECK_PTR(lOutputCol);
    
    Int32 lnInputRowSize = 0;
    
    //!!!!For String columns, we need to get the max Column size of it.
    //But I'm not sure whether there is function in StrCol can do this.
    //Just leave it here and find it later.
    //    lDataCol->getRowSize(0, lnInputRowSize);
    //   lDataCol->getMaxRowSize(lnInputRowSize);
    lnInputRowSize = lDataCol->getSizeOfData();
    
    //zhyang, 08/31/2011, it just a pointer to the column data, no need to allocate memory
    BYTE* lpRawData = NULL;
    
    Int32 lnOutputRowSize = 0;
    hr = DSSAEHelper::SizeOfType(lOutputType, &lnOutputRowSize);
    
    BYTE *lpResultData = new BYTE[lnOutputRowSize];
    CHECK_NEWED_PTR(lpResultData);
    
    MBase::DSSStrongPtr<BYTE, MBase::DeleteArray<BYTE> > lResultDataPtr(lpResultData);
    
    BYTE* lDummyResult = new BYTE[lnOutputRowSize];
    CHECK_NEWED_PTR(lDummyResult);
    MBase::DSSStrongPtr<BYTE, MBase::DeleteArray<BYTE> > lDummyResultPtr(lDummyResult);
    memset(lDummyResult, 0, lnOutputRowSize);
    
    vector<CComVariant> lvArrayElements;
    lvArrayElements.reserve(lnRow);
    
    DSSDataFlag lFlag = DssDataUnknown;
    Int32 lpGroupPtr;
    
    Int32 iRow = 0;
    for(iRow = 0; iRow < lnResRow; ++iRow)
    {
        lvArrayElements.clear();
        
        bool IfAllIgnoreCheckNull = true;
        
        Int32 lGroupStart = lpGroupBound[iRow];
        Int32 lGroupEnd = lpGroupBound[iRow + 1];
        
        lpGroupPtr = lGroupStart;
        bool lCalculate = true;
        
        while(lpGroupPtr < lGroupEnd && lCalculate)
        {
            Int32 lInputBlock = lpBlockList[lpGroupPtr];
            
            Int32 lStart = lpBlockAddress? lpBlockAddress[lInputBlock] : lInputBlock;
            Int32 lEnd = lpBlockAddress ? lpBlockAddress[lInputBlock + 1] : lInputBlock + 1;
            
            for(Int32 lInputRow = lStart; lInputRow < lEnd && lCalculate; ++lInputRow)
            {
                Int32 lRetSize = 0;
                EnumDSSDataType lValueType = DssDataTypeDouble;
                
                if(DssDataTypeBigDecimal == lInputType || (pCellFmtFunc == NULL && lInputType == DssDataTypeCellFormatData))
                {
                    hr = lDataCol -> getNumericVal(lInputRow, *((double*)lpRawData), lFlag);
                    CHECK_HR;
                }
                else
                {
                    
                    hr = lDataCol -> getData(lInputRow, ((const char **)&lpRawData), lRetSize, lValueType, lFlag);
                    CHECK_HR;
                }
                
                //!!!!
                //                if(DssdataIgnoreCheckNull == lFlag)
                //                    continue;
                IfAllIgnoreCheckNull = false;
                
                if(DssDataOk == lFlag)
                {
                    CComVariant lData;
                    hr = DFCDataConversion::ConvertToVariant(lpRawData, lValueType, lRetSize, &lData);
                    if(S_OK != hr)
                    {
                        lCalculate = false;
                        hr = lOutputCol -> setData(iRow, (char*)lDummyResult, lnOutputRowSize, lFlag);
                        CHECK_HR;
                        break;
                    }
                    lvArrayElements.push_back(lData);
                }
                else
                {
                    if(DssNumericErrorTreatAs == iNullCheckingOption)
                    {
                        if((DSSAEHelper::IsNumeric(lInputType)) || DssDataTypeBigDecimal == lInputType)
                        {
                            CComVariant lData;
                            lData.vt = VT_I2;
                            lData.iVal = 0;
                            lvArrayElements.push_back(lData);
                        }
                    }
                    else if(DssNumericErrorSkip == iNullCheckingOption)
                    {
                        //Skip the error, do nothing
                    }
                    else
                    {
                        lCalculate = false;
                        hr = lOutputCol->setData(iRow, (char*)lDummyResult, lnOutputRowSize, lFlag);
						CHECK_HR;
						break;
                    }
                }// if(DssDataOk == lFlag)
            }//for(Int32 lInputRow = lStart; lInputRow < lEnd && lCalculate; ++lInputRow)
            ++lpGroupPtr;
        }//while(lpGroupPtr < lGroupEnd && lCalculate)
        
        if(lvArrayElements.empty())
        {
            if(IfAllIgnoreCheckNull)
            {
                //!!!!
                //     lFlag = DssDataIgnoreCheckNull;
            }
            //!!!!
            hr = lOutputCol->setData(iRow, (char*)lDummyResult, lnOutputRowSize, lFlag);
			CHECK_HR;
			
			continue;
        }
        if(lCalculate)
        {
            SAFEARRAYBOUND lBound;
            lBound.lLbound = 0;
            Int32 lArraySize = lvArrayElements.size();
            lBound.cElements = lArraySize;
            
            SAFEARRAY *lpInputData = SafeArrayCreate(VT_VARIANT, 1, &lBound);
            CHECK_NEWED_PTR(lpInputData);
            
            CComVariant lInVariant;
            lInVariant.vt = VT_ARRAY | VT_VARIANT;
            lInVariant.parray = lpInputData;

            // DE49980 weilin 2016/11/24 Int32 => LONG in order to make 64bit environment work
            for(LONG i = 0; i < lArraySize; ++i)
            {
                hr = SafeArrayPutElement(lpInputData, (LONG *)&i, &lvArrayElements[i]);
                CHECK_HR;
            }
            
            CComVariant lResult;
            DSSDataFlag lFlag = DssDataUnknown;
            if (pCellFmtFunc != NULL)
				hr = pCellFmtFunc->CalculateCellData(lArraySize, &lInVariant, &lResult, &lFlag);
			else
                hr = pFunc -> Calculate(lArraySize, &lInVariant, lOutputType, &lResult, &lFlag);
            if (hr != S_OK)
			{
				return	hr = E_FAIL;
			}
            
            hr = hConvertFromVariant(lResult, lOutputType, lpResultData, lnOutputRowSize);
            CHECK_HR;
            lResultDataPtr.Reset(lpResultData);
            
            hr = lOutputCol -> setData(iRow, (char*)lpResultData, lnOutputRowSize, lFlag);
            if(hr != S_OK)
            {
				return hr = E_FAIL;
                
            }
        }
    }//for(iRow = 0; iRow < lnResRow; ++iRow)
    
    return S_OK;
}


int DSSSliceCalculator::hApplyNumericalGenericFunction (DSSGenericFunction *pFunc,
                                                        GenericFunctionType iFuncType,
                                                        DSSTabularIndex *ipLookupIndex, DSSTabularIndex *ipBreakbyIndex,
                                                        bool *ifIsVector, 
                                                        vector<Int32> &ivResSliceLevel,
                                                        EnumDSSDataType  iOutputDataType,
                                                        int *opResultSliceID)
{
    int hr = S_OK;    
    
    DSSCubeDataSlice **lppSliceIn = &mvSlice[0];
	int nSliceIn = mvSlice.size();
    
    DSSCube *lpCube = mpContext->mpCube;
    DSSTabularIndexPool *lpTabularIndexPool = NULL;
    hr = lpCube->getIndexPool(&lpTabularIndexPool);
    CHECK_HR;
    
    DSSTabularIndex *lpOutputIndex = NULL;
    
	//initialize a slice, which is always double now.
	DSSCubeDataSlice *lpSliceOut = NULL;
    if(mbOldRWD)
    {
        lpOutputIndex = ipLookupIndex;
        
        DSSDataColumn *lpColumn = NULL;
        int hr = DSSAEHelper::GenXTabCol(DssDataTypeDouble, lpOutputIndex->Count(), &lpColumn);
        CHECK_PTR(lpColumn);
        
        hr = mpContext->mpCube->CreateDataSlice(lpOutputIndex, lpColumn, &lpSliceOut);
        CHECK_PTR(lpSliceOut);
        
    }else
    {
        hr = lpTabularIndexPool->getIndex(ivResSliceLevel.size(), ivResSliceLevel.size() == 0 ? NULL : &ivResSliceLevel[0], &lpOutputIndex);
        CHECK_HR;
        
        int lnUnit = lpOutputIndex->getNumUnits();
        int *lpUnit = new int[lnUnit];
        MBase::DSSStrongPtr<Int32, MBase::DeleteArray<Int32> > lUnit1Ptr(lpUnit);
        hr = lpOutputIndex->getUnits(lnUnit, lpUnit);
        CHECK_HR;

        hr = mpContext->mpCube->CreateDataSlice(lnUnit, lpUnit, DssDataTypeDouble, &lpSliceOut);
        CHECK_PTR(lpSliceOut);
    }
    mpContext->mpCube->InsertDataSlice(lpSliceOut, *opResultSliceID);
    
    DSSDataColumn* lpResColumn = lpSliceOut->getColumn();
    CHECK_PTR(lpResColumn);
    
    bool lbAllResultDataValid = true;
    
    vector<Int32>** lpvDataByGroup = NULL;
    Int32 lnBreakByRow = 0;
    
    MBase::SimpleBuffer lSimpleBuffer;
	MBase::BufferPtr lBufferPtr(&lSimpleBuffer);
	MBase::Buffer *lpBuffer = lBufferPtr.Get();
	CHECK_NEWED_PTR(lpBuffer);

    hr = hGroupDataForGenericFunction(ipLookupIndex, ipBreakbyIndex, NULL, iFuncType, lpvDataByGroup, lpBuffer, lnBreakByRow);
    CHECK_HR;
    
    Int32 lnVecRow = ipLookupIndex->Count();
    double* lpParamValue = new double[lnVecRow];
    CHECK_NEWED_PTR(lpParamValue);
    MBase::DSSStrongPtr<double, MBase::DeleteArray<double> > lParamValuePtr(lpParamValue);
    
    DSSDataFlag* lpParamFlag = new DSSDataFlag[lnVecRow];
    CHECK_NEWED_PTR(lpParamFlag);
    MBase::DSSStrongPtr<DSSDataFlag, MBase::DeleteArray<DSSDataFlag> > lParamFlagPtr(lpParamFlag);
    
    DSSDataFlag* lpInputValid = new DSSDataFlag[lnVecRow];
    CHECK_NEWED_PTR(lpInputValid);
    MBase::DSSStrongPtr<DSSDataFlag, MBase::DeleteArray<DSSDataFlag> > lInputValidPtr(lpInputValid);
    
    //zhyang, 26/07/2011, change IsNullProcessable param
    bool lbCanProcessNull = false;
    hr = pFunc->IsNullProcessable(&lbCanProcessNull);
    CHECK_HR;
    
    double lData;
    DSSDataFlag lFlag;
    Int32 iElement = 0;
    
    Int32 lnTotalUnits;
    hr = hGetTotalUnits(lnTotalUnits);
    CHECK_HR;
    
    vector<Int32> lvKeys(lnTotalUnits);
    
    EnumNullCheckingOption iNullCheckingOption = mpContext->mNullCheckingOption;
    
    // jxie, 2010-9-14
	// check if we can use direct calculate
	bool lfDirectCalculate = false;
	hr = hCheckDirectCalculate(nSliceIn, lppSliceIn, ipLookupIndex, pFunc, iNullCheckingOption, lfDirectCalculate);
	CHECK_HR;
    
	if (lfDirectCalculate)
	{
		hr = hDirectCalculateForGenericFunction(nSliceIn, lppSliceIn, ipLookupIndex, pFunc, lpSliceOut, NULL, lpvDataByGroup,
                                                lnBreakByRow, iNullCheckingOption, iFuncType);
		if (hr == S_OK)
        {
            //memory leak
            //todo: convert lpvDataByGroup to vector<Int32, MBase::Allocator<Int32> >
            if(lpvDataByGroup)
            {
                for(int i = 0; i < lnBreakByRow;i++)
                {
                    if(lpvDataByGroup[i])
                    {
                        delete lpvDataByGroup[i];//->resize(0);
                    }
                }
                
                delete [] lpvDataByGroup;
            }
			return S_OK;
        }
		// if failed to calculate, let it go to the old approach
	}
    
    DSSSliceCorrelator lSliceCorrelator;
	hr = lSliceCorrelator.Init(ipLookupIndex, nSliceIn, lppSliceIn, lpSliceOut, mpContext->mpCube);
	CHECK_HR;
    
    // Sometimes we need to perform breakby index to slice index translation.
	DSSSliceCorrelator lBreakBySliceCorrelator;
	hr = lBreakBySliceCorrelator.Init(ipBreakbyIndex, nSliceIn, lppSliceIn, lpSliceOut, mpContext->mpCube);
	CHECK_HR;
    
    vector<DSSCubeDataSlice*> lvInputSlice = mvSlice;
    Int32 lnSilceIn = lvInputSlice.size();
    
    for(Int32 iGroup = 0; iGroup < lnBreakByRow; ++iGroup)
    {
        pFunc->Clear();
        bool lbSkipCalculation = false;
        bool lbHasSkipNull = false;
        
        Int32 groupSize = lpvDataByGroup[iGroup]->size();
        for(Int32 iElement = 0; iElement < groupSize; ++iElement)
        {
            lpInputValid[iElement] = DssDataOk;
        }
        
        for(Int32 iSlice = 0; iSlice < lnSilceIn; ++iSlice)
        {
            if(ifIsVector[iSlice])
            {
                Int32 count = 0;
                for(Int32 iElement = 0; iElement < groupSize; ++iElement)
                {
                    //                    hr = lvInputSlice[iSlice]->getNumericByOffset((lpvDataByGroup[iGroup])[iElement], lData, lFlag);
                    hr = lSliceCorrelator.GetInputValue(iSlice, (*lpvDataByGroup[iGroup])[iElement], &lData, &lFlag);
                    CHECK_HR;
                    
                    if(DssDataOk == lFlag)
                    {
                        lpParamValue[count] = lData;
                        lpParamFlag[count] = lFlag;
                        ++count;
                    }
                    else // lFlag is not DssDataOK;
                    {/*
                      if(DssDataIgnoreCheckNull == lFlag)
                      {
                      lpInputValid[iElement] = lFlag;
                      lbHasSkipNull = true; 
                      }*/
                        if(true == lbCanProcessNull)
                        {
                            lpParamValue[count] = lData;
                            lpParamFlag[count] = lFlag;
                            ++count;
                        }
                        else if(DssNumericErrorTreatAs == iNullCheckingOption)
                        {
                            lpParamValue[count] = 0;
                            lpParamFlag[count] = DssDataOk;
                            ++count;
                        }
                        else
                        {
                            lpInputValid[iElement] = DssDataInvalid;
                        }
                    }
                }
                
                if(0 == count)
                {
                    lbSkipCalculation = true;
                }
                else
                {
                    hr = pFunc->SetInputParam(iSlice+1, count, lpParamValue, lpParamFlag);
                    CHECK_HR
                }
            }
            else // if(ifIsVector[iSlice])  scalar
            {
                // aye, have to validate that ipBreakbyIndex == ipLookupIndex at the moment
				// weiwang 2011-03-15 TQMS464324 handle ipBreakbyIndex != ipLookupIndex
                if(groupSize > 0 && ipBreakbyIndex != ipLookupIndex)
                {
                    //                    hr = lvInputSlice[iSlice]->getNumericByOffset((lpvDataByGroup[iGroup])[0], lData, lFlag);
                    hr = lSliceCorrelator.GetInputValue(iSlice, (*lpvDataByGroup[iGroup])[0], &lData, &lFlag);
                    CHECK_HR;
                }
                else
                {
                    //                    hr = lvInputSlice[iSlice]->getNumericByOffset(iGroup, lData, lFlag);
                    hr = lBreakBySliceCorrelator.GetInputValue(iSlice, iGroup, &lData, &lFlag);
                    CHECK_HR;
                }
                
                /*
                 if (lFlag == DssDataIgnoreCheckNull) {
                 
                 lbSkipCalculation = true;
                 lbHasSkipNull = true;
                 
                 } */
                if(DssDataOk == lFlag || true == lbCanProcessNull)
                {
                    hr = pFunc->SetInputParam(iSlice+1, 1, &lData, &lFlag);
                    CHECK_HR;
                }
                else if(DssNumericErrorTreatAs == iNullCheckingOption)
                {
                    lData = 0;
                    lFlag = DssDataOk;
                    hr = pFunc->SetInputParam(iSlice+1, 1, &lData, &lFlag);
                    CHECK_HR;
                }
                else
                {
                    lbSkipCalculation = true;
                }
            }
        }
        
        if(!lbSkipCalculation)
        {
            hr = pFunc->Calculate();
            if (hr != S_OK)
            {
                return E_FAIL;
            }
            Int32 lnResultSize = 0;
            if(eGenericType2 == iFuncType)
            {
                hr = pFunc->GetOutput(groupSize, &lnResultSize, lpParamValue, lpParamFlag);
                CHECK_HR;
                
                if(lnResultSize > groupSize)
                {
                    return E_FAIL;
                }
                
                Int32 count = 0;                
                for(iElement = 0; iElement < groupSize; iElement ++){
					DSSDataFlag lFlag = lpInputValid[iElement];
                    
					double lValue = 0;
					if (lFlag == DssDataOk)
					{
						lFlag = lpParamFlag[count];
						lValue = lpParamValue[count];
						count++;
					}
					
                    //                    lpSliceOut->setNumericByOffset((lpvDataByGroup[iGroup])[iElement], lValue, lFlag);
                    hr = lSliceCorrelator.SetOutputValue((*lpvDataByGroup[iGroup])[iElement], lValue, lFlag);
					CHECK_HR;
				} //for
            }
            else //(eGenericType2 == iFuncType)
            {
                hr = pFunc->GetOutput(1, &lnResultSize, &lData, &lFlag);
                CHECK_HR;
                
                if(1 != lnResultSize)
                {
                    return E_FAIL;
                }
                hr = lpResColumn->setNumeric(iGroup, lData, lFlag);
                CHECK_HR;
            }
        }
        else if(lbAllResultDataValid || lbHasSkipNull)
        {
            DSSDataFlag lFlag = DssDataInvalid;
            
            if(lbHasSkipNull)
            {
                //!!!!    lFlag = DssdataIngoreNullCheck;
            }
            
            if(eGenericType2 == iFuncType)
            {
                for(iElement = 0; iElement < groupSize; ++iElement)
                {
                    //                    hr = lpSliceOut->setNumericByOffset((lpvDataByGroup[iGroup])[iElement], lData, lFlag);
                    hr = lSliceCorrelator.SetOutputValue((*lpvDataByGroup[iGroup])[iElement], lData, lFlag);
                    CHECK_HR
                }
            }
            else
            {
                hr = lpResColumn->setNumeric(iGroup, lData, lFlag);
                CHECK_HR;
            }
            lbAllResultDataValid = false;
        }
    }//for(Int32 iGroup = 0; iGroup < lnBreakByRow; ++iGroup)

    if(lpvDataByGroup)
    {
        for(int i = 0; i < lnBreakByRow;i++)
        {
            if(lpvDataByGroup[i])
            {
                delete lpvDataByGroup[i];
            }
        }
        
        delete [] lpvDataByGroup;
    }

    return hr = S_OK;
}

int  DSSSliceCalculator::hApplyNonNumericalGenericFunction (DSSNNGenericFunction *pFunc,
                                                            GenericFunctionType iFuncType,
                                                            DSSTabularIndex *ipLookupIndex,
                                                            DSSTabularIndex *ipBreakbyIndex,
                                                            bool *ifIsVector,
                                                            vector<Int32> &ivResSliceLevel,
                                                            EnumDSSDataType  iOutputDataType,
                                                            int *opResultSliceID,
                                                            bool bSupportCellFormat)
{
    int hr = S_OK;
    
    DSSCubeDataSlice **lppSliceIn = &mvSlice[0];
	int nSliceIn = mvSlice.size();
	EnumNullCheckingOption iNullCheckingOption = mpContext->mNullCheckingOption;
    
    if (iOutputDataType == DssDataTypeBigDecimal || (!bSupportCellFormat && iOutputDataType == DssDataTypeCellFormatData))
    {
		iOutputDataType = DssDataTypeDouble;
	}
    
    // jimzhang Aug/01/2011 : Temperary fix for RunningCount(). Maybe DSSAEHelper::GenXTabCol() need to be modified later.
    /*
    if (iOutputDataType == DssDataTypeLong)
    {
        iOutputDataType = DssDataTypeDouble;
    }*/
    
    // the only non-numerical type we are supporting now is date time.
	//DFCData_VriantSize() is not used here because of incomplete support of string
    Int32 lByte = 0;
	hr = DSSAEHelper::SizeOfType(iOutputDataType, &lByte);
	CHECK_HR;
    
	//initialize a slice, which is always double now.
	DSSCubeDataSlice *lpSliceOut = NULL;
	hr = mpContext->mpCube->CreateDataSlice(ivResSliceLevel.size(), &ivResSliceLevel[0], iOutputDataType, &lpSliceOut);
	CHECK_PTR(lpSliceOut);
    mpContext->mpCube->InsertDataSlice(lpSliceOut, *opResultSliceID);
    
    DSSDataColumn *lpResultCol = lpSliceOut->getColumn();
    
	bool lbAllResultDataValid = true;
	// aye
	// call the helper function to group data together
	Int32 lnBreakByRow = 0;
    vector<Int32> **lpvDataByGroup = NULL;
    
    // qjiang 07/28/2009 TQMS 361103
	// special treatment for First/Last
    EnumSimpleOperatorType lType = pFunc->getOperatorType();
    if (lType == DssFuncFirst || lType == DssFuncLast)
    {
        assert(nSliceIn == 1);
        // skip sort if it is First or Last, we only need the group info
        return hCalculateFirstLast(ipLookupIndex, &(mpContext->mvSortbyDefn), lppSliceIn[0], lpSliceOut, lType == DssFuncFirst);
    }
    
    MBase::SimpleBuffer lSimpleBuffer;
	MBase::BufferPtr lBufferPtr(&lSimpleBuffer);
	MBase::Buffer *lpBuffer = lBufferPtr.Get();
	CHECK_NEWED_PTR(lpBuffer);
    
    hr = hGroupDataForGenericFunction(ipLookupIndex, ipBreakbyIndex, NULL, iFuncType, lpvDataByGroup, lpBuffer, lnBreakByRow);
    CHECK_HR;
    
	DSSSliceCorrelator lSliceCorrelator;
	hr = lSliceCorrelator.Init(ipLookupIndex, nSliceIn, lppSliceIn, lpSliceOut, mpContext->mpCube);
	CHECK_HR;
    
    // Sometimes we need to perform breakby index to slice index translation.
	DSSSliceCorrelator lBreakBySliceCorrelator;
	hr = lBreakBySliceCorrelator.Init(ipBreakbyIndex, nSliceIn, lppSliceIn, lpSliceOut, mpContext->mpCube);
	CHECK_HR;
    
    // Prepare the temporary vectors for calculation
	Int32 lnVecRow = ipLookupIndex->Count();
    
	Int32 lnTotalUnits = 0;
	hr = hGetTotalUnits(lnTotalUnits);
	CHECK_HR;
    
	Int32 * lpIndex = new Int32[lnTotalUnits];
	CHECK_NEWED_PTR(lpIndex);
    MBase::DSSStrongPtr<Int32, MBase::DeleteArray<Int32> > lIndexPtr(lpIndex);
    
	//Initilize raw data space
	BYTE **ppRawData = new BYTE *[nSliceIn];
	CHECK_NEWED_PTR(ppRawData);
    MBase::DSSStrongPtr<BYTE *, MBase::DeleteArray<BYTE *> > ppRawDataStrongPtr(ppRawData);
    
	Int32 *pCbSize = new Int32 [nSliceIn];
	CHECK_NEWED_PTR(pCbSize);
    MBase::DSSStrongPtr<Int32, MBase::DeleteArray<Int32> > lCbSizePtr(pCbSize);
    
    Int32 iSlice = 0;
    // jimzhang Aug/01/2011 : Don't handle "BigDecimal" currently.
     for (iSlice = 0; iSlice < nSliceIn; iSlice ++)
     {
         hr = lppSliceIn[iSlice]->getSizeOfData(pCbSize[iSlice]);
         CHECK_HR;
     
         ppRawData[iSlice] = NULL;
	}
    
    DSSDataFlag *lpParamFlag = new DSSDataFlag[lnVecRow];
	CHECK_NEWED_PTR(lpParamFlag);
    MBase::DSSStrongPtr<DSSDataFlag, MBase::DeleteArray<DSSDataFlag> > lParamFlagPtr(lpParamFlag);
    
	DSSDataFlag *lpInputValid = new DSSDataFlag[lnVecRow];
	CHECK_NEWED_PTR(lpInputValid);
    MBase::DSSStrongPtr<DSSDataFlag, MBase::DeleteArray<DSSDataFlag> > lInputValidPtr(lpInputValid);
    
    //zhyang,26/07/2011, change IsNullProssable param
    bool lbCanProcessNULL = false;
	hr = pFunc->IsNullProcessable(&lbCanProcessNULL);
	CHECK_HR;
    
    DSSDataFlag lFlag;
	Int32 iElement = 0;
	//for each group, set parameter, calculate and put the reusult into outSlice.
	for(Int32 iGroup = 0; iGroup < lnBreakByRow; iGroup ++){
		//if (lpMask && lpMask->Get(iGroup) == 0)
		//	continue;
        
		bool lbSkipCalculation = false;
		bool lbHasSkipNull = false;
        
		pFunc->Clear(); //clear inner states of function
        
		Int32 groupSize = lpvDataByGroup[iGroup]->size();
		for (iElement = 0; iElement < groupSize; iElement ++){
			lpInputValid[iElement] = DssDataOk;
		}
        
		for(iSlice = 0; iSlice < nSliceIn; iSlice ++){
			if(ifIsVector[iSlice]){ // this is a vector
				//create a one-dimensional array for input parameter[size]
				SAFEARRAYBOUND lBound;
				lBound.lLbound = 0;
				lBound.cElements = groupSize;
				SAFEARRAY *lpInData = SafeArrayCreate(VT_VARIANT, 1, &lBound);
				CHECK_NEWED_PTR(lpInData);
                
				CComVariant lInVariant;
				lInVariant.vt = VT_ARRAY|VT_VARIANT;
				lInVariant.parray = lpInData;
                
				EnumDSSDataType valueType;
				LONG count = 0;
				for(iElement = 0; iElement < groupSize; iElement ++){
					Int32 lInputRow = (*lpvDataByGroup[iGroup])[iElement];
					CHECK_HR;
                    
					Int32 retSize = 0;
                    
                    valueType = lppSliceIn[iSlice]->getDataType();
                    
					if (valueType == DssDataTypeBigDecimal || (valueType == DssDataTypeCellFormatData && !bSupportCellFormat))
                    {
						valueType = DssDataTypeDouble;
                        
                        hr = lSliceCorrelator.GetInputValue(iSlice, lInputRow, (double*)ppRawData[iSlice], &lFlag);
						CHECK_HR;
					}
                    else
                    {
                        hr = lSliceCorrelator.GetInputValue(iSlice, lInputRow, &ppRawData[iSlice], pCbSize[iSlice], &retSize, &valueType, &lFlag);
                        
						CHECK_HR;
					}
                    
					CComVariant lData;
					//if flag is not OK, just skip the data
					if (lFlag == DssDataOk)
                    {
						hr = DFCDataConversion::ConvertToVariant(ppRawData[iSlice], valueType, retSize, &lData);
						if (hr != S_OK){
							lbSkipCalculation = true;
							break;
						}
                        
						hr = SafeArrayPutElement(lpInData, (LONG*)&count, &lData);
						_ASSERT(hr == S_OK);
						if (hr != S_OK){
                            return hr;
						}
                        
						lpParamFlag[count] = DssDataOk;
                        
						count ++;
                        
					}
                    // jimzhang July/13/2011 : "DssDataIgnoreCheckNull" is not supported on clinet now
                    /*
                     else if (lFlag == DssDataIgnoreCheckNull) { // qjiang Add support for new falg
                     
                     lbHasSkipNull = true;
                     
                     lpInputValid[iElement] = lFlag;
                     
                     }*/
                    else
                    {
						lData.vt = VT_I2;
						lData.iVal = 0;
						hr = SafeArrayPutElement(lpInData, (LONG*)&count, &lData);
                        
						_ASSERT(hr == S_OK);
						if (hr != S_OK){
                            return hr;
						}
                        
						if (lbCanProcessNULL == true){ //the plug in function wants NULL
							//ytang 2008-06-25 TQMS313044
							lpParamFlag[count] = lFlag;
							count ++;
						} else 	if((iNullCheckingOption == DssNumericErrorTreatAs) &&
                                   (DSSAEHelper::IsNumeric(valueType))){
							lpParamFlag[count] = DssDataOk;
							count ++;
                            
						} else { // Neither does the plug-in function want NULL nor treat NULL as 0
							lpInputValid[iElement] = DssDataInvalid;
						}
                        
                        
					} //if lFlag == DssDataOk
				} // for iElement
                
				if (count == 0)
                {
					lbSkipCalculation = true;
				} else {
					hr = pFunc -> SetInputParam(iSlice + 1, count, valueType, &lInVariant, lpParamFlag);
					CHECK_HR;
				}
			}
            else
            { //scalar
                hr = ipBreakbyIndex -> Item(iGroup, lnTotalUnits, lpIndex);
				CHECK_HR;
                
				Int32 retSize = 0;
				EnumDSSDataType valueType = lppSliceIn[iSlice]->getDataType();
                
				//464324
				if (groupSize>0 && ipBreakbyIndex != ipLookupIndex)
				{
					Int32 lInputRow = (*lpvDataByGroup[iGroup])[0];
					if (valueType == DssDataTypeBigDecimal) {
						valueType = DssDataTypeDouble;
                        
                        hr = lSliceCorrelator.GetInputValue(iSlice, lInputRow, (double*)ppRawData[iSlice], &lFlag);
                        CHECK_HR;
					}
                    else
                    {
                        hr = lSliceCorrelator.GetInputValue(iSlice, lInputRow, &ppRawData[iSlice], pCbSize[iSlice], &retSize, &valueType, &lFlag);
						CHECK_HR;
					}
				}
                else
				{
					if (valueType == DssDataTypeBigDecimal)
                    {
						valueType = DssDataTypeDouble;
                        
                        hr = lBreakBySliceCorrelator.GetInputValue(iSlice, iGroup, (double*)ppRawData[iSlice], &lFlag);
						CHECK_HR;
					}
                    else
                    {
                        hr = lBreakBySliceCorrelator.GetInputValue(iSlice, iGroup, &ppRawData[iSlice], pCbSize[iSlice], &retSize, &valueType, &lFlag);
						CHECK_HR;
					}
				}
				CComVariant lData;
                
				if (lFlag == DssDataOk){
                    
					hr = DFCDataConversion::ConvertToVariant(ppRawData[iSlice], valueType, retSize, &lData);
					if (hr != S_OK){
                        return hr;
					}
                    
					hr = pFunc -> SetInputParam(iSlice + 1, 1, valueType , &lData, &lFlag);
					CHECK_HR;
				}
                // jimzhang July/13/2011 : "DssDataIgnoreCheckNull" is not supported on client now
                /*
                 else if (lFlag == DssDataIgnoreCheckNull)
                 {
                 
                 lbSkipCalculation = true;
                 lbHasSkipNull = true;
                 
                 }*/
                else
                {
					if (lbCanProcessNULL == true)
                    { // set a dummy
                        hr = pFunc -> SetInputParam(iSlice + 1, 1 , valueType, &lData, &lFlag);
                        CHECK_HR;
                        
					}
                    else if(iNullCheckingOption == DssNumericErrorTreatAs && DSSAEHelper::IsNumeric(valueType))
                    {
						//set input as 0
						lData.vt = VT_I2;
						lData.iVal = 0;
                        
						lFlag = DssDataOk;
						hr = pFunc -> SetInputParam(iSlice + 1, 1 , valueType, &lData, &lFlag);
						CHECK_HR;
					}
                    else
                    {
                        lbSkipCalculation = true;
					}
				} // if (lFlag == DssDataOk)
			}
		} //for iSlice
        
		if (!lbSkipCalculation){ //do the calculation
			hr = pFunc -> Calculate();
			if (hr != S_OK)
            {
				return hr;
            }
            
			//get the output and set it into the result column
			Int32 lResultBufLen = lByte;
			BYTE *lpResultData = new BYTE[lResultBufLen];
			//CHECK_HR_PTR(lpResultData);
            CHECK_PTR(lpResultData);
            MBase::DSSStrongPtr<BYTE, MBase::DeleteArray<BYTE> > lResultDataPtr(lpResultData);
            
			Int32 lnResultSize = 0;
			CComVariant lResult;
			if (iFuncType == eGenericType2) { // ASSERT: resultSlice.rowNum == lookupIndex.rowNum
				hr = pFunc->GetOutput(lpvDataByGroup[iGroup]->size(), &lnResultSize,
                                      &lResult, lpParamFlag);
				if(hr != S_OK ||
                   (lResult.vt & (VT_ARRAY | VT_VARIANT) ) != (VT_ARRAY | VT_VARIANT) ||
                   lnResultSize > lpvDataByGroup[iGroup]->size()) {
                    return hr;
					//AE_MSG xhan result and input type mismatch or size mismatch
				}
                
				Int32 count = 0;
				for(iElement = 0; iElement < groupSize; iElement ++){
					Int32 lInputRow = (*lpvDataByGroup[iGroup])[iElement];
                    
					if (lpInputValid[iElement] == DssDataOk) {
						if (lpParamFlag[count] == DssDataOk) {
                            
							CComVariant resultElement;
							SafeArrayGetElement(lResult.parray, (LONG*)&count, &resultElement);
                            
							hr = hConvertFromVariant(resultElement, iOutputDataType,
                                                     lpResultData, lResultBufLen);
                            lResultDataPtr.Reset(lpResultData);
							CHECK_HR;
						}
                        
                        hr = lSliceCorrelator.SetOutputValue(lInputRow, lpResultData, lResultBufLen, iOutputDataType, lpParamFlag[count]);
						CHECK_HR;
                        
						if(hr != S_OK){
                            return hr;
						}
						count ++;
					} else { //input is invalid, set a dummy to the cell
                        hr = lSliceCorrelator.SetOutputValue(lInputRow, lpResultData, lByte, iOutputDataType, lpInputValid[iElement]);
						CHECK_HR;
                        
						if(hr != S_OK){
                            return hr;
						}
                        
					} //lpInputValid[iElement] == DssDataOk
				} //for
			} else { //Type 1 scalar Assert: resultSlice.index == breakByIndex
				hr = pFunc->GetOutput(1, &lnResultSize, &lResult, &lFlag);
				if(hr != S_OK || lnResultSize != 1) {
                    return hr;
					//AE_MSG xhan GetOutput() fails
				}
                
				if (lFlag == DssDataOk) {
					hr = hConvertFromVariant(lResult, iOutputDataType,  lpResultData, lResultBufLen);
                    lResultDataPtr.Reset(lpResultData);
					CHECK_HR;
				}
                
                hr = lpResultCol->setData(iGroup, (char *)lpResultData, lResultBufLen, lFlag);
				if(hr != S_OK)
                {
                    return hr;
                }
			} //if vector/scalar
            
		} //if bcaculationskip
		else if (lbAllResultDataValid || lbHasSkipNull)
		{
			DSSDataFlag lFlag = DssDataInvalid;
            // jimzhang July/13/2011 : "DssDataIgnoreCheckNull" is not supported on client now.
            /*
             if (lbHasSkipNull)
             lFlag = DssDataIgnoreCheckNull;*/
            
			//lwang, 383444, set result flag to invalid so that mAllDataValid will be checked. It's only need to be done once
			if (iFuncType == eGenericType2)
			{
				//04/24/2010, 400443
				for(iElement = 0; iElement < groupSize; iElement ++)
				{
					Int32 lInputRow = (*lpvDataByGroup[iGroup])[iElement];
                    hr = lSliceCorrelator.SetOutputValue(lInputRow, 0, 0, iOutputDataType, lFlag);
					CHECK_HR;
                    
					if(hr != S_OK){
                        return hr;
						//AE_MSG xhan result column setData() failes
					}
				} //for
			}
			else
			{
                hr = lpResultCol->setData(iGroup, 0, 0, lFlag);
                
                if(hr != S_OK)
                {
                    return hr;
                }
			}
			lbAllResultDataValid = false;
		}
	} //for igroup
    
    return hr = S_OK;
}

// aye, move the common part of LinearScanNumeric and LinearScanVariant here
// The engineer who wrote the LinearScanVariant shouldn't have copy-pasted it to start with.

// aye, 4/1/09, stream agg support
// In case iUseStreamAgg is called, the meaning of the output is changed
// 1) orBlockList, a vector of group number, i.e., the output row assignment
// 2) orInputRows, the number of input rows. If filter exists, this is the number that passes the filter.
// 3) orGroupBounds, not used, always NULL
// 4) orOutputRows, the number of output rows
// 5) orBlockAddress, when no view filter exists, this is NULL. Otherwise it has orInputRows elements,
//    which is the input row assignment.
int DSSSliceCalculator::hGroupData(DSSCubeDataSlice* ipSliceIn,
                                   DSSTabularIndex* ipLookupIndex,
                                   DSSCubeDataSlice* ipSliceOut,
                                   bool iUseStreamAgg,
                                   int* &orBlockList, int &orInputRows,
                                   int* &orGroupBounds, int &orOutputRows,
                                   Int32* &orBlockAddress,
                                   DSSXTabBitVector* &orMask,
                                   bool ifUseLookupAsInput
                                   )
{
	int hr = S_OK;
	
	DSSTabularIndex *lpResIndex = ipSliceOut->getIndices();
    orOutputRows = lpResIndex->Count();
    
	if (orOutputRows <= 0)
		return hr = S_OK;
    
    orInputRows = ipLookupIndex->Count();
    
    DSSTabularIndex *lpInputIndex = ipSliceIn->getIndices();
    DFCSimpleExpr* lpCondition = NULL;
    
	lpCondition = ipSliceIn->GetCondition();
    
	if (ifUseLookupAsInput)
	{
		lpInputIndex = ipLookupIndex;
	}
	else
	{
		lpInputIndex = ipSliceIn->getIndices();
		if(!lpInputIndex)//931 i-server
        {
            int lnRows = ipSliceIn->getColumn()->getRowCount();
            // Veracode: CWE ID 191: Integer Underflow (Wrap or Wraparound)
            if (lnRows < 0) lnRows = 0;
            // Veracode
            orInputRows = lnRows;
            orOutputRows = 1;
            
            orBlockList = new int[lnRows];
            for (int i = 0; i < lnRows; i++)
            {
                orBlockList[i] = i;
            }
            
            orGroupBounds = new int[2];
            orGroupBounds[0] = 0;
            orGroupBounds[1] = lnRows;
            return S_OK;
        }
	}
    
    // qjiang 08/16/2010 TQMS 427109
	// no condition? try the main filter
    if (mpContext->mpMaskInfos && NULL == lpCondition
        //don't reset mask for aggregation in conditional logic, those masks don't have condition info
        && !mpContext->mpMaskInfos->GetConditionalLogicFlag())
        mpContext->mpMaskInfos->Reset();
    
    int* lpInputRowMap = NULL;
    int* lpOutputRowMap = NULL;
    mpContext->mpCube->GetIndexRowMap(ipLookupIndex, lpInputIndex, &lpInputRowMap);
    mpContext->mpCube->GetIndexRowMap(ipLookupIndex, lpResIndex, &lpOutputRowMap);
    
    Int32 *lpInputRowAssignments = NULL, *lpOutputRowAssignments = NULL;
    
    Int32 lnResUnit = lpResIndex->getNumUnits();
    vector<Int32> lvResUnit(lnResUnit);
    hr = lpResIndex->getUnits(lnResUnit, &lvResUnit[0]);
    CHECK_HR;
    
    vector<Int32> *lpvGroup = new vector<Int32>[orOutputRows];
    CHECK_NEWED_PTR(lpvGroup);
    MBase::DSSStrongPtr<vector<Int32>, MBase::DeleteArray<vector<Int32> > > lGroupPtr(lpvGroup);
    
    int lnTotalUnit = 0;
    hr = hGetTotalUnits(lnTotalUnit);
    CHECK_HR;
    vector<Int32> lvKeys(lnTotalUnit);
    Int32 lOffset = 0;
    
    lpInputRowAssignments = new Int32[orInputRows];
    CHECK_NEWED_PTR(lpInputRowAssignments);
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lpInputRowAssignmentsPtr(lpInputRowAssignments);
    
    lpOutputRowAssignments = new Int32[orInputRows];
    CHECK_NEWED_PTR(lpOutputRowAssignments);
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lpOutputRowAssignmentsPtr(lpOutputRowAssignments);
    
    orGroupBounds = new Int32[orOutputRows+1];
    int iRow = 0;
    for (iRow = orOutputRows;  iRow >= 0; iRow--)
        orGroupBounds[iRow] = 0;
    
    bool lDoSort = mpContext->mvSortbyDefn.size() != 0 ? true : false;
    if(lDoSort)
    {
        hr = hSortIndex(ipLookupIndex, &(mpContext->mvSortbyDefn), ipSliceIn);
        CHECK_HR;
    }
    
    DSSXTabBitVector *lpNewOutputMask = NULL;
    DSSXTabBitVector *lpInputMask = NULL;
    
    if (mpContext->mpMaskInfos && ! mpContext->mpMaskInfos->IsEmpty())
	{
		bool lCalculated = false;
		hr = mpContext->mpMaskInfos->GetCalculatedMask(ipLookupIndex, &lpInputMask);
		CHECK_HR;
        
		// Check if we have the lpOutputMask already. If not, create one and set it in our iteration
		hr = mpContext->mpMaskInfos->GetMask(lpResIndex, &orMask, &lCalculated);
		CHECK_HR;
        
		// if not calculated, do it our next iteration over the data
		if (!lCalculated)
			lpNewOutputMask = orMask;
	}
    
    bool lUseMask = (mpSortIndex == NULL && lpInputMask);
    
    
    if (lUseMask)
	{
		lpInputMask->SetSeekPosition(0);
		iRow = lpInputMask->GetNextSetPosition();
	}
    else
        iRow = 0;
    
    DSSXTabKeyIterator lInputIt(ipLookupIndex, lpInputIndex);
    DSSXTabKeyIterator lResultIt(ipLookupIndex, lpResIndex);
    int lnInputs = 0;
    
    while (iRow >= 0 && iRow < orInputRows)
    {
        Int32 lRow = iRow;
		if (!lUseMask)
		{
			lRow = SortedIndex(iRow);
			iRow++;
			if (lpInputMask && !lpInputMask->Get(lRow))
				continue;
		}
		else
		{
			// lRow is set in the mask, we just prepare for the next position
			iRow = lpInputMask->GetNextSetPosition();
		}
        
        if (lpOutputRowMap)
        {
            lOffset = XTabIndexRowMapLookup(lpOutputRowMap, lRow);
        }
        else
        {
            hr = lResultIt.LookupTargetRow(lRow, &lOffset);
            CHECK_HR;
        }
        if (lpNewOutputMask)
            lpNewOutputMask->Set(lOffset, true);
        
        // aye, 6/25/2009, Block handling
		// Now that we have found that lRow <==> lOffset
		Int32 lInputColOffset = 0;
		if (lpInputRowMap)
        {
            lInputColOffset = XTabIndexRowMapLookup(lpInputRowMap, lRow);
        }
        else
        {                
            hr = lInputIt.LookupTargetRow(lRow, &lInputColOffset);
            CHECK_HR;
        }       
        
        // Record the row assignment and increment what is now the group size, but will
		//   in the end store the row offsets for the groups
		// mpittkin : 314040, 06/2008 - Change so we record each input/output row pairing,
		// to preserve sort order and handle MD anomaly of duplicate input rows.
		lpInputRowAssignments[lnInputs] = lInputColOffset;
		lpOutputRowAssignments[lnInputs] = lOffset;
        lnInputs++;
        
        orGroupBounds[lOffset]++;
    }
    
    orBlockList = new Int32[orInputRows];
    
    // calculate running sum to get the end cell for each group
	for(iRow = 1; iRow < orOutputRows; iRow++)
		orGroupBounds[iRow] += orGroupBounds[iRow - 1];
	orGroupBounds[orOutputRows] = orGroupBounds[orOutputRows - 1];
    
	// mpittkin, 314040, 06/2008
	// Use new way of recording each input/output row assignment individually first, to preserve sort order
	// and handle possible metadata anomaly where there are duplicate input rows
	for(Int32 i = lnInputs - 1; i >= 0; i--)
	{
		Int32 *lpListEnd = &orGroupBounds[lpOutputRowAssignments[i]];
		(*lpListEnd)--;
		orBlockList[*lpListEnd] = lpInputRowAssignments[i];
	}
    
	return hr = S_OK;
}

int  DSSSliceCalculator::hGetGenericLevels(DSSGenericFunction *ipFunc,
                                           DSSNNGenericFunction *ipNNFunc,
                                           vector<int> &ovTrueBreakby,
                                           vector<int> &ovLookupUnits,
                                           GenericFunctionType &oFuncType, bool *&ofIsVector,
                                           EnumDSSDataType &oOutputDataType)
{
    int hr = S_OK;
    
    AE_ASSERT(ipNNFunc || ipFunc);
    
    bool lisIfFunc = false;
	if(ipNNFunc)
	{
        EnumSimpleOperatorType lOpType = ipNNFunc->getOperatorType();
        if (lOpType == DssFuncIF)
        {
            lisIfFunc = true;
        }
	}
    
    // 1. BreakbyLevel = ResolveAggLevel(TrueLevel (Union(Lv1, Lv2)), L)
    int nSliceIn = mvSlice.size();
    ofIsVector = new bool[nSliceIn];
	CHECK_NEWED_PTR(ofIsVector);
    
    Int32 iSlice = 0;
	for (iSlice = 0; iSlice < nSliceIn; iSlice ++)
    {
		EnumDSSParameterType type = DssParameterReserved;
        if (ipFunc)
			hr = ipFunc->GetInputParamType(iSlice + 1, &type);
		else
			hr = ipNNFunc->GetInputParamType(iSlice + 1, &type);
        
		CHECK_HR;
        
		ofIsVector[iSlice] = (type == DssParameterVector);
	}
    
    if(mpContext)//check schema inside hTrueBreakByLevel, hard code to support 941 backward compatibility
    {
        hr = hTrueBreakByLevel(mpContext->mvBreakbyUnits, nSliceIn, &(mvSlice[0]), ofIsVector, ovTrueBreakby);
        CHECK_HR;
    }
    // 2. LookupUnitSpace = Union(Lv1, Lv2, Ls, BreakbyLevel)
    vector<Int32> lvUnits;
	hr = hSliceLevelUnion(nSliceIn, &(mvSlice[0]), lvUnits);
	CHECK_HR;
    
    // jimzhang Aug/22/2011
    // The third paramter(ifHasSort) is hard coded to "false" temporarily.
    // It should be assigned by input paramter of hGetGenericLevels()
    // which indicates whether sort is defined later.
    // fali, 11/27/2013, 816727, get ifHasSort based on the SortByDefinations
    bool lbHasSort = (mpContext->mvSortbyDefn).size()==0 ? false : true;
    hr = hResolveLookupUnits(ovTrueBreakby,lvUnits, lbHasSort, ovLookupUnits);
	CHECK_HR;
    
    // 3. check generic type
	//ctang 2010-6-10 TQMS 410488 check whether all the data type is numeric
    bool bAllNumerical = true;
	for (Int32 i = 0; i < nSliceIn; i ++)
	{
        bool bNumericalSlice;
        bool bCellFormatSlice;
        hr = hIsNumericalSlice(mvSlice[i], bNumericalSlice, bCellFormatSlice);
        CHECK_HR;
        
		if (!bNumericalSlice)
        {
            bAllNumerical = false;
            break;
        }
	}
    
    EnumDSSParameterType lOutputParamType = DssParameterReserved;
    bool bIsNNFunctionOnly = ipNNFunc != NULL ? ipNNFunc->IsNNFunctionOnly() : false;
    if (ipFunc/*ctang 2010-6-10*/ && bAllNumerical && !bIsNNFunctionOnly){
		hr = ipFunc->GetOutputType(&lOutputParamType);
		CHECK_HR;
        
		oOutputDataType = DssDataTypeDouble;
	}
	else  /*ctang TQMS 429487*/if (ipNNFunc)
	{
		EnumDSSDataType *inputType = new EnumDSSDataType[nSliceIn];
		CHECK_NEWED_PTR(inputType);
        //tliao, 912747, 2014/08/04, fix memory leak
        MBase::DSSStrongPtr<EnumDSSDataType> lInputTypePtr(inputType);
        
		for(iSlice = 0; iSlice < nSliceIn; iSlice ++)
        {
            inputType[iSlice] = mvSlice[iSlice]->getDataType();
            
			//ykiryakov Cobes for AH reports temporary fix
			if(lisIfFunc && iSlice == 2)
			{
				// aye, 316477, 6/25/2008
				// Change the way to get the constant value
				// if the slice has only 1 row and it's NULL, ignore its datatype
				// e.g., the return type of '(cond)? A : null' should only depends on A.
				DSSDataColumn *lpColumn;
                lpColumn = mvSlice[iSlice]->getColumn();
                //CHECK_PTR(lpColumn);
                
                Int32 lCount = 0;
                if(!lpColumn)
                {
                    lCount = 1;
                }
                else
                {
                    lCount = lpColumn->getRowCount();
                }
                
				if (lCount == 1)
				{
					EnumDSSDataType lValueType = DssDataTypeUnknown;
					DSSDataFlag lFlag = DssDataUnknown;
                    
                    int lRowSize = 0;
                    // hr = lpColumn-> getRowSize(0, lRowSize);
                    hr = mvSlice[iSlice]->getSizeOfData(lRowSize);
                    CHECK_HR;
                    
                    char* lvRawData ;
                    
                    //lpColumn->getData(0, (const char **)&lvRawData, lRowSize, lValueType, lFlag);
                    hr = mvSlice[iSlice]->getValByOffset(0, (const char **)&lvRawData, lRowSize, lValueType, lFlag, NULL);
                    CHECK_HR;
					if(lFlag != DssDataOk)
					{
						//use the data type of the first argument
						inputType[iSlice] = inputType[iSlice-1];
					}
				}
			}
		}
        
		hr = ipNNFunc->GetOutputType(nSliceIn, inputType, &oOutputDataType, &lOutputParamType);
		CHECK_HR;
    }
    //ctang TQMS 429287
	else
	{
		//the numeric generic function has a non-nuemric input slice, the executiong will failed, here just give a defualt type.
		oOutputDataType = DssDataTypeDouble;
	}
    
    if (lOutputParamType == DssParameterVector)
    {
		//Type 2
		oFuncType = eGenericType2;
	}
    else
    { //type 1
		oFuncType = eGenericType1;
        
	}
    
	return S_OK;
}

int DSSSliceCalculator::hTrueBreakByLevel(vector<int> &ivBreakbyUnit,
                                          Int32 nSliceIn, DSSCubeDataSlice **ippSlices,
                                          bool *ipvIsVector,
                                          vector<int> &ovTrueBreakbyLevel)
{
    int hr = S_OK;
    
    vector<DSSCubeDataSlice * > lvVectorSlice;
    
    //step 1: union vector slices
	Int32 iSlice = 0;
	for( iSlice = 0; iSlice < nSliceIn; iSlice ++)
    {        
		if (ipvIsVector[iSlice]) 
        {
			lvVectorSlice.push_back(ippSlices[iSlice]);
		}
	}
    
    if (lvVectorSlice.size() > 0)  //the input slices have vector slices
	{
        vector<Int32> lvUnionLevel;
		hr = hSliceLevelUnion(lvVectorSlice.size(), &(lvVectorSlice[0]), lvUnionLevel);
		CHECK_HR;
        
		//Step 2: Get true breakby level
		vector<Int32> lvTrueLevel;
        
        if(mLevelResolver.getMCESchema())
        {
            hr = mLevelResolver.ResolveTrueLevel(lvUnionLevel, lvTrueLevel);
            CHECK_HR;
        
            hr = mLevelResolver.ResolveAggLevel(lvTrueLevel, ivBreakbyUnit, ovTrueBreakbyLevel);
            CHECK_HR;
        }
        else//941 compatibility to support if function
        {
            ovTrueBreakbyLevel = lvUnionLevel;
        }
		//step 3 check if each scalar input level is an upper level of the true break-by level
		for (iSlice = 0; iSlice < nSliceIn; iSlice ++) {
			if(!ipvIsVector[iSlice])
            {
                DSSTabularIndex *lpIndices = ippSlices[iSlice] -> getIndices();                
                CHECK_PTR(lpIndices);
                
                Int32 lnUnit = lpIndices -> getNumUnits();
                
                if (lnUnit == 0)
                    continue;
                
                vector<int> lvUnits;
                hr = lpIndices -> getUnits(lvUnits);
                CHECK_HR;
                
                bool bIsUpperLevel = 0;
                hr = mLevelResolver.IsUpperLevel(lvUnits, ovTrueBreakbyLevel, bIsUpperLevel);
                CHECK_HR;
                
                if (!bIsUpperLevel)
                {
                    return hr = S_FALSE;
                }
            }            
		} //for
	} 
    else 
    { 
        //All inputs are scalar, the function signature is same as a simple function. In this case, the inputs should be
		//break-by the common level of scalars 
		vector<Int32> lvUnionLevel;
		hr = hSliceLevelUnion(nSliceIn, ippSlices, lvUnionLevel);
		CHECK_HR;
        
		//Step 2: Get true breakby level
		hr = mLevelResolver.ResolveTrueLevel(lvUnionLevel, ovTrueBreakbyLevel);
		CHECK_HR;
	}
    
	return hr = S_OK;
} 

int DSSSliceCalculator::hSliceLevelUnion(Int32 nSliceIn, DSSCubeDataSlice **ppSliceIn, vector<Int32> &ovUnits)
{    
	int hr = S_OK;
    
	Int32 iSlice = 0,  iUnit = 0;
	for(iSlice = 0; iSlice < nSliceIn; iSlice ++)
	{
		//check if everybody compatible with this one		   
        DSSTabularIndex *lpIndices1 = ppSliceIn[iSlice]->getIndices();
        
        //zhyang, constant slice, 931 compatiblity
        if(!lpIndices1)
            continue;
        
		Int32 lnUnit1 = lpIndices1->getNumUnits();
        
		Int32 * lpUnit1 = new Int32[lnUnit1];
		CHECK_NEWED_PTR(lpUnit1);
        MBase::DSSStrongPtr<Int32, MBase::DeleteArray<Int32> > lUnit1Ptr(lpUnit1);
        
        hr = lpIndices1->getUnits(lnUnit1, lpUnit1);
		CHECK_HR;
        
		for(iUnit = 0; iUnit < lnUnit1; iUnit ++)
			ovUnits.push_back(lpUnit1[iUnit]);        
	}
    
	sort(ovUnits.begin(), ovUnits.end());
	vector<Int32>::iterator lIt = unique(ovUnits.begin(), ovUnits.end());
	ovUnits.erase(lIt, ovUnits.end());
    
	return S_OK;
}

int DSSSliceCalculator::hResolveLookupUnits(vector<int> &irvInput1, vector<int> &irvInput2, bool ifHasSort, vector<int> &orvOutput)
{
    int hr = S_OK;
    
    vector<int> lvLookupUnits(irvInput1.size() + irvInput2.size());
    vector<int>::iterator liUnionLast = std::set_union(irvInput1.begin(), irvInput1.end(), irvInput2.begin(), irvInput2.end(), lvLookupUnits.begin());
    vector<int> lvUnionLookupUnits;
    for (vector<int>::iterator iter = lvLookupUnits.begin(); iter != liUnionLast; iter++)
    {
        lvUnionLookupUnits.push_back(*iter);
    }
    
	// aye, 256820, 12/10/2006
	// If there is consolidation, we still want to keep the 'redundant' units in lookup index. 
	// The subset population relies on it. 
	// For example, the template main index contains {YearCS, Month, Year, Country, M1}, where {Year} is redundant.
	// We want to perform a subsetting on it by taking {Country} out. 
	// The following is what happens:
	// 1) SubsetCSI will aggregate M1 into {YearCS, Month} level
	// 2) The results will be populated by SubsetPoulate into the view report's TabularData, expecting an index of {YearCS, Month, Year}
	// Therefore we want to generate the {YearCS, Month, Year} index in step 1).  
	// This is not a problem for the non-consolidation cases, as the index can be simply generated from the main index.
	// However, consolidation index is generated specially, see CDSSXTabIndexPool::GenerateNewIndex for details.
	// This is a little awkward though. 
    
    bool lHasConsolidation = false;
    
    DSSCube *lpCube = mpContext->mpCube;
    CHECK_PTR(lpCube);
    
    DSSTabularData* lpTabularData = lpCube->getTabularData();
    if(!lpTabularData && !mpContext->mpSchema)
    {
        //931 compatiblity
        orvOutput.clear();
        for (vector<int>::iterator iter = lvUnionLookupUnits.begin(); iter != lvUnionLookupUnits.end(); iter++)
        {
            orvOutput.push_back(*iter);
        }
        return S_OK;
    }
    CHECK_PTR(lpTabularData);
    
    //Get all units from tabular data
    Int32 lCount = lpTabularData->Count();     
    for (Int32 i = 0; i < lCount; i ++) 
    {        
        DSSTabularUnit *lpUnit = NULL;
        hr = lpTabularData->Item(i, &lpUnit);
        CHECK_HR_PTR(lpUnit);
        
        EnumDSSTemplateUnitType lUnitType = lpUnit->getUnitType();
        if(lUnitType == DssTemplateConsolidation)
            lHasConsolidation = true;
    }
    
	// aye, 10/15/07, OLAP performance
	// at the moment, lpLookupUnits contains the union of the input levels.
	// e.g., if input1 is (City, Month), input2 is (Year),
	// then lpLookupUnits contains (City, Year, Month). We call this UnionLevel
    
	// We normally want to use this index as lookup.	 
	// We could also use an expanded index, which contains all parent attributes in the dimensions,
	// e.g., (Country, Region, City, Year, Quarter, Month). We call this one 'AllLevel'
	// If the UnionLevel index is not generated but the AllLevel index is, we use the AllLevel.
	// This avoids generating the UnionLevel index for now. 
    
    DSSTabularIndexPool *lpIndexPool = NULL;
    hr = lpCube->getIndexPool(&lpIndexPool);
    CHECK_HR;
    
    DSSTabularIndex *lpIndex = NULL;
    bool lFound = lpIndexPool->Search(lvUnionLookupUnits.size(), lvUnionLookupUnits.size() == 0 ? NULL : &lvUnionLookupUnits[0], &lpIndex);
    
    if (ifHasSort || lHasConsolidation || !lFound) // if the UnionLevel does not exist yet, find AllLevels
    {
        if(mLevelResolver.getMCESchema())
        {
            hr = mLevelResolver.ResolveLookupUnits(lvUnionLookupUnits, orvOutput);
            CHECK_HR;
        }
        //fali, 816727, add lookupUnits involved by SortByDefinations
        if (ifHasSort)
        {
            vector<DSSSortDefinition*> lvSortByDefns = mpContext->mvSortbyDefn;
            
            int lAttr = 0;
            vector<int> lvUnits (lvSortByDefns.size());
            for (int i = 0; i < lvSortByDefns.size(); i++)
            {
                DSSSortDefinition* lpDefn = lvSortByDefns[i];
                
                int lUnit = -1;
                hr = hGetUnit(lpDefn, &lUnit);
                CHECK_HR;
                
                // there are metric sort by, have to use all-level units
                if (lUnit == -1)
                    return S_OK;
                
                if (std::binary_search(orvOutput.begin(), orvOutput.end(), lUnit))
                {
                    lvUnits[lAttr++] = lUnit;
                }
            }
            
            // the sort by units could be not unique
            //            std::sort(lvUnits.begin(), lvUnits.end());
            
            //Begin-fali, currently can't go into the following logic, because we add a metric sort defination for OLAP function and will lead to return all-level units
            vector<int> lpUnitsWithSortBy(lAttr + lvUnionLookupUnits.size());
            vector<int>::iterator lpLast = std::set_union(lvUnionLookupUnits.begin(), lvUnionLookupUnits.end(), lvUnits.begin(), lvUnits.end(), lpUnitsWithSortBy.begin());
            
            vector<int> lvUnionUnitsWithSortBy;
            for (vector<int>::iterator iter = lpUnitsWithSortBy.begin(); iter != lpLast; iter++)
            {
                lvUnionUnitsWithSortBy.push_back(*iter);
            }
            orvOutput = lvUnionUnitsWithSortBy;
            lFound = lpIndexPool->Search(lvUnionUnitsWithSortBy.size(), lvUnionUnitsWithSortBy.size() == 0 ? NULL : &lvUnionUnitsWithSortBy[0], &lpIndex);
            //End
        }
        
        if ((ifHasSort && lFound)|| lHasConsolidation)
        {
            return S_OK;
        }
        else
        {
            lFound = lpIndexPool->Search(orvOutput.size(), orvOutput.size() == 0 ? NULL : &orvOutput[0], &lpIndex);
            if (lFound)  //Use the AllLevel index if it is already generated. Otherwise fall through to use the UnionLevel
                return S_OK;		
        }
    }
    
    orvOutput.clear();
    for (vector<int>::iterator iter = lvUnionLookupUnits.begin(); iter != lvUnionLookupUnits.end(); iter++)
    {
        orvOutput.push_back(*iter);
    }
    
	return S_OK;
}

int DSSSliceCalculator::hGroupDataForGenericFunction(DSSTabularIndex* ipLookupIndex,
                                                     DSSTabularIndex* ipBreakbyIndex,	
                                                     DSSXTabSortDefns* ipSortDefns,
                                                     GenericFunctionType iFuncType,
                                                     vector<Int32> **& orvDataByGroup,
                                                     MBase::Buffer* ipBuffer,
                                                     Int32 &orBreakByRow)
                                                     //DSSXTabBitVector* &orMask)
{        
    int hr = S_OK;
    
    if(!mLevelResolver.getMCESchema())
    {
        Int32 lnRows = ipLookupIndex->Count();
        orvDataByGroup = new vector<Int32>* [lnRows];
        for(int i = 0;i < lnRows; i++)
        {
            orvDataByGroup[i] = new  vector<Int32>;
            orvDataByGroup[i]->push_back(i);
        }
        orBreakByRow = lnRows;
        return S_OK;
    }
    
    orBreakByRow = ipBreakbyIndex -> Count();
    
    Int32 lnVecRow = 0;
    lnVecRow = ipLookupIndex -> Count();
    
    Int32 lnTotalUnits = 0;
    
    hr = hGetTotalUnits(lnTotalUnits);
    CHECK_HR;
    
    vector<Int32> lvKeys(lnTotalUnits);
    
    orvDataByGroup = new  vector<Int32>* [orBreakByRow];
    CHECK_NEWED_PTR(orvDataByGroup);
    
    Int32 iGroup = 0;
    
     for(iGroup = 0; iGroup < orBreakByRow; ++iGroup)
     {
         orvDataByGroup[iGroup] = new  vector<Int32>;
         CHECK_NEWED_PTR(orvDataByGroup[iGroup]);
    }
    
    vector<DSSCubeDataSlice*> lvInputSlices = mvSlice;
    int nSlices = lvInputSlices.size();
    
    Int32 lSliceId = 0;
    if(nSlices > 0)
    {
        DSSTabularIndex* lpIndex;
        lpIndex = lvInputSlices[0]->getIndices();
        
        Int32 lnRows = lpIndex -> Count();
        if(lnRows != lnVecRow)
        {
            Int32 lMaxRowCount = lnRows;
            for(Int32 iSlice = 1; iSlice < nSlices; ++iSlice)
            {
                DSSTabularIndex* lpIndex = lvInputSlices[iSlice] -> getIndices();
                lnRows = lpIndex->Count();
                if(lnRows > lMaxRowCount)
                {
                    lMaxRowCount = lnRows;
                    lSliceId = iSlice;
                }
            }
        }
    }
    
    bool lDoSort = mpContext->mvSortbyDefn.size() != 0 ? true : false;
    if(lDoSort)
    {
        hr = hSortIndex(ipLookupIndex, &(mpContext->mvSortbyDefn), lvInputSlices[lSliceId]);
        CHECK_HR;
    }
    
    // aye, The following logic should be combined
    DSSXTabBitVector *lpInputMask = NULL;
    DSSXTabBitVector *lpNewOutputMask = NULL;
    
    DSSXTabBitVector* orMask = NULL;
    if (mpContext->mpMaskInfos && !mpContext->mpMaskInfos->IsEmpty())
    {
        bool lCalculated = false;
        hr = mpContext->mpMaskInfos->GetCalculatedMask(ipLookupIndex, &lpInputMask);
        CHECK_HR;
        
        // Check if we have the lpOutputMask already. If not, create one and set it in our iteration
        hr = mpContext->mpMaskInfos->GetMask(ipBreakbyIndex, &orMask, &lCalculated);
        CHECK_HR;
        
        // if not calculated, do it our next iteration over the data
        if (!lCalculated)
            lpNewOutputMask = orMask;
    }
    
    for(Int32 iRow = 0; iRow < lnVecRow; ++iRow)
    {
        Int32 lSortedRow = SortedIndex(iRow);
        
        if (lpInputMask && !lpInputMask->Get(lSortedRow))
            continue;
        
        hr = ipLookupIndex->Item(lSortedRow, lvKeys.size(), &lvKeys[0]);
        CHECK_HR;
        
        if(iFuncType == eGenericType2)
        {
            // DE
            /*
             bool lHidden = false;
             for (Int32 i = 0; i < lvDEUnitIDs.size(); i++)
             {
             Int32 lElementIndex = lvKeys[lvDEUnitIDs[i]];
             Int32 lOrdinal = 0;
             
             // When this call is called from CompoundMetric event, and if the CM event is before the
             // DE event, the function should see the elements that are displayed in the base,
             // which should check get_BaseOrdinal
             HRESULT hr = S_OK;
             
             VARIANT_BOOL lUseBaseOrdinal = VARIANT_FALSE;
             hr = lvDEUnitPtrs[i]->get_UseBaseOrdinal(&lUseBaseOrdinal);
             CHECK_HR;
             
             if (lUseBaseOrdinal == VARIANT_TRUE)
             hr = lvDEUnitPtrs[i]->get_BaseOrdinal(lElementIndex, &lOrdinal);
             else
             hr = lvDEUnitPtrs[i]->get_Ordinal(lElementIndex, &lOrdinal);
             CHECK_HR;
             
             if (lOrdinal < 0 && !mbCalcHiddenDE) {
             lHidden = true;
             break;
             }
             
             }
             
             if (lHidden)
             continue;
             */
        }
        else
        {
            /*
             bool lIgnored = false;
             hr = hIsIgnoredInAggregation(&lvKeys[0], lvDEUnitPtrs, lvDEUnitIDs, lvCGIndexMaps, lIgnored);
             CHECK_HR;
             
             if (lIgnored)
             continue;
             */
        }
        
        /*
         if (lpRowMap)
         {
         lGroup = XTabIndexRowMapLookup(lpRowMap, lSortedRow);
         }
         else
         {
         hr = ipBreakbyIndex -> LookUp(lnTotalUnits, &lvKeys[0], &lGroup);
         CHECK_HR;
         }*/
        //Use the following simple code
        
        Int32 lGroup = 0;
        hr = ipBreakbyIndex->LookUp(lnTotalUnits, &lvKeys[0], &lGroup);
        
        Int32 lRow = SortedIndex(iRow);
        orvDataByGroup[lGroup]->push_back(lRow);
        
        if (lpNewOutputMask)
        lpNewOutputMask->Set(lGroup, true);
         
    }
    
    return hr;
}

int DSSSliceCalculator::hGetTotalUnits(Int32& oTotalUnits)
{
    DSSCube *lpCube = mpContext->mpCube;
    CHECK_PTR(lpCube);
    
    DSSTabularData* lpTabularData = lpCube->getTabularData();
    CHECK_PTR(lpTabularData);
    
    oTotalUnits = lpTabularData->Count();
    
    return S_OK;
}

int DSSSliceCalculator::hSortIndex(DSSTabularIndex *ipLookUpIndex,
                                   vector<DSSSortDefinition *> *ivpSortDefns,
                                   DSSCubeDataSlice *pSliceIn)
{
    int hr = S_OK;
    
    if (ivpSortDefns)
    {
        hr = mpContext->mpCube->SortIndex(&mpSortIndex, ipLookUpIndex, ivpSortDefns, pSliceIn);
        CHECK_HR_PTR(mpSortIndex);
    }
    return S_OK;
}

int DSSSliceCalculator::hCheckDirectCalculate(DSSTabularIndex* ipLookUpIndex,
                                              bool& orUseDirectCalculate,
                                              DSSSimpleFunction* ipFuncObj)
{
	orUseDirectCalculate = false;
    
    int lnSliceIn = mvSlice.size();
    
	if (lnSliceIn < 2) // no optimize for less than 2 input slices
		return S_OK;
    
    
    bool lSupportDC = ipFuncObj->SupportDirectCalculate();
    
	if (lSupportDC != true)
		return S_OK;
    
	// go over each slice, to check whether all input slices are at the same level
	for (Int32 i = 0; i < lnSliceIn; i++)
	{
		DSSDataType_Type lType = mvSlice[i] -> getDataType();
        
		// though this function is only called in ApplyNumericalSimpleFunction,
		// the input slice type could still be cell format, which we cannot use direct calculate
		if (lType == DssDataTypeCellFormatData || lType == DssDataTypeBigDecimal)
		{
			return S_OK;
		}
        
		DSSTabularIndex *lpIndex = mvSlice[i] -> getIndices();
        
        if(lpIndex)
        {
            if (!ipLookUpIndex -> IsSameOrder (lpIndex))
            {	// if input slices are at different levels, we only let it go if they are const slices
                Int32 lnUnit = lpIndex -> getNumUnits();
                if (lnUnit != 0)
                {
                    return S_OK;
                }
            }
            
            // direct calculate can be performed on conditional metric when skip null
            if (mvSlice[i]->GetMask() && mpContext->mNullCheckingOption == DssNumericErrorTreatAs)
                return S_OK;
        }
	}
    
	// pass all the checks, use it
	orUseDirectCalculate = true;
    
	return S_OK;
}

int DSSSliceCalculator::hDirectCalculateForSimpleFunction(DSSTabularIndex* ipLookUpIndex,
                                                          DSSSimpleFunction* pFuncObj,
                                                          DSSCubeDataSlice* opResSlice,
                                                          DSSXTabBitVector* ipMask,
                                                          EnumNullCheckingOption iNullCheckingOption)
{
	// currently we only support direct calculate for 2 input slices
	HRESULT hr = S_OK;
    
	CHECK_PTR(pFuncObj);
    
    MBase::SimpleBuffer lSimpleBuffer;
	MBase::BufferPtr lBufferPtr(&lSimpleBuffer);
	MBase::Buffer *lpBuffer = lBufferPtr.Get();
	CHECK_NEWED_PTR(lpBuffer);
    
    int nSliceIn = mvSlice.size();
    DSSCubeDataSlice **ppSliceIn = &mvSlice[0];
	void** lpData = new (lpBuffer) void* [nSliceIn];
	CHECK_NEWED_PTR(lpData);
    
	Int32** lpOffsets = new (lpBuffer) Int32* [nSliceIn];
	CHECK_NEWED_PTR(lpOffsets);
    
	DSSData_Flags** lpFlags = new (lpBuffer) DSSData_Flags* [nSliceIn];
	CHECK_NEWED_PTR(lpFlags);
    
	Int32* lpRows = new (lpBuffer) Int32 [nSliceIn];
	CHECK_NEWED_PTR(lpRows);
    
	double* lpDivider = new (lpBuffer) double [nSliceIn];
	CHECK_NEWED_PTR(lpDivider);
    
	VARIANT_BOOL* lpAllDataValid = new (lpBuffer) VARIANT_BOOL[nSliceIn];
	CHECK_NEWED_PTR(lpAllDataValid);
	// slice and column may have different type due to Fixed Precision Double change
	// as we are directly operating on the column, the type should be the exact type of column
	DSSDataType_Type* lpType = new (lpBuffer) DSSDataType_Type [nSliceIn];
    
	// prepare the inputs
	for (Int32 iSlice = 0; iSlice < nSliceIn; iSlice ++)
	{
		DSSDataColumn *lpColumn = mvSlice[iSlice] -> getColumn();
		CHECK_PTR(lpColumn);
        
		lpRows[iSlice] = lpColumn -> getRowCount();
        
		// lweng, 2012-7-22, TQMS 627588, skip calcution if any of the input slices with row # 0
		if (lpRows[iSlice] == 0)
			return S_FALSE;
        
		lpOffsets[iSlice] = NULL;//lpColumn->GetOffsets(); hard code ltm
        
		if (false)//lpColumn->IsDistinct())
		{
			lpOffsets[iSlice] = NULL;
			lpRows[iSlice] = 1;
		}
        
		lpType[iSlice] = lpColumn -> getDataType();
        
		hr = lpColumn -> getRawDataAndFlags(&lpData[iSlice], &lpFlags[iSlice]);
        CHECK_HR;
        
		if (lpData[iSlice] == NULL || lpFlags[iSlice] == NULL)
			return S_FALSE;
        bool lbAllDataValid = false;
		hr = lpColumn -> getAllDataValidFlag(lbAllDataValid);
		CHECK_HR;
        lpAllDataValid[iSlice] = lbAllDataValid ? VARIANT_TRUE : VARIANT_FALSE;
        
		if (!lpAllDataValid[iSlice] && iNullCheckingOption == DssNumericErrorTreatAs)
		{
			//DSSData_Flags* lpFlag = lpFlags[iSlice];
			Int32 lnRow = lpRows[iSlice];
            
			for (Int32 i = 0; i < lnRow; i++)
			{
				// qjiang, exists IgnoreCheckNull, have to go thru the old code paths
                // DssDataIgnoreCheckNull is not supported
				//if (lpFlag[i] == DssDataIgnoreCheckNull)
				//	return S_FALSE;
			}
		}
        
		lpDivider[iSlice] = lpColumn -> getDivider();
        
	}
    
	DSSDataColumn *lpResColumn = opResSlice -> getColumn();
	CHECK_PTR(lpResColumn);
    
	DSSDataColumn *lpResCol = lpResColumn;
	CHECK_PTR(lpResCol);
    
	Int32 lnResSize = lpResCol->getRowCount();
    
	double* lpResData = NULL;
	DSSDataFlag* lpResFlags = NULL;
    
	DSSTabularIndex *lpResIndex = opResSlice -> getIndices();
	CHECK_PTR(lpResIndex);
    
	bool lbSameOrder = ipLookUpIndex -> IsSameOrder (lpResIndex);
    
	if (lbSameOrder)
	{// directly operate on the result column
		hr = lpResCol -> getRawDataAndFlags((void**)&lpResData, &lpResFlags);
		CHECK_HR;
		if (lpResData == NULL || lpResFlags == NULL)
			return S_FALSE;
	}
	else
	{// input/output slices should be at the same level, but sometimes they can have different orders,
        // in this case, we'll still allocate some temp arrays to store the intermediate result, and translate them to result at the last step
		lpResData = new (lpBuffer) double[lnResSize];
		CHECK_NEWED_PTR(lpResData);
        
		lpResFlags = new (lpBuffer) DSSDataFlag[lnResSize];
		CHECK_NEWED_PTR(lpResFlags);
	}
    
	VARIANT_BOOL lAllDataValid = VARIANT_FALSE;
    
	hr = pFuncObj -> DirectCalculate(nSliceIn, lpRows, lpOffsets, lpData, lpDivider, lpFlags, lpAllDataValid,
                                     lpType, ipMask, lnResSize, lpResData, lpResFlags, &lAllDataValid, iNullCheckingOption);
	CHECK_HR;
    
	if (lbSameOrder)
	{
		hr = lpResColumn -> setAllDataValidFlag(lAllDataValid);
		CHECK_HR;
	}
	else
	{// use correlator to do the final translation
		DSSSliceCorrelator lSliceCorrelator;
		lSliceCorrelator.Init(ipLookUpIndex, nSliceIn, ppSliceIn, opResSlice, mpContext->mpCube);
		for (Int32 iRow = 0; iRow < lnResSize; iRow ++)
		{
			hr = lSliceCorrelator.SetOutputValue(iRow, lpResData[iRow], lpResFlags[iRow]);
			CHECK_HR;
		}
	}
    
	return S_OK;
}

// jxie, 2010-9-14
// check if it supports direct calculate for generic functions
int DSSSliceCalculator::hCheckDirectCalculate(Int32 nSliceIn,
                                              DSSCubeDataSlice** ppSliceIn,
                                              DSSTabularIndex* ipLookUpIndex,
                                              DSSGenericFunction* ipFunc,
                                              EnumNullCheckingOption iNullCheckOption,
                                              bool& orUseDirectCalculate)
{
	orUseDirectCalculate = false;
    
	bool lSupportDC = false;
	lSupportDC = ipFunc -> SupportDirectCalculate();
    
	if (lSupportDC != true)
		return S_OK;
    
	// go over each slice, to check whether all input slices are at the same level
	for (Int32 i = 0; i < nSliceIn; i++)
	{
		DSSDataType_Type lType = ppSliceIn[i] -> getDataType();
        
		// though this function is only called in ApplyNumericalSimpleFunction,
		// the input slice type could still be cell format, which we cannot use direct calculate
		if (lType == DssDataTypeCellFormatData || lType == DssDataTypeBigDecimal)
		{
			return S_OK;
		}
        
		DSSTabularIndex *lpIndex = ppSliceIn[i] -> getIndices();
		CHECK_PTR(lpIndex);
        
		if (!ipLookUpIndex -> IsSameOrder (lpIndex))
		{	// jxie, if input slices are at different levels, we won't use direct calculate now
			return S_OK;
		}
        
		// direct calculate can be performed on conditional metric when skip null
		if (ppSliceIn[i]->GetMask() && iNullCheckOption == DssNumericErrorTreatAs)
			return S_OK;
	}
    
	// pass all the checks, use it
	orUseDirectCalculate = true;
    
	return S_OK;
}

// jxie, 2010-9-14
// use direct calculate for some generic function to improve performance
int DSSSliceCalculator::hDirectCalculateForGenericFunction(Int32 nSliceIn, DSSCubeDataSlice** ppSliceIn, DSSTabularIndex* ipLookUpIndex, DSSGenericFunction* ipFunc,
                                                           DSSCubeDataSlice* opResSlice, DSSXTabBitVector* ipMask, vector<Int32> ** ipvDataByGroup,
                                                           Int32 inBreakbyRow, EnumNullCheckingOption iNullCheckingOption, GenericFunctionType iFuncType)
{
	// currently we only support direct calculate for 2 input slices
	HRESULT hr = S_OK;
    
	CHECK_PTR(ipFunc);
    
    MBase::SimpleBuffer lSimpleBuffer;
	MBase::BufferPtr lBufferPtr(&lSimpleBuffer);
	MBase::Buffer *lpBuffer = lBufferPtr.Get();
	CHECK_NEWED_PTR(lpBuffer);
    
	void** lpData = new (lpBuffer) void* [nSliceIn];
	CHECK_NEWED_PTR(lpData);
    
	Int32** lpOffsets = new (lpBuffer) Int32* [nSliceIn];
	CHECK_NEWED_PTR(lpOffsets);
    
	DSSData_Flags** lpFlags = new (lpBuffer) DSSData_Flags* [nSliceIn];
	CHECK_NEWED_PTR(lpFlags);
    
	Int32* lpRows = new (lpBuffer) Int32 [nSliceIn];
	CHECK_NEWED_PTR(lpRows);
    
	double* lpDivider = new (lpBuffer) double [nSliceIn];
	CHECK_NEWED_PTR(lpDivider);
    
	VARIANT_BOOL* lpAllDataValid = new (lpBuffer) VARIANT_BOOL[nSliceIn];
	CHECK_NEWED_PTR(lpAllDataValid);
	// slice and column may have different type due to Fixed Precision Double change
	// as we are directly operating on the column, the type should be the exact type of column
	DSSDataType_Type* lpType = new (lpBuffer) DSSDataType_Type [nSliceIn];
    
	// prepare the inputs
	for (Int32 iSlice = 0; iSlice < nSliceIn; iSlice ++)
	{
		DSSDataColumn *lpColumn = ppSliceIn[iSlice] -> getColumn();
        
		lpRows[iSlice] = lpColumn -> getRowCount();
        
		// lweng, 2012-7-22, TQMS 627588, skip calcution if any of the input slices with row # 0
		if (lpRows[iSlice] == 0)
			return S_FALSE;
        
		lpOffsets[iSlice] = NULL;//lpColumn->GetOffsets();
        
		if (false)//lpColumn->IsDistinct())
		{
			lpOffsets[iSlice] = NULL;
			lpRows[iSlice] = 1;
		}
        
		lpType[iSlice] = lpColumn -> getDataType();
		
		hr = lpColumn -> getRawDataAndFlags(&lpData[iSlice], &lpFlags[iSlice]);
		CHECK_HR;
        
		if (lpData[iSlice] == NULL || lpFlags[iSlice] == NULL)
			return S_FALSE;
        
        bool lbAllDataValid = false;
		hr = lpColumn -> getAllDataValidFlag(lbAllDataValid);
		CHECK_HR;
        lpAllDataValid[iSlice] = lbAllDataValid ? VARIANT_TRUE : VARIANT_FALSE;
        
		if (lpAllDataValid[iSlice] != true && iNullCheckingOption == DssNumericErrorTreatAs)
		{
			//DSSData_Flags* lpFlag = lpFlags[iSlice];
			Int32 lnRow = lpRows[iSlice];
            
			for (Int32 i = 0; i < lnRow; i++)
			{
				// qjiang, exists IgnoreCheckNull, have to go thru the old code paths
                //				if (lpFlag[i] == DssDataIgnoreCheckNull)
                //					return S_FALSE;
			}
		}
        
		lpDivider[iSlice] = lpColumn -> getDivider(); 
        
	}
    
	DSSDataColumn *lpResColumn = opResSlice -> getColumn ();
	CHECK_PTR(lpResColumn);
    
	Int32 lnResSize = lpResColumn->getRowCount();
    
	double* lpResData = NULL;
	DSSData_Flags* lpResFlags = NULL;
    
	DSSTabularIndex *lpResIndex = opResSlice -> getIndices();
    
	bool lbSameOrder = ipLookUpIndex -> IsSameOrder (lpResIndex);
    
	if (lbSameOrder)
	{// directly operate on the result column
		hr = lpResColumn -> getRawDataAndFlags((void**)&lpResData, &lpResFlags);
		CHECK_HR;
		if (lpResData == NULL || lpResFlags == NULL)
			return S_FALSE;
	}
	else
	{// input/output slices should be at the same level, but sometimes they can have different orders,
        // in this case, we'll still allocate some temp arrays to store the intermediate result, and translate them to result at the last step
		lpResData = new (lpBuffer) double[lnResSize];
		CHECK_NEWED_PTR(lpResData);
        
		lpResFlags = new (lpBuffer) DSSData_Flags[lnResSize];
		CHECK_NEWED_PTR(lpResFlags);
	}
    
	VARIANT_BOOL lAllDataValid = VARIANT_FALSE;
    
	DSSSliceCorrelator lSliceCorrelator;
	lSliceCorrelator.Init(ipLookUpIndex, nSliceIn, ppSliceIn, opResSlice, mpContext->mpCube);
    
	bool lbAllResultDataValid = true;
    
	//for each group, set parameter, calculate and put the reusult into outSlice.
	for(Int32 iGroup = 0; iGroup < inBreakbyRow; iGroup ++)
    {
		if (ipMask && ipMask->Get(iGroup) == 0)
			continue;
        
		Int32 groupSize = ipvDataByGroup[iGroup]->size();
		hr = ipFunc -> DirectCalculate(nSliceIn, lpRows, lpOffsets, lpData, lpDivider, lpFlags, lpAllDataValid,
                                       lpType, ipvDataByGroup[iGroup], lnResSize, lpResData, lpResFlags, &lAllDataValid, iNullCheckingOption);
		CHECK_HR;
        
		if (lbSameOrder)
		{
			if (lbAllResultDataValid && !lAllDataValid)
			{
				hr = lpResColumn -> setAllDataValidFlag(lAllDataValid);
				CHECK_HR;
                
				lbAllResultDataValid = false; // this flag can only be set once
			}
		}
		else
		{
			//get the output and set it into the result column
			if (iFuncType == eGenericType2) { // ASSERT: resultSlice.rowNum == lookupIndex.rowNum
				// need to fill empty cell in the output
				for(Int32 iElement = 0; iElement < groupSize; iElement ++){
					Int32 lRow = (*ipvDataByGroup[iGroup])[iElement];
					hr = lSliceCorrelator.SetOutputValue(lRow, lpResData[lRow], lpResFlags[lRow]);
					CHECK_HR;
				} //for
                
			} else { //scalar type 1 Assert: result.Index == breakByIndex
				hr = lpResColumn->setNumeric(iGroup, lpResData[0], lpResFlags[0]);
				CHECK_HR;
			}
		}
	}
    
	return S_OK;
}

// for function "Count", the inputs can determine the behavior
int DSSSliceCalculator::hGetCountType(DSSSliceCalculator::CountType &oCountType)
{
	int hr = S_OK;
    
    DSSCubeDataSlice *lpSliceIn = mvSlice[0];
	EnumSimpleOperatorType lType = mpContext->mpFuncObject->getOperatorType();
    
	if (lType != DssFuncCount)
		return S_OK;
    
	if (mpContext->mfIsOnIDColumn)
		oCountType = eCountID;
	else
	{
		DSSDataColumn* lpColumn = lpSliceIn -> getColumn();
		CHECK_PTR(lpColumn);
        
		if (lpColumn)
		{
			Int32 lnRow = lpColumn->getRowCount();
            
			XTabCellData lCellData;
			if (lnRow == 1)
			{
				// ok, this is a constant slice, check if it is "*"
				hr = lpColumn->getCellData(0, lCellData);
				CHECK_HR;
                
				CComVariant lConstStar(_T("*"));
                
				CComVariant lVariant;
				DFCDataConversion::ConvertToVariant(lCellData.mpData, lCellData.mDataType, lCellData.mSize, &lVariant);
                
				DSSData_Flags lFlag = DssDataOk;
				// dzhou 2011-08-02 TQMS461880 just compare with '*', is ok to trim.
				// ok to set default value.
				Int32 lCompare = variantCompare(lVariant, lConstStar, &lFlag, true, true);
				if (lFlag == DssDataOk && lCompare == 0)
				{
					// it is Count(*)
					oCountType = eCountStar;
				}
			}
            /*
             if (lpColumn->GetOffsets() != NULL)
             {
             oCountType = eCountOffsets;
             }*/
		}
	}
    
	return S_OK;
}

// qjiang 07/28/2009 TQMS 361103
//special handling for First/Last
// only use the sort definition to do linear row-row comparison, no sorting
int DSSSliceCalculator::hCalculateFirstLast(DSSTabularIndex *ipInputIndex,
                                            vector<DSSSortDefinition *> *ipSortDefns,
                                            DSSCubeDataSlice *ipSlice,
                                            DSSCubeDataSlice *opSlice,
                                            bool iFirst)
{
	int hr = S_OK;
	// all the pointers are checked before this call, no need to check again
    
    Int32 *lpGroupBound = NULL;
	Int32 *lpBlockList = NULL;
	Int32 *lpBlockAddress = NULL;
    
	DSSTabularIndex *lpResIndex = opSlice->getIndices();
	CHECK_PTR(lpResIndex);
    
	Int32 lnResRow;	// number of output rows
	Int32 lnRow; // number of input rows
	lnResRow = lpResIndex->Count();
    
	bool lbSameOrder = ipInputIndex->IsSameOrder(lpResIndex);
    
	DSSXTabBitVector* lpMask = NULL;
    
	if (!lbSameOrder)
	{
		hr = hGroupData(ipSlice, ipInputIndex, opSlice, false, lpBlockList, lnRow, lpGroupBound,
                        lnResRow, lpBlockAddress, lpMask, true);
		CHECK_HR;
	}
	else if (mpContext->mpMaskInfos && !mpContext->mpMaskInfos->IsEmpty())
	{
		hr = mpContext->mpMaskInfos->GetCalculatedMask(ipInputIndex, &lpMask);
		CHECK_HR;
	}
    
	DSSXTabKeyIterator lIt(ipInputIndex);
    
	Int32 lnUnit = ipInputIndex -> getNumUnits();
    
	vector<Int32> lvUnits(lnUnit);
	hr = ipInputIndex -> getUnits(lnUnit, &lvUnits[0]);
	CHECK_HR;
    
	Int32 lnTotalUnit = 0;
	hr = hGetTotalUnits(lnTotalUnit);
	CHECK_HR;
    
	// lvUnitPos stores the position of each unit in this index
	// e.g. ipInputIndex has 3 units (1,2,5), then lvUnitPos = (-, 0, 1, -, -, 2), '-' means no entry.
	vector<Int32> lvUnitPos(lnTotalUnit);
    for (Int32 iUnit = 0; iUnit < lnTotalUnit; iUnit++)
        lvUnitPos[iUnit] = -1;
	for (Int32 iUnit = 0; iUnit < lnUnit; iUnit++)
	{
		lvUnitPos[lvUnits[iUnit]] = iUnit;
	}
    
	Int32 lnRowSize = 0;
	lnRowSize = ipSlice -> getColumn()->getSizeOfData();
    
    //tliao, 912747, 2014/08/04, fix memory leak
	BYTE* lpRawData = NULL;//new BYTE[lnRowSize];
	//CHECK_NEWED_PTR(lpRawData);
    
	Int32 lcbWritten = 0;
	DSSDataType_Type lType = DssDataTypeUnknown;
	DSSData_Flags lFlag = DssDataOk;
    
	Int32 lnSort = 0;
    
    if (ipSortDefns != NULL)
    {
        lnSort = ipSortDefns -> size();
    }
     
    CDSSDoSort lDoSort(ipSortDefns);
    vector<CDSSBucketSort*> lvSort(lnSort);
    vector<Int32> lvSortUnit(lnSort);
    
    if (!lbSameOrder)
    {
    // based on the sort definitions, create coresponding sort object, here we only use their comparison functions, won't do any actual sorting
    hr = lDoSort.OLAPSortInit(mpContext->mpCube, ipInputIndex, ipSlice);
    CHECK_HR;
     
    
    DSSSortDefinition *lpDefn = NULL;
    for (Int32 iSort = 0; iSort < lnSort; iSort ++)
    {
        lpDefn = (*ipSortDefns)[iSort];
        CHECK_HR;
     
        hr = lDoSort.ConstructBucketSort(lpDefn, &lvSort[iSort]);
        CHECK_HR;
     
        // weiwang 2009-08-12 TQMS352257 Use hidden element to sort for agg function calculation
        lvSort[iSort]->SetIgnoreHiddenFlag(false);
     
        Int32 lnUnitInTabularTata = 0;
        hr = hGetUnit(lpDefn, &lnUnitInTabularTata);
        CHECK_HR;
     
        if (lnUnitInTabularTata == -1)
            lvSortUnit[iSort] = -1;
        else
            lvSortUnit[iSort] = lvUnitPos[lnUnitInTabularTata];
    }
    }
    
    Int32 lRow = -1;
    
	DSSSliceCorrelator lSliceCorrelator;
	lSliceCorrelator.Init(ipInputIndex, 1, &ipSlice, opSlice, mpContext->mpCube);
	// for each group, go through the elements, select the First/Last element as wanted
	for (Int32 iRow = 0; iRow < lnResRow; iRow++)
	{
		if (lpMask && lpMask->Get(iRow) == 0) // filtered out, skip it
			continue;
        
        lRow = iRow;
        
        if (!lbSameOrder)
        {
            Int32 lpGroupPtr = lpGroupBound[iRow];
			Int32 lpGroupEnd = lpGroupBound[iRow + 1];
            
			// group size could be 0, skip it
			if (lpGroupPtr == lpGroupEnd)
				continue;
            
			Int32 lFirstValidBlock = -1;
			// find the first non-null row
			while (lpGroupPtr < lpGroupEnd)
			{
				Int32 lBlock = lpBlockList[lpGroupPtr];
				Int32 lStart = lpBlockAddress ? lpBlockAddress[lBlock] : lBlock;
				Int32 lEnd = lpBlockAddress ? lpBlockAddress[lBlock+1] : lBlock+1;
                
				for (Int32 iInputRow = lStart; iInputRow < lEnd; iInputRow++)
				{
					lRow = iInputRow;
					hr = lSliceCorrelator.GetInputValue(0, iInputRow, &lpRawData, lnRowSize, &lcbWritten, &lType, &lFlag);
					CHECK_HR;
                    
					if (lFlag == DssDataOk)
					{
						break;
					}
				}
                
				// we find the first valid element
				if (lFlag == DssDataOk)
				{
					lFirstValidBlock = lBlock;
					break;
				}
                
				lpGroupPtr++;
			}
            
			// here lnRow points to the first non-NULL row
            
			if (lnSort == 0)
			{// no sort definition, simply pick the first or last non-Null element
				if (!iFirst)
				{
					Int32 lpGroupPtr = lpGroupBound[iRow + 1] - 1;
					Int32 lpGroupStart = lpGroupBound[iRow];
					while (lpGroupPtr >= lpGroupStart)
					{
						Int32 lBlock = lpBlockList[lpGroupPtr];
						Int32 lStart = lpBlockAddress ? lpBlockAddress[lBlock] : lBlock;
						Int32 lEnd = lpBlockAddress ? lpBlockAddress[lBlock+1] : lBlock+1;
                        
						for (Int32 iInputRow = lEnd-1; iInputRow >= lStart; iInputRow--)
						{
							lRow = iInputRow;
                            
							hr = lSliceCorrelator.GetInputValue(0, iInputRow, &lpRawData, lnRowSize, &lcbWritten, &lType, &lFlag);
							CHECK_HR;
                            
							if (lFlag == DssDataOk)
							{
								break;
							}
						}
                        
						// we find the last valid element
						if (lFlag == DssDataOk)
						{
							break;
						}
                        
						lpGroupPtr--;
					}
                    
				}
			}
			else
			{
				// ok, we'll try to linear scan all rows in this group to determine which one is the First/Last based on the sort definition
				while (lpGroupPtr < lpGroupEnd)
				{
					Int32 lBlock = lpBlockList[lpGroupPtr];
					Int32 lStart = lpBlockAddress ? lpBlockAddress[lBlock] : lBlock;
					Int32 lEnd = lpBlockAddress ? lpBlockAddress[lBlock+1] : lBlock+1;
                    
					if (lBlock == lFirstValidBlock)
						lStart = lRow+1;
                    
					for (Int32 iInputRow = lStart; iInputRow < lEnd; iInputRow++)
					{
						hr = lSliceCorrelator.GetInputValue(0, iInputRow, &lpRawData, lnRowSize, &lcbWritten, &lType, &lFlag);
						CHECK_HR;
                        
						if (lFlag != DssDataOk) // only care rows that have valid elements
							continue;
                        
						for (Int32 iSort = 0; iSort < lnSort; iSort++)
						{
							Int32 lUnit = lvSortUnit[iSort];
                            
                            if (lUnit == -1)
                            {
                                DSSSortDefinition* lpDefn = (*ipSortDefns)[iSort];
                                if (lpDefn->sortType != DssSortValue && lpDefn->sortType != DssSortMetric)
                                    continue;
                            }
                            
							// lUnit == -1 means comparing metric columns
							// else this sort definition is qualified on the attribute column(e.g. ID, DESC, etc)
							lIt.SetRow(lRow);
							Int32 lRow1 = (lUnit == -1) ? lRow : lIt.GetCol(lUnit);
                            
							lIt.SetRow(iInputRow);
							Int32 lRow2 = (lUnit == -1) ? iInputRow : lIt.GetCol(lUnit);
                            
							if (lRow1 == lRow2)
								continue;
                            
							if ((*lvSort[iSort])(lRow1, lRow2))
							{
								lRow = iFirst ? lRow : iInputRow;
								break;
							}
							else if ((*lvSort[iSort])(lRow2, lRow1))
							{
								lRow = iFirst ? iInputRow : lRow;
								break;
							}
						}
					}
                    
					lpGroupPtr ++;
				}
			}
        }
         
		// here lnRow should be the result row in source slice
		hr = lSliceCorrelator.GetInputValue(0, lRow, &lpRawData, lnRowSize, &lcbWritten, &lType, &lFlag);
		CHECK_HR;
        
		// qjiang 08/14/2009 TQMS 370253
		// if input slice has type of UTF8 or MBChar, as the output is WideChar, convert it here
		if (lType == DssDataTypeUTF8Char)
		{
			MBase::ReturnWString lStr = MBase::UTF8ToWideChar((const char*)lpRawData);
			hr = lSliceCorrelator.SetOutputValue(lRow, (BYTE*)lStr.c_str(), lcbWritten, DssDataTypeVarChar, lFlag);
		}
        /*
         else if (lType == DssDataTypeMBChar)
         {
         MBase::ReturnWString lStr = MBase::MultiByteToWideChar((const char*)lpRawData);
         hr = lSliceCorrelator.SetOutputValue(lnRow, (BYTE*)lStr.c_str(), lcbWritten, DssDataTypeVarChar, lFlag);
         }*/
		else
		{
			hr = lSliceCorrelator.SetOutputValue(lRow, lpRawData, lcbWritten, lType, lFlag);
		}
		CHECK_HR;
	}
    //tliao, 912747, 2014/08/04, fix memory leak
    for (int i = 0; i < lvSort.size(); i++)
    {
        if (lvSort[i])
        {
            delete lvSort[i];
            lvSort[i] = NULL;
        }
    }

	return S_OK;
}

// get the unit of the sort definition
int DSSSliceCalculator::hGetUnit(DSSSortDefinition *ipSortDefn, Int32 *opUnit)
{
	CHECK_PTR(opUnit);
    
	EnumDSSSortType lType = ipSortDefn->sortType;
    
	if (lType == DssSortMetric || lType == DssSortValue)
	{
		*opUnit = -1;
		return S_OK;
	}
    
	Int32 lnForm = 0;
	
	return ipSortDefn->get_AttributeAndForm(opUnit, &lnForm);
}

// Check if there is any missing input slice.
// If there is any, propogate it to the output slice
// 814074, fali
int DSSSliceCalculator::hPropogateMissing(Int32 nSliceIn, DSSCubeDataSlice **ppSliceIn,
                                          Int32 nUnit, Int32 *pUnit, DSSCubeDataSlice **ppSliceOut,
                                          bool  &oMissingFound)
{
    HRESULT hr = S_OK;
    
    Int32 iSlice = 0;
    for(iSlice = 0; iSlice < nSliceIn; iSlice ++)
    {
        DSSDataType_Type lInputType = DssDataTypeUnknown;
        lInputType = ppSliceIn[iSlice]->getDataType();
        CHECK_HR;
        
        if (lInputType == DssDataTypeMissing) {
            hr = mpContext->mpCube->CreateDataSlice(nUnit, pUnit, DssDataTypeMissing, ppSliceOut);
            CHECK_HR_PTR(*ppSliceOut);
            
            oMissingFound = true;
            
            return S_OK;
        }
    }
    
    oMissingFound = false;
    
    return S_OK;
    
}

// qjiang TQMS 334423
// in generic functions and simple functions, need to translate the conditions of the slices to real flag array
int DSSSliceCalculator::hProcessConditionalMetric(Int32 nSliceIn, DSSCubeDataSlice** ppSliceIn)
{
	HRESULT hr = S_OK;
    
	// first set the current info "Where"
	if (mpContext->mpMaskInfos && !mpContext->mpMaskInfos->GetConditionalLogicFlag()) //896649 don't reset for conditional logic
		mpContext->mpMaskInfos->Reset();
    
	for (Int32 i = 0; i < nSliceIn; i++)
	{
		hr = ppSliceIn[i]->CompleteCondition();
		CHECK_HR;
	}
    
	return S_OK;
}

int DSSSliceCalculator::hIsNumericalSlice(DSSCubeDataSlice *ipDataSlice, bool &obNumericalSlice, bool &obCellFormatSlice)
{
    EnumDSSDataType inputType = ipDataSlice->getDataType();
	
	switch (inputType)
	{
			
		case DssDataTypeInteger :
		case DssDataTypeUnsigned :
		case DssDataTypeNumeric :
		case DssDataTypeDecimal :
		case DssDataTypeReal :
		case DssDataTypeDouble :
		case DssDataTypeFloat :
		case DssDataTypeShort :
		case DssDataTypeLong :
		case DssDataTypeBool :
		case DssDataTypeBigDecimal :
		case DssDataTypeMissing:
        case DssDataTypeInt64:
			obNumericalSlice = true;
            obCellFormatSlice = false;
            break;
			
		case DssDataTypeCellFormatData:
            obNumericalSlice = true;
            obCellFormatSlice = true;
            break;
			
		default :
			obNumericalSlice = false;
            obCellFormatSlice = false;
            break;
	}

    return S_OK;
}

