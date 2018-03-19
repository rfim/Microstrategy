/*
 *  DSSSumFunction.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 2/4/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef SumFunction_h
#define SumFunction_h 1

#import "DSSAggregationFunction.h"

class DSSSumFunction : public DSSAggregationFunction
{
public:
	DSSSumFunction();
	
	virtual ~DSSSumFunction();

	virtual int Calculate(int nSize, double *pVec, double *opResult, DSSDataFlag *pFlag);
	
	// qjiang 04/16/2009 enhance DirectCalculate to suppord different numeric types
	virtual int DirectCalculate(int iStart,
								int * ipGroupLinkList,
								int * ipBlockAddress,
								void* ipData, double iDivider,
								EnumDSSDataType iDataType,
								DSSDataFlag* ipDataFlags,
								bool iAllDataValid,
								EnumNullCheckingOption iNullCheckingOption,
								double* opResult,
								DSSDataFlag* opDataFlag);
	
	virtual bool SupportDirectCalculate();
    //tliao, 822148, 2014/09/23, support cell format data calculation
    virtual bool SupportCellFormat() {return true;}
    virtual int CalculateCellData(Int32 nParam, CComVariant * pParam, CComVariant * pResult, DSSDataFlag * pFlag);
	
private:

    bool mDistinct;// = false;
    
	double AvgImpl(int nSize, double *pData);
    
    void CalculateCellTypeAndFormat(Int32 nParam, EnumCellFormatDataTypes *pTypes, DSS_ID *pFormatIDs, EnumCellFormatDataTypes *oResultType, DSS_ID *oResultFormatID);
	
	// qjiang 04/16/2009
	int hGroupAggImpl(int iStart,
						  int * ipGroupLinkList,
						  int * ipBlockAddress,
						  void* ipData,
						  EnumDSSDataType iDataType,
						  DSSDataFlag* ipDataFlags,
						  bool iAllDataValid,
						  EnumNullCheckingOption iNullCheckingOption,
						  double* opResult,
						  DSSDataFlag* opDataFlag);
	
	
	// qjiang 08/14/2009
	// direct calculate for average
	int hGroupAvgImpl(int iStart,
						  int * ipGroupLinkList,
						  int * ipBlockAddress,
						  void* ipData,
						  EnumDSSDataType iDataType,
						  DSSDataFlag* ipDataFlags,
						  bool iAllDataValid,
						  EnumNullCheckingOption iNullCheckingOption,
						  double* opResult,
						  DSSDataFlag* opDataFlag);
	
	int hGroupMedianImpl(int iStart,
							 int * ipGroupLinkList,
							 int * ipBlockAddress,
							 void* ipData,
							 EnumDSSDataType iDataType,
							 DSSDataFlag* ipDataFlags,
							 bool iAllDataValid,
							 EnumNullCheckingOption iNullCheckingOption,
							 double* opResult,
							 DSSDataFlag* opDataFlag);
	
	int hGroupProductImpl(int iStart,
							  int * ipGroupLinkList,
							  int * ipBlockAddress,
							  void* ipData, double iDivider,
							  EnumDSSDataType iDataType,
							  DSSDataFlag* ipDataFlags,
							  bool iAllDataValid,
							  EnumNullCheckingOption iNullCheckingOption,
							  double* opResult,
							  DSSDataFlag* opDataFlag);
	
	template <class InputDataType, class TempType>
	int hAvgImpl(int iStart, int * ipGroupLinkList, int *ipBlockAddress,  InputDataType* ipData, DSSDataFlag* ipDataFlags, bool iAllDataOK, 
					 EnumNullCheckingOption iNullCheckingOption, double* opResult, DSSDataFlag* opDataFlag);
	
	template <class InputDataType>
	int hMedianImpl(int iStart, int * ipGroupLinkList, int *ipBlockAddress,  InputDataType* ipData, DSSDataFlag* ipDataFlags, bool iAllDataOK, 
						EnumNullCheckingOption iNullCheckingOption, double* opResult, DSSDataFlag* opDataFlag);
	
	template <class InputDataType>
	int hProductImpl(int iStart, int * ipGroupLinkList, int *ipBlockAddress,  InputDataType* ipData, double iDivider, DSSDataFlag* ipDataFlags, bool iAllDataOK, 
						 EnumNullCheckingOption iNullCheckingOption, double* opResult, DSSDataFlag* opDataFlag);
	// qjiang 04/16/2009
	// change to use function template to support different numeric types
	// 06/15/2009
	// add function template for temporary storage of the aggregating result, TempType can only be double or Int64
	template <class InputDataType, class TempType>
	int hSumImpl(int iStart, int * ipGroupLinkList, int *ipBlockAddress,  InputDataType* ipData, DSSDataFlag* ipDataFlags, bool iAllDataOK, 
					 EnumNullCheckingOption iNullCheckingOption, double* opResult, DSSDataFlag* opDataFlag);
	
    int setProperty(Int32 Index, CComVariant *pValue);
};


#endif
