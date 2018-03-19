//
//  DSSModels.h
//  ReportServiceCore
//
//  Created by Zeng Jinliang on 8/7/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#ifndef __ReportServiceCore__DSSModels__
#define __ReportServiceCore__DSSModels__

#include "DSSModel.h"

class DSSModels: public DSSSecondClassObject
{
public:
	DSSModels();
	virtual ~DSSModels();
	DSSModels(DSSObjectContext* ipObjectContext);
    
	int Count();
	DSSModel* Item(int iIndex);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	void Clear();
    void Add(DSSModel* iModel, int iPos = -1);
	
    const std::vector<DSSModel*>& getModelSet() { return mvpModelInfo; }
private:
	std::vector<DSSModel*> mvpModelInfo;
};

#endif /* defined(__ReportServiceCore__DSSModels__) */
