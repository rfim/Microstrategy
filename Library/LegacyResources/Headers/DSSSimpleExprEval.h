//
//  DSSSimpleExprEval.h
//  ReportServiceCore
//
//  Created by Taimin Liao on 1/24/13.
//  Copyright (c) 2013 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSSimpleExprEval_h
#define ReportServiceCore_DSSSimpleExprEval_h

#import "DSSMaskInfos.h"
#import "DSSTabularIndex.h"
#import "DSSRelation.h"
#import "DSSXTabRefLookupTable.h"
#import "DSSDataColumnSimple.h"

class DSSFunctionServer;
class DSSCube;
class DSSTabularIndex;
class DSSCalculationPlan;
class DSSGUnitContainer;
struct DSSSliceID;
class DFCSimpleExpr;
class DFCSimpleExprUnit;
class DFCSimpleExprConst;
class DFCSimpleExprRelation;
class MCESchema;
class AEFilterExprNode;
class DSSFunction;
class DFCFunc;

class DSSGroupingInfos
{
public:
	// GroupingInfo stores how a slice can be aggregated to a different level
	struct DSSGroupingInfo
	{
		vector<int> mInputLevels;		// the level of the item
		vector<int> mGroupLevels;		// the level of the group
		vector<int> mLookupLevels;      // the level of the lookup index, which is the index used to join input and output
		int *mpGroupBounds;             // the start of the vector of each group
		int *mpBlockList;               // the vectors of all groups concatenated
		int *mpBlockAddress;            // the address of each item in the group
		int mnInputRows;
		bool mUseStreamAgg;             // whether the other data structures are used for streaming aggregation
		DFCSimpleExpr* mpCondition;
	};
    
	DSSGroupingInfos(){};
	int FindGroupInfo(    DFCSimpleExpr* ipCondition,
                      bool iUseStreamAgg,
                      int* oNumInputRows,
                      DSSTabularIndex* ipInputIndex,
                      DSSTabularIndex* ipGroupIndex,
                      DSSTabularIndex* ipLookupIndex,
                      int* &opGroupBounds,
                      int* &opBlockList,
                      int* &opBlockAddress);
    
	int AddGroupInfo(    DFCSimpleExpr* ipCondition,
                     bool iUseStreamAgg,
                     int iNumInputRows,
                     DSSTabularIndex* ipInputIndex,
                     DSSTabularIndex* ipGroupIndex,
                     DSSTabularIndex* ipLookupIndex,
                     int* ipGroupBounds,
                     int* ipBlockList,
                     int* ipBlockAddress);
private:
	vector<struct DSSGroupingInfo> mvInfos;	// the stored grouping information
	int hGetIndexLevels(DSSTabularIndex* ipIndex, vector<int> &orLevel);
};

inline  int DSSGroupingInfos::FindGroupInfo(DFCSimpleExpr* ipCondition,
                                            bool iUseStreamAgg,
                                            int* oNumInputRows,
                                            DSSTabularIndex* ipInputIndex,
                                            DSSTabularIndex *ipGroupIndex,
                                            DSSTabularIndex *ipLookupIndex,
                                            int* &opGroupBounds,
                                            int* &opBlockList,
                                            int* &opBlockAddress)
{
	struct DSSGroupingInfo lInfo;
    
	opBlockList = NULL;
	opGroupBounds = NULL;
	opBlockAddress = NULL;
    
	int hr = hGetIndexLevels(ipInputIndex, lInfo.mInputLevels);
	CHECK_HR;
    
	hr = hGetIndexLevels(ipGroupIndex, lInfo.mGroupLevels);
	CHECK_HR;
    
	hr = hGetIndexLevels(ipLookupIndex, lInfo.mLookupLevels);
	CHECK_HR;
    
	lInfo.mUseStreamAgg = iUseStreamAgg;
	lInfo.mpCondition = ipCondition;
    
	// Try to find if we have such a group info already
	int i = 0;
	for (i = 0; i < mvInfos.size(); i++)
	{
        
		if (mvInfos[i].mGroupLevels == lInfo.mGroupLevels
			&& mvInfos[i].mInputLevels == lInfo.mInputLevels
			&& mvInfos[i].mLookupLevels == lInfo.mLookupLevels
			&& mvInfos[i].mUseStreamAgg == lInfo.mUseStreamAgg
			&& mvInfos[i].mpCondition == lInfo.mpCondition
			)
		{
			// We found a match. return them
			*oNumInputRows = mvInfos[i].mnInputRows;
			opGroupBounds = mvInfos[i].mpGroupBounds;
			opBlockList = mvInfos[i].mpBlockList;
			opBlockAddress = mvInfos[i].mpBlockAddress;
			break;
		}
	}
    
	return S_OK;
}

