/*
 *  DSSConcreteElementProxy.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-30.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CONCRETE_ELEMENT_PROXY_H
#define CONCRETE_ELEMENT_PROXY_H

#include "DSSBaseElementProxy.h"
class DSSAttribute;
class DSSCustomGroup;
class DSSConsolidation;

class DSSConcreteElementProxy : public DSSBaseElementProxy
{
private:
	int mArity;
	int mBaseFormCount;
	std::vector<int> mBaseFormIDs;
	std::vector<EnumDSSBaseFormType> mBaseFormTypes;
	std::vector<std::string> mBaseFormValues;
	int mNodeID;
	int mBandNumber;
	GUID mAttributeID;
	
public:
	DSSConcreteElementProxy();
	virtual ~DSSConcreteElementProxy();
	DSSConcreteElementProxy(DSSObjectContext* ipObjectContext);
	DSSConcreteElementProxy(DSSObjectContext* ipObjectContext, GUID iObjectID, std::vector<int>& iBaseFormIds, std::vector<EnumDSSBaseFormType>& iBaseFormTypes, std::vector<std::string>& iBaseFormValues, int iNodeID, int iBandNumber, GUID iAttributeID);
    DSSConcreteElementProxy(DSSObjectContext* ipObjectContext, GUID iObjectID, std::vector<int>& iBaseFormIds, std::vector<EnumDSSBaseFormType>& iBaseFormTypes, std::vector<std::string>& iBaseFormValues, GUID iAttributeID);
	int getArity();
	int getBaseFormCount();
	int Parse(std::string iElementID);
	bool IsEqual(DSSBaseElementProxy* ipElement);
	DSSAttribute* getAttribute();
	std::string getValue(int iBaseFormID);
	void Clone(DSSBaseElementProxy* ipElement);
	int getNodeID();
	int getBandNumber();
	DSSCustomGroup* getCustomGroup();
    DSSConsolidation* getConsolidation();
};

#endif
