//
//  mathPackImplement.h
//  ReportServiceCore
//
//  Created by He Tengfei on 2/22/12.
//  Copyright 2012 MicroStrategy, Inc.. All rights reserved.
//

#ifndef mathPackImplement_h
#define mathPackImplement_h 1

#include "DSSFunctionUtility.h"

#define PI 3.1415926535897932384626433


int MSI_ABS(enum MathError *error, double *pResult, double param0);
int MSI_ACOS(enum MathError *error, double *pResult, double param0);
int MSI_ACOSH(enum MathError *error, double *pResult, double param0);
int MSI_ASIN(enum MathError *error, double *pResult, double param0);
int MSI_ASINH(enum MathError *error, double *pResult, double param0);
int MSI_ATAN(enum MathError *error, double *pResult, double param0);
int MSI_ATAN2(enum MathError *error, double *pResult, double param0, double param1);
int MSI_ATANH(enum MathError *error, double *pResult, double param0);
int MSI_CEILING(enum MathError *error, double *pResult, double param0);
int MSI_COMBIN(enum MathError *error, double *pResult, double param0, double param1);
int MSI_COS(enum MathError *error, double *pResult, double param0);
int MSI_COSH(enum MathError *error, double *pResult, double param0);
int MSI_DEGREES(enum MathError *error, double *pResult, double param0);
int MSI_EXP(enum MathError *error, double *pResult, double param0);
int MSI_FACTORIAL(enum MathError *error, double *pResult, double param0);
int MSI_FLOOR(enum MathError *error, double *pResult, double param0);
int MSI_INT(enum MathError *error, double *pResult, double param0);
int MSI_LN(enum MathError *error, double *pResult, double param0);
int MSI_LOG(enum MathError *error, double *pResult, double param0, double param1);
int MSI_LOG10(enum MathError *error, double *pResult, double param0);
int MSI_GETPI(enum MathError *error, double *pResult);
int MSI_POWER(enum MathError *error, double *pResult, double param0, double param1);
int MSI_QUOTIENT(enum MathError *error, double *pResult, double param0, double param1);
int MSI_RADIANS(enum MathError *error, double *pResult, double param0);
int MSI_RAND(enum MathError *error, double *pResult);
int MSI_RANDBETWEEN(enum MathError *error, double *pResult, double param0, double param1);
int MSI_ROUND(enum MathError *error, double *pResult, double param0, double param1);
int MSI_SIN(enum MathError *error, double *pResult, double param0);
int MSI_SINH(enum MathError *error, double *pResult, double param0);
int MSI_SQRT(enum MathError *error, double *pResult, double param0);
int MSI_TAN(enum MathError *error, double *pResult, double param0);
int MSI_TANH(enum MathError *error, double *pResult, double param0);
int MSI_TRUNC(enum MathError *error, double *pResult, double param0, double param1);
int MSI_MOD(enum MathError *error, double *pResult, double param0, double param1);

#endif