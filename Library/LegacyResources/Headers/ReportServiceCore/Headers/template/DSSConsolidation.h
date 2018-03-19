/*
 *  DSSConsolidation.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CONSOLIDATION_H
#define CONSOLIDATION_H

#include "DSSObjectInfo.h"
class DSSAttributeFormWrappers;
class DSSConsolidationElement;

typedef enum EnumDSSConsolidationType
{
    DssConsolidationTypeNormal = 0,
    DssConsolidationTypeDerivedElement = 1,
    DssConsolidationTypeRecursiveDerivedElement = 2
} EnumDSSConsolidationType;

class DSSConsolidation : public DSSObjectInfo
{
private:
	std::vector<GUID> mConsolidationElements;
    EnumDSSConsolidationType mConsolidationType;
	DSSAttributeFormWrappers* mpFormWrappers;
public:
	DSSConsolidation();
	virtual ~DSSConsolidation();
	DSSConsolidation(GUID& iID, EnumDSSObjectType iType, std::string iName, DSSObjectContext* iObjectContext, GUID iContainerID);
	int Count();
	DSSConsolidationElement* Item(int i);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	virtual void MakeCleanDefn();
    EnumDSSConsolidationType getConsolidationType() { return mConsolidationType; }
	DSSAttributeFormWrappers* getFormWrappers() { return mpFormWrappers; }
};

#endif
