//
//  DSSTabularData.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 11/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "DSSTabularData.h"

#include "DSSrwdconstants.h"
#include "DSSGUnitContainer.h"

#include "DSSTabularConsolidation.h"
#include "DSSTemplate.h"
#import "DSSXTabView.h"
#include "DSSViewDataSetHelper.h"
#include "DSSTabularMetrics.h"
#include "DSSTabularRefCustomGroup.h"
#include "MsiCOM/MsiCOM/Msi_atlbase.h"
#include "DSSXTabRefLookupTable.h"

#include "DSSDataSource.h"
#include "DSSTabularRefConsolidation.h"
#include "DSSMaskInfos.h"
#include "DSSFunctionServer.h"
#include "DSSSimpleExprEval.h"
#include "DSSCalculationHelper.h"
#include "SimpleBuffer.h"
#include "DSSCube.h"
#include "DSSViewDataSet.h"
#include "DSSCubeDataSlice.h"
#include "DSSRWDataModel.h"
#include "DSSSliceCorrelator.h"
#import "DSSStrongPtr.h"
#include "SimpleExpr/DFCSimpleExpr.h"
#include "DSSXTabRecursiveLookupTable.h"
#include <algorithm>
#include "statistic.h"
#import "DSSSort_includes.h"

// 5/30/2012 tehe: initialize ref member
DSSTabularData::DSSTabularData():mDummyUnitIndex(-1)
,mbIsNewRWD(true), mpDataModelBase(NULL), mpMetrics(NULL),mpIndexPool(NULL),mpObjectContext(NULL),mpXTabView(NULL),mpGlobalLookupContainer(NULL),mDataSetGUID(GUID_NULL),mpBuffer(NULL)
{
}

DSSTabularData::DSSTabularData(GUID iDataSetGUID):mDummyUnitIndex(-1)
,mbIsNewRWD(true), mpDataModelBase(NULL), mpMetrics(NULL),mpIndexPool(NULL),mpObjectContext(NULL),mpXTabView(NULL),mpGlobalLookupContainer(NULL), mpBuffer(NULL)
{
    mDataSetGUID = iDataSetGUID;
}


DSSTabularData::~DSSTabularData()
{
    // tehe: we need delete the ref unit for new rwd. here metrics is created on demand, so we will delete units first to avoid memory corrupt
    if(mbIsNewRWD)
    {
        for (int i=0; i<mvUnits.size(); i++)
        {
            if(mvUnits[i] && mvUnits[i]->getUnitType() != DssTemplateMetrics)
            {
                delete mvUnits[i];
                mvUnits[i] = NULL;
            }
        }
        //no need to delete mvRecursiveUnits, as it will be pushed into mvUnits.
        /*
        for (int i=0; i<mvRecursiveUnits.size(); i++)
        {
            if(mvRecursiveUnits[i])
            {
                delete mvRecursiveUnits[i];
                mvRecursiveUnits[i] = NULL;
            }
        }
        */
        if(mpMetrics)
        {
            delete mpMetrics;
            mpMetrics = NULL;
        }
        
        if (mpBuffer)
        {
            delete mpBuffer;
            mpBuffer = NULL;
        }
    }
}

int DSSTabularData::init(DSSGUnitContainer *ipGlobalLookupContainer)
{
    int hr = S_OK;
    
    //zhyang, FG workflow. it has already been initialized
    if(mvUnits.size() != 0)
        return hr;
    
    if (!mpMetrics)
    {
        mpMetrics = new DSSTabularMetrics();
    }
    
    mpGlobalLookupContainer = ipGlobalLookupContainer;
    int lnUnitSize = mnUnits;// Count();
    int lMetric = 0;
    for(int i = 0; i < lnUnitSize; ++i)
    {
        if(i == mMetircPos)
        {
            mvUnits.push_back(mpMetrics);
            lMetric = 1;
        }
        else
        {
            DSSTabularUnit *lpTabularUnit = NULL;
            if(mvUnitGlobalIndex[i] == -1)
            {
                MBase::CString lGUIDStr = DSSAEHelper::Guid2String(mvUnitGUID[i]);
                hr = hCreateDummyUnit(i, mvUnitType[i], lGUIDStr, &lpTabularUnit);
                CHECK_HR;
            }
            else if (mvUnitGlobalIndex[i] >= 0 || mvUnitType[i] == DssTemplateAttribute)
            {
                hr = mpGlobalLookupContainer->getUnitsForViewDataSet(&mvUnitGUID[i], &mvUnitType[i], mvUnitGlobalIndex[i], &lpTabularUnit);
                CHECK_HR;
            }
            else
            {
                MBase::CString lGUIDStr = DSSAEHelper::Guid2String(mvUnitGUID[i]);
                hr = hCreateDummyUnit(i, mvUnitType[i], lGUIDStr, &lpTabularUnit);
                CHECK_HR;
            }
            lpTabularUnit->setUnitIndex(mvUnits.size());
            mvUnits.push_back(lpTabularUnit);
            
            
        }
    }
    
    //ctang: set the CG's ordinal
    for (int i = 0; i < mvCGPos.size(); i++)
    {
        AE_ASSERT(mvUnitType[mvCGPos[i]] == DssTemplateCustomGroup);
        DSSTabularRefCustomGroup* lpRefCG = static_cast<DSSTabularRefCustomGroup*>(mvUnits[mvCGPos[i]]);
        lpRefCG->SetOrdinal(mvCGOrdinals[i]);
        //CHECK_HR; //zhyang, CG in extended view don't need ordinal so don't check hr here
    }
    
    for (int i = 0; i < mvAttributePos.size(); i++)
    {
        AE_ASSERT(mvUnitType[mvAttributePos[i]] == DssTemplateAttribute);
        DSSXTabRefLookupTable* lpRefLookup = static_cast<DSSXTabRefLookupTable*>(mvUnits[mvAttributePos[i]]);
        hr = lpRefLookup->SetSelectedForms(mvIsAllFormSelect[i], mvSelectedForms[i], mvSelectedLocales[i]);
        CHECK_HR;
    }

    //msun: to put the recursive units back to mvUnits;
    for (int i = 0; i < mvRecursiveUnits.size(); i++) {
        DSSTabularUnit* lRU = mvRecursiveUnits[i];
        int lRUIndex = lRU->getUnitIndex();
        delete mvUnits[lRUIndex];
        mvUnits[lRUIndex] = lRU;
    }
    return S_OK;
}

int DSSTabularData::getDummyUnitIndex(int *pUnitIndex)
{
    *pUnitIndex = mDummyUnitIndex;
    return S_OK;
}

int DSSTabularData::Count(int *pCount)
{
    *pCount = mvUnits.size();
    return S_OK;
}

int DSSTabularData::Count()
{
    return mvUnits.size();
}

int DSSTabularData::getMetrics(DSSTabularMetrics **oppTabularMetrics)
{
    *oppTabularMetrics = mpMetrics;
    return S_OK;
}

int DSSTabularData::get_MetricsC(DSSTabularMetrics **oppTabularMetrics)
{
    *oppTabularMetrics = mpMetrics;
    return S_OK;
}

int DSSTabularData::FindUnitI(const GUID &irGUID, EnumDSSTemplateUnitType iType, int &iPosition, bool iCheckRecursive)
{
    iPosition = -1;
    
    //for recursive consolidation (de), the unit in tabulardata is type of dsstemplateattribute.
    if (iCheckRecursive && mvRecursiveUnits.size()>0 && iType == DssTemplateConsolidation) {
        iType = DssTemplateAttribute;
    }
    
    if (iType == DssTemplateMetrics)
    {
        iPosition = mMetircPos;
        return iPosition != -1 ? S_OK : E_FAIL;
    }
    
    // tehe 12/21/2011 if the unit type is consolidation, we need doing some special handling.
    int lCandidate = -1;
    
    for(int i = 0; i < mvUnitGUID.size(); ++i)
    {
        if(ATL::InlineIsEqualGUID(mvUnitGUID[i], irGUID))
        {
            // iPosition = i;
            // break;
            if(iType == DssTemplateReserved)
            {
                if(mvUnitType[i] != DssTemplateCustomGroup)
                {
                    iPosition = i;
                    return S_OK;
                }
                else
                {
                    lCandidate = i;
                }
            }
            else if(mvUnitType[i] == iType)
            {
                if(!iCheckRecursive)
                {
                    iPosition = i;
                    return S_OK;
                }
                //temp for financial report, to get the recursive unit instead of the data unit,
                //here we assume the recursive unit is after corresponding data unit
                else
                {
                    lCandidate = i;
                }
            }
        }
    }
    
    if(lCandidate != -1)
    {
        iPosition = lCandidate;
        return S_OK;
    }
    
    return S_FALSE;
    
}

//xiazhou, 08/12/2014. An extension of FindUnit. If irGUID is not in the tabular data,
//then try to find Linked attribute of irGUID instead.
int DSSTabularData::FindLinkedUnit(const GUID &irGUID, EnumDSSTemplateUnitType iType)
{
//  Firstly, find irGUID.
    int lUnitPos = FindUnit(irGUID, iType);
    if(lUnitPos != -1)//exist
        return lUnitPos;
    
//  Then, try to find linked attribute of irGUID from tabular data.
    if(iType == DssTemplateMetrics || !mpGlobalLookupContainer)
        return -1;
    
    vector<GUID>* lvpLinkedAttr = mpGlobalLookupContainer->getLinkedAttributes(irGUID);
    if(lvpLinkedAttr)
    {
        for(int i = 0; i < lvpLinkedAttr->size(); i++)
        {
            lUnitPos = FindUnit(lvpLinkedAttr->at(i), iType);
            if(lUnitPos != -1)
                return lUnitPos;
        }
    }
    
    return -1;
}

int DSSTabularData::FindUnit(const GUID &irGUID, EnumDSSTemplateUnitType iType, bool iCheckRecursive)
{
    int lIndex = -1;
    FindUnitI(irGUID, iType, lIndex, iCheckRecursive);
    return lIndex;
}

int DSSTabularData::FindUnitI(const GUID &irGUID, EnumDSSTemplateUnitType iType, int *opIndex)
{
    return opIndex ? FindUnitI(irGUID, iType, *opIndex) : E_INVALIDARG;
}


int DSSTabularData::Item(int iUnitIndex, DSSTabularUnit **oppTabularUnit)
{
    if(iUnitIndex < 0 || iUnitIndex >= mvUnits.size())
        return E_FAIL;
    *oppTabularUnit = mvUnits[iUnitIndex];
    return S_OK;
}

DSSTabularUnit* DSSTabularData::Item(int iUnitIndex)
{
    DSSTabularUnit* lpTabularUnit;
    int hr = Item(iUnitIndex, &lpTabularUnit);
    if (hr != S_OK)
    {
        return NULL;
    }
    return lpTabularUnit;
}

int DSSTabularData::ItemC(int iUnitIndex, DSSTabularUnit **oppTabularUnit)
{
    if(iUnitIndex < 0 || iUnitIndex >= mvUnits.size())
        return E_FAIL;
    *oppTabularUnit = mvUnits[iUnitIndex];
    return S_OK;
}

int DSSTabularData::FindElement(DSSBaseElementProxy *ipElement, int &orUnit, int &orKey)
{
    int hr = S_OK;
    GUID lUnitGUID = ipElement->getObjectID();
	//int lUnitIndex = FindUnit(lUnitGUID, DssTemplateReserved);
    int lUnitIndex = -1;
    hr = FindUnitI(lUnitGUID, DssTemplateReserved, lUnitIndex);
    CHECK_HR;
    
	if (lUnitIndex < 0)
		return E_FAIL;		//not a supported element
	
	DSSTabularUnit *lpUnit = Item(lUnitIndex);
	CHECK_PTR(lpUnit);
	
	orUnit = lUnitIndex;
	hr = lpUnit->FindElement(ipElement, orKey);
    
	if (hr == S_FALSE)  // subtotal element
	{	
		if(ipElement->getElementType() == DssElementSubtotal)
		{
			orKey = - static_cast<DSSSubtotalElementProxy*> (ipElement)->getSubtotalIndex();
			return S_OK;
		}
		return S_FALSE;
	}
	else
		return hr;
}

int DSSTabularData::loadRecursiveUnits(DSSBinaryDataReader * ipReadStream)
{
    int hr = S_OK;
//    mvRecursiveUnits
  
    int lnRUnits = 0;
    hr = ipReadStream->ReadInt(&lnRUnits);
    CHECK_HR;
    
    for (int i=0; i<lnRUnits; i++) {
        DSSXTabRecursiveLookupTable* lpRU = new DSSXTabRecursiveLookupTable();
        hr = lpRU->loadFromBinary(ipReadStream);
        CHECK_HR;
        lpRU->setTabularData(this);
        mvRecursiveUnits.push_back(lpRU);
    }
    return hr;
}

int DSSTabularData::loadFromBinary(DSSBinaryDataReader * ipReadStream)
{
    int hr = S_OK;
    
    int lBlockType = 0;
    hr = ipReadStream->BeginReadBlock(&lBlockType);
    CHECK_HR_BLOCK(EnumNewBlockDSSID);
    
    hr = ipReadStream->ReadInt(&mnUnits);
    CHECK_HR;
    
    for(int i = 0; i < mnUnits; ++i)
    {
        int lType = -1;
        int lTmpLocale = 0;
        hr = ipReadStream->ReadInt(&lType);
        CHECK_HR;
        
        EnumDSSTemplateUnitType lUnitType = (EnumDSSTemplateUnitType)lType;
        
        switch (lUnitType) {
            case DssTemplateAttribute:
            case DssTemplateCustomGroup:
            case DssTemplateConsolidation:
            {
                mvUnitType.push_back(lUnitType);
                MBase::CString lDSSID;
                hr = ipReadStream->ReadString(lDSSID);
                CHECK_HR;
                
                GUID lGUID;
                hr = DSSViewDataSetHelper::str2GUID(lDSSID, lGUID);
                CHECK_HR;
                
                mvUnitGUID.push_back(lGUID);
                
                int lIndex = -1;
                hr = ipReadStream->ReadInt(&lIndex);
                CHECK_HR;
                
                mvUnitGlobalIndex.push_back(lIndex);
                
                int lnForms;
                hr = ipReadStream->ReadInt(&lnForms);
                mvForms.push_back(lnForms);
                
                if(-1 == lIndex && ATL::InlineIsGUIDNULL(lGUID))
                {
                    mDummyUnitIndex = i;
                    continue;
                }
                
                if (lUnitType == DssTemplateAttribute && lIndex > -1)
                {
                    mvAttributePos.push_back(mvUnitGUID.size() - 1);
                    bool lfFull;
                    hr = ipReadStream->ReadBool(&lfFull);
                    CHECK_HR;
                    
                    mvIsAllFormSelect.push_back(lfFull);
                    vector<int> lvForms;
                    vector<int> lvLocales;
                    if (!lfFull)
                    {
                        int lTmp;
                        hr = ipReadStream->ReadInt(&lTmp);
                        lvForms.resize(lTmp);
                        
                        for (int j = 0; j < lTmp; j++)
                        {
                            hr = ipReadStream->ReadInt(&lvForms[j]);
                            CHECK_HR;
                        }
                        
                        hr = ipReadStream->ReadInt(&lTmp);
                        lvForms.resize(lTmp);
                        
                        AE_ASSERT(lTmp == lvForms.size());
                        lvLocales.resize(lTmp);
                        for (int j = 0; j < lTmp; j++)
                        {
                            hr = ipReadStream->ReadInt(&lvLocales[j]);
                            if (lvLocales[j] != 0)
                                lTmpLocale = lvLocales[j];
                            CHECK_HR;
                        }
                    }
                    mvSelectedForms.push_back(lvForms);
                    mvSelectedLocales.push_back(lvLocales);
                }
                
                if (lUnitType == DssTemplateCustomGroup && lIndex > -1)
                {
                    mvCGPos.push_back(mvUnitGUID.size() - 1);
                    
                    vector<int> lvOrdinal;
                    int lnCG;
                    hr = ipReadStream->ReadInt(&lnCG);
                    CHECK_HR;
                    
                    lvOrdinal.resize(lnCG);
                    for (int j = 0; j < lnCG; j++)
                    {
                        hr = ipReadStream->ReadInt(&lvOrdinal[j]);
                        CHECK_HR;
                    }
                    mvCGOrdinals.push_back(lvOrdinal);
                }
            }
                break;
            case DssTemplateMetrics:
            {
                hr = ipReadStream->ReadInt(&mMetircPos);
                CHECK_HR;
                
                int lnMetrics = -1;
                hr = ipReadStream->ReadInt(&lnMetrics);
                CHECK_HR;

                if (!mpMetrics)
                {
                    mpMetrics = new DSSTabularMetrics();
                }
                
                mpMetrics->Init(mMetircPos, lnMetrics);
                
                mvUnitType.push_back(DssTemplateMetrics);
                mvUnitGUID.push_back(GUID_NULL);
                mvUnitGlobalIndex.push_back(-1);
                mvForms.push_back(-1);
                
                for (int j = 0; j < lnMetrics; ++j)
                {
                    MBase::CString lDSSID;
                    hr = ipReadStream->ReadString(lDSSID);
                    CHECK_HR;
                    
                    GUID lGUID;
                    hr = DSSViewDataSetHelper::str2GUID(lDSSID, lGUID);
                    CHECK_HR;
                    
                    int lMetricTranslatedTotal;
                    hr = ipReadStream->ReadInt(&lMetricTranslatedTotal);
                    CHECK_HR;
                    
                    //zhyang, for the last two parameters, we just use double and reserved value
                    //need to change later
                    hr = mpMetrics->AddMetric(i, lDSSID, DssDataTypeDouble, lMetricTranslatedTotal);
                    CHECK_HR;
                }
            }
                break;
            default:
                break;
        }
        // weilin, DE32160, 2016/4/21 for locale of each unit
        mvLocales.push_back(lTmpLocale);
    }
    
    //tliao, more data for smart client
    if (ipReadStream->MoreDataInBlock())
    {
        hr = ipReadStream->BeginReadBlock(&lBlockType);
        CHECK_HR_BLOCK(EnumNewBlockDSSIDChild1);
    
        for(int i = 0; i < mnUnits; ++i)
        {
            switch (mvUnitType[i]) {
                 case DssTemplateMetrics:
                {                    
                    int lnMetrics = -1;
                    hr = ipReadStream->ReadInt(&lnMetrics);
                    CHECK_HR;
                    
                    int lnHiddenMetrics = 0;
                    hr = ipReadStream->ReadInt(&lnHiddenMetrics);
                    CHECK_HR;

                    if (lnMetrics+lnHiddenMetrics > 0)
                    {
                        mpMetrics->clear();
                    }
                    
                    for (int j = 0; j < lnMetrics+lnHiddenMetrics; ++j)
                    {
                        MBase::CString lDSSID;
                        hr = ipReadStream->ReadString(lDSSID);
                        CHECK_HR;
                        
                        GUID lGUID;
                        hr = DSSViewDataSetHelper::str2GUID(lDSSID, lGUID);
                        CHECK_HR;
                        
                        int lMetricTranslatedTotal;
                        hr = ipReadStream->ReadInt(&lMetricTranslatedTotal);
                        CHECK_HR;
                        
                        //zhyang, for the last two parameters, we just use double and reserved value
                        //need to change later
                        if (j < lnMetrics)
                        {
                            hr = mpMetrics->AddMetric(i, lDSSID, DssDataTypeDouble, lMetricTranslatedTotal);
                        }
                        else
                        {
                            hr = mpMetrics->AddMetric(i, lDSSID, DssDataTypeDouble, lMetricTranslatedTotal, false);
                        }
                        CHECK_HR;
                        
                        MBase::CString lTotalImplDSSID;
                        hr = ipReadStream->ReadString(lTotalImplDSSID);
                        CHECK_HR;
                            
                        GUID lTotalImplGUID;
                        hr = DSSViewDataSetHelper::str2GUID(lTotalImplDSSID, lTotalImplGUID);
                        CHECK_HR;
                            
                        int lDynamicAggregationFuntion = DssMetricReserved;
                        hr = ipReadStream->ReadInt(&lDynamicAggregationFuntion);
                        CHECK_HR;
                            
                        MBase::CString lDATotalImplDSSID;
                        hr = ipReadStream->ReadString(lDATotalImplDSSID);
                        CHECK_HR;
                            
                        GUID lDATotalImplGUID;
                        hr = DSSViewDataSetHelper::str2GUID(lDATotalImplDSSID, lDATotalImplGUID);
                        CHECK_HR;
                            
                        int lnAggInfo = 0;
                        hr = ipReadStream->ReadInt(&lnAggInfo);
                        CHECK_HR;
                            
                        vector<GUID> lvAttrGUID;
                        vector<EnumDSSAggregation> lvAggInfo;
                        for(int i = 0; i < lnAggInfo; ++i)
                        {
                            MBase::CString lDSSID;
                            hr = ipReadStream->ReadString(lDSSID);
                            CHECK_HR;
                            
                            GUID lGUID;
                            hr = DSSViewDataSetHelper::str2GUID(lDSSID, lGUID);
                            CHECK_HR;
                                
                            lvAttrGUID.push_back(lGUID);
                                
                            int lAggInfo;
                            hr = ipReadStream->ReadInt(&lAggInfo);
                            CHECK_HR;
                                
                            lvAggInfo.push_back((EnumDSSAggregation)lAggInfo);
                        }
                        
                        int lAvailableSystemSubtotal = 0;
                        hr = ipReadStream->ReadInt(&lAvailableSystemSubtotal);
                        CHECK_HR;
                        
                        int lAvailableUserDefinedSubtotal = 0;
                        hr = ipReadStream->ReadInt(&lAvailableUserDefinedSubtotal);
                        CHECK_HR;
                        
                        vector<GUID> lvUserDefinedSubtotalGUID;
                        for(int i = 0; i < lAvailableUserDefinedSubtotal; ++i)
                        {
                            MBase::CString lDSSID;
                            hr = ipReadStream->ReadString(lDSSID);
                            CHECK_HR;
                            
                            GUID lGUID;
                            hr = DSSViewDataSetHelper::str2GUID(lDSSID, lGUID);
                            CHECK_HR;
                            
                            lvUserDefinedSubtotalGUID.push_back(lGUID);
                        }
                        
                        hr = mpMetrics->setDynamicAggFunc((EnumDSSMetricType)lDynamicAggregationFuntion);
                        CHECK_HR;
                            
                        hr = mpMetrics->setAggInfo(lvAttrGUID, lvAggInfo);
                        CHECK_HR;
                            
                        hr = mpMetrics->setTotalImplID(lTotalImplGUID);
                        CHECK_HR;
                            
                        hr = mpMetrics->setDynamicAggID(lDATotalImplGUID);
                        CHECK_HR;
                        
                        hr = mpMetrics->setAvailableSubtotals(lAvailableSystemSubtotal, lvUserDefinedSubtotalGUID);
                        CHECK_HR;
                    }
                }
                    break;
                default:
                    break;
            }
        }
        //EnumNewBlockDSSIDChild1
        hr = ipReadStream->EndReadBlock();
        CHECK_HR;
        
        if (ipReadStream->MoreDataInBlock())
        {
            //tliao, 933283, 2014/08/08, dynamic dimty
            hr = ipReadStream->BeginReadBlock(&lBlockType);
            CHECK_HR_BLOCK(EnumNewBlockDynamicDimty);
            
            int lnLevels = 0;
            hr = ipReadStream->ReadInt(&lnLevels);
            CHECK_HR;
            
            mvDynamicDimty.resize(lnLevels);
            
            int lnMetrics = 0;
            hr = ipReadStream->ReadInt(&lnMetrics);
            CHECK_HR;
            
            for (int iLevel = 0; iLevel < lnLevels; iLevel++)
            {
                for (int iMetric = 0; iMetric < lnMetrics; iMetric++)
                {
                    MBase::CString lDSSID;
                    hr = ipReadStream->ReadString(lDSSID);
                    CHECK_HR;
                    
                    GUID lGUID;
                    hr = DSSViewDataSetHelper::str2GUID(lDSSID, lGUID);
                    CHECK_HR;
                    
                    vector<int> lvUnits;
                    hr = ipReadStream->ReadIntArray(lvUnits);
                    CHECK_HR;
                    
                    mvDynamicDimty[iLevel].insert(map<GUID,vector<int> >::value_type(lGUID, lvUnits));
                }
            }
            //tliao, 954998, 2014/08/28, support dynamic sort by
            if (ipReadStream->MoreDataInBlock())
            {
                int lnOLAPMetrics = 0;
                hr = ipReadStream->ReadInt(&lnOLAPMetrics);
                CHECK_HR;
                
                if (lnOLAPMetrics > 0)
                {
                    MBase::SimpleBuffer *lpBuffer = new MBase::SimpleBuffer();
                    MBase::StrongPtr<MBase::SimpleBuffer> lpBufferHolder(lpBuffer);
                    DSSReadBlockStream<DSSBinaryDataReader> lReadBlockStream (*ipReadStream);
                    mvDynamicSortBy.resize(1);
                    
                    for (int iMetric = 0; iMetric < lnOLAPMetrics; iMetric++)
                    {
                        MBase::CString lDSSID;
                        hr = ipReadStream->ReadString(lDSSID);
                        CHECK_HR;
                        
                        GUID lGUID;
                        hr = DSSViewDataSetHelper::str2GUID(lDSSID, lGUID);
                        CHECK_HR;
                        
                        DFCSimpleExpr* lpSimpleExpr = DFCSimpleExprBranch::Unserialize(&lReadBlockStream,lpBuffer);
                        
                        mvDynamicSortBy[0].insert(std::map<GUID,DFCSimpleExpr* >::value_type(lGUID, lpSimpleExpr));
                    }
                    
                    if (mpBuffer)
                    {
                        delete mpBuffer;
                        mpBuffer = NULL;
                    }
                    mpBuffer = lpBuffer;
                    
                    lpBufferHolder.GiveUp();
                }
            }
            
            //EnumNewBlockDynamicDimty
            hr = ipReadStream->EndReadBlock();
            CHECK_HR;
        }

    }
    
    //EnumNewBlockDSSID
    hr = ipReadStream->EndReadBlock();
    CHECK_HR;
    
    return hr;
}


