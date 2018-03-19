//
//  AEFilterExpr.h
//  ReportServiceCore
//
//  Created by Taimin Liao on 1/31/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_AEFilterExpr_h
#define ReportServiceCore_AEFilterExpr_h

#include "DSSSliceID.h"
#include "DSSTabularIndex.h"
#include "DSSDataColumn.h"
#include "DSSCubeDataSlice.h"


class DSSCube;
class DSSTabularIndex;
class DSSDataColumn;
class DSSCubeDataSlice;

// This file includes a list of classes that are used for linear scan filter evaluation. 
// The expression is to 
// this is the class/struct that holds the expression  tree
// the expression tree contains the following members:
// each one is a node, with one or more children
// each children can be a node or a slice,
// a slice can be reduced to a unit number and a vector for fast lookup
// If the slice contains 2 or more units in the index, it will stay unchanged as a slice

// Each node is one of AND/OR/NOT, or a slice
enum EnumDssAEFilterBool
{
    AE_FILTER_FALSE = 0x00,
    AE_FILTER_TRUE = 0x01,
    AE_FILTER_NULL = 0x02    
};

class AEFilterAndNode;
class AEFilterOrNode;
class AEFilterNotNode;
class AEFilterSliceNode;
class AEFilterSingleUnitSliceNode;
class AEFilterExprNode;

// NOTE: it is conceptually cleaner to use a vector to hold the pointers of the children
// But the vector is slower than the raw pointer approach, therefore we use mnChildren and mpChildren
class AEFilterExprNode
{
	friend class DSSSimpleExprEval;
public:
	AEFilterExprNode(int iNumChildren);
	virtual ~AEFilterExprNode();
	virtual BYTE Evaluate(int iRow, int inKeys, int *lpKey);
    
	virtual int SetupRowMap(DSSTabularIndex* ipBaseIndex, DSSCube* ipCube, bool &orAllRowMapSetup);
    
	// Find the best single unit slice node that is AND'ed to the rest of the expression
	virtual void FindUnitLimit(int &orUnit, BYTE* &orKeys, double &orSelectPercentage, AEFilterExprNode* &orNode);
	virtual void FindUnitORCondition(int &orUnit, BYTE* &orKeys, AEFilterExprNode* &orNode);
    
	virtual void ReplaceChild(AEFilterExprNode *ipNode, AEFilterExprNode *ipNewNode);
    
	// Get the result of the filter in bit vector format. One indicates true/faslse, the other for null or not
	virtual int GetBitVector(DSSXTabBitVector** oppDataVector, DSSXTabBitVector** oppFlagVector); 
    
	DSSXTabBitVector* AllocateBitVector();
	void ReleaseBitVector(DSSXTabBitVector *ipBV);
	int GetNumChildren();
	AEFilterExprNode* GetChild(int i);
	virtual int GetMaxUnit();
    
	virtual double EstimateCostForBitVectorEval();
	virtual double EstimateCostForRowEval();
protected:
	int mnChildren;
	AEFilterExprNode** mpChildren;
	AEFilterExprNode* mpParent;
	AEFilterExprNode* mpRoot;
	DSSTabularIndex* mpBaseIndex;		// the base index for the expression evaluation.
    
	std::map<DSSXTabBitVector*, bool> mvBitVector;	// a table to store the created bit vectors and if it's currently used
};

class AEFilterAndNode: public AEFilterExprNode
{	
public:
	AEFilterAndNode(int iNumChildren)
    :AEFilterExprNode(iNumChildren)
	{
	}
    
	int GetBitVector(DSSXTabBitVector **oppDataMask, DSSXTabBitVector **oppFlagMask);
    
	// this function is put here so that it becomes inline function
    BYTE Evaluate(int iRow, int inKeys, int *ipKey)
    {
		// false && null = false;
		// true && null = null;
		// null && null = null;
		BYTE lValue = AE_FILTER_TRUE;
        for (int i = 0; i < mnChildren; i++)
		{
			BYTE lChildVal = mpChildren[i]->Evaluate(iRow, inKeys, ipKey);
			if (lChildVal == AE_FILTER_FALSE)
				return AE_FILTER_FALSE;
			else if (lChildVal == AE_FILTER_NULL)
				lValue = AE_FILTER_NULL;
		}
        return lValue;
    }
    
