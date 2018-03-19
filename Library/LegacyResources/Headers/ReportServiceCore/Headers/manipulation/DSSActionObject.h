/*
 *  DSSActionObject.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-27.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ACTIONOBJECT_H
#define ACTIONOBJECT_H

#include <vector>
#include <string>
#include "DSSrwdconstants.h"

class DSSActionObject
{
protected:
	EnumManipulationCommandTypes mManipulationType;
	std::string mEventID;
	bool mIsDeltaXML;
	bool mApplyNow;

public:
	DSSActionObject();
	virtual ~DSSActionObject();
	DSSActionObject(EnumManipulationCommandTypes iManipulationType);
	EnumManipulationCommandTypes getManipulationType();
	virtual std::vector<std::string> getArguments(int iWebServerType = 0); // 0 for jsp and 1 for asp
	virtual void setArguments(std::vector<std::string> args, int iWebServerType = 0);
	bool IsDeltaXML();
	bool getApplyNow();
	void setApplyNow(bool iApplyNow);
};

#endif
