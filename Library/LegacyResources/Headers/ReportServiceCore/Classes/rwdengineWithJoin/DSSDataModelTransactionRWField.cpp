/*
 *  DSSDataModelTransactionRWField.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 11/11/10.
 *  Copyright 2010 MicroStrategy, Inc. All rights reserved.
 *
 */

#import <set>

#import "DSSDataModelTransactionRWField.h"
#import "DSSRWGridModel.h"
#import "DSSRWFieldGroupModel.h"

#import "DSSViewDataSet.h"
#import "DSSTabularData.h"
#import "DSSCube.h"
#import "DSSXTabView.h"
#import "DSSXTabHeader.h"
#import "DSSTabularMetrics.h"

#import "DSSRWFieldGroupNode.h"
#import "DSSFields.h"
#import "DSSRWControl.h"
/*
 #import "DSSBinaryDataReader.h"
 #import "DSSTemplate.h"
 #import <sstream>
 
 #import "DSSDataModelXTabImpl.h"
 #import "DSSTabularAttribute.h"
 #import "DSSDataColumn.h"
 #import "DSSTabularMetrics.h"
 #import "DSSXTabView.h"
 #import "DSSDataModelFieldGroupImpl.h"
 #import "DSSCalculationPlan.h"
 #import "DSSXTabHeader.h"
 #import "DSSSort_includes.h"
 #import "DSSTabularConsolidation.h"
 #import "DSSCube.h"
 
 const char Tag_TRS_Root[] = "transaction_data";
 const char Attr_TRS_NodeKey[] = "nodekey";
 const char Tag_TRS_HeaderInfo[] = "header_info";
 const char Tag_TRS_Column[] = "column";
 const char Attr_TRS_Type[] = "type";
 const char Attr_TRS_UnitID[] = "unitid";
 const char Attr_TRS_BaseFormID[] = "baseform";
 const char Attr_TRS_DataType[] = "data_type";
 const char Tag_TRS_Records[] = "records";
 const char Tag_TRS_Record[] = "record";
 const char Attr_TRS_ColumnID[] = "id";
 const char Attr_TRS_ColumnValue[] = "value";
 const char Tag_TRS_Columns[] = "columns";
 const char Tag_TRS_Value[] = "value";
 
 #define START_TAG(x) "<" << x << ">"
 #define END_TAG(x) "</" << x << ">"
 #define OPEN_TAG(x)  "<" << x << " "
 #define CLOSE_TAG  " >"
 #define CLOSE_AND_END_TAG  " />"
 #define ATT_VAL(x) "=\"" << x << "\" "
 
 
 void WriteHeaderInfo(int index, DSSColumnMappingInfo* ipColumnInfo, EnumDSSDataType iDataType, ostringstream &orStrStream)
 {
 orStrStream << OPEN_TAG(Tag_TRS_Column);							//<column
 
 orStrStream << Attr_TRS_ColumnID << ATT_VAL(index);					//id = "0"
 
 orStrStream << Attr_TRS_Type << ATT_VAL(ipColumnInfo->getType());	//type = "6"
 
 GUID lUnitID = ipColumnInfo->getAttributeID();
 if (ipColumnInfo->getType() == DssTypeMetric)
 lUnitID = ipColumnInfo->getMetricID();
 
 orStrStream << Attr_TRS_UnitID << ATT_VAL(DSSAEHelper::Guid2String(lUnitID).c_str());	//unitid = "8d679D4AABCDEF12345678"
 
 orStrStream << Attr_TRS_DataType << ATT_VAL(iDataType);				//data_type = "6"
 
 if (ipColumnInfo->getType() == DssTypeAttributeForm)				//baseform = "0"
 orStrStream << Attr_TRS_BaseFormID << ATT_VAL(ipColumnInfo->getBaseFormID());
 
 orStrStream << CLOSE_AND_END_TAG;											// />
 }
 
 void hEscape(MBase::CString &irText, const char &irChar, const char *ipReplaceStr)
 {
 int count = 0;
 int lMax = irText.size();
 int lPos = 0;
 while ((lPos = irText.find(irChar, lPos)) != irText.npos
 && count < lMax)
 {
 irText.replace(lPos, 1, ipReplaceStr);
 lPos++;
 count++;
 }
 return;
 }
 
 void hEncodeXML(MBase::CString &irText)
 {
 hEscape(irText, '&', "&amp;");
 
 hEscape(irText, '"', "&quot;");
 
 hEscape(irText, '<', "&lt;");
 
 hEscape(irText, '>', "&gt;");
 
 hEscape(irText, '\'', "&apos;");
 
 return;
 }
 
 void WriteColumnData(MBase::CString &irText, ostringstream &orStrStream)
 {
 hEncodeXML(irText);
 
 orStrStream << START_TAG(Tag_TRS_Value) << irText << END_TAG(Tag_TRS_Value);
 
 return;
 }
 

void ConvertDateTime(MBase::CString &orText)
{
    int year=0, month=0, day=0, hour=0, minute=0, second=0;
    int n=sscanf(orText.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
    
    char lBuffer[32];
    if(n==6) // date & time
    {
        sprintf(lBuffer, "%d/%d/%d %d:%02d:%02d", month, day, year, hour, minute, second);
        orText = lBuffer;
    }
    else if(n==3)  // date only
    {
        sprintf(lBuffer, "%d/%d/%d", month, day, year);
        orText = lBuffer;
    }
    else //time only
    {
        n=sscanf(orText.c_str(), "%d:%d:%d", &hour, &minute, &second);
        if (n == 3)
        {
            sprintf(lBuffer, "%d:%02d:%02d", hour, minute, second);
            orText = lBuffer;
        }
    }
    return;
}


 void ConvertDataTypeAndToString(const AERawData &irRawData, EnumDSSDataType iTargetDataType, MBase::CString &orText)
 {
 EnumDSSDataType lRawDataType = irRawData.mDataType;
 
 if (iTargetDataType == DssDataTypeTime)
 iTargetDataType = DssDataTypeTimeStamp;		//463182
 
 bool lbIsRawDataDateTime = (lRawDataType == DssDataTypeTime
 || lRawDataType == DssDataTypeDate
 || lRawDataType == DssDataTypeTimeStamp);
 
 bool lbIsTargetDateTime = (iTargetDataType == DssDataTypeTime
 || iTargetDataType == DssDataTypeDate
 || iTargetDataType == DssDataTypeTimeStamp);
 
 if (lbIsRawDataDateTime && lbIsTargetDateTime)
 {
 //both are date time type
 DSSAEHelper::ConvertToString(irRawData.mValue, irRawData.mSizeInByte,
 iTargetDataType, irRawData.mFlag, orText);
 }
 else if (lbIsRawDataDateTime)
 {
 //convert date time to numeric
 double lVal = 0;
 DSSAEHelper::ConvertToNumeric(irRawData.mValue, irRawData.mSizeInByte,
 irRawData.mDataType, irRawData.mFlag, lVal);
 
 DSSAEHelper::ConvertToString((char*)&lVal, sizeof(double), DssDataTypeDouble, DssDataOk, orText);
 }
 else if (lbIsTargetDateTime)
 {
 //convert from numeric to date time
 double lVal = 0;
 DSSAEHelper::ConvertToNumeric(irRawData.mValue, irRawData.mSizeInByte,
 irRawData.mDataType, irRawData.mFlag, lVal);
 
 MDataType::DSSDateTime lDataTime(lVal);
 
 DSSAEHelper::ConvertToString((char*)&lDataTime, sizeof(MDataType::DSSDateTime), iTargetDataType, DssDataOk, orText);
 }
 else
 {
 //not date time, still use the raw data type
 DSSAEHelper::ConvertToString(irRawData.mValue, irRawData.mSizeInByte,
 irRawData.mDataType, irRawData.mFlag, orText);
 }
 }
 
 void WriteRecord(TransactionRecord* ipRecord, vector<ColumnInfo> &irHeaderInfo, ostringstream &orStrStream)
 {
 if (ipRecord->mFlag < DssTransactionDataUpdate || ipRecord->mFlag > DssTransactionDataInsert)
 return;
 
 orStrStream << OPEN_TAG(Tag_TRS_Record);							//<record type = "update" >
 orStrStream << Attr_TRS_Type << ATT_VAL(ipRecord->mFlag) << " >";	//
 
 orStrStream << START_TAG(Tag_TRS_Columns);							//<columns>
 
 int lnColumns = irHeaderInfo.size();
 for (int i = 0; i < lnColumns; i++)
 {
 if (ipRecord->mFlag == DssTransactionDataUpdate
 && irHeaderInfo[i].mpDefinition->getColumnFlags() == DssTransactionMappingOptional
 && (!ipRecord->mvpText[i]))
 {
 //skip if the optional column is not changed
 continue;
 }
 
 orStrStream << OPEN_TAG(Tag_TRS_Column);						//<column id = "0"
 orStrStream << Attr_TRS_ColumnID << ATT_VAL(i);
 
 MBase::CString lText;
 EnumDSSTransactionDataChangeType lType = DssTransactionDataOriginal;
 if (!ipRecord->mvpText[i])	//original data
 {
 AERawData *lpRawData = &ipRecord->mvOriginalData[i];
 if (lpRawData->mDataType != irHeaderInfo[i].mDataType && lpRawData->mFlag == DssDataOk)
 {
 //465875, formate the raw data using the transaction report data type
 ConvertDataTypeAndToString(*lpRawData, irHeaderInfo[i].mDataType, lText);
 }
 else
 {
 DSSAEHelper::ConvertToString(lpRawData->mValue, lpRawData->mSizeInByte, lpRawData->mDataType, lpRawData->mFlag, lText);
 }
 }
 else	//updated data
 {
 lText = *ipRecord->mvpText[i];
 lType = DssTransactionDataUpdate;
 }
 
 orStrStream << Attr_TRS_Type << ATT_VAL(lType) << CLOSE_TAG;	// type = "update" >
 WriteColumnData(lText, orStrStream);							//<value> XXX </value>
 orStrStream << END_TAG(Tag_TRS_Column);							//</column>
 }
 orStrStream << END_TAG(Tag_TRS_Columns);							//</columns>
 
 orStrStream << END_TAG(Tag_TRS_Record);								//</record>
 }
 */

