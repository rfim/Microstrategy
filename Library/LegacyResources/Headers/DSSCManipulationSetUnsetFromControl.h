/*
 *  DSSCManipulationSetUnsetFromControl.h
 *  ReportServiceCore
 *
 *  Created by lcao on 5/9/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CMANIPULATION_SET_UNSET_FROM_CONTROL_H
#define CMANIPULATION_SET_UNSET_FROM_CONTROL_H

#include "DSSRWControlNode.h"
#include "DSSCManipulation.h"

class DSSCManipulationSetUnsetFromControl : public DSSCManipulation
{
private:
	EnumDSSXMLRWSelectorUnsetStatus mSus;
	int mSuc;
	void Persist();
    bool mAOXML;
    DSSRWNode* mpNode;  //support both template node and control node. lcao,2013-1-7
    
    DSSRWSectionNode* mpRedrawParentNode;
	int mRedrawChildIndex;
	
public:
	DSSCManipulationSetUnsetFromControl(DSSRWNode* ipNode, EnumDSSXMLRWSelectorUnsetStatus iSus=DssXMLRWSelectorUnsetStatusDefault, int iSuc=1, bool iAOXML = true);
	DSSCManipulationSetUnsetFromControl(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSCManipulationSetUnsetFromControl();
	int Execute();
    DSSRWControlNode* getControlNode();
    DSSRWSectionNode* getRedrawParentNode();
    int getRedrawChildIndex();
    
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
};

inline DSSRWControlNode* DSSCManipulationSetUnsetFromControl::getControlNode()
{
    return mpControlNode;
}

inline DSSRWSectionNode* DSSCManipulationSetUnsetFromControl::getRedrawParentNode()
{
    return mpRedrawParentNode;
}

inline int DSSCManipulationSetUnsetFromControl::getRedrawChildIndex()
{
    return mRedrawChildIndex;
}

#endif