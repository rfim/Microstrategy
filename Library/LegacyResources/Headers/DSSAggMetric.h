/*
 *  DSSAggMetric.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 11-1-13.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AGGMETRIC_H
#define AGGMETRIC_H

#include "DSSMetric.h"

class DSSAggMetric : public DSSMetric
{
public:
	DSSAggMetric();
	virtual ~DSSAggMetric();
	DSSAggMetric(GUID& iID, EnumDSSObjectType iType, std::string iName, DSSObjectContext* iObjectContext, GUID iContainerID);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
};

#endif
