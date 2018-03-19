/*
 *  DSSAODisplayMode.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-27.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef AODISPLAYMODE_H
#define AODISPLAYMODE_H

#include <string>
#include "DSSActionObject.h"

class DSSAODisplayMode : public DSSActionObject
{
private:
	std::string mNodeKey;
	EnumDSSDisplayMode mDisplayMode;
public:
	DSSAODisplayMode();
	virtual ~DSSAODisplayMode();
	DSSAODisplayMode(EnumManipulationCommandTypes iManipulationType);
	std::string getNodeKey();
	void setNodeKey(std::string iNodeKey);
	EnumDSSDisplayMode getDisplayMode();
	void setDisplayMode(EnumDSSDisplayMode iMode);
	
	std::vector<std::string> getArguments(int iWebServerType = 0);
	void setArguments(std::vector<std::string> args, int iWebServerType = 0);
};

#endif
