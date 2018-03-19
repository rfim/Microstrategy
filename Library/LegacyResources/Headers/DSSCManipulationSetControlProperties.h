/*
 *  DSSCManipulationSetControlProperties.h
 *  ReportServiceCore
 *
 *  Created by lcao on 3/30/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CMANIPULATION_SET_CONTROL_PROPERTIES_H
#define CMANIPULATION_SET_CONTROL_PROPERTIES_H

#include <map>
#include <vector>
#include "DSSCManipulation.h"
#include "DSSRWControlNode.h"
#include "DSSAOSetObjectProperties.h"
#include "DSSCManipulationSetProperties.h"

class DSSRWControl;

class DSSCManipulationSetControlProperties : public DSSCManipulationSetProperties
{
private:
	void UpdateModel();
	void PersistProperties();
	void hParsePropertiesFromActionObject(DSSAOSetObjectProperties* ipAO);
    
    //std::string hBuildXML();
//#ifndef __ANDROID__
//    void hParseXML(std::string& iXMLString, DSSObjectContext* ipContext);
//#endif
	
public:
	DSSCManipulationSetControlProperties(DSSRWControlNode* ipControlNode, bool iAOXML=true);
	DSSCManipulationSetControlProperties(DSSActionObject* ipAO, DSSObjectContext* ipContext);
	virtual ~DSSCManipulationSetControlProperties();
	int Execute();
    
#ifndef __ANDROID__
    void GenerateAODeltaXML();
    virtual void hParseXML(std::string& iXMLString, DSSObjectContext* ipContext);
#endif
    
    DSSFormat* getFormat(int iIndex, bool iReadOnly = false);
};

#endif
