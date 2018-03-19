/*
 *  DSSAttributeForm.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ATTRIBUTE_FORM_H
#define ATTRIBUTE_FORM_H

#include "DSSBaseElementsProxy.h"

class DSSAttributeForm : public DSSObjectInfo
{
private:
	std::string mUseName;
	std::string mUseDescription;
	int mBaseFormID;
	int mBaseFormType;
public:
	DSSAttributeForm();
	virtual ~DSSAttributeForm();
	DSSAttributeForm(GUID& iID, EnumDSSObjectType iType, std::string iName, DSSObjectContext* iObjectContext, GUID& iContainerID);	
	std::string getUseName();
	std::string getUseDescription();
	int getBaseFormID();
	int getBaseFormType();
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);

};

#endif
