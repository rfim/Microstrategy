//
//  DSSRWNGBModel.h
//  ReportServiceCore
//
//  Created by junwang on R.O.C. 10/11/2011.
//  Copyright 2011 Microstrategy. All rights reserved.
//

#include "DSSRWSectionModel.h"


class DSSRWNGBModel:public  DSSRWSectionModel
{
    
    
public:
    DSSRWNGBModel();
    
    ~DSSRWNGBModel();
    
    virtual int LoadDataFromBinary(DSSBinaryDataReader *ipReadStream);
    
    virtual int Init(DSSRWDataModel* ipDataModel, DSSRWNode* ipNode);
    /* use parent method
    int createIterator(DSSRWSectionModelIterator* ipParentIterator, DSSRWNodeModelIterator **oppIterator);
    
    int GetCandidateElements(DSSRWSectionModelIterator *pParentIterator, vector<GUID>& ivSelectedUnits,
                                     vector< vector<int> >& ivvSelectedElements,GUID iUnit, vector<int>& ovElements);
    
    int GetCandidateElements(DSSRWSectionModelIterator* pParentIterator, vector<vector<int> >& 
                                     ivvSelectedElements,vector<int>& ovElements);
    
     */
    virtual int GetCandidateElements(int iSelectedRow, int iLevel, vector<int>& ovElements);
    
    int getDisplayForms(vector<int> &opDisplayForms);
    
    int getDelimeter(MBase::CString &opDelimeter);
    
protected:
    
    int mPageByStyle;
    MBase::CString mDelimeter;
    vector<int> mvDisplayForms;
    
};
