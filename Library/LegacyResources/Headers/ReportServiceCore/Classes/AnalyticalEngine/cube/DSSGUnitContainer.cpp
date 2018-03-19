//
//  DSSGUnitContainer.cpp
//  ReportServiceCore
//
//  Created by Xiaoqi Zeng on 9/29/11.
//  Copyright 2011 MicroStrategy. All rights reserved.
//
#include "DSSGUnitContainer.h"
#include "DSSGlobalLookUpTable.h"
#include "DSSBinaryDataReader.h"

#include "DSSModelPopulation.h"
#include "DSSTabularRefCustomGroup.h"
#include "DSSTabularRefConsolidation.h"
#include "DSSXTabRefLookupTable.h"
#include "DSSDataRefColumn.h"
#include "DSSRWUtil.h"
#include "DSSRWDataModel.h"
#include "DSSBaseElementProxy.h"
#include "DSSViewDataSetHelper.h"


int hLoadUnitConsolidation(DSSBinaryDataReader *mpReadStream, int index, DSSTabularUnit **oppXTabUnit, DSSGlobalLookUpTable* ipLookUp = NULL, bool ibLoadAttr = true);
int hLoadUnitCustomGroup(DSSBinaryDataReader *mpReadStream, int index, DSSTabularUnit **oppXTabUnit);

//DSSRWUtil
int CompareCSElements4Display(DSSTabularConsolidation *ipConsolidation1, int iEleIndex1, void *ipTabularData1,
                              DSSTabularConsolidation *ipConsolidation2, int iEleIndex2, void *ipTabularData2,
                              RWUtilContext iContext,
                              int & oResult);
//DSSRWUtil
int CompareCGElements(DSSTabularCustomGroup *ipCustomGroup1, int iEleIndex1,
                      DSSTabularCustomGroup *ipCustomGroup2, int iEleIndex2,
                      RWUtilContext iContext,
                      int& oResult);

//DSSRWUtil
int CompareDEWithAttr4Display(DSSTabularConsolidation *ipConsolidation, int iEleIndex1, 
                              DSSTabularAttribute *ipLookUpTable, int iEleIndex2,
                              RWUtilContext iContext,
                              bool fIsReversed,
                              int & oResult);

DSSGUnitMap::DSSGUnitMap()
{
    
}

DSSGUnitMap::~DSSGUnitMap()
{
    for(int i = 0; i < mvUnits.size(); i++)
    {
        if(mvUnits[i])
        {
            delete mvUnits[i];
            mvUnits[i] = NULL;
        }
        
        if(mvLocalToGlobalMap[i])
        {
            delete mvLocalToGlobalMap[i];
            mvLocalToGlobalMap[i] = NULL;
        }
        
        if(mvGlobalToLocalMap[i])
        {
            delete mvGlobalToLocalMap[i];
            mvGlobalToLocalMap[i] = NULL;
        }
    }
    mvUnits.clear();
    mvGlobalToLocalMap.clear();
    mvLocalToGlobalMap.clear();
    
    for(int i = 0; i < mvArchiveUnits.size(); i++)
    {
        if(mvArchiveUnits[i])
        {
            delete mvArchiveUnits[i];
            mvArchiveUnits[i] = NULL;
        }
    }
    mvArchiveUnits.clear();
}

int DSSGUnitMap::setGUID(MBase::CString iStringGUID)
{
	int len = iStringGUID.length();
	if (len > 0)
	{
#ifdef NO_WCHAR_SUPPORT
        MBase::String2Guid(mID, iStringGUID.c_str());
#else
		vector<wchar_t> lString(len);
		mbstowcs(&lString[0], iStringGUID.c_str(), len);
		MBase::String2Guid(mID, &lString[0]);
#endif
	}
	return S_OK;
}

vector<int>* DSSGUnitMap::GetGlobalToLocalMap(DSSTabularUnit* ipUnit)
{
	int* lGToLMap = NULL;
    for (Int32 i = 0; i < mvUnits.size(); i++)
    {
        //fali, 11/25/2013, 815090, for two datasets which contains the same consolidation, GUIDs are the same. Get the map by comparing address firstly.
        if(mvUnits[i] == ipUnit)
        {
            lGToLMap = mvGlobalToLocalMap[i];
        }
    }
    if (lGToLMap == NULL)
    {
        for (Int32 i = 0; i < mvUnits.size(); i++)
        {
            //tliao, 10/14/2013, after UC manipulation, the consolidation unit in GUnitContainer has been changed,
            //but the consolidation unit in base dataset tabular data is not changed
            //if (mvUnits[i] == ipUnit)
            if (mvUnits[i]->getGUID() == ipUnit->getGUID())
            {
                lGToLMap = mvGlobalToLocalMap[i];
            }
        }
    }
    if (lGToLMap != NULL) {
        vector<int>* lvGlobalToLocalMap = new vector<int>();
        lvGlobalToLocalMap->assign(lGToLMap, lGToLMap+mGlobalRowCount);
        return lvGlobalToLocalMap;
    }
    return NULL;
}
vector<int>* DSSGUnitMap::GetLocalToGlobalMap(DSSTabularUnit* ipUnit)
{
	int* lLToGMap = NULL;
    int lUnitRowCount;
    for (Int32 i = 0; i < mvUnits.size(); i++)
    {
        //fali, 11/25/2013, 815090, for two datasets which contains the same consolidation, GUIDs are the same. Get the map by comparing address firstly.
        if(mvUnits[i] == ipUnit)
        {
            lLToGMap = mvLocalToGlobalMap[i];
            lUnitRowCount = mvUnitRowCount[i];
        }
    }
    if (lLToGMap == NULL)
    {
        for (Int32 i = 0; i < mvUnits.size(); i++)
        {
            //tliao, 10/14/2013, after UC manipulation, the consolidation unit in GUnitContainer has been changed,
            //but the consolidation unit in base dataset tabular data is not changed
            //if (mvUnits[i] == ipUnit)
            if (mvUnits[i]->getGUID() == ipUnit->getGUID())
            {
                lLToGMap = mvLocalToGlobalMap[i];
                lUnitRowCount = mvUnitRowCount[i];
            }
        }
    }
    if (lLToGMap != NULL) {
        vector<int>* lvLocalToGlobalMap = new vector<int>();
        lvLocalToGlobalMap->assign(lLToGMap, lLToGMap+lUnitRowCount);
        return lvLocalToGlobalMap;
    }
    return NULL;
}

int DSSGUnitMap::LoadHeaderFromBinary(DSSBinaryDataReader *ipReadStream, bool ifFirstTime)
{
    int lBlockType;
    int hr = ipReadStream->BeginReadBlock(&lBlockType);
    CHECK_HR;
    
    MBase::CString lStringGUID;
    hr = ipReadStream->ReadString(lStringGUID);
    CHECK_HR;
   
    setGUID(lStringGUID);
	CHECK_HR;
    
    int lType;
	hr = ipReadStream->ReadInt(&lType);
	CHECK_HR;
    
    mUnitType = (EnumDSSTemplateUnitType)lType;
    
    int lUnitSize;
	hr = ipReadStream->ReadInt(&lUnitSize);
	CHECK_HR;
    
    if(ifFirstTime)
    {
        mvUnits.resize(lUnitSize);
        mvUnitRowCount.resize(lUnitSize);
        mvLocalToGlobalMap.resize(lUnitSize);
        mvGlobalToLocalMap.resize(lUnitSize);
        
        for(int i = 0; i < lUnitSize; i++)
        {
            mvUnits[i] = NULL;
            mvUnitRowCount[i] = 0;
            mvLocalToGlobalMap[i] = NULL;
            mvGlobalToLocalMap[i] = NULL;
        }
        
    }
    
    hr = ipReadStream->EndReadBlock();
    
	return hr;
}

