 /*
 *  DSSTemplateMetric.h
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TEMPLATE_METRIC_H
#define TEMPLATE_METRIC_H

#include "DSSTemplateUnit.h"
#include "DSSInputControlSetting.h"

class DSSMetric;
class DSSCExpression;
class DSSModels;

class DSSTemplateMetric : public DSSTemplateUnit
{
private:
	int mColumnWidth;
	int mFormatRefIDs[2];
	int mResolvedFormatRefIDs[2];
	DSSThresholds* mpThresholds;
	DSSInputControlSetting* mpInputControlSetting;
	DSSCExpression* mpLimitExpression;
    DSSCExpression* mpRuntimeLimitExpression;
    
    DSSModels* mpModels;
	CComVariant mForecastBackward;
	CComVariant mForecastForward;
	bool mIsDuplicatedRuntimeMetric;
	bool mForecastEnable;
	bool mIsExtremeRuntimeMetric;
public:

	int getMetricWidth();

	DSSTemplateMetric();
	virtual ~DSSTemplateMetric();
	DSSTemplateMetric(DSSObjectContext* ipObjectContext);	
	DSSMetric* getMetric();
	int LoadDefnFromBinary(DSSBinaryDataReader* ipReader, int iIndex);
	DSSFormat* getHeaderFormat(bool ibUseResolved=false, bool ibOriginal = false);
	DSSFormat* getGridFormat(bool ibUseResolved=false, bool ibOriginal = false);
	DSSThresholds* getThresholds();
    void setThreholds(DSSThresholds* iThresholds);
	void setResolvedFormatRefID(int iIndex, int iFormatRefID);
    void setFormatRefID(int iIndex, int iFormatRefID);
	EnumUnitTransactionFlags IsEditable();
	EnumUnitTransactionFlags IsRequired();
	int getColumnIndex();
	std::string getInputFormat();
	std::string getInputControlKey();
    DSSRWControl* getInputControl();
    
    DSSFormat* getFormat(int iFormatType, bool isReadOnly = true);
    
    int getColumnWidth();
    void Duplicate(DSSTemplateMetric* ipMetric);
    DSSCExpression* getLimitExpression();
    void setLimitExpression(DSSCExpression* ipLimitExpression);
    int getOfflineColumnIndex();
    //tliao, used by AE
    DSSCExpression* getRuntimeLimitExpression();
    void setRuntimeLimitExpression(DSSCExpression* ipLimitExpression);
    
    // jzeng, used by viewer to draw html5 VI
    DSSModels* getModels() { return mpModels; }
    const DSSModels* getModels() const { return mpModels; }
	const CComVariant& getForecastBackward() const { return mForecastBackward; }
	const CComVariant& getForecastForward() const { return mForecastForward; }
	bool getIsDuplicatedRuntimeMetric() { return mIsDuplicatedRuntimeMetric; }
	bool getForecastEnable() { return mForecastEnable; }
	bool getDssReportXml2RawDates() { return mIsExtremeRuntimeMetric; }
};

#endif
