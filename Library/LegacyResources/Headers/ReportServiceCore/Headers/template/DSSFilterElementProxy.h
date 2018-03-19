/*
 *  DSSFilterElementProxy.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-5-1.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSBaseElementProxy.h"
class DSSCustomGroup;

class DSSFilterElementProxy : public DSSBaseElementProxy
{
private:
	int mNodeID;
	GUID mFilterID;
	std::string mNodeName;
	
public:
	DSSFilterElementProxy();
	virtual ~DSSFilterElementProxy();
	DSSFilterElementProxy(DSSObjectContext* ipObjectContext);
	DSSFilterElementProxy(DSSObjectContext* ipObjectContext, GUID iObjectID, int iNodeID, GUID iFilterID, std::string iNodeName);
	int Parse(std::string iElementID);
	bool IsEqual(DSSBaseElementProxy* ipElement);
	void Clone(DSSBaseElementProxy* ipElement);
	DSSCustomGroup* getCustomGroup();
	int getNodeID();
	GUID getFilterID();
	std::string getNodeName();
};
