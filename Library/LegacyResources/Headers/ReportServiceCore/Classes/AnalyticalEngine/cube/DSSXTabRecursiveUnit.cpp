//
//  DSSXTabRecursiveUnit.cpp
//  ReportServiceCore
//
//  Created by Sun, Mingmin on 3/16/16.
//  Copyright © 2016 MicroStrategy, Inc. All rights reserved.
//

#include "DSSXTabRecursiveUnit.h"
#include "DSSSort_includes.h"

#include "StdAfx.h"
#include "DSSXTabRecursiveUnit.h"

// CDSSTabularData
#include "DSSTabularData.h"
#include "DSSBinaryDataReader.h"
#include "DSSRWDataModel.h"

DSSXTabRecursiveUnit::DSSXTabRecursiveUnit()
{
    mnRows = 0;
    mpRows = NULL;
}

DSSXTabRecursiveUnit::~DSSXTabRecursiveUnit()
{
    //TODO: to release memory
}

int DSSXTabRecursiveUnit::CountRows()
{
    return S_OK;
}

int DSSXTabRecursiveUnit::CountDisplayRows()
{
    return mnRows;
}

int DSSXTabRecursiveUnit::FindElement(DSSBaseElementProxy *ipElement, int &orKey)
{
    return S_OK;
}

int	DSSXTabRecursiveUnit::createElement(int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement, int iGlobalKey, int iGlobalKeyVer)
{
    return S_OK;
}

bool DSSXTabRecursiveUnit::compareElement(DSSBaseElementProxy *ipElement, int iKey)
{
    DSSTabularUnit* lChildUnit = getChildUnit();
    if (lChildUnit && lChildUnit->getUnitType() == DssTemplateAttribute) {
      return (DSSTabularAttribute*)lChildUnit->compareElement(ipElement, iKey);
    }
    return false;
}

int DSSXTabRecursiveUnit::TransferDisplay2DataKey (int iElement, int & orkey)
{
    //subtotal cases
    if (iElement < 0) {
        orkey = iElement;
        return S_OK;
    }
    
    if (iElement < mnRows) {
        if (mpRows) {
            orkey = mpRows[iElement];
        }
    }
    return S_OK;
}

bool DSSXTabRecursiveUnit::IsDisplayed(int iElement)
{
    if (iElement<0 || iElement>=mnRows) {
        return false;
    }
    
    return mvpElements[iElement]->mfIsDisplayed & !mvpElements[iElement]->mfIsHidden;
}

bool DSSXTabRecursiveUnit::IsAvailable(int iElement)
{
    if (iElement<0 || iElement>=mnRows) {
        return false;
    }
    
    return mvpElements[iElement]->mfIsAvailable;
}

void DSSXTabRecursiveUnit::setDisplayed(int iElement)
{
    if (iElement<0 || iElement>=mnRows) {
        return ;
    }
    
    if (mvpElements[iElement]->mfIsAvailable) {
        mvpElements[iElement]->mfIsDisplayed = true;
        mvpElements[iElement]->mfIsHidden = false;
    }
    
    
    if (mvpElements[iElement]->mfIsExpand || !mvpElements[iElement]->mfIsAvailable) {
        int lChildIndex = mvpElements[iElement]->mFirstChild;
        while (lChildIndex != -1) {
            setDisplayed(lChildIndex);
            lChildIndex = mvpElements[lChildIndex]->mNextSibling;
        }
    }
}

void DSSXTabRecursiveUnit::setHidden(int iElement)
{
    if (iElement<0 || iElement>=mnRows) {
        return ;
    }
    
    //DE48307, to check if there is a displayed ancestor.
    //if not, should never hide this element.
    int parent = mvpElements[iElement]->mParent;
    bool lHasDisplayAncestor = false;
    while (parent >= 0 && parent <mnRows) {
        if (IsAvailable(parent)) {
            lHasDisplayAncestor = true;
            break;
        }
        parent = mvpElements[parent]->mParent;
    }
    if (!lHasDisplayAncestor) {
        return;
    }
    
    mvpElements[iElement]->mfIsDisplayed = false;
    mvpElements[iElement]->mfIsHidden = true;
    
    int lChildIndex = mvpElements[iElement]->mFirstChild;
    while (lChildIndex != -1) {
        setHidden(lChildIndex);
        lChildIndex = mvpElements[lChildIndex]->mNextSibling;
    }
}


