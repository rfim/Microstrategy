//
//  DSSTabularRefCustomGroup.h
//  ReportServiceCore
//
//  Created by Tang, Cheng on 11/23/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef ReportServiceCore_DSSTabularRefCustomGroup_h
#define ReportServiceCore_DSSTabularRefCustomGroup_h

#include "DSSTabularCustomGroup.h"

class DSSTabularRefCustomGroup : public DSSTabularCustomGroup
{
public:
    
    DSSTabularRefCustomGroup();
    
    virtual ~DSSTabularRefCustomGroup();
    
    int Init(DSSTabularCustomGroup* ipUnit);
    
    int SetGlobalRowMap(int iGlobalRowCount, int* ipGlobalToLocalMap, int* ipLocalToGlobalMap);
    
	int GetGlobalRowMap(int* opGlobalRowCount, int** oppGlobalToLocalMap, int** oppLocalToGlobalMap);
    
    int SetOrdinal(vector<int>& ivOrdinal);
    
    //ctang : change it to virtual function
	virtual int setElement(int index, XTabCustomGroupElement &irElement);
    
	virtual XTabCustomGroupElement * getElement(int index);
	//ctang: wrapper of get ordinal
    virtual int getOrdinal(int index);
    
	virtual int getMaxDepth();
	
	virtual int getParentKeys(int index, vector<int> &orParentKeys);
	
	//int getFilterID(int index);
	
	//int getBandNumber(int index);
	
	virtual int getText(int index, MBase::CString &orDisplayName);
	
	virtual const char * getTextAndLength(int index, int &orLength);
	
	virtual int CountRows();
	
	virtual int FindElement(DSSBaseElementProxy *ipElement, int &orKey);
	
	virtual int createElement(int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement, int iGlobalKey=-1,int iGlobalKeyVer = -1);
	
	virtual DSSBaseElementProxy * getCustomGroupElement(int index);
	
	virtual int size();
    
	virtual int getGUID(GUID &irVal);
    
	virtual const GUID &getGUID();
	
    virtual bool IsRuntimeUnit();
    
    //zhyang, TQMS 540004, we need to get the parent key and translate it
    virtual int getParentKey(int childKey, int *parent);
    
    virtual int getTabularUnit(DSSTabularUnit** oppUnit);
    
private:
    
    int TranslateLocalToGlobal(Int32 iLocal);
    int TranslateGlobalToLocal(Int32 iGlobal);
    
    DSSTabularCustomGroup* mpTabularCustomGroup;
    
    int mGlobalRowCount;
    int* mpGlobalToLocalMap;
    int* mpLocalToGlobalMap;
    
    vector<int> mvOrdinal;
};


#endif
