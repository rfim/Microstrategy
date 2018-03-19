//
//  DSSSimpleFunctionHelper.cpp
//  ReportServiceCore
//
//  Created by Yang Zhuo on 7/20/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//

#include "DSSSimpleFunctionHelper.h"

int DSSSimpleFunctionHelper::DirectCalculate(void* ipData1, void* ipData2, 
                                             EnumDSSDataType iType1, EnumDSSDataType iType2,
                                             Int32 iSize1, Int32 iSize2,
                                             double iDivider1, double iDivider2, 
                                             DSSDataFlag* ipFlag1, DSSDataFlag* ipFlag2,
                                             VARIANT_BOOL iAllValid1, VARIANT_BOOL iAllValid2,
                                             /*CDSSXTabBitVector* ipMask, */
                                             Int32 nResSize, double* opData, DSSDataFlag* opFlag, 
                                             VARIANT_BOOL* opAllValid, EnumNullCheckingOption iNullCheckingOption, 
                                             EnumDSSFunction iOperatorType)
{
    int hr = S_OK;
    
    bool lAllValid1 = VARIANT_TRUE == iAllValid1 ? true : false;
    bool lAllValid2 = VARIANT_TRUE == iAllValid2 ? true : false;
    
    double lDivider1 = iDivider1 > 1.0 ? iDivider1 : 0;
    double lDivider2 = iDivider2 > 1.0 ? iDivider2 : 0;
    
    bool lbAllValid = true;
    
    switch(iType1)
	{
        case DssDataTypeNumeric:
        case DssDataTypeDecimal:
        case DssDataTypeDouble:
            switch(iType2)
		{
            case DssDataTypeNumeric:
            case DssDataTypeDecimal:
            case DssDataTypeDouble:
                hr = hDirectCalculate<double, double>((double*)ipData1, (double*)ipData2, iSize1, iSize2,
                                                      lDivider1, lDivider2, ipFlag1, ipFlag2, lAllValid1, lAllValid2, 
                                                      nResSize, opData, opFlag, lbAllValid, iNullCheckingOption,
                                                      iOperatorType);
                break;
                
            case DssDataTypeLong:
            case DssDataTypeInteger:
                hr = hDirectCalculate<double, Int32>((double*)ipData1, (Int32*)ipData2, iSize1, iSize2,
                                                     lDivider1, lDivider2, ipFlag1, ipFlag2, lAllValid1, lAllValid2, 
                                                     nResSize, opData, opFlag, lbAllValid, iNullCheckingOption, 
                                                     iOperatorType);
                break;
                
            case DssDataTypeShort:
                hr = hDirectCalculate<double, short>((double*)ipData1, (short*)ipData2, iSize1, iSize2,
                                                     lDivider1, lDivider2, ipFlag1, ipFlag2, lAllValid1, lAllValid2, 
                                                     nResSize, opData, opFlag, lbAllValid, iNullCheckingOption, 
                                                     iOperatorType);
                break;
            default:
                AE_ASSERT(false);
		}
            break;
            
        case DssDataTypeLong:
        case DssDataTypeInteger:
            switch(iType2)
		{
            case DssDataTypeNumeric:
            case DssDataTypeDecimal:
            case DssDataTypeDouble:
                hr = hDirectCalculate<Int32, double>((Int32*)ipData1, (double*)ipData2, iSize1, iSize2,
                                                     lDivider1, lDivider2, ipFlag1, ipFlag2, lAllValid1, lAllValid2, 
                                                     nResSize, opData, opFlag, lbAllValid, iNullCheckingOption, 
                                                     iOperatorType);
                break;
                
            case DssDataTypeLong:
            case DssDataTypeInteger:
                hr = hDirectCalculate<Int32, Int32>((Int32*)ipData1, (Int32*)ipData2, iSize1, iSize2,
                                                    lDivider1, lDivider2, ipFlag1, ipFlag2, lAllValid1, lAllValid2, 
                                                    nResSize, opData, opFlag, lbAllValid, iNullCheckingOption, 
                                                    iOperatorType);
                break;
                
            case DssDataTypeShort:
                hr = hDirectCalculate<Int32, short>((Int32*)ipData1, (short*)ipData2, iSize1, iSize2,
                                                    lDivider1, lDivider2, ipFlag1, ipFlag2, lAllValid1, lAllValid2, 
                                                    nResSize, opData, opFlag, lbAllValid, iNullCheckingOption, 
                                                    iOperatorType);
                break;
            default:
                AE_ASSERT(false);
		}
            break;
        case DssDataTypeShort:
            switch(iType2)
		{
            case DssDataTypeNumeric:
            case DssDataTypeDecimal:
            case DssDataTypeDouble:
                hr = hDirectCalculate<short, double>((short*)ipData1, (double*)ipData2, iSize1, iSize2,
                                                     lDivider1, lDivider2, ipFlag1, ipFlag2, lAllValid1, lAllValid2, 
                                                     nResSize, opData, opFlag, lbAllValid, iNullCheckingOption, 
                                                     iOperatorType);
                break;
                
            case DssDataTypeLong:
            case DssDataTypeInteger:
                hr = hDirectCalculate<short, Int32>((short*)ipData1, (Int32*)ipData2, iSize1, iSize2,
                                                    lDivider1, lDivider2, ipFlag1, ipFlag2, lAllValid1, lAllValid2, 
                                                    nResSize, opData, opFlag, lbAllValid, iNullCheckingOption, 
                                                    iOperatorType);
                break;
                
            case DssDataTypeShort:
                hr = hDirectCalculate<short, short>((short*)ipData1, (short*)ipData2, iSize1, iSize2,
                                                    lDivider1, lDivider2, ipFlag1, ipFlag2, lAllValid1, lAllValid2, 
                                                    nResSize, opData, opFlag, lbAllValid, iNullCheckingOption,
                                                    iOperatorType);
                break;
            default:
                AE_ASSERT(false);
		}
            break;
        default:
            AE_ASSERT(false);
	}
    
	*opAllValid = lbAllValid ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}