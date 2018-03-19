//
//  DSSBandingM.cpp
//  ReportServiceCore
//
//  Created by Yang Zhuo on 7/27/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//

#include "DSSBandingM.h"
#import "VariantCommonInclude.h"
#import "DSSFunctionUtility.h"

#define UNINITIALIZED -1

DSSBandingM::DSSBandingM()
: mDataSize(UNINITIALIZED), mHasResidue(false),mpDataPtr(NULL),mpDataFlagPtr(NULL)
{
    // 2/21/2012 tehe we also need initialize mpDataPtr and mpDataFlagPtr
}


DSSBandingM::~DSSBandingM()
{
    Clear();
}


int DSSBandingM::setProperty (Int32 Index, CComVariant *pValue)
{
	if(!pValue) return E_POINTER;
	int hr;
    
	// convert to variant bool type
	hr = VariantChangeType(pValue, pValue, 0, VT_BOOL);
	if ( FAILED(hr) ) 
		return E_FAIL;
    
	mHasResidue = pValue->boolVal ? true : false;
    
	return S_OK;
}

int DSSBandingM::Calculate()
{
 	Int32 i = 0;
 	if(mDataSize == UNINITIALIZED || mFunctionIndex!=DssFuncBandingM)
    {
        return E_FAIL;
    }
    
	// check whether all the required input parameters have been set.
    
	/*if(!mpDataPtr.Get() || !mpDataFlagPtr.Get())
		return ReportCOMErrorNP(CLSID_DSSFunctionServer,
                                IID_ICDSSGenericFunction,
                                FUNC_E_INTERNAL);
    */
    
    if(!mpDataPtr || !mpDataFlagPtr)
    {
        return E_FAIL;
    }
     
	// Allocate space for mpResultPtr and mpFlag
	/*mpResultPtr.Reset(new double[mDataSize]);  
	if(mpResultPtr.IsNull()) return E_OUTOFMEMORY;
    
	mpResultFlagPtr.Reset(new DSSData_Flags[mDataSize]);
	if(mpResultFlagPtr.IsNull()) return E_OUTOFMEMORY;
    */
    mpResultPtr = new double[mDataSize];
    if(!mpResultPtr)
    {
        return E_OUTOFMEMORY;
    }
    mpResultFlagPtr = new DSSDataFlag[mDataSize];
    if(!mpResultFlagPtr)
    {
        return E_OUTOFMEMORY;
    }
    
	for (i = 0; i < mDataSize; i++)
	{
		if (mpDataFlagPtr[i] == DssDataOk)
		{
			mpResultPtr[i] = mpDataPtr[i];
			mpResultFlagPtr[i] = DssDataOk;
		}
		else
			mpResultFlagPtr[i] = DssDataUnknown;
	}
    
	return S_OK;
}

int DSSBandingM::SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSDataFlag *pFlag)
{
	if(nParamIndex != 1) return E_INVALIDARG;
    
	if(nSize < 1) return E_INVALIDARG;
	
	if(!pData) return E_POINTER;
    
	mDataSize = nSize;
	
    // allocat memory to record input data
	/*mpDataPtr.Reset(new double[nSize]);
	if(mpDataPtr.IsNull()) return E_OUTOFMEMORY;
	memmove(mpDataPtr.Get(), pData, nSize * sizeof(double));
    
	mpDataFlagPtr.Reset(new DSSData_Flags[nSize]);
	if(mpDataFlagPtr.IsNull()) return E_OUTOFMEMORY;
	memmove(mpDataFlagPtr.Get(), pFlag, nSize * sizeof(DSSData_Flags));
    */
    if(mpDataPtr)
    {
        delete []mpDataPtr;
    }
    mpDataPtr = new double[nSize];
    if(!mpDataPtr)
    {
        return E_OUTOFMEMORY;
    }
    memmove(mpDataPtr, pData, nSize * sizeof(double));
    
    if(mpDataFlagPtr)
    {
        delete []mpDataFlagPtr;
    }
    mpDataFlagPtr = new DSSDataFlag[nSize];
    if(!mpDataFlagPtr)
    {
        return E_OUTOFMEMORY;
    }
    memmove(mpDataFlagPtr, pFlag, nSize * sizeof(DSSDataFlag));
    
	return S_OK;
}

