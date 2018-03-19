//
//  DSSXTabDispInfo.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 11/14/11.
//  Copyright 2011 MicroStrategy, Inc.. All rights reserved.
//

#include "DSSXTabDispInfo.h"
#include <stdlib.h>

#include "DSSAEHelper.h"
DSSXTabDispInfo::DSSXTabDispInfo()
:mnPosition(0)
{
}


DSSXTabDispInfo::~DSSXTabDispInfo()
{
   	int n = mvpvLevel.size(), i;
	for(i = 0; i < n; i ++)
	{
		delete [] mvpOrgUnit[i];
		delete mvpvLevel[i];
		delete mvpvLevel2[i];
	}
    
	n = mvpSliceInfos.size();
	for(i = 0; i < n; i ++)
		delete mvpSliceInfos[i];
}



int DSSXTabDispInfo::get_LevelCount(int *pLevelCount)
{
    if(pLevelCount == NULL)
		return E_POINTER;
    
	*pLevelCount = mvpvLevel.size();
	return S_OK;
}

int DSSXTabDispInfo::get_PositionCount(int *pMetricCount)
{
   
	if(pMetricCount == NULL)
		return E_POINTER;
    
	*pMetricCount = mnPosition;
	return S_OK;
    
}

int DSSXTabDispInfo::get_SliceCountByLevel(int  iLevel, int *pSliceCount)
{
	if(iLevel < 0 || iLevel >= mvpvLevel.size())
		return E_INVALIDINDEX;
    
	if(pSliceCount == NULL)
		return E_POINTER;
    
	*pSliceCount = mvpvLevel[iLevel] -> size();
	return S_OK;
}

int DSSXTabDispInfo::get_SliceCountByLevel2(int  iLevel, int *pSliceCount)
{
	if(iLevel < 0 || iLevel >= mvpvLevel2.size())
		return E_INVALIDINDEX;
    
	if(pSliceCount == NULL)
		return E_POINTER;
    
	*pSliceCount = mvpvLevel2[iLevel] -> size();
	return S_OK;
}

int DSSXTabDispInfo::get_SliceInfo(int  iLevel, int iOffset, int *pSliceID, int *pPosition, int *pTag)
{
	if(iLevel < 0 || iLevel >= mvpvLevel.size())
		return E_INVALIDINDEX;
	
	if(iOffset <0 || iOffset >= mvpvLevel[iLevel] -> size())
		return E_INVALIDINDEX;
    
	if((pSliceID == NULL) || (pPosition == NULL) || (pTag == NULL))
		return E_POINTER;
    
	int index = (*mvpvLevel[iLevel])[iOffset];
	if(index >= mvpSliceInfos.size())
		return E_FAIL;
    
	DSSXTabDispSliceInfo * pInfo = static_cast<DSSXTabDispSliceInfo *> (mvpSliceInfos[index]);
	*pSliceID = pInfo -> mSliceID;
	*pPosition= pInfo -> mPosition;
	*pTag = pInfo -> mTag;
    
	return S_OK;
}

int DSSXTabDispInfo::get_SliceInfo2(int  iLevel, int iOffset, int *pSliceID, int *pPosition, int *pTag, EnumDssSliceFlag * pSliceFlag)
{
	if(iLevel < 0 || iLevel >= mvpvLevel2.size())
		return E_INVALIDINDEX;
	
	if(iOffset <0 || iOffset >= mvpvLevel2[iLevel] -> size())
		return E_INVALIDINDEX;
    
	if((pSliceID == NULL) || (pPosition == NULL) || (pTag == NULL) || (pSliceFlag == NULL))
		return E_POINTER;
    
	int index = (*mvpvLevel2[iLevel])[iOffset];
	if(index >= mvpSliceInfos.size())
		return E_FAIL;
    
	DSSXTabDispSliceInfo * pInfo = static_cast<DSSXTabDispSliceInfo *> (mvpSliceInfos[index]);
	*pSliceID = pInfo -> mSliceID;
	*pPosition= pInfo -> mPosition;
	*pTag = pInfo -> mTag;
	*pSliceFlag = pInfo -> mSliceFlag;
    
	return S_OK;
}

