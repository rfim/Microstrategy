//
//  DSSOperatorGeneric.cpp
//  ReportServiceCore
//
//  Created by Zhuo Yang on 11/2/12.
//  Copyright (c) 2012 Microstrategy. All rights reserved.
//

#include "DSSOperatorGeneric.h"

#import "DSSFunctionUtility.h"

#pragma warning(disable: 4800)

DSSOperatorGeneric::DSSOperatorGeneric() : mpResult(NULL), mpResultFlag(NULL), 
mppInput(NULL), mppInputFlag(NULL), mNumParam(0), mParamSize(0)
{
}

DSSOperatorGeneric::~DSSOperatorGeneric()
{
	if(mpResult) delete []mpResult;
	if(mpResultFlag) delete []mpResultFlag;
    
	if(mNumParam && mppInput)
	{
		for(Int32 i = 0; i < mNumParam; i++)
		{
			if(mppInput[i]) delete [](mppInput[i]);
			if(mppInputFlag[i]) delete [](mppInputFlag[i]);
		}
	}
    
	delete [] mppInput;
	delete [] mppInputFlag;
}

int DSSOperatorGeneric::setProperty (Int32 Index, CComVariant *pValue)
{
 	return E_NOTIMPL;
}

int DSSOperatorGeneric::Calculate ()
{    
	Int32 i = 0;
	for(  i = 0; i < mNumParam; i++)
	{
		if(mppInput[i] == NULL) 
			return E_FAIL;
        
		if(mppInputFlag[i] == NULL)
			return E_FAIL;
	}
    
	if(mpResult) delete [] mpResult;
	if(mpResultFlag) delete [] mpResultFlag;
    
	mpResult = new double [mParamSize];
	if (!mpResult)
		return E_OUTOFMEMORY;
    
	mpResultFlag = new DSSDataFlag [mParamSize];
	if (!mpResultFlag)
		return E_OUTOFMEMORY;
    
	for ( i = 0; i < mParamSize; i ++)
    {
		switch (mFunctionIndex)
        {
            case DssFuncFirstTrue:
			{
				Int32 j = 0;
				for( j = 0; j < mNumParam; j++)
				{
					if(mppInputFlag[j][i] == DssDataOk && mppInput[j][i] == true)
					{
						mpResult[i] = j+1;
						mpResultFlag[i] = DssDataOk;
						break;
					}                    
				}
                
				if(j == mNumParam)
				{
					mpResult[i] = 0;
					mpResultFlag[i] = DssDataOk;
				}
			}
                break;
                
            default:
                return E_FAIL;
                
		} // switch
	} // for    
    
	return S_OK;
}

int DSSOperatorGeneric::GetOutput (Int32 nSize, Int32 *nDataSize, double *pData, DSSDataFlag *pFlag)
{
	if(!nDataSize || !pData || !pFlag) return E_POINTER;
    
	if (nSize < mParamSize) 
		return E_FAIL;
    
	for (Int32 i = 0; i < mParamSize; i ++)
    {
		pData[i] = mpResult[i];
		pFlag[i] = mpResultFlag[i];
	}
    
	*nDataSize = mParamSize;
    
	return S_OK;
}

int DSSOperatorGeneric::GetOutputType (EnumDSSParameterType * pType)
{
 	if(!pType) return E_POINTER;
    
	*pType = DssParameterVector;
	
	return S_OK;
}

int DSSOperatorGeneric::Clear ()
{    
	if(mpResult) delete []mpResult;
	if(mpResultFlag) delete []mpResultFlag;
    
	if(mNumParam)
	{
		for(Int32 i = 0; i < mNumParam; i++)
		{
			if(mppInput && mppInput[i]) delete [](mppInput[i]);
			if(mppInputFlag && mppInputFlag[i]) delete [](mppInputFlag[i]);
		}
	}
    
	delete [] mppInput;
	delete [] mppInputFlag;
    
	mpResult = NULL;
	mpResultFlag = NULL;
	mppInput = NULL;
	mppInputFlag = NULL;
    
	// mNumParam = 0;
	mParamSize = 0;
    
	return S_OK;
}

