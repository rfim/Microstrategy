/*
 *  DSSRWFieldGroupModel.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 9/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#include "DSSRWFieldGroupModel.h"
#include "DSSRWSectionNode.h"
#import "DSSViewDataSetHelper.h"
#import "DSSRWSectionModelIterator.h"
#import "DSSRWFieldGroupModelIterator.h"
#import "DSSStrongPtr.h"
#import "MsiCOM/MsiCOM/Msi_atlbase.h"
#import "DSSXTabBitVector.h"
#import "RWDJoin.h"
#import "DSSTabularMetrics.h"
#import "DSSTabularData.h"
#import "DSSXTabViewHelper.h"
#import "DSSRWUtil.h"

#include "DSSGBTable.h"
#import "DSSMaskInfos.h"

#import "DSSDataSource.h"
#import "DSSDataModelFieldGroupImpl.h"
#import "DSSDataModelTransactionRWField.h"
#include <algorithm>
#include "DSSModelPopulation.h"

#include "DSSSQL/DFCCSI.h"
#include "SimpleBuffer.h"
#include "CSITransaction/DFCCSITransactionBase.h"
#include "DSSOfflineTransactionCSIInfo.h"
#include "BinaryDataLock.h"

int TypeIDList::Add(EnumDSSObjectType iType, GUID* ipID)
{
	if (!ipID) return E_INVALIDARG;
    
	size_t lSize = mList.size();
	for (Int32 i = 0; i < static_cast<Int32>(lSize); ++i)
	{
		_ASSERT(mList[i]);
        
		EnumDSSObjectType lType = DssTypeReserved;
		GUID lID = GUID_NULL;
        
		lType = mList[i]->mType;
		lID = mList[i]->mID;
        
        if (lType == iType && ATL::InlineIsEqualGUID(lID, *ipID))
            return S_OK;
	}
    
	TypeIDPair* lpContent = new TypeIDPair;
    
	if (!lpContent) return E_OUTOFMEMORY;
    
	lpContent->mType = iType;
	lpContent->mID = *ipID;
    
	mList.push_back(lpContent);
    
	return S_OK;
}

int TypeIDList::Count(Int32* opCount)
{
	if (!opCount)
		return E_POINTER;
	*opCount = static_cast<Int32>(mList.size());
	return S_OK;
}

int TypeIDList::TypeIDByIndex(Int32 iIndex,  EnumDSSObjectType* opType, GUID* opID)
{
	if (!opType || !opID)
		return E_POINTER;
    
	// 1-based index.
	if (iIndex < 1 || iIndex > static_cast<Int32>(mList.size())) return E_INVALIDARG;
    
	_ASSERT(mList[iIndex - 1]);
	*opType = mList[iIndex - 1]->mType;
	*opID = mList[iIndex - 1]->mID;
    
	return S_OK;
}

int TypeIDList::Clear()
{
	size_t lSize = mList.size();
	for (Int32 i = 0; i < static_cast<Int32>(lSize); i++)
	{
		_ASSERT(mList[i]);
		delete mList[i];
	}
	mList.clear();
    
	return S_OK;
}

int TypeIDList::Find(EnumDSSObjectType iType, GUID* ipID, Int32* oIndex)
{
	if (!oIndex) return E_POINTER;
	if (!ipID) return E_INVALIDARG;
    
	*oIndex = -1;
    
	size_t lSize = mList.size();
	Int32 i = 0;
	for (i = 0; i < static_cast<Int32>(lSize); ++i)
	{
		_ASSERT(mList[i]);
        
		EnumDSSObjectType lType = DssTypeReserved;
		GUID lID = GUID_NULL;
        
		lType = mList[i]->mType;
		lID = mList[i]->mID;
        if (lType == iType && ATL::InlineIsEqualGUID(lID, *ipID))
			break;
	}
    
	if ((i >= 0) && (i < static_cast<Int32>(lSize)))
		*oIndex = i + 1;
    
	return S_OK;
}

int TypeIDList::LoadFromBinary(DSSBinaryDataReader *ipReadStream)
{
    HRESULT hr=S_OK;
    
    int lnList = -1;
    hr = ipReadStream->ReadInt(&lnList);
    CHECK_HR;
    
    mList.clear();
    
    for(int i = 0; i < lnList; ++i)
    {
        int lType=-1;
        hr=ipReadStream->ReadInt(&lType);
        CHECK_HR;
        
        string lGUIDStr;
        hr = ipReadStream->ReadString(lGUIDStr);
        CHECK_HR;
        
        GUID lGUID;
        hr = DSSViewDataSetHelper::str2GUID(lGUIDStr, lGUID);
        CHECK_HR;
        
        TypeIDPair *lpTypeIDPair=new TypeIDPair();
        
        lpTypeIDPair->mType=(EnumDSSObjectType)lType;
        lpTypeIDPair->mID=lGUID;
        
        mList.push_back(lpTypeIDPair);
    }
    
    return S_OK;
}

DSSRTTemplateInfo::DSSRTTemplateInfo(): mbHasMetrics(false), mpUnits(NULL), mpOLUnits(NULL),
mpRuntimeTemplate(NULL), mpExtRuntimeTemplate(NULL), mpOLRuntimeTemplate(NULL), mpOLExtRuntimeTemplate(NULL), mpBaseTemplate(NULL),
mViewPtr(NULL), mOLViewPtr(NULL), mExtendViewPtr(NULL), mpExtendCube(NULL), mpOLExtendCube(NULL),
mpGBTable(NULL), mpOLGBTable(NULL), mOLExtendViewPtr(NULL)
{
    mReportID = GUID_NULL;
	mReportIndex = -1;
}

DSSRTTemplateInfo::~DSSRTTemplateInfo()
{
	mvMetricList.clear();
    
	if (mpUnits)
    {
        delete mpUnits;
        mpUnits = NULL;
    }
    if (mpOLUnits)
    {
        delete mpOLUnits;
        mpOLUnits = NULL;
    }
    
    mvActualGroupByUnits.clear();
    
    if (mpRuntimeTemplate)
    {
        delete mpRuntimeTemplate;
        mpRuntimeTemplate = NULL;
    }
    if (mpExtRuntimeTemplate)
    {
        delete mpExtRuntimeTemplate;
        mpExtRuntimeTemplate = NULL;
    }
    if (mpOLRuntimeTemplate)
    {
        delete mpOLRuntimeTemplate;
        mpOLRuntimeTemplate = NULL;
    }
    if (mpOLExtRuntimeTemplate)
    {
        delete mpOLExtRuntimeTemplate;
        mpOLExtRuntimeTemplate = NULL;
    }
    if (mpBaseTemplate)
    {
        delete mpBaseTemplate;
        mpBaseTemplate = NULL;
    }
    
    if (mViewPtr)
    {
        delete mViewPtr;
        mViewPtr = NULL;
    }
    if (mOLViewPtr)
    {
        delete mOLViewPtr;
        mOLViewPtr = NULL;
    }
    if (mExtendViewPtr)
    {
        delete mExtendViewPtr;
        mExtendViewPtr = NULL;
    }
    
    if (mpGBTable)
    {
        delete  mpGBTable;
        mpGBTable = NULL;
    }
    
    if (mpOLGBTable)
    {
        delete  mpOLGBTable;
        mpOLGBTable = NULL;
    }
    
    if(mOLExtendViewPtr)
    {
        delete mOLExtendViewPtr;
        mOLExtendViewPtr = NULL;
    }
    
    mpOLExtendCube = NULL;
}

int DSSRTTemplateInfo::LoadFromBinary(DSSBinaryDataReader *ipReadStream, DSSGUnitContainer* ipGUnitContainer, DSSCMNodeModel* ipNodeModelCacheManager)
{
    HRESULT hr=S_OK;
    
    string lGUIDStr;
    hr = ipReadStream->ReadString(lGUIDStr);
    CHECK_HR;
    
    hr = DSSViewDataSetHelper::str2GUID(lGUIDStr, mReportID);
    CHECK_HR;
    
    hr=ipReadStream->ReadInt(&mReportIndex);
    CHECK_HR;
    
    hr=ipReadStream->ReadBool(&mbHasMetrics);
    CHECK_HR;
    
    int lnMetricList=-1;
    hr=ipReadStream->ReadInt(&lnMetricList);
    CHECK_HR;
    
    mvMetricList.clear();
    for(int i=0;i<lnMetricList;i++)
    {
        string lGUIDStr;
        hr = ipReadStream->ReadString(lGUIDStr);
        CHECK_HR;
        
        GUID lGUID;
        hr = DSSViewDataSetHelper::str2GUID(lGUIDStr, lGUID);
        CHECK_HR;
        
        mvMetricList.push_back(lGUID);
    }
    
    int lFlagForMpUnits=0;
    hr=ipReadStream->ReadInt(&lFlagForMpUnits);
    CHECK_HR;
    
    mpUnits=NULL;
    
    if(lFlagForMpUnits==1)
    {
        mpUnits=new TypeIDList();
        hr=mpUnits->LoadFromBinary(ipReadStream);
        CHECK_HR;
    }
    
    int lFlagForMpOLUnits=0;
    hr=ipReadStream->ReadInt(&lFlagForMpOLUnits);
    CHECK_HR;
    
    mpOLUnits=NULL;
    
    if(lFlagForMpOLUnits==1)
    {
        mpOLUnits=new TypeIDList();
        hr=mpOLUnits->LoadFromBinary(ipReadStream);
        CHECK_HR;
    }
    
    int lFlagForViewPtr=0;
    hr=ipReadStream->ReadInt(&lFlagForViewPtr);
    CHECK_HR;
    
    // mViewPtr=NULL;
    if(lFlagForViewPtr==1)
    {
        // 8/24/2012 tehe
        if(!mViewPtr)
        {
            mViewPtr = new DSSViewDataSet();
        }
        mViewPtr->hInit();
        
        mViewPtr->setNodeModelCacheManager(ipNodeModelCacheManager);
        
        hr=mViewPtr->loadFromBinary(ipReadStream);
        CHECK_HR;
    }
    
    bool lfGBTable;
    hr = ipReadStream->ReadBool(&lfGBTable);
    CHECK_HR;
    
    if (lfGBTable)
    {
        if (mpGBTable)
        {
            delete mpGBTable;
            mpGBTable = NULL;
        }
        mpGBTable = new DSSGBTable();
        int lType;
        hr = ipReadStream->ReadInt(&lType);
        CHECK_HR;
        
        hr = mpGBTable->loadFromBinary(ipReadStream);
        CHECK_HR;
    }
    else
        mpGBTable = NULL;
    
    if (mpGBTable)
    {
        mpGBTable->mpGUnitContainer = ipGUnitContainer;
    }
    
    int lFlagForOLViewPtr=0;
    hr=ipReadStream->ReadInt(&lFlagForOLViewPtr);
    CHECK_HR;
    
    // mOLViewPtr=NULL;
    if(lFlagForOLViewPtr==1)
    {
        // 8/24/2012 tehe
        if(!mOLViewPtr)
        {
            mOLViewPtr = new DSSViewDataSet();
        }
        mOLViewPtr->hInit();
        
        mOLViewPtr->setNodeModelCacheManager(ipNodeModelCacheManager);
        
        hr=mOLViewPtr->loadFromBinary(ipReadStream);
        CHECK_HR;
    }
    
    lfGBTable = false;
    hr = ipReadStream->ReadBool(&lfGBTable);
    CHECK_HR;
    
    if (lfGBTable)
    {
        if (mpOLGBTable)
        {
            delete mpOLGBTable;
            mpOLGBTable = NULL;
        }
        mpOLGBTable = new DSSGBTable();
        int lType;
        hr = ipReadStream->ReadInt(&lType);
        CHECK_HR;
        
        hr = mpOLGBTable->loadFromBinary(ipReadStream);
        CHECK_HR;
    }
    else
        mpOLGBTable = NULL;
    
    if (mpOLGBTable)
    {
        mpOLGBTable->mpGUnitContainer = ipGUnitContainer;
    }
    
    //zhyang, 01/13/2012
    int lExistExtendView;
    hr=ipReadStream->ReadInt(&lExistExtendView);
    CHECK_HR;
    
    if (lExistExtendView == 1) {
        // 8/24/2012 tehe
        if (!mExtendViewPtr)
        {
            mExtendViewPtr = new DSSViewDataSet();
        }
        mExtendViewPtr->hInit();
        
        mExtendViewPtr->setNodeModelCacheManager(ipNodeModelCacheManager);
        
        hr = mExtendViewPtr->loadExtendFromBinary(ipReadStream);
        CHECK_HR;
        
        mpExtendCube = mExtendViewPtr->GetCube();
    }
    
    //zhyang, 12/10/2012, OLExtendedCube
    int lExistingOLExtendedView;
    hr = ipReadStream->ReadInt(&lExistingOLExtendedView);
    CHECK_HR;
    
    if(lExistingOLExtendedView == 1)
    {
        if(!mOLExtendViewPtr)
        {
            mOLExtendViewPtr = new DSSViewDataSet();
        }
        
        mOLExtendViewPtr->hInit();
        
        mOLExtendViewPtr->setNodeModelCacheManager(ipNodeModelCacheManager);
        
        hr = mOLExtendViewPtr->loadExtendFromBinary(ipReadStream);
        CHECK_HR;
        
        mpOLExtendCube = mOLExtendViewPtr->GetCube();
    }
    
    return hr;
}

int DSSRTTemplateInfo::GetReportID(GUID* opReportID)
{
	if (!opReportID)
		return E_POINTER;
    
	*opReportID = mReportID;
    
	return S_OK;
}

int DSSRTTemplateInfo::HasMetrics(bool* opHasMetrics)
{
	if (!opHasMetrics)
		return E_POINTER;
    
	*opHasMetrics = mbHasMetrics;
    
	return S_OK;
}

int DSSRTTemplateInfo::Count(Int32* opCount)
{
	if (!opCount)
		return E_POINTER;
    
	*opCount = static_cast<Int32>(mvMetricList.size());
    
	return S_OK;
}

int DSSRTTemplateInfo::GetMetricByIndex(Int32 iIndex, GUID* opMetricID)
{
	if (!opMetricID)
		return E_POINTER;
    
	if (iIndex < 1 || iIndex >static_cast<Int32>(mvMetricList.size()))
		return E_INVALIDARG;
    
	*opMetricID = mvMetricList[iIndex - 1];
    
	return S_OK;
}


int DSSRTTemplateInfo::AddMetric(GUID iMetricID)
{
	if (!mbHasMetrics)
		mbHasMetrics = true;
    
	mvMetricList.push_back(iMetricID);
    
	return S_OK;
}

int DSSRTTemplateInfo::GetUnits(TypeIDList** oppUnits)
{
	if (!oppUnits)
		return E_POINTER;
    
	*oppUnits = mpUnits;
    
	return S_OK;
}

void DSSRTTemplateInfo::hPartialCleanUp()
{
    mvMetricList.clear();
    
	if (mpUnits)
    {
        delete mpUnits;
        mpUnits = NULL;
    }
    if (mpOLUnits)
    {
        delete mpOLUnits;
        mpOLUnits = NULL;
    }
    
    mvActualGroupByUnits.clear();
    
    if (mpRuntimeTemplate)
    {
        delete mpRuntimeTemplate;
        mpRuntimeTemplate = NULL;
    }
    if (mpExtRuntimeTemplate)
    {
        delete mpExtRuntimeTemplate;
        mpExtRuntimeTemplate = NULL;
    }
    if (mpOLRuntimeTemplate)
    {
        delete mpOLRuntimeTemplate;
        mpOLRuntimeTemplate = NULL;
    }
    if (mpOLExtRuntimeTemplate)
    {
        delete mpOLExtRuntimeTemplate;
        mpOLExtRuntimeTemplate = NULL;
    }
    if (mpBaseTemplate)
    {
        delete mpBaseTemplate;
        mpBaseTemplate = NULL;
    }
    
    // 8/24/2012
    if (mViewPtr)
    {
        mViewPtr->hPartialCleanUp();
    }
    if (mOLViewPtr)
    {
        mOLViewPtr->hPartialCleanUp();
    }
    if (mExtendViewPtr)
    {
        mExtendViewPtr->hPartialCleanUp();
    }
    
    if (mOLExtendViewPtr)
    {
        mOLExtendViewPtr->hPartialCleanUp();
    }
    
}
//fali, only used for smart client.
void DSSRTTemplateInfo::CleanExtendView()
{
    if (mExtendViewPtr)
    {
        delete mExtendViewPtr;
        mExtendViewPtr = NULL;
        mpExtendCube = NULL;
    }
}

DSSRWFieldGroupModel::DSSRWFieldGroupModel(bool ibOnDemandLoading) : mpBuffer(NULL), mIncludeCSI(false), DSSRWDDICModel(NULL)
{
    mbOnDemandLoading = ibOnDemandLoading;
}

DSSRWFieldGroupModel::DSSRWFieldGroupModel(DSSRWDataModel *ipDataModel, bool ibOnDemandLoading) : mpBuffer(NULL), mIncludeCSI(false), DSSRWDDICModel(ipDataModel)
{
    mpDataModel = ipDataModel;
    mbOnDemandLoading = ibOnDemandLoading;
}

DSSRWFieldGroupModel::~DSSRWFieldGroupModel()
{
    hCleanUp(true);
}

int DSSRWFieldGroupModel::hInitTabularUnitInViewDataSet()
{
    DSSGUnitContainer *ipGlobalLookupContainer;
    mpDataModel->GetGlobalLookupContainer(&ipGlobalLookupContainer);
    
    int lnRTTemplateInfo = mvpRTTemplateInfo.size();
    for (int i = 0; i < lnRTTemplateInfo; ++i)
    {
        if (NULL != mvpRTTemplateInfo[i]->mViewPtr)
            mvpRTTemplateInfo[i]->mViewPtr->Init(ipGlobalLookupContainer);
        if (NULL != mvpRTTemplateInfo[i]->mOLViewPtr)
            mvpRTTemplateInfo[i]->mOLViewPtr->Init(ipGlobalLookupContainer);
        if (NULL != mvpRTTemplateInfo[i]->mExtendViewPtr)
            mvpRTTemplateInfo[i]->mExtendViewPtr->Init(ipGlobalLookupContainer);
        if (NULL != mvpRTTemplateInfo[i]->mOLExtendViewPtr)
            mvpRTTemplateInfo[i]->mOLExtendViewPtr->Init(ipGlobalLookupContainer);
    }
    
    return S_OK;
}

int DSSRWFieldGroupModel::LoadDataFromBinary(DSSBinaryDataReader *ipReadStream)
{
    // 5/28/2012 tehe: now we only support disk storage on model level, maybe we will support on slice level later as grid model
    int hr = S_OK;
    
    int lBlockType = 0;
    hr = ipReadStream->BeginReadBlock(&lBlockType);
    CHECK_HR_BLOCK(EnumNewBlockFieldGroupModel);
    
    hr = DSSRWNodeModel::LoadDataFromBinary(ipReadStream);
    CHECK_HR;
    
    if(ipReadStream->ProbeBlockHeader(EnumNewBlockCSIs))
    {
        hr = ipReadStream->BeginReadBlock(&lBlockType);
        CHECK_HR_BLOCK(EnumNewBlockCSIs);
        
        int lnTemplateInfo;
        hr = ipReadStream->ReadInt(&lnTemplateInfo);
        CHECK_HR;
        
        if (mpBuffer)
        {
            delete mpBuffer;
            mpBuffer = NULL;
        }
        MBase::SimpleBuffer *lpBuffer = new MBase::SimpleBuffer();
        mpBuffer = lpBuffer;
        
        if (lnTemplateInfo > 0)
        {
            // weilin DE39756 2016/09/29 duplicate new cause memory leak, just remove it
            // lpBuffer = new MBase::SimpleBuffer();
        
            for (int i = 0; i < lnTemplateInfo; i++)
            {
                int lFlag;
                hr = ipReadStream->ReadInt(&lFlag);
                CHECK_HR;
            
                if (1 == lFlag)
                {
                    DSSReadBlockStream<DSSBinaryDataReader> lReadBlockStream (*ipReadStream);
                    DFCCSI * lpCSI = new (lpBuffer) DFCCSI(lpBuffer);
                    hr = lpCSI->Unserialize(&lReadBlockStream);
                    CHECK_HR;
            
                    mvpCSI.push_back(lpCSI);
                    mIncludeCSI = true;
                    
                    if (lpCSI)
                    {
                        MBase::ReturnWString lCSI_Text = lpCSI->getText(lpBuffer);
                        MODEL_DEBUG_LOG("DSSRWFieldGroupModel", "One of the CSIs for FG node", mNodeKey.c_str(), lCSI_Text.c_str(), lCSI_Text.GetLength())
                    }
                }
                else
                    mvpCSI.push_back(NULL);

            }
                
            mpDataModel->addNodeKey(mNodeKey);
        }
        
        if (ipReadStream->MoreDataInBlock())
        {
            int lnDDICModel;
            hr = ipReadStream->ReadInt(&lnDDICModel);
            CHECK_HR;
            
            DSSReadBlockStream<DSSBinaryDataReader> lReadBlockStream (*ipReadStream);
            
            for(int i = 0; i < lnDDICModel; ++i)
            {
                MBase::CString lGUIDStr;
                hr = ipReadStream->ReadString(lGUIDStr);
                CHECK_HR;
                
                GUID lReportGUID;
                hr = DSSViewDataSetHelper::str2GUID(lGUIDStr, lReportGUID);
                CHECK_HR;
                mvDatasetIDForDDIC.push_back(lReportGUID);
                
                DFCCSI * lpCSIForDDIC = new (mpBuffer) DFCCSI(mpBuffer);
                hr = lpCSIForDDIC->Unserialize(&lReadBlockStream);
                CHECK_HR;
                mvCSIForDDIC.push_back(lpCSIForDDIC);
                
                if (lpCSIForDDIC)
                {
                    char lnum[] = {'0', '\0'};
                    lnum[0] += i;
                    MBase::ReturnWString lCSI_Text = lpCSIForDDIC->getText(lpBuffer);
                    MODEL_DEBUG_LOG("DSSRWFieldGroupModel", "CSI for Grid's DDIC #", lnum, lCSI_Text.c_str(), lCSI_Text.GetLength())
                }
            }
        }
        
        hr = ipReadStream->EndReadBlock();
        CHECK_HR;
    }

    
    int lInitialLoading;
    hr = ipReadStream->ReadInt(&lInitialLoading);
    CHECK_HR;
    
    if(0 == lInitialLoading)
    {
        IsInitlaLoading = true;
        
        hr = hInitialLoading(ipReadStream);
        CHECK_HR;
    }
    else
    {
        IsInitlaLoading = false;
        
        hr = hFullBinaryLoading(ipReadStream);
        CHECK_HR;
    }
    
    if(ipReadStream->MoreDataInBlock())
    {
        hr = ipReadStream->BeginReadBlock(&lBlockType);
        CHECK_HR;
        
        int lnDDICModel;
        hr = ipReadStream->ReadInt(&lnDDICModel);
        CHECK_HR;
        
        if(lnDDICModel > 0)
        {
            mvDDICIndex.clear();
            mvvDDICAttr.clear();
        }
        for(int i = 0; i < lnDDICModel; ++i)
        {
            MBase::CString lControlKey;
            hr = ipReadStream->ReadString(lControlKey);
            CHECK_HR;
            
            string lkey = lControlKey;
            mControlKeyToIndexMap[lkey] = i;
            
            DSSTabularIndex *lpIndex;
            hr = DSSViewDataSetHelper::loadIndex(ipReadStream, &lpIndex);
            CHECK_HR;
            
            mvDDICIndex.push_back(lpIndex);
            //mpDataModel->InsertDDICIndex(lpIndex);
            int lUnitsSize;
            hr = ipReadStream->ReadInt(&lUnitsSize);
            CHECK_HR;
            
            vector<DSSTabularAttribute *> lvAttr;

            for(int i = 0; i < lUnitsSize; ++i)
            {
                DSSTabularUnit *lpUnit = NULL;
                int lType;
                hr= ipReadStream->ReadInt(&lType);
                CHECK_HR;
                
                DSSTabularAttribute *lpAttr = NULL;
                if(lType == 1)
                {
                    int lUnitType, lUnitID;
                    hr = ipReadStream->ReadInt(&lUnitType);
                    CHECK_HR;
                    
                    hr = ipReadStream->ReadInt(&lUnitID);
                    CHECK_HR;
                    
                    if(lUnitType == EnumObjectDummyUnit)//xiazhou, 927901, handle missing unit.
                    {
                        DSSTabularAttribute *lpAttribute = new DSSTabularAttribute();
                        hr = lpAttribute->Init(lUnitID, 0);
                        if (hr == S_OK)
                            lpAttr = lpAttribute;
                        else
                        {
                            delete lpAttribute;
                        }
                    }
                    else
                    {
                        hr = DSSModelPopulation::LoadAttributeForDDIC(ipReadStream, i, &lpUnit);
                        CHECK_HR;
                        
                        DSSTabularAttribute *lpAttribute = mpDataModel->GetTabularAttributeByGUID(lpUnit->getGUID());
                        if(!lpAttribute) //xiazhou, 947410, handle missing unit.
                        {
                            lpAttribute = new DSSTabularAttribute();
                            hr = lpAttribute->Init(lUnitID, 0);
                            if (hr == S_OK)
                                lpAttr = lpAttribute;
                            else
                                delete lpAttribute;
                        }
                        else
                        {
                            DSSGlobalLookUpTable* lpLookUp=dynamic_cast<DSSGlobalLookUpTable *>(lpAttribute);
                            if(lpLookUp)
                            {
                                lpAttr = static_cast<DSSTabularAttribute *>(lpUnit);
                                lpAttr->Init(i, lpLookUp->getArity());
                            }
                        }
                    }
                }
                lvAttr.push_back(lpAttr);
                //mpDataModel->InsertDDICAttr(lpAttr);
                
            }
            
            mvvDDICAttr.push_back(lvAttr);
        }
        
        hr = ipReadStream->EndReadBlock();
        CHECK_HR;
    }
    
    if(!mControlKeyToIndexMap.empty())//xiazhou, 909509. Clear DDIC cache.
    {
        map<string, int>::iterator ite = mControlKeyToIndexMap.begin();
        
        while(ite != mControlKeyToIndexMap.end())
        {
            string lDICKey = ite->first;
            map<int, string> *lpTextMap = mpDataModel->getDisplayTextMap(lDICKey);
            if(lpTextMap)
                lpTextMap->clear();
            
            map<int,DSSBaseElementProxy*> *lpProxyMap = mpDataModel->getElementProxyMap(lDICKey);
            if(lpProxyMap)
            {
                map<int,DSSBaseElementProxy*>::iterator lIt= lpProxyMap->begin();
                
                while(lIt!=lpProxyMap->end())
                {
                    delete lIt->second;
                    
                    lIt->second=NULL;
                    
                    lIt++;
                }
                
                lpProxyMap->clear();
            }
            ite++;
        }
    }
    
    hr = ipReadStream->EndReadBlock();
    CHECK_HR;
    
    // jimzhang dec/22/2011 Initialzie XTabView for view datasets of FG. Don't return error at this step.
    // tliao, 09/16/2013, don't do initialization when there is CSI existing, we will do it at the end of view evaluation
    if (mvpCSI.size() == 0)
        InitXTabView();
    
    return hr;
}

int DSSRWFieldGroupModel::createIterator(DSSRWSectionModelIterator *ipParentIterator, DSSRWNodeModelIterator **oppIterator)
{
	int hr = S_OK;
    
	//hr = hCreateXTabView();       // jimzhang oct/24/2011 Don't implement temporarily.
	//CHECK_HR;
    
	//hr = hCreateExtendCubes();    // jimzhang oct/24/2011 Don't implement temporarily.
	//CHECK_HR;
    
	DSSRWFieldGroupModelIterator * lpFieldGroupIterator = new DSSRWFieldGroupModelIterator();
	if (!lpFieldGroupIterator)
        return E_OUTOFMEMORY;
    
    // 5/28/2012 tehe:
    if(mbOnDemandLoading && !mpNode)
    {
        DSSObjectContext *lpContext = mpDataModel->getObjectContext();
        mpNode = lpContext->getNode(mNodeKey);
        CHECK_PTR(mpNode);
    }
    
	hr = lpFieldGroupIterator->Init(mpDataModel, this, mpNode, ipParentIterator);
	CHECK_HR;
    
	*oppIterator = lpFieldGroupIterator;
    
	return S_OK;
}

int DSSRWFieldGroupModel::getCandidateElements(vector<GUID>& ipSelectedUnits,
                                               vector<vector<int> >& ipSelectedElements,
                                               GUID iUnit, vector<int>& opElements)
{
    int hr = S_OK;
    
	Int32 lnSelectedUnits = ipSelectedUnits.size();
    
	Int32 lnTemplateCount = mvpRTTemplateInfo.size();
	if(!lnTemplateCount) // static fields only, no metric data
		return S_OK;
    
    //tliao, 07/22/2013, 773354, add primary flag into consideration when calculate candidate element
    vector<Int32> lvPrimaryTemplate, lvSecondaryTemplate;
    lvPrimaryTemplate.reserve(lnTemplateCount);
    lvSecondaryTemplate.reserve(lnTemplateCount);
    
    Int32 lGlobalUnitID = -1;
    lGlobalUnitID = mpDataModel->FindUnit(iUnit);
    
    DSSGUnitContainer* lpGUnitContainer;
    hr = mpDataModel->GetGlobalLookupContainer(&lpGUnitContainer);
    CHECK_HR;
    
    Int32 lnGlobalRowCount = -1;
    if (lGlobalUnitID >= 0)
    {
        bool lfIncludeDE = true;
        hr = lpGUnitContainer->GetGlobalRowCount(lGlobalUnitID, &lnGlobalRowCount, lfIncludeDE);
        CHECK_HR;
    }
    else
        lnGlobalRowCount = 0;
    
    lnGlobalRowCount++;
    
    DSSXTabBitVector lResultBV(lnGlobalRowCount);
    if (lnGlobalRowCount > 0)
        lResultBV.BulkSet(0, lnGlobalRowCount-1, false);
    
    
    DSSXTabBitVector lTmpBV(lnGlobalRowCount);
    if (lnGlobalRowCount > 0)
        lTmpBV.BulkSet(0, lnGlobalRowCount-1, false);
    
    vector<Int32> lvElement;
    
    vector<Int32> lvPrimary, lvSecondary;
    
    for(Int32 lTemplate = 0; lTemplate < lnTemplateCount; ++lTemplate)
    {
        if (!mvpRTTemplateInfo[lTemplate]->mViewPtr)
            continue;
        
        if (mvPrimaryFlag[lTemplate] != 0)
            lvPrimary.push_back(lTemplate);
        else
            lvSecondary.push_back(lTemplate);
    }
    
    for (Int32 i = 0; i < lvSecondary.size(); i++)
        lvPrimary.push_back(lvSecondary[i]);
    
    //junwang, 2012/07/26, TQMS 635839
    
    //TQMS 712817, hyan, 1/28/2013
    //we need to init to false following with server side
    bool lfTotalIncluded = false;
    bool lfPrimary = false;
    
	for (Int32 iTemplate = 0; iTemplate < lvPrimary.size(); iTemplate++)
	{
        int i = lvPrimary[iTemplate];
        
        if (!mvpRTTemplateInfo[i]->mViewPtr)
            continue;
        
        bool lfCurrentPrimary = mvPrimaryFlag[i] != 0 ? true : false;
        lfPrimary |= lfCurrentPrimary;
        
        vector<Int32> lvSelectedUnitIDs;
		lvSelectedUnitIDs.resize(lnSelectedUnits);
        
        if (mvpRTTemplateInfo[i]->mpGBTable)
        {
            hr = mvpRTTemplateInfo[i]->mpGBTable->GetAvalibleElement(ipSelectedUnits, ipSelectedElements, iUnit, lvElement);
            CHECK_HR;
            
            for (int j = 0; j < lvElement.size(); j++)
            {
                if (lvElement[j] == DSSXTAB_TOTAL_KEY)
                {
                    lfTotalIncluded = true;
                    continue;
                }
                lTmpBV.Set(lvElement[j], true);
            }
        }
        else
        {
            //zhyang, BOA issue
            //according to Linda, we should consider OLExtendedViewDataset first, then OLViewDataset
            //if OL don't contain the unit, we should get from extendedviewDataset (if non, viewdataset) to get element list
            DSSViewDataSet *lpViewDataSet = mvpRTTemplateInfo[i]->mOLExtendViewPtr;
            if(!lpViewDataSet)
            {
                lpViewDataSet = mvpRTTemplateInfo[i]->mOLViewPtr;
            }
            
            if(lpViewDataSet)
            {
                int lLocalUnit;
                hr = lpViewDataSet->getTabularData()->FindUnitI(iUnit, DssTemplateReserved, lLocalUnit);
                if(hr != S_OK)
                    lpViewDataSet = NULL;
            }
            
            if(!lpViewDataSet)
            {
                lpViewDataSet = mvpRTTemplateInfo[i]->mExtendViewPtr;
            }
            
            if(!lpViewDataSet)
            {
                lpViewDataSet = mvpRTTemplateInfo[i]->mViewPtr;
            }
            
            if(!lpViewDataSet)
                continue;
            
            int lLocalUnit;
            hr = lpViewDataSet->getTabularData()->FindUnitI(iUnit, DssTemplateReserved, lLocalUnit);
            // #547698 2/10/2012 tehe here we need checking whether we have found the unit
            if(hr != S_OK)
                continue;
            
            DSSCube *lpCube = lpViewDataSet->GetCube();
            CHECK_PTR(lpCube);
            
            DSSTabularIndexPool *lpIndexPool = NULL;
            hr = lpCube->getIndexPool(&lpIndexPool);
            CHECK_HR;
            
            DSSTabularIndex *lpMainIndex = NULL;
            hr = lpIndexPool->getMainIndex(&lpMainIndex);
            CHECK_HR;
            
            Int32 lnRow = lpMainIndex->Count();
            
            //junwang, 2012/07/26
            if(0 == lnRow || 0 == lpMainIndex->getNumUnits())
                continue;
            
            DSSXTabBitVector lMask;
            // find out which rows are selected
            if(lnSelectedUnits > 0)
            {
                vector<int> lvEffectiveUnit;
                vector<vector<int> > lvEffectiveUnitElements;
                
                for(Int32 j = 0; j < lnSelectedUnits; ++j)
                {
                    Int32 lUnitID;
                    hr = lpViewDataSet->getTabularData()->FindUnitI(ipSelectedUnits[j], DssTemplateReserved, lUnitID);
                    if (hr != S_OK)// if not found, insert -1 as invalid unit
                    {
                        lvSelectedUnitIDs[j] = -1;
                    }
                    else
                    {
                        lvSelectedUnitIDs[j] = lUnitID;
                        //hyan,2012/03/05
                        //Temporary fix of TQMS 539529
                        //Accroding to the logic of CacculateMask, we need to comment the code below
                        
                        // rzhu, 07/12/2011, 489045, check selected elements, if total is selected, we take it as all
                        
                        bool lbHasTotal=false;
                        
                        Int32 lnElements = ipSelectedElements[j].size();
                        if ( lnElements > 0)
                        {
                            for (Int32 k = 0; k < lnElements; k++)
                            {
                                if (ipSelectedElements[j][k] == DSSXTAB_TOTAL_KEY)
                                {
                                    //lvSelectedUnitIDs[j] = -1;
                                    lbHasTotal=true;
                                    break;
                                }
                            }
                        }
                        
                        if(!lbHasTotal)
                        {
                            lvEffectiveUnit.push_back(lUnitID);
                            lvEffectiveUnitElements.push_back(ipSelectedElements[j]);
                        }
                    }
                }
                
                //hr = lpMainIndex->CalculateMask(lnSelectedUnits, &lvSelectedUnitIDs[0], &ipSelectedElements[0], lMask);
                
                hr = lpMainIndex->CalculateMask(lvEffectiveUnit.size(), &lvEffectiveUnit[0], &lvEffectiveUnitElements[0], lMask);
                CHECK_HR;
            }
            else
            {
                lMask.Init(lnRow, false);
                if (lnRow > 0)
                {
                    lMask.BulkSet(0, lnRow - 1, true);
                }
            }
            
            // #547698 tehe 2/9/2012 here we need consider whether a element can display
            DSSTabularData *lpTabularData = NULL;
            hr = lpViewDataSet->getTabularData(&lpTabularData);
            CHECK_HR;
            
            hr = hCheckDisplayable(lpTabularData, lpMainIndex, &lMask);
            CHECK_HR;
            
            int NumUnits = lpMainIndex->getNumUnits();
            //int* lpUnits = new int[NumUnits];
            MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lUnitsPtr (new int[NumUnits]);
            CHECK_PTR(lUnitsPtr.Get());
            //hr = lpMainIndex->getUnits(NumUnits, lpUnits);
            hr = lpMainIndex->getUnits(NumUnits, lUnitsPtr.Get());
            CHECK_HR;
            int lUnitPosInIndex = -1;
            for (int i = 0; i < NumUnits; i++)
            {
                //if (lpUnits[i] == lLocalUnit)
                if (lUnitsPtr.Get()[i] == lLocalUnit)
                {
                    lUnitPosInIndex = i;
                    break;
                }
            };
            
            if (-1 == lUnitPosInIndex)
                continue;
            
            lMask.SetSeekPosition(0);
            Int32 iRow = lMask.GetNextSetPosition();
            while (iRow > -1 && iRow < lnRow)
            {
                Int32 pKey = lpMainIndex->getKey(iRow, lUnitPosInIndex);
                lTmpBV.Set(pKey, true);
                iRow = lMask.GetNextSetPosition();
            }
            
            if (!lfTotalIncluded && lMask.IsAnyTrue(0, lnRow - 1))
                lfTotalIncluded = true;
        }
        
        if (iTemplate == 0 || lfCurrentPrimary)
            lResultBV.CalculateOr(&lTmpBV);
        else if (!lfPrimary)    //only do inner join when all datasets are secondary
            lResultBV.CalculateAnd(&lTmpBV);
        
        if (lnGlobalRowCount > 0)
            lTmpBV.BulkSet(0, lnGlobalRowCount - 1, false);
	}
    
    Int32 lSize = lResultBV.GetSetSize();
    opElements.resize(lSize);
    
    lResultBV.SetSeekPosition(0);
    Int32 lEle = lResultBV.GetNextSetPosition();
    Int32 lLen = 0;
    while (lEle >= 0 && lEle < lnGlobalRowCount)
    {
        opElements[lLen++] = lEle;
        lEle = lResultBV.GetNextSetPosition();
    }
    
    //junwang, 2012/07/26, TQMS 635839
    if(lfTotalIncluded)
        opElements.push_back(DSSXTabTotalKey);
    
	return S_OK;
}

// jimzhang, Feb/18/2013, tqms 715065, if FG contains DDIC controls and is targeted by UC selector, and when UC selector making a change, new FG node model will be serialized from server side, however, mControlKeyToIndexMap and mvDDICIndex are serialized from server side only when first opening the document or change UC selector. That's why we should copy mControlKeyToIndexMap and mvDDICIndex from old FG node model to new one.
int DSSRWFieldGroupModel::initFromOldModel(DSSRWNodeModel* ipOldModel)
{
    DSSRWFieldGroupModel *lpOldFGModel = dynamic_cast<DSSRWFieldGroupModel *>(ipOldModel);
    if (! lpOldFGModel)
        return S_FALSE;
    
    // TQMS 756330, when changing UC selector and loading data, the partial node model and full node model will be set to DataModel before loading their data, so it's necessary to ensure node model's data has been loaded here.
    int hr = lpOldFGModel->EnsureModelData();
    CHECK_HR;
    
    if (! mControlKeyToIndexMap.size() && lpOldFGModel->mControlKeyToIndexMap.size())
    {
        mControlKeyToIndexMap.insert(lpOldFGModel->mControlKeyToIndexMap.begin(), lpOldFGModel->mControlKeyToIndexMap.end());
        mvDDICIndex.assign(lpOldFGModel->mvDDICIndex.begin(), lpOldFGModel->mvDDICIndex.end());
        mvvDDICAttr.assign(lpOldFGModel->mvvDDICAttr.begin(), lpOldFGModel->mvvDDICAttr.end());
    }
    
    return S_OK;
}

int DSSRWFieldGroupModel::hSliceAndJoin(vector<GUID>& ipSelectedUnits, vector<vector<int> >& ipSelectedElements,
                                        bool ibUseOLViews, JoinedIndexTable** oppJoinResult,
                                        bool iRetrieveSortUnits, vector<Int32>& orvSortUnits)
{
    int hr = S_OK;
    
    // todo: remove duplicated selected units if any
    
    Int32 lnTemplateCount = mvpRTTemplateInfo.size();
	if(!lnTemplateCount)
	{
		// static fields only
		*oppJoinResult = NULL;
		return S_OK;
	}
    //DE28818,msun, 2/18/2016, to avoid possible crash
    if (ipSelectedElements.size()<ipSelectedUnits.size())
    {
        *oppJoinResult = NULL;
        return S_OK;
    }
        
    Int32 lnSelectedUnits = ipSelectedUnits.size();
    
    vector<DSSCube*> lvCubes;
    vector<DSSViewDataSet *> lvViewDatasets;
    vector<vector<IndexMask*> > lvIndexMasks;
    
    vector<char> lvPrimary;
	// rzhu, 06/24/2011, primary datasets should be joined before secondary datasets
	Int32 lnPrimaryCount = 0;
	Int32 lInsertPos = 0;
    
    for (Int32 iTemplate = 0; iTemplate < lnTemplateCount; iTemplate++)
	{
        DSSViewDataSet *lpViewDataSet = NULL;
        if (ibUseOLViews)
            lpViewDataSet = mvpRTTemplateInfo[iTemplate]->mOLExtendViewPtr ? mvpRTTemplateInfo[iTemplate]->mOLExtendViewPtr : mvpRTTemplateInfo[iTemplate]->mOLViewPtr;
        else
            lpViewDataSet = mvpRTTemplateInfo[iTemplate]->mExtendViewPtr ? mvpRTTemplateInfo[iTemplate]->mExtendViewPtr : mvpRTTemplateInfo[iTemplate]->mViewPtr;
        
        if (! lpViewDataSet)
            continue;
        
        DSSGBTable* lpGBTable = ibUseOLViews ? mvpRTTemplateInfo[iTemplate]->mpOLGBTable : mvpRTTemplateInfo[iTemplate]->mpGBTable;
        
        bool lIsPrimary = (0 != mvPrimaryFlag[iTemplate]) ? true : false;
        lInsertPos = lIsPrimary ? lnPrimaryCount : lvPrimary.size();
		if (lIsPrimary)
        {
            lnPrimaryCount++;
			lvPrimary.insert(lvPrimary.begin() + lInsertPos, true);
        }
		else
        {
			lvPrimary.push_back(false);
        }
        
        lvViewDatasets.insert(lvViewDatasets.begin() + lInsertPos, lpViewDataSet);
        
        DSSCube *lpCube = NULL;
		if (ibUseOLViews)
		{
			if (mvpRTTemplateInfo[iTemplate]->mpOLExtendCube)
				lpCube = mvpRTTemplateInfo[iTemplate]->mpOLExtendCube;
			else
				lpCube = mvpRTTemplateInfo[iTemplate]->mOLViewPtr->GetCube();
		}
		else
		{
			if (mvpRTTemplateInfo[iTemplate]->mpExtendCube)
				lpCube = mvpRTTemplateInfo[iTemplate]->mpExtendCube;
			else
				lpCube = mvpRTTemplateInfo[iTemplate]->mViewPtr->GetCube();
		}
        
        lvCubes.insert(lvCubes.begin() + lInsertPos, lpCube);
        
        vector<IndexMask*> lvIndexMask;
		lvIndexMasks.insert(lvIndexMasks.begin() + lInsertPos, lvIndexMask);
        
        DSSTabularIndexPool* lpIndicesPool = NULL;
        hr = lpCube->getIndexPool(&lpIndicesPool);
		CHECK_HR;
        
        DSSTabularIndex *lpMainIndex = NULL;
        hr = lpIndicesPool->getMainIndex(&lpMainIndex);
        CHECK_PTR(lpMainIndex);
        
        Int32 lnUnits = lpMainIndex->getNumUnits();
        vector<Int32> lvUnits;
        if (lnUnits > 0)
        {
            hr = lpMainIndex->getUnits(lvUnits);
            CHECK_HR;
        }
        
        DSSTabularData *lpTabularData = lpViewDataSet->getTabularData();
        Int32 lDummyUnitIndex = -1;
        hr = lpTabularData->getDummyUnitIndex(&lDummyUnitIndex);
        CHECK_HR;
        
        Int32 lnValidSelectedUnits = 0;
		vector<Int32> lvSelectedUnitIndex;
		vector<Int32> lvSelectedUnitIDs;
		vector<bool> lvHasTotalElements;
		vector<bool> lvHasConcreteElements;
		vector<vector<Int32> > lvvConcreteSelElements;
		vector<Int32> lvNonSelectedUnits;
		lvNonSelectedUnits.assign(lvUnits.begin(), lvUnits.end());
        
        Int32 lnRelatedSelectedUnits = 0;
		vector<Int32> lvRelatedSelectedUnitIndex;
        
		// convert unit IDs and find out units containing subtotal selections
		Int32 u, j, k;
		for (u = 0; u < lnSelectedUnits; u++)
		{
			Int32 lUnitID;
			if (ATL::InlineIsGUIDNULL(ipSelectedUnits[u]))
				continue;
			
			hr = lpTabularData->FindUnitI(ipSelectedUnits[u], DssTemplateReserved, lUnitID);
			if (hr != S_OK)
            {
                if (lpGBTable)
                {
                    Int32 lUnitIndex = -1;
                    hr = lpGBTable->FindUnitI(&ipSelectedUnits[u], &lUnitIndex);
                    if (S_OK == hr)
                    {
                        lvRelatedSelectedUnitIndex.push_back(u);
                        lnRelatedSelectedUnits++;
                    }
                }
                
				continue;
            }
            
			// check if the unit is in the main index
			Int32 lnNonSelecteUnits = lvNonSelectedUnits.size();
			for (k = 0; k < lnNonSelecteUnits; k++)
			{
				if (lUnitID == lvNonSelectedUnits[k])
					break;
			}
			if (k < lnNonSelecteUnits)
			{
				lnValidSelectedUnits++;
				lvSelectedUnitIDs.push_back(lUnitID);
				lvSelectedUnitIndex.push_back(u);
				lvNonSelectedUnits.erase(lvNonSelectedUnits.begin() + k);
			}
		}
        
        lvHasTotalElements.resize(lnValidSelectedUnits, false);
		lvHasConcreteElements.resize(lnValidSelectedUnits, false);
		lvvConcreteSelElements.resize(lnValidSelectedUnits);
        
        bool iEmptySelection = false;
        
        for (u = 0; u < lnRelatedSelectedUnits; u++)
		{
			Int32 lnElements = ipSelectedElements[lvRelatedSelectedUnitIndex[u]].size();
			if (0 == lnElements)
			{
				iEmptySelection = true;
				break;
			}
		}
        
        if (! iEmptySelection)
        {
            for(u = 0; u < lnValidSelectedUnits; u++)
            {
                Int32 lnElements = ipSelectedElements[lvSelectedUnitIndex[u]].size();
                if(lnElements == 0)
                {
                    iEmptySelection = true;
                    break;
                }
                
                for (j = 0; j < lnElements; j++)
                {
                    if (ipSelectedElements[lvSelectedUnitIndex[u]][j] == DSSXTAB_TOTAL_KEY)
                    {
                        lvHasTotalElements[u] = true;
                    }
                    else
                    {
                        lvHasConcreteElements[u] = true;
                        lvvConcreteSelElements[u].push_back(ipSelectedElements[lvSelectedUnitIndex[u]][j]);
                    }
                }
            }
        }
        
        if(iEmptySelection || (lnValidSelectedUnits == 0 && lnRelatedSelectedUnits == 0))
		{
            Int32 lnRow = lpMainIndex->Count();
			IndexMask* lpIndexMask = new IndexMask(lpMainIndex, lnRow);
            CHECK_PTR(lpIndexMask);
            
			lpIndexMask->mMask.Init(lnRow, false);
			if (lnValidSelectedUnits == 0 && lnRow > 0)
				lpIndexMask->mMask.BulkSet(0, lnRow - 1, true);
            
            DSSTabularData *lpTabularData = NULL;
            hr = lpViewDataSet->getTabularData(&lpTabularData);
            CHECK_PTR(lpTabularData);
            
            hr = hCheckDisplayable(lpTabularData, lpMainIndex, &lpIndexMask->mMask);
            CHECK_HR;
            
			lvIndexMasks[lInsertPos].push_back(lpIndexMask);
            
            
            
			continue; // back to template loop
		}
        
        vector<vector<bool>* > lpPaths;
        
        if (lnValidSelectedUnits == 0 && lnRelatedSelectedUnits > 0)
		{
			vector<bool>* lpPath = new vector<bool>();
			lpPaths.push_back(lpPath);
		}
        
		for (u = 0; u < lnValidSelectedUnits; u++)
		{
			Int32 lnSize = lpPaths.size();
			Int32 lStartIndex = 0;
			vector<bool>* lpPath;
			if (lnSize == 0)
			{
				lpPath = new vector<bool>();
				lpPaths.push_back(lpPath);
				lnSize = 1;
			}
			if (lvHasConcreteElements[u] && lvHasTotalElements[u])
			{   // need to copy all the paths
				for(j = 0; j < lnSize; j++)
				{
					lpPath = new vector<bool>();
					lpPath->assign(lpPaths[j]->begin(), lpPaths[j]->end());
					lpPaths.push_back(lpPath);
				}
			}
			if (lvHasTotalElements[u])
			{
				for(j = 0; j < lnSize; j++)
				{
					lpPaths[lStartIndex + j]->push_back(false);
				}
				lStartIndex = lnSize;
			}
			if (lvHasConcreteElements[u])
			{
				for(j = 0; j < lnSize; j++)
				{
					lpPaths[lStartIndex + j]->push_back(true);
				}
			}
		}
        
		// lpPaths may contains duplicated paths, we'll remove the duplicate after we get the index
		Int32 lnPath = lpPaths.size();
        
		vector<Int32> lvIndexUnits;
		vector<Int32> lvTmpSelUnitIDs;
		vector<vector<Int32> > lvTmpSelElements;
        vector<GUID> lvTmpSelUnitGUIDs;
        
        DSSIDSet lsTotalIDs;
		bool hasSelection = false;
		for(j = 0; j < lnPath; j++)
		{
			lvIndexUnits.clear();
			lvTmpSelUnitIDs.clear();
			lvTmpSelElements.clear();
            lvTmpSelUnitGUIDs.clear();
			lsTotalIDs.clear();
			for (u = 0; u <lnValidSelectedUnits; u++)
			{
				if ((*lpPaths[j])[u])
				{
					hasSelection = true;
					lvIndexUnits.push_back(lvSelectedUnitIDs[u]);
					lvTmpSelUnitIDs.push_back(lvSelectedUnitIDs[u]);
					lvTmpSelElements.push_back(lvvConcreteSelElements[u]);
                    
                    if(lpGBTable)
						lvTmpSelUnitGUIDs.push_back(ipSelectedUnits[lvSelectedUnitIndex[u]]);
				}
				else
				{
					lsTotalIDs.insert(ipSelectedUnits[lvSelectedUnitIndex[u]]);
				}
			}
            
            if (lpGBTable && lnRelatedSelectedUnits > 0)
			{
				for (u = 0; u < lnRelatedSelectedUnits; u++)
				{
					lvTmpSelUnitGUIDs.push_back(ipSelectedUnits[lvRelatedSelectedUnitIndex[u]]);
					lvTmpSelElements.push_back(ipSelectedElements[lvRelatedSelectedUnitIndex[u]]);
				}
			}
            
			Int32 lnSize = lvNonSelectedUnits.size();
			if (lDummyUnitIndex == -1 || lvIndexUnits.size() == lnValidSelectedUnits)
			{
				for (k = 0; k < lnSize; k++)
					lvIndexUnits.push_back(lvNonSelectedUnits[k]);
			}
			else // do not add dummy unit to index
			{
				for (k = 0; k < lnSize; k++)
					if (lvNonSelectedUnits[k] != lDummyUnitIndex)
						lvIndexUnits.push_back(lvNonSelectedUnits[k]);
			}
            
			std::sort(lvIndexUnits.begin(), lvIndexUnits.end());
			DSSTabularIndex *lpIndex = NULL;
			hr = lpIndicesPool->getIndex(lvIndexUnits.size(), lvIndexUnits.size() > 0 ? &lvIndexUnits[0] : NULL, &lpIndex);
			CHECK_HR;
			
			vector<IndexMask*>::iterator it = lvIndexMasks[lInsertPos].begin();
			while(it != lvIndexMasks[lInsertPos].end())
			{
                if((*it)->mpIndex == lpIndex)
					break;
				
				it++;
			}
            
			if (it == lvIndexMasks[lInsertPos].end())
			{
				Int32 lnRow = lpIndex->Count();
                IndexMask* lpIndexMask = new IndexMask(lpIndex, lnRow);
                CHECK_NEWED_PTR(lpIndexMask);
                
				lpIndexMask->msTotalUnits = lsTotalIDs;
                if (!lpGBTable && lvTmpSelUnitIDs.size() > 0)
				{
					hr = lpIndex->CalculateMask(lvTmpSelUnitIDs.size(), &lvTmpSelUnitIDs[0], &lvTmpSelElements[0], lpIndexMask->mMask);
					CHECK_HR;
				}
                else if (lpGBTable && lvTmpSelUnitGUIDs.size() > 0)
				{
					hr = hCalculateMaskViaGBTable(lvTmpSelUnitGUIDs, lvTmpSelElements, lpCube, lpIndex, lpGBTable, lpIndexMask->mMask);
					CHECK_HR;
				}
				else
				{
					Int32 lnRow = lpIndex->Count();
					if (lnRow > 0)
					{
						lpIndexMask->mMask.Init(lnRow, false);
						lpIndexMask->mMask.BulkSet(0, lnRow - 1, true);
					}
				}
                
                // #547698 tehe 2/9/2012 check displayable for element
                DSSTabularData *lpTabularData = NULL;
                hr = lpViewDataSet -> getTabularData(&lpTabularData);
                CHECK_HR;
                
				hr = hCheckDisplayable(lpTabularData, lpIndex, &lpIndexMask->mMask);
				CHECK_HR;
                
				lvIndexMasks[lInsertPos].push_back(lpIndexMask);
			}
		} // end: for(j = 0; j < lnPath; j++)
        
        // 8/20/2012 tehe: memory leak
        for(int i=0;i< lpPaths.size(); i++)
        {
            if(lpPaths[i])
            {
                lpPaths[i]->clear();
                delete lpPaths[i];
                lpPaths[i] = NULL;
            }
        }
        lpPaths.clear();
        
    } // end: for (Int32 iTemplate = 0; iTemplate < lnTemplateCount; iTemplate++)
    
    //ctang: if it is in header section, we need to add UC node to the GB unit we passed to JoinUtility, so join utility will change the join type according to UC
	vector<GUID> lvGBUnit;
	lvGBUnit.resize(ipSelectedUnits.size());
	lvGBUnit.assign(ipSelectedUnits.begin(), ipSelectedUnits.end());
	vector<bool> lvHasConcreteOnly(ipSelectedUnits.size(), true);
    
	if (DssFieldGroupDataLevelDefaultReport != mDataLevel)
	{
        vector<DSSRWNode *> lvpUCNodes;
        hr = getParentUnitConditionNodes(mpNode, lvpUCNodes);
        CHECK_HR;
        
        int lnUCNodes = lvpUCNodes.size();
        for (int i = 0; i < lnUCNodes; ++i)
        {
            DSSRWSectionNode *lpThisNode = static_cast<DSSRWSectionNode*>(lvpUCNodes[i]);
            DSSGroupByUnits* lpUnits = lpThisNode->getGroupByUnits();
            if (!lpUnits || ! lpUnits->Count())
                continue;
            
            DSSGroupByUnit *lpUnit = lpUnits->Item(0);
            GUID lUCUnitID = lpUnit->getObjectID();
            if (GUID_NULL == lUCUnitID )
                continue;
            
            lvGBUnit.push_back(lUCUnitID);
            lvHasConcreteOnly.push_back(true);//UC should only have concrete
        }
	}
    
	if (mDataLevel != DssFieldGroupDataLevelDefaultReport && ipSelectedUnits.size() > 0)
	{
		for (Int32 i = 0; i < ipSelectedUnits.size(); i++)
		{
			for (Int32 j = 0; j < ipSelectedElements[i].size(); j++)
			{
				if (ipSelectedElements[i][j] == DSSXTAB_TOTAL_KEY)
				{
					lvHasConcreteOnly[i] = false;
					break;
				}
				bool lfCompoundNull = false;
				hr = mpDataModel->IsCompoundNull(ipSelectedUnits[i], ipSelectedElements[i][j], &lfCompoundNull);
				CHECK_HR;
                
				if (lfCompoundNull)
				{
					lvHasConcreteOnly[i] = false;
					break;
				}
			}
		}
	}
    
	if(!lvIndexMasks.size())
	{
		*oppJoinResult = NULL;
		return S_OK;
	}
    
    DSSGUnitContainer *lpGUnitContainer = NULL;
    hr = DSSRWNodeModel::mpDataModel->GetGlobalLookupContainer(&lpGUnitContainer);
    CHECK_PTR(lpGUnitContainer);
    
#ifdef NDEBUG
    DSSMultipleLevelJoinHelper lJoinHelper(lpGUnitContainer);
#else
    DSSMultipleLevelJoinHelper lJoinHelper(lpGUnitContainer, mpDataModel->getObjectContext());
#endif
    hr = lJoinHelper.JoinWithMask(lvViewDatasets, lvIndexMasks, (bool*)(&lvPrimary[0]),
                                  oppJoinResult, mDataLevel == DssFieldGroupDataLevelDefaultReport,
                                  lvGBUnit, lvHasConcreteOnly);
	CHECK_HR;
    
	if (iRetrieveSortUnits)
	{
		vector<GUID> lvSortUnitsID = mvGroupByUnits; //TQMS 640421, should sort gb units first
		Int32 lnDataSets = lvCubes.size();
		for (Int32 iDataSet = 0; iDataSet < lnDataSets; ++iDataSet)
		{
            DSSTabularIndexPool *lpIndicesPool = NULL;
            hr = lvCubes[iDataSet]->getIndexPool(&lpIndicesPool);
            
			DSSTabularIndex* lpMainIndex;
            hr = lpIndicesPool->getMainIndex(&lpMainIndex);
			CHECK_PTR(lpMainIndex);
            
			Int32 lnUnit = lpMainIndex->getNumUnits();
			if (lnUnit > 0)
			{
                vector<Int32> lvUnits;
                hr = lpMainIndex->getUnits(lvUnits);
				CHECK_HR;
                
				bool lfMatch = false;
				for (Int32 j = 0; j < lnUnit; ++j)
				{
					lfMatch = false;
                    
                    DSSTabularUnit *lpTabularUnit = NULL;
                    hr = lvViewDatasets[iDataSet]->getTabularData()->Item(lvUnits[j], &lpTabularUnit);
                    CHECK_PTR(lpTabularUnit);
                    
                    GUID lUnitGUID = GUID_NULL;
					hr = lpTabularUnit->getGUID(lUnitGUID);
					CHECK_HR;
                    
					for (Int32 k = 0; k < lvSortUnitsID.size(); k++)
					{
						if (lUnitGUID == lvSortUnitsID[k])
						{
							lfMatch = true;
							break;
						}
					}
					if (!lfMatch)
						lvSortUnitsID.push_back(lUnitGUID);
				}// end match unit
			}
		} // end for each dataset
        
		for (Int32 iUnit = 0; iUnit < lvSortUnitsID.size(); ++iUnit)
		{
			Int32 lIndex = -1;
			hr = mpDataModel->GetUnitIndexByGUID(&lvSortUnitsID[iUnit], &lIndex);
			if (S_OK == hr)
			{
				orvSortUnits.push_back(lIndex);
			}
		}
	}//end: if (iRetrieveSortUnits)
    
    // tehe 4/17/2012 : memory leak
    for(int i=0;i<lvIndexMasks.size(); i++)
    {
        for(int j=0;j<lvIndexMasks[i].size();j++)
        {
            if(lvIndexMasks[i][j])
            {
                delete lvIndexMasks[i][j];
                lvIndexMasks[i][j] = NULL;
            }
        }
        lvIndexMasks[i].clear();
    }
    lvIndexMasks.clear();
    
    
    return S_OK;
}

int DSSRWFieldGroupModel::InitXTabView()
{
    int hr = S_OK;
    
    int lnRTT = mvpRTTemplateInfo.size();
    mvpXTabView.resize(lnRTT);
    for (int iRTT = 0; iRTT < lnRTT; ++iRTT)
    {
        if (NULL == mvpRTTemplateInfo[iRTT]->mViewPtr)
        {
            mvpXTabView[iRTT] = NULL;
            continue;
        }
        
        // jimzhang Mar/19/2012 Currently on iPad, only DSSRTTemplateInfo::mViewPtr is used for creating XTabView of Field Group,
        //      however, in server's logic, both DSSRTTemplateInfo::mpRuntimeTemplate and DSSRTTemplateInfo::mpExtRuntimeTemplate
        //      may be used when creating XTabView of Field Group. Current iPad logic may cause issues, and need to be modified to
        //      to follow server's logic in the futher.
        hr = DSSXTabViewHelper::InitXTabViewForFieldGroup(mvpRTTemplateInfo[iRTT]->mViewPtr, &(mvpXTabView[iRTT]));
        CHECK_HR;
    }
    
    return hr;
}

//xiazhou, for offline UC, 04/23/2014
int DSSRWFieldGroupModel::clearXTabView()
{
    int hr = S_OK;
    
    int lnRTT = mvpRTTemplateInfo.size();
    
    for (int iRTT = 0; iRTT < lnRTT; ++iRTT)
    {
        if (mvpRTTemplateInfo[iRTT]->mViewPtr)
        {
            mvpRTTemplateInfo[iRTT]->mViewPtr->clearXTabView();
            mvpXTabView[iRTT] = NULL;
        }
    }
    
    return hr;

}

void DSSRWFieldGroupModel::ReleaseUnusedMemory()
{
    if (!mpTransactionModel)
    {
        hCleanUp();		//clean up itself
        DSSRWNodeModel::ReleaseUnusedMemory();
    }
    return;
}

void DSSRWFieldGroupModel::hCleanUp(bool ibCalledByDestuctor)
{
    // tehe 4/18/2012: memory leak
    if(ibCalledByDestuctor)
    {
        for(int i=0;i<mvpRTTemplateInfo.size();i++)
        {
            if(mvpRTTemplateInfo[i])
            {
                delete mvpRTTemplateInfo[i];
                mvpRTTemplateInfo[i] = NULL;
            }
        }
        mvpRTTemplateInfo.clear();
        if (mpBuffer)
        {
            delete mpBuffer;
            mpBuffer = NULL;
        }
    }
    // 8/24/2012 tehe: partial release
    //tliao, 915114, we still need it for onDemandLoading
    else if (mbOnDemandLoading)
    {
        for(int i=0;i<mvpRTTemplateInfo.size();i++)
        {
            if(mvpRTTemplateInfo[i])
            {
                mvpRTTemplateInfo[i]->hPartialCleanUp();
            }
        }
    }
}

int DSSRWFieldGroupModel::EnsureModelData(bool *obUpdated)
{
    if(!mbOnDemandLoading)
        return S_OK;
    
    int hr = S_OK;
    switch (mStatus) {
        case XTAB_UNDATALOADED:
        {
            SmartLock lSmartLock(BinaryDataLock::getInstance(), this);
            if (mStatus != XTAB_UNDATALOADED)
            {
                // after the lock unlocked, if the status has been changed, just return it
                break;
            }
            
            hr = hLoadDataFromCache();
            if(hr != S_OK)
            {
                mStatus = XTAB_DATALOADFAIL;
                return hr;
            }
            
            if(obUpdated)
            {
                *obUpdated = true;
            }
            
            mStatus = XTAB_INITIALIZED;
        }
            break;
            
        case XTAB_INITIALIZED:
        {
            // do nothing
        }
            break;
            
        case XTAB_UNINITIALIZED: // tehe: coresponding to partial clean up
        {
            SmartLock lSmartLock(BinaryDataLock::getInstance(), this);
            if (mStatus != XTAB_UNINITIALIZED)
            {
                // after the lock unlocked, if the status has been changed, just return it
                break;
            }
            
            hr = hLoadDataFromCache();
            if(hr != S_OK)
            {
                mStatus = XTAB_DATALOADFAIL;
                return hr;
            }
            
            if(obUpdated)
            {
                *obUpdated = false;
            }
            mStatus = XTAB_INITIALIZED;
        }
            break;
        case XTAB_DATALOADFAIL:
            return E_FAIL;
            break;
            
        default:
            break;
    }
    
    return S_OK;
}

int DSSRWFieldGroupModel::hLoadDataFromCache()
{
    int hr = S_OK;
    CHECK_PTR(mpDataSource);
    
    hr = mpDataSource->LoadNewRWDModel(this);
    CHECK_HR;
    
    // 5/23/2012 tehe: we should initialize units value after model loaded, this should always keep same with NodeMode::Init method
    DSSObjectContext *lpContext = mpDataModel->getObjectContext();
    mpNode = lpContext->getNode(mNodeKey);
    hr = hInitTabularUnitInViewDataSet();
    CHECK_HR;
    
    return S_OK;
}

int DSSRWFieldGroupModel::setBinaryTransactionModel (unsigned char * ipBinaryModel, int iSize)
{
    int hr = S_OK;
	hr = this->EnsureModelData();
	CHECK_HR;
    
    hr = EnsureTransactionData();
    CHECK_HR;
    
    return DSSDataModelFieldGroupImpl::LoadBinaryTransactionModel(ipBinaryModel, iSize, mpTransactionModel,NULL);
}

int DSSRWFieldGroupModel::getBinaryTransactionModel (bool includeCommitted, unsigned char** oppBinaryModel, int* opSize)
{
	int hr = S_OK;
	if(mpTransactionModel)
	{
		hr = mpTransactionModel->SaveToBinary(includeCommitted, oppBinaryModel, opSize);
		CHECK_HR;
	}
	return S_OK;
}

int DSSRWFieldGroupModel::EnsureTransactionData()
{
    if(!mpDataModel->getObjectContext())
        return E_FAIL;
    
    DSSRWNode* lpNode = mpDataModel->getObjectContext()->getNode(mNodeKey);
    CHECK_PTR(lpNode);
    
    AE_ASSERT(lpNode->getType() == DssRWNodeFieldGroup);
    
    DSSRWFieldGroupNode * lpThisNode = static_cast<DSSRWFieldGroupNode*> (lpNode);
    DSSTransactionReport *lpTargetReport = lpThisNode->getTransactionReport();
    DSSOfflineTransactionCSIInfo *lpCSIInfo = lpThisNode->getOfflineTransactionCSIInfo();
    DFCCSITransactionBase* lpCSI = lpCSIInfo?lpCSIInfo->getTransactionCSI():NULL;

    if(!mpTransactionModel)
    {
		if (lpThisNode->IsTransactionEnabled() || lpCSI)
		{
			mpTransactionModel = new DSSDataModelTransactionRWField();
            int hr = mpTransactionModel->InitWithNodeModel(lpTargetReport, this);
            if (hr != S_OK)
            {
                _ASSERT(false);
                delete mpTransactionModel;
                mpTransactionModel = NULL;
            }
		}
    }
    
    //return OK if not a transaction enabled xtab model
	if (!mpTransactionModel)
		return S_OK;
    
    return DSSDataModelFieldGroupImpl::EnsureTransactionData(mpTransactionModel, lpThisNode, lpCSI);
}
int DSSRWFieldGroupModel::hInitialLoading(DSSBinaryDataReader *ipReadStream)
{
    int hr = S_OK;
    
    hr = ipReadStream->ReadInt(&mRowCount);
    CHECK_HR;
    
    hr = ipReadStream->ReadInt(&mFieldCount);
    CHECK_HR;
    
    if(mRowCount > 0 && mFieldCount > 0)
    {
        hr = DSSViewDataSetHelper::loadStringVector(ipReadStream, mvFieldText);
        CHECK_HR;
        
        hr = ipReadStream->ReadIntArray(mvThresholdVectorID);
        CHECK_HR;
        
        hr = ipReadStream->ReadIntArray(mvLinkTextVectorID);
        CHECK_HR;
        
        hr = ipReadStream->ReadIntArray(mvThreshold);
        CHECK_HR;
        
        //zhyang, 640759, server side threshold is 1 based but client is 0 based.
        for(int i = 0; i < mvThreshold.size(); ++i)
        {
            mvThreshold[i] = mvThreshold[i] - 1;
        }
        
        int lnLinkText;
        hr = ipReadStream->ReadInt(&lnLinkText);
        CHECK_HR;
        
        for(int i = 0; i < lnLinkText; ++i)
        {
            vector<int> lvLinkType;
            vector<string> lvLinkText;
            
            hr = ipReadStream->ReadIntArray(lvLinkType);
            CHECK_HR;
            
            hr = DSSViewDataSetHelper::loadStringVector(ipReadStream, lvLinkText);
            CHECK_HR;
            
            mvvLinkTextType.push_back(lvLinkType);
            mvvLinkText.push_back(lvLinkText);
        }
    }
    
    return hr;
}

int DSSRWFieldGroupModel::hFullBinaryLoading(DSSBinaryDataReader *ipReadStream)
{
    int hr = S_OK;
    
    int lnPrimaryFlagSize;
    hr = ipReadStream->ReadInt(&lnPrimaryFlagSize);
    CHECK_HR;
    
    if(lnPrimaryFlagSize != 0)
    {
        mvPrimaryFlag.clear();
        hr = ipReadStream->ReadIntArray(mvPrimaryFlag);
        CHECK_HR;
    }
    
    int lDataLevel = -1;
    hr = ipReadStream->ReadInt(&lDataLevel);
    CHECK_HR;
    
    mDataLevel = (EnumDSSFieldGroupDataLevel)lDataLevel;
    
    int lnRTTTemplateInfo = -1;
    hr = ipReadStream->ReadInt(&lnRTTTemplateInfo);
    CHECK_HR;
    
    // 8/24/2012 tehe
    if(mvpRTTemplateInfo.empty())
    {
        mvpRTTemplateInfo.resize(lnRTTTemplateInfo);
        
        for(int i =0; i<lnRTTTemplateInfo; i++)
        {
            DSSRTTemplateInfo *lpRTTTemplateInfo = new DSSRTTemplateInfo();
            CHECK_PTR(lpRTTTemplateInfo);
            
            mvpRTTemplateInfo[i] = lpRTTTemplateInfo;
        }
    }
    
    DSSGUnitContainer* lpGUnitContainer;
    hr = mpDataModel->GetGlobalLookupContainer(&lpGUnitContainer);
    CHECK_HR;
    
    for(int i = 0; i < lnRTTTemplateInfo; ++i)
    {
        //        DSSRTTemplateInfo *lpRTTTemplateInfo = new DSSRTTemplateInfo();
        //        CHECK_PTR(lpRTTTemplateInfo);
        
        hr = mvpRTTemplateInfo[i]->LoadFromBinary(ipReadStream, lpGUnitContainer, mpNodeModelCacheManager);
        CHECK_HR;
        
        DSSViewDataSet *lpViewDataSet = mvpRTTemplateInfo[i]->mViewPtr;
        if (lpViewDataSet)
        {
            hr = lpViewDataSet->Init(lpGUnitContainer);
            CHECK_HR
            /*DSSTabularData *lpTabularData = NULL;
             hr = lpViewDataSet->getTabularData(&lpTabularData);
             if (lpTabularData)
             {
             lpTabularData->init(lpViewDataSet);
             }*/
        }
        lpViewDataSet = mvpRTTemplateInfo[i]->mOLViewPtr;
        if (lpViewDataSet)
        {
            hr = lpViewDataSet->Init(lpGUnitContainer);
            CHECK_HR;
            /*
             DSSTabularData *lpTabularData = NULL;
             hr = lpViewDataSet->getTabularData(&lpTabularData);
             if (lpTabularData)
             {
             lpTabularData->init(lpViewDataSet);
             }*/
        }
        
        //zhyang, init extend cube's tabular data
        lpViewDataSet = mvpRTTemplateInfo[i]->mExtendViewPtr;
        if (lpViewDataSet)
        {
            hr = lpViewDataSet->Init(lpGUnitContainer);
            CHECK_HR;
            /*
             DSSTabularData *lpTabularData = NULL;
             hr = lpViewDataSet->getTabularData(&lpTabularData);
             if (lpTabularData)
             {
             lpTabularData->init(lpViewDataSet);
             }*/
        }
        
        lpViewDataSet = mvpRTTemplateInfo[i]->mOLExtendViewPtr;
        if (lpViewDataSet)
        {
            hr = lpViewDataSet->Init(lpGUnitContainer);
            CHECK_HR;
            /*
             DSSTabularData *lpTabularData = NULL;
             hr = lpViewDataSet->getTabularData(&lpTabularData);
             if (lpTabularData)
             {
             lpTabularData->init(lpViewDataSet);
             }*/
        }
    }
    
    return hr;
}

