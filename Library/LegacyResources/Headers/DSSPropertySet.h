/*
 *  DSSPropertySet.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-3-24.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PROPERTYSET_H
#define PROPERTYSET_H

#include <map>
#include "DSSProperty.h"

typedef std::map<std::string, DSSProperty*> PropertyMap;

class DSSPropertySet : public DSSPropertySetBaseClass
{
private:
	PropertyMap mPropertyMap;
public:
	DSSPropertySet();
	virtual ~DSSPropertySet();
	DSSPropertyBaseClass* getProperty(std::string iPropertyName);
	void InsertProperty(std::string iPropertyName, DSSProperty* ipProperty);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	void setName(std::string value);
    
    // for VI manipulation
    //std::string GenerateXML();
    PropertyMap getPropertyMap();
    void Duplicate(DSSPropertySet* ipPropertySet);
    
    std::string GenerateXML();
    void LoadDefnFromXMLNode(void* ipXMLNode);
};

inline PropertyMap DSSPropertySet::getPropertyMap()
{
    return mPropertyMap;
}

inline void DSSPropertySet::Duplicate(DSSPropertySet* ipPropertySet)
{
    if (ipPropertySet)
    {
        mPropertyMap = ipPropertySet->getPropertyMap();
    }
}
 
#endif
