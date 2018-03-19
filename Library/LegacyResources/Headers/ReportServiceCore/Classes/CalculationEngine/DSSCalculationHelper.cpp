/*
 *  DSSCalculationHelper.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 1/25/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#import <set>

#include "DSSCalculationHelper.h"
#import "Filter/DSSExpNodeConstant.h"
#import "DSSCube.h"
#import "DSSSliceCalculator.h"
#import "DSSCubeDataSlice.h"
#import "DSSTabularIndex.h"
#import "DSSFunctionObject.h"
#import "DSSCubeDataSlice.h"
#import "DSSDataColumn.h"
#import "DSSViewDataSet.h"
#import "DFCDataConversion.h"
#import "DSSFunctionUtility.h"
#import "DSSTabularUnit.h"
#import "DSSTabularAttribute.h"
#import "DSSSliceCorrelator.h"
#import "DSSSortConverter.h"
#import "DSSExpNodeFormShortcut.h"
#import "DSSFunctionServer.h"
#import "DSSDimty.h"
#import "Filter/DSSExpNodeOperator.h"
#import "DSSRWGridModel.h"
#import "DSSFunction.h"
#import "DSSCExpression.h"
#import "DSSExpNodeShortcut.h"
#import "DSSAggMetric.h"
#import "DSSSortDefinition.h"
#import "DSSExpNodeTime.h"
#import "DSSFunction.h"
#import "DSSRankFunction.h"
#import "DSSExpNodeBigDecimal.h"
#import "DSSCalculationEvent.h"
#import "DSSStrongPtr.h"


EventContext::EventContext(DSSCalculationEvent *ipEvent,
                           DSSTabularData *ipTabularData,
                           IDSSTemplateModel *ipDataModel,
                           DSSViewDataSet *ipViewDataset,
                           DSSCube *ipCube,
                           DSSTemplate * ipTemplate,
                           int iLevel,
                           int iEventIndex,
                           DSSCalculationPlan *ipCalculationPlan,
                           DSSObjectContext *ipObjectContext,
                           MCESchema *ipSchema,
                           Int32 *ipEventType,
                           GUID *ipEventID,
                           int *ipEventTag
                           ):mpEvent(ipEvent), mpDataModel(ipDataModel), mpTabularData(ipTabularData), mpCube(ipCube), mLevel(iLevel), mEvent(iEventIndex)
, mpCalculationPlan(ipCalculationPlan), mpObjectContext(ipObjectContext), mpSchema(ipSchema), mpTemplate(ipTemplate), mpViewDataset(ipViewDataset)
{
    if (ipEvent)
    {
        ipEvent->mEventIndex = iEventIndex;
        if (ipEventType && ipEventID && ipEventTag)
        {
            ipEvent->mEventType = (EnumEventType)(*ipEventType);
            ipEvent->mID = *ipEventID;
            ipEvent->mEventTag = *ipEventTag;
        }
    }
}

EventContext* EventContext::Init(DSSCalculationEvent *ipEvent,
                                 void *ipDummy,
                                 DSSObjectContext *ipObjectContext,
                                 DSSFunctionServer *ipFunctionServer,
                                 DSSCube *ipSourceCube,
                                 DSSCube *ipTargetCube,
                                 DSSCalculationPlan *ipCalculationPlan,
                                 int iLevel,
                                 int iEvent,
                                 EnumNullCheckingOption iNullCheckingOption,
                                 int iEventType,
                                 DSSReportInstance *ipReportInstance,
                                 DSSTemplate * ipTemplate,
                                 MBase::Buffer *ipBuffer,
                                 int iSessionID,
                                 DSSDocumentDefinition *ipDeocumentDef,
                                 DSSLevelResolver *ipResolveLevel,
                                 DSSTabularData *ipTabularData,
                                 IDSSTemplateModel *ipDataModel,
                                 DSSViewDataSet *ipViewDataset,
                                 MCESchema *ipSchema,
                                 GUID *ipEventID,
                                 int *ipEventTag)
{
    mpEvent = ipEvent;
    mpDataModel = ipDataModel;
    mpTabularData = ipTabularData;
    mpCube = ipSourceCube;
    mLevel = iLevel;
    mEvent = iEvent;
    mpCalculationPlan = ipCalculationPlan;
    mpObjectContext = ipObjectContext;
    mpSchema = ipSchema;
    mpTemplate = ipTemplate;
    mpViewDataset = ipViewDataset;
    
    if (ipEvent)
    {
        ipEvent->mEventIndex = iEvent;
        ipEvent->mEventType = (EnumEventType)(iEventType);

        if (ipEventID && ipEventTag)
        {
            ipEvent->mID = *ipEventID;
            ipEvent->mEventTag = *ipEventTag;
        }
    }
    return this;
}


CalculationContext::~CalculationContext()
{
    for(int i = 0; i < mvSortbyDefn.size(); ++i)
    {
        delete mvSortbyDefn[i];
    }
};

int DSSCalculationHelper::GenerateConstantSlice(DSSExpNode * ipConstNode,
												 DSSCube * ipCube,
												 DSSSliceID *opSliceID)
{
	if(!ipConstNode || !opSliceID || !ipCube)
		return E_FAIL;
	int hr = S_OK;
	
	EnumDSSNodeType lType = (EnumDSSNodeType)ipConstNode->getType();
	
    switch (lType)
    {
        case DssNodeTime:
		{
			//XW 7/11/00 Need to go through TimeNode interface
			DSSExpNodeTime *lpDTimeNode = static_cast<DSSExpNodeTime*>(ipConstNode);
			CHECK_PTR(lpDTimeNode);
            
			MDataType::DSSDateTime lDateTime;
            int lcbData = sizeof(MDataType::DSSDateTime);
            hr = DFCDataConversion::ConvertFromVariant(lpDTimeNode->getRawValue(), DssDataTypeDate, (unsigned char *)&lDateTime, &lcbData);
            CHECK_HR;
            
            //now we should put it in a constant slice
            hr = DSSCalculationHelper::GetConstantSlice(ipCube, 0, 0,
                                                        reinterpret_cast<char *>(&lDateTime), sizeof(MDataType::DateTime),
                                                        DssDataTypeTimeStamp, DssDataOk, opSliceID);
			CHECK_HR;
		}
            break;
            
        case DssNodeConstant:
        {
            EnumDSSDataType lDataType = (EnumDSSDataType)static_cast<DSSExpNodeConstant*>(ipConstNode)->getDataType();
            
            if(lDataType == DssDataTypeUnknown)
                lDataType = DssDataTypeDouble;
            
            CComVariant *lData = static_cast<DSSExpNodeConstant*>(ipConstNode)->getValue();
            CHECK_PTR(lData);
            
            switch(lDataType)
            {
                case DssDataTypeInteger:
                case DssDataTypeDouble:
                case DssDataTypeLong:
                case DssDataTypeBool:
                case DssDataTypeShort:
                case DssDataTypeReal:
                case DssDataTypeFloat:
                case DssDataTypeUnsigned:
                case DssDataTypeNumeric:
                case DssDataTypeDecimal:
                case DssDataTypeInt64: {
                    // Sanity check to make sure the variant is of the right type
                    if (lData->vt != VT_R8)
                    {
                        hr = lData->ChangeType(VT_R8);
                        CHECK_HR;
                    }
                    
                    hr = GetConstantSlice(ipCube,
                                          0, 0,
                                          reinterpret_cast<char *>(&lData->dblVal),
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
                    if (VT_BSTR == lData->vt)
                    {
                        // jimzhang Mar/15/2012 At current, constant string serialized from server side is encoded as wide char.
                        //      As current string conversion on iPad can't support UTF8 and wide char at the same time, convert wide char to UTF8 string here.
                        //      Below convertion logic is just for temporary use, as all strings serialized from server may be changed to UTF8 in the future.
                        
                        char *lpUTF8 = NULL;
                        int lLen = 0;
                        
                        const MBase::ReturnString lRSData(MBase::WideCharToUTF8(lData->bstrVal));
                        lLen = lRSData.GetLength() + sizeof(char);
                        lpUTF8 = new char[lLen];
                        assert(lpUTF8);
                        
                        if (lRSData.c_str() )
                            memmove(lpUTF8, lRSData.c_str(), lLen);
                        else
                            ::memset((void *)lpUTF8, 0, lLen);
                        
                        hr = GetConstantSlice(ipCube,
                                              0, 0,
                                              lpUTF8,
                                              lLen,
                                              DssDataTypeVarChar,
                                              DssDataOk,
                                              opSliceID);
                        CHECK_HR;
                        
                        if(lpUTF8)
                            delete lpUTF8;
                    }
                    
                    break;
                }
                default:
                    return E_FAIL;
            }
        }
            break;
        case DssNodeBigDecimal:
        {
            DSSExpNodeBigDecimal* lCBDNode = static_cast<DSSExpNodeBigDecimal*>(ipConstNode);
            if(!lCBDNode) return E_FAIL;
            
            MDataType::DSSBigDecimal* lpBigDecimal = new MDataType::DSSBigDecimal(lCBDNode->getBigDecimal().c_str());
            MBase::StrongPtr<MDataType::DSSBigDecimal> lpHolder(lpBigDecimal);

            hr = GetConstantSlice(ipCube,
                                  0, 0,
                                  reinterpret_cast<char *>(lpBigDecimal), sizeof(MDataType::DSSBigDecimal),
                                  DssDataTypeBigDecimal,
                                  DssDataOk, opSliceID);
            CHECK_HR;
            
            break;
        }
                default:
            return E_FAIL;
	}
	
	return S_OK;
}

int DSSCalculationHelper::CalculateNewSlice(CalculationContext *ipContext, DSSSliceID *opSliceID)
{
    int hr = S_OK;
	DSSSliceCalculator lCalculator(ipContext);
	hr = lCalculator.CalculateNewSlice(&(opSliceID->mSliceID));
    CHECK_HR;
    opSliceID->mpCube = ipContext->mpCube;
    return hr;
}

int DSSCalculationHelper::CalculateNewSlice(DSSExpNodeOperator *ipOperator, CalculationContext *ipContext, DSSSliceID *opSliceID)
{
    ////////////////////////////////////////////////////////////////////////
	// Given the function operator, cube, target level and the source slices
	// calculate the new slice in the cube
	/////////////////////////////////////////////////////////////////////////
    
	AE_ASSERT(ipOperator);    
    int hr = S_OK;
	//code segment for supporting in and not in operators. They are special operators
	//because there is no such function of in or not in
	//xhan 07/10/2001
	EnumDSSFunction lFuncType = DssFunctionCustom;
    DSSFunction *lpFunction = ipOperator->getFunction();
    if(lpFunction)
    {
        lFuncType = lpFunction->getFunctionType();
        
        // dzhou 2011-07-29 TQMS461880
        
    //	bool lbTrimString = true;
        /*
        CDSSAEHelper::ReadTrimStringFlag(ipReportInstance, lbTrimString);
        */
        // qjiang 03/27/2009 TQMS 350820
        // treate Equal/NotEqual as In/NotIn
        DSSFunctionServer *lpFuncServer = new DSSFunctionServer();
        MBase::DSSStrongPtr<DSSFunctionServer> lFunctionServerPtr(lpFuncServer);
        
        if (lFuncType == DssFunctionIn || lFuncType == DssFunctionNotIn || lFuncType == DssFunctionEquals || lFuncType == DssFunctionNotEqual)
        {
            int lnSlices = ipContext->mvSliceID.size();
            DSSSliceID *lpSliceIDs = new DSSSliceID[lnSlices];
            MBase::DSSStrongPtr<DSSSliceID, MBase::DeleteArray<DSSSliceID> > lSliceIDsPtr(lpSliceIDs);
            for (int i = 0; i < lnSlices; i++)
            {
                lpSliceIDs[i] = *(ipContext->mvSliceID[i]);
            }
            
            return CalculateIn(lpFuncServer, lFuncType, ipContext->mpCube, lnSlices, lpSliceIDs,
                               opSliceID, ipContext->mNullCheckingOption, ipContext->mpSchema);
        }
        
        DSSFunctionObject *lpFunctionObject = NULL;
        GUID lPackageGUID = lpFunction->getPackageID();
        lpFunctionObject = lpFuncServer->FindFunctionObject(lpFunction->getFunctionIndex(), &lPackageGUID);
        if(FAILED(hr) || !lpFunctionObject)
        {
            //TQMS 891174, show NULL for functions not supported
            if(!lpFunctionObject)
            {
                return E_FAIL;
            }
            return hr;
        }
        
        // kshi 2011-07-29 TQMS461880
        /*
        hr = CDSSAEHelper::SetTrimStringFlag(lpFunctionObject, lbTrimString);
         */
        //CHECK_HR;
        
        // the user specified properties are recorded at the operator node
        // need to go through the property list to set the function properties
        std::vector<CComVariant> lpPropertySet = ipOperator->getFunctionPropertyVec();
        
        Int32 lCount = lpPropertySet.size();
        
        // qxue(7/20/2006): these are new OLAP functions that require to check the Distinct property
        // aye, 217629, 6/10/2008
        // Support for Count(Distinct)
        // xiyu,338907,12/12/2008, Support Distinct, Null for Max/Min
        // fali, 820468, support Sum(Distinct)
        if(lFuncType == DssFunctionOLAPSum || lFuncType == DssFunctionOLAPAvg || lFuncType == DssFunctionOLAPCount || lFuncType == DssFunctionCount || lFuncType == DssFunctionOLAPMax || lFuncType==DssFunctionOLAPMin || lFuncType == DssFunctionSum)
        {
            // 05/14/2008 mwang get the property of Distinct
            CComVariant lVariant = lpPropertySet[0];
            lpFunctionObject->setProperty(-1, &lVariant);
            
            // 05/14/2008 mwang get the property of CountNull
            lVariant = lpPropertySet[1];
            lpFunctionObject->setProperty(-2, &lVariant);  // countNull
        }
        
        // the first three properties are system defined properties which do not belong
        // to any particular function.
        for(Int32 i = DssFunctionPropertySystemCount+1; i <= lCount; i++)
        {
            CComVariant lVariant = lpPropertySet[i-1];
            hr = lpFunctionObject->setProperty(i-DssFunctionPropertySystemCount, &lVariant);
        }
        
        //tliao, 812084, special handling for Rank function on metric slider for we can't get the properties from lpPropertySet
        if (lCount <= DssFunctionPropertySystemCount && ipOperator->getOperatorType() == DssFunctionRank)
        {
            DSSRankFunction *lpRankFunciton = dynamic_cast<DSSRankFunction*>(lpFunctionObject);
            CHECK_PTR(lpRankFunciton);
            CComVariant lVariant(ipOperator->getAscending());
            lpRankFunciton->SetPropertyAsc(&lVariant);
            CComVariant lVariant2(ipOperator->getByValue());
            lpRankFunciton->SetPropertyByValue(&lVariant2);        
        }
        
        ipContext->mpFuncObject = lpFunctionObject;
    }
    return CalculateNewSlice(ipContext, opSliceID);
}


