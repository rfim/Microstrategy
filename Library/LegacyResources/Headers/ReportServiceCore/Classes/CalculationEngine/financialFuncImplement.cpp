//
//  financialFuncImplement.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 2/23/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#include "financialFuncImplement.h"

#include "DateTimeHelper.h"

//#include "StdAfx.h"
//#include "DSSFinancialPackage/financialFuncImplement.h"
//#include "PDCHeader/PDCstdlib.h"
//#include "PDCHeader/PDCtime.h"
//#include "DSSFunctionServer/FunctionServerHelper/Utility.h"
#define MAXERROR (1e-7)

#pragma warning(disable:4018)	//signed/unsigned mismatch

// #include "DSSFinancialPackage/DateTimeHelper.h"

// ytang, 2009-06-15
enum EnumRootFindingMethod
{
	EnumMethodSecant,
	EnumMethodNewton,
	EnumMethodLastone
};

inline BOOL  ApproxiEqual(double a, double b)
{
	return (fabs(a - b) <= MAXERROR) ? TRUE : FALSE;
}

static double invert (enum MathError *errorFlag, double(*func)(enum MathError *, double, const void * []), 
					  double input, double first, double second, const void * params[], 
					  Int32 nLimits, double err );
static double _NPV_stub(enum MathError *error, double rate, const void *param[]);
static double _RATE_stub(enum MathError *error, double rate, const void *param[]);
static double _ODDFPRICE_stub(enum MathError *error, double yield, const void *param[]);
static double _PRICE_stub(enum MathError *error, double yield, const void *param[]);
static double _XNPV_stub(enum MathError *error, double rate, const void *param[]);
// ytang, 2009-06-15
static double _IRR_NewtonMethod(enum MathError *errorFlag, double(*func)(enum MathError *, double, const void * []),  
                                double guess, const void * params[], Int32 nLimits, double err );
static double _NPV_Df(enum MathError *error, double rate, const void * params[]);

static double _XIRR_NewtonMethod(enum MathError *errorFlag, double(*func)(enum MathError *, double, const void * []),
                                 double guess, const void * params[], Int32 nLimits, double err );
static double _XNPV_Df(enum MathError *error, double rate, const void * params[]);

