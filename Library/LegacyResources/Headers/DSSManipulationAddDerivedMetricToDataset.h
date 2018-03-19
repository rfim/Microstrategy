//
//  DSSManipulationAddDerivedMetricToDataset.h
//  ReportServiceCore
//
//  Created by Zhu, Wei on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ReportServiceCore_DSSManipulationAddDerivedMetricToDataset_h
#define ReportServiceCore_DSSManipulationAddDerivedMetricToDataset_h

#include "DSSFormat.h"
#include "DSSDataElement.h"
#include "DSSCManipulation.h"
#include "DSSMetric.h"

class DSSManipulationAddDerivedMetricToDataset : public DSSCManipulation
{
private:
    
    DSSMetric* mNewMetric;
    DSSDataElement* mDataElement;
    std::string mFormula;
    std::string mName;
    bool mAggregateFromBase;
    std::vector<PropertyInfo*> mProperties;
    
    std::string hGetUniqueName(std::string iName);
#ifndef __ANDROID__
	void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
public:
	DSSManipulationAddDerivedMetricToDataset(DSSDataElement* iDataElement, std::string iFormula, std::string iName,
                                          std::vector<PropertyInfo*> iProperties, bool iAggregateFromBase);
    
    DSSManipulationAddDerivedMetricToDataset(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSManipulationAddDerivedMetricToDataset();
    
	int Execute();
    void GenerateAODeltaXML();
};

#endif
