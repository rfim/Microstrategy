//
//  DSSAnyElementProxy.h
//  ReportServiceCore
//
//  Created by Dong, Li on 3/30/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#ifndef ANY_ELEMENT_PROXY_H
#define ANY_ELEMENT_PROXY_H

#include "DSSBaseElementProxy.h"

class DSSAnyElementProxy : public DSSBaseElementProxy
{    
private:
    std::string mAnyString;
public:
	DSSAnyElementProxy();
	virtual ~DSSAnyElementProxy();
	DSSAnyElementProxy(DSSObjectContext* ipObjectContext);
	int Parse(std::string iElementID);
	bool IsEqual(DSSBaseElementProxy* ipElement);
    std::string getAnyString();
};

#endif