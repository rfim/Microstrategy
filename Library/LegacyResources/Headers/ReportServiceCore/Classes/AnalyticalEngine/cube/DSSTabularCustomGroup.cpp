/*
 *  DSSTabularCustomGroup.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 5/1/10.
 *  Copyright 2010 MicroStrategy, Inc. All rights reserved.
 *
 */

#import "DSSTabularCustomGroup.h"
#import "DSSDataModelBase.h"
#import "DSSBaseElementProxy.h"
#import "DSSConcreteElementProxy.h"
#import "DSSBandElementProxy.h"
#import "DSSSubExpressionElementProxy.h"
#import "DSSFilterElementProxy.h"
#import "DSSJointConcreteElementProxy.h"
#import "DSSNullElementProxy.h"

DSSTabularCustomGroup::DSSTabularCustomGroup():maxDepth(-1)
{
	mUnitType = DssTemplateCustomGroup;
	mpObjContext = NULL;
}

DSSTabularCustomGroup::~DSSTabularCustomGroup()
{
	for(int i=0;i<mvpElements.size();i++)
	{
		if (mvpElements[i])
		{
			delete mvpElements[i];
			mvpElements[i]=NULL;
		}
	}
}

int DSSTabularCustomGroup::Init(int index, int nElements)
{
	mUnitID = index;
	
	_ASSERT(mvElements.empty());
    //	mvElements.resize(nElements);
	mvpElements.resize(nElements);
	for(int i = 0; i < nElements; i++)
	{
		mvElements.push_back(XTabCustomGroupElement());
	}
	return 0;
}

int DSSTabularCustomGroup::setElement(int index, XTabCustomGroupElement &irElement)
{
	if (index >= mvElements.size())
	{
		int lnNewElements = index + 1 - mvElements.size();
		for (int i = 0; i < lnNewElements; i++)
		{
			mvElements.push_back(XTabCustomGroupElement());
			mvpElements.push_back(NULL);
		}
	}
	
	XTabCustomGroupElement *lpElement = &mvElements[index];
    
	lpElement->depth = irElement.depth;
	lpElement->mType = irElement.mType;
	lpElement->nodeID = irElement.nodeID;
	lpElement->ordinal = irElement.ordinal;
	lpElement->displayName = irElement.displayName;
	lpElement->elementID = irElement.elementID;
	
	
	int parent = irElement.parent;
	int lnElements = mvElements.size();
	AE_ASSERT (parent < lnElements);
	lpElement->parent = parent;
    
	lpElement->isLoaded = true;
	
	return S_OK;
}

int DSSTabularCustomGroup::getMaxDepth()
{
	if (maxDepth == -1)
	{
		vector<XTabCustomGroupElement>::iterator it = mvElements.begin();
		vector<XTabCustomGroupElement>::iterator end = mvElements.end();
		for (; it < end; it++)
		{
			if (it->isLoaded == true && it->depth > maxDepth)
                //			if (it->depth > maxDepth)
				maxDepth = it->depth;
		}
	}
	
	return maxDepth;
}

int DSSTabularCustomGroup::getParentKeys(int index, vector<int> &orParentKeys)
{
	if (index < 0 || index >= mvElements.size())
		return E_INVALIDINDEX;
	
	int parent = mvElements[index].parent;
	while (parent >= 0)
	{
		orParentKeys.push_back(parent);
		parent = mvElements[parent].parent;
	}
	return S_OK;
}

int DSSTabularCustomGroup::CountRows()
{
	return mvElements.size();
}

int DSSTabularCustomGroup::FindElement(DSSBaseElementProxy *ipElement, int &orKey)
{
	EnumDSSElementType lType = ipElement->getElementType();
	
	if (lType == DssElementSubtotal)
		return S_FALSE;
	else if (lType == DssElementNULL && mNullElementIndex >= 0)
	{
		orKey = mNullElementIndex;
		return S_OK;
	}
    
	for(int i=0;i<mvpElements.size();i++)
	{
		if(!mvpElements[i])
		{
			//skip unloaded elements
			if(mvElements[i].isLoaded == false)
				continue;
			//create
			int hr = hCreateElement(i);
			if (hr != S_OK && hr != S_FALSE)
				return hr;
            
			if (hr == S_FALSE)	//may be the invisible root node
				continue;
		}
		if(mvpElements[i]->IsEqual(ipElement))
		{
			orKey=i;
			return S_OK;
		}
		
	}
	return S_FALSE;
}

