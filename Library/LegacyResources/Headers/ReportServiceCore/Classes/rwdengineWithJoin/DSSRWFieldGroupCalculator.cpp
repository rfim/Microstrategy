//
//  DSSRWFieldGroupCalculator.cpp
//  ReportServiceCore
//
//  Created by Jia Zhang on 9/19/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#import "DSSRWFieldGroupCalculator.h"
#import "DSSRWFieldGroupModelIterator.h"
#import "DSSField.h"
#import "DSSTabularAttribute.h"
//#import "DSSDataRefColumn.h"
#import "DSSStrongPtr.h"

#import "DSSCExpression.h"      
#import "DSSFunctionServer.h"
#import "DSSExpNode.h"
#import "DSSExpNodeOperator.h"
#import "DSSExpNodeElementsObject.h"
#import "DSSFunctionObject.h"
#import "DSSExpNodeConstant.h"
#import "DSSExpNodeShortcut.h"
#import "DSSExpNodeFormShortcut.h"
#import "DSSExpNodeTime.h"
#import "DSSSimpleFunction.h"
#import "DSSNNSimpleFunction.h"
#import "DSSGenericFunction.h"
#import "DSSNNGenericFunction.h"
#import "DFCDataConversion.h"
#import "DSSDateTime.h"
#import "MsiCOM/MsiCOM/Msi_atlbase.h"
#import "DSSComparisonOperatorFunc.h"
#import "DSSStringFunc.h"

class DSSRWFieldGroupModelIterator;

int DSSRWFieldGroupCalculator::hCheckBigDecimal (CComVariant * opValue, DSSDataFlag * opValueFlag)
{
	int hr = S_OK;
    
	double lTemp = 0;
    
	if ( opValue->vt == VT_DISPATCH && (S_OK == hBigDecimalToDouble(opValue, lTemp)) )
	{
		hr = opValue->Clear();
		VariantInit(opValue);
		opValue->vt = VT_R8;
		if(FAILED(hr))
		{
			opValue->vt = VT_EMPTY;
			*opValueFlag = DssDataRWInvalid;
			return S_FALSE;
		}
		opValue->dblVal = lTemp;
	}
    
	return S_OK;
}

// Feb/09/2012, todo, implememt below logic as server side
int DSSRWFieldGroupCalculator::hBigDecimalToDouble(CComVariant *ipVar, double &orDouble)
{
    return E_NOTIMPL;
}

