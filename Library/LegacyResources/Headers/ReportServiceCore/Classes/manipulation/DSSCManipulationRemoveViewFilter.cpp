//
//  DSSCManipulationRemoveViewFilter.cpp
//  ReportServiceCore
//
//  Created by Li, Dong on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "DSSCManipulationRemoveViewFilter.h"
#include "DSSCExpression.h"
#include "DSSAODeltaXML.h"
#include "DSSExpNode.h"
#ifdef __ANDROID__
    #include "TinyXml/tinyxml.h"
#else
    #include "DSSSimpleXML.h"
#endif

DSSCManipulationRemoveViewFilter::DSSCManipulationRemoveViewFilter(DSSRWTemplateNode* ipTemplateNode):DSSCManipulationAddViewFilter(ipTemplateNode, NULL)
{
    mType = ManipulationRemoveViewFilter;
}

DSSCManipulationRemoveViewFilter::~DSSCManipulationRemoveViewFilter()
{
    
}

int DSSCManipulationRemoveViewFilter::Execute()
{
    return DSSCManipulationAddViewFilter::Execute();
}