//try to determine the lowest common level of input slices
// for example if slice1 at level {A, B} and slice2 at level {B, C}, then 
//the output level is {A, B, C}
int DSSCalculationHelper::GetUnionLevel(vector<DSSCubeDataSlice*> &irvSlice, vector<int> &orvUnits)
{
	int hr = S_OK;
	
	set<int> lUnitList;
	
	int lnSlices = irvSlice.size();
	for(int iSlice = 0; iSlice < lnSlices; iSlice ++)
	{
		DSSCubeDataSlice *lpCubeSlice = irvSlice[iSlice];
		CHECK_PTR(lpCubeSlice);
		
		DSSTabularIndex *lpIndex = lpCubeSlice->getIndices();
		if(!lpIndex)//it is possible that the cube slice is a constant slice which doesn't have a index
            continue;
        
		int lnUnits = lpIndex->getNumUnits();
		vector<int> lvUnits(lnUnits);
		hr = lpIndex->getUnits(lnUnits, lnUnits > 0 ? &lvUnits[0] : NULL);
		CHECK_HR;
		
		for(int iUnit = 0; iUnit < lnUnits; iUnit++)
			lUnitList.insert(lvUnits[iUnit]);
	}

	int count = lUnitList.size();
	orvUnits.reserve(count);
	set<int>::iterator it = lUnitList.begin();
	for (; it != lUnitList.end(); it++)
	{
		orvUnits.push_back(*it);
	}
	
	return S_OK;
}

int DSSCalculationHelper::GetSliceFromID(int nSlices, DSSSliceID *ipSliceIDs, MBase::Buffer *ipBuffer, DSSCubeDataSlice ** &opSlices)
{
    int hr = S_OK;
    
	opSlices = new (ipBuffer) DSSCubeDataSlice* [nSlices];
	CHECK_NEWED_PTR(opSlices);
	for (int iSlice = 0; iSlice < nSlices; iSlice ++)
	{
		DSSCubeDataSlice* lpSlice;
		hr = ipSliceIDs[iSlice].GetSlice(&lpSlice);
		CHECK_HR_PTR(lpSlice);
        
		opSlices[iSlice] = lpSlice;
	}
    
	return S_OK;
}

int DSSCalculationHelper::CalculateIn(DSSFunctionServer     *ipFuncServer,
                                      EnumDSSFunction       iFuncType,
                                      DSSCube               *ipCube,
                                      Int32                 inSliceIDs,
                                      DSSSliceID            *ipSliceIDs,
                                      DSSSliceID            *opResultSlice,
                                      EnumNullCheckingOption iNULLChecking,
                                      MCESchema *ipSchema,
                                      bool isDSTrx)
{
	if (inSliceIDs < 2)
		return E_FAIL;
    
    EnumNullCheckingOption lNullChecking = DssNumericErrorSkip;
	// qjiang 03/27/2009 TQMS 350820
	// we do Null checking on Equal/NotEqual
	if (iFuncType == DssFunctionEquals || iFuncType == DssFunctionNotEqual)
	{
		lNullChecking = iNULLChecking;
	}
    
	// The slice that contains the in-list result
	DSSSliceID lOrSliceID;
	lOrSliceID.mSliceID = -1;
    bool lIsEqualsOrIn = iFuncType == DssFunctionEquals || iFuncType == DssFunctionIn;
	int hr = hTryBinarySearchForInList(ipCube, inSliceIDs, ipSliceIDs, &lOrSliceID, iNULLChecking, true, lIsEqualsOrIn, isDSTrx);
	CHECK_HR;
    
	if (lOrSliceID.mSliceID == -1)
	{
		//test equal first
        GUID lBuildInPackageGUID = {0xACC5BFA0,0x11D3,0xDD9C,{0xC0,0x00,0x81,0xB9,0xEA,0x33,0x22,0x4F}};
        DSSFunctionObject *lpFunctionEqObject = ipFuncServer->FindFunctionObject(lIsEqualsOrIn ? DssFuncEquals:DssFuncNotEqual, &lBuildInPackageGUID);
		CHECK_PTR(lpFunctionEqObject);
        MBase::DSSStrongPtr<DSSFunctionObject> lFunctionEqObjectPtr(lpFunctionEqObject);
		// dzhou 2011-07-29 TQMS461880
		//hr = CDSSAEHelper::SetTrimStringFlag(lpFunctionEqObject, ibTrimString);
		// do not need to check here. 
		//CHECK_HR;
        
		DSSSliceID * lpEqualSliceID = new DSSSliceID[inSliceIDs - 1];
		CHECK_NEWED_PTR(lpEqualSliceID);
        MBase::DSSStrongPtr<DSSSliceID, MBase::DeleteArray<DSSSliceID> > lEqualSliceIDPtr(lpEqualSliceID);
        
		Int32 i = 0;
		DSSSliceID lParam[2];
		for(i = 1; i < inSliceIDs; i ++){
			lParam[0] = ipSliceIDs[0];
			lParam[1] = ipSliceIDs[i];

            vector<DSSSliceID*> lvSliceID;
            lvSliceID.push_back(&lParam[0]);
            lvSliceID.push_back(&lParam[1]);
            
            CalculationContext *lCalcContext = DSSCalculationHelper::getCalculationContext(lpFunctionEqObject,
                                                                                           lpFunctionEqObject->getOperatorType(),
                                                                                           ipCube,
                                                                                           NULL,
                                                                                           &lvSliceID,
                                                                                           iNULLChecking,
                                                                                           ipSchema,
                                                                                           NULL,
                                                                                           NULL,
                                                                                           false,
                                                                                           isDSTrx);
            MBase::DSSStrongPtr<CalculationContext> lCalcContextPtr(lCalcContext);
            
            hr = DSSCalculationHelper::CalculateNewSlice(lCalcContext, &(lpEqualSliceID[i-1]));
            CHECK_HR;
		}
        
		//do or
		DSSFunctionObject *lpFunctionOrObject = ipFuncServer->FindFunctionObject(lIsEqualsOrIn ? DssFuncOr : DssFuncAnd, &lBuildInPackageGUID);
		CHECK_PTR(lpFunctionOrObject);
        MBase::DSSStrongPtr<DSSFunctionObject> lFunctionOrObjectPtr(lpFunctionOrObject);
        
		// dzhou 2011-07-29 TQMS461880
		//hr = CDSSAEHelper::SetTrimStringFlag(lpFunctionEqObject, ibTrimString);
		// do not need to check here. 
		//CHECK_HR;
        
		lOrSliceID = lpEqualSliceID[0];
		for(i = 1; i < inSliceIDs - 1; i ++){
			lParam[0] = lOrSliceID;
			lParam[1] = lpEqualSliceID[i];

            vector<DSSSliceID*> lvSliceID;
            lvSliceID.push_back(&lParam[0]);
            lvSliceID.push_back(&lParam[1]);
            
            CalculationContext *lCalcContext = DSSCalculationHelper::getCalculationContext(lpFunctionOrObject,
                                                                                           lpFunctionOrObject->getOperatorType(),
                                                                                           ipCube,
                                                                                           NULL,
                                                                                           &lvSliceID,
                                                                                           iNULLChecking,
                                                                                           ipSchema);

            MBase::DSSStrongPtr<CalculationContext> lCalcContextPtr(lCalcContext);
            hr = DSSCalculationHelper::CalculateNewSlice(lCalcContext, &(lOrSliceID));
            CHECK_HR;
		}
	}
    
    //tliao, 948231, 2014/08/26, WHNULL needs to be filtered out for NotIn function
    if (iFuncType==DssFunctionNotIn)
    {
        DSSCubeDataSlice* lpMetricSlice = NULL;
        hr = ipSliceIDs[0].GetSlice(&lpMetricSlice);
        CHECK_HR;
        
        DSSDataColumn* lpColumnC = lpMetricSlice->getColumn();
        Int32 lnRows = lpColumnC->getRowCount();
        
        if (lnRows > 0)
        {
            int lineNo = -1;
            
            for (int iRow = 0; iRow < lnRows; iRow++)
            {
                DSSDataFlag lFlag = DssDataOk;
                hr = lpColumnC->getFlag(iRow, lFlag);
                CHECK_HR;
                
                if (lFlag == DssDataNull)
                {
                    lineNo = iRow;
                    break;
                }
            }
            
            if (lineNo != -1)
            {
                DSSCubeDataSlice* lpResultSlice;
                hr = lOrSliceID.GetSlice(&lpResultSlice);
                CHECK_HR;
                
                DSSDataColumn* lpResultColumn = lpResultSlice->getColumn();
                CHECK_PTR(lpResultColumn);
                
                
                bool lVal = false;
                
                hr = lpResultColumn->setData(lineNo, (char*)&lVal, sizeof(bool), DssDataOk);
                CHECK_HR;
            }
        }
    }
    /* tliao: 11/06/2013, we will calculate it directly in above steps, because lpColumn -> getRawDataAndFlags(..) can't work as expected when row number is large
	//if it is not in, do not
	// qjiang 03/27/2009 TQMS 350820
	// treate Equal/NotEqual as In/NotIn
	if (iFuncType == DssFunctionIn || iFuncType == DssFunctionEquals) {
		*opResultSlice = lOrSliceID;
	} else {
		// qjiang 09/01/2009
		// since function "Not" does not perform well, we'll directly do a "Not" operation on the result slice
        DSSCubeDataSlice* lpSlice = NULL;
		hr = lOrSliceID.GetSlice(&lpSlice);
		CHECK_HR_PTR(lpSlice);
        
		DSSDataColumn *lpColumn = lpSlice -> getColumn();
        
		void* lpData = NULL;
		DSSData_Flags* lpFlags = NULL;
		hr = lpColumn -> getRawDataAndFlags(&lpData, &lpFlags);
		CHECK_HR;
        
		if (lpData == NULL || lpFlags == NULL)
			return S_FALSE;
		
		DSSDataType_Type lType = lpColumn -> getDataType();
        
		Int32 lnRows = lpColumn -> getRowCount();
        
		hr = hDirectCalculateNot(lnRows, lpData, lType, lpFlags, lNullChecking);
		CHECK_HR;
        
		*opResultSlice = lOrSliceID;
	}
    */
    *opResultSlice = lOrSliceID;
	return hr;
}

