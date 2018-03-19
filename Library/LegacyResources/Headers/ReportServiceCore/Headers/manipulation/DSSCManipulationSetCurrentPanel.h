/*
 *  DSSCManipulationSetCurrentPanel.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-11-3.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CMANIPULATION_SET_CURRENT_PANEL_H
#define CMANIPULATION_SET_CURRENT_PANEL_H

#import "DSSObjectContext.h"
#include "DSSCManipulation.h"

class DSSAOPanel;
class DSSRWSectionNode;
class DSSRWNode;

class DSSCManipulationSetCurrentPanel : public DSSCManipulation
{
private:
	DSSRWSectionNode* mpPanel;
	DSSRWSectionNode* mpPanelStack;
    bool mAOXML;
		
public:
	DSSCManipulationSetCurrentPanel(DSSRWSectionNode* ipPanel, bool iAOXML = false);
	DSSCManipulationSetCurrentPanel(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    virtual ~DSSCManipulationSetCurrentPanel();
	int Execute();
	DSSRWNode* getPanelStack();
    
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
};

#endif
