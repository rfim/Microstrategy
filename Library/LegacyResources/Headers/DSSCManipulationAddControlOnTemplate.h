//
//  DSSCManipulationAddControlOnTemplate.h
//  ReportServiceCore
//
//  Created by Li, Dong on 12/19/12.
//
//

#ifndef CMANIPULATION_ADD_CONTROL_ON_TEMPLATE_H
#define CMANIPULATION_ADD_CONTROL_ON_TEMPLATE_H

#include "DSSCManipulation.h"
#include "DSSRWTemplateNode.h"

class DSSCManipulationAddControlOnTemplate: public DSSCManipulation
{
public:
    DSSCManipulationAddControlOnTemplate(EnumDSSRWControlType iControlType, GUID iSourceID, bool iUnitCondition, bool iShowAll, std::vector<std::string>& iTargetKeys, std::vector<PropertyInfo*>& iProperties, DSSRWTemplateNode* ipTemplateNode);
    DSSCManipulationAddControlOnTemplate(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationAddControlOnTemplate();
    
    int Execute();
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    //void AddControl();
    //void AddControlTarget(std::string iTargetKey);
    // DSSRWSectionNode* GetGroupByNode(DSSRWNode* ipSectionNode);
    DSSRWTemplateNode* getTemplateNode();
protected:
    // DSSRWControl* mpControl;
    DSSRWTemplateNode* mpTemplateNode;
    GUID mSourceID;
    bool mUnitCondition;
    bool mShowAll;
    std::vector<std::string> mTargetKeys;
    std::vector<PropertyInfo*> mProperties;
    std::vector<std::string> mGroupByNodeKeys;
};
#endif