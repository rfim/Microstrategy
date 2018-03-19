//
//  DSSRWViewEvaluation.cpp
//  ReportServiceCore
//
//  Created by Xiaolong Wang on 4/5/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#include "DSSRWViewEvaluation.h"
#include "DSSRWDataModel.h"

#include "DSSAEHelper.h"

DSSRWViewEvaluation::DSSRWViewEvaluation(): mpDataModel(NULL), mViewIndex(-1)
{}

DSSRWViewEvaluation::~DSSRWViewEvaluation()
{}

int DSSRWViewEvaluation::Init(DSSRWDataModel *ipDataModel, std::string iNodeKey, int iViewIndex)
{
    mpDataModel = ipDataModel;
    mNodeKey = iNodeKey;
    mViewIndex = iViewIndex;
    return S_OK;
}

int DSSRWViewEvaluation::Execute()
{
    return mpDataModel->EvaluateView(mNodeKey, mViewIndex);
}