DSSDataModelTransactionRWField::DSSDataModelTransactionRWField():mpSourceFGModel(NULL), mBlockSize(0)
{
    mbNewRWD = true;
}

DSSDataModelTransactionRWField::~DSSDataModelTransactionRWField()
{
//	this->hCleanUp();
}

int DSSDataModelTransactionRWField::InitWithNodeModel(DSSTransactionReport *ipTransactionReport, DSSRWNodeModel *ipNodeModel)
{
    if(!ipNodeModel)
        return E_INVALIDARG;
    
    int hr = S_OK;
    mpSourceFGModel = static_cast<DSSRWFieldGroupModel*>(ipNodeModel);
    mNodeKey = mpSourceFGModel->getNodeKey();
    mnColumns = 0;
    mpTransactionReport = NULL;
    
    _ASSERT(mpSourceFGModel->getNodeType() == DssRWNodeFieldGroup);
    
	{
        DSSRWFieldGroupModel *lpFieldModel = static_cast<DSSRWFieldGroupModel*>(mpSourceFGModel);
        
        DSSRWNode *lpRWNode = NULL;
        lpFieldModel->getNode(&lpRWNode);
        CHECK_PTR(lpRWNode);
        
        DSSFields *lpFields = dynamic_cast<DSSRWFieldGroupNode *>(lpRWNode)->getFields();
        CHECK_PTR(lpFields);
        
        mColOrdinalAdjustor = lpFields->Count();
	}
    
    if(!ipTransactionReport)
    {
        mStatus = XTAB_INITIALIZED;
        return hr;
    }
    
	map<int, DSSColumnMappingInfo*>* lpColumnInfoMap = ipTransactionReport->getColumnMap();

	
	mpTransactionReport = ipTransactionReport;
    mpSourceFGModel = static_cast<DSSRWFieldGroupModel*>(ipNodeModel);
    mNodeKey = mpSourceFGModel->getNodeKey();
    int lnTargetColumns = 0;
    //782890
    map<int, DSSColumnMappingInfo*>::iterator lIter;
	for (lIter = lpColumnInfoMap->begin(); lIter != lpColumnInfoMap->end(); lIter++)
	{
        if(lIter->first > lnTargetColumns)
            lnTargetColumns = lIter->first;
    }
    mnColumns = lnTargetColumns;
	
	mHeaderInfo.resize(lnTargetColumns);
    
	int i = 0;
	for (i = 0; i <lnTargetColumns; i++)
	{
		DSSColumnMappingInfo *lpColumnInfo = lpColumnInfoMap->find(i+1) != lpColumnInfoMap->end() ? (*lpColumnInfoMap)[i + 1] : NULL;	//column mapping info is 1-based
        //		AE_ASSERT(lpColumnInfo);
        
		mHeaderInfo[i].mpDefinition = lpColumnInfo;
		
		mHeaderInfo[i].mDataType = lpColumnInfo ? lpColumnInfo->getDataType() : DssDataTypeReserved;
		
		mHeaderInfo[i].mpTabularUnit = NULL;
		
		mHeaderInfo[i].mBaseForm = 0;
	}
    
    //get the "required" column info
    DSSRWDataModel *lpDataModel = mpSourceFGModel->getDataModel();
    CHECK_PTR(lpDataModel);
    DSSObjectContext* lpObjectContext = lpDataModel->getObjectContext();
    
    vector<int> lvRequiredColumnIndices;
    if(lpObjectContext)
    {
        DSSRWNode* lpNode = lpObjectContext->getNode(mpSourceFGModel->getNodeKey());
        lvRequiredColumnIndices = ((DSSRWFieldGroupNode*)lpNode)->getRequiredColumnIndexs();
        for(int i = 0; i < lvRequiredColumnIndices.size(); i++)
        {
            //the column index in lvRequiredColumnIndices is 1-based;
            if(lvRequiredColumnIndices[i] > 0 && lvRequiredColumnIndices[i] <=mHeaderInfo.size())
            {
                mHeaderInfo[lvRequiredColumnIndices[i] - 1].mbRequired = true;
            }
        }
    }
    
	mStatus = XTAB_INITIALIZED;
	
	return S_OK;
}

