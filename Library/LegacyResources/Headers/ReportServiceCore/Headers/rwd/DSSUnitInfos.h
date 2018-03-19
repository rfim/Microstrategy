//
//  DSSUnitInfos.h
//  ReportServiceCore
//
//  Created by Zeng Jinliang on 8/1/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#ifndef __ReportServiceCore__DSSUnitInfos__
#define __ReportServiceCore__DSSUnitInfos__

#include "DSSUnitInfo.h"

class DSSUnitInfos: public DSSSecondClassObject
{
public:
	DSSUnitInfos();
	virtual ~DSSUnitInfos();
	DSSUnitInfos(DSSObjectContext* ipObjectContext);
    
	int Count();
	DSSUnitInfo* Item(int iIndex);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	void Add(DSSUnitInfo* iUnitInfo, int iPos = -1);
    void Delete(DSSUnitInfo* iUnitInfo);
	void Clear();
	
    const GUID& getDataSetRef() const { return mDataSetRef; }
	int Find(const GUID& iID) const;
    const DSSUnitInfo* getUnitInfo(const GUID& iID) const;
    const std::vector<DSSUnitInfo*>& getUnitInfos() const { return mUnitInfos; }
private:
    GUID mDataSetRef;
	std::vector<DSSUnitInfo*> mUnitInfos;
};



#endif /* defined(__ReportServiceCore__DSSUnitInfos__) */
