/*
 *  DSSMetrics.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 11-3-1.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef METRICS_H
#define METRICS_H

#include "DSSMetric.h"
#include <vector>

class DSSMetrics : public DSSSecondClassObject
{
private:
	std::vector<GUID> mMetricIDs;
public:
	DSSMetrics();
	virtual ~DSSMetrics();
	DSSMetrics(DSSObjectContext* ipObjectContext);
	int Count();
	DSSMetric* Item(int iIndex);
	void Add(GUID iID, int iIndex=-1);
    void RemoveByID(GUID iID);
	DSSMetric* FindByID(GUID iID);
};

#endif
