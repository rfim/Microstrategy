/*
 *  DSSDataModelGroupbyImpl.cpp
 *  ReportServiceCore
 *
 *  Created by xiaoyan yu on 4/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import "DSSDataModelGroupbyImpl.h"
#import "DSSObjectContext.h"
#import "DSSObjectInfo.h"
#import "DSSTabularUnit.h"
#import "DSSAllElementProxy.h"
#import "DSSXTabBitVector.h"

void GroupbyElement::generateAllElementList(int curLev, int totLev)
{
	if(curLev >= totLev - 1)
		return;

    if(index != -1 && (children.size() == 0 || children[0].index != -1))
        return;
    /*
    set<int> lvChildren;
    for(int i = 1; i < children.size(); i++)
    {
        GroupbyElement* lpCur = &(children[i]);
        if(lpCur)
        {
            for(int j = 0; j < lpCur->children.size(); j++)
            {
                if(lvChildren.find(lpCur->children[j].index) == lvChildren.end())
                {
                    children[0].children.push_back(lpCur->children[j]);
                    lvChildren.insert(lpCur->children[j].index);
                }
            }
        }
    }
	*/			

	if(index == -1 && children.size() == 0)
	{
		GroupbyElement lAllElement;
		lAllElement.index = -1;
		lAllElement.children.resize(0);	
		children.push_back(lAllElement);
	}
	if(children[0].children.size() == 0)
	{
		GroupbyElement lAllElement;
        lAllElement.index = -1;
        lAllElement.children.resize(0);				
		children[0].children.push_back(lAllElement);
	}
    for(int i = 0; i < children.size(); i++)
        children[i].generateAllElementList(curLev + 1, totLev);
}

int GroupbyElement::getLeaveCount(vector<GroupbyElement*> *opvGroupbyElements)
{
    if(children.size() == 0 && index != -1)
    {
        opvGroupbyElements->push_back(this);
        return 1;
    }
    if (index == -1)
        return 0;
    int lCount = 0;
    for(int i = 0; i < children.size(); i++)
    {
        GroupbyElement* lpCur = &(children[i]);
        if(lpCur)
        {
            lCount += lpCur->getLeaveCount(opvGroupbyElements);
        }
    }
    
    return lCount;
}

void printGroupbyElement(GroupbyElement * ipElement, int level)
{
	if (ipElement!=NULL){
		for (int i=0; i<level; i++) {
			printf("\t");
		}
		int index=ipElement->index;
		printf("index:%d ",index);
		int n=ipElement->children.size();
		printf("size:%d\n",n);
		if (n>0) {
			for (int i=0; i<n; i++) {
				//printf("%dth child\n",i);
				printGroupbyElement(&(ipElement->children[i]),level+1);
			}
		}
	}
}

int SizeOfGroupbyElement(GroupbyElement ipElement)
{
	int lSize = 0;

	int lnChildrent = ipElement.children.size();
	lSize += sizeof(int) * (1 + lnChildrent);
	for (int i = 0; i < lnChildrent; i++)
	{
		lSize += SizeOfGroupbyElement(ipElement.children[i]);
	}
	
	return lSize;
}

