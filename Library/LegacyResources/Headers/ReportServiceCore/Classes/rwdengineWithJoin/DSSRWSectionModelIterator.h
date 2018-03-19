/*
 *  DSSRWSectionModelIterator.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 9/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef RWSectionModelIterator_H
#define RWSectionModelIterator_H

#include <vector>
#include <set>
#include <string.h>
#include "GUID.h"
#include "DSSRWNodeModelIterator.h"
#include "DSSIPhoneEnums.h"
#include "DSSRWSectionIterator.h"
#import "DSSXtabHeader.h"
#import "DSSXtabView.h"
#import "DSSRWDEngine.h"

using MBase::operator <;
class DSSRWSectionModel;
class DSSBaseElementProxy;
class DSSGroupByUnit;
class DSSBaseElementsProxy;
class DSSRWCompare;
class DSSRWSectionCompare;

class DSSRWSectionModelIterator : public DSSRWNodeModelIterator, public DSSRWSectionIterator
{
public:
	
	DSSRWSectionModelIterator();
	
	virtual ~DSSRWSectionModelIterator();
    
    
    //init the member variables
    virtual int init(DSSRWDataModel* ipDataModel, DSSRWNode* ipNode, DSSRWNodeModel* ipModel,
                     DSSRWSectionModelIterator* ipParent, bool isAll=false);
	
    
    //Get child node and create child node model's iterator
    virtual int Item(std::string iNodeKey, DSSRWIterator** oppIterator);
    
    //inherited from DSSRWSectionIterator
    virtual int Item(int iChild, DSSRWIterator** oppIterator, bool* opbNew = NULL);
    
    //used by child-iterator
    virtual int getGroupByElements(vector< vector<int> >& ovvGroupByElements); //merge current GroupByeElements and mvSelectedElements
    virtual int getGroupByMetrics(set<GUID, less<GUID> >& osGroupByMetrics);  //merge current GroupByMetrics and msCurrentMetrics
    
    virtual int getGroupByElements(vector< vector<int> >& ovvGroupByElements, int iChildBranch);
    
    int getSelectedElements(std::vector<int>& ovSelectedElements); //assign mvSelectedElements
    
    
    //interfaces same with DSSRWSectionIterator
    
    int ReInit(bool DeleteChildren = false, bool iIsAll = false);
	
	/*!
	 @method
	 @abstract   return the number of recurrence under the current group-by path
	 @discussion return 1 if it's not repreated section (DssRWSectionGroupBySection)
	 */
	int CountRows();
	
	/*!
	 @method
	 @abstract   move to the first repeat section
	 @discussion
	 */
	bool FirstRow();
	
	/*!
	 @method
	 @abstract   move cursor to the next repeat section
	 @discussion if cursor is at the last one, return false
	 */
	bool NextRow();
	
	/*!
	 @method
	 @abstract   return the number of children
	 @discussion
	 */
	int Count();
    
    /*!
	 @method
	 @abstract  get available commands from handle
	 @discussion
	 */
	int getCommandList(DSSCellHandle &irHandle, vector<EnumManipulationCommandTypes> &orvCommand);
    
    
    /*!
	 @method
	 @abstract   get the special field group iterator to support conditional formatting
	 @discussion
	 */
	DSSRWFieldGroupIterator* getSpecialFieldGroupIterator();
    
    
    /*!
	 @abstract   get the child iterator
	 @discussion the difference from Item() is that it won't create new child if it doesn't exist
	 */
	DSSRWIterator * ItemLight(int iChild)
	{
		return (iChild >= 0 && iChild < mnChildren && mCurrent < mvChildren[iChild].size()) ? mvChildren[iChild][mCurrent] : NULL;
	};
    
    bool get_IsReplaced()
	{
		return mIsReplaced;
	}
    
	
	DSSBaseElementProxy * get_ReplacedElement()
	{
		if (mvReplacedElements.size() > 0)
			return mvReplacedElements[0];
		else
			return NULL;
	}
	
	int CountReplacedElements()
	{
		return (int)mvReplacedElements.size();
	}
    
    //hyan,16/8/2012
    //TQMS 631195,644873
    //will be called in selectorviewer.mm
    //if(lpIterator->CountCandidateElements()>0)  within method handleEvent
    int CountCandidateElements()
    {
        return (int)mvCandidateElements.size();
    }
	
	DSSBaseElementProxy * ItemReplacedElement(int i)
	{
		if ( i < 0 || i >= mvReplacedElements.size())
			return NULL;
		else {
			return mvReplacedElements[i];
		}
	}
    
    
    virtual int GetNGBSelectedRow();
    
    virtual int RefreshChildIterator(DSSRWIterator *ipChildIterator, DSSRWNode *ipChildNode);
    
    int getLevel()
    {
        return mLevel;
    }
    int getSectionType()
    {
        return mSectionType;
    }
    bool isAll()
    {
        return mIsAll;
    }
    
    virtual int FindTag(vector<GUID>& ivGroupByUnits, int* opTag, int iChildBranch=-1);
    
    virtual int getCurrentBranch()
    {
        return mCurrent;
    }
    
    int toString(std::ostream& out);
    
    //junwang, 03/02/2012,
    void updateGroupbyElements(const vector<vector<int> >& ivvGroupbyElement);
    //junwang, 06/21/2012, #614304
    void updateGroupbyMetrics(const set<GUID, less<GUID> >& isGroupbyMetrics);
    
    
    //hyan,2012/03/12
    bool getEmptySelectFlag();
    
    DSSRWSectionModelIterator* getParentIterator()
    {
        return DSSRWNodeModelIterator::mpParentIterator;
    }
    
    int PopulateNGBElements(vector<DSSBaseElementProxy*> & iovNGBElements,DSSRWNodeModel *ipNodeModel);
    
    
    //hyan,2/22/2013
    virtual int getNGBIndexRowCount();
    
    //tliao, 05/28/2013
    virtual int SetNGBWindow(int iStart, int iEnd);
    virtual int ClearNGBWindow(bool DeleteChildren = true);
    virtual bool IsNGBAll();
    // weilin US51811 2016/07/20 refresh child grid iterators
    virtual int RefreshChildGridIterators();

    // weilin US51811 2016/07/19 replay autopick and refresh child iterator
    int ReplayAutoPickAndRefreshChildIterator();

    // weilin US51811
    bool getSkip();
    void setSkip(bool ibTemp);

    // weilin US51811
    int hGetTopmostPosInCandidate(string iTemplateKey, GUID iUnitId, bool ibAllowTotalSelected);
