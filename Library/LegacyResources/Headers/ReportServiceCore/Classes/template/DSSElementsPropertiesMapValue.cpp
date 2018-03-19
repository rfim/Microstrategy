//
//  DSSElementsPropertiesMapValue.cpp
//  ReportServiceCore
//
//  Created by Zeng Jinliang on 9/11/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#include "DSSElementsPropertiesMapValue.h"

DSSElementsPropertiesMapValue::DSSElementsPropertiesMapValue(DSSObjectContext* ipObjectContext) : DSSSecondClassObject(ipObjectContext)
{
}

DSSElementsPropertiesMapValue::~DSSElementsPropertiesMapValue()
{
    Clear();
}

int DSSElementsPropertiesMapValue::LoadDefnFromBinary(DSSBinaryDataReader* ipReader)
{
    int lBlockType;
    ipReader->BeginReadBlock(&lBlockType);
    if (lBlockType != BINARY_BLOCK_ELEMENTS_PROPERTIES_MAP_VALUE)
        return S_FALSE;
    
    int lCount;
    ipReader->ReadInt(&lCount);
    for (int i = 0; (i < lCount) && (ipReader->MoreDataInBlock()); i++)
    {
        ipReader->BeginReadBlock(&lBlockType);
        if (lBlockType != BINARY_BLOCK_ELEMENTS)
            return E_DEFNSTREAM;
        DSSBaseElementsProxy* lpElements = new DSSBaseElementsProxy(mpObjectContext);
        if (!lpElements)
            return E_OUT_OF_MEMORY;
        
        int hr = lpElements->LoadDefnFromBinary(ipReader);
        if (hr != S_OK)
            return hr;
        
        ipReader->EndReadBlock(); // BINARY_BLOCK_ELEMENTS
        
        std::string lValue;
        ipReader->ReadString(lValue);
        if (!Add(lpElements, lValue))
            return S_FALSE;
    }
    
    ipReader->EndReadBlock(); // BINARY_BLOCK_ELEMENTS_PROPERTIES_MAP_VALUE
    
    return S_OK;
}

void DSSElementsPropertiesMapValue::Clear()
{
    for (ElementsPropertiesValueMapType::iterator it = mElementsPropertiesValueMap.begin(); it != mElementsPropertiesValueMap.end(); it++)
        delete it->first;
    mElementsPropertiesValueMap.clear();
}

bool DSSElementsPropertiesMapValue::Add(DSSBaseElementsProxy* ipElements, const std::string& iValue)
{
    if (!ipElements)
        return false;

    ElementsPropertiesValueMapType::iterator it = mElementsPropertiesValueMap.find(ipElements);
    if (it != mElementsPropertiesValueMap.end())
        Delete(ipElements);
    mElementsPropertiesValueMap[ipElements] = iValue;
    return true;
}

bool DSSElementsPropertiesMapValue::Delete(DSSBaseElementsProxy *ipElements)
{
    if (!ipElements)
        return false;
    for (ElementsPropertiesValueMapType::iterator it = mElementsPropertiesValueMap.begin(); it != mElementsPropertiesValueMap.end(); it++)
    {
        if (it->first != ipElements)
            continue;
        
        delete it->first;
        mElementsPropertiesValueMap.erase(it);
        return true;
    }
    
    return false;
}
