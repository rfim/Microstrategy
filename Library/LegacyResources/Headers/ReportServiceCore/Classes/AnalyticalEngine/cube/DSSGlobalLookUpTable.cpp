/*
 *  DSSGlobalLookUpTable.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaoqi Zeng on 9/22/11.
 *  Copyright 2011 MicroStrategy. All rights reserved.
 *
 */

#include "DSSGlobalLookUpTable.h"
#include "DSSDataRefColumn.h"
#include "DSSBinaryDataReader.h"
#include "DSSModelPopulation.h"
#include "DSSDataRefColumn.h"
#include "DSSStrongPtr.h"
#include "DSSXTabRefLookupTable.h"
#include "DSSTabularRefConsolidation.h"
#include "DSSGUnitContainer.h"
#include "DSSXTabBitVector.h"
#include "DSSRWDataModel.h"
#include "DSSSort_includes.h"
#include "DSSViewDataSetHelper.h"
#include "DSSBaseElementProxy.h"
#include <algorithm>

//#define CHECK_HR_BLOCK(type) {CHECK_HR; if (type != lBlockType) return E_DATABLOCK;}

int hLoadUnitConsolidation(DSSBinaryDataReader *mpReadStream, int index, DSSTabularUnit **oppXTabUnit, DSSGlobalLookUpTable* ipGLookUp, bool ibLoadAttr = true);

DSSGlobalLookUpTable::DSSGlobalLookUpTable():
mfHasDE(false),
mGlobalEleCount(0),
mnBaseDE(0),
mnColumns(0),
mnRow(0),
mpDEElement(NULL),
mpExistingRows(NULL),
mpMapDEFromBinaryToStorage(NULL),
mnDEInBinary(0)
{
	
}

DSSGlobalLookUpTable::~DSSGlobalLookUpTable()
{
    for (int i = 0; i < mvpDisplayForms.size(); i++)
        if (mvpDisplayForms[i])
        {
            delete mvpDisplayForms[i];
            mvpDisplayForms[i] = NULL;
        }
    
    mvpDisplayForms.clear();
    mvDisplayFormCount.clear();
    
    for (int i = 0; i < mvpDEs.size(); i++)
        if (mvpDEs[i])
        {
            delete mvpDEs[i];
            mvpDEs[i] = NULL;
        }
    
    mvpDEs.clear();
    
    for (int i = 0; i < mvpDELocalToGlobalMap.size(); i++)
        if (mvpDELocalToGlobalMap[i])
        {
            delete mvpDELocalToGlobalMap[i];
            mvpDELocalToGlobalMap[i] = NULL;
        }
    for (int i = 0; i < mvpDEGlobalToLocalMap.size(); i++)
        if (mvpDEGlobalToLocalMap[i])
        {
            delete mvpDEGlobalToLocalMap[i];
            mvpDEGlobalToLocalMap[i] = NULL;
        }
    
    mvpDELocalToGlobalMap.clear();
    mvpDEGlobalToLocalMap.clear();
    
    if (mpDEElement)
    {
        delete mpDEElement;
        mpDEElement = NULL;
    }
    
    for (int i = 0; i < mvAsc.size(); i++)
        if (mvAsc[i])
            delete [] mvAsc[i];
    
    mvAsc.clear();
    
    for (int i = 0; i < mvSortingResult.size(); i++)
        if (mvSortingResult[i])
            delete [] mvSortingResult[i];
    
    mvSortingResult.clear();
    
    mvSortingForms.clear();
    
    if(mpExistingRows)
        delete mpExistingRows;
    mpExistingRows = NULL;
    
    cleanup();
}

void DSSGlobalLookUpTable::cleanup()
{
    int lnForms = mvColumns.size();
	for (int i = 0; i < lnForms; i++)
	{
		if (mvColumns[i])
		{
			delete mvColumns[i];
			mvColumns[i] = NULL;
		}
	}
    
    mvColumns.clear();
    if (mpMapDEFromBinaryToStorage)
        delete [] mpMapDEFromBinaryToStorage;
    mpMapDEFromBinaryToStorage = NULL;
    mnDEInBinary = 0;
}

int DSSGlobalLookUpTable::GetSortingResult(vector<int>& ivForms, bool* ipAsc, bool ibNullOnTop, int** oppSortingResult, int* opnResult)
{
    if (!ipAsc || !oppSortingResult || !opnResult)
        return E_INVALIDARG;
    
    int iOrder = 0;
    for (; iOrder < mvSortingForms.size(); iOrder++)
    {
        if(mvSortingForms[iOrder].size() != ivForms.size())
            continue;
        if(mvNullOnTop[iOrder] != ibNullOnTop)
            continue;
        
        int j = 0;
        for (; j < mvSortingForms[iOrder].size(); j++)
            if (mvSortingForms[iOrder][j] != ivForms[j] || mvAsc[iOrder][j] != ipAsc[j])
                break;
        if (j == mvSortingForms[iOrder].size())//found
            break;
    }
    
    if (iOrder < mvSortingForms.size())
    {
        *oppSortingResult = mvSortingResult[iOrder];
        *opnResult = mnRow + 1;//join NULL
        return S_OK;
    }
    else
    {
        *opnResult = 0;
        *oppSortingResult = NULL;
        return S_FALSE;
    }
}

int DSSGlobalLookUpTable::getBucketID(vector<int>& ivForms, bool* ipAsc, bool ibNullOnTop)
{
    if (!ipAsc)
        return E_INVALIDARG;
    
    int iOrder = 0;
    for (; iOrder < mvSortingForms.size(); iOrder++)
    {
        if(ivForms.size() != mvSortingForms[iOrder].size())
            continue;
        if(mvNullOnTop[iOrder] != ibNullOnTop)
            continue;
        int j = 0;
        for (; j < mvSortingForms[iOrder].size(); j++)
            if (mvSortingForms[iOrder][j] != ivForms[j] || mvAsc[iOrder][j] != ipAsc[j])
                break;
        if (j == mvSortingForms[iOrder].size())//found
            break;
    }
    if (iOrder < mvSortingForms.size())
    {
        return iOrder;
    }
    else
    {
        return -1;
    }
}

int DSSGlobalLookUpTable::getSortResult(int iBucketID, int** oppSortingResult, int* opnResult)
{
    if(iBucketID < 0 || iBucketID >= mvSortingForms.size())
        return E_INVALIDARG;
    
    if(mvSortingResultDirty[iBucketID])
    {
        int hr = sortAndMergeCertainSortingResult(iBucketID);
        CHECK_HR;
        mvSortingResultDirty[iBucketID] = false;
    }
    
    *oppSortingResult = mvSortingResult[iBucketID];
    *opnResult = mnRow + 1;//join NULL
    return S_OK;
}

