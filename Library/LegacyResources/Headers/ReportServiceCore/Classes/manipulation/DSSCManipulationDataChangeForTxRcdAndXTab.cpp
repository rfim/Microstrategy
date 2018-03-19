/*
 *  DSSCManipulationDataChangeForTxRcdAndXTab.cpp
 *  ReportServiceCore
 *
 *  Created by Cao Lei on 11-11-02.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSCManipulationDataChangeForTxRcdAndXTab.h"
#include "DSSDocumentDefinition.h"
#include "DSSRWTemplateNode.h"
#include "DSSRWFieldGroupNode.h"
#import "DSSRWGridIterator.h"
#import "DSSRWFieldGroupIterator.h"
#include "DSSDataModelXTabImpl.h"
#include "DSSRWDEngine.h"
#include "DSSAODataChange.h"

DSSCManipulationDataChangeForTxRcdAndXTab::DSSCManipulationDataChangeForTxRcdAndXTab(DSSRWIterator* ipIterator, std::vector<DSSCellHandle*>& ipCellHandles, std::vector<CComVariant>& iVariants, int inModifiedCellIndex, CComVariant iNewValue, bool ibAutoRefresh)
{
	mpIterator = ipIterator;
	mpNode = mpIterator->getRWNode();
	mpCellHandles = ipCellHandles;
	mVariants = iVariants;
	mnModifiedCellIndex = inModifiedCellIndex;
	mNewVariant = iNewValue;
    mbAutoRefresh = ibAutoRefresh;
    mIsFromActionObject = false;
	mNewVariant = iNewValue;
    
    mType = ManipulationDataChangeForTxRcdAndXTab;
}

DSSCManipulationDataChangeForTxRcdAndXTab::~DSSCManipulationDataChangeForTxRcdAndXTab()
{}

int DSSCManipulationDataChangeForTxRcdAndXTab::Execute()
{
    if (hCheckLayoutLoaded(mpNode)==S_FALSE)
		return S_FALSE;
    
	HRESULT hr = S_OK;
	if (mnModifiedCellIndex<0 || mnModifiedCellIndex>=mpCellHandles.size())
		return E_INVALIDINDEX;
    
	if (mpIterator->getType() == DssIteratorGrid)
	{
		DSSRWGridIterator* lpGridIterator = static_cast<DSSRWGridIterator*> (mpIterator);
		DSSCellHandle* lpCellHandle = mpCellHandles[mnModifiedCellIndex];
		hr = lpGridIterator->GenerateTransactionRecord(*lpCellHandle, mNewVariant);
		if (hr != S_OK)
			return hr;

		std::vector<DSSCellHandle*>::iterator itCell = mpCellHandles.begin();
		std::vector<CComVariant>::iterator itValue = mVariants.begin();
		while (itCell!=mpCellHandles.end() && itValue!=mVariants.end())
		{
			DSSCellHandle* lpCellHdl = (*itCell);
			CComVariant lVariant = (*itValue);
			hr = lpGridIterator->UpdateXTabCell(*lpCellHdl, lVariant);
			if (hr != S_OK)
				return hr;

			itCell++;
			itValue++;
		}
        
        if (mbAutoRefresh)
        {
            DSSTemplate* lpTemplate = static_cast<DSSRWTemplateNode*>(mpNode)->getTemplate();
            IDSSTemplateModel* lpTemplateDataModel = lpTemplate->getTemplateModelInterface();
            if (lpTemplateDataModel)
                lpTemplateDataModel->ReCalculate();
            mNodesModelChanged.push_back(mpNode);
        }
	}
	else if (mpIterator->getType() == DssIteratorFieldGroup)
	{
		DSSRWFieldGroupIterator* lpFGIterator = static_cast<DSSRWFieldGroupIterator*> (mpIterator);
		DSSCellHandle* lpCellHandle = mpCellHandles[mnModifiedCellIndex];
		hr = lpFGIterator->GenerateTransactionRecord(*lpCellHandle, mNewVariant);
		if (hr != S_OK)
			return hr;
	
		std::vector<DSSCellHandle*>::iterator itCell = mpCellHandles.begin();
		std::vector<CComVariant>::iterator itValue = mVariants.begin();
		while (itCell!=mpCellHandles.end() && itValue!=mVariants.end())
		{
			DSSCellHandle* lpCellHdl = (*itCell);
			CComVariant lVariant = (*itValue);
			hr = lpFGIterator->UpdateFieldCell(*lpCellHdl, lVariant);
			if (hr != S_OK)
				return hr;

			itCell++;
			itValue++;
		}	
	}
    
    if (!mIsFromActionObject)
    {
        mpActionObject = new DSSAODataChange(ManipulationDataChangeForTxRcdAndXTab);    //774050. We should use a new AO. But currently this AO is only used for recording manipulation type. Let's use a new AO later if there is any more usage. lcao, 20130724
    }

	return S_OK;
}

std::vector<DSSRWNode*>* DSSCManipulationDataChangeForTxRcdAndXTab::getNodesDataChanged()
{
    mNodesDataChanged.push_back(mpNode);
    return &mNodesDataChanged;
}
