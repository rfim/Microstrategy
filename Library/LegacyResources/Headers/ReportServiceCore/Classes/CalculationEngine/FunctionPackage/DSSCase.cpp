//
//  DSSCase.cpp
//  ReportServiceCore
//
//  Created by Yang Zhuo on 7/20/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//

#include "DSSCase.h"
#include "DSSFunctionUtility.h"
#include "DFCDataConversion.h"

DSSCase::DSSCase():mResultData(DssDataInvalid), mDataSize(0), mfIsNumeric(false)
{
    
}

DSSCase::~DSSCase()
{
    Int32 lSize = mvInputData.size();
    for(Int32 i = 0; i < lSize; ++i)
    {
        if(mvInputData[i])
        {
            VariantClear(mvInputData[i]);
            delete mvInputData[i];
        }
    }
    VariantClear(&mResultData);
}

int DSSCase::hCalculateNumeric()
{
    int hr = S_OK;
    
    int nParam = mDataSize;
    
    switch (mFunctionIndex) {
        case DssFuncCase:
        {
            if(nParam < 3)
            {
                return E_FAIL;
            }
            
            Int32 i = 0, k = 0;
            k = nParam / 2;
            mResultFlag = DssDataInvalid;
            
            for(i = 0; i < k; ++i)
            {
                if(DssDataOk != mvDataFlag[i * 2])
                {
                    mResultFlag = mvDataFlag[i * 2];
                    return S_OK;
                }
                bool cond = (bool)mvNumericInputData[i * 2];
                if(cond)
                {
                    mNumericResultData = mvNumericInputData[i * 2 + 1];
                    mResultFlag = mvDataFlag[i * 2 + 1];
                    break;
                }
            }
            
            if(i == k && nParam == 2 * i + 1)
            {
                mNumericResultData = mvNumericInputData[nParam - 1];
                mResultFlag = mvDataFlag[nParam - 1];
            }
            break;
        }   
        case DssFuncCaseV:
        {
            if(nParam < 4)
            {
                return E_FAIL;
            }
            
            Int32 i = 0, k = 0;
            k = (nParam - 1) / 2;
            mResultFlag = DssDataInvalid;
            
            if(DssDataOk != mvDataFlag[0])
            {
                mResultFlag = mvDataFlag[0];
                break;
            }
            
            for (i = 0; i < k; ++i) {
                bool bEqual = false;
                hr = CmpNumericParams(0, i*2+1, &bEqual);
                CHECK_HR;
                
                if(bEqual)
                {
                    mNumericResultData = mvNumericInputData[i*2+2];
                    mResultFlag = mvDataFlag[i*2+2];
                    break;
                }
            }
            
            if(i == k && (nParam-1 == 2*i+1))
            {
                mNumericResultData = mvNumericInputData[nParam - 1];
                mResultFlag = mvDataFlag[nParam - 1];
            }
        }
            break;
        case DssFuncIF:
        {
            if(3 != nParam)
            {
                return E_FAIL;
            }
            
            mResultFlag = DssDataInvalid;
            if(DssDataOk != mvDataFlag[0])
            {
                mResultFlag = mvDataFlag[0];
                break;
            }
            
            bool cond = (bool)mvNumericInputData[0];
            if(cond)
            {
                mNumericResultData = mvNumericInputData[1];
                mResultFlag = mvDataFlag[1];
            }
            else
            {
                mNumericResultData = mvNumericInputData[2];
                mResultFlag = mvDataFlag[2];
                
                if(0xFF == mResultFlag)
                {
                    //!!!!zhyang, 20/07/2011, DssDataIgnoreCheckNull is undefined
                    //mResultFlag = DssDataIgnoreCheckNull;
                }
            }
        }
            break;
        default:
            assert(false);
    }
    
    return hr;
}

