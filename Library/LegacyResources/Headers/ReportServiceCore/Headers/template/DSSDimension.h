/*
 *  DSSDimension.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-5-7.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef DIMENSTION_H
#define DIMENSTION_H

#include "DSSObjectInfo.h"

class DSSDimension : public DSSObjectInfo
{
public:
	DSSDimension();
	virtual ~DSSDimension();
	DSSDimension(GUID& iID, EnumDSSObjectType iType, std::string iName, DSSObjectContext* iObjectContext, GUID iContainerID);
};

#endif
