/*
 *  DSSGlobalLookUpTable.h
 *
 *  Created by Xiaoqi Zeng on 09/22/11.
 *  Copyright 2011 MicroStrategy, Inc.. All rights reserved.
 *
 */

#pragma once

#import "DSSTabularAttribute.h"
#import "DSSTabularConsolidation.h"
#import <map>
#import <set>

class DSSBinaryDataReader;

typedef map<int, int, less<int> > IntToIntMap;

typedef vector<int> IntVector;

//ctang element info
class DSSXTabBitVector;

class DSSGlobalLookUpTable : public DSSTabularAttribute
{
public:
	
	DSSGlobalLookUpTable();
	~DSSGlobalLookUpTable();
    
    int LoadHeaderFromBinary(DSSBinaryDataReader *ipReadStream, bool ifFirtTime);
    int LoadDataFromBinary(DSSBinaryDataReader *ipReadStream, bool ifFirtTime, DSSObjectContext* ipObjContext);
    
    //xzeng 2012-04-20, to reduce memory leak.
    void ResizeDEVectors(int iDESize);
    void EraseInvalid(int iDEPos);
    
    //junwang, 580226, ifViewFirst has default "true" value, same with server
    int GetGlobalUnit(int iGlobalID, DSSTabularUnit** opUnit, int* opLocalKey, bool* ofDE, bool ifViewFirst = true, int* opSourceIndex = NULL, int* opViewIndx = NULL,vector<int> *ipvViewIndices=NULL);

    
    int IsDEElement(int iGlobalElementKey, bool* ofDE);
    
	//xzeng, load all DE related content except mvDEs at once 
	int LoadDERelatedPreFromBinary(DSSBinaryDataReader *ipReadStream);
    //partial update for DE
    int PartialUpdateForDE(DSSBinaryDataReader *ipReadStream, DSSObjectContext* ipObjContext);
    
    int GetRefUnit(EnumDSSTemplateUnitType* ipType, int iUnitIndex, DSSTabularUnit** oppTabularUnit);
    
    int GetDisplayForm(int iSourceIndex, int** oppForms, int* opnForms);
    
    //hyan,2012/02/01
    int GetDisplayFormUnionForControl(vector<int> &irvUnionForms);
    
    int GenerateDisplayFormUnionForControl();

    
    int HasDE(bool* opfHasDE);
    
    int CountRows();
    
    //junwang, 2012/02/20
    int GetMaxGlobalKey(int* opMaxKey);
    
    //ctang 2012-04-16
    int IsDEOnView(DSSTabularConsolidation* ipCS, bool* opFlag);

    int GetDELocalToGlobalMap(DSSTabularUnit* ipUnit, int** oppMap, EnumDSSTemplateUnitType iType);
    vector<int>* getLocalToGlobalMap(GUID iDataSetGUID);
    //junwang, 2012/04/18
    //void dump();
    
    int GetSortingResult(vector<int>& ivForms, bool* ipAsc, bool ibNullOnTop, int** oppSortingResult, int* opnResult);
    
    int getBucketID(vector<int>& ivForms, bool* ipAsc, bool ibNullOnTop);
    int getSortResult(int iBucketID, int** oppSortingResult, int* opnResult);
    int setSortingResult(vector<Int32>& ivForms, vector<bool>& ipAsc, bool ibNullOnTop, Int32* ipOrder);
    bool operator()(int a, int b);//zhyang
    
    
    //junwang, 2012/08/15, get branch elements containing the leaf element
    int getBranchElements(int iGlobalKey, vector<int>& ovBranchElements, bool lfInCludeDeOnView=false);
    
    // 12/18/2012 tehe
    int setTrimFlag(bool ibTrimFlag);
    
    
    //hyan, 1/30/2013
    int getElementCount();
    
    void cleanup();
    
    //zhyang, 788933, add an interface to get all child elements from difference DE with the same global key
    int GetAllGlobalUnit(int iGlobalID, vector<DSSTabularUnit*> *opUnit, vector<int> *opLocalKey, vector<bool> *ofDE, bool ifViewFirst = true, vector<int> *opSourceIndex = NULL, vector<int> *opViewIndx = NULL, vector<int> *ipvViewIndices = NULL);

    
    //int GetGlobalToLocalMap(DSSTabularUnit* ipUnit, Int32* opRowCount, Int32** oppRowMap, IntToIntMap** oppRowMap2);
    vector<int>* getGlobalToLocalMap(GUID iDataSetGUID);
    IntToIntMap* getGlobalToLocalMap2(GUID iDataSetGUID);
    // jimzhang, Dec/10/2013, TQMS 820419. When extending columns of global lookup table, we should extend the row count of global lookup table. That's why we add this function.
    int ExtendRow(int inRow);
    
    //zhyang, 812102
    int FindElement(DSSBaseElementProxy *ipElement, int &orKey);
    
    //zhyang, 900741
    int resetSortingResult();
    
    int getParentElementKeys(vector<int>& oParentkeys);
    
private: //private methods
//  a wrapper of DSSDataColumn::Compare. Return value also depends on iAsc.
    static int compareHelper(DSSDataColumn *ipColumn, long long iRow1, long long iRow2, bool ifIsNullDisplayOnTop, int iLocal, bool ibSimpleComparison, bool iAsc);
    
private:
    
    //int mArity;
    int mnColumns;
    
    int mnRow;
    std::vector<int> mvColumnAttributes;
    std::vector<int> mvColumnLocales;
    
    std::vector< std::vector<int> > mvSortingForms;
    std::vector<bool*> mvAsc;
    std::vector<int*> mvSortingResult;
    std::vector<bool> mvNullOnTop;
    int mSortIndex;  //zhyang, UC will bring new elements, this will indicate which sort result it will be merged to
    //std::vector<bool> mvExistingRows;
    bool *mpExistingRows;
    std::vector<bool> mvIsSerializedFromServer;
    std::vector<bool> mvSortingResultDirty;
    int sortAndMergeCertainSortingResult(int iBucketID);
    
    //DE, below DE related content are the same as in the server side, which are created by ctang
	bool mfHasDE;
	vector<DSSTabularConsolidation*> mvpDEs;
	vector<int> mvMaxGlobalKey;
    
	vector<int> mvUnitEleCount;
	vector<IntVector*>mvpDELocalToGlobalMap;
	vector<IntToIntMap*> mvpDEGlobalToLocalMap;
    
    vector<GUID> mvDataSetGUID;
    vector< vector<int> > mvLocalToGlobalMap;
	vector< vector<int> > mvGlobalToLocalMap;
    vector<IntToIntMap*> mvGlobalToLocalMap2;
    
	int mGlobalEleCount;
    
	vector<GUID> mvViewTemplateID;
	int mnBaseDE;

    //ctang display form
    vector<int*> mvpDisplayForms;
    vector<int> mvDisplayFormCount;
    
    //hyan, display form union for control
    vector<int> mvDisplayFormUnion;
    
    int* mpMapDEFromBinaryToStorage;
    int mnDEInBinary;
    
    
    //ctang:
    DSSXTabBitVector* mpDEElement;//is this element an attribute element
    //DSSXTabBitVector* mpDisplayElement;//if it is a de element, is this element a display element
    
    vector<int>  mvSourceIndex4DE;
    //ctang: the view index is used for solve the unstable sorting of DE elements
    vector<int> mvViewIndex4DE;
    
    std::vector<int> mvParentElements; //for financial report control
	
};