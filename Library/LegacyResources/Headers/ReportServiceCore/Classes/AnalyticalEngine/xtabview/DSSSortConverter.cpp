#import "DSSSort.h"
#import "DSSSorts.h"
#import "DSSAxis.h"
#import "DSSSortConverter.h"
#import "DSSTabularUnit.h"
#import "DSSTabularAttribute.h"
#import "DSSSortDefinition.h"
#import "DSSDataModelBase.h"
#import "DSSDataModelXTabImpl.h"
#import "DSSXTabHeader.h"
#import "DSSXTabView.h"
#import "DSSTemplate.h"
#import "DSSTabularMetrics.h"
#import "DSSTabularConsolidation.h"
#import "DSSDataColumn.h"
#import "DSSTabularData.h"
#import "DSSViewDataSet.h"
#import "DSSGUnitContainer.h"
#import "DSSCalculationEvent.h"
#import "DSSMetric.h"
#import "DSSXTabRecursiveLookupTable.h"
#include <algorithm>
#include "SimpleBuffer.h"

DSSSortConverter::DSSSortConverter(DSSDataModelBase *ipDataModelBase, DSSTabularData* model):mbIsNullOnTop(true)
{
    mbNewRWD = false;
	_model=model;
	generateAttributeDefaultSorts();
    
    DSSDataModelXTabImpl *lpXTabModel = dynamic_cast<DSSDataModelXTabImpl*>(ipDataModelBase);
    if (lpXTabModel)
    {
        mpXTabView=lpXTabModel->getXTabView();
        mbIsNullOnTop = lpXTabModel->getNullOnTop();
        lpXTabModel->getNullReplacement(mNullReplacement);
        mpTemplate = lpXTabModel->getTemplate();
    }else{
        mpXTabView = NULL;
        mpTemplate = NULL;
    }
}

DSSSortConverter::DSSSortConverter(DSSViewDataSet * ipViewDataSet, DSSXTabView *ipXTabView):mbIsNullOnTop(true)
{
    mbNewRWD  = true;
    _model= ipViewDataSet->getTabularData();
    ipViewDataSet->getTemplate(&mpTemplate);
    mpXTabView = ipXTabView;
    
	generateAttributeDefaultSorts();
    
    ipViewDataSet->getNullOnTop(&mbIsNullOnTop);
    ipViewDataSet->getNullReplacement(mNullReplacement);
    
}

DSSSortConverter::~DSSSortConverter()
{
}

int DSSSortConverter::OLAPSortConvert(vector<DSSSort*> &ivSorts,
                                                              EventContext *ipEventContext,
                                                              int inSlices,
                                                              int *ipInSliceIDs,
                                                              DSSMetric *ipDimMetricGov, vector<DSSSortDefinition*> **oppSortDefinition)
{
    int hr = S_OK;
    
    vector<DSSSortDefinition*>* definitions=new vector<DSSSortDefinition*>;
    
    //tliao: OLAPSort doesn't need to access DSSXTabView
    /*
    vector<int> lUnitIDs[2] = {
        //((DSSDataModelXTabImpl*)_model)->getXTabView()->getHeader(0)->getUnits(),
        //((DSSDataModelXTabImpl*)_model)->getXTabView()->getHeader(1)->getUnits()
        mpXTabView->getHeader(0)->getUnits(),
        mpXTabView->getHeader(1)->getUnits()
    };
    */
    for(int i  = 0; i < ivSorts.size(); ++i)
    {
        DSSSort *lSort = ivSorts[i];
//        int lAxisID = hGetAxisID(lSort);
        
        HRESULT unitID = -1;
		switch(lSort->getSortType())
		{
			case DssSortAttributeDefault:
				//				unitID=convertAttributeDefaultSortDefinition(lSort,lUnitIDs[lAxisID],definitions);
                unitID=hConvertAttributeDefaultSortDefinition(lSort,definitions);
				break;
			case DssSortAttributeForm:
                //				unitID=convertAttributeFormSortDefinition(lSort,lUnitIDs[lAxisID],definitions);
                unitID=hConvertAttributeFormSortDefinition(lSort,definitions);
                if(unitID == E_FAIL)
                    hr = unitID;
                
				// TQMS 339380
				//if(unitID >= 0 && _model->Item(unitID)->getUnitType()==DssTemplateConsolidation)
				//	mask.push_back(unitID);
				break;
			case DssSortMetric:
			{
                //				unitID = convertMetricSortDefinition(lSort,lUnitIDs[1-lAxisID], definitions);
                unitID = ConvertOlapMetricSort(lSort,definitions, ipEventContext, ipDimMetricGov);
				break;
			}
			case DssSortConsolidationDefault:
                //				unitID=convertConsolidationSortDefinition(lSort,lUnitIDs[lAxisID],definitions);
                unitID=hConvertConsolidationSortDefinition(lSort,definitions);
				break;
			case DssSortCustomGroupDefault:
			case DssSortCustomGroupForm:
                //				unitID=convertCustomGroupSortDefinition(lSort,lUnitIDs[lAxisID],definitions);
                unitID=hConvertCustomGroupSortDefinition(lSort,definitions);
				break;
			case DssSortSubtotalsPosition:
			{
				EnumDSSSubtotalsPosition pos=(EnumDSSSubtotalsPosition)lSort->getSubtotalPosition();
				if( pos==DssSubtotalsPositionMix || pos==DssSubtotalsPositionFirst || pos==DssSubtotalsPositionLast )
					_subtotalPosition=pos;
				break;
			}
			case DssSortMetricHierarchical:
			{
                //				unitID=convertMetricHierarchicalSortDefinition(lSort,lUnitIDs[lAxisID],mpXTabView->getHeader(lAxisID),definitions);
				break;
			}
            case DssSortValue:
                hConvertValueSortDefinition(lSort,definitions);
                break;
			default:
				break;
		}
    }
    //fali, 816727, add a SortValue defination by default
//    if (definitions->size() == 0)
    {
        DSSSortDefinition* def = new DSSSortDefinition(_model);
        def->sortType = DssSortValue;
        def->subtotalPosition = DssSubtotalsPositionLast;
        def->isNullOnTop = false;
        def->isAscending = true;
        
        //fali, 820583, set mNullPosition to DssOLAPNullPositionLast in the constructor of DSSSortDefinition as server-side did. 
//        // tehe, TQMS817929, olap metric sort, set null on bottom, it will affect functions like OLAPCount, we can inital mNullPosition in constructor of DSSSortDefinition, but I'm not sure whether it will affect original logic. so set the value here.
//        def->mNullPosition = DssOLAPNullPositionLast;
        
    
        definitions->push_back(def);
    }
    
    // tehe, TQMS818895
    if(hr == S_OK)
    {
        *oppSortDefinition = definitions;
    }
    else
    {
        *oppSortDefinition = NULL;
        for(int i=0; i<definitions->size(); i++)
        {
            delete (*definitions)[i];
        }
        
        delete definitions;
    }
    
    return hr;
}

int DSSSortConverter::hGetAxisID(DSSSort* iSort)
{
    //Row
    DSSAxis *lRow = mpTemplate->getRow();
    for(int i = 0; i < lRow->Count(); ++i)
    {
        DSSTemplateUnit *lUnit = lRow->Item(i);
        if(DssTemplateAttribute == (lUnit->getType()))
        {
            if(lUnit->getID() == iSort->getObjectID())
            {
                return 0;
            }
        }
        else if(DssTemplateMetrics == (lUnit->getType()))
        {
            DSSTemplateMetrics *lpMetrics = (DSSTemplateMetrics *)lUnit;
            DSSTemplateMetric *lpMetric = lpMetrics->ItemByGUID(iSort->getObjectID());
            if(NULL != lpMetric)
            {
                return 0;
            }
        }
    }
    
    //Column
    DSSAxis *lColumn = mpTemplate->getColumn();
    for(int i = 0; i < lColumn->Count(); ++i)
    {
        DSSTemplateUnit *lUnit = lColumn->Item(i);
        if(DssTemplateAttribute == (lUnit->getType()))
        {
            if(lUnit->getID() == iSort->getObjectID())
            {
                return 1;
            }
        }
        else if(DssTemplateMetrics == (lUnit->getType()))
        {
            DSSTemplateMetrics *lpMetrics = (DSSTemplateMetrics *)lUnit;
            DSSTemplateMetric *lpMetric = lpMetrics->ItemByGUID(iSort->getObjectID());
            if(NULL != lpMetric)
            {
                return 1;
            }
        }
    }
    
    return -1;
}

