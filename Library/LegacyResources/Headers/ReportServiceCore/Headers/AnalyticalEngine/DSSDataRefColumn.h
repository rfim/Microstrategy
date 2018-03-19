/*
 *  DSSDataRefColumn.h
 *  ReportServiceCore
 *
 *  Created by Xiaoqi Zeng on 9/22/11.
 *  Copyright 2011 MicroStrategy. All rights reserved.
 *
 */

#pragma once

#import "DSSDataColumn.h"
#import "DSSDataPartialColumn.h"

class DSSBinaryDataReader;


class DSSDataRefColumn : public DSSDataColumn
{
public:
	
	DSSDataRefColumn();//This RowNum is the total RowCount of GlobalLookUpTable
	~DSSDataRefColumn();
    
    
	virtual int Init(EnumDSSDataType iDataType, long long iRowCount, bool ibUseMappedFile = false);
    
	int getDataC(int iRow, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag);
    
	/*bool ExistRow(int iRow);
     
     int AddLocalColumn(int iGlobalIndex, DSSDataColumn *ipData);
     
     int FillMap(int iGlobalIndex, int iLocalColumn);
     */
    void setRowSize(int iRowSize);
    
    int LoadDataFromBinary(DSSBinaryDataReader *ipReadStream, bool iIsFirstTimeLoad);
    int PartialLoadDataFromBinary(DSSBinaryDataReader *ipReadStream);
    int FirstLoadDataFromBinary(DSSBinaryDataReader *ipReadStream);
    
    
	virtual long long getRowCount();
    
    virtual int get_RowCount(int* opRowCount);
	
	virtual int getSizeOfData() ;
    
	virtual int getRowSize(long long iRow, int &orSize) ;
	
	virtual int setData(long long iRow, char *ipData, int iSizeInByte, DSSDataFlag iFlag) ;
	
	virtual int getData(long long iRow, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag) ;
	
	virtual int getDataCopy(long long iRow, char *pBuffer, int iBufSize, int &orCopiedSize, EnumDSSDataType &orType, DSSDataFlag &orFlag) ;
	
	virtual int setDataInBulk(long long iStart, long long iEnd, char *ipData, int iSizeInByte, DSSDataFlag *ipFlags, int iSIBFlags) ;
	
	virtual int getNumericVal(long long iRow, double &orVal, DSSDataFlag &orFlag) ;
	
	virtual int setNumeric(long long iRow, double iVal, DSSDataFlag iFlag) ;
	
	virtual int getFlag(long long iRow, DSSDataFlag &orFlag) ;
	
	virtual int setFlag(long long iRow, DSSDataFlag Flag) ;
	
	virtual int setFlags(long long iStart, long long iEnd, DSSDataFlag *ipFlags, int iSizeInByte) ;
	
	virtual int setFlags(long long iStart, long long iEnd, DSSDataFlag *ipFlags, int* iElementList, int iElementSize) ;
	
	virtual int Order(long long iRow1, long long iRow2) ;
    
    virtual int Order(long long iRow1, long long iRow2,bool fIsDisplayNullOnTop);
    
    virtual int CompareToData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags, bool iIsNullDisplayOnTop, bool ibSimpleComparison);
    
	virtual int CreatePartition();
	
	virtual void RecycleMemory();
    
    inline bool IsCompatible() {return mfCompatible;}
    
    int SaveDataToSingleColumn(DSSDataColumn* ipColumn);
    
    bool IsRefColumn();
	
    virtual int reserveMemForStrData();
    
    virtual int createDataFlag(EnumDSSDataFlag iFlag);
    
    virtual int createMMapMemory(const char * ipFilePath, bool ibUseNewFile);
	virtual int MapFileBuffer();
    virtual int UnMapFileBuffer();
    virtual int ExtendColumn(int iRows);
    virtual int Remove(int iRow);
    virtual int Insert(int iRow);
    
    virtual int syncDataTypeWithDataColumn();
    
protected:
	
	// Compare the values in two rows. The two row indices are already validated and their
	// data status are ok.
	// weiwang 2009-05-19 Add support for string comparison without locale.
	virtual int hCompareRow(long long iRow1, long long iRow2, bool iIsNullDisplayOnTop,int iLocale=0, bool ibSimpleComparison = false);
	
	// Compare an internal data (indicated by row number) with and external data
	// weiwang 2009-05-19 Add support for string comparison without locale.
	virtual int hCompareData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags, bool iIsNullDisplayOnTop,bool ibSimpleComparison = false);
    
    int hFindColIndex(long long iRow);
    
    //ctang: 2012-7-24
    bool mfCompatible;
	
private:
	
	/*vector< int > mvMapGlobalIndexToLocalColumn;
     vector< DSSDataColumn* > mvpLocalColumn;
     vector< int > mvLocalColumnGlobalStart;*/
    
    vector<DSSDataPartialColumn*> mvColumnList;
    vector<int> mvStartGlobalKey;
    
    int mMaxRowSize;
    
};
