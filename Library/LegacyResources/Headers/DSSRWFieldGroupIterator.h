/*
 *  DSSRWFieldGroupIterator.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 3/20/10.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import "DSSRWIterator.h"
#import "DSSGlobalLookUpTable.h"

class DSSDataModelFieldGroupImpl;
class FieldGroupCell;
class CComVariant;
class DSSDataModelTransaction;
class AERawData;
class DSSRWInputControlIterator;
class DSSField;
class DSSAttribute;

class DSSRWFieldGroupIterator : public DSSRWIterator
{
public:
	DSSRWFieldGroupIterator();
	virtual ~DSSRWFieldGroupIterator();
	
	int Init(DSSRWNode *ipNode, DSSRWSectionIterator * ipParent, vector<DSSBaseElementProxy*>& ivNGBElements);
	
	//int ReInit();         // jimzhang nov/24/2011 Comment out old code temporarily
    virtual int ReInit();   // jimzhang nov/24/2011
	
	/*!
	 @method     
	 @abstract   return the total number of rows
	 @discussion 
	 */
	//int CountRows();          // jimzhang nov/24/2011 Comment out old code temporarily
    virtual int CountRows();    // jimzhang nov/24/2011
	
	/*!
	 @method     
	 @abstract   return the number of fields on each row
	 @discussion 
	 */
	//int CountFields();        // jimzhang nov/24/2011 Comment out old code temporarily
    virtual int CountFields();  // jimzhang nov/24/2011 
    
	
	/*!
	 @method     
	 @abstract   move cursor to the first row
	 @discussion if field group is empty, return false
	 */
	//bool FirstRow();          // jimzhang nov/24/2011 Comment out old code temporarily
    virtual bool FirstRow();    // jimzhang nov/24/2011
	
	/*!
	 @method     
	 @abstract   move cursor to the next row
	 @discussion if cursor is at the last row, return false
	 */
	//bool NextRow();           // jimzhang nov/24/2011 Comment out old code temporarily
    virtual bool NextRow();     // jimzhang nov/24/2011
	
	/*!
	 @method     
	 @abstract   get field group cell
	 @discussion
	 */
	//FieldGroupCell getFieldGroupCell(int iField);         // jimzhang nov/24/2011 Comment out old code temporarily
    virtual FieldGroupCell getFieldGroupCell(int iField);   // jimzhang nov/24/2011
	
    virtual int CountJoinedIndexRows();   // add for special field group threshold
	/*!
	 @method     
	 @abstract  return field's display text
	 @discussion
	 */
	//int getFieldText(int field, std::string &orText);
	
	/*!
	 @method     
	 @abstract  return threshold ID, return -1 when threshold is not applied
	 @discussion 0-based ID
	 */
	//int getThresholdID(int field);

	/*!
	 @method     
	 @abstract  return true if the field has any link text at current row
	 @discussion
	 */
	//bool HasLinks(int field);

	/*!
	 @method     
	 @abstract  get link text for the given type; return S_FALSE if that type is not defined
	 @discussion
	 */
	//int getLinkText(int field, EnumDSSRWLinkType iType, std::string &orLinkText);
	
	/*!
	 @method     
	 @abstract  return true if the text is from right left (Hebrew)
	 @discussion
	 */
	bool IsRightToLef(int field);
	
	/*!
	 @method     
	 @abstract  get available commands from handle
	 @discussion
	 */
	int getCommandList(DSSCellHandle &irHandle, vector<EnumManipulationCommandTypes> &orvCommand);
	
	inline DSSDataModelFieldGroupImpl *getModel(){return mpFieldGroupModel;}

	virtual int UpdateData(DSSCellHandle &irHandle, CComVariant &irNewValue);
	
	virtual DSSDataModelTransaction* getTransactionDataModel();
	
	virtual int UndoDataChange(DSSCellHandle &irHandle);

	virtual FieldGroupCell getFieldGroupCell(int iRow, int iField);
	
	virtual int MarkRows(int iRowOrdinal, EnumDSSRWMarkRowAction iActionType);
	
    virtual int getDataChangeType(DSSCellHandle &irHandle, bool ibTrxSubPreset = false);
    
	DSSCellHandle getCellHandleAt(int iField, int iRow);
	
	virtual int getFieldIndexFromHandle(DSSCellHandle &irHandle);
	
	virtual int getRowIndexFromHandle(DSSCellHandle &irHandle);
	
    virtual int CreateInputControlIterator(std::string iKey, DSSRWInputControlIterator** oppControlIterator);
    
	int getTrxColumnIndexByFieldID(int iField);
    
    virtual int GenerateTransactionRecord(DSSCellHandle &irHandle, CComVariant &irNewValue);

    virtual int UpdateFieldCell(DSSCellHandle &irHandle, CComVariant &irNewValue);
    
    int getRowOrdinal(int iRow);
    // Sep/19/2012, todo, implement this function in old RWD, currently this function is hard code.
    virtual bool getNullChecking() { return false; }
    
    int getCurrentMetricValueByGUID (GUID *ipReportGUID, GUID *ipMetricGUID, const std::string *ipFieldKey, AERawData *opRawData);
    
    int getCurrentFormValueByID(DSSAttribute *ipAttr, GUID *ipFormGUID, AERawData *opRawData);
    
    // weilin DE51631 2016/12/22 Get Attribute Field Form GUID
    GUID getAttrFieldFormID(DSSField *ipField, DSSAttribute *ipAttr, GUID iAttrID);

    virtual bool isTransactionModelValidated();
    
    virtual EnumDSSDataType getTransactionInputDataType(DSSCellHandle& irHandle);
    
