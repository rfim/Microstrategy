/*
 *  DSSBandElementProxy.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-30.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSBaseElementProxy.h"
class DSSCustomGroup;

class DSSBandElementProxy : public DSSBaseElementProxy
{
private:
	int mNodeID;
	int mBandNumber;
	std::string mNodeName;
	double mStart;
	double mEnd;
	double mResidue;
public:
	DSSBandElementProxy();
	virtual ~DSSBandElementProxy();
	DSSBandElementProxy(DSSObjectContext* ipObjectContext);
	DSSBandElementProxy(DSSObjectContext* ipObjectContext, GUID iObjectID, int iNodeID, int iBandNumber, std::string iNodeName, int iStart, int iEnd, int iResidue);
	int Parse(std::string iElementID);
	bool IsEqual(DSSBaseElementProxy* ipElement);
	void Clone(DSSBaseElementProxy* ipElement);
	DSSCustomGroup* getCustomGroup();
	int getNodeID();
	int getBandNumber();
	std::string getNodeName();
	double getStart();
	double getEnd();
	double getResidue();
};
