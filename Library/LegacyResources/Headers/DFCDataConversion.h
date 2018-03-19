//
//  DFCDataConversion.h
//  ReportServiceCore
//
//  Created by Zhang Jia on 7/18/11.
//  Copyright 2011 MicroStrategy Beijing R&D Center. All rights reserved.
//

#ifndef DFCDataConversion_h
#define DFCDataConversion_h 1

#import "common/DSSDateTime.h"
#import "common/DSSIPhoneEnums.h"
#import "DataType/DataType/Variant.h"
#include "MsiCOM/MsiCOM/Msi_variant.h"

class CComVariant;
class DFCDataConversion {
public:
    static int ConvertFromVariant(CComVariant *pVar, EnumDSSDataType iType, unsigned char *ppData, int *pcbData);

    static int ConvertToVariant(unsigned char *ipData, EnumDSSDataType iType, int icbDataSize, CComVariant *opVar);

    static int VariantSize(CComVariant *pVar, EnumDSSDataType iType, int *pcbData);

    static bool IsDataTypeCompatible(EnumDSSDataType typeA, EnumDSSDataType typeB);
    
    static int ConvertToCommonType(EnumDSSDataType iType, EnumDSSDataType &oType);
    
    // deliu, 2012-04-20
    static int DFCVariantChange2BSTR(CComVariant * pvargDest,CComVariant * pvargSrc,EnumDSSDataType *pDataType);
    
    
    static int MsiVariantToVARIANT(const MDataType::Variant& iVariant, VARIANT& oVariant);
    
    static int TolocalVARIANT(const MDataType::DateTime& irOriginal, VARIANT & orVal, bool ibIsGmt);
    
    static void checkDispatchType(CComVariant* v, bool* oIsBigDecimal, bool* oIsCellFormatData, bool* oIsDate);
};

#endif