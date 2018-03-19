#import "DSSSorter.h"
#import "DSSSortDefinition.h"
#import "DSSXTabHeader.h"
#import "DSSDataColumn.h"
#import "DSSSort_includes.h"
#import "DSSCustomGroupSorter.h"
#import "DSSTabularCustomGroup.h"
#import "DSSTabularData.h"
#import "DSSDataSource.h"
#import <map>

DSSCustomGroupSorter::DSSCustomGroupSorter(DSSSortDefinition* definition)
{
	_unitID=definition->unitID;
	_isAscending=definition->isAscending;
	_isNullOnTop=definition->isNullOnTop;
	_subtotalPosition=definition->subtotalPosition;
	_model=definition->model;
}

DSSCustomGroupSorter::~DSSCustomGroupSorter() {
}

/*
int getMaxOrdinal(DSSTabularCustomGroup* ipCustomGroup)
{
	int lMax = -1;
	
	int lnElements = ipCustomGroup->CountRows();
	for (int i = 0; i < lnElements; i++)
	{
		int lOrdinal = ipCustomGroup->getElement(i)->ordinal;;
		if (lOrdinal > lMax)
			lMax = lOrdinal;
	}
	return lMax;
}
 */

int DSSCustomGroupSorter::sort(DSSXTabHeader* header, vector<int>& items)
{
	int hr = S_OK;
	
	if(items.size()<=1)
		return hr;

	DSSTabularCustomGroup* customGroup=(DSSTabularCustomGroup*)(_model->Item(_unitID));
	vector<vector<int>*> bucketList;
	vector<int> subtotalBucket;
    vector<int> joinNullBucket;
	
    //hyan,2012/05/17
    int lnElements = customGroup->CountRows();
    int lDataReserveSize = lnElements * sizeof(int);
    
    MEM_GOVERNOR_RESERVE(lDataReserveSize, lDataReserveSize)	
	int i = 0;
    
	for(i = 0; i < lnElements + 1; ++i)
    {
		bucketList.push_back(new vector<int>);
    }
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
            //ctang: use getOrdinalloca
			int rank=customGroup->getOrdinal(elementID) + 1;
            if (rank >= 0)
            {
                //790238, in old RWD the ordinal can be larger than lnElements
                if(rank >= bucketList.size())
                {
                    for(int i = bucketList.size(); i <= rank; i++)
                        bucketList.push_back(new vector<int>);
                }
                bucketList[rank]->push_back(items[i]);
            }
            else
                joinNullBucket.push_back(items[i]);
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
    
    if (_isNullOnTop)
    {
		for(i=0;i<joinNullBucket.size();++i)
			items.push_back(joinNullBucket[i]);
    }
    
    for(i=0;i<bucketList.size();++i)
    {
        vector<int>* list=bucketList[i];
        for(int j=0;j<list->size();++j)
            items.push_back((*list)[j]);
    }


    if (!_isNullOnTop)
    {
		for(i=0;i<joinNullBucket.size();++i)
			items.push_back(joinNullBucket[i]);
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