//zhyang
int DSSGlobalLookUpTable::sortAndMergeCertainSortingResult(int iBucketID)
{
    mSortIndex = iBucketID;
    //no need to check whether the sorting result is dirty or not because if the code comes here, the result must be dirty
    //zhyang, sort new elements and merge to existing sorting result
    //for(int i = 0; i < mvSortingResult.size(); ++i)
    {
        //2.construct bucket for existing elements
        //vector<vector<int> > lvExistingResultBucket;
        vector<int> lvExistingResultBucket;
        map<int, vector<int> > extraLink;
        vector<int> lvNewElementsID;
        lvExistingResultBucket.resize(mnRow + 1);//join null
        memset(&lvExistingResultBucket[0], -1, sizeof(int) * (mnRow + 1));
        vector<int> lvNullBuckets;
        
        //1. set all element's rank in lvNewElementsID to -1
        for (int j = 0; j < lvNewElementsID.size(); ++j) {
            mvSortingResult[mSortIndex][lvNewElementsID[j]] = -1;
        }
        
        for(int j = 0; j < mnRow; ++j)
        {
            if(!mpExistingRows[j])
            {
                lvNullBuckets.push_back(j);
                continue;
            }
            
            int rank = mvSortingResult[mSortIndex][j];
            if(rank != -1)
            {
                //lvExistingResultBucket[rank].push_back(j);
                if(-1 == lvExistingResultBucket[rank])
                {
                    lvExistingResultBucket[rank] = j;
                }
                else if(lvExistingResultBucket[rank] >= 0)
                {
                    vector<int> extraBucket;
                    extraBucket.push_back(lvExistingResultBucket[rank]);
                    extraBucket.push_back(j);
                    extraLink.insert(map<int, vector<int> >::value_type(rank, extraBucket));
                    lvExistingResultBucket[rank] = -2;
                }
                else //lvExistingResultBucket[rank] == -2
                {
                    map<int, vector<int> >::iterator iter = extraLink.find(rank);
                    if (iter != extraLink.end()) {
                        (iter->second).push_back(j);
                    }
                    else
                    {
                        assert(false);
                    }
                }
            }
            else
            {
                lvNewElementsID.push_back(j);
            }
        }
        
        //second time serialization and all elements are serialized to client side when doing first time serialization
        if(lvNewElementsID.size() == 0)
            return S_OK;
        
        
        _xtab_sort(&lvNewElementsID[0], &lvNewElementsID[0] + lvNewElementsID.size(), *this);
        //merge
        
        //3. do merge
        int lExistingPos = 0, lNewElementsPos = 0;
        int rank = 0;
        int order = 0;
        
        int lNullBucketSize = lvNullBuckets.size();
        if(mvNullOnTop[mSortIndex] && lNullBucketSize != 0)
        {
            for(int j = 0; j < lNullBucketSize; ++j)
            {
                mvSortingResult[mSortIndex][lvNullBuckets[j]] = rank;
            }
            mvSortingResult[mSortIndex][mnRow] = rank; //zhyang, 824221, there is a null there I don't consider when doing merge
            rank++;
        }
        
        while (lExistingPos < lvExistingResultBucket.size() && lNewElementsPos < lvNewElementsID.size()) {
            if(lvExistingResultBucket[lExistingPos] == -1)  //no element in the bucket
            {
                lExistingPos++;
                continue;
            }
            
            //3.1 compare  lvExistingResultBucket[lExistingStart]'s elements and lvNewElementsID[lNewElementsStart]'s element
            
            int lExistingElementIndex;
            if(lvExistingResultBucket[lExistingPos] >= 0)
                lExistingElementIndex = lvExistingResultBucket[lExistingPos];
            else
            {
                map<int, vector<int> >::iterator iter = extraLink.find(lExistingPos);
                lExistingElementIndex = (iter->second)[0];
            }
            
            for(int i = 0; i < mvSortingForms[mSortIndex].size(); ++i)
            {
                DSSDataColumn* lColumn;
                Item(mvSortingForms[mSortIndex][i], &lColumn);
                if(lColumn)
                {
                    //xiazhou,966577, sort according to ascending setting
                    order = compareHelper(lColumn, lExistingElementIndex, lvNewElementsID[lNewElementsPos],
                                  mvAsc[mSortIndex][i] ? mvNullOnTop[mSortIndex] : !mvNullOnTop[mSortIndex], 0, false, mvAsc[mSortIndex][i]);
                    if(order != 0)
                        break;
                }
                else
                {
                    assert(false);
                }
            }
            
            //3.1.1 if lvExistingResultBucket[lExistingStart]'s elements is smaller
            if(order <= 0)
            {
                /*for(int i = 0; i < lvExistingResultBucket[lExistingPos].size(); ++i)
                 {
                 mvSortingResult[mSortIndex][lvExistingResultBucket[lExistingPos][i]] = rank;
                 }*/
                
                if(lvExistingResultBucket[lExistingPos] >= 0)
                    mvSortingResult[mSortIndex][lExistingElementIndex] = rank;
                else
                {
                    map<int, vector<int> >::iterator iter = extraLink.find(lExistingPos);
                    for(int k = 0; k < (iter->second).size(); ++k)
                    {
                        mvSortingResult[mSortIndex][(iter->second)[k]] = rank;
                    }
                }
                
                lExistingPos++;
                if (order < 0) {
                    rank++;
                    continue;
                }
            }
            
            //3.1.3 if they are equal
            /*if(order == 0)
             {
             for(int i = 0; i < lvExistingResultBucket[lExistingPos].size(); ++i)
             {
             mvSortingResult[mSortIndex][lvExistingResultBucket[lExistingPos][i]] = rank;
             }
             lExistingPos++;
             }*/
            
            //3.1.2 if lvNewElementsID[lNewElementsStart]'s element is smaller
            mvSortingResult[mSortIndex][lvNewElementsID[lNewElementsPos++]] = rank;
            bool lEqual = true;
            while(lEqual && lNewElementsPos < lvNewElementsID.size())
            {
                //check whether lvNewElementsID[lNewElementsPos] and lvNewElementsID[lNewElementsPos-1] are equal
                for(int i = 0; i < mvSortingForms[mSortIndex].size(); ++i)
                {
                    DSSDataColumn* lColumn;
                    Item(mvSortingForms[mSortIndex][i], &lColumn);
                    if(lColumn)
                    {
                        order = compareHelper(lColumn, lvNewElementsID[lNewElementsPos-1], lvNewElementsID[lNewElementsPos],
                                      mvAsc[mSortIndex][i] ? mvNullOnTop[mSortIndex] : !mvNullOnTop[mSortIndex], 0, false, mvAsc[mSortIndex][i]);
                        if(order != 0)
                            break;
                    }
                    else
                    {
                        assert(false);
                    }
                }
                
                if(order == 0)
                    mvSortingResult[mSortIndex][lvNewElementsID[lNewElementsPos++]] = rank;
                else
                    lEqual = false;
            }
            rank++;
        }
        
        while (lExistingPos < lvExistingResultBucket.size())
        {
            /*for(int i = 0; i < lvExistingResultBucket[lExistingPos].size(); ++i)
             {
             mvSortingResult[mSortIndex][lvExistingResultBucket[lExistingPos][i]] = rank;
             }*/
            if(lvExistingResultBucket[lExistingPos] == -1)
            {
                lExistingPos++;
                continue;
            }
            
            if(lvExistingResultBucket[lExistingPos] >= 0)
                mvSortingResult[mSortIndex][lvExistingResultBucket[lExistingPos]] = rank;
            else
            {
                map<int, vector<int> >::iterator iter = extraLink.find(lExistingPos);
                for(int k = 0; k < (iter->second).size(); ++k)
                {
                    mvSortingResult[mSortIndex][(iter->second)[k]] = rank;
                }
            }
            
            rank++;
            lExistingPos++;
        }
        
        //zhyang, fix a crash when lNewElementsPos == 0
        order = -1;
        while (lNewElementsPos < lvNewElementsID.size())
        {
            //check whether lvNewElementsID[lNewElementsPos] and lvNewElementsID[lNewElementsPos-1] are equal
            for(int i = 0; i < mvSortingForms[mSortIndex].size(); ++i)
            {
                DSSDataColumn* lColumn = NULL;
                Item(mvSortingForms[mSortIndex][i], &lColumn);
                if(lColumn && lNewElementsPos)
                {
                    order = compareHelper(lColumn, lvNewElementsID[lNewElementsPos-1], lvNewElementsID[lNewElementsPos], mvAsc[mSortIndex][i] ? mvNullOnTop[mSortIndex] : !mvNullOnTop[mSortIndex], 0, false, mvAsc[mSortIndex][i]);
                    if(order != 0)
                        break;
                }
                else if(!lColumn)
                {
                    assert(false);
                }
            }
            
            if(order != 0)
                rank++;
            mvSortingResult[mSortIndex][lvNewElementsID[lNewElementsPos++]] = rank;
        }
        
        if(!mvNullOnTop[mSortIndex] && lNullBucketSize != 0)
        {
            rank++;
            for(int j = 0; j < lNullBucketSize; ++j)
            {
                mvSortingResult[mSortIndex][lvNullBuckets[j]] = rank;
            }
            mvSortingResult[mSortIndex][mnRow] = rank; //zhyang, 824221, there is a null there I don't consider when doing merge
        }
    }
    
    return S_OK;
}

int DSSGlobalLookUpTable::setSortingResult(vector<Int32>& ivForms, vector<bool>& ipAsc, bool ibNullOnTop, Int32* ipOrder)
{
	AE_ASSERT(ipAsc.size() == ivForms.size() && ipOrder);
    
	//search existing sort result
	Int32 lnForms = ivForms.size();
	Int32 i = 0;
	for (; i < mvSortingForms.size(); i++)
	{
		if (mvSortingForms[i].size() != lnForms)
			continue;
        
        if(mvNullOnTop[i] != ibNullOnTop)
            continue;
        
		Int32 j = 0;
		for (; j < lnForms; j++)
		{
			if (!(mvSortingForms[i][j] == ivForms[j] && ipAsc[j] == mvAsc[i][j]))
				break;
		}
		if (j == lnForms)//found existing sort result
			break;
	}
    
	if (i == mvSortingForms.size())
	{
		bool* lpAsc = new  bool [lnForms];
		Int32* lpOrder = new Int32[mnRow + 1];//include join NULL
		CHECK_NEWED_PTR(lpAsc && lpOrder);
        
		for (Int32 j = 0; j < lnForms; j++)
			lpAsc[j] = ipAsc[j];
        
		memmove(lpOrder, ipOrder, sizeof(Int32) * (mnRow + 1));
        
		mvSortingForms.push_back(ivForms);
		mvAsc.push_back(lpAsc);
		mvSortingResult.push_back(lpOrder);
        mvNullOnTop.push_back(ibNullOnTop);
        mvIsSerializedFromServer.push_back(false);
        mvSortingResultDirty.push_back(false);
	}
    
	return S_OK;
}

int DSSGlobalLookUpTable::getElementCount()
{
    if (!mfHasDE)
        return mnRow;
    else
        return  mnRow+1+mGlobalEleCount;
}

int DSSGlobalLookUpTable::GetDisplayFormUnionForControl(vector<int> &irvUnionForms)
{
    int hr=0;
    
    
    if(mvDisplayFormUnion.size()==0)
    {
        hr=GenerateDisplayFormUnionForControl();
        
        CHECK_HR;
    }
    
    
    irvUnionForms.assign(mvDisplayFormUnion.begin(), mvDisplayFormUnion.end());
    
    return S_OK;
}


