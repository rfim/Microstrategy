//
//  DSSManipulationRemoveDerivedMetricFromDataset.h
//  ReportServiceCore
//
//  Created by Zhu, Wei on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ReportServiceCore_DSSManipulationRemoveDerivedMetricFromDataset_h
#define ReportServiceCore_DSSManipulationRemoveDerivedMetricFromDataset_h

#include "DSSFormat.h"
#include "DSSDataElement.h"
#include "DSSRWTemplateNode.h"
#include "DSSCManipulation.h"
#include "DSSMetric.h"

class DSSManipulationRemoveDerivedMetricFromDataset : public DSSCManipulation
{
private:
    
    DSSMetric* mMetric;
    DSSDataElement* mDataElement;
    std::string mMetricID;
    std::vector<DSSRWTemplateNode*> mTemplateNodes;
    
    void RemoveMetricFromTemplateNodes(DSSRWNode* iCurrentNode);
    
#ifndef __ANDROID__
	void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
public:
	DSSManipulationRemoveDerivedMetricFromDataset(DSSDataElement* iDataElement, std::string iMetricID);
    
    DSSManipulationRemoveDerivedMetricFromDataset(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSManipulationRemoveDerivedMetricFromDataset();
    
	int Execute();
    void GenerateAODeltaXML();
};


#endif
