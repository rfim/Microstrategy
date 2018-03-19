//
//  DSSXTabViewHelper.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 11/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "DSSXTabViewHelper.h"

#include "DSSAEHelper.h"

#include "DSSTemplate.h"
#include "DSSTabularData.h"
#include "DSSTabularUnit.h"
#include "DSSXTabDispInfo.h"
#include "DSSXTabLayoutInfo.h"
#include "DSSViewDataSet.h"
#include "DSSXTabView.h"
#include "DSSStrongPtr.h"

#include "DSSRWFieldGroupModel.h"
#include "DSSMaskInfos.h"
#include "DSSBuiltInPackage.h"
#include "DSSLevelResolver.h"
#include "DSSCubeDataSlice.h"
#include "DSSCalculationHelper.h"
#include "DSSFunctionServer.h"
#define ZERO_EPLISON (1.0e-15)

int DSSXTabViewHelper::InitXTabView(DSSViewDataSet *ipViewDataSet, 
                                    DSSTemplate *ipRuntimeTemplate,
                                    DSSXTabView **opView,
                                    DSSRWGridModel *ipGridModel,
                                    bool iDoPreXTab, DSSMaskInfos *ipMaskInfo, bool ibHideZero, bool ibHideNull, MCESchema* ipSchema)
{
	if (!(ipViewDataSet && ipRuntimeTemplate && ipGridModel)) 
		return E_POINTER;
        
    // tehe: now threshold will serialized from server side and saved in cube slice  
	
    //get all the metric currently in the Template
	//the number might differ from the one in TabularData
	int lnMetric = 0;
	DSSTemplateMetrics* lpTemplateMetrics;
	lpTemplateMetrics = ipRuntimeTemplate -> getTemplateMetrics();	
	//lpTemplateMetrics is OK to be NULL
	
	if(!lpTemplateMetrics)
		lnMetric = 0;
	else
		lnMetric = lpTemplateMetrics -> Count();
    
    
	//now I can new a DispInfo and insert all the metric in.
	DSSXTabDispInfo* lpDispInfo = new DSSXTabDispInfo();
    MBase::DSSStrongPtr<DSSXTabDispInfo> lDispInfoStrongPtr(lpDispInfo);
	int hr = lpDispInfo -> init(lnMetric);
	CHECK_HR;
   
	// Extract DisplayInfo
	hr = ExtractDispInfo(ipRuntimeTemplate, 
						 ipViewDataSet, 
                         lpDispInfo);
	CHECK_HR;
    
	//now we are ready to construct the view
	// New a XTabView
	DSSXTabView* lpView;
	hr = ipViewDataSet -> RefreshXTabView(lpDispInfo, &lpView);
	CHECK_HR;

    //fali, 815802, if it's initial loading, ipMaskInfo is not null, else generate it.
    if (ipMaskInfo)
        lpView->setHideNullZeroMaskInfo(ipMaskInfo);
    else if (ipGridModel->getCurrentSliceFlag() != DssBinaryContextDetailInitialSlice && ipGridModel->IsEvaluatedOnClient() && (ibHideZero || ibHideNull))
    {        
        DSSMaskInfos* lpMaskInfos = new DSSMaskInfos();
        //Currently we hard code hideNull=true, hideZero=true, need serialize the related setting type.
        hr=CalculateMaskInfoOfMainIndex(ipViewDataSet, lpTemplateMetrics,
                                     ibHideZero, ibHideNull, ipSchema, lpMaskInfos, ipViewDataSet->GetCube());
        CHECK_HR;
        lpView->setHideNullZeroMaskInfo(lpMaskInfos);
    }
	
    lpView->Set_DoPreXTab(iDoPreXTab);
    DSSTabularData* lpTabularData;
	hr = ipViewDataSet -> getTabularData(&lpTabularData);
    CHECK_HR;

    
	// New an XTabLayout object
	DSSXTabLayoutInfo* lpLayoutInfo = new DSSXTabLayoutInfo();
    MBase::DSSStrongPtr<DSSXTabLayoutInfo> lpLayoutInfoStrongPtr(lpLayoutInfo);
	// Initialize the layout object
	hr = lpLayoutInfo -> Init(ipRuntimeTemplate, lpTabularData, ipGridModel);
	CHECK_HR;
    
	// Initialize the New View
	hr = lpView -> Init(lpLayoutInfo, NULL);
	CHECK_HR;
     
    // tehe: here we put the sort logic in the GetCurrentWindow of xtabview
    
    //zhyang, extreme, trend line, reference line info
    lpView->mmapIncludeBreakBy = ipViewDataSet->mmapIncludeBreakBy;
    lpView->mmapElementKeys = ipViewDataSet->mmapElementKeys;
    lpView->mmapLabels = ipViewDataSet->mmapLabels;
    lpView->mmapNewExtremes = ipViewDataSet->mmapNewExtremes;
    lpView->mmapForecastInfos = ipViewDataSet->mmapForecastInfos;
    
	//Display is processed, XTabView is constructed and linked with ViewResult
	if (opView)   // qxue: make opView optional (used in CDSSFieldGroupModel)
	{
		*opView = lpView;
	}
	return S_OK;
}

