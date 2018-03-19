/*
 *  DSSRWSectionModelIterator.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 9/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#include "DSSRWSectionModelIterator.h"
#include "DSSRWDataModel.h"
#include "DSSRWNode.h"
#include "DSSRWSectionNode.h"
#include "DSSRWSectionModel.h"
#include "DSSAEHelper.h"
#include "DSSGroupByUnit.h"
#include "DSSCExpression.h"
#include "DSSExpNode.h"
#include "DSSExpNodeOperator.h"
#include "DSSExpNodeElementsObject.h"
#include "DSSRWGroupbyIterator.h"
#include "DSSDocumentDefinition.h"
#include "DSSRWFieldGroupModelIterator.h"
#include "DSSRWControlModelIterator.h"
#include "DSSRWGridModel.h"
#include "DSSTabularAttribute.h"
#include "DSSSubtotalElementProxy.h"
#include "DSSRWCompare.h"
#include "DSSSort_includes.h"
#include "DSSDataColumn.h"
#include "DSSConsolidation.h"
#include "DSSTabularConsolidation.h"
#include "DSSCustomGroup.h"
#include "DSSTabularCustomGroup.h"
#include "DSSGUnitContainer.h"
#import "DSSDataModelGroupbyImpl.h"
#include "DSSRWGridIterator.h"

DSSRWSectionModelIterator::DSSRWSectionModelIterator():
mpCandiateInBase(NULL),mLevel(-1),mNGBChildBranch(0),mbEmptySelectFlag(false),mbGotAutoPickProperty(false),mbAutoPickTopmost(false),mbSkip(false)
{
    
}


DSSRWSectionModelIterator::~DSSRWSectionModelIterator()
{
    if(mpDataModel)
    {
        mpDataModel->removeNodeKey2Iterator(DSSRWNodeModelIterator::mNodeKey, this);
    }
}

int DSSRWSectionModelIterator::init(DSSRWDataModel *ipDataModel, DSSRWNode *ipNode, DSSRWNodeModel *ipModel, DSSRWSectionModelIterator *ipParent, bool isAll)
{
    
    DSSRWNodeModelIterator::Init(ipDataModel,ipNode,ipModel,ipParent);
    
    mpDataModel->addNodeKey2Iterator(ipNode->getKey(), this);
    mGlobalLookupVer = mpDataModel->getGlobalLookupVer();
    //junwang, 12/14/2011, new rwd for NGB All
    if(ipParent)
        mNGBChildBranch = ipParent->getCurrentBranch();
    
    mnChildren = static_cast<DSSRWSectionNode*>(ipNode)->Count();
	mvChildren.resize(mnChildren);
	
	mSectionType = static_cast<DSSRWSectionNode*>(ipNode)->getSectionType();
    
	if (mSectionType != DssRWSectionGroupBySection)
	{
		for(int i = 0; i < mnChildren; i++)
		{
			mvChildren[i].resize(1);
			mvChildren[i][0] = NULL;
		}
	}
    
    //initialize base class DSSRWSectionIterator
    DSSRWSectionIterator::mType = DssIteratorSection;
    DSSRWSectionIterator::mpParentIterator = dynamic_cast<DSSRWSectionIterator*>(ipParent);
    DSSRWSectionIterator::mNodeKey = ipNode->getKey();
    DSSRWSectionIterator::mpObjectContext = ipNode->getObjectContext();
    
    DSSRWNodeModelIterator::mType = DssIteratorSection;
    DSSRWNodeModelIterator::mNodeKey = ipNode->getKey();
    
    
    return ReInit(true, isAll);
}

int DSSRWSectionModelIterator::ReInit(bool DeleteChildren , bool iIsAll)
{
    mbEmptySelectFlag=false;
    
    bool lbGlobalLookupChanged = false;
    if(mGlobalLookupVer != mpDataModel->getGlobalLookupVer())
    {
        mGlobalLookupVer = mpDataModel->getGlobalLookupVer();
        lbGlobalLookupChanged = true;
    }
    
    DSSRWSectionModelIterator *lpParentSectionIterator=dynamic_cast<DSSRWSectionModelIterator*>(DSSRWSectionIterator::mpParentIterator);
    
    if(lpParentSectionIterator)
    {
        if(lpParentSectionIterator->getEmptySelectFlag())
            mbEmptySelectFlag=true;
    }
    
    //zhyang, 01/11/2012
    bool lbUpdated = false;
    int hr = mpDataModel->updateNodeModel(DSSRWNodeModelIterator::mNodeKey, lbUpdated);
    CHECK_HR
    
    DSSRWNodeModel* lpChildNodeModel = mpDataModel->getNodeModel(DSSRWNodeModelIterator::mNodeKey);
    mpNodeModel = lpChildNodeModel;
    
    mIsReplaced = false;
	mCurrent = 0;
	mIsAll=iIsAll;
	
	for (int i = 0; i < mvReplacedElements.size(); i++)
	{
		if (mvReplacedElements[i])
			delete mvReplacedElements[i];
	}
	mvReplacedElements.clear();
    
	DSSRWSectionNode * lpThisNode = static_cast<DSSRWSectionNode*>(DSSRWNodeModelIterator::getRWNode());
    
	_ASSERT(lpThisNode);
    
    int lParentNGB = 0;
    int lLevel = -1;
    
    msCurrentMetrics.clear();
    msGroupByMetrics.clear();
    
    DSSRWSectionModelIterator* lpParentIterator = DSSRWNodeModelIterator::mpParentIterator;
    
    if(lpParentIterator)
    {
        lParentNGB = lpParentIterator->GetNGBSelectedRow();
        if (lpParentIterator->mSectionType == DssRWSectionGroupBySection)
        {
            lLevel = lpParentIterator->getLevel();
        }
    }
    mParentNGB = lParentNGB;
    
    if(lpParentIterator)
    {
        lpParentIterator->getGroupByElements(mvGroupByElements,mNGBChildBranch);
        lpParentIterator->getGroupByMetrics(msGroupByMetrics);
    }
    
    if (mpNodeModel)
    {
        if (mSectionType == DssRWSectionGroupBySection)
        {
            hr = static_cast<DSSRWSectionModel*>(mpNodeModel)->GetCandidateElements(lParentNGB, lLevel+1, mvCandidateElements);
        }
        else
            hr = static_cast<DSSRWSectionModel*>(mpNodeModel)->GetCandidateElements(lpParentIterator, mvGroupByElements, mvCandidateElements);
        
        CHECK_HR;
    }
    
    DSSGroupByUnits* lpGroupByUnits = lpThisNode->getGroupByUnits();
    DSSGroupByUnit* lpUnit = NULL;
    int lnUnit = 0;
    if (lpGroupByUnits)
    {
        lnUnit = lpGroupByUnits->Count();
        if(lnUnit>0)
        {
            lpUnit = lpGroupByUnits->Item(0);
            if (lpUnit->HasSubtotal())
            {
                mvDefTags.push_back(1);  //for subtotal
            }
        }
    }
    
    mvSelectedElements.clear();
    
    //NGB
    if (mSectionType == DssRWSectionGroupBySection)
    {
        mLevel = lLevel+1;
        GUID lID = lpUnit->getObjectID();
        mPageByStyle = lpThisNode->getPageByStyle();
        
        //step 1: calculate global keys
        mvRow.assign(mvCandidateElements.begin(), mvCandidateElements.end());
        NGBPathInfo* lpNGBPath = mpDataModel->getNGBPathInfo(lpThisNode->getKey());
        for(int i=0; i<mvRow.size(); i++)
        {
            int lGlobalKey = -1;
            lpNGBPath->GetNGBKey(mvRow[i]-1, mLevel, &lGlobalKey);
            mvCandidateElements[i] = lGlobalKey;
        }
        
        bool lfRemoveNull = mpDataModel->get_RemoveNullProperty();
        if(lfRemoveNull)
        {
            for(int i=0; i<mvCandidateElements.size(); i++)
            {
                bool lfIsCompoundNull = false;
                mpDataModel->IsCompoundNull(lID, mvCandidateElements[i], &lfIsCompoundNull);
                if(lfIsCompoundNull)
                {
                    mvCandidateElements.erase(mvCandidateElements.begin()+i);
                    mvRow.erase(mvRow.begin()+i);
                    break;
                }
            }
        }
        
        //step 2, calculate current elements
        //lvCurrentElements stores the element index for mvCandidateElements, 1-based
        vector<int> lvCurrentElements;
        
        //special handle for parent NGB all
        if(lpParentIterator&&lpParentIterator->isAll())
        {
            mPageByStyle = DssRWPageByStyleAll;
            mIsAll = true;
            lvCurrentElements.push_back(0);
            hSortCandidateElements();
        }
        else
            hGetCurrentElements(lvCurrentElements);
        
        /*
         //step 2: NGB sorting
         if(lnRow>1)
         {
         //TODO: NGB sorting
         DSSRWSectionCompare lCMP;
         lCMP.Init(this, mpDataModel->GetDisplayNullOnTop());
         
         int* lpIndex = new int[lnRow];
         int* lpTemp = new int[lnRow];
         
         for (int i=0; i<lnRow; i++)
         {
         lpIndex[i] = i;
         }
         
         _xtab_sort(lpIndex,lpIndex+lnRow,lCMP);
         
         for (int i=0; i<lnRow; i++)
         {
         lpTemp[i] = mvRow[lpIndex[i]];
         }
         memmove(&mvRow[0],lpTemp,sizeof(int)*lnRow);
         for (int i=0; i<lnRow; i++)
         {
         lpTemp[i] = mvCandidateElements[lpIndex[i]];
         }
         memmove(&mvCandidateElements[0], lpTemp, sizeof(int)*lnRow);
         
         delete[] lpIndex;
         delete[] lpTemp;
         }
         */
        
        
        int lnRow = mvRow.size();
        mCurrent = -1;
        if(lvCurrentElements.size()>0)
        {
            mCurrent = 0;
            if(lvCurrentElements[0] == 0) //All
            {
                mIsAll = true;
            }
            else
            {
                mIsAll = false;
                mvSelectedElements.resize(1);
                mvSelectedElements[0] = lvCurrentElements[0]-1;
            }
        }
        else
        {
            CComVariant* lpAutoSelectProp = lpThisNode->getObjectContext()->getDocumentDefinition()->getPropertyValue("AnalyticalEngineProperties", "AutoSelectionForUnset");
            int lUnsetOption = 0;
            
            if (lpAutoSelectProp)
                lUnsetOption = lpAutoSelectProp->lVal;
            
            if ((mPageByStyle == DssRWPageByStyleBoth || mPageByStyle == DssRWPageByStyleIndividual) && lnRow > 0)
            {
                mIsAll = false;
                mvSelectedElements.resize(1);
                mCurrent = 0;
                if (lUnsetOption == DssAEUnsetTypeChooseLast)
                {
                    mvSelectedElements[0] = lnRow - 1;
                }
                else
                {
                    mvSelectedElements[0] = 0;
                }
            }
            else if (mPageByStyle != DssRWPageByStyleIndividual)
            {
                mIsAll = true;
                mCurrent = 0;
            }
            else
            {
                mIsAll = true;
            }
        }
        
        if (DeleteChildren)
        {
            int size = mvCandidateElements.size();
            if (size == 0)
            {
                //tliao, 771763, empty NGB
                size = 1;
                //tliao, 774061, clear gb elements
                for (int i = 0; i < mvGroupByElements.size(); i++)
                    mvGroupByElements[i].clear();
            }
            
            for (int j = 0; j < mnChildren; j++)
			{
				// reinit mvchildren for gb.
				for (int k = 0; k < mvChildren[j].size(); k++)
				{
					if (mvChildren[j][k])
						delete mvChildren[j][k];
				}
				mvChildren[j].clear();
				
                mvChildren[j].resize(size);
                for (int k=0; k<size; k++)
                {
                    mvChildren[j][k] = NULL;
                }
			}
            if (mpSpecialFieldGroupIterator)
            {
                delete mpSpecialFieldGroupIterator;
                mpSpecialFieldGroupIterator = NULL;
            }
        }
        else
        {
            int size = mvCandidateElements.size();
            if (size == 0)
            {
                //tliao, 2014/07/23, clear gb elements
                for (int i = 0; i < mvGroupByElements.size(); i++)
                    mvGroupByElements[i].clear();
            }
            
			for (int j = 0; j < mnChildren; j++)
			{
				if ( (mvChildren[j].size() > 0) && (mvChildren[j][0]) )
				{
                    if (mvChildren[j][0]->getType() == DssIteratorSection)
                    {
                        dynamic_cast<DSSRWSectionModelIterator*>(mvChildren[j][0])->updateGroupbyElements(mvGroupByElements);
                        dynamic_cast<DSSRWSectionModelIterator*>(mvChildren[j][0])->updateGroupbyMetrics(msGroupByMetrics);
                    }
				}
                if(lbGlobalLookupChanged)
                {
                    if(mvRow.size() > mvChildren[j].size())
                    {
                        mvChildren[j].resize(mvRow.size());
                    }
                }
			}
        }
    }//end of NGB
    else
    {
        //CGB or UC
        if (mSectionType == DssRWSectionControlGroupBySection||
            mSectionType == DssRWSectionUnitCondition)
        {
            
            EnumDSSCurrentElementStatus lStatus = DssCurrentElementReserved;
            
            //junwang, 2012/03/21, #547222, empty source CGB selector
            if(lpUnit)
                lStatus = lpUnit->getCurrentElementsStatus();
            
            if (mSectionType == DssRWSectionUnitCondition)
            {
                if(lStatus == DssCurrentElementUnset)
                {
                    mIsReplaced = true;
                    mvReplacedElements.push_back(new DSSAllElementProxy(mpDataModel->getObjectContext()));
                }
            }

            hr = hGetCurrentElements(mvSelectedElements);
            
            CHECK_HR;

            if (mbSkip)
            {
                return S_OK;
            }
            /*
             int lnRow = mvCandidateElements.size();
             
             if(lnRow>1)
             {
             //TODO: NGB sorting
             DSSRWSectionCompare lCMP;
             lCMP.Init(this, mpDataModel->GetDisplayNullOnTop());
             
             int* lpIndex = new int[lnRow];
             int* lpTemp = new int[lnRow];
             
             for (int i=0; i<lnRow; i++)
             {
             lpIndex[i] = i;
             }
             
             _xtab_sort(lpIndex,lpIndex+lnRow,lCMP);
             
             for (int i=0; i<lnRow; i++)
             {
             lpTemp[i] = mvCandidateElements[lpIndex[i]];
             }
             memmove(&mvCandidateElements[0],lpTemp,sizeof(int)*lnRow);
             
             delete[] lpIndex;
             delete[] lpTemp;
             }
             hGetCurrentElements(mvCandidateElements, -1, mvSelectedElements);
             */
        }
        else //other, non NGB, CGB and UC
        {
            if(mvCandidateElements.size()>0)
                mvSelectedElements.push_back(0);
        }
    }
    
    if (mSectionType != DssRWSectionGroupBySection)
	{
        for (int j = 0; j < mnChildren; j++)
		{
			if ( (mvChildren[j].size() > 0) && (mvChildren[j][0]) )
			{
                //03/02/2012, #548480
                if (mvChildren[j][0]->getType() == DssIteratorSection)
                {
                    DSSRWSectionModelIterator* lpSecChild = dynamic_cast<DSSRWSectionModelIterator*>(mvChildren[j][0]);
                    lpSecChild->updateGroupbyElements(mvGroupByElements);
                    lpSecChild->updateGroupbyMetrics(msGroupByMetrics);
                }
			}
		}
        
		if (mpSpecialFieldGroupIterator)
		{
			hr = mpSpecialFieldGroupIterator->ReInit();
			CHECK_HR;
		}
	}
    
    
    
    //TQMS 723014,hyan,2/28/2013
    //we need to update the child section model iterator
    hr = hRefreshChildIterators();
    CHECK_HR;
    
    return 0;
}