int DSSTabularCustomGroup::createElement(int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement, int iGlobalKey, int iGlobalKeyVer)
{
	if (!oppElement)
	{
		return E_POINTER;
	}
	
	if (key < 0)
		return createSpecialElement(DssElementSubtotal, -key, ipObjectContext, oppElement);
	
	if(key<0||key>=mvElements.size())
		return E_INVALIDINDEX;
	MBase::CString lElementID=mvElements[key].elementID;
	EnumDSSElementType lType = DSSBaseElementProxy::ParseElementType(lElementID);
	DSSBaseElementProxy **lppElement=&mvpElements[key];
	switch (lType) {
		case DssElementConcrete:
		{	DSSConcreteElementProxy *lpConcreteElement = new DSSConcreteElementProxy(ipObjectContext);
			if(!(*lppElement))
			{
				(*lppElement)=new DSSConcreteElementProxy(ipObjectContext);
				(static_cast<DSSConcreteElementProxy *>(*lppElement))->Parse(lElementID);
			}
			lpConcreteElement->Clone((*lppElement));
			*oppElement=lpConcreteElement;
		}
			break;
		case DssElementSubexpression:
		{	DSSSubExpressionElementProxy *lpSExpElement = new DSSSubExpressionElementProxy(ipObjectContext);
			if(!(*lppElement))
			{
				(*lppElement)=new DSSSubExpressionElementProxy(ipObjectContext);
				(static_cast<DSSSubExpressionElementProxy *>(*lppElement))->Parse(lElementID);
			}
			lpSExpElement->Clone((*lppElement));
			*oppElement=lpSExpElement;
		}
			break;
		case DssElementFilter:
		{	DSSFilterElementProxy *lpFilterElement = new DSSFilterElementProxy(ipObjectContext);
			if(!(*lppElement))
			{
				(*lppElement)=new DSSFilterElementProxy(ipObjectContext);
				(static_cast<DSSFilterElementProxy *>(*lppElement))->Parse(lElementID);
			}
			lpFilterElement->Clone((*lppElement));
			*oppElement=lpFilterElement;
		}
			break;
		case DssElementBand:
		{	DSSBandElementProxy *lpBandElement = new DSSBandElementProxy(ipObjectContext);
			if(!(*lppElement))
			{
				(*lppElement)=new DSSBandElementProxy(ipObjectContext);
				(static_cast<DSSBandElementProxy *>(*lppElement))->Parse(lElementID);
			}
			lpBandElement->Clone((*lppElement));
			*oppElement=lpBandElement;
		}
			break;
		case DssElementJoint:
		{	DSSJointConcreteElementProxy *lpJointConcreteElement = new DSSJointConcreteElementProxy(ipObjectContext);
			if(!(*lppElement))
			{
				(*lppElement)=new DSSJointConcreteElementProxy(ipObjectContext);
				(static_cast<DSSJointConcreteElementProxy *>(*lppElement))->Parse(lElementID);
			}
			lpJointConcreteElement->Clone((*lppElement));
			*oppElement=lpJointConcreteElement;
		}
			break;
		default:
			return E_INVALIDARG;
	}
    
    //junwang, 04/19/2012
    if(*oppElement)
        (*oppElement)->setGlobalKey(iGlobalKey, iGlobalKeyVer);
	return S_OK;
	
}

int DSSTabularCustomGroup::getText(int index, MBase::CString &orDisplayName)
{
	if (index < 0 || index >= mvElements.size())
		return E_INVALIDINDEX;
	orDisplayName = mvElements[index].displayName;
	return S_OK;
	
}

int DSSTabularCustomGroup::getOrdinal(int index)
{
    if (index < 0 || index >= mvElements.size())
        return -2;
    
    return mvElements[index].ordinal;
}

const char * DSSTabularCustomGroup::getTextAndLength(int index, int &orLength)
{
	orLength = 0;
	
	if (index < 0 || index >= mvElements.size())
		return NULL;
    
	orLength = mvElements[index].displayName.length();
	
	return mvElements[index].displayName.c_str();
}

