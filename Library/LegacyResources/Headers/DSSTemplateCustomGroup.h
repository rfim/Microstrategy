/*
 *  DSSTemplateCustomGroup.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef TEMPLATE_CUSTOMGROUP_H
#define TEMPLATE_CUSTOMGROUP_H

#include "DSSTemplateUnit.h"
class DSSCustomGroup;

class DSSTemplateCustomGroup : public DSSTemplateUnit
{
private:
	bool mFlatten;
public:
	DSSTemplateCustomGroup();
	virtual ~DSSTemplateCustomGroup();
	DSSTemplateCustomGroup(DSSObjectContext* ipObjectContext);	
	DSSCustomGroup* getCustomGroup();
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader, int iIndex, DSSAxis* ipAxis);
	bool getFlatten();
    void setFlatten(bool iFlatten);
};

#endif