int DSSCalculationHelper::hTryBinarySearchForInList(DSSCube *ipCube,
                                                    Int32 inSliceIDs,
                                                    DSSSliceID *ipSliceIDs,
                                                    DSSSliceID *opResultSliceID,
                                                    EnumNullCheckingOption iNullCheckingOption,
                                                    bool ibTrimString,
                                                    bool ibIsEqualsOrIn,
                                                    bool isDSTrx)
{
	// aye, 1/28/08
	// use a new algorithm to calculate the in-list operation.
	// A(a1, a2, ..., an) in (a1', a2', ..., ak')	
	// If k > 2, We first check if (a1 ,..., an) are sorted. If true, use binary search
	// 
	// The check adds n comparisons, but give tremendous benefit
	Int32 i = 0;
    
	HRESULT hr = S_OK;
    
	// First check if it makes sense to do binary search. If the number of elements in the in-list 
	// is small, don't do it.
    
	// Next check the first slice to see if it's sorted.
	DSSCubeDataSlice* lpMetricSlice = NULL;
	hr = ipSliceIDs[0].GetSlice(&lpMetricSlice);
	CHECK_HR;
    
	DSSDataColumn* lpColumnC = lpMetricSlice->getColumn();
    bool lfUseRefColumn = false; //hard code
    /*
     CDSSXTabColumn* lpColumnC = NULL;
     
     // qjiang TQMS 06/08/2009 TQMS 348157
     // first check whether there is referenced column
     hr = lpMetricSlice->get_RefColumnC(&lpColumnC);
     CHECK_HR;
     
     bool lfUseRefColumn = true;
     
     if (lpColumnC == NULL)
     { 
     // no reference column, the original column must be there
     hr = lpMetricSlice -> get_ColumnC(&lpColumnC);
     CHECK_HR_PTR(lpColumnC);
     lfUseRefColumn = false;
     }
    // no matter where the colum comes from, we need to check the sort status, to make sure our binary search work
	XTabColumnSortStatus lStatus = lpColumnC->GetSortStatus();
	if (lStatus == XTabColumnNotSorted)
		return S_OK;
    
	// qjiang 03/17/2011 if column is compressed, don't do binary search on it
	if (lpColumnC->GetOffsets() != NULL)
		return S_OK;
    */
    DSSCubeDataSlice* lpSlice = NULL;
	// for our optimization to work, the first slice is MetricSlice, the rests should be constant	
	for (i = 1; i < inSliceIDs; i++)
	{
        
		hr = ipSliceIDs[i].GetSlice(&lpSlice);
		CHECK_HR;
        
		DSSDataColumn* lpColumn = lpSlice->getColumn();
        
		Int32 lnRows = lpColumn->getRowCount();
        
		if (lnRows != 1)
			return S_OK;
	}
    
    // Next check the first slice to see if it's sorted.
    // Check if each row is less than the next one
    Int32 lnRows = lpColumnC->getRowCount();
    for (i = 1; i < lnRows; i++)
    {
        XTabCellData lData1;
        XTabCellData lData2;
        
        hr = lpColumnC -> getCellData(i - 1, lData1);
        CHECK_HR;
        
        hr = lpColumnC -> getCellData(i, lData2);
        CHECK_HR;
        
        // aye, todo, figure out how to handle null value. For now, assume null is the
        // first value
        if ((lData1.mStatus != DssDataOk && (i != 1 || (i == 1 && isDSTrx)))|| lData2.mStatus != DssDataOk)
            break;
        
        // For the null value case, as it only appears as the first element, skip it
        if (lData1.mStatus != DssDataOk)
            continue;
        
        // weiwang 2009-07-03 change back to variant comparison
        CComVariant lVar1;
        HRESULT hr = DFCDataConversion::ConvertToVariant(lData1.mpData, lData1.mDataType, lData1.mSize, &lVar1);
        if (hr != S_OK)
            break;
        
        CComVariant lVar2;
        hr = DFCDataConversion::ConvertToVariant(lData2.mpData, lData2.mDataType, lData2.mSize, &lVar2);
        if (hr != S_OK)
            break;
        
        DSSData_Flags lFlag = DssDataOk;
        // dzhou 2011-07-29 TQMS461880
        if (variantCompare(lVar1, lVar2, &lFlag, false, ibTrimString) >= 0)
            break;
        
    }
    
    // if we come here without going through all loops, this column is not sorted
    if (i < lnRows)
        return S_OK;

    /*
    // Next check the first slice to see if it's sorted.
	if (lStatus == XTabColumnSortUnknown)
	{  
		Int32 lnRows = 0;
		hr = lpColumnC->get_RowCount(&lnRows);
		CHECK_HR;
        
		// Check if each row is less than the next one
		for (i = 1; i < lnRows; i++)
		{
			XTabCellData lData1;
			XTabCellData lData2;
            
			hr = lpColumnC -> GetCellData(i - 1, lData1);
			CHECK_HR;
            
			hr = lpColumnC -> GetCellData(i, lData2);
			CHECK_HR;
            
			// aye, todo, figure out how to handle null value. For now, assume null is the
			// first value
			if ((lData1.mStatus != DssDataOk && i != 1) || lData2.mStatus != DssDataOk)
				break;
            
			// For the null value case, as it only appears as the first element, skip it
			if (lData1.mStatus != DssDataOk)
				continue;
            
			// weiwang 2009-07-03 change back to variant comparison
			CComVariant lVar1;
			HRESULT hr = DFCData_ConvertToVariant(lData1.mpData, lData1.mDataType, lData1.mSize, &lVar1);
			if (hr != S_OK)
				break;
            
			CComVariant lVar2;
			hr = DFCData_ConvertToVariant(lData2.mpData, lData2.mDataType, lData2.mSize, &lVar2);
			if (hr != S_OK)
				break;
            
			DSSData_Flags lFlag = DssDataOk;
			// dzhou 2011-07-29 TQMS461880
			if (variantCompare(lVar1, lVar2, &lFlag, false, ibTrimString) >= 0)
				break;
            
		}
        
		// if we come here without going through all loops, this column is not sorted
		if (i < lnRows)
		{
			lpColumnC->SetSortStatus(XTabColumnNotSorted);
			return S_OK;
		}
		else
			lpColumnC->SetSortStatus(XTabColumnSorted);		
	}
    */
    
	// Once we come to this point, we know for sure that the first slice is sorted, and the rests are constants.
	// Generate a result slice and initialize it to false
	DSSCubeDataSlice* lpResultSlice;
	
	hr = ipSliceIDs[0].GetSlice(&lpSlice);
	CHECK_HR;
    
	DSSTabularIndex* lpIndex = lpSlice->getIndices();
    
	Int32 lnUnits = lpIndex->getNumUnits();
    
	// Sometime SQL engine passes strange in-list such as '1' in '1'. We don't do binary search 
	// for it. 
	if (lnUnits != 1)
		return S_OK;
    
	vector<Int32> lvUnits(lnUnits);
	hr = lpIndex->getUnits(lnUnits, &lvUnits[0]);
	CHECK_HR;
    
	Int32 lnTotalUnits = 0;
    DSSTabularData* lpTabularData = ipCube->getTabularData();
    CHECK_PTR(lpTabularData);
    
    lnTotalUnits = lpTabularData->Count();
    
	hr = ipCube->CreateDataSlice(lnUnits, &lvUnits[0], DssDataTypeBool, &lpResultSlice);
	CHECK_HR;
    
	DSSDataColumn* lpResultColumn = lpResultSlice->getColumn();
	CHECK_PTR(lpResultColumn);
    
	lnRows = lpResultColumn->getRowCount();
    
	for (i = 0; i < lnRows; i++)
	{
		bool lVal = !ibIsEqualsOrIn;
		hr = lpResultColumn->setData(i, (char*)&lVal, sizeof(bool), DssDataOk);
		CHECK_HR;
	}
	
	vector<DSSXTabKey> lvUnitKeys (lnTotalUnits);
	DSSXTabKey* lpUnitKeys= &lvUnitKeys[0];
    
	bool lfFirstIsNull = false;
    
	XTabCellData lData;
	lpColumnC->getCellData(0, lData);
	if (lData.mStatus != DssDataOk)
	{
		lfFirstIsNull = true;
	}
    
	CComVariant lVariantZero;
	double lZero = 0.0;
	DFCDataConversion::ConvertToVariant((unsigned char*)&lZero, DssDataTypeDouble, sizeof(double), &lVariantZero);
    
	// row number in column may be more than that in index
    // tliao, 09/24/2013, the mRowCount is different from the real RowCount, it contains join-null
	Int32 lnRowInColumn = lnRows;//lpColumnC->getRowCount();
    /*
    // aye, 7/3/2009
	// In case the index is not distinct, we need to use a bit mask to record if a row in the column 
	// is selected. 
	bool lIsIndexDistinct = static_cast<CDSSXTabIndices*> (lpIndex.p)->IsDistinct();
	CDSSXTabBitVector lMask(lnRowInColumn);
    */
    bool lIsIndexDistinct = true; //hard code
	DSSXTabBitVector lMask(lnRowInColumn);
	// For the elements in the in-list, find the position in the slice and set it to true
	for (i = 1; i < inSliceIDs; i++)
	{
		DSSCubeDataSlice* lpConstantSlice;
		hr = ipSliceIDs[i].GetSlice(&lpConstantSlice);
		CHECK_HR;
        
		DSSDataColumn* lpConstantColumn = lpConstantSlice->getColumn();
        
		XTabCellData lCellData;
		hr = lpConstantColumn->getCellData(0, lCellData);
		CHECK_HR;
		
        bool lbNullVal = false;
		// qjiang TQMS 350820
		// Null checking for equal/not equal
		if (lfFirstIsNull && (iNullCheckingOption == DssNumericErrorTreatAs || isDSTrx /*975900*/))
		{
            CComVariant lElement;
			DFCDataConversion::ConvertToVariant(lCellData.mpData, lCellData.mDataType, lCellData.mSize, &lElement);
			DSSData_Flags lFlag = DssDataOk;
			// dzhou 2011-07-29 TQMS461880
			if (variantCompare(lElement, lVariantZero, &lFlag, false, ibTrimString) == 0)
			{
                lbNullVal = true;
                bool lVal = isDSTrx ? false /*989127 & 993234*/: ibIsEqualsOrIn;
				Int32 lRow = 0;
				if (!lIsIndexDistinct)
				{
					// jxie, 2010-9-9, TQMS413071
					if (lfUseRefColumn)
						lMask.Set(0, true);
					else
					{
						// if don't use refColumn, just set index for lpResultColumn
						hr = lpResultColumn->setData(0, (char*)&lVal, sizeof(bool), DssDataOk);
						CHECK_HR;
					}
				}
				else
				{
					if (lfUseRefColumn)
					{
						lpUnitKeys[lvUnits[0]] = 0;
						hr = lpIndex->LookUp(lnTotalUnits, lpUnitKeys, &lRow);
						if (hr == S_OK)
						{
							hr = lpResultColumn->setData(lRow, (char*)&lVal, sizeof(bool), DssDataOk);
							CHECK_HR;
						}
					}
					else
					{
						hr = lpResultColumn -> setData(0, (char*)&lVal, sizeof(bool), DssDataOk);
						CHECK_HR;
					}
				}
			}
		}
        
		Int32 lIndex = FindRowIndexByCellData(&lCellData, 
                                                &lpColumnC, 
                                                1, 
                                                lnRowInColumn, 
                                                true, 
                                                NULL, 
                                                0);
		if (lIndex >= 0 && !(isDSTrx && lbNullVal))
		{
			bool lVal = ibIsEqualsOrIn;
            if (!lIsIndexDistinct)
			{
				// jxie, 2010-9-9, TQMS413071
				if (lfUseRefColumn)
					lMask.Set(lIndex, true);
				else
				{
					// if don't use refColumn, just set index for lpResultColumn
					hr = lpResultColumn->setData(lIndex, (char*)&lVal, sizeof(bool), DssDataOk);
					CHECK_HR;
				}
			}
			else
			{
				if (lfUseRefColumn)
				{
					// here the order of the column and index may be different, need to do a lookup
					lpUnitKeys[lvUnits[0]] = lIndex;
					hr = lpIndex->LookUp(lnTotalUnits, lpUnitKeys, &lIndex);
					if (hr != S_OK)
					{	// not find in index, continue
						continue;
					}
				}
                
				hr = lpResultColumn->setData(lIndex, (char*)&lVal, sizeof(bool), DssDataOk);
				CHECK_HR;
			}
		}
	}
    
    // jxie, 2010-9-9, TQMS413071
	if (!lIsIndexDistinct && lfUseRefColumn)
	{
		// do one pass through the index to set the result column
		DSSXTabKeyIterator lIt(lpIndex);
		Int32 lnRows = lpIndex->Count();
		bool lVal = ibIsEqualsOrIn;
		for (i = 0; i < lnRows; i++)
		{
			Int32 lKey = lIt.GetCol(0);
			lIt.NextRow();		
			if (lMask.Get(lKey))
			{
				hr = lpResultColumn->setData(i, (char*)&lVal, sizeof(bool), DssDataOk);
				CHECK_HR;
			}
		}
	}
    
	hr = ipCube->InsertDataSlice(lpResultSlice, opResultSliceID->mSliceID);
	CHECK_HR;
    
	opResultSliceID->mpCube = ipCube;
    
	return S_OK;
}

