/*
 *  DSSRWGridModel.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 9/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef RWGridModel_H
#define RWGridModel_H

#import <set>

#import "DSSRWNodeModel.h"

#import "DSSBaseString.h"

#include "DSSRWDDICModel.h"

using MBase::operator <;

class DSSViewDataSet;
class DSSRWSectionModelIterator;
class DSSXTabView;
class DSSTemplate;
class DSSRWGridModelIterator;
class DSSCube;
class DSSCalculationPlan;
class DSSSortDefinition;
class DSSRWGridIterator;
class DSSRelation;
class DSSTabularMetrics;
class DSSXTabBitVector;

class DSSRWDataModel;
//ctang: GBTable
class DSSGBTable;
class DFCCSI;
using MBase::Buffer;
class DSSAxis;
class DSSMaskInfos;



typedef struct SubtotalInformation{
	int toalID;
	string mGuid;
	vector<EnumDSSDataType> dataTypes;
	string name;
	EnumDSSMetricType type;
}SubtotalInformation;


class DSSRWGridModel : public DSSRWNodeModel,
public IDSSTemplateModel, public DSSRWDDICModel //xiazhou, 07/02/2014. DDIC related members and methods are moved to DSSRWDDICModel.
{
public:
    DSSRWGridModel(DSSRWDataModel *ipDataModel = NULL, bool ibOnDemandLoading = false); // tehe
	virtual ~DSSRWGridModel();
    int LoadDataFromBinary(DSSBinaryDataReader *ipReadStream);
    int setSubtotalInfo(int index, int iTotalID, string &irTotalGUID, vector<int> &irvDataTypes, string &irTotalName, int iTotalType);
	
    virtual int createIterator(DSSRWSectionModelIterator *ipParent, DSSRWNodeModelIterator **opIterator){return 0;};
    virtual int createIterator(DSSRWSectionModelIterator *ipParent, DSSRWGridIterator **opIterator);
    
    virtual int getCandidateElements(vector<GUID> &iSelectedUnits,
                                     vector<vector<int> > &iSelectedElements,
                                     GUID iUnit, vector<int> &opElements);
    
    int getDataModel(DSSRWDataModel **ppDataModel);
    
    int GetXTabView(DSSXTabView **ppXTabView);
    
    virtual DSSXTabView * &getXTabView();
    
    int CreateXTabView(DSSXTabView **ppXTabView);
    
    int GetActualGBElements(DSSRWSectionModelIterator* ipParentIterator, vector<vector<int> >& ovvActualGBElements, int& oLocalTag);
    
    int getTotalCount();
    
    int CheckSlice(vector<GUID>& irUnits, vector<bool>& irSelected, bool* isAvailable);
    
    int GetViewTemplate(DSSTemplate **ppTemplate);
    int GetViewDataSet(DSSViewDataSet **ppViewDataSet);
    inline DSSViewDataSet * GetViewDataSet(){return mpViewDataSet;};
    
    // 9/10/2012 tehe
    // int getGroupByUnits(vector<int> & ovGUID);
    // int getActualGroupByUnits(vector<int>& ovGroupByUnits);
    int getRunTimeUnits(vector<int> & ovRunTimeUnits);
    void setIsRunTimeUnit(int iUnitID,bool iIsRuntimeUnit);
    
    // tehe 2/3/2012 support schema relation for grid
    virtual int hFindRelation(GUID &iUnit, DSSRelation **oppRelation, GUID *opParent, GUID *opChild);
    
	virtual int Refresh();
    
    // 5/24/2012 tehe
    virtual int EnsureModelData(bool *obUpdated = NULL);
    int UnLoadSliceData();
    
    // 8/27/2012 tehe
    bool HasGroupbyMetrics();
    
    void ClearCMNodeModel();
    // compatible to old architecture
    
public:
    virtual DSSCube* & getCube();
    // return total units number
    int Count();
    
    int setTemplate(DSSTemplate *ipTemplate);
    
    virtual DSSTemplate *getTemplate();
    
    int getMetricUnitID();
    
    // DSSTabularUnit * Item(int index);
    
    int FindMetric(GUID iMetricID);
    
    virtual EnumDSSMetricType getSubtotalType(int iTotalIndex, GUID iGUID = GUID_NULL);
    
    int getElementNullDisplay(DSSDataFlag iNullFlag, string &orVal);
    
    const char *getSubtotalNameAndLength(int iTotalIndex, int &orLength);
    
    int getDataNullDisplay(DSSDataFlag iNullFlag, MBase::CString &orVal);
    
    int getMaxDataOffset(int iMetricIndex, bool ibSkipNull = false);
    
	int getMinDataOffset(int iMetricIndex, bool ibSkipNull = false);
    
    int getMetricID(int iMetricIndex, GUID &oMetricID);
    
    //int setCellFmtIDs(vector<MBase::CString> &irvVal);
    
    //int getCellFmtIDs(int offset, GUID &orVal);
    
    virtual DSSCalculationPlan *getCalculationPlan();
    
    int FindUnit(const GUID &irGUID, EnumDSSTemplateUnitType iType);
    
    virtual DSSTabularUnit* FindTabularUnit(const GUID &irGUID, EnumDSSTemplateUnitType iType);
    
    virtual void ReleaseUnusedMemory();
    
    virtual int ReCalculate();
    
    virtual inline DSSObjectContext * getObjectContext();
    
    virtual int getMetricsUnit(DSSTabularMetrics **oppMetricsUnit);
    
    bool hasCellFormat();
    
    virtual int CountUnits();
    
    virtual int LoadAllDataSlices();
    
    virtual int getBinaryTransactionModel (bool includeCommitted, unsigned char** oppBinaryModel, int* opSize);
    
    virtual int setBinaryTransactionModel (unsigned char * ipBinaryModel, int iSize);
    
    int EnsureTransactionData();
    virtual DSSDataModelTransaction *getTransactionDataModel(){return mpTransactionModel;};
    //fyang, 09/18/12, support Android client
    virtual EnumDSSSubtotalsPosition getRowSubtotalPosition();
	virtual EnumDSSSubtotalsPosition getColSubtotalPosition();
    
    int clearXTabView();
    int RefreshXTabViewThresholds();
    
    virtual bool IsEvaluatedOnClient();
    
    inline DFCCSI * getCSI() {return mpCSI;}
    
	bool isThresholdRecalculated(){return mbThresholdRecalculated;}
    
	int setThresholdCalcuated(bool ibCalculated){mbThresholdRecalculated = ibCalculated;return S_OK;};
    
    inline void CleanMinMaxOffset(){mvMaxDataOffset.clear();mvMinDataOffset.clear();};
    
    bool HasInvalidDE(){return mbHasInvalidDE;};
    string getDEInvalidErrorMessage(){return mErrorMessage;};
    
    GUID getSubtotalGUID(int iTotalIndex);
    
    bool IsTransactionEnabled();
    
    virtual int getGridModelVersion(){return 1;};
    
    bool hasMultiPassCSI();
    //tliao, 950812, 2014/8/13
    void setViewEvaluationIsDone() {mbViewEvaluationIsDone = true;}
    bool ViewEvaluationIsDone() {return mbViewEvaluationIsDone;}
    bool IsCubeDataReady();
protected:
    int hInitTabularUnitInViewDataSet();
    /*
     virtual int hFindRelation(MBase::RefContainer< CComPtr<ICDSSRelation> > &vRelations,
     GUID& iUnit,
     ICDSSRelation** oppRelation,
     GUID *opParent,
     GUID *opChild);
     */
    // DSSTemplate *mpOriginalTemplate;
    DSSTemplate *mpRuntimeTemplate;
    DSSTemplate *mpExtRuntimeTemplate;
    DSSViewDataSet *mpViewDataSet;
    
    friend class DSSRWDataModel;
    friend class DSSRWSectionModel;
    
