/*
 *  DSSRWSectionModel.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 9/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef RWSectionModel_H
#define RWSectionModel_H

#import "DSSRWNodeModel.h"
#import "DSSRWDataModel.h"
#import "DSSRWNode.h"
#import "DSSBaseString.h"
#import <set>

class DSSRWNodeModelIterator;
class DSSRWSectionModelIterator;
class DSSBaseElementProxy;


class DSSRWSectionModel : public DSSRWNodeModel
{
public:
	
	DSSRWSectionModel();
	
	virtual ~DSSRWSectionModel();
    
    virtual int LoadDataFromBinary(DSSBinaryDataReader *ipReadStream);
    
    virtual int Init(DSSRWDataModel* ipDataModel, DSSRWNode* ipNode);
    
    virtual int createIterator(DSSRWSectionModelIterator* ipParentIterator, DSSRWNodeModelIterator **oppIterator);
   
    virtual int GetCandidateElements(DSSRWSectionModelIterator *pParentIterator, vector<GUID>& ivSelectedUnits,
                                     vector< vector<int> >& ivvSelectedElements,GUID iUnit, vector<int>& ovElements);
    
    /*!
	 @method     
	 @abstract  get candidate elements for current section node(CGB or UC)
	 @discussion
	 */
    virtual int GetCandidateElements(DSSRWSectionModelIterator* pParentIterator, vector<vector<int> >& 
                                     ivvSelectedElements,vector<int>& ovElements);

    /*!
	 @method     
	 @abstract  get candidate elements(represented by rowIndex in NGBPathInfo) for NGB
                different from CGB and UC which are represented by offset in lookup table
	 @discussion
	 */
    virtual int GetCandidateElements(int iSelectedRow, int iLevel, vector<int>& ovElements);
    
    
    virtual int AddSelectorLeafToTargets(const string &iTarget);
    
    EnumDSSRWSectionType GetSectionType()
    {
        return mSectionType;
    }
    
    //junwang, 4/1/2012
    virtual int setGroupingIndexArray(std::vector<DSSTabularIndex*>& ivIndex);
    virtual int setGroupingIndexInfo(vector<int> &ivSourceIndex, vector<char> ivPrimaryFlagForDatasets);

protected:
    
    EnumDSSRWSectionType mSectionType;
    int mPageByStyle;
    bool isPartial;
    vector<Int32> mvCandidateElements;
    
};

#endif