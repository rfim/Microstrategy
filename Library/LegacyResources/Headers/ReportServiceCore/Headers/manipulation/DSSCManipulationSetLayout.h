/*
 *  DSSCManipulationSetLayout.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-19.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CMANIPULATION_SET_LAYOUT_H
#define CMANIPULATION_SET_LAYOUT_H

#import "DSSObjectContext.h"
#include "DSSCManipulation.h"

class DSSAOLayout;
class DSSRWSectionNode;
class DSSRWNode;

class DSSCManipulationSetLayout : public DSSCManipulation
{
private:
	DSSRWSectionNode* mpRootNode;
	DSSRWSectionNode* mpLayoutNode;
	bool mNeedRedraw;
    bool mAOXML;
		
public:
	DSSCManipulationSetLayout(DSSRWSectionNode* ipRootNode, DSSRWSectionNode* ipLayoutNode, bool iAOXML = false);
	DSSCManipulationSetLayout(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSCManipulationSetLayout();
	virtual int Execute();
	DSSRWNode* getRootNode();
	DSSRWNode*	getSectionObject();
    
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
};

#endif
