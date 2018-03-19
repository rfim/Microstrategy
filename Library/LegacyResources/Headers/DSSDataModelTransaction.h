/*
 *  DSSDataModelTransaction.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 11/11/10.
 *  Copyright 2010 MicroStrategy, Inc. All rights reserved.
 *
 */

#import "DSSAEHelper.h"
//#import "DSSDataModelFieldGroupImpl.h"
#import <map>

class DSSTransactionReport;
class DSSDataModelBase;
class DSSColumnMappingInfo;
class DSSXTabView;
class DSSBinaryDataReader;
class DSSTabularUnit;
class DSSRWNodeModel;
class DSSDataModelFieldGroupImpl;
class IDSSTemplateModel;
class DSSBinaryDataWriter;
class IDSSSourceModelForTransaction;
class DFCCSITransactionBase;

typedef struct TransactionRecord {
	vector<MBase::CString*>mvpText;     //user’s changes, NULL if no change
	
	vector<AERawData> mvOriginalData;	//original value
	
	vector<int> mvDataOffset;		    //grid attribute: element offset; grid metric: data offset; field: field ID
	
	vector<MBase::CString> mvOriginalFieldText;    //original TEXT for FG case
	
	long mOrder;                        //to indicate this record is inserted earlier or later compared to other records
	
	int mRowOrdinal;                    //which row in the grid is corresponding to this record
	
	int mColOrdinal;                    //which column, for grid cases only not 0 when it has attribute unit in column axis
	
	EnumDSSTransactionDataChangeType mFlag;     //updated, submitted (committed) or discarded (original)
    
    vector<MBase::CString*> mvpDisplayText;     //text for displayed form only used when mapped to DDIC whose the write back form may not be the displayed form
    
    vector<AERawData> mvDisplayData;            //raw data for the display form
    
    int mbMarked;                       //whether this record is marked for submission
	
    int mbValidate;                     //whether this record passed required column validation; 1- passed, 0-failed
    
}TransactionRecord;

typedef struct ColumnInfo {
	DSSColumnMappingInfo *mpDefinition;    //pointer to online transaction mapping info
	 
	EnumDSSDataType mDataType;          //data type
	
	DSSTabularUnit *mpTabularUnit;      //a pointer to the mapped tabular unit for grids
	
	int mBaseForm;                      //base form id or metric index
    
	bool mbRequired;                    //for required column validation
}ColumnInfo;

typedef pair<int, int> RecordKey;
typedef map<RecordKey, TransactionRecord*> RecordMap;

class DSSDataModelTransaction
{
public:
    
	DSSDataModelTransaction();
	
	virtual ~DSSDataModelTransaction();
	
//************* transaction data model initialization ****************************
    
    /*!
	 @abstract	init with transaction report and data model of the grid/Field group where the transaction is mapped
     @param 
     Init with DSSDataModelBase for old RWD model structure before 940
     InitWithNodeModel with DSSRWNodeModel for new RWD model since 940
	 */
    int Init(DSSTransactionReport *ipTransactionReport, DSSDataModelBase *ipXTabModel);
    // tehe 12/30/2011
    virtual int InitWithNodeModel(DSSTransactionReport *ipTransactionReport, DSSRWNodeModel *ipNodeModel);
    
    
     /*!
	 @abstract	set the mapping info between the transaction columns and grid unit/Fields they are mapped to
	 @param
     irvUnitMap for mapTrxColumn2XTabUnit;		
     irvBaseFormMap for mapTrxColumn2Metric;
     irvWBFormMap for mapTrxColumn2WriteBackForm;
	 */
	void setMapTransactionColumnToMetric(vector<int> &irvUnitMap, vector<int> &irvBaseFormMap,vector<int> &irvWBFormMap);
	
    
	int LoadFromXML(MBase::CString &irXML);                 //no implementation
	
	int LoadFromBinary(DSSBinaryDataReader *ipReader);      //no implementation

