/*
 *  DSSAOSetObjectProperties.cpp
 *  ReportServiceCore
 *
 *  Created by lcao on 3/30/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSAOSetObjectProperties.h"
#include <sstream>

DSSAOSetObjectProperties::DSSAOSetObjectProperties()
{
//	mManipulationType = ManipulationSetControlProperties;
}

DSSAOSetObjectProperties::DSSAOSetObjectProperties(EnumManipulationCommandTypes iManipulationType):DSSActionObject(iManipulationType)
{
}

DSSAOSetObjectProperties::~DSSAOSetObjectProperties()
{}

int DSSAOSetObjectProperties::getFmtType()
{
	return mFmtType;
}

std::string DSSAOSetObjectProperties::getData()
{
	return mData;
}

std::string DSSAOSetObjectProperties::getNodeKey()
{
	return mNodeKey;
}

void DSSAOSetObjectProperties::setFmtType(int value)
{
	mFmtType = value;
}

void DSSAOSetObjectProperties::setData(std::string value)
{
	mData = value;
}

void DSSAOSetObjectProperties::setNodeKey(std::string iNodeKey)
{
	mNodeKey = iNodeKey;
}

std::vector<std::string> DSSAOSetObjectProperties::getArguments(int iWebServerType)
{
	std::vector<std::string> arguments;
	
	arguments.push_back("2048009");		//Event ID for setObjectProps
	arguments.push_back("");
	
	arguments.push_back("data");
	arguments.push_back(mData);
	
	std::string temp;
	std::stringstream oss;
	oss << mFmtType;
	temp = oss.str();
	arguments.push_back("FormatType");
	arguments.push_back(temp);		//5.fmtType
	
	arguments.push_back("applyNow");
	if (mApplyNow)
		arguments.push_back("1");
	else
		arguments.push_back("0");	//7.applyNow
	
	arguments.push_back("nodeKey");
	arguments.push_back(mNodeKey);	//9.nodeKey
	
	arguments.push_back("manipulationType");
	oss.str("");
	oss << mManipulationType;
	temp = oss.str();
	arguments.push_back(temp);		//11.manipulationType
	
	return arguments;
}

void DSSAOSetObjectProperties::setArguments(std::vector<std::string> args, int iWebServerType)
{
	if (args.size() < 12)
		return;
		
	mData = args[3];
	std::string tmp = args[5];
	mFmtType = atoi(tmp.c_str());
	mNodeKey = args[9];
	tmp = args[11];
	mManipulationType = (EnumManipulationCommandTypes)atoi(tmp.c_str());
}