//
//  CDSSConsolidationEvaluator.cpp
//  ReportServiceCore
//
//  Created by Liao Taimin on 4/17/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#include "CDSSConsolidationEvaluator.h"

#import "DFCDataConversion.h"

#import "DSSCExpression.h"
#import "DSSFunctionServer.h"
#import "DSSTabularConsolidation.h"
#import "DSSTabularData.h"
#import "DSSFunction.h"
#import "DSSExpNode.h"
#import "DSSExpNodeConstant.h"
#import "DSSExpNodeOperator.h"
#import "DSSExpNodeGroup.h"
#import "DSSAggregationFunction.h"
#import "DSSNNGenericFunction.h"
#import "DSSGlobalLookUpTable.h"
#import "DSSExpNodeElementsObject.h"
#import "DSSExpNodeShortcut.h"

CDSSConsolidationEvaluator::CDSSConsolidationEvaluator():mpExpression(NULL), mnParam(0), mpFunctionServer(NULL), mDynamicFuncIndex(DssFuncUnknown), mNonAggFuncIndex(DssFuncUnknown)
,mbHasCellFmt(false), mNullCheckingOption(DssNumericErrorSkip)
{}


CDSSConsolidationEvaluator::~CDSSConsolidationEvaluator()
{
	if(mpExpression)
    {
        delete mpExpression;
        mpExpression = NULL;
    }
    
	if(mpFunctionServer)
    {
        delete mpFunctionServer;
        mpFunctionServer = NULL;
    }
}

// jxie, TQMS 403115, 2010-5-14
int CDSSConsolidationEvaluator::SetNullCheckingOption(EnumNullCheckingOption iNullCheckingOption)
{
	mNullCheckingOption = iNullCheckingOption;
	return S_OK;
}

//## Other Operations (implementation)
int CDSSConsolidationEvaluator::Calculate(int nParam, double *pParam, double *pResult, DSSDataFlag *pFlag)
{    
    int hr = S_OK;
    
    CHECK_HR_PTR(pResult);
	CHECK_HR_PTR(pFlag);
    
	if(nParam < mnParam)
	{
		*pFlag = DssDataInvalid;
		return S_OK;
	}
    
	//now the fun started
    
	switch(mType)
	{
        case DssEvaluatorCalculation:
		{
			//I will started to pass the argument piece.
			DSSExpNode *lpNode = mpExpression -> getRoot();
			CHECK_PTR(lpNode);
            
			Int32 lParamUsed = 0;
			hr = Calculate_0(lpNode, nParam, pParam, pResult, pFlag, &lParamUsed);
			//lwang, 06/11/2008, TQMS 293002. If the child of DE is spliced, there will be remaining parameters
			if (lParamUsed < nParam)
			{
				*pFlag = DssDataInvalid;
				return S_OK;
			}
            
		}
            break;
        case DssEvaluatorGroup:
		{
			hr = CalculateGroup(nParam, pParam, pResult, pFlag);
		}
            break;
        case DssEvaluatorFilter :
        case DssEvaluatorResidue:
        case DssEvaluatorElementSingle:
        case DssEvaluatorElementList:
		{
			hr = CalculateDynamicAgg(nParam, pParam, pResult, pFlag);
		}
            break;
        default:
            break;
	}
    
	return hr;
}

int CDSSConsolidationEvaluator::CalculateRepeat(Int32 nParam, Int32 nRepeat, double **pParam, double *pResult, DSSData_Flags *pFlag)
{
    int hr = S_OK;
	CHECK_HR_PTR(pResult);
	CHECK_HR_PTR(pFlag);
    
	Int32 i = 0;
	for(i = 0; i < nRepeat; i++)
	{
		hr = Calculate(nParam, pParam[i], pResult + i, pFlag + i);
		CHECK_HR;
	}
    
	return hr = S_OK;
}

int CDSSConsolidationEvaluator::SetProperty(Int32 Index, VARIANT *pValue)
{
	return E_FAIL;		//should be internal error
}

int CDSSConsolidationEvaluator::Calculate_0(DSSExpNode *pNode, Int32 nParam,double *pParam, double *pResult, DSSData_Flags *pFlag, Int32 *pParamUsed)
{
	//internal won't check pResult, pFlag and pParamUsed    
	//the node is null
	CHECK_PTR(pNode);
    
    int hr = S_OK;
    
	EnumDSSNodeType lNodeType = pNode -> getType();
	
    
	AE_ASSERT(pResult);
	AE_ASSERT(pFlag);
	AE_ASSERT(pParamUsed);
    
	switch(lNodeType)
	{
        case DssNodeConstant:
            //we do expect constant
            //easy, conversion
            //XW: DOUBLE ONLY
		{
			DSSExpNodeConstant *lpConstant = dynamic_cast<DSSExpNodeConstant*>(pNode);
            
            CComVariant *lpVariant = lpConstant->getValue();
            hr = VariantChangeType(lpVariant, lpVariant, 0, VT_R8);
            if ( FAILED(hr) ) return hr;
            
            if(lpVariant->vt == VT_R8)
                *pResult = lpVariant->dblVal;
            else
                return E_FAIL;
            
			*pFlag = DssDataOk;
			*pParamUsed = 0;
		}
            break;
        case DssNodeElementsObject:
        case DssNodeShortcut:
            //we do exprect these two types and it may be put of
            //the Parameter passed in.
            AE_ASSERT(pParam);
            *pResult = pParam[0];
            *pFlag = DssDataOk;
            *pParamUsed = 1;
            break;
        case DssNodeOperator:
		{
			Int32 lnChild = 0,  iChild = 0 ;
			lnChild = pNode -> Count();
            
			//XW: how to deal with NULL while evaluating the true,
			//maybe property is a good solution!!!!
            double lpChildValue[lnChild];
			CHECK_NEWED_PTR(lpChildValue);
            
			DSSData_Flags lpChildValueFlag[lnChild];
			CHECK_NEWED_PTR(lpChildValueFlag);
            
			Int32 iParam = 0,  lParamUsed = 0 ;
			for(iChild = 0; iChild < lnChild; iChild ++)
			{
				DSSExpNode * lpChild = pNode -> Item(iChild);
				CHECK_PTR(lpChild);
                
				hr = Calculate_0(lpChild,
								 nParam - iParam,
								 pParam + iParam,
								 &lpChildValue[iChild],
								 &lpChildValueFlag[iChild],
								 &lParamUsed);
                
				if(lpChildValueFlag[iChild] != DssDataOk)
				{
					// jxie, TQMS 403115, 2010-5-10
					// using NullCheckingOption to skip null
					if (mNullCheckingOption == DssNumericErrorTreatAs)
					{
						lpChildValue[iChild] = 0;
						lpChildValueFlag[iChild] = DssDataOk;
					}
					else
					{
						*pResult = lpChildValue[iChild];
						*pFlag = lpChildValueFlag[iChild];
						return hr = S_OK;
					}
				}
                
				CHECK_HR;
				iParam += lParamUsed;
			}
            
			//now we need to do the calculation
			DSSExpNodeOperator *lpOperatorC = dynamic_cast<DSSExpNodeOperator*>(pNode);
			CHECK_PTR(lpOperatorC);
            
			DSSFunction *lpFunction = lpOperatorC -> getFunction();
			CHECK_PTR(lpFunction);
            
			EnumDSSFunctionType lFunctionType = lpFunction -> getAnalyticalType();
            
			if(lFunctionType != DssFunctionTypeSimple)
				return hr =  E_FAIL;
            
            GUID lPackageGUID = lpFunction->getPackageID();
            DSSFunctionObject *lpFunctionObject = mpFunctionServer->FindFunctionObject(lpFunction->getFunctionIndex(), &lPackageGUID);
			CHECK_PTR(lpFunctionObject);
            
			// dzhou 2011-07-29 TQMS461880
//			hr = CDSSAEHelper::SetTrimStringFlag(lpFunctionObject, mfTrimString);
            
			DSSSimpleFunction *lpSimpleFunction = dynamic_cast<DSSSimpleFunction*>(lpFunctionObject);
			
			if(lpSimpleFunction)
			{
                
				hr = lpSimpleFunction -> Calculate(lnChild, lpChildValue, pResult, pFlag);
				CHECK_HR;
			}
			else
			{
				//the only new case we need to handle is the
				//NNSimple where the return type is numeric
				DSSNNSimpleFunction *lpNNSimpleFunction = dynamic_cast<DSSNNSimpleFunction*>(lpFunctionObject);;
				CHECK_PTR(lpNNSimpleFunction);
                
				//now need to repackage the parameter into variants
				CComVariant input, output;
                
				//make a input parameter array[nParam]
				SAFEARRAYBOUND linputSABound;
				linputSABound.lLbound = 0;
				linputSABound.cElements = lnChild;
				input.vt = VT_ARRAY|VT_VARIANT;
				input.parray = SafeArrayCreate(VT_VARIANT, 1, &linputSABound);
                
				LONG j;;
				for (j = 0; j < lnChild; j++){
					CComVariant element;
					element.vt = VT_R8;
					element.dblVal = lpChildValue[j];
					SafeArrayPutElement(input.parray, &j, &element);
				}
                
                
				hr = lpNNSimpleFunction -> Calculate(lnChild, &input, &output, pFlag);
				CHECK_HR;
                
				if(VariantChangeType(&output, &output, 0, VT_R8) != S_OK)
				{
					*pFlag = DssDataNull;
				}
				else
				{
					*pResult = output.dblVal;
				}
                
			}
            
			//here set the result
			*pParamUsed = iParam;
		}
            break;
            
        default:
            //should not happen
            return hr =  E_FAIL;
	}

	return hr = S_OK;
}

