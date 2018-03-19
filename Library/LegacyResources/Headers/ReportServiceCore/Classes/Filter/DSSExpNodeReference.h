//
//  ExpNodeReference.h
//  ReportServiceCore
//
//  Created by Li, Dong on 3/14/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef EXPNODEREFERENCE_H
#define EXPNODEREFERENCE_H

#import "DSSExpNode.h"

class DSSExpNodeReference: public DSSExpNode
{
public:
    DSSExpNodeReference(DSSObjectContext* ipObjectContext, EnumDSSNodeType iNodeType, const std::string& irKey, DSSCExpression* ipExpression);
    ~DSSExpNodeReference();
    
    DSSObjectInfo* getTarget();
    void setTarget(DSSObjectInfo* lTarget);
    int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
    void* createXML(void* ipParentNode, std::vector<DSSObjectInfo*>* ipObjects);
    void Duplicate(DSSExpNode* ipSourceNode);
private:
    DSSObjectInfo* mTarget;
    GUID mTargetID;
};
#endif