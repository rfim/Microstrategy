/*
 *  DSSDataColumnSimple.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 12/29/09.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */

#import "DSSDataColumn.h"

template<class T>
class DSSDataColumnSimple : public DSSDataColumn
{
public:
	DSSDataColumnSimple();
	virtual ~DSSDataColumnSimple();

	int Init(EnumDSSDataType iDataType, long long iRowCount, bool ibUseMappedFile = false);
	
	long long getRowCount();
	
    int get_RowCount(int* opRowCount);
    
	int getSizeOfData();
	
	int getRowSize(long long iRow, int &orSize);
	
	int setData(long long iRow, char *ipData, int iSizeInByte, DSSDataFlag iFlag);
	
	int getData(long long iRow, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag);
	
	int getDataCopy(long long iRow, char *pBuffer, int iBufSize, int &orCopiedSize, EnumDSSDataType &orType, DSSDataFlag &orFlag);
	
	int setDataInBulk(long long iStart, long long iEnd, char *ipData, int iSizeInByte, DSSDataFlag *ipFlags, int iSIBFlags);
	
	int getNumericVal(long long iRow, double &orVal, DSSDataFlag &orFlag);
	
	int setNumeric(long long iRow, double iVal, DSSDataFlag iFlag);
	
	int getFlag(long long iRow, DSSDataFlag &orFlag);
	
	int setFlag(long long iRow, DSSDataFlag Flag);
	
	int setFlags(long long iStart, long long iEnd, DSSDataFlag *ipFlags, int iSizeInByte);
	
	int setFlags(long long iStart, long long iEnd, DSSDataFlag *ipFlags, int* iElementList, int iElementSize);
	
	int Order(long long iRow1, long long iRow2);
    
    virtual int Order(long long iRow1, long long iRow2,bool fIsDisplayNullOnTop);

    int reserveMemForStrData();

    int createDataFlag(EnumDSSDataFlag iFlag);

	virtual int size();
	
	virtual int CreatePartition();

	virtual int getRawDataAndFlags(void** opData, DSSDataFlag** opDataFlags);

	virtual void RecycleMemory();
    
    bool IsRefColumn();
    
    //to support mapped file
    virtual int createMMapMemory(const char * ipFilePath, bool ibUseNewFile);
	virtual int MapFileBuffer();
    virtual int UnMapFileBuffer();
    virtual int ExtendColumn(int iRows);
    
    virtual int Remove(int iRow); 
    virtual int Insert(int iRow);
    
private:
	int hCompareRow(long long iRow1, long long iRow2, bool iIsNullDisplayOnTop,int iLocale=0,   bool ibSimpleComparison = false);
	int hCompareData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags, bool iIsNullDisplayOnTop,bool ibSimpleComparison = false);

	int hGetDataPtr(long long iRow, T** oppDataPtr);
	int hGetPartition(long long iGroup, T** oppPartition);
    
    int hCreateFlagArray(EnumDSSDataFlag iFlag = DssDataOk);
	
	int mDataSize;
	T *mpData;
	
	T **mppPartitionList;
	int mnPartitions;
    
    bool mbUseMappedFile;
    void * mpMMPBuffer;
    int mMMPBufSize;
    std::string mFilePath;
    int mOffset;
    int mOffsetAligned;
};


