//
//  DSSDataPartialColumn.h
//  ReportServiceCore
//
//  Created by Tang, Cheng on 5/18/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSDataPartialColumn_h
#define ReportServiceCore_DSSDataPartialColumn_h


#import "DSSDataColumn.h"

#include "DSSDataHolder.h"

class DSSDataPartialColumn : public DSSDataColumn
{
public:
	DSSDataPartialColumn();
	virtual ~DSSDataPartialColumn();
    
    int LoadDataFromBinary(DSSBinaryDataReader *ipReadStream, bool iIsFirstTimeLoad);
    
    int PartialLoadDataFromBinary(DSSBinaryDataReader *ipReadStream);
    
    int FirstLoadDataFromBinary(DSSBinaryDataReader *ipReadStream);
    
    
	int Init(EnumDSSDataType iDataType, long long iRowCount, bool ibUseMappedFile = false);
	
	long long getRowCount();
	
    int get_RowCount(int* opRowCount);
    
	int setDataInBulk(long long iStart, long long iEnd, char *ipData, int iSizeInByte, DSSDataFlag *ipFlags, int iSIBFlags);
	
	int getNumericVal(long long iRow, double &orVal, DSSDataFlag &orFlag);
	
	int setNumeric(long long iRow, double iVal, DSSDataFlag iFlag);
	
	int getFlag(long long iRow, DSSDataFlag &orFlag);
	
	int setFlag(long long iRow, DSSDataFlag Flag);
	
	int setFlags(long long iStart, long long iEnd, DSSDataFlag *ipFlags, int iSizeInByte);
	
	int setFlags(long long iStart, long long iEnd, DSSDataFlag *ipFlags, int* iElementList, int iElementSize);
    
    int size();
    
    int CompareToData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags, bool iIsNullDisplayOnTop, bool ibSimpleComparison);
	
	virtual int CreatePartition();
    
	virtual int getRawDataAndFlags(void** opData, DSSDataFlag** opDataFlags);
    
	virtual void RecycleMemory();
    
	virtual int Order(long long iRow1, long long iRow2);
    
    //TQMS 696592, hyan, 13/12/2012, we need add the nullontop argument
    virtual int Order(long long iRow1, long long iRow2,bool fIsDisplayNullOnTop);
    
    virtual int getSizeOfData() = 0;
	
	virtual int getRowSize(long long iRow, int &orSize) = 0;
	
	virtual int setData(long long iRow, char *ipData, int iSizeInByte, DSSDataFlag iFlag) = 0;
	
	virtual int getData(long long iRow, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag) = 0;
	
	virtual int getDataCopy(long long iRow, char *pBuffer, int iBufSize, int &orCopiedSize, EnumDSSDataType &orType, DSSDataFlag &orFlag) = 0;
    
    bool IsRefColumn();
    
    int SaveToSingleColumn(DSSDataColumn* ipColumn, int iOffset);
	
    virtual int reserveMemForStrData();
    
    virtual int createDataFlag(EnumDSSDataFlag iFlag);
	
    virtual int createMMapMemory(const char * ipFilePath, bool ibUseNewFile);
	virtual int MapFileBuffer();
    virtual int UnMapFileBuffer();
    
    virtual int ExtendColumn(int iRows);
    virtual int Remove(int iRow);
    virtual int Insert(int iRow);

protected:
	int hCompareRow(long long iRow1, long long iRow2, bool iIsNullDisplayOnTop,int iLocale=0,  bool ibSimpleComparison = false);
	virtual int hCompareData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags, bool iIsNullDisplayOnTop, bool ibSimpleComparison = false);
    
    virtual int hLoadData(DSSBinaryDataReader *ipReadStream, DSSDataHolder* ipHolder, EnumDSSDataType iDataType, int nElement) = 0;
    
    
	
	vector<DSSDataHolder*> mvDataList;
    vector<int> mvStartKey;
    vector<int> mvGlobalKeyToListOffset;
};


#endif
