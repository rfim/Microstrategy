/*
 *  DSSCalculationHelper.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 1/25/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef CalculationHelper_h
#define CalculationHelper_h 1

#import "DSSAEHelper.h"
#import "DSSModelInterface.h"
#import "DSSSliceID.h"
#import "DSSCube.h"
#import "DSSDataColumn.h"

class DSSExpNode;
class DSSCube;
class DSSFunctionObject;
class DSSCubeDataSlice;
class DSSCalculationEvent;
class DSSCalculationPlan;
class DSSObjectContext;

class DSSDataModelXTabImpl;
class DSSSortDefinition;
class DSSSort;
class DSSExpNodeFormShortcut;
class DSSFunctionServer;
class DFCSchema;
class DSSMaskInfos;
class DSSExpNodeOperator;
class MCESchema;
class DSSCExpression;
class DSSMetric;
class DSSFunction;

class DSSReportInstance;
class DSSDocumentDefinition;
class DSSLevelResolver;
class DSSDataSource;
class DSSGenericFunction;
class DFCSimpleExpr;

struct EventContext
{
	DSSCalculationEvent *mpEvent;
	DSSTabularData *mpTabularData;
    IDSSTemplateModel *mpDataModel; 
    DSSViewDataSet *mpViewDataset;
	DSSCube *mpCube;
    DSSTemplate * mpTemplate;
	int mLevel;
    int mEvent;
	DSSCalculationPlan *mpCalculationPlan;
	DSSObjectContext *mpObjectContext;
    MCESchema *mpSchema;
	EventContext():mpEvent(NULL), mpDataModel(NULL), mpTabularData(NULL), mpCube(NULL), mLevel(0), mEvent(0)
	, mpCalculationPlan(NULL), mpObjectContext(NULL), mpSchema(NULL), mpTemplate(NULL), mpViewDataset(NULL){};
    
	EventContext(DSSCalculationEvent *ipEvent,
                 DSSTabularData *ipTabularData,
                 IDSSTemplateModel *ipDataModel,
                 DSSViewDataSet *ipViewDataset,
                 DSSCube *ipCube,
                 DSSTemplate * ipTemplate,
                 int iLevel,
                 int iEventIndex,
                 DSSCalculationPlan *ipCalculationPlan,
                 DSSObjectContext *ipObjectContext,
                 MCESchema *ipSchema,
                 Int32 *ipEventType = NULL,
                 GUID *ipEventID = NULL,
                 int *ipEventTag = NULL
                 );
    
    EventContext* Init(DSSCalculationEvent *ipEvent,
                       void *ipDummy,
                       DSSObjectContext *ipObjectContext,
                       DSSFunctionServer *ipFunctionServer,
                       DSSCube *ipSourceCube,
                       DSSCube *ipTargetCube,
                       DSSCalculationPlan *ipCalculationPlan,
                       int iLevel,
                       int iEvent,
                       EnumNullCheckingOption iNullCheckingOption,
                       int iEventType,
                       DSSReportInstance *ipReportInstance,
                       DSSTemplate * ipTemplate,
                       MBase::Buffer *ipBuffer,
                       int iSessionID = 0,
                       DSSDocumentDefinition *ipDeocumentDef = NULL,
                       DSSLevelResolver *ipResolveLevel = NULL,
                       DSSTabularData *ipTabularData = NULL,
                       IDSSTemplateModel *ipDataModel = NULL,
                       DSSViewDataSet *ipViewDataset = NULL,
                       MCESchema *ipSchema = NULL,
                       GUID *ipEventID = NULL,
                       int *ipEventTag = NULL
                       );
    
};

typedef struct CalculationContext
{
	DSSFunctionObject *	mpFuncObject;
	EnumSimpleOperatorType	mFuncIndex;
	EnumDSSFunctionType mFuncType;
	DSSCube *mpCube;
	vector<int>			mvBreakbyUnits;
    vector<DSSSliceID*> mvSliceID;
    vector<DSSSortDefinition*> mvSortbyDefn;
	EnumNullCheckingOption mNullCheckingOption;
    bool mfIsOnIDColumn;
	DSSMaskInfos *mpMaskInfos;
    MCESchema *mpSchema;
    bool mbDSTrx;
	CalculationContext(): mpFuncObject(NULL), mpCube(NULL), mFuncIndex(DssFuncPlus)
    ,mFuncType(DssFunctionTypeReserved), mNullCheckingOption(DssNumericErrorSkip), mpMaskInfos(NULL), mpSchema(NULL),mbDSTrx(false){};
	
	~CalculationContext();
	
}CalculationContext;

class DSSCalculationHelper
{
public:
	
	DSSCalculationHelper() {}
	
	~DSSCalculationHelper() {}
	
	static int CalculateNewSlice(CalculationContext *ipContext, DSSSliceID *opSliceID);
    static int CalculateNewSlice(DSSExpNodeOperator *ipOperator, CalculationContext *ipContext, DSSSliceID *opSliceID);
	
	static int GenerateConstantSlice(DSSExpNode * ipNode,
									 DSSCube * ipCube,
									 DSSSliceID *opSliceID);
    
    inline static int GetConstantSlice(DSSCube *ipCube, int nUnit, int *pUnit, char *pData, int cbData, EnumDSSDataType Type, EnumDSSDataFlag Flags, DSSSliceID *pSliceID);

	static int GetUnionLevel(vector<DSSCubeDataSlice*> &irvSlice, vector<int> &orvUnits);
    static int GetSliceFromID(int nSlices, DSSSliceID *ipSliceIDs, MBase::Buffer *ipBuffer, DSSCubeDataSlice ** &opSlices);
    static int CalculateIn(DSSFunctionServer    *ipFuncServer,
                           EnumDSSFunction      iFuncType,
                           DSSCube              *ipCube,
                           Int32                inSliceIDs,
                           DSSSliceID           *ipSliceIDs,
                           DSSSliceID           *opResultSlice,
                           EnumNullCheckingOption iNULLChecking,
                           MCESchema            *ipSchema = NULL,
                           bool isDSTrx = false);
    
    static int GetPropertySlice(DSSCube *ipCube, Int32 iUnit, Int32 iForm, Int32 iLocale, DSSSliceID *pSliceID, bool iFailDE, bool iLevelSignificant=true, DSSGUnitContainer* ipContainer=NULL);
    
    static int ApplyRelationshipFilter( Int32              nDimUnits, 
                                        Int32              *ipUnitIDs, 
                                        DSSCubeDataSlice   *ipFilterSlice, 
                                        DSSCube            *ipTargetCube,
                                        DSSSliceID         *opResultSliceID);
    static int StrechSliceDownward(DSSCube * ipCube,
                            MBase::Buffer * ipBuffer,
                            DSSCubeDataSlice *pSliceIn, 
                            Int32 nInputLevel, Int32 *ipInputLevel,
                            Int32 nTargetLevel, Int32 *ipTargetLevel,
                            DSSCubeDataSlice ** ppSliceOut);
    static int CalculateAndOR(DSSFunctionObject * ipFuncObj,
                            Int32 nSliceID, DSSSliceID *ipSliceID,
                            DSSCube *ipTargetCube, 
                            EnumNullCheckingOption iNULLChecking,
                            bool bCalcHiddenDE,
                            DSSSliceID *opSliceID,
                            MCESchema *ipSchema=NULL);
    static int FindCommonLevel(Int32 nSliceIn, DSSSliceID *pSliceIDIn, 
                               MBase::Buffer *ipBuffer,vector<Int32, MBase::Allocator<Int32> > &ovUnits);

    static int GenerateAttributeFormInputs(DSSCube *ipCube, DSSExpNodeFormShortcut *ipAttrFormShortcut, DSSSliceID *iSliceID, bool iLevelSignificant=true);
    
    static int LinearFindRowIndexByCellData(XTabCellData* ipKey,  DSSDataColumn* ipColumn, Int32 iStartRow, bool iUseVariant);
    
    static int FindRowIndexByCellData(XTabCellData* ipKeys, DSSDataColumn** ippColumns, Int32 inKeys, 
                                       Int32 inRows, bool iUseVariant, Int32* opIndex=NULL, Int32 iStartIndex=0, bool ibTrimString=true);
    
    static int FindUnitType(DSSTabularData *ipTabularData, DSS_ID iID, vector<DSSCubeDataSlice*> &irvpSlices, DSSTemplateUnit_Type *opType, Int32 *opUnit);
    static int FindMetricBreakbyLevel(DSSExpNode *ipNode, DSSTabularData *ipTabularData, vector<DSSCubeDataSlice*> &irvpSlices, vector<int> *opvUnits);
    
    static int FindMetricSortbyDefns(DSSExpNode *ipNode, EventContext *iEventContext, int inSlices, int *ipInSliceIDs, DSSMetric *ipDimMetricGov, vector<DSSSortDefinition*>**oppSortDefinition);
    
    static CalculationContext* getCalculationContext(DSSFunctionObject          *ipFuncObject,
                                                     EnumSimpleOperatorType     iFuncIndex,
                                                     DSSCube                    *ipCube,
                                                     vector<int>                *ipvBreakbyUnits,
                                                     vector<DSSSliceID*>        *ipvSliceID,
                                                     EnumNullCheckingOption     iNullCheckingOption,
                                                     MCESchema                  *ipSchema = NULL,
                                                     DSSMaskInfos               *ipMaskInfos = NULL,
                                                     vector<DSSSortDefinition*> *ipvSortbyDefn = NULL,
                                                     bool                       ifIsOnIDColumn = false,
                                                     bool                       isDSTrx = false);
    static int ChooseAggregationFunction(DSSObjectContext    *ipObjContext,
                                         DSSFunctionServer   *ipFunctionServer,
                                         METRIC_ID           iMetricID,
                                         DSSFunctionObject   **oppFunctionObject,
                                         Int32               *opFunctionIndex,
                                         VARIANT_BOOL        *opIsOrderSignificant,
                                         DSSExpNode          **oppExprRoot,
                                         EnumDSSMetricType   iMetricType = DssMetricSubtotalAggregation);
    
    static int getAggFuncFromMetricExpr(DSSFunctionServer *ipFunctionServer,
                                        DSSCExpression *pExpr,
                                        DSSFunctionObject **oppFunctionObject,
                                        Int32 *opFunctionIndex,
                                        VARIANT_BOOL *opIsOrderSignificant,
                                        DSSExpNode **oppExprRoot);
    
    static int StrechSliceDownward(DSSCube * ipCube,
                                   DSSCubeDataSlice *pSliceIn,
                                   Int32 nInputLevel, Int32 *ipInputLevel,
                                   Int32 nTargetLevel, Int32 *ipTargetLevel,
                                   DSSCubeDataSlice ** ppSliceOut);
    
    static int getIsInputSignificantProperty(DSSFunction* ipFunction, DSSExpNodeOperator *ipOperator, bool &bInputSignificant);
    
    static int getDistinctProperty(DSSExpNodeOperator *pCDSSOperator, bool &bDistinct);
    
    static int validateOlapPattern(DSSGenericFunction *ipFuncObj, int inSortDefns, int onBreakbyUnits, int* opBreakbyUnits);
    
    static int InspectFilterSlice(bool ibEmptySlice,DSSCube *ipFilterCube,DSSSliceID &ioFilterSlice);
    
    static int FindMetricSortbyDefnsSimpleExpr(DSSExpNode                 *ipNode,
                                               DFCSimpleExpr              *ipSortLevel,
                                               EventContext               *iEventContext,
                                               int                        inSlices,
                                               int                        *ipInSliceIDs,
                                               DSSMetric                  *ipDimMetricGov,
                                               vector<DSSSortDefinition*> **oppSortDefinition);
private:
	// qjiang TQMS 361799
	// ad-hoc optimization for "Not" operation
	static int hDirectCalculateNot( Int32 nRows, 
                                    void* ipData,
                                    DSSDataType_Type iType,
                                    DSSData_Flags* ipFlags,
                                    EnumNullCheckingOption iNullCheckingOption);
    static int hTryBinarySearchForInList(DSSCube *ipCube,
                                         Int32 inSliceIDs,
                                         DSSSliceID *ipSliceIDs,
                                         DSSSliceID *opResultSliceID,
                                         EnumNullCheckingOption iNullCheckingOption,
                                         bool ibTrimString,
                                         bool ibIsEqualsOrIn,
                                         bool isDSTrx = false);
    	template <class Type>
	inline static int hCalculateNot(Int32 nRows, Type* ipData, DSSData_Flags* ipFlags,EnumNullCheckingOption iNullCheckingOption);
	
};

inline int DSSCalculationHelper::GetConstantSlice(DSSCube *ipCube, int nUnit, int *pUnit, char *pData, int cbData, EnumDSSDataType Type, EnumDSSDataFlag Flags, DSSSliceID *pSliceID)
{
    pSliceID->mpCube = ipCube;
	return ipCube->getConstantSlice( nUnit, pUnit, pData, cbData, Type, Flags, &(pSliceID->mSliceID) );
}

template <class Type>
inline int DSSCalculationHelper::hCalculateNot(Int32 nRows, Type* ipData, DSSData_Flags* ipFlags, EnumNullCheckingOption iNullCheckingOption)
{
	Type* lpData = ipData;
	for (Int32 i = -nRows; i < 0; ++ i)
	{
		*lpData = !(*lpData);
		++ lpData;
	}
    
	if (iNullCheckingOption == DssNumericErrorTreatAs)
		memset(ipFlags, DssDataOk, sizeof(DSSData_Flags) * nRows);
	else
	{
		for (Int32 i = 0; i < nRows; ++ i)
		{
			if (ipFlags[i] != DssDataOk)
				ipData[i] = false;
		}
	}
	return S_OK;
}


#endif
