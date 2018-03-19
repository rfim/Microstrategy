/*
 *  DSSDataModelXTabImpl.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import "DSSDataModelBase.h"
#import "DSSDataSource.h"
#import "DSSRWGridModel.h"

class DSSTabularUnit;
class DSSTabularMetrics;
class DSSCube;
class DSSXTabView;
class DSSTabularMetrics;
class DSSTabularIndexPool;
class DSSTemplateMetrics;
class DSSTemplate;
class DSSSortDefinition;
class DSSDataSource;
class DSSCalculationPlan;
class DSSTransactionReport;
class DFCCSITransactionBase;
class DSSDataModelXTabImpl : public DSSDataModelBase,
                             public IDSSTemplateModel
{
public:
	DSSDataModelXTabImpl();
	~DSSDataModelXTabImpl();
	
	int Init();
	
	int CreateXTabView();
	
	DSSCube * & getCube();
	
	DSSXTabView* & getXTabView();
	
	int CreateViewMap(int nSlices, vector<int> &irvGBUnits, vector<int> &irvGBPaths, vector<int> &irvRowWindowSize, vector<int> &irvRows, vector<int> &irvColWindowSize, vector<int> &irvCols);
	
	//get the slice window given group-by paths
	int Select(int nSlices, int nGBUnits, vector<GUID> &irGBUnitID, int *ipGBPaths,
			   vector<int> &orvRowWindow, vector<int> &orvColWindow,
			   int nExcludeGBUnits = 0, vector<GUID> *ipvExcludeGBUnitID = NULL, int nExcludePaths = 0, int *ipEXcludeGBPaths = NULL, bool ibIsGBAll = false);
	
	//refresh the view with new sort definition and layout
	virtual int Refresh();
	int Pivot(int iSrcAxis, int iSrcUnitPosition, int iTargetAxis, int iTargetUnitPosition);
	int setModel(DSSXTabView *ipView);
	int MoveMetric(DSSTemplateMetrics *ipMetrics);
	
	//populate the Null properties
	int setNullOnTop(bool &irVal);
	inline bool getNullOnTop();
	
	int setNullReplacement(MBase::CString &irVal);
	int getNullReplacement(MBase::CString &orReplacement);
	
	int setDataNullDisplay(vector<MBase::CString> &irvVal);
	int getDataNullDisplay(/*char*/ DSSDataFlag iNullFlag, MBase::CString &orVal); // jimzhang nov/03/2011 TQMS 510040 change type of first parameter from "char" to "DSSDataFlag"
	
	int setElementNullDisplay(vector<MBase::CString> &irvVal);
	int getElementNullDisplay(DSSDataFlag iNullFlag, MBase::CString &orVal);
	
	//set and get subtoal information
	int setSubtotalCount(int count);

	int setSubtotalInfo(int index, int iTotalID, MBase::CString &irTotalGUID, vector<int> &irvDataTypes, MBase::CString &irTotalName, int iTotalType);

	int getSubtotalName(int iTotalIndex, MBase::CString &orName);
	
	MBase::CString getSubtotalName(int iTotalIndex);
	
	const char *getSubtotalNameAndLength(int iTotalIndex, int &orLength);

	EnumDSSMetricType getSubtotalType(int iTotalIndex, GUID iGUID = GUID_NULL);
	
	inline int getSubtotalCount(){return (int)mvTotalInfo.size();};
    
    inline vector<SubtotalInformation> getSubtotals(){return mvTotalInfo;}

	inline int setCellFmtIDs(vector<MBase::CString> &irvVal);
	
	inline int getCellFmtIDs(int offset, GUID &orVal);

	int setMetricUnit(int);
	
	inline int getMetricUnitID();
	
	int getMetricsUnit(DSSTabularMetrics **oppMetricsUnit);
	
	int setIsRunTimeUnit(int index, bool val);
	
	inline void setTemplate(DSSTemplate* ipTemplate){mpTemplate = ipTemplate;};
	
	virtual DSSTemplate *getTemplate();
	
	int getMaxDataOffset(int iMetricIndex, bool ibSkipNull = false);

	int getMinDataOffset(int iMetricIndex, bool ibSkipNull = false);
	
	inline int countSlices(){return mnSlices;};
	
	int getRowRange(int iSlice, const int *&orpRows, int &ornRows);
	
	int getColRange(int iSlice, const int *&orpCols, int &ornCols);
	
	vector<int> & getGroupbyUnits(){return mvGBUnits;};

	virtual int FindElement(DSSBaseElementProxy *ipElement, int &orUnit, int &orKey);
	
	virtual int size();
	
	int setDataSource(DSSDataSource *ipDataSource);

	inline DSSDataSource *getDataSource(){return mpDataSource;};
	

	virtual EnumDSSSubtotalsPosition getRowSubtotalPosition();
	virtual EnumDSSSubtotalsPosition getColSubtotalPosition();
	
	virtual int ReCalculate();
	
	DSSCalculationPlan *getCalculationPlan();
    
    inline DSSObjectContext* getObjectContext(){return mpObjContext;};
	
	/*!
	 @abstract	data model will be removed from memory, release its content
	 @discussion it still keeps the necessary for reload from file again
	 */
	virtual void ReleaseUnusedMemory();
	
	void deleteSorts();
	

	int setDummyUnit(int iDummyUnitID);
    int getDummyUnitIndex(int *oDummyUnitIdx); //zhyang, compability with 931
	DSSTabularUnit* getDummyUnit(){return mpDummyUnit;}
	/*!
	 @abstract	load all metric data slices from disk regardless selected or not
	 @discussion it should be used only by transaction enabled grid due the performance impact
	 */
	int LoadAllDataSlices();
	
	//for offline transaction
	int getBinaryTransactionModel (bool includeCommitted, unsigned char** oppBinaryModel, int* opSize);
	
	virtual int setBinaryTransactionModel (unsigned char * ipBinaryModel, int size);
	
    static int LoadBinaryTransactionModel (unsigned char * ipBinaryModel, int size, DSSDataModelTransaction *lpTransactionModel,IDSSTemplateModel *lpTemplateNodeModel);
		
    virtual DSSDataModelTransaction *getTransactionDataModel(){return mpTransactionModel;};
    virtual int EnsureTransactionData();
    static int EnsureTransactionData(DSSDataModelTransaction *mpTransactionModel,
                                     IDSSTemplateModel *ipTemplateNodeModel,
                                     DSSTransactionReport *ipTargetReport,
                                     const vector<DSSTabularUnit *> *ipvTabularUnits,
                                     DFCCSITransactionBase* ipCSI = NULL);
    
    int setHeaderSource(DSSHeaderSource* ipSource);

    void setLoadHeaderBySlice(bool ibLoadbySlice){mbLoadHeaderBySlice = ibLoadbySlice;}
    
    inline DSSHeaderSource* getHeaderSource(){return mpHeaderSource;}

    int LoadFromHeaderSource(vector<int>& rvWindowID);
    
    int LoadHeaderKeys(DSSBinaryDataReader* ipBinaryReader, int lnKeys, int iAxis, vector<int>& rvWindowID, vector<int>& GBPath,  int nRuntimeUnit, vector<int>& MapRuntimeUnit2GBUnit);  
    
    int CreateViewMap2(int nSlices, vector<int> &irvGBUnits, vector<int> &irvGBPaths,vector<int*>& rvList, vector<int>& rvnList, vector<int*> &rvOffset, vector<int>& rvnOffset);
    int CreateViewMap3(int nSlices, vector<int> &irvGBUnits, int* pGBPath, char* pGBPage, int nGBPageSize, int GBPagePosInFile, vector<int*>& rvList, vector<int>& rvnList, vector<int*> &rvOffset, vector<int>& rvnOffset);
    int setDuplicatedRows(int iAxis, vector<int>& ivDuplicatedRows, vector<int>& ivOriginalRows);
    //the file path of the cache file containing the data should be mapped
    void setFilePath(std::string iPath){mFilePath = iPath;}
    
    std::string getFilePath(){return mFilePath;}
    //set the mapped file info for row list and row window offset
    void setMemPageInfo(char* rowList, char* rowWindow, char* colList, char* colWindow, 
                        int rowListSize, int rowWindowSize, int colListSize, int colWindowSize,
                        int rowListStart, int rowWindowStart, int colListStart, int colWindowStart,
                        int RowListOffset, int RowWindowOffset, int ColListOffset, int ColWindowOffset);
    
    int clearRowList(int iAxis);
    //always clear rowlist before try to set it
    int setRowList(int iAxis, int* rowList, int nRowList, int* rowOffset, int nRowOffset, bool ibMapped);
    //row list and row window offset on-demand mapping and unmapping
    int MapSliceWindow(int iAxis);
    
    int UnMapSliceWindow(int iAxis);
    
    int CreateMappedFileForDataSource();
    
    int LoadInputControlModel(std::string iKey, DSSDataModelControlImpl** oppControl);
    int CountUnits(){return (int)mvUnits.size();};
    
    virtual DSSTabularUnit* FindTabularUnit(const GUID &irGUID, EnumDSSTemplateUnitType iType);
    
    inline DSSTabularData *getTabularData() {return mpTabularData;}
    
    virtual int getGridModelVersion(){return 0;};
    