//be sure to set display before expand it.
void DSSXTabRecursiveUnit::setExpand(int iRow, bool ibExpand)
{
    if (!IsDisplayed(iRow)) {
        return;
    }
    
    if (iRow<0 || iRow>=mnRows) {
        return ;
    }
    
    mvpElements[iRow]->mfIsExpand = ibExpand;
}

bool DSSXTabRecursiveUnit::IsRecursiveUnit()
{
    return true;
}

int DSSXTabRecursiveUnit::loadFromBinary(DSSBinaryDataReader *ipReadStream)
{
    int hr = S_OK;
    
    hr = ipReadStream->ReadString(mGUIDStr);
    CHECK_HR;
    
    int len = mGUIDStr.length();
#ifdef NO_WCHAR_SUPPORT
    MBase::String2Guid(mGUID, mGUIDStr.c_str());
#else
    vector<wchar_t> lString(len);
    mbstowcs(&lString[0], mGUIDStr.c_str(), len);
    MBase::String2Guid(mGUID, &lString[0]);
#endif
    
    hr = ipReadStream->ReadInt(&mDataUnitID);
    CHECK_HR;
    hr = ipReadStream->ReadInt(&mUnitID);
    CHECK_HR;
    
    hr = ipReadStream->ReadInt(&mnRows);
    CHECK_HR;
    if (!mpRows) {
        mpRows = new int[mnRows];
    }
    hr = ipReadStream->ReadIntArray(mpRows, mnRows);
    CHECK_HR;
    
    for (int i=0; i<mnRows; i++) {
        CRAElement* lElement = new CRAElement();
        hr = ipReadStream->ReadInt(&lElement->mIndex);
        CHECK_HR;
        hr = ipReadStream->ReadShort(&lElement->mDepth);
        CHECK_HR;
        hr = ipReadStream->ReadBool(&lElement->mfIsExpand);
        CHECK_HR;
        hr = ipReadStream->ReadBool(&lElement->mfIsDisplayed);
        CHECK_HR;
        hr = ipReadStream->ReadInt(&lElement->mFirstChild);
        CHECK_HR;
        hr = ipReadStream->ReadInt(&lElement->mNextSibling);
        CHECK_HR;
        hr = ipReadStream->ReadBool(&lElement->mfIsHidden);
        CHECK_HR;
        hr = ipReadStream->ReadInt(&lElement->mParent);
        CHECK_HR;
        hr = ipReadStream->ReadBool(&lElement->mfIsAvailable);
        CHECK_HR;
        mvpElements.push_back(lElement);
    }
    
    if (ipReadStream->MoreDataInBlock() && ipReadStream->ProbeBlockHeader(EnumNewBlockRecursiveElementIDs))
    {
        int lBlockType;
        int hr =  ipReadStream->BeginReadBlock(&lBlockType);
        CHECK_HR;
        int lnConsolidationElements = 0;
        hr = ipReadStream->ReadInt(&lnConsolidationElements);
        CHECK_HR;
        for (int i=0; i<lnConsolidationElements; i++)
        {
            int lElementIndex, lOriginalIndex;
            string lId;
            hr = ipReadStream->ReadInt(&lElementIndex);
            CHECK_HR;
            hr = ipReadStream->ReadString(lId);
            CHECK_HR;
            hr = ipReadStream->ReadInt(&lOriginalIndex);
            CHECK_HR;
            CRAElement_Extent lCon = {lElementIndex, lOriginalIndex, lId};
            mvConsolidationElements.push_back(lCon);
        }
        ipReadStream->EndReadBlock();
    }
    return hr;
    
}

CRAElement* DSSXTabRecursiveUnit::get_elementRecursiveInfo(int iRow)
{
    if (iRow >= mvpElements.size()) {
        return NULL;
    }
    return mvpElements[iRow];
}

DSSTabularUnit* DSSXTabRecursiveUnit::getChildUnit()
{
    return mTabularData->Item(mDataUnitID);
}


