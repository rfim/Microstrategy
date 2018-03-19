/*
 *  DSSMetricElementProxy.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
 
#ifndef METRIC_ELEMENT_PROXY_H
#define METRIC_ELEMENT_PROXY_H

#include "DSSSubtotalElementProxy.h"
class DSSMetric;

class DSSMetricElementProxy : public DSSBaseElementProxy
{
public:
	DSSMetricElementProxy();
	virtual ~DSSMetricElementProxy();
	DSSMetricElementProxy(DSSObjectContext* ipObjectContext);
	DSSMetricElementProxy(DSSObjectContext* ipObjectContext, GUID& iObjectID);
	int Parse(std::string iElementID);
	DSSMetric* getMetric();
};

#endif
