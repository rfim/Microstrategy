/*
 *  DSSAEHelper.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/30/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#ifndef DSSAEHelper_H
#define DSSAEHelper_H  

#import <vector>
#import <set>
#import <map>
#import "DSSBigDecimal.h"
#import "DSSBigDecimal.h"
#import "DSSDateTime.h"
#import "DSSCellFmtData.h"
#import "DSSBaseString.h"
#import "DSSrwdconstants.h"
#import "GUID.h"
#import "Base/Defines/Asserte.h"
//#import "DSSRWDataModel.h"

#include "Base/Base/Allocator.h"

//#include "DSSFormat.h"

#import "VariantCommonInclude.h"
#include "DSSCOMMaster/DSSCOM_TLB_INC.H"


#define CHECK_HR  {if (hr != S_OK) return hr;}
#define CHECK_PTR(p) {if (!p) return E_POINTER;}
#define CHECK_NEWED_PTR(p) {if (!p) return E_OUT_OF_MEMORY;}
#define CHECK_HR_PTR(ptr) { if (hr != S_OK) return hr; if (!ptr) return hr = E_POINTER; }
#define AE_ASSERT(expr) {_ASSERT(expr); if (!(expr)) return E_ASSERT;}

//#define DSSXTAB_TOTAL_KEY -100

using namespace std;

class DSSDataColumn;
class DSSBaseElementProxy;
class DSSTemplate;
class DSSCalculationPlan;
class DSSViewDataSet;
class DSSTemplateUnit;
class DSSGUnitContainer;
class DSSObjectInfo;
class DSSDataPartialColumn;
class DSSDataHolder;
class DSSTabularAttribute;
class AERawData;
class DSSGlobalLookUpTable;
class DSSCube;
class DSSTabularData;
class DSSRWDataModel;
class DSSFormat;
typedef GUID DSS_ID;
class DFCCSITransactionBase;

struct METRIC_ID
{
	DSS_ID mDatasetID;
	DSS_ID mMetricID;
};

struct MetricIDCompare
{
	bool operator() (const METRIC_ID& metric1, const METRIC_ID& metric2) const
	{
		Int32 lCompResult = memcmp(&(metric1.mDatasetID), &(metric2.mDatasetID), sizeof(DSS_ID));
        
		if (lCompResult < 0)
			return true;
		else if (lCompResult > 0)
			return false;
		else
			return memcmp(&(metric1.mMetricID), &(metric2.mMetricID), sizeof(DSS_ID))<0;
	}
};

struct UnitFormInfo  //for attribute subsetting selection
{
	Int32 UnitID;
	Int32 nForms;
	Int32 *Forms;
	Int32 *Locales;
    
	bool mfSelectAll;
    
	bool mfNoChildPopulate;//ctang 2011-4-26 indicate that we need populate the child or not. only meaningful when create RefCS
    
	bool mfLocalPopulate;//ctang 2011-4-26 for the fake CG population in extension
	UnitFormInfo() : UnitID(-1), nForms(0), Forms(NULL), Locales(NULL), mfSelectAll(false), mfNoChildPopulate(false), mfLocalPopulate(false) {};
    
    bool operator <(const UnitFormInfo &iInfo) const {return UnitID < iInfo.UnitID;}
};
class DSSDataModelXTabImpl;


class IStatisticsInfo
{
public:
    virtual int size() = 0;
};

class IXTABAttributeInfo
{
public:
    virtual int getArity(){return 0;}
    virtual int Count(){return 0;}
    virtual int Item(int iForm, DSSDataColumn **oppColumn) {return 0;}
    virtual int getWHNullPosition() {return 0;}
    virtual void setWHNullPosition(int iPosition){}
    virtual int getBaseFormID(int iForm){return 0;}
    //to support transaction data
    virtual int setData(int iBaseFormID, int iRow, char *ipData, int iSizeInByte){return 0;}
    virtual int setData(int iBaseFormID, int iOffset, MBase::CString &irText) {return 0;}
    virtual int CountRows(){return 0;}
    virtual int FindFormWithLocale(int iAttribute, int iBaseFormID, int iLocale, int *opColumn) {return 0;}
    virtual int FindFormWithLocale(int iAttribute, int iBaseFormID, int iLocale, int &orColumn) {return 0;}
};

typedef	enum EnumXTabStatus
{
	XTAB_UNINITIALIZED = 0,
	XTAB_INITIALIZED,
    XTAB_UNDATALOADED,  // 11/26/2012 tehe: ensure model hasn't been called.
    XTAB_DATALOADFAIL   // tehe: fail to load data from binary
} EnumXTabStatus;

// 11/23/2012 tehe
typedef enum EnumCMFolderType
{
    CMFolderTypeDocument = 0,
    CMFolderTypeLibraryCache,
    CMFolderTypeTemp
}EnumCMFolderType;


typedef enum EnumCMBlockType
{
    cmBlockTypeUnknow = 0x1,
    cmDocumentInfo = 0x2, // current we will only save doc create time in this block
    cmOriginalIndexPoolInfo = 0x4,
    cmOriginalIndexPool = 0x8,
    cmTabularIndex = 0x10,        // key table for large index
    cmTabularIndicesInfo = 0x20,            // this block contains index info for all large indices
    cmMaxKeyDistribution = 0x40, // max key, key distribution for large index
    cmNewTabularIndex = 0x80,
    cmNewTabularIndicesInfo = 0x100,
    cmElementIndex = 0x200,
    cmElementIndicesInfos = 0x400,
    cmXTabHashTable = 0x800
    
}EnumCMBlockType;

typedef struct CMBlockHeader
{
    short mBlockType;
    long mBlockSize;   //  block data size
    long mBlockOffset; // block offset means the begin position of block data in file
}CMBlockHeader;

typedef enum EnumCMStatus
{
    cmStatusDisable = 0,    //  local cache is not supported
    cmStatusApplied,        // load from local cache
    cmStatusNotApplied      // support local cache, but binary compare failure or support local cache, but cache file did not exist before.
}EnumCMStatus;

class DSSAEHelper
{
public:
    static bool IsNumeric(EnumDSSDataType Type);
	static bool IsString(EnumDSSDataType Type); 
    
    //hyan,2012/05/21
    static bool IsString2(EnumDSSDataType Type);
    
    
    static bool IsDate(EnumDSSDataType Type);
    static bool IsBool(EnumDSSDataType Type);   
	static bool IsBigDecimal(EnumDSSDataType Type);    
	static bool IsCellFmtData(EnumDSSDataType Type);
	static bool IsBinary(EnumDSSDataType Type);
    
    static double NumericValue(EnumDSSDataType Type, void *data);
    
	static int CompareStrings(EnumDSSDataType Type1, char * string1, int size1, EnumDSSDataType Type2, char * string2, int size2, int locale=0);
	static int CompareDates(EnumDSSDataType Type1, void *data1, EnumDSSDataType Type2, void *data2);

    static int CompareData(EnumDSSDataType Type1, void *data1, Int32 iSize1, EnumDSSDataType Type2, void *data2, Int32 iSize2, Int32 locale=0);

    
	static int ConvertStringToVariant();
	static int GenXTabCol(EnumDSSDataType iType, long long nRows, DSSDataColumn **oppColumn, bool ibMetric = false, bool ibHasXTabNull = false, bool ibUseMappedFile = false);
    
    static int GenPartialCol(EnumDSSDataType iType, int nRows, DSSDataPartialColumn **oppColumn);
    //ctang: for partial column
    static int GenXTabDataHolder(EnumDSSDataType iType, int nRows, DSSDataHolder **oppHolder);
    
	static int CompareStringsWithLocale(const wchar_t * string1, const wchar_t * string2, int size1, int size2, int locale);
	static int CompareStringsWithLocale(const char * string1, const char * string2, int size1, int size2, int locale, bool ibSimpleComparison = true);
	static int ConvertStringToData(const char *ipString, int iDataType, void *pDataBuffer, int iBufferSize, int &orDataSize);
	static int ConvertToString(const void *ipData, int iSizeInByte, int iDataType, int iFlag, std::string &orStr, const char * ipDateFormat = NULL);
	static bool IsStringColumn(DSSDataColumn *ipColumn);
	static EnumDSSTemplateUnitType ConvertObject2UnitType(EnumDSSObjectType objectType);
	static int ConvertToNumeric(const void *ipData, int iSizeInByte, int iDataType, int iFlag, double &orNumericVal);
    
	//calculate the memory size of a vector,
	//including the vector itself, and the object/pointer every element is pointing to
	template<class T>
	static int SizeOfVector(std::vector<T> &irList);
	
	template<class T>
	static int SizeOfPVector(const std::vector<T*> &irList, size_t factor = 1);
    
	template<class T>
	static int SizeOfVoVector(std::vector<std::vector<T> > &irList);
	
	static int SizeOfStrings(const std::vector<MBase::CString> &irList);
	
	static int SizeOfBaseElement(DSSBaseElementProxy *ipElement);
    
	static MBase::CString Guid2String(GUID &irGUID);
    //junwang, 01/05/2012
    static MBase::CString MGuid2String(MBase::GUID &irGUID);
	
	static int testDebug();
	static void printGUID(GUID &irGUID);
    static void printCSI(DFCCSITransactionBase *ipCSI);
    
    static int SizeOfType(EnumDSSDataType iType, Int32 *opSize);    // jimzhang July/01/2011
    
	static int mVal;
    
    static int ConvertTargetLevel(int iLevel,
                                  DSSTemplate *ipTemplate,
                                  DSSDataModelXTabImpl *ipModel,
                                  DSSCalculationPlan *ipCalculationPlan,
                                  bool ifPreCons,
                                  vector<int> *ivUnits,
                                  int lCurEvent);
	
    // tehe 11/23/2011
    static int FindOriginalLevel(int iCurEvent,
                      DSSTemplate *ipTemplate,
//                      DSSViewDataSet *ipViewDataSet,
                      DSSCube *ipCube,
                      DSSCalculationPlan *ipCalculationPlan,
                      vector<int, MBase::Allocator<int> > *opvUnits);
    
    // Output parameter: lvUnits, initialized before the function is called
    // The element type of the vector is long, no MBase::Buffer used in the vector
    static int ConvertTargetLevel(int iLevel,
                       DSSTemplate *ipTemplate,
                       DSSTabularData *ipTabularData,
                       DSSCalculationPlan *ipCalculationPlan,
                       bool ifPreCons,
                       vector<int> *ivUnits,
                       int lCurEvent);
    
    static int IsDerivedElementUnit(DSSTemplate *ipTemplate, DSSTemplateUnit *ipTemplateUnit, DSSTabularData* ipTabularData, bool *opFlag);
    
    //junwang, 01/11/2012, query display text from specified tabular unit by global keys
    static int GetDisplayTextsForTabularUnit(DSSGUnitContainer* ipUnitContainer, DSSObjectInfo* ipObjInfo,
                                             GUID& iUnitGUID, const vector<int>& iGlobalKeys, vector<string>& ovTexts, const vector<int>& iDisplayForms, string &iDelimeter,
                                             DSSFormat *ipFormat=NULL,DSSTabularAttribute *ipTabularAttribute=NULL,DSSRWDataModel* ipDataModel=NULL);
    
    //zhyang
    static int GetDisplayTextsForTabularUnitForDDIC(DSSTabularAttribute *ipTabularAttribute, DSSObjectInfo *ipObjInfo, GUID &iUnitGUID, const vector<int> &iGlobalKeys, vector<string> &ovTexts,
                                             const vector<int>& ivDisplayForms, string &iAttributeFormSeperator, DSSFormat *ipFormat, DSSRWDataModel* ipDataModel, DSSGUnitContainer *ipUnitContainer);
        
    static std::string VariantToString(const CComVariant& irVariant);
    
    static int VariantToRawData(CComVariant &irVariant, char *&orpData, int &orDataSize, EnumDSSDataType &orDataType);
    
    
    //hyan,2012/07/25
    static int ConvertDataFlag(DSSDataFlag iNullFlag);
    
    // jimzhang, Sep/29/2012
    static int ConvertMetricStr2RawData(MBase::CString &irMetricStr, AERawData *opRawData);
    
    // 10/12/2012 tehe 
    static bool IsStringFunction(Int32 iOperatorType);
    static bool ContainsStringCmpOption(Int32 iOperatorType);
    
    
    //9/1/2013,hyan
    static int convertDEOnViewToLeafElements(int iGlobalKey,DSSGlobalLookUpTable *ipLookUp,std::set<int> &ivLeafElements);
    
    static int convertDEToLeafElements(int iGlobalKey,DSSGlobalLookUpTable *ipLookUp,std::set<int> &ivLeafElements);
    
    static int getDataSizeFromString(std::string irText, EnumDSSDataType iDataType, int* opDataSize);
    
    static bool ValidateNumeric(std::string irText, EnumDSSDataType iDataType);
    static bool ValidateDateTime(std::string irText, EnumDSSDataType iDataType);
    static bool ConvertStringToDataCheck(const char *ipString, EnumDSSDataType iDataType);
    static string getDataTypeName(EnumDSSDataType iDataType);
    
    static bool IsOverflowed(EnumDSSDataType iDataType, double iData);
    /**
     Function to compare two GUIDs
     @return
     0 - irID1 == irID2
     1 - irID1 > irID2
     -1 - irID1 < irID2
     */
    static Int32 InlineCompareGUID(const GUID& irID1, const GUID& irID2)
    {
        const unsigned Int32* p1 = (const unsigned Int32*)&irID1;
        const unsigned Int32* p2 = (const unsigned Int32*)&irID2;
        
        for (Int32 i = 0; i < 4; ++i, ++p1, ++p2)
        {
            if (*p1 < *p2) return -1;
            else if (*p1 > *p2) return 1;
        }
        
        return 0;
    }
    
    //moved to here by xhan 07/05/05
	struct IndirectOrder
	{
		Int32 * mpMap;
		IndirectOrder(Int32 *pMap)
        :mpMap(pMap){};
        
		bool operator ()(Int32 index1, Int32 index2)
		{
			return mpMap[index1] < mpMap[index2];
		}
	};
    
    // 4/19/2016 weilin
    // UTF8 Convert to GBK, return the GBK string to ret_str
    static int UTF8ToGBK(char *src, string& ret_str);
    
    // check whether the GBK string hs chinese words
    static bool HasChineseWord(const string &str);
    
    // Define a new strncmp to deal with sorting
    static int my_strncmp(const char *s1, const char *s2, int size, int locale, bool ibSimpleComparison = true);

    // Define a new strncasecmp to deal with sorting
    static int my_strncasecmp(const char *s1, const char *s2, int size, int locale, bool ibSimpleComparison = true);

    // Define a new strcmp to deal with sorting
    static int my_strcmp(const char *s1, const char *s2, int locale, bool ibSimpleComparison = true);

    // Define a new strcasecmp to deal with sorting
    static int my_strcasecmp(const char *s1, const char *s2, int locale, bool ibSimpleComparison = true);

    // weilin 2016/12/27
    static int compareWithSpecialChar(const char * string1, const char * string2, bool ibCaseSensitive);
    
