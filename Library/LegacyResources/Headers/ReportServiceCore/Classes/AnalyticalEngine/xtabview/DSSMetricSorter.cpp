#import "DSSSorter.h"
#import "DSSSortDefinition.h"
#import "DSSXTabHeader.h"
#import "DSSXTabView.h"
#import "DSSDataColumn.h"
#import "DSSSort_includes.h"
#import "DSSMetricSorter.h"
//#import "DSSDataModelBase.h"
//#import "DSSDataModelXTabImpl.h"
#import "DSSDateTime.h"
#import "DSSBigDecimal.h"
#import "DSSCellFmtData.h"
#import "DSSDataSource.h"
#import "DSSViewDataSet.h"
#import <map>
#import <algorithm>
#import "DSSStrongPtr.h"
#import "DSSTabularData.h"
#include "DSSXTabRecursiveUnit.h"

DSSMetricSorter::DSSMetricSorter(DSSSortDefinition* definition)
{
	_model=definition->model;
	_unitID=definition->unitID;
	_metricID=definition->metricID;
	_isAscending=definition->isAscending;
	_isNullOnTop=definition->isNullOnTop;
	_elementIDs=definition->elementIDs;
	_elementIDs[_unitID]=_metricID;
	_subtotalPosition=definition->subtotalPosition;
	_level=definition->level;
	_subtotalID=definition->subtotalID;
    //_mvUnits = definition->mvUnits;
    //_mvUnits = definition->units;
    _mvUnits = definition->mvUnits;
    _mvMaskUnits = definition->mvMaskUnits;
    _mNullReplacement = definition->mNullReplacement;
    _mbForMicroChart = definition->mForMicroChart; //zhyang, sort for micro chart
    
    
    mbNewRWD = _model->getIsNewRWD();
}

DSSMetricSorter::~DSSMetricSorter() {
	int lDataSize = _values.size() * sizeof(const char*)
    + _flags.size() * sizeof(DSSDataFlag)
    + _types.size() * sizeof(EnumDSSDataType)
    + _levels.size() * sizeof(int);
	_values.clear();
	_flags.clear();
	_types.clear();
	_levels.clear();
    MEM_GOVERNOR_RELEASE(lDataSize, lDataSize)
}

