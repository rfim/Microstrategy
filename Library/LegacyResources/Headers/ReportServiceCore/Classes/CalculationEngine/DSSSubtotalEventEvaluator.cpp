/*
 *  DSSSubtotalEventEvaluator.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 1/12/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#include "DSSSubtotalEventEvaluator.h"
#import "CalculationEngine/DSSCalculationPlan.h"
#import "DSSDataModelXTabImpl.h"
#import "DSSTabularMetrics.h"
#import "DSSTemplate.h"
#import "DSSTemplateDimension.h"
#import "DSSTemplateConsolidation.h"
#import "DSSTemplateCustomGroup.h"
#import "DSSViewDataSet.h"
#import "DSSTabularData.h"

#import "DSSFunctionServer.h"
#import "DSSStrongPtr.h"
#import "DSSFunctionObject.h"
#import "DSSDimMetricEvaluator.h"
#import "DSSCubeDataSlice.h"
#include <algorithm>
class MetricSubtotal;

int InitTargetLevel(int iLevel, DSSCalculationPlan *ipCalculationPlan, vector<int> &irvUnits)
{
	const vector<int> lSubtotalLevel = ipCalculationPlan->getSubtotalLevel(iLevel);
	irvUnits.assign(lSubtotalLevel.begin(), lSubtotalLevel.end());
	return S_OK;
}

EnumDSSMetricType FindMetricSubtotal(IDSSTemplateModel *ipDataModel, int iSubtotalID, GUID iMetricID, DSSViewDataSet* ipViewDataset)
{
    EnumDSSMetricType lTempalteSubtotal = DssMetricReserved;
    if (ipDataModel)
    {
        lTempalteSubtotal = ipDataModel->getSubtotalType(iSubtotalID - 1, iMetricID);
        //TODO: check DssMetricSubtotalAggregation or DssMetricSubtotal?
        if (/*lTempalteSubtotal == DssMetricSubtotal || */lTempalteSubtotal == DssMetricSubtotalAggregation
            || lTempalteSubtotal == DssMetricSubtotalSum)	//475097, somehow iSubtotalID is already resolved as SubtotalSum
        {
            DSSTabularMetrics *lpMetrics = NULL;
            ipDataModel->getMetricsUnit(&lpMetrics);
            if (lpMetrics)
            {
                int index = lpMetrics->FindMetric(iMetricID);
                if (index >= 0)
                {
                    return lpMetrics->getTotalImplementation(index);
                }
            }
        }
        //tliao, 1/14/2014, 826955, we need to check if this template subtotal is available for current metric
        else if (lTempalteSubtotal == DssMetricSubtotal || ((int)lTempalteSubtotal<-10 && (int)lTempalteSubtotal>-21) )
        {
            
            DSSTabularMetrics *lpMetrics = NULL;
            ipDataModel->getMetricsUnit(&lpMetrics);
            if (lpMetrics)
            {
                int lSystemSubtotal;
                vector<GUID> lUserDefinedSubtotals;
                int hr = lpMetrics->getAvailableSubtotals(iMetricID, lSystemSubtotal, lUserDefinedSubtotals);
                if(hr != S_OK)
                {
                    return lTempalteSubtotal;
                }
                if (lTempalteSubtotal == DssMetricSubtotal)
                {
                    GUID lMetricGUID = static_cast<DSSRWGridModel*>(ipDataModel)->getSubtotalGUID(iSubtotalID-1);
                    for (int i = 0; i < lUserDefinedSubtotals.size(); i++)
                    {
                        if (lMetricGUID == lUserDefinedSubtotals[i])
                        {
                            return lTempalteSubtotal;
                        }
                    }
                }
                else
                {
                    int lPos = (int)lTempalteSubtotal + 20;
                    if (((lSystemSubtotal>>lPos)& 0x01) == 1)
                    {
                        return lTempalteSubtotal;
                    }
                }
                return DssMetricReserved;
            }
            

        }
    }
    else if (ipViewDataset)
    {
        DSSTabularMetrics *lpMetrics = ipViewDataset->getTabularData()->getTabularMetircs();
        if (lpMetrics)
        {
            int index = lpMetrics->FindMetric(iMetricID);
            if (index >= 0)
            {
                return lpMetrics->getTotalImplementation(index);
            }
        }
    }
	
	return lTempalteSubtotal;
}

int ConvertToFunctionType(EnumDSSMetricType iMetricType, EnumSimpleOperatorType &orFunctionIndex);

DSSSubtotalEventEvaluator::DSSSubtotalEventEvaluator()
{
}