int DSSCase::Calculate()
{
    int hr = S_OK;
    if (mfIsNumeric)
		return hCalculateNumeric();
    
	Int32 nParam = mDataSize;
	switch(mFunctionIndex)
	{
        case DssFuncCase:
        {

            if(nParam < 3)
                return E_FAIL;
            
            Int32 i = 0,  k = 0 ;
            k = nParam / 2;
            mResultFlag = DssDataInvalid;
            
            for (i = 0; i < k; i++)
            {
                bool cond = false;
                
                if (mvDataFlag[i * 2] != DssDataOk)
                {
                    mResultFlag = mvDataFlag[i * 2];
                    return S_OK;
                }
                
                hr = GetParamFromVariant(i*2, DssDataTypeBool, (BYTE *)&cond, sizeof(bool));
                CHECK_HR;
                if (cond)
                {
                    hr = CopyParamOut(2*i+1, &mResultData);
                    CHECK_HR;
                    mResultFlag = mvDataFlag[2 * i + 1];
                    break;
                }
            }
            
            if ((i==k) && (nParam==2*i+1))
            {
                hr = CopyParamOut(nParam-1, &mResultData);
                mResultFlag = mvDataFlag[nParam - 1];
            }
        }
            
            break;
            
        case DssFuncCaseV:
        {
            if(nParam < 4)
                return E_FAIL;
            
            Int32 i = 0,  k = 0 ;
            k = (nParam - 1) / 2;
            mResultFlag = DssDataInvalid;
            
            if(mvDataFlag[0] != DssDataOk)
            {
                mResultFlag = mvDataFlag[0];
                break;
            }
            
            for (i = 0; i < k; i++)
            {
                bool bEqual = false;
                hr = CmpParams(0, i*2+1, &bEqual);
                CHECK_HR;
                if (bEqual)
                {
                    CopyParamOut(i*2+2, &mResultData);
                    mResultFlag = mvDataFlag[i * 2 + 2];
                    break;
                }
            }
            
            if ((i==k) && (nParam-1==2*i+1))
            {
                CopyParamOut(nParam-1, &mResultData);
                mResultFlag = mvDataFlag[nParam - 1];
            }
            
            
        }
            
            break;
            
        case DssFuncIF:
		{
			if(nParam != 3)
				return E_FAIL;
            
			mResultFlag = DssDataInvalid;
            
	
			if(mvDataFlag[0] != DssDataOk)
			{
				mResultFlag = mvDataFlag[0];
				break;
			}
			bool cond = false;
			hr = GetParamFromVariant(0, DssDataTypeBool, (BYTE *)&cond, sizeof(bool));
			CHECK_HR;
            
			if (cond)
			{
				hr = CopyParamOut(1, &mResultData);
				CHECK_HR;
				mResultFlag = mvDataFlag[1];
			} else {
                
				hr = CopyParamOut(2, &mResultData);
				CHECK_HR;
				mResultFlag = mvDataFlag[2];
                
				if (mResultFlag == 0xFF) 
                {
                    //!!!!zhyang
                    //mResultFlag = DssDataIgnoreCheckNull;
                }
			}
            
		}
            
            break;
            
        default:
            AE_ASSERT(false);
	}
    return hr;
}

int DSSCase::setProperty(int index, CComVariant *pValue)
{
    return E_NOTIMPL;
}

int DSSCase::GetOutputType(EnumDSSParameterType *pType)
{
    if(!pType)
    {
        return E_POINTER;
    }
    
    *pType = DssParameterScalar;
    return S_OK;
}

