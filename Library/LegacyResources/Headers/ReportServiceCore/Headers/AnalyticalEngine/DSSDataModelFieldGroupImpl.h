/*
 *  DataModelFieldGroup.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 3/20/10.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import "DSSDataModelBase.h"
#import "DSSField.h"

class FieldLink{
public:
	FieldLink(int lnLinks)
	{
		mtype.resize(lnLinks);
		mtext.resize(lnLinks);
	};
	
	~FieldLink()
	{
		if (mtext.size() > 0)
		{
			vector<MBase::CString*>::iterator it = mtext.begin();
			for (; it < mtext.end(); it++)
			{
				if (*it)
				{
					delete *it;
					*it = NULL;
				}
			}
		}
	}
	
	int size();
	
	vector<EnumDSSRWLinkType> mtype;
	vector<MBase::CString*> mtext;
};

class DSSDataModelFieldGroupImpl : public DSSDataModelBase,public IDSSFieldGroupModel
{
public:
	DSSDataModelFieldGroupImpl();
	~DSSDataModelFieldGroupImpl();
	
	int Init(int nRows, int nFields);
	
	int Init();//set mType
	
	int AddField(int index, MBase::CString * ipText, int iThresholdID);
	
	int AddLink(int index, FieldLink *ipLinnk);
	
	int CreateViewMap(vector<int> &irGBPath);
    
    //when the irGBPath is already sorted
    int CreateViewMap2(vector<int> &irGBPath);
	
	int CountRows();
	
	int CountFields();
	
	int getFieldText(int row, int field, MBase::CString &orText);
	
	int getThresholdID(int row, int field);
	
	int getLink(int row, int field, FieldLink **oppLink);
	
	bool HasThreshold(int row, int field);
	
	bool HasLinks(int row, int field);
	
	int Select(int nPaths, int nGBUnits, vector<GUID> &irGBUnitID, int *ipGBPaths, vector<int> &orvElements);
	
	int AddStaticField(int iField, vector<MBase::CString *> & irvpFieldText);
	
	int AddStaticFieldLink(int iField,  vector<vector<MBase::CString *> > & irvpLinkText);
	
	int AddField(int index, int iField, vector<MBase::CString *> &irvpSubText, int iThresholdID);
	
	int AddLink(int index, int iField, FieldLink *ipLinnk, vector<vector<MBase::CString *> > &irvpSubText);
	
	virtual int size();

	int setFieldText(int row, int field, MBase::CString &irNewText);
	
	int AddRawData(int index, AERawData &irVal);
	
	int getRawData(int row, int field, const char **oppData, int *opDataSize, EnumDSSDataType *opDataType, DSSDataFlag *opDataFlag);
	
	int setRawData(int row, int field, MBase::CString &irText, EnumDSSDataType iDataType = DssDataTypeMissing);
	
	virtual void ReleaseUnusedMemory();
		
	//for offline transaction
	int getBinaryTransactionModel (bool includeCommitted, unsigned char** oppBinaryModel, int* opSize);
	
	int setBinaryTransactionModel (unsigned char * ipBinaryModel, int size);
    static int LoadBinaryTransactionModel (unsigned char * ipBinaryModel, int iSize, DSSDataModelTransaction *ipTransactionModel, DSSDataModelFieldGroupImpl * iFieldGroupModel = NULL);
    
	int EnsureTransactionData();
    static int EnsureTransactionData(DSSDataModelTransaction *ipTransactionModel, DSSRWFieldGroupNode  *ipFGNode, DFCCSITransactionBase* ipCSI = NULL);
    virtual DSSDataModelTransaction *getTransactionDataModel(){return mpTransactionModel;};
    int setDataSource(DSSDataSource *ipDataSource);
    
    int LoadData(vector<int>& irvRows);
    
    void setHasStaticFieldLookup(bool ibHasStaticFieldLookup){mbbHasStaticFieldLookup = ibHasStaticFieldLookup;}
    bool getHasStaticFieldLookup(){return mbbHasStaticFieldLookup;}
    
    int getRowOrdinal(int& iRow);
    
    // jimzhang, Sep/29/2012, to support recalculate of threshold in transaction
    int setThresholdID(int row, int field, int iThresholdID);
        
    int LoadInputControlModel(std::string iKey, DSSDataModelControlImpl** oppControl);
private:
	
    //for offline transacton
    static int hFindSourceField(DSSColumnMappingInfo * ipColumnInfo, DSSField *ipField);
    
    int hSelectDistinctFromSortedKeys(vector<int> &irvKeyTable, int nKeys, vector<int> &orSortedList, vector<int> &orDistinctList);
    
	void hCleanUp();
	
	int mnRows;				//number of rows
	
	int mnFields;			//number of field per row
	
//	int mnTotalFields;		//equal mnRows * mnFidlds
	
	bool mbHasThreshold;
	
	bool mbHasLinks;
	
	vector<MBase::CString*> mvText;
	vector<AERawData> mvRawData;
	
	//used for conditional formatting, start from 1. 0 means no threshold
	vector<int> mvThresholdID;
	
	vector<FieldLink*> mvLinks;
	
//	vector<vector<int> > mvRowList;
    vector<int> mvRowList;
    vector<int> mvRowOffset;
	vector<vector<MBase::CString*> > mvStaticSubField;
	vector<vector<vector<MBase::CString*> > > mvStaticSubLink;
    
    DSSDataSource* mpDataSource;
    bool mbbHasStaticFieldLookup;
};
