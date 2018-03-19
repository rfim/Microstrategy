/*
 *  DSSAttributeElementProxy.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ATTRIBUTE_ELEMENT_PROXY_H
#define ATTRIBUTE_ELEMENT_PROXY_H

#include "DSSMetricElementProxy.h"
#include <vector>
class DSSAttribute;
class DSSConsolidation;

class DSSAttributeElementProxy : public DSSBaseElementProxy
{
private:
	int mArity;
	int mBaseFormCount;
	std::vector<int> mBaseFormIDs;
	std::vector<EnumDSSBaseFormType> mBaseFormTypes;
	std::vector<std::string> mBaseFormValues;
    void hPopulateBaseFormTypes();
    GUID mConsolidationID;
	
public:
	DSSAttributeElementProxy();
	virtual ~DSSAttributeElementProxy();
	DSSAttributeElementProxy(DSSObjectContext* ipObjectContext);
    DSSAttributeElementProxy(DSSObjectContext* ipObjectContext, GUID iObjectID, std::vector<int>& iBaseFormIds, std::vector<EnumDSSBaseFormType>& iBaseFormTypes, std::vector<std::string>& iBaseFormValues);
    DSSAttributeElementProxy(DSSObjectContext* ipObjectContext, GUID& iObjectID, GUID& iConsoliationID, std::vector<int>& iBaseFormIds, std::vector<EnumDSSBaseFormType>& iBaseFormTypes, std::vector<std::string>& iBaseFormValues);
	int getArity();
	int getBaseFormCount();
	int Parse(std::string iElementID);
	bool IsEqual(DSSBaseElementProxy* ipElement);
    bool IsEqualTrim(DSSBaseElementProxy* ipElement);
    DSSAttribute* getAttribute();
    DSSConsolidation* getConsolidation();
	std::string getValue(int iBaseFormID);
	void Clone(DSSBaseElementProxy* ipElement);
	std::string getCompactElementID(); //supposed to be called by manipulation code only, gui/ae please do not call this function
	std::string getCompactElementIDwithDisplayText();

	std::vector<int>* getBaseFormIDs();
	std::vector<EnumDSSBaseFormType>* getBaseFormTypes();
    // weilin 2016/06/27 DE34057 check whether the terseid represents this element
    bool IsEqualToTerseID(std::string iTerseID);
    DSSAttributeElementProxy(std::string iTerseid);
};

inline std::vector<int>* DSSAttributeElementProxy::getBaseFormIDs()
{
	return &mBaseFormIDs;
}

inline std::vector<EnumDSSBaseFormType>* DSSAttributeElementProxy::getBaseFormTypes()
{
	return &mBaseFormTypes;
}

#endif