DSSSubtotalEventEvaluator::~DSSSubtotalEventEvaluator()
{
    //zhyang, 938276, memory leak
    for(int i = 0; i < mvpAttrInAxis.size(); ++i)
        delete mvpAttrInAxis[i];
}

int DSSSubtotalEventEvaluator::ApplyEvent(EventContext *ipCntxt)
{
	int hr = S_OK;
	CHECK_PTR(ipCntxt);
    if(!ipCntxt->mpTabularData)
    {
        IDSSTemplateModel* lpDataModel = ipCntxt->mpDataModel;
        CHECK_PTR(lpDataModel);
        
        DSSDataModelXTabImpl* lpXTabModel = dynamic_cast<DSSDataModelXTabImpl*>(lpDataModel);
        CHECK_PTR(lpXTabModel);
        
        DSSTabularData* lpTabularData = lpXTabModel->getTabularData();
        CHECK_PTR(lpTabularData);
        
        ipCntxt->mpTabularData = lpTabularData;
        
        ipCntxt->mpCube->setTabularData(lpTabularData);
        
        DSSTabularIndexPool* lpIndexPool = NULL;
        hr = ipCntxt->mpCube->getIndexPool(&lpIndexPool);
        CHECK_HR_PTR(lpIndexPool);
        
        hr = lpIndexPool->Init(lpXTabModel);
        CHECK_HR;
        
        hr = lpTabularData->setIndexPool(lpIndexPool);
        CHECK_HR;
    }
    
	DSSCalculationPlan *lpCalculationPlan = ipCntxt->mpCalculationPlan;
	int iLevel = ipCntxt->mLevel;
	int iEvent = ipCntxt->mpEvent->mEventIndex;
	
	int lTemplateSubtotal = lpCalculationPlan->getTemplateSubtotal(iLevel);
    
    // initialize report base level
	// report base level is the template level
    //zhyang, 938276, memory leak
    for(int i = 0; i < mvpAttrInAxis.size(); ++i)
        delete mvpAttrInAxis[i];
	mvpAttrInAxis.clear();
    mvnAttrInAxis.clear();
    mvnAxisIndex.clear();
    /*
    ////tliao, 827178, 12/27/2013, runtime unit(NGB,CGB) is not on mpTemplate
    if (ipCntxt->mpTemplate)
    {
        hr = InitReportBaseLevel(ipCntxt->mpTemplate, ipCntxt->mpTabularData, &mvpAttrInAxis, &mvnAttrInAxis, &mvnAxisIndex);
        if(hr != S_OK) return hr;
    }
    //fali, 817016, if mpEventContext->mpTemplate is NULL, it's FG and all units are on row axis, GetReportLevelUnits is enough.
    else*/
    {
        mvnAxisIndex.push_back(0);
        int lnUnitCount = ipCntxt->mpTabularData->Count();
        mvnAttrInAxis.push_back(lnUnitCount);
        
        
        int* lpReportLevelUnits = new int[lnUnitCount];
        int lnReportLevelUnits = 0;
        hr = ipCntxt->mpTabularData->GetReportLevelUnits(lnUnitCount, lpReportLevelUnits, &lnReportLevelUnits);
        CHECK_HR;
        
        mvpAttrInAxis.push_back(lpReportLevelUnits);
    }
    
	// initialize the target level
	vector<int> lvTargetLevel;
	hr = InitTargetLevel(iLevel, lpCalculationPlan, lvTargetLevel);
	CHECK_HR;
	
	int iMetric = 0;
	int lNumMetric= lpCalculationPlan->getNumMetrics();
	
	// walk through all metrics
	for(iMetric = 0; iMetric < lNumMetric; iMetric++)
	{
		bool lfIsCalculated = 0;
		int lSliceID = 0;
		
		hr = lpCalculationPlan->getMetricSliceID(iLevel,
												 iEvent,
												 iMetric,
												 &lfIsCalculated,
												 &lSliceID);
		CHECK_HR;
		
		if(lfIsCalculated)
			continue;
		
		// get the previous slice id
		hr = lpCalculationPlan->getMetricSliceID(iLevel,
												 iEvent-1,
												 iMetric,
												 &lfIsCalculated,
												 &lSliceID);
		CHECK_HR;
		
		bool lbResultCalculated = false;
		int lResultSliceID = -1;
		if(lTemplateSubtotal == ipCntxt->mpEvent->mEventTag)
		{
			// the event matches the template subtotal of the current level
			// calculate the subtotal of the current metric
			//for the component of compound metric, use the container's subtotal type
			GUID lMetricID2 = lpCalculationPlan->getMetricID2(iMetric);
			CHECK_HR;
            
            bool lbUseContainer = true;
			
            //find it from system subtotal first
			EnumDSSMetricType lTotalImpl = FindMetricSubtotal(ipCntxt->mpDataModel, lTemplateSubtotal, lMetricID2, ipCntxt->mpViewDataset);
			
            if (lTotalImpl == DssMetricReserved && ipCntxt->mpTemplate)    //Then find it from custom subtotal
                lTotalImpl =ipCntxt->mpTemplate->getMetricType(lTemplateSubtotal, lMetricID2);
            
            GUID lMetricID = lpCalculationPlan->getMetricID(iMetric);
            if (lMetricID != lMetricID2 && lTotalImpl == DssMetricSubtotalAggregation) //if compound metric's subtotal implementation is default
            {
                //use component metric's setting
                lTotalImpl = FindMetricSubtotal(ipCntxt->mpDataModel, lTemplateSubtotal, lMetricID, ipCntxt->mpViewDataset);
                lbUseContainer = false;
                if (lTotalImpl == DssMetricReserved && ipCntxt->mpTemplate)    //Then find it from custom subtotal
                    lTotalImpl =ipCntxt->mpTemplate->getMetricType(lTemplateSubtotal, lMetricID);
            }
            
			if(lTotalImpl != DssMetricReserved)
			{
                // lweng, 2012-4-1, TQMS 574333, check if it is total subtotal
				if (lTotalImpl == DssMetricSubtotalSum)
				{
					DSS_ID lOriginalMetricID = lpCalculationPlan->getMetricID(iMetric);
					Int32 lGeneratedSliceID = -1;
                    
					hr = ipCntxt->mpCube->get_GeneratedMetricID(lOriginalMetricID, iLevel, &lGeneratedSliceID);
					CHECK_HR;
                    
					if (lGeneratedSliceID != -1)
					{
						hr = lpCalculationPlan->setMetricSliceID(iLevel,
                                                                  iEvent,
                                                                  iMetric,
                                                                  true,
                                                                  lGeneratedSliceID);
						if(hr != S_OK) return hr;
                        
						continue;
					}
				}
                
				// find the metric subtotal
				// calculate subtotal for the metric
				DSSSliceID lSliceID;
				hr = hEvaluateSubtotal(ipCntxt,
									   lTotalImpl,
									   iMetric,
									   lvTargetLevel,
                                       lbUseContainer,
									   &lSliceID);
				
				// succeed, put the slice id into the plan
				if(hr == S_OK)
				{
					lbResultCalculated = true;
					lResultSliceID = lSliceID.mSliceID;
				}
                if(hr == E_OUT_OF_MEMORY)
                    return hr;
			}
		}//end of if(lTemplateSubtotal == mEventTag)
		else
		{
			// the event is not the template subtotal of the level
			// just copy over all the slices from previous event
			lbResultCalculated = lfIsCalculated;
			lResultSliceID = lSliceID;
		}
        
		hr = lpCalculationPlan->setMetricSliceID(iLevel,
												 iEvent,
												 iMetric,
												 lbResultCalculated,
												 lResultSliceID);
		CHECK_HR;
	}
	return hr;
	
}

