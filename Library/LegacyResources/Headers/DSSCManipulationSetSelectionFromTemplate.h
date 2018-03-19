/*
 *  DSSCManipulationSetSelectionFromTemplate.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-15.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CMANIPULATION_SET_SELECTION_FROM_TEMPLATE_H
#define CMANIPULATION_SET_SELECTION_FROM_TEMPLATE_H

#include <map>
#include "DSSObjectContext.h"
#include "DSSRWIterator.h"
#include "DSSCellHandle.h"
#include "DSSCManipulation.h"
class DSSAOSelection;
class DSSRWTemplateNode;
class DSSRWNode;
class DSSRWControl;
class DSSBaseElementsProxy;
class DSSTemplateUnit;

typedef std::vector<DSSRWControl*> ControlVector;

class DSSCManipulationSetSelectionFromTemplate : public DSSCManipulation
{
private:
	DSSRWTemplateNode* mpTemplateNode;
	DSSBaseElementsProxy *mpElements;
	DSSTemplateUnit* mpTemplateUnit;
	int mCase;
	ControlVector mControls;
		
	bool hMatchAllElementToControl(DSSRWControl* ipControl, DSSTemplateUnit* ipTemplateUnit);
	bool hMatchElementToControl(DSSRWControl* ipControl, DSSBaseElementProxy* ipElement);
	void hInit(DSSRWIterator* ipIterator, DSSCellHandle* ipCellHandle, std::string iDisplayText = "");
    
    // clark, new requirement from frontend - create a DSSCManipulationSetSelectionFromTemplate for each RWControl on the TemplateNode
    GUID mSourceID; // source id for the RWControl
    bool mIsNewBehavior;
    bool mAOXML;
    bool mIsUC;
    
    //724108, similar with 707627, lcao 20130301
    DSSRWSectionNode* mpRedrawParentNode;
	int mRedrawChildIndex;
    
    DSSRWIterator* mpIterator;//lishan manipulation
    std::vector<DSSCellHandle> mpCellHandles;
public:
	DSSCManipulationSetSelectionFromTemplate(DSSRWIterator* ipIterator, DSSCellHandle* ipCellHandle, std::string iDisplayText);

	DSSCManipulationSetSelectionFromTemplate(DSSRWIterator* ipIterator, DSSCellHandle* ipCellHandle, bool iAOXML = false);
	DSSCManipulationSetSelectionFromTemplate(DSSRWIterator* ipIterator, std::vector<DSSCellHandle*> iCellHandleVector, bool iAOXML = false);
    DSSCManipulationSetSelectionFromTemplate(DSSRWTemplateNode* ipTemplateNode, GUID iSourceID, DSSBaseElementsProxy* ipElements, bool iAOXML = false);
	DSSCManipulationSetSelectionFromTemplate(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSCManipulationSetSelectionFromTemplate();
	int Execute();
	DSSRWTemplateNode* getTemplateNode();
	ControlVector* getControls();
    bool IsNeedServer();

    DSSRWSectionNode* getRedrawParentNode();
    int getRedrawChildIndex();
	DSSRWNode* getSelectionNode();
    
    void GenerateAODeltaXML();
    
    DSSRWIterator* getIterator();// lishan manipulation
    std::vector<DSSCellHandle> getCellHandles();
    void resetAllLayouts();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
};
inline DSSRWSectionNode* DSSCManipulationSetSelectionFromTemplate::getRedrawParentNode()
{
    return mpRedrawParentNode;
}

inline int DSSCManipulationSetSelectionFromTemplate::getRedrawChildIndex()
{
    return mRedrawChildIndex;
}

#endif