//Returns the cumulative interest paid on a loan between start_period and end_period.
double _CUMIPMT(enum MathError *error, double rate, Int32 nper, double pv, 
				Int32 start_period, Int32 end_period, Int32 type) {
	if (DoubleLessThan(rate, 0) || nper < 0 || pv < 0 || start_period < 1 || end_period < 1
		|| start_period > end_period || type < 0 || type > 1){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	double result = 0;
	Int32 i = 0;
	for (i = start_period; i <= end_period; i ++){
		result += _IPMT(error, rate, i, nper, pv, 0, type);
		if (*error != MathNoError)
			return result; //failed
	}
    
	CheckValue(error, result);
    
	return result;
    
    
}

//Returns the cumulative principal paid on a loan between start_period and
// end_period.
double _CUMPRINC(enum MathError *error, double rate, Int32 nper, double pv, 
                 Int32 start_period, Int32 end_period, Int32 type) {
	if (DoubleLessThan(rate, 0) || nper < 0 || pv < 0 || start_period < 1 || end_period < 1
		|| start_period > end_period || type < 0 || type > 1){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	double result = 0;
	Int32 i = 0;
	for (i = start_period; i <= end_period; i ++){
		result += _PPMT(error, rate, i, nper, pv, 0, type);
		if (*error != MathNoError)
			return result; //failed
	}
    
	CheckValue(error, result);
    
	return result;
    
    
}

//Returns the depreciation of an asset for a specified period using the 
//fixed-declining balance method.
double _DB(enum MathError *error, double cost, double salvage, Int32 life, Int32 period,
		   Int32 month){
	if (life < 0 || period > life || period <= 0 || cost < 0 || salvage < 0){  
		SetMathError(error, MathDomain);
		return 0;
	}
    
	if (month < 1 || month > 12 || ( month == 12 && period > life) || 
		(month != 12 && period > life + 1)){  
		SetMathError(error, MathDomain);
		return 0;
	}
    
	
	double rate = 1 - pow(salvage / cost, 1 / (double)life);
	CheckValue(error, rate);
	//round rate to three decimal places
	rate = (Int32)(rate * 1000 + 0.5);
	rate /= 1000;
    
	if (*error != MathNoError)
		return 0;
    
	double result;
	if (period == 1)//first time period
		result = cost * rate * month / 12;
	else if (period <= life)
		result = rate * (cost * pow(1 - rate, period - 2) - 
                         (cost * rate * month / 12) * pow(1 - rate, period - 2));
	else { // last time period
		_ASSERTE (month != 12 && period == life + 1);
		result = rate * (cost * pow(1 - rate, period - 2) - 
                         (cost * rate * month / 12) * pow(1 - rate, period - 2)) * (12 - month) / 12;
	}
    
	CheckValue(error, result);
    
	return result;
}

//Returns the depreciation of an asset for a specified period using the double-declining 
//balance method or some other method you specify.
double _DDB(enum MathError *error, double cost, double salvage, double life, double period,
            double factor){
    
	if (life < 0 || period > life || period <= 0 || cost < 0 || salvage < 0 || factor <= 0 ){  
		SetMathError(error, MathDomain);
		return 0;
	}
    
	
	double rate = factor / life;
    
    
	//total depreciation from prior periods
	double fDepreciation = cost * ( 1 - pow(1 - rate, period - 1));
	CheckValue(error, fDepreciation);
	
	double result = cost * pow (1 - rate, period - 1) * rate;
	CheckValue(error, result);
    
	if ((cost - salvage) < (fDepreciation + result)) {
		if ((cost -salvage) > fDepreciation)
			result = cost - salvage - fDepreciation;
		else
			result = 0;
	}
    
	CheckValue(error, result);
    
	return result;
    
}

//Converts a dollar price expressed as a fraction into a dollar price expressed as 
//a decimal number. 
double _DOLLARDE(enum MathError *error, double fractionalDollar, Int32 fraction){
	if (fraction < 1){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	//xhan issue 208686 12/04/04 fix interger overflow dead loop
	double factor = 1;
	while (factor < fraction)
		factor *= 10;
    
	double result = (Int32)(fractionalDollar) + 
    ( (fractionalDollar - (Int32)(fractionalDollar)) * factor / fraction);
	CheckValue(error, result);
    
	return result;
    
}

//Converts a dollar price expressed as a decimal number into a dollar price 
//expressed as a fraction. 
double _DOLLARFR(enum MathError *error, double decimalDollar, Int32 fraction){
	if (fraction < 1){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	//xhan issue 208686 12/04/04 fix interger overflow dead loop
	double factor = 1;
	while (factor < fraction)
		factor *= 10;
    
	double result = (Int32)(decimalDollar) + 
    ( (decimalDollar - (Int32)(decimalDollar)) / factor * fraction);
	CheckValue(error, result);
    
	return result;
    
}

//Returns the effective annual interest rate, given the nominal annual interest 
//rate and the number of compounding periods per year.
double _EFFECT(enum MathError *error, double nominal_rate, Int32 npery){
	if (nominal_rate < 0 || npery < 1){
		SetMathError(error, MathDomain);
		return 0;
	}
	
	double result = pow(1 + nominal_rate / npery, npery) - 1;
	CheckValue(error, result);
    
	return result;
}


//Returns the future value of an investment based on periodic, constant payments 
//and a constant interest rate.
double _FV(enum MathError *error, double rate, double nper,double pmt, double pv,
		   Int32 type){
	double result;
    
	if (ApproxiEqual(rate, 0.0)){
		result = -pv - pmt * nper;
	} else {
		result = - pv * pow(1 + rate, nper) - 
        pmt * (1 + rate * type)  * (pow(1 + rate, nper) - 1) / rate;
	}
    
	CheckValue(error, result);
    
	return result;
    
}

//Returns the future value of an initial principal after applying a series of 
//compound interest rates.
double _FVSCHEDULE(enum MathError *error, double pv, Int32 n, double schedule[]){
	double result = pv;
    
	Int32 i = 0;
	for (i = 0; i < n; i ++)
		result *= (1 + schedule[i]);
    
	CheckValue(error, result);
    
	return result;
    
}



//Returns the interest payment for a given period for an investment based on 
//periodic, constant payments and a constant interest rate.
double _IPMT(enum MathError *error, double rate, double per, double nper, double pv,
			 double fv, Int32 type){
    
	if (per < 1 || per > nper || type < 0 || type > 1){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	double pmt = _PMT(error, rate, nper, pv, fv, type);
    
	if (*error != MathNoError)
		return 0;
    
	double result;
	
    
	// the formula from exel manual doesn't match, So I am using my own formula.
	if (type == 0) {
        //	result = - pv * pow(1 + rate, per - 1) * rate - 
        //				pmt * (pow(1 + rate, per - 1) - 1);
		result = (-pv * rate - pmt) * pow(1 + rate, per - 1) + pmt;
	} else { // type = 1;
		_ASSERTE(type == 1);
		if (per < 2)
			result = 0;
		else
            //	result = -pv * pow(1 + rate, per - 2) * rate - 
            //			pmt * (pow(1 + rate, per - 1) - 1);
            result = (-pv * rate - pmt * (1 + rate)) * pow(1 + rate, per - 2) + pmt;
	}
    
	CheckValue(error, result);
    
	return result;
    
    
}

// Returns the internal rate of return for a series of cash flows 
// User will supply 1) guess; 2) method (0 for secant, 1 for Newton); and 3) Precision 
double _IRR(enum MathError *error, Int32 nValue, double values[], double guess, Int32 method, double precision) 
{		
	BOOL bHasPositive = FALSE, 
    bHasNegative = FALSE; 
    
	// input values should at least have one positive value and one negative value
	Int32 i = 0;
	for ( i = 0; i < nValue; i++){
		if (DoubleLessThan(values[i], 0) ) 
			bHasNegative = TRUE;
        
		if (DoubleGreaterThan(values[i], 0) )
			bHasPositive = TRUE;
        
		if (bHasPositive && bHasNegative)
			break;
	}
    
	if (!bHasPositive || !bHasNegative) {
		SetMathError(error, MathDomain);
		return 0;
	}
    
	const void *param[2];
	param[0] = &nValue;
	param[1] = values;
    
	double result, first, delta, sign;
    
	Int32 MAX_ITERATION = 1000;
    
	// ytang, 2009-06-15, User can choose Secant or Newton's method to be used to find root
	if (method == EnumMethodSecant)
	{
		//ytang 2008-JUL-10 
		//TQMS285745
		//first try 21 points at 0.1 interval between [guess-1, guess+1]
		//i.e. {guess,guess-0.1,guess+0.1,guess-0.2,guess+0.2,...,guess-1,guess+1}
		for (delta = 0, first = guess, sign = 1; delta <= 2; delta += 0.1) {
			first = first + delta * sign;
			sign *= -1;
			result = invert(error, _NPV_stub, 0, 0, first, param, MAX_ITERATION, precision);
			CheckValue(error, result);
			if (*error == MathNoError && result >= -100 && result <= 100)
				return result;
			else
				*error = MathNoError;
		}
		//try 21 points at 0.1 interval between [-1.0, 1.0]	
		//i.e. {0, -0.1, 0.1, -0.2, 0.2,..., -1, 1}
		for (delta = 0, first = 0, sign = 1; delta <= 2; delta += 0.1){
			first = first + delta * sign;
			sign *= -1;
			result = invert(error, _NPV_stub, 0, 0, first, param, MAX_ITERATION, precision);
			CheckValue(error, result);
			if (*error == MathNoError && result >= -100 && result <= 100)
				return result;
			else
				*error = MathNoError;
		}	
		//try 201 points at 0.01 interval between [guess-1, guess+1]
		for (delta = 0, first = guess, sign = 1; delta <= 2; delta += 0.01) {
			first = first + delta * sign;
			sign *= -1;
			result = invert(error, _NPV_stub, 0, 0, first, param, MAX_ITERATION, precision);
			CheckValue(error, result);
			if (*error == MathNoError && result >= -100 && result <= 100)
				return result;
			else
				*error = MathNoError;
		}
		//try 201 points at 0.01 interval between [-1.0, 1.0]	
		for (delta = 0, first = 0, sign = 1; delta <= 2; delta += 0.01){
			first = first + delta * sign;
			sign *= -1;
			result = invert(error, _NPV_stub, 0, 0, first, param, MAX_ITERATION, precision);
			CheckValue(error, result);
			if (*error == MathNoError && result >= -100 && result <= 100)
				return result;
			else
				*error = MathNoError;
		}
	}// end of method == EnumMethodSecant
	// Newton's method
	else {
		result = _IRR_NewtonMethod(error, _NPV_stub, guess, param, MAX_ITERATION, precision);
		CheckValue(error, result);
		if (*error == MathNoError && result > -1 && result <= 10)
			return result;
		else
			*error = MathNoError;
	}
    
	SetMathError(error, MathConvergence);
    
	return result;
}

//Returns the modified internal rate of return for a series of periodic cash flows.
// MIRR considers both the cost of the investment and the interest received on 
//reinvestment of cash.
double _MIRR(enum MathError *error, Int32 nValue, double values[], double financeRate,
			 double reinvestRate){
    // Veracode: CWE ID 191: Integer Underflow (Wrap or Wraparound)
	if (nValue < 0) nValue = 0;
    // Veracode
	double *posCashFlows = new double [nValue];
	double *negCashFlows = new double [nValue];
    
	memset(posCashFlows, 0, nValue * sizeof(double));
 	memset(negCashFlows, 0, nValue * sizeof(double));
    
	BOOL bHasNegtives = FALSE;
	Int32 i = 0;
	for (i = 0; i < nValue; i ++){
		if (DoubleGreaterEqual(values[i],0) )
			posCashFlows[i] = values[i];
		else {
			negCashFlows[i] = values[i];
			bHasNegtives = TRUE;
		}
	}
    
	//no negtive cash flow, wrong input
	double result = 0;
	if (!bHasNegtives || nValue < 2){
		SetMathError(error, MathOverflow);
	} else{
		double posNFV = _NPV(error, reinvestRate, nValue, posCashFlows) * 
        pow(1 + reinvestRate, nValue);
		double negNPV = _NPV(error, financeRate, nValue, negCashFlows) * ( 1 + financeRate);
        
		result = pow(-1 * posNFV / negNPV, 1.0 / (nValue - 1)) - 1;
		CheckValue(error, result);
	}	
    
	delete [] posCashFlows;
	delete [] negCashFlows;
    
	return result;
}


//Returns the nominal annual interest rate, given the effective rate and the number
// of compounding periods per year.
double _NOMINAL(enum MathError *error, double effect_rate, Int32 npery){
	if (effect_rate < 0 || npery < 1){
		SetMathError(error, MathDomain);
		return 0;
	}
	
	double result = (pow(1 + effect_rate, 1 / (double)npery) - 1) * npery;
	CheckValue(error, result);
    
	return result;
}


//Returns the number of periods for an investment based on periodic, constant 
//payments and a constant interest rate.
double _NPER(enum MathError *error, double rate, double pmt, double pv, double fv, 
			 Int32 type){
	double result;
	
	if (ApproxiEqual(rate, 0.0)){
		result = -(pv + fv) / pmt;
	} else {
		result = log((pmt * ( 1 + rate * type) - fv * rate) / (pv * rate + pmt * (1 + rate * type)))
        / log(1 + rate);
        
	}
    
	CheckValue(error, result);
    
	return result;
}


//Calculates the net present value of an investment by using a discount rate and 
//a series of future payments (negative values) and income (positive values).
double _NPV(enum MathError *error, double rate, Int32 nValue, double values[]){
	double result = 0;
	double powRate = 1 + rate;
    
	Int32 i = 0;
	for (i = 0; i < nValue; i++){
		result += values[i] / powRate;
		powRate *= (1 + rate);
	}
    
	CheckValue(error, result);
    
	return result;
    
}


//Calculates the payment for a loan based on constant payments and a constant interest rate.
double _PMT(enum MathError *error, double rate, double nper, double pv, double fv, Int32 type){
	double result;
    
    
	if (type < 0 || type > 1){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	if (ApproxiEqual(rate, 0.0)){
		result = (pv + fv) / nper * -1;
	} else {
		result = (pv * pow(1 + rate, nper) + fv) * rate / (1 + rate * type) 
        / (pow(1 + rate, nper) - 1) * -1;
	}
    
	CheckValue(error, result);
    
	return result;
    
}

//Returns the payment on the principal for a given period for an investment based 
//on periodic, constant payments and a constant interest rate.
double _PPMT(enum MathError *error, double rate, double per, double nper, double pv,
			 double fv, Int32 type){
    
	if (per < 1 || per > nper || type < 0 || type > 1){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	double pmt = _PMT(error, rate, nper, pv, fv, type);
    
	double impt = _IPMT(error, rate, per, nper, pv, fv, type);
    
	double result = pmt - impt;
    
	CheckValue(error, result);
    
	return result;
    
    
}


//Returns the present value of an investment. The present value is the total amount
//that a series of future payments is worth now.
double _PV(enum MathError *error, double rate, double nper, double pmt, double fv,
		   Int32 type){
	double result;
    
	if (ApproxiEqual(rate, 0.0)){
		result = -fv - pmt * nper;
	} else {
		result =  - (fv + pmt * (1 + rate * type)  * (pow(1 + rate, nper) - 1) / rate)
        / pow(1 + rate, nper);
	}
    
	CheckValue(error, result);
    
	return result;
    
}

//Returns the interest rate per period of an annuity. RATE is calculated by 
//iteration and can have zero or more solutions
double _RATE(enum MathError *error, double nper, double pmt, double pv, double fv,
			 Int32 type, double guess){
	const void *param[5];
	param[0] = &nper;
	param[1] = &pmt;
	param[2] = &pv;
	param[3] = &fv;
	param[4] = &type;
    
	double first, result;
	
	Int32 i = 0;
    
	//try 5 points between guess and 1
	for (i = 1; i <= 5; i ++){
		first = (1 - guess) * 0.2 * i + guess;
		result = invert(error, _RATE_stub, 0, first, guess, param, 1000, 0.0000001);
		CheckValue(error, result);
		if (*error == MathNoError && result >= -1 && result <= 1)
			return result;
		else
			*error = MathNoError;
        
	}
    
	//try 5 points between 0 and guess) 
	for (i = 4; i >= 0 ; i --){
		first = guess * 0.2 * i;
		result = invert(error, _RATE_stub, 0, first, guess, param, 1000, 0.0000001);
		CheckValue(error, result);
		if (*error == MathNoError && result >= -1 && result <= 1)
			return result;
		else
			*error = MathNoError;
	}
    
	
	SetMathError(error, MathConvergence);
    
	return result;
}


//Returns the straight-line depreciation of an asset for one period.
double _SLN(enum MathError *error, double cost, double salvage, double life){
	//XW:fix domain error
	if (cost < 0 || salvage < 0 || life <= 0)
	{
		SetMathError(error, MathDomain);
		return 0;
	}
    
	double result = (cost - salvage) / life;
	
	CheckValue(error, result);
    
	return result;
}

//Returns the sum-of-years' digits depreciation of an asset for a specified period.
double _SYD(enum MathError *error, double cost, double salvage, double life, 
            double period) {
	//XW:fix domain error
	if (cost < 0 || salvage < 0 || life <= 0 || period <= 0 || period > life)
	{
		SetMathError(error, MathDomain);
		return 0;
	}
    
    
	double result = (cost - salvage) * (life - period + 1) * 2 /life / (life + 1); 
    
	CheckValue(error, result);
    
	return result;
}

//switches to straight-line depreciation when depreciation is greater than 
//the declining balance calculation.
double _VDB(enum MathError *error, double cost, double salvage, double life, 
			Int32 period, double factor) {
	
	if (life < 0 || period > life || period <= 0 || cost < 0 || salvage < 0 || factor <= 0 ){  
		SetMathError(error, MathDomain);
		return 0;
	}
    
	double curCost = cost; // current cost of the asset
	double ddb;
	double sln;
	double result = 0;
	for (Int32 i = 1; i <= period; i ++){
		ddb = _DDB(error, cost, salvage, life, i, factor);
		sln = _SLN(error, curCost, salvage, life - i + 1);
		if (*error != MathNoError)
			return 0;
        
		if (DoubleLessThan(ddb, sln) ) 
			break;
		
        
		curCost -= ddb;
	}
    
	result = DoubleLessThan(ddb, sln) ? sln : ddb;
    
	CheckValue(error, result);
    
	return result;
    
}


//Author:Andrea
//Modified by Xiaonan Han to work for finanical functions
// func points to a function stub which is a wrapper of the financial function
// first and second are two initial values for iterations
// params is for the rest of the parameters needed by the financial function 
// nLimits is the limit for iteration number.
double invert (enum MathError *errorFlag, double(*func)(enum MathError *, double, const void * []), double input, 
			   double first, double second, const void * params[], Int32 nLimits, double err )
{
	double d, df, x1, f1, f0;
	
	// the initial point at 0
	f0 = func(errorFlag, first, params) - input;
	//f0 = func(errorFlag, second, params) - input;
    
	if (*errorFlag != MathNoError)
		return 0;
    
	// ytang, TQMS285745, Aug25,2008
	// return first(=0) if the first try yields 0
	if (fabs(f0) <= err)		
		return first;
    
	x1 = second;
	d = second - first;
    
	Int32 i = 0;
	while ( fabs(f0) > err && i < nLimits)
	{
		f1 = func(errorFlag, x1, params) - input;
		if (*errorFlag != MathNoError)
			return 0;
		
		df=f1-f0;
		if ( df != 0.0) 
		{
			d *= -f1 / df;
			x1 += d;
			f0 = f1;
		}
		else
		{
			// handle low slope case
			break; 
		}
        
		i ++;
	}
    
	// qjiang 10/27/2009 TQMS 378442 
	// in Secant Method, the convergence condition can be fabs(d) < E
	if (fabs(d) < 1E-15 && fabs(f0) < MAXERROR)
	{
		// already converged
	}
	else if (fabs(f0) > err)
	{
		SetMathError(errorFlag, MathConvergence);
	}
    
	return x1;
}

// used by IRR
double _NPV_stub(enum MathError *error, double rate, const void *param[]){
	Int32 nSize = *((Int32 *)(param[0]));
	double *values = (double *)param[1];
    
	double result = _NPV(error, rate, nSize, values);
	CheckValue(error, result);
    
	return result;
	
}

//use by Rate to do the iteration in invert
double _RATE_stub(enum MathError *error, double rate, const void *param[]){
	double nper = *((double *) param[0]);
	double pmt  = *((double *) param[1]);
	double pv   = *((double *) param[2]);
	double fv	= *((double *) param[3]);
	Int32	   type = *((Int32  *)   param[4]);
	
	
    
	double result;
	if (ApproxiEqual(rate, 0.0)){
		result = pv + pmt * nper + fv;
	} else {
		result = pv * pow(1 + rate, nper) 
        + pmt * (1 + rate * type) * (pow(1 + rate, nper) - 1)/ rate 
        + fv;
	}
    
	CheckValue(error, result);
    
	return result;
	
}




BOOL isLegal(const Date & settlement, const Date & maturity, Int32 frequency, Int32 basis){
	if (!isLegalDate(settlement) || ! isLegalDate(maturity) || settlement >= maturity
		|| basis < 0 || basis > 5 
		|| (frequency != 1 && frequency != 2 && frequency != 4))
		return FALSE;
	else 
		return TRUE;
}

//Returns the accrued interest for a security that pays periodic interest.
double _ACCRINT(enum MathError *error, const Date & issue, const Date & /*firstInterest*/, 
				const Date & settlement, double rate, double par, Int32 frequency, Int32 basis){
	
	if (!isLegal(issue, settlement, 1, basis) || rate < 0 || par < 0 ){
		SetMathError(error, MathDomain);
		return 0;
	}
    
    
	Basis eBasis = (Basis) basis;
	double result = 0;
    
	if (eBasis == eBasisActualActual){
        
		Int32 nQuasi = 0;
		//initialize span from frequency
		Int32 span = 12 / frequency;
        
		Date quasi1 = issue;
		Date quasi2 = shiftByMonth(issue, span);
		//adjustDate(quasi2, issue);
        
		while (quasi2 < settlement){
			nQuasi ++;
			quasi1 = quasi2;
			quasi2 = shiftByMonth(quasi2, span);
            //	adjustDate(quasi2, issue);
		}
        
		double fraction = (double) (difference(settlement, quasi1, eBasis))
        /  difference(quasi2, quasi1, eBasis);
        
		result = par * rate / frequency * (nQuasi + fraction);
	} else {
        
		Int32 D = daysPerYear(eBasis);
		Int32 diff = difference(settlement, issue, eBasis);
		
		//formula from "Investment Management Second Edition" F.J.Fabozzi P482 
		result = par * rate / D * diff; 
	}
    
	CheckValue(error, result);
    
	return result;
    
}	


//Returns the accrued interest for a security that pays interest at maturity.
double _ACCRINTM(enum MathError *error, const Date & issue, const Date & maturity, double rate, double par, Int32 basis){
	if (!isLegal(issue, maturity, 1, basis) || rate < 0 || par < 0 ){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	Basis eBasis = (Basis) basis;
	Int32 A = difference(maturity, issue, eBasis);
	Int32 D = daysPerYear(eBasis);
    
	double result = par * rate * A / D;
    
	CheckValue(error, result);
    
	return result;
}

//Returns the number of days from the beginning of the coupon period to the settlement date.
Int32  _COUPDAYBS(enum MathError *error, const Date & settlement, const Date & maturity, Int32 frequency, Int32 basis) {
	Date begin = _COUPPCD(error, settlement, maturity, frequency, basis);
    
	if (*error != MathNoError)
		return 0;
    
	Basis eBasis = (Basis) basis;
	Int32 result = difference(settlement, begin, eBasis); 
	
	return result;
    
}

//Returns the number of days in the coupon period that contains the settlement date.
double  _COUPDAYS(enum MathError *error, const Date & settlement, const Date & maturity, Int32 frequency, Int32 basis) {
	if (!isLegal(settlement, maturity, frequency, basis) ){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	double result = 0;
    
	Basis eBasis = (Basis) basis;
    
	if (eBasis == eBasisActualActual){
		Date begin = _COUPPCD(error, settlement, maturity, frequency, basis);
		Date end = _COUPNCD(error, settlement, maturity, frequency, basis);
        
		if (*error != MathNoError)
			return 0;
		result = difference(end, begin, eBasis); 
	} else {
		Int32 D = daysPerYear(eBasis);
        
		result = (double)D / frequency;
	}
    
	return result;
    
}

//Returns the number of days from the settlement date to the next coupon date.
Int32  _COUPDAYSNC(enum MathError *error, const Date & settlement, const Date & maturity, Int32 frequency, Int32 basis) {
    
	Int32 result = 0;
	if (basis == 0){
		Int32 coupdays = (Int32)_COUPDAYS(error, settlement, maturity, frequency, basis);
		Int32 coupbs = (Int32)_COUPDAYBS(error, settlement, maturity, frequency, basis);
        
		if (*error != MathNoError)
			return 0;
        
		result = coupdays - coupbs;
	} else {
        
		Date next = _COUPNCD(error, settlement, maturity, frequency, basis);
        
		if (*error != MathNoError)
			return 0;
        
		Basis eBasis = (Basis) basis;
		result = difference(next, settlement, eBasis); 
	}
    
	return result;
    
}

//Returns the number of coupons payable between the settlement date and maturity 
//date, rounded up to the nearest whole coupon.
Int32  _COUPNUM(enum MathError *error, const Date & settlement, const Date & maturity, Int32 frequency, Int32 basis) {
	if (!isLegal(settlement, maturity, frequency, basis) ){
		SetMathError(error, MathDomain);
		return 0;
	}
	//Basis eBasis = (Basis) basis;
    
	// initialize span by frenquncey 
	Int32 span = - 12 / frequency;
    
	Int32 result = 1;
	Date preCouponDate = shiftByMonth(maturity, span);
	adjustDate(preCouponDate, maturity);
    
	while (preCouponDate > settlement){
		result ++;
		preCouponDate = shiftByMonth(preCouponDate, span);
		adjustDate(preCouponDate, maturity);
	}
    
	return result;
    
}

//Returns a date that represents the next coupon date before the settlement date.
Date _COUPNCD(enum MathError *error, const Date & settlement, const Date & maturity, Int32 frequency, Int32 basis) {
	Date result;
	
	if (!isLegal(settlement, maturity, frequency, basis) ){
		SetMathError(error, MathDomain);
		return result;
	}
	//Basis eBasis = (Basis) basis;
	
    
	// initialize span by frenquncey 
	Int32 span = - 12 / frequency;
    
	result = maturity;
    
	Date preDate = shiftByMonth(result, span);
	adjustDate(preDate, maturity);
    
	while (preDate > settlement) {
		result = preDate;
		preDate = shiftByMonth(result, span);
		adjustDate(preDate, maturity);
	}
    
	if (!isLegalDate(result))
		SetMathError(error, MathDomain);
    
	return result;
    
}


//Returns a date that represents the previous coupon date before the settlement date.
Date _COUPPCD(enum MathError *error, const Date & settlement, const Date & maturity, Int32 frequency, Int32 basis) {
	Date result; 
    
	if (!isLegal(settlement, maturity, frequency, basis) ){
		SetMathError(error, MathDomain);
		return result;
	}
	//Basis eBasis = (Basis) basis;
	
	// initialize span by frenquncey 
	Int32 span = - 12 / frequency;
    
	result = maturity;
    
	while (result > settlement) {
		result = shiftByMonth(result, span);
		adjustDate(result, maturity);
	}
    
	if (!isLegalDate(result))
		SetMathError(error, MathDomain);
    
	return result;
    
}


//Returns the discount rate for a security.
double _DISC(enum MathError *error, const Date & settlement, const Date & maturity, double pr,
			 double redemption, Int32 basis){
	if (!isLegal(settlement, maturity, 1, basis) || pr < 0 || redemption < 0 ){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	Basis eBasis = (Basis) basis;
    
	Int32 DSM = difference(maturity, settlement, eBasis);
	Int32 B = daysPerYear(eBasis);
    
	double result = (redemption - pr) / redemption * B / DSM;
    
	CheckValue(error, result);
    
	return result;
    
}

//Returns the Macauley duration for an assumed par value of $100
double _DURATION(enum MathError *error, const Date & settlement, const Date & maturity,
				 double couponRate, double  yldRate, Int32 frequency, Int32 basis){
    
	if (!isLegal(settlement, maturity, frequency, basis) || couponRate < 0 
		|| yldRate < 0 ){
		SetMathError(error, MathDomain);
		return 0;
	}
	//Basis eBasis = (Basis) basis;
    
    
    //	int DSC = _COUPDAYSNC(error, settlement, maturity, frequency, basis);
	double  E = _COUPDAYS(error, settlement, maturity, frequency, basis);
	Int32 A = _COUPDAYBS(error, settlement, maturity, frequency, basis);
	double DSC = E - A;
    
	Int32 N = _COUPNUM(error, settlement, maturity, frequency, basis);
    
	if (*error != MathNoError)
		return 0;
    
	double rate = (1 + yldRate / frequency); // discount rate for each period
	double firstPeriod = (double) (DSC) / E;
    
	double pvTotal = 0; // total present value of cash flows
	double pvTotalT = 0; // pvTotal * coupon period
    
	Int32 i = 0;
	double pv;
	for (i = 1; i <= N; i ++){
		pv = 100 * couponRate / frequency / pow(rate, i - 1 + firstPeriod);
		pvTotal += pv;
		pvTotalT += pv * (i - 1 + firstPeriod);
	}
    
	pv = 100 / pow(rate, N - 1 + firstPeriod);
	pvTotal += pv;
	pvTotalT += pv * (N - 1 + firstPeriod);
    
	double result = pvTotalT / pvTotal / frequency;
    
	CheckValue(error, result);
    
	return result;
    
}

//Returns the modified duration for a security with an assumed par value of $100.
double _MDURATION(enum MathError *error, const Date & settlement, const Date & maturity,
                  double couponRate, double  yldRate, Int32 frequency, Int32 basis){
	double duration = _DURATION(error, settlement, maturity, couponRate, yldRate, 
								frequency, basis);
	
	if (*error != MathNoError)
		return 0;
    
	double result = duration / (1 + yldRate / frequency);
    
	CheckValue(error, result);
    
	return result;
}

//Returns the interest rate for a fully invested security.
double _INTRATE(enum MathError *error, const Date & settlement, const Date & maturity, double investment,
				double redemption, Int32 basis) {
	if (!isLegal(settlement, maturity, 1, basis) ||  investment <= 0 || redemption <= 0 ){
		SetMathError(error, MathDomain);
		return 0;
	}
	Basis eBasis = (Basis) basis;
    
	Int32 B = daysPerYear(eBasis);
	Int32 DSM = difference(maturity, settlement, eBasis);
    
	double result = (redemption - investment) / investment * B / DSM;
    
	CheckValue(error, result);
    
	return result;
    
}

//Returns the price per $100 face value of a security having an odd (short or long) first period.
double _ODDFPRICE(enum MathError *error, const Date & settlement, const Date & maturity, const Date & issue,
				  const Date & firstCoupon, double couponRate, double yldRate, double redemption, 
				  Int32 frequency, Int32 basis) {
    
	if (!isLegal(settlement, maturity, frequency, basis) || !isLegalDate(issue) 
		|| !isLegalDate(firstCoupon)
		|| !(issue < settlement && settlement < firstCoupon && firstCoupon < maturity)
		||  couponRate < 0 	|| yldRate < 0 || redemption <= 0){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	Basis eBasis = (Basis) basis;
    
	Int32 i = 0;
	double result = 0;
	double pvTotal = 0; // total present value of cash flows
	double rate = (1 + yldRate / frequency); // discount rate for each period
    
	Int32 firstCouponDays = difference(firstCoupon, issue, eBasis);
	double  E = _COUPDAYS(error, settlement, maturity, frequency, basis);
    
	if (firstCouponDays <= E){ 
		//the first period is odd short period
		Int32 A = difference(settlement, issue, eBasis);
		Int32 DSC = difference(firstCoupon, settlement, eBasis);
		Int32 DFC = A + DSC;
		Int32 N = _COUPNUM(error, settlement, maturity, frequency, basis);
        
		double firstPeriod = (double) (DSC) / E;
        
		pvTotal = redemption / pow(rate, N - 1 + firstPeriod);
		pvTotal += 100 * couponRate / frequency * DFC / E / pow(rate, firstPeriod);
        
		for (i = 2; i <= N; i ++)
			pvTotal += 100 * couponRate / frequency / pow(rate, i - 1 + firstPeriod);
        
		result = pvTotal - 100 * couponRate / frequency * A / E;
        
	} else {
		// the first period is odd long period
		//first quasi coupon date from settlement 
		Date firstQuasiDateSettle = _COUPNCD(error, settlement, maturity, frequency, eBasis);		
		// number of whole qusi periods from settlement to firstCouponDate
		Int32 Nq = _COUPNUM(error, settlement, firstCoupon, frequency, basis) - 1;
		// number of days from settlement to first qusicoupon date
		Int32 DSC = difference(firstQuasiDateSettle, settlement, eBasis);
		// number of  normal period
		Int32 nNormal = _COUPNUM(error, firstCoupon, maturity, frequency, basis); 
		
		//number of whole quasi periods from issue date to firstCouponDate
		Int32 nQuasi = _COUPNUM(error, issue, firstCoupon, frequency, basis) - 1;
		//first quasi coupon date from issue
		Date firstQuasiDateIssue = _COUPNCD(error, issue, maturity, frequency, eBasis);
		//number of days from issue to first quasi date
		Int32 A0 = difference(firstQuasiDateIssue, issue, eBasis);
		//normal length of issue quasi period
		double NL0 = _COUPDAYS(error, issue, maturity, frequency, eBasis);
        
		//number of quasi periods from settle to firstCouponDate
		double fNSettle = Nq + (double)(DSC) / E;
		
		//number of quasi periods from issue to firstCouponDate
		double fNIssue = nQuasi + (double) (A0) / NL0;
		
		//number of quasi periods between issue and settle
		double fNIssueSettle = fNIssue - fNSettle;
        
		pvTotal = redemption / pow(rate, nNormal + fNSettle);
		pvTotal += 100 * couponRate / frequency * fNIssue / pow(rate, fNSettle);
        
		for (i = 1; i <= nNormal; i ++)
			pvTotal += 100 * couponRate / frequency / pow(rate, i + fNSettle);
        
		result = pvTotal - 100 * couponRate / frequency * fNIssueSettle;
	}
    
	CheckValue(error, result);
    
	return result;
    
}

//used by _ODDFYIELD()
double _ODDFPRICE_stub(enum MathError *error, double yield, const void *param[]){
	Date	settlement	= *((Date *)(param[0]));
	Date	maturity	= *((Date *)(param[1]));
	Date	issue		= *((Date *)(param[2]));
	Date	firstCoupon	= *((Date *)(param[3]));
	double	couponRate	= *((double *)(param[4]));
	double	redemption	= *((double *)(param[5]));
	Int32		frequency	= *((Int32 *)(param[6]));
	Int32		basis		= *((Int32 *)(param[7]));
    
    
	double result = _ODDFPRICE(error, settlement, maturity, issue, firstCoupon, 
                               couponRate, yield, redemption, frequency, basis);
	CheckValue(error, result);
    
	return result;
	
}

//Returns the yield of a security that has an odd (short or long) first period.
double _ODDFYIELD(enum MathError *error, const Date & settlement, const Date & maturity, const Date & issue,
				  const Date & firstCoupon, double couponRate, double price, double redemption,
				  Int32 frequency, Int32 basis) {	
	double guess = couponRate;
	const void *param[8];
	param[0] = &settlement;
	param[1] = &maturity;
	param[2] = &issue;
	param[3] = &firstCoupon;
	param[4] = &couponRate;
	param[5] = &redemption;
	param[6] = &frequency;
	param[7] = &basis;
    
    
	double result = invert(error, _ODDFPRICE_stub, price, 0, guess, param, 1000, 0.0000001);
	CheckValue(error, result);
    
	return result;
    
    
}


//Returns the price per $100 face value of a security having an odd (short or long)
// last coupon period.
double _ODDLPRICE(enum MathError *error, const Date & settlement, const Date & maturity, 
				  const Date & lastInterest, double couponRate, double yldRate, double redemption,
				  Int32 frequency, Int32 basis){
    
	if (!isLegal(settlement, maturity, frequency, basis) || !isLegalDate(lastInterest)
		|| !(lastInterest < settlement) || couponRate < 0 || yldRate < 0 
		|| redemption <= 0){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	Basis eBasis = (Basis) basis;
	// first quasi coupon date from last interest
	Date firstQuasiLIn = _COUPNCD(error, lastInterest, maturity, frequency, basis);
	// the number of days in quasi coupon period of last interest
	double nLInCouponDays = _COUPDAYS(error, lastInterest, maturity, frequency, basis);
	//the number of whole quasi coupon periods from last interest to maturity
	Int32 nQuasiLIn = _COUPNUM(error, lastInterest, maturity, frequency, basis) - 1;
	//count in fraction period
	double fQuasiLIn = nQuasiLIn + 
    (double)(difference(firstQuasiLIn, lastInterest, eBasis)) / nLInCouponDays;
    
	// first quasi coupon date from  settlement
	Date firstQuasiSettle = _COUPNCD(error, settlement, maturity, frequency, basis);
	// the number of days in quasi coupon period of settlement
	double nSettleCouponDays = _COUPDAYS(error, settlement, maturity, frequency, basis);
	//the number of whole quasi coupon periods from settlement to maturity
	Int32 nQuasiSettle = _COUPNUM(error, settlement, maturity, frequency, basis) - 1;
	//count in fraction period
	double fQuasiSettle = nQuasiSettle + 
    (double)(difference(firstQuasiSettle, settlement, eBasis)) / nSettleCouponDays;
    
	double result = (redemption + fQuasiLIn * 100 * couponRate / frequency) 
    / (1 + fQuasiSettle * yldRate / frequency) 
    - (fQuasiLIn - fQuasiSettle) * 100 * couponRate / frequency;
	
	CheckValue(error, result);
    
	return result;
}

//Returns the yield of a security that has an odd (short or long) last period.
double _ODDLYIELD(enum MathError *error, const Date & settlement, const Date & maturity, 
				  const Date & lastInterest, double couponRate, double price, double redemption,
				  Int32 frequency, Int32 basis){
	if (!isLegal(settlement, maturity, frequency, basis) || !isLegalDate(lastInterest)
		|| !(lastInterest < settlement) || couponRate < 0 || price <= 0 
		|| redemption <= 0){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	Basis eBasis = (Basis) basis;
	// first quasi coupon date from last interest
	Date firstQuasiLIn = _COUPNCD(error, lastInterest, maturity, frequency, basis);
	// the number of days in quasi coupon period of last interest
	double nLInCouponDays = _COUPDAYS(error, lastInterest, maturity, frequency, basis);
	//the number of whole quasi coupon periods from last interest to maturity
	Int32 nQuasiLIn = _COUPNUM(error, lastInterest, maturity, frequency, basis) - 1;
	//count in fraction period
	double fQuasiLIn = nQuasiLIn + 
    (double)(difference(firstQuasiLIn, lastInterest, eBasis)) / nLInCouponDays;
    
	// first quasi coupon date from  settlement
	Date firstQuasiSettle = _COUPNCD(error, settlement, maturity, frequency, basis);
	// the number of days in quasi coupon period of settlement
	double nSettleCouponDays = _COUPDAYS(error, settlement, maturity, frequency, basis);
	//the number of whole quasi coupon periods from settlement to maturity
	Int32 nQuasiSettle = _COUPNUM(error, settlement, maturity, frequency, basis) - 1;
	//count in fraction period
	double fQuasiSettle = nQuasiSettle + 
    (double)(difference(firstQuasiSettle, settlement, eBasis)) / nSettleCouponDays;
    
	double temp = price + (fQuasiLIn - fQuasiSettle) * 100 * couponRate / frequency;
	double result = ((redemption + fQuasiLIn * 100 * couponRate / frequency) - temp)
    / temp * frequency / fQuasiSettle;
    
	CheckValue(error, result);
    
	return result;
    
}

//returns the price per $100 face value of a security that pays periodic interest.
double _PRICE(enum MathError *error, const Date & settlement, const Date & maturity,
              double couponRate, double  yldRate, double redemption, 
              Int32 frequency, Int32 basis){
    
	if (!isLegal(settlement, maturity, frequency, basis) || couponRate < 0 
		|| yldRate < 0 || redemption <= 0){
		SetMathError(error, MathDomain);
		return 0;
	}
	//Basis eBasis = (Basis) basis;
    
    //	int DSC = _COUPDAYSNC(error, settlement, maturity, frequency, basis);
	double E = _COUPDAYS(error, settlement, maturity, frequency, basis);
	Int32 N = _COUPNUM(error, settlement, maturity, frequency, basis);
	Int32 A = _COUPDAYBS(error, settlement, maturity, frequency, basis);
	double DSC = E - A;
	
	if (*error != MathNoError)
		return 0;
    
	double pvTotal = 0; // total present value of cash flows
    
	if (N == 1) { // settlement and maturity are in the same coupon period
		double rate = 1 + yldRate / frequency * DSC / E;
        
		pvTotal = (redemption + 100 * couponRate / frequency) / rate; 
		
	} else {
		double rate = (1 + yldRate / frequency); // discount rate for each period
		double firstPeriod = (double) (DSC) / E;
        
		pvTotal = redemption / pow(rate, N - 1 + firstPeriod);
        
        
		for (Int32 i = 1; i <= N; i ++)
			pvTotal += 100 * couponRate / frequency / pow(rate, i - 1 + firstPeriod);
	}
    
	double result = pvTotal - 100 * couponRate / frequency * A / E;
    
	CheckValue(error, result);
    
	return result;
    
}

//used by yield()
double _PRICE_stub(enum MathError *error, double yield, const void *param[]){
	Date	settlement	= *((Date *)(param[0]));
	Date	maturity	= *((Date *)param[1]);
	double	couponRate	= *((double *)param[2]);
	double	redemption	= *((double *)param[3]);
	Int32		frequency	= *((Int32 *)param[4]);
	Int32		basis		= *((Int32 *)param[5]);
    
    
	double result = _PRICE(error, settlement, maturity, couponRate, yield, redemption,
                           frequency, basis);
	CheckValue(error, result);
    
	return result;
	
}
//Returns the yield on a security that pays periodic interest. 
double _YIELD(enum MathError *error, const Date & settlement, const Date & maturity,
              double couponRate, double  price, double redemption, 
              Int32 frequency, Int32 basis){
    
	if (!isLegal(settlement, maturity, frequency, basis) || couponRate < 0 
		|| price <= 0 || redemption <= 0){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	Basis eBasis = (Basis) basis;
    
	Int32 N = _COUPNUM(error, settlement, maturity, frequency, basis);
	if (*error != MathNoError)
		return 0;
    
	double result;
	if (N == 1) { // we have less one coupon period
		Int32 A = _COUPDAYBS(error, settlement, maturity, frequency, basis);
		double E = _COUPDAYS(error, settlement, maturity, frequency, basis);
		Int32 DSR = difference(maturity, settlement, eBasis);
        
		double rate = couponRate / frequency;
		double temp = price / 100 + A / E * rate;
		result = (redemption / 100 + rate - temp) / temp * frequency * E / DSR;
	} else {
		// use iteration to get yield rate
		double guess = couponRate;
		const void *param[6];
		param[0] = &settlement;
		param[1] = &maturity;
		param[2] = &couponRate;
		param[3] = &redemption;
		param[4] = &frequency;
		param[5] = &basis;
		
        
		result = invert(error, _PRICE_stub, price, 0, guess, param, 100, 0.0000001);
        
	}
	
	CheckValue(error, result);
    
	return result;
    
}



//Returns the price per $100 face value of a discounted security.
double _PRICEDISC(enum MathError *error, const Date & settlement, const Date & maturity, 
				  double discRate, double redemption, Int32 basis){
	if (!isLegal(settlement, maturity, 1, basis) ||  discRate <= 0 || redemption <= 0 ){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	Basis eBasis = (Basis) basis;
    
	Int32 B = daysPerYear(eBasis);
	Int32 DSM = difference(maturity, settlement, eBasis);
    
	double result = redemption * (1 - discRate * DSM / B);
    
	CheckValue(error, result);
    
	return result;
}

//Returns the price per $100 face value of a security that pays interest at maturity.
double _PRICEMAT(enum MathError *error, const Date & settlement, const Date & maturity, 
				 const Date & issue, double rate, double yld, Int32 basis) {
    
	if (!isLegal(settlement, maturity, 1, basis) || !isLegalDate(issue) 
		|| settlement < issue || rate < 0 || yld < 0 ){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	Basis eBasis = (Basis) basis;
    
	Int32 A = difference(settlement, issue, eBasis);
	Int32 B = daysPerYear(eBasis);
	Int32 DSM = difference(maturity, settlement, eBasis);
	Int32 DIM = difference(maturity, issue, eBasis);
    
	double result = (100 + rate * 100 * DIM / B) / ( 1 + yld * DSM / B)
    - (rate * 100 * A / B);
    
	CheckValue(error, result);
    
	return result;
}

//Returns the amount received at maturity for a fully invested security.
double _RECEIVED(enum MathError *error, const Date & settlement, const Date & maturity, double investment,
				 double discount, Int32 basis){
	
	if (!isLegal(settlement, maturity, 1, basis) ||  discount <= 0 || investment <= 0 ){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	Basis eBasis = (Basis) basis;
    
	Int32 B = daysPerYear(eBasis);
	Int32 DSM = difference(maturity, settlement, eBasis);
    
	double result = investment / ( 1 - discount * DSM / B);
    
	CheckValue(error, result);
    
	return result;
}

//Returns the bond-equivalent yield for a treasury bill.
double _TBILLEQ(enum MathError *error, const Date & settlement, const Date & maturity, double discount){
	
	Int32 DSM = difference(maturity, settlement, eBasisActual360);
    
	if (!isLegalDate(settlement) || !isLegalDate(maturity) ||
		settlement > maturity || discount <= 0 || DSM > 365){
		SetMathError(error, MathDomain);
		return 0;
	}
    
    
	double result = 365 * discount /( 360 - discount * DSM);
	CheckValue(error, result);
    
	return result;
}

//Returns the price per $100 face value for a treasury bill.
double _TBILLPRICE(enum MathError *error, const Date & settlement, const Date & maturity, double discount){
	Int32 DSM = difference(maturity, settlement, eBasisActual360);
    
	if (!isLegalDate(settlement) || !isLegalDate(maturity) ||
		settlement > maturity || discount <= 0 || DSM > 365){
		SetMathError(error, MathDomain);
		return 0;
	}
    
    
 	double result = 100 * ( 1 - discount * DSM / 360);
	CheckValue(error, result);
    
	return result;
}

//Returns the yield for a treasury bill.
double _TBILLYIELD(enum MathError *error, const Date & settlement, const Date & maturity, double pr){
	Int32 DSM = difference(maturity, settlement, eBasisActual360);
    
	if (!isLegalDate(settlement) || !isLegalDate(maturity) ||
		settlement > maturity || pr <= 0 || DSM > 365){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	double result = (100 - pr) / pr * 360 / DSM;
	CheckValue(error, result);
    
	return result;
    
}


//Returns the net present value for a schedule of cash flows that is not 
//necessarily periodic. 
double _XNPV(enum MathError *error, double rate,Int32 n, double values[], Date dates[],Int32 valid[]){
    
	Int32 i = 0;
    Int32 min=0;
	for (i = 0; i < n; i ++)
    {
        if(valid[i]!=2)
        {
            continue;
        }
		if ( !isLegalDate(dates[i])){
			SetMathError(error, MathDomain);
			return 0;
		}
        if(dates[min]>dates[i])
        {
            min=i;
        }
    }
    
	double result = 0.0;
    
	for (i = 0; i < n; i ++)
    {
        if(valid[i]!=2)
        {
            continue;
        }
		result += values[i] / pow(1 + rate, difference(dates[i], dates[min], eBasisActualActual) / (double)365);
    }
    
	CheckValue(error, result);
    
	return result;
    
}

// used by _XIRR
double _XNPV_stub(enum MathError *error, double rate, const void *param[]){
	Int32 nSize = *((Int32 *)(param[0]));
	double *values = (double *)param[1];
	Date *dates = (Date *)param[2];
    Int32 *valid=(Int32*)param[3];
    
	double result = _XNPV(error, rate, nSize, values, dates,valid);
	CheckValue(error, result);
    
	return result;
	
}
//Returns the internal rate of return for a schedule of cash flows that is not 
//necessarily periodic.
double _XIRR(enum MathError *error, Int32 n, double values[], Date dates[], double guess,Int32 valid[]){
    
	const void *param[4];
	param[0] = &n;
	param[1] = values;
	param[2] = dates;
    param[3] = valid;
    
	//double result = invert(error, _XNPV_stub, 0, 0, guess, param, 100, 0.0000001);
    double result=_XIRR_NewtonMethod(error, _XNPV_stub, guess, param, 100, 0.0000001);
	CheckValue(error, result);
    
	return result;
}



//Returns the annual yield for a discounted security.
double  _YIELDDISC(enum MathError *error, const Date & settlement, const Date & maturity, double price,
                   double redemption, Int32 basis){
	
	if (!isLegal(settlement, maturity, 1, basis) ||  price <= 0 || redemption <= 0 ){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	
	Basis eBasis = (Basis) basis;
	Int32 B = daysPerYear(eBasis);
	Int32 DSM = difference(maturity, settlement, eBasis);
    
	double result = (redemption - price) / price * B / DSM;
    
	CheckValue(error, result);
    
	return result;
}

//Returns the annual yield of a security that pays interest at maturity.
double _YIELDMAT(enum MathError *error, const Date & settlement, const Date & maturity, const Date & issue,
				 double rate, double price, Int32 basis){
    
	if (!isLegal(settlement, maturity, 1, basis) || !isLegalDate(issue) 
		|| settlement < issue || rate < 0 || price < 0 ){
		SetMathError(error, MathDomain);
		return 0;
	}
    
	Basis eBasis = (Basis) basis;
	Int32 A = difference(settlement, issue, eBasis);
	Int32 B = daysPerYear(eBasis);
	Int32 DSM = difference(maturity, settlement, eBasis);
	Int32 DIM = difference(maturity, issue, eBasis);
    
	double temp = price / 100 + rate * A / B;
	double result = (1 + rate * DIM / B - temp ) / temp * B / DSM;
    
	CheckValue(error, result);
    
	return result;
}

//adjust result date according to maturity date
void adjustDate(Date & result, const Date & maturity){
    
    if (maturity.GetDay() == maturity.GetDaysInMonth())
        result.PutDay(result.GetDaysInMonth());
}

// ytang, IRR using Newton's method, 2009-06-14
// Returns the internal rate of return for a series of cash flows 
// func		- pointer to the NPV function
// guess	- user's guess for IRR, default = 0.1 
// params	- params[0]: number of cash flows, params[1]: the array of cash flows.
// nLimits	- maximum allowed number of iterations before giving up.  
// err		- approximation factor, f(x) <= err will be considered f(x) = 0. default = 0.00001. Note with too small an err IRR 
//            would take longer to converge, sometimes may never converge. 
double _IRR_NewtonMethod(enum MathError *errorFlag, double(*func)(enum MathError *, double, const void * []),  
                         double guess, const void * params[], Int32 nLimits, double err )
{
	Int32 i = 0;
	double x = guess;
	double df = 0.0;
	
	while ( fabs(func(errorFlag, x, params)) > err && i < nLimits)
	{
		// Test to see if the derivative is correct. TO BE REMOVED BEFORE CHECK IN!
		double df_test;
		double delta_test = 1.0E-10;
		double f_x, f_x_plus_d;
		f_x = func(errorFlag, x, params);
		f_x_plus_d = func(errorFlag, x + delta_test, params);
		df_test =  (f_x_plus_d - f_x) / delta_test; // End of test
        
		df = _NPV_Df(errorFlag, x, params); 
        
		if ( df != 0.0) 
		{			
			x = x - ( func(errorFlag, x, params) / df );
		}
		else
		{
			// handle low slope case
			break; 
		}
        
		i ++;
	}
    
	if (fabs(func(errorFlag, x, params)) > err)
		SetMathError(errorFlag, MathConvergence);
    
	return x;	
}

// ytang, 2009-06-15
// Return: The derivative of the _NPV function at [rate]
// F(rate) = _NPV = values[0]/(1+rate) + values[1]/(1+rate)^2 + ... + values[nValue]/(1+rate)^n
// By the Chain rule: F'(x) = F'(g(x))*g'(x)
// Df(rate) = [values[0] + 2*values[1]/(1+rate) + 3*values[2]/(1+rate)^2 +...+ n*values[n-1]/(1+rate)^(n-1)]*(-1)/(1+rate)^2
double _NPV_Df(enum MathError *error, double rate, const void * params[])
{
	double result = 0;
	double powRate = 1;
    
	Int32 nSize = *((Int32 *)(params[0]));
	double *values = (double *)params[1];
    
	Int32 i = 0;
    
	for (i = 0; i < nSize; ++i)
	{
		result += ( (i+1.0) * values[i] ) / powRate; 
		powRate *= (1 + rate);
	}
	
	result *= -1.0 / ( (1+rate) * (1+rate) );
    
	CheckValue(error, result);
    
	return result;
}

double _XNPV_Df(enum MathError *error, double rate, const void * params[])
{
	Int32 n = *((Int32 *)(params[0]));
	double *values = (double *)params[1];
	Date *dates = (Date *)params[2];
	Int32 *valid = (Int32 *)params[3];
    
	Int32 i = 0;
	Int32 min = 0;
	for (i = 0; i < n; i ++) {
		if (valid[i] != 2) {
			continue;
		}
		if (!isLegalDate(dates[i])){
			SetMathError(error, MathDomain);
			return 0;
		}
		if (dates[min] > dates[i]) {
			min = i;
		}
	}
    
	double result = 0.0;
    
	for (i = 0; i < n; i ++)
	{
		if (valid[i] != 2) {
			continue;
		}
		double diff = difference(dates[i], dates[min], eBasisActualActual) / (double)365;
		result += - diff * values[i] / pow(1 + rate, 1 + diff);
	}
	CheckValue(error, result);
    
	return result;
}

double _XIRR_NewtonMethod(enum MathError *errorFlag, double(*func)(enum MathError *, double, const void * []),
                          double guess, const void * params[], Int32 nLimits, double err )
{
	Int32 i = 0;
	double x = guess;
//	double df = 0.0;
	
	double fx = func(errorFlag, x, params);
	double dfx = _XNPV_Df(errorFlag, x, params);
	while ( i < nLimits && fabs(fx) > err)
	{
		if (dfx == 0.0)
			break;
		x = x - fx/dfx;
		fx = func(errorFlag, x, params);
		dfx = _XNPV_Df(errorFlag, x, params);
		i ++;
	}
    
	if (fabs(func(errorFlag, x, params)) > err)
		SetMathError(errorFlag, MathConvergence);
    
	return x;	
}
