//
//  DSSAttributeListElementProxy.h
//  ReportServiceCore
//
//  Created by Sun, Mingmin on 9/2/15.
//  Copyright (c) 2015 MicroStrategy, Inc. All rights reserved.
//

#include "DSSBaseElementProxy.h"
class DSSAttribute;

class DSSAttributeListElementProxy : public DSSBaseElementProxy
{
public:
    DSSAttributeListElementProxy();
    virtual ~DSSAttributeListElementProxy();
    DSSAttributeListElementProxy(DSSObjectContext* ipObjectContext);
    int Parse(std::string iElementID);
    DSSAttribute* getAttribute();
};