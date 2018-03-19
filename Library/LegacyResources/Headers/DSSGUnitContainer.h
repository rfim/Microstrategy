//
//  DSSGUnitContainer.h
//  ReportServiceCore
//
//  Created by Xiaoqi Zeng on 9/29/11.
//  Copyright 2011 MicroStrategy. All rights reserved.
//

#pragma once

#include "DSSGlobalLookUpTable.h"

class DSSBinaryDataReader;

typedef map<int, int, less<int> > IntToIntMap;

class DSSGUnitMap
{
    friend class DSSGUnitContainer;
    
public:
    DSSGUnitMap();
    ~DSSGUnitMap();
    
    int setGUID(MBase::CString iStringGUID);
    int LoadHeaderFromBinary(DSSBinaryDataReader *ipReadStream, bool ifFirstTime);
    int LoadDataFromBinary(DSSBinaryDataReader *ipReadStream, DSSObjectContext *ipObjContext);
    
    int GetUnitIndexAndLocalKey(int iGlobalKey, int* opUnitIndex, int* opLocalKey);
    vector<int>* GetGlobalToLocalMap(DSSTabularUnit* ipUnit);
    vector<int>* GetLocalToGlobalMap(DSSTabularUnit* ipUnit);
    int findElement(DSSBaseElementProxy *ipElement, int &orKey);

private:
    
    GUID mID;
	EnumDSSTemplateUnitType mUnitType;
	vector<DSSTabularUnit*> mvUnits;
    //tliao, 905202, 2014/08/22, avoid memory leak, we can't delete mvUnits directly before destructor
    //we will archive the mvUnits if new binary is coming and delete them all in desstructor
    vector<DSSTabularUnit*> mvArchiveUnits;
    
	int mGlobalRowCount;
	vector<int> mvUnitRowCount;
	vector<int*> mvLocalToGlobalMap;
	vector<int*> mvGlobalToLocalMap;
    
};

class DSSGUnitContainer
{
public:
    DSSGUnitContainer();
    ~DSSGUnitContainer();
    
    int Init();
    int getAttributeNum();
    
    //ctang 2011-11-16
    int getGlobalLookup(int iIndex, DSSTabularAttribute** oppLookupTable);
    int getGlobalLookupByID(GUID* ipID, int* opIndex, DSSTabularAttribute** oppLookupTable);
    
    int getGUnitMap(int iIndex, DSSGUnitMap** oppGUnitMap);
    int getUnitFromGUnitMapByID(GUID& ipGUID, EnumDSSTemplateUnitType* ipType, int iUnitIndex, DSSTabularUnit** oppTabularUnit);
    
    //ctang: return the number of attributes
    int countLookupTables();
    //ctang: return the number of all units, include CS and CG
    int count();
    int GetGlobalUnitIDByGUID(GUID iID, int* opGlobalID);
    int GetGlobalUnitGUIDByID(int iGlobalID, GUID* opGUID);
    int GetUnitType(int iIndex, EnumDSSTemplateUnitType* opType, bool* opfDE);
    
    int GetGlobalRowCount(Int32 iIndex, Int32 *opGlobalRowCount, bool ifIncludeDE);
    
    int GetGlobalUnit(GUID& iCSID, int iGlobalID, DSSTabularUnit** opUnit, int* opLocalKey, bool* ofDE);
    
    
    int LoadHeaderFromBinary(DSSBinaryDataReader *ipReadStream);
    int LoadDataFromBinary(DSSBinaryDataReader *ipReadStream, DSSObjectContext *ipObjContext);
    
    int getUnitsForViewDataSet(GUID* ipGUID, EnumDSSTemplateUnitType* ipType, int iUnitIndex, DSSTabularUnit** oppTabularUnit);
    
    //junwang, 12/21/2011
    int Order(int iUnit, int iForm, int iRow1, int iRow2, bool fIsNullDisplayOnTop, bool fIsParentFirst, int Locale,
              bool fUseDefaultSort, int* pResult, bool fUseBaseElement,vector<int> *ipvViewIndices=NULL/*TQMS 731649,hyan,3/27/2013 following flash's change*/);
    //ctang
    //int GetLocalToGlobalMap(DSSTabularUnit* ipUnit, int** oppMap);
    //int GetGlobalToLocalMap(DSSTabularUnit* ipUnit, Int32* opRowCount, Int32** oppRowMap, IntToIntMap** oppRowMap2);
    
    vector<int>* getLocalToGlobalMap(GUID iDataSetGUID, DSSTabularUnit* ipUnit);
    vector<int>* getGlobalToLocalMap(GUID iDataSetGUID, DSSTabularUnit* ipUnit);
    IntToIntMap* getGlobalToLocalMap2(GUID iDataSetGUID, DSSTabularUnit* ipUnit);
    //zhyang
    int getBucketID(int iUnitID, vector<int>& ivForms, bool* ipAsc, bool ibNullOnTop);
    int getSortResult(int iUnitID, int iBucketID, int** oppSortingResult, int* opnResult);
    int setSortingResult(int iUnitID, vector<Int32>& ivForms, vector<bool>& ipAsc, bool ibNullOnTop, Int32* ipOrder);
    
    // 12/18/2012 tehe
    int setTrimFlag(bool ibTrimFlag);
    
    int clear();
    
    int findElement4CSCG(DSSBaseElementProxy *ipElement, int &orKey);
    
    bool isDerivedUnit(GUID &lID);
    vector<GUID>* getLinkedAttributes(const GUID iGUID);
    
private:
    int mAttributeNum;
    vector <GUID> mvLookUpIDs;
    vector <DSSGlobalLookUpTable*> mvpGlobalLookUps;
    vector<DSSGUnitMap*> mvpGUnitMapList;
    vector<DSSTabularUnit *> mvDerivedUnits;
    
    vector<vector<GUID> >mvvAttrLinkedInfo;
    map<GUID, int, less<GUID> > mMapToLinkedGroupID;
    
    bool mfLoadStatus;
    
    bool mfReLoad;
};
