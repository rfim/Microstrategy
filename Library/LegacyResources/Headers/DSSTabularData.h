//
//  DSSTabularData.h
//  ReportServiceCore
//
//  Created by He Tengfei on 11/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef DSSTABULARDATA
#define DSSTABULARDATA

#include "GUID.h"
#include "DSSIPhoneEnums.h"
#include "DSSDataModelBase.h"

#import "DSSRWGridModel.h"

class DSSTabularMetrics;
class DSSTabularUnit;
class DSSCube;
class DSSGUnitContainer;
class DSSViewDataSet;
class DSSTemplate;
class DSSObjectContext;
class DSSXTabView;
class DSSSliceID;
class DSSCubeDataSlice;
class DSSTabularIndex;
class DSSXTabBitVector;
class DSSTabularIndexPool;
class MCESchema;
class DSSRelation;
class DSSBinaryDataReader;
class DFCSimpleExpr;

class DSSTabularData 
{
public:
    DSSTabularData();
    DSSTabularData(GUID iDataSetGUID);
    ~DSSTabularData();
    
    int init(DSSGUnitContainer *ipGlobalLookupContainer);
    int Init(DSSDataModelBase *ipModelBase, const vector<DSSTabularUnit *> & irvUnits);
    
    int getMetrics(DSSTabularMetrics **oppTabularMetrics);
    int get_MetricsC(DSSTabularMetrics **oppTabularMetrics);
    
    int FindUnitI(const GUID &irGUID, EnumDSSTemplateUnitType iType, int &iPosition, bool iCheckRecursive=false);
    int FindUnit(const GUID &irGUID, EnumDSSTemplateUnitType iType, bool iCheckRecursive = false);
    int FindUnitI(const GUID &irGUID, EnumDSSTemplateUnitType iType, int *opIndex);
    int FindLinkedUnit(const GUID &irGUID, EnumDSSTemplateUnitType iType);//xiazhou, 08/12/2014. Inculde linked units in the search.
    
    int Item(int iUnitIndex, DSSTabularUnit **oppTabularData);
    DSSTabularUnit* Item(int iUnitIndex);
    int ItemC(int iUnitIndex, DSSTabularUnit **oppTabularData);
    
    int Count(int *pCount);
    int Count();
    int getDummyUnitIndex(int *pUnitIndex);
    int FindElement(DSSBaseElementProxy *ipElement, int &orUnit, int &orKey);
    
    inline void setObjectContext(DSSObjectContext *ipObjectContext){mpObjectContext = ipObjectContext;};
    inline DSSObjectContext *getObjectContext(){return mpObjectContext;};
    inline void setXTabView(DSSXTabView* ipXTabView){mpXTabView = ipXTabView;};
    inline DSSXTabView* getXTabView(){return mpXTabView;};
    
    int loadFromBinary(DSSBinaryDataReader * ipReadStream);
    int getMetricsCount();
    inline DSSTabularMetrics* getTabularMetircs(){return mpMetrics;};
    DSSGUnitContainer* getGUnitContainer();
    inline int getMetircPos(){return mMetircPos;};
    const vector<DSSTabularUnit *> * getUnits(){return &mvUnits;};
    const vector<DSSTabularUnit *> * getRecursiveUnits() { return &mvRecursiveUnits;}
    inline void setUnitCount(int iCount) { mnUnits = iCount; }
    inline int getUnitCount() { return mnUnits;};
    const GUID& getMetricID(int iMetricIndex);
    int getUnitGUIDAndType(int pos, GUID &irGUID, EnumDSSTemplateUnitType &irType);
    
    //zhyang, 677674. 
    //be careful we should check whether it is attribute GUID before we call it
    bool canUseSSO(GUID &iGUID);
    int enableSSOForAllUnits();
    int inheritSSOStatusForAllAttr(DSSViewDataSet *ipViewDataSet);
    
    //3/16/2012 tehe
    void setIsRunTimeUnit(int iUnitID,bool iIsRuntimeUnit);
    
