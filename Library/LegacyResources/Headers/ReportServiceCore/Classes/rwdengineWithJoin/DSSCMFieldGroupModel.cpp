//
//  DSSCMFieldGroupModel.cpp
//  ReportServiceCore
//
//  Created by He Tengfei on 6/17/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#include "DSSCMFieldGroupModel.h"

#include "DSSDataSource.h"
#include "DSSRWNodeModel.h"

int DSSCMFieldGroupModel::SetNodeModel(DSSRWNodeModel *ipNodeModel)
{
    DSSCMNodeModel::SetNodeModel(ipNodeModel);
    
    MBase::CString lFilePath = "";
    
    DSSDataSource *lpDataSource = ipNodeModel->getModelDataSource();
    if (lpDataSource) {
        lFilePath = lpDataSource->getDataSourcePath();
        
        if(lFilePath != "")
        {
            int position = lFilePath.find_last_of('/');
            
            MBase::CString lFileFolder = lFilePath.substr(0,position);
            
            MBase::CString lFileName = lFilePath.substr(position+1);
            
            if(!lFileFolder.empty() && !lFileName.empty())
                lFilePath = lFileFolder+"/"+lFileName+mpNodeModel->getNodeKey()+"_cm";
            else
                lFilePath = "";
        }
    }
    
    mLocalCachePath = lFilePath;
    
    return S_OK;
}

bool DSSCMFieldGroupModel::FindLocalCache()
{
    // tehe: for field group model, we will not keep the cache file
    return false;
}

