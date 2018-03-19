/*
 *  DSSModelPopulation.cpp
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 1/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import "DSSModelPopulation.h"
#import "DSSBinaryDataReader.h"
#import "DSSStrongPtr.h"
#import "DSSDateTime.h"
#import "DSSCellFmtData.h"
#import "DSSBigDecimal.h"

#import "DSSAEHelper.h"
#import "DSSDataModelBase.h"
#import "DSSDataModelXTabImpl.h"
#import "DSSDataModelControlImpl.h"
#import "DSSDataModelFieldGroupImpl.h"
#import "DSSDataModelGroupbyImpl.h"

#import "DSSTabularUnit.h"
#import "DSSTabularAttribute.h"
#import "DSSTabularMetrics.h"
#import "DSSTabularIndexPool.h"
#import "DSSTabularIndex.h"
#import "DSSCube.h"
#import "DSSDataColumn.h"
#import "DSSCubeDataSlice.h"
#import "DSSTabularCustomGroup.h"
#import "DSSTabularConsolidation.h"

#import "DSSXTabView.h"
#import "DSSXTabHeader.h"
#import "DSSDataSource.h"

#import "DSSSort_includes.h"

#import "DSSDocumentInstance.h"
#import "DSSCalculationPlan.h"
#import "DSSCalculationEvent.h"
#include<algorithm>
#include<stdlib.h>


#include "DSSDataRefColumn.h"
#include "DSSGlobalLookUpTable.h"
#include "DSSGUnitContainer.h"
#include <unistd.h>

#define CHECK_HR_BLOCK(type) {CHECK_HR; if (type != lBlockType) return E_DATABLOCK;}

static const double gSecondsSince1970 = MDataType::DSSDateTime(1970, 1, 1).ToDATE();

template<class T1, class T2>
class ElementSort
{
public:
	ElementSort(T1 *ipKey, int nRows, int nUnits, T2 *ipKey2 = NULL): mpKey(ipKey), mnRows(nRows), mnUnits(nUnits), mCur(0), mpSecondaryKey(ipKey2)
	{}

	inline void ReInit(T1 *ipKey, int nRows, int nUnits)
	{
		mpKey = ipKey;
		mnRows = nRows;
		mnUnits = nUnits;
		mCur = 0;
	}

	inline void SetUnit(int iUnit)
	{	if (iUnit < mnUnits)	mCur = iUnit; }

	bool operator() (int iRow1, int iRow2)
	{
		if (!mpSecondaryKey)
			return mpKey[iRow1 * mnUnits + mCur] < mpKey[iRow2 * mnUnits + mCur];
		else
		{
			if (mpKey[iRow1 * mnUnits + mCur] != mpKey[iRow2 * mnUnits + mCur])
				return mpKey[iRow1 * mnUnits + mCur] < mpKey[iRow2 * mnUnits + mCur];
			else
				return mpSecondaryKey[iRow1] < mpSecondaryKey[iRow2];
		}
	}

private:
	T1 *mpKey;
	int mnRows;
	int mnUnits;
	int mCur;
	T2 *mpSecondaryKey;
};

template<class T>
int MergeVectors(vector<vector<T> > &rSrc, vector<T> &rDes)
{
	int lTotalSize = 0;
	int lnSlices = rSrc.size();
	int i = 0;
	for (; i < lnSlices; i++)
	{
		lTotalSize += rSrc[i].size();
	}

	vector<T> *lpVec = &rDes;
	lpVec->reserve(lTotalSize);
	for (i = 0; i < lnSlices; i++)
	{
		lpVec->insert(lpVec->end(), rSrc[i].begin(), rSrc[i].end());
	}
	return 0;
}

int LinearMapUnits(vector<int> &rSrcUnits, vector<int> &rTargetUnits, vector<int> &rSrc2TargetMap)
{
	int lnSrcUnits = rSrcUnits.size();
	int lnTgtUnits = rTargetUnits.size();
	rSrc2TargetMap.resize(lnSrcUnits);

	for (int i = 0; i < lnSrcUnits; i++) {
		int j = 0;
		for (j = 0; j < lnTgtUnits; j++) {
			if (rSrcUnits[i] == rTargetUnits[j])
			{
				rSrc2TargetMap[i] = j;
				break;
			}
		}
		if (j >= lnTgtUnits)
			return E_INVALIDINDEX;
	}
	return S_OK;
}

int ReadVariant(DSSBinaryDataReader *ipReadStream, CComVariant& rVal, int flag = 0)
{
	if (flag == 1)
		return ipReadStream->ReadVariant(rVal);

	//read AE's variant, vt type need converted
	rVal.Clear();

	int lType = DssDataTypeUnknown;
	int hr = ipReadStream->ReadInt(&lType);
	CHECK_HR;

	switch (lType)
	{
		case DssDataTypeMissing:	//VT_EMPTY
			rVal.vt = VT_EMPTY;
			hr = ipReadStream->ReadByte(&rVal.bVal);
			break;
		case DssDataTypeInteger:	//VT_I4
		{
			rVal.vt = VT_I4;
			int lVal = 0;
			hr = ipReadStream->ReadInt(&lVal);
			rVal.lVal = lVal;
		}
			break;
		case DssDataTypeShort:		//VT_I2
			rVal.vt = VT_I2;
			hr = ipReadStream->ReadShort(&rVal.iVal);
			break;
		case DssDataTypeChar:		//VT_UI1
			rVal.vt = VT_UI1;
			hr = ipReadStream->ReadByte(&rVal.bVal);
			break;

		case DssDataTypeBool:		//VT_BOOL
		{
			rVal.vt = VT_BOOL;
			BYTE lVal = 0;
			hr = ipReadStream->ReadByte(&lVal);
			rVal.boolVal = lVal != 0 ? VARIANT_TRUE : VARIANT_FALSE;
		}
			break;

		case DssDataTypeFloat:		//VT_R4:
			rVal.vt = VT_R4;
			hr = ipReadStream->ReadFloat(&rVal.fltVal);
			break;

		case DssDataTypeDouble:		//VT_R8:
			rVal.vt = VT_R8;
			hr = ipReadStream->ReadDouble(&rVal.dblVal);
			break;

		case DssDataTypeVarChar:	//VT_BSTR:
			rVal.vt = VT_BSTR;
			hr = ipReadStream->ReadBSTRVariant(rVal);
			break;
		case DssDataTypeBigDecimal:
		{
			MBase::CString lVal;
			hr = ipReadStream->ReadString(lVal);
			MDataType::DSSBigDecimal lBigDecimal(lVal.c_str());
		}
			break;
		case DssDataTypeTime:
		{
			hr = ipReadStream->ReadDouble(&rVal.date);
		}
			break;
		case DssDataTypeCellFormatData:
		{
			/*
             int lobjtype = 0;
             hr = ipReadStream->ReadInt(&lobjtype);
             CHECK_HR;
             
             if (lobjtype != DssDataTypeCellFormatData)
             return E_UNEXPECTED_DATATYPE;
             
             double lVal = 0;
             int lFmtType = 0;
             int lFmtID = 0;
             hr = ipReadStream->ReadDouble(&lVal);
             hr = ipReadStream->ReadInt(&lFmtType);
             hr = ipReadStream->ReadInt(&lFmtID);
             */
            //680047
            //treat the cellfmt type as a numeric value
            double lVal = 0;
            int lFmtType = 0;
            
            ipReadStream->ReadDouble(&lVal);
            ipReadStream->ReadInt(&lFmtType);//format type,not used in client
            
            MBase::CString lString;
			hr = ipReadStream->ReadString(lString);//format ID, not used in client
            
            rVal.vt = VT_R8;
            rVal.dblVal = lVal;
		}
			break;
		default:
			return E_FAIL;
	}

	return hr;
}

int ReadAERawData(DSSBinaryDataReader *ipReadStream, AERawData& irVal, int &iBufferSize)
{
	int lType = DssDataTypeUnknown;
	int hr = ipReadStream->ReadInt(&lType);
	CHECK_HR;

	irVal.mDataType = (EnumDSSDataType)lType;

	if (iBufferSize < 8)
	{
		if (irVal.mValue)
			delete irVal.mValue;
		iBufferSize = 8;
		irVal.mValue = new char[iBufferSize];
	}

	irVal.mFlag = DssDataOk;

	switch (lType)
	{
		case DssDataTypeMissing:
			hr = ipReadStream->ReadByte((BYTE*)irVal.mValue);
			irVal.mSizeInByte = sizeof(char);
			irVal.mFlag = DssDataNull;
			break;
		case DssDataTypeBool:
		case DssDataTypeChar:
			hr = ipReadStream->ReadByte((BYTE*)irVal.mValue);
			irVal.mSizeInByte = sizeof(char);
			break;

		case DssDataTypeInteger:
			irVal.mSizeInByte = sizeof(int);
			hr = ipReadStream->ReadInt((int*)irVal.mValue);
			break;

		case DssDataTypeShort:
			irVal.mSizeInByte = sizeof(short);
			hr = ipReadStream->ReadShort((short*)irVal.mValue);
			break;

		case DssDataTypeFloat:
			irVal.mSizeInByte = sizeof(float);
			hr = ipReadStream->ReadFloat((float*)irVal.mValue);
			break;

		case DssDataTypeDouble:
			irVal.mSizeInByte = sizeof(double);
			hr = ipReadStream->ReadDouble((double*)irVal.mValue);
			break;

		case DssDataTypeVarChar:
		{
			MBase::CString lString;
			hr = ipReadStream->ReadString(lString);

			int lDataSize = lString.length() + 1;

			if (iBufferSize < lDataSize)
			{
				delete irVal.mValue;
				iBufferSize = lDataSize * 2;
				irVal.mValue = new char[iBufferSize];
			}

			irVal.mSizeInByte = lDataSize;
			memmove(irVal.mValue, lString.c_str(), lDataSize);
		}
			break;
		case DssDataTypeBigDecimal:
		{
			//big decimal need special handle
			return S_FALSE;
		}
			break;
		case DssDataTypeTime:
		{
			//TODO:
			irVal.mSizeInByte = sizeof(double);
			double lTimeStamp = 0;
			hr = ipReadStream->ReadDouble(&lTimeStamp);

			MDataType::DSSDateTime lDateTime(lTimeStamp/(double)MDataType::DAY_IN_MILLISECONDS + gSecondsSince1970);

			int lDataSize = sizeof(MDataType::DSSDateTime);
			if (iBufferSize < lDataSize)
			{
				delete irVal.mValue;
				iBufferSize = lDataSize * 2;
				irVal.mValue = new char[iBufferSize];
			}

			irVal.mSizeInByte = lDataSize;
			memmove (irVal.mValue, (void*)&lDateTime, lDataSize);
		}
			break;
		case DssDataTypeCellFormatData:
		{
			/*
             int lobjtype = 0;
             hr = ipReadStream->ReadInt(&lobjtype);
             CHECK_HR;
             
             if (lobjtype != DssDataTypeCellFormatData)
             return E_UNEXPECTED_DATATYPE;
             
             double lVal = 0;
             int lFmtType = 0;
             int lFmtID = 0;
             hr = ipReadStream->ReadDouble(&lVal);
             hr = ipReadStream->ReadInt(&lFmtType);
             hr = ipReadStream->ReadInt(&lFmtID);
             */
            //680047
            //treat the cellfmt type as a numeric value
            double lVal = 0;
            int lFmtType = 0;
            
            ipReadStream->ReadDouble(&lVal);
            ipReadStream->ReadInt(&lFmtType);//format type,not used in client
            
            MBase::CString lString;
			hr = ipReadStream->ReadString(lString);//format ID, not used in client
            
            irVal.mDataType = DssDataTypeDouble;
            irVal.mSizeInByte = sizeof(double);
            memmove (irVal.mValue, (double*)&lVal, irVal.mSizeInByte);
		}
			break;
		default:
			return E_FAIL;
	}

	return hr;
}

int hLoadUnits(DSSBinaryDataReader *ipReadStream, DSSDataModelBase *ipModel, vector<int> *opvMetricDataTypes = NULL, int *opMetricUnitID = NULL, int *opDummyUnit = NULL);
int hLoadUnitAttribute(DSSBinaryDataReader *ipReadStream, int index, DSSTabularUnit **oppXTabUnit);
int hLoadUnitMetrics(DSSBinaryDataReader *ipReadStream, int index, DSSTabularUnit **oppXTabUnit, vector<int> *opvMetricDataTypes);
int hLoadUnitCustomGroup(DSSBinaryDataReader *ipReadStream, int index, DSSTabularUnit **oppXTabUnit);
int hLoadUnitConsolidation(DSSBinaryDataReader *ipReadStream, int index, DSSTabularUnit **oppXTabUnit, DSSGlobalLookUpTable* ipGlobalLookUp = NULL, bool ibLoadAttr = false);
int hLoadNullElement(DSSBinaryDataReader *ipReadStream, DSSTabularUnit *ipXTabUnit);

//junwang 09/01/2011 Red Rocket: issue 500695
int hLoadInputOrder(DSSBinaryDataReader *ipReadStream, DSSDataColumn *ipColumn, vector<int>& ivPartialElementList, int maxRowCount);

//load xtab model
int hLoadHeader(DSSBinaryDataReader *ipReadStream, int iAxis, LayoutInfo *ipLayoutInfo, vector<int> &irGBPath, int &rnRows,
					   vector<unsigned char> &rTotalID, vector<unsigned char> &rMetricID,
					   vector<short> &rLevels, vector<int> &rKeyTable, bool ibEmptyMetrics = false);
int hSkipCellData(DSSBinaryDataReader *ipReadStream, int iMetricsID, int iSubtotalID,
						 vector<int> &irMetricDataType, vector<vector<int> > &irTotalDatatype, int &orCellSize);
int hLoadXTabInfo(DSSBinaryDataReader *ipReadStream, DSSDataModelXTabImpl *ipModel, LayoutInfo * ipLayout, vector<vector<int> > &irTotalDatatype);
int hSetMetrics(DSSDataModelXTabImpl *ipXTabModel, int metricUnitID, LayoutInfo *ipLayout);
int hSetRuntimeUnit(DSSDataModelXTabImpl *ipModel, LayoutInfo * ipLayout);

int hLoadRawDataColumn(DSSBinaryDataReader *ipReadStream, DSSDataColumn *ipColumn, EnumDSSDataType iDataType, int nElements, vector<int> &irvElementList);
int hLoadStringDataColumn(DSSBinaryDataReader *ipReadStream, DSSDataColumn *ipColumn, EnumDSSDataType iDataType, int nElements, vector<int> &irvElementList);
int hBuildCellIndex(LayoutInfo *ipLayout, DSSDataModelXTabImpl *ipXTabModel, vector<char> &irvbXTabNull, vector<DSSTabularIndex*> &orCellIndices);
int hCreateDataSlices(DSSDataModelXTabImpl *ipXTabModel, vector<DSSTabularIndex*> &orCellIndices, vector<int> &irMetricDataType, vector<vector<int> > &irTotalDatatype, bool ibHasXTabNull = false);

//load control model
int hLoadControlElementList(DSSBinaryDataReader *ipReadStream, DSSDataModelControlImpl *ipModel);

//load field group model
int hLoadFields(DSSBinaryDataReader *ipReadStream, DSSDataModelFieldGroupImpl *ipModel, vector<int> &irGBPath, bool ibHasStaticFieldLookup);

//load groupby model
int hLoadGroupbyElement(DSSBinaryDataReader *ipReadStream, GroupbyElement *ipElement);
int hLoadTextAndSpecials(DSSBinaryDataReader *ipReadStream, DSSDataModelGroupbyImpl *ipModel);

//new load slice
int hLoadSlicedView2(DSSBinaryDataReader *ipReadStream, DSSDataModelXTabImpl *ipModel, LayoutInfo * ipLayout,vector<int> &irMetricDataType, vector<vector<int> > &irTotalDatatype, int* ipvSliceOffset, int inSliceOffset);

/*int hLoadRefColumn(DSSBinaryDataReader *ipReadStream, DSSDataColumn* &ipColumn);
int hLoadGlobalLookUpTable(DSSBinaryDataReader *ipReadStream, DSSTabularAttribute** ippTabAttr);
int hLoadGlobalLookUpTables(DSSBinaryDataReader *ipReadStream);*/

int hScanSlices(DSSBinaryDataReader *ipReadStream, DSSDataModelXTabImpl *ipModel, LayoutInfo * ipLayout, vector<int> &irMetricDataType, vector<vector<int> > &irTotalDatatype, int nSlices, vector<vector<int> > &orHeaderCount, int** oppStreamOffset, int* opnStreamOffset);

int hCountRows(DSSBinaryDataReader *ipReadStream, int iAxis, int nTemplateUnits, int &rnRows);

int hLoadHeaderKeys(DSSBinaryDataReader *ipReadStream, DSSDataModelXTabImpl *ipModel, LayoutInfo * ipLayout,
						   int nSlices, vector<vector<int> > &orHeaderCount, int* opStreamOffset, int onStreamOffset);

int hLoadHeader(DSSBinaryDataReader *ipReadStream, int iAxis, LayoutInfo *ipLayoutInfo, int *ipGBPath, int nGBUnits, int nRowsRead, bool ibIsMetricEmpty);

int hCreateXTabHeader(LayoutInfo *ipLayout, DSSDataModelXTabImpl *ipXTabModel, vector<int>& ivRowRange, bool ibHasDummy = false);

void hSortHeader(vector<int> &irvSortedRows, int lnRows, LayoutInfo *ipLayout);

void hSelectDistinctRowsAndLevels(LayoutInfo *ipLayout, vector<int>& irvRowRange, vector<int> &irvSortedRows, vector<int> &orvDistinctRows, vector<int> &orvLevelRange, bool ibHasDummy = false);

int hCreateIndex(LayoutInfo *ipLayout, DSSDataModelXTabImpl *ipXTabModel, int nLevels, vector<vector<int> > &irvLevelUnits, vector<int> &irvLevelRange, vector<int> &irvDistinctRows, vector<DSSTabularIndex *> &orvIndices, bool ibLoadbySlice = false);

int hLoadStaticFields(DSSBinaryDataReader *mpReadStream, DSSDataModelFieldGroupImpl *ipFieldGroupModel);

int hLoadCells(DSSBinaryDataReader *ipReadStream, EnumDSSDataType iDataType, vector<unsigned char> &orBuffer, int &orSizeInByte, int &orCellFmtType, int &orCellFmtID);
size_t hSizeOf(EnumDSSDataType iDataType);
int hSetMetricCellByOffset(DSSBinaryDataReader *ipReadStream, DSSXTabView *ipXTabView, int iRowLevel, int iColLevel, int iMetricID, long long offset, vector<unsigned char> &rvBuffer, EnumDSSDataFlag iCellFlags, int iThresholdID);

int hLoadCalculationPlan(DSSBinaryDataReader *ipReadStream, DSSDataModelXTabImpl *ipModel);

int hLoadInputControlModel(DSSBinaryDataReader *ipReadStream, DSSDataModelControlImpl **oppContainerModel);
//load slice for new binary data structure
int hLoadSlicedView3(DSSBinaryDataReader *ipReadStream, DSSDataModelXTabImpl *ipModel, LayoutInfo * ipLayout,  vector<int> &irMetricDataType, vector<vector<int> > &irTotalDatatype, int* ipvSliceOffset, int inSliceOffset,int lViewEndOffset, int lLevelInfoOffset, bool ibTransactionEnabled);

int hLoadSliceMap(DSSBinaryDataReader *mpReadStream, DSSDataModelXTabImpl* ipMOdel, int nSlices, vector<int*> &orvList, vector<int> &ovnList, vector<int*> &orvOffset, vector<int> &ovnOffset);

int hCreateXTabHeader2(LayoutInfo *ipLayout, DSSDataModelXTabImpl *ipXTabModel, int* ipRowList, int nRowlist, int* ipRowWindowOffset, int nRowWindowOffset, int ibIsSorted, bool ibHasDummy = false);

int hLoadHeaderKeys2(DSSBinaryDataReader *mpReadStream, DSSDataModelXTabImpl *ipModel, LayoutInfo * ipLayout,vector<int *> &rvList, vector<int> &rvnList, vector<int *> &rvOffset, vector<int> &rvnOffset, int inGBs, int& obSorted);

int hLoadHeader2(DSSBinaryDataReader *mpReadStream, int iAxis, LayoutInfo *ipLayoutInfo, int *ipGBPath, int nGBUnits, int nGBPaths, int*& rvRowList, int& rnRowList, int*& rvRowOffset, int& rnRowOffset, bool ibIsMetricEmpty);

void hSelectDistinctRowsAndLevels2(LayoutInfo *ipLayout,  int* ipRowlist, int nRowlist, int* ipRowWindowOffset, int nRowWindowOffset, int inRows, vector<int> &orvDistinctRows, vector<int> &orvLevelRange, bool ibHasDummy = false,  bool iFullGrid = false);

int hLoadHeaderLevels(DSSBinaryDataReader *mpReadStream, int iAxis, LayoutInfo *ipLayoutInfo, int *ipGBPath, int nGBUnits, int nGBPaths, int*& rvRowList, int& rnRowList, int*& rvRowOffset, int& rnRowOffset, vector<bool> rvFlag, bool ibIsMetricEmpty, bool ibHasDummyUnit = false);

int hLoadHeaderKeys3(DSSBinaryDataReader *mpReadStream, DSSDataModelXTabImpl *ipModel, int** pGBPaths, int* pGBKeySize, char** pGBPage, int* pGBPageSize, int* pPagePosInFile, LayoutInfo * ipLayout,vector<int*> &rvList, vector<int> &rvnList, vector<int*> &rvOffset, vector<int> &rvnOffset, vector<int>& ivHeaderSize, int inGBs, int& obSorted,int iLevelInfoOffset = -1, bool ibHasDummyUnit = false);


int hCreateXTabHeader3(LayoutInfo *ipLayout, DSSDataModelXTabImpl *ipXTabModel);

int hCreateXTabHeader4(DSSBinaryDataReader* ipStream, LayoutInfo *ipLayout, DSSDataModelXTabImpl *ipXTabModel,int*& rvRowList, int& rnRowList, int*& rvRowOffset, int& rnRowOffset, int headerSize);

int hLoadFields2(DSSBinaryDataReader* ipReadStream, DSSDataModelFieldGroupImpl* lpFieldGroupModel, int* lpOffset, int lnOffset, int lFieldDataEndOffset,vector<int>& lvGBPaths, bool lbHasStaticField, bool ibTransactionEnabled);

int hLoadGBHeader(DSSBinaryDataReader *mpReadStream, DSSDataModelXTabImpl* ipModel, int iAxis, LayoutInfo *ipLayoutInfo, int nGBPaths, int **oppGBPath, int* opGBKeySize, char** oppGBPageStart, int* opGBPageSize,int* opGBPagePosInFile);

int hCheckXtabNulls(DSSDataModelXTabImpl *ipXTabModel, int inSlices, bool& obHasXTabNull);

DSSModelPopulation::DSSModelPopulation(DSSBinaryDataReader * ipReadStream):mpReadStream(ipReadStream), mmbDelayLoading(false)
{
}

DSSModelPopulation::~DSSModelPopulation()
{}

int DSSModelPopulation::Init(DSSDocumentInstance *ipDI, const char* ipBinaryFilePath, std::string iSubFolder)
{
	if (ipBinaryFilePath)
	{
		mDataSourcePath = ipBinaryFilePath;
	}
	else
	{
	int hr = DataSourceUtil::getLocalPath(mDataSourcePath);
	CHECK_HR;

	hr = DataSourceUtil::createFolder(mDataSourcePath);
	CHECK_HR;
        
        std::string lSubFolder = mDataSourcePath + "/" + iSubFolder;
        hr = DataSourceUtil::createFolder(lSubFolder);
        CHECK_HR;
	}
	mDocumentID = iSubFolder;

	return S_OK;
}

int DSSModelPopulation::LoadGlobalLookup()
{
	return E_NOTIMPL;
}

int DSSModelPopulation::LoadModelFrombinary(DSSDataModelBase **oppModel)
{

/*	DSSViewDataSet *lpViewDataSet = new DSSViewDataSet();
	DSSNewModelPopulation *lpLoader = new DSSNewModelPopulation(mpReadStream);
	lpLoader->LoadModelDataSetFrombinary(&lpViewDataSet);
    return S_OK;
*/
	int hr = S_OK;
	EnumBinaryObjectType lModelType = EnumObjectReserved;
	hr = mpReadStream->ReadInt((int*)(&lModelType));
	CHECK_HR;

	int lStartPosition = mpReadStream->GetPosition();
	int lBlockType = 0;
	switch (lModelType) {
		case EnumObjectXTabModel:
		{
			int lBlockType = 0;
			hr = mpReadStream->BeginReadBlock(&lBlockType);
			CHECK_HR_BLOCK(EnumDataBlockXTabModelV2);

			hr = LoadXTabModel(mpReadStream, oppModel, true);
			CHECK_HR;

			hr = mpReadStream->EndReadBlock();
			CHECK_HR;
		}
			break;
		case EnumObjectControlNodeModel:
		{
			hr = mpReadStream->BeginReadBlock(&lBlockType);
			CHECK_HR_BLOCK(EnumDataBlockControlNodeModelV1);

			hr = LoadControlModel(mpReadStream, oppModel, true);
			CHECK_HR;

			hr = mpReadStream->EndReadBlock();
			CHECK_HR;
		}
			break;
		case EnumObjectFieldGroupModel:
		{
			hr = mpReadStream->BeginReadBlock(&lBlockType);
			CHECK_HR_BLOCK(EnumDataBlockFieldGroupModelV1);

			hr = LoadFieldGroupModel(mpReadStream, oppModel,true);
			CHECK_HR;

			hr = mpReadStream->EndReadBlock();
			CHECK_HR;
		}
			break;
		case EnumObjectGroupbyModel:
		{
			hr = mpReadStream->BeginReadBlock(&lBlockType);
			CHECK_HR_BLOCK(EnumDataBlockGroupbyModelV1);

			hr = LoadGroupbyModel(mpReadStream, oppModel);
			CHECK_HR;

			hr = mpReadStream->EndReadBlock();
			CHECK_HR;//end of groupby model
		}
			break;
		default:
		{
			hr = mpReadStream->BeginReadBlock(&lBlockType);
			CHECK_HR;
			hr = mpReadStream->EndReadBlock();
			CHECK_HR;
		}
			break;
	}

	int lEndPosition = mpReadStream->GetPosition();;
	if (oppModel && *oppModel)
	{
		(*oppModel)->setDataSourcePath(mDataSourcePath);

		MBase::CString lLocalPath;
		int hr = DataSourceUtil::getLocalPath(lLocalPath);
		CHECK_HR;

		DSSDataSource *lpDS = new DSSDataSource();
		if (lLocalPath.compare(mDataSourcePath) != 0)
		{
			hr = lpDS->Init(mpReadStream, lStartPosition, lEndPosition, mDataSourcePath, false);
			CHECK_HR;
		}
		else
		{
			//we need create local copy of binary data
			MBase::CString lPath = lLocalPath + "/" + mDocumentID + "/m_" + (*oppModel)->getNodeKey();
			hr = lpDS->Init(mpReadStream, lStartPosition, lEndPosition, lPath, true);
			CHECK_HR;
		}

		(*oppModel)->setSubFolder(mDocumentID);
		(*oppModel)->setModelDataSource(lpDS);
	}

	//printf("size of model:= %d\n", (*oppModel)->size());
	//TODO: expection handling, log error messages
	return hr;
}

int DSSModelPopulation::LoadXTabModel(DSSBinaryDataReader *mpReadStream, DSSDataModelBase **oppModel, bool bIsInitialLoading)
{
	int hr = S_OK;
	int lBlockType = 0;

	if (bIsInitialLoading)
	{
		//read the node key
		MBase::CString lNodeKey;
		hr = mpReadStream->ReadString(lNodeKey);
		CHECK_HR;

		//create XTabModel
		DSSDataModelXTabImpl *lpXTabModel = new DSSDataModelXTabImpl();;
		MBase::DSSStrongPtr<DSSDataModelXTabImpl> lModelPtr(lpXTabModel);

		hr = lpXTabModel->Init();
		CHECK_HR;

		hr = lpXTabModel->setNodeKey(lNodeKey);
		CHECK_HR;

		if (oppModel)
		{
			*oppModel = lpXTabModel;
			lModelPtr.GiveUp();
		}
	}
	else
	{
		DSSDataModelXTabImpl *lpXTabModel = static_cast<DSSDataModelXTabImpl*> (*oppModel);
		CHECK_PTR(lpXTabModel);
        
        //read the node key
		MBase::CString lNodeKey;
		hr = mpReadStream->ReadString(lNodeKey);
		CHECK_HR;

        DSSRWTemplateNode* lpNode = NULL;
        bool lbTransactionEnabled = false;
        DSSObjectContext* lpObjectContext = lpXTabModel->getObjectContext();
        if(lpObjectContext)
        {
            lpNode = (DSSRWTemplateNode*)(lpObjectContext->getNode(lNodeKey));
            if(lpNode && lpNode->IsTransactionEnabled())
                lbTransactionEnabled = true;
        }
        
#ifndef __ANDROID__       
        //TODO: CHECK SECURITY MODE
        if(DataSourceUtil::isMappedFileSupported())
        {
            if(!lbTransactionEnabled && mpReadStream->IsLittleEndian())
            {
                std::string lFilePath = mpReadStream->getFilePath();
                lpXTabModel->setFilePath(lFilePath);
            }
        }
#endif

		//load xtab related info, i.e. null display, layout, subtotal
		LayoutInfo lLayoutInfos[3];
		vector<vector<int> > lvTotalDataTypes;
		hr = hLoadXTabInfo(mpReadStream, lpXTabModel, &lLayoutInfos[0], lvTotalDataTypes);
		CHECK_HR;

		//load unit lookup table
		vector<int> lvMetricDataTypes;
		int lMetricUnitID = -1;
		hr = mpReadStream->BeginReadBlock(&lBlockType);
		CHECK_HR_BLOCK(EnumDataBlockXTabModelUnitsV1);
		{
			int lDummyUnit = -1;
			hr = hLoadUnits(mpReadStream, lpXTabModel, &lvMetricDataTypes, &lMetricUnitID, &lDummyUnit);
			CHECK_HR;
			
			if(lDummyUnit >= 0)
			{
				hr = lpXTabModel->setDummyUnit(lDummyUnit);
				CHECK_HR;
			}
		}
		hr = mpReadStream->EndReadBlock();
		CHECK_HR;

		hr = hSetMetrics(lpXTabModel, lMetricUnitID, &lLayoutInfos[0]);
		CHECK_HR;

		//429191, put runtime unit to the opposite axis of metrics unit
		hr = hSetRuntimeUnit(lpXTabModel, &lLayoutInfos[0]);
		CHECK_HR;

		int lViewOffset = mpReadStream->GetPosition();		//remember this position of view slices block

        int lXtabViewVer = 0;
		mpReadStream->BeginReadBlock(&lBlockType);		//skip view slices block and read it later.

        if(lBlockType == EnumDataBlockXTabModelViewsV1)
            lXtabViewVer = 1;
        else if(lBlockType == EnumDataBlockXTabModelViewsV2)
            lXtabViewVer = 2;
        else
            return E_DATABLOCK;
		//CHECK_HR_BLOCK(EnumDataBlockXTabModelViewsV1);
		//CHECK_HR_BLOCK(EnumDataBlockXTabModelViewsV2);
		hr = mpReadStream->EndReadBlock();
		CHECK_HR;

		int lViewEndOffset = mpReadStream->GetPosition();
		
		//load cell format
		hr=mpReadStream->BeginReadBlock(&lBlockType);
		CHECK_HR_BLOCK(EnumDataBlockXTabModelCellFormatsV1);
		{
			int lLen=0;
			hr=mpReadStream->ReadInt(&lLen);
			CHECK_HR;

			vector<MBase::CString> lCellFmtIDs(lLen);	//410821
			for (int i=0; i<lLen; i++) {
				hr = mpReadStream->ReadString(lCellFmtIDs[i]);
				CHECK_HR;
			}
			hr=lpXTabModel->setCellFmtIDs(lCellFmtIDs);
			CHECK_HR;

		}
		hr = mpReadStream->EndReadBlock();
		CHECK_HR;

        int lLevelInfoOffset = -1;
		int* lpSliceOffset = NULL;
        int lnSliceOffset = 0;
		if (mpReadStream->MoreDataInBlock())
		{
            hr = mpReadStream->BeginReadBlock(&lBlockType);
			CHECK_HR_BLOCK(EnumDataBlockXTabModelSliceInfoV1);
			{
                hr = mpReadStream->ReadInt(&lnSliceOffset);
                CHECK_HR;
                
                lpSliceOffset = new int[lnSliceOffset + 2];
                
				hr = mpReadStream->ReadIntArray(lpSliceOffset, lnSliceOffset);
				CHECK_HR;
			}
            //the subtotal info needed for header creation
            if(mpReadStream->MoreDataInBlock())
            {	
                hr = mpReadStream->BeginReadBlock(&lBlockType);
                if(lBlockType == EnumDataBlockXTabModelLevelInfoV1)
                {
                    lLevelInfoOffset = mpReadStream->GetPosition();
                }
                mpReadStream->EndReadBlock();
                CHECK_HR;
            }
			hr = mpReadStream->EndReadBlock();
			CHECK_HR;
		}

		if (mpReadStream->MoreDataInBlock())
		{
			hr = mpReadStream->BeginReadBlock(&lBlockType);
			CHECK_HR_BLOCK(EnumDataBlockXTabModelCalculationPlanV1);
			{
				hLoadCalculationPlan(mpReadStream, lpXTabModel);
                
				//swallow the error
			}
			hr = mpReadStream->EndReadBlock();
			CHECK_HR;
            
            if (mpReadStream->MoreDataInBlock())
            {
                hr = mpReadStream->BeginReadBlock(&lBlockType);
                CHECK_HR_BLOCK(EnumDataBlockInputControlModelV1);
                {
                    int lnControlModels = 0;
                    hr = mpReadStream->ReadInt(&lnControlModels);
                    CHECK_HR;
                    
                    for(int i = 0; i < lnControlModels; i++)
                    {
                        EnumBinaryObjectType lModelType = EnumObjectReserved;
                        hr = mpReadStream->ReadInt((int*)(&lModelType));
                        CHECK_HR;
                        
                        AE_ASSERT(lModelType == EnumObjectControlNodeModel);
                        
                        hr = mpReadStream->BeginReadBlock(&lBlockType);
                        CHECK_HR_BLOCK(EnumDataBlockControlNodeModelV1);
                        
                        DSSDataModelControlImpl* lpControlModel = NULL;
                        hLoadInputControlModel(mpReadStream, &lpControlModel);
                        
                        //swallow the error

                        if(lpControlModel)
                        {
                            hr = lpControlModel->setParentNodeKey(lNodeKey);
                            CHECK_HR;
                            
                            lpXTabModel->addControlModel(lpControlModel);
                            DSSRWControl* lpControl = lpObjectContext->getControl(lpControlModel->getNodeKey());
                            if(lpControl)
                            {
                                lpControl->setControlNodeModel(lpControlModel);
                            }
                        }
                        
                        hr = mpReadStream->EndReadBlock();
                        CHECK_HR;
                    }
                }
                hr = mpReadStream->EndReadBlock();
                CHECK_HR;
            }
		}

		//reset stream position to view slices block
		hr = mpReadStream->SetPosition(lViewOffset);
		CHECK_HR;
//		hr = hLoadSlicedView(lpXTabModel, &lLayoutInfos[0], lvMetricDataTypes, lvTotalDataTypes);
        if(lXtabViewVer == 1)
            hr = hLoadSlicedView2(mpReadStream, lpXTabModel, &lLayoutInfos[0], lvMetricDataTypes, lvTotalDataTypes, lpSliceOffset, lnSliceOffset);
        else if(lXtabViewVer == 2)
            hr = hLoadSlicedView3(mpReadStream, lpXTabModel, &lLayoutInfos[0], lvMetricDataTypes, lvTotalDataTypes, lpSliceOffset, lnSliceOffset,lViewEndOffset, lLevelInfoOffset, lbTransactionEnabled);
        else
            return E_DATABLOCK;

		if (hr != S_OK && hr != S_FALSE)
			return hr;

		DSSCalculationPlan *lpCalcPlan = lpXTabModel->getCalculationPlan();
		hr = lpCalcPlan->Process();
		CHECK_HR;

		lpXTabModel->setStatus(XTAB_INITIALIZED);

		//409716: temporary fix
		if (hr == S_FALSE)
			return S_OK;
	}
	return S_OK;
}


int DSSModelPopulation::LoadControlModel(DSSBinaryDataReader *mpReadStream, DSSDataModelBase **oppModel, bool bIsInitialLoading)
{
	int hr = S_OK;
	int lBlockType = 0;

	if(bIsInitialLoading)
	{
		//read the node key
		MBase::CString lNodeKey;
		hr = mpReadStream->ReadString(lNodeKey);
		CHECK_HR;

		//create XTabModel
		DSSDataModelControlImpl *lpControlModel = new DSSDataModelControlImpl();;
		MBase::DSSStrongPtr<DSSDataModelControlImpl> lModelHolder(lpControlModel);

		hr = lpControlModel->setNodeKey(lNodeKey);
		CHECK_HR;

		hr = lpControlModel->Init();
		CHECK_HR;


		if (oppModel)
		{
			*oppModel = lpControlModel;
			lModelHolder.GiveUp();
		}
	}
	else
	{
		/*
		//read the node key
		MBase::CString lNodeKey;
		hr = mpReadStream->ReadString(lNodeKey);
		CHECK_HR;

		//create XTabModel
		DSSDataModelControlImpl *lpControlModel = new DSSDataModelControlImpl();;
		MBase::DSSStrongPtr<DSSDataModelControlImpl> lModelHolder(lpControlModel);

		hr = lpControlModel->setNodeKey(lNodeKey);
		CHECK_HR;
		//*/
		DSSDataModelControlImpl *lpControlModel = static_cast<DSSDataModelControlImpl*> (*oppModel);
		CHECK_PTR(lpControlModel);

		//read the node key
		MBase::CString lNodeKey;
		hr = mpReadStream->ReadString(lNodeKey);
		CHECK_HR;
		//*/

		//load unit lookup table
		hr = mpReadStream->BeginReadBlock(&lBlockType);
		CHECK_HR_BLOCK(EnumDataBlockControlNodeModelUnitsV2)
		{
			hr = hLoadUnits(mpReadStream, lpControlModel);
			CHECK_HR;
		}
		hr = mpReadStream->EndReadBlock();
		CHECK_HR;

		if (lpControlModel->Count() == 0)		//empty control model
		{
			//return mpReadStream->EndReadBlock();	//jump to the end of EnumDataBlockControlNodeModelV1
		}

		//load control element list
		hr = mpReadStream->BeginReadBlock(&lBlockType);
		CHECK_HR_BLOCK(EnumDataBlockControlNodeModelElementsV1);
		{
			hr = hLoadControlElementList(mpReadStream, lpControlModel);
			CHECK_HR;
		}
		hr = mpReadStream->EndReadBlock();
		CHECK_HR;

		//load group-by path
		hr = mpReadStream->BeginReadBlock(&lBlockType);
		CHECK_HR_BLOCK(EnumDataBlockControlNodeModelRowsV1);
		{
			vector<int> lvGBPath;
			hr = mpReadStream->ReadIntArray(lvGBPath);
			CHECK_HR;

			hr = lpControlModel->CreateViewMap(lvGBPath);
			CHECK_HR;
		}
		hr = mpReadStream->EndReadBlock();
		CHECK_HR;

		//load flag
		hr = mpReadStream->BeginReadBlock(&lBlockType);
		CHECK_HR_BLOCK(EnumDataBlockControlNodeModelFlagV1);
		{
			int lFlag = 0;
			hr = mpReadStream->ReadInt(&lFlag);
			CHECK_HR;

			lpControlModel->setCGBFlag(lFlag);
		}
		hr = mpReadStream->EndReadBlock();
		CHECK_HR;

		if (mpReadStream->MoreDataInBlock())
		{
			hr = mpReadStream->BeginReadBlock(&lBlockType);
			CHECK_HR_BLOCK(EnumDataBlockControlNodeModelRangeV1);
			{
				int lnProperties = 0;
				hr = mpReadStream->ReadInt(&lnProperties);
				CHECK_HR;

				for (int i = 0; i < lnProperties; i++)
				{
					CComVariant lVal;
					hr = ReadVariant(mpReadStream, lVal);
					CHECK_HR;

					hr = lpControlModel->setMetricRange(i, &lVal);
					CHECK_HR;
				}
			}
			hr = mpReadStream->EndReadBlock();
			CHECK_HR;
		}

		lpControlModel->setStatus(XTAB_INITIALIZED);

		//409716: temporary fix
		if (hr == S_FALSE)
			return S_OK;
	}
	return S_OK;
}

