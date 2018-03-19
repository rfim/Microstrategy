/*
 *  DSSDimMetricEvaluator.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 1/12/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#include <set>
#import "DSSDimMetricEvaluator.h"
#import "Filter/DSSExpNodeOperator.h"
#import "Filter/DSSExpNodeShortcut.h"
#import "Filter/DSSCExpression.h"
#import "CalculationEngine/DSSCalculationPlan.h"
#import "DSSFunctionServer.h"
#import "DSSStrongPtr.h"
#import "DSSStdMathPackage.h"
#import "DSSDataModelXTabImpl.h"
#import "DSSTabularUnit.h"
//#import "MetricNameParser.h"
#import "DSSTabularMetrics.h"
#import "DSSCalculationHelper.h"
#import "DSSExpNodeFormShortcut.h"
#import "DSSFunction.h"
#import "DSSAggMetric.h"
#import "DSSDimty.h"
#import "DSSTabularData.h"
#import "DSSTabularRefConsolidation.h"
#import "DSSRWGridModel.h"
#import "DSSRWDataModel.h"
#include "SimpleBuffer.h"
#import "DSSSubtotalEventEvaluator.h"
#import "DSSCubeDataSlice.h"
#import "DSSTabularIndex.h"
#import "DSSLevelResolver.h"
#import "DSSTemplateDimension.h"
#import "DSSFunctionObject.h"
#import "DSSTabularIndexPool.h"
#include <algorithm>

DSSDimMetricEvaluator::DSSDimMetricEvaluator(EventContext *ipContext, EnumNullCheckingOption iOption)
: mpEventContext(ipContext), mNullCheckingOption(iOption), mbSubtotalContext(false)
, mbFilterEvaluation(false), mpIntermediateCube(NULL),mbNonSmart(false)
{
    mpBuffer = new MBase::SimpleBuffer();
    mpLevelResolver = new DSSLevelResolver(ipContext->mpCube, ipContext->mpSchema);
    mpLevelResolver->Init();
}

DSSDimMetricEvaluator::~DSSDimMetricEvaluator()
{
    if(mpBuffer)
    {
        delete mpBuffer;
        mpBuffer = NULL;
    }
    
    if(mpLevelResolver)
    {
        delete mpLevelResolver;
        mpLevelResolver = NULL;
    }
    
    if (mpIntermediateCube)
    {
        delete mpIntermediateCube;
        mpIntermediateCube = NULL;
    }
}

int DSSDimMetricEvaluator::CalculateDimMetric(METRIC_ID &irDimMetricID, DSSSliceID *opSliceID)
{
    return CalculateDimMetric(irDimMetricID.mMetricID, opSliceID);
}

int DSSDimMetricEvaluator::CalculateDimMetric(GUID &irDimMetricID, DSSSliceID *opSliceID)
{
    int hr = S_OK;
    if(!mpEventContext->mpCube->getTabularData())//when connected to 931 i-server, the tabularData will be NULL
    {
        IDSSTemplateModel* lpDataModel = mpEventContext->mpDataModel;
        CHECK_PTR(lpDataModel);
        
        DSSDataModelXTabImpl* lpXTabModel = dynamic_cast<DSSDataModelXTabImpl*>(lpDataModel);
        CHECK_PTR(lpXTabModel);
        
        DSSTabularData* lpTabularData = lpXTabModel->getTabularData();
        CHECK_PTR(lpTabularData);
        
        mpEventContext->mpCube->setTabularData(lpTabularData);
        
        DSSTabularIndexPool* lpIndexPool = NULL;
        hr = mpEventContext->mpCube->getIndexPool(&lpIndexPool);
        CHECK_HR_PTR(lpIndexPool);
        
        hr = lpIndexPool->Init(lpXTabModel);
        CHECK_HR;
        
        hr = lpTabularData->setIndexPool(lpIndexPool);
        CHECK_HR;
    }
        //TODO:store the intermediate result in a separate cube ???
        DSSCube *lpCube = new DSSCube();
        CHECK_NEWED_PTR(lpCube);
        mpIntermediateCube = lpCube;
        mpIntermediateCube->Init(mpEventContext->mpCube->getTabularData(), true);
 
	DSSObjectInfo *lpObj = mpEventContext->mpObjectContext->FindObjectH(irDimMetricID);
    CHECK_PTR(lpObj);
	DSSMetric *lpDimMetric = static_cast<DSSMetric*> (lpObj);	
	 
	return CalculateDimMetric_0(irDimMetricID, lpDimMetric, 0, false, opSliceID);
}

int DSSDimMetricEvaluator::CalculateDimMetric_0(GUID iDimMetricID, 
						 DSSMetric *ipDimMetricGov,
						 int iOffset, 
						 bool iIsIntermediate,
						 DSSSliceID *opSliceID)
{
	int hr = S_OK;
	
	//check if the component metric of govener metric is already calculated
	hr = hGetMetricSliceID(iDimMetricID, ipDimMetricGov, opSliceID);
	if (hr == S_OK)
		return hr;
	
	//not there yet, continue to process this metric
	DSSObjectInfo *lpObj = mpEventContext->mpObjectContext->FindObjectH(iDimMetricID);
	DSSMetric *lpDimMetric = static_cast<DSSMetric*> (lpObj);	
		
	//Check if metric is decomposable, if not return error
    
	if (mpEventContext->mpEvent->mEventType == DssEventCompoundMetric){
        EnumDSSDecomposable lDecomposable = lpDimMetric->getDecomposable();
		bool lIsDecomposable = (lDecomposable==DssDecomposableFalse || lDecomposable==DssDecomposableReserved) ? false : true;//mpEventContext->mpCalculationPlan->IsDecomposable(iDimMetricID);
		if(!lIsDecomposable){
            mbNonSmart = true;
        }

	}
	
    if(!mbNonSmart)
    {
        DSSExpNode *lpDimMetricRoot = lpDimMetric->getExpression()->Root();
        CHECK_PTR(lpDimMetricRoot);
	
        hr = CalculateDimExpression(lpDimMetricRoot,
						 		ipDimMetricGov,
								lpDimMetric,
								iOffset,
								iIsIntermediate,
                                opSliceID);
        CHECK_HR;
    }else
    {
        //Check if metric is decomposable, if not return error
        if (mpEventContext->mpEvent->mEventType == DssEventCompoundMetric){
            if(!mpEventContext->mpCalculationPlan)
                return hr = E_FAIL;
            
            bool lIsDecomposable = mpEventContext->mpCalculationPlan->IsDecomposable(iDimMetricID);
            if(!lIsDecomposable)
                return hr = E_FAIL;
        }
        
        DSSObjectInfo *lpObj = mpEventContext->mpObjectContext->FindObjectH(iDimMetricID);
        DSSMetric *lpDimMetric = static_cast<DSSMetric*> (lpObj);
        
        DSSExpNode *lpDimMetricRoot = lpDimMetric->getExpression()->Root();
        CHECK_PTR(lpDimMetricRoot);
        
        hr = CalculateDimExpression(lpDimMetricRoot,
                                    ipDimMetricGov,
                                    lpDimMetric,
                                    iOffset,
                                    iIsIntermediate,
                                    opSliceID);
        CHECK_HR;
    }
	return hr;
	
}

int DSSDimMetricEvaluator::CalculateDimExpression(
						   DSSExpNode *ipMetricRoot, 
						   DSSMetric *ipDimMetricGov, 
						   DSSMetric *ipDimMetric,
						   int iOffset, 
						   bool iIsIntermediate,
						   DSSSliceID *opSliceID)
{
	if(!ipMetricRoot) return E_FAIL;
	
	int hr = S_OK;
	
	// get type of the root node
	EnumDSSNodeType lNodeType = (EnumDSSNodeType)ipMetricRoot->getType();
	switch(lNodeType){
		case DssNodeOperator:
		{
			// <DimMetric> op <DimMetric>
			// op must be a simple function such as +,-,*,/

			DSSExpNodeOperator *lpOperator = static_cast<DSSExpNodeOperator*> (ipMetricRoot);
            
            DSSFunction *lpFunction = NULL;
            EnumDSSFunctionSQLType lFuncSQLType = DssFunctionSQLTypeReserved;
            if(!mbNonSmart)
            {
                lpFunction = lpOperator->getFunction();
                CHECK_PTR(lpFunction);
                
                lFuncSQLType = lpFunction->getSQLType();
                if((lFuncSQLType != DssFunctionSQLTypeArithmetic) &&
                   (lFuncSQLType != DssFunctionSQLTypeRelative ) &&
                   (lFuncSQLType != DssFunctionSQLTypeComparison) &&
                   (lFuncSQLType != DssFunctionSQLTypeLogic))
                    return E_FAIL;
            }
            
                int lnChildCount = ipMetricRoot->Count();
                //_ASSERT(lnChildCount > 0); // some functions has no operand, e.g. CurrentDate().
                
                // get sliceID for each children
                vector<DSSSliceID> lvSliceID(lnChildCount);
                DSSSliceID *lpChildSliceID = &lvSliceID[0];
                
                int iChild = 0;
                for(iChild = 0; iChild < lnChildCount; iChild++)
                {
                    DSSExpNode *lpChildNode = ipMetricRoot->Item(iChild);
                    CHECK_PTR(lpChildNode);
                    
                    EnumDSSNodeType lChildNodeType = (EnumDSSNodeType)lpChildNode->getType();
                    
                    switch(lChildNodeType){
                        case DssNodeShortcut:
                        {
                            DSSExpNodeShortcut *lpShortcut = static_cast<DSSExpNodeShortcut*> (lpChildNode);
                            DSSObjectInfo *lpTarget = lpShortcut->getTarget();
                            
                            // should be a Dim DSSMetric
                            if (lpTarget->getType() == DssTypeMetric)
                            {
                                hr = CalculateDimMetric_0(lpTarget->getID(), ipDimMetricGov, iOffset,  true, &lpChildSliceID[iChild]);
                                CHECK_HR;
                            }
                            else
                            {
                                return hr = E_FAIL;
                            }
                        }
                            break;
                        case DssNodeConstant:
                        case DssNodeTime:
                        case DssNodeBigDecimal:
                        case DssNodeCellFormatData:
                        {
                            // need to construct a constant slice
                            hr = DSSCalculationHelper::GenerateConstantSlice(lpChildNode, mpEventContext->mpCube, &(lpChildSliceID[iChild]));
                            CHECK_HR;
                        }
                            break;
                            
                        case DssNodeOperator:
                        {
                            bool lbIntermediate = mpIntermediateCube ? true : false;
                            hr = CalculateDimExpression(lpChildNode, ipDimMetricGov, ipDimMetric, iOffset, lbIntermediate, &lpChildSliceID[iChild]);
                            CHECK_HR;
                        }
                            break;
                        default:
                            break;
                    } // end of switch(lChildNodeType)
                    
                }// end of for each child
                
                EnumSimpleOperatorType lFuncIndex = (EnumSimpleOperatorType)lpOperator->getFunctionIndex();
                if (lFuncIndex == DssFuncUnknown)
                {
                    //this is just to keep backward compatibility with old rwd engine
                    int lOperatorType = lpOperator->getOperatorType();
                    if(lOperatorType == DssFunctionIF)
                    {
                        lFuncIndex = DssFuncIF;
                    }else{
                        lFuncIndex = (EnumSimpleOperatorType)lpOperator->getOperatorType();
                    }

                }
                vector<DSSCubeDataSlice*> lvSlice;
                vector<DSSSliceID*> lvpSliceID;
                for (int i = 0; i < lvSliceID.size(); i++)
                {
                    lvpSliceID.push_back(&(lvSliceID[i]));
                    
                    DSSCubeDataSlice *lpSlice = NULL;
                    hr = lvpSliceID[i]->GetSlice(&lpSlice);
                    CHECK_HR_PTR(lpSlice);
                    lvSlice.push_back(lpSlice);
                }
                
                CalculationContext *lCalcContext = DSSCalculationHelper::getCalculationContext(NULL,
                                                                                               lFuncIndex,
                                                                                               iIsIntermediate ? mpIntermediateCube : mpEventContext->mpCube,
                                                                                               NULL,
                                                                                               &lvpSliceID,
                                                                                               mNullCheckingOption,
                                                                                               mpEventContext->mpSchema);
                MBase::DSSStrongPtr<CalculationContext> lCalcContextPtr(lCalcContext);
            
                if (mbNonSmart || (lFuncSQLType == DssFunctionSQLTypeArithmetic
                    || lFuncSQLType == DssFunctionSQLTypeComparison
                    || lFuncSQLType == DssFunctionSQLTypeLogic))
                {
                    hr = DSSCalculationHelper::GetUnionLevel(lvSlice, lCalcContext->mvBreakbyUnits);
                    CHECK_HR;
                    
                    //DSSFunctionServer *lpFunctionServer = new DSSFunctionServer();
                    //CHECK_PTR(lpFunctionServer);
                    //MBase::DSSStrongPtr<DSSFunctionServer> lFunctionServerPtr (lpFunctionServer);
                    
                    // jimzhang, Nov/13/2012, begin, hard code to support Mod().
                    //On server side, Mod() is in StdMathSTOS package, and its function index is 33. But at current, only function type (CDSSFunction::mFunctionType on server side) is serialized to client, and function type of Mod() is -1. So, to support Mod() on client, hard code its function type to a valid value here.
                    if (lCalcContext->mFuncIndex < 0)
                    {
                        //getExpressionType()  =	DssExpressionGeneric
                        EnumDSSExpressionType lExprType = lpOperator->getExpressionType();
                        int lnOperands = lpOperator->Count();
                        if (DssExpressionGeneric == lExprType && 2 == lnOperands)
                            lCalcContext->mFuncIndex = DssFunctionMathMod;
                    }
                    if(mbNonSmart)//hard code for 941 workflow
                    {
                        int lFunctionOperatrorType = lpOperator->getOperatorType();
                        if(lFunctionOperatrorType == -1)
                        {
                            EnumDSSExpressionType lExprType = lpOperator->getExpressionType();
                            int lnOperands = lpOperator->Count();
                            if (DssExpressionGeneric == lExprType && 2 == lnOperands)
                            {
                                DSSFunctionPackage* lpMathPkg = new DSSStdMathPackage();
                                MBase::StrongPtr<DSSFunctionPackage> lMathPkgPtr(lpMathPkg);
                                lCalcContext->mFuncIndex = (EnumSimpleOperatorType)34;
                                lCalcContext->mpFuncObject = lpMathPkg->Item(lCalcContext->mFuncIndex);
                            }
                        }
                    }
                    // jimzhang, Nov/13/2012, end
                    
                    hr = DSSCalculationHelper::CalculateNewSlice(lpOperator, lCalcContext, opSliceID);
                    CHECK_HR;
                }
                else
                {
                    //relative functions like rank
                    /*hr = DSSCalculationHelper::FindMetricBreakbyLevel(ipMetricRoot, lCalcContext->mpCube->getTabularData(), lvSlice, &lCalcContext->mvBreakbyUnits);*/
                    //tliao, 951691, 2014/8/14, support dynamic break by
                    hr = ReplaceDimty(ipDimMetric, lCalcContext->mpCube->getTabularData(), &lCalcContext->mvBreakbyUnits);
                    CHECK_HR;
                    
                    vector<DSSSortDefinition*>* lpvSortbyDefn = NULL;
                    //tliao, 964108, 2014/09/10, use "OrderSignificant" property to decide whether we need to reolve sortby
                    bool lfOrderSignificant = lpFunction->getOrderSignificant();
                    if (lfOrderSignificant)
                    {
                        //tliao, 954998, 2014/08/28, support dynamic sort by
                        DFCSimpleExpr* lpSortLevel = NULL;
                        hr = ReplaceSortBy(ipDimMetric, lCalcContext->mpCube->getTabularData(), lpSortLevel);
                        CHECK_HR;
                        
                        
                        if (lpSortLevel == NULL)
                        {
                            hr = DSSCalculationHelper::FindMetricSortbyDefns(ipMetricRoot, mpEventContext, lnChildCount, NULL, ipDimMetricGov, &lpvSortbyDefn);
                        }
                        else
                        {
                            hr = DSSCalculationHelper::FindMetricSortbyDefnsSimpleExpr(ipMetricRoot, lpSortLevel, mpEventContext, lnChildCount, NULL, ipDimMetricGov, &lpvSortbyDefn);
                        }
                        CHECK_HR;
                        
                        if (lpvSortbyDefn)
                            lCalcContext->mvSortbyDefn = *lpvSortbyDefn;
                    }
                    
                    hr = DSSCalculationHelper::CalculateNewSlice(lpOperator, lCalcContext, opSliceID);
                    CHECK_HR;
                    //tliao, 912747, 2014/08/04, fix memory leak
                    if (lpvSortbyDefn)
                    {
                        delete lpvSortbyDefn;
                    }
                }
                if (lCalcContext->mpFuncObject)
                {
                    //delete lCalcContext->mpFuncObject;
                    MBase::DSSStrongPtr<DSSFunctionObject> lFunctionObjectPtr(lCalcContext->mpFuncObject);
                }
            break;
		} // end of case DssNodeOperator
			
		case DssNodeShortcut:
		{
			// must be an <aggMetric> <Dimty>
			
			DSSExpNodeShortcut *lpShortCut = static_cast<DSSExpNodeShortcut*> (ipMetricRoot);
			DSSObjectInfo *lpTarget = lpShortCut->getTarget();
			
			if(lpTarget->getType() == DssTypeAggMetric)
			{				
				hr = CalculateAggMetric(lpTarget->getID(), ipDimMetricGov, ipDimMetric, iOffset, iIsIntermediate, opSliceID);
				CHECK_HR;
			}
			else if (lpTarget->getType() == DssTypeMetric)
			{
				hr = CalculateDimMetric_0(lpTarget->getID(), ipDimMetricGov, iOffset, iIsIntermediate, opSliceID);
				CHECK_HR;
				
			} else
				return  hr = E_FAIL;
			
			break;
			
		} // end of case DssNodeShortcut
			
		case DssNodeConstant:
		case DssNodeTime:
		case DssNodeBigDecimal:
		case DssNodeCellFormatData:
		{
			// need to construct a constant slice
			hr = DSSCalculationHelper::GenerateConstantSlice(ipMetricRoot, mpEventContext->mpCube, opSliceID);
			CHECK_HR;
			
		} // end of case DssNodeConstant
			
		default:
			return E_FAIL;
	} // end of switch(lNodeType)
	
	return S_OK;
}