vector<DSSSortDefinition*>* DSSSortConverter::convert(DSSAxis* axis, vector<int>& unitIDs)
{
	_subtotalPosition=DssSubtotalsPositionLast;
	vector<DSSSortDefinition*>* definitions=new vector<DSSSortDefinition*>;
	vector<int> mask;
	DSSSorts* sorts=NULL;

    // weilin DE43292 2016/09/12 add defensive code to avoid crash
    if (axis)
    {
        sorts=axis->getSorts();
    }
	if(sorts==NULL)
		return definitions;
	int i, unitID;
	for(i=0;i<sorts->Count();++i)
	{
		unitID=-1;
		DSSSort* sort=sorts->Item(i);
		switch(sort->getSortType())
		{
			case DssSortAttributeDefault:
				unitID=convertAttributeDefaultSortDefinition(sort,unitIDs,definitions);
				break;
			case DssSortAttributeForm:
            {
				unitID=convertAttributeFormSortDefinition(sort,unitIDs,definitions,true);
                if (unitID == -1) {
                    unitID = convertRecursiveUnitFormSortDefinition(sort, unitIDs, definitions);
                }
                //zhyang,727162, 726514
				// TQMS 339380
				//if(unitID >= 0 && _model->Item(unitID)->getUnitType()!=DssTemplateConsolidation)
				//	mask.push_back(unitID);
                //zhyang, 745798, 745499, 748229, generate DE default sort
                /*
                 if(unitID >= 0 && _model->Item(unitID)->getUnitType()==DssTemplateConsolidation)
                 {
                 generateDefaultSortDefinition(unitID, definitions);
                 }*/
                break;
            }
            case DssSortMetric:
			{
                //				DSSXTabHeader* header=mpXTabView->getHeader(hGetTemplate()->getRow()==axis ? 1 : 0);
				DSSXTabHeader* header=mpXTabView->getHeader(axis->getIndex() == DssAxisRows ? 1 : 0);
				unitID = convertMetricSortDefinition(sort, header->getUnits(), definitions);
            }
				break;
			case DssSortConsolidationDefault:
            //yimliu, 2014/07/22, 923948.
            case DssSortConsolidationForm:
				unitID=convertConsolidationSortDefinition(sort,unitIDs,definitions);
                if (unitID == -1) {
                    unitID = convertRecursiveUnitFormSortDefinition(sort, unitIDs, definitions);
                }
				break;
			case DssSortCustomGroupDefault:
			case DssSortCustomGroupForm:
				unitID=convertCustomGroupSortDefinition(sort,unitIDs,definitions);
				break;
			case DssSortSubtotalsPosition:
			{
				EnumDSSSubtotalsPosition pos=(EnumDSSSubtotalsPosition)sort->getSubtotalPosition();
				if( pos==DssSubtotalsPositionMix || pos==DssSubtotalsPositionFirst || pos==DssSubtotalsPositionLast )
					_subtotalPosition=pos;
				break;
			}
			case DssSortMetricHierarchical:
			{
                //				DSSXTabHeader* header=mpXTabView->getHeader(hGetTemplate()->getRow()==axis ? 0 : 1);
				DSSXTabHeader* header=mpXTabView->getHeader(axis->getIndex() == DssAxisRows ? 0 : 1);
				unitID=convertMetricHierarchicalSortDefinition(sort,unitIDs,header,definitions);
                unitID = -1;//zhyang, do not mask metric
				break;
			}
            case DssSortRecursiveAttributeForm:
                unitID = convertRecursiveUnitFormSortDefinition(sort, unitIDs, definitions);
                break;
            case DssSortRecursiveAttributeDefault:
                unitID = convertAttributeDefaultSortDefinition(sort,unitIDs,definitions);
                break;
			default:
				break;
		}
		if(unitID>=0 && sort->getSortType()!=DssSortAttributeForm && sort->getSortType()!=DssSortMetric)
			mask.push_back(unitID);
        
	}
    
	vector<int> lRuntimeUnits;
	//unitID=-1;
	for(i=0;i<unitIDs.size();++i)
	{
		unitID=unitIDs[i];
		bool needDefaultSort=true;
		for(int j=0;j<mask.size();++j)
		{
			if(mask[j]==unitID)
			{
				needDefaultSort=false;
				break;
			}
		}
		if (needDefaultSort)
		{
			DSSTabularUnit* unit=_model->Item(unitID);
			if (unit && unit->IsRuntimeUnit())
			{
				needDefaultSort=false;
				lRuntimeUnits.push_back(unitID);
			}
		}
		if(needDefaultSort)
			generateDefaultSortDefinition(unitID,definitions);
	}
	
    //hyan,28/08/2012,650700,
    //we need to pushback the runtime unit by inverse order without sorting
	//lwang, 12/07/2010, 429095, sort runtime units by their unit id
	//sort(lRuntimeUnits.begin(), lRuntimeUnits.end());
    //for (i = 0; i < lRuntimeUnits.size(); i++)
    for (i = lRuntimeUnits.size()-1; i >=0 ; i--)
	{
		generateDefaultSortDefinition(lRuntimeUnits[i],definitions);
	}
    
	return definitions;
}

vector<DSSSortDefinition*>* DSSSortConverter::convertForMicroChart(DSSAxis* axis, vector<int>& unitIDs, GUID &iMetricGUID, bool ibAsc, bool isTreeMode, int iSubtotalID)
{
    vector<DSSSortDefinition*>* definitions=new vector<DSSSortDefinition*>;
    DSSXTabHeader* header=mpXTabView->getHeader(axis->getIndex() == DssAxisRows ? 0 : 1);
    convertMetricHierarchicalSortDefinition(NULL,unitIDs,header,definitions,true, &iMetricGUID, ibAsc, isTreeMode, iSubtotalID);
    return definitions;
}

void DSSSortConverter::generateAttributeDefaultSorts()
{
	for(int i=0;i<Count();++i)
	{
		DSSTabularUnit* unit=_model->Item(i);
		if(unit==NULL)
			continue;
		GUID guid;
		unit->getGUID(guid);
		DSSObjectInfo* object=hGetObjectContext()->FindObjectH(guid);
		if(object!=NULL && object->getType()==DssTypeAttribute)
		{
			DSSSorts* sorts=((DSSAttribute*)object)->getSorts();
			if(sorts!=NULL && sorts->Count()>0)
				_defaultAttributeSorts[guid]=sorts;
		}
	}
}

