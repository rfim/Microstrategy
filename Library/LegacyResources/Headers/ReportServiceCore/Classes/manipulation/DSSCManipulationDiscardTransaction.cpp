//
//  DSSCManipulationDiscardTransaction.cpp
//  ReportServiceCore
//
//  Created by Zeng Jinliang on 6/30/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#include "DSSCManipulationDiscardTransaction.h"
#import "DSSRWGridIterator.h"
#import "DSSRWFieldGroupIterator.h"
#include "DSSRWTemplateNode.h"
#include "DSSDataModelXTabImpl.h"

DSSCManipulationDiscardTransaction::DSSCManipulationDiscardTransaction(DSSRWIterator* ipIterator, DSSCellHandle* ipCellHandle, bool iAutoRefresh)
{
	mpNode = ipIterator->getRWNode();
    mpCellHandles.push_back(ipCellHandle);
	mpIterator = ipIterator;
	mAutoRefresh = iAutoRefresh;
	mIsFromActionObject = false;
    
    mType = ManipulationDiscardTransaction;
}

DSSCManipulationDiscardTransaction::DSSCManipulationDiscardTransaction(DSSRWIterator* ipIterator, std::vector<DSSCellHandle*>& ipCellHandles, bool iAutoRefresh)
{
	mpNode = ipIterator->getRWNode();
	mpCellHandles = ipCellHandles;
	mpIterator = ipIterator;
	mAutoRefresh = iAutoRefresh;
	mIsFromActionObject = false;
    
    mType = ManipulationDiscardTransaction;
}

DSSCManipulationDiscardTransaction::DSSCManipulationDiscardTransaction(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationDiscardTransaction;
	mIsFromActionObject = true;
    
    // TODO
}

DSSCManipulationDiscardTransaction::~DSSCManipulationDiscardTransaction()
{}

int DSSCManipulationDiscardTransaction::Execute()
{
    if (!mpNode)
        return S_FALSE;
    if (hCheckLayoutLoaded(mpNode) == S_FALSE)
		return S_FALSE;
    
	HRESULT hr = S_OK;
	if (mpIterator->getType() == DssIteratorGrid)
	{
        for (std::vector<DSSCellHandle* >::iterator itCell = mpCellHandles.begin(); itCell != mpCellHandles.end(); itCell++)
        {
            DSSCellHandle *lpCellHandle = (*itCell);
            hr = (static_cast<DSSRWGridIterator*>(mpIterator))->DiscardTransactionChange(*lpCellHandle);
            if (hr != S_OK)
                return hr;
        }
        
		if (mAutoRefresh)
		{
			DSSTemplate * lpTemplate = static_cast<DSSRWTemplateNode*>(mpNode)->getTemplate();
            IDSSTemplateModel *lpTemplateDataModel = lpTemplate->getTemplateModelInterface();
            
			if (lpTemplateDataModel)
			{
				lpTemplateDataModel->ReCalculate();
			}
            mNodesModelChanged.push_back(mpIterator->getRWNode());
		}
	}
	else if (mpIterator->getType() == DssIteratorFieldGroup)
	{
        for (std::vector<DSSCellHandle* >::iterator itCell = mpCellHandles.begin(); itCell != mpCellHandles.end(); itCell++)
        {
            DSSCellHandle *lpCellHandle = (*itCell);
            hr = (static_cast<DSSRWFieldGroupIterator*>(mpIterator))->DiscardTransactionChange(*lpCellHandle);
            if (hr != S_OK)
                return hr;
        }
	}
    
	if (!mIsFromActionObject)
	{
		// TODO
	}

    if (std::find(mNodesDataChanged.begin(), mNodesDataChanged.end(), mpNode) == mNodesDataChanged.end())
        mNodesDataChanged.push_back(mpNode);
    
	return S_OK;
}
