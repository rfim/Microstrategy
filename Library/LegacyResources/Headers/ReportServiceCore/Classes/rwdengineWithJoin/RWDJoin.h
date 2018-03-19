//
//  RWDJoin.h
//  ReportServiceCore
//
//  Created by Tang, Cheng on 11/16/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef ReportServiceCore_RWDJoin_h
#define ReportServiceCore_RWDJoin_h

#include "DSSTabularIndex.h"
#include "DSSXTabBitVector.h"
#include "DSSGUnitContainer.h"
#include "DSSViewDataSet.h"
#include "GUID.h"
#include <set>
using MBase::operator<;

typedef std::set<GUID, std::less<GUID> > DSSIDSet;

class IndexMask
{
public:
    DSSTabularIndex* mpIndex;
    DSSXTabBitVector mMask;
    DSSIDSet msTotalUnits;

    // weilin DE39755 2016/09/29 add ipBuffer to avoid memory leak
    IndexMask(DSSTabularIndex* mpIndex, int inRow, MBase::Buffer* ipBuffer = NULL);
};


class JoinedIndexTable
{
private:
    int hBuildColumnMap(DSSTabularIndex* ipIndex, int* opMap);
    int InitColumn(vector<DSSViewDataSet*>& ivViewDataSet, vector<vector<IndexMask*> >& ivMask);
    int InitColumn(vector<DSSViewDataSet*>& ivViewDataSet);
    
    int InitRows(int inIndex, DSSTabularIndex** ippIndices);
    
public:
    DSSGUnitContainer* mpGunitContainer;
    DSSObjectContext * mpObjectContext; // jimzhang Jul/16/2012 Purpose of adding this member is to get column name in toSting()
    //the index table
    std::vector<int> mvColumnMap;
    int mnRow;
    int* mpKey;
    //for optimization of one index case. hold the index
    DSSTabularIndex* mpIndices;
    
    //the optimization for one index case
    int* mpMap;
    int mnColumn;
      
    JoinedIndexTable(DSSGUnitContainer* ipContainer, DSSObjectContext *ipObjectContext = NULL);
    ~JoinedIndexTable();
    int Init(int inIndex, DSSTabularIndex** ippIndices, vector<DSSViewDataSet*>& ivViewDataSet);
    
    int Init(int inIndex, DSSTabularIndex** ippIndices, vector<DSSViewDataSet*>& ivViewDataSet, vector<vector<IndexMask*> >& ivMask);
    
    int GetKey(int iRow, int iColumn, int* opKey);
    
    void toString();
};

class DSSMultipleLevelJoinHelper
{
private:
    DSSGUnitContainer* mpGunitContainer;
    
    DSSObjectContext* mpObjectContext;  // jimzhang Jul/16/2012 Purpose of adding this member is to get column name in toSting()
    
    //ctang 2011-7-20 join index directly
	int GenJoinedCubes(vector<DSSViewDataSet*> &ivViewDataSet,
                           vector< vector<IndexMask*> > &vMasks,     // each vector<IndexMask*> represents the masks for indices of a single dataset
                           bool *ipPrimary,
                           int iStart,
                           int iEnd,
                           vector<DSSTabularIndex*>& ovrJoinedIndex,
                           vector<DSSXTabBitVector*>& ovrMasks,
                           vector<DSSIDSet >& ovrTotalUnits,
                           bool *opPrimary,
                           // rzhu, 07/07/2011, change join type if it is non-detail section
                           bool lfDetailSection /* indicate if it is detail section */, 
                           vector<GUID>& lvGBUnits /*GB units, used only if lfDetailSection==false*/, 
                           vector<bool>& hasConcreteOnly /*used only if lfDetailSection==false,
                                                          for each GB units, indicate where the GB elements contains contrecte only,
                                                          if there is NULL, Total, the value is false*/,
                           set<GUID>& lsUnitIDs /*io parameters, indicate the unit ids of join result*/);
    
	int IsJoinable(DSSTabularIndex* ipIndices1, DSSViewDataSet* ipViewDataSet1, DSSIDSet& isTotalUnits1, DSSTabularIndex* ipIndices2, DSSViewDataSet* ipViewDataSet2, DSSIDSet& isTotalUnits2, bool* opJoinable);
    
	int hGetUnitIDs(DSSViewDataSet* ipViewDataSet, DSSTabularIndex* ipIndices, DSSIDSet& isUnitIDs);
    
	int hSliceOneIndices(DSSTabularIndex* ipIndices, DSSViewDataSet* ipViewDataSet, DSSXTabBitVector* ipMask, DSSTabularIndex** oppIndices);    
    
public:
    DSSMultipleLevelJoinHelper(DSSGUnitContainer* ipGUnitContainer, DSSObjectContext *ipObjectContext = NULL);
    
    int JoinWithMask(vector<DSSViewDataSet*>& ivViewDataSet, 
                     vector<vector<IndexMask*> >& ivMask,
                     bool* ipPrimary,
                     JoinedIndexTable** oppJoinedIndexTable,
                     // rzhu, 07/07/2011, change join type if it is non-detail section
                     bool lfDetailSection/* indicate if it is detail section */,
                     vector<GUID>& lvGBUnits/*GB units, used only if lfDetailSection==false*/,
                     vector<bool>& hasConcreteOnly/*used only if lfDetailSection==false,
                                                   for each GB units, indicate where the GB elements contains contrecte only,
                                                   if there is NULL, Total, the value is false*/,
                     bool ifInitFromIndex = false);
};

#endif