int DSSSubtotalEventEvaluator::hEvaluateSubtotal(EventContext *ipCntxt,
											  EnumDSSMetricType iTotalImpl,
											  int iMetric,
											  vector<int> &irvUnits,
                                              bool iUseContainer,
											  DSSSliceID *opSliceID)
{
	int hr = S_OK;
	
	EnumNullCheckingOption lNullCheckingOption = ipCntxt->mpCalculationPlan->getNullCheckingOption(iMetric);
	
	int iLevel = ipCntxt->mLevel;
	int iEvent = ipCntxt->mpEvent->mEventIndex;
	
	
	bool lfIsCalculated = false;
	int lSliceID = 0;
	hr = ipCntxt->mpCalculationPlan->getMetricSliceID(iLevel,
											 iEvent - 1,
											 iMetric,
											 &lfIsCalculated,
											 &lSliceID);
	CHECK_HR;
	
	if(!lfIsCalculated)
		return E_FAIL;
	
    if(!ipCntxt->mpViewDataset)//931 i-server, for backward compatiblity
    {
        //TODO:
        if (!irvUnits.empty())
            return E_FAIL;		//only support grand total
        
        if(iTotalImpl != DssMetricSubtotal && iTotalImpl != DssMetricSubtotalAggregation)
        {
            if(iTotalImpl == DssMetricNormal)
                return E_FAIL;
            
            EnumSimpleOperatorType lFunctionID;
            hr = ConvertToFunctionType(iTotalImpl, lFunctionID);
            CHECK_HR;
            
            DSSSliceID lFinalInputSlice(ipCntxt->mpCube, lSliceID);
            vector<DSSSliceID*> lvSliceID;
            lvSliceID.push_back(&lFinalInputSlice);
            
            vector<int> lvBreakbyUnits;
            lvBreakbyUnits.assign(irvUnits.begin(), irvUnits.end());
            
            CalculationContext *lCalcContext = DSSCalculationHelper::getCalculationContext(NULL,
                                                                                           lFunctionID,
                                                                                           ipCntxt->mpCube,
                                                                                           &lvBreakbyUnits,
                                                                                           &lvSliceID,
                                                                                           lNullCheckingOption,
                                                                                           NULL);
             MBase::DSSStrongPtr<CalculationContext> lCalcContextPtr(lCalcContext);
            
            hr = DSSCalculationHelper::CalculateNewSlice(lCalcContext, opSliceID);
            CHECK_HR;
        }
        else
        {
            //Total subtoal, not implemented
            //aggregate subtoal, not imeplemented
            return E_NOTIMPL;
        }
		
        return S_OK;
    }
    //fali, 816976, for those datasets which subtotalDimentionalityAware=false, expand input slice to mainIndex level.
    bool lSubtotalDimensionalityAware = ipCntxt->mpViewDataset->IsSubtotalDimensionalityAware();
    
    if(!lSubtotalDimensionalityAware)
    {
        DSSSliceID *opSliceID = new DSSSliceID();
        hr = ExpandSlice(ipCntxt->mpCube,lSliceID, opSliceID);
        CHECK_HR;
        lSliceID = opSliceID->mSliceID;
        ipCntxt->mpCube = opSliceID->mpCube;
    }
    
    //get metric's dynamic aggregation function
    EnumDSSMetricType lDAImpl = DssMetricReserved;
    GUID lMetricID;
    //tliao, 1/24/2014, 833151, use correct metric ID
    if (iUseContainer)
    {
        lMetricID = ipCntxt->mpCalculationPlan->getMetricID2(iMetric);
    }
    else
    {
        lMetricID = ipCntxt->mpCalculationPlan->getMetricID(iMetric);
    }
    DSSTabularMetrics* lpMetrics = ipCntxt->mpTabularData->getTabularMetircs();
    int pos = lpMetrics->FindMetric(lMetricID);
    if (pos != -1)
    {
        lDAImpl = lpMetrics->getDynamicAggFunc(pos);
    }
    
    //If metric's Total Subtotal Function is a user-defined subtotal, we will use DSSDimMetricEvaluator to do calculation
    //tliao, 2/20/2014, 848720
    if (iTotalImpl == DssMetricSubtotal)// || (iTotalImpl == DssMetricSubtotalAggregation && lDAImpl == DssMetricSubtotal))
    {
        DSSDimMetricEvaluator lDimMetricEvaluator(ipCntxt, ipCntxt->mpCalculationPlan->getNullCheckingOption(iMetric));
        GUID iMetricID = ipCntxt->mpCalculationPlan->getMetricID(iMetric);
        lDimMetricEvaluator.setSubtotalContext(iMetricID,
                                               &mvpAttrInAxis,
                                               &mvnAttrInAxis,
                                               &mvnAxisIndex);
        
        int lTemplateSubtotal = ipCntxt->mpCalculationPlan->getTemplateSubtotal(iLevel);
        DSSRWGridModel *lpGridModel = dynamic_cast<DSSRWGridModel*>(ipCntxt->mpDataModel);
        GUID lMetricGUID = lpGridModel->getSubtotalGUID(lTemplateSubtotal-1);
        //if lMetricGUID is not GUID_NULL, that means the template subtotal is a user-defined subtotal
        if (lMetricGUID ==  GUID_NULL)
        {
            //The template subtotal should be "Total" and Total subtotal function of the metric is a user-defined subtotal
            EnumDSSMetricType lTempalteSubtotal = lpGridModel->getSubtotalType(lTemplateSubtotal - 1);
            if (lTempalteSubtotal == DssMetricSubtotalSum)
            {
                //need to get Total subtotal function GUID of the metric
                if (pos != -1)
                {
                    //if (iTotalImpl == DssMetricSubtotal)
                        lMetricGUID = lpMetrics->getTotalImplID(pos);
                    //else
                        //lMetricGUID = lpMetrics->getDynamicAggID(pos);
                }
                else
                {
                    //this should not happen..
                    return E_FAIL;
                }
            }
            if (lMetricGUID == GUID_NULL)
            {
                //this should not happen..
                return E_FAIL;
            }
        }
        DSSSliceID lResult;
        hr = lDimMetricEvaluator.CalculateDimMetric(lMetricGUID, &lResult);
        lfIsCalculated = (hr == S_OK && lResult.mSliceID >= 0);
        if (!lfIsCalculated)
            lResult.mSliceID = -1;
        
        *opSliceID = lResult;
    }
    else
    {
        if(iTotalImpl == DssMetricNormal)
			return E_FAIL;
        
        EnumSimpleOperatorType lFunctionIndex = DssFuncUnknown;
        bool lSubtotalNonAggMetricAware = false;
        
        DSSSliceID lFinalInputSlice(ipCntxt->mpCube, lSliceID);
        
        if(iTotalImpl == DssMetricSubtotalAggregation) //Total subtotal function is "Default"
        {
            //tliao, 2/20/2014, 848904
            //if (lDAImpl == DssMetricSubtotalAggregation || lDAImpl == DssMetricSubtotal) //metric's dynamic aggregation function is "Default" or user-defined
            //tliao, 4/21/2014, 870511,874486, if template subtotal is Sum and metric's dynamic aggregation function is not "Default" nor user-defined
            //                          use the dynamic aggregation function
            int lTemplateSubtotal = ipCntxt->mpCalculationPlan->getTemplateSubtotal(iLevel);
            DSSRWGridModel *lpGridModel = dynamic_cast<DSSRWGridModel*>(ipCntxt->mpDataModel);
            //tliao, 4/25/2014, 883856, for field group model, there is only SUM subtotal
            EnumDSSMetricType lTempalteSubtotal = lpGridModel ? lpGridModel->getSubtotalType(lTemplateSubtotal - 1) : DssMetricSubtotalSum;
            if (lTempalteSubtotal == DssMetricSubtotalSum && lDAImpl != DssMetricSubtotalAggregation && lDAImpl != DssMetricSubtotal && lDAImpl != DssMetricSubtotalSum)
            {
                hr = ConvertToFunctionType(lDAImpl, lFunctionIndex);
                CHECK_HR;
            }
            else
            {
                lSubtotalNonAggMetricAware = true;
                DSSFunctionServer *lpFunctionServer = new DSSFunctionServer();
                MBase::DSSStrongPtr<DSSFunctionServer> SpFunctionServer(lpFunctionServer);
                if (lTempalteSubtotal == DssMetricSubtotalSum && lDAImpl == DssMetricSubtotalSum)
                {
                    lFunctionIndex = DssFuncSum;
                }
                else
                {
                    VARIANT_BOOL lfOrderSignificant = VARIANT_FALSE;
                    DSSExpNode *lpNode = NULL;
                    Int32 lFuncIndex = DssFuncUnknown;
                    METRIC_ID lMetric_ID;
                    lMetric_ID.mMetricID = lDAImpl == DssMetricSubtotal ? lpMetrics->getDynamicAggID(pos) : lMetricID;
                    DSSFunctionObject *lpFunctionObject = NULL;
                    
                    // the ChooseAggregationFunction returns the expression itself, not the expr for the agg function
                    // Don't use it
                    hr = DSSCalculationHelper::ChooseAggregationFunction(ipCntxt->mpObjectContext,
                                                                         lpFunctionServer,
                                                                         lMetric_ID,
                                                                         &lpFunctionObject,
                                                                         &lFuncIndex,
                                                                         &lfOrderSignificant,
                                                                         &lpNode,
                                                                         lDAImpl);
                    CHECK_HR;
                    
                    if (lpFunctionObject == NULL)
                    {
                        return E_FAIL;
                    }
                    
                    MBase::DSSStrongPtr<DSSFunctionObject> lFunctionObjectPtr(lpFunctionObject);
                    
                    lFunctionIndex = (EnumSimpleOperatorType)lFuncIndex;
                }
                if (lSubtotalNonAggMetricAware && lSubtotalDimensionalityAware)
                {
                    vector<Int32> lvTempLevels;
                    vector<DSSSortDefinition*> lpTempSortDefns;
                    DSSLevelResolver *lpLevelResolver = new DSSLevelResolver(ipCntxt->mpCube, ipCntxt->mpSchema);
                    lpLevelResolver->Init();
                    METRIC_ID lMETRIC_ID;
                    lMETRIC_ID.mDatasetID = GUID_NULL;
                    lMETRIC_ID.mMetricID = lMetricID;
                    
                    hr = lpLevelResolver->ResolveInterimAggLevel(lMETRIC_ID, lSliceID, irvUnits, lvTempLevels, &lpTempSortDefns);
                    CHECK_HR;
                    
                    if (lpTempSortDefns.size() != 0)
                    {
                        DSSFunctionObject *lpFirstFunc = lpFunctionServer->getFunctionByIndex(DssFuncFirst);
                        MBase::DSSStrongPtr<DSSFunctionObject> lFirstFuncPtr(lpFirstFunc);
                        
                        vector<DSSSliceID*> lvSliceID;
                        DSSSliceID lDSSSliceID(ipCntxt->mpCube, lSliceID);
                        lvSliceID.push_back(&lDSSSliceID);
                        CalculationContext *lCalcContext = DSSCalculationHelper::getCalculationContext(lpFirstFunc,
                                                                                                       DssFuncFirst,
                                                                                                       ipCntxt->mpCube,
                                                                                                       &lvTempLevels,
                                                                                                       &lvSliceID,
                                                                                                       lNullCheckingOption,
                                                                                                       ipCntxt->mpSchema,
                                                                                                       NULL,
                                                                                                       &lpTempSortDefns);
                        MBase::DSSStrongPtr<CalculationContext> lCalcContextPtr(lCalcContext);
                        
                        hr = DSSCalculationHelper::CalculateNewSlice(lCalcContext, &lFinalInputSlice);
                        CHECK_HR;
                    }
                }
            }
            /*else    //dynamic aggregation function is not "Default" nor user-defined subtotal
            {
                hr = ConvertToFunctionType(lDAImpl, lFunctionIndex);
                CHECK_HR;
            }*/
        }
        else //Total subtotal function is not "Default" nor user-defined subtotal
        {
            hr = ConvertToFunctionType(iTotalImpl, lFunctionIndex);
            CHECK_HR;
        }

        vector<DSSSliceID*> lvSliceID;
        lvSliceID.push_back(&lFinalInputSlice);
        
        vector<int> lvBreakbyUnits;
        lvBreakbyUnits.assign(irvUnits.begin(), irvUnits.end());
        
        CalculationContext *lCalcContext = DSSCalculationHelper::getCalculationContext(NULL,
                                                                                       lFunctionIndex,
                                                                                       ipCntxt->mpCube,
                                                                                       &lvBreakbyUnits,
                                                                                       &lvSliceID,
                                                                                       lNullCheckingOption,
                                                                                       ipCntxt->mpSchema);
        MBase::DSSStrongPtr<CalculationContext> lCalcContextPtr(lCalcContext);
        
		hr = DSSCalculationHelper::CalculateNewSlice(lCalcContext, opSliceID);
        if (lCalcContext->mpFuncObject)
        {
            //delete lCalcContext->mpFuncObject;
            MBase::DSSStrongPtr<DSSFunctionObject> lFunctionObjectPtr(lCalcContext->mpFuncObject);
        }
		CHECK_HR;

    }
		
	return S_OK;
}

