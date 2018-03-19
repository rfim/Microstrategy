/*
 *  DSSCManipulationToggleCurrentSelectionOnControl.h
 *  ReportServiceCore
 *
 *  Created by lcao on 3/28/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CMANIPULATION_TOGGLE_CURRENT_SELECTION_ON_CONTROL_H
#define CMANIPULATION_TOGGLE_CURRENT_SELECTION_ON_CONTROL_H

#include "DSSObjectContext.h"
#include "DSSCManipulation.h"
#include "DSSSimpleXML.h"
class DSSActionObject;
class DSSRWControlNode;
class DSSRWNode;
class DSSRWControl;
class DSSBaseElementsProxy;
class DSSCExpression;
class DSSExpNode;

class DSSCManipulationToggleCurrentSelectionOnControl : public DSSCManipulation
{
private:
	DSSBaseElementsProxy *mpElements;
	DSSCExpression* mpExpression;
    DSSRWControlNode* mpControlNode;
    DSSRWSectionNode* mpRedrawParentNode;
	int mRedrawChildIndex;
		
	void hParseExpressionFromXML(std::string& iXMLString, DSSObjectContext* ipContext);
	std::string hBuildExpressionXML();
	DSSExpNode* hBuildExpNode(SimpleXMLNode& iXMLNode, DSSExpNode* ipParentExpNode, DSSObjectContext* ipContext);
	
public:
	DSSCManipulationToggleCurrentSelectionOnControl(DSSRWControlNode* ipControlNode);
	DSSCManipulationToggleCurrentSelectionOnControl(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSCManipulationToggleCurrentSelectionOnControl();
	int Execute();
    DSSRWControlNode* getControlNode();
    DSSRWSectionNode* getRedrawParentNode();
    int getRedrawChildIndex();
};

inline DSSRWControlNode* DSSCManipulationToggleCurrentSelectionOnControl::getControlNode()
{
    return mpControlNode;
}

inline DSSRWSectionNode* DSSCManipulationToggleCurrentSelectionOnControl::getRedrawParentNode()
{
    return mpRedrawParentNode;
}

inline int DSSCManipulationToggleCurrentSelectionOnControl::getRedrawChildIndex()
{
    return mRedrawChildIndex;
}

#endif