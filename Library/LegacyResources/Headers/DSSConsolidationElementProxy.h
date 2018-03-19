/*
 *  DSSConsolidationElementProxy.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-30.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CONSOLIDATION_ELEMENT_PROXY_H
#define CONSOLIDATION_ELEMENT_PROXY_H

#include "DSSBaseElementProxy.h"
class DSSConsolidationElement;

class DSSConsolidationElementProxy : public DSSBaseElementProxy
{
private:
	GUID mConsolidationElementID;
	GUID mContainerID;					// Could be GUID_NULL
public:
	DSSConsolidationElementProxy();
	virtual ~DSSConsolidationElementProxy();
	DSSConsolidationElementProxy(DSSObjectContext* ipObjectContext);
	DSSConsolidationElementProxy(DSSObjectContext* ipObjectContext, GUID iUnitObjectID, EnumDSSObjectType iUnitObjectType, GUID iConsolidationElementID, GUID iContainerID);
	int Parse(std::string iElementID);
	bool IsEqual(DSSBaseElementProxy* ipElement);
	GUID getConsolidationID();
	GUID getConsolidationElementID();
	GUID getAttributeID();
	GUID getContainerID();
	void Clone(DSSBaseElementProxy* ipElement);
	DSSConsolidationElement* getConsolidationElement();
};

#endif