private:

	void hCleanUp();
	
	int LoadFromDataSource(int iSlice);
	
	void UnLoadFromDataSource();

	int hLoadAllFromDataSource();
    
    int hCreateMappedFile();
	
	DSSTabularMetrics *mpMetricsUnit;
	DSSTabularUnit *mpDummyUnit;
	int mMetricUnitID;
	
	vector<SubtotalInformation> mvTotalInfo;
	int mnTotals;
	EnumDSSSubtotalsPosition mTotalPosition;
	
	bool mbIsNullOnTop;
	vector<MBase::CString> mvDataNullDisplay;
	MBase::CString mNullReplacement;
	vector<MBase::CString> mvElementNullDisplay;
	
	DSSCube *mpCube;
	DSSXTabView *mpView;
	DSSTemplate *mpTemplate;
	
	bool mbMetricsMoved;

	int mnSlices;
    
    std::string mFilePath;
	int* mpRowList;		//row list of all slices
	int* mpRowWindowOffset;	//offset in mvRowList
	int* mpColList;		//column list of all slices
	int* mpColWindowOffset;	//offset in mvColList
    int mnRowList;
    int mnRowWindowOffset;
    int mnColList;
    int mnColWindowOffset;
    char* mpRowListPage;
    char* mpColListPage;
    char* mpRowWindowPage;
    char* mpColWindowPage;
    int mnRowListPage;
    int mnColListPage;
    int mnRowWindowPage;
    int mnColWindowPage;
    int mRowListPageStart;
    int mColListPageStart;
    int mRowWindowPageStart;
    int mColWindowPageStart;
    int mRowListPageOffset;
    int mRowWindowPageOffset;
    int mColListPageOffset;
    int mColWindowPageOffset;
    bool mbRowMapped;
    bool mbColMapped;
    
	vector<DSSSortDefinition*>* mvRowSorts;
	vector<DSSSortDefinition*>* mvColumnSorts;
	vector<GUID> mvCellFmtIDs;
	
	vector<int> mvMaxDataOffset;	//max metric value at report level, ignore NULL
	vector<int> mvMinDataOffset;	//min metric value at report level, ignore NULL
	
	vector<int> mvGBUnits;
	
	DSSDataSource *mpDataSource;
    bool mbDataSourceMapped;
   
    DSSHeaderSource *mpHeaderSource;
	bool mbLoadHeaderBySlice;
    
	DSSCalculationPlan *mpCalculationPlan;
    //the duplicatedrows can only caused by dummy units in the new binary structure for grids.
    vector<std::map<int,int> > mDuplicatedRows;
};

