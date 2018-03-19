/*
 *  DSSTabularMetrics.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import "DSSTabularUnit.h"

class DSSCubeDataSlice;

class DSSTabularMetrics : public DSSTabularUnit
{
public:
	DSSTabularMetrics();
	~DSSTabularMetrics();
    
	//int getUnitType(EnumDSSTemplateUnitType *opVal);
	int Init(int index, int nMetrics);
	int AddMetric(int index, MBase::CString &irGUID, int iDataType, int iSubtotalType, bool isDisplayed = true);
    int AddMetricToAnotherMetrics(DSSTabularMetrics* ipMetrics);
	int Count();
	int CountRows();
    int CountSlices();
	int getMetricID(int iMetric, GUID &orMetricID);
    
	EnumDSSDataType getMetricDataType(int iMetric);
	
	int FindElement(DSSBaseElementProxy *ipElement, int &orKey);
    
	int createElement(int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement, int iGlobalKey=-1, int iGlobalKeyVer = -1);
	
	int FindMetric(GUID &irMetricID);
	int FindMetricI(GUID irMetricID, int *opMetricIndex);
	
	virtual int size();
	
	EnumDSSMetricType getTotalImplementation(int iMetric);
    
    //juwnang, 04/19/2012
    bool compareElement(DSSBaseElementProxy *ipElement, int iKey);
    
    int addSlice(DSSCubeDataSlice* ipSlice);
    vector<DSSCubeDataSlice *>& getSliceList();
    const GUID& getMetricID(int iMetric);
    int FindDisplayedMetric(GUID &irMetricID);
    vector<GUID> getDisplayedMetricIDs();
    int setDisplayedMetricIDs(vector<GUID> iGUIDs);
    
    int setAggInfo(vector<GUID> &ivAggAttr, vector<EnumDSSAggregation> &ivAggFunc);
    int getAggregationInfo(GUID iMetricID, vector<GUID> &ovNonAggAttr, vector<EnumDSSAggregation> &ovAggInfo);
    int setDynamicAggFunc(EnumDSSMetricType iFunc);
    EnumDSSMetricType getDynamicAggFunc(int iIndex);
    int setTotalImplID(GUID iTotalImplID);
    int setDynamicAggID(GUID iDAImplID);
    GUID getTotalImplID(int index);
    GUID getDynamicAggID(int index);
    int clear();
    int setAvailableSubtotals(int iSystemSubtotal, vector<GUID> iUserDefinedSubtotals);
    int getAvailableSubtotals(GUID iMetricID, int &oSystemSubtotal, vector<GUID> &oUserDefinedSubtotals);
    
    int ItemC (int index, DSSCubeDataSlice **oppCubeSlice);
    
private:
	
	vector<GUID> mvMetricIDs;
	vector<MBase::CString> mvMetricIDStr;
	vector<EnumDSSDataType> mvDataTypes;
	vector<EnumDSSMetricType> mvTotalImpl;		//total subtotal implementation
	vector<DSSCubeDataSlice *> mvSlices;
    vector<GUID> mvDisplayedMetricIDs;      //metrics that will be displayed
    
    vector<vector<GUID> > mvAggAttr;
    vector<vector<EnumDSSAggregation> > mvAggInfo;
    vector<EnumDSSMetricType> mvDynamicAggFunc; //dynamic aggregation function
    vector<GUID> mvTotalImplIDs;
    vector<GUID> mvDynamicAggIDs;
    vector<int> mvAvailableSystemSubtotals;
    vector<vector<GUID> > mvAvailableUserDefinedSubtotals;
};

inline int DSSTabularMetrics::Count()
{
	return mvMetricIDs.size();
}

inline int DSSTabularMetrics::CountRows()
{
	return mvMetricIDs.size();
}

inline int DSSTabularMetrics::CountSlices()
{
	return mvSlices.size();
}

inline vector<DSSCubeDataSlice *>& DSSTabularMetrics::getSliceList()
{
    return mvSlices;
}