private:
    static bool IsComparisonOperatorFunction(Int32 iOperatorType);
    
	void Dummy();
    
    static int mIgnoreUTF8Error;
	
	
};

template<class T>
int DSSAEHelper::SizeOfVoVector(std::vector<std::vector<T> > &irList)
{
	int lSize = 0;
	int lCount = irList.size();
	
	//the vector itself
	lSize += sizeof(std::vector<T>) * lCount;
	
	//size of the object it points to
	for (int i = 0; i < lCount; i++)
	{
		lSize += SizeOfVector(irList[i]);
	}
	return lSize;
}

template<class T>
int DSSAEHelper::SizeOfVector(std::vector<T> &irList)
{
	return irList.size() * sizeof(T);
}


template<class T>
int DSSAEHelper::SizeOfPVector(const std::vector<T*> &irList, size_t factor)
{
	int lSize = 0;
	int lCount = irList.size();
    
	//the vector itself
	lSize += sizeof(T*) * lCount;
	
	//size of the object it points to
	for (int i = 0; i < lCount; i++)
	{
		if (irList[i])
			lSize += irList[i]->size() * factor;
	}
	return lSize;
}

class AERawData{
public:	
	char *mValue;
	int mSizeInByte;
	EnumDSSDataType mDataType;
	DSSDataFlag mFlag;
	
