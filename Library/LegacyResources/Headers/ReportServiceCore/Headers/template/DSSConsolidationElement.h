/*
 *  DSSConsolidationElement.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CONSOLIDATION_ELEMENT_H
#define CONSOLIDATION_ELEMENT_H

#include "DSSObjectInfo.h"
class DSSFormat;
class DSSCExpression;
class DSSConsolidationElement : public DSSObjectInfo
{
private:
	int mFormatRefIDs[4];
    DSSCExpression* mpExpression;
public:
	DSSConsolidationElement();
	virtual ~DSSConsolidationElement();
	DSSConsolidationElement(GUID& iID, EnumDSSObjectType iType, std::string iName, DSSObjectContext* iObjectContext, GUID iContainerID);
	DSSFormat* getHeaderFormat();
	DSSFormat* getGridFormat();
	DSSFormat* getChildHeaderFormat();
	DSSFormat* getChildGridFormat();
    inline DSSCExpression* getExpression() {return mpExpression;}
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	virtual void MakeCleanDefn();
};

#endif
