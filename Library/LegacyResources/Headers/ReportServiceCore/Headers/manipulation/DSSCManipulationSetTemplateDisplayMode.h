/*
 *  DSSCManipulationSetTemplateDisplayMode.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-19.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CMANIPULATION_SET_TEMPLATE_DISPLAY_MODE_H
#define CMANIPULATION_SET_TEMPLATE_DISPLAY_MODE_H

#include "DSSrwdconstants.h"
#include "DSSObjectContext.h"
#include "DSSCManipulation.h"

class DSSAODisplayMode;
class DSSRWTemplateNode;

class DSSCManipulationSetTemplateDisplayMode : public DSSCManipulation
{
private:
	DSSRWTemplateNode* mpTemplateNode;
	EnumDSSDisplayMode mDisplayNode;
	bool mNeedUpdate;
    bool mAOXML;
		
public:
	DSSCManipulationSetTemplateDisplayMode(DSSRWTemplateNode* ipTemplateNode, EnumDSSDisplayMode iDisplayNode, bool iAOXML = false);
	DSSCManipulationSetTemplateDisplayMode(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSCManipulationSetTemplateDisplayMode();
	int Execute();
	DSSRWTemplateNode* getTemplateNode();
    
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
};

#endif
