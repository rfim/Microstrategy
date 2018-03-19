/*
 *  DSSSimpleFunction.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 2/4/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef SimpleFunction_h
#define SimpleFunction_h 1

#import "CalculationEngine/DSSFunctionObject.h"
#import "DSSFunctionUtility.h"
#import "DSSXTabBitVector.h"

class DSSXTabBitVector;
class DSSSimpleFunction : public DSSFunctionObject
{
public:
	DSSSimpleFunction();
	
	virtual ~DSSSimpleFunction();
	
	inline virtual EnumDSSFunctionType getFunctionType(){return DssFunctionTypeSimple;};
	
	virtual int Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag) = 0;
	
	int CalculateRepeat(int nParam, int nRepeat, double **ipParams, double *opResult, DSSDataFlag *opFlag);
    
    virtual bool SupportDirectCalculate();
    
    virtual int DirectCalculate(Int32 nParam, Int32* ipSize, 
                                Int32** ipOffsets, void** ipData, double* ipDivider, 
                                DSSDataFlag** ipFlags, VARIANT_BOOL* ipAllDataValid,
                                DSSDataType_Type* iType, DSSXTabBitVector* ipMask,
                                Int32 nResSize, double* opData, DSSDataFlag* opFlag, VARIANT_BOOL* opAllDataValid,
                                EnumNullCheckingOption iNullCheckingOption);
};

// qjiang 08/12/2009
// direct calculate for simple functions have similar operations, add this helper class to avoid too much duplication
class CDSSSimpleFunctionHelper
{
public:
    
	// common entrance of direct calculate for simple functions
	static int DirectCalculate(void* ipData1, void* ipData2, 
                                Int32* ipOffset1, Int32* ipOffset2,  
                                DSSDataType_Type iType1, DSSDataType_Type iType2,
                                Int32 iSize1, Int32 iSize2,
                                double iDivider1, double iDivider2, 
                                DSSDataFlag* ipFlags1, DSSDataFlag* ipFlags2,
                                VARIANT_BOOL iAllValid1, VARIANT_BOOL iAllValid2,
                                DSSXTabBitVector* ipMask, Int32 nResSize, double* opData, DSSDataFlag* opFlag, VARIANT_BOOL* opAllValid,
                                EnumNullCheckingOption iNullCheckingOption, EnumSimpleOperatorType iOperatorType);
private:
    
	template <class Type1, class Type2>
	static int hDirectCalculate(Type1* ipParam1, Type2* ipParam2, 
                                Int32* ipOffset1, Int32* ipOffset2,  
                                Int32 iSize1, Int32 iSize2,
                                double iDivider1, double iDivider2, 
                                DSSDataFlag* ipFlags1, DSSDataFlag* ipFlags2,
                                bool ibAllValid1, bool ibAllValid2,
                                DSSXTabBitVector* ipMask, Int32 nResSize, double* opData, DSSDataFlag* opFlag, bool& oAllValid,
                                EnumNullCheckingOption iNullCheckingOption, EnumSimpleOperatorType iOperatorType);
};

// 03/16/2011 add support on compressed data
template <class Type1, class Type2>
int CDSSSimpleFunctionHelper::hDirectCalculate(Type1* ipParam1, Type2* ipParam2, 
                                                Int32* ipOffset1, Int32* ipOffset2,	  
                                                Int32 iSize1, Int32 iSize2,
                                                double iDivider1, double iDivider2, 
                                                DSSDataFlag* ipFlags1, DSSDataFlag* ipFlags2,
                                                bool ibAllValid1, bool ibAllValid2,
                                                DSSXTabBitVector* ipMask, Int32 nResSize, double* opData, DSSDataFlag* opFlag, bool& oAllValid,
                                                EnumNullCheckingOption iNullCheckingOption, EnumSimpleOperatorType iOperatorType)
{
	HRESULT hr = S_OK;
    
	enum MathError error = MathNoError;
    
	DSSDataFlag lFlag1 = DssDataOk;
	DSSDataFlag lFlag2 = DssDataOk;
    
	bool lfCheckValue = true;
	// we know the results of following Operators won't be NAN/INFINITE, no need to do the check
	switch (iOperatorType)
	{
		case DssFuncEquals:
		case DssFuncNotEqual:
		case DssFuncGreater:
		case DssFuncLess:
		case DssFuncGreaterEqual:
		case DssFuncLessEqual:
			lfCheckValue = false;
			break;
		default:
			break;
	}
    
	memset(opFlag, DssDataOk, nResSize * sizeof(DSSDataFlag));
	oAllValid = true;
    
	
	Int32 lRow1 = ipOffset1 ? ipOffset1[0] : 0;
	Int32 lRow2 = ipOffset2 ? ipOffset2[0] : 0;
	// get the first value, in case they are const, no need to get again
	double lVal1 = iDivider1 ? (ipParam1[lRow1]/iDivider1) : ipParam1[lRow1];
	double lVal2 = iDivider2 ? (ipParam2[lRow2]/iDivider2) : ipParam2[lRow2];
	
	bool lConst1 = (iSize1 == 1);
	bool lConst2 = (iSize2 == 1);
    
	for (Int32 iRow = 0; iRow < nResSize; iRow ++)
	{
		if (ipMask && ipMask->Get(iRow) == false)
			continue;
        
		lRow1 = ipOffset1 ? ipOffset1[iRow] : iRow;
		lRow2 = ipOffset2 ? ipOffset2[iRow] : iRow;
		
		if (!lConst1)
		{
			lVal1 = iDivider1 ? (ipParam1[lRow1]/iDivider1) : ipParam1[lRow1];
		}
        
		if (!lConst2)
		{
			lVal2 = iDivider2 ? (ipParam2[lRow2]/iDivider2) : ipParam2[lRow2];
		}
        
		switch (iOperatorType)
		{
            case DssFuncEquals:
                opData[iRow] = DoubleEqual(lVal1, lVal2);
                break;
                
            case DssFuncNotEqual:
                opData[iRow] = !DoubleEqual(lVal1, lVal2);
                break;
                
            case DssFuncGreater:
                opData[iRow] = DoubleGreaterThan(lVal1, lVal2);
                break;
                
            case DssFuncLess:
                opData[iRow] = DoubleLessThan(lVal1, lVal2);
                break;
                
            case DssFuncGreaterEqual:
                opData[iRow] = DoubleGreaterEqual(lVal1, lVal2);
                break;
                
            case DssFuncLessEqual:
                opData[iRow] = DoubleLessEqual(lVal1, lVal2);
                break;
                
            case DssFuncAdd:
            case DssFuncPlus:
                opData[iRow] = lVal1 + lVal2;
                break;
                
            case DssFuncMinus:
                opData[iRow] = lVal1 - lVal2;
                break;
                
            case DssFuncTimes:	
            case DssFuncMultiply:
                opData[iRow] = lVal1 * lVal2;
                break;
                
            case DssFuncDivide:
                if (lVal2)
                    opData[iRow] = lVal1 / lVal2;
                else
                {// ideally, if divided by zero, here opData[iRow] should be DBL_MAX, but as it is operated directly on the output column, set it 0 here
                    opData[iRow] = 0; 
                    opFlag[iRow] = DssDataZerodivide;
                    oAllValid = false;
                }
                break;
                
            default:
                _ASSERT(false);
                return hr = S_FALSE;
		}
		
		if (lfCheckValue)
		{
			// this value check is costly, we should avoid calling it if possible
			CheckValue(&error, opData[iRow]);
            
			if(error != MathNoError)
			{
				opData[iRow] = 0;
				opFlag[iRow] = DssDataInvalid;
				error = MathNoError;
			}
		}
	}
    
	if ((ibAllValid1 && ibAllValid2) || iNullCheckingOption == DssNumericErrorTreatAs)
	{
		// all data are valid, or treat null as zero, no further checking for the flag
	}
	else
	{
		lFlag1 = ipFlags1[0];
		lFlag2 = ipFlags2[0];
        
		DSSDataFlag lFlag = DssDataUnknown;
        
		for (Int32 iRow = 0; iRow < nResSize; iRow ++)
		{
			if (ipMask && ipMask -> Get(iRow) == false)
				continue;
            
			if (!lConst1)
				lFlag1 = ipFlags1[iRow];
            
			if (!lConst2)
				lFlag2 = ipFlags2[iRow];
            
			if (lFlag1 == DssDataOk && lFlag2 == DssDataOk)
				continue;
            
			lFlag = (lFlag1 != DssDataOk) ? lFlag1 : lFlag2;
            
			if (iOperatorType == DssFuncEquals || iOperatorType == DssFuncNotEqual)
			{
				if(lFlag1 != DssDataOk && lFlag2 == DssDataOk)
					lFlag = DssDataNull;
				else
				{
					if ((iOperatorType == DssFuncEquals && lFlag1 == lFlag2) ||
						(iOperatorType == DssFuncNotEqual && lFlag1 != lFlag2))
						opData[iRow] = 1.0;
					lFlag = DssDataOk;
				}
			}
			if (lFlag != DssDataOk)
			{
				opData[iRow] = 0;
				oAllValid = false;
			}
			opFlag[iRow] = lFlag;
		}
	}
	
	return S_OK;
}

#endif
