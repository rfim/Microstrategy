#import "DSSSortDefinition.h"
#import "DSSDataModelBase.h"
#import "DSSSorter.h"
#import "DSSAttributeFormSorter.h"
#import "DSSMetricSorter.h"
#import "DSSMetricDefaultSorter.h"
#import "DSSDerivedElementSorter.h"
#import "DSSConsolidationSorter.h"
#import "DSSCustomGroupSorter.h"
#import "DSSAttributeSorter.h"
#import "DSSRecursiveLookupTableSorter.h"

DSSSortDefinition::DSSSortDefinition(DSSTabularData* m)
{
	model=m;
	_sorter=NULL;
    subtotalID = -1;
    mForMicroChart = false;
    
    //fali, 820583, set null position to DssOLAPNullPositionLast consistent with server-side logic
    mNullPosition = DssOLAPNullPositionLast;
}

DSSSortDefinition::~DSSSortDefinition()
{
	if(_sorter)
	{
		delete _sorter;
		_sorter=NULL;
	}
}

DSSSorter* DSSSortDefinition::getSorter()
{
	if(_sorter==NULL)
	{
		switch(sortType)
		{
			case DssSortAttributeForm:
				_sorter=new DSSAttributeFormSorter(this);
				break;
			case DssSortMetric:
				_sorter=new DSSMetricSorter(this);
				break;
			case DssSortReserved:
				_sorter=new DSSMetricDefaultSorter(this);
				break;
			case DssSortConsolidationDefault:
				if(formID>=0)
					_sorter=new DSSDerivedElementSorter(this, DssSortDerivedElementDefault);
				else
					_sorter=new DSSConsolidationSorter(this);
				break;
			case DssSortCustomGroupDefault:
				_sorter=new DSSCustomGroupSorter(this);
				break;
            case DssSortAttributeOpt:
                _sorter=new DSSAttributeSorter(this);
                break;
            case DssSortDerivedElementForm:
            case DssSortDerivedElementDefault:
                _sorter = new DSSDerivedElementSorter(this, sortType);
                break;
            case DssSortRecursiveAttributeDefault:
            case DssSortRecursiveAttributeForm:
                _sorter = new DSSRecursiveLookupTableSorter(this, sortType);
                break;
			case DssSortSubtotalsPosition: // implicitly done in SortAttributeForm
			case DssSortAttributeDefault:
			case DssSortMetricHierarchical:
			case DssSortGroupByMetric:
			case DssSortGroupBySubtotal:
			case DssSortDimensionForm:
			case DssSortDimensionDefault:
			case DssSortValue:
			case DssSortCustomGroupForm:
			default:
				// sortType is not supported
				return NULL;
		}
	}
	return _sorter;
}

EnumDSSSubtotalsPosition DSSSortDefinition::getSubtotalPosition()
{
	return subtotalPosition;
}

int DSSSortDefinition::get_AttributeAndForm(Int32 *pAttr, Int32 *pForm)
{
	if(sortType == DssSortAttributeForm || sortType == DssSortAttributeColumn ||
	   sortType == DssSortDerivedElementDefault||
	   sortType == DssSortDerivedElementForm)
	{
		*pAttr = unitID;
		*pForm = formID;
		return S_OK;
	}
    
	if(sortType == DssSortConsolidationDefault ||
	   sortType == DssSortCustomGroupDefault	||
	   sortType == DssSortCustomGroupForm)
	{
		*pAttr = unitID;
		*pForm = 0;			//XW there is no form per se in consolidation
		return S_OK;
	}
    
	return E_FAIL;
}
