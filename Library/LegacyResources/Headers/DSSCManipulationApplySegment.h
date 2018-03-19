/*
 *  DSSCManipulationApplySegment.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 11-8-17.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CMANIPULATION_APPLY_SEGMENT_H
#define CMANIPULATION_APPLY_SEGMENT_H

#include "DSSRWControlNode.h"
#include "DSSCManipulation.h"

class DSSCManipulationApplySegment : public DSSCManipulation
{
private:
	GUID mSegmentID;
	DSSRWNode* mpRootNode;
	NodeMap mNodeMapServerDef; // for nodes needed to update definition from server
	// mNodeMapServer is for nodes needed to update data from server (similar to other manipulations)
	void hCheckControlNode(DSSRWNode* ipNode, std::vector<DSSRWNode*>* ioNodesModelChanged);
	void hCheckTargetNode(DSSRWNode* ipNode, std::vector<DSSRWNode*>* ioNodesModelChanged, bool isTargetNode);
	std::string hBuildExpressionXML();
	
public:
	DSSCManipulationApplySegment(DSSRWNode* ipRootNode, GUID iSegmentID);
	//DSSCManipulationApplySegment(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSCManipulationApplySegment();
	int Execute();
	NodeMap* getNodeMapServerDef();
};

#endif