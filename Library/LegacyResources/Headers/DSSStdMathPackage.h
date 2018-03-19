//
//  DSSStdMathPackage.h
//  ReportServiceCore
//
//  Created by He Tengfei on 2/21/12.
//  Copyright 2012 MicroStrategy, Inc.. All rights reserved.
//



#ifndef DSSStdMathPackage_h 
#define DSSStdMathPackage_h 1

#include "DSSFunctionPackage.h"

class DSSFunctionObject;

class DSSStdMathPackage : public DSSFunctionPackage
{
public:
    
    DSSStdMathPackage();
    
    ~DSSStdMathPackage();
      
    int Count (int *opCount);
    
    int getCLSID (GUID *opCLSID);
    
    int getDescription (char **opDescription);
    
    int getName (char **opName);
    
    int getVersion (char **opVersion);
    
    DSSFunctionObject* Item(int iIndex);
    
};

#endif