	/*!
	 @abstract	populat with an existing transaction data model 
	 @discussion 1, this API is designed to support incremental data loading freature.
				    Users make changes on the current slice data, later on when full binary data is instanciate
					we need transfer transaction record to match the new grid/FG data model
				 2, this API doesn't work with UC and other partial binary data, becuase we are not able to match the changes
					due to the element keys change in the grid/FG data model.
     @param
        DSSDataModelBase for old RWD structure before 940
        DSSRWNodeModel for new RWD structure since 940
	 */
	int Init(DSSDataModelTransaction *ipTransactionModel, DSSDataModelBase *ipBaseDataModel);
	virtual int Init(DSSDataModelTransaction *ipSourceTransactionModel, DSSRWNodeModel *ipNodeModel);

    
    
//************* transaction data record manipulations ****************************	
	
	/*!
	 @abstract	insert a transaction record
	 @param iOriginalRow	row ordinal in XTabView for grid, or row ordinal for field group
	 param iOriginalColumn	column ordinal in XTabView for grid, or field index for field group
	 param iColumnIndex		the index of column in transaction report that to be updated
	 param irText	the value to be updated for that column
	 param ipvOriginalValue	original value to be used to fill the record
     param ipvDisplayValue display form raw data, only used when there is DDIC defined in the grid/field group
	 @discussion 1, row/column ordinal is the order before apply sort
				 2, (iOriginalRow, iOriginalColumn) will be used as the key of transaction record
	 */
	virtual int AddRecord(int iOriginalRow, int iOriginalColumn, int iColumnIndex, MBase::CString &irText, vector<AERawData> *ipvOriginalValue, vector<AERawData> *ipvDisplayValue);
    
    virtual int GetOriginalValue(int iOriginalRow, int iOriginalColumn, int iColumnIndex, AERawData& opOriginalValue);
	
	/*!
	 @abstract	select or un-select a row. If it's a new selected row, insert a transaction record with data on that row
	 @discussion change a status of the record identifed by iOriginal Row by iType
	 */
	virtual int MarkRecord(int iOriginalRow, vector<AERawData> *ipvOriginalValue, vector<AERawData> *ipvDisplayData, EnumDSSTransactionDataChangeType iType);
	
	/*!
	 @abstract	update metric data in grid
	 @param iOriginalRow row ordinal in XTabView for grid, or row ordinal for field group
     param iOriginalColumn column ordinal in XTabView for grid, or field index for field group
     param iColumnIndex		the index of column in transaction report that to be updated
     param ipNewValue   user's transaction change for grid column/field mapped to iColumnIndex-th transaction column
     param ibDisplay  if this column has DDIC defined the flag would be true
	 */
	virtual int UpdateDataCell(int iOriginalRow, int iOriginalColumn, int iColumnIndex, MBase::CString *ipNewValue, bool ibDisplay = false);
	
	/*!
	 @abstract update attribute data column in grid
     @param iOriginalRow row ordinal in XTabView for grid, or row ordinal for field group
     param iOriginalColumn column ordinal in XTabView for grid, or field index for field group
     param iColumnIndex		the index of column in transaction report that to be updated
     param ipXTabunit   the xtab unit mapped by the transaction column, used for future discard operations
     param iBaseForm    the base form ID or metric Index of the unit mapped by transaction column, used for future discard operations
     param ipNewValue   user's transaction change for grid column/field mapped to iColumnIndex-th transaction column
     param ibDisplay  if this column has DDIC defined the flag would be true
	 */
	virtual int UpdateHeaderCell(int iOriginalRow, int iOriginalColumn, int iColumnIndex, DSSTabularUnit *ipXTabunit, int iBaseForm, int iOffset, MBase::CString *ipNewValue, bool ibDisplay = false);
	
	/*!
	 @abstract	update field data in field group model
     @param iOriginalRow row ordinal in XTabView for grid, or row ordinal for field group
     param iOriginalColumn column ordinal in XTabView for grid, or field index for field group
     param iColumnIndex		the index of column in transaction report that to be updated
     param ipNewValue   user's transaction change for grid column/field mapped to iColumnIndex-th transaction column
     param ibDisplay  if this column has DDIC defined the flag would be true
	 */
	virtual int UpdateField(int iOriginalRow, int iOriginalColumn, int iColumnIndex, MBase::CString *ipNewValue, bool ibDisplay = false);