int DSSDataModelTransactionRWField::AddRecord(int iOriginalRow, int iOriginalColumn, int iColumnIndex, MBase::CString &irNewValue, vector<AERawData> *ipvOriginalValue, vector<AERawData> *ipvDisplayValue)
{
	if (iColumnIndex < 0)
		return E_INVALIDINDEX;
	
	int lAdjustedRow = iOriginalRow;
	int lAdjustedCol = iOriginalColumn;
	hAdjustOrdinal(lAdjustedRow, lAdjustedCol);
	
	
	RecordMap::iterator it = mUpdatedRecords.find(RecordKey(lAdjustedRow, lAdjustedCol));
	TransactionRecord *lpRecord = NULL;
	if (it == mUpdatedRecords.end())
	{
		lpRecord = hCreateNewRecord(DssTransactionDataUpdate, ipvOriginalValue, ipvDisplayValue);
		CHECK_PTR(lpRecord);
		
		mUpdatedRecords[RecordKey(lAdjustedRow, lAdjustedCol)] = lpRecord;
		lpRecord->mRowOrdinal = lAdjustedRow;
		lpRecord->mColOrdinal = lAdjustedCol;
        
        // Sep/28/2012, Initialize "changed order" of all fields in this transaction record.
        for (int i = 0; i < lpRecord->mvDisplayData.size(); ++i)
            lpRecord->mvDisplayData[i].mSizeInByte = 0;
        lpRecord->mvDisplayData[iColumnIndex].mSizeInByte = 1;
	}
	else
	{
		lpRecord = it->second;
		lpRecord->mOrder = mSequenceNumber++;		//473062, submit the latest change last
		lpRecord->mFlag = DssTransactionDataUpdate;
        
        // Sep/28/2012, begin, update "changed oder" of the field which is specified by @Param3.
        int lLastestColIndex = 0;
        for (int i = 1; i < lpRecord->mvDisplayData.size(); ++i)
            if (lpRecord->mvDisplayData[lLastestColIndex].mSizeInByte < lpRecord->mvDisplayData[i].mSizeInByte)
                lLastestColIndex = i;
        
        if (lLastestColIndex != iColumnIndex)
            lpRecord->mvDisplayData[iColumnIndex].mSizeInByte = lpRecord->mvDisplayData[lLastestColIndex].mSizeInByte + 1;
        // Sep/28/2012, end
	}
    
	if(mDiscardedRecords.find(RecordKey(lAdjustedRow, lAdjustedCol))!= mDiscardedRecords.end())
       mDiscardedRecords.erase(RecordKey(lAdjustedRow, lAdjustedCol));
    
    if(iColumnIndex < mHeaderInfo.size())
    {
        if (mHeaderInfo[iColumnIndex].mDataType == DssDataTypeTime
            || mHeaderInfo[iColumnIndex].mDataType == DssDataTypeDate
            || mHeaderInfo[iColumnIndex].mDataType == DssDataTypeTimeStamp)
        {
            ConvertDateTime(irNewValue);
        }
    }else if(iColumnIndex - mHeaderInfo.size() < mvOfflineColumns.size())
    {
        if (mvOfflineColumns[iColumnIndex- mHeaderInfo.size()].mDataType == DssDataTypeTime
            || mvOfflineColumns[iColumnIndex- mHeaderInfo.size()].mDataType == DssDataTypeDate
            || mvOfflineColumns[iColumnIndex- mHeaderInfo.size()].mDataType == DssDataTypeTimeStamp)
        {
            ConvertDateTime(irNewValue);
        }
    }
    
    if (!lpRecord->mvpText[iColumnIndex])
    {
        lpRecord->mvpText[iColumnIndex] = new MBase::CString();
        
    }
    *lpRecord->mvpText[iColumnIndex] = irNewValue;

	AERawData *lpRawData = &(lpRecord->mvOriginalData[iColumnIndex]);
	
	//make a copy of the original data
	lpRawData->TakeOwnership();
	
    //TODO:
//        if (mpSourceDataModel->getModelType() == DssModelFieldGroup && lpRecord->mvOriginalFieldText[iColumnIndex].length() == 0)
        {
            //473405 copy the original field text
//            static_cast<DSSDataModelFieldGroupImpl*>(mpSourceDataModel)->getFieldText(iOriginalRow, iOriginalColumn, lpRecord->mvOriginalFieldText[iColumnIndex]);
        }
	
	return S_OK;
}

int DSSDataModelTransactionRWField::setAllCommit()
{
//    if (mpSourceDataModel && mpSourceDataModel->getModelType() == DssModelFieldGroup)
	{
		hUpdateFieldRawData();
	}
    
    //call parent's method
    DSSDataModelTransaction::setAllCommit();

	//if (mpSourceDataModel && mpSourceDataModel->getModelType() == DssModelXTab)
	{
        //TODO: recalculate for FG?
		//472476
//		static_cast<DSSDataModelXTabImpl*> (mpSourceDataModel)->getCube()->MakeOriginalDataSliceCopy();
	}
	
	return S_OK;
}

// jimzhang, Sep/28/2012, return "changed order" of a transaction field (which is specified by @Param3) in the transaction record
//      (which is specified by @Param1 and @Param2).
//      "changed order" indicates changed sequence of fields in a transaction record. "changed order" is bigger, then the correspounding 
//      field is modified more recently.
//      To avoid change structure of TransactionRecord, "changed order" is saved in TransactionRecord::mvDisplayData[i].mSizeInByte.
//      This change affects very limited existing logic (e.g. offline transaction).
int DSSDataModelTransactionRWField::getFieldChangedOrder(int iOriginalRow, int iOriginalCol, int iColumnIndex, int *ipChgOrder)
{
    if (! ipChgOrder)
        return E_INVALIDARG;
    
    hAdjustOrdinal(iOriginalRow, iOriginalCol);
    RecordMap::iterator it = mUpdatedRecords.find(RecordKey(iOriginalRow, iOriginalCol));
    if (it == mUpdatedRecords.end())
        return E_FAIL;
    
    TransactionRecord *lpRecord = it->second;
    *ipChgOrder = lpRecord->mvDisplayData[iColumnIndex].mSizeInByte;
    
    return S_OK;
}

int DSSDataModelTransactionRWField::UpdateField(int iOriginalRow, int iOriginalColumn, int iColumnIndex, MBase::CString *ipNewValue, bool ibDisplay)
{
    //TODO:
//	AE_ASSERT(mpSourceDataModel->getModelType() == DssModelFieldGroup);
	
	if (ipNewValue)
	{
		//for discard action save the field index at record level, though it can be moved to header ino.
		RecordMap::iterator it = mUpdatedRecords.find(RecordKey(iOriginalRow, 0));
		if (it != mUpdatedRecords.end() && iColumnIndex >= 0)
		{		
			TransactionRecord *lpRecord = it->second;
			lpRecord->mvDataOffset[iColumnIndex] = iOriginalColumn;
            
            //save the display text for undo for DIC
            if(ibDisplay)
            {
                if(!lpRecord->mvpDisplayText[iColumnIndex])
                {
                    lpRecord->mvpDisplayText[iColumnIndex] = new MBase::CString();
                }
                *(lpRecord->mvpDisplayText[iColumnIndex]) = *ipNewValue;
            }
		}
//		return static_cast<DSSDataModelFieldGroupImpl*> (mpSourceDataModel)->setFieldText(iOriginalRow, iOriginalColumn, *ipNewValue);
	}
/*	else
	{
		RecordMap::iterator it = mUpdatedRecords.find(RecordKey(iOriginalRow, 0));
		if (it != mUpdatedRecords.end())
		{
			TransactionRecord *lpRecord = it->second;
			MBase::CString *lpOriginalText = &lpRecord->mvOriginalFieldText[iColumnIndex];
			return static_cast<DSSDataModelFieldGroupImpl*> (mpSourceDataModel)->setFieldText(iOriginalRow, iOriginalColumn, *lpOriginalText);
		}
	}*/
	return S_OK;
}