int DSSGlobalLookUpTable::GenerateDisplayFormUnionForControl()
{
    int hr=0;
    
    int lnReport=mvpDisplayForms.size();
    
    set<int> lsetForms;
    
    for(int i=0;i<lnReport;i++)
    {
        int *lpDisplayForms=NULL;
        int lnDisplayForms=0;
        
        hr=GetDisplayForm(i, &lpDisplayForms, &lnDisplayForms);
        CHECK_HR;
        
        for(int j=0;j<lnDisplayForms;j++)
        {
            if(std::find(lsetForms.begin(), lsetForms.end(), lpDisplayForms[j])==lsetForms.end())
            {
                lsetForms.insert(lpDisplayForms[j]);
                mvDisplayFormUnion.push_back(lpDisplayForms[j]);
            }
        }
        
    }
    
    
    return S_OK;
    
}


int DSSGlobalLookUpTable::GetDisplayForm(int iSourceIndex, int **oppForms, int *opnForms)
{
    if (iSourceIndex < 0 || iSourceIndex >= mvpDisplayForms.size() || !oppForms || !opnForms)
        return E_INVALIDARG;
    
    *oppForms = mvpDisplayForms[iSourceIndex];
    *opnForms = mvDisplayFormCount[iSourceIndex];
    
    return S_OK;
}

int DSSGlobalLookUpTable::LoadHeaderFromBinary(DSSBinaryDataReader *ipReadStream, bool ifFirtTime)
{
    int lBlockType;
    int hr = ipReadStream->BeginReadBlock(&lBlockType);
    //	CHECK_HR_BLOCK(EnumDataBlockGlobalLookUpTable);
    CHECK_HR;
    
    MBase::CString lStringGUID;
    hr = ipReadStream->ReadString(lStringGUID);
    CHECK_HR;
    
    setGUID(lStringGUID);
    
    hr = ipReadStream->ReadInt(&mArity);
    CHECK_HR;
    
	hr = ipReadStream->ReadInt(&mnColumns);
	CHECK_HR;
    
    if (ifFirtTime)
    {
        mvBaseFormID.resize(mnColumns);
        mvBaseFormType.resize(mnColumns);
        mvColumnAttributes.resize(mnColumns);
        mvColumnLocales.resize(mnColumns);
        //ctang: clean DEs
		for (int i = 0; i < mvpDEs.size(); i++)
			if (mvpDEs[i])
			{
				delete mvpDEs[i];
				mvpDEs[i] = NULL;
			}
        
		mvpDEs.clear();
        
		for (int i = 0; i < mvpDELocalToGlobalMap.size(); i++)
			if (mvpDELocalToGlobalMap[i])
			{
				delete mvpDELocalToGlobalMap[i];
				mvpDELocalToGlobalMap[i] = NULL;
			}
		for (int i = 0; i < mvpDEGlobalToLocalMap.size(); i++)
			if (mvpDEGlobalToLocalMap[i])
			{
				delete mvpDEGlobalToLocalMap[i];
				mvpDEGlobalToLocalMap[i] = NULL;
			}
        
		mvpDELocalToGlobalMap.clear();
		mvpDEGlobalToLocalMap.clear();
        
        if (mpMapDEFromBinaryToStorage)
			delete [] mpMapDEFromBinaryToStorage;
		mpMapDEFromBinaryToStorage = NULL;
		mnDEInBinary = 0;
    }
    
    for (int i = 0; i < mnColumns; i++)
    {
        hr = ipReadStream->ReadInt(&mvBaseFormID[i]);
        CHECK_HR;
    }
    
    for (int i = 0; i < mnColumns; i++)
    {
        int lType;
        hr = ipReadStream->ReadInt(&lType);
        CHECK_HR;
        
        mvBaseFormType[i] = (EnumDSSBaseFormType)lType;
    }
    
    for (int i = 0; i < mnColumns; i++)
    {
        hr = ipReadStream->ReadInt(&mvColumnAttributes[i]);
        CHECK_HR;
    }
    
    for (int i = 0; i < mnColumns; i++)
    {
        hr = ipReadStream->ReadInt(&mvColumnLocales[i]);
        CHECK_HR;
    }
    
    //todo: hold the segment info
    int lnSegment;
    hr = ipReadStream->ReadInt(&lnSegment);
    CHECK_HR;
    
    for (int i = 0; i < lnSegment; i++)
    {
        int tmp;
        hr = ipReadStream->ReadInt(&tmp);
        CHECK_HR;
    }
    
    /*TYPE_RESERVE = 0, DEHIDDEN, ATTRIBUTE, DEDISPLAY*/
    //ctang: element info
    hr = ipReadStream->ReadInt(&mnRow);
    CHECK_HR;
    
    if (ifFirtTime)
    {
        if (mpDEElement)
        {
            delete mpDEElement;
            mpDEElement = NULL;
        }
        
        mpDEElement = new DSSXTabBitVector();
        mpDEElement->Init(mnRow, false);
        
        if (mnRow > 0)
        {
            mpDEElement->BulkSet(0, mnRow - 1, false);
        }
    }
    //ctang: load bit vector, to compact the binary size
    hr = mpDEElement->LoadFromBinary(ipReadStream);
    CHECK_HR;
    
    //todo read distinct flag
    int lfDistinct;
    hr = ipReadStream->ReadInt(&lfDistinct);
    CHECK_HR;
    
    //ctang: hold the display form info
    if (ifFirtTime)
    {
        int lnReport = 0;
        hr = ipReadStream->ReadInt(&lnReport);
        CHECK_HR;
        
        mvpDisplayForms.resize(lnReport);
        mvDisplayFormCount.resize(lnReport);
        for (int i = 0; i < lnReport; i++)
        {
            hr = ipReadStream->ReadInt(&mvDisplayFormCount[i]);
            CHECK_HR;
            
            if (mvpDisplayForms[i])
            {
                delete mvpDisplayForms[i];
            }
            
            mvpDisplayForms[i] = new int[mvDisplayFormCount[i]];
            for (int j = 0; j < mvDisplayFormCount[i]; j++)
            {
                hr = ipReadStream->ReadInt(&mvpDisplayForms[i][j]);
                CHECK_HR;
            }
        }
    }
    
    //zhyang, 763173, for UC on dataset, clean sort result
    for (int i = 0; i < mvAsc.size(); i++)
        if (mvAsc[i])
            delete [] mvAsc[i];
    
    mvAsc.clear();
    
    for (int i = 0; i < mvSortingResult.size(); i++)
        if (mvSortingResult[i])
            delete [] mvSortingResult[i];
    
    mvSortingResult.clear();
    
    mvSortingForms.clear();
    mvIsSerializedFromServer.clear();
    mvSortingResultDirty.clear();
    mvNullOnTop.clear();
    if(mpExistingRows)
        delete mpExistingRows;
    mpExistingRows = NULL;
    
    return ipReadStream->EndReadBlock();
}

int DSSGlobalLookUpTable::GetGlobalUnit(int iGlobalID, DSSTabularUnit** opUnit, int* opLocalKey, bool* ofDE, bool ifViewFirst, int* opSourceIndex, int* opViewIndx,vector<int> *ipvViewIndices)

{
    // weilin 2016/08/30 DE42497 "iGlobalID < 0" means subtotal element, we can use "iGlobalID < 0" here
    // if (iGlobalID < 0)
    //    return E_INVALIDARG;
    
	bool lfDE;
	//ctang 2011-5-31 the leaf elements of DE now are compiled into global lookup table, so we should first try to find these unit from DE, then Glookup
	int hr = IsDEElement(iGlobalID, &lfDE);
	CHECK_HR;
	if (!lfDE)
	{
		//todo ctang: since it is possible that there are in-consistent type of columns, return the local lookup table
        //if (mpDEElement && iGlobalID < mnRow && !mpDEElement->Get(iGlobalID))
        {
            *opLocalKey = iGlobalID;
            *opUnit = this;
            if (ofDE)
                *ofDE = false;
            
            return S_OK;
        }
	}
    
    int lDE = -1;
	//ctang 2011-9-8 search the DE in view first.
	vector<int> lvOrder;
	if (ifViewFirst)
	{
        //TQMS 731649,hyan,3/27/2013
        //following flash's change
        if(ipvViewIndices && ipvViewIndices->size()>0)
        {
            for(int i=0;i<ipvViewIndices->size();i++)
            {
                for (int j = mvViewIndex4DE.size() - 1; j >= mnBaseDE; j--)
                    //for(int j=mnBaseDE;j<mvViewIndex4DE.size();j++)
                {
                    if(ipvViewIndices->at(i)==mvViewIndex4DE[j])
                        lvOrder.push_back(j);
                }
            }
            
            for (int i = 0; i < mnBaseDE; i++)
                lvOrder.push_back(i);
        }
        else
        {
            //zhyang, 782162, 780906
            if(mvpDEs.size() > 0)
                for(int i = mvpDEs.size()-1; i >= mnBaseDE; --i)
                    lvOrder.push_back(i);
            //for (int i = mnBaseDE; i < mvpDEs.size(); i++)
            //    lvOrder.push_back(i);
            for (int i = mnBaseDE; i < mvpDEs.size(); i++)
                lvOrder.push_back(i);
            for (int i = 0; i < mnBaseDE; i++)
                lvOrder.push_back(i);
        }
	}
	else
	{
		for (int i = 0; i < mvpDEs.size(); i++)
			lvOrder.push_back(i);
	}
	for (int j = 0; j < lvOrder.size(); j++)
	{
		int iDE = lvOrder[j];
		if (mvpDEGlobalToLocalMap[iDE])
		{
			if (mvpDEGlobalToLocalMap[iDE]->find(iGlobalID) != mvpDEGlobalToLocalMap[iDE]->end())
			{
				*opLocalKey = (*(mvpDEGlobalToLocalMap[iDE]))[iGlobalID];
				//ctang 2011-7-8
				//if there exist two local DE element map to the same global key (it is possiable for the leaf element, two leaf may have the same global key)
				//chose the first one whose ordinal >= 0
				lDE = iDE;
				int lOrdinal = mvpDEs[iDE]->getElement(*opLocalKey)->ordinal;
				if (lOrdinal >= 0)
					break;
			}
		}
	}
    
	if (lDE >= 0 && lDE < mvpDEs.size())
	{
		*opUnit = static_cast<DSSTabularUnit*>(mvpDEs[lDE]);
        
		if (ofDE)
			*ofDE = true;
        
        //ctang: get the source index and view index
        if (opSourceIndex)
            *opSourceIndex = mvSourceIndex4DE[lDE];
        
        if (opViewIndx)
            *opViewIndx = mvViewIndex4DE[lDE];
        
		return S_OK;
	}
	else
		return S_FALSE;
}


