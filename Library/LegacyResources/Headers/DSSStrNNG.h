//
//	DSSStrNNG.h
//  ReportServiceCore
//
//  Create by xzhu on 08/12/2014
//  Copyright (c) 2014 MicroStratey, Inc. All rights reserved.

#ifndef DSSStrNNG_h
#define DSSStrNNG_h 1

#import "DSSNNGenericFunction.h"
#import "DSSNNAggregationFunction.h"

class DSSStrNNG : public DSSNNGenericFunction

{
  public:
      DSSStrNNG();

      ~DSSStrNNG();

      int get_FunctionType (EnumDSSFunctionType *  pType);

      int setProperty (int Index, CComVariant *pValue);

      int Calculate ();

      int SetInputParam (Int32 nParamIndex, Int32 nSize, EnumDSSDataType dataType, CComVariant *pData, DSSDataFlag *pFlag);

      int GetOutput (Int32 nSize, 	// Input buffer size
							  Int32 *nDataSize, 	// return data size
							  CComVariant *pData, 	// Double array stores the result of function
							  DSSDataFlag *pFlag	// Output flags
							  );

      int GetOutputType (Int32 nSize, EnumDSSDataType *pInputType, EnumDSSDataType *pOutputDataType, EnumDSSParameterType *pOutParamType);

      int GetInputParamType (Int32 nParamIndex, EnumDSSParameterType *pType);

      int IsNullProcessable (bool *pCanProcessNULL);

      int Clear ();

      int Init (EnumSimpleOperatorType iFunctionIndex);

	  int get_OperatorType(EnumSimpleOperatorType * opType);
    
	  int getOutputType(EnumDSSDataType InputType, EnumDSSDataType* pOutputType);

	  int Calculate(int nSize, CComVariant * pVec, EnumDSSDataType iType, CComVariant * pResult, DSSDataFlag * pFlag);
    
      //inherits from DSSGenericFunction
      int GetOutput(Int32, Int32 *, double *, DSSDataFlag *);
      int SetInputParam(Int32, Int32, double *, DSSDataFlag *);
      int GetOutputType(EnumDSSParameterType *);

      virtual bool IsNNFunctionOnly() {return true;}
 
  private:
  
	   Int32 mDataSize; // Input vector length
	   CComVariant mInputData;  // Input variant safe array
	   EnumDSSDataType mInputDataType;
	   CComVariant mResultData;
	   DSSData_Flags mResultFlag;
	   MBase::StrongArrayPtr<DSSData_Flags> mpDataFlagPtr;   //input flags
	   bool mInputFlag;
       std::string mDelimiter;
	   int CalculateConcat();
};



#endif
