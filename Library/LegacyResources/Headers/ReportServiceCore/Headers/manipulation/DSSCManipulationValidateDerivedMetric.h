//
//  DSSCManipulationValidateDerivedMetric.h
//  ReportServiceCore
//
//  Created by Li, Dong on 3/30/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#ifndef CMANIPULATION_VALIDATE_DERIVED_METRIC_H
#define CMANIPULATION_VALIDATE_DERIVED_METRIC_H

#include "DSSCManipulation.h"
#include "DSSDataElement.h"

class DSSCManipulationValidateDerivedMetric: public DSSCManipulation
{
public:
    DSSCManipulationValidateDerivedMetric(std::string iFormula, DSSDataElement* ipElement);
    DSSCManipulationValidateDerivedMetric(DSSActionObject* ipAO, DSSObjectContext* ipContext);
    ~DSSCManipulationValidateDerivedMetric();
    
    int Execute();
    void GenerateAODeltaXML();
#ifndef __ANDROID__
    void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
private:
    std::string mFormula;
    DSSDataElement* mpElement;
};

#endif
