//
//  DSSRWViewEvaluation.h
//  ReportServiceCore
//
//  Created by Xiaolong Wang on 4/5/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef __ReportServiceCore__DSSRWViewEvaluation__
#define __ReportServiceCore__DSSRWViewEvaluation__

#include <string>

class DSSRWDataModel;

class DSSRWViewEvaluation
{
public:
    
    DSSRWViewEvaluation();
    
    ~DSSRWViewEvaluation();
    
    int Init(DSSRWDataModel * ipDataModel, std::string iNodeKey, int iViewIndex);
    
    int Execute();
    
private:
    DSSRWDataModel *mpDataModel;
//    int mNodeIndex;
    std::string mNodeKey;
    int mViewIndex;
};

#endif /* defined(__ReportServiceCore__DSSRWViewEvaluation__) */
