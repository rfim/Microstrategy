/*
 *  DSSTabularIndexPool.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#ifndef DSSTABULARINDEXPOOL_H
#define DSSTABULARINDEXPOOL_H

#import "DSSAEHelper.h"
#import "DSSDataModelXTabImpl.h"
#include <list>

class DSSTabularIndex;
class DSSViewDataSet;

class DSSTabularIndexPool
{
    
public:
	DSSTabularIndexPool();
	~DSSTabularIndexPool();
    
    int Init(DSSDataModelXTabImpl *pTabularData, bool ifCubeReport = false);
    int Init(int iUnits, bool ifCubeReport = false);
	
	int CreateIndex(int nUnits, int nRows, int *pUnits, int *ipKey, DSSTabularIndex **oppIndex, bool ibLoadbySlice = false);
	int getIndex(int nUnits, int *pUnits, DSSTabularIndex **oppIndex);
    
	int size();
	
	int setMainIndex(DSSTabularIndex *ipMainIndex);
    
	int getMainIndex(DSSTabularIndex **ipMainIndex);
	inline DSSTabularIndex* getMainIndex(){return mpMainIndex;};
    
    int setDisplayMainIndex(DSSTabularIndex *ipDispMainIndex);
    int getDisplayMainIndex(DSSTabularIndex **ipDispMainIndex);
    inline DSSTabularIndex* getDisplayMainIndex(){return mpDisplayMainIndex;};
    
	bool Search(int nUnits, int *pUnits, DSSTabularIndex **oppIndex);
    
    void toString();
    
    int InsertToPool(DSSTabularIndex* pIndices);

    DSSTabularIndex* getSpecialIndex();     //to keep backward compatibility for old RWD engine
    
    //junwang, 2012/05/31, 
    int createMMapMemory(const char * ipFilePath, bool ibUseNewFile);
    
    // 8/23/2012 tehe
    int releaseMMapMemory();
    int reloadMMapMemory();
    inline bool getMMapStatus() { return mMMPStatus; }
    inline void setMMapStatus(bool ibMMapStatus) { mMMPStatus = ibMMapStatus; }
    void setBinaryKeys(int iBinaryKeys);

	void GetIndexRowMap( DSSTabularIndex* ipBase, DSSTabularIndex* ipTarget, int** oppRowMap);
    int AddIndexRowMap(DSSTabularIndex* ipBase, DSSTabularIndex* ipTarget, int** oppRowMap = NULL);   

    vector<list<DSSTabularIndex*> >* getIndexList(){return mpvIndexList;}
    int GenerateNewIndex(int nUnits, int *pUnits, DSSTabularIndex **oppIndex);
    inline bool IsInitialized() {return mpTreeRoot != NULL;}
private:
	int hUpdate(int nUnits, int *pUnits, DSSTabularIndex **oppIndex);
	int hInsertIndex(DSSTabularIndex *ipIndex);
	
    typedef struct {
        DSSTabularIndex* mpBaseIndex;
        DSSTabularIndex* mpTargetIndex;
        int* mpRowMap;
    } RowMapSt;
    
    vector< RowMapSt, MBase::Allocator<RowMapSt> > mvRowMaps;

	DSSTabularIndex *mpMainIndex;
    DSSTabularIndex *mpDisplayMainIndex;
    DSSDataModelXTabImpl* mpDataModelXTab;
    //use a tree and a vector to replace this one
	//vector<DSSTabularIndex *> mvIndexList;
    
    Int32 mnBinaryKeys;
    Int32 mnUnits;
    
    typedef struct node
    {
        struct node* left;
        struct node* right;
        DSSTabularIndex* pIndex;
        
        node(): left(NULL), right(NULL), pIndex(NULL){}
    } Node;
    
    Node* mpTreeRoot;
    bool mfCubeReport;
    
    typedef list<DSSTabularIndex*> XTabIndexList;
    vector<XTabIndexList> *mpvIndexList;
    vector<DSSTabularIndex*> mvIndices;//only used by old engine logic
    
    int hTranslateToBinaryKeys(Int32 nUnit, Int32 *pUnits, bool *opBinaryKeys);

    
    int getMininumBaseIndex(int nUnits, int *pUnits, DSSTabularIndex **oppIndex);
    int InsertToTree(DSSTabularIndex *pIndices);
    int InsertToIndexList(DSSTabularIndex *pIndices);
    int deleteNode(Node *node);

    //DSSTabularData *mpTabularData;
    //DSSViewDataSet *mpViewDataset;

	DSSTabularIndex *mpGrandTotalIndex;     //to keep backward compatibility for old RWD engine
    
    bool mMMPStatus; // tehe: indicate whether the mmap has created
};

#endif
