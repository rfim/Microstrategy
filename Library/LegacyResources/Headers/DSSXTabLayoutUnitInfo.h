//
//  DSSXTabLayoutUnitInfo.h
//  ReportServiceCore
//
//  Created by He Tengfei on 11/14/11.
//  Copyright 2011 MicroStrategy, Inc.. All rights reserved.
//

#ifndef DSSXTABLAYOUTUNITINFO_H
#define DSSXTABLAYOUTUNITINFO_H 

#include "DSSrwdconstants.h"


typedef int DSSXTabKey;

/**
 * tehe: DSSXTabLayoutInfo need not considering page by header
 */
class DSSXTabLayoutUnitInfo 
{
    
public:
    
    DSSXTabLayoutUnitInfo();
    
    ~DSSXTabLayoutUnitInfo();
    
    //	The attribute corresponding to this unit.
    int get_UnitID(int *pUnitID);
    
    //	Whether the unit is a TemplateMetric.
    int Is_Metric(int *pFlag);
    
    //	Init the XTabLayoutUnitInfo object.
    // int Init(int UnitID, DSSXTabKey ElementID = DSSXTabKeyUnset, bool IsMetric = false);
    int Init(int UnitID, bool IsMetric = false);
    
private:
    int mUnitID;
    
    //	Wether the metric is a TemplateMetric. "true" indicates
    //	it is.
    
    bool mIsMetric;
    
};

// Class DSSXTabLayoutUnitInfo 

#endif