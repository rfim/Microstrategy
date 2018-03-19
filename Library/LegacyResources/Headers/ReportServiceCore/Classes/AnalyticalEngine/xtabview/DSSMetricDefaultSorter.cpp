#import "DSSSorter.h"
#import "DSSSortDefinition.h"
#import "DSSXTabHeader.h"
#import "DSSSort_includes.h"
#import "DSSMetricDefaultSorter.h"
#import "DSSDataModelXTabImpl.h"
#import "DSSTabularMetrics.h"

DSSMetricDefaultSorter::DSSMetricDefaultSorter(DSSSortDefinition* definition)
{
	_unitID=definition->unitID;
	_subtotalPosition=definition->subtotalPosition;
	_model=(DSSDataModelXTabImpl*)definition->model;
}

DSSMetricDefaultSorter::~DSSMetricDefaultSorter() {
}

int DSSMetricDefaultSorter::sort(DSSXTabHeader* header, vector<int>& items)
{
	int hr = S_OK;
	if(items.size()<=1 || !header->HasMetric())
		return hr;
	_header=header;

	_xtab_sort(&items[0], &items[0] + items.size(), *this);	
	
	return hr;
}

bool DSSMetricDefaultSorter::operator ()(int a, int b)
{
	int ma=_header->getMetricID(a);
	int mb=_header->getMetricID(b);
	return ma<mb;
}

	/*
			var i:int, j:int;
			var buckets:Array=new Array;
			var bucketList:Array=new Array;   // rank => list of items with the same rank
			var subtotalBucket:Array=new Array;
			
			for(i=0;i<header.layout.metricIDs.length;++i)
				buckets[header.layout.metricIDs[i]]=i;

			for(i=0;i<items.length;++i)
			{
				var item:XTabHeaderItem=items[i];
					var rank:int=buckets[item.metricID];
					if(bucketList[rank]==undefined)
						bucketList[rank]=new Array();
					bucketList[rank].push(item);
			}

			// collect items from buckets
			var sortedItems:Array=new Array();
			if(_subtotalPosition==EnumDSSSubtotalsPosition.DssSubtotalsPositionFirst)
			{
				for(i=0;i<subtotalBucket.length;++i)
					sortedItems.push(subtotalBucket[i]);
			}
			for(i=0;i<bucketList.length;++i)
			{
				if(bucketList[i]!=undefined)
				{
					for(j=0;j<bucketList[i].length;++j)
						sortedItems.push(bucketList[i][j]);
				}
			}
			if(_subtotalPosition!=EnumDSSSubtotalsPosition.DssSubtotalsPositionFirst)
			{
				for(i=0;i<subtotalBucket.length;++i)
					sortedItems.push(subtotalBucket[i]);
			}
			
			return sortedItems;

	return true;
}

bool DSSMetricDefaultSorter::operator()(int a, int b)
{
	return true;
}


		override public function sort(header:DSSXTabHeader, items:Array):Array
		{
			// distribute items into the buckets
			var i:int, j:int;
			var buckets:Array=new Array;
			var bucketList:Array=new Array;   // rank => list of items with the same rank
			var subtotalBucket:Array=new Array;
			
			for(i=0;i<header.layout.metricIDs.length;++i)
				buckets[header.layout.metricIDs[i]]=i;

			for(i=0;i<items.length;++i)
			{
				var item:XTabHeaderItem=items[i];
					var rank:int=buckets[item.metricID];
					if(bucketList[rank]==undefined)
						bucketList[rank]=new Array();
					bucketList[rank].push(item);
			}

			// collect items from buckets
			var sortedItems:Array=new Array();
			if(_subtotalPosition==EnumDSSSubtotalsPosition.DssSubtotalsPositionFirst)
			{
				for(i=0;i<subtotalBucket.length;++i)
					sortedItems.push(subtotalBucket[i]);
			}
			for(i=0;i<bucketList.length;++i)
			{
				if(bucketList[i]!=undefined)
				{
					for(j=0;j<bucketList[i].length;++j)
						sortedItems.push(bucketList[i][j]);
				}
			}
			if(_subtotalPosition!=EnumDSSSubtotalsPosition.DssSubtotalsPositionFirst)
			{
				for(i=0;i<subtotalBucket.length;++i)
					sortedItems.push(subtotalBucket[i]);
			}
			
			return sortedItems;
		}
	}
}
*/
