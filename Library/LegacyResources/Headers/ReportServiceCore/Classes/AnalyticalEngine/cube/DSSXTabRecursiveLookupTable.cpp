//
//  DSSXTabRecursiveLookupTable.cpp
//  ReportServiceCore
//
//  Created by Sun, Mingmin on 3/16/16.
//  Copyright © 2016 MicroStrategy, Inc. All rights reserved.
//

#include "StdAfx.h"
#include "DSSXTabRecursiveLookupTable.h"
#include "DSSBinaryDataReader.h"
#include "DSSTabularData.h"
#include "DSSTabularAttribute.h"

DSSXTabRecursiveLookupTable::DSSXTabRecursiveLookupTable()
{
    //mUnitType = DssTemplateRecursiveAttribute;
    mUnitType = DssTemplateAttribute;
}

int DSSXTabRecursiveLookupTable::getArity()
{
    DSSTabularUnit* lDataUnit = getChildUnit();
    if (lDataUnit && lDataUnit->getUnitType() == DssTemplateAttribute) {
        DSSTabularAttribute* lDataAttr = (DSSTabularAttribute*)lDataUnit;
        return lDataAttr->getArity();
    }
    return 0;
}


int DSSXTabRecursiveLookupTable::Count()
{
    DSSTabularUnit* lDataUnit = getChildUnit();
    if (lDataUnit && lDataUnit->getUnitType() == DssTemplateAttribute){
        return ((DSSTabularAttribute*)lDataUnit) -> Count();
    }
    return 0;
}

int DSSXTabRecursiveLookupTable::CountRows()
{
    DSSTabularUnit* lDataUnit = getChildUnit();
    if (lDataUnit && lDataUnit->getUnitType() == DssTemplateAttribute){
        return ((DSSTabularAttribute*)lDataUnit) -> CountRows();
    }
    return 0;
}

int DSSXTabRecursiveLookupTable::Item(int iForm, DSSDataColumn **oppColumn)
{
    DSSTabularUnit* lDataUnit = getChildUnit();
    if (lDataUnit && lDataUnit->getUnitType() == DssTemplateAttribute){
        return ((DSSTabularAttribute*)lDataUnit) -> Item(iForm, oppColumn);
    }
    
    return S_FALSE;
}

int DSSXTabRecursiveLookupTable::getWHNullPosition()
{
    DSSTabularUnit* lDataUnit = getChildUnit();
    if (lDataUnit && lDataUnit->getUnitType() == DssTemplateAttribute){
        return ((DSSTabularAttribute*)lDataUnit) -> getWHNullPosition();
    }
    
    return -1;
}
void DSSXTabRecursiveLookupTable::setWHNullPosition(int iPosition)
{
    DSSTabularUnit* lDataUnit = getChildUnit();
    if (lDataUnit && lDataUnit->getUnitType() == DssTemplateAttribute){
        return ((DSSTabularAttribute*)lDataUnit) -> setWHNullPosition(iPosition);
    }
}

int DSSXTabRecursiveLookupTable::getBaseFormID(int iForm)
{
    DSSTabularUnit* lDataUnit = getChildUnit();
    if (lDataUnit && lDataUnit->getUnitType() == DssTemplateAttribute){
        return ((DSSTabularAttribute*)lDataUnit) -> getBaseFormID(iForm);
    }
    return -1;
}

//to support transaction data
int DSSXTabRecursiveLookupTable::setData(int iBaseFormID, int iRow, char *ipData, int iSizeInByte)
{
    DSSTabularUnit* lDataUnit = getChildUnit();
    if (lDataUnit && lDataUnit->getUnitType() == DssTemplateAttribute){
        return ((DSSTabularAttribute*)lDataUnit) -> setData(iBaseFormID,iRow,ipData,iSizeInByte);
    }
    return S_OK;
}

int DSSXTabRecursiveLookupTable::setData(int iBaseFormID, int iOffset, MBase::CString &irText)
{
    DSSTabularUnit* lDataUnit = getChildUnit();
    if (lDataUnit && lDataUnit->getUnitType() == DssTemplateAttribute){
        return ((DSSTabularAttribute*)lDataUnit) -> setData(iBaseFormID,iOffset,irText);
    }
    return S_OK;
}

int DSSXTabRecursiveLookupTable::FindFormWithLocale(int iAttribute, int iBaseFormID, int iLocale, int *opColumn)
{
    DSSTabularUnit* lDataUnit = getChildUnit();
    if (lDataUnit && lDataUnit->getUnitType() == DssTemplateAttribute){
        return ((DSSTabularAttribute*)lDataUnit) -> FindFormWithLocale(iAttribute, iBaseFormID, iLocale, opColumn);
    }
    return S_FALSE;
}

int DSSXTabRecursiveLookupTable::FindFormWithLocale(int iAttribute, int iBaseFormID, int iLocale, int &orColumn)
{
    DSSTabularUnit* lDataUnit = getChildUnit();
    if (lDataUnit && lDataUnit->getUnitType() == DssTemplateAttribute){
        return ((DSSTabularAttribute*)lDataUnit) -> FindFormWithLocale(iAttribute, iBaseFormID, iLocale, orColumn);
    }
    return S_FALSE;
}

int	DSSXTabRecursiveLookupTable::createElement(int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement, int iGlobalKey, int iGlobalKeyVer)
{
    int lChildKey = -1;
    int hr = S_OK;
    hr = TransferDisplay2DataKey(key, lChildKey);
    CHECK_HR;
    
    DSSTabularUnit* lDataUnit = getChildUnit();
    if (lDataUnit) {
        return lDataUnit->createElement(lChildKey, ipObjectContext, oppElement, iGlobalKey, iGlobalKeyVer);
    }
    return hr;
}
