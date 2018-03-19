/*
 *  DSSXTabHeader.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import <map>
#import "DSSAEHelper.h"

#include "Base/Base/Buffer.h"
#include "Base/Base/Allocator.h"

class DSSXTabView;
class DSSTabularIndex;
class DSSTabularIndexPool;
class DSSBucketSort;
class DSSXTabHashTable;
class DSSDataModelXTabImpl;
class DSSTabularCustomGroup;
class DSSXTabBitVector;

class DSSTabularConsolidation;
class DSSXTabRecursiveUnit;

class DSSXTabHeader
{

public:
	DSSXTabHeader();
	~DSSXTabHeader();
    
    int Init(vector<int> &irvUnits, int nRuntimeUnits, int iMetricID, int nRows,
			 vector<DSSTabularIndex*> &irvpIndice, vector<int> &irvSubtotalID, DSSDataModelXTabImpl *ipXTabModel);
    
	int getNumUnits();
	int getUnits(int nUnitSize, int *opUnits);
	int getUnit(int index);
	vector<int>& getUnits();
	
	/*!
	 @method     
	 @abstract  return the number of levels
	 */
	inline int getLevelCount();
	
	/*!
	 @method     
	 @abstract  return level ID at the give position, if iRow is out of bounary, return -1
	 */
	int getLevel(int iRow);
	
	/*!
	 @method     
	 @abstract  return level ID given the unit list and subtotal ID, if the input is invalid, return -1
	 */
	int getLevelByUnits(vector<int> &irUnits, int iSubtoalIndex);
	
	/*!
	 @method     
	 @abstract  get subtotal ID at the given position, return HR value
	 */
	int getSubtotalID(int iRow, int &orSutotalID);
	
	/*!
	 @method     
	 @abstract  get subtotal ID at the given level, return HR value
	 */
	int getSubtotalIDByLevel(int level, int &orSutotalID);

	/*!
	 @method     
	 @abstract  get XTabIndex at the give position, return HR value
	 */
	int getIndex(int iRow, DSSTabularIndex **oppIndex);
	
	/*!
	 @method     
	 @abstract  get XTabIndex at the give level, return HR value
	 */
	int getIndexByLevel(int level, DSSTabularIndex **oppIndex);
	
	/*!
	 @method     
	 @abstract  get the offset in the corresponding level XTabIndex, if iRow is out of bounary, return -1
	 */
	int getOffsetOfIndex(int iRow);
	
	int FillIndex(int iRow, int nUnit, int *pIndex, int *pMetric);
    int Fill_Index(int iRow, int nUnit, int *pIndex, int *pMetric);
	int FillIndexWithTag(int iRow, int nUnits, int *pIndex, int *pMetric);
	virtual int getUnitKey(int iRow, int iUnit);
	
	
	int getHeaderElements(int iRowCount, const int *iRowList, int nUnits, bool *ipbHierDisp,
						   vector<int> &orUnitID, vector<vector<int> > &orBlock, vector<vector<int> > &orEleKeys);

	int getRowCount();
	
	bool HasMetric();
	
	int getMetricID(int iRow);
	
	int getNumRuntimeUnits();
	
	int getOffsetByElements(std::map<int, int> &irKeys);

	int getOffsetByElements(vector<int> &irKeys);
    
    int isFirstMetricCell(int iRow, bool* pIsFirstorNonMetricCell);
    
	//return i, where mvDispRows[i] is different from mvDispRows[iRow] only by their metrics, attribute elements are the same
	inline int AdjustRowByMetricID(int iRow, int iMetricID);
	
	int size();
	
	int setGroupbyMetric(int nSelectedMetrics, vector<bool> &irvSelectedMetrics);
    
    //zhyang, 12/30/2011, subtotal sort, other kinds of sorts are implemented in sorter classes
    // int sort();
    
    //zhyang
    int fillIndexUsingElements(Int32 *ipKeys, map<int, int> &imElementIDs, int &iTag, int *ipChildKey);
    int getLevelByElements(std::map<int, int> &irKeys);
    int createMMapMemory(const char * ipFilePath, bool& ibUseNewFile);
    
    int replaceRowByCertainSubtotal(int iRowID, int iLevel, int iSubtotalID, int iMetricID);
    int getNumUnitsForCertainRow(int iRowID, vector<int> &ivTargetUnits);
    int getDisplayOffsetByElements(vector<int> &irKeys);