int DSSRWSectionModelIterator::hRefreshChildIterators()
{
    int hr = S_OK;
    
    for(int i=0;i<mnChildren;i++)
    {
        int lnSize=mvChildren[i].size();

        for(int j=0;j<lnSize;j++)
        {
            if(mvChildren[i][j])
            {
                if(mvChildren[i][j]->getType()==DssIteratorSection)
                {
                    DSSRWSectionModelIterator* lpSecChild = dynamic_cast<DSSRWSectionModelIterator*>(mvChildren[i][j]);
                    
                    if(lpSecChild)
                    {
                        hr = lpSecChild->ReInit();
                        CHECK_HR;
                    }
                }
            }
        }
    }
    
    return S_OK;
}

// weilin US51811 2016/07/20 refresh child grid iterators
int DSSRWSectionModelIterator::RefreshChildGridIterators()
{
    int hr = S_OK;

    for(int i=0;i<mnChildren;i++)
    {
        int lnSize=mvChildren[i].size();

        for(int j=0;j<lnSize;j++)
        {
            if(mvChildren[i][j])
            {
                if(mvChildren[i][j]->getType()==DssIteratorGrid)
                {
                    DSSRWGridIterator* lpGridChild = dynamic_cast<DSSRWGridIterator *>(mvChildren[i][j]);
                    
                    if(lpGridChild)
                    {
                        hr = lpGridChild->ReInit();
                        CHECK_HR;
                    }
                }
            }
        }
    }

    return S_OK;
}

// weilin US51811 2016/07/19 replay autopick and refresh child iterator
int DSSRWSectionModelIterator::ReplayAutoPickAndRefreshChildIterator()
{
    int hr = hCalDefaultElementsCGB(mvCandidateElements, mvSelectedElements);
    CHECK_HR

    if (mSectionType != DssRWSectionGroupBySection)
    {
        for (int j = 0; j < mnChildren; j++)
        {
            if ( (mvChildren[j].size() > 0) && (mvChildren[j][0]) )
            {
                //03/02/2012, #548480
                if (mvChildren[j][0]->getType() == DssIteratorSection)
                {
                    DSSRWSectionModelIterator* lpSecChild = dynamic_cast<DSSRWSectionModelIterator*>(mvChildren[j][0]);
                    lpSecChild->updateGroupbyElements(mvGroupByElements);
                    lpSecChild->updateGroupbyMetrics(msGroupByMetrics);
                }
            }
        }

        if (mpSpecialFieldGroupIterator)
        {
            hr = mpSpecialFieldGroupIterator->ReInit();
            CHECK_HR
        }
    }

    hr = hRefreshChildIterators();
    CHECK_HR;
}

// weilin US51811
bool DSSRWSectionModelIterator::getSkip()
{
    return mbSkip;
}
void DSSRWSectionModelIterator::setSkip(bool ibTemp)
{
    mbSkip = ibTemp;
}

//hyan,2/22/2013
int DSSRWSectionModelIterator::getNGBIndexRowCount()
{
    if (mSectionType == DssRWSectionGroupBySection)
    {
        DSSRWSectionNode * lpThisNode = static_cast<DSSRWSectionNode*>(DSSRWNodeModelIterator::getRWNode());
        
        NGBPathInfo* lpNGBPath = mpDataModel->getNGBPathInfo(lpThisNode->getKey());
        //tliao, 4/21/2014, 880575, it is possible that there is no NGBPathInfo
        if (!lpNGBPath)
        {
            return 0;
        }
        
        if (mLevel == 0)
            return lpNGBPath->getNGBIndexRowCount();
        
        int lRowCount = 0;
        int hr = lpNGBPath->GetRowCountByNGBPath(mParentNGB, mLevel, lRowCount);
        CHECK_HR;
        
        return lRowCount;
    }
    
    return 1;
}

int DSSRWSectionModelIterator::SetNGBWindow(int iStart, int iEnd)
{
    if (iStart < 0 || iStart > iEnd)
        return E_INVALIDARG;
    
    int hr = S_OK;
    DSSRWSectionNode * lpThisNode = static_cast<DSSRWSectionNode*>(DSSRWNodeModelIterator::getRWNode());
    NGBPathInfo* lpNGBPath = mpDataModel->getNGBPathInfo(lpThisNode->getKey());
    //tliao, 4/21/2014, 880575, it is possible that there is no NGBPathInfo
    if (!lpNGBPath)
    {
        return S_OK;
    }
    if (mLevel == 0)
    {
        if (iEnd >= lpNGBPath->getNGBIndexRowCount())
            return S_FALSE;
        
        hr = mpDataModel->SetNGBWindow(lpThisNode->getKey(), iStart, iEnd);
        CHECK_HR;
    }
    else
    {
        int lRowCount = 0;
        int hr = lpNGBPath->GetRowCountByNGBPath(mParentNGB, mLevel, lRowCount);
        CHECK_HR;
        
        if(lRowCount <= iEnd-iStart)
            return S_FALSE;
        
        hr = mpDataModel->SetNGBWindow(lpThisNode->getKey(), mParentNGB+iStart, mParentNGB+iEnd);
        CHECK_HR;
    }
    
    return ReInit(true, false);
}

int DSSRWSectionModelIterator::ClearNGBWindow(bool DeleteChildren)
{
    DSSRWSectionNode * lpThisNode = static_cast<DSSRWSectionNode*>(DSSRWNodeModelIterator::getRWNode());
    if (!mpDataModel->ContainsNGBWindow(lpThisNode->getKey()))  //no NGB window, doesn't need to clear
        return S_OK;
    int hr = mpDataModel->ClearNGBWindow(lpThisNode->getKey());
    CHECK_HR;
    
    return ReInit(DeleteChildren, false);
}

bool DSSRWSectionModelIterator::IsNGBAll()
{
    return mIsAll;
}

bool DSSRWSectionModelIterator::getEmptySelectFlag()
{
    return mbEmptySelectFlag;
}

int DSSRWSectionModelIterator::Item(std::string iNodeKey, DSSRWIterator **oppIterator)
{
    *oppIterator = NULL;
    int index = -1;
    
    for (int i=0; i<mpNode->Count(); i++)
    {
        DSSRWNode* lpNode = mpNode->Item(0);
        if (lpNode&&lpNode->getKey()==iNodeKey)
        {
            index = i; break;
        }
    }
    
    return Item(index, oppIterator);
}


int DSSRWSectionModelIterator::Item(int iChild, DSSRWIterator** oppIterator, bool* opbNew)
{
    if (iChild < 0 || iChild >= mnChildren)
		return E_INVALIDINDEX;
    
	if (mCurrent >= mvChildren[iChild].size())
		return E_INVALIDARG;
    
	int hr = S_OK;
    
	if (!mvChildren[iChild][mCurrent])
	{
		DSSRWNode * lpThisNode = mpNode;
		if (!lpThisNode)
			return E_FAIL;
		
		DSSRWNode *lpChildNode = lpThisNode->Item(iChild);
		if (!lpChildNode)
			return E_FAIL;
		
        string lNodeKey = lpChildNode->getKey();
        
        
        //zhyang, 01/11/2012
        bool lbUpdated = false;
        hr = mpDataModel->updateNodeModel(lNodeKey, lbUpdated);
        CHECK_HR;
        
        DSSRWNodeModel* lpChildNodeModel = mpDataModel->getNodeModel(lNodeKey);
        
		DSSRWNodeModelIterator *lpChildIterator = NULL;
        DSSRWIterator* lpBaseIterator = NULL;
        
        if(lpChildNodeModel)
        {
			if(opbNew)
				*opbNew = true;
            switch (lpChildNodeModel->getNodeType())
            {
                case DssRWNodeSection:
                    hr = lpChildNodeModel->createIterator(this, &lpChildIterator);//
                    CHECK_HR;
                    
                    lpBaseIterator = (DSSRWIterator*)(static_cast<DSSRWSectionModelIterator*>(lpChildIterator));
                    break;
                case DssRWNodeControl:
                    hr = lpChildNodeModel->createIterator(this, &lpChildIterator);//
                    CHECK_HR;
                    
                    lpBaseIterator = (DSSRWIterator*)(static_cast<DSSRWControlModelIterator*>(lpChildIterator));
                    break;
                case DssRWNodeTemplate:
                {
                    DSSRWGridIterator *lpGridIterator = NULL;
                    hr = ((DSSRWGridModel *)lpChildNodeModel)->createIterator(this, &lpGridIterator);
                    CHECK_HR;
                    
                    lpBaseIterator = (DSSRWIterator *)lpGridIterator;
                }
                    break;
                case DssRWNodeFieldGroup:
                {
                    hr = lpChildNodeModel->createIterator(this, &lpChildIterator);
                    CHECK_HR;
                    
                    lpBaseIterator = (DSSRWIterator*)(static_cast<DSSRWFieldGroupModelIterator*>(lpChildIterator));
                }
                    break;
                default:
                    break;
            }
        }
        else
        {
            //683110, android workflow is different from ios, after the first manipulation it will try to refresh the iterators of all the DSSRWNodes in the RWD, however, if some nodes are designed in a info window which doesn't shown up during initial execution, the model of the template node or FG node will be empty, so if we don't check the type of the node, it will always create a section iterator for those template/FG nodes
            
            // 738305, 4/10/2013, tehe: for ios, we need the same logic. we may not find the control model during initial loading, maybe it's caused by the we can't see the control
            if(lpChildNode->getType() != DssRWNodeSection)
            {
                
                lpBaseIterator = NULL;
                // tehe: support emtpy iterator for empty grid
                switch (lpChildNode->getType())
                {
                    case DssRWNodeTemplate:
                    {
						if(opbNew)
							*opbNew = true;
                        
						DSSRWGridIterator *lpGridIterator = new DSSRWGridIterator();
						hr = lpGridIterator->Init(mpDataModel, NULL, lpChildNode, this);
						CHECK_HR;
                        
						lpBaseIterator = lpGridIterator;
                    }
                        break;
                        
                    default:
                        break;
                }
            }
            else
            {
				if(opbNew)
					*opbNew = true;
                lpChildIterator = new DSSRWSectionModelIterator();
                static_cast<DSSRWSectionModelIterator*>(lpChildIterator)->init(mpDataModel,lpChildNode,static_cast<DSSRWSectionModel*>(lpChildNodeModel),this);
                lpBaseIterator = (DSSRWIterator*)(static_cast<DSSRWSectionModelIterator*>(lpChildIterator));
            }
        }
		
        if (lpBaseIterator)
		{
            mvChildren[iChild][mCurrent] = lpBaseIterator;
            
			if (hr == E_OUT_OF_MEMORY)
			{
				if (oppIterator)
					*oppIterator = mvChildren[iChild][mCurrent];
				return hr;
			}
		}
	}
	if (oppIterator)
		*oppIterator = mvChildren[iChild][mCurrent];
    else
        return S_FALSE;
    
	//hyan, 702836, 2/4/2013
	//to avoid many crash in Android, we set the return value to S_FALSE when child is NULL
	if(*oppIterator==NULL)
		return S_FALSE;
    
	return hr;
    
}

int DSSRWSectionModelIterator::getCommandList(DSSCellHandle &irHandle, vector<EnumManipulationCommandTypes> &orvCommand)
{
    return S_OK;
}

int DSSRWSectionModelIterator::getGroupByElements(vector<vector<int> > &ovvGroupByElements)
{
    
    ovvGroupByElements = mvGroupByElements;
    if (mpNodeModel&&mSectionType!=DssRWSectionUnitCondition)
    {
        //ignore if it is panel stack selector or have no source
        
        DSSGroupByUnits* lpGroupByUnits = static_cast<DSSRWSectionNode*>(mpNode)->getGroupByUnits();
        
        if (lpGroupByUnits==NULL)
            return S_OK;
        
        int lNumUnit = lpGroupByUnits->Count();
        
        // #540668 tehe we can't filter the metric selector
        if(lNumUnit == 0 /*|| lpGroupByUnits->Item(0)->getObjectID() == GUID_NULL*/) // metrics selector
            return 0;
        
        int lIndex = ovvGroupByElements.size();
        ovvGroupByElements.resize(lIndex+1);
        for (int i=0; i<mvSelectedElements.size(); i++)
        {
            ovvGroupByElements[lIndex].push_back(mvCandidateElements[mvSelectedElements[i]]);
        }
    }
    
    return 0;
}

int DSSRWSectionModelIterator::getGroupByMetrics(set<GUID, less<GUID> > &osGroupByMetrics)
{
    if (msGroupByMetrics.size()>0)
    {
        std::copy(msGroupByMetrics.begin(), msGroupByMetrics.end(), std::inserter(osGroupByMetrics, osGroupByMetrics.begin()));
    }
    
    if (mpNodeModel&&msCurrentMetrics.size()>0)
    {
        std::copy(msCurrentMetrics.begin(), msCurrentMetrics.end(), std::inserter(osGroupByMetrics, osGroupByMetrics.begin()));
    }
    return 0;
}

