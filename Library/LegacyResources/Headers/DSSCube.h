/*
 *  DSSCube.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import "DSSAEHelper.h"


class DSSTabularIndex;
class DSSTabularIndexPool;
class DSSDataColumn;
class DSSCubeDataSlice;
class DSSXTabDispInfo;
class DSSXTabView;
class DSSTabularData;
class DSSFunctionPackage;
class DSSTabularConsolidation;
class DSSXTabBitVector;
class DSSSortDefinition;

class DSSCube
{
public:
	DSSCube(DSSTabularData *ipTabularData = NULL);
	~DSSCube();
//    int Init();
	int Init(DSSTabularData *ipTabularData, bool ibIsTempCube = false);
	int reinit(DSSTabularData *ipTabularData);
	int CreateDataSlice(DSSTabularIndex *ipIndex, DSSDataColumn *ipColumn, DSSCubeDataSlice **oppDataSlice);
	int InsertDataSlice(DSSCubeDataSlice * ipDataSlice, int &orSliceID);	
	int InsertSlice(DSSCubeDataSlice * ipDataSlice, int *opSliceID);
	int CreateDataSlice(int nUnits, int *ipUnits, EnumDSSDataType iType, DSSCubeDataSlice **oppDataSlice);
    //create a slice at [iUnit] level.
    //if oppNewIndex is not NUll, then there is new index generated but not managed by index pool. Caller needs to release memory.
    int CreateSingleUnitSlice(int iUnit, EnumDSSDataType iType, DSSCubeDataSlice **oppDataSlice, DSSTabularIndex **oppNewIndex);
	
	int getIndexPool(DSSTabularIndexPool **oppIndexPool);
    int getIndex(int nUnits, int *pUnits, DSSTabularIndex **oppIndex);
    void GetIndexRowMap( DSSTabularIndex* ipBase, DSSTabularIndex* ipTarget, int** oppRowMap);
	
	int size();

	int CreateConstantSlice(char * ipData, int iSizeInByte,
						 EnumDSSDataType iType, DSSDataFlag iFlag, int *opSliceID);

	DSSCubeDataSlice *getSliceByID(int iSliceID);
    int get_SliceByID(int iSliceID,DSSCubeDataSlice** oppSlice);
	//create a temporary slice, caller doesn't hold the reference and don't need release the pointer
	int CreateCalculationSlice(DSSTabularIndex *ipIndex, EnumDSSDataType iType, DSSCubeDataSlice **oppSlice, int *opSliceID);
    int CreateCalculationSlice(DSSTabularIndex *ipIndex, DSSDataColumn *ipColumn, int *opSliceID);

	//update Calculation Result by moving slices from temporary list to mvSliceList
	int UpdateCalculateResult(vector<int> &irvOriginalSliceID, vector<int> &irvNewSlicedID);

	//make a link copy of the current data slices. In transaction, we need restore these data slices
	int MakeOriginalDataSliceCopy();
	
	//For transaction, restore the original data slices
	int RestoreOriginalDataSlice();
    
    // tehe
    //int get_XTabView(DSSXTabDispInfo *ipDispInfo, DSSXTabView **oppXtabView);
    
    //junwang, 2012/05/31, 
    int createMMapMemory(const char * ipFilePath, bool ibUseNewFile);
    
    // 8/23/2012 tehe
	int releaseMMapMemory();
    void hPartialCleanUp();
    
    int getConstantSlice(int nUnit, int *pUnit, char *ipData, int cbData, EnumDSSDataType iType, DSSDataFlag iFlag, int *opSliceID);
    int duplicateSlice(DSSCubeDataSlice *ipSourceSlice, DSSCubeDataSlice **oppOutSlice, bool ifShareColumn);
    int getAttrFormSlice (Int32 iUnit, Int32 iForm, Int32 *opSliceID, bool iUseLookupForAttributes, DSSGUnitContainer* ipContainer = NULL);
    int get_AttrFormSlice(Int32 iUnit, Int32 iForm, Int32 *opSliceID, bool iLevelSignificant = true, DSSGUnitContainer* ipContainer = NULL);
    inline DSSTabularData* getTabularData() { return mpTabularData;}
    inline void setTabularData(DSSTabularData* ipTabularData){mpTabularData = ipTabularData;}
    inline int get_TabularData(DSSTabularData **oppTabularData)
    {
        if (oppTabularData)
            *oppTabularData = mpTabularData;
        return S_OK;
    }
    inline int get_TabularDataC(DSSTabularData **oppTabularData)
    {
        if (oppTabularData)
            *oppTabularData = mpTabularData;
        return S_OK;
    }
    int CountSlices();
    int Consolidate(DSSFunctionPackage *pFormula, Int32 ConsolidationUnit, Int32 SliceInID,
                    EnumNullCheckingOption iNullCheckingOption, Int32 *pSliceOutID);
    
    int IsNumericalSlice(DSSCubeDataSlice *pSliceIn/*in*/, BOOL &bNumericalSlice /*out*/, BOOL &bCellFormatSlice /*out*/);

    int newCalculateConslidation(DSSCubeDataSlice *pSliceIn,
                                DSSTabularConsolidation *pConsolidation,
                                DSSFunctionPackage *pFormula,
                                Int32 ConsolidationUnit,
                                EnumNullCheckingOption iNullCheckingOption,
                                Int32 *pChildUnits,
                                Int32 nChildUnits,
                                DSSCubeDataSlice *pSliceOut,
                                 bool ibIsCS2CS = false);	//lwang, 7/02/2007, ibIsCS2CS is true
                                                            //if both input and output slice are consolidation
    int CalculateConsolidationElement(DSSCubeDataSlice *pSliceIn,
                                      DSSCubeDataSlice *pSliceOut,
                                      DSSTabularConsolidation *pConsolidation,
                                      DSSFunctionPackage *pFormula,
                                      Int32 iConsolidationUnit, EnumNullCheckingOption iNullCheckingOption,
                                      Int32 *pIndex, Int32 nTotalUnit,
                                      Int32 *pChildUnits, Int32 nChildUnit,
                                      DSSXTabBitVector & irBVParentEle, DSSXTabBitVector & irBVChildEle,
                                      double &result, DSSDataFlag &oFlag,
                                      bool ibIsCS2CS);
    int hGetCSLDCalculationElements(DSSCubeDataSlice *ipSliceIn,
                                    DSSCubeDataSlice *ipSliceOut,
                                    DSSTabularConsolidation *ipConsolidation,
                                    DSSFunctionPackage *ipFormula,
                                    Int32 iConsolidationUnit, EnumNullCheckingOption iNullCheckingOption,
                                    Int32 *ipIndex, Int32 nTotalUnit,
                                    Int32 *ipChildUnits, Int32 nChildUnit,
                                    DSSXTabBitVector & irBVParentEle, DSSXTabBitVector & irBVChildEle,
                                    Int32 iTotalParams, double *opParams,
                                    Int32 & orCountParams, DSSDataFlag &orFlag,
                                    bool ibIsDistinct,	bool ibIsCS2CS);
    int SortIndex (Int32 **pIndex, DSSTabularIndex *pCalcIndex, vector<DSSSortDefinition*> *pSortDefns, DSSCubeDataSlice *pSlice);
    
    // lweng, 2011-12-9, add for SFB
    struct PairCompare
    {
        bool operator() (const pair<DSS_ID, Int32>& iPair1, const pair<DSS_ID, Int32>& iPair2) const
        {
            Int32 lCompareResult = DSSAEHelper::InlineCompareGUID(iPair1.first, iPair2.first);
            if (lCompareResult < 0)
                return true;
            else if (lCompareResult > 0)
                return false;
            else
                return iPair1.second < iPair2.second;
        }
    };
    typedef std::map<pair<DSS_ID, Int32>, Int32, PairCompare>  SFBMetricMap;
    
    // lweng, 2012-10-23, add two methods for subtotal from base
	int put_GeneratedMetricID(DSS_ID iOriginalMetricID, Int32 iLevel, Int32 iGeneratedSliceID);
	int get_GeneratedMetricID(DSS_ID iOriginalMetricID, Int32 iLevel, Int32 *opGeneratedSliceID);
    
    int removeUncalculatedSlices(int iMaxCalculatedSliceID);
    int removeUselessSlices(int iMaxUsefulSliceID);
    
    //used in temporary cube for trx conditional evaluation
    int markSliceFromOtherCube(DSSCubeDataSlice* ipSlice);
    void markTemporaryCube(bool ibIsTempCube = true){mbTemporaryCube = ibIsTempCube;}
private:
    int IsNumerical(EnumDSSDataType inputType/*in*/, BOOL &bNumericalSlice /*out*/, BOOL &bCellFormatSlice /*out*/);
    int hGetCSLDCalculationElementsCount(DSSTabularConsolidation *ipConsolidation, Int32 iCSEleIndex, Int32 &orCountParams);

private:
	DSSTabularIndexPool *mpIndexPool;
	vector<DSSCubeDataSlice *> mvSliceList;
	vector<DSSCubeDataSlice *> mvTempSlice4Calculation;		//temporary slices list
	vector<DSSCubeDataSlice *> mvOriginalDataSlice;
    
    set<DSSCubeDataSlice*> mvSliceFromOtherCube;
    bool mbTemporaryCube;
    DSSTabularData *mpTabularData;
    // lweng, 2012-10-23, add for SFB, (original metric id, level, generated metric id)
	SFBMetricMap mSFBMetricMap;
    
};