int DSSModelPopulation::LoadGroupbyModel(DSSBinaryDataReader *mpReadStream, DSSDataModelBase **oppModel)
{
	int hr = S_OK;
	int lBlockType = 0;
	{
		//read the node key
		MBase::CString lNodeKey;
		hr = mpReadStream->ReadString(lNodeKey);
		CHECK_HR;

		//create GroupbyModel
		DSSDataModelGroupbyImpl *lpGroupbyModel = new DSSDataModelGroupbyImpl();;
		MBase::DSSStrongPtr<DSSDataModelGroupbyImpl> lModelHolder(lpGroupbyModel);

		hr = lpGroupbyModel->setNodeKey(lNodeKey);
		CHECK_HR;

		//load unit lookup table
		hr = mpReadStream->BeginReadBlock(&lBlockType);
		CHECK_HR_BLOCK(EnumDataBlockGroupbyModelUnitsV1)
		{
			hr = hLoadUnits(mpReadStream, lpGroupbyModel);
			CHECK_HR;
		}
		hr = mpReadStream->EndReadBlock();
		CHECK_HR;//end of groupby units

		//load text of units and special elements
		hr = mpReadStream->BeginReadBlock(&lBlockType);
		CHECK_HR_BLOCK(EnumDataBlockGroupbyModelTextsV1)
		{
			hr = hLoadTextAndSpecials(mpReadStream, lpGroupbyModel);
			CHECK_HR;
		}
		hr = mpReadStream->EndReadBlock();
		CHECK_HR;// end of groupby element texts

		//load groupby element list
		hr = mpReadStream->BeginReadBlock(&lBlockType);
		CHECK_HR_BLOCK(EnumDataBlockGroupbyModelElementsV1);
		{
			GroupbyElement	*lpRootElement=new GroupbyElement;
			hr = hLoadGroupbyElement(mpReadStream, lpRootElement);
			CHECK_HR;
			lpGroupbyModel->SetRootElement(lpRootElement);
		}
		//uncomment for test
		//lpGroupbyModel->toString();
		hr = mpReadStream->EndReadBlock();
		CHECK_HR;// end of groupby elements

		lpGroupbyModel->setStatus(XTAB_INITIALIZED);

		if (oppModel)
		{
			*oppModel = lpGroupbyModel;
			lModelHolder.GiveUp();
		}
	}

	return S_OK;

}


int hLoadGroupbyElement(DSSBinaryDataReader *mpReadStream, GroupbyElement * ipElement)
{
	int hr = S_OK;
	int index = 0;
	hr= mpReadStream->ReadInt(&index);
	CHECK_HR;
	ipElement->index=index;// the top root will have the length of the integer array instead of index
    ipElement->flag = -2;//-2 means uninitialize
	int lnElements = 0;
	hr = mpReadStream->ReadInt(&lnElements);
	CHECK_HR;

	ipElement->children.resize(lnElements);

	for (int i = 0; i < lnElements; i++)
	{
		GroupbyElement *lpElement=&(ipElement->children[i]);
        lpElement->parent = ipElement;
		hr=hLoadGroupbyElement(mpReadStream, lpElement);
		CHECK_HR;
	}

	return S_OK;
}

int hLoadTextAndSpecials(DSSBinaryDataReader *mpReadStream, DSSDataModelGroupbyImpl *ipModel)
{
	int hr = S_OK;
	//text of each element in each unit

	for (int i=0; i<ipModel->Count(); i++) {
		int lnElements=0;
		hr=mpReadStream->ReadInt(&lnElements);
		CHECK_HR;
		vector<MBase::CString> lvDisplayTexts;
		for (int j=0; j<lnElements; j++) {
			MBase::CString lDisplayText;
			hr=mpReadStream->ReadString(lDisplayText);
			CHECK_HR;
			lvDisplayTexts.push_back(lDisplayText);
		}

		hr=ipModel->SetTexts(i,lvDisplayTexts);
		CHECK_HR;
	}

	//special elements such as total
	int lnSpecials=0;
	hr=mpReadStream->ReadInt(&lnSpecials);
	CHECK_HR;
	ipModel->SetSpecialCount(lnSpecials);

	for (int i=0; i<lnSpecials; i++) {
		MBase::CString lDisplayText;
		MBase::CString lElementID;
		hr=mpReadStream->ReadString(lDisplayText);
		CHECK_HR;
		hr=mpReadStream->ReadString(lElementID);
		CHECK_HR;
		hr=ipModel->SetSpeical(i,lDisplayText, lElementID);
		CHECK_HR;
	}
	return S_OK;
}
int DSSModelPopulation::LoadFieldGroupModel(DSSBinaryDataReader *mpReadStream, DSSDataModelBase **oppModel, bool bIsInitialLoading)
{
	int hr = S_OK;
	int lBlockType = 0;
	if(bIsInitialLoading)
	{
		//read the node key
		MBase::CString lNodeKey;
		hr = mpReadStream->ReadString(lNodeKey);
		CHECK_HR;

		//create XTabModel
		DSSDataModelFieldGroupImpl *lpFieldGroupModel = new DSSDataModelFieldGroupImpl();;
		MBase::DSSStrongPtr<DSSDataModelFieldGroupImpl> lModelHolder(lpFieldGroupModel);

		hr = lpFieldGroupModel->setNodeKey(lNodeKey);
		CHECK_HR;

		hr = lpFieldGroupModel->Init();
		CHECK_HR;

		if (oppModel)
		{
			*oppModel = lpFieldGroupModel;
			lModelHolder.GiveUp();
		}

	}else
	{
		/*
		//read the node key
		MBase::CString lNodeKey;
		hr = mpReadStream->ReadString(lNodeKey);
		CHECK_HR;

		//create XTabModel
		DSSDataModelFieldGroupImpl *lpFieldGroupModel = new DSSDataModelFieldGroupImpl();;
		MBase::DSSStrongPtr<DSSDataModelFieldGroupImpl> lModelHolder(lpFieldGroupModel);

		hr = lpFieldGroupModel->setNodeKey(lNodeKey);
		CHECK_HR;
		*/
		DSSDataModelFieldGroupImpl *lpFieldGroupModel = static_cast<DSSDataModelFieldGroupImpl*> (*oppModel);
		CHECK_PTR(lpFieldGroupModel);

		//read the node key
		MBase::CString lNodeKey;
		hr = mpReadStream->ReadString(lNodeKey);
		CHECK_HR;

        DSSRWFieldGroupNode* lpNode = NULL;
        bool lbTransactionEnabled = false;
        DSSObjectContext* lpObjectContext = lpFieldGroupModel->getObjectContext();
        if(lpObjectContext)
        {
            lpNode = (DSSRWFieldGroupNode*)(lpObjectContext->getNode(lNodeKey));
            //515972, also disable on demand loading for subsections
            if( lpNode->getType() != DssRWNodeFieldGroup || lpNode->IsTransactionEnabled())
                lbTransactionEnabled = true;
        }
        
		//load unit lookup table
		hr = mpReadStream->BeginReadBlock(&lBlockType);
		CHECK_HR_BLOCK(EnumDataBlockFieldGroupModelUnitsV1);
		{
			hr = hLoadUnits(mpReadStream, lpFieldGroupModel);
			CHECK_HR;
		}
		hr = mpReadStream->EndReadBlock();
		CHECK_HR;

		bool lbHasStaticField = false;
        int lFieldBlockStartOffset = mpReadStream->GetPosition();
		mpReadStream->BeginReadBlock(&lBlockType);

		if (lBlockType == EnumDataBlockFieldGroupModelStaticFieldsV1)
		{
			hr = hLoadStaticFields(mpReadStream, lpFieldGroupModel);
			CHECK_HR;

			hr = mpReadStream->EndReadBlock();
			CHECK_HR;

			lbHasStaticField = true;
            
			lFieldBlockStartOffset = mpReadStream->GetPosition();
			mpReadStream->BeginReadBlock(&lBlockType);
 
		}
		
        if(lBlockType == EnumDataBlockFieldGroupModelFieldsV1)
		{
			vector<int> lvGBPaths;
			hr = hLoadFields(mpReadStream, lpFieldGroupModel, lvGBPaths, lbHasStaticField);
			CHECK_HR;

			hr = lpFieldGroupModel->CreateViewMap(lvGBPaths);
			CHECK_HR;
		}
        else if(lBlockType == EnumDataBlockFieldGroupModelFieldsV2)
        {
            int lFieldDataStartOffset = mpReadStream->GetPosition();
            hr = mpReadStream->EndReadBlock();
            CHECK_HR;
            int lFieldDataEndOffset =  mpReadStream->GetPosition();
            hr = mpReadStream->BeginReadBlock(&lBlockType);
            CHECK_HR;
            
            int* ipOffset = NULL;
            int inOffset = 0;
            // int lEndOffset = -1;
            CHECK_HR_BLOCK(EnumDataBlockFieldGroupModelFieldsOffsetV1);
            {
                hr = mpReadStream->ReadInt(&inOffset);
                CHECK_HR;
                
                if(inOffset > 0)
                {
                    ipOffset = new int[inOffset + 2];
                    hr = mpReadStream->ReadIntArray(ipOffset,inOffset);
                    CHECK_HR;
                }
                
                hr = mpReadStream->EndReadBlock();
                CHECK_HR;
                
                // lEndOffset = mpReadStream->GetPosition();
            }
            
            hr = mpReadStream->SetPosition(lFieldBlockStartOffset);
            CHECK_HR;
            
            hr = mpReadStream->BeginReadBlock(&lBlockType);
            CHECK_HR;
            
            hr = mpReadStream->SetPosition(lFieldDataStartOffset);
            CHECK_HR;
            
            vector<int> lvGBPaths;
			hr = hLoadFields2(mpReadStream, lpFieldGroupModel, ipOffset, inOffset, lFieldDataEndOffset, lvGBPaths, lbHasStaticField, lbTransactionEnabled);
			CHECK_HR;
            
			hr = lpFieldGroupModel->CreateViewMap2(lvGBPaths);
			CHECK_HR;
            
            hr = mpReadStream->EndReadBlock();
            CHECK_HR;
            
            hr = mpReadStream->BeginReadBlock(&lBlockType);
            CHECK_HR;
        }
		hr = mpReadStream->EndReadBlock();
		CHECK_HR;

		if (mpReadStream->MoreDataInBlock())
		{
			hr = mpReadStream->BeginReadBlock(&lBlockType);
			CHECK_HR_BLOCK(EnumDataBlockFieldGroupModelRawDataV1);
			{
				BYTE lbIsTransactionEnabled = 0;
				hr = mpReadStream->ReadByte(&lbIsTransactionEnabled);
				CHECK_HR;

				if (lbIsTransactionEnabled != 0)
				{
					vector<char> lvIsEditableFlag;
					hr = mpReadStream->ReadByteArray(lvIsEditableFlag);
					CHECK_HR;

					int lSize = 0;
					hr = mpReadStream->ReadInt(&lSize);
					CHECK_HR;

					int lBufferSize = 8;
					char *lpBuffer = new char[lBufferSize];
					AERawData lRawData(lpBuffer, 0, DssDataTypeUnknown, DssDataOk, true);
					for (int i = 0; i < lSize; i++)
					{
						hr = ReadAERawData(mpReadStream, lRawData, lBufferSize);
						if (hr != S_OK && hr != S_FALSE)
							return hr;
						if (hr == S_OK)
						{
							hr = lpFieldGroupModel->AddRawData(i, lRawData);
							CHECK_HR;
						}
						else if (hr == S_FALSE && lRawData.mDataType == DssDataTypeBigDecimal)
						{
							//481565, move the reading bigdecimal logic out of ReadAERawData()
							//we can't copy mBigInt class member of the local BigDecimal object to raw data
							MBase::CString lVal;
							mpReadStream->ReadString(lVal);
                            
							//this is local variable. It should be released after raw data is copied to field group data model
							MDataType::DSSBigDecimal lBigDecimal(lVal.c_str());

							lRawData.mSizeInByte = sizeof(MDataType::DSSBigDecimal);
							
							char *lpOldBuf = lRawData.mValue;
							lRawData.mValue = (char*)&lBigDecimal;

							hr = lpFieldGroupModel->AddRawData(i, lRawData);
							CHECK_HR;

							//484992, restore the original buffer instead of NULL
							lRawData.mValue = lpOldBuf;
                            //580208 & 580340, for BigDecimal type, the mValue is reverted to the orignial buffer, 
                            //so the data type shouldn't be kept as DssDataTypeBigDecimal
                            lRawData.mDataType = DssDataTypeMissing;
						}
					}
				}
			}
			hr = mpReadStream->EndReadBlock();
			CHECK_HR;
            
            //load input control data model
            if (mpReadStream->MoreDataInBlock())
            {
                hr = mpReadStream->BeginReadBlock(&lBlockType);
                CHECK_HR_BLOCK(EnumDataBlockInputControlModelV1);
                {
                    int lnControlModels = 0;
                    hr = mpReadStream->ReadInt(&lnControlModels);
                    CHECK_HR;
                    
                    for(int i = 0; i < lnControlModels; i++)
                    {
                        EnumBinaryObjectType lModelType = EnumObjectReserved;
                        hr = mpReadStream->ReadInt((int*)(&lModelType));
                        CHECK_HR;
                        
                        AE_ASSERT(lModelType == EnumObjectControlNodeModel);
                        
                        hr = mpReadStream->BeginReadBlock(&lBlockType);
                        CHECK_HR_BLOCK(EnumDataBlockControlNodeModelV1);
                        
                        DSSDataModelControlImpl* lpControlModel = NULL;
                        hLoadInputControlModel(mpReadStream, &lpControlModel);
                        //swallow the error
                        
                        if(lpControlModel)
                        {
                            hr = lpControlModel->setParentNodeKey(lNodeKey);
                            CHECK_HR;
                            
                            lpFieldGroupModel->addControlModel(lpControlModel);
                            DSSRWControl* lpControl = lpObjectContext->getControl(lpControlModel->getNodeKey());
                            if(lpControl)
                            {
                                lpControl->setControlNodeModel(lpControlModel);
                            }
                        }
                        
                        hr = mpReadStream->EndReadBlock();
                        CHECK_HR;
                    }
                }
                hr = mpReadStream->EndReadBlock();
                CHECK_HR;
            }
		}

		lpFieldGroupModel->setStatus(XTAB_INITIALIZED);

		//409716: temporary fix
		if (hr == S_FALSE)
			return S_OK;
	}
	return S_OK;
}

int hLoadInputControlModel(DSSBinaryDataReader *ipReadStream, DSSDataModelControlImpl **oppModel)
{
    int hr = S_OK;
    
    DSSDataModelControlImpl *lpControlModel = new DSSDataModelControlImpl();
    MBase::DSSStrongPtr<DSSDataModelControlImpl> lModelHolder(lpControlModel);
    
    int lBlockType = 0;
    //read the node key
    MBase::CString lNodeKey;
    hr = ipReadStream->ReadString(lNodeKey);
    CHECK_HR;
    
    hr = lpControlModel->setNodeKey(lNodeKey);
    CHECK_HR;
    
    hr = lpControlModel->Init();
    CHECK_HR;
    
    //load unit lookup table
    hr = ipReadStream->BeginReadBlock(&lBlockType);
    CHECK_HR_BLOCK(EnumDataBlockControlNodeModelUnitsV2)
    {
        hr = hLoadUnits(ipReadStream, lpControlModel);
        CHECK_HR;
    }
    hr = ipReadStream->EndReadBlock();
    CHECK_HR;
    
    //load group-by path
    hr = ipReadStream->BeginReadBlock(&lBlockType);
    CHECK_HR_BLOCK(EnumDataBlockControlNodeModelRowsV1);
    {
        vector<int> lvGBPath;
        hr = ipReadStream->ReadIntArray(lvGBPath);
        CHECK_HR;
        
        hr = lpControlModel->CreateViewMap(lvGBPath);
        CHECK_HR;
    }
    hr = ipReadStream->EndReadBlock();
    CHECK_HR;
    
    //element list is optional for input control data model
    if (ipReadStream->MoreDataInBlock())
    {
        int lnElements = 0;
        //load control element list
        hr = ipReadStream->BeginReadBlock(&lBlockType);
        CHECK_HR_BLOCK(EnumDataBlockControlNodeModelElementsV1)
        {
            hr = ipReadStream->ReadInt(&lnElements);
            CHECK_HR;
            
            hr = lpControlModel->Init(lnElements);
            CHECK_HR;
            
            MBase::CString lElementID;
            
            for (int i = 0; i < lnElements; i++)
            {
                int lobjtype = 0;
                hr = ipReadStream->ReadInt(&lobjtype);
                CHECK_HR;
                
                if (lobjtype != EnumObjectControlElement)
                    return E_UNEXPECTED_DATATYPE;
                
                MBase::CString lDisplayText;
                hr = ipReadStream->ReadString(lDisplayText);
                CHECK_HR;
                
                hr = lpControlModel->AddElement(i, lDisplayText, lElementID);
                CHECK_HR;
            }
        }
        if(ipReadStream->MoreDataInBlock())
        {
            for(int i = 0; i < lnElements; i++)
            {
                MBase::CString lMatchText;
                hr = ipReadStream->ReadString(lMatchText);
                CHECK_HR;
                
                hr = lpControlModel->AddElementMatchText(i, lMatchText);
                CHECK_HR;
            }
            
            if(ipReadStream->MoreDataInBlock())
            {
                hr = ipReadStream->BeginReadBlock(&lBlockType);
                CHECK_HR_BLOCK(EnumDataBlockControlNodeModelElementsV2)
                for(int i = 0; i < lnElements; i++)
                {
                    int lIndex = -1;
                    hr = ipReadStream->ReadInt(&lIndex);
                    CHECK_HR;
                    
                    hr = lpControlModel->SetElementIndex(i, lIndex);
                    CHECK_HR;
                }
                hr = ipReadStream->EndReadBlock();
                CHECK_HR;
            }
            
        }
        hr = ipReadStream->EndReadBlock();
        CHECK_HR;
    }
    
    lpControlModel->setStatus(XTAB_INITIALIZED);
    
    if(oppModel)
    {
        (*oppModel) = lpControlModel;
        lModelHolder.GiveUp();
    }
    
    return S_OK;
}

int hLoadUnits(DSSBinaryDataReader *mpReadStream, DSSDataModelBase *ipModel, vector<int> * opvMetricDataTypes, int * opMetricUnitID, int *opDummyUnit)
{
	int hr = S_OK;
	int lnUnits = 0;
	hr = mpReadStream->ReadInt(&lnUnits);
	CHECK_HR;

	int i = 0;
	for (i = 0; i < lnUnits; i++)
	{
		EnumBinaryObjectType objtype = EnumObjectReserved;
		hr = mpReadStream->ReadInt((int*)(&objtype));
		CHECK_HR;

		int lUnitID = 0;
		hr = mpReadStream->ReadInt(&lUnitID);
		CHECK_HR;

		DSSTabularUnit *lpUnit = NULL;
		switch (objtype) {
			case EnumObjectAttributeUnit:
			{
				hr = hLoadUnitAttribute(mpReadStream, lUnitID, &lpUnit);
				CHECK_HR;
			}
			break;

			case EnumObjectDummyUnit:
			{
				//dummy unit should be added to layou info at last
				DSSTabularAttribute *lpAttribute = new DSSTabularAttribute();
				hr = lpAttribute->Init(lUnitID, 0);
				if (hr == S_OK)
					lpUnit = lpAttribute;
				else
					delete lpAttribute;
				
				if(opDummyUnit)
					(*opDummyUnit) = lUnitID;
				//TODO:[Dummy] distinguish the real dummy unit and CS/CG dummy
			}
			break;

			case EnumObjectMetricUnit:
			{
				hr = hLoadUnitMetrics(mpReadStream, lUnitID, &lpUnit, opvMetricDataTypes);
				CHECK_HR;
				if (opMetricUnitID)
					*opMetricUnitID = lUnitID;
			}
			break;

			case EnumObjectConsolidationUnit:
			{
				hr = hLoadUnitConsolidation(mpReadStream, lUnitID, &lpUnit);
				CHECK_HR;

				reinterpret_cast<DSSTabularConsolidation*>(lpUnit)->setParentInfo(ipModel);
			}
			break;

			case EnumObjectCustomGroupUnit:
			{
				hr = hLoadUnitCustomGroup(mpReadStream, lUnitID, &lpUnit);
				CHECK_HR;
				reinterpret_cast<DSSTabularCustomGroup*>(lpUnit)->setParentInfo(ipModel);
			}
				break;
			default:
				return E_UNEXPECTED_DATATYPE;
		}
		CHECK_PTR(lpUnit);
		hr = ipModel->AddUnit(lUnitID, lpUnit);
		CHECK_HR;
	}
	return hr;
}

int hLoadUnitAttribute(DSSBinaryDataReader *mpReadStream, int index, DSSTabularUnit **oppXTabUnit)
{
	int hr = S_OK;
	int lBlockType = 0;
	hr = mpReadStream->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockAttributeV1);
	{
		MBase::CString lguid;
		hr = mpReadStream->ReadString(lguid);
		CHECK_HR;

		int i = 0;
		bool lbIsRawData;
		hr = mpReadStream->ReadBool(&lbIsRawData);
		CHECK_HR;

		int lArity = 0;
		hr = mpReadStream->ReadInt(&lArity);
		CHECK_HR;

		DSSTabularAttribute *lpAttribute = new DSSTabularAttribute();
		MBase::DSSStrongPtr<DSSTabularAttribute> lpAttributeHolder(lpAttribute);
		hr = lpAttribute->Init(index, lArity);
		CHECK_HR;

		hr = lpAttribute->setGUID(lguid);
		CHECK_HR;

		int lnForms = 0;
		vector<int> lvBaseFormID;
		vector<int> lvDataTypes;
		vector<int> lvFormTypes;
		hr = mpReadStream->BeginReadBlock(&lBlockType);
		CHECK_HR_BLOCK(EnumDataBlockAttributeFormsV1);
		{
			hr = mpReadStream->ReadInt(&lnForms);
			CHECK_HR;

			lvBaseFormID.resize(lnForms);
			lvDataTypes.resize(lnForms);
			lvFormTypes.resize(lnForms);

			for (i = 0; i < lnForms; i++)
			{
				int lobjtype = 0;
				hr = mpReadStream->ReadInt(&lobjtype);
				CHECK_HR;

				if (lobjtype != EnumObjectAttributeForm)
					return E_UNEXPECTED_DATATYPE;

				hr = mpReadStream->ReadInt(&lvBaseFormID[i]);
                CHECK_HR;
				hr = mpReadStream->ReadInt(&lvDataTypes[i]);
                CHECK_HR;
				hr = mpReadStream->ReadInt(&lvFormTypes[i]);
				CHECK_HR;
			}
		}
		hr = mpReadStream->EndReadBlock();
		CHECK_HR;

		hr = mpReadStream->BeginReadBlock(&lBlockType);
		CHECK_HR_BLOCK(EnumDataBlockAttributeElementsV1);
		{
			vector<int> lvPartialEleList;
			hr = mpReadStream->ReadIntArray(lvPartialEleList);
			CHECK_HR;

			bool lbPartialData = lvPartialEleList.size() > 0;
			int lMaxElementCount = 0;
			for (i = 0; i < lnForms; i++)
			{
				int lnElements = 0;
				hr = mpReadStream->ReadInt(&lnElements);
				CHECK_HR;

				lMaxElementCount = lnElements;
				if (lbPartialData)
				{
					_ASSERT(lnElements == lvPartialEleList.size());

					//find out the largest element ID
					int lMaxID = 0;
					for (int ie = 0; ie < lnElements; ie++)
					{
						if (lvPartialEleList[ie] > lMaxID)
							lMaxID = lvPartialEleList[ie];
					}
					lMaxElementCount = lMaxID + 1;
				}

				DSSDataColumn *lpColumn = NULL;
				hr = DSSAEHelper::GenXTabCol((EnumDSSDataType)lvDataTypes[i], lMaxElementCount, &lpColumn, false, false, true);
				CHECK_HR;

				if (lbIsRawData)
				{
					if (DssDataTypeVarChar != lvDataTypes[i] && DssDataTypeBigDecimal != lvDataTypes[i] &&
						DssDataTypeMBChar != lvDataTypes[i] && DssDataTypeBinary != lvDataTypes[i] && DssDataTypeUTF8Char!= lvDataTypes[i])
					{
						hr = hLoadRawDataColumn(mpReadStream, lpColumn, (EnumDSSDataType)lvDataTypes[i], lnElements, lvPartialEleList);
						CHECK_HR;
					}
					else
					{
						hr = hLoadStringDataColumn(mpReadStream, lpColumn, (EnumDSSDataType)lvDataTypes[i], lnElements, lvPartialEleList);
						CHECK_HR;
					}
				}
				else
				{
					hr = hLoadStringDataColumn(mpReadStream, lpColumn, (EnumDSSDataType)lvDataTypes[i], lnElements, lvPartialEleList);
					CHECK_HR;
				}

				vector<char> lvFlags;
				hr = mpReadStream->ReadByteArray(lvFlags);
				CHECK_HR;

				if (!lvFlags.empty())
				{
					if(lbPartialData)
						hr = lpColumn->setFlags(0, lvFlags.size(), (DSSDataFlag*)(&lvFlags[0]), (int*)(&lvPartialEleList[0]), lvPartialEleList.size());
					else
						hr = lpColumn->setFlags(0, lvFlags.size(), (DSSDataFlag*)(&lvFlags[0]), lvFlags.size() * sizeof(char));
					CHECK_HR;
				}
				hr = lpAttribute->AddFormDataColumn(lvBaseFormID[i], lvFormTypes[i], lpColumn);
				CHECK_HR;
			}
            
            //junwang 09/01/2011 Red Rocket: issue 500695
            if(lbIsRawData)
            {
                if(mpReadStream->MoreDataInBlock())
                {
                    hr = mpReadStream->BeginReadBlock(&lBlockType);
                    CHECK_HR_BLOCK(EnumDataBlockAttributeInputOrderV1);
                    
                    DSSDataColumn* lpColumn = NULL;
                    for(i = 0; i<lnForms; i++)
                    {
                        hr = lpAttribute->Item(i, &lpColumn);
                        CHECK_HR;
                        
                        hr = hLoadInputOrder(mpReadStream, lpColumn, lvPartialEleList, lMaxElementCount);
                        CHECK_HR;
                    }
                    
                    hr = mpReadStream->EndReadBlock();
                    CHECK_HR;   
                }
            }
		}
		hr = mpReadStream->EndReadBlock();
		CHECK_HR;

		//409782: load null element
		hr = hLoadNullElement(mpReadStream, lpAttribute);
		CHECK_HR;

		if (oppXTabUnit)
		{
			*oppXTabUnit = lpAttribute;
			lpAttributeHolder.GiveUp();
		}
	}
	hr = mpReadStream->EndReadBlock();
	return hr;
}

//junwang 09/01/2011 Red Rocket: issue 500695
int hLoadInputOrder(DSSBinaryDataReader *ipReadStream, DSSDataColumn *ipColumn, vector<int>& ivPartialElementList, int columnRowCount)
{
    int hr = S_OK;
    //  int lnElements = ipColumn->getRowCount();
    //   int lObj = 0;
    
    vector<int> lvRankId;
    vector<int> lvSortOrder;
    
    ipReadStream->ReadIntArray(lvRankId);
    
    
    //convert rank to sort result
    if(!lvRankId.empty())
    {
        lvSortOrder.resize(lvRankId.size());
        
        int i;
        for (i=0; i<(int)lvRankId.size(); i++) 
        {
            lvSortOrder[lvRankId[i]] = i;
        }
        
        if(ivPartialElementList.size() > 0 && ivPartialElementList.size() < lvSortOrder.size())
        {
            int lSize = 0;
            for(int j = 0;  j < lvSortOrder.size() && lSize <= columnRowCount; j++)
            {
                if(lvSortOrder[j] < columnRowCount)
                {
                    lvSortOrder[lSize++] = lvSortOrder[j];
                }
            }
            lvSortOrder.resize(columnRowCount);
        }
        hr = ipColumn->SetInputOrder((int)lvSortOrder.size(), &lvSortOrder[0]);
        
        CHECK_HR;
    }
    
    return S_OK;
}

int hLoadNullElement(DSSBinaryDataReader *mpReadStream, DSSTabularUnit *ipXTabUnit)
{
	vector<int> lvElementID;
	int hr = mpReadStream->ReadIntArray(lvElementID);
	CHECK_HR;

	int lnNullElements = 0;
	hr = mpReadStream->ReadInt(&lnNullElements);
	CHECK_HR;

	if (lnNullElements == 1)
	{
		MBase::CString lNullElementStr;
		hr = mpReadStream->ReadString(lNullElementStr);
		CHECK_HR;

		ipXTabUnit->setNullElement(lvElementID[0], lNullElementStr);
	}
	return S_OK;
}


int hLoadUnitMetrics(DSSBinaryDataReader *mpReadStream, int index, DSSTabularUnit **oppXTabUnit, vector<int> *opvMetricDataTypes)
{
	int hr = S_OK;
	int lBlockType = 0;

	int lnMetrics = 0;
	hr = mpReadStream->ReadInt(&lnMetrics);
	CHECK_HR;

	DSSTabularMetrics *lpMetrics = new DSSTabularMetrics();
	MBase::DSSStrongPtr<DSSTabularMetrics> lpMetricsHolder(lpMetrics);
	hr = lpMetrics->Init(index, lnMetrics);
	CHECK_HR;

	opvMetricDataTypes->resize(lnMetrics);

	int i = 0;
	for (i = 0; i < lnMetrics; i++)
	{
		int lObj = 0;
		hr = mpReadStream->ReadInt(&lObj);
		CHECK_HR;

		if (lObj != EnumObjectMetric)
			return E_UNEXPECTED_DATATYPE;

		hr = mpReadStream->BeginReadBlock(&lBlockType);
		CHECK_HR_BLOCK(EnumDataBlockMetricV1);
		{
			int lMetricID = 0;
			hr = mpReadStream->ReadInt(&lMetricID);
			CHECK_HR;

			hr = mpReadStream->ReadInt(&(*opvMetricDataTypes)[i]);
			CHECK_HR;

			MBase::CString lguid;
			hr = mpReadStream->ReadString(lguid);
			CHECK_HR;

			vector<int> lvMetricImplementations;
			hr = mpReadStream->BeginReadBlock(&lBlockType);
			CHECK_HR_BLOCK(EnumDataBlockMetricSubtotalV1);
			{
				hr = mpReadStream->ReadIntArray(lvMetricImplementations);
				CHECK_HR;
				if (lvMetricImplementations.size() < 2)
					return E_FAIL;
			}

			hr = lpMetrics->AddMetric(lMetricID, lguid, (*opvMetricDataTypes)[i], lvMetricImplementations[1]);
			CHECK_HR;

			hr = mpReadStream->EndReadBlock();
			CHECK_HR;
		}
		hr = mpReadStream->EndReadBlock();
	}

	if (oppXTabUnit)
	{
		*oppXTabUnit = lpMetrics;
		lpMetricsHolder.GiveUp();
	}

	return hr;
}

int hLoadUnitCustomGroup(DSSBinaryDataReader *mpReadStream, int index, DSSTabularUnit **oppXTabUnit)
{
	int hr = S_OK;
	int lBlockType = 0;
	hr = mpReadStream->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockCustomGroupV1);
	{
		MBase::CString lguid;
		hr = mpReadStream->ReadString(lguid);
		CHECK_HR;

		int lnElements = 0;
		hr = mpReadStream->ReadInt(&lnElements);
		CHECK_HR;

		DSSTabularCustomGroup *lpCustomGroup = new DSSTabularCustomGroup();
		MBase::DSSStrongPtr<DSSTabularCustomGroup> lpCustomGroupHolder(lpCustomGroup);
		hr = lpCustomGroup->Init(index, lnElements);
		CHECK_HR;
		hr = lpCustomGroup->setGUID(lguid);
		CHECK_HR;

		XTabCustomGroupElement lCGElement;
		int i = 0;
		for (i = 0; i < lnElements; i++)
		{
			int lobjtype = 0;
			hr = mpReadStream->ReadInt(&lobjtype);
			CHECK_HR;

			if (lobjtype != EnumObjectCustomGroupElement)
				return E_UNEXPECTED_DATATYPE;

			int id = 0;
			hr = mpReadStream->ReadInt(&id);
			CHECK_HR;

			MBase::CString lNullElementID;
			hr = mpReadStream->ReadString(lNullElementID);
			CHECK_HR;

			hr = mpReadStream->ReadString(lCGElement.elementID);	//TODO: check if there's memory leak
			CHECK_HR;

			hr = mpReadStream->ReadInt(&lCGElement.depth);
			CHECK_HR;

			hr = mpReadStream->ReadInt(&lCGElement.parent);
			CHECK_HR;

			hr = mpReadStream->ReadInt(&lCGElement.mType);
			CHECK_HR;

			hr = mpReadStream->ReadInt(&lCGElement.ordinal);
			CHECK_HR;

			hr = mpReadStream->ReadInt(&lCGElement.nodeID);
			CHECK_HR;

			hr = mpReadStream->ReadString(lCGElement.displayName);
			CHECK_HR;

			if(lNullElementID.size()>0)
				lpCustomGroup->setNullElement(id, lNullElementID);

			hr = lpCustomGroup->setElement(id, lCGElement);
			CHECK_HR;
		}

		if (oppXTabUnit)
		{
			*oppXTabUnit = lpCustomGroup;
			lpCustomGroupHolder.GiveUp();
		}
	}
	hr = mpReadStream->EndReadBlock();
	return hr;
}

int hLoadUnitConsolidation(DSSBinaryDataReader *mpReadStream, int index, DSSTabularUnit **oppXTabUnit,DSSGlobalLookUpTable* ipGlobalLookUp, bool standAloneDE)
{
	int hr = S_OK;
	int lBlockType = 0;
	hr = mpReadStream->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockConsolidationV1);
	{
		MBase::CString lguid;
		hr = mpReadStream->ReadString(lguid);
		CHECK_HR;

        DSSTabularConsolidation *lpConsolidation = new DSSTabularConsolidation();
		MBase::DSSStrongPtr<DSSTabularConsolidation> lpConsolidationHolder(lpConsolidation);

        hr = lpConsolidation->setGUID(lguid);
		CHECK_HR;
        
        GUID lID = lpConsolidation->getGUID();
        
		bool lbIsDerivedElement = false;
		hr = mpReadStream->ReadBool(&lbIsDerivedElement);
		CHECK_HR;

		DSSTabularUnit *lpChildUnit = NULL;
		if (lbIsDerivedElement)
		{
            if (ipGlobalLookUp)
            {
                GUID lGLookUpID = ipGlobalLookUp->getGUID();
                if (lGLookUpID == lID || standAloneDE)
                {
                    lpChildUnit = ipGlobalLookUp;
                }
                else
                {
                    assert(false);
                    return E_FAIL;
                }
            }
            else
            {
                hr = hLoadUnitAttribute(mpReadStream, index, &lpChildUnit);
                CHECK_HR;
            }
		}

		int lnElements = 0;
		hr = mpReadStream->ReadInt(&lnElements);
		CHECK_HR;
        
        hr = lpConsolidation->Init(index, lnElements);
		CHECK_HR;

		XTabConsolidationElement lCSElement;
		int i = 0;
		for (i = 0; i < lnElements; i++)
		{
			int lobjtype = 0;
			hr = mpReadStream->ReadInt(&lobjtype);
			CHECK_HR;

			if (lobjtype != EnumObjectConsolidationElement)
				return E_UNEXPECTED_DATATYPE;

			int id = 0;
			hr = mpReadStream->ReadInt(&id);
			CHECK_HR;

			MBase::CString lNullElementID;
			hr = mpReadStream->ReadString(lNullElementID);
			CHECK_HR;

			hr = mpReadStream->ReadString(lCSElement.guidstr);	//TODO: check if there's memory leak
			CHECK_HR;

			hr = mpReadStream->ReadInt(&lCSElement.branch);
			CHECK_HR;

			hr = mpReadStream->ReadInt(&lCSElement.ordinal);
			CHECK_HR;

			hr = mpReadStream->ReadString(lCSElement.value);
			CHECK_HR;

			hr = mpReadStream->ReadInt(&lCSElement.childElementID);
			CHECK_HR;

			if(lNullElementID.size()>0)
				lpConsolidation->setNullElement(id, lNullElementID);

			hr = lpConsolidation->setElement(id, lCSElement);
			CHECK_HR;
		}
        
        if(lbIsDerivedElement && ipGlobalLookUp/*687572*/)
        {
            hr = mpReadStream->BeginReadBlock(&lBlockType);
            CHECK_HR;
            
            //zhyang, capatiablt with old logic 
            if(mpReadStream->MoreDataInBlock())
            {
                for(int i = 0; i < lnElements; i++)
                {
                    if(lpConsolidation->getElement(i)->branch)
                    {
                        hr = mpReadStream->ReadIntArray(lpConsolidation->getElement(i)->mvLeaves);
                        CHECK_HR;
                    }
                }
            }
            
            hr = mpReadStream->EndReadBlock();
            CHECK_HR;
        }

		hr = lpConsolidation->setIsDerivedElement(lbIsDerivedElement, lpChildUnit, ipGlobalLookUp != NULL);
		CHECK_HR;

		if (oppXTabUnit)
		{
			*oppXTabUnit = lpConsolidation;
			lpConsolidationHolder.GiveUp();
		}
	}
	hr = mpReadStream->EndReadBlock();
	return hr;
	return hr;
}
 
