/*
 *  DSSManipulationSetSelectionFromControl.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-3-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSRWControlNode.h"
#include "DSSCManipulation.h"

typedef std::map<std::string, DSSRWNode*> NodeMapModelChanged;

class DSSCManipulationSetSelectionFromControl : public DSSCManipulation
{
private:
	DSSRWControlNode* mpControlNode;
	DSSBaseElementsProxy *mpElements;
	NodeMapModelChanged mNodeMapModelChanged;
	DSSRWNode* mpPanelStack;
	
public:
	DSSCManipulationSetSelectionFromControl(DSSRWControlNode* ipControlNode, DSSBaseElementsProxy *ipElements);
	~DSSCManipulationSetSelectionFromControl();
	int Execute();
	NodeMapModelChanged* getNodeMapModelChanged();
	DSSRWNode* getPanelStack();
};
