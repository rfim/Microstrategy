/*
 *  DSSDataModelBase.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 1/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import "DSSAEHelper.h"
#import "DSSViewMap.h"
#import "DSSModelInterface.h"
#import <map>
class DSSObjectInfo;
//class ObjectMap;
class DSSViewMap;
class DSSTabularUnit;
class DSSXTabHashTable;
class DSSBaseElementProxy;
class DSSObjectContext;
class DSSDocumentInstance;
class DSSBaseElementsProxy;
class DSSDataSource;
class DSSDataModelTransaction;
class DSSDataModelControlImpl;
class DSSTabularData;

class DSSDataModelBase : public IStatisticsInfo, public IDSSSourceModelForTransaction
{
public:
	DSSDataModelBase();
	virtual ~DSSDataModelBase();
	
	int setNodeKey(MBase::CString &rNodeKey);
	
	virtual const string & getNodeKey();

	EnumDSSDataModelType getModelType();
	
	int AddUnit(int iUnitID, DSSTabularUnit*);
	
	virtual int FindUnit(const GUID &irGUID, EnumDSSTemplateUnitType iType);
	
	//find an DSSBaseElementProxy from unit
	virtual int FindElement(DSSBaseElementProxy *ipElement, int &orUnit, int &orKey);
	
	//return the number of unit
	virtual int Count();
	
	//get the i-th unit
	virtual DSSTabularUnit* Item(int index);
	
	DSSBaseElementProxy* CreateElementFromElementID(MBase::CString &irElementID);
	
	int setParentInfo(DSSDocumentInstance *ipDI);
	DSSObjectInfo * getParentInfo();
	
	/**
	 * Find out the group-by element specified by the given group-by paths and unit.
	 * @param ipPartialGBPaths:		a list of partial group-by paths
	 * @param irTargetUnitID:		the DSS ID of the unit object
	 * @param ipCurrentSelection:	the currently selected elements (belonging to the unitID). Each element is an IDSSElement object.
	 * @param AutoSelectionFlag:	the option of what elements to be returned. 1 means the minimum element, 2 the maximum element.
	 * @param orAutoSelection:		the array which should keep the resulting elements
	 * @param orbReplaced:			whether the currently selected elements are available in the model
	 */
	int getGroupbyElement(vector<int> &irvPartialGBPaths, int nGBUnits, vector<GUID> &irGBUnitID, 
						  GUID &irTargetUnitID, DSSBaseElementsProxy * ipCurrentElements, int AutoSelectionFlag,
						  bool &orbReplaced, vector<int> &orAutoSelection);

	inline const MBase::CString & getDataSourcePath(){return mDataSourcePath;};
	
	inline void setDataSourcePath(MBase::CString &irPath){mDataSourcePath = irPath;};
	
	inline void setStatus(int status){mStatus = (EnumXTabStatus)status;};

	inline EnumXTabStatus getStatus(){return mStatus;};
	
	int EnsureModelData();
	
	inline void setModelDataSource(DSSDataSource *datasource){mpModelDataSource = datasource;};
	
	inline DSSDataSource* getModelDataSource(){return mpModelDataSource;};
	
	void setTransactionDataModel(DSSDataModelTransaction *ipModel);
	
//	inline DSSDataModelTransaction *getTransactionDataModel(){return mpTransactionModel;};
	
	/*!
	 @abstract	data model will be removed from memory, release its content
	 @discussion it still keeps the necessary for reload from file again
	 */
	virtual void ReleaseUnusedMemory();
	
	/*!
	 @abstract	get the included element key given the excluded element
	 @discussion 
	 */
	int ResolveExcludedElements(DSSTabularUnit *ipUnit, vector<int> &irExcludedElementKeys, vector<int> &orSelectedElementKeys);

	int CopyTransactionDataModel(DSSDataModelBase *ipSourceDataModel, bool ibForceCopy = false);
	
	inline void setCurrentSliceFlag(int iFlag){mCurrentSliceFlag = iFlag;};
	
	inline int getCurrentSliceFlag(){return mCurrentSliceFlag;};
	
	int getViewMap(DSSViewMap** oppViewMap);
    
    DSSObjectContext* getObjectContext(){return mpObjContext;}
    
    int addControlModel(DSSDataModelControlImpl* ipControl);
    
    int getControlModel(std::string iKey, DSSDataModelControlImpl** oppControl);
    
	int getControlModelCount(int* opCount);
    
    void setSubFolder(std::string iSubFolder){mSubFolder = iSubFolder;}
    
    std::string getSubFolder(){return mSubFolder;}
private:
	
	void hCleanUp();

protected:
	int hSelectDistinct(vector<int> &irvData, int nSortedUnits, int nUnits, vector<int> &orSortedList, vector<int> &orDistinctList);
	
	int hCreateViewMap(int nUnits, int *ipUnits, int nRows, int *ipKeys, std::string iFilePath = "", char* ipGBPage = NULL, int GBPageSize = 0, int GBPagePosInFile = 0);
	
	int hCountGroupPaths();
	
	bool hNeedReorderKeys(int nGBUnits, vector<GUID> &irGBUnitID);
	
	int hMapGBUnit(int nGBUnits, vector<GUID> &irGBUnitID, vector<int> &orMap, bool &orPartial);
	
	void hCheckDuplicateGBUnits(int nGBUnits, vector<GUID> &irGBUnitID, vector<vector<int> > &orMap, bool &orbHasDuplicate);
	
	int hSelectWindows(int nPaths, int nGBUnits, vector<GUID> &irGBUnitID, int *ipGBPaths, vector<int> &orWindows);
	
	bool hSortElements(int lUnitID, vector<int>& lvElements);
	
	int hCalculateSizeOfBase();
    
    DSSTabularData* hGetTabularData();

	MBase::CString mNodeKey;
	EnumDSSDataModelType mType;
	
	DSSDocumentInstance *mpParentInfo;
	DSSObjectContext *mpObjContext;
	DSSViewMap *mpViewMap;			//map groupbypath to slice windows

	bool mNullOnTop;
	
	vector<DSSTabularUnit *> mvUnits;
	
	DSSDataSource *mpModelDataSource;
	MBase::CString mDataSourcePath;
	
	EnumXTabStatus mStatus;
	
	DSSDataModelTransaction *mpTransactionModel;
	
	int mCurrentSliceFlag;			//only contains current slice data
    
    
    vector<DSSDataModelControlImpl *> mvControlModels;
    std::map<std::string, int> mapKeyToControl;
    
    std::string mSubFolder;
    
    MBase::CString mFilePathMMP;      //mapped meory file path
    
    DSSTabularData *mpTabularData;
};


