//
//  DSSFilterExprEvalCube.cpp
//  ReportServiceCore
//
//  Created by Zhuo Yang on 11/2/12.
//  Copyright (c) 2012 Microstrategy. All rights reserved.
//

#include <iostream>
#include "DSSFilterExprEvalCube.h"
#include "DSSCube.h"
#include "DSSCalculationPlan.h"
#include "DSSFunctionServer.h"
#include "DSSExpNode.h"
#include "DSSExpNodeShortcut.h"
#include "DSSExpNodeOperator.h"
//#include "DSSExpNodeTime.h"
#include "DSSExpNodeConstant.h"
#include "DSSStrongPtr.h"
#include "DSSDimMetricEvaluator.h"
#include "DSSCalculationHelper.h"
#include "DSSFunction.h"
#include "DSSCExpression.h"
#include "DSSFilter.h"
#include "DSSExpNodeFormShortcut.h"
#include "DSSExpNodeOperator.h"
#include "DSSFunctionObject.h"
#include "DSSExpNodeElementsObject.h"
#include "DSSTabularData.h"
#include "DSSTabularUnit.h"
#include "DSSTabularAttribute.h"
#include "DSSDimty.h"
#include "DSSGlobalLookUpTable.h"
#include "DSSCubeDataSlice.h"
#include "DSSTabularIndex.h"
#include "DSSExpNodeFieldShortCut.h"
#include "DSSExpNodeGridUnitShortcut.h"
#include "DSSRWFieldGroupModelIterator.h"
#include "DSSRWGridIterator.h"
#include "DSSRWFieldGroupNode.h"
#include "DSSDataModelTransaction.h"
#include "DSSXTabView.h"
#include "DSSXTabHeader.h"
#include "DFCDataConversion.h"
#include "DSSTabularMetrics.h"
#include <algorithm>

DSSFilterExprEvalCube::DSSFilterExprEvalCube(DSSCube *ipSourceCube, DSSCube *ipTargetCube, 
                                             DSSCalculationPlan *ipCalculationPlan,
                                             int iCurLevel, int iCurEvent, DSSFunctionServer *ipFunctionServer, 
                                             EventContext *ipEventContext) : 
mpSourceCube(ipSourceCube), mpTargetCube(ipTargetCube), mpCalculationPlan(ipCalculationPlan), mCurLevel(iCurLevel),
mCurEvent(iCurEvent), mpFunctionServer(ipFunctionServer), mpEventContext(ipEventContext),mpDocInstance(NULL),mpDataModel(NULL),mCurrentRow(-1)
{
    
}

int DSSFilterExprEvalCube::processFilter(DSSFilter *pFilter,DSSSliceID *opFilterSliceID)
{
    if(!pFilter)
		return S_FALSE;	//no filter
    
	DSSCExpression *lpExpression = pFilter->getExpression();
	CHECK_PTR(lpExpression);
    
	DSSExpNode *lpRoot = lpExpression->getRoot();
    
	//the rule is to return S_FALSE if a filter which can not be applied because of
	//either the leaf entity (attribute form, metric) can not be located. Thus we
	//give the caller a chance to skip the filtering
    
	//return error when unanticipated node or other errors encountered.
    
	//The caller of ProcessFilter if received S_FALSE, it will be
	//equivalent to empty filter which mean no filter is need to apply
	
    
	if (!lpRoot)
		// empty filter
		return S_FALSE;
    
	return processFilterUnit(lpRoot, opFilterSliceID);
}

int DSSFilterExprEvalCube::processFilterUnit(DSSExpNode *ipNode, DSSSliceID *opFilterSliceID)
{
    int hr = S_OK;
    
    //zhyang, init function just check whether source cube's tabular data and target cube's tabular data are same
    //hr = init()
    
    // EnumDSSNodeType lNodeType = ipNode->getType();
    EnumDSSExpressionType lExprType = ipNode->getExpressionType();
    
    switch (lExprType) {
        case DssFilterBranchQual:
            hr = processFilterBranchQual(ipNode, opFilterSliceID);
            break;
            
        case DssFilterSingleMetricQual:
        case DssFilterSingleBaseFormQual:
        case DssFilterSingleBaseFormExpression:
            //root is normal comparison operator
            hr = processFilterExpression(ipNode, opFilterSliceID);
            break;
            
        case DssFilterMultiBaseFormQual:
        case DssFilterJointFormQual:
            //root is normal comparison operator, leaf can be Elements
            hr = processFilterAdvancedAttributeQual(ipNode, opFilterSliceID);
            break;
            
        case DssFilterListFormQual:
        case DssFilterJointListQual:
        case DssFilterJointListFormQual:
            //root is IN, leaf can be Elements
            hr = processFilterAdvancedInQual(ipNode, opFilterSliceID);
            break;
            
        case DssFilterListQual:
            //root is IN
            hr = processFilterListQual(ipNode, opFilterSliceID);
            break;
            
        case DssFilterEmbedQual:
            hr = processFilterEmbedQual(ipNode, opFilterSliceID);
            break;
            
        case DssExpressionGeneric:
        //case DssExpressionReserved:
        case DssFilterMetricExpression:
            hr = processFilterGeneric(ipNode,  opFilterSliceID);
            break;
            
            //xhan 11/18/05 To deal with Relationship filter
        case DssFilterRelationshipQual:
            hr = processFilterRelationship(ipNode, opFilterSliceID);
            break;
            
        case DssExpressionCanceledPrompt:
		{
			// fcamargo 2/8/06
			// Ignore dormant prompt nodes. This is to support
			// dormant prompts in expressions
            
			// aye, 255303, 11/27/07
			// canceled prompt should be treated differently than null value.
			hr = S_FALSE;
		}
            break;
            
        default:
            assert(false);
            return hr = E_FAIL;
    }
    
    return hr;
}

int DSSFilterExprEvalCube::processFilterGeneric(DSSExpNode *ipNode, DSSSliceID *opFilterSliceID)
{
    int hr = S_OK;
    
    //here we received an expression with DssExpressionGeneric type
	//Three possibilities
	//1) a shortcut to another filter
	//2) a branch qualification if the node is an logic operator
	//3) a single metric qualification if the node is a comparison operator
    
	//This is basically the case where COM did not set the expression type. It is coming from MD4, thus
	//the possibilities are really limited.
    
    EnumDSSNodeType lNodeType = ipNode->getType();
    switch (lNodeType) {
        case DssNodeShortcut:
        {
            DSSExpNodeShortcut *lpShortcut = static_cast<DSSExpNodeShortcut *>(ipNode);
            CHECK_PTR(lpShortcut);
            
            DSSObjectInfo *lpTarget = lpShortcut->getTarget();
            CHECK_PTR(lpTarget);
            
            EnumDSSObjectType lObjectType = lpTarget->getType();
            if(lObjectType == DssTypeFilter)
            {
                DSSFilter *lpFilter = dynamic_cast<DSSFilter *>(lpTarget);
                if(!lpFilter)
                {
                    assert(false);
                    CHECK_PTR(lpFilter);
                }
                
                return processFilter(lpFilter, opFilterSliceID);
            }
            else
            {
                return E_FAIL;
            }
            break;
        }
        case DssNodeOperator:
        {
            DSSExpNodeOperator *lpOperator = dynamic_cast<DSSExpNodeOperator *>(ipNode);
            if(!lpOperator)
            {
                assert(false);
                return E_NOINTERFACE;
            }
            
            DSSFunction *lpFunction = lpOperator->getFunction();
			CHECK_PTR(lpFunction);
			
			EnumDSSFunctionSQLType lSQLType = lpFunction -> getSQLType();
            
			switch(lSQLType)
			{
                case DssFunctionSQLTypeLogic:
                    return hr = processFilterBranchQual(ipNode,
                                                        opFilterSliceID);
                case DssFunctionSQLTypeComparison:
                    return hr = processFilterExpression(ipNode,
                                                        opFilterSliceID);
                default:
                    //unknown function type
                    return hr = E_FAIL;
			}
        }
        break;
            
        default:
            //unknown Node type
            return hr = E_FAIL;
    }
    
    return hr;
}

int DSSFilterExprEvalCube::processFilterExpression(DSSExpNode *ipNode, DSSSliceID *opFilterSliceID)
{
    int hr = S_OK;
    
    // this function assume the input IDSSExpression to be:
	//
	// (1)			>				<- Output level is here now
	//			  /   \
	//			 /	   \
	//		  ShCut   Const			<- Continuatoin Dimty is associated with this node...
	//			|
	//		  metric
	//
	// (2)          >				<- Output level is here now
	//			  /   \
	//			 /	   \
	//	   Rank_Oper  Const			<- BreakBy is associated with Rank_Oper
	//			|
	//		  ShCut					<- Continuatoin Dimty is associated with this node...
	//			|
	//		  Metric
    
	// the other case is single base form qual and sing base form expression
    
    
	//We do not process Continuation or Output Level, could somebody explain it to me??
	//Even breakby, we could not deal with because it is going to be very complicated
	//I will put a function which is a dummy that return empty as the breakby level
    
	//we will assume, we have a way to retrieve a slice which would correspond to that metric
	//right now, we will just search for that metric id in TabularData
    
	//maybe we have to decide when to return S_FALSE and when to return FAIL
    
    return hr = evaluateSimpleExpression(ipNode, opFilterSliceID);
}

