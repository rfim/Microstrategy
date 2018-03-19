//
//  DSSAttributeSorter.h
//  ReportServiceCore
//
//  Created by Zhuo Yang on 6/21/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSAttributeSorter_h
#define ReportServiceCore_DSSAttributeSorter_h

#include <vector>
#include "DSSSorter.h"
#import "DSSAEHelper.h"
#import "DSSDataModelBase.h"

using namespace std;

class DSSSortDefinition;
class DSSXTabHeader;

class DSSAttributeSorter : public DSSSorter
{
public:
    DSSAttributeSorter(DSSSortDefinition* definition);
	virtual ~DSSAttributeSorter();
	int sort(DSSXTabHeader* header, vector<int>& items);
private:
    int _unitID;
    int _unitIDinContainer;
    int _bucketID;
    DSSDataModelBase* _model;
    EnumDSSSubtotalsPosition _subtotalPosition;
};

#endif
