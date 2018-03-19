/*
 *  DSSDataModelTransaction.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 11/11/10.
 *  Copyright 2010 MicroStrategy, Inc. All rights reserved.
 *
 */
#import <set>

#import "DSSDataModelTransaction.h"
#import "DSSBinaryDataReader.h"
#import "DSSTemplate.h"
#import "DSSRWTemplateNode.h"
#import "DSSRWFieldGroupNode.h"
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
#import "DSSDataElements.h"
#include "DSSRWGridModel.h"
#import "DSSDataSource.h"
#import "DSSViewDataSet.h"
#include "CSITransaction/DFCCSITransactionBase.h"

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
//const char Attr_TRS_ColumnValue[] = "value";
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
	
    EnumDSSObjectType lType = ipColumnInfo ? ipColumnInfo->getType() : DssTypeReserved;
	orStrStream << Attr_TRS_Type << ATT_VAL(lType);	//type = "6"
	
	GUID lUnitID = ipColumnInfo ? ipColumnInfo->getAttributeID() : GUID_NULL;
	if (ipColumnInfo && ipColumnInfo->getType() == DssTypeMetric)
		lUnitID = ipColumnInfo->getMetricID();
	
	orStrStream << Attr_TRS_UnitID << ATT_VAL(DSSAEHelper::Guid2String(lUnitID).c_str());	//unitid = "8d679D4AABCDEF12345678"
	
	orStrStream << Attr_TRS_DataType << ATT_VAL(iDataType);				//data_type = "6"

    
	if (lType == DssTypeAttributeForm)				//baseform = "0"
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

void DSSDataModelTransaction::ConvertDateTime(MBase::CString &orText)
{

#ifdef __ANDROID__
	return;
#endif

	int year=0, month=0, day=0, hour=0, minute=0, second=0;
	int n=sscanf(orText.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
	
	char lBuffer[32];
	if(n==6) // date & time
	{
		snprintf(lBuffer, 32, "%d/%d/%d %d:%02d:%02d", month, day, year, hour, minute, second);
		orText = lBuffer;
	}
	else if(n==3)  // date only
	{
		snprintf(lBuffer, 32, "%d/%d/%d", month, day, year);
		orText = lBuffer;
	}
	else //time only
	{
		n=sscanf(orText.c_str(), "%d:%d:%d", &hour, &minute, &second);
		if (n == 3)
		{
			snprintf(lBuffer, 32, "%d:%02d:%02d", hour, minute, second);
			orText = lBuffer;
		} 
        else
        {
            char am[20];
            memset(am, 0, 20);
            if(sscanf(orText.c_str(), "%d/%d/%d %d:%02d:%02d %s", &month, &day,&year, &hour, &minute, &second, am) == 7)
            {
                std::string amStr = am;
                if(amStr == "AM" && hour >=12)
                    hour -= 12;
                else if(amStr == "PM" && hour < 12)
                    hour += 12;
                
                snprintf(lBuffer, 32, "%d/%d/%d %d:%02d:%02d", month, day, year, hour, minute, second);
                orText = lBuffer;
            }
            else if (sscanf(orText.c_str(), "%d/%d/%d %d:%02d:%02d", &month, &day,&year, &hour, &minute, &second) == 6)
            {
                snprintf(lBuffer, 32, "%d/%d/%d %d:%02d:%02d", month, day, year, hour, minute, second);
                orText = lBuffer;
            }
        };
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
		HRESULT hr = DSSAEHelper::ConvertToNumeric(irRawData.mValue, irRawData.mSizeInByte,
									  irRawData.mDataType, irRawData.mFlag, lVal);
		
       
        MDataType::DSSDateTime lDataTime(lVal);
        int lDataSize = 0;
        //617352
        if(hr == E_FAIL && irRawData.mDataType == DssDataTypeVarChar)
        {
            DSSAEHelper::ConvertStringToData(irRawData.mValue, DssDataTypeTimeStamp, &lDataTime, sizeof(MDataType::DSSDateTime) ,lDataSize);
        }
		
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
            && irHeaderInfo[i].mpDefinition
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
                EnumDSSDataType lType = lpRawData->mDataType;
                if(lType == DssDataTypeTime)
                    lType = DssDataTypeTimeStamp;
				DSSAEHelper::ConvertToString(lpRawData->mValue, lpRawData->mSizeInByte, lType, lpRawData->mFlag, lText);
			}
		}
		else	//updated data
		{
            lText = *ipRecord->mvpText[i];
            /*
            if(ipRecord->mvDisplayData[i].mDataType == DssDataTypeTime)
            {
                MDataType::DSSDateTime* date=(MDataType::DSSDateTime*)ipRecord->mvDisplayData[i].mValue;
                if(date)
                {
                    int year = date->GetYear();
                    if(year > 1899)
                    {
                        int month = date->GetMonth();
                        int day = date->GetDay();
                        
                        char* lpBuffer = new char [sizeof(MDataType::DSSDateTime)];
                        int lSize = 0;
                        int hr = DSSAEHelper::ConvertStringToData(lText.c_str(), DssDataTypeTime, lpBuffer,sizeof(MDataType::DSSDateTime), lSize);
                        if(hr == S_OK)
                        {
                            MDataType::DSSDateTime* dispTime = (MDataType::DSSDateTime*)lpBuffer;
                            dispTime->PutYear(year);
                            dispTime->PutMonth(month);
                            dispTime->PutDay(day);
                            DSSAEHelper::ConvertToString(dispTime, sizeof(MDataType::DSSDateTime), DssDataTypeTimeStamp, DssDataOk, lText);
                        }
                    }
                }
            }
			*/
			lType = DssTransactionDataUpdate;
		}
		
		orStrStream << Attr_TRS_Type << ATT_VAL(lType) << CLOSE_TAG;	// type = "update" >
		WriteColumnData(lText, orStrStream);							//<value> XXX </value>
		orStrStream << END_TAG(Tag_TRS_Column);							//</column>
	}
	orStrStream << END_TAG(Tag_TRS_Columns);							//</columns>
	
	orStrStream << END_TAG(Tag_TRS_Record);								//</record>
}

DSSDataModelTransaction::DSSDataModelTransaction():mpTransactionReport(NULL), mpSourceDataModel(NULL), mnColumns(0)
,mStatus(XTAB_UNINITIALIZED), mRowOrdinalAdjustor(1), mColOrdinalAdjustor(1), mSequenceNumber(0), mbIsMetricOnColumn(true)
,mbMarkRowEnabled(false),mbIsValidated(false),mbNewRWD(false),mbHasOfflineTrx(false),mnOfflineColumns(0),mpCSI(NULL),mGlobalLookupDataVer(-1)
{}

DSSDataModelTransaction::~DSSDataModelTransaction()
{
	this->hCleanUp();
    mvValidatedRows.clear();
}

