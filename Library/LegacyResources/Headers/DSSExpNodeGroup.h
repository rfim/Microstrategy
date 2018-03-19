//
//  DSSExpNodeGroup.h
//  ReportServiceCore
//
//  Created by Li, Dong on 3/14/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef EXPNODEGROUP_H
#define EXPNODEGROUP_H

#import "DSSExpNode.h"

class DSSExpNodeGroup: public DSSExpNode
{
public:
    DSSExpNodeGroup(DSSObjectContext* ipObjectContext, EnumDSSNodeType iNodeType, const std::string& irKey, DSSCExpression* ipExpression);
    ~DSSExpNodeGroup();
    
    void removeObjects(std::vector<GUID> iObjectIDs);
    int findAndMove(DSSObjectInfo* iObject, int iNewPosition);
    int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
    // void* createXML(void* ipParentNode, std::vector<DSSObjectInfo*>* ipObjects);
};
#endif