int DSSDimMetricEvaluator::CalculateAggMetric(GUID iAggID, 
										   DSSMetric *ipDimMetricGov, 
										   DSSMetric *ipDimMetric,
										   int iOffset, 
										   bool iIsIntermediate,
										   DSSSliceID *opSliceID)
{
    DSSObjectInfo *lpMetricInfo = mpEventContext->mpObjectContext->FindObjectH(iAggID);
    CHECK_PTR(lpMetricInfo);
    
    DSSAggMetric *lpMetric = dynamic_cast<DSSAggMetric *>(lpMetricInfo);
    CHECK_PTR(lpMetric);
    
    DSSCExpression *lpMetricExpr = lpMetric->getExpression();
    CHECK_PTR(lpMetricExpr);
    
    DSSExpNode *lpMetricRoot = lpMetricExpr->Root();
    CHECK_PTR(lpMetricRoot);
    
	return CalculateAggExpression(lpMetricRoot, ipDimMetricGov, ipDimMetric, iOffset, iIsIntermediate, opSliceID);
}

int DSSDimMetricEvaluator::CalculateAggExpression(
						   DSSExpNode *ipMetricRoot, 
						   DSSMetric *ipDimMetricGov, 
						   DSSMetric *ipDimMetric,
						   int iOffset, 
						   bool iIsIntermediate,
						   DSSSliceID *opSliceID)
{
	if(!ipMetricRoot) return E_FAIL;
	
	int hr = S_OK;
	
	EnumDSSNodeType lNodeType = (EnumDSSNodeType)ipMetricRoot->getType();
	
	switch(lNodeType)
	{
		case DssNodeOperator:
		{
			DSSExpNodeOperator *lpOperator = static_cast<DSSExpNodeOperator*> (ipMetricRoot);
			
			//EnumDSSFunctionSQLType lSQLFuncType = DssFunctionSQLTypeAggregation;
			//lSQLFuncType = lpFunc->get_SQLTypeNS(&lSQLFuncType);
            
            //the root node of an aggregation metric should not be simple function
            //AE_ASSERT(DssFunctionSQLTypeAggregation == lSQLFuncType);
            
            DSSFunction *lpFunction = lpOperator->getFunction();
            CHECK_PTR(lpFunction);
            
            EnumDSSFunctionSQLType lFuncSQLType = lpFunction->getSQLType();
            switch (lFuncSQLType)
            {
                case DssFunctionSQLTypeArithmetic:
                case DssFunctionSQLTypeRelative:
                case DssFunctionSQLTypeComparison:
                case DssFunctionSQLTypeLogic:
                {
                    int lnChildCount = ipMetricRoot->Count();
					CHECK_HR;
                    
					// get sliceID for each child
                    vector<DSSSliceID> lvSlicedID(lnChildCount);
                    DSSSliceID *lpChildSliceID = &lvSlicedID[0];
                    
					for(int iChild = 0; iChild < lnChildCount; iChild++)
					{
                        DSSExpNode *lpChildNode = ipMetricRoot->Item(iChild);
                        CHECK_PTR(lpChildNode);
                        
						hr = CalculateAggExpression(lpChildNode, ipDimMetricGov, ipDimMetric, iOffset, true, &(lpChildSliceID[iChild]));
						CHECK_HR;
					}

                    EnumSimpleOperatorType lFuncIndex = (EnumSimpleOperatorType)lpOperator->getFunctionIndex();
                    if (lFuncIndex == DssFuncUnknown)
                    {
                        //this is just to keep backward compatibility with old rwd engine
                        lFuncIndex = (EnumSimpleOperatorType)lpOperator->getOperatorType();
                    }
                    vector<DSSSliceID*> lvpSliceID;
                    for (int i = 0; i < lvSlicedID.size(); i++)
                    {
                        lvpSliceID.push_back(&(lvSlicedID[i]));
                    }
                    
                    CalculationContext *lCalcContext = DSSCalculationHelper::getCalculationContext(NULL,
                                                                                                   lFuncIndex,
                                                                                                   iIsIntermediate ? mpIntermediateCube:mpEventContext->mpCube,
                                                                                                   NULL,
                                                                                                   &lvpSliceID,
                                                                                                   mNullCheckingOption,
                                                                                                   mpEventContext->mpSchema);
                    MBase::DSSStrongPtr<CalculationContext> lCalcContextPtr(lCalcContext);
                    
                    hr = DSSCalculationHelper::CalculateNewSlice(lpOperator, lCalcContext, opSliceID);
                    if (lCalcContext->mpFuncObject)
                    {
                        //delete lCalcContext->mpFuncObject;
                        MBase::DSSStrongPtr<DSSFunctionObject> lFunctionObjectPtr(lCalcContext->mpFuncObject);
                    }
                    CHECK_HR;
                }
                    break;
                    
                case DssFunctionSQLTypeAggregation:
                {   
                    int lnChildren = ipMetricRoot->Count();
                    vector<DSSSliceID> lvSlicedID(lnChildren);
                    DSSSliceID *lpChildSliceID = &lvSlicedID[0];
                    
                    bool lLevelSignificant = true;
                    hr = DSSCalculationHelper::getIsInputSignificantProperty(lpFunction, lpOperator, lLevelSignificant);
                    CHECK_HR;
                    
                    EnumDSSFunction lFuncType = (EnumDSSFunction)lpOperator->getOperatorType();
                    
                    //ctang 2010-4-29
					bool lfIDColumn = false;
                    
                    int iChild = 0;
                    for(iChild = 0; iChild < lnChildren; iChild ++)
                    {
                        DSSExpNode *lpChildNode = ipMetricRoot->Item(iChild);
                        CHECK_PTR(lpChildNode);
                        
                        hr = CalculateAggElement(lpChildNode, ipDimMetricGov, ipDimMetric, iOffset, true, &lpChildSliceID[iChild], lFuncType, &lfIDColumn, lLevelSignificant);
                        CHECK_HR;
                    }
                    
                    vector<DSSSortDefinition*> lvSortDefns;
                    //tliao, 964108, 2014/09/10, use "OrderSignificant" property to decide whether we need to reolve sortby
                    bool lfOrderSignificant = lpFunction->getOrderSignificant();
                    if (lfOrderSignificant)
                    {
                        //tliao, 954998, 2014/08/28, support dynamic sort by
                        DFCSimpleExpr* lpSortLevel = NULL;
                        hr = ReplaceSortBy(ipDimMetric, mpEventContext->mpCube->getTabularData(), lpSortLevel);
                        CHECK_HR;
                        
                        vector<DSSSortDefinition*> *lpvSortDefns = NULL;
                        
                        if (lpSortLevel == NULL)
                        {
                            //tliao, 2014/08/28, if there is no resolved sort by, use the old way
                            hr = DSSCalculationHelper::FindMetricSortbyDefns(ipMetricRoot, mpEventContext, lnChildren, NULL, ipDimMetricGov, &lpvSortDefns);
                        }
                        else
                        {
                            hr = DSSCalculationHelper::FindMetricSortbyDefnsSimpleExpr(ipMetricRoot, lpSortLevel, mpEventContext, lnChildren, NULL, ipDimMetricGov, &lpvSortDefns);
                        }
                        CHECK_HR;
                        
                        if (lpvSortDefns)
                        {
                            lvSortDefns.assign(lpvSortDefns->begin(), lpvSortDefns->end());
                            //tliao, 912747, 2014/08/04, fix memory leak
                            lpvSortDefns->clear();
                            delete lpvSortDefns;
                        }
                    }
                    /*
                     VARIANT_BOOL lfOrderSignificant;
                     hr = lpFunc->get_IsOrderSignificantNS(&lfOrderSignificant);
                     CHECK_HR;
                     
                     MBase::StrongBufferPtr<CDSSXTabSortDefns> lvSortDefns;
                     if(lfOrderSignificant == VARIANT_TRUE &&
                     mpTemplate != NULL)
                     {
                     hr =  CDSSCalculationHelper::FindMetricSortbyDefns(
                     ipMetricRoot,
                     mLevel,
                     mEvent,
                     mpTemplate,
                     mpSourceCube,
                     mpCalculationPlan,
                     mpReportInstance,
                     lnChildren,
                     lpChildSliceID,
                     mpBuffer,
                     &lvSortDefns
                     );
                     CHECK_HR;
                     }*/
                    
                    if (mpEventContext->mpEvent->mEventType == DssEventTemplateSubtotal)
                    {
                        hr = subtotalAgg(lnChildren, lpChildSliceID, ipDimMetric, iOffset, lpOperator, lvSortDefns, iIsIntermediate, opSliceID);
                        CHECK_HR;
                    }
                    else
                    {
                        hr = normalAgg(lnChildren, lpChildSliceID, ipDimMetric, lpOperator, lvSortDefns, iIsIntermediate, opSliceID, lfIDColumn);
                        CHECK_HR;
                    }
                }
                    break;
                default:
                    return E_FAIL;
            }
		}// end of case DssNodeOperator
            break;
			
		case DssNodeShortcut:
			// must be an aggregation function
		{
			DSSExpNodeShortcut *lpShortcut = static_cast<DSSExpNodeShortcut*> (ipMetricRoot);
			if(lpShortcut->getTarget()->getType()!= DssTypeAggMetric)
				return E_FAIL;
			
			DSSObjectInfo *lpTarget = lpShortcut->getTarget();
			CHECK_PTR(lpTarget);
			
			hr = CalculateAggMetric(lpTarget->getID(), ipDimMetricGov, ipDimMetric, iOffset, iIsIntermediate, opSliceID);
			CHECK_HR;
			
			break;
		}
		case DssNodeConstant:
		case DssNodeTime:
		case DssNodeBigDecimal:
		case DssNodeCellFormatData:
		{
			// need to construct a constant slice
			hr = DSSCalculationHelper::GenerateConstantSlice(ipMetricRoot, mpEventContext->mpCube, opSliceID);
			CHECK_HR;
		}
			
		default:
			return E_FAIL;
	}
	return S_OK;
}