// aye, 1/28/2008
// add a static version so that we can call from SliceCalculator
// weiwang 2009-06-22 Add parameter to support adding element to lookup for direct loading.
// opIndex is meaningfull when function returns -1, which means that we don't find the elements in mpTable,
// and the new element should be inserted right after row opIndex of mpTable.
Int32 DSSCalculationHelper::FindRowIndexByCellData(XTabCellData* ipKeys, DSSDataColumn** ippColumns, Int32 inKeys, 
                                            Int32 inRows, bool iUseVariant, Int32* opIndex, Int32 iStartIndex, bool ibTrimString)
{
    int hr = S_OK;
	// First check our Key columns to see if they are sorted. Assert if we are not.
    
	// 	Given an ID Value,  find its index in a sorted list of IDValues, 
	Int32 lIndex = (iStartIndex + inRows) / 2;	
    
	// The upper bound and lower bound are the range of number that we have not 
	// tested. 
	Int32 lUpperBound = inRows - 1;
	Int32 lLowerBound = iStartIndex;
	Int32 i = 0;
    
	// weiwang 2009-07-03
	Int32* lpSortOrder = NULL;
	if (iUseVariant)	// binary search for Inlist
	{
		AE_ASSERT(inKeys == 1);
		lpSortOrder = ippColumns[0]->GetInputOrder();
	}
    
	while (lUpperBound >= lLowerBound)
	{
		Int32 lCompareResult = 0;
        
		// Compare each key until we find the first unequal one
		for (i = 0; i < inKeys; i++)
		{
			DSSDataColumn *lpCol = ippColumns[i];		
			Int32 lResult = 0;
			
			if (!iUseVariant)
            {
                lResult = lpCol->CompareToData(lIndex, ipKeys[i].mpData, ipKeys[i].mSize, ipKeys[i].mStatus, true, true);
            }
			else
			{
				// The variant version of the compare are slightly different 
				// Most notably, 1) 1E-10 == 0, 2) "abc " == "abc"
				// This is used by the function server to perform equal checks for in-list
				// Now when moving the in-list calculation to binary search, we do the same here.
				XTabCellData lData;
				lpCol->getCellData((lpSortOrder ? lpSortOrder[lIndex] : lIndex), lData);
                
				// aye, 8/8/8, null comparison support	
				DSSData_Flags lFlag1 = lData.mStatus;
				DSSData_Flags lFlag2 = ipKeys[i].mStatus;
				if (lFlag1 == DssDataOk && lFlag2 == DssDataOk)
				{
					CComVariant lVar1;
					HRESULT hr = S_OK;
                    
					hr = DFCDataConversion::ConvertToVariant(lData.mpData, lData.mDataType, lData.mSize, &lVar1);
					if (hr != S_OK)
						return -1;
                    
					CComVariant lVar2;
                    
					hr = DFCDataConversion::ConvertToVariant(ipKeys[i].mpData, ipKeys[i].mDataType, lData.mSize, &lVar2);
					if (hr != S_OK)
						return -1;
                    
					DSSData_Flags lFlag = DssDataOk;
					// dzhou 2011-07-29 TQMS461880
					lResult = variantCompare(lVar1, lVar2, &lFlag, false, ibTrimString);
					// gzhou 01/28/2009: TQMS#347151--If lFlag is not DssDataOk, something is wrong already.
					if (lFlag == DssDataInvalid)
						return -1;
				} else
				{
					if (lFlag1 == lFlag2)
						lResult = 0;
					else if (lFlag2 == DssDataOk)
						lResult = -1;
					else
						lResult = 1;
				}
			}
            
			if (lResult != 0) {
				lCompareResult = lResult;
				break;
			}
		}
        
		if (lCompareResult == 0)
			return (lpSortOrder ? lpSortOrder[lIndex] : lIndex);
		
		if (lCompareResult < 0) {
			lLowerBound = lIndex + 1;
		} else {
			lUpperBound = lIndex - 1;
		}		
		lIndex = (lUpperBound + lLowerBound) / 2 ;
	} 
    
	// weiwang 2010-07-13 TQMS420350 Not found. For data population, need to check the previous and
	// the next ones to see if equal using variant comparison, since redundents using variant comparison
	// have been removed during Lookup population.
	if (opIndex)	// data populate case
	{
		// when come to here, rows <= lUpperBound is less than passed in. rows > lUpperBound is greater than passed in.
		Int32 lTempOffset[2];
		lTempOffset[0] = lUpperBound;		// previous one
		lTempOffset[1] = lUpperBound + 1;	// next one
		for (Int32 k = 0; k < 2; k++)
		{
			if (lTempOffset[k] < 0 || lTempOffset[k] >= inRows) continue;
            
			for (i = 0; i < inKeys; i++)
			{
				DSSDataColumn *lpCol = ippColumns[i];		
				Int32 lResult = 0;
                
				// find out the columns we need to use variant comparison
				EnumDSSDataType lType = lpCol->getDataType();
                
				if (lType == DssDataTypeVarChar || lType == DssDataTypeMBChar
					|| lType == DssDataTypeUTF8Char || lType == DssDataTypeBinary || lType == DssDataTypeBigDecimal)
				{
					XTabCellData lData;
					lpCol->getCellData(lTempOffset[k], lData);
                    
					DSSData_Flags lFlag1 = lData.mStatus;
					DSSData_Flags lFlag2 = ipKeys[i].mStatus;
					if (lFlag1 == DssDataOk && lFlag2 == DssDataOk)
					{
						CComVariant lVar1;
						hr = DFCDataConversion::ConvertToVariant(lData.mpData, lData.mDataType, lData.mSize, &lVar1);
						if (hr != S_OK)
							return -1;
                        
						CComVariant lVar2;
						hr = DFCDataConversion::ConvertToVariant(ipKeys[i].mpData, ipKeys[i].mDataType, lData.mSize, &lVar2);
						if (hr != S_OK)
							return -1;
                        
						DSSData_Flags lFlag = DssDataOk;
						//dzhou 2011-07-29 TQMS461880 
						lResult = variantCompare(lVar1, lVar2, &lFlag, true, ibTrimString);
						// gzhou 01/28/2009: TQMS#347151--If lFlag is not DssDataOk, something is wrong already.
						if (lFlag == DssDataInvalid)
							return -1;
                        
						if (lResult != 0)		// not equal
							break;
					}
					else if (lFlag1 != lFlag2)	// not equal
						break;
				}
				else
				{
					lResult = lpCol->CompareToData(lTempOffset[k], ipKeys[i].mpData, ipKeys[i].mSize, ipKeys[i].mStatus, true, true);
					if (lResult != 0)	// not equal
						break;
				}
			}
            
			if (i == inKeys)	// each column is equal, we found the one we need
				return lTempOffset[k];
		}
        
		// not found even in the previous and next ones
		*opIndex = lUpperBound;		// the lUpperBound is the place new element should be inserted.
	}
    
	// We have not found the value. return -1.
	return -1;
}

//fali, ipContainer is used to convert local key to global key. it should be null if ipCube already uses global index
int DSSCalculationHelper::GetPropertySlice(DSSCube *ipCube, Int32 iUnit, Int32 iForm, Int32 iLocale, DSSSliceID *pSliceID, bool iFailDE, bool iLevelSignificant, DSSGUnitContainer* ipContainer)
{    
    int hr = S_OK;
    DSSTabularData* lpTabularData = ipCube->getTabularData();
    CHECK_PTR(lpTabularData);
    DSSTabularUnit* lpTabularDataUnit = lpTabularData->Item(iUnit);
    CHECK_PTR(lpTabularDataUnit);
    
    //xzhu,08/04/2014,delete this codes
    //if (ipContainer == NULL) {
        //ipContainer = lpTabularData->getGUnitContainer();
    //}
    
	DSSTabularAttribute* lpXTabLookUpTable;
	//get lookup to find the form
    
	EnumDSSTemplateUnitType lUnitType = lpTabularDataUnit->getUnitType();
    
	//lwang, 11/29/2007, TQMS 244359,250668, apply threshold condition on DE;
	//TQMS 290121, apply filter condition on DE;
	if (lUnitType == DssTemplateAttribute)
	{
		lpXTabLookUpTable = (DSSTabularAttribute*)(lpTabularDataUnit);
	}
	else if (lUnitType == DssTemplateCustomGroup && !iFailDE)
	{
//		CComQIPtr<ICDSSXTabCustomGroup, &IID_ICDSSXTabCustomGroup> lpTabCustomGroup(lpTabularDataUnit);
//		CHECK_PTR(lpTabCustomGroup);
//        
//		hr = lpTabCustomGroup->get_LookUpTable(0, &lpXTabLookUpTable);
//		CHECK_HR_PTR(lpXTabLookUpTable);
	}
	else
	{
		//lwang, 05/13/2008, TQMS 290121, temporarily disable filter condition on DE
	}
    
	// aye, 290453, use Locale information to find forms
	Int32 lColumn = -1;
	//ctang 2011-2-28 in the case we construct prpperty slice from relation, the iForm will be -1
	if (iForm >= 0)
	{
        hr = lpXTabLookUpTable -> FindFormWithLocale(0, iForm, iLocale, lColumn);		//Need to change to FindFormIWithLocale later
        CHECK_HR;	
	}
	
	pSliceID->mpCube = ipCube;
    
	return ipCube->getAttrFormSlice(iUnit, lColumn, &(pSliceID->mSliceID), iLevelSignificant, ipContainer);
}

