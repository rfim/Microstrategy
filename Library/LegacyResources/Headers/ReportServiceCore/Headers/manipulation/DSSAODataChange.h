/*
 *  DSSAODataChange.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 11-6-23.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AO_DATA_CHANGE_H
#define AO_DATA_CHANGE_H

#include "DSSActionObject.h"
#include "DSSObjectInfo.h"

struct DSSCellHandleInfo
{
	EnumDSSCellHandleType CellHandleType;
	int FieldIndex;
	int RowIndexForField;
	int RowOrdinal;
	int ColOrdinal;
	int Axis;
	int Ordinal;
	int Depth;
	int FormID;
};

class DSSAODataChange : public DSSActionObject 
{
private:
	std::string mNodeKey;
	std::string mVariantString;
	std::vector<std::string> mVariantStrings;
	bool mAutoRefresh;
	bool mUpdateSingleCell;
	std::vector<int> mNGBPath;
	EnumDSSIteratorType mIteratorType;
	DSSCellHandleInfo mCellHandleInfo;
	std::vector<DSSCellHandleInfo> mCellHandleInfos;
	
public:
	DSSAODataChange();
	DSSAODataChange(EnumManipulationCommandTypes iManipulationType);
	virtual ~DSSAODataChange();
	std::vector<std::string> getArguments(int iWebServerType = 0);
	void setArguments(std::vector<std::string> args, int iWebServerType);
	void setNodeKey(std::string value);
	std::string getNodeKey();
	
	void setVariantString(std::string value);
	std::string getVariantString();
	void AddVariantString(std::string value);
	std::vector<std::string>* getVariantStrings();
	
	void setAutoRefresh(bool value);
	bool getAutoRefresh();
	void setUpdateSingleCell(bool value);
	bool getUpdateSingleCell();
	void setNGBPath(std::vector<int> iNGBPath);
	std::vector<int> getNGBPath();
	EnumDSSIteratorType getIteratorType();
	void setIteratorType(EnumDSSIteratorType iType);
	
	DSSCellHandleInfo getCellHandleInfo();
	void setCellHandleInfo(DSSCellHandleInfo iInfo);
	std::vector<DSSCellHandleInfo>* getCellHandleInfos();
	void AddCellHandleInfo(DSSCellHandleInfo iInfo);
};

#endif