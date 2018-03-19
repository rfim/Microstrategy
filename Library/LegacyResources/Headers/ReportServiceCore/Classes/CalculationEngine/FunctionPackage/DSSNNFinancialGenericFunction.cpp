//
//  DSSNNFinancialGenericFunction.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 2/22/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#include "DSSNNFinancialGenericFunction.h"
#include "DSSDateTime.h"
#include "DSSFunctionUtility.h"
#include "DFCDataConversion.h"
#include "financialFuncImplement.h"

DSSNNFinancialGenericFunction::DSSNNFinancialGenericFunction() : mFunctionIndex(0), mGuess(0.1), mpValues(NULL), mpDates(NULL),
mpValid(NULL),
mnValueSize(0), mnDateSize(0), mbRateReady(false), mfResult(0),
mFlag(DssDataInvalid), mRate(0)
{
}

DSSNNFinancialGenericFunction::~DSSNNFinancialGenericFunction()
{
    if (mpValues) {
        delete [] mpValues;
        mpValues = NULL;
    }
    
    if (mpDates) {
        delete [] mpDates;
        mpDates = NULL;
    }
    
    if(mpValid)
    {
        delete [] mpValid;
        mpValid = NULL;
    }
}


int DSSNNFinancialGenericFunction::GetOutput(Int32 nSize, 	// Input buffer size
              Int32 *nDataSize, 	// return data size
              CComVariant *pData, 	// Double array stores the result of function
              DSSDataFlag *pFlag)	// Output flags);    
{
    if(!nDataSize || !pData || !pFlag) return E_POINTER;
    
	if (nSize < 1) 
		return E_FAIL;
    
	*pFlag = mFlag;
	*nDataSize = 1;
    
	if(mFlag == DssDataOk) {
		// return value type is double
        DFCDataConversion::ConvertToVariant((BYTE *)(&mfResult), DssDataTypeDouble, 0, pData);
	}
    
	return S_OK;
}

int DSSNNFinancialGenericFunction::GetOutputType(Int32 nSize, EnumDSSDataType *pInputType, 
                  EnumDSSDataType *pOutputDataType, EnumDSSParameterType *pOutParamType)
{
    if(!pOutputDataType || !pOutParamType) 
		return E_POINTER;
    
	*pOutputDataType = DssDataTypeDouble;
	*pOutParamType = DssParameterScalar;
	
	return S_OK;
}

