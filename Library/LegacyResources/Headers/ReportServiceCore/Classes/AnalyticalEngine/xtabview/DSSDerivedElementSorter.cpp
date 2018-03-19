#import "DSSSorter.h"
#import "DSSSortDefinition.h"
#import "DSSXTabHeader.h"
#import "DSSDataColumn.h"
#import "DSSSort_includes.h"
#import "DSSDerivedElementSorter.h"
#import "DSSTabularAttribute.h"
#import "DSSTabularConsolidation.h"
#import "DSSTabularData.h"
#import "DSSDataSource.h"
#import <map>
#import "DSSGUnitContainer.h"

DSSDerivedElementSorter::DSSDerivedElementSorter(DSSSortDefinition* definition, EnumDSSSortType sortType)
{
	_model=definition->model;
	_formID=definition->formID;
	_unitID=definition->unitID;
	_isAscending=definition->isAscending;
	_isNullOnTop=definition->isNullOnTop;
	_subtotalPosition=definition->subtotalPosition;
    _globalRowCount = -1; //813598
    _sortType = sortType;
}

DSSDerivedElementSorter::~DSSDerivedElementSorter() {
	int lDataSize = _buckets.size();
	_buckets.clear();
MEM_GOVERNOR_RELEASE(lDataSize, lDataSize)	
}

