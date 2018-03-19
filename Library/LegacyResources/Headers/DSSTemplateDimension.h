/*
 *  DSSTemplateDimension.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-5-7.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TEMPLATE_DIMENSTION_H
#define TEMPLATE_DIMENSTION_H

#include "DSSTemplateUnit.h"
class DSSDimension;
class DSSTemplateAttribute;
class DSSAxis;

typedef std::vector<DSSTemplateAttribute*> TemplateAttributes;

class DSSTemplateDimension : public DSSTemplateUnit
{
private:
	TemplateAttributes mTemplateAttributes;
public:
	DSSTemplateDimension();
	virtual ~DSSTemplateDimension();
	DSSTemplateDimension(DSSObjectContext* ipObjectContext);
	DSSDimension* getDimension();
	int Count();
	DSSTemplateAttribute* Item(int iIndex);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader, int iIndex, DSSAxis* ipAxis);	
};

#endif