int DSSDataModelTransaction::Init(DSSTransactionReport *ipTransactionReport, DSSDataModelBase *ipDataModel)
{
    if (!ipTransactionReport || !ipDataModel)
        return E_INVALIDARG;
    
	map<int, DSSColumnMappingInfo*>* lpColumnInfoMap = ipTransactionReport->getColumnMap();
	
	mpTransactionReport = ipTransactionReport;
	mpSourceDataModel = ipDataModel;

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
    for(int i = 0; i < lnTargetColumns; i++)
    {
		DSSColumnMappingInfo *lpColumnInfo = lpColumnInfoMap->find(i+1) != lpColumnInfoMap->end() ? (*lpColumnInfoMap)[i + 1] : NULL;	//column mapping info is 1-based
//		AE_ASSERT(lpColumnInfo);

		mHeaderInfo[i].mpDefinition = lpColumnInfo;
		
		mHeaderInfo[i].mDataType = lpColumnInfo ? lpColumnInfo->getDataType() : DssDataTypeReserved;
		
		mHeaderInfo[i].mpTabularUnit = NULL;
		
		mHeaderInfo[i].mBaseForm = 0;
        
        mHeaderInfo[i].mbRequired = false;
	}

	if (mpSourceDataModel->getModelType() == DssModelXTab)
	{
		DSSDataModelXTabImpl *lpXTabModel = static_cast<DSSDataModelXTabImpl*> (mpSourceDataModel);
		mbIsMetricOnColumn = lpXTabModel->getXTabView()->getHeader(1)->HasMetric();
		DSSTabularMetrics *lpMetrics = NULL;
		lpXTabModel->getMetricsUnit(&lpMetrics);
		int lnMetrics = lpMetrics ? lpMetrics->Count() : 0;
		
		if (mbIsMetricOnColumn)
			mColOrdinalAdjustor = lnMetrics > 0 ? lnMetrics : 1;
		else
			mRowOrdinalAdjustor = lnMetrics > 0 ? lnMetrics : 1;

		//481559, make a copy of original metric data slices for discard
		lpXTabModel->getCube()->MakeOriginalDataSliceCopy();
	}
	else
	{
		mColOrdinalAdjustor = static_cast<DSSDataModelFieldGroupImpl*> (mpSourceDataModel)->CountFields();
	}
    //get the "required" column info
  	DSSObjectContext* lpObjectContext = mpSourceDataModel->getObjectContext();
    vector<int> lvRequiredColumnIndices;
    if(lpObjectContext)
    {
        DSSRWNode* lpNode = lpObjectContext->getNode(mpSourceDataModel->getNodeKey());
        if(lpNode->getType() == DssRWNodeTemplate)        
        {
            lvRequiredColumnIndices = ((DSSRWTemplateNode*)lpNode)->getRequiredColumnIndexs();
        }
        else if(lpNode->getType() == DssRWNodeFieldGroup)
        {
            lvRequiredColumnIndices = ((DSSRWFieldGroupNode*)lpNode)->getRequiredColumnIndexs();
        }
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

int DSSDataModelTransaction::Init(DSSDataModelTransaction *ipSourceTransactionModel, DSSRWNodeModel *ipNodeModel)
{
    return E_NOTIMPL;      //it's for new rwd engine
}

int DSSDataModelTransaction::InitWithNodeModel(DSSTransactionReport *ipTransactionReport, DSSRWNodeModel *ipNodeModel)
{
    return E_NOTIMPL;
}

int DSSDataModelTransaction::SetGlobalLookupDataVer(int iVer)
{
    mGlobalLookupDataVer = iVer;
    return S_OK;
}

int DSSDataModelTransaction::GetGlobalLookupDataVer()
{
    return mGlobalLookupDataVer;
}

int DSSDataModelTransaction::AddRecord(int iOriginalRow, int iOriginalColumn, int iColumnIndex, MBase::CString &irNewValue, vector<AERawData> *ipvOriginalValue,vector<AERawData> *ipvDisplayValue)
{
	if (iColumnIndex < 0 || iColumnIndex >= mHeaderInfo.size())
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
        
        //592678 
#ifdef __ANDROID__
        if(mbMarkRowEnabled == true)
            lpRecord->mFlag = DssTransactionDataOriginal;
#endif
	}
	else
	{
		lpRecord = it->second;
		lpRecord->mOrder = mSequenceNumber++;		//473062, submit the latest change last
#ifdef __ANDROID__
        if(mbMarkRowEnabled != true)
#endif
		lpRecord->mFlag = DssTransactionDataUpdate;
	}
	
	if (mHeaderInfo[iColumnIndex].mDataType == DssDataTypeTime
		|| mHeaderInfo[iColumnIndex].mDataType == DssDataTypeDate
		|| mHeaderInfo[iColumnIndex].mDataType == DssDataTypeTimeStamp)
		ConvertDateTime(irNewValue);
	
    //558429, the originalFieldText obtained from getFieldText may be empty string
    //so we'd better use mvpText[i] to see whether column i has been modified
    if (mpSourceDataModel->getModelType() == DssModelFieldGroup && !lpRecord->mvpText[iColumnIndex])
        //lpRecord->mvOriginalFieldText[iColumnIndex].length() == 0)
	{
		//473405 copy the original field text
		static_cast<DSSDataModelFieldGroupImpl*>(mpSourceDataModel)->getFieldText(iOriginalRow, iOriginalColumn, lpRecord->mvOriginalFieldText[iColumnIndex]);
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

int DSSDataModelTransaction::GetOriginalValue(int iOriginalRow, int iOriginalColumn, int iColumnIndex, AERawData& opOriginalValue)
{
    if (iColumnIndex < 0 || iColumnIndex >= mHeaderInfo.size())
        return E_INVALIDINDEX;
    
    int lAdjustedRow = iOriginalRow;
    int lAdjustedCol = iOriginalColumn;
    hAdjustOrdinal(lAdjustedRow, lAdjustedCol);
    
    
    RecordMap::iterator it = mUpdatedRecords.find(RecordKey(lAdjustedRow, lAdjustedCol));
    //TransactionRecord *lpRecord = NULL;
    if (it != mUpdatedRecords.end())
    {
        opOriginalValue = it->second->mvDisplayData[iColumnIndex];
    }
    return S_OK;
}

int DSSDataModelTransaction::generateTransactionDelta(vector<DSS_ID>& ovDataSetID, vector<unsigned char*>& ovDelta, vector<int>& ovDeltaSize)
{
    int hr = 0;

    ovDataSetID.clear();
    ovDelta.clear();
    ovDeltaSize.clear();
    if(mpTransactionReport || mpCSI)
    {
        DFCCSITransactionBase* lpCSI = mpCSI ? mpCSI : mpTransactionReport->getTransactionCSI();
        if(lpCSI)
        {
            hCheckMetricColumnData();	//msun, 6/3/2015, pick up derived metric changes due to re-calculation
            
            MBase::Buffer* lpTempBuffer = MBase::CreateBuffer(1, 1);
            DSS_ID lTargetID = lpCSI->GetDatasetDSSID();

            int lnRecords = GetTransactionCount();
            int lnColumns = GetColumnCount();
            int lnOfflineColuns = GetOfflineColumnCount();
            
            //hard code, delete after object model change is done
            if(!mpCSI && lnOfflineColuns == 0)
            {
                lnOfflineColuns = lnColumns;
                lnColumns = 0;
            }
            vector<vector<pair<void*, EnumDSSDataType> > > lvvRecords;
            for(int i = 0; i < lnRecords; i++)
            {
				std::vector<std::pair<void*, EnumDSSDataType> > lvRecord;
                for(int j = lnColumns; j < lnColumns + lnOfflineColuns; j++)
                {
					string lText = "";
                    EnumDSSDataType lType = DssDataTypeReserved;
                    hr = GetValue(i, j, &lText, &lType);
                    CHECK_HR;
                    
                    //generate the pair, convert string to DFCString
#ifdef NO_WCHAR_SUPPORT
					// tehe: make_pair will fail on android, because definition of MBase::String
                   // lvRecord.push_back(make_pair(lText, lType));
#else
                    
					if (!DSSAEHelper::IsDate(lType))
                    {
                        MBase::ReturnWString lwText = MBase::UTF8ToWideChar(lText.c_str());
                        //DFCString lwText(lText.begin(), lText.end());
                        DFCStringAllocator lStrAllocator(lpTempBuffer);
                        DFCString* lpwText = new (lpTempBuffer)DFCString(lwText.c_str(), lStrAllocator);
                        
                        lvRecord.push_back(make_pair(reinterpret_cast<void *>(lpwText), lType));
                    }
                    else //fali, 938399, change string to DateTime type and pass the pair to ReplacePlaceholder
                    {
                        int lDataTimeSize = 0;
                        DSSAEHelper::SizeOfType(lType, &lDataTimeSize);
                        char* lpDataBuffer = new (lpTempBuffer) char[lDataTimeSize];
                        
                        int lDataSize;
                        hr = DSSAEHelper::ConvertStringToData(lText.c_str(), lType, lpDataBuffer, lDataTimeSize, lDataSize);
                        CHECK_HR;
                        MDataType::DSSDateTime* lpDSSDateTime = static_cast<MDataType::DSSDateTime*>((void*)lpDataBuffer);
                        MDataType::DateTime* lpDateTime = new (lpTempBuffer) MDataType::DateTime(lpDSSDateTime->GetYear(),
                                                                                                 lpDSSDateTime->GetMonth(),
                                                                                                 lpDSSDateTime->GetDay(),
                                                                                                 lpDSSDateTime->GetHour(),
                                                                                                 lpDSSDateTime->GetMinute(),
                                                                                                 lpDSSDateTime->GetSecond(),
                                                                                                 lpDSSDateTime->GetMillisecond());
                        
                        lvRecord.push_back(make_pair(lpDateTime, lType));
                        
                    }
#endif
                }
                lvvRecords.push_back(lvRecord);
            }
            vector<DFCCSITransactionBase*> lpReplaceds = lpCSI->ReplacePlaceholder(lvvRecords);
            delete lpTempBuffer;
            lpTempBuffer = NULL;
            
            ovDataSetID.push_back(lTargetID);
            
            DSSBinaryDataWriter lBinaryWriter;
            lBinaryWriter.Init();
            
            DSSWriteBlockStream<DSSBinaryDataWriter> lWStream(lBinaryWriter);
            
            int lnReplaceds = lpReplaceds.size();
            lWStream.WriteInt(lnReplaceds);
            for(int i = 0; i < lnReplaceds; i++)
            {
                hr = lpReplaceds[i]->Serialize(&lWStream);
                CHECK_HR;
            }
            unsigned char* lpBinary = NULL;
            int lBinarySize = 0;
            
            hr = lBinaryWriter.getBinary(&lpBinary, &lBinarySize);
            CHECK_HR;
                        
            ovDelta.push_back(lpBinary);
            ovDeltaSize.push_back(lBinarySize);
        }
        else
        {
            //fali, 891375. If CSI is NULL, that means CSI user defined is invalid, e.g. there's any object missing from target dataset.It pops out error.
            //hasun- we cannot just return S_FALSE, need to distinguish the case that offline CSI has error and offline CSI is not defined.
            //return S_FALSE;
            return S_OK;
        }
    }
    return hr;
}

//no longer used
int DSSDataModelTransaction::generateTransactionDelta(vector<DSS_ID>& ovDataSetID, vector<vector<string> >& ovvDelta)
{
    int hr = 0;
/*
    ovDataSetID.clear();
    ovvDelta.clear();
    if(mpTransactionReport)
    {
         DFCCSITransactionBase* lpCSI = mpTransactionReport->getTransactionCSI();
         if(lpCSI)
         {
             DSS_ID lTargetID = lpCSI->GetDatasetDSSID();
             
             int lnRecords = GetTransactionCount();
             int lnColumns = GetColumnCount();
             vector<vector<pair<DFCString, EnumDSSDataType> > > lvvRecords;
             for(int i = 0; i < lnRecords; i++)
             {
                 vector<pair<DFCString, EnumDSSDataType> > lvRecord;
                 for(int j = 0; j < lnColumns; j++)
                 {
					 std::string lText = "";
                     EnumDSSDataType lType = DssDataTypeReserved;
                     hr = GetValue(i, j, &lText, &lType);
                     CHECK_HR;
                     
                     //generate the pair, convert string to DFCString
#ifdef NO_WCHAR_SUPPORT
                    // tehe: make_pair will fail on android, because definition of MBase::String
                   // lvRecord.push_back(make_pair(lText, lType));
#else
                     DFCString lwText(lText.begin(), lText.end());
                     lvRecord.push_back(make_pair(lwText, lType));
#endif
                 }
                 lvvRecords.push_back(lvRecord);
             }
             vector<DFCCSITransactionBase*> lpReplaceds = lpCSI->ReplacePlaceholder(lvvRecords);
             
             ovDataSetID.push_back(lTargetID);
             ovvDelta.resize(ovDataSetID.size());
             for(int i = 0; i < lpReplaceds.size(); i++)
             {
                 MBase::ReturnWString lwString = lpReplaceds[i]->GetText();
#ifdef NO_WCHAR_SUPPORT     
                 ovvDelta[ovvDelta.size() -1].push_back(lwString.c_str());
#else
                 string lString(lwString.c_str(), lwString.c_str() + lwString.GetLength());
                 ovvDelta[ovvDelta.size() -1].push_back(lString);
#endif
             }
         }
    }
 */
    return hr;
}
int DSSDataModelTransaction::getOutputXML(MBase::CString &orXMLContent)
{
	if (hIsEmpty())// && mDeletedRecords.empty() && mInsertedRecords.empty())
		return S_FALSE;	//459606

    if(mpTransactionReport)
    {
        /*
        int hr = 0;
        DFCCSITransactionBase* lpCSI = mpTransactionReport->getTransactionCSI();
        if(lpCSI)
        {
            int lnRecords = GetTransactionCount();
            int lnColumns = GetColumnCount();
            for(int i = 0; i < lnRecords; i++)
            {
                vector<pair<DFCString*, EnumDSSDataType> > lvValue;
                for(int j = 0; j < lnColumns; j++)
                {
                    string lText = "";
                    EnumDSSDataType lType = DssDataTypeReserved;
                    hr = GetValue(i, j, &lText, &lType);
                    CHECK_HR;
                    
                    //generate the pair, convert string to DFCString
#ifdef NO_WCHAR_SUPPORT
                    lvValue.push_back(pair<DFCString*, EnumDSSDataType>(&lText, lType));
#else
                    DFCString lwText(lText.begin(), lText.end());
                    lvValue.push_back(pair<DFCString*, EnumDSSDataType>(&lwText, lType));
#endif
                }
                lpCSI->ReplacePlaceholder(lvValue);
            }
        }
         */
    }
	std::ostringstream lStrStream;
	int i = 0;
	int lnRecords = mvpRecord.size(); 
	for (i = 0; i < lnRecords; i++) 
	{
		if (mvpRecord[i] && mvpRecord[i]->mFlag < DssTransactionDataOriginal)
			break;
	}
	if (i >= lnRecords)
		return S_FALSE;		//there's no marked rows
	
	hCheckMetricColumnData();	//468017, pick up derived metric changes due to re-calculation
	
	//1. write root node
	lStrStream << OPEN_TAG(Tag_TRS_Root);					//<transaction_data node_key = "K135" >
	MBase::CString lNodeKey;
	if (mpSourceDataModel)
		lNodeKey = mpSourceDataModel->getNodeKey();
    else
    {
        lNodeKey = mNodeKey;
    }
		
	lStrStream << Attr_TRS_NodeKey << ATT_VAL(lNodeKey) << " >";
	
	//2. write header info
	lStrStream << START_TAG(Tag_TRS_HeaderInfo);			//<header_info>
	for (i = 0; i < mnColumns; i++)
	{
        WriteHeaderInfo(i, mHeaderInfo[i].mpDefinition, mHeaderInfo[i].mDataType, lStrStream);
	}
	lStrStream << END_TAG(Tag_TRS_HeaderInfo);				//</header_info>
	
	//3. write transaction record data
	lStrStream<< START_TAG(Tag_TRS_Records);				//<records>
	lnRecords = mvpRecord.size();
	
	vector<int> lvSortedOrder(lnRecords);
	for (i = 0; i < lnRecords; i++)
	{
		lvSortedOrder[i] = i;
	}
	
	if (lnRecords > 0)
	{
		_xtab_sort(&lvSortedOrder[0], &lvSortedOrder[0] + lnRecords, *this);
	}
	
	for (i = 0; i < lnRecords; i++)
	{
		TransactionRecord* lpRecord = mvpRecord[lvSortedOrder[i]];
		if (lpRecord)
		{
			WriteRecord(lpRecord, mHeaderInfo, lStrStream);
		}
	}
	lStrStream << END_TAG(Tag_TRS_Records);					//</records>
	
	lStrStream << END_TAG(Tag_TRS_Root);					//</transaction_data>
	orXMLContent = lStrStream.str();
	return S_OK;
}

int DSSDataModelTransaction::size()
{
	return 0;
}


int DSSDataModelTransaction::UndoChange(int iOriginalRow, int iOriginalColumn, int iColumnIndex)
{
	hAdjustOrdinal(iOriginalRow, iOriginalColumn);
	
	//first check updated records list
	RecordMap::iterator it = mUpdatedRecords.find(RecordKey(iOriginalRow, iOriginalColumn));
	if (it != mUpdatedRecords.end() && iColumnIndex >= 0)
	{		
		TransactionRecord *lpRecord = it->second;
		if (lpRecord->mvpText[iColumnIndex])
		{
			delete lpRecord->mvpText[iColumnIndex];
			lpRecord->mvpText[iColumnIndex] = NULL;
		}
		
		int i = 0;
		for (i = 0; i < mnColumns; i++)
		{
			if (lpRecord->mvpText[i])
				break;
		}
	}
	
	hUndoMarkedRows(iOriginalRow);
	
	return S_OK;
}

int DSSDataModelTransaction::setAllCommit()
{
    if (mpSourceDataModel && mpSourceDataModel->getModelType() == DssModelFieldGroup)
	{
		hUpdateFieldRawData();
	}
	
	//set record flag to submitted
	for (vector<TransactionRecord*>::iterator it = mvpRecord.begin(); it < mvpRecord.end(); it++)
	{
		if ((*it) && (*it)->mFlag < DssTransactionDataOriginal)
		{
			(*it)->mFlag = DssTransactionDataCommit;		
		}
	}

	bool lbKeepSubmitted = true;	//to support increnmental data loading, we need preserve the commited records
	hCleanUp(lbKeepSubmitted, true);		//468726, clear everything
	Discard(false); //discard unmarked changes
	if (mpSourceDataModel && mpSourceDataModel->getModelType() == DssModelXTab)
	{
        static_cast<DSSDataModelXTabImpl*> (mpSourceDataModel)->ReCalculate();
		//472476
		static_cast<DSSDataModelXTabImpl*> (mpSourceDataModel)->getCube()->MakeOriginalDataSliceCopy();
	}
	
	return S_OK;
}

EnumDSSTransactionDataChangeType DSSDataModelTransaction::getRecordeType(int iOriginalRow, int iOriginalColumn)
{
	hAdjustOrdinal(iOriginalRow, iOriginalColumn);
	
	EnumDSSTransactionDataChangeType lType = DssTransactionDataReserved;
	RecordMap::iterator it = mUpdatedRecords.find(RecordKey(iOriginalRow, iOriginalColumn));
	if (it != mUpdatedRecords.end())
	{
		TransactionRecord *lpRecord = it->second;
		lType = lpRecord->mFlag;
	}

	if (lType == DssTransactionDataReserved)
		lType = DssTransactionDataOriginal;
	
	return lType;
}

int DSSDataModelTransaction::LoadFromXML(MBase::CString &irXML)
{
	return S_OK;
}

int DSSDataModelTransaction::LoadFromBinary(DSSBinaryDataReader *ipReader)
{
	return S_OK;
}

int DSSDataModelTransaction::UpdateDataCell(int iOriginalRow, int iOriginalColumn, int iColumnIndex, MBase::CString *ipNewValue,bool ibDisplay)
{
	AE_ASSERT (mpSourceDataModel->getModelType() == DssModelXTab);
	
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
		return static_cast<DSSDataModelXTabImpl*> (mpSourceDataModel)->getXTabView()->setCellValue(iOriginalRow, iOriginalColumn, lpData, ipNewValue->size(), DssDataOk);
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
            return static_cast<DSSDataModelXTabImpl*>(mpSourceDataModel)->getXTabView()->setCellValue(iOriginalRow, iOriginalColumn, lpRecord->mvDisplayData[iColumnIndex].mValue,lpRecord->mvDisplayData[iColumnIndex].mSizeInByte, lpRecord->mvDisplayData[iColumnIndex].mFlag,lbIsRawData);
                                                                                                     
        }
        else
        {
            return static_cast<DSSDataModelXTabImpl*> (mpSourceDataModel)->getXTabView()->setCellValue(iOriginalRow, iOriginalColumn,
                                              lpRecord->mvOriginalData[iColumnIndex].mValue,
                                              lpRecord->mvOriginalData[iColumnIndex].mSizeInByte,
                                              lpRecord->mvOriginalData[iColumnIndex].mFlag,
                                              lbIsRawData);
        }
	}
}

int DSSDataModelTransaction::UpdateHeaderCell(int iOriginalRow, int iOriginalColumn, int iColumnIndex, DSSTabularUnit *ipXTabunit,
											  int iBaseForm, int iOffset, MBase::CString *ipNewValue, bool ibDisplay)
{
    if (iColumnIndex < 0 || iColumnIndex >= mnColumns)
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
	
	//473061, clear bucket sort cache
	static_cast<DSSDataModelXTabImpl*> (mpSourceDataModel)->deleteSorts();
	
	if (ipNewValue)
	{
		AERawData *lpOrignalData = NULL;
        AERawData *lpOrignalDisplayData = NULL;
		if (!mHeaderInfo[iColumnIndex].mpTabularUnit)
		{
			mHeaderInfo[iColumnIndex].mpTabularUnit = ipXTabunit;
			mHeaderInfo[iColumnIndex].mBaseForm = iBaseForm;
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

int DSSDataModelTransaction::UpdateField(int iOriginalRow, int iOriginalColumn, int iColumnIndex, MBase::CString *ipNewValue, bool ibDisplay)
{
	AE_ASSERT(mpSourceDataModel->getModelType() == DssModelFieldGroup);
	
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
#ifdef __ANDROID__
        //676880, for ANdroid, we need to update the field raw data to support recalculate
        static_cast<DSSDataModelFieldGroupImpl*> (mpSourceDataModel)->setRawData(iOriginalRow, iOriginalColumn, *ipNewValue,mHeaderInfo[iColumnIndex].mDataType);
#endif
		return static_cast<DSSDataModelFieldGroupImpl*> (mpSourceDataModel)->setFieldText(iOriginalRow, iOriginalColumn, *ipNewValue);
	}
	else
	{
		RecordMap::iterator it = mUpdatedRecords.find(RecordKey(iOriginalRow, 0));
		if (it != mUpdatedRecords.end())
		{
			TransactionRecord *lpRecord = it->second;
			MBase::CString *lpOriginalText = &lpRecord->mvOriginalFieldText[iColumnIndex];
#ifdef __ANDROID__
            //712988, for ANdroid, we need to update the field raw data to support recalculate
            int lnFields = static_cast<DSSDataModelFieldGroupImpl*> (mpSourceDataModel)->CountFields();
            static_cast<DSSDataModelFieldGroupImpl*> (mpSourceDataModel)->AddRawData(iOriginalRow * lnFields + iOriginalColumn, lpRecord->mvOriginalData[iColumnIndex]);
#endif
			return static_cast<DSSDataModelFieldGroupImpl*> (mpSourceDataModel)->setFieldText(iOriginalRow, iOriginalColumn, *lpOriginalText);
		}
	}
	return S_OK;
}

void DSSDataModelTransaction::hCleanUpRecords(vector<TransactionRecord*> &irvRecords, bool ibKeepSubmitedRecord, bool ibKeepUnMarkedRecord)
{
	for (vector<TransactionRecord*>::iterator it = irvRecords.begin(); it < irvRecords.end(); it++)
	{
		if (ibKeepSubmitedRecord && (*it) && (*it)->mFlag == DssTransactionDataCommit)
		{
			mvpRecordSubmitted.push_back(*it);
			(*it) = NULL;
		}
		
		if ((*it) && !(ibKeepUnMarkedRecord && (*it)->mFlag == DssTransactionDataOriginal))
		{
			for (vector<MBase::CString*>::iterator it2 = (*it)->mvpText.begin(); it2 < (*it)->mvpText.end(); it2++)
			{
				if (*it2)
				{
					delete (*it2);
					(*it2) = NULL;
				}
			}
            for (vector<MBase::CString*>::iterator it2 = (*it)->mvpDisplayText.begin(); it2 < (*it)->mvpDisplayText.end(); it2++)
			{
				if (*it2)
				{
					delete (*it2);
					(*it2) = NULL;
				}
			}

			delete *it;
			*it = NULL;
		}
	}
}

void DSSDataModelTransaction::hCleanUp(bool ibKeepSubmitedRecord, bool ibKeepUnMarkedRecord)
{  
    if(ibKeepSubmitedRecord)
    {
		for(RecordMap::iterator it = mUpdatedRecords.begin(); it != mUpdatedRecords.end(); it++)
		{
			TransactionRecord *lpRecord = it->second;
			//we keep the committed records
			if(lpRecord->mFlag == DssTransactionDataCommit)
			{
				mSubmittedRecords.push_back(std::pair<RecordKey, TransactionRecord*> ( it->first, it->second));
			}
		}
    }
	if(ibKeepUnMarkedRecord)
	{
		for(RecordMap::iterator it = mUpdatedRecords.begin(); it != mUpdatedRecords.end();)// it++)
		{
			TransactionRecord *lpRecord = it->second;
			//we keep the unmarked records
			if(lpRecord->mFlag != DssTransactionDataOriginal)
			{
				mUpdatedRecords.erase(it++);
			}
            else
            {
                ++it;
            }
		}
		
		hCleanUpRecords(mvpRecord, ibKeepSubmitedRecord, ibKeepUnMarkedRecord);
		vector<TransactionRecord*> lvRecords;
		for(int i = 0; i < mvpRecord.size(); i++)
		{
			if(mvpRecord[i] && mvpRecord[i]->mFlag == DssTransactionDataOriginal)
				lvRecords.push_back(mvpRecord[i]);
		}
		mvpRecord.clear();
		mvpRecord.assign(lvRecords.begin(), lvRecords.end());
				
		if (!ibKeepSubmitedRecord)
		{
			hCleanUpRecords(mvpRecordSubmitted, ibKeepSubmitedRecord, ibKeepUnMarkedRecord);
			mvpRecordSubmitted.clear();
            
            mSubmittedRecords.clear();
		}
		
		mSequenceNumber = lvRecords.size();
	}
	else
	{
        mUpdatedRecords.clear();

        
		hCleanUpRecords(mvpRecord, ibKeepSubmitedRecord);
		mvpRecord.clear();
		
		if (!ibKeepSubmitedRecord)
		{
			hCleanUpRecords(mvpRecordSubmitted, ibKeepSubmitedRecord);
			mvpRecordSubmitted.clear();
            
            mSubmittedRecords.clear();
		}
		
		mSequenceNumber = 0;
        
        mvValidatedRows.clear();
	}
    mvUpdateRecords.clear();
}

int DSSDataModelTransaction::Discard(bool ibRestore)
{
	if (mStatus != XTAB_INITIALIZED)
		return E_FAIL;		//472924
		
	int hr = S_OK;
	
	//undo changes
	if (mpSourceDataModel->getModelType() == DssModelXTab)
	{
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
		hCleanUp(true);		//475902, clean up the transaction records before manipulate calculation plan
		
		//537872
        //if(lvUpdatedRecords.size() > 0)
        //726932
        if(ibRestore)
        {
            //472476
            hr = static_cast<DSSDataModelXTabImpl*> (mpSourceDataModel)->getCube()->RestoreOriginalDataSlice();
            CHECK_HR;
		}
		
		DSSCalculationPlan *lpCalcPlan = static_cast<DSSDataModelXTabImpl*> (mpSourceDataModel)->getCalculationPlan();
		CHECK_PTR(lpCalcPlan);

        
		//data is already restored, no need to recalculate
		//TQMS 613533,689419
		lpCalcPlan->setStatus(DssCalcStatusNeedEvalThres);
		
		//TQMS 476909 & 610782, in some cases (for 476909, there are derived metric in the xtab), when user updated some cells and submit the changes and then discard, we still need to recalculate
		//lpCalcPlan->MarkUnCalculated(0);
		
	}//end of if (mpSourceDataModel->getModelType() == DssModelXTab)
	else if (mpSourceDataModel->getModelType() == DssModelFieldGroup)
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
		}
		hCleanUp(true);
	}

	return S_OK;
}

int DSSDataModelTransaction::hUpdateCells(TransactionRecord *lpRecord, int iRow, int iCol, bool IsUndo, int iTrxColumnIndex)
{
	int hr = S_OK;
	
	for (int iColumnIndex = 0; iColumnIndex < mnColumns + mnOfflineColumns; iColumnIndex++)
	{
		if (!IsUndo && !lpRecord->mvpText[iColumnIndex])	//it's not undo, and no data change
			continue;
			
        //if iTrxColumnIndex >=0, it means only update cell for iTrxColumnIndex
        if(iTrxColumnIndex >=0 && iTrxColumnIndex != iColumnIndex)
            continue;
        
		MBase::CString *lpText = IsUndo ? NULL : lpRecord->mvpText[iColumnIndex];
		if(!IsUndo && lpRecord->mvpDisplayText[iColumnIndex])
        {
            lpText = lpRecord->mvpDisplayText[iColumnIndex];
        }
		int lOriginalRow = iRow;
		int lOriginalColumn = iCol;
		DSSTabularUnit *lpUnit = NULL;
        if(iColumnIndex < mnColumns)
        {
            lpUnit = mHeaderInfo[iColumnIndex].mpTabularUnit;
        }
        else
        {
            lpUnit = mvOfflineColumns[iColumnIndex - mnColumns].mpTabularUnit;
        }
		if (lpUnit)
		{
			//undo attribute element change
			int lBaseForm = -1;
            if(iColumnIndex < mnColumns)
            {
                lBaseForm = mHeaderInfo[iColumnIndex].mBaseForm;
            }else
            {
                lBaseForm = mvOfflineColumns[iColumnIndex - mnColumns].mBaseForm;
            }
            
			int lOffset = lpRecord->mvDataOffset[iColumnIndex];
			
			//466982 & 467220, only undo changes for updated cells
			if(lOffset >= 0)
			{
				hr = UpdateHeaderCell(lOriginalRow, lOriginalColumn, iColumnIndex, lpUnit, lBaseForm, lOffset, lpText);
				CHECK_HR;
			}
		}
		else {
			//undo metric cell change
			if (mbIsMetricOnColumn)
				lOriginalColumn = lpRecord->mvDataOffset[iColumnIndex];
			else
				lOriginalRow = lpRecord->mvDataOffset[iColumnIndex];
			
			if (lOriginalColumn >= 0 && lOriginalRow >= 0)
			{
				MBase::CString *lpText = IsUndo ? NULL : lpRecord->mvpText[iColumnIndex];
				
				hr = UpdateDataCell(lOriginalRow, lOriginalColumn, iColumnIndex, lpText);
				CHECK_HR;
			}
		}
	}//end of for (int iColumnIndex = 0; iColumnIndex < mnColumns; iColumnIndex++)
	return S_OK;
}

int DSSDataModelTransaction::MarkRecord(int iOriginalRow, vector<AERawData> *ipvOriginalValue, vector<AERawData> *ipvDisplayData, EnumDSSTransactionDataChangeType iType)
{
	int ldummy = 0;
	hAdjustOrdinal(iOriginalRow, ldummy);
	
	if (iType == DssTransactionDataInsert)
		return hAddRecordMarked(iOriginalRow, ipvOriginalValue,ipvDisplayData, DssTransactionDataUpdate, mUpdatedRecords);		
	else if (iType == DssTransactionDataOriginal)
		return hUndoMarkedRows(iOriginalRow);
	else
		return E_INVALIDARG;
}

int DSSDataModelTransaction::hAddRecordMarked(int iAdjustedRow, vector<AERawData> *ipvOriginalValue, vector<AERawData> *ipvDisplayValue,EnumDSSTransactionDataChangeType iType, RecordMap &irRecordMap)
{
	RecordMap::iterator it = irRecordMap.find(RecordKey(iAdjustedRow, 0));
	TransactionRecord *lpRecord = NULL;
	if (it == irRecordMap.end())
	{
		lpRecord = hCreateNewRecord(iType, ipvOriginalValue,ipvDisplayValue);
		CHECK_PTR(lpRecord);
		
		irRecordMap[RecordKey(iAdjustedRow, 0)] = lpRecord;
		lpRecord->mRowOrdinal = iAdjustedRow;
		lpRecord->mColOrdinal = 0;
	}
	else
	{
		lpRecord = it->second;
		lpRecord->mFlag = iType;
	}
	lpRecord->mbMarked = 1;
    
    //592678
#ifdef __ANDROID__
    if(mbMarkRowEnabled == false)
    {
        for(it = irRecordMap.begin(); it != irRecordMap.end(); it++)
        {
            lpRecord = it->second;
            if(lpRecord->mbMarked == 0)
            {
                lpRecord->mFlag = DssTransactionDataOriginal;
            }
        }
    }
    mbMarkRowEnabled = true;
#endif
	return S_OK;
}

bool DSSDataModelTransaction::IsDataUpdated(int iOriginalRow, int iOriginalColumn, int iColumnIndex,bool* opbSubmit,bool ibValidateEmpty)
{
    if(iColumnIndex < 0)
        return false;
	
    hAdjustOrdinal(iOriginalRow, iOriginalColumn);
    
	RecordMap::iterator it = mUpdatedRecords.find(RecordKey(iOriginalRow, iOriginalColumn));
	if (it != mUpdatedRecords.end())
	{		
		TransactionRecord *lpRecord = it->second;
        if(!ibValidateEmpty)
        {
            if (lpRecord->mvpText[iColumnIndex])
            {
                if(opbSubmit)
                {
                    EnumDSSDataType lColumnDataType = DssDataTypeReserved;
                    if(iColumnIndex < mHeaderInfo.size())
                    {
                        lColumnDataType = mHeaderInfo[iColumnIndex].mDataType;
                    }
                    else if(iColumnIndex - mHeaderInfo.size() < mvOfflineColumns.size())
                    {
                        lColumnDataType = mvOfflineColumns[iColumnIndex - mHeaderInfo.size()].mDataType;
                    }
                    std::string lText = "";
                    ConvertDataTypeAndToString(lpRecord->mvOriginalData[iColumnIndex], lColumnDataType, lText);
                    std::string mText = *(lpRecord->mvpText[iColumnIndex]);
                    if(mText == "" || mText == lText)
                        (*opbSubmit) = false;
                    //722489
                    else if(lColumnDataType == DssDataTypeDate)
                    {
                        if(lText != "" && strstr(mText.c_str(), lText.c_str()))
                        {
                            (*opbSubmit) = false;
                        }
                    }
                    else if(lColumnDataType == DssDataTypeTimeStamp ||lColumnDataType == DssDataTypeTime)
                    {
                        ConvertDateTime(lText);
                        ConvertDateTime(mText);
                        if(lText != "" && strstr(lText.c_str(), mText.c_str()))
                        {
                            (*opbSubmit) = false;
                        }
                    }
                    else
                        (*opbSubmit) = true;
                }
                return true;
            }
        }else{
            if(lpRecord->mvpText[iColumnIndex])
            {
                std::string mText = *(lpRecord->mvpText[iColumnIndex]);
                if(mText == "")
                {
                    if(opbSubmit)
                        (*opbSubmit) = false;
                    return false;
                }else
                {
                    if(opbSubmit)
                        (*opbSubmit) = true;
                    return true;
                }
            }else{
                std::string lText = "";
                if(lpRecord->mvOriginalData[iColumnIndex].mFlag == DssDataNull && DSSAEHelper::IsDate(mHeaderInfo[iColumnIndex].mDataType))
                {
                    lText = "";
                }else{
                    ConvertDataTypeAndToString(lpRecord->mvOriginalData[iColumnIndex], mHeaderInfo[iColumnIndex].mDataType, lText);
                }
                if(lText == "")
                {
                    if(opbSubmit)
                        (*opbSubmit) = false;
                    return false;
                }else
                {
                    if(opbSubmit)
                        (*opbSubmit) = true;
                    return true;
                }
            }
        }
	}
	return false;
}

bool DSSDataModelTransaction::IsDataUpdatedOrSubmitted(int iOriginalRow, int iOriginalColumn, int iColumnIndex,bool* opbSubmit)
{
    if(IsDataUpdated(iOriginalRow, iOriginalColumn, iColumnIndex, opbSubmit))
        return true;
    
	hAdjustOrdinal(iOriginalRow, iOriginalColumn);
    
	vector< std::pair< RecordKey, TransactionRecord*> >::iterator it = mSubmittedRecords.begin();
	while(it != mSubmittedRecords.end())
	{
		TransactionRecord *lpRecord = it->second;
		if (lpRecord->mvpText[iColumnIndex])
		{
            if(opbSubmit)
            {
                EnumDSSDataType lColumnDataType = DssDataTypeReserved;
                if(iColumnIndex < mHeaderInfo.size())
                {
                    lColumnDataType = mHeaderInfo[iColumnIndex].mDataType;
                }
                else if(iColumnIndex - mHeaderInfo.size() < mvOfflineColumns.size())
                {
                    lColumnDataType = mvOfflineColumns[iColumnIndex - mHeaderInfo.size()].mDataType;
                }

                std::string lText = "";
                ConvertDataTypeAndToString(lpRecord->mvOriginalData[iColumnIndex], mHeaderInfo[iColumnIndex].mDataType, lText);
                std::string mText = *(lpRecord->mvpText[iColumnIndex]);
                if(mText == "" || mText == lText)
                    (*opbSubmit) = false;
                //722489
                else if(lColumnDataType == DssDataTypeDate)
                {
                    if(lText != "" && strstr(mText.c_str(), lText.c_str()))
                    {
                        (*opbSubmit) = false;
                    }
                }
                else if(lColumnDataType == DssDataTypeTimeStamp ||lColumnDataType == DssDataTypeTime)
                {
                    ConvertDateTime(lText);
                    ConvertDateTime(mText);
                    if(lText != "" && strstr(lText.c_str(), mText.c_str()))
                    {
                        (*opbSubmit) = false;
                    }
                }
                else
                    (*opbSubmit) = true;
            }
			return true;
		}
        it++;
	}
	return false;
}

// jimzhang, Nov/7/2012, port from RWD_MCE_iOS branch to support recalculate of threshold in transaction
int DSSDataModelTransaction::getUpdatedValue(int iOriginalRow, int iOriginalColumn, int iColumnIndex, MBase::CString &orText)
{
	hAdjustOrdinal(iOriginalRow, iOriginalColumn);
	
	RecordMap::iterator it = mUpdatedRecords.find(RecordKey(iOriginalRow, iOriginalColumn));
	if (it != mUpdatedRecords.end())
	{		
		TransactionRecord *lpRecord = it->second;
		if (lpRecord->mvpText[iColumnIndex])
		{
			orText = *(lpRecord->mvpText[iColumnIndex]);
		}
	}
	return S_OK;
}

MBase::CString*  DSSDataModelTransaction::getUpdatedAndSubmittedData(int iOriginalRow, int iOriginalColumn, int iColumnIndex, bool ibDisplay, bool ibSkipUpdateValue, bool* opDisplay)
{
    if (mUpdatedRecords.empty() && mSubmittedRecords.empty())
        return NULL;
    
	hAdjustOrdinal(iOriginalRow, iOriginalColumn);

    if (!mUpdatedRecords.empty() && !ibSkipUpdateValue)
    {
        RecordMap::iterator it = mUpdatedRecords.find(RecordKey(iOriginalRow, iOriginalColumn));
        if (it != mUpdatedRecords.end())
        {		
            TransactionRecord *lpRecord = it->second;
            if (lpRecord->mvpText[iColumnIndex])
            {
                if (lpRecord->mvpDisplayText[iColumnIndex] && ibDisplay)
                {
                    if(opDisplay)
                    {
                        *opDisplay = true;
                    }
                    return lpRecord->mvpDisplayText[iColumnIndex];
                }
                else
                    return lpRecord->mvpText[iColumnIndex];
            }
        }
    }
    
    if (!mSubmittedRecords.empty())
    {
        // jimzhang, Dec/17/2012, TQMS 684909, one editable field it can be updated and submitted more than one time, in this case, there are multiple records with same key in "mSubmittedRecords". To return the text updated latest, we should tranverse "mSubmittedRecords" reversely. [todo] It may be an issue to save multiple records with same key in "mSubmittedRecords", related code is in DSSDataModelTransaction::hCleanUp().
        vector< std::pair< RecordKey, TransactionRecord*> >::reverse_iterator rIter = mSubmittedRecords.rbegin();
        for (; rIter != mSubmittedRecords.rend(); rIter++)
        {
            RecordKey &lrKey = rIter->first;
            if (lrKey.first == iOriginalRow && lrKey.second == iOriginalColumn)
            {
                TransactionRecord* lpRecord = rIter->second;
                MBase::CString* lpText = NULL;
                if (lpRecord->mvpDisplayText[iColumnIndex] && ibDisplay)
                {
                    if(opDisplay)
                    {
                        *opDisplay = true;
                    }
                    lpText = lpRecord->mvpDisplayText[iColumnIndex];
                }
                else
                    lpText = lpRecord->mvpText[iColumnIndex];
                //752004, multiple submitted records can be made on one row
                if(lpText)
                    return lpText;
            }
        }        
        
        //TODO: linear search needs improved, but mSubmittedRecords is declared as vector
        vector< std::pair< RecordKey, TransactionRecord*> >::iterator  it = mSubmittedRecords.begin();
        for (; it != mSubmittedRecords.end(); it++)
        {
            RecordKey &lrKey = it->first;
            if (lrKey.first == iOriginalRow && lrKey.second == iOriginalColumn)
            {
                TransactionRecord* lpRecord = it->second;
                if (lpRecord->mvpDisplayText[iColumnIndex] && ibDisplay)
                {
                    if(opDisplay)
                    {
                        *opDisplay = true;
                    }
                    return lpRecord->mvpDisplayText[iColumnIndex];
                }
                else
                    return lpRecord->mvpText[iColumnIndex];
            }
        }
    }
	return NULL;
}

int DSSDataModelTransaction::hUndoMarkedRows(int iAdjustedRow)
{
	RecordMap::iterator it = mUpdatedRecords.find(RecordKey(iAdjustedRow, 0));
	if (it != mUpdatedRecords.end())
	{
		TransactionRecord *lpRecord = it->second;
		lpRecord->mFlag = DssTransactionDataOriginal;
        lpRecord->mbMarked = 0;
	}
	return S_OK;
}

TransactionRecord *DSSDataModelTransaction::hCreateNewRecord(EnumDSSTransactionDataChangeType iRecordType,
															 vector<AERawData> *ipvOriginalValue, vector<AERawData> * ipvDisplayValue)
{
	if (!ipvOriginalValue || !ipvDisplayValue)		//we need the original value to create a new record
		return NULL;
	
	int lnColumns = ipvOriginalValue->size();
	_ASSERT(lnColumns == mnColumns + mnOfflineColumns);
	
	TransactionRecord *lpRecord = new TransactionRecord();
	lpRecord->mvpText.resize(lnColumns);
	lpRecord->mvOriginalData.resize(lnColumns);
    lpRecord->mvDisplayData.resize(lnColumns);
	lpRecord->mFlag = iRecordType;
	lpRecord->mvDataOffset.resize(lnColumns);
	lpRecord->mOrder = mSequenceNumber++;
	lpRecord->mvOriginalFieldText.resize(lnColumns);
	lpRecord->mRowOrdinal = -1;
	lpRecord->mColOrdinal = -1;
	lpRecord->mvpDisplayText.resize(lnColumns);
    lpRecord->mbMarked = 0;
    lpRecord->mbValidate = 1;
	//add it into record list
	mvpRecord.push_back(lpRecord);
	
	for (int i = 0; i < lnColumns; i++)
	{
		lpRecord->mvOriginalData[i] = (*ipvOriginalValue)[i];	//476901, when merge transaction data model, the original data model lost the memory ownership
		lpRecord->mvpText[i] = NULL; 
		lpRecord->mvDataOffset[i] = -1;
		//lpRecord->mvOriginalFieldText;
        lpRecord->mvpDisplayText[i] = NULL;
        
        lpRecord->mvDisplayData[i] = (*ipvDisplayValue)[i];
        
        
        //DE6170
        if(lpRecord->mvOriginalData[i].mDataType == DssDataTypeInt64 && mHeaderInfo[i].mDataType == DssDataTypeInteger)
        {
            mHeaderInfo[i].mDataType = DssDataTypeInt64;
        }
    }
	return lpRecord;
}

void DSSDataModelTransaction::setMapTransactionColumnToMetric(vector<int> &irvUnitMap, vector<int> &irvBaseFormMap, vector<int> &irvWBForm)
{
    _ASSERT(irvUnitMap.size() == mnColumns);

	mapTrxColumn2XTabUnit.assign(irvUnitMap.begin(), irvUnitMap.end());
	mapTrxColumn2Metric.assign(irvBaseFormMap.begin(), irvBaseFormMap.end());
    mapTrxColumn2WriteBackForm.assign(irvWBForm.begin(),irvWBForm.end());
	/*
	for (int i = 0; i < irvUnitMap.size(); i++)
	{
		if (mapTrxColumn2XTabUnit[i] >= 0)
		{
			//it's an attribute, no need to remember it
			mapTrxColumn2XTabUnit[i] = -1;
			mapTrxColumn2Metric[i] = -1;
		}
	}
	 */
}

int DSSDataModelTransaction::hCheckMetricColumnData()
{
	if (mpSourceDataModel && mpSourceDataModel->getModelType() == DssModelXTab)
    {
		DSSXTabView *lpXTabView = static_cast<DSSDataModelXTabImpl*> (mpSourceDataModel)->getXTabView();
        return hCheckMetricColumnData(lpXTabView);
    }
    return S_OK;
}

int DSSDataModelTransaction::hCheckMetricColumnData(DSSXTabView *lpXTabView)
{
	int hr = S_OK;
	{
		for (RecordMap::iterator it = mUpdatedRecords.begin(); it != mUpdatedRecords.end(); it++)
		{
			int lOriginalRow = it->first.first;
			int lOriginalColumn = it->first.second;
			if (mbIsMetricOnColumn)
				lOriginalColumn = lOriginalColumn * mColOrdinalAdjustor;
			else
				lOriginalRow = lOriginalRow * mRowOrdinalAdjustor;
			
			TransactionRecord *lpRecord = it->second;
			
			vector<AERawData> lvRecalculatedData;		//get metric data after re-calculation
            vector<AERawData> lvDisplayData;
			hr = this->GenerateRecordFromXTabView(lpXTabView, lOriginalRow, lOriginalColumn, lvRecalculatedData,lvDisplayData);
			CHECK_HR;
			
			for (int iColumnIndex = 0; iColumnIndex < mnColumns; iColumnIndex++)
			{
				//skip columns mapped to attribute columns
				if(mapTrxColumn2XTabUnit[iColumnIndex] >= 0)
					continue;
				
				int lMetricID = mapTrxColumn2Metric[iColumnIndex];
				if (lMetricID >= 0 && !lpRecord->mvpText[iColumnIndex])
				{
					//only update an unchanged metric column
					if (iColumnIndex < lvRecalculatedData.size())
					{
						lpRecord->mvOriginalData[iColumnIndex].Assign(lvRecalculatedData[iColumnIndex]);
					}
				}
			}//end of for (int iColumnIndex = 0; iColumnIndex < mnColumns; iColumnIndex++)
            for (int iColumnIndex = 0; iColumnIndex < mnOfflineColumns; iColumnIndex++)
			{
				//skip columns mapped to attribute columns
				if(mapOfflineTrxColumn2XTabUnit[iColumnIndex] >= 0)
					continue;
				
				int lMetricID = mapOfflineTrxColumn2Metric[iColumnIndex];
				if (lMetricID >= 0 && !lpRecord->mvpText[iColumnIndex + mnColumns])
				{
					//only update an unchanged metric column
					if (iColumnIndex < lvRecalculatedData.size())
					{
						lpRecord->mvOriginalData[iColumnIndex + mnColumns].Assign(lvRecalculatedData[iColumnIndex + mnColumns]);
					}
				}
			}//end of for (int iColumnIndex = 0; iColumnIndex < mnOfflineColumns; iColumnIndex++)
		}
	}
	return S_OK;
}

int DSSDataModelTransaction::MergeTo(DSSDataModelTransaction *pTarget)
{
	if (!pTarget)
		return E_INVALIDARG;
    
    pTarget->mpTransactionReport = mpTransactionReport;
    pTarget->mpCSI = mpCSI;
    for (Int32 k = 0; k<associatedNodes.size(); k++) {
        pTarget->associatedNodes.push_back(associatedNodes[k]);
    }
    bool lbIsTargetEmpty = false;
	if (pTarget->mnColumns == 0)
	{
        lbIsTargetEmpty = true;
		//the target model is empty, copy it over
		pTarget->mHeaderInfo.resize(mnColumns);
		for (int i = 0; i < mnColumns; i++)
		{
			pTarget->mHeaderInfo[i].mpDefinition = mHeaderInfo[i].mpDefinition;
			pTarget->mHeaderInfo[i].mDataType = mHeaderInfo[i].mDataType;
			pTarget->mHeaderInfo[i].mpTabularUnit = NULL;
			pTarget->mHeaderInfo[i].mBaseForm = 0;
            pTarget->mHeaderInfo[i].mbRequired = mHeaderInfo[i].mbRequired;
		}
		pTarget->mnColumns = mnColumns;
	}
	else if (pTarget->mnColumns == mnColumns)
	{
		for (int i = 0; i < mnColumns; i++)
		{
			if (pTarget->mHeaderInfo[i].mpDefinition != mHeaderInfo[i].mpDefinition)
				return E_FAIL;	//two models are inconsistent
		}
	}
	else
		return E_FAIL;		//two models are inconsistent
	
    if (pTarget->mnOfflineColumns == 0)
	{
		//the target model is empty, copy it over
		pTarget->mvOfflineColumns.resize(mnOfflineColumns);
		for (int i = 0; i < mnOfflineColumns; i++)
		{
			pTarget->mvOfflineColumns[i].mpDefinition = NULL;
			pTarget->mvOfflineColumns[i].mDataType = mvOfflineColumns[i].mDataType;
			pTarget->mvOfflineColumns[i].mpTabularUnit = NULL;
			pTarget->mvOfflineColumns[i].mBaseForm = 0;
            pTarget->mvOfflineColumns[i].mbRequired = mvOfflineColumns[i].mbRequired;
		}
		pTarget->mnOfflineColumns = mnOfflineColumns;
        pTarget->mapOfflineTrxColumn2TrxColumn.assign(mapOfflineTrxColumn2TrxColumn.begin(), mapOfflineTrxColumn2TrxColumn.end());
	}
	else if (pTarget->mnOfflineColumns == mnOfflineColumns)
	{
		//do nothing
	}
	else
		return E_FAIL;		//two models are inconsistent
    
	hCheckMetricColumnData();

	int lnRecords = mvpRecord.size();;
	for (int i = 0; i < lnRecords; i++)
	{
		if (mvpRecord[i])
		{
			TransactionRecord *lpRecord = pTarget->hCreateNewRecord(mvpRecord[i]->mFlag, &mvpRecord[i]->mvOriginalData, &mvpRecord[i]->mvDisplayData);
			CHECK_PTR(lpRecord);
			
			vector<MBase::CString*> &lrTextStrings = mvpRecord[i]->mvpText;
			int lnColumns = lrTextStrings.size();
			lpRecord->mvpText.resize(lnColumns);
			for (int j = 0; j < lnColumns; j++)
			{
				if (lrTextStrings[j])
				{
					lpRecord->mvpText[j] = new MBase::CString(*lrTextStrings[j]);
					
				}
			}
            
            vector<MBase::CString*> &lrDisplayTextStrings = mvpRecord[i]->mvpDisplayText;
			lnColumns = lrDisplayTextStrings.size();
			lpRecord->mvpDisplayText.resize(lnColumns);
			for (int j = 0; j < lnColumns; j++)
			{
				if (lrDisplayTextStrings[j])
				{
					lpRecord->mvpDisplayText[j] = new MBase::CString(*lrDisplayTextStrings[j]);
					
				}
			}
            
			lpRecord->mvOriginalFieldText.assign(mvpRecord[i]->mvOriginalFieldText.begin(), mvpRecord[i]->mvOriginalFieldText.end());
            lpRecord->mbMarked = mvpRecord[i]->mbMarked;
            lpRecord->mbValidate = mvpRecord[i]->mbValidate;
            if(lbIsTargetEmpty)
            {
                for(RecordMap::iterator it = mUpdatedRecords.begin(); it != mUpdatedRecords.end(); it++)
                {
                    if(it->second == mvpRecord[i])
                    {
                        pTarget->hAddUpdatedRecord(it->first, lpRecord);
                    }
                }
            }
		}
	}

	return S_OK;
}
    	
void DSSDataModelTransaction::hAddUpdatedRecord(RecordKey iKey, TransactionRecord* ipRecord)
{
    mUpdatedRecords[iKey] = ipRecord;
}

bool DSSDataModelTransaction::operator() (int a, int b)
{
	if (a < mvpRecord.size() && b < mvpRecord.size())
	{
		return mvpRecord[a]->mOrder < mvpRecord[b]->mOrder;
	}
	else
	{
		_ASSERT(false);
		return false;
	}
}

int DSSDataModelTransaction::hUpdateFieldRawData()
{
	int hr = S_OK;
	
	DSSDataModelFieldGroupImpl *lpFieldGroupModel = static_cast<DSSDataModelFieldGroupImpl*> (mpSourceDataModel);
	
	for (RecordMap::iterator it = mUpdatedRecords.begin(); it != mUpdatedRecords.end(); it++)
	{
		int lOriginalRow = it->first.first;
		TransactionRecord *lpRecord = it->second;
		if (lpRecord && lpRecord->mFlag >= DssTransactionDataUpdate && lpRecord->mFlag <= DssTransactionDataInsert)
		{
			for (int iColumnIndex = 0; iColumnIndex < mnColumns; iColumnIndex++)
			{
				int lField = lpRecord->mvDataOffset[iColumnIndex];
				if(lField >= 0 && lpRecord->mvpText[iColumnIndex])
				{
					hr = lpFieldGroupModel->setRawData(lOriginalRow, lField, *lpRecord->mvpText[iColumnIndex],mHeaderInfo[iColumnIndex].mDataType);
					CHECK_HR;
				}
			}
		}
	}
	return S_OK;
}

int DSSDataModelTransaction::Init(DSSDataModelTransaction *ipTransactionModel, DSSDataModelBase *ipBaseDataModel)
{
	if (!ipTransactionModel || !ipBaseDataModel)
		return E_INVALIDARG;
	
	int hr = ipBaseDataModel->EnsureModelData();
	CHECK_HR;
	
	hr = hIsDataModelMatch(ipTransactionModel->mpSourceDataModel, ipBaseDataModel);
	if (hr == S_FALSE)
	{
//		_ASSERT(false);		//unexpected
	}
//	CHECK_HR;
	
	hr = this->Init(ipTransactionModel->mpTransactionReport, ipBaseDataModel);
	CHECK_HR;
	
	hr = hPopulateHeaderInfo(ipTransactionModel);
	CHECK_HR;
	
	if (ipTransactionModel->mpSourceDataModel->getModelType() == DssModelXTab)
    {
        DSSDataModelXTabImpl *lpXTabModelSrc = static_cast<DSSDataModelXTabImpl*> (ipTransactionModel->mpSourceDataModel);
        DSSDataModelXTabImpl *lpXTabModel = static_cast<DSSDataModelXTabImpl*> (mpSourceDataModel);
        
        hr = lpXTabModel->MapSliceWindow(0);
        CHECK_HR;
        
        hr = lpXTabModel->MapSliceWindow(1);
        CHECK_HR;
        
		hr = lpXTabModel->LoadAllDataSlices();
		CHECK_HR;
        
        hr = lpXTabModel->UnMapSliceWindow(0);
        CHECK_HR;
        
        hr = lpXTabModel->UnMapSliceWindow(1);
        CHECK_HR;	
        
        hr = hPopulateTransactionRecordsForGrid(ipTransactionModel, lpXTabModelSrc, lpXTabModel);
    }
    else if(ipTransactionModel->mpSourceDataModel->getModelType() == DssModelFieldGroup)
    {
		DSSDataModelFieldGroupImpl *lpSourceFGModel = static_cast<DSSDataModelFieldGroupImpl*> (ipTransactionModel->mpSourceDataModel);
        
		DSSDataModelFieldGroupImpl *lpThisFGModel = static_cast<DSSDataModelFieldGroupImpl*> (mpSourceDataModel);
        
        hr = hPopulateTransactionRecordsForFG(ipTransactionModel, lpSourceFGModel, lpThisFGModel);
    }
	CHECK_HR;
	
    mbIsValidated = ipTransactionModel->mbIsValidated;
    mvValidatedRows.clear();
    mvValidatedRows.assign(ipTransactionModel->mvValidatedRows.begin(),ipTransactionModel->mvValidatedRows.end());
	return S_OK;
}

int DSSDataModelTransaction::hIsDataModelMatch(DSSDataModelBase *ipDataModel, DSSDataModelBase *ipBaseDataModel)
{
	if (ipDataModel->getModelType() != ipBaseDataModel->getModelType())
		return S_FALSE;
	
	int lnUnits = ipDataModel->Count();	
	if (ipBaseDataModel->Count() != lnUnits)
		return S_FALSE;
	
	int i = 0;
	for (i = 0; i < lnUnits; i++)
	{
		DSSTabularUnit *lpXTabUnit1 = ipDataModel->Item(i);
		DSSTabularUnit *lpXTabUnit2 = ipBaseDataModel->Item(i);
		if (lpXTabUnit1->getUnitType() != lpXTabUnit2->getUnitType()
			|| !MBase::IsEqualGUID(lpXTabUnit1->getGUID(), lpXTabUnit2->getGUID()))
			return S_FALSE;
	}
	return S_OK;
}

int DSSDataModelTransaction::hPopulateHeaderInfo(DSSDataModelTransaction *ipTransactionModel)
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
		mHeaderInfo[i].mbRequired = ipTransactionModel->mHeaderInfo[i].mbRequired;
	}
	
	mapTrxColumn2XTabUnit.assign(ipTransactionModel->mapTrxColumn2XTabUnit.begin(), ipTransactionModel->mapTrxColumn2XTabUnit.end());
	mapTrxColumn2Metric.assign(ipTransactionModel->mapTrxColumn2Metric.begin(), ipTransactionModel->mapTrxColumn2Metric.end());
    mapTrxColumn2WriteBackForm.assign(ipTransactionModel->mapTrxColumn2WriteBackForm.begin(), ipTransactionModel->mapTrxColumn2WriteBackForm.end());
	return S_OK;
}

