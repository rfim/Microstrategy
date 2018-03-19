//
//  DSSModel.h
//  ReportServiceCore
//
//  Created by Zeng Jinliang on 8/7/14.
//  Copyright (c) 2014 MicroStrategy, Inc. All rights reserved.
//

#ifndef __ReportServiceCore__DSSModel__
#define __ReportServiceCore__DSSModel__

#include "DSSSecondClassObject.h"

typedef enum EnumDSSModelType
{
    DssModelTypeReserved = 0,
    DssModelReferenceLineRoot = 0,
    DssModelMax = 1,
    DssModelMin = 2,
    DssModelAvg = 3,
    DssModelMed = 4,
    DssModelFirst = 5,
    DssModelLast = 6,
    DssModelConstant = 7,
    DssModelReferenceLineLastOne,
    DssModelTrendLineRoot = 255,
    DssModelBestfit = 256,
    DssModelLinear = 257,
    DssModelLog = 258,
    DssModelExp = 259,
    DssModelPow = 260,
    DssModelPoly = 261,
    DssModelTrendLineLastOne
} EnumDSSModelType;

typedef enum EnumDSSModelLevel
{
    DssModelLevelReserved = 0,
    DssModelLevelPerCell = 1,
    DssModelLevelPerRow = 2,
    DssModelLevelPerColumn = 3,
    DssModelLevelPerPanel = 4,
    DssModelLevelLastOne
} EnumDSSModelLevel;

typedef enum EnumDSSModelLabels
{
    DssModelLabelsReserved = -1,
    DssModelLabelsNone = 0x00,
    DssModelLabelsType = 0x01,
    DssModelLabelsMetric = 0x02,
    DssModelLabelsValue = 0x04
} EnumDSSModelLabels;

class DSSModel : public DSSSecondClassObject
{
public:
	DSSModel(DSSObjectContext* ipObjectContext);
	virtual ~DSSModel() {}
    
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	
    EnumDSSModelType getType() const { return mType; }
    EnumDSSModelLevel getLevel() const { return mLevel; }
    EnumDSSModelLabels getLabels() const { return mLabels; }
    bool getIncludeBreakBy() const { return mIncludeBreakBy; }
    CComVariant getConstant() const { return mConstant; }
    const std::string& getFormatString() const { return mFormatString; }
    bool getEnable() const { return mEnable; }
    int getDualAxisIndex() const { return mDualAxisIndex; }
    
private:
	EnumDSSModelType mType;
	EnumDSSModelLevel mLevel;
	EnumDSSModelLabels mLabels;
	bool mIncludeBreakBy;
	CComVariant mConstant;
    std::string mFormatString;
	bool mEnable;
	int mDualAxisIndex;
	
};


#endif /* defined(__ReportServiceCore__DSSModel__) */