int DSSXTabDispInfo::get_SliceInfo3(int  iLevel, int iOffset, int *pSliceID, int *pPosition,
                                    int *pTag,
                                    EnumDssSliceFlag * pSliceFlag,
                                    EnumDSSRuntimeSubtotalType *pExtremeType,
                                    int *pModelIndex,
                                    int *pTRMetricID,
                                    int *pXVariableCount,
                                    int **ppXVariables,
                                    EnumDSSObjectType **ppXVariableTypes,
                                    int **ppXIndexMetrics)
{
    if(iLevel < 0 || iLevel >= mvpvLevel2.size())
        return E_INVALIDINDEX;
    
    if(iOffset <0 || iOffset >= mvpvLevel2[iLevel] -> size())
        return E_INVALIDINDEX;
    
    if((pSliceID == NULL) || (pPosition == NULL) || (pTag == NULL) || (pSliceFlag == NULL) || (pExtremeType==NULL)
       || (pModelIndex == NULL) || (pTRMetricID == NULL) || (pXVariableCount == NULL) || (ppXVariables == NULL) || (ppXIndexMetrics == NULL))
        return E_POINTER;
    
    Int32 index = (*mvpvLevel2[iLevel])[iOffset];
    if(index >= mvpSliceInfos.size())
        return E_FAIL;
    
    DSSXTabDispSliceInfo * pInfo = static_cast<DSSXTabDispSliceInfo *> (mvpSliceInfos[index]);
    *pSliceID = pInfo -> mSliceID;
    *pPosition= pInfo -> mPosition;
    *pTag = pInfo -> mTag;
    *pSliceFlag = pInfo -> mSliceFlag;
    *pExtremeType = pInfo->mExtremeType;
    *pModelIndex = pInfo->mModelIndex;
    *pTRMetricID = pInfo->mTRMetricID;
    *pXVariableCount = pInfo->mnXVariable;
    if (pInfo->mnXVariable > 0)
    {
        *ppXVariables = &(pInfo->mvXVariables[0]);
        *ppXVariableTypes = &(pInfo->mvXVariableTypes[0]);
        *ppXIndexMetrics = &(pInfo->mvXIndexMetrics[0]);
    }
    else
    {
        *ppXVariables = NULL;
        *ppXVariableTypes = NULL;
        *ppXIndexMetrics = NULL;
    }
    
    return S_OK;
}

int DSSXTabDispInfo::get_UnitCountByLevel(int  iLevel, int *pCount)
{
	if(iLevel < 0 || iLevel >= mvpvLevel.size())
		return E_INVALIDINDEX;
    
	if(pCount == NULL)
		return E_POINTER;
	
	*pCount = mvnOrgUnit[iLevel];
	return S_OK;
}

int DSSXTabDispInfo::get_OrgUnitByLevel(int  iLevel, int cb, int *pOrgUnit, int *pcbWritten)
{
	if(iLevel < 0 || iLevel >= mvpvLevel.size())
		return E_INVALIDINDEX;
	
	if(cb < mvnOrgUnit[iLevel])
		return E_FAIL;
    
	if(pcbWritten == NULL)
		return E_POINTER;
    
	*pcbWritten = mvnOrgUnit[iLevel];
    
	if(mvnOrgUnit[iLevel] != 0)
		memmove(pOrgUnit, mvpOrgUnit[iLevel], sizeof(int)*mvnOrgUnit[iLevel]);
    
	return S_OK;
}