int DSSDataModelTransaction::hPopulateTransactionRecordsForGrid(DSSDataModelTransaction *ipTransactionModel, IDSSTemplateModel *ipSourceModel, IDSSTemplateModel *ipThisModel)
{
	int hr = S_OK;
	mbMarkRowEnabled = ipTransactionModel->mbMarkRowEnabled;
	{
		DSSXTabView *lpXTabViewSrc = ipSourceModel->getXTabView();
		DSSXTabView *lpXTabView = ipThisModel->getXTabView();
		
		hr = ipThisModel->LoadAllDataSlices();
		CHECK_HR;
		
		int lnUnits = ipSourceModel->CountUnits();
		vector<int> lvKeys(lnUnits);

		bool lbNeedRecalculate = false;
		DSSCalculationPlan *lpCalcPlan = ipThisModel->getCalculationPlan();
		//1, copy the submitted records first
		if (!ipTransactionModel->mvpRecordSubmitted.empty())
		{
			lbNeedRecalculate = true;
			int lnRecords = ipTransactionModel->mvpRecordSubmitted.size();
			for (int i = 0; i < lnRecords; i++)
			{
				hr = hPopulateOneRecord(lpXTabViewSrc, lpXTabView, ipTransactionModel->mvpRecordSubmitted[i], lvKeys,true);
				CHECK_HR;
                
			}
			
			//483054, recalculate and save the result for discard
			lpCalcPlan->setStatus(DssCalcStatusUnCalculated);
			
			hr = ipThisModel->ReCalculate();
			if (hr != S_OK)
			{
				_ASSERT(false);	//not expected
			}
			
			ipThisModel->getCube()->MakeOriginalDataSliceCopy();
			
		}
		
		//2, copy un-submitted records
//		RecordMap::iterator it = ipTransactionModel->mUpdatedRecords.begin();
//		RecordMap::iterator end = ipTransactionModel->mUpdatedRecords.end();
        //fali, 938314, mvpRecord have the same order with user's submission order. It's more reasonable if we should return error message
        vector<TransactionRecord*> lvpRecord = ipTransactionModel->mvpRecord;
		for (int i=0; i<lvpRecord.size(); i++)
		{
			TransactionRecord *lpRecordSrc = lvpRecord[i];
			//481535, the unmarked row contains changed value, still need copy over
			if (lpRecordSrc->mFlag == DssTransactionDataReserved) //|| lpRecordSrc->mFlag == DssTransactionDataOriginal 
				continue;
			
			hr = hPopulateOneRecord(lpXTabViewSrc, lpXTabView, lpRecordSrc, lvKeys);
			CHECK_HR;
			
			if (!lbNeedRecalculate)
			{
				lbNeedRecalculate = true;
			}
			
		}//end of for (; it != end; it++)
		
		mSequenceNumber = ipTransactionModel->mSequenceNumber;

		if(lpCalcPlan && lbNeedRecalculate)		//481817; don't set calculation plan if nothing is changed
		{
            lpCalcPlan->MarkUnCalculated(0);
//			lpCalcPlan->setStatus(DssCalcStatusUnCalculated);
		}
			
		hr = ipThisModel->ReCalculate();
		CHECK_HR;
	}
    return S_OK;
}