int CDSSConsolidationEvaluator::init(DSSCExpression *pExpression, DSSFunctionServer *pFunctionServer)
{
    int hr = S_OK;
    
	AE_ASSERT(pExpression);
	AE_ASSERT(pFunctionServer);
    
	mpExpression = pExpression;
    
	mpFunctionServer = pFunctionServer;
    
	DSSExpNode * lpNode = pExpression->getRoot();
	CHECK_PTR(lpNode);
    
	EnumDSSExpressionType lpExprType = lpNode->getExpressionType();
    
	//lwang, 10/09/2007, support of non-numerical aggregation function
	mAggFuncInputDataType = DssDataTypeUnknown;
    
	switch(lpExprType)
	{
        case DssExpressionGeneric: // Group, Calculation, Other node
		{
			EnumDSSNodeType lNodeType = lpNode->getType();
            
			switch(lNodeType)
			{
                case DssNodeElementsObject: // this is old ElementsObject, equal to DssExpressionElementSingle
				{
					mType = DssEvaluatorElementSingle;
				}
                    break;
                    
                case DssNodeConstant:
				{
					mType = DssEvaluatorCalculation;
				}
                    break;
                    // lwang 03/14/2007: add the node type shortcut
                case DssNodeShortcut:
				{
					mType = DssEvaluatorCalculation;
					mnParam ++;
				}
                    break;
                case DssNodeOperator:
				{
					mType = DssEvaluatorCalculation;
					
					//need to figure out how many leaves we have
                    vector<DSSExpNode*> lpLeaves;
                    hr = hGetAllLeaves(lpNode, lpLeaves);
                    CHECK_HR;
                    
                    int lnLeaves = lpLeaves.size();
                    
					Int32 iLeaf = 0 ;
                    
					for(iLeaf = 0; iLeaf < lnLeaves; iLeaf ++)
					{
						DSSExpNode * lpLeaf = lpLeaves[iLeaf];
						CHECK_PTR(lpLeaf);
                        
						EnumDSSNodeType lNodeType = lpLeaf -> getType();
                        
						switch(lNodeType)
						{
                            case DssNodeConstant:
                                //we do expect constant
                                break;
                            case DssNodeElementsObject:
                            case DssNodeShortcut:
                                //we do exprect these two types and it may be put of
                                //the Parameter passed in.
                                mnParam ++;
                                break;
                            default:
                                //should not happen
                                return E_FAIL;
						}
					}
				}
                    break;
                    
                case DssNodeResidue:
				{
					mType = DssEvaluatorResidue;
				}
                    break;
                    
                case DssNodeGroup:
				{
					mType = DssEvaluatorGroup;
                    
					Int32 lNumChild = lpNode->Count();
                    
					Int32 iChild;
					for(iChild = 1; iChild <= lNumChild; iChild++)
					{
						DSSExpNode * lpChildNode = lpNode->Item(iChild);
						CHECK_PTR(lpChildNode);
                        
						EnumDSSNodeType lChildType = lpChildNode->getType();
                        
						switch(lChildType)
						{
                            case DssNodeShortcut:
                            case DssNodeReference:
                                mnParam++;
                                break;
                            case DssNodeElementsObject:
                            case DssNodeOperator:
                                break;
                            default:
                                //should not happen
                                return E_FAIL;
						}
					} // end of For loop
				}
                    break;
                    
                default:
                    //should not happen
                    return E_FAIL;
			}
            
		}
            break;
            
        case DssExpressionElementSingle:	// Element Objects for single derived element
		{
			mType = DssEvaluatorElementSingle;
		}
            break;
            
        case DssExpressionElementList:		// Element Objects for ordered derived element
		{
			mType = DssEvaluatorElementList;
		}
            break;
            
        case DssFilterSingleBaseFormQual:
        case DssFilterMultiBaseFormQual:
        case DssFilterJointFormQual:
        case DssFilterListQual:
        case DssFilterListFormQual:
        case DssFilterJointListQual:
        case DssFilterJointListFormQual:
        case DssFilterSingleBaseFormExpression:
        case DssFilterSingleMetricQual:
        case DssFilterMultiMetricQual:
        case DssFilterMetricExpression:
        case DssFilterEmbedQual:
        case DssFilterBranchQual:
        case DssFilterRelationshipQual:
        case DssFilterAllAttributeQual:
        case DssFilterAttributeIDQual:
        case DssFilterAttributeDESCQual:
		{
			mType = DssEvaluatorFilter;
		}
            break;
            
        default:
			//should not happen
			return E_FAIL;
	}
    
	return S_OK;
    //## end CDSSConsolidationEvaluator::init%908080194.body
}

int CDSSConsolidationEvaluator::CalculateGroup(Int32 nParam, double *pParam, double *pResult, DSSData_Flags *pFlag)
{
    int hr = S_OK;
	DSSExpNode * lpNode = mpExpression->getRoot();
	CHECK_PTR(lpNode);
    
	DSSExpNodeGroup *lpGroup = dynamic_cast<DSSExpNodeGroup*>(lpNode);
	CHECK_PTR(lpGroup);
    
	DSSFunction *lpFunction = NULL;
    /*  TODO NEED SUPPORT
	hr = lpGroup->get_Function(&lpFunction);
	CHECK_HR;
    */
    
	if(lpFunction == NULL)
	{
		hr = CalculateDynamicAgg(nParam, pParam, pResult, pFlag);
		CHECK_HR;
	}
	else
        // use the defined function
	{
        
		EnumDSSFunctionType lFunctionType;
		hr = lpFunction->getAnalyticalType();
		CHECK_HR;
        
		if((lFunctionType != DssFunctionTypeAggregation) &&
           (lFunctionType != DssFunctionTypeGeneric))
			return hr =  E_FAIL;
        
        GUID lPackageGUID = lpFunction->getPackageID();
        DSSFunctionObject *lpFunctionObject = mpFunctionServer->FindFunctionObject(lpFunction->getFunctionIndex(), &lPackageGUID);
        CHECK_PTR(lpFunctionObject);
        
		// dzhou 2011-07-29 TQMS461880
//		hr = CDSSAEHelper::SetTrimStringFlag(lpFunctionObject, mfTrimString);
        
		DSSAggregationFunction *lpAggFunction = dynamic_cast<DSSAggregationFunction*>(lpFunctionObject);
		if(lpAggFunction)
		{
			hr = lpAggFunction->Calculate(nParam, pParam, pResult, pFlag);
			CHECK_HR;
		}
		//lwang; 11/18/2008; 331949, handle First/Last function
		else if(mDynamicFuncIndex == DssFuncFirst || mDynamicFuncIndex == DssFuncLast)
		{
			return hApplyNonAggFunc(lpFunctionObject, nParam, pParam, pResult, pFlag);
		}
		else
		{
			return hApplyGenericFunc(lpFunctionObject, nParam, pParam, pResult, pFlag);
		}
	}
    
	return hr;
}