int DSSMetricSorter::sort(DSSXTabHeader* header, vector<int>& items)
{
	int hr = S_OK;
	
	int count=items.size();
	if(count<=1)
		return hr;
    
	DSSXTabView* view=hGetXTabView();
	DSSXTabHeader* rowHeader=view->getHeader(0);
	int columnID;
    
    if(!mbNewRWD)
    {
        if(header==rowHeader)
            columnID=view->getHeader(1)->getOffsetByElements(_elementIDs);
        else
            columnID=rowHeader->getOffsetByElements(_elementIDs);
        if(columnID<0)
            return hr;
	}
    
	int i;
	MBase::CString nullString;
	hGetNullReplacement(nullString);
    char *pEnd = NULL;
	double nullReplacement = strtod(nullString.c_str(), &pEnd);
    bool hasValidNullReplacement = (nullString.c_str() != pEnd);
	bool hasNullReplacement=strcasecmp(nullString.c_str(),"NaN") && hasValidNullReplacement;
    
	int lDataSize = _values.size() * sizeof(const char*)
    + _flags.size() * sizeof(DSSDataFlag)
    + _types.size() * sizeof(EnumDSSDataType)
    + _levels.size() * sizeof(int);
	_values.clear();
	_flags.clear();
	_types.clear();
	_levels.clear();
    MEM_GOVERNOR_RELEASE(lDataSize, lDataSize)
	
	lDataSize = count * (sizeof(const char*)
						 + sizeof(DSSDataFlag)
						 + sizeof(EnumDSSDataType)
						 + sizeof(int));
	{
        MEM_GOVERNOR_RESERVE(lDataSize, lDataSize)
        
        int tag = _subtotalID;
        if(_level <= 0)
        {
            tag = -1;
        }
        
        vector<int> items2;
        if(_mbForMicroChart)
        {
            hr = buildRowMapForMicroChart(header, items, items2);
            CHECK_HR
        }
        
        for (i=0; i<count; ++i)
        {
            //int rowID=header->AdjustRowByMetricID(items[i], _metricID);
            int rowID=header->AdjustRowByMetricID(_mbForMicroChart ? items2[i] : items[i], _metricID);
            const char *data;
            int size;
            EnumDSSDataType type;
            DSSDataFlag flag;
            int actualUnits;
            
            if(!mbNewRWD)
            {
                view->GetRawCellInfo(header==rowHeader ? rowID : columnID,
                                     header==rowHeader ? columnID : rowID,
                                     &data, size, type, flag);
            }
            else
            {
                //zhyang, 547768, level up when doing hierachical sort, only for new rwd
                //I didn't familar with old rwd logic so I can't fix it old rwd
                //need to fix it later
                
                /*
                 rowID = header->getSubtotalRowIndex(rowID, _level, _mvUnits, _subtotalID);
                 if(rowID != -1)
                 {
                 view->GetRawCellInfo2(header==rowHeader ? rowID : columnID,
                 header==rowHeader ? columnID : rowID,
                 &data, size, type, flag, NULL, NULL, NULL, false, true, header==rowHeader);
                 }*/
                
                //the elementIDs is by dataunit id, change it to display unit id.
                const vector<DSSTabularUnit*> * lvRAUnits = _model->getRecursiveUnits();
                
                if (lvRAUnits && _elementIDs.size() > 1) {
                    std::map<int, int> lElementIDCopy = _elementIDs;
                    _elementIDs.clear();
                    map<int, int>::iterator it;
                    for (it = lElementIDCopy.begin(); it != lElementIDCopy.end(); it++)
                    {
                        int lUnitID = it->first;
                        
                        int lNewUnitID = lUnitID;
                        
                        for (int j = 0; j < lvRAUnits->size(); j++){
                            DSSXTabRecursiveUnit* lpRAUnit = static_cast<DSSXTabRecursiveUnit*>((*lvRAUnits)[j]);
                            
                            if (lpRAUnit && lpRAUnit->getDataUnit() == lUnitID) {
                                lNewUnitID = lpRAUnit->getUnitIndex();
                                break;
                            }
                        }
                        _elementIDs[lNewUnitID] = it->second;
                    }
                }
                

                hr = view->getRawCellInfo2(_elementIDs, rowID, header!=rowHeader, _mvMaskUnits, _metricID, 0, tag, &data, &size, &type, &flag, actualUnits);
            }
            /*if(rowID == -1)
             {
             _values.push_back(NULL);
             _flags.push_back(DssDataInvalid);
             _types.push_back(DssDataTypeDouble);
             }
             else */
            //zhyang, what kind of null?
            if(hr != S_OK)
            {
                _values.push_back(data);
                _flags.push_back(DssDataInvalidCell);
                _types.push_back(type);
            }
            else if(flag && hasNullReplacement) // null
            {
                _values.push_back((char*)&nullReplacement);
                _flags.push_back(0);
                _types.push_back(DssDataTypeDouble);
            }
            else
            {
                _values.push_back(data);
                _flags.push_back(flag);
                _types.push_back(type);
            }
            //_levels.push_back(-(header->getNumUnitsForCertainRow(rowID, _mvUnits)));
            //if(flag == DssDataOk)
            
            if(mbNewRWD)
            {
                if(hr == S_OK)
                    _levels.push_back(-actualUnits);
                else
                    _levels.push_back(-_mvUnits.size());
            }
            else //746893
            {
                _levels.push_back(header->getLevel(rowID));
            }
            
            //zhyang, for old logic, we use getLevel Function to get the row level
            //for new logic, if _level is 0, we can still use this method.
            /*   if(!mbNewRWD || 0 == _level)
             {
             _levels.push_back(header->getLevel(rowID));
             }
             else
             {
             int lnTotalUnits = view->mnTotalUnits;
             
             DSSXTabKey *lpKeys = new DSSXTabKey[lnTotalUnits];
             CHECK_NEWED_PTR(lpKeys);
             MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lpKeysPtr(lpKeys);
             
             for(int i = 0; i < lnTotalUnits; ++i)
             {
             lpKeys[i] = DSSXTAB_TOTAL_KEY;
             }
             
             //mask
             if(header!=rowHeader)
             {
             hr = view->getHeader(1)->FillIndex(rowID, lnTotalUnits, lpKeys, &_metricID);
             CHECK_HR;
             
             }else
             {
             hr = view->getHeader(0)->FillIndex(rowID, lnTotalUnits, lpKeys, &_metricID);
             CHECK_HR;
             }
             
             
             int lTag = 0;
             if(_level != 0)
             {
             lTag = _subtotalID;
             }
             else
             {
             //mask
             if(header!=rowHeader)
             {
             hr = view->getHeader(1)->getSubtotalID(rowID, lTag);
             CHECK_HR;
             }else
             {
             hr = view->getHeader(0)->getSubtotalID(rowID, lTag);
             CHECK_HR;
             }
             }
             
             int lDummyUnitIndex = view->mDummyUnitIndex;
             for(int i = 0; i < _mvUnits.size(); ++i)
             {
             lpKeys[_mvUnits[i]] = DSSXTAB_TOTAL_KEY;
             if(-1 != lDummyUnitIndex)
             {
             lpKeys[lDummyUnitIndex] = DSSXTAB_TOTAL_KEY;
             }
             }
             
             vector<int> lvUnits;
             for(int i = 0; i < lnTotalUnits; ++i)
             {
             if(DSSXTAB_TOTAL_KEY != lpKeys[i])
             lvUnits.push_back(i);
             }
             _levels.push_back(header->getLevelByUnits(lvUnits, lTag));
             }*/
        }
        MEM_GOVERNOR_COMMIT(lDataSize)
	}
	
    hr = S_OK;
    MEM_GOVERNOR_RESERVE(count * sizeof(int) * 2, count * sizeof(int) * 2)//for vector<int>rank & vector<int>oldItems
	vector<int> ranks;
	for(i=0;i<count;++i)
		ranks.push_back(i);
    
    int lOriginalLevel;
    if(mbNewRWD) //746893
    {
        //zhyang, metric sort
        lOriginalLevel = _level;
        if(_level == 0)
        {
            _level = -(header->getNumUnits());
            if(header->HasMetric())
                _level++;
        }
        //zhyang, HS
        else
        {
            _level= -_mvUnits.size();
            if(_mbForMicroChart)
                _level--;
        }
    }
    
	_xtab_sort(&ranks[0], &ranks[0] + count, *this);
    
    
    if(mbNewRWD) //746893
    {
        //677108 & 675319
        _level = lOriginalLevel;
    }
	
	vector<int> oldItems=items;
	
    vector<int> metricOrder;
    metricOrder.resize(ranks.size());
    
	for(i=0;i<ranks.size();++i)
        metricOrder[ranks[i]] = i;
    MEM_GOVERNOR_COMMIT(count* sizeof(int)*2)
	items.clear();
    items.resize(metricOrder.size());
    hr = respectHierarchyRightMost(header, oldItems, metricOrder, items);
    //non-financial report case
    if (hr == S_FALSE) {
        for(i=0;i<ranks.size();++i){
            items[i] = oldItems[ranks[i]];
        }
        hr = S_OK;
    }
    ranks.clear();
    oldItems.clear();
    metricOrder.clear();
    MEM_GOVERNOR_RELEASE(count*sizeof(int)*2, count*sizeof(int)*2)
	return hr;
}
//inputRank[rank] = rowIndex, output same
//metricOrder[rowIndex] = rank
int DSSMetricSorter::respectHierarchyRightMost(DSSXTabHeader* header, vector<int>& inputRank, vector<int>& metricOrder, vector<int>& outputRank)
{
    int hr = S_OK;
    if (!header) {
        return hr;
    }
    //1, to find the recursive unit on which we respect hierarchy. And also the dssxtabheader
    int lnUnits = header->getNumUnits();
    DSSXTabRecursiveUnit* lpSortUnit = NULL;
    int lSortUnitIndex = -1;
    int lPreSortUnitIndex = -1;
    for (int i = lnUnits - 1; i >= 0; i--) {
        DSSTabularUnit* lpUnit = _model->Item(header->getUnit(i));
        if (lpUnit && lpUnit->IsRecursiveUnit()) {
            lpSortUnit = static_cast<DSSXTabRecursiveUnit*>(lpUnit);
            lSortUnitIndex = i;
            
            if (i >= 1) {
                DSSTabularUnit* lpPreUnit = _model->Item(header->getUnit(i - 1));
                if (lpPreUnit && lpPreUnit->IsRecursiveUnit()) {
                    lPreSortUnitIndex = i -1;
                }
            }
            break;
        }
    }
    //no recursive unit found, just return
    if(lSortUnitIndex == -1)
    {
        return S_FALSE;
    }
    
    DSSXTabHeader* lpSortHeader = header;
    
    //2, convert row index ranking array to display elementKey bucket array.
    // and also help information to convert back
    int lnElement = lpSortUnit->CountDisplayRows();
    int lnBucket =  lnElement + 1;
    int lnRows = inputRank.size();
    
    Int32* lpMetricOrder = new Int32 [lnBucket];
    CHECK_NEWED_PTR(lpMetricOrder);
    memset(lpMetricOrder, -1, sizeof(Int32) * lnBucket);
    
    Int32* lpStart = new Int32 [lnBucket];
    CHECK_NEWED_PTR(lpStart);
    memset(lpStart, -1, sizeof(Int32) * lnBucket);
    
    Int32* lpEnd = new Int32 [lnBucket];
    CHECK_NEWED_PTR(lpEnd);
    memset(lpEnd, -1, sizeof(Int32) * lnBucket);
    
    Int32* lpBucketList = new Int32 [lnRows];
    CHECK_NEWED_PTR(lpBucketList);
    memset(lpBucketList, -1, sizeof(Int32) * lnRows);
    
    Int32* lpElementOrder = new Int32 [lnElement];
    CHECK_NEWED_PTR(lpElementOrder);
    
    Int32 lRow;
    Int32 lBucket = 0;
    if (lPreSortUnitIndex == -1) {
        for (Int32 iRow = 0; iRow < lnRows; iRow++)
        {
            lRow = inputRank[iRow];
            
            int lElementKey = lpSortHeader->getUnitKey(lRow, lSortUnitIndex);
            
            if (lElementKey < 0) {//put it to the subtotal link list
                lBucket = lnElement;
            } else {
                lBucket = lElementKey;
            }
            
            AE_ASSERT(lBucket >= 0 && lBucket < lnBucket);
            
            // since there is no subtotal, get the first metric value for the recursive element to compare
            if (lpMetricOrder[lBucket] == -1)
                lpMetricOrder[lBucket] = metricOrder[inputRank[iRow]];
            
            if (lpStart[lBucket] == -1)
            {
                lpStart[lBucket] = iRow;
                lpEnd[lBucket] = iRow;
            }
            else
            {
                lpBucketList[lpEnd[lBucket] ] = iRow;
                lpEnd[lBucket] = iRow;
            }
        }
        //3,sort the bucket array by hierarchy
        // lpElementOrder[i] = j: the jth element should be put in the ith position, means rank is i
        // should handle element whose metric order is -1
        hr = lpSortUnit->Sort(lpMetricOrder, lnElement, lpElementOrder, false);
        CHECK_HR;
        
        //4, convert the result back to row index ranking array
        Int32 lCurRow = 0;
        int * lpResultRow = &outputRank[0];
        for (Int32 i = 0; i < lnBucket; i++)
        {
            lBucket = i == lnElement ? lnElement: lpElementOrder[i];
            
            lCurRow = lpStart[lBucket];
            
            while (lCurRow != -1)
            {
                *lpResultRow = inputRank[lCurRow];
                lpResultRow++;
                lCurRow = lpBucketList[lCurRow];
            }
        }
    }
    else{
        int lStartRow = inputRank[0];
        int lElementKey = lpSortHeader->getUnitKey(lStartRow, lSortUnitIndex);
        int lPreSortKey = lpSortHeader->getUnitKey(lStartRow, lPreSortUnitIndex);
        int lNextElementKey, lPreSortKey2;
        int * lpResultRow = &outputRank[0];
        
        for (Int32 iRow = 0; iRow < lnRows; iRow++)
        {
            if (lElementKey < 0) {//put it to the subtotal link list
                lBucket = lnElement;
            } else {
                lBucket = lElementKey;
            }
            
            AE_ASSERT(lBucket >= 0 && lBucket < lnBucket);
            
            if (iRow < lnRows - 1) {
                int lNextRow = inputRank[iRow +1];
                lNextElementKey = lpSortHeader->getUnitKey(lNextRow, lSortUnitIndex);
                lPreSortKey2 = lpSortHeader->getUnitKey(lNextRow, lPreSortUnitIndex);
            }
            
            // since there is no subtotal, get the first metric value for the recursive element to compare
            if (lpMetricOrder[lBucket] == -1)
                lpMetricOrder[lBucket] = metricOrder[inputRank[iRow]];
            
            if (lpStart[lBucket] == -1)
            {
                lpStart[lBucket] = iRow;
                lpEnd[lBucket] = iRow;
            }
            else
            {
                lpBucketList[lpEnd[lBucket] ] = iRow;
                lpEnd[lBucket] = iRow;
            }
            
            if ( ( (lPreSortKey != lPreSortKey2) && (lElementKey != lNextElementKey) ) || (iRow == lnRows - 1) )
            {
                // the next block, sort the recursive elements in the right-most recursive unit in the previous block
                // lpElementOrder[i] = j: the jth element should be put in the ith position, means rank is i
                // should handle element whose metric order is -1
                /*
                 *	mchen, "if" branch is a functional subset of the "else" branch, it exists for optimization.
                 *	but there is a case that in filter panel we only select child element, here lpStart[0] = -1 but in the index 1,2,... lpStart may be not -1
                 *	set false in the entrance, need to review it in the future.
                 */
                {
                    hr = lpSortUnit->Sort(lpMetricOrder, lnElement, lpElementOrder, false);
                    CHECK_HR;
                    
                    Int32 lCurRow = 0;
                    for (Int32 i = 0; i < lnBucket; i++)
                    {
                        lBucket = i == lnElement ? lnElement: lpElementOrder[i];
                        
                        lCurRow = lpStart[lBucket];
                        
                        while (lCurRow != -1)
                        {
                            *lpResultRow = inputRank[lCurRow];
                            lpResultRow++;
                            lCurRow = lpBucketList[lCurRow];
                        }
                    }
                }
                
                lPreSortKey = lPreSortKey2;
                
                // prepare data for the next block
                memset(lpMetricOrder, -1, sizeof(Int32) * lnBucket);
                memset(lpStart, -1, sizeof(Int32) * lnBucket);
                memset(lpEnd, -1, sizeof(Int32) * lnBucket);
            }
            
            lElementKey = lNextElementKey;
            
        }
    }
    
    return hr;
}