int DSSTabularData::hCreateDummyUnit(int iUnitIndex, EnumDSSTemplateUnitType iUnitType, MBase::CString &irUnitIDStr, DSSTabularUnit **oppTabularData)
{
    if (!oppTabularData)
        return E_POINTER;
    
    int hr = S_OK;
    
    switch (iUnitType) {
        case DssTemplateAttribute:
        {
            DSSTabularAttribute *lpAttr = new DSSTabularAttribute();
            //zhyang, 636369, we need to specify arity when the number of forms is greater than 0 
            hr = lpAttr->Init(iUnitIndex, mvForms[iUnitIndex] > 0 ? 1 : 0);
            CHECK_HR;
            
            hr = lpAttr->setGUID(irUnitIDStr);
            CHECK_HR;
            
            //zhyang, missing unit
            if(!ATL::InlineIsGUIDNULL(mvUnitGUID[iUnitIndex]))
            {
                for(int i = 0; i < mvForms[iUnitIndex]; ++i)
                {
                    DSSDataColumn *lpColumn = NULL;
                    hr = DSSAEHelper::GenXTabCol(DssDataTypeMissing, 1, &lpColumn);
                    CHECK_HR;
                    
                    hr = lpAttr->AddFormDataColumn(i, DssBaseFormReserved, lpColumn);
                    CHECK_HR;
                }
            }
            
            *oppTabularData = lpAttr;
        }
            break;
        case DssTemplateCustomGroup:
        {
            DSSTabularCustomGroup *lpCustomGroup = new DSSTabularCustomGroup();
            hr = lpCustomGroup->Init(iUnitIndex, 0);
            CHECK_HR;
            
            hr = lpCustomGroup->setGUID(irUnitIDStr);
            CHECK_HR;
            
            *oppTabularData = lpCustomGroup;
        }
            break;
        case DssTemplateConsolidation:
        {
            DSSTabularConsolidation *lpConsolidation = new DSSTabularConsolidation();
            hr = lpConsolidation->Init(iUnitIndex, 0);
            CHECK_HR;
            
            hr = lpConsolidation->setGUID(irUnitIDStr);
            CHECK_HR;
            
            *oppTabularData = lpConsolidation;
        }
            break;
        default:
            _ASSERT(false);
            break;
    }
    return  S_OK;
}

DSSGUnitContainer* DSSTabularData::getGUnitContainer()
{
    return mpGlobalLookupContainer;
}

const GUID& DSSTabularData::getMetricID(int iMetricIndex)
{
    return mpMetrics->getMetricID(iMetricIndex);
}

void DSSTabularData::setIsRunTimeUnit(int iUnitID,bool iIsRuntimeUnit)
{
    if(iUnitID < mvUnits.size())
    {
        mvUnits[iUnitID]->setIsRuntimeUnit(iIsRuntimeUnit);
    }
}

bool DSSTabularData::canUseSSO(GUID &irGUID)
{
    for(int i = 0; i < mvUnitGUID.size(); ++i)
    {
        if(ATL::InlineIsEqualGUID(mvUnitGUID[i], irGUID))
        {
            DSSTabularUnit *lpUnit = mvUnits[i];
            
            // tehe: skip dummy unit
            DSSXTabRefLookupTable *lpLookupTable = dynamic_cast<DSSXTabRefLookupTable *>(lpUnit);
            if(lpLookupTable)
                return lpLookupTable->canUesSSO();
        }
    }
    
    return false;
}

int DSSTabularData::enableSSOForAllUnits()
{
    for(int i = 0; i < mvUnits.size(); ++i)
    {
        if(mvUnitType[i] == DssTemplateAttribute && i != mDummyUnitIndex)
        {
            DSSTabularUnit *lpUnit = mvUnits[i];
            assert(dynamic_cast<DSSXTabRefLookupTable *>(lpUnit) != NULL);
            (static_cast<DSSXTabRefLookupTable *>(lpUnit))->enableSSO();
        }
    }
    return S_OK;
}

int DSSTabularData::inheritSSOStatusForAllAttr(DSSViewDataSet *ipViewDataSet)
{
    for(int i = 0; i < mvUnits.size(); ++i)
    {
        if(mvUnitType[i] == DssTemplateAttribute)
        {
            DSSTabularUnit *lpUnit = mvUnits[i];
            // tehe: skip dummy unit
            DSSXTabRefLookupTable *lpLookupTable = dynamic_cast<DSSXTabRefLookupTable *>(lpUnit);
            //zhyang, 740180
            if(lpLookupTable && !(ipViewDataSet->getTabularData()->canUseSSO(mvUnitGUID[i])))
            {
                lpLookupTable->disableSSO();
            }
        }
    }
    return S_OK;
}

int DSSTabularData::getUnitGUIDAndType(int pos, GUID &irGUID, EnumDSSTemplateUnitType &irType)
{
    if (pos < 0 || pos > mvUnitGUID.size())
        return E_INVALIDARG;
    
    irGUID = mvUnitGUID[pos];
    irType = mvUnitType[pos];
    
    return S_OK;
}

int DSSTabularData::Init(DSSDataModelBase *ipModelBase, const vector<DSSTabularUnit *> & irvUnits)
{
    //This init function is used by old rwd engine to keep backward compatibility
    //only the necessary data members are initialized be facilitate sorter classes
    mbIsNewRWD = false;
    mpDataModelBase = ipModelBase;
    mvUnits.assign(irvUnits.begin(), irvUnits.end());
    
    int lnUnits = mvUnits.size();
    mvUnitGUID.resize(lnUnits);
    mvUnitType.resize(lnUnits);
    
    for (int i = 0; i < lnUnits; i++)
    {
        DSSTabularUnit *lpUnit = mvUnits[i];
        EnumDSSTemplateUnitType lType = lpUnit->getUnitType();
        
        mvUnitType[i] = lType;
        
        if (lType != DssTemplateMetrics)
        {
            lpUnit->getGUID(mvUnitGUID[i]);
        }
        else
        {
            mvUnitGUID[i] = GUID_NULL;
            mMetircPos = i;
            mpMetrics = dynamic_cast<DSSTabularMetrics*>(lpUnit);
            _ASSERT(mpMetrics);
        }
    }
    mnUnits = lnUnits;
    
    if (ipModelBase->getModelType() == DssModelXTab)
    {
        DSSDataModelXTabImpl *lpXTabModel = dynamic_cast<DSSDataModelXTabImpl*>(ipModelBase);
        if (lpXTabModel)
        {
            this->setXTabView(lpXTabModel->getXTabView());
            this->CopySubtotals(lpXTabModel->getSubtotals());
        }
    }
    
    this->setObjectContext(ipModelBase->getObjectContext());
    
    return S_OK;
}

// 1/11/2011 tehe: meric unit, hidden unit and dummy unit are not report level units
int DSSTabularData::GetReportLevelUnits(int nUnit, int *iopUnits, int *opnUnit)
{
    int hr = S_OK;
	AE_ASSERT(iopUnits);
	AE_ASSERT(opnUnit);
    
	//at first use iopUnits as boolean flag array
	int i = 0;
	for (i = 0; i < nUnit; i ++)
		iopUnits[i] = 1;
    
	for (i = 0; i < nUnit; i ++)
	{ //find out which unit is the implicit unit
		EnumDSSTemplateUnitType lType = DssTemplateReserved;
        DSSTabularUnit *lpTabularUnit = NULL;
        hr = Item(i,&lpTabularUnit);
        CHECK_HR;
        
		lType = lpTabularUnit->getUnitType();
        
		if (lType == DssTemplateMetrics)
		{	//remove metrics unit
			iopUnits[i] = 0;
		}
		else if (lType == DssTemplateConsolidation)
		{
			//remove the implicit child unit
			DSSTabularConsolidation *lpConsolidation = static_cast<DSSTabularConsolidation *>(lpTabularUnit);
			int lChildUnit = -1;
            // tehe 1/11/2011 special handling for consolidation: at server side we can get the implicit child unit direct from Consolidation unit, but at client side if the Consolidation is a DE, the child unit of it is a GlobalLookuptable unit, if Consolidation is not a DE, the child unit will not be serialized from server side .
            hr = FindUnitI(lpConsolidation->getGUID(), DssTemplateCustomGroup, lChildUnit);
            CHECK_HR;
            
            if(lChildUnit == -1)
                return E_FAIL;
			iopUnits[lChildUnit] = 0;
		}
        
	}
    
    int lDummyUnitIndex;
    hr = getDummyUnitIndex(&lDummyUnitIndex);
    CHECK_HR
    
	if (lDummyUnitIndex != -1)
		iopUnits[lDummyUnitIndex] = 0;
    
	//finally fill iopUnits with the correct contents
	int lnExplicitUnits = 0;
	for (i = 0; i < nUnit; i ++)
	{
		if (iopUnits[i])
		{
			iopUnits[lnExplicitUnits] = i;
			lnExplicitUnits ++;
		}
	}
    
	*opnUnit = lnExplicitUnits;
    
	return S_OK;
}



//imihai, 2013-06-05
int DSSTabularData::setSubtotalInformation(int index, int iTotalID, MBase::CString &irTotalGUID, vector<EnumDSSDataType> &irvDataTypes, MBase::CString &irTotalName, int iTotalType)
{
	if (index >= mvTotals.size())
	{
		int count = mvTotals.size() - 1;
		for (; count < index; count++)
		{
			mvTotals.push_back(SubtotalInformation());
		}
		mnTotals = mvTotals.size();
	}
	
	SubtotalInformation *lpTotal = &mvTotals[index];
	lpTotal->toalID = iTotalID;
	lpTotal->mGuid = irTotalGUID;
	lpTotal->name = irTotalName;
	lpTotal->type = (EnumDSSMetricType)iTotalType;
	
	lpTotal->dataTypes.reserve(irvDataTypes.size());
	for (int i = 0; i < irvDataTypes.size(); i++)
		lpTotal->dataTypes.push_back(irvDataTypes[i]);
	return 0;
}

//imihai, 2013-06-05
EnumDSSMetricType DSSTabularData::getSubtotalType(int iTotalIndex)
{
	if (iTotalIndex >= 0 && iTotalIndex < mnTotals)
		return mvTotals[iTotalIndex].type;
	else
		return DssMetricReserved;
}

//imihai, 2013-06-05
const char *DSSTabularData::getSubtotalNameAndLength(int iTotalIndex, int &orLength)
{
	if (iTotalIndex < 0 || iTotalIndex >= mnTotals)
		return NULL;
	
	orLength = mvTotals[iTotalIndex].name.length();
	return mvTotals[iTotalIndex].name.c_str();
}

int DSSTabularData::CopySubtotals(vector<SubtotalInformation> modelTotals){
    int hr = S_OK;
    for (int i=0; i<modelTotals.size(); i++)
    {
        SubtotalInformation *lpTotal = &modelTotals[i];
        hr = setSubtotalInformation(i, lpTotal->toalID, lpTotal->mGuid, lpTotal->dataTypes, lpTotal->name, lpTotal->type);
        CHECK_HR;
    }
    return hr;
}

