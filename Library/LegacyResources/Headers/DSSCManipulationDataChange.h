/*
 *  DSSCManipulationDataChange.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-11-11.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CMANIPULATION_DATA_CHANGE_H
#define CMANIPULATION_DATA_CHANGE_H

#include "DSSObjectInfo.h"
#include "DSSRWIterator.h"
#include "DSSCellHandle.h"
#include "DSSCManipulation.h"
#include <vector>

class DSSCManipulationDataChange : public DSSCManipulation
{
private:
	DSSRWNode* mpNode;
	DSSRWIterator* mpIterator;
	bool mAutoRefresh;

	bool mUpdateSingleCell;

	DSSCellHandle* mpCellHandle;
	CComVariant mVariant;

	std::vector<DSSCellHandle*> mpCellHandles;
	std::vector<CComVariant> mVariants;
public:
	DSSCManipulationDataChange(DSSRWIterator* ipIterator, DSSCellHandle* ipCellHandle, CComVariant iVariant, bool iAutoRefresh=false);
	DSSCManipulationDataChange(DSSRWIterator* ipIterator, std::vector<DSSCellHandle*>& ipCellHandles, std::vector<CComVariant>& iVariants, bool iAutoRefresh=false);
	DSSCManipulationDataChange(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	DSSRWNode* getRWNode();
	bool isAutoRefresh();
	virtual ~DSSCManipulationDataChange();
	int Execute();
};

inline DSSRWNode* DSSCManipulationDataChange::getRWNode()
{
    return mpNode;
}

inline bool DSSCManipulationDataChange::isAutoRefresh()
{
    return mAutoRefresh;
}

#endif
