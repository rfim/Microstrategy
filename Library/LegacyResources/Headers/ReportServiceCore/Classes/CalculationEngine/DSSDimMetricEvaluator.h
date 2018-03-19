/*
 *  DSSDimMetricEvaluator.h
 *  ReportServiceCore
 *
 *  Created by Xiaolong Wang on 1/12/11.
 *  Copyright 2011 MicroStrategy, Inc. All rights reserved.
 *
 */

#ifndef CDSSDimMetricEvaluator_h
#define CDSSDimMetricEvaluator_h 1

//#import "DSSAEHelper.h"
#import "CalculationEngine/DSSCalculationEvent.h"
#include "SimpleBuffer.h"
#include "DSSLevelResolver.h"

class DSSExpNode;
class DSSMetric;
class DSSCube;
class DSSSortDefinition;
class DSSExpNodeOperator;
class DSSLevelResolver;

class DSSDimMetricEvaluator
{
	
public:
	DSSDimMetricEvaluator(EventContext *ipContext, EnumNullCheckingOption iOption);
	
	~DSSDimMetricEvaluator();
	
	
	int CalculateDimMetric(GUID &irDimMetricID, DSSSliceID *opSliceID);
	
    int CalculateDimMetric(METRIC_ID &irDimMetricID, DSSSliceID *opSliceID);
    
    int setSubtotalContext(GUID &iMetricID,
                           vector<Int32 *> *ippAttrInAxis,
                           vector<Int32> *ipnAttrInAxis,
                           vector<Int32 > *ipnAxisIndex);
    
    void set_FilterFlag(bool IsFilter){mbFilterEvaluation = IsFilter; }
	
private:
	
	int CalculateDimMetric_0(GUID iDimMetricID,
							 DSSMetric *ipDimMetricGov,
							 int iOffset, 
							 bool iIsIntermediate,
							 DSSSliceID *opSliceID);
	
	int CalculateDimExpression(DSSExpNode *ipMetricRoot, 
							   DSSMetric *ipDimMetricGov, 
							   DSSMetric *ipDimMetric,
							   int iOffset, 
							   bool iIsIntermediate,
							   DSSSliceID *opSliceID);
		
	int CalculateAggMetric(GUID iAggID, 
						   DSSMetric *ipDimMetricGov, 
						   DSSMetric *ipDimMetric,
						   int iOffset, 
						   bool iIsIntermediate,
						   DSSSliceID *opSliceID);
	
	int CalculateAggExpression(DSSExpNode *ipMetricRoot, 
							   DSSMetric *ipDimMetricGov, 
							   DSSMetric *ipDimMetric,
							   int iOffset, 
							   bool iIsIntermediate,
							   DSSSliceID *opSliceID);
	
	int CalculateAggElement(DSSExpNode *ipRoot ,
							DSSMetric *ipDimMetricGov,
							DSSMetric *ipDimMetric,
							int iLevel,
							bool iIsIntermediate,
							DSSSliceID *opSliceID,
                            EnumDSSFunction iFuncType,
                            //ctang 2010-4-29
                            bool *obIDColumn = NULL,
                            bool iLevelSignificant = true);
	
	int normalAgg(int nChild,
				  DSSSliceID *ipChildSliceID,
				  DSSMetric *ipDimMetric, 
				  DSSExpNodeOperator *ipOperator,
				  vector<DSSSortDefinition *> & irpSortDefns,
				  bool iIsIntermediate,
				  DSSSliceID *opSliceID,
                  //ctang 2010-4-29
                  bool bIsOnIDColumn);
	
	//For subtotal metric aggregation
	int subtotalAgg(int nChild, 
					DSSSliceID *ipChildSliceID,
					DSSMetric *ipDimMetric,
					int iOffset, 
					DSSExpNodeOperator *ipOperator,
					vector<DSSSortDefinition *> & irpSortDefns,
					bool iIsIntermediate,
					DSSSliceID *opSliceID);
	
	int hGetMetricSliceID(GUID & irMetricID, DSSMetric *ipDimMetricGov, DSSSliceID *opSliceID);
    
    int ReplaceDimty(DSSMetric *ipDimMetric, DSSTabularData *ipTabularData, vector<int> *opLevel);
    int ReplaceSortBy(DSSMetric *ipDimMetric, DSSTabularData *ipTabularData, DFCSimpleExpr*& opSortLevel);
    
    int FindDerivedElementUnit(DSS_ID &iGuid, Int32 &orUnit);
    
    int get_SliceLevel(DSSSliceID iSliceID, vector<int> *opLevel);
    int CheckLevelCompatible(vector<int> &iLevel, vector<int> &ioResultLevel);
    int ResolveOuterMostLevel(vector<int> &iOrgLevel, vector<int> &oResultLevel);

	EventContext *mpEventContext;
	DSSCube *mpIntermediateCube;
	
	EnumNullCheckingOption mNullCheckingOption;
    
    bool mbSubtotalContext;
    GUID mMeMetricID;
    MBase::SimpleBuffer * mpBuffer;
    DSSLevelResolver* mpLevelResolver;
    std::vector<Int32 *> *mppAttrInAxis;
	std::vector<Int32> *mpnAttrInAxis;
	std::vector<Int32> *mpnAxisIndex;
    
    bool mbFilterEvaluation;
    bool mbNonSmart;//for compatibility
};

#endif