int DSSModelPopulation::hLoadSlicedView(DSSDataModelXTabImpl *ipModel, LayoutInfo * ipLayout,
									 vector<int> &irMetricDataType, vector<vector<int> > &irTotalDatatype)
{
	int hr = S_OK;
	int lBlockType = 0;
	hr = mpReadStream->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockXTabModelViewsV1);
	{
		int lnSlices = 0;
		hr = mpReadStream->ReadInt(&lnSlices);
		CHECK_HR;

		int lStreamDataPosition = mpReadStream->GetPosition();

		vector<vector<char> > lvCellFlags(lnSlices);
		vector<vector<int> > lvCellThresholdID(lnSlices);	//TODO:
		vector<vector<int> > lvCellThresholdMetricID(lnSlices);	//TODO:


		vector<vector<int> > lvRowKeys(lnSlices);
		vector<vector<unsigned char> > lvRowMetricID(lnSlices);
		vector<vector<unsigned char> > lvRowSubtotalID(lnSlices);
		vector<vector<short> > lvRowLevelID(lnSlices);

		vector<vector<int> > lvColKeys(lnSlices);
		vector<vector<unsigned char> > lvColMetricID(lnSlices);
		vector<vector<unsigned char> > lvColSubtotalID(lnSlices);
		vector<vector<short> > lvColLevelID(lnSlices);

		int lMetricsUnitID = ipModel->getMetricUnitID();
		if (lMetricsUnitID >= 0)	//410114
		{
			DSSTabularMetrics *lpMetrics = NULL;
			ipModel->getMetricsUnit(&lpMetrics);
			CHECK_PTR(lpMetrics);
            
			if (lpMetrics->Count() <= 0)
				lMetricsUnitID = -1;
		}

		bool lbMetricOnColAxis = (ipLayout[1].metricsPos >= 0);

		int i = 0;
		for (i = 0; i < lnSlices; i++)
		{
			int lObj = 0;
			hr = mpReadStream->ReadInt(&lObj);
			CHECK_HR;

			if (lObj != EnumObjectXTabView)
				return E_UNEXPECTED_DATATYPE;

			//TODO[opt]: move all vectors out of the big loop (lnSlices)

			//1. load gropu-by path of the current slice
			int lnGBs = 0;
			vector<int> lvGroupByPath;
			vector<unsigned char> lvGBTotalID, lvGBMetricID;
			vector<short> lvGBLevel;
			hr = hLoadHeader(mpReadStream, 2, ipLayout+2, lvGroupByPath, lnGBs, lvGBTotalID, lvGBMetricID, lvGBLevel, lvGroupByPath);
			CHECK_HR;

			//2. load row header element of the current slice
			int lnDispRows = 0;
			hr = hLoadHeader(mpReadStream, 0, ipLayout, lvGroupByPath, lnDispRows, lvRowSubtotalID[i], lvRowMetricID[i], lvRowLevelID[i], lvRowKeys[i]);
			CHECK_HR;

			//3. load column header element of the current slice
			int lnDispCols = 0;
			hr = hLoadHeader(mpReadStream, 1, ipLayout + 1, lvGroupByPath, lnDispCols, lvColSubtotalID[i], lvColMetricID[i], lvColLevelID[i], lvColKeys[i]);
			CHECK_HR;

			//4. load metric cell values of the current slice
			if (lnDispRows > 0 && lnDispCols > 0 && lMetricsUnitID >= 0)
			{
				int lGBTotalID = lvGBTotalID.size() > 0 ? lvGBTotalID[0] : 0;

				int iRow = 0;
				for (iRow = 0; iRow < lnDispRows; iRow++)
				{
					int lRowTotalID = lvRowSubtotalID[i][iRow];
					int lMetricID = (lMetricsUnitID >= 0 && !lbMetricOnColAxis) ? lvRowMetricID[i][iRow] : -1;	//metrics on the row axis

					int iCol = 0;
					for (iCol = 0; iCol < lnDispCols; iCol++)
					{
						int lColTotalID = lvColSubtotalID[i][iCol];
						// pick max(lGBTotalID, lRowTotalID, lColTotalID)
						int lSubtotalID = lColTotalID > lRowTotalID ? lColTotalID : lRowTotalID;
						if (lSubtotalID < lGBTotalID)
							lSubtotalID = lGBTotalID;

						if (lSubtotalID > 0)		//valid subtotal
						{
							if ((lRowTotalID > 0 && lSubtotalID > lRowTotalID) || (lColTotalID > 0 && lSubtotalID > lColTotalID) || (lGBTotalID > 0 && lSubtotalID > lGBTotalID))
								lSubtotalID = -1;	//invalid subtotal
						}
						else
							lSubtotalID = 0;		//no subtotal

						if (lbMetricOnColAxis && lMetricsUnitID >= 0)
							lMetricID = lvColMetricID[i][iCol];

						int lCellSize = 0;
						hr = hSkipCellData(mpReadStream, lMetricID, lSubtotalID, irMetricDataType, irTotalDatatype, lCellSize);
						CHECK_HR;
					}//end of for (iCol = 0; iCol < lnDispCols; iCol++)
				}//end of for (iRow = 0; iRow < lnDispRows; iRow++)
			}
			else if (lMetricsUnitID >= 0)
			{
				int lGBTotalID = lvGBTotalID.size() > 0 ? lvGBTotalID[0] : 0;
				if (lbMetricOnColAxis)	//except GB units, all are on column axis
				{
					//lvCellValue[i].resize(lnDispCols);
					//vector<unsigned char> *lpCells = &lvCellValue[i][0];

					int iCol = 0;
					for (iCol = 0; iCol < lnDispCols; iCol++)
					{
						int lColTotalID = lvColSubtotalID[i][iCol];
						int lSubtotalID = lGBTotalID > lColTotalID ? lGBTotalID : lColTotalID;
						if (lSubtotalID > 0)		//valid subtotal
						{
							if (lGBTotalID > 0 && lSubtotalID > lGBTotalID)
								lSubtotalID = -1;	//invalid subtotal
						}
						else
							lSubtotalID = 0;		//no subtotal

						int lCellSize = 0;
						hr = hSkipCellData(mpReadStream, lvColMetricID[i][iCol], lSubtotalID, irMetricDataType, irTotalDatatype, lCellSize);
						CHECK_HR;
						//EnumDSSDataType lCellDataType;
						//vector<char> lvBuffer;
						//int lCellFmtType = 0, lCellFmtID = 0;
						//hr = hLoadCells(lvColMetricID[iCol], lvColTotalID[iCol],irMetricDataType, irTotalDatatype, lCellDataType, *lpCells, lCellFmtType, lCellFmtID);
						CHECK_HR;
						//lpCells++;
					}
				}
				else	//units on row axis only
				{
					//lvCellValue[i].resize(lnDispRows);
					//vector<unsigned char> *lpCells = &lvCellValue[i][0];

					int iRow = 0;
					for (iRow = 0; iRow < lnDispRows; iRow++)
					{
						int lRowTotalID = lvRowSubtotalID[i][iRow];
						int lSubtotalID = lGBTotalID > lRowTotalID ? lGBTotalID : lRowTotalID;
						if (lSubtotalID > 0)		//valid subtotal
						{
							if (lRowTotalID > 0 && lSubtotalID > lRowTotalID)
								lSubtotalID = -1;	//invalid subtotal
						}
						else
							lSubtotalID = 0;		//no subtotal

						int lCellSize = 0;
						hr = hSkipCellData(mpReadStream, lvRowMetricID[i][iRow], lSubtotalID, irMetricDataType, irTotalDatatype, lCellSize);
						CHECK_HR;
						//EnumDSSDataType lCellDataType;
						//vector<char> lvBuffer;
						//int lCellFmtType = 0, lCellFmtID = 0;
						//hr = hLoadCells(lvRowMetricID[iRow], lvRowTotalID[iRow], irMetricDataType, irTotalDatatype, lCellDataType, *lpCells, lCellFmtType, lCellFmtID);
						CHECK_HR;
						//lpCells++;
					}
				}
			}
			else if (lnDispRows > 0 && lnDispCols > 0 && !ipLayout[0].vUnits.empty() && !ipLayout[1].vUnits.empty())
			{
				//lwang, 10/5/19, 409363
				hr = mpReadStream->Skip(lnDispRows * lnDispCols * sizeof(bool));
				CHECK_HR;

			}//end of else if (lMetricsUnitID >= 0)

			//5. load data flags
			//vector<char> lvFlags;
			hr = mpReadStream->ReadByteArray(lvCellFlags[i]);
			CHECK_HR;

			//6. load threshold
			//vector<int> lvThresholdID;
			hr = mpReadStream->ReadIntArray(lvCellThresholdID[i]);
			CHECK_HR;
			//vector<int> lvThresholdMetricID;
			hr = mpReadStream->ReadIntArray(lvCellThresholdMetricID[i]);
			CHECK_HR;
		}

		MergeVectors(lvRowKeys, ipLayout[0].mvKeys);
		MergeVectors(lvRowLevelID, ipLayout[0].mvLevelID);
		MergeVectors(lvRowSubtotalID, ipLayout[0].mvSubTotalID);
		MergeVectors(lvColKeys, ipLayout[1].mvKeys);
		MergeVectors(lvColLevelID, ipLayout[1].mvLevelID);
		MergeVectors(lvColSubtotalID, ipLayout[1].mvSubTotalID);

		DSSCube *lpCube = ipModel->getCube();
		CHECK_PTR(lpCube);

		DSSTabularIndexPool *lpIndexPool = NULL;
		lpCube->getIndexPool(&lpIndexPool);
		CHECK_PTR(lpIndexPool);

		//TODO: normalize level ID:
		for (i = 0; i < 2; i++)
		{
			int lnUnits = ipLayout[i].vUnits.size();
			int lBitMapSize = 1 << lnUnits;
			vector<char> lvBitMap(lBitMapSize > 0 ? lBitMapSize : 1);
			memset(&lvBitMap[0], 0, sizeof(char));

			vector<short> *lpvLevelID = &ipLayout[i].mvLevelID;
			vector<short>::iterator it = lpvLevelID->begin();
			for (; it != lpvLevelID->end(); it++)
			{
				if (lvBitMap[*it] == 0)
				{
					lvBitMap[*it] = 1;
				}
			}
			int lev = 1;
			vector<char>::iterator itLev = lvBitMap.begin();
			for (; itLev != lvBitMap.end(); itLev++)
			{
				if (*itLev == 1)
				{
					*itLev = lev++;
				}
			}

			//no need to change mvLevelID;
			//for (it = lpvLevelID->begin(); it != lpvLevelID->end(); it++)
			//{				*it = lvBitMap[*it] - 1;			}

			vector<vector<short> > *lpOrigSubtotalID = i == 0 ? &lvRowLevelID : &lvColLevelID;
			vector<vector<short> > ::iterator itOrig = lpOrigSubtotalID->begin();
			for (; itOrig != lpOrigSubtotalID->end(); itOrig++)
			{
				for (it = itOrig->begin(); it != itOrig->end(); it++)
					*it = lvBitMap[*it] - 1;
			}
		}

		hr = hBuildHeaderIndex(ipLayout, ipModel);
		CHECK_HR;

		hr = hBuildHeaderIndex(ipLayout+1, ipModel);
		CHECK_HR;

		int lnRows = ipLayout[0].mvDistinctRows.size();
		int lnColumns = ipLayout[1].mvDistinctRows.size();
		vector<char> lvbXTabNull(lnRows * lnColumns);
		memset(&(lvbXTabNull[0]), 0, sizeof(char) * lnRows * lnColumns);

/*		vector<int> *lpvMap2DistinctRowID = &ipLayout[0].mvMap2DistinctRows;
		vector<int> *lpvMap2DistinctColID = &ipLayout[1].mvMap2DistinctRows;

		int iRow = 0, iCol = 0;
		for (i = 0; i < lnSlices; i++)
		{
			int lnRowsSlice = lvRowLevelID[i].size();
			int lnColsSlice = lvColLevelID[i].size();
			//TODO: this logic is not correct, lvCellFlag may not cover the area [lnRowsSlice * lnColsSlice]
			//So we should set lvbXTabNull to true, and reset each one that's not XTabNULL;
			if (!lvCellFlags[i].empty())
			{
				int j = 0;
				for (j = 0; j < lnRowsSlice; j++)
				{
					char *lpXTabNull = &lvbXTabNull[(*lpvMap2DistinctRowID)[iRow + j]];
					char *lpFlag = &lvCellFlags[i][j * lnColsSlice];
					int k = 0;
					for (k = 0; k < lnColsSlice; k++)
					{
						if ( lpFlag[k] == DssDataInvalidCell
							&& lpXTabNull[(*lpvMap2DistinctColID)[iCol]] == 0)
						{
							lpXTabNull[(*lpvMap2DistinctColID)[iCol]] = 1;
						}
					}
				}
			}
			iRow += lnRowsSlice;
			iCol += lnColsSlice;
		}*/

		//TODO: runtime unit is not on row header
		vector<DSSTabularIndex*> lvCellIndices;
		hr = hBuildCellIndex(ipLayout, ipModel, lvbXTabNull, lvCellIndices);
		CHECK_HR;

		hr = hCreateDataSlices(ipModel, lvCellIndices, irMetricDataType, irTotalDatatype);
		CHECK_HR;

		DSSXTabView *lpXTabView = ipModel->getXTabView();
		CHECK_PTR(lpXTabView);

		//load cell data
		hr = mpReadStream->SetPosition(lStreamDataPosition);
		CHECK_HR;

		vector<int> lvRowUnitsMap;
		vector<int> lvColUnitsMap;
		lvRowUnitsMap.assign(ipLayout[0].vUnits.begin(), ipLayout[0].vUnits.end());
		lvColUnitsMap.assign(ipLayout[1].vUnits.begin(), ipLayout[1].vUnits.end());
		if (ipLayout[0].metricsPos >= 0 && ipLayout[0].metricsPos < lvRowUnitsMap.size())
			lvRowUnitsMap[ipLayout[0].metricsPos] = -1;

		if (ipLayout[1].metricsPos >= 0 && ipLayout[1].metricsPos < lvColUnitsMap.size())
			lvColUnitsMap[ipLayout[1].metricsPos] = -1;

		int lnTotalUnits = ipModel->Count();
		vector<int> lvCellKeys(lnTotalUnits);

		//int lnColLevles = ipLayout[1].mTotalLevels;
		//int lnMetrics = irMetricDataType.size();
		//int lnSubtotals = irTotalDatatype.size();
		//if (lnSubtotals == 0)
		//	lnSubtotals = 1;

		//int lSlicesPerRow = lnColLevles * lnMetrics * lnSubtotals;
		//int lSlicesPerCol = lnMetrics * lnSubtotals;

		vector<int> lvGBKeys;
		lvGBKeys.reserve(lnSlices * (ipLayout[2].vUnits.size()));

		vector<int> lvRowWindowSize(lnSlices + 1);
		vector<int> lvColWindowSize(lnSlices + 1);
		lvRowWindowSize[0] = 0;
		lvColWindowSize[0] = 0;

		int lnRowUnitsDiv = ipLayout[0].vUnits.size() - ipLayout[0].mnRuntimeUnits;
		int lnColUnitsDiv = ipLayout[1].vUnits.size() - ipLayout[1].mnRuntimeUnits;

		if (lnRowUnitsDiv <= 0)
			lnRowUnitsDiv = 1;
		if (lnColUnitsDiv <= 0)
			lnColUnitsDiv = 1;

		for (i = 0; i < lnSlices; i++)
		{
			int lObj = 0;
			hr = mpReadStream->ReadInt(&lObj);
			CHECK_HR;

			if (lObj != EnumObjectXTabView)
				return E_UNEXPECTED_DATATYPE;

			int lnGBs = 0;
			vector<int> lvGroupByPath;
			vector<unsigned char> lvGBTotalID, lvGBMetricID;
			vector<short> lvGBLevel;
			hr = hLoadHeader(mpReadStream, 2, ipLayout+2, lvGroupByPath, lnGBs, lvGBTotalID, lvGBMetricID, lvGBLevel, lvGroupByPath);
			CHECK_HR;
			lvGBKeys.insert(lvGBKeys.end(), lvGroupByPath.begin(), lvGroupByPath.end());

			//skip headers
			int lRowWindowSize = 0;
			hr = mpReadStream->ReadInt(&lRowWindowSize);
            CHECK_HR;
			if (lRowWindowSize > 0)
				mpReadStream->Skip(lRowWindowSize * sizeof(int));

			int lColWindowSize = 0;
			mpReadStream->ReadInt(&lColWindowSize);

			if (lColWindowSize > 0)
				mpReadStream->Skip(lColWindowSize * sizeof(int));

			if (lMetricsUnitID >= 0 && lRowWindowSize == 0)
				lRowWindowSize = 1;

			if (lMetricsUnitID >= 0 && lColWindowSize == 0)
				lColWindowSize = 1;

			lvRowWindowSize[i + 1] = lvRowWindowSize[i] + lRowWindowSize / lnRowUnitsDiv;
			lvColWindowSize[i + 1] = lvColWindowSize[i] + lColWindowSize / lnColUnitsDiv;

			//load metric cell values of the current slice
			int lnDispRows = lvRowLevelID[i].size();
			int lnDispCols = lvColLevelID[i].size();
			if (lnDispRows > 0 && lnDispCols > 0 && lMetricsUnitID >= 0)
			{
				int lGBTotalID = lvGBTotalID.size() > 0 ? lvGBTotalID[0] : 0;

				int iRow = 0;
				for (iRow = 0; iRow < lnDispRows; iRow++)
				{
					for (int k = 0; k < lvRowUnitsMap.size(); k++)
					{
						if (lvRowUnitsMap[k] >= 0)
							lvCellKeys[lvRowUnitsMap[k]] = lvRowKeys[i][iRow * lvRowUnitsMap.size() + k];
					}

					int lRowTotalID = lvRowSubtotalID[i][iRow];
					int lMetricID = (lMetricsUnitID >= 0 && !lbMetricOnColAxis) ? lvRowMetricID[i][iRow] : -1;	//metrics on the row axis
					int lRowLevel = lvRowLevelID[i][iRow];

					int iCol = 0;
					for (iCol = 0; iCol < lnDispCols; iCol++)
					{
						for (int k = 0; k < lvColUnitsMap.size(); k++)
						{
							if (lvColUnitsMap[k] >= 0)
								lvCellKeys[lvColUnitsMap[k]] = lvColKeys[i][iCol * lvColUnitsMap.size() + k];
						}

						int lColTotalID = lvColSubtotalID[i][iCol];
						// pick max(lGBTotalID, lRowTotalID, lColTotalID)
						int lSubtotalID = lColTotalID > lRowTotalID ? lColTotalID : lRowTotalID;
						if (lSubtotalID < lGBTotalID)
							lSubtotalID = lGBTotalID;

						if (lSubtotalID > 0)		//valid subtotal
						{
							if ((lRowTotalID > 0 && lSubtotalID > lRowTotalID) || (lColTotalID > 0 && lSubtotalID > lColTotalID) || (lGBTotalID > 0 && lSubtotalID > lGBTotalID))
								lSubtotalID = -1;	//invalid subtotal
						}
						else
							lSubtotalID = 0;		//no subtotal

						if (lbMetricOnColAxis && lMetricsUnitID >= 0)
							lMetricID = lvColMetricID[i][iCol];

						int lColLevel = lvColLevelID[i][iCol];
						hr = hSetMetricCell(lpXTabView, lMetricID, lSubtotalID, lnTotalUnits, &lvCellKeys[0],
											iRow * lnDispCols + iCol, lRowLevel, lColLevel, irMetricDataType, irTotalDatatype,
											lvCellFlags[i], lvCellThresholdID[i], lvCellThresholdMetricID[i]);
						CHECK_HR;

/*						EnumDSSDataType lCellDataType;
						vector<unsigned char> lvBuffer;
						int lCellFmtType = 0, lCellFmtID = 0;
						hr = hLoadCells(lMetricID, lSubtotalID, irMetricDataType, irTotalDatatype, lCellDataType, lvBuffer, lCellFmtType, lCellFmtID);
						CHECK_HR;

						//fill the keys: TODO[opt]: calculate offset directly

						DSSCubeDataSlice *lpSlice = NULL;	//TODO: lvDataSlices[lRowLevel * lSlicesPerRow + lColLevel * lSlicesPerCol + lMetricID * lnSubtotals + lSubtotalID];
						hr = lpXTabView->getDataSlice(lRowLevel, lColLevel, lMetricID, &lpSlice);
						CHECK_PTR(lpSlice);

						EnumDSSDataFlag lFlag = DssDataOk;
						if (!lvCellFlags[i].empty())
							lFlag = static_cast<EnumDSSDataFlag>(lvCellFlags[i][iRow * lnDispCols + iCol]);
						//hr = lpSlice->setVal(lnGridUnits, &lvCellKeys[0], reinterpret_cast<char*> (&lvBuffer[0]), lvBuffer.size(), lCellDataType, lFlag);
						hr = lpSlice->setVal(lnGridUnits + lnRuntimeUnits, &lvCellKeys[0], reinterpret_cast<char*> (&lvBuffer[0]), lvBuffer.size(), lCellDataType, lFlag);
						CHECK_HR;
 */
					}//end of for (iCol = 0; iCol < lnDispCols; iCol++)
				}//end of for (iRow = 0; iRow < lnDispRows; iRow++)
			}
			else if (lMetricsUnitID >= 0)
			{
				int lGBTotalID = lvGBTotalID.size() > 0 ? lvGBTotalID[0] : 0;
				if (lbMetricOnColAxis)	//except GB units, all are on column axis
				{
					//lvCellValue[i].resize(lnDispCols);
					//vector<unsigned char> *lpCells = &lvCellValue[i][0];

					int iCol = 0;
					for (iCol = 0; iCol < lnDispCols; iCol++)
					{
						for (int k = 0; k < lvColUnitsMap.size(); k++)
						{
							if (lvColUnitsMap[k] >= 0)
								lvCellKeys[lvColUnitsMap[k]] = lvColKeys[i][iCol * lvColUnitsMap.size() + k];
						}

						int lColTotalID = lvColSubtotalID[i][iCol];
						int lSubtotalID = lGBTotalID > lColTotalID ? lGBTotalID : lColTotalID;
						if (lSubtotalID > 0)		//valid subtotal
						{
							if (lGBTotalID > 0 && lSubtotalID > lGBTotalID)
								lSubtotalID = -1;	//invalid subtotal
						}
						else
							lSubtotalID = 0;		//no subtotal

						int lColLevel = lvColLevelID[i][iCol];
						int lMetricID = lvColMetricID[i][iCol];

						hr = hSetMetricCell(lpXTabView, lMetricID, lSubtotalID, lnTotalUnits, &lvCellKeys[0],
											iCol, 0, lColLevel, irMetricDataType, irTotalDatatype,
											lvCellFlags[i], lvCellThresholdID[i], lvCellThresholdMetricID[i]);
						CHECK_HR;
					}
				}
				else	//units on row axis only
				{
					//lvCellValue[i].resize(lnDispRows);
					//vector<unsigned char> *lpCells = &lvCellValue[i][0];

					int iRow = 0;
					for (iRow = 0; iRow < lnDispRows; iRow++)
					{
						for (int k = 0; k < lvRowUnitsMap.size(); k++)
						{
							if (lvRowUnitsMap[k] >= 0)
								lvCellKeys[lvRowUnitsMap[k]] = lvRowKeys[i][iRow * lvRowUnitsMap.size() + k];
						}

						int lRowTotalID = lvRowSubtotalID[i][iRow];
						int lSubtotalID = lGBTotalID > lRowTotalID ? lGBTotalID : lRowTotalID;
						if (lSubtotalID > 0)		//valid subtotal
						{
							if (lRowTotalID > 0 && lSubtotalID > lRowTotalID)
								lSubtotalID = -1;	//invalid subtotal
						}
						else
							lSubtotalID = 0;		//no subtotal

						int lRowLevel = lvRowLevelID[i][iRow];
						int lMetricID = lvRowMetricID[i][iRow];

						hr = hSetMetricCell(lpXTabView, lMetricID, lSubtotalID, lnTotalUnits, &lvCellKeys[0],
											iRow, lRowLevel, 0, irMetricDataType, irTotalDatatype,
											lvCellFlags[i], lvCellThresholdID[i], lvCellThresholdMetricID[i]);
						CHECK_HR;
					}
				}
			}
			else if (lnDispRows > 0 && lnDispCols > 0 && !ipLayout[0].vUnits.empty() && !ipLayout[1].vUnits.empty())
			{
				//lwang, 10/5/19, 409363
				hr = mpReadStream->Skip(lnDispRows * lnDispCols * sizeof(bool));
				CHECK_HR;

			}//end of else if (lMetricsUnitID >= 0)

			//skip cell flags
			int lSkipSize = 0;
			mpReadStream->ReadInt(&lSkipSize);

			if (lSkipSize > 0)
				mpReadStream->Skip(lSkipSize * sizeof(char));

			//skip threshold ID
			mpReadStream->ReadInt(&lSkipSize);

			if (lSkipSize > 0)
				mpReadStream->Skip(lSkipSize * sizeof(int));

			//skip threshold metric ID
			mpReadStream->ReadInt(&lSkipSize);

			if (lSkipSize > 0)
				hr = mpReadStream->Skip(lSkipSize * sizeof(int));
			CHECK_HR;
		}

		//build slice window
		hr = ipModel->CreateViewMap(lnSlices, ipLayout[2].vUnits, lvGBKeys,
//									lvRowWindowSize, ipLayout[0].mvMap2DistinctRows,
//									lvColWindowSize, ipLayout[1].mvMap2DistinctRows);
									lvRowWindowSize, ipLayout[0].mvMap2DisplayedRows,
									lvColWindowSize, ipLayout[1].mvMap2DisplayedRows);
		CHECK_HR;
	}
	hr = mpReadStream->EndReadBlock();
	return hr;
}

//the header is a 2-D array in the binary stream, represent the header element keys
//this function will return: number of rows, header elements keys
//for each row, save the subtotal type, metric index and encode the level
int hLoadHeader(DSSBinaryDataReader *mpReadStream, int iAxis, LayoutInfo *ipLayoutInfo, vector<int> &irvGBPath, int &rnRows,
								 vector<unsigned char> &rTotalID, vector<unsigned char> &rMetricID,
								 vector<short> &rLevels, vector<int> &rKeyTable, bool ibEmptyMetrics)
{
	int hr = S_OK;
	hr = mpReadStream->ReadIntArray(rKeyTable);
	CHECK_HR;

	int i = 0;
	int j = 0;
	int lMetricPos = ipLayoutInfo->metricsPos;
	int lnUnits = ipLayoutInfo->vUnits.size();
	bool lbHasMetric = (lMetricPos >= 0 && lMetricPos < lnUnits);

	if (ipLayoutInfo->mnRuntimeUnits > 0)		//handle runtime unit
	{
		int lnRTUnits = ipLayoutInfo->mnRuntimeUnits;
		int lnStaticUnits = lnUnits - lnRTUnits;

		//_ASSERT(lnGBUnits + lnStaticUnits == lnUnits);

		int *lpGBPath = &irvGBPath[0];
		int *lpMap = &(ipLayoutInfo->mvMapRuntimeUnit2GBUnit[0]);
		if (rKeyTable.size() > 0 && lnStaticUnits > 0)
		{
			vector<int> lvTemp(rKeyTable);

			int lnRows = rKeyTable.size() / lnStaticUnits;
			rKeyTable.resize(lnRows * lnUnits);

			int *lpSrc = &lvTemp[0];
			int *lpDes = &rKeyTable[0];
			for (int i = 0; i < lnRows; i++)
			{
				memmove(lpDes, lpSrc, sizeof(int) * lnStaticUnits);
				lpDes += lnStaticUnits;
				lpSrc += lnStaticUnits;
				for (int j = 0; j < lnRTUnits; j++)
				{
					*lpDes = lpGBPath[lpMap[j]];
					if (*lpDes < 0)
						*lpDes += 10;	//total in group by header start from -11, -12, -13 ...
					lpDes++;
				}
			}
		}
		else
		{
			if(rKeyTable.size() > 0) //means the grid has dummy unit
			{
				int lnRow = rKeyTable.size();
				rKeyTable.resize(lnRTUnits * lnRow);
				for(int i = 0; i < lnRow; i++)
				{
					for (int j = 0; j < lnRTUnits; j++)
					{
						int key = lpGBPath[lpMap[j]];
						//411589
						if (key < 0)
							key += 10;	//total in group by header start from -11, -12, -13 ... 
						rKeyTable[j + i * lnRTUnits] = key;
					}
				}
			}
			else
			{
				//row units are all runtime
				rKeyTable.resize(lnRTUnits);
				for (int j = 0; j < lnRTUnits; j++)
				{
					int key = lpGBPath[lpMap[j]];
					//411589
					if (key < 0)
						key += 10;	//total in group by header start from -11, -12, -13 ... 
					rKeyTable[j] = key;
				}
			}
		}
	}

	rnRows = 0;
	if (lnUnits > 0)
	{
		rnRows = rKeyTable.size() / lnUnits;
		_ASSERT(iAxis != 2 || rnRows == 1);		//for group-by axis, each slice only contains one row

		if (rnRows * lnUnits != rKeyTable.size() || rnRows <= 0)
			return E_DATASTREAM;

		rMetricID.resize(rnRows);
		rTotalID.resize(rnRows);
		rLevels.resize(rnRows);

		int *lpKey = &rKeyTable[0];
		for (i = 0; i < rnRows; i++)
		{
			if (lbHasMetric)
				rMetricID[i] = lpKey[lMetricPos];
			else
				rMetricID[i] = -1;

			int lTotalID = 0;
			int lLevel = 0;
			for (j = 0; j < lnUnits; j++)
			{
				//level is using binary encoding, if header has 3 units {A,B,C},
				//the level of row {a1, b1, SUM} is 0+0+4=4, row {a1, SUM, SUM} is 0+2+4=6
				if (*lpKey < 0)
				{
					lLevel += (1 << j);
					//TODO: test groupby total case
					//total in group by header start from -11, -12, -13 ...
					lTotalID = -(*lpKey) - 10*iAxis*(iAxis - 1)/2;
				}
				lpKey++;
			}
			rTotalID[i] = lTotalID;
			rLevels[i] = lLevel;
		}
	}
	else if (iAxis != 2 && !ibEmptyMetrics)	//lnUnits = 0 and not group-by
	{
		if (rKeyTable.size() > 0)
			rnRows = rKeyTable.size();		//428803, duplicate rows
		else
			rnRows = 1;

		for (i = 0; i < rnRows; i++)
		{
			rMetricID.push_back(-1);
			rTotalID.push_back(0);
			rLevels.push_back(0);
		}
	}
	return hr;
}

int hSkipCellData(DSSBinaryDataReader *mpReadStream, int iMetricsID, int iSubtotalID, vector<int> &irMetricDataType, vector<vector<int> > &irTotalDatatype,
								   int &orCellSize)
{
	int hr = S_OK;

	int lDataType = 0;
	if (iMetricsID >= 0)
	{
		//TODO: test 303432, 379426, 302944
		if (iSubtotalID == 0)	//no subtotal
		{
			lDataType = irMetricDataType[iMetricsID];
		}
		else if (iSubtotalID > 0)//valid subtoal
		{
			lDataType = irTotalDatatype[iSubtotalID - 1][iMetricsID];
		}
		else					//invalid subtotal
		{
			//TODO: 330297
			lDataType = DssDataTypeReserved;
		}
	}
	else	//no metrics, only xtab nulls
	{
		lDataType = DssDataTypeChar;
	}

	orCellSize = hSizeOf((EnumDSSDataType)lDataType);

	if (DssDataTypeVarChar != lDataType && DssDataTypeBigDecimal != lDataType &&
		DssDataTypeMBChar != lDataType && DssDataTypeBinary != lDataType)
	{
		if (DssDataTypeCellFormatData == lDataType)
		{
			int lobjtype = 0;
			hr = mpReadStream->ReadInt(&lobjtype);
			CHECK_HR;

			if (lobjtype != DssDataTypeCellFormatData)
				return E_UNEXPECTED_DATATYPE;
		}

		hr = mpReadStream->Skip(orCellSize);
		CHECK_HR;

		if (DssDataTypeCellFormatData == lDataType) {
			orCellSize += sizeof(int) * 2;
			hr = mpReadStream->Skip(sizeof(int) * 2);
			CHECK_HR;
		}
	}
	else {
		orCellSize += sizeof(unsigned int);

		unsigned int lStrLen = 0;
		hr = mpReadStream->ReadStringLen(&lStrLen);
		CHECK_HR;

		orCellSize += lStrLen;
		hr = mpReadStream->Skip(lStrLen);
		CHECK_HR;
	}
	return 0;
}

int DSSModelPopulation::hLoadCells(int iMetricsID, int iSubtotalID,
								vector<int> &irMetricDataType, vector<vector<int> > &irTotalDatatype,
								EnumDSSDataType &orCellDataType, vector<unsigned char> &orBuffer, int &orCellFmtType, int &orCellFmtID)
{
	int hr = S_OK;

	int lDataType = 0;
	if (iMetricsID >= 0)
	{
		//TODO: test 303432, 379426, 302944
		if (iSubtotalID == 0)	//no subtotal
		{
			lDataType = irMetricDataType[iMetricsID];
		}
		else if (iSubtotalID > 0)//valid subtoal
		{
			lDataType = irTotalDatatype[iSubtotalID - 1][iMetricsID];
		}
		else					//invalid subtotal
		{
			//TODO: 330297
			lDataType = DssDataTypeReserved;
		}
	}
	else	//no metrics, only xtab nulls
	{
		lDataType = DssDataTypeChar;
	}

	orCellDataType = (EnumDSSDataType)lDataType;

	int lSize = hSizeOf(orCellDataType);
	if (DssDataTypeVarChar != lDataType && DssDataTypeBigDecimal != lDataType &&
		DssDataTypeMBChar != lDataType && DssDataTypeBinary != lDataType)
	{
		if (DssDataTypeCellFormatData == lDataType)
		{
			int lobjtype = 0;
			hr = mpReadStream->ReadInt(&lobjtype);
			CHECK_HR;

			if (lobjtype != DssDataTypeCellFormatData)
				return E_UNEXPECTED_DATATYPE;
		}

		if (lSize <= 0)
			return S_OK;

		orBuffer.resize(lSize);

		hr = mpReadStream->Read((unsigned char*)(&orBuffer[0]), lSize, lSize);
		CHECK_HR;

		if (lDataType == DssDataTypeLong)
		{
			//411868, long long is 8 bytes in Mac OS
			long long lVal = *((int*)(&orBuffer[0]));
			*((long long *)&orBuffer[0]) = lVal;
		}
		else if (DssDataTypeCellFormatData == lDataType) {
			mpReadStream->ReadInt(&orCellFmtType);
            mpReadStream->ReadInt(&orCellFmtID);
		}
	}
	else {
		//TODO[test]: string metric value
		unsigned int lStrLen = 0;
		hr = mpReadStream->ReadStringLen(&lStrLen);
		CHECK_HR;

		orBuffer.resize(lStrLen);
		hr = mpReadStream->ReadUTF8String(reinterpret_cast<char*>(&orBuffer[0]), lStrLen);
		CHECK_HR;
	}
	return 0;
}

int hLoadXTabInfo(DSSBinaryDataReader *mpReadStream, DSSDataModelXTabImpl *ipModel, LayoutInfo * ipLayout, vector<vector<int> > &irTotalDatatype)
{
	int hr = S_OK;
	int i = 0;

	int lBlockType = 0;
	//1. load null display property
	hr = mpReadStream->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockXTabModelNullsV1);
	{
		bool lNullOnTop = true;
		hr = mpReadStream->ReadBool(&lNullOnTop);
		CHECK_HR;

		ipModel->setNullOnTop(lNullOnTop);

		MBase::CString lNullReplacement;
		hr = mpReadStream->ReadString(lNullReplacement);
		CHECK_HR;

		ipModel->setNullReplacement(lNullReplacement);

		int lnTypes = 5;
		hr = mpReadStream->ReadInt(&lnTypes);
		CHECK_HR;

		vector<int> lvNullTypes;
		lvNullTypes.resize(lnTypes);
		hr = mpReadStream->ReadIntArray(&lvNullTypes[0], lnTypes);
		CHECK_HR;

		hr = mpReadStream->ReadInt(&lnTypes);
		CHECK_HR;
		vector<MBase::CString> lvDataNulls;
		lvDataNulls.resize(lnTypes);
		for (i = 0; i < lnTypes; i++)
		{
			hr = mpReadStream->ReadString(lvDataNulls[i]);
			CHECK_HR;
		}
		ipModel->setDataNullDisplay(lvDataNulls);

		hr = mpReadStream->ReadInt(&lnTypes);
		CHECK_HR;
		vector<MBase::CString> lvElementNulls;
		lvElementNulls.resize(lnTypes);
		for (i = 0; i < lnTypes; i++)
		{
			hr = mpReadStream->ReadString(lvElementNulls[i]);
			CHECK_HR;
		}
		ipModel->setElementNullDisplay(lvElementNulls);
	}
	mpReadStream->EndReadBlock();

	//2. load layout, row/column/group-by header units
	hr = mpReadStream->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockXTabModelLayoutV1);
	{
		for (i = 0; i < 3; i++)
		{
			ipLayout[i].axis = i;
			hr = mpReadStream->ReadIntArray(ipLayout[i].vUnits);
			CHECK_HR;
			ipLayout[i].mnRuntimeUnits = 0;
		}
	}
	mpReadStream->EndReadBlock();

	//3. load subtotal type
	hr = mpReadStream->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockXTabModelSubtotalsV1);
	{
		int lnTotals = 0;
		hr = mpReadStream->ReadInt(&lnTotals);
		CHECK_HR;

		ipModel->setSubtotalCount(lnTotals);
		irTotalDatatype.resize(lnTotals);
		int lnMetrics = 0;
		for (i = 0; i < lnTotals; i++)
		{
			int lObjType = 0;
			mpReadStream->ReadInt(&lObjType);

			if (lObjType != EnumObjectSubtotal)
				return E_UNEXPECTED_DATATYPE;

			int id = 0;
			hr = mpReadStream->ReadInt(&id);
			CHECK_HR;

			//id is 1 based
			if (id - 1 >= irTotalDatatype.size())
			{
				int count = irTotalDatatype.size() - 1;
				for (; count < id - 1; count++)
				{
					irTotalDatatype.push_back(vector<int>());
				}
			}

			MBase::CString lguid;
			hr = mpReadStream->ReadString(lguid);
			CHECK_HR;

			hr = mpReadStream->ReadIntArray(irTotalDatatype[id - 1]);
			CHECK_HR;

			if (lnMetrics == 0)
				lnMetrics = irTotalDatatype[id - 1].size();

			MBase::CString lName;
			hr = mpReadStream->ReadString(lName);
			CHECK_HR;

			int lType = 0;
			hr = mpReadStream->ReadInt(&lType);
			CHECK_HR;

			hr = ipModel->setSubtotalInfo(id - 1, id, lguid, irTotalDatatype[id - 1], lName, lType);
			CHECK_HR;
		}
		for (i = 0; i < irTotalDatatype.size(); i++)
		{
			if (irTotalDatatype[i].empty())
				irTotalDatatype[i].resize(lnMetrics);
		}
        

	}
	hr = mpReadStream->EndReadBlock();
	return hr;
}