int DSSGlobalLookUpTable::IsDEElement(int iGlobalElementKey, bool* ofDE)
{
	*ofDE = iGlobalElementKey > CountRows();
	return S_OK;
}

int DSSGlobalLookUpTable::HasDE(bool* opfHasDE)
{
    *opfHasDE = mfHasDE;
    return S_OK;
}

//ctang: return is the DE from view or base
int DSSGlobalLookUpTable::IsDEOnView(DSSTabularConsolidation* ipCS, bool* opFlag)
{
    if (!opFlag)
        return E_INVALIDARG;
    *opFlag = false;
    int i;
    for (i = 0; i < mvpDEs.size(); i++)
        if (mvpDEs[i] == ipCS)
            break;
    
    if (i >= mnBaseDE && i < mvpDEs.size())
        *opFlag = true;
    return S_OK;
    
}

void DSSGlobalLookUpTable::EraseInvalid(int iDEPos)
{
    //xzeng, 2012-04-20, to reduce memory leak, delete memory before erase.
    if (mvpDEs[iDEPos])
        delete mvpDEs[iDEPos];
    mvpDEs.erase(mvpDEs.begin() + iDEPos);
    
    mvMaxGlobalKey.erase(mvMaxGlobalKey.begin() + iDEPos);
    mvUnitEleCount.erase(mvUnitEleCount.begin() + iDEPos);
    
    if (mvpDELocalToGlobalMap[iDEPos])
        delete mvpDELocalToGlobalMap[iDEPos];
    mvpDELocalToGlobalMap.erase(mvpDELocalToGlobalMap.begin() + iDEPos);
    
    if (mvpDEGlobalToLocalMap[iDEPos])
        delete mvpDEGlobalToLocalMap[iDEPos];
    mvpDEGlobalToLocalMap.erase(mvpDEGlobalToLocalMap.begin() + iDEPos);
}

void DSSGlobalLookUpTable::ResizeDEVectors(int iDESize)
{
    //xzeng, 2012-04-20, to reduce memory leak.
    int lPreDESize = mvpDEs.size();
    if (iDESize < lPreDESize)
    {
        for(int i = iDESize; i < lPreDESize; i++)
        {
            if (mvpDEs[i]){ delete mvpDEs[i]; mvpDEs[i] = NULL;}
            if (mvpDELocalToGlobalMap[i]){ delete mvpDELocalToGlobalMap[i]; mvpDELocalToGlobalMap[i] = NULL;}
            if (mvpDEGlobalToLocalMap[i]){ delete mvpDEGlobalToLocalMap[i]; mvpDEGlobalToLocalMap[i] = NULL;}
        }
    }
    
    mvpDEs.resize(iDESize);
    mvMaxGlobalKey.resize(iDESize);
    mvUnitEleCount.resize(iDESize);
	mvpDELocalToGlobalMap.resize(iDESize);
	mvpDEGlobalToLocalMap.resize(iDESize);
    
    if(iDESize > lPreDESize)
    {
        for(int i = lPreDESize; i < iDESize; i++)
        {
            mvpDEs[i] = NULL;
            mvpDELocalToGlobalMap[i] = NULL;
            mvpDEGlobalToLocalMap[i] = NULL;
        }
    }
}

int DSSGlobalLookUpTable::LoadDERelatedPreFromBinary(DSSBinaryDataReader *ipReadStream)
{
	//xzeng, 2011-12-15, read DERelatedPre to stream
    
    //vector<GUID> lvTemplateIDs
    //read
    
    //compare with mvViewTemplateID
    //merge a new vector
    
    //load data;
    //for each DE i block in binary
    //if mvDE[j] exist
    //skip this block
    //else
    //load DE and related data, save to mvDE[j], the same to the maps
    
    //clear the map
    if (mpMapDEFromBinaryToStorage)
    {
        delete [] mpMapDEFromBinaryToStorage;
        mpMapDEFromBinaryToStorage = NULL;
        mnDEInBinary = 0;
    }
    
    int lfHasDE;
    HRESULT hr = ipReadStream->ReadInt(&lfHasDE);
    CHECK_HR;
    
    mfHasDE = (bool)lfHasDE;
    
    int lDESize, lViewTemplateSize;
	hr = ipReadStream->ReadInt(&lDESize);
	CHECK_HR;
    
    mnDEInBinary = lDESize;
    
	//read mvViewTemplateID and erase invalid DE
	hr = ipReadStream->ReadInt(&lViewTemplateSize);
	CHECK_HR;
    
    vector<GUID> lvViewTemplateID;
    
    lvViewTemplateID.resize(lViewTemplateSize);
    for (int i = 0; i < lViewTemplateSize; i++)
    {
        //        hr = ipReadStream->ReadGUID(lvViewTemplateID[i]);
        //        CHECK_HR;
        //xzeng 2011-1-5 because now we use FlexWriter in server side to write GUID to make flash side run through
        MBase::CString lStringGUID;
        hr = ipReadStream->ReadString(lStringGUID);
        CHECK_HR;
        
        int len = lStringGUID.length();
        if (len > 0)
        {
#ifdef NO_WCHAR_SUPPORT
            MBase::String2Guid(lvViewTemplateID[i], lStringGUID.c_str());
#else
            vector<wchar_t> lString(len);
            mbstowcs(&lString[0], lStringGUID.c_str(), len);
            MBase::String2Guid(lvViewTemplateID[i], &lString[0]);
#endif
        }
    }
    
    //save mGlobalEleCount
	hr = ipReadStream->ReadInt(&mGlobalEleCount);
	CHECK_HR;
    
	hr = ipReadStream->ReadInt(&mnBaseDE);
	CHECK_HR;
    
    vector<int> lvSourceIndex4DE;
    hr = ipReadStream->ReadIntArray(lvSourceIndex4DE);
    CHECK_HR;
    
    vector<int> lvViewIndex4DE;
    hr = ipReadStream->ReadIntArray(lvViewIndex4DE);
    CHECK_HR;
    
    mpMapDEFromBinaryToStorage = new int[lDESize];
    
    for (int i = 0; i < mnBaseDE; i++)
        mpMapDEFromBinaryToStorage[i] = i;
    
    int lnDEs = mnBaseDE >= mvpDEs.size() ? mnBaseDE : mvpDEs.size();
    for(int i = 0, j = 0; i < lvViewTemplateID.size(); i++)
    {
        for(j = 0; j < mvViewTemplateID.size(); j++)
        {
            if(mvViewTemplateID[j] == lvViewTemplateID[i])
                break;
        }
        
        if (j < mvViewTemplateID.size())
            mpMapDEFromBinaryToStorage[mnBaseDE + i] = mnBaseDE + j;
        else
        {
            mpMapDEFromBinaryToStorage[mnBaseDE + i] = lnDEs++;
        }
    }
    
    mvSourceIndex4DE.resize(lnDEs);
    mvViewIndex4DE.resize(lnDEs);
    
    // jimzhang, Jul/23/2013, TQMS 774236, Tang Cheng said the iterator should start with 0 instead of mnBaseDE.
    //for (int i = mnBaseDE; i < mnDEInBinary; i++)
    for (int i = 0; i < mnDEInBinary; i++)
    {
        mvSourceIndex4DE[mpMapDEFromBinaryToStorage[i]] = lvSourceIndex4DE[i];
        mvViewIndex4DE[mpMapDEFromBinaryToStorage[i]] = lvViewIndex4DE[i];
    }
    
    mvViewTemplateID.resize(lnDEs - mnBaseDE);
    for (int i = 0; i < lViewTemplateSize; i++)
    {
        mvViewTemplateID[mpMapDEFromBinaryToStorage[i + mnBaseDE] - mnBaseDE] = lvViewTemplateID[i];
    }
    
    ResizeDEVectors(lnDEs);
    
    
    return S_OK;
}