int DSSFilterExprEvalCube::evaluateSimpleExpression(DSSExpNode *ipNode, DSSSliceID *opSliceID)
{
    int hr = S_OK;
    
    //the assumption here is
	//the leaf of the expression have no elementsObject, have no attribute and
	//have no Multibase form.
    
	//By specifing this, all the generic operator now make sense (without resort to tuple operator)
    
    if(ipNode == NULL)
        return E_FAIL;
    
    EnumDSSNodeType lNodeType = ipNode->getType();
    switch (lNodeType) {
        case DssNodeConstant:
        case DssNodeTime:
        case DssNodeBigDecimal:
        case DssNodeCellFormatData:
        {
            hr = DSSCalculationHelper::GenerateConstantSlice(ipNode, mpTargetCube, opSliceID);
            CHECK_HR;
            break;
        }
        case DssNodeOperator:
        {
            // We expect the operators to be + - * /
            int lnChild, iChild;
            lnChild = ipNode->Count();
            DSSSliceID *lpChildSliceID = new DSSSliceID[lnChild];
            CHECK_PTR(lpChildSliceID);
            MBase::DSSStrongPtr<DSSSliceID, MBase::DeleteArray<DSSSliceID> > lpChildSliceIDPtr(lpChildSliceID);
            
            for(iChild = 0; iChild < lnChild; ++iChild)
            {
                DSSExpNode *lpChild = ipNode->Item(iChild);
                hr = evaluateSimpleExpression(lpChild, &lpChildSliceID[iChild]);
                CHECK_HR;
            }
            
            hr= evaluateExpressionFunction(ipNode, lnChild, lpChildSliceID, opSliceID);
            CHECK_HR;
            
            break;
        }
        case DssNodeShortcut:
        {
            DSSExpNodeShortcut *lpShortCut = dynamic_cast<DSSExpNodeShortcut *>(ipNode);
            if (!lpShortCut) {
                return E_NOINTERFACE;
            }
            
            DSSObjectInfo *lpObject = lpShortCut->getTarget();
            EnumDSSObjectType lTargetType = lpObject->getType();
            if(lTargetType != DssTypeMetric)
            {
                return S_FALSE;
            }
            
            GUID lMetricID = lpObject->getID();
            hr = evaluateMetric(lMetricID, opSliceID);
            CHECK_HR;
            
            break;
        }
        case DssNodeFormShortcut:
        {
            DSSExpNodeFormShortcut *lpFormShortCut = dynamic_cast<DSSExpNodeFormShortcut*>(ipNode);
            std::vector<DSSSliceID> lvSliceID;
			//should only be single base form
			hr = hGenerateAttributeFormInputs(lpFormShortCut, lvSliceID);
			if(hr != S_OK) return hr;
            
			if(lvSliceID.size() != 1)
				return E_FAIL;
            
			*opSliceID = lvSliceID[0];
            break;
        }
        case DssNodeFieldShortcut:
		{
			//can only process metric
			DSSExpNodeFieldShortcut *lpFGShortCut = dynamic_cast<DSSExpNodeFieldShortcut*>(ipNode);
			if(lpFGShortCut == NULL) return E_NOINTERFACE;
            
			string lNodeKey  = lpFGShortCut->getNodeKey();
			CHECK_HR;
            
			string lFieldKey = lpFGShortCut->getFieldKey();
			CHECK_HR;
            
            char *lpData = NULL;
            int lDataSize = 0;
            EnumDSSDataType lDataType = DssDataTypeReserved;
            DSSDataFlag lDataFlag = 0;
            bool lbDataUpdated = false;
            

            
            if(!mpDataModel)
                return E_FAIL;
            DSSRWIterator* lpIterator = NULL;
            hr = mpDataModel->getCachedIterator(lNodeKey,&lpIterator);
            if(hr != S_OK || !lpIterator)
                return E_FAIL;
            
            DSSRWFieldGroupModelIterator* lpFGIterator = dynamic_cast<DSSRWFieldGroupModelIterator*>(lpIterator);
            CHECK_PTR(lpFGIterator);
            
            int lCount = lpFGIterator->CountFields();
            
            DSSRWFieldGroupNode* lpFGNode = dynamic_cast<DSSRWFieldGroupNode*>(lpIterator->getRWNode());
            if(!lpFGNode)
                return E_FAIL;
            
            DSSFields* lpFields = lpFGNode->getFields();
            assert(lpFields->Count() == lCount);
			Int32 iField = 0;
			for(; iField < lCount; iField++)
			{
				DSSField* lpField = lpFields->Item(iField);
				CHECK_HR_PTR(lpField);
                
                string lKey = lpField->getKey();
                
                if (!lKey.compare(lFieldKey))
						break;
			}
			
            int iRow = lpFGIterator->getRowOrdinal(0);
            if(iRow < 0)
                iRow = 0;
			if(iField <= lCount)
			{
				//now we have the field index, we need to get the raw value
				//First try to get the value from the transaction data model
                
                DSSDataModelTransaction* lpTransaction = lpFGIterator->getTransactionDataModel();
				if(lpTransaction)
				{
                    int lColumn = lpFGIterator->getTrxColumnIndexByFieldID(iField);
                    if(lColumn >0)
                    {
                        //TODO: iRow
                        string* lpString = lpTransaction->getUpdatedAndSubmittedData(iRow,iField, lColumn);
                        if(lpString)
                        {
                            lDataType = lpTransaction->getColumnDataType(lColumn);
             
                            bool lbIsString = (DssDataTypeChar == lDataType || DssDataTypeVarChar == lDataType
                                || DssDataTypeMBChar == lDataType || DssDataTypeUTF8Char == lDataType);
             
                            if(lbIsString)
                            {
                                lDataSize = lpString->length() + 1;
                                lpData = new char[lDataSize];
                                memset(lpData,0,lDataSize);
                                memmove(lpData,lpString->c_str(), lpString->length());
                            }else if(lDataType == DssDataTypeBigDecimal)
                            {
                                //TODO:
                                MDataType::DSSBigDecimal* lpBigDecimal = new MDataType::DSSBigDecimal(lpString->c_str());
                                lDataSize = sizeof(MDataType::DSSBigDecimal);
                                lpData = (char*)lpBigDecimal;
                            }else
                            {
                                int lBufferSize = 0;
                                hr = DSSAEHelper::getDataSizeFromString(*lpString, lDataType, &lBufferSize);
                                CHECK_HR;
                                
                                lpData = new char[lBufferSize];
                                hr = DSSAEHelper::ConvertStringToData(lpString->c_str(), lDataType, lpData, lBufferSize, lDataSize);
                                CHECK_HR;
                            }
                            lbDataUpdated = true;
                        }
                    }

				}
                
				//data not chagned by transaction, get it from iterator
				if(lbDataUpdated == VARIANT_FALSE)
				{
                    const char* lpCChar = NULL;
					hr = lpFGIterator->getRawData(iRow, iField, &lpCChar, &lDataSize, &lDataType, &lDataFlag);
                    CHECK_HR;
                    
                    if(lDataFlag == DssDataNull)
                    {
                        hr = lpFGIterator->getRawDataForInitialRendering(iRow, iField, lDataSize, lDataType, &lpCChar, &lDataFlag);
                        CHECK_HR;
                    }
                    if(lpCChar && lDataFlag == DssDataOk )
                    {
                        lpData = new char[lDataSize];
                        memmove(lpData, lpCChar, lDataSize);
                    }else
                    {
                        lDataType = DssDataTypeVarChar;
                        lDataSize = 0;
                        lpData = new char[1];
                        memset(lpData, 0, 1);
                    }
				}
            
                switch(lDataType){
                    case DssDataTypeInteger:
                    {
                        int lInt = *((int*)lpData);
                        double lValue = lInt;
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lValue),
                                                                    sizeof(double),
                                                                    DssDataTypeDouble,
                                                                    DssDataOk,
                                                                    opSliceID);
                        CHECK_HR;
                        
                        break;
                    }
                    case DssDataTypeShort:
                    {
                        short lInt = *((short*)lpData);
                        double lValue = lInt;
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lValue),
                                                                    sizeof(double),
                                                                    DssDataTypeDouble,
                                                                    DssDataOk,
                                                                    opSliceID);
                        CHECK_HR;
                        
                        break;
                    }
                    case DssDataTypeUnsigned:
                    {
                        unsigned int lInt = *((unsigned int*)lpData);
                        double lValue = lInt;
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lValue),
                                                                    sizeof(double),
                                                                    DssDataTypeDouble,
                                                                    DssDataOk,
                                                                    opSliceID);
                        CHECK_HR;
                        
                        break;
                    }
                    case DssDataTypeLong:
                    {
                        long lInt = *((long*)lpData);
                        double lValue = lInt;
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lValue),
                                                                    sizeof(double),
                                                                    DssDataTypeDouble,
                                                                    DssDataOk,
                                                                    opSliceID);
                        CHECK_HR;
                        
                        break;
                    }
                    case DssDataTypeBool:
                    {
                        bool lInt = *((bool*)lpData);
                        double lValue = lInt;
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lValue),
                                                                    sizeof(double),
                                                                    DssDataTypeDouble,
                                                                    DssDataOk,
                                                                    opSliceID);
                        CHECK_HR;
                        
                        break;
                    }
                    case DssDataTypeFloat:
                    case DssDataTypeReal:
                    {
                        // fcamargo 8/23: Changed to use get_Value, constant noew stores data as variant
                        float flt = *((float*)lpData);
                        double lValue = flt;
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lValue),
                                                                    sizeof(double),
                                                                    DssDataTypeDouble,
                                                                    DssDataOk,
                                                                    opSliceID);
                        CHECK_HR;
                        
                        break;
                    }
                    case DssDataTypeDouble:
                    case DssDataTypeNumeric:
                    case DssDataTypeDecimal:
                    {
                        // fcamargo 8/23: Changed to use get_Value, constant noew stores data as variant
                        double lValue = *((double*)lpData);

                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lValue),
                                                                    sizeof(double),
                                                                    DssDataTypeDouble,
                                                                    DssDataOk,
                                                                    opSliceID);
                        CHECK_HR;
                        
                        break;
                    }

                    case DssDataTypeChar:
                    case DssDataTypeVarChar:
                    case DssDataTypeUTF8Char:
                    case DssDataTypeNChar:
                    case DssDataTypeNVarChar:
                    case DssDataTypeLongVarChar:
                    case DssDataTypeBinary:
                    case DssDataTypeVarBin:
                    case DssDataTypeLongVarBin:
                    case DssDataTypeMBChar:
                    case DssDataTypePattern:
                    case DssDataTypeNPattern:
                    {
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                  0, 0,
                                                  lpData,
                                                  lDataSize,
                                                  DssDataTypeVarChar,
                                                  DssDataOk,
                                                  opSliceID);
						CHECK_HR;
                        
						break;
					}
                    case  DssDataTypeInt64:
                    {
                        long long lValue = *lpData;
                        
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lValue),
                                                                    sizeof(long long),
                                                                    DssDataTypeInt64,
                                                                    DssDataOk,
                                                                    opSliceID);
                        CHECK_HR;
                        
                        break;
                    }
                    case DssDataTypeTime:
                    case DssDataTypeTimeStamp:
                    case DssDataTypeDate:
                    {
                        //now we should put it in a constant slice
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube, 0, 0,
                                                                    reinterpret_cast<char *>(&lpData), sizeof(MDataType::DateTime),
                                                                    DssDataTypeTimeStamp, DssDataOk, opSliceID);
                        CHECK_HR;
                        break;
                    }
					case DssDataTypeBigDecimal:
                    {
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                              0, 0,
                                              reinterpret_cast<char *>(lpData), sizeof(MDataType::DSSBigDecimal),
                                              DssDataTypeBigDecimal,
                                              DssDataOk, opSliceID);
                        CHECK_HR;
                        break;
					}
                    case DssDataTypeCellFormatData:
                    {
                        //TODO: treate as double here
                        double lValue = *lpData;
                        
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lValue),
                                                                    sizeof(double),
                                                                    DssDataTypeDouble,
                                                                    DssDataOk,
                                                                    opSliceID);
                        CHECK_HR;
                        break;
                    }
                    default:
                        return E_FAIL;
				}
                if(lpData)
                    delete lpData;
            }
            break;
        }
        case DssNodeGridUnitShortcut:
		{
            //can only process metric
			DSSExpNodeGridUnitShortcut *lpGridUnit = dynamic_cast<DSSExpNodeGridUnitShortcut*>(ipNode);
			if(lpGridUnit == NULL) return E_NOINTERFACE;
            
			string lNodeKey  = lpGridUnit->getNodeKey();
			CHECK_HR;
            
            if(!mpDataModel)
                return E_FAIL;
            DSSRWIterator* lpIterator = NULL;
            hr = mpDataModel->getCachedIterator(lNodeKey,&lpIterator);
            if(hr != S_OK || !lpIterator)
                return E_FAIL;
            
            DSSRWGridIterator* lpGridIterator = dynamic_cast<DSSRWGridIterator*>(lpIterator);
            CHECK_PTR(lpGridIterator);
            
			if(mCurrentRow >= 0)
			{
				CComVariant lData;
				hr = GenerateConstantFromGridRow(lpGridUnit, lpGridIterator, /*mpSliceWindow,*/ mCurrentRow, &lData);
				CHECK_HR;
                
				switch(lData.vt){
					case VT_UNKNOWN:
					case VT_I2:
					case VT_I4:
					case VT_R4:
					case VT_R8:
					case VT_BOOL:
					case VT_I1:
					case VT_UI1:
					case VT_UI2:
					case VT_UI4:
					case VT_INT:
					case VT_UINT:
					{
						// fcamargo 8/23: Changed to use get_Value, constant noew stores data as variant
                        
						// Sanity check to make sure the variant is of the right type
						if (lData.vt != VT_R8) {
							hr = lData.ChangeType( VT_R8 );
							if (FAILED(hr)) return hr;
						}
                        
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lData.dblVal),
                                                                    sizeof(double),
                                                                    DssDataTypeDouble,
                                                                    DssDataOk,
                                                                    opSliceID);
                        CHECK_HR;
						break;
                    }
					case VT_BSTR:
                    {
                        char *lpUTF8 = NULL;
                        int lLen = 0;
                        
                        const MBase::ReturnString lRSData(MBase::WideCharToUTF8(lData.bstrVal));
                        lLen = lRSData.GetLength() + sizeof(char);
                        lpUTF8 = new char[lLen];
                        assert(lpUTF8);
                        
                        if (lRSData.c_str() )
                            memmove(lpUTF8, lRSData.c_str(), lLen);
                        else
                            ::memset((void *)lpUTF8, 0, lLen);
                        
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    lpUTF8,
                                                                    lLen,
                                                                    DssDataTypeVarChar,
                                                                    DssDataOk,
                                                                    opSliceID);
						CHECK_HR;
                        delete lpUTF8;
                        break;
                    }
					case VT_I8:
					case VT_UI8:
					{
                        long long lValue = lData.lVal;
						hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lValue),
                                                                    sizeof(long long),
                                                                    DssDataTypeInt64,
                                                                    DssDataOk,
                                                                    opSliceID);
                        CHECK_HR;
						break;
					}
                    case VT_DATE:
                    {
                        //TODO:
                        break;
                    }
					case VT_DISPATCH:
                    {
                        //TODO:
                        break;
                    }
                    case VT_EMPTY:
                    {
                        double ldbValue = 0.0;
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&ldbValue),
                                                                    sizeof(double),
                                                                    DssDataTypeDouble,
                                                                    DssDataNull,
                                                                    opSliceID);
                        CHECK_HR;
						break;
                        break;
                    }
					default:
						return E_FAIL;
				}//end switch(lData.vt)
			}/*else
              {
              CComPtr<IDSSTemplate> lpTemplate;
              hr = lpGridIterator->get_Template(&lpTemplate);
              CHECK_HR_PTR(lpTemplate);
              
              CComQIPtr<ICDSSTemplate, &IID_ICDSSTemplate> licpTemplate(lpTemplate);
              CHECK_PTR(licpTemplate);
              
              CComPtr<ICDSSXTabView> lpXTabView;
              hr = licpTemplate->get_XTabView(&lpXTabView);
              CHECK_HR_PTR(lpXTabView);
              
              //Get the Cube from View
              CComPtr<ICDSSCube> lpCube;
              hr = lpXTabView->get_Cube(&lpCube);
              CHECK_HR_PTR(lpCube);
              
              //Get the TabularData from Cube
              CComPtr<ICDSSTabularData> lpTabularData;
              hr = lpCube->get_TabularData(&lpTabularData);
              CHECK_HR_PTR(lpTabularData);
              
              DSS_ID lUnitID = GUID_NULL;
              hr = lpGridUnit->get_TargetID(&lUnitID);
              CHECK_HR;
              
              CDSSSliceID* lpSliceID = NULL;
              
              CComPtr<IDSSSlicedGridWindow> lpSliceWindow;
              hr = lpGridIterator->get_SlicedGridWindow(&lpSliceWindow);
              CHECK_HR;
              
              Int32 lUnitIndex = -1;
              DSSTemplateUnit_Type lUnitType = DssTemplateReserved;
              hr = lpTabularData->FindUnitI(lUnitID, lUnitType, &lUnitIndex);
              CHECK_HR;
              
              CComPtr<ICDSSTabularDataUnit> lpTabularDataUnit;
              hr = lpTabularData->Item(lUnitIndex, &lpTabularDataUnit);
              if(hr == S_OK)
              {
              hr = lpTabularDataUnit->get_UnitType(&lUnitType);
              CHECK_HR;
              
              //attribute form
              if(lUnitType == DssTemplateAttribute)
              {
              //should only be single base form
              DSS_ID lFormID = GUID_NULL;
              hr = lpGridUnit->get_FormID(&lFormID);
              CHECK_HR;
              
              CComPtr<ICDSSTemplateUnit> lpTemplateUnit;
              hr = licpTemplate->SearchUnit(&lUnitID, DssTemplateAttribute, &lpTemplateUnit);
              CHECK_HR_PTR(lpTemplateUnit);
              
              CComQIPtr<ICDSSTemplateAttribute, &IID_ICDSSTemplateAttribute> lpTemplateAttribute(lpTemplateUnit);
              CHECK_PTR(lpTemplateAttribute);
              
              CComPtr<ICDSSAttribute> lpAttribute;
              hr = lpTemplateAttribute->get_Attribute(&lpAttribute);
              CHECK_HR_PTR(lpAttribute);
              
              Int32 lLocale = 0;
              //hr = lpGridUnit->GetDataLocale();
              CHECK_HR;
              
              hr = GenerateAttributeFormInputsFromOtherCube(lpCube, lpAttribute, lpTabularDataUnit, lUnitIndex, lFormID, lLocale, pSliceID);
              CHECK_HR;
              
              }
              }
              //metric
              else if(hr == S_FALSE)
              {
              hr = EvaluateMetricFromOtherCube(lpCube, lpSliceWindow, lUnitID, pSliceID);
              CHECK_HR;
              }else{
              return hr;
              }
              }*/
            break;
        }
        default:
            return E_FAIL;
    }
    
    return hr;
}