int CDSSConsolidationEvaluator::CalculateDynamicAgg(Int32 nParam, double *pParam, double *pResult, DSSData_Flags *pFlag)
{
    int hr = S_OK;
    
    if (0 == nParam)
    {
        if(mDynamicFuncIndex == DssFuncUnknown)
            *pFlag = DssDataUnCalculated;
        else
            *pFlag = DssDataInvalid;
    }
    else if (1 == nParam && DssEvaluatorElementSingle == mType)
    {
        CHECK_HR_PTR(pParam);
        *pResult = pParam[0];
        *pFlag = DssDataOk;
    }
    else
    {
        // no Dynamic Agg defined
        if(mDynamicFuncIndex == DssFuncUnknown)
        {
            // yma 12/11/08 338774
            *pFlag = DssDataUnCalculated;
            return S_OK;
        }
        
        //lwang, 01/13/2009, 343282, support for non-agg metrics
        double lNonAggResult = 0;
        DSSData_Flags lFlag = DssDataOk;
        GUID lBuildInPackageGUID = {0xACC5BFA0,0x11D3,0xDD9C,{0xC0,0x00,0x81,0xB9,0xEA,0x33,0x22,0x4F}};//GUID_DSSBuiltInPackage;
        
        if(mNonAggFuncIndex != DssFuncUnknown)
        {            
            DSSFunctionObject *lpNonAggFunctionObject = mpFunctionServer->FindFunctionObject(mNonAggFuncIndex, &lBuildInPackageGUID);
            CHECK_PTR(lpNonAggFunctionObject);
            
            //apply First/Last function for non-agg metrics
            hr = hApplyNonAggFunc(lpNonAggFunctionObject, nParam, pParam, &lNonAggResult, &lFlag);
            CHECK_HR;
            
            nParam = 1;
            pParam = &lNonAggResult;
            
            if (mDynamicFuncIndex == DssFuncMax || mDynamicFuncIndex == DssFuncMin
				|| mDynamicFuncIndex == DssFuncSum || mDynamicFuncIndex == DssFuncAvg
				|| mDynamicFuncIndex == DssFuncProduct || mDynamicFuncIndex == DssFuncMedian)	//optimize for max/min/sum/avg/product/median
            {
                *pResult = lNonAggResult;
                *pFlag = lFlag;
                return S_OK;
            }
        }
        
        // has Dynamic Agg defined
        DSSFunctionObject *lpFunctionObject = mpFunctionServer->FindFunctionObject(mDynamicFuncIndex, &lBuildInPackageGUID);;
        CHECK_PTR(lpFunctionObject);
        
        DSSAggregationFunction *lpAggFunction = dynamic_cast<DSSAggregationFunction*>(lpFunctionObject);
        if(lpAggFunction)
        {
            hr = lpAggFunction->Calculate(nParam, pParam, pResult, pFlag);
            CHECK_HR;
        }
        //lwang; 11/18/2008; 331949, handle First/Last function
        else if(mDynamicFuncIndex == DssFuncFirst || mDynamicFuncIndex == DssFuncLast)
        {
            return hApplyNonAggFunc(lpFunctionObject, nParam, pParam, pResult, pFlag);
        }
        else
        {
            return hApplyGenericFunc(lpFunctionObject, nParam, pParam, pResult, pFlag);
        }
    }
    
	return hr;
}

int CDSSConsolidationEvaluator::put_DynamicAggFunc(Int32 iFuncIndex)
{
	mDynamicFuncIndex = iFuncIndex;
    
	return S_OK;
}

int CDSSConsolidationEvaluator::put_NonAggFunc(Int32 iFuncIndex)
{
	mNonAggFuncIndex = iFuncIndex;
    
	return S_OK;
}

int CDSSConsolidationEvaluator::get_EvaluatorType(EnumEvaluatorType *pEvalType)
{
	CHECK_PTR(pEvalType);
    
	*pEvalType = mType;
    
	return S_OK;
}

//lwang, 10/09/2007, add support of non-numerical calculation
// implementation of ICDSSNNSimpleFunction: Calculate(), CalculateRepeat(), GetOutputType()
int CDSSConsolidationEvaluator::CalculateRepeat(int nParam, int nRepeat,CComVariant *ipParam, CComVariant *opResult,DSSDataFlag *opFlag)
{
	// this function should not be called
	return E_FAIL;
}

int CDSSConsolidationEvaluator::GetOutputType(int size, EnumDSSDataType *inputType, EnumDSSDataType *outputType)
{
 	if(outputType == NULL || inputType == NULL)
		return E_POINTER;
    
	*outputType = *inputType;
	return S_OK;
}

