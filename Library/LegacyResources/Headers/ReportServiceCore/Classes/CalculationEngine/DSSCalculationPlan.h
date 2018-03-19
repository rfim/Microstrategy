/*
 *  DSSCalculationPlan.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 1/8/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef CalculationPlan_h
#define CalculationPlan_h 1

#import "CalculationEngine/DSSCalculationHelper.h"

class DSSDataModelXTabImpl;
class DSSCube;
class DSSViewDataSet;
class SerializedExtremeInfo;

typedef enum EnumCalcStatus
{
	DssCalcStatusUnInitialized = 1,
	DssCalcStatusUnCalculated,
	DssCalcStatusCalculated,
    DssCalcStatusNeedEvalThres
} EnumCalcStatus;


class DSSCalculationPlan
{
public:
	DSSCalculationPlan();
	
	~DSSCalculationPlan();
	
	int Init(DSSDataModelXTabImpl *ipDataModel);
	
	int Process(bool isBaseDataset = false);
	
	int AddEvent(DSSCalculationEvent &irEvent);
	
	int AddMetrics(vector<GUID> &irvMetricID, vector<int> &irvMetrics, vector<int> &irvMetrics2, DSSViewDataSet *ipViewDataSet = NULL);
    int AddMetrics2(vector<GUID> &irvMetricID, vector<int> &irvMetricList1, vector<int> &irvMetricList2);
    int getMetrics2(vector<GUID> &irvMetricID, vector<int> &irvMetricList1, vector<int> &irvMetricList2);
	
	int AddLevel(const vector<int> &irvLevelUnits, int ipTemplateSubTotal = 0);
    int getLevelInfo(int iLevel, vector<int> &ivrUnits, int &irTempateSubtotal);
	
	//return the number of calculation event
	int getNumEvents();
	
	//return the number of data slices a metric, including every subtotal type and subtotal level
	int getNumLevels();
	
	//return the number of all metrics used in the calculation
	//the same component metric in different smart metric may be treated as different metrics
	int getNumMetrics();
	
	//get the i-th calculation event
	DSSCalculationEvent* GetEvent(int iEvent);
    
    // get the info of attribute threshold, such as count/attribute id/slice ID for each level
    size_t getNumAttriThresholds();
    GUID getAttriID(int iIndex);
    int getAttriThresholdSliceID(int iLevel, const GUID &irAttributeID, int *opSliceID, bool* opIsCalculated = NULL);
    int getAttriThresholdSliceID(int iLevel, int irAttributeIndex, int *opSliceID, bool* opIsCalculated = NULL);
    int getAllAttriThresholdSliceID(vector<int> &irvAtrriThresholdSliceID);
    
	//get the metric slice ID of i-th even at the given level. iLevel, iEvent and iMetric are 0-based
	int getMetricSliceID(int iLevel, int iEvent, int iMetric,
						 bool *opIsCalculated, int *opSliceID);
    
    int getMetricSliceID(int iLevel, GUID &irGUID, bool *opIsCalculated, int *opSliceID);
    int getMetricThresholdSliceID(int iLevel, GUID &irGUID, int *opSliceID);
    int getMetricExtremeSliceID(int iLevel, GUID &irGUID, bool *opIsCalculated, int *opSliceID);
	
	//set the metric slice ID of i-th even at the given level. iLevel, iEvent and iMetric are 0-based
	int setMetricSliceID(int iLevel, int iEvent, int iMetric,
						 bool bIsCalculated, int iSliceID);
    int setMetricSliceID2(int iMetric, int iSliceID);
	//get the GUID of non-smart DSSMetric
	GUID getMetricID(int iMetric);
	
	//get the GUID of container DSSMetric
	GUID getMetricID2(int iMetric);
	
	int getMetricIndex(GUID &irMetricID);
    HRESULT get_MetricIndexEx(DSS_ID &irDatasetID, DSS_ID &irMetricID, Int32 *opIndex);
    int get_MetricIndexEx(DSS_ID &irDatasetID, DSS_ID &irMetricID);
	
	int getMetricIndexWithGoverner(GUID &irMetricID, GUID &irGovMetricID);
	
	//get the subtoal type
	virtual int getTemplateSubtotal(int iLevel);
	
	const vector<int> & getSubtotalLevel(int iLevel){return mvLevels[iLevel].mvUnits;};
	
	//either DssDefaultOrdering or DssExtendedOrdering
	inline void setOrderingRule(EnumOrderingType iRule){mOrderingRule = iRule;};
	
	inline EnumOrderingType getOrderingRule(){return mOrderingRule;};
	
	bool IsDecomposable(GUID &irMetricID);
	
	typedef struct LevelInfo
	{
		int mTemplateSubtotal;  // index of the template subtotal, 1-based, -1 means no subtotal
		vector<int> mvUnits;
	}LevelInfo;
	
	//reset slice flag to uncalculated for any slice related to iMetric, expect the original metric event at report level
	int MarkUnCalculated(int iMetric);
	
	inline void setStatus(EnumCalcStatus iStatus){
		if (mStatus != DssCalcStatusUnInitialized || iStatus == DssCalcStatusCalculated)
			mStatus = iStatus;
	};
	
	inline EnumCalcStatus getStatus(){return mStatus;};
	
	int FinalizeEvent(DSSCube *ipCube);
	
	inline void setNullCheckingOption(vector<EnumNullCheckingOption> &irvOption)
	{
		mvMetricNullCheckingOption.assign(irvOption.begin(), irvOption.end());
	};
	
	inline void setLevelMetricFlag(vector<bool> &irvFlag)
	{
		mvLevelMetricFlag.assign(irvFlag.begin(), irvFlag.end());
	}
	
	inline EnumNullCheckingOption getNullCheckingOption(int iMetric)
	{
		return (iMetric >= 0 && iMetric < mNumMetrics && !mvMetricNullCheckingOption.empty()) ? mvMetricNullCheckingOption[mvComponentMetrics[iMetric]] : DssNumericErrorTreatAs;
	}
	
	bool IsLevelMetric(int iMetric)
	{
		return (iMetric >= 0 && iMetric < mNumMetrics && !mvLevelMetricFlag.empty()) ? mvLevelMetricFlag[mvComponentMetrics[iMetric]] : false;
	}
	
// tehe 11/23/2011 for new rwd
public:
    int getEventIndex(GUID iUnitID);
    int getNumUnits(int iLevel, int *opNumUnits);
    int getUnits(int iLevel,  int iNumUnits, int *opUnitAxis, int *opUnitIndex);
    int getUnitIDs(int iLevel, int iNumUnits, GUID *opUnitIDs);
    int getUnitIDsAndTypes(int iLevel,  int iNumUnits, GUID* opUnitIDs, EnumDSSTemplateUnitType* opUnitTypes);
    int setMappingTable(vector<int> &irMappingTable);
    int setThreshold(vector<int> &irThreshold);
    int get_MetricIDEx(Int32 iMetric, DSS_ID *opMetric);
    int setThresholdSliceID(int iLevel, int iMetric, int iSliceID);
    
    inline vector<int> getMappingTable(){return mvNewRWDMappingTable;};
    inline vector<int> getThreshold(){return mvNewRWDThreshold;};
    
    int getMaxCalculatedSliceID();
    int getMaxUsefulSliceID();
    
    int InitMetricListEx(int iSize, GUID *ipMetricIDs, int *ipSliceIDs);
    
    int setAttriThresholdIDs(const vector<GUID>& irAttriIDs);
    int setAttriThresholdSliceIDs(const vector<int> &irSliceIDs);
    int setAttriThresholdSliceID(int iLevel, const GUID &irAttributeID, int iSliceID);
    
    //zhyang, extreme information
    int setExtremInfo(vector<EnumDSSRuntimeSubtotalType> ivExtremType,
                      vector<int> ivExtremeMappingTable,
                      vector<int> ivExtremeInfoMappingTable,
                      vector<SerializedExtremeInfo> ivExtremeInfo);
    int getExtremeType(int iLevel, EnumDSSRuntimeSubtotalType *ipExtremeType);
    int getModelInfo(int iLevel, int &lModelIndex, int &lTRMetricID, int &lnXVariables, int *&lpXVariables, EnumDSSObjectType *&lpXVariableTypes, int *&lpXIndexMetric);
    int getAllExtremeSliceID(vector<int> &irvExtremeSliceID);
    
private:

	int hCheckCompoundMetricEvent();
	
	int hInitMappingTable(DSSDataModelXTabImpl *ipDataModel);

    int hSelectDistinvtComponentMetricSlice(vector<int> &orSliceID);
    
	int hNewInitMappingTable(bool isBaseDataset = false);
	
	//what to serialize
	//1, event list (ordered? just doable event list?): <event type, GUID>
	//	1.a validation, which compound metric event can't be evaluated
	//	1.b validation, which subtotal event can't be evaluated.
	//		1.b.1 if compound metric and its component metric use different total implementation, fails
	//2, metrics
	//	2.a. GUID
	//	2.b. metric list 1
	//	2.c. metric list 2
	//3, component metric ID 2
	//4, level info? subtoal info?
	//	4.a. subtotal type
	//	4.b. level units
	//5, EnumOrderingType
	//6, level units list?
	//7, original metric list, and compound metric list
	//8, dimension aware: true = fail? false = fail? 

	vector<GUID> mvMetricID;							//unique template metric GUID
    vector<GUID> mvThresholdAttribute;  // vector for GUID of template attribute which have threshold definied
    vector<int> mvThresholdAttributeSliceID;    // vector for slice id of the attribuet threshold
    vector<bool> mvAttributeSliceFlag;    // vector for calculate status of each attribute threshold's slice
    
	DSSDataModelXTabImpl *mpDataModel;
	
	int mNumEvents;
	vector<DSSCalculationEvent> mvCalculationEvents;		//sorted event list
	
	int mNumMetrics;
	vector<int> mvComponentMetrics;						//component metric for smartmetric calculation
	vector<int> mvContainerMetrics;
	vector<bool> mvbIsDecomposable;
	
	int mNumLevels;
	vector<LevelInfo> mvLevels;				//subtotal levels
	
	
	EnumOrderingType mOrderingRule;
	
	vector<int> mvSliceMappingTable;		//mvSliceMappingTable[iLevel][iEvent][iMetric]
	vector<bool> mvSliceFlag;				//mvSliceFlag[iLevel][iEvent][iMetric]
	
	EnumCalcStatus mStatus;
	
	vector<int> mvPreviousSlices;			//used to update cube after calculation
	
	vector<EnumNullCheckingOption> mvMetricNullCheckingOption;
	vector<bool> mvLevelMetricFlag;
    vector<int> mvNewRWDMappingTable;
    vector<int> mvNewRWDThreshold;
    
    vector<EnumDSSRuntimeSubtotalType> mvExtremType;
    vector<int> mvExtremeMappingTable;
    vector<int> mvExtremeInfoMappingTable;
    vector<SerializedExtremeInfo> mvExtremeInfo;
};


#endif
