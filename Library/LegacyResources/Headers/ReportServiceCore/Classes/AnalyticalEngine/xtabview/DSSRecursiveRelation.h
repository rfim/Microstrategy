//
//  DSSRecursiveRelation.h
//  ReportServiceCore
//
//  Created by Sun, Mingmin on 3/17/16.
//  Copyright © 2016 MicroStrategy, Inc. All rights reserved.
//

#ifndef DSSRecursiveRelation_h
#define DSSRecursiveRelation_h

#define CHECK_NEWED_PTR(ptr) {if (!ptr) return E_OUTOFMEMORY; }
// parent-child table
class DSSRecursivePCTable
{
public:
    
    // deliu, 2014-04-22, add IsHidden column for report filter
    typedef enum {DssRelationParent = 0, DssRelationLevel, DssRelationHasChild, DssRelationIsHidden} RelationType;
    
    DSSRecursivePCTable(MBase::Buffer *pBuffer) : mpBuffer(pBuffer)
    , mnRows(0), mpParents(NULL)
    , mpLevels(NULL), mpHasChildren(NULL)
    , mpIsHidden(NULL)
    {};
    
    HRESULT Init(Int32 iRowCount)
    {
        HRESULT hr = S_OK;
        mnRows = iRowCount;
        
        if (iRowCount < 0)
            return S_FALSE;
        
        mpIndex = new (mpBuffer) Int32[mnRows];
        CHECK_NEWED_PTR(mpIndex);
        
        // deliu, display attribute as flat list when there is no recursive relation table
        // set all the elements as root as default
        for (Int32 i = 0; i < mnRows; i++)
            mpIndex[i] = i;
        
        mpParents = new (mpBuffer) Int32[mnRows];
        CHECK_NEWED_PTR(mpParents);
        memset(mpParents, -1, sizeof(Int32) * mnRows);
        
        mpLevels = new (mpBuffer) short[mnRows];
        CHECK_NEWED_PTR(mpLevels);
        memset(mpLevels, 0, sizeof(short) * mnRows);
        
        mpHasChildren = new (mpBuffer) bool[mnRows];
        CHECK_NEWED_PTR(mpHasChildren);
        memset(mpHasChildren, 0, sizeof(bool) * mnRows);
        //for(Int32 i = 0; i < mnRows; i++)
        //	mpHasChildren[i] = true;
        
        mpIsHidden = new (mpBuffer) bool[mnRows];
        CHECK_NEWED_PTR(mpIsHidden);
        memset(mpIsHidden, 0, sizeof(bool) * mnRows);
        
        return S_OK;
    }
    
    virtual Int32 get_RowCount();
    
    //get children's index
    virtual Int32* get_Children();
    
    // get the elements' parents
    virtual Int32* get_Parents();
    
    virtual short* get_Levels();
    
    virtual bool* get_ChildrenFlags();
    
    virtual bool* get_HiddenFlags();
    
    HRESULT  SetChildren(Int32 iStart, Int32 iEnd, Int32* ipChildren, Int32 iRowCount);
    
    HRESULT  SetParents(Int32 iStart, Int32 iEnd, Int32* ipParents, Int32 iRowCount);
    
    virtual HRESULT  SetRelationData(Int32 iStart, Int32 iEnd, BYTE *pData,
                                                      Int32 iRowCount, DSSDataType_Type iTargetType, Int32 iRelationType);
    
    HRESULT  Save7u(ICDSSWriteBlockStream *pStm, Int32 fClearDirty, Int32 Flag);
    
    HRESULT  Load7u(ICDSSReadBlockStream *pStm, Int32 Flag);
    
    HRESULT  SelectInto(DSSRecursivePCTable* ipPCTable, Int32 nRows, Int32* pRows, Int32* lpNewChildren, Int32* lpNewParents, Int32* ipOrig2NewIND, std::set<Int32>* ipHidden = NULL);
    
    HRESULT  SetHiddenFlag(Int32 iRow, bool iValue);
    
    HRESULT  PopulateFromRelationTable(DSSRecursivePCTable* ipPCTable, Int32 iRows, Int32* ipRows);
    
    HRESULT  get_Parent(Int32 index, Int32* opParentIndex);
    
protected:
    