int DSSXTabViewHelper::RefreshThresholdSlices(DSSViewDataSet *ipViewDataSet,
                                              DSSTemplate *ipRuntimeTemplate,
                                              DSSXTabView *ipView)
{
    int hr = S_OK;
    
    if(!ipView)
        return S_OK;
    
	if(!ipRuntimeTemplate || !ipViewDataSet) return E_POINTER; // EH: should not pass in NULL pointer
    
	//go throught the final events and put the slice id to the disp info
	DSSTemplateMetrics *lpTemplateMetrics;
	lpTemplateMetrics = ipRuntimeTemplate -> getTemplateMetrics();
	//lpTemplateMetrics is OK to be NULL
    
	int lNumMetrics = 0,  iMetric = 0 ;
	if(!lpTemplateMetrics)
		lNumMetrics = 0;
	else
	{
		lNumMetrics = lpTemplateMetrics -> Count();
	}
	
	// get the current calculation plan
	DSSCalculationPlan *lpCalculationPlan;
	lpCalculationPlan = ipViewDataSet -> getCalculationPlan();
	CHECK_PTR(lpCalculationPlan);	// EH: the chance is the template was not evaluated yet.
    
	// insert all the slices into the display info
	int lNumEvents = 0;
	lNumEvents = lpCalculationPlan->getNumEvents();
	
	int lNumLevels = 0;
	lNumLevels = lpCalculationPlan->getNumLevels();
    
	for(int iLevel = 0; iLevel < lNumLevels; iLevel++)
	{
		// build the level in the display info first
		vector<int> lvTotalUnits;
		int lTemplateSubtotal = 0;
        hr = lpCalculationPlan->getLevelInfo(iLevel, lvTotalUnits, lTemplateSubtotal);
        CHECK_HR;
        
		for(iMetric = 0; iMetric < lNumMetrics; iMetric ++)
		{
			DSSTemplateMetric *lpTemplateMetric;
            lpTemplateMetric = lpTemplateMetrics -> Item(iMetric);
            
			GUID lMetricID;
			lMetricID = lpTemplateMetric -> getID();
            
            int lThresholdSliceID = -1;
            hr = lpCalculationPlan->getMetricThresholdSliceID(iLevel,lMetricID, &lThresholdSliceID);
            CHECK_HR;
            
            if(lThresholdSliceID != -1)
            {
                ipView->ResetThresholdSlice(lvTotalUnits, lThresholdSliceID, iMetric, lTemplateSubtotal, DssDisplayIndexSlice);
                CHECK_HR;
            }
		}
        
        int lNumAttris = lpCalculationPlan->getNumAttriThresholds();
        for (int iAttri = 0; iAttri < lNumAttris; iAttri++)
        {
            const GUID& lAttriID = lpCalculationPlan->getAttriID(iAttri);
            int lThresholdSliceID = -1;
            hr = lpCalculationPlan->getAttriThresholdSliceID(iLevel, lAttriID, &lThresholdSliceID, NULL);
            CHECK_HR;
            
            if (lThresholdSliceID != -1)
            {
                ipView->addAttributeThresholdSlice(iLevel, lTemplateSubtotal, &lAttriID, lThresholdSliceID);
                CHECK_HR;
            }
        }
    }
    return hr;
}