int DSSOperatorGeneric::SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSDataFlag * pFlag)
{    
	if(mNumParam < 1) 
		return E_FAIL;
    
	if((nParamIndex < 1) || (nParamIndex > mNumParam)) return E_INVALIDARG;
    
	if(nSize < 1) return E_INVALIDARG;
    
	if(!pData || !pFlag) return E_POINTER;
    
	if(mppInput == NULL)
	{
		mppInput = new bool * [mNumParam];
		if(mppInput == NULL)
			return E_OUTOFMEMORY;
        
		mppInputFlag = new DSSDataFlag * [mNumParam];
		if(mppInputFlag == NULL)
			return E_OUTOFMEMORY;
        
		for(Int32 i = 0; i < mNumParam; i++)
		{
			mppInput[i] = NULL;
			mppInputFlag[i] = NULL;
		}
        
		mParamSize = nSize;
	}
    
	if(nSize != mParamSize)
		return E_INVALIDARG;
    
	Int32 lIndex = nParamIndex-1;
	if(mppInput[lIndex] == NULL)
	{
		mppInput[lIndex] = new bool[nSize];
		if( mppInput[lIndex] == NULL)
			return E_OUTOFMEMORY;
        
		mppInputFlag[lIndex] = new DSSDataFlag[nSize];
		if( mppInputFlag[lIndex] == NULL)
			return E_OUTOFMEMORY;
        
		for(Int32 i = 0; i < nSize; i++)
		{
			mppInput[lIndex][i] = static_cast<bool>(pData[i]);
			mppInputFlag[lIndex][i] = pFlag[i];
		}        
	}
	else
	{
		return E_FAIL;
	}    
    
	return S_OK;
}

int DSSOperatorGeneric::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
	if(!pType) return E_POINTER;
    
	*pType = DssParameterVector;
    
	// assume the time this function get called is same as the number of input parameters
	mNumParam++; 
    
	return S_OK;
}

int DSSOperatorGeneric::IsNullProcessable(bool *pCanProcessNULL)
{
	if(!pCanProcessNULL) return E_POINTER;
	
	*pCanProcessNULL = true; // always assume input data is valid
    
	return S_OK;    
}


// jxie, 2010-9-25
// support direct calculate
int DSSOperatorGeneric::DirectCalculate (Int32 nParam, Int32* ipSize, Int32** ipOffsets,
                                         void** ipData, double* ipDivider, DSSData_Flags** ipFlags, VARIANT_BOOL* ipAllDataValid,
                                         DSSDataType_Type* iType, vector<Int32> * ipvDataByGroup,
                                         Int32 nResSize, double* opData, DSSData_Flags* opFlag, VARIANT_BOOL* opAllDataValid,
                                         EnumNullCheckingOption iNullCheckingOption)
{    
	Int32 i = 0;
	assert(nParam > 0);
	Int32 lnSize = ipSize[0];
	if (ipvDataByGroup != NULL)
		lnSize = ipvDataByGroup->size();
    
	Int32 lRow = 0;
	for (i = 0; i < lnSize; i++)
	{
		if (ipvDataByGroup != NULL)
			lRow = (*ipvDataByGroup)[i];
		else
			lRow = i;
        
		switch (mFunctionIndex)
		{
            case DssFuncFirstTrue:
			{
				Int32 j = 0;
				for (j = 0; j < nParam; j++)
				{
					if(ipFlags[j][lRow] == DssDataOk &&
                       (bool)( GetNumericData(ipData[j], lRow, iType[j]) ) == true)
					{
						opData[lRow] = j + 1;
						opFlag[lRow] = DssDataOk;
						break;
					}
				}
                
				if (j == nParam)
				{
					opData[lRow] = 0;
					opFlag[lRow] = DssDataOk;
				}
			}
                break;
                
            default:
                return E_FAIL;
		} // switch
	} // for
    
	*opAllDataValid = VARIANT_TRUE;
    
	return S_OK;
}

// jxie, 2010-9-25
// support direct calculate on all functions in this class 
bool DSSOperatorGeneric::SupportDirectCalculate()
{
	return true;
}