int DSSGlobalLookUpTable::GetRefUnit(EnumDSSTemplateUnitType* ipType, int iUnitIndex, DSSTabularUnit** oppTabularUnit)
{
    if (*ipType == DssTemplateAttribute)
    {
        //zhyang, 12/03/2012, create rel attribute
        DSSXTabRefLookupTable* lpRefLookup = new DSSXTabRefLookupTable;
        int hr = lpRefLookup->Init(static_cast<DSSTabularAttribute*>(this));
        CHECK_HR;
        
        *oppTabularUnit = lpRefLookup;
        //*oppTabularUnit = this;
        return S_OK;
    }
    else if (*ipType)
    {
        int lnDEs = mpMapDEFromBinaryToStorage ? mnDEInBinary : mvpDEs.size();
        if (iUnitIndex < 0 || iUnitIndex >= lnDEs)
            return E_INVALIDARG;
        
        int lUnitIndex = mpMapDEFromBinaryToStorage ? mpMapDEFromBinaryToStorage[iUnitIndex] : iUnitIndex;
        DSSTabularConsolidation* lpDE = mvpDEs[lUnitIndex];
        
        *oppTabularUnit = new DSSTabularRefConsolidation();
        CHECK_NEWED_PTR(*oppTabularUnit);
        
        int hr = static_cast<DSSTabularRefConsolidation*>(*oppTabularUnit)->Init(lpDE);
        CHECK_HR;
        
        hr = static_cast<DSSTabularRefConsolidation*>(*oppTabularUnit)->SetGlobalRowMap(mvUnitEleCount[lUnitIndex], NULL, mvpDEGlobalToLocalMap[lUnitIndex], &(*mvpDELocalToGlobalMap[lUnitIndex])[0]);
        CHECK_HR;
        
        // TQMS 785587, tehe: set global key for join null 
        static_cast<DSSTabularRefConsolidation*>(*oppTabularUnit)->setJoinNull(mnRow);
        
        return S_OK;
    }
    return E_FAIL;
}

int DSSGlobalLookUpTable::GetDELocalToGlobalMap(DSSTabularUnit* ipUnit, int** oppMap, EnumDSSTemplateUnitType iType)
{
    if (!ipUnit || !oppMap)
        return E_INVALIDARG;
    
    *oppMap = NULL;
    if (iType == DssTemplateConsolidation)
    {
        for (int i = 0; i < mvpDEs.size(); i++)
            if (mvpDEs[i] == ipUnit)
            {
                *oppMap = mvpDELocalToGlobalMap[i]->size() > 0 ? &(*mvpDELocalToGlobalMap[i])[0] : NULL;
                return S_OK;
            }
    }
    
    return  iType == DssTemplateAttribute ? S_OK : S_FALSE;
}

int DSSGlobalLookUpTable::PartialUpdateForDE(DSSBinaryDataReader *ipReadStream, DSSObjectContext* ipObjContext)
{
    
    
    int lBlockType;
    
    for (int i = 0; i < mnDEInBinary; i++)
    {
        int hr = ipReadStream->BeginReadBlock(&lBlockType);
        CHECK_HR_BLOCK(EnumNewBlockDE);
        
        AE_ASSERT(mpMapDEFromBinaryToStorage);
        Int32 lDEPos = mpMapDEFromBinaryToStorage[i];
        
        if (!mvpDEs[lDEPos])
        {
            EnumBinaryObjectType objtype = EnumObjectReserved;
            hr = ipReadStream->ReadInt((int*)(&objtype));
            CHECK_HR;
            
            int lUnitID = 0;
            hr = ipReadStream->ReadInt(&lUnitID);
            CHECK_HR;
            
            DSSTabularConsolidation *lpDE;
            hr = hLoadUnitConsolidation(ipReadStream, lUnitID, (DSSTabularUnit**)&lpDE, this);
            CHECK_HR;
            
            mvpDEs[lDEPos] = lpDE;
            //ctang: set obj context
            mvpDEs[lDEPos]->setObjectContextRef(ipObjContext);
            
            hr = ipReadStream->ReadInt(&mvMaxGlobalKey[lDEPos]);
            CHECK_HR;
            
            hr = ipReadStream->ReadInt(&mvUnitEleCount[lDEPos]);
            CHECK_HR;
            
            int lSize;
            hr = ipReadStream->ReadInt(&lSize);
            CHECK_HR;
            
            //junwang, 2012/03/29
            assert(lSize >= mvUnitEleCount[lDEPos]);
            
            if(mvpDELocalToGlobalMap[lDEPos]) delete mvpDELocalToGlobalMap[lDEPos];
            mvpDELocalToGlobalMap[lDEPos] = new IntVector(lSize);
            for (int j = 0; j < lSize; j++)
            {
                hr = ipReadStream->ReadInt(&(*mvpDELocalToGlobalMap[lDEPos])[j]);
                CHECK_HR;
            }
            
            hr = ipReadStream->ReadInt(&lSize);
            CHECK_HR;
            
            if(mvpDEGlobalToLocalMap[lDEPos]) delete mvpDEGlobalToLocalMap[lDEPos];
            mvpDEGlobalToLocalMap[lDEPos] = new IntToIntMap();
            for (int j = 0; j < lSize; j++)
            {
                int lKey, lValue;
                hr = ipReadStream->ReadInt(&lKey);
                CHECK_HR;
                
                hr = ipReadStream->ReadInt(&lValue);
                CHECK_HR;
                
                (*mvpDEGlobalToLocalMap[lDEPos])[lKey] = lValue;
            }
        }
        
        hr = ipReadStream->EndReadBlock();
        CHECK_HR;
    }
    
    return S_OK;
}

int DSSGlobalLookUpTable::CountRows()
{
    if (!mvColumns.empty() && mvColumns[0])
		return mnRow;
	else
		return 0;
}

