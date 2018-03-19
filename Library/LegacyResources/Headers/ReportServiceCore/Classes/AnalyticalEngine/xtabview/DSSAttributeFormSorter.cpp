#import "DSSSorter.h"
#import "DSSSortDefinition.h"
#import "DSSXTabHeader.h"
#import "DSSDataColumn.h"
#import "DSSSort_includes.h"
#import "DSSAttributeFormSorter.h"
#import "DSSTabularAttribute.h"
#import "DSSTabularData.h"
#import "DSSDataSource.h"
#import "DSSTabularData.h"
#import "DSSViewDataSet.h"
#import "DSSGUnitContainer.h"
#import "DSSXTabRecursiveUnit.h"
#import <map>
#define MAX_ROWCOUNT_SUPPORT 300000
DSSAttributeFormSorter::DSSAttributeFormSorter(DSSSortDefinition* definition)
{
	_unitID=definition->unitID;
	_formID=definition->formID;
	_isAscending=definition->isAscending;
	_isNullOnTop=definition->isNullOnTop;
	_subtotalPosition=definition->subtotalPosition;
	_model=definition->model;
    
    _column = NULL;
    _nSortedElem = 0;
}

DSSAttributeFormSorter::~DSSAttributeFormSorter() {
	int lDataSize = _buckets.size()*sizeof(int)*2;
	_buckets.clear();
    MEM_GOVERNOR_RELEASE(lDataSize, lDataSize)
}

int DSSAttributeFormSorter::sort(DSSXTabHeader* header, vector<int>& items)
{
    //668941, zhyang. Jun's optimization didn't consider join null element. since we have selector sort optimization, we don't use this to sort.
    //return sort_New(header, items);
    
	int hr = S_OK;
	
	if(items.size()<=1)
		return hr;
    
    //current mobile device doesn't support grids with too many rows
    //if(items.size() >= MAX_ROWCOUNT_SUPPORT)
    //    return hr;
    
    bool found = false;
    vector<int> lvForms;
    int formIndex;
    DSSTabularUnit* lUnit = _model->Item(_unitID);
    DSSTabularAttribute* attribute = NULL;
    if (lUnit->IsRecursiveUnit()) {
        attribute = (DSSTabularAttribute*)((DSSXTabRecursiveUnit*)lUnit)->getChildUnit();
        //TODO: sort for recursive attribute
        //return hr;
    }
    else
        attribute=(DSSTabularAttribute*)(lUnit);
    hr = attribute->FindFormWithLocale(_unitID, _formID, 0, formIndex);
    //TQMS 653168, for extra Unit, hr will be S_FALSE
    if(hr==S_FALSE)
        return S_OK;
    lvForms.push_back(formIndex);
    bool *lpAsc = new bool();
    // 8/20/2012 tehe: memory leak
    MBase::StrongPtr<bool> sptrAsc(lpAsc);
    
    *lpAsc = _isAscending;
    //TQMS 696028, hyan, 5/1/2013
    //Init to -1
    int lUnitIDinGUnitContainer=-1;
    DSSGUnitContainer * lpGlobalLookupContainer = NULL;
    bool lbUseSSO = false;
    
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
        int* lpInputOrder = _column->GetInputOrder();
        if(!lpInputOrder)
        {
             if(((double)items.size() / (double)count) < 0.2)
             {
                 return sortDisplay(header, items);
             }
        }
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
		//vector<int> ranks=_buckets;
        MEM_GOVERNOR_COMMIT(count * sizeof(int) * 2)
        
        
        
        
        if(lpInputOrder)
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
        else
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
                    if(lpInputOrder || _column->Order(ranks[i], ranks[i-1]))  // if differ from the last element
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
    
    if(lbUseSSO && !found && lpGlobalLookupContainer &&(lUnitIDinGUnitContainer != -1))
    {
        vector<bool> lvAsc;
        lvAsc.push_back(*lpAsc);
        hr = lpGlobalLookupContainer->setSortingResult(lUnitIDinGUnitContainer, lvForms, lvAsc, _isNullOnTop, &_buckets[0]);
        CHECK_HR;
    }
    
	return hr;
}

