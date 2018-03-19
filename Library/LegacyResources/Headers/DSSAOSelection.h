/*
 *  DSSAOSelection.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-27.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AOSelection_H
#define AOSelection_H

#include "DSSActionObject.h"
#include "GUID.h"

typedef std::vector<std::string> StringVector;

class DSSAOSelection : public DSSActionObject
{
private:
	EnumDSSRWTreeType mTreeType;
	std::string mNodeKey;
	std::string mControlKey;
	StringVector mElementIDs;
	StringVector mServerKeys;
	
public:
	DSSAOSelection();
	virtual ~DSSAOSelection();
	DSSAOSelection(EnumManipulationCommandTypes iManipulationType);
	EnumDSSRWTreeType getTreeType();
	std::string getNodeKey();
	std::string getControlKey();
	StringVector* getElementIDs();
	StringVector* getServerKeys();
	void setTreeType(EnumDSSRWTreeType iTreeType);
	void setNodeKey(std::string iNodeKey);
	void setControlKey(std::string iControlKey);
	void addElementID(std::string iElementID);
	void addServerKey(std::string iServerKeys);
		
	std::vector<std::string> getArguments(int iWebServerType = 0);
	void setArguments(std::vector<std::string> args, int iWebServerType = 0);	
	void split(const std::string &s, char delim, std::vector<std::string> &elems); 
};

#endif