int DSSDataModelTransactionRWField::Discard(bool ibRestore)
{
	if (mStatus != XTAB_INITIALIZED)
		return E_FAIL;		//472924
    
	int hr = S_OK;
	
	//undo changes
	{
		for (RecordMap::iterator it = mUpdatedRecords.begin(); it != mUpdatedRecords.end(); it++)
		{
			int lOriginalRow = it->first.first;
			TransactionRecord *lpRecord = it->second;
			
			for (int iColumnIndex = 0; iColumnIndex < mnColumns; iColumnIndex++)
			{
				int lField = lpRecord->mvDataOffset[iColumnIndex];
				//466982 & 467220, only undo changes for updated cells
				if(lField >= 0)
				{
					hr = UpdateField(lOriginalRow, lField, iColumnIndex, NULL);
					CHECK_HR;
				}
			}
            if(mDiscardedRecords.find(it->first) == mDiscardedRecords.end())
                mDiscardedRecords.insert(it->first);
		}
        DSSDataModelTransaction::hCleanUp(true);
	}
    //fali, 964048, discard DDIC current selection
    DSSRWNodeModel* lpNodeModel = dynamic_cast<DSSRWNodeModel* >(this->getSourceDataModel());
    CHECK_PTR(lpNodeModel);
    DSSRWNode* lpNode = NULL;
    hr = lpNodeModel->getNode(&lpNode);
    CHECK_HR_PTR(lpNode);
    DSSRWFieldGroupNode * lpFGNode = dynamic_cast<DSSRWFieldGroupNode *>(lpNode);
    DSSFields* lpFields = lpFGNode->getFields();
    for (int i = 0; i < lpFields->Count(); i++)
    {
        DSSField* lpField = lpFields->Item(i);     
        DSSRWControl* lpControl = lpField->getInputControl();
        
        if (lpControl != NULL )
        {
            DSSBaseElementsProxy* lpElements = lpControl->getCurrentElements();
            lpElements->Clear();
        }
    }
	return S_OK;
}


bool DSSDataModelTransactionRWField::IsDataDiscarded(int iOriginalRow, int iOriginalCol, int iColumnIndex)
{
    hAdjustOrdinal(iOriginalRow, iOriginalCol);
    if(mDiscardedRecords.find(RecordKey(iOriginalRow,iOriginalCol))!= mDiscardedRecords.end())
        return true;
    
	return false;
}
int DSSDataModelTransactionRWField::hUpdateFieldRawData()
{
    /*
    int hr = S_OK;
	for (RecordMap::iterator it = mUpdatedRecords.begin(); it != mUpdatedRecords.end(); it++)
	{
		int lOriginalRow = it->first.first;
		TransactionRecord *lpRecord = it->second;
		if (lpRecord && lpRecord->mFlag >= DssTransactionDataUpdate && lpRecord->mFlag <= DssTransactionDataInsert)
		{
			for (int iColumnIndex = 0; iColumnIndex < mnColumns; iColumnIndex++)
			{
				if(lpRecord->mvpText[iColumnIndex])
				{
                    hr = DSSAEHelper::ConvertMetricStr2RawData(*(lpRecord->mvpText[iColumnIndex]), &lpRecord->mvOriginalData[iColumnIndex]);
					CHECK_HR;
				}
			}
		}
	}*/
	return S_OK;
}

int hIsDataModelMatch2(DSSRWFieldGroupModel *ipSourceFGModel, DSSRWFieldGroupModel *ipThisFGModel)
{
    //TODO: to check all runtime view templates?
	return S_OK;
}

int DSSDataModelTransactionRWField::Init(DSSDataModelTransaction *ipTransactionModel, DSSRWNodeModel *ipThisNodeModel)
{
    DSSDataModelTransactionRWField *lpSourceTransactionModel = dynamic_cast<DSSDataModelTransactionRWField*>(ipTransactionModel);
    
	if (!lpSourceTransactionModel || !ipThisNodeModel)
		return E_INVALIDARG;
    
    if (ipThisNodeModel->getNodeType() != DssRWNodeFieldGroup || lpSourceTransactionModel->mpSourceFGModel->getNodeType() != ipThisNodeModel->getNodeType())
        return S_FALSE;
	
    mpTransactionReport = ipTransactionModel->getTransactionReport();
    mpCSI = ipTransactionModel->getCSI();
	int hr = ipThisNodeModel->EnsureModelData();
	CHECK_HR;
    
	//check if two models (mpSourceFGModel) are match
    DSSRWFieldGroupModel *lpThisFGModel = dynamic_cast<DSSRWFieldGroupModel*>(ipThisNodeModel);
    CHECK_PTR(lpThisFGModel);
    
    hr = hIsDataModelMatch2(lpSourceTransactionModel->mpSourceFGModel, lpThisFGModel);
    CHECK_HR;
    
	hr = this->InitWithNodeModel(lpSourceTransactionModel->mpTransactionReport, ipThisNodeModel);
	CHECK_HR;
	
	hr = hPopulateHeaderInfo2(lpSourceTransactionModel);
	CHECK_HR;

	hr = hPopulateTransactionRecordsForFG(ipTransactionModel, NULL, NULL);
	CHECK_HR;
	
    // Sep/12/2012, copy "mapping of GB elements to row block" from source transaction model to this transaction model
    const map<vector<vector<int> >, int> lrmSrcMapping = lpSourceTransactionModel->mmGBElements2BlockOff;
    for (map<vector<vector<int> >, int>::const_iterator iterSrcMapping = lrmSrcMapping.begin(); iterSrcMapping != lrmSrcMapping.end(); ++iterSrcMapping)
    {
        mmGBElements2BlockOff.insert(*iterSrcMapping);
    }

    // Sep/12/2012 copy row count of row blocks from source transaction model to this transaction model
    mBlockSize = lpSourceTransactionModel->mBlockSize;
    mbIsValidated = isModelValidated(-1);
    mvValidatedRows.clear();
    vector<int>* lpValidatedRows = ipTransactionModel->getValidatedRows();
    if(lpValidatedRows)
        mvValidatedRows.assign(lpValidatedRows->begin(),lpValidatedRows->end());
	return S_OK;
}

int DSSDataModelTransactionRWField::hPopulateHeaderInfo2(DSSDataModelTransactionRWField *ipTransactionModel)
{
	int i = 0;
	for (i = 0; i <mnColumns; i++)
	{
        //no need to check tabular unit for field group
		mHeaderInfo[i].mpTabularUnit = NULL;
		mHeaderInfo[i].mBaseForm = ipTransactionModel->mHeaderInfo[i].mBaseForm;
	}
	
	mapTrxColumn2XTabUnit.assign(ipTransactionModel->mapTrxColumn2XTabUnit.begin(), ipTransactionModel->mapTrxColumn2XTabUnit.end());
	mapTrxColumn2Metric.assign(ipTransactionModel->mapTrxColumn2Metric.begin(), ipTransactionModel->mapTrxColumn2Metric.end());
    mapTrxColumn2WriteBackForm.assign(ipTransactionModel->mapTrxColumn2WriteBackForm.begin(), ipTransactionModel->mapTrxColumn2WriteBackForm.end());
    
    mnOfflineColumns = ipTransactionModel->mnOfflineColumns;
    mvOfflineColumns.resize(mnOfflineColumns);
    for(i = 0; i < mnOfflineColumns;i++)
    {
        mvOfflineColumns[i].mDataType = ipTransactionModel->mvOfflineColumns[i].mDataType;
        mvOfflineColumns[i].mbRequired = ipTransactionModel->mvOfflineColumns[i].mbRequired;
        mvOfflineColumns[i].mBaseForm = ipTransactionModel->mvOfflineColumns[i].mBaseForm;
        mvOfflineColumns[i].mpTabularUnit = NULL;
        mvOfflineColumns[i].mpDefinition = NULL;
    }
    mapOfflineTrxColumn2Metric.assign(ipTransactionModel->mapOfflineTrxColumn2Metric.begin(), ipTransactionModel->mapOfflineTrxColumn2Metric.end());
    mapOfflineTrxColumn2WriteBackForm.assign(ipTransactionModel->mapOfflineTrxColumn2WriteBackForm.begin(), ipTransactionModel->mapOfflineTrxColumn2WriteBackForm.end());
    mapOfflineTrxColumn2XTabUnit.assign(ipTransactionModel->mapOfflineTrxColumn2XTabUnit.begin(), ipTransactionModel->mapOfflineTrxColumn2XTabUnit.end());
    mapOfflineTrxColumn2TrxColumn.assign(ipTransactionModel->mapOfflineTrxColumn2TrxColumn.begin(),ipTransactionModel->mapOfflineTrxColumn2TrxColumn.end());

	return S_OK;
}


