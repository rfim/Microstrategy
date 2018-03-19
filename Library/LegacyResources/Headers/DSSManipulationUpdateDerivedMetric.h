//
//  DSSManipulationUpdateDerivedMetric.h
//  ReportServiceCore
//
//  Created by Zhu, Wei on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ReportServiceCore_DSSManipulationUpdateDerivedMetric_h
#define ReportServiceCore_DSSManipulationUpdateDerivedMetric_h

#include "DSSFormat.h"
#include "DSSDataElement.h"
#include "DSSRWTemplateNode.h"
#include "DSSCManipulation.h"
#include "DSSMetric.h"

class DSSManipulationUpdateDerivedMetric : public DSSCManipulation
{
private:
    
    DSSMetric* mMetric;
    DSSDataElement* mDataElement;
    std::string mNewFormula;
    std::string mNewName;
    std::string mOldFormula;
    std::string mOldName;
    std::vector<DSSRWTemplateNode*> mTemplateNodes;
    
    void GetAffectedTemplateNodes(DSSRWNode* iCurrentNode);
    
#ifndef __ANDROID__
	void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
public:
	DSSManipulationUpdateDerivedMetric(DSSDataElement* iDataElement, std::string iMetricID, std::string iFormula, std::string iName);
    
    DSSManipulationUpdateDerivedMetric(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSManipulationUpdateDerivedMetric();
    
	int Execute();
    void GenerateAODeltaXML();
};


#endif
