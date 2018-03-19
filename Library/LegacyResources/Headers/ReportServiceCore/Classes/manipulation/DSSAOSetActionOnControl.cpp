/*
 *  DSSAOSetActionOnControl.cpp
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 11-6-23.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSAOSetActionOnControl.h"
#include <sstream>
#include <stdlib.h>

DSSAOSetActionOnControl::DSSAOSetActionOnControl()
{
	mManipulationType = ManipulationRecalculate;
}

DSSAOSetActionOnControl::DSSAOSetActionOnControl(EnumManipulationCommandTypes iManipulationType) : DSSActionObject(iManipulationType)
{}

DSSAOSetActionOnControl::~DSSAOSetActionOnControl()
{}

std::vector<std::string> DSSAOSetActionOnControl::getArguments(int iWebServerType)
{
	std::vector<std::string> arguments;
	
	arguments.push_back(mControlNodeKey);
	arguments.push_back(mControlKey);
	
	std::string tmp; 
    std::stringstream oss;
    oss << mActionType;
    tmp = oss.str();
	arguments.push_back(tmp);
	
	return arguments;
}

void DSSAOSetActionOnControl::setArguments(std::vector<std::string> args, int iWebServerType)
{
	if (args.size() < 3) return;
	
	mControlNodeKey = args[0];
	mControlKey = args[1];
	
	std::string tmp = args[2];
	mActionType = (EnumDSSRWControlActionType)atoi(tmp.c_str());	
}

void DSSAOSetActionOnControl::setCtrlKey(std::string value)
{
	mControlKey = value;
}

std::string DSSAOSetActionOnControl::getCtrlKey()
{
	return mControlKey;
}

void DSSAOSetActionOnControl::setNodeKey(std::string value)
{
	mControlNodeKey = value;
}

std::string DSSAOSetActionOnControl::getNodeKey()
{
	return mControlNodeKey;
}

void DSSAOSetActionOnControl::setActionType(EnumDSSRWControlActionType iType)
{
	mActionType = iType;
}

EnumDSSRWControlActionType DSSAOSetActionOnControl::getActionType()
{
	return mActionType;
}