int ConvertToFunctionType(EnumDSSMetricType iMetricType, EnumSimpleOperatorType &orFunctionIndex)
{
	switch (iMetricType) {
		case DssMetricSubtotalSum:
		{
			orFunctionIndex = DssFuncSum;
		}
			break;
		case DssMetricSubtotalCount:
		{
			orFunctionIndex = DssFuncCount;
		}
			break;
		case DssMetricSubtotalAverage:
		{
			orFunctionIndex = DssFuncAvg;
		}
			break;
		case DssMetricSubtotalMin:
		{
			orFunctionIndex = DssFuncMin;
		}
			break;
		case DssMetricSubtotalMax:
		{
			orFunctionIndex = DssFuncMax;
		}
            break;
        case DssMetricSubtotalStdDev:
        {
            orFunctionIndex = DssFuncStdev;
        }
			break;
        case DssMetricSubtotalVariance:
        {
            orFunctionIndex = DssFuncVar;
        }
            break;
		case DssMetricSubtotalGeometricMean:
        {
            orFunctionIndex = DssFuncGeomean;
        }
            break;
            
		case DssMetricSubtotalProduct:
        {
            orFunctionIndex = DssFuncProduct;
        }
            break;
            
		case DssMetricSubtotalMedian:
        {
            orFunctionIndex = DssFuncMedian;
        }
            break;
            
		case DssMetricSubtotalMode:
        {
            orFunctionIndex = DssFuncMode;
        }
            break;
		
		case DssMetricSubtotalAggregation:
		default:
			return S_FALSE;
	}
	return S_OK;
}

