/*
 *  DSSFormatPropertySetMap.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

/**
 * This class contains the map from property name to property ID, type and (default) value for a given property set.
 * The map is used as the property set definition.
 */
 
#ifndef FORMAT_PROPERTYSET_MAP_H
#define FORMAT_PROPERTYSET_MAP_H
 
 #include "DSSPropertySet.h"
 
// typedef std::map<std::string, int> PropertyIDMap;
// typedef std::map<std::string, EnumVariantType> PropertyTypeMap;
// typedef std::map<std::string, std::string> PropertyValueMap;

 
 class DSSFormatPropertySetMap
 {
 private:
	 std::string mName; // propertyset name
//	 PropertyIDMap mPropertyIDMap;
//	 PropertyTypeMap mPropertyTypeMap;
//	 PropertyValueMap mPropertyValueMap;
     
     std::vector<std::string> mvPropertyNames;  //0-based
     std::vector<EnumVariantType> mvPropertyTypes;  //0-based
     std::map<std::string, int> mPropertyIDMap;
     
 public:
	 DSSFormatPropertySetMap();
	 virtual ~DSSFormatPropertySetMap();
	 void SetPropertyIDType(std::string iPropertyName, int iPropertyID, int  iPropertyType, std::string iPropertyValue=""); // iPropertyType is EnumVariantType
	 int getPropertyID(std::string iPropertyName);
	 //EnumVariantType getPropertyType(std::string iPropertyName);
     EnumVariantType getPropertyType(int iPropertyID);
	 std::string getPropertyName(int iPropertyID);
	 std::string getName();
	 void setName(std::string iName);
 };

inline void DSSFormatPropertySetMap::SetPropertyIDType(std::string iPropertyName, int iPropertyID, int iPropertyType, std::string iPropertyValue)
{
    mvPropertyNames.push_back(iPropertyName);
    mvPropertyTypes.push_back(EnumVariantType(iPropertyType));
    mPropertyIDMap[iPropertyName] = iPropertyID;
}

inline int DSSFormatPropertySetMap::getPropertyID(std::string iPropertyName)
{
    std::map<std::string, int>::iterator iter = mPropertyIDMap.find(iPropertyName);
	
	if (iter!=mPropertyIDMap.end())
		return (*iter).second;
	
	return 0;
}

/*inline EnumVariantType DSSFormatPropertySetMap::getPropertyType(std::string iPropertyName)
{
    int idx = getPropertyID(iPropertyName);
    
    if (idx>0 && idx<=mvPropertyTypes.size())
    {
        return mvPropertyTypes[idx-1];
    }
    
    return VT_EMPTY;
}*/

inline EnumVariantType DSSFormatPropertySetMap::getPropertyType(int iPropertyID)
{
    if (iPropertyID>0 && iPropertyID<=mvPropertyTypes.size())
    {
        return mvPropertyTypes[iPropertyID-1];
    }
    
    return VT_EMPTY;
}


inline std::string DSSFormatPropertySetMap::getPropertyName(int iPropertyID)
{
    if (iPropertyID>0 && iPropertyID<=mvPropertyNames.size())
        return mvPropertyNames[iPropertyID-1];
    
    return "";
}

inline std::string DSSFormatPropertySetMap::getName()
{
    return mName;
}

inline void DSSFormatPropertySetMap::setName(std::string iName)
{
    mName = iName;
}

#endif
