/*
 *  DSSDataModelTransactionRWGrid.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 11/11/10.
 *  Copyright 2010 MicroStrategy, Inc. All rights reserved.
 *
 */

#import <set>
#import <fstream>
#import "CSITransaction/DFCCSITransactionBase.h"
#import "DSSDataModelTransactionRWGrid.h"
#import "DSSRWGridModel.h"
#import "SimpleBuffer.h"
#import "DSSViewDataSet.h"
#import "DSSTabularData.h"
#import "DSSCube.h"
#import "DSSXTabView.h"
#import "DSSXTabHeader.h"
#import "DSSTabularMetrics.h"
#import "DSSTabularConsolidation.h"
#import "DSSTabularAttribute.h"
#import "DSSRWDataModel.h"
#import "DSSRWNode.h"
#import "DSSSort_includes.h"
#import "DSSRWTemplateNode.h"
#import "DSSDataElements.h"
#import "DSSDataElement.H"
#import "CSITransaction/DFCCSIUpdate.h"
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

DSSDataModelTransactionRWGrid::DSSDataModelTransactionRWGrid():mpSourceGridModel(NULL), mpXTabView(NULL)
{
    mbNewRWD = true;
}

DSSDataModelTransactionRWGrid::~DSSDataModelTransactionRWGrid()
{
//	this->hCleanUp();
}

int DSSDataModelTransactionRWGrid::InitWithNodeModel(DSSTransactionReport *ipTransactionReport, DSSRWNodeModel *ipNodeModel)
{
    if(!ipNodeModel)
        return E_INVALIDARG;
    
    int hr = S_OK;
    mpSourceGridModel = static_cast<DSSRWGridModel*>(ipNodeModel);
    mNodeKey = mpSourceGridModel->getNodeKey();
    mnColumns = 0;
    mpTransactionReport = NULL;
    _ASSERT(mpSourceGridModel->getNodeType() == DssRWNodeTemplate);
    
	{
        DSSViewDataSet *lpViewDataSet = NULL;
        hr = mpSourceGridModel->GetViewDataSet(&lpViewDataSet);
        CHECK_HR;
        
        DSSTabularData *lpTabularData = NULL;
        hr = lpViewDataSet->getTabularData(&lpTabularData);
        CHECK_HR;
        
        mpXTabView = mpSourceGridModel->getXTabView();
        CHECK_PTR(mpXTabView);
        
        mpCube = mpSourceGridModel->getCube();
        CHECK_PTR(mpCube);
        
		mbIsMetricOnColumn = mpXTabView->getHeader(1)->HasMetric();
        
		DSSTabularMetrics *lpMetrics = NULL;
        hr = lpTabularData->getMetrics(&lpMetrics);
        CHECK_PTR(lpMetrics);
        
		int lnMetrics = lpMetrics ? lpMetrics->Count() : 0;
		
		if (mbIsMetricOnColumn)
			mColOrdinalAdjustor = lnMetrics > 0 ? lnMetrics : 1;
		else
			mRowOrdinalAdjustor = lnMetrics > 0 ? lnMetrics : 1;
        
		//481559, make a copy of original metric data slices for discard
		lpViewDataSet->GetCube()->MakeOriginalDataSliceCopy();
	}

    if(!ipTransactionReport)
    {
        mStatus = XTAB_INITIALIZED;
        return hr;
    }
    
	map<int, DSSColumnMappingInfo*>* lpColumnInfoMap = ipTransactionReport->getColumnMap();
	
	mpTransactionReport = ipTransactionReport;

	
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
    
    DSSRWDataModel *lpDataModel = NULL;
    mpSourceGridModel->getDataModel(&lpDataModel);
    CHECK_PTR(lpDataModel);
    DSSObjectContext* lpObjectContext = lpDataModel->getObjectContext();
    
    vector<int> lvRequiredColumnIndices;
    if(lpObjectContext)
    {
        DSSRWNode* lpNode = lpObjectContext->getNode(mpSourceGridModel->getNodeKey());
        lvRequiredColumnIndices = ((DSSRWTemplateNode*)lpNode)->getRequiredColumnIndexs();
        
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

int DSSDataModelTransactionRWGrid::AddRecord(int iOriginalRow, int iOriginalColumn, int iColumnIndex, MBase::CString &irNewValue, vector<AERawData> *ipvOriginalValue, vector<AERawData> *ipvDisplayValue)
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
	}
	else
	{
		lpRecord = it->second;
		lpRecord->mOrder = mSequenceNumber++;		//473062, submit the latest change last
		lpRecord->mFlag = DssTransactionDataUpdate;
	}
	
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
	
    //also copy the orignal display data for undo
    (lpRecord->mvDisplayData[iColumnIndex]).TakeOwnership();
    
    return S_OK;
}

