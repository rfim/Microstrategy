//
//  DSSRecursiveRelation.cpp
//  ReportServiceCore
//
//  Created by Sun, Mingmin on 3/17/16.
//  Copyright © 2016 MicroStrategy, Inc. All rights reserved.
//

//==============================================================================================
//	FILENAME	:	CDSSXTabRecursiveRelation.cpp
//	DESCRIPTION	:	CDSSXTabRecursiveRelation class.
//	AUTHOR		:	Liu, Dengzhi
//	CREATION	:	2013-04-04
//	HISTORY		:
//==============================================================================================

#include "StdAfx.h"
#include "DSSRecursiveRelation.h"
#define CHECK_HR if(hr!=S_OK) return hr;

Int32 DSSRecursivePCTable::get_RowCount()
{
    return mnRows;
}

Int32* DSSRecursivePCTable::get_Children()
{
    return mpIndex;
}

Int32* DSSRecursivePCTable::get_Parents()
{
    return mpParents;
}

short* DSSRecursivePCTable::get_Levels()
{
    return mpLevels;
}

bool* DSSRecursivePCTable::get_ChildrenFlags()
{
    return mpHasChildren;
}

bool* DSSRecursivePCTable::get_HiddenFlags()
{
    return mpIsHidden;
}

HRESULT  DSSRecursivePCTable::SetChildren(Int32 iStart, Int32 iEnd, Int32* ipChildren, Int32 iRowCount)
{
    HRESULT hr = S_OK;
    if(iStart > iEnd || iEnd >= mnRows || iStart < 0)
        return hr = DSSXTAB_E_INVALIDINDEX;
    //AE_MSG xhan
    
    memmove(mpIndex + iStart, ipChildren, sizeof(Int32) *iRowCount);
    
    return S_OK;
}

HRESULT  DSSRecursivePCTable::SetParents(Int32 iStart, Int32 iEnd, Int32* ipParents, Int32 iRowCount)
{
    HRESULT hr = S_OK;
    if(iStart > iEnd || iEnd >= mnRows || iStart < 0)
        return hr = DSSXTAB_E_INVALIDINDEX;
    //AE_MSG xhan
    
    memmove(mpParents + iStart, ipParents, sizeof(Int32) *iRowCount);
    
    return S_OK;
}

HRESULT  DSSRecursivePCTable::SetRelationData(Int32 iStart, Int32 iEnd, BYTE *pData,
                                                                Int32 iRowCount, DSSDataType_Type iTargetType, Int32 iRelationType)
{
    HRESULT hr = S_OK;
    if(iStart > iEnd || iEnd >= mnRows || iStart < 0)
        return hr = DSSXTAB_E_INVALIDINDEX;
    //AE_MSG xhan
    
    Int32 lData = -1;
    for(Int32 i = 0; i < iRowCount; i++)
    {
        switch(iTargetType)
        {
            case DssDataTypeBool:
                lData = ((bool*)pData)[i];
                break;
                
            case DssDataTypeShort:
                lData = ((short*)pData)[i];
                break;
                
            case DssDataTypeInteger:
            case DssDataTypeUnsigned:
            case DssDataTypeLong:
                lData = ((Int32*)pData)[i];
                break;
                
            case DssDataTypeDouble:
            case DssDataTypeNumeric:
            case DssDataTypeDecimal:
                lData= ((double*)pData)[i];
                break;
                
            case DssDataTypeInt64:
                lData = ((Int64*)pData)[i];
                break;
                
            default:
                break;
        }
        
        if (iRelationType == DssRelationLevel)
            mpLevels[i + iStart]  = lData;
        else if (iRelationType == DssRelationHasChild)
            mpHasChildren[i + iStart]  = lData;
        else if (iRelationType == DssRelationIsHidden)
            mpIsHidden[i + iStart] = lData;
        else
            return S_FALSE;
    }
    
    return S_OK;
}

HRESULT  DSSRecursivePCTable::Save7u(ICDSSWriteBlockStream *pStm, Int32 fClearDirty, Int32 Flag)
{
    HRESULT hr = S_OK;
    
    hr = pStm->WriteInt(mnRows);
    CHECK_HR;
    
    if (mnRows > 0)
    {
        hr = pStm->WriteIntArray(mpIndex, mnRows);
        CHECK_HR;
        
        hr = pStm->WriteIntArray(mpParents, mnRows);
        CHECK_HR;
        
        hr = pStm->WriteShortArray(mpLevels, mnRows);
        CHECK_HR;
        
        for (Int32 i = 0; i < mnRows; i++)
        {
            hr = pStm->WriteBoolean(mpHasChildren[i] ? VARIANT_TRUE : VARIANT_FALSE);
            CHECK_HR;
        }
        
        for (Int32 i = 0; i < mnRows; i++)
        {
            hr = pStm->WriteBoolean(mpIsHidden[i] ? VARIANT_TRUE : VARIANT_FALSE);
            CHECK_HR;
        }
    }
    
    return S_OK;
}

