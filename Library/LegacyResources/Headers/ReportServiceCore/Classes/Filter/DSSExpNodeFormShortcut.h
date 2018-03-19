/*
 *  DSSExpNodeFormShortcut.h
 *  ReportServiceCore
 *
 *  Created by dong shi on 12/2/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef EXPNODEFORMSHORTCUT_H
#define	EXPNODEFORMSHORTCUT_H


#import "DSSExpNode.h"

class DSSAttribute;
class DSSAttributeForm;
class DSSExpNodeFormShortcut: public DSSExpNode
{
public:
	DSSExpNodeFormShortcut(DSSObjectContext* ipObjectContext, const std::string& irKey, DSSCExpression* ipExpression);
	~DSSExpNodeFormShortcut();
	
	DSSAttribute* getAttribute();
	void setAttribute(DSSAttribute* ipAttribute);
	
	DSSAttributeForm* getForm();
	void setForm(DSSAttributeForm* ipForm);
	
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);	
	
	void Duplicate(DSSExpNode* ipSourceNode);
	void* createXML(void* ipParentNode, std::vector<DSSObjectInfo*>* ipObjects);
	
private:
	GUID mAttributeID;
	GUID mFormID;
	DSSAttribute* mpAttribute;
	DSSAttributeForm* mpForm;
};

#endif 