int hSetRuntimeUnit(DSSDataModelXTabImpl *ipModel, LayoutInfo * ipLayout)
{
	bool lbPutRuntimeUnitOnRow = (ipLayout[0].metricsPos < 0);

	int lnRowUnits = ipLayout[0].vUnits.size();
	vector<int> lvbRowGroupbyUnit(lnRowUnits);		//mark if a row unit is group-by unit
	if (lnRowUnits > 0)
		memset(&lvbRowGroupbyUnit[0], 0, sizeof(int) * lnRowUnits);

	int lnColUnits = ipLayout[1].vUnits.size();
	vector<int> lvbColGroupbyUnit(lnColUnits);		//mark if a column unit is group-by unit
	if (lnColUnits > 0)
		memset(&lvbColGroupbyUnit[0], 0, sizeof(int) * lnColUnits);

	int lnRuntimeUnits = 0;
	int lnGBUnits = ipLayout[2].vUnits.size();
	int i = 0, j = 0;
	for (i = 0; i < lnGBUnits; i++)
	{
		int lUnitID = ipLayout[2].vUnits[i];

		//for each group-by unit, check if it's on row axis
		for (j = 0; j < lnRowUnits; j++)
		{
			if (lUnitID == ipLayout[0].vUnits[j])
			{
				lvbRowGroupbyUnit[j] = 1;
				break;
			}
		}
		if (j >= lnRowUnits)
		{
			//then check if it's on column axis
			for (j = 0; j < lnColUnits; j++)
			{
				if (lUnitID == ipLayout[1].vUnits[j])
				{
					lvbColGroupbyUnit[j] = 1;
					break;
				}
			}
			if (j >= lnColUnits)
			{
				if (lbPutRuntimeUnitOnRow)
				{
					ipLayout[0].vUnits.push_back(lUnitID);
					lvbRowGroupbyUnit.push_back(1);
					lnRuntimeUnits++;
					ipLayout[0].mvMapRuntimeUnit2GBUnit.push_back(i);
				}
				else
				{
					ipLayout[1].vUnits.push_back(lUnitID);
					lvbColGroupbyUnit.push_back(1);
					lnRuntimeUnits++;
					ipLayout[1].mvMapRuntimeUnit2GBUnit.push_back(i);
				}
				int hr = ipModel->setIsRunTimeUnit(lUnitID, true);
				CHECK_HR
			}
		}
	}
	if (lbPutRuntimeUnitOnRow)
		ipLayout[0].mnRuntimeUnits = lnRuntimeUnits;
	else
		ipLayout[1].mnRuntimeUnits = lnRuntimeUnits;

	//set unit sort order, group-by units first
	lnRowUnits = ipLayout[0].vUnits.size();
	ipLayout[0].mvUnitSortOrder.reserve(lnRowUnits);
	for (i = 0; i < lnRowUnits; i++)
	{
		if (lvbRowGroupbyUnit[i] == 1)
			ipLayout[0].mvUnitSortOrder.push_back(i);
	}
	for (i = 0; i < lnRowUnits; i++)
	{
		if (lvbRowGroupbyUnit[i] != 1)
			ipLayout[0].mvUnitSortOrder.push_back(i);
	}

	lnColUnits = ipLayout[1].vUnits.size();
	ipLayout[1].mvUnitSortOrder.reserve(lnColUnits);
	for (i = 0; i < lnColUnits; i++)
	{
		if (lvbColGroupbyUnit[i] == 1)
			ipLayout[1].mvUnitSortOrder.push_back(i);
	}
	for (i = 0; i < lnColUnits; i++)
	{
		if (lvbColGroupbyUnit[i] != 1)
			ipLayout[1].mvUnitSortOrder.push_back(i);
	}

	return S_OK;
}

int hSetMetrics(DSSDataModelXTabImpl *ipXTabModel, int metricUnitID, LayoutInfo *ipLayout)
{
	//set metrics position
	ipXTabModel->setMetricUnit(metricUnitID);

	int i = 0;
	for (i = 0; i < 3; i++)
	{
		ipLayout[i].metricsPos = -1;
		int j = 0;
		int lnUnits = ipLayout[i].vUnits.size();
		for (j = 0; j < lnUnits && metricUnitID >= 0; j++)
		{
			if (ipLayout[i].vUnits[j] == metricUnitID)
			{
				ipLayout[i].metricsPos = j;
				break;
			}
		}
	}
	return 0;
}

int hLoadRawDataColumn(DSSBinaryDataReader *mpReadStream, DSSDataColumn *ipColumn, EnumDSSDataType iDataType, int nElements, vector<int> &irvElementList)
{
	int hr = S_OK;

	bool lbIsFullColumn = irvElementList.empty();

	int lSizeOfDataType = hSizeOf(iDataType);
	int nSize = lSizeOfDataType * nElements;
	if (nSize <= 0)
		return 0;
	vector<char> lvRawDataBuffer(nSize);
	hr = mpReadStream->Read((unsigned char*)(&lvRawDataBuffer[0]), nSize, lSizeOfDataType);
	CHECK_HR;

	if (iDataType == DssDataTypeLong)
	{
		char *lpData = &lvRawDataBuffer[0];
		for (int i = 0; i < nElements; i++)
		{
			int elementID = lbIsFullColumn ? i : irvElementList[i];
			//411868, long long is 8 bytes in Mac OS
			long long lVal = *((int*)(&lvRawDataBuffer[0]));
			hr = ipColumn->setData(elementID, (char*)&lVal, sizeof(long long), 0);
			CHECK_HR;
			lpData += 8;
		}
		return S_OK;
	}
	else if (iDataType < DssDataTypeDate || iDataType > DssDataTypeTimeStamp)
	{
		if (lbIsFullColumn)
		{
			return ipColumn->setDataInBulk(0, nElements, &lvRawDataBuffer[0], lvRawDataBuffer.size(), NULL, 0);
		}
		else
		{
			char *lpData = &lvRawDataBuffer[0];
			for (int i = 0; i < nElements; i++)
			{
				int elementID = irvElementList[i];
				hr = ipColumn->setData(elementID, lpData, lSizeOfDataType, DssDataOk);
				CHECK_HR;
				lpData += lSizeOfDataType;
			}
		}
		return S_OK;
	}
	else
	{
		char *lpData = &lvRawDataBuffer[0];
		for (int i = 0; i < nElements; i++)
		{
			int elementID = lbIsFullColumn ? i : irvElementList[i];

			MDataType::DSSDateTime lTmp(*((double*)lpData)/(double)MDataType::DAY_IN_MILLISECONDS + gSecondsSince1970);
			hr = ipColumn->setData(elementID, (char*)&lTmp, sizeof(MDataType::DSSDateTime), 0);
			CHECK_HR;
			lpData += 8;
		}
		return S_OK;
	}

}

int hLoadStringDataColumn(DSSBinaryDataReader *mpReadStream, DSSDataColumn *ipColumn, EnumDSSDataType iDataType, int nElements, vector<int> &irvElementList)
{
	int hr = S_OK;
	bool lbIsString = (DssDataTypeChar == iDataType || DssDataTypeVarChar == iDataType
					  || DssDataTypeMBChar == iDataType || DssDataTypeUTF8Char == iDataType);

	bool lbIsDateTime = !lbIsString && (DssDataTypeDate == iDataType || DssDataTypeTime == iDataType
									  || DssDataTypeTimeStamp == iDataType);

	vector<char> lvUTF8Data(32);		//initialize to 32 bytes

	int lBufferSize = 0;
	lBufferSize = ipColumn->getSizeOfData();
	CHECK_HR;

	if (lBufferSize < 8)
		lBufferSize = 8;
	vector<char> lvNumericalData(lBufferSize);

	bool lbIsFullColumn = irvElementList.empty();

	int i = 0;
	for (i = 0; i < nElements; i++)
	{
		int elementID = lbIsFullColumn ? i : irvElementList[i];

		unsigned int lStrLen = 0;
		hr = mpReadStream->ReadStringLen(&lStrLen);
		CHECK_HR;

		if (lStrLen > lvUTF8Data.size())
			lvUTF8Data.resize(lStrLen * 2);

		hr = mpReadStream->ReadUTF8String(&lvUTF8Data[0], lStrLen);
		CHECK_HR;

		if (lbIsString)
		{
			hr = ipColumn->setData(elementID, &lvUTF8Data[0], lStrLen * sizeof(char), DssDataOk);
			CHECK_HR;
		}
		else if(iDataType==DssDataTypeBigDecimal)
		{
			MDataType::DSSBigDecimal lBigDecimal(&(lvUTF8Data[0]));
			hr=ipColumn->setData(elementID, reinterpret_cast<char *> (&lBigDecimal), sizeof(MDataType::DSSBigDecimal),DssDataOk);
			CHECK_HR;
		}
		else if (DssDataTypeBinary == iDataType)
		{
			//lwang, 07/07/2010, 413152, convert binary data to hex string
			vector<char> lHexString (lStrLen * 2 + 1);
			for (int j = 0; j < lStrLen; j++)
			{
				unsigned char lVal = lvUTF8Data[j];

				unsigned char loffset = (unsigned char)((lVal & 0xF0U) >> 4);
				lHexString[2 * j] = loffset <= 9 ? '0' + loffset : 'A' + loffset - 10;

				loffset = ((lVal & 0x0F));
				lHexString[2 * j + 1] = loffset <= 9 ? '0' + loffset : 'A' + loffset - 10;
			}
			lHexString[2 * lStrLen] = '\0';

			hr = ipColumn->setData(elementID, &lHexString[0], (lStrLen * 2 + 1) * sizeof(char), DssDataOk);
			CHECK_HR;
		}
		else if (!lbIsDateTime)
		{
			int lDataSize = 0;
			hr = DSSAEHelper::ConvertStringToData(&lvUTF8Data[0], iDataType, &lvNumericalData[0], lBufferSize, lDataSize);
			CHECK_HR;


			hr = ipColumn->setData(elementID, &lvNumericalData[0], lDataSize, DssDataOk);
			CHECK_HR;
		}
		else
		{
				double lVal = atof(&lvUTF8Data[0]);
				MDataType::DSSDateTime lTmp(lVal/(double)MDataType::DAY_IN_MILLISECONDS + gSecondsSince1970);
				hr = ipColumn->setData(elementID, (char*)&lTmp, sizeof(MDataType::DSSDateTime), 0);
				CHECK_HR;
		}
	}
	return S_OK;
}

/*int hLoadRefColumn(DSSBinaryDataReader *ipReadStream, DSSDataColumn* &ipColumn)
{
	int lBlockType = 0;
	int hr = ipReadStream->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockRefColumn);

	//1. load type and row count
	int lType;
	hr = ipReadStream->ReadInt(&lType);
    CHECK_HR;

    int lRowSize;
    hr = ipReadStream->ReadInt(&lRowSize);
    CHECK_HR;

	int lRow;
	//load row count
	hr = ipReadStream->ReadInt(&lRow);
	CHECK_HR;

	//2. use row count to init map
	ipColumn = new DSSDataRefColumn;
    CHECK_NEWED_PTR(ipColumn);

	ipColumn->Init((EnumDSSDataType)lType, lRow);
    static_cast<DSSDataRefColumn*>(ipColumn)->setRowSize(lRowSize);


	//3. load selected rows
	int lnSelectedRow;
	hr = ipReadStream->ReadInt(&lnSelectedRow);
	CHECK_HR;

	//load local column cout
	Int32 lnLocalColumn;
	hr = ipReadStream->ReadInt(&lnLocalColumn);
	CHECK_HR;

	vector<int> lvLocalGlobalStart;
	lvLocalGlobalStart.resize(lnLocalColumn);
	for (int i = 0; i < lnLocalColumn; i++)
	{
		lvLocalGlobalStart[i] = lRow + 1;
	}

    vector < std::pair<int,int> > lvNotSelectedBefore;

	//4. get global map information
	for (int i = 0; i < lnSelectedRow; i++)
	{
		// read two integers
		//1. element's global key 2. element's data column ID
		int lGlobalIndex;
		int lLocalColumnId;

		hr = ipReadStream->ReadInt(&lGlobalIndex);
        CHECK_HR;

		hr = ipReadStream->ReadInt(&lLocalColumnId);
		CHECK_HR;

		if (lGlobalIndex < lvLocalGlobalStart[lLocalColumnId])
		{
			lvLocalGlobalStart[lLocalColumnId] = lGlobalIndex;
		}

        //if (!(static_cast<DSSDataRefColumn*>(ipColumn))->ExistRow(lGlobalIndex))
        //{
        //}

		(static_cast<DSSDataRefColumn*>(ipColumn))->FillMap(lGlobalIndex, lLocalColumnId);

	}

	//mvMap should be finished.
	//mvStartGlobalKey should be finished.

	//5. load local column cout
	//resize mvColumn

	//load each local column
	for (int i = 0; i < lnLocalColumn; i++)
	{
        //load the flag of null column
        bool lfNotNull = true;
        hr = ipReadStream->ReadBool(&lfNotNull);
        CHECK_HR;

        if (!lfNotNull)
        {
            //push a null column to local columns
            (static_cast<DSSDataRefColumn*>(ipColumn))->AddLocalColumn(-1, NULL);
        }
        else
        {
            //1. load type and size
            int lColType;
            int lnColumnRow;

            hr = ipReadStream->ReadInt(&lColType);
            CHECK_HR;

            hr = ipReadStream->ReadInt(&lnColumnRow);
            CHECK_HR;

            //2. create data column
            //GenCol : type, size,
            DSSDataColumn* lpColumn;
            hr = DSSAEHelper::GenXTabCol((EnumDSSDataType)lColType, lnColumnRow, &lpColumn);
            CHECK_HR;


            //3.load Column data
            vector<int> EmptyVector;
            EmptyVector.resize(0);

            if (DssDataTypeVarChar != (EnumDSSDataType)lColType && DssDataTypeBigDecimal != (EnumDSSDataType)lColType &&
                DssDataTypeMBChar != (EnumDSSDataType)lColType && DssDataTypeBinary != (EnumDSSDataType)lColType && DssDataTypeUTF8Char != (EnumDSSDataType)lColType)
            {
                hr = hLoadRawDataColumn(ipReadStream, lpColumn, (EnumDSSDataType)lColType, lnColumnRow, EmptyVector);
                CHECK_HR;
            }
            else
            {
                hr = hLoadStringDataColumn(ipReadStream, lpColumn, (EnumDSSDataType)lColType, lnColumnRow, EmptyVector);
                CHECK_HR;
            }

            vector<char> lvFlags;
            hr = ipReadStream->ReadByteArray(lvFlags);
            CHECK_HR;

            if (!lvFlags.empty())
            {
                hr = lpColumn->setFlags(0, lvFlags.size(), (DSSDataFlag*)(&lvFlags[0]), lvFlags.size() * sizeof(char));
                CHECK_HR;
            }

            (static_cast<DSSDataRefColumn*>(ipColumn))->AddLocalColumn(lvLocalGlobalStart[i], lpColumn);

        }


	}

	hr = ipReadStream->EndReadBlock();
	CHECK_HR;

	return 0;
}

int hLoadGlobalLookUpTable(DSSBinaryDataReader *ipReadStream, DSSTabularAttribute** ippTabAttr)
{
	int lBlockType = 0;
	int hr = ipReadStream->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockGlobalLookUpTable);

    int lArity;
    hr = ipReadStream->ReadInt(&lArity);
    CHECK_HR;

    int lnColumn;
	hr = ipReadStream->ReadInt(&lnColumn);
	CHECK_HR;

    vector <int> lvFormID;
    vector <int> lvFormType;

    lvFormID.resize(lnColumn);
    lvFormType.resize(lnColumn);

    for (int i = 0; i < lnColumn; i++)
    {
        hr = ipReadStream->ReadInt(&lvFormID[i]);
        CHECK_HR;
    }

    for (int i = 0; i < lnColumn; i++)
    {
        hr = ipReadStream->ReadInt(&lvFormType[i]);
        CHECK_HR;
    }

	for (int i = 0; i < lnColumn; i++)
	{
		//create ref column
		DSSDataColumn* lpRefColumn;

		//call ref column log logic
		hr = hLoadRefColumn(ipReadStream, lpRefColumn);
		CHECK_HR;


		(*ippTabAttr)->AddFormDataColumn(lvFormID[i], lvFormType[i], lpRefColumn);//lFormID=0, lFormType=0 for test
	}


	hr = ipReadStream->EndReadBlock();
	CHECK_HR;

	return hr;
}

int hLoadGlobalLookUpTables(DSSBinaryDataReader *ipReadStream)
{
	//create global lookup table
    DSSTabularAttribute *lpGlookup;
	int hr = S_OK;

	lpGlookup = new DSSGlobalLookUpTable;

	hr  = hLoadGlobalLookUpTable(ipReadStream, &lpGlookup);
	CHECK_HR;

	return hr;
}*/


size_t hSizeOf(EnumDSSDataType iDataType)
{
	switch (iDataType)
	{
		case DssDataTypeShort:
			return sizeof(short);
		case DssDataTypeInteger:
		case DssDataTypeLong:
			return sizeof(int);
		case DssDataTypeFloat:
			return sizeof(float);
		case DssDataTypeDouble:
			return sizeof(double);
		case DssDataTypeBool:
		case DssDataTypeMissing:
			return sizeof(bool);
		case DssDataTypeTime:
		case DssDataTypeDate:
		case DssDataTypeTimeStamp:
		case DssDataTypeCellFormatData:
			return sizeof(double);
		case DssDataTypeChar:
		case DssDataTypeReserved:
		case DssDataTypeUnknown:
			return sizeof(char);
        case DssDataTypeInt64:
            return sizeof(long long);
		default:
			return 0;
			break;
	}
}

int DSSModelPopulation::hBuildHeaderIndex(LayoutInfo *ipLayout, DSSDataModelXTabImpl *ipXTabModel)
{
	int hr = S_OK;

	int lnKeySize = (*ipLayout).vUnits.size();
	int *lpKeyUnits = &ipLayout->vUnits[0];

	if (lnKeySize == 0)
	{
		//TODO:
	}

	//1. initialize:
	int lnRows = (*ipLayout).mvLevelID.size();
	if (lnRows <= 0)
	{
		//409716, after UC selection, model may be empty.  temporary fix: return false
		return S_FALSE;		//TODO[enh]:
	}

	/*
	if (lnRows == 0)		//special handle
	{

		ipLayout->mTotalLevels = 1;
		ipLayout->mvLevelUnitsID.push_back(vector<int>());
		ipLayout->mvStartRow.push_back(0);		//TODO: check
		ipLayout->mvStartRow.push_back(1);

		DSSTabularIndex *lpIndex = NULL;
		hr = lpIndexPool->CreateIndex(0, 1, NULL, NULL, &lpIndex);
		CHECK_HR;

		vector<DSSTabularIndex *> lvIndices;
		lvIndices.push_back(lpIndex);

		DSSXTabView *lpXTabView = ipXTabModel->getXTabView();
		CHECK_PTR(lpXTabView);

		DSSXTabHeader *lpXTabHeader = NULL;
		hr = lpXTabView->getHeader(ipLayout->axis, &lpXTabHeader);
		CHECK_PTR(lpXTabHeader);

		int lMetricsUnitID = -1;
		if (ipLayout->metricsPos >= 0)
			lMetricsUnitID = ipLayout->vUnits[ipLayout->metricsPos];

		vector<int> lvSubtotalFlag;
		lvSubtotalFlag.push_back(0);
		hr = lpXTabHeader->Init(ipLayout->vUnits, lMetricsUnitID, 1, lvIndices, lvSubtotalFlag, ipXTabModel);
		CHECK_HR;

		return S_OK;
	}
*/

	int *lpRKeys = &((*ipLayout).mvKeys[0]);	//row header keys
	if (lnKeySize > 0 && lnRows != (*ipLayout).mvKeys.size() / lnKeySize)
		return E_FAIL;

	vector<int> *lpvOriginalRows = &ipLayout->mvOriginalRows;
	lpvOriginalRows->resize(lnRows);
	int i = 0;
	for (i = 0; i < lnRows; i++)
		(*lpvOriginalRows)[i] = i;

	//2. sort the original rows		TODO[opt]: use bucket sort
	ElementSort<int, int> lSorter(lpRKeys, lnRows, lnKeySize);
	int lMetricPos = (*ipLayout).metricsPos;
	if (lMetricPos >= 0)
	{
		lSorter.SetUnit(lMetricPos);	//sort by metrics first
		_xtab_sort(&(*lpvOriginalRows)[0], &(*lpvOriginalRows)[0] +  lnRows, lSorter);	//merge sort
	}
	for (i = lnKeySize - 1; i >= 0; --i)
	{
		if (i != lMetricPos)
		{
			lSorter.SetUnit(i);			//only sort by attributes
			_xtab_sort(&(*lpvOriginalRows)[0], &(*lpvOriginalRows)[0] +  lnRows, lSorter);	//merge sort
		}
	}
	short *lpLevelID = &((*ipLayout).mvLevelID[0]);
	unsigned char *lpSubtotalID = &(ipLayout->mvSubTotalID[0]);
	ElementSort<short, unsigned char> lLevelSorter(lpLevelID, lnRows, 1, lpSubtotalID);
	_xtab_sort(&(*lpvOriginalRows)[0], &(*lpvOriginalRows)[0] + lnRows, lLevelSorter);	//sort by level

/*	//merge the duplicate rows
	vector<int> *lpvUniqueRows = &(ipLayout->mvUniqueRows);
	lpvUniqueRows->reserve(lnRows);
	lpvUniqueRows->push_back((*lpvOriginalRows)[0]);

	int *lpPre = lpRKeys + lnKeySize * (*lpvOriginalRows)[0];
	int *lpCur = NULL;
	for (i = 1; i < lnRows; i++)
	{
		lpCur = lpRKeys + lnKeySize * (*lpvOriginalRows)[i];
		int j = 0;
		for (j = 0; j < lnKeySize; j++)
		{
			if (lpCur[j] != lpPre[j])
			{
				lpvUniqueRows->push_back((*lpvOriginalRows)[i]);
				break;
			}
		}
		lpPre = lpCur;
	}
	int lnUniqueRows = lpvUniqueRows->size();
*/

	//3. select distinct
	vector<int> *lpvDistinctRows = &(ipLayout->mvDistinctRows);
	lpvDistinctRows->reserve(lnRows);
	vector<int> lvRowRange;		//row rang of each level, based on distinct rows

	int *lpPre = NULL;
	int *lpCur = NULL;
	if (lnRows > 0)
	{
		lpPre = lpRKeys + lnKeySize * (*lpvOriginalRows)[0];
		lpvDistinctRows->push_back((*lpvOriginalRows)[0]);
		lvRowRange.push_back(0);
	}
	//unsigned char *lpSubtotalID = &(ipLayout->mvSubTotalID[0]);
	for (i = 1; i < lnRows; i++)
	{
		int lRowID = (*lpvOriginalRows)[i];
		lpCur = lpRKeys + lnKeySize * lRowID;
		for (int j = lnKeySize -1; j >= 0; --j)
		{
			if (lpCur[j] != lpPre[j] && j != lMetricPos)	//ignore metrics;
				//&& !(lpCur[j] > 0 && lpPre[j] > 0))	//ignore metric and ignore different subtoal
			{
				lpvDistinctRows->push_back(lRowID);

				//if (lpCur[j] < 0 || lpPre[j] < 0)			//the level (include subtotal type) is changed
				if (lpLevelID[lRowID] != lpLevelID[(*lpvOriginalRows)[i - 1]]
					|| lpSubtotalID[lRowID] != lpSubtotalID[(*lpvOriginalRows)[i - 1]])			//the level (include subtotal type) is changed
				{
					lvRowRange.push_back(lpvDistinctRows->size() - 1);
				}
				break;
			}
		}
		lpPre = lpCur;
	}
	int lnDistinctRows = lpvDistinctRows->size();
	lvRowRange.push_back(lnDistinctRows);
	ipLayout->mvStartRow.assign(lvRowRange.begin(), lvRowRange.end());	//TODO:

	//4. set each distinct row's offset within the level
	int lnLevels = lvRowRange.size() - 1;
	ipLayout->mTotalLevels = lnLevels;			//TODO: remove mTotalLevels
/*	vector<int> lvOffset(lnDistinctRows);
	int *lpOffset = &lvOffset[0];
	for (i = 0; i < lnLevels; i++)
	{
		int end = lvRowRange[i + 1];		//the start row of the next level
		int offset = 0;
		int j = 0;
		for (j = lvRowRange[i]; j < end; j++)
		{
			*lpOffset++ = offset++;
		}
	}*/

	//4. get the subtotal's local ID;
	vector<int> lvSubtotalFlag(lnLevels);
	for (i = 0; i < lnLevels; i++)
	{
		lvSubtotalFlag[i] = ipLayout->mvSubTotalID[(*lpvDistinctRows)[lvRowRange[i]]];
	}

	//5. get each level's units
	vector<vector<int> > lvLevelUnits(lnLevels);
	for (i = 0; i < lnLevels; i++)
	{
		int *lpKeys = lpRKeys + lnKeySize * (*lpvDistinctRows)[lvRowRange[i]];
		for (int j = lnKeySize - 1; j >= 0; j--)
		{
			if (lpKeys[j] >= 0 && j != lMetricPos)
				lvLevelUnits[i].push_back(lpKeyUnits[j]);
		}
		std::sort(lvLevelUnits[i].begin(), lvLevelUnits[i].end());
	}
	//TODO:
	ipLayout->mvLevelUnitsID.assign(lvLevelUnits.begin(), lvLevelUnits.end());

	//6. create index
	DSSCube *lpCube = ipXTabModel->getCube();
	CHECK_PTR(lpCube);

	DSSTabularIndexPool *lpIndexPool = NULL;
	lpCube->getIndexPool(&lpIndexPool);
	CHECK_PTR(lpIndexPool);

	vector<DSSTabularIndex *> lvIndices (lnLevels);
	for (i = 0; i < lnLevels; i++)
	{
		int lnIndexUnits = lvLevelUnits[i].size();
		if (i > 0 && lnIndexUnits == lvLevelUnits[i - 1].size())		//check if only subtotal type is different
		{
			int k = 0;
			for (k = 0; k < lnIndexUnits; k++)
			{
				if (lvLevelUnits[i][k] != lvLevelUnits[i - 1][k])
					break;
			}
			if (k >= lnIndexUnits)	//same index
			{
				lvIndices[i] = lvIndices[i - 1];
				continue;
			}
		}
		vector<int> lvIndexUnitsMap;
		hr = LinearMapUnits(lvLevelUnits[i], ipLayout->vUnits, lvIndexUnitsMap);
		CHECK_HR;

		int start = lvRowRange[i];
		int end = lvRowRange[i + 1];

		vector<int> lvKeys;
		lvKeys.resize(lnIndexUnits * (end - start));

		int *lpKeys = lnIndexUnits > 0 ? &lvKeys[0] : NULL;
		for (int j = start; j < end; j++)
		{
			int *lpK = lpRKeys + lnKeySize * (*lpvDistinctRows)[j];
			for (int k = 0; k < lnIndexUnits; k++)
			{
				*lpKeys++ = lpK[lvIndexUnitsMap[k]];
			}
		}
		lpKeys = lnIndexUnits > 0 ? &lvKeys[0] : NULL;

		DSSTabularIndex *lpIndex = NULL;
		hr = lpIndexPool->CreateIndex(lnIndexUnits, end - start, &lvLevelUnits[i][0], lpKeys, &lpIndex);
		CHECK_HR;

		lvIndices[i] = lpIndex;
	}
	//ipLayout->mvIndices.assign(lvIndices.begin(), lvIndices.end());		//TODO:

	DSSXTabView *lpXTabView = ipXTabModel->getXTabView();
	CHECK_PTR(lpXTabView);

	DSSXTabHeader *lpXTabHeader = lpXTabView->getHeader(ipLayout->axis);
	CHECK_PTR(lpXTabHeader);

	int lMetricsUnitID = -1;
	if (ipLayout->metricsPos >= 0)
		lMetricsUnitID = ipLayout->vUnits[ipLayout->metricsPos];

	hr = lpXTabHeader->Init(ipLayout->vUnits, ipLayout->mnRuntimeUnits, lMetricsUnitID, lnDistinctRows, lvIndices, lvSubtotalFlag, ipXTabModel);
	CHECK_HR;

	//at last, we need a map from original rows to the distinct rows
	ipLayout->mvMap2DistinctRows.resize(lnRows);
	int *lpMap = NULL;
	if (lnRows > 0)
		lpMap = &ipLayout->mvMap2DistinctRows[0];

	i = 0;
	int j = 1;
	while(i < lnRows && j < lnDistinctRows)
	{
		int next = (*lpvDistinctRows)[j];
		int cur = (*lpvOriginalRows)[i];
		while (cur != next && i < lnRows)
		{
			lpMap[cur] = j - 1;
			cur = (*lpvOriginalRows)[++i];
		}
		j++;
	}
	while (i < lnRows)
	{
		lpMap[(*lpvOriginalRows)[i++]] = j - 1;
	}

	//we need a map for to display rows
	if (ipLayout->metricsPos >= 0)
	{
		DSSTabularMetrics *lpMetrics = NULL;
		ipXTabModel->getMetricsUnit(&lpMetrics);
		CHECK_PTR(lpMetrics);
		int lnMetrics = lpMetrics->Count();

		ipLayout->mvMap2DisplayedRows.resize(lnRows);
		int *lpMap = NULL;
		if (lnRows > 0)
			lpMap = &ipLayout->mvMap2DisplayedRows[0];

		i = 0;
		if (lnDistinctRows > 1)
		{
			int j = 1;
			while(i < lnRows && j < lnDistinctRows)
			{
				int next = (*lpvDistinctRows)[j];
				int cur = (*lpvOriginalRows)[i];
				int iMetric = 0;
				while (cur != next && i < lnRows)
				{
					lpMap[cur] = (j - 1) * lnMetrics + iMetric++ % lnMetrics;
					cur = (*lpvOriginalRows)[++i];
				}
				//if (iMetric > lnMetrics)
				//	return E_FAIL;			//it should not happen
				j++;
			}
			int iMetric = 0;
			while (i < lnRows)
			{
				lpMap[(*lpvOriginalRows)[i++]] = (j - 1) * lnMetrics + iMetric++ % lnMetrics;
			}
		}
		else if (lnMetrics > 0)	//the header only has metrics or there's only one distinct row
		{
			int lnRecurrence = lnRows / lnMetrics;
			if (lnMetrics * lnRecurrence == lnRows)
			{
				int iMetric = 0;
				while (i < lnRows)
				{
					for (int ir = 0; ir < lnRecurrence; ir++)
					{
						lpMap[(*lpvOriginalRows)[i++]] = iMetric;
					}
					iMetric++;
				}
			}
		}
	}
	else
		ipLayout->mvMap2DisplayedRows.assign(ipLayout->mvMap2DistinctRows.begin(), ipLayout->mvMap2DistinctRows.end());
	/*
	 vector<int> *lpvOffset = &(*ipLayout).mvOffset;		//TODO: retire mvOffset
	 lpvOffset->resize(lnDistinctRows);		//TODO[enh]: verify by Index->LookUP
	 for (i = 0; i <lnDistinctRows; i++)
	 (*lpvOffset)[i] = -1;

	 //TODO: retire mvStartRow, it's replaced by lvRowRange
	(*ipLayout).mvStartRow.reserve((*ipLayout).mTotalLevels + 1);
	for (i = 0; i < lnDistinctRows;)
	{
		(*ipLayout).mvStartRow.push_back(i);

		int level = (*ipLayout).mvLevelID[(*lpvDistinctRows)[i]];

		int offset = 0;
		(*lpvOffset)[(*lpvDistinctRows)[i]] = offset;
		++i;
		while (i < lnDistinctRows && (*ipLayout).mvLevelID[(*lpvDistinctRows)[i]] == level)
		{
			(*lpvOffset)[(*lpvDistinctRows)[i]] = ++offset;
			++i;
		}
	}
	(*ipLayout).mvStartRow.push_back(lnDistinctRows);

	//create index
	for (i = 0; i < ipLayout->mTotalLevels; i++)
	{
		int start = ipLayout->mvStartRow[i];
		int end = ipLayout->mvStartRow[i+1];

		//create an index
		int lLevelID = (*ipLayout).mvLevelID[(*lpvDistinctRows)[start]];
		vector<int> *lpvUnitsID = &(*ipLayout).mvLevelUnitsID[lLevelID];
		int lnIndexUnits = lpvUnitsID->size();
		vector<int> lvIndexUnitsMap(lnIndexUnits);

		int j = 0, k = 0;
		for (j = 0; j < lnIndexUnits; j++)
		{
			for (k = 0; k < lnUnits; k++)
			{
				if ((*ipLayout).vUnits[k] == (*lpvUnitsID)[j])
				{
					lvIndexUnitsMap[j] = k;
					break;
				}
			}
			if (k >= lnUnits)
				return E_FAIL;
		}

		vector<int> lvKeys;
		lvKeys.resize(lnIndexUnits * (end - start));
		int *lpKeys = &lvKeys[0];
		for (j = start; j < end; j++)
		{
			for (k = 0; k < lnIndexUnits; k++)
			{
				*lpKeys++ = lpRKeys[(*lpvDistinctRows)[j] * lnUnits + lvIndexUnitsMap[k]];
			}
		}
		lpKeys = &lvKeys[0];

		DSSTabularIndexPool *lpIndexPool = (DSSTabularIndexPool*)(*ipLayout).mpIndexPool;
		DSSTabularIndex *lpIndex = NULL;
		hr = lpIndexPool->CreateIndex(lnIndexUnits, end - start, &(*lpvUnitsID)[0], lpKeys, &lpIndex);
		CHECK_HR;

		ipLayout->mvIndices[lLevelID] = lpIndex;
	}


	int offset = (*lpvOffset)[0];
	for (i = 1; i <lnRows; i++)
	{
		if ((*lpvOffset)[i] < 0)		//the selected distinct row is the first of several duplicate rows
			(*lpvOffset)[i] = offset;
		else
			offset = (*lpvOffset)[i];
	}*/

	return 0;
}

int hBuildCellIndex(LayoutInfo *ipLayout, DSSDataModelXTabImpl *ipXTabModel, vector<char> &irvbXTabNull, vector<DSSTabularIndex*> &orCellIndices)
{
	int hr = S_OK;

	DSSCube *lpCube = ipXTabModel->getCube();
	CHECK_PTR(lpCube);

	DSSTabularIndexPool *lpIndexPool = NULL;
	hr = lpCube->getIndexPool(&lpIndexPool);
	CHECK_PTR(lpIndexPool);

	int lnRowLevels = ipLayout[0].mTotalLevels;
	int lnColLevels = ipLayout[1].mTotalLevels;

	//* TODO: delay loading...
	bool mbDelayLoading = true;
	if (mbDelayLoading)
	{
		orCellIndices.resize(lnRowLevels *lnColLevels);
		return S_OK;
	}

	orCellIndices.reserve(lnRowLevels *lnColLevels);		//TODO: add it to DSSXTabView and remove it from parameters

	//int lnRows = ipLayout[0].mvDistinctRows.size();
	int lnColumns = ipLayout[1].mvDistinctRows.size();

	int lnRowUnits = ipLayout[0].vUnits.size();
	int lnColUnits = ipLayout[1].vUnits.size();

	int iRowLevel = 0, iColLevel = 0;
	for (iRowLevel = 0; iRowLevel < lnRowLevels; iRowLevel++)
	{
		int startR = ipLayout[0].mvStartRow[iRowLevel];
		int endR = ipLayout[0].mvStartRow[iRowLevel + 1];

		//DSSTabularIndex *lpRowIndex = (DSSTabularIndex*)ipLayout[0].mvIndices[iRowLevel];
		//int lnRowUnits = 0;
		//hr = lpRowIndex->getNumUnits(lnRowUnits);
		//CHECK_HR;

		//vector<int> lvRowUnits(lnRowUnits > 0 ? lnRowUnits : 1);
		//hr = lpRowIndex->getUnits(lnRowUnits, &lvRowUnits[0]);
		//CHECK_HR;

		vector<int> *lpvRowUnits = &ipLayout[0].mvLevelUnitsID[iRowLevel];
		int lnRowLevelUnits = lpvRowUnits->size();
		int *lpRowKeyTable = &ipLayout[0].mvKeys[0];

		for (iColLevel = 0; iColLevel < lnColLevels; iColLevel++)
		{
			int startC = ipLayout[1].mvStartRow[iColLevel];
			int endC = ipLayout[1].mvStartRow[iColLevel + 1];

			//DSSTabularIndex *lpColIndex = (DSSTabularIndex*)ipLayout[1].mvIndices[iColLevel];

			int i = 0, j = 0, k = 0;
			bool lbHasXTabNull = false;

			if (!irvbXTabNull.empty())
			{
				for (i = startR; i < endR; i++)
				{
					for (j = startC; j < endC; j++)
					{
						if (irvbXTabNull[i * lnColumns + j] != 0)
						{
							lbHasXTabNull = true;
							break;
						}
					}
				}
			}

			//build key table
			vector<int> *lpvColUnits = &ipLayout[1].mvLevelUnitsID[iColLevel];
			int lnColLevelUnits = lpvColUnits->size();
			int *lpColKeyTable = &ipLayout[1].mvKeys[0];

			vector<int> lvUnits;
			lvUnits.insert(lvUnits.end(), lpvRowUnits->begin(), lpvRowUnits->end());
			lvUnits.insert(lvUnits.end(), lpvColUnits->begin(), lpvColUnits->end());
			std::sort(lvUnits.begin(), lvUnits.end());
			int lnUnits = lvUnits.size();

			if (lnUnits != lnRowLevelUnits + lnColLevelUnits)
				return E_FAIL;
			//TODO: move groupby units to the left most
			vector<int> lvRowUnitsMap(lnRowLevelUnits);
			for (i = 0; i < lnRowLevelUnits; i++)
			{
				for (j = 0; j < lnUnits; j++)
				{
					if (lvUnits[j] == (*lpvRowUnits)[i])
					{
						lvRowUnitsMap[i] = j;
						break;
					}
				}
			}

			vector<int> lvRowKeysMap(lnRowLevelUnits);
			for (i = 0; i < lnRowLevelUnits; i++)
			{
				for (j = 0; j < ipLayout[0].vUnits.size(); j++)
				{
					if (ipLayout[0].vUnits[j] == (*lpvRowUnits)[i])
					{
						lvRowKeysMap[i] = j;
						break;
					}
				}
			}

			vector<int> lvColUnitsMap(lnColLevelUnits);
			for (i = 0; i < lnColLevelUnits; i++)
			{
				for (j = 0; j < lnUnits; j++)
				{
					if (lvUnits[j] == (*lpvColUnits)[i])
					{
						lvColUnitsMap[i] = j;
						break;
					}
				}
			}

			vector<int> lvColKeysMap(lnColLevelUnits);
			for (i = 0; i < lnColLevelUnits; i++)
			{
				for (j = 0; j < ipLayout[1].vUnits.size(); j++)
				{
					if (ipLayout[1].vUnits[j] == (*lpvColUnits)[i])
					{
						lvColKeysMap[i] = j;
						break;
					}
				}
			}

			vector<int> lvKeys;
			if (!lbHasXTabNull)
				lvKeys.reserve(lnUnits * (endR - startR) * (endC - startC));

			vector<int> lvKeyBuffer(lnUnits);
			int lnIndexRows = 0;
			for (i = startR; i < endR; i++)
			{
				int *lpRowKeys = &lpRowKeyTable[ipLayout[0].mvDistinctRows[i] * lnRowUnits];//TODO[opt]: using index's key table and units map
				//fill row keys first
				for (k = 0; k < lnRowLevelUnits; k++)
				{
					lvKeyBuffer[lvRowUnitsMap[k]] = lpRowKeys[lvRowKeysMap[k]];
				}

				for (j = startC; j < endC; j++)
				{
					int *lpColKeys = &lpColKeyTable[ipLayout[1].mvDistinctRows[j] * lnColUnits];
					if (!lbHasXTabNull || irvbXTabNull[i * lnColumns + j] == 0)
					{
						//fill col keys first
						for (k = 0; k < lnColLevelUnits; k++)
						{
							lvKeyBuffer[lvColUnitsMap[k]] = lpColKeys[lvColKeysMap[k]];
						}
						lvKeys.insert(lvKeys.end(), lvKeyBuffer.begin(), lvKeyBuffer.end());
						lnIndexRows++;
					}
				}
			}
			int *lpKeys = &lvKeys[0];

			//create an index for cell
			DSSTabularIndex *lpIndex = NULL;
			hr = lpIndexPool->CreateIndex(lnUnits, lnIndexRows, &lvUnits[0], lpKeys, &lpIndex);
			CHECK_HR;

			orCellIndices.push_back(lpIndex);
		}
	}
	//*/
	return 0;
}

