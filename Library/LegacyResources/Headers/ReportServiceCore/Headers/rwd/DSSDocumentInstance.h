/*
 *  DSSDocumentInstance.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef DOCUMENT_INSTANCE_H
#define DOCUMENT_INSTANCE_H

#include <vector>
#include "DSSDocumentDefinition.h"
#include "Base/Base/Buffer.h"

class DSSRWGroupbyIterator;
typedef std::map< std::string, DSSRWGroupbyIterator* > GroupbyIterMap;
typedef std::map< std::string, DSSRWSectionNode* > GroupbyParentNodeMap;
typedef std::map< std::string, std::string > DatasetPromptAnswersMap;

typedef struct _IDandAlias
{
	GUID ID;
	std::string Alias;
}IDandAlias;

typedef std::map< GUID, std::vector<IDandAlias> > AliasMap;

typedef struct TemplateWithProperties
{
	DSSTemplate* mTemplate;
	bool mHasDE;
	bool mIsBasedOnCube;
	bool mIsCubeAllowsDrillOut;
	
	bool operator() (const TemplateWithProperties TWP1, const TemplateWithProperties TWP2) const
	{
		if (TWP1.mTemplate != TWP2.mTemplate)
			return (TWP1.mTemplate < TWP2.mTemplate);
		if (TWP1.mHasDE != TWP2.mHasDE)
			return (TWP1.mHasDE < TWP2.mHasDE);
		if (TWP1.mIsBasedOnCube != TWP2.mIsBasedOnCube)
			return (TWP1.mIsBasedOnCube < TWP2.mIsBasedOnCube);
		if (TWP1.mIsCubeAllowsDrillOut != TWP2.mIsCubeAllowsDrillOut)
			return (TWP1.mIsCubeAllowsDrillOut < TWP2.mIsCubeAllowsDrillOut);
		
		return false;
	}
}TemplateWithProperties;

typedef std::map< TemplateWithProperties, std::vector<DSSTemplateUnit*>, TemplateWithProperties > TemplateToUnitsMap;

class DSSRWDEngine;
class DSSRWIterator;
class DSSUserRuntime;
class DSSRWDataModel;
using MBase::Buffer;

class DSSDocumentInstance {
	
private:
	DSSDocumentDefinition* mpDefinition;
	DSSObjectContext* mpObjectContext;
	DSSUserRuntime* mpUserRuntime;
    
    //junwang, new rwd, 11/15/2011
    DSSRWDataModel* mpDataModel;
    
	GroupbyIterMap mpGroupbyIterMap;
    GroupbyParentNodeMap mpGBParentNodeMap;
	int mVersion;
	int mFlag1;
	int mFlag2;
	GUID mDefaultDataSetID;
	EnumDSSReportType mReportType;
	int ReadObjectDefinitionFromBinary(DSSBinaryDataReader* ipReader, DSSObjectInfo* ipObject, EnumDSSObjectType iType, bool iIsPartial=false);
	int hPopulateGroupByAlias();
	DSSRWDEngine *mpRWDEngine;
	int hLoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	int hLoadPartialDefnFromBinary(DSSBinaryDataReader* ipReader);
	int hLoadDataFromBinary(DSSBinaryDataReader* ipReader, const char* ipBinaryFilePath);
	int hLoadDrillFromBinary(DSSBinaryDataReader* ipReader);
	int hLoadImageFromBinary(DSSBinaryDataReader* ipReader);
	int hLoadWidgetFromBinary(DSSBinaryDataReader* ipReader);
	
	DSSObjectInfo* hNewObject(GUID& iID, EnumDSSObjectType iType, std::string iName, GUID& iContainerID, EnumDSSViewMedia iViewMedia = DssViewMediaReserved);
    
	int hLoadInstanceFromBinary(DSSBinaryDataReader* ipReader, const char* ipBinaryFilePath,bool iIsPartialRefresh);
	
	GUID mCacheId;
	Int64 mCacheUpdateTime;
	Int64 mCacheExpireTime;
	
    static void hReadPrivilegesFromData(std::string iData, std::string iPrivXMLStartTag, std::string iPrivXMLEndTag, std::vector<int>& iorPrivileges);
	
	// The method checks the user privilege and access on each drill path on each template and filters the drill path refs on the template units
	// iTemplateToUnitsMap maps each template to its units with drill paths refs
	void hPostProcessDrillBinary(TemplateToUnitsMap& iTemplateToUnitsMap);
    
    void hPostProcess();   //ported from flash code CDSSDocumentDefinition::postProcess(), simplified version to populate dataSourceID from template node to template. lcao 2012-5-25
	
	DSSFormat* hLoadFormat(DSSBinaryDataReader* ipReader, GUID& iID, int iIndex);
	int mDefinitionBinaryLimit;
	
	std::map<int, DSSFormat*> mDuplicateFormats;
    
	int mDefinitionSize;
    int mSmartClient;
    int mSmartClientProperty;
    
    //junwang, new rwd, 11/23/2011
    DSSRWGroupbyIterator* getLayoutGroupByIteratorNewRWD(std::string iKey);
	
	int hLoadCacheFromBinary(DSSBinaryDataReader* ipReader);
	int hLoadLayoutKeysFromBinary(DSSBinaryDataReader* ipReader);
    int hLoadDatasetsInfoFromBinary(DSSBinaryDataReader* ipReader);
    
    std::string mProjectID;
    std::string mDataSourcePath;
    MBase::Buffer * mpBuffer;
    
    // used only for UC on dataset:
    // category 1: nodes which only have their global lookup table updated, whose data source is not the target of UC on dataset;
    // category 2: nodes whose view will get refreshed after UC on dataset, whose data source is the target of UC on dataset;
    // when controller get data from i-server, category 2 nodes drop the original model and reload the data;
    // for category 1 nodes, controller needs to save the transaction data model to binary,
    // refresh teh nodes and reload the transaction data.
    std::vector<std::string> mvNodeKeysCategory2;
    
    //fali, 6/20/2014, used for dataset transaction error handling
    EnumDSSTransactionErrorCode mDSTxnErrorCode;
    std::vector<std::string> mvDSTxnErrorInfo;
public:
	DSSDocumentInstance();
	virtual ~DSSDocumentInstance();
	DSSDocumentDefinition* getDefinition();
	int LoadInstanceFromBinary(unsigned char *ipBinary, int length, bool iIsPartialRefresh, const char *ipBinaryFilePath = NULL);
	int CreateIterator(EnumDSSRWTreeType iTreeType, DSSRWIterator **oppIterator);
	int getVersion();
	int getFlag1();
	int getFlag2();
	GUID getDefaultDataSetID();
	EnumDSSReportType getReportType();
	DSSObjectContext* getObjectContext();
	DSSRWGroupbyIterator* getLayoutGroupByIterator(std::string iKey);
    DSSRWSectionNode* getGBParentNode(std::string iKey);
	GUID getCacheID();
	Int64 getCacheUpdateTime();
	Int64 getCacheExpireTime();
	int LoadImageSkipOthersFromBinary(unsigned char *ipBinaryDefn, int length, const char *ipBinaryFilePath = NULL);
	int LoadWidgetSkipOthersFromBinary(unsigned char *ipBinaryDefn, int length, const char *ipBinaryFilePath = NULL);
	void InitUserRuntimeFromData(std::string iData);
	DSSUserRuntime* getUserRuntime();
	int getDefinitionBinaryLimit();
	void setDefinitionBinaryLimit(int iLimit);
	void setLayoutLoaded(std::string iLayoutNodeKey, bool iLoaded);
	bool isInitialLayoutLoaded();
	bool IsTransactionEnabled();
    
    bool IsSmartClient();
    bool IsSmartClientProperty();
    bool HasOfflineCSI();
    
    //DSSRWDataModel is only used in new rwd engine. It may return NULL if it's using old rwd engine.
    //=====!!! Caller should check if it returns NULL !!! ====
    DSSRWDataModel* getDataModel(); //junwang, 11/15/2011,new rwd
    
    int hLoadCellFormatsFromBinary(DSSBinaryDataReader* ipReader);
	int LoadCacheFromBinary(unsigned char *ipBinary, int length, const char *ipBinaryFilePath = NULL);
	int LoadLayoutKeysFromBinary(unsigned char *ipBinary, int length, const char *ipBinaryFilePath = NULL);
	int LoadDatasetsInfoFromBinary(unsigned char *ipBinary, int length, const char *ipBinaryFilePath = NULL);
	int LoadPropertiesOfDocDefinitionOnlyFromBinary(unsigned char *ipBinary, int length, const char *ipBinaryFilePath = NULL);
    
    void setProjectID(std::string iProjectID);
    void setProjectProperties(std::string istrPrjProperties);
    void setTemplateUnitAliasEnable(bool iIsEnabled);
    std::vector<std::string>* getNodeKeysCategory2();
    void setNodeKeysCategory2(std::vector<std::string> lvNodeKeysCategory2);
    void resetNodeKeysCategory2();
    int applyTransactionDelta(std::vector<DSS_ID> ivDataSets, std::vector<unsigned char*>ivDelta, std::vector<int> ivDeltaSize, int* opFirstInvalidDelta = NULL);
    
    int getCachedIterator(std::string iNodeKey, DSSRWIterator** opIterator);
    
    bool IsSmartClientNotSupported();
    
    //fali, 6/20/2014, used for dataset transaction error handling
    EnumDSSTransactionErrorCode getDSTxnErrorCode();
    std::vector<std::string> getDSTxnErrorInfo();
    DatasetPromptAnswersMap getDatasetPromptAnswers();
    std::string getDatasetPromptAnswer(std::string iDatasetID);
    bool isDDA();
public:
    static DSSUserRuntime* CreateUserRuntimeFromData(std::string iData);
};

#endif

