//
//  DSSAOAddDataset.h
//  ReportServiceCore
//
//  Created by Zhu, Wei on 4/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ReportServiceCore_DSSAOAddDataset_h
#define ReportServiceCore_DSSAOAddDataset_h

#include "DSSActionObject.h"

class DSSAOAddDataset : public DSSActionObject
{
private:
	std::string mDssObjectID;
	int mType;
    bool mIsDefault;
    bool mIsDatasetAdded;
    
public:
	DSSAOAddDataset();
	virtual ~DSSAOAddDataset();
	DSSAOAddDataset(EnumManipulationCommandTypes iManipulationType);
	std::string getDSSObjectID();
	void setDSSObjectID(std::string iID);
	int getType();
    void setType(int iType);
    bool getIsDefault();
    void setIsDefault(bool iIsDefault);
    bool getIsDatasetAdded();
	void setIsDatasetAdded(bool iIsDatasetAdded);
	
	std::vector<std::string> getArguments(int iWebServerType = 0);
	void setArguments(std::vector<std::string> args, int iWebServerType = 0);
};

#endif
