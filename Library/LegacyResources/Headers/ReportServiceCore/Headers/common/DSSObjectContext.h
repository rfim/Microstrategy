/*
 *  DSSObjectContext.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 10-1-6.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef OBJECT_CONTEXT_H
#define OBJECT_CONTEXT_H

#include <iostream>
#include <map>
#include "DSSFromatPropertyDefinition.h"
#include "DSSTransactionReport.h"
#include "GUID.h"

using MBase::operator <;

class DSSObjectInfo;
class DSSRWNode;
class DSSRWControl;
class DSSDocumentSubtotals;
class DSSFormat;
class DSSDocumentDefinition;
class DSSDataElement;
class DSSDataElements;
class DSSRWDEngine;
class DSSRWIterator;
class DSSRWDataModel;
class DSSFunction;
class DSSField;
class DSSOfflineTransactionCSIInfo;
class DSSOfflineTransactionCSIInfos;
class DSSUnitInfos;

typedef std::map<std::string, DSSPropertySetBaseClass*> PropertySetsMap;

class CLocalSubtotalMap
{
public:
	
	typedef std::map< GUID, int > TTemplateToLocalSubtotalMap;
	
	CLocalSubtotalMap() {};
	
	virtual ~CLocalSubtotalMap() 
	{
	
	};
	
	void Clear()
	{
		mMap.clear();
	}
	
	bool Find(GUID iKey, int *opLocalIndex)
	{
		if (!opLocalIndex) return false;
		
		TTemplateToLocalSubtotalMap::iterator itr = mMap.find(iKey);
		
		if (itr != mMap.end())
		{
			*opLocalIndex = itr->second;
			return true;
		}
		else return false;
	}
	
	void Insert(GUID iKey, int iLocalIndex)
	{
		TTemplateToLocalSubtotalMap::iterator itr = mMap.find(iKey);
		
		if (itr != mMap.end()) itr->second = iLocalIndex;
		else mMap.insert(TTemplateToLocalSubtotalMap::value_type(iKey, iLocalIndex));
	}
	
	bool ToArray(GUID* oTemplateID, int* oLocalIndex)
	{
		if (!oTemplateID || !oLocalIndex)
			return false;
		int i = 0;
		for (TTemplateToLocalSubtotalMap::iterator itr = mMap.begin(); itr != mMap.end(); itr++)
		{
			oTemplateID[i] = itr->first;
			oLocalIndex[i] = itr->second;
			i++;
		}
		return true;
	}
	
	int MapSize()
	{
		return static_cast<int>(mMap.size());
	}
	
private:
	TTemplateToLocalSubtotalMap mMap;
};

class CGlobalToLocalSubtotalMap
{
public:
	
	typedef std::map< int, CLocalSubtotalMap > TGlobalToLocalSubtotalMap;
	
	CGlobalToLocalSubtotalMap() {};
	
	virtual ~CGlobalToLocalSubtotalMap() 
	{
	
	};
	
	void Clear()
	{
		mMap.clear();
	}
	
	CLocalSubtotalMap * Find(int iKey)
	{	
		TGlobalToLocalSubtotalMap::iterator itr = mMap.find(iKey);
		
		if (itr != mMap.end())
			return &(itr->second);
		
		else return NULL;
	}
	
	void Insert(int iKey, CLocalSubtotalMap* iLocalMap)
	{
		TGlobalToLocalSubtotalMap::iterator itr = mMap.find(iKey);
		
		if (itr != mMap.end()) itr->second = *iLocalMap;
		else mMap.insert(TGlobalToLocalSubtotalMap::value_type(iKey, *iLocalMap));
	}
	
	int GetTupleCount()
	{
		int lSize = 0;
		for (TGlobalToLocalSubtotalMap::iterator itr = mMap.begin(); itr != mMap.end(); itr++)
			lSize += itr->second.MapSize();
		return lSize;
	}
	
private:
	TGlobalToLocalSubtotalMap mMap;
};

class CGlobalSubtotalMap
{
public:
	
	typedef std::map< GUID, int > TTemplateToGlobalSubtotalMap;
	
	CGlobalSubtotalMap() {};
	
	virtual ~CGlobalSubtotalMap() 
	{
		
	};
	
	void Clear()
	{
		mMap.clear();
	}
	
	bool Find(GUID iKey, int *opGlobalIndex)
	{
		if (!opGlobalIndex) return false;
		
		TTemplateToGlobalSubtotalMap::iterator itr = mMap.find(iKey);
		
		if (itr != mMap.end())
		{
			*opGlobalIndex = itr->second;
			return true;
		}
		else return false;
	}
	
	void Insert(GUID iKey, int iGlobalIndex)
	{
		TTemplateToGlobalSubtotalMap::iterator itr = mMap.find(iKey);
		
		if (itr != mMap.end()) itr->second = iGlobalIndex;
		else mMap.insert(TTemplateToGlobalSubtotalMap::value_type(iKey, iGlobalIndex));
	}
	
private:
	TTemplateToGlobalSubtotalMap mMap;
};

class CLocalToGlobalSubtotalMap
{
public:
	
	typedef std::map< int, CGlobalSubtotalMap > TLocalToGlobalSubtotalMap;
	
	CLocalToGlobalSubtotalMap() {};
	
	virtual ~CLocalToGlobalSubtotalMap()
	{
	
	};
	
	void Clear()
	{
		mMap.clear();
	}
	
	CGlobalSubtotalMap * Find(int iKey)
	{
		TLocalToGlobalSubtotalMap::iterator itr = mMap.find(iKey);
		
		if (itr != mMap.end())
			return &(itr->second);
		
		else return NULL;
	}
	
	void Insert(int iKey, CGlobalSubtotalMap* iGlobalMap)
	{
		TLocalToGlobalSubtotalMap::iterator itr = mMap.find(iKey);
		
		if (itr != mMap.end()) itr->second = *iGlobalMap;
		else mMap.insert(TLocalToGlobalSubtotalMap::value_type(iKey, *iGlobalMap));
	}
	
private:
	TLocalToGlobalSubtotalMap mMap;
};

struct NodeAndControls
{
	std::string NodeKey;
    EnumDSSRWNodeType NodeType;
	std::vector<std::string> ControlKeys;	
};

struct DatasetResolvedMacros
{
	std::string REPORTNAME;
	std::string REPORTDESCRIPTION;
	std::string CREATIONDATE;
	std::string CREATIONTIME;
	std::string PROJECT;
	std::string USER;
	std::string PROMPTN;
	std::string PROMPTDETAILS;
	std::string EXECUTIONTIME;
	std::string FILTERDETAILS;
	std::string PAGEBYDETAILS;
    std::string REPORTPROMPTANSWER;
};

typedef std::map < GUID, DSSObjectInfo* > ObjectMap;
typedef std::map < std::string, DSSRWNode* > NodeMap;
typedef std::map < std::string, DSSRWControl* > ControlMap;
typedef std::map < std::string, DSSField* > FieldMap;
typedef std::map < EnumDSSFunction, DSSFunction* > FunctionMap;
typedef std::vector < NodeAndControls* > NodeAndControlsVector;
typedef std::map < std::string, NodeAndControlsVector> MapFromTargetToControl;
typedef std::map < std::string, NodeAndControls*> NodeAndControlsMap;
typedef std::map < std::string, std::string> ImagePathMap; //map from image path to image time stamp
typedef std::map < GUID, DatasetResolvedMacros* > DatasetResolvedMacrosMap;
typedef std::map < GUID, DSSRWNode*> MapFromTemplateToNode;
typedef std::map < GUID, GUID> PartitionFunctionMap; // from function id to its partition function id
typedef std::map < std::string, std::vector<std::string>* > ConditionalTransactionMap;

class DSSObjectContext {
	
private:
	ObjectMap mObjectMap;
	NodeMap mNodeMap;
	ControlMap mControlMap;
    FunctionMap mFunctionMap;
    FieldMap mFieldMap;
	DSSDocumentSubtotals* mpDocumentSubtotals;
	CGlobalToLocalSubtotalMap mGlobalToLocalSubtotalMap;
	CLocalToGlobalSubtotalMap mLocalToGlobalSubtotalMap;
	DSSFromatPropertyDefinition* mpFormatPropertyDefinition;
	std::vector<DSSFormat*> mFormats;
    std::vector<DSSFormat*> mFormatsNewObject;
    std::map<int, int> mFormatRefCount;
	int hClearSubtotalMap();
	DSSDocumentDefinition* mpDocumentDefinition;
	MapFromTargetToControl mMapFromTargetToControl;
	ImagePathMap mImageMap;
	std::vector<std::string> miPadWidgetNames;
	std::vector<std::string> miPhoneWidgetNames;
	std::vector<DSSTransactionReport*> mvTransactionReports;
	DSSDataElements* mpDataElements;
	std::map<GUID, DSSFormat*> mFormatsMap;
	DatasetResolvedMacrosMap mDatasetResolvedMacrosMap;
	DSSRWDEngine* mpRWDEngine;
    DSSOfflineTransactionCSIInfos* mpCSIInfos;
    
    /*
    int mNodeKeySeed;
    int mControlKeySeed;
    int mFieldKeySeed;
    int mSubtotalKeySeed;
    */
    int mKeySeed; // node/control/field/subtotal share the same key seed
    
    GUID mDefaultDataSetID;
    
    PropertySetsMap mProjectPropertySets;
    bool mTemplateUnitAliasEnabled;
	std::vector<std::string> mvNodeKeysCategory2; // same as DSSDocumentInstance::mvNodeKeysCategory2
    DSSRWDataModel* mpDataModel;
    bool mSmartClient;
    MapFromTemplateToNode mMapFromTemplateToNode;
    PartitionFunctionMap mPartitionFunctionMap;
    ConditionalTransactionMap mConditionalTransactionMap;
    
    // jzeng, 08/04/14, document layer unit info. priority: template > document > dataset/object
    std::vector<DSSUnitInfos*> mvpUnitInfos;