int DSSDataModelTransactionRWField::hSaveModelInfoToBinary(DSSBinaryDataWriter &irWriter, int &orSourceModelType)
{
    int hr = S_OK;
	//current slice flag 
	int currentSlice = mpSourceFGModel->getCurrentSliceFlag(); 
	hr = irWriter.WriteInt(currentSlice);
	CHECK_HR; 
	
	//source model type 
    orSourceModelType = DssModelFieldGroup;
	hr = irWriter.WriteInt(orSourceModelType);
	CHECK_HR; 
    
	return S_OK;
}

/*
int DSSDataModelTransactionRWField::hPopulateHeaderInfo(DSSDataModelTransactionRWField *ipTransactionModel)
{
	int i = 0;
	for (i = 0; i <mnColumns; i++)
	{
		DSSTabularUnit *lpUnitSrc = ipTransactionModel->mHeaderInfo[i].mpTabularUnit;
		if (lpUnitSrc)
		{
			int lUnitID = mpSourceDataModel->FindUnit(lpUnitSrc->getGUID(), lpUnitSrc->getUnitType());
			if(lUnitID <0 && lpUnitSrc->getUnitType() == DssTemplateAttribute)
			{
				//the unit may be a DE
				lUnitID=mpSourceDataModel->FindUnit(lpUnitSrc->getGUID(),DssTemplateConsolidation);
			}
			AE_ASSERT(lUnitID >= 0);
			mHeaderInfo[i].mpTabularUnit = mpSourceDataModel->Item(lUnitID);
		}
		else
		{
			mHeaderInfo[i].mpTabularUnit = NULL;
		}
		mHeaderInfo[i].mBaseForm = ipTransactionModel->mHeaderInfo[i].mBaseForm;
	}
	
	mapTrxColumn2XTabUnit.assign(ipTransactionModel->mapTrxColumn2XTabUnit.begin(), ipTransactionModel->mapTrxColumn2XTabUnit.end());
	mapTrxColumn2Metric.assign(ipTransactionModel->mapTrxColumn2Metric.begin(), ipTransactionModel->mapTrxColumn2Metric.end());
    
	return S_OK;
}

int DSSDataModelTransactionRWField::hPopulateTransactionRecords(DSSDataModelTransactionRWField *ipTransactionModel)
{
	int hr = S_OK;
	
	if (ipTransactionModel->mpSourceDataModel->getModelType() == DssModelXTab)
	{
		DSSDataModelXTabImpl *lpXTabModelSrc = static_cast<DSSDataModelXTabImpl*> (ipTransactionModel->mpSourceDataModel);
		DSSDataModelXTabImpl *lpXTabModel = static_cast<DSSDataModelXTabImpl*> (mpSourceDataModel);
		DSSXTabView *lpXTabViewSrc = lpXTabModelSrc->getXTabView();
		DSSXTabView *lpXTabView = lpXTabModel->getXTabView();
		
		hr = lpXTabModel->LoadAllDataSlices();
		CHECK_HR;
		
		int lnUnits = lpXTabModelSrc->Count();
		vector<int> lvKeys(lnUnits);
        
		bool lbNeedRecalculate = false;
		//1, copy the submitted records first
		if (!ipTransactionModel->mvpRecordSubmitted.empty())
		{
			lbNeedRecalculate = true;
			int lnRecords = ipTransactionModel->mvpRecordSubmitted.size();
			for (int i = 0; i < lnRecords; i++)
			{
				hr = hPopulateOneRecord(lpXTabViewSrc, lpXTabView, ipTransactionModel->mvpRecordSubmitted[i], lvKeys);
				CHECK_HR;
			}
			
			//483054, recalculate and save the result for discard
			DSSCalculationPlan *lpCalcPlan = lpXTabModel->getCalculationPlan();
			lpCalcPlan->setStatus(DssCalcStatusUnCalculated);
			
			hr = lpXTabModel->ReCalculate();
			if (hr != S_OK)
			{
				_ASSERT(false);	//not expected
			}
			
			lpXTabModel->getCube()->MakeOriginalDataSliceCopy();
			
		}
		
		//2, copy un-submitted records
		RecordMap::iterator it = ipTransactionModel->mUpdatedRecords.begin();
		RecordMap::iterator end = ipTransactionModel->mUpdatedRecords.end();
		for (; it != end; it++)
		{
			TransactionRecord *lpRecordSrc = it->second;
			//481535, the unmarked row contains changed value, still need copy over
			if (lpRecordSrc->mFlag == DssTransactionDataReserved) //|| lpRecordSrc->mFlag == DssTransactionDataOriginal 
				continue;
			
			hr = hPopulateOneRecord(lpXTabViewSrc, lpXTabView, lpRecordSrc, lvKeys);
			CHECK_HR;
			
			if (!lbNeedRecalculate)
				lbNeedRecalculate = true;
			
		}//end of for (; it != end; it++)
		
		mSequenceNumber = ipTransactionModel->mSequenceNumber;
        
		DSSCalculationPlan *lpCalcPlan = lpXTabModel->getCalculationPlan();
		if(lpCalcPlan && lbNeedRecalculate)		//481817; don't set calculation plan if nothing is changed
			lpCalcPlan->setStatus(DssCalcStatusUnCalculated);
        
		hr = lpXTabModel->ReCalculate();
		CHECK_HR;
	}
	else if(ipTransactionModel->mpSourceDataModel->getModelType() == DssModelFieldGroup)
	{
		DSSDataModelFieldGroupImpl *lpFGModel = static_cast<DSSDataModelFieldGroupImpl*> (ipTransactionModel->mpSourceDataModel);
		
		//TODO: inheritate field group's change?
		//1, copy the submitted records first
		if (!ipTransactionModel->mvpRecordSubmitted.empty())
		{
			int lnRecords = ipTransactionModel->mvpRecordSubmitted.size();
			for (int i = 0; i < lnRecords; i++)
			{
				hr = hPopulateOneRecordForFieldGroup(lpFGModel, ipTransactionModel->mvpRecordSubmitted[i]);
				CHECK_HR;
			}
		}
		
		//2, copy un-submitted records
		RecordMap::iterator it = ipTransactionModel->mUpdatedRecords.begin();
		RecordMap::iterator end = ipTransactionModel->mUpdatedRecords.end();
		for (; it != end; it++)
		{
			TransactionRecord *lpRecordSrc = it->second;
			if (lpRecordSrc->mFlag == DssTransactionDataOriginal || lpRecordSrc->mFlag == DssTransactionDataReserved)
				continue;
			
			hr = hPopulateOneRecordForFieldGroup(lpFGModel, lpRecordSrc);
			CHECK_HR;
		}//end of for (; it != end; it++)
		
		mSequenceNumber = ipTransactionModel->mSequenceNumber;
        
	}
    
	return S_OK;
    
}

int DSSDataModelTransactionRWField::hPopulateOneRecordForFieldGroup(DSSDataModelFieldGroupImpl* ipFGModelSrc, TransactionRecord * ipRecordSrc)
{
	int hr = S_OK;
    
	int lRowOrdinalSrc = ipRecordSrc->mRowOrdinal;
	int lColOrdinalSrc = ipRecordSrc->mColOrdinal;
	
	AE_ASSERT(lRowOrdinalSrc >= 0 && lColOrdinalSrc >= 0);
	
	//1, map the previous change to the current xtab data model by row/column ordinal
	int lNewRow = lRowOrdinalSrc;
	int lNewCol = lColOrdinalSrc;
	CHECK_HR;
	
	hr = hMapRowForFieldGroup(ipFGModelSrc, lNewRow);
	if(hr != S_OK){}//swallow error
    
	//3, create a new record
	TransactionRecord *lpRecord = hCreateNewRecord(ipRecordSrc->mFlag, &ipRecordSrc->mvOriginalData);
	CHECK_PTR(lpRecord);
	
	//4, copy other data members of the record
	lpRecord->mOrder = ipRecordSrc->mOrder;
	lpRecord->mvDataOffset.assign(ipRecordSrc->mvDataOffset.begin(), ipRecordSrc->mvDataOffset.end());
	lpRecord->mvOriginalFieldText.assign(ipRecordSrc->mvOriginalFieldText.begin(), ipRecordSrc->mvOriginalFieldText.end());
	vector<MBase::CString*> &lrTextStrings = ipRecordSrc->mvpText;
	int iColumnIndex = 0;
	for (iColumnIndex = 0; iColumnIndex < mnColumns; iColumnIndex++)
	{
		if (lrTextStrings[iColumnIndex])
		{
			lpRecord->mvpText[iColumnIndex] = new MBase::CString(*lrTextStrings[iColumnIndex]);
		}
		//make a copy of the original data
		lpRecord->mvOriginalData[iColumnIndex].TakeOwnership();
	}
	
	lpRecord->mRowOrdinal = lNewRow;
	lpRecord->mColOrdinal = lNewCol;
	
	//5, write the change
	for (iColumnIndex = 0; iColumnIndex < mnColumns; iColumnIndex++)
	{
		if (lpRecord->mvpText[iColumnIndex])
		{
			//adjust row/column offset for metric columns
			int lOffset = lpRecord->mvDataOffset[iColumnIndex];
			
			hr = UpdateField(lpRecord->mRowOrdinal, lOffset, iColumnIndex, lpRecord->mvpText[iColumnIndex]);
			CHECK_HR;
		}
	}
	
	//6, add the new record in the lookup table
	if (ipRecordSrc->mFlag != DssTransactionDataCommit)
	{
		mUpdatedRecords[RecordKey(lNewRow, lNewCol)] = lpRecord;
	}
	
	return S_OK;
}

int DSSDataModelTransactionRWField::hPopulateOneRecord(DSSXTabView *ipXTabViewSrc, DSSXTabView *ipXTabView,
                                                  TransactionRecord * ipRecordSrc, vector<int> &rvKeys)
{
	int hr = S_OK;
	
	int lRowOrdinalSrc = ipRecordSrc->mRowOrdinal;
	int lColOrdinalSrc = ipRecordSrc->mColOrdinal;
	
	AE_ASSERT(lRowOrdinalSrc >= 0 && lColOrdinalSrc >= 0);
	
	//1, map the previous change to the current xtab data model by row/column ordinal
	int lNewRow = 0;
	int lNewCol = 0;
	hr = hMapRowAndColumns(lRowOrdinalSrc, lColOrdinalSrc, rvKeys, ipXTabViewSrc, lNewRow, lNewCol);
	CHECK_HR;
    
	//2, get raw data from the current xtab data model
	vector<AERawData> lvRawData;
	hr = ipXTabView->GenerateRecordWithLookup(lNewRow, lNewCol, mapTrxColumn2XTabUnit, mapTrxColumn2Metric, lvRawData);
	CHECK_HR;
	
	//3, create a new record
	TransactionRecord *lpRecord = hCreateNewRecord(ipRecordSrc->mFlag, &lvRawData);
	CHECK_PTR(lpRecord);
	
	//4, copy other data members of the record
	lpRecord->mOrder = ipRecordSrc->mOrder;
	lpRecord->mvDataOffset.assign(ipRecordSrc->mvDataOffset.begin(), ipRecordSrc->mvDataOffset.end());
	lpRecord->mvOriginalFieldText.assign(ipRecordSrc->mvOriginalFieldText.begin(), ipRecordSrc->mvOriginalFieldText.end());
	vector<MBase::CString*> &lrTextStrings = ipRecordSrc->mvpText;
	int iColumnIndex = 0;
	for (iColumnIndex = 0; iColumnIndex < mnColumns; iColumnIndex++)
	{
		if (lrTextStrings[iColumnIndex])
		{
			lpRecord->mvpText[iColumnIndex] = new MBase::CString(*lrTextStrings[iColumnIndex]);
		}
		//make a copy of the original data
		lpRecord->mvOriginalData[iColumnIndex] = ipRecordSrc->mvOriginalData[iColumnIndex];
		lpRecord->mvOriginalData[iColumnIndex].TakeOwnership();
		
		//adjust row/column offset for metric columns
		int lOffset = lpRecord->mvDataOffset[iColumnIndex];
		if (!mHeaderInfo[iColumnIndex].mpTabularUnit && lOffset >= 0)
		{
			if (mbIsMetricOnColumn)
				lpRecord->mvDataOffset[iColumnIndex] = lNewCol + ipRecordSrc->mvDataOffset[iColumnIndex] - lColOrdinalSrc;
			else
				lpRecord->mvDataOffset[iColumnIndex] = lNewRow + ipRecordSrc->mvDataOffset[iColumnIndex] - lRowOrdinalSrc;
		}
	}
	
	lpRecord->mRowOrdinal = lNewRow;
	lpRecord->mColOrdinal = lNewCol;
	
	//5, write the change to xtab data model
	hr = hUpdateCells(lpRecord, lNewRow, lNewCol, false);
	CHECK_HR;
	
	//6, add the new record in the lookup table
	if (ipRecordSrc->mFlag != DssTransactionDataCommit)
	{
		mUpdatedRecords[RecordKey(lNewRow, lNewCol)] = lpRecord;
	}
    
	return S_OK;
}

int DSSDataModelTransactionRWField::hMapRowAndColumns(int iRowOrdinalSrc, int iColOrdinalSrc,
                                                 vector<int> &irvKeys, DSSXTabView *ipXTabViewSrc,
                                                 int &orNewRow, int &orNewCol)
{
	int hr = S_OK;
	
	DSSDataModelXTabImpl *lpXTabModel = static_cast<DSSDataModelXTabImpl*>(mpSourceDataModel);
	int lnUnits = lpXTabModel->Count();
	_ASSERT(irvKeys.size() == lnUnits);
	for (int i = 0; i < lnUnits; i++)
	{
		irvKeys[i] = DSSXTAB_TOTAL_KEY;
	}
	
	int lMetricKey = 0;
	hr = ipXTabViewSrc->getHeader(0)->FillIndex(iRowOrdinalSrc, lnUnits, &irvKeys[0], &lMetricKey);
	CHECK_HR;
	
	hr = ipXTabViewSrc->getHeader(1)->FillIndex(iColOrdinalSrc, lnUnits, &irvKeys[0], &lMetricKey);
	CHECK_HR;
	
	int lMetricUnitID = lpXTabModel->getMetricUnitID();
	irvKeys[lMetricUnitID] = lMetricKey;
    
	DSSXTabView *lpXTabView = lpXTabModel->getXTabView();
	orNewRow = lpXTabView->getHeader(0)->getOffsetByElements(irvKeys);
	orNewCol = lpXTabView->getHeader(1)->getOffsetByElements(irvKeys);
	
	if(orNewRow <  0 && lpXTabView->getHeader(0)->getNumUnits() == 0)
		orNewRow = 0;
	if(orNewCol < 0 && lpXTabView->getHeader(1)->getNumUnits() == 0)
		orNewCol = 0;
	
	AE_ASSERT(orNewRow >= 0 && orNewCol >= 0);
	
	return S_OK;
}

int DSSDataModelTransactionRWField::hMapRowForFieldGroup(DSSDataModelFieldGroupImpl* ipFGModelSrc, int& iRow)
{
	int hr = S_OK;
	
	CHECK_PTR(ipFGModelSrc);
	
	DSSDataModelFieldGroupImpl *lpFGModel = static_cast<DSSDataModelFieldGroupImpl*> (mpSourceDataModel);
	
	DSSViewMap* lpViewMapSrc = NULL;
	hr = ipFGModelSrc->getViewMap(&lpViewMapSrc);
	CHECK_HR;
	
	DSSViewMap* lpViewMap = NULL;
	hr = lpFGModel->getViewMap(&lpViewMap);
	CHECK_HR;
	
	if(lpViewMapSrc != NULL && lpViewMap != NULL)
	{
		
		int lnGBUnit = lpViewMapSrc->getUnitCount();
		if(lnGBUnit != lpViewMap->getUnitCount())
			return E_FAIL;
		
		int lnGBPathSrc = lpViewMapSrc->CountRows();
		
		if(iRow >= lnGBPathSrc)
			return E_INVALIDINDEX;
		
		vector<int> lvKeys(lnGBUnit);
		DSSObjectContext* lpObjectContext = ipFGModelSrc->getParentInfo()->getObjectContext();
		for(int i = 0; i < lnGBUnit;i++)
		{
			int lKeySrc;
			hr = lpViewMapSrc->getUnitKey(iRow, i, lKeySrc);
			CHECK_HR;
			
			GUID lGUID = GUID_NULL;
			hr = lpViewMapSrc->getUnitID(i, lGUID);
			CHECK_HR;
			
			int lUnitSrc = ipFGModelSrc->FindUnit(lGUID, DssTemplateReserved);
			if(lUnitSrc < 0)
				return E_INVALIDINDEX;
			
			DSSTabularUnit* lpUnitSrc = ipFGModelSrc->Item(lUnitSrc);
			if(!lpUnitSrc)
				return E_POINTER;
			
			DSSBaseElementProxy* lpElement = NULL;
			hr = lpUnitSrc->createElement(lKeySrc, lpObjectContext, &lpElement);
			CHECK_HR;
			
			int lUnit = lpFGModel->FindUnit(lGUID, DssTemplateReserved);
			if(lUnit < 0)
				return E_INVALIDINDEX;
			
			DSSTabularUnit* lpUnit = lpFGModel->Item(lUnit);
			if(!lpUnit)
				return E_POINTER;
			
			//bascially j should equal to i
			int j = lpViewMap->findGBUnit(lGUID);
			if(j < 0 || j >= lnGBUnit)
				return E_INVALIDINDEX;
			
			hr = lpUnit->FindElement(lpElement, lvKeys[j]);
			CHECK_HR;
			
		}
		int* lpKeys = &lvKeys[0];
		hr = lpViewMap->getView(lpKeys, lnGBUnit, iRow);
		CHECK_HR;
		
	}
	return S_OK;
}
 */