int DSSXTabDispInfo::put_SliceInfo(int  iLevel,
                                   int  SliceID,
                                   int  Position,
                                   int  Tag,
                                   EnumDssSliceFlag SliceFlag,
                                   EnumDSSRuntimeSubtotalType ExtremeType,
                                   int iModelIndex,
                                   int iTRMetricID,
                                   int nXVariable,
                                   int *ipXVariables,
                                   EnumDSSObjectType *ipXVariableTypes,
                                   int *ipXIndexMetrics)
{
    if(iLevel < 0 || iLevel >= mvpvLevel.size())
        return E_INVALIDINDEX;
    
    if(Position >= mnPosition)
        //can not do that
        return E_FAIL;
    
    DSSXTabDispSliceInfo * pInfo = new DSSXTabDispSliceInfo(SliceID, Tag, Position, SliceFlag, ExtremeType, iModelIndex, iTRMetricID, nXVariable, ipXVariables, ipXVariableTypes, ipXIndexMetrics);
    
    if(SliceFlag == DssOriginalSlice)
        //mvpvLevel only keep DssOriginalSlice
        mvpvLevel[iLevel] -> push_back(mvpSliceInfos.size());
    
    //mvpvLevel2 keep all slice
    mvpvLevel2[iLevel] -> push_back(mvpSliceInfos.size());
    
    mvpSliceInfos.push_back(pInfo);
    
    return S_OK;
}

int DSSXTabDispInfo::putAttributeThresholdSliceInfo(int iLevel, int iTag, DSS_ID ipAttriID, int opSliceID)
{
    std::stringstream keyString;
    keyString << iLevel;
    keyString << "_";
    keyString << iTag;
    keyString << "_";
    keyString << opSliceID;
    
    mAttributeThresholdSlices.push_back(std::make_pair(keyString.str(), ipAttriID));
    
    return S_OK;
}

DSSXTabDispInfo::AttributeThresholdSliceInfo& DSSXTabDispInfo::getAttributeThresholdSlices()
{
    return mAttributeThresholdSlices;
}

int DSSXTabDispInfo::init(int nPosition)
{
	if(mnPosition != 0)
		return E_FAIL;		//we should use a status later.
    
    mAttributeThresholdSlices.clear();
	mnPosition = nPosition;
	return S_OK;
}

int DSSXTabDispInfo::AddLevel(int inUnit, int *ipUnit, int *opLevel)
{
	//it is ok to be equal to zero, grand total
	if(inUnit < 0)
		return E_FAIL;
    
	//search for the same level first
	int lnLevel = mvpvLevel.size(), iLevel, iUnit;
    //	bool lfFound = false;
	for(iLevel = 0; iLevel < lnLevel; iLevel ++)
	{
		//compare the mvpOrgUnit[iLevel] with ipUnit
		if(mvnOrgUnit[iLevel] == inUnit)
		{
			for(iUnit = 0; iUnit < inUnit; iUnit ++)
				if(mvpOrgUnit[iLevel][iUnit] != ipUnit[iUnit])
					break;
			
			if(iUnit == inUnit)
			{
				//if it reach here, it means they are the same
				*opLevel = iLevel;
				return S_OK;
			}
		}
	}
    
	//not found, add it
	int * lpUnit = new int[inUnit];
	memmove(lpUnit, ipUnit, sizeof(int) * inUnit);
    
	mvpOrgUnit.push_back(lpUnit);
	mvnOrgUnit.push_back(inUnit);
    
	mvpvLevel.push_back(new vector<int>);
	mvpvLevel2.push_back(new vector<int>);
	
	*opLevel = lnLevel;	//that is the previous number of level
	return S_OK;
}

// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
//#if 0
//if(iLevel < 0 || iLevel >= mnLevel)
//return DSSXTAB_E_INVALIDINDEX;
//
//if(mnOrgUnit[iLevel] != 0)
////already set
//return E_FAIL;
//
//mnOrgUnit[iLevel] = cb;
//mpOrgUnit[iLevel] = new (mBuffer) int[mnOrgUnit[iLevel]];
//memmove(mpOrgUnit[iLevel], pOrgUnit, sizeof(int) * mnOrgUnit[iLevel]);
//
//return S_OK;

// #endif