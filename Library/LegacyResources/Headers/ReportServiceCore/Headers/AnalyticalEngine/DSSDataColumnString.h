/*
 *  DSSDataColumnString.h
 *  DashboardViewer
 *
 *  Created by Xiaolong Wang on 1/21/10.
 *  Copyright 2010 MicroStrategy, Inc.. All rights reserved.
 *
 */


#import "DSSDataColumn.h"
template<class T>
struct strData
{
    T* mpData;
    int mSize;
    DSSDataFlag mFlag;
    strData():mpData(NULL),mSize(0){}
    ~strData(){if(mpData)delete [] mpData;}
};

template<class T>
class DSSDataColumnString : public DSSDataColumn
{
public:
	DSSDataColumnString();
	virtual ~DSSDataColumnString();
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
    
    //TQMS 696592, hyan, 13/12/2012, we need add the nullontop argument
    int Order(long long iRow1, long long iRow2,bool fIsDisplayNullOnTop);
	
    int reserveMemForStrData();
    
    int createDataFlag(EnumDSSDataFlag iFlag);

	virtual int size();

	virtual int CreatePartition();
	
	virtual void RecycleMemory();
    
    bool IsRefColumn();
	
    //to support mapped file
    virtual int createMMapMemory(const char * ipFilePath, bool ibUseNewFile);
    virtual int MapFileBuffer();
    virtual int UnMapFileBuffer();
    
    // 12/18/2012 tehe #696636
    virtual int setTrimFlag(bool ifTrimString) { mbTrimString = ifTrimString;  return S_OK;}
    virtual int getTrimFlag(bool *ofTrimString) { *ofTrimString = mbTrimString; return S_OK; }
    
    virtual int ExtendColumn(int iRows);
    virtual int Remove(int iRow);
    virtual int Insert(int iRow);
    
private:
    virtual int hCompareRow(long long iRow1, long long iRow2, bool iIsNullDisplayOnTop,int iLocale=0, bool ibSimpleComparison = false);
    virtual int hCompareRow(long long iRow1, long long iRow2, bool iIsNullDisplayOnTop,int iLocale, bool ibSimpleComparison, bool ibCaseSensitive);
    virtual int hCompareData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags, bool iIsNullDisplayOnTop, bool ibSimpleComparison = false);
    virtual int hCompareData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags, bool iIsNullDisplayOnTop, bool ibSimpleComparison, bool ibCaseSensitive);

	int mMaxRowSize;
	
	int mDataSize;
    
    strData<T>** mppData;
    
    bool mbUseMappedFile;
    void * mpMMPBuffer;
    int mMMPBufSize;
    
    bool mbTrimString;
    
    //zhyang, 777421, partition
    bool hNeedPartition();
    int hCreatePartition();
    int hGetPartition(int iGroup, strData<T>*** oppPartition);
    strData<T>* hGetDataPtr(long long iRow, int& oResult);
    strData<T> ***mppPartitionList;
    int mnPartitions;
    bool mbUsePartition;
};