//This is the Non-Numerical simple function interface.
// The caller function should check the input data type, output data type and function type,
// to decide whether to call NonNumerical or Numerical interface.
int CDSSConsolidationEvaluator::Calculate(int nParam, CComVariant *ipParam,CComVariant *opResult, DSSDataFlag *opFlag)
{
    int hr = S_OK;
    
	CHECK_PTR(opResult);
	CHECK_PTR(opFlag);
    
	if(nParam < mnParam)
	{
		*opFlag = DssDataInvalid;
		return S_OK;
	}
    
	//now the fun started
    
	switch(mType)
	{
        case DssEvaluatorCalculation:
		{
			DSSExpNode * lpNode = mpExpression->getRoot();
			CHECK_PTR(lpNode);
            
			Int32 lParamUsed = 0;
			LONG index = 0;
			hr = hCalculateNN_0(lpNode, nParam, ipParam, index, opResult, opFlag, &lParamUsed);
		}
            break;
        case DssEvaluatorGroup:
		{
			hr = hCalculateNNGroup(nParam, ipParam, opResult, opFlag);
		}
            break;
        case DssEvaluatorFilter :
        case DssEvaluatorResidue:
        case DssEvaluatorElementSingle:
        case DssEvaluatorElementList:
		{
			hr = hCalculateNNDynamicAgg(nParam, ipParam, opResult, opFlag);
		}
            break;
        default:
            hr = E_FAIL;
	}
    
	if (hr != S_OK)
		return hr =  E_FAIL;
    
	return S_OK;
}
int CDSSConsolidationEvaluator::hCalculateNN_0(DSSExpNode *ipNode, Int32 nParam,VARIANT *ipParam, LONG index, VARIANT *opResult,DSSData_Flags *opFlag, Int32 *pParamUsed)
{
    int hr = S_OK;
    
	CHECK_PTR(ipNode);
    
	EnumDSSNodeType lNodeType = ipNode->getType();
    
	AE_ASSERT(opResult);
	AE_ASSERT(opFlag);
	AE_ASSERT(pParamUsed);
    
	switch(lNodeType)
	{
        case DssNodeConstant:
		{
			DSSExpNodeConstant *lpConstant = dynamic_cast<DSSExpNodeConstant*>(ipNode);
			CHECK_PTR(lpConstant);
            
			//EnumDSSDataType lDataType = (EnumDSSDataType)lpConstant->getDataType();
            
			// jxie, 2011-1-30, support numeric data now
            
			//// the logic to decide whether it's numerical type is following CDSSCube::IsNumericalSlice()
			//if (!mbHasCellFmt && (lDataType == DssDataTypeInteger || lDataType == DssDataTypeUnsigned
			//  ||lDataType == DssDataTypeNumeric || lDataType == DssDataTypeDecimal
			//  ||lDataType == DssDataTypeReal	|| lDataType == DssDataTypeDouble
			//  ||lDataType == DssDataTypeFloat	|| lDataType == DssDataTypeShort
			//  ||lDataType == DssDataTypeLong	|| lDataType == DssDataTypeBool
			//  ||lDataType == DssDataTypeBigDecimal || lDataType == DssDataTypeMissing))
			//{
			//	// maybe we want to support this in the future,
			//	// but now it should not happen, so just return error
			//	*opFlag = DssDataInvalid;
			//	//return E_FAIL;
			//}
			//else
			{
				opResult = lpConstant->getValue();
                
				*opFlag = DssDataOk;
				*pParamUsed = 0;
			}
		}
            break;
        case DssNodeElementsObject:
        case DssNodeShortcut:
		{
			AE_ASSERT(ipParam);
			hr = SafeArrayGetElement(ipParam->parray, &index, opResult);
			CHECK_HR;
            
			*opFlag = DssDataOk;
			*pParamUsed = 1;
		}
            break;
        case DssNodeOperator:
		{
			DSSExpNodeOperator *lpOperator = dynamic_cast<DSSExpNodeOperator*>(ipNode);
			CHECK_PTR(lpOperator);
            
			DSSFunction * lpFunction = lpOperator->getFunction();
			CHECK_PTR(lpFunction);
            
			DSSFunctionObject * lpFunctionObject;
			hr = hFindFunctionObject(lpFunction, DssFunctionTypeSimple, &lpFunctionObject);
			CHECK_HR_PTR(lpFunctionObject);
            
//			CComQIPtr<ICDSSCellFormatFunction, &IID_ICDSSCellFormatFunction> lpCellFmtFunc(lpFunctionObject);
            
			Int32 lnChild = ipNode->Count();
            
			//Create a one-dimensional Safearray for input paramters[lnChild];
			SAFEARRAYBOUND lBound;
			lBound.lLbound = 0;
			lBound.cElements = lnChild;
            
			CComVariant lChildrenValues;
			lChildrenValues.vt = VT_ARRAY|VT_VARIANT;
			lChildrenValues.parray = SafeArrayCreate(VT_VARIANT, 1, &lBound);
            
			DSSData_Flags lpChildrenFlags[lnChild];
			CHECK_NEWED_PTR(lpChildrenFlags);
            
			Int32 iPar = 0;
			Int32 lParamUsed = 0;
			Int32 iChild = 0 ;
			for(iChild = 0; iChild < lnChild; iChild ++)
			{
				DSSExpNode * lpChild = ipNode->Item(iChild);
				CHECK_PTR(lpChild);
                
				CComVariant lResult;
                
				hr = hCalculateNN_0(lpChild,
                                    nParam,
                                    ipParam, index + iPar,
                                    &lResult,
                                    &lpChildrenFlags[iChild],
                                    &lParamUsed);
                
				if(lpChildrenFlags[iChild] != DssDataOk)
				{
					*opFlag = lpChildrenFlags[iChild];
					return hr = S_OK;
				}
                
				CHECK_HR;
                
//				hr = hCheckCellFormatData(&lResult, lpCellFmtFunc);
				CHECK_HR;
                
				//add lResult to array
				LONG lindex = iChild;
				hr = SafeArrayPutElement(lChildrenValues.parray, &lindex, &lResult);
				CHECK_HR;
                
				iPar += lParamUsed;
			}
            
			//now we need to do the calculation
            /*
			if (mbHasCellFmt)
			{
				if (lpCellFmtFunc)
				{
					hr = lpCellFmtFunc->CalculateCellData(lnChild, &lChildrenValues, opResult, opFlag);
					CHECK_HR;
				}
				else
				{
					double * lpChildNum = reinterpret_cast<double *>(alloca(sizeof(double) * lnChild));
					CHECK_NEWED_PTR(lpChildNum);
                    
					hr = hGetNumerialVal(&lChildrenValues, lnChild, lpChildNum);
					CHECK_HR;
                    
					CComQIPtr<ICDSSSimpleFunction, &IID_ICDSSSimpleFunction> lpSimpleFunction(lpFunctionObject);
					if (lpSimpleFunction)
					{
						double ldVal = 0;
						hr = lpSimpleFunction->Calculate(lnChild, lpChildNum, &ldVal, opFlag);
						CHECK_HR;
						
						hr = CreateCellData(ldVal, opResult);
						CHECK_HR;
					}
					else
					{
						//lwang, 01/08/2009, 342892, at last apply non-numerical simple function
						CComQIPtr<ICDSSNNSimpleFunction, &IID_ICDSSNNSimpleFunction> lpNNSimpleFunction(lpFunctionObject);
						CHECK_PTR(lpNNSimpleFunction);
                        
						CComVariant lChildNNVal;
						lChildNNVal.vt = VT_ARRAY|VT_VARIANT;
						lChildNNVal.parray = SafeArrayCreate(VT_VARIANT, 1, &lBound);
                        
						Int32 iChild = 0 ;
						for(iChild = 0; iChild < lnChild; iChild ++)
						{
							CComVariant lData;
							hr = DFCData_ConvertToVariant((BYTE*)(lpChildNum + iChild), DssDataTypeDouble, sizeof(double), &lData);
							CHECK_HR;
                            
							LONG lindex = iChild;
							hr = SafeArrayPutElement(lChildNNVal.parray, &lindex, &lData);
							CHECK_HR;
						}
                        
						hr = lpNNSimpleFunction->Calculate(lnChild, &lChildNNVal, opResult, opFlag);
						CHECK_HR;
					}
				}//end of if (lpCellFmtFunc)
			}else
             */
			{
				DSSNNSimpleFunction *lpNNSimpleFunction = dynamic_cast<DSSNNSimpleFunction*>(lpFunctionObject);
				CHECK_PTR(lpNNSimpleFunction);
                CComVariant * lpResult;
				hr = lpNNSimpleFunction->Calculate(lnChild, &lChildrenValues, lpResult, opFlag);
				CHECK_HR;
                opResult = lpResult;
			}
            
			//here set the result
			*pParamUsed = iPar;
		}
            break;
            
        default:
            return E_FAIL;
	}
	return S_OK;
}

int CDSSConsolidationEvaluator::hCalculateNNGroup(Int32 nParam,VARIANT *ipParam, VARIANT *opResult, DSSData_Flags *opFlag)
{    
    int hr = S_OK;
	DSSExpNode * lpNode = mpExpression->getRoot();
	CHECK_PTR(lpNode);
    
	DSSExpNodeGroup *lpGroup = dynamic_cast<DSSExpNodeGroup*>(lpNode);
	CHECK_PTR(lpGroup);
    
	DSSFunction *lpFunction = NULL;
    /*  TODO NEED SUPPORT
     hr = lpGroup->get_Function(&lpFunction);
     CHECK_HR;
     */
    
    
	if(lpFunction == NULL)
	{
		hr = hCalculateNNDynamicAgg(nParam, ipParam, opResult, opFlag);
		CHECK_HR;
	}
	else if (mAggFuncInputDataType == DssDataTypeUnknown)
	{
		*opFlag = DssDataInvalid;
	}
	else
	{
		DSSFunctionObject * lpFunctionObject;
		hr = hFindFunctionObject(lpFunction, DssFunctionTypeAggregation, &lpFunctionObject);
		CHECK_HR_PTR(lpFunctionObject);
        
		hr = hApplyNNAggregation(lpFunctionObject, nParam, ipParam, opResult, opFlag);
		CHECK_HR;
	}
    
	return S_OK;
}