	/*!
	 @abstract	copy the content to target transaction data model
	 @discussion it returns E_FAIL if two transaction data models don't belong to the same transaction report
     called by controller
	 */
	virtual int MergeTo(DSSDataModelTransaction *pTargetTransactionModel);
	
//************* transaction data record manipulations ****************************	
	/*!
	 @abstract	empty transaction data model and restore XTab data model and field group data model
	 @param ibRestore is used to roll back calculation results.
	 */
	virtual  int Discard(bool ibRestore = true);
	
	/*!
	 @abstract	generate the xml format representation of transaction data model
	 @discussion
	 */
	int getOutputXML(MBase::CString &orXMLContent);
	
	/*!
	 @abstract	empty transaction data model, set transaction records as submitted
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
	
    /*!
     @abstract check if there is any unsubmitted transaction records
     */
	bool HasUpdateRecord();
	
    /*!
     @abstract check the transaction record type for a input row/column combination in a grid/field group
     @discussion the type can be update/original(discarded)/committed(submitted)
     */
	EnumDSSTransactionDataChangeType getRecordeType(int iOriginalRow, int iOriginalCol);
	
    /*!
     @abstract check if the value for a cell or a field is updated or not
     @discussion opbSubmit is true when the updated value is the same with original value
     IsDataUpdatedOrSubmitted will check both updated records and submitted records
     */
    bool IsDataUpdated(int iOriginalRow, int iOriginalCol, int iColumnIndex, bool* opbSubmit = NULL, bool ibValidateEmpty = false);//add opbSubmit because of 670825 and 671305//add ibAutoSubmission because of 906371
    
	bool IsDataUpdatedOrSubmitted(int iOriginalRow, int iOriginalCol, int iColumnIndex, bool* opbSubmit = NULL);
	bool IsDataUpdated(int iOriginalRow, int iOriginalCol, DSSTabularUnit* ipTabularUnit, int iBaseForm);
	
    /*!
     @abstract get the updated value for a cell or a field is updated or not
     @discussion
     getUpdatedValue will only return a value if the cell is changed and not submitted
     getUpdatedAndSubmittedData will return value even if the cell's change is submitted
     if ibDisplay is true, the returned value will be the display form value for DDIC
     */
    // jimzhang, Nov/7/2012, port from RWD_MCE_iOS branch to support recalculate of threshold in transaction
    int getUpdatedValue(int iOriginalRow, int iOriginalColumn, int iColumnIndex, MBase::CString &orText);
	MBase::CString* getUpdatedAndSubmittedData(int iOriginalRow, int iOriginalCol, int iColumnIndex,bool ibDisplay = true, bool ibSkipUpdatedValue = false, bool* opDisplay = NULL);
    
    /*!
     @abstract manually set a status of a transaction record
     */
	int setRecordFlag(int lOriginalRow, int lOriginalCol, EnumDSSTransactionDataChangeType lFlag);
    
    /*!
     @abstract manually set the original text for a particular field
     */
	virtual int setOriginalFieldText(int lOriginalRow, int lOriginalCol, int iColumnIndex, std::string iFieldText);
    
//************* utility functions ****************************
    /*!
     @abstract compare the mOrder of two transaction records to check which one is generated earlier
     */
	bool operator() (int a, int b);
	
    //no implementation
	int size();
    
	EnumXTabStatus getStatus() {return mStatus;};
    
    DSSTransactionReport* getTransactionReport(){return mpTransactionReport;}
    string getTransactionReportID();
    
    /*!
     @abstract save the transaction header infos and transaction records into a binary oppBinaryModel with size opSize
     @param includeCommitted if this value is true the binary will also contain the submitted records.
     */
	virtual int SaveToBinary(bool includeCommitted, unsigned char** oppBinaryModel, int* opSize);
  
    /*!
     @abstract get/set the source data model
     */
    virtual IDSSSourceModelForTransaction* getSourceDataModel();
    void setSourceDataModel(DSSDataModelBase* ipModel){mpSourceDataModel = ipModel;}
    
