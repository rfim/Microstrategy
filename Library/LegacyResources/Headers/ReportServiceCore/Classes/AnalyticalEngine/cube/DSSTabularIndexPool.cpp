/*
 *  DSSTabularIndexPool.cpp
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import "DSSTabularIndexPool.h"
#import "DSSTabularIndex.h"
#import "DSSStrongPtr.h"
#import "DSSTabularAttribute.h"
#import "DSSObjectContext.h"
#import "DSSObjectInfo.h"

#include "DSSViewDataSet.h"
#include <algorithm>

#include "DSSCMBlockFileUtil.h"

DSSTabularIndexPool::DSSTabularIndexPool():mpMainIndex(NULL), mpvIndexList(NULL),mfCubeReport(false), mpDisplayMainIndex(NULL)
,mpGrandTotalIndex(NULL)
{
    mnBinaryKeys = 0;
    mnUnits = 0;
    mpTreeRoot = NULL;
    
    // 8/23/2012 tehe
    mMMPStatus = false;
}

DSSTabularIndexPool::~DSSTabularIndexPool()
{
    // 8/30/2012 tehe: we should be careful release memory map here
    releaseMMapMemory();
    
    if(mpTreeRoot)
    {
        deleteNode(mpTreeRoot);
        mpTreeRoot = NULL;
    }
    // tehe 4/18/2012: memory leak, I think use mpvIndexList.size may be more safe
    // for(int i = 0; i < mnBinaryKeys; ++i)
    if(mpvIndexList)
    {
        for(int i=0;i<(*mpvIndexList).size();i++)
        {
            list<DSSTabularIndex*>& lList = (*mpvIndexList)[i];
            while (!lList.empty())
            {
                DSSTabularIndex* lIndex = lList.front();
                if (lIndex)
                {
                    delete lIndex;
                    lIndex = NULL;
                }
                lList.pop_front();
            }
            /*
            for(list<DSSTabularIndex*>::iterator iter = lList.begin(); iter != lList.end(); ++iter)
            {
                if(*iter)
                {
                    delete *iter;
                    *iter = NULL;
                }
            }*/
        }
        mpvIndexList->clear();
        delete mpvIndexList;
        mpvIndexList = NULL;
    }
    
    if (mpGrandTotalIndex)
    {
        delete mpGrandTotalIndex;
        mpGrandTotalIndex = NULL;
    }
    
    for(int i = 0; i< mvIndices.size();i++)
    {
        if(mvIndices[i])
            delete mvIndices[i];
        mvIndices[i] = NULL;
    }
    mvIndices.clear();
}

int DSSTabularIndexPool::Init(DSSDataModelXTabImpl *pTabularData, bool ifCubeReport)
{
    int hr = S_OK;
    
    mpDataModelXTab = pTabularData;
    mnUnits = pTabularData->Count();
    mnBinaryKeys = mnUnits;
    mpvIndexList = new vector<XTabIndexList>(mnBinaryKeys+1);
    CHECK_NEWED_PTR(mpvIndexList);
    
    mpTreeRoot = new Node();
    CHECK_NEWED_PTR(mpTreeRoot);
    
    mfCubeReport = ifCubeReport;
    
    return hr;
}

int DSSTabularIndexPool::Init(int iUnits, bool ifCubeReport)
{
    int hr = S_OK;
    
    mpDataModelXTab = NULL;
    mnUnits = iUnits;
    // mnBinaryKeys = mnUnits;
    mpvIndexList = new vector<XTabIndexList>(mnBinaryKeys+1);
    CHECK_NEWED_PTR(mpvIndexList);
    
    mpTreeRoot = new Node();
    CHECK_NEWED_PTR(mpTreeRoot);
    
    mfCubeReport = ifCubeReport;
    
    return hr;
}

