//
//  DSSCManipulationRemoveControl.h
//  ReportServiceCore
//
//  Created by Li, Dong on 1/12/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CMANIPULATION_REMOVE_CONTROL
#define CMANIPULATION_REMOVE_CONTROL

#include "DSSCManipulationRemoveRWNode.h"
#include "DSSRWControl.h"

// This manipulation is used for remove the control node only.
// To remove the control from a template, please use DSSCManipulationRemoveControlFromTemplate
class DSSCManipulationRemoveControl : public DSSCManipulationRemoveRWNode
{
public:
    DSSCManipulationRemoveControl(DSSRWNode* ipNode);
    DSSCManipulationRemoveControl(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    virtual ~DSSCManipulationRemoveControl();
    
    int Execute();
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
protected:
    // DSSRWControl* mpControl;
    std::vector<std::string> mTargetKeys;
    void RemoveControl();
    DSSRWNode* RemoveControlTarget(DSSRWNode* ipNode, std::string iTargetKey);
    DSSRWControlNode* getChildControlNode(DSSRWNode* ipNode);
};

#endif