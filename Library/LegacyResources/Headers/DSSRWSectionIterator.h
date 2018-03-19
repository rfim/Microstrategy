/*
 *  DSSRWSectionIterator.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 2/23/10.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import "DSSRWIterator.h"
class DSSBaseElementProxy;
class DSSRWFieldGroupIterator;
class DSSDataModelGroupbyImpl;

class DSSRWSectionIterator : public DSSRWIterator
{
public:
	DSSRWSectionIterator();
	virtual ~DSSRWSectionIterator();
	
	int Init(DSSRWNode *ipNode, DSSRWSectionIterator * ipParent, vector<DSSBaseElementProxy*>& ivNGBElements);
	int Init(DSSRWNode *ipNode, DSSRWSectionIterator * ipParent, vector<DSSBaseElementProxy*>& ivNGBElements, bool iIsAll);
	
	virtual int ReInit(bool DeleteChildren = false, bool iIsAll = false);
	
	/*!
	 @method     
	 @abstract   return the number of recurrence under the current group-by path
	 @discussion return 1 if it's not repreated section (DssRWSectionGroupBySection)
	 */
	virtual int CountRows();
	
	/*!
	 @method     
	 @abstract   move to the first repeat section
	 @discussion
	 */
	virtual bool FirstRow();
	
	/*!
	 @method     
	 @abstract   move cursor to the next repeat section
	 @discussion if cursor is at the last one, return false
	 */
	virtual bool NextRow();
	
	/*!
	 @method     
	 @abstract   return the number of children
	 @discussion 
	 */
	virtual int Count();
	
	/*!
	 @method     
	 @abstract   get the child iterator
	 @discussion
	 */
	virtual int Item(int iChild, DSSRWIterator** oppIterator, bool* opbNew = NULL);

	/*!
	 @abstract   get the child iterator
	 @discussion the difference from Item() is that it won't create new child if it doesn't exist
	 */
	virtual DSSRWIterator * ItemLight(int iChild)
	{
		return (iChild >= 0 && iChild < mnChildren && mCurrent < mvChildren[iChild].size()) ? mvChildren[iChild][mCurrent] : NULL;
	};
	
	/*!
	 @method     
	 @abstract  get available commands from handle
	 @discussion
	 */
	virtual int getCommandList(DSSCellHandle &irHandle, vector<EnumManipulationCommandTypes> &orvCommand);
	
	/*!
	 @method     
	 @abstract   get the special field group iterator to support conditional formatting
	 @discussion
	 */
	virtual DSSRWFieldGroupIterator* getSpecialFieldGroupIterator();
	
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
    //declare this virtual function in sectioniterator and we will overload in sectionmodeliterator
    //to avoid incompatible within the check in selectorviewer.mm 
    // if(lpIterator->CountCandidateElements()>0)  within method handleEvent
    //here we return 1  (1>0).
    virtual int CountCandidateElements()
    {
        return 1;
    }
    
    //hyan,2/22/2013
    virtual int getNGBIndexRowCount();
    
	//tliao, 06/09/2013
    virtual int SetNGBWindow(int iStart, int iEnd);
    virtual int ClearNGBWindow(bool DeleteChildren = true);
    virtual bool IsNGBAll() {return mIsAll;}
    
    
	DSSBaseElementProxy * ItemReplacedElement(int i)
	{
		if ( i < 0 || i >= mvReplacedElements.size())
			return NULL;
		else {
			return mvReplacedElements[i];
		}
	}

	/*!
	 // inline function, return the current NGB element if it's a NGB section iterator; return NULL otherwise
	 */
	inline DSSBaseElementProxy * getCurrentNGBElement() { return mCurrent < mvGBElements.size() ? mvGBElements[mCurrent] : NULL;};
	
	virtual int RefreshChildIterator(DSSRWIterator *ipChildIterator, DSSRWNode *ipChildNode);
    
    // weilin 2016/07/20
    string getNodeKey();

    // weilin 2016/08/02
    int getCurrent();

    EnumDSSRWSectionType getSectionType();
	
virtual int FindChildIterator(std::string &irNodeKey, vector<int> &irvPath, DSSRWIterator **oppChildIterator);

    bool IsGroupbyAll();
    
	/*!
	 @abstract		add or remove an interator from the iterator tree
	 @discussion	1. ipNode is the one to be added or removed.
     2. right now, we only support remove or add a CGB/UC section node, which only has one child
	 */
    int UpdateIteratorRemove(DSSRWNode* ipNode, std::vector<DSSRWIterator*>* ipIteratorsRmved=NULL);
    int UpdateIteratorAdd(DSSRWNode* ipNode);
    
protected:
	
	virtual int hGetNGBPathForChild(DSSRWIterator *ipChildIterator, vector<int> &orvNGBPath);
	
	int hSetPageByElement(int iPageByStyle, vector<DSSBaseElementProxy *> &irvRealElements, bool ibIsUnset);
	
	void hClearGBElements();
	
    DSSDataModelGroupbyImpl* hGetGroupByModel();
    
	int mnRows;
	vector<int> mvRows;
	int mCursor;

	int mnChildren;
	vector<vector<DSSRWIterator*> > mvChildren;
	bool mIsReplaced;
	vector<DSSBaseElementProxy*> mvReplacedElements;
	EnumDSSRWSectionType mSectionType;
	vector<DSSBaseElementProxy*> mvGBElements;	// if this is ngb, use this to record elements.
	int mCurrent;	// current status of ngb iterator
	bool mIsAll;	//if parant NGB is ALL
	
	DSSRWFieldGroupIterator* mpSpecialFieldGroupIterator;
};
