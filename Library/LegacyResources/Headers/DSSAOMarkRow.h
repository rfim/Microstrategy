/*
 *  DSSAOMarkRow.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 11-6-24.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AO_MARK_ROW_H
#define AO_MARK_ROW_H

#include "DSSActionObject.h"

class DSSAOMarkRow : public DSSActionObject 
{
private:
	std::string mNodeKey;
	int mRowOrdinal;
	EnumDSSRWMarkRowAction mActionType;
	std::vector<int> mNGBPath;
public:
	DSSAOMarkRow();
	DSSAOMarkRow(EnumManipulationCommandTypes iManipulationType);
	~DSSAOMarkRow();
	std::vector<std::string> getArguments(int iWebServerType = 0);
	void setArguments(std::vector<std::string> args, int iWebServerType);
	void setNodeKey(std::string value);
	std::string getNodeKey();
	void setRowOrdinal(int iOrdinal);
	int getRowOrdinal();
	void setActionType(EnumDSSRWMarkRowAction iType);
	EnumDSSRWMarkRowAction getActionType();
	void setNGBPath(std::vector<int> iNGBPath);
	std::vector<int> getNGBPath();
};

#endif