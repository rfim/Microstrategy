//
//  DSSJoinHelper.h
//  ReportServiceCore
//
//  Created by Tang, Cheng on 11/17/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef ReportServiceCore_DSSJoinHelper_h
#define ReportServiceCore_DSSJoinHelper_h

#include "DSSGUnitContainer.h"
#include "DSSTabularIndex.h"
#include "rwdengineWithJoin/DSSViewDataSet.h"
#include "DSSXTabBitVector.h"

typedef enum EnumRWDJoinFlag
{
    CompoundOuterJoin		= 3,
    CompoundLeftOuterJoin	,
    CompoundRightOuterJoin	,
    CompoundInnerJoin		,
    GridExtension			,
    RWDInnerJoin			,
    RWDOuterJoin			,
    RWDLeftOuterJoin		,
    RWDRightOuterJoin		,
    RWDUnion				
}	EnumRWDJoinFlag;

struct ElementBlockInfo
{
    int mRowInLeft;
    int mStartRowInRight;
    int mEndRowInRight;
};


class DSSJoinUtility
{
public:
	DSSJoinUtility(DSSGUnitContainer* ipContainer, EnumRWDJoinFlag JoinFlag, bool ifForIterator);
    
	int Init(vector<DSSTabularIndex*>& lvIndices, vector<DSSViewDataSet*>& lvViewDataSet, vector<DSSXTabBitVector*>& lvMask);
    
	int get_JoinResult(DSSTabularIndex** oppJoinResult);
    
private:
    
	int hGetGlobaUnitInfo(Int32 iIndex, vector<EnumDSSTemplateUnitType>& ovUnitType, vector<int> &ovGlobalIndex);
    
    int hCompareTwoRows(DSSTabularIndex* ipIndex1, int iRow1, int inColumn1, int *ipJoinColumn1,
                        DSSTabularIndex* ipIndex2, int iRow2, int inColumn2, int *ipJoinColumn2);
    
    int hSortIndexWithMask(DSSTabularIndex* ipIndex, int* ipColumn, int inColumn, DSSXTabBitVector* ipMask, int** oppSortMap);
    
	vector<DSSTabularIndex*> mvIndices;
	vector<DSSViewDataSet*> mvSourceViewDataSet;
	vector<DSSXTabBitVector*> mvMask;
    
	bool mfForIterator;
	DSSGUnitContainer* mpContainer;
	EnumRWDJoinFlag mJoinFlag;
    
};


class DSSJoinHelper
{
public:
    
	//ctang 2010-12-29
	static int join_index(vector<DSSTabularIndex*> &vIndices, 
												vector<DSSViewDataSet*> &vSourceTabularData,
												vector<DSSXTabBitVector*> &vMasks,
												vector<EnumRWDJoinFlag> &vJoinFlag,
												DSSGUnitContainer* ipContainer, 
												DSSTabularIndex** oppJoinResult);

	DSSJoinHelper();
    
	~DSSJoinHelper();
    
};
	
#endif
