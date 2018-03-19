//
//  DSSDataHolder.h
//  ReportServiceCore
//
//  Created by Tang, Cheng on 5/18/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSDataHolder_h
#define ReportServiceCore_DSSDataHolder_h
#include "DSSrwdconstants.h"

class DSSBinaryDataReader;

class DSSDataHolder
{
public:
	DSSDataHolder();
	virtual ~DSSDataHolder();
    
    virtual int LoadDataFromBinary(DSSBinaryDataReader *ipReadStream) = 0;
    
	virtual int Init(EnumDSSDataType iDataType, int iRowCount) = 0;
	
	virtual int getRowSize(int iRow, int &orSize) = 0;
	
	virtual int setData(int iRow, char *ipData, int iSizeInByte) = 0;
    
    virtual int setDataToNULL(int iRow, int iSizeInByte) = 0;
	
	virtual int getData(int iRow, const char **oppData, int &orSize) = 0;
	
	virtual int getDataCopy(int iRow, char *pBuffer, int iBufSize, int &orCopiedSize) = 0;
	
	virtual int setDataInBulk(char *ipData, int iSizeInByte) = 0;
	
	virtual int getNumericVal(int iRow, double &orVal, DSSDataFlag &orFlag) = 0;
	
	virtual int setNumeric(int iRow, double iVal) = 0;
    
    virtual int hCompareData(int iRow, EnumDSSDataType iDataType, unsigned char* ipData, int iSize, bool ibSimpleComparison = false) = 0;
    
    virtual int count() = 0;
    
    virtual int ExtendColumn(int iRows) = 0;
};


#endif
