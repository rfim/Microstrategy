/*
 *  DSSDataColumn.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#include "DSSrwdconstants.h"
#import "DSSAEHelper.h"

#include "DSSXTabBitVector.h"

// aye, 2006-11-09
// use a data structure to represent the data contained in each cell of the column
typedef struct {
	unsigned char* mpData;	// pointer to the memory location to store the data
	double mData;			// if the real data is integer, while type is double, use this member to hold it
	Int32 mSize;			// the actual number of meaningful bytes in the data
	DSSDataType_Type mDataType;	// data type of the data
	DSSDataFlag mStatus;	// the status of the data
} XTabCellData;


//typedef enum {XTabColumnSortUnknown = 0, XTabColumnSorted = 1, XTabColumnNotSorted = 2} XTabColumnSortStatus;
#define COL_PRTTN_BIT			9
#define COLUMN_PARTITION_SIZE	512	//size = 2^COL_PRTTN_BIT

class DSSDataColumn : public IStatisticsInfo
{
public:
	DSSDataColumn();
	virtual ~DSSDataColumn();
    
	virtual int Init(EnumDSSDataType iDataType, long long iRowCount, bool ibUseMappedFile = false) = 0;
	
	EnumDSSDataType getDataType();
	
	virtual long long getRowCount() = 0;
	
    virtual int get_RowCount(int* opRowCount) = 0;
    
	virtual int getSizeOfData() = 0;
	
	virtual int getRowSize(long long iRow, int &orSize) = 0;
	
	virtual int setData(long long iRow, char *ipData, int iSizeInByte, DSSDataFlag iFlag) = 0;
	virtual int SetData(long long iRow, unsigned char *ipData, int iSizeInByte, EnumDSSDataType iType, DSSDataFlag iFlag);
	
	virtual int getData(long long iRow, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag) = 0;
	
	virtual int getDataCopy(long long iRow, char *pBuffer, int iBufSize, int &orCopiedSize, EnumDSSDataType &orType, DSSDataFlag &orFlag) = 0;
	
	virtual int setDataInBulk(long long iStart, long long iEnd, char *ipData, int iSizeInByte, DSSDataFlag *ipFlags, int iSIBFlags) = 0;
	
	virtual int getNumericVal(long long iRow, double &orVal, DSSDataFlag &orFlag) = 0;
	
	virtual int setNumeric(long long iRow, double iVal, DSSDataFlag iFlag) = 0;
	
	virtual int getFlag(long long iRow, DSSDataFlag &orFlag) = 0;
	
	virtual int setFlag(long long iRow, DSSDataFlag Flag) = 0;
	
	virtual int setFlags(long long iStart, long long iEnd, DSSDataFlag *ipFlags, int iSizeInByte) = 0;
	
	virtual int setFlags(long long iStart, long long iEnd, DSSDataFlag *ipFlags, int* iElementList, int iElementSize) = 0;
	
	virtual int Order(long long iRow1, long long iRow2) = 0;
    
    virtual int Order(long long iRow1, long long iRow2,bool fIsDisplayNullOnTop) = 0;
    
	//		int Order(int iRow1, int iRow2, bool fIsNullDisplayOnTop, int Locale, bool ibSimpleComparison = false);
	inline int Compare(long long iRow1, long long iRow2, bool fIsNullDisplayOnTop, int Locale, 
                       bool ibSimpleComparison = false, bool ibCaseSensitive = true);
	
	virtual int CompareToData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags, 
                              bool iIsNullDisplayOnTop = true, bool ibSimpleComparison = true);
    virtual int CompareToData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags,
                              bool iIsNullDisplayOnTop, bool ibSimpleComparison, bool ibCaseSensitive);
	
	int getAllDataValidFlag(bool &orAllDataValid);
	
	int setAllDataValidFlag(bool iAllDataValid);
	
	virtual int getRawDataAndFlags(void** opData, DSSDataFlag** opDataFlags);
	
	virtual int CreatePartition() = 0;
    
	virtual int size();
	
	virtual void RecycleMemory() = 0;

    virtual int reserveMemForStrData() = 0;

    virtual int createDataFlag(EnumDSSDataFlag iFlag) = 0;
    virtual bool IsRefColumn() = 0;

    virtual int ExtendColumn(int inRow) = 0;
    
    virtual int Remove(int iRow) = 0;
    virtual int Insert(int iRow) = 0;
    
    //junwang 09/01/2011 Red Rocket: issue 500695
    int SetInputOrder(int inRowCount, int* ipInputOrder);
    int* GetInputOrder();
    int getInputOrderCnt();
    
    //to support mapped file
    virtual int createMMapMemory(const char * ipFilePath, bool ibUseNewFile) = 0;
	virtual int MapFileBuffer() = 0;
    virtual int UnMapFileBuffer() = 0;
    //ctang 2012-7-24
    inline void setJoinNULLFlag(bool iFlag) {mfHasJoinNULL = iFlag;}
    inline bool HasJoinNULL() {return mfHasJoinNULL;}
    
    
    
    // 5/17/2012 tehe: support deley loading 
    inline void setDataBeginPosition(int iBinaryOffset) { mDataBeginPosition = iBinaryOffset; }
    inline int getDataBeginPostion() { return mDataBeginPosition; }
    
    inline void setDataEndPosition(int iBinaryOffset) { mDataEndPosition = iBinaryOffset; }
    inline int getDataEndPostion() { return mDataEndPosition; }
    
    inline void setFlagBeginPosition(int iFlagPosition) { mFlagBeginPosition = iFlagPosition; }
    inline int getFlagBeginPosition() {return mFlagBeginPosition; }
    
    // 12/18/2012 tehe #696636
    virtual int setTrimFlag(bool ifTrimString) { return S_OK; }
    virtual int getTrimFlag(bool *ofTrimString) { *ofTrimString = false; return S_OK; }
    
    inline int getCellData(int iRow, XTabCellData& orData);
    
    inline int* GetSortOrder() {return mpSortOrder;}
    
    virtual EnumDSSDataType getOriginalType() {return mOriginalDataType;}
    virtual void setOriginalType(EnumDSSDataType iType) { mOriginalDataType = iType; }
    
    virtual double getDivider() { return mDivider; }
    virtual void setDivider(double iDivider) {mDivider = iDivider; }
    
    virtual int GetRange(CComVariant* pMin, CComVariant* pMax, DSSXTabBitVector* ipBV = NULL);
protected:
	
	// Compare the values in two rows. The two row indices are already validated and their
	// data status are ok.
    // weiwang 2009-05-19 Add support for string comparison without locale.
    virtual int hCompareRow(long long iRow1, long long iRow2, bool iIsNullDisplayOnTop,int iLocale=0, bool ibSimpleComparison = false) = 0;
    // jzeng 2015-07-16 Add support for string comparison case sensitive. If subclass want compare Row with case sensitive, it must override this API.
    virtual int hCompareRow(long long iRow1, long long iRow2, bool iIsNullDisplayOnTop,int iLocale, bool ibSimpleComparison, bool ibCaseSensitive);
	
	// Compare an internal data (indicated by row number) with and external data
    // weiwang 2009-05-19 Add support for string comparison without locale.
    virtual int hCompareData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags, bool iIsNullDisplayOnTop, bool ibSimpleComparison = false) = 0;
    // jzeng 2015-07-16 Add support for string comparison case sensitive. If subclass want compare Data with case sensitive, it must override this API.
    virtual int hCompareData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags, bool iIsNullDisplayOnTop, bool ibSimpleComparison, bool ibCaseSensitive);
    
	int hCompareFlag(DSSDataFlag iFlags1, DSSDataFlag iFlags2, bool iIsNullDisplayOnTop);
    
    //	int mBaseFormID;
    //	EnumDSSBaseFormType mFormType;
	EnumDSSDataType mDataType;
    
	//XTabColumnSortStatus mSortStatus;
	bool mbSorted;
	int* mpSortOrder;
    
    //junwang 09/01/2011 input order, Red Rocket
    int* mpInputOrder;
    int mInputOrderCnt; //zhyang, 773961
    int *mpInputRank; //zhyang, 801671
    
	long long mRowCount;
	DSSDataFlag *mpFlags;
	bool mbAllDataValid;
	
	bool mbSimpleStrComparison;		//apply locale independent sort if true;
    
    // 5/17/2012 tehe: data position in data stream
    int mDataBeginPosition;  // the start position of a column in binary stream
    int mDataEndPosition;
    int mFlagBeginPosition;  // flag data position of a column in binary stream
    
    //ctang 2012-7-24 flag for new rwd column with join null
    bool mfHasJoinNULL;
    
    EnumDSSDataType mOriginalDataType;
    
    double mDivider;
};

inline int DSSDataColumn::Compare(long long iRow1, long long iRow2, bool fIsNullDisplayOnTop, int Locale, bool ibSimpleComparison, bool ibCaseSensitive)
{
	if (mbSorted)
	{
		if (iRow1 < iRow2)
			return -1;
		else if (iRow1 > iRow2)
			return 1;
		else 
			return 0;		
	}
	
    //zhyang, 02/24/2012, in new rwd, if iRow is equal ot mRowCount, then the row must be a join null
    //need to treat this with the fIsNullDisplayOnTop flag;
    if(iRow1 == mRowCount)
    {
        return fIsNullDisplayOnTop ? -1 : 1;
    }
    if(iRow2 == mRowCount)
    {
        return !fIsNullDisplayOnTop ? -1 : 1;
    }
    
	if(iRow1 >= mRowCount || iRow2 >= mRowCount)
		return iRow1 >= mRowCount ? -1 : 1;
	
    //zhyang, 12/08/2011, we must get the flag using getFlag function due to 
    //DataRefColumn have a different way to get data flag
    DSSDataFlag lFlag1, lFlag2;
    getFlag(iRow1, lFlag1);
    getFlag(iRow2, lFlag2);
	/*
    if (lFlag1 == DssDataOk && lFlag2 == DssDataOk)
		return hCompareRow(iRow1, iRow2, Locale, ibSimpleComparison);
	else
		return hCompareFlag(lFlag1, lFlag2, fIsNullDisplayOnTop);
		*/
	return hCompareRow(iRow1, iRow2,fIsNullDisplayOnTop, Locale , ibSimpleComparison, ibCaseSensitive);
    /*
	if (mpFlags == NULL || (mpFlags[iRow1] == DssDataOk && mpFlags[iRow2] == DssDataOk))
		return hCompareRow(iRow1, iRow2, Locale, ibSimpleComparison);
	else
		return hCompareFlag(mpFlags[iRow1], mpFlags[iRow2], fIsNullDisplayOnTop);	
    */	
}

inline int DSSDataColumn::hCompareFlag(DSSDataFlag iFlags1, DSSDataFlag iFlags2, bool iIsNullDisplayOnTop)
{	
	// We consider DssDataOk is greater than other flags. All other flags (null and missing) are equal.	
	int lRank1 = (iFlags1 == DssDataOk) ? 1 : 0;
	int lRank2 = (iFlags2 == DssDataOk) ? 1 : 0;
	return (iIsNullDisplayOnTop)? lRank1 - lRank2 : lRank2 - lRank1;
}

//junwang 09/01/2011 Red Rocket: issue 500695
inline int* DSSDataColumn::GetInputOrder()
{
    return mpInputOrder;
}

//zhyang, 773961
inline int DSSDataColumn::getInputOrderCnt()
{
    return mInputOrderCnt;
}

inline int DSSDataColumn::getCellData(int iRow, XTabCellData& orData)
{
    return getData(iRow, (const char**)(&orData.mpData), orData.mSize, orData.mDataType, orData.mStatus);
}