int DSSGUnitMap::LoadDataFromBinary(DSSBinaryDataReader *ipReadStream, DSSObjectContext *ipObjContext)
{
    int lBlockType;
    int hr = ipReadStream->BeginReadBlock(&lBlockType);
    CHECK_HR;
    
    // read NotNull UnitSize
	int lNotNullUnitSize = 0;
    vector<int> lvNotNullUnits;
    
    hr = ipReadStream->ReadInt(&lNotNullUnitSize);

    lvNotNullUnits.resize(lNotNullUnitSize);
    
	// read indices of notnull mvUnits
	for (int i = 0; i < lNotNullUnitSize; i++)
	{
		hr = ipReadStream->ReadInt(&lvNotNullUnits[i]);
		CHECK_HR;
	}
    
	// read GlobalRowCount
	hr = ipReadStream->ReadInt(&mGlobalRowCount);
    
	//read UnitRowCount
	for (int i = 0; i < lNotNullUnitSize; i++)
	{
		hr = ipReadStream->ReadInt(&mvUnitRowCount[lvNotNullUnits[i]]);
		CHECK_HR;
        
        //initialize mvLocalToGlobalMap and mvGlobalToLocalMap
        if (!mvLocalToGlobalMap[lvNotNullUnits[i]])
        {
            mvLocalToGlobalMap[lvNotNullUnits[i]] = new int[mvUnitRowCount[lvNotNullUnits[i]]];
        }
        
        if (!mvGlobalToLocalMap[lvNotNullUnits[i]])
        {
            mvGlobalToLocalMap[lvNotNullUnits[i]] = new int[mGlobalRowCount];
        }

	}
    
    
    
	//read LocalToGlobalMap
	for (int i = 0; i < lNotNullUnitSize; i++)
	{
		for(int lUnitElemId = 0; lUnitElemId < mvUnitRowCount[lvNotNullUnits[i]]; lUnitElemId ++)
		{
			hr = ipReadStream->ReadInt(&mvLocalToGlobalMap[lvNotNullUnits[i]][lUnitElemId]);
			CHECK_HR;
		}
	}
    
	//read GlobalToLocalMap
	for (int i = 0; i < lNotNullUnitSize; i++)
	{
		for(int lGlobalId = 0; lGlobalId < mGlobalRowCount; lGlobalId ++)
		{
			hr = ipReadStream->ReadInt(&mvGlobalToLocalMap[lvNotNullUnits[i]][lGlobalId]);
			CHECK_HR;
		}
	}
    
    //read consolidation unit
    for (int i = 0; i < lNotNullUnitSize; i++)
    {
        EnumBinaryObjectType objtype = EnumObjectReserved;
		hr = ipReadStream->ReadInt((int*)(&objtype));
		CHECK_HR;
        
		int lUnitID = 0;
		hr = ipReadStream->ReadInt(&lUnitID);
		CHECK_HR;
        
        //tliao, 959484,959583, 2014/08/26, postpone archive time to avoid crash
        //xiazhou, fix typo for issue 817189
        if(mvUnits[lvNotNullUnits[i]])
        {
            mvArchiveUnits.push_back(mvUnits[lvNotNullUnits[i]]);
            mvUnits[lvNotNullUnits[i]] = NULL;
        }
        //mvUnits[lvNotNullUnits[i]] = NULL;
		switch (objtype) {               
			case EnumObjectConsolidationUnit:
			{
				hr = hLoadUnitConsolidation(ipReadStream, lUnitID, &mvUnits[lvNotNullUnits[i]]);
				CHECK_HR;

//				reinterpret_cast<DSSTabularConsolidation*>(mvUnits[i])->setParentInfo(ipModel);
			}
                break;
                
			case EnumObjectCustomGroupUnit:
			{
				hr = hLoadUnitCustomGroup(ipReadStream, lUnitID, &mvUnits[lvNotNullUnits[i]]);
				CHECK_HR;
//				reinterpret_cast<DSSTabularCustomGroup*>(mvUnits[i])->setParentInfo(ipModel);
			}
				break;
			default:
				return E_UNEXPECTED_DATATYPE;
		}
        
		CHECK_PTR(mvUnits[lvNotNullUnits[i]]);
        mvUnits[lvNotNullUnits[i]]->setObjectContextRef(ipObjContext);
        
//		hr = ipModel->AddUnit(lUnitID, lpUnit);
//		CHECK_HR;       
        
        
    }
    
    hr = ipReadStream->EndReadBlock();
    
    return hr;
}

int DSSGUnitMap::findElement(DSSBaseElementProxy *ipElement, int &orKey)
{
    EnumDSSTemplateUnitType lUnitType = ipElement->getUnitType();
    if(lUnitType != mUnitType)
        return S_FALSE;
    
    GUID lID = ipElement->getObjectID();
    if(mID != lID)
        return S_FALSE;
    
    int hr = S_OK;
    for(int i = 0; i < mvUnits.size(); ++i)
    {
        hr = mvUnits[i]->FindElement(ipElement, orKey);
        if(hr == S_OK)
        {
            orKey = mvLocalToGlobalMap[i][orKey];
            return hr;
        }
    }
    
    return S_FALSE;
}

//DSSGUnitMap End




DSSGUnitContainer::DSSGUnitContainer():mfLoadStatus(false), mfReLoad(false)
{
    mAttributeNum = 0;
    mvpGlobalLookUps.resize(0);
}

DSSGUnitContainer::~DSSGUnitContainer()
{
    for (int i = 0; i < mAttributeNum; i ++)
    {
        if (mvpGlobalLookUps[i])
        {
            delete mvpGlobalLookUps[i];
        }
    }
    
    mvpGlobalLookUps.clear();
    mvLookUpIDs.clear();
    
    for (int i = 0; i < mvpGUnitMapList.size(); i++)
    {
        if (mvpGUnitMapList[i])
        {
            delete mvpGUnitMapList[i];
        }
    }
    mvpGUnitMapList.clear();
    
    for(int i = 0; i < mvDerivedUnits.size(); ++i)
    {
        if(mvDerivedUnits[i])
        {
            delete mvDerivedUnits[i];
        }
    }
    
    mvDerivedUnits.clear();
}

int DSSGUnitContainer::Init()
{
    mvpGlobalLookUps.resize(mAttributeNum);
    mvLookUpIDs.resize(mAttributeNum);
    
    for (int i = 0; i < mAttributeNum; i ++)
    {
        mvpGlobalLookUps[i] = new DSSGlobalLookUpTable();
        CHECK_NEWED_PTR(mvpGlobalLookUps[i]);
    }
    
    return S_OK;
}

int DSSGUnitContainer::clear()
{
    
    /*for (int i = 0; i < mAttributeNum; i ++)
    {
        if (mvpGlobalLookUps[i])
            mvpGlobalLookUps[i]->cleanup();
    }
    
    for (int i = 0; i < mvpGUnitMapList.size(); i++)
    {
        if (mvpGUnitMapList[i])
        {
            delete mvpGUnitMapList[i];
        }
    }
    mvpGUnitMapList.clear();*/
    
    mfReLoad = true;
    
    return S_OK;
}

int DSSGUnitContainer::getAttributeNum()
{
    return mAttributeNum;
}

