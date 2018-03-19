/*
 *  DSSExpNodeShortcut.h
 *  ReportServiceCore
 *
 *  Created by dong shi on 12/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef EXPNODESHORTCUT_H
#define	EXPNODESHORTCUT_H
#import "DSSExpNode.h"

class DSSObjectInfo;
class DSSExpNodeShortcut : public DSSExpNode
{
public:
	DSSExpNodeShortcut(DSSObjectContext* ipObjectContext, const std::string& irKey, DSSCExpression* ipExpression);
	~DSSExpNodeShortcut();
	
	DSSObjectInfo* getTarget();
	void setTareget(DSSObjectInfo* ipTarget);
	void* createXML(void* ipParentNode, std::vector<DSSObjectInfo*>* ipObjects); 
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	bool IsDerived();
	void Duplicate(DSSExpNode* ipSourceNode);
	GUID getDataSourceObjectID();
    //fali, get target ID, especially for FACT, which object is not serialized to client.
    GUID getTargetID();
private:
	DSSObjectInfo* mpTarget;
	GUID mTargetID;
	GUID mDataSourceObjectID;
};




#endif
