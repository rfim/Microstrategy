//
//	DSSStrNNG.cpp
//  ReportServiceCore
//
//  Create by xzhu on 08/12/2014
//  Copyright (c) 2014 MicroStratey, Inc. All rights reserved.

#include "DSSStrNNG.h"
#include "DFCDataConversion.h"
#include "DSSFunctionUtility.h"

DSSStrNNG::DSSStrNNG()
 : mResultFlag(DssDataInvalid), mInputFlag(false),mDelimiter(", ")
{

}

DSSStrNNG::~DSSStrNNG()
{
}


int DSSStrNNG::get_FunctionType (EnumDSSFunctionType *  pType)
{
	HRESULT hr=S_OK;
    
	if (!pType) return E_POINTER;

	*pType = DssFunctionTypeGeneric;

	return hr = S_OK;
}

/*********************************************************************
FUNCTION:	CDSSStrNNG::SetProperty 
INPUTS:		Index					
			pValue					

OUTPUTS:	HResult
CREATED:	03/04/14,xiyu

HISTORY:
	06/04/14 (TQMS 894162,  xiyu):  Add a delimiter property



**********************************************************************/
int DSSStrNNG::setProperty (int Index, CComVariant *pValue)
{
	HRESULT hr=S_OK;
	if (!pValue)
		return E_POINTER;


	if(Index==2)
	{
		if(mFunctionIndex==DssFuncConcatAgg)
		{
			if (pValue->vt != VT_BSTR)
			{
				hr = VariantChangeType(pValue, pValue, 0, VT_BSTR);
				if (hr != S_OK) return S_OK;
			}

            BYTE * delimiter=new BYTE[256];
            int len=0;
            hr = DFCDataConversion::VariantSize(pValue, DssDataTypeVarChar, &len);
            CHECK_HR;
            DFCDataConversion::ConvertFromVariant(pValue, DssDataTypeVarChar, delimiter, &len);
            mDelimiter.assign((char *)delimiter);
            delete[] delimiter;
		}
	}
 	return hr;
}

int DSSStrNNG::Calculate ()
{
	HRESULT hr=S_OK;

	if(mFunctionIndex==DssFuncConcatAgg)
		return CalculateConcat();
    
	return hr = E_FAIL;
}


int DSSStrNNG::SetInputParam (Int32 nParamIndex, Int32 nSize, double * pData, DSSDataFlag *)
{
    return E_NOTIMPL;
}

int DSSStrNNG::SetInputParam (Int32 nParamIndex, Int32 nSize,EnumDSSDataType dataType, CComVariant *pData, DSSDataFlag *pFlag)
{
	HRESULT hr=S_OK;

	if(nParamIndex != 1) return hr = E_FAIL;
	
	if(nSize < 1) return E_FAIL;
	
	if(!pData	|| ( (pData->vt & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT)) )
		return E_FAIL;

	mDataSize = nSize;
	mInputDataType = dataType;
	mInputData.Copy(pData);
	if(pFlag)
	{
		mInputFlag=true;
		mpDataFlagPtr.Reset(new DSSData_Flags[mDataSize]);
		CHECK_NEWED_PTR(mpDataFlagPtr.Get());
		memmove(mpDataFlagPtr.Get(), pFlag, mDataSize * sizeof(DSSData_Flags));
	}
	return hr = S_OK;
}

int DSSStrNNG::GetOutput (Int32 nSize, Int32 *nDataSize, double *pData, DSSDataFlag *pFlag){
    return E_NOTIMPL;
}

int DSSStrNNG::GetOutput (Int32 nSize, Int32 *nDataSize, CComVariant *pData, DSSDataFlag *pFlag)
{
	HRESULT hr=S_OK;

	AE_ASSERT(nSize == 1);

	CHECK_PTR(nDataSize);
	CHECK_PTR(pData);
	CHECK_PTR(pFlag);

	*nDataSize = 1;
	*pFlag = mResultFlag;

	if (mResultFlag == DssDataOk)
		VariantCopy(pData, &mResultData);

	return hr = S_OK;
}

int DSSStrNNG:: GetOutputType(EnumDSSParameterType *pOutParamType)
{
    HREFTYPE hr=S_OK;
    
    CHECK_PTR(pOutParamType);
    *pOutParamType = DssParameterScalar;
    
    return hr;
}


int DSSStrNNG::GetOutputType (Int32 nSize, EnumDSSDataType *pInputType, EnumDSSDataType *pOutputDataType,
                              EnumDSSParameterType *pOutParamType)
{
	HRESULT hr=S_OK;
    
	CHECK_PTR(pInputType);
	CHECK_PTR(pOutputDataType);
	CHECK_PTR(pOutParamType);

	*pOutParamType = DssParameterScalar;

	AE_ASSERT(nSize == 1);

	*pOutputDataType = DssDataTypeVarChar;

	return hr = S_OK;
}