bool DSSMetricSorter::operator()(int a, int b)
{
	if(_subtotalPosition!=DssSubtotalsPositionMix)
	{
		if(_levels[a]<_level || _levels[b]<_level)
		{
			if((_levels[a]<_level && _levels[b]<_level) || (_levels[a] > 0 && _levels[b] > 0))
				return false;
			else if(_levels[a]<_level)
				return _subtotalPosition==DssSubtotalsPositionFirst ? false : true;
			else
				return _subtotalPosition==DssSubtotalsPositionFirst ? true : false;
		}
		
		if(_levels[a]>_level || _levels[b]>_level)
		{
			//421169: ignore subtotals even if one of them is at the target level
			if((_levels[a]>_level && _levels[b]>_level) || (_levels[a] > 0 && _levels[b] > 0))
				return false;
			else if(_levels[a]>_level)
				return _subtotalPosition==DssSubtotalsPositionFirst ? true : false;
			else
				return _subtotalPosition==DssSubtotalsPositionFirst ? false : true;
		}
	}
    
    const char* va=_values[a];
	const char* vb=_values[b];
	DSSDataFlag fa=_flags[a];
	DSSDataFlag fb=_flags[b];
    
	if(fa || fb)//either is NULL
	{
		if(fa != DssDataOk && fb != DssDataOk) return false;
		if(_isNullOnTop) return fa ? true : false;
		return fa ? false : true;
	}
	
	if(_types[a]!=_types[b])
	{
		if(isNumerical(_types[a]) && isNumerical(_types[b]))
		{
			//419164,null replacement and descending
			if(_isAscending)
			{
				return toDouble(va,_types[a]) < toDouble(vb,_types[b]);
			}else
			{
				return toDouble(va,_types[a]) > toDouble(vb,_types[b]);
			}
		}
        //tliao, 2/18/2014, 848175, handle "a is null, b is non-numerical" case
		else if (isNumerical(_types[a]))    //a is null replacement and b is non-numerical
        {
            return _isAscending ? true : false;
        }
        else if (isNumerical(_types[b]))    //b is null replacement and a is non-numerical
        {
            return _isAscending ? false : true;
        }
        else
			return false;
	}
	bool result=false;//no change
    
	switch(_types[a])
	{
		case DssDataTypeShort:
		{
			if(_isAscending)
			{
				result=*(short*)va < *(short*)vb;
			}
			else
			{
				result=*(short*)vb < *(short*)va;
			}
            
			break;
		}
		case DssDataTypeInteger:
		{
			if(_isAscending)
			{
				result=*(int*)va < *(int*)vb;
			}else
			{
				result=*(int*)vb < *(int*)va;
			}
            
			break;
		}
		case DssDataTypeUnsigned:
		{
			if(_isAscending)
			{
				result=*(unsigned int*)va < *(unsigned int*)vb;
			}else
			{
				result=*(unsigned int*)vb < *(unsigned int*)va;
			}
            
			break;
		}
        case DssDataTypeLong:
        case DssDataTypeInt64:
		{
			if(_isAscending)
			{
				result=*(long long*)va < *(long long*)vb;
			}else
			{
				result=*(long long*)vb < *(long long*)va;
			}
            
			break;
		}
		case DssDataTypeFloat:
		case DssDataTypeReal:
		{
			if(_isAscending)
			{
				result=*(float*)va < *(float*)vb;
			}else
			{
				result=*(float*)vb < *(float*)va;
			}
            
			break;
		}
		case DssDataTypeDouble:
		case DssDataTypeNumeric:
		case DssDataTypeDecimal:
		{
			if(_isAscending)
			{
				result=*(double*)va < *(double*)vb;
			}else
			{
				result=*(double*)vb < *(double*)va;
			}
            
            break;
		}
		case DssDataTypeBool:
		case DssDataTypeMissing:
		{
			if(_isAscending)
			{
				result=*(bool*)va < *(bool*)vb;
			}else
			{
				result=*(bool*)vb < *(bool*)va;
			}
            break;
		}
		case DssDataTypeDate:
		case DssDataTypeTime:
		case DssDataTypeTimeStamp:
		{
			if(_isAscending)
			{
				result=*(MDataType::DSSDateTime*)va < *(MDataType::DSSDateTime*)vb;
			}else
			{
				result=*(MDataType::DSSDateTime*)vb < *(MDataType::DSSDateTime*)va;
			}
			break;
		}
		case DssDataTypeCellFormatData:
		{
			if(_isAscending)
			{
				result=*(MDataType::DSSCellFmtData*)va < *(MDataType::DSSCellFmtData*)vb;
			}else
			{
				result=*(MDataType::DSSCellFmtData*)vb < *(MDataType::DSSCellFmtData*)va;
			}
            
            break;
		}
		case DssDataTypeBigDecimal:
		{
			if(_isAscending)
			{
				result=*(MDataType::DSSBigDecimal*)va < *(MDataType::DSSBigDecimal*)vb;
                
			}else
			{
				result=*(MDataType::DSSBigDecimal*)vb < *(MDataType::DSSBigDecimal*)va;
			}
            
			break;
		}
		case DssDataTypeChar:
		case DssDataTypeVarChar:
		case DssDataTypeNChar:
		case DssDataTypeNVarChar:
		case DssDataTypeLongVarChar:
		case DssDataTypeMBChar:
		case DssDataTypeUTF8Char:
		case DssDataTypeBinary:
		case DssDataTypeVarBin:
		case DssDataTypeLongVarBin:
			if(_isAscending)
			{
				result=strcasecmp((const char*)va, (const char*)vb)<0;
			}
			else
			{
				result=strcasecmp((const char*)vb, (const char*)va)<0;
			}
            break;
		default:
			break;
            
			
	}
	return result;
}