//converts metric DSS_ID into metric input integer id
int DSSFilterExprEvalCube::evaluateMetric(GUID &iMetricID, DSSSliceID *opMetricID)
{
    int hr = hGenerateMetricSlice(iMetricID, opMetricID);
    
    if(hr != S_OK)
    {
        //must be a derived metric
        /*
		EnumNullCheckingOption lNullCheckingOption = DssNumericErrorSkip;
		{
			// jxie, 2011-12-8, TQMS 525637, the smart metric calculation in VF, ML and threshold should read AE NullCheckingOption.
			// Now we just read AE NullChecking in SM calculation part.
			// For condition evaluation in VF, ML and threshold, we need more discussion if read or not
            
			//TQMS 369006 xiyu read Null option from the template if any
			if(mpTemplate){
				CComPtr<ICDSSObjectInfo> lcpObjInfo;
				hr = mpTemplate->get_Info(&lcpObjInfo);
				CHECK_HR;
				if (lcpObjInfo){
					DSS_ID lTemplateID;
					hr = lcpObjInfo->get_ID(&lTemplateID);
					CHECK_HR;
                    
					DSS_ID lViewID = lTemplateID;
                    
					hr = CDSSAEHelper::FindNullCheckingOption(
                                                              mpReportInstance,
                                                              mpObjContext,
                                                              mNullCheckingOption,
                                                              lTemplateID,
                                                              DssTypeTemplate,
                                                              &lNullCheckingOption,
                                                              &lViewID);
					CHECK_HR;
				}
			}
			else
			{
				//TQMS 227591 ; xhan 10/21/05
				hr = CDSSAEHelper::FindNullCheckingOption(mpReportInstance,
                                                          &lNullCheckingOption);
				CHECK_HR;
			}
		}
        
		CDSSDimMetricEvaluator lDimMetricEvaluator(mpSource,
                                                   mpObjContext,
                                                   mpFuncServer,
                                                   mpSourceCube,
                                                   mpTargetCube,
                                                   mpCalculationPlan,
                                                   mCurLevel,
                                                   mCurEvent,
                                                   lNullCheckingOption,	// jxie, 2011-12-15, read AE NULL Checking option for SM in VF, ML and threshold
                                                   DssEventCompoundMetric,
                                                   //ctang 2010-5-6 TQMS 398917
                                                   mpReportInstance,
                                                   mpTemplate, //xhan 10/03/05 TQMS 208970 View filter evaluation AV
                                                   mpBuffer,
                                                   0,
                                                   NULL,
                                                   mpLocalSchema);
        
		lDimMetricEvaluator.set_FilterFlag(true);
        */
        mpEventContext->mpEvent = new DSSCalculationEvent();
        mpEventContext->mpEvent->mEventType = DssEventCompoundMetric;
        mpEventContext->mpEvent->mEventIndex = mCurEvent;
        
        DSSDimMetricEvaluator lDimMetricEvaluator(mpEventContext, DssNumericErrorSkip);
        //fali, 889852, set view filter flag to allow get metric index regardless of Gov metric later. 
        lDimMetricEvaluator.set_FilterFlag(true);
        hr = lDimMetricEvaluator.CalculateDimMetric(iMetricID, opMetricID);
        CHECK_HR;
        opMetricID->mpCube = mpEventContext->mpCube;
    }
    
    return hr;
}

int DSSFilterExprEvalCube::hGenerateMetricSlice(GUID &iMetricID, DSSSliceID *opSliceID)
{
    int hr = S_OK;
    int lMetricIndex = mpCalculationPlan->getMetricIndex(iMetricID);
    if(lMetricIndex >= 0)
    {
        bool lfCalculated;
        int lOrgSliceID;
        
        hr = mpCalculationPlan->getMetricSliceID(mCurLevel, mCurEvent-1, lMetricIndex, &lfCalculated, &lOrgSliceID);
        CHECK_HR;
        
        if(lfCalculated)
        {
            opSliceID->mSliceID = lOrgSliceID;
            opSliceID->mpCube = mpSourceCube;
            return S_OK;
        }
    }
    
    // not find the metric in the plan or not calculated in the plan
    // try to go through the express to calculate the metric     
    DSSObjectInfo *lpMetricInfo = mpEventContext->mpObjectContext->FindObjectH(iMetricID);
    DSSMetric *lpMetric = dynamic_cast<DSSMetric*>(lpMetricInfo);
    if(!lpMetric)
        return hr = E_FAIL;
    
    DSSCExpression *lpLimit = lpMetric->getExpression();
    CHECK_PTR(lpLimit);
    DSSExpNode *lpRoot = lpLimit->getRoot();
    
    //EvaluateSimpleExpression is base class CDSSFilterExprEval's travese function
    hr = evaluateSimpleExpression(lpRoot, opSliceID);
    
    return hr; //702437
}

