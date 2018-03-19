/*
 *  DSSJointConcreteElementProxy.cpp
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-6-16.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdlib.h>
#include "DSSJointConcreteElementProxy.h"

DSSJointConcreteElementProxy::DSSJointConcreteElementProxy()
{}

DSSJointConcreteElementProxy::~DSSJointConcreteElementProxy()
{
	std::vector<DSSAttributeElementProxy*>::iterator it;
	
	// delete content:
	for ( it=mAttributeElements.begin() ; it != mAttributeElements.end(); it++ )
		delete (*it);
}

DSSJointConcreteElementProxy::DSSJointConcreteElementProxy(DSSObjectContext* ipObjectContext) : DSSBaseElementProxy(ipObjectContext)
{}

int DSSJointConcreteElementProxy::Parse(std::string iElementID)
{
	int hr = S_OK;
	
	DSSBaseElementProxy::Parse(iElementID);
	
	std::vector<std::string> lTokens = hParseString(iElementID);
	
	DSSBaseElementProxy::ConvertFromStringToGUID(lTokens[1], mObjectID);
	
	mNodeID = atoi(lTokens[2].c_str());
	
	mBandNumber = atoi(lTokens[3].c_str());
	
	int lTokenCount = lTokens.size();
	int lStartTokenIndex = 4;
	for (int i=5; i<lTokenCount; i++)
	{
		if (lTokens[i].length()==32)
		{
			// assemble from Token lStartTokenIndex to Token i-1
			std::string lAttributeElementID = "BB:" + lTokens[lStartTokenIndex];
			for (int j=lStartTokenIndex+1; j<i; j++)
			{
				lAttributeElementID += ":";
				lAttributeElementID += lTokens[j];
			}
			
			DSSAttributeElementProxy* lpAttributeElement = new DSSAttributeElementProxy(getObjectContext());
			hr = lpAttributeElement->Parse(lAttributeElementID);
			if (hr!=S_OK)
				return hr;
			mAttributeElements.push_back(lpAttributeElement);
			
			lStartTokenIndex = i;
		}
	}
	
	// assemble last one
	std::string lAttributeElementID = "BB:" + lTokens[lStartTokenIndex];
	for (int j=lStartTokenIndex+1; j<lTokenCount; j++)
	{
		lAttributeElementID += ":";
		lAttributeElementID += lTokens[j];
	}
	
	DSSAttributeElementProxy* lpAttributeElement = new DSSAttributeElementProxy(getObjectContext());
	hr = lpAttributeElement->Parse(lAttributeElementID);
	if (hr!=S_OK)
		return hr;
	mAttributeElements.push_back(lpAttributeElement);
	
	return S_OK;
}

bool DSSJointConcreteElementProxy::IsEqual(DSSBaseElementProxy* ipElement)
{
	if (!DSSBaseElementProxy::IsEqual(ipElement))
		return false;
	
	return (mElementID==ipElement->getElementID());
}

std::vector<DSSAttributeElementProxy*>* DSSJointConcreteElementProxy::getAttributeElements()
{
	return &mAttributeElements;
}

void DSSJointConcreteElementProxy::Clone(DSSBaseElementProxy* ipElement)
{
	DSSBaseElementProxy::Clone(ipElement);
	mNodeID = ((DSSJointConcreteElementProxy*)(ipElement))->mNodeID;
	mBandNumber = ((DSSJointConcreteElementProxy*)(ipElement))->mBandNumber;
	
	std::vector<DSSAttributeElementProxy*>::iterator it;
	for ( it=mAttributeElements.begin() ; it != mAttributeElements.end(); it++ )
		delete (*it);
	
	mAttributeElements.clear();
	
	for ( it=((DSSJointConcreteElementProxy*)(ipElement))->mAttributeElements.begin() ; it != ((DSSJointConcreteElementProxy*)(ipElement))->mAttributeElements.end(); it++ )
	{
		DSSAttributeElementProxy* lpAttributeElement = new DSSAttributeElementProxy(getObjectContext());
		lpAttributeElement->Clone(*it);
		mAttributeElements.push_back(lpAttributeElement);
	}
}

int DSSJointConcreteElementProxy::getNodeID()
{
	return mNodeID;
}

int DSSJointConcreteElementProxy::getBandNumber()
{
	return mBandNumber;
}

DSSCustomGroup* DSSJointConcreteElementProxy::getCustomGroup()
{
	return (DSSCustomGroup*)(getObjectContext()->FindObjectH(mObjectID));
}