int DSSRWFieldGroupCalculator::Calculate (DSSRWFieldGroupIterator *ipIterator, DSSExpNode *ipNode, DSSField *ipField, DSSFunctionServer *ipFunctionServer, AERawData *opRawData)
{
    if (!ipNode) 
        return E_POINTER;
    
    HRESULT hr = S_OK;
    
    opRawData->mValue = NULL;
    opRawData->mSizeInByte = 0;
    opRawData->mDataType = DssDataTypeReserved;
    opRawData->mFlag = DssDataOk;
    
    DSSRWFieldGroupModelIterator *lpNewRWDIterator = dynamic_cast<DSSRWFieldGroupModelIterator *>(ipIterator);
    
    EnumDSSNodeType lNodeType = ipNode->getType();
    switch (lNodeType)
    {
        case DssNodeConstant:
        {
            DSSExpNodeConstant *lpConstant = dynamic_cast<DSSExpNodeConstant*>(ipNode);
            CHECK_PTR(lpConstant);
            
            CComVariant *lpConsVal = lpConstant->getValue();
            if (VT_BSTR == lpConsVal->vt)
            {
                // jimzhang Mar/15/2012 At current, constant string serialized from server side is encoded as wide char. 
                //      As current string conversion on iPad can't support UTF8 and wide char at the same time, convert wide char to UTF8 string here.
                //      Below convertion logic is just for temporary use, as all strings serialized from server may be changed to UTF8 in the future.
                
                char *lpUTF8 = NULL;
                int lLen = 0;
                
                const MBase::ReturnString lData(MBase::WideCharToUTF8(lpConsVal->bstrVal));
                lLen = lData.GetLength() + sizeof(char);
                lpUTF8 = new char[lLen];
                assert(lpUTF8);
                
                if (lData.c_str() )
                    memmove(lpUTF8, lData.c_str(), lLen);
                else
                    ::memset((void *)lpUTF8, 0, lLen);
                
                //yimliu, 2014/06/20, 883809 related, memory leak.
                /*
                opRawData->mValue = lpUTF8;
                opRawData->mSizeInByte = lLen;
                opRawData->mDataType = DssDataTypeVarChar;
                 */
                AERawData lRawData(lpUTF8, lLen, DssDataTypeVarChar, DssDataOk, true);
                opRawData->Assign(lRawData);
                
            }
            else
            {
                //yimliu, 2014/06/20, 883809 related, memory leak.
                /*
                hr = DSSAEHelper::VariantToRawData(*lpConsVal, opRawData->mValue, opRawData->mSizeInByte, opRawData->mDataType);
                */
                AERawData lRawData(NULL, opRawData->mSizeInByte, opRawData->mDataType, opRawData->mFlag, true);
                hr = DSSAEHelper::VariantToRawData(*lpConsVal, lRawData.mValue, lRawData.mSizeInByte, lRawData.mDataType);
                opRawData->Assign(lRawData);
            }
            
            if (S_OK != hr)
            {   
                opRawData->mFlag = DssDataRWInvalid;
                return hr; 
            }
        }
            break;
            
        case DssNodeTime:
        {
            DSSExpNodeTime *lpTime = dynamic_cast<DSSExpNodeTime *>(ipNode);
            CHECK_PTR(lpTime);
            
            //yimliu, 2014/06/20, 883809 related, memory leak.
            /*
            hr = DSSAEHelper::VariantToRawData(*lpTime->getRawValue(), opRawData->mValue, opRawData->mSizeInByte, opRawData->mDataType);
            */
            AERawData lRawData(NULL, opRawData->mSizeInByte, opRawData->mDataType, opRawData->mFlag, true);
            hr = DSSAEHelper::VariantToRawData(*lpTime->getRawValue(), lRawData.mValue, lRawData.mSizeInByte, lRawData.mDataType);
            opRawData->Assign(lRawData);
            
            if (S_OK != hr)
            {
                opRawData->mFlag = DssDataRWInvalid;
                return hr;
            }
        }
            break;
            
        case DssNodeMe:
        {
            if (! ipField)
                break;
            
            EnumDSSRWFieldType lType = ipField->getType();
            if (DssRWFieldConcatenation == lType)
            {
                DSSFields* lpChildren = ipField->getChildFields();
                CHECK_PTR(lpChildren);
                
                int lnChildren = lpChildren->Count();
                if (1 == lnChildren)
                {
                    DSSField* lpChild = lpChildren->Item(0);
                    CHECK_PTR(lpChild);
                    
                    lType = lpChild->getType();
                    if (DssRWFieldCalculation == lType)
                    {
                        DSSCExpression *lpExp = lpChild->getExpression();
                        CHECK_PTR(lpExp);
                        
                        DSSExpNode *lpExpNode = lpExp->Root();
                        CHECK_PTR(lpExpNode);
                        return Calculate(ipIterator, lpExpNode, lpChild, ipFunctionServer, opRawData);
                    }
                    else
                    {
                        GUID lObjID = GUID_NULL;
                        EnumDSSObjectType lObjType = DssTypeReserved;
                        
                        DSSObjectInfo* lpObjInfo = lpChild->getFieldObject();
                        if (lpObjInfo)
                        {
                            lObjID = lpObjInfo->getID();
                            lObjType = lpObjInfo->getType();
                        }
                        
                        if (DssTypeMetric == lObjType)
                        {
                            GUID lSourceGUID = lpChild->getDataSourceObjectID();
                            std::string lKey = lpChild->getKey();
                            
                            if (lpNewRWDIterator)
                                hr = lpNewRWDIterator->getCurrentMetricValueByGUID(&lSourceGUID, &lObjID, &lKey, opRawData);
                            else
                                hr = ipIterator->getCurrentMetricValueByGUID(&lSourceGUID, &lObjID, &lKey, opRawData);
                            
                            if (S_OK != hr)
                            {                                
                                opRawData->mFlag = DssDataInvalid;
                                return E_FAIL;
                            }
                            
                            if (DssDataOk != opRawData->mFlag)
                            {   
                                //lwang, 380904, set variant data type when it's NULL
                                if (opRawData->mDataType == DssDataTypeTime || opRawData->mDataType == DssDataTypeDate 
                                    || opRawData->mDataType == DssDataTypeTimeStamp || opRawData->mDataType == DssDataTypeBigDecimal
                                    || opRawData->mDataType == DssDataTypeCellFormatData || opRawData->mDataType == DssDataTypeMBChar
                                    || opRawData->mDataType == DssDataTypeUTF8Char || opRawData->mDataType == DssDataTypeVarChar
                                    || opRawData->mDataType == DssDataTypeNVarChar)
                                {                                    
                                    opRawData->mDataType = DssDataTypeReserved;
                                }
                                else
                                {
                                    char *lpDouble = (char *) new double(.0);
                                    CHECK_NEWED_PTR(lpDouble);
                                    
                                    AERawData lRawData(lpDouble, sizeof (double), DssDataTypeDouble, opRawData->mFlag, true);
                                    opRawData->Assign(lRawData);
                                }
                            }
                            
                            return hr;
                        }
                        // weilin 2016/12/21 DE51631 deal with attribute
                        else if (DssTypeAttribute == lObjType) {
                            DSSAttribute *lpAttr = dynamic_cast<DSSAttribute *>(lpObjInfo);
                            if (lpAttr && DssRWFieldObjectValue == lType)
                            {
                                DSSAttributeFormWrappers *lpChildForms = lpChild->getAttributeForms();
                                if (lpChildForms)
                                {
                                    GUID lFormGUID = GUID_NULL;

                                    // get form GUID
                                    if (lpChildForms->Count() == 0) // if we don't define form on text, such as {Category}
                                    {
                                        lFormGUID = ipIterator->getAttrFieldFormID(lpChild, lpAttr, lObjID);
                                    }
                                    else // if we define form on text, such as {Category@DESC}
                                    {
                                        lFormGUID = lpChildForms->Item(0)->getID();
                                    }

                                    // get Data
                                    if (lFormGUID != GUID_NULL)
                                    {
                                        if (lpNewRWDIterator)
                                            hr = lpNewRWDIterator->getCurrentFormValueByID(lpAttr, &lFormGUID, opRawData);
                                        else
                                            hr = ipIterator->getCurrentFormValueByID(lpAttr, &lFormGUID, opRawData);

                                        if (hr == S_OK)
                                        {
                                            return hr;
                                        }
                                    }
                                }
                            }
                        }
                        AERawData lRawData(NULL, 0, DssDataTypeVarChar, DssDataOk, true);
                            
                        std::string lText = ipField->getText();
                        int lStrLen = lText.size() + 1;
                        lRawData.mSizeInByte = lStrLen;
                        lRawData.mValue = new char[lStrLen];
                        memmove(lRawData.mValue, lText.c_str(), lStrLen);
                        opRawData->Assign(lRawData);
                        
                        return S_OK;
                    }
                } // END: if (1 == lnChildren)
            } // END: if (DssRWFieldConcatenation == lType)
            
            opRawData->mFlag = DssDataRWInvalid;
			return E_FAIL;
        } // END: case DssNodeMe
            break;
            
        case DssNodeShortcut:
        {
            DSSExpNodeShortcut *lpShortCut = dynamic_cast<DSSExpNodeShortcut*>(ipNode);
            CHECK_PTR(lpShortCut);
            
			DSSObjectInfo *lpTarget = lpShortCut->getTarget();
            CHECK_PTR(lpTarget);
            
            EnumDSSObjectType lTargetType = lpTarget->getType();
            if (DssTypeMetric == lTargetType)
            {
                GUID lSourceGUID = lpShortCut->getDataSourceObjectID();
                GUID lMetricGUID = lpTarget->getID();
                
                if (lpNewRWDIterator)
                    hr = lpNewRWDIterator->getCurrentMetricValueByGUID(&lSourceGUID, &lMetricGUID, NULL, opRawData);
                else
                    hr = ipIterator->getCurrentMetricValueByGUID(&lSourceGUID, &lMetricGUID, NULL, opRawData);
                
                if (S_OK != hr)
                {                                
                    opRawData->mFlag = DssDataInvalid;
                    return E_FAIL;
                }
                else if (DssDataOk != opRawData->mFlag)
                {   
                    //lwang, 380904, set variant data type when it's NULL
                    if (opRawData->mDataType == DssDataTypeTime || opRawData->mDataType == DssDataTypeDate 
                        || opRawData->mDataType == DssDataTypeTimeStamp || opRawData->mDataType == DssDataTypeBigDecimal
                        || opRawData->mDataType == DssDataTypeCellFormatData || opRawData->mDataType == DssDataTypeMBChar
                        || opRawData->mDataType == DssDataTypeUTF8Char || opRawData->mDataType == DssDataTypeVarChar
                        || opRawData->mDataType == DssDataTypeNVarChar)
                    {                                    
                        opRawData->mDataType = DssDataTypeReserved;
                    }
                    else
                    {
                        char *lpDouble = (char *) new double(.0);
                        CHECK_NEWED_PTR(lpDouble);

                        AERawData lRawData(lpDouble, sizeof (double), DssDataTypeDouble, opRawData->mFlag, true);
                        opRawData->Assign(lRawData);
                    }
                    
                    return S_OK;
                }
            }
            else
            {
                opRawData->mFlag = DssDataRWInvalid;
                return E_UNEXPECTED;
            }
        } // END: case DssNodeShortcut
            break;
            
        case DssNodeFormShortcut:
        {
            // 1. Get attribue and base form GUID of this attribute form
            DSSExpNodeFormShortcut *lpFormShortcut = dynamic_cast<DSSExpNodeFormShortcut *>(ipNode);
            CHECK_PTR(lpFormShortcut);            
            DSSAttribute *lpAttr = lpFormShortcut->getAttribute();
            CHECK_PTR(lpAttr);
            
            DSSAttributeForm *lpAttrForm = lpFormShortcut->getForm();
            CHECK_PTR(lpAttrForm);
            GUID lFormGUID = lpAttrForm->getID();
            
            // 2, get form data via its base from ID and attribute GUID            
            if (lpNewRWDIterator)
                hr = lpNewRWDIterator->getCurrentFormValueByID(lpAttr, &lFormGUID, opRawData);
            else
                hr = ipIterator->getCurrentFormValueByID(lpAttr, &lFormGUID, opRawData);
            
            if (S_OK != hr)
            {
                opRawData->mFlag = DssDataInvalid;
                return S_OK;
            }
        } // END: case DssNodeFormShortcut
            break;
            
        case DssNodeOperator:
        {
            DSSExpNodeOperator *lpOperator = dynamic_cast<DSSExpNodeOperator*>(ipNode);
            CHECK_PTR(lpOperator);
            
            int lFunctionIndex = NULL != lpNewRWDIterator ? lpOperator->getFunctionIndex() : lpOperator->getOperatorType();
            DSSFunctionObject *lpFunctionObject = NULL;
            
            //            map<DSSExpNodeOperator *, DSSFunctionObject *>::iterator iter = mOperator2FunObj.find(lpOperator);
            //            if (iter != mOperator2FunObj.end())
            //            {
            //                lpFunctionObject = iter->second;
            //            }
            //            else
            //            {
            //
            
            // Sep/19/2012, todo, cache function object in/out of DSSRWFieldGroupCalculator 
            GUID lPackageGUID = lpOperator->getPackageID();
            lpFunctionObject = ipFunctionServer->FindFunctionObject(lFunctionIndex, &lPackageGUID);
            CHECK_PTR(lpFunctionObject);            
            
            //mOperator2FunObj[lpOperator] = lpFunctionObject;
            
            std::vector<CComVariant>  lvProperty = lpOperator->getFunctionPropertyVec();
            int lnProperty = lvProperty.size();
            for (int i = DssFunctionPropertySystemCount; i < lnProperty; ++i)
            {
                hr = lpFunctionObject->setProperty(i + 1 - DssFunctionPropertySystemCount, &lvProperty[i]);
                
                //zhyang, 883776, some funtion's set property is not implmented
                if(hr != S_OK && hr != E_NOTIMPL)
                {
                    //yimliu, 2014/06/20, 883809 related, memory leak.
                    if (lpFunctionObject != NULL)
                    {
                        delete lpFunctionObject;
                        lpFunctionObject = NULL;
                    }
                    return hr;
                }
                hr = S_OK;
            }
            
            VARTYPE lInputDataType = VT_EMPTY;
            
            EnumDSSFunctionType lFunctionType = lpFunctionObject->getFunctionType();
            if (DssFunctionTypeSimple == lFunctionType)
            {
                bool lfStringFunc = false;
                
                DSSSimpleFunction *lpSimpleFunction = dynamic_cast<DSSSimpleFunction*> (lpFunctionObject);
                DSSNNSimpleFunction *lpNNSimpleFunction = dynamic_cast<DSSNNSimpleFunction*> (lpFunctionObject);
                if (!lpSimpleFunction && !lpNNSimpleFunction)
                {
                    opRawData->mFlag = DssDataInvalid;
                    //yimliu, 2014/06/20, 883809 related, memory leak.
                    if (lpFunctionObject != NULL)
                    {
                        delete lpFunctionObject;
                        lpFunctionObject = NULL;
                    }
                    return E_FAIL;
                }
                
                // 12/18/2012 tehe #697869: if function contain string compare option we need set it before we call the function
                EnumSimpleOperatorType lOpType = DssFuncUnknown;
                lOpType = lpSimpleFunction->getOperatorType();
                lfStringFunc = DSSAEHelper::IsStringFunction(lOpType);
                
                bool lbStringOption = lfStringFunc ? true : DSSAEHelper::ContainsStringCmpOption(lOpType);
                
                if(lpNNSimpleFunction && lbStringOption && lpNewRWDIterator)
                {
                    bool lfTrimString = lpNewRWDIterator->getTrimString();
                    DSSStringFunc *lpStringFunc = dynamic_cast<DSSStringFunc *>(lpNNSimpleFunction);
                    if (lpStringFunc)
                        lpStringFunc->SetStringCompareOption(lfTrimString ? VARIANT_TRUE : VARIANT_FALSE);
                    
                    DSSComparisonOperatorFunc *lpCmpOperatorFunc = dynamic_cast<DSSComparisonOperatorFunc *>(lpNNSimpleFunction);
                    if (lpCmpOperatorFunc)
                        lpCmpOperatorFunc->SetStringCompareOption(lfTrimString ? VARIANT_TRUE : VARIANT_FALSE);

                }
                                  
                
                bool lfAllNumeric = true;
                
				// 10/31/2012 tehe: check package type
				if (lPackageGUID == GUID_DSSBuiltInPackage && (DssFuncIn == lFunctionIndex || DssFuncNotIn == lFunctionIndex))
                {
                    DSSExpNodeOperator *lpOperator = dynamic_cast<DSSExpNodeOperator*>(ipNode);
                    CHECK_PTR(lpOperator);
                    
                    // TQMS 687030, if expression type of operator is DssFilterSingleMetricQual, use DSSIn to calculate
                    if (DssFilterSingleMetricQual != lpOperator->getExpressionType())
                    {
                        //yimliu, 2014/06/20, 883809 related, memory leak.
                        /*
                        return hCalculateFunctionIn(ipIterator, ipNode, ipField, ipFunctionServer, opRawData);
                         */
                        hr = hCalculateFunctionIn(ipIterator, ipNode, ipField, ipFunctionServer, opRawData);
                        if (lpFunctionObject != NULL)
                        {
                            delete lpFunctionObject;
                            lpFunctionObject = NULL;
                        }
                        return hr;
                    }
                }
                
                int lnChildCount = lpOperator->Count();
                //assert(lnChildCount > 0); // some functions have no parameter/input, e.g. CurrentDate()
                
				SAFEARRAYBOUND lBound;
				lBound.lLbound = 0;
                // tehe: string function related
				lBound.cElements = (lfStringFunc ? (2 *lnChildCount) : lnChildCount);
                
				CComVariant lParam;
				VariantInit(&lParam);
				
				lParam.vt = VT_ARRAY|VT_VARIANT;
				lParam.parray = SafeArrayCreate(VT_VARIANT, 1, &lBound);
                
                // jimzhang Feb/08/2012 ToDo Implement below logic of server
                /*BOOL bCellFormatData = false;
                 BOOL bCellFormatFunction = false;
                 
                 CComQIPtr<ICDSSCellFormatFunction, &IID_ICDSSCellFormatFunction> lpCellFunc(pSimpleFunc);
                 if(lpCellFunc)
                 bCellFormatFunction = true;*/
                
                for (LONG iChild = 0; iChild < lnChildCount; ++iChild)
                {   
                    DSSExpNode *lpChildNode = lpOperator->Item(iChild);
                    CHECK_PTR(lpChildNode);
                    
                    //yimliu, 2014/05/29, 883809, Memory leak. lRawData should have its ownership if it is not assigned or copied.
                    /*
                     AERawData lRawData;
                     */
                    
                    //yimliu, 2014/06/20, 883809 related, memory leak. If lRawData has ownership here. It will delete data belongs to other object.
                    //Memory leak is fixed somewhere else.
                    /*
                     AERawData lRawData(true);
                     */
                    AERawData lRawData;
                    
                    hr = Calculate(ipIterator, lpChildNode, ipField, ipFunctionServer, &lRawData);
                    CHECK_HR;
                    
                    CComVariant lValue;
                    DSSDataFlag lValueFlag = lRawData.mFlag;
                    if (DssDataOk == lValueFlag)
                    {
                        hr = DFCDataConversion::ConvertToVariant((unsigned char *)lRawData.mValue, lRawData.mDataType, lRawData.mSizeInByte, &lValue);
                        CHECK_HR;
                    }
                    
                    if (DssDataOk != lValueFlag)
					{
                        if(lfStringFunc)
                        {
                            lValue.vt = VT_EMPTY;
                            
                            // 12/4/2012 tehe: process null in Field Group Iterator
                            opRawData->mDataType = DssDataTypeReserved;
                            opRawData->mFlag = lValueFlag;
                            //yimliu, 2014/06/20, 883809 related, memory leak.
                            if (lpFunctionObject != NULL)
                            {
                                delete lpFunctionObject;
                                lpFunctionObject = NULL;
                            }
                            return S_OK;
                        }
						// ytang, 2010-07-21, TQMS401084/405642
						if ( hIsNumerical(lRawData.mDataType) && ipIterator->getNullChecking()
                            /*&& lFunSQLType != DssFunctionSQLTypeComparison*/
                            // jimzhang Apr/17/2012 As there is no property "FunSQLType" defined on iPad currently,
                            //      use below dynamic_cast to have a alternative check. However, this is just a temporary fix,
                            //      and may cause issues.
                            && ! dynamic_cast<DSSComparisonOperatorFunc*>(lpFunctionObject))
						{
							lValue.Clear();
							lValue.vt = VT_R8;
							lValue.dblVal = 0;
							// lValueFlag = DssDataOk;
						}
						else
						{
							//lwang, 09/17/2008, 309273; skip calculation for Null.
							if (hIsNumerical(lRawData.mDataType))
							{
                                char *lpDouble = (char *) new double(.0);
                                CHECK_NEWED_PTR(lpDouble);
                                
                                AERawData lRawData(lpDouble, sizeof (double), DssDataTypeDouble, opRawData->mFlag, true);
                                opRawData->Assign(lRawData);
							}
							else
                            {
                                opRawData->mDataType = DssDataTypeReserved;
                            }
                            
							lValue.vt = VT_EMPTY;
							opRawData->mFlag = lValueFlag;
                            //yimliu, 2014/06/20, 883809 related, memory leak.
                            if (lpFunctionObject != NULL)
                            {
                                delete lpFunctionObject;
                                lpFunctionObject = NULL;
                            }
							return S_OK;
						}
					}
                    else if (hIsNumerical(lRawData.mDataType))	// jxie, 2011-1-24, TQMS 453258
					{
						hr = lValue.ChangeType(VT_R8);
						if (hr != S_OK)
						{
                            char *lpDouble = (char *) new double(.0);
                            CHECK_NEWED_PTR(lpDouble);
                            
                            AERawData lRawData(lpDouble, sizeof (double), DssDataTypeDouble, DssDataRWInvalid, true);
                            opRawData->Assign(lRawData);
                            
                            //yimliu, 2014/06/20, 883809 related, memory leak.
                            if (lpFunctionObject != NULL)
                            {
                                delete lpFunctionObject;
                                lpFunctionObject = NULL;
                            }
                            
							return S_OK;
						}
					}
                    else
					{
						lfAllNumeric = false;	// jxie, 2011-1-24, TQMS 453258
						
						hr = hCheckBigDecimal(&lValue, &(opRawData->mFlag));
						CHECK_HR;
                        
                        // jimzhang Feb/08/2012 ToDo Implement below logic of server
						// Check Cell Format
                        /*CComPtr<IDSSCellData> lpCellData;
                         if(lValue.vt == VT_DISPATCH)
                         {
                         hr = lValue.pdispVal->QueryInterface(IID_IDSSCellData, (void**)&lpCellData);
                         
                         if(lpCellData)
                         {
                         bCellFormatData = true;
                         
                         if(!bCellFormatFunction)
                         {
                         double tmpValue = 0;
                         hr = lpCellData->get_Value(&tmpValue);
                         CHECK_HR;
                         
                         lValue.Clear();
                         lValue.vt = VT_R8;
                         lValue.dblVal = tmpValue;
                         }
                         
                         }
                         }
                         // 3/15/2012 be careful that for vt==VT_DATE we will not do converting, we save the raw data, and directly get the raw data from function object.
                         else if(lValue.vt == VT_DATE)
                         {
                         DATE lDate = lValue.date;
                         MDataType::DSSDateTime lDateTime(lDate);
                         
                         hr = lValue.Clear();
                         CHECK_HR;
                         
                         hr = DFCDataConversion::ConvertToVariant((unsigned char *) &lDate, DssDataTypeDate, 0, &lValue);
                         CHECK_HR;                            
                         }*/
					}
                    
                    lInputDataType = lValue.vt;      
                    if(lfStringFunc)
                    {
                        EnumDSSDataType lValueType = DssDataTypeUnknown;
                        
						switch(lInputDataType)
						{
                            case VT_I2:
                                lValueType = DssDataTypeShort;
                                break;
                                
                                //case VT_I4:		// can't process VT_I4 and VT_DISPATCH
                                //	break;
                                
                            case VT_R4:
                                lValueType = DssDataTypeFloat;
                                break;
                                
                            case VT_R8:
                                lValueType = DssDataTypeDouble;
                                break;
                                
                            case VT_BOOL:
                                lValueType = DssDataTypeBool;
                                break;
                                
                            case VT_I8:
                                lValueType = DssDataTypeInt64;
                                break;
                             
                            // tehe, TQMS819814, support Date, Time and TimeStamp
                                // Oct/22/2013, TQMS 802342, comment out below case as server side, since we do not know which value we should use to assign to "lValueType": DssDataTypeDate, DssDataTypeTime or DssDataTypeTimeStamp? And different value will result into different formatting.
                                //case VT_DATE:
                                //  lValueType = DssDataTypeDate;
                                //  break;
                                lValueType = lRawData.mDataType;
                                break;
                                
                            case VT_DISPATCH:
                                lValueType = DssDataTypeBigDecimal;
                                break;
                                
                            default:
                                break;
						}
                        
						LONG lIndex = 2 * iChild;
                        
						CComVariant v;
						hr = DFCDataConversion::ConvertToVariant((BYTE *)&lValueType, DssDataTypeInteger, 0, &v);
						CHECK_HR;
                        
						hr = SafeArrayPutElement(lParam.parray, &lIndex, &v);
						CHECK_HR;
                        
						lIndex = 2 * iChild + 1;
						hr = SafeArrayPutElement(lParam.parray, &lIndex, &lValue);
						CHECK_HR;
                    }
                    else
                    {
                        SafeArrayPutElement(lParam.parray, (LONG*)&iChild, &lValue);
                    }
                } // END: for (Int32 iChild = 0; iChild < lnChildCount; ++iChild)
                
                // jimzhang Feb/08/2012 ToDo Implement below logic of server
                /*if(bCellFormatFunction && bCellFormatData)
                 {
                 // Cell Fromat 
                 hr = lpCellFunc->CalculateCellData(lChildCount, &lParam, pValue, pValueFlag);
                 CHECK_HR;
                 
                 if(*pValueFlag == DssDataOk)
                 return S_OK;
                 
                 }
                 else*/
                // 11/12/2012 tehe : some function object extends from SimpleFunction, some extends from NNSimpleFunction, some may extends from both.
                bool lbNNSimpleFunction = false;
                if (lpSimpleFunction && lfAllNumeric)
                {
                    MBase::StrongArrayPtr<double> lpParamD(new double[lnChildCount]);                    
                    
					// Variant Array to Double Array
					for(LONG j = 0; j < lnChildCount; j++)
					{
						CComVariant ltmpVal;
						SafeArrayGetElement(lParam.parray, (LONG*)&j, &ltmpVal);
                        
                        // jimzhang Feb/08/2012 ToDo Implement below logic of server
                        /*if(ltmpVal.vt == VT_DISPATCH)
                         {
                         CComPtr<IDSSCellData> lpCellData;
                         hr = ltmpVal.pdispVal->QueryInterface(IID_IDSSCellData, (void**)&lpCellData);
                         
                         if(lpCellData)
                         {
                         double tmpValue = 0;
                         hr = lpCellData->get_Value(&tmpValue);
                         CHECK_HR;
                         
                         lpParamD[j] = tmpValue;
                         continue;
                         }
                         }
                         else {*/
						// Big Decimal has been checked
						ltmpVal.ChangeType(VT_R8);
						lpParamD[j] = ltmpVal.dblVal;
					}
                    
                    double lResult = 0;
					hr = lpSimpleFunction->Calculate(lnChildCount, lpParamD.Get(), &lResult, &(opRawData->mFlag));
					if(hr == E_NOTIMPL)
                    {
                        lbNNSimpleFunction = true;
                    }
                    else
                    {
                        CHECK_HR;
                        
                        if (DssDataOk == opRawData->mFlag)
                        {
                            char *lpDouble = (char *) new double(lResult);
                            CHECK_NEWED_PTR(lpDouble);
                            
                            AERawData lRawData(lpDouble, sizeof (double), DssDataTypeDouble, opRawData->mFlag, true);
                            opRawData->Assign(lRawData);
                            
                            //yimliu, 2014/06/20, 883809 related, memory leak.
                            if (lpFunctionObject != NULL)
                            {
                                delete lpFunctionObject;
                                lpFunctionObject = NULL;
                            }
                            
                            return S_OK;
                        }
                        else
                        {
                            char *lpDouble = (char *) new double(.0);
                            CHECK_NEWED_PTR(lpDouble);
                            
                            AERawData lRawData(lpDouble, sizeof (double), DssDataTypeDouble, opRawData->mFlag, true);
                            opRawData->Assign(lRawData);
                            
                            //yimliu, 2014/06/20, 883809 related, memory leak.
                            if (lpFunctionObject != NULL)
                            {
                                delete lpFunctionObject;
                                lpFunctionObject = NULL;
                            }
                            
                            return S_OK;
                        }
                    }
                }
                // 11/19/2012 
                if ((!(lpSimpleFunction && lfAllNumeric ) && lpNNSimpleFunction) || lbNNSimpleFunction)
                {
                    CComVariant lValue;
                    DSSDataFlag lValueFlag;
                    hr = lpNNSimpleFunction->Calculate(lnChildCount, &lParam, &lValue, &lValueFlag);
					CHECK_HR;
                    
					if (DssDataOk == lValueFlag)
					{
                        /*
                        return DSSAEHelper::VariantToRawData(lValue, opRawData->mValue, opRawData->mSizeInByte, opRawData->mDataType);
                         */
                        
                        //yimliu, 2014/06/20, 883809 related, memory leak.
                        /*
                        int hr = DSSAEHelper::VariantToRawData(lValue, opRawData->mValue, opRawData->mSizeInByte, opRawData->mDataType);
                        */
                        AERawData lRawData(NULL, opRawData->mSizeInByte, opRawData->mDataType, opRawData->mFlag, true);
                        int hr = DSSAEHelper::VariantToRawData(lValue, lRawData.mValue, lRawData.mSizeInByte, lRawData.mDataType);
                        opRawData->Assign(lRawData);
                        
                        //yimliu, 2014/05/29, 883809, Memory leak.
                        //Actually every return in this function forgets to delete lpFunctionObject, thus has risk of memory leak.
                        //Don't have time to fix them all.
                        //yimliu, 2014/06/20, already fixed all of them.
                        if (lpFunctionObject != NULL)
                        {
                            delete lpFunctionObject;
                            lpFunctionObject = NULL;
                        }
                        
                        return hr;
					}
					else
					{ 
                        // 12/3/2012 tehe: handle Trim("")
                        opRawData->mFlag = lValueFlag;
						opRawData->mDataType = DssDataTypeReserved;
                        
                        //yimliu, 2014/06/20, 883809 related, memory leak.
                        if (lpFunctionObject != NULL)
                        {
                            delete lpFunctionObject;
                            lpFunctionObject = NULL;
                        }
                        
						return S_OK;
					}
                }
                else
                {
                    // function type not supported
					opRawData->mFlag = DssDataInvalid;
                    
                    //yimliu, 2014/06/20, 883809 related, memory leak.
                    if (lpFunctionObject != NULL)
                    {
                        delete lpFunctionObject;
                        lpFunctionObject = NULL;
                    }
					return E_FAIL;
                }
            } // END: if (DssFunctionTypeSimple == lFunctionType)
            else if (DssFunctionTypeGeneric == lFunctionType)
            {
                DSSGenericFunction *lpGenericFunction = dynamic_cast<DSSGenericFunction *>(lpFunctionObject);
                DSSNNGenericFunction *lpNNGenericFunction = dynamic_cast<DSSNNGenericFunction *>(lpFunctionObject);
                if (!lpGenericFunction && !lpNNGenericFunction)
                {
                    opRawData->mFlag = DssDataInvalid;
                    //yimliu, 2014/06/20, 883809 related, memory leak.
                    if (lpFunctionObject != NULL)
                    {
                        delete lpFunctionObject;
                        lpFunctionObject = NULL;
                    }
                    
                    return E_FAIL;   
                }
                
                int lnChildCount = lpOperator->Count();
                assert(lnChildCount > 0);
                
                vector<DSSDataFlag> lvFlags;
				vector<CComVariant> lvValues;
				lvFlags.resize(lnChildCount, DssDataOk);
				lvValues.resize(lnChildCount);
                
                bool lfAllNumeric = true;
				for (Int32 i = 0; i < lnChildCount; i++)
				{                    
                    DSSExpNode *lpChildNode = lpOperator->Item(i);
                    CHECK_PTR(lpChildNode);
                    
                    AERawData lRawData;
                    hr = Calculate(ipIterator, lpChildNode, ipField, ipFunctionServer, &lRawData);
                    CHECK_HR;
                    
                    lvFlags[i] = lRawData.mFlag;
                    if (DssDataOk == lvFlags[i])
                    {
                        hr = DFCDataConversion::ConvertToVariant((unsigned char *)lRawData.mValue, lRawData.mDataType, lRawData.mSizeInByte, &lvValues[i]);
                        CHECK_HR;
                    }
                    
                    if (lfAllNumeric && ! hIsNumerical(lRawData.mDataType))
                        lfAllNumeric = false;
				}
                
                if (lpGenericFunction && lfAllNumeric)
                {
                    for (Int32 i = 0; i < lnChildCount; i++)
                    {
                        hr = hCheckBigDecimal(&lvValues[i], &(opRawData->mFlag));
						if (S_OK != hr)
                        {
                            if (i)
                                lpGenericFunction->Clear();
                            
                            //yimliu, 2014/06/20, 883809 related, memory leak.
                            if (lpFunctionObject != NULL)
                            {
                                delete lpFunctionObject;
                                lpFunctionObject = NULL;
                            }
                            
                            return hr;
                        }
                        
                        // jimzhang Feb/08/2012 ToDo Implement below logic of server
                        /*if(lvFlags[i] == DssDataOk && lvValues[i].vt == VT_DISPATCH)
                         {
                         CComPtr<IDSSCellData> lpCellData;
                         hr = lvValues[i].pdispVal->QueryInterface(IID_IDSSCellData, (void**)&lpCellData);
                         
                         if(lpCellData)
                         {
                         double tmpValue = 0;
                         hr = lpCellData->get_Value(&tmpValue);
                         CHECK_HR;
                         
                         lvValues[i].Clear();
                         lvValues[i].vt = VT_R8;
                         lvValues[i].dblVal = tmpValue;
                         }
                         }*/
                        
                        // apply null checking.
						if (lvFlags[i] != DssDataOk)
						{
							bool lbCanProcessNULL = false;
							hr = lpGenericFunction->IsNullProcessable(&lbCanProcessNULL);
							if (S_OK != hr)
                            {
                                if (i)
                                    lpGenericFunction->Clear();
                                
                                //yimliu, 2014/06/20, 883809 related, memory leak.
                                if (lpFunctionObject != NULL)
                                {
                                    delete lpFunctionObject;
                                    lpFunctionObject = NULL;
                                }
                                
                                return hr;
                            }
                            
							// ytang, 2010-07-21, TQMS401084/405642
							if(false == lbCanProcessNULL && ipIterator->getNullChecking() /*&& lFunSQLType != DssFunctionSQLTypeComparison*/)
							{
								lvValues[i].Clear();
								lvValues[i].vt = VT_R8;
								lvValues[i].dblVal = 0;
								lvFlags[i] = DssDataOk;
							}							
							else
							{
								if (lFunctionIndex == DssFuncIsNull || lFunctionIndex == DssFuncIsNotNull || lFunctionIndex == DssFuncNullToZero || lFunctionIndex == DssFuncZeroToNull)
								{
									//for IsNull and IsNotNull, we have supported the numeric interface, don't return it	
									lvValues[i].Clear();
									lvValues[i].vt = VT_R8;
									lvValues[i].dblVal = 0;
								}
								else
								{
                                    char *lpDouble = (char *) new double(.0);
                                    CHECK_NEWED_PTR(lpDouble);
                                    
                                    AERawData lRawData(lpDouble, sizeof (double), DssDataTypeDouble, opRawData->mFlag, true);
                                    opRawData->Assign(lRawData);
                                    
									// ytang, 2010-07-22, TQMS401084/405642			
									//return S_OK;
								}
							}
						}
                        
                        hr = lvValues[i].ChangeType(VT_R8);
						if (hr != S_OK)
						{
                            if (i)
                                lpGenericFunction->Clear();
                            
                            char *lpDouble = (char *) new double(.0);
                            CHECK_NEWED_PTR(lpDouble);
                            
                            AERawData lRawData(lpDouble, sizeof (double), DssDataTypeDouble, DssDataRWInvalid, true);
                            opRawData->Assign(lRawData);
                            
                            //yimliu, 2014/06/20, 883809 related, memory leak.
                            if (lpFunctionObject != NULL)
                            {
                                delete lpFunctionObject;
                                lpFunctionObject = NULL;
                            }
                            
							return S_OK;
						}
                        
						hr = lpGenericFunction->SetInputParam(i+1, 1, &lvValues[i].dblVal, &lvFlags[i]);
						if (S_OK != hr)
                        {
                            lpGenericFunction->Clear();
                            
                            //yimliu, 2014/06/20, 883809 related, memory leak.
                            if (lpFunctionObject != NULL)
                            {
                                delete lpFunctionObject;
                                lpFunctionObject = NULL;
                            }
                            
                            return hr;
                        }
                    } //END: for (Int32 i = 0; i < lChildCount; i++)
                    
                    hr = lpGenericFunction->Calculate();
					if (S_OK != hr)
                    {
                        lpGenericFunction->Clear();
                        
                        //yimliu, 2014/06/20, 883809 related, memory leak.
                        if (lpFunctionObject != NULL)
                        {
                            delete lpFunctionObject;
                            lpFunctionObject = NULL;
                        }
                        
                        return hr;
                    }
                    
					Int32 lnDataSize = 1;
					double ldbResult[1];
					DSSDataFlag lDataFlags[1];
                    
					hr = lpGenericFunction->GetOutput(1, &lnDataSize, ldbResult, lDataFlags);
					if (S_OK != hr)
                    {
                        lpGenericFunction->Clear();
                        
                        //yimliu, 2014/06/20, 883809 related, memory leak.
                        if (lpFunctionObject != NULL)
                        {
                            delete lpFunctionObject;
                            lpFunctionObject = NULL;
                        }
                        
                        return hr;
                    }
                    
                    hr = lpGenericFunction->Clear();
                    CHECK_HR;
                    
					if(lDataFlags[0] == DssDataOk)
					{
                        char *lpDouble = (char *) new double(ldbResult[0]);
                        CHECK_NEWED_PTR(lpDouble);
                        
                        AERawData lRawData(lpDouble, sizeof (double), DssDataTypeDouble, opRawData->mFlag, true);
                        opRawData->Assign(lRawData);
                        
                        //yimliu, 2014/06/20, 883809 related, memory leak.
                        if (lpFunctionObject != NULL)
                        {
                            delete lpFunctionObject;
                            lpFunctionObject = NULL;
                        }
                        
						return S_OK;
					}
					else
					{
                        char *lpDouble = (char *) new double(.0);
                        CHECK_NEWED_PTR(lpDouble);
                        
                        AERawData lRawData(lpDouble, sizeof (double), DssDataTypeDouble, opRawData->mFlag, true);
                        opRawData->Assign(lRawData);
					}
                    
					opRawData->mFlag = lDataFlags[0];
                    
                    //yimliu, 2014/06/20, 883809 related, memory leak.
                    if (lpFunctionObject != NULL)
                    {
                        delete lpFunctionObject;
                        lpFunctionObject = NULL;
                    }
                    
					return S_OK;                    
                } // END: if (lpGenericFunction && lfAllNumeric)
                else if (lpNNGenericFunction)
                {
                    switch(lFunctionIndex)
                    {
                        case DssFuncIsNull:
                        case DssFuncIsNotNull:
                        case DssFuncNullToZero:
                        case DssFuncZeroToNull:
                        {
                            if(lnChildCount != 1)
                            {
                                opRawData->mFlag = DssDataInvalid;
                                
                                //yimliu, 2014/06/20, 883809 related, memory leak.
                                if (lpFunctionObject != NULL)
                                {
                                    delete lpFunctionObject;
                                    lpFunctionObject = NULL;
                                }
                                
                                return E_FAIL;
                            }
                            
                            SAFEARRAYBOUND lBound;
                            lBound.lLbound = 0;
                            lBound.cElements = 1;
                            
                            CComVariant lParam;
                            lParam.vt = VT_ARRAY|VT_VARIANT;
                            lParam.parray = SafeArrayCreate(VT_VARIANT, 1, &lBound);
                            
                            // jimzhang Feb/08/2012 ToDo Implement below logic of server
                            /*if(lvFlags[0] == DssDataOk && lvValues[0].vt == VT_DISPATCH)
                             {
                             CComPtr<IDSSCellData> lpCellData;
                             hr = lvValues[0].pdispVal->QueryInterface(IID_IDSSCellData, (void**)&lpCellData);
                             
                             if(lpCellData)
                             {
                             double tmpValue = 0;
                             hr = lpCellData->get_Value(&tmpValue);
                             CHECK_HR;
                             
                             lvValues[0].Clear();
                             lvValues[0].vt = VT_R8;
                             lvValues[0].dblVal = tmpValue;
                             }
                             
                             }*/
                            
                            // no null checking 
                            
                            LONG l = 0;
                            SafeArrayPutElement(lParam.parray, &l, &lvValues[0]);
                            EnumDSSDataType lDataType;
                            
                            if(lvValues[0].vt == VT_EMPTY || lvValues[0].vt == VT_R8)
                                lDataType = DssDataTypeDouble;
                            else if(lvValues[0].vt == VT_I4)
                                lDataType = DssDataTypeLong;
                            else if(lvValues[0].vt == VT_I2)
                                lDataType = DssDataTypeShort;
                            else if(lvValues[0].vt == VT_R4)
                                lDataType = DssDataTypeFloat;
                            else if(lvValues[0].vt == VT_INT)
                                lDataType = DssDataTypeInteger;
                            else if(lvValues[0].vt == VT_BOOL)
                                lDataType = DssDataTypeBool;
                            else
                                lDataType = DssDataTypeUnknown;
                            
                            hr = lpNNGenericFunction->SetInputParam(1, 1, lDataType, &lParam, &lvFlags[0]);
                            if (S_OK != hr)
                            {
                                lpNNGenericFunction->Clear();
                                
                                //yimliu, 2014/06/20, 883809 related, memory leak.
                                if (lpFunctionObject != NULL)
                                {
                                    delete lpFunctionObject;
                                    lpFunctionObject = NULL;
                                }
                                
                                return hr;
                            }
                            
                            hr = lpNNGenericFunction->Calculate();
                            if (S_OK != hr)
                            {
                                lpNNGenericFunction->Clear();
                                
                                //yimliu, 2014/06/20, 883809 related, memory leak.
                                if (lpFunctionObject != NULL)
                                {
                                    delete lpFunctionObject;
                                    lpFunctionObject = NULL;
                                }
                                
                                return hr;
                            }
                            
                            Int32 ldataSize = 1;                            
                            CComVariant lResult;
                            CComVariant lValue;
                            // zchen: fix mem leak since Oct 2005, no need to create safearray for output parameter!
                            lpNNGenericFunction->GetOutput(1, &ldataSize, &lResult, &(opRawData->mFlag));

                            if( (opRawData->mFlag == DssDataOk) && (lResult.vt == (VT_ARRAY|VT_VARIANT)) )
                            {
                                l = 0;
                                SafeArrayGetElement(lResult.parray, &l, &lValue);
                                //yimliu, 2014/06/20
                                /*
                                DSSAEHelper::VariantToRawData(lValue, opRawData->mValue, opRawData->mSizeInByte, opRawData->mDataType);
                                */
                                AERawData lRawData(NULL, opRawData->mSizeInByte, opRawData->mDataType, opRawData->mFlag, true);
                                DSSAEHelper::VariantToRawData(lValue, lRawData.mValue, lRawData.mSizeInByte, lRawData.mDataType);
                                opRawData->Assign(lRawData);
                            }
                            
                            hr = lpNNGenericFunction->Clear();
                            CHECK_HR;
                            
                            //yimliu, 2014/06/20, 883809 related, memory leak.
                            if (lpFunctionObject != NULL)
                            {
                                delete lpFunctionObject;
                                lpFunctionObject = NULL;
                            }
                            
                            return S_OK;
                        }
                            break;
                        case DssFuncCase:
                        case DssFuncCaseV:
                        case DssFuncIF:
                        {
                            //lwang, 09/10/2008, 327651; The above functions are changed to generic functions to fix TQMS 325343                            
                            for (Int32 i=0; i<lnChildCount; i++)
                            {
                                hr = hCheckBigDecimal(&lvValues[i], &(opRawData->mFlag));
                                if (S_OK != hr)
                                {
                                    if (i)                                        
                                        lpNNGenericFunction->Clear();
                                    
                                    //yimliu, 2014/06/20, 883809 related, memory leak.
                                    if (lpFunctionObject != NULL)
                                    {
                                        delete lpFunctionObject;
                                        lpFunctionObject = NULL;
                                    }
                                    
                                    return hr;
                                }
                                
                                // jimzhang Feb/08/2012 ToDo Implement below logic of server
                                /*if(lvFlags[i] == DssDataOk && lvValues[i].vt == VT_DISPATCH)
                                 {
                                 CComPtr<IDSSCellData> lpCellData;
                                 hr = lvValues[i].pdispVal->QueryInterface(IID_IDSSCellData, (void**)&lpCellData);
                                 
                                 if(lpCellData)
                                 {
                                 double tmpValue = 0;
                                 hr = lpCellData->get_Value(&tmpValue);
                                 CHECK_HR;
                                 
                                 lvValues[i].Clear();
                                 lvValues[i].vt = VT_R8;
                                 lvValues[i].dblVal = tmpValue;
                                 }
                                 
                                 }*/
                                
                                // no null checking. CASE/IF function is null processible
                                
                                /*
                                 if (lvFlags[i] == DssDataOk)
                                    lInputDataType = lvValues[i].vt;
                                */
                                SAFEARRAYBOUND lBound;
                                lBound.lLbound = 0;
                                lBound.cElements = 1;
                                
                                CComVariant lParam;
                                lParam.vt = VT_ARRAY|VT_VARIANT;
                                lParam.parray = SafeArrayCreate(VT_VARIANT, 1, &lBound);
                                
                                LONG l = 0;
                                SafeArrayPutElement(lParam.parray, &l, &lvValues[i]);
                                
                                EnumDSSDataType lDataType = DssDataTypeDouble;		//does not matter
                                hr = lpNNGenericFunction->SetInputParam(i+1, 1, lDataType, &lParam, &lvFlags[i]);
                                if (S_OK != hr)
                                {
                                    lpNNGenericFunction->Clear();
                                    
                                    //yimliu, 2014/06/20, 883809 related, memory leak.
                                    if (lpFunctionObject != NULL)
                                    {
                                        delete lpFunctionObject;
                                        lpFunctionObject = NULL;
                                    }
                                    
                                    return hr;
                                }
                            }
                            
                            hr = lpNNGenericFunction->Calculate();
                            if (S_OK != hr)
                            {
                                lpNNGenericFunction->Clear();
                                
                                //yimliu, 2014/06/20, 883809 related, memory leak.
                                if (lpFunctionObject != NULL)
                                {
                                    delete lpFunctionObject;
                                    lpFunctionObject = NULL;
                                }
                                
                                return hr;
                            }
                            
                            Int32 ldataSize = 1;
                            CComVariant lValue;
                            DSSDataFlag lValueFlag;
                            hr = lpNNGenericFunction->GetOutput(1, &ldataSize, &lValue, &lValueFlag);
                            if (S_OK != hr)
                            {
                                lpNNGenericFunction->Clear();
                                
                                //yimliu, 2014/06/20, 883809 related, memory leak.
                                if (lpFunctionObject != NULL)
                                {
                                    delete lpFunctionObject;
                                    lpFunctionObject = NULL;
                                }
                                
                                return hr;
                            }
                            
                            if (DssDataOk == lValueFlag)
                            {
                                //yimliu, 2014/06/20, 883809 related, memory leak.
                                /*
                                hr = DSSAEHelper::VariantToRawData(lValue, opRawData->mValue, opRawData->mSizeInByte, opRawData->mDataType);
                                */
                                AERawData lRawData(NULL, opRawData->mSizeInByte, opRawData->mDataType, opRawData->mFlag, true);
                                hr = DSSAEHelper::VariantToRawData(lValue, lRawData.mValue, lRawData.mSizeInByte, lRawData.mDataType);
                                opRawData->Assign(lRawData);
                            }
                            else
                            {
                                opRawData->mFlag = lValueFlag;
                                // Dec/5/2012, [todo], convert VARTYPE to EnumDSSDataType
                            }   
                            
                            hr = lpNNGenericFunction->Clear();
                            CHECK_HR;
                            
                            //yimliu, 2014/06/20, 883809 related, memory leak.
                            if (lpFunctionObject != NULL)
                            {
                                delete lpFunctionObject;
                                lpFunctionObject = NULL;
                            }
                            
                            return hr;
                        }
                            break;
                        default:
                        {
                            opRawData->mFlag = DssDataInvalid;
                            
                            //yimliu, 2014/06/20, 883809 related, memory leak.
                            if (lpFunctionObject != NULL)
                            {
                                delete lpFunctionObject;
                                lpFunctionObject = NULL;
                            }
                            
                            return E_NOTIMPL;
                        }
                            break;
                    }                    
                } // END: else if (lpNNGenericFunction)
                else
                {
                    // function type not supported
					opRawData->mFlag = DssDataInvalid;
                    
                    //yimliu, 2014/06/20, 883809 related, memory leak.
                    if (lpFunctionObject != NULL)
                    {
                        delete lpFunctionObject;
                        lpFunctionObject = NULL;
                    }
                    
					return E_FAIL;
                }
            } // END: else if (DssFunctionTypeGeneric == lFunctionType)
            else
            {
                // function type not supported
				opRawData->mFlag = DssDataInvalid;
                
                //yimliu, 2014/06/20, 883809 related, memory leak.
                if (lpFunctionObject != NULL)
                {
                    delete lpFunctionObject;
                    lpFunctionObject = NULL;
                }
                
				return E_FAIL;
            }
        } // END: case DssNodeOperator
            break;
            
        default:
        {
            opRawData->mFlag= DssDataRWInvalid;
			return S_FALSE;
        }
            break;
    } // END: switch (lNodeType)
    
    return hr;
}

