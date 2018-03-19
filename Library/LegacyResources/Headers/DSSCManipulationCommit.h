/*
 *  DSSCManipulationCommit.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-11-16.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CMANIPULATION_COMMIT_H
#define CMANIPULATION_COMMIT_H

#include "DSSRWControl.h"
#include "DSSRWControlNode.h"
#include "DSSCManipulation.h"

class DSSCManipulationCommit : public DSSCManipulation
{
private:
	DSSRWControl* mpControl;
	std::string mControlNodeKey;
public:
	DSSCManipulationCommit(DSSRWControlNode* ipControlNode, DSSRWControl* ipControl);
	virtual ~DSSCManipulationCommit();
	int Execute();
	DSSRWControl* getControl();
	std::string getControlNodeKey();
};

#endif