int DSSAttributeFormSorter::sort_New(DSSXTabHeader* header, vector<int>& items)
{
    int hr = S_OK;
    
    int lnItem = items.size();
	if(lnItem<=1)
		return hr;
    
    vector<int> subtotalElems;
    vector<int> nonSubtotalElems;
    vector<int> elementIds;
    
    //step 1, compute local unit index
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
    
    //step 2, compute element ids
    MEM_GOVERNOR_RESERVE(lnItem*sizeof(int)*2,lnItem*sizeof(int)*2)
    elementIds.resize(items.size());
    nonSubtotalElems.resize(items.size());
    MEM_GOVERNOR_COMMIT(lnItem*sizeof(int)*2)
    
    int nSubtotal = 0;
    for (int i=0; i<items.size(); i++)
    {
		int elementID=header->getUnitKey(items[i],unitIndex);
		if(elementID>=0) // normal element
		{
            elementIds[i-nSubtotal] = elementID;
            nonSubtotalElems[i-nSubtotal] = items[i];
		}
		else // subtotal
		{
            subtotalElems.push_back(items[i]);
            nSubtotal++;
		}
    }
    if(items.size() == nSubtotal) //subtotal elements
        return hr;
    
    elementIds.resize((int)items.size()-nSubtotal);
    nonSubtotalElems.resize(elementIds.size());
    
    
    if(_column==NULL)
    {
        DSSTabularAttribute* attribute=(DSSTabularAttribute*)(_model->Item(_unitID));
		int formIndex;
		hr = attribute->FindFormWithLocale(_unitID, _formID, 0, formIndex);
		CHECK_HR;
        
		hr = attribute->Item(formIndex, &_column);
		CHECK_HR;
    }
    int count=_column->getRowCount();
    
    //step 3, if empty initialize _buckets and _orders
    if(_buckets.size()==0)
    {
        
        MEM_GOVERNOR_RESERVE(count*sizeof(int)*2,count*sizeof(int)*2)
        _buckets.resize(count,-1);
        MEM_GOVERNOR_COMMIT(count*sizeof(int)*2)
        
        int* lpOrder = NULL;
        lpOrder = _column->GetInputOrder();
        
        if (lpOrder&&lpOrder[0]>=0)
        {
            int rank = 0;
            //lwang, 723724, if there's pre-defined element order, sort descending is still treated as ascending.
            if (_isAscending)
            {
                _buckets[lpOrder[0]] = 0;
                
                for (int i=1; i<count; i++)
                {
                    if(lpOrder[i]==-1)
                        break;
                    if(_column->Order(lpOrder[i], lpOrder[i-1]))  // if differ from the last element
                        rank++;
                    _buckets[lpOrder[i]]=rank;
                }
            }
            else
            {
                _buckets[lpOrder[count - 1]] = 0;
                
                for (int i = count - 2; i >= 0; i--)
                {
                    if(lpOrder[i]==-1)
                        continue;
                    if(_column->Order(lpOrder[i], lpOrder[i+1]))  // if differ from the last element
                        rank++;
                    _buckets[lpOrder[i]]=rank;
                }
            }
        }
    }
    
    //step 4, compute sorted and unsorted elements
    vector<int> lvUnSortedElems;
    for (int i=0; i<elementIds.size(); i++)
    {
        if(_buckets[elementIds[i]]==-1)
        {
            lvUnSortedElems.push_back(elementIds[i]);
            _buckets[elementIds[i]] = -2;  //-2 : visited unsorted elements to avoid duplicated elements
        }
    }
    
    //step 5, sort unsorted elements
    if(lvUnSortedElems.size()>1)
    {
        _xtab_sort(&lvUnSortedElems[0], &lvUnSortedElems[0]+lvUnSortedElems.size(), *this);
    }
    
    
    if(_nSortedElem==0)
    {
        _buckets.resize(count,-1);
        if(lvUnSortedElems.size()>0)
        {
            _buckets[lvUnSortedElems[0]] = 0;
            int rank = 0;
            for (int i=1; i<lvUnSortedElems.size(); i++)
            {
                if(_column->Order(lvUnSortedElems[i], lvUnSortedElems[i-1]))
                    rank++;
                _buckets[lvUnSortedElems[i]] = rank;
            }
        }
    }
    else if(lvUnSortedElems.size()>0)
    {
        vector<int> lvBucket;
        vector<int> lvOldStartPos;
        vector<int> lvNewStartPos;
        vector<int> lvNextPos;
        
        MEM_GOVERNOR_RESERVE(4*count*sizeof(int),4*count*sizeof(int))
        lvBucket.resize(count,-1);
        lvOldStartPos.resize(count,-1);
        lvNewStartPos.resize(count,-1);
        lvNextPos.resize(count,-1);
        MEM_GOVERNOR_COMMIT(4*count*sizeof(int))
        
        lvBucket[lvUnSortedElems[0]] = 0;
        int rank = 0;
        for (int i=1; i<lvUnSortedElems.size();i++)
        {
            if(_column->Order(lvUnSortedElems[i], lvUnSortedElems[i-1]))
                rank++;
            lvBucket[lvUnSortedElems[i]] = rank;
        }
        
        //merge
        for (int i=0; i<count; i++)
        {
            int oldRank = _buckets[i];
            if(oldRank>=0)
            {
                if(lvOldStartPos[oldRank]==-1)
                {
                    lvOldStartPos[oldRank]= i;
                }
                else
                {
                    lvNextPos[i] = lvOldStartPos[oldRank];
                    lvOldStartPos[oldRank] = i;
                }
                continue;
            }
            
            int newRank = lvBucket[i];
            if (newRank>=0)
            {
                if(lvNewStartPos[newRank]==-1)
                    lvNewStartPos[newRank] = i;
                else
                {
                    lvNextPos[i] = lvNewStartPos[newRank];
                    lvNewStartPos[newRank] = i;
                }
            }
        }
        
        int lOldStart = 0;
        int lNewStart = 0;
        
        int curRank = 0;
        while (true)
        {
            if(lOldStart>=count||lvOldStartPos[lOldStart]==-1)
            {
                while (lNewStart<count&&lvNewStartPos[lNewStart]>=0) {
                    int lStart = lvNewStartPos[lNewStart];
                    _buckets[lStart] = curRank;
                    
                    lStart = lvNextPos[lStart];
                    while (lStart!=-1) {
                        _buckets[lStart] = curRank; lStart = lvNextPos[lStart];
                    }
                    
                    lNewStart++;  curRank++;
                }
                break;
            }
            
            if(lNewStart>=count||lvNewStartPos[lNewStart]==-1)
            {
                while (lOldStart<count&&lvOldStartPos[lOldStart]>=0) {
                    int lStart = lvOldStartPos[lOldStart];
                    _buckets[lStart] = curRank;
                    
                    lStart = lvNextPos[lStart];
                    while (lStart!=-1) {
                        _buckets[lStart] = curRank; lStart = lvNextPos[lStart];
                    }
                    
                    lOldStart++;  curRank++;
                }
                
                break;
            }
            
            int order = _column->Compare(lvOldStartPos[lOldStart], lvNewStartPos[lNewStart], _isAscending ? _isNullOnTop : !_isNullOnTop, 0, true);
            
            if(order == 0)
            {
                int lstart = lvOldStartPos[lOldStart];
                _buckets[lstart] = curRank;
                
                lstart = lvNextPos[lstart];
                while (lstart!=-1) {
                    _buckets[lstart] = curRank; lstart = lvNextPos[lstart];
                }
                lOldStart++;
                
                lstart = lvNewStartPos[lNewStart];
                _buckets[lstart] = curRank;
                lstart = lvNextPos[lstart];
                while (lstart!=-1) {
                    _buckets[lstart] = curRank; lstart = lvNextPos[lstart];
                }
                lNewStart++;
                
                curRank++;
            }
            else
            {
                if((order<0&&_isAscending)||(order>0&&!_isAscending))
                {
                    int lstart = lvOldStartPos[lOldStart];
                    _buckets[lstart] = curRank;
                    
                    lstart = lvNextPos[lstart];
                    while (lstart!=-1) {
                        _buckets[lstart] = curRank; lstart = lvNextPos[lstart];
                    }
                    lOldStart++;
                    
                    curRank++;
                }
                else
                {
                    int lstart = lvNewStartPos[lNewStart];
                    _buckets[lstart] = curRank;
                    lstart = lvNextPos[lstart];
                    while (lstart!=-1) {
                        _buckets[lstart] = curRank; lstart = lvNextPos[lstart];
                    }
                    lNewStart++;
                    
                    curRank++;
                }
            }
        }
        lvBucket.clear();
        lvOldStartPos.clear();
        lvNextPos.clear();
        lvNewStartPos.clear();
        MEM_GOVERNOR_RELEASE(4*count*sizeof(int),4*count*sizeof(int))
        
    }
    
    _nSortedElem+=lvUnSortedElems.size();
    
    {
        MEM_GOVERNOR_RESERVE((count+elementIds.size())*sizeof(int),(count+elementIds.size())*sizeof(int))
    }
    vector<int> lvCount;
    vector<int> lvDuplicatedID;
    lvCount.resize(count,0);
    lvDuplicatedID.resize(elementIds.size(),0);
    MEM_GOVERNOR_COMMIT((count+elementIds.size())*sizeof(int))
    
    
    for(int i=0; i<elementIds.size();i++)
    {
        int rank = _buckets[elementIds[i]];
        lvDuplicatedID[i] = lvCount[rank];
        lvCount[rank]++;
    }
    for (int i=1; i<count; i++) {
        lvCount[i]+=lvCount[i-1];
    }
    
    
    
    int offset = 0;
    
    if(_subtotalPosition == DssSubtotalsPositionFirst)
    {
        for(int i=0;i<subtotalElems.size();++i)
            items[i] = subtotalElems[i];
        offset = subtotalElems.size();
    }
    
    for (int i=0; i<elementIds.size(); i++)
    {
        int rank = _buckets[elementIds[i]];
        int pos = 0;
        if(rank>0)
        {
            pos = offset + lvCount[rank-1] + lvDuplicatedID[i];
        }
        else
            pos = offset + lvDuplicatedID[i];
        
        items[pos] = nonSubtotalElems[i];
    }
    offset+=elementIds.size();
    
    
    lvCount.clear();
    lvDuplicatedID.clear();
    MEM_GOVERNOR_RELEASE((count+elementIds.size())*sizeof(int),(count+elementIds.size())*sizeof(int))
    
    
    elementIds.clear();
    nonSubtotalElems.clear();
    MEM_GOVERNOR_RELEASE(2*lnItem*sizeof(int),2*lnItem*sizeof(int))
    
    if (_subtotalPosition != DssSubtotalsPositionFirst)
    {
        for(int i=0;i<subtotalElems.size();++i)
            items[i+offset] = subtotalElems[i];
    }
    
    return hr;
}