public:
	int mFlag1;
	int mFlag2;
	friend class DSSDocumentInstance;	
	DSSObjectContext();
	virtual ~DSSObjectContext();
	
	DSSObjectInfo* FindObjectH(GUID& iID);
    void RemoveObjectByID(GUID& iID);
	void InsertObject(GUID& iID, DSSObjectInfo* ipObject);	
	
	virtual DSSRWNode* getNode(std::string iKey);
	void InsertNode(std::string iKey, DSSRWNode* ipNode);
	void RemoveNode(std::string iKey);
	
    virtual DSSRWControl* getControl(std::string iKey);
	void InsertControl(std::string iKey, DSSRWControl* ipControl);
	void RemoveControl(std::string iKey);
    void AddTargetToNodeAndControls(std::string iTargetKey, std::string iNodeKey);
    bool RemoveTargetToNodeAndControls(std::string iTargetKey, std::string iNodeKey = "");
    
    DSSField* getField(std::string iKey);
    void InsertField(std::string iKey, DSSField* ipField);
    void RemoveField(std::string iKey);
	
    DSSDocumentSubtotals* getDocumentSubtotals();
    DSSFunction* getFunction(EnumDSSFunction iType);
    void InsertFunction(EnumDSSFunction iType, DSSFunction* ipFunction);
    void InsertFunction(std::string iName, DSSFunction* ipFunction);
    void RemoveFunction(EnumDSSFunction iType);
    
	void setDocumentSubtotals(DSSDocumentSubtotals* ipDocumentSubtotals);
	
	int getTemplateLevelSubtotalIndex(int iGlobalIndex, GUID* ipTemplateID, int* opLocalIndex);
	int getDocumentLevelSubtotalIndex(int iLocalIndex, GUID* ipTemplateID, int* opGlobalIndex);
	int AddToSubtotalMap(int iGlobalIndex, GUID* ipTemplateID, int iLocalIndex);
	
