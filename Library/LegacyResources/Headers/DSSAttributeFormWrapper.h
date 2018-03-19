/*
 *  DSSAttributeFormWrapper.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ATTRIBUTE_FORM_WRAPPER_H
#define ATTRIBUTE_FORM_WRAPPER_H

#include "DSSAttributeForm.h"

class DSSAttributeFormWrapper : public DSSSecondClassObject
{
private:
	GUID mFormID;
	std::string mUseName;
	std::string mUseDescription;
	int mBaseFormID;
	int mBaseFormType;
	int mViewIndex;

	std::string mstrShapeKey;
    int mnGeographicalRole;
    
    std::vector<int> mBaseFormIDs;
    std::vector<int> mBaseFormTypes;
public:
	DSSAttributeFormWrapper();
	virtual ~DSSAttributeFormWrapper();
	DSSAttributeFormWrapper(DSSObjectContext* ipObjectContext);	
	std::string getUseName();
	std::string getUseDescription();
	int getBaseFormID();
    int getBaseFormType();
	int getViewIndex();
	GUID getID();
	EnumDSSObjectType getType();
	std::string getName();
	DSSObjectInfo* getInfo();
	int LoadDefnFromBinaryFromAttribute(DSSBinaryDataReader* ipReader);
	int LoadDefnFromBinaryFromTemplateAttribute(DSSBinaryDataReader* ipReader);
	int LoadDefnFromBinaryFromConsolidation(DSSBinaryDataReader* ipReader);
    void Duplicate(DSSAttributeFormWrapper* ipAttributeForm);
    
    std::string getShapeKey();
    int getGeographicalRole();
    
    // jzeng;11/05/15; for compound id
    std::vector<int>& getBaseFormIDs() { return mBaseFormIDs; };
    std::vector<int>& getBaseFormTypes() { return mBaseFormTypes; };
};

inline std::string DSSAttributeFormWrapper::getShapeKey()
{
    return mstrShapeKey;
}

inline int DSSAttributeFormWrapper::getGeographicalRole()
{
    return mnGeographicalRole;
}

#endif
