/*
 *  DSSTabularConsolidation.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 5/1/10.
 *  Copyright 2010 MicroStrategy, Inc. All rights reserved.
 *
 */

#import "DSSTabularConsolidation.h"
#import "DSSTabularAttribute.h"
#import "DSSConsolidationElementProxy.h"
#import "DSSDataModelBase.h"
#import "DSSConsolidationElement.h"
#import "DSSConsolidation.h"

DSSTabularConsolidation::DSSTabularConsolidation():mbIsDerivedElement(false), mpChildUnit(NULL), mbUseGlobalLookUp(false)
{
	mUnitType = DssTemplateConsolidation;
}

DSSTabularConsolidation::~DSSTabularConsolidation()
{
	if (mpChildUnit && !mbUseGlobalLookUp)
	{
		delete mpChildUnit;
		mpChildUnit = NULL;
	}
}

bool DSSTabularConsolidation::IsDerivedElement()
{
    return mbIsDerivedElement;
}

// weilin 2016/08/18 DE41482 check nde
bool DSSTabularConsolidation::IsNewDerivedElement()
{
    DSSObjectInfo* lpObj = mpObjContext->FindObjectH(mGUID);
    if (lpObj) {
        DSSConsolidation * lpObjConsolidation = dynamic_cast<DSSConsolidation*>(lpObj);
        if (lpObjConsolidation) {
            return lpObjConsolidation->getConsolidationType() == DssConsolidationTypeDerivedElement;
        }
    }
    return false;
}

int DSSTabularConsolidation::Init(int index, int nElements)
{
	mUnitID = index;
    
	_ASSERT(mvElements.empty());
	mvElements.resize(nElements);
	return S_OK;
}

int DSSTabularConsolidation::setIsDerivedElement(bool isDE, DSSTabularUnit *ipChildUnit,  bool iUseGlobal)
{
	mbIsDerivedElement = isDE;
	mpChildUnit = reinterpret_cast<DSSTabularAttribute*> (ipChildUnit);
    mbUseGlobalLookUp = iUseGlobal;
	return S_OK;
}

int DSSTabularConsolidation::setElement(int index, XTabConsolidationElement &irElement)
{
	if (index >= mvElements.size())
	{
		int lnNewElements = index + 1 - mvElements.size();
		for (int i = 0; i < lnNewElements; i++)
		{
			mvElements.push_back(XTabConsolidationElement());
		}
	}
	
	XTabConsolidationElement *lpElement = &mvElements[index];
	
	lpElement->branch = irElement.branch;
	lpElement->childElementID = irElement.childElementID;
	lpElement->ordinal = irElement.ordinal;
	lpElement->value = irElement.value;
	lpElement->guidstr = irElement.guidstr;
	int len = irElement.guidstr.length();
	if (len > 0)
	{
#ifdef NO_WCHAR_SUPPORT
		MBase::String2Guid(lpElement->guid, irElement.guidstr.c_str());
#else
		vector<wchar_t> lString(len);
		mbstowcs(&lString[0], irElement.guidstr.c_str(), len);
		MBase::String2Guid(lpElement->guid, &lString[0]);
#endif
	}
	lpElement->pConsolidationElement = NULL;
	return S_OK;
}

int DSSTabularConsolidation::getNumAllElements()
{
    return mvElements.size();
}

int DSSTabularConsolidation::CountRows()
{
	return mvElements.size();
}

int DSSTabularConsolidation::GetMaxKey()
{
    return mvElements.size() - 1;
}

int DSSTabularConsolidation::TranslateLocalToGlobal(int iLocal)
{
    return iLocal;
}

int DSSTabularConsolidation::TranslateGlobalToLocal(int iGlobal)
{
    return iGlobal;
}

