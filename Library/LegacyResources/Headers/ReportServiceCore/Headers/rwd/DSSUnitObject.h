/*
 *  DSSUnitObject.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 11-1-18.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef UNIT_OBJECT_H
#define UNIT_OBJECT_H

#include <vector>
#include "DSSSecondClassObject.h"

typedef enum
{
    HEADER = 0,
    GRID = 1,
    SUBTOTAL_HEADER = 2,
    SUBTOTAL_GRID = 3,
    FORMAT_MAX = 4
} EnumFormatRefIndex;

class DSSUnitObject : public DSSSecondClassObject
{
private:
	GUID mID;
	std::string mAlias;
	int mElementCount;
	std::vector<int> mBaseFormIDs;
	std::vector<int> mBaseFormTypes;
    std::vector<int> mFormatRefIDs;
    
    std::vector<GUID> mFormIDs;
    std::vector<GUID> mDataTemplateFormIDs;
    std::vector<GUID> mBrowseFormIDs;
    
public:
	DSSUnitObject();
	virtual ~DSSUnitObject();
	DSSUnitObject(GUID iID, std::string iAlias, int iElementCount, DSSObjectContext* ipObjectContext);
	GUID getID();
	std::string getAlias();
	int getElementCount();
	std::vector<int>* getBaseFormIDs();
	std::vector<int>* getBaseFormTypes();
	void AddBaseForm(int iBaseFormID, int iBaseFormType);
    
    DSSFormat* getHeaderFormat();
    DSSFormat* getGridFormat();
    DSSFormat* getSubtotalHeaderFormat();
    DSSFormat* getSubtotalGridFormat();
    std::vector<int> getFormatRefIDs();
    void setFormatRefIDs(std::vector<int>& iRefIDs);
    
    int FormCount();
    GUID FormItem(int iIndex);
    int DataTemplateFormCount();
    GUID DataTemplateFormItem(int iIndex);
    int BrowseFormCount();
    GUID BrowseFormItem(int iIndex);
    void AddForm(GUID iFormID);
    void AddDataTemplateForm(GUID iDataTemplateFormID);
    void AddBrowseForm(GUID iBrowseFormID);
    std::vector<GUID>* getFormIDs();
    std::vector<GUID>* getDataTemplateFormIDs();
    std::vector<GUID>* getBrowseFormIDs();
    void SetFormatRefID(int iIndex, int iFormatID);
};

inline int DSSUnitObject::BrowseFormCount()
{
    return (int)mBrowseFormIDs.size();
}

inline GUID DSSUnitObject::BrowseFormItem(int iIndex)
{
    if(iIndex < 0 || iIndex >= mBrowseFormIDs.size())
        return GUID_NULL;
    
    return mBrowseFormIDs[iIndex];
}

inline void DSSUnitObject::AddBrowseForm(GUID iBrowseFormID)
{
    mBrowseFormIDs.push_back(iBrowseFormID);
}

inline std::vector<GUID>* DSSUnitObject::getFormIDs()
{
    return &mFormIDs;
}

inline std::vector<GUID>* DSSUnitObject::getDataTemplateFormIDs()
{
    return &mDataTemplateFormIDs;
}

inline std::vector<GUID>* DSSUnitObject::getBrowseFormIDs()
{
    return &mBrowseFormIDs;
}
#endif
