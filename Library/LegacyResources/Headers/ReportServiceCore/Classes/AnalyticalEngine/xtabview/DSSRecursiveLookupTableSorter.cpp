//
//  DSSRecursiveLookupTableSorter.cpp
//  ReportServiceCore
//
//  Created by Sun, Mingmin on 6/12/16.
//  Copyright © 2016 MicroStrategy, Inc. All rights reserved.
//

#import "DSSRecursiveLookupTableSorter.h"
#import "DSSSorter.h"
#import "DSSSortDefinition.h"
#import "DSSXTabHeader.h"
#import "DSSDataColumn.h"
#import "DSSSort_includes.h"
#import "DSSTabularAttribute.h"
#import "DSSTabularData.h"
#import "DSSDataSource.h"
#import "DSSTabularData.h"
#import "DSSViewDataSet.h"
#import "DSSGUnitContainer.h"
#import "DSSXTabRecursiveUnit.h"
#import <map>
#define MAX_ROWCOUNT_SUPPORT 300000
DSSRecursiveLookupTableSorter::DSSRecursiveLookupTableSorter(DSSSortDefinition* definition, EnumDSSSortType sortType)
{
    if (sortType ==  DssSortRecursiveAttributeDefault) {
        _unitID=definition->unitID;
        _unitIDinContainer = definition->unitIDInContianer;
        _bucketID = definition->bucketID;
        _subtotalPosition=definition->subtotalPosition;
        _model=definition->model;
        DSSGUnitContainer * lpGlobalLookupContainer = ((DSSTabularData *)definition->model)->getGUnitContainer();
        
        int *lpResult;
        int lnResult;
        
        int hr = lpGlobalLookupContainer->getSortResult(_unitIDinContainer, _bucketID, &lpResult, &lnResult);
        if(hr == S_OK)
        {
            _buckets.resize(lnResult);
            for (int i = 0; i < lnResult; ++i) {
                _buckets[i] = lpResult[i];
            }
        }
        else
        {
            assert(false);
        }
    }
    else
    {
        _unitID=definition->unitID;
        _formID=definition->formID;
        _isAscending=definition->isAscending;
        _isNullOnTop=definition->isNullOnTop;
        _subtotalPosition=definition->subtotalPosition;
        _model=definition->model;
        _sortType = sortType;
        _column = NULL;
        _nSortedElem = 0;
    }
    mpGlobal2LocalRecursiveMap = NULL;
}

DSSRecursiveLookupTableSorter::~DSSRecursiveLookupTableSorter() {
    int lDataSize = _buckets.size()*sizeof(int)*2;
    _buckets.clear();
    if (mpGlobal2LocalRecursiveMap)
    {
        delete [] mpGlobal2LocalRecursiveMap;
    }
    MEM_GOVERNOR_RELEASE(lDataSize, lDataSize)
}