int DSSRWSectionModelIterator::getSelectedElements(std::vector<int> &ovSelectedElements)
{
    
    ovSelectedElements.resize(mvSelectedElements.size());
    for (int i=0; i<mvSelectedElements.size(); i++)
    {
        ovSelectedElements[i] = mvCandidateElements[mvSelectedElements[i]];
    }
    
    return 0;
}



int DSSRWSectionModelIterator::CountRows()
{
    if(mSectionType == DssRWSectionGroupBySection)
    {
        return mvRow.size();
    }
    else
        return 1;
    
    return 0;
}

/*!
 @method
 @abstract   move to the first repeat section
 @discussion
 */
bool DSSRWSectionModelIterator::FirstRow()
{
	if (mSectionType == DssRWSectionGroupBySection)
	{
        mCurrent = 0;
        //tliao, 771763, empty NGB
        //        if(mvRow.size()==0)
        //            return false;
        
        return true;
	}
	else {
		return true;
	}
}

/*!
 @method
 @abstract   move cursor to the next repeat section
 @discussion if cursor is at the last one, return false
 */
bool DSSRWSectionModelIterator::NextRow()
{
    //junwang, #611495, #611494, NGB has NextRow when it's all
    if (mSectionType == DssRWSectionGroupBySection&&mIsAll)
	{
        if((mCurrent+1)<mvRow.size())
		{
			mCurrent++;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
    {
		return false;
	}
}

int DSSRWSectionModelIterator::Count()
{
    return mnChildren;
}


int DSSRWSectionModelIterator::GetNGBSelectedRow()
{
    if (mSectionType==DssRWSectionGroupBySection)
    {
        if(mIsAll)
        {
            //tliao, 771763, empty NGB
            if (mvRow.size() == 0)
                return mParentNGB;
            return mvRow[mCurrent] - 1;
        }
        else
        {
            if(!mvSelectedElements.empty())
            {
                return mvRow[mvSelectedElements[0]]-1;
            }
            else
                return mParentNGB;
        }
    }
    else
        return mParentNGB;
}

int DSSRWSectionModelIterator::RefreshChildIterator(DSSRWIterator *ipChildIterator, DSSRWNode *ipChildNode)
{
	EnumDSSRWNodeType lType = ipChildNode->getType();
    
    if (lType == DssRWNodeSection)
    {
        return static_cast<DSSRWSectionModelIterator*>(ipChildIterator)->ReInit();
    }
    else if(lType == DssRWNodeControl)
    {
        return static_cast<DSSRWControlModelIterator*>(ipChildIterator)->ReInit();
    }
    else if(lType == DssRWNodeTemplate)
    {
        return ipChildIterator->ReInit();
    }
    else if(lType == DssRWNodeFieldGroup)
    {
        return ipChildIterator->ReInit();
    }
    return 0;
}


int DSSRWSectionModelIterator::hFindDefaultElements(const vector<int> &ivCandidateElements, vector<int> &ovDefaultElements)
{
    switch (mSectionType)
    {
        case DssRWSectionGroupBySection:
            return hCalDefaultElementsNGB(ivCandidateElements, ovDefaultElements);
            break;
        case DssRWSectionControlGroupBySection:
            return hCalDefaultElementsCGB(ivCandidateElements, ovDefaultElements);
            break;
        case DssRWSectionUnitCondition:
            return hCalDefaultElementsUC(ivCandidateElements,ovDefaultElements);
            break;
        default:
            break;
    }
    
	return E_FAIL;
}


int DSSRWSectionModelIterator::hProcessMetricsCGB(DSSRWNode* ipNode, DSSGroupByUnit* ipUnit,
                                                  EnumDSSCurrentElementStatus iStatus)
{
    if (!ipNode || !ipUnit)
        return E_INVALIDARG;
    
    DSSRWSectionNode* lpThisNode = static_cast<DSSRWSectionNode*>(ipNode);
    
    vector<DSSTemplateMetric*> lMetrics;
    lpThisNode->getControlMetrics(lMetrics);
    int lCount =  lMetrics.size();
    
    msCurrentMetrics.clear();
    
    //bool lValid = false;
    
	bool lfAllMetrics = false;
    
    
	if(iStatus == DssCurrentElementUnset)
	{
		msCurrentMetrics.clear();
        
		if (lCount > 0)
		{
            mIsReplaced = true;
            GUID lMetricId = GUID_NULL;
            lMetricId = lMetrics[0]->getID();
            msCurrentMetrics.insert(lMetricId);
            mvReplacedElements.push_back(new DSSMetricElementProxy(mpDataModel->getObjectContext(),lMetricId));
			lfAllMetrics = false;
		}
		else
		{
			lfAllMetrics = true;
		}
	}
	else
	{
        // get elements from gb unit
        DSSBaseElementsProxy* lpCurrentElements = ipUnit->getCurrentElements();
        for (int i=0; i<lpCurrentElements->Count(); i++)
        {
            DSSBaseElementProxy* lpCurrentElement = lpCurrentElements->Item(i);
            if (lpCurrentElement->getElementType() == DssElementAll)
            {
                lfAllMetrics = true;  break;
            }
            
            for (int j=0; j<lCount; j++)
            {
                if (MBase::IsEqualGUID(lpCurrentElement->getObjectID(), lMetrics[j]->getID()))
                {
                    msCurrentMetrics.insert(lMetrics[j]->getID());
                    break;
                }
            }
        }
        
        //TQMS663129 hyan,12/10/2012
        //add handle for empty case, choose the first metric
        //TQMS685122 hyan,27/11/2012
        //only for Desired status
        if(iStatus==DssCurrentElementDesired&&msCurrentMetrics.size()==0&&false==lfAllMetrics)
        {
            if(lCount>0)
            {
                GUID lMetricId = GUID_NULL;
                lMetricId = lMetrics[0]->getID();
                
                msCurrentMetrics.insert(lMetrics[0]->getID());
                mvReplacedElements.push_back(new DSSMetricElementProxy(mpDataModel->getObjectContext(),lMetricId));
                
                mIsReplaced=true;
                lfAllMetrics=false;
                
            }
        }
        
	}
    
	if (lfAllMetrics)
	{
		msCurrentMetrics.clear();
		int iMetric = 0;
		for (iMetric = 0; iMetric < lCount; iMetric++)
		{
			GUID lMetricID = GUID_NULL;
            lMetricID = lMetrics[iMetric]->getID();
			msCurrentMetrics.insert(lMetricID);
		}
	}
    
	return S_OK;
}


// @BEWARE: This method is not for metric selector as it has different meaning in the expression and might not be IN
int DSSRWSectionModelIterator::hGetCurrentGBElementsUseExpr(DSSGroupByUnit *ipUnit, bool *opUseExpr, DSSBaseElementsProxy **oppElements)
{
    
	_ASSERTE(ipUnit&& opUseExpr && oppElements);
    
    DSSCExpression* lpExpression = ipUnit->getExpression();
    (*opUseExpr) = false;
    
    if(lpExpression)
    {
        DSSExpNode* lpExpRoot = lpExpression->Root();
        
        if(lpExpRoot!=NULL&&lpExpRoot->getType() == DssNodeOperator
           &&(static_cast<DSSExpNodeOperator*>(lpExpRoot)->getOperatorType()==DssFunctionNotIn))
        {
            (*opUseExpr) = true;
            DSSBaseElementsProxy *lpExcludedElements = static_cast<DSSExpNodeElementsObject*> (lpExpRoot->getChildNode()->getLastSibling())->getElements();
            *oppElements = lpExcludedElements;
        }
        //hack code, for test
        else
            *oppElements = ipUnit->getCurrentElements();
    }
    else
    {
        (*oppElements) = ipUnit->getCurrentElements();
        return 0;
    }
    
    return 0;
}

int DSSRWSectionModelIterator::FindTag(vector<GUID> &ivGroupByUnits, int *opTag, int iChildBranch)
{
    *opTag = 0;
    
    DSSRWSectionModelIterator* lpParentIterator = DSSRWNodeModelIterator::mpParentIterator;
    
    if(mpNodeModel==NULL && lpParentIterator==NULL)
        return S_OK;
    
    if (mpNodeModel!=NULL)
    {
        if(mSectionType == DssRWSectionControlGroupBySection||
           mSectionType == DssRWSectionUnitCondition)
        {
            int i=0;
            for (; i<(int)mvSelectedElements.size(); i++)
            {
                if(mvCandidateElements[mvSelectedElements[i]] == DSSXTAB_TOTAL_KEY)
                    break;
            }
            if(i<(int)mvSelectedElements.size())
            {
                int j=0;
                GUID lID = GUID_NULL;
                mpNodeModel->GetUnit(&lID);
                for (; j<ivGroupByUnits.size(); j++)
                {
                    if (MBase::IsEqualGUID(ivGroupByUnits[j], lID))
                    {
                        break;
                    }
                }
                if (j<(int)ivGroupByUnits.size()&&msTags.size()>0)
                {
                    *opTag = *(msTags.begin());
                }
            }
        }
        else
            if(mSectionType == DssRWSectionGroupBySection)
            {
                bool hasTotal = false;
                if(mIsAll)
                {
                    if (iChildBranch>=0&&
                        iChildBranch<mvCandidateElements.size()&&
                        mvCandidateElements[iChildBranch]==DSSXTabTotalKey)
                    {
                        hasTotal = true;
                    }
                }
                else
                {
                    for (int i=0; i<(int)mvSelectedElements.size(); i++)
                    {
                        if(mvCandidateElements[mvSelectedElements[i]] == DSSXTabTotalKey)
                        {
                            hasTotal = true; break;
                        }
                    }
                }
                if (hasTotal)
                {
                    GUID lID = GUID_NULL;
                    mpNodeModel->GetUnit(&lID);
                    int j=0;
                    for (; j<ivGroupByUnits.size(); j++)
                    {
                        if (MBase::IsEqualGUID(ivGroupByUnits[j], lID))
                        {
                            break;
                        }
                    }
                    if(j<ivGroupByUnits.size())
                    {
                        if(msTags.size()>0)
                            *opTag = *(msTags.begin());
                        else
                            if (mvDefTags.size()>0) {
                                *opTag = *(mvDefTags.begin());
                            }
                    }
                }
                
            }
    }
    
    if (*opTag==0&&lpParentIterator!=NULL)
    {
        return static_cast<DSSRWSectionModelIterator*>(lpParentIterator)->FindTag(ivGroupByUnits, opTag, mNGBChildBranch);
    }
    return S_OK;
}

//ivCandidateElements: global keys, ovCurrentElements: row index of ivCandidateElements
int DSSRWSectionModelIterator::hGetCurrentElementsNGB(const vector<int> &ivCandidateElements, vector<int> &ovCurrentElements)
{
    HRESULT hr = S_OK;
    DSSRWSectionNode* lpRWSectionNode = static_cast<DSSRWSectionNode*>(mpNode);
    
    DSSGroupByUnits* lpGroupByUnits = lpRWSectionNode->getGroupByUnits();
    //junwang, 2012/03/21, #547222
    if(lpGroupByUnits==NULL||lpGroupByUnits->Count()==0)
        return S_OK;
    
    DSSGroupByUnit* lpUnit = lpGroupByUnits->Item(0);
	
    EnumDSSCurrentElementStatus lStatus = DssCurrentElementReserved;
    
    lStatus = lpUnit->getCurrentElementsStatus();
    
    if (lStatus == DssCurrentElementUnset)
    {
        return hFindDefaultElements(ivCandidateElements, ovCurrentElements);
    }
    
    //ctang 2011-4-8 for not in
    DSSBaseElementsProxy* lpElements = NULL;
    bool lUseExpr = false;
    int lCountElements = 0;
    hGetCurrentGBElementsUseExpr(lpUnit, &lUseExpr, &lpElements);
    lCountElements = lpElements->Count();
    
    //junwang,2012/03/27, NGB has no exclude
    if(lCountElements == 0)
    {
        return S_OK;
    }
    
    EnumDSSObjectType lType = lpUnit->getObjectType();
    GUID lID = GUID_NULL;
    lID = lpUnit->getObjectID();
    
    int lNumRow = ivCandidateElements.size();
    int iElement = 0;
    
    for (; iElement<lCountElements; iElement++)
    {
        DSSBaseElementProxy* lpElement = NULL;
        lpElement = lpElements->Item(iElement);
        
        EnumDSSElementType lElementType = lpElement->getElementType();
        
        if (lNumRow > 0 && (lElementType == DssElementAll ||
                            (mPageByStyle == DssRWPageByStyleAll && lElementType != DssElementAll) ))
            //this is the same regardless the unit type
		{
			// this is normal gb all, because control gb all was processed earlier.
			//!!!! special handling
			if(mPageByStyle == DssRWPageByStyleIndividual)
			{
				//ctang 2010-12-23 in NGB case, all is not include in pCandidateElements
				//except the case that parent NGB option is All, then pCandidateElements only contain 0
				//opCurrentElements.push_back(pCandidateElements[0]);
				if (lNumRow > 0)
					ovCurrentElements.push_back(1); // imagine there's an all in the beginning, but we don't use.
				// else not found valid element
				return S_OK;
			}
            
			if(mPageByStyle == DssRWPageByStyleAll || mPageByStyle == DssRWPageByStyleBoth)
			{
				//ctang 2010-12-23 in NGB case, use 0 to represent all
				ovCurrentElements.push_back(0);	// imagine there's an all in the beginning, but we don't use.
				return S_OK;
			}
		}
        
        int lLocalIndex = -1;
        hr = mpDataModel->LookForElement(lpElement, lID, lType, ivCandidateElements, &lLocalIndex);
        if(hr == S_OK)
        {
            ovCurrentElements.push_back(lLocalIndex+1);
        }
    }
    
    //check subtotal
    for (int i=0; i<ivCandidateElements.size(); i++) {
        if(ivCandidateElements[i] == DSSXTabTotalKey)
        {
            for(int j=0; j<lCountElements; j++)
            {
                DSSBaseElementProxy* lpElement = lpElements->Item(j);
                EnumDSSElementType lElementType = lpElement->getElementType();
                if(lElementType == DssElementSubtotal &&(!lUseExpr))
                    msTags.insert(1);
                else if(lElementType!=DssElementSubtotal && lUseExpr)
                    msTags.insert(1);
            }
            break;
        }
    }
    
	if (lUseExpr)
	{
        vector<bool> lvMask(lNumRow,false);
        
		for (int i = 0; i < ovCurrentElements.size(); i++)
		{
			int lPos = ovCurrentElements[i] - 1;
			AE_ASSERT(lPos < lNumRow);
			lvMask[lPos] = true;
		}
		ovCurrentElements.clear();
		for (int i = 0; i < lNumRow; i++)
			if (!lvMask[i])
				ovCurrentElements.push_back(i);
        
		if (!ovCurrentElements.empty())
		{
            DSSDocumentSubtotalRefs* lpGBSubtotals = lpUnit->getSubtotals();
			
            if(lpGBSubtotals)
            {
                int lSize = lpGBSubtotals->Count();
                
                if(lSize>0)
                    msTags.insert(1);
            }
		}
        
	}
    
	if(ovCurrentElements.size()==0 && lStatus == DssCurrentElementDesired)
		return hCalDefaultElementsNGB(ivCandidateElements, ovCurrentElements);
    
    return S_OK;
}

int DSSRWSectionModelIterator::hCalDefaultElementsNGB(const vector<int> &ivCandidateElements, vector<int> &ovDefaultElements)
{
    mIsReplaced = true;
    int lNumRow = ivCandidateElements.size();
    
    CComVariant* lpAutoSelectProp = mpDataModel->getObjectContext()->getDocumentDefinition()->getPropertyValue("AnalyticalEngineProperties", "AutoSelectionForUnset");
	int lUnsetOption = 0;
	
	if (lpAutoSelectProp)
		lUnsetOption = lpAutoSelectProp->lVal;
    
    
    if (mPageByStyle == DssRWPageByStyleAll)
    {
        ovDefaultElements.push_back(0);
        return S_OK;
    }
    else if(mPageByStyle == DssRWPageByStyleBoth)
    {
        if(lUnsetOption!=0 && ivCandidateElements.size()>0)
        {
            if(lUnsetOption == 1)
                ovDefaultElements.push_back(1);
            else
                if(lUnsetOption == 2)
                    ovDefaultElements.push_back(lNumRow);
        }
        else
            if(ivCandidateElements.size()>0)
                ovDefaultElements.push_back(0);
        
        return S_OK;
    }
    else if(mPageByStyle == DssRWPageByStyleIndividual)
    {
        if(lNumRow>0)
        {
            if(lUnsetOption==0 || lUnsetOption == 1)
                ovDefaultElements.push_back(1);
            else
                ovDefaultElements.push_back(lNumRow);
        }
        return S_OK;
    }
    
    
	return E_FAIL;
    
}

//for non-metrics CGB
int DSSRWSectionModelIterator::hGetCurrentElementsCGB(const vector<int> &ivCandidateElements, vector<int> &ovCurrentElements)
{
    HRESULT hr = S_OK;
    DSSRWSectionNode* lpRWSectionNode = static_cast<DSSRWSectionNode*>(mpNode);
    
    DSSGroupByUnits* lpGroupByUnits = lpRWSectionNode->getGroupByUnits();
    
    //junwang, 2012/03/21, #547222
    if(lpGroupByUnits==NULL||lpGroupByUnits->Count()==0)
        return S_OK;
    
    DSSGroupByUnit* lpUnit = lpGroupByUnits->Item(0);
    EnumDSSCurrentElementStatus lStatus = DssCurrentElementReserved;
    
    lStatus = lpUnit->getCurrentElementsStatus();
    
    GUID lID = GUID_NULL;
    lID = lpUnit->getObjectID();
    
	//ctang 2010-12-30
	EnumDSSObjectType lType = lpUnit->getObjectType();
    
    if (MBase::IsGUIDNULL(lID)) //metrics selector
    {
        hProcessMetricsCGB(mpNode, lpUnit, lStatus);
        return S_OK;
    }
    else
        if(lType == DssTypeMetric)
            return S_OK;
    
    if (lStatus == DssCurrentElementUnset)
    {
        return hCalDefaultElementsCGB(ivCandidateElements, ovCurrentElements);
    }
    
    //ctang 2011-4-8 for not in
    DSSBaseElementsProxy* lpElements = NULL;
    bool lUseExpr = false;
    int lCountElements = 0;
    hGetCurrentGBElementsUseExpr(lpUnit, &lUseExpr, &lpElements);
    lCountElements = lpElements->Count();
    
    int lNumRow = ivCandidateElements.size();
    int iElement = 0;
    
    if(!lUseExpr&&lCountElements==0)
    {
        mbEmptySelectFlag=true;
        
        return S_OK;
    }
    
    if(lCountElements>0) //junwang, 2012/02/27, special handle for empty selection and exclude option
    {
        for (; iElement<lCountElements; iElement++)
        {
            DSSBaseElementProxy* lpElement = NULL;
            lpElement = lpElements->Item(iElement);
            
            EnumDSSElementType lElementType = lpElement->getElementType();
            
            if (lElementType == DssElementAll)
            {
                //ctang 2011-8-19
                mIsAll = true;
                
                for (int i=0; i<ivCandidateElements.size(); i++)
                {
                    ovCurrentElements.push_back(i);
                }
                
                for (int i=0; i<mvDefTags.size(); i++)
                {
                    msTags.insert(mvDefTags[i]);
                }
                
                return  S_OK;
            }
            
            int lLocalIndex = -1;
            hr = mpDataModel->LookForElement(lpElement, lID, lType, ivCandidateElements, &lLocalIndex);
            if(hr == S_OK)
            {
                ovCurrentElements.push_back(lLocalIndex);
            }
        }
        
        //for subtotal
        for (int i=0; i<ivCandidateElements.size(); i++) {
            if(ivCandidateElements[i] == DSSXTabTotalKey)
            {
                for(int j=0; j<lCountElements; j++)
                {
                    DSSBaseElementProxy* lpElement = lpElements->Item(j);
                    EnumDSSElementType lElementType = lpElement->getElementType();
                    if(lElementType == DssElementSubtotal &&(!lUseExpr))
                        msTags.insert(1);
                    else if(lElementType!=DssElementSubtotal && lUseExpr)
                        msTags.insert(1);
                }
                break;
            }
        }
    }
    
	if (lUseExpr)
	{
        vector<bool> lvMask(lNumRow,false);
        
		for (int i = 0; i < ovCurrentElements.size(); i++)
		{
			int lPos = ovCurrentElements[i];
			AE_ASSERT(lPos < lNumRow);
			lvMask[lPos] = true;
		}
		ovCurrentElements.clear();
		for (int i = 0; i < lNumRow; i++)
			if (!lvMask[i])
				ovCurrentElements.push_back(i);
        
		if (!ovCurrentElements.empty())
		{
            DSSDocumentSubtotalRefs* lpGBSubtotals = lpUnit->getSubtotals();
			
            if(lpGBSubtotals)
            {
                int lSize = lpGBSubtotals->Count();
                
                if(lSize>0)
                    msTags.insert(1);
            }
		}
        
	}
    
	if(ovCurrentElements.size()==0 && lStatus == DssCurrentElementDesired)
		return hCalDefaultElementsCGB(ivCandidateElements, ovCurrentElements);
    
    return S_OK;
}

//for non-metrics CGB
int DSSRWSectionModelIterator::hCalDefaultElementsCGB(const vector<int> &ivCandidateElements, vector<int> &ovDefaultElements)
{
    int lNumRow = (int)ivCandidateElements.size();
    //HRESULT hr = S_OK;
    
    DSSGroupByUnits* lpGroupByUnits = static_cast<DSSRWSectionNode*>(mpNode)->getGroupByUnits();
    int lNumUnit = 0;
    lNumUnit = lpGroupByUnits->Count();
    
    _ASSERT(lNumUnit > 0);
    
    EnumDSSObjectType lUnitObjectType;
    GUID lUnitId;
    DSSGroupByUnit* lpUnit = lpGroupByUnits->Item(0);
    
    lUnitId = lpUnit->getObjectID();
    lUnitObjectType = lpUnit->getObjectType();
    
    EnumDSSXMLRWSelectorUnsetStatus lUnsetStatus;
    lUnsetStatus = lpUnit->getUnsetStatus();
    
    int lElementCount = lpUnit->getUnsetElementNumber();
    
    //ctang 2011-9-6
    EnumDSSCurrentElementStatus lCurrentStatus;
    lCurrentStatus = lpUnit->getCurrentElementsStatus();
    
//    xiazhou, begin:if needs to keep CGB targets consistent, then use same auto-pick result
//    for all targets.

    const NodeAndControlsVector *lvSources = mpDataModel->targetToSource(DSSRWNodeModelIterator::mNodeKey);
    bool lbDoAutoPick = true;
    vector<string> lvCGBSourceKeys;
    int lTemplateControlCount = 0;
    string lTemplateKey;
    int lTopmostPos = -1;
    // weilin DE42497 autopick topmost support "Show Options for Total"
    bool lAllowTotalSelected = false;

    if(lvSources)
    {
        for(int i = 0; i < lvSources->size(); ++i)
        {
            const NodeAndControls *lpSource = lvSources->at(i);
            if(lpSource)
            {
                for(int j = 0; j < lpSource->ControlKeys.size(); ++j)
                {
                    string lControlKey = lpSource->ControlKeys[j];
                    CHECK_PTR(DSSRWSectionIterator::mpObjectContext);
                    DSSRWControl* lpControl = DSSRWSectionIterator::mpObjectContext->getControl(lControlKey);
                    CHECK_PTR(lpControl);

                    if(!lpControl->isUC())
                    {
                        lvCGBSourceKeys.push_back(lControlKey);
                        if (lpSource->NodeType == DssRWNodeTemplate)
                        {
                            ++lTemplateControlCount;
                            lTemplateKey = lpSource->NodeKey;
                            if (lpControl->getSubtotals())
                            {
                                lAllowTotalSelected = true;
                            }
                        }
                    }
                }
            }
        }
        // weilin US51811 2016/07/19 autopick topmost
        if (lTemplateControlCount == 1) // we deal with only one source template
        {
            hGetAutoPickTopmostProperty(lvSources, lTemplateKey);
            // CHECK_HR
            if (mbGotAutoPickProperty && mbAutoPickTopmost) // if we set the autopick topmost property
            {
                lTopmostPos = hGetTopmostPosInCandidate(lTemplateKey, lUnitId, lAllowTotalSelected);
                if (mbSkip)
                {
                    return S_OK;
                }
            }
        }
    }

    bool lbUseSelectionMap = false;
    if(lbUseSelectionMap && lCurrentStatus == DssCurrentElementUnset)
    {//needs to read the setting. Disable for now.
        if(lvSources)
        {
            AutoSelectionMap &lAutoSelectionMap = mpDataModel->getAutoSelectionMap();

            for(vector<string>::iterator ite = lvCGBSourceKeys.begin(); ite != lvCGBSourceKeys.end(); ite++)
            {
                //There is already default selections for the CGB, and this is not its primary target. Then use picked selections
                if(lAutoSelectionMap.hasDefaultSelections(*ite) && !lAutoSelectionMap.isPrimaryTarget(*ite, DSSRWNodeModelIterator::mNodeKey))
                {
                    vector<int> lvDefaultSelections = lAutoSelectionMap.getDefaultSelections(*ite);
                    //find default selections position in ivCandidateElements
                    for(vector<int>::iterator selectionIte = lvDefaultSelections.begin(); selectionIte != lvDefaultSelections.end(); selectionIte++)
                    {
                        vector<int>::const_iterator selected = std::find(ivCandidateElements.begin(), ivCandidateElements.end(), *selectionIte);
                        if( selected != ivCandidateElements.end())
                        {
                            ovDefaultElements.push_back(*selected);
                        }
                    }
                    lbDoAutoPick = false;
                }
            }
        }
        
        if(!lbDoAutoPick)
            return S_OK;
    }
//    xiazhou, end:if needs to keep CGB targets consistent, then use same auto-pick result
//    for all targets.
    
    if(lUnsetStatus == DssXMLRWSelectorUnsetStatusDefault)
    {
        lUnsetStatus = DssXMLRWSelectorUnsetStatusFirst;
        lElementCount = 1;
    }
    _ASSERT(!MBase::IsGUIDNULL(lUnitId));
    
    //now we should be able to locate this element
    int lUnit = 0;
    lUnit = mpDataModel->FindUnit(lUnitId);
    //    DSSTabularAttribute* lpGlobalLookUp = mpDataModel->GetTabularAttributeByGUID(lUnitId);
    
    int lnSelected = 0;
    vector<int> lSelectedRowVec;
    if (lUnsetStatus == DssXMLRWSelectorUnsetStatusFirst)
    {
        int lSelectedRow = 0;
        // weilin 2016/07/20 US51811 First pick topmost
        if (lTopmostPos != -1)
        {
            lSelectedRowVec.push_back(lTopmostPos);
            lnSelected++;
        }
        while (lnSelected<lElementCount&&lSelectedRow<lNumRow)
        {
            // we don't pick total element, only real wh element.
            // according to the current sorting, total is at the end.
            if (ivCandidateElements[lSelectedRow] == DSSXTAB_TOTAL_KEY
                //ctang 2011-9-6
                || (lCurrentStatus == DssCurrentElementDesired && mpCandiateInBase!=NULL
                    && (*mpCandiateInBase)[lSelectedRow]==1))
            {
                lSelectedRow++;
                continue;
            }

            if (lTopmostPos != -1 && lSelectedRow == lTopmostPos)
            {
                lSelectedRow++;
                continue;
            }

            lSelectedRowVec.push_back(lSelectedRow);
            lSelectedRow++;
            lnSelected++;
        }
    }
    else
    {
        int lSelectRow = lNumRow - 1;
        while (lnSelected<lElementCount && lSelectRow>=0)
        {
            // we don't pick total element, only real wh element.
            // according to the current sorting, total is at the end.
            if (ivCandidateElements[lSelectRow] == DSSXTAB_TOTAL_KEY
                //ctang 2011-9-6
                || (lCurrentStatus == DssCurrentElementDesired && mpCandiateInBase!=NULL && (*mpCandiateInBase)[lSelectRow]==0))
            {
                lSelectRow--;
                continue;
            }

            lSelectedRowVec.push_back(lSelectRow);
            lSelectRow--;
            lnSelected++;
        }
    }

    for (int i = 0; i < lSelectedRowVec.size(); ++i)
    {
        ovDefaultElements.push_back(lSelectedRowVec[i]);

        //junwang, 12/8/2011
        DSSBaseElementProxy* lpElement = NULL;
        DSSTabularUnit* lpTabUnit = NULL;
        int lLocalKey = -1;
        mpDataModel->GetTabularUnit(lUnitId, ivCandidateElements[lSelectedRowVec[i]], &lpTabUnit, &lLocalKey);

        //TQMS 681036 hyan, 7/12/2012
        bool lfNULLElement = false;
        mpDataModel->IsCompoundNull(lUnitId, ivCandidateElements[lSelectedRowVec[i]], &lfNULLElement);
        if(lfNULLElement)
            lpElement = new DSSNullElementProxy(mpDataModel->getObjectContext(),lUnitId);
        else
        {
            if(lpTabUnit)
            {
                lpTabUnit->createElement(lLocalKey, mpDataModel->getObjectContext(), &lpElement, ivCandidateElements[lSelectedRowVec[i]],mGlobalLookupVer);
            }
        }

        if(lpElement!=NULL)
            mvReplacedElements.push_back(lpElement);
        //junwang, #553015, set mIsReplaced true only when there is at least one replaced element
        mIsReplaced = true;
    }

    //junwang, 2012/7/19, TQMS 624580, append total if it's the only one available, same as server side.
    if (lNumRow == 1 && ivCandidateElements[0] == DSSXTAB_TOTAL_KEY)
    {
        ovDefaultElements.push_back(0);
        mIsReplaced = true;
        //subtotal index for total will be 1,hyan, 11/12/2012
        DSSBaseElementProxy* lpElement = new DSSSubtotalElementProxy(mpDataModel->getObjectContext(),lUnitId,1);
        //DSSBaseElementProxy* lpElement = new DSSSubtotalElementProxy(mpDataModel->getObjectContext(),lUnitId,ivCandidateElements[0]);
        mvReplacedElements.push_back(lpElement);
    }
    
    if(lbUseSelectionMap && lCurrentStatus == DssCurrentElementUnset)
    {//update AutoSelectionMap
        vector<int> lvSelections;
        for(vector<int>::iterator ite = ovDefaultElements.begin(); ite != ovDefaultElements.end(); ite++)
            lvSelections.push_back(ivCandidateElements[*ite]);
        
        
        for(vector<string>::iterator ite = lvCGBSourceKeys.begin(); ite != lvCGBSourceKeys.end(); ite++)
        {
            AutoSelectionMap &lAutoSelectionMap = mpDataModel->getAutoSelectionMap();
        
            if(!lAutoSelectionMap.hasDefaultSelections(*ite) || lAutoSelectionMap.isPrimaryTarget(*ite, DSSRWNodeModelIterator::mNodeKey))
            {
                int hr = lAutoSelectionMap.updateMap(*ite, DSSRWNodeModelIterator::mNodeKey, lvSelections);
                CHECK_HR;
            }
        }
    }
    
    return S_OK;
    
}

int DSSRWSectionModelIterator::hGetCurrentElementsUC(const vector<int> &ivCandidateElements, vector<int> &ovCurrentElements)
{
    HRESULT hr = S_OK;
    DSSRWSectionNode* lpRWSectionNode = static_cast<DSSRWSectionNode*>(mpNode);
    
    DSSGroupByUnits* lpGroupByUnits = lpRWSectionNode->getGroupByUnits();
    //junwang, 2012/03/21, #547222
    if(lpGroupByUnits==NULL||lpGroupByUnits->Count()==0)
        return S_OK;
    
    DSSGroupByUnit* lpUnit = lpGroupByUnits->Item(0);
    
    EnumDSSCurrentElementStatus lStatus = DssCurrentElementReserved;
    
    lStatus = lpUnit->getCurrentElementsStatus();
    
    GUID lID = GUID_NULL;
    lID = lpUnit->getObjectID();
    
	//ctang 2010-12-30
	EnumDSSObjectType lType = lpUnit->getObjectType();
    
    if (lStatus == DssCurrentElementUnset)
    {
        return hCalDefaultElementsUC(ivCandidateElements, ovCurrentElements);
    }
    
    //ctang 2011-4-8 for not in
    DSSBaseElementsProxy* lpElements = NULL;
    bool lUseExpr = false;
    int lCountElements = 0;
    hGetCurrentGBElementsUseExpr(lpUnit, &lUseExpr, &lpElements);
    lCountElements = lpElements->Count();
    if (lCountElements == 0)
    {
        mbEmptySelectFlag=true;
        //ctang 2011-4-8 empty selection
        return S_OK;
    }
    
    // check whether this is total only.
    if (lCountElements == 1)
    {
        DSSBaseElementProxy* lpElement;
        lpElement = lpElements->Item(0);
        
        EnumDSSElementType lElementType = DssElementReserved;
        lElementType = lpElement->getElementType();
        // all rows.
        if (lElementType == DssElementSubtotal)
        {
            for (int i = 0; i < ivCandidateElements.size(); i++)
            {
                ovCurrentElements.push_back(i);
            }
            return S_OK;
        }
    }
    
    int lNumRow = ivCandidateElements.size();
    int iElement = 0;
    
    for (; iElement<lCountElements; iElement++)
    {
        DSSBaseElementProxy* lpElement = NULL;
        lpElement = lpElements->Item(iElement);
        
        EnumDSSElementType lElementType = lpElement->getElementType();
        if (lElementType==DssElementSubtotal)
        {
            continue; //ignore
        }
        
        if (lElementType == DssElementAll)
        {
            //ctang 2011-8-19
            mIsAll = true;
            
            for (int i=0; i<ivCandidateElements.size(); i++)
            {
                ovCurrentElements.push_back(i);
            }
            
            for (int i=0; i<mvDefTags.size(); i++)
            {
                msTags.insert(mvDefTags[i]);
            }
            
            return  S_OK;
        }
        
        int lLocalIndex = -1;
        hr = mpDataModel->LookForElement(lpElement, lID, lType, ivCandidateElements, &lLocalIndex);
        if(hr == S_OK)
        {
            ovCurrentElements.push_back(lLocalIndex);
        }
    }
    
    //for subtotal
    for (int i=0; i<ivCandidateElements.size(); i++) {
        if(ivCandidateElements[i] == DSSXTabTotalKey)
        {
            for(int j=0; j<lCountElements; j++)
            {
                DSSBaseElementProxy* lpElement = lpElements->Item(j);
                EnumDSSElementType lElementType = lpElement->getElementType();
                if(lElementType == DssElementSubtotal &&(!lUseExpr))
                    msTags.insert(1);
                else if(lElementType!=DssElementSubtotal && lUseExpr)
                    msTags.insert(1);
            }
            break;
        }
    }
    
    
	if (lUseExpr)
	{
        vector<bool> lvMask(lNumRow,false);
        
		for (int i = 0; i < ovCurrentElements.size(); i++)
		{
			int lPos = ovCurrentElements[i];
			AE_ASSERT(lPos < lNumRow);
			lvMask[lPos] = true;
		}
		ovCurrentElements.clear();
		for (int i = 0; i < lNumRow; i++)
			if (!lvMask[i])
				ovCurrentElements.push_back(i);
        
		if (!ovCurrentElements.empty())
		{
            DSSDocumentSubtotalRefs* lpGBSubtotals = lpUnit->getSubtotals();
			
            if(lpGBSubtotals)
            {
                int lSize = lpGBSubtotals->Count();
                
                if(lSize>0)
                    msTags.insert(1);
            }
		}
        
	}
    
	if(ovCurrentElements.size()==0 && lStatus == DssCurrentElementDesired)
		return hCalDefaultElementsUC(ivCandidateElements, ovCurrentElements);
    
    return S_OK;
}

int DSSRWSectionModelIterator::hCalDefaultElementsUC(const vector<int> &ivCandidateElements, vector<int> &ovDefaultElements)
{
    for (int i=0; i<(int)ivCandidateElements.size(); i++)
    {
        ovDefaultElements.push_back(i);
    }
    mIsReplaced = true;
    mvReplacedElements.push_back(new DSSAllElementProxy(DSSRWSectionIterator::mpObjectContext));
    
    return S_OK;
}

int DSSRWSectionModelIterator::hget_ElementIndex(int iRow, int iUnit, int *pElementIndex)
{
    *pElementIndex = mvCandidateElements[iRow];
    
    return S_OK;
    /*
     GUID lUnitID = GUID_NULL;
     HRESULT hr=mpDataModel->GetUnitGUIDByIndex(iUnit, &lUnitID);
     CHECK_HR;
     
     bool lbFound = false;
     if(MBase::IsEqualGUID(mpNodeModel->mUnit, lUnitID))
     {
     if(iRow>=0 && iRow<mvCandidateElements.size())
     {
     *pElementIndex=mvCandidateElements[iRow];
     lbFound = true;
     }
     else
     {
     return S_FALSE;
     }
     }
     else
     {
     vector<GUID>* lGroupByUnits = &mpNodeModel->mvGroupByUnits;
     for(int i=0; i<lGroupByUnits->size(); ++i)
     {
     if(MBase::IsEqualGUID((*lGroupByUnits)[i], lUnitID)==0)
     {
     if(i<mvGroupByElements.size() && iRow>=0 && iRow<mvGroupByElements[i].size())
     {
     *pElementIndex=mvGroupByElements[i][iRow];
     lbFound = true;
     break;
     }
     return S_FALSE;
     }
     }
     }
     if (lbFound)
     {
     bool lfCompoundNULL = false;
     HRESULT hr = mpDataModel->IsCompoundNull(lUnitID, *pElementIndex, &lfCompoundNULL);
     CHECK_HR;
     if (lfCompoundNULL)
     *pElementIndex = -1;
     return S_OK;
     }
     
     return S_FALSE;
     */
}

int DSSRWSectionModelIterator::getGroupByElements(vector<vector<int> > &ovvGroupByElements, int iChildBranch)
{
    if (mSectionType == DssRWSectionGroupBySection && mIsAll)
    {
        ovvGroupByElements = mvGroupByElements;
        int lSize = ovvGroupByElements.size();
        ovvGroupByElements.resize(lSize+1);
        
        if (iChildBranch>=0&&iChildBranch<mvCandidateElements.size())
        {
            ovvGroupByElements[lSize].push_back(mvCandidateElements[iChildBranch]);
            
            return S_OK;
        }
    }
    else
    {
        return getGroupByElements(ovvGroupByElements);
    }
    
    return S_OK;
}

int DSSRWSectionModelIterator::toString(std::ostream &out)
{
    out<<"SectionIterator  "<<DSSRWNodeModelIterator::mNodeKey<<std::endl;
    
    out<<"SectionType  ";
    
    if(mSectionType == DssRWSectionGroupBySection)
        out<<"NGB\n";
    else if(mSectionType == DssRWSectionControlGroupBySection)
        out<<"CGB\n";
    else if(mSectionType == DssRWSectionUnitCondition)
        out<<"UC\n";
    else
        out<<"Other\n";
    
    out<<"GroupbyElements\n";
    for (int i=0; i<mvGroupByElements.size(); i++)
    {
        out<<"  ";
        for (int j=0; j<mvGroupByElements[i].size(); j++)
        {
            out<<mvGroupByElements[i][j]<<"  ";
        }
        out<<"\n";
    }
    
    out<<"GroupbyMetrics\n";
    //    set<MBase::GUID, less<MBase::GUID> >::iterator it = msGroupByMetrics.begin();
    set<GUID, less<GUID> >::iterator it = msGroupByMetrics.begin();
    for (; it!=msGroupByMetrics.end(); it++)
    {
        GUID lID = *it;
        out<<"  "<<DSSAEHelper::Guid2String(lID)<<"\n";
    }
    
    if (mSectionType == DssRWSectionGroupBySection||
        mSectionType == DssRWSectionControlGroupBySection||
        mSectionType == DssRWSectionUnitCondition)
    {
        out<<"Candidate Elements:\n";
        
        vector<MBase::CString> lvTexts;
        lvTexts.resize(mvCandidateElements.size());
        
        DSSRWSectionNode* lpThisNode = dynamic_cast<DSSRWSectionNode*>(DSSRWNodeModelIterator::getRWNode());
        
        DSSGroupByUnit* lpUnit = lpThisNode->getGroupByUnits()->Item(0);
        
        GUID lID = lpUnit->getObjectID();
        
        DSSObjectInfo* lpObjectInfo = lpUnit->getObject();
        EnumDSSObjectType lType = lpObjectInfo->getType();
        
        if(lType == DssTypeAttribute)
        {
            DSSAttribute *lpAttribute=(DSSAttribute*)lpObjectInfo;
            
            DSSTabularAttribute *lpTabularAttribute=mpDataModel->GetTabularAttributeByGUID(lID);
            
            DSSAttributeFormWrappers* lpAttributeFormWrappers= new DSSAttributeFormWrappers;
            lpAttribute->getDisplayFormWrappers(lpAttributeFormWrappers);
            if(lpAttributeFormWrappers)
            {
                for(int i=0;i<lpAttributeFormWrappers->Count();i++)
                {
                    DSSAttributeFormWrapper *lpAttributeFormWrapper=lpAttributeFormWrappers->Item(i);
                    
                    int lBaseFormID=lpAttributeFormWrapper->getBaseFormID();
                    
                    //TODO[opt]: cache data form
                    int lColumnID = 0;
                    int hr = lpTabularAttribute->FindFormWithLocale(0, lBaseFormID, 0, lColumnID);
                    
                    if(hr!=S_OK)
                        continue;//return S_OK;
                    
                    //CHECK_HR;
                    
                    DSSDataColumn *lpColumn = NULL;
                    hr = lpTabularAttribute->Item(lColumnID, &lpColumn);
                    CHECK_HR;
                    
                    //how to get offset?
                    int lOffset=0;
                    
                    const void* lCellData=NULL;
                    int lCellDataSize=0;
                    EnumDSSDataType lCellDataType;
                    DSSDataFlag lCellDataFlag;
                    
                    for (int j=0; j<mvCandidateElements.size(); j++)
                    {
                        if(mvCandidateElements[j] == -100)
                        {
                            lvTexts[j] = "Total";
                            continue;
                        }
                        lOffset = mvCandidateElements[j];
                        lpColumn->getData(lOffset, (const char**)(&lCellData), lCellDataSize, lCellDataType,lCellDataFlag);
                        
                        std::string sValue;
                        DSSAEHelper::ConvertToString(lCellData, lCellDataSize, lCellDataType, lCellDataFlag, sValue);
                        lvTexts[j].append(sValue);
                    }
                }
                
                delete lpAttributeFormWrappers;
                lpAttributeFormWrappers = NULL;
            }
        }
        else if(lType == DssTypeConsolidation)
        {
            //DSSConsolidation* lpCS = (DSSConsolidation*)lpObjectInfo;
            DSSTabularUnit* lpTabularUnit = NULL;
            int lLocalKey = -1;
            
            for (int j=0; j<mvCandidateElements.size(); j++)
            {
                if(mvCandidateElements[j]==-100)
                {
                    lvTexts[j] = "Total";
                    continue;
                }
                
                mpDataModel->GetTabularUnit(lID, mvCandidateElements[j], &lpTabularUnit, &lLocalKey);
                AE_ASSERT(lpTabularUnit);
                DSSTabularConsolidation* lpTCS = dynamic_cast<DSSTabularConsolidation*>(lpTabularUnit);
                AE_ASSERT(lpTCS);
                XTabConsolidationElement* lpCSElement = lpTCS->getElement(lLocalKey);
                AE_ASSERT(lpCSElement);
                lvTexts[j] = lpCSElement->value;
            }
        }
        else if(lpObjectInfo->getType() == DssTypeFilter)
        {
            //DSSCustomGroup* lpCG = (DSSCustomGroup*)lpObjectInfo;
            DSSTabularUnit* lpTabularUnit = NULL;
            int lLocalKey = -1;
            
            for (int j=0; j<mvCandidateElements.size(); j++)
            {
                if (mvCandidateElements[j] == -100)
                {
                    lvTexts[j] = "Total";
                    continue;
                }
                
                mpDataModel->GetTabularUnit(lID, mvCandidateElements[j], &lpTabularUnit, &lLocalKey);
                AE_ASSERT(lpTabularUnit);
                DSSTabularCustomGroup* lpTCG = dynamic_cast<DSSTabularCustomGroup*>(lpTabularUnit);
                AE_ASSERT(lpTCG);
                XTabCustomGroupElement* lpCGElement = lpTCG->getElement(lLocalKey);
                AE_ASSERT(lpCGElement);
                lvTexts[j] = lpCGElement->displayName;
            }
        }
        
        for (int j=0; j<mvCandidateElements.size(); j++)
        {
            out<<"  "<<lvTexts[j]<<"  "<<mvCandidateElements[j]<<"\n";
        }
        
        out<<"mCurrent  "<<mCurrent<<"\n";
        
        //selected elements
        out<<"selected elements\n";
        for (int j=0; j<mvSelectedElements.size(); j++)
        {
            out<<mvSelectedElements[j]<<"  ";
        }
        out<<std::endl;
        
        //selected metrics
        //       set<MBase::GUID,less<MBase::GUID> >::iterator it = msCurrentMetrics.begin();
        set<GUID,less<GUID> >::iterator it = msCurrentMetrics.begin();
        for (; it!=msCurrentMetrics.end(); it++)
        {
            GUID lID = *it;
            out<<"  "<<DSSAEHelper::Guid2String(lID)<<"\n";
        }
        
        out<<"parentNGB   "<<mParentNGB<<"\n";
        out<<"NGBLevel  "<<mLevel<<"\n";
        out<<"NGBChildBranch  "<<mNGBChildBranch<<"\n";
        if(mPageByStyle == DssRWPageByStyleReserved)
            out<<"PageByStyle   "<<"Reserved\n";
        else if(mPageByStyle == DssRWPageByStyleAll)
            out<<"PageByStyle   "<<"ALL\n";
        else if(mPageByStyle == DssRWPageByStyleBoth)
            out<<"PageByStyle   "<<"Both\n";
        else if(mPageByStyle == DssRWPageByStyleIndividual)
            out<<"PageByStyle   "<<"Individual\n";
        else
            out<<"PageByStyle   "<<"Unknown\n";
        
    }
    
    return S_OK;
}

DSSRWFieldGroupIterator* DSSRWSectionModelIterator::getSpecialFieldGroupIterator()
{
    if (!mpSpecialFieldGroupIterator)
	{
		DSSRWNode * lpThisNode = DSSRWNodeModelIterator::getRWNode();
		if (!lpThisNode)
			return NULL;
		
        DSSRWNode* lpFGNode = lpThisNode->getSpecialFGNode();
        
        if(lpFGNode == NULL)
            return NULL;
        
        DSSRWNodeModel* lpModel = mpDataModel->getNodeModel(lpFGNode->getKey());
        DSSRWNodeModelIterator* lpChildIterator = NULL;
        
        if(lpModel)
        {
            lpModel->createIterator(this, &lpChildIterator);
        }
        
		mpSpecialFieldGroupIterator = dynamic_cast<DSSRWFieldGroupIterator*>(dynamic_cast<DSSRWFieldGroupModelIterator*>(lpChildIterator));
	}
	
	return mpSpecialFieldGroupIterator;
}

//junwang, 03/02/2012
void DSSRWSectionModelIterator::updateGroupbyElements(const vector<vector<int> > &ivvGroupbyElement)
{
    mvGroupByElements = ivvGroupbyElement;
}

//junwang, 06/21/2012
void DSSRWSectionModelIterator::updateGroupbyMetrics(const set<GUID, less<GUID> > &isGroupbyMetrics)
{
    msGroupByMetrics = isGroupbyMetrics;
}

int DSSRWSectionModelIterator::hGetCurrentElements(vector<int> &ovCurrentElements)
{
    DSSRWSectionNode* lpRWSectionNode = static_cast<DSSRWSectionNode*>(mpNode);
    DSSGroupByUnits* lpGroupByUnits = lpRWSectionNode->getGroupByUnits();
    if(lpGroupByUnits->Count()<=0||lpGroupByUnits->Item(0)==NULL)
        return S_OK;
    
    DSSGroupByUnit* lpGroupByUnit = lpGroupByUnits->Item(0);
    EnumDSSCurrentElementStatus lStatus = lpGroupByUnit->getCurrentElementsStatus();
    
    // weilin DE40067 2016/08/04 add this section node to parent section node's source template node's mvTargetCGBNodeVec
    if(mSectionType == DssRWSectionControlGroupBySection || mSectionType == DssRWSectionUnitCondition)
    {
        if (DSSRWSectionIterator::mpParentIterator != NULL)
        {
            DSSRWSectionNode* lpParentSectionNode = static_cast<DSSRWSectionNode *>(this->getObjectContext()->getNode(DSSRWSectionIterator::mpParentIterator->getNodeKey()));
            assert(lpParentSectionNode != NULL);
            vector<DSSRWTemplateNode *>lvSourceTemplateNodes = lpParentSectionNode->getSourceTemplateNodes();
            for (int i = 0; i < lvSourceTemplateNodes.size(); ++i)
            {
                DSSRWSectionNode* lpThisSectionNode = static_cast<DSSRWSectionNode *>(this->getObjectContext()->getNode(DSSRWNodeModelIterator::mNodeKey));
                lpThisSectionNode->InsertToSourceTemplateNodes(lvSourceTemplateNodes[i]);
                lvSourceTemplateNodes[i]->InsertToTargetCGBNodeVec(lpThisSectionNode);
            }
        }
    }
    if(mSectionType == DssRWSectionGroupBySection)
    {
        if(lStatus == DssCurrentElementChosen || lStatus == DssCurrentElementDesired)
            hGetCurrentNGBElementsFromSelection(mvCandidateElements, ovCurrentElements);
        
        //junwang, #598848
        if(ovCurrentElements.size()>0&&ovCurrentElements[0]==0)
        {
            mIsAll = true;
            hSortCandidateElements();
        }
        
        DSSBaseElementsProxy* lpElements = NULL;
        bool lUseExpr = false;
        int lCountElements = 0;
        hGetCurrentGBElementsUseExpr(lpGroupByUnit, &lUseExpr, &lpElements);
        lCountElements = lpElements->Count();
        
        //desired but empty selection will not triggle default elements computaton
        if(lStatus == DssCurrentElementUnset || (ovCurrentElements.empty()&&lStatus == DssCurrentElementDesired && !(!lUseExpr&&lCountElements==0)))
        {
            hSortCandidateElements();
            hCalDefaultElementsNGB(mvCandidateElements, ovCurrentElements);
        }
    }
    else if(mSectionType == DssRWSectionControlGroupBySection)
    {
        GUID lID =  lpGroupByUnit->getObjectID();
        EnumDSSObjectType lType = lpGroupByUnit->getObjectType();
        
        if(lID == GUID_NULL)
            return hProcessMetricsCGB(mpNode, lpGroupByUnit, lStatus);
        else
            if(lType == DssTypeMetric)
                return S_OK;
        
        if(mvCandidateElements.empty()/*||(mvCandidateElements.size()==1&&mvCandidateElements[0]==-100)*/)
            return S_OK;

        if(lStatus == DssCurrentElementChosen || lStatus == DssCurrentElementDesired)
            hGetCurrentCGBElementsFromSelection(mvCandidateElements, ovCurrentElements);
        
        DSSBaseElementsProxy* lpElements = NULL;
        bool lUseExpr = false;
        int lCountElements = 0;
        hGetCurrentGBElementsUseExpr(lpGroupByUnit, &lUseExpr, &lpElements);
        lCountElements = lpElements->Count();
        
        //zhyang, 762824, 2013/07/02, desired but empty selection will NOT triggle default elements computaton
        //desired but empty selection will still triggle default elements computaton, junwang, 2012/08/03, TQMS 637038
        if(lStatus == DssCurrentElementUnset || (ovCurrentElements.empty()&&lStatus == DssCurrentElementDesired&&!(!lUseExpr&&lCountElements==0)))
        {
            hSortCandidateElements();
            hCalDefaultElementsCGB(mvCandidateElements, ovCurrentElements);
            if (mbSkip)
            {
                return S_OK;
            }
        }
    }
    else if(mSectionType == DssRWSectionUnitCondition)
    {
        //TQMS 690822, hyan, 10/12/2012
        GUID lID =  lpGroupByUnit->getObjectID();
        
        EnumDSSObjectType lType = lpGroupByUnit->getObjectType();
        
        //metric condition selector
        if(lID!=GUID_NULL&&lType == DssTypeMetric)
            return S_OK;
        
        
        if(lStatus == DssCurrentElementChosen || lStatus == DssCurrentElementDesired)
            hGetCurrentUCElementsFromSelection(mvCandidateElements, ovCurrentElements);
        
        if(lStatus == DssCurrentElementUnset || (ovCurrentElements.empty()&&lStatus == DssCurrentElementDesired))
        {
            hCalDefaultElementsUC(mvCandidateElements, ovCurrentElements);
        }
    }
    
    return S_OK;
}

int DSSRWSectionModelIterator::hGetCurrentNGBElementsFromSelection(const vector<int> &ivCandidateElements, vector<int> &ovCurrentElements)
{
    HRESULT hr = S_OK;
    
    DSSRWSectionNode* lpRWSectionNode = static_cast<DSSRWSectionNode*>(mpNode);
    
    DSSGroupByUnits* lpGroupByUnits = lpRWSectionNode->getGroupByUnits();
    
    DSSGroupByUnit* lpUnit = lpGroupByUnits->Item(0);
	
    EnumDSSCurrentElementStatus lStatus = DssCurrentElementReserved;
    
    lStatus = lpUnit->getCurrentElementsStatus();
    
    if (lStatus == DssCurrentElementUnset)
    {
        return S_OK;
    }
    
    //ctang 2011-4-8 for not in
    DSSBaseElementsProxy* lpElements = NULL;
    bool lUseExpr = false;
    int lCountElements = 0;
    lpElements = lpUnit->getCurrentElements();
    
    //    hGetCurrentGBElementsUseExpr(lpUnit, &lUseExpr, &lpElements);
    lCountElements = lpElements->Count();
    
    //junwang,2012/03/27, NGB has no exclude
    if(lCountElements == 0)
    {
        return S_OK;
    }
    
    EnumDSSObjectType lType = lpUnit->getObjectType();
    GUID lID = GUID_NULL;
    lID = lpUnit->getObjectID();
    
    int lNumRow = ivCandidateElements.size();
    int iElement = 0;
    
    
    for (; iElement<lCountElements; iElement++)
    {
        DSSBaseElementProxy* lpElement = NULL;
        lpElement = lpElements->Item(iElement);
        
        EnumDSSElementType lElementType = lpElement->getElementType();
        
        if (lNumRow > 0 && (lElementType == DssElementAll ||
                            (mPageByStyle == DssRWPageByStyleAll && lElementType != DssElementAll) ))
            //this is the same regardless the unit type
		{
			// this is normal gb all, because control gb all was processed earlier.
			//!!!! special handling
			if(mPageByStyle == DssRWPageByStyleIndividual)
			{
				//ctang 2010-12-23 in NGB case, all is not include in pCandidateElements
				//except the case that parent NGB option is All, then pCandidateElements only contain 0
				//opCurrentElements.push_back(pCandidateElements[0]);
				if (lNumRow > 0)
					ovCurrentElements.push_back(1); // imagine there's an all in the beginning, but we don't use.
				// else not found valid element
				return S_OK;
			}
            
			if(mPageByStyle == DssRWPageByStyleAll || mPageByStyle == DssRWPageByStyleBoth)
			{
				//ctang 2010-12-23 in NGB case, use 0 to represent all
				ovCurrentElements.push_back(0);	// imagine there's an all in the beginning, but we don't use.
                mIsAll = true;
				return S_OK;
			}
		}
        
        int lLocalIndex = -1;
        hr = mpDataModel->LookForElement(lpElement, lID, lType, ivCandidateElements, &lLocalIndex);
        if(hr == S_OK)
        {
            ovCurrentElements.push_back(lLocalIndex+1);
        }
    }
    
    //check subtotal
    for (int i=0; i<ivCandidateElements.size(); i++) {
        if(ivCandidateElements[i] == DSSXTabTotalKey)
        {
            for(int j=0; j<lCountElements; j++)
            {
                DSSBaseElementProxy* lpElement = lpElements->Item(j);
                EnumDSSElementType lElementType = lpElement->getElementType();
                if(lElementType == DssElementSubtotal &&(!lUseExpr))
                    msTags.insert(1);
                else if(lElementType!=DssElementSubtotal && lUseExpr)
                    msTags.insert(1);
            }
            break;
        }
    }
    
    /*
     if (lUseExpr)
     {
     vector<bool> lvMask(lNumRow,false);
     
     for (int i = 0; i < ovCurrentElements.size(); i++)
     {
     int lPos = ovCurrentElements[i] - 1;
     AE_ASSERT(lPos < lNumRow);
     lvMask[lPos] = true;
     }
     ovCurrentElements.clear();
     for (int i = 0; i < lNumRow; i++)
     if (!lvMask[i])
     ovCurrentElements.push_back(i);
     
     if (!ovCurrentElements.empty())
     {
     DSSDocumentSubtotalRefs* lpGBSubtotals = lpUnit->getSubtotals();
     
     if(lpGBSubtotals)
     {
     int lSize = lpGBSubtotals->Count();
     
     if(lSize>0)
     msTags.insert(1);
     }
     }
     
     }
     */
    return S_OK;
}

int DSSRWSectionModelIterator::hGetCurrentCGBElementsFromSelection(const vector<int> &ivCandidateElements, vector<int> &ovCurrentElements)
{
    HRESULT hr = S_OK;
    DSSRWSectionNode* lpRWSectionNode = static_cast<DSSRWSectionNode*>(mpNode);
    
    DSSGroupByUnits* lpGroupByUnits = lpRWSectionNode->getGroupByUnits();
    
    //junwang, 2012/03/21, #547222
    if(lpGroupByUnits==NULL||lpGroupByUnits->Count()==0)
        return S_OK;
    
    DSSGroupByUnit* lpUnit = lpGroupByUnits->Item(0);
    EnumDSSCurrentElementStatus lStatus = DssCurrentElementReserved;
    
    lStatus = lpUnit->getCurrentElementsStatus();
    
    GUID lID = GUID_NULL;
    lID = lpUnit->getObjectID();
    
	//ctang 2010-12-30
	EnumDSSObjectType lType = lpUnit->getObjectType();
    
    if (MBase::IsGUIDNULL(lID)) //metrics selector
    {
        hProcessMetricsCGB(mpNode, lpUnit, lStatus);
        return S_OK;
    }
    else
        if(lType == DssTypeMetric)
            return S_OK;
    
    if (lStatus == DssCurrentElementUnset)
    {
        return S_OK;
    }
    
    //ctang 2011-4-8 for not in
    DSSBaseElementsProxy* lpElements = NULL;
    bool lUseExpr = false;
    int lCountElements = 0;
    hGetCurrentGBElementsUseExpr(lpUnit, &lUseExpr, &lpElements);
    lCountElements = lpElements->Count();
    
    int lNumRow = ivCandidateElements.size();
    int iElement = 0;
    
    if(!lUseExpr&&lCountElements==0)
    {
        mbEmptySelectFlag=true;
        
        return S_OK;
    }
    
    if(lCountElements>0) //junwang, 2012/02/27, special handle for empty selection and exclude option
    {
        for (; iElement<lCountElements; iElement++)
        {
            DSSBaseElementProxy* lpElement = NULL;
            lpElement = lpElements->Item(iElement);
            
            EnumDSSElementType lElementType = lpElement->getElementType();
            
            if (lElementType == DssElementAll)
            {
                //ctang 2011-8-19
                mIsAll = true;
                
                for (int i=0; i<ivCandidateElements.size(); i++)
                {
                    ovCurrentElements.push_back(i);
                }
                
                for (int i=0; i<mvDefTags.size(); i++)
                {
                    msTags.insert(mvDefTags[i]);
                }
                
                return  S_OK;
            }
            
            int lLocalIndex = -1;
            hr = mpDataModel->LookForElement(lpElement, lID, lType, ivCandidateElements, &lLocalIndex);
            if(hr == S_OK)
            {
                ovCurrentElements.push_back(lLocalIndex);
            }
        }
        
        //for subtotal
        for (int i=0; i<ivCandidateElements.size(); i++) {
            if(ivCandidateElements[i] == DSSXTabTotalKey)
            {
                int j=0;
                for(; j<lCountElements; j++)
                {
                    DSSBaseElementProxy* lpElement = lpElements->Item(j);
                    EnumDSSElementType lElementType = lpElement->getElementType();
                    
                    if(lElementType == DssElementSubtotal)
                        break;
                    //if(lElementType == DssElementSubtotal &&(!lUseExpr))
                    //{
                    //    msTags.insert(1);
                    //}
                    //else if(lElementType!=DssElementSubtotal && lUseExpr)
                    //    msTags.insert(1);
                }
                if(j==lCountElements&&lUseExpr)
                    msTags.insert(1);
                else if(j!=lCountElements&&!lUseExpr)
                    msTags.insert(1);
                break;
            }
        }
    }
    
	if (lUseExpr)
	{
        vector<bool> lvMask(lNumRow,false);
        
		for (int i = 0; i < ovCurrentElements.size(); i++)
		{
			int lPos = ovCurrentElements[i];
			AE_ASSERT(lPos < lNumRow);
			lvMask[lPos] = true;
		}
		ovCurrentElements.clear();
		for (int i = 0; i < lNumRow; i++)
			if (!lvMask[i])
				ovCurrentElements.push_back(i);
        
		if (!ovCurrentElements.empty())
		{
            DSSDocumentSubtotalRefs* lpGBSubtotals = lpUnit->getSubtotals();
			
            if(lpGBSubtotals)
            {
                int lSize = lpGBSubtotals->Count();
                
                if(lSize>0)
                    msTags.insert(1);
            }
		}
        
	}
    
    return S_OK;
}

int DSSRWSectionModelIterator::hGetCurrentUCElementsFromSelection(const vector<int> &ivCandidateElements, vector<int> &ovCurrentElements)
{
    HRESULT hr = S_OK;
    DSSRWSectionNode* lpRWSectionNode = static_cast<DSSRWSectionNode*>(mpNode);
    
    DSSGroupByUnits* lpGroupByUnits = lpRWSectionNode->getGroupByUnits();
    //junwang, 2012/03/21, #547222
    if(lpGroupByUnits==NULL||lpGroupByUnits->Count()==0)
        return S_OK;
    
    DSSGroupByUnit* lpUnit = lpGroupByUnits->Item(0);
	
    EnumDSSCurrentElementStatus lStatus = DssCurrentElementReserved;
    
    lStatus = lpUnit->getCurrentElementsStatus();
    
    GUID lID = GUID_NULL;
    lID = lpUnit->getObjectID();
    
	//ctang 2010-12-30
	EnumDSSObjectType lType = lpUnit->getObjectType();
    
    if (lStatus == DssCurrentElementUnset)
    {
        return S_OK;
    }
    
    //ctang 2011-4-8 for not in
    DSSBaseElementsProxy* lpElements = NULL;
    bool lUseExpr = false;
    int lCountElements = 0;
    hGetCurrentGBElementsUseExpr(lpUnit, &lUseExpr, &lpElements);
    lCountElements = lpElements->Count();
    if (lCountElements == 0)
    {
        mbEmptySelectFlag=true;
        //ctang 2011-4-8 empty selection
        return S_OK;
    }
    
    // check whether this is total only.
    if (lCountElements == 1)
    {
        DSSBaseElementProxy* lpElement;
        lpElement = lpElements->Item(0);
        
        EnumDSSElementType lElementType = DssElementReserved;
        lElementType = lpElement->getElementType();
        // all rows.
        if (lElementType == DssElementSubtotal)
        {
            for (int i = 0; i < ivCandidateElements.size(); i++)
            {
                ovCurrentElements.push_back(i);
            }
            return S_OK;
        }
    }
    
    int lNumRow = ivCandidateElements.size();
    int iElement = 0;
    
    for (; iElement<lCountElements; iElement++)
    {
        DSSBaseElementProxy* lpElement = NULL;
        lpElement = lpElements->Item(iElement);
        
        EnumDSSElementType lElementType = lpElement->getElementType();
        if (lElementType==DssElementSubtotal)
        {
            continue; //ignore
        }
        
        if (lElementType == DssElementAll)
        {
            //ctang 2011-8-19
            mIsAll = true;
            
            for (int i=0; i<ivCandidateElements.size(); i++)
            {
                ovCurrentElements.push_back(i);
            }
            
            for (int i=0; i<mvDefTags.size(); i++)
            {
                msTags.insert(mvDefTags[i]);
            }
            
            return  S_OK;
        }
        
        int lLocalIndex = -1;
        hr = mpDataModel->LookForElement(lpElement, lID, lType, ivCandidateElements, &lLocalIndex);
        if(hr == S_OK)
        {
            ovCurrentElements.push_back(lLocalIndex);
        }
    }
    
    //for subtotal
    for (int i=0; i<ivCandidateElements.size(); i++) {
        if(ivCandidateElements[i] == DSSXTabTotalKey)
        {
            for(int j=0; j<lCountElements; j++)
            {
                DSSBaseElementProxy* lpElement = lpElements->Item(j);
                EnumDSSElementType lElementType = lpElement->getElementType();
                if(lElementType == DssElementSubtotal &&(!lUseExpr))
                    msTags.insert(1);
                else if(lElementType!=DssElementSubtotal && lUseExpr)
                    msTags.insert(1);
            }
            break;
        }
    }
    
    
	if (lUseExpr)
	{
        vector<bool> lvMask(lNumRow,false);
        
		for (int i = 0; i < ovCurrentElements.size(); i++)
		{
			int lPos = ovCurrentElements[i];
			AE_ASSERT(lPos < lNumRow);
			lvMask[lPos] = true;
		}
		ovCurrentElements.clear();
		for (int i = 0; i < lNumRow; i++)
			if (!lvMask[i])
				ovCurrentElements.push_back(i);
        
		if (!ovCurrentElements.empty())
		{
            DSSDocumentSubtotalRefs* lpGBSubtotals = lpUnit->getSubtotals();
			
            if(lpGBSubtotals)
            {
                int lSize = lpGBSubtotals->Count();
                
                if(lSize>0)
                    msTags.insert(1);
            }
		}
        
	}
    
    return S_OK;
}

int DSSRWSectionModelIterator::hSortCandidateElements()
{
    
    int lnRow = mvCandidateElements.size();
    if(lnRow <= 1)
        return S_OK;
    
    bool lfNeedSort = false;
    bool lfInverse  = false;
    int  lNum = 1;
    bool lfFirstN = true;
    int hr = S_OK;
    
    //junwang, 2012/05/04, use first n instead sort for optimization
    if (mSectionType == DssRWSectionGroupBySection)
    {
        //junwang, 2012/07/04, sorting has been done  on NGBPathInfo in server side
        lfNeedSort = false;
        /*
         CComVariant* lpAutoSelectProp = mpDataModel->getObjectContext()->getDocumentDefinition()->getPropertyValue("AnalyticalEngineProperties", "AutoSelectionForUnset");
         int lUnsetOption = 0;
         
         if (lpAutoSelectProp)
         lUnsetOption = lpAutoSelectProp->lVal;
         
         if(mPageByStyle != DssRWPageByStyleAll && !mIsAll)
         {
         if((lUnsetOption == 0 && mPageByStyle == DssRWPageByStyleIndividual)||lUnsetOption == 1)
         {
         lfNeedSort = true; lfInverse = false; lNum = 1;
         }
         else if(lUnsetOption == 2)
         {
         lfNeedSort = true; lfInverse = true; lNum = 1;
         }
         else if(lUnsetOption==0 && mPageByStyle!=DssRWPageByStyleIndividual)
         {
         lfNeedSort = true; lfFirstN = false;
         }
         }
         else
         {
         lfNeedSort = true; lfFirstN = false;
         }
         */
    }
    else if(mSectionType == DssRWSectionControlGroupBySection)
    {
        DSSGroupByUnits* lpGroupByUnits = static_cast<DSSRWSectionNode*>(mpNode)->getGroupByUnits();
        
        if(lpGroupByUnits->Count()==0||lpGroupByUnits->Item(0)==NULL)
            return S_OK;
        
        DSSGroupByUnit* lpUnit = lpGroupByUnits->Item(0);
        GUID lUnitId;
        lUnitId = lpUnit->getObjectID();
        if(lUnitId == GUID_NULL)
            return S_OK;
        
        int lElementCount = lpUnit->getUnsetElementNumber();
        
        EnumDSSXMLRWSelectorUnsetStatus lUnsetStatus;
        lUnsetStatus = lpUnit->getUnsetStatus();
        if(lUnsetStatus == DssXMLRWSelectorUnsetStatusDefault)
        {
            lUnsetStatus = DssXMLRWSelectorUnsetStatusFirst;
            lElementCount = 1;
        }
        
        if(lUnsetStatus == DssXMLRWSelectorUnsetStatusFirst)
            lfInverse = false;
        else if(lUnsetStatus == DssXMLRWSelectorUnsetStatusLast)
            lfInverse = true;
        
        lfNeedSort = true;
        lNum = lElementCount;
    }
    
    if(lfNeedSort)
    {
        DSSRWSectionCompare lCMP;
        lCMP.Init(this, mpDataModel->GetDisplayNullOnTop());
        
        int lUnit;
        vector<int> lvFroms;
        bool *lpAscs = NULL;
        hr = lCMP.getSortInfo(lUnit, lvFroms, &lpAscs);
        MBase::StrongPtr<bool, MBase::DeleteArray<bool> > lpAscsPtr(lpAscs);
        
        if(hr==S_OK)
        {
            DSSGUnitContainer *lGlobalLookupContianer;
            hr = mpDataModel->GetGlobalLookupContainer(&lGlobalLookupContianer);
            CHECK_HR;
            
            EnumDSSTemplateUnitType lUnitType;
            bool lfDE = false;
            hr = lGlobalLookupContianer->GetUnitType(lUnit, &lUnitType, &lfDE);
            CHECK_HR;
            
            if(lUnitType == DssTemplateAttribute && !lfDE)
            {
                int lBucketID = lGlobalLookupContianer->getBucketID(lUnit, lvFroms, lpAscs, mpDataModel->GetDisplayNullOnTop());
                if(true  &&  lBucketID != -1)
                {
                    int lnBuckets;
                    int *lpBuckets;
                    hr = lGlobalLookupContianer->getSortResult(lUnit, lBucketID, &lpBuckets, &lnBuckets);
                    CHECK_HR;
                    
                    vector<int> lvAttrBucket;
                    lvAttrBucket.resize(lnBuckets);
                    for(int i = 0; i < lnBuckets; ++i)
                    {
                        lvAttrBucket[i] = -1;
                    }
                    
                    bool lbHasTotal = false;
                    for(int i = 0; i < mvCandidateElements.size(); ++i)
                    {
                        if(mvCandidateElements[i] == DSSXTAB_TOTAL_KEY)
                            lbHasTotal = true;
                        else
                            lvAttrBucket[lpBuckets[mvCandidateElements[i]]] = mvCandidateElements[i];
                    }
                    
                    mvCandidateElements.clear();
                    for(int i = 0; i < lnBuckets; ++i)
                    {
                        if(lvAttrBucket[i] != -1)
                            mvCandidateElements.push_back(lvAttrBucket[i]);
                    }
                    if(lbHasTotal)
                        mvCandidateElements.push_back(DSSXTAB_TOTAL_KEY);
                    return S_OK;
                }
            }
        }
        
        
        {
            int* lpIndex = new int[lnRow];
            int* lpTemp = new int[lnRow];
            
            for (int i=0; i<lnRow; i++)
            {
                lpIndex[i] = i;
            }
            
            bool lbContainTotal=false;
            
            for(int i=0;i<mvCandidateElements.size();i++)
            {
                if(mvCandidateElements[i]==DSSXTAB_TOTAL_KEY)
                    lbContainTotal=true;
            }
            
            //TQMS 722108, hyan, 2/22/2013
            //if CGB set last 1 and contain total, _xtab_first_n won't sort since total always the last
            if(lbContainTotal&&lNum+1<lnRow)
                lNum+=1;
            
            if(lfFirstN)
                hr = _xtab_first_n(lpIndex, lpIndex+lnRow, lCMP, lNum, lfInverse);
            else
                hr = _xtab_sort(lpIndex, lpIndex+lnRow, lCMP);
            
            for (int i=0; i<lnRow; i++)
            {
                lpTemp[i] = mvCandidateElements[lpIndex[i]];
            }
            memmove(&mvCandidateElements[0], lpTemp, sizeof(int)*lnRow);
            
            if(mSectionType == DssRWSectionGroupBySection)
            {
                for (int i=0; i<lnRow; i++)
                {
                    lpTemp[i] = mvRow[lpIndex[i]];
                }
                memmove(&mvRow[0],lpTemp,sizeof(int)*lnRow);
            }
            
            delete[] lpIndex;
            delete[] lpTemp;
        }
    }
    return S_OK;
    
    
    /*
     if(lnRow>1)
     {
     DSSRWSectionCompare lCMP;
     lCMP.Init(this, mpDataModel->GetDisplayNullOnTop());
     
     int* lpIndex = new int[lnRow];
     int* lpTemp = new int[lnRow];
     
     for (int i=0; i<lnRow; i++)
     {
     lpIndex[i] = i;
     }
     
     _xtab_sort(lpIndex,lpIndex+lnRow,lCMP);
     
     for (int i=0; i<lnRow; i++)
     {
     lpTemp[i] = mvCandidateElements[lpIndex[i]];
     }
     memmove(&mvCandidateElements[0], lpTemp, sizeof(int)*lnRow);
     
     if(mSectionType == DssRWSectionGroupBySection)
     {
     for (int i=0; i<lnRow; i++)
     {
     lpTemp[i] = mvRow[lpIndex[i]];
     }
     memmove(&mvRow[0],lpTemp,sizeof(int)*lnRow);
     }
     
     delete[] lpIndex;
     delete[] lpTemp;
     }*/
}

int DSSRWSectionModelIterator::hSortCandidateElementsOld()
{
    int lnRow = mvCandidateElements.size();
    
    if(lnRow>1)
    {
        DSSRWSectionCompare lCMP;
        lCMP.Init(this, mpDataModel->GetDisplayNullOnTop());
        
        int* lpIndex = new int[lnRow];
        int* lpTemp = new int[lnRow];
        
        for (int i=0; i<lnRow; i++)
        {
            lpIndex[i] = i;
        }
        
        _xtab_sort(lpIndex,lpIndex+lnRow,lCMP);
        
        for (int i=0; i<lnRow; i++)
        {
            lpTemp[i] = mvCandidateElements[lpIndex[i]];
        }
        memmove(&mvCandidateElements[0], lpTemp, sizeof(int)*lnRow);
        
        if(mSectionType == DssRWSectionGroupBySection)
        {
            for (int i=0; i<lnRow; i++)
            {
                lpTemp[i] = mvRow[lpIndex[i]];
            }
            memmove(&mvRow[0],lpTemp,sizeof(int)*lnRow);
        }
        
        delete[] lpIndex;
        delete[] lpTemp;
    }
    
    return S_OK;
}
int DSSRWSectionModelIterator::hGetCurrentElementsOld(vector<int> &ovCurrentElements)
{
    hSortCandidateElementsOld();
    
    switch (mSectionType) {
        case DssRWSectionGroupBySection:
            hGetCurrentElementsNGB(mvCandidateElements, ovCurrentElements);
            break;
        case DssRWSectionControlGroupBySection:
            hGetCurrentElementsCGB(mvCandidateElements, ovCurrentElements);
            break;
        case DssRWSectionUnitCondition:
            hGetCurrentElementsUC(mvCandidateElements, ovCurrentElements);
            break;
        default:
            break;
    }
    
    return S_OK;
}

// 11/7/2012 tehe: populate ngb elements
int DSSRWSectionModelIterator::PopulateNGBElements(vector<DSSBaseElementProxy*> & iovNGBElements,DSSRWNodeModel *ipNodeModel)
{
    int hr = S_OK;
    
    
    if(ipNodeModel==NULL)
        return E_INVALIDARG;
    
    
    iovNGBElements.clear();
    
    vector<DSSBaseElementProxy*> lvNGBElements;
    DSSRWSectionModelIterator *pIterator = this;
    
    
    //TQMS 696960, hyan, 1/24/2013
    //We need to handle NGB All choose case, in this case, we cannot get the current elements from NGB Model
    DSSRWSectionModelIterator *lpParentIterator=this;
    
    vector<GUID> lvGroupByUnits;
    
    hr=ipNodeModel->GetGroupByUnits(&lvGroupByUnits);
    
    CHECK_HR;
    
    
    vector<vector<int> > lvvGroupByElements;
    hr = lpParentIterator->getGroupByElements(lvvGroupByElements);
    CHECK_HR;
    
    while (pIterator)
    {
        EnumDSSRWSectionType sectionType = (EnumDSSRWSectionType)pIterator->getSectionType();
        if(sectionType != DssRWSectionGroupBySection)
        {
            pIterator = pIterator->getParentIterator();
            continue;
        }
        
        DSSRWSectionNode *lpSectionNode = dynamic_cast<DSSRWSectionNode*>(pIterator->DSSRWNodeModelIterator::getRWNode());
        CHECK_PTR(lpSectionNode);
        
        NGBPathInfo *lpNGBPath = mpDataModel->getNGBPathInfo(pIterator->DSSRWNodeModelIterator::mNodeKey);
        if(!lpNGBPath)
            break;
        
        DSSBaseElementProxy *lpAllElement = lpNGBPath->GetAllElement();
        CHECK_PTR(lpAllElement);
        
        int pageType = lpSectionNode->getPageByStyle();
        if(pageType == DssRWPageByStyleAll)
        {
            lvNGBElements.push_back(lpAllElement);
            pIterator = pIterator->getParentIterator();
            continue;
        }
        
        
        //hyan, 1/24/2013
        //get Unit for NGB
        DSSRWNodeModel *lpNodeModel=pIterator->mpNodeModel;
        CHECK_PTR(lpNodeModel);
        
        
        GUID lNGBUnitGUID=GUID_NULL;
        
        hr=lpNodeModel->GetUnit(&lNGBUnitGUID);
        
        CHECK_HR;
        
        
        //check NGB Unit in the GroupbyElements
        int lNGBUnitIndex=-1;
        
        int lnsize=lvGroupByUnits.size();
        
        for(int i=0;i<lnsize;i++)
        {
            if(lvGroupByUnits[i]==lNGBUnitGUID)
            {
                lNGBUnitIndex=i;
                
                break;
            }
        }
        
        vector<int> lvCurrentElements;
        
        if(lNGBUnitIndex!=-1)
        {
            int size = lvvGroupByElements.size();
            
            if(size > 0&&lNGBUnitIndex<size)
                lvCurrentElements = lvvGroupByElements[lNGBUnitIndex];
            else
            {
                // tehe
                assert(false);
            }
        }
        //tliao, 779342, 08/09/2013, need to use pIterator->mNodeKey instead of this->mNodeKey
        int lUnitIndex = lpNGBPath->getNGBNodeIndex(pIterator->DSSRWNodeModelIterator::mNodeKey);
        assert(lUnitIndex != -1);
        if (lUnitIndex == -1)
            return S_FALSE;
        vector<DSSBaseElementProxy*> lvElements;
        lpNGBPath->GetElements(lvCurrentElements, lUnitIndex, lvElements);
        // tehe: just put the first element
        if(lvElements.size() >0)
            lvNGBElements.push_back(lvElements[0]);
        else
            lvNGBElements.push_back(NULL);
        
        pIterator = pIterator->getParentIterator();
    }
    
    iovNGBElements.assign(lvNGBElements.rbegin(), lvNGBElements.rend());
    
    return hr;
}

int DSSRWSectionModelIterator::AnalyzeTransactionThreshold(DSSThreshold* ipThreshold, bool* opResult)
{
    int hr = S_OK;
    if(ipThreshold && mpDataModel)
    {
        hr = mpDataModel->AnalyzeTransactionThreshold(ipThreshold, opResult);
        CHECK_HR;
    }
    return hr;
}

// weilin US51811
int DSSRWSectionModelIterator::hGetTopmostPosInCandidate(string iTemplateKey, GUID iUnitId, bool ibAllowTotalSelected)
{
    multimap<MBase::CString, DSSRWIterator *>lNodeKey2Iterator = mpDataModel->getNodeKey2Iterator();
    typedef multimap<MBase::CString, DSSRWIterator *>::iterator iterator;
    pair<iterator, iterator> beginEnd = lNodeKey2Iterator.equal_range(iTemplateKey);
    iterator begin = beginEnd.first;
    iterator end = beginEnd.second;

    vector<int> lSectionNGBPath;
    DSSRWSectionIterator* lpParentIterator = dynamic_cast<DSSRWSectionIterator *>(DSSRWSectionIterator::mpParentIterator);
    DSSRWIterator* lpIterator = NULL;
    if (lpParentIterator != NULL)
    {
        if (lpParentIterator->getSectionType() == DssRWSectionGroupBySection)
        {
            lSectionNGBPath.push_back(lpParentIterator->getCurrent()); // Why not use this->getNGBPath directly? Because in the step of create the iterator tree, we don't place this iterator to its parent iterator and this->getNGBPath will cause crash.
        }
        lpParentIterator->getNGBPath(lSectionNGBPath);
        // this->getObjectContext()->getRWDEngine()->FindIterator(lTemplateKey, lSectionNGBPath, &lpIterator);
    }

    if (begin != end)
    {
        while (begin != end)
        {
            lpIterator = begin->second;
            if(lpIterator != NULL) // when we have done with source grid
            {
                DSSRWGridIterator* lpGridIterator = dynamic_cast<DSSRWGridIterator *>(lpIterator);

                // weilin DE40331 2016/08/02 For autopick topmost, we get ngb path here and we only care about the iterator in compatible ngb path.
                vector<int> lGridNGBPath;
                lpParentIterator = lpGridIterator->getParentIterator();
                if (lpParentIterator != NULL)
                {
                    if (lpParentIterator->getSectionType() == DssRWSectionGroupBySection)
                    {
                        lGridNGBPath.push_back(lpParentIterator->getCurrent()); // Why not use this->getNGBPath directly? Because in the step of create the iterator tree, we don't place this iterator to its parent iterator and this->getNGBPath will cause crash.
                    }
                    lpParentIterator->getNGBPath(lGridNGBPath);
                }

                int j = lGridNGBPath.size() - 1, k = lSectionNGBPath.size() - 1;
                while (j >= 0 && k >= 0)
                {
                    if (lGridNGBPath[j] != lSectionNGBPath[k])
                    {
                        break;
                    }
                    j--;
                    k--;
                }
                if (j >= 0 && k >= 0)
                {
                    begin++;
                    continue;
                }

                DSSXTabView* lpXTabView = lpGridIterator->getXTabView();

                if (lpXTabView != NULL)
                {
                    mbSkip = false;
                    return lpGridIterator->getTopmostPosInCandidate(iUnitId, ibAllowTotalSelected, mvCandidateElements);
                }
                else  // If grids are in different panel or layout, lpXTabView may be null and we should reinit grid iterator to generate the xtabview
                {
                    lpGridIterator->setReinitForView(true);
                    lpGridIterator->ReInit();
                    // Skip reinit this CGB sectionmodeliterator and we will replay it afterwards.
                    mbSkip = true;
                    return -1;
                }
            }
            begin++;
        }
    }
    else // when we didn't reinit source grid
    {
        // Skip reinit this CGB sectionmodeliterator and we will replay it afterwards.
        mbSkip = true;
        return -1;
    }
}

// weilin US51811 2016/07/19 Get autopick property for cgb
void DSSRWSectionModelIterator::hGetAutoPickTopmostProperty(const NodeAndControlsVector* ipSources, string iTemplateKey)
{
    if (mbGotAutoPickProperty)
    {
        return;
    }
    for(int i = 0; i < ipSources->size(); ++i)
    {
        const NodeAndControls *lpSource = ipSources->at(i);
        if(lpSource)
        {
            for(int j = 0; j < lpSource->ControlKeys.size(); ++j)
            {
                string lControlKey = lpSource->ControlKeys[j];
                CHECK_PTR(DSSRWSectionIterator::mpObjectContext);
                DSSRWControl* lpControl = DSSRWSectionIterator::mpObjectContext->getControl(lControlKey);
                CHECK_PTR(lpControl);
                CComVariant lComVar;
                if(!lpControl->isUC() && lpControl->GetProperty("FormattingSelector", 31, &lComVar) == S_OK && lComVar.vt == VT_BOOL) // get property AutoPickBasedOnSourceTemplateControl -> "FormattingSelector":31
                {
                    mbGotAutoPickProperty = true;
                    if (lComVar.iVal == -1) // if we set the autopick topmost property
                    {
                        mbAutoPickTopmost = true;

                        // In source grid template node, we add the target CGB section node.
                        DSSRWTemplateNode* lpSourceTemplateNode = static_cast<DSSRWTemplateNode *>(DSSRWSectionIterator::mpObjectContext->getNode(iTemplateKey));
                        DSSRWSectionNode* lpTargetCGBNode = static_cast<DSSRWSectionNode *>(DSSRWSectionIterator::mpObjectContext->getNode(DSSRWSectionIterator::mNodeKey));
                        if (lpSourceTemplateNode != NULL && lpTargetCGBNode != NULL)
                        {
                            lpSourceTemplateNode->InsertToTargetCGBNodeVec(lpTargetCGBNode);
                            lpTargetCGBNode->InsertToSourceTemplateNodes(lpSourceTemplateNode);
                        }
                    }
                    else
                    {
                        mbAutoPickTopmost = false;
                    }
                }
            }
        }
    }
}