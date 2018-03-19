//
//  DSSNNSimpleFunction.cpp
//  ReportServiceCore
//
//  Created by Zhang Jia on 7/22/11.
//  Copyright 2011 MicroStrategy Beijing R&D Center. All rights reserved.
//

#include "DSSNNSimpleFunction.h"

DSSNNSimpleFunction::DSSNNSimpleFunction()
{}

DSSNNSimpleFunction::~DSSNNSimpleFunction()
{}

int DSSNNSimpleFunction::CalculateRepeat(int nParam, int nRepeat, CComVariant *ipParams, CComVariant *opResult, DSSDataFlag *opFlag)
{
	if(!ipParams || !opResult || !opFlag) return E_POINTER;
    
	HRESULT hr = E_FAIL;
    
	// suppose ipParams is a two-dimensional array of Variants[nRepeat, nParam]
	if ( ((ipParams->vt & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT)) ||
        SafeArrayGetDim(ipParams->parray) != 2)
		return E_FAIL;
    
	LONG bound1, bound2;
	hr = SafeArrayGetUBound(ipParams->parray, 1, &bound1);
	if(hr != S_OK || bound1 < nRepeat - 1) //nRepeat can be less than bound1
		return E_FAIL;
    
	hr = SafeArrayGetUBound(ipParams->parray, 2, &bound2);
	if(hr != S_OK || bound2 != nParam - 1)
		return E_FAIL;
    
	//result should be a one-dimensional array of Variants[nRepeat]
	opResult->vt = VT_ARRAY|VT_VARIANT;
	SAFEARRAYBOUND lSABound;
	lSABound.lLbound = 0;
	lSABound.cElements = nRepeat;
	opResult->parray = SafeArrayCreate(VT_VARIANT, 1, &lSABound);
    
	LONG i;
	LONG indice[2]; 
	for(i = 0; i < nRepeat; i++)
	{
		CComVariant input, output;
        
		//make a input parameter array[nParam]
		SAFEARRAYBOUND linputSABound;
		linputSABound.lLbound = 0;
		linputSABound.cElements = nParam;
		input.vt = VT_ARRAY|VT_VARIANT;
		input.parray = SafeArrayCreate(VT_VARIANT, 1, &linputSABound);
        
		LONG j;
		indice[0] = i;
		for (j = 0; j < nParam; j++){
			CComVariant element;
			indice[1] = j;
			hr = SafeArrayGetElement(ipParams->parray, indice, &element);
			if (hr != S_OK)	return hr;
            
			hr = SafeArrayPutElement(input.parray, &j, &element);
			if (hr != S_OK)	return hr;
		}
        
		hr = Calculate(nParam, &input, &output, opFlag+i);
		if (hr != S_OK)	return hr; 
        
		hr = SafeArrayPutElement(opResult->parray, &i, &output);
		if (hr != S_OK)	return hr;        
	}
    
	return S_OK;
}