//	std::string getDefaultFormatPropertyValue(std::string iPropertySetName, std::string iPropertyName);
	std::string getPropertyNameFromID(std::string iPropertySetName, int iPropertyID);
	EnumVariantType getPropertyTypeFromID(std::string iPropertySetName, int iPropertyID);
	std::string getPropertySetNameFromID(int iPropertySetID);
	DSSFormatPropertySetMap* getFormatPropertySet(std::string iPropertySetName);
	
	DSSFormat* getFormat(int iFormatID);
    
    //hyan,2012/03/07
    inline size_t getFormatVectorSize()
    {
        return mFormats.size();
    }
    
    DSSFormat* getFormatForNewObject(int iFormatID);
    int InsertFormat(DSSFormat* ipFormat);
    int InsertFormat(DSSFormat* ipFormat, int iDuplicateFrom);
    int getFormatRefCount(int iFormatRefID);
    int IncreaseFormatRefCount(int iFormatRefID);
    int DecreaseFormatRefCount(int iFormatRefID);
    
    std::map<int, int>& getFormatRefCountMap();
    
	DSSDocumentDefinition* getDocumentDefinition();
	MapFromTargetToControl* getMapFromTargetToControl();
	void InsertImage(std::string iImagePath, std::string iImageTimeStamp);
	ImagePathMap* getImageMap();
	void AddiPadWidget(std::string iWidgetName);
	void AddiPhoneWidget(std::string iWidgetName);
	std::vector<std::string>* getiPadWidgetNames();
	std::vector<std::string>* getiPhoneWidgetNames();
	
	int CountTransactionReports();
	DSSTransactionReport* ItemByIndexTransactionReport(int i);
	DSSTransactionReport* ItemByIDTransactionReport(GUID iID);
	DSSDataElements* getDataElements();
	
	DatasetResolvedMacrosMap* getDatasetResolvedMacrosMap();
	NodeMap* getNodeMap();
	int getFormatCount();
	
	DSSRWDEngine* getRWDEngine();
	void setRWDEngine(DSSRWDEngine* ipEngine);
    
    void updateKeySeed(std::string iKey);
    std::string createNodeKey();
    std::string createControlKey();
    std::string createFieldKey();
    std::string createSubtotalKey();
    static GUID createGUID();
    
    DSSDataElement* getDefaultDataSet();
    GUID getDefaultDataSetID();
    void setDefaultDataSetID(GUID iDefaultDataSetID);
    
    void loadProjectPropertySets(std::string istrPrjSettingXML);
    CComVariant* getProjectPropertyValue(std::string istrPropertySetName, std::string istrPropertyName);
    
    void setTemplateUnitAliasEnable(bool iIsEnabled);
    bool IsTemplateUnitAliasEnabled();
	int getKeySeed();
    void UpdateIteratorRemoveNode(DSSRWNode* ipNode, std::vector<DSSRWIterator*>* ipIteratorsRmved=NULL);
    void UpdateIteratorAddNode(DSSRWNode* ipNode);

	// cannot directly use document instance in the iterator update of android, so add the same API in object context
	std::vector<std::string>* getNodeKeysCategory2();
	void setNodeKeysCategory2(std::vector<std::string> lvNodeKeysCategory2);
	void resetNodeKeysCategory2();
    
    void setDataModel(DSSRWDataModel* ipDataModel);
    DSSRWDataModel* getDataModel();
    
    bool IsSmartClient();
    void setIsSmartClient(bool iSmartClient);
    
    void populateDDICs();
    void InsertTemplateToNode(const GUID& iTemplateId, DSSRWNode* ipTemplateNode);
    DSSRWNode* getNodeFromTemplate(const GUID& iTemplateId);
    
    void InsertPartitionFunction(const GUID& iFunctionID, const GUID& iPartitionFunctionID);
    DSSFunction* getPartitionFunction(const GUID& iFunctionID);
    
    void InsertConditionalTransaction(std::string iSourceNodeKey, std::string iTargetNodeKey);
    std::vector<std::string>* getConditionalTransactionTargetNodeKeys(std::string iSourceNodeKey);
    
    DSSOfflineTransactionCSIInfos* getCSIInfos();
    DSSOfflineTransactionCSIInfo* getCSIInfoByKey(std::string iKey);
    
    std::vector<DSSUnitInfos*>& getUnitInfos() { return mvpUnitInfos; }
    const std::vector<DSSUnitInfos*>& getUnitInfos() const { return mvpUnitInfos; }
    const DSSUnitInfos* getUnitInfos(const GUID& iDatasetID) const;
    int searchControlWithReplaceByID(GUID iID);
    ControlMap getControlMap();
};

inline DSSFormat* DSSObjectContext::getFormatForNewObject(int iFormatID)
{
    if (iFormatID > 0 && iFormatID <= mFormats.size())	// 1-based index
		return mFormatsNewObject[iFormatID-1];
	
	return NULL;
}

inline bool DSSObjectContext::IsSmartClient()
{
    return mSmartClient;
}

inline void DSSObjectContext::setIsSmartClient(bool iSmartClient)
{
    mSmartClient = iSmartClient;
}

#endif
