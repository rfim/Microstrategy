//
//  DSSStatisticalPackage.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 3/8/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#include "DSSStatisticalPackage.h"

#include "DSSStatisticsSTOS.h"
// DSSStatisticsVTOS
#include "DSSStatisticsVTOS.h"
// DSSStatisticsVSTOS
#include "DSSStatisticsVSTOS.h"
// DSSStatisticsVVTOS
#include "DSSStatisticsGeneric.h"
// DSSStatisticsSVTOSSimple
#include "DSSStatisticsSVTOSSimple.h"
// DSSStatisticsSVVTSSimple
#include "DSSStatisticsSVVTOSSimple.h"

// zyu 06/22/2005
// ******** DEPRECATED *********
// The following arrangement is problematic. It basically rules out the possiblity to
// add new functions in each category. For example, you cannot add a new STOS function
// without renumbering the functions. This requires that the PDS files be changed 
// accordingly, which is a chore. 
#define VTOSOFFSET	(1)
#define VTOSNUM		(6)
#define STOSOFFSET	(VTOSOFFSET + VTOSNUM)		// 7
#define STOSNUM		(28)
#define VSTOSOFFSET (STOSOFFSET + STOSNUM)		// 35
#define VSTOSNUM	(2)
#define VVTOSOFFSET	(VSTOSOFFSET + VSTOSNUM)	// 37
#define VVTOSNUM	(12)
// The next new function should be number 49

// zyu 06/22/2005
// Here is what I am going to deal with it. I'll give a symbolic name for new
// functions starting from 49. Each new function is numbered sequentially
#include "DSSStatistics_include.h"

DSSStatisticalPackage::DSSStatisticalPackage()
{
}


DSSStatisticalPackage::~DSSStatisticalPackage()
{
}

DSSFunctionObject * DSSStatisticalPackage::Item(Int32 nIndex)
{
    DSSFunctionObject *lpFunctionObject;
	
	switch (nIndex)
	{
        case 0 + VTOSOFFSET:
        case 1 + VTOSOFFSET:
        case 2 + VTOSOFFSET:
        case 3 + VTOSOFFSET:
        case 4 + VTOSOFFSET:
        case 5 + VTOSOFFSET:
		{
            DSSStatisticsVTOS *pFunc = new DSSStatisticsVTOS();
            pFunc->Init(nIndex - VTOSOFFSET);
            lpFunctionObject = pFunc;
		}
            break;
        case 0 + STOSOFFSET:
        case 1 + STOSOFFSET:
        case 2 + STOSOFFSET:
        case 3 + STOSOFFSET:
        case 4 + STOSOFFSET:
            
        case 5 + STOSOFFSET:
        case 6 + STOSOFFSET:
        case 7 + STOSOFFSET:
        case 8 + STOSOFFSET:
        case 9 + STOSOFFSET:
            
        case 10 + STOSOFFSET:
        case 11 + STOSOFFSET:
        case 12 + STOSOFFSET:
        case 13 + STOSOFFSET:
        case 14 + STOSOFFSET:
            
        case 15 + STOSOFFSET:
        case 16 + STOSOFFSET:
        case 17 + STOSOFFSET:
        case 18 + STOSOFFSET:
        case 19 + STOSOFFSET:
            
        case 20 + STOSOFFSET:
        case 21 + STOSOFFSET:
        case 22 + STOSOFFSET:
        case 23 + STOSOFFSET:
        case 24 + STOSOFFSET:
            
        case 25 + STOSOFFSET:
        case 26 + STOSOFFSET:
        case 27 + STOSOFFSET:
		{
            DSSStatisticsSTOS *pFunc = new DSSStatisticsSTOS();
            pFunc->Init(nIndex - STOSOFFSET);
            lpFunctionObject = pFunc;
		}
            break;
            
        case 0 + VSTOSOFFSET:
        case 1 + VSTOSOFFSET:
		{
            DSSStatisticsVSTOS *pFunc = new DSSStatisticsVSTOS();
            pFunc->Init(nIndex - VSTOSOFFSET);
            lpFunctionObject = pFunc;
		}
            break;
            
        case 0 + VVTOSOFFSET:
        case 1 + VVTOSOFFSET:
        case 2 + VVTOSOFFSET:
        case 3 + VVTOSOFFSET:
        case 4 + VVTOSOFFSET:
            
        case 5 + VVTOSOFFSET:
        case 6 + VVTOSOFFSET:
        case 7 + VVTOSOFFSET:
        case 8 + VVTOSOFFSET:
        case 9 + VVTOSOFFSET:
            
        case 10 + VVTOSOFFSET:
        case 11 + VVTOSOFFSET:
		{
            DSSStatisticsGeneric *pFunc = new DSSStatisticsGeneric();
            pFunc->Init(nIndex - VVTOSOFFSET);
            lpFunctionObject = pFunc;
		}
            break;
            
            // zyu 06/23/2005
            // New functions starts here!!!
            // From this day on, we SHOULD give each new function a symbolic name
            // and add code as follows.
        case DssStatFuncLinearTrend_S:
        case DssStatFuncGrowth_S:
        case DssStatFuncForecast_S:
		{
            DSSStatisticsSTOS *pFunc = new DSSStatisticsSTOS();
            // Pass original index directly
            pFunc->Init(nIndex);
            lpFunctionObject = pFunc;
		}
            break;
            
        case DssStatFuncLinearTrend_V:
        case DssStatFuncGrowth_V:
		{
            DSSStatisticsSVTOSSimple *pFunc = new DSSStatisticsSVTOSSimple();
            // Pass original index directly
            pFunc->Init(nIndex);
            lpFunctionObject = pFunc;
		}
            break;
            
        case DssStatFuncForecast_V:
		{
            DSSStatisticsSVVTOSSimple *pFunc = new DSSStatisticsSVVTOSSimple();
            // Pass original index directly
            pFunc->Init(nIndex);
            lpFunctionObject = pFunc;
		}
            break;
            
        default :	lpFunctionObject = NULL;
	}
	return lpFunctionObject;
}

int DSSStatisticalPackage::Count(Int32 *pCount)
{
    if (pCount == NULL)
		return E_POINTER;
    
	// Always hard code it!
	*pCount = 54;
	return S_OK;
}

int DSSStatisticalPackage::getCLSID (GUID *opCLSID)
{
    return E_NOTIMPL;
}

int DSSStatisticalPackage::getDescription (char **opDescription)
{
    return E_NOTIMPL;
}

int DSSStatisticalPackage::getName (char **opName)
{
    return E_NOTIMPL;
}

int DSSStatisticalPackage::getVersion (char **opVersion)
{
    return E_NOTIMPL;
}


