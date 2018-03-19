//
//  DSSModelInterface.h
//  ReportServiceCore
//
//  Created by temp on 5/3/12.
//  Copyright (c) 2012 MicroStrategy, Inc. All rights reserved.
//

#ifndef ReportServiceCore_DSSModelInterface_h
#define ReportServiceCore_DSSModelInterface_h

#import "DSSAEHelper.h"

class DSSCube;
class DSSTabularMetrics;
class DSSXTabView;
class DSSCalculationPlan;
class DSSTabularUnit;
class DSSDataModelTransaction;
class DSSObjectContext;

/*
    to keep the compatibility of new rwd engine and old one,
    put common methods into abstract class start with IDSSXXX
 */


class IDSSTemplateModel
{
public:
    virtual DSSCube * & getCube() = 0;
    virtual DSSObjectContext * getObjectContext() = 0;
    virtual EnumDSSMetricType getSubtotalType(int iTotalIndex, GUID iGUID = GUID_NULL) = 0;
    virtual int getMetricsUnit(DSSTabularMetrics **oppMetricsUnit) = 0;
    virtual int ReCalculate() = 0;
    virtual int Refresh() = 0;
    virtual DSSXTabView* &getXTabView() = 0;
    virtual DSSCalculationPlan *getCalculationPlan() = 0;
    virtual int CountUnits() = 0;
    virtual int LoadAllDataSlices() = 0;
    virtual DSSTabularUnit* FindTabularUnit(const GUID &irGUID, EnumDSSTemplateUnitType iType) = 0;
    virtual DSSTemplate *getTemplate() = 0;
    virtual int setBinaryTransactionModel (unsigned char * ipBinaryModel, int size) = 0;
    virtual EnumDSSSubtotalsPosition getRowSubtotalPosition() = 0;
	virtual EnumDSSSubtotalsPosition getColSubtotalPosition() = 0;
    virtual DSSDataModelTransaction *getTransactionDataModel() = 0;
    virtual int getBinaryTransactionModel (bool includeCommitted, unsigned char** oppBinaryModel, int* opSize) = 0;
    virtual int getGridModelVersion() = 0; //0-DSSDataModelXTabImpl, 1-DSSRWGridModel
};

class IDSSFieldGroupModel
{
public:
    virtual int setBinaryTransactionModel (unsigned char * ipBinaryModel, int size) = 0;
    virtual DSSDataModelTransaction *getTransactionDataModel() = 0;
    virtual int getBinaryTransactionModel (bool includeCommitted, unsigned char** oppBinaryModel, int* opSize) = 0;
};

class IDSSSourceModelForTransaction
{
public:
    virtual const string & getNodeKey() = 0;
};
#endif