double DSSMetricSorter::toDouble(const char* value, EnumDSSDataType type)
{
	switch(type)
	{
		case DssDataTypeShort:
			return (double)(*(short*)value);
		case DssDataTypeInteger:
			return (double)(*(int*)value);
		case DssDataTypeUnsigned:
			return (double)(*(unsigned int*)value);
		case DssDataTypeLong:
			return (double)(*(long long*)value);
		case DssDataTypeFloat:
		case DssDataTypeReal:
			return (double)(*(float*)value);
		case DssDataTypeDouble:
		case DssDataTypeNumeric:
		case DssDataTypeDecimal:
			return *(double*)value;
		case DssDataTypeBool:
		case DssDataTypeMissing:
			return (double)(*(bool*)value);
		case DssDataTypeCellFormatData:
			return ((MDataType::DSSCellFmtData*)value)->GetValue();
        default:
            return 0;
	}
}

bool DSSMetricSorter::isNumerical(EnumDSSDataType type)
{
	return type==DssDataTypeShort ||
    type==DssDataTypeInteger ||
    type==DssDataTypeUnsigned ||
    type==DssDataTypeLong ||
    type==DssDataTypeFloat ||
    type==DssDataTypeReal ||
    type==DssDataTypeDouble ||
    type==DssDataTypeNumeric ||
    type==DssDataTypeDecimal ||
    type==DssDataTypeBool ||
    type==DssDataTypeMissing ||
    type==DssDataTypeCellFormatData;
}

