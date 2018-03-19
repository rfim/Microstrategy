/*
 *  DSSSubExpressionElementProxy.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-5-1.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSBaseElementProxy.h"
class DSSCustomGroup;

class DSSSubExpressionElementProxy : public DSSBaseElementProxy
{
private:
	int mNodeID;
public:
	DSSSubExpressionElementProxy();
	virtual ~DSSSubExpressionElementProxy();
	DSSSubExpressionElementProxy(DSSObjectContext* ipObjectContext);
	DSSSubExpressionElementProxy(DSSObjectContext* ipObjectContext, GUID iObjectID, int iNodeID);
	int Parse(std::string iElementID);
	bool IsEqual(DSSBaseElementProxy* ipElement);
	void Clone(DSSBaseElementProxy* ipElement);
	DSSCustomGroup* getCustomGroup();
	int getNodeID();
};
