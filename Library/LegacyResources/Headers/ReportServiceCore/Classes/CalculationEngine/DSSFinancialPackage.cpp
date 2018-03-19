//
//  DSSFinancialPackage.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 2/22/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#include "DSSFinancialPackage.h"

#include "DSSFinancialSTOS.h"
#include "DSSFinancialVSSTOS.h"
#include "DSSFinancialVSTOS.h"
#include "DSSFinancialVTOS.h"
#include "DSSNNFinancialGenericFunction.h"
#include "DSSNNFinancialSTOS.h"

#define NNSTOSBASE		19	//start index of Non-numerical function
#define VTOSBASE		45	//start index of Vector to Scalar function
#define VSTOSBASE		46	//start index of VSToS function
#define VVTOSBASE		48

#define MIRRINDEX		49
#define XIRRINDEX		48
#define XNPVINDEX		50


DSSFinancialPackage::DSSFinancialPackage()
{
}

DSSFinancialPackage::~DSSFinancialPackage()
{
}


int DSSFinancialPackage::Count (int *opCount)
{
    // tehe here we hardcode the functioncount value to 50,same as server side
    const Int32 FunctionCount = 50;
    
	if (opCount == NULL)
		return E_POINTER;
    
	*opCount = FunctionCount;
	
	return S_OK;
}

int DSSFinancialPackage::getCLSID (GUID *opCLSID)
{
    return E_NOTIMPL;
}

int DSSFinancialPackage::getDescription (char **opDescription)
{
    return E_NOTIMPL;
}

int DSSFinancialPackage::getName (char **opName)
{
    return E_NOTIMPL;
}

int DSSFinancialPackage::getVersion (char **opVersion)
{
    return E_NOTIMPL;
}

DSSFunctionObject* DSSFinancialPackage::Item(int iIndex)
{
    DSSFunctionObject *lpFunctionObject = NULL;
	Int32 nItemCount = 0;
	int hr = Count(&nItemCount);
    if(hr != S_OK)
        return NULL;
    
	if (iIndex < 1 || iIndex > nItemCount)
		return NULL;
    
	if (iIndex < NNSTOSBASE) {// numerical STOS function
		DSSFinancialSTOS *pFunc = new DSSFinancialSTOS();
		pFunc->Init(iIndex);
		lpFunctionObject = pFunc;
	} else if (iIndex < VTOSBASE) { // Non numrical STOS Function
		DSSNNFinancialSTOS *pFunc = new DSSNNFinancialSTOS();
		pFunc->Init(iIndex);
		lpFunctionObject = pFunc;
	} else if (iIndex < VSTOSBASE){ //numerical VTOS function ie IRR
		DSSFinancialVTOS *pFunc = new DSSFinancialVTOS();
		pFunc->Init(iIndex);
		lpFunctionObject = pFunc;
	} else if (iIndex < VVTOSBASE){ // numerical VSTOS function
		DSSFinancialVSTOS *pFunc = new DSSFinancialVSTOS();
		pFunc->Init(iIndex);
		lpFunctionObject = pFunc;
        
	} else if (iIndex == MIRRINDEX) { //mirr function VSSTOS
		DSSFinancialVSSTOS *pFunc = new DSSFinancialVSSTOS();
		pFunc->Init(iIndex);
		lpFunctionObject = pFunc;
		
	} else if (iIndex == XIRRINDEX || iIndex == XNPVINDEX) { //Xirr and xnpv
		DSSNNFinancialGenericFunction *pFunc = new DSSNNFinancialGenericFunction();
		pFunc->Init(iIndex);
		lpFunctionObject = pFunc;
	} 
    
	return lpFunctionObject;
}
