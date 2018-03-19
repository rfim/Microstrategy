//
//  DSSXTabLayoutUnitInfo.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 11/14/11.
//  Copyright 2011 MicroStrategy, Inc.. All rights reserved.
//

#include "DSSXTabLayoutUnitInfo.h"

#include "DSSrwdconstants.h"

#include "DSSAEHelper.h"

DSSXTabLayoutUnitInfo::DSSXTabLayoutUnitInfo()
:  mUnitID(-1), mIsMetric(false)
{
}


DSSXTabLayoutUnitInfo::~DSSXTabLayoutUnitInfo()
{
}

int DSSXTabLayoutUnitInfo::get_UnitID(int *pUnitID)
{
	if (pUnitID == NULL) return E_POINTER;
    
	*pUnitID = mUnitID;
    
	return S_OK;
}

int DSSXTabLayoutUnitInfo::Is_Metric(int *pFlag)
{
	if (pFlag == NULL) return E_POINTER;
    
	*pFlag = mIsMetric;
    
	return S_OK;
}

int DSSXTabLayoutUnitInfo::Init(int UnitID, bool IsMetric)
{
	mUnitID = UnitID;
	mIsMetric = IsMetric;
    
	return S_OK;
}