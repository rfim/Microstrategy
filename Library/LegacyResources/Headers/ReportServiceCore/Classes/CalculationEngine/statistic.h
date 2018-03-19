//
//  statistic.h
//  ReportServiceCore
//
//  Created by He Tengfei on 3/8/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//
#ifndef _STATISTIC_
#define _STATISTIC_

#include "DSSFunctionUtility.h"

#define PI 3.1415926535897932384626433
#define	I_PI 0.3183098861837906715377675 // 1/pi
#define	LOG_SQRT_PI     0.5723649429247000870717135 /* log (sqrt (pi)) */
#define	I_SQRT_PI       0.5641895835477562869480795 /* 1 / sqrt (pi) */
#define I_SQRT_TWO_PI   0.39894228040143  /* 1/sqrt(2*pi) */



#ifndef max
inline double max(double x, double y)
{
	return (x > y)? x:y;
}
#endif

#ifndef min
inline double min(double x, double y)
{
	return (x < y)? x:y;
}
#endif

enum Hypothesis
{
	HypUnkown = -100,
	HypLess = -1,
	HypEqual = 0,
	HypGreater = 1,
	HypOneSide = 2
};


// UMV estimation of the mean of a population
double Mean(enum MathError *error, Int32 n, double Vn[]);

// UMV estimation of the variance of a population Vn with an unknown mean
double Variance(enum MathError *error, Int32 n, double Vn[]);

// UMV estimation of standard deviation of a population Vn with an unknown mean
double StDev(enum MathError *error, Int32 n, double Vn[]);

// UMV estimation of the variance of a population Vn with a known mean 
double VarianceKM(enum MathError *error, Int32 n, double Vn[], double mean);

// UMV estimation of the standard deviation of a population Vn with a known mean 
double StDevKM(enum MathError *error, Int32 n, double Vn[], double mean);

// Average absolute deviation from mean
double AvgDev(enum MathError *error, Int32 n, double Vn[]);

// UMV estimation of the kurtosis of the population Vn
double Kurtosis(enum MathError *error, Int32 n, double Vn[]);

// UMV estimation of the skew of the population
double Skew(enum MathError *error, Int32 n, double Vn[]);

// UMV estimation of the covariance of two populations
double Covariance( enum MathError *error, Int32 nX, double X[], Int32 nY, double Y[] );

// UMV estimation of the correlation of two populations
double Correlation( enum MathError *error, Int32 nX, double X[], Int32 nY, double Y[] );

// UMV estimation of the R sqare coefficient of two populations
double RSquare( enum MathError *error, Int32 nX, double X[], Int32 nY, double Y[] );

// UMV estimation of the covariance of two populations with known means
double CovarianceKM( enum MathError *error, Int32 nX, double X[], double meanX, Int32 nY, double Y[], double meanY );

// UMV estimation of the correlation of two populations with known means
double CorrelationKM( enum MathError *error, Int32 nX, double X[], double meanX, Int32 nY, double Y[], double meanY );

// UMV estimation of the R sqare coefficient of two populations with known means
double RSquareKM( enum MathError *error, Int32 nX, double X[], double meanX, Int32 nY, double Y[], double meanY );

// Tests the hypotesis Hyp on the mean of a population X with a known standard deviation stdevX against the value hypMean
double ZTest( enum MathError *error, Int32 nX, double X[], double stdevX, double hypMean, enum Hypothesis Hyp );

// Tests the hypotesis Hyp on the mean of a population X with unknown standard deviation against the value hypMean
double MeanTTest( enum MathError *error, Int32 nX, double X[], double hypMean, enum Hypothesis Hyp );

// Tests the hypotesis Hyp on the mean of two populations with paired samples
// The test is performed against a mean difference of offset
double PairedTTest( enum MathError *error, Int32 nX, double X[], Int32 nY, double Y[], enum Hypothesis Hyp, double offset=0.0 );

// Tests the hypotesis Hyp on the mean of two populations with the same unknown variance
// The test is performed against a mean difference of offset
double HomoscedasticTTest( enum MathError *error, Int32 nX, double X[], Int32 nY, double Y[], enum Hypothesis Hyp, double offset=0.0 );

// Tests the hypotesis Hyp on the mean of two populations with different unknown variances
// The test is performed against a mean difference of offset
double HeteroscedasticTTest( enum MathError *error, Int32 nX, double X[], Int32 nY, double Y[], enum Hypothesis Hyp, double offset=0.0 );

// Tests the hypotesis Hyp on the variance of a population X against the value hypVar
double VarTest( enum MathError *error, Int32 nX, double X[], double hypVar, enum Hypothesis Hyp );

