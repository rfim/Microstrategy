//==============================================================================================
//	FILENAME	:	DSSMax2.h
//	AUTHOR		:	Xiaonan Han
//	CREATION	:	01/29/2002
//	Copyright (C) MicroStrategy Incorporated 2002
//	All Rights Reserved
//==============================================================================================


#ifndef Max2_h
#define Max2_h 1

#import "CalculationEngine/DSSNNSimpleFunction.h" // jimzhang June/30/2011 

// For supporting VARIANT
#import "PDCHeader/PDCwinnt.h"
#import "MsiCOM/MsiCOM/Msi_atldef.h"
#import "MsiCOM/MsiCOM/Msi_objbase.h"
#import "MsiCOM/MsiCOM/Msi_oleauto.h"
#import "MsiCOM/MsiCOM/Msi_ccomvariant.h"


class DSSMax2 : public DSSNNSimpleFunction
{
    
public:
    DSSMax2();
    
    ~DSSMax2();
    
    int Calculate (int nParam, CComVariant *pParam, CComVariant *pResult, DSSDataFlag *pFlag);
    
    int GetOutputType (int size, EnumDSSDataType *inputType, EnumDSSDataType *outputType);
    
    int Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag);
    
    virtual int setProperty (int Index, CComVariant *pValue);
    
    // int Init (EnumDSSFunction iFunctionIndex);
};

#endif
