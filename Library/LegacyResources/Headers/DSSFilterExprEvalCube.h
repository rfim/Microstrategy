//
//  DSSFilterExprValCube.h
//  ReportServiceCore
//
//  Created by Zhuo Yang on 11/2/12.
//  Copyright (c) 2012 Microstrategy. All rights reserved.
//

#ifndef ReportServiceCore_DSSFilterExprValCube_h
#define ReportServiceCore_DSSFilterExprValCube_h

#include "GUID.h"
#include "DSSSliceID.h"
#include "DSSIPhoneEnums.h"
#include "DSSCOMMaster/DSSCOM_TLB_INC.H"
#include <vector>
class DSSCube;
class DSSCalculationPlan;
class DSSFunctionServer;
class DSSExpNode;
class CComVariant;
class EventContext;
class DSSFilter;
class DSSExpNodeFormShortcut;
class DSSBaseElementProxy;
class DSSRWDataModel;
class DSSDocumentInstance;
class DSSExpNodeGridUnitShortcut;
class DSSRWGridIterator;
enum FilterExpressionLeaf_Type {SLICE, ATTRIBUTE2, ELEMENT} ;

typedef struct FilterExpressionLeaf
{
	FilterExpressionLeaf_Type mType;
	DSSSliceID mSliceID;
	Int32 mAttributeUnit;
    
    
	EnumDSSDataType * mpTypes;
	Int32 * mpcbDatas;
	unsigned char ** mppDatas;
	DSSDataFlag * mpFlags;
} FilterExpressionLeaf;

class DSSFilterExprEvalCube
{
public:
    DSSFilterExprEvalCube(DSSCube *ipSourceCube, DSSCube *ipTargetCube, DSSCalculationPlan *ipCalculationPlan,
                          int iCurLevel, int iCurEvent, DSSFunctionServer *ipFunctionServer, EventContext *ipEventContext);
    int processFilterUnit(DSSExpNode *ipNode, DSSSliceID *opFilterSliceID);
    int processFilter(DSSFilter *ipFilter, DSSSliceID *opFilterSliceID);
    int SetDocumentInstance(DSSRWDataModel* ipModel);
    int SetDocumentInstance(DSSDocumentInstance* ipInstance);
    int SetCurrentRow(int iRow);
private:
    int processFilterGeneric(DSSExpNode *ipNode, DSSSliceID *opFilterSliceID);
    int processFilterExpression(DSSExpNode *ipNode, DSSSliceID *opFilterSliceID);
    int processFilterBranchQual(DSSExpNode *ipNode, DSSSliceID *opFilterSliceID);
	int processFilterAdvancedAttributeQual(DSSExpNode *ipNode, DSSSliceID *opFilterSliceID);
	int processFilterAdvancedInQual(DSSExpNode *ipNode,DSSSliceID *opFilterSliceID);
	int processFilterListQual(DSSExpNode *ipNode,DSSSliceID *opFilterSliceID);
	int processFilterEmbedQual(DSSExpNode *ipNode,DSSSliceID *opFilterSliceID);
	int processFilterRelationship(DSSExpNode *ipNode,DSSSliceID *opFilterSliceID);
    
    int evaluateSimpleExpression(DSSExpNode *ipNode, DSSSliceID *opFilterSliceID);
    int evaluateMetric(GUID &iMetricID, DSSSliceID *opFilterSliceID);
    int evaluateExpressionFunction(DSSExpNode *ipNode, int inChild, DSSSliceID *ipSliceID, DSSSliceID *opSliceID);
    int evaluateRelationFunction(DSSFunction_Type iFunction, int inChild, DSSSliceID *ipSliceID, DSSSliceID *opSliceID);
    int evaluateFilterExpression(DSSExpNode *ipNode, std::vector<FilterExpressionLeaf>&vChild,DSSSliceID *opFilterSliceID);
    int evaluateElementFilter(Int32 iAttributeID, const FilterExpressionLeaf& iElement, DSSSliceID *opFilterSliceID);
    int evaluateEqualFunction(std::vector<FilterExpressionLeaf>&vChild, DSSSliceID *opFilterSliceID);
    
    int hGenerateMetricSlice(GUID &iMetricID, DSSSliceID *opFilterSliceID);
    int hRetrieveExpressionLeaf(DSSExpNode * ipNode, std::vector<FilterExpressionLeaf>& vLeaf);
    int hGenerateAttributeInput(const DSS_ID &iAttributeID, Int32 &oAttributeID);
    int hApplyFiltertoDimty(Int32 nDimUnits, DSS_ID *ipDimUnits, DSSSliceID *ipFilterSliceID, DSSSliceID *opResultSliceID);
    int hGenerateAttributeFormInputs(DSSExpNodeFormShortcut * ipAttrFormShorcut, std::vector<DSSSliceID> &oAttributeFormIDs);
    int hGenerateElementInputs(DSSBaseElementProxy *ipElement,
                               Int32 &oAttributeID,
                               EnumDSSDataType * &opTypes,
                               Int32 * &opcbDatas,
                               unsigned char ** &oppDatas,
                               DSSDataFlag * &opFlags);
    
    //int getConstantFromNode(DSSExpNode *ipNode, DSSSliceID *opFilterSliceID);
    //int generateConstantInput(CComVariant *ipData, EnumDSSDataType iDataType, DSSSliceID *opFilterSliceID);
    int GenerateConstantFromGridRow(DSSExpNodeGridUnitShortcut* ipGridNode,
                                                           DSSRWGridIterator* ipGridIterator,
                                                           Int32 iRow,
                                                           CComVariant* opValue);
    DSSCube *mpSourceCube;
    DSSCube *mpTargetCube;
    DSSCalculationPlan *mpCalculationPlan;
    int mCurLevel;
    int mCurEvent;
    DSSFunctionServer *mpFunctionServer;
    EventContext *mpEventContext;
    DSSRWDataModel* mpDataModel;
    DSSDocumentInstance* mpDocInstance;
    int mCurrentRow;
};

#endif