int DSSSortConverter::generateDefaultSortDefinition(int unitID, vector<DSSSortDefinition*>* definitions)
{
	DSSTabularUnit* unit=_model->Item(unitID);
	if(unit==NULL)
		return -1;
	EnumDSSSubtotalsPosition subtotalPosition=_subtotalPosition;
    bool isAscending=true;
	bool isNullOnTop=hGetNullOnTop();
	EnumDSSTemplateUnitType unitType=unit->getUnitType();
    
	if(unitType==DssTemplateAttribute)
	{
		GUID guid;
		unit->getGUID(guid);
		map<GUID, DSSSorts*>::const_iterator it=_defaultAttributeSorts.find(guid);
        //TODO: default sort on recursive units?
		if(it!=_defaultAttributeSorts.end())
		{
			DSSSorts* sorts=it->second;
			// bool explicitSort=false;
            
            if(mbNewRWD)
            {
                int arity=((DSSTabularAttribute*)unit)->getArity();
                bool *arityMask = new bool[arity];
                MBase::StrongPtr<bool, MBase::DeleteArray<bool> > lpArityMaskPtr(arityMask);
                for(int i = 0; i < arity; ++i)
                    arityMask[i] = false;
                
                vector<int> lvForms;
                bool *lpAsc = new bool[sorts->Count() + arity];
                MBase::StrongPtr<bool, MBase::DeleteArray<bool> > lpAscPtr(lpAsc);
                
                for(int i=0;i<sorts->Count();++i)
                {
                    DSSSort* sort=sorts->Item(i);
                    if(sort->getSortType()==DssSortAttributeForm || sort->getSortType()==DssSortRecursiveAttributeForm )
                    {
                        int lBaseFormID = sort->getBaseFormID();
                        lvForms.push_back(lBaseFormID);
                        lpAsc[i] = sort->getAscending();
                        
                        if ((lBaseFormID >= 0) && (lBaseFormID < arity))
                            arityMask[lBaseFormID] = true;
                    }
                }
                
                
                for(int i=0;i<arity;++i)
                {
                    if(!arityMask[i])
                    {
                        //zhyang, get the correct form
                        int formIndex;
                        if(((DSSTabularAttribute*)unit)->FindFormWithLocale(unitID, i, 0, formIndex)!=S_OK)
                            return -1;
                        lvForms.push_back(formIndex);
                        lpAsc[i+sorts->Count()] = isAscending;
                    }
                }
                
                DSSTabularData* lpTabularData = mpXTabView != NULL ? mpXTabView->GetViewDataSet()->getTabularData() : _model;
                if(lpTabularData->canUseSSO(guid))
                {
                    int lUnitIDinGUnitContainer;
                    lpTabularData->getGUnitContainer()->GetGlobalUnitIDByGUID(guid, &lUnitIDinGUnitContainer);
                    
                    int lBucketID = lpTabularData->getGUnitContainer()->getBucketID(lUnitIDinGUnitContainer, lvForms, lpAsc, hGetNullOnTop());
                    
                    if(true && lBucketID != -1)
                    {
                        DSSSortDefinition* def=new DSSSortDefinition(_model);
                        def->unitID = unitID;
                        def->sortType = DssSortAttributeOpt;
                        def->unitIDInContianer = lUnitIDinGUnitContainer;
                        def->bucketID = lBucketID;
                        def->subtotalPosition=subtotalPosition;
                        definitions->push_back(def);
                        return unitID;
                    }
                }
            }
            
			for(int i=0;i<sorts->Count();++i)
			{
				DSSSort* sort=sorts->Item(i);
				vector<int> unitIDs;
				unitIDs.push_back(unitID);
                convertAttributeFormSortDefinition(sort, unitIDs, definitions);//718127
				//if(sort->getSortType()==DssSortAttributeForm && convertAttributeFormSortDefinition(sort, unitIDs, definitions)>=0)
                // explicitSort=true;
                //    ;
			}
		}
	}
	else if(unitType==DssTemplateMetrics)
	{
		DSSSortDefinition* def=new DSSSortDefinition(_model);
		def->sortType=DssSortReserved;
		def->unitID=unitID;
		def->subtotalPosition=subtotalPosition;
		def->isNullOnTop=isNullOnTop;
		def->isAscending=isAscending;
		definitions->push_back(def);
	}
    
	if(unitType==DssTemplateAttribute)
	{
		int arity=((DSSTabularAttribute*)unit)->getArity();
        
        GUID guid;
        unit->getGUID(guid);
        
        //fali,894595
        DSSTabularData* lpTabularData = NULL;
        //mpXTabView->GetViewDataSet() may be NULL for oldRWD
        if (mpXTabView != NULL && mpXTabView->GetViewDataSet() != NULL)
        {
            lpTabularData = mpXTabView->GetViewDataSet()->getTabularData();
        }
        else
        {
            lpTabularData = _model;
        }
        // DSSTabularData* lpTabularData = _model == NULL ? mpXTabView->GetViewDataSet()->getTabularData() : _model;
        
        if(mbNewRWD && lpTabularData->canUseSSO(guid))
        //if(mbNewRWD &&mpXTabView->GetViewDataSet()->getTabularData()->canUseSSO(guid))
        {
            vector<int> lvForms;
            bool *lpAsc = new bool[arity];
            MBase::StrongPtr<bool, MBase::DeleteArray<bool> > lpAscPtr(lpAsc);
            
            for(int i=0;i<arity;++i)
            {
                
                lvForms.push_back(i);
                lpAsc[i] = isAscending;
            }
            
            int lUnitIDinGUnitContainer;
            
            lpTabularData->getGUnitContainer()->GetGlobalUnitIDByGUID(guid, &lUnitIDinGUnitContainer);
            
            int lBucketID = lpTabularData->getGUnitContainer()->getBucketID(lUnitIDinGUnitContainer, lvForms, lpAsc, hGetNullOnTop());
            
            /*
             mpXTabView->GetViewDataSet()->getTabularData()->getGUnitContainer()->GetGlobalUnitIDByGUID(guid, &lUnitIDinGUnitContainer);
            
            int lBucketID = mpXTabView->GetViewDataSet()->getTabularData()->getGUnitContainer()->getBucketID(lUnitIDinGUnitContainer, lvForms, lpAsc, hGetNullOnTop());
            */
            
            if(lBucketID != -1)//msun TODO: template discard the sort definition in global lookup table.
            {
                DSSSortDefinition* def=new DSSSortDefinition(_model);
                if (unit->IsRecursiveUnit()) {
                    def->sortType = DssSortRecursiveAttributeDefault;
                }
                else{
                    def->sortType = DssSortAttributeOpt;
                }
                def->unitIDInContianer = lUnitIDinGUnitContainer;
                def->unitID = unitID;
                def->bucketID = lBucketID;
                def->subtotalPosition=subtotalPosition;
                definitions->push_back(def);
                return unitID;
            }
        }
        
        
		for(int i=0;i<arity;++i)
		{
			DSSSortDefinition* def=new DSSSortDefinition(_model);
            if (unit->IsRecursiveUnit()) {
                def->sortType = DssSortRecursiveAttributeForm;
            }
            else{
                def->sortType=DssSortAttributeForm;
            }
			
			def->unitID=unitID;
			def->formID=i;
			def->subtotalPosition=subtotalPosition;
			def->isNullOnTop=isNullOnTop;
			def->isAscending=isAscending;
			definitions->push_back(def);
		}
		return unitID;
	}
	else if(unitType==DssTemplateCustomGroup)
	{
		DSSSortDefinition* def=new DSSSortDefinition(_model);
		def->sortType=DssSortCustomGroupDefault;
		def->unitID=unitID;
		def->subtotalPosition=subtotalPosition;
		def->isNullOnTop=isNullOnTop;
		def->isAscending=isAscending;
		definitions->push_back(def);
		return unitID;
	}
	else if(unitType==DssTemplateConsolidation)
	{
		if(((DSSTabularConsolidation*)unit)->IsDerivedElement())
		{
			generateDerivedElementDefaultSortDefinition((DSSTabularConsolidation*)unit, definitions);
		}
        else
        {
            DSSSortDefinition* def=new DSSSortDefinition(_model);
            def->sortType=DssSortConsolidationDefault;
            def->unitID=unitID;
            def->formID=-1;
            def->subtotalPosition=subtotalPosition;
            def->isNullOnTop=isNullOnTop;
            def->isAscending=isAscending;
            definitions->push_back(def);
        }
		return unitID;
	}
	return -1;
}


EnumDSSSubtotalsPosition DSSSortConverter::determineSubtotalPosition(DSSSort* sort)
{
	EnumDSSSubtotalsPosition pos=(EnumDSSSubtotalsPosition)sort->getSubtotalPosition();
	if(pos==DssSubtotalsPositionInherit)
		return _subtotalPosition;
	else
		return _subtotalPosition=pos;
}


int DSSSortConverter::convertRecursiveUnitFormSortDefinition(DSSSort* sort, vector<int>& unitIDs, vector<DSSSortDefinition*>* definitions, bool ibDEFormSort)
{
    EnumDSSSortType sortType=DssSortRecursiveAttributeForm;
    bool isAscending=sort->getAscending();
    EnumDSSSubtotalsPosition subtotalPosition=determineSubtotalPosition(sort);
    bool isNullOnTop=hGetNullOnTop();
    GUID guid=sort->getObjectID();
    int unitID=FindUnit(guid,DssTemplateAttribute, false, true);
    if(unitID<0)
    {
        unitID=FindUnit(guid,DssTemplateConsolidation, false, true);
        if(unitID<0)
            return -1;
        //sortType = ibDEFormSort ? DssSortRecursiveDerivedElementForm : DssSortRecursiveDerivedElementDefault;
    }
    
    int i,j;
    bool found=false;
    for(i=0;i<unitIDs.size();++i)
    {
        if(unitIDs[i]==unitID)
        {
            found=true;
            break;
        }
    }
    if(!found)
        return -1;
    
    int formID=sort->getBaseFormID();
    found=false;
    for(j=0;j<definitions->size();++j)
    {
        DSSSortDefinition* def=(*definitions)[j];
        if(def->sortType==DssSortRecursiveAttributeForm && def->unitID==unitID && def->formID==formID)
        {
            found=true;
            break;
        }
    }
    if(found)
        return -1;
    
    DSSXTabRecursiveLookupTable* attribute;
    if(sortType==DssSortRecursiveAttributeForm)
    {
        attribute=(DSSXTabRecursiveLookupTable*)(_model->Item(unitID));
    }
    else // DssTemplateConsolidation
    {
        DSSTabularConsolidation* consolidation=(DSSTabularConsolidation*)(_model->Item(unitID));
        if(!consolidation)
            return -1;
        attribute=(DSSXTabRecursiveLookupTable*)consolidation->getChildUnit();
    }
    if(!attribute)
        return -1;
    int formIndex;
    if(attribute->FindFormWithLocale(unitID, formID, 0, formIndex)!=S_OK)
        return -1;
    DSSDataColumn *column;
    if(attribute->Item(formIndex, &column)!=S_OK)
        return -1;
    if(column->getDataType()==DssDataTypeMissing)
        return -1;
    
    DSSSortDefinition* def=new DSSSortDefinition(_model);
    def->sortType= sortType;
    def->unitID=unitID;
    def->formID=formID;
    def->subtotalPosition=subtotalPosition;
    def->isNullOnTop=isNullOnTop;
    def->isAscending=isAscending;
    definitions->push_back(def);
    
    return unitID;
    //zhyang,727162, 726514
    return -1;

}