//This function carries out arithmetic and logic calculation such as +,-,*,/, <, > 
int DSSFilterExprEvalCube::evaluateExpressionFunction(DSSExpNode *ipNode, int lnChild, DSSSliceID *ipSliceID, DSSSliceID *opSliceID)
{
    int hr = S_OK;
    
    DSSExpNodeOperator *lpOperator = dynamic_cast<DSSExpNodeOperator *>(ipNode);
    if(!lpOperator)
    {
        return E_NOINTERFACE;
    }
    
    DSSFunction *lpFunction = lpOperator->getFunction();
    EnumDSSFunctionSQLType lFunctionSQLType = DssFunctionSQLTypeReserved;
    
    if(lpFunction)
    {
        lFunctionSQLType = lpFunction->getSQLType();
    }
    
    vector<DSSCubeDataSlice*> lvSlice;
    vector<DSSSliceID*> lvSliceID;
    for (int i = 0; i < lnChild; i++)
    {
        lvSliceID.push_back(&ipSliceID[i]);
        DSSCubeDataSlice *lpSlice = NULL;
        hr = ipSliceID[i].GetSlice(&lpSlice);
        CHECK_HR;
        lvSlice.push_back(lpSlice);
    }
    
    MCESchema* lpSchema = mpEventContext?mpEventContext->mpSchema:NULL;
    CalculationContext *lCalcContext = DSSCalculationHelper::getCalculationContext(NULL,
                                                                                   (EnumSimpleOperatorType)lpOperator->getOperatorType(),
                                                                                   mpTargetCube,
                                                                                   NULL,
                                                                                   &lvSliceID,
                                                                                   DssNumericErrorSkip,
                                                                                   lpSchema);
    MBase::DSSStrongPtr<CalculationContext> lCalcContextPtr(lCalcContext);
    
    if(lFunctionSQLType == DssFunctionSQLTypeRelative)
    {
        vector<Int32> lvUnits;
        hr = DSSCalculationHelper::FindMetricBreakbyLevel(ipNode, mpTargetCube->getTabularData(), lvSlice, &lvUnits);
        CHECK_HR;
        
        lCalcContext->mvBreakbyUnits.assign(lvUnits.begin(), lvUnits.end());
        /*
        // yma 8/4/08, TQMS 320895, need to read sort by
        VARIANT_BOOL lfOrderSignificant;
        hr = lpFunction->get_IsOrderSignificantNS(&lfOrderSignificant);
        CHECK_HR;
        
        MBase::StrongBufferPtr<CDSSXTabSortDefns> lpSortDefnsPtr;
        if(lfOrderSignificant == VARIANT_TRUE && mpTemplate != NULL)
        {
            hr =  CDSSCalculationHelper::FindMetricSortbyDefns(
                                                               ipNode,
                                                               mCurLevel,
                                                               mCurEvent,
                                                               mpTemplate,
                                                               mpSourceCube,
                                                               mpCalculationPlan,
                                                               mpReportInstance,
                                                               inSliceID,
                                                               ipSliceID,
                                                               mpBuffer,
                                                               &lpSortDefnsPtr
                                                               );
            CHECK_HR;
        }
        */
    }
    
    
    hr = DSSCalculationHelper::CalculateNewSlice(lpOperator, lCalcContext, opSliceID);
    if (lCalcContext->mpFuncObject)
    {
        //delete lCalcContext->mpFuncObject;
        MBase::DSSStrongPtr<DSSFunctionObject> lFunctionObjectPtr(lCalcContext->mpFuncObject);
    }
    CHECK_HR;
    
    return hr;
}

int DSSFilterExprEvalCube::processFilterBranchQual(DSSExpNode *ipNode, DSSSliceID *opFilterSliceID)
{
    int hr = S_OK;
    
    Int32 lnChild = ipNode -> Count();
    
	DSSSliceID *lpChildSliceID = new DSSSliceID[lnChild];
    CHECK_PTR(lpChildSliceID);
    MBase::DSSStrongPtr<DSSSliceID, MBase::DeleteArray<DSSSliceID> > lpChildSliceIDPtr(lpChildSliceID);
    
	// aye, 255303, 11/27/07
	// canceled prompt handling, allow S_FALSE
	// A child may become empty due to canceled prompt.
	// We skip it
    
	Int32 lnNonEmptyChild = 0;
	Int32 lNonEmptyChild = 0;
	for	(int iChild = 0; iChild < lnChild; iChild++)
	{
		DSSExpNode *lpChildNode = ipNode->Item(iChild);
		CHECK_PTR(lpChildNode);
        
		hr = processFilterUnit(lpChildNode,  &lpChildSliceID[lNonEmptyChild]);
		if (FAILED(hr)) return hr;
        
		if (hr == S_OK)
		{
			lnNonEmptyChild++;
			lNonEmptyChild++;
		}
	}
    
	if (lnNonEmptyChild == 0)
		return S_FALSE;
    
	DSSExpNodeOperator *lpOperator = dynamic_cast<DSSExpNodeOperator*>(ipNode);
	if(!lpOperator) return E_NOINTERFACE;
    
    EnumDSSFunction lFunction = DssFunctionReserved;
    if(lpOperator->getFunction())
    {
        lFunction = lpOperator -> getFunction()->getFunctionType();
    }else{
        lFunction = (EnumDSSFunction)lpOperator->getFunctionIndex();
    }
    
	if(lFunction == DssFunctionAnd ||
       lFunction == DssFunctionNot ||
       lFunction == DssFunctionOr){
		//now depend on it is AND or OR
		hr =  evaluateRelationFunction(lFunction, lnNonEmptyChild, lpChildSliceID, opFilterSliceID);
		return hr;
	}
    
	return E_FAIL;
}

int DSSFilterExprEvalCube::processFilterAdvancedAttributeQual(DSSExpNode *ipNode, DSSSliceID *opFilterSliceID)
{
    // The result filter tree should look like the following if the operator is not "<>":
	//             And
	//             / \
	//          Optr Optr
	//
	// Otherwise, the tree should look like:
	//              Or
	//             / \
	//          Optr Optr
	//
    
    
	// Here our strategy is to produce by the first argument, a list of attribute (base) form slice and attributes
	// For the other arguments, it should be a list of slice and elements
	// The position should be exactly attriute form match the slice and attributes match elements
    
	// we need two function, first one will convert a ICDSSNode to a list of attribute form slice and attributes
	// the other one convert a ICDSSNode to a list of slice and elements
    int hr = S_OK;
    
    Int32 lnChild = ipNode -> Count();
    
	if(lnChild < 1) return hr = E_FAIL;
    
	std::vector<FilterExpressionLeaf> ** lpArgument = new std::vector<FilterExpressionLeaf>*[lnChild];
	CHECK_NEWED_PTR(lpArgument);
    
	for(int iChild = 0; iChild < lnChild; iChild ++)
	{
		DSSExpNode *lpChild = ipNode -> Item(iChild);
		if(hr != S_OK) return hr;
        
		lpArgument[iChild] = new std::vector<FilterExpressionLeaf>();
        
		CHECK_NEWED_PTR(lpArgument[iChild]);
        
		hr = hRetrieveExpressionLeaf(lpChild, *lpArgument[iChild]);
		if(hr != S_OK) return hr;
	}
    
	//now we need to figure it out
	//first all the array returned are going to be the same size
	Int32 iList, lnList = lpArgument[0] -> size();
	for(int iChild = 0; iChild < lnChild; iChild ++)
		if(lpArgument[iChild] -> size() != lnList)
			return hr = E_FAIL;
    
	DSSSliceID * lpSliceID = new DSSSliceID[lnList];
	CHECK_NEWED_PTR(lpSliceID);
    
	DSSSliceID *lpChildSliceID = new DSSSliceID[lnChild];
	CHECK_NEWED_PTR(lpChildSliceID);
    
	std::vector<FilterExpressionLeaf> lArgByList;
	for(iList = 0; iList < lnList; iList ++)
	{
		lArgByList.clear();
		Int32 lNum = 0;
		for(int iChild = 0; iChild < lnChild; iChild ++) {
			lArgByList.push_back((*lpArgument[iChild])[iList]);
			if ((*lpArgument[iChild])[iList].mType != SLICE)
				lpChildSliceID[lNum ++] = (*lpArgument[iChild])[iList].mSliceID;
		}
        //the operator to use is pNode
		hr = evaluateFilterExpression(ipNode, lArgByList, &lpSliceID[iList]);
		if(hr != S_OK) return hr;
	}
    
    
	//Operator, need to check the pNode, if it is "<>"
	DSSExpNodeOperator *lpOperator = dynamic_cast<DSSExpNodeOperator*>(ipNode);
	if(!lpOperator) return hr = E_NOINTERFACE;
    
    EnumDSSFunction lFuncType = DssFunctionReserved;
    if(lpOperator->getFunction())
    {
        lFuncType = lpOperator -> getFunction()->getFunctionType();
    }else{
        lFuncType = (EnumDSSFunction)lpOperator->getFunctionIndex();
    }
    
	if(lFuncType != DssFunctionNotEqual)
		//now depend on it is AND or OR
        hr = evaluateRelationFunction(DssFunctionAnd,  lnList, lpSliceID, opFilterSliceID);
    else
        hr = evaluateRelationFunction(DssFunctionOr, lnList, lpSliceID, opFilterSliceID);
    
    return hr;
}

int DSSFilterExprEvalCube::processFilterAdvancedInQual(DSSExpNode *ipNode,DSSSliceID *opFilterSliceID)
{
    int hr = S_OK;
    //modified from DFCAppObjsLoader::ProcessFilterAdvancedInQual
	// The result filter tree shoud look like:
	//                   OR
	//                  /  \
	//				  And  And
	//                / \  / \
	//               =  = =   =
	//
    
    
	// we need to take out every child beginning with the second one to
	// compare with the first one
	// for each comparison we need to compare both
    
    
	//actually the ultimate representation is an array of sliceID and array of variants.
	//and array of unit which indicate the table
	//also an array of type
    
	Int32 lnChild = ipNode -> Count();
    
	if(lnChild < 2) return hr = E_FAIL;
    
	std::vector<FilterExpressionLeaf> ** lpArgument = new std::vector<FilterExpressionLeaf>*[lnChild];
	CHECK_NEWED_PTR(lpArgument);
    
	for(int iChild = 0; iChild < lnChild; iChild ++)
	{
		DSSExpNode *lpChild = ipNode -> Item(iChild);
		if(hr != S_OK) return hr;
        
		lpArgument[iChild] = new std::vector<FilterExpressionLeaf>();
        
		CHECK_NEWED_PTR(lpArgument[iChild]);
        
		hr = hRetrieveExpressionLeaf(lpChild, *lpArgument[iChild]);
		if(hr != S_OK) return hr;
	}
    
	//now we need to figure it out
	//first all the array returned are going to be the same size
	Int32 iList, lnList = lpArgument[0] -> size();
	for(int iChild = 0; iChild < lnChild; iChild ++)
		if(lpArgument[iChild] -> size() != lnList)
			return hr = E_FAIL;
    
	DSSSliceID * lpSliceID = new DSSSliceID[lnChild - 1];
	CHECK_NEWED_PTR(lpSliceID);
    
	DSSSliceID * lpSliceID2 = new DSSSliceID [lnList];
	CHECK_NEWED_PTR(lpSliceID2);
    
	std::vector<FilterExpressionLeaf> lArgByList;
    
	for(int iChild = 1; iChild < lnChild; iChild ++)
	{
		for(iList = 0; iList < lnList; iList ++)
		{
			lArgByList.clear();
			lArgByList.push_back((*lpArgument[0])[iList]);
			lArgByList.push_back((*lpArgument[iChild])[iList]);
            
			hr = evaluateEqualFunction(lArgByList, &lpSliceID2[iList]);
			if(hr != S_OK) return hr;
		}
        
		hr = evaluateRelationFunction(DssFunctionAnd, lnList, lpSliceID2, &lpSliceID[iChild - 1]);
		if(hr != S_OK) return hr;
	}
	
    
	DSSSliceID lFilterID;
	hr = evaluateRelationFunction(DssFunctionOr, lnChild - 1, lpSliceID, &lFilterID);
	CHECK_HR;
    
	//to see if it is not in list or in list
	DSSExpNodeOperator *lpOperator = dynamic_cast<DSSExpNodeOperator*>(ipNode);
	CHECK_PTR(lpOperator);
    
    EnumDSSFunction lFuncType = DssFunctionReserved;
    if(lpOperator->getFunction())
    {
        lFuncType = lpOperator -> getFunction()->getFunctionType();
    }else{
        lFuncType = (EnumDSSFunction)lpOperator->getFunctionIndex();
    }
    
	if (lFuncType == DssFunctionNotIn) { //add not
		hr = evaluateRelationFunction(DssFunctionNot, 1, &lFilterID, opFilterSliceID);
		CHECK_HR;
        
	} else
		*opFilterSliceID = lFilterID;
    
    return hr;
}