int DSSDimMetricEvaluator::CalculateAggElement(DSSExpNode *ipRoot ,DSSMetric *ipDimMetricGov,DSSMetric *ipDimMetric,
                                               int iOffset, bool iIsIntermediate, DSSSliceID *opSliceID, EnumDSSFunction iFuncType,
                                               //ctang 2010-4-29
                                               bool *obIDColumn,
                                               bool iLevelSignificant)
{
	if(!ipRoot) return E_FAIL;
    int hr = S_OK;
	EnumDSSNodeType lNodeType = ipRoot->getType();
	switch(lNodeType)
	{
        case DssNodeOperator:
		{
            DSSExpNodeOperator *lpOperator = static_cast<DSSExpNodeOperator *>(ipRoot);
            DSSFunction* lpFunction = lpOperator->getFunction();
            CHECK_PTR(lpFunction);
            
            EnumDSSFunction lFuncType = (EnumDSSFunction)lpOperator->getOperatorType();
            
            EnumDSSFunctionSQLType lSQLFuncType = lpFunction->getSQLType();
            // must be simple arithemetic function such as +,-,*,/
			if((lSQLFuncType != DssFunctionSQLTypeArithmetic) &&
			   (lSQLFuncType != DssFunctionSQLTypeRelative ) &&
			   (lSQLFuncType != DssFunctionSQLTypeComparison) &&
			   (lSQLFuncType != DssFunctionSQLTypeLogic))
				return E_FAIL;

            // get child count of the node
			Int32 lnChildCount = ipRoot->Count(); 

            DSSSliceID *lpChildSliceID = new DSSSliceID[lnChildCount];
            
			for(int iChild = 0; iChild < lnChildCount; iChild++)
			{
                DSSExpNode *lpNode = ipRoot->Item(iChild);
				hr = CalculateAggElement(lpNode, ipDimMetricGov, ipDimMetric, iOffset, true, &(lpChildSliceID[iChild]), lFuncType);
				CHECK_HR;
                
                //8/26/2014,xzhu,delete
                //lpChildSliceID[iChild].mpCube = mpEventContext->mpCube;
			}
            
			// =====================================================
			// incomplete_calculate
			// calculate the slice for the current operator
			//      return opSliceID back
			// =====================================================
            
            
            
            MBase::Allocator<Int32> lAlloc(mpBuffer);
            vector<Int32, MBase::Allocator<Int32> > lvUnits(lAlloc);
            hr = DSSCalculationHelper::FindCommonLevel(lnChildCount, lpChildSliceID, mpBuffer, lvUnits);
            CHECK_HR;
            
            vector<int> lvBreakbyUnits;
            lvBreakbyUnits.assign(lvUnits.begin(), lvUnits.end());
            
            EnumSimpleOperatorType lFuncIndex = (EnumSimpleOperatorType)lpOperator->getFunctionIndex();
            
            vector<DSSSliceID*> lvSliceID;
            for (int i = 0; i < lnChildCount; i++)
            {
                lvSliceID.push_back(&(lpChildSliceID[i]));
            }
            
            CalculationContext *lCalcContext = DSSCalculationHelper::getCalculationContext(NULL,
                                                                                           lFuncIndex,
                                                                                           iIsIntermediate? mpIntermediateCube:mpEventContext->mpCube,
                                                                                           &lvBreakbyUnits,
                                                                                           &lvSliceID,
                                                                                           mNullCheckingOption,
                                                                                           mpEventContext->mpSchema);
            MBase::DSSStrongPtr<CalculationContext> lCalcContextPtr(lCalcContext);
            
            hr = DSSCalculationHelper::CalculateNewSlice(lpOperator, lCalcContext, opSliceID);
            if (lCalcContext->mpFuncObject)
            {
                //delete lCalcContext->mpFuncObject;
                MBase::DSSStrongPtr<DSSFunctionObject> lFunctionObjectPtr(lCalcContext->mpFuncObject);
            }
            CHECK_HR;

			/*
             CComQIPtr<IDSSOperator, &IID_IDSSOperator> ldpOperator(lpOperator);
             if(!ldpOperator) return E_FAIL;
             
            vector<Int32> lvUnits;
			hr = DSSCalculationHelper::FindCommonLevel(lnChildCount, lpChildSliceID, mpBuffer, lvUnits);
			CHECK_HR;
            
			Int32 lvUnitsSize = lvUnits.size();
			hr = DSSCalculationHelper::CalculateNewSlice(
                                                          mpSource,
                                                          mpFunctionServer,
                                                          ldpOperator,
                                                          iIsIntermediate? mpIntermediateCube.p:mpTargetCube,
                                                          lvUnitsSize,
                                                          (lvUnitsSize>0)?&lvUnits[0]:NULL,
                                                          lnChildCount,
                                                          lpChildSliceID,
                                                          mNullCheckingOption,
                                                          opSliceID,
                                                          NULL,
                                                          ipDimMetricGov,
                                                          mpReportInstance);
			CHECK_HR;
            
            */
		}
            
            break;
            
        case DssNodeShortcut:
		{
			// Dim metric or Me metric
            DSSExpNodeShortcut * lpShortcut= static_cast<DSSExpNodeShortcut *>(ipRoot);
            DSSObjectInfo *lpObjInfo = lpShortcut->getTarget();
            //tliao, 1/21/2014, 835860, avoid crash
            if(!lpObjInfo) return E_FAIL;
            EnumDSSObjectType lInfoType = lpObjInfo->getType();
            
			switch (lInfoType){
				case DssTypeAttribute:
                {
                    DSSAttribute *lpAttr = (DSSAttribute *)lpObjInfo;
                    GUID lAttrID = lpAttr->getID();
                    DSSTabularData *lpTabularData = mpEventContext->mpCube->getTabularData();
                    int lAttributeUnit = lpTabularData->FindUnit(lAttrID, DssTemplateAttribute);
                    if (lAttributeUnit < 0)
                    {// 814074, fali, for the unit which can't be found, it may be the parameter which is not in the template but used in the function(like count). It should diplay Missing null
                        bool lDummy = false;
                        hr = DSSCalculationHelper::GetConstantSlice(iIsIntermediate?mpIntermediateCube:mpEventContext->mpCube, 0, 0, (char *)&lDummy, sizeof(BOOL), DssDataTypeMissing, DssDataOk, opSliceID);
                        CHECK_HR;
                    }
                    else
                    {
                        hr = DSSCalculationHelper::GetPropertySlice(iIsIntermediate?mpIntermediateCube:mpEventContext->mpCube, lAttributeUnit, 0, 0, opSliceID, false, iLevelSignificant);
                        CHECK_HR;
                    }
                    //ctang 2010-4-29
                    if (/*!lfDE && */obIDColumn) // qjiang, no optimization for DE right now
                        *obIDColumn = true;
                    
                    //lwang, 10/09/2007, TQMS 257992, add support for derive metric based on attribute
                    // like AggrFunc(Attr), e.g. DM1 = Count(Month);
                    // ID form will be used for calculation
                  /*
                    CComPtr<ICDSSObjectInfo> lpAttrInfo;
                    hr = lpShortcut->get_Target(&lpAttrInfo);
                    CHECK_HR_PTR(lpAttrInfo);
                    
                    CComQIPtr<ICDSSAttribute, &IID_ICDSSAttribute> lpAttribute(lpAttrInfo);
                    CHECK_PTR(lpAttrInfo);
                    
                    Int32 lArity = -1;
                    hr = lpAttribute->get_ArityNS(&lArity);
                    CHECK_HR;
                    
                    // qjiang TQMS452365 we can support count on compound id now, for other aggregation functions, still fail it 
                    if (lArity > 1 && iFuncType != DssFunctionCount)
                        return E_FAIL;	//can't have more than one form as metric calculation input
                    
                    DSS_ID lAttributeID;
                    hr = lpShortcut->get_TargetID(&lAttributeID);
                    CHECK_HR;
                    
                    bool lfDE = false;
                    Int32 lAttributeUnit = -1;
                    hr = mpTabularData -> FindUnitI(lAttributeID, DssTemplateAttribute, &lAttributeUnit);
                    if(hr != S_OK)
                    {
                        hr = FindDerivedElementUnit(lAttributeID, lAttributeUnit);
                        CHECK_HR;
                        
                        lfDE = true;
                    }
                    
                    // get the first ID form slice.
                    hr = CDSSCalculationHelper::GetPropertySlice(mpTargetCube, lAttributeUnit, 0, 0, opSliceID, false, iLevelSignificant); 
                    CHECK_HR;
                    //ctang 2010-4-29
                    if (!lfDE && obIDColumn) // qjiang, no optimization for DE right now
                        *obIDColumn = true;*/
                }
					break;
				case DssTypeMetric:
				case DssTypeAggMetric:
                {
                    // Nested aggregation
                    /*
                    DSS_ID lID;
                    hr = lpShortcut->get_TargetID(&lID);
                    CHECK_HR;
                    */
                    DSSObjectInfo *lpTarget = lpShortcut->getTarget();
                    GUID lID = lpTarget->getID();
                    
                    hr = CalculateDimMetric_0(lID, ipDimMetricGov, iOffset+1, iIsIntermediate, opSliceID);
                    CHECK_HR;
                }
					break;
				default:
					return E_FAIL;
					break;
			}
		}
            
            break;
            
        case DssNodeMe:
		{
			// the calculation is based on the me metric
			hr = hGetMetricSliceID(mMeMetricID, ipDimMetricGov, opSliceID);
			return hr;
		}
            
            break;
            
        case DssNodeConstant:
        case DssNodeTime:
        case DssNodeBigDecimal:
        case DssNodeCellFormatData:
		{
			// need to construct a constant slice
			// hr = get_ConstantSlice(ipRoot, ipCube, opSliceID);
            hr = DSSCalculationHelper::GenerateConstantSlice(ipRoot, mpEventContext->mpCube, opSliceID);
			return hr;
		}
            break;
            
        case DssNodeFormShortcut:
		{
			//xhan 02/13/2006 To take attribute for as calculation input
			//We don't worry the Governing Metric for the attribute form, because 
			//an attribute form won't have its own subtotals like a metric has and there is no
			//ambiguity if the attribute form is in compound metric metric expression.
			//The metric expression looks like ipDimMetric = AggFunc(Attr@Form)
            
            DSSExpNodeFormShortcut *lpFormShorCut = (DSSExpNodeFormShortcut*) ipRoot;
            
            DSSCube *lpCube = iIsIntermediate? mpIntermediateCube:mpEventContext->mpCube;
            DSSTabularData *lpTabularData = lpCube->getTabularData();
            CHECK_PTR(lpTabularData);
            
            DSSAttribute *lpAttr = lpFormShorCut->getAttribute();
            GUID lAttrGUID = lpAttr->getID();
            
            int lAttrPos = lpTabularData->FindUnit(lAttrGUID, DssTemplateAttribute);
            
            //fali, 824676, For those attribute which can't be found, generate missing slice
            if (lAttrPos < 0)
            {
                bool lDummy =false;
                hr = DSSCalculationHelper::GetConstantSlice(lpCube, 0, 0, (char *)&lDummy, sizeof(BOOL), DssDataTypeMissing, DssDataOk, opSliceID);
                CHECK_HR;
            }
            else
            {
                hr = DSSCalculationHelper::GenerateAttributeFormInputs(lpCube, lpFormShorCut, opSliceID, iLevelSignificant);
                CHECK_HR;
            }           
            
            
            if (obIDColumn)
                *obIDColumn = true;
          /*
			CComQIPtr<ICDSSFormShortcut, &IID_ICDSSFormShortcut> licpFormShortcut(ipRoot);
			CHECK_PTR(licpFormShortcut);
            
			DSS_ID lAttributeID;
			hr = licpFormShortcut -> get_AttributeID(&lAttributeID);
			CHECK_HR;
            
			bool lfDE = false;
			Int32 lAttributeUnit = -1;
			hr = mpTabularData -> FindUnitI(lAttributeID, DssTemplateAttribute, &lAttributeUnit);
			if(hr != S_OK)
			{
				hr = FindDerivedElementUnit(lAttributeID, lAttributeUnit);
				CHECK_HR;
                
				lfDE = true;
			}
            
			MBase::Allocator<CDSSSliceID> lAlloc(mpBuffer);
			std::vector<CDSSSliceID, MBase::Allocator<CDSSSliceID> > lvSliceID(lAlloc);
			hr = CDSSCalculationHelper::GenerateAttributeFormInputs(
                                                                    licpFormShortcut,
                                                                    iIsIntermediate? mpIntermediateCube.p:mpTargetCube,
                                                                    lvSliceID,
                                                                    lAttributeUnit,
                                                                    //ctang 2010-4-29
                                                                    lfDE ? NULL : obIDColumn,
                                                                    iLevelSignificant); // qjiang TQMS 443641
            
			bool lfCountID = (iFuncType == DssFunctionCount) && (obIDColumn && *obIDColumn);
            
			// qjiang TQMS452365 we can support count on compound id now, for other aggregation functions, still fail it 
			if(lvSliceID.size() != 1 && !lfCountID) // can't have more than one form as metric calculation input
				return E_FAIL;
			
            
			*opSliceID = lvSliceID[0];
			*/
            
		}
            break;
            
        default: return E_FAIL;
	}

	return hr;
}

