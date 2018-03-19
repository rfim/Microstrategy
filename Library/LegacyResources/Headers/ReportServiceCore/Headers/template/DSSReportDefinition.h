/*
 *  DSSDataElement.h
 *  ReportServiceCore
 *
 *  Created by Cao Lei on 13-4-18.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef REPORT_DEFINITION_H
#define REPORT_DEFINITION_H

#include "DSSObjectInfo.h"

class DSSReportDefinition : public DSSObjectInfo
{
private:
	GUID mBaseReportID;
	GUID mDataTemplateID;
	int mnDisplayMode;
	int mnReportType;
public:
	DSSReportDefinition();
	virtual ~DSSReportDefinition();
	DSSReportDefinition(GUID& iID, EnumDSSObjectType iType, std::string iName, DSSObjectContext* iObjectContext, GUID iContainerID);
};

#endif
