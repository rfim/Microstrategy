/*
 *  DSSAOPivotUnit.h
 *  ReportServiceCore
 *
 *  Created by admin on 10/20/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AOPIVOT_UNIT_H
#define AOPIVOT_UNIT_H

#include "DSSActionObject.h"
#include "GUID.h"

typedef std::vector<std::string> StringVector;

class DSSAOPivotUnit : public DSSActionObject
{
private:
	//EnumDSSObjectType mObjectType;
	//GUID mObjectID;
	
	std::string mNodeKey;

	int dAxis;
	int oAxis;
	int dPos;
	int oPos;

public:
	DSSAOPivotUnit();
	virtual ~DSSAOPivotUnit();
	DSSAOPivotUnit(EnumManipulationCommandTypes iManipulationType);
	
	//EnumDSSObjectType getObjectType();
	//GUID getObjectID();
	
	std::string getNodeKey();
	
	//void setObjectType(EnumDSSObjectType iType);
	//void setObjectID(GUID iID);
	
	void setNodeKey(std::string iKey);
	
	int getdAxis();
	int getoAxis();
	int getdPos();
	int getoPos();
	void setdAxis(int _dAxis);
	void setoAxis(int _oAxis);
	void setdPos(int _dPos);
	void setoPos(int _oPos);


	std::vector<std::string> getArguments(int iWebServerType = 0);
	void setArguments(std::vector<std::string> args, int iWebServerType = 0);
};

#endif
