//
//  DSSAttributeThresholdItems.cpp
//  ReportServiceCore
//
//  Created by Zeng, Jinliang on 3/22/16.
//  Copyright © 2016 MicroStrategy, Inc. All rights reserved.
//

#include "DSSAttributeThresholdItems.h"
#include "DSSTemplate.h"
#include "DSSRWGridModel.h"
#include "DSSXTabView.h"
#include "DSSAttributeThresholdIterator.h"

int DSSAttributeThresholdItems::init(DSSTemplate *ipTemplate, int iStartRow/* = -1*/, int iNumRows/* = -1*/)
{
    _ASSERTE(ipTemplate != NULL);
    if (!ipTemplate) return E_INVALIDARG;
    
    if (!ipTemplate->hasAttributeThreshold())
        return S_OK;
    
    DSSRWGridModel* lpGridModel = ipTemplate->getGridModel();
    DSSXTabView* lpXTabView = (lpGridModel ? lpGridModel->getXTabView() : NULL);

    return init(lpXTabView, iStartRow, iNumRows);
}

int DSSAttributeThresholdItems::init(DSSXTabView *ipView, int iStartRow/* = -1*/, int iNumRows/* = -1*/)
{
    if (!mItems.empty())
    {
        mItems.clear();
        mIDMap.clear();
    }
   
    if (!ipView)
        return S_OK;
    
    DSSAttributeThresholdIterator* lpAttriThresholdIter = ipView->getAttributeThresholdIterator();
    int hr = lpAttriThresholdIter->Init(iStartRow, iNumRows, VARIANT_FALSE, VARIANT_FALSE);
    CHECK_HR;
    
    // if First() return false, it means there's no threshold at all.
    int isNext = lpAttriThresholdIter->First();
    while (isNext == S_OK)
    {
        EnumDSSAxisName lAxisName = DssAxisReserved;
        int lnOrdinal = 0;
        hr = lpAttriThresholdIter->getRow(&lAxisName, &lnOrdinal);
        CHECK_HR;
        
        DSS_ID lAttrID = GUID_NULL;
        int lnThresholdID = 0;
        hr = lpAttriThresholdIter->getThreshold(&lAttrID, &lnThresholdID);
        CHECK_HR;
        
        int lKey = generateItemsMapKey(lAxisName, lnOrdinal, lAttrID);
        mItems.addItem(lKey, lnThresholdID);
        
        isNext = lpAttriThresholdIter->Next();
    }
    
    return S_OK;
    
} // Init


bool DSSAttributeThresholdItems::getAttributeThresholdItem(EnumDSSAxisName iAxis, int iOrdinal, const DSS_ID& irAttributeID, int& orThresholdID)
{
    size_t lKey = generateItemsMapKey(iAxis, iOrdinal, irAttributeID);
    orThresholdID = mItems.find(lKey);
    return (orThresholdID > -1);
}

bool DSSAttributeThresholdItems::getAttributeThresholdItem(EnumDSSAxisName iAxis, int iOrdinal, int irAttributeIDIndex, int& orThresholdID, int iMinThresholdID)
{
    size_t lKey = generateItemsMapKey(iAxis, iOrdinal, irAttributeIDIndex);
    orThresholdID = mItems.find(lKey, iMinThresholdID);
    return (orThresholdID > -1);
    
}

int DSSAttributeThresholdItems::getMergedAttributeThresholdItem(EnumDSSAxisName iAxis, const std::vector<int>& iOrdinal, const DSS_ID& irAttributeID, int& orThresholdID)
{
    orThresholdID = -1;
    int lThresholdOrdinal = -1;
    int lIDIndex = getAttributeIDIndex(irAttributeID);
    for (std::vector<int>::const_iterator it = iOrdinal.begin(); it != iOrdinal.end(); it++)
    {
        int lThresholdID = -1;
        if (getAttributeThresholdItem(iAxis, *it, lIDIndex, lThresholdID, orThresholdID) && (lThresholdID != orThresholdID))
        {
            orThresholdID = (orThresholdID == -1) ? lThresholdID : std::min(orThresholdID, lThresholdID);
            // if the threshold id changed, set the output ordinal
            if (orThresholdID == lThresholdID)
            {
                lThresholdOrdinal = std::distance(iOrdinal.begin(), it);
            }
            // Optimization: if it is already the first threshold in the thresholds collection, no need to check more
            if (orThresholdID == 1)
                return lThresholdOrdinal;
        }
    }
    
    return lThresholdOrdinal;
}

size_t DSSAttributeThresholdItems::generateItemsMapKey(EnumDSSAxisName iAxis, int iOrdinal, const DSS_ID& irAttributeID)
{
    int lIDIndex = getAttributeIDIndex(irAttributeID);
    return generateItemsMapKey(iAxis, iOrdinal, lIDIndex);
}

