/*
 *  DSSDataElements.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 11-1-18.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef DATA_ELEMENTS_H
#define DATA_ELEMENTS_H

#include "DSSDataElement.h"
#include <vector>

class DSSDataElements : public DSSSecondClassObject
{
private:
	std::vector<DSSDataElement*> mDataElements;
	int mDefaultIndex;
public:
	DSSDataElements();
	virtual ~DSSDataElements();
	DSSDataElements(DSSObjectContext* ipObjectContext);
	int Count();
	DSSDataElement* Item(int iIndex);
	void Add(DSSDataElement* ipDataElement, int iIndex=-1);
	void setDefaultIndex(int iDefaultIndex);
	DSSDataElement* getDefaultDataElement();
	DSSDataElement* FindByID(GUID iID);
};

#endif
