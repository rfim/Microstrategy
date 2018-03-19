/*
 *  DSSColumnMappingInfo.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 11-1-6.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef COLUMN_MAPPING_INFO_H
#define COLUMN_MAPPING_INFO_H

#include "GUID.h"
#include "DSSrwdconstants.h"

class DSSColumnMappingInfo
{
private:
	EnumDSSObjectType mType;
	GUID mAttributeID;
	GUID mFormID;
	int mBaseFormID;
	GUID mMetricID;
	int mColumnIndex;
	EnumDSSBaseFormType mBaseFormType;
	EnumDSSTransactionMappingFlags mColumnFlags;
	EnumDSSDataType mDataType;
	
public:
	DSSColumnMappingInfo(EnumDSSObjectType iType,
					  GUID& iAttributeID,
					  GUID& iFormID,
					  int iBaseFormID,
					  GUID& iMetricID,
					  EnumDSSDataType iDataType,
					  EnumDSSBaseFormType iBaseFormType,
					  int iColumnIndex,
					  EnumDSSTransactionMappingFlags iColumnFlags);
	
	virtual ~DSSColumnMappingInfo();
	
	EnumDSSObjectType getType();
	GUID getAttributeID();
	GUID getFormID();
	int getBaseFormID();
	GUID getMetricID();
	int getColumnIndex();
	EnumDSSBaseFormType getBaseFormType();
	EnumDSSTransactionMappingFlags getColumnFlags();	
	EnumDSSDataType getDataType();
};
#endif
