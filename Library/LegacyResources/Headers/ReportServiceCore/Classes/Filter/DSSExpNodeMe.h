//
//  DSSExpNodeMe.h
//  ReportServiceCore
//
//  Created by Li, Dong on 4/12/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#ifndef EXPNODEME_H
#define EXPNODEME_H
#import "DSSExpNode.h"

class DSSExpNodeMe: public DSSExpNode
{
public:
    DSSExpNodeMe(DSSObjectContext* ipObjectContext, EnumDSSNodeType iNodeType, const std::string& irKey, DSSCExpression* ipExpression);
    ~DSSExpNodeMe();
};
#endif
