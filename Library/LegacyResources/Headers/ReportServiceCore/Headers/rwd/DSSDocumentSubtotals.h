/*
 *  DSSDocumentSubtotals.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 10-1-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef DocumentSubtotals_h
#define DocumentSubtotals_h

#include "DSSDocumentSubtotal.h"
#include <vector>

class DSSDocumentSubtotals : public DSSSecondClassObject
{
private:
	std::vector<DSSDocumentSubtotal*> mSubtotals;
public:	
	DSSDocumentSubtotals();
	virtual ~DSSDocumentSubtotals();
	DSSDocumentSubtotals(DSSObjectContext* ipObjectContext);	
	int Count();
	DSSDocumentSubtotal* Item(int iIndex);
	void Add(DSSDocumentSubtotal* iSubtotal, int iIndex=-1);
	DSSDocumentSubtotal* FindByKey(std::string iKey);
};

#endif

