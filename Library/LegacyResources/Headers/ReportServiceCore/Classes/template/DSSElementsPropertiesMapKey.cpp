//
//  DSSElementsPropertiesMapKey.cpp
//  ReportServiceCore
//
//  Created by Zeng Jinliang on 9/9/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#include "DSSElementsPropertiesMapKey.h"
#import "DSSStrongPtr.h"

DSSElementsPropertiesMapKey::DSSElementsPropertiesMapKey(DSSObjectContext* ipObjectContext) : DSSSecondClassObject(ipObjectContext)
{
}

DSSElementsPropertiesMapKey::~DSSElementsPropertiesMapKey()
{
    Clear();
}

bool DSSElementsPropertiesMapKey::operator==(const DSSElementsPropertiesMapKey& rhs) const
{
    int lCount = Count();
    if (rhs.Count() != lCount)
        return false;
    
    for (int i = 0; i < lCount; i++)
    {
        DSSDropZoneUnit* lpDZUnit = mDZUnits[i];
        // suppose DZUnit is unique, else need reference count
        bool isFound = false;
        for (int j = 0; (j < rhs.Count()) && (!isFound); j++)
        {
            DSSDropZoneUnit* lpRhsDZUnit = rhs.getItem(j);
            if (lpDZUnit == lpRhsDZUnit)
            {
                isFound = true;
                break;
            }
            if ((lpRhsDZUnit && !lpRhsDZUnit) || (!lpRhsDZUnit && lpRhsDZUnit))
                continue;
            
            // it's enough that only comparing the ID and type
            if ((lpDZUnit->getID() == lpRhsDZUnit->getID()) &&(lpDZUnit->getUnitType() == lpRhsDZUnit->getUnitType()))
                isFound = true;
        }
        if (!isFound)
            return false;
    }
    
    return  true;
}

int DSSElementsPropertiesMapKey::LoadDefnFromBinary(DSSBinaryDataReader* ipReader)
{
    int lBlockType;
    ipReader->BeginReadBlock(&lBlockType);
    if (lBlockType != BINARY_BLOCK_ELEMENTS_PROPERTIES_MAP_KEY)
        return S_FALSE;
    
    int lUnitCnt;
    ipReader->ReadInt(&lUnitCnt);
    for (int i = 0; (i < lUnitCnt) && (ipReader->MoreDataInBlock()); i++)
    {
        DSS_DZUnitID lID;
        ipReader->ReadGUID(lID);
        
        int lTmpUnitType;
        ipReader->ReadInt(&lTmpUnitType);
        EnumDSSDropZoneUnitType lUnitType = static_cast<EnumDSSDropZoneUnitType>(lTmpUnitType);
        
        DSSDropZoneUnit* lpUnit = new DSSDropZoneUnit(lID, lUnitType);
        if (!lpUnit) return S_FALSE;
        mDZUnits.push_back(lpUnit);
    }
    
    ipReader->EndReadBlock(); // BINARY_BLOCK_ELEMENTS_PROPERTIES_MAP_KEY
    
    return S_OK;
}

void DSSElementsPropertiesMapKey::Clear()
{
    for (std::vector<DSSDropZoneUnit*>::iterator it = mDZUnits.begin(); it != mDZUnits.end(); it++)
        delete *it;
    mDZUnits.clear();
}

DSSDropZoneUnit* DSSElementsPropertiesMapKey::getItem(int index) const
{
    if (index >= 0 && index < Count())
        return mDZUnits[index];
    return NULL;
}