bool DSSAttributeFormSorter::sortUnitElements(vector<int>& elements)
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

bool DSSAttributeFormSorter::operator()(int a, int b)
{
    int Locale = 0;
    if (_model != NULL)
        _model->getUnitLocale(_unitID, Locale);

	int order = _column->Compare(a, b, _isAscending ? _isNullOnTop : !_isNullOnTop, Locale, false, false);
	return _isAscending ? order < 0 : order > 0;
}

int DSSAttributeFormSorter::buildBucket(DSSXTabHeader* header)
{
    _header = header;
    int hr = S_OK;
    
    if(_buckets.size()==0)
	{
        vector<int>& units=header->getUnits();
        for(int i=0;i<units.size();++i)
        {
            if(units[i]==_unitID)
            {
                _unitIndex=i;
                break;
            }
        }
        
        
		DSSTabularAttribute* attribute=(DSSTabularAttribute*)(_model->Item(_unitID));
		int formIndex;
		hr = attribute->FindFormWithLocale(_unitID, _formID, 0, formIndex);
		CHECK_HR;
        
		hr = attribute->Item(formIndex, &_column);
		CHECK_HR;
        
        for(int i = 0; i < count; ++i)
        {
            mvUnitKeys.push_back(_header->getUnitKey(i, _unitIndex));
        }
        
        //zhyang, 02/23/2012, 545632, there may be a join null in the attribute.
        //For the attribute that doesn't contian a join null,
        //the index out of range will be handeled in _column->Compare() function.
		//int count=_column->getRowCount() + 1;
        MEM_GOVERNOR_RESERVE(count * sizeof(int) * 2, count * sizeof(int) * 2) //for both _buckets & ranks
		int i;
		for(i=0;i<count;++i)
			_buckets.push_back(i);
		vector<int> ranks=_buckets;
        MEM_GOVERNOR_COMMIT(count * sizeof(int) * 2)
		_xtab_sort(&ranks[0], &ranks[0] + count, *this);
		int rank=0;
		if (count > 0)		//477477, it's a runtime unit, only has total
		{
			_buckets[ranks[0]]=0;
			for(i=1;i<count;++i)
			{
				if(mvUnitKeys[ranks[i]]<0 || mvUnitKeys[ranks[i-1]]<0 || _column->Order(ranks[i], ranks[i-1]))  // if differ from the last element
					rank++;
				_buckets[ranks[i]]=rank;
			}
		}
		
		ranks.clear();
        MEM_GOVERNOR_RELEASE(count * sizeof(int)* 2, count * sizeof(int)* 2)
	}
    
    return hr;
}

bool DSSAttributeFormSorter::OnlySortDisplay()
{
    return false;
}

int DSSAttributeFormSorter::sortDisplay(DSSXTabHeader* header, vector<int>& items)
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
}