int DSSXTabRecursiveUnit::updateExpandStatus(int iElement, bool iExpand, bool iApplyAll)
{
    if (iApplyAll) {
        for (int i=0; i<mnRows; i++) {
            if (iExpand) {
                if (mvpElements[i]->mfIsAvailable) {
                    setDisplayed(i);
                    if (mvpElements[i]->mFirstChild != -1) {
                        setExpand(i, true);
                    }
                }
            }
            else
            {
                if (mvpElements[i]->mFirstChild != -1) {
                    setExpand(i, false);
                }
                
                if (mvpElements[i]->mParent != -1) {
                    setHidden(i);
                }
                
            }
            
        }
        return 0;
    }
    
    if (iElement<0 || iElement>=mnRows) {
        return -1;
    }
    
    if (!mvpElements[iElement]->mfIsAvailable)
        return -1;
    
    if(!IsDisplayed(iElement))
        return -1;
    
    setExpand(iElement, iExpand);
    CRAElement* lElementInfo = get_elementRecursiveInfo(iElement);
    if (lElementInfo) {
        int lChildIndex = lElementInfo->mFirstChild;
        while (lChildIndex != -1) {
            if (iExpand) {
                setDisplayed(lChildIndex);
            }
            else
            {
                setHidden(lChildIndex);
            }
            lChildIndex = mvpElements[lChildIndex]->mNextSibling;
        }
    }
    
    return -1;
}


int DSSXTabRecursiveUnit::getCountDisplayChildren(int iElement)
{
    if (iElement<0 || iElement>=mnRows) {
        return -1;
    }
    
    int lCount = 0;
    int lChild = mvpElements[iElement]->mFirstChild;
    while (lChild >= 0 && lChild < mnRows) {
        if (mvpElements[lChild]->mfIsAvailable && IsDisplayed(lChild)) {
            lCount++;
        }
        
        if (IsDisplayed(lChild)) {
            lCount += getCountDisplayChildren(lChild);
        }
        
        lChild = mvpElements[lChild]->mNextSibling;
    }
    return lCount;
}

int DSSXTabRecursiveUnit::checkElementType(int iRow, bool* oIsBranch)
{
    for (int i =0; i < mvConsolidationElements.size();i++)
    {
       if (mvConsolidationElements[i].index == iRow)
       {
           *oIsBranch = true;
           return S_OK;
       }
    }
    *oIsBranch = false;
    return S_OK;
}

int DSSXTabRecursiveUnit::get_ElementOriginalIndex(int iRow, int * lOriIndex)
{
    
    for (int i =0; i < mvConsolidationElements.size();i++)
    {
        if (mvConsolidationElements[i].index == iRow)
        {
            *lOriIndex = mvConsolidationElements[i].originalIndex;
            return S_OK;
        }
    }
    *lOriIndex = -1;
    return S_OK;
}

void DSSXTabRecursiveUnit::GenData2RecursiveMap(Int32* ipData2RecursiveMap, Int32 iRow)
{
    if (!ipData2RecursiveMap || iRow <= 0)
        return;
    
    for (Int32 i = 0; i < mvpElements.size(); ++i)
    {
        if (mvpElements[i]->mIndex >= 0 && mvpElements[i]->mIndex < iRow)
            ipData2RecursiveMap[mvpElements[i]->mIndex] = i;
    }
    return;
}

//below sort functions copied from CDSSXTabRecursiveUnit in Server codes.
int DSSXTabRecursiveUnit::hSortFormByInputOrder(Int32* ipElements, Int32 iElements, Int32* ipInputOrder, Int32* opElementOrder, bool ifIsDataOrder)
{
    int hr = S_OK;
    
    // no need to sort if element num less than one
    if (iElements <= 1)
        return S_OK;
    
    // get the element keys
    //Int32* lpKeyOrder = reinterpret_cast<Int32 *>(_alloca(sizeof(Int32) * iElements));
    Int32* lpKeyOrder = new Int32[iElements];
    CHECK_NEWED_PTR(lpKeyOrder);
    MBase::StrongPtr<Int32> lpIntHolder(lpKeyOrder);
    
    for (Int32 i = 0; i < iElements; i++)
        lpKeyOrder[i] = (ifIsDataOrder ? ipInputOrder[mvpElements[ipElements[i] ]->mIndex] : ipInputOrder[ipElements[i] ]);
    
    // sort the element by input form value order
    DSSAEHelper::IndirectOrder lOrder(lpKeyOrder);
    _xtab_sort(opElementOrder, opElementOrder + iElements, lOrder);
    
    return S_OK;
}

