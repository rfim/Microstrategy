/*
 *  DSSExpNodeElementsObject.h
 *  ReportServiceCore
 *
 *  Created by dong shi on 12/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef EXPNODEELEMENTSOBJECT_H
#define EXPNODEELEMENTSOBJECT_H


#import "DSSExpNode.h"

class DSSBaseElementsProxy;
class DSSExpNodeElementsObject : public DSSExpNode
{
public:
	DSSExpNodeElementsObject(DSSObjectContext* ipObjectContext, const std::string& irKey, DSSCExpression* ipExpression);
	~DSSExpNodeElementsObject();
	
	void* createXML(void* ipParser, std::vector<DSSObjectInfo*>* ipObjects); 
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);	//<to be finished.
	
	DSSBaseElementsProxy* getElements();
	void setElements(DSSBaseElementsProxy* ipElements);
	
	void Duplicate(DSSExpNode* ipSourceNode); 
private:
	DSSBaseElementsProxy* mpElements; //IDSSElements
	
};
inline DSSBaseElementsProxy* DSSExpNodeElementsObject::getElements()
{
	return mpElements;
}

#endif 