//Comments added by xhan 11/07/05
//This function is actually the filtering function. After the filter expression is evaluated  and
// resulting filter slice ipFilterSlice is generated, we need to apply this filter slice to the original
//cube, and populate the filtered data to a new tabular data structure.
// This function is called by subset engine or metric limit evaluator.
// If it is from metric limit, then nSelectedUnits == -1 and pSelectedUnitInfo = NULL
int DSSTabularData::filterPopulate(
                             DSSCube            *ipCube, 
                             int                iNumUnits, 
                             int                *ipUnits, 
                             vector<DSSSliceID> &ivSliceIDs, 
                             METRIC_ID          *ipMetricIDs, 
                             DSSCubeDataSlice   *ipFilterSlice, 
                             DSSXTabBitVector   *ipMask, 
                             int                nSelectedUnits,
                             UnitFormInfo       *pSelectedUnitInfo,
                             int                iFilterDEFlag,
                             bool               ibRWDSubsetFromBase, 
                             DSSGUnitContainer  *ipGlobalLookupContainer, 
                             int                iNumDerivedElements, 
                             int                *ipDerivedElementUnits)
{
    int hr = S_OK;
    
    int lNumSlices = ivSliceIDs.size();
    DSSSliceID *lpSlices = &ivSliceIDs[0];
    
    MBase::SimpleBuffer lBuffer;
    MBase::SimpleBuffer *lpBuffer = &lBuffer;
    MBase::Allocator<Int32> lAlloc(lpBuffer);
    
    DSSTabularData* lpOriginalTabularData = ipCube->getTabularData();
    CHECK_PTR(lpOriginalTabularData);
    
    int lOrgDummyUnitIndex = -1;
    hr = lpOriginalTabularData->getDummyUnitIndex(&lOrgDummyUnitIndex);
    CHECK_HR;
    
    //ctang: this is a view use global lookup, mark the GUnitContainer
    mpGlobalLookupContainer = ipGlobalLookupContainer;
    
    //xhan If there is no consolidation unit in original tabular data, NumUnits is same as the unit count of original tabularData
	// If there is any consolidation unit, then pUnits contains the consolidation unit or the child unit of the consolidation unit,
	// and in such case, NumUnits is less than the unit count of the original tabular data
	//For example, if the report has unit {Year, MonthCS}, then the original tabular data has {Year, Month, MonthCS},
	// and pUnits either contains {Year, MonthCS} or {Year, Month}
    
	// first need to make sure the list of units is ordered.
    int* lpUnit = new (lpBuffer) int[iNumUnits+1];
    CHECK_NEWED_PTR(lpUnit);
    
    if (iNumUnits > 0)
    {
        assert(lpUnit != NULL);
    }
    memmove(lpUnit, ipUnits, sizeof(int) * iNumUnits);
    if (lOrgDummyUnitIndex != -1)
        lpUnit[iNumUnits++] = lOrgDummyUnitIndex;
    
    std::sort(lpUnit, lpUnit+iNumUnits);
    
    // maybe we need any duplication
    
	// Make sure the given slice are at least aggregated to the
	// level of the remaining attribute.
    if (lNumSlices > 0)
        assert(lpSlices != NULL);
    
    int iSlice = 0;
    for (iSlice = 0; iSlice < lNumSlices; iSlice++)
    {
        //check if all the slice's level is compatible with the selection
        DSSCubeDataSlice* lpSlice = NULL;
        hr = lpSlices[iSlice].GetSlice(&lpSlice);
		CHECK_HR_PTR(lpSlice);
        
        DSSTabularIndex* lpIndex = lpSlice->getIndices();
        CHECK_PTR(lpIndex);
        
        int lnUnit2 = lpIndex->getNumUnits();
        int* lpUnit2 = new (lpBuffer) int[lnUnit2];
        CHECK_NEWED_PTR(lpUnit2);
        
        hr = lpIndex->getUnits(lnUnit2, lpUnit2);
        CHECK_HR;
        
        if (!std::includes(lpUnit, lpUnit+iNumUnits, lpUnit2, lpUnit2+lnUnit2))
        {
            assert(false);// Input slice's level should be contained in lpUnit, otherwise, internal error, should assert
			//For example, the metric slice is at level {Store, Year} and lpUnit has {Store, Country}, there must be some
			// thing wrong here, because we assume lpUnit, in no consolidatin case, contains the same units as the
			//original tabular data.
        }
    }
    
    //////////////////////////////////////////////////
    mvUnits.resize(iNumUnits);
    mvUnitType.resize(iNumUnits);
    mvUnitGUID.resize(iNumUnits);
    mvUnitGlobalIndex.resize(iNumUnits);
    
    if (nSelectedUnits == -1) //Metric Limit event
    {
        mvCGPos = lpOriginalTabularData->getCGPosition();
        mvCGOrdinals = lpOriginalTabularData->getCGOrdinals();
    }
    //////////////////////////////////////////////////
    
    DSSTabularIndexPool* lpOriginalIndexPool = lpOriginalTabularData->getIndexPool();
    CHECK_PTR(lpOriginalIndexPool);
    
    DSSTabularIndex* lpOriginalIndex = NULL;
    hr = lpOriginalIndexPool->getIndex(iNumUnits, lpUnit, &lpOriginalIndex);
    CHECK_HR_PTR(lpOriginalIndex);
    
    //we need to take a look at the filter now and see which row will be copied out.
    
	//lwang, 6/05/2007, add support for filtering DE(Derived Element) and unqualified rows.
	//Based on the filter result, we'll copy more rows that are needed to recalculate Derive Element
	DSSXTabBitVector lbvUnqulifiedRows(0);
    
    //Begin: Calculating Filtered Rows
    int* lpFilteredRows = NULL;
    int* lpRowForOrigIND = NULL;
    int lnFilteredRows = 0;
    hr = filterIndex(lpOriginalIndex, ipFilterSlice, ipMask, lpOriginalTabularData, lnFilteredRows, lpFilteredRows, lpRowForOrigIND, &lbvUnqulifiedRows, iFilterDEFlag, lpBuffer);
    CHECK_HR_PTR(lpRowForOrigIND);
    //End: Calculating Filtered Rows
    
    //xhan Both lpFilteredRows and lpRowForOrigIND are the results from applying ipFilterSlice to lpOriginalIndices.
	// lpFilteredRows has row numbers that satisfying the ipFilterSlice
	// lpRowForOrigIND contains the unit keys conresponding the row numbers in lpFilteredRows
	// For example, if the original report is
	//					 Year		Country		M
	//					 2000		USA			10
	//					 2000		Canada		20
	//					 2001		USA			30
	//					 2001       Canada      20
	//	and if filter is M < 30 then
	// the lpOriginalIndices is =  [ {0, 0}, {0, 1}, {1, 0} {1, 1} ]
	// ipFilterSlice is [1, 1, 0, 1]
	// lnFilteredRows = 3 lpFilteredRows[] = [0, 1, 3],  lpRowForOrigIND = [ {0, 0}, {0, 1} {1, 1}]
    
	// weiwang 20100-09-21 info for local/global key translation
    typedef std::map<int, int, less<int> > IntToIntMapType;
    //int lnUnitRows;
    Int32* lGToLMap[iNumUnits];
	IntToIntMapType* lGToLMap2[iNumUnits];
	memset(lGToLMap, 0, sizeof(Int32*) * iNumUnits);
	memset(lGToLMap2, 0, sizeof(IntToIntMapType*) * iNumUnits);
    
    vector<Int32, MBase::Allocator<Int32> > ** lppvRowForLUT = new (lpBuffer) vector<Int32, MBase::Allocator<Int32> >*[iNumUnits];
	CHECK_NEWED_PTR(lppvRowForLUT);
    
    int* lpRowForIND = new (lpBuffer) int[iNumUnits * lnFilteredRows];
    CHECK_NEWED_PTR(lpRowForIND);
    
    //Begin: TabularData Populating Filtered Key For Units
    
    //xhan 11/09/05
	//Populate the units from the original tabular data to the new tabular data with the consideration of filter result
    int iUnit = 0;
    for (iUnit = 0; iUnit < iNumUnits; iUnit++)
    {
        DSSTabularUnit* lpTabularUnit = NULL;
        hr = lpOriginalTabularData->Item(lpUnit[iUnit], &lpTabularUnit);
        CHECK_HR_PTR(lpTabularUnit);
        
        if (lpUnit[iUnit] == lOrgDummyUnitIndex)
            mDummyUnitIndex = iUnit;    //keep track the dummy unit in the new tabular data
        
        EnumDSSTemplateUnitType lType= lpTabularUnit->getUnitType();
        
        //////////////////////////////////////////////////
        mvUnitType[iUnit] = lType;
        GUID lGUID;
        int lIndex = -1;
        hr = lpOriginalTabularData->getUnitGUIDAndGlobalOffset(lpUnit[iUnit], lGUID, lIndex);
        CHECK_HR;
        
        mvUnitGUID[iUnit] = lGUID;
        mvUnitGlobalIndex[iUnit] = lIndex;
        //////////////////////////////////////////////////
        
        // weiwang 2010-12-01 use ref unit for ML
		if (nSelectedUnits == -1)	// ML
		{
			DSSTabularUnit* lpOrgTabularUnit = NULL;
			hr = lpTabularUnit->getTabularUnit(&lpOrgTabularUnit);
			CHECK_HR_PTR(lpOrgTabularUnit);
            
			if (lpOrgTabularUnit != lpTabularUnit)	// Ref Unit
			{
				DSSTabularUnit * lpRefUnit = NULL;
				hr = createReferenceUnitI(lType, iUnit, lpTabularUnit, lpOriginalTabularData, &lpRefUnit);
				CHECK_HR_PTR(lpRefUnit);
				mvUnits[iUnit] = lpRefUnit;
                
				// need to copy over the map info
				Int32 lRowCount;
				Int32* lpGToLMap;
				Int32* lpLToGMap;
				IntToIntMapType* lpGToLIntMap;// for DE case
				if (lType == DssTemplateCustomGroup)
				{
					hr = static_cast<DSSTabularRefCustomGroup*>(lpTabularUnit)->GetGlobalRowMap(&lRowCount, &lpGToLMap, &lpLToGMap);
					CHECK_HR;
					hr = static_cast<DSSTabularRefCustomGroup*>(lpRefUnit)->SetGlobalRowMap(lRowCount, lpGToLMap, lpLToGMap);
					CHECK_HR;

                    int pos = -1;
                    for (int i = 0; i < mvCGPos.size(); i++) {
                        if (mvCGPos[i] == iUnit) {
                            pos = i;
                            break;
                        }
                    }
                    if (pos != -1)
                        static_cast<DSSTabularRefCustomGroup*>(lpRefUnit)->SetOrdinal(mvCGOrdinals[pos]);
				}
				else if (lType == DssTemplateConsolidation)
				{
					hr = static_cast<DSSTabularRefConsolidation*>(lpTabularUnit)->GetGlobalRowMap(&lRowCount, &lpGToLMap, &lpGToLIntMap, &lpLToGMap);
					CHECK_HR;
					hr = static_cast<DSSTabularRefConsolidation*>(lpRefUnit)->SetGlobalRowMap(lRowCount, lpGToLMap, lpGToLIntMap, lpLToGMap);
					CHECK_HR;
				}
                
				lppvRowForLUT[iUnit] = NULL;
				Int32* lpDest = lpRowForIND + iUnit * (size_t)lnFilteredRows;
				Int32* lpSrc = lpRowForOrigIND + iUnit * (size_t)lnFilteredRows;
				memmove(lpDest, lpSrc, sizeof(Int32) * (size_t)lnFilteredRows);
                
				continue;
			}
		}
        
        //weiwang 2010-11-26 local populate for DE units
        bool lfDE = false;
        if (lType == DssTemplateAttribute || lType == DssTemplateConsolidation)
        {
            for (int iDEUnit = 0; iDEUnit < iNumDerivedElements; iDEUnit++) {
                if (ipDerivedElementUnits[iDEUnit] == lpUnit[iUnit])
                {
                    lfDE = true;
                    break;
                }
            }
        }
        
        if (/*ibRWDLookUp*/  ipGlobalLookupContainer && iUnit != mDummyUnitIndex && !lfDE)	// rwd lookup
		{
			DSSTabularUnit* lpTabularDataUnit2 = lpTabularUnit;
			if (/*lpGUnitContainer*/  ibRWDSubsetFromBase && lType == DssTemplateAttribute)
			{
				GUID lID;
				hr = lpTabularUnit->getGUID(lID);
				CHECK_HR;
                
				DSSTabularAttribute* lpLookUp = NULL;
				hr = ipGlobalLookupContainer->getGlobalLookupByID(&lID, NULL, &lpLookUp);
				CHECK_HR_PTR(lpLookUp);
                
				lpTabularDataUnit2 = (DSSTabularUnit*)lpLookUp;
			}
            
			CHECK_HR_PTR(lpTabularDataUnit2);
            
			DSSTabularUnit* lpRefSubsetUnit = NULL;
			hr = createReferenceUnitI(lType, iUnit, lpTabularDataUnit2, lpOriginalTabularData, &lpRefSubsetUnit);
			CHECK_HR_PTR(lpRefSubsetUnit);
			mvUnits[iUnit] = lpRefSubsetUnit;
            
			if (lType == DssTemplateAttribute)
			{
				vector<Int32> lvSelectedForm;
				bool lfSelectedAll = false;
                
				Int32 i = 0;
				for (; i < nSelectedUnits; i++)
				{
					if (lpUnit[iUnit] == pSelectedUnitInfo[i].UnitID)
						break;
				}
                
				if (i < nSelectedUnits)
				{ //Found
					for (Int32 j = 0; j < pSelectedUnitInfo[i].nForms; j++)
					{
						Int32 lForm = -1;
						hr = static_cast<DSSTabularAttribute*>(lpTabularDataUnit2)->FindFormWithLocale(0,pSelectedUnitInfo[i].Forms[j], pSelectedUnitInfo[i].Locales[j], lForm);
						if (hr == S_OK)
							lvSelectedForm.push_back(lForm);
					}
                    
					lfSelectedAll = pSelectedUnitInfo[i].mfSelectAll;
				}
                
				/*Int32 lnForm = lfSelectedAll ? -1 : lvSelectedForm.size();
				Int32* lpForm = lvSelectedForm.size() > 0 ? &lvSelectedForm[0] : NULL;
                // not sure below code is useful.
				hr = static_cast<DSSXTabRefLookupTable*>(lpRefSubsetUnit)->SetSelectedForms(lnForm, lpForm);
                 */
				CHECK_HR;
			}
			else
			{
				Int32 lRowCount;
				Int32* lpGToLMap;
				Int32* lpLToGMap;
				IntToIntMapType* lpGToLIntMap;// for DE case
                
				if (/*lpGUnitContainer*/  ibRWDSubsetFromBase)
				{
                    DSSTabularUnit * lpOrignUnit = NULL;
                    hr = lpTabularUnit->getTabularUnit(&lpOrignUnit);
                    CHECK_HR;
                    
					vector<int>* lpTmpMap1 = ipGlobalLookupContainer->getGlobalToLocalMap(mDataSetGUID, lpOrignUnit);
					if (lpTmpMap1 != NULL && lpTmpMap1->size() > 0)
                    {
                        lpGToLMap = &(*lpTmpMap1)[0];
                    }
                    lRowCount = lpTmpMap1->size();
                    lpGToLIntMap = ipGlobalLookupContainer->getGlobalToLocalMap2(mDataSetGUID, lpOrignUnit);
					
                    vector<int>* lpTmpMap2 = ipGlobalLookupContainer->getLocalToGlobalMap(mDataSetGUID, lpOrignUnit);
					if (lpTmpMap2 != NULL && lpTmpMap2->size() > 0)
                    {
                        lpLToGMap = &(*lpTmpMap2)[0];
                    }
				}
				else
				{
					if (lType == DssTemplateCustomGroup)
						hr = static_cast<DSSTabularRefCustomGroup*>(lpTabularUnit)->GetGlobalRowMap(&lRowCount, &lpGToLMap, &lpLToGMap);
					else
						hr = static_cast<DSSTabularRefConsolidation*>(lpTabularUnit)->GetGlobalRowMap(&lRowCount, &lpGToLMap, &lpGToLIntMap, &lpLToGMap);
					CHECK_HR;
				}
                
				if (lType == DssTemplateCustomGroup)
                {
					hr = static_cast<DSSTabularRefCustomGroup*>(lpRefSubsetUnit)->SetGlobalRowMap(lRowCount, lpGToLMap, lpLToGMap);
                    int pos = -1;
                    for (int i = 0; i < mvCGPos.size(); i++) {
                        if (mvCGPos[i] == iUnit) {
                            pos = i;
                            break;
                        }
                    }
                    if (pos != -1)
                        static_cast<DSSTabularRefCustomGroup*>(lpRefSubsetUnit)->SetOrdinal(mvCGOrdinals[pos]);
                }
				else
				{
					hr = static_cast<DSSTabularRefConsolidation*>(lpRefSubsetUnit)->SetGlobalRowMap(lRowCount, lpGToLMap, lpGToLIntMap, lpLToGMap);
					CHECK_HR;
				}
				CHECK_HR;
			}
            
			lppvRowForLUT[iUnit] = NULL;
			Int32* lpDest = lpRowForIND + iUnit * (size_t)lnFilteredRows;
			Int32* lpSrc = lpRowForOrigIND + iUnit * (size_t)lnFilteredRows;
            
			if (/*lpGUnitContainer*/  ibRWDSubsetFromBase)
			{
                DSSTabularUnit * lpOrignUnit = NULL;
                hr = lpTabularUnit->getTabularUnit(&lpOrignUnit);
                CHECK_HR;
                
                vector<int>* lpLocal2Global = ipGlobalLookupContainer->getLocalToGlobalMap(mDataSetGUID, lpOrignUnit);
                
				vector<int>* lpGlobal2Local = ipGlobalLookupContainer->getGlobalToLocalMap(mDataSetGUID, lpOrignUnit);
                if (lpGlobal2Local != NULL && lpGlobal2Local->size() != 0)
                {
                    lGToLMap[iUnit] = (Int32*)(&(*lpGlobal2Local)[0]);
                }
                lGToLMap2[iUnit] = ipGlobalLookupContainer->getGlobalToLocalMap2(mDataSetGUID, lpOrignUnit);
                
				if (lpLocal2Global != NULL && lpLocal2Global->size() > 0)
				{
					for (Int32 i = 0; i < (size_t)lnFilteredRows; i++)
					{
						*lpDest = (*lpLocal2Global)[*lpSrc];
						lpDest++;
						lpSrc++;
					}
				}
				else
				{
					memmove(lpDest, lpSrc, sizeof(Int32) * (size_t)lnFilteredRows);
				}
			}
			else
			{
				memmove(lpDest, lpSrc, sizeof(Int32) * (size_t)lnFilteredRows);
			}
		}
        else if (iUnit == mDummyUnitIndex)
        {
            //tliao: only dummy unit go here
			DSSTabularUnit * lpRefSubsetUnit = NULL;
			hr = createReferenceUnitI(lType, iUnit, lpTabularUnit, lpOriginalTabularData, &lpRefSubsetUnit);
			CHECK_HR_PTR(lpRefSubsetUnit);
			mvUnits[iUnit] = lpRefSubsetUnit;

            lppvRowForLUT[iUnit] = new (lpBuffer) vector<Int32, MBase::Allocator<Int32> >(lAlloc);
			CHECK_NEWED_PTR(lppvRowForLUT[iUnit]);
            
			//LOG_MESSAGE(Engine, Perf, "BEGIN: TabularData Populating Filtered Key for Unit %1", <<iUnit);
			//lpRowForIND is new unit keys used to generate new indices in the result tabular data.
			// lpRowForIND is derived from lpFilteredRows and lpRowForOrigIND
			//In the above example lpRowForIND is = [ {0, 0}, {0, 1} {1, 1}]
			hr = filterUnit(lpRowForOrigIND + iUnit * (size_t)lnFilteredRows,
							lnFilteredRows,
							*lppvRowForLUT[iUnit],
							lpRowForIND + iUnit * (size_t)lnFilteredRows,
							lpBuffer);
			CHECK_HR;

		}
        else
        {
            /*
            hr = CreateUnit_0(lType, iUnit, &mvUnits[iUnit]);
			CHECK_HR;
            
			lppvRowForLUT[iUnit] = new (lpBuffer -> p) vector<Int32, MBase::Allocator<Int32> >(lAlloc);
			CHECK_NEWED_PTR(lppvRowForLUT[iUnit]);
            
			//LOG_MESSAGE(Engine, Perf, "BEGIN: TabularData Populating Filtered Key for Unit %1", <<iUnit);
			//lpRowForIND is new unit keys used to generate new indices in the result tabular data.
			// lpRowForIND is derived from lpFilteredRows and lpRowForOrigIND
			//In the above example lpRowForIND is = [ {0, 0}, {0, 1} {1, 1}]
			hr = filterUnit(lpRowForOrigIND + iUnit * (size_t)lnFilteredRows,
							lnFilteredRows,
							*lppvRowForLUT[iUnit],
							lpRowForIND + iUnit * (size_t)lnFilteredRows,
							lpBuffer -> p);
			CHECK_HR;
			//LOG_MESSAGE(Engine, Perf, "END: TabularData Populating Filtered Key for Unit %1", <<iUnit);
            
			//here we will do it based on type
			Int32 lppvRowForLUTSize = lppvRowForLUT[iUnit]->size();
			Int32 * lppvRowForLUTStart = (lppvRowForLUTSize>0)? &(*lppvRowForLUT[iUnit])[0]:NULL;
			switch(lType)
			{
                case DssTemplateAttribute:
				{
					CComPtr<ICDSSXTabLookUpTable> lpLUT;
					hr = lpTabularDataUnit -> QueryInterface(IID_ICDSSXTabLookUpTable,
                                                             (LPVOID *)&lpLUT);
					CHECK_HR_PTR(lpLUT);
                    
					if (nSelectedUnits > -1 )
					{	//It is from subset Engine, only copy the relevant forms
						Int32 i = 0;
						for ( ; i < nSelectedUnits; i ++)
						{
							if (lpUnit[iUnit] == pSelectedUnitInfo[i].UnitID)
								break;
						}
                        
						if (i < nSelectedUnits)
						{ //Found
							hr = static_cast<CDSSXTabLookUpTable *>(mvUnits[iUnit])
                            -> FilterPopulate(lpLUT,
                                              iUnit,
                                              lppvRowForLUTSize,
                                              lppvRowForLUTStart ,
                                              pSelectedUnitInfo + i);
							CHECK_HR;
                            
                            
						} else
						{	//The unit is not in subset selection, as a place holder, don't select any form.
							UnitFormInfo lUnitInfo;
							lUnitInfo.nForms = 0;
                            
							hr = static_cast<CDSSXTabLookUpTable *>(mvUnits[iUnit])
                            -> FilterPopulate(lpLUT,
                                              iUnit,
                                              lppvRowForLUTSize ,
                                              lppvRowForLUTStart ,
                                              &lUnitInfo);
							CHECK_HR;
                            
						}
                        
                        
					} else { //It is from metric limt, copy all forms
                        
						hr = static_cast<CDSSXTabLookUpTable *>(mvUnits[iUnit])
                        -> FilterPopulate(lpLUT,
                                          iUnit,
                                          lppvRowForLUTSize,
                                          lppvRowForLUTStart ,
                                          NULL);
						CHECK_HR;
					}
				}
                    break;
                case DssTemplateCustomGroup:
				{
					CComPtr<ICDSSXTabCustomGroup> lpCustomGroup;
					hr = lpTabularDataUnit -> QueryInterface(IID_ICDSSXTabCustomGroup,
                                                             (LPVOID *)&lpCustomGroup);
					CHECK_HR_PTR(lpCustomGroup);
                    
					hr = static_cast<CDSSXTabCustomGroup *>(mvUnits[iUnit])
                    -> Populate(lpCustomGroup,
                                iUnit,
                                *lppvRowForLUT[iUnit],
                                lpRowForIND + iUnit * (size_t)lnFilteredRows, lnFilteredRows);
					CHECK_HR;
				}
                    break;
                case DssTemplateConsolidation:
				{
					CComPtr<ICDSSXTabConsolidation> lpConsolidation;
					hr = lpTabularDataUnit -> QueryInterface(IID_ICDSSXTabConsolidation,
                                                             (LPVOID *)&lpConsolidation);
					CHECK_HR_PTR(lpConsolidation);
                    
					Int32 lnUnitInCSLD = 0;
					hr = lpConsolidation -> get_NumUnits(&lnUnitInCSLD);
					CHECK_HR;
                    
					Int32 * lpUnitInCSLD = new (lpBuffer -> p) Int32[lnUnitInCSLD];
					CHECK_NEWED_PTR(lpUnitInCSLD);
					Int32 * lpUnitInCSLD2 = new (lpBuffer -> p) Int32[lnUnitInCSLD];
					CHECK_NEWED_PTR(lpUnitInCSLD2);
					hr = lpConsolidation -> get_Units(lnUnitInCSLD, lpUnitInCSLD);
					CHECK_HR;
                    
					IDSSTabularUnit ** lppUnit = new (lpBuffer -> p) IDSSTabularUnit*[lnUnitInCSLD];
					CHECK_NEWED_PTR(lppUnit);
					IDSSTabularUnit ** lppUnit2 = new (lpBuffer -> p) IDSSTabularUnit*[lnUnitInCSLD];
					CHECK_NEWED_PTR(lppUnit2);
                    
					for(Int32 iUnit2 = 0; iUnit2 < lnUnitInCSLD; iUnit2 ++)
					{
						DSSTabularUnit * lpSubUnit;
						hr = lpOriginalTabularData-> Item(lpUnitInCSLD[iUnit2], &lpSubUnit);
						CHECK_HR;
                        
						lppUnit[iUnit2] = lpSubUnit;
                        
						DSSTemplateUnit_Type lSubType;
						hr = lpSubUnit -> get_UnitType(&lSubType);
						CHECK_HR;
                        
						DSSTabularUnit * lpTemp = NULL;
						lpUnitInCSLD2[iUnit2] = mvUnits.size();
						hr = CreateUnit_0(lSubType, lpUnitInCSLD2[iUnit2], &lpTemp);
                        
						mvUnits.push_back(lpTemp);
						lppUnit2[iUnit2] = lpTemp;
						CHECK_HR;
					}
					hr = static_cast<CDSSXTabConsolidation *>(mvUnits[iUnit])
                    -> Populate(lpConsolidation,
                                iUnit,
                                lpUnitInCSLD2,
                                lppUnit,
                                lppUnit2,
                                *lppvRowForLUT[iUnit],
                                lpRowForIND + iUnit * (size_t)lnFilteredRows, lnFilteredRows);
					CHECK_HR;
				}
                    break;
                default:
                    return hr = E_FAIL;
			}
             */
          }
        
        //ctang: 673438 since MCE could do another subset after join, we need inherit the repopulation flag
        /*
		if (lType == DssTemplateConsolidation)
		{
			CDSSXTabConsolidation* lpCS = static_cast<CDSSXTabConsolidation*>(mvUnits[iUnit]);
			CDSSXTabConsolidation* lpOrgCS = static_cast<CDSSXTabConsolidation*>(lpTabularDataUnit.p);
			lpCS->setRepopulate(lpOrgCS->NeedRepopulate());
			lpCS->setUpdateDEIndex(lpOrgCS->NeedUpdateDEIndex());
		}
        */
        
    }
    
    //End: Populating Filtered Key For Units
    
    //Adjust each select unit ID to the new tabular data unit id
	if (nSelectedUnits > -1 )
	{
		for (Int32 i = 0; i < nSelectedUnits; i ++)
		{
			for(iUnit = 0; iUnit < iNumUnits; iUnit ++)
			{
				if (lpUnit[iUnit] == pSelectedUnitInfo[i].UnitID)
				{
					pSelectedUnitInfo[i].UnitID = iUnit; //Synchronize the unit id
					break;
				}
                
			}
            
		}
	}
    
    //construct the main index for the new index pool
    DSSTabularIndex *lpMainIndex = new DSSTabularIndex();
    hr = writeIndex(iNumUnits, lpRowForIND, lnFilteredRows, lpMainIndex);
    CHECK_HR;
    
    //if (lvDEUnits.size() > 0)
	{
		hr = hSetUnqualifiedRows(lpMainIndex, &lbvUnqulifiedRows, lpFilteredRows, lnFilteredRows);
		CHECK_HR;
	}
    
    //////////////////////////////////////////populate the metrics
    int lnMetrics = ivSliceIDs.size();
    mMetircPos = mvUnits.size();
    if (!mpMetrics)
    {
        mpMetrics = new DSSTabularMetrics();
    }
    mpMetrics->Init(mMetircPos, lnMetrics);
    mvUnitType.push_back(DssTemplateMetrics);
    mvUnitGUID.push_back(GUID_NULL);
    mvUnitGlobalIndex.push_back(-1);
    
    DSSTabularMetrics* lpOriginalMetrics = lpOriginalTabularData->getTabularMetircs();
    for (int j = 0; j < lnMetrics; ++j) 
    {
        //need to skip SFB metrics here
        DSSCubeDataSlice* lpSliceFrom = NULL;
        hr = lpSlices[j].GetSlice(&lpSliceFrom);
		CHECK_HR_PTR(lpSliceFrom);
        if (lpSliceFrom->IsSFBSlice())
            continue;
        
        MBase::CString lDSSID = DSSAEHelper::Guid2String(ipMetricIDs[j].mMetricID);
        int lMetricTranslatedTotal = DssMetricReserved;
        EnumDSSMetricType lDynamicAggFunction = DssMetricReserved;
        GUID lTotalImplGUID = GUID_NULL;
        GUID lDATotalImplGUID = GUID_NULL;
        vector<GUID> lvNonAggAttr;
        vector<EnumDSSAggregation> lvAggInfo;
        int lAvailableSystemSubtotal;
        vector<GUID> lAvailableUserDefinedSubtotal;
        int pos = lpOriginalMetrics->FindMetric(ipMetricIDs[j].mMetricID);
        if (pos != -1)
        {
            lMetricTranslatedTotal = lpOriginalMetrics->getTotalImplementation(pos);
            lDynamicAggFunction = lpOriginalMetrics->getDynamicAggFunc(pos);
            lTotalImplGUID = lpOriginalMetrics->getTotalImplID(pos);
            lDATotalImplGUID = lpOriginalMetrics->getDynamicAggID(pos);
            lpOriginalMetrics->getAggregationInfo(ipMetricIDs[j].mMetricID, lvNonAggAttr, lvAggInfo);
            lpOriginalMetrics->getAvailableSubtotals(ipMetricIDs[j].mMetricID, lAvailableSystemSubtotal, lAvailableUserDefinedSubtotal);
        }
            
        //zhyang, for the last two parameters, we just use double and reserved value
        //need to change later
        hr = mpMetrics->AddMetric(j, lDSSID, DssDataTypeDouble, lMetricTranslatedTotal, false);
        CHECK_HR;
        
        hr = mpMetrics->setDynamicAggFunc(lDynamicAggFunction);
        CHECK_HR;
        
        hr = mpMetrics->setTotalImplID(lTotalImplGUID);
        CHECK_HR;
        
        hr = mpMetrics->setDynamicAggID(lDATotalImplGUID);
        CHECK_HR;
        
        hr = mpMetrics->setAggInfo(lvNonAggAttr, lvAggInfo);
        CHECK_HR;
        
        hr = mpMetrics->setAvailableSubtotals(lAvailableSystemSubtotal, lAvailableUserDefinedSubtotal);
        CHECK_HR;
    }
    mpMetrics->setDisplayedMetricIDs(lpOriginalMetrics->getDisplayedMetricIDs());
    
    mvUnits.push_back(mpMetrics);
    mnUnits = mvUnits.size();

    mpIndexPool = new DSSTabularIndexPool();
    
    // tehe, 12/31/2011 before loading indexs, we need set the right binary keys
    int lMetricPos = getMetircPos();
    int lnBinaryKeys = getUnitCount();
    if(lMetricPos != -1)
        lnBinaryKeys -= 1;
    mpIndexPool->setBinaryKeys(lnBinaryKeys);
    
    mpIndexPool->Init(lpMainIndex->getNumUnits());
    hr = mpIndexPool->setMainIndex(lpMainIndex);
    CHECK_HR;

    //now  we need to populate the slice.
    int lRowCount = lpMainIndex->Count();
    
    for (int iSlice = 0; iSlice < lNumSlices; iSlice++)
    {
        DSSCubeDataSlice* lpSliceFrom = NULL;
        hr = lpSlices[iSlice].GetSlice(&lpSliceFrom);
		CHECK_HR_PTR(lpSliceFrom);
        
        // jxie, add rowmap from original index to level metric index to speed up writing level metric slices
        int* lpRowMap = NULL;
        {
            int lnOrgRows = 0;
            lnOrgRows = lpOriginalIndex->Count();
            
            DSSTabularIndex* lpMetricIndex = lpSliceFrom->getIndices();
            CHECK_PTR(lpMetricIndex);
            int lnMetricRows = 0;
            lnMetricRows = lpMetricIndex->Count();
            
            if (lnMetricRows != lnOrgRows && lnFilteredRows == lRowCount)
            {
                lpOriginalIndexPool->GetIndexRowMap(lpOriginalIndex, lpMetricIndex, &lpRowMap);
            }
        }
        
        DSSCubeDataSlice* lpSliceTo = NULL;
        hr = writeSlice(lpSliceFrom, iNumUnits, lpUnit,lppvRowForLUT, lnFilteredRows, lpFilteredRows, &lpSliceTo,lpBuffer, lpRowMap, lGToLMap, lGToLMap2);
        CHECK_HR;
        lpSliceTo->setSFBFlag(lpSliceFrom->IsSFBSlice());
        hr = mpMetrics -> addSlice(lpSliceTo);
		CHECK_HR;
    }
    
    return S_OK;
}

