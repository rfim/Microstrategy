//
//  DSSXTabRecursiveUnit.h
//  ReportServiceCore
//
//  Created by Sun, Mingmin on 3/16/16.
//  Copyright © 2016 MicroStrategy, Inc. All rights reserved.
//

#ifndef DSSXTabRecursiveUnit_h
#define DSSXTabRecursiveUnit_h
#include "DSSTabularUnit.h"
#include "DSSRecursiveRelation.h"

//class DSS_DIM_STORAGE_EXIM CDSSXTabRecursiveUnit;

class DSSTabularData;
class DSSBinaryDataReader;
struct CRAElement_Extent
{
    int index;
    int originalIndex;
    string ID;
};

class DSSXTabRecursiveUnit : public DSSTabularUnit
{
public:
    DSSXTabRecursiveUnit();
    ~DSSXTabRecursiveUnit();
    virtual int CountRows();
    
    virtual int FindElement(DSSBaseElementProxy *ipElement, int &orKey);
    
    virtual int	createElement(int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement, int iGlobalKey=-1, int iGlobalKeyVer = -1);
    
    virtual bool compareElement(DSSBaseElementProxy *ipElement, int iKey) ;
    
    int loadFromBinary(DSSBinaryDataReader *ipReadStream);
    
    int TransferDisplay2DataKey (int iElement, int & orkey);
    bool IsDisplayed(int iElement);
    bool IsAvailable(int iElement);
    bool IsRecursiveUnit();
    inline int getDataUnit(){return mDataUnitID;};
    
    inline void setTabularData(DSSTabularData* ipTabularData) { mTabularData = ipTabularData;};
    inline DSSTabularData* getTabularData() { return mTabularData;};
    virtual DSSTabularUnit* getChildUnit();
    CRAElement* get_elementRecursiveInfo(int iRow);
    
    int updateExpandStatus(int iElement, bool iExpand, bool iApplyAll = false);
    
    int getCountDisplayChildren(int iElement);
    int Sort(Int32* ipOrder, Int32 iRow, Int32* opElementOrder, bool ifIsDataOrder = false);
    int CountDisplayRows();
    int checkElementType(int iRow, bool* oIsBranch);
    inline bool hasNDEElements() { return mvConsolidationElements.size()>0;};
    int get_ElementOriginalIndex(int iRow, int * lOriIndex);
    void GenData2RecursiveMap(Int32* ipData2RecursiveMap, Int32 iRow);
private:
    //## children's unit indices, now only support one child
    //vector<Int32, MBase::Allocator<Int32> > mvUnits;
    
    //## row # of the displayed elements
    //Int32 mnDispRows;
    Int32 mDataUnitID;
    // map from displayed elements to sequence of tree nodes, size is num of displayed elements
    //Int32* mpMap2Elements;
    
    //## row # of tree nodes
    Int32 mnRows;
    
    //## keys of tree nodes in XTabLookUpTable
    //## there will be duplicated keys in M:M relation
    Int32* mpRows;
    
    // deliu, 2013-11-26, since tree nodes are omitted, cache children status here
    //Int32 mMaxDataKey;
    
   // bool* mpChildrenStatus;
    
    // In the hypothesis that each element have the same child structure in the graph
    // deliu, 2013-11-25, this only survives when generating recursive relation tree
    //vector<CRATreeNode* , MBase::Allocator<CRATreeNode*  > > mvPCNodes;
    
    // list of root nodes as start point for tree traversal
    //vector<CRATreeNode* , MBase::Allocator<CRATreeNode*  > > mvPCRootNodes;
    
    // if there is no need to record the path, Display structure is not needed any more
    // list of display elements/nodes, length is the # of nodes in the tree structure, may be larger than # of lookup table elements when there is M:M relation
    vector<CRAElement* , MBase::Allocator<CRAElement*  > > mvpElements;
    vector<CRAElement_Extent> mvConsolidationElements;
    void setDisplayed(int iRow);
    void setHidden(int iRow);
    void setExpand(int iRow, bool ibExpand);
    
    
    int hSortFormByInputOrder(Int32* ipElements, Int32 iElements, Int32* ipInputOrder, Int32* opElementOrder, bool ifIsDataOrder);
    int hSortRecursiveTree(CRAElement* ipElement, Int32& iSequence, Int32* ipInputOrder, Int32 *opElementOrder, bool ifIsDataOrder);
protected:
    DSSTabularData* mTabularData;
    //bool mfReMap2Global;	// for Re-Map from recursiveUnit-LocalData to recursiveUnit-GlobalData};

};
#endif /* DSSXTabRecursiveUnit_h */
