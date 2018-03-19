/*
 *  DSSCManipulationDataChangeForTxRcdAndXTab.h
 *  ReportServiceCore
 *
 *  Created by Cao Lei on 11-11-02.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CMANIPULATION_DATA_CHANGE_FOR_TXRCD_AND_XTAB_H
#define CMANIPULATION_DATA_CHANGE_FOR_TXRCD_AND_XTAB_H

#include "DSSObjectInfo.h"
#include "DSSRWIterator.h"
#include "DSSCellHandle.h"
#include "DSSCManipulation.h"
#include <vector>

class DSSCManipulationDataChangeForTxRcdAndXTab : public DSSCManipulation
{
private:
	std::vector<DSSCellHandle*> mpCellHandles;
	std::vector<CComVariant> mVariants;
	int mnModifiedCellIndex;
	CComVariant mNewVariant;
	DSSRWNode* mpNode;
	DSSRWIterator* mpIterator;
    bool mbAutoRefresh;
	
public:
	DSSCManipulationDataChangeForTxRcdAndXTab(DSSRWIterator* ipIterator, std::vector<DSSCellHandle*>& ipCellHandles, std::vector<CComVariant>& iVariants, int inModifiedCellIndex, CComVariant iNewValue, bool ibAutoRefresh=false);
	virtual ~DSSCManipulationDataChangeForTxRcdAndXTab();
	int Execute();
    DSSRWNode* getRWNode();
	bool isAutoRefresh();
    std::vector<DSSRWNode*>* getNodesDataChanged();
};

inline DSSRWNode* DSSCManipulationDataChangeForTxRcdAndXTab::getRWNode()
{
    return mpNode;
}

inline bool DSSCManipulationDataChangeForTxRcdAndXTab::isAutoRefresh()
{
    return mbAutoRefresh;
}
#endif
