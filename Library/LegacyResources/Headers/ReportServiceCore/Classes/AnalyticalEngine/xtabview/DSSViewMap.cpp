/*
 *  DSSViewMap.cpp
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 1/5/10.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import "DSSViewMap.h"
#import "../cube/DSSXTabHashTable.h"
#import "DSSDataSource.h"

DSSViewMap::DSSViewMap():mpGBLookupTable(NULL), mnGBPaths(0), mnGBUnits(0),mpGroupbyKeys(NULL),mnGroupbyKeys(0),mpGBPage(NULL),mGBPageSize(0),mGBPagePosInFile(0),mGBKeyOffset(0),mFilePath(""),mbGBLookupMapped(false)
{
}

DSSViewMap::~DSSViewMap()
{
	if (mpGBLookupTable)
	{
		delete mpGBLookupTable;
		mpGBLookupTable = NULL;
	}
    
    if(mpGBPage)
    {
        DataSourceUtil::releaseMappedFileBuffer(mpGBPage,mGBPageSize);
        mpGBPage = NULL;
        mpGroupbyKeys = NULL;
    }
    else
    {
        delete mpGroupbyKeys;
        mpGroupbyKeys = NULL;
    }
}

int DSSViewMap::Init(vector<GUID> &irGBUnitID,int nRows, int *ipKeys, std::string iFilePath, char* pGBPage, int GBPageSize, int GBPagePosInFile)
{
	int nUnits = irGBUnitID.size();
	vector<int> lvBucketSpace(nUnits);
	vector<int> lvMaxKeys(nUnits);
	
	int lNumBuckets = 0;
	int *lpUnitBucketSpace = NULL;
	int *lpUnitMaxKey = NULL;
	int *lpKeys = NULL;
	
	if (nUnits > 0)
	{
		memset(&lvMaxKeys[0], 0, sizeof(int)*nUnits);
		lpUnitBucketSpace = &lvBucketSpace[0];
		lpUnitMaxKey = &lvMaxKeys[0];
		
        mnGroupbyKeys = nRows * irGBUnitID.size();

        if(pGBPage)
        {
            mpGroupbyKeys = ipKeys;
            lpKeys = mpGroupbyKeys;
            mpGBPage = pGBPage;
            mGBPageSize = GBPageSize;
            mGBPagePosInFile = GBPagePosInFile;
            mGBKeyOffset = (char*)ipKeys - mpGBPage;
            mFilePath = iFilePath;
        }
        else
        {
            mpGroupbyKeys = new int [mnGroupbyKeys];
            memmove(mpGroupbyKeys, ipKeys, sizeof(int)*mnGroupbyKeys);
            lpKeys = mpGroupbyKeys;
            mpGBPage = NULL;
        }
	}
	
	if (mpGBLookupTable)
		delete mpGBLookupTable;
	
	mpGBLookupTable = new DSSXTabHashTable();
	int hr = mpGBLookupTable->CheckKeyValues(nUnits, nRows, lpKeys, lpUnitBucketSpace, lNumBuckets, lpUnitMaxKey);
	CHECK_HR;
	
	hr = mpGBLookupTable->Init(lNumBuckets, nUnits, lpUnitBucketSpace);
	CHECK_HR;
	
	hr = mpGBLookupTable->HashAll(lpKeys, nUnits, nRows);
	CHECK_HR;
	
	mnGBPaths = nRows;
	mnGBUnits = nUnits;
	mvGroupbyUnitID.assign(irGBUnitID.begin(), irGBUnitID.end());
    hr = UnMapGBKeys(false);
    CHECK_HR;
	return S_OK;
}

int DSSViewMap::CountRows()
{
	return mnGBPaths;
}

int DSSViewMap::getView(int *ipKeys, int nUnits, int &orViewID)
{
  
	if (nUnits >= mnGBUnits && mpGBLookupTable->LookUp(ipKeys, &orViewID))
	{
		return S_OK;
	}
	else
    {
        return S_FALSE;
    }
}

int DSSViewMap::getViewPartialMatch(int *ipKeys, int nUnits, vector<int> &orvViews, int &orCount)
{
	int lnRows = orvViews.size();
	orCount = mpGBLookupTable->PartialMatchLookUp(ipKeys, lnRows, &orvViews[0]);
	if (orCount >= 0)
		return S_OK;
	else
		return S_FALSE;

}

int DSSViewMap::getUnitKey(int iRow, int iUnit, int &orKey)
{
	if (iRow < mnGBPaths && iUnit < mnGBUnits)
	{
		orKey = mpGroupbyKeys[iRow * mnGBUnits + iUnit];
		return S_OK;
	}
	return E_FAIL;
}

int DSSViewMap::findGBUnit(GUID &irUnitID)
{
	int lnUnits = mnGBUnits;
	for(int i = 0; i < lnUnits; i ++)
	{
		if(MBase::IsEqualGUID(mvGroupbyUnitID[i], irUnitID))
		{
			return i;
		}
	}
	return -1;
}

int DSSViewMap::size()
{
	int lSize = sizeof(this);
	
	lSize += DSSAEHelper::SizeOfVector(mvGroupbyUnitID);
	
	lSize += mnGroupbyKeys * sizeof(int);
	
	if (mpGBLookupTable)
		lSize += mpGBLookupTable->size();
	
	return lSize;
}

int DSSViewMap::getSubtotalKeys(GUID &irUnitID, set<int> &irExcludedTotalKeys, vector<int> &orTotalKeys)
{
	int lUnit = findGBUnit(irUnitID);
	if (lUnit < 0 || lUnit >= mnGBUnits)
	{
		_ASSERT(false);
		return E_FAIL;
	}
	
	if (mnGBPaths == 0 || mnGBUnits == 0)
		return S_OK;
	
	set<int>::iterator end = irExcludedTotalKeys.end();
	set<int> lsSelectedKeys;
	int *lpKey = mpGroupbyKeys + lUnit;
	
	for (int i = 0; i < mnGBPaths; i++)
	{
		int key = *lpKey;
		if (key < 0)
		{
			if (irExcludedTotalKeys.find(key) == end)
			{
				lsSelectedKeys.insert(key);
				break;		//TODO: right now, there's only one subtotal in the group-by path
			}
		}
		lpKey += mnGBUnits;
	}
	for (set<int>::iterator it = lsSelectedKeys.begin(); it != lsSelectedKeys.end(); it++)
	{
		orTotalKeys.push_back(*it);
	}
	return S_OK;
}

int DSSViewMap::getUnitID(int iUnit, GUID& orUnitID)
{
	int hr = S_OK;
	
	if(iUnit < 0 || iUnit >= mnGBUnits)
		return E_INVALIDINDEX;
	
	orUnitID = mvGroupbyUnitID[iUnit];
	return hr;
}

int DSSViewMap::getGBUnitCount()
{
    return mnGBUnits;
}

int DSSViewMap::createMMapMemory(const char * ipFilePath, bool& ibUseNewFile)
{
    if (mnGBPaths * mnGBUnits * sizeof(int) > MEM_GOVERNOR_SIZE)
    {
        if(ibUseNewFile)
        {
            FILE *lpOutput = NULL;
            lpOutput = fopen(ipFilePath, "w");
            if(lpOutput)
            {
                fclose(lpOutput);
            }
            ibUseNewFile = false;
        }
        mbGBLookupMapped = true;
        return mpGBLookupTable->createMMapMemory(ipFilePath, ibUseNewFile);
    }
    else
    {
        return S_OK;
    }
}

int DSSViewMap::MapGBKeys(bool ibLoadHashTable)
{
    int hr = S_OK;
    if(mFilePath != "")
    {
        hr = DataSourceUtil::reloadMappedFileBuffer(mFilePath.c_str(), mGBPageSize, mGBKeyOffset, mGBPagePosInFile, 
                                                    (void**) &mpGBPage, (void**) &mpGroupbyKeys);
        CHECK_HR;
    }
    
    mpGBLookupTable->setNodeKeys(mpGroupbyKeys);
    
    if(mbGBLookupMapped)
    {
        if(ibLoadHashTable)
            mpGBLookupTable->reloadMMapMemory();
    }
    return S_OK;
}

int DSSViewMap::UnMapGBKeys(bool ibReleaseHashTable)
{
    if(mFilePath != "" && mpGBPage)
    {
        int hr = DataSourceUtil::releaseMappedFileBuffer(mpGBPage, mGBPageSize);
        CHECK_HR;
        mpGBPage = NULL;
        mpGroupbyKeys = NULL;
        
        if(ibReleaseHashTable)
            mpGBLookupTable->releaseMMapMemory();
    }
    return S_OK;
}