int DSSSortConverter::convertRecursiveUnitDefaultSortDefinition(DSSSort* sort, vector<int>& unitIDs, vector<DSSSortDefinition*>* definitions)
{
    return convertAttributeDefaultSortDefinition(sort, unitIDs, definitions);
}

int DSSSortConverter::convertAttributeFormSortDefinition(DSSSort* sort, vector<int>& unitIDs, vector<DSSSortDefinition*>* definitions, bool ibDEFormSort)
{
	EnumDSSSortType sortType=DssSortAttributeForm;
	bool isAscending=sort->getAscending();
	EnumDSSSubtotalsPosition subtotalPosition=determineSubtotalPosition(sort);
	bool isNullOnTop=hGetNullOnTop();
	GUID guid=sort->getObjectID();
	int unitID=FindUnit(guid,DssTemplateAttribute);
	if(unitID<0)
	{
		unitID=FindUnit(guid,DssTemplateConsolidation);
		if(unitID<0)
			return -1;
//		sortType=DssSortConsolidationDefault;
        sortType = ibDEFormSort ? DssSortDerivedElementForm : DssSortDerivedElementDefault;
	}
    
	int i,j;
	bool found=false;
	for(i=0;i<unitIDs.size();++i)
	{
		if(unitIDs[i]==unitID)
		{
			found=true;
			break;
		}
	}
	if(!found)
		return -1;
    
	int formID=sort->getBaseFormID();
	found=false;
	for(j=0;j<definitions->size();++j)
	{
		DSSSortDefinition* def=(*definitions)[j];
		if(def->sortType==DssSortAttributeForm && def->unitID==unitID && def->formID==formID)
		{
			found=true;
			break;
		}
	}
	if(found)
		return -1;
    
	DSSTabularAttribute* attribute;
	if(sortType==DssSortAttributeForm)
	{
		attribute=(DSSTabularAttribute*)(_model->Item(unitID));
	}
	else // DssTemplateConsolidation
	{
		DSSTabularConsolidation* consolidation=(DSSTabularConsolidation*)(_model->Item(unitID));
		if(!consolidation)
			return -1;
		attribute=(DSSTabularAttribute*)consolidation->getChildUnit();
	}
	if(!attribute)
		return -1;
	int formIndex;
	if(attribute->FindFormWithLocale(unitID, formID, 0, formIndex)!=S_OK)
		return -1;
	DSSDataColumn *column;
	if(attribute->Item(formIndex, &column)!=S_OK)
		return -1;
	if(column->getDataType()==DssDataTypeMissing)
		return -1;
	
	DSSSortDefinition* def=new DSSSortDefinition(_model);
	def->sortType= sortType;
	def->unitID=unitID;
	def->formID=formID;
	def->subtotalPosition=subtotalPosition;
	def->isNullOnTop=isNullOnTop;
	def->isAscending=isAscending;
	definitions->push_back(def);
    
    return unitID;
    //zhyang,727162, 726514
    return -1;
	//return unitID;
}

int DSSSortConverter::convertAttributeDefaultSortDefinition(DSSSort* sort, vector<int>& unitIDs, vector<DSSSortDefinition*>* definitions)
{
	GUID guid=sort->getObjectID();
	int unitID=FindUnit(guid,DssTemplateAttribute);
	if(unitID<0)
		return -1;
    
	for(int i=0;i<unitIDs.size();++i)
	{
		if(unitIDs[i]==unitID)
			return generateDefaultSortDefinition(unitID,definitions);
	}
	return -1;
}

int DSSSortConverter::convertMetricSortDefinition(DSSSort* sort, vector<int>& unitIDs, vector<DSSSortDefinition*>* definitions)
{
	DSSTabularMetrics* metrics = hGetMetricUnit();
	int metricID;
	GUID guid=sort->getObjectID();
    EnumDSSSubtotalsPosition subtotalPosition=determineSubtotalPosition(sort);
	if(metrics==NULL || (metricID=metrics->FindDisplayedMetric(guid))<0)
		return -1;
    
	EnumDSSSortType sortType=DssSortMetric;
	bool isAscending=sort->getAscending();
	int unitID=hGetMetricUnitID();
	
	/* 421350
     bool found=false;
     for(int j=0;j<unitIDs.size();++j)
     {
     if(unitIDs[j]==unitID)
     {
     found=true;
     break;
     }
     }
     if(!found)
     return -1;
     */
    
	
	int subtotalID=sort->getSubtotalIndex();
    
    bool isNullOnTop=hGetNullOnTop();
	DSSSortDefinition* def=new DSSSortDefinition(_model);
	def->sortType=sortType;
	def->unitID=unitID;
	def->metricID=metricID;
	def->subtotalID=subtotalID;
	def->subtotalPosition=subtotalPosition;
	def->isNullOnTop=isNullOnTop;
	def->isAscending=isAscending;
	def->level=0;
    def->mNullReplacement = mNullReplacement;
	
	
	DSSBaseElementsProxy* elements=sort->getElements();
	if(elements!=NULL)
	{
		int i;
		for(i=0;i<elements->Count();++i)
		{
			DSSBaseElementProxy* element=elements->Item(i);
			int unit, key;
			if(element->getElementType() != DssElementMetric)
            {
                if(FindElement(element, unit, key)==S_OK)
                    def->elementIDs[unit]=key;
                else
                {
                    //zhyang, 696548, if we can't find the criteria, it will be treate as invalid sort. Ignore it.
                    delete def;
                    return unitID;
                }
            }
		}
	}
    definitions->push_back(def);
	return unitID;
}