int DSSRecursiveLookupTableSorter::sort(DSSXTabHeader* header, vector<int>& items)
{
    int hr = S_OK;
    
    if(items.size()<=1)
        return hr;
    bool found = false;
    vector<int> lvForms;
    int formIndex;
    
    bool *lpAsc = new bool();
    // 8/20/2012 tehe: memory leak
    MBase::StrongPtr<bool> sptrAsc(lpAsc);
    
    *lpAsc = _isAscending;
    
    //TQMS 696028, hyan, 5/1/2013
    //Init to -1
    int lUnitIDinGUnitContainer=-1;
    DSSGUnitContainer * lpGlobalLookupContainer = NULL;
    bool lbUseSSO = false;
    
    if (_sortType == DssSortRecursiveAttributeForm) {
        
        DSSTabularUnit* lUnit = _model->Item(_unitID);
        DSSTabularAttribute* attribute = NULL;
        if (lUnit->IsRecursiveUnit()) {
            attribute = (DSSTabularAttribute*)((DSSXTabRecursiveUnit*)lUnit)->getChildUnit();
            //TODO: sort for recursive attribute
            //return hr;
        }
        else
            return hr;
        hr = attribute->FindFormWithLocale(_unitID, _formID, 0, formIndex);
        //TQMS 653168, for extra Unit, hr will be S_FALSE
        if(hr==S_FALSE)
            return S_OK;
        lvForms.push_back(formIndex);

        //    DSSViewDataSet *lpViewDataSet = dynamic_cast<DSSViewDataSet *>(_model);
        //    if(lpViewDataSet && _buckets.size()==0)
        if(_model->getIsNewRWD() && _buckets.size()==0)
        {
            DSSTabularUnit* unit=_model->Item(_unitID);
            GUID guid;
            unit->getGUID(guid);
            
            //zhyang, 677674
            if(_model->canUseSSO(guid))
            {
                lbUseSSO = true;
                
                lpGlobalLookupContainer = _model->getGUnitContainer();
                //hyan,2012/10/31, for missing unit, the lUnitIDinGUnitContainer will be -1
                hr = lpGlobalLookupContainer->GetGlobalUnitIDByGUID(guid, &lUnitIDinGUnitContainer);
                
                if(hr == S_OK)
                {
                    int lBucketID = lpGlobalLookupContainer->getBucketID(lUnitIDinGUnitContainer, lvForms, lpAsc, _isNullOnTop);
                    
                    if(lBucketID != -1)
                    {
                        found = true;
                        int *lpResult;
                        int lnResult;
                        
                        int hr = lpGlobalLookupContainer->getSortResult(lUnitIDinGUnitContainer, lBucketID, &lpResult, &lnResult);
                        if(hr == S_OK)
                        {
                            _buckets.resize(lnResult);
                            for (int i = 0; i < lnResult; ++i) {
                                _buckets[i] = lpResult[i];
                            }
                        }
                        
                    }
                }
            }
            else
            {
                lUnitIDinGUnitContainer = -1;
            }
            
        }
        
        if(_buckets.size()==0)
        {
            
            int formIndex;
            hr = attribute->FindFormWithLocale(_unitID, _formID, 0, formIndex);
            
            //TQMS 653168, for extra Unit, hr will be S_FALSE
            if(hr==S_FALSE)
                return S_OK;
            //CHECK_HR;
            
            hr = attribute->Item(formIndex, &_column);
            CHECK_HR;
            
            int count=_column->getRowCount() + 1;
            
            if (count <= 1)
                return S_OK;
            
            //junwang 09/02/2011, Red Rocket : issue 500695
            /*
            int* lpInputOrder = _column->GetInputOrder();
            if(!lpInputOrder)
            {
                if(((double)items.size() / (double)count) < 0.2)
                {
                    return sortDisplay(header, items);
                }
            }*/
            MEM_GOVERNOR_RESERVE(count * sizeof(int) * 2, count * sizeof(int) * 2) //for both _buckets & ranks
            int i;
            vector<int> lvNullBucket;
            DSSDataFlag lFlag;
            vector<int> ranks;
            ranks.reserve(count);
            
            for(i=0;i<count;++i)
            {
                hr = _column->getFlag(i, lFlag);
                //CHECK_HR;
                if(hr == S_OK && lFlag == DssDataOk)
                    ranks.push_back(i);
                else
                    lvNullBucket.push_back(i);
            }
            hr = S_OK;

            MEM_GOVERNOR_COMMIT(count * sizeof(int) * 2)
            
            mGlobalRow = _column->getRowCount();
            mpGlobal2LocalRecursiveMap = new Int32[mGlobalRow];
            CHECK_NEWED_PTR(mpGlobal2LocalRecursiveMap);

            ((DSSXTabRecursiveUnit*)lUnit)->GenData2RecursiveMap(mpGlobal2LocalRecursiveMap, mGlobalRow);
            /*if(lpInputOrder)
            {
                //zhyang, 752560
                //zhyang, 749885, for new rwd logic, we get the null elements out of other elements, so we can's simply use memmove.
                //lwang, 713078, becuse of the fix of 545632 count is bigger than lpInputOrder.
                //memmove(&ranks[0], lpInputOrder, ranks.size()*sizeof(int));
                ranks.clear();
                int lInputOrderCnt = _column->getInputOrderCnt();
                for(i=0;i<lInputOrderCnt;++i)
                {
                    hr = _column->getFlag(lpInputOrder[i], lFlag);
                    if(hr == S_OK && lFlag == DssDataOk)
                        ranks.push_back(lpInputOrder[i]);
                }
            }
            else*/
                //_xtab_sort(&ranks[0], &ranks[0] + count, *this);
                _xtab_sort(&ranks[0], &ranks[0] + ranks.size(), *this);
            
            int rank=0;
            _buckets.resize(count);
            if (count > 0)		//477477, it's a runtime unit, only has total
            {
                if(_isNullOnTop)
                {
                    for(int i = 0; i < lvNullBucket.size(); ++i)
                    {
                        _buckets[lvNullBucket[i]] = rank;
                    }
                    if(lvNullBucket.size() != 0)
                    {
                        rank++;
                    }
                }
                if(ranks.size() > 0)
                {
                    _buckets[ranks[0]]=rank;
                    for(i=1;i<ranks.size();++i)
                    {
                        if(/*lpInputOrder || */_column->Order(ranks[i], ranks[i-1]))  // if differ from the last element
                            rank++;
                        _buckets[ranks[i]]=rank;
                    }
                }
                if(!_isNullOnTop)
                {
                    rank++;
                    for(int i = 0; i < lvNullBucket.size(); ++i)
                    {
                        _buckets[lvNullBucket[i]] = rank;
                    }
                }
            }
            ranks.clear();
            MEM_GOVERNOR_RELEASE(count * sizeof(int) * 2, count * sizeof(int) * 2)
            
        }
        
        if(lbUseSSO && !found && lpGlobalLookupContainer &&(lUnitIDinGUnitContainer != -1))
        {
            vector<bool> lvAsc;
            lvAsc.push_back(*lpAsc);
            hr = lpGlobalLookupContainer->setSortingResult(lUnitIDinGUnitContainer, lvForms, lvAsc, _isNullOnTop, &_buckets[0]);
            CHECK_HR;
        }
    }
    else
    {
        if (_buckets.size() == 0) {
            return hr;
        }
    }
    //respect hierarchy
    displayRespectHierarchy(_buckets);
    
    int unitIndex=0;
    vector<int>& units=header->getUnits();
    for(int i=0;i<units.size();++i)
    {
        if(units[i]==_unitID)
        {
            unitIndex=i;
            break;
        }
    }
    
    int lReservSize = (items.size()+_buckets.size() + 1) * 2 * sizeof(int);
    MEM_GOVERNOR_RESERVE(lReservSize, lReservSize);
    int *first = new int[_buckets.size() + 1]; //zhyang, the last one is for subtotal bucket
    CHECK_NEWED_PTR(first);
    MBase::StrongArrayPtr<int> lPtrFirst(first);
    
    int *last = new int[_buckets.size() + 1];
    CHECK_NEWED_PTR(last);
    MBase::StrongArrayPtr<int> lPtrLast(last);
    
    int *next = new int[items.size()];
    CHECK_NEWED_PTR(next);
    MBase::StrongArrayPtr<int> lPtrNext(next);
    
    int *value = new int[items.size()];
    CHECK_NEWED_PTR(value);
    MBase::StrongArrayPtr<int> lPtrValue(value);
    
    memset(first, -1, sizeof(int) * (_buckets.size() + 1));
    memset(next, -1, sizeof(int) * (items.size()));
    MEM_GOVERNOR_COMMIT(lReservSize);
    
    for(int i=0;i<items.size();++i)
    {
        int elementID=header->getUnitKey(items[i],unitIndex);

        int rank = -1;
        if(elementID>=0) // normal element
            rank=_buckets[elementID];
        else // subtotal
            rank = _buckets.size();
        value[i] = items[i];
        if(first[rank] == -1)
        {
            first[rank] = i;
        }
        else
        {
            next[last[rank]] = i;
        }
        last[rank] = i;
    }
    
    
    items.clear();
    if(_subtotalPosition==DssSubtotalsPositionFirst)
    {
        for(int i = first[_buckets.size()]; i != -1; i = next[i])
            items.push_back(value[i]);
    }
    
    for(int i=0;i<_buckets.size();++i)
    {
        for(int j = first[i]; j != -1; j = next[j])
            items.push_back(value[j]);
    }
    
    if(_subtotalPosition!=DssSubtotalsPositionFirst)
    {
        for(int i = first[_buckets.size()]; i != -1; i = next[i])
            items.push_back(value[i]);
    }
    
    MEM_GOVERNOR_RELEASE(lReservSize, lReservSize)
    
    
    return S_OK;
}

