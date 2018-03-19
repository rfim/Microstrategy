/*
 *  DSSDataModelControlImpl.cpp
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import <set>
#import <algorithm>
#import "DSSDataModelControlImpl.h"
#import "DSSViewMap.h"
#import "DSSDataSource.h"
#import "PDCHeader/PDCwinnt.h"
#import "MsiCOM/MsiCOM/Msi_atldef.h"
#import "MsiCOM/MsiCOM/Msi_objbase.h"
#import "MsiCOM/MsiCOM/Msi_oleauto.h"
#import "MsiCOM/MsiCOM/Msi_ccomvariant.h"

DSSDataModelControlImpl::DSSDataModelControlImpl():mnElements(0), mGhostFlag(0)
, mpMetricValueMin(NULL), mpMetricValueMax(NULL), mpMetricValueCount(NULL)
, mnSlice(0),mnElementListCount(0),mpElementOffset(NULL),mpElementList(NULL)
,mbMapped(false),mpElementListPage(NULL),mpElementOffsetPage(NULL)
,mElementListPageSize(0),mElementListStart(0),mElementListPagePosInFile(0)
,mElementOffsetPageSize(0),mElementOffsetStart(0),mElementOffsetPagePosInFile(0),mParentNodeKey("")
{}

DSSDataModelControlImpl::~DSSDataModelControlImpl()
{
	hCleanUp();
}

void DSSDataModelControlImpl::hCleanUp()
{
	if (mpMetricValueMin)
	{
		delete mpMetricValueMin;
		mpMetricValueMin = NULL;
	}
	
	if (mpMetricValueMax)
	{
		delete mpMetricValueMax;
		mpMetricValueMax = NULL;
	}
	
	if (mpMetricValueCount) {
		delete mpMetricValueCount;
		mpMetricValueCount = NULL;
	}
	
	mvDisplayText.clear();
	mvElementID.clear();
    mvIndex.clear();
    hCleanElementList();
}

int DSSDataModelControlImpl::Init()
{
	mType = DssModelControl;
	return 0;
}
int DSSDataModelControlImpl::Init(int nElements)
{
	if (mnElements > 0 && nElements == 0)
		return 0;
	mType = DssModelControl;
	mvDisplayText.resize(nElements);
	mvElementID.resize(nElements);
    mvIndex.resize(nElements);
	mnElements = nElements;
	return 0;
}

int DSSDataModelControlImpl::AddElement(int iDisplayIndex, MBase::CString &irText, MBase::CString &irElementID, int iIndex)
{
	if (iDisplayIndex < 0 || iDisplayIndex >= mnElements)
		return E_INVALIDINDEX;
	
	mvDisplayText[iDisplayIndex] = irText;
	mvElementID[iDisplayIndex] = irElementID;
    mvIndex[iDisplayIndex] = iIndex < 0 ? iDisplayIndex : iIndex;
	return 0;
}

int DSSDataModelControlImpl::hCleanElementList()
{
    int hr = S_OK;
    if(mbMapped)
    {
        //TODO: unmap 
        hr = DataSourceUtil::releaseMappedFileBuffer(mpElementListPage, mElementListPageSize);
        CHECK_HR;
        
        hr = DataSourceUtil::releaseMappedFileBuffer(mpElementOffsetPage, mElementOffsetPageSize);
        CHECK_HR;
        
        mpElementList = NULL;
        mpElementListPage = NULL;
        mpElementOffset = NULL;
        mpElementOffsetPage = NULL;
    }
    else
    {
        if(mpElementList)
            delete mpElementList;
        mpElementList = NULL;
        if(mpElementOffset)
            delete mpElementOffset;
        mpElementOffset = NULL;
    }
    return hr;
}

int DSSDataModelControlImpl::CreateViewMap(vector<int> &irvData)
{
	// the last unit stores control element lookup, for ControlNodeModelUnitsV2
	int lnGBUnits = mvUnits.size() - 1;
	int hr = S_OK;
	if(lnGBUnits > 0)
	{
		int lnRows = irvData.size() / (lnGBUnits + 1);
		if (lnRows * mvUnits.size() != irvData.size())
		{
			//484762, check if the missing unit is subtotal
			//because of increment loading, the current slice only contains a subtotal element for an unit,
			//mvUnits is missing that unit due a server assumption. It's too late to make server side change
			//so we make some changes here
			vector<int> lvTemp;
			lvTemp.reserve(irvData.size());
			for (int i = 0; i < irvData.size(); i++)
			{
				//remove all subtotal from group-by path
				if (irvData[i] >= 0)
				{
					lvTemp.push_back(irvData[i]);
				}
			}
			
			if ( (lvTemp.size() / (lnGBUnits + 1)) * mvUnits.size() != lvTemp.size())
			{
				//removed too many subtotal
				return E_FAIL;
			}
			
			irvData.assign(lvTemp.begin(), lvTemp.end());
            
			//497550
            lnRows = irvData.size() / (lnGBUnits + 1);
            
			//497550
            lnRows = irvData.size() / (lnGBUnits + 1);
		}
		
        vector<int> lvSortedList;
		vector<int> lvDistinctList;
		hSelectDistinct(irvData, lnGBUnits, lnGBUnits + 1, lvSortedList, lvDistinctList);

		int lnDistinctRows = lvDistinctList.size() > 0 ? lvDistinctList.size() : lvSortedList.size();
        int* lpDistinctList = NULL;
        if(irvData.size() > 0)
            lpDistinctList = lvDistinctList.size() > 0 ? &lvDistinctList[0] : &lvSortedList[0];
        
        int* lpKeys = new int [lnDistinctRows * lnGBUnits];
        int* lpKeys2 = lpKeys;
		int i = 0;
  		for (i = 0; i < lnDistinctRows; i++)
		{
            if(lpDistinctList)
                memmove(lpKeys2, &irvData[lpDistinctList[i] * (lnGBUnits + 1)], sizeof(int) * lnGBUnits);
			lpKeys2 += lnGBUnits;
		}
        
        vector<int> lvGBUnits(lnGBUnits);
		for (i = 0; i < lnGBUnits; i++)
			lvGBUnits[i] = i;
        
 
        char* pGBPage = NULL;
        int* pGBPaths = NULL;
        int GBPageSize = 0;
        int GBKeyStart = 0;
        int GBPagePosInFile = 0;
#ifndef __ANDROID__    

        bool ibUseNewFile = true;;
        int lDataSize = lnDistinctRows * lnGBUnits * sizeof(int);
        if (lDataSize >= MEM_GOVERNOR_SIZE)
        {
            //TODO: CHECK SECURITY MODE
            if(DataSourceUtil::isMappedFileSupported())
            {
                mFilePathMMP = mDataSourcePath + mNodeKey.c_str() + "mmp";
                const void *lpData = lpKeys;
                hr = DataSourceUtil::createMappedFileBuffer(1, &lpData, &lDataSize, mFilePathMMP.c_str(), &ibUseNewFile, (void**)&pGBPage, &GBPageSize, (void**)&pGBPaths, &GBKeyStart, &GBPagePosInFile);
                CHECK_HR;
            
                ibUseNewFile = false;
                
                delete [] lpKeys;
                lpKeys = pGBPaths;
            }
        }

#endif		
        
		hr = hCreateViewMap(lnGBUnits, &lvGBUnits[0], lnDistinctRows, lpKeys, mFilePathMMP, pGBPage, GBPageSize, GBPagePosInFile);
		CHECK_HR;

        mnSlice = lnDistinctRows;
        mnElementListCount = lnRows;

        hr = hCleanElementList();
        CHECK_HR;
        
        int* lpElementOffset = new int[lnDistinctRows + 1];
        int* lpElementList = new int[lnRows];
        
        int lnElementOffset = 0;
        int lnElementCount = 0;
        lpElementOffset[lnElementOffset++] = 0;
        
		vector<int> lvRowList;
		i = 0;
		int j = 1;
		while(i < lnRows && j <= lnDistinctRows)
		{
            int next = -1;
            if(lpDistinctList)
                next = j < lnDistinctRows ? lpDistinctList[j] : -1;		//set to -1 to ignore next
			int cur = lvSortedList[i];			//current row offset
			while (cur != next)
			{
				lvRowList.push_back(cur);
				i++;
				if (i >= lnRows)
					break;
				cur = lvSortedList[i];
			}
			
			std::sort(lvRowList.begin(), lvRowList.end());		//keep the elements order (by its original row offset)

			//save the element id list          
            vector<int>::iterator it = lvRowList.begin();
			for (; it != lvRowList.end(); it++)
			{
				//get the control element id
                lpElementList[lnElementCount++] = irvData[*it * (lnGBUnits + 1) + lnGBUnits];
			}
            lpElementOffset[lnElementOffset++] = lnElementCount;
            
			lvRowList.clear();
			j++;
		}
        
        mbMapped = false;
#ifndef __ANDROID__    
        lDataSize = lnRows * sizeof(int);
        if (lDataSize >= MEM_GOVERNOR_SIZE)
        {
            //TODO: CHECK SECURITY MODE
            if(DataSourceUtil::isMappedFileSupported())
            {
                mbMapped = true;
                mFilePathMMP = mDataSourcePath + mNodeKey.c_str() + "mmp";
                const void *lpData = lpElementList;
                hr = DataSourceUtil::createMappedFileBuffer(1, &lpData, &lDataSize, mFilePathMMP.c_str(), &ibUseNewFile, (void**)&mpElementListPage, &mElementListPageSize, (void**)&mpElementList, &mElementListStart, &mElementListPagePosInFile);
                CHECK_HR;
                
                delete [] lpElementList;
                
                ibUseNewFile = false;
                
                lpData = lpElementOffset;
                hr = DataSourceUtil::createMappedFileBuffer(1, &lpData, &lDataSize, mFilePathMMP.c_str(), &ibUseNewFile, (void**)&mpElementOffsetPage, &mElementOffsetPageSize, (void**)&mpElementOffset, &mElementOffsetStart, &mElementOffsetPagePosInFile);
                CHECK_HR;
                    
                delete [] lpElementOffset;
            }
        }
#endif	
        if(!mbMapped)
        {
            mpElementList = lpElementList;
            mpElementOffset = lpElementOffset;
        }
	}
	else
								//TODO[test]: TQMS 315185: the elements may be repetitive
	{
        hr = hCleanElementList();
        CHECK_HR;
        
        mbMapped = false;
		hr = hCreateViewMap(0, NULL, 1, NULL);
		CHECK_HR;
        mnSlice = 1;
        mnElementListCount = irvData.size();

        mpElementOffset = new int[mnSlice + 1];
        mpElementOffset[0] = 0;
        mpElementOffset[1] = mnElementListCount;

        mpElementList = new int[mnElementListCount];
        memmove(mpElementList, &irvData[0], mnElementListCount * sizeof(int));
	}
	
	return 0;
}

void DSSDataModelControlImpl::setCGBFlag(int iFlag)
{
	mGhostFlag = iFlag;
}

int DSSDataModelControlImpl::CountElements()
{
	return mvDisplayText.size();
}

int DSSDataModelControlImpl::getElementText(int index, MBase::CString &orText)
{
	if (index < 0 || index >= mnElements)
		return E_INVALIDINDEX;
	orText = mvDisplayText[index];
	return 0;
}

bool DSSDataModelControlImpl::HasElementID(int index)
{
	if (index >= 0 && index < mnElements)
		return !(mvElementID[index].empty());
	else
		return false;
}

int DSSDataModelControlImpl::getElementID(int index, MBase::CString &orElementID)
{
	if (index >= 0 && index < mnElements && index < mvIndex.size())
	{
		orElementID = mvElementID[index];
		return S_OK;
	}
	else
		return E_INVALIDINDEX;
}

int DSSDataModelControlImpl::Select(int nPaths, int nGBUnits, vector<GUID> &irGBUnitID, int *ipGBPaths, vector<int> &orvElements)
{
	int hr = EnsureModelData();
	CHECK_HR;
	
    hr = mpViewMap->MapGBKeys();
    CHECK_HR;
    
	vector<int> lvSliceID;
    int lnLists = mnSlice;
	hr = hSelectWindows(nPaths, nGBUnits, irGBUnitID, ipGBPaths, lvSliceID);
    CHECK_HR;
    
    hr = mpViewMap->UnMapGBKeys();
    CHECK_HR;
						
	int lnSlices = lvSliceID.size();		//selected slices
	
	set<int> lsList;
	for (int i = 0; i < lnSlices; i++)
	{
		int offset = lvSliceID[i];
		if (offset < lnLists)
            for(int j = mpElementOffset[offset]; j < mpElementOffset[offset + 1]; j++)
                lsList.insert(mpElementList[j]);
	}
	int count = lsList.size();
	orvElements.reserve(count);
	set<int>::iterator it = lsList.begin();
	for (; it != lsList.end(); it++)
	{
        if (*it >= 0 && *it < mnElements)		//431827, exclude invalid elements
        {
            orvElements.push_back(*it);
        }
	}
	std::sort(orvElements.begin(), orvElements.end());

	return S_OK;
}

int DSSDataModelControlImpl::SelectSliceWindows(int nPaths, int nGBUnits, vector<GUID> &irGBUnitID, int *ipGBPaths, vector<int> &lvSlices)
{
	int hr = EnsureModelData();
	CHECK_HR;
	
    hr = mpViewMap->MapGBKeys();
    CHECK_HR;
    
	hr = hSelectWindows(nPaths, nGBUnits, irGBUnitID, ipGBPaths, lvSlices);
	CHECK_HR;
    
    hr = mpViewMap->UnMapGBKeys();
    CHECK_HR;
    
	return S_OK;
}

int DSSDataModelControlImpl::GetElementsBySliceWindow(vector<int>& irvSlices, vector<int> &orvElements)
{
	int hr = EnsureModelData();
	CHECK_HR;
	
	vector<int> lvSliceID;
    int lnLists = mnElementListCount;
	int lnSlices = irvSlices.size();		//selected slices
	
	set<int> lsList;
	for (int i = 0; i < lnSlices; i++)
	{
		int offset = irvSlices[i];
        if(offset < 0 || offset >= mnElementListCount)
            continue;
		if (offset < lnLists)
            for(int j = mpElementOffset[offset]; j < mpElementOffset[offset + 1]; j++)
                lsList.insert(mpElementList[j]);
	}
	int count = lsList.size();
	orvElements.reserve(count);
	set<int>::iterator it = lsList.begin();
	for (; it != lsList.end(); it++)
	{
        for(int i = 0; i < mvIndex.size(); i++)
        {
            //if (*it >= 0 && *it < mnElements)		//431827, exclude invalid elements
            if(*it >= 0 && *it == mvIndex[i])
            {
                orvElements.push_back(i);
                break;
            }
        }
	}
	std::sort(orvElements.begin(), orvElements.end());
    
	return S_OK;
}

DSSBaseElementsProxy * DSSDataModelControlImpl::createElementAt(int index)
{
/*	if (index < 0 || index > mnElements)
		return E_INVALIDINDEX;
	
	if (!mpControlModel->HasElementID(ei))
	{
		DSSBaseElementProxy *lpElement = NULL;
		hr = lpUnit->createElement(ei, mpObjectContext, &lpElement);
		CHECK_PTR(lpElement);
		
		setElementAt(ei, lpElement);
	}
	else		//handle subtoal and null element
	{
		//use the element id to contruct element
		MBase::CString lElementID;
		hr = mpControlModel->getElementID(ei, lElementID);
		CHECK_HR;
		
		setElementAt(ei, mpControlModel->CreateElementFromElementID(lElementID));
	}*/
	return NULL;
}
	
