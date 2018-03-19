//
//  DSSXTabRefLookupTable.cpp
//  ReportServiceCore
//
//  Created by Xiaoqi Zeng on 2/23/12.
//  Copyright 2012 MicroStrategy. All rights reserved.
//

#include "DSSXTabRefLookupTable.h"
#import "DSSGlobalLookUpTable.h"

DSSXTabRefLookupTable::DSSXTabRefLookupTable() : mnForms(-1), mpForms(NULL)
{
    mpTabularAttribute = NULL;
    mbCanUseSSO = true;
}

DSSXTabRefLookupTable::~DSSXTabRefLookupTable()
{
    if (mpForms)
        delete [] mpForms;
}

int DSSXTabRefLookupTable::Init(DSSTabularAttribute* ipTabularAttribute)
{
    mpTabularAttribute = ipTabularAttribute;
    return S_OK;
}


int DSSXTabRefLookupTable::SetSelectedForms(bool ifFull, vector<int>& ivrForms, vector<int>& ivrLocale)
{
    if (ifFull)
        mnForms = -1;
    else
    {
        mnForms = ivrForms.size();
        mpForms = new int[mnForms];
        for (int i = 0; i < mnForms; i++)
        {
            int hr = mpTabularAttribute->FindFormWithLocale(0, ivrForms[i], ivrLocale[i], mpForms[i]);
            CHECK_HR;
        }
    }
    
    return S_OK;
}
//int DSSXTabRefLookupTable::getColumnForms(std::vector<int>& ovColumnForms)
//{
//    return E_NOTIMPL;
//}
//
//int DSSXTabRefLookupTable::getColumnLocales(std::vector<int>& ovColumnLocals)
//{
//    return E_NOTIMPL;
//}

int DSSXTabRefLookupTable::getArity(int* opArity)
{
    if (!opArity) return E_INVALIDARG;
    *opArity = mpTabularAttribute->getArity();
    return S_OK;
}

int DSSXTabRefLookupTable::setArity(int iArity)
{
    return E_NOTIMPL;
}

int DSSXTabRefLookupTable::populate(DSSTabularAttribute originalUnit, int iUnit, int rowLimit)
{
    return E_NOTIMPL;
}


int DSSXTabRefLookupTable::findFormWithLocale(int iAttribute, int iForm, int iLocale, int &orColumn)
{
    int lColumn;
    int hr = mpTabularAttribute->FindFormWithLocale(iAttribute, iForm, iLocale, lColumn);
    CHECK_HR;
    if (mnForms != -1)
    {
        for (int i = 0; i < mnForms; i++)
        {
            if (mpForms[i] == lColumn)
            {
                orColumn = lColumn;
                return S_OK;
            }
        }
        orColumn = -1;
        return S_FALSE;
    }
    orColumn = lColumn;
    return S_OK;
}


int DSSXTabRefLookupTable::getUnitType(EnumDSSTemplateUnitType* oUnitType)
{
    return mpTabularAttribute->getUnitType();
}

int DSSXTabRefLookupTable::getGUID(GUID &opGuid)
{
    //if (!opGuid) return E_INVALIDARG;
    int hr = mpTabularAttribute->getGUID(opGuid);
    CHECK_HR;
    return S_OK;
}

const GUID &DSSXTabRefLookupTable::getGUID()
{
    return mpTabularAttribute->getGUID();
}

int DSSXTabRefLookupTable::setGuid(GUID iGuid)
{
    return E_NOTIMPL;
}

int DSSXTabRefLookupTable::FindElement(DSSBaseElementProxy *ipElement, int &orKey)
{
    return mpTabularAttribute->FindElement(ipElement, orKey);
}

int DSSXTabRefLookupTable::FindElement(EnumDSSDataType * ipTypes, Int32 * ipcbDatas, unsigned char ** ippDatas, DSSDataFlag * ipFlags, int &orKey)
{
    return mpTabularAttribute->FindElement(ipTypes, ipcbDatas, ippDatas, ipFlags, orKey);
}

int	DSSXTabRefLookupTable::createElement(int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement, int iGlobalKey, int iGlobalKeyVer)
{
    return mpTabularAttribute->createElement(key, ipObjectContext, oppElement,iGlobalKey, iGlobalKeyVer);
}


int DSSXTabRefLookupTable::CountRows()
{
    return mpTabularAttribute->CountRows();
}

int DSSXTabRefLookupTable::getElementCount()
{
    DSSGlobalLookUpTable *lpGlobalLookUpTable=dynamic_cast<DSSGlobalLookUpTable*>(mpTabularAttribute);
    
    if(lpGlobalLookUpTable)
        return lpGlobalLookUpTable->getElementCount();
    else
        return mpTabularAttribute->CountRows();
}

int DSSXTabRefLookupTable::FindFormWithLocale(int iAttribute, int iBaseFormID, int iLocale, int &orColumn)
{
    int lColumn;
    int hr = mpTabularAttribute->FindFormWithLocale(iAttribute, iBaseFormID, iLocale, lColumn);
    CHECK_HR;
    if (mnForms != -1)
    {
        for (int i = 0; i < mnForms; i++)
        {
            if (mpForms[i] == lColumn)
            {
                orColumn = lColumn;
                return S_OK;
            }
        }
        orColumn = -1;
        return S_FALSE;
    }
    orColumn = lColumn;
    return S_OK;
}

int DSSXTabRefLookupTable::Item(int iForm, DSSDataColumn **oppColumn)
{
    return mpTabularAttribute->Item(iForm, oppColumn);
}

int DSSXTabRefLookupTable::getArity()
{
    return mpTabularAttribute->getArity();
}

int DSSXTabRefLookupTable::AddFormDataColumn(int iBaseFormID, int iBaseFormType, DSSDataColumn *ipColumn)
{
    return mpTabularAttribute->AddFormDataColumn(iBaseFormID, iBaseFormType, ipColumn);
}

int DSSXTabRefLookupTable::Count()
{
    return mpTabularAttribute->Count();
}

int DSSXTabRefLookupTable::getBaseFormID(int iForm)
{
    return mpTabularAttribute->getBaseFormID(iForm);
}

EnumDSSBaseFormType DSSXTabRefLookupTable::getBaseFormType(int iForm)
{
    return mpTabularAttribute->getBaseFormType(iForm);
}

int DSSXTabRefLookupTable::size()
{
    return mpTabularAttribute->size();
}
//to support transaction data
int DSSXTabRefLookupTable::setData(int iBaseFormID, int iRow, char *ipData, int iSizeInByte)
{
    return mpTabularAttribute->setData(iBaseFormID, iRow, ipData, iSizeInByte);
}

int DSSXTabRefLookupTable::setData(int iBaseFormID, int iOffset, MBase::CString &irText)
{
    return mpTabularAttribute->setData(iBaseFormID, iOffset, irText);
}

int DSSXTabRefLookupTable::enableSSO()
{
    mbCanUseSSO = true;
    
    return S_OK;
}

int DSSXTabRefLookupTable::disableSSO()
{
    mbCanUseSSO = false;
    
    return S_OK;
}

bool DSSXTabRefLookupTable::canUesSSO()
{
    return mbCanUseSSO;
}

int DSSXTabRefLookupTable::getTabularUnit(DSSTabularUnit** oppUnit)
{
    return mpTabularAttribute->getTabularUnit(oppUnit);
}