    inline bool getIsNewRWD(){return mbIsNewRWD;};
    int GetReportLevelUnits(int nUnit, int *iopUnits, int *opnUnit);
    int filterPopulate(DSSCube            *ipCube,
                       int                iNumUnits, 
                       int                *ipUnits, 
                       vector<DSSSliceID> &ivSliceIDs, 
                       METRIC_ID          *ipMetricIDs, 
                       DSSCubeDataSlice   *ipFilterSlice, 
                       DSSXTabBitVector   *ipMask, 
                       int                nSelectedUnits,
                       UnitFormInfo       *pSelectedUnitInfo,
                       int                iFilterDEFlag = 0,
                       bool               ibRWDSubsetFromBase = false, 
                       DSSGUnitContainer  *ipGlobalLookupContainer = NULL, 
                       int                iNumDerivedElements = 0, 
                       int                *ipDerivedElementUnits = NULL);
    int filterIndex(DSSTabularIndex* ipIndex, DSSCubeDataSlice *ipFilterSlice, DSSXTabBitVector *ipMask, DSSTabularData* ipOriginalTabularData, int &onFilteredRows, int* &opFilteredRows, int* &opRowForOrigInd, DSSXTabBitVector * obvUnqualifiedRows, int iFilterDEFlag, MBase::Buffer *pBuffer);
    int hGetAllSelectedIndices(DSSTabularData  *lpOriginalTabularData,
                               DSSTabularIndex *ipIndices,
                               vector<Int32>   &ivDEUnits,
                               Int32           *ipFilteredRows,
                               Int32           nFilteredRows,
                               Int32           ** oppSelectedRows,
                               Int32           * opSelectedRowsCount,
                               DSSXTabBitVector * obvUnqualifiedRows,
                               Int32           iFilterDEFlag,
                               MBase::Buffer   *ipBuffer);
    int hGetDependentElementKeys(DSSTabularData *lpOriginalTabularData,
                                 vector<Int32> &irvDEUnits,
                                 vector<Int32> &irvUnitKey,
                                 vector< vector<Int32> > &rvNewKeys);
    int writeIndex(int inUnit, int* ipRowForIND, int inRow, DSSTabularIndex* ipIndex);
    typedef map<int, int, less<int> > IntToIntMap;
    int writeSlice(DSSCubeDataSlice *ipFrom, int iUnit, int* ipUnit, vector<Int32, MBase::Allocator<Int32> > **ppvRowForLUT, int iFilteredRows, int* ipFilteredRows, DSSCubeDataSlice** oppTo, MBase::Buffer *pBuffer,int* ipRowMap, Int32** ppGToLMap, IntToIntMap** ppGToLMap2);
    int createReferenceUnit(EnumDSSTemplateUnitType iType, 
                            int iIndex, 
                            DSSTabularUnit* ipUnit, 
                            DSSTabularData *ipOriginalTabularData, 
                            DSSTabularUnit** oppUnit, 
                            bool ifDEonDEElementBrowse = false,
                            //ctang 2010-11-5
                            bool ifGlobalDE = false,
                            //ctang 2011-4-26
                            bool ifNoChildPopulate = false); 
    int createReferenceUnitI(EnumDSSTemplateUnitType iType, 
                             int iIndex, 
                             DSSTabularUnit* ipUnit, 
                             DSSTabularData *ipOriginalTabularData, 
                             DSSTabularUnit** oppUnit, 
                             bool ifDEonDEElementBrowse = false,
                             //ctang 2010-11-5
                             bool ifGlobalDE = false,
                             //ctang 2011-4-26
                             bool ifNoChildPopulate = false); 
    int SubsetPopulate(DSSTabularData *ipTabularData,
                       Int32 NumUnits, UnitFormInfo *pUnitInfo,
                       Int32 NumMissingUnit, DSS_ID *ipMissingUnitIDs,
                       UnitFormInfo *ipMissingUnitFormInfo,
                       Int32 NumSlices, DSSCubeDataSlice **ipSlices, METRIC_ID* ipMetricIDs, 
                       VARIANT_BOOL IsUnitSame,
                       Int32 nRowLimit,
                       Int32 NumDerivedElements,
                       Int32 *ipDerivedElementUnits,
                       bool iRestoreDouble,
                       Int32 inElementBrowseCount,
                       Int32* ipElementBrowseList,
                       bool ifDEonDTElementBrowse,
                       bool ibHasFilter,
                       DSSGUnitContainer* ipGUContainer,
                       bool ibRWDSubsetFromBase,
                       //ctang 2010-8-25 add relation to support extend populate
                       DSSRelation *ipRelation, DSS_ID* iExtendAttribtue,
                       Int32 inJoinAttribute, 
                       //ctang 2012-7-17
                       bool* ipConflictMetric,
                       //thuang 2012-11-21 for MDX or DI Attr
                       MCESchema* ipSchema,
                       bool ifRePopulateDEInExtension,
                       bool IsOfflineDDIC);
    void clearForSubsetting();
    inline DSSTabularIndexPool* getIndexPool() {return mpIndexPool;}
    int setIndexPool(DSSTabularIndexPool* ipIndexPool);
    inline vector<int> getCGPosition() {return mvCGPos;}
    inline vector<vector<int> > getCGOrdinals() {return mvCGOrdinals;}
    int getLevels(vector<int> &lvUnits);
    int filterUnit (Int32*pRowForOrginialIND,
                    Int32 nRow,
                    vector<Int32, MBase::Allocator<Int32> >& vRowForLUT,
                    Int32 *opRowForIND,
                    MBase::Buffer *pBuffer);
    