	void FindUnitLimit(int &orUnit, BYTE* &orKeys, double &orSelectPercentage, AEFilterExprNode* &orNode)
	{
		// go to all children, find the one with the least SelectPercentage
		int lBestUnit = -1;
		BYTE *lpBestKeys = NULL;
		double lMinPercentage = 1.1;
		AEFilterExprNode *lpBestNode = NULL;
		for (int i = 0; i < mnChildren; i++)
		{
			int lUnit = -1;
			BYTE *lpKeys = NULL;
			double lPercentage = 0.0;
			AEFilterExprNode *lpNode = NULL;
			mpChildren[i]->FindUnitLimit(lUnit, lpKeys, lPercentage, lpNode);
            
			if (lUnit != -1 && lPercentage < lMinPercentage)
			{
				lBestUnit = lUnit;
				lpBestKeys = lpKeys;
				lMinPercentage = lPercentage;
				lpBestNode = lpNode;
			}
		}
        
		orUnit = lBestUnit;
		orKeys = lpBestKeys;
		orSelectPercentage = lMinPercentage;
		orNode = lpBestNode;
		return;
	}
    
	void ReplaceChild(AEFilterExprNode *ipNode, AEFilterExprNode *ipNewNode)
	{
		AEFilterExprNode::ReplaceChild(ipNode, ipNewNode);
        
		if (mnChildren == 1 && mpParent)
		{
			mpParent->ReplaceChild(this, mpChildren[0]);
			mpChildren[0] = NULL;
            
			// qjiang TQMS468696 mpParent could be deleted by the recursive call above, set it to null should be ok
			mpParent = NULL;
			delete this;
		}
	}
};

class AEFilterNotNode:public AEFilterExprNode
{
public:
	AEFilterNotNode()
    :AEFilterExprNode(1)	// The 'Not' function should have only 1 child
	{}
    
	int GetBitVector(DSSXTabBitVector **oppDataMask, DSSXTabBitVector **oppFlagMask);
    
    BYTE Evaluate(int iRow, int inKeys, int *ipKey)
    {
		BYTE lChildValue = mpChildren[0]->Evaluate(iRow, inKeys, ipKey);
		if (lChildValue == AE_FILTER_TRUE)
			return AE_FILTER_FALSE;
		else if (lChildValue == AE_FILTER_FALSE)
			return AE_FILTER_TRUE;
		else
			return AE_FILTER_NULL;
    }
};

class AEFilterOrNode: public AEFilterExprNode
{
public: 
	AEFilterOrNode(int iNumChildren)
    :AEFilterExprNode(iNumChildren)
	{
	}
    
	int GetBitVector(DSSXTabBitVector **oppDataMask, DSSXTabBitVector **oppFlagMask);
    
	BYTE Evaluate(int iRow, int inKeys, int *ipKey)
    {
		BYTE lValue = AE_FILTER_FALSE;
        for (int i = 0; i < mnChildren; i++)
		{
			BYTE lChildVal = mpChildren[i]->Evaluate(iRow, inKeys, ipKey);
			if (lChildVal == AE_FILTER_TRUE)
				return AE_FILTER_TRUE;
			else if (lChildVal == AE_FILTER_NULL)
				lValue = AE_FILTER_NULL;
		}
        return lValue;
    }
    
	void FindUnitORCondition(int &orUnit, BYTE* &orKeys, AEFilterExprNode* &orNode)
	{
		for (int i = 0; i < mnChildren; i++)
		{
			FindUnitORCondition(orUnit, orKeys, orNode);
            
			if (orNode)
				return;
		}
	}
    
	void ReplaceChild(AEFilterExprNode *ipNode, AEFilterExprNode *ipNewNode)
	{
		AEFilterExprNode::ReplaceChild(ipNode, ipNewNode);
        
		if (mnChildren == 1 && mpParent)
		{
			mpParent->ReplaceChild(this, mpChildren[0]);
			mpChildren[0] = NULL;
            
			// qjiang TQMS468696 mpParent could be deleted by the recursive call above, set it to null should be ok
			mpParent = NULL;
			delete this;
		}
	}
    
};

