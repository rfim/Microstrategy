/*
 *  DSSNonSharedPropertiesInViews.h
 *  ReportServiceCore
 *
 *  Created by Cao Lei on 11-8-14.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NON_SHARED_PROPERTIES_IN_VIEWS_H
#define NON_SHARED_PROPERTIES_IN_VIEWS_H

#include "DSSNonSharedPropertiesInView.h"
#include "DSSBinaryDataReader.h"
#include <vector>

class DSSNonSharedPropertiesInViews
{
private:
	std::vector<DSSNonSharedPropertiesInView*> mvNonSharedPrptiesInViews;

public:
	DSSNonSharedPropertiesInViews();
	~DSSNonSharedPropertiesInViews();

	int Count();
	DSSNonSharedPropertiesInView* Item(int iIndex);
	void Add(DSSNonSharedPropertiesInView* ipNonSharedPrptiesInView, int iIndex = -1);
	void Remove(int iIndex);
	void Clear();
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
};

#endif
