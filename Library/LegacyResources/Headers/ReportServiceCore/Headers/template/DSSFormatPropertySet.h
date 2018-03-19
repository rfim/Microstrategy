/*
 *  DSSFormatPropertySet.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef FORMAT_PROPERTYSET_H
#define FORMAT_PROPERTYSET_H

#include "DSSFormatProperty.h"

typedef std::map<int, DSSFormatProperty*> FormatPropertyMap; // property id as key

class DSSFormatPropertySet : public DSSSecondClassObject, public DSSPropertySetBaseClass
{
private:
	FormatPropertyMap mPropertyMap;
	DSSFormatPropertySetMap* mpFormatPropertySetMap; //This is the definition of the default format property set.

    //FormatPropertyMap& getPropertyMap();
    
public:
    void hSetFormatProperty(int iPropertyId, DSSFormatProperty* iProperty);

public:
	DSSFormatPropertySet();
	virtual ~DSSFormatPropertySet();
	DSSFormatPropertySet(DSSObjectContext* ipObjectContext);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader, DSSFormatPropertySetMap* ipFormatPropertySetMap);
	void setName(std::string iName);
	DSSPropertyBaseClass* getProperty(std::string iPropertyName);
	DSSFormatPropertySetMap* getFormatPropertySetMap();
    FormatPropertyMap* getPropertyMap();
	void setFormatPropertySetMap(DSSFormatPropertySetMap* ipFormatPropertySetMap);
    void* GenerateXMLNode();
    
    void Duplicate(DSSFormatPropertySet* ipPropertySet);
    
#ifndef __ANDROID__
    std::string GenerateXML();
#endif
};

inline FormatPropertyMap* DSSFormatPropertySet::getPropertyMap()
{
    return &mPropertyMap;
}

#endif
