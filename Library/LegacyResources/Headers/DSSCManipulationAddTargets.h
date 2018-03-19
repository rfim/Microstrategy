//
//  DSSCManipulationAddTargets.h
//  ReportServiceCore
//
//  Created by Li, Dong on 3/26/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#ifndef CMANIPULATION_ADD_TARGETS_H
#define CMANIPULATION_ADD_TARGETS_H

#include "DSSCManipulationAddControl.h"

class DSSCManipulationAddTargets: public DSSCManipulationAddControl
{
public:
    DSSCManipulationAddTargets(DSSRWControlNode* ipControlNode, DSSRWControl* ipControl, std::vector<std::string> iTargetKeys);
    DSSCManipulationAddTargets(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationAddTargets();
    
    int Execute();
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
};

#endif
