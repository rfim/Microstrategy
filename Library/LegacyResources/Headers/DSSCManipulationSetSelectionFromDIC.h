/*
 *  DSSManipulationSetSelectionFromDIC.h
 *  ReportServiceCore
 *
 *  Created by Cao Lei on 11-9-21.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

// this manipulation is similar with DSSCManipulationSetSelectionFromControl, and it is used for data input control

#ifndef CMANIPULATION_SET_SELECTION_FROM_DIC_H
#define CMANIPULATION_SET_SELECTION_FROM_DIC_H

#include "DSSObjectContext.h"
#include "DSSCManipulation.h"
#ifndef __ANDROID__
	#include "DSSSimpleXML.h"
#endif
class DSSActionObject;
class DSSRWControlNode;
class DSSRWNode;
class DSSRWControl;
class DSSBaseElementsProxy;
class DSSCExpression;
class DSSExpNode;
class DSSRWGridIterator;

typedef std::vector<DSSRWControl*> ControlVector;
class DSSCManipulationSetSelectionFromDIC : public DSSCManipulation
{
private:
	DSSRWNode* mpNode;
	DSSBaseElementsProxy *mpElements;
	DSSCExpression* mpExpression;
	bool mIsNotIn;
    DSSRWGridIterator* mpGridIterator;
    bool mIsUC;
    DSSRWSectionNode* mpRedrawParentNode;
	int mRedrawChildIndex;
    
	std::string hBuildExpressionXML();
#ifndef __ANDROID__
	void hParseExpressionFromXML(std::string& iXMLString, DSSObjectContext* ipContext);
	DSSExpNode* hBuildExpNode(SimpleXMLNode& iXMLNode, DSSExpNode* ipParentExpNode, DSSObjectContext* ipContext);
#endif	
public:
	DSSCManipulationSetSelectionFromDIC(DSSRWNode* ipNode, DSSRWControl* ipControl, DSSBaseElementsProxy *ipElements, DSSCExpression* ipExpression = NULL, bool iIsNotIn = false);
    DSSCManipulationSetSelectionFromDIC(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSCManipulationSetSelectionFromDIC();
	int Execute();
	DSSBaseElementsProxy* getElements();
    void setGridIterator(DSSRWGridIterator* ipGridIterator);
    ControlVector* getControls();
    DSSRWNode* getNode();
    DSSRWSectionNode* getRedrawParentNode();
    int getRedrawChildIndex();
    bool IsNeedServer();
    void resetAllLayouts();
    
    static int reinitDDICIterator(DSSRWControl * const ipControl);

};

inline DSSRWSectionNode* DSSCManipulationSetSelectionFromDIC::getRedrawParentNode()
{
    return mpRedrawParentNode;
}

inline int DSSCManipulationSetSelectionFromDIC::getRedrawChildIndex()
{
    return mRedrawChildIndex;
}

#endif
