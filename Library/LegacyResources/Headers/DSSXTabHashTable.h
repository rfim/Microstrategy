/*
 *  DSSXTabHashTable.h
 *  Currency Converter
 *
 *  Created by Xiaolong Wang on 12/18/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

// 9/5/2012 tehe: optimize for _Node, avoid call constructor
//typedef int _Node;
#include "DSSBaseString.h"

#include "DSSCMMemoryMap.h"

#include <vector>

// 9/5/2012 tehe: optimize for _Node, avoid call constructor
typedef int _Node;

class DSSXTabHashTable:public DSSCMMemoryMap
{
public:
	DSSXTabHashTable();
	~DSSXTabHashTable(void);
	int Init(int nRow, int nUnits, int *pUnitMaxKey);
	
	//put tuple (pkey, value) into hashtable
    //void Hash(int *pKey, int value);
	
    //use pKey to look up hash table, return value
    bool LookUp(int *pKey, int * ipvalue);
	
	//pKey is a patial key, multiple values may be returned
	int PartialMatchLookUp(int *pKey, int nRow, int *opValue);
	
	//Tracing function for debug purpose
	//void Statistics(std::fstream &lFileHandle, int nMinPrintLength);
	
	//construct the hashtable in one step by passing in all keys in ascending order(ibAscSorted = true) 
	//or un-sorted(ibAscSorted = false).
	int HashAll(int *pKey, int nUnits, int nRows);
	
	//Don't use LookUpEx() unless it's guaranteed that pKey is always a valid key (has an entry in the hash table). Use LookUp() instead
	bool LookUpEx(int *pKey, int *value);
	
	// qjiang 05/05/2009 TQMS 357941
	// add size estimation for DSSXTabHashTable
	//int GetSizeMax(UInt64* pcbSize);
	
	int CheckKeyValues(int nUnits, int nRows, int *pKeys, 
						 int *pUnitBucketSpace, int &oNumBuckets, int * pUnitMaxKey, bool ibNeedCalMaxKey=true);
	
	int NumBitSpace(int num);

	int size();
    
    int createMMapMemory(const char * ipFilePath, bool ibUseNewFile = false);
    int releaseMMapMemory();
    int reloadMMapMemory();
    int GetRawSize();

    int setNodeKeys(int* ipKeys);
    
    int Init(int nBuckets, int nUnits, int *pUnitBucketSpace,int iRows, int* ipKeys);

private:
    // Data Members for Class Attributes
    
	//The bucket node
//	struct _Node
//	{
//		int mValue;
//        _Node(int value):mValue(value){};
//		_Node():mValue(0){};
//	};
	
	//lwang, 11/2007, cube performance
	//if _Nodes are sorted or not
	//number of nodes
	int mnNodes;
	
	//count for DEBUG purpose
	int mCompareCount;
	
	//MBase::Buffer* mpBuffer;	
	void* mpBuffer;	
	
	// The number of rows in bucket list
	int mnBuckets;
	
	//The number of the unit
	int mnUnits;
	
	//storage of all buckets
	_Node	*mpNodeList;
	
	// The bucket list
	//_Node	**mBuckets;
	unsigned int *mBuckets;		//the node list offset for each bucket
	
	//The bit mask of bucket space allocated to each unit 
	int *mpBucketMask;
	
	//the bit space of bucket allocated to each unit
	int *mpBucketSpace;
	
	//the offset of bucket bits for each unit
	int *mpUnitBitOffset;
	
	
	//used by partial match implementation
	int *mpWildCardUnits;
    
    int* mpNodeKeys;

    
private:  //## implementation
	
	inline unsigned int HashFunc(int *pKey);
	inline bool IsKeyEqual(int *pKey1, int *pKey2);
	inline bool IsKeyPartialEqual(int *pKey1, int *pKey2);
};



//Hash each key sparately and then concatenate them together to form the bucket offset
//Hash(pkey[i]) = pkey[i] & mpBucketMask[i]

inline unsigned int DSSXTabHashTable::HashFunc(int *pKey){
    unsigned int lBucket = 0;
	//	for (int i = 0; i < mnUnits; i ++)
	//		lBucket |= (pKey[i] & mpBucketMask[i]) << mpUnitBitOffset[i];
	
	int *lpMask = mpBucketMask;
	int *lpBitOffset = mpUnitBitOffset;
	for (int i = -mnUnits; i < 0; i ++)
		lBucket |= ((*pKey++) & (*lpMask++)) << (*lpBitOffset++);
	
	
	return lBucket;
	
}

inline bool DSSXTabHashTable::IsKeyEqual(int *pKey1, int *pKey2){
	//		mCompareCount++;
	for (int i = 0; i < mnUnits; i ++) {
		if (pKey1[i] != pKey2[i])
			return false;
	}
	
	return true;
}

inline bool DSSXTabHashTable::IsKeyPartialEqual(int *pKey1, int *pKey2){
	for (int i = 0; i < mnUnits; i ++) {
		if (pKey1[i] != -100 && pKey1[i] != pKey2[i])	//430482
			return false;
	}
	
	return true;
}
