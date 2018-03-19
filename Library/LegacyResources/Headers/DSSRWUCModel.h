/*
 *  DSSRWUCModel.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 9/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef RWUCModel_H
#define RWUCModel_H

#import "DSSRWSectionModel.h"
#import "DSSTabularIndex.h"

class DSSRWUCModel : public DSSRWSectionModel
{
public:
	
	DSSRWUCModel();
	
	~DSSRWUCModel();
    
    virtual int LoadDataFromBinary(DSSBinaryDataReader *ipReadStream);
    
    /*use parent method
    int createIterator(DSSRWSectionModelIterator* ipParentIterator, DSSRWNodeModelIterator **oppIterator);
    virtual int getCandidateElements(vector<GUID> &iSelectedUnits,
                                     vector<vector<int> > &iSelectedElements,
                                     GUID iUnit, vector<int> &opElements);
    */
    virtual int GetCandidateElements(DSSRWSectionModelIterator *pParentIterator, vector<GUID>& ivSelectedUnits,
                             vector< vector<int> >& ivvSelectedElements,GUID iUnit, vector<int>& ovElements);
    
    virtual int GetCandidateElements(DSSRWSectionModelIterator *pParentIterator, vector<vector<int> > &ivvSelectedElements, vector<int> &ovElements);    
    
    //junwang, 2012/03/29
    virtual int setGroupingIndexArray(std::vector<DSSTabularIndex*>& ivIndex);
    
    void setParentOfControl(bool ifParentOfControl)
    {
        mfParentOfControl = ifParentOfControl;
    }
    
private:
    
    int genCandidateElements(DSSRWSectionModelIterator *pParentIterator, vector<GUID>& ivSelectedUnits,
                             vector< vector<int> >& ivvSelectedElements,GUID iUnit, vector<int>& ovElements);
    
    //zhyang, 793271, copy from DSSRWSectionModelIterator::hGetCurrentGBElementsUseExpr
    int hGetCurrentGBElementsUseExpr(DSSGroupByUnit *ipUnit, bool *opUseExpr, DSSBaseElementsProxy **oppElements);

    bool mfParentOfControl;
    std::vector<DSSTabularIndex*> mvIndex;
};

#endif