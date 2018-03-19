/*
 *  DSSTabularMetrics.cpp
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#include "DSSTabularMetrics.h"
#include "GUID.h"
#import "DSSCubeDataSlice.h"
#import "DSSMetricElementProxy.h"

DSSTabularMetrics::DSSTabularMetrics()
{
	mUnitType = DssTemplateMetrics;
}

DSSTabularMetrics::~DSSTabularMetrics()
{
    /*	//TODO: if mvMetrics hold weak reference?
     int lnMetrics = mvMetrics.size();
     for (int i = 0; i < lnMetrics; i++)
     {
     if (mvMetrics[i])
     {
     delete mvMetrics[i];
     mvMetrics[i] = NULL;
     }
     }*/
}

int DSSTabularMetrics::Init(int index, int nMetrics)
{
	mUnitID = index;
//	mvMetricIDs.reserve(nMetrics);
//	mvDataTypes.reserve(nMetrics);
//	mvTotalImpl.reserve(nMetrics);
	return S_OK;
}

int DSSTabularMetrics::AddMetric(int index, MBase::CString &irGUID, int iDataType, int iSubtotalType, bool isDisplayed)
{
	//TODO: keep the order as index?
	mvMetricIDStr.push_back(irGUID);
	int len = irGUID.length();
	if (len > 0)
	{
		GUID lGUID;
#ifdef NO_WCHAR_SUPPORT
		MBase::String2Guid(lGUID, irGUID.c_str());
#else
		vector<wchar_t> lString(len);
		mbstowcs(&lString[0], irGUID.c_str(), len);
		MBase::String2Guid(lGUID, &lString[0]);
#endif
		mvMetricIDs.push_back(lGUID);
        if (isDisplayed)
            mvDisplayedMetricIDs.push_back(lGUID);
	}
	mvDataTypes.push_back((EnumDSSDataType)iDataType);
	mvTotalImpl.push_back((EnumDSSMetricType)iSubtotalType);
    
	return S_OK;
}

int DSSTabularMetrics::getMetricID(int iMetric, GUID &orMetricID)
{
    /*
	if (iMetric >= 0 && iMetric < mvMetricIDs.size())
	{
		orMetricID = mvMetricIDs[iMetric];
		return S_OK;
	}
	else
	{
		return E_INVALIDINDEX;
	}
     */
    if (iMetric >= 0 && iMetric < mvDisplayedMetricIDs.size())
	{
		orMetricID = mvDisplayedMetricIDs[iMetric];
		return S_OK;
	} 
	else
	{
		return E_INVALIDINDEX;
	}
}

const GUID& DSSTabularMetrics::getMetricID(int iMetric)
{
	if (iMetric >= 0 && iMetric < mvMetricIDs.size())
		return mvMetricIDs[iMetric];
	else
		return GUID_NULL;
}

int DSSTabularMetrics::FindElement(DSSBaseElementProxy *ipElement, int &orKey)
{
	if (ipElement->getElementType() != DssElementMetric)
		return E_INVALIDARG;
    
	GUID lObjID = ipElement->getObjectID();
    
	//TODO[opt]: use non-linear search
	int lnMetrics = mvMetricIDs.size();
	for (int i = 0; i < lnMetrics; i++)
	{
		if (MBase::IsEqualGUID(lObjID, mvMetricIDs[i]))
		{
			orKey = i;
			return S_OK;
		}
	}
    
	orKey = -1;
	return S_FALSE;
}

int DSSTabularMetrics::createElement(int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement, int iGlobalKey,int iGlobalKeyVer)
{
	if (key >= 0 && key < mvMetricIDs.size() && oppElement)
	{
		*oppElement = new DSSMetricElementProxy(ipObjectContext, mvMetricIDs[key]);
		return S_OK;
	}
	return E_FAIL;
}

int DSSTabularMetrics::FindMetric(GUID &irMetricID)
{
	int lnMetrics = mvMetricIDs.size();
	int i = 0;
	for (i = 0; i < lnMetrics; i++)
	{
		if (MBase::IsEqualGUID(irMetricID, mvMetricIDs[i]))
			return i;
	}
	return -1;
}

int DSSTabularMetrics::FindMetricI(GUID irMetricID, int *opMetricIndex)
{
    if (opMetricIndex)
        *opMetricIndex = FindMetric(irMetricID);
    return S_OK;
}

EnumDSSDataType DSSTabularMetrics::getMetricDataType(int iMetric)
{
	if (iMetric >= 0 && iMetric < mvMetricIDs.size())
	{
		return mvDataTypes[iMetric];
	}
	return DssDataTypeUnknown;
}

int DSSTabularMetrics::size()
{
	int lSize = sizeof(this);
	
	lSize += DSSTabularUnit::size();
	
	lSize += DSSAEHelper::SizeOfVector(mvMetricIDs);
	
	lSize += DSSAEHelper::SizeOfStrings(mvMetricIDStr);
	
	lSize += mvDataTypes.size() * sizeof(EnumDSSDataType);
	
	lSize += mvDataTypes.size() * sizeof(EnumDSSMetricType);
	
	return lSize;
}

EnumDSSMetricType DSSTabularMetrics::getTotalImplementation(int iMetric)
{
	if (iMetric >= 0 && iMetric < mvTotalImpl.size())
		return mvTotalImpl[iMetric];
	else
		return DssMetricReserved;
}

bool DSSTabularMetrics::compareElement(DSSBaseElementProxy *ipElement, int iKey)
{
    if (ipElement->getElementType() != DssElementMetric)
		return false;
    
	int lnMetrics = mvMetricIDs.size();
    if(iKey<0||iKey>=lnMetrics)
        return false;
    
	GUID lObjID = ipElement->getObjectID();
    
    if(MBase::IsEqualGUID(lObjID, mvMetricIDs[iKey]))
        return true;
    
	return false;
}


