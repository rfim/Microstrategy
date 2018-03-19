//
//  DSSRWCompare.h
//  ReportServiceCore
//  Port from server-side DSSRWCompare.h
//  Created by junwang on R.O.C. 2011/12/12.
//  Copyright 2011 Microstrategy. All rights reserved.
//  
#ifndef DSSRWCOMPARE_H
#define DSSRWCOMPARE_H
#import <vector>
using namespace std;

#ifndef NULL
#define NULL 0
#endif

class DSSSort;
class DSSAttribute;
class DSSRWIterator;
class DSSRWControlModelIterator;
class DSSRWSectionModelIterator;
class DSSRWFieldGroupModelIterator;
class DSSRWGroupbyIterator;
class DSSRWDataModel;
class DSSRWNodeModel;
class DSSRWDDICInputControlIterator;
class DSSRWControl;
class DSSRWFieldGroupModel;
class DSSDataRefColumn;
class DSSObjectInfo;

//ctang: New RWD special vaule
typedef enum DSS_GroupByElement
{
    DSS_AnyElement = -5,
    DSS_TotalElement = -100
} DSS_GroupByElement;

class DSSRWCompare
{
public:
    
	DSSRWCompare() : mfDisplayNullOnTop(true), mLocale(0), mCompareType(eReservedCompare),
    mpModelIterator(NULL),mpDataModel(NULL){}
    
	void InitLocale(int& irLocale)
	{
		mLocale = irLocale;
	}
    
    int getSortInfo(int &oUnit, vector<int> &ovForms, bool **oppAscs);
    
	virtual bool operator() (int row1, int row2);
    
protected:
	int hConvertAttributeDefaultSort(DSSAttribute *pAttribute, bool lbUseBaseElement = false);
	int hConvertAttributeFormSort(DSSSort *ipSort, bool iIsDefaultSort = false, bool lbUseBaseElement = false);
	int hConvertCustomGroupSort(DSSSort *ipSort);
	int hConvertConsolidationSort(DSSSort *ipSort);
    
    
	void hInsertSort(bool ifMetric, int iUnitOrReport, int iFormOrMetric, bool ifAsc, bool ifParentFirst, bool ifUserDefaultSort, bool ifUseBaseElement = false);

    //TQMS731649, hyan, 3/26/2013
    int getViewIndexFromTarget(DSSRWNodeModel* ipNodeModel);
    
    //xiazhou, for DDIC targeting
    int getViewIndexFromTargetForDDIC(DSSRWControl* ipControl);
    vector<int> mvViewIndices;
    
	DSSRWIterator* mpModelIterator;
    DSSRWDataModel* mpDataModel;
    
    vector<bool> mvfMetric;
    vector<int> mvUnitOrReport;
	vector<int> mvFormOrMetric;
	vector<bool> mvfAsc;
	vector<bool> mvfParentFirst;
	vector<bool> mvUseDefaultSort;
	vector<bool> mvUseBaseElement; // rzhu, 11/22/2011, TQMS#520205, used for base DE in UC only, specify oder by base dataset or view dataset
    
    //junwang, 05/11/2012, optimize for compound null determination
    vector<int>  mvGlobalRowCount;
    
	bool mfDisplayNullOnTop;
	int  mLocale;
    
	typedef enum {eReservedCompare = 0, eSectionCompare, eControlCompare, eFieldGroupCompare, eGroupbyCompare, eDDICCompare} EnumCompareType;
    
	EnumCompareType mCompareType;
};

class DSSRWSectionCompare : public DSSRWCompare
{
public:
	DSSRWSectionCompare(){mCompareType = eSectionCompare;}
	
    int Init(DSSRWSectionModelIterator* pSectionIterator, bool iDisplayNullOnTop);

    //junwang, new rwd, 12/26/2011, avoid switch case
    virtual bool operator() (int row1, int row2);
};

class DSSRWControlCompare : public DSSRWCompare
{
public:
	DSSRWControlCompare(){mCompareType = eControlCompare;}
	int Init(DSSRWControlModelIterator* pControlIterator, bool iDisplayNullOnTop);
    
    //junwang, new rwd, 12/26/2011
    virtual bool operator() (int row1, int row2);
};

class DSSRWFieldGroupCompare : public DSSRWCompare
{
public:
	DSSRWFieldGroupCompare(){mCompareType = eFieldGroupCompare;}
	int Init(DSSRWFieldGroupModelIterator* pFieldGroupIterator, bool iDisplayNullOnTop, vector<int>& lvSortUnits, bool ibUseBaseIndexTable = false/*rzhu, 07/07/2011, indicate whether it is sorting mpBaseIndexTable*/);

    //junwang, new rwd, 12/26/2011
    virtual bool operator() (int row1, int row2);
    
    //junwang, new rwd, 12/29/2011
    int hConvertMetricSort(DSSSort *ipSort);
    
	bool mbUseBaseIndexTable;
};

//junwang, new rwd, 12/22/2011
class DSSRWGroupbyCompare : public DSSRWCompare
{
public:
	DSSRWGroupbyCompare(){mCompareType = eGroupbyCompare;}
	
    int Init(DSSRWGroupbyIterator* pGroupbyIterator, bool iDisplayNullOnTop);
    
    //junwang, new rwd, 12/26/2011
    virtual bool operator() (int row1, int row2);
};

class DSSRWDDICCompare : public DSSRWCompare
{
public:
    DSSRWDDICCompare(){mCompareType = eDDICCompare; mbUseBrowserSort = true;}
    
    int Init(DSSRWDDICInputControlIterator *pDDICIterator, bool iDisplayNullOnTop, DSSObjectInfo *iObjInfo = NULL, bool ibUseBrowserSort = true);
    virtual bool operator() (int row1, int row2);
    bool mbUseBrowserSort;
};

//tliao, 06/13/2014, 898249, refresh NGBPathInfo after applying delta and view evaluation
class DSSNGBElementCompare : public DSSRWCompare
{
public:
	int Init(DSSRWDataModel* ipDataMode, bool iDisplayNullOnTop, string iNodeKey);
	bool operator() (Int32 row1, Int32 row2);
};


#endif