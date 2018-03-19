//
//  DSSCMFieldGroupModel.h
//  ReportServiceCore
//
//  Created by He Tengfei on 6/17/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef __ReportServiceCore__DSSCMFieldGroupModel__
#define __ReportServiceCore__DSSCMFieldGroupModel__

#include "DSSCMNodeModel.h"

class DSSCMFieldGroupModel: public DSSCMNodeModel
{
public:
    DSSCMFieldGroupModel() {}
    virtual ~DSSCMFieldGroupModel() {}
    
    virtual int SetNodeModel(DSSRWNodeModel *ipNodeModel);
    
    virtual bool FindLocalCache();
};

#endif /* defined(__ReportServiceCore__DSSCMFieldGroupModel__) */