	AERawData():mValue(NULL), mSizeInByte(0), mDataType(DssDataTypeReserved), mFlag(DssDataOk), mbHasOwnership(false){};
	
    //yimliu, 2014/05/29, 883809.
    //In previous code, an object can't have ownership unless is assigned by another object.
    //yimliu, 2014/06/20, 883809. Delete this method. It may cause problem.
    /*
     AERawData(bool bHasOwnership):mValue(NULL), mSizeInByte(0), mDataType(DssDataTypeReserved), mFlag(DssDataOk), mbHasOwnership(bHasOwnership){};
     */
    
	AERawData(char *ipValue, int iSizeInByte, EnumDSSDataType iDataType, DSSDataFlag iFlag, bool ibHasOwnership)
    :mValue(ipValue), mSizeInByte(iSizeInByte), mDataType(iDataType), mFlag(iFlag), mbHasOwnership(ibHasOwnership){};
	
	~AERawData()
	{
		hDeleteData();
	};
	
	//in copy constructor and assign operator,
	//ownership shouldn't be copied over
	AERawData(const AERawData &irObj):mValue(irObj.mValue),
    mSizeInByte(irObj.mSizeInByte),
    mDataType(irObj.mDataType),
    mFlag(irObj.mFlag),
    mbHasOwnership(false)
	{};
    
