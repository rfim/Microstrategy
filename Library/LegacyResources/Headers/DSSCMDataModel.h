//
//  DSSCMDataModel.h
//  ReportServiceCore
//
//  Created by He Tengfei on 12/26/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSDataModelCacheManager_h
#define ReportServiceCore_DSSDataModelCacheManager_h

#import <map>

class DSSCMNodeModel;
class DSSRWNodeModel;

using namespace std;

class DSSCMDataModel
{
public:
    DSSCMDataModel(){}
    virtual ~DSSCMDataModel();
    
    DSSCMNodeModel * GetCacheManager(DSSRWNodeModel *ipNodeModel);    
    
    int RemoveCacheManager(DSSRWNodeModel *ipNodeModel);
    
private:

    map<DSSRWNodeModel *, DSSCMNodeModel*> mModelCacheManagers; 
};

#endif