// Tests the hypotesis on the ratio of the variances of two populations
double FTest( enum MathError *error, Int32 nX, double X[], Int32 nY, double Y[], enum Hypothesis Hyp, double ratio=1.0);

// tests the hypotesis that X follows the distribution Y
double ChiSqTest( enum MathError *error, Int32 nX, double X[], Int32 nY, double Y[] );

// slope of the regression line
double Slope(enum MathError *error, Int32 nY, double Y[], Int32 nX, double X[]);

// intercept of the regression line
double Intercept(enum MathError *error, Int32 nY, double Y[], Int32 nX, double X[]);

double SteYX(enum MathError *error, Int32 nY, double Y[], Int32 nX, double X[]);



double ALOGAM(enum MathError *error, double x);
double BETAIN(enum MathError *error, double x, double a, double b, double beta); 
double BETADIST(enum MathError *error, double x, double alpha, double beta, double LB=0.0, double UB=1.0);
double BETAINVERSE(enum MathError *error, double p, double alpha, double beta, double LB=0.0, double UB=1.0);
double BETAINV(enum MathError *error, double p, double q, double alpha);
double BINOMDIST(enum MathError *error, Int32 x, Int32 n, double p, bool fCum=true);
double CRITBINOM(enum MathError *error, Int32 n, double p, double f);
double FDIST(enum MathError *error, double p, Int32 df1, Int32 df2);
double FINV(enum MathError *error, double f, Int32 df1, Int32 df2);
double COMBIN(enum MathError *error, Int32 x, Int32 n); 
double CHIDIST(enum MathError *error, double x, Int32 df);
double CHIINV(enum MathError *error, double p, Int32 df);
double NORMSDIST(enum MathError *error, double z);	//accurate up to six digits
double NORMSINV(enum MathError *error, double f);
double NORMDIST(enum MathError *error, double x, double mean, double standard_dev, bool fCum=true);
double NORMINV(enum MathError *error, double p, double mean, double standard_dev);
double EXPONDIST(enum MathError *error, double x, double lambda, bool fCum=true);
double GAMMADIST(enum MathError *error, double x, double alpha, double beta, bool fCum=true);
double GAMMAINV(enum MathError *error, double p, double alpha, double beta);
double HYPGEOMDIST(enum MathError *error, Int32 x, Int32 n, Int32 M, Int32 N);
double LOGNORMDIST(enum MathError *error, double x, double mean, double standard_dev);
double LOGINV(enum MathError *error, double p, double mean, double standard_dev);
double NEGBINOMDIST(enum MathError *error, Int32 f, Int32 s, double p);
double PERMUT(enum MathError *error, Int32 x, Int32 n);
double POISSON(enum MathError *error, Int32 x, double mean, bool fCum=true);
double TTAILINV(enum MathError *error, double p, Int32 nu);
double TINV(enum MathError *error, double p, Int32 nu);
double TTAIL(enum MathError *error, double x, Int32 nu, Int32 tails);
double TDIST(enum MathError *error, double x, Int32 nu);
double WEIBULL(enum MathError *error, double x, double alpha, double beta, bool fCum=true);
double Standardize(enum MathError *error, double x, double mean, double standard_dev);
double Confidence(enum MathError *error, double alpha, double standard_dev, Int32 size);
double Fisher(enum MathError *error, double x);
double FisherInv(enum MathError *error, double x);

double oldTDIST(enum MathError *error, double x, Int32 nu);
double oldFDIST(enum MathError *error, double p, Int32 df1, Int32 df2);
double oldCHIDIST(enum MathError *error, double x, Int32 df);

double gammln(enum MathError *error, double xx);

// *** New statistic functions ***
// Helper functions
void LinearRegression (enum MathError * error, double py[], int n, double* m, double* b);
void LinearRegression (enum MathError * error, double px[], double py[], int n, double* m, double* b);

double LinearTrend_S(enum MathError * error, double x, double py[], int n);
double Growth_S(enum MathError * error, double x, double py[], int n);
double Forecast_S(enum MathError * error, double x, double px[], double py[], int n);

enum RegressionFunction
{
	IdFunction = 0,
	LogFunction = 1,
	InverseFunction = 2
};


class Regression
{
public:
    
	double (*Function)(double);
	double (*InverseFunction)(double);
	Int32 ParametersNum, ObservationsNum;
	double *Parameters;
	double *Variances;
	double *Pvalues;
	double Rsquare, Ssquare;
    
	Regression(enum MathError *error, Int32 varNum, Int32 obsNum, double Y[], double **X, Int32 function=0);
	~Regression();
    
	double Predict(enum MathError *error, Int32 nX, double X[]);
};

#endif // _STATISTIC_