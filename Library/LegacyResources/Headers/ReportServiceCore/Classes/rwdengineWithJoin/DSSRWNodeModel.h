/*
 *  DSSRWNodeModel.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 9/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef RWNodeModel_H
#define RWNodeModel_H

//#import "DSSRWDataModel.h"
//#import "DSSBinaryDataReader.h"
//#import "DSSDataModelTransaction.h"
#import "DSSModelInterface.h"

#include <map>
class DSSRWNodeModelIterator;
class DSSRWSectionModelIterator;
class DSSTabularIndex;
class DSSRWNode;

class DSSTabularUnit;
class DSSViewMap;


class DSSRelation;
class DSSDataSource;
class DSSDataModelTransaction;
class DSSBinaryDataReader;
class DSSRWDataModel;

class DSSCMNodeModel;
class DSSRWControl;
class DSSXTabBitVector;

//xiazhou
class DSSGroupByUnit;

class DSSRWNodeModel : public IDSSSourceModelForTransaction
{
public:
	
	DSSRWNodeModel();
	virtual ~DSSRWNodeModel();
    
    virtual int Init(DSSRWDataModel *ipDataModel, DSSRWNode *ipNode);
    
	virtual int LoadDataFromBinary(DSSBinaryDataReader *ipReadStream);
    virtual int createIterator(DSSRWSectionModelIterator *ipParent, DSSRWNodeModelIterator **opIterator);
    virtual int getCandidateElements(vector<GUID> &iSelectedUnits,
                                     vector<vector<int> > &iSelectedElements,
                                     GUID iUnit, vector<int> &opElements);
    
    //xiazhou, for DDIC targeting. Return candidates for CGB selector
    static int getCandidateElementsDDIC(vector<GUID> &iSelectedUnits,
                                        vector<vector<int> > &iSelectedElements,
                                        GUID iUnit, DSSRWControl *ipDDICControl, DSSRWNodeModel *ipContainerNodeModel, bool iGlobalFlag, vector<int> &opElements);
    
    virtual const string & getNodeKey()
    {
        return mNodeKey;
    };
    
    int GetGroupingIndex(DSSRWSectionModelIterator *pParentIterator, DSSTabularIndex** oppGroupingIndex);
    int getNode(DSSRWNode ** ppNode);
    int get_DataModelC(DSSRWDataModel **ppDataModel);
    int setDataModel(DSSRWDataModel *ipDataModel);
    int setNodeType(EnumDSSRWNodeType iNodeType);
    int GetGroupByUnits(vector<GUID> *opUnits);
    int GetUnit(GUID *pUnit);
    int GetCandidateElementsFromForUC(DSSRWNodeModelIterator *pParentIterator,
                                      vector<Int32>& opElements, bool *obFilter);
    inline EnumDSSRWNodeType getNodeType()
    {
        return mNodeType;
    }
    
    // 12/30/2011 tehe:
    void setTransactionDataModel(DSSDataModelTransaction *ipModel);
	
    //	inline DSSDataModelTransaction *getTransactionDataModel(){return mpTransactionModel;};
    
    // 5/28/2012 tehe: release un-used memory
    virtual void ReleaseUnusedMemory();
    
    // 2/3/2012 tehe: support both schema and dataset relations, child model need rewrite this method
    virtual int hFindRelation(GUID &iUnit, DSSRelation **oppRelation, GUID *opParent, GUID *opChild);
    
    int hFindRelation(vector<DSSRelation *> &ivRelations, GUID &iUnit, DSSRelation **oppRelation, GUID *opParent, GUID *opChild);
    
    // 3/12/2012 tehe: get GB count
    inline int getGBCount() { return (int)mvGroupByUnits.size(); }
    
    //junwang, 03/29/2012
    virtual int initFromOldModel(DSSRWNodeModel* ipOldModel);
    
    int CopyTransactionDataModel(DSSRWNodeModel *ipSourceDataModel, bool ibForceCopy = false);
    
    void setNodeKey(string istrNodeKey) { mNodeKey = istrNodeKey; }
    
    // 5/28/2012 tehe
    inline void setStatus(EnumXTabStatus iStatus) { mStatus = iStatus; }
    inline EnumXTabStatus getStatus() {return mStatus; }
    inline void setModelDataSource(DSSDataSource *ipDataSource) { mpDataSource = ipDataSource; }
    inline DSSDataSource * getModelDataSource() { return mpDataSource; }
    inline void setOnDemandLoading(bool isOnDemandLoading) { mbOnDemandLoading = isOnDemandLoading; }
    inline void SetNode(DSSRWNode *ipNode) { mpNode = ipNode; }
    
    // tehe : now the grid will implement disk storage on slice level and FG will on model level, so the logic for ensure model are different
    virtual int EnsureModelData(bool *obUpdated = NULL) { return S_OK; }
    
    //junwang, 2012/05/31
    virtual int createMMapMemory();
    
	inline void setCurrentSliceFlag(int iFlag){mCurrentSliceFlag = iFlag;};
	
	inline int getCurrentSliceFlag(){return mCurrentSliceFlag;};
    
    // 9/25/2012 tehe
    inline bool IsOnTransaction() {return mbTransactionEnable; }
    void SetTransactionEnable(bool ibValue) { mbTransactionEnable = ibValue; }
    
    inline DSSRWDataModel * getDataModel() { return mpDataModel; }
    
    //TQMS731649, hyan, 3/26/2013
    inline vector<string>& getTargets(){return mvTargets;}
    inline int getTreeIndex(){return mTreeIndex;}
    
    virtual bool IsEvaluatedOnClient();
    
    // virtual int LoadPartialDataFromBinary(DSSBinaryDataReader *ipReadStream) { return S_OK;}
    
    // 4/15/2013 tehe
    inline void setBinaryContextDetailFlags(int iFlag) { mBinaryContextDetailFlags = iFlag; }
    inline int getBinaryContextDetailFlags() { return mBinaryContextDetailFlags; }
    bool IsSupportCacheManager();
    void setCMNodeModel(DSSCMNodeModel *ipNodeModelCacheManager);
    virtual int getPrimaryFlag(bool *ibPrimary){*ibPrimary = mPrimaryFlag;return S_OK;};
    virtual void ClearCMNodeModel() { /*tehe: currently only grid model need implement it*/ }
    
    //xiazhou, for offline UC. Collect UC GBUnits on this node's path.
    int collectUCUnitsOnPath(map<DSS_ID, DSSGroupByUnit *> &oGBMap);
    //junwang, 2012/03/28,
    int getParentUnitConditionNodes(DSSRWNode* ipNode, vector<DSSRWNode*>& ovpUCNodes);
    