int DSSXTabViewHelper::ExtractDispInfo(DSSTemplate * pTemplate,
                                                              DSSViewDataSet * pViewDataSet, 
                                                              DSSXTabDispInfo * pDispInfo)	
{
    int hr = S_OK;
        
	if(!pTemplate || !pViewDataSet || !pDispInfo) return E_POINTER; // EH: should not pass in NULL pointer
    
	//go throught the final events and put the slice id to the disp info
	DSSTemplateMetrics *lpTemplateMetrics;
	lpTemplateMetrics = pTemplate -> getTemplateMetrics();
	//lpTemplateMetrics is OK to be NULL
    
	int lNumMetrics = 0,  iMetric = 0 ;
	if(!lpTemplateMetrics)
		lNumMetrics = 0;
	else
	{
		lNumMetrics = lpTemplateMetrics -> Count();
	}
	
    //get tabular data
    DSSCube* lpCurCube ;
    lpCurCube = pViewDataSet->GetCube();
    CHECK_PTR(lpCurCube)
    DSSTabularData* lpTabularData;
    lpTabularData = lpCurCube->getTabularData();
    CHECK_PTR(lpTabularData);
    
    
	// get the current calculation plan
	DSSCalculationPlan *lpCalculationPlan;
	lpCalculationPlan = pViewDataSet -> getCalculationPlan();
	CHECK_PTR(lpCalculationPlan);	// EH: the chance is the template was not evaluated yet.
    
	// insert all the slices into the display info
	
	int lNumLevels = 0;
	lNumLevels = lpCalculationPlan->getNumLevels();
    
	for(int iLevel = 0; iLevel < lNumLevels; iLevel++)
	{
		// build the level in the display info first
		vector<int> lvTotalUnits;
		int lTemplateSubtotal = 0,  lLevel = 0 ;
        EnumDSSRuntimeSubtotalType lExtremeType = DssRuntimeSubtotalDefault;
        
        hr = lpCalculationPlan->getLevelInfo(iLevel, lvTotalUnits, lTemplateSubtotal);
        CHECK_HR;
        
        if(iLevel != 0)
        {
            hr = lpCalculationPlan->getExtremeType(iLevel, &lExtremeType);
            CHECK_HR;
        }
        
        //int lvUnitsSize = lvTotalUnits.size();
        //int * lpvUnits = (lvUnitsSize>0)?&lvTotalUnits[0]:NULL;
       // hr = pDispInfo -> AddLevel(lvUnitsSize, lpvUnits, &lLevel);
        //CHECK_HR;
        //msun, financial report.
        hr = hReplaceWithRecursiveUnit (lpTabularData,pDispInfo,lvTotalUnits,&lLevel);
        CHECK_HR
        
		for(iMetric = 0; iMetric < lNumMetrics; iMetric ++)
		{
			DSSTemplateMetric *lpTemplateMetric;
            lpTemplateMetric = lpTemplateMetrics -> Item(iMetric);
			            
			GUID lMetricID;
			lMetricID = lpTemplateMetric -> getID();
			            
			bool lfIsCalculated = false;
			int lSliceID = 0;
            lfIsCalculated = true;
            
            if (lExtremeType != DssRuntimeSubtotalDefault && lExtremeType != DssRuntimeSubtotalPieTotal)
                hr = lpCalculationPlan->getMetricExtremeSliceID(iLevel, lMetricID, &lfIsCalculated, &lSliceID);
            else
                hr = lpCalculationPlan->getMetricSliceID(iLevel,lMetricID, &lfIsCalculated, &lSliceID);
            
            if (hr != S_OK)
            {
                // DE17663 defensive code to avoid crash in release, if metric id is mismatch in definition mode, ignore it in release build
                assert(false);
                continue;
            }
            
			if(!lfIsCalculated)	 continue;		//somehow the calculation does not work out
            if(-1 == lSliceID) continue;
            
			if (lExtremeType != DssRuntimeSubtotalDefault && lExtremeType != DssRuntimeSubtotalPieTotal)
            {
                Int32 lModelIndex = -1;
                int lTRMetricID = -1;
                Int32 lnXVariables = 0;
                int *lpXVariables = NULL;
                EnumDSSObjectType *lpXVariableTypes = NULL;
                int *lpXIndexMetric = NULL;
                
                hr = lpCalculationPlan->getModelInfo(iLevel, lModelIndex, lTRMetricID, lnXVariables, lpXVariables, lpXVariableTypes, lpXIndexMetric);
                CHECK_HR;
                
                if (lModelIndex > -1)
                {
                    hr = pDispInfo->put_SliceInfo(lLevel,
                                                  lSliceID,
                                                  iMetric,
                                                  lTemplateSubtotal,
                                                  DssDisplayExtremeSlice,
                                                  lExtremeType,
                                                  lModelIndex,
                                                  lTRMetricID,
                                                  lnXVariables,
                                                  lnXVariables > 0 ? &lpXVariables[0]:NULL,
                                                  lnXVariables > 0 ? lpXVariableTypes:NULL,
                                                  lnXVariables > 0 ? &lpXIndexMetric[0]:NULL);
                    CHECK_HR;
                }
                else
                {
                    hr = pDispInfo->put_SliceInfo(lLevel,
                                                  lSliceID,
                                                  iMetric,
                                                  lTemplateSubtotal,
                                                  DssDisplayExtremeSlice,
                                                  lExtremeType
                                                  );
                    CHECK_HR;
                }
                continue;
            }
            
			hr = pDispInfo -> put_SliceInfo(lLevel,
											lSliceID,
											iMetric,
											lTemplateSubtotal);
			CHECK_HR;
            
            int lThresholdSliceID = -1;
            hr = lpCalculationPlan->getMetricThresholdSliceID(iLevel,lMetricID, &lThresholdSliceID);
            CHECK_HR;
            
            if(lThresholdSliceID != -1)
            {
                hr = pDispInfo -> put_SliceInfo(lLevel,
                                                lThresholdSliceID,
                                                iMetric,
                                                lTemplateSubtotal,
                                                DssDisplayIndexSlice);
            }
		}
        
        
        int lNumAttrThresholds = lpCalculationPlan->getNumAttriThresholds();
        for (int i = 0; i < lNumAttrThresholds; i++)
        {
            DSS_ID lAttrID = lpCalculationPlan->getAttriID(i);
            
            int lSliceID = -1;
            lpCalculationPlan->getAttriThresholdSliceID(lLevel, i, &lSliceID);
            
            hr = pDispInfo->putAttributeThresholdSliceInfo(lLevel, lTemplateSubtotal, lAttrID, lSliceID);
            CHECK_HR;
        }
	}
    
	return S_OK;
}

