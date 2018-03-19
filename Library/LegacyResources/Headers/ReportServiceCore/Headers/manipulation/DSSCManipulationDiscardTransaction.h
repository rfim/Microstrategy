//
//  DSSCManipulationDiscardTransaction.h
//  ReportServiceCore
//
//  Created by Zeng Jinliang on 6/30/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#ifndef CManipulation_Discard_Transaction_H
#define CManipulation_Discard_Transaction_H

#include "DSSRWIterator.h"
#include "DSSCellHandle.h"
#include "DSSCManipulation.h"

class DSSCManipulationDiscardTransaction : public DSSCManipulation
{
private:
	DSSRWNode* mpNode;
	DSSRWIterator* mpIterator;
	bool mAutoRefresh;
    
	std::vector<DSSCellHandle*> mpCellHandles;
public:
	DSSCManipulationDiscardTransaction(DSSRWIterator* ipIterator, DSSCellHandle* ipCellHandle, bool iAutoRefresh = false);
	DSSCManipulationDiscardTransaction(DSSRWIterator* ipIterator, std::vector<DSSCellHandle*>& ipCellHandles, bool iAutoRefresh = false);
	DSSCManipulationDiscardTransaction(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	DSSRWNode* getRWNode() { return mpNode; }
	virtual ~DSSCManipulationDiscardTransaction();
	int Execute();
};

#endif /* defined(CManipulation_Discard_Transaction_H) */