int DSSDataModelTransactionRWField::SaveToBinary(bool includeCommitted, unsigned char** oppBinaryModel, int* opSize)
{
    // 1, save DSSDataModelTransaction object to binary
    int hr = DSSDataModelTransaction::SaveToBinary(includeCommitted, oppBinaryModel, opSize);
    CHECK_HR;    
    
    // 2, save "mmGBElements2BlockOff" and "mBlockSize" to binary
    
    DSSBinaryDataWriter lBinaryWriter;
    
    if (! mmGBElements2BlockOff.size())
    {
        lBinaryWriter.Init(sizeof(int));
        lBinaryWriter.WriteInt(0);
    }
    else
    {
        // 2.1 calculate the binary size of mmGBElements2BlockOff.
        
        map <vector<vector<int> >, int>::iterator iterMap = mmGBElements2BlockOff.begin();
        int lnGBUnits = iterMap->first.size();
        
        int lnSizeInInt = 3;
        for (iterMap = mmGBElements2BlockOff.begin(); iterMap != mmGBElements2BlockOff.end(); ++iterMap)
        {
            const vector<vector<int> > &lrvGBElements = iterMap->first;
            
            for (int iGBUnit = 0; iGBUnit < lnGBUnits; ++iGBUnit)
                lnSizeInInt = lnSizeInInt + lrvGBElements[iGBUnit].size() + 1;
            
            ++lnSizeInInt;
        }      
        
        lBinaryWriter.Init(lnSizeInInt * sizeof(int));
        
        // 2.2 save mmGBElements2BlockOff to lBinaryWriter
        
        lBinaryWriter.WriteInt(mmGBElements2BlockOff.size());
        lBinaryWriter.WriteInt(lnGBUnits);        
        
        for (iterMap = mmGBElements2BlockOff.begin(); iterMap != mmGBElements2BlockOff.end(); ++iterMap)
        {
            const vector<vector<int> > &lrvGBElements = iterMap->first;
            
            for (int iGBUnit = 0; iGBUnit < lnGBUnits; ++iGBUnit)
            {            
                const vector<int>& lrvGBUnitElements = lrvGBElements[iGBUnit];
                
                int lnGBUnitElements = lrvGBUnitElements.size();
                lBinaryWriter.WriteInt(lnGBUnitElements);
                
                for (int iElem = 0; iElem < lnGBUnitElements; ++iElem)
                    lBinaryWriter.WriteInt(lrvGBUnitElements[iElem]);            
            }
            
            lBinaryWriter.WriteInt(iterMap->second);
        }
        
        // 2.3 save "mBlockSize" to binary
        lBinaryWriter.WriteInt(mBlockSize);
    }
    
    unsigned char *lpMappingData = NULL;
    int lMappingSize = 0;
    hr = lBinaryWriter.getBinary(&lpMappingData, &lMappingSize);
    CHECK_HR;
    
    // 3, append binary of "mmGBElements2BlockOff" to binary of DSSDataModelTransaction object 
    unsigned char *lpBuf = new unsigned char[*opSize + lMappingSize];
    if (! lpBuf)
    {
        delete [] (*oppBinaryModel);
        delete [] lpMappingData;
        
        *oppBinaryModel = NULL;
        *opSize = 0;
        
        return E_OUTOFMEMORY;
    }
    
    memmove(lpBuf, *oppBinaryModel, *opSize);
    memmove(lpBuf + (*opSize), lpMappingData, lMappingSize);
    
    delete [] (*oppBinaryModel);
    delete [] lpMappingData;
    
    *oppBinaryModel = lpBuf;
    *opSize += lMappingSize;
    
    return hr;
}