private:
	
	int hCreateUnitMappingTable(DSSDataModelXTabImpl *ipModel);
	
	int hGetHeaderElementsOfCG(int iCurrentRow, DSSTabularCustomGroup *ipCustomGroup,
							   int iKey1, int iKey2, int iUnitStartDepth, int iUnitEndDepth,
							   bool ibfDistinctPrev, vector<vector<int> > &orvColumnBlocks, vector<vector<int> > &orvColumnElements);
	
	int hGetUnitDepth(vector<int> &orvUnitDepth);
	
	int hGetCustomGroup(int *ipColUnit, int iSize, DSSTabularCustomGroup ** oppCG);
	
	DSSXTabView *mpView;		//this is a weak reference
	
	int mnDispRows;			//number of rows to be displayed
	vector<int> mvDispRows;	//the acutall order to be displayed
	
	int mnRows;					//mnRows doesn't count metrics
	vector<int> mvOffset;		//each row's offset within the index
	vector<short> mvLevelID;	//mvElementSubtotalLevels;
    short *mpLevelID;
    
	vector<char> mvTotalID;	//mvTag;		//subtotal ID for each row, TODO: keep or not?
	vector<int> mvLevelOffset;          //the first row's offset of each level, used to replace mvOffset for optimization.
                                        //to get the offset for iRow, we use this formula:
                                        //      offset = iRow - mvLevelOffset[mvLevel[iRow]]; 
	
	int mnLevels;						//total number of levels
	vector<DSSTabularIndex *> mvIndices;	//index for each level
	vector<int> mvSubtotalID;			//subtotal's (local) ID for each level
	vector<int> mvUnitMapToIndex;		//map from header unit to each level's index unit, size = mnUnits * mnLevels
	
	int mnUnits;			//number of units of the header
	vector<int> mvUnits;	//units of the header
	int mnTabularUnits;		//number of total units of the grid
	int mnRuntimeUnits;		//runtime units that push-down to header because of Group-by units not on template
	vector<int> mvUnitDepth;//unit start depth

	bool mbHasMetric;		//if header units include metrics unit
	int mMetricsUnitID;		//the metrics unit ID
	int mnMetrics;			//number of metrics
	
	bool mbHasGBMetric;		//has group-by metric
	vector<int> mapToMetricID;	//when there's group-by metric, we need an extra reference
    
    void * mpMMPBuffer;
    int mMMPBufSize;
	
    bool mHasUnqualifiedRows;
	// slicing window
/*	int mnGBUnits;
	vector<int> mvGBUnits;
	vector<int> mvGBEleKey;
	int mnGBEntries;
	int * mGBIndex;
	int* mGBKeys;
	int * mpMaxUnitValue, *mpMinUnitValue;
	int * mSlices;
	DSSXTabHashTable* mpCHashTable;
	bool mbGBMetric;
*/	
 
// tehe 2011-11-16 for new rwd
public:
    int init(); // 8/9/2012 tehe: it will be called when there's no gb units
    
    int Init(DSSXTabView *pView, int nLevel, int *pTag, DSSTabularIndex **ppIndices, int nColUnit, int *pColUnit, int nAttrUnit, bool fMetric, int nMetric, int iRumtimeUnitCount, DSSTabularIndex **ppChildIndices, vector<bool>& irvHasRecursiveUnit);
	
    int InitBySelection(int nIndices, DSSTabularIndex** pIndices, DSSXTabBitVector** pBitVectors, int iHasMetricGroupBy, vector<int> ivMetrics, vector<int> *pTags);
    
    int CreateUnitMappingTable();
    int FillIndexEx(int iRow, int nUnit, int *ipKeys, int *pMetric, DSSTabularIndex **oppIndex, int *opOffset, int *ipChildKey);
    int InitMetricGroupBy(int iHasMetricGroupBy, vector<int> ivMetrics);
    
    int GetCurrentGBAxisRange(vector<int>& orRows);
    
// tehe 2011-11-16 for new rwd
private:
    bool hGetOffsetFlag();
    int hCountConsolidationAndRecursiveUnits(void);
    int hCheckDisplayable(int iRow, int iLevel, bool &ofDisplay);
    
    // 8/9/2012 tehe
    int hResetSlicingBuffer();
    
    // tehe
    int hInitMetrics(int iHasMetricGroupBy, vector<int> ivMetrics);

    //no attribute selector, has metric selector only
    bool mbIsNewRWD;
    int mHeaderWidth;
    int mnDispUnits;
    vector<int> mvDispUnits;

    int hResetXTabBuffer();
    
    int mnConsolidationUnit;
    
    vector<int> mvConsolidationUnit;
    vector<DSSTabularConsolidation *> mvConsolidation;

    //msun, 3/29/2016, financial reporting.
    int mnRecursiveUnits;
    vector<int> mvRecursiveUnits;
    vector<DSSXTabRecursiveUnit*> mvRecursiveAttr;
    vector<bool> mvHasRecursiveUnit;
    vector<int*> mvHasRecursiveUnits;
    vector<DSSTabularIndex *> mvIndices2;
    
    // 3/22/2012 tehe: 
    vector<bool> mvEnableTotalForCS;
    vector<bool> mvTotalStateForCS;
    friend class DSSXTabView;
    
    int mDummyUnitIndex;
    
    // 8/9/2012 tehe
    bool mbPreXtab;
    bool mbInitialized;
    map<DSSTabularIndex*, Int32*> *mMapIndexToTarget; // tehe: index has consolidation unit in it 
    
    // tehe: data member for slice window
    int mnCurRow;
    vector<int> mvCurRow; // tehe: currrent display rows, value of them directly means the offset in total rows (all levels)
    int mnCurDispRow;
    // tehe: if cur disp row map is null, which means the data is not sliced( has no selector include metric selector, will not run into initmetricgroupby or initby selection).
    int* mpCurDispRowMap;
    
    bool mbOffsetFlag; // tehe: if mbOffsetFlag is true, we will use mvOffset, else we will not init mvOffset, we will use mvLevelOffset instead of mvOffset.
    int mReserveMemorySize ;
};

inline int DSSXTabHeader::getLevelCount()
{
	return mnLevels;
}

inline int DSSXTabHeader::AdjustRowByMetricID(int iRow, int iMetricID)
{
	if (!mbHasMetric)
		return iRow;
	else
		return iRow + (iMetricID - iRow % mnMetrics);
}
