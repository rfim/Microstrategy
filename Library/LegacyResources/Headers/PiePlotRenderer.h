//
//  GMPiePlot.h
//  IPhoneChart
//
//  Created by dong shi on 2/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

//GMPiePlot is used to generate the pie chart specific for Graph Matrix.  --Stone.  2012-2-24
#ifndef MsiChart_GMPiePlot_h
#define MsiChart_GMPiePlot_h

#include "Axis.h"
#include "PieSliceObject.h"
#include "DataLabelManager.h"
#include "PiePlot.h"

using namespace MsiChart;

struct PieOption
{
    PieOption()
    {
        mLayout = PL_PIE;
        mRingFactor = 70;
        mScale = 1.0;
        mDepthInChart = 0;
        mPieRotation = 90;
    }
    EnumPieLayout mLayout; //Pie or Ring
    Int32 mRingFactor; //Percentage of Ring's Inner Radius;
    Int32 mMaxRadius;
    double mScale;
    Int32 mDepthInChart;
    Int32 mPieRotation;
};

class PiePlotRenderer
{
public:
    typedef MSynch::SmartPtr<PiePlotRenderer> Ptr;
    
    PiePlotRenderer(GraphObjectManager* ipManager, Dataset::Ptr iDatasetPtr, ChartContext::Ptr irChartContextPtr, EnumGraphMatrixPiePlotType iPlotType, Int32 iMaxRadius = -1);
    virtual ~PiePlotRenderer();
    
    std::vector<GraphObject::Ptr > hDrawSinglePie(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleIndex, const Point2D& iCenter);
    
    void EnableDataLabel();
    
    void GetDataLableVecs(std::vector<GraphObject::Ptr>& iorLables);
    virtual void AddObjectsToDataLabelManager(DataLabelManager::Ptr iDataLabelManagerPtr);
protected:

    PieSliceObject::Ptr hCreateSlice(Int32 iSeriesIndex, Int32 iGroupId, Int32 iAngleIndex, Int32 iSliceId, const Point2D& iCenter, Int32 iStartAngle, Int32 iEndAngle, bool iIsOneSeries);

	bool hGetAngleInfoForSinglePie(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleIndex, Int32& orValidSeriesNumber, std::vector<Int32>& orValidSeriesIndex, std::vector<Int32>& orAngles);
    
    double hGetTotal(Int32 iGroupIndex, Int32 iSeriesId, EnumGraphMatrixDataIndexType iDataIndexType, Int32 iAngleIndex);

    Int32 hGetPieRadius(Int32 iGroupId, Int32 iSeriesId, Int32 iAngleIndex);
    double hGetMaxSizeByGroupTotal();
    double hGetMinSizeByGroupTotal();
    
private:
    
    //--------------Data Label in GM-----------------//
    TextObject::Ptr hCreateAndPutDLforSlice(const PieSliceObject* ipSlice);
    DataLabelManager::Ptr hInitDataLabelManager();
    GMDataLabelVecs mDataLabelVec;
    bool mIsShowValue;
    
    GraphObjectManager* mpManager;
    Dataset::Ptr mDatasetPtr;
    ChartContext::Ptr mChartContextPtr;
    bool mIsVertical;
    bool mIsDegressiveSize;     
    bool mIsAngled;
    bool mIsSliceDropzoneHasTemplateMetric;
    bool mIsColorbyDropzoneHasTemplateMetric;
    bool mIsXDropzoneHasTemplateMetric;
    bool mIsYDropzoneHasTemplateMetric;
    bool mIsRowOrColzoneHasTemplateMetric;
    
    Int32 mSeriesSectionCount;
    Int32 mDataPositionPartSize;
    Int32 mDataSizeByPartSize;
    Int32 mSeriesCount;
    Int32 mSizeByIndex;
    Int32 mMaxPieRadius;
    Int32 mSeriesSectionStartAngel;
    double mSeriesSectionSubtotal;
    
    bool hUpdateSpecificColor(const TripleId& irId, GraphObject::Ptr lPtr);
    
    PieOption mPieOption;
    EnumGraphMatrixPiePlotType mPlotType;
    
    double mMinSizeByTotal;
    double mMaxSizeByTotal;
    
    void hInitMaxRadius();
    
    void hInit();
    
};

#endif
