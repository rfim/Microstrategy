//
//  DSSManipulationAddDerivedMetricToGrid.h
//  ReportServiceCore
//
//  Created by Zhu, Wei on 3/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ReportServiceCore_DSSManipulationAddDerivedMetricToGrid_h
#define ReportServiceCore_DSSManipulationAddDerivedMetricToGrid_h

#include "DSSFormat.h"
#include "DSSDataElement.h"
#include "DSSRWTemplateNode.h"
#include "DSSCManipulation.h"
#include "DSSMetric.h"

class DSSManipulationAddDerivedMetricToGrid : public DSSCManipulation
{
private:
    
	DSSRWTemplateNode* mpTemplateNode;
    DSSMetric* mNewMetric;
    DSSDataElement* mDataElement;
    std::string mFormula;
    std::string mName;
    bool mAggregateFromBase;
    std::vector<PropertyInfo*> mProperties;
    int mPos;
    
    std::string hGetUniqueName(std::string iName);
#ifndef __ANDROID__
	void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
public:
	DSSManipulationAddDerivedMetricToGrid(DSSRWTemplateNode* ipNode, int idPos, std::string iFormula, std::string iName,
                                            std::vector<PropertyInfo*> iProperties, bool iAggregateFromBase);

    DSSManipulationAddDerivedMetricToGrid(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSManipulationAddDerivedMetricToGrid();
    
	int Execute();
    void GenerateAODeltaXML();
};

#endif
