/*
 *  DSSTemplateConsolidation.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef TEMPLATE_CONSOLIDATION_H
#define TEMPLATE_CONSOLIDATION_H

#include "DSSTemplateUnit.h"
class DSSConsolidation;

class DSSTemplateConsolidation : public DSSTemplateUnit
{
public:
	DSSTemplateConsolidation();
	virtual ~DSSTemplateConsolidation();
	DSSTemplateConsolidation(DSSObjectContext* ipObjectContext);	
	DSSConsolidation* getConsolidation();
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader, int iIndex, DSSAxis* ipAxis);
};

#endif