int DSSXTabViewHelper::InitXTabViewForFieldGroup(DSSViewDataSet *ipViewDataSet, /*DSSRTTemplateInfo *ipTemplateInfo, */DSSXTabView **opView, bool iDoPreXTab)
{
    int hr = S_OK;
    
    if (!(ipViewDataSet /*&& ipTemplateInfo*/)) 
		return E_POINTER;
    
   	int lnMetric = 0;
    // hr = ipTemplateInfo->Count(&lnMetric);
	
//    if(lnMetric <= 0)
//        return S_OK;
    lnMetric = ipViewDataSet->getTabularData()->getMetricsCount();
	//now I can new a DispInfo and insert all the metric in.
	DSSXTabDispInfo* lpDispInfo = new DSSXTabDispInfo();
    MBase::DSSStrongPtr<DSSXTabDispInfo> lDispInfoStrongPtr(lpDispInfo);
	hr = lpDispInfo -> init(lnMetric);
	CHECK_HR;
    
	// Extract DisplayInfo
	hr = ExtractDispInfoForFieldGroup(/*ipTemplateInfo, */
						 ipViewDataSet, 
                         lpDispInfo
                                      );
	CHECK_HR;
    
	//now we are ready to construct the view
	// New a XTabView
	DSSXTabView* lpView;
	hr = ipViewDataSet -> RefreshXTabView(lpDispInfo, &lpView);
	CHECK_HR;    
    
    lpView->Set_DoPreXTab(iDoPreXTab);

    if (opView) 
	{
		*opView = lpView;
	}
	return S_OK;

}


