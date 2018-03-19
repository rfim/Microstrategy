//
//  DSSExpNodeGridUnitShortcut.h
//  ReportServiceCore
//
//  Created by Li, Dong on 1/9/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#ifndef EXPNODE_GRID_UNIT_SHORTCUT_H
#define	EXPNODE_GRID_UNIT_SHORTCUT_H

#import "DSSExpNode.h"

class DSSExpNodeGridUnitShortcut: public DSSExpNode
{
public:
	DSSExpNodeGridUnitShortcut(DSSObjectContext* ipObjectContext, EnumDSSNodeType iNodeType, const std::string& irKey, DSSCExpression* ipExpression);
	~DSSExpNodeGridUnitShortcut();
	
    std::string getNodeKey();
    int get_NodeKeyNS(std::string* opKey);
    void setNodeKey(std::string iNodeKey);
    DSSRWNode* getNode();
    
    GUID getTargetID();
    DSSObjectInfo* getTargetObject();
    
    GUID getFormID();
    DSSAttributeForm* getForm();
    
    EnumDSSLocale getLocale();
	
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	void Duplicate(DSSExpNode* ipSourceNode);
	
private:
    std::string mNodeKey;
    GUID mTargetID;
    GUID mFormID;
    EnumDSSLocale mLocale;
};
#endif