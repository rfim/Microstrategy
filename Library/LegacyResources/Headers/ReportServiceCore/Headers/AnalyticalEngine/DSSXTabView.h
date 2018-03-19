/*
 *  DSSXTabView.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import <map>
#import <set>

#import "DSSAEHelper.h"
#import "DSSTabularIndex.h"
#include "DSSCalculationPlan.h"
#include "DSSIPhoneEnums.h"

class DSSXTabHeader;

class DSSViewMap;
class DSSCubeDataSlice;
class DSSDataModelXTabImpl;
class DSSTabularUnit;

//class DSSRWGridModel;
class DSSSliceMapper;
class DSSXTabLayoutInfo;
class DSSXTabDispInfo;
class DSSGBTable;

class DSSMaskInfos;
class DSSModelInfo;
class DSSCube;

class DSSAttributeThresholdItems;
class DSSAttributeThresholdIterator;

typedef int DSSXTabKey;

#define HEADCOUNT 2

class ExtremeInfo
{
public:
    ExtremeInfo():mpMin(NULL), mpMax(NULL),
    mMinSize(0), mMaxSize(0),
    mDataFlag(DssDataOk){}
    ~ExtremeInfo(){}
    unsigned char *mpMin;
    unsigned char *mpMax;
    Int32 mMinSize;
    Int32 mMaxSize;
    DSSDataFlag mDataFlag;
};

class ForecastInfo
{
public:
    ForecastInfo():mpFWBeginElements(NULL), mpBWEndElements(NULL),
    mnFWPoints(0), mpFWXElements(NULL), mpFWLabels(NULL),
    mnBWPoints(0), mpBWXElements(NULL), mpBWLabels(NULL),
    mnVariables(0)
    {}
    ~ForecastInfo(){}
    
    Int32 mnVariables;
    //Int32 *mpVariables;
    //EnumDSSObjectType *mpVariableTypes;
    Int32 *mpFWBeginElements; // length of mnVariables
    Int32 *mpBWEndElements;  // length of mnVariables
    Int32 mnFWPoints;
    Int32 **mpFWXElements;     //lenght of mnVariables * mnFWPoints, only if the element key is -1, we use label in mpFWLabels
    string **mpFWLabels;         //lenght of mnVariables * mnFWPoints
    Int32 mnBWPoints;
    Int32 **mpBWXElements;     //lenght of mnVariables * mnBWPoints, only if the element key is -1, we use label in mpBWLabels
    string **mpBWLabels;         //lenght of mnVariables * mnBWPoints
};


class DSSXTabView
{
public:
	DSSXTabView();
	~DSSXTabView();
	
	int Init(DSSDataModelXTabImpl *ipXTabModel);
    
	DSSXTabHeader *getHeader(int iAxis);
	int get_Header(int iAxis, DSSXTabHeader** oppHeader);
	int CreateSliceMap(int nRowLevels, int nColLevels, int nMetrics);
	int AddSlice(int iCubeSliceID, int iLevel, int iMetric);
	int getDataSlice(int iRowLevel, int iColLevel, int iMetric, DSSCubeDataSlice **oppSlice);
    
    int addAttributeThresholdSlice(int iLevel, int iTag, const DSS_ID * ipAttrID, int iSliceID);
    int getAttributeThresholdSliceInfo(int iLevel, int iTag, const DSS_ID* ipAttrID, int* opSliceID);
    int getAttributeThresholdSliceByLevel(bool *iLevel, std::vector<int> &ovSliceID);
    
	/*!
	 @method
	 @abstract   return the number of metric levels in XTab View
	 @discussion
	 */
	inline int CountLevels(){return mnRowLevels * mnColLevels;};
	
	/*!
	 @abstract	get the subtotal ID, and level units. Return S_OK if succeed
	 @param	iLevel	input
	 param orTotalID	subtotal id at this level
	 param orvUnits		a list of tabular unit's id, 0-based
	 */
	int getLevelInfo(int iLevel, int &orTotlaID, vector<int> &orvUnits);
	
	/*!
	 @abstract	return the slice ID for a given metric at a level
	 */
	int getCubeSliceID(int iLevel, int iMetric);
	
	/*!
	 @abstract	return the numeric value of a metric cell
	 @discussion if the metric type is not numeric, return E_UNEXPECTED_DATATYPE
	 */
	int GetCellValue(int RowIndex, int ColIndex, double &orVal, DSSDataFlag &orDataFlag,
					 int *opThreshold = NULL, int *opMetric = NULL);
	
	/*!
	 @abstract	return the raw data of a metric cell
	 @discussion if the metric type is not numeric, return E_UNEXPECTED_DATATYPE
	 */
	int GetRawCellInfo(int RowIndex, int ColIndex, const char **oppData,
					   int &orDataSize, EnumDSSDataType &orDataType,
					   DSSDataFlag &orDataFlag, int *opThreshold = NULL,
					   int *opMetric = NULL, int *opTotalFlag = NULL, bool ibGetNullDataType = false);
    
	/*!
	 @abstract	find a row that matches given elements
	 @discussion return -1 if a valid row can't be found
	 @param iAxis	specify which header to look up, 0 for row header, and 1 for column header
	 param irKeys	a map from tabular unit ID to element key
	 */
	int getOffsetByElements(int iAxis, std::map<int, int> &irKeys);
	
	int getSubtotalIndex(int iRow, int iCol, int &orSubtotalIndex);
	
	int getLevel(int iRow, int iCol);
	
	int getMetricID(int iRow, int iCol);
	int get_Metric(int iRow, int iCol, int* opMetric);
	//int DSSSort();
	
	int getRowCount();
    int get_NumRows(int* opRow);

	int getColCount();
    int get_NumColumns(int* opCol);
    
	inline DSSDataModelXTabImpl * getXTabModel(){return mpXTabModel;};
	
	int size();
	
	int setCellValue(int RowIndex, int ColIndex, char *ipData, int iSizeInByte, DSSDataFlag iFlag, bool ibIsRawData = false);
    int setCellValueWithLookup(int RowIndex, int ColIndex, char *ipData, int iSizeInByte, DSSDataFlag iFlag, bool ibIsRawData = false);
	
	/*!
	 @abstract	for a given position [row, column], collect attribute form and metric values
	 */
	int GenerateRecord(int iRow, int iColumn, vector<int> &irvUnitID, vector<int> &irvFormID, vector<int> &irvWBFormID,vector<AERawData> &orvRecordData,vector<AERawData> &orvDisplayData);
    
    int GenerateRecordWithLookup(int iRow, int iColumn, vector<int> &irvUnitID, vector<int> &irvFormID, vector<int> &irvWBFormID, vector<AERawData> &orvRecordData, vector<AERawData> &orvDisplayData);
	/*!
	 @abstract	get the raw data of an element of tabular data unit
	 */
    int getUnitValue(DSSTabularUnit *ipUnit, int iKey, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
    /*!
     @abstract	get the threshold id of an element of tabular data unit
     */
    int getUnitThreshold(EnumDSSAxisName iAxis, int iOrdinal, const DSS_ID& irAttributeID, int& orThresholdID);
    /*!
     @abstract	get the attribute threshold id in the merged cells; if no threshold, return -1, else return the rowNum of the threshold
     */
    int getUnitMergedThreshold(EnumDSSAxisName iAxis, const vector<int>& iOrdinals, const DSS_ID& irAttributeID, int& orThresholdID);
    bool hasUnitThreshold(const DSS_ID& irAttributeID);
    
	/*!
	 @abstract	return number of rows of a metric
	 @discussion subtotal value is not included
	 */
	int getMetricCount(GUID &iMetricID);
	
	/*!
	 @abstract	release the metric data slices that can be loaded from disk again
	 @discussion
	 */
	int RecycleMetricSliceMemory();
    
    //imihai 06-10-2013
    int getOffsetForCoordinates(int iRow, int iCol, int &lOffset, DSSDataFlag &pFlags);
    
    //zhyang
    int sortForMicroChart(vector<int> &orRows, GUID &iMetricGUID, bool ibAsc = true, bool isTreeMode = false, int iSubtotalID = -100);
	
private:
    
	int hGetAttributeFormValue(DSSTabularUnit *ipUnit, int iOffset, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
    
	int hGetCustomGroupValue(DSSTabularUnit *ipUnit, int iOffset, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
    
	int hGetConsolidationValue(DSSTabularUnit *ipUnit, int iOffset, int iBaseForm, const void **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
	
	DSSDataModelXTabImpl *mpXTabModel;		//week reference
	
	// DSSXTabHeader *mpRowHeader;
	// DSSXTabHeader *mpColHeader;
    int mnLevels;
	int mnRowLevels;
	int mnColLevels;
	int mnMetrics;
	//int mnSubtotals;
	
	
	int *mpKeyBuffer;
	
	vector<int> mvpSliceMap;
    
    //tehe 11/24/2011 for new rwd
public:
    //inline DSSRWGridModel* &GetGridModel() { return mpGridModel; }
    inline DSSViewDataSet* GetViewDataSet(){return mpViewDataSet;};
    
    int Init (DSSXTabLayoutInfo *pLayout, DSSXTabView *pOldView);
    
    //int Init (DSSXTabDispInfo *pDispInfo, DSSCube *pCube);
    int Init (DSSXTabDispInfo *pDispInfo, DSSViewDataSet *ipViewDataSet);
    
    //## Operation: get_Cube%34F8D35901C7
    int GetCube(DSSCube **oppCube);
    int get_Cube(DSSCube **oppCube);
	// set Group-By units, metrics unit should be included.
	// For Metrics Units, we shall pass in GUID_NULL to indicate.
	int SetGBUnits(const vector<GUID>& ivGroupByUnits, DSSGBTable *ipGBTable);
    
	// Set current selection of GB units
	int ResetCurrentSelection(const vector<vector<int> >& ivvGBElements, int iTag, bool ibGBElementsChanged=true);
    
	// Return selected row/column for current selection.
	int GetCurrentWindow(vector<int> &orRows, vector<int> &orColumns);
    
    
	// Get available elements for a SINGLE target unit by giving selected elements of some of the units.
	// If you need to get available for multiple elements, we'd better get them in a single invoking for better performance.
	int GetAvailableElements(vector<GUID>& irSelectedUnits,
                             vector<vector<int> >& irSelectedElements,
                             GUID iTargetUnit,
                             vector<int>& orElements);
    
	int GetAvailableElements(vector<GUID>& irSelectedUnits,
                             vector<vector<int> >& irSelectedElements,
                             vector<GUID>& irTargetUnits,
                             vector<int>* opvElements);
	virtual int ConvertGUIDToLocalUnitID(const vector<GUID>& irvGUIDs, vector<int>& orvIDs);
    
    int Set_DoPreXTab(int iIsDoPreXTab);
    
    
    int GetRawCellInfo(int RowIndex, int ColIndex, char **ppData,
                       int *pcbWritten, EnumDSSDataType *pType,
                       DSSDataFlag * pCellFlag, int * pThresholdValue, int * pMetric);
    
    int GetRawCellInfo(int RowIndex, int ColIndex, char **ppData,
                       int *pcbWritten, EnumDSSDataType *pType,
                       DSSDataFlag * pCellFlag, int * pThresholdValue, int * pMetric,
                       int *opTotalFlag, bool ibGetNullDataType);
    
    int GetCellValue2(int RowIndex, int ColIndex, double &orVal, DSSDataFlag &orDataFlag,
                      int *opThreshold = NULL, int *opMetric = NULL);
	
    
    // 3/29/2012 tehe: initial ibGetNullDataType to be true
    int GetRawCellInfo2(int RowIndex, int ColIndex, const char **oppData,
                        int &orDataSize, EnumDSSDataType &orDataType,
                        DSSDataFlag &orDataFlag, int *opThreshold = NULL,
                        int *opMetric = NULL, int *opTotalFlag = NULL, bool ibGetNullDataType = true);
    
    
    int GetRawCellInfoFromMetricIndex(int RowIndex, int ColIndex, int iMetric, const char **oppData,
                                                   int &orDataSize, EnumDSSDataType &orDataType,
                                                   DSSDataFlag &orDataFlag, int *opThreshold = NULL,
                                                   int *opTotalFlag = NULL, bool ibGetNullDataType = true);

    
    int CheckSlice(vector<GUID>& irUnits, vector<bool>& irSelected, bool* isAvailable);
    
    int CheckSlice(vector<int>& irUnits, vector<bool>& irSelected, bool* isAvailable);
    
    int get_MetricValue2(int nKeys,
                         DSSXTabKey *pIndex,
                         int iMetric,
                         int iDummyIndex,
                         EnumDssSliceFlag iSliceFlag,
                         int iTag,
                         const char **oppData,
                         int *orSize,
                         EnumDSSDataType *opType,
                         DSSDataFlag *opFlags);
    
    int RefreshSortDefinition();
    
    //zhyang, get data for metric sort in new rwd
    int getRawCellInfo2(map<int, int> imElementIDs, int iNumber, bool ibRowKey, vector<int> &ivMask, int iMetric,
                        int iDummyIndex,
                        int iSubtotalID,
                        const char **oppData, int *orSize,
                        EnumDSSDataType *opType,
                        DSSDataFlag *opFlags,
                        int &actualUnits);
    
    //	This function will find the slice  that contains
    //	the data indexed by index.
    int FindDataLocFromIndex (DSSXTabKey *pIndex, int iMetric, int iTag, EnumDssSliceFlag iSliceFlag, DSSCubeDataSlice **ppSlice, DSSDataFlag &pFlags);
    
    inline void resetLastGBElements()
    {
        for (int i = 0; i < mnGroupByUnits && mpvLastGBElements; mpvLastGBElements[i]->clear(), i++)
            ;
    }
    
    // 6/28/2012 tehe: change to public
    bool isNonMetricsSelectionChanged(const vector<vector<int> >& ivvGBElements);
    
    int LoadAllSliceData();
    
    // 8/9/2012 tehe
    inline bool IsDoPreXtab() {return mDoPreXTab; }
    
    int mnTotalUnits; //zhyang, move it public for metric sort
    int mDummyUnitIndex;
    
    //09/18/12 fyang
    inline vector<DSSSortDefinition*>* getRowSorts(){return mvRowSorts;};
    inline vector<DSSSortDefinition*>* getColumnSorts(){return mvColumnSorts;};
    
    int getReportLevelSlice(int iMetric, DSSCubeDataSlice **oppSlice);
    
    inline void setHideNullZeroMaskInfo(DSSMaskInfos *ipMaskInfo) { mpHideNullZeroMaskInfo = ipMaskInfo; }
    
    inline DSSMaskInfos * getHideNullZeroMaskInfo() { return mpHideNullZeroMaskInfo; }
    int InsertToThresholdMap(pair<int,int> lLevelMetric,int lResultSliceID);
    
    int setCube(DSSCube* ipCube);
    
    bool isInitialized();
    
    int getMetricSliceOffset(int iRow, int iMetric, int* opOffset, bool ibSkipSubTotal = true);
    
    int ResetThresholdSlice(vector<int>& ivUnits, int  SliceID, int  Position, int  Tag, EnumDssSliceFlag SliceFlag);
    
    int getExtremes(EnumDSSRuntimeSubtotalType iExtremeType,
                    Int32 nRows,
                    Int32 *ipRows,
                    Int32 nColumns,
                    Int32 *ipColumns,
                    Int32 *onMetricIDs,
                    Int32 **oppMetricIDs,
                    EnumDSSDataType **oppDataTypes,
                    EnumDSSRuntimeSubtotalType **oppExtremeTypes,
                    Int32 *onGroupExtremeInfos,
                    void ** oppExtremeInfos);
    
    int getRefOrTrendInfo(EnumDSSRuntimeSubtotalType iLineType,
                          EnumDSSRuntimeSubtotalType iLevel,
                          Int32 nRows,
                          Int32 *ipRows,
                          Int32 nColumns,
                          Int32 *ipColumns,
                          Int32 nBreakbyUnits,
                          Int32 *ipBreakbyUnits,
                          Int32 *ipRowWindow,
                          Int32 *ipColumnWindow,
                          Int32 *onMetricIDs,
                          Int32 **oppMetricIDs,                                 // length of *onMetricIDs
                          Int32 **oppModelIndices,                       // length of *onMetricIDs
                          Int32 **oppTRMetricIDs,                        // length of *onMetricIDs, only for trendline over metrics case
                          EnumDSSDataType **oppDataTypes,          // length of *onMetricIDs
                          Int32 **oppnVariables,
                          Int32 ***opppVariables,
                          EnumDSSObjectType ***opppVariableTypes,
                          Int32 **opIsIncludeBreakBy,                    // length of *onMetricIDs, only for trendline over metrics case
                          Int32 **oppnBreakbyElements,                   // length of (nRows * nColumns) depends on iLevel
                          DSSXTabKey **oppBreanbyKeys,                    // length of (nRows * nColumns) * (*opnBreakElements) * nBreakbyUnits
                          Int32 ***oppnSize,                             // length of (nRows * nColumns) * (*opnBreakElements),
                          unsigned char ****oppInfo,                 // length of (nRows * nColumns) * (*opnBreakElements),
                          DSSDataFlag ***oppDataFlags,           // length of (nRows * nColumns) * (*opnBreakElements),
                          void ****oppForecastInfo                      // type fo ForecastInfo ***,length of (nRows * nColumns) * (*opnBreakElements),
    );
    
    inline int DeleteSortDefinition(){return hDeleteSortDefinition();};

    DSSAttributeThresholdIterator* getAttributeThresholdIterator();
    
    //tehe 11/24/2011 for new rwd
private:
    
    //get mask for out of view GBUnits
    int hGetMaskByOVGBUnits(const vector<vector<Int32> >& ivvGBElements, DSSXTabBitVector& orMask);
    
    int hLoadSliceByMetrics(vector<int> &ivMetrics);
    //	This interface function provides the access to the data.
    //	Given an coordinate, a value will be returned.
    int GetCell (int iRow, int iCol, EnumDssSliceFlag iSliceFlag, double & orVal, DSSDataFlag & pFlags, int* opThresholdID= NULL);
    
    //	This private function will find the index that is
    //	corresponding to the cell (iRow, iCol).
    int FindIndexFromCoordinate (int iRow, int iCol, DSSXTabKey *pIndex, int *pMetric, int *pTag, DSSTabularIndex **oppIndex = NULL, int *opOffset = NULL, DSSXTabKey *pChildIndex = NULL);
    
    int hGetRawCellInfo2(int RowIndex, int ColIndex, char **ppData,
                         int *pcbWritten, EnumDSSDataType *pType,
                         DSSDataFlag * pCellFlag, int * pThresholdValue, int * pMetric, int *opTotalFlag,
                         double* opData,
                         bool ibGetNullDataType,
                         bool ibMetricAsInput = false);//fyang: we use this flag to get the raw cell info for the metric cell corresponding to pMetric whose index is same with {rowIndex, colIndex}. But cell {rowIndex, ColIndex} is not necessarily under metric pMetric
    
    int hFindCellLevelIndex(Int32 iRow, Int32 iCol, DSSTabularIndex **oppIndex, Int32 *pMetric, Int32 *pTag,
                            Int32 *opOffset = 0, DSSCubeDataSlice **oppSlice = 0, Int32 *opOffsetSlice = 0, Int32 nExcludeUnits = 0, Int32* pExcludeUnits = NULL);
    
    int UpdateNumRowAndColumn ();
    
    int hGenerateSliceLevelInfo();
    
    // bool isNonMetricsSelectionChanged(const vector<vector<int> >& ivvGBElements);
    
    // int hSortHeaders();
    
	// set Group-By units, metrics unit should be included
	int SetGBUnits(const vector<int>& ivGroupByUnits);
    //
	// Get available elements for given target unit by giving selected elements of some of the units.
	// use the ipvSelectedElements to filter the datasets. It ignores the unit which is not in the dataset.
	// If the target unit is not in dataset, it will put -1 into the vector.
	int GetAvailableElements(vector<int>& irSelectedUnits,
                             vector<vector<int> >& irSelectedElements,
                             vector<int>& irTargetUnits,
                             vector<int>* opvElements);
    
    int hGetMetricValue(DSSXTabKey *pIndex,
                        int iMetric,
                        int iDummyIndex,
                        EnumDssSliceFlag iSliceFlag,
                        int iTag,
                        const char **ppData,
                        int *orSize,
                        EnumDSSDataType *pType,
                        DSSDataFlag *pFlags
                        );
    
    int hGenerateRecord(int iRow, int iColumn, vector<int> &irvUnitID, vector<int> &irvFormID, vector<int> &irvWBFormID,vector<DSSTabularUnit*> &irvTabularUnits, int lMetricUnitID, bool ibNeedLookup, vector<AERawData> &orvRecordData,vector<AERawData> &orvDisplayData);
    
    
    // 5/21/2012 tehe
    int hGetReportLevelIndex(DSSTabularIndex **opReportLevelIndex);
    
    // 7/5/2012 tehe
    int CalculateMask4Consolidation();
    
    int hGetMetricAndSlice(EnumDSSRuntimeSubtotalType iSubtotalType,
                           EnumDSSRuntimeSubtotalType iSubtotalLevel,
                           Int32 *opMetricCount,
                           vector<Int32>& orvMetricIDs,
                           vector<EnumDSSDataType>& orvDataTypes,
                           map<Int32, map<Int32, vector<Int32>, less<Int32> >, less<Int32> >& ormapMetricIDToSliceID/*metricID->(SubtotalType->sliceID)*/,
                           map<Int32, map<Int32, vector<DSSModelInfo*>, less<Int32> >, less<Int32> > &ormapMetricIDToModelInfo);
    
    int hgetValueFromSlice(Int32 iSliceID,
                           DSSXTabKey * pIndex,
                           DSSTabularIndex *ipIndexObject,
                           Int32 iOffsetInIndexObject,
                           EnumDSSRuntimeSubtotalType iExtremeType,
                           ExtremeInfo *iopExtremeInfo);
    
    int hgetValueFromSlice2(DSSCubeDataSlice* ipSlice,
                            DSSXTabKey * pIndex,
                            DSSTabularIndex *ipIndexObject,
                            Int32 iOffsetInIndexObject,
                            unsigned char **ppData,
                            Int32 *pcbWritten,
                            EnumDSSDataType *pDataType,
                            DSSDataFlag *pFlags,
                            Int32 *opOffset = NULL);
    
    int hGetRowAndColWithBreakBy(Int32 iRowStart, Int32 iRowEnd,
                                 Int32 iColStart, Int32 iColEnd,
                                 Int32 nBreakbyUnits, Int32 *ipBreakbyUnits,
                                 Int32* opnRows, Int32** oppRows,
                                 Int32 *opnCols, Int32** oppCols,
                                 Int32 *opBreakByAxis);
    
    int hInitAttributeThresholdItem();
    
    DSSXTabHeader *mHeaders[HEADCOUNT];
    
    //DSSRWGridModel *mpGridModel;		//week reference
    
    DSSCube *mpCube;
    
    // Data Members for Class Attributes
    
    EnumXTabStatus mfStatus;
    // int mnAttrUnit;
    DSSXTabKey *mCurIndex;
    
    // int mnMetric;
    
    bool mfInitInternal;
    
    int mCurTag;
    int mCurMetric;
    
    int mColCount;
    int mRowCount;
    
    DSSSliceMapper *mpSliceMap;
    // int mnLevel;
    
    
    int mHasCellFormat;
    
    //    bool mAllCellValid;
    int mnHeaderLevels[HEADCOUNT];
    //
    //
    struct LevelInfo
    {
        DSSCubeDataSlice *pCubeSlice;
        int *mapToRowHashKey;
        int *mapToColHashKey;
    };
    
    struct DSSSliceLevel
    {
        DSSTabularIndex *mpCellIndices;
        DSSTabularIndex *mpRowIndices;
        DSSTabularIndex *mpColIndices;
    };
    
    int mSizeSliceLevel;
    DSSSliceLevel* mpSliceLevel;
    
    // number of Group-By units
    int mnGroupByUnits;
    
    // start position of Group-By units array, metrics unit is not included
    int* mpGroupByUnits;
    
    int* mpGroupByUnitsPos; //Index of Valid GBUnit in original GB Units
    
    // indicate whether it has group-by
    bool mbHasValidGroupBy;
    
    // indicate whether it has MSIMetric Group-By
    bool mbHasMetricGroupBy;
    
    // indicate the pos of metric units in Group-By units array
    int mGBMetricPos;
    
    DSSViewDataSet *mpViewDataSet;
    
    // maps from GB position to level slice index. Of length 2^mnGroupByUnits;
    // Key: convert to position array to a int value, each bit of the int represents if the GB unit is included.
    // Value: index of SliceLevel
    vector<int, MBase::Allocator<int> >** mppvMapGBPos2LevelSlice;
    
    // start position of list of vector which contains selected elements for each GB unit.
    vector<int>** mpvLastGBElements;
    
    
    bool mDoPreXTab; //Indicate wether we should do Pre-XTabbing
    
    // used when no selector over non-metrics units
    bool mHasInitialized;
	
    // comparator for Index. It compares by the number of units of the index.
    struct IndexUnitComparator {
		bool operator() (DSSTabularIndex *index1, DSSTabularIndex *index2)
		{
			int lnUnits1 = 0, lnUnits2 = 0;
			lnUnits1 = index1->getNumUnits();
			lnUnits2 = index2->getNumUnits();
			return lnUnits1>lnUnits2;
		}
    }mIndexUnitComparator;
    
    //zhyang, cache sort definition
    vector<DSSSortDefinition*>* mvRowSorts;
	vector<DSSSortDefinition*>* mvColumnSorts;
    
    int hDeleteSortDefinition();
    
    // 5/17/2012 tehe
    DSSTabularIndex *mpReportLevelIndex;
    bool mbOnDemandLoading;
    
    vector<int> mvGBMetrics;      // 6/28/2012 tehe
    
    bool mbConsoBVCalculated;
    vector<DSSXTabBitVector *> mpvConsolidationBVs;
    
    // 9/10/2012 tehe
    DSSGBTable *mpGBTable;
    int *mpOVGBUnits;
    int *mpOVGBUnitPos;
    int mnOVGBUnits;
    vector<int>** mpvLastOVGBElements;
    
    // tehe: support null zero hiden feature
    DSSMaskInfos *mpHideNullZeroMaskInfo;
    
    //zhyang, extreme
    // DSSCube *mTrendModelCubePtr;
    map<Int32, bool, less<Int32> > mmapIncludeBreakBy;// map from trend model slice to a boolean indicate whether it include breakby or not
    map<Int32, map<double, Int32, less<double>, MBase::Allocator<pair<double const, Int32> > >*, less<Int32> > mmapElementKeys; // <unitID, <value from Index function, element Key>
    map<Int32, map<double, string, less<double>, MBase::Allocator<pair<double const, string> > >*, less<Int32> > mmapLabels; // <unitID, <value from Index function, labels> >, don't forget to delete BSTR*
    map<Int32, int, less<Int32> > mmapNewExtremes; // original slice id to CDSSSliceID
    map<Int32, ForecastInfo *, less<Int32> > mmapForecastInfos; // original trend model slice to ForecastInfo array
    
    friend class DSSXTabViewHelper;
private:
    // jzeng, for attribute threshold
    DSSAttributeThresholdItems* mpAttributeThresholdItems;
    DSSAttributeThresholdIterator* mpAttributeThresholdIterator;
    
    struct AttributeThresholdKey
    {
        Int32 mLevel;
        Int32 mTag;
        DSS_ID mAttrID;
        AttributeThresholdKey(Int32 iLevel, Int32 iTag, const DSS_ID& iAttrID)
        {
            mLevel = iLevel;
            mTag = iTag;
            mAttrID = iAttrID;
        }
    };
    
    struct AttributeThresholdKeyCmp
    {
        bool operator()(const AttributeThresholdKey& left, const AttributeThresholdKey& right) const
        {
            if (left.mLevel < right.mLevel)
                return true;
            else if (left.mLevel > right.mLevel)
                return false;
            else // ==
            {
                if (left.mTag < right.mTag)
                    return true;
                else if (left.mTag > right.mTag)
                    return false;
                else
                {
                    if (MBase::CompareGUID(left.mAttrID, right.mAttrID) < 0)
                        return true;
                    else
                        return false;
                }
            }
        }
    };
    
    typedef std::map<AttributeThresholdKey, int, AttributeThresholdKeyCmp> AttributeThresholdSliceMap;
    AttributeThresholdSliceMap mAttributeThresholdSliceInfo;
    
    friend class DSSAttributeThresholdIterator; //US 13300
};