int DSSTabularConsolidation::FindElement(DSSBaseElementProxy *ipElement, int &orKey)
{
	int lnElements = mvElements.size();
	EnumDSSElementType lElementType=ipElement->getElementType();
	if (lElementType == DssElementNULL && mNullElementIndex >= 0)
	{
		orKey = mNullElementIndex;
		return S_OK;
	}
    
	if (!mbIsDerivedElement)
	{
		if (lElementType != DssElementConsolidation)
			return S_FALSE;
		//TODO: check consolidation ID
		GUID lConsolidationElementID = static_cast<DSSConsolidationElementProxy*> (ipElement)->getConsolidationElementID();
		
		for (int i = 0; i < lnElements; i++)
		{
			if (MBase::IsEqualGUID(lConsolidationElementID, mvElements[i].guid))
			{
				orKey=i;
				return S_OK;
			}
		}
	}
	else
	{
		if(lElementType==DssElementConcrete)
		{
			int lKey=0;
			int hr =mpChildUnit->FindElement(ipElement, lKey);
			CHECK_HR;
            vector<int> lvCandidateKeys;
			for(int i=0;i<lnElements;i++)
			{
				if (IsLeaf(i)) {
                    //hyan,2012/07/23
                    //add check ordinal, temporary fix about issue 628992
                    //xiazhou, 2013/09/09, TQMS 787071. hyan's fix will cause failure finding elements
                    //whose ordinal all < 0. In this situation, return the first matched elements.
					//if(mvElements[i].ordinal>=0 &&   lKey==mvElements[i].childElementID)
                    if(lKey==mvElements[i].childElementID)
					{
                        if(mvElements[i].ordinal >= 0)
                        {
                            orKey=i;
                            return S_OK;
                        }
						lvCandidateKeys.push_back(i);
					}
				}
			}
            if(lvCandidateKeys.size() > 0)
            {
                orKey = lvCandidateKeys[0];
                return S_OK;
            }
		}else if(lElementType==DssElementConsolidation)
		{
			DSSConsolidationElementProxy* lpConsolidationElement = static_cast<DSSConsolidationElementProxy*> (ipElement);
			GUID lAttributeID;
			int hr=mpChildUnit->getGUID(lAttributeID);
			CHECK_HR;
			GUID lConsolidationElementID=lpConsolidationElement->getConsolidationElementID();
			//TODO check container
			//if(!MBase::IsEqualGUID(lpConsolidationElement->getContainerID(),lContainerID)||
			if( !MBase::IsEqualGUID(lpConsolidationElement->getAttributeID(),lAttributeID))
            {
                //992254;11/19/2014; it might be a new DE which is based on consolidation unit
                if( !MBase::IsEqualGUID(lpConsolidationElement->getConsolidationID(),mGUID))
                {
                    return S_FALSE;
                }
            }
			
			
            int lCandidate = -1;
			for(int i=0;i<lnElements;i++)
			{
				if(!IsLeaf(i))
				{
					if(MBase::IsEqualGUID(lConsolidationElementID,mvElements[i].guid))
					{
                        //993028, check ordinal. If it's invisible, return it only if it's the only matched element.
                        if (mvElements[i].ordinal < 0)
                        {
                            lCandidate = i;
                            continue;
                        }
                        
						orKey=i;
						return S_OK;
					}
				}
			}
            
            if (lCandidate >= 0)
            {
                orKey = lCandidate;
                return S_OK;
            }
			
		}
	}
	return S_FALSE;
}

int DSSTabularConsolidation::createElement(int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement, int iGlobalKey,int iGlobalKeyVer)
{
	if (!oppElement)
		return E_POINTER;
	
	//412139, (key >= mvElement.size()) returns true if key is negative since it's treated as unsigned.
	if (key < 0)
		return createSpecialElement(DssElementSubtotal, -key, ipObjectContext, oppElement);
	
	int hr = S_OK;
	if (key >= mvElements.size())
		return E_INVALIDINDEX;
	
	if (!mbIsDerivedElement)
	{
		GUID lConsolidationElementID=mvElements[key].guid;
        
		DSSObjectInfo *lpConElemObj = ipObjectContext->FindObjectH(lConsolidationElementID);
		GUID lContainerID = GUID_NULL;
		if (lpConElemObj)
			lContainerID = lpConElemObj->getContainerID();
        
		*oppElement = new DSSConsolidationElementProxy(ipObjectContext, mGUID, DssTypeConsolidation, lConsolidationElementID, lContainerID);
        
	}
	else
	{
        bool lbIsNewDE = mpChildUnit && !(MBase::IsEqualGUID(mGUID, mpChildUnit->getGUID()) && !MBase::IsGUIDNULL(mGUID));

		if(IsLeaf(key))
		{//attribute element
			hr=mpChildUnit->createNewDrivedElement(mvElements[key].childElementID, ipObjectContext,
                                                   lbIsNewDE ? &mGUID : NULL,
                                                   oppElement, iGlobalKey, iGlobalKeyVer);
			CHECK_HR;
		}
		else
		{//consolidation element
			GUID lConsolidationElementID=mvElements[key].guid;
			GUID lAttributeID = mGUID;;
            if (!lbIsNewDE)
            {
                hr=mpChildUnit->getGUID(lAttributeID);
            }
			CHECK_HR;
			
			DSSObjectInfo *lpConElemObj = ipObjectContext->FindObjectH(lConsolidationElementID);
			GUID lContainerID = GUID_NULL;
			if (lpConElemObj)
				lContainerID = lpConElemObj->getContainerID();
            
            *oppElement = new DSSConsolidationElementProxy(ipObjectContext, lAttributeID,
                                                           lbIsNewDE? DssTypeConsolidation:DssTypeAttribute,
                                                           lConsolidationElementID, lContainerID);
		}
	}
    
    //junwang, 04/19/2012
    if(*oppElement)
        (*oppElement)->setGlobalKey(iGlobalKey,iGlobalKeyVer);
    
	return hr;
}

