/*
 *  DSSCalculationPlan.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 1/8/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#include "DSSCalculationPlan.h"
#import "DSSDataModelXTabImpl.h"
#import "DSSObjectInfo.h"
#import "DSSTabularMetrics.h"
#import "CalculationEngine/DSSCalculationEvent.h"
#import "DSSXTabView.h"
#import "DSSCube.h"
#import "DSSViewDataSet.h"

int BuildCompoundMetricEvent(DSSDataModelXTabImpl *ipDataModel, vector<DSSCalculationEvent> &irvCalculationEvents)
{
	DSSTabularMetrics *lpMetrics = NULL;
	int hr = ipDataModel->getMetricsUnit(&lpMetrics);
	CHECK_HR;
	
	DSSObjectContext * lpObjectContext = ipDataModel->getParentInfo()->getObjectContext();
	int lnMetrics = lpMetrics->Count();
	
	int i = 0;
	for (i = 0; i < lnMetrics; i++)
	{
		GUID lMetricID = GUID_NULL;
		hr = lpMetrics->getMetricID(i, lMetricID);
		CHECK_HR;
		
		DSSObjectInfo *lpObj = lpObjectContext->FindObjectH(lMetricID);
		if (lpObj->getType() == DssTypeMetric)
		{
			bool lbIsDecomposable = false;
			if (lbIsDecomposable)
			{
				DSSCalculationEvent lEvent;
				lEvent.mEventType = DssEventCompoundMetric;
				lEvent.mID = lMetricID;
				lEvent.mEventTag = 0;
				lEvent.mEventIndex = irvCalculationEvents.size();
				
				irvCalculationEvents.push_back(lEvent);
			}
		}
	}
	return S_OK;
}

int BuildSubtotalEvent(DSSDataModelXTabImpl *ipDataModel, vector<DSSCalculationEvent> &irvCalculationEvents)
{
	int lnSubtotals = ipDataModel->getSubtotalCount();
	
	int i = 0;
	for (i = 0; i < lnSubtotals; i++)
	{
		DSSCalculationEvent lEvent;
		lEvent.mEventType = DssEventTemplateSubtotal;
		lEvent.mID = GUID_NULL;
		lEvent.mEventTag = i + 1;// ipDataModel->getSubtotalType(i - 1);
		lEvent.mEventIndex = irvCalculationEvents.size();
		
		irvCalculationEvents.push_back(lEvent);
	}
	return S_OK;
}

int hGetMetricIDs(DSSDataModelXTabImpl *ipDataModel, vector<int> &irvMetrics, vector<GUID> &irvMetricID)
{
	DSSTabularMetrics *lpMetrics = NULL;
	int hr = ipDataModel->getMetricsUnit(&lpMetrics);
	CHECK_HR;
	
	int lnMetrics = lpMetrics->Count();
	
	int i = 0;
	for (i = 0; i < lnMetrics; i++)
	{
		GUID lMetricID = GUID_NULL;
		hr = lpMetrics->getMetricID(i, lMetricID);
		CHECK_HR;
		
		irvMetricID.push_back(lMetricID);
		irvMetrics.push_back(i);
	}
	return S_OK;
}

int hGetLevelInfo(DSSDataModelXTabImpl *ipDataModel, vector<DSSCalculationPlan::LevelInfo> &orvLevelInfos)
{
	DSSXTabView *lpXTabView = ipDataModel->getXTabView();
	
	int lnLevels = lpXTabView->CountLevels();
	orvLevelInfos.resize(lnLevels);
	
	for (int i = 0; i < lnLevels; i++)
	{
		int lTotalID = 0;
		int hr = lpXTabView->getLevelInfo(i, lTotalID, orvLevelInfos[i].mvUnits);
		if (hr == S_OK && lTotalID > 0)
		{
			orvLevelInfos[i].mTemplateSubtotal = lTotalID; //ipDataModel->getSubtotalType(lTotalID - 1);
		}
		else
		{
			orvLevelInfos[i].mTemplateSubtotal = -1;
		}
	}
	
	return S_OK;
}



DSSCalculationPlan::DSSCalculationPlan():	mNumLevels(0), mNumEvents(0), mNumMetrics(0), mpDataModel(NULL)
,mOrderingRule(DssDefaultOrdering), mStatus(DssCalcStatusUnInitialized)
{}

DSSCalculationPlan::~DSSCalculationPlan()
{}

int DSSCalculationPlan::getNumUnits(int iLevel, int *opNumUnits)
{
    *opNumUnits = mvLevels[iLevel].mvUnits.size();
    return S_OK;
}


int DSSCalculationPlan::getUnitIDs(int iLevel, int iNumUnits, GUID *opUnitIDs)
{
    return E_NOTIMPL;
}

int DSSCalculationPlan::getUnits(int iLevel,  int iNumUnits, int *opUnitAxis, int *opUnitIndex)
{
    return E_NOTIMPL;
}

int DSSCalculationPlan::getUnitIDsAndTypes(int iLevel,  int iNumUnits, GUID* opUnitIDs, EnumDSSTemplateUnitType* opUnitTypes)
{
    return E_NOTIMPL;
}

int DSSCalculationPlan::getEventIndex(GUID iUnitID)
{
    int lEventIndex = -1;
    for(int i=0;i<mvCalculationEvents.size();i++)
    {
        if(mvCalculationEvents[i].mID == iUnitID)
        {
            lEventIndex = i;
            break;
        }
    }
    return lEventIndex;
}

int DSSCalculationPlan::Init(DSSDataModelXTabImpl *ipDataModel)
{
    //	int hr = S_OK;
    
    //	hGetMetricIDs(ipDataModel, mvComponentMetrics, mvMetricID);
    //	mvContainerMetrics.assign(mvComponentMetrics.begin(), mvComponentMetrics.end());
	
	//add original metric event
	DSSCalculationEvent lEvent;
	lEvent.mEventType = DssEventOriginalMetric;
	lEvent.mID = GUID_NULL;
	lEvent.mEventTag = -1;
	lEvent.mEventIndex = mvCalculationEvents.size();
	mvCalculationEvents.push_back(lEvent);
	
    //	hr = BuildSubtotalEvent(ipDataModel, mvCalculationEvents);
    //	CHECK_HR;
	
    //	hr = BuildCompoundMetricEvent(ipDataModel, mvCalculationEvents);
    //	CHECK_HR;
	
	mpDataModel = ipDataModel;
	
    //	hCheckCompoundMetricEvent();
	
    //	hr = hInitMappingTable(ipDataModel);
    //	CHECK_HR;
	
	return S_OK;
}

int DSSCalculationPlan::Process(bool isBaseDataset)
{
    if (!mpDataModel)
    {
        //new rwd engine
        hCheckCompoundMetricEvent();
        
        int hr = hNewInitMappingTable(isBaseDataset);
        CHECK_HR;
    }
    else
    {
	int hr= hGetLevelInfo(mpDataModel, mvLevels);
	CHECK_HR;
	
	hCheckCompoundMetricEvent();
	
	hr = hInitMappingTable(mpDataModel);
	CHECK_HR;
	}
    
	setStatus(DssCalcStatusCalculated);
	
	return S_OK;
}

int DSSCalculationPlan::hCheckCompoundMetricEvent()
{
	mvbIsDecomposable.resize(mvMetricID.size());
	
	for (int i = 0; i < mvContainerMetrics.size(); i++)
	{
		mvbIsDecomposable[mvContainerMetrics[i]] = false;
		for (int j = 0; j < mvCalculationEvents.size(); j++)
		{
			if (mvCalculationEvents[j].mEventType == DssEventCompoundMetric
				&& MBase::IsEqualGUID(mvCalculationEvents[j].mID, mvMetricID[mvContainerMetrics[i]]))
				mvbIsDecomposable[mvContainerMetrics[i]] = true;
		}
	}
	return S_OK;
}

int DSSCalculationPlan::hInitMappingTable(DSSDataModelXTabImpl *ipDataModel)
{
	int lnEvents = mvCalculationEvents.size();
	
	int lnMetrics = mvComponentMetrics.size();
	
	DSSXTabView *lpXTabView = ipDataModel->getXTabView();
	
	int lnLevels = lpXTabView->CountLevels();
	
	mvSliceMappingTable.resize(lnLevels * lnEvents * lnMetrics);		//mvSliceMappingTable[iLevel][iEvent][iMetric]
	mvSliceFlag.resize(lnLevels * lnEvents * lnMetrics);				//mvSliceFlag[iLevel][iEvent][iMetric]
	mvPreviousSlices.resize(lnLevels * lnMetrics);
	
	for (int i = 0; i < lnLevels; i++)
	{
		//set all levles at the original event to the original metric slice
		for (int m = 0; m < lnMetrics; m++)
		{
			int lMetricIndex = mvComponentMetrics[m];
			
			int lSliceID = lpXTabView->getCubeSliceID(0, lMetricIndex);
			
			bool lbIsCalculated = !mvbIsDecomposable[mvComponentMetrics[m]];	//set smart metric as un-calculated
			
			if (mvLevels[i].mTemplateSubtotal < 0 && i > 0)
			{
				//473388, this level is the cross of two different subtotals, set as invalid (no input)
				lSliceID = -1;
				lbIsCalculated = false;
			}
			
			int offset = i * lnEvents * lnMetrics + m;
			
			mvSliceMappingTable	[offset] = lSliceID;
			mvSliceFlag			[offset] = lbIsCalculated;
            
			lSliceID = lpXTabView->getCubeSliceID(i, lMetricIndex);
			mvPreviousSlices[i * lnMetrics + m] = lSliceID;
		}
		
		//all others are not calculated
		for (int e = 1; e < lnEvents; e++)
		{
			int *lpSlice = &mvSliceMappingTable[i * lnEvents * lnMetrics + e * lnMetrics];
			for (int j = 0; j < lnMetrics; j++)
			{
				lpSlice[j] = -1;
				mvSliceFlag[i * lnEvents * lnMetrics + e * lnMetrics + j] = false;
			}
		}
	}
	
	mNumLevels = lnLevels;
	mNumEvents = lnEvents;
	mNumMetrics = lnMetrics;
	
	return S_OK;
}

int DSSCalculationPlan::getNumEvents()
{
	return mNumEvents;
}

int DSSCalculationPlan::getNumLevels()
{
	return mNumLevels;
}

int DSSCalculationPlan::getNumMetrics()
{
	return mvComponentMetrics.size();
}

DSSCalculationEvent* DSSCalculationPlan::GetEvent(int iEvent)
{
	if (iEvent < mvCalculationEvents.size())
		return &mvCalculationEvents[iEvent];
	else
		return NULL;
}

int DSSCalculationPlan::getMetricSliceID(int iLevel, int iEvent, int iMetric,
                                         bool *opIsCalculated, int *opSliceID)
{
	int offset = iLevel * mNumEvents * mNumMetrics + iEvent * mNumMetrics + iMetric;
	
	//mvSliceFlag[iLevel][iEvent + 1][iMetric]
	if (opIsCalculated)
		*opIsCalculated = mvSliceFlag[offset];
	
	//mvSliceMappingTable[iLevel][iEvent + 1][iMetric]
	if (opSliceID)
		*opSliceID = mvSliceMappingTable[offset];
	
	return S_OK;
}

int DSSCalculationPlan::getMetricSliceID(int iLevel, GUID &irGUID, bool *opIsCalculated, int *opSliceID)
{
    *opIsCalculated = true;
    int pos = -1;
    for(int i = 0; i < mvMetricID.size(); ++i)
    {
        if(MBase::IsEqualGUID(mvMetricID[i], irGUID))
        {
            pos = i;
            break;
        }
    }
    if(pos != -1)
    {
        for(int i = 0; i < mvComponentMetrics.size(); ++i)
        {
            if(pos == mvComponentMetrics[i])
            {
                *opSliceID = mvNewRWDMappingTable[i + iLevel * mvContainerMetrics.size()];
                return S_OK;
            }
        }
    }
    return E_FAIL;
}

int DSSCalculationPlan::getMetricExtremeSliceID(int iLevel, GUID &irGUID, bool *opIsCalculated, int *opSliceID)
{
    *opIsCalculated = true;
    int pos = -1;
    for(int i = 0; i < mvMetricID.size(); ++i)
    {
        if(MBase::IsEqualGUID(mvMetricID[i], irGUID))
        {
            pos = i;
            break;
        }
    }
    if(pos != -1)
    {
        for(int i = 0; i < mvComponentMetrics.size(); ++i)
        {
            if(pos == mvComponentMetrics[i])
            {
                *opSliceID = mvExtremeMappingTable[i + iLevel * mvContainerMetrics.size()];
                return S_OK;
            }
        }
    }
    return E_FAIL;
}


int DSSCalculationPlan::getMetricThresholdSliceID(int iLevel, GUID &irGUID, int *opSliceID)
{
    int pos = -1;
    for(int i = 0; i < mvMetricID.size(); ++i)
    {
        if(MBase::IsEqualGUID(mvMetricID[i], irGUID))
        {
            pos = i;
            break;
        }
    }
    if(pos != -1)
    {
        for(int i = 0; i < mvComponentMetrics.size(); ++i)
        {
            if(pos == mvComponentMetrics[i])
            {
                *opSliceID = mvNewRWDThreshold[i + iLevel * mvContainerMetrics.size()];
                return S_OK;
            }
        }
    }
    else
    {
        // 03/22/16, to reuse the API for attribute threshold like server.
        return getAttriThresholdSliceID(iLevel, irGUID, opSliceID);
    }
    return E_FAIL;

}

int DSSCalculationPlan::getAttriThresholdSliceID(int iLevel, const GUID &irAttributeID, int *opSliceID, bool* opIsCalculated)
{
    int pos = -1;
    for (int i = 0; i < mvThresholdAttribute.size(); ++i)
    {
        if (MBase::IsEqualGUID(mvThresholdAttribute[i], irAttributeID))
        {
            pos = i;
            break;
        }
    }
    if (pos == -1)
    {
        return E_FAIL;
    }
    return getAttriThresholdSliceID(iLevel, pos, opSliceID, opIsCalculated);
}

int DSSCalculationPlan::getAttriThresholdSliceID(int iLevel, int irAttributeIndex, int *opSliceID, bool* opIsCalculated)
{
    int offset = irAttributeIndex + iLevel * getNumAttriThresholds();
    int hr = E_FAIL;
    if (opSliceID && (offset >= 0) && ((size_t)offset < mvThresholdAttributeSliceID.size()))
    {
        *opSliceID = mvThresholdAttributeSliceID[offset];
        hr = S_OK;
    }
    if (opIsCalculated && (offset >= 0) && ((size_t)offset < mvAttributeSliceFlag.size()))
    {
        *opIsCalculated = mvAttributeSliceFlag[offset];
        hr = S_OK;
    }
    
    return hr;
}

int DSSCalculationPlan::getAllAttriThresholdSliceID(vector<int> &irvAtrriThresholdSliceID)
{
    for (vector<int>::const_iterator iter = mvThresholdAttributeSliceID.begin(); iter != mvThresholdAttributeSliceID.end(); iter++) {
        irvAtrriThresholdSliceID.push_back(*iter);
    }
    
    return S_OK;
}

size_t DSSCalculationPlan::getNumAttriThresholds()
{
    return mvThresholdAttribute.size();
}

GUID DSSCalculationPlan::getAttriID(int iIndex)
{
    if (iIndex < 0 || ((size_t)iIndex >= mvThresholdAttribute.size())) {
        return GUID_NULL;
    }
    return mvThresholdAttribute[iIndex];
}

int DSSCalculationPlan::setMetricSliceID(int iLevel, int iEvent, int iMetric,
                                         bool bIsCalculated, int iSliceID)
{
	int offset = iLevel * mNumEvents * mNumMetrics + iEvent * mNumMetrics + iMetric;
	
	//mvSliceFlag[iLevel][iEvent + 1][iMetric]
	mvSliceFlag[offset] = bIsCalculated;
	
	//mvSliceMappingTable[iLevel][iEvent + 1][iMetric]
	mvSliceMappingTable[offset] = iSliceID;
    
    if(iLevel * mNumMetrics + iMetric < mvNewRWDMappingTable.size())
    {
        //also set the mapping table for New RWD
        mvNewRWDMappingTable[iLevel * mNumMetrics + iMetric] = iSliceID;
    }
	return S_OK;
}
int DSSCalculationPlan::setMetricSliceID2(int iMetric, int iSliceID)
{
    for(int i = 0; i < mvComponentMetrics.size(); i++)
    {
        if(mvComponentMetrics[i] == mvComponentMetrics[iMetric])
        {
            for (int level = 0; level < mNumLevels; level++)
            {
                int offset = level * mNumEvents * mNumMetrics + i;
                mvSliceMappingTable[offset] = iSliceID;
            }
        }
    }
    return S_OK;
}
GUID DSSCalculationPlan::getMetricID(int iMetric)
{
	return mvMetricID[mvComponentMetrics[iMetric]];
}

GUID DSSCalculationPlan::getMetricID2(int iMetric)
{
	return mvMetricID[mvContainerMetrics[iMetric]];
}

int DSSCalculationPlan::getTemplateSubtotal(int iLevel)
{
	return mvLevels[iLevel].mTemplateSubtotal;
}

int DSSCalculationPlan::getMetricIndex(GUID &irMetricID)
{
	int lnMetrics = mvComponentMetrics.size();
	
	for (int i = 0; i < lnMetrics; i++)
	{
		if (MBase::IsEqualGUID(mvMetricID[mvComponentMetrics[i]], irMetricID))
			return i;
	}
	return -1;
}

HRESULT DSSCalculationPlan::get_MetricIndexEx(DSS_ID &irDatasetID, DSS_ID &irMetricID, Int32 *opIndex)
{
    //mDatasetID is not supported yet.
    if (opIndex)
    {
        *opIndex = getMetricIndex(irMetricID);
    }
    return S_OK;
}

int DSSCalculationPlan::get_MetricIndexEx(DSS_ID &irDatasetID, DSS_ID &irMetricID)
{
    //mDatasetID is not supported yet.
    return getMetricIndex(irMetricID);
}


int DSSCalculationPlan::getMetricIndexWithGoverner(GUID &irMetricID, GUID &irGovMetricID)
{
	int lnMetrics = mvComponentMetrics.size();
	
	for (int i = 0; i < lnMetrics; i++)
	{
		if (MBase::IsEqualGUID(mvMetricID[mvComponentMetrics[i]], irMetricID)
			&& MBase::IsEqualGUID(mvMetricID[mvContainerMetrics[i]], irGovMetricID))
			return i;
	}
	return -1;
}

bool DSSCalculationPlan::IsDecomposable(GUID &irMetricID)
{
	int lnMetrics = mvContainerMetrics.size();
	
	for (int i = 0; i < lnMetrics; i++)
	{
		int lMetric = mvContainerMetrics[i];
		if (MBase::IsEqualGUID(mvMetricID[lMetric], irMetricID))
			return mvbIsDecomposable[lMetric];
	}
	return false;
}

int DSSCalculationPlan::MarkUnCalculated(int iMetric)
{
	//TODO: to enhance. only reset the related slices:
	//iMetric's subtotal
	//compound metric depending on iMetric and its subtotal
	setStatus(DssCalcStatusUnCalculated);
	
	int lnEvents = mNumEvents;
	int lnMetrics = mNumMetrics;
	int lnLevels = mNumLevels;
	
	for (int i = 0; i < lnLevels; i++)
	{
		//set all levles at the original event to the original metric slice
		for (int m = 0; m < lnMetrics; m++)
		{
			bool lbIsSmartMetric = mvbIsDecomposable[mvComponentMetrics[m]];
			if (lbIsSmartMetric)
			{
				int offset = i * lnEvents * lnMetrics + m;
				mvSliceFlag[offset] = false;	//set smart metric as un-calculated
			}
		}
		
		for (int e = 1; e < lnEvents; e++)
		{
			for (int j = 0; j < lnMetrics; j++)
			{
				mvSliceFlag[i * lnEvents * lnMetrics + e * lnMetrics + j] = false;
			}
		}
	}
    
    for (std::vector<bool>::iterator it = mvAttributeSliceFlag.begin(); it != mvAttributeSliceFlag.end(); it++)
    {
        *it = false;
    }
    
	return S_OK;
}

int DSSCalculationPlan::FinalizeEvent(DSSCube *ipCube)
{
	if (mNumEvents == 1)
		return S_OK;
	
	int lLastEvent = mNumEvents - 1;
	
	int hr = S_OK;
	
	vector<int> lvOriginalID;
	vector<int> lvUpdatedID;
	for (int i = 0; i < mNumLevels; i++)
	{
		for (int m = 0; m < mNumMetrics; m++)
		{
			int *lpOriginalEvenSlicedID = &mvPreviousSlices[i * mNumMetrics + m];
			int *lpFinalEvenSlicedID = &mvSliceMappingTable[i * mNumEvents * mNumMetrics + lLastEvent * mNumMetrics + m];
			
			bool lbIsCalculated = mvSliceFlag[i * mNumEvents * mNumMetrics + lLastEvent * mNumMetrics + m];
			
			if (!lbIsCalculated && *lpOriginalEvenSlicedID >= 0)	//473388, the original slice may not exist
			{
				//467977, somehow this slice is not generated, replace with a dummy slice
				//468752, don't share the dummy slices, otherwise, ipCube->UpdateCalculateResult() can't do it right.
                
				int lDummySliceID = -1;
				double lVal = 0;
				hr = ipCube->getConstantSlice(0, 0, (char*)&lVal, sizeof(double), DssDataTypeDouble, DssDataUnCalculated, &lDummySliceID);
				CHECK_HR;
                
				*lpFinalEvenSlicedID = lDummySliceID;
               // *lpFinalEvenSlicedID = *lpOriginalEvenSlicedID;
				mvSliceFlag[i * mNumEvents * mNumMetrics + lLastEvent * mNumMetrics + m] = true;
			}
			
			if (*lpOriginalEvenSlicedID != *lpFinalEvenSlicedID && *lpOriginalEvenSlicedID >= 0 && *lpFinalEvenSlicedID >= 0)
			{
				//need update this slice in the cube
				lvOriginalID.push_back(*lpOriginalEvenSlicedID);
				lvUpdatedID.push_back(*lpFinalEvenSlicedID);
			}
            
		}
	}
	
	hr = ipCube->UpdateCalculateResult(lvOriginalID, lvUpdatedID);
	CHECK_HR;
	
	//update slice ID in calculation plan
    if(mvNewRWDMappingTable.size() > 0)
    {
        for (int i = 0; i < mNumLevels; i++)
        {
            for (int m = 0; m < mNumMetrics; m++)
            {
                int *lpOriginalEvenSlicedID = &mvPreviousSlices[i * mNumMetrics + m];
                int *lpFinalEvenSlicedID = &mvSliceMappingTable[i * mNumEvents * mNumMetrics + lLastEvent * mNumMetrics + m];
                
                bool lbIsCalculated = mvSliceFlag[i * mNumEvents * mNumMetrics + lLastEvent * mNumMetrics + m];
                
                if (*lpOriginalEvenSlicedID != *lpFinalEvenSlicedID && lbIsCalculated)
                {
                    *lpFinalEvenSlicedID = *lpOriginalEvenSlicedID;
                    mvNewRWDMappingTable[i * mNumMetrics + m] = *lpOriginalEvenSlicedID;
                }
            }
        }
	}else{
        //931 old rwd i-server, compatiblity
        //update slice ID in calculation plan
        for (int i = 0; i < mNumLevels; i++)
        {
            for (int m = 0; m < mNumMetrics; m++)
            {
                int *lpOriginalEvenSlicedID = &mvPreviousSlices[i * mNumMetrics + m];
                int *lpFinalEvenSlicedID = &mvSliceMappingTable[i * mNumEvents * mNumMetrics + lLastEvent * mNumMetrics + m];
                
                bool lbIsCalculated = mvSliceFlag[i * mNumEvents * mNumMetrics + lLastEvent * mNumMetrics + m];
                
                if (*lpOriginalEvenSlicedID != *lpFinalEvenSlicedID && lbIsCalculated)
                {
                    *lpFinalEvenSlicedID = *lpOriginalEvenSlicedID;	
                }
            }
        }
    }
	setStatus(DssCalcStatusCalculated);
	
	return S_OK;
}

int DSSCalculationPlan::AddEvent(DSSCalculationEvent &irEvent)
{
	irEvent.mEventIndex = mvCalculationEvents.size();
	
    //revert to 1-based
//	if (irEvent.mEventType == DssEventTemplateSubtotal)
//		irEvent.mEventTag--;			//convert to 0-based
    
	mvCalculationEvents.push_back(irEvent);
	return S_OK;
}

int DSSCalculationPlan::AddMetrics(vector<GUID> &irvMetricID, vector<int> &irvMetricList1, vector<int> &irvMetricList2, DSSViewDataSet *ipViewDataSet)
{
	//we need re-order the metric list according to the metric index in TabularMetrics. For example:
	//irvMetricID = {Cost}, {Revenue}, {Profit}			irvMetricID = {Profit}, {Cost}, {Revenue}
	//irvMetricList1 = {0, 1, 0, 1, 2}			==>		irvMetricList1 = {1, 2, 1, 2, 0}
	//irvMetricList2 = {0, 1, 2, 2, 2}					irvMetricList2 = {1, 2, 0, 0, 0}
	int hr = S_OK;
    
	DSSTabularMetrics *lpMetrics = NULL;
    if(ipViewDataSet)
    {
        DSSTabularData *lpTabularData = NULL;
        lpTabularData = ipViewDataSet->getTabularData();
        lpMetrics = lpTabularData->getTabularMetircs();
    }
    else
    {
        hr = mpDataModel->getMetricsUnit(&lpMetrics);
        CHECK_HR;
	}
	int lnMetrics = irvMetricID.size();
	vector<int> lvMetricIndex(lnMetrics);
    
	int count = 0;
	int i = 0;
	for (i = 0; i < lnMetrics; i++)
	{
		int index = lpMetrics->FindMetric(irvMetricID[i]);
		if (index >= 0)
		{
			lvMetricIndex[i] = index;
			count++;
		}
		else
		{
			lvMetricIndex[i] = -1;
		}
	}
	
	if (count == lnMetrics)	//we are about to release 921, so do not touch the existing code
	{
        mvMetricID.resize(lnMetrics);
        for (i = 0; i < lnMetrics; i++)
        {
            mvMetricID[lvMetricIndex[i]] = irvMetricID[i];
        }

        lnMetrics = irvMetricList1.size();
        mvComponentMetrics.resize(lnMetrics);
        mvContainerMetrics.resize(lnMetrics);
        for (i = 0; i < lnMetrics; i++)
        {
            mvComponentMetrics[i] = lvMetricIndex[irvMetricList1[i]];
            mvContainerMetrics[i] = lvMetricIndex[irvMetricList2[i]];
        }
	}
	else
	{	//477344, there may be un-displayed componet metrics
		lnMetrics = lpMetrics->Count();
		if (count != lnMetrics)	//all XTabMetrics should be matched
			return E_FAIL;
		
		mvMetricID.resize(lnMetrics);
		for (i = 0; i < lnMetrics; i++)
		{
			hr = lpMetrics->getMetricID(i, mvMetricID[i]);
			CHECK_HR;
		}
		
		mvComponentMetrics.reserve(lnMetrics);
		mvContainerMetrics.reserve(lnMetrics);
		
		lnMetrics = irvMetricList1.size();
		for (i = 0; i < lnMetrics; i++)
		{
			int index1 = lvMetricIndex[irvMetricList1[i]];
			int index2 = lvMetricIndex[irvMetricList2[i]];
			if (index1 >= 0 && index2 >= 0)
			{
				mvComponentMetrics.push_back(index1);
				mvContainerMetrics.push_back(index2);
			}
		}
	}
	return S_OK;
}

int DSSCalculationPlan::AddMetrics2(vector<GUID> &irvMetricID, vector<int> &irvMetricList1, vector<int> &irvMetricList2)
{
    for(int i = 0; i < irvMetricID.size(); ++i)
    {
        mvMetricID.push_back(irvMetricID[i]);
    }
    
    for(int i = 0; i < irvMetricList1.size(); ++i)
    {
        mvComponentMetrics.push_back(irvMetricList1[i]);
        mvContainerMetrics.push_back(irvMetricList2[i]);
    }
    return S_OK;
}

int DSSCalculationPlan::getMetrics2(vector<GUID> &irvMetricID, vector<int> &irvMetricList1, vector<int> &irvMetricList2)
{
    for(int i = 0; i < mvMetricID.size(); ++i)
    {
        irvMetricID.push_back(mvMetricID[i]);
    }
    
    for(int i = 0; i < mvComponentMetrics.size(); ++i)
    {
        irvMetricList1.push_back(mvComponentMetrics[i]);
        irvMetricList2.push_back(mvContainerMetrics[i]);
    }
    return S_OK;
}

int DSSCalculationPlan::AddLevel(const vector<int> &irvLevelUnits, int ipTemplateSubTotal)
{
    LevelInfo lLevelInfo;
    lLevelInfo.mvUnits.assign(irvLevelUnits.begin(), irvLevelUnits.end());
    lLevelInfo.mTemplateSubtotal = ipTemplateSubTotal;
    mvLevels.push_back(lLevelInfo);
    mNumLevels++;
	return S_OK;
}

int DSSCalculationPlan::getLevelInfo(int iLevel, vector<int> &ivrUnits, int &irTempateSubtotal)
{
    if(iLevel < 0 || iLevel >= mvLevels.size())
    {
        return E_FAIL;
    }
    ivrUnits.assign(mvLevels[iLevel].mvUnits.begin(), mvLevels[iLevel].mvUnits.end());
    irTempateSubtotal = mvLevels[iLevel].mTemplateSubtotal;
    return S_OK;
}

int DSSCalculationPlan::setMappingTable(vector<int> &irMappingTable)
{
    mvNewRWDMappingTable = irMappingTable;
    return S_OK;
}

int DSSCalculationPlan::setThreshold(vector<int> &irThreshold)
{
    mvNewRWDThreshold = irThreshold;
    return S_OK;
}

int DSSCalculationPlan::setAttriThresholdIDs(const vector<GUID>& irAttriIDs)
{
    mvThresholdAttribute = irAttriIDs;
    return S_OK;
}

int DSSCalculationPlan::setAttriThresholdSliceIDs(const vector<int> &irSliceIDs)
{
    mvThresholdAttributeSliceID = irSliceIDs;
    mvAttributeSliceFlag.clear();
    for (std::vector<int>::const_iterator it = irSliceIDs.begin(); it != irSliceIDs.end(); it++)
    {
        mvAttributeSliceFlag.push_back(true);
    }
    return S_OK;
}

int DSSCalculationPlan::setAttriThresholdSliceID(int iLevel, const GUID &irAttributeID, int iSliceID)
{
    int pos = -1;
    for (int i = 0; i < mvThresholdAttribute.size(); ++i)
    {
        if (MBase::IsEqualGUID(mvThresholdAttribute[i], irAttributeID))
        {
            pos = i;
            break;
        }
    }
    if (pos == -1)
    {
        return E_FAIL;
    }
    int lOffset = pos + iLevel * getNumAttriThresholds();
    if ((lOffset < 0) || (lOffset >= mvThresholdAttributeSliceID.size()))
        return S_FALSE;
    mvThresholdAttributeSliceID[lOffset] = iSliceID;
    mvAttributeSliceFlag[lOffset] = true;
    return S_OK;
}

int DSSCalculationPlan::hSelectDistinvtComponentMetricSlice(vector<int> &orSliceID)
{
    int lnMetrics = mvMetricID.size();
    orSliceID.resize(lnMetrics);
    
    int m = 0;
    for (m = 0; m < lnMetrics; m++)
    {
        orSliceID[m] = -1;
    }

    int lnComponents = mvComponentMetrics.size();
    int count = 0;
    for (m = 0; m < lnComponents; m++)
    {
        int lMetricIndex = mvComponentMetrics[m];
        
        //component metric in last event could be -1, un-calculated. In that case, this metric is all NULLs.
        if (orSliceID[lMetricIndex] < 0 && mvNewRWDMappingTable[m] >= 0)
        {
            orSliceID[lMetricIndex] = mvNewRWDMappingTable[m];
            count++;
            
            if (count >= lnMetrics)
                break;
        }
    }
    
    return S_OK;
}

int DSSCalculationPlan::hNewInitMappingTable(bool isBaseDataset)
{
	int lnEvents = mvCalculationEvents.size();
	
	int lnMetrics = mvComponentMetrics.size();
	
	int lnLevels = mvLevels.size();
    
    _ASSERT(mvNewRWDMappingTable.size() == lnLevels * lnMetrics);
	
	mvSliceMappingTable.resize(lnLevels * lnEvents * lnMetrics);		//mvSliceMappingTable[iLevel][iEvent][iMetric]
	mvSliceFlag.resize(lnLevels * lnEvents * lnMetrics);				//mvSliceFlag[iLevel][iEvent][iMetric]
	mvPreviousSlices.resize(lnLevels * lnMetrics);
	
    vector<int> lvComponentMetricSliceID;
    hSelectDistinvtComponentMetricSlice(lvComponentMetricSliceID);
    
	for (int i = 0; i < lnLevels; i++)
	{
		//set all levles at the original event to the original metric slice
        // if base dataset, set all levles at the last event to the original metric slice
		for (int m = 0; m < lnMetrics; m++)
		{
			int lMetricIndex = mvComponentMetrics[m];
			
//			int lSliceID = lpXTabView->getCubeSliceID(0, lMetricIndex);
			int lSliceID = lvComponentMetricSliceID[lMetricIndex];     //view level slice ID
            
			//bool lbIsCalculated = isBaseDataset ? true : !mvbIsDecomposable[mvComponentMetrics[m]];	//set smart metric as un-calculated, for base dataset, always calculated
            //tliao, 12/18/2013, 824357, the slice of base dataset can be uncalculated
			bool lbIsCalculated = false;
            if (isBaseDataset)
            {
                if (lSliceID >= 0)
                    lbIsCalculated = true;
            }
            else
            {
                lbIsCalculated = !mvbIsDecomposable[mvComponentMetrics[m]];
            }
            
			if (mvLevels[i].mTemplateSubtotal < 0 && i > 0)
			{
				//473388, this level is the cross of two different subtotals, set as invalid (no input)
				lSliceID = -1;
				lbIsCalculated = false;
			}
			
			int offset = i * lnEvents * lnMetrics + m;
            if (isBaseDataset)
            {
                offset += (lnEvents - 1) * lnMetrics;
            }
			
			mvSliceMappingTable	[offset] = lSliceID;
			mvSliceFlag			[offset] = lbIsCalculated;
            
			lSliceID = mvNewRWDMappingTable[m + i * lnMetrics];         //current (totoal) level slice ID
			mvPreviousSlices[i * lnMetrics + m] = lSliceID;
		}
		
        
		//all others are not calculated
		int start = isBaseDataset ? 0 : 1;
        int end = isBaseDataset ? lnEvents - 1 : lnEvents;
        
        for (int e = start; e < end; e++)
		{
			int *lpSlice = &mvSliceMappingTable[i * lnEvents * lnMetrics + e * lnMetrics];
			for (int j = 0; j < lnMetrics; j++)
			{
				lpSlice[j] = -1;
				mvSliceFlag[i * lnEvents * lnMetrics + e * lnMetrics + j] = false;
			}
		}
	}
	
	mNumLevels = lnLevels;
	mNumEvents = lnEvents;
	mNumMetrics = lnMetrics;
	
	return S_OK;
}

int DSSCalculationPlan::setThresholdSliceID(int iLevel, int iMetric, int iSliceID)
{
    // Jun/19/2013, TQMS 759322, setThresholdSliceID() is only for new RWD, so just return S_OK in case of old RWD.
    if (mpDataModel)
    {
        // only old RWD logic will reach here.
        return S_OK;
    }
    
    if(iMetric >= mNumMetrics || iLevel >= mNumLevels)
        return S_FALSE;
    int lOffset = iMetric + iLevel * mvContainerMetrics.size();
    if(lOffset >= mvNewRWDThreshold.size())
        return S_FALSE;
    mvNewRWDThreshold[lOffset] = iSliceID;
    return S_OK;
}
int DSSCalculationPlan::get_MetricIDEx (Int32 iMetric, GUID *opMetric)
{
	if(!opMetric) return E_POINTER;
	if(iMetric >= mvMetricID.size()) return E_FAIL;
    
    *opMetric = mvMetricID[iMetric];

	return S_OK;
}

int DSSCalculationPlan::getMaxCalculatedSliceID()
{
    int max = -1;
    for (int i = 0; i < mvSliceFlag.size(); i++)
    {
        if (mvSliceFlag[i] == true)
        {
            if (max < mvSliceMappingTable[i])
            {
                max = mvSliceMappingTable[i];
            }
        }
    }
    return max;
}

int DSSCalculationPlan::getMaxUsefulSliceID()
{
    int max = -1;
    for (int i = 0; i < mvNewRWDMappingTable.size(); i++)
    {
        if (mvNewRWDMappingTable[i] > max)
        {
            max = mvNewRWDMappingTable[i];
        }
    }
    for (int i = 0; i < mvNewRWDThreshold.size(); i++)
    {
        if (mvNewRWDThreshold[i] > max)
        {
            max = mvNewRWDThreshold[i];
        }
    }
    for (int i = 0; i < mvThresholdAttributeSliceID.size(); i++)
    {
        if (mvThresholdAttributeSliceID[i] > max)
        {
            max = mvThresholdAttributeSliceID[i];
        }
    }

    return max;
}

int DSSCalculationPlan::InitMetricListEx(int iSize, GUID *ipMetricIDs, int *ipSliceIDs)
{
    DSSCalculationEvent lEvent;
	lEvent.mEventType = DssEventOriginalMetric;
	lEvent.mID = GUID_NULL;
	lEvent.mEventTag = -1;
	lEvent.mEventIndex = mvCalculationEvents.size();
	mvCalculationEvents.push_back(lEvent);
    
    vector<GUID> lvMetricID;
    vector<int> lvMetricList1;
    vector<int> lvMetricList2;
    vector<int> lvMappingTable;
    
    int iMetric = 0;
    for (; iMetric < iSize; iMetric++)
    {
        lvMetricID.push_back(ipMetricIDs[iMetric]);
        lvMetricList1.push_back(iMetric);
        lvMetricList2.push_back(iMetric);
        lvMappingTable.push_back(ipSliceIDs[iMetric]);
        mvSliceMappingTable.push_back(ipSliceIDs[iMetric]);
        mvSliceFlag.push_back(true);

    }
    
    AddMetrics2(lvMetricID, lvMetricList1, lvMetricList2);
    setMappingTable(lvMappingTable);
    vector<int> lvlevel;
    lvlevel.push_back(0);
    AddLevel(lvlevel, -1);

    return S_OK;
}

int DSSCalculationPlan::setExtremInfo(vector<EnumDSSRuntimeSubtotalType> ivExtremType,
                                      vector<int> ivExtremeMappingTable,
                                      vector<int> ivExtremeInfoMappingTable,
                                      vector<SerializedExtremeInfo> ivExtremeInfo)
{
    mvExtremType = ivExtremType;
    mvExtremeMappingTable = ivExtremeMappingTable;
    mvExtremeInfoMappingTable = ivExtremeInfoMappingTable;
    mvExtremeInfo = ivExtremeInfo;
    return S_OK;
}

int DSSCalculationPlan::getExtremeType(int iLevel, EnumDSSRuntimeSubtotalType *ipExtremeType)
{
    if(!ipExtremeType)
        return E_POINTER;
    if(iLevel >= mvExtremType.size())
    {
        *ipExtremeType = DssRuntimeSubtotalDefault;
        return S_OK;
    }
    *ipExtremeType = mvExtremType[iLevel];
    return S_OK;
}

int DSSCalculationPlan::getModelInfo(int iLevel, int &lModelIndex, int &lTRMetricID, int &lnXVariables, int *&lpXVariables, EnumDSSObjectType *&lpXVariableTypes, int *&lpXIndexMetric)
{
    if(iLevel >= mvExtremType.size())
        return E_INVALIDARG;
    
    lModelIndex = mvExtremeInfo[iLevel].mModelIndex;
    lTRMetricID = mvExtremeInfo[iLevel].mTRMetricID;
    lnXVariables = mvExtremeInfo[iLevel].mnXVariable;
    
    lpXVariables = mvExtremeInfo[iLevel].mvXVariables.size() > 0 ? &(mvExtremeInfo[iLevel].mvXVariables[0]) : NULL;
    lpXVariableTypes = mvExtremeInfo[iLevel].mvXVariableTypes.size() > 0 ? &(mvExtremeInfo[iLevel].mvXVariableTypes[0]) : NULL;
    lpXIndexMetric = mvExtremeInfo[iLevel].mvXIndexMetrics.size()>0 ? &(mvExtremeInfo[iLevel].mvXIndexMetrics[0]) : NULL;
    
    return S_OK;
}

int DSSCalculationPlan::getAllExtremeSliceID(vector<int> &irvExtremeSliceID)
{
    for(int i = 0; i < mvExtremeMappingTable.size(); ++i)
        if(mvExtremeMappingTable[i] != -1)
            irvExtremeSliceID.push_back(mvExtremeMappingTable[i]);
    return S_OK;
}
