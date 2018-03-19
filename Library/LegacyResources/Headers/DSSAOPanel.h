/*
 *  DSSAOPanel.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-11-3.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef AOPANEL_H
#define AOPANEL_H

#include <string>
#include "DSSActionObject.h"

class DSSAOPanel : public DSSActionObject
{
private:
	std::string mPanelStackKey;
	std::string mPanelKey;
    std::string mUnitContextKey;
public:
	DSSAOPanel();
	virtual ~DSSAOPanel();
	DSSAOPanel(EnumManipulationCommandTypes iManipulationType);
	std::string getPanelKey();
	void setPanelKey(std::string iKey);
	std::string getPanelStackKey();
	void setPanelStackKey(std::string iKey);
    std::string getUnitContextKey();
    void setUnitContextKey(std::string iKey);
	
	std::vector<std::string> getArguments(int iWebServerType = 0);
	void setArguments(std::vector<std::string> args, int iWebServerType = 0);
};

#endif
