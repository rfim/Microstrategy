/*
 *  DSSObjectInfo.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef OBJECT_INFO_H
#define OBJECT_INFO_H

#include "DSSObjectContext.h"
//#include "DSSAccessControlEntry.h"
#include "DSSUserRuntime.h"
class DSSAccessControlEntry; // yanzli: clean up header files

class DSSObjectInfo {

private:
	//data members
	GUID mID;
	std::string mName;
	std::string mDsc;
	GUID mContainerID;
	std::vector<DSSAccessControlEntry> mACL;
	GUID mOwnerID;
	EnumDSSViewMedia mViewMedia;
	/**
	* The map from propertyset name to DSSPropertySet
	*/
protected:
    EnumDSSObjectType mType;
    DSSObjectContext* mpObjectContext;
	PropertySetsMap mPropertySetsMap;
    
    
public:
	DSSObjectInfo();
	virtual ~DSSObjectInfo();
	
	// Constructor specifically used for creating object from the object ACL table in the document drill binary 
	DSSObjectInfo(GUID& iID, GUID& iOnwerID, DSSObjectContext* iObjectContext, EnumDSSViewMedia iViewMedia = DssViewMediaReserved);
	
	DSSObjectInfo(GUID& iID, EnumDSSObjectType iType, std::string iName, DSSObjectContext* iObjectContext, GUID iContainerID=GUID_NULL, EnumDSSViewMedia iViewMedia = DssViewMediaReserved);
	GUID getID();
	EnumDSSObjectType getType();
	std::string getName();
    std::string& getNameRef();
    void setName(std::string iName);
	void setDsc(std::string iDsc);
	std::string getDsc();
	GUID getContainerID();
	DSSObjectContext* getObjectContext();
	int LoadPropertiesFromBinary(DSSBinaryDataReader* ipReader);
	void InsertPropertySet(std::string iPropertySetName, DSSPropertySetBaseClass* ipPropertySet);
	CComVariant* getPropertyValue(std::string iPropertySetName, std::string iPropertyName);
	void setPropertyValue(std::string iPropertySetName, std::string iPropertyName, std::string iPropertyValue);
	void setPropertyValue(std::string iPropertySetName, std::string iPropertyName, CComVariant& iPropertyValue);
	virtual void MakeCleanDefn();
    PropertySetsMap getPropertySetsMap();
	
	GUID getOnwerID();
	void putOwnerID(GUID& iOnwerID);
	void addACE(GUID& iTrusteeID, int iTypeAndRights);
	bool CheckAccess(DSSUserRuntime* ipUserRuntime, int iDesiredAccess, bool iRequireAll = true);
	EnumDSSViewMedia getViewMedia();
    
    std::vector<DSSAccessControlEntry> getACL();
    void Duplicate(DSSObjectInfo* ipObjectInfo);
};

inline PropertySetsMap DSSObjectInfo::getPropertySetsMap()
{
    return mPropertySetsMap;
}

#endif