int CDSSConsolidationEvaluator::hCalculateNNDynamicAgg(Int32 nParam, VARIANT *ipParam,
                                                                             VARIANT *opResult, DSSData_Flags *opFlag)
{
    int hr = S_OK;
    
    if (0 == nParam)
    {
        if(mDynamicFuncIndex == DssFuncUnknown || mAggFuncInputDataType == DssDataTypeUnknown)
            *opFlag = DssDataUnCalculated;
        else
            *opFlag = DssDataInvalid;
    }
    else if (1 == nParam && DssEvaluatorElementSingle == mType)
    {
        CHECK_PTR(ipParam);
        
        LONG index = 0;
        hr = SafeArrayGetElement(ipParam->parray, &index, opResult);
        CHECK_HR;
        
        *opFlag = DssDataOk;
    }
    else
    {
        // no Dynamic Agg defined
        if(mDynamicFuncIndex == DssFuncUnknown || mAggFuncInputDataType == DssDataTypeUnknown)
        {
            *opFlag = DssDataUnCalculated;
        }
        else
        {
            // has Dynamic Agg defined
            GUID lBuildInPackageGUID = {0xACC5BFA0,0x11D3,0xDD9C,{0xC0,0x00,0x81,0xB9,0xEA,0x33,0x22,0x4F}};//GUID_DSSBuiltInPackage;

            DSSFunctionObject *lpFunctionObject = mpFunctionServer->FindFunctionObject(mDynamicFuncIndex, &lBuildInPackageGUID);
            CHECK_PTR(lpFunctionObject);
            
            hr = hApplyNNAggregation(lpFunctionObject, nParam, ipParam, opResult, opFlag, true);
            CHECK_HR;
        }
    }
    
	return S_OK;
}

int CDSSConsolidationEvaluator::hFindFunctionObject(DSSFunction * ipFunction,
                                                    EnumDSSFunctionType iType,
                                                    DSSFunctionObject **oppFunctionObject)
{
	int hr = S_OK;
    
	EnumDSSFunctionType lFunctionType = ipFunction->getAnalyticalType();
    
	if(lFunctionType != iType)
		return hr =  E_FAIL;
    
	// dzhou 2011-07-29 TQMS461880
    GUID lPackageGUID = ipFunction->getPackageID();
    *oppFunctionObject = mpFunctionServer->FindFunctionObject(ipFunction->getFunctionIndex(), &lPackageGUID);
    
//	hr = CDSSAEHelper::SetTrimStringFlag(*oppFunctionObject, mfTrimString);
	//CHECK_HR;
	
	return hr = S_OK;
}

// Assume consolidation element should have the same input and output data type, otherwise the calculation will fail.
// So the input data type here is not an array of all input parameters.
int CDSSConsolidationEvaluator::get_FunctionDataType(EnumDSSDataType inputDataType,
                                                     EnumDSSDataType *opOutputDataType,
                                                     VARIANT_BOOL *opIsNNFunction)
{
	int hr = S_OK;
    
	if (!opOutputDataType || !opIsNNFunction)
		return E_INVALIDARG;
    
	*opOutputDataType = DssDataTypeDouble;
	*opIsNNFunction = VARIANT_FALSE;
    
	DSSExpNode * lpNode = mpExpression->getRoot();
	CHECK_PTR(lpNode);
    
	switch(mType)
	{
        case DssEvaluatorCalculation:
		{
			hr = hGetCalculationDataType(lpNode, inputDataType, opOutputDataType, opIsNNFunction);
		}
            break;
        case DssEvaluatorGroup:
		{
			hr = hGetGroupDataType(lpNode, inputDataType, opOutputDataType, opIsNNFunction);
		}
            break;
        case DssEvaluatorFilter :
        case DssEvaluatorResidue:
        case DssEvaluatorElementSingle:
        case DssEvaluatorElementList:
		{
			hr = hGetDynamicAggDataType(inputDataType, opOutputDataType, opIsNNFunction);
		}
            break;
        default:
            hr = E_FAIL;
	}
	
	if (hr != S_OK)
		return hr =  E_FAIL;
    
	//lwang, 333292, calculation with cell format data
	if (inputDataType == DssDataTypeCellFormatData)
	{
		mbHasCellFmt = true;
        
		mAggFuncInputDataType = inputDataType;
		*opOutputDataType = DssDataTypeCellFormatData;
		*opIsNNFunction = VARIANT_TRUE;
	}
    
	return S_OK;
}

int CDSSConsolidationEvaluator::hGetCalculationDataType(DSSExpNode *ipNode,
                                                        EnumDSSDataType inputDataType,
                                                        EnumDSSDataType *opOutputDataType,
                                                        VARIANT_BOOL *opIsNNFunction)
{
	int hr = S_OK;
	EnumDSSNodeType lNodeType = ipNode->getType();
	CHECK_HR;
    
	switch(lNodeType)
	{
        case DssNodeConstant:
		{
            DSSExpNodeConstant *lpConstant = dynamic_cast<DSSExpNodeConstant*>(ipNode);
			CHECK_PTR(lpConstant);
            
			*opOutputDataType = (EnumDSSDataType)lpConstant->getDataType();
            
			//The function type is set to NN for constant.
			*opIsNNFunction = VARIANT_TRUE;
		}
            break;
        case DssNodeElementsObject:
        case DssNodeShortcut:
		{
			*opOutputDataType = inputDataType;
			//set function type to NN
			*opIsNNFunction = VARIANT_TRUE;
		}
            break;
        case DssNodeOperator:
		{
			DSSExpNodeOperator *lpOperator = dynamic_cast<DSSExpNodeOperator*>(ipNode);
			CHECK_PTR(lpOperator);
            
			DSSFunction * lpFunction = lpOperator->getFunction();
			CHECK_PTR(lpFunction);
            
			DSSFunctionObject * lpFunctionObject;
			hr = hFindFunctionObject(lpFunction, DssFunctionTypeSimple, &lpFunctionObject);
			CHECK_HR_PTR(lpFunctionObject);
            
			//lwang, 11/27/2007, TQMS 288894.
			DSSNNSimpleFunction *lpNNSimpleFunction = dynamic_cast<DSSNNSimpleFunction*>(lpFunctionObject);;
			if(!lpNNSimpleFunction)
			{
				DSSSimpleFunction *lpSimpleFunction = dynamic_cast<DSSSimpleFunction*>(lpFunctionObject);
				CHECK_PTR(lpSimpleFunction);
                
				*opOutputDataType = DssDataTypeDouble;
				*opIsNNFunction = VARIANT_FALSE;
			}
			else
			{
				Int32 lnChild = ipNode->Count();
                
				DSSDataType_Type inputType[lnChild];
				CHECK_NEWED_PTR(inputType);
                
				Int32 iChild = 0;
				for(iChild = 0; iChild < lnChild; iChild ++) {
					inputType[iChild] = inputDataType;
				}
                
				hr = lpNNSimpleFunction->GetOutputType(lnChild, inputType, opOutputDataType);
				CHECK_HR;
                
				*opIsNNFunction = VARIANT_TRUE;
			}
		}
            break;
        default:
            return E_FAIL;
	}
    
	return S_OK;
}

int CDSSConsolidationEvaluator::hGetGroupDataType(DSSExpNode *ipNode,
                                                  EnumDSSDataType inputDataType,
                                                  EnumDSSDataType *opOutputDataType,
                                                  VARIANT_BOOL *opIsNNFunction)
{
	int hr = S_OK;
    
	DSSExpNodeGroup *lpGroup = dynamic_cast<DSSExpNodeGroup*>(ipNode);
	CHECK_PTR(lpGroup);
    
	DSSFunction *lpFunction = NULL;
    /*  TODO NEED SUPPORT
     hr = lpGroup->get_Function(&lpFunction);
     CHECK_HR;
     */
    
	if(lpFunction != NULL)
	{
		DSSFunctionObject * lpFunctionObject;
		hr = hFindFunctionObject(lpFunction, DssFunctionTypeAggregation, &lpFunctionObject);
		CHECK_HR_PTR(lpFunctionObject);
        
		return hGetAggregationFuncDataType(lpFunctionObject, inputDataType, opOutputDataType, opIsNNFunction);
	}
	else
	{
		return hGetDynamicAggDataType(inputDataType, opOutputDataType, opIsNNFunction);
	}
}