int hCreateDataSlices(DSSDataModelXTabImpl *ipXTabModel, vector<DSSTabularIndex*> &irCellIndices,
									   vector<int> &irMetricDataType, vector<vector<int> > &irTotalDatatype, bool ibHasXTabNull)
{
	int hr = S_OK;

	//build slicing map
	DSSXTabView *lpXTabView = ipXTabModel->getXTabView();
	CHECK_PTR(lpXTabView);

	DSSXTabHeader *lpRowHeader = lpXTabView->getHeader(0);
	CHECK_PTR(lpRowHeader);

	int lnRowLevels = lpRowHeader->getLevelCount();

	DSSXTabHeader *lpColHeader = lpXTabView->getHeader(1);
	CHECK_PTR(lpColHeader);

	int lnColLevles = lpColHeader->getLevelCount();

	int lnMetrics = irMetricDataType.size();
	int lnSubtotals = irTotalDatatype.size();

	lpXTabView->CreateSliceMap(lnRowLevels, lnColLevles, lnMetrics);

	DSSCube *lpCube = ipXTabModel->getCube();
	CHECK_PTR(lpCube);


	for (int i = 0; i < lnRowLevels; i++)
	{
		DSSTabularIndex *lpRowIndex = NULL;
		lpRowHeader->getIndexByLevel(i, &lpRowIndex);
		CHECK_PTR(lpRowIndex);

		int lnRowIndexSize = lpRowIndex->Count();

		int lRowTotalID = 0;
		hr = lpRowHeader->getSubtotalIDByLevel(i, lRowTotalID);
		CHECK_HR;

		for (int j = 0; j < lnColLevles; j++)
		{
			DSSTabularIndex *lpColIndex = NULL;
			hr = lpColHeader->getIndexByLevel(j, &lpColIndex);
			CHECK_HR;

			int lnColIndexSize = lpColIndex->Count();

			int lColTotalID = 0;
			hr = lpColHeader->getSubtotalIDByLevel(j, lColTotalID);
			CHECK_HR;

			if (lRowTotalID > 0 && lColTotalID > 0 && lRowTotalID != lColTotalID)	//different subtotals
				continue;
			if (lRowTotalID > lnSubtotals || lColTotalID > lnSubtotals)		//lRowTotalID is 1-based
				return E_FAIL;

			for (int k = 0; k < lnMetrics; k++)
			{
				int lDataType = irMetricDataType[k];

				if (lRowTotalID > 0)
					lDataType = irTotalDatatype[lRowTotalID - 1][k];
				else if (lColTotalID > 0)
					lDataType = irTotalDatatype[lColTotalID - 1][k];

				DSSDataColumn *lpColumn = NULL;
                long long lColSize = ((long long)lnRowIndexSize)* lnColIndexSize;
				hr = DSSAEHelper::GenXTabCol((EnumDSSDataType)lDataType, lColSize, &lpColumn, true, ibHasXTabNull);
				CHECK_HR;
				CHECK_PTR(lpColumn);

				DSSCubeDataSlice *lpSlice = NULL;
				lpCube->CreateDataSlice(irCellIndices[i * lnColLevles + j], lpColumn, &lpSlice);
				CHECK_PTR(lpSlice);

				int lSliceID = 0;
				hr = lpCube->InsertDataSlice(lpSlice, lSliceID);
				CHECK_HR;

				hr = lpXTabView->AddSlice(lSliceID, i * lnColLevles + j, k);
				CHECK_HR;
			}
		}
	}

	/*	int lnRowLevels = ipLayout[0].mTotalLevels;
	 int lnColLevles = ipLayout[1].mTotalLevels;
	 int lnMetrics = irMetricDataType.size();
	 int lnSubtotals = irTotalDatatype.size();
	 if (lnSubtotals == 0)
	 lnSubtotals = 1;

	 orDataSlice.resize(lnRowLevels * lnColLevles * lnMetrics * lnSubtotals);

	 //create report levle slices
	 int i = 0;
	 int lnRowIndexSize = ipLayout[0].mvIndices[0]->Count();
	 int lnColIndexSize = ipLayout[1].mvIndices[0]->Count();

	 for (i = 0; i < lnMetrics; i++)
	 {
	 DSSDataColumn *lpColumn = NULL;
	 hr = DSSAEHelper::GenXTabCol((EnumDSSDataType)irMetricDataType[i], lnRowIndexSize * lnColIndexSize, &lpColumn);
	 CHECK_HR;

	 DSSCubeDataSlice *lpSlice = NULL;
	 hr = ipCube->CreateDataSlice(irCellIndices[0], lpColumn, &lpSlice);
	 CHECK_HR;

	 orDataSlice[i] = lpSlice;
	 }

	 int count = lnMetrics;
	 for (i = 1; i < lnRowLevels; i++)
	 {
	 lnRowIndexSize = ipLayout[0].mvIndices[i]->Count();

	 for (int j = 1; j < lnColLevles; j++)
	 {
	 lnColIndexSize = ipLayout[1].mvIndices[j]->Count();

	 for (int k = 0; k < lnMetrics; k++)
	 {
	 for (int m = 0; m < lnSubtotals; m++)
	 {
	 DSSDataColumn *lpColumn = NULL;
	 hr = DSSAEHelper::GenXTabCol((EnumDSSDataType)irTotalDatatype[m][k], lnRowIndexSize * lnColIndexSize, &lpColumn);
	 CHECK_HR;

	 DSSCubeDataSlice *lpSlice = NULL;
	 hr = ipCube->CreateDataSlice(irCellIndices[i * lnColLevles + j], lpColumn, &lpSlice);

	 orDataSlice[count++] = lpSlice;
	 }
	 }
	 }
	 }*/
	return S_OK;
}

int DSSModelPopulation::hSetMetricCell(DSSXTabView *ipXTabView, int iMetricID, int iSubtotalID, int nUnits, int *ipKey,
									int iCellOffset, int iRowLevel, int iColLevel,
									vector<int> &irvMetricDataType, vector<vector<int> > &irvTotalDatatype,
									vector<char> &irvCellFlags, vector<int> &irvThresholdID, vector<int> &irvThresholdMetricID)
{
	EnumDSSDataType lCellDataType;
	vector<unsigned char> lvBuffer;
	int lCellFmtType = 0, lCellFmtID = 0;
	int hr = hLoadCells(iMetricID, iSubtotalID, irvMetricDataType, irvTotalDatatype, lCellDataType, lvBuffer, lCellFmtType, lCellFmtID);
	CHECK_HR;

	DSSCubeDataSlice *lpSlice = NULL;
	hr = ipXTabView->getDataSlice(iRowLevel, iColLevel, iMetricID, &lpSlice);
	CHECK_HR;
	if (!lpSlice)	//411031, xtab null
		return S_OK;

	EnumDSSDataFlag lFlag = DssDataOk;
	if (!irvCellFlags.empty())
		lFlag = static_cast<EnumDSSDataFlag>(irvCellFlags[iCellOffset]);

	int lThresholdID = 0;
	if (!irvThresholdID.empty())
		lThresholdID = irvThresholdID[iCellOffset];

	if(lCellDataType== DssDataTypeCellFormatData)
	{
		double lValue=*(reinterpret_cast<double *> (&lvBuffer[0]));
		MDataType::DSSCellFmtData lCellFmtData(lValue, lCellFmtType, lCellFmtID);
		lpSlice->setValueAndThreshold(nUnits, ipKey, reinterpret_cast<char*> (&lCellFmtData), sizeof(MDataType::DSSCellFmtData), lCellDataType, lFlag, lThresholdID);
	}else if (lCellDataType==DssDataTypeBigDecimal)
	{
		MDataType::DSSBigDecimal lBigDecimal(reinterpret_cast<char*> (&lvBuffer[0]));
		lpSlice->setValueAndThreshold(nUnits, ipKey, reinterpret_cast<char*> (&lBigDecimal), sizeof(MDataType::DSSBigDecimal), lCellDataType, lFlag, lThresholdID);
	}
	else if (lCellDataType <	DssDataTypeDate || lCellDataType > DssDataTypeTimeStamp)
	{
		 lpSlice->setValueAndThreshold(nUnits, ipKey, reinterpret_cast<char*> (&lvBuffer[0]), lvBuffer.size(), lCellDataType, lFlag, lThresholdID);
	}
	else
	{
		MDataType::DSSDateTime lTmp(*((double*)&lvBuffer[0])/(double)MDataType::DAY_IN_MILLISECONDS + gSecondsSince1970);
		 lpSlice->setValueAndThreshold(nUnits, ipKey, (char*)&lTmp, sizeof(MDataType::DSSDateTime), lCellDataType, lFlag, lThresholdID);
	}
	return S_OK;
}

int hSetMetricCellByOffset(DSSBinaryDataReader *ipReadStream, DSSXTabView *ipXTabView, int iRowLevel, int iColLevel, int iMetricID,
											long long offset, vector<unsigned char> &rvBuffer, EnumDSSDataFlag iCellFlags, int iThresholdID)
{
	int hr = S_OK;
	DSSCubeDataSlice *lpSlice = NULL;
	hr = ipXTabView->getDataSlice(iRowLevel, iColLevel, iMetricID, &lpSlice);
	CHECK_HR;

	if (!lpSlice)	//TODOD: test 411031, xtab null
	{
		return ipReadStream->Skip(sizeof(bool));
	}

	EnumDSSDataType lCellDataType = lpSlice->getDataType();
	int lSizeInByte = 0;
	int lCellFmtType = 0, lCellFmtID = 0;
	hr = hLoadCells(ipReadStream, lCellDataType, rvBuffer, lSizeInByte, lCellFmtType, lCellFmtID);
	CHECK_HR;

	if(lCellDataType== DssDataTypeCellFormatData)
	{
        //zhyang, use new way to deal with cell format
		double lValue=*(reinterpret_cast<double *> (&rvBuffer[0]));
        GUID lCellFormatGUID;
        ipXTabView->getXTabModel()->getCellFmtIDs(lCellFmtID, lCellFormatGUID);
		//MDataType::DSSCellFmtData lCellFmtData(lValue, lCellFmtType, lCellFmtID);
        MDataType::DSSCellFmtData lCellFmtData(lValue, lCellFmtType, lCellFormatGUID);
		return lpSlice->setValueAndThresholdByOffset(offset, reinterpret_cast<char*> (&lCellFmtData), sizeof(MDataType::DSSCellFmtData), lCellDataType, iCellFlags, iThresholdID);
	}
	else if (lCellDataType==DssDataTypeBigDecimal)
	{
		MDataType::DSSBigDecimal lBigDecimal(reinterpret_cast<char*> (&rvBuffer[0]));
		return lpSlice->setValueAndThresholdByOffset(offset, reinterpret_cast<char*> (&lBigDecimal), sizeof(MDataType::DSSBigDecimal), lCellDataType, iCellFlags, iThresholdID);
	}
	else if (lCellDataType < DssDataTypeDate || lCellDataType > DssDataTypeTimeStamp)
	{
		return lpSlice->setValueAndThresholdByOffset(offset, reinterpret_cast<char*> (&rvBuffer[0]), lSizeInByte, lCellDataType, iCellFlags, iThresholdID);
	}
	else
	{
		MDataType::DSSDateTime lTmp(*((double*)&rvBuffer[0])/(double)MDataType::DAY_IN_MILLISECONDS + gSecondsSince1970);
		return lpSlice->setValueAndThresholdByOffset(offset, (char*)&lTmp, sizeof(MDataType::DSSDateTime), lCellDataType, iCellFlags, iThresholdID);
	}
}

int hLoadControlElementList(DSSBinaryDataReader *mpReadStream, DSSDataModelControlImpl *ipModel)
{
	int hr = S_OK;
	int lnElements = 0;
	hr = mpReadStream->ReadInt(&lnElements);
	CHECK_HR;

	hr = ipModel->Init(lnElements);
	CHECK_HR;

	for (int i = 0; i < lnElements; i++)
	{
		int lobjtype = 0;
		hr = mpReadStream->ReadInt(&lobjtype);
		CHECK_HR;

		if (lobjtype != EnumObjectControlElement)
			return E_UNEXPECTED_DATATYPE;

		MBase::CString lElementID;
		hr = mpReadStream->ReadString(lElementID);
		CHECK_HR;

		MBase::CString lDisplayText;
		hr = mpReadStream->ReadString(lDisplayText);
		CHECK_HR;

		int index = 0;
		hr= mpReadStream->ReadInt(&index);
		CHECK_HR;

		//TODO: special handle total element "lElementID"
		hr = ipModel->AddElement(index, lDisplayText, lElementID);
		CHECK_HR;
	}
	return S_OK;
}

int hLoadStaticFields(DSSBinaryDataReader *mpReadStream, DSSDataModelFieldGroupImpl *ipFieldGroupModel)
{
	int lnFields = 0;
	int hr = mpReadStream->ReadInt(&lnFields);
	CHECK_HR;

	hr = ipFieldGroupModel->Init(0, lnFields);
	CHECK_HR;

	int i = 0;
	for (i = 0; i < lnFields; i++)
	{
		int lnSubFields = 0;
		hr = mpReadStream->ReadInt(&lnSubFields);
		CHECK_HR;

		vector<MBase::CString *> lvSubFields(lnSubFields);
		for (int j = 0; j < lnSubFields; j++)
		{
			MBase::CString *lpText = new MBase::CString;
			mpReadStream->ReadString(*lpText);

			lvSubFields[j] = lpText;
		}

		ipFieldGroupModel->AddStaticField(i, lvSubFields);
	}

	for (i = 0; i < lnFields; i++)
	{
		int lnLinks = 0;
		hr = mpReadStream->ReadInt(&lnLinks);
		CHECK_HR;

		vector<vector<MBase::CString *> > lvSubLinkText(lnLinks);
		for (int j = 0; j < lnLinks; j++)
		{
			int lnSubLinks = 0;
			hr = mpReadStream->ReadInt(&lnSubLinks);
			CHECK_HR;

			lvSubLinkText[j].resize(lnSubLinks);
			for (int k = 0; k < lnSubLinks; k++)
			{
				MBase::CString *lpText = new MBase::CString;
				mpReadStream->ReadString(*lpText);

				lvSubLinkText[j][k] = lpText;
			}

		}
        ipFieldGroupModel->AddStaticFieldLink(i, lvSubLinkText);
	}
	return S_OK;
}

int hLoadFields2(DSSBinaryDataReader *mpReadStream, DSSDataModelFieldGroupImpl *ipFieldGroupModel, int* ipOffset, int inOffset, int lFieldDataEndOffset, vector<int> &irvGBPaths, bool ibHasStaticFieldLookup, bool ibTransactionEnabled)
{
	int lnRows = 0;
	int hr = mpReadStream->ReadInt(&lnRows);
	CHECK_HR;
	
	if (lnRows <= 0)
		return S_OK;
	
	int lnGBUnits = 0;
	hr = mpReadStream->ReadInt(&lnGBUnits);
	CHECK_HR;
	
	int lnFields = 0;
	hr = mpReadStream->ReadInt(&lnFields);
	CHECK_HR;
	
	hr = ipFieldGroupModel->Init(lnRows, lnFields);
	CHECK_HR;
	
    ipFieldGroupModel->setHasStaticFieldLookup(ibHasStaticFieldLookup);
    
    hr = mpReadStream->ReadIntArray(irvGBPaths);
    CHECK_HR;
    
    _ASSERT(irvGBPaths.size() == lnRows * lnGBUnits);
    
    _ASSERT(inOffset == lnRows);
    
    bool lbDelayLoading = !ibTransactionEnabled;
    //delay loading
    //if(lbDelayLoading)
    {
        int lStreamOffset = ipFieldGroupModel->getModelDataSource()->getFileOffset();	//mpReadStream's offset in the file
        
        int lStart = mpReadStream->GetPosition();		//slice's offset within the read stream
        int lEnd = lFieldDataEndOffset;
        MBase::CString lLocalPath;
        int hr = DataSourceUtil::getLocalPath(lLocalPath);
        CHECK_HR;
        
        DSSDataSource *lpDS = new DSSDataSource();
        
        if (lLocalPath.find(ipFieldGroupModel->getDataSourcePath()) == lLocalPath.npos)
        {				
            hr = lpDS->Init(mpReadStream, lStreamOffset, lStart, lEnd, lnRows,
                            ipOffset, inOffset, ipFieldGroupModel->getDataSourcePath(), false);
            CHECK_HR;
        }
        else
        {
            MBase::CString lNodeKey = ipFieldGroupModel->getNodeKey();
            std::string lSubFolder = ipFieldGroupModel->getSubFolder();
            lLocalPath = ipFieldGroupModel->getDataSourcePath() + "/" + lSubFolder + "/" + lNodeKey;
            hr = lpDS->Init(mpReadStream, lStreamOffset, lStart, lEnd, lnRows,
                            ipOffset, inOffset, lLocalPath, true, true);
            CHECK_HR;
        }
        
        hr = ipFieldGroupModel->setDataSource(lpDS);
        CHECK_HR;
    }
    if(!lbDelayLoading)
    {
        vector<int> lvRows(lnRows);
        for(int i = 0; i< lnRows; i++)
            lvRows[i] = i;
        ipFieldGroupModel->LoadData(lvRows);
    }

	return S_OK;
}

int hLoadFields(DSSBinaryDataReader *mpReadStream, DSSDataModelFieldGroupImpl *ipFieldGroupModel, vector<int> &irvGBPaths, bool ibHasStaticFieldLookup)
{
	int lnRows = 0;
	int hr = mpReadStream->ReadInt(&lnRows);
	CHECK_HR;

	if (lnRows <= 0)
		return S_OK;

	int lnGBUnits = 0;
	hr = mpReadStream->Peek((unsigned char*)&lnGBUnits, sizeof(int), sizeof(int));
	CHECK_HR;

	vector<int> lBuffer(1 + lnGBUnits + 1);
	hr = mpReadStream->Peek((unsigned char*)&lBuffer[0], (1	//lnGBUnits
							+ lnGBUnits
							+ 1) * sizeof(int), sizeof(int));				//lnFields
	CHECK_HR;

	int lnFields = lBuffer[lnGBUnits + 1];
	hr = ipFieldGroupModel->Init(lnRows, lnFields);
	CHECK_HR;

	irvGBPaths.resize(lnRows * lnGBUnits);
	int count = 0;
	for (int i = 0; i < lnRows; i++)
	{
		int lSize = 0;
		hr = mpReadStream->ReadInt(&lSize);
		CHECK_HR;

		if (lSize == lnGBUnits)
		{
			if (lnGBUnits > 0)
			{
				hr = mpReadStream->ReadIntArray(&irvGBPaths[i * lnGBUnits], lnGBUnits);
				CHECK_HR;
			}

			hr = mpReadStream->ReadInt(&lSize);
			CHECK_HR;

			if (lSize == lnFields)
			{
				for (int f = 0; f < lnFields; f++)
				{
					int lobjtype = 0;
					hr = mpReadStream->ReadInt(&lobjtype);
					CHECK_HR;

					if (lobjtype != EnumObjectField)
						return E_UNEXPECTED_DATATYPE;

					int lThresholdID = 0;
					hr = mpReadStream->ReadInt(&lThresholdID);
					CHECK_HR;

					if (ibHasStaticFieldLookup)
					{
						int lnSubFields = 0;
						hr = mpReadStream->ReadInt(&lnSubFields);
						CHECK_HR;

						vector<MBase::CString *> lvSubFields(lnSubFields);
						for (int j = 0; j < lnSubFields; j++)
						{
							MBase::CString *lpText = new MBase::CString;
							mpReadStream->ReadString(*lpText);

							lvSubFields[j] = lpText;
						}

						ipFieldGroupModel->AddField(count, f, lvSubFields, lThresholdID);
					}
					else
					{
						MBase::CString *lpText = new MBase::CString;
						hr = mpReadStream->ReadString(*lpText);
						CHECK_HR;

						hr = ipFieldGroupModel->AddField(count, lpText, lThresholdID);
						if (hr != S_OK)
							delete lpText;
						CHECK_HR;
					}

					int lnLinks = 0;
					hr = mpReadStream->ReadInt(&lnLinks);
					CHECK_HR;

					if (lnLinks == 0)
					{
						hr = mpReadStream->ReadInt(&lSize);
						CHECK_HR;
					}
					else if (lnLinks > 0)
					{
						FieldLink *lpLink = new FieldLink(lnLinks);
						hr = mpReadStream->ReadIntArray((int*)(&lpLink->mtype[0]), lnLinks);
						CHECK_HR;

						MBase::DSSStrongPtr<FieldLink> lpLinkHolder(lpLink);

						hr = mpReadStream->ReadInt(&lSize);
						CHECK_HR;

						if (ibHasStaticFieldLookup)
						{
							vector<vector<MBase::CString *> > lvLinkText(lnLinks);
							for (int k = 0; k < lnLinks; k++)
							{
								int lnSubLinks = 0;
								hr = mpReadStream->ReadInt(&lnSubLinks);
								CHECK_HR;

								lvLinkText[k].resize(lnSubLinks);
								for (int s = 0; s < lnSubLinks; s++)
								{
									MBase::CString *lpText = new MBase::CString;
									mpReadStream->ReadString(*lpText);

									lvLinkText[k][s] = lpText;
								}

							}
							hr = ipFieldGroupModel->AddLink(count, f, lpLink, lvLinkText);
							CHECK_HR;
						}
						else
						{
							for (int k = 0; k < lnLinks; k++)
							{
								MBase::CString *lpLinkText = new MBase::CString;
								hr = mpReadStream->ReadString(*lpLinkText);
								CHECK_HR;
								//link text
								//TODO:[test] 359130

								lpLink->mtext[k] = lpLinkText;
							}
							///TODO:[test]
							hr = ipFieldGroupModel->AddLink(count, lpLink);
							CHECK_HR;
						}
						lpLinkHolder.GiveUp();
					}
					count++;
				}
			}
			else
			{
				return E_FAIL;
			}

		}
		else
			return E_FAIL;
	}

	return S_OK;
}

int hLoadSlicedView2(DSSBinaryDataReader *mpReadStream, DSSDataModelXTabImpl *ipModel, LayoutInfo * ipLayout, vector<int> &irMetricDataType, vector<vector<int> > &irTotalDatatype,   int* ipvSliceOffset, int inSliceOffset)
{
	int hr = S_OK;
	int lBlockType = 0;
	hr = mpReadStream->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockXTabModelViewsV1);
	{
		int lnSlices = 0;
		hr = mpReadStream->ReadInt(&lnSlices);
		CHECK_HR;

		int lSliceStartPosition = mpReadStream->GetPosition();

		//get slice map
		vector<vector<int> > lvWindowSize;
		//vector<int> lvStreamOffset;
		hr = hScanSlices(mpReadStream, ipModel, ipLayout, irMetricDataType, irTotalDatatype, lnSlices, lvWindowSize, &ipvSliceOffset, &inSliceOffset);
		CHECK_HR;

		bool mbDelayLoading = true;
		if (mbDelayLoading)
		{
			int lStreamOffset = ipModel->getModelDataSource()->getFileOffset();	//mpReadStream's offset in the file

			int lSliceEndPosition = mpReadStream->GetPosition();		//slice's offset within the read stream

			MBase::CString lLocalPath;
			int hr = DataSourceUtil::getLocalPath(lLocalPath);
			CHECK_HR;

			DSSDataSource *lpDS = new DSSDataSource();

			if (lLocalPath.find(ipModel->getDataSourcePath()) == lLocalPath.npos)
			{
                hr = lpDS->Init(mpReadStream, lStreamOffset, lSliceStartPosition, lSliceEndPosition,
                                lnSlices, ipvSliceOffset, inSliceOffset, ipModel->getDataSourcePath(), false);
                CHECK_HR;
			}
			else
			{
				MBase::CString lNodeKey = ipModel->getNodeKey();
                std::string lSubFolder = ipModel->getSubFolder();
				lLocalPath = ipModel->getDataSourcePath() + "/" + lSubFolder + "/" + lNodeKey;
				hr = lpDS->Init(mpReadStream, lStreamOffset, lSliceStartPosition, lSliceEndPosition,
								lnSlices, ipvSliceOffset, inSliceOffset, lLocalPath, true);
				CHECK_HR;
			}

			hr = ipModel->setDataSource(lpDS);
			CHECK_HR;
		}

		//load header keys
		hr = mpReadStream->SetPosition(lSliceStartPosition);
		CHECK_HR;

		hr = hLoadHeaderKeys(mpReadStream, ipModel, ipLayout, lnSlices, lvWindowSize, ipvSliceOffset, inSliceOffset);
		CHECK_HR;

		//create xtab header
		bool lbHasDummyUnit = (ipModel->getDummyUnit() != NULL);
		hr = hCreateXTabHeader(ipLayout, ipModel, lvWindowSize[0], lbHasDummyUnit);
		CHECK_HR;
		
		hr = hCreateXTabHeader(ipLayout + 1, ipModel, lvWindowSize[1]);
		CHECK_HR;

		//create data slices
		vector<char> lvDummy;
		vector<DSSTabularIndex*> lvCellIndices;
		hr = hBuildCellIndex(ipLayout, ipModel, lvDummy, lvCellIndices);
		CHECK_HR;

		hr = hCreateDataSlices(ipModel, lvCellIndices, irMetricDataType, irTotalDatatype);
		CHECK_HR;

		//build slice window
		hr = ipModel->CreateViewMap(lnSlices, ipLayout[2].vUnits, ipLayout[2].mvKeys,
									lvWindowSize[0], ipLayout[0].mvMap2DisplayedRows,
									lvWindowSize[1], ipLayout[1].mvMap2DisplayedRows);
		CHECK_HR;

		//load metric cell data
		hr = mpReadStream->SetPosition(lSliceStartPosition);
		CHECK_HR;

		if (!mbDelayLoading)
		{
			int lnSlices = ipModel->countSlices();
			for (int iSlice = 0; iSlice < lnSlices; iSlice++)
			{
				int lDataOffset = lSliceStartPosition + ipvSliceOffset[2 * iSlice];
				int lFlagOffset = lSliceStartPosition + ipvSliceOffset[2 * iSlice + 1];
				hr = DSSModelPopulation::LoadMetricData(ipModel, mpReadStream, iSlice, lDataOffset, lFlagOffset);
				CHECK_HR;
			}
            delete ipvSliceOffset;
		}
	}
	hr = mpReadStream->EndReadBlock();
	return hr;
}

int hLoadSlicedView3(DSSBinaryDataReader *mpReadStream, DSSDataModelXTabImpl *ipModel, LayoutInfo * ipLayout,  vector<int> &irMetricDataType, vector<vector<int> > &irTotalDatatype, int* ipvSliceOffset, int inSliceOffset, int iViewEndPosition, int iLevelInfoOffset, bool ibTransactionEnabled)
{
	int hr = S_OK;
	int lBlockType = 0;
	hr = mpReadStream->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockXTabModelViewsV2);
	{
		int lnSlices = 0;
		hr = mpReadStream->ReadInt(&lnSlices);
		CHECK_HR;
		
		int lbIsPartial = 0;
		hr = mpReadStream->ReadInt(&lbIsPartial);
		CHECK_HR;
		
		int lnRow = 0;
		hr = mpReadStream->ReadInt(&lnRow);
		CHECK_HR;
		
		int lnColumn = 0;
		hr = mpReadStream->ReadInt(&lnColumn);
		CHECK_HR;
		  
		int lSliceStartPosition = mpReadStream->GetPosition();
        vector<int*> lvList;
        vector<int*> lvOffset;
        vector<int> lnList;
        vector<int> lnOffset;
        bool mbDelayLoading =  true;
        ipLayout[0].mTotalLevels = 0;
        ipLayout[1].mTotalLevels = 0;
        int* pGBPaths = NULL;
        char* pGBPage = NULL;
        int GBPageSize = 0;
        int GBKeySize = 0;
        int GBPagePosInFile = 0;
		if((lnRow != 0 || lnColumn != 0) && lnSlices != 0)
        {
            //get slice map

            hr = hLoadSliceMap(mpReadStream, ipModel, lnSlices, lvList,lnList, lvOffset, lnOffset);
            CHECK_HR;
              
            //add unmap logic since lvList, lvOffset are now mapped to file and they are read-only
            if(lnRow == 0)
            {
 //               int lReservedSize = lnOffset[0] * sizeof(int);
//MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)
                int* lpvRowOffset = new int[lnOffset[0]];
//MEM_GOVERNOR_COMMIT(lReservedSize)
                memset(lpvRowOffset, 0, sizeof(int) * lnOffset[0]);
                ipModel->clearRowList(0);
                lvList[0] = NULL;
                lvOffset[0] = lpvRowOffset;
                ipModel->setRowList(0, lvList[0], lnList[0], lvOffset[0], lnOffset[0],false);
            }
            if(lnColumn == 0)
            {
 //               int lReservedSize = lnOffset[1] * sizeof(int);
//MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)
                int* lpvColOffset = new int[lnOffset[1]];
//MEM_GOVERNOR_COMMIT(lReservedSize)                
                memset(lpvColOffset, 0, sizeof(int) * lnOffset[1]);
                ipModel->clearRowList(1);
                lvList[1] = NULL;
                lvOffset[1] = lpvColOffset;
                ipModel->setRowList(1, lvList[1], lnList[1], lvOffset[1], lnOffset[1], false);
            }
            if (mbDelayLoading)
            {
                int lStreamOffset = ipModel->getModelDataSource()->getFileOffset();	//mpReadStream's offset in the file
                
                int lSliceEndPosition = iViewEndPosition;
                
                MBase::CString lLocalPath;
                int hr = DataSourceUtil::getLocalPath(lLocalPath);
                CHECK_HR;
                
                DSSDataSource *lpDS = new DSSDataSource();
                
                if (lLocalPath.find(ipModel->getDataSourcePath()) == lLocalPath.npos)
                {				
                hr = lpDS->Init(mpReadStream, lStreamOffset, lSliceStartPosition, lSliceEndPosition,
                                lnSlices, ipvSliceOffset, inSliceOffset, ipModel->getDataSourcePath(), false);
                CHECK_HR;
                }
                else
                {
                    MBase::CString lNodeKey = ipModel->getNodeKey();
                    std::string lSubFolder = ipModel->getSubFolder();
                    lLocalPath = ipModel->getDataSourcePath() + "/" + lSubFolder + "/" + lNodeKey;
                    hr = lpDS->Init(mpReadStream, lStreamOffset, lSliceStartPosition, lSliceEndPosition,
                                    lnSlices, ipvSliceOffset, inSliceOffset, lLocalPath, true);
                    CHECK_HR;
                }
                
                hr = ipModel->setDataSource(lpDS);
                CHECK_HR;
                
                //TODO: CHECK SECURITY MODE
                if(DataSourceUtil::isMappedFileSupported())
                {
                    hr = ipModel->CreateMappedFileForDataSource();
                    CHECK_HR;
                }
            }
        

            bool lbLoadbySlice = !ibTransactionEnabled && (ipLayout[2].vUnits.size() != 0);     
            ipModel->setLoadHeaderBySlice(lbLoadbySlice);
         
            int lbIsHeaderSorted = 1;
            bool lbHasDummyUnit = (ipModel->getDummyUnit() != NULL);
            //actually lbIsHeaderSorted is always true for the new binary structure
            if(lbLoadbySlice)
            {
                vector<int> lvHeaderSize;
                if(iLevelInfoOffset > 0)
                {
                    //1. load the gb header, is this necessary? YES
                    hr = hLoadHeaderKeys3(mpReadStream, ipModel, &pGBPaths, &GBKeySize, &pGBPage, &GBPageSize, &GBPagePosInFile,ipLayout, lvList, lnList, lvOffset, lnOffset, lvHeaderSize, lnSlices, lbIsHeaderSorted, iLevelInfoOffset, lbHasDummyUnit);
                     CHECK_HR;
                     
                    AE_ASSERT(lvHeaderSize.size() == 2)
                    
                    int lPosition = mpReadStream->GetPosition();
                    //2. create row header
                    hr = mpReadStream->SetPosition(iLevelInfoOffset);
                    CHECK_HR;
                    
                    hr = hCreateXTabHeader4(mpReadStream, ipLayout, ipModel, lvList[0], lnList[0], lvOffset[0], lnOffset[0], lvHeaderSize[0]);
                    CHECK_HR;
                    
                    //3. create column header
                    hr = hCreateXTabHeader4(mpReadStream, ipLayout + 1, ipModel, lvList[1], lnList[1], lvOffset[1], lnOffset[1],lvHeaderSize[1]);
                    CHECK_HR;
                    
                    hr = mpReadStream->SetPosition(lPosition);
                    CHECK_HR;
                }
                else
                {
                    hr = hLoadHeaderKeys3(mpReadStream, ipModel, NULL, NULL, NULL,NULL,NULL, ipLayout, lvList, lnList, lvOffset, lnOffset, lvHeaderSize, lnSlices, lbIsHeaderSorted, iLevelInfoOffset, lbHasDummyUnit);
                    CHECK_HR;
                    
                    //create xtab header
                    hr = hCreateXTabHeader3(ipLayout, ipModel);
                    CHECK_HR;
                    
                    hr = hCreateXTabHeader3(ipLayout + 1, ipModel);
                    CHECK_HR;
                }
            }
            else
            {
   
                    hr = hLoadHeaderKeys2(mpReadStream, ipModel, ipLayout, lvList, lnList, lvOffset, lnOffset, lnSlices, lbIsHeaderSorted);
                    CHECK_HR;
                
                    //create xtab header
                    hr = hCreateXTabHeader2(ipLayout, ipModel, lvList[0], lnList[0], lvOffset[0], lnOffset[0], lbIsHeaderSorted, lbHasDummyUnit);
                    CHECK_HR;
                
                    hr = hCreateXTabHeader2(ipLayout + 1, ipModel, lvList[1], lnList[1], lvOffset[1], lnOffset[1], lbIsHeaderSorted);
                    CHECK_HR;

            }
        }
		//create data slices
		vector<char> lvDummy;
		vector<DSSTabularIndex*> lvCellIndices;
		hr = hBuildCellIndex(ipLayout, ipModel, lvDummy, lvCellIndices);
		CHECK_HR;
		
        
        if(pGBPaths)
        {
            hr = ipModel->CreateViewMap3(lnSlices,ipLayout[2].vUnits, pGBPaths, pGBPage, GBPageSize, GBPagePosInFile, lvList, lnList, lvOffset, lnOffset);
        }else
        {
            hr = ipModel->CreateViewMap2(lnSlices,ipLayout[2].vUnits, ipLayout[2].mvKeys, lvList, lnList, lvOffset, lnOffset);
        }
		CHECK_HR;
		
        bool lbHasXTabNull = false;
        hCheckXtabNulls(ipModel, lnSlices, lbHasXTabNull);
        //swallow error here, since inside hCheckXtabNull we only can "get" type methods and won't "set" anything
        //CHECK_HR;
        
		hr = hCreateDataSlices(ipModel, lvCellIndices, irMetricDataType, irTotalDatatype, lbHasXTabNull);
		CHECK_HR;
        
        hr = ipModel->UnMapSliceWindow(0);
        CHECK_HR;
        
        hr = ipModel->UnMapSliceWindow(1);
        CHECK_HR;
        
		//load metric cell data
		hr = mpReadStream->SetPosition(lSliceStartPosition);
		CHECK_HR;
		
		if (!mbDelayLoading)
		{
			//shouldn't be empty
			if(inSliceOffset > 0)
			{
				for (int iSlice = 0; iSlice < lnSlices; iSlice++)
				{
					int lDataOffset = lSliceStartPosition + ipvSliceOffset[2 * iSlice];
					int lFlagOffset = lSliceStartPosition + ipvSliceOffset[2 * iSlice + 1];
					hr = DSSModelPopulation::LoadMetricData(ipModel, mpReadStream, iSlice, lDataOffset, lFlagOffset);
					CHECK_HR;
				}
			}
		}
	}
	hr = mpReadStream->EndReadBlock();
	return hr;
}
int hLoadSliceMap(DSSBinaryDataReader *mpReadStream, DSSDataModelXTabImpl* ipXTabModel, int nSlices, vector<int*> &orvList, vector<int> &ovnList, vector<int*> &orvOffset, vector<int> &ovnOffset)
{
	int hr = S_OK;
	orvList.resize(2);
	orvOffset.resize(2);
    ovnList.resize(2);
    ovnOffset.resize(2);
    for(int i = 0; i < 2; i++)
    {
        orvList[i] = NULL;
        orvOffset[i] = NULL;
        ovnList[i] = 0;
        ovnOffset[i] = 0;
    }
	if(mpReadStream && nSlices > 0)
	{
        std::string iFilePath = ipXTabModel->getFilePath();
        if(iFilePath != "")
        {
            int lFileStart = mpReadStream->getStart();
            
            //row list
            hr = mpReadStream->ReadInt(&ovnList[0]);
            CHECK_HR;
            
            int pageSize = getpagesize();
            int pos = lFileStart + mpReadStream->GetPosition();
            int RowListPageStart = pos/pageSize * pageSize;
            int RowListPageOffset = pos % pageSize;
            char* rowListPage = NULL;
            int lSize = ovnList[0] * sizeof(int);
            int nRowListSize = lSize + RowListPageOffset;
            hr = DataSourceUtil::reloadMappedFileBuffer(iFilePath.c_str(), nRowListSize, RowListPageOffset, RowListPageStart, (void**)&rowListPage, (void**)(&orvList[0]));
            CHECK_HR;
            
            hr = mpReadStream->Skip(lSize);
            CHECK_HR;
       
            //row window offset
            hr = mpReadStream->ReadInt(&ovnOffset[0]);
            CHECK_HR;
            pos = lFileStart + mpReadStream->GetPosition();
            int RowWindowPageStart = pos/pageSize * pageSize;
            int RowWindowPageOffset = pos % pageSize;
            char* rowOffsetPage = NULL;
            lSize = ovnOffset[0] * sizeof(int);
            int nRowWindowSize = lSize + RowWindowPageOffset;
            hr = DataSourceUtil::reloadMappedFileBuffer(iFilePath.c_str(), nRowWindowSize, RowWindowPageOffset, RowWindowPageStart, (void**)&rowOffsetPage, (void**)(&orvOffset[0]));
            CHECK_HR;
            hr = mpReadStream->Skip(lSize);
            CHECK_HR;
            
            //col list
            hr = mpReadStream->ReadInt(&ovnList[1]);
            CHECK_HR;
            pos = lFileStart + mpReadStream->GetPosition();
            int ColListPageStart = pos/pageSize * pageSize;
            int ColListPageOffset = pos % pageSize;
            char* colListPage = NULL;
            lSize = ovnList[1] * sizeof(int);
            int nColListSize = lSize + ColListPageOffset;
            hr = DataSourceUtil::reloadMappedFileBuffer(iFilePath.c_str(), nColListSize, ColListPageOffset, ColListPageStart, (void**)&colListPage, (void**)(&orvList[1]));
            CHECK_HR;
            hr = mpReadStream->Skip(lSize);
            CHECK_HR;
            
            //col window offset        
            hr = mpReadStream->ReadInt(&ovnOffset[1]);
            CHECK_HR;
            pos = lFileStart + mpReadStream->GetPosition();
            int ColWindowPageStart = pos/pageSize * pageSize;
            int ColWindowPageOffset = pos % pageSize;
            char* colOffsetPage = NULL;
            lSize = ovnOffset[1] * sizeof(int);
            int nColWindowSize = lSize + ColWindowPageOffset;
            hr = DataSourceUtil::reloadMappedFileBuffer(iFilePath.c_str(), nColWindowSize, ColWindowPageOffset, ColWindowPageStart, (void**)&colOffsetPage, (void**)(&orvOffset[1]));
            CHECK_HR;
            hr = mpReadStream->Skip(lSize);
            CHECK_HR;
            
            ipXTabModel->setMemPageInfo(rowListPage,rowOffsetPage,colListPage,colOffsetPage,
                                        nRowListSize,nRowWindowSize,nColListSize,nColWindowSize, 
                                        RowListPageStart, RowWindowPageStart, ColListPageStart, ColWindowPageStart,
                                        RowListPageOffset, RowWindowPageOffset, ColListPageOffset, ColWindowPageOffset);
        }
        else
        {
            //row list
            hr = mpReadStream->ReadInt(&ovnList[0]);
            CHECK_HR;
            
            {
 //           int lReservedSize = ovnList[0] * sizeof(int);
//MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)  
            orvList[0] = new int[ovnList[0]];
            hr = mpReadStream->ReadIntArray(orvList[0], ovnList[0]);
            CHECK_HR;
//MEM_GOVERNOR_COMMIT(lReservedSize)
            }
            
            //row window offset
            hr = mpReadStream->ReadInt(&ovnOffset[0]);
            CHECK_HR;
            
            {
 //           int lReservedSize = ovnOffset[0] * sizeof(int);
//MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize) 
            orvOffset[0] = new int[ovnOffset[0]];
            hr = mpReadStream->ReadIntArray(orvOffset[0], ovnOffset[0]);
            CHECK_HR;
//MEM_GOVERNOR_COMMIT(lReservedSize)
            }
            
            //col list
            hr = mpReadStream->ReadInt(&ovnList[1]);
            CHECK_HR;

            {
 //           int lReservedSize = ovnList[1] * sizeof(int);
//MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)             
            orvList[1] = new int[ovnList[1]];
            hr = mpReadStream->ReadIntArray(orvList[1], ovnList[1]);
            CHECK_HR;
//MEM_GOVERNOR_COMMIT(lReservedSize)
            }
            
            //col window offset        
            hr = mpReadStream->ReadInt(&ovnOffset[1]);
            CHECK_HR;
    
            {
 //           int lReservedSize = ovnOffset[1] * sizeof(int);
//MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize) 
            orvOffset[1] = new int[ovnOffset[1]];
            hr = mpReadStream->ReadIntArray(orvOffset[1], ovnOffset[1]);
            CHECK_HR;
//MEM_GOVERNOR_COMMIT(lReservedSize)
            }
            
            ipXTabModel->setRowList(0, orvList[0], ovnList[0], orvOffset[0], ovnOffset[0],false);
            ipXTabModel->setRowList(1, orvList[1], ovnList[1], orvOffset[1], ovnOffset[1],false);
        }

	}
	
	return S_OK;
}