int DSSDimMetricEvaluator::normalAgg(int nChild,
                                     DSSSliceID *ipChildSliceID,
                                     DSSMetric *ipDimMetric,
                                     DSSExpNodeOperator *ipOperator,
                                     vector<DSSSortDefinition *> & irpSortDefns,
                                     bool iIsIntermediate,
                                     DSSSliceID *opSliceID,
                                     //ctang 2010-4-29
                                     bool bIsOnIDColumn)
{
    int hr = S_OK;
	vector<Int32> lvAggUnits;
    
    //find aggregation metric level
	hr = ReplaceDimty(ipDimMetric, mpEventContext->mpCube->getTabularData(), &lvAggUnits);
	if (hr != S_OK)
        return hr;
    
	//xhan delete bogus level checking. The aggregtion level resolution is done inside aggregation function itself
	// weiwang 2010-05-12 TQMS388189 bring back level checking when MetricLevelDetermination flag is true
	//EnumDSSReportType lReportType = DssReportTypeReserved;
  
    //zhyang, how to know the report type?
//	if (mpReportInstance)
//		mpReportInstance->get_ReportType(&lReportType);
    
//	if (lReportType == DssReportTypeBase)
/*	{
		CComPtr<ICDSSReportDefinition> lCDef;
		hr = mpReportInstance->get_Definition(&lCDef);
		CHECK_HR;
		CComPtr<ICDSSObjectInfo> lObjInfo;
		hr = lCDef->get_Info(&lObjInfo);
		CHECK_HR_PTR(lObjInfo);
		DSS_ID lReportID;
		hr = lObjInfo->get_ID(&lReportID);
		CHECK_HR;
		CComVariant lVariant;
		hr = CDSSAEHelper::readProperty(mpReportInstance, "MetricLevelDetermination", DssMetricLevelDetermination, DssPropertySetAnalyticalEngineProperties, lReportID, DssTypeReportDefinition, lObjInfo, lVariant);
		if (hr == S_OK)
		{
			VariantChangeType(&lVariant, &lVariant, 0, VT_BOOL);
			if (lVariant.boolVal == VARIANT_TRUE)
			{
				//find metric slice level inside aggregation function.
				LongVector lInnerLevel(lAlloc);
				hr = get_SliceLevel(ipChildSliceID[0], &lInnerLevel);
				if(hr != S_OK) return hr;
                
				//Check if inner level is compatible with aggregation level
				//We say they are compatible if aggregation level is a subset of inner level
				//If not compatible , return fail
				if (!std::includes(lInnerLevel.begin(), lInnerLevel.end(), lvAggUnits.begin(), lvAggUnits.end()))
					return hr = E_FAIL;
			}
		}
	}
  */  
//	Int32 lAggUnitsSize= lvAggUnits.size();
//	Int32 *lpAggUnits = (lAggUnitsSize> 0 )?&lvAggUnits[0]:NULL;
	
/*    
	hr = DSSCalculationHelper::CalculateNewSlice(
                                                  mpSource,
                                                  mpFunctionServer,
                                                  ipOperator,
                                                  iIsIntermediate?mpIntermediateCube.p:mpTargetCube,
                                                  lAggUnitsSize,
                                                  lpAggUnits,
                                                  nChild,
                                                  ipChildSliceID,
                                                  mNullCheckingOption,
                                                  opSliceID,
                                                  ipSortDefns,
                                                  //mSessionId,
                                                  ipDimMetric,
                                                  mpReportInstance,
                                                  //ctang 2010-4-29
                                                  false,
                                                  bIsOnIDColumn);
  */
    /*
    CalculationContext lCalcContext;
    //lCalcContext.mFuncIndex = (EnumSimpleOperatorType)ipOperator->getOperatorType();
    lCalcContext.mFuncIndex = (EnumSimpleOperatorType)ipOperator->getFunctionIndex();
    lCalcContext.mpCube = mpEventContext->mpCube;
    
    //lCalcContext.mvSliceIDIn.push_back(*ipChildSliceID);
    for (int i = 0; i < nChild; i++)
    {
        lCalcContext.mvSliceIDIn.push_back(ipChildSliceID[i]);
    }
    
    lCalcContext.mNullCheckingOption = mNullCheckingOption;
    //vector<int> lvBreakByUnit;
    //zhyang, hard code
    //MetricNameParser::getBreakbyUnit(ipDimMetricGov->getName(), mpEventContext, &lvBreakByUnit);
    //hr = MetricNameParser::getBreakbyUnit(ipDimMetric->getName(), mpEventContext, &lvBreakByUnit);
    //CHECK_HR;
    sort(lvAggUnits.begin(), lvAggUnits.end());
    lCalcContext.mvBreakbyUnits.assign(lvAggUnits.begin(), lvAggUnits.end());
    //	hr = DSSCalculationHelper::GetUnionLevel(mpEventContext->mpTargetCube, lvSlicedID, lCalcContext.mvBreakbyUnits);

    //zhyang add sortby for agg
    //vector<DSSSort *> lvSort;
    //hr = MetricNameParser::getSorts(ipDimMetric->getName(), mpEventContext, lvSort);
    CHECK_HR;
    // jimzhang, Feb/27/2013, comment out below line of code to pass compiling
    //vector<DSSSortDefinition *>* lvSortbyDef = DSSCalculationHelper::FindMetricSortbyDefns(lvSort, mpEventContext);
    //vector<DSSSortDefinition *>* lvSortbyDef;
    //vector<DSSSortDefinition *>* lvSortbyDef = new vector<DSSSortDefinition *>;
    //MetricNameParser::getSortbyDefinations(ipDimMetricGov->getName(), mpEventContext, &lvSortbyDef);
    lCalcContext.mvSortbyDefn.assign(irpSortDefns.begin(), irpSortDefns.end());
    lCalcContext.mpSchema = mpEventContext->mpSchema;
    */
    EnumSimpleOperatorType lFuncIndex = (EnumSimpleOperatorType)ipOperator->getFunctionIndex();

    vector<DSSSliceID*> lvSliceID;
    for (int i = 0; i < nChild; i++)
    {
        lvSliceID.push_back(&(ipChildSliceID[i]));
    }
    
    vector<int> lvBreakbyUnits;
    lvBreakbyUnits.assign(lvAggUnits.begin(), lvAggUnits.end());
    vector<DSSSortDefinition*> lpvSortbyDefn;
    lpvSortbyDefn.assign(irpSortDefns.begin(), irpSortDefns.end());
    
    CalculationContext *lCalcContext = DSSCalculationHelper::getCalculationContext(NULL,
                                                                                   lFuncIndex,
                                                                                   iIsIntermediate? mpIntermediateCube:mpEventContext->mpCube,
                                                                                   &lvBreakbyUnits,
                                                                                   &lvSliceID,
                                                                                   mNullCheckingOption,
                                                                                   mpEventContext->mpSchema,
                                                                                   NULL,
                                                                                   &lpvSortbyDefn,
                                                                                   bIsOnIDColumn);
    MBase::DSSStrongPtr<CalculationContext> lCalcContextPtr(lCalcContext);
    
    hr =  DSSCalculationHelper::CalculateNewSlice(ipOperator, lCalcContext, opSliceID);
    if (lCalcContext->mpFuncObject)
    {
        //delete lCalcContext->mpFuncObject;
        MBase::DSSStrongPtr<DSSFunctionObject> lFunctionObjectPtr(lCalcContext->mpFuncObject);
    }
    CHECK_HR;
    
    return hr;
}

