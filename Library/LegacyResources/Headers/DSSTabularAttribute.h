/*
 *  DSSTabularAttribute.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import "DSSTabularUnit.h"

class DSSDataColumn;

class DSSTabularAttribute : public DSSTabularUnit
{
public:
	DSSTabularAttribute();
	~DSSTabularAttribute();
	
	int Init(int index, int arity);
	
	virtual int AddFormDataColumn(int iBaseFormID, int iBaseFormType, DSSDataColumn *ipColumn);
	
	virtual int Count();
	
	virtual int Item(int iForm, DSSDataColumn **oppColumn);
    
	virtual int FindFormWithLocale(int iAttribute, int iBaseFormID, int iLocale, int &orColumn);

    inline int FindFormWithLocale(int iAttribute, int iBaseFormID, int iLocale, int *opColumn)
    {
        return opColumn ? this->FindFormWithLocale(iAttribute, iBaseFormID, iLocale, *opColumn) : E_INVALIDARG;
    };

	virtual int getArity();
	
	virtual int getBaseFormID(int iForm);
	
	virtual EnumDSSBaseFormType getBaseFormType(int iForm);
	
	virtual int CountRows();
    
	virtual int FindElement(DSSBaseElementProxy *ipElement, int &orKey);
    virtual int FindElement(EnumDSSDataType * ipTypes, Int32 * ipcbDatas, unsigned char ** ippDatas, DSSDataFlag * ipFlags, int &orKey);
    
	virtual int	createElement(int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement, int iGlobalKey=-1, int iGlobalKeyVer = -1);
    
    int createNewDrivedElement(int key, DSSObjectContext *ipObjectContext, GUID * ipConsolidationElementID, DSSBaseElementProxy **oppElement, int iGlobalKey, int iGlobalKeyVer);
    
    //hyan,2012/03/16
    //add for resolve attributeID of AutoText
    virtual int	getAttributeElementIDForAutoText(int key, DSSObjectContext *ipObjectContext, MBase::CString &irAttributeIDText);
    
	virtual int size();
    
	//to support transaction data
	virtual int setData(int iBaseFormID, int iRow, char *ipData, int iSizeInByte);
	virtual int setData(int iBaseFormID, int iOffset, MBase::CString &irText);
    
    //juwnang, 04/19/2012
    virtual bool compareElement(DSSBaseElementProxy *ipElement, int iKey);
    
    bool FindElement(DSSBaseElementProxy *ipElement, const int* ipKey, int iNumKeys, int &orKeyIndex);
    
    //To support mapped file
    int createMMapMemory(const char * ipFilePath, bool ibUseNewFile);
    int MapFileBuffer();
    int UnMapFileBuffer();
    
    int GetAllForm(vector<int>& orvForms);
    
    int getWHNullPosition();
    void setWHNullPosition(int iPosition);
    
    int replaceColumn(vector<DSSDataColumn*> ipNewColumn);
protected:
    
	int mArity;
	int mnForms;
	vector<DSSDataColumn* > mvColumns;
	vector<int> mvBaseFormID;
	vector<EnumDSSBaseFormType> mvBaseFormType;
	
	vector<int> mvKeyFormID;
	vector<EnumDSSBaseFormType> mvKeyFormType;
	
	vector<unsigned char> mvIDFormDataBuffer;
    //fali, for dataset transaction. WHNull's position is not fixed
    int mWHNullPosition;
};
