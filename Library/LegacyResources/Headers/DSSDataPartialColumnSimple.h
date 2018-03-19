//
//  DSSDataPartialColumnSimple.h
//  ReportServiceCore
//
//  Created by Tang, Cheng on 5/18/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSDataPartialColumnSimple_h
#define ReportServiceCore_DSSDataPartialColumnSimple_h

#include "DSSDataPartialColumn.h"

class DSSDataPartialColumnSimple : public DSSDataPartialColumn
{
public:
    DSSDataPartialColumnSimple();
	virtual ~DSSDataPartialColumnSimple();
    
    int getSizeOfData();
	
	int getRowSize(long long iRow, int &orSize);
	
	int setData(long long iRow, char *ipData, int iSizeInByte, DSSDataFlag iFlag);
	
	int getData(long long iRow, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag);
	
	int getDataCopy(long long iRow, char *pBuffer, int iBufSize, int &orCopiedSize, EnumDSSDataType &orType, DSSDataFlag &orFlag);

private:
    
    int hCompareData(long long iRow, unsigned char* ipData, int iSize, DSSDataFlag iFlags, bool iIsNullDisplayOnTop,bool ibSimpleComparison = false);
    
    int hLoadData(DSSBinaryDataReader *ipReadStream, DSSDataHolder* ipHolder, EnumDSSDataType iDataType, int nElement);
    
    int hLoadRawData(DSSBinaryDataReader *ipReadStream, DSSDataHolder* ipHolder, EnumDSSDataType iDataType, int nElement);
    
    int hLoadStringData(DSSBinaryDataReader *ipReadStream, DSSDataHolder* ipHolder, EnumDSSDataType iDataType, int nElement);  
    int mDataSize;
};

#endif