int DSSTabularIndexPool::CreateIndex(int nUnits, int nRows, int *pUnits, int *ipKey, DSSTabularIndex **oppIndex,bool ibLoadbySlice)
{
	DSSTabularIndex *lpIndex = NULL;
    //	bool lFound = Search(nUnits, pUnits, &lpIndex);
    bool lFound = false;
	if (lFound)
    {
        *oppIndex = lpIndex;
		return S_OK;  //zhyang, 04/08/2011, I think we should return S_OK here. If we find the index, there is no need to create another one
    }
    //zhyang, 04/08/2011, We just use "new" to create the index because mpMainIndex may not be ready here
	lpIndex = new DSSTabularIndex();
	MBase::DSSStrongPtr<DSSTabularIndex> lpHolder(lpIndex);
	int hr = lpIndex->Init(nUnits, nRows, pUnits, ipKey, mpDataModelXTab,ibLoadbySlice);
	CHECK_HR;
	
    //maybe we can use the InsertToPool function instead of this one
	//hr = hInsertIndex(lpIndex);
    
    //temporily we don't insert it, just create it and return.
    //    hr = InsertToPool(lpIndex);
    mvIndices.push_back(lpIndex);
	CHECK_HR;
	
	if (oppIndex)
		*oppIndex = lpIndex;
	
    //    lpIndex->toString();
    
	lpHolder.GiveUp();
	
	return S_OK;
}

int DSSTabularIndexPool::getIndex(int nUnits, int *pUnits, DSSTabularIndex **oppIndex)
{
	bool lFound = Search(nUnits, pUnits, oppIndex);
	if (lFound)
		return S_OK;
	
	return hUpdate(nUnits, pUnits, oppIndex);
}

bool DSSTabularIndexPool::Search(int nUnits, int *pUnits, DSSTabularIndex **oppIndex)
{
    //zhyang, we need to run the document so just change it to return false;
    //return false;
    
    // tehe: check tree root, maybe there's no index in index pool
    if(mpTreeRoot == NULL)
        return false;
    
    int hr = S_OK;
	bool* lpBinaryKeys = new bool[mnBinaryKeys];
    CHECK_NEWED_PTR(lpBinaryKeys);
    MBase::DSSStrongPtr<bool, MBase::DeleteArray<bool> > lBinaryKeysPtr(lpBinaryKeys);
    
    hr = hTranslateToBinaryKeys(nUnits, pUnits, lpBinaryKeys);
    CHECK_HR;
    
    Node* lpNode = mpTreeRoot;
    for(int i = 0; i < mnBinaryKeys; ++i)
    {
        if(lpBinaryKeys[i])
        {
            lpNode = lpNode->right;
        }
        else
        {
            lpNode = lpNode->left;
        }
        
        if(NULL == lpNode)
        {
            return false; // not found
        }
    }
    
    *oppIndex = lpNode->pIndex;
    
    if(NULL != *oppIndex)
    {
        (*oppIndex)->UpdateTimeStamp();
        return true;
    }
    else
    {
        return false;
    }
    
	return false;
}

int DSSTabularIndexPool::hUpdate(int nUnits, int *pUnits, DSSTabularIndex **oppIndex)
{
	//TODO:
    int hr = S_OK;
    
    // To prevent another thread insert the same index, search again
    bool lFound = Search(nUnits, pUnits, oppIndex);
    if (lFound) {
        return hr = S_OK;
    }
    
    hr = GenerateNewIndex(nUnits, pUnits, oppIndex);
    CHECK_HR;
    
    hr = InsertToPool(*oppIndex);
    CHECK_HR;
    
    (*oppIndex) -> UpdateTimeStamp();
    
	return hr = S_OK;
}

int DSSTabularIndexPool::hInsertIndex(DSSTabularIndex *ipIndex)
{
	if (ipIndex)
    {
		//mvIndexList.push_back(ipIndex);
        InsertToPool(ipIndex);
    }
	return S_OK;
}

int DSSTabularIndexPool::size()
{
    /*	int lSize = sizeof(this);
     
     int lnIndices = mvIndexList.size();
     for (int i = 0; i < lnIndices; i++)
     {
     if (mvIndexList[i])
     {
     lSize += mvIndexList[i]->size();
     }
     }
     return lSize;
     */
    return 0;
}

int DSSTabularIndexPool::hTranslateToBinaryKeys(int nUnit, int *pUnits, bool *opBinaryKeys)
{
    int hr = S_OK;
    
    memset(opBinaryKeys, 0, sizeof(bool) * mnBinaryKeys);
    assert(nUnit <= mnBinaryKeys);
    
    for(int i = 0; i < nUnit; ++i)
    {
        assert(pUnits[i] < mnBinaryKeys);
        opBinaryKeys[pUnits[i]] = true;
    }
    
    return hr;
}

