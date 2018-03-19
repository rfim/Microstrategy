//
//  DSSIndexFunction.cpp
//  ReportServiceCore
//
//  Created by Jia Zhang on 7/27/11.
//  Copyright 2011 MicroStrategy, Inc. All rights reserved.
//

#include "DSSIndexFunction.h"

DSSIndexFunction::DSSIndexFunction() : mV(NULL), mVSize(0), mS(0.0), mSFlag(false), mVFlag(NULL)
{
}

DSSIndexFunction::~DSSIndexFunction()
{
	Clear();
}

int DSSIndexFunction::GetOutputType (EnumDSSParameterType *pType)
{
	CHECK_PTR(pType);
	*pType = DssParameterScalar;
	return S_OK;
}

int DSSIndexFunction::SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSDataFlag *pFlag)
{
    int hr = S_OK;
    
	CHECK_PTR(pData);
	CHECK_PTR(pFlag);
        
	switch (nParamIndex)
	{
        case 1:
		{
			if (nSize < 1) return E_FAIL;
			if (mV != NULL) delete [] mV;
            
			mVSize = nSize;
			mV = new double[nSize];
            
			if (mVFlag != NULL) delete [] mVFlag;
			mVFlag = new DSSDataFlag[nSize];
            
			for (Int32 i=0; i<nSize; i++)
			{
				mV[i] = pData[i];
				mVFlag[i] = pFlag[i];
			}
		}
            break;
        case 2:
            if (nSize != 1) return E_FAIL;
            mS = *pData;
            mSFlag = *pFlag;
            break;
        default: 
            return hr = E_FAIL;
            // AE_MSG; YMA; Invalid Argument
	}
    
	return hr = S_OK;
}

int DSSIndexFunction::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
    int hr = S_OK;
    
	CHECK_PTR(pType);
	switch (nParamIndex)
	{
        case 1:
            *pType = DssParameterVector;
            break;
        case 2:
            *pType = DssParameterScalar;
            break;
        default: 
            return hr = E_FAIL;
            // AE_MSG; YMA; Invalid Argument
	}
    
	return hr = S_OK;
}

int DSSIndexFunction::Clear ()
{
	if (mV != NULL) delete [] mV;
	if (mVFlag != NULL) delete [] mVFlag;
    
	// YMA 1/19/00 
	// Need to ground the pointers, otherwise next time when set the input parameters
	// there will be AV.
	mV = NULL;
	mVFlag = NULL;
    
	return S_OK;
}