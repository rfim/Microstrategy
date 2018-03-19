/*
 *  DSSCubeDataSlice.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import "DSSrwdconstants.h"
#import "Base/Defines/BasicTypes.h"

class DSSTabularIndex;
class DSSDataColumn;
class DSSXTabBitVector;
class DSSTabularData;
class DSSCube;
class MCESchema;
class DFCSimpleExpr;

#include <map>
#include <vector>

typedef std::map<int, int> IntToIntMap;

class DSSCubeDataSlice
{
public:
	DSSCubeDataSlice();
	virtual ~DSSCubeDataSlice();
	
	int Init(DSSDataColumn *ipColumn, DSSTabularIndex *ipIndices, bool ibColumnShared = false);
    
    int SetMetricPosInfo(bool ifCreate, int iMetricPos);
	
	inline DSSTabularIndex *getIndices(){return mpIndex;};
	inline int get_IndicesC(DSSTabularIndex **oppIndex){*oppIndex = mpIndex;return 0;};
    
	virtual DSSDataColumn * getColumn();
	virtual int get_ColumnC(DSSDataColumn **oppColumn, bool iCreate = true);
    virtual int get_RefColumnC(DSSDataColumn **oppColumn);
    
	virtual EnumDSSDataType getDataType();
	
	virtual int getSizeOfData(int &orRowSize);
	virtual int getFlag(int nKey, int *ipKey, DSSDataFlag &orFlag);
	
	virtual int setVal(int nKey, int *ipKey, char *ipData, int iSizeInByte, EnumDSSDataType iType, DSSDataFlag iFlag);
	virtual int getVal(int nKey, int *ipKey, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag, int *opThresholdID = NULL);
	virtual int setValByOffset(int iOffset, char *ipData, int iSizeInByte, EnumDSSDataType iType, DSSDataFlag iFlag);
	virtual int getValByOffset(int iOffset, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag, int *opThresholdID);
	//int getValCopy(int nKey, int *ipKey, char *pBuffer, int iBufSize, int &orCopiedSize, EnumDSSDataType &orType, DSSDataFlag &orFlag);
	
	virtual int getNumeric(int nKey, int *ipKey, double &orVal, DSSDataFlag &orFlag, int *opThresholdID = NULL);
	virtual int getNumericByOffset(int iOffset, double &orVal, DSSDataFlag &orFlag, int *opThresholdID = NULL);
	
	virtual int setNumericByOffset(int iOffset, double &irVal, DSSDataFlag &irFlag);
	
	virtual int setValueAndThreshold(int nKey, int *ipKey, char *ipData, int iSizeInByte, EnumDSSDataType iType, DSSDataFlag iFlag, int iThresholdID);
	
	virtual int setValueAndThresholdByOffset(long long offset, char *ipData, int iSizeInByte, EnumDSSDataType iType, DSSDataFlag iFlag, int iThresholdID);
    
	virtual int updateValueWithString(int offset, char *ipString, int iStrLength);
    
	virtual int size();
	
	virtual void RecycleMemory();
    
    //zhyang
    virtual int SetThreshold(DSSDataColumn *lpThresholdColumn);
	
	virtual int CopyThreshold(DSSCubeDataSlice *ipSourceDataSlice);
    
    int setThresholdSlice(DSSCubeDataSlice *ipThreshholdSlice);
    
    bool IsConstantSlice(){return mSliceType == 1;}
    
    void setConstantSlice(){mSliceType = 1;}
    
    virtual int setCondition(DFCSimpleExpr* ipCondition, DSSXTabBitVector* ipMask);
    
    virtual int Populate (DSSCubeDataSlice *pOriginalSlice,
                          DSSTabularData *ipTabularData,
                          Int32 *pUnitsMap = NULL,
                          std::vector<Int32> *ipConvertedAttributes = NULL,
                          //ICDSSSource *ipSource, 
                          bool iRestoreDouble = false,
                          Int32** ippGToLMap1 = NULL,
                          IntToIntMap** ippGToLMap2 = NULL,
                          MCESchema * ipSchema = NULL);
	inline DSSXTabBitVector* GetMask() {return mpMask;};
    inline DFCSimpleExpr* GetCondition() {return mpCondition;};

    void clearIndex(){mpIndex = NULL;}
    int AttrFromInit(DSSTabularData *ipTabularData, Int32 iUnit, Int32 iOffset, bool iUseLookupForAttributes, vector<int>* ipLocalToGlobalMap);
    inline int GetRefUnitPos() {return mUnitPos;}
    bool IsSFBSlice();
    int setSFBFlag(bool iSFBFlag);
    
    bool IsConflict(){return false;};
    
    int CompleteCondition();
    
    int get_MetricValueRange(VARIANT * pMin, VARIANT * pMax);

    int put_MetricValueRange(VARIANT * pMin, VARIANT * pMax);
    
    int get_OriginalRowCount(Int32* pRowCount);
    
    int put_OriginalRowCount(Int32 nRowCount);
    
    int CalculateRange(DSSTabularData* ipTabularData = NULL);
private:
    int hFindUnitPosition(DSSTabularIndex* ipIndex, Int32 iUnit, Int32 &orUnitPos);
    int hCreateColumnFromRefColumn();
    
	DSSTabularIndex *mpIndex;
	DSSDataColumn *mpColumn;
    bool mbColumnIsShared;
	unsigned char *mpThresholdID;
    int mSliceType;
    
    //ctang: add two memebers. they are needed in Flex, maybe also needed in IPAD in the future.
    bool mfCreate;//true means it is a calculated slice, false means it is a original slice
    int mMetricPos;//when mfCreate is true, it means the offset of the metric in tabular data
    
    DSSXTabBitVector *mpMask;
    DFCSimpleExpr *mpCondition;

    // qjiang 06/08/2009 TQMS 348157, fali, TQMS 810082
    // for view filter operations, we use the column in lookup table to do binary search instead of the column from index as it may be not sorted.
    // add member mpRefColumn to remember it, it is only assigned in CDSSMetricSlice::AttrFromInit
    DSSDataColumn *mpRefColumn;

    int mUnitPos;// mpColumn unit position in the mpIndex
    bool mfSFBFlag; //indicate whether it is a SFB slice
	std::vector<int>* mpLocalToGlobalMap; //used only for property slice
    
    CComVariant mMinValue;
    CComVariant mMaxValue;
    int mOrigRowCount;
};