int DSSFilterExprEvalCube::processFilterListQual(DSSExpNode *ipNode,DSSSliceID *opFilterSliceID)
{
    int hr = S_OK;
    
    //does the function in the Node has to be IN ????
	//does the node have exactly two children???? I am assuming there can be any number of children
	//from the second children, all the element will just be concatenated
    
	Int32 lnChild = ipNode -> Count();
    
	if(lnChild < 1) return hr = E_FAIL;
    
	std::vector<FilterExpressionLeaf> ** lpArgument = new std::vector<FilterExpressionLeaf>*[lnChild];
	CHECK_NEWED_PTR(lpArgument);
    
	for(int iChild = 0; iChild < lnChild; iChild ++)
	{
		DSSExpNode *lpChild = ipNode -> Item(iChild);
		if(hr != S_OK) return hr;
        
		lpArgument[iChild] = new std::vector<FilterExpressionLeaf>();
        
		CHECK_NEWED_PTR(lpArgument[iChild]);
        
		hr = hRetrieveExpressionLeaf(lpChild, *lpArgument[iChild]);
		if(hr != S_OK) return hr;
        
	}
    
	//when we evaluate, the first array should only have one element which is a table,
	//the rest of the array, each element will be a variant and we will try to
	//find that element in the table, we then need to OR all the slices resulted
    
	if(lpArgument[0] -> size() != 1 || (*lpArgument[0])[0].mType != ATTRIBUTE2)
		//the filter is not well structured
		return hr = E_FAIL;
    
	//this is the table we will use to find the element
	//Int32 lUnit = (*lpArgument[0])[0].mAttributeUnit;
	
	std::vector<DSSSliceID> lvChildSliceID;
    
	for(int iChild = 1; iChild < lnChild; iChild ++)
	{
		Int32 iElement, lnElement = lpArgument[iChild] -> size();
        
		for(iElement = 0; iElement < lnElement; iElement ++)
		{
			if((*lpArgument[iChild])[iElement].mType != ELEMENT)
				//the filter is not well structured
				return E_FAIL;
            
			DSSSliceID lSliceID;
			hr = evaluateElementFilter((*lpArgument[0])[0].mAttributeUnit, (*lpArgument[iChild])[iElement], &lSliceID);
			if(hr != S_OK) return hr;
            
			lvChildSliceID.push_back(lSliceID);
		}
		
	}
    
	DSSSliceID lFilterID;
	//02/01/06 nprabhudessai: VC 8 requirement
	DSSSliceID *lpCDSSSliceID = NULL; // dummy null pointer
	hr = evaluateRelationFunction(DssFunctionOr, lvChildSliceID.size(), (lvChildSliceID.size()>0)?&lvChildSliceID[0]:lpCDSSSliceID, &lFilterID);
	CHECK_HR;
    
	//to see if it is not in list or in list
	//Added by Xiaonan Han 04/18/01
	DSSExpNodeOperator *lpOperator = dynamic_cast<DSSExpNodeOperator*>(ipNode);
	CHECK_PTR(lpOperator);
    
    EnumDSSFunction lFuncType = DssFunctionReserved;
    if(lpOperator->getFunction())
    {
        lFuncType = lpOperator -> getFunction()->getFunctionType();
    }else{
         lFuncType = lpOperator->getFunctionIndex() == DssFuncNot ? DssFunctionNotIn : DssFunctionReserved;
    }
    
	if (lFuncType == DssFunctionNotIn) { //add not
		hr = evaluateRelationFunction(DssFunctionNot, 1, &lFilterID, opFilterSliceID);
		CHECK_HR;
        
	} else
		*opFilterSliceID = lFilterID;
    
    return hr;
}

int DSSFilterExprEvalCube::processFilterEmbedQual(DSSExpNode *ipNode,DSSSliceID *opFilterSliceID)
{
    int hr = S_OK;
    
    DSSExpNodeShortcut *lpShortCut = dynamic_cast<DSSExpNodeShortcut *>(ipNode);
    if (!lpShortCut) {
        return E_NOINTERFACE;
    }
    
    DSSObjectInfo *lpObject = lpShortCut->getTarget();
    EnumDSSObjectType lTargetType = lpObject->getType();
    
	switch (lTargetType)
	{
        case DssTypeFilter:
		{
			DSS_ID lTargetID = lpObject->getID();
            
			AE_ASSERT(mpEventContext->mpObjectContext);
            
			DSSObjectInfo *lpTargetInfo = mpEventContext->mpObjectContext->FindObjectH(lTargetID);
			CHECK_PTR(lpTargetInfo);
            
            DSSFilter *lpFilter = dynamic_cast<DSSFilter *>(lpTargetInfo);
            if(!lpFilter)
            {
                assert(false);
                CHECK_PTR(lpFilter);
            }

			hr = processFilter(lpFilter,  opFilterSliceID);
			if(hr != S_OK) return hr;
            
		}
            break;
        default:
            // wrong type seen
            return hr = E_FAIL;
	}
    
    return hr;
}

//xhan 11/21/05, Evaluate relationship filter
//Suppose the expression tree is
//			Relationship Node {Dimension}
//				|-  Filter Expression
int DSSFilterExprEvalCube::processFilterRelationship(DSSExpNode *ipNode,DSSSliceID *opFilterSliceID)
{
    int hr = S_OK;
    
    Int32 lnChild = ipNode -> Count();
    
	AE_ASSERT(lnChild == 1); //Suppose relationship filter node can only has one child
    
    
	//Evaluate the child node as any other filter node
	DSSExpNode *lpChildNode = ipNode -> Item(0);
	CHECK_PTR(lpChildNode);
    
	DSSSliceID	lFilterSlice;
	hr = processFilterUnit(lpChildNode,  &lFilterSlice);
	CHECK_HR;
    
	//Get dimensionality from Relationship node
	DSSDimty *lpDimty = ipNode->getDimty();
	CHECK_PTR(lpDimty);
    
	Int32 lnUnits = lpDimty->Count();
    
	DSS_ID *lpUnitIDs = new DSS_ID [lnUnits];
	CHECK_NEWED_PTR(lpUnitIDs);
    
    
	for(Int32 iUnit = 0; iUnit < lnUnits; iUnit++)
	{
		DSSDimtyUnit* lpDimUnit = lpDimty->Item(iUnit);
		CHECK_PTR(lpDimUnit);
        
		EnumDSSDimtyUnitType lUnitType = lpDimUnit->getUnitType();
        
		AE_ASSERT(lUnitType == DssDimtyUnitTypeAttribute);
        
        lpUnitIDs[iUnit] = lpDimUnit->getTargetID();
	}
    
	hr = hApplyFiltertoDimty(lnUnits, lpUnitIDs, &lFilterSlice, opFilterSliceID);
	CHECK_HR;
    
	return S_OK;
}

//evalute EQUAL, AND, OR, NOT
int DSSFilterExprEvalCube::evaluateRelationFunction(DSSFunction_Type iFunction,int inChild, DSSSliceID *ipSliceID, DSSSliceID *opSliceID)
{
	AE_ASSERT(iFunction == DssFunctionEquals ||
              iFunction == DssFunctionAnd ||
              iFunction == DssFunctionOr ||
              iFunction == DssFunctionNot);
    
    
	DSSFunctionObject *lpFunctionObject = mpFunctionServer->getFunctionByIndex(iFunction);
	CHECK_PTR(lpFunctionObject);
    MBase::DSSStrongPtr<DSSFunctionObject> lFunctionObjectPtr(lpFunctionObject);
    
    vector<DSSSliceID*> lvSliceID;
    for (int i = 0; i < inChild; i++)
    {
        lvSliceID.push_back(&ipSliceID[i]);
    }
    
    CalculationContext *lCalcContext = DSSCalculationHelper::getCalculationContext(lpFunctionObject,
                                                                                   lpFunctionObject->getOperatorType(),
                                                                                   mpTargetCube,
                                                                                   NULL,
                                                                                   &lvSliceID,
                                                                                   DssNumericErrorSkip,
                                                                                   mpEventContext->mpSchema);
    MBase::DSSStrongPtr<CalculationContext> lCalcContextPtr(lCalcContext);
    
    return DSSCalculationHelper::CalculateNewSlice(lCalcContext, opSliceID);
}

int DSSFilterExprEvalCube::evaluateFilterExpression(DSSExpNode *ipNode, std::vector<FilterExpressionLeaf>&vChild,DSSSliceID *opFilterSliceID)
{
    int hr = S_OK;
    
	//Need to set function property
    DSSExpNodeOperator *lpOperator = dynamic_cast<DSSExpNodeOperator*>(ipNode);
	if(!lpOperator) return hr = E_NOINTERFACE;
    
	DSSFunction_Type lFunction = DssFunctionReserved;
    if(lpOperator->getFunction())
    {
        lFunction = lpOperator -> getFunction()->getFunctionType();
    }else{
        lFunction = (EnumDSSFunction)lpOperator->getFunctionIndex();
    }

	if(lFunction != DssFunctionEquals)
	{
		//not equal, then have to be all slices
		Int32 iSlice, lnSlice = vChild.size();
		DSSSliceID * lpSliceID = new DSSSliceID [lnSlice];
		CHECK_NEWED_PTR(lpSliceID);
        
        
		for(iSlice = 0; iSlice < lnSlice; iSlice ++)
		{
			if(vChild[iSlice].mType != SLICE)
				return hr = E_FAIL;
            
			lpSliceID[iSlice] = vChild[iSlice].mSliceID;
		}
        
		return hr = evaluateExpressionFunction(ipNode, lnSlice, lpSliceID, opFilterSliceID);
	}
	
	return hr = evaluateEqualFunction(vChild, opFilterSliceID);
}

