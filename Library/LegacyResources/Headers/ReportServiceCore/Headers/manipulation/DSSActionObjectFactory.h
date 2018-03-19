/*
 *  DSSActionObjectFactory.h
 *  ReportServiceCore
 *
 *  Created by Ji Jin on 9/7/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ACTIONOBJECTFACTORY_H
#define ACTIONOBJECTFACTORY_H

#include <vector>
#include <string>
#include "DSSrwdconstants.h"
#include "DSSActionObject.h"

class DSSActionObjectFactory
{
	
public:
	DSSActionObjectFactory();
	static DSSActionObject* getActionObject(std::vector<std::string> args);
};

#endif