int DSSGlobalLookUpTable::LoadDataFromBinary(DSSBinaryDataReader *ipReadStream, bool ifFirtTime, DSSObjectContext* ipObjContext)
{
    int lBlockType;
    int hr = ipReadStream->BeginReadBlock(&lBlockType);
    CHECK_HR;
    
    int lnRow;
    hr = ipReadStream->ReadInt(&lnRow);
    CHECK_HR;
    
    _ASSERT(lnRow == mnRow);
    
    int lnColumn;
    hr = ipReadStream->ReadInt(&lnColumn);
    CHECK_HR;
    /*if(mvExistingRows.size() == 0)
     {
     mvExistingRows.resize(mnRow);
     for(int i = 0; i < mnRow; ++i)
     {
     mvExistingRows[i] = false;
     }
     }*/
    if(mpExistingRows == NULL)
    {
        mpExistingRows = new bool[mnRow];
        memset(mpExistingRows, false, mnRow * sizeof(bool));
    }
    
    
    bool lfFull = false;
    vector<int> lvSelected;
    bool lfShareSelectedElements = false;
    int lPosition = ipReadStream->GetPosition();
    hr = ipReadStream->BeginReadBlock(&lBlockType);
    CHECK_HR;
    if (lBlockType != EnumNewBlockOptionalSelectElement)
    {
        hr = ipReadStream->EndReadBlock();
        CHECK_HR;
        ipReadStream->SetPosition(lPosition);
    }
    else
    {
        hr = DSSViewDataSetHelper::loadCompressedIntArray(ipReadStream, lvSelected);
        CHECK_HR;
        lfShareSelectedElements = true;
        ipReadStream->EndReadBlock();
    }
    
    for (int i = 0; i < lnColumn; i++)
    {
        int lBlockType;
        int hr = ipReadStream->BeginReadBlock(&lBlockType);
        CHECK_HR;
        
        bool lfCompatible;
        hr = ipReadStream->ReadBool(&lfCompatible);
        CHECK_HR;
        
        if (lfCompatible)
        {
            int lType;
            hr = ipReadStream->ReadInt(&lType);
            CHECK_HR;
            
            int lnRow;
            hr = ipReadStream->ReadInt(&lnRow);
            CHECK_HR;
            
            DSSDataColumn* lpColumn = NULL;
            
            bool lfFullData;
            hr = ipReadStream->ReadBool(&lfFullData);
            CHECK_HR;
            
            vector<int> lvSelectedElement;
            if (!lfFullData)
            {
                if (lfShareSelectedElements)
                    lvSelectedElement.assign(lvSelected.begin(), lvSelected.end());
                else
                {
                    hr = DSSViewDataSetHelper::loadCompressedIntArray(ipReadStream, lvSelectedElement);
                    CHECK_HR;
                    if (i == 0)
                    {
                        lvSelected.assign(lvSelectedElement.begin(), lvSelectedElement.end());
                    }
                }
            }
            
            if (i == 0)
            {
                lfFull = lfFullData;
            }
            
            if (lvSelectedElement.size() == 0 && !lfFullData)
            {
                //we don't need to load this column
                if (ifFirtTime)
                {
                    hr = DSSAEHelper::GenXTabCol((EnumDSSDataType)lType, 0, &lpColumn);
                    CHECK_HR;
                    AddFormDataColumn(mvBaseFormID[i], mvBaseFormType[i], lpColumn);
                }
                hr = ipReadStream->EndReadBlock();
                CHECK_HR;
                
                continue;
            }
            
            if (!ifFirtTime && i < mvColumns.size())
            {
                lpColumn = mvColumns[i];
                int lnCount = 0;
                if (lpColumn)
                {
                    lnCount = lpColumn->getRowCount();
                }
                if ((lnCount == 0 && lnRow != 0) || (lnCount != lnRow))//this column is not loaded last time, we need allocate space now
                {
                    if (lpColumn)
                        delete lpColumn;
                    lpColumn = NULL;
                    
                    mvColumns[i] = NULL;
                    
                    hr = DSSAEHelper::GenXTabCol((EnumDSSDataType)lType, lnRow, &lpColumn);
                    CHECK_HR;
                    
                    mvColumns[i] = lpColumn;
                }
                if (lpColumn->IsRefColumn())
                    return E_UNEXPECTED;
            }
            else
            {
                hr = DSSAEHelper::GenXTabCol((EnumDSSDataType)lType, lnRow, &lpColumn);
                CHECK_HR;
                
            }
            
            bool lfRawData;//seems useless
            hr = ipReadStream->ReadBool(&lfRawData);
            //CHECK_HR;
            
            int lnElements = 0;
            hr = ipReadStream->ReadInt(&lnElements);
            CHECK_HR;
            
            if ((lfFullData && lnElements != lnRow) ||(!lfFullData && lnElements != lvSelectedElement.size()))
                return E_UNEXPECTED;
            
            hr = DSSViewDataSetHelper::loadColumn(ipReadStream, lpColumn, false, lfFullData || lvSelectedElement.size() == 0 ? NULL : &lvSelectedElement);
            CHECK_HR;
            
            //zhyang, 752560
            if(ipReadStream->MoreDataInBlock())
            {
                hr = ipReadStream->BeginReadBlock(&lBlockType);
                CHECK_HR;
                
                int len = 0;
                hr = ipReadStream->ReadInt(&len);
                CHECK_HR;
                
                //zhyang, don't add strong pointer here, input order will be controled by lpColumn
                int *lpInputorder = new int[len];
                hr = ipReadStream->ReadIntArray(lpInputorder, len);
                CHECK_HR;
                
                hr = lpColumn->SetInputOrder(len, lpInputorder);
                CHECK_HR
                
                hr = ipReadStream->EndReadBlock();
                CHECK_HR;
            }
            
            //ctang: set join null
            hr = lpColumn->setFlag(lnRow - 1, DssDataNull);
            CHECK_HR;
            if (ifFirtTime)
                AddFormDataColumn(mvBaseFormID[i], mvBaseFormType[i], lpColumn);
            
        }
        else
        {
            DSSDataColumn* lpRefColumn;
            if (ifFirtTime)
            {
                lpRefColumn = new DSSDataRefColumn();
                CHECK_NEWED_PTR(lpRefColumn);
            }
            else
            {
                lpRefColumn = mvColumns[i];
                if (!lpRefColumn->IsRefColumn())
                    return E_UNEXPECTED;
            }
            
            hr = (static_cast<DSSDataRefColumn*>(lpRefColumn))->LoadDataFromBinary(ipReadStream, ifFirtTime);
            CHECK_HR;
            
            //zhyang, 752560
            if(ipReadStream->MoreDataInBlock())
            {
                hr = ipReadStream->BeginReadBlock(&lBlockType);
                CHECK_HR;
                
                int len = 0;
                hr = ipReadStream->ReadInt(&len);
                CHECK_HR;
                
                //zhyang, don't add strong pointer here, input order will be controled by lpColumn
                int *lpInputorder = new int[len];
                hr = ipReadStream->ReadIntArray(lpInputorder, len);
                CHECK_HR;
                
                hr = lpRefColumn->SetInputOrder(len, lpInputorder);
                CHECK_HR
                
                hr = ipReadStream->EndReadBlock();
                CHECK_HR;
            }
            
            if (ifFirtTime)
                AddFormDataColumn(mvBaseFormID[i], mvBaseFormType[i], lpRefColumn);
            
        }
        
        hr = ipReadStream->EndReadBlock();
        CHECK_HR;
        
    }
    
    //ctang: check the missing form
    if (ifFirtTime)
    {
        for (int iColumn = lnColumn; iColumn < mnColumns; iColumn++)
        {
            DSSDataColumn* lpColumn = NULL;
            //msun, for the missing form, no need to allocate memory
           /* if (lvSelected.size() > 0)
            {
                hr = DSSAEHelper::GenXTabCol(DssDataTypeMissing, mnRow + 1, &lpColumn);
                CHECK_HR;
            }
            else*/
            {
                //ctang: allocate a empty column which can be used to get column type
                hr = DSSAEHelper::GenXTabCol(DssDataTypeMissing, 0, &lpColumn);
                CHECK_HR;
            }
            AddFormDataColumn(mvBaseFormID[iColumn], mvBaseFormType[iColumn], lpColumn);
        }
    }
    
    vector<int> lvNewElementsID;
    bool lbHasNewElements = false;
    
    //todo : load block info
    bool lfBlock = false;
    hr = ipReadStream->ReadBool(&lfBlock);
    CHECK_HR;
    
    if (lfBlock)
    {
        int lnBlock;
        hr = ipReadStream->ReadInt(&lnBlock);
        CHECK_HR;
        for (int i = 0; i < lnBlock; i++)
        {
            int start;
            hr = ipReadStream->ReadInt(&start);
            CHECK_HR;
            
            int end;
            hr = ipReadStream->ReadInt(&end);
            CHECK_HR;
            
            for(int j = start; j <= end; ++j)
            {
                if(false == mpExistingRows[j])
                {
                    lvNewElementsID.push_back(j);
                    mpExistingRows[j] = true;
                    lbHasNewElements = true;
                }
            }
        }
    }
    else if (lfFull)
    {
        for (int i = 0; i < lnRow; i++)
        {
            if (false == mpExistingRows[i])
            {
                lvNewElementsID.push_back(i);
                mpExistingRows[i] = true;
                lbHasNewElements = true;
            }
        }
    }
    else
    {
        for (int i = 0; i < lvSelected.size(); i++)
        {
            if (lvSelected[i] >= 0 && lvSelected[i] < lnRow && false == mpExistingRows[lvSelected[i]])
            {
                lvNewElementsID.push_back(lvSelected[i]);
                mpExistingRows[lvSelected[i]] = true;
                lbHasNewElements = true;
            }
        }
    }
    
    //xzeng, 2011-12-19, Load DE Related
    hr = ipReadStream->BeginReadBlock(&lBlockType);
    CHECK_HR_BLOCK(EnumNewBlockDERelated);
    
    hr = LoadDERelatedPreFromBinary(ipReadStream);
    CHECK_HR;
    
    hr = PartialUpdateForDE(ipReadStream, ipObjContext);
    CHECK_HR;
    
    hr = ipReadStream->EndReadBlock();
    CHECK_HR;
    //DE related content finished
    
    //ctang: load sorting result
    int lnSorting = 0;
    if (ipReadStream->MoreDataInBlock())
    {
        hr = ipReadStream->ReadInt(&lnSorting);
        CHECK_HR;
    }
    
    for (int i = 0; i < lnSorting; i++)
    {
        int lnForms;
        hr = ipReadStream->ReadInt(&lnForms);
        CHECK_HR;
        
        vector<int> lvForms;
        lvForms.resize(lnForms);
        
        int lForm;
        for (int j = 0; j < lnForms; j++)
        {
            hr = ipReadStream->ReadInt(&lForm);
            CHECK_HR;
            
            lvForms[j] = lForm;
        }
        
        
        hr = ipReadStream->ReadInt(&lnForms);
        CHECK_HR;
        
        bool* lpAsc = new bool[lnForms];
        CHECK_NEWED_PTR(lpAsc);
        MBase::StrongPtr<bool> lpStrong(lpAsc);
        
        for (int j = 0; j < lnForms; j++)
        {
            hr = ipReadStream->ReadBool(&lpAsc[j]);
            CHECK_HR;
        }
        
        bool lNullOnTop = false;
        hr = ipReadStream->ReadBool(&lNullOnTop);
        CHECK_HR;
        
        hr = ipReadStream->ReadInt(&lnForms);
        CHECK_HR;
        
        int* lpOrder = new int[lnForms];
        CHECK_NEWED_PTR(lpOrder);
        MBase::StrongPtr<int> lpStrongHolder(lpOrder);
        
        hr = ipReadStream->ReadIntArray(lpOrder, lnForms);
        CHECK_HR;
        
        //check if it is exist
        int* lpTmp = NULL;
        int lntmp = 0;
        
        hr = GetSortingResult(lvForms, lpAsc, lNullOnTop, &lpTmp, &lntmp);
        if (FAILED(hr))
            return hr;
        
        if (hr == S_FALSE)// not exist
        {
            mvSortingForms.push_back(lvForms);
            mvAsc.push_back(lpAsc);
            mvSortingResult.push_back(lpOrder);
            mvNullOnTop.push_back(lNullOnTop);
            mvIsSerializedFromServer.push_back(true);
            mvSortingResultDirty.push_back(false);
            
            lpStrong.GiveUp();
            lpStrongHolder.GiveUp();
        }
    }
    if (ipReadStream->ProbeBlockHeader(EnumNewBlockAttrGMapData) || ipReadStream->ProbeBlockHeader(EnumNewBlockAttributeRank))
    {
        hr = ipReadStream->BeginReadBlock(&lBlockType);
        //yimliu, 2014/09/18, add code to deal with new block EnumNewBlockAttributeRank.
        //CHECK_HR_BLOCK(EnumNewBlockAttrGMapData);
        CHECK_HR;
        //EnumNewBlockAttrGMapData is in second binary for smart client. And EnumNewBlockAttributeRank is for document based on Multi-partitioned cube. Because right now, smart client doesn't support any kind of cube, actually, these two block will not exist at the same time.
        if (lBlockType == EnumNewBlockAttrGMapData)
        {
            
            int DatasetCount = 0;
            hr = ipReadStream->ReadInt(&DatasetCount);
            
            mvDataSetGUID.resize(DatasetCount);
            mvLocalToGlobalMap.resize(DatasetCount);
            mvGlobalToLocalMap.resize(DatasetCount);
            mvGlobalToLocalMap2.resize(DatasetCount);
            for (int i=0; i<DatasetCount; i++)
            {
                MBase::CString lGUIDStr;
                hr = ipReadStream->ReadString(lGUIDStr);
                CHECK_HR;
                
                GUID lReportGUID;
                hr = DSSViewDataSetHelper::str2GUID(lGUIDStr, lReportGUID);
                mvDataSetGUID[i] = lReportGUID;
                //zhyang, local to global map
                //hr = ipReadStream->ReadIntArray(mvLocalToGlobalMap[i]);
                //yimliu, 2014/08/25. Use compressed map.
                hr = DSSViewDataSetHelper::loadCompressedIntArray(ipReadStream, mvLocalToGlobalMap[i]);
                CHECK_HR;
                
                //zhyang, global to local map
                //hr = ipReadStream->ReadIntArray(mvGlobalToLocalMap[i]);
                //yimliu, 2014/08/25. Use compressed map.
                hr = DSSViewDataSetHelper::loadCompressedIntArray(ipReadStream, mvGlobalToLocalMap[i]);
                CHECK_HR;
                
                vector<int> lvGlobalToLocalMap2;
                //hr = ipReadStream->ReadIntArray(lvGlobalToLocalMap2);
                //yimliu, 2014/08/25. Use compressed map.
                hr = DSSViewDataSetHelper::loadCompressedIntArray(ipReadStream, lvGlobalToLocalMap2);
                CHECK_HR;
                
                if (lvGlobalToLocalMap2.size() == 0)
                {
                    mvGlobalToLocalMap2[i] = NULL;
                }
                else
                {
                    IntToIntMap* lpMap = new IntToIntMap();
                    int lSize = lvGlobalToLocalMap2.size() >> 1;
                    for (int j = 0; j < lSize; j++)
                    {
                        (*lpMap)[lvGlobalToLocalMap2[j*2]] = lvGlobalToLocalMap2[j*2+1];
                    }
                    mvGlobalToLocalMap2[i] = lpMap;
                }
            }
            
            ipReadStream->EndReadBlock();//End Reading for EnumNewBlockAttrGMapData
            
            if (ipReadStream->MoreDataInBlock()) {
                hr = ipReadStream->BeginReadBlock(&lBlockType);
                CHECK_HR_BLOCK(EnumNewBlockAttributeRank);
                hr = ipReadStream->EndReadBlock();//End Reading for EnumNewBlockAttributeRank
                CHECK_HR;
            }
        }
        else if (lBlockType == EnumNewBlockAttributeRank)
        {
            hr = ipReadStream->EndReadBlock();//End Reading for EnumNewBlockAttributeRank
            CHECK_HR;
        }
        else
        {
            return E_DATABLOCK;
        }
    }
    
    //financial report, filter, parent elements
    if (ipReadStream->ProbeBlockHeader(EnumNewBlockParent4GLU) && (lnRow > 0))
    {
        hr = ipReadStream->BeginReadBlock(&lBlockType);
        //CHECK_HR_BLOCK(EnumNewBlockParent4GLU);
        CHECK_HR;
        mvParentElements.resize(lnRow);
        hr = ipReadStream->ReadIntArray(&mvParentElements[0], lnRow);
        CHECK_HR;
        
        hr = ipReadStream->EndReadBlock();
        CHECK_HR;
    }
    
	hr = ipReadStream->EndReadBlock();//End Reading for GlobalLookUpTable
	CHECK_HR;
    
    //zhyang
    if(lbHasNewElements)
    {
        //there are new elements coming
        for(int i = 0; i < mvSortingResult.size(); ++i)
        {
            if(mvIsSerializedFromServer[i])
                continue;
            mvSortingResultDirty[i] = true;  //when new elements come, mark the result as dirty
            
            for(int j = 0; j < lvNewElementsID.size(); ++j)
            {
                mvSortingResult[i][lvNewElementsID[j]] = -1;
            }
        }
    }
    
	return hr;
}