int DSSXTabViewHelper::ExtractDispInfoForFieldGroup(/*DSSRTTemplateInfo *ipTemplateInfo,*/ DSSViewDataSet *ipViewDataSet, DSSXTabDispInfo *pDispInfo)
{
    int hr = S_OK;
    
	if(/*!ipTemplateInfo || */!ipViewDataSet || !pDispInfo) 
        return E_POINTER;     
	
    int lNumMetrics = 0;

    // hr = ipTemplateInfo->Count(&lNumMetrics);
    lNumMetrics = ipViewDataSet->getTabularData()->getMetricsCount();
    // ipViewDataSet->getMetr
	// get the current calculation plan
	DSSCalculationPlan *lpCalculationPlan;
	lpCalculationPlan = ipViewDataSet -> getCalculationPlan();
	CHECK_PTR(lpCalculationPlan);	
    
	// insert all the slices into the display info
	
	int lNumLevels = 0;
	lNumLevels = lpCalculationPlan->getNumLevels();
    
	for(int iLevel = 0; iLevel < lNumLevels; iLevel++)
	{
		// build the level in the display info first
		vector<int> lvTotalUnits;
		int lTemplateSubtotal = 0,  lLevel = 0 ;
        hr = lpCalculationPlan->getLevelInfo(iLevel, lvTotalUnits, lTemplateSubtotal);
        CHECK_HR;
        int lvUnitsSize = lvTotalUnits.size();
        int * lpvUnits = (lvUnitsSize>0)?&lvTotalUnits[0]:NULL;
        hr = pDispInfo -> AddLevel(lvUnitsSize, lpvUnits, &lLevel);
        CHECK_HR;
        
        int iMetric = 0;
		for(iMetric = 0; iMetric < lNumMetrics; iMetric ++)
		{
			GUID lMetricID;
			// hr = ipTemplateInfo -> GetMetricByIndex(iMetric+1, &lMetricID);
            lMetricID = ipViewDataSet->getTabularData()->getMetricID(iMetric);
            CHECK_HR;
            
			bool lfIsCalculated = false;
			int lSliceID = 0;
            lfIsCalculated = true;
            
            hr = lpCalculationPlan->getMetricSliceID(iLevel,lMetricID, &lfIsCalculated, &lSliceID);
//            CHECK_HR;
            //xiazhou, 948942, 08/13/2014. It's possible that calculation plan has less metrics than tabular data.
            if(hr != S_OK)
            {
                hr = S_OK;
                continue;
            }
            
			if(!lfIsCalculated)	 continue;		//somehow the calculation does not work out
            
			hr = pDispInfo -> put_SliceInfo(lLevel, 
											lSliceID, 
											iMetric, 
											lTemplateSubtotal);
			CHECK_HR;
		}
        
	}
    
	return S_OK;

}

