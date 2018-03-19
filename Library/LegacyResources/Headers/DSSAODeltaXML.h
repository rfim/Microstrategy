/*
 *  DSSAODeltaXML.h
 *  ReportServiceCore
 *
 *  Created by dong shi on 12/28/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

/*
 *  DSSAOSelection.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 10-4-27.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AODeltaXML_H
#define AODeltaXML_H

#include "DSSActionObject.h"
#include "GUID.h"
#include "string"

class DSSCExpression;
class DSSAODeltaXML : public DSSActionObject
{
public:
	DSSAODeltaXML();
	DSSAODeltaXML(EnumManipulationCommandTypes iManipulationType);
	~DSSAODeltaXML();
	std::vector<std::string> getArguments(int iWebServerType = 0);
	void setArguments(std::vector<std::string> args, int iWebServerType = 0);
	void setXMLString(std::string iXML);
	std::string getXML();
	void setIsNotIn(bool value);
	bool getIsNotIn();
    void appendXMLString(std::string iXML);
private:
	std::string mXML;
	bool mIsNotIn;		//2011-6-2 lcao TQMS478489
};

inline void DSSAODeltaXML::appendXMLString(std::string iXML)
{
    mXML += iXML;
}

#endif
