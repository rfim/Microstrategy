//
//  DSSCManipulationShowBanding.h
//  ReportServiceCore
//
//  Created by Li, Dong on 4/15/13.
//
//
#ifndef CMANIPULATION_SHOW_BANDING_H
#define CMANIPULATION_SHOW_BANDING_H

#include "DSSCManipulation.h"
#include "DSSRWTemplateNode.h"

class DSSCManipulationShowBanding: public DSSCManipulation
{
public:
    DSSCManipulationShowBanding(DSSRWTemplateNode* ipTemplateNode, bool iBandingEnabled);
    DSSCManipulationShowBanding(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationShowBanding();
    
    int Execute();
    void GenerateAODeltaXML();
    DSSRWTemplateNode* getTemplateNode();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif

private:
    DSSRWTemplateNode* mpTemplateNode;
    bool mBandingEnabled;
};
#endif