int DSSGUnitContainer::LoadHeaderFromBinary(DSSBinaryDataReader *ipReadStream)
{
    int lBlockType = -1;
    int hr = ipReadStream->BeginReadBlock(&lBlockType);
    //CHECK_HR_BLOCK(type)
    CHECK_HR;
    
    hr = ipReadStream->ReadInt(&mAttributeNum);
    CHECK_HR;
    
    if (!mfLoadStatus)
    {
        Init();
    }
    
    for (int i = 0; i < mAttributeNum; i ++)
    {
        int hr = mvpGlobalLookUps[i]->LoadHeaderFromBinary(ipReadStream, !mfLoadStatus | mfReLoad);
        mvpGlobalLookUps[i]->getGUID(mvLookUpIDs[i]);
        CHECK_HR;
    }
    
    int lnGUnitMap;
	hr = ipReadStream->ReadInt(&lnGUnitMap);
    CHECK_HR;
    
    //GUnitMap
    mvpGUnitMapList.resize(lnGUnitMap);    
    
    for (Int32 i = 0; i < lnGUnitMap; i++)
	{
        if (!mvpGUnitMapList[i])
            mvpGUnitMapList[i] = new DSSGUnitMap;
        hr = mvpGUnitMapList[i]->LoadHeaderFromBinary(ipReadStream, !mfLoadStatus | mfReLoad);
		CHECK_HR;
	}
    
    if(ipReadStream->MoreDataInBlock())
    {
    
        int lnDANewDE;
        hr = ipReadStream->ReadInt(&lnDANewDE);
        CHECK_HR;
    
        for(int i = 0; i < lnDANewDE; ++i)
        {
            int lUnitType;
            hr = ipReadStream->ReadInt(&lUnitType);
            CHECK_HR;
        
            if(lUnitType == DssTemplateAttribute)
            {
                DSSGlobalLookUpTable *lpAttr = new DSSGlobalLookUpTable();
                hr = lpAttr->LoadHeaderFromBinary(ipReadStream, true);//!mfLoadStatus | mfReLoad);//986825
                CHECK_HR;
            
                mvDerivedUnits.push_back(lpAttr);
            }
            else
            {
                mvDerivedUnits.push_back(NULL);
            }
        }
    }
    //xiazhou, 08/12/2014. Linked attribute info
    if(ipReadStream->MoreDataInBlock())
    {
        hr = ipReadStream->BeginReadBlock(&lBlockType);
        CHECK_HR;
        mvvAttrLinkedInfo.clear();
        mMapToLinkedGroupID.clear();
        
        int lnLinkedGroup, lnGroupSize;
        hr = ipReadStream->ReadInt(&lnLinkedGroup);
        CHECK_HR;
        
        for(int i = 0; i < lnLinkedGroup; i++)
        {
            hr = ipReadStream->ReadInt(&lnGroupSize);
            CHECK_HR;
            
            vector<GUID> lvGroup;
            
            for(int j = 0; j < lnGroupSize; j++)
            {
                MBase::CString lDSSID;
                hr = ipReadStream->ReadString(lDSSID);
                CHECK_HR;
                
                GUID lGUID;
                hr = DSSViewDataSetHelper::str2GUID(lDSSID, lGUID);
                CHECK_HR;
                
                lvGroup.push_back(lGUID);
                mMapToLinkedGroupID.insert(make_pair(lGUID, i));
            }
            mvvAttrLinkedInfo.push_back(lvGroup);
        }
        
        hr= ipReadStream->EndReadBlock();
        CHECK_HR;
    }
    
    hr = ipReadStream->EndReadBlock();
    CHECK_HR;
    
    return hr;        
}

int DSSGUnitContainer::LoadDataFromBinary(DSSBinaryDataReader *ipReadStream, DSSObjectContext *ipObjContext)
{
    int lBlockType;
    int hr = ipReadStream->BeginReadBlock(&lBlockType);
    CHECK_HR;
    
    int lAttributeNum;
    hr = ipReadStream->ReadInt(&lAttributeNum);
    CHECK_HR;
    
    if (lAttributeNum != mAttributeNum)
    {
        //if initial loading failed and mAttributeNum is not set, we should not continue, otherwise it crashes
        return E_FAIL;
    }
    
    bool* lpSelectedUnits = new bool[mAttributeNum];
    MBase::StrongPtr<bool> lpHolder(lpSelectedUnits);
    CHECK_NEWED_PTR(lpSelectedUnits);
    
    memset(lpSelectedUnits, 0, sizeof(bool) * mAttributeNum);
    
    int lPosition = ipReadStream->GetPosition();
    hr = ipReadStream->BeginReadBlock(&lBlockType);
    if (hr != S_OK || lBlockType != EnumNewBlockOptionalSelectUnit)
    {
        hr = ipReadStream->EndReadBlock();
        CHECK_HR;
        ipReadStream->SetPosition(lPosition);
    }
    else
    {
        for (int i = 0; i < mAttributeNum; i++)
        {
            hr = ipReadStream->ReadBool(&lpSelectedUnits[i]);
            CHECK_HR;
        }
        ipReadStream->EndReadBlock();
    }
    
    for (int i = 0; i < mAttributeNum; i ++)
    {
        if (mfReLoad && lpSelectedUnits[i])
        {
            mvpGlobalLookUps[i]->cleanup();
            hr = mvpGlobalLookUps[i]->LoadDataFromBinary(ipReadStream, true, ipObjContext);
            CHECK_HR;
        }
        else
        {
            hr = mvpGlobalLookUps[i]->LoadDataFromBinary(ipReadStream, !mfLoadStatus, ipObjContext);
            CHECK_HR;
        }
    }
    
    //GUnitMap
    int lnGUnitMap;
	hr = ipReadStream->ReadInt(&lnGUnitMap);
    CHECK_HR;
    
    assert(lnGUnitMap == mvpGUnitMapList.size());
    
    for (int i = 0; i < lnGUnitMap; i++)
    {
        //tliao, 959484,959583, 2014/08/26, postpone archive time to avoid crash
        //tliao, 905202, 2014/08/22, need to archive the old mvUnits of GUnitMap to avoid memory leak
        //mvpGUnitMapList[i]->archive();
        hr = mvpGUnitMapList[i]->LoadDataFromBinary(ipReadStream, ipObjContext);
        CHECK_HR;
    }
    
    //if(ipReadStream->MoreDataInBlock())
    if(ipReadStream->ProbeBlockHeader(EnumNewBlockNewDADE))
    {
        hr = ipReadStream->BeginReadBlock(&lBlockType);
        CHECK_HR;
        
        int lnDerivedUnit;
        hr = ipReadStream->ReadInt(&lnDerivedUnit);
        CHECK_HR;
        
        for(int i = 0; i < lnDerivedUnit; ++i)
        {
            int lUnitType;
            hr = ipReadStream->ReadInt(&lUnitType);
            CHECK_HR;
            
            
            if(lUnitType == DssTemplateAttribute)
            {
                DSSGlobalLookUpTable *lpAttr = static_cast<DSSGlobalLookUpTable *>(mvDerivedUnits[i]);
                hr = lpAttr->LoadDataFromBinary(ipReadStream, !mfLoadStatus, ipObjContext);
                CHECK_HR;
            }
            else if(lUnitType == DssTemplateConsolidation)
            {
                MBase::CString lDSSID;
                hr = ipReadStream->ReadString(lDSSID);
                CHECK_HR;
                
                GUID lGUID;
                hr = DSSViewDataSetHelper::str2GUID(lDSSID, lGUID);
                CHECK_HR;
                
                DSSGlobalLookUpTable *lpChildUnit = NULL;
                bool isFound = false;
                for(int i = 0; i < mvpGlobalLookUps.size(); ++i)
                {
                    if(mvpGlobalLookUps[i]->getGUID() == lGUID)
                    {
                        lpChildUnit = mvpGlobalLookUps[i];
                        isFound = true;
                        break;
                    }
                }
                
                //yimliu, 2014/07/10, 922697, crash. If consolidation is based on DA, we have to find its child in derived units.
                if (!isFound) {
                    for (int i = 0; i < mvDerivedUnits.size(); ++i) {
                        if (mvDerivedUnits[i] != NULL && mvDerivedUnits[i]->getUnitType() == DssTemplateAttribute && mvDerivedUnits[i]->getGUID() == lGUID) {
                            lpChildUnit = static_cast<DSSGlobalLookUpTable *>(mvDerivedUnits[i]);
                            break;
                        }
                    }
                }
                
                EnumBinaryObjectType objtype = EnumObjectReserved;
                hr = ipReadStream->ReadInt((int*)(&objtype));
                CHECK_HR;
                
                assert(objtype == EnumObjectConsolidationUnit);
                
                int lUnitID = 0;
                hr = ipReadStream->ReadInt(&lUnitID);
                CHECK_HR;
                
                DSSTabularUnit *lpUnit = NULL;
                hLoadUnitConsolidation(ipReadStream, lUnitID, &lpUnit, lpChildUnit, true);
                
                lpUnit->setObjectContextRef(ipObjContext);
                
                mvDerivedUnits[i] = lpUnit;
            }
        }
        
        hr = ipReadStream->EndReadBlock();
        CHECK_HR;
    }
    
    mfLoadStatus = true;
    mfReLoad = false;
    
    hr = ipReadStream->EndReadBlock();
    return hr;
}