int DSSDataModelTransaction::hPopulateTransactionRecordsForFG(DSSDataModelTransaction *ipTransactionModel, DSSDataModelFieldGroupImpl* ipSourceFGModel, DSSDataModelFieldGroupImpl* ipThisFGModel)
{
	int hr = S_OK;
//	if(ipTransactionModel->mpSourceDataModel->getModelType() == DssModelFieldGroup)
	{
//		DSSDataModelFieldGroupImpl *lpFGModel = static_cast<DSSDataModelFieldGroupImpl*> (ipTransactionModel->mpSourceDataModel);
		
		//TODO: inheritate field group's change?
		//1, copy the submitted records first
		if (!ipTransactionModel->mvpRecordSubmitted.empty())
		{
			int lnRecords = ipTransactionModel->mvpRecordSubmitted.size();
			for (int i = 0; i < lnRecords; i++)
			{
				hr = hPopulateOneRecordForFieldGroup(ipSourceFGModel, ipThisFGModel, ipTransactionModel->mvpRecordSubmitted[i], true, true);
				CHECK_HR;
			}
		}
		
		//2, copy un-submitted records
//		RecordMap::iterator it = ipTransactionModel->mUpdatedRecords.begin();
//		RecordMap::iterator end = ipTransactionModel->mUpdatedRecords.end();
        //fali, 938314, mvpRecord have the same order with user's submission order. It's more reasonable if we should return error message
        vector<TransactionRecord*> lvpRecord = ipTransactionModel->mvpRecord;
		for (int i=0; i<lvpRecord.size(); i++)
		{
			TransactionRecord *lpRecordSrc = lvpRecord[i];
			if (lpRecordSrc->mFlag == DssTransactionDataOriginal || lpRecordSrc->mFlag == DssTransactionDataReserved)
				continue;
			
			hr = hPopulateOneRecordForFieldGroup(ipSourceFGModel, ipThisFGModel, lpRecordSrc, false);
			CHECK_HR;
		}//end of for (; it != end; it++)
		
		mSequenceNumber = ipTransactionModel->mSequenceNumber;
		 
	}

	return S_OK;

}

