//
//  DSSCManipulationAddDataset.cpp
//  ReportServiceCore
//
//  Created by Zhu, Wei on 4/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "DSSCManipulationAddDataset.h"
#include "DSSAOAddDataset.h"

DSSCManipulationAddDataset::DSSCManipulationAddDataset(std::string iID, int iType, bool iIsDefault, bool iIsDatasetAdded)
{
    mDssObjectID = iID;
    mType = iType;
    mIsDefault = iIsDefault;
    mIsDatasetAdded = iIsDatasetAdded;
    
    mType = ManipulationAddDataset;
}

DSSCManipulationAddDataset::~DSSCManipulationAddDataset()
{}

int DSSCManipulationAddDataset::Execute()
{
    if (mDssObjectID.size() == 0)
        return S_FALSE;
    
    mpActionObject = new DSSAOAddDataset();
    DSSAOAddDataset* lTemp = (DSSAOAddDataset*) mpActionObject;
    lTemp->setDSSObjectID(mDssObjectID);
    lTemp->setType(mType);
    lTemp->setIsDefault(mIsDefault);
    lTemp->setIsDatasetAdded(mIsDatasetAdded);
	return S_OK;
}