int DSSStrNNG::GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType)
{
	HRESULT hr=S_OK;
    
	if (!pType) 
		return hr = E_FAIL;

	if (nParamIndex != 1)
		return hr = E_FAIL;

	*pType = DssParameterVector;

	return hr = S_OK;
}

int DSSStrNNG::IsNullProcessable (bool *pCanProcessNULL)
{
	HRESULT hr=S_OK;
    
	if(!pCanProcessNULL) 
		return hr = E_FAIL;

	*pCanProcessNULL = VARIANT_TRUE; // always assume input data is valid

	return hr = S_OK;
}

int DSSStrNNG::Clear()
{
	HRESULT hr=S_OK;
    
	mDataSize = 0;
	mInputDataType = DssDataTypeUnknown;
	mResultFlag = DssDataUnknown;

	mInputData.Clear();
	mResultData.Clear();
    
	return hr = S_OK;
}

int DSSStrNNG::Init(EnumSimpleOperatorType iFunctionIndex)
{
  HRESULT hr=S_OK;
    
  mFunctionIndex = iFunctionIndex;
    
  return hr = S_OK;
}

int DSSStrNNG::get_OperatorType(EnumSimpleOperatorType * opType)
{
	HRESULT hr=S_OK;
	CHECK_PTR(opType);

	*opType = mFunctionIndex;
	return hr = S_OK;
}

int DSSStrNNG::getOutputType(EnumDSSDataType InputType, EnumDSSDataType* pOutputType)
{
	if (!pOutputType)
		return E_POINTER;

	*pOutputType=DssDataTypeVarChar;

	return S_OK;
}

// Caculate interface method for the variant input
int DSSStrNNG::Calculate(int nSize, CComVariant * pVec, EnumDSSDataType iType, CComVariant * pResult, DSSDataFlag * pFlag )
{
	HRESULT hr=S_OK;

	// We have only one input, ie the vector of variants.
	hr = SetInputParam(1,nSize, iType, pVec, NULL);
	CHECK_HR;

	if(mFunctionIndex==DssFuncConcatAgg)
	{	
		hr=CalculateConcat();
		CHECK_HR;
	}
	else return E_FAIL;

	*pFlag = mResultFlag;

	if (mResultFlag == DssDataOk) 
		VariantCopy(pResult, &mResultData);
	return S_OK;
}

int DSSStrNNG::CalculateConcat()
{
    //edited based on Concat from DSSStringFunc.cpp but without pairing up with datatype
	HRESULT hr = S_OK;

	mResultFlag = DssDataUnknown;
	
	if(mDataSize<1)
		return hr;

	MBase::BufferPtr lBufferPtr(MBase::CreateBuffer(1, 1));
	MBase::Buffer *lpBuffer = lBufferPtr.Get();
	Int32 lMaxBuffer = 256;
	BYTE *lpStr = new (lpBuffer) BYTE[lMaxBuffer];
    
    std::string lConcatStr;

	for(Int32 i=0;i<mDataSize;i++)
	{
		if(mInputFlag&&mpDataFlagPtr[i]!=DssDataOk)
		{	//null
			if(i<mDataSize-1)
				lConcatStr = lConcatStr+mDelimiter;
			continue;
		}
		CComVariant  lVar;
		getVariant(&mInputData, i, lVar);


		if (lVar.vt == VT_EMPTY)
		{
			if(i<mDataSize-1)
                lConcatStr = lConcatStr+mDelimiter;
			continue;
		}

		if (lVar.vt != VT_BSTR)
		{
			hr=DFCDataConversion::DFCVariantChange2BSTR(&lVar, &lVar, &mInputDataType);
			CHECK_HR;
		}

		Int32 lsLen = 0;
		hr = DFCDataConversion::VariantSize(&lVar, DssDataTypeVarChar, &lsLen);
		CHECK_HR;

		if(lsLen <= 0)
		{
			if(i<mDataSize-1)
                lConcatStr = lConcatStr+mDelimiter;
			continue;
		}

		if(lsLen > lMaxBuffer)
		{
			lMaxBuffer = lsLen;
			lpStr = new (lpBuffer) BYTE[lMaxBuffer];
		}
		hr = DFCDataConversion::ConvertFromVariant(&lVar, DssDataTypeVarChar, lpStr, &lsLen);
		CHECK_HR;

        char *lpWstr=(char *)lpStr;


		if(i < mDataSize - 1)
		{
            lConcatStr = lConcatStr+lpWstr;
            lConcatStr = lConcatStr+mDelimiter;
		}else
            lConcatStr = lConcatStr+lpWstr;
	}

	hr = DFCDataConversion::ConvertToVariant((BYTE *)lConcatStr.c_str(), DssDataTypeVarChar, 0, &mResultData);
	CHECK_HR;
	
	mResultFlag=DssDataOk;
	return S_OK;

}