int DSSCase::GetOutputType(int size, EnumDSSDataType *inputType, 
                           EnumDSSDataType *outputType, EnumDSSParameterType *opOutParamType)
{
    int hr = S_OK;
    
    CHECK_PTR(inputType);
    CHECK_PTR(outputType);
    CHECK_PTR(opOutParamType);
    
    *opOutParamType = DssParameterScalar;
    
    Int32 i = 0;
    
    if(!outputType)
    {
        return E_FAIL;
    }
    
    EnumDSSDataType lOutputType = DssDataTypeUnknown;
    switch (mFunctionIndex) {
        case DssFuncCase:
        
            if(size < 3)
            {
                return E_FAIL;
            }
            lOutputType = inputType[1];
            for(i = 3; i < size; i+=2)
            {
                CmpDataTypes(lOutputType, inputType[i], &lOutputType);
            }
            if(i == size)
            {
                CmpDataTypes(lOutputType, inputType[size-1], &lOutputType);
            }
            *outputType = lOutputType;
            break;
        
        case DssFuncCaseV:
            if (size < 4)
                return E_FAIL;	
            lOutputType = inputType[2];
            for (i = 4; i < size; i += 2)
            {
                CmpDataTypes(lOutputType, inputType[i], &lOutputType);
            }
            
            if (i == size)	
            {
                CmpDataTypes(lOutputType, inputType[size - 1], &lOutputType);
            }
            
            *outputType = lOutputType;
            break;
            
        case DssFuncIF:
		{
			if (size !=  3)
            {
				return hr = E_FAIL;	
            }
            
			CmpDataTypes(inputType[1], inputType[2], &lOutputType);
            
			*outputType = lOutputType;     
		} 
            break;
            
        default:
            assert(false);
    }
    
    return hr;
}

int DSSCase::CopyParamOut(long index, CComVariant *pResult)
{
    int hr = S_OK;
    CComVariant **lppVar = &(mvInputData[index]);
    if(*lppVar)
    {
        VariantCopy(&mResultData, *lppVar);
    }
    else
    {
        return E_FAIL;
    }
    
    return hr;
}

int DSSCase::GetParamFromVariant(long index, EnumDSSDataType type, unsigned char *opResult, int size)
{
    CComVariant **lppVar = &(mvInputData[index]);
    if(*lppVar)
    {
        return DFCDataConversion::ConvertFromVariant(*lppVar, type, opResult, &size);
    }
    else
    {
        return E_FAIL;
    }
    return S_OK;
}

int DSSCase::CmpNumericParams(int index1, int index2, bool *fEqual)
{
    //zhyang, 20/07/2011, use DoubleEqual function to compare two doubles instead of the server side operator ==
    *fEqual = DoubleEqual(mvNumericInputData[index1], mvNumericInputData[index2]);
    return S_OK;
}

int DSSCase::CmpParams(int index1, int index2, bool *fEqual)
{
    int hr = S_OK;
    double v1, v2;
    hr = GetParamFromVariant(index1, DssDataTypeDouble, (unsigned char *)&v1, sizeof(double));
    CHECK_HR;
    hr = GetParamFromVariant(index2, DssDataTypeDouble, (unsigned char *)&v2, sizeof(double));
    CHECK_HR;
    
    //zhyang, 20/07/2011, use DoubleEqual function to compare two doubles instead of the server side operator ==
    *fEqual = DoubleEqual(v1, v2);
    return S_OK;
}

