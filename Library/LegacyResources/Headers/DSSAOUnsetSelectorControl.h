/*
 *  DSSAOUnsetSelectorControl.h
 *  ReportServiceCore
 *
 *  Created by lcao on 5/9/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AO_UNSET_SELECTOR_CONTROL_H
#define AO_UNSET_SELECTOR_CONTROL_H

#include "DSSActionObject.h"
#include "GUID.h"

class DSSAOUnsetSelectorControl : public DSSActionObject 
{
private:
	std::string mCtrlKey;
	std::string mNodeKey;
	EnumDSSXMLRWSelectorUnsetStatus mSus;
	int mSuc;
	
public:
	DSSAOUnsetSelectorControl();
	DSSAOUnsetSelectorControl(EnumManipulationCommandTypes iManipulationType);
	virtual ~DSSAOUnsetSelectorControl();
	std::vector<std::string> getArguments(int iWebServerType = 0);
	void setArguments(std::vector<std::string> args, int iWebServerType);
	void setCtrlKey(std::string value);
	std::string getCtrlKey();
	void setNodeKey(std::string value);
	std::string getNodeKey();
	EnumDSSXMLRWSelectorUnsetStatus getSus();
	void setSus(EnumDSSXMLRWSelectorUnsetStatus iSus);
	int getSuc();
	void setSuc(int iSuc);
	
};

#endif