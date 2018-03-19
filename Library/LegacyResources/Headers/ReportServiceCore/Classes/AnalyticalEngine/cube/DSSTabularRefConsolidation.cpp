//
//  DSSTabularRefConsolidation.cpp
//  ReportServiceCore
//
//  Created by Tang, Cheng on 11/23/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "DSSTabularRefConsolidation.h"

DSSTabularRefConsolidation::DSSTabularRefConsolidation() : mpTabularConsolidation(NULL), mGlobalRowCount(-1),
                            mpGlobalToLocalMap(NULL), mpLocalToGlobalMap(NULL), mpGToLIntMap(NULL),mJoinNull(-1), mMaxOrdinal(-1)
{
    
}

int DSSTabularRefConsolidation::Init(DSSTabularConsolidation* ipUnit)
{
    mpTabularConsolidation = ipUnit;
    
    // TQMS 785587, tehe
    if(mpTabularConsolidation)
    {
        int lnRows = mpTabularConsolidation->CountRows();
        for (int iRow = 0; iRow<lnRows; iRow++)
        {
            XTabConsolidationElement*  lpElement  = mpTabularConsolidation->getElement(iRow);
            if(lpElement && lpElement->ordinal > mMaxOrdinal)
                mMaxOrdinal = lpElement->ordinal;
        }
    }
    return S_OK;
}

int DSSTabularRefConsolidation::TranslateLocalToGlobal(Int32 iLocal)
{
    return mGlobalRowCount != -1 ? ( iLocal >= 0 ? mpLocalToGlobalMap[iLocal] : iLocal) : iLocal;
}

int DSSTabularRefConsolidation::TranslateGlobalToLocal(Int32 iGlobal)
{
    Int32 lLocal = iGlobal;
	if (mGlobalRowCount != -1)
	{
		if (mpGToLIntMap)
		{
			IntToIntMapType::iterator it = mpGToLIntMap->find(iGlobal);
			if (it == mpGToLIntMap->end())
			{
                if(iGlobal == mJoinNull)
                    lLocal = -2;
                else
                    lLocal = -1;
			}
			else
				lLocal = it->second;
		}
        // TQMS 785587, tehe
		else if(iGlobal < mGlobalRowCount && iGlobal != mJoinNull)
			lLocal = iGlobal >= 0 ? mpGlobalToLocalMap[iGlobal] : iGlobal;
        else if(iGlobal == mJoinNull)
            lLocal = iGlobal < mGlobalRowCount && iGlobal >=0 && mpGlobalToLocalMap[iGlobal] >= 0 ? mpGlobalToLocalMap[iGlobal] : -2;
        else
            lLocal = -1;
	}
	return lLocal;
}
int DSSTabularRefConsolidation::GetMaxKey()
{
    if (mGlobalRowCount != -1)
    {
        int max = -1;
        if (mpGlobalToLocalMap)
        {
            // 7/10/2012 tehe: I think here we should return max key of global keys 
//            for (int i = 0; i < mGlobalRowCount; i++)
//                if (mpGlobalToLocalMap[i] > max)
//                    max = mpGlobalToLocalMap[i];
            max = mGlobalRowCount -1 ;
        }
        else if (mpGToLIntMap)
        {
            IntToIntMapType::iterator lIt = mpGToLIntMap->begin();
            while (lIt != mpGToLIntMap->end())
            {
                if (max < lIt->first)
                    max = lIt->first;
                lIt++;
            }
        }
        return max+1;          // 7/10/2012 tehe: we should return max +1 
    }
    return mpTabularConsolidation->GetMaxKey();
	
}

int DSSTabularRefConsolidation::getNumAllElements()
{
    return mpTabularConsolidation->getNumAllElements();
}

int DSSTabularRefConsolidation::SetGlobalRowMap(int iGlobalRowCount, int* ipGlobalToLocalMap, IntToIntMapType* ipGlobalToLocalMap2, int* ipLocalToGlobalMap)
{
    mGlobalRowCount = iGlobalRowCount;
    mpGlobalToLocalMap = ipGlobalToLocalMap;
    mpGToLIntMap = ipGlobalToLocalMap2;
    mpLocalToGlobalMap = ipLocalToGlobalMap;
    return S_OK;
}

int DSSTabularRefConsolidation::GetGlobalRowMap(int* opGlobalRowCount, int** oppGlobalToLocalMap, IntToIntMapType** oppGlobalToLocalMap2, int** oppLocalToGlobalMap)
{
    *opGlobalRowCount = mGlobalRowCount;
    *oppGlobalToLocalMap = mpGlobalToLocalMap;
    *oppGlobalToLocalMap2 = mpGToLIntMap;
    *oppLocalToGlobalMap = mpLocalToGlobalMap;
    return S_OK;
}


bool DSSTabularRefConsolidation::IsDerivedElement()
{
    return mpTabularConsolidation->IsDerivedElement();
}

// weilin 2016/08/18 DE41482 check nde
bool DSSTabularRefConsolidation::IsNewDerivedElement()
{
    return mpTabularConsolidation->IsNewDerivedElement();
}

int DSSTabularRefConsolidation::setElement(int index, XTabConsolidationElement &irElement)
{
    assert(false);
    return E_NOTIMPL;
}

int DSSTabularRefConsolidation::setIsDerivedElement(bool isDE, DSSTabularUnit *ipChildUnit, bool ibUseGlobal)
{
    assert(false);
    return E_NOTIMPL;
}

XTabConsolidationElement* DSSTabularRefConsolidation::getElement(int index)
{
    int lKey = TranslateGlobalToLocal(index);
    if (lKey < 0)
    {
        return NULL;
    }
    return mpTabularConsolidation->getElement(lKey);
}