int DSSDimMetricEvaluator::subtotalAgg(int nChild,
				DSSSliceID *ipChildSliceID,
				DSSMetric *ipDimMetric,
				int iOffset, 
				DSSExpNodeOperator *ipOperator,
				vector<DSSSortDefinition *> & irpSortDefns,
				bool iIsIntermediate,
				DSSSliceID *opSliceID)
{
	if (!mbSubtotalContext)
		return  E_FAIL;
    
	// =================================================================
	// incomplete_dimty
	// After calculating the slice of child of the aggregation function,
	// first replace the dimty of the current function using the target level
	//    ReplaceDimty(ICDSSMetric *ipDimMetric, vector<long> *opOuterLevel)
	//    get_SliceLevel(long iChildSliceID, vector<long> *opInnerLevel)
	//
	// then resolve the dimty by applying the rules
	//    ResolveOuterLevel(vector<long> *ipInnerLevel, vector<long> *iopOuterLevel)
	// =================================================================
	// apply rule 1
//	MBase::Allocator<Int32> lAlloc(mpBuffer);
//	LongVector lOuterLevel(lAlloc);
    vector<Int32> lOuterLevel;
	int hr = ReplaceDimty(ipDimMetric, mpEventContext->mpCube->getTabularData(), &lOuterLevel);
	if(FAILED(hr)) return hr;
    
	//xhan 09/15/05 From now on, all metrics in AE has true levels. We will rely on a CDSSResolveLevel object to resolve
	//Subtotal levels. Original Rule 2 and Rule 3 in <<Nested Aggregtation>> are obsolete.
    
	vector<Int32> lInnerLevel;
	hr = get_SliceLevel(ipChildSliceID[0], &lInnerLevel);
	if(hr != S_OK) return hr;
	vector<Int32> lvLevel;
	for(Int32 iChild = 1; iChild < nChild; iChild++)
	{
		hr = get_SliceLevel(ipChildSliceID[iChild], &lvLevel);
		if(hr != S_OK) return hr;
        
		hr = CheckLevelCompatible(lvLevel, lInnerLevel);
		if(hr != S_OK) return hr;
        
		lvLevel.clear();
	}
    
    
	// apply rule 2, resolve to true aggregtion level
	//For example AggF1(AggF2(M) {Year, Store} ) {Month, Store} is resolved to {Year, Store}
	vector<int> lResultLevel;
	hr = mpLevelResolver->ResolveAggLevel(lInnerLevel,lOuterLevel,lResultLevel);
	CHECK_HR;
	
    vector<DSSSliceID*> lvpSliceID;
    for (int i = 0; i < nChild; i++)
    {
        lvpSliceID.push_back(&(ipChildSliceID[i]));
    }
    
	if(iOffset == 0)
	{
		//Rule 3 Resolve aggregation level with the template level,
		//For example, if aggregation level is {Year} and template level is {Market} then the most level is {}
		vector<int> lOuterMost;
		hr = ResolveOuterMostLevel(lResultLevel, lOuterMost);
		if(FAILED(hr)) return hr;
        /*
		hr = CDSSCalculationHelper::CalculateNewSlice(
                                                      mpSource,
                                                      mpFunctionServer,
                                                      ipOperator,
                                                      iIsIntermediate? mpIntermediateCube.p:mpTargetCube,
                                                      lnOuterMost,
                                                      lpOuterMost,
                                                      nChild,
                                                      ipChildSliceID,
                                                      mNullCheckingOption,
                                                      opSliceID,
                                                      ipSortDefns,
                                                      ipDimMetric,
                                                      mpReportInstance,
                                                      false, false, NULL, NULL, NULL, NULL,
                                                      mpResolveLevel);
		if(FAILED(hr)) return hr;*/
        
        CalculationContext *lCalcContext = DSSCalculationHelper::getCalculationContext(NULL,
                                                                                       (EnumSimpleOperatorType)ipOperator->getOperatorType(),
                                                                                       iIsIntermediate? mpIntermediateCube:mpEventContext->mpCube,
                                                                                       &lOuterMost,
                                                                                       &lvpSliceID,
                                                                                       mNullCheckingOption,
                                                                                       mpEventContext->mpSchema,
                                                                                       NULL,
                                                                                       &irpSortDefns);
        MBase::DSSStrongPtr<CalculationContext> lCalcContextPtr(lCalcContext);
        
        hr = DSSCalculationHelper::CalculateNewSlice(ipOperator, lCalcContext, opSliceID);
        if (lCalcContext->mpFuncObject)
        {
            //delete lCalcContext->mpFuncObject;
            MBase::DSSStrongPtr<DSSFunctionObject> lFunctionObjectPtr(lCalcContext->mpFuncObject);
        }
        CHECK_HR;
	}
	else
	{
		//xhan It is an aggregation that is nested inside another aggregation
		// calculate the output slice
        /*
		hr = CDSSCalculationHelper::CalculateNewSlice(
                                                      mpSource,
                                                      mpFunctionServer,
                                                      ipOperator,
                                                      iIsIntermediate? mpIntermediateCube.p:mpTargetCube,
                                                      lnResultLevel,
                                                      lpResultLevel,
                                                      nChild,
                                                      ipChildSliceID,
                                                      mNullCheckingOption,
                                                      opSliceID,
                                                      ipSortDefns,
                                                      ipDimMetric,
                                                      mpReportInstance,
                                                      false, false, NULL, NULL, NULL, NULL,
                                                      mpResolveLevel);
		if(FAILED(hr)) return hr;
        */
        CalculationContext *lCalcContext = DSSCalculationHelper::getCalculationContext(NULL,
                                                                                       (EnumSimpleOperatorType)ipOperator->getOperatorType(),
                                                                                       iIsIntermediate? mpIntermediateCube:mpEventContext->mpCube,
                                                                                       &lResultLevel,
                                                                                       &lvpSliceID,
                                                                                       mNullCheckingOption,
                                                                                       mpEventContext->mpSchema,
                                                                                       NULL,
                                                                                       &irpSortDefns);
        MBase::DSSStrongPtr<CalculationContext> lCalcContextPtr(lCalcContext);
        
        hr = DSSCalculationHelper::CalculateNewSlice(ipOperator, lCalcContext, opSliceID);
        if (lCalcContext->mpFuncObject)
        {
            //delete lCalcContext->mpFuncObject;
            MBase::DSSStrongPtr<DSSFunctionObject> lFunctionObjectPtr(lCalcContext->mpFuncObject);
        }
        CHECK_HR;
	}
    
	return S_OK;
}