//input items[dataElementKey] = rank
//output items[displayElementKey] = rank
int DSSRecursiveLookupTableSorter::displayRespectHierarchy(vector<int>& items)
{
    int hr = S_OK;
    DSSTabularUnit* lUnit = _model->Item(_unitID);
    if (!lUnit || !lUnit->IsRecursiveUnit()) {
        return hr;
    }
    int lnItems = items.size();
    int * lpHierarchyOrder = new int [lnItems];
    memset(lpHierarchyOrder, -1, sizeof(int) * lnItems);
    DSSXTabRecursiveUnit* lpRAUnit = (DSSXTabRecursiveUnit*)lUnit;
    hr = lpRAUnit->Sort(&items[0], lnItems, lpHierarchyOrder, true);
    
    items.clear();
    items.resize(lnItems);
    memset(&items[0], -1, sizeof(int) * lnItems);
    for (int i = 0; i < lnItems; i++) {
        if (lpHierarchyOrder[i] != -1) {
            items[lpHierarchyOrder[i]] = i;
        }
    }
}

bool DSSRecursiveLookupTableSorter::operator()(int a, int b)
{
    int Locale = 0;
    if (_model != NULL)
        _model->getUnitLocale(_unitID, Locale);
    
    if (_formID == 0)
    {
        DSSTabularUnit* lUnit = _model->Item(_unitID);
        if (lUnit && lUnit->IsRecursiveUnit())
        {
            DSSXTabRecursiveUnit* lpRAUnit = (DSSXTabRecursiveUnit*)lUnit;
            bool lIsLeftBranch = false, lIsRightBranch = false;
            if (lpRAUnit->hasNDEElements())
            {
                lpRAUnit->checkElementType(mpGlobal2LocalRecursiveMap[a], &lIsLeftBranch);
                lpRAUnit->checkElementType(mpGlobal2LocalRecursiveMap[b], &lIsRightBranch);
                if (lIsRightBranch || lIsLeftBranch) {
                    if (lIsLeftBranch && lIsRightBranch)
                    {
                        int leftOriginalIndex, rightOriginalIndex;
                        lpRAUnit->get_ElementOriginalIndex(mpGlobal2LocalRecursiveMap[a], &leftOriginalIndex);
                        lpRAUnit->get_ElementOriginalIndex(mpGlobal2LocalRecursiveMap[b], &rightOriginalIndex);
                        int order = leftOriginalIndex - rightOriginalIndex;
                        return _isAscending ? order <0 : order > 0;
                    }
                    else{
                        int order = lIsRightBranch - lIsLeftBranch;
                        return _isAscending ? order < 0 : order > 0;
                    }
                }
            }
        }
    }
    int order = _column->Compare(a, b, _isAscending ? _isNullOnTop : !_isNullOnTop, Locale, false, false);
    return _isAscending ? order < 0 : order > 0;
}

