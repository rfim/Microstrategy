/*
 *  SimpleOperator.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 2/4/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#include "DSSSimpleOperatorFunc.h"
#import "DSSFunctionUtility.h"
#import "CellDataHelper.h"

DSSSimpleOperatorFunc::DSSSimpleOperatorFunc(): mOutputType(DssDataTypeUnknown)
{}

DSSSimpleOperatorFunc::~DSSSimpleOperatorFunc()
{}

int DSSSimpleOperatorFunc::Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag)
{
	enum MathError error = MathNoError;
	
	if(!ipParams || !opResult || !opFlag)
		return E_POINTER;
	
	*opFlag = DssDataOk;
	int i = 0;
	
	switch(mFunctionIndex)
	{
		case DssFuncPlus:
			if(nParam < 2)
				return E_FAIL;
			
			*opResult = 0;
			for(i=0; i<nParam; i++)
				*opResult += ipParams[i];
			
			*opFlag = DssDataOk;
			break;
			
		case DssFuncAdd:
			if(nParam < 1)
				return E_FAIL;
			
			*opResult = 0;
			for(i=0; i<nParam; i++)
				*opResult += ipParams[i];
			
			*opFlag = DssDataOk;
			break;
			
		case DssFuncMinus:
			if(nParam < 2)
				return E_FAIL;
			
			*opResult = ipParams[0];
			for(i = 1; i < nParam; i++)
				*opResult = (*opResult) - ipParams[i];
			
			*opFlag = DssDataOk;
			break;
			
		case DssFuncTimes:
		case DssFuncMultiply:
			if(nParam < 2)
				return E_FAIL;
			
			*opResult = 1;
			for(i = 0; i < nParam; i++)
				*opResult = (*opResult) * ipParams[i];
			
			*opFlag = DssDataOk;
			break;
			
		case DssFuncDivide:
			if(nParam < 2)
				return E_FAIL;
			
			*opFlag = DssDataOk;
			*opResult = ipParams[0];
			for(i = 1; i < nParam; i++)
			{
				if(ipParams[i] == 0)
				{
					*opResult = DBL_MAX;
					*opFlag = DssDataZerodivide;
					break;
				}
				else
					*opResult = (*opResult) / ipParams[i];
			}
			
			break;
			
		case DssFuncUnaryMinus:
			if(nParam != 1)
				return E_FAIL;
			
			*opResult = -(ipParams[0]);
			*opFlag = DssDataOk;
			break;
			
		case DssFuncAbs:
			if(nParam != 1) 
				return E_FAIL;
			
			*opResult = fabs(ipParams[0]) ;
			*opFlag = DssDataOk;
			break;
			
		case DssFuncAverage:
			if(nParam < 1)
				return E_FAIL;
			
			*opResult = hAvgImpl(nParam, ipParams);
			*opFlag = DssDataOk;
			
			break;
			
			
		default: 
			_ASSERT(false);
			return E_FAIL;
	}
	
	CheckValue(&error, *opResult);
	
	if(error != MathNoError)
	{
		*opResult = 0;
		*opFlag = DssDataInvalid;
	}
	
	return S_OK;
}

double DSSSimpleOperatorFunc::hAvgImpl(int nSize, double* pData)
{
	double d = 0;
	double M = pData[0];
	
	for(int i=1; i<nSize; i++)
	{
		d = (pData[i] - M)/(i+1);
		M = M + d;
	}
	
	return M;
}

int DSSSimpleOperatorFunc::Calculate(int nParam, CComVariant *ipParam, CComVariant *opResult, DSSDataFlag *opFlag)
{
    int hr;
	enum MathError error = MathNoError;
    
	if(!ipParam || !opResult || !opFlag)
        return E_POINTER;
    
	if ( (ipParam->vt & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT))
		return E_FAIL;
    
    *opFlag = DssDataOk;
	LONG i = 0;
	double lResult = 0;
	double lTemp = 0;
	MDataType::DSSDateTime lTempDate;
	Int32 nDoubleSize = sizeof(double);
	Int32 nDateSize = sizeof(MDataType::DSSDateTime);
	CComVariant element;
	EnumDSSDataType lDateType = (mOutputType == DssDataTypeUnknown) ? DssDataTypeTimeStamp : mOutputType;
    
    // jxie, 2011-1-30, TQMS 450450
	// indicates if all the input data includes at least one timestamp data
	bool lfHasDateInput = false;
	// indicates if current input data type is timestamp
	bool lfCurDateInput = false;
    
    switch (mFunctionIndex)
    {
        case DssFuncPlus:
        case DssFuncAdd:
            // check number of input parameters
            if(nParam < 1)
                return S_FALSE;
            
            for (i = 0; i < nParam; i++)
            {
                SafeArrayGetElement(ipParam->parray, &i, &element);
                lfCurDateInput = false;
                if (element.vt == VT_DATE && (mOutputType == DssDataTypeDate || mOutputType == DssDataTypeTime || mOutputType == DssDataTypeTimeStamp || mOutputType == DssDataTypeUnknown))	// datetime data
                {
                    hr = DFCDataConversion::ConvertFromVariant(&element, lDateType, (BYTE *)&lTempDate, &nDateSize);
                    if (!FAILED(hr))	// failed means the data type is big decimal
                    {
                        lfCurDateInput = true;
                        
                        // deliu, 2013-01-16, 664841 disable calculation for both DateTime
                        if (lfHasDateInput)
                        {
                            opResult->vt = VT_EMPTY;
                            *opFlag = DssDataInvalid;
                            return S_OK;
                        }
                        
                        lfHasDateInput = true;
                        lTemp = lTempDate.ToDATE();
                    }
                }
                if (!lfCurDateInput)	// numeric or big decimal data
                {
                    hr = DFCDataConversion::ConvertFromVariant(&element, DssDataTypeDouble, (BYTE *)&lTemp, &nDoubleSize);
                    if (FAILED(hr))
                    {
                        opResult->vt = VT_EMPTY;
                        *opFlag = DssDataInvalid;
                        return S_OK;
                    }
                }
                
                lResult += lTemp;
            }
            
            break;
            
        case DssFuncMinus:
            // check number of input parameters
            if(nParam < 2)
                return S_FALSE;
            
            for(i = 0; i < nParam; i++)
            {
                SafeArrayGetElement(ipParam->parray, &i, &element);
                lfCurDateInput = false;
                if (element.vt == VT_DATE && (mOutputType == DssDataTypeDate || mOutputType == DssDataTypeTime || mOutputType == DssDataTypeTimeStamp || mOutputType == DssDataTypeUnknown))	// datetime data
                {
                    hr = DFCDataConversion::ConvertFromVariant(&element, lDateType, (BYTE *)&lTempDate, &nDateSize);
                    if (!FAILED(hr))	// failed means the data type is big decimal
                    {
                        lfCurDateInput = true;
                        lfHasDateInput = true;
                        lTemp = lTempDate.ToDATE();
                    }
                }
                if (!lfCurDateInput)	// numeric or big decimal data
                {
                    hr = DFCDataConversion::ConvertFromVariant(&element, DssDataTypeDouble, (BYTE *)&lTemp, &nDoubleSize);
                    if (FAILED(hr))
                    {
                        opResult->vt = VT_EMPTY;
                        *opFlag = DssDataInvalid;
                        return S_OK;
                    }
                }
                
                if (i == 0)
                    lResult = lTemp;
                else
                    lResult -= lTemp;
            }
            
            break;
        case DssFuncTimes:
        case DssFuncMultiply: // Yin-Shen Chao; 17 Apr 2006; More Built-In Functions
            // check number of input parameters
            if(nParam < 2)
                return S_FALSE;
            
            lResult = 1;
            for(i = 0; i < nParam; i++)
            {
                SafeArrayGetElement(ipParam->parray, &i, &element);
                lfCurDateInput = false;
                if (element.vt == VT_DATE && (mOutputType == DssDataTypeDate || mOutputType == DssDataTypeTime || mOutputType == DssDataTypeTimeStamp || mOutputType == DssDataTypeUnknown))	// datetime data
                {
                    hr = DFCDataConversion::ConvertFromVariant(&element, lDateType, (BYTE *)&lTempDate, &nDateSize);
                    if (!FAILED(hr))	// failed means the data type is big decimal
                    {
                        // deliu, 2013-01-16, 664841 disable mutliply for DateTime
                        opResult->vt = VT_EMPTY;
                        *opFlag = DssDataInvalid;
                        return S_OK;
                    }
                }
                if (!lfCurDateInput)	// numeric or big decimal data
                {
                    hr = DFCDataConversion::ConvertFromVariant(&element, DssDataTypeDouble, (BYTE *)&lTemp, &nDoubleSize);
                    if (FAILED(hr))
                    {
                        opResult->vt = VT_EMPTY;
                        *opFlag = DssDataInvalid;
                        return S_OK;
                    }
                }
                
                lResult *= lTemp;
            }
            
            break;
        case DssFuncDivide:
            if(nParam < 2)
                return S_FALSE;
            
            for(i = 0; i < nParam; i++)
            {
                SafeArrayGetElement(ipParam->parray, &i, &element);
                lfCurDateInput = false;
                if (element.vt == VT_DATE && (mOutputType == DssDataTypeDate || mOutputType == DssDataTypeTime || mOutputType == DssDataTypeTimeStamp || mOutputType == DssDataTypeUnknown))	// datetime data
                {
                    hr = DFCDataConversion::ConvertFromVariant(&element, lDateType, (BYTE *)&lTempDate, &nDateSize);
                    if (!FAILED(hr))	// failed means the data type is big decimal
                    {
                        // deliu, 2013-01-16, 664841, disable divide DateTime
                        opResult->vt = VT_EMPTY;
                        *opFlag = DssDataInvalid;
                        return S_OK;
                    }
                }
                if (!lfCurDateInput)	// numeric or big decimal data
                {
                    hr = DFCDataConversion::ConvertFromVariant(&element, DssDataTypeDouble,
                                                    (BYTE *)&lTemp, &nDoubleSize);
                    if (FAILED(hr))
                    {
                        opResult->vt = VT_EMPTY;
                        *opFlag = DssDataInvalid;
                        return S_OK;
                    }
                }
                
                if (i == 0)
                    lResult = lTemp;
                else
                {
                    if(lTemp == 0)
                    {
                        lResult = DBL_MAX;
                        *opFlag = DssDataZerodivide;
                        break;
                    }
                    else
                        lResult /= lTemp;
                }
            }
            
            break;
        
        case DssFuncUnaryMinus:
            if(nParam != 1)
                return S_FALSE;
            
            i = 0;
            SafeArrayGetElement(ipParam->parray, &i, &element);
            lfCurDateInput = false;
            if (element.vt == VT_DATE && (mOutputType == DssDataTypeDate || mOutputType == DssDataTypeTime || mOutputType == DssDataTypeTimeStamp || mOutputType == DssDataTypeUnknown))	// datetime data
            {
                hr = DFCDataConversion::ConvertFromVariant(&element, lDateType, (BYTE *)&lTempDate, &nDateSize);
                if (!FAILED(hr))	// failed means the data type is big decimal
                {
                    lfCurDateInput = true;
                    lfHasDateInput = true;
                    lTemp = lTempDate.ToDATE();
                }
            }
            if (!lfCurDateInput)	// numeric or big decimal data
            {
                hr = DFCDataConversion::ConvertFromVariant(&element, DssDataTypeDouble, (BYTE *)&lTemp, &nDoubleSize);
                if (FAILED(hr))
                {
                    *opFlag = DssDataInvalid;
                    return S_OK;
                }
            }
			
            lResult = -lTemp;
            break;
        
        case DssFuncEquals:
        case DssFuncNotEqual:
        case DssFuncGreater:
        case DssFuncLess:
        case DssFuncGreaterEqual:
        case DssFuncLessEqual:
        case DssFuncBetween:
        case DssFuncNotBetween:
        case DssFuncAnd:
        case DssFuncOr:
        case DssFuncNot:
            _ASSERTE(false); //implemented in other files
            break;
        
        case DssFuncAbs:
            if(nParam != 1) 
                return S_FALSE;
            
            i = 0;
            SafeArrayGetElement(ipParam->parray, &i, &element);
            lfCurDateInput = false;
            if (element.vt == VT_DATE && (mOutputType == DssDataTypeDate || mOutputType == DssDataTypeTime || mOutputType == DssDataTypeTimeStamp || mOutputType == DssDataTypeUnknown))	// datetime data
            {
                hr = DFCDataConversion::ConvertFromVariant(&element, lDateType, (BYTE *)&lTempDate, &nDateSize);
                if (!FAILED(hr))	// failed means the data type is big decimal
                {
                    lfCurDateInput = true;
                    lfHasDateInput = true;
                    lTemp = lTempDate.ToDATE();
                }
            }
            if (!lfCurDateInput)	// numeric or big decimal data
            {
                hr = DFCDataConversion::ConvertFromVariant(&element, DssDataTypeDouble, (BYTE *)&lTemp, &nDoubleSize);
                if (FAILED(hr))
                {
                    opResult->vt = VT_EMPTY;
                    *opFlag = DssDataInvalid;
                    return S_OK;
                }
            }
            
            lResult = fabs(lTemp) ;
            break;
        
            // Yin-Shen Chao; 17 Apr 2006; More Built-In Functions
        case DssFuncAverage:
            if(nParam < 1)
                return S_FALSE;
            
            for(i = 0; i < nParam; i++)
            {
                SafeArrayGetElement(ipParam->parray, &i, &element);
                lfCurDateInput = false;
                if (element.vt == VT_DATE && (mOutputType == DssDataTypeDate || mOutputType == DssDataTypeTime || mOutputType == DssDataTypeTimeStamp || mOutputType == DssDataTypeUnknown))	// datetime data
                {
                    hr = DFCDataConversion::ConvertFromVariant(&element, lDateType, (BYTE *)&lTempDate, &nDateSize);
                    if (!FAILED(hr))	// failed means the data type is big decimal
                    {
                        lfCurDateInput = true;
                        lfHasDateInput = true;
                        lTemp = lTempDate.ToDATE();
                    }
                }
                if (!lfCurDateInput)	// numeric or big decimal data
                {
                    hr = DFCDataConversion::ConvertFromVariant(&element, DssDataTypeDouble, (BYTE *)&lTemp, &nDoubleSize);
                    if (FAILED(hr))
                    {
                        opResult->vt = VT_EMPTY;
                        *opFlag = DssDataInvalid;
                        return S_OK;
                    }
                }
                
                lResult += lTemp;
            }
            lResult /= nParam;
            
            break;
            
        default: 
            return S_FALSE;
    }
    
    if (*opFlag == DssDataOk)
	{
		CheckValue(&error, lResult);
        
		if(error != MathNoError)
		{
			lResult = 0;
			*opFlag = DssDataInvalid;
		}
	}
    
    if (*opFlag == DssDataOk)
	{
		if (lfHasDateInput || mOutputType == DssDataTypeDate || mOutputType == DssDataTypeTime || mOutputType == DssDataTypeTimeStamp)	// datetime data
		{
			MDataType::DSSDateTime lDate(lResult);
            DFCDataConversion::ConvertToVariant((BYTE *)(&lDate), lDateType, 0, opResult);
		}
		else
		{
            DFCDataConversion::ConvertToVariant((BYTE *)(&lResult), DssDataTypeDouble, 0, opResult);
		}
	}
	else
    {
		opResult->vt = VT_EMPTY;
    }
    
    return S_OK;
}

int DSSSimpleOperatorFunc::GetOutputType (int size, EnumDSSDataType *inputType, EnumDSSDataType *outputType)
{
    if(outputType == NULL || inputType == NULL) 
		return E_POINTER;
    
	// jxie, 2011-1-28, TQMS 454172
	switch (*inputType)
	{
		case DssDataTypeInteger:
		case DssDataTypeShort:
		case DssDataTypeLong:
			*outputType = DssDataTypeDouble;
			break;
		default:
			*outputType = *inputType;
			break;
	}
    
	// jxie, 2011-1-18, TQMS 450450
	mOutputType = *outputType;
	
	return S_OK;
}
int DSSSimpleOperatorFunc::DirectCalculate(Int32 nParam, Int32* ipSize, 
                            Int32** ipOffsets, void** ipData, double* ipDivider, 
                            DSSDataFlag** ipFlags, VARIANT_BOOL* ipAllDataValid,
                            DSSDataType_Type* iType, DSSXTabBitVector* ipMask,
                            Int32 nResSize, double* opData, DSSDataFlag* opFlag, VARIANT_BOOL* opAllDataValid,
                            EnumNullCheckingOption iNullCheckingOption)
{
    HRESULT hr = S_OK;
    
	// translate Add/Multiply to Plus/Times if more than 2 inputs
	if ((mFunctionIndex == DssFuncAdd || mFunctionIndex == DssFuncMultiply) && nParam > 2)
	{
		EnumSimpleOperatorType lOperatorType = DssFuncPlus;
		if (mFunctionIndex == DssFuncMultiply)
		{
			lOperatorType = DssFuncTimes;
		}
        
		hr = CDSSSimpleFunctionHelper::DirectCalculate(ipData[0], ipData[1], ipOffsets[0], ipOffsets[1], iType[0], iType[1], ipSize[0], ipSize[1], ipDivider[0], ipDivider[1], ipFlags[0], ipFlags[1], ipAllDataValid[0], ipAllDataValid[1], ipMask, nResSize, opData, opFlag, opAllDataValid, iNullCheckingOption, lOperatorType);
		CHECK_HR;
        
		for (Int32 i = 2; i < nParam; i++)
		{
			hr = CDSSSimpleFunctionHelper::DirectCalculate(opData, ipData[i], ipOffsets[0], ipOffsets[1], DssDataTypeDouble, iType[i], nResSize, ipSize[i], 1, ipDivider[i], opFlag, ipFlags[i], *opAllDataValid, ipAllDataValid[i], ipMask, nResSize, opData, opFlag, opAllDataValid, iNullCheckingOption, lOperatorType);
			CHECK_HR;
		}
	}
	else
	{
		AE_ASSERT(nParam == 2);
		hr = CDSSSimpleFunctionHelper::DirectCalculate(ipData[0], ipData[1], ipOffsets[0], ipOffsets[1], iType[0], iType[1], ipSize[0], ipSize[1], ipDivider[0], ipDivider[1], ipFlags[0], ipFlags[1], ipAllDataValid[0], ipAllDataValid[1], ipMask, nResSize, opData, opFlag, opAllDataValid, iNullCheckingOption, mFunctionIndex);
		CHECK_HR;
	}
	
	return S_OK;
}

bool DSSSimpleOperatorFunc::SupportDirectCalculate()
{    
    switch (mFunctionIndex) {
        case DssFuncPlus:
        case DssFuncAdd:
        case DssFuncMinus:
        case DssFuncTimes:
        case DssFuncMultiply:
        case DssFuncDivide:
            return true;
            break;
        default:
            break;
    }
    
    return false;
}
//tliao, 822148, 2014/09/23, support cell format data calculation
int DSSSimpleOperatorFunc::CalculateCellData(Int32 nParam, CComVariant * pParam, CComVariant * pResult, DSSDataFlag * pFlag)
{
	int hr;
    
	if (nParam < 1)
		return E_FAIL;
    
	if (!pParam || !pResult || !pFlag)
		return E_POINTER;
    
	if ((pParam->vt & (VT_ARRAY | VT_VARIANT)) != (VT_ARRAY | VT_VARIANT))
		return E_FAIL;
    
	vector<double> lvValues(nParam);
	vector<EnumCellFormatDataTypes> lvTypes(nParam);
	vector<DSS_ID> lvFormatIDs(nParam);
    
	hr=CellDataHelper::SplitCellData(nParam, pParam, lvValues, lvTypes, lvFormatIDs);
	CHECK_HR;
    
	*pFlag=DssDataOk;
    
	double lResult;
	hr=Calculate(lvValues.size(), &lvValues[0], &lResult, pFlag);
	CHECK_HR;
    
	EnumCellFormatDataTypes lResultType;
	DSS_ID lResultFormatID;
	hr=CalculateCellTypeAndFormat(lvTypes.size(), &lvTypes[0], &lvFormatIDs[0], &lResultType, &lResultFormatID);
	CHECK_HR;
    
	hr=CellDataHelper::CreateVariantFromCellData(lResult, lResultType, lResultFormatID, pResult);
	CHECK_HR;
    
	return S_OK;
}


int DSSSimpleOperatorFunc::CalculateCellTypeAndFormat(Int32 nParam, EnumCellFormatDataTypes *pTypes, DSS_ID *pFormatIDs, EnumCellFormatDataTypes *oResultType, DSS_ID *oResultFormatID)
{
	switch (mFunctionIndex)
	{
		case DssFuncUnaryMinus:
		case DssFuncAbs:
		{
			*oResultType=pTypes[0];
			*oResultFormatID=pFormatIDs[0];
			break;
		}
            
		case DssFuncPlus:
		case DssFuncMinus:
		case DssFuncAdd:	// aye, 339844, 1/9/2008
		case DssFuncAverage:
		{
			Int32 i;
			*oResultType=DssCellFormatDataTypeMIX;
			*oResultFormatID=CellDataHelper::GetMixFormatID();
            
			for(i=0; i<nParam; ++i)
			{
				if(i==0)
					*oResultType=pTypes[0];
				else if(*oResultType!=pTypes[i]) // A+B or A+const or const+A
				{
					if(*oResultType==DssCellFormatDataTypeDouble) // const+A
						*oResultType=pTypes[i];
					else if(pTypes[i]!=DssCellFormatDataTypeDouble) // A+B
					{
						*oResultType=DssCellFormatDataTypeMIX;
						break;
					}
				}
			}
            
			for(i=0; i<nParam; ++i)
			{
				if(i==0)
					*oResultFormatID=pFormatIDs[0];
				else if(!ATL::InlineIsEqualGUID(*oResultFormatID,pFormatIDs[i]))  // A+B or A+const or const+A
				{
					if(ATL::InlineIsGUIDNULL(*oResultFormatID))  // const+A
						*oResultFormatID=pFormatIDs[i];
					else if(!InlineIsGUIDNULL(pFormatIDs[i]))  // A+B
					{
						*oResultFormatID=CellDataHelper::GetMixFormatID();
						break;
					}
				}
			}
			break;
		}
            
		case DssFuncTimes:
		case DssFuncMultiply:
		{
			Int32 i;
			*oResultType=DssCellFormatDataTypeMIX;
			*oResultFormatID=CellDataHelper::GetMixFormatID();
            
			for(i=0; i<nParam; ++i)
			{
				if(i==0)
					*oResultType=pTypes[0];
				else if(*oResultType!=pTypes[i]) // A*B or const*A or A*const
				{
					if(*oResultType==DssCellFormatDataTypeDouble) // const*A
						*oResultType=pTypes[i];
					else if(pTypes[i]!=DssCellFormatDataTypeDouble) // A*B
					{
						*oResultType=DssCellFormatDataTypeMIX;
						break;
					}
				}
				else if(pTypes[i]!=DssCellFormatDataTypeDouble) // A*A
				{
					*oResultType=DssCellFormatDataTypeMIX;
					break;
				}
			}
            
			for(i=0; i<nParam; ++i)
			{
				if(i==0)
					*oResultFormatID=pFormatIDs[0];
				else if(!ATL::InlineIsEqualGUID(*oResultFormatID,pFormatIDs[i]))  // A*B or A*const or const*A
				{
					if(ATL::InlineIsGUIDNULL(*oResultFormatID))  // const*A
						*oResultFormatID=pFormatIDs[i];
					else if(!InlineIsGUIDNULL(pFormatIDs[i]))  // A*B
					{
						*oResultFormatID=CellDataHelper::GetMixFormatID();
						break;
					}
				}
				else if(!InlineIsGUIDNULL(pFormatIDs[i]))  // A*A
				{
					*oResultFormatID=CellDataHelper::GetMixFormatID();
					break;
				}
			}
			break;
		}
            
		case DssFuncDivide:
		{
			Int32 i;
			*oResultType=DssCellFormatDataTypeMIX;
			*oResultFormatID=CellDataHelper::GetMixFormatID();
            
			for(i=0; i<nParam; ++i)
			{
				if(i==0)
					*oResultType=pTypes[0];
				else if(*oResultType!=pTypes[i]) // A/B or const/A or A/const
				{
					if(*oResultType==DssCellFormatDataTypeDouble || pTypes[i]!=DssCellFormatDataTypeDouble) // const/A or A/B
					{
						*oResultType=DssCellFormatDataTypeMIX;
						break;
					}
				}
				else if(pTypes[i]!=DssCellFormatDataTypeDouble) // A/A
					*oResultType=DssCellFormatDataTypeDouble;
			}
            
			for(i=0; i<nParam; ++i)
			{
				if(i==0)
					*oResultFormatID=pFormatIDs[0];
				else if(!ATL::InlineIsEqualGUID(*oResultFormatID,pFormatIDs[i]))  // A/B or A/const or const/A
				{
					if(ATL::InlineIsGUIDNULL(*oResultFormatID) || !InlineIsGUIDNULL(pFormatIDs[i]))  // const/A or A/B
					{
						*oResultFormatID=CellDataHelper::GetMixFormatID();
						break;
					}
				}
				else if(!InlineIsGUIDNULL(pFormatIDs[i]))  // A/A
				{
					*oResultFormatID=CellDataHelper::GetDefaultFormatID();
					break;
				}
			}
			break;
		}
            
		default:
		{
			*oResultType=DssCellFormatDataTypeDouble;
			*oResultFormatID=CellDataHelper::GetDefaultFormatID();
			break;
		}
	}
	return S_OK;
}
