/*
 *  DSSSubtotalElementProxy.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 10-2-24.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SUBTOTAL_ELEMENT_PROXY_H
#define SUBTOTAL_ELEMENT_PROXY_H

#include "DSSAllElementProxy.h"

class DSSSubtotalElementProxy : public DSSBaseElementProxy
{
private:
	int mSubtotalIndex;
public:
	DSSSubtotalElementProxy();
	virtual ~DSSSubtotalElementProxy();
	DSSSubtotalElementProxy(DSSObjectContext* ipObjectContext);
	DSSSubtotalElementProxy(DSSObjectContext* ipObjectContext, GUID& iObjectID, int iSubtotalIndex);
	int Parse(std::string iElementID);
	int getSubtotalIndex();
	bool IsEqual(DSSBaseElementProxy* ipElement);
	void Clone(DSSBaseElementProxy* ipElement);
};

#endif
