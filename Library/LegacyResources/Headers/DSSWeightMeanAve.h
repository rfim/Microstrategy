//
//	DSSWeightMeanAve.h
//  ReportServiceCore
//
//  Create by xzhu on 07/01/2014
//  Copyright (c) 2014 MicroStratey, Inc. All rights reserved.

#ifndef DSSWeightMeanAve_h
#define DSSWeightMeanAve_h 1

// CComObjectRootEx_M
#import "DSSGenericFunction.h"

class DSSWeightMeanAveFunction:public DSSGenericFunction
						
{
public:
	DSSWeightMeanAveFunction();

	~DSSWeightMeanAveFunction();

    int get_FunctionType (EnumDSSFunctionType *  pType);
    int setProperty (Int32 Index, CComVariant *pValue);

	int Calculate ();
    int SetInputParam (Int32 nParamIndex, Int32 nSize, double *pData, DSSData_Flags *pFlag);

    int GetOutput (Int32 nSize, 	// Input buffer size
                   Int32 *nDataSize, 	// return data size
                   double *pData, 	// Double array stores the result of function
                   DSSData_Flags *pFlag	// Output flags
                   );

    int GetOutputType (EnumDSSParameterType *pType);
    int GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType);
    int IsNullProcessable (bool *pCanProcessNULL);
    int Clear ();

	int SetUseAsWeightStdevP(bool iUseAsWeightStdevP);

private:
	double  *mpValues;
	double	*mpWeights;
	DSSData_Flags* mpFlags;

	double  mResult;
	DSSData_Flags mResultFlag;

	Int32	mDataSize;

	bool	mbUseAsWeightStdevP;
};


#endif