/*
 *  DSSTabularIndex.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#ifndef DSSTABULARINDEX_H
#define DSSTABULARINDEX_H

#import "../DSSAEHelper.h"
#import "DSSXTabHashTable.h"
#import<map>
#import "DSSDataModelXTabImpl.h"
#import "DSSXTabBitVector.h"

#include "DSSCMMemoryMap.h"
#include "DSSMemoryGovernorHelper.h" //yimliu, 2014/06/25, class the call MEM_GOVERNOR_RELEASE when it deconstructs.

#ifndef XTAB_SAME_ROWMAP
#define XTAB_SAME_ROWMAP ((int*) -1)
#endif

#ifndef XTAB_CONSTANT_ROWMAP
#define XTAB_CONSTANT_ROWMAP ((int*) -2)
#endif

class DSSTabularIndex;
class DSSRelation;
class DSSXTabKeyIterator;
struct CMIndexInfo;

class ElementIndex;


class ElementIndex:public DSSCMMemoryMap
{
public:
    ElementIndex();
    virtual ~ElementIndex();
    
    int mnElements;
    int* mpElements; // of length mnElements
    int* mpElementAddress; // of length mnElements+1
    int* mpRows; //sorted row index, of length mpIndices->Count()
    DSSTabularIndex *mpIndex;
    
    virtual int createMMapMemory(const char * ipFilePath, bool ibUseNewFile = false);
    virtual int releaseMMapMemory();
    virtual int reloadMMapMemory();
    
    virtual int GetRawSize();
    
    int Size();
};

// tehe 12/9/2011
inline int XTabIndexRowMapLookup(int *ipRowMap, int i)
{
	if (ipRowMap)
	{
		if (ipRowMap == XTAB_SAME_ROWMAP)
			return i;
		else if (ipRowMap == XTAB_CONSTANT_ROWMAP)
			return 0;
		else
			return ipRowMap[i];
	}
	else
		return -1;
}

// tehe 12/9/2011 the type of index should be "int" type
class DSSXTabKeyIterator {
public:
	DSSXTabKeyIterator(DSSTabularIndex *ipIndex, DSSTabularIndex *ipTargetIndex = NULL);
	~DSSXTabKeyIterator();
    
	void NextRow()
	{
		mpCurrentRow += mnSourceUnits;
		return;
	}
    
	void SetRow(int iRow)
	{
		mpCurrentRow = mpKeys + mnSourceUnits * iRow;
		return;
	}
    
	int GetCol(int iCol)
	{
        return mpCurrentRow[iCol];		
	}
    
	void GetRow(int nUnits, int* opKeys)
	{
        memmove(opKeys, mpCurrentRow, mnSourceUnits*sizeof(int));
	}
    
	int LookupTargetRow(int iRow, int *opTargetRow);
    
private:
    int mnSourceUnits;
	int *mpCurrentRow;		
	int *mpKeys;
    
	DSSTabularIndex *mpTargetIndex;
	int *mpLocalKeys;
	int mnTargetUnits;
	int *mpTargetUnits;	// the corresponding position of the units in the TargetIndex
};


class DSSElementBlocks
{
//	friend class CDSSXTabIndices;
public:
	DSSElementBlocks() :mCurrentBlock(0), mpAddress(NULL), mSize(0), mTotalRows(0), mMaxKey(0){}
	
	int Init(Int32 iSize, Int32 iTotalRows, Int32 iMaxKey)
	{
		// if the number of blocks is close to the total number of rows, we set all block size to 1
		mTotalRows = iTotalRows;
		if (iTotalRows <= iSize * (size_t)2)
			mSize = iTotalRows;
		else
		{
			mSize = iSize;
			mpAddress = new Int32[iSize + 1];
			CHECK_NEWED_PTR(mpAddress);
            
			memset(mpAddress, 0, sizeof(Int32) * iSize);
			mpAddress[iSize] = iTotalRows;		// Set the last entry to TotalRows
		}
        
		mpKeyIndex = new Int32[iMaxKey + 2];
		CHECK_NEWED_PTR(mpKeyIndex);
		memset(mpKeyIndex, 0, sizeof(Int32) * (iMaxKey + 2));
        
		mpSortedBlocks = new Int32[mSize];
		CHECK_NEWED_PTR(mpSortedBlocks);
        
		mpRank = new Int32[mSize];
		CHECK_NEWED_PTR(mpRank);
        
		mMaxKey = iMaxKey;
		return S_OK;
	}
    
	// qjiang 05/05/2009 TQMS 357941
	// add size estimation for element blocks
	int GetSizeMax(UInt64* pcbSize)
	{
		AE_ASSERT(pcbSize != NULL);
        
		*pcbSize = sizeof(Int32); // mSize
		*pcbSize += sizeof(Int32); // mTotalRows
		*pcbSize += sizeof(Int32); // mCurrentBlock
		*pcbSize += sizeof(Int32); // mMaxKey
        
		*pcbSize += sizeof(Int32) * mSize; // mpSortedBlocks
		*pcbSize += sizeof(Int32) * mSize; // mpRank
		*pcbSize += sizeof(Int32) * (mMaxKey + 2); // mpKeyIndex
        
		if (mpAddress)
		{
			*pcbSize += sizeof(Int32) * mSize; // mpAddress
		}
        
		return S_OK;
	}
    
	void AddBlock(Int32 iAddress, Int32 iKey)
	{
		_ASSERTE(mCurrentBlock < mSize && iAddress < mTotalRows);
		if (mpAddress)
		{
			mpAddress[mCurrentBlock] = iAddress;
			mCurrentBlock++;
		}
        
		// store the count of each key right now. The real index will be setup in Finalize().
		mpKeyIndex[iKey + 1] ++;
	}
    
	void Finalize(DSSXTabKeyIterator &irIterator, Int32 iPos)
	{
		// do a running sum on mpKeyIndex
		Int32 i = 0;
		for (i = 1; i <= mMaxKey; i++)
			mpKeyIndex[i] += mpKeyIndex[i - 1];
        
		mpKeyIndex[mMaxKey + 1] = mSize;
        
		// At the moment, mpKeyIndex stores the starting location of the each key
		Int32 lKey = 0;
		if (mpAddress)
		{
			Int32* lpAddress = mpAddress;
			for (i = 0; i < mSize; i++)
			{
				// Int32 lKey = ipKeys[lRow * iStride];
				irIterator.SetRow(*lpAddress);						// Get the key from the iterator
				lKey = irIterator.GetCol(iPos);			// Get the key
				mpSortedBlocks[ mpKeyIndex[lKey] ] = i;			// the list of blocks sorted according to the key
				mpRank[i] = mpKeyIndex[lKey];					// the rank of the current block in the sorted array
				mpKeyIndex[lKey]++;
				lpAddress++;
			}
		}
		else
		{
			irIterator.SetRow(0);
			for (i = 0; i < mSize; i++)
			{
				lKey = irIterator.GetCol(iPos);			// Get the key
				mpSortedBlocks[ mpKeyIndex[lKey] ] = i;			// the list of blocks sorted according to the key
				mpRank[i] = mpKeyIndex[lKey];					// the rank of the current block in the sorted array
				mpKeyIndex[lKey]++;
				irIterator.NextRow();
			}
		}
        
		// At the moment, mpKeyIndex points to the end of each key
		// Shift the number right
		for (i = mMaxKey + 1; i > 0; i--)
			mpKeyIndex[i] = mpKeyIndex[i - 1];
		mpKeyIndex[0] = 0;
	}
    
	Int32 GetNumBlocks()
	{	// returns the number of blocks for the attribute
		return mSize;
	}
    
	Int32 GetNumKeys()
	{
		return mMaxKey + 1;
	}
    
	Int32 GetStartAddress( Int32 iBlockNum )
	{	// returns the starting address of the block
		_ASSERTE(iBlockNum < mSize);
		return mpAddress? mpAddress[iBlockNum] : iBlockNum;
	}
    
	Int32 GetBlockByRank( Int32 iBlockRank)
	{
		return mpSortedBlocks[iBlockRank];
	}
    
	Int32* GetStartAddressPtr()
	{
		return mpAddress;
	}
	
	Int32* GetRankPtr()
	{
		return mpRank;
	}
    
	Int32 GetEndAddress( Int32 iBlockNum)
	{	// returns the ending address of the block
		//_ASSERTE(iBlockNum < mSize);
		return mpAddress? mpAddress[iBlockNum + 1] - 1 : iBlockNum;
	}
    
	// Get the blocks for the key. opStart points to the first block, opEnd points
	// to the block after the last block
	void GetBlocksForKey( Int32 iKey, Int32* &opStart, Int32* &opEnd)
	{
		_ASSERTE(iKey <= mMaxKey);
        
		opStart = & mpSortedBlocks[ mpKeyIndex[iKey] ];
		opEnd = & mpSortedBlocks[ mpKeyIndex[iKey + 1]];
	}
    
	void GetBlockRanksForKey( Int32 iKey, Int32 &orStartRank, Int32 &orEndRank)
	{
		orStartRank = mpKeyIndex[iKey];
		orEndRank = mpKeyIndex[iKey + 1];
	}
    
	Int32 GetRank(Int32 iBlock)
	{
		return mpRank[iBlock];
	}
    
	Int32* GetKeyIndexPtr()
 	{
 		return mpKeyIndex;
 	}
    
	Int32 *mpAddress;
private:
	Int32 mSize;
	Int32 mTotalRows;
	Int32 mCurrentBlock;
	Int32 *mpKeyIndex;			// the index to translate key to blocks
	Int32 *mpSortedBlocks;		// the blocks sorted by key value
	Int32 mMaxKey;
	Int32 *mpRank;				// the rank of each block in the sorted list
};


class DSSTabularIndex: public DSSCMMemoryMap
{
    friend class DSSTabularIndexPool;
    
public:
	DSSTabularIndex();
	~DSSTabularIndex();
	
	int Init(int nUnits, int nRows, int *pUnits, int *ipKey, DSSDataModelXTabImpl *ipTabularData, bool ibDelayLoad = false);
    
    int Init(int inUnits, int inRows, int *ipUnits, int *ipKeys);
	
	//return number of rows
	inline int Count();
    int Count(int* opCount);
	//return units
	int getNumUnits();
	int getUnits(int  nUnits, int *pUnits);
	int getUnits(vector<int> &orvUnits);
	
	//get keys
	int Item(int iRow, int nKeys, int *opKey);
	int LookUp(int nKeys, int *ipKey, int *opRow);
	
	int GenDistinctC(vector<int> &ivUnits, DSSTabularIndex *pIndices, int *opRowMap);
	
	//int PartialMatchLookUp(int nUnits, int *pIndex, int nSize, int *opSize, int *opOffsets);
	//int PartialMatchLookUpEx(int nUnits, int *pIndex, int nSize, int *opSize, int *opOffsets);
	//int PartialMatchFind(int nUnits, int *pIndex, int *pMask);
	
	int ValidateLookUpEx(int nUnits);
	//Don't use LookUpEx() if pKey is NOT always logically valid (valid means pIndex).
	int LookUpEx(int nUnits, int *ipKey, int *pKeyBuffer, int *opRow);
    
    
	int ValidateItemEx(int nUnits);
	int ItemEx(int iRow, int nKeys, int *opKey);
	
	inline const int * getKeyTable() {return mpKeys;}
	
	int getKey(int iRow, int iUnit);
	
	int size();
    void UpdateTimeStamp();
    void toString();
    
	int PartialMatch(const vector<int> &iUnits, const vector<vector<int> > &elements, int iUnit, vector<int> &iCandidateElements);
    int CalculateMask(int nUnits, int *ipUnits, vector<int>* ipvElements, DSSXTabBitVector& orBitVector);
    
    
    //hyan,03/02/2012
    int CalculateMaskUsingRelation(int iIndexUnit, GUID* iIndexGUID, int iNonIndexUnit, GUID* iNonIndexGUID,
                                vector<int>& ivNonIndexUnitElement, DSSRelation* ipRelation,DSSGUnitContainer *ipGUnitContainer, DSSXTabBitVector& orBitVector,
                                   vector<int> *ipLocal2Global = NULL);
    
    
    
    
    //ctang : sort index with mask
    int SortWithMask(int nUnits, int * ipUnitPos, DSSXTabBitVector* ipMask, int** oppSortedRowMap);
    
    DSSXTabBitVector* GetUnqualifiedRows();
    DSSXTabBitVector* GetHiddenRows();
    
    // tehe: return whether the row is unqulified given the row index
    bool IsUnqualified(int iRow);
    
    bool IsSameOrder(DSSTabularIndex* ipIndex);
    
    
    //hyan, transform global index(which get from GlobalLookUp using GUID) to local Index using mpUnits
    int TransformGlobalIndexToLocalIndex(int iGlobalIndex,int *oplocalIndex);
    
    // tehe
    int LookupActualKey(int *ipKeys, int *opRow);
    
    //zhyang
    int loadBitVectorFromBinary(DSSBinaryDataReader * ipReadStream);
    
    //msun
    int loadHiddenMaskFromBinary(DSSBinaryDataReader * ipReadStream);
    
     int setDistinct(bool ibDistinct);
    
    //junwang, debug
    void dump();
    
    int setHeaderKeys(vector<int>& ivKeys, vector<int>& ivRowList);
    
    // 7/6/2012 tehe
    int GetElementsForUnit(Int32 iUnit, Int32 *pCount, Int32 **pElements);
    
    //zhyang, 31/8/2012, oprimization for maxkey
    int getMaxKey(int iUnit);
    int setMaxKeys(vector<int> ivMaxKeys);
    int addKeyDistribution(int *lpKeyDistribution);
    
    // 3/1/3013 tehe 
    void setMainIndexOrder(bool ibMainIndexOrder) { isMainIndexOrder = ibMainIndexOrder; }
    void setSortByLastUnit(bool ibSortByLastUnit) { mfSortByLastUnit = ibSortByLastUnit; }
    
    // tehe: avoid key copy
    void setKeys(int *ipKeys, int length);
        
    inline bool IsMainIndexOrder() {return isMainIndexOrder;}
    int Populate(DSSTabularIndex *pOriginal,
                 Int32 *pUnitsMap,
                 Int32 nMissingUnit,
                 Int32 nRowLimit,
                 Int32 ibElementBrowseCount,
                 Int32** ippRowMap);
    bool IsDistinct();
    DSSElementBlocks* getElementBlocks(int iUnit);
    bool hasElementBlocks();
    void SetUnqualified(int iRow, bool iFlag);
    int DecompressKeys(DSSXTabBitVector* ipMask, Int32 nUnits, Int32* pUnits,
                       Int32** ippGlobalLUIndex, Int32* ipRowMap, Int32 nResultRow,
                       vector<Int32*>& ovKeys, Int32 iDefaultValue = -1);
    
    int ResetHeaderKeys(int* ipKeys, int iSize, int inRows, bool lbReCalKeyDistribution = true, bool lbMatchKeys = false);

    
    // 1/4/2013 tehe: support model cache manager
    inline void setNumUnits(int inUnits) { mnUnits = inUnits; }
    int Init(MBase::CString iFilePath, const CMIndexInfo * ipIndexInfo);
    int GetMaxKeyDistribution(char **oppBinary, int *opSize);
    void GetCMIndexInfo(CMIndexInfo **oppCMIndexInfo);
    int SetMaxKeyDistribution(const char * ipMaxKeyDistribution, int iDataSize, int &oMaxKeyDistributionSize);
    
    
    // 5/8/2013 tehe
    int hGetElementIndexForUnit(int iUnitPos, ElementIndex** oppElementIndex);
    inline vector<ElementIndex*>* GetElementIndices() {return mpvElementIndices; }
    int SetElementIndex(int iUnitIndex, ElementIndex *ipElementIndex);
    
    int ClearElementIndex();
    
    int createMMapMemory(const char * ipFilePath, bool ibUseNewFile = false);
    int releaseMMapMemory();
    int reloadMMapMemory();
    int GetRawSize();
    
    //854903, fali, this method is to decide if the index is ordered linear key, which means there's only one unit and the key is ordered from 0, like 0,1,2,....n
    bool isOrderedLinearKey();
private:
	//int CheckKeyValues(int nUnits, int nRows, int *pKeys, int *pUnitBucketSpace, int &oNumBuckets, int * pUnitMaxKey = NULL);
	//int NumBitSpace(int num);
    
    	
	// New logic for GenDistinctC to boost performance
	int hCollapse(int nUnits, int * pUnits, int ** pKeys, //MBase::Buffer * p, 
                  int * pDistinct, int& iorSize, vector<int>& orUnqualifiedRows, int *opSortResult,
                  bool iRawSortResult);
	void hCalcKeyDistributions(vector<int *> *ipKeyColumns = NULL, bool iCalcDummyOnly = false);
    void CalcKeyDistributions(int* ipKeyCol, int iMaxKey, int nRow, int* opDist);
    int hConvertUnitID(vector<int> &ivUnits, vector<int> &ivPos);
    int Collapse(int nUnits, int * ipUnitPos, int **ippKeys, int * pDistinct, int& iorSize, 
                 int* opRowMap, bool iRawSortResult);
    
    int hBuildElementIndexForUnit(int iUnitPos, ElementIndex** oppElementIndex);
    
    // 3/1/2013 tehe: avoid create hashtable if we don't do looking up
    int hCreateHashTable();
    void hCalcMaxKey();
    
	int *mpKeys;
	int mnRows;
	int mnUnits;
	int *mpUnits;
	int *mpLookUpKeyBuf;
    bool isMainIndexOrder;
    bool mIsDistinct;
    bool mfSortByLastUnit;
	
	std::vector<int> mvMaxKeys;
	std::vector<int*> mvKeyDistributions;
    
	DSSXTabHashTable* mpHashTable;
    std::map<int, int> mMapRow2PartialIndex;
    std::vector<int> mvRowList;
    
    std::vector<int> mvMapRow2PartialIndex; //junwang, 2012/07/25
    bool mbUseMap; //junwang, 2012/07/25
    
    bool mbPartialLoading;
    DSSDataModelXTabImpl *mpTabularData; 
    
    DSSXTabBitVector mUnqualifiedRows;
    DSSXTabBitVector mHiddenRows; // added for financial report
    vector<ElementIndex*>* mpvElementIndices;
    
    friend class DSSXTabKeyIterator;
    friend class ElementIndex;
    
    // 9/5/2012 tehe
    vector<int> mvElementCounts;
    
    vector<DSSElementBlocks*> mvElementBlocks;
    
    DSSElementBlocks* mpRootBlock;

};

inline int DSSTabularIndex::Count()
{
    return mnRows;
}
#endif
