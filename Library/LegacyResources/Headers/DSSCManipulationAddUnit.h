//
//  DSSCManipulationAddUnit.h
//  ReportServiceCore
//
//  Created by wzhu on 12/26/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef ReportServiceCore_DSSCManipulationAddUnit_h
#define ReportServiceCore_DSSCManipulationAddUnit_h


#import "GUID.h"
#import "DSSRWIterator.h"
#import "DSSCellHandle.h"
#include "DSSCManipulation.h"
#include "DSSUnitObject.h"

class DSSRWTemplateNode;
class DSSTemplateAttribute;

class DSSCManipulationAddUnit : public DSSCManipulation
{
private:    
	DSSRWTemplateNode* mpTemplateNode;
	DSSTemplateUnit* mpUnit;
    DSSTemplate* mTemplate;
    DSSObjectInfo* mObject;
    
    DSSAxis* mAxis;
    int mPos;
    DSSUnitObject* mUnitObject;

#ifndef __ANDROID__
	void hParseAOXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
public:
	DSSCManipulationAddUnit(DSSRWTemplateNode* ipNode, DSSObjectInfo* iObject, EnumDSSAxisName idAxis, int idPos);
    DSSCManipulationAddUnit(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSCManipulationAddUnit();
    
	int Execute();
    void GenerateAODeltaXML();
};

#endif