int CDSSConsolidationEvaluator::hGetDynamicAggDataType(EnumDSSDataType inputDataType,
                                                       EnumDSSDataType *opOutputDataType,
                                                       VARIANT_BOOL *opIsNNFunction)
{
	//int hr = S_OK;
    
	//lwang, 11/27/2007, TQMS 288894.
	// mnParam is 0 if mType = {DssEvaluatorGroup, DssEvaluatorFilter, DssEvaluatorResidue,
	//						DssEvaluatorElementSingle, DssEvaluatorElementList}
	if (mnParam == 1 || mDynamicFuncIndex == DssFuncUnknown){
		*opOutputDataType = inputDataType;
		//set function type to NN
		*opIsNNFunction = VARIANT_TRUE;
		return S_OK;
	}
	else
	{        
        GUID lBuildInPackageGUID = {0xACC5BFA0,0x11D3,0xDD9C,{0xC0,0x00,0x81,0xB9,0xEA,0x33,0x22,0x4F}};//GUID_DSSBuiltInPackage;
        
        DSSFunctionObject *lpFunctionObject = mpFunctionServer->FindFunctionObject(mDynamicFuncIndex, &lBuildInPackageGUID);
        CHECK_PTR(lpFunctionObject);
        
		return hGetAggregationFuncDataType(lpFunctionObject, inputDataType, opOutputDataType, opIsNNFunction);
	}
}

int CDSSConsolidationEvaluator::hGetAggregationFuncDataType(DSSFunctionObject *ipFunctionObject,
                                                            EnumDSSDataType inputDataType,
                                                            EnumDSSDataType *opOutputDataType,
                                                            VARIANT_BOOL *opIsNNFunction)
{
	int hr = S_OK;
    
	//lwang, 11/27/2007, TQMS 288894.
	//ipFunctionObject can be ICDSSAggregationFunction and ICDSSVariantAggFunc at the same time
//	CComQIPtr<ICDSSVariantAggFunc, &IID_ICDSSVariantAggFunc> lpAggVariantFunc(ipFunctionObject);
    
	if(!false)//lpAggVariantFunc)
	
    {
		DSSAggregationFunction *lpAggFunction = dynamic_cast<DSSAggregationFunction*>(ipFunctionObject);
		if(lpAggFunction)
		{
			*opOutputDataType = DssDataTypeDouble;
			*opIsNNFunction = VARIANT_FALSE;
		}
		else
		{
			//lwang, 01/14/2009, 344149, it may be a generic function
			EnumDSSParameterType lOutParamType = DssParameterReserved;
			DSSNNGenericFunction *lpNNGenericFunction =  dynamic_cast<DSSNNGenericFunction*>(ipFunctionObject);
			if (!lpNNGenericFunction)
			{
				DSSGenericFunction *lpGenericFunction =  dynamic_cast<DSSGenericFunction*>(ipFunctionObject);
				CHECK_PTR(lpGenericFunction);
                
				hr = lpGenericFunction->GetOutputType(&lOutParamType);
				CHECK_HR;
                
				*opOutputDataType = inputDataType;
				mAggFuncInputDataType = inputDataType;
				*opIsNNFunction = VARIANT_FALSE;
			}
			else
			{
				hr = lpNNGenericFunction->GetOutputType(1, &inputDataType, opOutputDataType, &lOutParamType);
				CHECK_HR;
                
				mAggFuncInputDataType = inputDataType;
				*opIsNNFunction = VARIANT_TRUE;
			}
			if (lOutParamType != DssParameterScalar)
				return E_FAIL;
		}
	}
	else
	{
        //not support
        AE_ASSERT(false);
        /*
		{
			hr = lpAggVariantFunc->GetOutputType(inputDataType, opOutputDataType);
			CHECK_HR;
		}
         */
		mAggFuncInputDataType = inputDataType;
		*opIsNNFunction = VARIANT_TRUE;
	}
	return S_OK;
}

int CDSSConsolidationEvaluator::get_ParamCount(Int32 *opCount)
{
	if (!opCount)
		return E_POINTER;
    
	*opCount = mnParam;
	return S_OK;
}