int DSSXTabRecursiveUnit::hSortRecursiveTree(CRAElement* ipElement, Int32& iSequence, Int32* ipInputOrder, Int32 *opElementOrder, bool ifIsDataOrder)
{
    int hr = S_OK;
    
    vector<Int32> lvChildren;
    Int32 lChild = ipElement->mFirstChild;
    while (lChild != -1)
    {
        lvChildren.push_back(lChild);
        
        CRAElement* lpElement = mvpElements[lChild];
        lChild = lpElement->mNextSibling;
    }
    
    Int32 lnChildren = lvChildren.size();
    if (lnChildren == 0)
        return S_OK;
    
    // sort the elment in the same level by form
    Int32 lpOrder[lnChildren];
    CHECK_NEWED_PTR(lpOrder);
    for (Int32 i = 0; i < lnChildren; i++)
        lpOrder[i] = i;
    
    hr = hSortFormByInputOrder(&lvChildren[0], lnChildren, ipInputOrder, lpOrder, ifIsDataOrder);
    CHECK_HR;
    
    // sort by level, sort the roots firstly, then the children
    for (Int32 i = 0; i < lnChildren; i++)
    {
        Int32 lElement = lvChildren[lpOrder[i] ];
        // sort the roots
        opElementOrder[iSequence++] = lElement;
        
        // sort the children
        hr = hSortRecursiveTree(mvpElements[lElement], iSequence, ipInputOrder, opElementOrder, ifIsDataOrder);
        CHECK_HR;
    }
    
    return S_OK;
}

//inputOrder[elementKey] = rank, outputOrder[rank] = displayElementKey
//ifIsDataOrder TURE if the inputOrder is based on data element key
//TODO: should skip the hidden elements while sorting?
int DSSXTabRecursiveUnit::Sort(Int32* ipOrder, Int32 iRow, Int32* opElementOrder, bool ifIsDataOrder)
{
    int hr = S_OK;
    // get the roots row sequence
    vector<Int32> lvRoots;
    CRAElement* lpElement = mvpElements[0];
    // ldong 2/5/2014 just sort every element, not matter it is displayed or not
    // it is possible that a root node is not displayed(a hidden branch element as root)
    lvRoots.push_back(0);
    Int32 lNext = lpElement->mNextSibling;
    while (lNext != -1)
    {
        lvRoots.push_back(lNext);
        lpElement = mvpElements[lNext];
        lNext = lpElement->mNextSibling;
    }
    
    //// comment code below and fix a bug
    //if (lpElement->mfIsDisplayed)
    //	lvRoots.push_back(0);
    //Int32 lNext = lpElement->mNextSibling;
    //while (lNext != -1)
    //{
    //	lpElement = mvpElements[lNext];
    //	if (lpElement->mfIsDisplayed)
    //		lvRoots.push_back(lNext);
    //	lNext = lpElement->mNextSibling;
    //}
    
    // sort the elment in the same level by form
    Int32 lnRoot = lvRoots.size();
    Int32* lpOrder = new Int32 [lnRoot];
    CHECK_NEWED_PTR(lpOrder);
    for (Int32 i = 0; i < lnRoot; i++)
        lpOrder[i] = i;
    
    hr = hSortFormByInputOrder(&lvRoots[0], lnRoot, ipOrder, lpOrder, ifIsDataOrder);
    CHECK_HR;
    
    // sort by level, sort the roots firstly, then the children
    Int32 lNum = 0;
    for (Int32 i = 0; i < lnRoot; i++)
    {
        Int32 lElement = lvRoots[lpOrder[i] ];
        // sort the roots
        opElementOrder[lNum++] = lElement;
        
        // sort the children
        hr = hSortRecursiveTree(mvpElements[lElement], lNum, ipOrder, opElementOrder, ifIsDataOrder);
        CHECK_HR;
    }
    
    //_ASSERT(lNum <= iRow);
    
    return S_OK;
}