int DSSDerivedElementSorter::sort(DSSXTabHeader* header, vector<int>& items)
{
	int hr = S_OK;
	
	if(items.size()<=1)
		return hr;
 
	_consolidation=(DSSTabularConsolidation*)(_model->Item(_unitID));
    
    //xiazhou, 2013-07-26, TQMS 765743.
    int notSet = -1;//old RWD logic doesn't use GUnitContainer, and null is not allowed in DE in those cases.
    int unitGlobalID = notSet;
    int globalRowCount = notSet;
    if(_model->getIsNewRWD()){
        DSSGUnitContainer* gContainer = _model->getGUnitContainer();
        if(gContainer)
        {
            hr = gContainer->GetGlobalUnitIDByGUID(_consolidation->getGUID(), &unitGlobalID);
            CHECK_HR
            //Third parameter has to be false to get right global row count
            hr = gContainer->GetGlobalRowCount(unitGlobalID, &globalRowCount, false);
            CHECK_HR
        }
        this->_globalRowCount = globalRowCount;
    }
    
	if(_buckets.size()==0)
	{
		DSSTabularAttribute* attribute=(DSSTabularAttribute*)_consolidation->getChildUnit();
		int formIndex;
		hr = attribute->FindFormWithLocale(_unitID, _formID, 0, formIndex);
		CHECK_HR;
		
		hr = attribute->Item(formIndex, &_column);
		CHECK_HR;
		
		EnumDSSDataType formType=_column->getDataType();
		_stringTyped=(formType==DssDataTypeChar || formType==DssDataTypeVarChar
					  || formType==DssDataTypeLongVarChar || formType==DssDataTypeBinary
					  || formType==DssDataTypeVarBin || formType==DssDataTypeLongVarBin
					  || formType==DssDataTypeNChar || formType==DssDataTypeNVarChar)
                        //zhyang, TQMS 545269, tread UTF-8 char as string
                      || formType==DssDataTypeUTF8Char;						
		/*ctang 2011-12-27
         For new RWD, the DE element distribution is a little different from normal DE
         Suppose there are N elements in the CS unit, the key of those elements may be greater than N.
         For example, there are 5 elements in DE totally, but the global key of the branch element may be 101 due to the global lookup table.
         So I change a little bit on the bucket sort, follow the server side logic:
         the bucket size should be lMaxKey, while the size of Rank should be count.
         In this way we can get the correct result, but we have the risk of the memory cost
         Sometimes we will have a larget buckets, but only a few of them contain data.
         Maybe need optimization in the future.
         */
        
        int count = _consolidation->CountRows();
		int lMaxKey=_consolidation->GetMaxKey() + 1;
MEM_GOVERNOR_RESERVE(lMaxKey * sizeof(int), lMaxKey * sizeof(int))
		int i;
		_buckets.resize(lMaxKey);
        if (lMaxKey > 0)
            memset(&_buckets[0], -1, sizeof(int) * lMaxKey);
MEM_GOVERNOR_COMMIT(count * sizeof(int))
		vector<int> ranks;
        for (i = 0; i < count; i++)
        {
            int lGlobal = _consolidation->TranslateLocalToGlobal(i);
            if (lGlobal < 0 || lGlobal >= lMaxKey)
            {
                assert(false);
                return E_FAIL;
            }
            ranks.push_back(lGlobal);
        }
		_xtab_sort(&ranks[0], &ranks[0] + count, *this);
        
        int rank = 0;
		int prevID = -1;
		for(i=0;i<count;++i)
		{
			int id = ranks[i];
            //zhyang, 768455, check whether two elements are equal
			if (prevID >= 0 && ((*this)(id, prevID) != (*this)(prevID, id) /*== false*/))
			{
				rank++;
				prevID = id;
			}
			else if (prevID < 0)
			{
				prevID = id;
			}
			_buckets[id]=rank;
		}
        
/*
        int rank = 0;
        _buckets[ranks[0]]=rank;
        for (int i = 1; i < count; ++i)
        {
            XTabConsolidationElement* pa=_consolidation->getElement(ranks[i-1]);
            XTabConsolidationElement* pb=_consolidation->getElement(ranks[i]);
            
            int ia=_consolidation->getChildElementKey(ranks[i-1]);
            int ib=_consolidation->getChildElementKey(ranks[i]);
            
            //both are branch
            if(pa->branch && pb->branch)
            {
                if(_stringTyped)
                {
                    //if(strcasecmp(pa->value.c_str(), pb->value.c_str()) != 0)
                    if(strcmp(pa->value.c_str(), pb->value.c_str()) != 0)
                        ++rank;
                }
                else
                {
                    ++rank;
                }
            }
            
            //both are not branch
            else if(!pa->branch && !pb->branch)
            {
                if(_column->Order(ia, ib) != 0)
                    ++rank;
            }
            
            //one is branch, the other is not
            else if (_stringTyped)
            {
                if(pa->branch)
                {
                    int result=_column->CompareToData(ib, (unsigned char*)(pa->value.c_str()), (int)pa->value.length(), DssDataOk, _isNullOnTop, true);
                    if(result != 0)
                        ++rank;
                }
                
                if(pb->branch)
                {
                    int result=_column->CompareToData(ia, (unsigned char*)(pb->value.c_str()), (int)pb->value.length(), DssDataOk, _isNullOnTop, true);
                    if(result != 0)
                        ++rank;
                }
            }
            else
            {
                //zhyang, one is branch and one is not. if the data type is not string, branch element will be treated as 
                //null, so we need to check whether the other element is null.
                DSSDataFlag lFlag;
                if(pa->branch)
                {
                    hr = _column->getFlag(ib, lFlag);
                }
                else if(pb->branch)
                {
                    hr = _column->getFlag(ia, lFlag);
                }
                CHECK_HR;
                if(hr == DssDataOk)
                    ++rank;
            }
            _buckets[ranks[i]]=rank;
        }
 */
//		for(i=0;i<count;++i)
//			_buckets[ranks[i]]=i;
	}	

	vector<vector<int>*> bucketList;
	vector<int> subtotalBucket;
	// distribute items into the buckets
	int lDataReserveSize = _buckets.size() * sizeof(int);
MEM_GOVERNOR_RESERVE(lDataReserveSize, lDataReserveSize)		
	int i;
	for(i=0;i<_buckets.size();++i)
		bucketList.push_back(new vector<int>);
MEM_GOVERNOR_COMMIT(lDataReserveSize)	
	int unitIndex=0;
	vector<int>& units=header->getUnits();
	for(i=0;i<units.size();++i)
	{
		if(units[i]==_unitID)
		{
			unitIndex=i;
			break;
		}
	}
	
    vector<int> nullBucket;
	for(i=0;i<items.size();++i)
	{
		int elementID=header->getUnitKey(items[i],unitIndex);
		if(elementID>=0) // normal element
		{
            //xiazhou, 785587. Deal with join null with local_key < 0.
            //for join null elements with local key >= 0, they are ranked
            //in operator()
            if(elementID == _globalRowCount)
            {
                nullBucket.push_back(items[i]);
            }
            else
            {
                int rank=_buckets[elementID];
                bucketList[rank]->push_back(items[i]);
            }
		}
		else // subtotal
		{
			subtotalBucket.push_back(items[i]);
		}
	}
    
	items.clear();
    
	// collect items from buckets
	if(_subtotalPosition==DssSubtotalsPositionFirst)
	{
		for(i=0;i<subtotalBucket.size();++i)
			items.push_back(subtotalBucket[i]);
	}
    
    if(_isNullOnTop)
        items.insert(items.end(), nullBucket.begin(), nullBucket.end());
    
	for(i=0;i<bucketList.size();++i)
	{
		vector<int>* list=bucketList[i];
		for(int j=0;j<list->size();++j)
			items.push_back((*list)[j]);
	}
    
    if(!_isNullOnTop)
        items.insert(items.end(), nullBucket.begin(), nullBucket.end());

	if(_subtotalPosition!=DssSubtotalsPositionFirst)
	{
		for(i=0;i<subtotalBucket.size();++i)
			items.push_back(subtotalBucket[i]);
	}
	
	for(i=0;i<bucketList.size();++i)
		delete bucketList[i];
MEM_GOVERNOR_RELEASE(lDataReserveSize, lDataReserveSize)	
	return hr;
}

