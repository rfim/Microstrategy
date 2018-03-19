//
//  DSSCManipulationSetTargets.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "DSSCManipulationSetTargets.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
#include "TinyXml/tinyxml.h"
#else
#include "DSSSimpleXML.h"
#endif
#include "DSSBaseElementProxy.h"

DSSCManipulationSetTargets::DSSCManipulationSetTargets(DSSRWControlNode* ipControlNode, DSSRWControl* ipControl, std::vector<std::string> iTargetKeys): DSSCManipulationRemoveTargets(ipControlNode, ipControl, iTargetKeys)
{
    mType = ManipulationSetTargets;
}

DSSCManipulationSetTargets::DSSCManipulationSetTargets(DSSActionObject* ipAO, DSSObjectContext* ipContext): DSSCManipulationRemoveTargets(ipAO, ipContext)
{
    mType = ManipulationSetTargets;
    
    if (!ipAO->IsDeltaXML())
        return;
    
#ifndef __ANDROID__
    DSSAODeltaXML* lpAO = static_cast<DSSAODeltaXML*> (ipAO);
    std::string lXMLString = lpAO->getXML();
    
    hParseAOXML(lXMLString, ipContext);
#endif
}

DSSCManipulationSetTargets::~DSSCManipulationSetTargets()
{
    
}

int DSSCManipulationSetTargets::Execute()
{
/*    std::vector<std::string> lTargetKeys;
    for(int i = 0; i < mpControl->getCountTargets(); i++)
        lTargetKeys.push_back(mpControl->getTargetKey(i));
    DSSCManipulationAddTargets* lpManipulationAddTargets = new DSSCManipulationAddTargets(mpControlNode, mpControl, lTargetKeys);
    for(int i = 0; i < mpControl->getCountTargets(); i++)
        lpManipulationAddTargets->AddControlTarget(mpControl->getTargetKey(i));

*/    return DSSCManipulationRemoveTargets::Execute();
}