int DSSDataModelTransaction::hPopulateOneRecordForFieldGroup(DSSDataModelFieldGroupImpl* ipSourceFGModel, DSSDataModelFieldGroupImpl* ipThisFGModel, TransactionRecord * ipRecordSrc, bool bUpdateRawData, bool ibSubmitted)
{
	int hr = S_OK;

	int lRowOrdinalSrc = ipRecordSrc->mRowOrdinal;
	int lColOrdinalSrc = ipRecordSrc->mColOrdinal;
	
	AE_ASSERT(lRowOrdinalSrc >= 0 && lColOrdinalSrc >= 0);
	
	//1, map the previous change to the current xtab data model by row/column ordinal
	int lNewRow = lRowOrdinalSrc;
	int lNewCol = lColOrdinalSrc;
	CHECK_HR;
	
//	DSSDataModelFieldGroupImpl *lpFieldGroupModel = static_cast<DSSDataModelFieldGroupImpl*> (mpSourceDataModel);
	
	hr = hMapRowForFieldGroup(ipSourceFGModel, ipThisFGModel, lNewRow);
	if(hr != S_OK){}//swallow error

	//3, create a new record
	TransactionRecord *lpRecord = hCreateNewRecord(ipRecordSrc->mFlag, &ipRecordSrc->mvOriginalData, &ipRecordSrc->mvDisplayData);
	CHECK_PTR(lpRecord);
	
	//4, copy other data members of the record
    lpRecord->mbMarked = ipRecordSrc->mbMarked;
    lpRecord->mbValidate = ipRecordSrc->mbValidate;
	lpRecord->mOrder = ipRecordSrc->mOrder;
	lpRecord->mvDataOffset.assign(ipRecordSrc->mvDataOffset.begin(), ipRecordSrc->mvDataOffset.end());
	lpRecord->mvOriginalFieldText.assign(ipRecordSrc->mvOriginalFieldText.begin(), ipRecordSrc->mvOriginalFieldText.end());
	vector<MBase::CString*> &lrTextStrings = ipRecordSrc->mvpText;
    vector<MBase::CString*> &lrDisplayTextStrings = ipRecordSrc->mvpDisplayText;
	int iColumnIndex = 0;
	for (iColumnIndex = 0; iColumnIndex < mnColumns; iColumnIndex++)
	{
		if (lrTextStrings[iColumnIndex])
		{
			lpRecord->mvpText[iColumnIndex] = new MBase::CString(*lrTextStrings[iColumnIndex]);
		}
        
        if (lrDisplayTextStrings[iColumnIndex])
		{
			lpRecord->mvpDisplayText[iColumnIndex] = new MBase::CString(*lrDisplayTextStrings[iColumnIndex]);
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
			
            if(lpRecord->mvpDisplayText[iColumnIndex])
            {
                hr = UpdateField(lpRecord->mRowOrdinal, lOffset, iColumnIndex, lpRecord->mvpDisplayText[iColumnIndex],true);
                CHECK_HR;
            }
            else
            {
                hr = UpdateField(lpRecord->mRowOrdinal, lOffset, iColumnIndex, lpRecord->mvpText[iColumnIndex]);
                CHECK_HR;
            }
			
			//483966, update raw data for submitted changes
			if (ipThisFGModel && bUpdateRawData && lOffset >= 0 && lpRecord->mvpText[iColumnIndex])
			{
				hr = ipThisFGModel->setRawData(lpRecord->mRowOrdinal, lOffset, *lpRecord->mvpText[iColumnIndex], mHeaderInfo[iColumnIndex].mDataType);
				CHECK_HR;
			}
		}
	}
	
	//6, add the new record in the lookup table
	if (ipRecordSrc->mFlag != DssTransactionDataCommit)
	{
		mUpdatedRecords[RecordKey(lNewRow, lNewCol)] = lpRecord;
	}
	
    //add the record to submitted rows
    if(ibSubmitted)
    {
        RecordKey lKey = RecordKey(lNewRow,lNewCol);
        mSubmittedRecords.push_back(std::pair<RecordKey,TransactionRecord*>(lKey, lpRecord));
    }
	return S_OK;
}

