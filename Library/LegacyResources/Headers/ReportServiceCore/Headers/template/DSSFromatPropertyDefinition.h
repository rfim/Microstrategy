/*
 *  FromatPropertySetDefinition.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

/**
* Class for the default format property definition
*/

#ifndef FORMAT_PROPERTYSET_DEFINITION_H
#define FORMAT_PROPERTYSET_DEFINITION_H

#include "DSSFormatPropertySetMap.h"

typedef std::map<std::string, DSSFormatPropertySetMap*> FormatPropertySetsMap;

class DSSFromatPropertyDefinition
{
private:
	FormatPropertySetsMap mFormatPropertySetsMap;
	std::vector<std::string>* mpPropertySetNameVector;
public:
	DSSFromatPropertyDefinition();
	virtual ~DSSFromatPropertyDefinition();
	//std::string getPropertyValue(std::string iPropertySetName, std::string iPropertyName);
	std::string getPropertyNameFromID(std::string iPropertySetName, int iPropertyID);
	EnumVariantType getPropertyTypeFromID(std::string iPropertySetName, int iPropertyID);
	std::string getPropertySetNameFromID(int iPropertySetID);
	DSSFormatPropertySetMap* getFormatPropertySet(std::string iPropertySetName);
};

#endif
