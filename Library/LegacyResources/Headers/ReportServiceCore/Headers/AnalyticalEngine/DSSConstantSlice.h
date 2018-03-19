/*
 *  DSSConstantSlice.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 2/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#import "DSSCubeDataSlice.h"
//#include "PDCwinerror.h"

class DSSConstantSlice : public DSSCubeDataSlice
{
public:
	DSSConstantSlice();
	
	int Init(char *ipData, int SizeInBype, EnumDSSDataType iDataType, DSSDataFlag iFlag);
	
	virtual ~DSSConstantSlice();
	
	virtual EnumDSSDataType getDataType();
	
	virtual int getSizeOfData(int &orRowSize);
	virtual int getFlag(int nKey, int *ipKey, DSSDataFlag &orFlag);
	
	virtual int setVal(int nKey, int *ipKey, char *ipData, int iSizeInByte, EnumDSSDataType iType, DSSDataFlag iFlag);
	virtual int getVal(int nKey, int *ipKey, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag, int *opThresholdID = NULL);
	virtual int setValByOffset(int iOffset, char *ipData, int iSizeInByte, EnumDSSDataType iType, DSSDataFlag iFlag);
	virtual int getValByOffset(int iOffset, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag, int *opThresholdID);
	
	virtual int getNumeric(int nKey, int *ipKey, double &orVal, DSSDataFlag &orFlag, int *opThresholdID = NULL);
	virtual int getNumericByOffset(int iOffset, double &orVal, DSSDataFlag &orFlag, int *opThresholdID = NULL);
	
	virtual int setNumericByOffset(int iOffset, double &irVal, DSSDataFlag &irFlag);
	
	virtual int setValueAndThreshold(int nKey, int *ipKey, char *ipData, int iSizeInByte, EnumDSSDataType iType, DSSDataFlag iFlag, int iThresholdID);
	
	virtual int setValueAndThresholdByOffset(int offset, char *ipData, int iSizeInByte, EnumDSSDataType iType, DSSDataFlag iFlag, int iThresholdID);
	
	virtual int updateValueWithString(int offset, char *ipString, int iStrLength);
	
	virtual int size();
	
private:
	
	EnumDSSDataType mDataType;
	
	int mDataSize;
	
	DSSDataFlag mDataFlag;
	
	unsigned char *mpData;
};