//
//  DSSTabularRefConsolidation.h
//  ReportServiceCore
//
//  Created by Tang, Cheng on 11/23/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef ReportServiceCore_DSSTabularRefConsolidation_h
#define ReportServiceCore_DSSTabularRefConsolidation_h
#include <map>
#include "DSSTabularConsolidation.h"

typedef std::map<int, int, less<int> > IntToIntMapType;

class DSSTabularRefConsolidation : public DSSTabularConsolidation
{
public:
    DSSTabularRefConsolidation();
    
    virtual int TranslateLocalToGlobal(Int32 iLocal);
    
    int SetGlobalRowMap(int iGlobalRowCount, int* ipGlobalToLocalMap, IntToIntMapType* ipGlobalToLocalMap2, int* ipLocalToGlobalMap);
    
	int GetGlobalRowMap(int* opGlobalRowCount, int** oppGlobalToLocalMap, IntToIntMapType** oppGlobalToLocalMap2, int** oppLocalToGlobalMap);
    
    int Init(DSSTabularConsolidation* ipUnit);
    
    //ctang: 2011-11-23 change it to virtual
    virtual int setIsDerivedElement(bool isDE, DSSTabularUnit *ipChildUnit = NULL, bool ibUseGlobal = false);
    
    virtual bool IsDerivedElement();

    // weilin 2016/08/18 DE41482 check nde
    virtual bool IsNewDerivedElement();
	
	virtual int setElement(int index, XTabConsolidationElement &irElement);
    
    virtual XTabConsolidationElement* getElement(int index);
	
	virtual int getText(int index, MBase::CString &orDisplayName);
	
	virtual const char * getTextAndLength(int index, int &orLength);
	
	virtual int CountRows();
	
	virtual int FindElement(DSSBaseElementProxy *ipElement, int &orKey);
	
	virtual int createElement(int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement, int iGlobalKey=-1,int iGlobalKeyVer = -1);
    
	virtual bool IsLeaf(int index);
	
    virtual int getChildElementID(int index, GUID* lpID);	
    
	virtual bool IsDisplayed(int index);
	
	virtual DSSTabularUnit * getChildUnit();
	
	virtual int getChildElementKey(int index);
	
	virtual DSSConsolidationElement* getConsolidationElement(int index);
	
	virtual int size();
    
	virtual int getGUID(GUID &irVal);
    
	virtual const GUID &getGUID();
	
    virtual bool IsRuntimeUnit();
    
    virtual int GetMaxKey();
    
    virtual int getNumAllElements();
    
    // 3/22/2012 tehe 
    virtual bool hasElementDisplayable();
    DSSTabularConsolidation* getConsolidation() {return mpTabularConsolidation;}
    
    virtual int getTabularUnit(DSSTabularUnit** oppUnit);
    
    inline void setJoinNull(int iGlobalKey) { mJoinNull = iGlobalKey; }

    IntToIntMapType* getGToLIntMap();
private:
    
    
    int TranslateGlobalToLocal(Int32 iGlobal);
    
    
    DSSTabularConsolidation* mpTabularConsolidation;
    
    int mGlobalRowCount;
    int* mpGlobalToLocalMap;
    int* mpLocalToGlobalMap;
    //ctang: for DE
    IntToIntMapType* mpGToLIntMap;
    
    // TQMS 785587, 9/15/2013 tehe
    int mMaxOrdinal;
    int mJoinNull;  // global key for join null
};

#endif