int hScanSlices(DSSBinaryDataReader *mpReadStream, DSSDataModelXTabImpl *ipModel, LayoutInfo * ipLayout, vector<int> &irMetricDataType, vector<vector<int> > &irTotalDatatype,
								 int nSlices, vector<vector<int> > &orWindowSize, int** oppStreamOffset, int* opnStreamOffset)
{
	bool lbOffsetPreloaded = (*opnStreamOffset) > 0;

	orWindowSize.resize(3);

	if (nSlices > 0)
	{
		if (!lbOffsetPreloaded)
        {
			//orStreamOffset.reserve(2 * nSlices + 1);
            (*oppStreamOffset) = new int [2 * nSlices + 1];
            (*opnStreamOffset) = 2 * nSlices + 1;
        }

		orWindowSize[0].reserve(nSlices + 1);
		orWindowSize[1].reserve(nSlices + 1);
		orWindowSize[2].reserve(nSlices + 1);
		orWindowSize[0].push_back(0);
		orWindowSize[1].push_back(0);
		orWindowSize[2].push_back(0);
	}

	int lBaseStreamOffset = mpReadStream->GetPosition();
	int lnRowCount = 0;
	int lnColCount = 0;
	int lnGBCount = 0;

	int hr = S_OK;

	//get metric unit index
	int lMetricsUnitID = ipModel->getMetricUnitID();
	if (lMetricsUnitID >= 0)	//410114
	{
		DSSTabularMetrics *lpMetrics = NULL;
		ipModel->getMetricsUnit(&lpMetrics);
		CHECK_PTR(lpMetrics);
		if (lpMetrics->Count() <= 0)
			lMetricsUnitID = -1;
	}

	bool lbMetricOnColAxis = (ipLayout[1].metricsPos >= 0);

	int i = 0;
	for (i = 0; i < nSlices; i++)
	{
		int lObj = 0;
		hr = mpReadStream->ReadInt(&lObj);
		CHECK_HR;

		if (lObj != EnumObjectXTabView)
			return E_UNEXPECTED_DATATYPE;

		//1. load gropu-by path of the current slice
		int lnGBs = 0;
		vector<int> lvGroupByPath;
		vector<unsigned char> lvGBTotalID, lvGBMetricID;
		vector<short> lvGBLevel;
		hr = hLoadHeader(mpReadStream, 2, ipLayout+2, lvGroupByPath, lnGBs, lvGBTotalID, lvGBMetricID, lvGBLevel, lvGroupByPath);
		CHECK_HR;

		//2. load row header element of the current slice
		int lnDispRows = 0;
		vector<int> lvRowKeys;
		vector<unsigned char> lvRowMetricID;
		vector<unsigned char> lvRowSubtotalID;
		vector<short> lvRowLevelID;
		hr = hLoadHeader(mpReadStream, 0, ipLayout, lvGroupByPath, lnDispRows, lvRowSubtotalID, lvRowMetricID, lvRowLevelID, lvRowKeys, lMetricsUnitID < 0);
		CHECK_HR;

		//3. load column header element of the current slice
		int lnDispCols = 0;
		vector<int> lvColKeys;
		vector<unsigned char> lvColMetricID;
		vector<unsigned char> lvColSubtotalID;
		vector<short> lvColLevelID;
		hr = hLoadHeader(mpReadStream, 1, ipLayout + 1, lvGroupByPath, lnDispCols, lvColSubtotalID, lvColMetricID, lvColLevelID, lvColKeys, lMetricsUnitID < 0);
		CHECK_HR;

		lnRowCount += lnDispRows;
		lnColCount += lnDispCols;
		lnGBCount += lnGBs;

		orWindowSize[0].push_back(lnRowCount);
		orWindowSize[1].push_back(lnColCount);
		orWindowSize[2].push_back(lnGBCount);

        int lnStreamOffset = 0;
		if (lbOffsetPreloaded)			//no need to scan
		{
			hr = mpReadStream->SetPosition((*oppStreamOffset)[i * 2 + 1] + lBaseStreamOffset);
			CHECK_HR;
		}
		else
		{
		int lStreamOffset = mpReadStream->GetPosition();
		(*oppStreamOffset)[lnStreamOffset++] = lStreamOffset - lBaseStreamOffset;

		//4. load metric cell values of the current slice
		if (lnDispRows > 0 && lnDispCols > 0 && lMetricsUnitID >= 0)
		{
			int lGBTotalID = lvGBTotalID.size() > 0 ? lvGBTotalID[0] : 0;

			int iRow = 0;
			for (iRow = 0; iRow < lnDispRows; iRow++)
			{
				int lRowTotalID = lvRowSubtotalID[iRow];
				int lMetricID = (lMetricsUnitID >= 0 && !lbMetricOnColAxis) ? lvRowMetricID[iRow] : -1;	//metrics on the row axis

				int iCol = 0;
				for (iCol = 0; iCol < lnDispCols; iCol++)
				{
					int lColTotalID = lvColSubtotalID[iCol];
					// pick max(lGBTotalID, lRowTotalID, lColTotalID)
					int lSubtotalID = lColTotalID > lRowTotalID ? lColTotalID : lRowTotalID;
					if (lSubtotalID < lGBTotalID)
						lSubtotalID = lGBTotalID;

					if (lSubtotalID > 0)
					{
						if ((lRowTotalID > 0 && lSubtotalID > lRowTotalID) || (lColTotalID > 0 && lSubtotalID > lColTotalID) || (lGBTotalID > 0 && lSubtotalID > lGBTotalID))
							lSubtotalID = -1;	//invalid subtotal
					}
					else
						lSubtotalID = 0;		//no subtotal

					if (lbMetricOnColAxis && lMetricsUnitID >= 0)
						lMetricID = lvColMetricID[iCol];

					int lCellSize = 0;
					hr = hSkipCellData(mpReadStream, lMetricID, lSubtotalID, irMetricDataType, irTotalDatatype, lCellSize);
					CHECK_HR;
				}//end of for (iCol = 0; iCol < lnDispCols; iCol++)
			}//end of for (iRow = 0; iRow < lnDispRows; iRow++)
		}
		else if (lMetricsUnitID >= 0)
		{
			int lGBTotalID = lvGBTotalID.size() > 0 ? lvGBTotalID[0] : 0;
			if (lbMetricOnColAxis)	//except GB units, all are on column axis
			{
				int iCol = 0;
				for (iCol = 0; iCol < lnDispCols; iCol++)
				{
					int lColTotalID = lvColSubtotalID[iCol];
					int lSubtotalID = lGBTotalID > lColTotalID ? lGBTotalID : lColTotalID;
					if (lSubtotalID > 0)		//valid subtotal
					{
						if (lGBTotalID > 0 && lSubtotalID > lGBTotalID)
							lSubtotalID = -1;	//invalid subtotal
					}
					else
						lSubtotalID = 0;		//no subtotal

					int lCellSize = 0;
					hr = hSkipCellData(mpReadStream, lvColMetricID[iCol], lSubtotalID, irMetricDataType, irTotalDatatype, lCellSize);
					CHECK_HR;
				}
			}
			else	//units on row axis only
			{
				int iRow = 0;
				for (iRow = 0; iRow < lnDispRows; iRow++)
				{
					int lRowTotalID = lvRowSubtotalID[iRow];
					int lSubtotalID = lGBTotalID > lRowTotalID ? lGBTotalID : lRowTotalID;
					if (lSubtotalID > 0)		//valid subtotal
					{
						if (lRowTotalID > 0 && lSubtotalID > lRowTotalID)
							lSubtotalID = -1;	//invalid subtotal
					}
					else
						lSubtotalID = 0;		//no subtotal

					int lCellSize = 0;
					hr = hSkipCellData(mpReadStream, lvRowMetricID[iRow], lSubtotalID, irMetricDataType, irTotalDatatype, lCellSize);
					CHECK_HR;
				}
			}
		}
		else if (lnDispRows > 0 && lnDispCols > 0 && !ipLayout[0].vUnits.empty() && !ipLayout[1].vUnits.empty())
		{
			//lwang, 10/5/19, 409363
			hr = mpReadStream->Skip(lnDispRows * lnDispCols * sizeof(bool));
			CHECK_HR;

		}//end of else if (lMetricsUnitID >= 0)

		lStreamOffset = mpReadStream->GetPosition();
        (*oppStreamOffset)[lnStreamOffset++] = lStreamOffset - lBaseStreamOffset;
	  }
		//skip cell flags
		int lSkipSize = 0;
		mpReadStream->ReadInt(&lSkipSize);
		if (lSkipSize > 0)
			mpReadStream->Skip(lSkipSize * sizeof(char));

		//skip threshold ID
		mpReadStream->ReadInt(&lSkipSize);
		if (lSkipSize > 0)
			mpReadStream->Skip(lSkipSize * sizeof(int));

		//skip threshold metric ID
		mpReadStream->ReadInt(&lSkipSize);
		if (lSkipSize > 0)
			hr = mpReadStream->Skip(lSkipSize * sizeof(int));
		CHECK_HR;
	}
	return S_OK;
}

int hCountRows(DSSBinaryDataReader *mpReadStream, int iAxis, int nTemplateUnits, int &rnRows)
{
	int hr = S_OK;

	int lSize = 0;
	hr = mpReadStream->ReadInt(&lSize);
	CHECK_HR;

	/*
	int nTemplateUnits = ipLayoutInfo->vUnits.size();
	if (ipLayoutInfo->mnRuntimeUnits > 0)		//handle runtime unit
	{
		int lnRTUnits = ipLayoutInfo->mnRuntimeUnits;
		nTemplateUnits = lnTemplateUnits - lnRTUnits;
	}
	//*/

	if (nTemplateUnits > 0)
	{
		rnRows = lSize / nTemplateUnits;
	}
	else if (iAxis != 2)	//lnUnits = 0 and not group-by
	{
		rnRows = 1;
	}

	hr = mpReadStream->Skip(sizeof(int) * lSize);
	CHECK_HR;

	return S_OK;
}

int hLoadHeaderKeys2(DSSBinaryDataReader *mpReadStream, DSSDataModelXTabImpl *ipModel, LayoutInfo * ipLayout,vector<int *> &rvList, vector<int> &rvnList, vector<int *> &rvOffset, vector<int> &rvnOffset, int inGBs, int& obSorted)
{
	int hr = S_OK;
    
    if(inGBs > 0)
    {
        int lbIsHeaderSorted = 1; 
        hr = mpReadStream->ReadInt(&lbIsHeaderSorted);
        CHECK_HR;
        int lObj = 0;
        hr = mpReadStream->ReadInt(&lObj);
        CHECK_HR;
        
        if (lObj != EnumObjectXTabView)
            return E_UNEXPECTED_DATATYPE;
    }
    
	//get metric unit index
	int lMetricsUnitID = ipModel->getMetricUnitID();
	if (lMetricsUnitID >= 0)	//410114
	{
		DSSTabularMetrics *lpMetrics = NULL;
		ipModel->getMetricsUnit(&lpMetrics);
		CHECK_PTR(lpMetrics);	
		if (lpMetrics->Count() <= 0)
			lMetricsUnitID = -1;
	}
	bool lbIsMetricEmpty = lMetricsUnitID < 0;
	 
	ipLayout[0].setRowCount(rvnList[0]);
	ipLayout[1].setRowCount(rvnList[1]);

	int lnGBUnits = ipLayout[2].vUnits.size();
	//in this case, the slice count is 1 with no GB paths
	if(lnGBUnits == 0)
		ipLayout[2].setRowCount(0);
    else
        ipLayout[2].setRowCount(inGBs);
	
    if(inGBs > 0)
    {   
        int lZero = 0;
        int* lNULL = NULL;
        hr = hLoadHeader2(mpReadStream, 2, ipLayout+2, NULL, 0, 0, lNULL,lZero, lNULL,lZero, lbIsMetricEmpty);
        CHECK_HR;
        
        int *lpGBPath = NULL;
        int lnGBPaths = 0;
        if(lnGBUnits > 0) 
        {
            lpGBPath = &ipLayout[2].mvKeys[0];
            lnGBPaths = ipLayout[2].mvKeys.size()/lnGBUnits;
        }
        
        hr = hLoadHeader2(mpReadStream, 0, ipLayout, lpGBPath, lnGBUnits, lnGBPaths, rvList[0], rvnList[0], rvOffset[0], rvnOffset[0], lbIsMetricEmpty);		
        CHECK_HR;
        
        hr = hLoadHeader2(mpReadStream, 1, ipLayout+1, lpGBPath, lnGBUnits, lnGBPaths, rvList[1], rvnList[1], rvOffset[1], rvnOffset[1],lbIsMetricEmpty);		
        CHECK_HR;
    }

	return S_OK;
}

int hLoadHeaderKeys3(DSSBinaryDataReader *mpReadStream, DSSDataModelXTabImpl *ipModel, int** pGBPaths, int* pGBKeySize, char** pGBPage, int* pGBPageSize, int * pPagePosInFile, LayoutInfo * ipLayout,vector<int*> &rvList, vector<int> &rvnList, vector<int*> &rvOffset, vector<int> &rvnOffset, vector<int>& ivHeaderSize, int inGBs, int& obSorted, int iLevelInfoOffset, bool ibHasDummyUnit)
{
	int hr = S_OK;
    
    if(inGBs > 0)
    {
        int lbIsHeaderSorted = 1; 
        hr = mpReadStream->ReadInt(&lbIsHeaderSorted);
        CHECK_HR;
        obSorted = lbIsHeaderSorted;
        
        int lObj = 0;
        hr = mpReadStream->ReadInt(&lObj);
        CHECK_HR;
        
        if (lObj != EnumObjectXTabView)
            return E_UNEXPECTED_DATATYPE;
    }
    
	//get metric unit index
	int lMetricsUnitID = ipModel->getMetricUnitID();
	if (lMetricsUnitID >= 0)	//410114
	{
		DSSTabularMetrics *lpMetrics = NULL;
		ipModel->getMetricsUnit(&lpMetrics);
		CHECK_PTR(lpMetrics);	
		if (lpMetrics->Count() <= 0)
			lMetricsUnitID = -1;
	}
	bool lbIsMetricEmpty = lMetricsUnitID < 0;
	
    vector<bool> rvFlag;
    int lnMetrics = 0;
    DSSTabularMetrics *lpMetrics = NULL;
    ipModel->getMetricsUnit(&lpMetrics);

    if(lpMetrics)
        lnMetrics = lpMetrics->Count();
    rvFlag.resize(lnMetrics);
    for(int i = 0; i < lnMetrics; i++)
        rvFlag[i] = false;
    
    if(iLevelInfoOffset <= 0)
    {
        ipLayout[0].setRowCount(rvnList[0], false);
        ipLayout[1].setRowCount(rvnList[1], false);
    }
    ipLayout[0].mTotalLevels = 0;
    ipLayout[1].mTotalLevels = 0;

    int lnGBUnits = ipLayout[2].vUnits.size();
    if(inGBs > 0)
    {    
        int *lpGBPath = NULL;
        int lnGBPaths = 0;
        
        int lHeaderStartPosition = mpReadStream->GetPosition();
        
        if(!pGBPaths || ipModel->getFilePath() == "")
        {

            //in this case, the slice count is 1 with no GB paths
            if(lnGBUnits == 0)
                ipLayout[2].setRowCount(0);
            else
                ipLayout[2].setRowCount(inGBs);
            
            int lZero = 0;
            int* lNull = NULL;
            hr = hLoadHeader2(mpReadStream, 2, ipLayout+2, NULL, 0, 0, lNull,lZero,lNull,lZero, lbIsMetricEmpty);
            CHECK_HR;
            

            if(lnGBUnits > 0) 
            {
                lpGBPath = &ipLayout[2].mvKeys[0];
                lnGBPaths = ipLayout[2].mvKeys.size()/lnGBUnits;
            }
        }
        else
        {
            hr = hLoadGBHeader(mpReadStream, ipModel, 2, ipLayout + 2, inGBs, pGBPaths, pGBKeySize, pGBPage, pGBPageSize, pPagePosInFile);
            CHECK_HR;
            
            if(lnGBUnits > 0) 
            {
                lpGBPath = *pGBPaths;
                if(pGBKeySize)
                    lnGBPaths = (*pGBKeySize)/lnGBUnits;
            }
        }
        if(iLevelInfoOffset > 0)
        {
            int lSize = 0;
            hr =  mpReadStream->ReadInt(&lSize);
            CHECK_HR;
            
            ivHeaderSize.push_back(lSize);
            
            hr = mpReadStream->Skip(lSize * sizeof(int));
            CHECK_HR;
            
            hr =  mpReadStream->ReadInt(&lSize);
            CHECK_HR;
            
            ivHeaderSize.push_back(lSize);
            
            hr = mpReadStream->Skip(lSize * sizeof(int));
            CHECK_HR; 
            
        }
        else
        {
            hr = hLoadHeaderLevels(mpReadStream, 0, ipLayout, lpGBPath, lnGBUnits, lnGBPaths, rvList[0], rvnList[0], rvOffset[0], rvnOffset[0], rvFlag,lbIsMetricEmpty, ibHasDummyUnit);
            CHECK_HR;
        
            hr = hLoadHeaderLevels(mpReadStream, 1, ipLayout+1, lpGBPath, lnGBUnits, lnGBPaths, rvList[1], rvnList[1], rvOffset[1], rvnOffset[1],rvFlag, lbIsMetricEmpty);
            CHECK_HR;
        }
        int lHeaderEndPosition = mpReadStream->GetPosition();
        
        int lStreamOffset = ipModel->getModelDataSource()->getFileOffset();	//mpReadStream's offset in the file
        
        MBase::CString lLocalPath;
        hr = DataSourceUtil::getLocalPath(lLocalPath);
        CHECK_HR;
        
        DSSHeaderSource* lpHS = new DSSHeaderSource();
        
        if (lLocalPath.find(ipModel->getDataSourcePath()) == lLocalPath.npos)
        {				
            hr = lpHS->Init(mpReadStream, lStreamOffset, lHeaderStartPosition, lHeaderEndPosition,ipLayout[0].mnRuntimeUnits, ipLayout[0].mvMapRuntimeUnit2GBUnit, 
                            ipLayout[1].mnRuntimeUnits, ipLayout[1].mvMapRuntimeUnit2GBUnit,   ipModel->getDataSourcePath(), false);
            CHECK_HR;
            
        }
        else
        {
            MBase::CString lNodeKey = ipModel->getNodeKey();
            std::string lSubFolder = ipModel->getSubFolder();
            lLocalPath = ipModel->getDataSourcePath() + "/" + lSubFolder + "/Header_" + lNodeKey;
            hr = lpHS->Init(mpReadStream, lStreamOffset, lHeaderStartPosition, lHeaderEndPosition, ipLayout[0].mnRuntimeUnits, ipLayout[0].mvMapRuntimeUnit2GBUnit, 
                            ipLayout[1].mnRuntimeUnits, ipLayout[1].mvMapRuntimeUnit2GBUnit, lLocalPath, true);
            CHECK_HR;
            
        }
        
        hr = ipModel->setHeaderSource(lpHS);
        CHECK_HR;

    }
	return S_OK;
}

int hLoadHeaderKeys(DSSBinaryDataReader *mpReadStream, DSSDataModelXTabImpl *ipModel, LayoutInfo * ipLayout,
								 int nSlices, vector<vector<int> > &orWindowSize, int* opStreamOffset, int onStreamOffset)
{
	if (nSlices <= 0)
		return S_OK;

	int hr = S_OK;

	//get metric unit index
	int lMetricsUnitID = ipModel->getMetricUnitID();
	if (lMetricsUnitID >= 0)	//410114
	{
		DSSTabularMetrics *lpMetrics = NULL;
		ipModel->getMetricsUnit(&lpMetrics);
		CHECK_PTR(lpMetrics);
		if (lpMetrics->Count() <= 0)
			lMetricsUnitID = -1;
	}
	bool lbIsMetricEmpty = lMetricsUnitID < 0;

	int lnTotalRows = orWindowSize[0][nSlices];
	int lnTotalCols = orWindowSize[1][nSlices];
	int lnTotalGBs = orWindowSize[2][nSlices];

	ipLayout[0].setRowCount(lnTotalRows);
	ipLayout[1].setRowCount(lnTotalCols);
	ipLayout[2].setRowCount(lnTotalGBs);

    /*
	vector<int> *lpvRowWindow = NULL;		//for debug purpose
	vector<int> *lpvColWindow = NULL;
	vector<int> *lpvGBWindow = NULL;
	lpvRowWindow = &orWindowSize[0];
	lpvColWindow = &orWindowSize[1];
	lpvGBWindow = &orWindowSize[2];
	*/
     
	int lBaseStreamOffset = mpReadStream->GetPosition();
	
	int lnGBUnits = ipLayout[2].vUnits.size();

	int i = 0;
	for (i = 0; i < nSlices; i++)
	{
		int lObj = 0;
		hr = mpReadStream->ReadInt(&lObj);
		CHECK_HR;

		if (lObj != EnumObjectXTabView)
			return E_UNEXPECTED_DATATYPE;

		hr = hLoadHeader(mpReadStream, 2, ipLayout+2, NULL, 0, orWindowSize[2][i], lbIsMetricEmpty);
		CHECK_HR;

		int *lpGBPath = &ipLayout[2].mvKeys[i * lnGBUnits];

		hr = hLoadHeader(mpReadStream, 0, ipLayout, lpGBPath, lnGBUnits, orWindowSize[0][i], lbIsMetricEmpty);
		CHECK_HR;

		hr = hLoadHeader(mpReadStream, 1, ipLayout+1, lpGBPath, lnGBUnits, orWindowSize[1][i], lbIsMetricEmpty);
		CHECK_HR;

		//just to the end of metric slices
		hr = mpReadStream->SetPosition(lBaseStreamOffset + opStreamOffset[2 * i + 1]);
		CHECK_HR;

		//skip cell flags
		int lSkipSize = 0;
		mpReadStream->ReadInt(&lSkipSize);
		if (lSkipSize > 0)
			mpReadStream->Skip(lSkipSize * sizeof(char));

		//skip threshold ID
		mpReadStream->ReadInt(&lSkipSize);
		if (lSkipSize > 0)
			mpReadStream->Skip(lSkipSize * sizeof(int));

		//skip threshold metric ID
		hr = mpReadStream->ReadInt(&lSkipSize);
		if (lSkipSize > 0)
			hr = mpReadStream->Skip(lSkipSize * sizeof(int));
		CHECK_HR;
	}
	return S_OK;
}

int hLoadHeader(DSSBinaryDataReader *mpReadStream, int iAxis, LayoutInfo *ipLayoutInfo, int *ipGBPath, int nGBUnits, int nRowsRead, bool ibIsMetricEmpty)
{
	int hr = S_OK;

	int lSize = 0;
	hr = mpReadStream->ReadInt(&lSize);
	CHECK_HR;

	int i = 0;
	int j = 0;
	int lMetricPos = ipLayoutInfo->metricsPos;
	int lnUnits = ipLayoutInfo->vUnits.size();
	bool lbHasMetric = (lMetricPos >= 0 && lMetricPos < lnUnits);

	unsigned char *lpTotalID = &ipLayoutInfo->mvSubTotalID[0] + nRowsRead;
	unsigned char *lpMetricID = &ipLayoutInfo->mvMetricID[0] + nRowsRead;
	short *lpLevels = &ipLayoutInfo->mvLevelID[0] + nRowsRead;
	int *lpKeyTable =&ipLayoutInfo->mvKeys[0]  + nRowsRead * lnUnits;

	if (ipLayoutInfo->mnRuntimeUnits > 0)		//handle runtime unit
	{
		int lnRTUnits = ipLayoutInfo->mnRuntimeUnits;
		int lnStaticUnits = lnUnits - lnRTUnits;

		int *lpMap = &(ipLayoutInfo->mvMapRuntimeUnit2GBUnit[0]);

		if (lSize > 0 && lnStaticUnits > 0)
		{
			int lnRows = lSize / lnStaticUnits;

			vector<int> lvTemp(lSize);
			hr = mpReadStream->ReadIntArray(&lvTemp[0], lSize);
			CHECK_HR;

			int *lpSrc = &lvTemp[0];
			int *lpDes = lpKeyTable;
			for (int i = 0; i < lnRows; i++)
			{
				memmove(lpDes, lpSrc, sizeof(int) * lnStaticUnits);
				lpDes += lnStaticUnits;
				lpSrc += lnStaticUnits;
				for (int j = 0; j < lnRTUnits; j++)
				{
                    if(ipGBPath)
                        *lpDes = ipGBPath[lpMap[j]];
					if (*lpDes < 0)
						*lpDes += 10;	//total in group by header start from -11, -12, -13 ...
					lpDes++;
				}
			}
			lSize = lnRows * lnUnits;
		}
		else
		{
			if (lSize > 0)
				mpReadStream->Skip(lSize * (int)sizeof(int));

			//row units are all runtime
			for (int j = 0; j < lnRTUnits; j++)
			{
				int key = 0;
                if(ipGBPath)
                    key = ipGBPath[lpMap[j]];
				//411589
				if (key < 0)
					key += 10;	//total in group by header start from -11, -12, -13 ...
				lpKeyTable[j] = key;
			}
			lSize = lnRTUnits;
		}
	}
	else
	{
		hr = mpReadStream->ReadIntArray(lpKeyTable, lSize);
		CHECK_HR;
	}

	int lnRows = 0;
	if (lnUnits > 0 && lSize > 0)
	{
		lnRows = lSize / lnUnits;
		_ASSERT(iAxis != 2 || lnRows == 1);		//for group-by axis, each slice only contains one row

		_ASSERT (lnRows * lnUnits == lSize && lnRows > 0);

		int *lpKey = lpKeyTable;

		for (i = 0; i < lnRows; i++)
		{
			if (lbHasMetric)
				lpMetricID[i] = lpKey[lMetricPos];
			else
				lpMetricID[i] = -1;

			int lTotalID = 0;
			int lLevel = 0;
			for (j = 0; j < lnUnits; j++)
			{
				//level is using binary encoding, if header has 3 units {A,B,C},
				//the level of row {a1, b1, SUM} is 0+0+4=4, row {a1, SUM, SUM} is 0+2+4=6
				if (*lpKey < 0)
				{
					lLevel += (1 << j);
					//total in group by header start from -11, -12, -13 ...
					lTotalID = -(*lpKey) - 10*iAxis*(iAxis - 1)/2;
				}
				lpKey++;
			}
			lpTotalID[i] = lTotalID;
			lpLevels[i] = lLevel;
		}
	}
	else if (iAxis != 2 && !ibIsMetricEmpty)	//lnUnits = 0 and not group-by
	{
		*lpMetricID = -1;
		*lpTotalID = 0;
		*lpLevels = 0;
	}
	return hr;
}
//we add this method to handle runtime units of the nonsliced view
int hLoadHeader2(DSSBinaryDataReader *mpReadStream, int iAxis, LayoutInfo *ipLayoutInfo, int *ipGBPath, int nGBUnits, int nGBPaths, int*& rvRowList, int& rnRowList, int*& rvRowOffset, int& rnRowOffset, bool ibIsMetricEmpty)
{
	int hr = S_OK;
	
	_ASSERT(nGBPaths == rnRowOffset - 1 || nGBPaths == 0);
	
	int lSize = 0;
	hr = mpReadStream->ReadInt(&lSize);
	CHECK_HR;
	int lOriginalSize = lSize;
	int i = 0;
	int j = 0;
	int lMetricPos = ipLayoutInfo->metricsPos;
	int lnUnits = ipLayoutInfo->vUnits.size();
	bool lbHasMetric = (lMetricPos >= 0 && lMetricPos < lnUnits);
	
	unsigned char *lpTotalID = &ipLayoutInfo->mvSubTotalID[0];
	unsigned char *lpMetricID = &ipLayoutInfo->mvMetricID[0];
	short *lpLevels = &ipLayoutInfo->mvLevelID[0];
	int *lpKeyTable =&ipLayoutInfo->mvKeys[0];
	
	if (ipLayoutInfo->mnRuntimeUnits > 0)		//handle runtime unit
	{
		int lnRTUnits = ipLayoutInfo->mnRuntimeUnits;
		int lnStaticUnits = lnUnits - lnRTUnits;
		
		int *lpMap = &(ipLayoutInfo->mvMapRuntimeUnit2GBUnit[0]);
		
		if (lSize > 0 && lnStaticUnits > 0)
		{
			int lnRows = lSize / lnStaticUnits;
			
			vector<int> lvTemp(lSize);
			hr = mpReadStream->ReadIntArray(&lvTemp[0], lSize);
			CHECK_HR;
			
			int *lpSrc; //= &lvTemp[0];
			int *lpDes; //= lpKeyTable;
			int lnLoadedRows = 0;
			int k = 0;//slice id
			for(int i = 0; i < rnRowList; i++)
			{
				if(i >= rvRowOffset[k + 1])
					k++;
				
				int * lpGBPath = ipGBPath + k * nGBUnits;
				
				lpDes = lpKeyTable + rvRowList[i] * lnUnits;
				lpSrc = &lvTemp[0] + rvRowList[i] * lnStaticUnits;
				memmove(lpDes, lpSrc, sizeof(int) * lnStaticUnits);
				
				lpDes += lnStaticUnits;
				for (int j = 0; j < lnRTUnits; j++)
				{
					*lpDes = lpGBPath[lpMap[j]];
					if (*lpDes < 0)
						*lpDes += 10;	//total in group by header start from -11, -12, -13 ... 
					lpDes++;
				}
				lnLoadedRows ++;

			}
            //TQMS 498739, the ipLayout may have more rows than the rows loaded from binary
            //because the headers of different slices maybe the same
			//AE_ASSERT(lnLoadedRows == lnRows);
            AE_ASSERT(k == rnRowOffset - 2);
			lSize = lnRows * lnUnits;		
		}
		else
		{
			if (lSize > 0)
				mpReadStream->Skip(lSize * (int)sizeof(int));
			
			//row units are all runtime
			int k = 0;//slice id
			for(int i = 0; i < rnRowList; i++)
			{
				if(i >= rvRowOffset[k + 1])
					k++;
				int * lpGBPath = ipGBPath + k * nGBUnits;

				for (int j = 0; j < lnRTUnits; j++)
				{
					int key = lpGBPath[lpMap[j]];
					//411589
					if (key < 0)
						key += 10;	//total in group by header start from -11, -12, -13 ... 
					lpKeyTable[j + lnRTUnits * rvRowList[i]] = key;
				}
			}
			AE_ASSERT(k == rnRowOffset - 2);
			lSize = lnRTUnits;
		}
	}
	else
	{ 
        //the RWD in 475118
        if(lnUnits == 0  && ipLayoutInfo->mvLevelID.size() == 0)
        {
            ipLayoutInfo->setRowCount(lSize);
            lpKeyTable = &ipLayoutInfo->mvKeys[0];
            lpTotalID = &ipLayoutInfo->mvSubTotalID[0];
            lpMetricID = &ipLayoutInfo->mvMetricID[0];
            lpLevels = &ipLayoutInfo->mvLevelID[0];
            
            if(rnRowList != lSize)
            {           
                delete rvRowList;
                rvRowList = new int[lSize];
                for(int i = 0; i < lSize; i ++)
                    rvRowList[i] = i;
                rnRowList = lSize;
                
                rvRowOffset[rnRowOffset - 1] = lSize;
            }
        }

        hr = mpReadStream->ReadIntArray(lpKeyTable, lSize);
        CHECK_HR;

	}
	
	int lnRows = 0;
	//NOTICE: modified, for non sliced view, the lSize == 0 while lnUnits may be larger than one for the Groupby axis
	if (lnUnits > 0 && lSize > 0)
	{
        //510655 
        if(lOriginalSize == 0)
            lnRows = rnRowList;
        else
        {
            lnRows = lSize / lnUnits;
		
            _ASSERT (lnRows * lnUnits == lSize && lnRows > 0);
		}
		int *lpKey = lpKeyTable;
		
		for (i = 0; i < lnRows; i++)
		{	
			if (lbHasMetric)
				lpMetricID[i] = lpKey[lMetricPos];
			else
				lpMetricID[i] = -1;
			
			int lTotalID = 0;
			int lLevel = 0;
			for (j = 0; j < lnUnits; j++)
			{
				//level is using binary encoding, if header has 3 units {A,B,C},
				//the level of row {a1, b1, SUM} is 0+0+4=4, row {a1, SUM, SUM} is 0+2+4=6
				if (*lpKey < 0)
				{
					lLevel += (1 << j);
					//total in group by header start from -11, -12, -13 ... 
					lTotalID = -(*lpKey) - 10*iAxis*(iAxis - 1)/2;
				}
				lpKey++;
			}
			lpTotalID[i] = lTotalID;
			lpLevels[i] = lLevel;
		}
	}
	else if (iAxis != 2 && (!ibIsMetricEmpty|| ipLayoutInfo->mvLevelID.size() != 0))	//lnUnits = 0 and not group-by
	{
        *lpMetricID = -1;
        *lpTotalID = 0;
        *lpLevels = 0;
	}
	return hr;
}