int DSSCalculationHelper::ApplyRelationshipFilter(Int32 nDimUnits, 
                                                  Int32 *ipUnitIDs, 
                                                  DSSCubeDataSlice *ipFilterSlice, 
                                                  DSSCube *ipTargetCube,
                                                  DSSSliceID *opResultSliceID)
                                                  //MBase::Buffer * ipBuffer)
{
	HRESULT hr = S_OK;
    
	//Get filter slice attribute level
	DSSTabularIndex* lpFilterIndex = ipFilterSlice->getIndices();
	CHECK_PTR(lpFilterIndex);
    
	Int32 lnFilterUnit = lpFilterIndex->getNumUnits();
    
	Int32 * lpFilterUnit = new Int32[lnFilterUnit];
	CHECK_NEWED_PTR(lpFilterUnit);
    MBase::DSSStrongPtr<Int32, MBase::DeleteArray<Int32> > lFilterUnitPtr(lpFilterUnit);
    
	hr = lpFilterIndex->getUnits(lnFilterUnit, lpFilterUnit);
	CHECK_HR;
    
    
	// Build up relational table between attributs and filter slice, generate new slice at the level of attributes
	Int32 *lpRelationUnits = new Int32 [nDimUnits + lnFilterUnit];
    MBase::DSSStrongPtr<Int32, MBase::DeleteArray<Int32> > lRelationUnitsPtr(lpRelationUnits);
	Int32 *lpUnionLast = std::set_union(ipUnitIDs, ipUnitIDs + nDimUnits,
										lpFilterUnit, lpFilterUnit + lnFilterUnit, lpRelationUnits);
	Int32 lnRelationUnits = lpUnionLast - lpRelationUnits;
    
    
	DSSTabularIndex* lpRelationalTable;
	hr = ipTargetCube->getIndex(lnRelationUnits, lpRelationUnits, &lpRelationalTable);
	CHECK_HR_PTR(lpRelationalTable);
	//CDSSXTabIndexPtr lIndexPtr(lpRelationalTable); // ldong 12/24/2010
    
	//Create result slice 
	DSSCubeDataSlice* lpOutSlice = NULL;
	hr = ipTargetCube->CreateDataSlice(nDimUnits, ipUnitIDs, DssDataTypeBool, &lpOutSlice);
	CHECK_HR_PTR(lpOutSlice);
    
	//Initialize output slice to zero 
	DSSDataColumn* lpOutColumn = lpOutSlice->getColumn();
	CHECK_PTR(lpOutColumn);
	
	Int32 lnOutRow = lpOutColumn->getRowCount();
    
	bool lFalse = false;
	Int32 i = 0;
	for (i = 0; i < lnOutRow; i++)
	{
		hr = lpOutColumn->setData(i, (char *) (&lFalse), sizeof(bool), DssDataOk);
		CHECK_HR;
	}
    
	//look at relation table, produce the result
	Int32 lnRelationRow = lpRelationalTable -> Count();
    
	DSSSliceCorrelator lSliceCorrelator;
	hr = lSliceCorrelator.Init(lpRelationalTable, 1, &ipFilterSlice, lpOutSlice, ipTargetCube);
	CHECK_HR;
    
	for(i = 0; i < lnRelationRow; i++)
	{
		double lVal = 0;
		DSSData_Flags lFlag = DssDataUnknown;
		hr = lSliceCorrelator.GetInputValue(0, i, &lVal, &lFlag);
		CHECK_HR;
        
		if (lFlag == DssDataOk && lVal == 1)
		{
			bool lBoolVal = true;
            
			hr = lSliceCorrelator.SetOutputValue(i, (BYTE*) &lBoolVal, sizeof(bool), DssDataTypeBool, DssDataOk);
			CHECK_HR;
		}	
        
	}
    
	//Insert result slice to target cube
	hr = ipTargetCube->InsertDataSlice(lpOutSlice, opResultSliceID->mSliceID );
	CHECK_HR;
    
	opResultSliceID->mpCube = ipTargetCube;
    
    
	return S_OK;
    
}

// qjiang 09/01/2009 TQMS 361799
// ad-hoc optimization for "Not" operation
// ideally, this optimization should be applied to the function "CDSSSimpleOperatorGeneric", 
// but it is a big change to optimize Generic Function now, leave that for later release
int DSSCalculationHelper::hDirectCalculateNot(Int32 nRows, 
                                              void* ipData,
                                              DSSDataType_Type iType,
                                              DSSData_Flags* ipFlags,
                                              EnumNullCheckingOption iNullCheckingOption)
{
	int hr = S_OK;
	switch(iType)
	{
        case DssDataTypeDouble:
            hr = hCalculateNot(nRows, (double*)ipData, ipFlags, iNullCheckingOption);
            CHECK_HR;
            break;
        case DssDataTypeBool:
            hr = hCalculateNot(nRows, (bool*)ipData, ipFlags, iNullCheckingOption);
            CHECK_HR;
            break;
        case DssDataTypeMissing:	// jxie, TQMS 437507, 2010-12-21
            break;
        default:
            AE_ASSERT(false);
	}
    
	return S_OK;
}

//Strech input slice downward to the target level.
//We assume the input level is higher than the targe level
//An example is used to illustrate the algorithm
// Input slice is at {Year, Product} level, and the target level is {Month Product}
//We first strech input slice column to a {Year, Month, Product} column
//Then the output slice = {Month Product} index + the {Year, Month, Product} Column
int DSSCalculationHelper::StrechSliceDownward(DSSCube * ipCube,
                                              MBase::Buffer * ipBuffer,
                                              DSSCubeDataSlice *pSliceIn, 
                                              Int32 nInputLevel, Int32 *ipInputLevel,
                                              Int32 nTargetLevel, Int32 *ipTargetLevel,
                                              DSSCubeDataSlice ** ppSliceOut)
{
	HRESULT hr = S_OK;
    
    
	//create output slice, which has the same data type as the input slice 
	DSSDataType_Type lSliceDataType = pSliceIn->getDataType();
    
	Int32 lRowSize = 0;
	hr = pSliceIn->getSizeOfData(lRowSize);
	CHECK_HR;
    
	// qjiang 06/06/2009
	// original column may be integer column, while its true type is double, check it
	 DSSDataColumn *lpColIn = pSliceIn->getColumn();
    
	double lDivider = static_cast<DSSDataColumn *>(lpColIn) -> getDivider();
	if (lDivider > 1)
	{
		lSliceDataType = DssDataTypeDouble;
		lRowSize = sizeof(double);
	}
    
	hr = ipCube->CreateDataSlice(nTargetLevel, ipTargetLevel, lSliceDataType,ppSliceOut);
	CHECK_HR_PTR(*ppSliceOut);
    
	//create the lookup index, whose level is the union of the input level and the target level
	Int32 *lpUnion = new (ipBuffer) Int32 [nTargetLevel + nInputLevel];
	Int32 *lpUnionLast = std::set_union(ipTargetLevel, ipTargetLevel + nTargetLevel, 
										ipInputLevel, ipInputLevel + nInputLevel, lpUnion);
	Int32 lnLookupLevel = lpUnionLast - lpUnion;
    
    
	DSSTabularIndex *lpLookupIndex;
	hr = ipCube->getIndex(lnLookupLevel, lpUnion, &lpLookupIndex);
	CHECK_HR_PTR(lpLookupIndex);
    
	Int32 lnRow = lpLookupIndex->Count();
	//lpLookupIndex has the same number of rows as the ppSliceOut
    
	DSSTabularData *lpTabularData = ipCube->getTabularData();
	CHECK_PTR(lpTabularData);
    
	Int32 lnTotalUnits = 0;
	hr = lpTabularData->Count(&lnTotalUnits);
	CHECK_HR;
    
	DSSSliceCorrelator lSliceCorrelator;
	hr = lSliceCorrelator.Init(lpLookupIndex, 1, &pSliceIn, *ppSliceOut, ipCube);
	CHECK_HR;
    
	BYTE *lpRawData = NULL;//new (ipBuffer) BYTE[lRowSize];
//	CHECK_NEWED_PTR(lpRawData);
    
	Int32 iRow = 0;
	for (iRow = 0; iRow < lnRow; iRow ++)
	{
		Int32 lRetSize = 0;
		DSSDataType_Type lValueType = DssDataTypeUnknown;
		DSSData_Flags lFlag = DssDataUnknown;
        
		//get value from input Slice
		hr = lSliceCorrelator.GetInputValue(0, iRow, &lpRawData, lRowSize, &lRetSize, &lValueType, &lFlag);
		CHECK_HR;
        
		//Set value to the output slice
		hr = lSliceCorrelator.SetOutputValue(iRow, lpRawData, lRetSize, lValueType, lFlag);
		CHECK_HR;
	}
    
	return S_OK;
}

//And/OR operator can take more than 2 input slices, so we evaluate them in pairs 
int DSSCalculationHelper::CalculateAndOR(DSSFunctionObject * ipFuncObj,
                                         Int32 nSliceID, DSSSliceID *ipSliceID, 
                                         //DSSObjectContext *ipSource,
                                         DSSCube *ipTargetCube, 
                                         EnumNullCheckingOption iNULLChecking,
                                         bool bCalcHiddenDE,
                                         DSSSliceID *opSliceID,
                                         MCESchema *ipSchema)
{
	int hr = S_OK;
	
	if(nSliceID < 1) 
		return E_FAIL;
	
	DSSSliceID lCurrentSlice = ipSliceID[0];
    
	Int32 iSlice;
	for(iSlice = 1; iSlice < nSliceID; iSlice ++)
	{
		DSSSliceID lParam[2];
		lParam[0] = lCurrentSlice;
		lParam[1] = ipSliceID[iSlice];
        
        vector<DSSSliceID*> lvSliceID;
        lvSliceID.push_back(&lParam[0]);
        lvSliceID.push_back(&lParam[1]);
        
        CalculationContext *lCalcContext = DSSCalculationHelper::getCalculationContext(ipFuncObj,
                                                                                       ipFuncObj->getOperatorType(),
                                                                                       ipTargetCube,
                                                                                       NULL,
                                                                                       &lvSliceID,
                                                                                       iNULLChecking,
                                                                                       ipSchema);
        MBase::DSSStrongPtr<CalculationContext> lCalcContextPtr(lCalcContext);
        
        hr = DSSCalculationHelper::CalculateNewSlice(lCalcContext, &(lCurrentSlice));
        CHECK_HR;
	}
    
	*opSliceID = lCurrentSlice;
    
	return S_OK;
}

//try to determine the lowest common level of input slices
// for example if slice1 at level {A, B} and slice2 at level {B, C}, then 
//the output level is {A, B, C}
int DSSCalculationHelper::FindCommonLevel(Int32 nSliceIn, DSSSliceID *pSliceIDIn, 
                                        MBase::Buffer *ipBuffer,vector<Int32, MBase::Allocator<Int32> > &ovUnits)
{
	int hr = S_OK;
    
	//collect level units from different slices
	for(Int32 iSlice = 0; iSlice < nSliceIn; iSlice ++)
	{
		DSSCubeDataSlice *lpCubeSlice;
		hr = pSliceIDIn[iSlice].GetSlice(&lpCubeSlice);
		CHECK_HR;
        
		DSSTabularIndex *lpIndices = lpCubeSlice -> getIndices();
		CHECK_PTR(lpIndices);
        
		Int32 lnUnit = lpIndices -> getNumUnits();
        
		Int32 * lpUnit = new (ipBuffer) Int32[lnUnit];
		CHECK_NEWED_PTR(lpUnit);
        
		hr = lpIndices -> getUnits(lnUnit, lpUnit);
		CHECK_HR;
        
		for(Int32 iUnit = 0; iUnit < lnUnit; iUnit ++)
			ovUnits.push_back(lpUnit[iUnit]);
        
	}
    
	//union all input levels together  to get target level
	sort(ovUnits.begin(), ovUnits.end());
	vector<Int32, MBase::Allocator<Int32> >::iterator lIt = unique(ovUnits.begin(), ovUnits.end());
	if (lIt != ovUnits.end())
		ovUnits.erase(lIt, ovUnits.end());
    
	return S_OK;
}

int DSSCalculationHelper::FindMetricSortbyDefns(DSSExpNode *ipNode,
                                                EventContext *ipEventContext,
                                                int inSlices,
                                                int *ipInSliceIDs,
                                                DSSMetric *ipDimMetricGov,
                                                vector<DSSSortDefinition*>**oppSortDefinition)
{
    int hr = S_OK;
    
    DSSExpNodeOperator *lpOperator = dynamic_cast<DSSExpNodeOperator*> (ipNode);
    if (! lpOperator)
        return NULL;
    
    DSSSorts *lpSorts = lpOperator->getSorts();
    int lnSort = 0;

    if (NULL != lpSorts)
    {
        lnSort = lpSorts->Count();
    }
    vector<DSSSort*> lvSort(lnSort);
    for (int i = 0; i < lnSort; ++i)
        lvSort[i] = lpSorts->Item(i);
    
    //fali, 837672, get viewDataSet from gridmodel for grid, and get viewDataset directly from event context for FG.
    DSSViewDataSet *lpViewDS = NULL;
    DSSRWGridModel *lpGridModel = dynamic_cast<DSSRWGridModel *>(ipEventContext->mpDataModel);
    if (lpGridModel)
    {
        hr = lpGridModel->GetViewDataSet(&lpViewDS);
        if (S_OK != hr)
            return NULL;
    }
    else
    {
        lpViewDS = ipEventContext->mpViewDataset;
        if (!lpViewDS) {
            return NULL;
        }
    }
    
    DSSSortConverter converter(lpViewDS);
    return converter.OLAPSortConvert(lvSort, ipEventContext, inSlices, ipInSliceIDs, ipDimMetricGov, oppSortDefinition);
}

