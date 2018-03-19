/*
 *  DSSManipulationSetSelectionFromControl.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-3-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CMANIPULATION_SET_SELECTION_FROM_CONTROL_H
#define CMANIPULATION_SET_SELECTION_FROM_CONTROL_H

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
#ifdef __ANDROID__
class TiXmlElement;
#endif

class DSSCManipulationSetSelectionFromControl : public DSSCManipulation
{
private:
	DSSBaseElementsProxy *mpElements;
	DSSCExpression* mpExpression;
    DSSRWControlNode* mpControlNode;
	bool mIsNotIn;
	std::string hBuildExpressionXML();
	void hParseExpressionFromXML(std::string& iXMLString, DSSObjectContext* ipContext);
    //707627, lcao 20130125
    DSSRWSectionNode* mpRedrawParentNode;
	int mRedrawChildIndex;
    bool mIsUC;
#ifndef __ANDROID__
    DSSExpNode* hBuildExpNode(SimpleXMLNode& iXMLNode, DSSExpNode* ipParentExpNode, DSSObjectContext* ipContext);
#else
	DSSExpNode* hBuildExpNode(const TiXmlElement* ipXMLNode, DSSExpNode* ipParentExpNode, DSSObjectContext* ipContext);
#endif
    
public:
	DSSCManipulationSetSelectionFromControl(DSSRWControlNode* ipControlNode, DSSBaseElementsProxy *ipElements, DSSCExpression* ipExpression = NULL, bool iIsNotIn = false);
	DSSCManipulationSetSelectionFromControl(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSCManipulationSetSelectionFromControl();
	int Execute();
    bool IsNeedServer();
	DSSBaseElementsProxy* getElements();
#ifndef __ANDROID__
    static DSSExpNode* hBuildExpNode(DSSCExpression*& ipExpression, SimpleXMLNode& iXMLNode, DSSExpNode* ipParentExpNode, DSSObjectContext* ipContext);
#endif
    DSSRWControlNode* getControlNode();
    
    DSSRWSectionNode* getRedrawParentNode();
    int getRedrawChildIndex();
	DSSRWNode* getSelectionNode();
    static void setLimitExpression(DSSRWControl* ipControl);//for offline metric condition evaluation.
    DSSCExpression* getExpression();
    void resetAllLayouts();


};

inline DSSRWControlNode* DSSCManipulationSetSelectionFromControl::getControlNode()
{
    return mpControlNode;
}
inline DSSRWSectionNode* DSSCManipulationSetSelectionFromControl::getRedrawParentNode()
{
    return mpRedrawParentNode;
}

inline int DSSCManipulationSetSelectionFromControl::getRedrawChildIndex()
{
    return mRedrawChildIndex;
}

#endif
