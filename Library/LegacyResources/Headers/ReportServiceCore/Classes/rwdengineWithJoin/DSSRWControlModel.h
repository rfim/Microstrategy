/*
 *  DSSRWControlModel.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 9/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef RWControlModel_H
#define RWControlModel_H

#import "DSSRWNodeModel.h"
#import "DSSTabularIndex.h"
class DSSRWIterator;
class DSSRWControl;
class DSSRWControlModelIterator;
class DSSGlobalLookUpTable;

class DSSRWControlModel : public DSSRWNodeModel
{
public:
	
	DSSRWControlModel();
	
	~DSSRWControlModel();
	virtual int LoadDataFromBinary(DSSBinaryDataReader *ipReadStream);
    virtual int createIterator(DSSRWSectionModelIterator *ipParent, DSSRWNodeModelIterator **opIterator);

    virtual int getCandidateElements(vector<GUID> &iSelectedUnits,
                                     vector<vector<int> > &iSelectedElements,
                                     GUID iUnit, vector<int> &opElements);

    int getCandidateElementsForRefresh(vector<vector<Int32> > &ivvGroupByElements,vector<int> &opElements,DSSRWSectionModelIterator* ipParent=NULL);
    
    //junwang, 03/29/2012
    int getCandidateElements(vector<GUID> &iSelectedUnits,
                             vector<vector<int> > &iSelectedElements,
                             GUID iUnit, vector<int> &opElements, DSSRWSectionModelIterator* ipParent);
    
    int getControlType(bool *ibUC);    
    
	int setMetricRange(int iType, CComVariant *ipVal);
	
	const CComVariant * getMetricRange(int iType);
    
    int setSectionTargetGroupingIndex();
	
	//virtual void ReleaseUnusedMemory();
    
    //hyan,2011/12/14
    int ReadVariant(DSSBinaryDataReader *ipReadStream, CComVariant& rVal, int flag = 0);
    
    
    //hyan,2011/12/21
    //Add for support consolidation
    int getTabularUnitFromTarget(DSSTabularUnit **oppTabularUnit);

    //junwang, 2012/02/15
    bool hasUCTarget();
    
    //junwang, 2012/03/29
    virtual int initFromOldModel(DSSRWNodeModel *ipOldModel);
    
    //junwang, 2012/03/29
    int updateParentUCState();
    
    //junwang, 05/31/2012
    virtual int createMMapMemory();
    
    
    //hyan,2012/06/07
    inline bool getCandidateElementsFromServerFlag()
    {
        return mbCandiateElementsFromServer;
    }
    
    int getDisplayForms(vector<int> &opDisplayForms);
    
    int getDelimeter(MBase::CString &opDelimeter);
    
    vector<MBase::CString> getCandidateAttributeElement();
    
    int getRelationInfo(int iRow, int *oParent, int* oLevel, bool *oExpand, bool* oHasChild);
    bool isRecursiveUnit();
    inline void setRecursive() {mIsRecursiveUnit = true;}
    int generateRecursiveInformation(vector<int> & ivElements);
private:
    //junwang, 2012/02/17
    vector<DSSTabularIndex*> mvIndices;
    
    void hCleanUp();
	
    
    CComVariant *mpMetricValueMin;
	CComVariant *mpMetricValueMax;
	CComVariant *mpMetricValueCount;

    //this is for current slice binary data, we directly send candidateElements from server to client
	vector<int> mvCandidateElements;
    //msun,financial report
    vector<int> mvParentElements;
    vector<int> mvElementLevels;
    vector<bool> mvIsElementExpand;
    vector<bool> mvHasChildren;
    bool mIsRecursiveUnit;
    int mRowCount;
    
    vector<MBase::CString> mvCandidateAttributeElement;
    bool mIsPartialData;
    
    
    //hyan,2012/06/07
    bool mbCandiateElementsFromServer;
    
    //zhyang, 07/09/2012, Linda's optimization
    //vector<int> mvSourceIndex;
    
    MBase::CString mDelimeter;
    vector<int> mvDisplayForms;
    
    //zhyang, 804162
    //int mPrimaryFlag;
    
};

#endif