int DSSGUnitContainer::getGlobalLookup(int iIndex, DSSTabularAttribute** oppLookupTable)
{
    if (oppLookupTable == NULL || iIndex < 0 || iIndex >= mvpGlobalLookUps.size()+mvpGUnitMapList.size()+mvDerivedUnits.size()+1)
        return E_INVALIDARG;
    
    if(iIndex < mvpGlobalLookUps.size())
    {
        *oppLookupTable = mvpGlobalLookUps[iIndex];
    }
    else if(iIndex <= mvpGlobalLookUps.size() + mvpGUnitMapList.size())
    {
        return E_INVALIDARG;
    }
    else
    {
        int lIndex = iIndex - mvpGlobalLookUps.size() - mvpGUnitMapList.size() - 1;
        if(mvDerivedUnits[lIndex]->getUnitType() == DssTemplateAttribute)
        {
            *oppLookupTable = static_cast<DSSGlobalLookUpTable *>(mvDerivedUnits[lIndex]);
        }
        else
        {
            return E_INVALIDARG;
        }
    }
    
    
    return S_OK;
}

int DSSGUnitContainer::getGlobalLookupByID(GUID* ipID, int* opIndex, DSSTabularAttribute** oppLookupTable)
{
    if (!ipID)
        return E_INVALIDARG;
    
    int lnUnit = mvpGlobalLookUps.size();
    int lUnit;
    for (lUnit = 0; lUnit < lnUnit; lUnit++)
    {
        if (ATLINLINE::IsEqualGUID(mvLookUpIDs[lUnit], *ipID))
        {
            if (opIndex)
                *opIndex = lUnit;
            if (oppLookupTable)
                *oppLookupTable = mvpGlobalLookUps[lUnit];
            return S_OK;
        }
        
    }
    
    //zhyang, 909683, it can be derived attribute
    int idxToAdd = mvpGlobalLookUps.size() + mvpGUnitMapList.size() + 1;
    for(int i = 0; i < mvDerivedUnits.size(); ++i)
    {
        if(mvDerivedUnits[i]->getUnitType() == DssTemplateAttribute
           && ATLINLINE::IsEqualGUID(mvDerivedUnits[i]->getGUID(), *ipID))
        {
            if(opIndex)
                *opIndex = i + idxToAdd;
            if(oppLookupTable)
                *oppLookupTable = static_cast<DSSTabularAttribute *> (mvDerivedUnits[i]);
            return S_OK;
        }
    }
    
    return S_FALSE;
    
}

int DSSGUnitContainer::getGUnitMap(int iIndex, DSSGUnitMap** oppGUnitMap)
{
    if (!oppGUnitMap || iIndex < 0 || iIndex >= mvpGUnitMapList.size())
        return E_INVALIDARG;
    *oppGUnitMap = mvpGUnitMapList[iIndex];
    return S_OK;
}

int DSSGUnitContainer::getUnitFromGUnitMapByID(GUID& iGUID, EnumDSSTemplateUnitType* ipType, int iUnitIndex, DSSTabularUnit** oppTabularUnit)
{
    for (int iUnit = 0; iUnit < mvpGUnitMapList.size(); iUnit++)
        if (ATLINLINE::IsEqualGUID(mvpGUnitMapList[iUnit]->mID, iGUID))
        {
            if (mvpGUnitMapList[iUnit]->mUnitType == *ipType && iUnitIndex >= 0 && iUnitIndex < mvpGUnitMapList[iUnit]->mvUnits.size())
            {
                *oppTabularUnit = mvpGUnitMapList[iUnit]->mvUnits[iUnitIndex];
                return S_OK;
            }
        }
    
    return S_FALSE;
}

//ctang: return the number of attributes
int DSSGUnitContainer::countLookupTables()
{
    return mvpGlobalLookUps.size();
}
//ctang: return the number of all units, include CS and CG
int DSSGUnitContainer::count()
{
    return mvpGlobalLookUps.size() + mvpGUnitMapList.size();
}

int DSSGUnitContainer::GetGlobalUnitGUIDByID(int iGlobalID, GUID* opGUID)
{
    int lnUnit = mvpGUnitMapList.size() + mvLookUpIDs.size() + mvDerivedUnits.size() + 1;
    
    if (iGlobalID < 0 || iGlobalID >= lnUnit || !opGUID)
        return E_INVALIDARG;
    
    if (iGlobalID < mvpGlobalLookUps.size())
        *opGUID = mvLookUpIDs[iGlobalID];
    else if(iGlobalID < mvpGlobalLookUps.size() + mvpGUnitMapList.size())
    {
        DSSGUnitMap* lpUnitMap = mvpGUnitMapList[iGlobalID - mvpGlobalLookUps.size()];
        *opGUID = lpUnitMap->mID;
    }
    else if(iGlobalID == mvpGlobalLookUps.size() + mvpGUnitMapList.size())
    {
        //zhyang, dummy unit;
        *opGUID = GUID_NULL;
    }
    else
    {
        *opGUID = mvDerivedUnits[iGlobalID-mvpGlobalLookUps.size()-mvpGUnitMapList.size()-1]->getGUID();
    }
    return S_OK;
}

int DSSGUnitContainer::GetGlobalUnitIDByGUID(GUID iID, int* opGlobalID)
{
    int i = 0;
    for (; i < mvpGlobalLookUps.size(); i++)
    {
        if (mvLookUpIDs[i] == iID)
        {
            *opGlobalID = i;
            return S_OK;
        }
    }
    
    for (i = 0; i < mvpGUnitMapList.size(); i++)
    {
        DSSGUnitMap* lpGUnitMap = mvpGUnitMapList[i];
        if (lpGUnitMap->mID == iID)
        {
            *opGlobalID = mvpGlobalLookUps.size() + i;
            return S_OK;
        }
    }
    
    for(int i = 0; i < mvDerivedUnits.size(); ++i)
    {
        if(mvDerivedUnits[i]->getGUID() == iID)
        {
            *opGlobalID = mvpGlobalLookUps.size() + mvpGUnitMapList.size() + i + 1;
            return S_OK;
        }
    }
    
    return S_FALSE;
}