int DSSTabularIndexPool::GenerateNewIndex(int nUnits, int *pUnits, DSSTabularIndex **oppIndex)
{
    int hr = S_OK;
    
    //a little different from the server side
    //There is no base index pass in, so we need to find the minimun base index each time
    DSSTabularIndex* lpBaseIndex = NULL;
    hr = getMininumBaseIndex(nUnits, pUnits, &lpBaseIndex);
    
    if(S_OK == hr)
    {
 //       assert(NULL != lpBaseIndex);
        
        DSSTabularIndex* lpNewIndex = new DSSTabularIndex();
        
        //Deal with index space, Currently we don't support it yet.
        
        int *lpRowmap = NULL;
        vector<int> lvUnits;
        for(int i = 0; i < nUnits; ++i)
        {
            lvUnits.push_back(pUnits[i]);
        }
        
        if(lpBaseIndex)
        {
            hr = lpBaseIndex->GenDistinctC(lvUnits, lpNewIndex, lpRowmap);
            //yimliu, 2014/06/24, 913379, the memory governor is hit, but no pop-up message.
            CHECK_HR;
        }else
        {
            hr = lpNewIndex->Init(0,1,NULL,NULL);
            CHECK_HR;
        }
        
        *oppIndex = lpNewIndex;
        
        //Deal with the RowMap here. Currently we don't support it yet
    }
    else //if(S_OK == hr) There is no base index
    {
        //There will be consolidations, Currently we don't support it yet
    }
    
    
    return hr = S_OK;
}
int DSSTabularIndexPool::InsertToPool(DSSTabularIndex* pIndices)
{
    int hr = S_OK;
    
    hr = InsertToTree(pIndices);
    if(S_OK == hr)
    {
        hr = InsertToIndexList(pIndices);
        CHECK_HR;
    }
    return hr = S_OK;
}

int DSSTabularIndexPool::getMininumBaseIndex(int nUnits, int *pUnits, DSSTabularIndex **oppIndex)
{
    int hr = S_OK;
    
    for(int lUnitCount = nUnits + 1; lUnitCount < mnBinaryKeys + 1; ++lUnitCount)
    {
        XTabIndexList &lIndexList = (*mpvIndexList)[lUnitCount];
        for (XTabIndexList::iterator lCurItr = lIndexList.begin(); lCurItr != lIndexList.end(); lCurItr++)
        {
            DSSTabularIndex* lpCurIndex = *lCurItr;
            int lnUnit = lpCurIndex->getNumUnits();
            int* lpUnit = new int[lnUnit];
            CHECK_NEWED_PTR(lpUnit);
            MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lUnitPtr(lpUnit);
            
            hr = lpCurIndex->getUnits(lnUnit, lpUnit);
            CHECK_HR;
            
            if(std::includes(lpUnit, lpUnit + lnUnit, pUnits, pUnits + nUnits))
            {
                *oppIndex = lpCurIndex;
                return S_OK;
            }
        }
    }
    
    return hr = S_OK;
}

int DSSTabularIndexPool::InsertToTree(DSSTabularIndex *pIndices)
{
    int hr = S_OK;
    
    bool *lpBinaryKeys = new bool[mnBinaryKeys];
    CHECK_NEWED_PTR(lpBinaryKeys);
    MBase::DSSStrongPtr<bool, MBase::DeleteArray<bool> > lBinaryKeyPtr(lpBinaryKeys);
    
    int lnUnit = pIndices->getNumUnits();
    int *lpUnits = new int[lnUnit];
    CHECK_NEWED_PTR(lpUnits);
    MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lUnitsPtr(lpUnits);
    
    hr = pIndices->getUnits(lnUnit, lpUnits);
    CHECK_HR;
    
    hr = hTranslateToBinaryKeys(lnUnit, lpUnits, lpBinaryKeys);
    CHECK_HR;
    
    Node *lpNode = mpTreeRoot;
    assert(lpNode);
    
    for(int i = 0; i < mnBinaryKeys; ++i)
    {
        if(lpBinaryKeys[i])
        {
            if(NULL == lpNode->right)
            {
                lpNode->right = new Node();
                CHECK_NEWED_PTR(lpNode->right);
            }
            lpNode = lpNode->right;
        }
        else
        {
            if(NULL == lpNode->left)
            {
                lpNode->left = new Node();
                CHECK_NEWED_PTR(lpNode->left);
            }
            lpNode = lpNode->left;
        }
    }
    
    //Redundant index!! This could happen when an pre-Orion client running a report with cosolidation unit and
    //serialize this report to the server. Skip this index.
    if(NULL != lpNode->pIndex)
    {
        return E_FAIL;
    }
    
    lpNode->pIndex = pIndices;
    
    return hr = S_OK;
}