int DSSXTabViewHelper::CalculateMaskInfoOfMainIndex(DSSViewDataSet* ipViewDataSet,DSSTemplateMetrics *ipTemplateMetrics,
                                                                           bool bHideZero, bool bHideNull, MCESchema* ipSchema, DSSMaskInfos* opMaskInfos, DSSCube* ipTargetCube)
{
    int hr = S_OK;
    DSSCube* lpViewCube = ipViewDataSet->GetCube();
    // Calculation plan can be got from View Dataset
    DSSCalculationPlan* lpCalculationPlan = ipViewDataSet->getCalculationPlan();
    
    int lNumEvent = lpCalculationPlan->getNumEvents();
    int lnMetric = 0;
    if(!ipTemplateMetrics)
        lnMetric = 0;
    else
    {
        lnMetric = ipTemplateMetrics->Count();
    }
    
    int lFinalEvent = lNumEvent - 1;
    int lLevel = 0;    //the original level
    
    DSSFunctionObject* lpFunctionOrObject = DSSBuiltInPackage::getFunctionByIndex(DssFunctionOr);
    MBase::DSSStrongPtr<DSSFunctionObject> lFunctionOrObjectPtr(lpFunctionOrObject);
    
    DSSSliceID lFilterSliceID;
    DSSSliceID lResultFilterSliceID;
    
    for (int i = 0; i < lnMetric; i ++)
    {
        DSSTemplateMetric* lpTemplateMetric = ipTemplateMetrics->Item(i);
        
//        DSS_ID lMetricID = lpCalculationPlan->getMetricID(i);
        DSSMetric* lMetric = lpTemplateMetric->getMetric();
        DSS_ID lMetricID = lMetric->getID();
        
        int lMetricIndex = lpCalculationPlan->getMetricIndex(lMetricID);

        int lSliceID;
        bool lbIsCalculated;
        hr = lpCalculationPlan->getMetricSliceID(lLevel, lFinalEvent, lMetricIndex, &lbIsCalculated, &lSliceID);
        CHECK_HR;
        
        DSSSliceID tmpCDSSliceID(lpViewCube, lSliceID);
        hr = CalculateHideNullorHideZero(tmpCDSSliceID, bHideZero, bHideNull, ipSchema, lFilterSliceID, ipTargetCube);
        CHECK_HR;
        
        if(i == 0)
        {
            lResultFilterSliceID = lFilterSliceID;
        }
        else
        {
            DSSSliceID lParam[2];
            lParam[0] = lResultFilterSliceID;
            lParam[1] = lFilterSliceID;
            
           hr = DSSCalculationHelper::CalculateAndOR(lpFunctionOrObject,
                                                     2, lParam,
                                                     ipTargetCube,
                                                     DssNumericErrorSkip/*iNULLChecking*/,
                                                     false/*bCalcHiddenDE*/,
                                                     &lResultFilterSliceID,
                                                     ipSchema);
            CHECK_HR;
        }
    }
    
    DSSCubeDataSlice* lpResultFilterSlice;
    hr = lResultFilterSliceID.GetSlice(&lpResultFilterSlice);
    CHECK_HR_PTR(lpResultFilterSlice);
    
//    //got the index of lpResultFilterSlice
    DSSTabularIndex* lpResultIndex = lpResultFilterSlice->getIndices();
    
    //Get the main index of the view template by IndexPool
    DSSTabularData* lpTabularData = lpViewCube-> getTabularData();

    DSSTabularIndexPool* lpIndexPool = lpTabularData->getIndexPool();
    
    DSSTabularIndex* lpViewDTMainIndex = NULL;
    hr = lpIndexPool->getMainIndex(&lpViewDTMainIndex);
    CHECK_HR_PTR(lpViewDTMainIndex);

    //if the current index is not the main index,stretch the current index to main index
    if(lpResultIndex->IsSameOrder(lpViewDTMainIndex) == false)
    {
        //To expand the current index to the view main index
        int lnUnit = lpResultIndex->getNumUnits();
        
        int* lpUnits = new int[lnUnit];
        MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lUnitPtr(lpUnits);
        hr = lpResultIndex->getUnits(lnUnit, lpUnits);
        CHECK_HR;
        
        int lnTargetLevel = lpViewDTMainIndex->getNumUnits();
        
        int* lpTargetLevel = new int[lnTargetLevel];
        MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lTargetLevelPtr(lpTargetLevel);
        hr = lpViewDTMainIndex->getUnits(lnTargetLevel, lpTargetLevel);
        CHECK_HR;
        
        DSSCubeDataSlice* lpTmpResultFilerSlice;
        hr = DSSCalculationHelper::StrechSliceDownward(ipTargetCube,
                                                        lpResultFilterSlice, lnUnit, lpUnits,
                                                        lnTargetLevel, lpTargetLevel, &lpTmpResultFilerSlice);
        CHECK_HR_PTR(lpTmpResultFilerSlice);
        
        lpResultFilterSlice = lpTmpResultFilerSlice;
    }

    Int32 lnRow = lpViewDTMainIndex->Count();
    
    DSSXTabBitVector* lpMask = new DSSXTabBitVector(lnRow);
    
    DSSDataColumn* lpColumn = lpResultFilterSlice->getColumn();
    CHECK_HR_PTR(lpColumn);
    
    double lVal = 0;
    DSSData_Flags lFlag = DssDataOk;
    for (Int32 i = 0; i < lnRow; i++)
    {
        hr = lpColumn->getNumericVal(i, lVal, lFlag);
        CHECK_HR;
        
        if (lVal)
            lpMask->Set(i, true);
    }

    DSSMaskInfos* lNullOrZeroMaskInfos = new DSSMaskInfos();
    
    hr = lNullOrZeroMaskInfos->Init(lpViewDTMainIndex, ipTargetCube, lpMask);
    CHECK_HR;
    
    *opMaskInfos = *lNullOrZeroMaskInfos;

    return S_OK;
}

