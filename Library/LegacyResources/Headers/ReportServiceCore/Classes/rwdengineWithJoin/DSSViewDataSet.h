//
//  DSSViewDataSet.h
//  ReportServiceCore
//
//  Created by Yang Zhuo on 9/20/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//
#ifndef DSSVIEWDATASET
#define DSSVIEWDATASET

#include <vector>
#include <string>
#import "DSSCube.h"
#import "DSSCalculationPlan.h"
#import "DSSBinaryDataReader.h"
#import "DSSTabularIndex.h"
#import "DSSTabularIndexPool.h"
#include "DSSTabularData.h"

class DSSXTabDispInfo;
class DSSXTabView;
class DSSTabularMetrics;
class DSSGUnitContainer;
class DFCCSI;
class DFCCSITransactionBase;
class DFCCSIUpdate;
class DFCCSIDelete;
class DFCCSIInsert;
class DSSMaskInfos;
class DSSFunctionServer;
class DFCSimpleExpr;

class DSSCMNodeModel;
class CMIndexPoolInfo;
class DSSSliceID;
class ForecastInfo;

//5/17/2012 tehe
typedef struct SliceInfos {
    SliceInfos(){}
    ~SliceInfos() {}
    int mStartPosition;
    int mEndPosition;
    int mnSlice;
    vector<int> mvSliceOffset;
}SliceInfos;

// Dec/30/2013, below struct store info of value that user inserts
typedef struct ValInfo
{
    bool mfIsAttrVal; // true: this value is of attribute.
    bool mfIsID; // true: this value is of ID form.
    bool mfIsExist; // true: this value exists in attribute form to be inserted.
    string mVal;    // string representation of the value to insert.
    int mFormOffset; // offset of the corresponding form in DSSTabularAttribute::mvBaseFormID
    int mOffsetInCol; // if mfIsExist is true, "mOffsetInCol" is the value's offest in form's column.
    int mUnitIndex; // corresponding unit's local index in DSSTabularData
} ValInfo_t;

// Jan/6/2014, below struct store the information of value to insert into an attribute
typedef struct AttriInfo
{
    vector<int> mvIDVal; // store the offset of ID value to insert
    vector<int> mvNonIDVal; // store the offset of non-ID value to insert
    bool mIsExtended; // true: the attribute has been extended; false: the attribute has not been extended;
    
    typedef enum EnumInsertedID
    {
        EnumInsertedID_NEW,
        EnumInsertedID_EXISTING,
        EnumInsertedID_NONE
    } EnumInsertedID;
    EnumInsertedID mInsertedID;
} AttrInfo_t;

typedef enum EnumUpdateType
{
    UPDATE_UNKNOWN,
    UPDATE_ATTR_LOOKUP,
    UPDATE_RELATION,
    UPDATE_METRIC
} EnumUpdateType;

struct SerializedExtremeInfo
{
    EnumDSSRuntimeSubtotalType mExtremeType;
    Int32 mModelIndex;
    Int32 mTRMetricID;
    Int32 mnXVariable;
    vector<Int32> mvXVariables;
    vector<EnumDSSObjectType> mvXVariableTypes;
    vector<Int32> mvXIndexMetrics;
    
    SerializedExtremeInfo(EnumDSSRuntimeSubtotalType ExtremeType = DssRuntimeSubtotalDefault,
                          Int32 iModelIndex = -1,
                          Int32 iTRMetricID = -1,
                          Int32 nXVariable = 0,
                          Int32 *ipXVariables = NULL,
                          EnumDSSObjectType *ipXVariableTypes = NULL,
                          Int32 *ipXIndexMetrics = NULL)
    :mExtremeType(ExtremeType),
    mModelIndex(iModelIndex), mTRMetricID(iTRMetricID), mnXVariable(nXVariable)
    {
        if (mnXVariable > 0)
        {
            mvXVariables.resize(mnXVariable);
            mvXVariableTypes.resize(mnXVariable);
            mvXIndexMetrics.resize(mnXVariable);
            memmove(&mvXVariables[0], ipXVariables, mnXVariable * sizeof(Int32));
            memmove(&mvXIndexMetrics[0], ipXIndexMetrics, mnXVariable * sizeof(Int32));
            memmove(&mvXVariableTypes[0], ipXVariableTypes, mnXVariable * sizeof(EnumDSSObjectType));
        }
    };
};


