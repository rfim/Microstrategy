//
//  DSSCManipulationSetGraphProperties.h
//  ReportServiceCore
//
//  Created by Li, Dong on 4/18/13.
//
//

#ifndef CMANIPULATION_SET_GRAPH_PROPERTIES_H
#define CMANIPULATION_SET_GRAPH_PROPERTIES_H

#include "DSSCManipulation.h"
#include "DSSRWTemplateNode.h"

class DSSCManipulationSetGraphProperties: public DSSCManipulation
{
public:
    DSSCManipulationSetGraphProperties(DSSRWTemplateNode* ipTemplateNode, int iMaxCategories, bool iIsPieGraph);
    DSSCManipulationSetGraphProperties(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationSetGraphProperties();
    
    int Execute();
    void GenerateAODeltaXML();
    DSSRWTemplateNode* getTemplateNode();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif

private:
    DSSRWTemplateNode* mpTemplateNode;
    int mMaxCategories;
    bool mIsPieGraph;
};

#endif