int DSSSubtotalEventEvaluator::InitReportBaseLevel(DSSTemplate *ipTemplate,
                                                   DSSTabularData *ipTabularData,
                                                   //MBase::Buffer *ipBuffer,
                                                   std::vector<Int32 *>  *oppvAttrInAxis,
                                                   std::vector<Int32> *opvAttrInAxis,
                                                   std::vector<Int32> *opAxisIndex)

{
    int hr = S_OK;
    
	// mvpAttrInAxis is going to hold the position of each attribute in the
	// tabular data by each axis.
	// should be as follows:
	//   axis 0: attribute01, attribute02, attribute03 ...
	//   axis 1: attribute11, attribute12, attribute13 ...
	//    .
	//    .
	//   axis N: attributeN1, attributeN2, attributeN3 ...
    
	// find the attributes from each axis
	for(Int32 iAxis = 1; iAxis <= 2; iAxis++)
	{
		Int32 *lpAttrList = 0,  lnAttr = 0 ,  lnUnit = 0 ;
		DSSAxis *lpAxis = ipTemplate -> getAxis(iAxis);
		CHECK_PTR(lpAxis);
        
		lnUnit = lpAxis -> Count();
        
		// lnUnit does not taken into consideration of Dimension
		// Calculate number of attributes in each axis
		lnAttr = 0;
		Int32 iUnit = 0;
		for(iUnit = 0; iUnit < lnUnit; iUnit ++)
		{
			DSSTemplateUnit *lpTemplateUnit = lpAxis -> Item(iUnit);
			CHECK_PTR(lpTemplateUnit);
            
			DSSTemplateUnit_Type lUnitType = lpTemplateUnit -> getType();
            
			switch (lUnitType)
			{
                case DssTemplateAttribute:
                case DssTemplateConsolidation:
                case DssTemplateCustomGroup:
                    lnAttr++;
                    break;
                    
                    // Incomplete_Consolidation or Metrics
                    
                case DssTemplateMetrics:
					break;
                    
                case DssTemplateDimension:
				{
					DSSTemplateDimension *lpTemplateDimension = dynamic_cast<DSSTemplateDimension*>(lpTemplateUnit);
					if(!lpTemplateUnit) return hr =  E_NOINTERFACE;
                    
					lnAttr += lpTemplateDimension->Count();
				}
                    break;
                    
                default:
                    return E_FAIL;
			} // end of switch lUnitType
		} // end of for(iUnit = 0; iUnit < lnUnit; iUnit ++)
        
		// convert the AttributeID into the position in the tabular data
		lpAttrList = new Int32[lnAttr];
		CHECK_NEWED_PTR(lpAttrList);
        
		lnAttr = 0;
		for(iUnit = 0; iUnit < lnUnit; iUnit++)
		{
			DSSTemplateUnit *lpTemplateUnit = lpAxis -> Item(iUnit);
			CHECK_PTR(lpTemplateUnit);
            
			DSSTemplateUnit_Type lUnitType = lpTemplateUnit -> getType();
            
			hr = S_OK;
			DSS_ID lID = GUID_NULL;
			switch (lUnitType)
			{
                case DssTemplateAttribute:
				{
					DSSTemplateAttribute *lpTemplateAttr = dynamic_cast<DSSTemplateAttribute*>(lpTemplateUnit);
					if(!lpTemplateAttr) return E_FAIL;
					
					lID = lpTemplateAttr->getID();
                    
					// aye, 291064
					// look for attributes that are converted to CustomGroup or Consolidation
					// fail if we cannot find the ID.
					hr = ipTabularData->FindUnitI(lID, DssTemplateReserved, lpAttrList[lnAttr]);
					if(FAILED(hr)) return hr;
                    
					lnAttr++;
				}
					break;
                    
                    // Incomplete_Consolidation or Metrics
                case DssTemplateConsolidation:
				{
					DSSTemplateConsolidation *lpTemplateConsolidation = dynamic_cast<DSSTemplateConsolidation*>(lpTemplateUnit);
					if(!lpTemplateConsolidation) return E_FAIL;
					
					lID = lpTemplateConsolidation -> getID();
                    
					hr = ipTabularData->FindUnitI(lID, DssTemplateConsolidation, lpAttrList[lnAttr]);
					if(FAILED(hr)) return hr;
                    
					lnAttr++;
				}
                    
                    break;
                case DssTemplateCustomGroup:
				{
					DSSTemplateCustomGroup *lpTemplateCustomGroup = dynamic_cast<DSSTemplateCustomGroup*>(lpTemplateUnit);
					if(!lpTemplateCustomGroup) return E_FAIL;
					
					lID = lpTemplateCustomGroup -> getID();
                    
					hr = ipTabularData->FindUnitI(lID, DssTemplateCustomGroup, lpAttrList[lnAttr]);
					if(FAILED(hr)) return hr;
                    
					lnAttr++;
				}
                    
                    break;
                case DssTemplateMetrics:
					break;
                    
                case DssTemplateDimension:
				{
					Int32 iDimUnit = 0 ;
					DSSTemplateDimension *lpTemplateDimension = dynamic_cast<DSSTemplateDimension*>(lpTemplateUnit);
                    
					if(!lpTemplateUnit) return hr = E_FAIL;
                    
					for(iDimUnit = 0; iDimUnit < lpTemplateDimension->Count(); iDimUnit++)
					{
						DSSTemplateAttribute *lpTemplateAttr = lpTemplateDimension->Item(iDimUnit);
						CHECK_PTR(lpTemplateAttr);
                        
						DSS_ID lID = lpTemplateAttr->getID();
                        
						hr = ipTabularData->FindUnitI(lID, DssTemplateReserved, lpAttrList[lnAttr]);
                        
						if (hr == S_FALSE)
							return E_FAIL;
                        
						if(FAILED(hr)) return hr;
                        
						lnAttr++;
					}
					break;
				}
                    
                default:
					return E_FAIL;
			} // end of switch(lUnitType)
            
			if (hr == S_FALSE)
				return E_FAIL;
            
		} // end of for(iUnit=0; iUnit<lnUnit; iUnit++)
        
		// Axis index
		Int32 lIndex = lpAxis->getIndex();
        
		if (opAxisIndex)
			opAxisIndex->push_back(lIndex);
        
		// insert into the lpvpAttrInAxis and lpvnAttrInAxis
		opvAttrInAxis->push_back(lnAttr);
		oppvAttrInAxis->push_back(lpAttrList);
        
        
	} // for(long iAxis = 0; iAxis < lnAxes; iAxis++)
    
	return hr;
}