class DSSViewDataSet
{
public:
    DSSViewDataSet(bool isOnDemandLoading = false);
    ~DSSViewDataSet();
    
    int loadFromBinary(DSSBinaryDataReader * ipReadStream);
    int loadExtendFromBinary(DSSBinaryDataReader * ipReadStream);
    int Init(DSSGUnitContainer *ipGlobalLookupContainer);
    
    // 3/16/2012 tehe
    int RefreshXTabView(DSSXTabDispInfo *ipDispInfo, DSSXTabView **oppXTabView);
    
    //xiazhou, offline UC
    inline void setXTabView(DSSXTabView *lpXTabView){mpXTabView = lpXTabView;}
    
    DSSCube* &GetCube();
    void setCube(DSSCube *ipCube);
    
    // 3/16/2012 tehe
    //zhyang, we should avoid call Count on view dataset, it should be called on tabular data
    inline int Count() {
        //assert(false);};
        return mpTabularData->Count();
    };
    
    //07/08/2013 imihai
    inline int getMetricsCount() { return mpTabularData->getMetricsCount(); }
    
    DSSCalculationPlan* getCalculationPlan();
    DSSXTabView* getXTabView(){return mpXTabView;};
    
    //zhyang
    int FindUnit(const GUID &irGUID, EnumDSSTemplateUnitType iType);
    
    DSSTabularUnit * Item(int iUnitIndex);
    
    //  5/21/2012 tehe
    inline SliceInfos * getSliceInfos() { return mpSliceInfos; }
    int LoadSliceDataFromDataSource(int nSize, DSSXTabBitVector** pBitVectors, int * pSliceID);
    inline void setSliceDataSource(DSSDataSource *ipSliceDataSource) { mpSliceDataSource = ipSliceDataSource; } // data source for slice only
    inline DSSDataSource * getSliceDataSource() {return mpSliceDataSource ; }
    inline bool IsOnDemandLoading() {return mbOnDemandLoading; };
    
    bool hasCellFormat(){return mbHasCellFormat;};
    
    void hCleanUp();
    
    // 5/30/2012 tehe
    int RecycleMetricSliceMemoryForNewRWD(bool ibHasEnoughMemory,int nSize = 0, int *ipDisableSlice = NULL);
    
    //junwang, 2012/05/31,
    int createMMapMemory(const char * ipFilePath, bool ibUseNewFile);
    
    //const vector<DSSTabularUnit *> * getUnits(){return &mvUnits;};
    
    // 8/23/2012 tehe
    void hPartialCleanUp();
    void hInit(bool isBaseDataset = false, GUID iDatasetGUID = GUID_NULL);
    
    // 9/25/2012 tehe
    int releaseMMapMemory();
    
   
    void setBuffer(MBase::Buffer *ipBuffer) {mpBuffer = ipBuffer;}    
    
    
    
    int setTemplate(DSSTemplate *ipTemplate);
    int getTemplate(DSSTemplate **oppTemplate);
    
    int setNullOnTop(bool iNullOnTop);
    int getNullOnTop(bool *iNullOnTop);
    int setNullReplacement(MBase::CString &iNullString);
    int getNullReplacement(MBase::CString &iNullString);
    
    DSSGUnitContainer* getGUnitContainer(){return mpTabularData->getGUnitContainer();};
    int getTabularData(DSSTabularData **oppTabularData);
    int setTabularData(DSSTabularData *ipTabularData);
    inline DSSTabularData* getTabularData(){return mpTabularData;};
    
    int setObjectContext(DSSObjectContext *ipObjectContext);
    DSSObjectContext *getObjectContext();
    
    int FindElement(DSSBaseElementProxy *ipElement, int &orUnit, int &orKey){assert(false); return S_FALSE;};
    
    //zhyang, interface in IStatisticsInfo
    int size(){return 0;};
    
