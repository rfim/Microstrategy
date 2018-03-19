/*
 *  DSSSubExpressionElementProxy.cpp
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-5-1.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <sstream>
#include <stdlib.h>
#include "DSSSubExpressionElementProxy.h"

DSSSubExpressionElementProxy::DSSSubExpressionElementProxy()
{
	mElementID = "";
}

DSSSubExpressionElementProxy::~DSSSubExpressionElementProxy()
{}

DSSSubExpressionElementProxy::DSSSubExpressionElementProxy(DSSObjectContext* ipObjectContext) : DSSBaseElementProxy(ipObjectContext)
{
	mElementID = "";
}

DSSSubExpressionElementProxy::DSSSubExpressionElementProxy(DSSObjectContext* ipObjectContext, GUID iObjectID, int iNodeID): DSSBaseElementProxy(ipObjectContext)
{
	mElementType = DssElementSubexpression;
	mUnitType = DssTemplateCustomGroup;
	mObjectID = iObjectID;
	mNodeID = iNodeID;
		
	mElementID = "NE:";
	
	char lIDString[33];
	DSSBaseElementProxy::ConvertGUIDToString(mObjectID, lIDString);
	mElementID += lIDString;
	
	mElementID += ":";
	
	std::ostringstream oss1;
	oss1 << mNodeID;
	mElementID += oss1.str();
}

int DSSSubExpressionElementProxy::Parse(std::string iElementID)
{
	DSSBaseElementProxy::Parse(iElementID);
	
	std::vector<std::string> lTokens = hParseString(iElementID);
	
	DSSBaseElementProxy::ConvertFromStringToGUID(lTokens[1], mObjectID);
	
	mNodeID = atoi(lTokens[2].c_str());
	
	return S_OK;
}

bool DSSSubExpressionElementProxy::IsEqual(DSSBaseElementProxy* ipElement)
{
	if (!DSSBaseElementProxy::IsEqual(ipElement))
		return false;
	
	if (mNodeID != ((DSSSubExpressionElementProxy*)(ipElement))->mNodeID)
		return false;
	
	return true;
}

void DSSSubExpressionElementProxy::Clone(DSSBaseElementProxy* ipElement)
{
	DSSBaseElementProxy::Clone(ipElement);
	mNodeID = ((DSSSubExpressionElementProxy*)(ipElement))->mNodeID;
}

DSSCustomGroup* DSSSubExpressionElementProxy::getCustomGroup()
{
	return (DSSCustomGroup*)(getObjectContext()->FindObjectH(mObjectID));
}

int DSSSubExpressionElementProxy::getNodeID()
{
	return mNodeID;
}