inline bool DSSDataModelXTabImpl::getNullOnTop()
{
	return mbIsNullOnTop;
}

inline int DSSDataModelXTabImpl::getMetricUnitID()
{
	return mMetricUnitID;
}

inline int DSSDataModelXTabImpl::getMetricsUnit(DSSTabularMetrics **oppMetricsUnit)
{
	if (oppMetricsUnit)
		*oppMetricsUnit = mpMetricsUnit;
	return S_OK;
}
inline int DSSDataModelXTabImpl::setCellFmtIDs(vector<MBase::CString> &irvVal)
{
	vector<wchar_t> lGUIDString(32);
	int lnFmts = (int)irvVal.size();
	mvCellFmtIDs.reserve(lnFmts);
	
	for (int i = 0; i < lnFmts; i++)
	{
#ifdef NO_WCHAR_SUPPORT
		GUID lGUID = GUID_NULL;
		MBase::String2Guid(lGUID, irvVal[i].c_str());
#else
		int len = (int)irvVal[i].length();
		if (len > lGUIDString.size())
			lGUIDString.resize(len);
		mbstowcs(&lGUIDString[0], irvVal[i].c_str(), len);

		GUID lGUID = GUID_NULL;
		MBase::String2Guid(lGUID, &lGUIDString[0]);	
#endif
		mvCellFmtIDs.push_back(lGUID);
	}
	return S_OK;
}
inline int DSSDataModelXTabImpl::getCellFmtIDs(int offset, GUID &orVal)
{
	if (offset<0||offset>=mvCellFmtIDs.size()) {
		return E_INVALIDINDEX;
	}
	orVal=mvCellFmtIDs[offset];
	return S_OK;
}