int DSSTabularConsolidation::getText(int index, MBase::CString &orDisplayName)
{
	if (index < 0 || index >= mvElements.size())
		return E_INVALIDINDEX;
	orDisplayName = mvElements[index].value;
	return S_OK;
	
}

const char * DSSTabularConsolidation::getTextAndLength(int index, int &orLength)
{
	orLength = 0;
	
	if (index < 0 || index >= mvElements.size())
		return NULL;
    
	orLength = mvElements[index].value.length();
	
	return mvElements[index].value.c_str();
}

bool DSSTabularConsolidation::IsLeaf(int index)
{
	//410614, for consolidation unit in control/fieldgroup model, branch is ignored, need check childElementID to decide if it's leaf
	if (index < mvElements.size())
		return mvElements[index].branch == 0 || mvElements[index].childElementID >= 0;
	else
		return false;
}

bool DSSTabularConsolidation::IsDisplayed(int index)
{
	if (index < mvElements.size())
		return mvElements[index].ordinal >= 0;
	else if (index == mvElements.size())    //10/8/2014; lwang; 976543; check join NULL, which is displayable
        return true;
    else
		return false;
}

DSSTabularUnit * DSSTabularConsolidation::getChildUnit()
{
	return mpChildUnit;
}

int DSSTabularConsolidation::getChildElementKey(int index)
{
	if (index < mvElements.size())
		return mvElements[index].childElementID;
	return -1;
}


int DSSTabularConsolidation::getChildElementID(int index, GUID *lpID)
{
    CHECK_PTR(lpID);
    if(index<mvElements.size())
        *lpID = mvElements[index].guid;
    else
        return S_FALSE;
    
    return S_OK;
}


XTabConsolidationElement* DSSTabularConsolidation::getElement(int index)
{
	if (index < mvElements.size())
		return &mvElements[index];
	else
		return NULL;
}

DSSConsolidationElement* DSSTabularConsolidation::getConsolidationElement(int index)
{
	if (index < 0 || index >= mvElements.size())
		return NULL;
	
	XTabConsolidationElement *lpElement = &mvElements[index];
	if (lpElement->branch != 0 && lpElement->pConsolidationElement == NULL)
	{
		//it's a branch element but its element is not assigned
		if (!mpObjContext)
		{
			mpObjContext = hGetContext();
			_ASSERT(mpObjContext);
		}
		DSSObjectInfo *lpObj = mpObjContext->FindObjectH(lpElement->guid);
		if (lpObj)
			lpElement->pConsolidationElement = reinterpret_cast<DSSConsolidationElement*> (lpObj);
	}
	return lpElement->pConsolidationElement;
}

int DSSTabularConsolidation::size()
{
	int lSize = sizeof(this);
    
	lSize += DSSTabularUnit::size();
    
	int lnElements = mvElements.size();
	
	lSize += lnElements * sizeof(XTabConsolidationElement);
	
	for (int i = 0; i < lnElements; i++)
	{
		XTabConsolidationElement *lpElement = &mvElements[i];
		lSize += lpElement->guidstr.size() * sizeof(char);
		lSize += lpElement->value.size() * sizeof(char);
		if (lpElement->pConsolidationElement)
			lSize += sizeof(DSSConsolidationElement);
	}
	
	lSize += sizeof(mbIsDerivedElement) + sizeof(mpChildUnit);
	
	if (mpChildUnit)
		lSize += mpChildUnit->size();
	
	return lSize;
}


// 3/22/2012 tehe: return true when consolidation has any element to display, else return false
bool DSSTabularConsolidation::hasElementDisplayable()
{
    bool lbEnableDisplay = false;
    //    if(!mbIsDerivedElement)
    //    {
    for(int i=0;i<mvElements.size();i++)
    {
        XTabConsolidationElement csElement = mvElements[i];
        if(csElement.ordinal >= 0)
        {
            lbEnableDisplay = true;
            break;
        }
    }
    
    //    }
    //    else
    //    {
    //        DSSGlobalLookUpTable *lpGlobalLookup = dynamic_cast<DSSGlobalLookUpTable*>(mpChildUnit);
    //        lbEnableDisplay = lpGlobalLookup->IsEnableDisplay();
    //    }
    return lbEnableDisplay;
}