int DSSDimMetricEvaluator::hGetMetricSliceID(GUID & irMetricID, DSSMetric *ipDimMetricGov, DSSSliceID *opSliceID)
{
	int hr = S_OK;
	
	GUID iDimMetricGovID = ipDimMetricGov->getID();
	
	int lMetricIndex = mpEventContext->mpCalculationPlan->getMetricIndexWithGoverner(irMetricID, iDimMetricGovID);
	if(lMetricIndex < 0)
	{
        //fali, 889852, allow get metricIndex regardless of Gov Metric when mbFilterEvaluation equals true. Used for DM in filter which is not view template.
        if (mbSubtotalContext || mbFilterEvaluation)
            lMetricIndex = mpEventContext->mpCalculationPlan->getMetricIndex(irMetricID);
		
		if (lMetricIndex < 0)
			return E_FAIL;
	}

	int iLevel = mpEventContext->mLevel;
	int iEvent = mpEventContext->mpEvent->mEventIndex;
	
	//check from the previous event. Its metric slice may not be set in the current event yet.
	bool lfCalculated = false;
	hr = mpEventContext->mpCalculationPlan->getMetricSliceID(iLevel, iEvent-1, lMetricIndex, &lfCalculated, &(opSliceID->mSliceID));
	CHECK_HR;
	
	// For non-smart metric, it should be in the previous event already
	if(!lfCalculated)
		return E_FAIL;
	
	if (mpEventContext->mpEvent->mEventType == DssEventTemplateSubtotal)
	{
		//if dimension awareness is false, need expand slice
//		hr = CDSSCalculationHelper::ExpandSlice(iMetricID, mpSource, mpTargetCube,
//												mpReportInstance, mpFunctionServer, lpObjectInfo,	mpBuffer, lTempSliceID,  &lViewID, opSliceID);
		CHECK_HR;
	}
	
    opSliceID->mpCube = mpEventContext->mpCube;
	
	return S_OK;
}

