//
//  DSSCManipulationAddShortcutMetric.h
//  ReportServiceCore
//
//  Created by Li, Dong on 3/28/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#ifndef CMANIPULATION_ADD_SHORTCUT_METRIC_H
#define CMANIPULATION_ADD_SHORTCUT_METRIC_H

#include "DSSCManipulation.h"
#include "DSSRWTemplateNode.h"
#include "DSSDataElements.h"
#include "DSSMetrics.h"
#include "DSSTemplateUnit.h"

class DSSCManipulationAddShortcutMetric: DSSCManipulation
{
public:
    DSSCManipulationAddShortcutMetric(DSSRWTemplateNode* ipTemplateNode, DSSDataElement* ipDataElement, std::vector<DSSTemplateUnit*> iUnits, std::vector<std::string>   iFunctionParameters = std::vector<std::string>(), EnumDSSFunctionType iFunctionType1 = DssFunctionTypeReserved, EnumDSSFunctionType iFunctionType2 = DssFunctionTypeReserved, std::vector<DSSSort*> iSortByUnits = std::vector<DSSSort*>(), std::vector<DSSObjectInfo*> iBreakByUnits = std::vector<DSSObjectInfo*>(), std::vector<PropertyInfo*> iProperties = std::vector<PropertyInfo*>());
    DSSCManipulationAddShortcutMetric(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationAddShortcutMetric();
    
    int Execute();
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
private:
    DSSRWTemplateNode* mpTemplateNode;
    DSSDataElement* mpDataElement;
    DSSMetric* mpNewMetric;
    std::string mName;
    int mPos;
    
    EnumDSSFunctionType mFunctionType1;
    EnumDSSFunctionType mFunctionType2;
    std::vector<std::string> mFunctionParameters;
    std::vector<DSSTemplateUnit*> mUnits;
    std::vector<DSSSort*> mSortByUnits;
    std::vector<DSSObjectInfo*> mBreakByUnits;
    std::vector<PropertyInfo*> mProperties;
    
    std::string hGetUniqueName(std::string iName);
};
#endif
