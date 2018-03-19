#import "DSSSorter.h"
#import "DSSSortDefinition.h"
#import "DSSXTabHeader.h"
#import "DSSDataColumn.h"
#import "DSSSort_includes.h"
#import "DSSConsolidationSorter.h"
#import "DSSTabularConsolidation.h"
#import "DSSTabularData.h"
#import "DSSDataSource.h"
//#import "DSSDataModelXTabImpl.h"
#import <map>
#import "DSSGUnitContainer.h"

DSSConsolidationSorter::DSSConsolidationSorter(DSSSortDefinition* definition):_consolidation(NULL)
{
	_unitID=definition->unitID;
	_isAscending=definition->isAscending;
	_isNullOnTop=definition->isNullOnTop;
	_subtotalPosition=definition->subtotalPosition;
	_model=definition->model;
    
    mbNewRWD = _model->getIsNewRWD();
/*    if(dynamic_cast<DSSDataModelXTabImpl *>(_model) != NULL)
    {
        mbNewRWD = false;
    }
    else
    {
        mbNewRWD = true;
    }*/
}

DSSConsolidationSorter::~DSSConsolidationSorter() {
	int lDataSize = _buckets.size()*sizeof(int);
	_buckets.clear();
MEM_GOVERNOR_RELEASE(lDataSize, lDataSize)	
}

int DSSConsolidationSorter::sort(DSSXTabHeader* header, vector<int>& items)
{
	int hr = S_OK;
	
	if(items.size()<=1)
		return hr;
    
    

	_consolidation=(DSSTabularConsolidation*)(_model->Item(_unitID));
    
    //xiazhou, 2013-07-26, TQMS 765743.
    int notSet = -1;//old RWD logic doesn't use GUnitContainer, and null is not allowed in DE in those cases.
    if(!mbNewRWD)
        _globalRowCount = notSet;
    else
    {
        int unitGlobalID = notSet;
        int globalRowCount = notSet;
        DSSGUnitContainer* gContainer = _model->getGUnitContainer();
        if(gContainer)
        {
            hr = gContainer->GetGlobalUnitIDByGUID(_consolidation->getGUID(), &unitGlobalID);
            if (hr == S_OK) {
                //Third parameter has to be false to get right global row count
                hr = gContainer->GetGlobalRowCount(unitGlobalID, &globalRowCount, false);
                CHECK_HR
            }
            else {
                if (!_consolidation->IsDerivedElement()) {
                    // new DE maybe can't find the GUID in global up table
                    hr = S_OK;
                }
            }
        }
        this->_globalRowCount = globalRowCount;
    }

    int count;
    if(!mbNewRWD)
        count = _consolidation->CountRows();
    else
    {
        if(_consolidation->IsDerivedElement())
            count = _consolidation->GetMaxKey() + 1;
        else
            //tliao, 955575, 2014/08/26, need to consider compound null key
            count = _consolidation->CountRows() + 1;
    }
    
	if(_buckets.size()==0)
	{
MEM_GOVERNOR_RESERVE(count * sizeof(int), count * sizeof(int))
		int i;
        for(i=0;i<count;++i)
            _buckets.push_back(i);
MEM_GOVERNOR_COMMIT(count * sizeof(int))	
		vector<int> ranks=_buckets;
		_xtab_sort(&ranks[0], &ranks[0] + count, *this);
		//for(i=0;i<count;++i)
		//	_buckets[ranks[i]]=i;
        int rank = 0;
		int prevID = -1;
		for(i=0;i<count;++i)
		{
			int id = ranks[i];
			if (prevID >= 0 && ((*this)(id, prevID) || (*this)(prevID, id) /*== false*/))
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
	}	

	vector<vector<int>*> bucketList;
	vector<int> subtotalBucket;
	int lDataReserveSize = count * sizeof(int);
MEM_GOVERNOR_RESERVE(lDataReserveSize, lDataReserveSize)	
	int i, n;
	for(i=0, n=count;i<n;++i)
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

	for(i=0;i<items.size();++i)
	{
		int elementID=header->getUnitKey(items[i],unitIndex);
		if(elementID>=0) // normal element
		{
			int rank=_buckets[elementID];		//TQMS 432179
			bucketList[rank]->push_back(items[i]);
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

	if(_isAscending)
	{
		for(i=0;i<bucketList.size();++i)
		{
			vector<int>* list=bucketList[i];
			for(int j=0;j<list->size();++j)
				items.push_back((*list)[j]);
		}
	}
	else
	{
		for(i=bucketList.size()-1;i>=0;--i)
		{
			vector<int>* list=bucketList[i];
			for(int j=0;j<list->size();++j)
				items.push_back((*list)[j]);
		}
	}

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

bool DSSConsolidationSorter::operator()(int a, int b)
{
    //lwang, 12/21/2012, to support global lookup in new RWD engine
    
    //this function is called to build buckets for consolidation elements. But If global lookup is used, 
    //sometimes we can't get XTabConsolidationElement.
    
    XTabConsolidationElement *lpEa = _consolidation->getElement(a);
    XTabConsolidationElement *lpEb = _consolidation->getElement(b);
    
    if (lpEa != NULL && lpEb != NULL)
    {
        //zhyang, when two element have the same ordinal, we should sort according to the result of previous result
        /*if(_consolidation->getElement(a)->ordinal == _consolidation->getElement(b)->ordinal)
        {
            int posa = _firstPosInItems[a];
            int posb = _firstPosInItems[b];
            
            if(posa != -1 &&posb != -1)
                return posa < posb;
        }*/
        //xiazhou, 2013-07-26, TQMS 765743. If global key equals _globalRowCount, then it's null.
        if(a == _globalRowCount)
            return _isNullOnTop ? true : false;
        
        if(b == _globalRowCount)
            return _isNullOnTop ? false : true;
        
        return _consolidation->getElement(a)->ordinal < _consolidation->getElement(b)->ordinal;
    }
    else
    {
        return lpEa == NULL && lpEb != NULL ? true: false;  //treat 
        
    }
}
