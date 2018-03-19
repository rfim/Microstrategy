/*
 *  DSSManipulationSetActionOnControl.h
 *  ReportServiceCore
 *
 *  Created by Li Jing on 11-3-07.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CMANIPULATION_SET_ACTION_ON_CONTROL_H
#define CMANIPULATION_SET_ACTION_ON_CONTROL_H

#include <vector>
#include "DSSrwdconstants.h"
#include "DSSCManipulation.h"

class DSSRWNode;
class DSSRWControlNode;
class DSSRWControl;

class DSSCManipulationSetActionOnControl : public DSSCManipulation
{
private:
	DSSRWControlNode* mpControlNode;
	DSSRWControl* mpControl;
	EnumDSSRWControlActionType mAction;
	void hSetActionOnNode(DSSRWNode *ipRWNode);
	
public:
	DSSCManipulationSetActionOnControl(DSSRWControlNode* ipControlNode, DSSRWControl* ipControl, EnumDSSRWControlActionType iAction);
	DSSCManipulationSetActionOnControl(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSCManipulationSetActionOnControl();
	int Execute();
	DSSRWControlNode* getControlNode();
	DSSRWControl* getControl();
	EnumDSSRWControlActionType getAction();
    
    void GenerateAODeltaXML();
#ifndef __ANDORID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
};

#endif
