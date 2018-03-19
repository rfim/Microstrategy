//
//  DSSXTabDispInfo.h
//  ReportServiceCore
//
//  Created by He Tengfei on 11/14/11.
//  Copyright 2011 MicroStrategy, Inc.. All rights reserved.
//


#ifndef DSSXTABDISPINFO_H
#define DSSXTABDISPINFO_H 

#include "DSSrwdconstants.h"
#include "DSSCalculationPlan.h"

#import <vector>

using namespace std;


struct DSSXTabDispSliceInfo
{
    int mSliceID;
    int mTag;
    int mPosition;
    EnumDssSliceFlag mSliceFlag;
    EnumDSSRuntimeSubtotalType mExtremeType;
    int mModelIndex;
    int mTRMetricID;
    int mnXVariable;
    vector<int> mvXVariables;
    vector<EnumDSSObjectType> mvXVariableTypes;
    vector<int> mvXIndexMetrics;
    DSSXTabDispSliceInfo(int SliceID,
                         int Tag,
                         int Position,
                         EnumDssSliceFlag SliceFlag = DssOriginalSlice,
                         EnumDSSRuntimeSubtotalType ExtremeType = DssRuntimeSubtotalDefault,
                         int iModelIndex = -1,
                         int iTRMetricID = -1,
                         int nXVariable = 0,
                         int *ipXVariables = NULL,
                         EnumDSSObjectType *ipXVariableTypes = NULL,
                         int *ipXIndexMetrics = NULL)
    :mSliceID(SliceID), mTag(Tag), mPosition(Position), mSliceFlag(SliceFlag), mExtremeType(ExtremeType),
    mModelIndex(iModelIndex), mTRMetricID(iTRMetricID), mnXVariable(nXVariable)
    {
        if (mnXVariable > 0)
        {
            mvXVariables.resize(mnXVariable);
            mvXVariableTypes.resize(mnXVariable);
            mvXIndexMetrics.resize(mnXVariable);
            memmove(&mvXVariables[0], ipXVariables, mnXVariable * sizeof(Int32));
            memmove(&mvXIndexMetrics[0], ipXIndexMetrics, mnXVariable * sizeof(Int32));
            memmove(&mvXVariableTypes[0], ipXVariableTypes, mnXVariable * sizeof(EnumDSSObjectType));
        }
    };
    DSSXTabDispSliceInfo(int SliceID, int Tag, int Position, EnumDssSliceFlag SliceFlag = DssOriginalSlice)
    :mSliceID(SliceID), mTag(Tag), mPosition(Position), mSliceFlag(SliceFlag){};
    DSSXTabDispSliceInfo()
    :mSliceID(0), mTag(0), mPosition(0), mSliceFlag(DssOriginalSlice){};
};

class DSSXTabDispInfo
{
  
    
public:
    DSSXTabDispInfo();
    
    ~DSSXTabDispInfo();
    
    
    int get_LevelCount(int *pLevelCount);
    
    int get_PositionCount(int *pMetricCount);
    
    int get_SliceCountByLevel(int  iLevel, int *pSliceCount);
    
    int get_SliceCountByLevel2(int  iLevel, int *pSliceCount);
    
    int get_SliceInfo(int  iLevel, int iOffset, int *pSliceID, int *pPosition, int *pTag);
    
    int get_SliceInfo2(int  iLevel, int iOffset, int *pSliceID, int *pPosition, int *pTag, EnumDssSliceFlag * pSliceFlag);
    
    int get_SliceInfo3(int  iLevel, int iOffset, int *pSliceID, int *pPosition,
                       int *pTag,
                       EnumDssSliceFlag * pSliceFlag,
                       EnumDSSRuntimeSubtotalType *pExtremeType,
                       int *pModelIndex,
                       int *pTRMetricID,
                       int *pXVariableCount,
                       int **ppXVariables,
                       EnumDSSObjectType **ppXVariableTypes,
                       int **ppXIndexMetrics);
    
    int putAttributeThresholdSliceInfo(int iLevel, int iTag, DSS_ID ipAttrID, int opSliceID);
    typedef std::vector<std::pair<std::string, DSS_ID> > AttributeThresholdSliceInfo;
    AttributeThresholdSliceInfo& getAttributeThresholdSlices();
    
    int get_UnitCountByLevel(int  iLevel, int *pCount);
    
    int get_OrgUnitByLevel(int  iLevel, int cb, int *pOrgUnit, int *pcbWritten);
    
    //int put_SliceInfo(int  iLevel, int  SliceID, int  Position, int  Tag, EnumDssSliceFlag = DssOriginalSlice);
    
    int put_SliceInfo(int  iLevel,
                      int  SliceID,
                      int  Position,
                      int  Tag,
                      EnumDssSliceFlag = DssOriginalSlice,
                      EnumDSSRuntimeSubtotalType ExtremeType = DssRuntimeSubtotalDefault,
                      int iModelIndex = -1,
                      int iTRMetricID = -1,
                      int nXVariable = 0,
                      int *ipXVariables = NULL,
                      EnumDSSObjectType *ipXVariableTypes = NULL,
                      int *ipXIndexMetrics = NULL);
    
    int init(int nPosition);
    
    int AddLevel(int inUnit, int *ipUnit, int *opLevel);

private:
    int mnPosition;
    
    vector<int *> mvpOrgUnit;
    vector<int> mvnOrgUnit;
private:  
    // Additional Implementation Declarations
    vector<DSSXTabDispSliceInfo *> mvpSliceInfos;
	
    vector<vector<int> *> mvpvLevel;	//each level is a vector of pointers 
    
    vector<vector<int> *> mvpvLevel2;//each level is a vector of pointers 
    
    AttributeThresholdSliceInfo mAttributeThresholdSlices;
};

// Class DSSXTabDispInfo 





#endif