class AEFilterSingleUnitSliceNode: public AEFilterExprNode
{
public:
	AEFilterSingleUnitSliceNode();
    
	~AEFilterSingleUnitSliceNode();
    
    int Init(DSSCubeDataSlice *ipSlice);
    
    BYTE Evaluate(int iRow, int inKeys, int *ipKey)
    {
        return mpValue[ipKey[mUnit]];
    }
    
	int GetBitVector(DSSXTabBitVector **oppDataMask, DSSXTabBitVector **oppFlagMask);
    
	double EstimateCostForBitVectorEval();
	double EstimateCostForRowEval();
    
	int SetupRowMap(DSSTabularIndex *ipBaseIndex, DSSCube *ipCube, bool &orAllRowMapSetup);
    
	void FindUnitLimit(int &orUnit, BYTE* &orKeys, double &orSelectPercentage, AEFilterExprNode* &orNode)
	{
		orUnit = mUnit;
		orKeys = mpValue;
		orSelectPercentage = mSelectPercentage;
		orNode = this;
	}
    
	void FindUnitORCondition(int &orUnit, BYTE* &orKeys, AEFilterExprNode* &orNode)
	{
		orUnit = mUnit;
		orKeys = mpValue;
		orNode = this;
	}
    
	int GetMaxUnit()
	{
		return mUnit;
	}
    
private: 
	int mUnit;
    BYTE* mpValue;
	int mMaxKey;
	double mSelectPercentage;
};

class AEFilterSliceNode: AEFilterExprNode
{
public:
	AEFilterSliceNode(DSSCubeDataSlice* ipSlice);
    
	~AEFilterSliceNode();
    
	double EstimateCostForBitVectorEval();
	double EstimateCostForRowEval();
    
	BYTE Evaluate(int iRow, int inKeys, int* ipKey)
	{
		double lValue = 0;
		DSSDataFlag lFlag = DssDataOk;
        
		// If there is index row map, use it
		int hr = S_OK;
		if (mpRowMap)
		{
			int lRow = XTabIndexRowMapLookup(mpRowMap, iRow);
			hr = mpColumn->getNumericVal(lRow, lValue, lFlag);
		}
		else
			hr = mpSlice->getNumeric(inKeys, ipKey, lValue, lFlag);
        
		if (hr != S_OK)
			return AE_FILTER_NULL;//throw MBase::ExceptionImplementation<MBase::BaseException>(L"Error getting numerical value in filter evaluation");
		
		if (lFlag != S_OK)
			return AE_FILTER_NULL;
		else if (lValue == 0)
			return AE_FILTER_FALSE;
		else
			return AE_FILTER_TRUE;
	}
    
	int GetMaxUnit()
	{
		return mMaxUnit;
	}
    
	int GetBitVector(DSSXTabBitVector **oppDataMask, DSSXTabBitVector **oppFlagMask);
    
	int SetupRowMap(DSSTabularIndex *ipBaseIndex, DSSCube* ipCube, bool &orAllRowMapSetup);
    
	// a helper function to create the right node for the slice
	static int NewSliceNode(DSSSliceID iSliceID, AEFilterExprNode** oppNode);
private:
    
	int hGetBitVector(DSSXTabBitVector* opDataMask, DSSXTabBitVector* opFlagMask);
	
	template <class DataType>
	int hFastGetBitVector(DataType* ipData, DSSDataFlag* ipFlags, int nRowInBase, int nRowInColumn, DSSXTabBitVector* opDataMask, DSSXTabBitVector* opFlagMask);
    
	DSSCubeDataSlice* mpSlice;
	DSSDataColumn* mpColumn;
	int *mpRowMap;
	int mMaxUnit;
};

