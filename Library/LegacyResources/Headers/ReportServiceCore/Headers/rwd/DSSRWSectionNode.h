/*
 *  DSSRWSectionNode.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef RWSectionNode_h
#define RWSectionNode_h

#include "DSSRWFieldGroupNode.h"

class DSSDataModelControlImpl;
class DSSDataModelFieldGroupImpl;
class DSSDataModelGroupbyImpl;

class DSSRWSectionNode : public DSSRWNode
{
private:
	EnumDSSRWSectionType mSectionType;
	int mPageByStyle;
	
	//GroupByNode:
	std::string mLatestControlKey;
	
	//PanelStack, layout:
	int mCurrentNodeIndex;
	std::string mCurrentNodeKey;
	
	DSSDataModelControlImpl* mpControlNodeModel;
	DSSDataModelFieldGroupImpl* mpFieldGroupModel;
	DSSDataModelGroupbyImpl* mpGroupByModel;
	
	DSSDataModelControlImpl* mpControlNodeModelLatest;
	DSSDataModelFieldGroupImpl* mpFieldGroupModelLatest;
	DSSDataModelGroupbyImpl* mpGroupByModelLatest;
		
    // weilin DE40067 2016/08/04
    std::vector<DSSRWTemplateNode *> mvSourceTemplateNodes;
public:
	DSSRWSectionNode();
	virtual ~DSSRWSectionNode();
	DSSRWSectionNode(DSSObjectContext* ipObjectContext);
	EnumDSSRWSectionType getSectionType();
    void setSectionType(EnumDSSRWSectionType iSectionType);
	int getPageByStyle();
	
	//GroupByNode:
	DSSGroupByUnits* getGroupByUnits();
    void setGroupByUnits(DSSGroupByUnits* ipGroupByUnits);
	std::string getLatestControlKey();
	void setLatestControlKey(std::string iKey);
	
	//PanelStack, layout:
	int getCurrentNodeIndex();
	std::string getCurrentNodeKey();
	void setCurrentNodeIndex(int iIndex);
	void setCurrentNodeKey(std::string iKey);
	DSSRWNode* getCurrentNode();
	
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader, bool ibDelSubTree=true);
	int LoadPartialDefnFromBinary(DSSBinaryDataReader* ipReader);
	
	DSSDataModelControlImpl* getControlNodeModel();
	DSSDataModelFieldGroupImpl* getFieldGroupModel();
	DSSDataModelGroupbyImpl* getGroupByModel();
	void setControlNodeModel(DSSDataModelControlImpl* ipControlNodeModel);
	void setFieldGroupModel(DSSDataModelFieldGroupImpl* ipFieldGroupModel);
	void setGroupByModel(DSSDataModelGroupbyImpl* ipGroupByModel);
	void getControlMetrics(std::vector<DSSTemplateMetric*>& ioTemplateMetrics);

	void MakeCleanDefn(bool ibDelSubTree=true);
	void DiscardTransaction();
	void UpdateDataModel();
	
	DSSRWSectionNode* getLayoutNode();

	bool HasNonSectionLeaf();
	
	int LoadLayoutKeysFromBinary(DSSBinaryDataReader* ipReader);
    
    bool isUCorCGB();
    
    void Duplicate(DSSRWNode* ipNode);

    // weilin DE40067 2016/08/04
    void InsertToSourceTemplateNodes(DSSRWTemplateNode* ipTemplateNode);
    std::vector<DSSRWTemplateNode *> getSourceTemplateNodes();
};

inline void DSSRWSectionNode::setSectionType(EnumDSSRWSectionType iSectionType)
{
    mSectionType = iSectionType;
}

inline void DSSRWSectionNode::setGroupByUnits(DSSGroupByUnits* ipGroupByUnits)
{
    mpGroupByUnits = ipGroupByUnits;
}

inline bool DSSRWSectionNode::isUCorCGB()
{
    if (mSectionType==DssRWSectionControlGroupBySection || mSectionType==DssRWSectionUnitCondition)
        return true;
    return false;
}


#endif
