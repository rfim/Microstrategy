//
//  DSSExpNodeBigDecimal.h
//  ReportServiceCore
//
//  Created by Li, Dong on 12/16/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef EXPNODE_BIG_DECIMAL_H
#define EXPNODE_BIG_DECIMAL_H

#include "DSSExpNode.h"

class DSSExpNodeBigDecimal: public DSSExpNode
{
public:
    DSSExpNodeBigDecimal(DSSObjectContext* ipObjectContext, EnumDSSNodeType iNodeType, const std::string& irKey, DSSCExpression* ipExpression);
    ~DSSExpNodeBigDecimal();
    std::string getBigDecimal();
    void setBigDecimal(std::string iBigDecimal);
    int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
    void Duplicate(DSSExpNode* ipSourceNode);
private:
    std::string mBigDecimal;
};
#endif