int DSSDataModelTransactionRWGrid::setAllCommit()
{
    DSSDataModelTransaction::setAllCommit();
    
	if (mpCube)
	{
        DSSCalculationPlan *lpCalcPlan = mpSourceGridModel->getCalculationPlan();
        CHECK_PTR(lpCalcPlan);
        //tliao, 966522, 2014/09/11, need to mark sliceFlag as false
        lpCalcPlan->MarkUnCalculated(0);
        //lpCalcPlan->setStatus(DssCalcStatusUnCalculated);
        
		int hr = mpSourceGridModel->ReCalculate();

		mpSourceGridModel->setThresholdCalcuated(true);

        AE_ASSERT(hr == S_OK);
        
		//472476
		mpCube->MakeOriginalDataSliceCopy();
	}
	
	return S_OK;
}

int DSSDataModelTransactionRWGrid::UpdateDataCell(int iOriginalRow, int iOriginalColumn, int iColumnIndex, MBase::CString *ipNewValue, bool ibDisplay)
{
//	AE_ASSERT (mpSourceDataModel->getModelType() == DssModelXTab);
	
	int row = iOriginalRow;
	int col = iOriginalColumn;
	hAdjustOrdinal(row, col);
	
	if (ipNewValue)
	{
		//save the original column oridinal for discard action
		RecordMap::iterator it = mUpdatedRecords.find(RecordKey(row, col));
		if (it != mUpdatedRecords.end() && iColumnIndex >= 0)
		{		
			TransactionRecord *lpRecord = it->second;
			lpRecord->mvDataOffset[iColumnIndex] = mbIsMetricOnColumn ? iOriginalColumn : iOriginalRow;

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
		
		//473344, support set data to NULL
		char *lpData = NULL;
		if (ipNewValue->length() > 0)
			lpData = (char*)(ipNewValue->c_str());
		return mpXTabView->setCellValueWithLookup(iOriginalRow, iOriginalColumn, lpData, ipNewValue->size(), DssDataOk);
	}
	else
	{
		AE_ASSERT(iColumnIndex >= 0);
        
		//it's for undo.
		RecordMap::iterator it = mUpdatedRecords.find(RecordKey(row, col));
		AE_ASSERT (it != mUpdatedRecords.end());
        
		TransactionRecord *lpRecord = it->second;
        
		//reset metric slice data with raw data
		bool lbIsRawData = true;

        //DDIC where wb form different from display form
        if(!(lpRecord->mvDisplayData[iColumnIndex].mValue == NULL 
             && lpRecord->mvDisplayData[iColumnIndex].mSizeInByte < 0
             && lpRecord->mvDisplayData[iColumnIndex].mDataType == DssDataTypeUnknown))
        {
            return mpXTabView->setCellValueWithLookup(iOriginalRow, iOriginalColumn,
                                            lpRecord->mvDisplayData[iColumnIndex].mValue,
                                            lpRecord->mvDisplayData[iColumnIndex].mSizeInByte,
                                            lpRecord->mvDisplayData[iColumnIndex].mFlag,
                                            lbIsRawData);
            
        }
        else
        {
            return mpXTabView->setCellValueWithLookup(iOriginalRow, iOriginalColumn,
                                        lpRecord->mvOriginalData[iColumnIndex].mValue,
                                        lpRecord->mvOriginalData[iColumnIndex].mSizeInByte,
                                        lpRecord->mvOriginalData[iColumnIndex].mFlag,
                                        lbIsRawData);
        }
	}
}

int DSSDataModelTransactionRWGrid::UpdateHeaderCell(int iOriginalRow, int iOriginalColumn, int iColumnIndex, DSSTabularUnit *ipXTabunit, int iBaseForm, int iOffset, MBase::CString *ipNewValue, bool ibDisplay)
{
	if (iColumnIndex < 0 || iColumnIndex >= mnColumns + mnOfflineColumns)
		return E_INVALIDINDEX;
	
	hAdjustOrdinal(iOriginalRow, iOriginalColumn);
	
	int lOriginalOffset = iOffset; //for DE
	
	if(ipXTabunit->getUnitType() == DssTemplateConsolidation)	//474246, handle DE
	{
		iOffset = static_cast<DSSTabularConsolidation*> (ipXTabunit)->getChildElementKey(iOffset);
		ipXTabunit = static_cast<DSSTabularConsolidation*> (ipXTabunit)->getChildUnit();
	}
	
	DSSTabularAttribute *lpAttribute = NULL;
	if (ipXTabunit->getUnitType() == DssTemplateAttribute)
		lpAttribute = static_cast<DSSTabularAttribute*> (ipXTabunit);
	CHECK_PTR(lpAttribute);
	
    //TODO:
	//473061, clear bucket sort cache
//	static_cast<DSSDataModelXTabImpl*> (mpSourceDataModel)->deleteSorts();
	
	if (ipNewValue)
	{
		AERawData *lpOrignalData = NULL;
        AERawData *lpOrignalDisplayData = NULL;
		if (iColumnIndex < mHeaderInfo.size() && !mHeaderInfo[iColumnIndex].mpTabularUnit)
		{
			mHeaderInfo[iColumnIndex].mpTabularUnit = ipXTabunit;
			mHeaderInfo[iColumnIndex].mBaseForm = iBaseForm;
		}else if (iColumnIndex - mHeaderInfo.size() < mvOfflineColumns.size() && !mvOfflineColumns[iColumnIndex].mpTabularUnit)
        {
            mvOfflineColumns[iColumnIndex - mHeaderInfo.size()].mpTabularUnit = ipXTabunit;
			mvOfflineColumns[iColumnIndex - mHeaderInfo.size()].mBaseForm = iBaseForm;
        }
		else
		{
			//it's an already changed attribute forms, loop through to get the real original data
			for (RecordMap::iterator it = mUpdatedRecords.begin(); it != mUpdatedRecords.end(); it++)
			{
				TransactionRecord *lpRecord = it->second;
				if (lpRecord->mvDataOffset[iColumnIndex] == lOriginalOffset)
				{
					lpOrignalData = &(lpRecord->mvOriginalData[iColumnIndex]);
                    lpOrignalDisplayData = &(lpRecord->mvDisplayData[iColumnIndex]);
					break;
				}
			}
		}
        
		//save the offset for discard action
		RecordMap::iterator it = mUpdatedRecords.find(RecordKey(iOriginalRow, iOriginalColumn));
		if (it != mUpdatedRecords.end())
		{		
			TransactionRecord *lpRecord = it->second;
			lpRecord->mvDataOffset[iColumnIndex] = lOriginalOffset;
			
			if (lpOrignalData)
			{
				//473412, get the original attribute form value from previous transaction record
				lpOrignalData->CopyTo(lpRecord->mvOriginalData[iColumnIndex]);
			}
            if (lpOrignalDisplayData)
            {
                lpOrignalDisplayData->CopyTo(lpRecord->mvDisplayData[iColumnIndex]);
            }
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
		
		return lpAttribute->setData(iBaseForm, iOffset, *ipNewValue);
	}
	else
	{
		//it's for undo.
		RecordMap::iterator it = mUpdatedRecords.find(RecordKey(iOriginalRow, iOriginalColumn));
		AE_ASSERT (it != mUpdatedRecords.end());
		TransactionRecord *lpRecord = it->second;
        
        //DDIC where wb form different from display form
        if(!(lpRecord->mvDisplayData[iColumnIndex].mValue == NULL 
             && lpRecord->mvDisplayData[iColumnIndex].mSizeInByte < 0
             && lpRecord->mvDisplayData[iColumnIndex].mDataType == DssDataTypeUnknown))
        {
            return lpAttribute->setData(iBaseForm, iOffset,
                                        lpRecord->mvDisplayData[iColumnIndex].mFlag == DssDataNull? NULL:lpRecord->mvDisplayData[iColumnIndex].mValue,
                                        lpRecord->mvDisplayData[iColumnIndex].mFlag == DssDataNull? 0:lpRecord->mvDisplayData[iColumnIndex].mSizeInByte);
            
        }
        else
        {
            return lpAttribute->setData(iBaseForm, iOffset,
                                        lpRecord->mvOriginalData[iColumnIndex].mFlag == DssDataNull? NULL:lpRecord->mvOriginalData[iColumnIndex].mValue,
                                        lpRecord->mvOriginalData[iColumnIndex].mFlag == DssDataNull? 0:lpRecord->mvOriginalData[iColumnIndex].mSizeInByte);
        }
	}
	
}

int DSSDataModelTransactionRWGrid::Discard(bool ibRestore)
{
	if (mStatus != XTAB_INITIALIZED)
		return E_FAIL;		//472924
    
	int hr = S_OK;
	
	//undo changes
	{
/*		for (RecordMap::iterator it = mUpdatedRecords.begin(); it != mUpdatedRecords.end(); it++)
		{
			TransactionRecord *lpRecord = it->second;
			int lOriginalRow = it->first.first;
			int lOriginalColumn = it->first.second;
			
			hr = hUpdateCells(lpRecord, lOriginalRow, lOriginalColumn, true);
			CHECK_HR;
		}//end of for
*/		
        vector<int> lvUpdatedRecords;
        map<int,int> lvOriginalRows;
        map<int,int> lvOriginalCols;
        for (RecordMap::iterator it = mUpdatedRecords.begin(); it != mUpdatedRecords.end(); it++)
		{
			TransactionRecord *lpRecord = it->second;
			
            for(int i = 0;i < mvpRecord.size(); i++)
            {
                if(mvpRecord[i] == lpRecord)
                {
                    lvUpdatedRecords.push_back(i);
                    lvOriginalRows[i] = it->first.first;
                    lvOriginalCols[i] = it->first.second;
                    break;
                }
            }
		}//end of for
		
        if (lvUpdatedRecords.size() > 0)
        {
            _xtab_sort(&lvUpdatedRecords[0], &lvUpdatedRecords[0] + lvUpdatedRecords.size(), *this);
        }
        
        for(int i = lvUpdatedRecords.size() - 1; i >=0; i--)
        {
            hr = hUpdateCells(mvpRecord[lvUpdatedRecords[i]], lvOriginalRows[lvUpdatedRecords[i]], lvOriginalCols[lvUpdatedRecords[i]], true);
			CHECK_HR;
        }
        
        DSSDataModelTransaction::hCleanUp(true);		//475902, clean up the transaction records before manipulate calculation plan
		
        //742212
		if (ibRestore)
        {
            //472476
            hr = mpCube->RestoreOriginalDataSlice();
            CHECK_HR;
        }
		
		DSSCalculationPlan *lpCalcPlan = mpSourceGridModel->getCalculationPlan();
		CHECK_PTR(lpCalcPlan);
		
		//data is already restored, no need to recalculate
        //TQMS 613533,689419
		lpCalcPlan->setStatus(DssCalcStatusNeedEvalThres);
        
        //zhyang, 677674
        if(mpSourceGridModel)
        {
            DSSViewDataSet *lpViewDataSet;
            hr = mpSourceGridModel->GetViewDataSet(&lpViewDataSet);
            CHECK_HR;
            
            hr = lpViewDataSet->getTabularData()->enableSSOForAllUnits();
            CHECK_HR
            
            mpSourceGridModel->getXTabView()->DeleteSortDefinition();
		}
	}//end of if (mpSourceDataModel->getModelType() == DssModelXTab)
    
	return S_OK;
}

int hIsDataModelMatch2(DSSRWGridModel *ipSourceGridModel, DSSRWGridModel *ipThisGridModel)
{
    int hr = S_OK;
    
    DSSViewDataSet *lpThisViewDataSet = NULL; 
    hr = ipThisGridModel->GetViewDataSet(&lpThisViewDataSet);
    CHECK_HR;
    
    DSSViewDataSet *lpSourceViewDataSet = NULL; 
    hr = ipSourceGridModel->GetViewDataSet(&lpSourceViewDataSet);
    CHECK_HR;
    
	int lnUnits = lpSourceViewDataSet->Count();
	if (lpThisViewDataSet->Count() != lnUnits) 
		return S_FALSE; 
	
	int i = 0; 
	for (i = 0; i < lnUnits; i++) 
	{ 
		DSSTabularUnit *lpXTabUnit1 = lpThisViewDataSet->Item(i); 
		DSSTabularUnit *lpXTabUnit2 = lpSourceViewDataSet->Item(i); 
		if (lpXTabUnit1->getUnitType() != lpXTabUnit2->getUnitType() 
			|| !MBase::IsEqualGUID(lpXTabUnit1->getGUID(), lpXTabUnit2->getGUID())) 
			return S_FALSE; 
	} 
    return hr; 
}

int DSSDataModelTransactionRWGrid::Init(DSSDataModelTransaction *ipTransactionModel, DSSRWNodeModel *ipThisNodeModel)
{
    DSSDataModelTransactionRWGrid *lpSourceTransactionModel = dynamic_cast<DSSDataModelTransactionRWGrid*>(ipTransactionModel);
    
	if (!lpSourceTransactionModel || !ipThisNodeModel)
		return E_INVALIDARG;
    
    if (ipThisNodeModel->getNodeType() != DssRWNodeTemplate || lpSourceTransactionModel->mpSourceGridModel->getNodeType() != ipThisNodeModel->getNodeType())
        return S_FALSE;
	
	int hr = ipThisNodeModel->EnsureModelData();
	CHECK_HR;

    mpTransactionReport = ipTransactionModel->getTransactionReport();
    mpCSI = ipTransactionModel->getCSI();
	//check if two models (mpSourceGridModel) are match
    DSSRWGridModel *lpThisGridModel = dynamic_cast<DSSRWGridModel*>(ipThisNodeModel);
    CHECK_PTR(lpThisGridModel);
    
	hr = hIsDataModelMatch2(lpSourceTransactionModel->mpSourceGridModel, lpThisGridModel);
	CHECK_HR;
    
	hr = this->InitWithNodeModel(lpSourceTransactionModel->mpTransactionReport, ipThisNodeModel);
	CHECK_HR;
	
	hr = hPopulateHeaderInfo2(lpSourceTransactionModel);
	CHECK_HR;
	
	hr = hPopulateTransactionRecordsForGrid(ipTransactionModel, lpSourceTransactionModel->mpSourceGridModel, lpThisGridModel);
	CHECK_HR;
	
    mbIsValidated = isModelValidated(-1);
    mvValidatedRows.clear();
    vector<int>* lpValidatedRows = ipTransactionModel->getValidatedRows();
    if(lpValidatedRows)
        mvValidatedRows.assign(lpValidatedRows->begin(),lpValidatedRows->end());
    
	return S_OK;
}

int DSSDataModelTransactionRWGrid::hPopulateHeaderInfo2(DSSDataModelTransactionRWGrid *ipTransactionModel)
{
    DSSViewDataSet *lpThisViewDataSet = NULL;
    int hr = mpSourceGridModel->GetViewDataSet(&lpThisViewDataSet);
    CHECK_PTR(lpThisViewDataSet);
    
	int i = 0;
	for (i = 0; i <mnColumns; i++)
	{
		DSSTabularUnit *lpUnitSrc = ipTransactionModel->mHeaderInfo[i].mpTabularUnit;
		if (lpUnitSrc)
		{
			int lUnitID = -1;
            hr = lpThisViewDataSet->getTabularData()->FindUnitI(lpUnitSrc->getGUID(), lpUnitSrc->getUnitType(), lUnitID);
			if(lUnitID <0 && lpUnitSrc->getUnitType() == DssTemplateAttribute)
			{
				//the unit may be a DE
				hr = lpThisViewDataSet->getTabularData()->FindUnitI(lpUnitSrc->getGUID(),DssTemplateConsolidation, lUnitID);
			}
			AE_ASSERT(lUnitID >= 0);
			mHeaderInfo[i].mpTabularUnit = lpThisViewDataSet->Item(lUnitID);
		}
		else
		{
			mHeaderInfo[i].mpTabularUnit = NULL;
		}
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
        DSSTabularUnit *lpUnitSrc = ipTransactionModel->mvOfflineColumns[i].mpTabularUnit;
        if(lpUnitSrc)
        {
            int lUnitID = -1;
            hr = lpThisViewDataSet->getTabularData()->FindUnitI(lpUnitSrc->getGUID(), lpUnitSrc->getUnitType(), lUnitID);
			if(lUnitID <0 && lpUnitSrc->getUnitType() == DssTemplateAttribute)
			{
				//the unit may be a DE
				hr = lpThisViewDataSet->getTabularData()->FindUnitI(lpUnitSrc->getGUID(),DssTemplateConsolidation, lUnitID);
			}
			AE_ASSERT(lUnitID >= 0);
			mvOfflineColumns[i].mpTabularUnit = lpThisViewDataSet->Item(lUnitID);
        }
        mvOfflineColumns[i].mpDefinition = NULL;
    }
    mapOfflineTrxColumn2Metric.assign(ipTransactionModel->mapOfflineTrxColumn2Metric.begin(), ipTransactionModel->mapOfflineTrxColumn2Metric.end());
    mapOfflineTrxColumn2WriteBackForm.assign(ipTransactionModel->mapOfflineTrxColumn2WriteBackForm.begin(), ipTransactionModel->mapOfflineTrxColumn2WriteBackForm.end());
    mapOfflineTrxColumn2XTabUnit.assign(ipTransactionModel->mapOfflineTrxColumn2XTabUnit.begin(), ipTransactionModel->mapOfflineTrxColumn2XTabUnit.end());
    mapOfflineTrxColumn2TrxColumn.assign(ipTransactionModel->mapOfflineTrxColumn2TrxColumn.begin(),ipTransactionModel->mapOfflineTrxColumn2TrxColumn.end());
	return S_OK;
}

int DSSDataModelTransactionRWGrid::GenerateRecordFromXTabView(DSSXTabView *ipXTabView, int iRow, int iCol, vector<AERawData> &orvRawData, vector<AERawData> &orvDisplayData)
{
    if(mnOfflineColumns == 0)
    {
       return ipXTabView->GenerateRecordWithLookup(iRow, iCol, mapTrxColumn2XTabUnit, mapTrxColumn2Metric, mapTrxColumn2WriteBackForm, orvRawData, orvDisplayData);
    }else
    {
        int lnColumns = mnColumns + mnOfflineColumns;
        int lnTrxColumns = mnColumns;
        int lnOfflineTrxColumns = mnOfflineColumns;
        vector<int> lMapTrx2XTabUnit(lnColumns, -1);
        vector<int> lMapTrx2BForm(lnColumns, -1);
        vector<int> lMapTrx2WBForm(lnColumns, -1);
        memmove(&lMapTrx2XTabUnit[0], &mapTrxColumn2XTabUnit[0],sizeof(int) * lnTrxColumns);
        memmove(&lMapTrx2XTabUnit[lnTrxColumns], &mapOfflineTrxColumn2XTabUnit[0], sizeof(int)*lnOfflineTrxColumns);
        memmove(&lMapTrx2BForm[0], &mapTrxColumn2Metric[0],sizeof(int) * lnTrxColumns);
        memmove(&lMapTrx2BForm[lnTrxColumns], &mapOfflineTrxColumn2Metric[0], sizeof(int)*lnOfflineTrxColumns);
        memmove(&lMapTrx2WBForm[0], &mapTrxColumn2WriteBackForm[0],sizeof(int) * lnTrxColumns);
        memmove(&lMapTrx2WBForm[lnTrxColumns], &mapOfflineTrxColumn2WriteBackForm[0], sizeof(int)*lnOfflineTrxColumns);
        
        return ipXTabView->GenerateRecordWithLookup(iRow, iCol, lMapTrx2XTabUnit, lMapTrx2BForm, lMapTrx2WBForm, orvRawData, orvDisplayData);

    }
}

int DSSDataModelTransactionRWGrid::hCheckMetricColumnData()
{
	if (mpSourceGridModel->getXTabView())
    {
        return DSSDataModelTransaction::hCheckMetricColumnData(mpSourceGridModel->getXTabView());
    }
    return S_OK;
}

int DSSDataModelTransactionRWGrid::hMapRowAndColumns(int iRow, int iCol, vector<int> &irvKeys, DSSXTabView *ipXTabViewSrc, int &orRow, int &orCol)
{
	int lnUnits = mpSourceGridModel->Count();
	int lMetricUnitID = mpSourceGridModel->getMetricUnitID();
    
    return DSSDataModelTransaction::hMapRowAndColumns(iRow, iCol, irvKeys, ipXTabViewSrc, mpXTabView, orRow, orCol, lnUnits, lMetricUnitID);
}

int DSSDataModelTransactionRWGrid::hSaveModelInfoToBinary(DSSBinaryDataWriter &irWriter, int &orSourceModelType)
{
    int hr = S_OK;
	//current slice flag 
	int currentSlice = mpSourceGridModel->getCurrentSliceFlag(); 
	hr = irWriter.WriteInt(currentSlice);
	CHECK_HR; 
	
	//source model type 
    orSourceModelType = DssModelXTab;
	hr = irWriter.WriteInt(orSourceModelType);
	CHECK_HR; 
    
	return S_OK;
}

int DSSDataModelTransactionRWGrid::UpdateRequiredColumnStatus()
{
    //get the "required" column info
  	DSSRWDataModel *lpDataModel = NULL;
    mpSourceGridModel->getDataModel(&lpDataModel);
    CHECK_PTR(lpDataModel);
    DSSObjectContext* lpObjectContext = lpDataModel->getObjectContext();
    
    vector<int> lvRequiredColumnIndices;
    if(lpObjectContext)
    {
        for(int i = 0; i < mHeaderInfo.size(); i++)
        {
            mHeaderInfo[i].mbRequired = false;
        }

        DSSRWNode* lpNode = lpObjectContext->getNode(mpSourceGridModel->getNodeKey());
        lvRequiredColumnIndices = ((DSSRWTemplateNode*)lpNode)->getRequiredColumnIndexs();
        
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

int DSSDataModelTransactionRWGrid::DiscardTransactionOperation(int iOriginalRow, int iOriginalColumn, int iColumnIndex)
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
    hr = hUpdateCells(lpRecord, lAdjustedRow, lAdjustedCol, true, iColumnIndex);
    CHECK_HR;
    
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
        mUpdatedRecords.erase(it);
    }
    
    //3. calculation related
    DSSCalculationPlan *lpCalcPlan = mpSourceGridModel->getCalculationPlan();
    CHECK_PTR(lpCalcPlan);
    
    lpCalcPlan->setStatus(DssCalcStatusUnCalculated);
    
    //zhyang, 677674
    if(mpSourceGridModel)
    {
        DSSViewDataSet *lpViewDataSet;
        hr = mpSourceGridModel->GetViewDataSet(&lpViewDataSet);
        CHECK_HR;
        
        hr = lpViewDataSet->getTabularData()->enableSSOForAllUnits();
        CHECK_HR
        
        mpSourceGridModel->getXTabView()->DeleteSortDefinition();
    }

    return hr;
}
/*
int DSSDataModelTransactionRWGrid::hPopulateTransactionRecords(DSSDataModelTransactionRWGrid *ipTransactionModel)
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

int DSSDataModelTransactionRWGrid::hPopulateOneRecordForFieldGroup(DSSDataModelFieldGroupImpl* ipFGModelSrc, TransactionRecord * ipRecordSrc)
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

int DSSDataModelTransactionRWGrid::hPopulateOneRecord(DSSXTabView *ipXTabViewSrc, DSSXTabView *ipXTabView,
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

int DSSDataModelTransactionRWGrid::hMapRowAndColumns(int iRowOrdinalSrc, int iColOrdinalSrc,
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

int DSSDataModelTransactionRWGrid::hMapRowForFieldGroup(DSSDataModelFieldGroupImpl* ipFGModelSrc, int& iRow)
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