/*
 *  DSSPropertySetBaseClass.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-3-24.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PROPERTYSET_BASE_CLASS_H
#define PROPERTYSET_BASE_CLASS_H

#include "DSSPropertyBaseClass.h"

class DSSPropertySetBaseClass
{
protected:
	std::string mName;
public:
	DSSPropertySetBaseClass();
	virtual ~DSSPropertySetBaseClass();
	std::string getName();
	virtual DSSPropertyBaseClass* getProperty(std::string iName) = 0;
};

#endif