int hLoadGBHeader(DSSBinaryDataReader *mpReadStream, DSSDataModelXTabImpl* ipModel, int iAxis, LayoutInfo *ipLayoutInfo, int nGBPaths, int **oppGBPath, int* opGBKeySize, char** oppGBPageStart, int* opGBPageSize, int* opGBPagePosInFile)
{
	int hr = S_OK;
    int lSize = 0;
	hr = mpReadStream->ReadInt(&lSize);
    if(opGBKeySize)
        (*opGBKeySize) = lSize;
	CHECK_HR;
    AE_ASSERT(nGBPaths * ipLayoutInfo->vUnits.size() == lSize);
    
//	int lMetricPos = ipLayoutInfo->metricsPos;
//	int lnUnits = ipLayoutInfo->vUnits.size();
//	bool lbHasMetric = (lMetricPos >= 0 && lMetricPos < lnUnits);
	
//	unsigned char *lpTotalID = &ipLayoutInfo->mvSubTotalID[0];
//	unsigned char *lpMetricID = &ipLayoutInfo->mvMetricID[0];
//	short *lpLevels = &ipLayoutInfo->mvLevelID[0];
    
    std::string iFilePath = ipModel->getFilePath();
    int lFileStart = mpReadStream->getStart();
    int pageSize = getpagesize();
    int pos = lFileStart + mpReadStream->GetPosition();
    int pageStart = pos/pageSize * pageSize;
    int pageOffset = pos % pageSize;
    char* lpKeys = NULL;
    hr = DataSourceUtil::reloadMappedFileBuffer(iFilePath.c_str(), lSize * sizeof(int) + pageOffset, pageOffset, pageStart, (void**)&lpKeys, (void**)oppGBPath);
    CHECK_HR;
    if(oppGBPageStart)
        (*oppGBPageStart) = lpKeys;
    if(opGBPageSize)
         (*opGBPageSize) = lSize *sizeof(int) + pageOffset;
    if(opGBPagePosInFile)
        (*opGBPagePosInFile) = pageStart;
    //orvList[0] = new int[ovnList[0]];
    hr = mpReadStream->Skip(lSize * sizeof(int));
    //hr = mpReadStream->ReadIntArray(ipGBPath, lSize);
    CHECK_HR;
	/*
	int lnRows = nGBPaths;
    int *lpKey = *oppGBPath;
    
    for (int i = 0; i < lnRows; i++)
    {	
        if (lbHasMetric)
            lpMetricID[i] = lpKey[lMetricPos];
        else
            lpMetricID[i] = -1;
        
        int lTotalID = 0;
        int lLevel = 0;
        for (int j = 0; j < lnUnits; j++)
        {
            //level is using binary encoding, if header has 3 units {A,B,C},
            //the level of row {a1, b1, SUM} is 0+0+4=4, row {a1, SUM, SUM} is 0+2+4=6
            if (*lpKey < 0)
            {
                lLevel += (1 << j);
                //total in group by header start from -11, -12, -13 ... 
                lTotalID = -(*lpKey) - 10*iAxis*(iAxis - 1)/2;
            }
            lpKey++;
        }
        lpTotalID[i] = lTotalID;
        lpLevels[i] = lLevel;
    }*/
	return hr;
}
int hLoadHeaderLevels(DSSBinaryDataReader *mpReadStream, int iAxis, LayoutInfo *ipLayoutInfo, int *ipGBPath, int nGBUnits, int nGBPaths, int*& rvRowList, int& rnRowList, int*& rvRowOffset, int& rnRowOffset, vector<bool> rvFlag, bool ibIsMetricEmpty,bool ibHasDummy)
{
	int hr = S_OK;
	
	_ASSERT(nGBPaths == rnRowOffset - 1 || nGBPaths == 0);
	
	int lSize = 0;
	hr = mpReadStream->ReadInt(&lSize);
	CHECK_HR;
    
	int lMetricPos = ipLayoutInfo->metricsPos;
    int lSelectedMetricCount = 0;
    
	int lnUnits = ipLayoutInfo->vUnits.size();
	bool lbHasMetric = (lMetricPos >= 0 && lMetricPos < lnUnits);
	
	unsigned char *lpTotalID = &ipLayoutInfo->mvSubTotalID[0];
	unsigned char *lpMetricID = &ipLayoutInfo->mvMetricID[0];
	short *lpLevels = &ipLayoutInfo->mvLevelID[0];

    int *lpUnitSortOrder = lnUnits > 0 ? &ipLayoutInfo->mvUnitSortOrder[0] : NULL;
    vector<int> lvLevelRange;
    vector<int> mapRow2Slice;
    int lnDistinctRows = 0;
    int lnRTUnits = ipLayoutInfo->mnRuntimeUnits;
    int lnStaticUnits = lnUnits - lnRTUnits;
    int lnRows = 0;
    if (lnUnits > 0) //lSize could be 0 if all units are runtime units.
	{

        int *lpMap = &(ipLayoutInfo->mvMapRuntimeUnit2GBUnit[0]);
        
        if(lnStaticUnits > 0)
            lnRows = lSize / lnStaticUnits;
 
        if(lnStaticUnits == 0)//all units are runtimeunits.
        {
            if(lSize > 0)//although unlikely
                mpReadStream->Skip(lSize * sizeof(int));
            lnRows = nGBPaths;
            if(ipLayoutInfo->mvLevelID.size() == 0)
            {
                ipLayoutInfo->setRowCount(lnRows, false);
                lpTotalID = &ipLayoutInfo->mvSubTotalID[0];
                lpMetricID = &ipLayoutInfo->mvMetricID[0];
                lpLevels = &ipLayoutInfo->mvLevelID[0];
            }
        }
        
           
        //      510681 & 510655
        //        if(ipLayoutInfo->mnRuntimeUnits > 0 && lnStaticUnits > 0 || ibHasDummy )
        {
            if(rnRowList > 0)
            {
                mapRow2Slice.resize(lnRows);
                memset(&mapRow2Slice[0],0,sizeof(int)*lnRows);
                for(int i = 0; i < rnRowOffset - 1; i++)
                {
                    for(int j = rvRowOffset[i]; j < rvRowOffset[i+1]; j++)
                    {
                        if(rvRowList[j] >= mapRow2Slice.size())
                        {
                            mapRow2Slice.resize(rvRowList[j]+1);   
                            for(int k = mapRow2Slice.size(); k < rvRowList[j] + 1; k++)
                                mapRow2Slice[k] = -1;
                        }
                        mapRow2Slice[rvRowList[j]] = i;
                    }
                }
            }
        }

        vector<int> lvData(lnUnits);
        vector<int> lvPrevRowData(lnUnits);
        for(int i = 0; i < lnRows; i++)
        {
            hr = mpReadStream->ReadIntArray(&lvData[0], lnStaticUnits);
            CHECK_HR;
            
            if(ipLayoutInfo->mnRuntimeUnits > 0)
            {
                int * lpGBPath = NULL;
 //               if(lnStaticUnits > 0)
                if(mapRow2Slice.size() > 0)
                {
  //                  AE_ASSERT(mapRow2Slice[i] != -1);
                    
                    //don't crash
                    if(mapRow2Slice[i] == -1)
                        mapRow2Slice[i] = 0;
                    
                    lpGBPath = ipGBPath + mapRow2Slice[i] * nGBUnits;
                }
                else
                {
                   lpGBPath = ipGBPath + i * nGBUnits;
                }
                
                int* lpDes = &lvData[lnStaticUnits];
                for (int j = 0; j < lnRTUnits; j++)
                {
                    if(lpGBPath)
                        *lpDes = lpGBPath[lpMap[j]];
                    if (*lpDes < 0)
                        *lpDes += 10;	//total in group by header start from -11, -12, -13 ... 
                    lpDes++;
                }
            }
            
            if (lbHasMetric)
                lpMetricID[i] = lvData[lMetricPos];
            else
                lpMetricID[i] = -1;
            
            int lTotalID = 0;
            int lLevel = 0;
            int* lpKey = &lvData[0];
            for (int j = 0; j < lnUnits; j++)
            {
                //level is using binary encoding, if header has 3 units {A,B,C},
                //the level of row {a1, b1, SUM} is 0+0+4=4, row {a1, SUM, SUM} is 0+2+4=6
                if (*lpKey < 0)
                {
                    lLevel += (1 << j);
                    //total in group by header start from -11, -12, -13 ... 
                    lTotalID = -(*lpKey) - 10*iAxis*(iAxis - 1)/2;
                }
                lpKey++;
            }
            lpTotalID[i] = lTotalID;
            lpLevels[i] = lLevel;
            
            vector<int> lvLevelUnits;
            if(i == 0)
            {
                lnDistinctRows++;
                lvLevelRange.push_back(lnDistinctRows - 1);
                ipLayoutInfo->mvSubtotalFlag.push_back(lTotalID);
                for (int j = lnUnits - 1; j >= 0; j--)
                {
                    if (lvData[j] >= 0 && j != lMetricPos)
                        lvLevelUnits.push_back(ipLayoutInfo->vUnits[j]);
                }
                std::sort(lvLevelUnits.begin(), lvLevelUnits.end());
                ipLayoutInfo->mvLevelUnitsID.push_back(lvLevelUnits);
            }
            else
            {
                bool lbDistinct = false;
                for (int j = lnUnits -1; j >= 0; --j)
                {
                    int unit_id = lpUnitSortOrder[j];

                    if (lvData[unit_id] != lvPrevRowData[unit_id] && unit_id != lMetricPos)	//ignore metrics; 
                    {
                        lnDistinctRows ++;
                        lbDistinct = true;
                        if (lpLevels[i] != lpLevels[i - 1]
                            || lpTotalID[i] != lpTotalID[i - 1])			//the level (include subtotal type) is changed
                        {
                            lvLevelRange.push_back(lnDistinctRows - 1);
                            ipLayoutInfo->mvSubtotalFlag.push_back(lTotalID);
                            for (int j = lnUnits - 1; j >= 0; j--)
                            {
                                if (lvData[j] >= 0 && j != lMetricPos)
                                    lvLevelUnits.push_back(ipLayoutInfo->vUnits[j]);
                            }
                            std::sort(lvLevelUnits.begin(), lvLevelUnits.end());
                            ipLayoutInfo->mvLevelUnitsID.push_back(lvLevelUnits);
                        }
                        break;
                    }
                }
                
                if(!lbDistinct && ibHasDummy && mapRow2Slice[i] == mapRow2Slice[i-1])
                {
                    lnDistinctRows ++;
                }
            }
           
            if(lbHasMetric && rvFlag.size() > 0 && lSelectedMetricCount != rvFlag.size())
            {
                int lMetricID = lvData[lMetricPos];
                if (lMetricID < rvFlag.size() && lMetricID >= 0 && !rvFlag[lMetricID])
                {
                    lSelectedMetricCount++;
                    rvFlag[lMetricID] = true;
                }
            }
           
            lvPrevRowData.assign(lvData.begin(), lvData.end());
        }
    }
    else if (iAxis != 2 && (!ibIsMetricEmpty || ipLayoutInfo->mvLevelID.size() != 0) )	//lnUnits = 0 and not group-by
	{

        //the RWD in 475118
        if(lnUnits == 0 && ipLayoutInfo->mvLevelID.size() == 0)
        {
            ipLayoutInfo->setRowCount(lSize, false);
            lpTotalID = &ipLayoutInfo->mvSubTotalID[0];
            lpMetricID = &ipLayoutInfo->mvMetricID[0];
            lpLevels = &ipLayoutInfo->mvLevelID[0];
      
            if(rnRowList != lSize)
            {           
                delete rvRowList;
                rvRowList = new int[lSize];
                for(int i = 0; i < lSize; i ++)
                    rvRowList[i] = i;
                rnRowList = lSize;
                
                rvRowOffset[rnRowOffset - 1] = lSize;
            }
        }
        hr = mpReadStream->Skip(lSize * sizeof(int));
        CHECK_HR;          
        
        *lpMetricID = -1;
        *lpTotalID = 0;
        *lpLevels = 0;

        lvLevelRange.push_back(0);
        lnDistinctRows = 1;
        vector<int> lvLevelUnitsID;
        ipLayoutInfo->mvLevelUnitsID.push_back(lvLevelUnitsID);
        ipLayoutInfo->mvSubtotalFlag.push_back(0);
        ipLayoutInfo->mvLevelRowRange.push_back(0);

    }
    
    if((lSize == 0 && lnUnits == 0) || (ibIsMetricEmpty && lSize == 0 && lnStaticUnits == 0 && lnUnits >= 0))
    {
        if(rvRowList)
        {
            delete rvRowList;
            rvRowList = NULL;
        }
        for(int j = 0; j < rnRowOffset; j++)
            rvRowOffset[j] = 0;
        rnRowList = 0;
        ipLayoutInfo->setRowCount(lSize, false);
        ipLayoutInfo->mvLevelUnitsID.clear();
        ipLayoutInfo->mvSubtotalFlag.clear();
        lvLevelRange.clear();
        
    }

    ipLayoutInfo->mTotalLevels = lvLevelRange.size();
    lvLevelRange.push_back(lnDistinctRows);		//add end range of the last level
    
    ipLayoutInfo->mvLevelRowRange.assign(lvLevelRange.begin(), lvLevelRange.end());

    
    if(lbHasMetric)
    {
        ipLayoutInfo->mvSelectedMetricFlag.assign(rvFlag.begin(), rvFlag.end());
        ipLayoutInfo->mSelectedMetricCount = lSelectedMetricCount;
    }
     
 	return hr;
}

/*input: keyTable
 output: sorted / distinct row list [0, N) or a new key table based on the distinct rows?
 xtab header w/ indices
 map from original rows in keytable to header row number
 */
int hCreateXTabHeader2(LayoutInfo *ipLayout, DSSDataModelXTabImpl *ipXTabModel,int* ipRowList, int inRowList, int* ipOffset, int inOffset, int ibIsSorted, bool ibHasDummyUnit)
{
	int hr = S_OK;
	
	int lnKeySize = (*ipLayout).vUnits.size();
	int *lpKeyUnits = &ipLayout->vUnits[0];
	
	if (lnKeySize == 0)
	{
		//TODO:
	}
	
	//1. initialize:
	int lnRows = (*ipLayout).mvLevelID.size();
	if (lnRows <= 0)
	{
		//409716, after UC selection, model may be empty.  temporary fix: return false
		//return S_FALSE;		//TODO[enh]:
	}
	
	if (lnKeySize > 0 && lnRows != (*ipLayout).mvKeys.size() / lnKeySize)
		return E_FAIL;
	
	vector<int> lvSortedRows;		//its content is the original row id, size is lnRows
	vector<int> lvDistinctRows;		//its content is the origianl row id, size is the distinct rows
	vector<int> lvLevelRange;		//its content is the offset in lvDistinctRows, size is the number of levels
	
	//2. sort the original rows		TODO[opt]: use bucket sort
	int *lpRKeys = &((*ipLayout).mvKeys[0]);	//row header keys
	
	if(ibIsSorted == 1)
	{
		//the rows are already sorted
		//for(int i = 0; i < lnRows; i++)
		//	lvSortedRows.push_back(i);
		//3. select distinct
        int lMaxRowIndex = -1;
        bool lbFullGrid = false;
        if(ipLayout->metricsPos < 0)
        {
            for(int i = 0; i < inRowList && ipRowList; i++)
            {
                if(ipRowList[i]> lMaxRowIndex)
                    lMaxRowIndex = ipRowList[i];
                if(lMaxRowIndex == inRowList - 1)
                    lbFullGrid = true;
                
            }
        }
        hSelectDistinctRowsAndLevels2(ipLayout, ipRowList, inRowList, ipOffset, inOffset,lnRows, lvDistinctRows, lvLevelRange, ibHasDummyUnit, lbFullGrid);
	}
    //actually ibIsSorted is always 1
	else
	{
		hSortHeader(lvSortedRows, lnRows, ipLayout);
		//3. select distinct
		vector<int> lvRowRange;
		hSelectDistinctRowsAndLevels(ipLayout, lvRowRange, lvSortedRows, lvDistinctRows, lvLevelRange);
	}
	
    int lnLevels = lvLevelRange.size();
    ipLayout->mTotalLevels = lnLevels;			//TODO: remove mTotalLevels
    
    int lnDistinctRows = lvDistinctRows.size();
    lvLevelRange.push_back(lnDistinctRows);		//add end range of the last level
    
    ipLayout->mvLevelRowRange.assign(lvLevelRange.begin(), lvLevelRange.end());
      

    //4. get the subtotal's local ID;
    vector<int> lvSubtotalFlag(lnLevels);
    int i = 0;
    for (i = 0; i < lnLevels; i++)
    {
        lvSubtotalFlag[i] = ipLayout->mvSubTotalID[lvDistinctRows[lvLevelRange[i]]];
    }
    
    //5. get each level's units
    int lMetricPos = (*ipLayout).metricsPos;
    vector<vector<int> > lvLevelUnits(lnLevels);
    for (i = 0; i < lnLevels; i++)
    {
        int *lpKeys = lpRKeys + lnKeySize * lvDistinctRows[lvLevelRange[i]];
        for (int j = lnKeySize - 1; j >= 0; j--)
        {
            if (lpKeys[j] >= 0 && j != lMetricPos)
                lvLevelUnits[i].push_back(lpKeyUnits[j]);
        }
        std::sort(lvLevelUnits[i].begin(), lvLevelUnits[i].end());
    }
    ipLayout->mvLevelUnitsID.assign(lvLevelUnits.begin(), lvLevelUnits.end());
    
	//6. create index
	vector<DSSTabularIndex *> lvIndices (lnLevels);
	hr = hCreateIndex(ipLayout, ipXTabModel, lnLevels, ipLayout->mvLevelUnitsID, ipLayout->mvLevelRowRange, lvDistinctRows, lvIndices);
	CHECK_HR;
	
	//7. create xtab header	
	DSSXTabView *lpXTabView = ipXTabModel->getXTabView();
	CHECK_PTR(lpXTabView);
	
	DSSXTabHeader *lpXTabHeader = lpXTabView->getHeader(ipLayout->axis);
	CHECK_PTR(lpXTabHeader);
	
	int lMetricsUnitID = -1;
	if (ipLayout->metricsPos >= 0)
		lMetricsUnitID = ipLayout->vUnits[ipLayout->metricsPos];
	
	hr = lpXTabHeader->Init(ipLayout->vUnits, ipLayout->mnRuntimeUnits, lMetricsUnitID, lnDistinctRows, lvIndices, lvSubtotalFlag, ipXTabModel);
	CHECK_HR;
	
	int lnMetrics = 1;
	{
		if (ipLayout->metricsPos >= 0)
		{
			DSSTabularMetrics *lpMetrics = NULL;
			ipXTabModel->getMetricsUnit(&lpMetrics);
			CHECK_PTR(lpMetrics);	
			lnMetrics = lpMetrics->Count();
			
			//sanity check for metric selector for partial data loading (current slice only)
			if (lnMetrics > 0 && lnRows > 0)
			{
				vector<bool> lvFlags(lnMetrics);				//flag to indicate which metric is selected
				for (int j = 0; j < lnMetrics; j++)
					lvFlags[j] = false;
				
				int lSelectedMetricCount = 0;					//count how many metrics are selected
				int next = -1;
				if (lnDistinctRows > 1)
					next = lvDistinctRows[1];		//only check the first set of distinct rows
				
				int i = 0;
				while (i < lnRows)
				{
					int cur;
					if(ibIsSorted)
					{
						cur = i;
					}
					else 
					{
						cur = lvSortedRows[i];
					}

					if (cur == next)
						break;
					
					int lMetricID = lpRKeys[cur * lnKeySize + lMetricPos];
					if (lMetricID < lnMetrics && lMetricID >= 0 && !lvFlags[lMetricID])
					{
						lSelectedMetricCount++;
						lvFlags[lMetricID] = true;
						
						if (lSelectedMetricCount == lnMetrics)	//all metrics are selected, break
							break;
					}
					i++;
				}
				if (lSelectedMetricCount < lnMetrics)
				{
					hr = lpXTabHeader->setGroupbyMetric(lSelectedMetricCount, lvFlags);
					CHECK_HR;
				}
				lnMetrics = lSelectedMetricCount;
			}
		}
		
		AE_ASSERT (lnMetrics != 0);
		
	
		if(ibIsSorted)
		{
			//ipLayout->mvMap2DisplayedRows.assign(rvRowList.begin(), rvRowList.end());
		}
		else
		{
			ipLayout->mvMap2DisplayedRows.resize(lnRows);
			int *lpMap = NULL;
			if (lnRows > 0)
				lpMap = &ipLayout->mvMap2DisplayedRows[0];
			
			if (lnDistinctRows >= 1)
			{
				int rowID = 0;
				for (int j = 1; j <= lnDistinctRows; j++)
				{
					int i = rowID;
					int cur; //= lvSortedRows[i];
					int next = -1;
					if (j < lnDistinctRows)
						next = lvDistinctRows[j];
					
					int iMetric = 0;
					while (i < lnRows)
					{
						cur = lvSortedRows[i];
						if (cur == next)		//break once reach the next
							break;
						
						lpMap[cur] = (j - 1) * lnMetrics + iMetric;
						i++;
					}
					
					int lnRecurrence = (i - rowID) / lnMetrics;			//number of duplicated rows
					AE_ASSERT(lnMetrics * lnRecurrence == (i - rowID));
					
					if (lnRecurrence > 1)		//need reset row ID if there're duplicated rows
					{
						i = rowID;
						//cur = lvSortedRows[i];
						iMetric = 0;
						while (i < lnRows)
						{
							cur = lvSortedRows[i];
							if (cur == next)		//break once reach the next
								break;
							
							for (int ir = 0; ir < lnRecurrence && i < lnRows; ir++)
							{
								cur = lvSortedRows[i];
								lpMap[cur] = (j - 1) * lnMetrics + iMetric;
								i++;
							}
							iMetric++;
						}
					}//end of if (lnRecurrence > 1)
					rowID = i;
				}//end of for (j = 0; j <= lnDistinctRows; j++)
			}
			else if (lnMetrics > 0)	//the header only has metrics or there's only one distinct row
			{
				int lnRecurrence = lnRows / lnMetrics;
				_ASSERT(lnMetrics * lnRecurrence == lnRows);
				{
					int iMetric = 0;
					while (i < lnRows)
					{
						for (int ir = 0; ir < lnRecurrence; ir++)
						{
							lpMap[lvSortedRows[i++]] = iMetric;
						}
						iMetric++;
					}
				}
			}
			
		}

	}
	return 0;
}
int hCreateXTabHeader4(DSSBinaryDataReader* ipStream, LayoutInfo *ipLayout, DSSDataModelXTabImpl *ipXTabModel, int*& rvList, int& rnList, int*& rvOffset, int& rnOffset,int iHeaderSize)
{
    int hr = S_OK;
      
    int lVer = 0;
    hr = ipStream->ReadInt(&lVer);
    CHECK_HR;
    
    int lnRow = 0;
    hr = ipStream->ReadInt(&lnRow);
    CHECK_HR;
    
    _ASSERT(lnRow >= 0);
    
    if(lVer == 0)
    {
        //load metric id
        int lSize = 0;
        hr = ipStream->ReadInt(&lSize);
        CHECK_HR;
        _ASSERT(lSize == lnRow);
        ipLayout->mvMetricID.resize(lSize);
        if(lSize > 0)
        {
            hr = ipStream->ReadByteArray(&ipLayout->mvMetricID[0], lSize);
            CHECK_HR;
        }
        
        //load level id
        lSize = 0;
        hr = ipStream->ReadInt(&lSize);
        CHECK_HR;
        _ASSERT(lSize == lnRow);
        ipLayout->mvLevelID.resize(lSize);
        if(lSize > 0)
        {
            hr = ipStream->ReadShortArray(&ipLayout->mvLevelID[0], lSize);
            CHECK_HR;
        }
        
        //load total id
        lSize = 0;
        hr = ipStream->ReadInt(&lSize);
        CHECK_HR;
        _ASSERT(lSize == lnRow);
        ipLayout->mvSubTotalID.resize(lSize);
        if(lSize > 0)
        {
            hr = ipStream->ReadByteArray(&ipLayout->mvSubTotalID[0], lSize);
            CHECK_HR;
        }
    }
    else if(lVer == 1)
    {
        //load metric id
       
        int lSize = 0;
               
        hr = ipStream->ReadInt(&lSize);
        CHECK_HR;
        vector<int> lvRowIndex(lSize); 
        if(lSize > 0)
        {
            hr = ipStream->ReadIntArray(&lvRowIndex[0], lSize);
            CHECK_HR;
        }
        //load metric id
        lSize = 0;
        hr = ipStream->ReadInt(&lSize);
        CHECK_HR;
        vector<unsigned char> lvMetricID(lSize); 
        if(lSize > 0)
        {
            hr = ipStream->ReadByteArray(&lvMetricID[0], lSize);
            CHECK_HR;
        }
        
        //load level id
        
        lSize = 0;
        hr = ipStream->ReadInt(&lSize);
        CHECK_HR;
        vector<short> lvLevelID(lSize);
        if(lSize > 0)
        {
            hr = ipStream->ReadShortArray(&lvLevelID[0], lSize);
            CHECK_HR;
        } 
        
        //load total id
        lSize = 0;
        hr = ipStream->ReadInt(&lSize);
        CHECK_HR;
        vector<unsigned char> lvSubtotalID(lSize);
        if(lSize > 0)
        {
            hr = ipStream->ReadByteArray(&lvSubtotalID[0], lSize);
            CHECK_HR;
        }

        ipLayout->setRowCount(lnRow,false,false);

        memset(&ipLayout->mvMetricID[0], -1, sizeof(unsigned char) * lnRow);
        memset(&ipLayout->mvLevelID[0],0, sizeof(short)*lnRow);
   //     memset(&ipLayout->mvSubTotalID[0], 0, sizeof(unsigned char)*lnRow);
        for(int i= 0; i < lvRowIndex.size(); i++)
        {
            if(lvRowIndex[i] < 0 || lvRowIndex[i] >= lnRow)
                continue;
            ipLayout->mvMetricID[lvRowIndex[i]] = lvMetricID[i];
    //        ipLayout->mvSubTotalID[lvRowIndex[i]] = lvSubtotalID[i];
            ipLayout->mvLevelID[lvRowIndex[i]] = lvLevelID[i];
        }
  
    }
    else
    {
        _ASSERT(false);
    }
    
    //get metric unit index
	int lMetricsUnitID = ipXTabModel->getMetricUnitID();
	if (lMetricsUnitID >= 0)	//410114
	{
		DSSTabularMetrics *lpMetrics = NULL;
		ipXTabModel->getMetricsUnit(&lpMetrics);
		CHECK_PTR(lpMetrics);	
		if (lpMetrics->Count() <= 0)
			lMetricsUnitID = -1;
	}
	bool lbIsMetricEmpty = lMetricsUnitID < 0;
    //
    
    if(iHeaderSize == 0 && (ipLayout->vUnits.size() == 0 || (ipLayout->vUnits.size() == ipLayout->mnRuntimeUnits)) && lbIsMetricEmpty)
    {
//        int lReservedSize = rnOffset * sizeof(int);
//MEM_GOVERNOR_RESERVE(lReservedSize, lReservedSize)
        int* lpvRowOffset = new int[rnOffset];
//MEM_GOVERNOR_COMMIT(lReservedSize)
        memset(lpvRowOffset, 0, sizeof(int) * rnOffset);
        ipXTabModel->clearRowList(ipLayout->axis);
        rvOffset = lpvRowOffset;
        rvList = NULL;
        rnList = 0;
        for(int j = 0; j < rnOffset; j++)
            rvOffset[j] = 0;
        ipXTabModel->setRowList(ipLayout->axis, rvList, rnList, rvOffset, rnOffset, false);
    }
    
    vector<int> lvLevelRange;
    hr = ipStream->ReadIntArray(lvLevelRange);
    CHECK_HR;
    
    vector<int> lvSubtotalFlag;
    hr = ipStream->ReadIntArray(lvSubtotalFlag);
    CHECK_HR;
    
    int lSize = 0;
    hr = ipStream->ReadInt(&lSize);
    CHECK_HR;
    
    vector<vector<int> > lvLevelUnitsID(lSize);
    for(int i = 0; i < lSize; i++)
    {
        hr = ipStream->ReadIntArray(lvLevelUnitsID[i]);
        CHECK_HR;
    }
    
    vector<int> lvDuplicatedRow;
    hr = ipStream->ReadIntArray(lvDuplicatedRow);
    CHECK_HR;
    
    _ASSERT(lnRow >= lvDuplicatedRow.size());
    
    int lnLevels = lvLevelRange.size();
    ipLayout->mTotalLevels = lnLevels;
//    lvLevelRange.push_back(lnRow - lvDuplicatedRow.size());
    lvLevelRange.push_back(lnRow);
    
    
    lMetricsUnitID = -1;
	if (ipLayout->metricsPos >= 0)
		lMetricsUnitID = ipLayout->vUnits[ipLayout->metricsPos];
  
    int lnMetrics = -1;
    DSSTabularMetrics *lpMetrics = NULL;
    ipXTabModel->getMetricsUnit(&lpMetrics);
    
    if(lpMetrics)
        lnMetrics = lpMetrics->Count();

    vector<int> lvDistinctRows;
    int lnDistinctRow = lnRow;
    if(lvDuplicatedRow.size() > 0)// && ipLayout->axis == 0)
    {
        if(lMetricsUnitID >= 0 && lnMetrics > 0)
        {
            if(lnRow / lnMetrics == lnRow - lvDuplicatedRow.size())//if duplicated rows are only caused by metric unit
            {
                int j = 0;
                for(int i = 0;i < lnRow ; i++)
                {
                    if(j < lvDuplicatedRow.size() && lvDuplicatedRow[j] == i)
                        j++;
                    else
                        lvDistinctRows.push_back(i);
                }
                vector<int> lvOriginalRows;
                j = 0;
                int i = 0;
                int k = 0;
                while(lvOriginalRows.size() < lvDuplicatedRow.size())
                {
                    AE_ASSERT( j < lvDuplicatedRow.size());
                    if(i < lvDuplicatedRow[j])
                    {
                        k = i;
                    }
                    else
                    {
                        lvOriginalRows.push_back(k);
                        j++;
                    }
                    i++;
                }
                lvOriginalRows.resize(lvDuplicatedRow.size());
            
                hr = ipXTabModel->setDuplicatedRows(ipLayout->axis, lvDuplicatedRow, lvOriginalRows);
                CHECK_HR;
                
                lvLevelRange[lvLevelRange.size()-1]/=lnMetrics;
                lnDistinctRow /= lnMetrics;
            }
        }
    }
   
    // create index
    vector<DSSTabularIndex *> lvIndices (lnLevels);
	hr = hCreateIndex(ipLayout, ipXTabModel, lnLevels, lvLevelUnitsID, lvLevelRange, lvDistinctRows, lvIndices, true);
	CHECK_HR;
	
	//7create xtab header	
	DSSXTabView *lpXTabView = ipXTabModel->getXTabView();
	CHECK_PTR(lpXTabView);
	
	DSSXTabHeader *lpXTabHeader = lpXTabView->getHeader(ipLayout->axis);
	CHECK_PTR(lpXTabHeader);


	hr = lpXTabHeader->Init(ipLayout->vUnits, ipLayout->mnRuntimeUnits, lMetricsUnitID, lnDistinctRow, lvIndices, lvSubtotalFlag, ipXTabModel);
	CHECK_HR;
	
    vector<bool> rvFlag;
    rvFlag.resize(lnMetrics);
    for(int i = 0; i < lnMetrics; i++)
        rvFlag[i] = false;
    int lMetricPos = ipLayout->metricsPos;
    
	int lnUnits = ipLayout->vUnits.size();
	bool lbHasMetric = (lMetricPos >= 0 && lMetricPos < lnUnits);

    int lSelectedMetricCount = 0;
    for(int i = 0;i  < lnRow; i++)
    {
        if(lbHasMetric && rvFlag.size() > 0 && lSelectedMetricCount != rvFlag.size())
        {
            int lMetricID = ipLayout->mvMetricID[i];
            if (lMetricID < rvFlag.size() && lMetricID >= 0 && !rvFlag[lMetricID])
            {
                lSelectedMetricCount++;
                rvFlag[lMetricID] = true;
            }
        }
        if(lSelectedMetricCount == rvFlag.size())
            break;
    }
    
    if (ipLayout->metricsPos >= 0)
    {
        DSSTabularMetrics *lpMetrics = NULL;
        hr = ipXTabModel->getMetricsUnit(&lpMetrics);
        CHECK_PTR(lpMetrics);	
        int lnMetrics = lpMetrics->Count();
        
        if (lSelectedMetricCount < lnMetrics)
        {
            hr = lpXTabHeader->setGroupbyMetric(lSelectedMetricCount, rvFlag);
            CHECK_HR;
        }
    }
    vector<short> lTempLevelID;
    vector<unsigned char> lTempMetricID;
    ipLayout->mvLevelID.swap(lTempLevelID);
    ipLayout->mvMetricID.swap(lTempMetricID);
 //   ipLayout->mvLevelID.resize(0);
//    ipLayout->mvSubTotalID.resize(0);
   // ipLayout->mvMetricID.resize(0);
    
    return hr;
}
//the headkeys are not loaded
int hCreateXTabHeader3(LayoutInfo *ipLayout, DSSDataModelXTabImpl *ipXTabModel)
{
	int hr = S_OK;
	
	//1. initialize:
	int lnRows = (*ipLayout).mvLevelID.size();
	if (lnRows <= 0)
	{
		//409716, after UC selection, model may be empty.  temporary fix: return false
		//return S_FALSE;		//TODO[enh]:
	}
	
	vector<int> lvLevelRange;		//its content is the offset in lvDistinctRows, size is the number of levels
	
    int lnLevels = ipLayout->mvLevelRowRange.size() - 1;
    //the axis is empty
    if(lnLevels < 0)
        lnLevels = 0;
    //4. get the subtotal's local ID
    //5. get each level's units
   	//6. create index
	vector<DSSTabularIndex *> lvIndices (lnLevels);
    vector<int> lvDistinctRows(0); //dummy
	hr = hCreateIndex(ipLayout, ipXTabModel, lnLevels, ipLayout->mvLevelUnitsID, ipLayout->mvLevelRowRange, lvDistinctRows, lvIndices, true);
	CHECK_HR;
	
	//7. create xtab header	
	DSSXTabView *lpXTabView = ipXTabModel->getXTabView();
	CHECK_PTR(lpXTabView);
	
	DSSXTabHeader *lpXTabHeader = lpXTabView->getHeader(ipLayout->axis);
	CHECK_PTR(lpXTabHeader);
	
	int lMetricsUnitID = -1;
	if (ipLayout->metricsPos >= 0)
		lMetricsUnitID = ipLayout->vUnits[ipLayout->metricsPos];
	
	hr = lpXTabHeader->Init(ipLayout->vUnits, ipLayout->mnRuntimeUnits, lMetricsUnitID, lnRows, lvIndices, ipLayout->mvSubtotalFlag, ipXTabModel);
	CHECK_HR;
	

    if (ipLayout->metricsPos >= 0)
    {
        DSSTabularMetrics *lpMetrics = NULL;
        ipXTabModel->getMetricsUnit(&lpMetrics);
        CHECK_PTR(lpMetrics);	
        int lnMetrics = lpMetrics->Count();
        
        if (ipLayout->mSelectedMetricCount < lnMetrics)
        {
            hr = lpXTabHeader->setGroupbyMetric(ipLayout->mSelectedMetricCount, ipLayout->mvSelectedMetricFlag);
            CHECK_HR;
        }
    }

	return 0;
}

int hCreateXTabHeader(LayoutInfo *ipLayout, DSSDataModelXTabImpl *ipXTabModel, vector<int>& irvRowRange, bool ibHasDummy)
{
	int hr = S_OK;

	int lnKeySize = (*ipLayout).vUnits.size();
	int *lpKeyUnits = &ipLayout->vUnits[0];

	if (lnKeySize == 0)
	{
		//TODO:
	}

	//1. initialize:
	int lnRows = (*ipLayout).mvLevelID.size();
	if (lnRows <= 0)
	{
		//409716, after UC selection, model may be empty.  temporary fix: return false
		//return S_FALSE;		//TODO[enh]:
	}

	if (lnKeySize > 0 && lnRows != (*ipLayout).mvKeys.size() / lnKeySize)
		return E_FAIL;

	vector<int> lvSortedRows;		//its content is the original row id, size is lnRows
	vector<int> lvDistinctRows;		//its content is the origianl row id, size is the distinct rows
	vector<int> lvLevelRange;		//its content is the offset in lvDistinctRows, size is the number of levels

	//2. sort the original rows		TODO[opt]: use bucket sort
	int *lpRKeys = &((*ipLayout).mvKeys[0]);	//row header keys
	hSortHeader(lvSortedRows, lnRows, ipLayout);

	//3. select distinct
	hSelectDistinctRowsAndLevels(ipLayout, irvRowRange, lvSortedRows, lvDistinctRows, lvLevelRange, ibHasDummy);


	int lnLevels = lvLevelRange.size();
	ipLayout->mTotalLevels = lnLevels;			//TODO: remove mTotalLevels

	int lnDistinctRows = lvDistinctRows.size();
	lvLevelRange.push_back(lnDistinctRows);		//add end range of the last level

	ipLayout->mvLevelRowRange.assign(lvLevelRange.begin(), lvLevelRange.end());

	//4. get the subtotal's local ID;
	vector<int> lvSubtotalFlag(lnLevels);
	int i = 0;
	for (i = 0; i < lnLevels; i++)
	{
		lvSubtotalFlag[i] = ipLayout->mvSubTotalID[lvDistinctRows[lvLevelRange[i]]];
	}

	//5. get each level's units
	int lMetricPos = (*ipLayout).metricsPos;
	vector<vector<int> > lvLevelUnits(lnLevels);
	for (i = 0; i < lnLevels; i++)
	{
		int *lpKeys = lpRKeys + lnKeySize * lvDistinctRows[lvLevelRange[i]];
		for (int j = lnKeySize - 1; j >= 0; j--)
		{
			if (lpKeys[j] >= 0 && j != lMetricPos)
				lvLevelUnits[i].push_back(lpKeyUnits[j]);
		}
		std::sort(lvLevelUnits[i].begin(), lvLevelUnits[i].end());
	}
	ipLayout->mvLevelUnitsID.assign(lvLevelUnits.begin(), lvLevelUnits.end());

	//6. create index
	vector<DSSTabularIndex *> lvIndices (lnLevels);
	hr = hCreateIndex(ipLayout, ipXTabModel, lnLevels, lvLevelUnits, lvLevelRange, lvDistinctRows, lvIndices);
	CHECK_HR;

	//7. create xtab header
	DSSXTabView *lpXTabView = ipXTabModel->getXTabView();
	CHECK_PTR(lpXTabView);

	DSSXTabHeader *lpXTabHeader = lpXTabView->getHeader(ipLayout->axis);
	CHECK_PTR(lpXTabHeader);

	int lMetricsUnitID = -1;
	if (ipLayout->metricsPos >= 0)
		lMetricsUnitID = ipLayout->vUnits[ipLayout->metricsPos];

	hr = lpXTabHeader->Init(ipLayout->vUnits, ipLayout->mnRuntimeUnits, lMetricsUnitID, lnDistinctRows, lvIndices, lvSubtotalFlag, ipXTabModel);
	CHECK_HR;

	//map to the displayed rows
	int lnMetrics = 1;
	{
		if (ipLayout->metricsPos >= 0)
		{
			DSSTabularMetrics *lpMetrics = NULL;
			ipXTabModel->getMetricsUnit(&lpMetrics);
			CHECK_PTR(lpMetrics);
			lnMetrics = lpMetrics->Count();

			//sanity check for metric selector for partial data loading (current slice only)
			if (lnMetrics > 0 && lnRows > 0)
			{
				vector<bool> lvFlags(lnMetrics);				//flag to indicate which metric is selected
				for (int j = 0; j < lnMetrics; j++)
					lvFlags[j] = false;

				int lSelectedMetricCount = 0;					//count how many metrics are selected
				int next = -1;
				if (lnDistinctRows > 1)
					next = lvDistinctRows[1];		//only check the first set of distinct rows

				int i = 0;
				while (i < lnRows)
				{
					int cur = lvSortedRows[i];
					if (cur == next)
						break;

					int lMetricID = lpRKeys[cur * lnKeySize + lMetricPos];
					if (lMetricID < lnMetrics && lMetricID >= 0 && !lvFlags[lMetricID])
					{
						lSelectedMetricCount++;
						lvFlags[lMetricID] = true;

						if (lSelectedMetricCount == lnMetrics)	//all metrics are selected, break
							break;
					}
					i++;
				}
				if (lSelectedMetricCount < lnMetrics)
				{
					hr = lpXTabHeader->setGroupbyMetric(lSelectedMetricCount, lvFlags);
					CHECK_HR;
				}
				lnMetrics = lSelectedMetricCount;
			}
		}

		AE_ASSERT (lnMetrics != 0);

		ipLayout->mvMap2DisplayedRows.resize(lnRows);
		int *lpMap = NULL;
		if (lnRows > 0)
			lpMap = &ipLayout->mvMap2DisplayedRows[0];

		if (lnDistinctRows >= 1)
		{
			int rowID = 0;
			for (int j = 1; j <= lnDistinctRows; j++)
			{
				int i = rowID;
				int cur; //= lvSortedRows[i];
				int next = -1;
				if (j < lnDistinctRows)
					next = lvDistinctRows[j];

				int iMetric = 0;
				while (i < lnRows)
				{
					cur = lvSortedRows[i];
					if (cur == next)		//break once reach the next
						break;

					lpMap[cur] = (j - 1) * lnMetrics + iMetric++;	//set the row ID
					i++;
				}

				int lnRecurrence = (i - rowID) / lnMetrics;			//number of duplicated rows
				AE_ASSERT(lnMetrics * lnRecurrence == (i - rowID));

				if (lnRecurrence > 1)		//need reset row ID if there're duplicated rows
				{
					i = rowID;
					//cur = lvSortedRows[i];
					iMetric = 0;
					while (i < lnRows)
					{
						cur = lvSortedRows[i];
						if (cur == next)		//break once reach the next
							break;

						for (int ir = 0; ir < lnRecurrence && i < lnRows; ir++)
						{
							cur = lvSortedRows[i];
							lpMap[cur] = (j - 1) * lnMetrics + iMetric;
							i++;
						}
						iMetric++;
					}
				}//end of if (lnRecurrence > 1)
				rowID = i;
			}//end of for (j = 0; j <= lnDistinctRows; j++)
		}
		else if (lnMetrics > 0)	//the header only has metrics or there's only one distinct row
		{
			int lnRecurrence = lnRows / lnMetrics;
			_ASSERT(lnMetrics * lnRecurrence == lnRows);
			{
				int iMetric = 0;
				while (i < lnRows)
				{
					for (int ir = 0; ir < lnRecurrence; ir++)
					{
						lpMap[lvSortedRows[i++]] = iMetric;
					}
					iMetric++;
				}
			}
		}
	}

	return 0;
}

