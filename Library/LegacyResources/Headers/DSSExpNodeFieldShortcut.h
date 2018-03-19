//
//  DSSExpNodeFieldShortcut.h
//  ReportServiceCore
//
//  Created by Li, Dong on 1/7/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#ifndef EXPNODEFIELDSHORTCUT_H
#define	EXPNODEFIELDSHORTCUT_H

#import "DSSExpNode.h"

class DSSField;
class DSSExpNodeFieldShortcut: public DSSExpNode
{
public:
	DSSExpNodeFieldShortcut(DSSObjectContext* ipObjectContext, EnumDSSNodeType iNodeType, const std::string& irKey, DSSCExpression* ipExpression);
	~DSSExpNodeFieldShortcut();
	
    std::string getNodeKey();
    int get_NodeKeyNS(std::string* opKey);
    void setNodeKey(std::string iNodeKey);
    
    std::string getFieldKey();
    int get_FieldKeyNS(std::string* opKey);
    void setFieldKey(std::string iFieldKey);
    
    DSSRWNode* getNode();
    DSSField* getField();
	
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);	
	void Duplicate(DSSExpNode* ipSourceNode);
	
private:
    std::string mNodeKey;
    std::string mFieldKey;
};
#endif