int DSSGlobalLookUpTable::getParentElementKeys(vector<int>& ovParentElements)
{
    ovParentElements = mvParentElements;
    
    return S_OK;
}

//junwang, 02/21/2012
int DSSGlobalLookUpTable::GetMaxGlobalKey(int *opMaxKey)
{
    if(opMaxKey==NULL)
        return S_FALSE;
    
    *opMaxKey = -1;
    for (int i=0; i<mvMaxGlobalKey.size(); i++)
    {
        if(*opMaxKey<mvMaxGlobalKey[i])
            *opMaxKey = mvMaxGlobalKey[i];
    }
    
    if(*opMaxKey<mnRow-1)
        *opMaxKey = mnRow - 1;
    
    return S_OK;
}

//junwang, 04/18/2012
/*
void DSSGlobalLookUpTable::dump()
{
    std::cout<<"<DSSGlobalLookUpTable>\n";
    std::cout<<"HasDE = "<< (mfHasDE?"Ture":"False")<<"\n";
    if (mfHasDE)
    {
        std::cout<<"nDEUnit = "<<mvpDEs.size()<<"\n";
        std::cout<<"nBaseDE = "<<mnBaseDE<<"\n";
        
        for (int i=0; i<(int)mvpDEs.size(); i++)
        {
            std::cout<<"\nDE unit "<<i<<"\n";
            std::cout<<"Max Global Key "<<mvMaxGlobalKey[i]<<"\n";
            std::cout<<"Unit Element Count "<<mvUnitEleCount[i]<<"\n";
            std::cout<<"GloalToLocalMap:\n";
            IntToIntMap* lGTLMap = mvpDEGlobalToLocalMap[i];
            if(lGTLMap)
            {
                IntToIntMap::iterator it = (*lGTLMap).begin();
                for(;it!=(*lGTLMap).end();it++)
                    std::cout<<it->first<<"  "<<it->second<<"\n";
            }
            std::cout<<"LocalToGlobalMap:\n";
            IntVector* lLTG = mvpDELocalToGlobalMap[i];
            if (lLTG)
            {
                for(int j=0; j<(*lLTG).size();j++)
                    std::cout<<j<<"  "<<(*lLTG)[j]<<"\n";
            }
            
            std::cout<<"Consolidation unit:\n";
            if(mvpDEs[i])
            {
                mvpDEs[i]->dump();
            }
        }
    }
    std::cout<<"<DSSGlobalLookUpTable/>\n";
    
    //TODO, attribute dump
}
*/
bool DSSGlobalLookUpTable::operator()(int a, int b)
{
    for(int i = 0; i < mvSortingForms[mSortIndex].size(); ++i)
    {
        DSSDataColumn* lColumn = NULL;
        Item(mvSortingForms[mSortIndex][i], &lColumn);
        assert(lColumn != NULL);
        if(lColumn)
        {
            int order = lColumn->Compare(a, b, mvAsc[mSortIndex][i] ? mvNullOnTop[mSortIndex] : !mvNullOnTop[mSortIndex], 0, true);
            if(order != 0)
                return mvAsc[mSortIndex][i] ? order < 0 : order > 0;
        }
        else
        {
            assert(false);
        }
    }
    return false;
}


//junwang, 2012/08/16
int DSSGlobalLookUpTable::getBranchElements(int iGlobalKey, vector<int> &ovBranchElements, bool lfInCludeDeOnView)
{
    int hr = S_OK;
    
    int lnDe = mnBaseDE;
    if(lfInCludeDeOnView)
        lnDe = mvpDEs.size();
    
    for (int i=0; i<mvpDEs.size()&&i<lnDe; i++)
    {
        IntToIntMap* lpGlobalToLocal = mvpDEGlobalToLocalMap[i];
        IntVector* lpLocalToGlobal = mvpDELocalToGlobalMap[i];
        DSSTabularConsolidation* lpConsolidation = mvpDEs[i];
        
        if (lpGlobalToLocal)
        {
            std::map<int, int>::iterator it = (*lpGlobalToLocal).find(iGlobalKey);
            if (it!=(*lpGlobalToLocal).end())
            {
                int lLocalKey = it->second;
                int lCountRow = lpConsolidation->CountRows();
                for (int j=0; j<lCountRow ; j++)
                {
                    XTabConsolidationElement* lpElement = lpConsolidation->getElement(j);
                    if(lpElement&&lpElement->ordinal>=0)
                    {
                        for (int k=0; k<lpElement->mvLeaves.size(); k++) {
                            if(lpElement->mvLeaves[k] == lLocalKey)
                            {
                                ovBranchElements.push_back((*lpLocalToGlobal)[j]);
                            }
                        }
                    }
                }
                
            }
        }
    }
    
    return hr;
}


int DSSGlobalLookUpTable::setTrimFlag(bool ibTrimFlag)
{
    for(int i=0; i<mvColumns.size(); i++)
    {
        if(mvColumns[i] && DSSAEHelper::IsStringColumn(mvColumns[i]))
        {
            mvColumns[i]->setTrimFlag(ibTrimFlag);
        }
    }
    return S_OK;
}

