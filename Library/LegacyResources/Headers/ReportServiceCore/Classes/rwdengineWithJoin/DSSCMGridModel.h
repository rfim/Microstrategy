//
//  DSSCMGridModel.h
//  ReportServiceCore
//
//  Created by He Tengfei on 6/17/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef __ReportServiceCore__DSSCMGridModel__
#define __ReportServiceCore__DSSCMGridModel__

#include "DSSCMNodeModel.h"

class DSSCMGridModel : public DSSCMNodeModel
{
public:
    DSSCMGridModel() {}
    virtual ~DSSCMGridModel() {}
    
    int SetNodeModel(DSSRWNodeModel *ipNodeModel);
    
    bool FindLocalCache();
    
    bool IsSupportLoadFromCache();
    
    int AppendMoreBlocks(int iFlags);

    int LoadFromLocalCache();
    
    int RestoreContext();
};

#endif /* defined(__ReportServiceCore__DSSCMGridModel__) */
