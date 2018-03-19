//
//  DSSCManipulationRemoveTargets.h
//  ReportServiceCore
//
//  Created by Li, Dong on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CMANIPULATION_REMOVE_TARGETS
#define CMANIPULATION_REMOVE_TARGETS

#include "DSSCManipulationRemoveControl.h"

class DSSCManipulationRemoveTargets: public DSSCManipulationRemoveControl
{
public:
    DSSCManipulationRemoveTargets(DSSRWControlNode* ipControlNode, DSSRWControl* ipControl, std::vector<std::string> iTargetKeys);
    DSSCManipulationRemoveTargets(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationRemoveTargets();
    
    int Execute();
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
};
#endif