HRESULT  DSSRecursivePCTable::Load7u(ICDSSReadBlockStream *pStm, Int32 Flag)
{
    HRESULT hr = S_OK;
    
    Int32 lnRows;
    hr = pStm->ReadInt(&lnRows);
    CHECK_HR;
    
    hr = Init(lnRows);
    CHECK_HR;
    
    if (mnRows > 0)
    {
        hr = pStm->ReadIntArray(mnRows, mpIndex);
        CHECK_HR;
        
        hr = pStm->ReadIntArray(mnRows, mpParents);
        CHECK_HR;
        
        hr = pStm->ReadShortArray(mnRows, mpLevels);
        CHECK_HR;
        
        VARIANT_BOOL lBool;
        for (Int32 i = 0; i < mnRows; i++)
        {
            hr = pStm->ReadBoolean(&lBool);
            CHECK_HR;
            
            mpHasChildren[i] = (lBool == VARIANT_TRUE) ? true : false;
        }
        
        for (Int32 i = 0; i < mnRows; i++)
        {
            hr = pStm->ReadBoolean(&lBool);
            CHECK_HR;
            
            mpIsHidden[i] = (lBool == VARIANT_TRUE) ? true : false;
        }
        
    }
    
    return S_OK;
}

HRESULT  DSSRecursivePCTable::SelectInto(DSSRecursivePCTable* ipPCTable, Int32 nRows, Int32* pRows, Int32* lpNewChildren, Int32* lpNewParents, Int32* ipOrig2NewIND, std::set<Int32>* ipHidden)
{ 
    assert(ipPCTable != NULL);
    
    Int32 lRow;
    Int32 lChild;
    Int32 lParent;
    for (Int32 i = 0; i < nRows; i++)
    {
        lRow = (pRows) ? pRows[i] : i;
        lChild = (lpNewChildren) ? lpNewChildren[i] : ipPCTable->mpIndex[i];
        lParent = (lpNewParents) ? lpNewParents[i] : ipPCTable->mpParents[i];
        
        if (lChild != -1)
            mpIndex[i] = (ipOrig2NewIND == NULL) ? lChild : ipOrig2NewIND[lChild];
        else
            mpIndex[i] = -1;
        
        if (lParent != -1)
            mpParents[i] = (ipOrig2NewIND == NULL) ? lParent : ipOrig2NewIND[lParent];
        else
            mpParents[i] = -1;
        
        mpLevels[i] = ipPCTable->mpLevels[lRow];
        mpHasChildren[i] = ipPCTable->mpHasChildren[lRow];
        mpIsHidden[i] = ipPCTable->mpIsHidden[lRow];
        // ipHidden is a set contain the missing parent id
        // so when we set the hidden flag, the missing parent's hidden flag should be set true.
        if (ipHidden)
        {
            if ((*ipHidden).find(lRow) != (*ipHidden).end())
            {
                mpIsHidden[i] =  true;
            }
        }
    }
    
    return S_OK;
}

// populate recursive relation table from original table by map
HRESULT  DSSRecursivePCTable::PopulateFromRelationTable(DSSRecursivePCTable* ipPCTable, Int32 iRows, Int32* ipRows)
{
    HRESULT hr = S_OK;
    
    mnRows = (ipRows == NULL) ? ipPCTable->mnRows : iRows;
    
    mpIndex = new (mpBuffer) Int32[mnRows];
    //CHECK_NEWED_PTR(mpIndex);
    
    mpParents = new (mpBuffer) Int32[mnRows];
    //CHECK_NEWED_PTR(mpParents);
    
    mpLevels = new (mpBuffer) short[mnRows];
    //CHECK_NEWED_PTR(mpLevels);
    
    mpHasChildren = new (mpBuffer) bool[mnRows];
    //CHECK_NEWED_PTR(mpHasChildren);
    
    mpIsHidden = new (mpBuffer) bool[mnRows];
    //CHECK_NEWED_PTR(mpIsHidden);
    
    if (ipRows == NULL)
    {
        memmove(mpIndex, ipPCTable->mpIndex, sizeof(Int32) * mnRows);
        memmove(mpParents, ipPCTable->mpParents, sizeof(Int32) * mnRows);
        memmove(mpLevels, ipPCTable->mpLevels, sizeof(short) * mnRows);
        memmove(mpHasChildren, ipPCTable->mpHasChildren, sizeof(bool) * mnRows);
        memmove(mpIsHidden, ipPCTable->mpIsHidden, sizeof(bool) * mnRows);
    }
    else
    {
        Int32 lRow;
        for (Int32 i = 0; i < mnRows; i++)
        {
            lRow = ipRows[i];
            mpIndex[i] = ipPCTable->mpIndex[lRow];
            mpParents[i] = ipPCTable->mpParents[lRow];
            mpLevels[i] = ipPCTable->mpLevels[lRow];
            mpHasChildren[i] = ipPCTable->mpHasChildren[lRow];
            mpIsHidden[i] = ipPCTable->mpIsHidden[lRow];
        }
    }
    
    
    return S_OK;
}


HRESULT  DSSRecursivePCTable::SetHiddenFlag(Int32 iRow, bool iValue)
{
    mpIsHidden[iRow] = iValue;
    return S_OK;
}

HRESULT  DSSRecursivePCTable::get_Parent(Int32 index, Int32* opParentIndex)
{
    if (index < 0 || index >= mnRows || opParentIndex==NULL)
        return E_INVALIDARG;
    
    if (mpParents == NULL)
        *opParentIndex = -1;
    else
        *opParentIndex = mpParents[index];
    
    return S_OK;
}