int DSSDataModelTransaction::GenerateRecordFromXTabView(DSSXTabView *ipXTabView, int iRow, int iCol, vector<AERawData> &orvRawData, vector<AERawData> &orvDisplayData)
{
	return ipXTabView->GenerateRecord(iRow, iCol, mapTrxColumn2XTabUnit, mapTrxColumn2Metric, mapTrxColumn2WriteBackForm, orvRawData, orvDisplayData);
}

int DSSDataModelTransaction::hPopulateOneRecord(DSSXTabView *ipXTabViewSrc, DSSXTabView *ipXTabView,
												TransactionRecord * ipRecordSrc, vector<int> &rvKeys, bool ibSubmitted)
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
    vector<AERawData> lvDisplayData;
	hr = this->GenerateRecordFromXTabView(ipXTabView, lNewRow, lNewCol, lvRawData, lvDisplayData);
	CHECK_HR;
	
	//3, create a new record
	TransactionRecord *lpRecord = hCreateNewRecord(ipRecordSrc->mFlag, &lvRawData, &lvDisplayData);
	CHECK_PTR(lpRecord);
	
	//4, copy other data members of the record
    lpRecord->mbMarked = ipRecordSrc->mbMarked;
    lpRecord->mbValidate = ipRecordSrc->mbValidate;
	lpRecord->mOrder = ipRecordSrc->mOrder;
	lpRecord->mvDataOffset.assign(ipRecordSrc->mvDataOffset.begin(), ipRecordSrc->mvDataOffset.end());
	lpRecord->mvOriginalFieldText.assign(ipRecordSrc->mvOriginalFieldText.begin(), ipRecordSrc->mvOriginalFieldText.end());
	vector<MBase::CString*> &lrTextStrings = ipRecordSrc->mvpText;
    vector<MBase::CString*> &lrDisplayTextStrings = ipRecordSrc->mvpDisplayText;
	int iColumnIndex = 0;
	for (iColumnIndex = 0; iColumnIndex < mnColumns; iColumnIndex++)
	{
		if (lrTextStrings[iColumnIndex])
		{
			lpRecord->mvpText[iColumnIndex] = new MBase::CString(*lrTextStrings[iColumnIndex]);
		}
        
        if (lrDisplayTextStrings[iColumnIndex])
		{
			lpRecord->mvpDisplayText[iColumnIndex] = new MBase::CString(*lrDisplayTextStrings[iColumnIndex]);
		}
		//make a copy of the original data
		lpRecord->mvOriginalData[iColumnIndex] = ipRecordSrc->mvOriginalData[iColumnIndex];
		lpRecord->mvOriginalData[iColumnIndex].TakeOwnership();
		
        lpRecord->mvDisplayData[iColumnIndex] = ipRecordSrc->mvDisplayData[iColumnIndex];
        lpRecord->mvDisplayData[iColumnIndex].TakeOwnership();
        
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

    //add the record to submitted rows
    if(ibSubmitted)
    {
        RecordKey lKey = RecordKey(lNewRow,lNewCol);
        mSubmittedRecords.push_back(std::pair<RecordKey,TransactionRecord*>(lKey, lpRecord));
    }
	return S_OK;
}

int DSSDataModelTransaction::hMapRowAndColumns(int iRowOrdinalSrc, int iColOrdinalSrc,
											   vector<int> &irvKeys, DSSXTabView *ipXTabViewSrc,
											   int &orNewRow, int &orNewCol)
{
	DSSDataModelXTabImpl *lpXTabModel = static_cast<DSSDataModelXTabImpl*>(mpSourceDataModel);
	int lnUnits = lpXTabModel->Count();
	_ASSERT(irvKeys.size() == lnUnits);
    
	int lMetricUnitID = lpXTabModel->getMetricUnitID(); 
    
	DSSXTabView *lpXTabView = lpXTabModel->getXTabView(); 
    
    return hMapRowAndColumns(iRowOrdinalSrc, iColOrdinalSrc, irvKeys, ipXTabViewSrc, lpXTabView, orNewRow, orNewCol, lnUnits, lMetricUnitID);
}

int DSSDataModelTransaction::hMapRowAndColumns(int iRowOrdinalSrc, int iColOrdinalSrc,
											   vector<int> &irvKeys, DSSXTabView *ipXTabViewSrc, DSSXTabView *lpXTabView,
											   int &orNewRow, int &orNewCol, int lnUnits, int lMetricUnitID)
{
	int hr = S_OK;
	
	for (int i = 0; i < lnUnits; i++)
	{
		irvKeys[i] = DSSXTAB_TOTAL_KEY;
	}
	
	int lMetricKey = 0;
	hr = ipXTabViewSrc->getHeader(0)->FillIndex(iRowOrdinalSrc, lnUnits, &irvKeys[0], &lMetricKey);
	CHECK_HR;
	
	hr = ipXTabViewSrc->getHeader(1)->FillIndex(iColOrdinalSrc, lnUnits, &irvKeys[0], &lMetricKey);
	CHECK_HR;
	
	irvKeys[lMetricUnitID] = lMetricKey;

    //fali, 939000, For DE, Not all index row is displayed. Only get the displayed offset
	orNewRow = lpXTabView->getHeader(0)->getDisplayOffsetByElements(irvKeys);
	orNewCol = lpXTabView->getHeader(1)->getDisplayOffsetByElements(irvKeys);
	
	if(orNewRow <  0 && lpXTabView->getHeader(0)->getNumUnits() == 0)
		orNewRow = 0;
	if(orNewCol < 0 && lpXTabView->getHeader(1)->getNumUnits() == 0)
		orNewCol = 0;
	
//	AE_ASSERT(orNewRow >= 0 && orNewCol >= 0);
	
	return S_OK;
}

int DSSDataModelTransaction::hMapRowForFieldGroup(DSSDataModelFieldGroupImpl* ipFGModelSrc, DSSDataModelFieldGroupImpl* ipThisFGModel, int& iRow)
{
	int hr = S_OK;
	
	CHECK_PTR(ipFGModelSrc);
	
//	DSSDataModelFieldGroupImpl *lpFGModel = static_cast<DSSDataModelFieldGroupImpl*> (mpSourceDataModel);
    
	DSSViewMap* lpViewMapSrc = NULL;
	hr = ipFGModelSrc->getViewMap(&lpViewMapSrc);
	CHECK_HR;
	
	DSSViewMap* lpViewMap = NULL;
	hr = ipThisFGModel->getViewMap(&lpViewMap);
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
        
        hr = lpViewMapSrc->MapGBKeys();
        CHECK_HR;
        
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
			
			int lUnit = ipThisFGModel->FindUnit(lGUID, DssTemplateReserved);
			if(lUnit < 0)
				return E_INVALIDINDEX;
			
			DSSTabularUnit* lpUnit = ipThisFGModel->Item(lUnit);
			if(!lpUnit)
				return E_POINTER;
			
			//bascially j should equal to i
			int j = lpViewMap->findGBUnit(lGUID);
			if(j < 0 || j >= lnGBUnit)
				return E_INVALIDINDEX;
			
			hr = lpUnit->FindElement(lpElement, lvKeys[j]);
			CHECK_HR;
			
		}
        
        hr = lpViewMapSrc->UnMapGBKeys();
        CHECK_HR;
        
        hr = lpViewMap->MapGBKeys();
        CHECK_HR;
        
		int* lpKeys = &lvKeys[0];
		hr = lpViewMap->getView(lpKeys, lnGBUnit, iRow);
		CHECK_HR;
        
        hr = lpViewMap->UnMapGBKeys();
        CHECK_HR;
        
        hr = ipThisFGModel->getRowOrdinal(iRow);
        CHECK_HR;
		
	}
	return S_OK;
}

