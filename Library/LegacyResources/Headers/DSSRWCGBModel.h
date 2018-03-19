//
//  DSSRWCGBModel.h
//  ReportServiceCore
//
//  Created by junwang on R.O.C. 12/2/2011.
//  Copyright 2011 Microstrategy. All rights reserved.
//
#include "DSSRWSectionModel.h"


class DSSRWCGBModel: public  DSSRWSectionModel
{
    
    
public:
    DSSRWCGBModel();
    ~DSSRWCGBModel();
    /*use parent method
    int CreateIterator(DSSRWSectionModelIterator* ipParentIterator, DSSRWNodeModelIterator **oppIterator);
    int Init(DSSRWDataModel* ipDataModel, DSSRWNode* ipNode);
    virtual int getCandidateElements(vector<GUID> &iSelectedUnits,
                                     vector<vector<int> > &iSelectedElements,
                                     GUID iUnit, vector<int> &opElements);
    */
    virtual int LoadDataFromBinary(DSSBinaryDataReader *ipReadStream);
    
    virtual int GetCandidateElements(DSSRWSectionModelIterator *pParentIterator, vector<GUID>& ivSelectedUnits,
                             vector< vector<int> >& ivvSelectedElements,GUID iUnit, vector<int>& ovElements);
    virtual int GetCandidateElements(DSSRWSectionModelIterator *pParentIterator, vector<vector<int> > &ivvSelectedElements, vector<int> &ovElements);
    
    //junwang,04/01/2012, 
    virtual int setGroupingIndexArray(std::vector<DSSTabularIndex*>& ivIndex);
    
private:
    //only when this section model has a UC-selector target 
    std::vector<DSSTabularIndex*> mvIndex;
    
};
