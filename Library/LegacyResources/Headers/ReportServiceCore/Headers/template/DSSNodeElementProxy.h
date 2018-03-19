/*
 *  DSSNodeElementProxy.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-3-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NODE_ELEMENT_PROXY_H
#define NODE_ELEMENT_PROXY_H

#include "DSSNullElementProxy.h"

class DSSNodeElementProxy : public DSSBaseElementProxy
{
private:
	std::string mNodeKey;
public:
	DSSNodeElementProxy();
	virtual ~DSSNodeElementProxy();
	DSSNodeElementProxy(DSSObjectContext* ipObjectContext);
	DSSNodeElementProxy(DSSObjectContext* ipObjectContext, std::string iNodeKey);
	int Parse(std::string iElementID);
	bool IsEqual(DSSBaseElementProxy* ipElement);
	std::string getNodeKey();
	void Clone(DSSBaseElementProxy* ipElement);
};

#endif