int DSSDataModelTransaction::hSaveTransactionRecordToBinary(DSSBinaryDataWriter* ipBinaryWriter, int lSourceModelType, int lOriginalRow,int lOriginalColumn, TransactionRecord* lpRecord)
{
    int hr = S_OK;
    //transaction type
    CHECK_PTR(ipBinaryWriter);
    
    hr = ipBinaryWriter->WriteInt(lpRecord->mFlag);
    CHECK_HR;
      
    //record key
    hr = ipBinaryWriter->WriteInt(lOriginalRow);
    CHECK_HR;
    hr = ipBinaryWriter->WriteInt(lOriginalColumn);
    CHECK_HR;
    
    hr = ipBinaryWriter->WriteInt(lpRecord->mbMarked);
    CHECK_HR;
   
    hr = ipBinaryWriter->WriteInt(lpRecord->mbValidate);
    CHECK_HR;
    
    //write the value of the changed cells
    //first write changed cell count(dummy)
    int lnCell = 0;
    int lPos = ipBinaryWriter->GetPosition();
    hr = ipBinaryWriter->WriteInt(lnCell);
    CHECK_HR;
    
    for(int i = 0; i < lpRecord->mvpText.size(); i++)
    {
        if(lpRecord->mvpText[i] != NULL)
        {
            lnCell++;
            
            //column index
            hr = ipBinaryWriter->WriteInt(i);
            CHECK_HR;
            
            //changed value
            hr = ipBinaryWriter->WriteInt(lpRecord->mvpText[i]->size());
            CHECK_HR;
            
            hr = ipBinaryWriter->Write((const unsigned char*)(lpRecord->mvpText[i]->c_str()), lpRecord->mvpText[i]->size());
            CHECK_HR;
            
            //TODO: for DIC, we also need to save the mvpDisplayText
            if(lpRecord->mvpDisplayText[i])
            {
                hr = ipBinaryWriter->WriteInt(lpRecord->mvpDisplayText[i]->size());
                CHECK_HR;
                
                hr = ipBinaryWriter->Write((const unsigned char*)(lpRecord->mvpDisplayText[i]->c_str()), lpRecord->mvpDisplayText[i]->size());
                CHECK_HR; 
            }
            else
            {
                hr = ipBinaryWriter->WriteInt(-1);
                CHECK_HR;
            }
            
            //changed cell offset for header cells
            hr = ipBinaryWriter->WriteInt(lpRecord->mvDataOffset[i]);
            CHECK_HR;
            
            //the original value for the modified cell, used for discard
            hr = ipBinaryWriter->WriteInt(lpRecord->mvOriginalData[i].mFlag);
            CHECK_HR;
            
            hr = ipBinaryWriter->WriteInt(lpRecord->mvOriginalData[i].mDataType);
            CHECK_HR;
            
            hr = ipBinaryWriter->WriteInt(lpRecord->mvOriginalData[i].mSizeInByte);
            CHECK_HR;
            
            if(lpRecord->mvOriginalData[i].mValue)
            {
                hr = ipBinaryWriter->Write((unsigned char*)(lpRecord->mvOriginalData[i].mValue), lpRecord->mvOriginalData[i].mSizeInByte);
                CHECK_HR;
            }
            else
            {
                if(lpRecord->mvOriginalData[i].mSizeInByte > 0)
                {
                    vector<unsigned char> lvDummy(lpRecord->mvOriginalData[i].mSizeInByte);
                    hr = ipBinaryWriter->Write(&lvDummy[0], lpRecord->mvOriginalData[i].mSizeInByte);
                    CHECK_HR;
                }
            }
            if(lSourceModelType == DssModelFieldGroup)
            {
                hr = ipBinaryWriter->WriteInt(lpRecord->mvOriginalFieldText[i].size());
                CHECK_HR;
                
                hr = ipBinaryWriter->Write((unsigned char*)(lpRecord->mvOriginalFieldText[i].c_str()), lpRecord->mvOriginalFieldText[i].size());
                CHECK_HR;	
            }
        }
    }
    hr = ipBinaryWriter->WriteIntAtOffset(lPos, lnCell);
    CHECK_HR;

	return S_OK;
}

int DSSDataModelTransaction::hSaveModelInfoToBinary(DSSBinaryDataWriter &irWriter, int &orSourceModelType)
{
    int hr = S_OK;
	
    CHECK_PTR(mpSourceDataModel);
    
	//current slice flag 
	int currentSlice = mpSourceDataModel->getCurrentSliceFlag(); 
	hr = irWriter.WriteInt(currentSlice);
	CHECK_HR; 
	
	//source model type 
	orSourceModelType = mpSourceDataModel->getModelType(); 
	hr = irWriter.WriteInt(orSourceModelType);
	CHECK_HR; 
    
	return S_OK;
}

int DSSDataModelTransaction::SaveToBinary(bool includeCommitted, unsigned char** oppBinaryModel, int* opSize)
{
	int hr = S_OK;
	
	DSSBinaryDataWriter lBinaryWriter;
	lBinaryWriter.Init();
    
	int lSourceModelType = 0; 
    hr = hSaveModelInfoToBinary(lBinaryWriter, lSourceModelType);
    CHECK_HR;
    
	//the xtab and baseform info stored in the transaction column
	vector<int> lvColumnIndex;
	for(int i = 0; i < mHeaderInfo.size(); i++)
	{
		if(mHeaderInfo[i].mpTabularUnit)
		{
			lvColumnIndex.push_back(i);
		}
	}
    for(int i = 0; i < mvOfflineColumns.size(); i++)
	{
		if(mvOfflineColumns[i].mpTabularUnit)
		{
			lvColumnIndex.push_back(i + mHeaderInfo.size());
		}
	}
	
	hr = lBinaryWriter.WriteInt(lvColumnIndex.size());
	CHECK_HR;
	
	for(int i = 0; i < lvColumnIndex.size(); i++)
	{
		//column index
		hr = lBinaryWriter.WriteInt(lvColumnIndex[i]);
		CHECK_HR;
		
		//xtab unit guid
		GUID lGUID = GUID_NULL;
		hr = lvColumnIndex[i] < mHeaderInfo.size() ? mHeaderInfo[lvColumnIndex[i]].mpTabularUnit->getGUID(lGUID) : mvOfflineColumns[lvColumnIndex[i] - mHeaderInfo.size()].mpTabularUnit->getGUID(lGUID);
		CHECK_HR;
		
		hr = lBinaryWriter.Write((unsigned char*)(&lGUID), sizeof(GUID));
		CHECK_HR;
		
		//baseform
        int lBaseForm = lvColumnIndex[i] < mHeaderInfo.size() ? mHeaderInfo[lvColumnIndex[i]].mBaseForm : mvOfflineColumns[lvColumnIndex[i] - mHeaderInfo.size()].mBaseForm;
		hr = lBinaryWriter.WriteInt(lBaseForm);
		CHECK_HR;
	}
    
    hr = lBinaryWriter.WriteInt(mvOfflineColumns.size());
	CHECK_HR;
    
    for(int i = 0; i < mvOfflineColumns.size(); i++)
    {
        hr = lBinaryWriter.WriteInt(mvOfflineColumns[i].mDataType);
        CHECK_HR;
    }
	
	//write the transaction records
	//1.first write the record count (dummy)
	int lnRecord = 0;
	int lPos0 = lBinaryWriter.GetPosition();
	hr = lBinaryWriter.WriteInt(lnRecord);
	CHECK_HR;

    //2.1 write the submitted records
    if(includeCommitted)
    {
        for(int i = 0; i < mSubmittedRecords.size(); i++)
        {
            RecordKey lPair = mSubmittedRecords[i].first;
            TransactionRecord* lpRecord = mSubmittedRecords[i].second;
            if(lpRecord)
            {
                lnRecord++;
                int lOriginalRow = lPair.first * mRowOrdinalAdjustor;
                int lOriginalColumn = lPair.second * mColOrdinalAdjustor;
                hr = hSaveTransactionRecordToBinary(&lBinaryWriter, lSourceModelType, lOriginalRow, lOriginalColumn, lpRecord);
                CHECK_HR; 
            }
        }
    }
	//2.2 then write the update records
	for(RecordMap::iterator it = mUpdatedRecords.begin(); it != mUpdatedRecords.end(); it++)
	{
		RecordKey lPair = it->first;
		int lOriginalRow = lPair.first * mRowOrdinalAdjustor;
		int lOriginalColumn = lPair.second * mColOrdinalAdjustor;
		TransactionRecord* lpRecord = it->second;
        
        CHECK_HR;
	//	if(lpRecord->mFlag == DssTransactionDataUpdate || (lpRecord->mFlag == DssTransactionDataCommit && includeCommitted))
        if(lpRecord->mFlag == DssTransactionDataUpdate 
             || (lpRecord->mFlag == DssTransactionDataCommit && includeCommitted)//sometimes we need to restore the submitted changes
             ||lpRecord->mFlag == DssTransactionDataOriginal)//for unmarked rows, we still need to retain the changes 
		{
			lnRecord++;
            
            hr = hSaveTransactionRecordToBinary(&lBinaryWriter, lSourceModelType, lOriginalRow, lOriginalColumn, lpRecord);
            CHECK_HR; 
        }
	}

    //618234, for FG cases, we need to store the rawdata
    //zhyang, 671285
    if(mpSourceDataModel && mpSourceDataModel->getModelType() == DssModelFieldGroup)
    {
        vector<int> lvRow;
        vector<int> lvField;
        vector<MBase::CString> lvText;
        for (RecordMap::iterator it = mUpdatedRecords.begin(); it != mUpdatedRecords.end(); it++)
        {
            int lOriginalRow = it->first.first;
            TransactionRecord *lpRecord = it->second;
            if (lpRecord && lpRecord->mFlag >= DssTransactionDataUpdate && lpRecord->mFlag <= DssTransactionDataInsert)
            {
                for (int iColumnIndex = 0; iColumnIndex < mnColumns; iColumnIndex++)
                {
                    int lField = lpRecord->mvDataOffset[iColumnIndex];
                    if(lField >= 0 && lpRecord->mvpText[iColumnIndex])
                    {
                        lvRow.push_back(lOriginalRow);
                        lvField.push_back(lField);
                        lvText.push_back(*lpRecord->mvpText[iColumnIndex]);
                    }
                }
            }
        }
        lBinaryWriter.WriteInt(lvField.size());
        for(int i = 0;i < lvField.size();i++)
        {
            hr = lBinaryWriter.WriteInt(lvRow[i]);
            CHECK_HR;
            
            hr = lBinaryWriter.WriteInt(lvField[i]);
            CHECK_HR;
            
            hr = lBinaryWriter.WriteInt(lvText[i].size());
            CHECK_HR;
            
            hr = lBinaryWriter.Write((const unsigned char*)(lvText[i].c_str()), lvText[i].size());
            CHECK_HR;
        }
    }

	hr = lBinaryWriter.WriteIntAtOffset(lPos0, lnRecord);
	CHECK_HR;
	
	hr = lBinaryWriter.getBinary(oppBinaryModel, opSize);
	CHECK_HR;

	return S_OK;
}

int DSSDataModelTransaction::setColumnInfo(int iColumnIndex, DSSTabularUnit* ipUnit, int iForm)
{
	if(iColumnIndex < 0 || iColumnIndex >= mHeaderInfo.size() + mvOfflineColumns.size())
		return E_INVALIDINDEX;
	
    if(iColumnIndex < mHeaderInfo.size())
    {
        mHeaderInfo[iColumnIndex].mpTabularUnit = ipUnit;
        mHeaderInfo[iColumnIndex].mBaseForm = iForm;
	}else{
        mvOfflineColumns[iColumnIndex - mHeaderInfo.size()].mpTabularUnit = ipUnit;
        mvOfflineColumns[iColumnIndex - mHeaderInfo.size()].mBaseForm = iForm;
    }
	return S_OK;
}


int DSSDataModelTransaction::getColumnInfo(int iColumnIndex, DSSTabularUnit** oppUnit, int* opForm)
{
	if(iColumnIndex < 0 || iColumnIndex >= mHeaderInfo.size() + mvOfflineColumns.size())
		return E_INVALIDINDEX;
	
    if(iColumnIndex < mHeaderInfo.size())
    {
        if(oppUnit)
            (*oppUnit) = mHeaderInfo[iColumnIndex].mpTabularUnit;
        if(opForm)
            (*opForm) = mHeaderInfo[iColumnIndex].mBaseForm;
	}else{
        if(oppUnit)
            (*oppUnit) = mvOfflineColumns[iColumnIndex - mHeaderInfo.size()].mpTabularUnit;
        if(opForm)
            (*opForm) = mvOfflineColumns[iColumnIndex - mHeaderInfo.size()].mBaseForm;
    }
	return S_OK;
}

EnumDSSDataType DSSDataModelTransaction::getColumnDataType(int iColumnIndex)
{
    if(iColumnIndex >= 0 && iColumnIndex < mHeaderInfo.size())
        return mHeaderInfo[iColumnIndex].mDataType;
    if(iColumnIndex >= mHeaderInfo.size() && iColumnIndex - mHeaderInfo.size() < mvOfflineColumns.size())
        return mvOfflineColumns[iColumnIndex - mHeaderInfo.size()].mDataType;
    return DssDataTypeMissing;
}

int DSSDataModelTransaction::setColumnDataType(int iColumnIndex, EnumDSSDataType iDataType)
{
    if(iColumnIndex >= 0 && iColumnIndex < mHeaderInfo.size())
    {
        mHeaderInfo[iColumnIndex].mDataType = iDataType;
    }
    if(iColumnIndex >= mHeaderInfo.size() && iColumnIndex - mHeaderInfo.size() < mvOfflineColumns.size())
    {
        mvOfflineColumns[iColumnIndex - mHeaderInfo.size()].mDataType = iDataType;
    }
    return S_OK;
}

int DSSDataModelTransaction::setRecordFlag(int iOriginalRow, int iOriginalColumn, EnumDSSTransactionDataChangeType lFlag)
{
	int lAdjustedRow = iOriginalRow;
	int lAdjustedCol = iOriginalColumn;
	hAdjustOrdinal(lAdjustedRow, lAdjustedCol);
		
	RecordMap::iterator it = mUpdatedRecords.find(RecordKey(lAdjustedRow, lAdjustedCol));
	if (it != mUpdatedRecords.end())
	{
		TransactionRecord *lpRecord = it->second;
		lpRecord->mFlag = lFlag;
	}
	return S_OK;
}

int DSSDataModelTransaction::getColumnMappingInfo(vector<int>** oppMapTrxColumn2XTabUnit, vector<int>** oppMapTrxColumn2BaseForm, vector<int>** oppMapTrxColumn2WBForm)
{
	if(oppMapTrxColumn2XTabUnit)
		(*oppMapTrxColumn2XTabUnit) = &mapTrxColumn2XTabUnit;
	
	if(oppMapTrxColumn2BaseForm)
		(*oppMapTrxColumn2BaseForm) = &mapTrxColumn2Metric;
	
    if(oppMapTrxColumn2WBForm)
        (*oppMapTrxColumn2WBForm) = &mapTrxColumn2WriteBackForm;
	return S_OK;
}

int DSSDataModelTransaction::getOfflineColumnMappingInfo(vector<int>** oppMapTrxColumn2XTabUnit, vector<int>** oppMapTrxColumn2BaseForm, vector<int>** oppMapTrxColumn2WBForm)
{
	if(oppMapTrxColumn2XTabUnit)
		(*oppMapTrxColumn2XTabUnit) = &mapOfflineTrxColumn2XTabUnit;
	
	if(oppMapTrxColumn2BaseForm)
		(*oppMapTrxColumn2BaseForm) = &mapOfflineTrxColumn2Metric;
	
    if(oppMapTrxColumn2WBForm)
        (*oppMapTrxColumn2WBForm) = &mapOfflineTrxColumn2WriteBackForm;
	return S_OK;
}

