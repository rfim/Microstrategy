//
//  DSSRWFieldGroupCalculator.h
//  ReportServiceCore
//
//  Created by Jia Zhang on 9/19/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSRWFieldGroupCalculator_h
#define ReportServiceCore_DSSRWFieldGroupCalculator_h

//#import "DSSRWNodeModelIterator.h"
//#import "DSSRWFieldGroupIterator.h"
#import "DSSAEHelper.h"
#import "DSSRWFieldGroupIterator.h"
//#import "DSSRWDHelper.h"
//#import "../../../Common/MsiCOM/MsiCOM/Msi_variant.h"
#import "PDCHeader/PDCwinnt.h"
#import "MsiCOM/MsiCOM/Msi_atldef.h"
#import "MsiCOM/MsiCOM/Msi_objbase.h"
#import "MsiCOM/MsiCOM/Msi_oleauto.h"
#import "MsiCOM/MsiCOM/Msi_ccomvariant.h"

class DSSField;
class DSSFunctionServer;
class DSSExpNode;   
class DSSExpNodeOperator;
class DSSFunctionObject;

class DSSRWFieldGroupCalculator
{
public:
    static int Calculate (DSSRWFieldGroupIterator *ipIterator, DSSExpNode *ipNode, DSSField *ipField, DSSFunctionServer *ipFunctionServer, 
                          AERawData *opRawData);
    
private:
    static int hCalculateFunctionIn (DSSRWFieldGroupIterator *ipIterator, DSSExpNode *ipNode, DSSField *ipField, DSSFunctionServer *ipFunctionServer, AERawData *opRawData);
    
    static inline bool hIsNumerical(VARTYPE iVarType)
	{
		return (iVarType == VT_I2 || iVarType == VT_I4 || iVarType == VT_R4 || iVarType == VT_R8 
                || iVarType == VT_BOOL || iVarType == VT_I1 || iVarType == VT_UI1 || iVarType == VT_UI2 
                || iVarType == VT_UI4 || iVarType == VT_I8 || iVarType == VT_UI8 || iVarType == VT_INT || iVarType == VT_UINT);
	}
    
    static inline bool hIsNumerical(EnumDSSDataType iType)
    {
        if (iType == DssDataTypeTime || iType == DssDataTypeDate || iType == DssDataTypeTimeStamp
            || iType == DssDataTypeBigDecimal || iType == DssDataTypeCellFormatData
            || iType == DssDataTypeMBChar || iType == DssDataTypeUTF8Char || iType == DssDataTypeVarChar || iType == DssDataTypeNVarChar)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    
    static int hCheckBigDecimal (CComVariant * opValue, DSSDataFlag *opValueFlag);
    
    static int hBigDecimalToDouble(CComVariant *ipVar, double &orDouble);
};

#endif