int DSSRWFieldGroupModel::hCalculateMaskViaGBTable(vector<GUID>& irSelectedUnits, vector<vector<Int32> >& irSelectedElements,
                                                   DSSCube* ipCube, DSSTabularIndex *ipIndex, DSSGBTable* ipGBTable, DSSXTabBitVector& orBitVector)
{
    int hr = S_OK;
	Int32 lnRow = ipIndex->Count();
    CHECK_HR;
    
	orBitVector.Init(lnRow, false);
	if (lnRow == 0)
		return S_OK;
	
	if (irSelectedUnits.size() == 0)
	{
		if (lnRow > 0)
			orBitVector.BulkSet(0, lnRow-1, true);
		return S_OK;
	}
    
    DSSTabularIndexPool* lpIndicesPool = NULL;
    hr = ipCube->getIndexPool(&lpIndicesPool);
    CHECK_HR;
    
    DSSTabularIndex *lpMainIndex = NULL;
    hr = lpIndicesPool->getMainIndex(&lpMainIndex);
    CHECK_PTR(lpMainIndex);
    
    DSSXTabBitVector *lpMainBV = new DSSXTabBitVector();
    hr = ipGBTable->GetSelectedTargetRow(irSelectedUnits, irSelectedElements, *lpMainBV);
    CHECK_HR;
    
    DSSMaskInfos *lpMaskInfo = new DSSMaskInfos();
	hr = lpMaskInfo->Init(lpMainIndex, ipCube, lpMainBV);
	CHECK_HR;
    
    //fali, 905304, avoid memory leak
    if (lpMainBV)
    {
        delete lpMainBV;
        lpMainBV = NULL;
    }
    DSSXTabBitVector *lpResult = NULL;
	hr = lpMaskInfo->GetCalculatedMask2(ipIndex, &lpResult);
	CHECK_HR;
    
    orBitVector.Populate(lpResult);
    
    //fali, 905304, avoid memory leak
    if (lpMaskInfo)
    {
        delete lpMaskInfo;
        lpMaskInfo = NULL;
    }
	return S_OK;
}

