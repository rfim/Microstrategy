//
//  DSSCManipulationRemoveViewFilter.h
//  ReportServiceCore
//
//  Created by Li, Dong on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CMANIPULATION_REMOVE_VIEW_FILTER_H
#define CMANIPULATION_REMOVE_VIEW_FILTER_H

#include "DSSCManipulationAddViewFilter.h"
#include "DSSRWTemplateNode.h"
#ifndef __ANDROID__
    #include "DSSSimpleXML.h"
#endif

class DSSExpNode;
class DSSCManipulationRemoveViewFilter: public DSSCManipulationAddViewFilter
{
public:
    DSSCManipulationRemoveViewFilter(DSSRWTemplateNode* ipTemplateNode);
    ~DSSCManipulationRemoveViewFilter();
    
    int Execute();
private:
    DSSRWTemplateNode* mpTemplateNode;
};
#endif
