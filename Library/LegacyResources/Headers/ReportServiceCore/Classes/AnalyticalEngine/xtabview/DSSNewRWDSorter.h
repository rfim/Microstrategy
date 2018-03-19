//
//  DSSNewRWDSorter.h
//  ReportServiceCore
//
//  Created by Yang Zhuo on 5/14/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#ifndef DSSNEWRWDSORTER
#define DSSNEWRWDSORTER

#include "DSSSorter.h"

class DSSSortDefinition;

class DSSNewRWDSorter : public DSSSorter
{
public:
    DSSNewRWDSorter(DSSXTabHeader* iHeader, vector<DSSSortDefinition*>* iSortDefinition);
    virtual int sort(DSSXTabHeader* header, vector<int>& items);
    bool operator()(int a, int b);
    //int pushSorters(DSSSorter &iSorter);
private:
    vector<DSSSorter *> mvSorters;
    DSSXTabHeader* _header;
};

#endif