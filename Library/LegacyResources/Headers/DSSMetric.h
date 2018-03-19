/*
 *  DSSMetric.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef METRIC_H
#define METRIC_H

#include "DSSAttributeFormWrappers.h"
class DSSCExpression;
class DSSDimty;

class DSSMetric : public DSSObjectInfo
{
private:
	int mFormatRefIDs[2];
    std::string mFormula;
    int mState;
    
protected:
	DSSCExpression* mpExpression;
    DSSDimty* mpDimty;
	EnumDSSDecomposable mDecomposable;
    std::vector<GUID> mvUserDefinedSubtotals;
    
public:
	DSSMetric();
	virtual ~DSSMetric();
	DSSMetric(GUID& iID, EnumDSSObjectType iType, std::string iName, DSSObjectContext* iObjectContext, GUID iContainerID);
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	DSSFormat* getHeaderFormat();
	DSSFormat* getGridFormat();
	virtual void MakeCleanDefn();
	DSSCExpression* getExpression();
    
//    // get formula
    std::string getFormula();
    void setFormula(std::string iFormula);
    EnumDSSDecomposable getDecomposable();
    DSSDimty* getDimty();
    
    int getUserDefinedSubtotalCount();
    GUID getUserDefinedSubtotal(int iIndex);
    
    bool isDerivedMetric();
};

#endif

