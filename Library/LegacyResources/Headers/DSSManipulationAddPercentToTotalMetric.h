//
//  DSSManipulationAddPercentToTotalMetric.h
//  ReportServiceCore
//
//  Created by Zhu, Wei on 3/29/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ReportServiceCore_DSSManipulationAddPercentToTotalMetric_h
#define ReportServiceCore_DSSManipulationAddPercentToTotalMetric_h

#include "DSSFormat.h"
#include "DSSDataElement.h"
#include "DSSRWTemplateNode.h"
#include "DSSTemplateMetric.h"
#include "DSSTemplateUnit.h"
#include "DSSObjectInfo.h"
#include "DSSCManipulation.h"
#include "DSSCellHandle.h"
#include "DSSMetric.h"


class DSSManipulationAddPercentToTotalMetric : public DSSCManipulation
{
private:
    
	DSSRWTemplateNode* mpTemplateNode;
    DSSTemplateMetric* mTemplateMetric;
    int mAxesBitMap;
    std::vector<DSSTemplateUnit*> mUnits;
    bool mAggregateFromBase;
    DSSMetric* mNewMetric;
    DSSDataElement* mDataElement;
    
    
#ifndef __ANDROID__
	void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
public:

	DSSManipulationAddPercentToTotalMetric(DSSRWTemplateNode* iNode, DSSCellHandle* iCellHandle, int iAxesBitMap,  std::vector<DSSTemplateUnit*> iUnits, bool iAggregateFromBase = false);
    
    DSSManipulationAddPercentToTotalMetric(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSManipulationAddPercentToTotalMetric();
    
	int Execute();
    void GenerateAODeltaXML();
};

#endif