//int DSSGlobalLookUpTable::GetGlobalToLocalMap(DSSTabularUnit* ipUnit, Int32* opRowCount, Int32** oppRowMap, IntToIntMap** oppRowMap2)
//{
//	if (!opRowCount || !oppRowMap || !oppRowMap2)
//		return E_INVALIDARG;
//    
//	HRESULT hr = S_OK;
//    
//	*opRowCount = -1;
//	*oppRowMap = NULL;
//	*oppRowMap2 = NULL;
//    
//    return hr;

    /*
	EnumDSSTemplateUnitType lType = ipUnit->getUnitType();
	if (lType == DssTemplateAttribute)
	{
		DSSTabularAttribute* lpLookUp = static_cast<DSSTabularAttribute*>(ipUnit);
		for (Int32 i = 0; i < mvLookUpTables.size(); i++)
		{
			if (mvLookUpTables[i] == lpLookUp)
			{
				*opRowCount = mnRows;
				CHECK_HR;
				*oppRowMap = mvGlobalToLocalMap1[i];
				*oppRowMap2 = mvGlobalToLocalMap2[i];
				// at most one of them is not NULL
				return S_OK;
			}
		}
	}
	else if (lType == DssTemplateConsolidation)
	{
		//ctang 2011-7-4
		MSynch::RWLock::ReadSmartLock lLock(*mDERWLockPtr);
        
		CDSSXTabConsolidation* lpCS = static_cast<CDSSXTabConsolidation*>(ipUnit);
		for (Int32 i = 0; i < mvDEs.size(); i++)
		{
			if (mvDEs[i] == lpCS)
			{
                
				*opRowCount = mvMaxGlobalKey[i] + 1;
				if (mvDEGlobalToLocalMap2[i])
					*oppRowMap = &(*mvDEGlobalToLocalMap2[i])[0];
				else
					*oppRowMap = NULL;
				*oppRowMap2 = mvDEGlobalToLocalMap[i];
                
				// at most one of them is not NULL
				return S_OK;
			}
		}
	}
    
	return S_FALSE;*/
//}


int DSSGlobalLookUpTable::GetAllGlobalUnit(int iGlobalID, vector<DSSTabularUnit*> *opUnit, vector<int> *opLocalKey, vector<bool> *ofDE, bool ifViewFirst, vector<int> *opSourceIndex, vector<int> *opViewIndx, vector<int> *ipvViewIndices)
{
    
    if (iGlobalID < 0)
        return E_INVALIDARG;
    
	bool lfDE;
	//ctang 2011-5-31 the leaf elements of DE now are compiled into global lookup table, so we should first try to find these unit from DE, then Glookup
	int hr = IsDEElement(iGlobalID, &lfDE);
	CHECK_HR;
	if (!lfDE)
	{
		//todo ctang: since it is possible that there are in-consistent type of columns, return the local lookup table
        //if (mpDEElement && iGlobalID < mnRow && !mpDEElement->Get(iGlobalID))
        {
            opLocalKey->push_back(iGlobalID);
            opUnit->push_back(this);
            if(ofDE)
                ofDE->push_back(false);
            return S_OK;
        }
	}
    
    int lDE = -1;
	//ctang 2011-9-8 search the DE in view first.
	vector<int> lvOrder;
	if (ifViewFirst)
	{
        //TQMS 731649,hyan,3/27/2013
        //following flash's change
        if(ipvViewIndices && ipvViewIndices->size()>0)
        {
            for(int i=0;i<ipvViewIndices->size();i++)
            {
                for (int j = mvViewIndex4DE.size() - 1; j >= mnBaseDE; j--)
                    //for(int j=mnBaseDE;j<mvViewIndex4DE.size();j++)
                {
                    if(ipvViewIndices->at(i)==mvViewIndex4DE[j])
                        lvOrder.push_back(j);
                }
            }
            
            for (int i = 0; i < mnBaseDE; i++)
                lvOrder.push_back(i);
        }
        else
        {
            //zhyang, 782162, 780906
            if(mvpDEs.size() > 0)
                for(int i = mvpDEs.size()-1; i >= mnBaseDE; --i)
                    lvOrder.push_back(i);
            //for (int i = mnBaseDE; i < mvpDEs.size(); i++)
            //    lvOrder.push_back(i);
            for (int i = 0; i < mnBaseDE; i++)
                lvOrder.push_back(i);
        }
	}
	else
	{
		for (int i = 0; i < mvpDEs.size(); i++)
			lvOrder.push_back(i);
	}
    
	for (int j = 0; j < lvOrder.size(); j++)
	{
		int iDE = lvOrder[j];
		if (mvpDEGlobalToLocalMap[iDE])
		{
			if (mvpDEGlobalToLocalMap[iDE]->find(iGlobalID) != mvpDEGlobalToLocalMap[iDE]->end())
			{
				int lLocalKey = (*(mvpDEGlobalToLocalMap[iDE]))[iGlobalID];
				//ctang 2011-7-8
				//if there exist two local DE element map to the same global key (it is possiable for the leaf element, two leaf may have the same global key)
				//chose the first one whose ordinal >= 0
				lDE = iDE;
				int lOrdinal = mvpDEs[iDE]->getElement(lLocalKey)->ordinal;
				if (lOrdinal >= 0 && lDE >= 0 && lDE < mvpDEs.size())
                {
                    opLocalKey->push_back(lLocalKey);
                    opUnit->push_back(static_cast<DSSTabularUnit*>(mvpDEs[lDE]));
                    
                    if (ofDE)
                        ofDE->push_back(true);
                    
                    //ctang: get the source index and view index
                    if (opSourceIndex)
                        opSourceIndex->push_back(mvSourceIndex4DE[lDE]);
                    
                    if (opViewIndx)
                        opViewIndx->push_back(mvViewIndex4DE[lDE]);
                }
			}
		}
	}
    
    return S_OK;
}

int DSSGlobalLookUpTable::compareHelper(DSSDataColumn *ipColumn, long long iRow1, long long iRow2, bool ifIsNullDisplayOnTop, int iLocal, bool ibSimpleComparison, bool iAsc)
{
    int order = ipColumn->Compare(iRow1, iRow2, ifIsNullDisplayOnTop, iLocal);
    return iAsc ? order : -order;
}

int DSSGlobalLookUpTable::ExtendRow(int inRow)
{
    int lnExtendedRow = mnRow + inRow;
    
    // copy existing rows and initialize extended rows as true
    MBase::DSSStrongPtr<bool, MBase::DeleteArray<bool> >lExistingRowsPtr(new bool[lnExtendedRow]);
    memmove(lExistingRowsPtr.Get(), mpExistingRows, mnRow * sizeof(bool));
    for (int i = 0; i < inRow; ++i)
        lExistingRowsPtr.Get()[mnRow+i] = true;
    
    delete [] mpExistingRows;
    mpExistingRows = lExistingRowsPtr.GiveUp();
    
    mnRow = lnExtendedRow;
    
    resetSortingResult();
    
    return S_OK;
}

int DSSGlobalLookUpTable::FindElement(DSSBaseElementProxy *ipElement, int &orKey)
{
    int hr = S_OK;
    EnumDSSElementType lType = ipElement->getElementType();
    if(lType == DssElementConsolidation)
    {
        //it is a DE;
        for(int i = 0; i < mvpDEs.size(); ++i)
        {
            hr = mvpDEs[i]->FindElement(ipElement, orKey);
            if(hr == S_OK)
            {
                orKey = (*(mvpDELocalToGlobalMap[i]))[orKey];
                return hr;
            }
        }
        
        return S_FALSE;
        
    }
    else
    {
        return DSSTabularAttribute::FindElement(ipElement, orKey);
    }
    
    return hr;
}
vector<int>* DSSGlobalLookUpTable::getLocalToGlobalMap(GUID iDataSetGUID)
{
    for (int i=0; i<mvDataSetGUID.size(); i++)
    {
        if (iDataSetGUID == mvDataSetGUID[i])
        {
            return &mvLocalToGlobalMap[i];
            
        }
    }
    return NULL;
}

vector<int>* DSSGlobalLookUpTable::getGlobalToLocalMap(GUID iDataSetGUID)
{
    for (int i=0; i<mvDataSetGUID.size(); i++)
    {
        if (iDataSetGUID == mvDataSetGUID[i])
        {
            return &mvGlobalToLocalMap[i];
        }
    }
    return NULL;
}

IntToIntMap* DSSGlobalLookUpTable::getGlobalToLocalMap2(GUID iDataSetGUID)
{
    for (int i=0; i<mvDataSetGUID.size(); i++)
    {
        if (iDataSetGUID == mvDataSetGUID[i])
        {
            return mvGlobalToLocalMap2[i];
        }
    }
    return NULL;
}

int DSSGlobalLookUpTable::resetSortingResult()
{
    for (int i = 0; i < mvAsc.size(); i++)
        if (mvAsc[i])
            delete [] mvAsc[i];
    
    mvAsc.clear();
    
    for (int i = 0; i < mvSortingResult.size(); i++)
        if (mvSortingResult[i])
            delete [] mvSortingResult[i];
    
    mvSortingResult.clear();
    
    mvSortingForms.clear();
    
    mvSortingResultDirty.clear();
    
    return S_OK;
}
