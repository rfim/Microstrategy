//
//  DSSNewRWDSorter.cpp
//  ReportServiceCore
//
//  Created by Yang Zhuo on 5/14/12.
//  Copyright 2012 MicroStrategy, Inc. All rights reserved.
//

#include "DSSNewRWDSorter.h"
#include "DSSSortDefinition.h"
#import "DSSSort_includes.h"

DSSNewRWDSorter::DSSNewRWDSorter(DSSXTabHeader* iHeader,vector<DSSSortDefinition*>* iSortDefinition)
{
    _header = iHeader;
    for(int i = 0; i < iSortDefinition->size(); ++i)
    {
        DSSSorter *lpSorter = (*iSortDefinition)[i]->getSorter();
        mvSorters.push_back(lpSorter);
    }
}

int DSSNewRWDSorter::sort(DSSXTabHeader* header, vector<int>& items)
{
    
    for(int i = 0; i < mvSorters.size(); ++i)
    {
        
        mvSorters[i]->count = items.size();
    }
    _xtab_sort(&items[0], &items[0] + items.size(), *this);
    return S_OK;
}

bool DSSNewRWDSorter::operator()(int a, int b)
{
    int result = 0;
    int currentBuiltSorter = -1;
    for (int i = 0; i < mvSorters.size(); ++i) 
    {
        if(currentBuiltSorter < i)
        {
            mvSorters[i]->buildBucket(_header);
        }

        result = mvSorters[i]->_buckets[a] - mvSorters[i]->_buckets[b];
        if(result != 0)
            return result < 0 ? true : false;
    }
    return false;
}