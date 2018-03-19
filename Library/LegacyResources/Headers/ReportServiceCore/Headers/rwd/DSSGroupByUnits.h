/*
 *  DSSGroupByUnits.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef GroupByUnits_h
#define GroupByUnits_h

#include "DSSGroupByUnit.h"
#include <vector>

class DSSGroupByUnits : public DSSSecondClassObject
{
private:
	std::vector<DSSGroupByUnit*> mGroupByUnits;
	DSSSorts* mpSorts;
public:
	DSSGroupByUnits();
	virtual ~DSSGroupByUnits();
	DSSGroupByUnits(DSSObjectContext* ipObjectContext);
	int Count();
	DSSGroupByUnit* Item(int iIndex);
	void Add(DSSGroupByUnit* iGroupByUnit, int iIndex=-1);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
    void setSorts(DSSSorts* lpSorts);
    DSSSorts* getSorts();
};

#endif
