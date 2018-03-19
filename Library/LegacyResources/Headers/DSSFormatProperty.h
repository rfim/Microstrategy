/*
 *  DSSFormatProperty.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef FORMAT_PROPERTY_H
#define FORMAT_PROPERTY_H

#include "DSSSecondClassObject.h"

class DSSFormatProperty : public DSSSecondClassObject, public DSSPropertyBaseClass
{
private:
	int mPropertyID;
	std::string mPropertySetName;
public:
	DSSFormatProperty();
	virtual ~DSSFormatProperty();
	DSSFormatProperty(DSSObjectContext* ipObjectContext);
	DSSFormatProperty(DSSObjectContext* ipObjectContext, int iPropertyID, CComVariant& iValue, std::string iPropertySetName);
	std::string getName();
	VARTYPE getType();
};

#endif
