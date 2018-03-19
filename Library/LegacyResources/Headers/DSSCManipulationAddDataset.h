//
//  DSSCManipulationAddDataset.h
//  ReportServiceCore
//
//  Created by Zhu, Wei on 4/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ReportServiceCore_DSSCManipulationAddDataset_h
#define ReportServiceCore_DSSCManipulationAddDataset_h

#include "DSSCManipulation.h"

class DSSCManipulationAddDataset : public DSSCManipulation
{
private:
    std::string mDssObjectID;
	int mType;
    bool mIsDefault;
    bool mIsDatasetAdded;
    
public:
	DSSCManipulationAddDataset(std::string iID, int iType, bool iIsDefault, bool iIsDatasetAdded);
	virtual ~DSSCManipulationAddDataset();
	int Execute();
};


#endif
