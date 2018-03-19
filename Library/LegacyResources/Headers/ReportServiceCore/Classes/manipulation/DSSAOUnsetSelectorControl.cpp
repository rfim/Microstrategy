/*
 *  DSSAOUnsetSelectorControl.cpp
 *  ReportServiceCore
 *
 *  Created by lcao on 5/9/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#include "DSSAOUnsetSelectorControl.h"
#include <sstream>

DSSAOUnsetSelectorControl::DSSAOUnsetSelectorControl()
{
	mManipulationType = ManipulationSetUnsetFromControl;
}

DSSAOUnsetSelectorControl::DSSAOUnsetSelectorControl(EnumManipulationCommandTypes iManipulationType) : DSSActionObject(iManipulationType)
{}

DSSAOUnsetSelectorControl::~DSSAOUnsetSelectorControl()
{}

std::string DSSAOUnsetSelectorControl::getCtrlKey()
{
	return mCtrlKey;
}

std::string DSSAOUnsetSelectorControl::getNodeKey()
{
	return mNodeKey;
}

void DSSAOUnsetSelectorControl::setCtrlKey(std::string value)
{
	mCtrlKey = value;
}

void DSSAOUnsetSelectorControl::setNodeKey(std::string value)
{
	mNodeKey = value;
}

EnumDSSXMLRWSelectorUnsetStatus DSSAOUnsetSelectorControl::getSus()
{
	return mSus;
}

int DSSAOUnsetSelectorControl::getSuc()
{
	return mSuc;
}

void DSSAOUnsetSelectorControl::setSus(EnumDSSXMLRWSelectorUnsetStatus iSus)
{
	mSus = iSus;
}

void DSSAOUnsetSelectorControl::setSuc(int iSuc)
{
	mSuc = iSuc;
}

std::vector<std::string> DSSAOUnsetSelectorControl::getArguments(int iWebServerType)
{
	std::vector<std::string> arguments;
	
	arguments.push_back("2048128");		//Event ID for unsetSelectorControl
	arguments.push_back("");
	
	arguments.push_back("ctlKey");
	arguments.push_back(mCtrlKey);
	
	arguments.push_back("nodeKey");
	arguments.push_back(mNodeKey);	//5.push nodeKey
	
	std::string tmp; 
    std::stringstream oss;
    oss << mSus;
    tmp = oss.str();
	arguments.push_back("sus");
	arguments.push_back(tmp);	//7.push mSus
	
	oss.str("");
	oss << mSuc;
	tmp = oss.str();
	arguments.push_back("suc");
	arguments.push_back(tmp);	//9.push mSuc
	
	arguments.push_back("applyNow");
	if (mApplyNow)
		arguments.push_back("1");
	else
		arguments.push_back("0");	//11.applyNow
	
	return arguments;
}

void DSSAOUnsetSelectorControl::setArguments(std::vector<std::string> args, int iWebServerType)
{
	if (args.size() < 12)
		return;
		
	mCtrlKey = args[3];
	mNodeKey = args[5];
	
	std::string tmp = args[7];
	mSus = (EnumDSSXMLRWSelectorUnsetStatus)atoi(tmp.c_str());
	
	tmp = args[9];
	mSuc = atoi(tmp.c_str());
}