bool DSSRWFieldGroupModel::IsEvaluatedOnClient()
{
    return mIncludeCSI;
}

int DSSRWFieldGroupModel::getPrimaryFlag(bool *ibPrimary)
{
    if(!ibPrimary)
        return E_INVALIDARG;
    *ibPrimary = false;
    int len = mvPrimaryFlag.size();
    if(len == 0)
        return S_FALSE;
    for(int i = 0; i < len; ++i)
        if(mvPrimaryFlag[i])
            *ibPrimary = true;
    return S_OK;
}

bool DSSRWFieldGroupModel::IsTransactionEnabled()
{
    if(!mpDataModel->getObjectContext())
        return false;
    
    DSSRWNode* lpNode = mpDataModel->getObjectContext()->getNode(mNodeKey);
    if (!lpNode)
        return false;
    
    DSSRWFieldGroupNode * lpThisNode = static_cast<DSSRWFieldGroupNode*> (lpNode);
    if (!lpThisNode)
        return false;
    
    return lpThisNode->IsTransactionEnabled();
}

bool DSSRWFieldGroupModel::hasMultiPassCSI()
{
    if(mvpCSI.size() > 0)
    {
        for(int i = 0; i < mvpCSI.size(); i++)
        {
            DFCCSI* lpCSI = mvpCSI[i];
            if(!lpCSI)
            {
                continue;
            }
            int lIMDBIndex = lpCSI->GetIMDBSaveAsIndex();
            if(lIMDBIndex != -1)
            {
                return true;
            }else{
                if(lpCSI->GetTableJoinTree() && lpCSI->GetTableJoinTree()->hasTempTable())
                {
                    return true;
                }
            }
        }
    }
    return false;
}