int DSSNNFinancialGenericFunction::SetInputParam(Int32 nParamIndex, 	// The index of input parameter
                  Int32 nSize, 	// Size of variant, if size >1, then pData is an array of VARIANTs
                  EnumDSSDataType dataType,
                  CComVariant *pData,
                  DSSDataFlag *pFlag)
{
    if((nParamIndex < 1) || 
	   (nParamIndex > 2 && mFunctionIndex == 48) ||
	   (nParamIndex > 3 && mFunctionIndex == 50)) 
        return E_INVALIDARG;
    
	if(nSize < 1) return E_INVALIDARG;
	
	if(!pData) return E_POINTER;
    
	Int32 sizeDate =sizeof(MDataType::DSSDateTime);
	Int32 sizeDouble = sizeof(double);
	Int32 i = 0;
	HRESULT hr = S_OK;
    
	if (mFunctionIndex == 48) { //XIRR
		if ( (pData->vt & (VT_ARRAY | VT_VARIANT) ) != (VT_ARRAY | VT_VARIANT) )
			return E_FAIL;
        
		switch (nParamIndex){
            case 1: //values
                if (!dataTypeAssignmentCompatible(dataType, DssDataTypeDouble)) 
                    return E_FAIL;
                
                if(mpValues == NULL)	{
                    
                    if(mpValid==NULL)
                    {
                        mpValid=new Int32[nSize];
                        if(!mpValid)
                            return  E_OUTOFMEMORY;
                        for(i=0;i<nSize;i++)
                        {
                             mpValid[i]=0;
                        }
                    }
                    
                    mnValueSize = nSize;
                    mpValues = new double[nSize];   // allocat memory to record input data
                    if(!mpValues) return E_OUTOFMEMORY;
                    
                    for(i = 0; i < nSize; i++) {
                        if(pFlag[i]==DssDataOk)
                        {
                            hr = getParamFromVariant(pData, i, DssDataTypeDouble, (BYTE *)&mpValues[i], sizeDouble);
                            
                            if(hr != S_OK)
                                return E_FAIL;
                            
                            mpValid[i]++;
                        }
                    }
                } else
                    _ASSERT(false);
                
                break;
                
            case 2: //dates
                if (!dataTypeAssignmentCompatible(dataType, DssDataTypeDate)) 
                    return E_FAIL;
                
                if(mpDates == NULL)	{
                    
                    if(mpValid==NULL)
                    {
                        mpValid=new Int32[nSize];
                        if(!mpValid)
                            return  E_OUTOFMEMORY;
                        for(i=0;i<nSize;i++)
                        {
                            mpValid[i]=0;
                        }
                    }
                    
                    mnDateSize = nSize;
                    mpDates = new MDataType::DSSDateTime[nSize];    // allocat memory to record input data
                    if(!mpDates) return E_OUTOFMEMORY;
                    
                    for(i = 0; i < nSize; i++) {
                        if(pFlag[i]==DssDataOk)
                        {
                            hr = getParamFromVariant(pData, i, DssDataTypeDate, (BYTE *)&mpDates[i], sizeDate);
                            
                            if(hr != S_OK)
                                return E_FAIL;
                            
                            mpValid[i]++;
                        }
                       
                    }
                } else
                    _ASSERT(false);
                
                break;
                
            default:
				_ASSERT(false);
		} //switch
        
	} else if (mFunctionIndex == 50) {//XNPV
		switch (nParamIndex){
            case 1: //values
                if (!dataTypeAssignmentCompatible(dataType, DssDataTypeDouble)) 
                    return E_FAIL;
                
                if ( (pData->vt & (VT_ARRAY | VT_VARIANT) ) != (VT_ARRAY | VT_VARIANT) )
                    return E_FAIL;
                
                if(mpValues == NULL){
                    
                    if(mpValid==NULL)
                    {
                        mpValid=new Int32[nSize];
                        if(!mpValid)
                            return  E_OUTOFMEMORY;
                        for(i=0;i<nSize;i++)
                        {
                            mpValid[i]=0;
                        }
                    }
                    
                    mnValueSize = nSize;
                    mpValues = new double[nSize];  // allocat memory to record input data
                    if(!mpValues) return E_OUTOFMEMORY;
                    
                    for(i = 0; i < nSize; i++) {
                        if(pFlag[i]==DssDataOk)
                        {
                            hr = getParamFromVariant(pData, i, DssDataTypeDouble, (BYTE *)&mpValues[i], sizeDouble);
                            if(hr != S_OK)
                                return E_FAIL;
                            
                            mpValid[i]++;
                        }
                        
                    }
                } else
                    _ASSERT(false);
                
                break;
                
            case 2: //dates
                if (!dataTypeAssignmentCompatible(dataType, DssDataTypeDate)) 
                    return E_FAIL;
                
                if ( (pData->vt & (VT_ARRAY | VT_VARIANT) ) != (VT_ARRAY | VT_VARIANT) )
                    return E_FAIL;
                
                if(mpDates == NULL)	{
                    
                    if(mpValid==NULL)
                    {
                        mpValid=new Int32[nSize];
                        if(!mpValid)
                            return  E_OUTOFMEMORY;
                        for(i=0;i<nSize;i++)
                        {
                            mpValid[i]=0;
                        }
                    }
                    
                    mnDateSize = nSize;
                    mpDates = new MDataType::DSSDateTime[nSize]; // allocat memory to record input data
                    if(!mpDates) return E_OUTOFMEMORY;
                    
                    for(i = 0; i < nSize; i++) {
                        if(pFlag[i]==DssDataOk)
                        {
                            hr = getParamFromVariant(pData, i, DssDataTypeDate, (BYTE *)&mpDates[i], sizeDate);
                            if(hr != S_OK)
                                return E_FAIL;
                            mpValid[i]++;
                        }
                       
                    }
                } else
                    _ASSERT(false);
                
                break;
                
            case 3: // rate
                if (!dataTypeAssignmentCompatible(dataType, DssDataTypeDouble) || nSize != 1) 
                    return E_FAIL;
                
                if(pFlag[0]!=DssDataOk)
                {
                    return E_FAIL;
                }
                
                if(!mbRateReady){
                    Int32 size = sizeDouble;
                    hr = DFCDataConversion::ConvertFromVariant(pData, DssDataTypeDouble, (BYTE *)&mRate, &size);
                    if(hr != S_OK) 
                        return E_FAIL;
                    mbRateReady = true;
                } else 
                    _ASSERT(false);
                
                break;
                
            default:
				_ASSERT(false);
		} //switch
        
	} else
		return E_FAIL;
    
	return S_OK;
}

int DSSNNFinancialGenericFunction::Calculate()
{
    enum MathError error = MathNoError;
	HRESULT hr = S_OK;
    
	try {
		if (mFunctionIndex == 48){ //XIRR
			if(!mpValues || !mpDates || mnValueSize != mnDateSize) 
				return E_FAIL;
            
			mfResult = _XIRR(&error, mnValueSize, mpValues, mpDates, mGuess, mpValid);
            
		} else if (mFunctionIndex == 50){ //XNPV
			if(!mpValues || !mpDates || !mbRateReady || mnValueSize != mnDateSize) 
				return E_FAIL;
            
			mfResult = _XNPV(&error, mRate, mnValueSize, mpValues, mpDates, mpValid);
            
		} else
			return E_FAIL;
        
	} catch (...) {
		hr = E_FAIL;
	}
    
	if(error == MathNoError)
		mFlag = DssDataOk;
	else
		mFlag = DssDataInvalid;
    
	return hr;
}