//Find out which rows at report level should be kept after fitlering.
int DSSTabularData::filterIndex(DSSTabularIndex* ipIndex, DSSCubeDataSlice *ipFilterSlice, DSSXTabBitVector *ipMask, DSSTabularData* ipOriginalTabularData, int &onFilteredRows, int* &opFilteredRows, int* &opRowForOrigInd, DSSXTabBitVector * obvUnqualifiedRows, int iFilterDEFlag, MBase::Buffer *ipBuffer)
{
    assert(ipIndex != NULL);
    assert(ipFilterSlice != NULL || ipMask != NULL);
    assert(ipBuffer != NULL);
    
    int hr = S_OK;
    
	//we will analyze which way we should do subset.
    DSSTabularIndex* lpFilterIndex = NULL;
    
    if (ipFilterSlice)
    {
        lpFilterIndex = ipFilterSlice->getIndices();
        CHECK_PTR(lpFilterIndex);
    }
    else
    {
        lpFilterIndex = ipIndex;
    }
    
    int lnFilterUnits = lpFilterIndex->getNumUnits();
    int* lpFilterUnits = new (ipBuffer) int[lnFilterUnits];
    CHECK_NEWED_PTR(lpFilterUnits);
    
    hr = lpFilterIndex->getUnits(lnFilterUnits, lpFilterUnits);
    CHECK_HR;
    
    int lnUnits = ipIndex->getNumUnits();
    int* lpUnits = new (ipBuffer) int[lnUnits];
    CHECK_NEWED_PTR(lpUnits);
    
    hr = ipIndex->getUnits(lnUnits, lpUnits);
    CHECK_HR;
    
    int lnRow = ipIndex->Count();
    
    if (ipMask)
        assert(lnRow == ipMask->GetLength());
    
    int lnFilterRow = lpFilterIndex->Count();
    
    DSSDataColumn* lpFilterCol = NULL;
    if (ipFilterSlice)
    {
        lpFilterCol = ipFilterSlice->getColumn();
        CHECK_PTR(lpFilterCol);
    }
    
    //pre-allocate filtered rows array. Trade more space with performance
    if (ipMask)
    {
        int lEstimateSetSize = ipMask->GetEstimateSetSize();
        opFilteredRows = new (ipBuffer) int[lEstimateSetSize];
    }
    else
    {
        opFilteredRows = new (ipBuffer) int[lnRow];
    }
    CHECK_PTR(opFilteredRows);
    
    int lnSize = (lnUnits == 0) ? 1 : lpUnits[lnUnits - 1] + 1;
    int* lpIndex = new (ipBuffer) int[lnSize];
    CHECK_NEWED_PTR(lpIndex);
    int iUnit = 0;
    for (iUnit = 0; iUnit < lnSize; iUnit++)
    {
        lpIndex[iUnit] = DSSXTAB_TOTAL_KEY;
    }
    
    onFilteredRows = 0;
    int iRow = 0;
    DSSDataFlag lFlag;
    double lVal = 0;
    
    if (lnFilterUnits ==  lnUnits)
    {
        //if filter slice is at same level with original tabular data level
        //then just scan filter slice itself, which is enough
        assert(std::includes(lpUnits, lpUnits+lnUnits, lpFilterUnits, lpFilterUnits+lnFilterUnits));
        assert(lnFilterRow == lnRow);
        
        if (ipMask)
        {
            ipMask->SetSeekPosition(0);
            iRow = ipMask->GetNextSetPosition();
            while (iRow >= 0 && iRow < lnRow) 
            {
                opFilteredRows[onFilteredRows++] = iRow;
                iRow = ipMask->GetNextSetPosition();
            }
        }
        else
        {
            for (iRow = 0; iRow < lnRow; iRow++)
            {
                hr = lpFilterCol->getNumericVal(iRow, lVal, lFlag);
                CHECK_HR;
                
                if (lFlag == DssDataOk && lVal == 1)
                {
                    opFilteredRows[onFilteredRows++] = iRow;
                }
            }
        }
    }
    else    //filter slice is at higher level than report level, use Partial Match
    {
        AE_ASSERT(lpFilterCol);

        hr = OldFilterIndices(ipIndex, ipFilterSlice, onFilteredRows,
                              opFilteredRows, ipBuffer, ipOriginalTabularData);
        CHECK_HR;
    }
    
    //lwang, 6/01/2007, check if there're any DEs. If so, we need get all referenced rows to re-calculate DE
	vector<Int32> lvDEUnits;
	for(iUnit = 0; iUnit < lnUnits; iUnit ++)
	{
		DSSTabularUnit* lpTabularDataUnit = NULL;
		hr = ipOriginalTabularData->Item(lpUnits[iUnit], &lpTabularDataUnit);
		CHECK_HR;
        
		DSSTemplateUnit_Type lType = lpTabularDataUnit -> getUnitType();
        
		switch(lType)
		{
            case DssTemplateConsolidation:
			{
				//check if this consolidation is actually a DE
				bool lbIsDE = static_cast<DSSTabularConsolidation *>(lpTabularDataUnit)->IsDerivedElement();
				//ctang: 694964,  for DE in join grid, at subset phase the CS unit only have leaf element, so we don't need to check the unqualify rows.
				//ctang: 695150, for DE in extension case, we won't re-populate DE, but we need update DE index.
				/*
                bool lfNeedUpdateDEIndex = static_cast<CDSSXTabConsolidation *>(lpTabularDataUnit.p)->NeedUpdateDEIndex();
				*/
                bool lfNeedUpdateDEIndex = false;//hard code
                //bool lfNeedRepopulate = static_cast<CDSSXTabConsolidation *>(lpTabularDataUnit.p)->NeedRepopulate();
                
				if (lbIsDE && !lfNeedUpdateDEIndex){
					lvDEUnits.push_back(lpUnits[iUnit]);
				}
			}
                break;
            default:
                break;
		}//end of swith(lType)
	}
    
	obvUnqualifiedRows->Init(lnRow, NULL);
    
	if (lvDEUnits.size() > 0)
    {
		Int32 lnSelectedRows = 0;
		Int32 *lpSelectedRows = NULL;
        
		hr = hGetAllSelectedIndices(ipOriginalTabularData, ipIndex, lvDEUnits, opFilteredRows, onFilteredRows,
                                    &lpSelectedRows, &lnSelectedRows, obvUnqualifiedRows, iFilterDEFlag, ipBuffer);
		CHECK_HR;
        
		onFilteredRows = lnSelectedRows;
		opFilteredRows = lpSelectedRows;
	}
    
	//Generate row map for original index
    opRowForOrigInd = new (ipBuffer) int[lnUnits * onFilteredRows];
    for (iRow = 0; iRow < onFilteredRows; iRow++)
    {
        hr = ipIndex->Item(opFilteredRows[iRow], lnSize, lpIndex);
        CHECK_HR;
        
        for(iUnit = 0; iUnit < lnUnits; iUnit++)
            opRowForOrigInd[iUnit * onFilteredRows + iRow] = lpIndex[lpUnits[iUnit]];
    }
    
    return S_OK;
}

#define FILTER_WITH_HIDDENDE 1

//if the tabular data has DE units, we need get all referenced rows by DE units in the filtered result.
// For example, if the filtered result is
//			RowNO.	 Year		DE_Country	M
//			0		 2000		USA			10
//			4		 2001       Canada      20
//			5		 2001       USA/Canada  1.5
//  suppose Year = {2000, 2001}, DE_Country = {USA, Canada, USA/Canada};
//  to calculate Row 5 {2001, USA/Canada}, we need {2001, USA} and {2001, Canada}, so we add a new row
//			3		 2001		USA			30
//	then the output oppSelectedRows will be {0,3,4,5},
//  and obvUnqualifiedRows = {000100}, where row 3 is marked as unqualified, and row4 is qualified.
int  DSSTabularData::hGetAllSelectedIndices(DSSTabularData  *lpOriginalTabularData,
                                            DSSTabularIndex *ipIndices,
                                            vector<Int32>   &ivDEUnits,
                                            Int32           *ipFilteredRows,
                                            Int32           nFilteredRows,
                                            Int32           ** oppSelectedRows,
                                            Int32           * opSelectedRowsCount,
                                            DSSXTabBitVector * obvUnqualifiedRows,
                                            Int32           iFilterDEFlag,
                                            MBase::Buffer   *ipBuffer)
{
	HRESULT hr = S_OK;
    
	AE_ASSERT(ipIndices != NULL);
	AE_ASSERT(ipBuffer != NULL);
	if (nFilteredRows > 0)
		AE_ASSERT (ipFilteredRows != NULL);
    
	AE_ASSERT(oppSelectedRows != NULL && opSelectedRowsCount != NULL && obvUnqualifiedRows != NULL);
    
	*oppSelectedRows = ipFilteredRows;
	*opSelectedRowsCount = nFilteredRows;
    
	Int32 lnDEUnits = ivDEUnits.size();
    
	if (nFilteredRows == 0 || lnDEUnits == 0)
		return S_OK;
    
	Int32 lnUnits = ipIndices -> getNumUnits();
    
	Int32 * lpUnits = new (ipBuffer)Int32[lnUnits];
	CHECK_NEWED_PTR(lpUnits);
    
	hr = ipIndices -> getUnits(lnUnits, lpUnits);
	CHECK_HR;
    
	//If lnUnit is 0, set lnTotalUnits to 1, xhan 08/06/01
	Int32 lnTotalUnits = (lnUnits == 0) ? 1 : lpUnits[lnUnits - 1] + 1;
	vector<Int32> lpUnitKeys;
	lpUnitKeys.resize(lnTotalUnits);
    
	//initialize unit key
	Int32 iUnit = 0;
	for(iUnit = 0; iUnit < lnTotalUnits; iUnit ++)
		lpUnitKeys[iUnit] = DSSXTAB_TOTAL_KEY;
    
	// get the collection of XtabConsolidation units
	vector<DSSTabularConsolidation *> lvXtabConsolidations;
	lvXtabConsolidations.resize(lnDEUnits);
    
	Int32 iDE = 0;
	for (iDE = 0; iDE < lnDEUnits; iDE++){
		if (ivDEUnits[iDE] >= lnTotalUnits)
			return E_FAIL;
        
		DSSTabularUnit* lpTabularDataUnit = NULL;
		hr = lpOriginalTabularData->Item(ivDEUnits[iDE], &lpTabularDataUnit);
		CHECK_HR;
        
		DSSTabularConsolidation* lpConsolidation = static_cast<DSSTabularConsolidation*>(lpTabularDataUnit);
		CHECK_PTR(lpConsolidation);
        
		lvXtabConsolidations[iDE] = lpConsolidation;
	}
    
	//create tags for rows in the original indices
	Int32  lnRow = ipIndices -> Count();
    
	DSSXTabBitVector lbvFilteredRows(lnRow);		//indicating which row is from the filtered Rows
	DSSXTabBitVector lbvCopiedRows(lnRow);		//indicating which row will be copied to output
	obvUnqualifiedRows->Init(lnRow, NULL);
    
	Int32 iRow = 0;
	for (iRow = 0; iRow < nFilteredRows; iRow ++) {
		lbvFilteredRows.Set(ipFilteredRows[iRow], true);
	}
    
	Int32 i = 0;
	Int32 k = 0;
	Int32 lnSelectedRows = 0;
	for (i = 0; i < nFilteredRows; i ++) {
		iRow = ipFilteredRows[i];
        
		// skip iRow if unqualified in the original tabular data or already added
		if (iFilterDEFlag != FILTER_WITH_HIDDENDE && (ipIndices->IsUnqualified(iRow) == true
                                                      || lbvCopiedRows.Get(iRow) == true))
			continue;
        
		hr = ipIndices->Item(iRow, lnTotalUnits, &lpUnitKeys[0]);
		CHECK_HR;
        
		bool lbIsHidden = false;
        
		if (iFilterDEFlag != FILTER_WITH_HIDDENDE)
		{
            for (iDE = 0; iDE < lnDEUnits; iDE++){
                Int32 lOrdinal = -1;
                hr = lvXtabConsolidations[iDE]->getOrdinal(lpUnitKeys[ivDEUnits[iDE]], &lOrdinal);
                CHECK_HR;
                
                if (lOrdinal < 0){
                    lbIsHidden = true;
                    break;
                }
            }
		}
		//if any DE's ordinal < 0(means it's not displayed), we will not copy this row and its children
		if (lbIsHidden == true)
			continue;
        
		//ctang 2011-9-20
		if (!lbvCopiedRows.Get(iRow))
		{
			lbvCopiedRows.Set(iRow, true);
			++lnSelectedRows;
		}
		//when there are DEs, we need copy the dependent rows, which are referenced by DE calculation.
		vector< vector<Int32> > lvNewKeys;
		lvNewKeys.push_back(lpUnitKeys);	//add the original key as seed
		hr = hGetDependentElementKeys(lpOriginalTabularData,ivDEUnits,lpUnitKeys,lvNewKeys);
		CHECK_HR;
        
		Int32 lRefRow;
		Int32 lnKeys = lvNewKeys.size();
		// loop start from 1, to skip the original key
		for (k = 1; k < lnKeys; k++){
			hr = ipIndices->LookUp(lnTotalUnits, &lvNewKeys[k][0], &lRefRow);
            
			if (hr == S_OK && lbvCopiedRows.Get(lRefRow) == false){
				lbvCopiedRows.Set(lRefRow, true);
				++lnSelectedRows;
                
				// set unqualified if either it's unqualified in the original tabular data, or it's not in filter result
				if (iFilterDEFlag != FILTER_WITH_HIDDENDE && (!lbvFilteredRows.Get(lRefRow) || ipIndices->IsUnqualified(lRefRow))){
					obvUnqualifiedRows->Set(lRefRow, true);
				}
			}
			else{
				// look up fails or already added, don't need add RefRow.
			}
		}//end of for (k = 0)
	}//end of for (i = 0;
    
	Int32 * lpSelectedRows = new (ipBuffer) Int32[lnSelectedRows];
	CHECK_NEWED_PTR(lpSelectedRows);
    
	for (iRow = 0, i = 0; i < lnRow; ++i){
		if (lbvCopiedRows.Get(i)){
			AE_ASSERT (iRow < lnSelectedRows);
			lpSelectedRows[iRow++] = i;
		}
	}
    
	// ldong 03/15/2012 497145 
	// in some cases, iRow != lnSelectedRows, we should use iRow here. In fact, lnSelectedRows may be double counted
	*opSelectedRowsCount = iRow;
	*oppSelectedRows = lpSelectedRows;
    
	return S_OK;
}

// input irvUnitKey is key for {97+98, Book, DC+NY}
// output: kyes for 9 rows:	{97+98, Book, DC+NY},	{97, Book, DC+NY},	{98, Book, DC+NY},
//							{97+98, Book, DC},		{97, Book, DC},		{98, Book, DC},
//							{97+98, Book, NY},		{97, Book, NY},		{98, Book, NY}
int  DSSTabularData::hGetDependentElementKeys(DSSTabularData *lpOriginalTabularData,
                                              vector<Int32> &irvDEUnits,
                                              vector<Int32> &irvUnitKey,
                                              vector< vector<Int32> > &rvNewKeys)
{
	HRESULT hr = S_OK;
    
	AE_ASSERT (lpOriginalTabularData != NULL);
    
	vector< vector<Int32> > lvOldKeys;
    
	Int32 iDE = 0;
	Int32 lnDEUnits = irvDEUnits.size();
	Int32 lnTotalUnits = irvUnitKey.size();
    
	vector<DSSTabularConsolidation *> lvXtabConsolidations;
	lvXtabConsolidations.resize(lnDEUnits);
	if (lnDEUnits > 0)
    {
		for (iDE = 0; iDE < lnDEUnits; iDE++)
        {
			DSSTabularUnit* lpTabularDataUnit = NULL;
			hr = lpOriginalTabularData->Item(irvDEUnits[iDE], &lpTabularDataUnit);
			CHECK_HR;
            
			DSSTabularConsolidation * lpConsolidation = static_cast<DSSTabularConsolidation *>(lpTabularDataUnit);
			CHECK_HR_PTR(lpConsolidation);
            
			lvXtabConsolidations[iDE] = lpConsolidation;
		}
	}
    
	//each loop, we will replace one DE unit element with its children elements
	// and generate new keys(with children).
	Int32 i = 0;
	Int32 j = 0;
	Int32 k = 0;
	for (i = 0; i < lnDEUnits; i++)
    {
		iDE = irvDEUnits[i]; // position of this derived element
        
		if ( iDE >= lnTotalUnits)
			return E_FAIL;
        
		Int32 lnElements = lvXtabConsolidations[i]->getNumAllElements();
        
		Int32 lMaxKey = 0;
		for (Int32 iRow = 0; iRow < lnElements; iRow++)
		{
			Int32 lTemp = lvXtabConsolidations[i]->TranslateLocalToGlobal(iRow);
			if (lTemp > lMaxKey)
				lMaxKey = lTemp;
		}
        
		DSSXTabBitVector lbvResolvedEles(lMaxKey + 1);
        
		vector<Int32> lElementList;
		lElementList.push_back(irvUnitKey[iDE]);		// add the original element as seed
		Int32 iCurrentEle = 0;	//first unresolved element: irvUnitKey[iDE]
        
		while (iCurrentEle < lElementList.size())
        {	// loop until every element in the list is resolved
			// For the children of current element, then add unresolved children to the end of the list.
			bool lfIsBranch;
			vector<int>* lpvLeaves = NULL;
			hr = lvXtabConsolidations[i]->GetLeavesByElement(lElementList[iCurrentEle],
                                                             &lfIsBranch,
                                                             lpvLeaves);
			CHECK_HR;
            
			lbvResolvedEles.Set(lElementList[iCurrentEle], true);
            
			if (lfIsBranch == true)
            {
				Int32 lnLeaves = (*lpvLeaves).size();
                
				Int32 iLeaf = 0;
				for (iLeaf = 0; iLeaf < lnLeaves; iLeaf++)
                {
					//ctang 2010-11-9 translate to global
					Int32 iEle = lvXtabConsolidations[i]->TranslateLocalToGlobal((*lpvLeaves)[iLeaf]);
                    
					if (iEle < 0)	//lwang, 10/20/2008, TQMS 333545
						continue;
					//if (lvXtabConsolidations[i]->IsSmart() == false &&
					if (lbvResolvedEles.Get(iEle) == false)
                    {
						lElementList.push_back(iEle);
					}
				}
			}// end of if not branch
			iCurrentEle++;
		}//end of while
        
		//now we have the children element list, generate new keys
		if (lElementList.size() > 1)
        {
			//copy new keys to old keys
			lvOldKeys.clear();
			lvOldKeys.assign(rvNewKeys.begin(), rvNewKeys.end());
            
			for (j = 1; j < lElementList.size(); ++j)
            {	// loop starts from 1, skip the original element
                // so that rvNewKeys won't have duplicated keys
				Int32 iEle = lElementList[j];
				for (k = 0; k < lvOldKeys.size(); ++k)
                {
					//generate new key
					vector<Int32> lpKey = lvOldKeys[k];
					lpKey[iDE] = iEle;
                    
					rvNewKeys.push_back(lpKey);
				}
			}
		}//end of if
	}//end of for (i = 0; i < lnDEUnits; i++)
    
	return S_OK;
}

