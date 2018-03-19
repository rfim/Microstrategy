/*
 *  DSSCManipulationMarkRow.cpp
 *  ReportServiceCore
 *
 *  Created by Li Jing on 11-03-07.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSCManipulationMarkRow.h"
#include "DSSrwdconstants.h"
#include "DSSAOMarkRow.h"
#include "DSSRWDEngine.h"

DSSCManipulationMarkRow::DSSCManipulationMarkRow(DSSRWIterator* ipIterator, int iRowOrdinal, EnumDSSRWMarkRowAction iAction)
{
	mpNode = ipIterator->getRWNode();
	mpIterator = ipIterator;
	mRowOrdinal = iRowOrdinal;
	mAction = iAction;
	mIsFromActionObject = false;
    
    mType = ManipulationMarkRow;
}

DSSCManipulationMarkRow::DSSCManipulationMarkRow(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
	DSSAOMarkRow* lpAO = static_cast<DSSAOMarkRow*> (ipAO);
	mpNode = (DSSRWNode*)(ipContext->getNode(lpAO->getNodeKey()));
	mRowOrdinal = lpAO->getRowOrdinal();
	mAction = lpAO->getActionType();
	
	std::string lKey = mpNode->getKey();
	std::vector<int> lNGBPath = lpAO->getNGBPath();
	DSSRWIterator* lpIterator;
	mpNode->getObjectContext()->getRWDEngine()->FindIterator(lKey, lNGBPath, &lpIterator);
	mpIterator = lpIterator;
	
	mIsFromActionObject = true;
    
    mType = ManipulationMarkRow;
}

DSSCManipulationMarkRow::~DSSCManipulationMarkRow()
{}

int DSSCManipulationMarkRow::Execute()
{
    if (!mpNode)
        return S_FALSE;
    
    if (hCheckLayoutLoaded(mpNode)==S_FALSE)
		return S_FALSE;
    
	DSSRWTemplateNode * lpRWTemplateNode = NULL;
    DSSRWFieldGroupNode* lpRWFieldGroupNode = NULL;
    
    if (mpNode->getType() == DssRWNodeTemplate)
    {
        lpRWTemplateNode = (DSSRWTemplateNode*)(mpNode);
        if (lpRWTemplateNode->IsTransactionEnabled())
        {
            ((DSSRWGridIterator*)mpIterator)->MarkRows(mRowOrdinal, mAction);
        }
    } else if (mpNode->getType() == DssRWNodeFieldGroup)
    {
        lpRWFieldGroupNode = (DSSRWFieldGroupNode*)mpNode;
        if (lpRWFieldGroupNode->IsTransactionEnabled())
        {
            ((DSSRWFieldGroupIterator*)mpIterator)->MarkRows(mRowOrdinal, mAction);
        }
        
    }

	if (!mIsFromActionObject)
	{
		mpActionObject = new DSSAOMarkRow();
		((DSSAOMarkRow*)mpActionObject)->setNodeKey(mpNode->getKey());
		((DSSAOMarkRow*)mpActionObject)->setRowOrdinal(mRowOrdinal);
		((DSSAOMarkRow*)mpActionObject)->setActionType(mAction);
		
		std::vector<int> lNGBPath;
		mpIterator->getNGBPath(lNGBPath);
		((DSSAOMarkRow*)mpActionObject)->setNGBPath(lNGBPath);
	}
	
	return S_OK;
}


