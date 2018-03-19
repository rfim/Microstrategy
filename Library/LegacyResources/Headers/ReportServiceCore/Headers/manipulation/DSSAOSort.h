/*
 *  DSSAOSort.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-29.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AOSORT_H
#define AOSORT_H

#include "DSSActionObject.h"
#include "GUID.h"

typedef std::vector<std::string> StringVector;

class DSSAOSort : public DSSActionObject
{
private:
	EnumDSSObjectType mObjectType;
	GUID mObjectID;
	GUID mFormID;
	EnumDSSObjectType mFormType;
	bool mClearSorts;
	bool mAscending;
	std::string mNodeKey;
	EnumDSSAxisName mAxisIndex;
	int mSubtotalPosition;
	StringVector mElementIDs;
	
public:
	DSSAOSort();
	virtual ~DSSAOSort();
	DSSAOSort(EnumManipulationCommandTypes iManipulationType);
	
	EnumDSSObjectType getObjectType();
	GUID getObjectID();
	GUID getFormID();
	EnumDSSObjectType getFormType();
	bool getClearSorts();
	bool getAscending();
	std::string getNodeKey();
	EnumDSSAxisName getAxisIndex();
	int getSubtotalPosition();
	StringVector* getElementIDs();
		
	void setObjectType(EnumDSSObjectType iType);
	void setObjectID(GUID iID);
	void setFormID(GUID iFormID);
	void setFormType(EnumDSSObjectType iFormType);
	void setClearSorts(bool iClearSorts);
	void setAscending(bool iAscending);
	void setNodeKey(std::string iKey);
	void setAxisIndex(EnumDSSAxisName iAxisIndex);
	void setSubtotalPosition(int iSubtotalPosition);
	void addElementID(std::string iElementID);
		
	std::vector<std::string> getArguments(int iWebServerType = 0);
	void setArguments(std::vector<std::string> args, int iWebServerType = 0);	
	void split(const std::string &s, char delim, std::vector<std::string> &elems);

};

#endif
