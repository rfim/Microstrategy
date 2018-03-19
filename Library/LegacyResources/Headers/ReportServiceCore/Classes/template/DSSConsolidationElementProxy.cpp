/*
 *  DSSConsolidationElementProxy.cpp
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-30.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSConsolidationElementProxy.h"
#include <sstream>

DSSConsolidationElementProxy::DSSConsolidationElementProxy()
{
	mElementID = "";
	mConsolidationElementID = GUID_NULL;
	mContainerID = GUID_NULL;
}

DSSConsolidationElementProxy::~DSSConsolidationElementProxy()
{}

DSSConsolidationElementProxy::DSSConsolidationElementProxy(DSSObjectContext* ipObjectContext) : DSSBaseElementProxy(ipObjectContext)
{
	mElementID = "";
	mConsolidationElementID = GUID_NULL;
	mContainerID = GUID_NULL;
}

DSSConsolidationElementProxy::	DSSConsolidationElementProxy(DSSObjectContext* ipObjectContext, GUID iUnitObjectID, EnumDSSObjectType iUnitObjectType, GUID iConsolidationElementID, GUID iContainerID)
{
	mElementType = DssElementConsolidation;
	
	mObjectID = iUnitObjectID;

	mConsolidationElementID = iConsolidationElementID;
	mContainerID = iContainerID;

	// determine whether unit type is template consolidation or template attribute
	if ((iUnitObjectType == DssTypeConsolidation) ||
		(iUnitObjectType == DssTypeAttribute))
	{
		if (iUnitObjectType == DssTypeConsolidation)
		{
			mUnitType = DssTemplateConsolidation;
			mElementID = "HF:";
		}
		else 
		{
			mUnitType = DssTemplateAttribute;
			mElementID = "HB:";
		}

		char lIDString[33];
		DSSBaseElementProxy::ConvertGUIDToString(mObjectID, lIDString);
		mElementID += lIDString;
		
		mElementID += ":";
		
		DSSBaseElementProxy::ConvertGUIDToString(mConsolidationElementID, lIDString);
		mElementID += lIDString;

		if (mContainerID != GUID_NULL)
		{
			mElementID += ":";
			DSSBaseElementProxy::ConvertGUIDToString(mContainerID, lIDString);
			mElementID += lIDString;

			EnumDSSObjectType lContainerType = DssTypeReserved;
			DSSObjectInfo *lpContainerObj = ipObjectContext->FindObjectH(mContainerID);
			if (lpContainerObj)
				lContainerType = lpContainerObj->getType();

			mElementID += ":";
			std::ostringstream lTypeString;
			lTypeString << lContainerType;
			mElementID += lTypeString.str();
		}
	}
}

int DSSConsolidationElementProxy::Parse(std::string iElementID)
{
	DSSBaseElementProxy::Parse(iElementID);
	
	std::vector<std::string> lTokens = hParseString(iElementID);
	
	DSSBaseElementProxy::ConvertFromStringToGUID(lTokens[1], mObjectID);
	
	DSSBaseElementProxy::ConvertFromStringToGUID(lTokens[2], mConsolidationElementID);
	
	if (lTokens.size()>=4)
		DSSBaseElementProxy::ConvertFromStringToGUID(lTokens[3], mContainerID);

	return S_OK;
}

bool DSSConsolidationElementProxy::IsEqual(DSSBaseElementProxy* ipElement)
{
	if (!DSSBaseElementProxy::IsEqual(ipElement))
		return false;
	
	DSSConsolidationElementProxy* lpElement = (DSSConsolidationElementProxy*)(ipElement);
	
	if (!IsEqualGUID(mConsolidationElementID, lpElement->getConsolidationElementID()))
		return false;
					
	return IsEqualGUID(mContainerID, lpElement->getContainerID());
}
	
GUID DSSConsolidationElementProxy::getConsolidationID()
{
	if (mUnitType == DssTemplateConsolidation)
		return mObjectID;
	else
		return GUID_NULL;
}

GUID DSSConsolidationElementProxy::getConsolidationElementID()
{
	return mConsolidationElementID;
}

GUID DSSConsolidationElementProxy::getAttributeID()
{
	if (mUnitType == DssTemplateAttribute)
		return mObjectID;
	else
		return GUID_NULL;
}

GUID DSSConsolidationElementProxy::getContainerID()
{
	return mContainerID;
}

void DSSConsolidationElementProxy::Clone(DSSBaseElementProxy* ipElement)
{
	DSSBaseElementProxy::Clone(ipElement);
	mConsolidationElementID = ((DSSConsolidationElementProxy*)(ipElement))->mConsolidationElementID;
	mContainerID = ((DSSConsolidationElementProxy*)(ipElement))->mContainerID;
}

DSSConsolidationElement* DSSConsolidationElementProxy::getConsolidationElement()
{
	return (DSSConsolidationElement*)(getObjectContext()->FindObjectH(mConsolidationElementID));
}
