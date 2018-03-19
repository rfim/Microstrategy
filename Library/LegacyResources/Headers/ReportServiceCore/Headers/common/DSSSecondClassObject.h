/*
 *  DSSSecondClassObject.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 10-1-4.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SECOND_CLASS_OBJECT_H
#define SECOND_CLASS_OBJECT_H

#include "stddef.h"
#include "DSSObjectInfo.h"

class DSSSecondClassObject
{
protected:
	DSSObjectContext* mpObjectContext;
	
public:
	DSSSecondClassObject();
	virtual ~DSSSecondClassObject();
	DSSSecondClassObject(DSSObjectContext* ipObjectContext);
	DSSObjectContext* getObjectContext();
};

#endif