int DSSCalculationHelper::GenerateAttributeFormInputs(DSSCube *ipCube, 
                                                      DSSExpNodeFormShortcut *ipAttrFormShortcut,
                                                      DSSSliceID *iSliceID,
                                                      bool iLevelSignificant)
{
    DSSAttribute *lpAttr = ipAttrFormShortcut->getAttribute();
    GUID lAttrGUID = lpAttr->getID();
    
    DSSTabularData *lpTabularData = ipCube->getTabularData();
    CHECK_PTR(lpTabularData);
    int lAttrPos = lpTabularData->FindUnit(lAttrGUID, DssTemplateAttribute);
    //tliao, 09/28/2013, the attribute doesn't exist in the tabular data
    if (lAttrPos < 0)
        return S_FALSE;
    
    int lBaseFormID = 0;
    
    //find baseform id
    DSSAttributeFormWrappers *lpWrappers = lpAttr->getFormWrappers();
    for(int j = 0; j < lpWrappers->Count(); ++j)
    {
        DSSAttributeFormWrapper *lpWrapper = lpWrappers->Item(j);
        if(ipAttrFormShortcut->getForm()->getID() == lpWrapper->getID())
        {
            lBaseFormID = lpWrapper->getBaseFormID();
        }
    }
    //fali, 956215, ipGUnitContainer should not be passed into GetPropertySlice. GenerateAttributeFormInputs seems only used when ipCube keeps global key
    return GetPropertySlice(ipCube, lAttrPos, lBaseFormID, 0, iSliceID, false, iLevelSignificant);
}


Int32  DSSCalculationHelper::LinearFindRowIndexByCellData(XTabCellData* ipKey,  DSSDataColumn* ipColumn, Int32 iStartRow, bool iUseVariant)
{
	AE_ASSERT(ipKey && ipColumn && iStartRow >= 0);
    
    int hr = S_OK;
    
	Int32 lnRow = ipColumn->getRowCount();
    
	bool lbTrimString = true;
	hr = ipColumn->getTrimFlag(&lbTrimString);
	CHECK_HR;
    
	Int32 lRow = iStartRow;
	while (lRow < lnRow)
	{
		Int32 lResult = 0;
		if (!iUseVariant)
			lResult = ipColumn->CompareToData(lRow, ipKey->mpData, ipKey->mSize, ipKey->mStatus, true, true);
		else
		{
			XTabCellData lData;
			ipColumn->getCellData(lRow, lData);
            
			// aye, 8/8/8, null comparison support	
			DSSData_Flags lFlag1 = lData.mStatus;
			DSSData_Flags lFlag2 = ipKey->mStatus;
			if (lFlag1 == DssDataOk && lFlag2 == DssDataOk)
			{
				CComVariant lVar1;
				HRESULT hr = S_OK;
                
				hr = DFCDataConversion::ConvertToVariant(lData.mpData, lData.mDataType, lData.mSize, &lVar1);
				if (hr != S_OK)
					return -1;
                
				CComVariant lVar2;
                
				hr = DFCDataConversion::ConvertToVariant(ipKey->mpData, ipKey->mDataType, lData.mSize, &lVar2);
				if (hr != S_OK)
					return -1;
                
				DSSData_Flags lFlag = DssDataOk;
				lResult = variantCompare(lVar1, lVar2, &lFlag, lbTrimString);
				// gzhou 01/28/2009: TQMS#347151--If lFlag is not DssDataOk, something is wrong already.
				if (lFlag == DssDataInvalid)
					return -1;
			} else
			{
				if (lFlag1 == lFlag2)
					lResult = 0;
				else if (lFlag2 == DssDataOk)
					lResult = -1;
				else
					lResult = 1;
			}
            
		}
        
		if (lResult == 0)
            break;
		else
			lRow++;
	}
    
	if (lRow < lnRow)
		return lRow;
	else
		return -1;
}

int DSSCalculationHelper::FindUnitType(DSSTabularData *ipTabularData, DSS_ID iID, vector<DSSCubeDataSlice*> &irvpSlices, DSSTemplateUnit_Type *opType, Int32 *opUnit)
{
    int hr = S_OK;
    
	Int32 lUnit;
	hr = ipTabularData -> FindUnitI(iID, DssTemplateAttribute, lUnit);
	if (hr == S_OK)
	{
		*opType = DssTemplateAttribute;
		*opUnit = lUnit;
		return hr;
	}
    
    bool lfFound = false;
	hr = ipTabularData -> FindUnitI(iID, DssTemplateConsolidation, lUnit);
	if (hr == S_OK)
    {
        Int32 i;
		for (i = 0; i < irvpSlices.size(); ++i)
		{
            DSSTabularIndex *lpIndex = irvpSlices[i]->getIndices();
            CHECK_PTR(lpIndex);
            
            vector<int> lvUnits;
            hr = lpIndex->getUnits(lvUnits);
            CHECK_HR;
            
            for (int j = 0; j < lvUnits.size(); ++j)
            {
                if (lvUnits[j] == lUnit)
                {
                    lfFound = true;
                    break;
                }
            }
            
            if (lfFound)
                break;
        }
    }
    
    if (lfFound)
	{
		*opType = DssTemplateConsolidation;
		*opUnit = lUnit;
		return S_OK;
	}
    
    hr = ipTabularData -> FindUnitI(iID, DssTemplateCustomGroup, lUnit);
	if (hr == S_OK)
	{
		*opType = DssTemplateCustomGroup;
		*opUnit = lUnit;
	}
    
	return hr;
}

int DSSCalculationHelper::FindMetricBreakbyLevel(DSSExpNode *ipNode, DSSTabularData *ipTabularData, vector<DSSCubeDataSlice*> &irvpSlices, vector<int> *opvUnits)
{
    if (!ipNode || !ipTabularData)
        return E_POINTER;
    
    int hr = S_OK;
    
    EnumDSSNodeDimty lDimtyType = ipNode->getDimtyType();
	if (lDimtyType != DssNodeDimtyBreakBy)
		return hr;
    
	// find the break by from the dim metric
    DSSDimty *lpBreakBy = ipNode->getDimty();
    CHECK_PTR(lpBreakBy);
    
	// find out the number of units in the break by
	int lnUnits = lpBreakBy->Count();
    
    for(Int32 iUnit = 0; iUnit < lnUnits; iUnit++)
    {
        DSSDimtyUnit *lpBreakByUnit = lpBreakBy->Item(iUnit);
        CHECK_PTR(lpBreakByUnit);
        
        EnumDSSDimtyUnitType lUnitType = lpBreakByUnit->getUnitType();
        switch (lUnitType)
		{
            case DssDimtyUnitTypeAttribute:
            {
                DSS_ID lID = lpBreakByUnit -> getTargetID();
				DSSTemplateUnit_Type lType;
				Int32 lUnit;
				hr = FindUnitType(ipTabularData, lID, irvpSlices, &lType, &lUnit);
				if (S_OK == hr)
					// find the unit, then add it into the level
					opvUnits->push_back(lUnit);
				else
					// not find the unit, just ignore it, set the hr from S_FALSE
					// back to S_OK
					hr = S_OK;
            }
                break;
            default:
                return E_FAIL;
        }
    }
    
    return S_OK;
}

CalculationContext* DSSCalculationHelper::getCalculationContext(
                                                 DSSFunctionObject          *ipFuncObject,
                                                 EnumSimpleOperatorType     iFuncIndex,
                                                 DSSCube                    *ipCube,
                                                 vector<int>                *ipvBreakbyUnits,
                                                 vector<DSSSliceID*>        *ipvSliceID,
                                                 EnumNullCheckingOption     iNullCheckingOption,
                                                 MCESchema                  *ipSchema,
                                                 DSSMaskInfos               *ipMaskInf,
                                                 vector<DSSSortDefinition*> *ipvSortbyDefn,
                                                 bool                       ifIsOnIDColumn,
                                                 bool                       ibDSTrx)
{
    CalculationContext* lCalcContext = new CalculationContext();
    lCalcContext->mpFuncObject = ipFuncObject;
    lCalcContext->mFuncIndex = iFuncIndex;
    lCalcContext->mFuncType = DssFunctionTypeReserved;
    lCalcContext->mpCube = ipCube;

    if (ipvBreakbyUnits)
    {
        lCalcContext->mvBreakbyUnits.assign((*ipvBreakbyUnits).begin(), (*ipvBreakbyUnits).end());
    }
    if (ipvSliceID)
    {
        lCalcContext->mvSliceID.assign((*ipvSliceID).begin(), (*ipvSliceID).end());
    }
    if (ipvSortbyDefn)
    {
        lCalcContext->mvSortbyDefn.assign((*ipvSortbyDefn).begin(), (*ipvSortbyDefn).end());
    }
    lCalcContext->mNullCheckingOption = iNullCheckingOption;
    lCalcContext->mpMaskInfos = ipMaskInf;
    lCalcContext->mpSchema = ipSchema;
    lCalcContext->mfIsOnIDColumn = ifIsOnIDColumn;
    lCalcContext->mbDSTrx = ibDSTrx;
    return lCalcContext;
}