int DSSTabularData::writeIndex(int iUnit, int* ipRowForIND, int iRow, DSSTabularIndex* ipIndex)
{
    int lnRow = 0;
    if (iUnit > 0)
    {
        assert(ipRowForIND != NULL);
        lnRow = iRow;
    }
    else if (iRow != 0)
    {
        lnRow = 1;  // Xhan 05/17/01 suppose report at least has one row because there must be
                    // a metric unit in this case.
    }
    int* lpMap = new int[iUnit*lnRow];
    CHECK_NEWED_PTR(lpMap);
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lMapPtr(lpMap);
    
    int* lpUnit = new int[iUnit];
    CHECK_NEWED_PTR(lpUnit);
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lUnitPtr(lpUnit);
    
    for(int lUnit = 0; lUnit < iUnit; lUnit++)
    {
        lpUnit[lUnit] = lUnit;
        for(int lRow = 0; lRow < lnRow; lRow++)
            lpMap[lRow * iUnit + lUnit] = ipRowForIND[lUnit * iRow + lRow];
    }
    
    int hr = ipIndex->Init(iUnit, lnRow, lpUnit, lpMap, NULL);
    CHECK_HR;
    
    return S_OK;
}

int DSSTabularData::writeSlice(DSSCubeDataSlice *ipFrom, int inUnit, int* ipUnit, vector<Int32, MBase::Allocator<Int32> > **ppvRowForLUT, int iFilteredRows, int* ipFilteredRows, DSSCubeDataSlice** oppTo, MBase::Buffer *pBuffer,int* ipRowMap, Int32** ppGToLMap, IntToIntMap** ppGToLMap2)
{
    assert(ipFrom != NULL);
    assert(oppTo != NULL);
    
    int hr = S_OK;
    
    DSSTabularIndex* lpIndicesFrom = ipFrom->getIndices();
    CHECK_PTR(lpIndicesFrom);
    
    int lnUnit = lpIndicesFrom->getNumUnits();
    int* lpUnitFrom = new (pBuffer) int[lnUnit];
    CHECK_NEWED_PTR(lpUnitFrom);
    hr = lpIndicesFrom->getUnits(lnUnit, lpUnitFrom);
    CHECK_HR;
    
    int* lpUnitTo = new (pBuffer) int[lnUnit];
    CHECK_NEWED_PTR(lpUnitTo);
    
    int iUnit = 0, iUnit2 = 0;
    for (iUnit = 0; iUnit < lnUnit; iUnit++)
    {
        for (iUnit2 = 0; iUnit2 < inUnit; iUnit2++)
            if (ipUnit[iUnit2] == lpUnitFrom[iUnit])
                break;
        if (iUnit2 == inUnit)
            assert(false);
        else
            lpUnitTo[iUnit] = iUnit2;
    }

    int* lpMetric2MainIndex = NULL;
    DSSTabularIndexPool* lpIndexPool = getIndexPool();
    CHECK_PTR(lpIndexPool);
    DSSTabularIndex* lpMainIndex = lpIndexPool->getMainIndex();
    
    if (ipRowMap)
    {
        DSSTabularIndex* lpMetricIndex = NULL;
        hr = lpIndexPool->getIndex(lnUnit, lpUnitTo, &lpMetricIndex);
        CHECK_HR_PTR(lpMetricIndex);
        
        int* lpMainIndex2Metric = NULL;
        lpIndexPool->GetIndexRowMap(lpMainIndex, lpMainIndex, &lpMainIndex2Metric);
        
        if (lpMainIndex2Metric)
        {
            if (lpMainIndex2Metric == XTAB_SAME_ROWMAP || lpMainIndex2Metric == XTAB_CONSTANT_ROWMAP)
                lpMetric2MainIndex = lpMainIndex2Metric;
            else
            {
                int lnMetricRows = 0;
                lnMetricRows = lpMetricIndex->Count();
                int lnTotalRows = 0;
                lnTotalRows = lpMainIndex->Count();
                
                lpMetric2MainIndex = new (pBuffer) int[lnMetricRows];
                memset(lpMetric2MainIndex, -1, sizeof(int) * lnMetricRows);
                
                int lnCountRows = 0;
                for (int i = 0; i < lnTotalRows && lnCountRows < lnMetricRows; i++)
                {
                    int lRow = lpMainIndex2Metric[i];
                    if (lpMetric2MainIndex[lRow] == -1)
                    {
                        lpMetric2MainIndex[lRow] = i;
                        lnCountRows++;
                    }
                }
            }
        }
    }
    
    DSSTabularIndex *lpIndicesTo;
    hr = lpIndexPool->getIndex(lnUnit, lpUnitTo, &lpIndicesTo);
    CHECK_HR;
    int lnRowTo = lpIndicesTo->Count();
    
    DSSDataColumn* lpColumnFrom = ipFrom->getColumn();
    CHECK_PTR(lpColumnFrom);
    
    DSSDataColumn *lpColumnTo = NULL;
    EnumDSSDataType lDataType = lpColumnFrom->getDataType();
    double lDivider = lpColumnFrom->getDivider();
    if (lDivider > 1)
    {
        lDataType = DssDataTypeDouble;
    }
    hr = DSSAEHelper::GenXTabCol(lDataType, lnRowTo, &lpColumnTo);
    CHECK_HR_PTR(lpColumnTo);
    
    // preserve the original type
	lpColumnTo->setOriginalType(lpColumnFrom->getOriginalType());
    
    int lnUnitFrom = (lnUnit == 0) ? 0 : (lpUnitFrom[lnUnit-1]+1);
    int* lpIndexFrom = new (pBuffer) int[lnUnitFrom];
    CHECK_NEWED_PTR(lpIndexFrom);
    
    int* lpIndexTo = new (pBuffer) int[lnUnitFrom];
    CHECK_NEWED_PTR(lpIndexTo);
    
    int iRow = 0;
    DSSDataFlag lFlag;
    
    int lRowSize = 0;
    hr = ipFrom->getSizeOfData(lRowSize);
    CHECK_HR;
    
    char* lpData = new (pBuffer) char[lRowSize];
    CHECK_NEWED_PTR(lpData);
    
    int lMainIndexRowCount = lpMainIndex->Count();
    
    if (lnUnit > 0) //normal case, attribute unit number is at least 1
    {
        //optimazation here
		// aye, 5/22/2009, if we are sure that the order of data are the same, we will directly copy
		// The order are the same if:
		// 1) the IndexTo is at the view tabulardata base level (as the index is generated by direct translation), or
		// 2) the IndexTo has the same number of rows as the view tabulardata base level (as we keep the order in this case)
        DSSTabularIndex* lpIndex1 = ipFrom->getIndices();
        CHECK_PTR(lpIndex1);
        
        bool lIsFromLowestLevel = lpIndex1->getKeyTable() == NULL;
        bool lIsMainIndexOrder = lpIndex1->IsMainIndexOrder();
        
        if (lnUnit == inUnit || 
            (lIsFromLowestLevel && lIsMainIndexOrder && lMainIndexRowCount == lnRowTo && lnUnit != 1))// filtered slice level is at the report level,
        {
            //no index mapping is necessary
            
            lpColumnFrom = ipFrom->getColumn();
			CHECK_PTR(lpColumnFrom);
            
			DSSXTabBitVector* lpMask = ipFrom->GetMask();
            
            int lSrc = 0;
            assert(lnRowTo == iFilteredRows);
            for (iRow = 0; iRow < lnRowTo; iRow++)
            {
                lSrc = ipFilteredRows[iRow];
                
                // for conditional metric, skip filtered rows
				if (lpMask && lpMask->Get(lSrc) == 0)
				{
					lpColumnTo->setFlag(iRow, DssDataInvalid);
					continue;
				}
                
                hr = lpColumnFrom->getData(lSrc, (const char**)(&lpData), lRowSize, lDataType, lFlag);
                CHECK_HR;
                
                hr = lpColumnTo->setData(iRow, lpData, lRowSize, lFlag);
                CHECK_HR;
            }
        } 
        else if (ipRowMap && lpMetric2MainIndex && ipFilteredRows)
        {
            //the filtered slice level is higher than report level
            //use the row map to bypass index mapping
            lpColumnFrom = ipFrom->getColumn();
			CHECK_PTR(lpColumnFrom);
            
			DSSXTabBitVector* lpMask = ipFrom->GetMask();
            
            int lSrc = 0;
            for (iRow = 0; iRow < lnRowTo; iRow++)
            {
                //the map is like this:
                //  MainIndex1  --->    MainIndex2
                //      |                   |
                // level metric1        level metric2
                
                //1st map: level metric2 to MainIndex2
                lSrc = XTabIndexRowMapLookup(lpMetric2MainIndex, iRow);
                
                //2nd map: MainIndex2 to MainIndex1
                lSrc = ipFilteredRows[lSrc];
                
                //3rd map: MainIndex1 to level metric1
                lSrc = XTabIndexRowMapLookup(ipRowMap, lSrc);
                
                // for conditional metric, skip filtered rows
				if (lpMask && lpMask->Get(lSrc) == 0)
				{
					lpColumnTo->setFlag(iRow, DssDataInvalid);
					continue;
				}

                hr = lpColumnFrom->getData(lSrc, (const char**)(&lpData), lRowSize, lDataType, lFlag);
                CHECK_HR;
                hr = lpColumnTo->setData(iRow, lpData, lRowSize, lFlag);
                CHECK_HR;
            }
        }
        else
        {
            //the filtered slice level is higher than report level
            //we have to do index mapping
            for(iRow = 0; iRow < lnRowTo; iRow ++)
			{
				hr = lpIndicesTo -> Item(iRow,
                                         lnUnitFrom,
                                         lpIndexTo);
				CHECK_HR;
                
				Int32 iUnit = 0;
				for(iUnit = 0; iUnit < lnUnit; iUnit ++)
				{
					Int32 lUnit = lpUnitTo[iUnit];
					Int32 lGKey = lpIndexTo[lUnit];
					if (ppvRowForLUT[lUnit] != NULL)
					{
						lpIndexFrom[lpUnitFrom[iUnit]] = (*(ppvRowForLUT[lUnit]))[lGKey];
					}
					else
					{
						if (ppGToLMap[lUnit])
						{
							lpIndexFrom[lpUnitFrom[iUnit]] = ppGToLMap[lUnit][lGKey];
						}
						else if (ppGToLMap2[lUnit])
						{
							lpIndexFrom[lpUnitFrom[iUnit]] = (*(ppGToLMap2[lUnit]))[lGKey];
						}
						else
						{
							lpIndexFrom[lpUnitFrom[iUnit]] = lGKey;
						}
					}
				}
                
				hr = ipFrom -> getVal(
                                      lnUnitFrom,
                                      lpIndexFrom,
                                      (const char **)&lpData,
                                      lRowSize,
                                      //&lcbWritten,
                                      lDataType,
                                      lFlag);
				CHECK_HR;
                
				hr = lpColumnTo -> setData(iRow,
                                           lpData,
                                           //lcbWritten,
                                           lRowSize,
                                           lFlag);
				CHECK_HR;
                
			}
        }
    }
    else
    {
        //only has one metric unit, lnRowTo should be one.
        if (lnRowTo == 1)
        {
            if (iFilteredRows == 0) //filtered out, return no data
                return S_OK;
            
            hr = lpColumnFrom->getData(0, (const char**)(&lpData), lRowSize, lDataType, lFlag);
            CHECK_HR;
            
            hr = lpColumnTo->setData(0, lpData, lRowSize, lFlag);
            CHECK_HR;
        }
    }

    *oppTo = new DSSCubeDataSlice();
    hr = (*oppTo)->Init(lpColumnTo, lpIndicesTo);
    CHECK_HR;
    return S_OK;
    
}

int DSSTabularData::createReferenceUnit(EnumDSSTemplateUnitType iType, 
                                        int iIndex, 
                                        DSSTabularUnit* ipUnit, 
                                        DSSTabularData *ipOriginalTabularData, 
                                        DSSTabularUnit** oppUnit, 
                                        bool ifDEonDEElementBrowse,
                                        //ctang 2010-11-5
                                        bool ifGlobalDE,
                                        //ctang 2011-4-26
                                        bool ifNoChildPopulate)
{
    int hr = S_OK;
    GUID lUnitGUID;
    hr = ipUnit->getGUID(lUnitGUID);
    CHECK_HR;
    if (iIndex == -1) {
        MBase::CString lGUIDStr = DSSAEHelper::Guid2String(lUnitGUID);
        int lIndex = mvUnits.size();
        hr = hCreateDummyUnit(lIndex, iType, lGUIDStr, oppUnit);
        CHECK_HR;
    }
    else if (iType == DssTemplateAttribute)
    {
        DSSTabularAttribute* lpTabularAttribute = NULL;
        int lUnitIndex;
        hr = mpGlobalLookupContainer->getGlobalLookupByID(&lUnitGUID, &lUnitIndex, &lpTabularAttribute);
        CHECK_HR;
        
        DSSGlobalLookUpTable* lpLookUp = dynamic_cast<DSSGlobalLookUpTable*>(lpTabularAttribute);        
        DSSXTabRefLookupTable* lpRefLookup = new DSSXTabRefLookupTable;
        int hr = lpRefLookup->Init(static_cast<DSSTabularAttribute*>(lpLookUp));
        CHECK_HR;
        *oppUnit = lpRefLookup;
    } 
    else if (iIndex >= 0)
    {
        if (iType == DssTemplateConsolidation)
        {
            DSSTabularAttribute* lpTabularAttribute = NULL;
            int lUnitIndex;
            hr = mpGlobalLookupContainer->getGlobalLookupByID(&lUnitGUID, &lUnitIndex, &lpTabularAttribute);
            if (hr == S_FALSE)  // not found in mvpGlobalLookUps, may be a real consolidation
            {
                DSSTabularUnit* lpUnit = NULL;
                hr = mpGlobalLookupContainer->getUnitFromGUnitMapByID(lUnitGUID, &iType, iIndex, &lpUnit);
                CHECK_HR;
                DSSTabularConsolidation* lpConsolidation = static_cast<DSSTabularConsolidation*>(lpUnit);
                
                DSSTabularUnit* lpChildUnit = lpConsolidation->getChildUnit();
                EnumDSSTemplateUnitType lChildUnitType = lpChildUnit->getUnitType();
                int lChildUnitIndex = lpChildUnit->getUnitIndex();
                if (ifNoChildPopulate)
                {
                    //TODO
                }
                else
                {
                    DSSTabularUnit* lpRefChildUnit = NULL;
                    hr = createReferenceUnit(lChildUnitType, lChildUnitIndex, lpChildUnit, ipOriginalTabularData, &lpRefChildUnit);
                    CHECK_HR_PTR(lpRefChildUnit);
                    mvUnits.push_back(lpRefChildUnit);
                    DSSTabularRefConsolidation* lpRefCS = new DSSTabularRefConsolidation();
                    int hr = lpRefCS->Init(static_cast<DSSTabularConsolidation*>(lpConsolidation));
                    CHECK_HR;
                    *oppUnit = lpRefCS;
                }
            } else
            {
                CHECK_HR;
                //is a DE
                //DSSGlobalLookUpTable* lpLookUp = dynamic_cast<DSSGlobalLookUpTable*>(lpTabularAttribute);
            }
        }
    } 
    else
    {
        MBase::CString lGUIDStr = DSSAEHelper::Guid2String(lUnitGUID);
        int lIndex = mvUnits.size();
        hr = hCreateDummyUnit(lIndex, iType, lGUIDStr, oppUnit);
        CHECK_HR;
    }
    
    return S_OK;
}

int DSSTabularData::createReferenceUnitI(EnumDSSTemplateUnitType iType, 
                         int iIndex, 
                         DSSTabularUnit* ipUnit, 
                         DSSTabularData *ipOriginalTabularData, 
                         DSSTabularUnit** oppUnit, 
                         bool ifDEonDEElementBrowse,
                         //ctang 2010-11-5
                         bool ifGlobalDE,
                         //ctang 2011-4-26
                         bool ifNoChildPopulate)
{
    HRESULT hr = S_OK;
    
	// weiwang 2010-12-23 Check if the passed in unit is ref unit or not
	DSSTabularUnit* lpTabularDataUnit = NULL;
	DSSTabularUnit* lpTabularDataUnitC = static_cast<DSSTabularUnit*>(ipUnit);
	hr = lpTabularDataUnitC->getTabularUnit(&lpTabularDataUnit);
	CHECK_HR_PTR(lpTabularDataUnit);
	DSSTabularUnit* lpTabularDataUnitC2 = static_cast<DSSTabularUnit*>(lpTabularDataUnit);
    
	switch (iType) {
        case DssTemplateAttribute:
		{
			DSSXTabRefLookupTable* lpAttribute = new DSSXTabRefLookupTable();
			CHECK_NEWED_PTR(lpAttribute);
            
            
			DSSTabularAttribute* lpXTabLookUpTable = static_cast<DSSTabularAttribute*>(lpTabularDataUnitC2);
			hr = lpAttribute->Init(lpXTabLookUpTable);
			CHECK_HR;
            lpAttribute->setUnitIndex(iIndex);
			*oppUnit = lpAttribute;
            
		}
            break;
            
        case DssTemplateConsolidation:
		{
			DSSTabularRefConsolidation* lpRefConsolidation = new DSSTabularRefConsolidation();
			CHECK_NEWED_PTR(lpRefConsolidation);
            
			DSSTabularConsolidation* lpXTabConsolidation = static_cast<DSSTabularConsolidation*>(lpTabularDataUnitC);
			DSSTabularConsolidation* lpXTabConsolidation2 = static_cast<DSSTabularConsolidation*>(lpTabularDataUnitC2);
            
			//Get the child unit of the consolidatin unit,
			//And create the reference unit for child first and then put it into the tabluar data and
			//the newly created reference consolidation unit
			/*
            Int32 lnUnitInCSLD = 0;
			hr = lpXTabConsolidation->get_NumUnits(&lnUnitInCSLD);
			CHECK_HR;
            
			AE_ASSERT(lnUnitInCSLD == 1); // right now in the implementation, the number of child unit must be one.
            */
			//get the unit child index
			Int32 lChildUnitIndex;
            hr = ipOriginalTabularData->FindUnitI(lpTabularDataUnitC->getGUID(), DssTemplateCustomGroup, lChildUnitIndex);
			/*
            hr = lpXTabConsolidation->get_Units(1, &lChildUnitIndex);
			*/
            CHECK_HR;
            
			//ctang 2010-11-5 in global DE case, we don't need to creat RefCG
			if (ifGlobalDE)
			{
				// jxie, 2012-8-10, TQMS 643791, memory leak fix
				// if this ref CS unit is initialized inside the same tabularData, no need to add reference for it
				/*
                hr = lpRefConsolidation->init(lpXTabConsolidation, index, lChildUnitIndex, (ipOriginalTabularData == this) ? true : false);
                 */
                hr = lpRefConsolidation->Init(lpXTabConsolidation);
				CHECK_HR;
			}
			else
			{
				AE_ASSERT(ipOriginalTabularData);
                
				DSSTabularUnit* lpChildUnit = NULL;
				hr = ipOriginalTabularData->Item(lChildUnitIndex, &lpChildUnit);
				CHECK_HR_PTR(lpChildUnit);
                
				if (lpXTabConsolidation != lpXTabConsolidation2)	// passed in is ref unit
				{
					DSSTabularUnit* lpChildUnit2 = NULL;
					hr = static_cast<DSSTabularUnit*>(lpChildUnit)->getTabularUnit(&lpChildUnit2);
					CHECK_HR_PTR(lpChildUnit2);
                    
					lpChildUnit = lpChildUnit2;
				}
                
				DSSTemplateUnit_Type lChildUnitType = lpChildUnit->getUnitType();
                
				if (ifNoChildPopulate)
				{
					DSS_ID lID;
					hr = lpChildUnit->getGUID(lID);
					CHECK_HR;
                    
					Int32 lIndex;
					hr = ipOriginalTabularData->FindUnitI(lID, lChildUnitType, lIndex);
					CHECK_HR;
					
					// jxie, 2012-8-10, TQMS 643791, memory leak fix
					// if this ref CS unit is initialized inside the same tabularData, no need to add reference for it
					hr = lpRefConsolidation->Init(lpXTabConsolidation2);//, index, lIndex, (ipOriginalTabularData == this) ? true : false);
					CHECK_HR;
				}
                
				else
				{
					//the index is the last of the mvUnits
					DSSTabularUnit* lpRefChildUnit = NULL;
					Int32 lSubsetChildUnitIndex = mvUnits.size();
					hr = createReferenceUnitI(lChildUnitType, lSubsetChildUnitIndex, lpChildUnit, NULL, &lpRefChildUnit);
					CHECK_HR_PTR(lpRefChildUnit);
                    
					///////////////////////////////////////
                    mvUnitType.push_back(lChildUnitType);
                    mvUnitGUID.push_back(lpChildUnit->getGUID());
                    mvUnitGlobalIndex.push_back(-1);
					mvUnits.push_back(lpRefChildUnit);
                    ///////////////////////////////////////
                    
					// jxie, 2012-8-10, TQMS 643791, memory leak fix
					// if this ref CS unit is initialized inside the same tabularData, no need to add reference for it
					hr = lpRefConsolidation->Init(lpXTabConsolidation2);//, index, lSubsetChildUnitIndex, (ipOriginalTabularData == this) ? true : false);
                    CHECK_HR;
                    lpRefConsolidation->setUnitIndex(iIndex);
				}
			}
            
			*oppUnit = lpRefConsolidation;
            
		}
            break;
            
        case DssTemplateCustomGroup:
		{
			// ldong 10/07/2011 special handling for element browsing of an DE unit on DT
			// in element browsing, we need to provide the original elements rather than the DE elements
			// so we need to provide the original Attribute unit rather than the CustomGroup unit
			if (ifDEonDEElementBrowse)
			{
				// get the first LookupTable from the CustomGroup unit
				//DSSTabularCustomGroup *lpXTabCustomGroup = static_cast<DSSTabularCustomGroup *> (ipUnit);
                
				DSSTabularAttribute* lpLookUpTable = NULL;
				/*
                hr = lpXTabCustomGroup -> get_LookUpTable(0, &lpLookUpTable); // the first one
				CHECK_HR_PTR(lpLookUpTable);
                */
				// create reference LookUpTable unit 
				DSSXTabRefLookupTable * lpAttribute = new DSSXTabRefLookupTable();
				CHECK_NEWED_PTR(lpAttribute);
                
				//CDSSXTabLookUpTable *lpXTabLookUpTable = static_cast<CDSSXTabLookUpTable *> (lpLookUpTable.p);
				hr = lpAttribute->Init(lpLookUpTable);
				CHECK_HR;
                
				*oppUnit = lpAttribute;
			}else
			{
				// the normal logic
				DSSTabularRefCustomGroup * lpRefCustomGroup = new DSSTabularRefCustomGroup();
				CHECK_NEWED_PTR(lpRefCustomGroup);
                
                
				// jxie, 2012-8-10, TQMS 643791, memory leak fix
				// if this ref CG unit is initialized inside the same tabularData, no need to add reference for it
				DSSTabularCustomGroup* lpXTabCustomGroup = static_cast<DSSTabularCustomGroup*>(lpTabularDataUnitC2);
				hr = lpRefCustomGroup->Init(lpXTabCustomGroup);//, index, (ipOriginalTabularData == this) ? true : false);
				CHECK_HR;
                lpRefCustomGroup->setUnitIndex(iIndex);
				*oppUnit = lpRefCustomGroup;
			}
		}
            break;
        default:
            AE_ASSERT(false);
	}

	return S_OK;
}

