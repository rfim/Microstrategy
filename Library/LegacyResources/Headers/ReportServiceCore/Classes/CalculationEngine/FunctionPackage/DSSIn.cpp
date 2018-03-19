//
//  DSSIn.cpp
//  ReportServiceCore
//
//  Created by Yang Zhuo on 7/26/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//

#import "DSSIn.h"
#import "VariantCommonInclude.h"
#import "DSSFunctionUtility.h"
#import "DFCDataConversion.h"

DSSIn::DSSIn()
{
}


DSSIn::~DSSIn()
{
}

int DSSIn::Calculate (Int32 nParam, CComVariant *pParam, CComVariant *pResult, DSSDataFlag *pFlag)
{
  	if(!pParam || !pResult || !pFlag) 
		return E_POINTER;
    
	if ( (pParam->vt & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT))
		return E_FAIL;
    
	if (nParam < 2) 
		return E_FAIL;
    
    
	*pFlag = DssDataOk;
	bool result = false;
	CComVariant  v1, v2;
    
	getVariant(pParam, 0, v1);
    
	//test if Param[0] in (Param[1], ... Param[n-1])
	for (Int32 i = 1; i < nParam; i ++){
		getVariant(pParam, i, v2);
        *pFlag = DssDataOk;
		if (variantCompare(v1, v2, pFlag) == 0 && *pFlag == DssDataOk) { //found
			result = true;
			break;
		}
		
	}
    
	*pFlag = DssDataOk;
    
	if (mFunctionIndex == DssFuncNotIn)
		result = ((result == true) ? false : true);
    
    DFCDataConversion::ConvertToVariant((BYTE *)(&result), DssDataTypeBool, 0, pResult);
    
	return  S_OK; 
}

int DSSIn::setProperty (Int32 Index, CComVariant *pValue)
{
	return E_NOTIMPL;
}

//int DSSIn::Init (EnumDSSFunction iFunctionIndex)
//{
//  	mFunctionIndex = iFunctionIndex;
//	return S_OK;
//}

int DSSIn::GetOutputType (Int32 /*size*/, EnumDSSDataType * /*inputType*/, EnumDSSDataType *outputType)
{
 	if(!outputType) 
		return E_POINTER;
    
	*outputType = DssDataTypeBool;
    
	return S_OK;

}

int DSSIn::Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag)
{
    if(!ipParams || !opResult || !opFlag) 
		return E_POINTER;
        
	if (nParam < 2) 
		return E_FAIL;
    
	*opFlag = DssDataOk;
	bool result = false;
	//test if Param[0] in (Param[1], ... Param[n-1])
	for (Int32 i = 1; i < nParam; i ++)
    {
        if (DoubleEqual(ipParams[0], ipParams[i]))
        {
			result = true;
			break;
		}
	}
        
	if (mFunctionIndex == DssFuncNotIn)
		result = ((result == true) ? false : true);
    
    int hr = DSSAEHelper::ConvertToNumeric(&result, sizeof(result), DssDataTypeBool, *opFlag, *opResult);
    if (S_OK != hr)
        *opFlag = DssDataInvalid;
    
	return  hr;
}