int DSSDataModelGroupbyImpl::GetElementIndexList(GroupbyElement * ipElement, int unit, bool iRemoveAll, vector<int> &orvIndexes, bool iUseMask)
{
	int hr=S_OK;
	if (unit<0||unit>=mvUnits.size()) {
		return hr;
	}
	if (ipElement!=NULL){
		int n=ipElement->children.size();
		if (n>0) {
			orvIndexes.clear();
			orvIndexes.reserve(n);

			for (int i=0; i<n; i++) {
				int index=ipElement->children[i].index;
				if(index<0&&-index<=mvSpecialElements.size()){
					
					if(iRemoveAll&&(-index-1)==mlAllElementOffset)//equal
						continue;
					
				}
                if (!iUseMask || (iUseMask && ipElement->children[i].flag != 0))
                    orvIndexes.push_back(index);
			}
		}
	}
	return hr;
}
void DSSDataModelGroupbyImpl::toString()
{
	printf("special elements\n");
	for(int i=0;i<mvSpecialElements.size();i++)
	 printf ("%s + %s \n", mvSpecialElements[i].displayText.c_str(),(mvSpecialElements[i].element->getElementID()).c_str() );
	printf("texts");
	for (int i=0; i<mvDisplayTexts.size(); i++) {
		printf("\n%dth unit\n", i);
		for (int j=0; j<mvDisplayTexts[i].size(); j++) {
			printf("%s ",mvDisplayTexts[i][j].c_str());
		}
	}
	printf("\ngroupby elements\n");
	printGroupbyElement(mpRootElement,0);
	
}
DSSDataModelGroupbyImpl::DSSDataModelGroupbyImpl():mlAllElementOffset(-1),mpAllElement(NULL),mpRootElement(NULL)
{
	mType=DssModelGroupby;
}
DSSDataModelGroupbyImpl::~DSSDataModelGroupbyImpl()
{
	if(mpRootElement){
		delete mpRootElement;
		mpRootElement=NULL;
	}
	for(int i=0;i<mvSpecialElements.size();i++){
		delete mvSpecialElements[i].element;
		mvSpecialElements[i].element=NULL;
	}
	
	if (mpAllElement)
	{
		delete mpAllElement;
		mpAllElement = NULL;
	}
	
	for(int i=0;i<mvUnitElements.size();i++)
	{
		for(int j=0;j<mvUnitElements[i].size();j++)
		{
			DSSBaseElementProxy *lpElement=mvUnitElements[i][j];
			if (lpElement) {
				delete lpElement;
				mvUnitElements[i][j]=NULL;
				lpElement=NULL;
			}
			
		}
	}
}
void DSSDataModelGroupbyImpl::SetRootElement(GroupbyElement *ipRootElement)
{	
	if(!mpRootElement)
		mpRootElement=ipRootElement;
}
int DSSDataModelGroupbyImpl::SetTexts(int iOffset,vector<MBase::CString> &irvDisplayTexts)
{
	int n=mvUnits.size();
	if(iOffset>=n || iOffset<0)
		return E_INVALIDINDEX;
	
	if (mvDisplayTexts.size()!=n) {
		mvDisplayTexts.resize(n);
	}
	mvDisplayTexts[iOffset]=irvDisplayTexts;
	return S_OK;
}
void DSSDataModelGroupbyImpl::SetSpecialCount(int n)
{   
	if (n>0) {
		mvSpecialElements.resize(n);
	}
}
int DSSDataModelGroupbyImpl::SetSpeical(int iOffset, MBase::CString &irDisplayText, MBase::CString &irElementID)
{	int n=mvSpecialElements.size(); 
	if(iOffset>=n || iOffset<0)
		return E_INVALIDINDEX;

	DSSBaseElementProxy *lpElement=CreateElementFromElementID(irElementID);
	CHECK_PTR(lpElement);
	mvSpecialElements[iOffset].displayText=irDisplayText;
	mvSpecialElements[iOffset].element=lpElement;

	DSSAllElementProxy lAllElement(mpObjContext);
	lAllElement.Parse("OA:(All)");
	if(lAllElement.IsEqual(lpElement))//equal
	   mlAllElementOffset=iOffset;
	return S_OK;
}
int DSSDataModelGroupbyImpl::GetText(int index, int unit, MBase::CString &orText)
{
	int n=mvUnits.size();
	if(unit>=n ||unit<0)
		return E_INVALIDINDEX;
	n=mvDisplayTexts[unit].size();
	if( index >= n || (index<0&&-index>mvSpecialElements.size()))
		return E_INVALIDINDEX;
	
	if(index<0)
		orText=mvSpecialElements[-index-1].displayText;
	else 
		orText=mvDisplayTexts[unit][index];
	return S_OK;
}
int DSSDataModelGroupbyImpl::GetElements(vector<int> &irvIndexes, int unit, vector<DSSBaseElementProxy *> &orvElements)
{
	int hr=S_OK;
	int n=mvUnits.size();
	if (unit<0||unit>=n) {
		return E_INVALIDINDEX;
	}
	if (mvUnitElements.size()!=n) {
		mvUnitElements.resize(n);
	}
	
	n=irvIndexes.size();
	if (n>0) {
		orvElements.clear();
		orvElements.reserve(n);
		
		for (int i=0; i<n; i++) {
			int index=irvIndexes[i];
			DSSBaseElementProxy *lpElement = NULL;
			if(index<0&&-index<=mvSpecialElements.size()){
					
				lpElement=mvSpecialElements[-index-1].element;
					
			}else if(index>=0){
				if(!mvUnitElements[unit].size())
					mvUnitElements[unit].resize(mvUnits[unit]->CountRows(),NULL);
				if(index>=mvUnitElements[unit].size())
					return E_INVALIDINDEX;
				
				lpElement=mvUnitElements[unit][index];						
				
				if(!lpElement){
					hr = mvUnits[unit]->createElement(index, mpObjContext, &lpElement);
					CHECK_HR;
					mvUnitElements[unit][index]=lpElement;
				}
			}else
				return E_INVALIDINDEX;
			CHECK_PTR(lpElement);
			orvElements.push_back(lpElement);
		}
		
	}
	return hr;
}
int DSSDataModelGroupbyImpl::GetAllText(MBase::CString &orText)
{
	if (mlAllElementOffset!=-1) {
		orText=mvSpecialElements[mlAllElementOffset].displayText;
	}else{
		orText=MBase::CString("All");
	}

	return S_OK;
}
DSSBaseElementProxy* DSSDataModelGroupbyImpl::GetAllElement()
{
	if (mlAllElementOffset!=-1) {
		return mvSpecialElements[mlAllElementOffset].element;
	}else if (!mpAllElement)
	{

		mpAllElement=new DSSAllElementProxy(mpObjContext);
	}	
	return mpAllElement;
	
		
}
int DSSDataModelGroupbyImpl::Select(vector<DSSBaseElementProxy*> &irvGBPath, bool iRemoveAll, int iUnsetType, vector<int> &orvOffsets, bool iUseMask)
{	
	GroupbyElement *lpCurElement = GetElement(irvGBPath, iUnsetType);
    
    if (lpCurElement == NULL)
        return S_OK;

	return GetElementIndexList(lpCurElement, irvGBPath.size(), iRemoveAll, orvOffsets, iUseMask);
}