protected:
    
	int hBuildTransactionColumnMap(DSSRWNode * ipNode);
        
    int hReInitFieldData();
    
    int hReInitCalcField(int iFieldIndex, DSSField *ipField);
    
    int hReInitThreshold(int iFieldIndex, DSSField *ipField);

	virtual int hGetFieldData(int iRow, vector<AERawData> &irvOriginalData);
    
	vector<int> mvMapToFields;	//map from transaction report column to a field
	vector<int> mvMapToWBColumn;	//map from field to transaction report column

private:
	
	DSSDataModelFieldGroupImpl *mpFieldGroupModel;

	int mCursor;
	vector<int> mvRows;
	int mnRows;
	
	vector<bool> mvbIsHebrew;	//from right to left
	
};

class FieldGroupCell
{
public:
	FieldGroupCell(int iRow, int iField, DSSRWFieldGroupIterator *ipIterator);
	
	//copy constructor
	FieldGroupCell(const FieldGroupCell &irObj):mRow(irObj.mRow),mField(irObj.mField),mpIterator(irObj.mpIterator){};
	
	/*!
	 @method     
	 @abstract  return field index, used to get field definition from FieldGroupNode
	 @discussion
	 */
	inline int getFieldID(){return mField;};
	
	/*!
	 @method     
	 @abstract  return field's display text
	 @discussion
	 */
	int getFieldText(std::string &orText);
	
	/*!
	 @method     
	 @abstract  return true if there's threshold applied
	 @discussion
	 */
	bool HasThreshold();
	
	/*!
	 @method     
	 @abstract  return threshold ID, return -1 when threshold is not applied
	 @discussion ID is 0-based
	 */
	int getThresholdID();
	
	/*!
	 @method     
	 @abstract  return true if the field has any link text at current row
	 @discussion
	 */
	bool HasLinks();
	
	/*!
	 @method     
	 @abstract  get link text for the given type; return S_FALSE if that type is not defined 
	 @discussion
	 */
	int getLinkText(EnumDSSRWLinkType iType, std::string &orLinkText);
	
	/*!
	 @method     
	 @abstract  return true if the text is from right left (Hebrew)
	 @discussion
	 */
	bool IsRightToLef(){return mpIterator->IsRightToLef(mField);}

	/*!
	 @method     
	 @abstract  return the cell handle
	 @discussion
	 */
	DSSCellHandle getHandle();
	
	/*!
	 @method
	 @abstract return the field group iterator
	 @discusstion
	 */
	DSSRWFieldGroupIterator* getFieldGroupIterator();
	
	/*!
	 @method     
	 @abstract  return field's raw data if it's avaiable
	 @discussion
	 */
	int getFieldRawData(const char **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
	
    /*!
	 @method     
	 @abstract  return the data type of the transaction column mapped to the field, dssdatatypemissing if not mapped
	 @discussion
	 */
    EnumDSSDataType getMappedTransactionColumnType();
    
private:
	int mRow;
	int mField;
	DSSRWFieldGroupIterator *mpIterator;	
};


