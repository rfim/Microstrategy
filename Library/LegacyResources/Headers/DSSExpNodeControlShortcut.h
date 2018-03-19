//
//  DSSExpNodeFieldShortcut.h
//  ReportServiceCore
//
//  Created by Li, Dong on 1/7/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#ifndef EXPNODECONTROLSHORTCUT_H
#define	EXPNODECONTROLSHORTCUT_H

#import "DSSExpNode.h"

class DSSExpNodeControlShortcut: public DSSExpNode
{
public:
	DSSExpNodeControlShortcut(DSSObjectContext* ipObjectContext, EnumDSSNodeType iNodeType, const std::string& irKey, DSSCExpression* ipExpression);
	~DSSExpNodeControlShortcut();
	
    std::string getNodeKey();
    void setNodeKey(std::string iNodeKey);
    
    std::string getControlKey();
    void setControlKey(std::string iControlKey);
    
    DSSRWNode* getNode();
    DSSRWControl* getControl();
	
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	void Duplicate(DSSExpNode* ipSourceNode);
	
private:
    std::string mNodeKey;
    std::string mControlKey;
};
#endif
