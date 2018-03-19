/*
 *  DSSRWNodeModelIterator.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 9/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#include "DSSRWNodeModelIterator.h"
#include "DSSRWSectionModelIterator.h"
#include "DSSRWSectionNode.h"

DSSRWNodeModelIterator::DSSRWNodeModelIterator():mpDataModel(NULL),mpNode(NULL),mpNodeModel(NULL),
mpParentIterator(NULL),mpObjectContext(NULL),mGlobalLookupVer(-1)
{
}

DSSRWNodeModelIterator::~DSSRWNodeModelIterator()
{
}

int DSSRWNodeModelIterator::Init(DSSRWDataModel *ipDataModel, DSSRWNode *ipNode, DSSRWNodeModel *ipNodeModel, DSSRWSectionModelIterator *ipParentIterator)
{
    mpDataModel = ipDataModel;
    mpNode = ipNode;
    mpNodeModel = ipNodeModel;
    mpParentIterator = ipParentIterator;
    
    return S_OK;
}


int DSSRWNodeModelIterator::Init(DSSRWNode* ipNode, DSSRWSectionModelIterator * ipParent, vector<DSSBaseElementProxy*>& ivNGBElements)
{
    int hr = S_OK;
    return hr;
}

int DSSRWNodeModelIterator::ReInit()
{
    int hr = S_OK;
    return hr;
}

DSSRWNode* DSSRWNodeModelIterator::getRWNode()
{
    return mpNode;
}

EnumDSSIteratorType DSSRWNodeModelIterator::getType()
{
    return mType;
}


int DSSRWNodeModelIterator::getCommandList(DSSCellHandle &irHandle, vector<EnumManipulationCommandTypes> &orvCommand)
{
    int hr = S_OK;
    return hr;
}

DSSBaseElementProxy * DSSRWNodeModelIterator::createElementFromHandle(DSSCellHandle &irHandle)
{
    return NULL;
}

int DSSRWNodeModelIterator::createElementsFromHandle(DSSCellHandle &irHandle, int iAxisIndex, vector<DSSBaseElementProxy *> &orvpElements)
{
    int hr = S_OK;
    return hr;
}

int DSSRWNodeModelIterator::createSiblingElementsFromHandle(DSSCellHandle &irHandle, bool iParentElementsOnly, vector<DSSBaseElementProxy *> &orvpElements)
{
    return E_NOTIMPL;
}

EnumDSSAxisName DSSRWNodeModelIterator::getAxisFromHandle(DSSCellHandle &irHandle)
{
    return DssAxisReserved;
}

int DSSRWNodeModelIterator::getFormFromHandle(DSSCellHandle &irHandle, GUID &orGUID)
{
    return E_NOTIMPL;
}

bool DSSRWNodeModelIterator::getUCWarning()
{
    return false;
}

void DSSRWNodeModelIterator::UpdateNGBElemets(vector<DSSBaseElementProxy*>& ivNGBElements)
{
    
}

int DSSRWNodeModelIterator::UpdateData(DSSCellHandle &irHandle, CComVariant &irNewValue)
{
    return E_NOTIMPL;
}

int DSSRWNodeModelIterator::DeleteData(DSSCellHandle &irHandle)
{
    return E_NOTIMPL;
}

int DSSRWNodeModelIterator::InsertData(vector<std::string> &irNewValue)
{
    return E_NOTIMPL;
}

int DSSRWNodeModelIterator::UndoDataChange(DSSCellHandle &irHandle)
{
    return E_NOTIMPL;
}

DSSDataModelTransaction* DSSRWNodeModelIterator::getTransactionDataModel()
{
    return NULL;
}

int DSSRWNodeModelIterator::getTransactionXML(std::string &orXMLContent)
{
    return E_NOTIMPL;
}

int DSSRWNodeModelIterator::setAllCommit()
{
    return E_NOTIMPL;
}

int DSSRWNodeModelIterator::MarkRows(int iRowOrdinal, EnumDSSRWMarkRowAction iActionType)
{
    return E_NOTIMPL;
}

int DSSRWNodeModelIterator::getDataChangeType(DSSCellHandle &irHandle, bool ibTrxSubPreset)
{
    return DssTransactionDataReserved;
}

bool DSSRWNodeModelIterator::HasDataChanged()
{
    return false;
}

void DSSRWNodeModelIterator::ReleaseUnusedMemory()
{
    
}

int DSSRWNodeModelIterator::getGroupbyPaths(DSSDataModelBase *ipModel, vector<int> &orGBPaths, int &ornPaths, int &ornGroupbyUnits, vector<GUID> &irvUnitGUID, bool ibIncludeSelfNode, bool ibExcludeUC)
{
    return E_NOTIMPL;
}


int DSSRWNodeModelIterator::getGroupbyPaths2(DSSDataModelBase *ipModel, vector<int> &orGBPaths, int &ornPaths, int &ornGroupbyUnits, vector<GUID> &irvUnitGUID, bool ibIncludeSelfNode, bool ibExcludeUC, bool &obIsNotIn, bool ibReplaceWithAll)
{
    return E_NOTIMPL;
}

int DSSRWNodeModelIterator::hGetCurentElements(DSSDataModelBase *ipModel, vector<vector<GBElement> > &orGBElements, vector<DSSTabularUnit*> &orvTabularUnits, bool& obIsNotIn, bool ibReplaceWithAll, bool ibIncludeSelf, bool ibExcludeUC)
{
    return E_NOTIMPL;
}

//junwang, new rwd, 12/27/2011
void DSSRWNodeModelIterator::getNonTrivialParents(vector<DSSRWSectionModelIterator *> &ovParentIterators)
{
    vector<DSSRWSectionModelIterator*> lvParents;
    DSSRWSectionModelIterator* lpParent = mpParentIterator;
    
    while (lpParent!=NULL)
    {
        EnumDSSRWSectionType lSectionType = (EnumDSSRWSectionType)lpParent->getSectionType();
        
        if(lSectionType == DssRWSectionGroupBySection)
            lvParents.push_back(lpParent);
        else if(lSectionType == DssRWSectionControlGroupBySection
                ||lSectionType == DssRWSectionUnitCondition)
        {
            //junwang, 2012/03/21, handle no-source CGB case
            DSSRWSectionNode* lpThisNode = dynamic_cast<DSSRWSectionNode*>(static_cast<DSSRWNodeModelIterator*>(lpParent)->getRWNode());
            if(lpThisNode->getGroupByUnits()&&lpThisNode->getGroupByUnits()->Count()>0)
                lvParents.push_back(lpParent);
        }
        lpParent = ((DSSRWNodeModelIterator*)lpParent)->mpParentIterator;
    }
    
    ovParentIterators.assign(lvParents.rbegin(), lvParents.rend());
}

DSSRWNodeModel* DSSRWNodeModelIterator::getNodeModel(bool iFromDataModel)
{
    if(mpNodeModel)
    {
        return mpNodeModel;
    }
    
    if(iFromDataModel && mpDataModel)
    {
        return mpDataModel->getNodeModel(mNodeKey);
    }

    return NULL;
}