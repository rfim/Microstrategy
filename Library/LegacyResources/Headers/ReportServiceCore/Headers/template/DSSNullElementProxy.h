/*
 *  DSSNullElementProxy.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-2.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NULL_ELEMENT_PROXY_H
#define NULL_ELEMENT_PROXY_H

#include "DSSBaseElementProxy.h"

class DSSNullElementProxy : public DSSBaseElementProxy
{
public:
	DSSNullElementProxy();
	virtual ~DSSNullElementProxy();
	DSSNullElementProxy(DSSObjectContext* ipObjectContext);
    DSSNullElementProxy(DSSObjectContext* ipObjectContext, GUID& iObjectID);
	bool IsEqual(DSSBaseElementProxy* ipElement);
};

#endif