    bool isNewRWD(){return mbNewRWD;}
    
//************* transaction column info related****************************
    
    /*!
     @abstract set/get the tabular unit and form id mapped to iColumnIndex
     */
	int setColumnInfo(int iColumnIndex, DSSTabularUnit* ipUnit, int iForm);
	int getColumnInfo(int iColumnIndex, DSSTabularUnit** oppUnit, int* opForm);
	
    /*!
     @abstract get the transaction mapping between tabular unit/fields and the transaction column indices
     @discussion
     getColumnMappingInfo will get the mapping for online transaction
     getOfflineColumnMappingInfo will get the mapping for dataset(offline) transaction
     */
	int getColumnMappingInfo(vector<int>** oppMapTrxColumn2XTabUnit, vector<int>** oppMapTrxColumn2BaseForm, vector<int>** oppMapTrxColumn2WBForm);
    int getOfflineColumnMappingInfo(vector<int>** oppMapTrxColumn2XTabUnit, vector<int>** oppMapTrxColumn2BaseForm, vector<int>** oppMapTrxColumn2WBForm);

    /*!
     @abstract get/set the data type of a transaction column
     */
    EnumDSSDataType getColumnDataType(int iColumnIndex);
    int setColumnDataType(int iColumnIndex, EnumDSSDataType iDataType);

	
//************* Android "Mark row" support ********************
    /*!
     @abstract find all rows "marked" to submit
     */
	int getMarkedRows(vector<int>& ivRows);
    
    /*!
     @abstract generate transaction record by a row/col ordinal in the xtabview
     @param orvDisplayData is display form values for DDIC
     @discussion usually called to generate a transaction record for a grid row that is "marked" but has no user's changes
     */
    virtual int GenerateRecordFromXTabView(DSSXTabView *ipXTabView, int iRow, int iCol, vector<AERawData> &orvRawData, vector<AERawData> &orvDisplayData);

    /*!
     @abstract manually mark the transaction record (if there is) for a particular row/column combination
     */
    int markRow(int iOriginalRow, int lOriginalCol);
    
//********* support "required" field validation ********************

    /*!
     @abstract validate all transaction records to see if all required columns are updated
     */
    bool validateRequiredColumns(bool ibOnlyValidateEmpty = false);

    /*!
     @abstract get those rows who failed the validation
     */
    int getValidationFailureRows(vector<int>& vRows);
    
    /*!
     @abstract manually set the validation failed flag
     @discussion used by offline transaction when user choose to edit a offline submission
     */
    int setValidationFailedFlag(int iOriginalRow, int lOriginalCol);
    
    /*!
     @abstract check if a transaction column is defined as "required"
     */
    bool isColumnRequired(int iColumnIndex);//0-based
    
    /*!
     @abstract check if a particular row is already validated or not
     */
    bool isModelValidated(int iRow);
    
    /*!
     @abstract get all the validated rows
     */
    vector<int>* getValidatedRows(){return &mvValidatedRows;}
    
    /*!
     @abstract update the required info from input control definition
     @discussion added to support conditional actions, the required setting can be changed on runtime
     */
    virtual int UpdateRequiredColumnStatus();
    
//********* support dataset(offline) transaction ********************

    /*!
     @abstract new APIs for quick access into the transaction record data
     */
    int GetTransactionCount();
    //number of rows
    int GetColumnCount();
    //number of columns
    int GetValue(int iRow, int iColumn, string* oText, EnumDSSDataType* oDataType);
    //get the DFCSimpleExprConst of (iRow, iColumn)
    
    /*!
     @abstract generate the dataset(offline) transaction delta
     @param ovDataSetID the GUID of the target datasets for each transaction record
     @discussion
     */
    int generateTransactionDelta(vector<DSS_ID>& ovDataSetID, vector<vector<string> >& ovvDelta); //not in use, no implementation
    int generateTransactionDelta(vector<DSS_ID>& ovDataSetID, vector<unsigned char*>& ovDelta, vector<int>& ovDeltaSize);
    