//generate the parameter reference list. e.g. consolidation element = Books / (Books + Music)
//given that ipLeave = {0,1} // (0 = Books, 1 = Music)
// then we will get opParamRefList = {0,0,1}
// the caller will be able to re-generate parameter list to feed consolidation evaluator.
int CDSSConsolidationEvaluator::GenerateParamRefList(DSSTabularConsolidation* ipConsolidation,
                                                     DSSTabularData * ipTabularData,
                                                     Int32 *ipLeaves,
                                                     Int32 nLeaves,
                                                     Int32 *opParamRefList)
{
	if (!ipConsolidation || !ipTabularData || !ipLeaves || !opParamRefList)
		return E_INVALIDARG;
    
	int hr = S_OK;
    
	map<Int32, Int32> lmapRowToLeafID;		//map from row position in custom group to leaf ID
	map<DSS_ID, Int32, less<DSS_ID> > lmapGUIDToLeafID;		//map from branch element DSS_ID to leaf ID
    
	//build the map
	for(Int32 iLeaf = 0; iLeaf < nLeaves; iLeaf ++){
		Int32 lCSIndex = ipLeaves[iLeaf];
		if (lCSIndex != -1)
		{
			bool lfIsBranch;
			Int32 lRowInCG;
            vector<int> *lpvLeaves;
//            hr = ipConsolidation->Item(lCSIndex, 1, &lfIsBranch, &lRowInCG);
            hr = ipConsolidation->GetLeavesByElement(lCSIndex, &lfIsBranch, lpvLeaves);
            CHECK_HR;
            lRowInCG = (*lpvLeaves)[0];
            
			if (lfIsBranch)
			{
				DSS_ID lguid;
				hr = ipConsolidation->getChildElementID(lCSIndex, &lguid);
				CHECK_HR;
				//map from branch element's DSS_ID to iLeaf
				lmapGUIDToLeafID.insert(map<DSS_ID, Int32, less<DSS_ID> >::value_type(lguid, iLeaf));
			}
			else
			{
				lmapRowToLeafID[lRowInCG] = iLeaf;				//map from row position to iLeaf
			}
		}
	}
    /*
	//get custom group and lookup table
	Int32 lCGIndex = -1;
	hr = ipConsolidation -> get_Units(1, &lCGIndex);
	CHECK_HR;
    
	CComPtr<ICDSSTabularDataUnit> lpChildUnit;
	hr = ipTabularData ->Item(lCGIndex, &lpChildUnit);
	CHECK_HR_PTR(lpChildUnit);
    
	CComQIPtr<ICDSSXTabCustomGroup, &IID_ICDSSXTabCustomGroup> lpCustomGroup(lpChildUnit);
    
	CComPtr<ICDSSXTabLookUpTable> lpLookUpTable;
	hr = lpCustomGroup->get_LookUpTable(0, &lpLookUpTable);		//only support single attribute for DE
	CHECK_HR_PTR(lpLookUpTable);
    
	MBase::Buffer* lpBuffer = MContractMgr::CreateGovernedBuffer(1, 1, _T("CDSSConsolidationEvaluator"));
	if(!lpBuffer)	// if unsuccessful, we assume it is 2-tier and we create normal buffer
		lpBuffer = MBase::CreateBuffer(1, 1);
	MBase::BufferPtr lBufferMCMPtr(lpBuffer);
    
    
    DSSTabularUnit *lpTabularUnit = ipConsolidation->getChildUnit();
    DSSGlobalLookUpTable *lpLookUpTable = dynamic_cast<DSSGlobalLookUpTable*>(lpTabularUnit);
    */
	//check expression
	DSSExpNode * lpRootNode = mpExpression -> getRoot();
	CHECK_PTR(lpRootNode);
    
	EnumDSSNodeType lNodeType = lpRootNode -> getType();
    
	Int32 lParamCount = 0;
    
	switch(lNodeType)
	{
        case DssNodeOperator:
		{            
			Int32 lnLeaves = lpRootNode->Count();;
            
			Int32 iLeaf = 0 ;
			for(iLeaf = 0; iLeaf < lnLeaves; iLeaf ++)
			{
				DSSExpNode * lpChild = lpRootNode -> Item(iLeaf);
				CHECK_PTR(lpChild);
                
				EnumDSSNodeType lNodeType = lpChild -> getType();
                
				switch(lNodeType)
				{
                    case DssNodeElementsObject:
					{
						//the child is a concrete element
                        DSSExpNodeElementsObject *lpElementsObject = static_cast<DSSExpNodeElementsObject*>(lpChild);
						CHECK_PTR(lpElementsObject);
                        
						DSSBaseElementsProxy* lpElements = lpElementsObject->getElements();
						CHECK_PTR(lpElements);
                        
						Int32 lnElement = lpElements->Count();
                        
						_ASSERT(lnElement == 1);	//we only support single attribute for DE
                        
						DSSBaseElementProxy *lpElement = lpElements->Item(1);
						CHECK_PTR(lpElement);
                        /*      need to implement in the future
						Int32 lRowInTable = 0;
						hr = gFindElementInTable(lpElement, lpLookUpTable, lpBuffer, false, &lRowInTable);
						CHECK_HR;	//will return false if search fails
                        
						Int32 lRowPositionInCG = -1;
						hr = lpCustomGroup -> get_RowPosition(0, lRowInTable, &lRowPositionInCG);
						CHECK_HR;
                        */
                        
                        Int32 lRowPositionInCG = 0;//hard code
						map<Int32, Int32>::iterator it = lmapRowToLeafID.find(lRowPositionInCG);
						AE_ASSERT(it != lmapRowToLeafID.end());
                        
						if (lParamCount < mnParam)
							opParamRefList[lParamCount++] = it->second;
						else
							return S_FALSE;
					}
                        break;
                    case DssNodeShortcut:
					{
						//the child is shortcut to a consolidation element
						DSSExpNodeShortcut *lpShortcut = dynamic_cast<DSSExpNodeShortcut*>(lpChild);
						CHECK_HR_PTR(lpShortcut);
                        
						DSSObjectInfo *lpObjectInfo = lpShortcut->getTarget();
						CHECK_PTR(lpObjectInfo);
                        
						DSS_ID lguid = lpObjectInfo->getID();
                        
						map<DSS_ID, Int32, less<DSS_ID> >::iterator it = lmapGUIDToLeafID.find(lguid);
						AE_ASSERT(it != lmapGUIDToLeafID.end());
                        
						if (lParamCount < mnParam)
							opParamRefList[lParamCount++] = it->second;
						else
							return S_FALSE;
					}
                        break;
                    case DssNodeConstant:
                        break;
                    default:
                        break;
				}
			}//end of for(iChild = 0; iChild < lnChild; iChild ++)
		}
            break;
        case DssNodeConstant:
        case DssNodeElementsObject:
        case DssNodeShortcut:
		{}
            break;
        default:
            break;
	}
	AE_ASSERT(lParamCount == mnParam);
    
	return S_OK;
}
/*
int CDSSConsolidationEvaluator::hCheckCellFormatData(CComVariant *ipValue, ICDSSCellFormatFunction *ipCellFmtFunction)
{
	int hr = S_OK;
	//since ICDSSCellFormatFunction support both normal data and cell format data
	//we convert the data only if function is not cell format function
	if (ipValue->vt == VT_DISPATCH)
	{
		CComPtr<IDSSCellData> lpCellData;
		hr = ipValue->pdispVal->QueryInterface(IID_IDSSCellData, (void**)&lpCellData);
        
		if(lpCellData)
		{
			if(!ipCellFmtFunction)
			{
				double tmpValue = 0;
				hr = lpCellData->get_Value(&tmpValue);
				CHECK_HR;
                
				ipValue->Clear();
				ipValue->vt = VT_R8;
				ipValue->dblVal = tmpValue;
			}
		}//end of if(lpCellData)
	}
    
	return S_OK;
}


int CDSSConsolidationEvaluator::hGetNumerialVal(VARIANT *ipValue, Int32 nParam, double *opParam)
{
	int hr = S_OK;
	LONG index = 0;
	for(index = 0; index < nParam; index ++)
	{
		CComVariant iVal;
		hr = SafeArrayGetElement(ipValue->parray, &index, &iVal);
		CHECK_HR;
        
		if (iVal.vt == VT_DISPATCH)
		{
			CComPtr<IDSSCellData> lpCellData;
			hr = iVal.pdispVal->QueryInterface(IID_IDSSCellData, (void**)&lpCellData);
            
			if(lpCellData)
			{
				hr = lpCellData->get_Value(opParam);
				CHECK_HR;
			}
		}
		else
		{
			hr = VariantChangeType(&iVal, &iVal, 0, VT_R8);
			CHECK_HR;
            
			*opParam = iVal.dblVal;
		}
		opParam++;
	}
    
	return S_OK;
}

int CDSSConsolidationEvaluator::CreateCellData(double iValue, VARIANT* oResult)
{
	if(!oResult)
		return E_POINTER;
    
	int hr = S_OK;
    
	CDSSCellData* lpCellData = new CComObject<CDSSCellData>;
	if(!lpCellData)
		return E_OUTOFMEMORY;
	((IDSSCellData *)lpCellData)->AddRef();
    
	MDataType::CellFmtData lCellFmtData(iValue, CellFmtDataTypeDouble, GUID_NULL);
	hr = lpCellData->put_CellFmtData(lCellFmtData);
	if(hr!=S_OK) return hr;
    
	oResult->vt=VT_DISPATCH;
	oResult->pdispVal=static_cast<IDispatch*>(lpCellData);
    
	return S_OK;
}
*/
int CDSSConsolidationEvaluator::hApplyNNAggregation(DSSFunctionObject *ipFunc,
													Int32 nParam, VARIANT *ipParam,
													VARIANT *opResult, DSSData_Flags *opFlag,
													bool ibIsDynamicAgg //lwang, 01/13/2009; if DE aggregation function is explicitly set, First/Last should not be applied
													)
{
	int hr = S_OK;
    
	//lwang, 01/13/2009, 343282, support for non-agg metrics
	CComVariant lNonAggResult;
	DSSData_Flags lFlag = DssDataOk;
    
	if(ibIsDynamicAgg && mNonAggFuncIndex != DssFuncUnknown)
	{        
        GUID lBuildInPackageGUID = {0xACC5BFA0,0x11D3,0xDD9C,{0xC0,0x00,0x81,0xB9,0xEA,0x33,0x22,0x4F}};//GUID_DSSBuiltInPackage;
        
        DSSFunctionObject *lpNonAggFunctionObject = mpFunctionServer->FindFunctionObject(mNonAggFuncIndex, &lBuildInPackageGUID);
        CHECK_PTR(lpNonAggFunctionObject);

        
		hr = hApplyNonAggFunc(lpNonAggFunctionObject, nParam, ipParam, &lNonAggResult, &lFlag);
		CHECK_HR;
        
		nParam = 1;
		ipParam = &lNonAggResult;
	}
    /*
	if (mbHasCellFmt)
	{
		CComQIPtr<ICDSSCellFormatFunction, &IID_ICDSSCellFormatFunction> lpCellFmtFunc(ipFunc);
		if (lpCellFmtFunc)
		{
			hr = lpCellFmtFunc->CalculateCellData(nParam, ipParam, opResult, opFlag);
			CHECK_HR;
		}
		else
		{
			double * lpNumerical = reinterpret_cast<double *>(alloca(sizeof(double) * nParam));
			CHECK_NEWED_PTR(lpNumerical);
            
			hr = hGetNumerialVal(ipParam, nParam, lpNumerical);
			CHECK_HR;
            
			CComQIPtr<ICDSSAggregationFunction, &IID_ICDSSAggregationFunction> lpAggFunction(ipFunc);
			if(lpAggFunction)
			{
				double ldVal = 0;
				hr = lpAggFunction->Calculate(nParam, lpNumerical, &ldVal, opFlag);
				CHECK_HR;
                
				hr = CreateCellData(ldVal, opResult);
				CHECK_HR;
			}
			else
			{
				//lwang, 01/08/2009, 342892, at last apply non-numerical aggregation function
				CComQIPtr<ICDSSVariantAggFunc, &IID_ICDSSVariantAggFunc> lpAggVariantFunc(ipFunc);
				if (!lpAggVariantFunc)
				{
					return hApplyNNGenericFunc(ipFunc, nParam, ipParam, opResult, opFlag);
				}
                
				SAFEARRAYBOUND lBound;
				lBound.lLbound = 0;
				lBound.cElements = nParam;
                
				CComVariant lNonNumericalVal;
				lNonNumericalVal.vt = VT_ARRAY|VT_VARIANT;
				lNonNumericalVal.parray = SafeArrayCreate(VT_VARIANT, 1, &lBound);
                
				Int32 i = 0 ;
				for(i = 0; i < nParam; i ++)
				{
					CComVariant lData;
					hr = DFCData_ConvertToVariant((BYTE*)(lpNumerical + i), DssDataTypeDouble, sizeof(double), &lData);
					CHECK_HR;
                    
					LONG lindex = i;
					hr = SafeArrayPutElement(lNonNumericalVal.parray, &lindex, &lData);
					CHECK_HR;
				}
                
				hr = lpAggVariantFunc->Calculate(nParam, &lNonNumericalVal, mAggFuncInputDataType, opResult, opFlag);
				CHECK_HR;
			}
		}
	}
	else*/
	{
//		CComQIPtr<ICDSSVariantAggFunc, &IID_ICDSSVariantAggFunc> lpAggVariantFunc(ipFunc);
		if(false)//lpAggVariantFunc)
		{
			return S_OK;//lpAggVariantFunc->Calculate(nParam, ipParam, mAggFuncInputDataType, opResult, opFlag);
		}
		else
			return hApplyNNGenericFunc(ipFunc, nParam, ipParam, opResult, opFlag);
	}
    
	return S_OK;
}