int DSSGUnitContainer::GetUnitType(int iIndex, EnumDSSTemplateUnitType* opType, bool* opfDE)
{
    int lnUnit = mvpGlobalLookUps.size() + mvpGUnitMapList.size() + mvDerivedUnits.size()+1;
    if (iIndex < 0 || iIndex >= lnUnit)
        return E_INVALIDARG;
    
    int hr = S_OK;
    
    if (iIndex < mvpGlobalLookUps.size())
    {
        *opType = DssTemplateAttribute;
        
        hr = mvpGlobalLookUps[iIndex]->HasDE(opfDE);
        CHECK_HR;        
    }
    else if(iIndex < mvpGlobalLookUps.size() + mvpGUnitMapList.size())
    {
        int lIndex = iIndex - mvpGlobalLookUps.size();
        
        *opType = mvpGUnitMapList[lIndex]->mUnitType;   
    }
    else if(iIndex == mvpGlobalLookUps.size() + mvpGUnitMapList.size())
    {
        //zhyang, it should be a dummy unit
        assert(false);
    }
    else{
        int lIndex = iIndex - mvpGlobalLookUps.size() - mvpGUnitMapList.size() - 1;
        *opType = mvDerivedUnits[lIndex]->getUnitType();
    }
    return S_OK;
}

int DSSGUnitContainer::GetGlobalRowCount(int iIndex, int *opGlobalRowCount, bool ifIncludeDE)
{
	AE_ASSERT(opGlobalRowCount);
	HRESULT hr = S_OK;
	if (iIndex < mvpGlobalLookUps.size())
	{
		DSSGlobalLookUpTable* lpLookUpTable = (mvpGlobalLookUps[iIndex]);
		if (ifIncludeDE)
		{
            hr = lpLookUpTable->GetMaxGlobalKey(opGlobalRowCount);
            CHECK_HR;
            (*opGlobalRowCount)++;
//			_ASSERT(false);
//            return E_NOTIMPL;
		}
		else
		{
			*opGlobalRowCount = lpLookUpTable->CountRows();
		}
		return hr;
	}
	else if(iIndex < mvpGlobalLookUps.size() + mvpGUnitMapList.size())
	{
		int lIndex = iIndex - mvpGlobalLookUps.size();
		
		DSSGUnitMap* lGUnitMap = mvpGUnitMapList[lIndex];
		*opGlobalRowCount = lGUnitMap->mGlobalRowCount;
		CHECK_HR;
        
		return hr;
	}else if (iIndex == mvpGlobalLookUps.size() + mvpGUnitMapList.size())
    {
        *opGlobalRowCount = 1;
        return hr;
    }
    else
    {
        int lIndex = iIndex - mvpGlobalLookUps.size() - mvpGUnitMapList.size() - 1;
        *opGlobalRowCount = mvDerivedUnits[lIndex]->CountRows();
        return hr;
    }
    
    return S_FALSE;
}