private:
    
    // 9/19/2012 tehe
    void hPartialCleanUp();
    int hBuildSliceDataSource(DSSBinaryDataReader *ipReadStream);
    int hLoadDataFromCache();
    
    // 9/10/2012 tehe
    bool hSearchOnTemplate(GUID & lID);
    bool hFindUnitInAxis(DSSAxis *lpAxis, GUID & iID);
    int hFindAllRelation(GUID iUnit, vector<DSSRelation *> & ovRelations);
    void hInit();
    
    int hGetMetrics(DSSTemplate* ipVT, set<GUID, less<GUID> >& irGroupByMetrics, vector<int>& vMetrics);
    int hLoadSubTotals(DSSBinaryDataReader *ipReadStream);
    
    // 5/8/2013 tehe
    int hPrepareElementIndex();
    
    DSSXTabView* mpXTabView;
    // map<Int32, ICDSSRelation* > mUnitToRelationMap;
    vector<GUID> mvActualGroupByUnits;
    
    vector<SubtotalInformation> mvTotals;
    int mnTotals;
	EnumDSSSubtotalsPosition mTotalPosition;
    
	//bool mbIsNullOnTop;
    vector<string> mvDataNullDisplay;
    vector<string> mvElementNullDisplay;
    string mNullReplacement;
    
    // weak reference to viewdataset's calculation plan
    DSSCalculationPlan *mpCalculationPlan;
    int mMetricUnitID;
    
    map<int, DSSRelation*> mUnitToRelationMap;
    
    DSSGBTable* mpGBTable;
    
    //int mPrimaryFlag;
    
    // compatible to old architecture