inline int DSSGroupingInfos::AddGroupInfo(     DFCSimpleExpr* ipCondition,
                                          bool iUseStreamAgg,
                                          int iNumInputRows,
                                          DSSTabularIndex* ipInputIndex,
                                          DSSTabularIndex *ipGroupIndex,
                                          DSSTabularIndex *ipLookupIndex,
                                          int* ipGroupBounds,
                                          int* ipBlockList,
                                          int* ipBlockAddress)
{
	struct DSSGroupingInfo lInfo;
    
	int hr = hGetIndexLevels(ipInputIndex, lInfo.mInputLevels);
	CHECK_HR;
    
	hr = hGetIndexLevels(ipGroupIndex, lInfo.mGroupLevels);
	CHECK_HR;
    
	hr = hGetIndexLevels(ipLookupIndex, lInfo.mLookupLevels);
	CHECK_HR;
    
	lInfo.mpCondition = ipCondition;
	lInfo.mUseStreamAgg = iUseStreamAgg;
	lInfo.mnInputRows = iNumInputRows;
	lInfo.mpGroupBounds = ipGroupBounds;
	lInfo.mpBlockList = ipBlockList;
	lInfo.mpBlockAddress = ipBlockAddress;
    
	mvInfos.push_back(lInfo);
	return S_OK;
}

inline int DSSGroupingInfos::hGetIndexLevels(DSSTabularIndex *ipIndex, std::vector<int> &orLevels)
{
	int hr = S_OK;
    
    int lnUnits = ipIndex->getNumUnits();
	CHECK_HR;
    
	orLevels.resize(lnUnits);
	if (lnUnits)
	{
		hr = ipIndex->getUnits(lnUnits, &orLevels[0]);
		CHECK_HR;
	}
    
	return S_OK;
}

class DSSSimpleExprEval
{
public:
    
    DSSSimpleExprEval(MBase::Buffer         *ipBuffer,
                      DSSFunctionServer     *ipFuncServer,
                      DSSObjectContext      *ipSource,
                      DSSCube               *ipSourceCube,
                      DSSCube               *ipTargetCube,
                      DSSCalculationPlan    *ipCalculationPlan,
                      int                   nAggLevel,
                      int                   *ipAggLevel,
                      EnumNullCheckingOption iNullChecking,
                      bool                  bIsCSI,
                      DSSGroupingInfos      *ipGroupInfos,
                      DSSMaskInfos          *ipMaskInfos,
                      bool                  bCalcHiddenDE = false,
                      bool                  bTrimString = true,
                      DSSGUnitContainer     *ipGUnitContainer = NULL,
                      // thuang@2012-11-16 for MDX or DI Attr in Metric level
                      MCESchema *ipSchema = NULL);
    
    ~DSSSimpleExprEval(){}
    
    
    int EvalExpr(DFCSimpleExpr *ipExpr, DSSSliceID* opResultID);
    
    // qjiang
    // move the filter evaluation here, so that it can be used in other projects
    //xiazhou, 04/23/2014, add support for Offline UC.
    static int CalculateMask(
                             DSSFunctionServer* ipFunctionServer,
                             DSSObjectContext* ipSource,
                             DSSCube* ipCube,					// the cube to be filtered
                             DFCSimpleExpr* ipFilter,				// the filter to be applied
                             DSSCalculationPlan* ipCalPlan,		// the calculation plan
                             DSSMaskInfos &irMaskInfos,
                             MBase::Buffer* ipBuffer,
                             map<DSS_ID, DSSGroupByUnit *> &iGBUnitMap,
                             bool ibFilterWithHiddenDE = false,
                             bool ibTrimString = true,
                             DSSGUnitContainer* ipGUnitContainer = NULL,
                             //thuang 2012-11-18 for resolving metric level which contain MDX Attrs
                             MCESchema* ipSchema = NULL,
                             bool ibEvaluateConditionalMetric = false,
                             bool isOfflineUC = false,
                             bool isDSTrx = false);
    
    // aye, 12/19/07, add a new method to construct AEFilterExpr, which supports linear scan filtering
    // Linear scan filtering avoids generating the intermediate slices in common cases.
    int GetAEFilterExpr( DFCSimpleExpr *ipExpr, AEFilterExprNode *ipRoot, bool isOfflineUC = false, bool isDSTrx = false);
    
