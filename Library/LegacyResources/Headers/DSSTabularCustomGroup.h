/*
 *  DSSTabularCustomGroup.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 5/1/10.
 *  Copyright 2010 MicroStrategy, Inc. All rights reserved.
 *
 */

#import "DSSTabularUnit.h"

typedef struct XTabCustomGroupElement {
	int				depth;
	int				parent;
	int				mType;
	int				ordinal;
	int				nodeID;
	MBase::CString	displayName;
	MBase::CString	elementID;
	bool			isLoaded;
	XTabCustomGroupElement()
	{
		depth = 0;
		parent = 0;
		mType = 0;
		ordinal = 0;
		nodeID = 0;
		displayName = "";
		elementID = "";
		isLoaded = false;
	}
}XTabCustomGroupElement;

class DSSTabularCustomGroup  : public DSSTabularUnit
{
public:
	DSSTabularCustomGroup();
	~DSSTabularCustomGroup();
	
	int Init(int index, int nElements);
    
    //ctang : change it to virtual function
	virtual int setElement(int index, XTabCustomGroupElement &irElement);
    //ctang: wrapper of get ordinal
    virtual int getOrdinal(int index);
    
	virtual XTabCustomGroupElement * getElement(int index);
	
	virtual int getMaxDepth();
	
	virtual int getParentKeys(int index, vector<int> &orParentKeys);
	
	//int getFilterID(int index);
	
	//int getBandNumber(int index);
	
	virtual int getText(int index, MBase::CString &orDisplayName);
	
	virtual const char * getTextAndLength(int index, int &orLength);
	
	virtual int CountRows();
	
	virtual int FindElement(DSSBaseElementProxy *ipElement, int &orKey);
	
	virtual int createElement(int key, DSSObjectContext *ipObjectContext, DSSBaseElementProxy **oppElement, int iGlobalKey=-1,int iGlobalKeyVer = -1);
	
	virtual DSSBaseElementProxy * getCustomGroupElement(int index);
	
	virtual int size();
    
    virtual int getParentKey(int childKey, int *parent);
    
    
    //juwnang, 04/19/2012
    virtual bool compareElement(DSSBaseElementProxy *ipElement, int iKey);
private:
	int hCreateElement(int index);
	
	vector<XTabCustomGroupElement> mvElements;
	vector<DSSBaseElementProxy *> mvpElements;
	int maxDepth;
};
