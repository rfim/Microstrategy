/*
 *  DSSDrillPath.h
 *  ReportServiceCore
 *
 *  Created by JING LI on 4/28/10.
 *  Copyright 2010 Microstrategy Inc. All rights reserved.
 *
 */
#ifndef DRILLPATH_h
#define DRILLPATH_h 1

#include "DSSrwdconstants.h"
#include "DSSSecondClassObject.h"
#include "DSSUserRuntime.h"

class DSSBinaryDataReader;

class DSSDrillPath : public DSSSecondClassObject
{
private:
	std::string mSetName;
	std::string mPathName;
	EnumDSSDrillImportance mDrillImportance;
	EnumDSSDrillType mDrillType;
    bool mbKeepOriginal;
	
	// Upon serialization from server, the property value does not necessarily reflect the correct value as it does not take drill within cube privilege into account
	// The value is adjusted inside HasPrivilegeAndAccess invoked from DSSDocumentInstance::hPostProcessDrillBinary
	bool mDrillWithin;
	
	int mRefIndex;	// 0-based reference index for this drill path
	GUID mDrillToObjectID;	// Drill-to object ID
	std::map<GUID, bool> mDrillToMetricIDWithinMap;	// Drill-to metric IDs and their within properties for all the metric paths
	
public:	
	DSSDrillPath();
	virtual ~DSSDrillPath();
	DSSDrillPath(DSSObjectContext* ipObjectContext);
	std::string getSetName();
	std::string getPathName();
	EnumDSSDrillImportance getDrillImportance();
	EnumDSSDrillType getDrillType();
	bool getDrillWithin();
	int getRefIndex();
	void setRefIndex(int iRefIndex);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader, bool iIsLoadExtendedProperties);
	bool HasPrivilegeAndAccess(DSSUserRuntime* ipUserRuntime, bool iIsTemplateHasDE, bool iIsCubeTemplate, bool iIsCubeAllowsDrillOut);

	DSSObjectInfo* getDrillToObject(); // bxia: added for Android issue 495178
    
    bool getKeepOriginal();
    bool setKeepOriginal(bool lbKeepOriginal);
};

inline bool DSSDrillPath::getKeepOriginal()
{
    return mbKeepOriginal;
}

inline bool DSSDrillPath::setKeepOriginal(bool lbKeepOriginal)
{
    mbKeepOriginal = lbKeepOriginal;
    return true;
}

#endif