int DSSNNFinancialGenericFunction::Init(Int32 iFunctionIndex)
{
    if (iFunctionIndex != 48 && iFunctionIndex != 50)
		return E_FAIL;
    
	mFunctionIndex = iFunctionIndex;
    
	if (mFunctionIndex == 48) //XIRR
		mGuess = 0.1;
    
	return S_OK;
}

int DSSNNFinancialGenericFunction::setProperty (Int32 Index, CComVariant *pValue)
{
    if (mFunctionIndex != 48)
		return E_NOTIMPL;
    
	HRESULT hr=S_OK;
    
    if(pValue==NULL)
    {
        return  E_POINTER;
    }
    
    switch (Index) {
        case 1: //guess
        {
            hr = VariantChangeType(pValue, pValue, 0, VT_R8);
            if(FAILED(hr))
            {
                return hr;
            }
            
            if(pValue->vt==VT_R8)
            {
                mGuess=pValue->dblVal;
            }
            else
            {
                return  E_FAIL;
            }
            break;
        }
            
        default:
            return  E_FAIL; //invalid property index
    }
    
	return S_OK;
}

int DSSNNFinancialGenericFunction::Clear()
{
    if(mpValues)
    {
        delete [] mpValues;
        mpValues = NULL;
    }
   
    if(mpDates)
    {
        delete [] mpDates;
        mpDates = NULL;
    }
    
    delete [] mpValid;
    mpValid = NULL;
    
	mnValueSize = 0;
	mnDateSize = 0;
	mbRateReady = false;
    
	mFlag = DssDataInvalid;
    
	return S_OK;
}

int DSSNNFinancialGenericFunction::IsNullProcessable(bool *pCanProcessNULL)
{
    if(!pCanProcessNULL) return E_POINTER;
	
	*pCanProcessNULL = VARIANT_TRUE; //need to handle nulls
    
	return S_OK;
}

int DSSNNFinancialGenericFunction::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
	if(!pType) return E_POINTER;
    
	if (mFunctionIndex == 48) { //XIRR
		switch (nParamIndex) {
            case 1:
            case 2:
                *pType = DssParameterVector;
                break;
            default:
                return E_FAIL;
		}
	} else if (mFunctionIndex == 50){ //XNPV
		switch (nParamIndex) {
            case 1:
            case 2:
                *pType = DssParameterVector;
                break;
            case 3:
                *pType = DssParameterScalar;
                break;
            default:
                return E_FAIL;
		}
	} else
		return E_FAIL;
    
	return S_OK;
}


bool DSSNNFinancialGenericFunction::dataTypeAssignmentCompatible(EnumDSSDataType sourceType, EnumDSSDataType destType)
{
	EnumDSSDataType lType = DssDataTypeUnknown;
    
	switch (sourceType){
        case DssDataTypeShort:
        case DssDataTypeInteger:
        case DssDataTypeUnsigned:
        case DssDataTypeLong:
        case DssDataTypeFloat:
        case DssDataTypeReal:
        case DssDataTypeDouble:
        case DssDataTypeNumeric:
        case DssDataTypeDecimal:
        case DssDataTypeBool:
        case DssDataTypeBigDecimal:     // xzhu, 8/11/2014
            lType = DssDataTypeDouble;
            break;
            
        case DssDataTypeDate:
        case DssDataTypeTime:
        case DssDataTypeTimeStamp:
            lType = DssDataTypeDate;
            break;
            
        default:
            return false;
	}
    
	return (lType == destType) ? true : false;    
    
}

int DSSNNFinancialGenericFunction::GetOutput(Int32, Int32 *, double *, DSSDataFlag *)
{
    return E_NOTIMPL;
}

int DSSNNFinancialGenericFunction::GetOutputType(EnumDSSParameterType *)
{
    return E_NOTIMPL;
}

int DSSNNFinancialGenericFunction::SetInputParam(Int32, Int32, double *, DSSDataFlag *)
{
    return E_NOTIMPL;
}

int DSSNNFinancialGenericFunction::getParamFromVariant(VARIANT *pParam,LONG index, EnumDSSDataType type, BYTE *outResult, Int32 size)
{
	CComVariant element;
	HRESULT hr = SafeArrayGetElement(pParam->parray, &index, &element);
	if (hr != S_OK) return hr;
	
	hr = DFCDataConversion::ConvertFromVariant(&element, type, outResult, &size);
	return hr;
}
