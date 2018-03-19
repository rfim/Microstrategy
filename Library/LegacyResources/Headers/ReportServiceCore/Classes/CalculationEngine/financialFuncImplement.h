//
//  financialFuncImplement.h
//  ReportServiceCore
//
//  Created by He Tengfei on 2/23/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//
#ifndef _FINANCIALFUNCIMPLEMENT_
#define _FINANCIALFUNCIMPLEMENT_ 1

#include "DSSFunctionUtility.h"


#ifdef WIN32

#ifdef DFC_FINANCE_EXPORT
#define DFC_FINANCE_EXIM _declspec(dllexport)
#else
#define DFC_FINANCE_EXIM _declspec(dllimport)
#endif
#else

#define DFC_FINANCE_EXIM

#endif

DFC_FINANCE_EXIM double _CUMIPMT(enum MathError *error, double rate, Int32 nper, double pv, 
                                 Int32 start_period, Int32 end_period, Int32 type);
DFC_FINANCE_EXIM double _CUMPRINC(enum MathError *error, double rate, Int32 nper, double pv, 
                                  Int32 start_period, Int32 end_period, Int32 type);
DFC_FINANCE_EXIM double _DB(enum MathError *error, double cost, double salvage, Int32 life, Int32 period,
                            Int32 month);
DFC_FINANCE_EXIM double _DDB(enum MathError *error, double cost, double salvage, double life, double period,
                             double factor);
DFC_FINANCE_EXIM double _DOLLARDE(enum MathError *error, double fractionalDollar, Int32 fraction);
DFC_FINANCE_EXIM double _DOLLARFR(enum MathError *error, double decimalDollar, Int32 fraction);
DFC_FINANCE_EXIM double _EFFECT(enum MathError *error, double nominal_rate, Int32 npery);
DFC_FINANCE_EXIM double _FV(enum MathError *error, double rate, double nper,double pmt, double pv,
                            Int32 type);
DFC_FINANCE_EXIM double _FVSCHEDULE(enum MathError *error, double pv, Int32 n, double schedule[]);
DFC_FINANCE_EXIM double _IPMT(enum MathError *error, double rate, double per, double nper, double pv,
                              double fv, Int32 type);
DFC_FINANCE_EXIM double _IRR(enum MathError *error, Int32 nValue, double values[], double guess, Int32 method, double precision);
DFC_FINANCE_EXIM double _MIRR(enum MathError *error, Int32 nValue, double values[], double financeRate,
                              double reinvestRate);
DFC_FINANCE_EXIM double _NOMINAL(enum MathError *error, double effect_rate, Int32 npery);
DFC_FINANCE_EXIM double _NPER(enum MathError *error, double rate, double pmt, double pv, double fv, 
                              Int32 type);
DFC_FINANCE_EXIM double _NPV(enum MathError *error, double rate, Int32 nValue, double values[]);
DFC_FINANCE_EXIM double _PMT(enum MathError *error, double rate, double nper, double pv, double fv,
                             Int32 type);
DFC_FINANCE_EXIM double _PPMT(enum MathError *error, double rate, double per, double nper, double pv,
                              double fv, Int32 type);
DFC_FINANCE_EXIM double _PV(enum MathError *error, double rate, double nper, double pmt, double fv,
                            Int32 type);
DFC_FINANCE_EXIM double _RATE(enum MathError *error, double nper, double pmt, double pv, double fv,
                              Int32 type, double guess);
DFC_FINANCE_EXIM double _SLN(enum MathError *error, double cost, double salvage, double life);
DFC_FINANCE_EXIM double _SYD(enum MathError *error, double cost, double salvage, double life, 
                             double per);
DFC_FINANCE_EXIM double _VDB(enum MathError *error, double cost, double salvage, double life, 
                             Int32 period, double factor);

// #include "DataType/DataType/DateTime.h"
#include "DSSDateTime.h"

typedef MDataType::DSSDateTime Date;

DFC_FINANCE_EXIM double _ACCRINT(enum MathError *error, const Date & issue, const Date & firstInterest, const Date & settlement, double rate, 
                                 double par, Int32 frequency, Int32 basis);