int DSSTabularData::hSetUnqualifiedRows(DSSTabularIndex *ipIndices,
                                        DSSXTabBitVector * ibvUnqualifiedRows,
                                        Int32 *ipFilteredRows,
                                        Int32 nFilteredRows)
{
	HRESULT hr = S_OK;
    
	AE_ASSERT(ibvUnqualifiedRows != NULL);
    
	DSSTabularIndex *lpIndices = static_cast<DSSTabularIndex *> (ipIndices);
	CHECK_HR_PTR(lpIndices);
    
	Int32 lnRows = ibvUnqualifiedRows->GetLength();
    
	Int32 iRow = 0;
	for (iRow = 0; iRow < nFilteredRows; ++iRow){
		AE_ASSERT( ipFilteredRows[iRow] < lnRows);
        
		if (ibvUnqualifiedRows->Get(ipFilteredRows[iRow]) == true){
			lpIndices->SetUnqualified(iRow, true);
		}
	}
    
	return S_OK;
}

int DSSTabularData::SubsetPopulate(
                   DSSTabularData *ipTabularData,
                   Int32 NumUnits, UnitFormInfo *pUnitInfo,
                   Int32 NumMissingUnit, DSS_ID *ipMissingUnitIDs,
                   UnitFormInfo *ipMissingUnitFormInfo,
                   Int32 NumSlices, DSSCubeDataSlice **ipSlices, METRIC_ID* ipMetricIDs, 
                   //EnumDSSDataShareLevel	iDataShareLevel,
                   VARIANT_BOOL IsUnitSame,
                   Int32 nRowLimit,
                   Int32 NumDerivedElements,
                   Int32 *ipDerivedElementUnits,
                   //ICDSSSource *ipSource,
                   bool iRestoreDouble,
                   Int32 inElementBrowseCount,
                   Int32* ipElementBrowseList,
                   bool ifDEonDTElementBrowse,
                   bool ibHasFilter,
                   DSSGUnitContainer* ipGUContainer,
                   bool ibRWDSubsetFromBase,
                   //ctang 2010-8-25 add relation to support extend populate
                   DSSRelation *ipRelation, DSS_ID* iExtendAttribtue,
                   Int32 inJoinAttribute, 
                   //*ctang: for UC on base de case*/  DSS_ID iViewTemplateID, 
                   //ICDSSReportInstance* ipRI,
                   //ctang 2012-7-17
                   bool* ipConflictMetric,
                   //thuang 2012-11-21 for MDX or DI Attr
                   MCESchema* ipSchema,
                   bool ifRePopulateDEInExtension,
                   bool IsOfflineDDIC)
{
    HRESULT hr = S_OK;
	
    AE_ASSERT(ipTabularData != NULL);
    
	//ctang: this is a view use global lookup, mark the GUnitContainer
	mpGlobalLookupContainer = ipGUContainer;
    
	//xhan 01/05/05 if there is no dynamic aggregation happened, we need to put the dummy unit to the
	// view tabular data
	Int32 lOrgDummyUnitIndex = -1;
	DSSTabularData *lpCOrgTabularData = ipTabularData;    
    
    mDataSetGUID = lpCOrgTabularData->getDataSetGUID();
	
    if (IsUnitSame == VARIANT_TRUE) {
		lpCOrgTabularData->getDummyUnitIndex(&lOrgDummyUnitIndex);
	}
    
	// first need to make sure the list of units is ordered
	Int32 lpUnit[NumUnits + 1];
	CHECK_NEWED_PTR(lpUnit);
    
	if (NumUnits > 0)
		AE_ASSERT(pUnitInfo != NULL);
    
	Int32 iUnit = 0;
	for (iUnit = 0; iUnit < NumUnits; iUnit ++)
		lpUnit[iUnit] = pUnitInfo[iUnit].UnitID;
    
	Int32 lnInputUnit = NumUnits;
    
	// jxie, 2010-8-12, don't add dummy unit if it is element browsing
	// ldong 4/24/2012 add DE on DT element browsing check, 583715
	if ((lOrgDummyUnitIndex != -1) && (ipElementBrowseList == NULL) && (!ifDEonDTElementBrowse))
		lpUnit[NumUnits ++] = lOrgDummyUnitIndex;
    
	sort(lpUnit, lpUnit + NumUnits);
    
	//ctang 2011-4-26 to make the unitinfo correct, we need the map between lpUnit and pUnitInfo
	Int32 lpUnitInfoMap[NumUnits];
	CHECK_NEWED_PTR(lpUnitInfoMap);
	memset(lpUnitInfoMap, -1, sizeof(Int32) * NumUnits);
    
	for (Int32 i = 0; i < NumUnits; i++)
		for (Int32 j = 0; j < lnInputUnit; j++)
		{
			if (lpUnit[i] != lOrgDummyUnitIndex && lpUnit[i] == pUnitInfo[j].UnitID)
				lpUnitInfoMap[i] = j;
		}
    
	//maybe we need any duplication
    
	// make sure the given slice are at least aggregate to the
	// level of the remaining attribute
	if (NumSlices > 0)
		AE_ASSERT(ipSlices != NULL);
    
	Int32 iSlice = 0;
	for(iSlice = 0; iSlice < NumSlices; iSlice ++)
	{
		//check if all the slice's level is compatible with the selection
        DSSTabularIndex* lpIndex = ipSlices[iSlice] ->getIndices();
        CHECK_PTR(lpIndex);
        
		Int32 lnUnit2 = lpIndex -> getNumUnits();
        
		Int32 lpUnit2[lnUnit2];
		CHECK_NEWED_PTR(lpUnit2);
        
		hr = lpIndex -> getUnits(lnUnit2, lpUnit2);
		CHECK_HR;
        
		if (!std::includes(lpUnit, lpUnit + NumUnits, lpUnit2, lpUnit2 + lnUnit2))
			return hr = E_FAIL;		//the slice is not appropriate
	}
    
	// yma 1/9/06 consider the DerivedElements
	//begin to copy units
	// ctang 2010-8-25 extend case
	Int32 lnRelationUnits = 0;
    
	int lUnitSize = 0;
	if (!ipRelation)
        lUnitSize = NumUnits + NumMissingUnit + NumDerivedElements;
	else
	{
		hr = ipRelation->get_NumUnits(&lnRelationUnits);
		CHECK_HR;
        
        lUnitSize = NumUnits + NumMissingUnit + NumDerivedElements + lnRelationUnits - inJoinAttribute;
	}
    mvUnits.resize(lUnitSize);
    mvUnitGUID.resize(lUnitSize);
    mvUnitType.resize(lUnitSize);
    mvUnitGlobalIndex.resize(lUnitSize);

    
	//ctang 2011-2-12 find the corresponding join attribtue in relation
	vector<DSS_ID> lvRelationUnitID(lnRelationUnits);
	DSSXTabBitVector lMask(lnRelationUnits);
    
	vector<Int32> lvJoinAttributesOrder;
	if (ipRelation)
	{
		hr = ipRelation->get_AllIDs(lnRelationUnits, &lvRelationUnitID[0]);
		CHECK_HR;
        
		for (Int32 i = 0; i < lvRelationUnitID.size(); i++)
		{
			for (Int32 j = 0; j < inJoinAttribute; j++)
				if (ATL::InlineIsEqualGUID(lvRelationUnitID[i], iExtendAttribtue[j]))
				{
					lMask.Set(i, true);
					lvJoinAttributesOrder.push_back(j);
				}
		}
		AE_ASSERT(lvJoinAttributesOrder.size() == inJoinAttribute);
	}
    
    
    
	Int32 lpDEUnits[NumDerivedElements];
	CHECK_NEWED_PTR(lpDEUnits);
    
	// when the unit is custom group, set to true
	// When the unit is consolidation, set to false
	bool lpDEUnitFlags[NumDerivedElements];
	CHECK_NEWED_PTR(lpDEUnitFlags);
    
    
	// aye, 8/15/2008
	// get the index, which may be used in populating the attribute
    
	DSSTabularIndexPool *lpIndexPool = ipTabularData->getIndexPool();
	CHECK_PTR(lpIndexPool);
    
	DSSTabularIndex *lpOriginalIndices = NULL;
	//ctang 2011-4-26 if fake CG is in main index, remove the CS
	Int32 lnMainIndexUnit = 0;
	Int32 lpMainIndexUnit[NumUnits];
	CHECK_NEWED_PTR(lpMainIndexUnit);
    
	for (Int32 i = 0; i < lnInputUnit; i++)
	{
		if (!pUnitInfo[i].mfNoChildPopulate)
			lpMainIndexUnit[lnMainIndexUnit++] = pUnitInfo[i].UnitID;//ctang 595933, use the correct unit ID
	}
	//ctang 2011-5-10 consider dummy unit
	if (lOrgDummyUnitIndex != -1 && ipElementBrowseList == NULL)
	{
		AE_ASSERT(lnInputUnit == NumUnits - 1);
		lpMainIndexUnit[lnMainIndexUnit++] = lOrgDummyUnitIndex;
	}
    
    if (IsOfflineDDIC)
    {
        DSSTabularIndex* lpOriginalBaseIndex = lpIndexPool->getMainIndex();
        CHECK_PTR(lpOriginalBaseIndex);
        
        vector<int> lvUnits;
        lvUnits.resize(lnMainIndexUnit);
        for (int i = 0; i < lnMainIndexUnit; i++)
        {
            lvUnits[i] = lpMainIndexUnit[i];
        }
        lpOriginalIndices = new DSSTabularIndex();
        hr = lpOriginalBaseIndex->GenDistinctC(lvUnits, lpOriginalIndices, NULL);
        CHECK_HR;
    }
    else
    {
        hr = lpIndexPool->getIndex(lnMainIndexUnit, lpMainIndexUnit, &lpOriginalIndices);
    }
	
	// ldong 6/21/2012 617129
	// for DEonDTElementBrowse
	// 1, if no  filter, lbShareWithBase=true, tabular data has index for {CG} & {CS}, CG unit is passed in, we can get CG index. 
	// 2, if has filter, lbShareWithBase=false,have index {CS}, don't have index {CG}, CS unit is passed in, will do special handling later to get {CG} index
	// 3, if tabular data is obtained after a ML, it is similar as that after filter: have index {CS}, don't have index {CG}.
	// But lbShareWithBase=true and CG unit is passed in, which is similar as case 1 "no filter". so lpOriginalIndices will be NULL
	// We need to make this case same as case 2 "has filter".
	if (hr == E_FAIL)
	{
		if (ifDEonDTElementBrowse)// && (iDataShareLevel >= DSSDataShareUnit))
		{
			// should be case 3, CG unit is passed in, but no index {CG}. get CS unit now
			DSSTabularUnit* lpTabularDataUnit = NULL;
			hr = ipTabularData -> Item(lpUnit[0], &lpTabularDataUnit);
			CHECK_HR;
            
			DSSTemplateUnit_Type lType = lpTabularDataUnit -> getUnitType();
			AE_ASSERT(lType == DssTemplateCustomGroup); // should be CG
            
			// get CS unit and reset iDataShareLevel
			DSS_ID lID = GUID_NULL;
			hr = lpTabularDataUnit->getGUID(lID);
			CHECK_HR;
			
			Int32 lUnitIndex = -1;
			hr = ipTabularData->FindUnitI(lID, DssTemplateReserved/*type not important*/  , lUnitIndex);
			CHECK_HR;
			
			hr = ipTabularData -> Item(lUnitIndex, &lpTabularDataUnit);
			CHECK_HR;
			
			lType = lpTabularDataUnit -> getUnitType();
			AE_ASSERT(lType == DssTemplateConsolidation); // should be CS
            
			lpUnit[0] = lUnitIndex;
			//iDataShareLevel = DSSDataShareNone;
            
			hr = lpIndexPool->getIndex(NumUnits, lpUnit, &lpOriginalIndices);
			CHECK_HR_PTR(lpOriginalIndices);
		}else
			return E_FAIL;
	}else
    {
        CHECK_HR;
		CHECK_HR_PTR(lpOriginalIndices);
    }
	
	//CDSSXTabIndexPtr lIndexPtr(static_cast<ICDSSXTabIndices *>(lpOriginalIndices)); // ldong, 12/08/2010
    
	// aye, 9/20/2007
	// The list of attributes that are converted to CustomGroup
	std::vector<Int32> lvConvertedAttributes;
    
	Int32 i;
	for(i = 0; i < NumDerivedElements; i++)
	{
		lpDEUnitFlags[i] = false;
	}
    
	// ldong 04/05/2012 575382
	// for DEonDT ElementBrowse, if there is a filter, row # of CS unit is A, row # of CG unit is B, A >= B, we have index {CS}, we don't have index {CG}
	// luckily {CG} level index is: 0 .. (CG count - 1), which is the same as element browse index pattern
	Int32 lnDEonDTElementBrowseCount = -1;
    
	// weiwang local to global map
	Int32** lppLocal2Global = NULL;
    MBase::StrongArrayPtr<Int32 *> lppStrongPtrLocal2Global(lppLocal2Global);
	//ctang global to local map
	Int32** lGToLMap = NULL;
    MBase::StrongArrayPtr<Int32 *> lStrongPtrGToLMap(lGToLMap);

	IntToIntMapType** lGToLMap2 = NULL;
    MBase::StrongArrayPtr<IntToIntMapType *> lStrongPtrGToLMap2(lGToLMap2);
    
	if (/*lpGUnitContainer*/  ibRWDSubsetFromBase)
	{
		Int32 lnUnit = ipRelation ? NumUnits + lnRelationUnits - inJoinAttribute : NumUnits;
		lppLocal2Global = new Int32*[lnUnit];
        CHECK_NEWED_PTR(lppLocal2Global);
        lppStrongPtrLocal2Global.Reset(lppLocal2Global);
		memset(lppLocal2Global, 0, sizeof(Int32*) * lnUnit);
        
		//ctang 2011-2-11 prepare g2l map for metric slice populate
		lGToLMap = new Int32*[lnUnit];
        CHECK_NEWED_PTR(lGToLMap);
        lStrongPtrGToLMap.Reset(lGToLMap);

        lGToLMap2 = new IntToIntMapType*[lnUnit];
        CHECK_NEWED_PTR(lGToLMap2);
        lStrongPtrGToLMap2.Reset(lGToLMap2);
		memset(lGToLMap, 0, sizeof(Int32*) * lnUnit);
		memset(lGToLMap2, 0, sizeof(IntToIntMapType*) * lnUnit);
	}
    
	//ctang 2010-8-25 record extend attribute's position
	/*Int32 lExtendUnitPos = 0;*/
	vector<Int32> lvExtendUnitPos(inJoinAttribute);
    vector<int> lvCGPositions = ipTabularData->getCGPosition();
    vector<vector<int> > lvCGOrdinals = ipTabularData->getCGOrdinals();
	for(iUnit = 0; iUnit < NumUnits; iUnit ++)
	{
		DSSTabularUnit *lpTabularDataUnit;
		hr = ipTabularData -> Item(lpUnit[iUnit], &lpTabularDataUnit);
		CHECK_HR;
        
		//xhan 01/05/05
		if (lpUnit[iUnit] == lOrgDummyUnitIndex)
			mDummyUnitIndex = iUnit; //keep track the dummy unit in the new tabular data
        
		DSSTemplateUnit_Type lType = lpTabularDataUnit -> getUnitType();

        //////////////////////////////////////////////////
        mvUnitType[iUnit] = lType;
        GUID lGUID;
        int lIndex = -1;
        hr = ipTabularData->getUnitGUIDAndGlobalOffset(lpUnit[iUnit], lGUID, lIndex);
        CHECK_HR;
        
        mvUnitGUID[iUnit] = lGUID;
        mvUnitGlobalIndex[iUnit] = lIndex;
        //////////////////////////////////////////////////
        
		DSS_ID lID;
		hr = lpTabularDataUnit->getGUID(lID);
		CHECK_HR;
        
		if (iExtendAttribtue && pUnitInfo 
			&& lpUnitInfoMap[iUnit] >= 0 && lpUnitInfoMap[iUnit] < lnInputUnit 
			&& !pUnitInfo[lpUnitInfoMap[iUnit]].mfNoChildPopulate)
		{
			for (Int32 i = 0; i < inJoinAttribute; i++)
				if (ATL::InlineIsEqualGUID(lID, iExtendAttribtue[i]))
				{
					lvExtendUnitPos[i] = iUnit;
					break;
				}
		}
        
		//ctang 2010-11-2 check if currunt unit is DE or not
		bool lfDE = false;
		//ctang: DE on global lookup table
		bool lfDEOnGlobalLookUp = false;
		if (lType == DssTemplateAttribute || lType == DssTemplateConsolidation)
		{
			for(Int32 iDEUnit = 0; iDEUnit < NumDerivedElements; iDEUnit++)
			{
				if(ipDerivedElementUnits[iDEUnit] == lpUnit[iUnit])
				{
					lpDEUnits[iDEUnit] = iUnit;
					lpDEUnitFlags[iDEUnit] = lfDE = true;
                    
					if (lType == DssTemplateAttribute && ipGUContainer)
					{
						//if the orginal lookup table already ref to global lookup table
						//makr this flag to avoid the copy of global lookup.
						DSSTabularUnit *lpTmp = NULL;
						hr = lpTabularDataUnit->getTabularUnit(&lpTmp);//, &lfDEOnGlobalLookUp);
						CHECK_HR;
					}
                    
					break;
				}
			}
		}
        
		// ldong 12/10/2012 694850 DEonDTElementBrowseWithFilter
		bool lfDEonDTElementBrowseWithFilter = false;
        
		if (ipGUContainer && iUnit != mDummyUnitIndex && !lfDE /*ctang: fake CG in extension population will do the local population*/  && !pUnitInfo[lpUnitInfoMap[iUnit]].mfLocalPopulate)	// rwd lookup, DE will do the local populate
		{
			DSSTabularUnit * lpTabularDataUnit2 = lpTabularDataUnit;
			if(/*lpGUnitContainer*/  ibRWDSubsetFromBase && lType == DssTemplateAttribute)
			{
				DSS_ID lID;
				hr = lpTabularDataUnit->getGUID(lID);
				CHECK_HR;
                
				DSSTabularAttribute *lpLookUp = NULL;
				hr = /*lpGUnitContainer*/  ipGUContainer->getGlobalLookupByID(&lID, NULL, &lpLookUp);
				CHECK_HR_PTR(lpLookUp);
                
				lpTabularDataUnit2 = (DSSTabularUnit*)lpLookUp;
			}
            
			CHECK_HR_PTR(lpTabularDataUnit2);
            
			DSSTabularUnit * lpRefSubsetUnit = NULL;
			hr = createReferenceUnitI(lType, iUnit, lpTabularDataUnit2, ipTabularData, &lpRefSubsetUnit, false, false,
                                     //ctang 2011-4-26 for the case when fake CG exist in input slice, we don't want to populate CS' child
                                     pUnitInfo[lpUnitInfoMap[iUnit]].mfNoChildPopulate);
			CHECK_HR_PTR(lpRefSubsetUnit);
			mvUnits[iUnit] = lpRefSubsetUnit;
            
			if (lType == DssTemplateAttribute)
			{
				UnitFormInfo* lpUnitInfo = pUnitInfo + lpUnitInfoMap[iUnit];
				vector<Int32> lvSelectedForm;
                
				if (lpUnitInfo->mfSelectAll)// || /*ctang: if it is workingset report, follow the old logic, select all the forms*/  iDataShareLevel >= DSSDataShareUnit)
				{
					hr = static_cast<DSSTabularAttribute*>(lpTabularDataUnit2)->GetAllForm(lvSelectedForm);
					CHECK_HR;
				}
				else
				{
					for (Int32 j = 0; j < lpUnitInfo->nForms; j++)
					{
						Int32 lForm = -1;
						hr = static_cast<DSSTabularAttribute*>(lpTabularDataUnit2)->FindFormWithLocale(0,lpUnitInfo->Forms[j], lpUnitInfo->Locales[j], lForm);
						if (hr == S_OK)
							lvSelectedForm.push_back(lForm);
					}
				}
                
				//hr = static_cast<CDSSRefXTabLookUpTable*>(lpRefSubsetUnit)->SetSelectedForms(lvSelectedForm.size(),(lvSelectedForm.size() > 0) ? &lvSelectedForm[0] : NULL);
				CHECK_HR;
                
			}
			else
			{
				Int32 lRowCount;
				Int32* lpGToLMap;
				Int32* lpLToGMap;
				IntToIntMapType* lpGToLIntMap;// for DE case
                
				bool lfNewDEInExtension = false;
                
				if (/*lpGUnitContainer*/  ibRWDSubsetFromBase && ipGUContainer)
				{
                    DSSTabularUnit * lpOrignUnit = NULL;
                    hr = lpTabularDataUnit->getTabularUnit(&lpOrignUnit);
                    CHECK_HR;
                    
					vector<int>* lpTmpMap1 = ipGUContainer->getGlobalToLocalMap(mDataSetGUID,lpOrignUnit);
                    if (lpTmpMap1 != NULL && lpTmpMap1->size() != 0)
                    {
                        lpGToLMap = (Int32*)(&(*lpTmpMap1)[0]);
                    }
                    lRowCount = lpTmpMap1->size();
                    vector<int>* lpTmpMap2 = ipGUContainer->getLocalToGlobalMap(mDataSetGUID,lpOrignUnit);
                    if (lpTmpMap2 != NULL && lpTmpMap2->size() != 0)
                    {
                        lpLToGMap = (Int32*)(&(*lpTmpMap2)[0]);
                    }
                    lpGToLIntMap = ipGUContainer->getGlobalToLocalMap2(mDataSetGUID, lpOrignUnit);
				}
				else
				{
					DSSTabularUnit * lpOrignUnit = NULL;
					DSSTabularUnit* lpTabularDataUnitC = lpTabularDataUnit;
					hr = lpTabularDataUnitC->getTabularUnit(&lpOrignUnit);//, &lfRefUnit);
					CHECK_HR_PTR(lpOrignUnit);
                    bool lfRefUnit = lpTabularDataUnitC == lpOrignUnit ? false : true;
                    
					if (lfRefUnit)
					{
						if (lType == DssTemplateCustomGroup)
							hr = static_cast<DSSTabularRefCustomGroup*>(lpTabularDataUnit)->GetGlobalRowMap(&lRowCount, &lpGToLMap, &lpLToGMap);
						else
							hr = static_cast<DSSTabularRefConsolidation*>(lpTabularDataUnit)->GetGlobalRowMap(&lRowCount, &lpGToLMap, &lpGToLIntMap, &lpLToGMap);
						CHECK_HR;
					}
					else
						lfNewDEInExtension = true;
                    
				}
                
				if (!lfNewDEInExtension)
				{
					if (lType == DssTemplateCustomGroup)
                    {
						hr = static_cast<DSSTabularRefCustomGroup*>(lpRefSubsetUnit)->SetGlobalRowMap(lRowCount, lpGToLMap, lpLToGMap);
                        int pos = -1;
                        for (int i = 0; i < mvCGPos.size(); i++) {
                            if (mvCGPos[i] == iUnit) {
                                pos = i;
                                break;
                            }
                        }
                        if (pos != -1)
                            static_cast<DSSTabularRefCustomGroup*>(lpRefSubsetUnit)->SetOrdinal(mvCGOrdinals[pos]);
                    }
					else
					{
						hr = static_cast<DSSTabularRefConsolidation*>(lpRefSubsetUnit)->SetGlobalRowMap(lRowCount, lpGToLMap, lpGToLIntMap, lpLToGMap);
						CHECK_HR;
						//ctang: set inherit flag
						//hr = static_cast<DSSTabularRefConsolidation*>(lpRefSubsetUnit)->SetInheritFlag(ipGUContainer);
					}
					CHECK_HR;
				}
			}
            
			if (/*lpGUnitContainer*/  ipGUContainer && ibRWDSubsetFromBase)
			{
				DSSTabularUnit * lpOrignalUnit = NULL;
				bool lfGlobal = false;
				hr = lpTabularDataUnit->getTabularUnit(&lpOrignalUnit);//, &lfGlobal);
				CHECK_HR_PTR(lpOrignalUnit);
                
				if (!lfGlobal)
				{
					vector<int>* lpLocalToGlobalMap = ipGUContainer->getLocalToGlobalMap(mDataSetGUID,lpOrignalUnit);
                    if (lpLocalToGlobalMap != NULL && lpLocalToGlobalMap->size() != 0)
                    {
                        lppLocal2Global[iUnit] = (Int32*)(&(*lpLocalToGlobalMap)[0]);
                    }

                    vector<int>* lpGlobalToLocalMap = ipGUContainer->getGlobalToLocalMap(mDataSetGUID,lpOrignalUnit);
                    if (lpGlobalToLocalMap != NULL && lpGlobalToLocalMap->size() != 0)
                    {
                        lGToLMap[iUnit] = (Int32*)(&(*lpGlobalToLocalMap)[0]);
                    }
                    lGToLMap2[iUnit] = ipGUContainer->getGlobalToLocalMap2(mDataSetGUID,lpOrignalUnit);
                    
				}
			}
		}
		else if (iUnit == mDummyUnitIndex) //iDataShareLevel >= DSSDataShareUnit) {
        {	//it is only for workingset subsetting, cube subsetting can't share the unit or metric
            //tliao: only dummy unit go here
			DSSTabularUnit * lpRefSubsetUnit = NULL;
			hr = createReferenceUnitI(lType, iUnit, lpTabularDataUnit, ipTabularData, &lpRefSubsetUnit, ifDEonDTElementBrowse);
			CHECK_HR_PTR(lpRefSubsetUnit);
			mvUnits[iUnit] = lpRefSubsetUnit;
            
		} else {
			//ctang: DE on global lookup table
			if (((!ifDEonDTElementBrowse) || (lType != DssTemplateConsolidation)) && lfDEOnGlobalLookUp)
			{
				DSS_ID lID;
				hr = lpTabularDataUnit->getGUID(lID);
				CHECK_HR;
                
				//1.get global lookup
				DSSGUnitContainer* lpGUnitContainer = ipGUContainer;
				DSSTabularAttribute *lpGLookUp;
				hr = lpGUnitContainer->getGlobalLookupByID(&lID, NULL, &lpGLookUp);
				CHECK_HR_PTR(lpGLookUp);
                
				//DSSTabularAttribute* lpGLookUpC = static_cast<DSSTabularAttribute*>(lpGLookUp);
                
				//2.create empty CG
				DSSTabularUnit* lpTemp;
				/*
                hr = CreateUnit_0(DssTemplateCustomGroup, iUnit, &lpTemp);	
				*/
                   CHECK_HR;
                
				//3.init the newly created custom group unit with global lookup
				//DSSTabularCustomGroup* lpCG = static_cast<DSSTabularCustomGroup*>(lpTemp);
				/*
                hr = lpCG->Init(lpGLookUpC);
				CHECK_HR;
				
				hr = lpCG->put_IDI(lID);
				CHECK_HR;
                */
  				mvUnits[iUnit] = lpTemp;
                
				lvConvertedAttributes.push_back(iUnit);
			}
			else if ((!ifDEonDTElementBrowse) || (lType != DssTemplateConsolidation)) // ldong 03/21/2012 568877
			{// normal logic
                /*
				hr = CreateUnit_0(lType, iUnit, &mvUnits[iUnit]);
				CHECK_HR;
                */
				//ctang 2011-8-4 subset from cube, need to get the real local unit
  				DSSTabularUnit * lpOriginUnit;
				hr = static_cast<DSSTabularUnit*>(lpTabularDataUnit)->getTabularUnit(&lpOriginUnit);
				CHECK_HR_PTR(lpOriginUnit);
                
				bool lfRefUnit = lpOriginUnit != lpTabularDataUnit;
                
				switch(lType)
				{
                    case DssTemplateAttribute:
					{
						//DSSTabularAttribute *lpLUT = (DSSTabularAttribute*)lpOriginUnit;
                        
						//UnitFormInfo *lpUnitInfo =  (iUnit == mDummyUnitIndex) ? NULL: pUnitInfo + lpUnitInfoMap[iUnit];
                        
                        
						// aye, 322298, 8/5/2008
						// when copying part of the TabularData, we need to copy all elements of attributes that
						// are referenced by the partial copy.
						// To simplify the code, we find the max element ID and copy all elements from 0 to it.
						/*
                        (CComPtr<ICDSSXTabTable> lpTable;
						hr = lpTabularDataUnit->get_Table(&lpTable);
						CHECK_HR;
                        */
  						Int32 lElementCount = 0;
						//hr = lpTable->get_RowCount(&lElementCount);
						CHECK_HR;
                        
						//Int32 lMaxKey = -2;
						if (lElementCount > nRowLimit && nRowLimit > 0)
						{
							//mpittkin, 326620: from now on we will fail all reports that exceed the row limit
							return hr = E_FAIL;
                            
						}
                        
						// jxie, 2010-7-29
						// using different population with element range for element browsing
						if (inElementBrowseCount >= 0)
						{
							//hr = static_cast<DSSTabularAttribute*>(mvUnits[iUnit])->FilterPopulate(lpLUT, iUnit, inElementBrowseCount, ipElementBrowseList, lpUnitInfo, true);
							CHECK_HR;
						}
						else
						{					
							//hr = static_cast<DSSTabularAttribute *>(mvUnits[iUnit])->Populate(lpLUT, iUnit, lMaxKey + 1, lpUnitInfo);
							CHECK_HR;
						}
                        
						if (lfDE)
						{
							DSS_ID lID;
							hr = lpTabularDataUnit->getGUID(lID);
							CHECK_HR;
                            
							//DSSTabularUnit * lpCGUnit;
							//hr = CreateCustomGroupUnit(iUnit, lID, &lpCGUnit);
							CHECK_HR;
                            
							lvConvertedAttributes.push_back(iUnit);
						}
                        
					}
                        
                        break;
                        
                    case DssTemplateCustomGroup:
					{
						//DSSTabularCustomGroup *lpCustomGroup = (DSSTabularCustomGroup*)lpOriginUnit;

						//hr = static_cast<DSSTabularCustomGroup *>(mvUnits[iUnit])->Populate(lpCustomGroup, iUnit);
						CHECK_HR;
					}
                        
                        break;
                        
                    case DssTemplateConsolidation:
					{
						DSSTabularConsolidation *lpConsolidation = (DSSTabularConsolidation*)lpOriginUnit;
                        
                        DSSTabularConsolidation *lpRefCS = (DSSTabularConsolidation*)lpTabularDataUnit;
                        
                        /*
						Int32 lnUnitInCSLD = 0;
						hr = lpConsolidation -> get_NumUnits(&lnUnitInCSLD);
						CHECK_HR;
                        */
                          Int32 lnUnitInCSLD = 1;
						Int32 lpUnitInCSLD[lnUnitInCSLD];
						CHECK_NEWED_PTR(lpUnitInCSLD);
						Int32 lpUnitInCSLD2[lnUnitInCSLD];
						CHECK_NEWED_PTR(lpUnitInCSLD2);
						//ctang: handle for ref unit
						if (lfRefUnit)
							lpUnitInCSLD[0] = lpRefCS -> getUnitIndex();
						else
							lpUnitInCSLD[0] = lpConsolidation -> getUnitIndex();
						CHECK_HR;
                        
						DSSTabularUnit *lppUnit[lnUnitInCSLD];
						CHECK_NEWED_PTR(lppUnit);
						DSSTabularUnit *lppUnit2[lnUnitInCSLD];
						CHECK_NEWED_PTR(lppUnit2);
                        
						// yma 7/16/07
						// if this is the derived element, then mark it, later we need populate
						// the underline custom group unit for it.
						Int32 iDEUnit = 0;
						for(iDEUnit = 0; iDEUnit < NumDerivedElements; iDEUnit++)
						{
							if(ipDerivedElementUnits[iDEUnit] == lpUnit[iUnit])
							{
								lpDEUnits[iDEUnit] = iUnit;
								lpDEUnitFlags[iDEUnit] = false;
								break;
                                
							}
						}
                        
						for(Int32 iUnit2 = 0; iUnit2 < lnUnitInCSLD; iUnit2 ++)
						{
							DSSTabularUnit * lpSubUnit;
							hr = ipTabularData-> Item(lpUnitInCSLD[iUnit2], &lpSubUnit);
							CHECK_HR;
                            
							//ctang: for RefCG
							DSSTabularUnit * lpSubUnit2;
							hr = static_cast<DSSTabularUnit*>(lpSubUnit)->getTabularUnit(&lpSubUnit2);
							CHECK_HR;
                            
							lppUnit[iUnit2] = lpSubUnit2;
                            
							//DSSTemplateUnit_Type lSubType = lpSubUnit -> getUnitType();
                            
							DSSTabularUnit * lpTemp = NULL;
                            
							if(NumDerivedElements == 0 || iDEUnit == NumDerivedElements)
								lpUnitInCSLD2[iUnit2] = mvUnits.size(); // YMA: no DE in dataset or this unit is not DE
							else
								lpUnitInCSLD2[iUnit2] = NumUnits + NumMissingUnit + iDEUnit;
                            
							//hr = CreateUnit_0(lSubType, lpUnitInCSLD2[iUnit2], &lpTemp);
                            
							if(NumDerivedElements == 0 || iDEUnit == NumDerivedElements)
								mvUnits.push_back(lpTemp); // YMA: no DE in dataset or this unit is not DE
							else
								mvUnits[NumUnits + NumMissingUnit + iDEUnit] = lpTemp;
                            
							lppUnit2[iUnit2] = lpTemp;
							CHECK_HR;
						}
                        /*
						hr = static_cast<DSSTabularConsolidation *>(mvUnits[iUnit])-> Populate(lpConsolidation,
                                                                                             iUnit,
                                                                                             lnUnitInCSLD,
                                                                                             lpUnitInCSLD2,
                                                                                             lppUnit,
                                                                                             lppUnit2);
						CHECK_HR;
                        */
  						bool lbIsDE = lpConsolidation->IsDerivedElement();
                        
						if (lbIsDE)
						{
							//hr = static_cast<DSSTabularConsolidation *>(mvUnits[iUnit])->put_IsInherited(VARIANT_TRUE);
							CHECK_HR;
						}
						if (ipGUContainer && lfDE)
						{
							DSSTabularUnit* lpOriginCS = mvUnits[iUnit];
                            
							//hr = hReplaceDEUnit(iUnit, ipGUContainer, iViewTemplateID, true, ipRI);
							CHECK_HR;
							//ctang: if subset from RefCS, the index is already in global key, don't need to set the map
							if  (!lfRefUnit)
							{
								if (!lppLocal2Global)
								{
									Int32 lnUnit = ipRelation ? NumUnits + lnRelationUnits - inJoinAttribute : NumUnits;

                                    lppLocal2Global = new Int32*[lnUnit];
                                    CHECK_NEWED_PTR(lppLocal2Global);
                                    lppStrongPtrLocal2Global.Reset(lppLocal2Global);
                                    memset(lppLocal2Global, 0, sizeof(Int32*) * lnUnit);

                                    //ctang 2011-2-11 prepare g2l map for metric slice populate
                                    lGToLMap = new Int32*[lnUnit];
                                    CHECK_NEWED_PTR(lGToLMap);
                                    lStrongPtrGToLMap.Reset(lGToLMap);
                                    
                                    lGToLMap2 = new IntToIntMapType*[lnUnit];
                                    CHECK_NEWED_PTR(lGToLMap2);
                                    lStrongPtrGToLMap2.Reset(lGToLMap2);
                                    
									memset(lGToLMap, 0, sizeof(Int32*) * lnUnit);
									memset(lGToLMap2, 0, sizeof(IntToIntMapType*) * lnUnit);
								}
                                
								vector<int>* lpTmpMap1 = ipGUContainer->getLocalToGlobalMap(mDataSetGUID,lpOriginCS);
								if (lpTmpMap1 != NULL && lpTmpMap1->size() != 0)
                                {
                                    lppLocal2Global[iUnit] = (Int32*)(&(*lpTmpMap1)[0]);
                                }
                                
                                vector<int>* lpTmpMap2 = ipGUContainer->getGlobalToLocalMap(mDataSetGUID,lpOriginCS);
								if (lpTmpMap2 != NULL && lpTmpMap2->size() != 0)
                                {
                                    lGToLMap[iUnit] = (Int32*)(&(*lpTmpMap2)[0]);
                                }
                                lGToLMap2[iUnit] = ipGUContainer->getGlobalToLocalMap2(mDataSetGUID, lpOriginCS);							}
						}
					}
                        
                        break;
                        
                    default:
                        AE_ASSERT(false);
                        break;
                        
				} // switch
			} else {
				// ldong 03/21/2012 568877
				// if DEonDTElementBrowse == true and ShareBase == false, it must have a filter, so it is DE on DT element browsing with filter case
				// the current UnitIndex is still CS unit , but we need to get attribute from CG unit
				// note that ipTabularData is a filtered TabularData which only has the index for CS unit, not for CG unit. but it has both CG and CS unit.
				// CS index is already generated before. We assume the index of CS and CG unit should be the same, as only concrete elements are remaining in CS and CG unit
				// now we just need to populate Attribute unit from CG unit
				AE_ASSERT(lType == DssTemplateConsolidation);
                
				// for DE on DT, in Derived Element editor, sometimes we will do element browsing with filter
				// if with filter, we shouldn't use custom group unit because later in FilterPopulate the units order will be changed
				// e.g. (Year RegionCG SubCategory RegionCS) will be changed to (Year SubCategory RegionCS RegionCG)
				// so here we still use consolidation unit index 3, and after FilterPopulate, its unit index will be changed to 2
				// now we need to find unit index of RegionCG(3) based on that of RegionCS(2)
				Int32 lCSUnitIndex = lpUnit[iUnit]; // CS unit index
				Int32 lCGUnitIndex = lCSUnitIndex;  
				//hr = static_cast<DSSTabularData *>(ipTabularData) -> FindCGUnitIndex(lCSUnitIndex, lCGUnitIndex); // get corresponding CG unit index
				
				DSSTabularUnit * lpTabularDataUnit;
				hr = ipTabularData -> Item(lCGUnitIndex, &lpTabularDataUnit);
				CHECK_HR;
				
				DSSTemplateUnit_Type lTypeCG = lpTabularDataUnit -> getUnitType();
                
				AE_ASSERT(lTypeCG == DssTemplateCustomGroup); // should be CG
                
				// create Attribute
				//hr = CreateUnit_0(DssTemplateAttribute, iUnit, &mvUnits[iUnit]);
				CHECK_HR;
                
				// get CDSSXTabCustomGroup pointer
				//DSSTabularCustomGroup* lpCG = (DSSTabularCustomGroup*)lpTabularDataUnit;

				//DSSTabularCustomGroup *lpCG2 = static_cast<DSSTabularCustomGroup *> (lpCG);
				//DSSTabularCustomGroup *lpXTabCustomGroup = static_cast<DSSTabularCustomGroup *> (lpCG2);
				
				// get the first LookupTable from the CustomGroup unit
				DSSTabularAttribute *lpLUT;
				//hr = lpXTabCustomGroup -> get_LookUpTable(0, &lpLUT); // the first one is the attribute lookup table
				CHECK_HR_PTR(lpLUT);
                
				//DSSTabularAttribute* lpXTabLookUpTable = static_cast <DSSTabularAttribute *>(lpLUT);
                
				// prepare lpUnitInfo
				UnitFormInfo *lpUnitInfo =  (iUnit == mDummyUnitIndex) ? NULL: pUnitInfo + iUnit;
				if (lpUnitInfo)
				{
					// in fact we don't care UnitID here as UnitID will not be used for populating the attribute unit later
					lpUnitInfo->UnitID = lCGUnitIndex;
					//lpXTabLookUpTable->GetUnitFormInfo(lpUnitInfo); // copy unit info from attribute
				}
                
				// code below is copied from the Attribute population code above
                
				// aye, 322298, 8/5/2008
				// when copying part of the TabularData, we need to copy all elements of attributes that
				// are referenced by the partial copy.
				// To simplify the code, we find the max element ID and copy all elements from 0 to it.
				/*
                CDSSXTabTable* lpTable;
				hr = lpXTabLookUpTable->get_TableC(&lpTable);
				CHECK_HR;
                
				Int32 lElementCount = 0;
				hr = lpTable->get_RowCount(&lElementCount);
				CHECK_HR;
                
				Int32 lMaxKey = -2;
				if (lElementCount > nRowLimit && nRowLimit > 0)
				{
					//mpittkin, 326620: from now on we will fail all reports that exceed the row limit
					return hr = ReportCOMError(CLSID_DSSAnalyticalEngineServer, 
                                               IID_ICDSSAnalyticalEngineServer, DFC_QRYENG_RES_TRUNC,
                                               << nRowLimit); 
				}
                
				// jxie, 2010-7-29
				// using different population with element range for element browsing
				if (inElementBrowseCount >= 0)
				{
					hr = static_cast<CDSSXTabLookUpTable*>(mvUnits[iUnit])->FilterPopulate(lpLUT, iUnit, inElementBrowseCount, ipElementBrowseList, lpUnitInfo, true);
					CHECK_HR;
				}
				else
				{					
					hr = static_cast<CDSSXTabLookUpTable *>(mvUnits[iUnit])->Populate(lpLUT, iUnit, lMaxKey + 1, lpUnitInfo);
					CHECK_HR;
				}
				// ldong 04/05/2012 575382
				lnDEonDTElementBrowseCount = lElementCount;
				lfDEonDTElementBrowseWithFilter = true;
                 */
  			}// else if (!ifDEonDTElementBrowse)
		} //else if (iDataShareLevel >= DSSDataShareUnit)
        
		//ctang: 673438 since MCE could do another subset after join, we need inherit the repopulation flag
		// ldong 12/10/2012 694850 skip this step when lfDEonDTElementBrowseWithFilter is true
  		if ((lType == DssTemplateConsolidation) && (!lfDEonDTElementBrowseWithFilter))
		{
            /*
			DSSTabularConsolidation* lpCS = static_cast<DSSTabularConsolidation*>(mvUnits[iUnit]);
			DSSTabularConsolidation* lpOrgCS = static_cast<DSSTabularConsolidation*>(lpTabularDataUnit);
			//lpCS->setRepopulate(lpOrgCS->NeedRepopulate());
            
			//in extension, if join on DE mark that we need to update DE index
			VARIANT_BOOL lfDE;
			hr = lpCS->get_IsDerivedElement(&lfDE);
			CHECK_HR;
            
			if (ipRelation && lfDE == VARIANT_TRUE)
			{
				lpCS->setUpdateDEIndex(true);
			}
			else
				lpCS->setUpdateDEIndex(lpOrgCS->NeedUpdateDEIndex());
             */
  		}
	} //for
    
	//ctang 2010-8-25 add new attribtue
	/*Int32 lMaxParent = -1;*/
	//ctang 2011-2-15
  	vector<Int32> lvElementCount(lnRelationUnits);
	if (lnRelationUnits > 0)
		memset(&lvElementCount[0], 0, sizeof(Int32) * lnRelationUnits);
    
	vector<Int32> lvNewUnitPos;
	if (ipRelation)
	{
		//the unit in relation should always be global lookup table
		Int32 lExtendPos = NumUnits;
		for (Int32 i = 0; i < lnRelationUnits; i++)
		{
			//get the element count for the relation units
			DSSTabularAttribute *lpGLookup;
			Int32 lIndex;
			hr = ipGUContainer->getGlobalLookupByID(&lvRelationUnitID[i], &lIndex, &lpGLookup);
			CHECK_HR_PTR(lpGLookup);
            /*
			CDSSXTabTable* lpTable;
			hr = static_cast<DSSTabularAttribute*>(lpGLookup.p)->get_TableC(&lpTable);
			CHECK_HR;
            
			if (lpTable)
			{
				hr = lpTable->get_RowCount(&lvElementCount[i]);
				CHECK_HR;
			}
			else
				lvElementCount[i] = 0;//0 is reserved in this case
            */
  			if (!lMask.Get(i))
			{
				DSSTabularUnit * lpTabularDataUnit = (DSSTabularUnit*) lpGLookup;
                
				DSSTabularUnit * lpRefSubsetUnit = NULL;
				hr = createReferenceUnitI(DssTemplateAttribute, lExtendPos, lpTabularDataUnit, NULL, &lpRefSubsetUnit);
				CHECK_HR_PTR(lpRefSubsetUnit);
				mvUnits[lExtendPos++] = lpRefSubsetUnit;
				lvNewUnitPos.push_back(i);
			}
		}
		AE_ASSERT(lvNewUnitPos.size() == lnRelationUnits - inJoinAttribute);
        
	}
    vector< Int32 > lvMissingPos;
	lvMissingPos.resize(NumMissingUnit);
	//xhan 09/23/05 Populate the missing unit if there is any
	for(iUnit = 0; iUnit < NumMissingUnit; iUnit ++)
	{
		hr = hPopulateMissingUnit(NumUnits + iUnit, ipMissingUnitIDs[iUnit], ipMissingUnitFormInfo[iUnit], &(lvMissingPos[iUnit]), ipGUContainer);
		CHECK_HR;
	}
	//populate main index
	DSSTabularIndex * lpNewIndices = new DSSTabularIndex();
	CHECK_NEWED_PTR(lpNewIndices);
    
	//create unit map
	Int32 lnUnitOriginal = 0;
	hr = ipTabularData->Count(&lnUnitOriginal);
	CHECK_HR;
    
	Int32 lpUnitsMap[lnUnitOriginal];
	CHECK_NEWED_PTR(lpUnitsMap);
    
	for (iUnit = 0; iUnit < NumUnits; iUnit ++)
		lpUnitsMap[lpUnit[iUnit]] = iUnit;
    
	if (ipRelation)
	{
        /*
		CDSSXTabIndexPool *lpIndexPool = NULL;
		hr = (static_cast<CDSSTabularData *> (ipTabularData) )->get_IndicesPoolC(&lpIndexPool);
		CHECK_HR_PTR(lpIndexPool);
        
		CDSSXTabIndices *lpOriginalIndices = NULL;
		hr = lpIndexPool->GetMainInputIndices(&lpOriginalIndices);
		CHECK_HR_PTR(lpOriginalIndices);
        
		if (inJoinAttribute > 0)
			hr = CDSSJoinHelper::extend_index(lpOriginalIndices, ipRelation, static_cast<CDSSTabularData*>(ipTabularData), lvExtendUnitPos, lvJoinAttributesOrder, lvNewUnitPos, lppLocal2Global, lvElementCount.size() > 0 ? &lvElementCount[0] : NULL, this, lpNewIndices, NULL, NULL, NULL, true, ipGUContainer, ifRePopulateDEInExtension);
		else
			hr = CDSSJoinHelper::cross_join_relationAndIndex(lpOriginalIndices, ipRelation, static_cast<CDSSTabularData*>(ipTabularData), lppLocal2Global, this, lpNewIndices, NULL, NULL, NULL);
		CHECK_HR;
         */
  	}
	else
	{
		// ldong 04/05/2012 575382
		if (lnDEonDTElementBrowseCount < 0){ // normal logic
			hr = lpNewIndices->Populate(lpOriginalIndices, lpUnitsMap, NumMissingUnit, nRowLimit, inElementBrowseCount,  lppLocal2Global);
            CHECK_HR;
        }
		else // DE on DT element browse
			hr = S_OK; ////lpNewIndices->Populate(lpOriginalIndices, lpUnitsMap, NumMissingUnit, nRowLimit, lnDEonDTElementBrowseCount);
	}
    
	// ldong, 12/17/2010
	// lpNewIndices is the main index of the new index pool of the view report
	// yma 1/9/06
	// create Consolidation unit or custom group for Derived Element if necessary
  	Int32 iDEUnit;
	Int32 lnCurUnits = NumUnits+NumMissingUnit;
	for(iDEUnit = 0; iDEUnit < NumDerivedElements; iDEUnit++)
	{
		if(lpDEUnitFlags[iDEUnit])
            // already has custom group unit, need to create consolidation unit
		{
			//hr = CreateUnit_0(DssTemplateConsolidation, iDEUnit+lnCurUnits, &mvUnits[iDEUnit+lnCurUnits]);
			CHECK_HR;
            
			DSSTabularConsolidation * lpConsolidation;
			lpConsolidation = static_cast <DSSTabularConsolidation * > (mvUnits[iDEUnit+lnCurUnits]);
            
			DSS_ID lID;
			hr = mvUnits[lpDEUnits[iDEUnit]]->getGUID(lID);
			CHECK_HR;
            /*
			hr = lpConsolidation->put_IDI(lID);
			CHECK_HR;
            
			hr = lpConsolidation->Init(1, &lpDEUnits[iDEUnit]);
			CHECK_HR;
            
			hr = lpConsolidation->put_IsDerivedElement(VARIANT_TRUE);
			CHECK_HR;
             */
  		}
	}
    
	// ldong 08/19/2011
	/*
     if (NumDerivedElements > 0)
		mfHasDE = true;
    */
    
    //////////////////////////////////////////populate the metrics
    mMetircPos = mvUnits.size();
    if (!mpMetrics)
    {
        mpMetrics = new DSSTabularMetrics();
    }
    DSSTabularMetrics* lpNewMetrics = new DSSTabularMetrics();
    lpNewMetrics->Init(mMetircPos, NumSlices);
    mvUnitType.push_back(DssTemplateMetrics);
    mvUnitGUID.push_back(GUID_NULL);
    mvUnitGlobalIndex.push_back(-1);
    DSSTabularMetrics* lpOriginalMetrics = ipTabularData->getTabularMetircs();

    vector<GUID> lvMetricGUID;
    for (int j = 0; j < NumSlices; ++j) 
    {
        //need to skip SFB metrics here
        if (ipSlices[j]->IsSFBSlice())
            continue;
        
        GUID lGUID = ipMetricIDs[j].mMetricID;
        lvMetricGUID.push_back(lGUID);
        
        int lMetricTranslatedTotal = DssMetricReserved;
        int pos = lpOriginalMetrics->FindMetric(lGUID);
        if (pos != -1)
        {
            lMetricTranslatedTotal = lpOriginalMetrics->getTotalImplementation(pos);
        }
        //fali, 833299, if the condition is true, that means we can't get metric from lpOriginalMetrics(may be summary metric)
        //or we set total type as DssMetricReserved in filterPopulate(may be summary metric with VF)
        //try to get it from viewTabularData
        if (lMetricTranslatedTotal == DssMetricReserved)
        {
            //for summary metric, we can't get its subtotal info from base tabular data, but can get it from view tabular data
            pos = mpMetrics->FindMetric(lGUID);
            lMetricTranslatedTotal = mpMetrics->getTotalImplementation(pos);
        }
        
        //zhyang, for the last two parameters, we just use double and reserved value
        //need to change later
        MBase::CString lDSSID = DSSAEHelper::Guid2String(lGUID);
        hr = lpNewMetrics->AddMetric(j, lDSSID, DssDataTypeDouble, lMetricTranslatedTotal, false);
        CHECK_HR;
        
        GUID lTotalImplGUID = GUID_NULL;
        GUID lDATotalImplGUID = GUID_NULL;
        vector<GUID> lvNonAggAttr;
        vector<EnumDSSAggregation> lvAggInfo;
        EnumDSSMetricType lDynamicAggFunction = DssMetricReserved;
        int lAvailableSystemSubtotal;
        vector<GUID> lAvailableUserDefinedSubtotal;
        pos = mpMetrics->FindMetric(lGUID);
        if (pos != -1)
        {
            lDynamicAggFunction = mpMetrics->getDynamicAggFunc(pos);
            lTotalImplGUID = mpMetrics->getTotalImplID(pos);
            lDATotalImplGUID = mpMetrics->getDynamicAggID(pos);
            mpMetrics->getAggregationInfo(ipMetricIDs[j].mMetricID, lvNonAggAttr, lvAggInfo);
            mpMetrics->getAvailableSubtotals(ipMetricIDs[j].mMetricID, lAvailableSystemSubtotal, lAvailableUserDefinedSubtotal);
        }
        
        hr = lpNewMetrics->setDynamicAggFunc(lDynamicAggFunction);
        CHECK_HR;
        
        hr = lpNewMetrics->setTotalImplID(lTotalImplGUID);
        CHECK_HR;
        
        hr = lpNewMetrics->setDynamicAggID(lDATotalImplGUID);
        CHECK_HR;
        
        hr = lpNewMetrics->setAggInfo(lvNonAggAttr, lvAggInfo);
        CHECK_HR;
        
        hr = lpNewMetrics->setAvailableSubtotals(lAvailableSystemSubtotal, lAvailableUserDefinedSubtotal);
        CHECK_HR;

    }
    mpMetrics->AddMetricToAnotherMetrics(lpNewMetrics);
    lpNewMetrics->setDisplayedMetricIDs(mpMetrics->getDisplayedMetricIDs());

    delete mpMetrics;
    mpMetrics = NULL;
    mpMetrics = lpNewMetrics;
    
    mvUnits.push_back(mpMetrics);
    
    
    mnUnits = mvUnits.size();
    
    mpIndexPool = new DSSTabularIndexPool();
    
    // tehe, 12/31/2011 before loading indexs, we need set the right binary keys
    int lMetricPos = getMetircPos();
    int lnBinaryKeys = getUnitCount();
    if(lMetricPos != -1)
        lnBinaryKeys -= 1;
    mpIndexPool->setBinaryKeys(lnBinaryKeys);
    
    mpIndexPool->Init(lpNewIndices->getNumUnits());
    hr = mpIndexPool->setMainIndex(lpNewIndices);
    CHECK_HR;
    ////////////////////////////////////////

  	for(iSlice = 0; iSlice < NumSlices; iSlice ++)
	{
		//check if all the slice's level is compatible with the selection
		DSSCubeDataSlice * lpSliceTo = new DSSCubeDataSlice();
		CHECK_NEWED_PTR(lpSliceTo);
		//ctang 2011-2-11 using the global to local map instead of orignal tabular data
		hr = lpSliceTo->Populate(ipSlices[iSlice], this, lpUnitsMap, &lvConvertedAttributes, /*ipSource,*/   iRestoreDouble, lGToLMap, lGToLMap2, ipSchema);
		CHECK_HR;
        
        hr = mpMetrics->addSlice(lpSliceTo);
        CHECK_HR;
        
        DSSDataColumn *lpColumn = lpSliceTo->getColumn();
        CHECK_PTR(lpColumn);
  	}

    return S_OK;
}
  
