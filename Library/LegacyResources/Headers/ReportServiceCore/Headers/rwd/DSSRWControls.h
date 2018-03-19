/*
 *  DSSRWControls.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef RWControls_h
#define RWControls_h

#include "DSSRWControl.h"
#include <vector>

class DSSRWControls : public DSSSecondClassObject
{
private:
	std::vector<DSSRWControl*> mControls;
    DSSRWNode* mRWNode;
public:
	DSSRWControls();
	virtual ~DSSRWControls();
	DSSRWControls(DSSObjectContext* ipObjectContext);
	int Count();
	DSSRWControl* Item(int iIndex);
    void RemoveControlChild(int iIndex);
	void Add(DSSRWControl* iControl, int iIndex=-1);
	
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	int LoadPartialDefnFromBinary(DSSBinaryDataReader* ipReader);
    
    // clark, for carrying-over selections from source document to destination document when doing link drill
    DSSRWNode* getNode();
    void setNode(DSSRWNode* iRWNode);
};

#endif
