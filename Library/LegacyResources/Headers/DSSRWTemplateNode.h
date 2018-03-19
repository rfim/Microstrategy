/*
 *  RETemplateNode.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef RWTemplateNode_h
#define RWTemplateNode_h

#include <vector>
#include "DSSRWControlNode.h"
#include "DSSTransactionReport.h"

class DSSTemplate;
class DSSFilter;
class DSSOfflineTransactionCSIInfo;
class DSSRWTemplateNode : public DSSRWNode
{
private:
	GUID mTemplateID;
	GUID mDataSourceID;
    GUID mFilterID;
	EnumDSSDisplayMode mDisplayMode;
	int mTemplateOrigin;
	DSSRWControls* mpControls;
	DSSGraphProperties* mpGraphProperties;	
	GUID mTransactionReportID;
	EnumDSSRWNodeTransactionChangeType mTransactionChangeType;
    int mRecordNumber;
	int mGraphPropertiesSize;
	std::map<std::string, int> mAssociatedNodeInfos;
    int mMaxSeries; // for pie graph
    int mMaxCategories; // for other graph
    std::string mOfflineTransactionCSIKey;
    std::map<std::string, bool> mNodesInSameViewer;
    
    // weilin 2016/06/28 US54024 Support secondary templates in map
    std::vector<std::string> mvSecondaryTemplateKeys;

    // weilin US51811 2016/07/19 mark whether this template is changed after sorting manipulation
    bool mbAfterSortingManipulation;
    // weilin US51811 2016/07/20 target CGB node
    std::vector<DSSRWSectionNode *> mTargetCGBNodeVec;
public:
	DSSRWTemplateNode();
	virtual ~DSSRWTemplateNode();
	DSSRWTemplateNode(DSSObjectContext* ipObjectContext);
	DSSTemplate* getTemplate();
    void setTemplateID(GUID iTemplateID);
	GUID getDataSourceID();
    void setDataSourceID(GUID iDataSourceID);
	DSSObjectInfo* getDataSourceObject();
    GUID getFilterID();
    void setFilterID(GUID iFilterID);
    DSSFilter* getFilter();
    DSSCExpression* getViewFilterExpression();
    void setViewFilterExpression(DSSCExpression* iExpression);
	EnumDSSDisplayMode getDisplayMode();
	int getTemplateOrigin();
	DSSRWControls* getControls();
	void setDisplayMode(EnumDSSDisplayMode iDisplayMode);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader, bool ibDelSubTree=true);
	DSSGraphProperties* getGraphProperties();
    void setGraphProperties(DSSGraphProperties* ipGraphProperties);
	void getControlMetrics(std::vector<DSSTemplateMetric*>& ioTemplateMetrics);
	void MakeCleanDefn(bool ibDelSubTree=true);
	DSSTransactionReport* getTransactionReport();
	bool IsTransactionEnabled();
	std::vector<int> getRequiredColumnIndexs();
	EnumDSSRWNodeTransactionChangeType getTransactionChangeType();
    int getTransactionRecordNumber();
	void DiscardTransaction();
	int getAssociatedNodeManipulationCategory(std::string& iKey);
	int getAssociatedNodesCount();
    int getAssociatedNodesCount(int checkBit);
	std::string getAssociatedNodeKeyByIndex(int iIndex);	// 0 based
	std::string getAssociatedNodeKeysAsString();
    std::string getAssociatedNodeKeysAsString(int checkBit);

    int getGraphPropertiesSize();
    std::map<std::string, int> getAssociatedNodeInfos();
    virtual void Duplicate(DSSRWNode* ipNode);
    void clearControls();
    void clearAssociatedInfos();
    int getMaxSeries();
    void setMaxSeries(int iMaxSeries);
    int getMaxCategories();
    void setMaxCategories(int iMaxCategories);
    void getLinkedAssociatedRWNodeKeys(std::vector<std::string>& iNodeKeys, int iCategory = 24, bool ibExcludeSelection = true);
    DSSOfflineTransactionCSIInfo* getOfflineTransactionCSIInfo();
    void insertNodeKeyInSameViewer(std::string iNodeKey, bool isPrimary = false);
    bool IsNodeInSameViewer(std::string iNodeKey);

    // weilin 2016/06/28 US54024 Support secondary templates in map
    void parseSecondaryKeys(std::string iSecondaryTemplateKeys);
    std::vector<std::string> getSecondaryTemplateKeys();

    // weilin 2016/07/19
    void setAfterSortingManipulation(bool ibTemp);
    bool getAfterSortingManipulation();

    // weilin US51811 2016/07/20 insert CGB target node
    void InsertToTargetCGBNodeVec(DSSRWSectionNode* iTargetCGBNode);

    // weilin US51811 2016/07/20 TargetCGBNodeVec
    std::vector<DSSRWSectionNode *> getTargetCGBNodeVec();
private:
    void UpdateAssociatedNode(std::map<std::string, std::string> iKeyMap, DSSRWTemplateNode* ipTemplateNode = NULL);    
};

#endif
