//
//  DSSCManipulationAddControl.h
//  ReportServiceCore
//
//  Created by Li, Dong on 1/6/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//
#ifndef CMANIPULATION_ADD_CONTROL_H
#define CMANIPULATION_ADD_CONTROL_H

#include "DSSCManipulationAddRWNode.h"

class DSSCManipulationAddControl: public DSSCManipulationAddRWNode
{
public:
    /*
    DSSCManipulationAddControl(DSSRWControlNode* ipControlNode, DSSRWControl* ipControl, std::vector<PropertyInfo*> iProperties, DSSRWSectionNode* ipParentNode);
     */
    DSSCManipulationAddControl(DSSRWControlNode* ipControlNode, DSSRWControl* ipControl, std::vector<std::string> iTargetKeys);
    DSSCManipulationAddControl(EnumDSSRWControlType iControlType, GUID iSourceID, bool iUnitCondition, bool iShowAll, std::vector<std::string>& iTargetKeys, std::vector<PropertyInfo*>& iProperties, DSSRWSectionNode* ipParentNode);
    DSSCManipulationAddControl(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationAddControl();
    
    int Execute();
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    //void AddControl();
    //void AddControlTarget(std::string iTargetKey);
    // DSSRWSectionNode* GetGroupByNode(DSSRWNode* ipSectionNode);
    DSSRWControlNode* getControlNode();
protected:
    // DSSRWControl* mpControl;
    DSSRWControlNode* mpControlNode;
    GUID mSourceID;
    bool mUnitCondition;
    bool mShowAll;
    std::vector<std::string> mTargetKeys;
    std::vector<PropertyInfo*> mProperties;
    std::vector<std::string> mGroupByNodeKeys;
};
#endif