//only used for Field group
int DSSDataModelTransaction::setOriginalFieldText(int iOriginalRow, int iOriginalColumn, int iColumnIndex, std::string iFieldText)
{
	int lAdjustedRow = iOriginalRow;
	int lAdjustedCol = iOriginalColumn;
	hAdjustOrdinal(lAdjustedRow, lAdjustedCol);
	
	RecordMap::iterator it = mUpdatedRecords.find(RecordKey(lAdjustedRow, lAdjustedCol));
	if (it != mUpdatedRecords.end())
	{
		TransactionRecord *lpRecord = it->second;
		lpRecord->mvOriginalFieldText[iColumnIndex] = iFieldText;
	}
	return S_OK;
}

int DSSDataModelTransaction::getMarkedRows(vector<int>& ivRows)
{
	ivRows.clear();
	for(RecordMap::iterator iter = mUpdatedRecords.begin(); iter != mUpdatedRecords.end(); iter++)
	{
		int lRow = iter->first.first * mRowOrdinalAdjustor;
		TransactionRecord* lpRecord = iter->second;

        if(lpRecord->mbMarked)
		//if(lpRecord->mFlag != DssTransactionDataOriginal)//otherwise it is a unmarked row
		{
			ivRows.push_back(lRow);
		}
	}
	return S_OK;
}

bool DSSDataModelTransaction::IsDataUpdated(int iOriginalRow, int iOriginalCol, DSSTabularUnit* ipTabularUnit, int iBaseForm)
{
	for(int i = 0; i < mHeaderInfo.size(); i++)
	{
		if(mHeaderInfo[i].mpTabularUnit == ipTabularUnit && mHeaderInfo[i].mBaseForm == iBaseForm)
		{
			return IsDataUpdated(iOriginalRow, iOriginalCol, i);
		}
	}
	return false;
}

int DSSDataModelTransaction::markRow(int iOriginalRow, int lOriginalCol)
{
    int lAdjustedRow = iOriginalRow;
	int lAdjustedCol = lOriginalCol;
	hAdjustOrdinal(lAdjustedRow, lAdjustedCol);
    RecordMap::iterator it = mUpdatedRecords.find(RecordKey(lAdjustedRow, lAdjustedCol));
	TransactionRecord *lpRecord = NULL;
	if (it != mUpdatedRecords.end())
	{
		lpRecord = it->second;
		lpRecord->mbMarked = 1;
	}
    return 0;
}

int DSSDataModelTransaction::setValidationFailedFlag(int iOriginalRow, int lOriginalCol)
{
    int lAdjustedRow = iOriginalRow;
	int lAdjustedCol = lOriginalCol;
	hAdjustOrdinal(lAdjustedRow, lAdjustedCol);
    RecordMap::iterator it = mUpdatedRecords.find(RecordKey(lAdjustedRow, lAdjustedCol));
	TransactionRecord *lpRecord = NULL;
	if (it != mUpdatedRecords.end())
	{
		lpRecord = it->second;
		lpRecord->mbValidate = 0;
	}
    return 0;
}

bool DSSDataModelTransaction::validateRequiredColumns(bool ibOnlyValidateEmpty)
{
    UpdateRequiredColumnStatus();
    bool lbSuccess = true;
    mbIsValidated = true;
    for(RecordMap::iterator iter = mUpdatedRecords.begin(); iter != mUpdatedRecords.end(); iter++)
	{
		TransactionRecord* lpRecord = iter->second;
        if(lpRecord->mFlag == DssTransactionDataOriginal)
            continue;
        bool lbSuccessForRecord = true;
        for(int i = 0; i < mHeaderInfo.size(); i++)
        {
            //    check if a column is required
            if(mHeaderInfo[i].mbRequired)
            {
                if(ibOnlyValidateEmpty)
                {
                    if(lpRecord->mvpText[i])
                    {
                        std::string mText = *(lpRecord->mvpText[i]);
                        if(mText == "")
                        {
                            lbSuccessForRecord = false;
                            break;
                        }
                    }else
                    {
                        std::string lText = "";
                        if(DSSAEHelper::IsDate(mHeaderInfo[i].mDataType) && lpRecord->mvOriginalData[i].mFlag == DssDataNull)//1002045
                        {
                            lText = "";
                        }else{
                            ConvertDataTypeAndToString(lpRecord->mvOriginalData[i], mHeaderInfo[i].mDataType, lText);
                        }
                        if(lText == "")
                        {
                            lbSuccessForRecord = false;
                            break;
                        }
                    }
                }else
                {
                    if (!lpRecord->mvpText[i])
                    {
                        lbSuccessForRecord = false;
                        break;
                    }
                    std::string lText = "";
                    ConvertDataTypeAndToString(lpRecord->mvOriginalData[i], mHeaderInfo[i].mDataType, lText);
                    std::string mText = *(lpRecord->mvpText[i]);
                    if(mText == "" || mText == lText)
                    {
                        lbSuccessForRecord = false;
                        break;
                    }
                    else if(mHeaderInfo[i].mDataType == DssDataTypeDate)
                    {
                        if(lText != "" && strstr(mText.c_str(), lText.c_str()))
                        {
                            lbSuccessForRecord = false;
                            break;
                        }
                    }
                    else if(mHeaderInfo[i].mDataType == DssDataTypeTimeStamp ||mHeaderInfo[i].mDataType == DssDataTypeTime)
                    {
                        ConvertDateTime(lText);
                        ConvertDateTime(mText);
                        if(lText != "" && strstr(lText.c_str(), mText.c_str()))
                        {
                            lbSuccessForRecord = false;
                            break;
                        }
                    }
                }
            }
        }
        if(!lbSuccessForRecord)
        {
            lbSuccess = false;
            iter->second->mbValidate = 0;
        }
        mvValidatedRows.push_back(iter->first.first);
	}
    return lbSuccess;
}

int DSSDataModelTransaction::getValidationFailureRows(vector<int>& vRows)
{
    int hr = S_OK;
    vRows.clear();
    for(RecordMap::iterator iter = mUpdatedRecords.begin(); iter != mUpdatedRecords.end(); iter++)
	{
		TransactionRecord* lpRecord = iter->second;
        if(lpRecord->mFlag == DssTransactionDataOriginal)
            continue;
        if(lpRecord->mbValidate == 0)
            vRows.push_back(iter->first.first * mRowOrdinalAdjustor);
	}

    return hr;
}

bool DSSDataModelTransaction::isColumnRequired(int iColumnIndex)//0-based
{
    UpdateRequiredColumnStatus();
    if(iColumnIndex >= 0 && iColumnIndex < mHeaderInfo.size())
        return mHeaderInfo[iColumnIndex].mbRequired;
    return false;
}

IDSSSourceModelForTransaction* DSSDataModelTransaction::getSourceDataModel()
{
    return mpSourceDataModel;
}

bool DSSDataModelTransaction::HasUpdateRecord()
{
    for(int i = 0; i < mvpRecord.size();i++)
    {
        if(mvpRecord[i]->mFlag == DssTransactionDataUpdate)
            return true;
    }
    return false;
}

bool DSSDataModelTransaction::isModelValidated(int iRow)
{
    if(iRow < 0)
        return mbIsValidated;
    
    int lAdjustedRow = iRow;
	int lAdjustedCol = 0;
	hAdjustOrdinal(lAdjustedRow, lAdjustedCol);
    for(int i = 0; i < mvValidatedRows.size(); i++)
        if(lAdjustedRow == mvValidatedRows[i])
            return true;
    return false;
}

int DSSDataModelTransaction::GetTransactionCount()
{
    //TODO: move the logic of generating mvUpdateRecords to setAllCommit
    //fali, 943105, user may operate multi-times, then the mFlag may change, collect mvUpdateRecords at every call.
    if(mvUpdateRecords.size() != 0)
    {
        mvUpdateRecords.clear();
    }
    for(int i = 0; i < mvpRecord.size();i++)
    {
        if(mvpRecord[i]->mFlag <= DssTransactionDataInsert
           && mvpRecord[i]->mFlag >= DssTransactionDataUpdate)
            mvUpdateRecords.push_back(i);
    }
    return mvUpdateRecords.size();
}

int DSSDataModelTransaction::GetColumnCount()
{
    return mHeaderInfo.size();
}

int DSSDataModelTransaction::GetOfflineColumnCount()
{
    return mvOfflineColumns.size();
}
int DSSDataModelTransaction::GetValue(int iRow, int iColumn, string* oText, EnumDSSDataType* oDataType)
{
    if(iRow < 0 || iColumn < 0 || iRow >= mvUpdateRecords.size() || iColumn >= mHeaderInfo.size() + mvOfflineColumns.size()
       ||mvUpdateRecords[iRow] < 0 || mvUpdateRecords[iRow] >= mvpRecord.size())
        return S_FALSE;
    
    TransactionRecord* lpRecord = mvpRecord[mvUpdateRecords[iRow]];
    EnumDSSDataType lDataType = DssDataTypeReserved;
    if(iColumn >= mHeaderInfo.size())
    {
        int lColumn = mapOfflineTrxColumn2TrxColumn[iColumn - mHeaderInfo.size()];
        //lDataType = mvOfflineColumns[iColumn - mHeaderInfo.size()].mDataType;
        if(lColumn >= 0 && lColumn <mHeaderInfo.size())
        {
            iColumn = lColumn;
        }
        AERawData* lpRawData = &(lpRecord->mvOriginalData[iColumn]);
        lDataType = lpRawData->mDataType;
    }
    else
    {
        lDataType = mHeaderInfo[iColumn].mDataType;
    }
    
    if(oText)
    {
        //TransactionRecord* lpRecord = mvpRecord[mvUpdateRecords[iRow]];
        if (!lpRecord->mvpText[iColumn])	//original data
        {
            AERawData *lpRawData = &(lpRecord->mvOriginalData[iColumn]);
            if (lpRawData->mDataType != lDataType && lpRawData->mFlag == DssDataOk)
            {
                //465875, formate the raw data using the transaction report data type
                ConvertDataTypeAndToString(*lpRawData, lDataType, *oText);
            }
            else
            {
                EnumDSSDataType lType = lpRawData->mDataType;
                if(lType == DssDataTypeTime)
                    lType = DssDataTypeTimeStamp;
                DSSAEHelper::ConvertToString(lpRawData->mValue, lpRawData->mSizeInByte, lType, lpRawData->mFlag, *oText);
            }
        }
        else	//updated data
        {
            (*oText) = *lpRecord->mvpText[iColumn];
        }
    }
    
    if(oDataType)
        (*oDataType) = lDataType;
    
    return S_OK;
}

int DSSDataModelTransaction::UpdateRequiredColumnStatus()
{
    //get the "required" column info
  	DSSObjectContext* lpObjectContext = mpSourceDataModel->getObjectContext();
    vector<int> lvRequiredColumnIndices;
    if(lpObjectContext)
    {
        for(int i = 0; i < mHeaderInfo.size(); i++)
        {
            mHeaderInfo[i].mbRequired = false;
        }

        
        DSSRWNode* lpNode = lpObjectContext->getNode(mpSourceDataModel->getNodeKey());
        if(lpNode->getType() == DssRWNodeTemplate)
        {
            lvRequiredColumnIndices = ((DSSRWTemplateNode*)lpNode)->getRequiredColumnIndexs();
        }
        else if(lpNode->getType() == DssRWNodeFieldGroup)
        {
            lvRequiredColumnIndices = ((DSSRWFieldGroupNode*)lpNode)->getRequiredColumnIndexs();
        }
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

int DSSDataModelTransaction::setOfflineTransaction(vector<int> &irvUnitMap, vector<int> &irvBaseFormMap, vector<int> &irvWBForm, vector<EnumDSSDataType>& irvDataType, vector<int> &irvMapOfflineColumns)
{
    int hr = S_OK;
    int lnColumns = irvUnitMap.size();
    if(lnColumns > 0)
    {
        mbHasOfflineTrx = true;
        mnOfflineColumns = lnColumns;
    }else{
        return hr;
    }

    if(irvDataType.size()!= lnColumns || irvBaseFormMap.size()!= lnColumns || irvWBForm.size() != lnColumns)
        return E_INVALIDARG;
    
    mvOfflineColumns.resize(lnColumns);
    for(int i = 0; i < lnColumns; i++)
    {
        mvOfflineColumns[i].mDataType = irvDataType[i];
        mvOfflineColumns[i].mBaseForm = 0;
        mvOfflineColumns[i].mbRequired = false;
        mvOfflineColumns[i].mpDefinition = NULL;
        mvOfflineColumns[i].mpTabularUnit = NULL;
    }
    mapOfflineTrxColumn2XTabUnit.assign(irvUnitMap.begin(), irvUnitMap.end());
	mapOfflineTrxColumn2Metric.assign(irvBaseFormMap.begin(), irvBaseFormMap.end());
    mapOfflineTrxColumn2WriteBackForm.assign(irvWBForm.begin(),irvWBForm.end());
    mapOfflineTrxColumn2TrxColumn.assign(irvMapOfflineColumns.begin(), irvMapOfflineColumns.end());
    return hr;
}

int DSSDataModelTransaction::DiscardTransactionOperation(int iOriginalRow, int iOriginalColumn, int iColumnIndex)
{
    if (mStatus != XTAB_INITIALIZED)
        return E_FAIL;
    //472924
    
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
    
    //undo changes
    if (mpSourceDataModel->getModelType() == DssModelXTab)
    {
        
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
        DSSCalculationPlan *lpCalcPlan = static_cast<DSSDataModelXTabImpl*> (mpSourceDataModel)->getCalculationPlan();
        CHECK_PTR(lpCalcPlan);
        
        
        //data is already restored, no need to recalculate
        //TQMS 613533,689419
        lpCalcPlan->setStatus(DssCalcStatusNeedEvalThres);
        //TQMS 476909 & 610782, in some cases (for 476909, there are derived metric in the xtab), when user updated some cells and submit the changes and then discard, we still need to recalculate
        //lpCalcPlan->MarkUnCalculated(0);
    }//end of if (mpSourceDataModel->getModelType() == DssModelXTab)
    else if (mpSourceDataModel->getModelType() == DssModelFieldGroup)
    {
        
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
            
            mUpdatedRecords.erase(it);
        }
    }
    
    return S_OK;

}
//de37999, change function signature to avoid memory leak.
string DSSDataModelTransaction::getTransactionReportID()
{
    char lIDChars[33];
    DSSBaseElementProxy::ConvertGUIDToString(mpTransactionReport->getID(), lIDChars);
    string lIDString(lIDChars);
    return lIDString;
}