int DSSDataModelGroupbyImpl::size()
{
	int lSize = sizeof(this);
	
	lSize += hCalculateSizeOfBase();
	
	if (mpRootElement)
		lSize += SizeOfGroupbyElement(*mpRootElement);
	
	//estimate size of element
	int lEstimateSize = (sizeof(SpecialElement) + 12 + sizeof(DSSBaseElementProxy)) * mvSpecialElements.size();

	if (mpAllElement)
		lSize += sizeof(DSSAllElementProxy);
	
	int lnUnits = mvUnits.size();
	for (int i = 0; i < lnUnits; i++)
	{
		lSize += DSSAEHelper::SizeOfStrings(mvDisplayTexts[i]);
		
		lSize += DSSAEHelper::SizeOfVector(mvUnitElements[i]);

		lEstimateSize += sizeof(DSSBaseElementProxy) * mvUnitElements[i].size();		//TODO:
	}
	return lSize + lEstimateSize;
}

int DSSDataModelGroupbyImpl::GetElementCount(int iUnit)
{
	if(iUnit < 0 || iUnit >= mvUnitElements.size())
		return 0;
	return mvUnitElements[iUnit].size();
}

int DSSDataModelGroupbyImpl::GetSpecialElementCount()
{
	return mvSpecialElements.size();
}

DSSBaseElementProxy* DSSDataModelGroupbyImpl::getSpecialElement(int i)
{
	if(i < 0 || i >= mvSpecialElements.size())
		return NULL;
	return mvSpecialElements[i].element;
}

std::string DSSDataModelGroupbyImpl::getSpecialElementText(int i)
{
	if(i < 0 || i >= mvSpecialElements.size())
		return "";
	return mvSpecialElements[i].displayText;
}

GroupbyElement* DSSDataModelGroupbyImpl::GetRootElement()
{
	return mpRootElement;
}