int DSSCalculationHelper::ChooseAggregationFunction(DSSObjectContext    *ipObjContext,
                                                    DSSFunctionServer   *ipFunctionServer,
                                                    METRIC_ID           iMetricID,
                                                    DSSFunctionObject   **oppFunctionObject,
                                                    Int32               *opFunctionIndex,
                                                    VARIANT_BOOL        *opIsOrderSignificant,
                                                    DSSExpNode          **oppExprRoot,
                                                    EnumDSSMetricType   iMetricType)
{
    int hr = S_OK;
    
	DSSObjectInfo *lpInfo = ipObjContext->FindObjectH(iMetricID.mMetricID);
	CHECK_PTR(lpInfo);
    
	DSSMetric *lpMetric = dynamic_cast<DSSMetric*>(lpInfo);
	/*
	CComPtr<ICDSSMetricSubtotals> lpMetricSubtotals;
	hr = lpMetric -> get_Subtotals(&lpMetricSubtotals);
	CHECK_HR_PTR(lpMetricSubtotals);
    
	CComPtr<ICDSSSource> lpSource;
	hr = lpInfo->get_Source(&lpSource);
	CHECK_HR_PTR(lpSource);
    
	CComPtr<ICDSSProject> lpProject;
	hr = lpSource->get_Project(&lpProject);
	CHECK_HR_PTR(lpProject);
    
	DSS_ID lSubtotalID;
	hr = lpProject->get_SystemSubtotalID(DssMetricSubtotalAggregation, &lSubtotalID);
	// it will fail if this is an old project.
	CHECK_HR;
    
	CComPtr<ICDSSMetricSubtotal> lpMetricSubtotal;
	hr = lpMetricSubtotals->ItemBySubtotalID(&lSubtotalID, &lpMetricSubtotal);
	CHECK_HR;
    
	
	if((hr == DSSCOM_E_OBJECT_NOT_FOUND) || !lpMetricSubtotal)
	{
     
         DssMetricSubtotalAggregation is not set, we suppose to make up the special
         flag, so GUI could show (XXXX) in the grid
         
         the other place is the CalculationEventEvaluation. At each of the
         ApplyEvent, we need to fill out the failed calculation with (XXX) instead of
         failing the operation. (Metric Limit is the exception.
         
         We could have a constant slice created (with cardinality 1) and use it over and
         over to fill in the places. However, the risk here is that we need to make
         sure it need to be carried over in the subsequent calculation as well.
         //
        
		*oppFunctionObject = NULL;
		*opFunctionIndex = DssFuncUnknown;
		return hr = S_OK;
	}
	*/
    if(iMetricType == DssMetricReserved)
	{
        /*Dynamic Aggregation Function is not set, we suppose to make up the special
        flag, so GUI could show (XXXX) in the grid
        
        the other place is the CalculationEventEvaluation. At each of the
        ApplyEvent, we need to fill out the failed calculation with (XXX) instead of
        failing the operation. (Metric Limit is the exception.
                                
                                We could have a constant slice created (with cardinality 1) and use it over and
                                over to fill in the places. However, the risk here is that we need to make
                                sure it need to be carried over in the subsequent calculation as well.
                                */
                                
        *oppFunctionObject = NULL;
        *opFunctionIndex = DssFuncUnknown;
        return hr = S_OK;
    }

	
    /*
	DSS_ID lImplID = GUID_NULL;
	hr  = lpMetricSubtotal -> get_ImplementationID(&lImplID);
	CHECK_HR;
    
	DSS_ID lDefMetricID;
	hr = lpMetricSubtotal -> get_DefinitionID(&lDefMetricID);
	CHECK_HR;
    
	if(lImplID == lDefMetricID)*/
    if (iMetricType == DssMetricSubtotalAggregation)
	{
		//By default, we are going to use sum as aggregation function.
		//But for Min and Max, we are going to itself as aggregation function.
        
		DSSCExpression *lpExpr = lpMetric->getExpression();
		CHECK_PTR(lpExpr);
        
		hr = getAggFuncFromMetricExpr(ipFunctionServer, lpExpr, oppFunctionObject, opFunctionIndex, opIsOrderSignificant, oppExprRoot);
		CHECK_HR;
        
        
	}
	else
	{
		// Find the subtotal object from the bindingtobal, since
        /*
		CComPtr<ICDSSObjectInfo> lpImplInfo;
		hr = CDSSAEHelper::FindObjectByID(ipSource, ipObjContext, &lImplID, &lpImplInfo);
		CHECK_HR_PTR(lpImplInfo);
        
        
		CComQIPtr<ICDSSMetric, &IID_ICDSSMetric> lpImpl(lpImplInfo);
        
        
		CComPtr<ICDSSExpression> lpExpr;
		hr = lpImpl->get_Expression(&lpExpr);
		CHECK_HR_PTR(lpExpr);*/
        DSSCExpression *lpExpr = lpMetric->getExpression();
		CHECK_PTR(lpExpr);
        
		DSSExpNode* lpNode = lpExpr -> getRoot();
		CHECK_PTR(lpNode);
        
		EnumDSSNodeType lNodeType = lpNode -> getType();
        
		if (lNodeType == DssNodeShortcut)
        {
            DSSExpNodeShortcut *lpShortcut = static_cast<DSSExpNodeShortcut*> (lpNode);
			if(lpShortcut->getTarget()->getType()!= DssTypeAggMetric)
				return E_FAIL;
			
			DSSObjectInfo *lpTarget = lpShortcut->getTarget();
			CHECK_PTR(lpTarget);
            
			DSS_ID lAggID = lpTarget->getID();
            
			// find the agg metric from the binding table
            DSSObjectInfo *lpMetricInfo = ipObjContext->FindObjectH(lAggID);
            CHECK_PTR(lpMetricInfo);
            
            DSSAggMetric *lpMetric = dynamic_cast<DSSAggMetric *>(lpMetricInfo);
            CHECK_PTR(lpMetric);
            
            DSSCExpression *lpMetricExpr = lpMetric->getExpression();
            CHECK_PTR(lpMetricExpr);
            
            DSSExpNode *lpMetricRoot = lpMetricExpr->Root();
            CHECK_PTR(lpMetricRoot);
            
            lpNode = lpMetricRoot;
            lNodeType = lpNode -> getType();
		}
        
		if(lNodeType != DssNodeOperator)
			//XW: msg: Aggregation expression root needs to be an operator
			return E_FAIL;
        
        *oppExprRoot = lpNode;
        
        DSSExpNodeOperator *lpOperator = static_cast<DSSExpNodeOperator*> (lpNode);
        
        DSSFunction *lpFunction = lpOperator->getFunction();
        CHECK_PTR(lpFunction);
        
		EnumDSSFunctionType lFunctionType = lpFunction->getAnalyticalType();
        
		if(!(lFunctionType == DssFunctionTypeAggregation || lFunctionType == DssFunctionTypeGeneric) )
			//XW: msg: An aggregation function is expected.
			return E_FAIL;
        
        Int32 lFuncIndex = lpFunction->getFunctionIndex();
        
        *opFunctionIndex = lFuncIndex;
        *oppFunctionObject = ipFunctionServer -> getFunctionByIndex(lFuncIndex);
        
        *opIsOrderSignificant = VARIANT_FALSE;
	}
    
	return S_OK;
}

int DSSCalculationHelper::getAggFuncFromMetricExpr(DSSFunctionServer *ipFunctionServer,
                                                   DSSCExpression *pExpr,
                                                   DSSFunctionObject **oppFunctionObject,
                                                   Int32 *opFunctionIndex,
                                                   VARIANT_BOOL *opIsOrderSignificant,
                                                   DSSExpNode **oppExprRoot)
{
    int hr = S_OK;
    
	DSSExpNode *lpNode = pExpr->getRoot();
	CHECK_PTR(lpNode);
	
	EnumDSSNodeType lNodeType = lpNode -> getType();
    
	switch (lNodeType) {
        case DssNodeOperator:
		{
			*oppExprRoot = lpNode;
            
			DSSExpNodeOperator *lpOperatorC = dynamic_cast<DSSExpNodeOperator*>(lpNode);
            CHECK_PTR(lpOperatorC);
            
			DSSFunction *lpFunction = lpOperatorC->getFunction();
			CHECK_PTR(lpFunction);
            
            *opIsOrderSignificant = VARIANT_FALSE;
            /*
			hr = lpFunction->get_IsOrderSignificantNS(opIsOrderSignificant);
			CHECK_HR;
            
			CComQIPtr<ICDSSFunction2, &IID_ICDSSFunction2> lpFunction2(lpFunction);
			CHECK_PTR(lpFunction2);
			
			CComPtr<ICDSSFunction2> lpAggFunction;
			hr = lpFunction2->get_PartitionFunction2(&lpAggFunction);
			*/
            
            DSSFunction *lpAggFunction = lpFunction->getPartitionFunction();
            
			EnumDSSFunction lFunctionType;
            
			if (lpAggFunction){
                
				lFunctionType = lpFunction->getFunctionType();
                
				if (lFunctionType == DssFunctionCount){ // special dealing with count
					bool lbDistinct = false;
					hr = getDistinctProperty(lpOperatorC, lbDistinct);
					CHECK_HR;
                    
					if (lbDistinct) {
						*oppFunctionObject = NULL;
						*opFunctionIndex = DssFuncUnknown;
                        
						return hr = S_OK;
					}
				}
                
				Int32 lFuncIndex = lpFunction->getFunctionIndex();
                
				*opFunctionIndex = lFuncIndex;
				*oppFunctionObject = ipFunctionServer -> getFunctionByIndex(lFuncIndex);
			} else {
				// for the compatibility with old meta data
                
				lFunctionType = lpFunction->getFunctionType();
                
				switch (lFunctionType) {
                    case DssFunctionSum:
                        *opFunctionIndex = DssFuncSum;
                        *oppFunctionObject = ipFunctionServer -> getFunctionByIndex(DssFuncSum);
                        
                        break;
                        
                    case DssFunctionCount:
					{
						bool lbDistinct = false;
						hr = getDistinctProperty(lpOperatorC, lbDistinct);
						CHECK_HR;
                        
						if (lbDistinct)
						{
							*opFunctionIndex = DssFuncUnknown;
							*oppFunctionObject = NULL;
						}
						else {
							*opFunctionIndex = DssFuncSum;
							*oppFunctionObject = ipFunctionServer -> getFunctionByIndex(DssFuncSum);
						}
					}
                        
                        break;
                        
                    case DssFunctionMin:
                        *opFunctionIndex = DssFuncMin;
                        *oppFunctionObject = ipFunctionServer -> getFunctionByIndex(DssFuncMin);

                        break;
                        
                    case DssFunctionMax:
                        *opFunctionIndex = DssFuncMax;
                        *oppFunctionObject = ipFunctionServer -> getFunctionByIndex(DssFuncMax);

                        break;
                        
                    default:
                        *opFunctionIndex = DssFuncUnknown;
                        *oppFunctionObject = NULL;
                        
				}
               
			} //else
            
		}
            
            break;
        case DssNodeShortcut:
		{
			DSSExpNodeShortcut *lpShortCut = dynamic_cast<DSSExpNodeShortcut*>(lpNode);
			CHECK_PTR(lpShortCut);
            
			DSSObjectInfo_Type lTargetType = lpShortCut -> getTarget()->getType();

			if (lTargetType != DssTypeAggMetric)
				return hr = E_FAIL;
            
			DSSObjectInfo *lpObjInfo = lpShortCut->getTarget();
            
			DSSAggMetric *lpMetric = dynamic_cast<DSSAggMetric*>(lpObjInfo);
			CHECK_PTR(lpMetric);
            
			DSSCExpression *lpExpr = lpMetric->getExpression();
			CHECK_PTR(lpExpr);
            
			hr = getAggFuncFromMetricExpr(ipFunctionServer, lpExpr, oppFunctionObject, opFunctionIndex, opIsOrderSignificant, oppExprRoot);
			CHECK_HR;
		}
            
            break;
            
        default:
            return hr = E_FAIL;
            
	} //switch (lNodeType)
    
	return hr = S_OK;
    
}

//Strech input slice downward to the target level.
//We assume the input level is higher than the targe level
//An example is used to illustrate the algorithm
// Input slice is at {Year, Product} level, and the target level is {Month Product}
//We first strech input slice column to a {Year, Month, Product} column
//Then the output slice = {Month Product} index + the {Year, Month, Product} Column
int DSSCalculationHelper::StrechSliceDownward(DSSCube * ipCube,
                                              //MBase::Buffer * ipBuffer,
                                              DSSCubeDataSlice *pSliceIn,
                                              Int32 nInputLevel, Int32 *ipInputLevel,
                                              Int32 nTargetLevel, Int32 *ipTargetLevel,
                                              DSSCubeDataSlice ** ppSliceOut)
{
	HRESULT hr = S_OK;
    
    
	//create output slice, which has the same data type as the input slice
	DSSDataType_Type lSliceDataType = pSliceIn->getDataType();
    
	Int32 lRowSize = 0;
	hr = pSliceIn->getSizeOfData(lRowSize);
	CHECK_HR;
    
	// qjiang 06/06/2009
	// original column may be integer column, while its true type is double, check it
	DSSDataColumn *lpColIn = pSliceIn->getColumn();
	CHECK_PTR(lpColIn);
    
	double lDivider = lpColIn->getDivider();
	if (lDivider > 1)
	{
		lSliceDataType = DssDataTypeDouble;
		lRowSize = sizeof(double);
	}
    
	hr = ipCube->CreateDataSlice(nTargetLevel, ipTargetLevel, lSliceDataType, ppSliceOut);
	CHECK_HR_PTR(*ppSliceOut);
    
	//create the lookup index, whose level is the union of the input level and the target level
	Int32 *lpUnion = new Int32 [nTargetLevel + nInputLevel];
	Int32 *lpUnionLast = std::set_union(ipTargetLevel, ipTargetLevel + nTargetLevel,
										ipInputLevel, ipInputLevel + nInputLevel, lpUnion);
	Int32 lnLookupLevel = lpUnionLast - lpUnion;
    
    
	DSSTabularIndex *lpLookupIndex;
	hr = ipCube->getIndex(lnLookupLevel, lpUnion, &lpLookupIndex);
	CHECK_HR_PTR(lpLookupIndex);
    
	Int32 lnRow = lpLookupIndex->Count();
	//lpLookupIndex has the same number of rows as the ppSliceOut
    
	DSSTabularData *lpTabularData = ipCube->getTabularData();
	CHECK_PTR(lpTabularData);
    
	Int32 lnTotalUnits = 0;
	hr = lpTabularData->Count(&lnTotalUnits);
	CHECK_HR;
    
	DSSSliceCorrelator lSliceCorrelator;
	hr = lSliceCorrelator.Init(lpLookupIndex, 1, &pSliceIn, *ppSliceOut, ipCube);
	CHECK_HR;
    
	BYTE *lpRawData = new BYTE[lRowSize];
	CHECK_NEWED_PTR(lpRawData);
    
	Int32 iRow = 0;
	for (iRow = 0; iRow < lnRow; iRow ++)
	{
		Int32 lRetSize = 0;
		DSSDataType_Type lValueType = DssDataTypeUnknown;
		DSSData_Flags lFlag = DssDataUnknown;
        
		//get value from input Slice
		hr = lSliceCorrelator.GetInputValue(0, iRow, &lpRawData, lRowSize, &lRetSize, &lValueType, &lFlag);
		CHECK_HR;
        
		//Set value to the output slice
		hr = lSliceCorrelator.SetOutputValue(iRow, lpRawData, lRetSize, lValueType, lFlag);
		CHECK_HR;
	}
    
	return S_OK;
}