int DSSDataModelTransactionRWField::setOriginalFieldText(int iOriginalRow, int iOriginalColumn, int iColumnIndex, std::string iFieldText)
{
    // Sep/5/2012, in new RWD, original field text of transaction record is not saved to binary, so there is no need to 
    //      call setOriginalFieldText() in new RWD. But this function is called by DSSDataModelFieldGroupImpl::LoadBinaryTransactionModel() 
    //      which is a static function shared by old RWD and new RWD, therefore, just make a empty implementation here.
    return S_OK;
}

int DSSDataModelTransactionRWField::EnsureRawData(DSSRWFieldGroupModelIterator *ipFGIter, int iBlockOff, int inRows)
{
    bool lbHasError = false;
    
    RecordMap::iterator iterRecordMap;
    for (iterRecordMap = mUpdatedRecords.begin(); iterRecordMap != mUpdatedRecords.end(); ++iterRecordMap)
    {
        int lAdjustedRow = iterRecordMap->first.first;
        
        // TQMS 687424, only populate raw data of transaction records which are under current group-by elements
        if (lAdjustedRow < iBlockOff || lAdjustedRow >= iBlockOff + inRows)
            continue;
        
        TransactionRecord *lpTrxRecord = iterRecordMap->second;        
        vector<AERawData> &lrvRawData = lpTrxRecord->mvOriginalData;
        
        for (int iTrxColumn = 0; iTrxColumn < mapTrxColumn2Metric.size(); ++iTrxColumn)
        {
            int lFieldIndex = mapTrxColumn2Metric[iTrxColumn];
            if (lFieldIndex < 0)
                continue;
          
            if (DssDataTypeReserved == lrvRawData[iTrxColumn].mDataType)
            {
                AERawData lRawData;
                int hr = ipFGIter->getRawData(lAdjustedRow - iBlockOff, lFieldIndex,
                                              (const char**)&lRawData.mValue,
                                              &lRawData.mSizeInByte,
                                              &lRawData.mDataType,
                                              &lRawData.mFlag,
                                              false,
                                              true);//the raw data is for orginal data, we should discard unsubmitted trx change
                
                if (S_OK != hr)
                    lbHasError = true;
                
                // Dec/11/2012, TQMS 687446, make a copy of column data, and let raw data of transacton record point to the copyed data. The reason to do so is is AERawData::mbHasOwnership in transaction record is set to true in DSSDataModelTransaction::hPopulateOneRecordForFieldGroup(). [todo] Another way to fix this issue is in DSSDataModelTransaction::hCreateNewRecord(), use AERawData::Assign() to copy raw data, and in DSSDataModelTransaction::hPopulateOneRecordForFieldGroup(), do not call AERawData::TakeOwnership() for raw data.
                lrvRawData[iTrxColumn] = lRawData;
                lrvRawData[iTrxColumn].TakeOwnership();
            }
        }
    }
    
    return false == lbHasError ? S_OK : S_FALSE;
}

