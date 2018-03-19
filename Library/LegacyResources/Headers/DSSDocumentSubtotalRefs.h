/*
 *  DSSDocumentSubtotalRefs.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 10-1-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef DocumentSubtotalRefs_h
#define DocumentSubtotalRefs_h

#include "DSSDocumentSubtotals.h"
#include <vector>

class DSSDocumentSubtotalRefs : public DSSSecondClassObject
{
private:
	std::vector<std::string> mSubtotalKeys;
public:	
	DSSDocumentSubtotalRefs();
	virtual ~DSSDocumentSubtotalRefs();
	DSSDocumentSubtotalRefs(DSSObjectContext* ipObjectContext);	
	int Count();
	DSSDocumentSubtotal* Item(int iIndex);
	void Add(std::string iKey);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
};

#endif


