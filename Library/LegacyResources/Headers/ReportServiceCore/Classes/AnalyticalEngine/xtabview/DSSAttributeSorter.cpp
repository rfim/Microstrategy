//
//  DSSAttributeSorter.cpp
//  ReportServiceCore
//
//  Created by Zhuo Yang on 6/21/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#include <iostream>
#include "DSSAttributeSorter.h"
#import "DSSDataSource.h"
#import "DSSXTabHeader.h"
#import "DSSSortDefinition.h"
#import "DSSRWDataModel.h"
#import "DSSGlobalLookUpTable.h"
#import "DSSGUnitContainer.h"
#import "DSSTabularData.h"
#import "DSSViewDataSet.h"

DSSAttributeSorter::DSSAttributeSorter(DSSSortDefinition* definition)
{
    _unitID=definition->unitID;
    _unitIDinContainer = definition->unitIDInContianer;
    _bucketID = definition->bucketID;
	_subtotalPosition=definition->subtotalPosition;
    
    //zhyang, get sort result form attribute
    //_buckets=definition->model->getBuckets(_unitID, _bucketID);
    
    //DSSViewDataSet *lpViewDataSet = (DSSViewDataSet *)definition->model;
   // ((DSSTabularData *)(definition->model))->getViewDataSet(&lpViewDataSet);
    
    //DSSGUnitContainer * lpGlobalLookupContainer = lpViewDataSet->getTabularData()->getGUnitContainer();
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

DSSAttributeSorter::~DSSAttributeSorter()
{
    
}

int DSSAttributeSorter::sort(DSSXTabHeader* header, vector<int>& items)
{
    int hr = S_OK;
    if (_buckets.size() == 0) {
        return hr;
    }
    
    //vector<vector<int>*> bucketList;
    vector<int> bucketList;
	vector<int> subtotalBucket;
    map<int, vector<int> > extraLink;
	// distribute items into the buckets
	int lDataReserveSize = _buckets.size() * sizeof(int);
MEM_GOVERNOR_RESERVE(lDataReserveSize, lDataReserveSize)	
	int i;
	//for(i=0;i<_buckets.size();++i)
	//	bucketList.push_back(new vector<int>);
    bucketList.resize(_buckets.size());
    memset(&bucketList[0], -1, sizeof(int) * _buckets.size());
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
			int rank=_buckets[elementID];
            if (rank >= bucketList.size())
            {
                //add defensive code.
                //found in TQMS 776550. step: 0) enalbe malloc guard; 1) switch to layout 1; 2) swicth to layout 2 and click "Business"; 3)swicth layout1 and edit Revenue on the first row to "100"
                return E_FAIL;
            }
            if(-1 == bucketList[rank])
                bucketList[rank] = items[i];
            else if(-2 == bucketList[rank])
            {
                map<int, vector<int> >::iterator iter = extraLink.find(rank);
                if (iter != extraLink.end()) {
                    (iter->second).push_back(items[i]);
                }
                else
                {
                    assert(false);
                }
            }
            else
            {
                vector<int> extraBucket;
                extraBucket.push_back(bucketList[rank]);
                extraBucket.push_back(items[i]);
                extraLink.insert(map<int, vector<int> >::value_type(rank, extraBucket));
                bucketList[rank] = -2;
            }
			//bucketList[rank]->push_back(items[i]);
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
    
	for(i=0;i<bucketList.size();++i)
	{
		/*vector<int>* list=bucketList[i];
		for(int j=0;j<list->size();++j)
			items.push_back((*list)[j]);
         */
        if(bucketList[i] >= 0)
        {
            items.push_back(bucketList[i]);
        }
        else if(-2 == bucketList[i])
        {
            map<int, vector<int> >::iterator iter = extraLink.find(i);
            if(iter != extraLink.end())
            {
                for(int j = 0; j < (iter->second).size(); ++j)
                    items.push_back((iter->second)[j]);
            }
        }
	}
    
	if(_subtotalPosition!=DssSubtotalsPositionFirst)
	{
		for(i=0;i<subtotalBucket.size();++i)
			items.push_back(subtotalBucket[i]);
	}
	
	//for(i=0;i<bucketList.size();++i)
	//	delete bucketList[i];
    MEM_GOVERNOR_RELEASE(lDataReserveSize, lDataReserveSize)	
	return hr;
}