int DSSXTabViewHelper::CalculateHideNullorHideZero(DSSSliceID &irSliceID, bool bHideZero, bool bHideNull, MCESchema* ipSchema, DSSSliceID &orFilterSliceID, DSSCube* opTargetCube)
{
    int hr = S_OK;
    bool lMissingorNotExisted;
    hr = CheckMissing(irSliceID, lMissingorNotExisted);//True means SliceID = -1 or Slice type = missing
    CHECK_HR;
    
    DSSCubeDataSlice* lpSlice = NULL;
    DSSTabularIndex* lpXTabIndices = NULL;
    int lnUnit = 0;
    int *lpUnit = NULL;
    int lnRow = 0;
    DSSDataColumn* lpColumn = NULL;
    
    if(irSliceID.mSliceID != -1)
    {
        hr = irSliceID.GetSlice(&lpSlice);
        CHECK_HR_PTR(lpSlice);
        
        lpXTabIndices = lpSlice->getIndices();
        lnUnit = lpXTabIndices->getNumUnits();
        lpUnit = new int[lnUnit];
        hr = lpXTabIndices->getUnits(lnUnit, lpUnit);
        CHECK_HR_PTR(lpUnit);
        
        lnRow = lpXTabIndices->Count();
        lpColumn = lpSlice->getColumn();
    }
    
    bool lNumeric;
    if(!lMissingorNotExisted) // Check the slice whether it is a numerical slice if we can find the slice in the cube
    {
        BOOL lIsNumerical;
        BOOL lCellFormatSlice;
        //It does not matter that use oppTargetCube for the IsNumericalSlice function although lpCDSSCubeSlice is not in oppTargetCube.
        hr = opTargetCube->IsNumericalSlice(lpSlice, lIsNumerical, lCellFormatSlice);
        CHECK_HR;
        
        if(lIsNumerical)
            lNumeric = true;
        else
            lNumeric = false;
    }
    
    DSSSliceID lSliceID1;
    if(bHideZero)
    {
        short lVal = 1;
        hr = DSSCalculationHelper::GetConstantSlice(opTargetCube, lnUnit, lpUnit, reinterpret_cast<char *>(&lVal),
                                                     sizeof(short), DssDataTypeShort, DssDataOk, &lSliceID1);
        CHECK_HR;
        
        //If SliceID = -1 or Slice type = missing, we can directly set the result to 1
        //Otherwise, we need to do the value comparison
        if(!lMissingorNotExisted && lNumeric)
        {
            DSSCubeDataSlice* lpCubeSlice;
            hr = lSliceID1.GetSlice(&lpCubeSlice);
            CHECK_HR_PTR(lpCubeSlice);
            
            DSSDataColumn* lpResultTableColumn = lpCubeSlice->getColumn();
            
            DSSData_Flags lStatus = DssDataOk;
            double lValue = 0;
            
            for(Int32 i = 0; i < lnRow; i ++)
            {
                hr = lpColumn->getNumericVal(i, lValue, lStatus);
                CHECK_HR;
                if (lStatus == DssDataOk)
                {
                    double ltmp = lValue > 0 ? lValue : -lValue;
                    if(ltmp < ZERO_EPLISON) //data equal 0
                    {
                        short lVal = 0;
                        hr = lpResultTableColumn->setData(i, (char*)&lVal, sizeof(short), DssDataOk);
                        CHECK_HR;
                    }
                }
            }
        }
    }
    
    DSSSliceID lSliceID2;
    if(bHideNull)
    {
        //create constant slice 0
        //when the lMissingorNotExisted is true, just create a constant slice whose value is 0 is ok
        short lVal = 0;
        hr = DSSCalculationHelper::GetConstantSlice(opTargetCube, lnUnit, lpUnit, reinterpret_cast<char *>(&lVal),
                                                     sizeof(short), DssDataTypeShort, DssDataOk, &lSliceID2);
        CHECK_HR;
        
        if(!lMissingorNotExisted)//lMissingorNotExisted is false, we need to check the Flags
        {
            DSSCubeDataSlice* lpCubeSlice;
            hr = lSliceID2.GetSlice(&lpCubeSlice);
            CHECK_HR_PTR(lpCubeSlice);
            
            DSSDataColumn* lpResultTableColumn = lpCubeSlice->getColumn();
            
            DSSData_Flags lStatus = DssDataOk;
            char* lpData = NULL;
            int lSize;
            EnumDSSDataType lType;
            for(Int32 i = 0; i < lnRow; i ++)
            {
                hr = lpColumn->getData(i, (const char**)&lpData, lSize, lType, lStatus);
                CHECK_HR;
                
                if(lStatus == DssDataOk)
                {
                    short lVal = 1;
                    hr = lpResultTableColumn->setData(i, (char*)&lVal, sizeof(short), DssDataOk);
                    CHECK_HR;
                }
            }
    
        }
    }
    
    if(bHideZero && bHideNull)
    {
        DSSFunctionObject* lpFunctionObject = DSSBuiltInPackage::getFunctionByIndex(DssFunctionAnd);
        MBase::DSSStrongPtr<DSSFunctionObject> lFunctionObjectPtr(lpFunctionObject);
        DSSSliceID lParam[2];
        lParam[0] = lSliceID1;
        lParam[1] = lSliceID2;
        
        DSSLevelResolver lLevelResolver(opTargetCube);
        hr = lLevelResolver.Init();
        CHECK_HR;
        
        hr = DSSCalculationHelper::CalculateAndOR(lpFunctionObject,
                                                  2, lParam,
                                                  opTargetCube,
                                                  DssNumericErrorSkip/*iNULLChecking*/,
                                                  false/*bCalcHiddenDE*/,
                                                  &orFilterSliceID,
                                                  ipSchema);
        CHECK_HR;
    }
    else
    {
        if(bHideZero)
        {
            orFilterSliceID = lSliceID1;
        }
        else
        {
            orFilterSliceID = lSliceID2;
        }
    }
    return S_OK;
}