// Output parameter: vLeaf
// The element type of the std::vector is FilterExpressionLeaf and in some cases
// the elements may be stored in MBase::Buffer
// So the buffer should be passed in as a parameter
int DSSFilterExprEvalCube::hRetrieveExpressionLeaf(DSSExpNode * ipNode, std::vector<FilterExpressionLeaf>& vLeaf)
{    
	//here we will retrieve the leaf of the subtree rooted with pNode
	//the leave can be anything from Attribute, Attribute form, multiple entity connected by Tuple operator
    
	//we should not encounter opertor node other than tuple
    
	//the distinction between this expression is there will be no calculation going on
	//the comparison or IN node is specially treated so that (Slice 1, Slice 2) == (Slice 3, Slice 4)
	//will be able to evalute by (Slice == Slice 3) AND (Slice 2 == Slice 4)
    
	//However, no way, we can translate (Slice 1, Slice 2) / (Slice 3, Slice 4).
	//in simple expression, we can recursively process calculation
	//here we only collect leaves and anticipate the calculation will be comparison or IN node, but
	//our leaf types are extended to attribute, elements object
    int hr = S_OK;
    
	if(ipNode == NULL)
		return hr = E_FAIL;
    
	EnumDSSNodeType lType = ipNode->getType();
    
	switch (lType) {
            
        case DssNodeOperator:
		{
			DSSExpNodeOperator *lpOperator = dynamic_cast<DSSExpNodeOperator*>(ipNode);
            if(!lpOperator) return hr = E_NOINTERFACE;
            
            DSSFunction_Type lFunction = DssFunctionReserved;
            if(lpOperator->getFunction())
            {
                lFunction = lpOperator -> getFunction()->getFunctionType();
            }else{
                lFunction =  (EnumDSSFunction)lpOperator->getFunctionIndex();
            }
			if(lFunction != DssFunctionTuple)
				return hr = E_FAIL;
            
			// get the iIndex child node
			Int32 lnChild = ipNode -> Count();
            
			for(int iChild = 0; iChild < lnChild; iChild ++)
			{
				DSSExpNode *lpChild = ipNode -> Item(iChild);
				CHECK_PTR(lpChild);
				
				hr = hRetrieveExpressionLeaf(lpChild, vLeaf);
				if(hr != S_OK) return hr;
                
			}
			break;
		}
        case DssNodeConstant:
        case DssNodeTime:
        case DssNodeBigDecimal:
        case DssNodeCellFormatData:
		{
			FilterExpressionLeaf lLeaf;
			lLeaf.mType = SLICE;
            
            DSSSliceID* lSliceID = &(lLeaf.mSliceID);
            hr = DSSCalculationHelper::GenerateConstantSlice(ipNode, mpTargetCube, lSliceID);
			CHECK_HR;
            
			vLeaf.push_back(lLeaf);
			break;
		}
        case DssNodeShortcut:
		{
            
			//can only process metric
			DSSExpNodeShortcut *lpShortCut = dynamic_cast<DSSExpNodeShortcut *>(ipNode);
            if (!lpShortCut) {
                return E_NOINTERFACE;
            }

            EnumDSSObjectType lTargetType = lpShortCut->getTarget()->getType();
            
			switch(lTargetType)
			{
                case DssTypeMetric:
				{
					FilterExpressionLeaf lLeaf;
					lLeaf.mType = SLICE;
					
                    GUID lMetricID = lpShortCut->getTarget()->getID();
                    
					if(hr != S_OK) return hr;
                    
					hr = evaluateMetric(lMetricID, &(lLeaf.mSliceID));
					CHECK_HR;
                    
					vLeaf.push_back(lLeaf);
					break;
				}
                case DssTypeAttribute:
				{
					FilterExpressionLeaf lLeaf;
					lLeaf.mType = ATTRIBUTE2;
					
					//Attribute need to translate to a table
					DSS_ID lAttributeID = lpShortCut->getTarget()->getID();;
                    
					hr = hGenerateAttributeInput(lAttributeID, lLeaf.mAttributeUnit);
                    
					
					vLeaf.push_back(lLeaf);
					break;
				}
                default:
                    return hr = E_FAIL;
			}
			break;
		}
            
        case DssNodeFormShortcut:
		{
			DSSExpNodeFormShortcut *lpFormShortCut = dynamic_cast<DSSExpNodeFormShortcut*>(ipNode);
			if(lpFormShortCut == NULL) return hr = E_NOINTERFACE;
            
			std::vector<DSSSliceID> lvSliceID;
			//should only be single base form
			hr = hGenerateAttributeFormInputs(lpFormShortCut, lvSliceID);
			if(hr != S_OK) return hr;
            
			Int32 lnSlice = lvSliceID.size(), iSlice;
			for(iSlice = 0; iSlice < lnSlice; iSlice ++)
			{
				FilterExpressionLeaf lLeaf;
				lLeaf.mType = SLICE;
				lLeaf.mSliceID = lvSliceID[iSlice];
                
				vLeaf.push_back(lLeaf);
			}
			break;
		}
        case DssNodeElementsObject:
		{
			//for elements
			//we loop through each element
			//for each element, we will remember the attribute it correspond to and also
			//remember ID forms value
			DSSExpNodeElementsObject *lpElementsObject = dynamic_cast<DSSExpNodeElementsObject*>(ipNode);
			if(!lpElementsObject) return hr = E_NOINTERFACE;
            
			DSSBaseElementsProxy *lpElements = lpElementsObject->getElements();
			CHECK_PTR(lpElements);
			
			Int32 lnElement = lpElements->Count();
			if(hr != S_OK) return hr;
			
			for(int iElement = 0; iElement < lnElement; iElement ++)
			{
                
				FilterExpressionLeaf lLeaf;
				lLeaf.mType = ELEMENT;			
				
				DSSBaseElementProxy *lpElement = lpElements->Item(iElement);
				CHECK_PTR(lpElement);
				hr =  hGenerateElementInputs(lpElement, 
                                            lLeaf.mAttributeUnit,
                                            lLeaf.mpTypes,
                                            lLeaf.mpcbDatas,
                                            lLeaf.mppDatas, 
                                            lLeaf.mpFlags);
				CHECK_HR;
				vLeaf.push_back(lLeaf);
			}
			break;
            
		}
        case DssNodeFieldShortcut:
		{
			//can only process metric
			DSSExpNodeFieldShortcut *lpFGShortCut = dynamic_cast<DSSExpNodeFieldShortcut*>(ipNode);
			if(lpFGShortCut == NULL) return E_NOINTERFACE;
            
			string lNodeKey  = lpFGShortCut->getNodeKey();
			CHECK_HR;
            
			string lFieldKey = lpFGShortCut->getFieldKey();
			CHECK_HR;
            
            if(!mpDataModel)
                return E_FAIL;
            DSSRWIterator* lpIterator = NULL;
            hr = mpDataModel->getCachedIterator(lNodeKey,&lpIterator);
            if(hr != S_OK || !lpIterator)
                return E_FAIL;
            
            DSSRWFieldGroupModelIterator* lpFGIterator = dynamic_cast<DSSRWFieldGroupModelIterator*>(lpIterator);
            CHECK_PTR(lpFGIterator);
            
            int lCount = lpFGIterator->CountFields();
            
            DSSRWFieldGroupNode* lpFGNode = dynamic_cast<DSSRWFieldGroupNode*>(lpIterator->getRWNode());
            if(!lpFGNode)
                return E_FAIL;
            
            DSSFields* lpFields = lpFGNode->getFields();
            assert(lpFields->Count() == lCount);
			Int32 iField = 1;
			for(; iField <= lCount; iField++)
			{
				DSSField* lpField = lpFields->Item(iField);
				CHECK_HR_PTR(lpField);
                
                string lKey = lpField->getKey();
                
                if (lKey.compare(lFieldKey));
                break;
			}
			
            int iRow = lpFGIterator->getRowOrdinal(0);
            if(iRow < 0)
                iRow = 0;
            
            FilterExpressionLeaf lLeaf;
            lLeaf.mType = SLICE;
            
			if(iField <= lCount)
			{
				//now we have the field index, we need to get the raw value
				char *lpData = NULL;
                int lDataSize = 0;
                EnumDSSDataType lDataType = DssDataTypeReserved;
                DSSDataFlag lDataFlag = 0;
				bool lbDataUpdated = false;
				//First try to get the value from the transaction data model
                
                DSSDataModelTransaction* lpTransaction = lpFGIterator->getTransactionDataModel();
				if(lpTransaction)
				{
                    int lColumn = lpFGIterator->getTrxColumnIndexByFieldID(iField);
                    if(lColumn >0)
                    {
                        //TODO: iRow
                        string* lpString = lpTransaction->getUpdatedAndSubmittedData(iRow,iField, lColumn);
                        if(lpString)
                        {
                            lDataType = lpTransaction->getColumnDataType(lColumn);
                            
                            int lBufferSize = 0;
                            hr = DSSAEHelper::getDataSizeFromString(*lpString, lDataType, &lBufferSize);
                            CHECK_HR;
                            
                            lpData = new char[lBufferSize];
                            hr = DSSAEHelper::ConvertStringToData(lpString->c_str(), lDataType, lpData, lBufferSize, lDataSize);
                            CHECK_HR;
                            
                            lbDataUpdated = true;
                        }
                    }
                    
				}
                
				//data not chagned by transaction, get it from iterator
				if(lbDataUpdated == VARIANT_FALSE)
				{
                    const char* lpCChar = NULL;
					hr = lpFGIterator->getRawData(iRow, iField, &lpCChar, &lDataSize, &lDataType, &lDataFlag);
                    CHECK_HR;
                    
                    if(lDataFlag == DssDataNull)
                    {
                        hr = lpFGIterator->getRawDataForInitialRendering(iRow, iField, lDataSize, lDataType, &lpCChar, &lDataFlag);
                        CHECK_HR;
                    }
                    if(lpCChar && lDataFlag == DssDataOk )
                    {
                        lpData = new char[lDataSize];
                        memmove(lpData, lpCChar, lDataSize);
                    }else
                    {
                        lDataType = DssDataTypeVarChar;
                        lDataSize = 0;
                        lpData = new char[1];
                        memset(lpData, 0, 1);
                    }
				}
                
                switch(lDataType){
                    case DssDataTypeInteger:
                    {
                        int lInt = *((int*)lpData);
                        double lValue = lInt;
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lValue),
                                                                    sizeof(double),
                                                                    DssDataTypeDouble,
                                                                    DssDataOk,
                                                                    &lLeaf.mSliceID);
                        CHECK_HR;
                        
                        break;
                    }
                    case DssDataTypeShort:
                    {
                        short lInt = *((short*)lpData);
                        double lValue = lInt;
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lValue),
                                                                    sizeof(double),
                                                                    DssDataTypeDouble,
                                                                    DssDataOk,
                                                                    &lLeaf.mSliceID);
                        CHECK_HR;
                        
                        break;
                    }
                    case DssDataTypeUnsigned:
                    {
                        unsigned int lInt = *((unsigned int*)lpData);
                        double lValue = lInt;
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lValue),
                                                                    sizeof(double),
                                                                    DssDataTypeDouble,
                                                                    DssDataOk,
                                                                    &lLeaf.mSliceID);
                        CHECK_HR;
                        
                        break;
                    }
                    case DssDataTypeLong:
                    {
                        long lInt = *((long*)lpData);
                        double lValue = lInt;
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lValue),
                                                                    sizeof(double),
                                                                    DssDataTypeDouble,
                                                                    DssDataOk,
                                                                    &lLeaf.mSliceID);
                        CHECK_HR;
                        
                        break;
                    }
                    case DssDataTypeBool:
                    {
                        bool lInt = *((bool*)lpData);
                        double lValue = lInt;
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lValue),
                                                                    sizeof(double),
                                                                    DssDataTypeDouble,
                                                                    DssDataOk,
                                                                    &lLeaf.mSliceID);
                        CHECK_HR;
                        
                        break;
                    }
                    case DssDataTypeFloat:
                    case DssDataTypeReal:
                    {
                        // fcamargo 8/23: Changed to use get_Value, constant noew stores data as variant
                        float flt = *((float*)lpData);
                        double lValue = flt;
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lValue),
                                                                    sizeof(double),
                                                                    DssDataTypeDouble,
                                                                    DssDataOk,
                                                                    &lLeaf.mSliceID);
                        CHECK_HR;
                        
                        break;
                    }
                    case DssDataTypeDouble:
                    case DssDataTypeNumeric:
                    case DssDataTypeDecimal:
                    {
                        // fcamargo 8/23: Changed to use get_Value, constant noew stores data as variant
                        double lValue = *((double*)lpData);
                        
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lValue),
                                                                    sizeof(double),
                                                                    DssDataTypeDouble,
                                                                    DssDataOk,
                                                                    &lLeaf.mSliceID);
                        CHECK_HR;
                        
                        break;
                    }
                    case DssDataTypeChar:
                    case DssDataTypeVarChar:
                    case DssDataTypeUTF8Char:
                    case DssDataTypeNChar:
                    case DssDataTypeNVarChar:
                    case DssDataTypeLongVarChar:
                    case DssDataTypeBinary:
                    case DssDataTypeVarBin:
                    case DssDataTypeLongVarBin:
                    case DssDataTypeMBChar:
                    case DssDataTypePattern:
                    case DssDataTypeNPattern:
                    {
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    lpData,
                                                                    lDataSize,
                                                                    DssDataTypeVarChar,
                                                                    DssDataOk,
                                                                    &lLeaf.mSliceID);
						CHECK_HR;
                        
						break;
					}
                    case  DssDataTypeInt64:
                    {
                        long long lValue = *lpData;
                        
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lValue),
                                                                    sizeof(long long),
                                                                    DssDataTypeInt64,
                                                                    DssDataOk,
                                                                    &lLeaf.mSliceID);
                        CHECK_HR;
                        
                        break;
                    }
                    case DssDataTypeTime:
                    case DssDataTypeTimeStamp:
                    case DssDataTypeDate:
                    {
                        //now we should put it in a constant slice
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube, 0, 0,
                                                                    reinterpret_cast<char *>(&lpData), sizeof(MDataType::DateTime),
                                                                    DssDataTypeTimeStamp, DssDataOk, &lLeaf.mSliceID);
                        CHECK_HR;
                        break;
                    }
					case DssDataTypeBigDecimal:
                    {
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(lpData), sizeof(MDataType::DSSBigDecimal),
                                                                    DssDataTypeBigDecimal,
                                                                    DssDataOk, &lLeaf.mSliceID);
                        CHECK_HR;
                        break;
					}
                    case DssDataTypeCellFormatData:
                    {
                        //TODO: treate as double here
                        double lValue = *lpData;
                        
                        hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                                    0, 0,
                                                                    reinterpret_cast<char *>(&lValue),
                                                                    sizeof(double),
                                                                    DssDataTypeDouble,
                                                                    DssDataOk,
                                                                    &lLeaf.mSliceID);
                        CHECK_HR;
                        break;
                    }
                    default:
                        return E_FAIL;
				}
            }
            break;
        }

        default:
            return hr = E_FAIL;
	} // switch (lType)
    
	return hr = S_OK;
}