int DSSGUnitContainer::Order(int iUnit, int iForm, int iRow1, int iRow2, bool fIsNullDisplayOnTop, bool fIsParentFirst, int Locale, bool fUseDefaultSort, int* pResult, bool fUseBaseElement,vector<int> *ipvViewIndices)
{
    int hr = S_OK;
    
    //junwang, 01/31/2012, speedup 
    if (iRow1 == iRow2)
    {
        *pResult = 0;
        return hr;
    }
    
    RWUtilContext lContext;
	
    lContext.mfDisplayNullOnTop = fIsNullDisplayOnTop;
    lContext.mForm = iForm;
    lContext.mLocale = Locale;
    lContext.mfIsDefaultSort = fUseDefaultSort;
    
    if (iUnit < mvLookUpIDs.size())
	{
		DSSGlobalLookUpTable* lpLookUpTable = mvpGlobalLookUps[iUnit];
        
        //handle the special NULL
        int lMaxKey = -1;
        hr = lpLookUpTable->GetMaxGlobalKey(&lMaxKey);
        CHECK_HR;
        int lRowCount = lMaxKey+1;
        if(iRow1==lRowCount || iRow2 == lRowCount)
        {
            if(iRow1 == lRowCount)
                *pResult = fIsNullDisplayOnTop?-1:1;
            else
                *pResult = fIsNullDisplayOnTop?1:-1;
            
            return S_OK;
        }
        
		Int32 lLocalIndex1, lLocalIndex2;
		DSSTabularConsolidation* lLocalObj1 = NULL;
		DSSTabularConsolidation* lLocalObj2 = NULL;
		bool lfDEElement1 = false, lfDEElement2 = false;
        
        int lSourceIndex1 = 0, lSourceIndex2 = 0;
        int lViewIndex1 = 0, lViewIndex2 = 0;
        
		//ctang: for the rows less than mnRows, it also may be the DE leaf element
		DSSTabularUnit* lLocalObj;
        hr = lpLookUpTable->GetGlobalUnit(iRow1, &lLocalObj, &lLocalIndex1, &lfDEElement1,!fUseBaseElement, &lSourceIndex1, &lViewIndex1,ipvViewIndices/*TQMS 731649,hyan,3/27/2013 following flash's change*/);

		CHECK_HR;
        
		EnumDSSTemplateUnitType lType= lLocalObj->getUnitType();
		        
		if (lType == DssTemplateConsolidation)
		{
			lfDEElement1 = true;
			lLocalObj1 = static_cast<DSSTabularConsolidation*>(lLocalObj);
		}
        
        hr = lpLookUpTable->GetGlobalUnit(iRow2, &lLocalObj, &lLocalIndex2, &lfDEElement2,!fUseBaseElement, &lSourceIndex2, &lViewIndex2,ipvViewIndices/*TQMS 731649,hyan,3/27/2013 following flash's change*/);

		CHECK_HR;
        
		lType = lLocalObj->getUnitType();
		CHECK_HR;
        
		if (lType == DssTemplateConsolidation)
		{
			lfDEElement2 = true;
			lLocalObj2 = static_cast<DSSTabularConsolidation*>(lLocalObj);
		}
        
		if (iRow1 < (lpLookUpTable->CountRows()) && iRow2 < (lpLookUpTable->CountRows()) && !lfDEElement1 && !lfDEElement2)
		{
			Int32 lColumn;
			hr = lpLookUpTable->FindFormWithLocale(0, iForm, 0, lColumn);
			if (hr != S_OK && hr != S_FALSE) return hr;
            
			// rzhu, 2011/06/01, if we cannot find column, treat it as eaqual
			if ( hr == S_FALSE)
			{
				*pResult = 0;
				return S_OK;
			}
            
            DSSDataColumn* lpColumn;
            hr = lpLookUpTable->Item(lColumn, &lpColumn);
            CHECK_HR;
            
            *pResult = static_cast<DSSDataRefColumn*>(lpColumn)->Order(iRow1, iRow2,fIsNullDisplayOnTop);
            return hr;
		}
        
		if (!lfDEElement1 || !lfDEElement2)
		{
			DSSTabularConsolidation* lpConsolidation;
			bool lbfReverse = false;
			Int32 lConLocal, lAttrLocal;
            
			if(!lfDEElement2)
			{
				lConLocal = lLocalIndex1;
				lAttrLocal = iRow2;
				lpConsolidation = lLocalObj1;
			}
			else
			{
				lbfReverse = true;
                
				lConLocal = lLocalIndex2;
				lAttrLocal = iRow1;
				lpConsolidation = lLocalObj2;
			}
            
            int lResult = -1;
        
            hr = CompareDEWithAttr4Display(lpConsolidation, lConLocal, lpLookUpTable, lAttrLocal, lContext, lbfReverse, lResult);

            if(lResult == 0)
                *pResult = 0;
            if(lResult>0)
                *pResult = 1;
            if(lResult<0)
                *pResult = -1;
            
			return S_OK;
		}
		else
		{
            int lResult;
            
            int lIndex1 = lViewIndex1 >= 0 ? lViewIndex1 : -1 * lSourceIndex1;
            int lIndex2 = lViewIndex2 >= 0 ? lViewIndex2 : -1 * lSourceIndex2;
            
            hr=CompareCSElements4Display(lLocalObj1,lLocalIndex1,NULL,lLocalObj2,lLocalIndex2, NULL, lContext,
                                         lIndex1, lIndex2,//ctang for DE sorting
                                         lResult);
            
            if(lResult == 0)
                *pResult = 0;
            if(lResult>0)
                *pResult = 1;
            if(lResult<0)
                *pResult = -1;
            
			return S_OK;
		}
	}
	else if(iUnit < mvpGlobalLookUps.size() + mvpGUnitMapList.size())
	{
		int lIndex = iUnit - mvpGlobalLookUps.size();
		AE_ASSERT(mvpGUnitMapList.size() > 0);
        
		DSSGUnitMap* lGUnitMap = mvpGUnitMapList[lIndex];
		int lUnitIndex1, lUnitIndex2;
		int lLocalKey1, lLocalKey2;
		int hr1 = lGUnitMap->GetUnitIndexAndLocalKey(iRow1, &lUnitIndex1, &lLocalKey1);
		if (FAILED(hr1))
			return hr1;
		int hr2 = lGUnitMap->GetUnitIndexAndLocalKey(iRow2, &lUnitIndex2, &lLocalKey2);
		if (FAILED(hr2))
			return hr2;
        
		// will handle the special NULL
		if (hr1 == S_FALSE ||  hr2 == S_FALSE)
		{
			if (hr1 == hr2) // both are NULL
				*pResult = 0;
			else if (hr1 == S_FALSE)
				*pResult = fIsNullDisplayOnTop ? -1 : 1;
			else
				*pResult = fIsNullDisplayOnTop ? 1 : -1;
            
			return S_OK;
		}
        
		switch(lGUnitMap->mUnitType)
		{
            case DssTemplateCustomGroup:
			{
                
                //junwang, 02/10/2012, #545080, use global key for CG comparison
            /*
                DSSTabularCustomGroup* lpCustomGroup1 = dynamic_cast<DSSTabularCustomGroup*>(lGUnitMap->mvUnits[lUnitIndex1]);
                DSSTabularCustomGroup* lpCustomGroup2 = dynamic_cast<DSSTabularCustomGroup*>(lGUnitMap->mvUnits[lUnitIndex2]);
                CompareCGElements(lpCustomGroup1, lLocalKey1, lpCustomGroup2, lLocalKey2, lContext, fIsParentFirst, lResult);
             */
                int lResult;
                if(iRow1<iRow2)
                    lResult = -1;
                else if(iRow1>iRow2)
                    lResult = 1;
                else
                    lResult = 0;
                
				if(lResult == 0) *pResult = 0;
				if(lResult > 0) *pResult = 1;
				if(lResult < 0) *pResult = -1;
                
				return S_OK;
			}
                
            case DssTemplateConsolidation:
			{
                DSSTabularConsolidation* lpConsolidation1 = dynamic_cast<DSSTabularConsolidation*>(lGUnitMap->mvUnits[lUnitIndex1]);
                DSSTabularConsolidation* lpConsolidation2 = dynamic_cast<DSSTabularConsolidation*>(lGUnitMap->mvUnits[lUnitIndex2]);
                
				int lResult;
                
                hr = CompareCSElements4Display(lpConsolidation1, lLocalKey1, NULL, lpConsolidation2, lLocalKey2, NULL, lContext, 0, 0,//ctang: for CS sorting, treat as equal
                    lResult);
				CHECK_HR;
                
				if(lResult == 0) *pResult = 0;
				if(lResult > 0) *pResult = 1;
				if(lResult < 0) *pResult = -1;
                
				return S_OK;
			}
            default:
                return E_FAIL;
		}
        
	}
    else
    {
        int lIndex = iUnit - mvpGlobalLookUps.size() - mvpGUnitMapList.size() - 1;
        assert(lIndex >= 0);
        assert(lIndex < mvDerivedUnits.size());
        DSSTabularUnit *lpUnit = mvDerivedUnits[lIndex];
        DSSGlobalLookUpTable* lpLookUpTable = static_cast<DSSGlobalLookUpTable *>(lpUnit);
        if(lpUnit->getUnitType() == DssTemplateAttribute)
        {
            Int32 lColumn;
			hr = lpLookUpTable->FindFormWithLocale(0, iForm, 0, lColumn);
			if (hr != S_OK && hr != S_FALSE) return hr;
            
			// rzhu, 2011/06/01, if we cannot find column, treat it as eaqual
			if ( hr == S_FALSE)
			{
				*pResult = 0;
				return S_OK;
			}
            
            DSSDataColumn* lpColumn;
            hr = lpLookUpTable->Item(lColumn, &lpColumn);
            CHECK_HR;
            
            *pResult = static_cast<DSSDataRefColumn*>(lpColumn)->Order(iRow1, iRow2,fIsNullDisplayOnTop);
            return hr;
        }
        else if(lpUnit->getUnitType() == DssTemplateConsolidation)
        {
            
            DSSTabularConsolidation* lpCS = static_cast<DSSTabularConsolidation *>(lpUnit);
            int lLocalKey1 = lpCS->TranslateGlobalToLocal(iRow1);
            int lLocalKey2 = lpCS->TranslateGlobalToLocal(iRow2);
            
            int lResult;
            
            hr = CompareCSElements4Display(lpCS, lLocalKey1, NULL, lpCS, lLocalKey2, NULL, lContext, 0, 0,//ctang: for CS sorting, treat as equal
                                           lResult);
            CHECK_HR;
            
            if(lResult == 0) *pResult = 0;
            if(lResult > 0) *pResult = 1;
            if(lResult < 0) *pResult = -1;
        }
        else
        {
            assert(false);
            *pResult = -1;
        }
        
        return S_OK;
    }
    
	return S_FALSE;
}
/*
int DSSGUnitContainer::GetLocalToGlobalMap(DSSTabularUnit* ipUnit, int** oppMap)
{
    if (!ipUnit || !oppMap)
        return E_INVALIDARG;
    
    GUID lID;
    int hr = ipUnit->getGUID(lID);
    CHECK_HR;
    
    *oppMap = NULL;
    
    for (int i = 0; i < mvLookUpIDs.size(); i++)
    {
        if (mvLookUpIDs[i] == lID)
        {
            EnumDSSTemplateUnitType lType = ipUnit->getUnitType();
            return mvpGlobalLookUps[i]->GetLocalToGlobalMap(ipUnit, oppMap, lType);
        }
    }
    
    for (int i = 0; i < mvpGUnitMapList.size(); i++)
    {
        if (mvpGUnitMapList[i]->mID == lID)
        {
            DSSGUnitMap* lpUnitMap = mvpGUnitMapList[i];
            AE_ASSERT(lpUnitMap);
            
            for (int j = 0; j < lpUnitMap->mvUnits.size(); j++)
                //fali, 11/25/2013, 815090, for two datasets which contains the same consolidation, GUIDs are the same. Get the map by comparing address firstly.
                if(ipUnit == lpUnitMap->mvUnits[j]){
                    *oppMap = lpUnitMap->mvLocalToGlobalMap[j];
                    return S_OK;
                }
            
            for (int j = 0; j < lpUnitMap->mvUnits.size(); j++)
                //tliao, 10/14/2013, after UC manipulation, the consolidation unit in GUnitContainer has been changed,
                //but the consolidation unit in base dataset tabular data is not changed
                if (ipUnit->getGUID() == lpUnitMap->mvUnits[j]->getGUID())
                {
                    *oppMap = lpUnitMap->mvLocalToGlobalMap[j];
                    return S_OK;
                }
        }
    }
    
    return S_FALSE;
}*/
/*
// This func is designed for non-globallookup units only
int DSSGUnitContainer::GetGlobalToLocalMap(DSSTabularUnit* ipUnit, Int32* opRowCount, Int32** oppRowMap, IntToIntMapType** oppRowMap2)
{
	if (!opRowCount || !oppRowMap || !oppRowMap2)
		return E_INVALIDARG;
    
	*opRowCount = -1;
	*oppRowMap = NULL;
	*oppRowMap2 = NULL;
    
	GUID lID;
	HRESULT hr = ipUnit->getGUID(lID);
	CHECK_HR;
    
	//ctang 2010-10-26 handle for DE
	DSSTabularAttribute* lGlobalLookUp;
	hr = getGlobalLookupByID(&lID, NULL, &lGlobalLookUp);
	if (hr == S_OK && lGlobalLookUp)
	{
		DSSGlobalLookUpTable* lCGlobalLookUp = static_cast<DSSGlobalLookUpTable*>(lGlobalLookUp);
        
		return lCGlobalLookUp->GetGlobalToLocalMap(ipUnit, opRowCount, oppRowMap, oppRowMap2);
	}
    
    for (int i = 0; i < mvpGUnitMapList.size(); i++)
    {
        if (mvpGUnitMapList[i]->mID == lID)
        {
            DSSGUnitMap* lpUnitMap = mvpGUnitMapList[i];
            AE_ASSERT(lpUnitMap);
            
            return lpUnitMap->GetGlobalToLocalMap(ipUnit, opRowCount, oppRowMap);
        }
    }
    
	return S_FALSE;
}
*/
int DSSGUnitContainer::GetGlobalUnit(GUID& iCSID, int iGlobalID, DSSTabularUnit** opUnit, int* opLocalKey, bool* ofDE)
{
	if (ofDE)
		*ofDE = false;
	for (Int32 iLookup = 0; iLookup < mvLookUpIDs.size(); iLookup++)
	{
		if(mvLookUpIDs[iLookup] == iCSID)
		{
			//what about DE
			DSSGlobalLookUpTable* lCGlobalLookUp = mvpGlobalLookUps[iLookup];
            
			int hr = lCGlobalLookUp->GetGlobalUnit(iGlobalID, opUnit, opLocalKey, ofDE);
			CHECK_HR;
            
			return S_OK;
		}
	}
    
	for (Int32 iUnit = 0; iUnit < mvpGUnitMapList.size(); iUnit++)
		{
			if (mvpGUnitMapList[iUnit]->mID == iCSID)
			{
				DSSGUnitMap* lpUnitMap = mvpGUnitMapList[iUnit];
				AE_ASSERT(lpUnitMap);
                
				int lUnitIndex;
				int hr = lpUnitMap->GetUnitIndexAndLocalKey(iGlobalID, &lUnitIndex, opLocalKey);
				CHECK_HR;
                
				*opUnit = lpUnitMap->mvUnits[lUnitIndex];
				return S_OK;
			}
		}
    
    for(int iUnit = 0; iUnit < mvDerivedUnits.size(); ++iUnit)
    {
        if(ATLINLINE::IsEqualGUID(mvDerivedUnits[iUnit]->getGUID(), iCSID))
        {
            DSSTabularUnit* lpUnit = mvDerivedUnits[iUnit];
            EnumDSSTemplateUnitType lType = lpUnit->getUnitType();
            if(lType == DssTemplateAttribute)
            {
                *opUnit = lpUnit;
                *opLocalKey = iGlobalID;
                return S_OK;
            }
            else if(lType == DssTemplateConsolidation)
            {
                *opUnit = lpUnit;
                DSSTabularConsolidation *lpCS = static_cast<DSSTabularConsolidation *>(lpUnit);
                *opLocalKey = lpCS->TranslateGlobalToLocal(iGlobalID);
                
                if(ofDE)
                    *ofDE = true;
                return S_OK;
            }
            else
            {
                assert(false);
            }
        }
    }
    
	return S_FALSE;
    
}