int DSSBandingM::GetOutput (Int32 nSize, Int32 *nDataSize, double *pData, DSSDataFlag *pFlag)
{
 	if(!nDataSize || !pData || !pFlag) return E_POINTER;
    
	// output buffer size shouldn't be smaller than mDataSize
	if(nSize < mDataSize) 
    {
        return E_FAIL;
    }
    
	// copy the result out
	Int32 i = 0;
	for(i = 0; i < mDataSize; i++)
	{
		pData[i] = mpResultPtr[i];
		pFlag[i] = mpResultFlagPtr[i];
	}
    
	*nDataSize = mDataSize;
    
	return S_OK;
}

int DSSBandingM::GetOutputType (EnumDSSParameterType *pType)
{
  	if(!pType) return E_POINTER;
    
	*pType = DssParameterVector;
	
	return S_OK;
}

int DSSBandingM::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
	if(!pType) return E_POINTER;
    
	if (nParamIndex != 1)
		return E_FAIL;
    
	*pType = DssParameterVector;
    
	return S_OK;
}

int DSSBandingM::IsNullProcessable (bool *pCanProcessNULL)
{
	if(!pCanProcessNULL) return E_POINTER;
	
	*pCanProcessNULL = FALSE; // always assume input data is valid
    
	return S_OK;
}

int DSSBandingM::Clear ()
{
	mDataSize = UNINITIALIZED;
    if(mpDataPtr)
    {
        delete []mpDataPtr;
        mpDataPtr = NULL;
    }
    if(mpDataFlagPtr)
    {
        delete []mpDataFlagPtr;
        mpDataFlagPtr = NULL;
    }
    if(mpResultPtr)
    {
        delete []mpResultPtr;
        mpResultPtr = NULL;
    }
    if(mpResultFlagPtr)
    {
        delete []mpResultFlagPtr;
        mpResultFlagPtr = NULL;
    }
	return S_OK;
}

//int DSSBandingM::Init (EnumSimpleOperatorType iFunctionIndex)
//{
//	mFunctionIndex = iFunctionIndex;
//	return S_OK;
//}

int DSSBandingM::DirectCalculate (Int32 nParam, Int32* ipSize, 
                                  void** ipData, double* ipDivider, DSSDataFlag** ipFlags,
                                  bool* ipAllDataValid, EnumDSSDataType* iType, vector<Int32> * ipvDataByGroup,
                                  Int32 nResSize, double* opData, DSSDataFlag* opFlag, bool* opAllDataValid,
                                  EnumNullCheckingOption iNullCheckingOption)
{
	Int32 i = 0;
 	AE_ASSERT(nParam == 1);
	Int32 lnSize = ipSize[0];
	if (ipvDataByGroup != NULL)
		lnSize = ipvDataByGroup->size();
    
	DSSDataFlag* lFlags = ipFlags[0];
	void* lpData = ipData[0];
	EnumDSSDataType lType = iType[0];
    
	Int32 lRow = 0;
	for (i = 0; i < lnSize; i++)
	{
		if (ipvDataByGroup != NULL)
			lRow = (*ipvDataByGroup)[i];
		else
			lRow = i;
        
		if (lFlags[lRow] == DssDataOk)
		{
			opData[lRow] = GetNumericData(lpData, lRow, lType);
			opFlag[lRow] = DssDataOk;
		}
		else if (iNullCheckingOption == DssNumericErrorTreatAs)
		{
			opData[lRow] = 0;
			opFlag[lRow] = DssDataOk;
		}
		else
			opFlag[lRow] = DssDataUnknown;
	}
    
	*opAllDataValid = VARIANT_TRUE;
    
	return S_OK;
}

bool DSSBandingM::SupportDirectCalculate()
{
	return true;
}