//fali, 816976, for those datasets which subtotalDimentionalityAware=false, expand slice to mainIndex level.
int DSSSubtotalEventEvaluator::ExpandSlice(DSSCube * ipCube,
                                           int iSliceID,
                                           DSSSliceID *opSliceID)
{
    
    int hr;
    opSliceID->mSliceID = iSliceID;
    opSliceID->mpCube = ipCube;
    
    DSSTabularIndexPool *oppIndexPool;
    hr = ipCube->getIndexPool(&oppIndexPool);
    CHECK_HR;
    
    DSSTabularIndex *lpMainIndex;
    oppIndexPool->getMainIndex(&lpMainIndex);
    CHECK_HR;
    
    int *lpUnits = 0 ;
    int lUnits = lpMainIndex -> getNumUnits();
    CHECK_HR;
    
    lpUnits = new int[lUnits];
    CHECK_NEWED_PTR(lpUnits);
    
    hr = lpMainIndex -> getUnits(lUnits, lpUnits);
    CHECK_HR;
    
    DSSCubeDataSlice* lpSlice2 = ipCube -> getSliceByID(iSliceID);
    
    DSSTabularIndex* lpIndices2 = lpSlice2 -> getIndices();
    
    Int32 *lpUnits2 = 0 ;
    int lUnits2 = lpIndices2 -> getNumUnits();
    
    lpUnits2 = new int[lUnits2];
    CHECK_NEWED_PTR(lpUnits2);
    
    hr = lpIndices2 -> getUnits(lUnits2, lpUnits2);
    CHECK_HR;
    
    bool match = false;
    if(lUnits == lUnits2)
        match = includes(lpUnits, lpUnits + lUnits, lpUnits2,  lpUnits2 + lUnits2);
    
    if(!match)
    {      //xhan 10/07/05 TQMS 226892, streching needs to deal with non-numericals as well as numericals.
        DSSCube *lpCubeC = static_cast<DSSCube*>(ipCube);
        
        DSSCubeDataSlice* lpSliceOut;
        hr = DSSCalculationHelper::StrechSliceDownward(lpCubeC, //ipBuffer,
                                                       lpSlice2,
                                                       lUnits2, lpUnits2,
                                                       lUnits, lpUnits,
                                                       &lpSliceOut);
        CHECK_HR_PTR(lpSliceOut);
        
        hr = lpCubeC->InsertDataSlice(lpSliceOut, (opSliceID->mSliceID) );
        CHECK_HR;
        
        opSliceID->mpCube = ipCube;
        
    }
    
    return S_OK;
    
}

