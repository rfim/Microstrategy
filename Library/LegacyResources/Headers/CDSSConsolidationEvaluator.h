//
//  CDSSConsolidationEvaluator.h
//  ReportServiceCore
//
//  Created by Liao Taimin on 4/17/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_CDSSConsolidationEvaluator_h
#define ReportServiceCore_CDSSConsolidationEvaluator_h

#include "DSSNNSimpleFunction.h"
class DSSCExpression;
class DSSTabularConsolidation;
class DSSFunctionObject;
class DSSExpNode;
class DSSFunction;

class CDSSConsolidationEvaluator : public DSSNNSimpleFunction
{

public:
    //## Constructors (specified)
    //## Operation: CDSSConsolidationEvaluator%908080187
    CDSSConsolidationEvaluator();
    
    //## Destructor (generated)
    ~CDSSConsolidationEvaluator();
    
    
    //## Other Operations (specified)
    //## Operation: Calculate%908080188
    //	Derived class must implement this method.
    virtual int Calculate(int nParam, double *pParam, double *pResult, DSSDataFlag *pFlag);
    
    //## Operation: CalculateRepeat%908080189
    int CalculateRepeat(Int32 nParam, Int32 nRepeat, double **pParam, double *pResult, DSSData_Flags *pFlag);
    
    //## Operation: SetProperty%908080191
    //	The derived class should implemented this method.
    int SetProperty(Int32 Index, VARIANT *pValue);
    
    //## Operation: init%908080194
    //	Derived class must implement this method.
    int init(DSSCExpression *pExpression, DSSFunctionServer *pFunctionServer);
    int put_DynamicAggFunc(Int32 iFuncIndex);
    
    int put_NonAggFunc(Int32 iFuncIndex);
    
    //lwang, 7/21/2007, get evaluator type
    int get_EvaluatorType(EnumEvaluatorType *pEvalType);
    
    //lwang, 10/09/2007, get function type and output data type
    int	get_FunctionDataType(EnumDSSDataType inputDataType,
                             EnumDSSDataType *opOutputDataType,
                             VARIANT_BOOL *opIsNonNumerical);
    
    // implementation of ICDSSNNSimpleFunction
    virtual int Calculate(int nSize, CComVariant *pVec, CComVariant *opResult, DSSDataFlag *pFlag);
    
    int CalculateRepeat(int nParam, int nRepeat, CComVariant *pParam, CComVariant *pResult, DSSDataFlag *pFlag);
    
    virtual int GetOutputType(int size, EnumDSSDataType *inputType, EnumDSSDataType *outputType);
    
    int get_ParamCount(Int32 *opCount);
    
    int GenerateParamRefList(DSSTabularConsolidation* ipConsolidation,
                             DSSTabularData * ipTabularData,
                                                   Int32 *ipLeaves,
                                                   Int32 nLeaves,
                                                   Int32 *opParamRefList);
	
    // jxie, TQMS 403115, 2010-5-14
    // set the null checking option
    int SetNullCheckingOption(EnumNullCheckingOption iNullCheckingOption);
    
    // kshi 2011-07-29 TQMS461880
    int SetStringCompareOption(VARIANT_BOOL ibTrimString);    
    
private:
    
    //## Other Operations (specified)
    //## Operation: Calculate_0%908080192
    int Calculate_0(DSSExpNode *pNode, Int32 nParam, double *pParam, double *pResult, DSSData_Flags *pFlag, Int32 *pParamUsed);
    
    // Data Members for Class Attributes
    
    //## Attribute: mpExpression
    //## begin CDSSConsolidationEvaluator::mpExpression.attr preserve=no  private: ICDSSExpression * {UA}
    DSSCExpression *mpExpression;
    //## end CDSSConsolidationEvaluator::mpExpression.attr
    
    //## Attribute: mnParam
    //## begin CDSSConsolidationEvaluator::mnParam.attr preserve=no  private: long {UA}
    Int32 mnParam;
    //## end CDSSConsolidationEvaluator::mnParam.attr
    
    //## Attribute: mpFunctionServer
    //## begin CDSSConsolidationEvaluator::mpFunctionServer.attr preserve=no  private: ICDSSFunctionServer * {UA}
    DSSFunctionServer *mpFunctionServer;
    //## end CDSSConsolidationEvaluator::mpFunctionServer.attr
    
