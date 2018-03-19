/*
 *  DSSPropertyBaseClass.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-3-25.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PROPERTY_BASE_CLASS_H
#define PROPERTY_BASE_CLASS_H

#include <iostream>
#include "DSSGraphProperties.h"

class DSSPropertyBaseClass
{
protected:
	CComVariant mValue;
public:
	DSSPropertyBaseClass();
	virtual ~DSSPropertyBaseClass();
	virtual std::string getName();
	CComVariant* getValue();
	virtual VARTYPE getType();
	void setValue(CComVariant* value);
};

#endif
