/*
 *  DSSDataModelTransactionRWGrid.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 11/11/10.
 *  Copyright 2010 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef ReportServiceCore_DSSDataModelTransactionRWGrid_h
#define ReportServiceCore_DSSDataModelTransactionRWGrid_h


#import "DSSRWNodeModel.h"
#import "DSSDataModelTransaction.h"

class DSSRWNodeModel;
class DSSRWGridModel;
class DSSCube;

class DSSDataModelTransactionRWGrid : public DSSDataModelTransaction
{
public:
    
	DSSDataModelTransactionRWGrid();
	
	virtual ~DSSDataModelTransactionRWGrid();
	
    //************* transaction data model initialization ****************************	
//	int Init(DSSTransactionReport *ipTransactionReport, DSSDataModelBase *ipXTabModel);
	
//	void setMapTransactionColumnToMetric(vector<int> &irvUnitMap, vector<int> &irvBaseFormMap);
	
//	int LoadFromXML(MBase::CString &irXML);
	
//	int LoadFromBinary(DSSBinaryDataReader *ipReader);
    
	/*!
	 @abstract	populat with an existing transaction data model 
	 @discussion 1, this API is designed to support incremental data loading freature.
     Users make changes on the current slice data, later on when full binary data is instanciate
     we need transfer transaction record to match the new grid/FG data model
     2, this API doesn't work with UC and other partial binary data, becuase we are not able to match the changes
     due to the element keys change in the grid/FG data model.
	 */
	virtual int Init(DSSDataModelTransaction *ipSourceTransactionModel, DSSRWNodeModel *ipNodeModel);
	
    virtual int InitWithNodeModel(DSSTransactionReport *ipTransactionReport, DSSRWNodeModel *ipNodeModel);
    
    //************* transaction data record manipulations ****************************	
	
	/*!
	 @abstract	insert a transaction record
	 @param iOriginalRow	row ordinal in XTabView for grid, or row ordinal for field group
	 param iOriginalColumn	column ordinal in XTabView for grid, or field index for field group
	 param iColumnIndex		the index of column in transaction report that to be updated
	 param irText	the value to be updated for that column
	 param ipvOriginalValue	original value to be used to fill the record
	 @discussion 1, row/column ordinal is the order before apply sort
     2, (iOriginalRow, iOriginalColumn) will be used as the key of transaction record
	 */
	virtual int AddRecord(int iOriginalRow, int iOriginalColumn, int iColumnIndex, MBase::CString &irText, vector<AERawData> *ipvOriginalValue, vector<AERawData> *ipvDisplayValue);
	
	/*!
	 @abstract	select or un-select a row. If it's a new selected row, insert a transaction record with data on that row
	 @discussion
	 */
//	int MarkRecord(int iOriginalRow, vector<AERawData> *ipvOriginalValue, EnumDSSTransactionDataChangeType iType);
	
	/*!
	 @abstract	update metric data in grid
	 @discussion
	 */
	virtual int UpdateDataCell(int iOriginalRow, int iOriginalColumn, int iColumnIndex, MBase::CString *ipNewValue, bool ibDisplay = false);
	
	/*!
	 @abstract update attribute data column in grid
	 @discussion
	 */
	virtual int UpdateHeaderCell(int iOriginalRow, int iOriginalColumn, int iColumnIndex, DSSTabularUnit *ipXTabunit, int iBaseForm, int iOffset, MBase::CString *ipNewValue, bool ibDisplay = false);
	
	/*!
	 @abstract	update field data in field group model
	 @discussion
	 */
//	int UpdateField(int iOriginalRow, int iOriginalColumn, int iColumnIndex, MBase::CString *ipNewValue);
    
	/*!
	 @abstract	copy the content to target transaction data model
	 @discussion it returns E_FAIL if two transaction data models don't belong to the same transaction report
	 */
//	int MergeTo(DSSDataModelTransactionRWGrid *pTargetTransactionModel);
	
    //************* transaction data record manipulations ****************************	
	/*!
	 @abstract	empty transaction data model and restore XTab data model and field group data model
	 @discussion
	 */
	virtual  int Discard(bool ibRestore = true);
	
	/*!
	 @abstract	generate the xml format representation of transaction data model
	 @discussion
	 */
//	int getOutputXML(MBase::CString &orXMLContent);
	
	/*!
	 @abstract	empty transaction data model
	 @discussion
	 */
	virtual int setAllCommit();
	
    /*
     @abstract	discard a transaction operation
	 @param iOriginalRow	row ordinal in XTabView for grid, or row ordinal for field group
	 param iOriginalColumn	column ordinal in XTabView for grid, or field index for field group
	 param iColumnIndex		the index of column in transaction report that to be discarded
     
     */
    virtual int DiscardTransactionOperation(int iOriginalRow, int iOriginalColumn, int iColumnIndex);
    
    //************* query transaction record ****************************
	
//	inline bool HasUpdateRecord(){return !mvpRecord.empty();};
	
