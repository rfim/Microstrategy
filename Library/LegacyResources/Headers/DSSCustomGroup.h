/*
 *  DSSCustomGroup.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CUSTOMGROUP_H
#define CUSTOMGROUP_H

#include "DSSObjectInfo.h"
class DSSNode;
class DSSFormat;

typedef std::map < int, DSSNode* > CustomGroupNodeMap;

class DSSCustomGroup : public DSSObjectInfo
{
private:
	CustomGroupNodeMap mNodes;
public:
	DSSCustomGroup();
	virtual ~DSSCustomGroup();
	DSSCustomGroup(GUID& iID, EnumDSSObjectType iType, std::string iName, DSSObjectContext* iObjectContext, GUID iContainerID);
	DSSFormat* getHeaderFormat(int iIndex);
	DSSFormat* getGridFormat(int iIndex);
	DSSFormat* getChildHeaderFormat(int iIndex);
	DSSFormat* getChildGridFormat(int iIndex);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	virtual void MakeCleanDefn();
};

#endif
