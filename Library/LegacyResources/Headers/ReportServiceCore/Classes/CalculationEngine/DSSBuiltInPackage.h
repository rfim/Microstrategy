//
//  DSSBuiltInPackage.h
//  ReportServiceCore
//
//  Created by Jia Zhang on 9/9/11.
//  Copyright 2011 MicroStrategy, Inc. All rights reserved.
//

#ifndef BuiltInPackage_h
#define BuiltInPackage_h 1

#import "DSSFunctionPackage.h"

class DSSFunctionObject;

class DSSBuiltInPackage : public DSSFunctionPackage
{
public:
    DSSBuiltInPackage();
    ~DSSBuiltInPackage();
    
    int Count (int *opCount);
    
    int getCLSID (GUID *opCLSID);
    
    int getDescription (char **opDescription);
    
    int getName (char **opName);
    
    int getVersion (char **opVersion);
    
    DSSFunctionObject* Item(int iIndex);
    
    static DSSFunctionObject* getFunctionByIndex(int iIndex);
    
};

#endif