// qjiang
// directly access the data and flag array of the column so that we can achieve better performance for MQ
template <class DataType>
int AEFilterSliceNode::hFastGetBitVector(DataType* ipData, DSSDataFlag* ipFlags, int nRowInBase, int nRowInColumn, DSSXTabBitVector* opDataMask, DSSXTabBitVector* opFlagMask)
{
	if (ipData == NULL || ipFlags == NULL)
		return E_POINTER;
    
	int lRow = 0;
	for (int iRow = 0; iRow < nRowInBase; iRow++)
	{
		lRow = XTabIndexRowMapLookup(mpRowMap, iRow);
		AE_ASSERT(lRow < nRowInColumn);
        
		if (ipFlags[lRow] != DssDataOk)
			opFlagMask->Set(iRow, true);
		else if (ipData[lRow] != 0)
			opDataMask->Set(iRow, true);
	}
    
	return S_OK;
}

//xiazhou, offline UC, 04/23/2014.
//A fake node, evaluate always return true/false as indicated.
class AEFilterFakeNode: public AEFilterExprNode
{
public:
	AEFilterFakeNode(int iNumChildren)
    :AEFilterExprNode(iNumChildren), mDefaultVal(AE_FILTER_TRUE)
	{
	}
    
    AEFilterFakeNode(int iNumChildren, EnumDssAEFilterBool iDefaultVal)
    :AEFilterExprNode(iNumChildren), mDefaultVal(iDefaultVal)
	{
	}
    
	// this function is put here so that it becomes inline function
    BYTE Evaluate(int iRow, int inKeys, int *ipKey)
    {
		return mDefaultVal;
    }
    
private:
    EnumDssAEFilterBool mDefaultVal;
};

// Strategy:
// 1) if the lowest level of the filter is still at block level, we use the block level approach
// // Go to the lowest level. Decide whether to use Tree data structure
// Question: what if there is a UL with low selection percentage?
// Let's build something to quickly map a child to its parent
// For each block, remembers its start/end address. If the evalulation falls into this category, return
// data directly

// For each block, find out the end-address. If it's UL and it's false, advance the lowest level block until it is out of 
// bound and mark each block as false. If it's OR and it's true, advance and mark each block as true.

// Let's say, for each EB, we remember the parent. 

// OK, now the algorithm becomes the following:
// 1) Find lowest level. Possibly at row level.
// 2) For the lowest level block, if it's UL, go through the selected blocks, 
// 3) Iterate lowest level block
// 4)     for each lowest level block that is visited
// 5)          get the row number
// 6)          if Key is needed (i.e., exists a node with NULL rowmap), decompress the keys
// 7)          for each UL node, rearrange so that the higher level block is evaluated first
// 8)		   if a UL node evaluates to false, get the block number, find the end address
// 9)          advance the lowest level block until it gets out of the range
// 10)  What if each node knows the key? We don't need to decompress anymore. Let's keep this as further enhancement.

// For now, let's do the following: build a tree to keep track the parent. 
// For each node, build a block of masks.
// Output: a lowest level block level mask

// This is more like a bitvector operation. The memory requirement is larger though. The amount of memory write
// is larger too. But it's simple. Why don't we just do it. Do we need to worry about the mapping or skipping?
// 



// 2) if the lowest level of the filter is at row level, consider two scenarios
//   2.1) Row approach, find out how many rows to visit, may be limited by the unit limit select percentage
//			 for each row, decompress the key, use key to index the KeyTable, 
//        Memory requirement: 1 mask array
//        Cost: SelectPercentage * RowIterationCost (decomp key, key lookup per node, multiple function call per node)
//   2.2) Column approach. 
//			 for each filter node, create a bit mask of values and flag. Size the same as the cube.
//			      for SingleSliceUnit node, for each key that's set, iterate all blocks and set the bits
//        Cost: 3~4 mask array, for each leaf node, need to create the mask array
//

// For each UL, we have two options: 
// 1) go with the lowest level, perform Item, and then evaluate.
//    The number of Item calls are the number of selected lowest level blocks.
// or 2)  
// Now here's our algorithm:
// 1) Find the lowest level block
// 2) if lowest level is UL and select percentage is smaller than some threshold, e.g., 10%, 
//		  initialize the mask array to FALSE, for each selected block, set to UNKNOWN
//    Or simply initialize everything to UNKNOWN
// 4) for each lowest level block
// 4.1)   if LLB mask != UNKNOWN continue;
// 4.2)   if no more expression, mask = true; continue;
// 5)     evaluate OR nodes. If any block is true, find end address EA. Set LLB mask up to EA to true
// 6)     evaluate UL nodes. If any block is false, find end address. Set LLB mask up to EA to false
// 7)     evaluate the rest of the expression. Value is the value of this evaluation result.

