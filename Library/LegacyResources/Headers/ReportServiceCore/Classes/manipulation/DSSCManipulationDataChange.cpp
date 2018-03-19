/*
 *  DSSCManipulationDataChange.cpp
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-11-11.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSCManipulationDataChange.h"
#include "DSSDocumentDefinition.h"
#include "DSSRWTemplateNode.h"
#include "DSSRWFieldGroupNode.h"
#import "DSSRWGridIterator.h"
#import "DSSRWFieldGroupIterator.h"
#include "DSSDataModelXTabImpl.h"
#import "DSSRWGridModel.h"
#include "DSSAODataChange.h"
#include "DSSRWDEngine.h"

DSSCManipulationDataChange::DSSCManipulationDataChange(DSSRWIterator* ipIterator, DSSCellHandle* ipCellHandle, CComVariant iVariant, bool iAutoRefresh)
{
	mpNode = ipIterator->getRWNode();
	mpCellHandle = ipCellHandle;
	mpIterator = ipIterator;
	mVariant = iVariant;
	mAutoRefresh = iAutoRefresh;
	mUpdateSingleCell = true;
	mIsFromActionObject = false;

    mType = ManipulationDataChange;
}

DSSCManipulationDataChange::DSSCManipulationDataChange(DSSRWIterator* ipIterator, std::vector<DSSCellHandle*>& ipCellHandles, std::vector<CComVariant>& iVariants, bool iAutoRefresh)
{
	mpNode = ipIterator->getRWNode();
	mpCellHandles = ipCellHandles;
	mpIterator = ipIterator;
	mVariants = iVariants;
	mAutoRefresh = iAutoRefresh;
	mUpdateSingleCell = false;
	mIsFromActionObject = false;

    mType = ManipulationDataChange;
}

DSSCManipulationDataChange::DSSCManipulationDataChange(DSSActionObject* ipAO, DSSObjectContext* ipContext)
{
    mType = ManipulationDataChange;

	DSSAODataChange* lpAO = static_cast<DSSAODataChange*> (ipAO);
	mpNode = ipContext->getNode(lpAO->getNodeKey());
	mAutoRefresh = lpAO->getAutoRefresh();
	mUpdateSingleCell = lpAO->getUpdateSingleCell();
	if (mUpdateSingleCell)
		mVariant = lpAO->getVariantString().c_str();
	else
	{
		std::vector<std::string>::iterator lIt;
		for (lIt = lpAO->getVariantStrings()->begin(); lIt!=lpAO->getVariantStrings()->end(); lIt++)
		{
			CComVariant lVariant((*lIt).c_str());
			mVariants.push_back(lVariant);
		}
	}

	std::string lKey = mpNode->getKey();
	std::vector<int> lNGBPath = lpAO->getNGBPath();
	mpNode->getObjectContext()->getRWDEngine()->FindIterator(lKey, lNGBPath, &mpIterator);

	if (mpIterator->getType() == DssIteratorGrid)
	{
		if (mUpdateSingleCell)
		{
			DSSCellHandle lCellHandle;
			if (lpAO->getCellHandleInfo().CellHandleType == DssCellHandleXTabHeaderElement)
				lCellHandle = ((DSSRWGridIterator*)mpIterator)->getElementHandleAt(lpAO->getCellHandleInfo().Axis-1, lpAO->getCellHandleInfo().Ordinal, lpAO->getCellHandleInfo().Depth, lpAO->getCellHandleInfo().FormID);
			else if (lpAO->getCellHandleInfo().CellHandleType == DssCellHandleXTabData)
				lCellHandle = ((DSSRWGridIterator*)mpIterator)->getCellHandleAt(lpAO->getCellHandleInfo().RowOrdinal, lpAO->getCellHandleInfo().ColOrdinal);

			mpCellHandle = &lCellHandle;
		}
		else
		{
			std::vector<DSSCellHandleInfo>::iterator it;
			for (it=lpAO->getCellHandleInfos()->begin(); it!=lpAO->getCellHandleInfos()->end(); it++)
			{
				DSSCellHandle lCellHandle;
				if (it->CellHandleType == DssCellHandleXTabHeaderElement)
					lCellHandle = ((DSSRWGridIterator*)mpIterator)->getElementHandleAt(it->Axis-1, it->Ordinal, it->Depth, it->FormID);
				else if (lpAO->getCellHandleInfo().CellHandleType == DssCellHandleXTabData)
					lCellHandle = ((DSSRWGridIterator*)mpIterator)->getCellHandleAt(it->RowOrdinal, it->ColOrdinal);

				mpCellHandles.push_back(&lCellHandle);
			}
		}
	}
	else if (mpIterator->getType() == DssIteratorFieldGroup)
	{
		if (mUpdateSingleCell)
		{
			DSSCellHandle lCellHandle = ((DSSRWFieldGroupIterator*)mpIterator)->getCellHandleAt(lpAO->getCellHandleInfo().FieldIndex, lpAO->getCellHandleInfo().RowIndexForField);
			mpCellHandle = &lCellHandle;
		}
		else
		{
			std::vector<DSSCellHandleInfo>::iterator it;
			for (it=lpAO->getCellHandleInfos()->begin(); it!=lpAO->getCellHandleInfos()->end(); it++)
			{
				DSSCellHandle lCellHandle = ((DSSRWFieldGroupIterator*)mpIterator)->getCellHandleAt(it->FieldIndex,it->RowIndexForField);
				mpCellHandles.push_back(&lCellHandle);
			}
		}
	}

	mIsFromActionObject = true;
}

DSSCManipulationDataChange::~DSSCManipulationDataChange()
{}

int DSSCManipulationDataChange::Execute()
{
    if (hCheckLayoutLoaded(mpNode)==S_FALSE)
		return S_FALSE;

	HRESULT hr = S_OK;
	if (mpIterator->getType() == DssIteratorGrid)
	{
		if (mUpdateSingleCell)
		{
			hr = static_cast<DSSRWGridIterator*> (mpIterator)->UpdateData(*mpCellHandle, mVariant);
			if (hr != S_OK)
				return hr;
		}
		else
		{
			std::vector<DSSCellHandle* >::iterator itCell = mpCellHandles.begin();
			std::vector<CComVariant>::iterator itValue = mVariants.begin();

			while ((itCell != mpCellHandles.end()) && (itValue != mVariants.end()))
			{
				DSSCellHandle *lpCellHandle = (*itCell);
				CComVariant lVariant = (*itValue);
				hr = static_cast<DSSRWGridIterator*> (mpIterator)->UpdateData(*lpCellHandle, lVariant);
				if (hr != S_OK)
					return hr;

				itCell++;
				itValue++;
			}
		}

		if (mAutoRefresh)
		{
			DSSTemplate * lpTemplate = static_cast<DSSRWTemplateNode*>(mpNode)->getTemplate();
//			DSSDataModelXTabImpl* lpDataModel = lpTemplate->getXTabModel();
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
		if (mUpdateSingleCell)
		{
			hr = static_cast<DSSRWFieldGroupIterator*> (mpIterator)->UpdateData(*mpCellHandle, mVariant);
			if (hr != S_OK)
				return hr;
		}
		else
		{
			std::vector<DSSCellHandle* >::iterator itCell = mpCellHandles.begin();
			std::vector<CComVariant>::iterator itValue = mVariants.begin();

			while ((itCell != mpCellHandles.end()) && (itValue != mVariants.end()))
			{
				DSSCellHandle *lpCellHandle = (*itCell);
				CComVariant lVariant = (*itValue);

				hr = static_cast<DSSRWFieldGroupIterator*> (mpIterator)->UpdateData(*lpCellHandle, lVariant);
				if (hr != S_OK)
					return hr;

				itCell++;
				itValue++;
			}
		}
	}

	if (!mIsFromActionObject)
	{
		mpActionObject = new DSSAODataChange();
		DSSAODataChange* lpAO = static_cast<DSSAODataChange*> (mpActionObject);
		lpAO->setNodeKey(mpNode->getKey());
		lpAO->setAutoRefresh(mAutoRefresh);
		lpAO->setUpdateSingleCell(mUpdateSingleCell);
		if (mUpdateSingleCell)
		{
			VariantChangeType(&mVariant, &mVariant, 0, VT_BSTR);
			std::string lText = MBase::WideCharToUTF8(mVariant.bstrVal).c_str();
			lpAO->setVariantString(lText);
		}
		else
		{
			std::vector<CComVariant>::iterator lIt;
			for (lIt=mVariants.begin(); lIt!=mVariants.end(); lIt++)
			{
				VariantChangeType(&(*lIt), &(*lIt), 0, VT_BSTR);
				std::string lText = MBase::WideCharToUTF8((*lIt).bstrVal).c_str();
				lpAO->AddVariantString(lText);
			}
		}

		std::vector<int> lNGBPath;
		mpIterator->getNGBPath(lNGBPath);
		lpAO->setNGBPath(lNGBPath);

		lpAO->setIteratorType(mpIterator->getType());

		if (mpIterator->getType() == DssIteratorGrid)
		{
			DSSRWGridIterator* lpGridIterator = (DSSRWGridIterator*)mpIterator;
			if (mUpdateSingleCell)
			{
				DSSCellHandleInfo lInfo;
				lInfo.CellHandleType = mpCellHandle->type;
				if (mpCellHandle->type==DssCellHandleXTabHeaderElement)
				{
					lInfo.Axis = lpGridIterator->getAxisFromHandle(*mpCellHandle);
					lInfo.Ordinal = lpGridIterator->getOrdinalFromHandle(*mpCellHandle);
					lInfo.Depth = lpGridIterator->getDepthFromHandle(*mpCellHandle);
					lInfo.FormID = lpGridIterator->getFormIDFromHandle(*mpCellHandle);
				}
				else if (mpCellHandle->type==DssCellHandleXTabData)
				{
					lpGridIterator->getOrdinalFromHandle(*mpCellHandle, lInfo.RowOrdinal, lInfo.ColOrdinal);
				}
				lpAO->setCellHandleInfo(lInfo);
			}
			else
			{
				std::vector<DSSCellHandle*>::iterator it;
				for (it=mpCellHandles.begin(); it!=mpCellHandles.end(); it++)
				{
					DSSCellHandleInfo lInfo;
					lInfo.CellHandleType = (*it)->type;
					if ((*it)->type==DssCellHandleXTabHeaderElement)
					{
						lInfo.Axis = lpGridIterator->getAxisFromHandle(**it);
						lInfo.Ordinal = lpGridIterator->getOrdinalFromHandle(**it);
						lInfo.Depth = lpGridIterator->getDepthFromHandle(**it);
						lInfo.FormID = lpGridIterator->getFormIDFromHandle(**it);
					}
					else if ((*it)->type==DssCellHandleXTabData)
					{
						lpGridIterator->getOrdinalFromHandle(**it, lInfo.RowOrdinal, lInfo.ColOrdinal);
					}
					lpAO->AddCellHandleInfo(lInfo);
				}
			}

		}
		else if (mpIterator->getType() == DssIteratorFieldGroup)
		{
			DSSRWFieldGroupIterator* lpFieldGroupIterator = (DSSRWFieldGroupIterator*)mpIterator;
			if (mUpdateSingleCell)
			{
				DSSCellHandleInfo lInfo;
				lInfo.FieldIndex = lpFieldGroupIterator->getFieldIndexFromHandle(*mpCellHandle);
				lInfo.RowIndexForField = lpFieldGroupIterator->getRowIndexFromHandle(*mpCellHandle);
				lpAO->setCellHandleInfo(lInfo);
			}
			else
			{
				std::vector<DSSCellHandle*>::iterator it;
				for (it=mpCellHandles.begin(); it!=mpCellHandles.end(); it++)
				{
					DSSCellHandleInfo lInfo;
					lInfo.FieldIndex = lpFieldGroupIterator->getFieldIndexFromHandle(**it);
					lInfo.RowIndexForField = lpFieldGroupIterator->getRowIndexFromHandle(**it);
					lpAO->AddCellHandleInfo(lInfo);
				}
			}
		}
	}

	mNodesDataChanged.push_back(mpNode);

	return S_OK;
}