DSSXTabView* DSSMetricSorter::hGetXTabView()
{
    return _model->getXTabView();
}
bool DSSMetricSorter::hGetNullReplacement(MBase::CString &iNullString)
{
    iNullString = _mNullReplacement;
    return true;
}

int DSSMetricSorter::buildBucket(DSSXTabHeader* header)
{
    if(_buckets.size() == 0)
    {
        DSSXTabView* view=hGetXTabView();
        DSSXTabHeader* rowHeader=view->getHeader(0);
        int columnID;
        
        if(!mbNewRWD)
        {
            if(header==rowHeader)
                columnID=view->getHeader(1)->getOffsetByElements(_elementIDs);
            else
                columnID=rowHeader->getOffsetByElements(_elementIDs);
            if(columnID<0)
                return -1; //zhyang, column is not exist, do not change
        }
        
        int i;
        MBase::CString nullString;
        hGetNullReplacement(nullString);
        char *pEnd = NULL;
        double nullReplacement = strtod(nullString.c_str(), &pEnd);
        bool hasValidNullReplacement = (pEnd != nullString.c_str());
        bool hasNullReplacement=strcasecmp(nullString.c_str(),"NaN") && hasValidNullReplacement;
        
        int lDataSize = _values.size() * sizeof(const char*)
		+ _flags.size() * sizeof(DSSDataFlag)
		+ _types.size() * sizeof(EnumDSSDataType)
		+ _levels.size() * sizeof(int);
        _values.clear();
        _flags.clear();
        _types.clear();
        _levels.clear();
        MEM_GOVERNOR_RELEASE(lDataSize, lDataSize)
        
        lDataSize = count * (sizeof(const char*)
                             + sizeof(DSSDataFlag)
                             + sizeof(EnumDSSDataType)
                             + sizeof(int));
        {
            MEM_GOVERNOR_RESERVE(lDataSize, lDataSize)
            for (i=0; i<count; ++i)
            {
                int rowID=header->AdjustRowByMetricID(i, _metricID);
                const char *data;
                int size;
                EnumDSSDataType type;
                DSSDataFlag flag;
                
                if(!mbNewRWD)
                {
                    view->GetRawCellInfo(header==rowHeader ? rowID : columnID,
                                         header==rowHeader ? columnID : rowID,
                                         &data, size, type, flag);
                }
                else
                {
                    //zhyang, 547768, level up when doing hierachical sort, only for new rwd
                    //I didn't familar with old rwd logic so I can't fix it old rwd
                    //need to fix it later
                    /*
                     rowID = header->getSubtotalRowIndex(rowID, _level, _mvUnits, _subtotalID);
                     if(rowID != -1)
                     {
                     view->GetRawCellInfo2(header==rowHeader ? rowID : columnID,
                     header==rowHeader ? columnID : rowID,
                     &data, size, type, flag, NULL, NULL, NULL, false, true, header==rowHeader);
                     }*/
                    //zhyang, I think this should not be called any more
                    assert(false);
                    // view->getRawCellInfo2(_elementIDs, rowID, header!=rowHeader, _mvUnits, _metricID, 0, _subtotalID, &data, &size, &type, &flag);
                }
                /*if(rowID == -1)
                 {
                 _values.push_back(NULL);
                 _flags.push_back(DssDataInvalid);
                 _types.push_back(DssDataTypeDouble);
                 }
                 else */
                if(flag && hasNullReplacement) // null
                {
                    _values.push_back((char*)&nullReplacement);
                    _flags.push_back(0);
                    _types.push_back(DssDataTypeDouble);
                }
                else
                {
                    _values.push_back(data);
                    _flags.push_back(flag);
                    _types.push_back(type);
                }
                _levels.push_back(header->getLevel(rowID));
            }
            MEM_GOVERNOR_COMMIT(lDataSize)
        }
        
        MEM_GOVERNOR_RESERVE(count * sizeof(int) * 2, count * sizeof(int) * 2)//for vector<int>rank & vector<int>oldItems
        
        for(i=0;i<count;++i)
            _ranks.push_back(i);
        _xtab_sort(&_ranks[0], &_ranks[0] + count, *this);
        
        if (count > 0)		//477477, it's a runtime unit, only has total
		{
            _buckets.resize(count);
            int rank = 0;
			_buckets[_ranks[0]]=0;
			for(i=1;i<count;++i)
			{
				//if(_column->Order(ranks[i], ranks[i-1]))  // if differ from the last element
                rank++;
				_buckets[_ranks[i]]=rank;
			}
		}
    }
    
    return S_OK;
}