// 8) in the end, go through the mask array. If mask = TRUE, it's true, otherwise it's false.

// Case 1: Country in (50% of 10) AND PartType in (1% of 10M)
// Plan1: Initial Country level mask (10), Write PartType level mask (5M fast write)
//      For each selected PartType (100K), check mask, if UNKNOWN, set to True. 
//      Total cost: 10M setup, 5M fast write, 100K random access

// Plan2: Initial Country level mask (10). 
//        For each selected PartType, check mask for Country through parent links. (100K x 3 link accesses)
//      Total cost: 10M setup (1 memset call), 400K random access

// Plan3: For each selected PartType, Item
//      Total cost: 10M setup, 100K Item calls, 
// Winner: Plan2.

// Case 2: Country in (10% of 10) AND PartType in (50% of 10M)
// Plan1: 10M setup, 9M fast write. 5M random access.
// Plan2: 10M setup, 5Mx4 random accesses. 
// Plan3: 10M setup, 5M Item calls. 
// Winner: Plan1.

// Case 3: Country in (50% of 10) AND PartType in (0.1% of 10M)
// Plan2 is the winner
// Plan1: 10M setup, 5M fast write, 
// Plan2: 10M setup, 10K x 4 random access 

// Case 4: Country in (50% of 10) AND PartType in (50% of 10M) AND Date in (0.1% of 10M)
// Plan1: Country (10M setup, 5M fast write.), PartType 5M random write, the l Date 10K random access
// Plan2: 10M setup, 5M random write, 10K random access
// Plan3: 10M setup, 10K Item calls

// Winner: Plan 3. 

// Case 5: Country in (50% of 10) AND PartType in (50% of 10M) AND Date in (50% of 10M)
// Plan1: 5M fast write, 5M random write, and 5M random access
// Plan2: 5M random write, 20M random access
// Plan3: 5M Item calls
// Winner: Plan2.

// Rule of thumb: if there are multiple lowest level conditions, and one of it is UL with small percentage
// go with plan3. 
// Otherwise, for each node, get the block level mask. 
// For higher level blocks, it's better to generate the mask at block level, and then use read.
// For lower level blocks, if there are multiple, it's better to select the lowest percentage one and use Item.


// Design goals for 9.0.1
// 1) mask approach at block level
// 2) For each UL or OR, directly operate on the block level mask
// 3) once the block level mask is calculated, for each block, evalulate the other expressions for each row
// 
// Mask: lpMask, lnBlocks, lpAddress, (mask has lnBlocks entries, with (lpAddress[i], lpAddress[i+1]) as the block
// For each SingleUnitSliceNode, go to all its keys that are selected, and get element index
//      from element index, get the block.
//      if (block at lowest level || lowest level is row level)
//          directly work on it
//		else
//			create a mask array at upper block level
//			from the block, get the lowest level block that it contains
// 0) Find OR expression
// 1) First initialize everything to UNKNOWN
// 2) for OR SingleUnitSlice expressions, set things to TRUE
// 3) For the rest of the expressions, find ULs
// 4) If UL is found, change all UNKNOWN in the mask to FALSE
// 5) for each UL, set visited places to UNKNOWN
// 6) Strip the UL out of the expressions. Go to 0.

// Test cases:
// 1) Country in (...) AND Date in (...)
// 2) Country in (...) OR Date in (...)
// 3) 




// The behavior are 3 types: AND -> initialize to FALSE, set to UNKNOWN
// OR --> initialize to UNKNOWN, set to TRUE
// NOT --> intialize to UNKNOWN, set to FALSE

// 
// For each node, add a function to say: evaluate and write the lowest level block

// Once we get block level mask, we will perform grouping
// Very low percentage ones go to different route

#endif