//	EnumDSSTransactionDataChangeType getRecordeType(int iOriginalRow, int iOriginalCol);
	
//	bool IsDataUpdated(int iOriginalRow, int iOriginalCol, int iColumnIndex); 
	
//	EnumXTabStatus getStatus() {return mStatus;};
	
    //************* others ****************************	
//	bool operator() (int a, int b);
	
//	int size();

    virtual int GenerateRecordFromXTabView(DSSXTabView *ipXTabView, int iRow, int iCol, vector<AERawData> &orvRawData, vector<AERawData> &orvDisplayData);
    
    void ResetXTabView(DSSXTabView* ipView){mpXTabView = ipView;}
    
    void ResetTransactionReport(DSSTransactionReport* ipReport){mpTransactionReport = ipReport;}
    
    virtual IDSSSourceModelForTransaction* getSourceDataModel(){return (DSSRWNodeModel*)mpSourceGridModel;};
    
    virtual int UpdateRequiredColumnStatus();
    
protected:
    
    virtual int hCheckMetricColumnData();
    
	virtual int hMapRowAndColumns(int iRow, int iCol, vector<int> &irvKeys, DSSXTabView *ipXTabViewSrc, int &orRow, int &orCol);
	
    virtual int hSaveModelInfoToBinary(DSSBinaryDataWriter &irWriter, int &orSourceModelType);
    
private:
	//deprecated method
//	int UndoChange(int iOriginalRow, int iOriginalColumn, int iColumnIndex);
    
private:
    
	int hPopulateHeaderInfo2(DSSDataModelTransactionRWGrid *ipTransactionModel);
    
/*	int hAddRecordMarked(int iAdjustedRow, vector<AERawData> *ipvOriginalValue, EnumDSSTransactionDataChangeType iType, RecordMap &irRecordMap);
	
	int hUndoMarkedRows(int iAdjustedRow);
	
	TransactionRecord *hCreateNewRecord(EnumDSSTransactionDataChangeType iRecordType, vector<AERawData> *ipvOriginalValue);

	
	void hCleanUp(bool ibKeepSubmitedRecord = false);
	
	int hCheckMetricColumnData();
	
	inline bool hIsEmpty(){return mvpRecord.empty();};
	
	inline void hAdjustOrdinal(int &row, int &col){row = row / mRowOrdinalAdjustor; col = col / mColOrdinalAdjustor;};
	
	int hUpdateFieldRawData();
	
	int hUpdateCells(TransactionRecord *lpRecord, int iRow, int iCol, bool IsUndo);
	
	int hIsDataModelMatch(DSSDataModelBase *ipDataModel, DSSDataModelBase *ipBaseDataModel);
	
	int hPopulateHeaderInfo(DSSDataModelTransactionRWGrid *ipTransactionModel);
	
	int hPopulateTransactionRecords(DSSDataModelTransactionRWGrid *ipTransactionModel);
    
	int hPopulateOneRecord(DSSXTabView *ipXTabViewSrc, DSSXTabView *ipXTabView, TransactionRecord * ipRecordSrc, vector<int> &rvKeys);
	
	int hPopulateOneRecordForFieldGroup(DSSDataModelFieldGroupImpl* ipFGModel, TransactionRecord * ipRecordSrc);
    
	int hMapRowAndColumns(int iRow, int iCol, vector<int> &irvKeys, DSSXTabView *ipXTabViewSrc, int &orRow, int &orCol);
	
	int hMapRowForFieldGroup(DSSDataModelFieldGroupImpl* ipFGModelSrc, int& iRow);
	
	void hCleanUpRecords(vector<TransactionRecord*> &irvRecords, bool ibKeepSubmitedRecord);
    
	DSSTransactionReport *mpTransactionReport;
	DSSDataModelBase *mpSourceDataModel;
	
	vector<ColumnInfo> mHeaderInfo;
	int mnColumns;
	
	vector<TransactionRecord*> mvpRecord;	//save all records used by the model
	
	vector<TransactionRecord*> mvpRecordSubmitted;
	
	RecordMap mUpdatedRecords;			//a lookup table for updated recored
    //	RecordMap mInsertedRecords;			//a lookup table for new recored
    //	RecordMap mDeletedRecords;			//a lookup table for removed recored
	
	EnumXTabStatus mStatus;
	
	vector<int> mapTrxColumn2XTabUnit;		//map transaction column to map tabular unit ID
	vector<int> mapTrxColumn2Metric;		//map transaction column to tabular unit baseform/metric ID
	
	int mRowOrdinalAdjustor;		//adjust ordinal to remove metrics
	int mColOrdinalAdjustor;		//for field group, mColOrdinalAdjustor will be # of fields
	long mSequenceNumber;
	
	bool mbIsMetricOnColumn;
*/    

    DSSRWGridModel *mpSourceGridModel;      //data source of transaction model in new RWD engine
    DSSXTabView *mpXTabView;
    DSSCube *mpCube;
    
};

#endif