int DSSMetricSorter::buildRowMapForMicroChart(DSSXTabHeader* header, vector<int> items, vector<int> &iovRowMap)
{
    int hr = S_OK;
    
    map<string, int> lmKeyToRow;
    DSSXTabView* view=hGetXTabView();
    int lnTotalUnits = view->mnTotalUnits;
    vector<int> lvHeaderUnits = header->getUnits();
    
    int *lpKeys = new int[lnTotalUnits];
    CHECK_NEWED_PTR(lpKeys);
    MBase::StrongArrayPtr<int> lpKeysPtr(lpKeys);
    
    int *lpChildKeys = new int[lnTotalUnits];
    CHECK_NEWED_PTR(lpKeys);
    MBase::StrongArrayPtr<int> lpKeysPtr2(lpChildKeys);
    
    for(int i = items.size() - 1; i >= 0; --i)
    {
        for(int j = 0; j < lnTotalUnits; ++j)
            lpKeys[j] = DSSXTabTotalKey;
        
        DSSTabularIndex *lpIndex = NULL;
        int lOffset;
        hr = header->FillIndexEx(items[i], lnTotalUnits, lpKeys, &_metricID, &lpIndex, &lOffset, lpChildKeys);
        CHECK_HR;
        
        string keyStr;
        bool lbSubtotal = false;
        /*       vector<int> lvUnitMap;
         vector<int> lvIndexUnits;
         
         hr = lpIndex->getUnits(lvIndexUnits);
         CHECK_HR;
         
         for(int i = 0; i < _mvUnits.size(); ++i)
         {
         int j = 0;
         for(; j < lvIndexUnits.size(); ++j)
         {
         if(_mvUnits[i] == lvIndexUnits[j])
         {
         lvUnitMap.push_back(j);
         break;
         }
         }
         if(j == lvIndexUnits.size())
         {
         lbSubtotal = true;
         break;
         }
         }
         */
        if(!lbSubtotal)
        {
            for(int j = 0; j < _mvUnits.size(); ++j)
            {
                if(lpKeys[_mvUnits[j]] == DSSXTabTotalKey)
                {
                    //lbSubtotal = true;
                    //break;
                    keyStr+='N';
                    lpKeys[_mvUnits[j]] = - lpKeys[_mvUnits[j]];
                }
                
                int tmp = lpKeys[_mvUnits[j]];
                while(tmp != 0)
                {
                    keyStr += ('0' + tmp % 10);
                    tmp /= 10;
                }
                keyStr +='-';
            }
            /*           for(int j = 0; j < lvUnitMap.size(); ++j)
             {
             if(lpKeys[lvUnitMap[j]] == DSSXTabTotalKey)
             {
             lbSubtotal = true;
             break;
             }
             
             int tmp = lpKeys[lvUnitMap[j]];
             while(tmp != 0)
             {
             keyStr += ('0' + tmp % 10);
             tmp /= 10;
             }
             keyStr +='-';
             }*/
        }
        
        if(lbSubtotal)
        {
            iovRowMap.push_back(items[i]);
        }
        else
        {
            if(lmKeyToRow.find(keyStr) == lmKeyToRow.end())
                lmKeyToRow[keyStr] = items[i];
            iovRowMap.push_back(lmKeyToRow[keyStr]);
        }
    }
    
    assert(items.size() == iovRowMap.size());
    reverse(iovRowMap.begin(), iovRowMap.end());
    return hr;
}
