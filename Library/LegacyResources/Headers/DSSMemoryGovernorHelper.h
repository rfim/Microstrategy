//
//  DSSMemoryGovernorHelper.h
//  ReportServiceCore
//
//  Created by yimliu on 6/25/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#ifndef __ReportServiceCore__DSSMemoryGovernorHelper__
#define __ReportServiceCore__DSSMemoryGovernorHelper__

#include <iostream>
#include "DSSDataSource.h"

//This class is used for release memory from Memory Governor automatically when object deconstruct.
//Especially when strong pointer deconstructs, it releases memory, but fail to tell Memory Governor the memory has been released.
//In this situation, a DSSMemoryGovernorHelper should be declared right after the declaration of strong pointer.
class DSSMemoryGovernorHelper
{
private:
    DSSMemoryGovernorHelper();
    int _iSize;
    bool _bRelease;
public:
    DSSMemoryGovernorHelper(int iSize, int bRelease = true):_iSize(iSize), _bRelease(bRelease){};
    
    void GiveUpRelease(){_bRelease = false;}
    
    ~DSSMemoryGovernorHelper()
    {
        if (_bRelease) {
            MEM_GOVERNOR_RELEASE(_iSize, _iSize);
        }
    }
};

#endif /* defined(__ReportServiceCore__DSSMemoryGovernorHelper__) */