int DSSXTabViewHelper::CheckMissing(DSSSliceID &irSliceID, bool &bMissingorNotExisted)
{
    int hr = S_OK;
    
    bMissingorNotExisted = false;
    Int32 lSliceID = irSliceID.mSliceID;
    
    if(lSliceID == -1)
        bMissingorNotExisted = true;
    else
    {
        DSSCubeDataSlice* lpCubeSlice;
        hr = irSliceID.GetSlice(&lpCubeSlice);
        CHECK_HR_PTR(lpCubeSlice);
        
        DSSDataColumn* lpXTabColumn = lpCubeSlice->getColumn();
        
        EnumDSSDataType lType = lpXTabColumn->getDataType();
        
        if (lType ==  DssDataTypeMissing)
            bMissingorNotExisted = true;
    }
    return S_OK;
}

int DSSXTabViewHelper::hReplaceWithRecursiveUnit(DSSTabularData* ipTabularData, DSSXTabDispInfo* ipDispInfo, vector<int>& irvTotalUnits, int* opLevel)
{
    if (!ipTabularData || !ipDispInfo) {
        return E_POINTER;
    }
    int hr = S_OK;
    int lvTotalUnitSize = irvTotalUnits.size();
    vector<int> lvDisplayUnits;
    
    for (int i=0; i<lvTotalUnitSize; i++) {
        DSSTabularUnit* lpTabularUnit;
        lpTabularUnit = ipTabularData->Item(irvTotalUnits[i]);
        CHECK_PTR(lpTabularUnit);
        //to check possible recursive unit
        GUID lID = lpTabularUnit->getGUID();
        EnumDSSTabularDataUnitType lUnitType = (EnumDSSTabularDataUnitType)lpTabularUnit->getUnitType();
        switch (lUnitType) {
            case DssUnitAttribute:
                int lUnit;
                hr = ipTabularData->FindUnitI(lID, DssTemplateAttribute, lUnit, true);
                if (hr == S_OK) {
                    lvDisplayUnits.push_back(lUnit);
                }
                else
                {
                    lvDisplayUnits.push_back(irvTotalUnits[i]);
                }
                break;
                
            default:
                lvDisplayUnits.push_back(irvTotalUnits[i]);
                break;
        }
    }
    
    int * lpvUnits = (lvTotalUnitSize>0)?&lvDisplayUnits[0]:NULL;
    hr = ipDispInfo -> AddLevel(lvTotalUnitSize, lpvUnits, opLevel);
    CHECK_HR;
    
    return hr;
}