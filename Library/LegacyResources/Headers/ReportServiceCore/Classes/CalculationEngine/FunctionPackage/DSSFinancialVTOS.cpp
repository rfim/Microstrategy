//
//  DSSFinancialVTOS.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 2/22/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#include "DSSFinancialVTOS.h"
#include "DSSFunctionUtility.h"
#include "financialFuncImplement.h"

DSSFinancialVTOS::DSSFinancialVTOS():mFunctionIndex(0),mProperty(0),mMethod(0),mPrecision(3.6E-8)
{}

DSSFinancialVTOS::~DSSFinancialVTOS() {}


int DSSFinancialVTOS::Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag)
{
    _ASSERT(mFunctionIndex == 45);
    
	if(nParam < 1) return E_INVALIDARG;
	if(!ipParams || !opResult || !opFlag) return E_POINTER;
    
	enum MathError error = MathNoError;
	HRESULT hr = S_OK;
    
	try {
		*opResult = _IRR(&error, nParam, ipParams, mProperty, mMethod, mPrecision);
	}catch (...) {
		hr = E_FAIL;
	}
    
    
	if(error == MathNoError)
		*opFlag = DssDataOk;
	else
		*opFlag = DssDataInvalid;
    
	return hr;
}

// int CalculateRepeat(int nParam, int nRepeat, double **ipParams, double *opResult, DSSDataFlag *opFlag);

int DSSFinancialVTOS::setProperty (Int32 iIndex, CComVariant *ipValue)
{
    switch (iIndex) {
        case 1:	// Guess - Default: 0.1
        {
            HRESULT hr = VariantChangeType(ipValue, ipValue, 0, VT_R8);
            if ( hr != S_OK ) return hr;
            mProperty = ipValue->dblVal;	
        }
            break;
        case 2:	// Method - 0: Secant, 1: Newton, Default: 0
        {
            HRESULT hr = VariantChangeType(ipValue, ipValue, 0, VT_I4);
            if ( hr != S_OK ) return hr;
            mMethod = ipValue->intVal;	
        }
            break;
        case 3:	// Precision - Default: 0.00001
        {
            HRESULT hr = VariantChangeType(ipValue, ipValue, 0, VT_R8);
            if ( hr != S_OK ) return hr;
            mPrecision = ipValue->dblVal;
        }
            break;
        default:
            break;
    }		
    
    return S_OK;
}

int DSSFinancialVTOS::Init(int iIndex)
{
    _ASSERT(iIndex == 45);
	mFunctionIndex = iIndex;
    
	//set default property value for some function other than 0
	mProperty = 0.1; //guess
    
	return S_OK;
}