// qjiang
// for aggregation functions, check if need to adjust the level of the input
// kshi 2011-07-29 TQMS461880
int DSSCalculationHelper::getIsInputSignificantProperty(DSSFunction* ipFunction, DSSExpNodeOperator *ipOperator, bool &bInputSignificant)
{
	int hr = S_OK;
    
	// extend the UseLookupForAttributes property to all functions that have SQLType "DssFunctionSQLTypeAggregation"
	if (ipFunction->getSQLType() == DssFunctionSQLTypeAggregation)
	{
		Int32 lPropertyIndex = -1;
        
		// since the property is not pre-added in the pds file, we cannot know the index in advance, so have to use name here
		// but normally function should not have too many properties, this approach won't introduce performance problems
		//hr = CDSSAEHelper::GetFunctionPropertyID(lpFunc, L"UseLookupForAttributes", &lPropertyIndex);
		//CHECK_HR;
        
        //tliao, 1/7/2014, 829260, use property name to get property index
        //tliao, 964482, 2014/09/18, get function property ID from DSSFunction
        lPropertyIndex = ipFunction->getFunctionPropertyID("UseLookupForAttributes");
        
        // this function does not have the property, ok to return
		if (lPropertyIndex == -1)
			return S_OK;
        
		lPropertyIndex += DssFunctionPropertySystemCount;
        
        std::vector<CComVariant> lvProperties = ipOperator->getFunctionPropertyVec();
        
        if (lvProperties.size() > lPropertyIndex-1)
        {
            CComVariant lVariant = lvProperties[lPropertyIndex-1];
            
            hr = VariantChangeType(&lVariant, &lVariant, 0, VT_BOOL);
            CHECK_HR;
            
            if (lVariant.vt == VT_BOOL)
                bInputSignificant = lVariant.boolVal ? true : false;
        }
    }
    
	return S_OK;
}

//xhan 01/31/02
//read distinct property from operator. return either true and false accordingly
int DSSCalculationHelper::getDistinctProperty(DSSExpNodeOperator *pCDSSOperator, bool &bDistinct)
{
	AE_ASSERT(pCDSSOperator);
	
	std::vector<CComVariant> lpPropertySet = pCDSSOperator->getFunctionPropertyVec();
	
	CComVariant lVariant = lpPropertySet[DssFunctionPropertyDistinct-1];
    
	int hr = VariantChangeType(&lVariant, &lVariant, 0, VT_BOOL);
	CHECK_HR;
    
	bDistinct = (lVariant.boolVal == VARIANT_TRUE) ? true : false;
    
	return hr = S_OK;
    
}

// 3/21/2014, fali, 861201
// 05/21/2008 mwang SQL engine introduces OLAPpattern to verify the parameters set by user, e.g. sort-by, break-by, distinct, etc.
// AE tries to behave consistently with SQLEngine, so AE will verify the OLAPPattern also.
// The difficulty is that parameters such as Distinct, HasWindow are stored inside the FunctionObject, while parameters such as break-by/sort-by are
// stored outside the function object. And according to SQLEngine, we need to modify the break-by/sort-by during the verification. Thus the verifcation
// logic is put here in this helper class rather than inside each function object. And it will probably modify the sortDefns and the breakbyUnits passed in
// to satisfy the critieria set by OLAPPattern

// fields: Partition-By (P), Order-By (O), Window (W), Exclude (E), Distinct (D), CountNulls (N), Range (R).
// values: Required (r), Optional (o), Not-Supported (n), Yes-Supported (y)
int DSSCalculationHelper::validateOlapPattern(DSSGenericFunction *ipFuncObj, int inSortDefns, int onBreakbyUnits, int* opBreakbyUnits)
{
    HRESULT hr=S_OK;
    CComVariant lOlapPattern;
    ipFuncObj->get_PropertyInfo(2, NULL, NULL, &lOlapPattern);
    if (lOlapPattern.vt != VT_BSTR) // invalid pattern, we have no way to verify it, thus treat the verification succeeded
        return S_OK;
    
    CComVariant lVar;
    EnumSimpleOperatorType lFuncIndex;
    ipFuncObj->get_PropertyInfo(-10, NULL, NULL, &lVar);
    if (lVar.vt != VT_I4)
        return S_OK;
    else
        lFuncIndex = static_cast<EnumSimpleOperatorType>(lVar.intVal);
    
    ipFuncObj->get_PropertyInfo(-1, NULL, NULL, &lVar);
    if (lVar.vt != VT_BOOL)
    {
        hr = VariantChangeType(&lVar, &lVar, 0, VT_BOOL);
        CHECK_HR;
    }
    bool lDistinct = lVar.boolVal==VARIANT_TRUE;
    
    ipFuncObj->get_PropertyInfo(-2, NULL, NULL, &lVar);
    if (lVar.vt != VT_BOOL)
    {
        hr = VariantChangeType(&lVar, &lVar, 0, VT_BOOL);
        CHECK_HR;
    }
    bool lCountNull = lVar.boolVal==VARIANT_TRUE;
    
    ipFuncObj->get_PropertyInfo(3, NULL, NULL, &lVar);
    if (lVar.vt != VT_BOOL)
    {
        hr = VariantChangeType(&lVar, &lVar, 0, VT_BOOL);
        CHECK_HR;
    }
    bool lHasWindow = lVar.boolVal == VARIANT_TRUE;

    bool lHasOrderby = (inSortDefns!=0);
    bool lHasBreakby = (onBreakbyUnits!=0);
    
    int lsLen = 0;
    hr = DFCDataConversion::VariantSize(&lOlapPattern, DssDataTypeVarChar, &lsLen);
    CHECK_HR;

    char *lpPattern = new char[lsLen];
    hr = DFCDataConversion::ConvertFromVariant(&lOlapPattern, DssDataTypeVarChar, (unsigned char*)lpPattern, &lsLen);
    CHECK_HR;
    
    const char* pField, *pValue;
    pField = lpPattern;
    pValue = lpPattern+2;
    char lFieldBuf[26];
    
    memset(lFieldBuf,'n', 26); // all the default values are 'n'
    while (pField<lpPattern + lsLen)
    {
        for (; pField<lpPattern + lsLen && (*pField<'A' || *pField>'Z'); ++pField)
            ;
        
        if (pField >= lpPattern+lsLen)
            break;
        pValue = pField + 2;
        for (; pValue<lpPattern + lsLen && (*pValue<'a' || *pValue>'z'); ++pValue)
            ;
        if (pValue >= lpPattern+lsLen)
            break;
        
        
        lFieldBuf[*pField-'A']=*pValue;
        
        pField=pValue+1;
        pValue=pField+2;
    }
    
    //Rule 0: If CountNULL and Distinct are both specified, Distinct takes over, just like simple Count function
    //If not OLAPCount function, mIsCountNulls has no meaning, so set it to be false
    if ( lFuncIndex != DssFuncOLAPCount || lDistinct)
    {
        lCountNull = false;
    }
    
    
    //Rule 1: If distinct is used, order-by and window are not allowed.
    //If we have distinct as well as order-by/window, we ignore order-by/window
    if (lDistinct)
    {
        lHasWindow = lHasOrderby = false;
    }
    
    //Rule 2: If by OLAP pattern, Partition-by/Order-by/Window is N/A, but user specified
    //anyway, we ignore user's input.
    if (lFieldBuf['W'-'A']=='n')
    {
        lHasWindow = false;
    }
    if (lFieldBuf['P'-'A']=='n')
    {
        lHasBreakby = false;
    }
    if (lFieldBuf['O'-'A']=='n')
    {
        lHasOrderby = false;
    }
    
    //Rule 3: If by OLAP pattern, Partition-by/Order-by/Window is Required, but user didn't
    //specify, verification fails.
    
    if ((lFieldBuf['W'-'A']=='r' && !lHasWindow) ||
        (lFieldBuf['P'-'A']=='r' && !lHasBreakby) ||
        (lFieldBuf['O'-'A']=='r' && !lHasOrderby) )
        
        return E_FAIL;
    
    
    //Rule 4: If we have Window but no Order-by, we ignore Window
    if (!lHasOrderby)
    {
        lHasWindow = false;
    }
    
    //Rule 5: If by OLAP pattern, Distinct is not supported, but somehow, user specified it, we ignore user's choice.
    if (lFieldBuf['D'-'A']=='n')
        lDistinct = false;
    
    
    // finish verification, change the properties accordingly
    lVar.boolVal = lHasWindow ? VARIANT_TRUE : VARIANT_FALSE;
    lVar.vt = VT_BOOL;
    ipFuncObj->setProperty(3, &lVar);
    
    lVar.boolVal = lDistinct ?  VARIANT_TRUE : VARIANT_FALSE;
    ipFuncObj->setProperty(-1, &lVar);
    
    lVar.boolVal = lCountNull ? VARIANT_TRUE : VARIANT_FALSE;
    ipFuncObj->setProperty(-2, &lVar);    
    
    //fali, I'm not sure about the effect of below line, temporily comment out
//    if (!lHasOrderby) ipSortDefns->GiveUp();
    if (!lHasBreakby)
    {
        onBreakbyUnits = 0;
        opBreakbyUnits = NULL;
    }
    return S_OK;
}

int DSSCalculationHelper::InspectFilterSlice(bool ibEmptySlice,
                                             DSSCube *ipFilterCube,
											 DSSSliceID &ioFilterSlice)
{
	HRESULT hr = S_OK;
    
	//xhan 03/22/05
	if (!ibEmptySlice)
	{ //check if there is anything missing in filter slice, if yes, return no data
		DSSCubeDataSlice* lpFilterSlice;
		hr = ioFilterSlice.GetSlice(&lpFilterSlice);
		CHECK_HR_PTR(lpFilterSlice);
        
		DSSDataType_Type lInputType = lpFilterSlice->getDataType();
        
		if (lInputType == DssDataTypeMissing)
			ibEmptySlice = true;
        
	}
    
	if (ibEmptySlice) //get a constant zero slice as the filter slice
	{
		double lVal = 0;
        
		hr = DSSCalculationHelper::GetConstantSlice(ipFilterCube,
                                                     0, NULL,
                                                     reinterpret_cast<char *>(&lVal),
                                                     sizeof(double),
                                                     DssDataTypeDouble,
                                                     DssDataOk,
                                                     &ioFilterSlice);
        
		CHECK_HR;
        
	}
    
	return S_OK;
    
}

//tliao, 954998, 2014/08/28, support dynamic sort by
int DSSCalculationHelper::FindMetricSortbyDefnsSimpleExpr(DSSExpNode                 *ipNode,
                                                          DFCSimpleExpr              *ipSortLevel,
                                                          EventContext               *ipEventContext,
                                                          int                        inSlices,
                                                          int                        *ipInSliceIDs,
                                                          DSSMetric                  *ipDimMetricGov,
                                                          vector<DSSSortDefinition*> **oppSortDefinition)
{
    int hr = S_OK;
    
    DSSExpNodeOperator *lpOperator = dynamic_cast<DSSExpNodeOperator*> (ipNode);
    if (! lpOperator)
        return E_FAIL;
    
    //fali, 837672, get viewDataSet from gridmodel for grid, and get viewDataset directly from event context for FG.
    DSSViewDataSet *lpViewDS = NULL;
    DSSRWGridModel *lpGridModel = dynamic_cast<DSSRWGridModel *>(ipEventContext->mpDataModel);
    if (lpGridModel)
    {
        hr = lpGridModel->GetViewDataSet(&lpViewDS);
        if (S_OK != hr)
            return E_FAIL;
    }
    else
    {
        lpViewDS = ipEventContext->mpViewDataset;
        if (!lpViewDS) {
            return E_FAIL;
        }
    }
    
    DSSSortConverter converter(lpViewDS);
    return converter.OLAPSortConvert(ipSortLevel, ipEventContext, inSlices, ipInSliceIDs, ipDimMetricGov, oppSortDefinition);
}
