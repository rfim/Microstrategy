/*
 *  DSSSorts.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SORTS_H
#define SORTS_H

#include "DSSSort.h"
#include <vector>

class DSSSorts : public DSSSecondClassObject
{
private:
	std::vector<DSSSort*> mSorts;
public:
	DSSSorts();
	virtual ~DSSSorts();
	DSSSorts(DSSObjectContext* ipObjectContext);
	int Count();
	DSSSort* Item(int iIndex);
	void Add(DSSSort* iSort, int iIndex=-1);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	int Find(GUID& iID);
    bool Find(GUID& iID, std::vector<DSSSort*>& oSortSet);
    void removeSort(DSSSort* iSort);
    void addSort(DSSSort* iSort, int iPos = -1);
	void Clear();
    bool Empty();
    static DSSSorts* DuplicateSorts(DSSSorts* ipSourceSort);
    void Duplicate(DSSSorts* ipSourceSort);
};

#endif
