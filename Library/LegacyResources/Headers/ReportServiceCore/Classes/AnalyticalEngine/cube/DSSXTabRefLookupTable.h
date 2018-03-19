//
//  DSSXTabRefLookupTable.h
//  ReportServiceCore
//
//  Created by Xiaoqi Zeng on 2/23/12.
//  Copyright 2012 MicroStrategy. All rights reserved.
//

#pragma once
#include "DSSTabularAttribute.h"

class DSSXTabRefLookupTable : public DSSTabularAttribute
{
private:
    DSSTabularAttribute* mpTabularAttribute;
    int mnForms;
    int* mpForms;
    
public:
    DSSXTabRefLookupTable();
    ~DSSXTabRefLookupTable();
    
    int Init(DSSTabularAttribute* ipTabularAttribute);		
    int SetSelectedForms(bool ifFull, vector<int>& ivrForms, vector<int>& ivrLocale);
    //    int getColumnForms(std::vector<int>& ovColumnForms);
    //	int getColumnLocales(std::vector<int>& ovColumnLocals);
    int getArity(int* opArity);
    int setArity(int iArity);
    int populate(DSSTabularAttribute originalUnit, int iUnit, int rowLimit = -1);
	int findFormWithLocale(int iAttribute, int iForm, int iLocale, int &orColumn);
    int getUnitType(EnumDSSTemplateUnitType* oUnitType);
    int getGUID(GUID &opGuid);//zhyang, 03/12/2012, change from GUID* to GUID& to implement the iterface in super class
	virtual const GUID &getGUID();
	int setGuid(GUID iGuid);
	int FindElement(DSSBaseElementProxy *ipElement, int &orKey);
	int	createElement(int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement, int iGlobalKey=-1,int iGlobalKeyVer = -1);
    int FindElement(EnumDSSDataType * ipTypes, Int32 * ipcbDatas, unsigned char ** ippDatas, DSSDataFlag * ipFlags, int &orKey);
    
    //zhyang, implement some function in super class
    virtual int CountRows();
    int FindFormWithLocale(int iAttribute, int iBaseFormID, int iLocale, int &orColumn);
    int Item(int iForm, DSSDataColumn **oppColumn);
    int getArity();
	
    //zhyang, 03/12/2012, for the following functions, maybe we haven't use them, just implement them.
	int AddFormDataColumn(int iBaseFormID, int iBaseFormType, DSSDataColumn *ipColumn);
	int Count();
	int getBaseFormID(int iForm);
	EnumDSSBaseFormType getBaseFormType(int iForm);
	int size();
	//to support transaction data
	int setData(int iBaseFormID, int iRow, char *ipData, int iSizeInByte);
	int setData(int iBaseFormID, int iOffset, MBase::CString &irText);
    
    //zhyang 677674
    int enableSSO();
    int disableSSO();
    bool canUesSSO();
    
    //hyan, 1/31/2013
    int getElementCount();
    
    
    
    virtual int getTabularUnit(DSSTabularUnit** oppUnit);
private:
    //zhyang, 677674, whether we can use selector sort optimization
    bool mbCanUseSSO;
};