    /*!
     @abstract set the mapping between transaction columns and grid/field group for dataset(offline) transaction
     @param irvUnitMap for mapOfflineTrxColumn2XTabUnit
     param irvBaseFormMap for mapOfflineTrxColumn2Metric
     param irvWBForm for mapOfflineTrxColumn2WriteBackForm
     param irvDataType  to set the data types of mvOfflineColumns
     param irvMapOfflineColumns for mapOfflineTrxColumn2TrxColumn
     */
    int setOfflineTransaction(vector<int> &irvUnitMap, vector<int> &irvBaseFormMap, vector<int> &irvWBForm, vector<EnumDSSDataType>& irvDataType, vector<int> &irvMapOfflineColumns);
    
    /*!
     @abstract check if there is dataset(offline) transaction defined
     */
    int hasOfflineTransaction(){return mbHasOfflineTrx;}
    
    /*!
     @abstract get the column count of dataset(offline) transaction
     */
    int GetOfflineColumnCount();
    
    /*!
     @abstract get/set CSI for dataset(offline) transaction
     */
    DFCCSITransactionBase* getCSI(){return mpCSI;}
    void setCSI(DFCCSITransactionBase* ipCSI){mpCSI = ipCSI;}
    
    int SetGlobalLookupDataVer(int iVer);
    
    int GetGlobalLookupDataVer();
    
    vector<string> associatedNodes;
    
private:
	//deprecated method
	int UndoChange(int iOriginalRow, int iOriginalColumn, int iColumnIndex);

	int hAddRecordMarked(int iAdjustedRow, vector<AERawData> *ipvOriginalValue, vector<AERawData> *ipvDisplayValue,EnumDSSTransactionDataChangeType iType, RecordMap &irRecordMap);
	
	int hUndoMarkedRows(int iAdjustedRow);
	
	inline bool hIsEmpty(){return mvpRecord.empty();};
	
	int hUpdateFieldRawData();
	
//********* methods used when initializing from another transaction data model  ********************
    
	int hIsDataModelMatch(DSSDataModelBase *ipDataModel, DSSDataModelBase *ipBaseDataModel);
	
	int hPopulateHeaderInfo(DSSDataModelTransaction *ipTransactionModel);
	
	int hPopulateOneRecord(DSSXTabView *ipXTabViewSrc, DSSXTabView *ipXTabView, TransactionRecord * ipRecordSrc, vector<int> &rvKeys,bool ibSubmitted = false);
	
	int hPopulateOneRecordForFieldGroup(DSSDataModelFieldGroupImpl* ipSourceFGModel, DSSDataModelFieldGroupImpl* ipThisFGModel, TransactionRecord * ipRecordSrc, bool bUpdateRawData, bool ibSubmitted = false);
		
	int hMapRowForFieldGroup(DSSDataModelFieldGroupImpl* ipFGModelSrc, DSSDataModelFieldGroupImpl* ipFGModelThis, int& iRow);
	
	void hCleanUpRecords(vector<TransactionRecord*> &irvRecords, bool ibKeepSubmitedRecord, bool ibKeepUnMarkedRecord = false);
    
    int hSaveTransactionRecordToBinary(DSSBinaryDataWriter* lpBinaryWriter, int lSourceModelType, int lOriginalRow,int lOriginalColumn, TransactionRecord* lpRecord);
    
    
//******** copy the mUpdatedRecords, used inside mergeTo method *****
    void hAddUpdatedRecord(RecordKey iKey, TransactionRecord* ipRecord);
                            
	DSSDataModelBase *mpSourceDataModel;

protected:
    
	inline void hAdjustOrdinal(int &row, int &col){row = row / mRowOrdinalAdjustor; col = col / mColOrdinalAdjustor;};  
    
    TransactionRecord *hCreateNewRecord(EnumDSSTransactionDataChangeType iRecordType, vector<AERawData> *ipvOriginalValue, vector<AERawData> *ipvDisplayData);
    
	int hUpdateCells(TransactionRecord *lpRecord, int iRow, int iCol, bool IsUndo, int iColumnIndex = -1);
    
    void hCleanUp(bool ibKeepSubmitedRecord = false, bool ibKeepUnMarkedRecord = false);
    
    void ConvertDateTime(MBase::CString &orText);
    