int DSSSortConverter::convertMetricHierarchicalSortDefinition(DSSSort* sort, vector<int>& unitIDs, DSSXTabHeader* header, vector<DSSSortDefinition*>* definitions, bool ibForMicroChart, GUID *iMetricGUID, bool ibAsc, bool isTreeMode, int iSubtotalID)
{
    DSSTabularMetrics* metrics = hGetMetricUnit();
	int metricID;
    bool isAscending = true;
    int subtotalID = 1;
    EnumDSSSubtotalsPosition subtotalPosition = DssSubtotalsPositionLast;
    
    if(ibForMicroChart)
    {
        if(metrics==NULL || (metricID=metrics->FindMetric(*iMetricGUID))<0)
            return -1;
        //metricID = metrics->Count() -1;
        //metricID = 0;
        isAscending = ibAsc;
        subtotalID = iSubtotalID;
        if(!isTreeMode)
            subtotalPosition = DssSubtotalsPositionMix;
    }
    else
    {
        GUID guid=sort->getObjectID();
        if(metrics==NULL || (metricID=metrics->FindMetric(guid))<0)
            return -1;
        isAscending=sort->getAscending();
        subtotalID=sort->getSubtotalIndex();
        subtotalPosition=determineSubtotalPosition(sort);
    }
    
    EnumDSSSortType sortType=DssSortMetric;
	int unitID=hGetMetricUnitID();
    
    
    
	bool isNullOnTop=hGetNullOnTop();
    
	DSSSortDefinition* def;
    
	DSSBaseElementsProxy* elements=NULL;
    if(!ibForMicroChart)
        elements = sort->getElements();
	map<int,int> keys;
	if(elements!=NULL)
	{
		for(int i=0;i<elements->Count();++i)
		{
			DSSBaseElementProxy* element=elements->Item(i);
			int unit, key;
			FindElement(element, unit, key);
			keys[unit]=key;
		}
	}
    
    vector<int> lvUsefulSortUnit;
    
    for(int i = 0; i < unitIDs.size(); ++i)
    {
        DSSTabularUnit* unit=_model->Item(unitIDs[i]);
        if (unit && unit->IsRuntimeUnit())
        {
            continue;
        }
        
        EnumDSSTemplateUnitType lType = unit->getUnitType();
        switch (lType) {
            case DssTemplateCustomGroup:
            case DssTemplateAttribute:
            case DssTemplateConsolidation:
                lvUsefulSortUnit.push_back(unitIDs[i]);
                break;
                
                //zhyang, current we don't support DssTemplateDimension in HS
            case DssTemplateDimension:
                assert(false);
                break;
                
            default:
                break;
        }
    }
    
    
    map<int,int>::const_iterator it;
    int lSize = lvUsefulSortUnit.size();
    bool lbMicroChartNormalNode = (ibForMicroChart && !isTreeMode);
    for(int i = 0; i < lSize -1; ++i)
    {
        if(lbMicroChartNormalNode)
        {
            if(lSize < 2)
                break;
            i = lSize - 2;
        }
        vector<int> tmpUnits;
        for(int j = 0; j <= i; ++j)
        {
            tmpUnits.push_back(lvUsefulSortUnit[j]);
        }
        
        def=new DSSSortDefinition(_model);
		def->sortType=sortType;
		def->unitID=unitID;
		def->metricID=metricID;
		def->subtotalID=subtotalID;
		def->subtotalPosition=subtotalPosition;
		def->isNullOnTop=isNullOnTop;
		def->isAscending=isAscending;
        def->mNullReplacement = mNullReplacement;
		
		for(it=keys.begin(); it!=keys.end(); ++it)
			def->elementIDs[it->first]=it->second;
        
        def->level=header->getLevelByUnits(tmpUnits, subtotalID);
        //def->mvUnits = tmpUnits;
        for(int k = i+1; k < lvUsefulSortUnit.size(); ++k)
            def->mvMaskUnits.push_back(lvUsefulSortUnit[k]);
        def->mvUnits = tmpUnits;
        if(def->level == -1 && ibForMicroChart)
        {
            def->mForMicroChart = true;
            if(def->mvMaskUnits.size() > 0)
                def->mvMaskUnits.resize(def->mvMaskUnits.size() - 1);
            if(lbMicroChartNormalNode)
                def->mvMaskUnits.clear();
        }
        definitions->push_back(def);
        if(lbMicroChartNormalNode)
            break;
        
        generateDefaultSortDefinition(lvUsefulSortUnit[i],definitions);
    }
    
    if(!ibForMicroChart)
        unitID = convertMetricSortDefinition(sort, header->getUnits(), definitions);
    /*	map<int,int>::const_iterator it;
     bool lbContiansMetric = false;
     int lOffset = 2;
     
     for(i=1;i<=unitIDs.size();++i)
     {
     if(unitIDs[i-1]==unitID) // or is dummy unit
     continue;
     
     if(i>1 && unitIDs[i-2]!=unitID)
     {
     if(!lbContiansMetric && unitID == unitIDs[i-lOffset])
     {
     //metric
     lbContiansMetric = true;
     lOffset--;
     }
     generateDefaultSortDefinition(unitIDs[i-lOffset],definitions);
     }
     
     def=new DSSSortDefinition(_model);
     def->sortType=sortType;
     def->unitID=unitID;
     def->metricID=metricID;
     def->subtotalID=subtotalID;
     def->subtotalPosition=subtotalPosition;
     def->isNullOnTop=isNullOnTop;
     def->isAscending=isAscending;
     definitions->push_back(def);
     
     for(it=keys.begin(); it!=keys.end(); ++it)
     def->elementIDs[it->first]=it->second;
     
     if(i<unitIDs.size())
     {
     vector<int> tmpUnits;
     int lastUnit = i;
     for(j=0;j<lastUnit;++j)
     {
     //The unit is not the metric
     if(unitID != unitIDs[j])
     tmpUnits.push_back(unitIDs[j]);
     //else
     //    lastUnit++;
     }
     def->level=header->getLevelByUnits(tmpUnits, subtotalID);
     def->units = tmpUnits;
     }
     else
     def->level=0;
     }
     */
	//return unitID;
    return -1;
}


int DSSSortConverter::convertCustomGroupSortDefinition(DSSSort* sort, vector<int>& unitIDs, vector<DSSSortDefinition*>* definitions)
{
	bool isAscending=sort->getAscending();
	EnumDSSSubtotalsPosition subtotalPosition=determineSubtotalPosition(sort);
	bool isNullOnTop=hGetNullOnTop();
	GUID guid=sort->getObjectID();
	int unitID=FindUnit(guid,DssTemplateCustomGroup);
	if(unitID<0)
		return -1;
    
	for(int i=0;i<unitIDs.size();++i)
	{
		if(unitIDs[i]==unitID)
		{
			DSSSortDefinition* def=new DSSSortDefinition(_model);
			def->sortType=DssSortCustomGroupDefault;
			def->unitID=unitID;
			def->subtotalPosition=subtotalPosition;
			def->isNullOnTop=isNullOnTop;
			def->isAscending=isAscending;
			definitions->push_back(def);
			return unitID;
		}
	}
	return -1;
}

int DSSSortConverter::convertConsolidationSortDefinition(DSSSort* sort, vector<int>& unitIDs, vector<DSSSortDefinition*>* definitions)
{
	bool isAscending=sort->getAscending();
	EnumDSSSubtotalsPosition subtotalPosition=determineSubtotalPosition(sort);
	bool isNullOnTop=hGetNullOnTop();
	GUID guid=sort->getObjectID();
	int unitID=FindUnit(guid,DssTemplateConsolidation);
	if(unitID<0)
		return -1;
    
	for(int i=0;i<unitIDs.size();++i)
	{
		if(unitIDs[i]==unitID)
		{
            DSSTabularConsolidation* lpConsolidation=dynamic_cast<DSSTabularConsolidation*>(_model->Item(unitID));
            if (lpConsolidation == NULL)
                return -1;
            //yimliu, 2014/07/22, 924008. Add codes to deal with NewDE.
            if (lpConsolidation -> IsDerivedElement())
            {
                int lnFormID = sort->getBaseFormID();
                DSSTabularAttribute * lpAttribute = dynamic_cast<DSSTabularAttribute*>(lpConsolidation->getChildUnit());
                if (lpAttribute == NULL) {
                    return -1;
                }
                int lnFormIndex;
                if (lpAttribute->FindFormWithLocale(unitID, lnFormID, 0, lnFormIndex) != S_OK) {
                    return -1;
                }
                DSSDataColumn *lpColumn;
                if(lpAttribute->Item(lnFormIndex, &lpColumn)!=S_OK)
                    return -1;
                if(lpColumn->getDataType()==DssDataTypeMissing)
                    return -1;
                DSSSortDefinition* def=new DSSSortDefinition(_model);
                def->sortType = DssSortDerivedElementForm;
                def->formID=lnFormID;
                def->unitID=unitID;
                def->subtotalPosition=subtotalPosition;
                def->isNullOnTop=isNullOnTop;
                def->isAscending=isAscending;
                definitions->push_back(def);
            }
            else
            {
                DSSSortDefinition* def=new DSSSortDefinition(_model);
                def->sortType = DssSortConsolidationDefault;
                def->formID=-1;
                def->unitID=unitID;
                def->subtotalPosition=subtotalPosition;
                def->isNullOnTop=isNullOnTop;
                def->isAscending=isAscending;
                definitions->push_back(def);
            }
			return unitID;
		}
	}
	return -1;
}