int DSSRWFieldGroupCalculator::hCalculateFunctionIn (DSSRWFieldGroupIterator *ipIterator, DSSExpNode *ipNode, DSSField *ipField, DSSFunctionServer *ipFunctionServer, AERawData *opRawData)
{
    int hr = S_OK;
    
    DSSRWFieldGroupModelIterator *lpNewRWDIterator = dynamic_cast<DSSRWFieldGroupModelIterator *>(ipIterator);
    
    DSSExpNodeOperator *lpOperator = dynamic_cast<DSSExpNodeOperator*>(ipNode);
    CHECK_PTR(lpOperator);
    
    int lFunType = lpOperator->getOperatorType();
    
    DSSExpNode *lpChildNode1 = lpOperator->Item(0);
    CHECK_PTR(lpChildNode1);
    
    // EnumDSSNodeType lpChildNodeType1 = lpChildNode1->getType();
    
    EnumDSSExpressionType lExpType = lpOperator->getExpressionType();
    if (DssFilterListQual == lExpType)
    {
        DSSExpNodeShortcut *lpShortCut = dynamic_cast<DSSExpNodeShortcut*>(lpChildNode1);
        CHECK_PTR(lpShortCut);
        
        DSSObjectInfo *lTargetObj = lpShortCut->getTarget();
        CHECK_PTR(lTargetObj);
        
        DSSBaseElementProxy *lpTargetElement = NULL;
        
        if (lpNewRWDIterator)
        {
            
            hr = lpNewRWDIterator->getCurrentElementProxyByObjInfo(lTargetObj, &lpTargetElement);
            if (S_OK != hr)
            {
                double lVal;
                
                if(lFunType == DssFunctionIn)
                    lVal = 0.0;
                else
                    lVal = 1.0;
                
                char *lpDouble = (char *) new double(lVal);
                CHECK_NEWED_PTR(lpDouble);
                
                AERawData lRawData(lpDouble, sizeof (double), DssDataTypeDouble, DssDataOk, true);
                opRawData->Assign(lRawData);
                
                return S_OK;
            }
        }
        else
        {
            // Sep/18/2012, todo, implement old RWD's logic of getting target element
            return E_NOTIMPL;
        }
        
        DSSExpNode *lpChildNode2 = lpOperator->Item(1);
        CHECK_PTR(lpChildNode2);
        
        DSSExpNodeElementsObject *lpElementsObj = dynamic_cast<DSSExpNodeElementsObject *>(lpChildNode2);
        CHECK_PTR(lpElementsObj);
        
        DSSBaseElementsProxy *lpElements = lpElementsObj->getElements();
        CHECK_PTR(lpElements);
        
        int lElementCount = lpElements->Count();
        for (Int32 i = 0; i < lElementCount; ++i)
        {
            DSSBaseElementProxy *lpElement = lpElements->Item(i);
            CHECK_PTR(lpElement);
            
            GUID lElementGUID = lpElement->getObjectID();
            if (!ATL::InlineIsEqualGUID(lElementGUID, lTargetObj->getID()))
                continue;
            bool lbEqual = false;
            if (lpNewRWDIterator->getTrimString() && lpElement->getElementType() == DssElementConcrete)
                lbEqual = lpTargetElement->IsEqualTrim(lpElement);
            else
                lbEqual = lpTargetElement->IsEqual(lpElement);
            if(lbEqual)
            {
                double lVal;
                
                if(lFunType == DssFunctionIn)
                    lVal = 1.0;
                else
                    lVal = 0.0;
                
                char *lpDouble = (char *) new double(lVal);
                CHECK_NEWED_PTR(lpDouble);
                
                AERawData lRawData(lpDouble, sizeof (double), DssDataTypeDouble, DssDataOk, true);
                opRawData->Assign(lRawData);
                
                return S_OK;
            }
        }
        
        double lVal;
        
        if(lFunType == DssFunctionIn)
            lVal = 0.0;
        else
            lVal = 1.0;
        
        char *lpDouble = (char *) new double(lVal);
        CHECK_NEWED_PTR(lpDouble);
        
        AERawData lRawData(lpDouble, sizeof (double), DssDataTypeDouble, DssDataOk, true);
        opRawData->Assign(lRawData);
        
        return S_OK;
    }
    else if (DssFilterJointListQual == lExpType)
    {
        // Feb/10/2012, todo, port server's logic
        
        char *lpDouble = (char *) new double(.0);
        CHECK_NEWED_PTR(lpDouble);
        
        AERawData lRawData(lpDouble, sizeof (double), DssDataTypeDouble, DssDataInvalid, true);
        opRawData->Assign(lRawData);
        
		return E_NOTIMPL;
    }
    else if (DssFilterListFormQual == lExpType)
    {
        DSSComparisonOperatorFunc *lpEqualFuncObj = dynamic_cast<DSSComparisonOperatorFunc *>(ipFunctionServer->getFunctionByIndex(DssFuncEquals));
        //DSSComparisonOperatorFunc *lpEqualFuncObj = dynamic_cast<DSSComparisonOperatorFunc *>(ipFunctionServer->getFunction(DssFuncEquals));
        CHECK_PTR(lpEqualFuncObj);
        
        MBase::DSSStrongPtr<DSSComparisonOperatorFunc> lEqualFuncObjPtr(lpEqualFuncObj);
        
        DSSExpNode *lTargetNode = ipNode->Item(0);
        CHECK_PTR(lTargetNode);
        
        AERawData lRawData;
        hr = Calculate(ipIterator, lTargetNode, ipField, ipFunctionServer, &lRawData);
        CHECK_HR;
        
        CComVariant lTargetValue;
		DSSDataFlag lTargetValueFlag = lRawData.mFlag;
        if (DssDataOk == lTargetValueFlag)
        {
            hr = DFCDataConversion::ConvertToVariant((unsigned char *)lRawData.mValue, lRawData.mDataType, lRawData.mSizeInByte, &lTargetValue);
            CHECK_HR;
        }
        
        bool lIsIn = false;
        
        Int32 lChildCount = ipNode->Count();
        for (Int32 i = 1; i < lChildCount; i++)
        {   
            DSSExpNode *lElementNode = ipNode->Item(i);
            CHECK_PTR(lElementNode);
            
            AERawData lRawData;
            hr = Calculate(ipIterator, lElementNode, ipField, ipFunctionServer, &lRawData);
            CHECK_HR;
            
            DSSDataFlag lElementFlag = lRawData.mFlag;
			CComVariant lElementValue;
            if (DssDataOk == lElementFlag)
            {
                hr = DFCDataConversion::ConvertToVariant((unsigned char *)lRawData.mValue, lRawData.mDataType, lRawData.mSizeInByte, &lElementValue);
                CHECK_HR;
            }
            
            //lwang, 11/2/2009, 382493, convert VT_DATE
			if (lElementValue.vt == VT_DATE)
			{
				DATE lDate = lElementValue.date;
				MDataType::DSSDateTime lDateTime(lDate);
				hr = lElementValue.Clear();
				CHECK_HR;
				hr = DFCDataConversion::ConvertToVariant((BYTE*) &lDateTime, DssDataTypeDate, 0, &lElementValue);
				CHECK_HR;
			}
            
            SAFEARRAYBOUND lBound;
			lBound.lLbound = 0;
			lBound.cElements = 2;
            
			CComVariant lParam;
            
			lParam.vt = VT_ARRAY|VT_VARIANT;
			lParam.parray = SafeArrayCreate(VT_VARIANT, 1, &lBound);
            
			LONG l = 0;
			SafeArrayPutElement(lParam.parray, &l, &lTargetValue);
			l = 1;
			SafeArrayPutElement(lParam.parray, &l, &lElementValue);
            
			CComVariant lResult;
			DSSDataFlag lFlag;
			hr = lpEqualFuncObj->Calculate(2, &lParam, &lResult, &lFlag);
			CHECK_HR;
            
			lResult.ChangeType(VT_BOOL);            
            if(lResult.boolVal == VARIANT_TRUE)
			{
				lIsIn = true;
				break;
			}
        }
        
        double lVal;
        
		if(lIsIn)
		{
			if(lFunType == DssFunctionIn)
				lVal = 1.0;
			else
				lVal = 0.0;
		}
		else
		{
			if(lFunType == DssFunctionIn)
				lVal = 0.0;
			else
				lVal = 1.0;
		}
        
        char *lpDouble = (char *) new double(lVal);
        CHECK_NEWED_PTR(lpDouble);
        
        AERawData lTmpRawData(lpDouble, sizeof (double), DssDataTypeDouble, DssDataOk, true);
        opRawData->Assign(lTmpRawData);
        
		return S_OK;
    }
    else
    {
        char *lpDouble = (char *) new double(.0);
        CHECK_NEWED_PTR(lpDouble);
        
        AERawData lRawData(lpDouble, sizeof (double), DssDataTypeDouble, DssDataInvalid, true);
        opRawData->Assign(lRawData);
        
		return E_FAIL;
    }
    
    return hr;
}