int DSSTabularRefConsolidation::getText(int index, MBase::CString &orDisplayName)
{
    int lKey = TranslateGlobalToLocal(index);
    if (lKey < 0)
    {
        assert(false);
        return E_FAIL;
    }
    return mpTabularConsolidation->getText(lKey, orDisplayName);
}

const char * DSSTabularRefConsolidation::getTextAndLength(int index, int &orLength)
{
    int lKey = TranslateGlobalToLocal(index);
    if (lKey < 0)
    {
        // assert(false); // TQMS 785587, tehe, DE join null
        return NULL;
    }
    return mpTabularConsolidation->getTextAndLength(lKey, orLength);
}

int DSSTabularRefConsolidation::CountRows()
{
    if (mGlobalRowCount != -1)
    {
        return mGlobalRowCount;
     //   return GetMaxKey(mpTabularConsolidation->CountRows());
    }
    return mpTabularConsolidation->CountRows();
}

int DSSTabularRefConsolidation::FindElement(DSSBaseElementProxy *ipElement, int &orKey)
{
    int llocalKey;
    int hr = mpTabularConsolidation->FindElement(ipElement, llocalKey);
    CHECK_HR;
    
    orKey = TranslateLocalToGlobal(llocalKey);
    return S_OK;
}

int DSSTabularRefConsolidation::createElement(int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement, int iGlobalKey, int iGlobalKeyVer)
{
    //zhyang, if the key is negtive, it must be a subtotal. no need to translate it to local.
    int lKey = key;
    if(lKey >= 0)
    {
        lKey = TranslateGlobalToLocal(key);
        //zhyang, uncomment it to check whether the translation it right or wrong.
    // # 551959 2/14/2012 tehe user maybe click the "total",so key value may be less than 0
        if (lKey < 0)
        {
            assert(false);
            return E_FAIL;
        }
    }
        
    return mpTabularConsolidation->createElement(lKey, ipObjectContext, oppElement, iGlobalKey,iGlobalKeyVer);
}

bool DSSTabularRefConsolidation::IsLeaf(int index)
{
    int lKey = TranslateGlobalToLocal(index);
    if (lKey < 0)
    {
        // assert(false); TQMS 785587, tehe, join null of DE
        return false;
    }
    return mpTabularConsolidation->IsLeaf(lKey);
}

bool DSSTabularRefConsolidation::IsDisplayed(int index)
{
    int lKey = TranslateGlobalToLocal(index);
    
    // TQMS 785587, tehe
    if (lKey == -2)
    {
        if(mMaxOrdinal + 1 < 0)
            return false;
        else
            return true;
    }
    else if (lKey < 0)
    {
        // 7/10/2012 tehe: sometimes we will call getMaxKey to get the max global key of RefConsolidation, then we will call IsDisplayed method to check whether the key "index" which value is between[0, max] can display. If the RefConsolidation is a DE, then the global key may not be continuous, and we may not find a local key by the global key "index", because the global key "index" itself may not exist. so here if the lKey value is less than 0 (which may be caused by the global key "index" doesn't exist), we will return false, else the App will crash.
//        assert(false);
//        return E_FAIL;
        return false;
    }
    return mpTabularConsolidation->IsDisplayed(lKey);
}

DSSTabularUnit * DSSTabularRefConsolidation::getChildUnit()
{
    return mpTabularConsolidation->getChildUnit();
}

int DSSTabularRefConsolidation::getChildElementKey(int index)
{
    int lKey = TranslateGlobalToLocal(index);
    if (lKey < 0)
    {
        assert(false);
        return -1;
    }
    return mpTabularConsolidation->getChildElementKey(lKey);
}

DSSConsolidationElement* DSSTabularRefConsolidation::getConsolidationElement(int index)
{
    int lKey = TranslateGlobalToLocal(index);
    if (lKey < 0)
    {
        // assert(false); // TQMS 785587, tehe, DE join null
        return NULL;
    }
    return mpTabularConsolidation->getConsolidationElement(lKey);
}

int DSSTabularRefConsolidation::size()
{
    return mpTabularConsolidation->size();
}

int DSSTabularRefConsolidation::getChildElementID(int index, GUID* lpID)
{
    int lKey = TranslateGlobalToLocal(index);
    if (lKey < 0)
    {
        *lpID = GUID_NULL;
        return S_FALSE;
    }
    return mpTabularConsolidation->getChildElementID(lKey, lpID);
}

int DSSTabularRefConsolidation::getGUID(GUID &irVal)
{
    return mpTabularConsolidation->getGUID(irVal);
}

const GUID &DSSTabularRefConsolidation::getGUID()
{
    return mpTabularConsolidation->getGUID();
}

bool DSSTabularRefConsolidation::IsRuntimeUnit()
{
    //return mpTabularConsolidation->IsRuntimeUnit();
    
    //TQMS429095
    //hyan,2012/08/28,we need to return mbIsRuntimeUnit
    return mbIsRuntimeUnit;
}

bool DSSTabularRefConsolidation::hasElementDisplayable()
{
    return mpTabularConsolidation->hasElementDisplayable();
}

int DSSTabularRefConsolidation::getTabularUnit(DSSTabularUnit** oppUnit)
{
    return mpTabularConsolidation->getTabularUnit(oppUnit);
}
IntToIntMapType* DSSTabularRefConsolidation::getGToLIntMap()
{
    return mpGToLIntMap;
}
/*
int DSSTabularRefConsolidation::getUnitIndex()
{
    return mpTabularConsolidation->getUnitIndex();
}
*/