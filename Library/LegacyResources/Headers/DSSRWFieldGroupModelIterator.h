/*
 *  DSSRWFieldGroupModelIterator.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 9/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef RWFieldGroupModelIterator_H
#define RWFieldGroupModelIterator_H

#import "DSSRWNodeModelIterator.h"
#import "DSSRWFieldGroupIterator.h"
#import <set>

typedef enum EnumThresholdResultType
{
    ThresholdInvalid = -1,
    ThresholdFalse = 0,
    ThresholdTrue = 1
} EnumThresholdResultType;

//hyan,2012/03/28
//add for get format
typedef enum EnumFormattingFieldType
{
    EnumFormattingFieldTypeReserved = 0,
    EnumFormattingFieldTypeAttribute = 1,
    EnumFormattingFieldTypeMetric = 2,
    EnumFormattingFieldTypeCalculationField=3
    
} EnumFormattingFieldType;

class JoinedIndexTable;
class DSSField;
class DSSFunctionServer;
class DSSExpNode;   
class DSSExpNodeOperator;
class DSSRWFieldGroupModel;
class DSSFunctionObject;
class FieldLink;

class DSSRWFieldGroupModelIterator : public DSSRWNodeModelIterator, public DSSRWFieldGroupIterator
{
public:	    
	DSSRWFieldGroupModelIterator();	
	virtual ~DSSRWFieldGroupModelIterator();
    
    int Init(DSSRWDataModel* pDataModel, DSSRWNodeModel *pNodeModel, DSSRWNode *pNode, DSSRWSectionModelIterator *ipParentIterator);
    
    virtual int ReInit();

    virtual int getCommandList(DSSCellHandle &irHandle, vector<EnumManipulationCommandTypes> &orvCommand);   
    
    virtual int getDataChangeType(DSSCellHandle &irHandle, bool ibTrxSubPreset = false);
    
    virtual int CountFields();
    virtual int CountRows();  
    virtual int CountJoinedIndexRows();
    
    virtual bool FirstRow();    
    virtual bool NextRow();
    
    virtual FieldGroupCell getFieldGroupCell(int iField);
    virtual FieldGroupCell getFieldGroupCell(int iRow, int iField);

    virtual bool getNullChecking();
    bool getTrimString();
    
    int getFieldText(int iRow, int iField, MBase::CString &orText);
    
    int getThresholdID(int row, int field);
    
    int getMetricValue(int iRow, int iRTTIndex, int iMetricIndex, const char *&orpData, int &orDataSize, EnumDSSDataType &orDataType, DSSDataFlag &orDataFlag);
    
    int getCurrentMetricValueByGUID (GUID *ipReportGUID, GUID *ipMetricGUID, const std::string *ipFieldKey, AERawData *opRawData);
    
    int getCurrentFormValueByID(DSSAttribute *ipAttr, GUID *ipFormGUID, AERawData *opRawData);
    
    int getCurrentElementProxyByObjInfo(DSSObjectInfo *ipObjInfo, DSSBaseElementProxy **oppCurElementProxy);
    
    int getTag(int * opTag);
    
    int FindUnit(GUID &irUnitID, int *opUnit);
    
    int getElementIndex(int iRow, int iUnit, int * opElementIndex);

    int getLocalUnitID(int iUnit, Int32 *oplocalUnitID, bool ibUseBaesIndexTable = false);

    int getElementIndexFromIndexTable(int iRow, int iUnitLocalIndex, int *opElementIndex);

    int getElementIndexFromBaseIndexTable(int iRow, int iUnitLocalIndex, int *opElementIndex);
    
    //fali, 827602, add parameter ibGetDDICOriginal
    int getRawData(int iRow, int iField, const char **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag, bool ibGetDDICOriginal=false, bool ibDiscardTrx = false);
    
    int getMetricPos(int iReport, GUID &irMetricGUID, int &orMetricPos);
    int getRTTIndex(int iReport, int &orRTTIndex);
            
    //hyan,2011/12/21,add for support link
    bool HasLinks(int iRow,int iField);
    
    int getLinkText(int iRow,int iField,EnumDSSRWLinkType iType, std::string &orLinkText);
    
    //hyan,2012/01/18,add for number formatting
    DSSFormat* getFieldFormat(DSSField *ipField);
    
    //hyan,2012/03/28
    DSSFormat* getFieldFormat(DSSField *ipField,EnumFormattingFieldType iFieldType,DSSFormat* ipParentFieldFormat,DSSThreshold *ipThreshold);
    
    virtual DSSDataModelTransaction* getTransactionDataModel();
    
    virtual int UpdateData(DSSCellHandle &irHandle, CComVariant &irNewValue);
    
	virtual int getFieldIndexFromHandle(DSSCellHandle &irHandle);
	
	virtual int getRowIndexFromHandle(DSSCellHandle &irHandle);
	
    virtual int CreateInputControlIterator(std::string iKey, DSSRWInputControlIterator** oppControlIterator);
    
    virtual int GenerateTransactionRecord(DSSCellHandle &irHandle, CComVariant &irNewValue);
    
    virtual int UpdateFieldCell(DSSCellHandle &irHandle, CComVariant &irNewValue);
    
    virtual int DiscardTransactionChange(DSSCellHandle &irHandle);
    // 5/28/2012 tehe 
    void ReleaseUnusedMemory();
    void hCleanUp();
    
    int MarkRows(int iRowOrdinal, EnumDSSRWMarkRowAction iActionType);
    
    EnumDSSDataType getTransactionInputDataType(DSSCellHandle& irHandle); 
    bool isTransactionModelValidated();
	//get the current slice offset id, used by transaction, e.g. 758889
	int getCurrentBlockOffset();
    
    //Conditional logic support
    virtual int AnalyzeTransactionThreshold(DSSThreshold* ipThreshold, bool* opResult);
    
    virtual int UpdateRequiredColumnStatus();
    //get raw data for intial binary
    int getRawDataForInitialRendering(int iRow, int iField, int iDataSize, EnumDSSDataType iDataType, const char **oppData, DSSDataFlag *opDataFlag);
    
    int getFieldDisplayText(int iRow, int iField, MBase::CString &orText);
    
    bool isDDICOnField(int iField);
    
protected:

    //fali, 827602, add parameter ibGetDDICOriginal, if true, get the original value of DDIC, else get the current selected value
	virtual int hGetFieldData(int iRow, vector<AERawData> &irvOriginalData, bool ibGetDDICOriginal=false);


    
private:
    int hReInitFieldData();
    
    int hReInitFieldTextAndLink(int iMainFieldIndex, int iLinkLocalIndex, DSSField *ipField, DSSFormat *ipFormat, string *iParentKey);
    
    int hReInitMetricField(int iMainFieldIndex, int iLinkLocalIndex, DSSField *ipField, DSSFormat *ipFormat, int iRTTIndex, int iMetricIndex);
    
    int hReInitAttrField(int iMainFieldIndex, int iLinkLocalIndex, DSSField *ipField, DSSFormat *ipFormat, string *iParentKey, GUID &irAttrGUID);
    //To be implemented.
//    int hReInitConsolidationField(int iMainFieldIndex, int iLinkLocalIndex, DSSField *ipField, DSSFormat * ipFormat, string *iParentKey, GUID &irCsGUID);
    //yimliu, 2014/07/16, 920357, 919585.
    int hReInitConsolidationFieldSimple(int iMainFieldIndex, int iLinkLocalIndex, DSSField *ipField, DSSFormat * ipFormat, string *iParentKey, GUID &irCsGUID);
    int hReInitThreshold(int iFieldIndex, DSSField *ipField);
    
    int hReInitTrxFieldDataType();
    
    void hReInitMembers();
    
    int hReInitFieldsMapping();
    
    int hReInitFieldMapping(DSSField *ipField);
    
    int hFindField(int iFieldIndex, DSSField **oppField);
    
    int hGetElementIndex(JoinedIndexTable* ipIndexTable, int iUnit, int iRow, int *opElementIndex);
    
    int hGetBaseIndexTable(JoinedIndexTable **opIndexTable);
    
    int hRefineGBElementsForOLView(vector< vector<Int32> > *opvvOLGroupByElements);
    
    int hGetMetricIndex(GUID &irSourceGUID, GUID &irMetricGUID, int &orRTTIndex, int &orMetricIndex);
    
    //fali, 827602, treat discard and get original DDIC value as the same, that means it will get value based on data before selection
    int hCalculateRawData(int iMainIndex, DSSField *ipField, int iRow, const char **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag,bool ibDiscardOrOriginalDDICElement = false);
    
    int hBuildTransactionWriteBackColumnMap(DSSRWNode * ipNode);
    
    void hEscape(MBase::CString &irText, const char &irChar, const char *ipReplaceStr);
    
    void hEncodeXML(MBase::CString &irText);
    
    void hEncodeURL(MBase::CString &irText);
    //tliao, 2013-1-15, port server side logic (encode link text) to client side
    int hGetEncodedLinkText(MBase::CString &irText, MBase::CString* iCurrentText, EnumDSSRWFieldType iFieldType, EnumDSSRWLinkType iLinkType, EnumDSSAutoText iAutoTextType);
    
    int hBuildOfflineTransactionColumnMap(DSSRWNode * ipNode);
    int hSetOfflineTransactionMapping();
    
    Int32 mnUnits; // including metrics

    vector<Int32> mvRows;
    Int32 mnRows;
    int mnDisplayRows;
    
    int mCurrent;    
    int mBaseCurrent;
    
    int mnFields;
    
    vector<Int32> mvSortUnits;
    JoinedIndexTable *mpIndexTable;
    JoinedIndexTable *mpBaseIndexTable;
    vector<vector<Int32> > mvGroupByElements;
    
    vector<MBase::CString> mvFeildsText;
    
    //hyan,2012/03/16
    //add for support link(HTML container and hyperlink)
    vector<FieldLink*> mvLinkFieldText;
    
    // Oct/16/2013, TQMS 801939, do not cache field's raw data, since these raw data may contain pointer which point to string in attribute column, and this string in attribute column may be deleted when editing Grid.
    //map<int, AERawData> mmCellIndex2RawData;
    
    //hyan,2012/03/16
    //add for support resolving attributeElementID for autotext
    map<string,MBase::CString> mvAttributeElementID;
    
	vector<int> mvThresholdID;  // 0-based, -1 means there is no threshold
    
    map<MBase::CString, int> mmFieldToRTTIndex;
    map<MBase::CString, int> mmFieldToMetricIndex;
    
    string mResolvedAutoText;
    // jimzhang Apr/6/2012 As function server is used in every calculation field and threshold evaluation, 
    //      define it as a member of field group iterator, and new an object only when it's needed.
    //      An alternative way is defining DSSFunctionServer as singleton pattern. But this will always new an object of function server,
    //      even if this object is never used.
    DSSFunctionServer *mpFunctionServer;
    
    map<DSSExpNodeOperator *, DSSFunctionObject *> mmOperator2FunObj;
    
    map<int, EnumDSSDataType> mmTrxFieldDataType;
    map<int, int> mmTrxFieldDataSize;
    
    bool mbHasTransactionEnabled;
    
    vector<int> mmField2WBForm;
    
    vector<bool> mvIsTrimed;
    
    
    //offline transaction support
    vector<int> mvOfflineMapToFields;	//map from transaction report column to a field
	vector<int> mvOfflineMapToWBColumn;	//map from field to transaction report column
    vector<EnumDSSDataType> mvOfflineDataType;
    vector<int> mapOfflineToTrxColumn;
    
    //fali, 960685, keep pointer of newly allocated AERawData
    vector<char *> mvpAllocatedRawData;
    friend class DSSRWFieldGroupCompare;
};

#endif