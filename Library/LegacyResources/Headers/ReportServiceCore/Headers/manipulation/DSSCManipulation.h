/*
 *  DSSCManipulation.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 11-3-12.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CMANIPULATION_H
#define CMANIPULATION_H

#include "DSSRWControlNode.h"
#include "DSSActionObject.h"
#include <vector>
#include <map>

typedef std::vector<DSSActionObject*> ActionObjects;

class DSSCManipulation
{
protected:
	std::vector<DSSRWNode*> mNodesModelChanged;
    std::vector<DSSRWControl*> mControlsModelChanged;
	std::vector<DSSRWNode*> mNodesControlPropertiesChanged;
    std::map < std::string, DSSRWNode* > mNodeMapServer;
	
	DSSActionObject* mpActionObject;
	bool mIsFromActionObject;
	
	NodeAndControlsMap mNodeMapSelectionUpdate;
	bool mIsMacro;
	std::string mLayoutKeyNeeded;
	bool mIsLayoutKeyNeededInitial;
    bool mNeedNewObjects;
    std::vector<DSSObjectInfo*> mNewObjects;
	
	//Only for DSSCManipulationSetSelectionFromControl
	DSSRWNode* mpPanelStack;
	
	DSSRWControl* mpControl;
    DSSRWControlNode* mpControlNode;
	
	//Only for DSSCManipulationSetSelectionFromTemplate and DSSCMacroManipulation
	ActionObjects mActionObjects;
    
    // Only for DSSCManipulationSetSelectionFromControl/DSSCManipulationSetSelectionFromTemplate/DSSCMacroManipulation
    std::map<std::string, std::vector<std::string> > mDDICModelChanged;
    bool mRetrievalGlobalLookupTableHeader;
    
	void AddMetricConditionSelectorsToMap(DSSRWNode* node);
	void AddMetricConditionSelectorsToMap(DSSRWNode* ipNode, std::string iControlKey); // used by DSSCManipulationSetSelectionFromControl, DSSCManipulationToggleCurrentSelectionOnControl, DSSCManipulationSetUnsetFromControl
    void AddMetricConditionSelectors(DSSRWNode* ipNode, std::vector<std::string>* ipMCSKeys);
	DSSRWNode* hGetLowestNonGroupBySectionNode(DSSRWNode* ipNode);
    
    bool mAOXML;
    
    EnumManipulationCommandTypes mType;
    
    std::vector<DSSRWNode*> mNodesRemoved;
    std::vector<DSSRWIterator*> mIteratorsRemoved;
    std::vector<std::string> mNodeKeysExclusive;
    
    std::vector<DSSRWNode*> mNodesDataChanged;
    bool mHasPlaceHolderTargets;
    bool mNeedResetAllLayouts;
public:
	DSSCManipulation();
	virtual ~DSSCManipulation();
	virtual int Execute();
	virtual std::vector<DSSRWNode*>* getNodesModelChanged();
    std::vector<DSSRWControl*>* getControlsModelChanged();
	std::vector<DSSRWNode*>* getNodesControlPropertiesChanged();
	NodeMap* getNodeMapServer();
	std::vector<DSSActionObject*>* getActionObjects();
    std::vector<DSSObjectInfo*>* getNewObjects();
	NodeAndControlsMap* getNodeMapSelectionUpdate();
	std::string getLayoutKeyNeeded();
    void setLayoutKeyNeeded(std::string istrLayoutKeyNeeded);
	bool getIsLayoutKeyNeededInitial();
	DSSRWNode* getPanelStack();
	DSSRWControlNode* getControlNode();
	DSSRWControl* getControl();
	DSSActionObject* getActionObject();
	int hCheckLayoutLoaded(DSSRWNode* ipNode);
	virtual DSSBaseElementsProxy* getElements();
    
    static void SortControlTarget(std::vector<DSSRWNode*>& iTargets);
    static void SortNodeOrder(std::vector<DSSRWNode*>& ioNodeVector);
    static bool IsAncestOf(DSSRWNode* ipAncestNode, DSSRWNode* ipChildNode);
    
    EnumManipulationCommandTypes getType();
    void setType(EnumManipulationCommandTypes iType);
       
    std::vector<DSSRWNode*>* getNodesRemoved();
    
    //helper function, should be moved into ManipulationHelper later.
    static DSSRWSectionNode* searchGroupByNodes(DSSRWNode* ipNode, GUID iSourceID = GUID_NULL);
    static DSSRWSectionNode* searchGroupByNodes2(DSSRWNode* ipNode, GUID iSourceID = GUID_NULL);
    void AddControl(DSSRWNode* ipNode, DSSRWControl* ipControl, bool ibisUC, std::vector<std::string>* ipTargetKeys, std::vector<std::string>* ipGroupbyNodeKeys, std::vector<PropertyInfo*>* ipProperties=NULL);
    void AddControlTarget(DSSRWNode* ipNode, DSSRWControl* ipControl, std::string iTargetKey, bool ibisUC, std::vector<std::string>* ipGroupbyNodeKeys);
    std::map<std::string, std::vector<std::string> >* getDDICModelChanged();
    void searchAssociatedSourceNodes(DSSRWNode* ipTargetNode, DSSRWNode* ipUnderNode, std::vector<DSSRWNode*>* ipSourceNodes);
    
    void populateSubtreeIntoNodeModelChanged(DSSRWNode* ipNode, DSSRWNode* ipExcpNode=NULL); //populate the whole subtree except ipExcpNode under ipNode into mNodeModelChanged. lcao, 20130625, 753166, 759161
    bool getRetrieveGlobalLookupTableHeader();
    void setRetrieveGlobalLookupTableHeader(bool ibUConDataset);
	virtual DSSRWNode* getSelectionNode();
    void ClearNodeMapServer();
    
    virtual std::vector<std::string>* getNodeKeysExclusive();
    virtual bool IsNeedServer();
    
    virtual std::vector<DSSRWNode*>* getNodesDataChanged();
    bool getHasPlaceHolderTargets();
    void setHasPlaceHolderTargets(bool ibHasPHTargets);
    bool needResetAllLayouts();//928807
    virtual void resetAllLayouts();//928807
};

inline std::vector<DSSRWNode*>* DSSCManipulation::getNodesRemoved()
{
    return &mNodesRemoved;
}

inline EnumManipulationCommandTypes DSSCManipulation::getType()
{
    return mType;
}

inline void DSSCManipulation::setType(EnumManipulationCommandTypes iType)
{
    mType = iType;
}



#endif