int DSSTabularIndexPool::InsertToIndexList(DSSTabularIndex *pIndices)
{
    int hr = S_OK;
    
    int lnUnit = pIndices->getNumUnits();
    assert(lnUnit <= mnBinaryKeys);
    
    XTabIndexList &lIndexList = (*mpvIndexList)[lnUnit];
    
    //!!!!Need to deal with Consolidations!!!!
    XTabIndexList::iterator lCurItr;
    for(lCurItr = lIndexList.begin(); lCurItr != lIndexList.end(); lCurItr++)
    {
        DSSTabularIndex* lCurIndex = *lCurItr;
        if(lCurIndex->getNumUnits() > lnUnit)
        {
            break;
        }
    }

    lIndexList.insert(lCurItr, pIndices);
    
    return hr = S_OK;
    
}

int DSSTabularIndexPool::setMainIndex(DSSTabularIndex *ipMainIndex)
{
    int hr = S_OK;
    
    CHECK_PTR(ipMainIndex);
    
    //zhyang, I'm not sure whether it will affect other parts, just use it to get how many units are there
    // mnBinaryKeys = ipMainIndex->getNumUnits();
    
    // tehe 4/17/2012: memory leak, we have already init it in init method
    if(!mpTreeRoot)
    {
        mpTreeRoot = new Node();
        CHECK_NEWED_PTR(mpTreeRoot);
    }
    if(!mpvIndexList)
    {
        mpvIndexList = new vector<XTabIndexList>(mnBinaryKeys+1);
        CHECK_NEWED_PTR(mpvIndexList);
    }
    
    mpMainIndex = ipMainIndex;
    ipMainIndex->isMainIndexOrder = true;
    hr = InsertToPool(mpMainIndex);
    CHECK_HR;
    /*
     int unit[3] = {0,1,2};
     DSSTabularIndex *lpIndex = NULL;
     getIndex(3, unit, &lpIndex);
     getIndex(2, unit, &lpIndex);
     getIndex(1, unit, &lpIndex);
     
     int unit2[2] = {1,2};
     getIndex(2, unit2, &lpIndex);
     getIndex(1, unit2, &lpIndex);
     
     int unit3[2] = {0, 2};
     getIndex(2, unit3, &lpIndex);
     
     int unit4[1] = {2};
     getIndex(1, unit4, &lpIndex);
     */
    return S_OK;
    
}

int DSSTabularIndexPool::deleteNode(Node *node)
{
    //zhyang, 02/08/2011, only non-null value is passed in
    //no need to check
    if(NULL == node)
    {
        return S_OK;
    }
    
    if(node->left)
    {
        deleteNode(node->left);
        node->left = NULL;
    }
    if(node->right)
    {
        deleteNode(node->right);
        node->right = NULL;
    }
    node->pIndex = NULL;
    delete node;
    node = NULL;
    
    return S_OK;
}

int DSSTabularIndexPool::getMainIndex(DSSTabularIndex **ipMainIndex)
{
    if(!ipMainIndex)
    {
        return E_POINTER;
    }
    *ipMainIndex = mpMainIndex;
    return S_OK;
}

int DSSTabularIndexPool::setDisplayMainIndex(DSSTabularIndex *ipDispMainIndex)
{
    int hr = S_OK;
    
    CHECK_PTR(ipDispMainIndex);
    
    //zhyang, I'm not sure whether it will affect other parts, just use it to get how many units are there
    // mnBinaryKeys = ipMainIndex->getNumUnits();
    
    // tehe 4/17/2012: memory leak, we have already init it in init method
    if(!mpTreeRoot)
    {
        mpTreeRoot = new Node();
        CHECK_NEWED_PTR(mpTreeRoot);
    }
    if(!mpvIndexList)
    {
        mpvIndexList = new vector<XTabIndexList>(mnBinaryKeys+1);
        CHECK_NEWED_PTR(mpvIndexList);
    }
    
    mpDisplayMainIndex = ipDispMainIndex;
    mpDisplayMainIndex->isMainIndexOrder = true;
    hr = InsertToPool(mpDisplayMainIndex);
    CHECK_HR;

    return hr;
}