XTabCustomGroupElement * DSSTabularCustomGroup::getElement(int index)
{
	if (index < mvElements.size())
		return &mvElements[index];
	else
		return NULL;
}

DSSBaseElementProxy * DSSTabularCustomGroup::getCustomGroupElement(int index)
{
	if (index < 0 || index >= mvpElements.size())
		return NULL;
	
	if (!mvpElements[index])
	{
		int hr = hCreateElement(index);
		if (hr != S_OK)
			return NULL;
	}
	return mvpElements[index];
}

int DSSTabularCustomGroup::hCreateElement(int index)
{
	DSSBaseElementProxy **lppElement=&mvpElements[index];
	MBase::CString lElementID=mvElements[index].elementID;
	if (!mpObjContext)
	{
		mpObjContext = hGetContext();
		CHECK_PTR(mpObjContext);
	}
	EnumDSSElementType lType = DSSBaseElementProxy::ParseElementType(lElementID);
	switch (lType) {
		case DssElementConcrete:
		{	DSSConcreteElementProxy *lpConcreteElement = new DSSConcreteElementProxy(mpObjContext);
			lpConcreteElement->Parse(lElementID);
			*lppElement=lpConcreteElement;
		}
			break;
		case DssElementSubexpression:
		{	DSSSubExpressionElementProxy *lpSExpElement = new DSSSubExpressionElementProxy(mpObjContext);
			lpSExpElement->Parse(lElementID);
			*lppElement=lpSExpElement;
		}
			break;
		case DssElementFilter:
		{	DSSFilterElementProxy *lpFilterElement = new DSSFilterElementProxy(mpObjContext);
			lpFilterElement->Parse(lElementID);
			*lppElement=lpFilterElement;
		}
			break;
		case DssElementBand:
		{	DSSBandElementProxy *lpBandElement = new DSSBandElementProxy(mpObjContext);
			lpBandElement->Parse(lElementID);
			*lppElement=lpBandElement;
		}
			break;
		case DssElementJoint:
		{	DSSJointConcreteElementProxy *lpJointConcreteElement = new DSSJointConcreteElementProxy(mpObjContext);
			lpJointConcreteElement->Parse(lElementID);
			*lppElement=lpJointConcreteElement;
		}
			break;
		default:
			return S_FALSE;
	}
	return S_OK;
}

int DSSTabularCustomGroup::size()
{
	int lSize = sizeof(this);
	
	lSize += DSSTabularUnit::size();
	
	int lnElements = mvElements.size();
	
	lSize += lnElements * sizeof(XTabCustomGroupElement);
	
	int i = 0;
	for (i = 0; i < lnElements; i++)
	{
		XTabCustomGroupElement *lpElement = &mvElements[i];
		lSize += lpElement->displayName.size() * sizeof(char);
		lSize += lpElement->elementID.size() * sizeof(char);
	}
    
	lnElements = mvpElements.size();
	lSize += lnElements * sizeof(DSSBaseElementProxy*);
	
	for (i = 0; i < lnElements; i++)
	{
		lSize += DSSAEHelper::SizeOfBaseElement(mvpElements[i]);
	}
	return lSize;
}

int DSSTabularCustomGroup::getParentKey(int childKey, int *parent)
{
    *parent = getElement(childKey)->parent;
    return S_OK;
}

bool DSSTabularCustomGroup::compareElement(DSSBaseElementProxy *ipElement, int iKey)
{
    EnumDSSElementType lType = ipElement->getElementType();
	
	if (lType == DssElementSubtotal)
		return false;
	else if (lType == DssElementNULL && mNullElementIndex >= 0&&iKey==mNullElementIndex)
	{
        return true;
	}
    
    if(iKey<0||iKey>=mvpElements.size())
        return false;
    
    if(!mvpElements[iKey])
    {
        //skip unloaded elements
        if(mvElements[iKey].isLoaded == false)
            return false;
        //create
        int hr = hCreateElement(iKey);
        if (hr != S_OK && hr != S_FALSE)
            return false;
        
        if (hr == S_FALSE)	//may be the invisible root node
            return false;
    }
    
    if (mvpElements[iKey]->IsEqual(ipElement))
    {
        return true;
    }
    
    return false;
}