    DSSRecursivePCTable() : mpBuffer(NULL)
    , mnRows(0), mpParents(NULL)
    , mpLevels(NULL), mpHasChildren(NULL)
    , mpIsHidden(NULL)
    {}
    
    //## Record num of elements in the P-C Table.
    // for (1:M), mnRows is equal to Rows in the Attribute; for (M:M), is num of (ParentID, ChildID)
    Int32	mnRows;
    
    //## Index of children
    Int32*	mpIndex;
    
    //## index of parents, -1 means no parent
    Int32*	mpParents;
    
    //## Level may be absolute level or relative level. If no filter, it is absolute level;
    //## if there is filter, it will be relative
    // not needed any more, should be deleted
    short*	mpLevels;
    
    //## For ROLAP, query engine doesn't provide it, assume every node is true; MDX has the children col
    bool*	mpHasChildren;
    
    //## 865219, IsHidden flag play a role in report filter. If IsHidden is true, elements only included in sort part but will not be displayed
    bool*	mpIsHidden;
    
    MBase::Buffer* mpBuffer;
    
};


// Node of PC-Tree
struct CRATreeNode
{
    CRATreeNode(MBase::Buffer *ipBuffer) : mIndex(-1), mnChildren(0), mSourceLevel(-1)
    , mvChildren(MBase::Allocator<Int32 >(ipBuffer) )
    , mfExpanded(true), mfDisplayed(true)
    , mfIsHidden(false), mParent(-1)
    {};
    
    CRATreeNode(const CRATreeNode& iCRANode)
    {
        mIndex = iCRANode.mIndex;
        mSourceLevel = iCRANode.mSourceLevel;
        //mfHasChildren = iCRANode.mfHasChildren;
        mnChildren = iCRANode.mnChildren;
        mvChildren.resize(mnChildren);
        
        for (Int32 i = 0; i < mnChildren; i++)
            mvChildren[i] = iCRANode.mvChildren[i];
        
        mfExpanded = iCRANode.mfExpanded;
        mfDisplayed = iCRANode.mfDisplayed;
        mfIsHidden = iCRANode.mfIsHidden;
        mParent = iCRANode.mParent;
    }
    
    // offset in the lookup table
    Int32	mIndex;
    
    // absolute level from the source
    short	mSourceLevel;
    
    // be true even if children is not loaded from data source
    //bool	mfHasChildren;
    
    Int32	mnChildren;
    // the children should be sorted, whether to use set is needed to be considered
    // may be empty when HasChildren is true because children is not loaded from data source yet
    vector<Int32 , MBase::Allocator<Int32 > > mvChildren;
    
    // deliu, using expand flag to indicate whether to display children
    bool	mfExpanded;
    
    // ldong 2/5/2014 use mfDisplayed flag to indicate if this element should be displayed, mainly for DE element
    // we don't display branch element with ordinal -1, or concrete element with ordinal -1 and has duplicate elements with positive ordinal
    // if concrete element with ordinal -1, but no duplicate elements, we should display it
    bool	mfDisplayed;
    
    // deliu, 2014-04-22, 865219, when there is report filter, filtered rows still exist in LookUpTable with hidden flag true.
    // If hidden, elements will be included in sort but not be displayed
    bool	mfIsHidden;
    
    // mchen, 2/3/2016, add parent info
    Int32 mParent;
};


// display element of recursive attribute
struct CRAElement
{
   
    
    // index in the lookup table, index of last element in the dispay path
    Int32 mIndex;
    
    // relative level in PC-Tree, length of mvDisplayPath - 1
    short mDepth;
    
    // current display status, dynamically modified
    bool mfIsExpand;
    
    // deliu, no path info is needeed now
    //vector<Int32 , MBase::Allocator<Int32 > > mvDisplayPath;
    
    // indicated whether to display or not. true-display; false-not display
    bool mfIsDisplayed;
    
    // row # of first child, -1 if no children
    Int32 mFirstChild;
    
    // row # of the next sibling, -1 if no
    Int32 mNextSibling;
    
    bool mfIsHidden;
    
    // mchen, add parent info, -1 if no parent
    Int32 mParent;
    
    bool mfIsAvailable;
};

#endif /* DSSRecursiveRelation_h */