//Convert an attribute indentified by its GUID to the input id
int DSSFilterExprEvalCube::hGenerateAttributeInput(const DSS_ID &iAttributeID, Int32 &oAttributeID) {
	//lwang, 11/29/2007, TQMS 244359,250668, apply threshold condition on DE
    DSSTabularData* lpTabularData = mpSourceCube->getTabularData();
	int hr = lpTabularData -> FindUnitI(iAttributeID, DssTemplateAttribute, oAttributeID);
	if (hr != S_OK)
	{
		//maybe it's a DE
		return lpTabularData -> FindUnitI(iAttributeID, DssTemplateConsolidation, oAttributeID);
	}
	return S_OK;
}

int DSSFilterExprEvalCube::evaluateEqualFunction(std::vector<FilterExpressionLeaf>&vChild, DSSSliceID *opFilterSliceID)
{
    int hr = S_OK;
    
	if(vChild.size() != 2)
		return hr = E_FAIL;
    
	if(vChild[0].mType != SLICE ||
       vChild[1].mType != SLICE)
	{
		if(vChild[0].mType == ATTRIBUTE2)
			return hr = evaluateElementFilter(vChild[0].mAttributeUnit, vChild[1], opFilterSliceID);
        
		return hr = evaluateElementFilter(vChild[1].mAttributeUnit, vChild[0],  opFilterSliceID);
	}
	
	//both are slice
	DSSSliceID lSliceID[2];
	lSliceID[0] = vChild[0].mSliceID;
	lSliceID[1] = vChild[1].mSliceID;
    
	return hr = evaluateRelationFunction(DssFunctionEquals, 
                                         2,
                                         lSliceID,							
                                         opFilterSliceID);
    
}

//Evaluate attribut element filter
int DSSFilterExprEvalCube::evaluateElementFilter(Int32 iAttributeID, const FilterExpressionLeaf& iElement, DSSSliceID *opFilterSliceID)
{
    
	int hr = S_OK;
    
	AE_ASSERT(iElement.mType == ELEMENT);
	
	//xhan 01/17/05 special handling when data in filter expression is missing
	if ((EnumDSSCellInfoFlags)(*(iElement.mpFlags)) == DssDataMissing )
		return 	 E_FAIL;
    
    
	if(iAttributeID != iElement.mAttributeUnit)
	{
		//the slice will be false
		bool lVal = false;
        return hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                           0, 0,
                                                           reinterpret_cast<char *>(&lVal),
                                                           sizeof(bool),
                                                           DssDataTypeBool,
                                                           DssDataOk,
                                                           opFilterSliceID);
	}
    
	//now they are refering to the same attribute
    DSSTabularData* lpTabularData = mpSourceCube->getTabularData();
	DSSTabularUnit* lpTabularUnit = NULL;
	hr = lpTabularData -> Item(iAttributeID, &lpTabularUnit);
	CHECK_HR_PTR(lpTabularUnit);
    
	DSSTemplateUnit_Type lUnitType = lpTabularUnit->getUnitType();
    
	DSS_ID lAttributeID;
	hr = lpTabularUnit->getGUID(lAttributeID);
	CHECK_HR;
    
	//lwang, 11/29/2007, TQMS 244359,250668, apply threshold condition on DE
	DSSTabularAttribute *lpTabularAttribute;
	if (lUnitType == DssTemplateAttribute)
	{
		lpTabularAttribute = dynamic_cast<DSSTabularAttribute*>(lpTabularUnit);
	}
	else
	{
		//lwang, 05/13/2008, TQMS 315841, temporarily disable threshold condition on DE
		return E_FAIL;
	}
    
	//now need to find the ICDSSAttribute
    /*
	CComPtr<ICDSSObjectInfo> lpInfo;
	hr = mpSource -> FindObject(&lAttributeID,
								DssTypeAttribute,
								EnumDSSSourceFlags(DssSourceBrowser | DssSourceDefn | DssSourceProperties),
								0,
								0,
								0,
								&lpInfo);
	CHECK_HR_PTR(lpInfo);
    
	CComPtr<ICDSSAttribute> lpAttribute;
	hr = lpInfo -> QueryInterface(IID_ICDSSAttribute, (LPVOID *)&lpAttribute);
	CHECK_HR_PTR(lpAttribute);
    
	Int32 lArity;
	hr = lpAttribute -> get_ArityNS(&lArity);
	CHECK_HR;*/
    Int32 lArity = lpTabularAttribute->getArity();
    
    
	Int32 * lpColumn = new Int32[lArity];
	for(Int32 iForm = 0; iForm < lArity; iForm ++)
	{
		//we need to find the correct form and value
		//we assume ID form doesn't have multiple locales.
		hr = lpTabularAttribute->FindFormWithLocale(0, iForm, DssLocaleNeutral, lpColumn[iForm]);
		CHECK_HR;
	}
    
    
	Int32 lPos;
    
	hr = lpTabularAttribute->FindElement(iElement.mpTypes,
                                         iElement.mpcbDatas,
                                         iElement.mppDatas,
                                         iElement.mpFlags,
                                         lPos);
     
	if(hr != S_OK && hr !=S_FALSE) return hr;
    
	if(hr == S_FALSE)
	{
		bool lVal = false;        
        return hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                    1, &iAttributeID,
                                                    reinterpret_cast<char *>(&lVal),
                                                    sizeof(bool),
                                                    DssDataTypeBool,
                                                    DssDataOk,
                                                    opFilterSliceID);
	}
	
    
	bool lVal = false;    
	hr = DSSCalculationHelper::GetConstantSlice(mpTargetCube,
                                                1, &iAttributeID,
                                                reinterpret_cast<char *>(&lVal),
                                                sizeof(bool),
                                                DssDataTypeBool,
                                                DssDataOk,
                                                opFilterSliceID);

	if(hr != S_OK) return hr;
    
	DSSCubeDataSlice* lpSlice;
	hr = opFilterSliceID->GetSlice(&lpSlice);
	CHECK_HR_PTR(lpSlice);
    
	DSSTabularIndex* lpIndices = lpSlice ->getIndices();
	CHECK_PTR(lpIndices);
    
	Int32 * lpIndex = new Int32[iAttributeID + 1];
	CHECK_NEWED_PTR(lpIndex);
    
	DSSDataColumn* lpColumn2 = lpSlice->getColumn();
	CHECK_PTR(lpColumn2);
    
    lpIndex[iAttributeID] = lPos;
        
    hr = lpIndices -> LookUp(iAttributeID + 1,
                            lpIndex,
                            &lPos);
    // rzhu, 11/07/2011, in new rwd, lpLookUpTable is global lookup table 
    // and it contains complete elements while lpIndices contains only necessary
    // rows. So we may not be able to find the element from lpIndices, but it sill
    // a valid case.
    if (hr == S_FALSE) return S_OK;
    if(hr != S_OK) return hr;
    //need to set lPos to be 1
    lVal = true;
    return hr = lpColumn2 -> setData(lPos, 
                                    reinterpret_cast<char *>(&lVal), 
                                    sizeof(bool), 
                                    DssDataOk);    
}

int DSSFilterExprEvalCube::hApplyFiltertoDimty(Int32 nDimUnits, DSS_ID *ipDimUnits, DSSSliceID *ipFilterSliceID, DSSSliceID *opResultSliceID)
{
	HRESULT hr = S_OK;
    
	DSSCubeDataSlice* lpFilterSlice = NULL;
    hr = ipFilterSliceID->GetSlice(&lpFilterSlice);
	CHECK_HR_PTR(lpFilterSlice);
    
	Int32 *lpUnitIDs = new Int32 [nDimUnits];
	CHECK_NEWED_PTR(lpUnitIDs);
    
	for (Int32 i = 0; i < nDimUnits; i ++)
	{
		hr = mpTargetCube->getTabularData()->FindUnitI(ipDimUnits[i], DssTemplateAttribute, lpUnitIDs[i]);
		CHECK_HR;
	}
    
	//sort the attribute units
	std::sort(lpUnitIDs, lpUnitIDs + nDimUnits);
    
	hr = DSSCalculationHelper::ApplyRelationshipFilter(nDimUnits, lpUnitIDs, lpFilterSlice, mpTargetCube, opResultSliceID);
                                                       //, mpBuffer);
    
	CHECK_HR;
    
	return S_OK;
}

int DSSFilterExprEvalCube::hGenerateAttributeFormInputs(DSSExpNodeFormShortcut * ipAttrFormShorcut,
                                                       std::vector<DSSSliceID> &oAttributeFormIDs)
{
    DSSSliceID attributeFormID;
    
	int hr = DSSCalculationHelper::GenerateAttributeFormInputs(mpTargetCube, ipAttrFormShorcut, &attributeFormID);
	if (hr != S_OK)
		return E_FAIL;
    
    attributeFormID.mpCube = mpTargetCube;
    oAttributeFormIDs.push_back(attributeFormID);
    
	return hr;
}