int DSSDataModelControlImpl::size()
{
	int lSize = sizeof(this);
	
	lSize += hCalculateSizeOfBase();
	
	lSize += DSSAEHelper::SizeOfStrings(mvDisplayText);

	lSize += DSSAEHelper::SizeOfStrings(mvElementID);

    lSize += sizeof(int) * (mnSlice + mnElementListCount);
    
	return lSize;
}

int DSSDataModelControlImpl::setMetricRange(int iType, CComVariant *ipVal)
{
	if (!ipVal)
		return E_INVALIDARG;
	
	switch (iType) {
		case 0:
		{
			if (!mpMetricValueMin)
				mpMetricValueMin = new CComVariant(*ipVal);
			else
				*mpMetricValueMin = *ipVal;
		}
			break;
		case 1:
		{
			if (!mpMetricValueMax)
				mpMetricValueMax = new CComVariant(*ipVal);
			else
				*mpMetricValueMax = *ipVal;
		}
			break;
		case 2:
		{
			AE_ASSERT(ipVal->vt == VT_I4);
			if (!mpMetricValueCount)
				mpMetricValueCount = new CComVariant(*ipVal);
			else
				*mpMetricValueCount = *ipVal;
		}
			break;
		default:
			return E_FAIL;
	}
	return S_OK;
}