int DSSTabularData::getUnitGUIDAndGlobalOffset(int i, GUID &irGUID, int &iOffset)
{
    if (i < 0 || i >= mvUnitGUID.size())
        return E_INVALIDINDEX;
    
    irGUID = mvUnitGUID[i];
    iOffset = mvUnitGlobalIndex[i];
    return S_OK;
}

int DSSTabularData::setIndexPool(DSSTabularIndexPool* ipIndexPool)
{
    mpIndexPool = ipIndexPool;
    return S_OK;
}

void DSSTabularData::clearForSubsetting()
{
    for (int i=0; i<mvUnits.size(); i++)
    {
        if(mvUnits[i] && mvUnits[i]->getUnitType() != DssTemplateMetrics)
        {
            delete mvUnits[i];
            mvUnits[i] = NULL;
        }
    }
    
    mvUnits.clear();
    mvUnitType.clear();
    mvUnitGUID.clear();
    mvUnitGlobalIndex.clear();
}

int DSSTabularData::getLevels(vector<int> &lvUnits)
{
    int pos = 0;
    for (int i = 0; i < mvUnitGUID.size(); i++)
    {
        if (mvUnitGlobalIndex[i] != -1)
        {
            lvUnits.push_back(pos);
            pos++;
        }
    }
    return S_OK;
}