void DSSSortConverter::generateDerivedElementDefaultSortDefinition(DSSTabularConsolidation* unit, vector<DSSSortDefinition*>* definitions)
{
	bool isAscending=true;
	EnumDSSSubtotalsPosition subtotalPosition=_subtotalPosition;
	bool isNullOnTop=hGetNullOnTop();
    
	DSSTabularAttribute* attribute=(DSSTabularAttribute*)unit->getChildUnit();
    
	GUID guid;
	unit->getGUID(guid);
    int arity=attribute->getArity();
    
	map<GUID, DSSSorts*>::const_iterator it=_defaultAttributeSorts.find(guid);
	if(it!=_defaultAttributeSorts.end())
	{
		DSSSorts* sorts=it->second;
		for(int i=0;i<sorts->Count();++i)
		{
			DSSSort* sort=sorts->Item(i);
			vector<int> unitIDs;
			unitIDs.push_back(unit->getUnitIndex());
			sort->getSortType()==DssSortAttributeForm && convertAttributeFormSortDefinition(sort, unitIDs, definitions);
		}
	}
	

	for(int i=0;i<arity;++i)
	{
		DSSSortDefinition* def=new DSSSortDefinition(_model);
		def->sortType=DssSortDerivedElementDefault;
		def->unitID=unit->getUnitIndex();
		def->formID=i;
		def->subtotalPosition=subtotalPosition;
		def->isNullOnTop=isNullOnTop;
		def->isAscending=isAscending;
		definitions->push_back(def);
	}
}

bool DSSSortConverter::hGetNullOnTop()
{
    return mbIsNullOnTop;
}

DSSTabularMetrics* DSSSortConverter::hGetMetricUnit()
{
    return _model->getTabularMetircs();
}

int DSSSortConverter::hGetMetricUnitID()
{
    return _model->getMetircPos();
}

DSSObjectContext* DSSSortConverter::hGetObjectContext()
{
    return _model->getObjectContext();
}

int DSSSortConverter::FindUnit(const GUID &irGUID, EnumDSSTemplateUnitType iType, bool includeLinkUnit, bool iCheckRecursive)
{
    if(!includeLinkUnit)
        return _model->FindUnit(irGUID, iType, iCheckRecursive);
    else//xiazhou, 08/12/2014. Find linked attribute if irGUID is missing in tabluar data
        return _model->FindLinkedUnit(irGUID, iType);
}

int DSSSortConverter::Count()
{
    return _model->Count();
}

int DSSSortConverter::FindElement(DSSBaseElementProxy *ipElement, int &orUnit, int &orKey)
{
    return _model->FindElement(ipElement, orUnit, orKey);
}


int DSSSortConverter::hConvertAttributeFormSortDefinition(DSSSort* sort, vector<DSSSortDefinition*>* definitions)
{
	EnumDSSSortType sortType=DssSortAttributeForm;
	bool isAscending=sort->getAscending();
	EnumDSSSubtotalsPosition subtotalPosition=determineSubtotalPosition(sort);
	bool isNullOnTop=hGetNullOnTop();
	GUID guid=sort->getObjectID();
	int unitID=FindUnit(guid,DssTemplateAttribute, false/*includeLinkUnit*/);
	if(unitID<0)//xiazhou, search order: attribute, consolidation, linked consolidation(DE), linked attribute.
	{
		unitID=FindUnit(guid,DssTemplateConsolidation, false);
		if(unitID<0)
        {
            unitID=FindUnit(guid,DssTemplateConsolidation);//search link unit
            if(unitID >= 0)
            {
               sortType=DssSortDerivedElementDefault;
            }
            else
            {
                unitID=FindUnit(guid,DssTemplateAttribute);
                if(unitID < 0)
                    return -1;
            }
        }
	}
    
	int j;
	bool found=false;

	int formID=sort->getBaseFormID();
	found=false;
	for(j=0;j<definitions->size();++j)
	{
		DSSSortDefinition* def=(*definitions)[j];
		if(def->sortType==DssSortAttributeForm && def->unitID==unitID && def->formID==formID)
		{
			found=true;
			break;
		}
	}
	if(found)
		return -1;
    
	DSSTabularAttribute* attribute;
	if(sortType==DssSortAttributeForm)
	{
		attribute=(DSSTabularAttribute*)(_model->Item(unitID));
	}
	else // DssTemplateConsolidation
	{
		DSSTabularConsolidation* consolidation=(DSSTabularConsolidation*)(_model->Item(unitID));
		if(!consolidation)
			return -1;
		attribute=(DSSTabularAttribute*)consolidation->getChildUnit();
	}
	if(!attribute)
		return -1;
	int formIndex;
	if(attribute->FindFormWithLocale(unitID, formID, 0, formIndex)!=S_OK)
		return -1;
	DSSDataColumn *column;
	if(attribute->Item(formIndex, &column)!=S_OK)
		return -1;
	if(column->getDataType()==DssDataTypeMissing)
		return E_FAIL; // tehe, TQMS818895, fail the metric if a single form is missing
	
	DSSSortDefinition* def=new DSSSortDefinition(_model);
	def->sortType=sortType;
	def->unitID=unitID;
	def->formID=formID;
	def->subtotalPosition=subtotalPosition;
	def->isNullOnTop=isNullOnTop;
	def->isAscending=isAscending;
	definitions->push_back(def);
    
    //zhyang,727162, 726514
    return -1;
	//return unitID;
}

int DSSSortConverter::hConvertAttributeDefaultSortDefinition(DSSSort* sort, vector<DSSSortDefinition*>* definitions)
{
	GUID guid=sort->getObjectID();
	int unitID=FindUnit(guid,DssTemplateAttribute);
	if(unitID<0)
		return -1;
    
    return generateDefaultSortDefinition(unitID,definitions);
}

int DSSSortConverter::ConvertOlapMetricSort(DSSSort* sort, vector<DSSSortDefinition*>* definitions, EventContext *ipEventContext, DSSMetric *ipDimMetricGov)
{
    int hr = S_OK;
    
	DSSTabularMetrics* metrics = hGetMetricUnit();
	int metricID;
	GUID lMetricID=sort->getObjectID();
    
	if(metrics==NULL || (metricID=metrics->FindMetric(lMetricID))<0)
		return -1;
    
    DSSSliceID lSliceID;
    int lMIndex = 0;
    DSSCalculationPlan *ipCalculationPlan = ipEventContext->mpCalculationPlan;
    if (ipDimMetricGov)
    {
        GUID iDimMetricGovID = ipDimMetricGov->getID();
        lMIndex = ipCalculationPlan->getMetricIndexWithGoverner(lMetricID, iDimMetricGovID);
    }
    
    if (!ipDimMetricGov || lMIndex == -1)
    {
        lMIndex = ipCalculationPlan->getMetricIndex(lMetricID);
    }
    
    bool lfCalculated = false;
    int iLevel = ipEventContext->mLevel;
	int iEvent = ipEventContext->mpEvent->mEventIndex;
    hr = ipCalculationPlan->getMetricSliceID(iLevel, iEvent, lMIndex, &lfCalculated, &(lSliceID.mSliceID));
    CHECK_HR;
    
    if (!lfCalculated)
        return -1;
    
    lSliceID.mpCube = ipEventContext->mpCube;
    
    EnumDSSSubtotalsPosition subtotalPosition=determineSubtotalPosition(sort);
	EnumDSSSortType sortType=DssSortMetric;
	bool isAscending=sort->getAscending();
    
	DSSSortDefinition* def=new DSSSortDefinition(_model);
	def->sortType=sortType;
	def->subtotalPosition=subtotalPosition;
	def->isNullOnTop=false;
	def->isAscending=isAscending;
    def->mSliceID = lSliceID;

    definitions->push_back(def);
	return S_OK;
}


int DSSSortConverter::hConvertCustomGroupSortDefinition(DSSSort* sort, vector<DSSSortDefinition*>* definitions)
{
	bool isAscending=true;
	EnumDSSSubtotalsPosition subtotalPosition=determineSubtotalPosition(sort);
	bool isNullOnTop=hGetNullOnTop();
	GUID guid=sort->getObjectID();
	int unitID=FindUnit(guid,DssTemplateCustomGroup);
	if(unitID<0)
		return -1;
    
    DSSSortDefinition* def=new DSSSortDefinition(_model);
    def->sortType=DssSortCustomGroupDefault;
    def->unitID=unitID;
    def->subtotalPosition=subtotalPosition;
    def->isNullOnTop=isNullOnTop;
    def->isAscending=isAscending;
    definitions->push_back(def);
    return unitID;
    
}

int DSSSortConverter::hConvertConsolidationSortDefinition(DSSSort* sort, vector<DSSSortDefinition*>* definitions)
{
	bool isAscending=sort->getAscending();
	EnumDSSSubtotalsPosition subtotalPosition=determineSubtotalPosition(sort);
	bool isNullOnTop=hGetNullOnTop();
	GUID guid=sort->getObjectID();
	int unitID=FindUnit(guid,DssTemplateConsolidation);
	if(unitID<0)
		return -1;
    
    DSSSortDefinition* def=new DSSSortDefinition(_model);
    def->sortType=DssSortConsolidationDefault;
    def->formID=-1;
    def->unitID=unitID;
    def->subtotalPosition=subtotalPosition;
    def->isNullOnTop=isNullOnTop;
    def->isAscending=isAscending;
    definitions->push_back(def);
    return unitID;
    
}

