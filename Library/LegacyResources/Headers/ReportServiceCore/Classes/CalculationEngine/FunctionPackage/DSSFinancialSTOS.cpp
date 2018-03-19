//
//  DSSFinancialSTOS.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 2/22/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#include "DSSFinancialSTOS.h"
#include "DSSFunctionUtility.h"
#include "financialFuncImplement.h"

DSSFinancialSTOS::DSSFinancialSTOS():mFunctionIndex(0)
{
    memset(mProperties, 0, sizeof(double) * 3);
}

DSSFinancialSTOS::~DSSFinancialSTOS()
{
}

int DSSFinancialSTOS::Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag)
{
 	if(!ipParams || !opResult || !opFlag) return E_POINTER;
    
	enum MathError error = MathNoError;
	HRESULT hr = S_OK;
    
	try {
		switch (mFunctionIndex)	{
            case 1: //CUMIPMT
                if (nParam == 5)
                    *opResult = _CUMIPMT(&error, ipParams[0], ipParams[1], ipParams[2], ipParams[3], ipParams[4], mProperties[0]);
                else
                    hr = E_FAIL;
                break;
                
            case 2: //CUMPRINC
                if (nParam == 5)
                    *opResult = _CUMPRINC(&error, ipParams[0], ipParams[1], ipParams[2], ipParams[3], ipParams[4], mProperties[0]);
                else
                    hr = E_FAIL;
                break;
                
            case 3: //DB
                if (nParam == 4)
                    *opResult = _DB(&error, ipParams[0], ipParams[1], ipParams[2], ipParams[3], mProperties[0]);
                else
                    hr = E_FAIL;
                break;
                
            case 4: //DDB
                if (nParam == 4)
                    *opResult = _DDB(&error, ipParams[0], ipParams[1], ipParams[2], ipParams[3], mProperties[0]);
                else
                    hr = E_FAIL;
                break;
                
            case 5: //DOLLARDE
                if (nParam == 2)
                    *opResult = _DOLLARDE(&error, ipParams[0], ipParams[1]);
                else
                    hr = E_FAIL;
                break;
                
            case 6: //DOLLARFR
                if (nParam == 2)
                    *opResult = _DOLLARFR(&error, ipParams[0], ipParams[1]);
                else
                    hr = E_FAIL;
                break;
                
            case 7: //EFFECT
                if (nParam == 2)
                    *opResult = _EFFECT(&error, ipParams[0], ipParams[1]);
                else
                    hr = E_FAIL;
                break;
                
            case 8:	//FV
                if (nParam == 4)
                    *opResult = _FV(&error, ipParams[0], ipParams[1], ipParams[2], ipParams[3], mProperties[0]);
                else
                    hr = E_FAIL;
                break;
                
            case 9: //IPMT
                if (nParam == 4)
                    *opResult = _IPMT(&error, ipParams[0], ipParams[1], ipParams[2], ipParams[3], mProperties[0], (Int32)mProperties[1]);
                else
                    hr = E_FAIL;
                break;
                
            case 10: //NOMINAL
                if (nParam == 2)
                    *opResult = _NOMINAL(&error, ipParams[0], ipParams[1]);
                else
                    hr = E_FAIL;
                break;
                
            case 11: //NPER
                if (nParam == 4)
                    *opResult = _NPER(&error, ipParams[0], ipParams[1], ipParams[2], ipParams[3], (Int32)mProperties[0]);
                else
                    hr = E_FAIL;
                break;
                
            case 12: //PMT
                if (nParam == 3)
                    *opResult = _PMT(&error, ipParams[0], ipParams[1], ipParams[2], mProperties[0], (Int32)mProperties[1]);
                else
                    hr = E_FAIL;
                break;
                
            case 13: //PPMT
                if (nParam == 4)
                    *opResult = _PPMT(&error, ipParams[0], ipParams[1], ipParams[2], ipParams[3], mProperties[0], (Int32)mProperties[1]);
                else
                    hr = E_FAIL;
                break;
                
            case 14: //PV
                if (nParam == 4)
                    *opResult = _PV(&error, ipParams[0], ipParams[1], ipParams[2], ipParams[3], (Int32)mProperties[0]);
                else
                    hr = E_FAIL;
                break;
                
            case 15: //RATE
                if (nParam == 3)
                    *opResult = _RATE(&error, ipParams[0], ipParams[1], ipParams[2], mProperties[0], (Int32)mProperties[1], mProperties[2]);
                else
                    hr = E_FAIL;
                break;
                
            case 16: //SLN
                if (nParam == 3)
                    *opResult = _SLN(&error, ipParams[0], ipParams[1], ipParams[2]);
                else
                    hr = E_FAIL;
                break;
                
                
            case 17: //SYD
                if (nParam == 4)
                    *opResult = _SYD(&error, ipParams[0], ipParams[1], ipParams[2], ipParams[3]);
                else
                    hr = E_FAIL;
                break;
                
            case 18: //VDB
                if (nParam == 4)
                    *opResult = _VDB(&error, ipParams[0], ipParams[1], ipParams[2], ipParams[3], mProperties[0]);
                else
                    hr = E_FAIL;
                break;
                
            default:
                //TRACE info
                _ASSERT(FALSE);
                hr = E_FAIL;
                break;
                
		}
	} catch (...) {
		hr = E_FAIL;
	}
    
	if (hr == S_OK) {
		if(error == MathNoError)
			*opFlag = DssDataOk;
		else
			*opFlag = DssDataInvalid;
	}
    
	return hr;
}

int DSSFinancialSTOS::setProperty(int iIndex, CComVariant *ipValue)
{
    int hr = VariantChangeType(ipValue, ipValue, 0, VT_R8);
    if ( hr != S_OK ) return hr;
    
    mProperties[iIndex - 1] = ipValue->dblVal;
    
    return S_OK;
}

int DSSFinancialSTOS::Init(int iIndex)
{
    mFunctionIndex = iIndex;
    
	//set default property value for some function other than 0
	switch (iIndex){
        case 3: //DB()
            mProperties[0] = 12; // set month 12
            break;
        case 4: //DDB()
            mProperties[0] = 2;	 // set factor 2
            break;
        case 15: //RATE()
            mProperties[2] = 0.1;	// set guess 0.1
            break;
        case 18: //VDB()
            mProperties[0] = 2;		// set factor 2
            break;
        default:
            break;
	}
    
	return S_OK;
}