int DSSTabularIndexPool::getDisplayMainIndex(DSSTabularIndex **ipDispMainIndex)
{
    if(!ipDispMainIndex)
    {
        return E_POINTER;
    }
    *ipDispMainIndex = mpDisplayMainIndex;
    return S_OK;
}

void DSSTabularIndexPool::toString()
{
    printf("Enter function DSSTabularIndexPool::toString()\n");
    printf("Index List:\n");
    
    int lCount = 1;
    
    for (int i = 0; i < mnBinaryKeys; ++i) {
        XTabIndexList &lIndexList = (*mpvIndexList)[i];
        for (XTabIndexList::iterator lCurItr = lIndexList.begin(); lCurItr != lIndexList.end(); lCurItr++)
        {
            DSSTabularIndex* lpCurIndex = *lCurItr;
            printf("Index %d:\n", lCount++);
            //printf("Address : %x\n", (unsigned int)lpCurIndex);
            printf("Address : %lx\n", (unsigned long)lpCurIndex); // yanzli: support arm64
            int lnUnit = lpCurIndex->getNumUnits();
            printf("There are total %d units int the index:\n", lnUnit);
            int* lpUnitNum = new int[lnUnit];
            //CHECK_NEWED_PTR(lpUnitNum);
            MBase::DSSStrongPtr<int, MBase::DeleteArray<int> > lUnitNumPtr(lpUnitNum);
            
            lpCurIndex->getUnits(lnUnit, lpUnitNum);
            for(int j = 0; j < lnUnit; ++j)
            {
                DSSTabularUnit *lpUnit = mpDataModelXTab->Item(lpUnitNum[j]);
                DSSTabularAttribute *lpAttr = (DSSTabularAttribute*)lpUnit;
                GUID lID;
                lpAttr->getGUID(lID);
                DSSObjectInfo *lpObj = mpDataModelXTab->getObjectContext()->FindObjectH(lID);
                string lName = lpObj->getName();
                printf("Unit %d is : %s\n", j, lName.c_str());
            }
            printf("\n");
        }
    }
}

void DSSTabularIndexPool::GetIndexRowMap(DSSTabularIndex* ipBase, DSSTabularIndex* ipTarget, int** oppRowMap)
{
    *oppRowMap = NULL;
    
	if (ipBase == NULL || ipTarget == NULL)
		return;
    
	int lnUnit = 0;
	lnUnit = ipTarget->getNumUnits();
    
	// weiwang 2009-09-14 TQMS374084 return XTAB_CONSTANT_ROWMAP only when index row number is 1
	int lnRow = 0;
	lnRow = ipTarget->Count();
    
	if (lnUnit == 0 && lnRow == 1)
	{
		*oppRowMap = XTAB_CONSTANT_ROWMAP;
		return;
	}
	else if (ipBase->IsSameOrder(ipTarget))
	{
		*oppRowMap = XTAB_SAME_ROWMAP;
		return;
	}
    
	// tehe: do we need consider cube report at client side? I don't think so.
//	if (mfCubeReport)
//	{
//		// ...
//	}
    
    // tehe: maybe we can do some optimize here
    // bool lFlag = false;
    for (int i = 0; i < mvRowMaps.size(); i++)
    {
        if (mvRowMaps[i].mpBaseIndex == ipBase && mvRowMaps[i].mpTargetIndex == ipTarget)
        {
            *oppRowMap = mvRowMaps[i].mpRowMap;
            // lFlag = true;
            break;
        }
    }
//	if(! lFlag)
//    {
//        AddIndexRowMap(ipBase, ipTarget, oppRowMap);
//    }
}

