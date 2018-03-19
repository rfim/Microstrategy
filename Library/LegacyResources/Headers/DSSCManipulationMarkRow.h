/*
 *  DSSCManipulationMarkRow.h
 *  ReportServiceCore
 *
 *  Created by Li Jing on 11-03-07.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CMANIPULATION_MARKROW_H
#define CMANIPULATION_MARKROW_H

#include "DSSRWGridIterator.h"
#include "DSSRWFieldGroupIterator.h"
#include "DSSRWTemplateNode.h"
#include "DSSCManipulation.h"

class DSSCManipulationMarkRow : public DSSCManipulation
{
private:
	DSSRWNode* mpNode;
	DSSRWIterator* mpIterator;
	int mRowOrdinal;
	EnumDSSRWMarkRowAction mAction;
public:
	DSSCManipulationMarkRow(DSSRWIterator* ipGridIterator, int iRowOrdinal, EnumDSSRWMarkRowAction iAction);
	DSSCManipulationMarkRow(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSCManipulationMarkRow();
	int Execute();
};

#endif
