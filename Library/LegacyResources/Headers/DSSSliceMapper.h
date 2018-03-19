//
//  DSSSliceMapper.h
//  ReportServiceCore
//
//  Created by He Tengfei on 11/15/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#ifndef DSSSLICEMAPPER_H
#define DSSSLICEMAPPER_H

#include <stddef.h>

#include <vector>
#import "DSSrwdconstants.h"

struct DSSModelInfo
{
    int mModelIndex;
    int mTRMetricID;
    int mnVariable;
    int *mpVariables;
    EnumDSSObjectType *mpVariableTypes;
    int *mpIndexMetricSlices;
};

class DSSSliceMapper 
{
    // tehe 4/19/2012 : avoid memory leak
	struct DSSTaggedSlices
	{
        DSSTaggedSlices():mpNext(NULL),mpSliceID(NULL) { }
		int mTag;
		EnumDssSliceFlag mSliceFlag;
		int * mpSliceID;
        EnumDSSRuntimeSubtotalType mExtremeType;
        DSSModelInfo *mpModelInfo;
		DSSTaggedSlices * mpNext;
        ~DSSTaggedSlices() { if(mpSliceID) delete [] mpSliceID; }
	};
    
	struct DSSLevelLookup
	{
        DSSLevelLookup():next(NULL),mlevel(NULL) {}
		int index;
		bool *mlevel;
		DSSLevelLookup *next;
	};
    
    
public:
    
    DSSSliceMapper(int levelNum, int MetricNum, int AttributeNum);
    
    
    ~DSSSliceMapper();
    
    
    //	Adds a reference to slice <SliceID> containing values of
    //	the metric <metric> at level <level> with tag <tag>
    //int AddSlice(bool *level, int tag, EnumDssSliceFlag SliceFlag, int metric, int SliceID, int levelID);
    int AddSlice(bool *level,
                 int tag,
                 EnumDssSliceFlag SliceFlag,
                 int metric,
                 int SliceID,
                 int levelID,
                 EnumDSSRuntimeSubtotalType lExtremeType = DssRuntimeSubtotalDefault,
                 int iModelIndex = -1,
                 int iTRMetricID = -1,
                 int nXVariable = 0,
                 int *ipXVariables = NULL,
                 EnumDSSObjectType *ipXVariableTypes = NULL,
                 int *ipXIndexMetrics = NULL);
    
   
    //	In case of success (S_OK) returns the Id of the slice
    //	that contains metric <metric> at level <level> with tag
    //	<tag>
    //	If not found returns S_FALSE
    int SearchID(bool *level, int tag, EnumDssSliceFlag SliceFlag, int metric, int *SliceID, EnumDSSRuntimeSubtotalType *opExtremeType = NULL);
    
    
    //	returns the number of tags in the level
    int Count(int levelID, int *count, EnumDssSliceFlag iSliceFlag = DssOriginalSlice);
    
   
    //	return the i-th tag in the j-th level
    int Item(int Ilevel, int ITag, bool **level, int *tag, EnumDssSliceFlag iSliceFlag = DssOriginalSlice, DSSModelInfo **oppModelInfo = NULL);
    
  
    //	hash functrion for fast level lookup
    int levelHashFunc(bool *level);
    
    //	performs fast level lookup though an hash table
    //	return 0 and the level index if found, 1 if not found.
    int FindLevelInHash(bool *level, int *Index);
    
    //	adds a new level to the structure and returns its index
    int AddLevelInHash(bool  *level, int *Index);
    
    //	return the i-th tag in the j-th level
    int GetLevel(int levelID, bool **level);
    
    //	returns the number of tags in the level
    int CountLevels(int *count);
    
    // Additional Public Declarations
    
    // int Item(int levelID, bool **level);
    
    
    void ClearBuffer();
    
    // 5/21/2012 tehe: return S_OK, if we can find the level, else return S_FALSE
    // int getTagsByLevel(bool *iLevel, std::vector<int> & ovTags);
    int getSlicesByLevel(bool *iLevel, std::vector<int> &ivMetrics, std::vector<int> & ovSliceID);
    int AddLevel(bool  *level, int *Index, int levelID);
    
    int ReplaceSliceByLevel(bool* level, int iSliceID, int iMetric, int iTag, EnumDssSliceFlag iSliceFlag);

protected:
    // Additional Protected Declarations
    
private:
    
    //	searches for a particular tag in the indexed level
    //	returns 0 and the tagged slice structure if found,
    //	1 if not found
    int FindTag(bool *level, int tag, EnumDssSliceFlag SliceFlag, DSSTaggedSlices **Slices);
    
    //	returns 0 and the tagged slice structure if found,
    //	1 if not found
    int FindTag(int Index, int tag, EnumDssSliceFlag SliceFlag, DSSTaggedSlices **Slices);
    
    //	adds a tag to the indexed level and returns a pointer to
    //	the tagged slice structure
    int AddTag(int Index,
              int tag,
              EnumDssSliceFlag SliceFlag,
              EnumDSSRuntimeSubtotalType ExtremeType,
              int iModelIndex,
              int iTRMetricID,
              int nXVariable,
              int *ipXVariables,
              EnumDSSObjectType *ipXVariableTypes,
              int *ipXIndexMetrics,
              DSSTaggedSlices **Slices);
    
    // Data Members for Class Attributes
    
    //	open hash table for fast level lookup
    //	each slot contains a linked list of levels that map to
    //	that slot
    DSSLevelLookup **mLevelHash;
    //	array of tag lists grouped by level (each element in a
    //	list pertains to the same level)
    DSSTaggedSlices **mpLevel;
    DSSTaggedSlices **mpLevel2;
    DSSTaggedSlices **mpLevel3;
    
    //	this array maps level indexs to the actual level
    //	definitions
    bool *mpLevelMap;
    int mMetricNum;
    //	maximum number of levels thet can be inserted in this
    //	structure (defined at construction time)
    
    int mMaxLevelNum;
    //	index of the next level to be inserted (and current
    //	number of levels inserted)
    int mNextLevelIndex;
    //	dimension of the hash table for fast level lookup
    //	(currently 2x the max number of levels)
    
    int mLevelHashDim;
    //## end DSSSliceMapper::mLevelHashDim.attr
    
    
    //	number of tags in each level
    
    int *mpTagNum;
    int *mpTagNum2;
    int *mpTagNum3;
   
    int mAttributeNum;
       
    // Additional Private Declarations
    enum {SliceMapBlock = 190};
      
    // 6/19/2012 tehe, value of LevelID come from calculation plan, level index is used by slicemapper, we can use level id to get level info from calculation plan, level info contains units and subtotal info, however level index in slicemap just used to difference how many different levels there are. we can use level index to get level info from slice map, but the level info just has the units info
    std::vector<int> mMapLeveID2LevelIndex;
private:  
    // Additional Implementation Declarations
    // compares two levels
    bool cmpLevels(bool *lvl1, bool *lvl2);
public:
    int Init(void);
    
};

// Class DSSSliceMapper 





#endif