// Sep/6/2012
// @Param1 group-by elemetns of field group iterator
// @Param2 row count of field group iterator under specific group-by elemetns
// @Return offset of corresponding block (space of mapped row numbers)
int DSSDataModelTransactionRWField::getBlockOffByGBElements(vector<vector<int> > &irvGBElements, int inRows)
{
    int lBlockOff = 0;
    
    map<vector<vector<int> >, int>::iterator iter = mmGBElements2BlockOff.find(irvGBElements);
    if (iter == mmGBElements2BlockOff.end())
    {
        mmGBElements2BlockOff[irvGBElements] = mBlockSize;
        lBlockOff = mBlockSize;
        mBlockSize += inRows;
    }
    else
    {
        lBlockOff = iter->second;
    }
    
    return lBlockOff;
}

int DSSDataModelTransactionRWField::AddGBElements2BlockOff(vector<vector<int> > &irvGBElements, int iBlockOff)
{
    mmGBElements2BlockOff[irvGBElements] = iBlockOff;
    
    return S_OK;
}

int DSSDataModelTransactionRWField::setBlockSize(int iBlockSize)
{
    return mBlockSize = iBlockSize;
}

int DSSDataModelTransactionRWField::UpdateRequiredColumnStatus()
{
    //get the "required" column info
    DSSRWDataModel *lpDataModel = mpSourceFGModel->getDataModel();
    CHECK_PTR(lpDataModel);
    DSSObjectContext* lpObjectContext = lpDataModel->getObjectContext();
    
    vector<int> lvRequiredColumnIndices;
    if(lpObjectContext)
    {
        for(int i = 0; i < mHeaderInfo.size(); i++)
        {
            mHeaderInfo[i].mbRequired = false;
        }

        DSSRWNode* lpNode = lpObjectContext->getNode(mpSourceFGModel->getNodeKey());
        lvRequiredColumnIndices = ((DSSRWFieldGroupNode*)lpNode)->getRequiredColumnIndexs();
        for(int i = 0; i < lvRequiredColumnIndices.size(); i++)
        {
            //the column index in lvRequiredColumnIndices is 1-based;
            if(lvRequiredColumnIndices[i] > 0 && lvRequiredColumnIndices[i] <=mHeaderInfo.size())
            {
                mHeaderInfo[lvRequiredColumnIndices[i] - 1].mbRequired = true;
            }
        }
    }
    return S_OK;
}

int DSSDataModelTransactionRWField::DiscardTransactionOperation(int iOriginalRow, int iOriginalColumn, int iColumnIndex)
{
    int hr = S_OK;
    
    //1. discard the transaction change
    int lAdjustedRow = iOriginalRow;
    int lAdjustedCol = iOriginalColumn;
    hAdjustOrdinal(lAdjustedRow, lAdjustedCol);
	
	RecordMap::iterator it = mUpdatedRecords.find(RecordKey(lAdjustedRow, lAdjustedCol));
    
    if(it == mUpdatedRecords.end())
    {
        //nothing to discard
        return hr;
    }
    
    TransactionRecord* lpRecord = it->second;
    int lField = lpRecord->mvDataOffset[iColumnIndex];
    //466982 & 467220, only undo changes for updated cells
    if(lField >= 0)
    {
        hr = UpdateField(lAdjustedRow, lField, iColumnIndex, NULL);
        CHECK_HR;
    }
    
    //2. clear data in the transaction record
    int lnChangedColumn = 0;
    for(int i = 0; i < lpRecord->mvpText.size(); i++)
    {
        if(lpRecord->mvpText[i])
        {
            if(i == iColumnIndex)
            {
                delete lpRecord->mvpText[i];
                lpRecord->mvpText[i] = NULL;
            }else{
                lnChangedColumn ++;
            }
        }
    }
    if(lnChangedColumn == 0)
    {
        //delete the transaction record
        lpRecord->mFlag = DssTransactionDataOriginal;

        if(mDiscardedRecords.find(it->first) == mDiscardedRecords.end())
            mDiscardedRecords.insert(it->first);
        
        mUpdatedRecords.erase(it);
    }

    return hr;
}
