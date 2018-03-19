/*
 *  DSSUserRuntime.h
 *  ReportServiceCore
 *
 *  Created by Wei Guo on 10/25/10.
 *  Copyright 2010 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef USER_RUNTIME_H
#define USER_RUNTIME_H

#include <vector>
#include "GUID.h"
#include "DSSIPhoneEnums.h"

class DSSAccessControlEntry;

class DSSUserRuntime {
	
private:
	GUID mUserID;
	std::vector<GUID> mGroupIDs;
	std::vector<int> mPrivileges;
	
public:
	DSSUserRuntime();
	DSSUserRuntime(GUID& iUserID, std::vector<GUID>& iGroupIDs, std::vector<int>& iPrivileges);
	
	GUID getUserID();
	std::vector<GUID> getGroupIDs();
	void putUserID(GUID& iUserID);
	void addGroupID(GUID& iGroupID);
	bool hasNamedPrivilege(int iIndex);
	
	int ComputeAccess(GUID& iObjectID, EnumDSSObjectType iObjectType, GUID& iOwnerID, std::vector<DSSAccessControlEntry>& iACL);
};

#endif