    int hLoadIndexPool(DSSBinaryDataReader * ipReadStream, bool ibLoadFromExtendCube = false);
    
    // 9/25/2012 tehe
    inline void setUnitCount(int iCount) { mpTabularData->setUnitCount(iCount); };
    
    int updateViewDataSet(DFCCSIUpdate* ipUpdate);
    
    // jimzhang, temp code, begin
    // Param:
    //  @1 [in], represend the condition expression in where-clause of the dataset SQL.
    //  @2 [out],
    //      true: there is metric condition in where-clause of the dataset SQL.
    //      false: there is no metric condition in where-clause of the dataset SQL.
    //  @3 [out]: store the ID of attribute which is in the where-clause of the dataset SQL and if it has ID form.
    
    int hGetUnitFromExpr(DFCSimpleExpr *ipExpr, bool &orHasMetric, map<DSS_ID, bool> &orAttrHasIDForm);
    
    // jimzhang, temp code, end
    
    int deleteFromViewDataSet(DFCCSIDelete* ipDelete);
    
    int insertIntoViewDataSet(DFCCSIInsert* ipInsert);
    
    int executeTransactionCSI(vector<DFCCSITransactionBase*> ivpTransaction);
    
    void setFunctionServer(DSSFunctionServer* ipServer){mpFunctionServer = ipServer;}
    
    DSSFunctionServer* getFunctionServer(){return mpFunctionServer;}
    
    inline void setNodeModelCacheManager(DSSCMNodeModel *ipNodeModelCacheManager) {  mpNodeModelCacheManager = ipNodeModelCacheManager; }
    int GetReportLevelIndex(DSSTabularIndex **opReportLevelIndex);
    void ClearCMNodeModel() { mpNodeModelCacheManager = NULL;}
    
    int clearXTabView();
    
    int setSchema(MCESchema *ipSchema);
    MCESchema *getSchema();
    void setHasCellFormat() {mbHasCellFormat = true;}
    inline bool IsSubtotalDimensionalityAware() {return mbSubtotalDimensionalityAware; };
    
    EnumDSSTransactionErrorCode getTxnErrorCode() {return mTxnErrorCode; }
    vector<string> getTxnErrorInfo();

private:
    // tehe: support local cache manager
    int hLoadIndices(DSSBinaryDataReader *ipReadStream);
    int hApplyLocalCacheManager(DSSBinaryDataReader *ipReadStream);
    int hGetIndexPoolInfo(DSSBinaryDataReader *ipReadStream, CMIndexPoolInfo **oppIndexPoolInfo);
    int hLoadNewIndices(DSSBinaryDataReader * ipReadStream);
    
    // 9/26/2012 tehe
    int hBuildSliceInfos(int nSlice);
    
    int hLoadGUID(DSSBinaryDataReader * ipReadStream);
    int hLoadCalculationPlan(DSSBinaryDataReader * ipReadStream);
    int hLoadExtremeSlice(DSSBinaryDataReader * ipReadStream);
    int hLoadCube(DSSBinaryDataReader * ipReadStream, bool ibOnDemandLoading = false);
    
    // 5/17/2012 tehe: now we only on demand loading for grid model
    int hLoadCubeSlices(DSSBinaryDataReader * ipReadStream, bool ibOnDemandLoading = false);
    int hSetUnqualifiedRows(DSSTabularIndex *ipIndices,
                            DSSXTabBitVector * ibvUnqualifiedRows,
                            Int32 *ipFilteredRows,
                            Int32 nFilteredRows);
    int hPopulateInfoForInsertUpdate(DFCCSITransactionBase* ipCSI,
                               int *ipBaseUnits,
                               int nBaseUnits,
                               vector<ValInfo_t> &ovValInfo,
                               map<int, AttrInfo_t> &omAttr2Info);
    int hValidateForm(vector<ValInfo_t> &ovValInfo,
                      map<int, AttrInfo_t> &omAttr2Info,
                      bool* opValid, string& oAttrName, string& osInvalidIDVal, string& osInvalidFormVal);
    int hValidateGUID(DFCCSITransactionBase* ipCSI, bool* opValid, bool* opHasLevelMetric, int* lSetInValidPos);
    