int DSSDimMetricEvaluator::ReplaceDimty(DSSMetric *ipDimMetric, DSSTabularData *ipTabularData, vector<int> *opLevel)
{
    if (!ipDimMetric) return E_FAIL;
    
    int hr = S_OK;
    vector<int> lpLevel;
    //tliao, 933283, 2014/08/08, get levels from serialized map
    bool lUseBaseTemplate4Subtotal = false;
    
    if (mpEventContext->mLevel > 0)
	{
		DSSDimty *lpDimty = ipDimMetric->getDimty();
        CHECK_PTR(lpDimty);
        
        int lnUnits = lpDimty->Count();
        for (int iUnit = 0; iUnit < lnUnits; iUnit++)
        {
            DSSDimtyUnit *lpDimtyUnit = lpDimty->Item(iUnit);
            CHECK_PTR(lpDimtyUnit);
            
            EnumDSSDimtyUnitType lUnitType = lpDimtyUnit->getUnitType();
            
			if (lUnitType == DssDimtyUnitTypeDimension || lUnitType == DssDimtyUnitTypeReportBaseLevel)
			{
				// these two dimty unit type should use the base template level to calculate subtotal
				// e.g. DM = count(category){~+}
				lUseBaseTemplate4Subtotal = true;
				break;
			}
		}// end of for
	}
    
    // qjiang 3/17/2014, for both Subtotal using metric expression and compound metric calculation on subtotal level, we use the subtotal context to cal dynamic dimty
    if (mbSubtotalContext)
    {
        lUseBaseTemplate4Subtotal = false;
    }
    
    map<GUID, vector<int> > *lpDynamicDimtyMap = NULL;
    
    if (lUseBaseTemplate4Subtotal)
    {
        lpDynamicDimtyMap = ipTabularData->getDynamicDimtyByLevel(0);
    }
    else
    {
        lpDynamicDimtyMap = ipTabularData->getDynamicDimtyByLevel(mpEventContext->mLevel);
    }
    
    bool lbFind = false;
    
    if (lpDynamicDimtyMap)
    {
        map<GUID,vector<int> >::iterator it = lpDynamicDimtyMap->find(ipDimMetric->getID());
        
        if(it != lpDynamicDimtyMap->end())
        {
            lpLevel = it->second;
            lbFind = true;
        }
    }

    //tliao, 933283, 2014/08/08, if you are using a build after 2014/08/08 and lbFind is false,
    //  there may be something wrong
    if (lbFind == false)
    {
        //tliao, 964108, 2014/09/10, need to distinguish the two kinds of dimty
        DSSExpNode *lpDimMetricRoot = ipDimMetric->getExpression()->Root();
        CHECK_PTR(lpDimMetricRoot);

        EnumDSSNodeDimty lDimtyType = lpDimMetricRoot->getDimtyType();
        DSSDimty *lpDimty = NULL;
        if (lDimtyType != DssNodeDimtyBreakBy)
        {
            lpDimty = ipDimMetric->getDimty();
        }
        else
        {
            lpDimty = lpDimMetricRoot->getDimty();
        }
        CHECK_PTR(lpDimty);
        
        int lnUnits = lpDimty->Count();
        for (int iUnit = 0; iUnit < lnUnits; iUnit++)
        {
            DSSDimtyUnit *lpDimtyUnit = lpDimty->Item(iUnit);
            CHECK_PTR(lpDimtyUnit);
            
            EnumDSSDimtyUnitType lUnitType = lpDimtyUnit->getUnitType();
            
            // replace based on the unit type
            if (lUnitType == DssDimtyUnitTypeAttribute)
            {
                DSS_ID lID = lpDimtyUnit->getTargetID();
                
                Int32 lPosition = 0;
                hr = ipTabularData->FindUnitI(lID, DssTemplateAttribute, lPosition);
                if(FAILED(hr)) return hr;
                
                if (hr == S_FALSE)
                {
                    hr = ipTabularData->FindUnitI(lID, DssTemplateConsolidation, lPosition);
                    if(FAILED(hr)) return hr;
                }
                
                if(hr == S_OK)
                    // find the unit, then add it into the level
                    lpLevel.push_back(lPosition);
                else
                    // not find the unit, just ignore it, set the hr from S_FALSE
                    // back to S_OK
                    hr = S_OK;
            }
            else if (lUnitType == DssDimtyUnitTypeReportBaseLevel && !mbSubtotalContext)
            {
                //for grid
                //tliao, 827178, 12/27/2013, runtime unit(NGB,CGB) is not on mpTemplate
                /*
                 if (mpEventContext->mpTemplate)
                 {
                 // used by derived metric xiaonanhan temporary fix 12/20/01
                 vector<Int32 *> lvpAttrInAxis;
                 vector<Int32> lvnAttrInAxis;
                 //                MBase::SimpleBuffer *lpBuffer = new MBase::SimpleBuffer();
                 //                MBase::StrongPtr<MBase::SimpleBuffer> lpBufferHolder(lpBuffer);
                 
                 hr = DSSSubtotalEventEvaluator::InitReportBaseLevel(mpEventContext->mpTemplate, mpEventContext->mpCube->getTabularData(),
                 &lvpAttrInAxis, &lvnAttrInAxis, NULL);
                 
                 
                 Int32 lnAxes = 0;
                 lnAxes = (lvnAttrInAxis).size();
                 for(Int32 iAxis = 0; iAxis < lnAxes; iAxis++) {
                 for(Int32 iAttr = 0; iAttr < (lvnAttrInAxis)[iAxis]; iAttr++)
                 lpLevel.push_back((lvpAttrInAxis)[iAxis][iAttr]);
                 }
                 }
                 //fali, 817016, if mpEventContext->mpTemplate is NULL, it's FG and all units are on row axis, GetReportLevelUnits is enough.
                 else*/
                {
                    int lnUnitCount = ipTabularData->Count();
                    vector<int> lvReportLevelUnits(lnUnitCount);
                    int lnReportLevelUnits = 0;
                    hr = ipTabularData->GetReportLevelUnits(lnUnitCount, &lvReportLevelUnits[0], &lnReportLevelUnits);
                    
                    for(int iAttr=0; iAttr<lnReportLevelUnits; iAttr++)
                        lpLevel.push_back((lvReportLevelUnits)[iAttr]);
                }
            }
            else
            {
                // used by subtotal report only
                if (!mbSubtotalContext)
                    return hr = E_FAIL;
                
                switch(lUnitType) {
                        // =====================================
                        // incomplete : other type of dimty unit
                        // =====================================
                    case DssDimtyUnitTypeDimension:
                    {
                        // get the dimension
                        
                        GUID lTargetID = lpDimtyUnit->getTargetID();
                        DSSObjectInfo* lpObjectInfo = mpEventContext->mpObjectContext->FindObjectH(lTargetID);
                        CHECK_PTR(lpObjectInfo);
                        
                        DSSTemplateDimension* lpDim = dynamic_cast<DSSTemplateDimension*>(lpObjectInfo);
                        if(!lpDim) return E_NOINTERFACE;
                        
                        // count how many attribute in this dimension
                        Int32 lCount = 0,  iIndex = 0 ;
                        lCount = lpDim->Count();
                        
                        // check if each attribute from this dimension is in the current tamplete
                        // if yes, added to the level
                        // else, just ignored
                        for(iIndex = 0; iIndex < lCount; iIndex++)
                        {
                            DSSTemplateAttribute* lpAttr;
                            lpAttr = lpDim->Item(iIndex);
                            CHECK_PTR(lpAttr);
                            
                            DSS_ID lID = lpAttr->getID();
                            
                            Int32 lPosition = 0;
                            hr = ipTabularData->FindUnitI(lID, DssTemplateAttribute, lPosition);
                            // find in the tabular data
                            if(hr == S_OK)
                            {
                                Int32 lnAxes = 0;
                                bool lFound = false;
                                lnAxes = (*mpnAttrInAxis).size();
                                for(Int32 iAxis = 0; iAxis < lnAxes; iAxis++)
                                {
                                    int lSize = (*mpnAttrInAxis)[iAxis];
                                    for(Int32 iAttr = 0; iAttr < lSize; iAttr++)
                                        if(lPosition == (*mppAttrInAxis)[iAxis][iAttr])
                                        {
                                            lpLevel.push_back(lPosition);
                                            lFound = true;
                                            break;
                                        }
                                    
                                    if(lFound)
                                        break;
                                }
                            }
                        }
                        
                        break;
                        
                    }
                    case DssDimtyUnitTypeReportLevel:
                    {
                        Int32 lnAxes = lpDimtyUnit->Count();
                        
                        if(lnAxes == 0)
                        {	// @
                            // copy the target level
                            /*
                             Int32 lnTargetAxes = 0, lRelPos = 0;
                             lnTargetAxes = (*mpnTargetAxisIndex).size();
                             for(Int32 i = 0; i < lnTargetAxes; i++)
                             {
                             hr = GetAttrByAxisAtReportLevel((*mpnTargetAxisIndex)[i], &lRelPos, &lpLevel);
                             if(FAILED(hr)) return hr;
                             }
                             */
                            vector<int> lTargetLevel = mpEventContext->mpCalculationPlan->getSubtotalLevel(mpEventContext->mLevel);
                            lpLevel.assign(lTargetLevel.begin(), lTargetLevel.end());
                        }
                        else
                        {
                            // @rcp +/- n
                            /*
                             Int32 lRelPos = 0;
                             hr = lpDimtyUnit->get_RelativePosition(&lRelPos);
                             if(FAILED(hr)) return hr;
                             
                             Int32 lAxisIndex = 0;
                             for(Int32 iAxis = 0; iAxis < lnAxes; iAxis++)
                             {
                             hr = lpDimtyUnit->Item(iAxis+1, &lAxisIndex);
                             if(FAILED(hr)) return hr;
                             
                             hr = GetAttrByAxisAtReportLevel(lAxisIndex, &lRelPos, &lpLevel);
                             if(FAILED(hr)) return hr;
                             
                             }*/
                        }
                        
                        break;
                    }
                    case DssDimtyUnitTypeReportBaseLevel:
                    {
                        // copy all the attributes in the base level
                        Int32 lnAxes = 0;
                        lnAxes = (*mpnAttrInAxis).size();
                        for(Int32 iAxis = 0; iAxis < lnAxes; iAxis++)
                        {
                            int lSize = (*mpnAttrInAxis)[iAxis];
                            for(Int32 iAttr = 0; iAttr < lSize; iAttr++)
                                lpLevel.push_back((*mppAttrInAxis)[iAxis][iAttr]);
                        }
                        
                        break;
                    }
                    default:
                        return E_FAIL;
                        
                } // end of switch(lUnitType)
            }
        }
        
    }
    
    Int32 lSize = lpLevel.size();
    if (lSize != 0)
	{
        //lwang, 02/05/2009, 348500,need to check for DE.
		//If the current Event is before DE event, we should replace the DE unit with CG unit
		Int32 j = 0;
		for (j = 0; j < lSize; j++)
        {
            DSSTabularUnit *lpTabularDataUnit;
			hr = ipTabularData->Item(lpLevel[j], &lpTabularDataUnit);
			CHECK_PTR(lpTabularDataUnit);
            
            EnumDSSTemplateUnitType lUnitType = lpTabularDataUnit->getUnitType();
			CHECK_HR;
            
            if (DssTemplateConsolidation == lUnitType)
            {
                // jxie, 2011-2-12, TQMS 457446
				// only do CG unit replacement when the event is DE event or Consolidation event on base template
                DSSTabularRefConsolidation *lpCS = dynamic_cast<DSSTabularRefConsolidation *>(lpTabularDataUnit);
                if (lpCS)
				{
                    bool lFlag = lpCS->IsDerivedElement();
                    
                    //04/04/03 xhan if report is subset report, skip the consolidation
					bool lfViewTemplate = true; // jimzhang, Apr/16/2013, "lfViewTemplate" is hard code, since hCheckViewTemplate() is not implemented. fali, 822182, set to true temporarily
					/*hr = hCheckViewTemplate(&lfViewTemplate);
					CHECK_HR;*/
                    
                    if (lFlag == true || !lfViewTemplate)
					{
						DSS_ID lGUID = GUID_NULL;
						hr = lpTabularDataUnit->getGUID(lGUID);
						CHECK_HR;
                        
						Int32 lTrueUnit = -1;
						hr = FindDerivedElementUnit(lGUID, lTrueUnit);
						CHECK_HR;
                        
						lpLevel[j] = lTrueUnit;
					}
                }
            }
        } // END: for (j = 0; j < lSize; j++)
        
        //need to order lpLevel
		std::sort(lpLevel.begin(), lpLevel.end());
        
		// remove the repeated attribute
		Int32 lPrev = 0;
		lPrev = lpLevel[0];
		opLevel->push_back(lpLevel[0]);
		for(Int32 i = 1; i < lSize; i++)
		{
			if(lPrev != lpLevel[i])
			{
				lPrev = lpLevel[i];
				opLevel->push_back(lPrev);
			}
		}
    } // END: if (lSize != 0)
    
    lpLevel.clear();
    
	return hr;
}

