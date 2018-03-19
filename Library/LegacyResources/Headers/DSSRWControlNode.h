/*
 *  DSSRWControlNode.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef RWControlNode_h
#define RWControlNode_h

#include "DSSRWControls.h"

class DSSRWControlNode : public DSSRWNode
{
private:
	DSSRWControls* mpControls;
public:
	DSSRWControlNode();
	virtual ~DSSRWControlNode();
	DSSRWControlNode(DSSObjectContext* ipObjectContext);
	DSSRWControls* getControls();
	void setControls(DSSRWControls* ipControls);
	
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader, bool ibDelSubTree=true);
	int LoadPartialDefnFromBinary(DSSBinaryDataReader* ipReader);

	void MakeCleanDefn(bool ibDelSubTree=true);
	
	void FindOtherControlNodesControlledByThis(std::vector<DSSRWNode*>& ioOtherControlNodes);
    
    void Duplicate(DSSRWNode* ipNode);
};

#endif
