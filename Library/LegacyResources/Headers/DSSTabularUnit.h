/*
 *  DSSTabularUnit.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/30/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import "DSSAEHelper.h"
#import "GUID.h"

class DSSBaseElementProxy;
class DSSObjectContext;
class DSSDataModelBase;

class DSSTabularUnit : public IXTABAttributeInfo, public IStatisticsInfo
{
public:
	DSSTabularUnit();
	virtual ~DSSTabularUnit();
	
	int getUnitIndex();
    
    //ctang
    void setUnitIndex(int iIndex);
    
	int setGUID(MBase::CString &irVal);
	
	//int getGUID(MBase::CString &orVal);
	
	int setGUID(GUID &irVal);
	
	virtual int getGUID(GUID &irVal);
	
	virtual const GUID &getGUID();//{return mGUID;};
	
    int get_IDI(GUID *opID);

	EnumDSSTemplateUnitType getUnitType();
	int get_UnitType(EnumDSSTemplateUnitType *opType);
    
	virtual int CountRows() = 0;
	
	inline void setIsRuntimeUnit(bool val){mbIsRuntimeUnit = val;};
	virtual bool IsRuntimeUnit();//{return mbIsRuntimeUnit;};
    virtual bool IsRecursiveUnit();
	/*
	 @method
	 @abstract   translate an element object to a local element key, return S_FALSE if not found
	 @discussion it's a linear search, the complexity is O(n*m)
	 */
	virtual int FindElement(DSSBaseElementProxy *ipElement, int &orKey) = 0;
    
	virtual int createElement(int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement, int iGlobalKey=-1, int iGlobalKeyVer = -1) = 0;
	
	int createSpecialElement(EnumDSSElementType iElementType, int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement);
	
	void setParentInfo(DSSDataModelBase *ipModel);
    
	void setNullElement(int index, MBase::CString &irElementID);
    
	virtual int size();
    
    inline void setObjectContextRef(DSSObjectContext *ipObjContext){mpObjContext = ipObjContext;};
	
    /*
     @method
     @abstract check the element object is same with the data having the local element key iKey, return value(true for equal, false for non-equal)
     */
    virtual bool compareElement(DSSBaseElementProxy *ipElement, int iKey) = 0;
    
    virtual int getTabularUnit(DSSTabularUnit** oppUnit);
    
    virtual MBase::CString getGUIDStr() { return mGUIDStr;}
    
protected:
	
	DSSObjectContext* hGetContext();
	
	DSSBaseElementProxy *hGetNullElement();
    
	//private data member
	int mUnitID;
	
	MBase::CString mGUIDStr;
	GUID mGUID;
	
	EnumDSSTemplateUnitType mUnitType;
	
	bool mbIsRuntimeUnit;
	
	DSSDataModelBase *mpModel;
	DSSObjectContext *mpObjContext;
    
	int mNullElementIndex;
	MBase::CString mNullElementIDStr;
	DSSBaseElementProxy *mpNullElement;
};

inline int DSSTabularUnit::getUnitIndex()
{
	return mUnitID;
}

inline EnumDSSTemplateUnitType DSSTabularUnit::getUnitType()
{
	return mUnitType;
}

inline int DSSTabularUnit::get_UnitType(EnumDSSTemplateUnitType *opType)
{
    if (opType)
        *opType = mUnitType;
    return S_OK;
}

inline void DSSTabularUnit::setUnitIndex(int iIndex)
{
    mUnitID = iIndex;
}