int DSSDimMetricEvaluator::FindDerivedElementUnit(DSS_ID &iGuid, Int32 &orUnit)
{
    HRESULT hr = S_OK;
    
    DSSTabularData *lpTabularData = mpEventContext->mpCube->getTabularData();
    
	// yma 12/19/08, 325966
	hr = lpTabularData->FindUnitI(iGuid, DssTemplateConsolidation, orUnit);
	if (hr != S_OK)
		return E_FAIL;
    
    //lwang, 02/01/2009, 345017
	DSSTabularUnit *lpTabularDataUnit = NULL;
	hr = lpTabularData->Item(orUnit, &lpTabularDataUnit);
	CHECK_PTR(lpTabularDataUnit);
    
    DSSTabularRefConsolidation *lpCS = dynamic_cast<DSSTabularRefConsolidation *>(lpTabularDataUnit);
    CHECK_PTR(lpCS);
    
    bool lfIsInherited = false; // jimzhang, Apr/16/2013, "lfIsInherited" is hard code, since there is no get_IsInherited() in DSSTabularRefConsolidation.
	/*hr = lpCS2->get_IsInherited(&lfIsInherited);
	CHECK_HR;*/
    
    if (false == lfIsInherited)
	{
		EnumOrderingType lOrderingRule = mpEventContext->mpCalculationPlan->getOrderingRule();
        
		Int32 lEventIndex = mpEventContext->mpCalculationPlan->getEventIndex(iGuid);
        int lThisEventIndex = mpEventContext->mpEvent->mEventIndex;
        
		if ((DssDefaultOrdering == lOrderingRule && mpEventContext->mLevel == 0 && lThisEventIndex < lEventIndex)
			|| (DssExtendedOrdering == lOrderingRule && lThisEventIndex < lEventIndex))
		{
			hr = lpTabularData -> FindUnitI(iGuid, DssTemplateCustomGroup, orUnit);
			if(hr != S_OK)
				return E_FAIL;
		}
	}
    
    return S_OK;
}

int DSSDimMetricEvaluator::setSubtotalContext(GUID &iMetricID,
                                              vector<Int32 *> *ippAttrInAxis,
                                              vector<Int32> *ipnAttrInAxis,
                                              vector<Int32 > *ipnAxisIndex)
{
    mMeMetricID = iMetricID;
    mppAttrInAxis	= ippAttrInAxis;
	mpnAttrInAxis	= ipnAttrInAxis;
	mpnAxisIndex	= ipnAxisIndex;
    mbSubtotalContext = true;
    
    return S_OK;
}

int DSSDimMetricEvaluator::get_SliceLevel(DSSSliceID iSliceID, vector<int> *opLevel)
{
	// Go to cube to find out the slice by the input iSliceID
	// Find out the indices of the slice
	// Find out the list of units of the indices

    
	DSSCubeDataSlice* lpSlice;
	int hr = iSliceID.GetSlice(&lpSlice);
	CHECK_HR_PTR(lpSlice);
    
	DSSTabularIndex* lpXTabIndices = lpSlice->getIndices();
	CHECK_PTR(lpXTabIndices);
    
	Int32 lUnits = 0,  *lpUnits = 0 ;
	lUnits = lpXTabIndices->getNumUnits();
    
	lpUnits = new (mpBuffer) Int32[lUnits];
	CHECK_NEWED_PTR(lpUnits);
    
	hr = lpXTabIndices->getUnits(lUnits, lpUnits);
	if(FAILED(hr)) return hr;
    
	for(Int32 i = 0; i < lUnits; i++)
		opLevel->push_back(lpUnits[i]);
    
	return hr;
}

int DSSDimMetricEvaluator::CheckLevelCompatible(vector<int> &iLevel, vector<int> &ioResultLevel)
{
	HRESULT hr = S_OK;
    
	//If iLevel is an upper level of ipResultLevel, then return S_OK
	Int32 lnLevel = iLevel.size();
	
	bool lbUpper = false;
	hr = mpLevelResolver->IsUpperLevel(iLevel, ioResultLevel, lbUpper);
	CHECK_HR;
    
	if (lbUpper)
		return S_OK;
    
	//If ipResultLevel is an upper level of iLevel, replace ipResultLevel's contents with iLevel's content
	//otherwise, level incompatible, return E_FAIL
	hr = mpLevelResolver->IsUpperLevel(ioResultLevel, iLevel, lbUpper);
	CHECK_HR;
    
	if (!lbUpper)
		return E_FAIL;
    
	ioResultLevel.clear();
	for (Int32 i = 0; i < lnLevel; i ++)
	{
		ioResultLevel.push_back(iLevel[i]);
	}
    
    
	return S_OK;;
}

int DSSDimMetricEvaluator::ResolveOuterMostLevel(vector<int> &iOrgLevel, vector<int> &oResultLevel)
{
	HRESULT hr = S_OK;
    
	//Xhan
	// collect target level from mpnTargetByAxis, it looks stupid, don't know if we remove all these junky stuff later.
    /*
	MBase::Allocator<Int32> lAlloc(mpBuffer);
	vector<Int32, MBase::Allocator<Int32> > lvTargetLevel(lAlloc);
	
	Int32 lnAxes = (*mpnTargetAxisIndex).size();
	for(Int32 iAxis = 0; iAxis < lnAxes; iAxis++)
	{
		Int32 lSize = (*mpnTargetByAxis)[iAxis];
		for(Int32 iAttr = 0; iAttr < lSize; iAttr++)
		{
			lvTargetLevel.push_back((*mppTargetByAxis)[iAxis][iAttr]);
		}
	}
    */
    
    
	vector<int> lTrueAggLevel;
    vector<int> lTargetLevel = mpEventContext->mpCalculationPlan->getSubtotalLevel(mpEventContext->mLevel);
//	Int32 lvTargetLevelSize = lvTargetLevel.size();
	hr = mpLevelResolver->ResolveAggLevel(iOrgLevel, lTargetLevel,lTrueAggLevel);
	CHECK_HR;
    
	//If true aggLevel is year, and target level is month, we need to downward adjust level to month
	//We trust apply aggregation function logic deal with aggregation level correctly
	hr = mpLevelResolver->ResolveDownwardLevel(lTrueAggLevel, lTargetLevel,oResultLevel);
    
	return hr;
}

int DSSDimMetricEvaluator::ReplaceSortBy(DSSMetric *ipDimMetric, DSSTabularData *ipTabularData, DFCSimpleExpr*& opSortLevel)
{
    vector<map<GUID, DFCSimpleExpr* > >* lvpDBB = ipTabularData->getDynamicBreakBy();
    DFCSimpleExpr* lpSortLevel = NULL;
    if (lvpDBB && !lvpDBB->empty())
    {
        map<GUID, DFCSimpleExpr* >::iterator it = ((*lvpDBB)[0]).find(ipDimMetric->getID());
        
        if(it != ((*lvpDBB)[0]).end())
        {
            lpSortLevel = it->second;
        }
    }
    opSortLevel = lpSortLevel;
    
    return S_OK;
}