bool DSSRecursiveLookupTableSorter::sortUnitElements(vector<int>& elements)
{
    if(elements.size()<=1)
        return true;
    
    DSSTabularAttribute* attribute=(DSSTabularAttribute*)(_model->Item(_unitID));
    int formIndex;
    if(attribute->FindFormWithLocale(_unitID, _formID, 0, formIndex)!=S_OK)
        return false;
    if(attribute->Item(formIndex, &_column)!=S_OK)
        return false;
    _xtab_sort(&elements[0], &elements[0] + elements.size(), *this);
    return true;
}


/*
int DSSRecursiveLookupTableSorter::sortDisplay(DSSXTabHeader* header, vector<int>& items)
{
    set<int> sortKeys;
    
    int unitIndex=0;
    vector<int>& units=header->getUnits();
    for(int i=0;i<units.size();++i)
    {
        if(units[i]==_unitID)
        {
            unitIndex=i;
            break;
        }
    }
    
    for(int i=0;i<items.size();++i)
    {
        int elementID=header->getUnitKey(items[i],unitIndex);
        if(elementID>=0)
            sortKeys.insert(elementID);
    }
    
    vector<int> ranks;
    ranks.assign(sortKeys.begin(), sortKeys.end());
    _xtab_sort(&ranks[0], &ranks[0] + ranks.size(), *this);
    
    map<int, int> sortResultMap;
    for(int i = 0; i < ranks.size(); ++i)
        sortResultMap[ranks[i]] = i;
    //sortResultMap[ranks[i]] = i;
    
    //==================start new sort logic
    
    int lReservSize = (items.size()+ranks.size() + 1) * 2 * sizeof(int);
    MEM_GOVERNOR_RESERVE(lReservSize, lReservSize);
    int *first = new int[ranks.size() + 1]; //zhyang, the last one is for subtotal bucket
    CHECK_NEWED_PTR(first);
    MBase::StrongArrayPtr<int> lPtrFirst(first);
    
    int *last = new int[ranks.size() + 1];
    CHECK_NEWED_PTR(last);
    MBase::StrongArrayPtr<int> lPtrLast(last);
    
    int *next = new int[items.size()];
    CHECK_NEWED_PTR(next);
    MBase::StrongArrayPtr<int> lPtrNext(next);
    
    int *value = new int[items.size()];
    CHECK_NEWED_PTR(value);
    MBase::StrongArrayPtr<int> lPtrValue(value);
    
    memset(first, -1, sizeof(int) * (ranks.size() + 1));
    memset(next, -1, sizeof(int) * (items.size()));
    MEM_GOVERNOR_COMMIT(lReservSize);
    
    //int pos = 0;
    for(int i=0;i<items.size();++i)
    {
        int elementID=header->getUnitKey(items[i],unitIndex);
        int rank = -1;
        if(elementID>=0) // normal element
            rank=sortResultMap[elementID];
        else // subtotal
            rank = ranks.size();
        value[i] = items[i];
        if(first[rank] == -1)
        {
            first[rank] = i;
        }
        else
        {
            next[last[rank]] = i;
        }
        last[rank] = i;
    }
    
    items.clear();
    if(_subtotalPosition==DssSubtotalsPositionFirst)
    {
        for(int i = first[ranks.size()]; i != -1; i = next[i])
            items.push_back(value[i]);
    }
    
    for(int i=0;i<ranks.size();++i)
    {
        for(int j = first[i]; j != -1; j = next[j])
            items.push_back(value[j]);
    }
    
    if(_subtotalPosition!=DssSubtotalsPositionFirst)
    {
        for(int i = first[ranks.size()]; i != -1; i = next[i])
            items.push_back(value[i]);
    }
    
    MEM_GOVERNOR_RELEASE(lReservSize, lReservSize)
    
    return S_OK;
}*/
