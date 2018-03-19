/*
 *  DSSBaseElementsProxy.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 10-1-6.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef BASE_ELEMENTS_PROXY_H
#define BASE_ELEMENTS_PROXY_H

#include "DSSAttributeElementProxy.h"
#include <vector>

class DSSBaseElementsProxy : public DSSSecondClassObject
{
private:
	std::vector<DSSBaseElementProxy*> mElements;
public:
	DSSBaseElementsProxy();
	virtual ~DSSBaseElementsProxy();
	DSSBaseElementsProxy(DSSObjectContext* ipObjectContext);
	int Count();
	DSSBaseElementProxy* Item(int iIndex);
	void AddElement(DSSBaseElementProxy* iElement);
	void Clear();
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
    bool isEqual(DSSBaseElementsProxy* ipElements) const;
    bool isEqualWithoutOrder(DSSBaseElementsProxy* ipElements) const;
	static DSSBaseElementProxy* Clone(DSSBaseElementProxy* ipSourceElement, DSSObjectContext* ipObjectContext);
	static DSSBaseElementProxy* CreateElementFromID(std::string iElementID, DSSObjectContext* ipObjectContext);
	static std::string ConvertFromLongToShort(std::string iElementID);
};

#endif
