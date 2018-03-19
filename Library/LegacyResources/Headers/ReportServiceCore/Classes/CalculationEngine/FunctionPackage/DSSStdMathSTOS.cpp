//
//  DSSStdMathSTOS.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 2/22/12.
//  Copyright 2012 MicroStrategy, Inc.. All rights reserved.
//

#include "DSSStdMathSTOS.h"
#include "mathPackImplement.h"
#include "DSSFunctionUtility.h"

DSSStdMathSTOS::DSSStdMathSTOS():mFunctionIndex(0)
{
}

DSSStdMathSTOS::~DSSStdMathSTOS()
{
}

int DSSStdMathSTOS::Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag)
{
    if(!ipParams || !opResult || !opFlag) return E_POINTER;
    
	enum MathError error = MathNoError;
	HRESULT hr = E_FAIL;
    
	try {
		switch (mFunctionIndex)	{
            case 0: // ABS
                if (nParam == 1)
                    hr = MSI_ABS(&error, opResult, ipParams[0]);
                
                break;
                
            case 1: //ACOS
                if (nParam == 1)
                    hr = MSI_ACOS(&error, opResult, ipParams[0]);
                break;
                
            case 2: //ACOSH
                if (nParam == 1)
                    hr = MSI_ACOSH(&error, opResult, ipParams[0]);
                break;
                
            case 3: //ASIN
                if (nParam == 1)
                    hr = MSI_ASIN(&error, opResult, ipParams[0]);
                break;
                
            case 4: //ATAN
                if (nParam == 1)
                    hr = MSI_ATAN(&error, opResult, ipParams[0]);
                break;
                
            case 5: //ATAN2
                if (nParam == 2)
                    hr = MSI_ATAN2(&error, opResult, ipParams[0], ipParams[1]);
                break;
                
            case 6: //ATANH
                if (nParam == 1)
                    hr = MSI_ATANH(&error, opResult, ipParams[0]);
                break;
                
            case 7: //CEILING
                if (nParam == 1)
                    hr = MSI_CEILING(&error, opResult, ipParams[0]);
                break;
                
            case 8:	//COMBIN
                if (nParam == 2)
                    hr = MSI_COMBIN(&error, opResult, ipParams[0], ipParams[1]);
                break;
                
            case 9: //COS
                if (nParam == 1)
                    hr = MSI_COS(&error, opResult, ipParams[0]);
                break;
                
            case 10: //COSH
                if (nParam == 1)
                    hr = MSI_COSH(&error, opResult, ipParams[0]);
                break;
                
                
            case 11: //DEGREES
                if (nParam == 1)
                    hr = MSI_DEGREES(&error, opResult, ipParams[0]);
                break;
                
            case 12: //EXP
                if (nParam == 1)
                    hr = MSI_EXP(&error, opResult, ipParams[0]);
                break;
                
            case 13: //FACTORIAL
                if (nParam == 1)
                    hr = MSI_FACTORIAL(&error, opResult, ipParams[0]);
                break;
                
            case 14: //FLOOR
                if (nParam == 1)
                    hr = MSI_FLOOR(&error, opResult, ipParams[0]);
                break;
                
            case 15: //INT
                if (nParam == 1)
                    hr = MSI_INT(&error, opResult, ipParams[0]);
                break;
                
            case 16: //LN
                if (nParam == 1)
                    hr = MSI_LN(&error, opResult, ipParams[0]);
                break;
                
            case 17: //LOG
                if (nParam == 1)
                    hr = MSI_LOG(&error, opResult, ipParams[0], 10);
                else if (nParam == 2)
                    hr = MSI_LOG(&error, opResult, ipParams[0], ipParams[1]);
                break;
                
            case 18: //LOG10
                if (nParam == 1)
                    hr = MSI_LOG(&error, opResult, ipParams[0], 10);
                break;
                
            case 19: //PI
                if (nParam == 0)
                    hr = MSI_GETPI(&error, opResult);
                break;
                
            case 20: //POWER
                if (nParam == 2)
                    hr = MSI_POWER(&error, opResult, ipParams[0], ipParams[1]);
                break;
				
                
            case 21: //QUOTIENT
                if (nParam == 2)
                    hr = MSI_QUOTIENT(&error, opResult, ipParams[0], ipParams[1]);
                break;
                
            case 22: //RADIANS
                if (nParam == 1)
                    hr = MSI_RADIANS(&error, opResult, ipParams[0]);
                break;
                
            case 23: //RAND
                if (nParam == 0)
                    hr = MSI_RAND(&error, opResult);
                break;
                
            case 24: //RANDBETWEEN
                if (nParam == 2)
                    hr = MSI_RANDBETWEEN(&error, opResult, ipParams[0], ipParams[1]);
                break;
                
            case 25: //ROUND
                if (nParam == 1)
                    hr = MSI_ROUND(&error, opResult, ipParams[0], 0);
                
                break;
                
            case 26: //SIN
                if (nParam == 1)
                    hr = MSI_SIN(&error, opResult, ipParams[0]);
                break;
                
            case 27: //SINH
                if (nParam == 1)
                    hr = MSI_SINH(&error, opResult, ipParams[0]);
                break;
                
            case 28: //SQRT
                if (nParam == 1)
                    hr = MSI_SQRT(&error, opResult, ipParams[0]);
                break;
                
            case 29: //TAN
                if (nParam == 1)
                    hr = MSI_TAN(&error, opResult, ipParams[0]);
                break;
                
            case 30: //TANH
                if (nParam == 1)
                    hr = MSI_TANH(&error, opResult, ipParams[0]);
                break;
                
            case 31: //TRUNC
                if (nParam == 1)
                    hr = MSI_TRUNC(&error, opResult, ipParams[0], 0);
                else if (nParam == 2)
                    hr = MSI_TRUNC(&error, opResult, ipParams[0], ipParams[1]);
                
                break;
                
            case 32: //ASINH
                if (nParam == 1)
                    hr = MSI_ASINH(&error, opResult, ipParams[0]);
                break;
                
            case 33: //MOD
                if (nParam == 2)
                    hr = MSI_MOD(&error, opResult, ipParams[0], ipParams[1]);
                break;
                
            case 34: //ROUND2
                if (nParam == 2)
                    hr = MSI_ROUND(&error, opResult, ipParams[0], ipParams[1]);
                break;
                
            default:
                //TRACE info
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

//int DSSStdMathSTOS::Init(int iIndex)
//{
//    mFunctionIndex = iIndex;
//    return S_OK;
//}
