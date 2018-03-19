/*
 *  DSSDataElement.h
 *  ReportServiceCore
 *
 *  Created by Luo Ming on 11-1-18.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef DATA_ELEMENT_H
#define DATA_ELEMENT_H

#include "DSSUnitObjects.h"
#include "DSSMetrics.h"

class DSSViewDataSet;

typedef enum EnumDSSDatasetServeMode
{
    DssDatasetServeAsSourceAndModel = 1,
    DssDatasetServeAsModel,
    DssDatasetServeModeLast
} EnumDSSDatasetServeMode;

class DSSDataElement : public DSSSecondClassObject
{
private:
	GUID mDatasetID;
	DSSUnitObjects* mpUnitObjects;
	DSSMetrics* mpMetrics;
    
    GUID mCubeInstanceID;
    GUID mSourceTableID;
    int mXDAType;
    DSSViewDataSet* mpBaseViewDataset;
    EnumDSSDatasetServeMode mDatasetServerMode;
public:
	DSSDataElement();
	virtual ~DSSDataElement();
	DSSDataElement(GUID iID, DSSObjectContext* ipObjectContext);
	GUID getDatasetID();
    void setDatasetID(GUID iDatasetID);
	DSSUnitObjects* getUnitObjects();
	void setUnitObjects(DSSUnitObjects* ipUnitObjects);
	DSSMetrics* getMetrics();
	void setMetrics(DSSMetrics* ipMetrics);
    
    GUID getCubeInstanceID();
    void setCubeInstanceID(GUID iID);
    GUID getSrcTableID();
    void setSrcTableID(GUID iID);
    int getXDAType();
    void setXDAType(int iType);
    void setViewDataSet(DSSViewDataSet *ipDataSet);
    inline DSSViewDataSet* getViewDataSet() {return mpBaseViewDataset;};
    
    bool isDDA() const { return (mDatasetServerMode == DssDatasetServeAsModel); }
    EnumDSSDatasetServeMode getServerMode() const { return mDatasetServerMode; }
    void setServerMode(EnumDSSDatasetServeMode iServerMode) { mDatasetServerMode = iServerMode; }
};

#endif