    // Additional Private Declarations
    //## begin CDSSConsolidationEvaluator.private preserve=yes
    EnumEvaluatorType mType;
    Int32 mDynamicFuncIndex; // for dynamic agg function
    
    Int32 mNonAggFuncIndex; // for non-agg metric
    
    EnumDSSDataType mAggFuncInputDataType;	// for non-numerical aggregation function
    
    bool mbHasCellFmt;
    
    // jxie, TQMS 403115, 2010-5-14
    // keep track of the null checking option
    EnumNullCheckingOption mNullCheckingOption;
    
    // kshi 2011-07-29 TQMS461880
    bool mfTrimString;
    //## end CDSSConsolidationEvaluator.private
    
    
private:  //## implementation
    // Additional Implementation Declarations
    //## begin CDSSConsolidationEvaluator.implementation preserve=yes
    int CalculateGroup(Int32 nParam, double *pParam, double *pResult, DSSData_Flags *pFlag);
    
    int CalculateDynamicAgg(Int32 nParam, double *pParam, double *pResult, DSSData_Flags *pFlag);
    
    //lwang, 10/09/2007, add support of non-numerical calculation
    int hCalculateNN_0(DSSExpNode *ipNode, Int32 nParam, VARIANT *ipParam, LONG index,
                                             VARIANT *opResult, DSSData_Flags *opFlag, Int32 *pParamUsed);
    
    int hCalculateNNGroup(Int32 nParam, VARIANT *ipParam,
                                                VARIANT *opResult, DSSData_Flags *opFlag);
    
    int hCalculateNNDynamicAgg(Int32 nParam, VARIANT *ipParam,
                                                     VARIANT *opResult, DSSData_Flags *opFlag);
    
    int hFindFunctionObject(DSSFunction *ipFunction, EnumDSSFunctionType iType,
                                                  DSSFunctionObject **oppFunctionObject);
    
    int hGetCalculationDataType(DSSExpNode *ipNode, EnumDSSDataType iutputDataType,
                                                      EnumDSSDataType *opOutputDataType,VARIANT_BOOL *opIsNNFunction);
    
    int hGetGroupDataType(DSSExpNode *ipNode, EnumDSSDataType iutputDataType,
                                                EnumDSSDataType *opOutputDataType, VARIANT_BOOL *opIsNNFunction);
    
    int hGetDynamicAggDataType(EnumDSSDataType iutputDataType,
                                                     EnumDSSDataType *opOutputDataType, VARIANT_BOOL *opIsNNFunction);
    
    int hGetAggregationFuncDataType(DSSFunctionObject *ipFunctionObject, EnumDSSDataType iutputDataType,
                                                          EnumDSSDataType *opOutputDataType, VARIANT_BOOL *opIsNNFunction);
    
    //lwang,333292; support for cell format data
    //HRESULT hCheckCellFormatData(CComVariant *ipData, ICDSSCellFormatFunction *ipCellFmtFunction);
    
//    HRESULT hGetNumerialVal(VARIANT *ipValue, Int32 nParam, double *opParam);
    
//    HRESULT CreateCellData(double pValue, VARIANT* oResult);
    
    int hApplyNNAggregation(DSSFunctionObject *ipFunc, Int32 nParam, VARIANT *ipParam, VARIANT *opResult, DSSData_Flags *opFlag, bool ibIsDynamicAgg = false);
    
    int hApplyNonAggFunc(DSSFunctionObject *ipFunc, Int32 nParam, double *pParam, double *pResult, DSSData_Flags *pFlag);
    
    int hApplyNonAggFunc(DSSFunctionObject *ipFunc, Int32 nParam, VARIANT *ipParam, VARIANT *opResult, DSSData_Flags *opFlag);
    
    int hApplyGenericFunc(DSSFunctionObject *ipFunc, Int32 nParam, double *ipParam, double *opResult, DSSData_Flags *opFlag);
    
    int hApplyNNGenericFunc(DSSFunctionObject *ipFunc, Int32 nParam, VARIANT *ipParam, VARIANT *opResult, DSSData_Flags *opFlag);
    
    int hGetAllLeaves(DSSExpNode *ipRoot, vector<DSSExpNode*> &opLeaves);

    //## end CDSSConsolidationEvaluator.implementation
    
};

// Class CDSSConsolidationEvaluator

#endif
