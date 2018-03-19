/*
 *  DSSSort.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SORT_H
#define SORT_H

#include "DSSTemplateAttribute.h"
#include <vector>

class DSSSort : DSSSecondClassObject
{
private:
	EnumDSSSortType mSortType;
	bool mAscending;
	GUID mObjectID;
	EnumDSSObjectType mObjectType;
	GUID mFormID;
	int mBaseFormID;
	int mSubtotalIndex;
	int mSubtotalPosition;
	int mLocale;
	DSSBaseElementsProxy* mpElements;
	
public:
	DSSSort();
	virtual ~DSSSort();
	DSSSort(DSSObjectContext* ipObjectContext);
	EnumDSSSortType getSortType();
	bool getAscending();
	GUID getObjectID();
	EnumDSSObjectType getObjectType();
	GUID getFormID();
	int getBaseFormID();
	int getSubtotalIndex();
	int getSubtotalPosition();
	DSSBaseElementsProxy* getElements();
	void setElements(DSSBaseElementsProxy* ipElements);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);

	void setSortType(EnumDSSSortType iSortType);
	void setAscending(bool iAscending);
	void setObjectID(GUID iID);
	void setSubtotalPosition(int iSubtotalPosition);
	void setFormID(GUID iFormID);
	void setBaseFormID(int iBaseFormID);
    
    int getLocale();
    void Duplicate(DSSSort* ipSort);
};

#endif
