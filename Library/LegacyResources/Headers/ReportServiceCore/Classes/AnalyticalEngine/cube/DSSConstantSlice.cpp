/*
 *  DSSConstantSlice.cpp
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 2/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#import <stdio.h>
#import "DSSConstantSlice.h"
#import "DSSCellFmtData.h"
#import "DSSBigDecimal.h"

#include "DSSAEHelper.h"

DSSConstantSlice::DSSConstantSlice(): mDataType(DssDataTypeUnknown), mDataSize(0), mDataFlag(DssDataNull), mpData(NULL)
{
    setConstantSlice();
}

DSSConstantSlice::~DSSConstantSlice()
{
	if (mpData)
	{
		delete mpData;
		mpData = NULL;
	}
}

int DSSConstantSlice::Init(char *ipData, int SizeInBype, EnumDSSDataType iDataType, DSSDataFlag iFlag)
{
	if (mpData)
		return E_FAIL;
		
	mpData = new unsigned char [SizeInBype];
	memmove(mpData, ipData, SizeInBype);
	
	mDataType = iDataType;
	mDataSize = SizeInBype;
	mDataFlag = iFlag;
	return S_OK;
}

EnumDSSDataType DSSConstantSlice::getDataType()
{
	return mDataType;
}

int DSSConstantSlice::getSizeOfData(int &orRowSize)
{
	orRowSize = mDataSize;
	return S_OK;
}

int DSSConstantSlice::getFlag(int nKey, int *ipKey, DSSDataFlag &orFlag)
{
	orFlag = mDataFlag;
	return S_OK;
}

int DSSConstantSlice::getVal(int nKey, int *ipKey, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag, int *opThresholdID)
{
	return getValByOffset(0, oppData, orSize, orType, orFlag, opThresholdID);
}

int DSSConstantSlice::getValByOffset(int iOffset, const char **oppData, int &orSize, EnumDSSDataType &orType, DSSDataFlag &orFlag, int *opThresholdID)
{
	if (oppData)
		*oppData = reinterpret_cast<const char *>(mpData);
	
	orSize = mDataSize;
	
	orType = mDataType;
	
	orFlag = mDataFlag;
	
	return S_OK;
}

int DSSConstantSlice::getNumeric(int nKey, int *ipKey, double &orVal, DSSDataFlag &orFlag, int *opThresholdID)
{
	return getNumericByOffset(0, orVal, orFlag, opThresholdID);
}

int DSSConstantSlice::getNumericByOffset(int iOffset, double &orVal, DSSDataFlag &orFlag, int *opThresholdID)
{
	orFlag = mDataFlag;
	
	switch(mDataType)
	{
		case DssDataTypeShort:
		{
			orVal = *(short*)mpData;
		}
			break;
		case DssDataTypeInteger:
		case DssDataTypeUnsigned:
		{
			orVal = *(int*)mpData;
		}
			break;
		case DssDataTypeLong:
		{
			orVal = *(long long*)mpData;
		}
			break;
		case DssDataTypeFloat:
		case DssDataTypeReal:
		{
			orVal = *(float*)mpData;
		}
			break;
		case DssDataTypeDouble:
		case DssDataTypeNumeric:
		case DssDataTypeDecimal:
		{
			orVal = *(double*)mpData;

		}
			break;
		case DssDataTypeBool:
		case DssDataTypeMissing:
		{
			orVal = *(bool*)mpData;
		}
			break;
		case DssDataTypeTime:
		case DssDataTypeTimeStamp:
		case DssDataTypeDate:
		{
			orFlag = DssDataNull;
		}
			break;
		case DssDataTypeCellFormatData:
		{
			orVal = ((MDataType::DSSCellFmtData*)mpData)->GetValue();
		}
			
			break;
/*		case DssDataTypeBigDecimal:
		case DssDataTypeChar:
		case DssDataTypeVarChar:
		case DssDataTypeNChar:
		case DssDataTypeNVarChar:
		case DssDataTypeLongVarChar:
		case DssDataTypeMBChar:
		case DssDataTypeUTF8Char:
		case DssDataTypeBinary:
		case DssDataTypeVarBin:
		case DssDataTypeLongVarBin:
			orFlag = DssDataNull;
			break;*/
		default:
			orFlag = DssDataNull;
			break;
	}
	return S_OK;	
}

int DSSConstantSlice::setVal(int nKey, int *ipKey, char *ipData, int iSizeInByte, EnumDSSDataType iType, DSSDataFlag iFlag)
{
	return E_NOTIMPL;
}

int DSSConstantSlice::setValByOffset(int iOffset, char *ipData, int iSizeInByte, EnumDSSDataType iType, DSSDataFlag iFlag)
{
	return E_NOTIMPL;
}

int DSSConstantSlice::setNumericByOffset(int iOffset, double &irVal, DSSDataFlag &irFlag)
{
	return E_NOTIMPL;
}

int DSSConstantSlice::setValueAndThreshold(int nKey, int *ipKey, char *ipData, int iSizeInByte, EnumDSSDataType iType, DSSDataFlag iFlag, int iThresholdID)
{
	return E_NOTIMPL;
}

int DSSConstantSlice::setValueAndThresholdByOffset(int offset, char *ipData, int iSizeInByte, EnumDSSDataType iType, DSSDataFlag iFlag, int iThresholdID)
{
	return E_NOTIMPL;
}

int DSSConstantSlice::updateValueWithString(int offset, char *ipString, int iStrLength)
{
	return E_NOTIMPL;
}

int DSSConstantSlice::size()
{
	int lSize = sizeof(this);
	
	lSize += DSSCubeDataSlice::size();
	
	lSize += mDataSize;
	
	return lSize;
}
