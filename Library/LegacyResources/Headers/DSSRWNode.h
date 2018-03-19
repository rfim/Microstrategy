/*
 *  DSSRWNode.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef RWNode_h
#define RWNode_h

#include "DSSGroupByUnits.h"
#include <vector>

class DSSRWFieldGroupNode;
#include "DSSNonSharedPropertiesInViews.h"

class DSSRWNode : public DSSSecondClassObject
{
private:
	EnumDSSRWNodeType mType;
	int mIndex;
	double mLeft;
	double mTop;
	double mWidth;
	double mHeight;
	int mZIndex;
	std::string mName;
	bool mExpanded;
	std::string mTitle;
	std::string mToolTipText;
	double mTitleHeight;
	int mFormatRefIDs[DssRWNodeFormatLastOne-1];
	DSSThresholds* mpThresholds;
    DSSThresholds* mpTransactionThresholds;
	EnumDSSRWTreeType mTreeType;
    //std::string mstrSpecialFGNodeKey;
	DSSRWFieldGroupNode* mpFieldGroupNode;

protected:
	std::string mKey;
	DSSRWNode* mpParentNode;
	std::vector<DSSRWNode*> mChildren;
	DSSGroupByUnits* mpGroupByUnits;
	bool mLayoutLoaded;
	int hLoadFormatsFromBinary(DSSBinaryDataReader* ipReader);
	int hLoadThresholdsFromBinary(DSSBinaryDataReader* ipReader);
    int hLoadTransactionThresholdsFromBinary(DSSBinaryDataReader* ipReader);
	DSSNonSharedPropertiesInViews* mpNonSharedPrptiesInViews;
	bool mNodeLoaded;
public:
	DSSRWNode();
	virtual ~DSSRWNode();
	DSSRWNode(DSSObjectContext* ipObjectContext);
	void Init(DSSRWNode* ipParentNode, std::string iKey, int iIndex, EnumDSSRWTreeType iTreeType, EnumDSSRWNodeType iNodeType);
	EnumDSSRWNodeType getType();
    void setType(EnumDSSRWNodeType iType);
	std::string getKey();
    void setKey(std::string iKey);
	int getIndex();
    void setIndex(int iIndex);
	int Count();
	DSSRWNode* Item(int iIndex);
	DSSRWNode* getParentNode();
    void setParentNode(DSSRWNode* iNode);
	void getGroupByPath(std::vector<DSSGroupByUnit*>& oGroupByPath);
	double getLeft();
	double getTop();
	double getWidth();
	double getHeight();
	void setLeft(double value);
	void setTop(double value);
	void setWidth(double value);
	void setHeight(double value);
	int getZIndex();
    void setZIndex(int iZIndex);
	std::string getName();
    void setName(std::string istrName);
	bool getExpanded();
	std::string getTitle();
    void setTitle(std::string iTitle);
	std::string getToolTipText();
    void setToolTipText(std::string iToolTipText);
	double getTitleHeight();
	void setTitleHeight(double value);
    void setFormatRefID(int iIndex, int iFormatRefID);
	virtual int LoadDefnFromBinary(DSSBinaryDataReader* ipReader, bool ibDelSubTree=true);
	virtual int LoadPartialDefnFromBinary(DSSBinaryDataReader* ipReader);
	int LoadRWNodeInfoFromBinary(DSSBinaryDataReader* ipReader);
    
    //709677. we only set ipIteratorRmved in manipulations. lcao
	void ClearRWNodeSubTree(std::vector<DSSRWNode*>* ipNodesRmved=NULL, std::vector<DSSRWIterator*>* ipIteratorsRmved=NULL);
	DSSRWNode* getRoot();
	DSSFormat* getFormat(EnumDSSRWNodeFormatType iFormatType);
	void AddNodeMap(std::vector<DSSRWNode*>& ioModelChangedNodeMap, NodeMap& ioServerNodeMap, NodeAndControlsMap& ioSelectionUpdateNodeMap, std::string iSourceNodeKey, bool isUC, std::string iUserTargetKey, bool iExcludeSourceNode = false);
	DSSThresholds* getThresholds();
    DSSThresholds* getTransactionThresholds();
	DSSRWNode* getHighestCGB();
    DSSRWNode* getHighestNGB();
    /*
     This function returns all the NGB keys. The highest NGB key is in the first element of opvNGBKeys.
     */
    void getAllNGBKeys(std::vector<std::string>* opvNGBKeys);
	EnumDSSRWTreeType getTreeType();
	DSSRWNode* getLayout();

	virtual void MakeCleanDefn(bool ibDelSubTree=true);
	void getLeaves(std::vector<DSSRWNode*>& ioXTabLeaves, std::vector<DSSRWNode*>& ioNonXTabLeaves);
	void AddChild(DSSRWNode* ipNode);
    void RemoveChild(int iIndex, std::vector<DSSRWNode*>* ipNodesRmved=NULL, std::vector<DSSRWIterator*>* ipIteratorsRmved=NULL);
    void MoveChild(int iDstIndex, int iSrcIndex);

	bool getLayoutLoaded();
	void setLayoutLoaded(bool iLoaded);
	/** Check whether current node is an ancestor of ipNode.
	 @param ipNode Specified another RWNode.
	 @return Return true if the current node is an ancestor of ipNode. Otherwise return false.
	 If ipNode is NULL, return false.
	 */
	bool IsAncestorOf(DSSRWNode* ipNode);
	void getNodesForDatasetSelector(DSSRWControl* ipControl,  std::map < std::string, DSSRWNode* >* ipNodeMapServer, std::vector<DSSRWNode*>* ipNodesModelChanged, bool iSubTreeAllIn, std::map<std::string, std::vector<std::string> >* ipDDICModelChanged = NULL);

	int LoadNonSharedPropretiesFromBinary(DSSBinaryDataReader* ipReader);
	int getWidthMode();
	int getHeightMode();
	bool getVisible();

	//for multi document view
	bool getUsePageWidthAsLayoutWidth();
	double getMaxHeight();
	bool getCanGrow();
	bool getCanShrink();
	bool getHideIfEmpty();

    DSSRWNode* GetHighestCGBNode();
    void RemoveGroupByNode(int index, std::vector<DSSRWNode*>* ipNodesRmved=NULL, std::vector<DSSRWIterator*>* ipIteratorsRmved=NULL);
    bool hIsExist(std::vector<std::string>& iServerKey, std::string& iKey);
    std::vector<std::string> GetServerNodeKeys(); //Array of node keys
    void GetOtherCGBNodes(std::vector<std::string>& iServerKeys);

	// virtual function to be override by subclass to discard transaction
	virtual void DiscardTransaction()
	{
		return;
	};
    int LoadSpecialFGNodeFromBinary(DSSBinaryDataReader* ipReader);
    DSSRWNode* getSpecialFGNode();

    bool hasUC(); //whether there is a UC selector under this node

	void FindControlNodes(std::vector<DSSRWNode*>& ioOtherControlNodes);

    bool isNodeLoaded();
    int setNodeLoaded();
    
    DSSGroupByUnits* getGroupByUnits();
    DSSNonSharedPropertiesInViews* getNonSharedPropertiesInViews();
    virtual void Duplicate(DSSRWNode* ipNode);
    bool CanHaveDataModel();

    bool isUCorCGB();
    bool isUC();
    bool isCGB();
    
    void spliceChildren(int iIndex);  //similar with removeChild(), but do not remove the subtree and the key-node map. lcao, 20120109
    
    bool getResolvedMetricFormat(DSSMetric* ipMetric, std::vector<DSSFormat*>& ovFmts);
       
    DSSRWNode* getHighestNonUCorCGBNode();
    bool IsUnderGroupByAll();
    bool IsAboveNGBAll();
	void getDescendantNodes(std::vector<DSSRWNode*> *ivpNodes, bool ibExcludeSection = true);
    DSSRWNode* GetCGBorUCNodesUnderPanel(std::vector<std::string> &iCGBorUCKeys);
    bool hasLeafNodeWithType(EnumDSSRWNodeType iType);
};

inline void DSSRWNode::setName(std::string istrName)
{
    mName = istrName;
}

inline void DSSRWNode::setZIndex(int inZIndex)
{
    mZIndex = inZIndex;
}

inline void DSSRWNode::setTitle(std::string istrTitle)
{
    mTitle = istrTitle;
}

inline void DSSRWNode::setToolTipText(std::string istrToolTipText)
{
    mToolTipText = istrToolTipText;
}

inline void DSSRWNode::setKey(std::string iKey)
{
    mKey = iKey;
}

#endif
