/*
 *  DSSAOSetObjectProperties.h
 *  ReportServiceCore
 *
 *  Created by lcao on 3/30/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AO_SET_OBJECT_PROPERTIES_H
#define AO_SET_OBJECT_PROPERTIES_H

#include "DSSActionObject.h"
#include "GUID.h"

class DSSAOSetObjectProperties : public DSSActionObject 
{
private:
	int mFmtType;
	std::string mNodeKey;
	std::string mData;
	
public:
	DSSAOSetObjectProperties();
	DSSAOSetObjectProperties(EnumManipulationCommandTypes iManipulationType);
	virtual ~DSSAOSetObjectProperties();
	std::vector<std::string> getArguments(int iWebServerType = 0);
	void setArguments(std::vector<std::string> args, int iWebServerType);
	int getFmtType();
	std::string getData();
	void setFmtType(int value);
	void setData(std::string value);
	std::string getNodeKey();
	void setNodeKey(std::string iNodeKey);
};

#endif