int DSSSortConverter::hConvertValueSortDefinition(DSSSort* sort, vector<DSSSortDefinition*>* definitions)
{
    EnumDSSSubtotalsPosition subtotalPosition=determineSubtotalPosition(sort);
	EnumDSSSortType sortType=DssSortValue;
	bool isAscending=sort->getAscending();
    
	DSSSortDefinition* def=new DSSSortDefinition(_model);
	def->sortType=sortType;
	def->subtotalPosition=subtotalPosition;
	def->isNullOnTop=false;
	def->isAscending=isAscending;
    
    definitions->push_back(def);
	return S_OK;

}

//tliao, 954998, 2014/08/28, support dynamic sort by
int DSSSortConverter::OLAPSortConvert(DFCSimpleExpr               *ipSortLevel,
                                      EventContext                *ipEventContext,
                                      int                         inSlices,
                                      int                         *ipInSliceIDs,
                                      DSSMetric                   *ipDimMetricGov,
                                      vector<DSSSortDefinition*>  **oppSortDefinition)
{
    int hr = S_OK;
    
    vector<DSSSortDefinition*>* definitions = new vector<DSSSortDefinition*>;
    
    // SQL engine guarantees this is branch, even though there is only one sort object
	AE_ASSERT(ipSortLevel->GetExprType() == DFCSimpleExpr::SIMPLE_EXPR_BRANCH);
    
	DFCSimpleExprBranch* lpBranch = static_cast<DFCSimpleExprBranch*>(ipSortLevel);
	vector<DFCSimpleExpr*, MBase::Allocator<DFCSimpleExpr*> > lvSortLevels(lpBranch->GetArgs());
    
	Int32 lnSort = lvSortLevels.size();
    
	for(Int32 i = 0; i < lnSort; i ++)
	{
		DFCSimpleExprUnit* lpUnit = static_cast<DFCSimpleExprUnit *>(lvSortLevels[i]);
        
		bool lIsMetric = lpUnit->IsMetric();
        
		if (lIsMetric)
		{
			// qjiang re-organize the code
			hr = ConvertMetricSortSimpleExpr(lpUnit, definitions, ipEventContext, ipDimMetricGov);
			if (hr != S_OK)
            {
                //tliao, 962960, 2014/09/02, just skip it
                hr = S_OK;
                continue;
            }
		}
		else
		{
			DSS_ID lAttributeID = lpUnit->GetAttrID();
            
			DSSTemplateUnit_Type lType = DssTemplateReserved;
			Int32 lUnit;
            
            hr = _model -> FindUnitI(lAttributeID, DssTemplateAttribute, lUnit);
            if (hr == S_OK)
            {
                lType = DssTemplateAttribute;
            }
            else
            {
                hr = _model -> FindUnitI(lAttributeID, DssTemplateConsolidation, lUnit);
                if (hr == S_OK)
                {
                    lType = DssTemplateConsolidation;
                }
                else
                {
            
                    hr = _model -> FindUnitI(lAttributeID, DssTemplateCustomGroup, lUnit);
                    if (hr == S_OK)
                    {
                        lType = DssTemplateCustomGroup;
                    }
                }
            }

			if(hr == S_OK)
			{
				// found the unit
				switch( lType )
				{
                    case DssTemplateAttribute:
					{
						// qjiang re-organize the code
						hr = ConvertAttributeSortSimpleExpr(lpUnit, definitions);
                        
						//if (hr != S_OK)
						//	continue;
                        
						break;
					}
                    case DssTemplateConsolidation:
					{
						// DE unit, after DE is calculated
						// qjiang add support for DE
						//if (lfIsDE)
							//hr = ConvertDerivedElementFormSortSimpleExpr(lpUnit, opSortDefns);
						//else
							hr = ConvertConsolidationSortSimpleExpr(lpUnit, definitions);
						//if (hr != S_OK)
						//	continue;
                        
						break;
					}
                    case DssTemplateCustomGroup:
					{
						// DE unit, but before DE is calculated
						// qjiang add support for CG
						//if (lfIsDE)
							//hr = ConvertPreDECustomGroupSort(lpUnit, opSortDefns);
						//else
							hr = ConvertCustomGroupSortSimpleExpr(lpUnit, definitions);
						//if (hr != S_OK)
						//	continue;
                        
						break;
					}
                    default:
                        break;
				} // end switch
                
				if (hr == S_FALSE)  // TQMS 368203: should only fail the metric if a single form is missing
                {
                    *oppSortDefinition = NULL;
                    for(int i=0; i<definitions->size(); i++)
                    {
                        delete (*definitions)[i];
                    }
                    
                    delete definitions;

                    return E_FAIL;  // TQMS 244575
                }
			} // end if
            else
            {
                //tliao, 962960, 2014/09/02, the attribute is not in the TabularData, just skip it
                hr = S_OK;
            }
			
		} // end else
        
	} // end for
    
    {
        DSSSortDefinition* def = new DSSSortDefinition(_model);
        def->sortType = DssSortValue;
        def->subtotalPosition = DssSubtotalsPositionLast;
        def->isNullOnTop = false;
        def->isAscending = true;
        
        definitions->push_back(def);
    }
    
    // tehe, TQMS818895
    if(hr == S_OK)
    {
        *oppSortDefinition = definitions;
    }
    else
    {
        *oppSortDefinition = NULL;
        for(int i=0; i<definitions->size(); i++)
        {
            delete (*definitions)[i];
        }
        
        delete definitions;
    }

	return hr = S_OK;
}

int DSSSortConverter::ConvertMetricSortSimpleExpr(DFCSimpleExprUnit           *ipSort,
                                                  vector<DSSSortDefinition*>  *definitions,
                                                  EventContext                *ipEventContext,
                                                  DSSMetric                   *ipDimMetricGov)
{
    int hr = S_OK;
    
	DSSTabularMetrics* metrics = hGetMetricUnit();
	int metricID;
	GUID lMetricID=ipSort->GetMetricID();
    
	if(metrics==NULL || (metricID=metrics->FindMetric(lMetricID))<0)
		return S_FALSE;
    
    DSSSliceID lSliceID;
    int lMIndex = 0;
    DSSCalculationPlan *ipCalculationPlan = ipEventContext->mpCalculationPlan;
    if (ipDimMetricGov)
    {
        GUID iDimMetricGovID = ipDimMetricGov->getID();
        lMIndex = ipCalculationPlan->getMetricIndexWithGoverner(lMetricID, iDimMetricGovID);
    }
    
    if (!ipDimMetricGov || lMIndex == -1)
    {
        lMIndex = ipCalculationPlan->getMetricIndex(lMetricID);
    }
    
    bool lfCalculated = false;
    int iLevel = ipEventContext->mLevel;
	int iEvent = ipEventContext->mpEvent->mEventIndex;
    hr = ipCalculationPlan->getMetricSliceID(iLevel, iEvent, lMIndex, &lfCalculated, &(lSliceID.mSliceID));
    CHECK_HR;
    
    if (!lfCalculated)
        return S_FALSE;
    
    lSliceID.mpCube = ipEventContext->mpCube;
    
    EnumDSSSubtotalsPosition subtotalPosition=DssSubtotalsPositionLast;//determineSubtotalPosition(sort);
	EnumDSSSortType sortType=DssSortMetric;
	bool isAscending=ipSort->GetSortOrder();
    
	DSSSortDefinition* def=new DSSSortDefinition(_model);
	def->sortType=sortType;
	def->subtotalPosition=subtotalPosition;
	def->isNullOnTop=false;
	def->isAscending=isAscending;
    def->mSliceID = lSliceID;
    
    definitions->push_back(def);
	return S_OK;

}