    static int GetInputSignificantProperty(DSSFunction* ipFunc, DFCSimpleExpr* ipExpr, bool& bInputSignificant);
    
private:
    int Init();
    int hEvalExpr(DFCSimpleExpr *ipExpr,
                  DSSSliceID * opResultID,
                  AEFilterExprNode **oppNode,
                  /*ctang 2010-4-30*/bool * obIsOnColumn = NULL,
                  bool isInputSignificant = true,
                  DFCFunc* ipParentFunc = NULL,
                  //ctang 2011-2-28 relation for the relation filter case
                  DSSRelation* ipRelation = NULL,
                  GUID* opAttrID = NULL,
                  int* opAttrForm = NULL,
                  int* opAttrLocal = NULL,
                  bool IsOfflineUC = false,
                  bool IsOfflineUCNot = false,
                  bool IsDSTrx = false);
    
    int hCalculateExpr(DFCSimpleExpr *ipExpr, DSSSliceID * opResultID, AEFilterExprNode **oppNode, bool IsOfflineUC = false, bool IsOfflineUCNot = false, bool isDSTrx = false);
    int hProcessConditionalMetric(DFCSimpleExpr* ipCond, DSSCube* ipCube, int iSliceID, DSSSliceID* opSliceID = NULL);
    int hGetMetricSliceFromUnit(DFCSimpleExprUnit *ipUnit, DSSSliceID &oSliceID);
    int hGetConstSlice(DFCSimpleExprConst *ipConst, DSSSliceID * opResultID);
    int hGetAttrFormFromUnit(DFCSimpleExprUnit *ipUnit,  int &oAttrIndex, int &oAttrForm, int &oLocale, bool *opIsDE = NULL, DFCFunc* ipParentFunc = NULL, DSSRelation* ipRelation = NULL, GUID* opAttrID = NULL);
    int hGetAggLevel(DFCFunc *ipFunc, Int32 & nAggLevel, Int32 * &opAggLevel);
    int hGetRelatedElement(DSSSliceID* ipInputSliceID, DSSRelation* ipRelation, DSS_ID* ipAttrID, Int32 iForm, Int32 iLocal, Int32* opnRelatedElement, Int32** oppRelatedElement, MBase::Buffer* ipBuffer);
    int hEvaluateRelation(DFCSimpleExprRelation *ipRelation, DSSSliceID * opResultID);
    int hGetBreakbyLevel(DFCFunc *ipFunc, int & nBreakbyLevel, int * &opBreakbyLevel);
    
    int OlapSortConversion(DFCFunc *pfunc, vector<DSSSortDefinition*> *opvSortDefinitions);
    
    
    
private:
    MBase::Buffer *mpBuffer;
    DSSFunctionServer *mpFuncServer;
    DSSObjectContext *mpSource;
    
    DSSCube *mpSourceCube;
    DSSCube *mpTargetCube;
    DSSTabularData *mpTabularData;
    
    DSSCalculationPlan *mpCalculationPlan;
    
    int	mnAggLevel; //default aggregation level
    int	*mpAggLevel;
    
    EnumNullCheckingOption mNullChecking;
    
    bool mbIsCSI;	//Whether the DFCSimpleExpr is a part of CSI or Ana SQL
    DSSGroupingInfos *mpGroupingInfos;
    DSSMaskInfos *mpMaskInfos;
    
    bool mbCalcHiddenDE; //if DFCSimpleExpr is a filtering expression in CSI, DE hidden elements will be evaluated
    
    bool mbTrimString;	// whether to trim the spaces in string
    DSSGUnitContainer *mpGUnitContainer;
    
    // thuang@2012-11-16 for MDX or DI Attr in Metric level
    MCESchema *mpLocalSchema;
};

// qjiang 02/10/2011 Use a helper class to handle conditional metric
class DSSSimpleExprGetCondition
{
public:
	DSSSimpleExprGetCondition(MBase::Buffer* ipBuffer, DSSFunctionServer *ipFuncServer, DSSObjectContext* ipSource);
	~DSSSimpleExprGetCondition();
    
	int ExtractMetricAndCondition(DFCSimpleExpr* ipExpr, DFCSimpleExpr** opMetric, std::vector<DFCSimpleExpr*>& ovConditions);
    
private:
    
	int hExtractExpr(DFCSimpleExpr* ipExpr, DFCSimpleExpr** opMetric, bool ifIgnoreCond = false, bool ifCondMetric = false);
	int hExtractBranch(DFCSimpleExpr* ipExpr, DFCSimpleExpr** opMetric, bool ifIgnoreCond);
    
	void hAddCondition(DFCSimpleExpr* ipCond);
    
	std::set<DFCSimpleExpr*> mConditionSet;
	std::vector<DFCSimpleExpr*> mvCondition;
    
	MBase::Buffer *mpBuffer;
	DSSFunctionServer *mpFuncServer;
	DSSObjectContext *mpSource;
};


#endif













