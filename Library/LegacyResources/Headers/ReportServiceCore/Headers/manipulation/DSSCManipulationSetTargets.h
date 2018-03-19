//
//  DSSCManipulationSetTargets.h
//  ReportServiceCore
//
//  Created by Li, Dong on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CMANIPULATION_SET_TARGETS_H
#define CMANIPULATION_SET_TARGETS_H

#include "DSSCManipulationAddTargets.h"
#include "DSSCManipulationRemoveTargets.h"

class DSSCManipulationSetTargets: public DSSCManipulationRemoveTargets
{
public:
    DSSCManipulationSetTargets(DSSRWControlNode* ipControlNode, DSSRWControl* ipControl, std::vector<std::string> iTargetKeys);
    DSSCManipulationSetTargets(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationSetTargets();
    
    int Execute();
};

#endif