int DSSSortConverter::ConvertAttributeSortSimpleExpr(DFCSimpleExprUnit           *ipSort,
                                                     vector<DSSSortDefinition*>  *definitions)
{
	HRESULT hr = S_OK;
    
	Int32 lAttrIndex = 0;
	Int32 lAttrForm = 0;
	Int32 lLocale = 0;
	hr = hGetAttrFormFromUnit(ipSort, lAttrIndex, lAttrForm, lLocale);
	CHECK_HR;
    
	//juzhang, 2014-5-8, TQMS 889074
	DSSTabularUnit* lpTabularUnit;
    hr = _model -> Item(lAttrIndex, &lpTabularUnit);
    CHECK_HR_PTR(lpTabularUnit);
    
    DSSTabularAttribute* lpTabularAttribute = dynamic_cast<DSSTabularAttribute*>(lpTabularUnit);
    CHECK_PTR(lpTabularAttribute);
    
	EnumDSSSortType lSortType = ipSort->GetSortType();
    
	Int32 lColumn = 0;
	hr = lpTabularAttribute->FindFormWithLocale(0, lAttrForm, lLocale, &lColumn);
	if(hr == S_OK)
	{
		DSSDataColumn* lpColumn = NULL;
		hr = lpTabularAttribute->Item(lColumn, &lpColumn);
		CHECK_HR_PTR(lpColumn);
        
		DSSDataType_Type lColumnType = lpColumn->getDataType();
        
		// qjiang TQMS901095, handle missing column
		if (lColumnType == DssDataTypeMissing)
		{
			if (lSortType == DssSortAttributeForm)
				return S_FALSE;		//don't need check other forms as it's only called by OLAP sort convert
			else if (lSortType == DssSortAttributeDefault)
				return S_OK;
		}
		
		//only when we can find the column in the lookup table, we add the sort, otherwise, we will ignore the
		//sort defined on the form
		bool lfAscending = ipSort->GetSortOrder();
        
        DSSSortDefinition* def=new DSSSortDefinition(_model);
        def->sortType=DssSortAttributeForm;
        def->unitID=lAttrIndex;
        def->formID=lAttrForm;
        def->subtotalPosition=DssSubtotalsPositionLast;
        def->isNullOnTop=false;
        def->isAscending=lfAscending;
        definitions->push_back(def);

        /*
		CDSSXTabSortDefn* lpSortDefn = new (mpBuffer) CDSSXTabSortDefn(mpBuffer);
		hr = lpSortDefn->init(DssSortAttributeForm,
                              lAttrIndex,
                              lAttrForm,
                              DssSubtotalsPositionLast,
                              false,	// Not for display
                              false,	// Olap sort, Null on bottom
                              lLocale,
                              lfAscending);
		CHECK_HR;
        
		opSortDefns->Add(lpSortDefn);
         */
	} 
	else if (lSortType == DssSortAttributeForm)
        return S_FALSE;	
    
	return S_OK;
}

int DSSSortConverter::hGetAttrFormFromUnit(DFCSimpleExprUnit *ipUnit, Int32 &oAttrIndex, Int32 &oAttrForm, Int32 &oLocale)
{
	DSS_ID lAttrID = GUID_NULL;
    MBase::SimpleBuffer *lpBuffer = new MBase::SimpleBuffer();
    MBase::StrongPtr<MBase::SimpleBuffer> lpBufferHolder(lpBuffer);
	MBase::Allocator<Int32> lAlloc(lpBuffer);
	vector<Int32, MBase::Allocator<Int32> > lAttrForms(lAlloc);
	vector<Int32, MBase::Allocator<Int32> > lLocales(lAlloc);
	Int32 lSourceID = 0;
	
	ipUnit->GetAttrUnit(lSourceID, lAttrID, lAttrForms, lLocales);
    
	HRESULT hr = _model->FindUnitI(lAttrID, DssTemplateReserved, &oAttrIndex);
	CHECK_HR;
    
	EnumDSSSortType lSortType = ipUnit->GetSortType();
	if(lSortType == DssSortAttributeForm) //juzhang, 2014-5-5, 886896, only check the missing form for attribute form sort
	{
		//juzhang, 2014-3-31, TQMS 868827 do some check, if there is missing form ,return FAIL
		DSSTabularUnit* lpTabularUnit;
		hr = _model-> Item(oAttrIndex, &lpTabularUnit);
		CHECK_HR_PTR(lpTabularUnit);
        
		DSSTabularAttribute* lpTabularAttribute = dynamic_cast<DSSTabularAttribute*>(lpTabularUnit);
		CHECK_PTR(lpTabularAttribute);
		
		for(Int32 i = 0; i < lAttrForms.size(); i ++)
		{
			Int32 lColumn = 0;
			hr = lpTabularAttribute->FindFormWithLocale(0, lAttrForms[i], lLocales[0], &lColumn);
			if(hr != S_OK)
				return S_FALSE;
		}
	}
    
	if (lAttrForms.size() > 0)
	{
		oAttrForm = lAttrForms[0];
        
		AE_ASSERT(lLocales.size() > 0);
		oLocale = lLocales[0];
	}
    
	return S_OK;
}

int DSSSortConverter::ConvertCustomGroupSortSimpleExpr(DFCSimpleExprUnit *ipSort,
                                                       vector<DSSSortDefinition*>  *definitions)
{
    int hr = S_OK;
    
	AE_ASSERT(ipSort);
    
	EnumDSSSortType lSortType = ipSort->GetSortType();
    
	if (lSortType == DssSortAttributeDefault)
	{
		//EnumDSSOrderParentFirst IsParentFirst = DssOrderParentFirstTrue;
        
		// we might need SQL engine to read this flag
		// like:
		// ipSort->get_IsParentFirstNS()
		
		DSS_ID &CustomGroupID = ipSort->GetAttrID();
        
		Int32 lCustomGroupPos = 0;
		hr = _model -> FindUnitI(CustomGroupID, DssTemplateCustomGroup, &lCustomGroupPos);
		CHECK_HR;
        
        bool isAscending=true;
        EnumDSSSubtotalsPosition subtotalPosition=DssSubtotalsPositionLast;//determineSubtotalPosition(sort);
        bool isNullOnTop=hGetNullOnTop();
        
        DSSSortDefinition* def=new DSSSortDefinition(_model);
        def->sortType=DssSortCustomGroupDefault;
        def->unitID=lCustomGroupPos;
        def->subtotalPosition=subtotalPosition;
        def->isNullOnTop=isNullOnTop;
        def->isAscending=isAscending;
        definitions->push_back(def);
        
        /*
		//now populate the CDSSXTabSortDefn
		CDSSXTabSortDefn* lpSortDefn = new (mpBuffer) CDSSXTabSortDefn(mpBuffer);
		CHECK_NEWED_PTR(lpSortDefn);
        
		hr = lpSortDefn-> init(DssSortCustomGroupDefault,
                               lCustomGroupPos,
                               true,
                               mfDisplayNullOnTop,
                               (IsParentFirst == DssOrderParentFirstTrue),
                               DssLocaleNeutral);
        
		CHECK_HR;
        
		hr = opSortDefns ->Add(lpSortDefn);
		CHECK_HR;
        */
		return hr = S_OK;
	}
	else
	{
		// not supported for now
	}
    
	return S_OK;
}

int DSSSortConverter::ConvertConsolidationSortSimpleExpr(DFCSimpleExprUnit *ipSort,
                                                         vector<DSSSortDefinition*>  *definitions)
{
	int hr = S_OK;
    
	AE_ASSERT(ipSort);
    
	DSS_ID &lID = ipSort->GetAttrID();
	bool lfAsc = ipSort->GetSortOrder();
    
	Int32 lConsolidationPos = 0;
	hr = _model -> FindUnitI(lID, DssTemplateConsolidation, &lConsolidationPos);
	CHECK_HR;
    
	EnumDSSSubtotalsPosition subtotalPosition=DssSubtotalsPositionLast;//determineSubtotalPosition(sort);
	bool isNullOnTop=hGetNullOnTop();
    
    DSSSortDefinition* def=new DSSSortDefinition(_model);
    def->sortType=DssSortConsolidationDefault;
    def->formID=-1;
    def->unitID=lConsolidationPos;
    def->subtotalPosition=subtotalPosition;
    def->isNullOnTop=isNullOnTop;
    def->isAscending=lfAsc;
    definitions->push_back(def);

    /*
	//now populate the CDSSXTabSortDefn
	CDSSXTabSortDefn* lpSortDefn = new (mpBuffer) CDSSXTabSortDefn(mpBuffer);
	CHECK_NEWED_PTR(lpSortDefn);
    
	hr = lpSortDefn->init(DssSortConsolidationDefault,
                          lConsolidationPos,
                          true,
                          mfDisplayNullOnTop,
                          true,
                          DssLocaleNeutral,
                          mSubtotalPos,
                          lfAsc);
	CHECK_HR;
    
	hr = opSortDefns->Add(lpSortDefn);
	CHECK_HR;
    */
	return S_OK;
}