const CComVariant * DSSDataModelControlImpl::getMetricRange(int iType)
{
	switch (iType) {
		case 0:
			return mpMetricValueMin;
		case 1:
			return mpMetricValueMax;
		case 2:
			return mpMetricValueCount;
		default:
			return NULL;
	}
}

void DSSDataModelControlImpl::ReleaseUnusedMemory()
{
	if (!mpTransactionModel)
	{
		hCleanUp();		//clean up itself
		DSSDataModelBase::ReleaseUnusedMemory();
	}
	return;
}

int DSSDataModelControlImpl::SetElementIndex(int i, int iIndex)
{
    if(i < 0 || i >= mvIndex.size())
        return E_INVALIDINDEX;
    mvIndex[i] = iIndex;
    return S_OK;
}

int DSSDataModelControlImpl::AddElementMatchText(int iDisplayIndex, MBase::CString iText)
{
    if (iDisplayIndex < 0 || iDisplayIndex >= mnElements)
		return E_INVALIDINDEX;
	
    if(mvElementMatchText.size() != mnElements)
        mvElementMatchText.resize(mnElements);
    mvElementMatchText[iDisplayIndex] = iText;
	return S_OK;
}

int DSSDataModelControlImpl::getElementMatchText(int iDisplayIndex, MBase::CString &orText)
{
    if (iDisplayIndex < 0 || iDisplayIndex >= mnElements)
		return E_INVALIDINDEX;
    if(mvElementMatchText.size() <= iDisplayIndex)
        orText = "";
    else
        orText = mvElementMatchText[iDisplayIndex];
    return S_OK;
}

int DSSDataModelControlImpl::setParentNodeKey(std::string iKey)
{
    mParentNodeKey = iKey;
    return S_OK;
}

int DSSDataModelControlImpl::getParentNodeKey(std::string& orKey)
{
    orKey = mParentNodeKey;
    return S_OK;
}

int DSSDataModelControlImpl::getElementIndex(int iIndex)
{
    if(iIndex < 0 || iIndex > mvIndex.size())
        return iIndex;
    else
        return mvIndex[iIndex];
}