    int hValidateUnitsInExpr(DFCSimpleExpr* ipExpr, bool* opValid);
    
    int hCheckLevel(vector<bool> &iovIsLevelMetric, bool ibGrandTotalLevel = false /*treat grand total level metric as level metric*/);
    int hIsLevelMetric(GUID* ipMetricGUID, bool* opIsLevelMetric, bool* opAtGrandTotalLevel);
    bool hIsLevelMetric(DSSMetric* ipMetric);
    int hGetLevelsOfLevelMetrics(DSSMetric* ipMetric, set<int>& iLevels);
    int hGetUpdateTypes(DFCCSIUpdate* ipUpdate, int *ipBaseUnits, int nBaseUnits, map<int, AttrInfo_t> imAttr2Info, vector<EnumUpdateType> &ovUpdateTypes);
    int hGetDuplicatedRow(int* ipKeyTable, int iKeyTableLength, vector<int> ivNewUnits, int* oDuplicatedRow);
    int hGetLevelsOfWHEREClause(DFCSimpleExpr* ipExpr, set<int>* osUnionLevelsOfLevelMetrics, bool* opIsReportLevel);
    bool hHaveSameFactForMetrics(GUID lMetricGUID1, GUID lMetricGUID2);
    int hParseMetricExpression(DSSExpNode *ipMetricRoot, GUID* opTargetFactGUID);
    
    int hConvertDataCheck(EnumDSSDataType iSrcType, string iSrcString, EnumDSSDataType iTargetType, bool* obSucceed);
    
    //for error info generation
    int hGetExprUnitName(DFCSimpleExpr* ipUnit, string& oAttrName, string& oAttrFormName, string& oMetricName);
    
    //3/23/2016, msun, for financial report.
    int loadRecursiveUnits(DSSBinaryDataReader * ipReadStream);
    
    DSSTabularData *mpTabularData;
    DSSCube *mpCube;
    DSSCalculationPlan *mpCalcPlan;
    DSSObjectContext *mpObjectContext;
    
    // tehe
    DSSXTabView *mpXTabView;
    
    MBase::Buffer *mpBuffer;
    bool mNullOnTop;
    MBase::CString mNullString;
    
    // 5/17/2012 tehe
    SliceInfos *mpSliceInfos;
    DSSDataSource *mpSliceDataSource;
    bool mbOnDemandLoading;
    
    // 5/30/2012 tehe: used to keep which slice is loaded
    set<int> msLoadedSlice;
    bool mbHasCellFormat;
    DSSTemplate *mpTemplate;
    
    DSSFunctionServer *mpFunctionServer;
    
    bool mbIsBaseDataset;
    DSSCMNodeModel *mpNodeModelCacheManager;
    
    MCESchema *mpSchema;
    
    bool mbSubtotalDimensionalityAware;
    
    //fali, 6/20/2014, if there's error when execute transaction CSI, prepare the error code and info for caller.
    //Error info stores the related Attribute/Metric Name, value etc. The length and order of it depend on error type(ErrorCode)
    EnumDSSTransactionErrorCode mTxnErrorCode;
    vector<std::string > mvTxnErrorInfo;
public:
    DSSCube *mTrendModelCubePtr;
    map<Int32, bool, less<Int32> > mmapIncludeBreakBy;// map from trend model slice to a boolean indicate whether it include breakby or not
    map<Int32, map<double, Int32, less<double>, MBase::Allocator<pair<double const, Int32> > >*, less<Int32> > mmapElementKeys; // <unitID, <value from Index function, element Key>
    map<Int32, map<double, string, less<double>, MBase::Allocator<pair<double const, string> > >*, less<Int32> > mmapLabels; // <unitID, <value from Index function, labels> >, don't forget to delete BSTR*
    map<Int32, Int32, less<Int32> > mmapNewExtremes; // original slice id to CDSSSliceID
    map<Int32, ForecastInfo *, less<Int32> > mmapForecastInfos; // original trend model slice to ForecastInfo array

};

#endif