void hSortHeader(vector<int> &irvSortedRows, int lnRows, LayoutInfo *ipLayout)
{
	irvSortedRows.resize(lnRows);
	int i = 0;
	for (i = 0; i < lnRows; i++)
		irvSortedRows[i] = i;

	//2. sort the original rows		TODO[opt]: use bucket sort
	int lnKeySize = (*ipLayout).vUnits.size();
	int *lpRKeys = &((*ipLayout).mvKeys[0]);	//row header keys
	ElementSort<int, int> lSorter(lpRKeys, lnRows, lnKeySize);
	int lMetricPos = (*ipLayout).metricsPos;
	if (lMetricPos >= 0)
	{
		lSorter.SetUnit(lMetricPos);	//sort by metrics first
		_xtab_sort(&irvSortedRows[0], &irvSortedRows[0] +  lnRows, lSorter);	//merge sort
	}
	int *lpUnitSortOrder = lnKeySize > 0 ? &ipLayout->mvUnitSortOrder[0] : NULL;
	for (i = lnKeySize - 1; i >= 0; --i)
	{
		int unit_id = lpUnitSortOrder[i];
		//if (i != lMetricPos)
		if (unit_id != lMetricPos)
		{
			lSorter.SetUnit(unit_id);			//only sort by attributes
			//lSorter.SetUnit(i);			//only sort by attributes
			_xtab_sort(&irvSortedRows[0], &irvSortedRows[0] +  lnRows, lSorter);	//merge sort
		}
	}
	short *lpLevelID = &((*ipLayout).mvLevelID[0]);
	unsigned char *lpSubtotalID = &(ipLayout->mvSubTotalID[0]);
	ElementSort<short, unsigned char> lLevelSorter(lpLevelID, lnRows, 1, lpSubtotalID);
	_xtab_sort(&irvSortedRows[0], &irvSortedRows[0] + lnRows, lLevelSorter);	//sort by level
}

void hSelectDistinctRowsAndLevels2(LayoutInfo *ipLayout,int* ipRowList, int nRowList, int* ipOffset, int nOffset, int inRows, vector<int> &orvDistinctRows, vector<int> &orvLevelRange, bool ibHasDummy, bool iFullGrid)
{
	int lnRows = inRows;
	int lnKeySize = (*ipLayout).vUnits.size();
    
    vector<int> lvMapRowToSlice;
	if(ibHasDummy)
	{
		lvMapRowToSlice.resize(nRowList);
		for(int i = 1; i < nOffset - 1; i++)
		{
            for(int j = ipOffset[i]; j < ipOffset[i+1]; j++)
            {
                if(ipRowList[j] > lvMapRowToSlice.size())
                   lvMapRowToSlice.resize(ipRowList[j] + 1);
                
                lvMapRowToSlice[ipRowList[j]] = i;
            }
		}
	}
    
	int *lpRKeys = NULL;
	int *lpPre = NULL;
	int *lpCur = NULL;
	if (lnRows > 0)
	{
		lpRKeys = &((*ipLayout).mvKeys[0]);	
		lpPre = lpRKeys + lnKeySize * 0;
		orvDistinctRows.push_back(0);
		orvLevelRange.push_back(0);
	}
	int *lpUnitSortOrder = lnKeySize > 0 ? &ipLayout->mvUnitSortOrder[0] : NULL;
	
	short *lpLevelID = &((*ipLayout).mvLevelID[0]);
	unsigned char *lpSubtotalID = &(ipLayout->mvSubTotalID[0]);
	int lMetricPos = (*ipLayout).metricsPos;
    
    int lPreRowID = -1;
	if (lnRows > 0)	
		lPreRowID = 0;
	for (int i = 1; i < lnRows; i++)
	{
        int lRowID = i;
		lpCur = lpRKeys + lnKeySize * lRowID;
        bool lbDistinct = false;
        int lnDistinctUnit = 0;
		for (int j = lnKeySize -1; j >= 0; --j)
		{
			int unit_id = lpUnitSortOrder[j];
            if(lpCur[unit_id] != lpPre[unit_id])
                lnDistinctUnit ++;
			//if (lpCur[j] != lpPre[j] && j != lMetricPos)	//ignore metrics; 
			if (lpCur[unit_id] != lpPre[unit_id] && unit_id != lMetricPos)	//ignore metrics; 
			{
				orvDistinctRows.push_back(lRowID);
				lbDistinct = true;
				if (lpLevelID[lRowID] != lpLevelID[i - 1]
					|| lpSubtotalID[lRowID] != lpSubtotalID[i - 1])			//the level (include subtotal type) is changed
				{
					orvLevelRange.push_back(orvDistinctRows.size() - 1);
				}
				break;
			}
		}
        
        if(!lbDistinct &&
           ((ibHasDummy && lvMapRowToSlice[lRowID] == lvMapRowToSlice[lPreRowID] && !(lMetricPos >= 0 && lnDistinctUnit == 1))
            ||iFullGrid))
		{
			orvDistinctRows.push_back(lRowID);
		}
		
		lpPre = lpCur;
		lPreRowID = lRowID;
	}
}


void hSelectDistinctRowsAndLevels(LayoutInfo *ipLayout, vector<int>& irvRowRange,  vector<int> &irvSortedRows, vector<int> &orvDistinctRows, vector<int> &orvLevelRange, bool ibHasDummy)
{
	int lnRows = irvSortedRows.size();
	int lnKeySize = (*ipLayout).vUnits.size();

	vector<int> lvMapRowToSlice;
	if(ibHasDummy)
	{
		lvMapRowToSlice.resize(lnRows);
		int j = 0;
		for(int i = 1; i < irvRowRange.size(); i++)
		{
			while(j < irvRowRange[i])
			{
				lvMapRowToSlice[j++] = i - 1;
			}
		}
	}
			
	int *lpRKeys = NULL;
	int *lpPre = NULL;
	int *lpCur = NULL;
	if (lnRows > 0)
	{
		lpRKeys = &((*ipLayout).mvKeys[0]);
		lpPre = lpRKeys + lnKeySize * irvSortedRows[0];
		orvDistinctRows.push_back(irvSortedRows[0]);
		orvLevelRange.push_back(0);
	}
	int *lpUnitSortOrder = lnKeySize > 0 ? &ipLayout->mvUnitSortOrder[0] : NULL;

	short *lpLevelID = &((*ipLayout).mvLevelID[0]);
	unsigned char *lpSubtotalID = &(ipLayout->mvSubTotalID[0]);
	int lMetricPos = (*ipLayout).metricsPos;

	int lPreRowID = -1;
	if (lnRows > 0)	
		lPreRowID = irvSortedRows[0];
	for (int i = 1; i < lnRows; i++)
	{
		int lRowID = irvSortedRows[i];
		lpCur = lpRKeys + lnKeySize * lRowID;
		bool lbDistinct = false;
		for (int j = lnKeySize -1; j >= 0; --j)
		{
			int unit_id = lpUnitSortOrder[j];
			//if (lpCur[j] != lpPre[j] && j != lMetricPos)	//ignore metrics;
			if (lpCur[unit_id] != lpPre[unit_id] && unit_id != lMetricPos)	//ignore metrics;
			{
				orvDistinctRows.push_back(lRowID);
				lbDistinct = true;

				if (lpLevelID[lRowID] != lpLevelID[irvSortedRows[i - 1]]
					|| lpSubtotalID[lRowID] != lpSubtotalID[irvSortedRows[i - 1]])			//the level (include subtotal type) is changed
				{
					orvLevelRange.push_back(orvDistinctRows.size() - 1);
				}
				break;
			}
		}
		//if the 2 rows belong to different slices, they should be treated as duplicated rows
		//we can do this only because the sorting is done by merge sort
		if(!lbDistinct && ibHasDummy && lvMapRowToSlice[lRowID] == lvMapRowToSlice[lPreRowID])
		{
			orvDistinctRows.push_back(lRowID);
		}
		
		lpPre = lpCur;
		lPreRowID = lRowID;
	}
}

int hCreateIndex(LayoutInfo *ipLayout, DSSDataModelXTabImpl *ipXTabModel, int nLevels,
								  vector<vector<int> > &irvLevelUnits, vector<int> &irvLevelRange, vector<int> &irvDistinctRows, vector<DSSTabularIndex *> &orvIndices,
                                    bool ibLoadbySlice)
{
	DSSCube *lpCube = ipXTabModel->getCube();
	CHECK_PTR(lpCube);

	DSSTabularIndexPool *lpIndexPool = NULL;
	lpCube->getIndexPool(&lpIndexPool);
	CHECK_PTR(lpIndexPool);

    int hr = lpIndexPool->Init(ipXTabModel);
    CHECK_HR;
    
	int lnKeySize = (*ipLayout).vUnits.size();
	int *lpRKeys = &((*ipLayout).mvKeys[0]);	//row header keys

	for (int i = 0; i < nLevels; i++)
	{
		int lnIndexUnits = irvLevelUnits[i].size();
		if (i > 0 && lnIndexUnits == irvLevelUnits[i - 1].size())		//check levels if only subtotal type is different from the previous level
		{
			int k = 0;
			for (k = 0; k < lnIndexUnits; k++)
			{
				if (irvLevelUnits[i][k] != irvLevelUnits[i - 1][k])
					break;
			}
			if (k >= lnIndexUnits)	//same index
			{
				orvIndices[i] = orvIndices[i - 1];
				continue;
			}
		}
		vector<int> lvIndexUnitsMap;
		hr = LinearMapUnits(irvLevelUnits[i], ipLayout->vUnits, lvIndexUnitsMap);
		CHECK_HR;

		int start = irvLevelRange[i];
		int end = irvLevelRange[i + 1];
        
        DSSTabularIndex *lpIndex = NULL;		
        if(!ibLoadbySlice)
        {
            vector<int> lvKeys;
            lvKeys.resize(lnIndexUnits * (end - start));
            
            int *lpKeys = lnIndexUnits > 0 ? &lvKeys[0] : NULL;
            for (int j = start; j < end; j++)
            {
                int *lpK = lpRKeys + lnKeySize * irvDistinctRows[j];
                for (int k = 0; k < lnIndexUnits; k++)
                {
                    *lpKeys++ = lpK[lvIndexUnitsMap[k]];
                }
            }
            lpKeys = lnIndexUnits > 0 ? &lvKeys[0] : NULL;
            
            hr = lpIndexPool->CreateIndex(lnIndexUnits, end - start, &irvLevelUnits[i][0], lpKeys, &lpIndex);
            CHECK_HR;
		}
        else
        {
            hr = lpIndexPool->CreateIndex(lnIndexUnits, end - start, &irvLevelUnits[i][0], NULL, &lpIndex, ibLoadbySlice);
            CHECK_HR;
        }
		orvIndices[i] = lpIndex;
	}
	return S_OK;
}

int DSSModelPopulation::LoadFieldData(DSSDataModelFieldGroupImpl *ipFieldGroupModel, DSSBinaryDataReader *ipReadStream, int iRow)
{
	int hr = S_OK;
    int lnFields = ipFieldGroupModel->CountFields();
    
    bool ibHasStaticFieldLookup = ipFieldGroupModel->getHasStaticFieldLookup();
    
    int lSize = 0;
    hr = ipReadStream->ReadInt(&lSize);
    CHECK_HR;

    if (lSize == lnFields)
    {
        int count = iRow * lnFields;
        for (int f = 0; f < lnFields; f++)
        {
            int lobjtype = 0;
            hr = ipReadStream->ReadInt(&lobjtype);
            CHECK_HR;
            
            if (lobjtype != EnumObjectField)
                return E_UNEXPECTED_DATATYPE;
            
            int lThresholdID = 0;
            hr = ipReadStream->ReadInt(&lThresholdID);
            CHECK_HR;
            
            if (ibHasStaticFieldLookup)
            {
                int lnSubFields = 0;
                hr = ipReadStream->ReadInt(&lnSubFields);
                CHECK_HR;
                
                vector<MBase::CString *> lvSubFields(lnSubFields);
                for (int j = 0; j < lnSubFields; j++)
                {
                    MBase::CString *lpText = new MBase::CString;
                    ipReadStream->ReadString(*lpText);
                    
                    lvSubFields[j] = lpText;
                }
                
                ipFieldGroupModel->AddField(count, f, lvSubFields, lThresholdID);
            }
            else
            {
                MBase::CString *lpText = new MBase::CString;
                hr = ipReadStream->ReadString(*lpText);
                CHECK_HR;
                
                hr = ipFieldGroupModel->AddField(count, lpText, lThresholdID);
                if (hr != S_OK)
                    delete lpText;
                CHECK_HR;
            }
            
            int lnLinks = 0;
            hr = ipReadStream->ReadInt(&lnLinks);
            CHECK_HR;
            
            if (lnLinks == 0)
            {
                hr = ipReadStream->ReadInt(&lSize);
                CHECK_HR;
            }
            else if (lnLinks > 0)
            {
                FieldLink *lpLink = new FieldLink(lnLinks);
                hr = ipReadStream->ReadIntArray((int*)(&lpLink->mtype[0]), lnLinks);
                CHECK_HR;
                
                MBase::DSSStrongPtr<FieldLink> lpLinkHolder(lpLink);
                
                hr = ipReadStream->ReadInt(&lSize);
                CHECK_HR;
                
                if (ibHasStaticFieldLookup)
                {
                    vector<vector<MBase::CString *> > lvLinkText(lnLinks);
                    for (int k = 0; k < lnLinks; k++)
                    {
                        int lnSubLinks = 0;
                        hr = ipReadStream->ReadInt(&lnSubLinks);
                        CHECK_HR;
                        
                        lvLinkText[k].resize(lnSubLinks);
                        for (int s = 0; s < lnSubLinks; s++)
                        {
                            MBase::CString *lpText = new MBase::CString;
                            ipReadStream->ReadString(*lpText);
                            
                            lvLinkText[k][s] = lpText;
                        }
                        
                    }
                    hr = ipFieldGroupModel->AddLink(count, f, lpLink, lvLinkText);
                    CHECK_HR;
                }
                else
                {
                    for (int k = 0; k < lnLinks; k++)
                    {
                        MBase::CString *lpLinkText = new MBase::CString;
                        hr = ipReadStream->ReadString(*lpLinkText);
                        CHECK_HR;
                        //link text
                        //TODO:[test] 359130
                        
                        lpLink->mtext[k] = lpLinkText;
                    }
                    ///TODO:[test]
                    hr = ipFieldGroupModel->AddLink(count, lpLink);
                    CHECK_HR;
                }
                lpLinkHolder.GiveUp();
            }
            count++;
        }
    }
    else
    {
        return E_FAIL;
    }
   	return S_OK;
}

int hCheckXtabNulls(DSSDataModelXTabImpl *ipXTabModel, int inSlices, bool& obHasXTabNull)
{   
    int hr = S_OK;
    
	DSSXTabView *lpXTabView = ipXTabModel->getXTabView();
	CHECK_PTR(lpXTabView);
	
	DSSXTabHeader *lpRowHeader = lpXTabView->getHeader(0);
	CHECK_PTR(lpRowHeader);
	
	DSSXTabHeader *lpColHeader = lpXTabView->getHeader(1);
	CHECK_PTR(lpColHeader);

    int lnValidMetricCell = 0;
    for(int iSlice = 0; iSlice < inSlices; iSlice++)
    {
        const int *lpRows = NULL;
        int lnDispRows = 0;
        hr = ipXTabModel->getRowRange(iSlice, lpRows, lnDispRows);
        CHECK_HR;
        
        const int *lpCols = NULL;
        int lnDispCols = 0;
        hr = ipXTabModel->getColRange(iSlice, lpCols, lnDispCols);
        CHECK_HR;
        
        for (int i = 0; i < lnDispRows; i++)
        {
            int iRow = lpRows[i];
            int lRowMetricID = lpRowHeader->getMetricID(iRow);
            
            for (int j = 0; j < lnDispCols; j++)
            {
                int iCol = lpCols[j];
                
                //get metric ID
                int lMetricID = lpColHeader->getMetricID(iCol);
                if (lMetricID < 0)
                    lMetricID = lRowMetricID;
                                
                if (lMetricID >= 0)
                {
                    lnValidMetricCell++;
                }
            }//end of for (j = 0; j < lnDispCols; j++)
        }//end of for (i = 0; i < lnDispRows; i++)
    }
    int lnRowCount = lpRowHeader->getRowCount();
    int lnColCount = lpColHeader->getRowCount();
    if(lnValidMetricCell < lnRowCount * lnColCount)
        obHasXTabNull = true;
    else
        obHasXTabNull = false;
    return S_OK;
}

int DSSModelPopulation::LoadMetricData(DSSDataModelXTabImpl *ipXTabModel, DSSBinaryDataReader *ipReadStream,
									int iSlice, int iDataOffset, int iFlagOffset)
{
	int hr = S_OK;

	DSSXTabView *lpXTabView = ipXTabModel->getXTabView();
	CHECK_PTR(lpXTabView);

	DSSXTabHeader *lpRowHeader = lpXTabView->getHeader(0);
	CHECK_PTR(lpRowHeader);

	DSSXTabHeader *lpColHeader = lpXTabView->getHeader(1);
	CHECK_PTR(lpColHeader);

	int lnMetrics = -1;
	DSSTabularMetrics *lpMetrics = NULL;
	ipXTabModel->getMetricsUnit(&lpMetrics);

	if (lpMetrics)
		lnMetrics = lpMetrics->Count();

	if (lnMetrics <= 0)
		return S_OK;

//	int lnSlices = ipXTabModel->countSlices();
//	for (int iSlice = 0; iSlice < lnSlices; iSlice++)
	{
		//jump to the end of slices to load data flags and threshold
		hr = ipReadStream->SetPosition(iFlagOffset);
		CHECK_HR;

		vector<char> lvFlags;
		hr = ipReadStream->ReadByteArray(lvFlags);
		CHECK_HR;

		vector<int> lvThresholdID;
		hr = ipReadStream->ReadIntArray(lvThresholdID);
		CHECK_HR;

		//jump to the start of slices to load data
		hr = ipReadStream->SetPosition(iDataOffset);
		CHECK_HR;

		//get row range and column range
		const int *lpRows = NULL;
		int lnDispRows = 0;
		hr = ipXTabModel->getRowRange(iSlice, lpRows, lnDispRows);
		CHECK_HR;

		const int *lpCols = NULL;
		int lnDispCols = 0;
		hr = ipXTabModel->getColRange(iSlice, lpCols, lnDispCols);
		CHECK_HR;

		vector<unsigned char> lvBuffer(16);

		for (int i = 0; i < lnDispRows; i++)
		{
			int iRow = lpRows[i];
			int lRowLevel = lpRowHeader->getLevel(iRow);
			int lRowOffset = lpRowHeader->getOffsetOfIndex(iRow);
			int lRowMetricID = lpRowHeader->getMetricID(iRow);
			int lRowTotalID = 0;
			hr = lpRowHeader->getSubtotalID(iRow, lRowTotalID);
			CHECK_HR;

			for (int j = 0; j < lnDispCols; j++)
			{
#ifdef __OBJC__
				// 531639 The app will be killed by iOS system if it fails to handle system
				// event within 10 seconds after user sends it to background. 
				if ((i * lnDispCols + j + 1) % 10000 == 0) {
					[[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
											 beforeDate:[NSDate dateWithTimeIntervalSinceNow:0.0]];
				}
#endif
				int iCol = lpCols[j];
				int lColLevel = lpColHeader->getLevel(iCol);
				int lColOffset = lpColHeader->getOffsetOfIndex(iCol);
				int lColTotalID = 0;

				//get metric ID
				int lMetricID = lpColHeader->getMetricID(iCol);
				if (lMetricID < 0)
					lMetricID = lRowMetricID;

				//get subtotal ID
				hr = lpColHeader->getSubtotalID(iCol, lColTotalID);
				CHECK_HR;
                /*
				int lSubtotalID = lColTotalID > lRowTotalID ? lColTotalID : lRowTotalID;
				if (lRowTotalID > 0 && lColTotalID > 0 && lRowTotalID != lColTotalID)
					lSubtotalID = -1;	//invalid subtotal
				*/
				//get offset
				int lnColIndexSize = 0;
				{
					DSSTabularIndex *lpColIndex = NULL;
					lpColHeader->getIndexByLevel(lColLevel, &lpColIndex);

					if (lpColIndex)
						lnColIndexSize = lpColIndex->Count();
				}
				long long lOffset = ((long long)lRowOffset) * lnColIndexSize + lColOffset;

				if (lMetricID >= 0)
				{
					int lFlagOffset = i * lnDispCols + j;

					EnumDSSDataFlag lFlag = DssDataOk;
					int lThresholdID = 0;

					if (!lvFlags.empty() && lFlagOffset < lvFlags.size())
					{
						lFlag = static_cast<EnumDSSDataFlag>(lvFlags[lFlagOffset]);
					}

					if (!lvThresholdID.empty() && lFlagOffset < lvThresholdID.size())
					{
						lThresholdID = lvThresholdID[lFlagOffset];
					}

					hr = hSetMetricCellByOffset(ipReadStream, lpXTabView, lRowLevel, lColLevel, lMetricID, lOffset, lvBuffer, lFlag, lThresholdID);
					CHECK_HR;
				}
			}//end of for (j = 0; j < lnDispCols; j++)
		}//end of for (i = 0; i < lnDispRows; i++)

	}
	return S_OK;
}


int hLoadCells(DSSBinaryDataReader *ipReadStream, EnumDSSDataType iDataType, vector<unsigned char> &orBuffer, int &orSizeInByte, int &orCellFmtType, int &orCellFmtID)
{
	int hr = S_OK;

	orSizeInByte = hSizeOf(iDataType);

	if (DssDataTypeVarChar != iDataType && DssDataTypeBigDecimal != iDataType &&
		DssDataTypeMBChar != iDataType && DssDataTypeBinary != iDataType)
	{
		if (DssDataTypeCellFormatData == iDataType)
		{
			int lobjtype = 0;
			hr = ipReadStream->ReadInt(&lobjtype);
			CHECK_HR;

			if (lobjtype != DssDataTypeCellFormatData)
				return E_UNEXPECTED_DATATYPE;
		}

		if (orSizeInByte <= 0)
			return S_OK;

		if (orSizeInByte > (int)orBuffer.size())
			orBuffer.resize(orSizeInByte * 2);

		hr = ipReadStream->Read((unsigned char*)(&orBuffer[0]), orSizeInByte, orSizeInByte);
		CHECK_HR;

		if (iDataType == DssDataTypeLong)
		{
			//411868, long long is 8 bytes in Mac OS
			long long lVal = *((int*)(&orBuffer[0]));
			*((long long *)&orBuffer[0]) = lVal;
			orSizeInByte = sizeof(long long);
		}
		else if (DssDataTypeCellFormatData == iDataType) {
			ipReadStream->ReadInt(&orCellFmtType);
			ipReadStream->ReadInt(&orCellFmtID);
		}
	}
	else if (DssDataTypeBinary == iDataType)
	{
		//lwang, 07/16/2010, 421660, convert binary data to hex string
		unsigned int lStrLen = 0;
		hr = ipReadStream->ReadStringLen(&lStrLen);
		CHECK_HR;

		vector<char> lString (lStrLen + 1);
		hr = ipReadStream->ReadUTF8String(&lString[0], lStrLen);
		CHECK_HR;

		if (lStrLen * 2 + 1 > (int)orBuffer.size())
			orBuffer.resize(lStrLen * 4);

		char * lpHexString = (char*)&orBuffer[0];
		for (int j = 0; j < lStrLen; j++)
		{
			unsigned char lVal = lString[j];
			snprintf(lpHexString, orBuffer.size(), "%02X", lVal);
			lpHexString += 2;
		}
		orBuffer[2 * lStrLen] = '\0';

		orSizeInByte = lStrLen * 2 + 1;
	}
	else {
		//TODO[test]: string metric value
		unsigned int lStrLen = 0;
		hr = ipReadStream->ReadStringLen(&lStrLen);
		CHECK_HR;

		if (lStrLen > (int)orBuffer.size())
			orBuffer.resize(lStrLen * 2);

		orSizeInByte = lStrLen;

		hr = ipReadStream->ReadUTF8String(reinterpret_cast<char*>(&orBuffer[0]), lStrLen);
		CHECK_HR;
	}
	return 0;
}

int hLoadCalculationPlan(DSSBinaryDataReader *ipReadStream, DSSDataModelXTabImpl *ipModel)
{
	int hr = S_OK;

	DSSCalculationPlan *lpCalcPlan = ipModel->getCalculationPlan();

	int lBlockType = 0;
	hr = ipReadStream->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockXTabModelEventsV1);
	{
		DSSCalculationEvent lEvent;
		int lnEvents = 0;
		hr = ipReadStream->ReadInt(&lnEvents);
		CHECK_HR;

		MBase::CString lGUIDStr;

		for (int i = 0; i < lnEvents; i++)
		{
			int lEventType;
			hr = ipReadStream->ReadInt(&lEventType);
			CHECK_HR;
			lEvent.mEventType = (EnumEventType)lEventType;

			hr = ipReadStream->ReadString(lGUIDStr);
			CHECK_HR;

			int len = lGUIDStr.length();
			if (len > 0)
			{
#ifdef NO_WCHAR_SUPPORT
				MBase::String2Guid(lEvent.mID, lGUIDStr.c_str());
#else
				vector<wchar_t> lString(len);
				mbstowcs(&lString[0], lGUIDStr.c_str(), len);
				MBase::String2Guid(lEvent.mID, &lString[0]);
#endif
			}

			hr = ipReadStream->ReadInt(&lEvent.mEventTag);
			CHECK_HR;

			hr = lpCalcPlan->AddEvent(lEvent);
			CHECK_HR;
		}

	}
	hr = ipReadStream->EndReadBlock();
	CHECK_HR;

	hr = ipReadStream->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockXTabModelEventMetricsV1);
	{
		int lnMetrics = 0;
		hr = ipReadStream->ReadInt(&lnMetrics);
		CHECK_HR;

		MBase::CString lGUIDStr;
		vector<GUID> lvMetricID(lnMetrics);
		int iMetric = 0;
		for (; iMetric < lnMetrics; iMetric++)
		{
			hr = ipReadStream->ReadString(lGUIDStr);
			CHECK_HR;

			int len = lGUIDStr.length();
			if (len > 0)
			{
#ifdef NO_WCHAR_SUPPORT
				MBase::String2Guid(lvMetricID[iMetric], lGUIDStr.c_str());
#else
				vector<wchar_t> lString(len);
				mbstowcs(&lString[0], lGUIDStr.c_str(), len);
				MBase::String2Guid(lvMetricID[iMetric], &lString[0]);
#endif
			}
		}

		vector<int> lvMetricList1;
		hr = ipReadStream->ReadIntArray(lvMetricList1);
		CHECK_HR;

		vector<int> lvMetricList2;
		hr = ipReadStream->ReadIntArray(lvMetricList2);
		CHECK_HR;

		hr = lpCalcPlan->AddMetrics(lvMetricID, lvMetricList1, lvMetricList2);
		CHECK_HR;

		if (ipReadStream->MoreDataInBlock())
		{
			//then read the metric's null check-in option
			vector<EnumNullCheckingOption> lvNullCheckingOption(lnMetrics);
			for (iMetric = 0; iMetric < lnMetrics; iMetric++)
			{
				int lOption = 0;
				hr = ipReadStream->ReadInt(&lOption);
				lvNullCheckingOption[iMetric] = (EnumNullCheckingOption)lOption;
				CHECK_HR;
			}

			lpCalcPlan->setNullCheckingOption(lvNullCheckingOption);

			//read level metric flag
			vector<bool> lvIsLevelMetrics(lnMetrics);
			for (iMetric = 0; iMetric < lnMetrics; iMetric++)
			{
				bool lFlag = false;
				hr = ipReadStream->ReadBool(&lFlag);
				CHECK_HR;

				lvIsLevelMetrics[iMetric] = lFlag;
			}

			lpCalcPlan->setLevelMetricFlag(lvIsLevelMetrics);
		}
	}
	hr = ipReadStream->EndReadBlock();
	CHECK_HR;


	hr = ipReadStream->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockXTabModelEventLevelsV1);
	{
		//reserved, not used yet
	}
	hr = ipReadStream->EndReadBlock();
	CHECK_HR;


	hr = ipReadStream->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockXTabModelPropertiesV1);
	{
		//number of properties we saved here
		int lnProperties = 0;
		hr = ipReadStream->ReadInt(&lnProperties);
		CHECK_HR;
	
   //     if(lnProperties > 0)
        {
            //the first one is evaluation order
            int lEvaluationOrder = 0;
            hr = ipReadStream->ReadInt(&lEvaluationOrder);
            CHECK_HR;
            
            lpCalcPlan->setOrderingRule((EnumOrderingType)lEvaluationOrder);
        }
	}
	hr = ipReadStream->EndReadBlock();
	CHECK_HR;

	return S_OK;
}

int DSSModelPopulation::LoadInputControlFromXTabModel(DSSBinaryDataReader* ipBinaryReader, std::string iKey, DSSDataModelControlImpl** oppControlModel)
{
    int hr = S_OK;
	int lBlockType = 0;
        
    //read the node key
    MBase::CString lNodeKey;
    hr = ipBinaryReader->ReadString(lNodeKey);
    CHECK_HR;
		
    //skip xtab info
    hr = ipBinaryReader->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockXTabModelNullsV1);
    hr = ipBinaryReader->EndReadBlock();
    CHECK_HR;
    
    hr = ipBinaryReader->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockXTabModelLayoutV1);
    hr = ipBinaryReader->EndReadBlock();
    CHECK_HR;
	
	hr = ipBinaryReader->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockXTabModelSubtotalsV1);
    hr = ipBinaryReader->EndReadBlock();
    CHECK_HR;

    //skip unit lookup table
    hr = ipBinaryReader->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockXTabModelUnitsV1);
    hr = ipBinaryReader->EndReadBlock();
    CHECK_HR;
    
    //skip view slices
    hr = ipBinaryReader->BeginReadBlock(&lBlockType);
	if(lBlockType != EnumDataBlockXTabModelViewsV1 && lBlockType != EnumDataBlockXTabModelViewsV2)
        return E_DATABLOCK;
    hr = ipBinaryReader->EndReadBlock();
    CHECK_HR;

    //skip cell format
    hr = ipBinaryReader->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockXTabModelCellFormatsV1);
    hr = ipBinaryReader->EndReadBlock();
    CHECK_HR;
		
    if (ipBinaryReader->MoreDataInBlock())
    {
        //skip slice offset
        hr = ipBinaryReader->BeginReadBlock(&lBlockType);
        CHECK_HR_BLOCK(EnumDataBlockXTabModelSliceInfoV1);
        hr = ipBinaryReader->EndReadBlock();
        CHECK_HR;
    }
    
    if(ipBinaryReader->MoreDataInBlock())
    {
        //skip calculation plan
        hr = ipBinaryReader->BeginReadBlock(&lBlockType);
        CHECK_HR_BLOCK(EnumDataBlockXTabModelCalculationPlanV1);
        hr = ipBinaryReader->EndReadBlock();
        CHECK_HR;
        
        if (ipBinaryReader->MoreDataInBlock())
        {
            hr = ipBinaryReader->BeginReadBlock(&lBlockType);
            CHECK_HR_BLOCK(EnumDataBlockInputControlModelV1);
            {
                int lnControlModels = 0;
                hr = ipBinaryReader->ReadInt(&lnControlModels);
                CHECK_HR;
                
                for(int i = 0; i < lnControlModels; i++)
                {
                    EnumBinaryObjectType lModelType = EnumObjectReserved;
                    hr = ipBinaryReader->ReadInt((int*)(&lModelType));
                    CHECK_HR;
                    
                    AE_ASSERT(lModelType == EnumObjectControlNodeModel);
                    
                    hr = ipBinaryReader->BeginReadBlock(&lBlockType);
                    CHECK_HR_BLOCK(EnumDataBlockControlNodeModelV1);
                    
                    int lOffset = ipBinaryReader->GetPosition();
        
                    //read the node key
                    MBase::CString lNodeKey;
                    hr = ipBinaryReader->ReadString(lNodeKey);
                    CHECK_HR;

                    if(lNodeKey == iKey)
                    {
                        ipBinaryReader->SetPosition(lOffset);
                        hr =  hLoadInputControlModel(ipBinaryReader, oppControlModel);
                    }
                    hr = ipBinaryReader->EndReadBlock();
                    CHECK_HR;
                }
            }
            hr = ipBinaryReader->EndReadBlock();
            CHECK_HR;
        }
    }
    return S_OK;
}

int DSSModelPopulation::LoadInputControlFromFieldGroupModel(DSSBinaryDataReader *ipBinaryReader, std::string iKey, DSSDataModelControlImpl **oppControlModel)
{
	int hr = S_OK;
	int lBlockType = 0;
    
    //read the node key
    MBase::CString lNodeKey;
    hr = ipBinaryReader->ReadString(lNodeKey);
    CHECK_HR;
    
	//skip unit lookup table
    hr = ipBinaryReader->BeginReadBlock(&lBlockType);
	CHECK_HR_BLOCK(EnumDataBlockFieldGroupModelUnitsV1);
    hr = ipBinaryReader->EndReadBlock();
    CHECK_HR;
    
    //skip fields
    hr = ipBinaryReader->BeginReadBlock(&lBlockType);
    if(lBlockType == EnumDataBlockFieldGroupModelStaticFieldsV1)
    {
        hr = ipBinaryReader->EndReadBlock();
        CHECK_HR;
        
        hr = ipBinaryReader->BeginReadBlock(&lBlockType);
    }
    
	if(lBlockType == EnumDataBlockFieldGroupModelFieldsV2)
    {
        hr = ipBinaryReader->EndReadBlock();
        CHECK_HR;
        
        hr = ipBinaryReader->BeginReadBlock(&lBlockType);
        CHECK_HR_BLOCK(EnumDataBlockFieldGroupModelFieldsOffsetV1);
    }
    hr = ipBinaryReader->EndReadBlock();
    CHECK_HR;
    
    if(ipBinaryReader->MoreDataInBlock())
    {
        hr = ipBinaryReader->BeginReadBlock(&lBlockType);
        CHECK_HR_BLOCK(EnumDataBlockFieldGroupModelRawDataV1);
        
        hr = ipBinaryReader->EndReadBlock();
        CHECK_HR;
        
        //load input control data model
        if (ipBinaryReader->MoreDataInBlock())
        {
            hr = ipBinaryReader->BeginReadBlock(&lBlockType);
            CHECK_HR_BLOCK(EnumDataBlockInputControlModelV1);
            {
                int lnControlModels = 0;
                hr = ipBinaryReader->ReadInt(&lnControlModels);
                CHECK_HR;
                
                for(int i = 0; i < lnControlModels; i++)
                {
                    EnumBinaryObjectType lModelType = EnumObjectReserved;
                    hr = ipBinaryReader->ReadInt((int*)(&lModelType));
                    CHECK_HR;
                    
                    AE_ASSERT(lModelType == EnumObjectControlNodeModel);
                    
                    hr = ipBinaryReader->BeginReadBlock(&lBlockType);
                    CHECK_HR_BLOCK(EnumDataBlockControlNodeModelV1);
                    
                    int lOffset = ipBinaryReader->GetPosition();
                    
                    //read the node key
                    MBase::CString lNodeKey;
                    hr = ipBinaryReader->ReadString(lNodeKey);
                    CHECK_HR;
                    
                    if(lNodeKey == iKey)
                    {
                        ipBinaryReader->SetPosition(lOffset);
                        hr =  hLoadInputControlModel(ipBinaryReader, oppControlModel);
                    }
                    hr = ipBinaryReader->EndReadBlock();
                    CHECK_HR;
                }
            }
        }
    }

	return S_OK;
}


int DSSModelPopulation::LoadAttributeForDDIC(DSSBinaryDataReader *ipReadStream, int index, DSSTabularUnit **oppXTabUnit)
{
    return hLoadUnitAttribute(ipReadStream, index, oppXTabUnit);
}