	//assign operator, ownership will not be transfered
	//this object will not own data memory
	AERawData &operator = (const AERawData & irObj)
	{
		if (this == &irObj)
			return (*this);
        
		this->hDeleteData();
		
		mValue = irObj.mValue;
		mSizeInByte = irObj.mSizeInByte;
		mDataType = irObj.mDataType;
		mFlag = irObj.mFlag;
		mbHasOwnership = false;
		return (*this);
	}
	
	//assign raw data from irSrcObj to this object, and transfer mbHasOwnership
	//this object will own the data memory only if irSrcObj owns it before
	void Assign(AERawData & irSrcObj)
	{
		if (this == &irSrcObj)
			return;
		
		(*this) = irSrcObj;
		
		if (irSrcObj.mbHasOwnership)
		{
			//hand over the ownership if the source owns it
			this->mbHasOwnership = true;
			irSrcObj.mbHasOwnership = false;
		}
	}
	
	//assign raw data from this object to irDesObj, owner ship is not transfered
	//irDesObj will always own its data memory by making a seperate copy
	void CopyTo(AERawData & irDesObj)
	{
		if (this == &irDesObj)
			return;
		
		irDesObj = *this;
		return irDesObj.TakeOwnership();
	}

	//make a local copy of data and take ownership
	void TakeOwnership();
    
    //use this API with caution, unsuitable use will cause memory leak
    void SetOwnership(bool iOwnership){mbHasOwnership = iOwnership;}
	
private:
	bool mbHasOwnership;
	void hDeleteData();
};

#endif