//lwang; 11/18/2008; 331949; special handle First/Last function which are non-aggregation functions.
int CDSSConsolidationEvaluator::hApplyNonAggFunc(DSSFunctionObject *ipFunc, Int32 nParam, double *ipParam, double *opResult, DSSData_Flags *opFlag)
{
	int hr = S_OK;
    
	CComVariant input;
	SAFEARRAYBOUND linputSABound;
	linputSABound.lLbound = 0;
	linputSABound.cElements = nParam;
	input.vt = VT_ARRAY|VT_VARIANT;
	input.parray = SafeArrayCreate(VT_VARIANT, 1, &linputSABound);
    
	//make a input parameter array[nParam]
	for (LONG j = 0; j < nParam; j++){
		CComVariant lData;
		hr = DFCDataConversion::ConvertToVariant((BYTE*)ipParam, DssDataTypeDouble, sizeof(double), &lData);
		CHECK_HR;
        
		hr = SafeArrayPutElement(input.parray, &j, &lData);
		AE_ASSERT(hr == S_OK);
        
		ipParam++;
	}
    
	CComVariant output;
	hr = hApplyNonAggFunc(ipFunc, nParam, &input, &output, opFlag);
	CHECK_HR;
    
	if (*opFlag == DssDataOk){
		Int32 lResSize = sizeof(double);
		hr = DFCDataConversion::ConvertFromVariant(&output, DssDataTypeDouble, (BYTE*)opResult, &lResSize);
		CHECK_HR;
	}
    
	return S_OK;
}

int CDSSConsolidationEvaluator::hApplyNonAggFunc(DSSFunctionObject *ipFunc, Int32 nParam, VARIANT *ipParam, VARIANT *opResult, DSSData_Flags *opFlag)
{
	int hr = S_OK;
    
	DSSNNGenericFunction *lpNNGenericFunction =  dynamic_cast<DSSNNGenericFunction*>(ipFunc);
	CHECK_PTR(lpNNGenericFunction);
    
	EnumDSSDataType lDataType = DssDataTypeReserved;
	vector<DSSData_Flags> lvInFlags;
	lvInFlags.resize(nParam);
    
	//Since data flags are passed to CDSSConsolidationEvaluator, manually set to DssDataOk.
	for (Int32 i = 0; i < nParam; i++)
		lvInFlags[i] = DssDataOk;
    
    CComVariant* lpComVariant = new CComVariant(ipParam);
	
    hr = lpNNGenericFunction->SetInputParam(1, nParam, lDataType, lpComVariant, nParam > 0 ? &lvInFlags[0] : NULL);
	CHECK_HR;
    
	hr = lpNNGenericFunction->Calculate();
	CHECK_HR;
    
	Int32 ldataSize = 1;
    CComVariant* lpResult;
    hr = lpNNGenericFunction->GetOutput(1, &ldataSize, lpResult, opFlag);
    CHECK_HR;
    
    opResult = lpResult;
    
    return S_OK;    
}

//lwang, 01/14/2009, 344149, support generic function
int CDSSConsolidationEvaluator::hApplyGenericFunc(DSSFunctionObject *ipFunctionObject, Int32 nParam, double *ipParam, double *opResult, DSSData_Flags *opFlag)
{
	int hr =S_OK;
    
	DSSGenericFunction *lpGenericFunction =  dynamic_cast<DSSGenericFunction*>(ipFunctionObject);
	CHECK_PTR(lpGenericFunction);
    
	hr = lpGenericFunction->Clear();
	CHECK_HR;
    
	DSSData_Flags lFlag = DssDataOk;
	Int32 i = 1;
	for (i = 0; i < nParam; i++)
	{
		hr = lpGenericFunction -> SetInputParam(i + 1, 1, ipParam + i, &lFlag);
		CHECK_HR;
	}
    
	hr = lpGenericFunction -> Calculate();
	CHECK_HR;
	
	Int32 lnResultSize = 0;
	hr = lpGenericFunction->GetOutput(1, &lnResultSize, opResult, opFlag);
	CHECK_HR;
    
	return S_OK;
}

int CDSSConsolidationEvaluator::hApplyNNGenericFunc(DSSFunctionObject *ipFunctionObject, Int32 nParam, VARIANT *ipParam, VARIANT *opResult, DSSData_Flags *opFlag)
{
	int hr =S_OK;
    
	DSSNNGenericFunction *lpNNGenericFunction =  dynamic_cast<DSSNNGenericFunction*>(ipFunctionObject);
	CHECK_PTR(lpNNGenericFunction);
    
	hr = lpNNGenericFunction->Clear();
	CHECK_HR;
    
	DSSData_Flags lFlag = DssDataOk;
	Int32 i = 1;
	for (i = 0; i < nParam; i++)
	{
        CComVariant* lpComVariant = new CComVariant(ipParam + i);
		hr = lpNNGenericFunction -> SetInputParam(i + 1, 1 , mAggFuncInputDataType, lpComVariant, &lFlag);
		CHECK_HR;
	}
    
	hr = lpNNGenericFunction -> Calculate();
	CHECK_HR;
	
	Int32 lnResultSize = 0;
    CComVariant* lpResult;
	hr = lpNNGenericFunction->GetOutput(1, &lnResultSize, lpResult, opFlag);
	CHECK_HR;
    opResult = lpResult;
	return S_OK;
}

// kshi  2011-07-29 TQMS461880
int CDSSConsolidationEvaluator::SetStringCompareOption(VARIANT_BOOL ibTrimString)
{
	mfTrimString = (ibTrimString == VARIANT_TRUE)?true:false;
	return S_OK;
}

int CDSSConsolidationEvaluator::hGetAllLeaves(DSSExpNode *ipRoot, vector<DSSExpNode*> &opLeaves)
{
    if (NULL == ipRoot->getChildNode())     //ipRoot is a leave
    {
        opLeaves.push_back(ipRoot);
        return S_OK;
    }
    
    for (int i = 0; i < ipRoot->Count(); i++)
    {
        DSSExpNode *lpNode = ipRoot->Item(i);
        CHECK_PTR(lpNode);
        hGetAllLeaves(lpNode, opLeaves);
    }
    
    return S_OK;
}

