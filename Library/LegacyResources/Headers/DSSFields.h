/*
 *  DSSFields.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-3-27.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef Fields_h
#define Fields_h

#include "DSSRWNode.h"
#include <vector>

class DSSField;

class DSSFields : public DSSSecondClassObject
{
//private:
	std::vector<DSSField*> mFields;
public:
	DSSFields();
	virtual ~DSSFields();
	DSSFields(DSSObjectContext* ipObjectContext);
	void Add(DSSField* ipField, int iIndex=-1);
	int Count();
	DSSField* Item(int iIndex);
	//bool IsTransactionEnabled();
	DSSField* FindByKey(std::string iKey);
};

#endif
