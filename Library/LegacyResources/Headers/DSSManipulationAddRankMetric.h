//
//  DSSManipulationAddRankMetric.h
//  ReportServiceCore
//
//  Created by Zhu, Wei on 3/28/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ReportServiceCore_DSSManipulationAddRankMetric_h
#define ReportServiceCore_DSSManipulationAddRankMetric_h

#include "DSSFormat.h"
#include "DSSDataElement.h"
#include "DSSRWTemplateNode.h"
#include "DSSTemplateMetric.h"
#include "DSSObjectInfo.h"
#include "DSSCManipulation.h"
#include "DSSCellHandle.h"
#include "DSSMetric.h"


class DSSManipulationAddRankMetric : public DSSCManipulation
{
private:
    
	DSSRWTemplateNode* mpTemplateNode;
    DSSTemplateMetric* mTemplateMetric;
    DSSMetric* mNewMetric;
    DSSDataElement* mDataElement;
    std::vector<DSSObjectInfo*> mBreakByObjects;
    bool mAscending;
    
#ifndef __ANDROID__
	void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
public:
	DSSManipulationAddRankMetric(DSSRWTemplateNode* iNode, DSSCellHandle* iCellHandle, bool iAscending,  std::vector<DSSObjectInfo*> iBreakByObjects);
    
    DSSManipulationAddRankMetric(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSManipulationAddRankMetric();
    
	int Execute();
    void GenerateAODeltaXML();
};

#endif