DFC_FINANCE_EXIM double _ACCRINTM(enum MathError *error, const Date & issue, const Date & maturity, double rate, double par, Int32 basis);
DFC_FINANCE_EXIM Int32  _COUPDAYBS(enum MathError *error, const Date & settlement, const Date & maturity, Int32 frequency, Int32 basis);
DFC_FINANCE_EXIM double _COUPDAYS(enum MathError *error, const Date & settlement, const Date & maturity, Int32 frequency, Int32 basis);
DFC_FINANCE_EXIM Int32  _COUPDAYSNC(enum MathError *error, const Date & settlement, const Date & maturity, Int32 frequency, Int32 basis);
DFC_FINANCE_EXIM Int32  _COUPNUM(enum MathError *error, const Date & settlement, const Date & maturity, Int32 frequency, Int32 basis);
DFC_FINANCE_EXIM Date _COUPNCD(enum MathError *error, const Date & settlement, const Date & maturity, Int32 frequency, Int32 basis);
DFC_FINANCE_EXIM Date _COUPPCD(enum MathError *error, const Date & settlement, const Date & maturity, Int32 frequency, Int32 basis);
DFC_FINANCE_EXIM double _DISC(enum MathError *error, const Date & settlement, const Date & maturity, double pr,
                              double redemption, Int32 basis);
DFC_FINANCE_EXIM double _DURATION(enum MathError *error, const Date & settlement, const Date & maturity,
                                  double couponRate, double  yldRate, Int32 frequency, Int32 basis);
DFC_FINANCE_EXIM double _MDURATION(enum MathError *error, const Date & settlement, const Date & maturity,
                                   double couponRate, double  yldRate, Int32 frequency, Int32 basis);
DFC_FINANCE_EXIM double _INTRATE(enum MathError *error, const Date & settlement, const Date & maturity, double investment,
                                 double redemption, Int32 basis);
DFC_FINANCE_EXIM double _ODDFPRICE(enum MathError *error, const Date & settlement, const Date & maturity, const Date & issue,
                                   const Date & firstCoupon, double couponRate, double yld, double redemption, 
                                   Int32 frequency, Int32 basis);
DFC_FINANCE_EXIM double _ODDFYIELD(enum MathError *error, const Date & settlement, const Date & maturity, const Date & issue,
                                   const Date & firstCoupon, double couponRate, double price, double redemption,
                                   Int32 frequency, Int32 basis);
DFC_FINANCE_EXIM double _ODDLPRICE(enum MathError *error, const Date & settlement, const Date & maturity, 
                                   const Date & lastInterest, double couponRate, double yld, double redemption,
                                   Int32 frequency, Int32 basis);
DFC_FINANCE_EXIM double _ODDLYIELD(enum MathError *error, const Date & settlement, const Date & maturity, 
                                   const Date & lastInterest, double couponRate, double price, double redemption,
                                   Int32 frequency, Int32 basis);
DFC_FINANCE_EXIM double _PRICE(enum MathError *error, const Date & settlement, const Date & maturity,
                               double couponRate, double  yldRate, double redemption, 
                               Int32 frequency, Int32 basis);
DFC_FINANCE_EXIM double _PRICEDISC(enum MathError *error, const Date & settlement, const Date & maturity, 
                                   double discRate, double redemption, Int32 basis);
DFC_FINANCE_EXIM double _PRICEMAT(enum MathError *error, const Date & settlement, const Date & maturity, 
                                  const Date & issue, double rate, double yld, Int32 basis);
DFC_FINANCE_EXIM double _YIELD(enum MathError *error, const Date & settlement, const Date & maturity,
                               double couponRate, double  price, double redemption, 
                               Int32 frequency, Int32 basis);
DFC_FINANCE_EXIM double _RECEIVED(enum MathError *error, const Date & settlement, const Date & maturity, double investment,
                                  double discount, Int32 basis);
DFC_FINANCE_EXIM double _TBILLEQ(enum MathError *error, const Date & settlement, const Date & maturity, double discount);
DFC_FINANCE_EXIM double _TBILLPRICE(enum MathError *error, const Date & settlement, const Date & maturity, double discount);
DFC_FINANCE_EXIM double _TBILLYIELD(enum MathError *error, const Date & settlement, const Date & maturity, double pr);
DFC_FINANCE_EXIM double _XNPV(enum MathError *error, double rate,Int32 n, double values[], Date dates[], Int32 valid[]);
DFC_FINANCE_EXIM double _XIRR(enum MathError *error, Int32 n, double values[], Date dates[], double guess, Int32 valid[]);
DFC_FINANCE_EXIM double  _YIELDDISC(enum MathError *error, const Date &settlement, const Date & maturity, double price,
                                    double redemption, Int32 basis);
DFC_FINANCE_EXIM double _YIELDMAT(enum MathError *error, const Date & settlement, const Date & maturity, const Date & issue,
                                  double rate, double price, Int32 basis);
void adjustDate(Date & result, const Date & maturity);


#endif