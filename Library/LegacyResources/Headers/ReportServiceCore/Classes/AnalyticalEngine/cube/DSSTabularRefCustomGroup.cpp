//
//  DSSTabularRefCustomGroup.cpp
//  ReportServiceCore
//
//  Created by Tang, Cheng on 11/23/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "DSSTabularRefCustomGroup.h"
#include "DSSNullElementProxy.h"

DSSTabularRefCustomGroup::DSSTabularRefCustomGroup() : mpTabularCustomGroup(NULL), mpGlobalToLocalMap(NULL), mpLocalToGlobalMap(NULL), mGlobalRowCount(-1)
{
    
}

DSSTabularRefCustomGroup::~DSSTabularRefCustomGroup()
{
    
}

int DSSTabularRefCustomGroup::Init(DSSTabularCustomGroup* ipUnit)
{
    mpTabularCustomGroup = ipUnit;
    return S_OK;
}

int DSSTabularRefCustomGroup::SetGlobalRowMap(int iGlobalRowCount, int* ipGlobalToLocalMap, int* ipLocalToGlobalMap)
{
    mGlobalRowCount = iGlobalRowCount;
    mpGlobalToLocalMap = ipGlobalToLocalMap;
    mpLocalToGlobalMap = ipLocalToGlobalMap;
    
    return S_OK;
}

int DSSTabularRefCustomGroup::GetGlobalRowMap(int* opGlobalRowCount, int** oppGlobalToLocalMap, int** oppLocalToGlobalMap)
{
    *opGlobalRowCount = mGlobalRowCount;
    *oppGlobalToLocalMap = mpGlobalToLocalMap;
    *oppLocalToGlobalMap = mpLocalToGlobalMap;
    return S_OK;
}

int DSSTabularRefCustomGroup::TranslateLocalToGlobal(Int32 iLocal)
{
        return mGlobalRowCount != -1 ? mpLocalToGlobalMap[iLocal] : iLocal;
}
int DSSTabularRefCustomGroup::TranslateGlobalToLocal(Int32 iGlobal)
{
    if (mGlobalRowCount != -1)
    {
        return iGlobal >= 0 && iGlobal < mGlobalRowCount ? mpGlobalToLocalMap[iGlobal] : -1;
    }
    return iGlobal;
}

int DSSTabularRefCustomGroup::setElement(int index, XTabCustomGroupElement &irElement)
{
    assert(false);
    return E_NOTIMPL;
}

XTabCustomGroupElement * DSSTabularRefCustomGroup::getElement(int index)
{
    int lKey = TranslateGlobalToLocal(index);
    if (lKey < 0)
    {
        //may be a join null
        //assert(false);
        return NULL;
    }
    return mpTabularCustomGroup->getElement(lKey);
}

int DSSTabularRefCustomGroup::getOrdinal(int index)
{
    if (index < 0 || index >= mGlobalRowCount)
        return -2;
    return mvOrdinal[index];
}

int DSSTabularRefCustomGroup::SetOrdinal(vector<int>& ivOrdinal)
{
    if (ivOrdinal.size() != mGlobalRowCount)
        return E_FAIL;
    mvOrdinal.assign(ivOrdinal.begin(), ivOrdinal.end());
    return S_OK;
}

int DSSTabularRefCustomGroup::getMaxDepth()
{
    return mpTabularCustomGroup->getMaxDepth();
}

int DSSTabularRefCustomGroup::getParentKeys(int index, vector<int> &orParentKeys)
{
    int lKey = TranslateGlobalToLocal(index);
    if (lKey < 0)
    {
        assert(false);
        return E_FAIL;
    }
    return mpTabularCustomGroup->getParentKeys(lKey, orParentKeys);
}

int DSSTabularRefCustomGroup::getText(int index, MBase::CString &orDisplayName)
{
    
    int lKey = TranslateGlobalToLocal(index);
    if (lKey < 0)
    {
        assert(false);
        return E_FAIL;
    }
    return mpTabularCustomGroup->getText(lKey, orDisplayName);
}