int DSSCase::CmpDataTypes(EnumDSSDataType type1, EnumDSSDataType type2, EnumDSSDataType *opOutputType)
{
    int hr = S_OK;
    
	*opOutputType = type1;
    
	if (type1 == type2)
	{
		return hr = S_OK;
	}
    
	// deliu, 2012-12-12, TQMS 603849, handle different types of the input parameters
	// now the type priorities for different data types are:
	// Bool->Short->UInt32/Int32->Numeric/Decimal->Real/Float->BigDecimal/CellFormat/Double->
	// Binary/VBinary->Char/VarChar->NChar/NVarChar
	// handle DateTime as double if one is DateTime and the other is not
    
	typedef enum {
		PTYReserved = 0,
		PTYBool = 1,
		PTYShort = 2,
		PTYInteger = 3,
		PTYLong = 4,
		PTYDecimal = 5,
		PTYFloat = 6,
		PTYInt64 = 7,
		PTYDouble = 8,
		PTYBinary = 9,
		PTYVarChar = 10,
		PTYDateTime = 11,
		PTYOthers = 12
	} TYPE_PTY;
    
	DSSDataType_Type lTypes[2];
	lTypes[0] = type1;
	lTypes[1] = type2;
	TYPE_PTY lTypePTY[2];
	for (Int32 i = 0; i < 2; i++)
	{
		switch (lTypes[i])
		{
            case DssDataTypeBool:
                lTypePTY[i] = PTYBool;
                break;
                
            case DssDataTypeShort:
                lTypePTY[i] = PTYShort;
                break;
                
            case DssDataTypeInteger:
            case DssDataTypeUnsigned:
                lTypePTY[i] = PTYInteger;
                break;
                
            case DssDataTypeLong:
                lTypePTY[i] = PTYLong;
                break;
                
            case DssDataTypeNumeric:
            case DssDataTypeDecimal:
                lTypePTY[i] = PTYDecimal;
                break;
                
            case DssDataTypeReal:
            case DssDataTypeFloat:
                lTypePTY[i] = PTYFloat;
                break;
                
            case DssDataTypeInt64:
                lTypePTY[i] = PTYInt64;
                break;
                
            case DssDataTypeBigDecimal:
            case DssDataTypeCellFormatData:
            case DssDataTypeDouble:
                lTypePTY[i] = PTYDouble;
                break;
                
            case DssDataTypeBinary:
            case DssDataTypeVarBin:
            case DssDataTypeLongVarBin:
                lTypePTY[i] = PTYBinary;
                break;
                
            case DssDataTypeChar:
            case DssDataTypeVarChar:
            case DssDataTypeLongVarChar:
            case DssDataTypeNChar:
            case DssDataTypeNVarChar:
                //// jxie, 2011-2-11, TQMS 454784
                //case DssDataTypeMBChar:
            case DssDataTypeUTF8Char:
                lTypePTY[i] = PTYVarChar;
                break;
                
            case DssDataTypeDate:
            case DssDataTypeTime:
            case DssDataTypeTimeStamp:
                lTypePTY[i] = PTYDateTime;
                break;
                
            default:
                lTypePTY[i] = PTYOthers;
                break;
		}
    }
    
	// if one is DateTime and the other is not, then treat the DateTime as double
	if (lTypePTY[0] != lTypePTY[1])
	{
		if (lTypePTY[0] == PTYDateTime)
			lTypePTY[0] = PTYDouble;
        
		if (lTypePTY[1] == PTYDateTime)
			lTypePTY[1] = PTYDouble;
	}
    
	// select the data type with higher priority
	TYPE_PTY lResTypePTY = lTypePTY[0] < lTypePTY[1] ? lTypePTY[1] : lTypePTY[0];
    
	// get the specific output type in each priority group, ignore BOOL
	// could use a array or map to map each
	switch (lResTypePTY)
	{
        case PTYShort:
            *opOutputType = DssDataTypeShort;
            break;
            
        case PTYInteger:
            *opOutputType = DssDataTypeInteger;
            break;
            
        case PTYLong:
            *opOutputType = DssDataTypeLong;
            break;
            
        case PTYDecimal:
            *opOutputType = DssDataTypeDecimal;
            break;
            
        case PTYFloat:
            *opOutputType = DssDataTypeFloat;
            break;
            
        case PTYInt64:
            *opOutputType = DssDataTypeInt64;
            break;
            
        case PTYDouble:
            *opOutputType = DssDataTypeDouble;
            break;
            
        case PTYBinary:
            *opOutputType = DssDataTypeVarBin;
            break;
            
        case PTYVarChar:
            *opOutputType = DssDataTypeVarChar;
            break;
            
        case PTYDateTime:
            *opOutputType = DssDataTypeTimeStamp;
            break;
            
        case PTYOthers:
        default:
            *opOutputType = DssDataTypeUnknown;
            break;
	}
    
    return hr;
}

