/*
 *  DSSRWFieldGroupModel.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 9/26/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef RWFieldGroupModel_H
#define RWFieldGroupModel_H

#import "DSSRWNodeModel.h"
#import "DSSViewDataSet.h"
#import "DSSTabularIndex.h"
#include "DSSRWDDICModel.h"

using MBase::operator <;

class JoinedIndexTable;
class IndexMask;
class DSSGBTable;

class DFCCSI;
using MBase::Buffer;

typedef std::set<GUID, less<GUID> > DSSIDSet;

struct TypeIDPair
{
	EnumDSSObjectType mType;
	GUID mID;
};

class TypeIDList
{
public:
	TypeIDList(){};
	~TypeIDList(){Clear();};
    
	int Add(EnumDSSObjectType iType, GUID* iID);
	int Count(Int32* opCount);
	int TypeIDByIndex(Int32 iIndex, EnumDSSObjectType* opType, GUID* opID);
	int Clear();
	int Find(EnumDSSObjectType iType, GUID* iID, Int32* oIndex);
    
    int LoadFromBinary(DSSBinaryDataReader *ipReadStream);
    
private:
	std::vector<TypeIDPair*> mList;
};

/*
 Fields in a FG node will be grouped by document datasets to create runtime templates.
 CDSSRTTemplateInfo is to enclose information needed for such purpose.
 */
class DSSRTTemplateInfo
{
public:
    DSSRTTemplateInfo();
	virtual ~DSSRTTemplateInfo();
    
	int GetReportID(GUID* opReportID);
	int HasMetrics(bool* opHasMetrics);
	int Count(Int32* opCount);
	int GetMetricByIndex(Int32 iIndex, GUID* opMetricID);
	int AddMetric(GUID MetricID);
	int GetUnits(TypeIDList** oppUnits);
    
    int LoadFromBinary(DSSBinaryDataReader *ipReadStream, DSSGUnitContainer* ipGUnitContainer, DSSCMNodeModel* ipNodeModelCacheManager);
    
    // 8/24/2012 tehe
    void hPartialCleanUp();
    inline DSSViewDataSet* getView() {return mViewPtr;}
    inline DSSViewDataSet* getOLView() {return mOLViewPtr;}
    inline DSSViewDataSet* getOLExtendView() {return mOLExtendViewPtr;}
    void CleanExtendView();

	
private:
	GUID mReportID;
	Int32 mReportIndex; // 1 based
	
    bool mbHasMetrics;
	vector<GUID> mvMetricList;  //MetricList
    
	TypeIDList* mpUnits;    // include attribute and metric units
	TypeIDList* mpOLUnits;  // Out-of-Level units
    
    vector<GUID> mvActualGroupByUnits;
    
	DSSTemplate *mpRuntimeTemplate, *mpExtRuntimeTemplate;
	DSSTemplate *mpOLRuntimeTemplate, *mpOLExtRuntimeTemplate;
    //ctang 2011-6-21
	DSSTemplate *mpBaseTemplate;
    
	//MBase::RefContainer< CComPtr<ICDSSRelation> >  mvRelations;
	DSSViewDataSet *mViewPtr, *mOLViewPtr, *mExtendViewPtr, *mOLExtendViewPtr;
	//map<Int32, ICDSSRelation* > mUnitToRelationMap;
	
	//ctang 2011-7-21 cache the extend view
	DSSCube *mpExtendCube, *mpOLExtendCube;
    //ctang GBTable
    DSSGBTable* mpGBTable;
    DSSGBTable* mpOLGBTable;
    
	friend class DSSRWFieldGroupModel;
	friend class DSSRWFieldGroupModelIterator;
	friend class DSSRWDataModel;
};
//xiazhou, 07/02/2014. DDIC related members and methods are moved to DSSRWDDICModel.
class DSSRWFieldGroupModel : public DSSRWNodeModel,public IDSSFieldGroupModel, public DSSRWDDICModel
{
public:
	DSSRWFieldGroupModel(bool ibOnDemandLoading = false);
    // 5/28/2012 tehe: on demand loading model
    DSSRWFieldGroupModel(DSSRWDataModel *ipDataModel, bool ibOnDemandLoading = false);
    
	~DSSRWFieldGroupModel();
    
	int LoadDataFromBinary(DSSBinaryDataReader *ipReadStream);
    
    virtual int createIterator(DSSRWSectionModelIterator *ipParentIterator, DSSRWNodeModelIterator **oppIterator);
    
    virtual int getCandidateElements(vector<GUID>& ipSelectedUnits,
                                     vector<vector<int> >& ipSelectedElements,
                                     GUID iUnit, vector<int>& opElements);
    
    virtual int initFromOldModel(DSSRWNodeModel* ipOldModel);
    
    // 5/28/2012 tehe
    virtual void ReleaseUnusedMemory();
    
    // 8/24/2012 tehe
    void hCleanUp(bool ibCalledByDestuctor = false);
    virtual int EnsureModelData(bool *obUpdated = NULL);
    
    //for transaction
    int setBinaryTransactionModel (unsigned char * ipBinaryModel, int iSize);
	int getBinaryTransactionModel (bool includeCommitted, unsigned char** oppBinaryModel, int* opSize);
    int EnsureTransactionData();
    virtual DSSDataModelTransaction *getTransactionDataModel(){return mpTransactionModel;};
    
    //xiazhou, for offline UC, 04/23/2014
    int clearXTabView();

    //zhyang, 757367
    int getPrimaryFlag(bool *ibPrimary);
    
    virtual bool IsEvaluatedOnClient();
    
    inline vector<DFCCSI *> getCSIs() {return mvpCSI;}
    int InitXTabView();
    inline vector<DSSRTTemplateInfo*> getRTTemplateInfos() {return mvpRTTemplateInfo;}
    
    bool IsTransactionEnabled();
    
    bool hasMultiPassCSI();
    
    EnumDSSFieldGroupDataLevel mDataLevel;
    vector<DSSRTTemplateInfo*> mvpRTTemplateInfo;
    vector<int> mvPrimaryFlag;
    
private:
    
    int hLoadDataFromCache();
    
    int hInitTabularUnitInViewDataSet();
    
    int hSliceAndJoin(vector<GUID>& ipSelectedUnits, vector<vector<int> >& ipSelectedElements,
                      bool ibUseOLViews, JoinedIndexTable** oppJoinResult,
                      bool iRetrieveSortUnits, vector<Int32>& orvSortUnits);
    
    int hCalculateMaskViaGBTable(vector<GUID>& irSelectedUnits, vector<vector<Int32> >& irSelectedElements,
                                 DSSCube* ipCube, DSSTabularIndex *ipIndex, DSSGBTable* ipGBTable, DSSXTabBitVector& orBitVector);
    
    int hInitialLoading(DSSBinaryDataReader *ipReadStream);
    int hFullBinaryLoading(DSSBinaryDataReader *ipReadStream);
    
    vector<DSSXTabView *> mvpXTabView;
    
    //zhyang,FG initial loading
    bool IsInitlaLoading;
    int mRowCount;
    int mFieldCount;
    vector<string> mvFieldText;
    vector<int> mvThresholdVectorID;
    vector<int> mvLinkTextVectorID;
    vector<int> mvThreshold;
    vector<vector<int> > mvvLinkTextType;
    vector<vector<string> > mvvLinkText;
    
    Buffer * mpBuffer;
    vector<DFCCSI *> mvpCSI;
    bool mIncludeCSI;
    
    friend class DSSRWFieldGroupModelIterator;
};

#endif