/*
 *  DSSTabularUnit.cpp
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/30/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import "DSSTabularUnit.h"
#import "DSSSubtotalElementProxy.h"
#import "DSSDataModelBase.h"

DSSTabularUnit::DSSTabularUnit():mUnitID(-1), mGUID(GUID_NULL), mbIsRuntimeUnit(false)
,mpModel(NULL), mpObjContext(NULL),mNullElementIndex(-1), mpNullElement(NULL)
{}

DSSTabularUnit::~DSSTabularUnit()
{
	if (mpNullElement)
	{
		delete mpNullElement;
		mpNullElement = NULL;
	}
}

int DSSTabularUnit::setGUID(MBase::CString &irVal)
{
	mGUIDStr = irVal;
	int len = irVal.length();
	if (len > 0)
	{
#ifdef NO_WCHAR_SUPPORT
		MBase::String2Guid(mGUID, irVal.c_str());
#else
		vector<wchar_t> lString(len);
		mbstowcs(&lString[0], irVal.c_str(), len);
		MBase::String2Guid(mGUID, &lString[0]);
#endif
	}
	return S_OK;
}

int DSSTabularUnit::getGUID(GUID &orVal)
{
	orVal = mGUID;
	return S_OK;
}

const GUID &DSSTabularUnit::getGUID()
{
    return mGUID;
}

bool DSSTabularUnit::IsRuntimeUnit()
{
    return mbIsRuntimeUnit;
}

int DSSTabularUnit::createSpecialElement(EnumDSSElementType iElementType, int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement)
{
	switch (iElementType) {
		case DssElementSubtotal:
		{
            //hyan, 11/12/2012
            //to make subtotal index for AE total (-100) to 1
            if(key==DSSXTAB_TOTAL_KEY || key==-DSSXTAB_TOTAL_KEY)
                key=1;
            
			DSSSubtotalElementProxy *lpSubtotalElement = new DSSSubtotalElementProxy(ipObjectContext, mGUID, key);	//465639, use the local index to create total element
			if (lpSubtotalElement)
			{
				*oppElement = lpSubtotalElement;
				return S_OK;
			}
		}
		case DssElementNULL:
		case DssElementAll:
		case DssElementNode:
		default:
			return NULL;	//not implemented yet
			break;
	}
	return E_FAIL;
}

void DSSTabularUnit::setParentInfo(DSSDataModelBase *ipModel)
{
	mpModel = ipModel;
}

DSSObjectContext* DSSTabularUnit::hGetContext()
{
	if (!mpModel)
	{
		_ASSERT(false);
		return NULL;
	}
	return mpModel->getParentInfo()->getObjectContext();
}

bool DSSTabularUnit::IsRecursiveUnit()
{
    return false;
}

void DSSTabularUnit::setNullElement(int index, MBase::CString &irElementID)
{
	mNullElementIndex = index;
	mNullElementIDStr = irElementID;
}

DSSBaseElementProxy *DSSTabularUnit::hGetNullElement()
{
	if (!mpNullElement)
	{
		if (!mpObjContext)
		{
			mpObjContext = hGetContext();
			_ASSERT(mpObjContext);
		}
		mpNullElement = mpModel->CreateElementFromElementID(mNullElementIDStr);
	}
	return mpNullElement;
}

int DSSTabularUnit::size()
{
	int lSize = 0;
	
	lSize += mGUIDStr.size() * sizeof(char);
	
	lSize += mNullElementIDStr.size() * sizeof(char);
	
	lSize += DSSAEHelper::SizeOfBaseElement(mpNullElement);
	
	return lSize;
}

int DSSTabularUnit::getTabularUnit(DSSTabularUnit** oppUnit)
{
    if (!oppUnit)
        return E_POINTER;
    
    *oppUnit = this;
    
    return S_OK;
}
	

int DSSTabularUnit::get_IDI(GUID *opID)
{
    if (opID)
        return this->getGUID(*opID);
    else
        return E_INVALIDARG;
}