int DSSGUnitMap::GetUnitIndexAndLocalKey(int iGlobalKey, int* opUnitIndex, int* opLocalKey)
{
	// if passing global key is out of index, return S_FALSE
	// it is possible because mGlobalRowCount value is used to indicate Compound NULL
	if (iGlobalKey < 0 || iGlobalKey >= mGlobalRowCount)
	{
		return S_FALSE;
	}
	for (Int32 i = 0; i < mvGlobalToLocalMap.size(); i++)
	{
		if(!mvGlobalToLocalMap[i])
			return S_FALSE;
		if (mvGlobalToLocalMap[i][iGlobalKey] != -1)
		{
			*opUnitIndex = i;
			*opLocalKey = mvGlobalToLocalMap[i][iGlobalKey];
			return S_OK;
		}
	}
    
	return S_FALSE;
}

int DSSGUnitContainer::getUnitsForViewDataSet(GUID* ipGUID, EnumDSSTemplateUnitType* ipType, int iUnitIndex, DSSTabularUnit** oppTabularUnit)
{
    if (!oppTabularUnit)
        return E_INVALIDARG;
    
    *oppTabularUnit = NULL;
    
    if (!ipGUID || !ipType)
        return S_FALSE;//empty unit
    
    for (int iLookup = 0; iLookup < mvLookUpIDs.size(); iLookup++)
        if (ATLINLINE::IsEqualGUID(mvLookUpIDs[iLookup], *ipGUID))
        {
            return mvpGlobalLookUps[iLookup]->GetRefUnit(ipType, iUnitIndex, oppTabularUnit);
        }
    
    for (int iUnit = 0; iUnit < mvpGUnitMapList.size(); iUnit++)
        if (ATLINLINE::IsEqualGUID(mvpGUnitMapList[iUnit]->mID, *ipGUID))
        {
            if (mvpGUnitMapList[iUnit]->mUnitType == *ipType && iUnitIndex >= 0 && iUnitIndex < mvpGUnitMapList[iUnit]->mvUnits.size())
            {
                DSSTabularUnit* lpUnit = mvpGUnitMapList[iUnit]->mvUnits[iUnitIndex];
                
                
                //xzeng 2012-02-23, 
                if (*ipType == DssTemplateAttribute)
                {
                    DSSXTabRefLookupTable* lpRefLookup = new DSSXTabRefLookupTable;
                    int hr = lpRefLookup->Init(static_cast<DSSTabularAttribute*>(lpUnit));
                    CHECK_HR;
                    
                    *oppTabularUnit = lpRefLookup;
                }
                else if (*ipType == DssTemplateConsolidation)
                {
                    DSSTabularRefConsolidation* lpRefCS = new DSSTabularRefConsolidation();
                    int hr = lpRefCS->Init(static_cast<DSSTabularConsolidation*>(lpUnit));
                    CHECK_HR;
                    
                    hr = lpRefCS->SetGlobalRowMap(mvpGUnitMapList[iUnit]->mGlobalRowCount, mvpGUnitMapList[iUnit]->mvGlobalToLocalMap[iUnitIndex], NULL, mvpGUnitMapList[iUnit]->mvLocalToGlobalMap[iUnitIndex]);
                    CHECK_HR;
                    
                    //zhyang, 836541, set join null
                    lpRefCS->setJoinNull(mvpGUnitMapList[iUnit]->mGlobalRowCount);
                    
                    *oppTabularUnit = lpRefCS;
                }
                else
                {
                    DSSTabularRefCustomGroup* lpRefCG = new DSSTabularRefCustomGroup();
                    int hr = lpRefCG->Init(static_cast<DSSTabularCustomGroup*>(lpUnit));
                    CHECK_HR;
                    
                    hr = lpRefCG->SetGlobalRowMap(mvpGUnitMapList[iUnit]->mGlobalRowCount, mvpGUnitMapList[iUnit]->mvGlobalToLocalMap[iUnitIndex], mvpGUnitMapList[iUnit]->mvLocalToGlobalMap[iUnitIndex]);
                    CHECK_HR;
                    
                    *oppTabularUnit = lpRefCG;
                }
                return S_OK;
            }
            else
            {
                assert(false);
                return E_FAIL;
            }
        }
    
    for(int i = 0; i < mvDerivedUnits.size(); ++i)
    {
        if(ATLINLINE::IsEqualGUID(mvDerivedUnits[i]->getGUID(), *ipGUID))
        {
            DSSTabularUnit* lpUnit = mvDerivedUnits[i];
            
            if (*ipType == DssTemplateAttribute)
            {
                DSSXTabRefLookupTable* lpRefLookup = new DSSXTabRefLookupTable;
                int hr = lpRefLookup->Init(static_cast<DSSTabularAttribute*>(lpUnit));
                CHECK_HR;
                
                *oppTabularUnit = lpRefLookup;
            }
            else if (*ipType == DssTemplateConsolidation)
            {
                DSSTabularRefConsolidation* lpRefCS = new DSSTabularRefConsolidation();
                int hr = lpRefCS->Init(static_cast<DSSTabularConsolidation*>(lpUnit));
                CHECK_HR;
                
             /*   hr = lpRefCS->SetGlobalRowMap(lpUnit->mGlobalRowCount, mvpGUnitMapList[iUnit]->mvGlobalToLocalMap[iUnitIndex], NULL, mvpGUnitMapList[iUnit]->mvLocalToGlobalMap[iUnitIndex]);
                CHECK_HR;
                
                //zhyang, 836541, set join null
                lpRefCS->setJoinNull(mvpGUnitMapList[iUnit]->mGlobalRowCount);
               */
                *oppTabularUnit = lpRefCS;
            }
            
            return S_OK;
        }
    }
    
    return S_FALSE;//not found
}