int DSSDataModelGroupbyImpl::createGBElementVector(GroupbyElement* ipRootElement, int &oLeaveCount)
{
    mvGroupbyElements.clear();
    
    oLeaveCount = ipRootElement->getLeaveCount(&mvGroupbyElements);
    
    assert(oLeaveCount == mvGroupbyElements.size());
    
    return S_OK;
}

bool DSSDataModelGroupbyImpl::IsGBElementReady()
{
    return mvGroupbyElements.size() != 0;
}

int DSSDataModelGroupbyImpl::SetNGBWindow(int iStart, int iEnd)
{
    if(iStart < 0 || iEnd < iStart || iEnd >= mvGroupbyElements.size())
        return S_FALSE;
    
    for (int i = 0; i < mvGroupbyElements.size(); i++)
    {
        mvGroupbyElements[i]->flag = 0;
        GroupbyElement* lpParentElemnt = mvGroupbyElements[i]->parent;
        
        while (lpParentElemnt != mpRootElement && lpParentElemnt->flag != 0)
        {
            lpParentElemnt->flag = 0;
            lpParentElemnt = lpParentElemnt->parent;
        }
    }
    
    for (int i = iStart; i <= iEnd; i++)
    {
        mvGroupbyElements[i]->flag = 1;
        GroupbyElement* lpParentElemnt = mvGroupbyElements[i]->parent;
        
        while (lpParentElemnt != mpRootElement && lpParentElemnt->flag != 1)
        {
            lpParentElemnt->flag = 1;
            lpParentElemnt = lpParentElemnt->parent;
        }
    }
    
    return S_OK;
}

int DSSDataModelGroupbyImpl::ClearNGBWindow()
{
    for (int i = 0; i < mvGroupbyElements.size(); i++)
    {
        mvGroupbyElements[i]->flag = -2;
        GroupbyElement* lpParentElemnt = mvGroupbyElements[i]->parent;
        
        while (lpParentElemnt != mpRootElement && lpParentElemnt->flag != -2)
        {
            lpParentElemnt->flag = -2;
            lpParentElemnt = lpParentElemnt->parent;
        }
    }
    
    mvGroupbyElements.clear();
    
    return S_OK;
}

GroupbyElement* DSSDataModelGroupbyImpl::GetElement(vector<DSSBaseElementProxy*> &irvGBPath, int iUnsetType)
{    
    GroupbyElement *lpCurElement=mpRootElement;
    
	for (int i=0; i<irvGBPath.size(); i++)   // for each element in the path
    {   
        bool lFound=false;
			
        if (irvGBPath[i]==NULL) //unset
        {
            if(lpCurElement->children.size()>0)
            {
                if(iUnsetType==DssAEUnsetTypeChooseFirst)//NOTE:other branches won't be reached
                    lpCurElement=&(lpCurElement->children[0]);
                else if(iUnsetType==DssAEUnsetTypeChooseFirstNotAll)
                {
                    if(lpCurElement->children.size()>1)
                    {
                        int index=lpCurElement->children[0].index;
							
                        if(index<0&&-index<=mvSpecialElements.size())
                        {
                            if((-index-1)==mlAllElementOffset)  //equal
                            {
                                lpCurElement=&(lpCurElement->children[1]);
                                continue;
                            }
								
                        }
                    }
                    lpCurElement=&(lpCurElement->children[0]);
                }
                else if(iUnsetType==DssAEUnsetTypeChooseLast)
                    lpCurElement=&(lpCurElement->children.back());
                else
                    return NULL;
            }
            else
                return NULL;
        }
        else
        {
            int unit=-1, key=-1;
            FindElement(irvGBPath[i], unit, key);
            
            for (int j=0; j<lpCurElement->children.size(); j++)
            {
                int index=lpCurElement->children[j].index;
                
                if (index<0&&key<0)
                {
                    if(irvGBPath[i]->IsEqual(mvSpecialElements[-index-1].element))
                    {
                        lpCurElement=&lpCurElement->children[j];
                        lFound=true;
                        break;
                    }     
                }
                else if(key==index)  //found the element
                {
                    lpCurElement=&lpCurElement->children[j];
                    lFound=true;
                    break;
                }
            }
            if (!lFound)//wrong groupby path, no change in the output vector
            {
                return NULL;
            }
        }
            
    }
    return lpCurElement;
}
