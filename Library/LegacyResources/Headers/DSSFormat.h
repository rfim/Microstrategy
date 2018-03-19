/*
 *  DSSFormat.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef FORMAT_H
#define FORMAT_H

#include "DSSFormatPropertySet.h"
#include <map>
#include <string>

typedef std::map<std::string, std::string> Property;

class PropertyInfo
{
public:
    int mFormatIndex;
    std::string mPropertySetName;
    std::string mPropertyName;
    CComVariant mPropertyValue;

    PropertyInfo();
    PropertyInfo(int iFormatIndex, std::string& iPropertySetName, std::string& iPropertyName, CComVariant& iPropertyValue);
    virtual ~PropertyInfo();
    //std::string getPropertyValueString();
    //static std::string getPropertyValueString(CComVariant& iPropertyValue);
    //static CComVariant& getPropertyValue(std::string iPropertyValueString, VARTYPE iType);
};

class DSSFormat : public DSSObjectInfo
{
private:
	int mIndex;
public:
	DSSFormat();
	virtual ~DSSFormat();
	DSSFormat(GUID& iID, EnumDSSObjectType iType, std::string iName, DSSObjectContext* iObjectContext, GUID& iContainerID, int iIndex=0);
    DSSFormat(int iIndex, std::vector<PropertyInfo*>* ipProperties, DSSObjectContext* ipContext);
	DSSFormat(int iIndex, Property* ipProperties, int iNumProperties, DSSObjectContext* ipContext);	//TODO:to be removed, used by other old manipulations
    int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	int getIndex();
    void setIndex(int iIndex);
    
    void* GenerateXMLNode();
    void Duplicate(DSSFormat* ipFormat);
    
    bool isContradict(DSSFormat* ipFmt);
    bool isEmpty();
    
#ifndef __ANDROID__
    std::string GenerateXML();
#endif
};

#endif
