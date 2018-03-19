/*
 *  AggregateGeneral.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 2/4/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef AggregateGeneralFunc_h
#define AggregateGeneralFunc_h 1

#import "DSSNNAggregationFunction.h"
#import "VariantCommonInclude.h"
/*
#import "PDCHeader/PDCwinnt.h"
#import "MsiCOM/MsiCOM/Msi_atldef.h"
#import "MsiCOM/MsiCOM/Msi_objbase.h"
#import "MsiCOM/MsiCOM/Msi_oleauto.h"
#import "MsiCOM/MsiCOM/Msi_ccomvariant.h"
*/
class DSSAggregateGeneralFunc : public DSSNNAggregationFunction
{
public:
	DSSAggregateGeneralFunc();
	
	virtual ~DSSAggregateGeneralFunc();

	// ICDSSAggregationFunction methods
	int Calculate(int nSize, double * pVec, double * pResult, DSSDataFlag * pFlag );
	int GetOutputType(EnumDSSDataType InputType, EnumDSSDataType* pOutputType);
	
	// ICDSSVariantAggFunc methods
	int Calculate(int nSize, CComVariant * pVec, EnumDSSDataType  iType, CComVariant * pResult, DSSDataFlag  * pFlag);
	
	bool operator ()(int Index1, int Index2);
	
	// qjiang 08/14/2009
	// Add direct calculate for certain aggregation functions
	bool SupportDirectCalculate();
	
	int DirectCalculate(int iStart,
											  int * ipGroupLinkList,
											  int * ipBlockAddress,
											  void* ipData, double iDivider,
											  EnumDSSDataType  iType,
											  DSSDataFlag * ipDataFlags,
											  bool iAllDataValid,
											  EnumNullCheckingOption iNullCheckingOption,
											  double* opResult,
											  DSSDataFlag * opDataFlag);
    
    int getOutputType (EnumDSSDataType InputType, EnumDSSDataType * OutputType );
    
    //zhyang, 28/07/2011, add this two function to set the property and clean the internal data
    int Clear();
    virtual int setProperty(int Index, CComVariant *ipVal);
    bool GetDistinctProperty();
    void SetInputDistinct(bool iVal = true);
    int SetWorkingBuffer(/*MBase::Buffer* ipBuffer, */Int32 iMaxKey);
    //tliao, 822148, 2014/09/23, support cell format data calculation
    virtual bool SupportCellFormat() {return true;}
    virtual int CalculateCellData(Int32 nParam, CComVariant * pParam, CComVariant * pResult, DSSDataFlag * pFlag);
	
private:
	
	int SetInputParam (int nSize, EnumDSSDataType  dataType, CComVariant *pData);

	int CalculateMaxMin();
	
	int CalculateCount();
	
	int CalculateMode();
	
	int CalculateMode(int nSize, double * pVec, double * pResult, DSSDataFlag  * pFlag); // Numerical version
	
	int Order(int row1, int row2, int& result);
	
	void CalculateCellTypeAndFormat(int nParam, EnumCellFormatDataTypes *pTypes, DSS_ID *pFormatIDs, EnumCellFormatDataTypes *oType, DSS_ID *oFormatID);
	
	// qjiang 08/14/2009
	// direct calculate implementations
	template<class Type>
	int hDirectCalculate(int iSize, int * ipGroupLinkList, int *ipBlockAddress,  Type* ipData, double iDivider, DSSDataFlag * ipDataFlags, bool iAllDataOK, 
							 EnumNullCheckingOption iNullCheckingOption, double* opResult, DSSDataFlag * opDataFlag, bool ibInteger);
	
	template<class Type>
	int hDirectCalculateMaxMin(int iSize, int * ipGroupLinkList, int *ipBlockAddress,  Type* ipData, DSSDataFlag * ipDataFlags, bool iAllDataOK, 
								   EnumNullCheckingOption iNullCheckingOption, double* opResult, DSSDataFlag * opDataFlag);
	
	template<class Type>
	int hDirectCalculateCount(int iSize, int * ipGroupLinkList, int *ipBlockAddress,  Type* ipData, DSSDataFlag * ipDataFlags, bool iAllDataOK, 
								  EnumNullCheckingOption iNullCheckingOption, double* opResult, DSSDataFlag * opDataFlag, bool ibInteger);
	
private:	
    int mDataSize; // Input vector length
    
    // DSSVariant data support
    CComVariant mInputData;  // Input variant safe array
    CComVariant mResultData;  // Result variant
    EnumDSSDataType mInputDataType;
    DSSDataFlag  mResultFlag;
    bool mDistinct;		// support for count distinct
    bool mRemovable;	//support for mode removable, TQMS361481
    
    bool mInputDistinct;    // Input is distinct
    
    bool* mpLookup;
    int* mpStack;
};



#endif