private:
    //hyan,2/27/2013, add for TQMS 723014
    virtual int hRefreshChildIterators();

    // weilin US51811 2016/07/19 autopick topmost
    bool mbGotAutoPickProperty;
    bool mbAutoPickTopmost;
    
    // weilin US51811 2016/07/19 Get autopick property
    void hGetAutoPickTopmostProperty(const NodeAndControlsVector* ipSources, string iTemplateKey);

    // weilin US51811 2016/07/19 Mark whether we skip reinit or nots
    bool mbSkip;
protected:
    
    //for NGB and non-metrics selector
    int hGetCurrentGBElementsUseExpr(DSSGroupByUnit* ipUnit, bool* opUseExpr, DSSBaseElementsProxy** oppElements);
    
    //for metrics CGB
    int hProcessMetricsCGB(DSSRWNode* ipNode, DSSGroupByUnit* ipUnit, EnumDSSCurrentElementStatus iStatus);
    
    
    //calculate current elements according to status{selected, desired, unset, subtotal}.
    int hGetCurrentElements(vector<int>& ovCurrentElements);
    
    //current elements for chosen and desired status
    int hGetCurrentNGBElementsFromSelection(const vector<int>& ivCandidateElements, vector<int>& ovCurrentElements);
    int hGetCurrentCGBElementsFromSelection(const vector<int>& ivCandidateElements, vector<int>& ovCurrentElements);
    int hGetCurrentUCElementsFromSelection(const vector<int>& ivCandidateElements, vector<int>& ovCurrentElements);
    
    //default elements for unset status
    int hCalDefaultElementsNGB(const vector<int> &ivCandidateElements, vector<int>& ovDefaultElements);
    int hCalDefaultElementsCGB(const vector<int> &ivCandidateElements, vector<int>& ovDefaultElements);
    int hCalDefaultElementsUC(const vector<int> &ivCandidateElements, vector<int>& ovDefaultElements);
    
    int hSortCandidateElements();
    
    //to be removed
    int hGetCurrentElementsOld(vector<int>& ovCurrentElements);
    int hSortCandidateElementsOld();
    int hGetCurrentElementsNGB(const vector<int>& ivCandidateElements, vector<int>& ovCurrentElements);
    int hGetCurrentElementsCGB(const vector<int>& ivCandidateElements, vector<int>& ovCurrentElements);
    int hGetCurrentElementsUC(const vector<int>& ivCandidateElements, vector<int>& ovCurrentElements);
    int hFindDefaultElements(const vector<int>& ivCandidateElements, vector<int>& ovDefaultElements);
    
    //for sort
    virtual int hget_ElementIndex(int iRow, int iUnit, int *pElementIndex);
    
    //Conditional logic support
    virtual int AnalyzeTransactionThreshold(DSSThreshold* ipThreshold, bool* opResult);
private:
    
protected:
    
    vector<int> mvCandidateElements; //global key
    
    vector<vector<int> > mvGroupByElements; //global key
    vector<int> mvSelectedElements; //local selected index of mvCandidateElements, for NGB the size 1
    
    //set<MBase::GUID, less<MBase::GUID> > msGroupByMetrics;
    //set<MBase::GUID, less<MBase::GUID> > msCurrentMetrics;
    set<GUID, less<GUID> > msGroupByMetrics;
    set<GUID, less<GUID> > msCurrentMetrics;
    
    int   mParentNGB; //the selected row for the nearest parent NGB Node in NGBPathInfo
    
    vector<int>* mpCandiateInBase;
    
    //for subtotal
    vector<int> mvDefTags;
    set<int> msTags;
    
    //for NGB
    int mPageByStyle;
    int mLevel;             //the index of current NGB Node in the NGBPath, 0-based
    std::vector<int> mvRow; //the row ids of mvCandidateElements in NGBPathInfo
    int mNGBChildBranch;    //used when parent is NGB All
    
    //hyan,2012/03/08
    bool mbEmptySelectFlag;
    
    friend class DSSRWSectionModel;
    friend class DSSRWControlModel;
    friend class DSSRWCompare;
    friend class DSSRWSectionCompare;
};

#endif