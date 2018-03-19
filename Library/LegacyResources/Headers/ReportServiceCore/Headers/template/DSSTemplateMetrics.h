/*
 *  DSSTemplateMetrics.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TEMPLATE_METRICS_H
#define TEMPLATE_METRICS_H

#include "DSSTemplateMetric.h"
#include <vector>

class DSSTemplateMetrics : public DSSTemplateUnit
{
private:
	std::vector<DSSTemplateMetric*> mMetrics;
public:
	DSSTemplateMetrics();
	virtual ~DSSTemplateMetrics();
	DSSTemplateMetrics(DSSObjectContext* ipObjectContext);	
	int Count();
	DSSTemplateMetric* Item(int iIndex);
	DSSTemplateMetric* ItemByGUID(GUID iGuid);
    void Add(DSSTemplateMetric* iTemplateMetric, int iIndex);
    void Remove(int iIndex);
    void Move(DSSTemplateMetric* iTemplateMetric, int iPos);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader, int iIndex, DSSAxis* ipAxis);
	EnumUnitTransactionFlags IsEditable();
	EnumUnitTransactionFlags IsRequired();
};

#endif