int DSSTabularMetrics::addSlice(DSSCubeDataSlice* ipSlice)
{
    mvSlices.push_back(ipSlice);
    return S_OK;
}

int DSSTabularMetrics::AddMetricToAnotherMetrics(DSSTabularMetrics* opMetrics)
{
    for (int i = 0; i < mvMetricIDs.size(); i++)
    {
        if (-1 == opMetrics->FindMetric(mvMetricIDs[i]))
        {
            opMetrics->AddMetric(0/* no use for now*/, mvMetricIDStr[i], mvDataTypes[i], mvTotalImpl[i]);
            opMetrics->setDynamicAggFunc(getDynamicAggFunc(i));
            opMetrics->setTotalImplID(mvTotalImplIDs[i]);
            opMetrics->setDynamicAggID(mvDynamicAggIDs[i]);
            opMetrics->setAggInfo(mvAggAttr[i], mvAggInfo[i]);
            opMetrics->setAvailableSubtotals(mvAvailableSystemSubtotals[i], mvAvailableUserDefinedSubtotals[i]);
        }
    }
    return S_OK;
}

int DSSTabularMetrics::FindDisplayedMetric(GUID &irMetricID)
{
	int lnMetrics = mvDisplayedMetricIDs.size();
	int i = 0;
	for (i = 0; i < lnMetrics; i++)
	{
		if (MBase::IsEqualGUID(irMetricID, mvDisplayedMetricIDs[i]))
			return i;
	}
	return -1;
}

vector<GUID> DSSTabularMetrics::getDisplayedMetricIDs()
{
    return mvDisplayedMetricIDs;
}

int DSSTabularMetrics::setDisplayedMetricIDs(vector<GUID> iGUIDs)
{
    mvDisplayedMetricIDs = iGUIDs;
    
    return S_OK;
}

int DSSTabularMetrics::setAggInfo(vector<GUID> &ivAggAttr, vector<EnumDSSAggregation> &ivAggInfo)
{
    mvAggAttr.push_back(ivAggAttr);
    mvAggInfo.push_back(ivAggInfo);
    return S_OK;
}

int DSSTabularMetrics::getAggregationInfo(GUID iMetricID, vector<GUID> &ovNonAggAttr, vector<EnumDSSAggregation> &ovAggInfo)
{
    int pos = FindMetric(iMetricID);
    if (pos != -1)
    {
        ovNonAggAttr = mvAggAttr[pos];
        ovAggInfo = mvAggInfo[pos];
    }
    return S_OK;
}

int DSSTabularMetrics::setDynamicAggFunc(EnumDSSMetricType iFunc)
{
    mvDynamicAggFunc.push_back(iFunc);
    return S_OK;
}

EnumDSSMetricType DSSTabularMetrics::getDynamicAggFunc(int iIndex)
{
    if (iIndex >= 0 && iIndex < mvDynamicAggFunc.size())
		return mvDynamicAggFunc[iIndex];
	else
		return DssMetricReserved;
}

int DSSTabularMetrics::setTotalImplID(GUID iTotalImplID)
{
    mvTotalImplIDs.push_back(iTotalImplID);
    return S_OK;
}

int DSSTabularMetrics::setDynamicAggID(GUID iDAImplID)
{
    mvDynamicAggIDs.push_back(iDAImplID);
    return S_OK;
}

GUID DSSTabularMetrics::getTotalImplID(int iIndex)
{
    if (iIndex >= 0 && iIndex < mvTotalImplIDs.size())
		return mvTotalImplIDs[iIndex];
	else
		return GUID_NULL;
}

GUID DSSTabularMetrics::getDynamicAggID(int iIndex)
{
    if (iIndex >= 0 && iIndex < mvDynamicAggIDs.size())
		return mvDynamicAggIDs[iIndex];
	else
		return GUID_NULL;
}

int DSSTabularMetrics::clear()
{
    mvMetricIDStr.clear();
	mvMetricIDs.clear();
    mvDisplayedMetricIDs.clear();
    mvDataTypes.clear();
	mvTotalImpl.clear();
    
    return S_OK;
}

int DSSTabularMetrics::setAvailableSubtotals(int iSystemSubtotal, vector<GUID> iUserDefinedSubtotals)
{
    mvAvailableSystemSubtotals.push_back(iSystemSubtotal);
    mvAvailableUserDefinedSubtotals.push_back(iUserDefinedSubtotals);
    
    return S_OK;
}

int DSSTabularMetrics::getAvailableSubtotals(GUID iMetricID, int &oSystemSubtotal, vector<GUID> &oUserDefinedSubtotals)
{
    int pos = FindMetric(iMetricID);
    if (pos != -1)
    {
        if(pos >= mvAvailableSystemSubtotals.size())
            return E_INVALIDINDEX;
        oSystemSubtotal = mvAvailableSystemSubtotals[pos];
        oUserDefinedSubtotals = mvAvailableUserDefinedSubtotals[pos];
    }
    return S_OK;
}

int DSSTabularMetrics::ItemC(int index, DSSCubeDataSlice **oppCubeSlice)
{
    if (mvSlices.size() > index && index >= 0 && oppCubeSlice)
    {
        //TODO: not sure if it's correct. Need check with Taimin or Zhuo
        *oppCubeSlice = mvSlices[index];
    }
    else
    {
        AE_ASSERT(false);
        //old rwd or something is wrong??
    }
    return S_OK;
}
