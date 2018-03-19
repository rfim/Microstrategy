/*
 *  DSSTemplate.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <vector>
#include "DSSAxis.h"
#include <vector>

class DSSDrillPaths;
class DSSDataModelXTabImpl;
class DSSRWGridModel;
class DSSDataModelTransaction;
class IDSSTemplateModel;
class DSSDataModelControlImpl;
class DSSRWTemplateNode;
class DSSDropZoneSet;

// used for hash the unit index (rowHashIndex = (metrics.count + MAX_TEMPLATE_UNIT + 1) + rowIndex, colHashIndex = (metrics.count + MAX_TEMPLATE_UNIT - 1) - colIndex
const int MAX_TEMPLATE_UNIT = 10000;

class DSSTemplate : public DSSObjectInfo
{
private:
	DSSAxis* mRow;
	DSSAxis* mColumn;
	bool mIsBandingHidden;
	bool mIsSubtotalsHidden;
	DSSDataModelXTabImpl* mpXTabModel; // old one
	DSSDataModelXTabImpl* mpXTabModelLatest; // new latest one for on-demand loading feature
	int mFormatRefIDs[6];
	DSSDrillPaths* mpDrillPaths;
	void hLoadResolvedFormatsAxis(DSSBinaryDataReader* ipReader, DSSAxis* lpAxis);
	bool hIsTransactionEnabled(DSSAxis* ipAxis);
    void hGetRequiredColumnIndexs(DSSAxis* ipAxis, std::vector<int>& ioColumnIndexs);
    GUID mDataSourceID;
    DSSRWGridModel *mpGridModel;       //week reference to grid model in new rwd engine
    std::map<int, std::map<GUID, int> > mSubtotalMtypeMapping;
    std::vector<GUID> mvUserDefinedSubtotals;
    DSSDropZoneSet* mpDropZoneSet;
    EnumDSSTimeRole mSeasonality;
    
public:
	DSSTemplate();
	virtual ~DSSTemplate();

	DSSTemplate(GUID& iID, EnumDSSObjectType iType, std::string iName, DSSObjectContext* iObjectContext, GUID iContainerID);
	DSSAxis* getRow();
	DSSAxis* getColumn();
	DSSAxis* getAxis(int iIndex); //1 based
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader);
	DSSDataModelXTabImpl* getXTabModel();
	int setXTabModel(DSSDataModelXTabImpl* ipXTabModel);
	void UpdateDataModel();
	DSSTemplateMetrics* getTemplateMetrics();
	DSSTemplateUnit* getTemplateUnitFromAxisIndex(int iAxisIndex, const GUID& iID, bool iNeedTemplateMetric = false); // iNeedTemplateMetric==true returns DSSTemplateMetric, otherwise returns DSSTemplateMetrics
	DSSTemplateUnit* getTemplateUnitFromAxis(DSSAxis* ipAxis, const GUID& iID, bool iNeedTemplateMetric = false);
    DSSTemplateUnit* searchTemplateUnit(const GUID& iID, EnumDSSTemplateUnitType unitType);
	DSSFormat* getHeaderFormat();
	DSSFormat* getHeaderValuesFormat();
	DSSFormat* getSubtotalHeadersFormat();
	DSSFormat* getSubtotalValuesFormat();
	DSSFormat* getDataFormat();
	DSSFormat* getReportFormat();
	DSSDrillPaths* getDrillPaths();
	virtual void MakeCleanDefn();
	bool IsBandingHidden();
    void setBandingHidden(bool iBandingHidden);
	bool IsSubtotalsHidden();
	DSSTemplateUnit * getInnermostUnit(int iAxisIndex, bool iIncludeMetricsUnit);
    DSSTemplateUnit * getInnerMostAttributeUnit();
	bool IsTransactionEnabled();
	void getRequiredColumnIndexs(std::vector<int>& ioColumnIndexs);
	//
	bool MoveUnit(int iSrcAxisIndex, int iSrcPos, int iDstAxisIndex, int iDstPos);
    void Duplicate(DSSTemplate* ipTemplate);
    void setFormatRefID(int iIndex, int iFormatRefID);
    // find disabled units or metrics
    DSSTemplateUnit* FindDisabledUnit(DSSObjectInfo* ipObject);
    DSSTemplateMetric* FindDisabledMetric(DSSObjectInfo* ipObject);
    
    // remove units or metrics
    void removeDisabledUnit(DSSTemplateUnit* ipUnit);
    void removeDisabledMetric(DSSTemplateMetric* ipMetric);
    void addDisabledUnit(DSSTemplateUnit* ipUnit);
    // disabled units and metrics
    std::vector<DSSTemplateUnit*> mDisabledUnits;
    std::vector<DSSTemplateMetric*> mDisabledMetrics;
    inline std::vector<DSSTemplateMetric*> *getDisabledMetrics() {return &mDisabledMetrics;}
    void setDataSourceID(GUID iGuidID);
    GUID getDataSourceID();
    
    bool hasThreshold();
    bool hasAttributeThreshold();
    //load DDIC from binary without loading its parent xtabmodel
    int LoadInputControlModel(std::string iKey, DSSDataModelControlImpl** oppControl);
    //lwang; added for new RWD engine to keep backward compatibility
    IDSSTemplateModel * getTemplateModelInterface();
    inline void setGridModel(DSSRWGridModel *ipNodeModel){mpGridModel = ipNodeModel;};
    inline DSSRWGridModel *getGridModel(){return mpGridModel;};
    DSSDataModelTransaction *getTransactionDataModel();
    
    DSSDataModelTransaction * getLatestTransactionDataModel();
    
    // 06/06/2013, clark, get metric type for metric subtotal
    EnumDSSMetricType getMetricType(int iSubTotalIndex, GUID iMetricID);
    
    int getUserDefinedSubtotalCount();
    GUID getUserDefinedSubtotal(int iIndex);
    
	DSSRWTemplateNode* getTemplateNode();
    void setTemplateNode(DSSRWTemplateNode* ipTemplateNode);
    bool rollbackToUnsetForAttributeSelector(DSSTemplateUnit* iUnit);
    // 06/24/2014, jzeng, get dropzone info
    DSSDropZoneSet* getDropZoneSet() { return mpDropZoneSet; }
    EnumDSSTimeRole getSeasonality() { return mSeasonality; }
};

inline void DSSTemplate::setDataSourceID(GUID iGuidID)
{
    mDataSourceID = iGuidID;
}

inline GUID DSSTemplate::getDataSourceID()
{
    return mDataSourceID;
}

inline void DSSTemplate::setBandingHidden(bool iBandingHidden)
{
    mIsBandingHidden = iBandingHidden;
}

inline void DSSTemplate::setFormatRefID(int iIndex, int iFormatRefID)
{
    mFormatRefIDs[iIndex] = iFormatRefID;
}
#endif
