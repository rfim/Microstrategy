/*
 *  DSSTabularConsolidation.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 5/1/10.
 *  Copyright 2010 MicroStrategy, Inc. All rights reserved.
 *
 */
#ifndef DSSTABULARCONSOLIDATION_H
#define DSSTABULARCONSOLIDATION_H

#import "DSSTabularUnit.h"


class DSSTabularAttribute;
class DSSConsolidationElement;

typedef struct XTabConsolidationElement {
	int				branch;
	int				ordinal;
	int				childElementID;
	GUID			guid;
	MBase::CString	guidstr;
	MBase::CString	value;
	DSSConsolidationElement *pConsolidationElement;
    vector<int> mvLeaves; //zhyang, leaves, only for DE
}XTabConsolidationElement;

class DSSTabularConsolidation  : public DSSTabularUnit
{
public:
	DSSTabularConsolidation();
	~DSSTabularConsolidation();
	
	int Init(int index, int nElements);
    
    //ctang: 2011-11-23 change it to virtual
	virtual int setIsDerivedElement(bool isDE, DSSTabularUnit *ipChildUnit = NULL, bool iUseGlobal = false);
	
    virtual bool IsDerivedElement();

    // weilin DE41482 check NDE
    virtual bool IsNewDerivedElement();
	
	virtual int setElement(int index, XTabConsolidationElement &irElement);
	
	virtual XTabConsolidationElement* getElement(int index);
	
	virtual int getText(int index, MBase::CString &orDisplayName);
	
	virtual const char * getTextAndLength(int index, int &orLength);
	
	virtual int CountRows();
    
    virtual int GetMaxKey();
	
	virtual int FindElement(DSSBaseElementProxy *ipElement, int &orKey);
	
	virtual int createElement(int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement, int iGlobalKey=-1, int iGlobalKeyVer = -1);
    
	virtual bool IsLeaf(int index);
	
	virtual bool IsDisplayed(int index);
	
	virtual DSSTabularUnit * getChildUnit();
	
	virtual int getChildElementKey(int index);
	
	virtual DSSConsolidationElement* getConsolidationElement(int index);
    
    //junwang, new rwd, 11/22/2011
    virtual int getChildElementID(int index, GUID* lpID);
	
	virtual int size();
    
    virtual int getNumAllElements();
    
    
    // 3/22/2012 tehe
    virtual int TranslateLocalToGlobal(int iLocal);
    virtual int TranslateGlobalToLocal(int iGlobal);
    
    virtual bool hasElementDisplayable();
    
    //juwnang, 04/19/2012
    virtual bool compareElement(DSSBaseElementProxy *ipElement, int iKey);
    
    //junwang, 04/18/2012
    virtual void dump();
    
    virtual int getOrdinal(int index, int *opOrdinal);
    
    virtual int GetLeavesByElement(Int32 Element, bool *opfBranch, vector<int> *opvLeaves);
    
    inline int Count(int *opElementsCount){return this->CountRows();};
    
    int Item(int index, int iChild, VARIANT_BOOL *opIsBranch, int *opChildKey);
    
    int GetDuplicateElements(int index, int *opElementsCount, int **oppElements);
    
private:
	vector<XTabConsolidationElement> mvElements;
	
	bool mbIsDerivedElement;
	DSSTabularAttribute *mpChildUnit;
    bool mbUseGlobalLookUp;
    
};

#endif
