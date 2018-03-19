/*
 *  DSSNode.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NODE_H
#define NODE_H

#include "DSSSecondClassObject.h"
class DSSFormat;

class DSSNode : public DSSSecondClassObject
{
private:
	int mFormatRefIDs[4];
public:
	DSSNode();
	virtual ~DSSNode();
	DSSNode(DSSObjectContext* iObjectContext);
	DSSFormat* getHeaderFormat();
	DSSFormat* getGridFormat();
	DSSFormat* getChildHeaderFormat();
	DSSFormat* getChildGridFormat();
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
};

#endif
