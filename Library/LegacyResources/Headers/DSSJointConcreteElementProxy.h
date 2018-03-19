/*
 *  DSSJointConcreteElementProxy.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-6-16.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef JOINT_CONCRETE_ELEMENT_PROXY_H
#define JOINT_CONCRETE_ELEMENT_PROXY_H

#include "DSSAttributeElementProxy.h"
#include <vector>

class DSSCustomGroup;

class DSSJointConcreteElementProxy : public DSSBaseElementProxy
{
private:
	int mNodeID;
	int mBandNumber;
	std::vector<DSSAttributeElementProxy*> mAttributeElements;
	
public:
	DSSJointConcreteElementProxy();
	virtual ~DSSJointConcreteElementProxy();
	DSSJointConcreteElementProxy(DSSObjectContext* ipObjectContext);
	int Parse(std::string iElementID);
	bool IsEqual(DSSBaseElementProxy* ipElement);
	std::vector<DSSAttributeElementProxy*>* getAttributeElements();
	void Clone(DSSBaseElementProxy* ipElement);
	int getNodeID();
	int getBandNumber();
	DSSCustomGroup* getCustomGroup();
};

#endif
