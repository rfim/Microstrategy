/*
 *  AttributeForms.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ATTRIBUTE_FORM_WRAPPERS_H
#define ATTRIBUTE_FORM_WRAPPERS_H

#include "DSSAttributeFormWrapper.h"
#include <vector>

class DSSAttributeFormWrappers : public DSSSecondClassObject
{
private:
	std::vector<DSSAttributeFormWrapper*> mForms;
public:
	DSSAttributeFormWrappers();
	virtual ~DSSAttributeFormWrappers();
	DSSAttributeFormWrappers(DSSObjectContext* ipObjectContext);
	int Count();
	DSSAttributeFormWrapper* Item(int iIndex);
	void AddForm(DSSAttributeFormWrapper* iForm, int iIndex=-1);
	void Clear();
};

#endif
