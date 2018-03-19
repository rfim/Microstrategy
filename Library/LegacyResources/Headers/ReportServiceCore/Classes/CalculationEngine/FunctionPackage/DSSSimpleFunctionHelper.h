//
//  DSSSimpleFunctionHelper.h
//  ReportServiceCore
//
//  Created by Yang Zhuo on 7/20/11.
//  Copyright 2011 Microstrategy. All rights reserved.
//

#ifndef DSSSIMPLEFUNCTIONHELPER_H
#define DSSSIMPLEFUNCTIONHELPER_H

#include "DSSSimpleFunction.h"
#include "DSSFunctionUtility.h"

class DSSSimpleFunctionHelper
{
public:
    static int DirectCalculate(void* ipData1, void* ipData2, 
                               EnumDSSDataType iType1, EnumDSSDataType iType2,
                               Int32 iSize1, Int32 iSize2,
                               double iDivider1, double iDivider2, 
                               DSSDataFlag* ipFlags1, DSSDataFlag* ipFlags2,
                               VARIANT_BOOL iAllValid1, VARIANT_BOOL iAllValid2,
                               /*CDSSXTabBitVector* ipMask, */
                               Int32 nResSize, double* opData, DSSDataFlag* opFlag, 
                               VARIANT_BOOL* opAllValid, EnumNullCheckingOption iNullCheckingOption, 
                               EnumDSSFunction iOperatorType);
private:
    
	template <class Type1, class Type2>
	static int hDirectCalculate(Type1* ipParam1, Type2* ipParam2, 
                                Int32 iSize1, Int32 iSize2,
                                double iDivider1, double iDivider2, 
                                DSSDataFlag* ipFlags1, DSSDataFlag* ipFlags2,
                                bool ibAllValid1, bool ibAllValid2,
                                //CDSSXTabBitVector* ipMask, 
                                Int32 nResSize, double* opData, DSSDataFlag* opFlag, bool& oAllValid,
                                EnumNullCheckingOption iNullCheckingOption, EnumDSSFunction iOperatorType);
};

template <class Type1, class Type2>
int DSSSimpleFunctionHelper::hDirectCalculate(Type1* ipParam1, Type2* ipParam2, 
                                              Int32 iSize1, Int32 iSize2,
                                              double iDivider1, double iDivider2, 
                                              DSSDataFlag* ipFlags1, DSSDataFlag* ipFlags2,
                                              bool ibAllValid1, bool ibAllValid2,
                                              //CDSSXTabBitVector* ipMask, 
                                              Int32 nResSize, double* opData, DSSDataFlag* opFlag, bool& oAllValid,
                                              EnumNullCheckingOption iNullCheckingOption, EnumDSSFunction iOperatorType)
{
    int hr = S_OK;
    enum MathError error = MathNoError;
    
    DSSDataFlag lFlag1 = DssDataOk;
    DSSDataFlag lFlag2 = DssDataOk;
    
    bool lfCheckValue = true;
    
    switch (iOperatorType)
	{
		case DssFunctionEquals:
		case DssFunctionNotEqual:
		case DssFunctionGreater:
		case DssFunctionLess:
		case DssFunctionGreaterEqual:
		case DssFunctionLessEqual:
			lfCheckValue = false;
			break;
		default:
			break;
	}
    
    memset(opFlag, DssDataOk, nResSize * sizeof(DSSDataFlag));
    
    double lVal1 = iDivider1 ? (ipParam1[0]/iDivider1) : ipParam1[0];
	double lVal2 = iDivider2 ? (ipParam2[0]/iDivider2) : ipParam2[0];
	
	bool lConst1 = (iSize1 == 1);
	bool lConst2 = (iSize2 == 1);
    
    for(int iRow = 0; iRow < nResSize; ++iRow)
    {
        if(!lConst1)
        {
            lVal1 = iDivider1 ? (ipParam1[iRow]/iDivider1) : ipParam1[iRow];
        }
        
        if(!lConst2)
        {
            lVal2 = iDivider2 ? (ipParam2[iRow]/iDivider2) : ipParam2[iRow];
        }
        
        switch (iOperatorType) {
            case DssFunctionEquals:
                opData[iRow] = DoubleEqual(lVal1, lVal2);
                break;
                
            case DssFunctionNotEqual:
                opData[iRow] = !DoubleEqual(lVal1, lVal2);
                break;
                
            case DssFunctionGreater:
                opData[iRow] = DoubleGreaterThan(lVal1, lVal2);
                break;
                
            case DssFunctionLess:
                opData[iRow] = DoubleLessThan(lVal1, lVal2);
                break;
                
            case DssFunctionGreaterEqual:
                opData[iRow] = DoubleGreaterEqual(lVal1, lVal2);
                break;
                
            case DssFunctionLessEqual:
                opData[iRow] = DoubleLessEqual(lVal1, lVal2);
                break;
                
            case DssFunctionAdd:
            case DssFunctionPlus:
                opData[iRow] = lVal1 + lVal2;
                break;
                
            case DssFunctionMinus:
                opData[iRow] = lVal1 - lVal2;
                break;
                
            case DssFunctionTimes:
            case DssFunctionMultiply:
                opData[iRow] = lVal1 * lVal2;
                break;
                
            case DssFunctionDivide:
                if(!DoubleEqual(lVal2, 0))   //use DoubleEqual function to see whether lVar2 is equal to 0
                {
                    opData[iRow] = lVal1 / lVal2;
                }
                else
                {
                    opData[iRow] = 0;
                    opFlag[iRow] = DssDataZerodivide;
                    oAllValid = false;
                }
                break;
                
            default:
                assert(false);
                return E_FAIL;
        }//switch (iOperatorType)
        
        if(lfCheckValue)
        {
            CheckValue(&error, opData[iRow]);
            
            if(MathNoError != error)
            {
                opData[iRow] = 0;
                opFlag[iRow] = DssDataInvalid;
                error = MathNoError;
            }
        }
        
    }//for(int iRow = 0; iRow < nResSize; ++iRow)
    
    if((ibAllValid1 && ibAllValid2) || DssNumericErrorTreatAs == iNullCheckingOption)
    {
        
    }
    else
    {
        lFlag1 = ipFlags1[0];
        lFlag2 = ipFlags2[0];
        
        DSSDataFlag lFlag = DssDataUnknown;
        
        for(Int32 iRow = 0; iRow < nResSize; ++iRow)
        {
            if(!lConst1)
            {
                lFlag1 = ipFlags1[iRow];
            }
            if(!lConst2)
            {
                lFlag2 = ipFlags2[iRow];
            }
            
            if(DssDataOk == lFlag1 && DssDataOk == lFlag2)
            {
                continue;
            }
            
            lFlag = (lFlag1 != DssDataOk) ? lFlag1 : lFlag2;
            
            if(DssFunctionEquals == iOperatorType || DssFunctionNotEqual == iOperatorType)
            {
                if(DssDataOk != lFlag1 && DssDataOk == lFlag2)
                {
                    lFlag = DssDataNull;
                }
                else
                {
                    if ((iOperatorType == DssFunctionEquals && lFlag1 == lFlag2) ||
						(iOperatorType == DssFunctionNotEqual && lFlag1 != lFlag2))
                    {
                        opData[iRow] = 1.0;
                    }
					lFlag = DssDataOk;
                }
            }
            
            if(DssDataOk != lFlag)
            {
                opData[iRow] = 0;
                oAllValid = false;
            }
            
            opFlag[iRow] = lFlag;
        }
    }
    
    return hr;
}

#endif