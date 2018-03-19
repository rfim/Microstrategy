//
//  DSSMaskInfos.h
//  ReportServiceCore
//
//  Created by He Tengfei on 11/16/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#ifndef DSSMASKINFOS_H
#define DSSMASKINFOS_H 

#import <map>
#import <vector>

class DSSCube;
class DSSCubeDataSlice;
class DSSXTabBitVector;
class DSSTabularIndex;
class DFCSimpleExpr;

using namespace std;

#ifndef NULL
#define NULL 0
#endif

class DSSMaskInfos
{
public:
	DSSMaskInfos();
    ~DSSMaskInfos();
    
    int Init(DSSCubeDataSlice* ipFilterSlice, DSSCube* ipCube);
    // enhance this method to accept a generated mask
    int Init(DSSTabularIndex *ipIndex, DSSCube* ipCube, DSSXTabBitVector* ipMask = NULL);
	int Init(DSSTabularIndex *ipIndex, DSSCube* ipCube, DFCSimpleExpr* ipCondition, DSSXTabBitVector* ipMask = NULL);

	
    
	// Get the mask for the index. 
	// If the mask is not there, create a new one but does not calculate
	int GetMask(DSSTabularIndex* ipIndex, DSSXTabBitVector** oppMask, bool* opCalculated);
    
	// Get the mask for the index. If the mask is not there, calculate the mask
	int GetCalculatedMask(DSSTabularIndex* ipIndex, DSSXTabBitVector** oppMask);
    
    // 5/23/2012 tehe: here we will caculate the mask by the parent index which contains all the units in ipIndex
	int GetCalculatedMask2(DSSTabularIndex* ipIndex, DSSXTabBitVector** oppMask);
    
    void AddCaculatedIndexMask(DSSTabularIndex* ipIndex, DSSXTabBitVector* ipMask);
    
    bool FindIndexMask(DSSTabularIndex* ipIndex, DSSXTabBitVector** oppMask);
    
    inline DSSTabularIndex * GetMainIndex() { return mpMainIndex; }
    
    DSSXTabBitVector* GetMainIndexMask();
    void SetMainFilter(DFCSimpleExpr* ipMainFilter);
    
	DFCSimpleExpr* GetMainFilter();
    
	int SetActiveInfos(DFCSimpleExpr* ipCondition);
    
	int GetMaxUnit();
	
	int SetMaxUnit(DFCSimpleExpr* ipCondition, int iMaxUnit);
    
	void Reset();
    
	bool IsEmpty();
	bool IsAllTrue();
    
    DSSTabularIndex *mpMainIndex;
    
    void SetConditionalLogicFlag(bool ibFlag);
    
    bool GetConditionalLogicFlag();
    
    int GetMainFilterMask(DSSTabularIndex *ipIndex, DSSXTabBitVector **oppMask);
    
private:
    
    
    int hAddCondition(DFCSimpleExpr* ipCondition);
    int hCalculateMask(DSSTabularIndex* ipIndex, map<DSSTabularIndex*, DSSXTabBitVector*>* iIndexToMaskMap, DSSXTabBitVector **oppMask);
    
	int mMaxUnit;	// the max level unit in the filter

	map<DSSTabularIndex*, DSSXTabBitVector* > *mIndexToMaskMap;
    
    vector<DSSTabularIndex *> mvCaculatedIndices;
    
    // qjiang 08/18/2010
	// after supporting conditional metric in AE, we should store the maxUnit for each condition
	struct ConditionInfo
	{
		map<DSSTabularIndex*, DSSXTabBitVector* >* mpIndexToMaskMap;
		int mMaxUnit;
	};
    
	typedef map<DFCSimpleExpr*, ConditionInfo> ConditionMask_MAP;
	ConditionMask_MAP mConditionToMaskMap;
    
	DFCSimpleExpr* mpMainFilter; // expression from where clause
    
	DSSCube* mpCube;
    
    bool mbForConditionalLogic;// the mask for the slicing window used by conditional logic would only have bitvector but no ConditionInfo
};

#endif