protected:
    virtual int hInitTabularUnitInViewDataSet();
    
    HRESULT STDMETHODCALLTYPE hConvertGBUnitByRelation(vector<GUID>& lvSourceUnitID,
                                                       vector<GUID>& ovDestUnitID,
                                                       std::map<Int32, DSSRelation* >& orMap);
    
    int hConvertElements(std::map<Int32, DSSRelation* >& lrMap,
                         vector<GUID>& lvUnits,
                         vector<vector<Int32> >& lvElements,
                         vector<vector<Int32> >& ovElements);
    
    // 5/8/2012 tehe: add ibReplaceAllElement param
    int hConvertElementByRelation(DSSRelation* lpRelation,
                                  GUID& iSourceID,
                                  vector<Int32>& lElements,
                                  vector<Int32>& ovElements, bool ibReplaceAllElement=false);
    
    int hAdjustConflictGBElements(vector< vector<Int32> >&  iovGroupByElements);
    
    int getCandidateElementsFromIndexArray(vector<DSSTabularIndex*> ivIndex, vector<GUID> &iSelectedUnits, vector<vector<int> > &iSelectedElements, GUID iUnit, vector<int> &opElements, DSSRWNodeModelIterator *pParentIterator = NULL);
    
    //xiazhou, 5/23/2014. Use base dataset instead of grouping index for smart document.
    int getCandidateElementsFromBase(DSSRWNodeModelIterator *pParentIterator, vector<GUID> &iSelectedUnits, vector<vector<int> > &iSelectedElements, GUID iUnit, vector<int> &opElements);
    int filterCandidatesByParentUC(DSSRWNodeModelIterator *pParentIterator, GUID iUnit, DSSXTabBitVector &oMask, bool &obFiltered);
    int genCandidateFromBase(vector<GUID> &iSelectedUnits, vector<vector<int> > &iSelectedElements, GUID iUnit,  DSSXTabBitVector &oMask);
    
    DSSRWDataModel *mpDataModel;
    DSSRWNode *mpNode;
    GUID mUnit;
    string mNodeKey;
    EnumDSSRWNodeType mNodeType;
    bool mNullOnTop;
    vector<GUID> mvGroupByUnits;
    vector<string> mvTargets;
    
    //tehe 12/30/2011
    DSSDataModelTransaction *mpTransactionModel;
    
    //zhyang
    vector<int> mvUnitConditionNodes;
    
    // 5/23/2012 tehe
    EnumXTabStatus mStatus;
    
    DSSDataSource *mpDataSource;  // tehe :data source for model
    bool mbOnDemandLoading;
    
    // 6/14/2012 tehe
    bool mbTransactionEnable;
    
    int mCurrentSliceFlag;
    
    int mTreeIndex;
    
    int mBinaryContextDetailFlags;
    
    DSSCMNodeModel *mpNodeModelCacheManager;
    
    int mPrimaryFlag;
    
    vector<char> mPrimaryFlagForDatasets;
    
    //zhyang, move to parent class
    //zhyang, 07/09/2012, Linda's optimization
    vector<int> mvSourceIndex;
    
private:
    void hPartialCleanUp();
    friend class DSSRWSectionModelIterator;
    
};

#endif