const char * DSSTabularRefCustomGroup::getTextAndLength(int index, int &orLength)
{
    int lKey = TranslateGlobalToLocal(index);
    if (lKey < 0)
    {
        //may be a join null
        //assert(false);
        return NULL;
    }
    return mpTabularCustomGroup->getTextAndLength(lKey, orLength);
}

int DSSTabularRefCustomGroup::CountRows()
{
    if (mGlobalRowCount != -1)
        return mGlobalRowCount;
    return mpTabularCustomGroup->CountRows();
}

int DSSTabularRefCustomGroup::FindElement(DSSBaseElementProxy *ipElement, int &orKey)
{
    int lLocalKey;
    int hr = mpTabularCustomGroup->FindElement(ipElement, lLocalKey);
    CHECK_HR;
    
    orKey = TranslateLocalToGlobal(lLocalKey);
    return S_OK;
}

int DSSTabularRefCustomGroup::createElement(int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement, int iGlobalKey,int iGlobalKeyVer)
{
//    assert(false);
//    return E_NOTIMPL;
    // # 551957 tehe we need implementing  this method
   // int lLocalKey = TranslateLocalToGlobal(key);
    //zhyang, 559031, we need to translate global to local to get the correct CG.
    //int lLocalKey = TranslateGlobalToLocal(key);
    //zhyang, 566480, deal with subtotal
    int lKey = key;
    if(lKey >= 0)
    {
        lKey = TranslateGlobalToLocal(key);
        //zhyang, uncomment it to check whether the translation it right or wrong.
        // # 551959 2/14/2012 tehe user maybe click the "total",so key value may be less than 0
        if (lKey < 0)
        {
            // #773357, tehe: in order to avoid crash when rendering grid, here we will create null element for not exists element
            GUID lUnitID = mpTabularCustomGroup->getGUID();
            DSSNullElementProxy *lpNullElement = new DSSNullElementProxy(ipObjectContext, lUnitID);
            *oppElement = lpNullElement;
            return S_OK;
        }
    }

    return mpTabularCustomGroup->createElement(lKey, ipObjectContext, oppElement, iGlobalKey, iGlobalKeyVer);
}

DSSBaseElementProxy * DSSTabularRefCustomGroup::getCustomGroupElement(int index)
{
    int lKey = TranslateGlobalToLocal(index);
    if (lKey < 0)
    {
//        assert(false);
        return NULL;    //join null
    }
    return mpTabularCustomGroup->getCustomGroupElement(lKey);
}

int DSSTabularRefCustomGroup::size()
{
    return mpTabularCustomGroup->size();
}

int DSSTabularRefCustomGroup::getGUID(GUID &irVal)
{
    return mpTabularCustomGroup->getGUID(irVal);
}

const GUID &DSSTabularRefCustomGroup::getGUID()
{
    return mpTabularCustomGroup->getGUID();
}

bool DSSTabularRefCustomGroup::IsRuntimeUnit()
{
    //return mpTabularCustomGroup->IsRuntimeUnit();
    //TQMS429095
    //hyan,2012/08/28,we need to return mbIsRuntimeUnit
    return mbIsRuntimeUnit;
}

int DSSTabularRefCustomGroup::getParentKey(int childKey, int *parent)
{
    XTabCustomGroupElement * lpElement = getElement(childKey);
    if (lpElement)
        *parent = lpElement->parent;
    
	if (mpLocalToGlobalMap && /*ctang handle for root*/parent && (*parent) >= 0 && (*parent) < mGlobalRowCount && lpElement)
		*parent = mpLocalToGlobalMap[*parent];
    else
        *parent = 0;
    
	return S_OK;
}

int DSSTabularRefCustomGroup::getTabularUnit(DSSTabularUnit** oppUnit)
{    
    return mpTabularCustomGroup->getTabularUnit(oppUnit);
}
