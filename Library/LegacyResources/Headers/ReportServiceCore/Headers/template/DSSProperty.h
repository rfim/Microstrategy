/*
 *  DSSProperty.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-3-19.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PROPERTY_H
#define PROPERTY_H

#include "DSSPropertySetBaseClass.h"

class DSSProperty : public DSSPropertyBaseClass
{
private:
	std::string mName;
public:
	DSSProperty();
	virtual ~DSSProperty();
	DSSProperty(std::string iName, CComVariant& iValue);
	std::string getName();
	VARTYPE getType();
};

#endif
