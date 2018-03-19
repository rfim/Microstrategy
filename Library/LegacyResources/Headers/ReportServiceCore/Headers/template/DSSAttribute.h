/*
 *  DSSAttribute.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "DSSSorts.h"
#include <vector>

class DSSAttribute : public DSSObjectInfo
{
private:
	DSSSorts* mpSorts;
    DSSSorts* mpBrowseSorts;
	DSSAttributeFormWrappers* mpFormWrappers;
public:
	DSSAttribute();
	virtual ~DSSAttribute();
	DSSAttribute(GUID& iID, EnumDSSObjectType iType, std::string iName, DSSObjectContext* iObjectContext, GUID iContainerID);
	DSSSorts* getSorts();
	DSSAttributeFormWrappers* getFormWrappers();
    DSSAttributeFormWrapper* getIDFormWrapper();
	void getDisplayFormWrappers(DSSAttributeFormWrappers* opAttributeFormWrappers);
	void AddFormWrapper(DSSAttributeFormWrapper* iFormWrapper);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	virtual void MakeCleanDefn();
    DSSSorts* getBrowseSorts();
};

#endif
