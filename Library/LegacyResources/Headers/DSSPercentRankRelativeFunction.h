//
//	DSSPercentRankRelative.cpp
//  ReportServiceCore
//
//  Create by xzhu on 06/20/2014
//  Copyright (c) 2014 MicroStratey, Inc. All rights reserved.
//


#ifndef DSSPercentRankFunction_h
#define DSSPercentRankFunction_h 1

#import "DSSGenericFunction.h"
#include "math.h"

typedef std::pair<double, bool> INVALUE_PAIR;

class DSSPercentRankRelativeFunction : public DSSGenericFunction
{
public:

	  DSSPercentRankRelativeFunction();

	  ~DSSPercentRankRelativeFunction();

	  //## Operation: get_FunctionType%3766871C03BF
      int get_FunctionType (EnumDSSFunctionType *  pType);

      int setProperty (Int32 Index, CComVariant *pValue);

      //## Operation: Calculate%379F29D20189
      int Calculate();
	
	  int GetOutput(Int32 nSize, Int32 *nDataSize,  double * pData, DSSData_Flags * pFlag);

	  int GetOutputType(EnumDSSParameterType * pType);

	  int SetInputParam(Int32 nParamIndex, Int32 nSize, double * pData, DSSData_Flags * pFlag);

	  //## Operation: GetInputParamType%379F29D2020B
      int GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType);

	  //## Operation: Clear%379F29D201D9
      int Clear ();

	  int IsNullProcessable(bool *pCanProcessNULL);
	 
	  int SetAsSQLVersion(bool ibSQLVersion);


private:
	
	Int32			mDataSize;			//number of input rows
	double			*mpdistValues;
	INVALUE_PAIR	*mpinValues;

	double			mdblSigFactor;		// 10^sig, where 1 <= sig <= 9
	bool			mbTruncate;			// true=truncate result; false=round result
	bool			mbRankOutliers;		// true=outliers return 0 or 1; false=outliers return empty cell
	bool			mbInclusive;			// true=inclusive; false=exclusive

	Int32			mdistValueCnt;		// number of distribution values

	double  *mpResults;
	DSSData_Flags *mpFlags;

	bool			mIsOneInput;		//Excel version PercentRank has two inputs, and SQL version has only one input, default is false

	double Round(double dblValue)
	{
		if(mbTruncate)
			return floor(mdblSigFactor * dblValue) / mdblSigFactor;
		else
			return floor(floor(10 * mdblSigFactor * dblValue + 5) / 10) / mdblSigFactor;
	}
    
	bool IsOutlier(double dblValue)
	{
		return (dblValue < mpdistValues[0]) || (dblValue > mpdistValues[mdistValueCnt-1]);
	}

	int CalcPercentRank(double *pValues, DSSData_Flags *pFlag);
	Int32 hPartition(double *pArray, Int32 iLeft, Int32 iRight, Int32 iPivot);
	int hQuickSort(double *pArray, Int32 iLeft, Int32 iRight);
	int hQuickSort2(double *pArray, Int32 iLeft, Int32 iRight);
	Int32 hBinarySearch(double *pArray, double dblKey, Int32 iLeft, Int32 iRight);
};

#endif