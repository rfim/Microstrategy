/*
 *  DSSDocumentSubtotal.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 10-1-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
 
#ifndef DocumentSubtotal_h
#define DocumentSubtotal_h

#include "DSSTemplate.h"

class DSSDocumentSubtotal : public DSSSecondClassObject
{
private:
	std::string mKey;
	std::string mName;
	EnumDSSMetricType mType;
public:	
	DSSDocumentSubtotal();
	virtual ~DSSDocumentSubtotal();
	DSSDocumentSubtotal(DSSObjectContext* ipObjectContext);	
	std::string getKey();
	std::string getName();
	EnumDSSMetricType getType();
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
    void Duplicate(DSSDocumentSubtotal* ipSubtotal);
};

#endif