int DSSAttributeThresholdItems::getAttributeIDIndex(const DSS_ID& irAttributeID)
{
    const IDMap::key_type& lGUIDKey = irAttributeID;
    IDMap::const_iterator it = mIDMap.find(lGUIDKey);
    int lIDIndex = -1;
    if (it == mIDMap.end())
    {
        lIDIndex = mIDMap.size();
        mIDMap.insert(IDMap::value_type(lGUIDKey, lIDIndex));
    }
    else
    {
        lIDIndex = it->second;
    }
    return lIDIndex;
}

bool DSSAttributeThresholdItems::hasThreshold(const DSS_ID& irAttributeID)
{
    return mIDMap.find(irAttributeID) != mIDMap.end();
}

size_t DSSAttributeThresholdItems::generateItemsMapKey(EnumDSSAxisName iAxis, int iOrdinal, int irAttributeIDIndex)
{
    // I tried to use the std::stringstream to generate string key, it's still performance issue
    // so use hash to generate key. I think there is no more than 1024 ID, 16 axis
    return (size_t)((iOrdinal << 10) + (irAttributeIDIndex << 4) + iAxis);
}

void DSSAttributeThresholdItems::getInfoFromMapKey(size_t iKey, EnumDSSAxisName &oAxis, int &oOrdinal, int &orAttributeIDIndex)
{
    oAxis = static_cast<EnumDSSAxisName>(iKey % 16);
    iKey = (iKey - oAxis) >> 4;
    orAttributeIDIndex = (iKey % 1024);
    iKey = iKey - orAttributeIDIndex;
    oOrdinal = iKey >> 10;
}

void DSSAttributeThresholdItems::RowToAttributeThreshold::addItem(size_t iKey, int iThresholdID)
{
    if (iThresholdID >= 0)
    {
        if (iThresholdID >= mThresholdInfos.size())
        {
            for (int i = mThresholdInfos.size(); i <= iThresholdID; i++)
            {
                RowKeys newKeys;
                mThresholdInfos.push_back(newKeys);
            }
        }
        mThresholdInfos[iThresholdID].push_back(iKey);
        mIsSorted = false;
    }
}

int DSSAttributeThresholdItems::RowToAttributeThreshold::find(size_t iKey, int iMinThresholdID)
{
    if (!mIsSorted)
    {
        sort();
    }
    
    int thresholdID = 0;
    for (std::vector<RowKeys>::const_iterator it = mThresholdInfos.begin(); (it != mThresholdInfos.end()) && ((iMinThresholdID == -1) || thresholdID < iMinThresholdID); it++, thresholdID++)
    {
        RowKeys::const_iterator itRow = std::lower_bound(it->begin(), it->end(), iKey);
        if (itRow != it->end())
        {
            if (*itRow == iKey)
            {
                return thresholdID;
            }
        }
    }
    
    return -1;
}

// it's used for find threshold id for merged cell, but the set::Insert (or sort(vector)) have performance issue
int DSSAttributeThresholdItems::RowToAttributeThreshold::find(const std::set<size_t>& iOrderedKeys, size_t& oKey)
{
    if (!mIsSorted)
    {
        sort();
    }
    
    if (iOrderedKeys.empty())
        return -1;
    
    int smallestKey = *(iOrderedKeys.begin());
    int bigestKey = *(iOrderedKeys.rbegin());
    
    if (smallestKey > bigestKey)
        return -1;
    
    for (std::vector<RowKeys>::iterator it = mThresholdInfos.begin(); it != mThresholdInfos.end(); it++)
    {
        RowKeys::iterator itLowerRow = std::lower_bound(it->begin(), it->end(), smallestKey);
        RowKeys::iterator itUpperRow = std::upper_bound(it->begin(), it->end(), bigestKey);
        if (itLowerRow != it->end())
        {
            std::set<size_t>::const_iterator itKey = iOrderedKeys.begin();
            while ((itKey != iOrderedKeys.end()) && (itLowerRow != itUpperRow))
            {
                oKey = *itKey;
                if (*itLowerRow > oKey)
                {
                    itKey++;
                }
                else if (*itLowerRow < oKey)
                {
                    itLowerRow++;
                }
                else
                {
                    // find the row
                    return std::distance(mThresholdInfos.begin(), it);
                }
            }
        }
    }
    
    return -1;
}

void DSSAttributeThresholdItems::RowToAttributeThreshold::sort()
{
    for (std::vector<RowKeys>::iterator it = mThresholdInfos.begin(); it != mThresholdInfos.end(); it++)
    {
        std::sort(it->begin(), it->end());
    }
    mIsSorted = true;
}

void DSSAttributeThresholdItems::RowToAttributeThreshold::clear()
{
    mThresholdInfos.clear();
}

bool DSSAttributeThresholdItems::RowToAttributeThreshold::empty()
{
    return mThresholdInfos.empty();
}