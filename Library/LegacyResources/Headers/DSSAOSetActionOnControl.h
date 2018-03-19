/*
 *  DSSAOSetActionOnControl.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 11-6-23.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AO_SET_ACTION_ON_CONTROL_H
#define AO_SET_ACTION_ON_CONTROL_H

#include "DSSActionObject.h"

class DSSAOSetActionOnControl : public DSSActionObject 
{
private:
	std::string mControlNodeKey;
	std::string mControlKey;
	EnumDSSRWControlActionType mActionType;
public:
	DSSAOSetActionOnControl();
	DSSAOSetActionOnControl(EnumManipulationCommandTypes iManipulationType);
	virtual ~DSSAOSetActionOnControl();
	std::vector<std::string> getArguments(int iWebServerType = 0);
	void setArguments(std::vector<std::string> args, int iWebServerType);
	void setCtrlKey(std::string value);
	std::string getCtrlKey();
	void setNodeKey(std::string value);
	std::string getNodeKey();
	void setActionType(EnumDSSRWControlActionType iType);
	EnumDSSRWControlActionType getActionType();
};

#endif
