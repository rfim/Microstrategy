//
//  DSSNNFinancialSTOS.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 2/22/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#include "DSSNNFinancialSTOS.h"
#include "DSSFunctionUtility.h"
#include "financialFuncImplement.h"
#include "DFCDataConversion.h"

DSSNNFinancialSTOS::DSSNNFinancialSTOS() {}

DSSNNFinancialSTOS::~DSSNNFinancialSTOS() {}

int DSSNNFinancialSTOS::Calculate(int nParam, CComVariant *ipParams, CComVariant *opResult, DSSDataFlag *opFlag)
{
    if(!ipParams || !opResult || !opFlag) 
		return E_POINTER;
    
	if ( (ipParams->vt & (VT_ARRAY|VT_VARIANT)) != (VT_ARRAY|VT_VARIANT))
		return E_FAIL;
    
	enum MathError error = MathNoError;
	HRESULT hr = E_FAIL;
	// Int32 sizeDate =sizeof(MDataType::DSSDateTime);
	Int32 sizeDouble = sizeof(double);
	double resultDouble;
	MDataType::DSSDateTime resultDate;
    
	try {
		switch (mFunctionIndex)	{
            case 19: //ACCRINT
                if (nParam == 5){
                    // MDataType::DSSDateTime issue, firstInterest, settlement;
                    double rate, frequency;
                    
//                    hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&issue, sizeDate);;
//                    if(hr != S_OK) return E_FAIL;
//                    
//                    hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&firstInterest, sizeDate);;
//                    if(hr != S_OK) return E_FAIL;
//                    
//                    hr = getParamFromVariant(ipParams, 2, DssDataTypeTime, (BYTE *)&settlement, sizeDate);;
//                    if(hr != S_OK) return E_FAIL;
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime issue(retDate);
                    
                    
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime firstInterest(retDate);
                    
                    
                    hr = getDateFromVariant(ipParams, 2, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    
                    hr = getParamFromVariant(ipParams, 3, DssDataTypeDouble, (BYTE *)&rate, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 4, DssDataTypeDouble, (BYTE *)&frequency, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    
                    resultDouble = _ACCRINT(&error, issue, firstInterest, settlement, rate,  
                                            mProperties[0]/*par*/, (Int32)frequency, (Int32)mProperties[1]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 20: //ACCRINTM
                if (nParam == 3){
                    // MDataType::DSSDateTime issue, maturity;
                    double rate;
                    
//                    hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&issue, sizeDate);;
//                    if(hr != S_OK) return E_FAIL;
                    
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime issue(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);

                    
                    hr = getParamFromVariant(ipParams, 2, DssDataTypeDouble, (BYTE *)&rate, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDouble = _ACCRINTM(&error, issue, maturity, rate,  mProperties[0]/*par*/, 
                                             (Int32)mProperties[1]/*basis*/);
                    
                    
                } else
                    hr = E_FAIL;
                
                break;
                
            case 21: //COUPDAYBS
                if (nParam == 3){
                    // MDataType::DSSDateTime settlement, maturity;
                    double frequency;
                    
                    // hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);
                    
                    hr = getParamFromVariant(ipParams, 2, DssDataTypeDouble, (BYTE *)&frequency, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDouble = _COUPDAYBS(&error, settlement, maturity, (Int32)frequency, (Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 22: //COUPDAYS
                if (nParam == 3){
                    // MDataType::DSSDateTime settlement, maturity;
                    double frequency;
                    
                    // hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);
                    
                    hr = getParamFromVariant(ipParams, 2, DssDataTypeDouble, (BYTE *)&frequency, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDouble = _COUPDAYS(&error, settlement, maturity, (Int32)frequency, (Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 23: //COUPDAYSNC
                if (nParam == 3){
                    // MDataType::DSSDateTime settlement, maturity;
                    double frequency;
                    
                    // hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);

                    
                    hr = getParamFromVariant(ipParams, 2, DssDataTypeDouble, (BYTE *)&frequency, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDouble = _COUPDAYSNC(&error, settlement, maturity, (Int32)frequency, (Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 24: //COUPNUM
                if (nParam == 3){
                    // MDataType::DSSDateTime settlement, maturity;
                    double frequency;
                    
                    // hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);

                    
                    hr = getParamFromVariant(ipParams, 2, DssDataTypeDouble, (BYTE *)&frequency, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDouble = _COUPNUM(&error, settlement, maturity, (Int32)frequency, (Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 25: //COUPNCD
                if (nParam == 3){
                    // MDataType::DSSDateTime settlement, maturity;
                    double frequency;
                    
                    // hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);

                    
                    hr = getParamFromVariant(ipParams, 2, DssDataTypeDouble, (BYTE *)&frequency, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDate = _COUPNCD(&error, settlement, maturity, (Int32)frequency, (Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 26: //COUPPCD
                if (nParam == 3){
                    // MDataType::DSSDateTime settlement, maturity;
                    double frequency;
                    
                    // hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);

                    
                    hr = getParamFromVariant(ipParams, 2, DssDataTypeDouble, (BYTE *)&frequency, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDate = _COUPPCD(&error, settlement, maturity, (Int32)frequency, (Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 27: //DISC
                if (nParam == 4){
                    // MDataType::DSSDateTime settlement, maturity;
                    double price, redemption;
                    
                    // hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);
                    
                    hr = getParamFromVariant(ipParams, 2, DssDataTypeDouble, (BYTE *)&price, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 3, DssDataTypeDouble, (BYTE *)&redemption, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDouble = _DISC(&error, settlement, maturity, price, redemption, (Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 28: //DURATION
                if (nParam == 5){
                    // MDataType::DSSDateTime settlement, maturity;
                    double couponRate, yldRate, frequency;
                    
                    // hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);

                    
                    hr = getParamFromVariant(ipParams, 2, DssDataTypeDouble, (BYTE *)&couponRate, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 3, DssDataTypeDouble, (BYTE *)&yldRate, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 4, DssDataTypeDouble, (BYTE *)&frequency, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDouble = _DURATION(&error, settlement, maturity, couponRate, yldRate, 
                                             (Int32)frequency, (Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 29: //MDURATION
                if (nParam == 5){
                    // MDataType::DSSDateTime settlement, maturity;
                    double couponRate, yldRate, frequency;
                    
                    // hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);
                    
                    hr = getParamFromVariant(ipParams, 2, DssDataTypeDouble, (BYTE *)&couponRate, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 3, DssDataTypeDouble, (BYTE *)&yldRate, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 4, DssDataTypeDouble, (BYTE *)&frequency, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDouble = _MDURATION(&error, settlement, maturity, couponRate, yldRate, 
                                              (Int32)frequency, (Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 30: //INTRATE
                if (nParam == 4){
                    // MDataType::DSSDateTime settlement, maturity;
                    double investment, redemption;
                    
                    // hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);
                    
                    hr = getParamFromVariant(ipParams, 2, DssDataTypeDouble, (BYTE *)&investment, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 3, DssDataTypeDouble, (BYTE *)&redemption, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    
                    resultDouble = _INTRATE(&error, settlement, maturity, investment, redemption, 
                                            (Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 31: //ODDFPRICE
                if (nParam == 8){
                    // MDataType::DSSDateTime settlement, maturity, issue, firstCoupon;
                    double couponRate, yldRate, redemption, frequency;
                    
//                    hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
//                    if(hr != S_OK) return E_FAIL;
//                    
//                    hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
//                    if(hr != S_OK) return E_FAIL;
//                    
//                    hr = getParamFromVariant(ipParams, 2, DssDataTypeTime, (BYTE *)&issue, sizeDate);
//                    if(hr != S_OK) return E_FAIL;
//                    
//                    hr = getParamFromVariant(ipParams, 3, DssDataTypeTime, (BYTE *)&firstCoupon, sizeDate);
//                    if(hr != S_OK) return E_FAIL;
                    
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);
                    
                    
                    hr = getDateFromVariant(ipParams, 2, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime issue(retDate);
                    
                    hr = getDateFromVariant(ipParams, 3, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime firstCoupon(retDate);

                    
                    hr = getParamFromVariant(ipParams, 4, DssDataTypeDouble, (BYTE *)&couponRate, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 5, DssDataTypeDouble, (BYTE *)&yldRate, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 6, DssDataTypeDouble, (BYTE *)&redemption, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 7, DssDataTypeDouble, (BYTE *)&frequency, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDouble = _ODDFPRICE(&error, settlement, maturity, issue, firstCoupon, couponRate, 
                                              yldRate, redemption, (Int32)frequency,	(Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 32: //ODDFYIELD
                if (nParam == 8){
                    // MDataType::DSSDateTime settlement, maturity, issue, firstCoupon;
                    double couponRate, price, redemption, frequency;
                    
//                    hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
//                    if(hr != S_OK) return E_FAIL;
//                    
//                    hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
//                    if(hr != S_OK) return E_FAIL;
//                    
//                    hr = getParamFromVariant(ipParams, 2, DssDataTypeTime, (BYTE *)&issue, sizeDate);
//                    if(hr != S_OK) return E_FAIL;
//                    
//                    hr = getParamFromVariant(ipParams, 3, DssDataTypeTime, (BYTE *)&firstCoupon, sizeDate);
//                    if(hr != S_OK) return E_FAIL;

                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);
                    
                    
                    hr = getDateFromVariant(ipParams, 2, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime issue(retDate);
                    
                    hr = getDateFromVariant(ipParams, 3, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime firstCoupon(retDate);
                    
                    hr = getParamFromVariant(ipParams, 4, DssDataTypeDouble, (BYTE *)&couponRate, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 5, DssDataTypeDouble, (BYTE *)&price, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 6, DssDataTypeDouble, (BYTE *)&redemption, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 7, DssDataTypeDouble, (BYTE *)&frequency, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDouble = _ODDFYIELD(&error, settlement, maturity, issue, firstCoupon, couponRate, 
                                              price, redemption, (Int32)frequency,	(Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 33: //ODDLPRICE
                if (nParam == 7){
                    // MDataType::DSSDateTime settlement, maturity, lastInterest;
                    double couponRate, yldRate, redemption, frequency;
                    
//                    hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
//                    if(hr != S_OK) return E_FAIL;
//                    
//                    hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
//                    if(hr != S_OK) return E_FAIL;
//                    
//                    hr = getParamFromVariant(ipParams, 2, DssDataTypeTime, (BYTE *)&lastInterest, sizeDate);
//                    if(hr != S_OK) return E_FAIL;
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);
                    
                    
                    hr = getDateFromVariant(ipParams, 2, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime lastInterest(retDate);

                    
                    hr = getParamFromVariant(ipParams, 3, DssDataTypeDouble, (BYTE *)&couponRate, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 4, DssDataTypeDouble, (BYTE *)&yldRate, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 5, DssDataTypeDouble, (BYTE *)&redemption, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 6, DssDataTypeDouble, (BYTE *)&frequency, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDouble = _ODDLPRICE(&error, settlement, maturity, lastInterest,  couponRate, 
                                              yldRate, redemption, (Int32)frequency,	(Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 34: //ODDLYIELD
                if (nParam == 7){
                    // MDataType::DSSDateTime settlement, maturity, lastInterest;
                    double couponRate, price, redemption, frequency;
                    
//                    hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
//                    if(hr != S_OK) return E_FAIL;
//                    
//                    hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
//                    if(hr != S_OK) return E_FAIL;
//                    
//                    hr = getParamFromVariant(ipParams, 2, DssDataTypeTime, (BYTE *)&lastInterest, sizeDate);
//                    if(hr != S_OK) return E_FAIL;
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);
                    
                    
                    hr = getDateFromVariant(ipParams, 2, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime lastInterest(retDate);
                    
                    hr = getParamFromVariant(ipParams, 3, DssDataTypeDouble, (BYTE *)&couponRate, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 4, DssDataTypeDouble, (BYTE *)&price, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 5, DssDataTypeDouble, (BYTE *)&redemption, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 6, DssDataTypeDouble, (BYTE *)&frequency, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDouble = _ODDLYIELD(&error, settlement, maturity, lastInterest, couponRate, 
                                              price, redemption, (Int32)frequency,	(Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 35: //PRICE
                if (nParam == 6){
                    // MDataType::DSSDateTime settlement, maturity;
                    double couponRate, yldRate, redemption, frequency;
                    
                    // hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);

                    
                    hr = getParamFromVariant(ipParams, 2, DssDataTypeDouble, (BYTE *)&couponRate, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 3, DssDataTypeDouble, (BYTE *)&yldRate, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 4, DssDataTypeDouble, (BYTE *)&redemption, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 5, DssDataTypeDouble, (BYTE *)&frequency, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDouble = _PRICE(&error, settlement, maturity, couponRate, 
                                          yldRate, redemption, (Int32)frequency,	(Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 36: //PRICEDISC
                if (nParam == 4){
                    // MDataType::DSSDateTime settlement, maturity;
                    double discRate, redemption;
                    
                    // hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);

                    
                    hr = getParamFromVariant(ipParams, 2, DssDataTypeDouble, (BYTE *)&discRate, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 3, DssDataTypeDouble, (BYTE *)&redemption, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDouble = _PRICEDISC(&error, settlement, maturity, discRate, 
                                              redemption, (Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 37: //PRICEMAT
                if (nParam == 5){
                    // MDataType::DSSDateTime settlement, maturity, issue;
                    double coupRate, yldRate;
                    
//                    hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
//                    if(hr != S_OK) return E_FAIL;
//                    
//                    hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
//                    if(hr != S_OK) return E_FAIL;
//                    
//                    hr = getParamFromVariant(ipParams, 2, DssDataTypeTime, (BYTE *)&issue, sizeDate);
//                    if(hr != S_OK) return E_FAIL;

                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);
                    
                    
                    hr = getDateFromVariant(ipParams, 2, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime issue(retDate);
                    
                    hr = getParamFromVariant(ipParams, 3, DssDataTypeDouble, (BYTE *)&coupRate, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 4, DssDataTypeDouble, (BYTE *)&yldRate, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDouble = _PRICEMAT(&error, settlement, maturity, issue, coupRate, 
                                             yldRate, (Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 38: //YIELD
                if (nParam == 6){
                    // MDataType::DSSDateTime settlement, maturity;
                    double coupRate, price, redemption, frequency;

                    // hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);

                    
                    hr = getParamFromVariant(ipParams, 2, DssDataTypeDouble, (BYTE *)&coupRate, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 3, DssDataTypeDouble, (BYTE *)&price, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 4, DssDataTypeDouble, (BYTE *)&redemption, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 5, DssDataTypeDouble, (BYTE *)&frequency, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDouble = _YIELD(&error, settlement, maturity, coupRate, price,
                                          redemption, (Int32)frequency, (Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 39: //RECEIVED
                if (nParam == 4){
                    // MDataType::DSSDateTime settlement, maturity;
                    double investment, discount;
                    
                    // hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);

                    
                    hr = getParamFromVariant(ipParams, 2, DssDataTypeDouble, (BYTE *)&investment, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 3, DssDataTypeDouble, (BYTE *)&discount, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    
                    resultDouble = _RECEIVED(&error, settlement, maturity, investment, discount,
                                             (Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 40: //TBILLEQ
                if (nParam == 3){
                    // MDataType::DSSDateTime settlement, maturity;
                    
                    
                    // hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);
                    
                    double discount;
                    hr = getParamFromVariant(ipParams, 2, DssDataTypeDouble, (BYTE *)&discount, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    
                    resultDouble = _TBILLEQ(&error, settlement, maturity, discount);
                } else
                    hr = E_FAIL;
                
                break;
                
                
            case 41: //TBILLPRICE
                if (nParam == 3){
                    // MDataType::DSSDateTime settlement, maturity;
                    double discount;
                    
                    // hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);

                    
                    hr = getParamFromVariant(ipParams, 2, DssDataTypeDouble, (BYTE *)&discount, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    
                    resultDouble = _TBILLPRICE(&error, settlement, maturity, discount);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 42: //TBILLYIELD
                if (nParam == 3){
                    // MDataType::DSSDateTime settlement, maturity;
                    double price;

                    // hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);

                    
                    hr = getParamFromVariant(ipParams, 2, DssDataTypeDouble, (BYTE *)&price, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    
                    resultDouble = _TBILLYIELD(&error, settlement, maturity, price);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 43: //YIELDDISC
                if (nParam == 4){
                    // MDataType::DSSDateTime settlement, maturity;
                    double price, redemption;

                    // hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);

                    
                    hr = getParamFromVariant(ipParams, 2, DssDataTypeDouble, (BYTE *)&price, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 3, DssDataTypeDouble, (BYTE *)&redemption, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDouble = _YIELDDISC(&error, settlement, maturity, price, redemption, (Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            case 44: //YIELDMAT
                if (nParam == 5){
                    // MDataType::DSSDateTime settlement, maturity, issue;
                    double price, rate;
                    
//                    hr = getParamFromVariant(ipParams, 0, DssDataTypeTime, (BYTE *)&settlement, sizeDate);
//                    if(hr != S_OK) return E_FAIL;
//                    
//                    hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
//                    if(hr != S_OK) return E_FAIL;
//                    
//                    hr = getParamFromVariant(ipParams, 2, DssDataTypeTime, (BYTE *)&issue, sizeDate);
//                    if(hr != S_OK) return E_FAIL;
                    double retDate;
                    hr = getDateFromVariant(ipParams, 0, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime settlement(retDate);
                    
                    // hr = getParamFromVariant(ipParams, 1, DssDataTypeTime, (BYTE *)&maturity, sizeDate);
                    hr = getDateFromVariant(ipParams, 1, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime maturity(retDate);
                    
                    hr = getDateFromVariant(ipParams, 2, &retDate);
                    if(hr != S_OK) return E_FAIL;
                    MDataType::DSSDateTime issue(retDate);
                    
                    hr = getParamFromVariant(ipParams, 3, DssDataTypeDouble, (BYTE *)&rate, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    hr = getParamFromVariant(ipParams, 4, DssDataTypeDouble, (BYTE *)&price, sizeDouble);
                    if(hr != S_OK) return E_FAIL;
                    
                    resultDouble = _YIELDMAT(&error, settlement, maturity, issue, rate, price, (Int32)mProperties[0]/*basis*/);
                } else
                    hr = E_FAIL;
                
                break;
                
            default:
                //TRACE info
                _ASSERT(FALSE);
                hr = E_FAIL;
                break;
                
		}
	} catch (...) {
		hr = E_FAIL;
	}
    
	if (hr == S_OK){
		if(error == MathNoError) {
			if (mFunctionIndex == 25 || mFunctionIndex == 26)
            {
				// return value type is MDataType::DSSDateTime
				// DFCDataConversion::ConvertToVariant((BYTE *)(&resultDate), DssDataTypeDate, 0, opResult);
                CComVariant ret;
                ret.vt = VT_DATE;
                ret.date = resultDate.ToDATE();
                *opResult = ret;
            }
			else
				// return value type is double
				DFCDataConversion::ConvertToVariant((BYTE *)(&resultDouble), DssDataTypeDouble, 0, opResult);
            
			*opFlag = DssDataOk;
		}
		else
			*opFlag = DssDataInvalid;
	}
	return hr; 
}

int DSSNNFinancialSTOS::GetOutputType (int size, EnumDSSDataType *inputType, EnumDSSDataType *outputType)
{
    if(!outputType) 
		return E_POINTER;
    
	if (mFunctionIndex == 25 || mFunctionIndex == 26)
		//only COUPNCD() And COUPPCD() return date 
		*outputType = DssDataTypeDate;
	else
		*outputType = DssDataTypeDouble;
    
	return S_OK;
}

int DSSNNFinancialSTOS::Init(int iIndex)
{
    mFunctionIndex = iIndex;
    
	//set default property value for some function other than 0
	switch (iIndex){
        case 19: //ACCRINT()
            mProperties[1] = 1000; // Par is 1000
            break;
        case 20: //ACCRINTM()
            mProperties[1] = 1000;	 // Par is 1000
            break;
        default:
            break;
	}
    
	return S_OK;
}

int DSSNNFinancialSTOS::setProperty (Int32 iIndex, CComVariant *ipValue)
{
    HRESULT hr = VariantChangeType(ipValue, ipValue, 0, VT_R8);
    if ( hr != S_OK ) return hr;
    
    mProperties[iIndex - 1] = ipValue->dblVal;
    
    return S_OK;
}

int DSSNNFinancialSTOS::getParamFromVariant(VARIANT *ipParams, LONG index, EnumDSSDataType type, BYTE *outResult,Int32 size)
{
    CComVariant element;
    
	HRESULT hr = SafeArrayGetElement(ipParams->parray, &index, &element);
	if (hr != S_OK) return hr;
    
	hr = DFCDataConversion::ConvertFromVariant(&element, type, outResult, &size);
    
	return hr;
}

int DSSNNFinancialSTOS::Calculate(int nParam, double *ipParams, double *opResult, DSSDataFlag *opFlag)
{
    return E_NOTIMPL;
}

int DSSNNFinancialSTOS::getDateFromVariant(VARIANT *ipParams, LONG index, double *outDate)
{
    CComVariant element;
    
	HRESULT hr = SafeArrayGetElement(ipParams->parray, &index, &element);
	if (hr != S_OK) return hr;
    
    *outDate = element.date;
    
    return S_OK;
}
