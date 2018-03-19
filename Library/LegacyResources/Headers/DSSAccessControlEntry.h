/*
 *  DSSAccessControlEntry.h
 *  ReportServiceCore
 *
 *  Created by Wei Guo on 10/25/10.
 *  Copyright 2010 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef ACCESS_CONTROL_ENTRY_H
#define ACCESS_CONTROL_ENTRY_H

#include "GUID.h"
#include "DSSIPhoneEnums.h"

class DSSAccessControlEntry {
	
private:
	GUID mTrusteeID;
	bool mDenyAccess;
	bool mInheritable;
	int mRights;
	EnumDSSAccessEntryType mType;
	
public:
	DSSAccessControlEntry();
	DSSAccessControlEntry(GUID& irTrusteeID, bool ibDenyAccess, bool ibInheritable, int iRights, EnumDSSAccessEntryType iType);
	DSSAccessControlEntry(GUID& irTrusteeID, int iTypeAndRights);
	
	GUID getTrusteeID();
	bool getDeniesAccess();
	bool getInheritable();
	int getRights();
	EnumDSSAccessEntryType getType();
	int getTypeAndRights();
		
	void putTrusteeID(GUID& irTrusteeID );
	void putDeniesAccess(bool ibDenyAccess);
	void putInheritable(bool ibInheritable);
	void putRights(int iRights);
	void putType(EnumDSSAccessEntryType iType);
};

#endif