    //imihai, 2013-06-05 copy subtotals to tabular data
    int setSubtotalInformation(int index, int iTotalID, string &irTotalGUID, vector<EnumDSSDataType> &irvDataTypes, string &irTotalName, int iTotalType);
    EnumDSSMetricType getSubtotalType(int iTotalIndex);    
    const char *getSubtotalNameAndLength(int iTotalIndex, int &orLength);
    int CopySubtotals(vector<SubtotalInformation> modelSubtotals);
    GUID getDataSetGUID();
    map<GUID, vector<int> > * getDynamicDimtyByLevel(int iLevel);
    vector<map<GUID, vector<int> > >* getDynamicDimty();
    void setDynamicDimty(vector<map<GUID, vector<int> > >* ipvDD);
    vector<map<GUID, DFCSimpleExpr* > >* getDynamicBreakBy();
    void setDynamicBreakBy(vector<map<GUID, DFCSimpleExpr* > >* ipvDBB);
    int loadRecursiveUnits(DSSBinaryDataReader * ipReadStream);
    int getUnitLocale(int iUnitId, int &iLocale);
    
private:
    
    int hCreateDummyUnit(int iUnitIndex, EnumDSSTemplateUnitType iUnitType, MBase::CString &irUnitID, DSSTabularUnit **oppTabularData);
    int getUnitGUIDAndGlobalOffset(int i, GUID &irGUID, int &iOffset);
    int hSetUnqualifiedRows(DSSTabularIndex *ipIndices,
                            DSSXTabBitVector * ibvUnqualifiedRows,
                            Int32 *ipFilteredRows,
                            Int32 nFilteredRows);
    int  OldFilterIndices(DSSTabularIndex *pIndices, DSSCubeDataSlice *pSlice,
                              Int32 &nFilteredRows, Int32 *opFilteredRows,
                              MBase::Buffer *pBuffer, DSSTabularData *ipOriginalTabularData);
    //xiyu, 343249, missing unit form info
    int hPopulateMissingUnit(Int32 UnitIndex, DSS_ID & iUnitID,UnitFormInfo & iUnitForm, Int32 * opMissingpos = NULL, DSSGUnitContainer* ipGUContainer = NULL);

    int mDummyUnitIndex;
    
    DSSObjectContext *mpObjectContext;      //set by DSSViewDataSet::setObjectContext()
    DSSXTabView* mpXTabView;            //set by DSSViewDataSet::RefreshXTabView();
    
    //zhyang, move those from view dataset
    vector<DSSTabularUnit *> mvUnits;
    vector<DSSTabularUnit *> mvRecursiveUnits;
    vector<GUID> mvUnitGUID;
    vector<EnumDSSTemplateUnitType> mvUnitType;
    vector<int> mvUnitGlobalIndex;      //position in global unit container; might be -1 if it's not there
    vector<int> mvForms;    //zhyang, how many forms are there in the attribute, only used for missing unit
    int mMetircPos;
    int mnUnits;
    DSSGUnitContainer *mpGlobalLookupContainer;
    DSSTabularMetrics *mpMetrics;
    
    //ctang: record the CG position with ordinal
    vector<int> mvCGPos;
    vector<vector<int> > mvCGOrdinals;
    
    //imihai, 2013-06-05 copy subtotals to tabular data
    vector<SubtotalInformation> mvTotals;
    int mnTotals;
    
    //ctang: selected form info for Ref-lookup
    vector<int> mvAttributePos;
    vector<vector<int> > mvSelectedForms;
    vector<vector<int> > mvSelectedLocales;
    vector<bool> mvIsAllFormSelect;
    
    //lwang, to keep backward compatibility
    bool mbIsNewRWD;
    DSSDataModelBase *mpDataModelBase;
    
    //tliao, keep consistent with server side
    DSSTabularIndexPool* mpIndexPool;
    
    GUID mDataSetGUID;
    
    vector<map<GUID, vector<int> > > mvDynamicDimty;
    //tliao, 954998, 2014/08/28, support dynamic sort by
    vector<map<GUID, DFCSimpleExpr* > > mvDynamicSortBy;
    Buffer * mpBuffer;

    // weilin, DE32160, 2016/4/21 for locale of each unit
    vector<int>mvLocales;
}; 

#endif