int DSSGUnitContainer::getBucketID(int iUnitID, vector<int>& ivForms, bool* ipAsc, bool ibNullOnTop)
{
    DSSTabularAttribute *lpAttr;
    int hr = getGlobalLookup(iUnitID, &lpAttr);
    if(hr != S_OK)
        return -1;
    
    DSSGlobalLookUpTable *lpGlobalLookup = dynamic_cast<DSSGlobalLookUpTable *>(lpAttr);
    if(lpGlobalLookup)
    {
        return lpGlobalLookup->getBucketID(ivForms, ipAsc, ibNullOnTop);
    }
    return -1;
}

int DSSGUnitContainer::getSortResult(int iUnitID, int iBucketID, int** oppSortingResult, int* opnResult)
{
    DSSTabularAttribute *lpAttr;
    int hr = getGlobalLookup(iUnitID, &lpAttr);
    CHECK_HR;
    
    DSSGlobalLookUpTable *lpGlobalLookup = dynamic_cast<DSSGlobalLookUpTable *>(lpAttr);
    if(lpGlobalLookup)
    {
        return lpGlobalLookup->getSortResult(iBucketID, oppSortingResult, opnResult);
    }
    return E_FAIL;
}

int DSSGUnitContainer::setSortingResult(int iUnitID, vector<Int32>& ivForms, vector<bool>& ipAsc, bool ibNullOnTop, Int32* ipOrder)
{
    DSSTabularAttribute *lpAttr;
    int hr = getGlobalLookup(iUnitID, &lpAttr);
    CHECK_HR;
    
    DSSGlobalLookUpTable *lpGlobalLookup = dynamic_cast<DSSGlobalLookUpTable *>(lpAttr);
    if(lpGlobalLookup)
    {
        return lpGlobalLookup->setSortingResult(ivForms, ipAsc, ibNullOnTop, ipOrder);
    }
    return E_FAIL;
}

int DSSGUnitContainer::setTrimFlag(bool ibTrimFlag)
{
    for(int i=0; i< mvpGlobalLookUps.size(); i++)
    {
        if(mvpGlobalLookUps[i])
        {
            mvpGlobalLookUps[i]->setTrimFlag(ibTrimFlag);
        }
    }
    
    return S_OK;
}

int DSSGUnitContainer::findElement4CSCG(DSSBaseElementProxy *ipElement, int &orKey)
{
    int hr = S_OK;
    for(int i = 0; i < mvpGUnitMapList.size(); ++i)
    {
        hr = mvpGUnitMapList[i]->findElement(ipElement, orKey);
        if(hr == S_OK)
            return hr;
    }
    
    return S_FALSE;
}
vector<int>* DSSGUnitContainer::getLocalToGlobalMap(GUID iDataSetGUID, DSSTabularUnit* ipUnit)
{
    GUID lUnitGUID = ipUnit->getGUID();
    if (mvpGlobalLookUps.size()>0) {
        for (int i=0; i<mvpGlobalLookUps.size(); i++) {
            if(mvpGlobalLookUps[i])
            {
                GUID lAttrGUID = mvpGlobalLookUps[i]->getGUID();
                if (lUnitGUID == lAttrGUID) {
                    return mvpGlobalLookUps[i]->getLocalToGlobalMap(iDataSetGUID);
                }
            }
        }
    }
    for (int i = 0; i < mvpGUnitMapList.size(); i++)
    {
        if (mvpGUnitMapList[i]->mID == lUnitGUID)
        {
            DSSGUnitMap* lpUnitMap = mvpGUnitMapList[i];
            return lpUnitMap->GetLocalToGlobalMap(ipUnit);
            
        }
    }
    return NULL;
}

vector<int>* DSSGUnitContainer::getGlobalToLocalMap(GUID iDataSetGUID, DSSTabularUnit* ipUnit)
{
    GUID lUnitGUID = ipUnit->getGUID();
    for (int i=0; i<mvpGlobalLookUps.size(); i++) {
        if(mvpGlobalLookUps[i])
        {
            GUID lAttrGUID = mvpGlobalLookUps[i]->getGUID();
            if (lUnitGUID == lAttrGUID) {
                return mvpGlobalLookUps[i]->getGlobalToLocalMap(iDataSetGUID);
            }
            
        }
    }
    for (int i = 0; i < mvpGUnitMapList.size(); i++)
    {
        if (mvpGUnitMapList[i]->mID == lUnitGUID)
        {
            DSSGUnitMap* lpUnitMap = mvpGUnitMapList[i];
            return lpUnitMap->GetGlobalToLocalMap(ipUnit);
        }
    }
    
    
    return NULL;
}

IntToIntMap* DSSGUnitContainer::getGlobalToLocalMap2(GUID iDataSetGUID, DSSTabularUnit* ipUnit)
{
    GUID lUnitGUID = ipUnit->getGUID();
    for (int i=0; i<mvpGlobalLookUps.size(); i++) {
        if(mvpGlobalLookUps[i])
        {
            GUID lAttrGUID = mvpGlobalLookUps[i]->getGUID();
            if (lUnitGUID == lAttrGUID) {
                return mvpGlobalLookUps[i]->getGlobalToLocalMap2(iDataSetGUID);
            }
        }
    }
    return NULL;
}

bool DSSGUnitContainer::isDerivedUnit(GUID &lID)
{
    for(int i = 0; i < mvDerivedUnits.size(); ++i)
    {
        if(ATL::InlineIsEqualGUID(mvDerivedUnits[i]->getGUID(), lID))
            return true;
    }
    return false;
}

vector<GUID>* DSSGUnitContainer::getLinkedAttributes(const GUID iGUID)
{
    if(mMapToLinkedGroupID.find(iGUID)!= mMapToLinkedGroupID.end())
    {
        int lGroupID = mMapToLinkedGroupID.find(iGUID)->second;
        
        if(lGroupID >= 0 && lGroupID < mvvAttrLinkedInfo.size())
            return &mvvAttrLinkedInfo[lGroupID];
    }
    
    return NULL;
}

