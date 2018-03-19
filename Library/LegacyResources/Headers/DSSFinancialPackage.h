//
//  DSSFinancialPackage.h
//  ReportServiceCore
//
//  Created by He Tengfei on 2/22/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#ifndef DSSFinancialPackage_h
#define DSSFinancialPackage_h 1

#include "DSSFunctionPackage.h"

class DSSFunctionObject;

class DSSFinancialPackage: public DSSFunctionPackage
{
public:
    DSSFinancialPackage();
    ~DSSFinancialPackage();
    
    int Count (int *opCount);
    
    int getCLSID (GUID *opCLSID);
    
    int getDescription (char **opDescription);
    
    int getName (char **opName);
    
    int getVersion (char **opVersion);
    
    DSSFunctionObject* Item(int iIndex);

};
#endif
