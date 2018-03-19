/*
 *  DSSDataModelBase.cpp
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 1/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import <set>
#import "DSSDataModelBase.h"
#import "DSSTabularUnit.h"
#import "DSSSort_includes.h"
#import "DSSViewMap.h"
#import "DSSBaseElementProxy.h"
#import "DSSTabularAttribute.h"
#import "DSSDataColumn.h"
#import "DSSAttributeElementProxy.h"
#import "DSSDocumentInstance.h"
#import "DSSTabularConsolidation.h"
#import "DSSSortConverter.h"
#import "DSSSortDefinition.h"
#import "DSSSorter.h"
#import "DSSDataSource.h"
#import "DSSDataModelTransaction.h"
#import "DSSDataModelControlImpl.h"
#import "DSSTabularData.h"
#import "DSSAttributeListElementProxy.h"
template<class T>
class ElementSort
{
public:
	ElementSort(T *ipKey, int nRows, int nUnits): mpKey(ipKey), mnRows(nRows), mnUnits(nUnits), mCur(0)
	{}
	
	inline void ReInit(T *ipKey, int nRows, int nUnits)
	{
		mpKey = ipKey;
		mnRows = nRows;
		mnUnits = nUnits;
		mCur = 0;
	}
	
	inline void SetUnit(int iUnit)
	{	if (iUnit < mnUnits)	mCur = iUnit; }
	
	bool operator() (int iRow1, int iRow2)
	{
		return mpKey[iRow1 * mnUnits + mCur] < mpKey[iRow2 * mnUnits + mCur];
	}
	
private:
	T *mpKey;
	int mnRows;
	int mnUnits;
	int mCur;
};

DSSDataModelBase::DSSDataModelBase():mNullOnTop(true), mpParentInfo(NULL), mpViewMap(NULL)//, mpObjMap(NULL)
,mType(DssModelReserved), mpObjContext(NULL), mpModelDataSource(NULL), mStatus(XTAB_UNINITIALIZED)
,mpTransactionModel(NULL), mCurrentSliceFlag(DssBinaryContextDetailInitialSlice)
,mpTabularData(NULL)
{
}

DSSDataModelBase::~DSSDataModelBase()
{
	hCleanUp();
	
	if (mpModelDataSource)
	{
		delete mpModelDataSource;
		mpModelDataSource = NULL;
	}
	if (mpTransactionModel)
	{
		delete mpTransactionModel;
		mpTransactionModel = NULL;
	}
}

void DSSDataModelBase::hCleanUp()
{
	int lnUnits = mvUnits.size();
	for (int i = 0; i < lnUnits; i++)
	{
		if (mvUnits[i])
		{
			delete mvUnits[i];
			mvUnits[i] = NULL;
		}
	}
	if (mpViewMap)
	{
		delete mpViewMap;
		mpViewMap = NULL;
	}
    
    mapKeyToControl.clear();   
    
    if(mpTabularData)
    {
        delete mpTabularData;
        mpTabularData = NULL;
    }
    if (!mFilePathMMP.empty())
    {
        DataSourceUtil::removeSubFolder(mFilePathMMP);
    }
}

int DSSDataModelBase::setNodeKey(MBase::CString &rNodeKey)
{
	mNodeKey = rNodeKey;
	return 0;
}

const string & DSSDataModelBase::getNodeKey()
{
	return mNodeKey;
}

EnumDSSDataModelType DSSDataModelBase::getModelType()
{
	return mType;
}

int DSSDataModelBase::AddUnit(int iUnitID, DSSTabularUnit* ipUnit)
{
	int lnUnits = mvUnits.size();
	
	if (iUnitID >= lnUnits)
	{
		for (int i = lnUnits; i < iUnitID; i++)
		{
			mvUnits.push_back(NULL);
		}
		mvUnits.push_back(ipUnit);
		
		if (iUnitID != mvUnits.size() - 1)		//TODO:[TBR] this check is for debug purpose
			return E_FAIL;
	}
	else if (mvUnits[iUnitID] == NULL)
	{
		mvUnits[iUnitID] = ipUnit;
	}
	else
		return E_INVALIDINDEX;
	return 0;
}

int DSSDataModelBase::setParentInfo(DSSDocumentInstance *ipDI)
{
	mpParentInfo = ipDI;
	mpObjContext = ipDI->getDefinition()->getObjectContext();
	return 0;
}

DSSObjectInfo * DSSDataModelBase::getParentInfo()
{
	return mpParentInfo->getDefinition();
}


//select distinct rows based on the first N keys
//irvKeyTable: a 2-D array, each row has nTotalKeys keys
//nKeys: number of keys used for sort and select distinct
int DSSDataModelBase::hSelectDistinct(vector<int> &irvKeyTable, int nKeys, int nTotalKeys, vector<int> &orSortedList, vector<int> &orDistinctList)
{
	if (nKeys == 0 || irvKeyTable.size() == 0)
		return 0;
	
	int lnRows = irvKeyTable.size() / nTotalKeys;		//nTotalKeys can be greater than nKeys
	orSortedList.resize(lnRows);
    
	int i = 0;
	for (i = 0; i < lnRows; i++)
		orSortedList[i] = i;
	
	//1. sort the original rows		TODO[opt]: use bucket sort
	int *lpKeys = &irvKeyTable[0];
	ElementSort<int> lSorter(lpKeys, lnRows, nTotalKeys);
	for (i = nKeys - 1; i >= 0; --i)
	{
		lSorter.SetUnit(i);
		_xtab_sort(&orSortedList[0], &orSortedList[0] + lnRows, lSorter);	//merge sort
	}
    
	//2. select distinct
	//orDistinctList.reserve(lnRows);
	int *lpPre = NULL;
	int *lpCur = NULL;
	if (lnRows > 0)
	{
		lpPre = lpKeys + nTotalKeys * orSortedList[0];
		//orDistinctList.push_back(orSortedList[0]);
	}
	
	for (i = 1; i < lnRows; i++)
	{
		lpCur = lpKeys + nTotalKeys * orSortedList[i];
        int j = nKeys - 1;
		for (; j >= 0; --j)
		{
			if (lpCur[j] != lpPre[j])
			{
                if(orDistinctList.size() > 0)
                    orDistinctList.push_back(orSortedList[i]);
				break;
			}
		}
        if(j < 0 && orDistinctList.size() == 0)
        {
            orDistinctList.assign(orSortedList.begin(), orSortedList.begin() + i);
        }
		lpPre = lpCur;
	}
	return 0;
}

int DSSDataModelBase::hCreateViewMap(int nUnits, int *ipUnits, int nRows, int *ipKeys, std::string iFilePath, char* pGBPage, int GBPageSizse, int GBPagePosInFile)
{
	if (mpViewMap)
		delete mpViewMap;
	mpViewMap = new DSSViewMap();
	
	vector<GUID> lvUnitGUID(nUnits);
	for (int i = 0; i < nUnits; i++)
	{
		_ASSERT(ipUnits[i] < mvUnits.size());
		int hr = mvUnits[ipUnits[i]]->getGUID(lvUnitGUID[i]);
		CHECK_HR;
	}
	
	return mpViewMap->Init(lvUnitGUID, nRows, ipKeys, iFilePath, pGBPage, GBPageSizse, GBPagePosInFile);
}

int DSSDataModelBase::hCountGroupPaths()
{
	return mpViewMap->CountRows();
}

//return true if the first N group-by units are same as mpViewMap's group-by units
inline bool DSSDataModelBase::hNeedReorderKeys(int nGBUnits, vector<GUID> &irGBUnitID)
{
	int lMyGBUnitsCount = mpViewMap->getUnitCount();
	if (nGBUnits != lMyGBUnitsCount)
		return true;
    
	for (int i = 0; i < lMyGBUnitsCount; i++)
	{
		int lUnit = mpViewMap->findGBUnit(irGBUnitID[i]);
		if (lUnit != i)
			return true;
	}
	return false;
}

int DSSDataModelBase::FindUnit(const GUID &irGUID, EnumDSSTemplateUnitType iType)
{
	int hr = S_OK;
	int lnUnit = mvUnits.size();
	GUID lID = GUID_NULL;
	EnumDSSTemplateUnitType lType = DssTemplateReserved;
	int lCandidate = -1;
    
	for(int iUnit = 0; iUnit < lnUnit; iUnit ++)
	{
		hr = mvUnits[iUnit]->getGUID(lID);
		if (hr != S_OK)
			return -1;
        
		if(MBase::IsEqualGUID(lID, irGUID))
		{
			lType = mvUnits[iUnit]->getUnitType();
            
			if (iType == DssTemplateReserved)
			{
				if (lType != DssTemplateCustomGroup)
				{
					return iUnit;
				}
				else
				{
					lCandidate = iUnit;
				}
			}
			else if(lType == iType)
			{
				return iUnit;
			}
		}
	}
    
	if (lCandidate != -1)
	{
		return lCandidate;
	}
	return -1;
}

int DSSDataModelBase::Count()
{
	return mvUnits.size();
}

DSSTabularUnit * DSSDataModelBase::Item(int index)
{
	if (index >= 0 && index < mvUnits.size())
		return mvUnits[index];
	else
		return NULL;
}

int DSSDataModelBase::FindElement(DSSBaseElementProxy *ipElement, int &orUnit, int &orKey)
{
	GUID lUnitGUID = ipElement->getObjectID();
	int lUnitIndex = FindUnit(lUnitGUID, DssTemplateReserved);
	if (lUnitIndex < 0)
		return E_FAIL;		//not a supported element
	
	DSSTabularUnit *lpUnit = Item(lUnitIndex);
	CHECK_PTR(lpUnit);
	
	orUnit = lUnitIndex;
	return lpUnit->FindElement(ipElement, orKey);
}

DSSBaseElementProxy* DSSDataModelBase::CreateElementFromElementID(MBase::CString &irElementID)
{
	EnumDSSElementType lType = DSSBaseElementProxy::ParseElementType(irElementID);
	switch (lType) {
		case DssElementMetric:
		{
            DSSMetricElementProxy *lpMetricElement = new DSSMetricElementProxy(mpObjContext);
            lpMetricElement->Parse(irElementID);
            return lpMetricElement;
		}
        case DssElementAttribute:
        {
            DSSAttributeListElementProxy* lpAttributeElement = new DSSAttributeListElementProxy(mpObjContext);
            ((DSSAttributeListElementProxy*)(lpAttributeElement))->Parse(irElementID);
            return lpAttributeElement;
        }
		case DssElementSubtotal:
		{
			DSSSubtotalElementProxy *lpSubtotalElement = new DSSSubtotalElementProxy(mpObjContext);
			lpSubtotalElement->Parse(irElementID);
			return lpSubtotalElement;
		}
		case DssElementNULL:
		{
			DSSNullElementProxy *lpNullElement = new DSSNullElementProxy(mpObjContext);
			lpNullElement->Parse(irElementID);
			return lpNullElement;
		}
		case DssElementAll:
		{
            DSSAllElementProxy *lpAllElement = new DSSAllElementProxy(mpObjContext);
            lpAllElement->Parse(irElementID);
            return lpAllElement;
		}
		case DssElementNode:
		{
            DSSNodeElementProxy *lpNodeElement = new DSSNodeElementProxy(mpObjContext);
            lpNodeElement->Parse(irElementID);
            return lpNodeElement;
		}
		default:
			break;
	}
	return NULL;
}

int DSSDataModelBase::hMapGBUnit(int nGBUnits, vector<GUID> &irGBUnitID, vector<int> &orMap, bool &orPartial)
{
	int lMyGBUnitsCount = mpViewMap->getUnitCount();
	orMap.resize(lMyGBUnitsCount);
	int i = 0;
	for (i = 0; i < lMyGBUnitsCount; i++)
		orMap[i] = -1;
	
	orPartial = false;
	
	for (i = 0; i < nGBUnits; i++)
	{
		int lUnit = mpViewMap->findGBUnit(irGBUnitID[i]);
		if (lUnit != -1)
		{
			orMap[lUnit] = i;
		}
	}
	
	for (i = 0; i < lMyGBUnitsCount; i++)
	{
		if (orMap[i] == -1)
		{
			orPartial = true;
			break;
		}
	}
	
	return S_OK;
}

void DSSDataModelBase::hCheckDuplicateGBUnits(int nGBUnits, vector<GUID> &irGBUnitID, vector<vector<int> > &orMap, bool &orbHasDuplicate)
{
	orbHasDuplicate = false;
	orMap.resize(nGBUnits);
	for (int i = 0; i < nGBUnits; i++)
	{
		for (int j = i + 1; j < nGBUnits; j++)
		{
			if (MBase::IsEqualGUID(irGBUnitID[i], irGBUnitID[j]))
			{
				if (!orbHasDuplicate)
					orbHasDuplicate = true;
				
				orMap[i].push_back(j);
			}
		}
	}
}


int DSSDataModelBase::hSelectWindows(int nPaths, int nGBUnits, vector<GUID> &irGBUnitID, int *ipGBPaths, vector<int> &orWindows)
{
	if (nGBUnits == 0)
		nPaths = 1;
	
	int hr = S_OK;
	
	orWindows.reserve(nPaths);
	int i = 0;
	bool lbReOrder = hNeedReorderKeys(nGBUnits, irGBUnitID);
	if (!lbReOrder)
	{
		for(i = 0; i < nPaths; i++)
		{
			int lSlice = 0;
			hr = mpViewMap->getView(ipGBPaths, nGBUnits, lSlice);
			if (hr == S_OK)
				orWindows.push_back(lSlice);
			ipGBPaths += nGBUnits;
		}
	}
	else	//need adjust order
	{
		vector<int> lvMap;
		bool lbPartialLookup = false;
		hr = hMapGBUnit(nGBUnits, irGBUnitID, lvMap, lbPartialLookup);
		CHECK_HR;
		
		int lMyGBUnitsCount = mpViewMap->getUnitCount();
		vector<int> lvKeys(lMyGBUnitsCount);
		int *lpMap = &lvMap[0];
		int *lpKeys = &lvKeys[0];
		
		bool lbHasDuplicateGBUnits = false;
		vector<vector<int> > lvDuplicate;
		hCheckDuplicateGBUnits(nGBUnits, irGBUnitID, lvDuplicate, lbHasDuplicateGBUnits);
		
		if (!lbPartialLookup)
		{
			for(i = 0; i < nPaths; i++)
			{
				int lSlice = 0;
				
				//lwang,415804, 415335, some group-by units may be the same attribute
				if (lbHasDuplicateGBUnits)
				{
					bool lbConflict = false;
					for (int j = 0; j < nGBUnits; j++)
					{
						if (!lvDuplicate[j].empty())
						{
							for (int k = 0; k < lvDuplicate[j].size(); k++)
							{
								if (ipGBPaths[j] != ipGBPaths[lvDuplicate[j][k]])
								{
									lbConflict = true;
									break;
								}
							}
							if (lbConflict)
								break;
						}
					}
					
					//if they are conflict, no data should be selected for this path
					if (lbConflict)
					{
						ipGBPaths += nGBUnits;
						continue;
					}
				}
				
				for (int j = 0; j < lMyGBUnitsCount; j++)
				{
					lpKeys[j] = ipGBPaths[lpMap[j]];		//reorder the groupby units
				}
				hr = mpViewMap->getView(lpKeys, lMyGBUnitsCount, lSlice);
				if (hr == S_OK)
					orWindows.push_back(lSlice);
				ipGBPaths += nGBUnits;
			}
		}		
		else
		{
			//TODO: partial lookup, caller only pass in partial groupby path
			for (i = 0; i < lMyGBUnitsCount; i++)
			{
				lvKeys[i] = DSSXTAB_TOTAL_KEY;
			}
			
			int lnRows = mpViewMap->CountRows();
			vector<int> lvSlices(lnRows);
			
			for(i = 0; i < nPaths; i++)
			{
				int j = 0;
				for (j = 0; j < lMyGBUnitsCount; j++)
				{
					if (lpMap[j] >= 0)
						lpKeys[j] = ipGBPaths[lpMap[j]];		//reorder the groupby units
				}
				
				int lCount = 0;
				hr = mpViewMap->getViewPartialMatch(lpKeys, lMyGBUnitsCount, lvSlices, lCount);
				if (hr != S_OK)
					continue;
				
				orWindows.insert(orWindows.end(), lvSlices.begin(), lvSlices.begin() + lCount);
				
				ipGBPaths += nGBUnits;
			}
		}
	}
	return S_OK;
}

int DSSDataModelBase::getGroupbyElement(vector<int> &irvPartialGBPaths,int nGBUnits, vector<GUID> &irGBUnitID, 
                                        GUID &irTargetUnitID, DSSBaseElementsProxy * ipCurrentElements, int iAutoSelectionFlag,
                                        bool &orbReplaced, vector<int> &orAutoSelection)
{
	int hr = S_OK;
	
	hr = EnsureModelData();		//434287
	CHECK_HR;
	
	orbReplaced = false;
	
	//1. find the element from this model
	int lnElements = ipCurrentElements->Count();
	vector<int> lvCurrentElements;
	lvCurrentElements.reserve(lnElements);
	int lUnitID = FindUnit(irTargetUnitID, DssTemplateReserved);
	if (lUnitID < 0)		//431046, model is empty, could be static fiedl group model.
		return S_OK;
	
	DSSTabularUnit *lpXTabUnit = Item(lUnitID);
	CHECK_PTR(lpXTabUnit);
	
	int i = 0;
	for (i = 0; i < lnElements; i++)
	{
		DSSBaseElementProxy *lpElement = ipCurrentElements->Item(i);
		EnumDSSElementType lType = lpElement->getElementType();
		if (lType != DssElementAll && lType != DssElementSubtotal)
		{
			int key = 0;
			hr = lpXTabUnit->FindElement(lpElement, key);
			if (hr == S_OK)		//432211, the current elements can be invalid
				lvCurrentElements.push_back(key);
		}
		else if (lType == DssElementSubtotal)
		{
			//TODO: how to get local subtoal key?
			// 466944: document level subtotal is pushed down so it must be there.
			return S_OK;
		}
		else
		{
			//419496: don't replace if there's ALL element
			return S_OK;
		}
	}
	
	//2. get the available element keys
	int lnPaths = 1;
	if (nGBUnits > 0)
		lnPaths = irvPartialGBPaths.size() / nGBUnits;
	
    hr = mpViewMap->MapGBKeys();
    CHECK_HR;
    
	vector<int> lvWindowID;
	hr = hSelectWindows(lnPaths, nGBUnits, irGBUnitID, &irvPartialGBPaths[0], lvWindowID);
	CHECK_HR;
	
	int lTargetUntiID = mpViewMap->findGBUnit(irTargetUnitID);
	if (lTargetUntiID < 0 && mpViewMap->getGBUnitCount()>0)
		return S_FALSE;
	
	set<int> lsCandidateKeys;
	int lnWindows = lvWindowID.size();
	for (i = 0; i < lnWindows; i++)
	{
		int lUnitKey = 0;
		hr = mpViewMap->getUnitKey(lvWindowID[i], lTargetUntiID, lUnitKey);
		if (hr != S_OK)
			continue;
		
		lsCandidateKeys.insert(lUnitKey);
	}
	
    hr = mpViewMap->UnMapGBKeys();
    CHECK_HR;
    
	//3. check if there's match
	set<int>::iterator end = lsCandidateKeys.end();
	lnElements = lvCurrentElements.size();
	for (i = 0; i < lnElements; i++)
	{
		if (lsCandidateKeys.find(lvCurrentElements[i]) != end)
		{
			return S_OK;
		}
	}
	
	//4. not found, auto select
	orbReplaced = true;
	
	lnElements = lsCandidateKeys.size();
	if (lnElements <= 0)
		return S_OK;
	
	DSSTabularConsolidation *lpDE = NULL;
	if (lpXTabUnit->getUnitType() == DssTemplateConsolidation)
	{
		lpDE = static_cast<DSSTabularConsolidation *> (lpXTabUnit);
		
		if (!lpDE->IsDerivedElement())
			lpDE = NULL;
	}
	
	vector<int> lvXTabKeys;
	lvXTabKeys.reserve(lnElements);
	set<int>::iterator it = lsCandidateKeys.begin();
	for (; it != end; it++)
	{
		if (*it < 0)		//428021, 402611, don't pick total for unset CGB in RWD over DSSCube 
			continue;
        
		if (!lpDE || lpDE->IsLeaf(*it))		//428007, don't pick branch element for unset CGB in RWD over DSSCube
			lvXTabKeys.push_back(*it);
	}
	
	lnElements = lvXTabKeys.size();
	if (lnElements <= 0)
		return S_OK;
	
	hSortElements(lUnitID, lvXTabKeys);
	
	if (iAutoSelectionFlag == 1)
	{
		orAutoSelection.push_back(lvXTabKeys[0]);
	}
	else
	{
		orAutoSelection.push_back(lvXTabKeys[lnElements - 1]);
	}
	
	return S_OK;
}

bool DSSDataModelBase::hSortElements(int unitID, vector<int>& elements)
{
	if(elements.size()<=1)
		return true;
	DSSSortConverter converter(this, hGetTabularData());
	vector<DSSSortDefinition*> definitions;
	converter.generateDefaultSortDefinition(unitID, &definitions);
	
	for(int i=definitions.size()-1; i>=0; --i)
	{
		definitions[i]->getSorter()->sortUnitElements(elements);
		delete definitions[i];
	}
	
	return false;
}

int DSSDataModelBase::hCalculateSizeOfBase()
{
	int lSize = 0;
	
	lSize += mNodeKey.size() * sizeof(char);
	
	lSize += mpViewMap->size();
	
	lSize += DSSAEHelper::SizeOfPVector(mvUnits);
	
	if (mpModelDataSource)
		lSize += mpModelDataSource->size();
	
	lSize += mDataSourcePath.size() * sizeof(char);
	
	return lSize;
}

int DSSDataModelBase::EnsureModelData()
{
	if (mStatus != XTAB_INITIALIZED)
	{
		int hr = mpModelDataSource->LoadModel(this);
		if (hr != S_OK)
		{
			ReleaseUnusedMemory();
			return hr;
		}
		
	}
	return S_OK;
}

void DSSDataModelBase::setTransactionDataModel(DSSDataModelTransaction *ipModel)
{
	if (mpTransactionModel)
		delete mpTransactionModel;
	mpTransactionModel = ipModel;
}

void DSSDataModelBase::ReleaseUnusedMemory()
{
	if (mType == DssModelXTab || mType == DssModelFieldGroup || mType == DssModelControl)
	{
		hCleanUp();
		mStatus = XTAB_UNINITIALIZED;
	}
}

int DSSDataModelBase::ResolveExcludedElements(DSSTabularUnit *ipUnit, vector<int> &irExcludedElementKeys, vector<int> &orSelectedElementKeys)
{
	int hr = S_OK;
    
	int lnTotalElements = ipUnit->CountRows();
	vector<bool> lvMask(lnTotalElements);		//bit map to mark excluded element
	int i = 0;
	for (i = 0; i < lnTotalElements; i++)
		lvMask[i] = false;
	
	//scan excluded elements
	set<int> lsTotalKey;
	for (i = 0; i < irExcludedElementKeys.size(); i++)
	{
		int key = irExcludedElementKeys[i];
		if (key >= 0)
		{
			lvMask[key] = true;		//it's an excluded element
		}
		else
		{
			lsTotalKey.insert(key);
		}
	}
	
	GUID lUnitID = GUID_NULL;
	ipUnit->getGUID(lUnitID);
	
	orSelectedElementKeys.clear();
	
	//save included elements based on the bit map
	for (i = 0; i < lnTotalElements; i++)
	{
		if (!lvMask[i])
		{
			orSelectedElementKeys.push_back(i);
		}
	}
	hr = mpViewMap->MapGBKeys();
    CHECK_HR;
	//get the included subtotal keys
	hr = mpViewMap->getSubtotalKeys(lUnitID, lsTotalKey, orSelectedElementKeys);
	CHECK_HR;
    
    hr = mpViewMap->UnMapGBKeys();
    CHECK_HR;
	
	return S_OK;
}

int DSSDataModelBase::CopyTransactionDataModel(DSSDataModelBase *ipSourceDataModel, bool ibForceCopy)
{
	if (!ipSourceDataModel->mpTransactionModel)
		return S_OK;
	
	if (mpTransactionModel)
		return E_FAIL;		//unexpected if transaction model is already created
    
    if(!ibForceCopy)
    {
        if (! (ipSourceDataModel->mCurrentSliceFlag == DssBinaryContextDetailInitialSlice
               && this->mCurrentSliceFlag == DssBinaryContextDetailInitialLayout))
            return S_OK;		//only transfer transaction change from current slice to current layout
    }
    
	mpTransactionModel = new DSSDataModelTransaction();
	int hr = mpTransactionModel->Init(ipSourceDataModel->mpTransactionModel, this);
	if (hr != S_OK)
	{
		delete mpTransactionModel;
		mpTransactionModel = NULL;
//		_ASSERT(false);//771771
	}
	return S_OK;
}

int DSSDataModelBase::getViewMap(DSSViewMap** oppViewMap)
{
	int hr = S_OK;
	
	if(oppViewMap)
		(*oppViewMap) = mpViewMap;
	
	return hr;
}

int DSSDataModelBase::addControlModel(DSSDataModelControlImpl* ipControl)
{
    int lnControls = mvControlModels.size();
    std::string lControlKey = ipControl->getNodeKey();
    mapKeyToControl[lControlKey] = lnControls;
    mvControlModels.push_back(ipControl);
    return S_OK;
}
int DSSDataModelBase::getControlModelCount(int* opCount)
{
    if(opCount)
        *opCount = mvControlModels.size();
    
    return S_OK;
}

int DSSDataModelBase::getControlModel(std::string iKey, DSSDataModelControlImpl** oppControl)
{
    if(mapKeyToControl.find(iKey) == mapKeyToControl.end())
        return S_FALSE;
    
    if(oppControl)
    {
        int lIndex = mapKeyToControl[iKey];
        if(lIndex < 0 || lIndex >= mvControlModels.size())
            return E_INVALIDINDEX;
        
        (*oppControl) = mvControlModels[lIndex];
    }
    
    return S_OK;
}

DSSTabularData * DSSDataModelBase::hGetTabularData()
{
    if (!mpTabularData)
    {
        mpTabularData = new DSSTabularData();
        mpTabularData->Init(this, mvUnits);
    }
    return mpTabularData;
}
