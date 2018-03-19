/*
 *  DSSAllElementProxy.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-3-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ALL_ELEMENT_PROXY_H
#define ALL_ELEMENT_PROXY_H

#include "DSSNodeElementProxy.h"

class DSSAllElementProxy : public DSSBaseElementProxy
{
private:
	std::string mAllString;
public:
	DSSAllElementProxy();
	virtual ~DSSAllElementProxy();
	DSSAllElementProxy(DSSObjectContext* ipObjectContext);
	int Parse(std::string iElementID);
	bool IsEqual(DSSBaseElementProxy* ipElement);
	std::string getAllString();
};

#endif
