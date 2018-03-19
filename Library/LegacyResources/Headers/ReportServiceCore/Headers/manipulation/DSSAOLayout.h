/*
 *  DSSAOLayout.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-27.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AOLAYOUT_H
#define AOLAYOUT_H

#include <string>
#include "DSSActionObject.h"

class DSSAOLayout : public DSSActionObject
{
private:
	std::string mLayoutNodeKey;
	std::string mRootNodeKey;
public:
	DSSAOLayout();
	virtual ~DSSAOLayout();
	DSSAOLayout(EnumManipulationCommandTypes iManipulationType);
	std::string getLayoutNodeKey();
	void setLayoutNodeKey(std::string iKey);
	std::string getRootNodeKey();
	void setRootNodeKey(std::string iKey);
	
	std::vector<std::string> getArguments(int iWebServerType = 0);
	void setArguments(std::vector<std::string> args, int iWebServerType = 0);
};

#endif