// tehe 1/4/2012 this method may be not used any more
int DSSTabularIndexPool::AddIndexRowMap(DSSTabularIndex* ipBase, DSSTabularIndex* ipTarget, int** oppRowMap)
{
    if (ipTarget == NULL || ipBase == NULL)
		return S_OK;
    
	// If the target has <=1 unit, no need to use rowmap to speedup lookup
	if (ipTarget->mnUnits <= 1 || ipBase == ipTarget)
		return S_OK;
	    
	// Create a new row map
	RowMapSt lSt;
	lSt.mpBaseIndex = ipBase;
	lSt.mpTargetIndex = ipTarget;
	lSt.mpRowMap = NULL;  // Don't set the rowmap first. Do it only after it's calculated.
    
	mvRowMaps.push_back(lSt);
    
	// We have not found a row map. Create one.
	int *lpRowMap = NULL;
	lpRowMap = new int [sizeof(int) * ipBase->mnRows];
	CHECK_NEWED_PTR(lpRowMap);
    
	//validate indices
	HRESULT hr = ipBase->ValidateItemEx(mnUnits);
	CHECK_HR;
    
	hr = ipTarget->ValidateLookUpEx(mnUnits);
	CHECK_HR;
    
	vector<int> lvInputKey(mnUnits);
	int *lpInputKeys = &lvInputKey[0];
    
	vector<int> lvTempKey(mnUnits);
	int *lpTempKeys = &lvTempKey[0];
    
	for(int i = 0; i < ipBase->mnRows; i++)
	{
		//Find the corresponding group
		hr = ipBase->ItemEx(i, mnUnits, lpInputKeys);
		CHECK_HR;
        
		int lOffset = 0;
        
		hr = ipTarget->LookUpEx(mnUnits, lpInputKeys, lpTempKeys, &lOffset);
		CHECK_HR;
        
		lpRowMap[i] = lOffset;
	}
    
	// Now the rowmap is calculated correctly. Set it in the vector now.
	mvRowMaps.back().mpRowMap = lpRowMap;
    
    if(oppRowMap)
    {
        *oppRowMap = lpRowMap;

    }
    
    return S_OK;
}

void DSSTabularIndexPool::setBinaryKeys(int iBinaryKeys)
{
    mnBinaryKeys = iBinaryKeys;
}

DSSTabularIndex* DSSTabularIndexPool::getSpecialIndex()
{
    //to keep backward compatibility for old RWD engine, we need create a grand total level index for transaction
    //and this index is not added to the tree (mpTreeRoot)
    if (!mpGrandTotalIndex)
    {
        mpGrandTotalIndex = new DSSTabularIndex();
        mpGrandTotalIndex->Init(0, 1, NULL, NULL, NULL);
    }
    return mpGrandTotalIndex;
}

int DSSTabularIndexPool::createMMapMemory(const char *ipFilePath, bool ibUseNewFile)
{
    if(!mMMPStatus)
    {
        int hr = S_OK;
        
        // tehe:only create mmap when index is large enough
        if(mpMainIndex)
        {
            long size = mpMainIndex->Count() * mpMainIndex->getNumUnits() * sizeof(int);
            if(size < MEM_GOVERNOR_SIZE)
                return S_OK;
        }
        
        // 8/30/2012 tehe: create file failure, ignore it, and don't do memory map
        if(ibUseNewFile)
        {
            hr = DSSCMBlockFileUtil::CreateFile(ipFilePath);
            if(hr != S_OK)
                return S_OK;
        }
        
        if(mpvIndexList)
        {
            for(int i=0;i<(*mpvIndexList).size();i++)
            {
                list<DSSTabularIndex*>& lList = (*mpvIndexList)[i];
                for(list<DSSTabularIndex*>::iterator iter = lList.begin(); iter != lList.end(); ++iter)
                {
                    if(*iter)
                    {
                        // 8/30/2012 tehe: we needn't create new file for each index
                        ibUseNewFile = false;
                        hr = (*iter)->createMMapMemory(ipFilePath, ibUseNewFile);
                        CHECK_HR;
                    }
                }
            }
        }
        mMMPStatus = true;
    }
    
    return S_OK;
}

int DSSTabularIndexPool::releaseMMapMemory()
{
    if(mpvIndexList)
    {
        for(int i=0;i<(*mpvIndexList).size();i++)
        {
            list<DSSTabularIndex*>& lList = (*mpvIndexList)[i];
            for(list<DSSTabularIndex*>::iterator iter = lList.begin(); iter != lList.end(); ++iter)
            {
                if(*iter && (*iter)->GetUseMappedFile())
                {
                    int hr = (*iter)->releaseMMapMemory();
                    CHECK_HR;
                }
            }
        }
    }
    return S_OK;
}

int DSSTabularIndexPool::reloadMMapMemory()
{
    if(mpvIndexList)
    {
        for(int i=0;i<(*mpvIndexList).size();i++)
        {
            list<DSSTabularIndex*>& lList = (*mpvIndexList)[i];
            for(list<DSSTabularIndex*>::iterator iter = lList.begin(); iter != lList.end(); ++iter)
            {
                if(*iter && (*iter)->GetUseMappedFile())
                {
                    int hr = (*iter)->reloadMMapMemory();
                    CHECK_HR;
                }
            }
        }
    }
    return S_OK;
}