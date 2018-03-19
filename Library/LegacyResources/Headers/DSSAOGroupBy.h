/*
 *  DSSAOGroupBy.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AOGROUPBY_H
#define AOGROUPBY_H

#include "DSSActionObject.h"

class DSSAOGroupBy : public DSSActionObject
{
private:
	std::string mNodeKey;
	std::string mElementID;
public:
	DSSAOGroupBy();
	virtual ~DSSAOGroupBy();
	DSSAOGroupBy(EnumManipulationCommandTypes iManipulationType);
	std::string getNodeKey();
	std::string getElementID();
	void setNodeKey(std::string iNodeKey);
	void setElementID(std::string iElementID);
	
	std::vector<std::string> getArguments(int iWebServerType = 0);
	void setArguments(std::vector<std::string> args, int iWebServerType = 0);
};

#endif
