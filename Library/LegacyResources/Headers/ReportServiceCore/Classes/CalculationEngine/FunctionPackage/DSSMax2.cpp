//==============================================================================================
//	FILENAME	:	DSSMax2.cpp
//	AUTHOR		:	Xiaonan Han
//	CREATION	:	04/12/2001
//	Copyright (C) MicroStrategy Incorporated 2001
//	All Rights Reserved
//==============================================================================================

#include "DSSMax2.h"

#import "DFCDataConversion.h"
#import "AnalyticalEngine/DSSAEHelper.h"
#import "DSSFunctionUtility.h"

DSSMax2::DSSMax2()
{
}

DSSMax2::~DSSMax2()
{
}

int DSSMax2::Calculate (int nParam, CComVariant *ipParams, CComVariant *opResult, DSSDataFlag *opFlag)
{
    if(!ipParams || !opResult || !opFlag) 
		return E_POINTER;
    
	if ( (ipParams->vt & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT))
		return E_FAIL;
    
	if (nParam < 1) 
		return E_FAIL;
    
	DSSDataFlag lFlag = DssDataOk;
	CComVariant v1;
    
	getVariant(ipParams, 0, v1);
    
	for (int i = 1; i < nParam; i ++)
    {
		CComVariant v2;
		getVariant(ipParams, i, v2);
        
		int result = variantCompare(v1, v2, &lFlag);
        
		if (lFlag == DssDataOk) 
        { 
            //  3/22/2012 tehe
			if ( (mFunctionIndex == DssFuncMax2 && result < 0) || //v1 < v2
				(mFunctionIndex == DssFuncMin2 && result > 0)) //v1 > v2
				//copy v2 to v1
				VariantCopy(&v1, &v2);
		} else 
        {
			break;
		}        
	}
    
	if (lFlag == DssDataOk)	
    {
		VariantCopy(opResult, &v1);
	}
    
	*opFlag = lFlag;    
    
	return  S_OK; 
} 

int DSSMax2::GetOutputType (int size, EnumDSSDataType *inputType, EnumDSSDataType *outputType)
{
 	if(outputType == NULL || inputType == NULL) 
		return E_POINTER;
    
	if (*inputType == DssDataTypeMBChar || *inputType == DssDataTypeUTF8Char)	// weiwang 2009-06-05 TQMS352644 UTF8 support
		*outputType = DssDataTypeVarChar;
	else if (*inputType == DssDataTypeShort || *inputType == DssDataTypeInteger ||
             *inputType == DssDataTypeLong)		// jxie, 2010-12-17, TQMS 446484
		*outputType = DssDataTypeDouble;
	else
		*outputType = *inputType;
    
    
	return S_OK;
}

int DSSMax2::Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag)
{
	if(!ipParams || !opResult || !opFlag) 
        return E_POINTER;
    
	if (nParam < 1) 
		return E_FAIL;
    
    /*
     DSSData_Flags lFlag = DssDataOk;
     double v1 = pParam[0];
     
     for (Int32 i = 1; i < nParam; i ++){
     double v2 = pParam[i];
     
     if ( (mFunctionIndex == DssFuncMax2 && v1 < v2) || //v1 < v2
     (mFunctionIndex == DssFuncMin2 && v1 > v2)) //v1 > v2
     //copy v2 to v1
     v1 = v2;
     }
     
     if (lFlag == DssDataOk)	{
     *pResult = v1;
     }
     
     *pFlag = lFlag;
     */
    
    // jimzhang June/29/2011 begin
    int lIndex = 0;
    for (int i = 1; i < nParam; i++) 
    {
        if ((ipParams[lIndex] < ipParams[i] && mFunctionIndex == DssFuncMax2) ||
            (ipParams[lIndex] > ipParams[i] && mFunctionIndex == DssFuncMin2))
        {
            lIndex = i; 
        }
    }    
    
    *opResult = ipParams[lIndex];
    *opFlag = DssDataOk;
    // jimzhang June/29/2011 end
    
	return S_OK;
}

int DSSMax2::setProperty (Int32 Index, CComVariant *pValue)
{
	return E_NOTIMPL;
}

//int DSSMax2::Init (EnumDSSFunction iFunctionIndex)
//{
//  	mFunctionIndex = iFunctionIndex;
//    
//	return S_OK;
//}
