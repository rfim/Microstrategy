/*
 *  DSSFunctionServer.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 2/4/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef FunctionServer_h
#define FunctionServer_h 1

#import <vector>
#import <map>

#include "DSSAEHelper.h"

using namespace std;

class DSSFunctionObject;
class DSSBuiltInPackage;
class DSSFunctionPackage;

// 2/22/2012 tehe now just for test 
const GUID GUID_DSSBuiltInPackage = {0xACC5BFA0,0x11D3,0xDD9C,{0xC0,0x00,0x81,0xB9,0xEA,0x33,0x22,0x4F}};
const GUID GUID_DSSStdMathPackage = {0xACC5BFA3,0x11D3,0xDD9C,{0xC0,0x00,0x81,0xB9,0xEA,0x33,0x22,0x4F}};

const GUID GUID_DSSFinancialPackage = {0xACC5BFA1,0x11D3,0xDD9C,{0xC0,0x00,0x81,0xB9,0xEA,0x33,0x22,0x4F}};

// 3/7/2012 support statistic package
const GUID GUID_DSSStatisticPackage = {0xACC5BFA2,0x11D3,0xDD9C,{0xC0,0x00,0x81,0xB9,0xEA,0x33,0x22,0x4F}};

// 2/27/2012 tehe make more flexible
typedef enum EnumDSSPackageType
{
    DssBuildInPackage = 0,
    DssStdMathPackage = 1,
    DssFinancialPackage = 2,
    DssStatisticPackage = 3 
} EnumDSSPackageType;

typedef std::map<EnumDSSPackageType, DSSFunctionPackage*> Map_Type2Package;
typedef std::pair<EnumDSSPackageType, DSSFunctionPackage*> Pair_Type2Package;
typedef std::map<EnumDSSPackageType, DSSFunctionPackage*>::iterator Iterator_Type2Package;

class DSSFunctionServer
{
public:
	DSSFunctionServer();
    ~DSSFunctionServer();
    
    DSSFunctionObject * getFunctionByIndex(int iIndex);
    
    DSSFunctionObject * FindFunctionObject(int iIndex, char *iPackageClassID);
    
    // 2/22/2012 tehe now just for simple test    
    DSSFunctionObject * FindFunctionObject(int iIndex, const GUID* iPackageID);
    
    // 2/27/2012 tehe get package type by guid
    int getFunctionPackageType(const GUID* iPackageID, EnumDSSPackageType *opPackageType);

private:
    //DSSBuiltInPackage *mpBuiltInPackage;
    // vector<char *> mpPackageClassIDs;
    // vector<DSSFunctionPackage *> mpPackages;
    
    // 2/27/2012 tehe support multi-packages
    Map_Type2Package mType2Package;
};

#endif
