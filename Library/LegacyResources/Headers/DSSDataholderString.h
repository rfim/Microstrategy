//
//  DSSDataholderString.h
//  ReportServiceCore
//
//  Created by Tang, Cheng on 5/18/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSDataholderString_h
#define ReportServiceCore_DSSDataholderString_h

#import "DSSDataHolder.h"

template <class T>
class DSSDataholderString : public DSSDataHolder
{
public:
	DSSDataholderString();
	virtual ~DSSDataholderString();
    
    int LoadDataFromBinary(DSSBinaryDataReader *ipReadStream);
    
	int Init(EnumDSSDataType iDataType, int iRowCount);
	
	int getRowSize(int iRow, int &orSize);
	
	int setData(int iRow, char *ipData, int iSizeInByte);
    
    int setDataToNULL(int iRow, int iSizeInByte);
	
	int getData(int iRow, const char **oppData, int &orSize);
	
	int getDataCopy(int iRow, char *pBuffer, int iBufSize, int &orCopiedSize);
	
	int setDataInBulk(char *ipData, int iSizeInByte);
	
	int getNumericVal(int iRow, double &orVal, DSSDataFlag &orFlag);
	
	int setNumeric(int iRow, double iVal);
    
    int hCompareData(int iRow, EnumDSSDataType iDataType, unsigned char* ipData, int iSize, bool ibSimpleComparison = false);
    
    virtual int count();
    
    int ExtendColumn(int iRows);
private:
    
    int mnRow;
    
    T** mppData;
    int *mpRowSize;
    
};

#endif