int DSSCase::SetInputParam(int nParamIndex, int nSize, double *pData, DSSDataFlag *pFlag)
{
    mfIsNumeric = true;
    
    if(1 != nSize || !pData || !pFlag)
    {
        return E_INVALIDARG;
    }
    
    if(nParamIndex > mvDataFlag.size())
    {
        mvDataFlag.resize(nParamIndex * 2);
        mvNumericInputData.resize(nParamIndex*2, -1);
    }
    
    if(mDataSize < nParamIndex)
    {
        mDataSize = nParamIndex;
    }
    
    mvDataFlag[nParamIndex-1] = *pFlag;
    mvNumericInputData[nParamIndex-1] = *pData;
    return S_OK;
}

int DSSCase::SetInputParam(int nParamIndex, int nSize, EnumDSSDataType dataType, CComVariant *pData, DSSDataFlag *pFlag)
{
    int hr = S_OK;
    if(1 != nSize || !pData || !pFlag)
    {
        return  E_INVALIDARG;
    }
    
    if((pData->vt & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT))
		return hr = E_FAIL;
    
	if (nParamIndex > mvDataFlag.size())
	{
		mvDataFlag.resize(nParamIndex * 2);
		mvInputData.resize(nParamIndex * 2, NULL);
	}
     
	if (mDataSize < nParamIndex)
	{
		mDataSize = nParamIndex;
	}
    
	mvDataFlag[nParamIndex - 1] = *pFlag;
    
    CComVariant **lppVar = &(mvInputData[nParamIndex-1]);
    long temp = 0;
    if (*lppVar)
		//VariantCopy((*lppVar), pData);
		SafeArrayGetElement(pData->parray, &temp, *lppVar);
	else
	{
		(*lppVar) = new CComVariant();
		SafeArrayGetElement(pData->parray, &temp, *lppVar);
	}
    
    return hr;
}

int DSSCase::GetOutput(int nSize, int *nDataSize, double *pData, DSSDataFlag *pflag)
{
    int hr = S_OK;
    assert(1 == nSize);
    CHECK_PTR(nDataSize);
    CHECK_PTR(pData);
    CHECK_PTR(pflag);
    
    *nDataSize = 1;
    *pflag = mResultFlag;
    if(DssDataOk == *pflag)
    {
        *pData = mNumericResultData;
    }
    
    return hr;
}

int DSSCase::GetOutput(int nSize, int *nDataSize, CComVariant *pData, DSSDataFlag *pFlag)
{
    int hr = S_OK;
    assert(1 == nSize);
    CHECK_PTR(nDataSize);
    CHECK_PTR(pData);
    CHECK_PTR(pFlag);
    
    *nDataSize = 1;
    *pFlag = mResultFlag;
    if(DssDataOk == *pFlag)
    {
        VariantCopy(pData, &mResultData);
    }
    VariantClear(&mResultData);
    
    return hr;
}

int DSSCase::IsNullProcessable(bool *pCanProcessNull)
{
    if(pCanProcessNull)
    {
        if(DssFuncCaseV == mFunctionIndex)
        {
            *pCanProcessNull = false;
        }
        else
        {
            *pCanProcessNull = true;
        }
    }
    return S_OK;
}

int DSSCase::Clear()
{
    for(Int32 i = 0; i < mDataSize; ++i)
    {
        mvDataFlag[i] = DssDataOk;
        if(!mfIsNumeric)
        {
            VariantClear(mvInputData[i]);
        }
    }
    mDataSize = 0;
    mResultFlag = DssDataInvalid;
    
    return S_OK;
}

int DSSCase::GetInputParamType(int nParamIndex, EnumDSSParameterType *pType)
{
    if(pType)
    {
        *pType = DssParameterVector;
    }
    return S_OK;
}