private:
    // int mnSlices;
	// vector<int> mvRowList;		//row list of all slices
	// vector<int> mvRowWindowOffset;	//offset in mvRowList
	// vector<int> mvColList;		//column list of all slices
	// vector<int> mvColWindowOffset;	//offset in mvColList
	// vector<int> mvGBUnits;
    
	// vector<DSSSortDefinition*>* mvRowSorts;
	// vector<DSSSortDefinition*>* mvColumnSorts;
    
    vector<int> mvMaxDataOffset;	//max metric value at report level, ignore NULL
	vector<int> mvMinDataOffset;	//min metric value at report level, ignore NULL
    vector<GUID> mvCellFmtIDs;
    
    
    int mMCEBehavior; //zhyang, 678206, add as Linda's request
    
    // map<DSSTabularIndex *, DSSXTabBitVector*> mIndexMaskMap;
    DSSMaskInfos *mpNullorZeroMaskInfo;
    
    //fali, 815802, add flag to support the population of mpNullorZeroMaskInfo after initial loading
    bool mbHideZero;
    
    bool mbHideNull;
    
	bool mbThresholdRecalculated;
    
    bool mbHasInvalidDE;
    std::string mErrorMessage;
    
    Buffer * mpBuffer;
    DFCCSI * mpCSI;
    bool mbViewEvaluationIsDone;
    
};
/*
 inline int DSSRWGridModel::setCellFmtIDs(vector<MBase::CString> &irvVal)
 {
 vector<wchar_t> lGUIDString(32);
 int lnFmts = irvVal.size();
 mvCellFmtIDs.reserve(lnFmts);
 
 for (int i = 0; i < lnFmts; i++)
 {
 int len = irvVal[i].length();
 if (len > lGUIDString.size())
 lGUIDString.resize(len);
 mbstowcs(&lGUIDString[0], irvVal[i].c_str(), len);
 
 GUID lGUID = GUID_NULL;
 MBase::String2Guid(lGUID, &lGUIDString[0]);
 mvCellFmtIDs.push_back(lGUID);
 }
 return S_OK;
 }
 
 inline int DSSRWGridModel::getCellFmtIDs(int offset, GUID &orVal)
 {
 if (offset<0||offset>=mvCellFmtIDs.size()) {
 return E_INVALIDINDEX;
 }
 orVal=mvCellFmtIDs[offset];
 return S_OK;
 }*/


#endif