//Convert extract information from attribute element object, including attribute id
//element data type, element data length and element value.
int DSSFilterExprEvalCube::hGenerateElementInputs(DSSBaseElementProxy *ipElement,
                                                  Int32 &oAttributeID,
                                                  EnumDSSDataType * &opTypes,
                                                  Int32 * &opcbDatas,
                                                  unsigned char ** &oppDatas,
                                                  DSSDataFlag * &opFlags)
{
    int hr = S_OK;
        /*
	CComPtr<ICDSSAttribute> lpAttribute;
	HRESULT hr = ipElement -> get_Attribute(&lpAttribute);
	CHECK_HR_PTR(lpAttribute);
    
	Int32 lArity = 0;   //hard code
	hr = lpAttribute -> get_ArityNS(&lArity);
	CHECK_HR;
    */
    
    DSSAttributeElementProxy* lpAttributeElement = dynamic_cast<DSSAttributeElementProxy*>(ipElement);
    CHECK_PTR(lpAttributeElement);
    Int32 lArity = lpAttributeElement->getArity();
    
	DSS_ID lAttributeID = ipElement -> getObjectID();
    
	DSSTabularAttribute* lpTabularAttribute;
	DSSGlobalLookUpTable* lpGlobalLookupTable;
    
    DSSTabularData* lpTabularData = mpSourceCube->getTabularData();
    
	hr = lpTabularData -> FindUnitI(lAttributeID, DssTemplateAttribute, oAttributeID);
    //	CHECK_HR;
	if (hr == S_OK)
	{
		DSSTabularUnit* lpTabularUnit;
		hr = lpTabularData -> Item(oAttributeID, &lpTabularUnit);
		CHECK_HR_PTR(lpTabularUnit);
        
		lpTabularAttribute = dynamic_cast<DSSTabularAttribute*>(lpTabularUnit);
        
		lpGlobalLookupTable = dynamic_cast<DSSGlobalLookUpTable*>(lpTabularAttribute);
	}
	else
	{
		//lwang, 11/29/2007, TQMS 244359,250668, apply threshold condition on DE
		//maybe it's a DE
        /*
		hr = mpTabularData -> FindUnitI(lAttributeID, DssTemplateConsolidation, &oAttributeID);
		CHECK_HR;
        
		CComPtr<ICDSSTabularDataUnit> lpTabularDataUnit;
		hr = mpTabularData -> Item(oAttributeID, &lpTabularDataUnit);
		CHECK_HR_PTR(lpTabularDataUnit);
        
		CComQIPtr<ICDSSXTabConsolidation, &IID_ICDSSXTabConsolidation> lpXTabConsolidaiton(lpTabularDataUnit);
		CHECK_PTR(lpXTabConsolidaiton);
        
		Int32 lChildUnit = -1;
		hr = lpXTabConsolidaiton->get_Units(1, &lChildUnit);
		CHECK_HR;
        
		CComPtr<ICDSSTabularDataUnit> lpChildUnit;
		hr = mpTabularData -> Item(lChildUnit, &lpChildUnit);
		CHECK_HR_PTR(lpChildUnit);
        
		CComQIPtr<ICDSSXTabCustomGroup, &IID_ICDSSXTabCustomGroup> lpTabCustomGroup(lpChildUnit);
		CHECK_PTR(lpTabCustomGroup);
        
		hr = lpTabCustomGroup->get_LookUpTable(0, &lpLookUpTable);
		CHECK_HR_PTR(lpLookUpTable);
        
		CComQIPtr<ICDSSTabularDataUnit, &IID_ICDSSTabularDataUnit> lpTabularDataUnit2(lpLookUpTable);
		CHECK_PTR(lpTabularDataUnit2);
        
		hr = lpTabularDataUnit2 -> get_Table(&lpTable);
		CHECK_HR_PTR(lpTable);
         */
	}
    
    
	opTypes = new EnumDSSDataType[lArity];
	CHECK_NEWED_PTR(opTypes);
	opcbDatas = new Int32[lArity];
	CHECK_NEWED_PTR(opcbDatas);
	oppDatas = new unsigned char *[lArity];
	CHECK_NEWED_PTR(oppDatas);
	opFlags = new DSSDataFlag[lArity];
	CHECK_NEWED_PTR(opFlags);
    
    
	Int32 iForm, iColumn;
	for(iForm = 0; iForm < lArity; iForm ++)
	{
        /*
		CComVariant lVar;
		hr = ipElement -> get_Value(iForm, &lVar);
		CHECK_HR;*/
        string lValue = lpAttributeElement->getValue(iForm);
		//we need to find the correct form and value
		//We assume ID forms don't have locales
		hr = lpTabularAttribute -> FindFormWithLocale(0, iForm, DssLocaleNeutral, iColumn);
		CHECK_HR;
        
		DSSDataColumn* lpColumn = NULL;
        hr = lpTabularAttribute->Item(iColumn, &lpColumn);
		CHECK_HR_PTR(lpColumn);
        
		opTypes[iForm] = lpColumn -> getDataType();
        
        /*
		hr = gConvertVariant(mpBuffer, 
                             &lVar, 
                             opTypes[iForm], 
                             &oppDatas[iForm],
                             &opcbDatas[iForm],
                             &opFlags[iForm]);*/
        if(opTypes[iForm] == DssDataTypeBigDecimal)
        {
            MDataType::DSSBigDecimal* lpBigDecimal = new MDataType::DSSBigDecimal(lValue.c_str());
            opcbDatas[iForm] = sizeof(MDataType::DSSBigDecimal);
            oppDatas[iForm] = (unsigned char*)lpBigDecimal;
        }else{
            oppDatas[iForm] = new unsigned char[lpColumn->getSizeOfData() + 1];
            hr = DSSAEHelper::ConvertStringToData(lValue.c_str(), opTypes[iForm], oppDatas[iForm], 1024, opcbDatas[iForm]);
            opFlags[iForm] = hr==S_OK ? DssDataOk : ~DssDataOk;
        }
        CHECK_HR;
	} //for iForm
    
	return S_OK;
    
}

int DSSFilterExprEvalCube::SetDocumentInstance(DSSDocumentInstance* ipInstance)
{
    mpDocInstance = ipInstance;
    return S_OK;
}

int DSSFilterExprEvalCube::SetDocumentInstance(DSSRWDataModel* ipModel)
{
    mpDataModel = ipModel;
    return S_OK;
}

int DSSFilterExprEvalCube::SetCurrentRow(int iRow)
{
    mCurrentRow = iRow;
    return S_OK;
}


int DSSFilterExprEvalCube::GenerateConstantFromGridRow(DSSExpNodeGridUnitShortcut* ipGridNode,
                                                       DSSRWGridIterator* ipGridIterator,
                                                       Int32 iRow,
                                                       CComVariant* opValue)
{
	int hr = S_OK;
    int lRow = iRow;
	hr = ipGridIterator->getOrdinalFromDisplay(0,iRow,lRow);
    CHECK_HR;
    
	DSS_ID lTargetUnitID = ipGridNode->getTargetID();
    
    DSSTabularData* lpTabularData = ipGridIterator->getTabularData();
    CHECK_PTR(lpTabularData);
    
    DSSXTabView* lpView = ipGridIterator->getXTabView();
    CHECK_PTR(lpView);
    
    DSSCube* lpCube = NULL;
    hr = lpView->GetCube(&lpCube);
    CHECK_HR_PTR(lpCube);
    
    // get element data
    unsigned char * lpData = NULL;
    DSSDataFlag lDataFlag = DssDataOk;
//    int lThresholdID = 0;
    int lDataSize = 0;
    EnumDSSDataType lDataType;
    
	int lUnitIndex = -1;
	EnumDSSTemplateUnitType lUnitType = DssTemplateReserved;
	hr = lpTabularData->FindUnitI(lTargetUnitID, lUnitType, &lUnitIndex);
	if(hr == S_OK)
	{
		DSSTabularUnit* lpTabularDataUnit = NULL;
		hr = lpTabularData->Item(lUnitIndex, &lpTabularDataUnit);
		CHECK_HR_PTR(lpTabularDataUnit);
        
		hr = lpTabularDataUnit->get_UnitType(&lUnitType);
		CHECK_HR;
        
		//attribute form
		if(lUnitType == DssTemplateAttribute)
		{
            int lForm = 0;
            //should only be single base form
			//TODO: get the attribute;
			DSS_ID lFormID = ipGridNode->getFormID();
            
            DSSObjectContext* lpObjectContext = ipGridIterator->getObjectContext();
            CHECK_PTR(lpObjectContext);
            
            DSSObjectInfo* lpInfo = lpObjectContext->FindObjectH(lTargetUnitID);
            DSSAttribute* lpAttribute = dynamic_cast<DSSAttribute*>(lpInfo);
            CHECK_PTR(lpAttribute)
            
            if(lpAttribute)
            {
                DSSAttributeFormWrappers* lpWrappers = NULL;
                lpWrappers = lpAttribute->getFormWrappers();
                if(lpWrappers)
                {
                    int lnForms = lpWrappers->Count();
                    for(int i = 0; i < lnForms; i++)
                    {
                        DSSAttributeFormWrapper* lpWrapper = lpWrappers->Item(i);
                        if(lpWrapper && lpWrapper->getID() == lFormID)
                        {
                            lForm = lpWrapper->getBaseFormID();
                            break;
                        }
                    }
                }
            }
            
            DSSTabularAttribute* lpTabularAttribute = static_cast<DSSTabularAttribute*>(lpTabularDataUnit);
            CHECK_PTR(lpTabularAttribute);
            
           // int lColumnIndex = -1;
			//hr = lpTabularAttribute->FindFormWithLocale(0, lForm, 0, &lColumnIndex);
			//CHECK_HR;
            
            int lnUnits = ipGridIterator->getNumUnits(0);
            for(int i = 0; i < lnUnits; i++ )
            {
                DSS_ID lUnitID = ipGridIterator->getUnitID(0, i);
                if(lUnitID == lTargetUnitID)
                {
                    //use iRow instead of lRow
                    hr = ipGridIterator->getElementValueAt(0, iRow, i, lForm, (const void**)&lpData, &lDataSize, &lDataType, &lDataFlag);
                    CHECK_HR;
                    break;
                }
            }
		}
	}
	//metric
	else if(hr == S_FALSE)
	{
        hr = S_OK;
        
		DSSTabularMetrics* lpMetrics = lpTabularData->getTabularMetircs();
		CHECK_PTR(lpMetrics);
        
		int lMetricIndex = lpMetrics->FindDisplayedMetric(lTargetUnitID);
        
		int lnRow = lpView->getRowCount();
        
		if(lnRow <= 0)
			return E_FAIL;
        
		int lnCol = lpView->getColCount();
        
		int lCol = 0;
		for( lCol = 0; lCol < lnCol; lCol++)
		{
			int lMetric = lpView->getMetricID(0,lCol);
            
			if(lMetric == lMetricIndex)
				break;
		}
        
		if(lCol >= lnCol)
			return E_FAIL;
        
		hr = lpView->GetRawCellInfo2(lRow, lCol, (const char**)&lpData, lDataSize, lDataType, lDataFlag);
        CHECK_HR;
	}
	else
		return E_INVALIDARG;
    
    if(hr == S_OK && lDataFlag == DssDataOk)
    {
        hr = DFCDataConversion::ConvertToVariant(
                                                 lpData,
                                                 lDataType,
                                                 lDataSize,
                                                 opValue);
    }
    else
    {
        if(hr != S_OK)
            return hr;
        
        //so everything else we will convert to VARIANT
        opValue->vt = VT_EMPTY;
    }
    CHECK_HR;
    
	return hr;
}













