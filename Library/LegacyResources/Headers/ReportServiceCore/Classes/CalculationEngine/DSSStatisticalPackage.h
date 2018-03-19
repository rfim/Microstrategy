//
//  DSSStatisticalPackage.h
//  ReportServiceCore
//
//  Created by He Tengfei on 3/8/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//
#ifndef CDSSStatisticalPackage_h
#define CDSSStatisticalPackage_h 1

#include "DSSFunctionPackage.h"

class DSSStatisticalPackage : public DSSFunctionPackage
{
public:
    DSSStatisticalPackage();
    ~DSSStatisticalPackage();
    
    int Count (int *opCount);
    
    int getCLSID (GUID *opCLSID);
    
    int getDescription (char **opDescription);
    
    int getName (char **opName);
    
    int getVersion (char **opVersion);
    
    DSSFunctionObject* Item(int iIndex);
};
#endif