bool DSSDerivedElementSorter::sortUnitElements(vector<int>& elements)
{
	if(elements.size()<=1)
		return true;
	_consolidation=(DSSTabularConsolidation*)(_model->Item(_unitID));
	DSSTabularAttribute* attribute=(DSSTabularAttribute*)_consolidation->getChildUnit();
	int formIndex;
	if(attribute->FindFormWithLocale(_unitID, _formID, 0, formIndex)!=S_OK)
		return false;
	if(attribute->Item(formIndex, &_column)!=S_OK)
		return false;
		
	EnumDSSDataType formType=_column->getDataType();
	_stringTyped=(formType==DssDataTypeChar || formType==DssDataTypeVarChar
				  || formType==DssDataTypeLongVarChar || formType==DssDataTypeBinary
				  || formType==DssDataTypeVarBin || formType==DssDataTypeLongVarBin
				  || formType==DssDataTypeNChar || formType==DssDataTypeNVarChar);						
		
	_xtab_sort(&elements[0], &elements[0] + elements.size(), *this);
	return true;
}

bool DSSDerivedElementSorter::operator()(int a, int b)
{
	XTabConsolidationElement* pa=_consolidation->getElement(a);
	XTabConsolidationElement* pb=_consolidation->getElement(b);
    
    if(pa->ordinal==-1 && pb->ordinal==-1)
		return false;
	if(pa->ordinal==-1)
		return false;
	if(pb->ordinal==-1)
		return true;
    
    //xiazhou, 2013-07-26, TQMS 765743. If global key equals _globalRowCount, then it's null.
    if(a == _globalRowCount)
        return _isNullOnTop ? true : false;
    
    if(b == _globalRowCount)
        return _isNullOnTop ? false : true;
    
    
    bool aIsNull = false, bIsNull = false;
    int ia=_consolidation->getChildElementKey(a);
	int ib=_consolidation->getChildElementKey(b);
    
    if(!pa->branch)
    {
        DSSDataFlag lFlag;
        int hr = _column->getFlag(ia, lFlag);
        
        if(hr == S_OK)
        {
            if(lFlag == DssDataNull || lFlag == DssDataMissing)
                aIsNull = true;
        }
    }
    
    if(!pb->branch)
    {
        DSSDataFlag lFlag;
        int hr = _column->getFlag(ib, lFlag);
        if(hr == S_OK)
        {
            if(lFlag == DssDataNull || lFlag == DssDataMissing)
                bIsNull = true;
        }
    }
    
    if(aIsNull || bIsNull)
    {
        return  aIsNull ? _isNullOnTop : !_isNullOnTop;
    }
    
//    xiazhou, 07/17/2014, distinguish form sort with default sort.
    //default sort, use ordinal to sort. When ordinal is the same, use attribute sort to break a tie.
    if(_sortType == DssSortDerivedElementDefault)
    {
        if(pa->ordinal == pb->ordinal)
        {
            if(pa->branch || pb->branch)
            {
                return false;
            }
            
            return _isAscending ? _column->Order(ia, ib, _isNullOnTop) < 0 : _column->Order(ia, ib, _isNullOnTop) >= 0;
        }
        else
            return pa->ordinal < pb->ordinal;
        
    }
    else if(_sortType == DssSortDerivedElementForm)//form sort, compare string value if possible.
    {
        if(pa->branch && pb->branch)
        {
            if(_stringTyped)
            {
                int Locale = 0;
                if (_model != NULL)
                    _model->getUnitLocale(_unitID, Locale);
                return DSSAEHelper::my_strcasecmp(pa->value.c_str(), pb->value.c_str(), Locale)<0 ? _isAscending : !_isAscending;
            }
            else
            {
                //zhyang, when both are branch but not string typed, use ordinal to break tie
                return pa->ordinal < pb->ordinal ? _isAscending : !_isAscending;
            }
        }
        
        //both are not branch
        if(!pa->branch && !pb->branch)
        {
            return _isAscending ? _column->Order(ia, ib, _isNullOnTop) < 0 : _column->Order(ia, ib, _isNullOnTop) >= 0;
        }
        
        //one is branch, the other is not
        if (_stringTyped)
        {
            if(pa->branch)
            {
                int result=_column->CompareToData(ib, (unsigned char*)(pa->value.c_str()), (int)pa->value.length(), DssDataOk, _isNullOnTop, true, false);
                return _isAscending ? result>0 : result<=0;
            }
            
            if(pb->branch)
            {
                int result=_column->CompareToData(ia, (unsigned char*)(pb->value.c_str()), (int)pb->value.length(), DssDataOk, _isNullOnTop, true, false);
                return _isAscending ? result<0 : result>=0;
            }
        }
        else
        {
            //TQMS 428461, for non-string, branch element is treated as NULL
            return _isNullOnTop ? pa->branch : !pa->branch;
        }
    }
    else
    {
        assert(false);
    }
    
	return true;
}
