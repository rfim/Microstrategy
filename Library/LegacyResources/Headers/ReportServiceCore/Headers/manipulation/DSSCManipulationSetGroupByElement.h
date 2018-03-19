/*
 *  DSSCManipulationSetGroupByElement.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-22.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CMANIPULATION_SET_GROUPBY_ELEMENT_H
#define CMANIPULATION_SET_GROUPBY_ELEMENT_H

#include <vector>
#include "DSSObjectContext.h"
#include "DSSCManipulation.h"

class DSSAOGroupBy;
class DSSRWSectionNode;
class DSSRWNode;
class DSSBaseElementProxy;

typedef std::vector<DSSRWSectionNode*> NodeVectorGroupBy;
typedef std::vector<DSSRWNode*> NodeVectorModelChanged;

typedef enum _EnumDSSSetGroupbyStatus
{
    DssSetGroupByReserved       = -1,
    DssSetGroupBySingleToSingle = 0,
    DssSetGroupByAllToSingle    = 1,
    DssSetGroupBySingleToAll    = 2
} EnumDSSSetGroupbyStatus;

class DSSCManipulationSetGroupByElement : public DSSCManipulation
{
private:
	DSSRWSectionNode* mpGroupBySectionNode;
	DSSBaseElementProxy* mpElement;
	NodeVectorGroupBy mGroupBySectionNodes;
	DSSRWSectionNode* mpRedrawParentNode;
	int mRedrawChildIndex;
	NodeVectorModelChanged mNodesModelChanged;
	NodeAndControlsMap mNodeMapSelectionUpdate;
	bool mAffectOtherLayout;
	void hAddGroupBySectionNode(DSSRWSectionNode* ipGroupBySectionNode);
	void hAddNodeModelChanged(DSSRWNode* ipNode, NodeAndControlsMap& ioSelectionUpdateNodeMap);
	bool mIsFromActionObject;
    bool mAOXML;
	EnumDSSSetGroupbyStatus mStatus;
    
	void FindSameGroupByUnit(DSSRWNode* ipNode, DSSGroupByUnit* ipGroupByUnit);
	
public:
	DSSCManipulationSetGroupByElement(DSSRWSectionNode* ipGroupBySectionNode, DSSBaseElementProxy* ipElement, bool iAOXML = false, bool iAffectOtherLayout = false);
	DSSCManipulationSetGroupByElement(DSSActionObject* ipAO, DSSObjectContext* ipContext, bool iAffectOtherLayout = false);
	virtual ~DSSCManipulationSetGroupByElement();
	int Execute();
	NodeVectorGroupBy* getGroupBySectionNodes();
	DSSRWSectionNode* getRedrawParentNode();
	int getRedrawChildIndex();
	NodeVectorModelChanged* getNodesModelChanged();
	NodeAndControlsMap* getNodeMapSelectionUpdate();
	std::string getGroupByNodeKey();
    EnumDSSSetGroupbyStatus getStatus();
    DSSRWSectionNode* getGroupBySectionNode();
    DSSBaseElementProxy* getElement();
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
};

#endif
