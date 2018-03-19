//
//  DSSCManipulationRemoveTemplate.h
//  ReportServiceCore
//
//  Created by Zhu, Wei on 4/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ReportServiceCore_DSSCManipulationRemoveTemplate_h
#define ReportServiceCore_DSSCManipulationRemoveTemplate_h

#include "DSSCManipulationRemoveRWNode.h"

class DSSCManipulationRemoveTemplate : public DSSCManipulationRemoveRWNode
{
public:
    DSSCManipulationRemoveTemplate(DSSRWNode* ipNode);
    virtual ~DSSCManipulationRemoveTemplate();
    
    int Execute();
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
};


#endif