bool DSSTabularConsolidation::compareElement(DSSBaseElementProxy *ipElement, int iKey)
{
    int lnElements = mvElements.size();
	EnumDSSElementType lElementType = ipElement->getElementType();
    
	if (lElementType == DssElementNULL && mNullElementIndex >= 0 &&iKey == mNullElementIndex )
	{
        return true;
	}
    
    if(iKey<0||iKey>=lnElements)
        return false;
    
	if (!mbIsDerivedElement)
	{
		if (lElementType != DssElementConsolidation)
			return false;
        
		GUID lConsolidationElementID = static_cast<DSSConsolidationElementProxy*> (ipElement)->getConsolidationElementID();
		
        if(MBase::IsEqualGUID(lConsolidationElementID, mvElements[iKey].guid))
            return true;
        else
            return false;
	}
	else
	{
		if(lElementType==DssElementConcrete)
		{
            if(IsLeaf(iKey))
            {
                int lKey = mvElements[iKey].childElementID;
                
                return mpChildUnit->compareElement(ipElement, lKey);
            }
            else
                return false;
		}else if(lElementType==DssElementConsolidation)
		{
			DSSConsolidationElementProxy* lpConsolidationElement = static_cast<DSSConsolidationElementProxy*> (ipElement);
			GUID lAttributeID;
			int hr=mpChildUnit->getGUID(lAttributeID);
			CHECK_HR;
            
			GUID lConsolidationElementID=lpConsolidationElement->getConsolidationElementID();
			//TODO check container
			//if(!MBase::IsEqualGUID(lpConsolidationElement->getContainerID(),lContainerID)||
			if( !MBase::IsEqualGUID(lpConsolidationElement->getAttributeID(),lAttributeID))
            {
                //992254;11/19/2014; it might be a new DE which is based on consolidation unit
                if( !MBase::IsEqualGUID(lpConsolidationElement->getConsolidationID(),mGUID))
                {
                    return false;
                }
            }
            
            if(!IsLeaf(iKey))
            {
                if(MBase::IsEqualGUID(lConsolidationElementID,mvElements[iKey].guid))
                {
                    return true;
                }
            }
		}
	}
    
    return false;
}

void DSSTabularConsolidation::dump()
{
    std::cout<<"<DSSTabularConsolidation>\n";
    
    std::cout<<"IsDE: "<<(mbIsDerivedElement?"True":"False")<<"\n";
    std::cout<<"UseGlobalLookUp: "<<(mbUseGlobalLookUp?"True":"False")<<"\n";
    std::cout<<"ElementCount: "<<mvElements.size()<<"\n";
    for(int i=0; i<(int)mvElements.size();i++)
    {
        std::cout<<"Element "<<i<<"\n";
        std::cout<<"  branch: "<<mvElements[i].branch<<"\n";
        std::cout<<"  ordinal: "<<mvElements[i].ordinal<<"\n";
        std::cout<<"  value: "<<mvElements[i].value<<"\n";
        std::cout<<"  childElementID: "<<mvElements[i].childElementID<<"\n";
        std::cout<<"  GUID: "<<mvElements[i].guidstr<<"\n";
        std::cout<<"  leaves: ";
        for (int j=0; j<(int)mvElements[i].mvLeaves.size(); j++) {
            std::cout<<mvElements[i].mvLeaves[j]<<"  ";
        }
        std::cout<<"\n";
    }
    
    std::cout<<"<DSSTabularConsolidation/>\n";
}

int DSSTabularConsolidation::getOrdinal(int index, int *opOrdinal)
{
    *opOrdinal = mvElements[index].ordinal;
    return S_OK;
}

int DSSTabularConsolidation::GetLeavesByElement (Int32 Element, bool *opfBranch, vector<int> *opvLeaves)
{
	if(Element < 0 || Element >= mvElements.size())
		return E_INVALIDARG;
    
	CHECK_PTR(opfBranch);
	CHECK_PTR(opvLeaves);
    
	XTabConsolidationElement* lpElement = &(mvElements[Element]);
    
	*opfBranch = lpElement -> branch;
	*opvLeaves = lpElement -> mvLeaves;
	return S_OK;
}

int DSSTabularConsolidation::Item(int index, int iChild, VARIANT_BOOL *opIsBranch, int *opChildKey)
{
    XTabConsolidationElement *lpElement = getElement(index);
    if (lpElement && opIsBranch && opChildKey)
    {
        *opIsBranch = lpElement->branch != 0 ? VARIANT_TRUE : VARIANT_FALSE;
        *opChildKey = lpElement->childElementID;
    }
    return S_OK;
}

int DSSTabularConsolidation::GetDuplicateElements(int index, int *opElementsCount, int **oppElements)
{
    if (! opElementsCount || *oppElements)
        return E_INVALIDARG;

    //not implemented yet, need server to serialize this information
    *opElementsCount = 0;
    *oppElements = NULL;
    return S_OK;
}