	int hPopulateTransactionRecordsForGrid(DSSDataModelTransaction *ipTransactionModel, IDSSTemplateModel *ipSourceModel, IDSSTemplateModel *ipThisModel);

	int hPopulateTransactionRecordsForFG(DSSDataModelTransaction *ipTransactionModel, DSSDataModelFieldGroupImpl* ipSourceFGModel = NULL, DSSDataModelFieldGroupImpl* ipThisFGModel = NULL);
    
	virtual int hCheckMetricColumnData();
	
    int hCheckMetricColumnData(DSSXTabView *lpXTabView);
    
	virtual int hMapRowAndColumns(int iRow, int iCol, vector<int> &irvKeys, DSSXTabView *ipXTabViewSrc, int &orRow, int &orCol);
	
    int hMapRowAndColumns(int iRowOrdinalSrc, int iColOrdinalSrc,
                          vector<int> &irvKeys, DSSXTabView *ipXTabViewSrc, DSSXTabView *ipThisXTabView,
                          int &orNewRow, int &orNewCol, int lnUnits, int lMetricUnitID);
    
    virtual int hSaveModelInfoToBinary(DSSBinaryDataWriter &irWriter, int &orSourceModelType);

protected:
    
	DSSTransactionReport *mpTransactionReport; //A pointer to the transaction report, only not null if online transaction is defined.
	vector<ColumnInfo> mHeaderInfo;   //header info for the online transaction columns.
	int mnColumns;                  //number of online transaction columns, mnColumns == mHeaderInfo.size()
	
	vector<TransactionRecord*> mvpRecord;	//save all records used by the model
	
	vector<TransactionRecord*> mvpRecordSubmitted; //submitted records
	
	RecordMap mUpdatedRecords;			//a lookup table for updated recored

	vector< std::pair< RecordKey, TransactionRecord*> >  mSubmittedRecords;        // a lookup table for submitted records
    
	EnumXTabStatus mStatus;
	
	vector<int> mapTrxColumn2XTabUnit;		//map transaction column to map tabular unit ID
	vector<int> mapTrxColumn2Metric;		//map transaction column to tabular unit baseform/metric ID
	vector<int> mapTrxColumn2WriteBackForm; //map transaction column to write back form ID of DDICs
	int mRowOrdinalAdjustor;		//adjust ordinal to remove metrics
	int mColOrdinalAdjustor;		//for field group, mColOrdinalAdjustor will be # of fields
	long mSequenceNumber;           //used to set the mOrder of a transaction record
	
	bool mbIsMetricOnColumn;
    
    MBase::CString mNodeKey;        //node key of the corresponding grid/FG node
    
    bool mbMarkRowEnabled;          //introduced by 592678, in Android GUI, modifying one row doesn't always mean mark it
    
    bool mbIsValidated;             //for 671321, used to indicate whether the model is already validated.
    vector<int> mvValidatedRows;    //if already validated, it would contain the validated transaction records
    
    bool mbNewRWD;
    
    //introduced to support dataset(offline) transaction
    vector<int> mvUpdateRecords;                //only used by offline transaction, provide transaction info to CSI replacement logic
    DFCCSITransactionBase* mpCSI;               //dataset(offline) transaction CSI
    vector<ColumnInfo> mvOfflineColumns;        //column infos of the dataset(offline) transactions
    bool mbHasOfflineTrx;                       //true if mvOfflineColumns.size() > 0
    int mnOfflineColumns;                       //number of dataset(offline) transaction columns, mnOfflineColumns == mvOfflineColumns.size()
    vector<int> mapOfflineTrxColumn2XTabUnit;		//map offline transaction column to map tabular unit Id
    vector<int> mapOfflineTrxColumn2Metric;         //map offline transaction column to tabular unit baseform/metric ID
    vector<int> mapOfflineTrxColumn2WriteBackForm;	//map offline transaction column to write back form ID of DDICs
    vector<int> mapOfflineTrxColumn2TrxColumn; 		//map offline transaction column to online transaction columns

    int mGlobalLookupDataVer;   //932778, track the transaction changes made on global lookup table
};