int DSSTabularData::getMetricsCount()
{
    return mpMetrics->Count();
}

int  DSSTabularData::OldFilterIndices(DSSTabularIndex *pIndices, DSSCubeDataSlice *pSlice,
                                      Int32 &nFilteredRows, Int32 *opFilteredRows,
                                      MBase::Buffer *pBuffer, DSSTabularData *ipOriginalTabularData)
{
	// If there is consolidation in the slice's level, we need to
	// generate equivalent slice having no consolidation unit in its level.
    
	// Scanning through the base indices and hash into the filter slice
	// generates a row map which indicate if row in the base indices should be kept
    
	//we are going to have a choice how to filter the indices.
    
	/*
     Think about it. For example, if we have User and Stock as the indices to be filtered.
     We have a slice at User level to filter out the user.
     
     We have two way to do it.
     1. We scan through the indices at User and Stock level, for each one, we will hash to
     the filter slice to determine if it should stay.
     
     The cost: size of (User, stock) indices
     
     2. Or we just go with the indices at (Stock) level and cartesian with the entry in
     the filter slice where = 1, then we will look up the (User, stock) indices, to find
     if the entry exist.
     
     The cost. number of 1 in filter slice * (Stock) indices
     
     
     Actually, this can apply to the way we generate the indices, whenver we need to generate
     our indices. We can go to the next indices which is lower than it and do a distinct. Or we
     go to the next one which is higher than it and cartesian with the lookup tables (that was aggregated)
     and then hash to the lower one to see which entry are really needed.
     */
    int hr = S_OK;
    
	Int32 iRow = 0,  lnRow = 0 ;
	lnRow = pIndices -> Count();
    
	DSSData_Flags lFlag;
	double lVal;
	nFilteredRows = 0;
    
	// aye, 383124, 2009/11/5
	// Need to pass the cube that contains index and slice to lSliceCorrelator
	DSSCube* lpCube = new DSSCube();
    lpCube->Init(ipOriginalTabularData);
    
	DSSSliceCorrelator lSliceCorrelator;
	hr = lSliceCorrelator.Init(pIndices, 1, &pSlice, pSlice, lpCube);
	CHECK_HR;
    
	//xhan 06/15/05 remove the dubious optimization code that may actually slows down the server by generating the
	//unnecessary indices
    //scan all rows one by one
	for(iRow = 0; iRow < lnRow; iRow ++)
	{
		hr = lSliceCorrelator.GetInputValue(0, iRow, &lVal, &lFlag);
		CHECK_HR;
        
		if(lFlag == DssDataOk && lVal == 1) {
			opFilteredRows[nFilteredRows ++] = iRow;
		}
	}
    
	return hr = S_OK;
}

//xiyu, 343249, add missing unit form info
int DSSTabularData::hPopulateMissingUnit(Int32 iUnitIndex, DSS_ID & iUnitID, UnitFormInfo & iUnitForm, Int32 * opMissingPos, DSSGUnitContainer* ipGUContainer)
{
	int hr = S_OK;
	if (ipGUContainer)
	{
        
		DSSTabularAttribute* lpLookUp = NULL;
		hr = ipGUContainer->getGlobalLookupByID(&iUnitID, NULL, &lpLookUp);
		if(FAILED(hr)) return hr;
		//jiawwu, 2013/05/14, if there is GLU for this missing unit, create ref lookup and record the missing position
		if(lpLookUp)
		{
            /*
			CComPtr<ICDSSTabularDataUnit> lpTabularDataUnit2 = NULL;
			hr = lpLookUp->QueryInterface(IID_ICDSSTabularDataUnit, (LPVOID*)&lpTabularDataUnit2);
			CHECK_HR_PTR(lpTabularDataUnit2);
            
			CDSSGlobalLookUpTable *lpGLU = static_cast<CDSSGlobalLookUpTable *>(lpLookUp.p);
			CDSSXTabTable* lpTable = NULL;
			hr = lpGLU->get_TableC(&lpTable);
			CHECK_HR;
			Int32 lRowCount, lnMissingUnit;
			hr = lpTable->get_RowCount(&lRowCount);
			CHECK_HR;
			hr = lpGLU->GetMissingUnitsCount(&lnMissingUnit);
			CHECK_HR;
            
			*opMissingPos = lRowCount - lnMissingUnit;
            
			CDSSTabularDataUnit * lpRefSubsetUnit = NULL;
			hr = CreateReferenceUnit(DssTemplateAttribute, UnitIndex, lpTabularDataUnit2, NULL, &lpRefSubsetUnit);
			CHECK_HR_PTR(lpRefSubsetUnit);
			mvUnits[UnitIndex] = lpRefSubsetUnit;
            */
			return S_OK;
		}
	}
    
    DSSTabularAttribute *lpAttr = new DSSTabularAttribute();
    //zhyang, 636369, we need to specify arity when the number of forms is greater than 0
    hr = lpAttr->Init(iUnitIndex, iUnitForm.nForms > 0 ? 1 : 0);
    CHECK_HR;
    
    MBase::CString lGUIDStr = DSSAEHelper::Guid2String(iUnitID);
    hr = lpAttr->setGUID(lGUIDStr);
    CHECK_HR;
    
    //zhyang, missing unit
    if(!ATL::InlineIsGUIDNULL(iUnitID))
    {
        for(int i = 0; i < iUnitForm.nForms; ++i)
        {
            DSSDataColumn *lpColumn = NULL;
            hr = DSSAEHelper::GenXTabCol(DssDataTypeMissing, 1, &lpColumn);
            CHECK_HR;
            
            hr = lpAttr->AddFormDataColumn(i, DssBaseFormReserved, lpColumn);
            CHECK_HR;
        }
    }
    
    //////////////////////////////////////////////////
    mvUnitType[iUnitIndex] = DssTemplateAttribute;
    mvUnitGUID[iUnitIndex] = iUnitID;
    mvUnitGlobalIndex[iUnitIndex] = -1;
    mvUnits[iUnitIndex] = lpAttr;
    //////////////////////////////////////////////////
	
	*opMissingPos = 0; //jiawwu, for view report the missing unit key as 0
	return S_OK;
}

int DSSTabularData::filterUnit (Int32*pRowForOrginialIND,
                                Int32 nRow,
                                vector<Int32, MBase::Allocator<Int32> >& vRowForLUT,
                                Int32 *opRowForIND,
                                MBase::Buffer *pBuffer)
{
    //## begin CDSSTabularData::filterUnit%36F02047001C.body preserve=yes
	//vRowForIND	array of indexes to the look up table which should make up the indices eventually
	//vRowForLUT	array of rows which should be copied to the lookup table
	//need to preprocess to split it into two arrays
	//if lpRowMap is {0, 1, 3, 1, 3} need to split into
	//{0,1,3} and {0, 1, 2, 1, 2}
    
	AE_ASSERT(pBuffer != NULL);
    
	//find out the max value in the pRowForOrginialIND
	Int32 lMaxIndex = 0;
	Int32 iRow = 0;
	for(iRow = 0; iRow < nRow; iRow ++) {
		if (pRowForOrginialIND[iRow] > lMaxIndex)
			lMaxIndex = pRowForOrginialIND[iRow];
	}
    
	if (lMaxIndex + 1 < nRow * log(double(nRow + 1))) {
		//xhan using map instead of sort
        
		//create the index map
		Int32 *lpMap = new (pBuffer) Int32[lMaxIndex + 1];
		CHECK_NEWED_PTR(lpMap);
        
		memset(lpMap, 0, (lMaxIndex + 1) * sizeof(Int32));
        
		for(iRow = 0; iRow < nRow; iRow ++)
			lpMap[pRowForOrginialIND[iRow]] = 1;
        
		//get unique index numbers to vRowForLUT
		Int32 lNewIndex = 0;
		for(iRow = 0; iRow < lMaxIndex + 1; iRow ++) {
			if (lpMap[iRow] == 1){
				vRowForLUT.push_back(iRow);
				//at same time, change lMap[iRow] to new index
				lpMap[iRow] = lNewIndex;
				lNewIndex ++;
			}
            
		}
        
		//fill opRowForIND with new indices
		for (iRow = 0; iRow < nRow; iRow ++)
			opRowForIND[iRow] = lpMap[pRowForOrginialIND[iRow]];
        
        
	} else { //old logic
		//logic copied from splitVec
		Int32 * lpMap = new (pBuffer) Int32[nRow];
		CHECK_NEWED_PTR(lpMap);
		for(iRow = 0; iRow < nRow; iRow ++)
			lpMap[iRow] = iRow;
        
		DSSAEHelper::IndirectOrder lOrder(pRowForOrginialIND);
		// lwang, 08/14/2007, TQMS 276457: call _xtab_sort_mcm() to use MCM governed buffer
		_xtab_sort(lpMap, lpMap + nRow, lOrder);
        
        
		//now we can go through lpMap
		Int32 lId = -1;	//this way we trick the program to become 0 at the first time
		Int32 lPrev = -1;
		for(iRow = 0; iRow < nRow; iRow ++)
		{
			if(lPrev != pRowForOrginialIND[lpMap[iRow]])
			{
				lPrev = pRowForOrginialIND[lpMap[iRow]];
				vRowForLUT.push_back(lPrev);
				opRowForIND[lpMap[iRow]] = ++lId;
                
			}
			else
				opRowForIND[lpMap[iRow]] = lId;
		}
	}
    
	return S_OK;
}

GUID DSSTabularData::getDataSetGUID()
{
    return mDataSetGUID;
}

map<GUID, vector<int> > * DSSTabularData::getDynamicDimtyByLevel(int iLevel)
{
    if (iLevel >= 0 && iLevel < mvDynamicDimty.size())
    {
        return &(mvDynamicDimty[iLevel]);
    }
    
    return NULL;
}

vector<map<GUID, vector<int> > >* DSSTabularData::getDynamicDimty()
{
    return &mvDynamicDimty;
}

void DSSTabularData::setDynamicDimty(vector<map<GUID, vector<int> > >* ipvDD)
{
    mvDynamicDimty = *ipvDD;
}

vector<map<GUID, DFCSimpleExpr* > >* DSSTabularData::getDynamicBreakBy()
{
    return &mvDynamicSortBy;
}

void DSSTabularData::setDynamicBreakBy(vector<map<GUID, DFCSimpleExpr* > >* ipvDBB)
{
    mvDynamicSortBy = *ipvDBB;
}

// weilin 2016-04-21 DE32160
int DSSTabularData::getUnitLocale(int iUnitId, int &iLocale)
{
    if (iUnitId >= mvLocales.size())
        return S_FALSE;
    iLocale = mvLocales[iUnitId];
    return S_OK;
}
