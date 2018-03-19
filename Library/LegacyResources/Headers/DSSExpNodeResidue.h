//
//  DSSExpNodeResidue.h
//  ReportServiceCore
//
//  Created by Li, Dong on 3/14/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef EXPNODERESIDUE_H
#define EXPNODERESIDUE_H

#import "DSSExpNode.h"

class DSSExpNodeResidue: public DSSExpNode
{
public:
    DSSExpNodeResidue(DSSObjectContext* ipObjectContext, EnumDSSNodeType iNodeType, const std::string& irKey, DSSCExpression* ipExpression);
    ~DSSExpNodeResidue();
    DSSObjectInfo* getTarget();
    void setTarget(DSSObjectInfo* lAttribute);
    
    int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
    void* createXML(void* ipParentNode, std::vector<DSSObjectInfo*>* ipObjects) ;
    void Duplicate(DSSExpNode* ipSourceNode);
private:
    DSSObjectInfo* mAttribute;
    GUID mAttributeID;
};
#endif
