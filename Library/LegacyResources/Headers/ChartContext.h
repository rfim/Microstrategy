//==============================================================================================
//	FILENAME	:	ChartContext.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2008-06-13
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_ChartContext_h
#define MsiChart_ChartContext_h

#include "FormatLine.h"
#include "FormatFill.h"
#include "FormatFont.h"
#include "../Common.h"
#include "CEModuleMain.h"
#include "ChartCanvas.h"
#include "../Dataset.h"
#include "Point2D.h"
#include "Vector2D.h"

#include "PDCHeader/PDCfstream"
#include "PDCHeader/PDCmap"
//struct tagVARIANT;
//typedef struct tagVARIANT VARIANT;
#include "FormatManager.h"
#include "ChartContextHelper.h"

#ifndef _VE_CE
#import "CGCanvas.h"
#endif


namespace MsiChart
{
#ifndef DPI
#define DPI 96		//	Usually 96DPI on Windows
#endif

#define POINTSOFONEINCH 72	//	1pt = 1/72 inch

#define TDGVirtualXMIN	(-16383)
#define TDGVirtualXMAX	16383
#define TDGVirtualYMIN	(-16383)
#define TDGVirtualYMAX	16383

	extern const Rect2D gDefaultTitlePosition;
	extern const Rect2D gDefaultSubtitlePosition;
	extern const Rect2D gDefaultFootnotePosition;
	extern const TripleId gNullTripleId;

	const Int32 gMaxTextLength = 255;
	const Int32 gDefaultUniformAxisMargin = 2;

	const Int32 gVirtualMarkerSize = 2;

	struct ChartAdvFormat;
	class Chart;

	/**
     This collection is introduced mainly for two purpose.
     1. It is used by GetFormatLine() and GetFormatFill().
     2. It is used by hApplyEnhancedSeriesEffect().
     */
	enum EnumCollectionType
	{
		CT_NO_COLLECTION,

		// General collection types
		CT_LINE,
		CT_GENERAL_SHAPE,
		CT_TEXT,

		// The following 6 collection types are only used by ABL chart for convenience to apply enhanced series effect.
		CT_VERTICAL_AREA,
		CT_VERTICAL_BAR,
		CT_HORIZONTAL_AREA,
		CT_HORIZONTAL_BAR,
		CT_DEPTH_LINE_OR_LINE_MARKER,

		/// Do not share format.
		CT_NO_FORMAT
	};

	/**
	 use to generate different chart component according to user requirement
	 */
	enum EnumChartComponent
	{
		CC_STATIC = 1,
		CC_DYNAMIC = 2,
		CC_ALL = 3, //the default
	};

    //	enum EnumLineJoinType
    //	{
    //		LJT_MITER = 0,
    //		LJT_BEVEL = 1,
    //		LJT_ROUND = 2,
    //	};

	enum EnumGraphMatrixLayoutType
	{
		GMLT_NORMAL = 0,
		GMLT_FIT_CONTENT = 1,
		GMLT_FIT_WINDOW = 2,
	};

	enum EnumGraphMatrixMarkerInChartType
	{
		GMMS_FROM_LINE_CHART_AS_LINE,
		GMMS_FROM_LINE_CHART_AS_DOT_IN_LINE,
		GMMS_FROM_LINE_CHART_AS_LINE_ONE_DOT,
		GMMS_FROM_LINE_CHART_AS_NONE_LINE,
		GMMS_FROM_BAR_CHART,
		GMMS_FROM_BUBBLE_CHART,
		GMMS_FROM_GRID_CHART,
        GMMS_FROM_TICK_SINGLE_METRIC,
        GMMS_FROM_TICK_DOUBLE_METRIC,
        GMMS_FROM_TICK_NONE_METRIC,
        GMMS_FROM_PIE,
	};

    typedef enum
    {
        GMAL_NONE,
        GMAL_GRID,
        GMAL_VALUE_SCATTER,
        GMAL_VALUE_BUBBLE,
        GMAL_ABL_VERTICAL,
        GMAL_ABL_HORIZONTAL,
        GMAL_INVALID,
    }EnumGraphMatrixAxisLayoutType;

    typedef enum
    {
        GMMGT_UNDEFINED = 0,
        GMMGT_AREA,
        GMMGT_BAR,
        GMMGT_LINE,
        GMMGT_SCATTER,
        GMMGT_BUBBLE,
        GMMGT_PIE,
        GMMGT_GRID,
        GMMGT_UNKNOWN,
    }EnumGraphMatrixMajorGraphType;

    typedef enum   {
    _COMBINED = -2,
    _GENERIC,
    _AREA,
    _BAR,
    _LINE2,
    _STEPLINE,
    _CIRCLE,
    _RECTANGLE,
    _SQUARE,
    _TICKER,
    _PIESLICE,
    _POLYGON,
    _PIE,
    _BULLET,
    _CURVE,
    _DEFAULT = 100
    } EnumShapeType;
    
    typedef enum   {
        Axis_x1,
        Axis_x2,
        Axis_y1,
        Axis_y2,
    } EnumAxis;
    
	struct PropertyKey
	{
		Int32 mPropertyId;
		Int32 mObjectId;
		Int32 mSeriesId;

		PropertyKey(Int32 iPropertyId = -1, Int32 iObjectId = gNullObjectId, Int32 iSeriesId = gNullSeriesId)
        : mPropertyId(iPropertyId), mObjectId(iObjectId), mSeriesId(iSeriesId)
		{
		}

		bool operator < (const PropertyKey& iKey) const
		{
			if (mPropertyId < iKey.mPropertyId)
				return true;
			if (mPropertyId > iKey.mPropertyId)
				return false;

			if (mObjectId < iKey.mObjectId)
				return true;
			if (mObjectId > iKey.mObjectId)
				return false;

			return (mSeriesId < iKey.mSeriesId);
		}
	};

	struct ThresholdCache
	{
		Int32 mSeriesId;
		Int32 mGroupId;
		bool mUseThreshold;
        FormatFill::Ptr mFormatFillPtr;
		ThresholdCache();
        void Reset();
	};

	/**
	 * Cache structure of Metric FormatFill.
	 */
	struct MetricFormatFillCache
	{
		bool mValid;					///< Indicate wheather this cache is valid.
		FormatFill::Ptr mFormatFillPtr;	///< cache.

		MetricFormatFillCache() : mValid(false), mFormatFillPtr(NULL) {}
	};

	/// Only used in time series chart.
	struct DatasetState
	{
		bool mIsSliced;
		bool mIsSliceEnabled;
		Int32 mBeginGroup;
		Int32 mEndGroup;
		DatasetState();
        void Reset();
	};

	/// Only used in time series chart.
	struct AxesState
	{
		bool mIsY1InZoomState;	///< Indicates whether Y1 axis is in zoom state;
		bool mIsO1InZoomState;	///< Indicates whether O1 axis is in zoom state;
		bool mIsO1SwipedOutOfZoom;		///< Indicates whether the range of O1 axis has been swiped;
		bool mIsY1RangeBeforeZoomReady;
		bool mIsO1RangeBeforeZoomReady;
		bool mUseManualGroupRange;
		Int32 mLastGroupID;	///< Indicates the last group ID which is shown in current graph.
		Int32 mValueAxisAreaWidth;
		Int32 mSuggestedDataAreaStartX; //Used in zooming state for time series
		Int32 mTopMargin;
		Int32 mO1LabelLevel;
		Int32 mSwipeOffsetOutOfZoom;
		double mY1LogicalMin;	///< Indicates the minimum logical coordinate of Y1 axis. Used in zoom state.
		double mY1LogicalMax;	///< Indicates the maximum logical coordinate of Y1 axis. Used in zoom state.
		double mO1LogicalMin;	///< Indicates the minimum logical coordinate of O1 axis. Used in zoom state.
		double mO1LogicalMax;	///< Indicates the maximum logical coordinate of O1 axis. Used in zoom state.
		double mY1LogicalMinBeforeZoom;	///< Indicates the minimum logical coordinate of Y1 axis before zooming.
		double mY1LogicalMaxBeforeZoom;	///< Indicates the maximum logical coordinate of Y1 axis before zooming.
		double mO1LogicalMinBeforeZoom;	///< Indicates the minimum logical coordinate of O1 axis before zooming.
		double mO1LogicalMaxBeforeZoom;	///< Indicates the maximum logical coordinate of O1 axis before zooming.
		double mO1InitLogicalMin;
		double mO1InitLogicalMax;
		std::vector<MBase::String>	mScrollViewLabels;
		std::vector<Int32>			mScrollViewLabelIds;
		std::vector<Rect2D>			mScrollViewLabelRects;
		DatasetState mInitDatasetState;
		AxesState();
        void Reset();
	};

	/// only used by uniform axis in graph matrix.
	struct UniformAxisContext
	{
		Int32 mDataAreaLeftMargin;		///< Indicates the margin between Y axis line and Y axis labels.
		Int32 mDataAreaRightMargin;		///< Indicates the margin at right of data area.
		Int32 mDataAreaTopMargin;		///< Indicates the margin at top of data area.
		Int32 mDataAreaBottomMargin;	///< Indicates the margin between X axis line and X axis labels.
		Int32 mGraphHeight;				///< Indicates an estimated graph height for font size calculation.
		bool mIsAxisTitleAndLabelRequired;	///< Indicate whether axis title and labels are required
		double mMinValue;				///< Indicates the minimum value in value axis.
		double mMaxValue;				///< Indicates the maximum value in value axis.
		double mXMinValue;				///< Indicates the minimum value in X axis for Scatter/Bubble chart
		double mXMaxValue;				///< Indicates the maximum value in X axis for Scatter/Bubble chart
		double mZMinValue;				///< Indicates the minimum value in X axis for Bubble chart
		double mZMaxValue;				///< Indicates the maximum value in X axis for Bubble chart
		Int32 mLabelWidthLimitY;		///< Indicates the maximum allowed lenght for the Y axis labels(only for category axis).
		Int32 mLabelWidthLimitX;		///< Indicates the maximum allowed lenght for the X axis labels(only for category axis).
        //Get the PrimaryMetricIndex from widget
        Int32 mXMetrixIndex;
        Int32 mYMetrixIndex;
        Int32 mZMetrixIndex;

        Int32 mX2MetrixIndex;
        Int32 mY2MetrixIndex;

        double mY2MinValue;				///< Indicates the minimum value in Y2 axis.
		double mY2MaxValue;				///< Indicates the maximum value in Y2 axis.
		double mX2MinValue;				///< Indicates the minimum value in X2 axis.
		double mX2MaxValue;				///< Indicates the maximum value in X2 axis.

        Int32 mSecondaryDataAreaLeftMargin;		///< Indicates the margin between Y axis line and Y axis labels.
		Int32 mSecondaryDataAreaRightMargin;		///< Indicates the margin at right of data area.
		Int32 mSecondaryDataAreaTopMargin;		///< Indicates the margin at top of data area.
		Int32 mSecondaryDataAreaBottomMargin;	///< Indicates the margin between X axis line and X axis labels.

        std::vector<Int32> mAxisHostMetricSet[4];  ///< Hosted metric on X1/X2/Y1/Y2 axis

        // Metric Valid
        bool mIsXValid;
        bool mIsYValid;
        bool mIsZValid;
        bool mIsX2Valid;
        bool mIsY2Valid;
        bool mIsZ2Valid;

		UniformAxisContext();
	};

	struct GraphMatrixContext
	{
		const static Int32 sVisualElementBound = 1;

		Int32 mDataGroup;    //< Data groups for one series.

		EnumDssGraphMatrixSizeMode  mSizeMode;	//How the size should be computed.
		double mMaxSizeVal;
		double mMinSizeVal;
		Int32 mSizeIndex;						//Size index for non-bubble chart.

		EnumDssGraphMatrixElementSizeMode mMaxElementSizeMode;
		EnumDssGraphMatrixElementSizeMode mMinElementSizeMode;
		double mMaxElementSize;					//used in size = specific
		double mMinElementSize;					//used in size = proprotional
		bool mIsMinElementSizeAsRatioToMax;		//whether min size is a ratio to max element size.
		double mEstGraphWidth;
		double mEstGraphHeight;

        // Use fixed Marker size for GridChart in special case
        Int32 mFixMarkerSizeFromWidget;

		EnumDssGraphMatrixColorMode mColorMode;	//How the marker should be colored
		Int32 mColorArity;						//Used in ByX or ByY
		double mMaxColorVal;
		double mMinColorVal;
		std::vector<Int32> mColors;
		Int32 mColorIndex;

		Int32 mMarkerShape;
		Int32 mMaxAutoBarWidth;				///< Indicates the maximum bar width, used in "Automatic" mode
		double mRatioToMaxRadius;			///< Indicates the ratio in the bubble max radius computation.
		double mLineSizeInput;				///< The inputed line width by user.

		Int32 mBubbleBourndaryPadding;		///< The padding between bubble and chart boundary
        Int32 mTickSizeInFD;                ///< Tick shape size in the fixed dimension
		std::vector<Int32> mVecXValueAxisLowPaddingSet;	 ///< The lowPadding for x axis
		std::vector<Int32> mVecXValueAxisHighPaddingSet; ///< The highPadding for y axis
		std::vector<Int32> mVecYValueAxisLowPaddingSet;	 //< The lowPadding for y axis
		std::vector<Int32> mVecYValueAxisHighPaddingSet; //< The highPadding for y axis
		std::vector<Int32> mExtraPaddings;				 //< Extra paddings for axis.

		bool mIsXDropZoneEmpty;
		bool mIsYDropZoneEmpty;
		bool mIsSeriesUniformInColor;			///< Is the each section of the line chart shares the same color.
		bool mShouldXAxisLabelRotate;
		EnumDssGraphMatrixMetricCategory mAddedMetricToTooltip;	///< which metric should be added to tooltip
		EnumGraphMatrixLayoutType mLayoutMode;	///< Layout mode.
		Int32 mCachedMaxElementSize;

        bool mHasChartWithOnlyDataMarkers;

        //Data Label Support
        bool mIsNeedRotateDataLabel;  //cached rotate state for Data Label in vertical Bar Chart
        bool mIsNeedAdjustAxis;
        double mAxisAdjustedValue;
        bool mIsDataLabelEnabled;

        //Custom Min/Max Support, only used in ABL chart.
        bool mIsSizebyCustomMinMaxEnabled;

		std::vector<Int32> mRowColorByUnits;
		std::vector<Int32> mColColorByUnits;
		std::vector<Int32> mBubbleSizeByIndexSet;
		std::set<double> mShownValueTickSet[2]; ///< The shown ticks for uniform x-axis, y-axis
		std::set<Int32> mCondesedMetricsSet;	///< The metrics which will condese the value labels whose number is under it.
		Int32 mCondenseMode;					///< The condense mode.
        Int32 mCondenseLimit;                   ///< lower bound for label condense.

        EnumGraphMatrixAxisLayoutType mAxisLayout;           ///< How the axis is assigned.
        ChartContext* mpChartContext;           ///< The parental chart context.

        Int32 mLabelStaggerStart;
        Int32 mLabelStaggerEnd;

        //Combination Graph Support
        bool mIsCombinationGraph;
        //Dual Axis Graph
        bool mIsDualAxisGraph;

        //pie chart specific
        bool mIsPieSliced;                      //< If there is any unit in the slice drop zone.
        bool mIsPieMetricSliced;                //< If metrics object is in the slice drop zone.
        bool mIsSliceDropzoneHasTemplateMetric; //< If template-metrics is in the slice drop zone.
        bool mIsColorbyDropzoneHasTemplateMetric; //< If template-metrics is in the colorby drop zone.
        bool mIsXDropzoneHasTemplateMetric; //< If template-metrics is in the X drop zone.
        bool mIsYDropzoneHasTemplateMetric; //< If template-metrics is in the Y drop zone.
        bool mIsBreakbyDropzoneHasTemplateMetric; //< If template-metric is in the Breakby dropzone.
        bool mIsRowOrColDropzoneHasTemplateMetric; //< If template-metrics is in the Row or Col drop zone.
        bool mIsEmptyXAxisDropWithTemplateMetric; //< If X-Dropzone empty and has TemplateMetric.
        bool mIsEmptyYAxisDropWithTemplateMetric; //< If Y-Dropzone empty and has TemplateMetric.
        bool mIsEmptyXAxisDropZone;
        bool mIsEmptyYAxisDropZone;
        bool mMarkerShown;
        Int32 mPieGroupSectionCount;
        Int32 mPieSeriesSectionCount;
        Int32 mCurrentPieSliceIndex;
        Int32 mCurrentPieAngleIndex;
        Int32 GetCurrentPieSlice();
        void SetCurrentPieSlice(Int32 iPieSliceIndex);
        Int32 GetCurrentAngleIndex();
        void SetCurrentAngleIndex(Int32 iAngleIndex);

        //Refactor ColorBy Mode for GraphMatrix
        std::vector<std::pair<Int32, Int32> > *mColorByKeyUnitsPair;
        std::map<std::string, Int32> *mColorByMap;
        std::map<int, bool> *mHasAreaBarInAxis;

		std::vector<std::pair<double, double> > mBubbleSizeRangeSet;
		std::set<TripleId> mIsolaetdDotsInNormalLineChart;   //<< The <Sid, Gid> pair to record the isolated dots in a normal line graph
        std::map<Int32, double> mValueAxisZoomScales;   ///< The value axis id to its zoom scale.

		bool IsColorValid() {return mColors.size() == mColorArity;}
		GraphMatrixContext();

		Int32 mCategoryAxisLableInteval;
		/** To get a ratio with the input
		 @param iVal, the input value.
		 @param iIsForColor, whether we are doing this for color related stuff or value related stuff.
		 @param iIsUseZeroAsBoundaryValue, if set true, the boundary value could be set as zero min -> 0 if min >0 or max -> 0 if max < 0;
		 @param iIsBubbleChart, if we are doing this for bubble chart.
		 @param iSizeIndex, the index of the size metric.
		 @return the ratio.
		 */
		double ToRatio(double iVal, bool iIsUseZeroAsBoundaryValue = false);
        double ToRatioEX(double iVal, bool iIsUseAbsValue = false);
        
        double GetSizebyPieRadius(double iVal, double iMax, double iMin, double maxRadius);

		/** To get a ratio with the input
		 @param iVal, the input value.
		 @param iMax, the max val.
		 @param iMin, the min val.
		 @param iIsUseZeroAsBoundaryValue, if set true, the boundary value could be set as zero min -> 0 if min >0 or max -> 0 if max < 0;
		 @return the ratio.
		 */
		static double ToRatio(double iVal, double iMax, double iMin, bool iIsUseZeroAsBoundaryValue);

		Int32 GetSizeByIndex(Int32 iIndex, bool iIsBubbleChart = false);

		/** To Get the max radius/element for one single element
		 @param iChartType, specifiy the chart type combined with the shape type.
		 @param iIsVertical, if the chart is vertical or not.
		 @param iUnitCount, the group count.
		 @param iIsBubbleChart, if we are doing this for bubble chart.
		 @return the max radius.
		 */
		Int32 GetMaxMarkerSize(EnumGraphMatrixMarkerInChartType iChartType, bool iIsVertical, Int32 iUnitCount, bool iIsBubbleChart = false);

		/** To Get the max radius/element for one single element in line chart, will call GetMaxMarkerSize
		 @param iIsVertical, if the chart is vertical or not.
		 @param iUnitCount, the group count.
		 @return the max radius.
		 */
		Int32 GetMaxElementSizeInLineChart(bool iIsVertical, Int32 iGroupCount, bool iIsIsolatedDotInLine = false, bool iIsLineShapeInCombinationChart = false);

		/*This function handles the size-by logic by considering the min value depends on different cases.
         @param iDataVal, the corresponding data value from dataset.
         @param iRatio, the iRatio val/max_val
         @param iMaxSize, the max size one element can acchieve.
         @return the scaled result.
         */
		double GetScaledSize(double iDataVal, double iMaxSize);
		double GetScaledSizeWithRatio(double iRatio, double iMaxSize);

		/** The following two are used to Get/Set Value axis padding across charts.
		 */
		Int32 GetValueAxisPadding(bool iIsXAxis, bool iIsHighPadding, Int32 iRequiredPadding = 0, Int32 iPaddingIndex = 0);
		void UpdateValueAxisPadding(bool iIsXAxis, bool iIsHighPadding, Int32 iPaddingValue);

		/** The following two are used to decided if one dot is an isolated dot in line chart or just one normal dot in line.
		 */
		bool IsIsolatedDot(const TripleId& irId);
        bool IsIsolatedDotInLine(const TripleId& irId);
		void AddToIsolatedDotList(const TripleId& irTripleId);

        /** Get the condense limit for labels. if the label is less than the limit, then keep it otherwise condesn it.
         */
        Int32 GetCondensedTextLimit(Int32 iFormatType);

        /** Get a FormatFill instance, the filling configuration are GM default.
         */
        FormatFill::Ptr GetBaseGradientFillForGM(unsigned char iAlpha = 255);

        /** Update the filling effect for graph matrix object.
         @param iAngle, this parmater is used when coloring mode = specific or color_by.
         */
        FormatFill::Ptr UpdateGradientEffectForGM(const TripleId& irLineId, FormatFill::Ptr iFillPtr, Dataset::Ptr iDataset, double iAngle, unsigned char iAlpha = 255);

        FormatFill::Ptr UpdateStepGradientEffectForGM(const TripleId& irLineId, FormatFill::Ptr iFillPtr, Dataset::Ptr iDataset, Int32 iGroupInterval , unsigned char iAlpha = 255);

        /* To make the line/shape a bit thicker to fit PM's requirement.
         @param iOriginValue, the value to be adjusted.
         @return the result that is adjusted.
		 */
		Int32 GetAdjustThicknessValue(Int32 iOriginValue);

        /** Sometimes, in Pie chart, we need to change the original triple id into other forms, so we can break one series into multiple ones.
         The following two are the mapping functions from origianl tripleIds to mapped ones, and vice verse.
         */
        //After dataset refactor, we add angleindex and sliceindex for pie slice objec, so we don't need to change the group and series id for pie slice in different subgroup.
        //TripleId GetLogicalTripleId(const TripleId& irRealId);
        //TripleId GetRealTripleId(Int32 iGroupSubIndex, Int32 iSeriesSubIndex, const TripleId& iBaseId);

        /** Get the zoom scale for a specific value axis.
         @param iObjectId, the object id for value axis.
         @returen, the zoom scale, if we did not have the information for the input id, then 1 is returned.
         */
        double GetValueAxisZoomScale(Int32 iValueAxisId);

    protected:
        Int32 hGetLogicalId(Int32 iInputId);
        double hGetMaxRadiusRatio(Int32 chartType);

	};

	class ValueAxis;
	class GraphObject;
	class LayerObject;
	typedef MSynch::SmartPtr<LayerObject> LayerObjectPtr;
	class Transformer;
	/**
     Some commonly values, such as graph width and height, need only one copy for a given chart.
     These values are maintained in ChartContext class. Each GraphObject or GraphObjectManager will
     contain a pointer of ChartContext. In addition, this class provides some utility methods for
     GraphObject and GraphObjectManager.
     */
	class ChartContext
	{
	public:
		/// If we use SmartPtr here, the reference count can be greater than 10000 and an assertion will be hit in SmartBase.h. So we use raw pointer here.
		/// Only ChartImpl holds a SmartPtr to ChartContext.
		typedef ChartContext* Ptr;
		//typedef MSynch::SmartPtr<ChartContext> Ptr;

		ChartContext(Dataset::Ptr iDatasetPtr);
		virtual ~ChartContext();

        void RefreshGraphMatrixContext();

        void SetChartOffset(Int32 iXOffset, Int32 iYOffset);

		/**
         Estimates the width and height of a string for given font name and font size.
         @param irString This is the input string.
         @param iFormatFontPtr Contains font format information.
         @param orWidth Width of the rectangle which  irString occupied.
         @param orHeight Height of the rectangle which  irString occupied.
         */
        //	void GetStringExtent(const MBase::String& irString, FormatFont::Ptr iFormatFontPtr, Int32& orWidth,Int32& orHeight);

		/**
		 Apply font format depending on current stored font name and style
		 @param iFormatFontPtr Contains font format information.
		 */
        //	void ApplyFontFormat(FormatFont::Ptr iFormatFontPtr);

		//	Transformation between Device and Virtual world
		/**
         Convert a rectangle from device-coordinates to virtual-coordinates.
         @param irImageRect Input rectangle in device-coordinates.
         @return A corresponding rectangle in virtual-coordinates.
         */
		Rect2D DVRect(const Rect2D& irImageRect) const;
		/**
         Convert a rectangle from virtual-coordinates to device-coordinates.
         @param irVirtualRect Input rectangle in virtual-coordinates.
         @return A corresponding rectangle in device-coordinates.
         */
		Rect2D VDRect(const Rect2D& irVirtualRect) const;

		/**
         Convert a point from device-coordinates to virtual-coordinates.
         @param irImagePoint Input rectangle in device-coordinates.
         @return A corresponding rectangle in virtual-coordinates.
         */
		Point2D DVPoint(const Point2D& irImagePoint) const;
		/**
         Convert a point from virtual-coordinates to device-coordinates.
         @param irVirtualPoint Input rectangle in virtual-coordinates.
         @return A corresponding rectangle in device-coordinates.
         */
		Point2D VDPoint(const Point2D& irVirtualPoint) const;
		/**
         Convert width from virtual-coordinates to device-coordinates.
         @param iVirtualWidth Input width in virtual-coordinates.
         @return A corresponding width in device-coordinates.
         */
		Int32 VDWidth(Int32 iVirtualWidth) const;
		/**
         Convert height from virtual-coordinates to device-coordinates.
         @param iVirtualHeight Input height in virtual-coordinates.
         @return A corresponding height in device-coordinates.
         */
		Int32 VDHeight(Int32 iVirtualHeight) const;
		/**
         Convert width from device-coordinates to virtual-coordinates.
         @param iImageWidth Input width in virtual-coordinates.
         @return A corresponding width in device-coordinates.
         */
		Int32 DVWidth(Int32 iImageWidth) const;
		/**
         Convert height from device-coordinates to virtual-coordinates.
         @param iImageHeight Input height in device-coordinates.
         @return A corresponding height in virtual-coordinates.
         */
		Int32 DVHeight(Int32 iImageHeight) const;

		// Transformation of Radius
		/**
         Convert radius from virtual-coordinates to device-coordinates.
         @param iVirtualRadius Input Radius in virtual-coordinates.
         @return A corrsponding Radius in device-coordinates.
         */
		Int32 VDRadius(Int32 iVirtualRadius) const;

		// Transformation between 3DG Virtual and MSTR Virtual
		/**
         Convert a point from TDG space (-16383, -16383, 16383, 16383) to MSTR space (0, 0, mGraphWidth, mGraphHeight).
         @param iX X-coordinate in TDG space.
         @param iY Y-coordinate in TDG space.
         @param orPoint Output a point in MSTR space.
         */
		void TDGToMSTRPoint(Int32 iX, Int32 iY, Point2D& orPoint) const;
		/**
         Convert a point from MSTR space to TDG space.
         @param irPoint A point in MSTR space.
         @param orX X-coordinate in TDG space.
         @param orY Y-coordinate in TDG space.
         */
		void MSTRToTDGPoint(const Point2D& irPoint, Int32& orX, Int32& orY) const;
		/**
         Convert a rectangle from TDG space to MSTR space.
         @param iTop Y-coordinate of top edge in TDG space.
         @param iLeft X-coordinate of left edge in TDG space.
         @param iBottom Y-coordinate of bottom edge in TDG space.
         @param iRight X-coordinate of right edge in TDG space.
         @param orRect Output a rectangle in MSTR space.
         */
		void TDGToMSTRRect(Int32 iTop, Int32 iLeft, Int32 iBottom, Int32 iRight, Rect2D& orRect) const;
		/**
         Convert a rectangle from MSTR space to TDG space.
         @param orRect A rectangle in MSTR space.
         @param orTop Y-coordinate of top edge in TDG space.
         @param orLeft X-coordinate of left edge in TDG space.
         @param orBottom Y-coordinate of bottom edge in TDG space.
         @param orRight X-coordinate of right edge in TDG space.
         */
		void MSTRToTDGRect(const Rect2D& irRect, Int32& orTop, Int32& orLeft, Int32& orBottom, Int32& orRight) const;
		/**
         Convert height from TDG space to MSTR space.
         @param iTDGHeight Height in TDG space.
         @param orMSTRHeight Height in MSTR space.
         */
		void TDGToMSTRHeight(Int32 iTDGHeight, Int32& orMSTRHeight) const;

		//Access
		/**
         Set graph width and height.
         @param iGraphWidth Specify a new graph width.
         @param iGraphHeight Specify a new graph height.
         */
		void SetGraphSize(Int32 iGraphWidth, Int32 iGraphHeight);
		/**
         Get graph width.
         @return Current graph width.
         */
		Int32 GetGraphWidth() const;
		/**
         Get graph height.
         @return Current graph height.
         */
		Int32 GetGraphHeight() const;
		/**
         Set 3D projection area. This function is only called in 3D Scatter chart.
         @param irProjectionArea Specify the projection area.
         */
		void SetProjectionArea(const Rect2D& irProjectionArea);
		/**
         Set manual layout.
         @param iManualLayout Indicates whether the layout is manual or automatic.
         */
		void SetManualLayout(bool iManualLayout);
		/**
         Checks whether current layout is manual or automatic.
         @return Return true if current layout is manual. Otherwise, return false.
         */
		bool IsManualLayout() const;
		/**
         Set whether the frame needs to be recalculated or not.
         @param iRecalculate Indicates the frame needs to be recalculated or not.
         */
		void SetRecalculateFrame(bool iRecalculate);
		/**
         Checks whether the frame needs to be recalculated or not.
         @return Return true if the frame needs to be recalculated. Otherwise, return false.
         */
		bool IsRecalculateFrame() const;
		/**
         This method tells ChartContext whether current line width is odd or not.
         @param iIsLineWidthOdd true = line width is odd. false = line width is even.
         */
		void SetLineWidthOdd(bool iIsLineWidthOdd);
		/**
         This method checks whether adjusted line path is needed or not.
         For vector graph output, this method will always return false.
         For other output, this method check whether current line width is odd or not.
         @return Return true if current line width is odd. Otherwise, return false.
         */
		bool NeedAdjustedLinePath() const;
		/**
         The method tells ChartContect whether the mode of font size and line width is aboslute or relative.
         @param iAbsolute true = absolute mode, false  = relative mode.
         */
		void SetAbsoluteFontLineSize(bool iAbsolute);
		/**
         Checks whether the mode of font size and line width is aboslute or relative.
         @return true = absolute mode, false  = relative mode.
         */
		bool IsAbsoluteFontLineSize() const;
		/**
		 Enable or disable support for advanced chart types.
		 */
		void SetAdvancedChartTypeSupport(bool iAdvancedChartTypeSupport);
		/**
		 Enable or disable support for advanced chart types.
		 */
		bool HasAdvancedChartTypeSupport() const;
		/**
		 Whether the currect chart type is an advanced chart type.
		 */
		bool IsAdvancedChartType() const;

		/**
		 Set a new Dataset.
		 */
		void SetDataset(Dataset::Ptr iDatasetPtr);
		/**
         Get the dataset ptr
         */
		Dataset::Ptr GetDataset();

		/**
         Get zoom scale.
         */
		double GetZoomScale(bool iIsHonest = true);

		/**
         Convert font size from virtual-coordinates to device-coordinates.
         @param iorSize Pass in virtual size as the input. Pass out device size as the output.
         */
		void VDFontSize(Int32& iorSize) const;
		/**
         Convert font size from device-coordinates to virtual-coordinates.
         @param iorSize Pass in device size as the input. Pass out virtual size as the output.
         */
		void DVFontSize(Int32& iorSize) const;
		/**
         Convert line width from virtual-coordinates to device-coordinates.
         @param iorSize Pass in virtual size as the input. Pass out device size as the output.
         */
		void VDLineWidth(Int32& iorSize) const;

		//Factory methods for FormatLine/FormatFill/FormatFont
		/**
         Factory method of FormatLine.
         @param irTripleId Specifies the TripleId of the FormatLine owner.
         @param iCollectionType specifies the collection type.
         @return A pointer to the requred FormatLine object.
         */
		virtual FormatLine::Ptr GetFormatLine(const TripleId& irTripleId, Int32 iCollectionType, EnumShapeType iShape = _GENERIC) const = 0;
		/**
         Factory method of FormatFill.
         @param irTripleId Specifies the TripleId of the FormatFill owner.
         @param iCollectionType specifies the collection type.
         @return A pointer to the requred FormatFill object.
         */
		virtual FormatFill::Ptr GetFormatFill(const TripleId& irTripleId, Int32 iCollectionType, EnumShapeType iShape = _GENERIC) const = 0;
		/**
		 * Factory method of FormatFill.
		 * Get FormatFill from Metric definition instead of graph binary.
		 * Currently this is only applied to riser objects (and the correponding legend markers)
		 * with 'Apply metric formatting when available' checked.
		 * by zshe.
		 * @param irTripleId TripleId of the riser object.
		 * @return A pointer to the Metric FormatFill object, NULL if 'Series color' is '<default>' or 'From Graph'
		 */
		virtual FormatFill::Ptr GetMetricFormatFill(const TripleId& irTripleId);
		/**
         Factory method of FormatFont.
         @param irTripleId Specifies the TripleId of the FormatFont owner.
         @return A pointer to the requred FormatFont object.
         */
		virtual FormatFont::Ptr GetFormatFont(const TripleId& irTripleId) const = 0;
		/**
         Get property value by TripleId and property ID.
         @param iPropertyId Specifies a property ID.
         @param irTripleId Specifies the TripleId of the property owner .
         @param opValue Stores the property value.
         @return Return true if a property value is fetched successfully. Otherwise, return false.
         */
		virtual bool GetProperty(Int32 iPropertyId, const TripleId& irTripleId, CComVariant* opValue) const;
		/**
         Set property value by TripleId and property ID.
         @param iPropertyId Specifies a property ID.
         @param irTripleId Specifies the TripleId of the property owner .
         @param ipValue Stores the property value.
         */
		virtual void PutProperty(Int32 iPropertyId, const TripleId& irTripleId, CComVariant* ipValue);

		/**
         Get location by property ID. The location will be in device coordinate.
         @param iPropertyId Specifies a property ID.
         @param opTop Y-coordinate of top edge.
         @param opLeft X-coordinate of left edge.
         @param opBottom Y-coordinate of bottom edge.
         @param opRight X-coordinate of right edge.
         */
		bool GetLocation(Int32 iPropertyId, Int32* opTop, Int32* opLeft, Int32* opBottom, Int32* opRight);

		/**
         Get location by property ID. The location will be in device coordinate.
         @param iPropertyId Specifies a property ID.
         @param opRect Outputs the location information.
         */
		bool GetLocation(Int32 iPropertyId, Rect2D* opRect);

		/**
         Put location by property ID. The input location is in device coordinate.
         @param iPropertyId Specifies a property ID.
         @param iTop Y-coordinate of top edge.
         @param iLeft X-coordinate of left edge.
         @param iBottom Y-coordinate of bottom edge.
         @param iRight X-coordinate of right edge.
         */
		void PutLocation(Int32 iPropertyId, Int32 iTop, Int32 iLeft, Int32 iBottom, Int32 iRight);

		/**
         Put location by property ID. The input location is in device coordinate.
         @param iPropertyId Specifies a property ID.
         @param iRect Specifies the location information.
         */
		void PutLocation(Int32 iPropertyId, const Rect2D& iRect);

		/**
         Nothing needs to be done in ChartContext::PutChartEngine(). GraphStyleContext::PutChartEngine() will put ipChart into GraphStyle.
         @ipChart Points to a Chart instance.
         */
		virtual void PutChartEngine(Chart* ipChart);

		/**
         Get the threshold fill color.
         @param iSeriesId Specifies the series id.
         @param iGroupId Specifies the group id.
         @param opValue Stores the color value.
         */
        FormatFill::Ptr GetThresholdFillColor(Int32 iSeriesId, Int32 iGroupId);//, CComVariant* opValue);

        void UpdateLineColorWithThreshold(const TripleId& irTripleId, FormatLine* iopFormatLine, Int32 iCollectionType);
		/**
         Parse number format property value into ChartAdvFormat.
         @param irValue Number format property value.
         @parma orFormat Parsing result.
         */
		void ParseValueToFormat(VARIANT& irValue, ChartAdvFormat& orFormat);


		//1.	Transform Primitives
		//1.1 SaveState
		/**
         Save current status of Cairo.
         */
		void SaveState();
		//1.2 Rotate
		/**
         Rotate current image drawn by Cairo.
         @param irAnchor Indicates the anchor point for rotation.
         @param iRadian Indicates a rotation angle.
         */
		void Rotate(const Point2D& irAnchor, double iRadian);
		//1.3 Scale
		/**
         Scale current image drawn by Cairo.
         @param iX Scale proportion along X-axis.
         @param iY Scale proportion along Y-axis.
         */
		void Scale(const double iX, const double iY);
		//1.3 Restore state
		/**
         Restore Cairo status saved by SaveState().
         */
		void RestoreState();

		//1.4 Clip
		/**
         Clip Cairo.
         */
		void Clip();

		//2.	Draw Primitives
		//2.1 Polgyon
		/**
         Draw a polygon using Cairo. The coordinates are Int32 integers.
         @param irPoints Represents a polygon to be drawn.
         @param iIsLinePath Indicates whether current path is a line path or fill path.
         */
		template<class T>
		void DrawPolygon(const std::vector< Point<T> >& irPoints, bool iIsLinePath = true, bool iIsCurveAnimation = false, FPoint2D* iBasePoint = NULL, unsigned char iLineStyle = LS_SOLID);
		//2.2 Straight line
		/**
         Draw a straight line using Cairo. The coordinates are Int32 integers.
         @param irStartPoint Indicates the start point of the straight line to be drawn.
         @param irEndPoint Indicates the end point of the straight line to be drawn.
         */
		template<class T>
		void DrawStraightLine(const Point<T>& irStartPoint, const Point<T>& irEndPoint);
		//2.3 Poly line
		/**
         Draw a polyline using Cairo. The coordinates are Int32 integers.
         @param irPoints Represents a polyline to be drawn.
         @param iIsLinePath Indicates whether a line path is needed or not.
         */
		template<class T>
		void DrawPolyLine(const std::vector< Point<T> >& irPoints, bool iIsLinePath = true, bool iIsCurveAnimation = false, FPoint2D* iBasePoint = NULL, unsigned char iLineStyle = LS_SOLID);
		//2.4 Rectangle
		/**
         Draw a rectangle using Cairo. The coordinates are float values.
         @param irRect Represents a rectangle to be drawn.
         @param iIsLinePath Indicates whether current path is a line path or fill path.
         */
		void DrawRectangle(const Rect2D& irRect, bool iIsLinePath);
		//2.5 Arc
		/**
         Draw an arc countclockwise using Cairo.
         @param irCenter Represents the center point of the arc to be drawn.
         @param iRadius Represents the radius of the arc to be drawn.
         @param iStartAngle Represents the start angle of the arc to be drawn.
         @param iEndAngle Represents the end angle of the arc to be drawn.
         */
		void DrawArc(const FPoint2D& irCenter, const float iRadius, const float iStartAngle, const float iEndAngle, bool iNewPath = true);
		/**
         Draw an arc clockwise using Cairo.
         @param irCenter Represents the center point of the arc to be drawn.
         @param iRadius Represents the radius of the arc to be drawn.
         @param iEndAngle Represents the end angle of the arc to be drawn.
         @param iStartAngle Represents the start angle of the arc to be drawn.
         */
		void DrawArcNegative(const FPoint2D& irCenter, const float iRadius, const float iEndAngle, const float iStartAngle, bool iNewPath = true);
		//2.6 Sector
		/**
         Draw a sector using Cairo.
         @param irCenter Represents the center point of the sector to be drawn.
         @param iRadius Represents the radius of the sector to be drawn.
         @param iStartAngle Represents the start angle of the sector to be drawn.
         @param iEndAngle Represents the end angle of the sector to be drawn.
         */
		void DrawSector(const FPoint2D& irCenter, const float iRadius, const float iStartAngle, const float iEndAngle);
		/**
         Draw a ring sector using Cairo.
         @param irCenter Represents the center point of the ring sector to be drawn.
         @param iOutRadius Represents the outer radius of the ring sector to be drawn.
         @param iInRadius Represents the inner radius of the ring sector to be drawn.
         @param iStartAngle Represents the start angle of the ring sector to be drawn.
         @param iEndAngle Represents the end angle of the ring sector to be drawn.
         */
		void DrawRingSector(const FPoint2D& irCenter, const float iOutRadius, const float iInRadius, const float iStartAngle, const float iEndAngle);
		//2.7 Cylinder (for pie)
		/**
         Draw a cylinder using Cairo.
         @param irCenter Represents the center point of the cylinder to be drawn.
         @param iRadius Represents the radius of the cylinder to be drawn.
         @param iStartAngle Represents the start angle of the cylinder to be drawn.
         @param iEndAngle Represents the end angle of the cylinder to be drawn.
         @param iDepth Represents the depth of the cylinder to be drawn.
         */
		void DrawCylinder(const FPoint2D& irCenter, const float iRadius, const float iStartAngle, const float iEndAngle, const float iDepth);
        //		/**
        //		Draw a string using Cairo.
        //		@param irPoint The anchor point of the string.
        //		@param ipString The string to be drawn.
        //		*/
        //		void DrawString(const Point2D& irPoint, const char* ipString);
		//2.8 Bevel effect
		/**
         Apply donut effect to a ring.
         @param irCenter The center point of the ring.
         @param iOuterRadius The outer radius of the ring.
         @param iInnerRadius The inner radius of the ring.
         @param iFormatFillPtr The fill format info of the ring.
         */
		template<class T>
		void ApplyDonutToRing(const Point<T>& irCenter, T iOuterRadius, T iInnerRadius, const FormatFill::Ptr iFormatFillPtr, EnumRadientGradientUsage iUsage = RG_RING, bool iHasExtraPara = false, int iExtraDepth = 0, int iExtraIntensity = 0);
		/**
         Apply beveled frame effect to a rectangle.
         @param irRect The input rectangle.
         @param iBevelDepth Specifies the depth of the beveled frame.
         @param irLTColor Specifies the color the the left top part of the beveled frame.
         @param iLTAlpha Specifies the transparency the the left top part of the beveled frame.
         @param irRBColor Specifies the color the the right bottom part of the beveled frame.
         @param iRBAlpha Specifies the transparency the the right bottom part of the beveled frame.
         */
		void ApplyBeveledFrameToRectangle(const Rect2D& irRect, double iRotationAngle, Int32 iBevelDepth, const RGBColor& irLTColor, Int32 iLTAlpha, const RGBColor& irRBColor, Int32 iRBAlpha);
#ifdef _VE_CE
		//Draw gauge marker
		void DrawGaugeMarker(FPoint2D* iBasePoint, Int32& iMarkerSize, Int32& iMarkerShape, float iAngle, short iSize);

		/**
		Draw a dashed line. The coordinates are short integers.
		*/
		template<class T>
		void DrawDashedLine(const Point<T>& irStartPoint, const Point<T>& irEndPoint, std::vector<double>& ipLinePattern);

		/**
		Apply donut effect to a rect.
		@param irStartPoint The start point of donut.
		@param irEndPoint The end point ofdonut.
		@param iFormatFillPtr The fill format info of the rectangle.
		*/
		void ApplyDonutToRect(const Point<double>& irStartPoint, const Point<double>& irEndPoint, const FormatFill::Ptr iFormatFillPtr);
#endif
		/**
		 Begin a new path.
		 */
		void NewPath();
		/**
		 Close current path.
		 */
		void ClosePath();

		/**
		 Move to a point.
		 @param iX,iY Specifies a point.
		 */
		void MoveTo(double iX, double iY, bool iIsCurveAnimation = false, FPoint2D* iBasePoint = NULL);

		/**
		 Line to a point.
		 @param iX,iY Specifies a point.
		 */
		void LineTo(double iX, double iY, bool iIsCurveAnimation = false, FPoint2D* iBasePoint = NULL);
#ifdef _VE_CE
		/**
		 Draw a cubic Bezier curve from the current postion of cursor to (iX, iY)
		 @param iCP1X specifies the X-axis value of the first control point.
		 @param iCP1Y specifies the Y-axis value of the first control point.
		 @param iCP2X specifies the X-axis value of the second control point.
		 @param iCP2Y specifies the Y-axis value of the second control point.
		 @param iX specifies the X-axis value of the end point.
		 @param iY specifies the Y-axis value of the end point.
		 */
		void BezierCurveTo(double iCP1X, double iCP1Y, double iCP2X, double iCP2Y, double iX, double iY);
#endif
		/**
		 Set fill color and alpha.
		 @param iRed,iGreen,iBlue Specifies a color.
		 @param iAlpah Specifies an opacity level.
		 */
		void SetRGBAFill(float iRed, float iGreen, float iBlue, float iAlpha);

		/**
		 Set line color and alpha.
		 @param iRed,iGreen,iBlue Specifies a color.
		 @param iAlpah Specifies an opacity level.
		 */
		void SetRGBAStroke(float iRed, float iGreen, float iBlue, float iAlpha);

		/**
		 Fill the current path.
		 */
		void FillPath();

		/**
		 Stroke the current path.
		 */
		void StrokePath();

		/**
		 Set the pattern of dash lines.
		 @param ipLinePattern An array which specifies the line pattern. Null for solid line.
		 @param iDashNumber Specifies the number of element in ipLinePattern. 0 for solid line.
		 */
		void SetLineDash(double* ipLinePattern, Int32 iDashNumber);

		/**
		 Set the line width.
		 @param iLineWidth The input line width.
		 */
		void SetLineWidth(Int32 iLineWidth);

		/**
		 Set the font size.
		 @param iFontSize The input font size.
		 */
		void SetFontSize( Int32 iFontSize);
		/**
		 Set the scale ratio for font rendering
		 @param iRatio, the ratio to scale
		 */
		void SetFontScale(double iRatio = 1.0);
		/**
		 Get the scale ratio for font rendering
		 @return the ratio to scale
		 */
		double GetFontScale() const;


		/**
		 Set the line join type.
		 @param iLineJoinType If it is an enumeration, where 0 means Miter, 1 means Bevel, and 2 mean Round.
		 */
		void SetLineJoin(EnumLineJoinType iLineJoinType);

		/**
		 Get the current line join type.
		 */
		EnumLineJoinType GetLineJoin();

		/**
         Save Cairo surface to file.
         @param iFormat Specifies the image format. Should be a value in EnumPictureFormatType.
         @param ipOutFile Specify the file handle of output file.
         @return Return true if the required image file is saved into a file successfully.
         Otherwise, return false.
         */
		bool SaveSurfaceToFile(Int32 iFormat, FILE* ipOutFile);
		/**
         Save Cairo surface to file.
         @param iFormat Specifies the image format. Should be a value in EnumPictureFormatType.
         @param oprMemory Stores the output image.
         @param orSize Memory size in bytes.
         @return Return true if the required image is saved into oprMemory successfully.
         Otherwise, return false.
         */
		bool SaveSurfaceToMemory(Int32 iFormat, char*& oprMemory, unsigned Int32& orSize);

#ifdef _VE_CE
		bool SaveSurfaceToString(std::string& orGraphDesc);
		bool SaveSurfaceToVEObjects(std::vector<MVE::VEFormatList>& ioFormatList, std::vector<MVE::VEUnitList>& ioUnitList);
#endif

		/**
         Calculate depth offset according to depth value and depth direction.
         @param iDepth Specifies the depth value.
         @param iAngle Specifies the depth direction.
         @param orOffsetX Horizontal offset.
         @param orOffsetY Vertical offset.
         @param iAbsoluteDepth Indicates whether iDepth is an absolute depth or not.
         If iAbsoluteDepth is true, the following equation will hold: orOffsetX*orOffsetX+orOffsetY*orOffsetY=iDepth*iDepth.
         */
		void CalculateDepthOffset(Int32 iDepth, Int32 iAngle, Int32& orOffsetX, Int32& orOffsetY, bool iAbsoluteDepth = false) const;
		/**
         This method calculates the direction of a depth line.
         This method checks the cross product of depth direction vector and side direction vector.
         @param iSideDireciton The direction of a line which is the generator of a depth line.
         @iDepthDirection Degree of the depth direction. It should be an integer which satisfys
         0<=iDepthDirection<=359.
         @return 0 if the cross product is 0.
         1 if the cross product is positive.
         -1 if the cross product is negative.
         Note that the x direction goes to right and the y direction goes to bottom.
         */
		Int32 NormalDirection(const TVector2D<Int32>& iSideDirection, Int32 iDepthDirection);

		/**
         Calculate the square of a given number.
         @iNumber Specifies a number.
         @return The square of iNumber.
         */
		double Square(double iNumber) const;

		/**
         Get current graph type.
         @return Return current graph type.
         */
		Int32 GetGraphType() const;

		/**
         Check whether current graph type is a Vertical type.
         @return Return true if current graph type is Vertical. Otherwise, return false.
         */
		bool IsVertical() const;

		/**
         Check whether current graph type is a Dual Axis type.
         @return Return true if current graph type is Dual Axis. Otherwise, return false.
         */
		bool IsDualAxis() const;

		/**
         Convert the marker size from dialog size to pixel size.
         @return Return the pixel size of the marker
         */
		Int32 DialogSizeToPixelSize(Int32 iDialogSize) const;

		/**
         Split a rectangle into two rectangle according to some percentage.
         @param irRect The input rectangle.
         @param iIsVertical Indicates whether it is vertically splitted or not.
         @param iPercentage The percentage.
         @param orRectTopRight The top(vertical) or right(horizontal) rectangle.
         @param orRectBottomLeft The bottom(vertical) or left(horizontal) rectangle.
         */
		void SplitRect(const Rect2D& irRect, bool iIsVertical, Int32 iPercentage, Rect2D& orRectTopRight, Rect2D& orRectBottomLeft) const;

		/**
         Get the automatic number format of axis labels.
         @param orFormat The output number format.
         */
		void GetAxisAutoNumberFormat(ChartAdvFormat& orFormat);
		void GetAxisAutoNumberFormat(ChartAdvFormat& orFormat, const std::vector<Int32>& irMetricSet);

        /**
         *This one is used for chart that has multiple value axis, currently we just consider scatter/bubble, in future we might need to support dural value axis case.
         */
        void GetAxisAutoNumberFormat(ChartAdvFormat& orFormat, const TripleId& irTripleId);

		/**
         This function gets the major graph type from the graph type. The major graph type is used to generate different PlotManager and Plot.
         */
		Int32 GetGraphMajorType() const;

		/**
         Convert from Point size to Pixel size.
         @param iPtSize Point size.
         @return Pixel size.
         */
		Int32 PtSizeToPixelSize(Int32 iPtSize) const;
		/**
         Convert from Pixle size to Point size.
         @param iPixelSize Pixel size.
         @return Point size.
         */
		Int32 PixelSizeToPtSize(Int32 iPixelSize) const;

		/**
         Check if we should reverse the drawing order of the series.
         @param ipValueAxis The input value axis.
         @param Indicate whether the chart is vertical or horizontal.
         @param iDepthDirection The input depth direction.
         */
		bool IsReverseSeriesOrder(ValueAxis* ipValueAxis, bool iIsVertical, Int32 iDepthDirection) const;

		/**
         Check if we should reverse the drawing order of the groups.
         @param Indicate whether the chart is vertical or horizontal.
         @param iDepthDirection The input depth direction.
         */
		bool IsReverseGroupOrder(bool iIsVertical, Int32 iDepthDirection) const;

		/**
         Create a cairo PDF surface for stream.
         @return Return a cairo PDF surface.
         */
		cairo_surface_t* CreatePDFSurfaceForStream();

		/**
         Create a cairo SVG surface for stream.
         @return Return a cairo SVG surface.
         */
		cairo_surface_t* CreateSVGSurfaceForStream();

		/**
         Get standard coordinates from polar coordinates.
         @iAngle Specifies an angle in radian.
         @iRadius Specifies a radius.
         @irCenter Specifies a center point.
         @return Return the standard coordinates acoording the input parameters.
         */
		Point2D GetPointByAngleRadiusCenter(double iAngle, double iRadius, const Point2D& irCenter) const;
		/**
         This function determines whether there is intersection between two rectangle and calculates the intersection part if there is.
         @param irRectA The first input rectangle.
         @param irRectB The second input rectangle.
         @param orRect The intersection rectangle.
         @param True means there is intersection while false means not.
         */
		bool RectangleIntersection(const Rect2D& irRectA, const Rect2D& irRectB, Rect2D& orRect) const;

		/**
         This function calculates the union rectangle of the two rectangles.
         @param irRectA The first input rectangle.
         @param irRectB The second input rectangle.
         @param orRect The union rectangle.
         */
		void RectangleUnion(const Rect2D& irRectA, const Rect2D& irRectB, Rect2D& orRect) const;

		/**
		 Get transformer which is useful in 3D chart.
		 @return Return a pointer to the transformer.
		 */
		Transformer* GetTransformer();
#ifndef _VE_CE
		/** Set Core Graphics context.
		 @param ipCGContext Specifies a Core Graphics context.
		 */
		void SetCGContext(CGContext* ipCGContext);
#endif
        bool GetIsCategoryXAxisForPartialRendering();
        void SetIsCategoryXAxisForPartialRendering(bool isForPartialRendering);

		/**
         Check whether current chart type is pareto or not.
         @return Return true if current chart type is pareto. Otherwise, return false.
         */
		bool IsParetoChart() const;

		/**
         Check whether current chart type is stacked or not.
         @return Return true if current chart type is stacked. Otherwise, return false.
         */
		bool IsStackedChart() const;

		/**
         Get the angle of a vector.
         @irVector Specifies a vector.
         @return return the angle of irVector in [-PI/2,3*PI/2]. For example, VectorToAngle(DVector2D(0, 1)) is PI/2.
         */
		double VectorToAngle(const DVector2D& irVector) const;

		/**
         Set image format.
         @iFormat Specifies an image format.
         */
		void SetImageFormat(Int32 iFormat);

		/**
         Get the current image format
         @return Return the current image format.
         */
		Int32 GetImageFormat() const;

		/** Access method for mPushGroupFlag.
         @param iPushGroupFlag Indicates whether cairo_push_group() has been called or not.
         */
		void SetPushGroup(bool iPushGroupFlag);

		/** Access method for mPushGroupFlag.
         @return Indicates whether cairo_push_group() has been called or not.
         */
		bool GetPushGroup() const;

		/**
         Check whether the graph object specified by irTripleId is beyond the riser count limit.
         @param irTripleId Specifies the graph object.
         @return True means the riser count limit is not reached, the current graph object could be added to the image map. False means the opposite.
         */
		bool CheckRiserCountLimit(const TripleId& irTripleId) const;

		/**
         Convert a double value to fixed point number with 2 non-zero digits after decimal point. This is useful for PDF size optimization.
         @param iValue Input value.
         @return Return the closest fixed point number to iValue.
         */
		double DoubleToFixedPointNumber(double iValue) const;

		/**
         Check whether current output image is vector graph or not.
         @return Return true if current output image is vector graph. Otherwise, return false.
         */
		bool IsVectorGraph() const;

		/**
         Check whether current chart context is generic context or graphstyle contest.
         @return Return true if generic, if graphstyle return false.
         */
		virtual bool IsGenericContext() const = 0;

		/**
		 Used in iPhone Chart
		 */
		void SetChartComponent(EnumChartComponent iChartComponent);

		EnumChartComponent GetChartComponent() const;

		/**
		 Get axes state.
		 @return Return current axes state.
		 */
		AxesState* GetAxesState();
		/**
		 Restore zoom state.
		 @param ipDataset Specifies a dataset.
		 */
		void RestoreZoomState(Dataset* ipDataset);

		/**
		 Save Dataset state.
		 @param ipDataset Specifies a dataset.
		 */
		void SaveDatasetState(Dataset* ipDataset);
		/**
		 Restore dataset.
		 If SaveDatasetState() has been called previously, the data set will be restored to the status captured by SaveDatasetState(). Otherwise, this method will do nothing.
		 @param ipDataset Specifies a dataset.
		 */
		void RestoreDataset(Dataset* ipDataset);

		/** Set the visibility of value axis part.
		 @param iIsValueAxisShown Indicates whether value axis part should be shown.
		 */
		void SetValueAxisShown(bool iIsValueAxisShown);

		/** Check whether value axis part should be shown or not.
		 @return Return true if value axis part should be shown. Otherwise, return false.
		 */
		bool IsValueAxisShown() const;

		/** Enable or disable animation.
		 @param iUseAnimation Indicates whether animation is needed or not.
		 */
		void EnableAnimation(bool iUseAnimation);

		/** Check whether animation is needed or not.
		 @return Return true if animation is needed. Otherwise, return false.
		 */
		bool IsAnimationEnabled() const;

        bool IsUseAnimation() const;

		/** Set the duration of animation.
		 @param iSeconds Specifies animation duration in seconds.
		 */
		void SetAnimationDuration(double iSeconds);

		/** Get the duration of animation.
		 @return Return the duration of animation in seconds.
		 */
		double GetAnimationDuration() const;

        // lasso selections
        void EnterGrayoutMode();
        void ExitGrayoutMode();
        bool IsGrayoutMode() const;
        void EnterDuplicateMode();
        void ExitDuplicateMode();
        bool IsDuplicateMode() const;

		/**
		 Set the key of corresponding template node. This key can be used for logging.
		 @param irKey Specifies a template node key.
		 */
		void SetTemplateNodeKey(const std::string& irKey);
		/**
		 Get the key of corresponding template node. This key can be used for logging.
		 @return Return current template node key.
		 */
		std::string GetTemplateNodeKey() const;

		/** Specifies whether current chart is a time series chart or not.
		 @param iIsTimeSeries Indicates whether current chart is a time series chart or not.
		 */
		void SetTimeSeriesChart(bool iIsTimeSeries);
		/** Check whether current chart is a time series chart or not.
		 @return Return true if current chart is a time series chart. Otherwise, return false.
		 */
		bool IsTimeSeriesChart() const;

        /**
         *To set which part of the chart should be rendered.
         *@param iIntegrity, enum from DssTimeSeriesItems
         */
        void SetIntegrity(Int32 iIntegrity);
        /**
         *To get which part of the chart should be rendered.
         *@return the integritey.
         */
        Int32 GetIntegrity();

		/** Specifies whether current chart is hiding axis elements.
		 @param iHide Indicates whether should hide gridline/labels/.
		 */
		void HideAxisElements(bool iHide);
		/** Check whether whether axis elements are hidden or not.
		 @return Return true if axis elements are hidden. Otherwise, return false.
		 */
		bool IsHidingAxisElements() const;

		/**
		 Enable or disable macro chart.
		 @param iIsMicroChart Specifies whether current chart is a micro chart or not.
		 */
		void EnableMicroChart(bool iIsMicroChart);
		/** Check whether current chart is a micro chart or not.
		 @return Return true if current chart is a micro chart. Otherwise, return false.
		 */
		bool IsMicroChart() const;

		/**
		 Enable or disable graph matrix.
		 @param iIsGraphMatrix Specifies whether current chart is a graph matrix or not.
		 */
		void EnableGraphMatrix(bool iIsGraphMatrix);

		/** Check whether current chart is a graph matrix or not.
		 @return Return true if current chart is a graph matrix. Otherwise, return false.
		 */
		bool IsGraphMatrix() const;

        EnumWidgetType GetWidgetType();

        void EnableVIDocument(bool iIsVIDocument);

        bool IsVIDocument() const;

		/**
		 Enable or disable voice over.
		 @param iIsVoiceOverMode Specifies whether current chart is in voice over mode or not.
		 */
		void EnableVoiceOver(bool iIsVoiceOverMode);

		/** Check whether current chart is a in voice over mode or not.
		 @return Return true if current chart is a in voice over mode. Otherwise, return false.
		 */
		bool IsVoiceOver() const;

		/** Check whether Legend should be built by group.
		 @return Return true if legend should be built by group. Otherwise, return false.
		 */
		bool IsLegendByGroup() const;


		/** Check whether current chart is a grid chart and each marker has its own color.
		 @return Return true if it is. Otherwise, return false.
		 */
		//bool IsGridChartWithVariousColoring() const;

		/**
		 Enable or disable uniform axis for graph matrix.
		 @param iUseUniformAxis Indicates whether uniform axis should be used.
		 */
		void EnableUniformAxis(bool iUseUniformAxis);
		/** Check whether uniform axis is enabled or not.
		 @return Return true if uniform axis is enabled. Otherwise, return false.
		 */
		bool IsUniformAxisMode() const;
		/** Check whether current output is data area for uniform axis.
		 @return Return true if current output is data area for uniform axis. Otherwise, return false.
		 */
		bool IsUADataAreaOutput();

		/**
		 Speicifies whehter entire chart is needed.
		 @param iIsEntireChart Speicifies whehter entire chart is needed.
		 */
		void SetEntireness(bool iIsEntireChart);
		/** Check whether entire chart is needed.
		 @return Return true if entire chart is needed. Otherwise, return false.
		 */
		bool IsEntireChart() const;

		/**
		 Specifies the usage of current image.
		 @param iImageUsage Specifies the usage of current image.
		 */
		void SetImageUsage(Int32 iImageUsage);
		/**
		 Check the usage of current image.
		 @return Return the usage of current image.
		 */
		Int32 GetImageUsage() const;

		/**
		 Set FormatManager.
		 @param iFormatManagerPtr Specifies a new FormatManager.
		 */
		void SetFormatManager(FormatManager::Ptr iFormatManagerPtr);

		/**
		 Get the pointer to FormatManager instance
		 @Return &mFormatManagerPtr
		 */
		FormatManager::Ptr GetFormatManager() const;

		/**
		 Get uniform axis context for graph matrix.
		 @return Return a uniform axis context.
		 */
		UniformAxisContext* GetUniformAxisContext();

		/**
		 Get graph matrix context for graph matrix.
		 @return Return a graph matrix context.
		 */
		GraphMatrixContext* GetGraphMatrixContext();

        Int32 GetChartXOffset() const;
        Int32 GetChartYOffset() const;
#ifndef _VE_CE
		/**
		 Get mpPrivateCGContext.
		 @return Return mpPrivateCGContext.
		 */
		CGContext* GetPrivateCGContext();
#endif
		/**
         Indiacates whether to use font anti-alias.
         @param iFontAntiAlias true = use font anti-alias. false = not use font anti-alias.
         */
		void SetFontAntiAlias(bool iFontAntiAlias);

		/**
         Check whether to use font anti-alias.
         @return Return true if font anti-alias is needed.
         */
		bool IsFontAntiAlias() const;

		/**
		 Indiacates whether the background of the whole graph is opaque.
		 @param iBackgroundOpaque true = Background of the whole graph is opaque. false = Otherwise.
		 */
		void SetBackgroundOpaque(bool iBackgroundOpaque);

		/**
		 Check whether the background of the whole graph is opaque.
		 @return true = Background of the whole graph is opaque. false = otherwise.
		 */
		bool IsBackgroundOpaque() const;

        // Get current Frame of Graph, used for GM Partial rendering
        bool GetCurrentGraphFrame(Rect2D& iorRect) const;
        void SetCurrentGraphFrame(Int32 iX, Int32 iY, Int32 iWidth, Int32 iHeight);

		/**
		 * Setter of member variable mApplyMetricFormatting.
		 */
		void SetApplyMetricFormatting(bool iApplyMetricFormatting);

		/**
		 * Getter of member variable mApplyMetricFormatting.
		 */
		bool GetApplyMetricFormatting() const;

		/**
         Check whether the background is bright or not.
         @iIsRecursive, when set ture and when frame and background are both transparent, we will search the first non-transpanent view up along the
         view tree and use its background color as the color for calculation.
         This is for the highlight effect when using graph as selector.
         */
		bool IsBackgroundBright(bool iIsRecursive = true) const;

		/**
		 For a given polygon, remove the edges if the edge length is zero.
		 In other words, merge the two adjacent points if they are the same
		 @param iorPolygon Specifies the polygon to be processed.
		 */
		void RemoveZeroEdgeInPolygon(std::vector<Point2D>& iorPolygon) const;

        void Reset();

        /**
         Trigger, called when animation is finished.
         */
		void AnimationDidFinished();

        /**
         Whether to disable the animation while zoom happens.
         @param iDisableAnimation, true means animation should be disabled for zooming
         */
        void DisableAnimationForZoom(bool iDisableAnimation);

        /**
         Getter method, is animation disabled while zooming.
         */
        bool IsAnimationDisabledForZoom();

        /**
         To decide if a graph object is data label.
         */
        bool IsDataLabels(const TripleId& irId) const;

		void SetHighlightSelectedObjects(LayerObjectPtr iHighlightLayerObjectPtr);
        void GetHighLightSelectedObjects(LayerObjectPtr &orHighlightLayerObjectPtr);
        void ResetHighlightObjectList(std::vector<TripleId>* ipInputList);
        void GetHighLightObjectList(std::vector<TripleId>& iorCurrentList);
        bool isBarAreaInAxis(const TripleId& irId);

#ifdef _VE_CE
		void AddNeedle(char iNeedleStyle, Point2D iBasePoint, short iAngle, float iSize);
		void FillSimple(const FormatFill* ipFill, float iDarkerFactor, RGBColor* iColor = NULL, bool iHasExtraPara = false);
		void FillPattern(const FormatFill* ipFill);
		void FillGradient(const FormatFill* ipFill, const Rect2D& irBoundingRect, float iDarkerFactor);
		void FillEdgeType(const FormatFill* ipFill, const std::vector<Point2D>& irPolygon);
		void FillDonutOrSphere(const FormatFill* ipFill, const Rect2D& irBoundingRect, bool iIsCircularShape);
		void FillBorderMetallic(const FormatFill* ipFill, short iBorderUse, double iCPX, double iCPY, double iRadius, double iThickness);
		void FillRadientGradient(const FormatFill* ipFill, EnumRadientGradientUsage iRGUsage, double iCPX, double iCPY, double iInnerRadius, double iOuterRadius, bool iHasExtraPara = false, int iExtraDepth = 0, int iExtraIntensity = 0);
		void MakeDarker(const FormatFill* ipFill, float iFactor);
		void RestoreColor(const FormatFill* ipFill);
		void ApplyLineFormat(const FormatLine* ipLine);
		void AddFormatedText(const FormatFont* ipFont, const Point2D& irStartPoint, const std::string& irString);
		void BeginFirstFrame();
		void BeginLastFrame();
		void EndFrame();
		void EndUnit();
#else
		/**
         Contains the current state of the rendering device, including coordinates of yet to be drawn shapes.
         Cario context, as cairo_t objects are named, are central to cairo and all drawing with cairo is always
         done to a cairo_t object.
         */
		CGContext* mpCGContext;

		/// Maintains the current path.
		CGPath* mpCGPath;

#endif
		LayerObject* mpBaseLayerObject;

		LayerObject* mpTopRendererLayer;	///< Maintains the address of top rendering layer.

		bool mIsCategoryXAxisNeedPartialRendering;

		Transformer* mpTransformer;
		/// Maintains a pointer to GridGraphViewer. Need to cast it to UIView* before using it.
		void* mpGridGraphViewer;

		/// Maintains a pointer to old ChartManager for animation support.
		void* mpOldChartManager;

		void* mpChartManager;

        void* mpGMCEContext;   //This is the GMCEContext from Widget

        float mGMHeightScaleRatio;
        float mGMWidthScaseRatio;
        bool  mIsTextAsShape;

		/**
         Return one default color from the default color palette for an input series.
         @param iSeriesId the color index, actually we use iSeriesId % max_index
         @return the color returned.
         */
		RGBColor GetSeriesColorFromPalette(Int32 iSeriesId) const;

        RGBColor* GetDefaultContrastColor(Int32 iColorUsage = -1) const;
		
		/// 471270 When there's curve polygon, need to draw legend and category labels on top layer.
		void SetHasCurvePolygon(bool iHasCurvePolygon);
		bool HasCurvePolygon() const;

        bool IsIsolatedDot(const TripleId& irId);
        bool IsIsolatedDotInLine(const TripleId& irId);

        /// Update zoom scale.
        void UpdateZoomScale(void* ipView);

        /// Update zoom scale to high resolution.
        void UpdateHighScaleFactorToViews(void* ipView);
        void UpdateHighScaleFactorToViews();

        /// Whether animation is possible
		bool IsAnimationPossible(double iZoomScale);


        /// Is view A sub view to B
        bool IsSubView(void* ipChildView, void* ipBaseView, bool iIsRecursive);
        bool IsBarChart() const;
        bool IsBulletChart() const;
        bool IsLineChart() const;
        bool IsAreaChart() const;
        bool IsPercentChart() const;
        bool IsPieChart() const;
        bool IsCategoryPie() const;
        EnumGraphMatrixMajorGraphType GetGraphMatrixGraphMajorType() const;

        void SetNeedColorColOffset(bool iIsNeedColorColOffset);

        bool IsBudgeting() const;

        /// Get the information about the graph themes
        ThemeType GetGraphThemes() const;

        void SetHover(const TripleId &iTripleId, bool iIsHover);
        bool GetHover(const TripleId &iTripleId) const;

        /**
		 * Get the estimation of memory usage, excluding those used by generated UIViews.
		 */
		virtual float GetMemoryUsageEstimation() const;

        bool IsAxesStateNULL() const;
        void SetCanvas(ChartCanvas *ipCanvas);
        ChartCanvas* GetCanvas();
        Rect2D GetGraphFrame();
        RGBColor GetColorById(const TripleId& irTripleId, float& orAlpha) const;
	protected:
		/**
         This is a helper method of GetFormatLine().
         @param irTripleId Specifies the TripleId of the FormatLine owner.
         @param iopFormatLine A pointer to FormatLine object.
         */
		void UpdateFormatLine(const TripleId& irTripleId, FormatLine* iopFormatLine) const;
		/**
         This is a helper method of GetFormatFill().
         @param irTripleId Specifies the TripleId of the FormatFill owner.
         @param iopFormatFill A pointer to FormatFill object.
         @param iCollectionType specifies the collection type. This is required by hApplyEnhancedSeriesEffect(),
         which is a helper method of this method.
         */
		void UpdateFormatFill(const TripleId& irTripleId, FormatFill* iopFormatFill, Int32 iCollectionType) const;
		/**
         This is a helper method of GetFormatFont().
         @param irTripleId Specifies the TripleId of the GetFormatFont owner.
         @param iopFormatFont A pointer to GetFormatFont object.
         */
		void UpdateFormatFont(const TripleId& irTripleId, FormatFont* iopFormatFont) const;

		/**
         Map one object ID to another if necessary. For example, DssGraphPieSlice is mapped to DssGraphRiser.
         @param iObjectId Specifies an object ID.
         @return return the object ID after mapping.
         */
		Int32 MapObject(Int32 iObjectId) const;

		/**
         Check whether current graph type is Vertical Bar.
         @return Return true if current graph type is Vertical Bar. Otherwise, return false.
         */
		bool IsVerticalBar() const;
		/**
         For GenericContext, this map is used to store the options from CETest.
         For GraphStyleContext, this map is used to store the resolved title text from MsiGraph.
         The title text in the property map of GraphStyle may contain micros. After these macros are resolved,
         the result text is not suitable to be put into GraphStyle. So this map is used in this case.
         */
		std::map<PropertyKey, CComVariant> mPropertyMap;

        /**
         Usually we will adjust the font size/line width passed from server side according to graph size. But for certain graph types (like budgeting)
         this step is not necessary.
         @return true means should NOT adjust the font size/line width, just use what it was from server side.
         */
        bool NoAdjustOfFontSize(const TripleId& irTripleId) const;
        bool NoAdjustOfLineWidth(const TripleId& irTripleId) const;

        Int32 mGraphWidth;	///< Graph width.
		Int32 mGraphHeight;	///< Graph height.
		Int32 mImageFormat;				///< Keeps the current image format.
		Int32 mUniformAxisLeftMargin;		///< only used by uniform axis. Indicates the margin between Y axis line and Y axis labels.
		Int32 mUniformAxisTopMargin;		///< only used by uniform axis. Indicates the margin at top of data area.
		Int32 mUniformAxisBottomMargin;		///< only used by uniform axis. Indicates the margin between X axis line and X axis labels.


		EnumChartComponent mChartComponent;

		Int32 mGraphType;	///< Graph type.
		Int32 mImageUsage;		///< Indicates the usage of current chart.
		Dataset::Ptr mDatasetPtr;	///< Points to a Dataset object.

		/**	Indicates whether current line width is odd or even.
         If the line width is odd and the image format is not vector graph, a line path needs to be adjusted for better visual effect.
         Reference: http://www.cairographics.org/FAQ/#sharp_lines
         */
		bool mIsLineWidthOdd;
		/**	Detemines font and line size resizing mode.True means absolute. False means relatvie.
         */
		bool mAbsoluteFontLineSize;
		bool mManualLayoutMode;	///< Indicates whether the layout is manual or automatic.
		bool mRecalculateFrame;	///< Indicates whether the frame need to be recalculate or not.
		bool mPushGroupFlag;	///< Indicates whether cairo_push_group() has been called or not.
		bool mIsValueAxisShown;	///< Indicates whether to show value axis part or not.
		bool mIsFontAntiAlias;
		bool mUseAnimation;		///< Indicates whether animation is needed or not.
        bool mUseAnimationForZoom; ///< Indicates whether animation should be supported for the charts genrated due to zoom
		bool mHasAnimatedObject;
		bool mIsTimeSeriesChart;	///< Indicates whether current chart is a time series chart or not.
		bool mIsMicroChart;			///< Indicates whether current chart is a micro chart or not.
		bool mIsUniformAxisMode;	///< Indicates whether uniform axis is enabled.
		bool mIsEntireChart;		///< Indicates whether entire chart is needed.
		bool mIsGraphMatrix;		///< Indicates whether current chart is a graph matrix or not.
        bool mIsVIDocument;
		bool mAdvancedChartTypeSupport;
		bool mHasCurvePolygon;		///< Indicates whether there's curve polygon in current graph.
		bool mIsVoiceOver;			///< Indicates whether currenct chart is in voice over mode.
		bool mIsBackgroundOpaque;	///< Indicates whether the background of current chart is opaque.
		double mAnimationDuration;///< Specifies the duration of animation in seconds;
		double mFontScaleRatio;		  ///< Specify the font scale ratio, default is one.

        bool mIsNeedColorColOffset;
        bool mShouldHideAxisElements; ///< Whether axis elements should be hidden.
		bool mApplyMetricFormatting;	///< Indicates whether to apply FormatFill from Metric definition or not.

		bool mIsGrayoutMode; ///< specify whether the graph is in gray out mode
        bool mIsDuplicated; ///< specify whether last selected data point is duplicated

		AxesState* mpAxesState;
		DatasetState* mpDatasetState;
		UniformAxisContext* mpUniformAxisContext;
		GraphMatrixContext*	mpGraphMatrixContext;

        // Very Large chart, we draw a small view cover current screen, this offset is current screen view to origin chart.
        Int32 mChartXOffset;
        Int32 mChartYOffset;

        // Used in time series chart, indicating which part of chart will be rendered.
        Int32 mChartIntegrity;

		ChartAdvFormat* mpAxisAutoNumberFormat;	///< Maintains the automatic number format of value axis.
        std::map<std::vector<Int32>, ChartAdvFormat*> mGMAxisAutoNumberFormatSet; ///< Maintains the automatic number format of value axis in graph matrix.
        std::map<TripleId, ChartAdvFormat*> mAxisAutoNumberFormatSet;

		ThresholdCache mThresholdCache;

		std::vector<MetricFormatFillCache> mMetricFormatFillCache;	///< Metric index -> MetricFormatFillCache.

		std::string mTemplateNodeKey;

		FormatManager::Ptr mFormatManagerPtr;

		EnumLineJoinType mLineJoinType;

        ChartCanvas  * mpCanvas;

    private:
		/**
         This is a helper method of UpdateFormatFill(). This method is used to apply enhanced series effect.
         @iopFormatFill Points to a FormatFill object.
         @iCollectionType Indicates the collection type of format owner.
         */
		void hApplyEnhancedSeriesEffect(FormatFill* iopFormatFill, Int32 iCollectionType) const;

		/**
         A wrapper of cairo_move_to().
         @param iX,iY Specifies a point.
         @parma iIsLinePath Indicates whether current pathe is a line path or not.
         */
		void hMoveTo(double iX, double iY, bool iIsLinePath, bool iIsCurveAnimation = false, FPoint2D* iBasePoint = NULL);
		/**
         A wrapper of cairo_line_to().
         @param iX,iY Specifies a point.
         @parma iIsLinePath Indicates whether current pathe is a line path or not.
         */
		void hLineTo(double iX, double iY, bool iIsLinePath, bool iIsCurveAnimation = false, FPoint2D* iBasePoint = NULL);

		/// Release mpStreamBuffer;
		void hReleaseStreamBuffer();

		void hParseValueContent(std::wfstream& irFile, const CComVariant& irValue);

		void hParseParrayContent(std::wfstream& irFile, const CComVariant& irValue);

		void hFinalizeVectorGraphSurface();
		Int32 hParseValueToFormat(const MBase::String& irFormatString, ChartAdvFormat& orFormat);

#ifndef _VE_CE
		/// When mpCGContext is NULL, we use a private CGContext to get	string width.
		CGContext* mpPrivateCGContext;

		//	FontKey mPublicFontKey;		///< Maintains current font key(mpCGContext). This is used by GetStringExtent() method.
        //	FontKey mPrivateFontKey;	///< Maintains current font key(mpPrivateCairo). This is used by GetStringExtent() method.
#endif
		LayerObjectPtr mpHighlightLayerObjectPtr;

        //Keep a record of the current highlighted graph objects.
        std::vector<TripleId> mHighLightedObjectList;

        std::map<TripleId, bool> mTripleIdHoverMap;

        Rect2D mGraphFrame;

		ChartContextHelper *mpChartContextHelper;
	};

    inline Rect2D ChartContext::GetGraphFrame()
    {
        return mGraphFrame;
    }
	inline Int32 ChartContext::VDWidth(Int32 iVirtualWidth) const
	{
		return static_cast<Int32>(static_cast<double>(iVirtualWidth * mGraphWidth) / gVirtualCoordinateWidth + 0.5);
	}

	inline Int32 ChartContext::VDHeight(Int32 iVirtualHeight) const
	{
		Int32 lGraphHeight = mGraphHeight;
		if (mIsUniformAxisMode && !mIsEntireChart)
		{
			_ASSERTE(mpUniformAxisContext != NULL);
			lGraphHeight = mpUniformAxisContext->mGraphHeight;
		}
		return static_cast<Int32>(static_cast<double>(iVirtualHeight * lGraphHeight) / gVirtualCoordinateHeight + 0.5);
	}

	inline Int32 ChartContext::DVWidth(Int32 iImageWidth) const
	{
		return static_cast<Int32>(static_cast<double>(iImageWidth * gVirtualCoordinateWidth) / mGraphWidth + 0.5);
	}

	inline Int32 ChartContext::DVHeight(Int32 iImageHeight) const
	{
		return static_cast<Int32>(static_cast<double>(iImageHeight * gVirtualCoordinateHeight) / mGraphHeight + 0.5);
	}

	inline void ChartContext::SetLineWidthOdd(bool iIsLineWidthOdd)
	{
		mIsLineWidthOdd = iIsLineWidthOdd;
	}
	inline bool ChartContext::NeedAdjustedLinePath() const
	{
		return mIsLineWidthOdd && (mImageFormat != PFT_PDF) && (mImageFormat != PFT_SVG);
	}

	inline bool ChartContext::IsVectorGraph() const
	{
		return (mImageFormat == PFT_PDF) || (mImageFormat == PFT_SVG);
	}

	inline void ChartContext::SetGraphSize(Int32 iGraphWidth, Int32 iGraphHeight)
	{
		mGraphWidth = iGraphWidth;
		mGraphHeight = iGraphHeight;
        
        //Init graph frame if not initilized, temp solution
        if (mGraphFrame.height == -1 && mGraphFrame.width == -1) {
            mGraphFrame.x = mGraphFrame.y = 0;
            mGraphFrame.height = iGraphHeight;
            mGraphFrame.width = iGraphWidth;
        }
	}

	inline Int32 ChartContext::GetGraphWidth() const
	{
		return mGraphWidth;
	}

	inline Int32 ChartContext::GetGraphHeight() const
	{
		return mGraphHeight;
	}

	inline void ChartContext::SetManualLayout(bool iManualLayout)
	{
		mManualLayoutMode = iManualLayout;
	}

	inline bool ChartContext::IsManualLayout() const
	{
		return mManualLayoutMode;
	}

	inline void ChartContext::SetRecalculateFrame(bool iRecalculate)
	{
		mRecalculateFrame = iRecalculate;
	}

	inline bool ChartContext::IsRecalculateFrame() const
	{
		return mRecalculateFrame;
	}

	inline void ChartContext::SetAbsoluteFontLineSize(bool iAbsolute)
	{
		mAbsoluteFontLineSize = iAbsolute;
	}

	inline bool ChartContext::IsAbsoluteFontLineSize() const
	{
		return mAbsoluteFontLineSize;
	}

	inline double ChartContext::Square(double iNumber) const
	{
		return iNumber * iNumber;
	}
	inline Int32 ChartContext::GetGraphType() const
	{
		return mGraphType;
	}

	inline Int32 ChartContext::DialogSizeToPixelSize(Int32 iDialogSize) const
	{
		return iDialogSize * std::min(mGraphWidth, mGraphHeight) / 2000;
	}
	inline void ChartContext::SetDataset(Dataset::Ptr iDatasetPtr)
	{
		mDatasetPtr = iDatasetPtr;
	}
	inline Dataset::Ptr ChartContext::GetDataset()
	{
		return mDatasetPtr;
	}


	inline void ChartContext::SetChartComponent(EnumChartComponent iChartComponent)
	{
		mChartComponent = iChartComponent;
	}

	inline EnumChartComponent ChartContext::GetChartComponent() const
	{
		return mChartComponent;
	}

	template<class T>
	void ChartContext::DrawPolygon(const std::vector< Point<T> >& irPoints, bool iIsLinePath, bool iIsCurveAnimation, FPoint2D* iBasePoint, unsigned char iLineStyle)
	{
		Int32 lNumberOfPoints = (Int32)irPoints.size();
		if(lNumberOfPoints < 3)
			return;

		NewPath();
		if (iIsLinePath && NeedAdjustedLinePath())
		{
			std::vector<FPoint2D> lAdjustedPoints;
			for(Int32 i = 0; i < lNumberOfPoints; ++i)
			{
				lAdjustedPoints.push_back(irPoints[i]);
			}

			Int32 lPreviousIndex = lNumberOfPoints - 1;
			for(Int32 i = 0; i < lNumberOfPoints; ++i)
			{
				Int32 lNextIndex = (i + 1 == lNumberOfPoints) ? 0 : i + 1;
				if (irPoints[i].x == irPoints[lPreviousIndex].x || irPoints[i].x == irPoints[lNextIndex].x)
				{
					lAdjustedPoints[i].x += 0.5f;
				}
				if (irPoints[i].y == irPoints[lPreviousIndex].y || irPoints[i].y == irPoints[lNextIndex].y)
				{
					lAdjustedPoints[i].y += 0.5f;
				}
				lPreviousIndex = i;
			}
			DrawPolyLine(lAdjustedPoints, false, iIsCurveAnimation, iBasePoint, iLineStyle);
		}
		else
		{
			DrawPolyLine(irPoints, false, iIsCurveAnimation, iBasePoint, iLineStyle);
		}
		ClosePath();
	}

	template<class T>
	void ChartContext::DrawStraightLine(const Point<T>& irStartPoint, const Point<T>& irEndPoint)
	{
		NewPath();
		hMoveTo(irStartPoint.x, irStartPoint.y, true);
		hLineTo(irEndPoint.x, irEndPoint.y, true);
	}

	template<class T>
	void ChartContext::DrawPolyLine(const std::vector< Point<T> >& irPoints, bool iIsLinePath, bool iIsCurveAnimation, FPoint2D* iBasePoint, unsigned char iLineStyle)
	{
		Int32 lNumberOfPoints = (Int32)irPoints.size();
		if(lNumberOfPoints<=0)
			return;
		NewPath();
		hMoveTo(irPoints[0].x, irPoints[0].y, iIsLinePath, iIsCurveAnimation, iBasePoint);
		Int32 lPreviousIndex = 0;
		for(Int32 i = 1; i < lNumberOfPoints; ++i)
		{
			// If the current point is not the last point and is on a vertical or horizontal segement formed by the prevous and the next point,
			// do not put it into the path.
			if ((i + 1 == lNumberOfPoints) || irPoints[i].IsOnVerOrHorSegment(irPoints[lPreviousIndex], irPoints[i + 1]) == false)
			{
				hLineTo(irPoints[i].x, irPoints[i].y, iIsLinePath, iIsCurveAnimation, iBasePoint);
				lPreviousIndex = i;
			}
		}
#ifdef _VE_CE
		if(!iIsLinePath)
		{
			if(irPoints[0].IsOnVerOrHorSegment(irPoints[lPreviousIndex], irPoints[1]) == false)
			{
				hLineTo(irPoints[0].x, irPoints[0].y, iIsLinePath, iIsCurveAnimation, iBasePoint);
			}
		}
#endif
	}

	template<class T>
	void ChartContext::ApplyDonutToRing(const Point<T>& irCenter, T iOuterRadius, T iInnerRadius, const FormatFill::Ptr iFormatFillPtr, EnumRadientGradientUsage iUsage, bool iHasExtraPara, int iExtraDepth, int iExtraIntensity)
	{
#ifdef _VE_CE
		double lCX = irCenter.x;
		double lCY = irCenter.y;

		double lMiddleRadius = iInnerRadius + (1.0 - (1.0 - iFormatFillPtr->mFillBevel.mDepth * 0.01)) * (iOuterRadius - iInnerRadius);

		SaveState();
		Clip();

		const short lMargin = 1;
		if (iUsage != RG_BORDERRING)
		{
			FillRadientGradient(iFormatFillPtr.Get(), RG_RING, lCX, lCY, iInnerRadius > lMargin ? iInnerRadius - lMargin : 0, lMiddleRadius);
			FillRadientGradient(iFormatFillPtr.Get(), RG_RING, lCX, lCY, iOuterRadius + lMargin, lMiddleRadius);
		}
		else
		{
			FillRadientGradient(iFormatFillPtr.Get(), RG_BORDERRING, lCX, lCY, iInnerRadius > lMargin ? iInnerRadius - lMargin : 0, iOuterRadius + lMargin, iHasExtraPara, iExtraDepth, iExtraIntensity);
		}

		RestoreState();
#else
        //	CGColorSpaceRef lColorspace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
		CGColorSpaceRef lColorspace = CGColorSpaceCreateDeviceRGB();
		double lMiddleRadius = iInnerRadius + (1.0 - (1.0 - iFormatFillPtr->mFillBevel.mDepth * 0.01)) * (iOuterRadius - iInnerRadius);

		size_t lNumOfLocations = 2;
		CGFloat lLocations[2] = { 0.0, 1.0 };
		float lTransparency = iFormatFillPtr->mFillBevel.mIntensity * 0.01f * iFormatFillPtr->mAlpha / 255.0f;
		CGFloat lComponents[8] = { 0.0, 0.0, 0.0, lTransparency,
			1.0, 1.0, 1.0, lTransparency
		};
		CGGradientRef lGradient = CGGradientCreateWithColorComponents (lColorspace, lComponents, lLocations, lNumOfLocations);

		CGPoint lCenter = CGPointMake(irCenter.x, irCenter.y);

		SaveState();
		Clip();

		// 429813 Add some margin at boundary to make the boundary look smoother.
		const Int32 lMargin = 1;
		CGContextDrawRadialGradient(mpCGContext, lGradient, lCenter,
									iInnerRadius > lMargin ? iInnerRadius - lMargin : 0,
									lCenter, lMiddleRadius, 0);
		CGContextDrawRadialGradient(mpCGContext, lGradient, lCenter,
									iOuterRadius + lMargin,
									lCenter, lMiddleRadius, 0);
		RestoreState();

		CGGradientRelease(lGradient);
		CGColorSpaceRelease(lColorspace);
#endif
	}
	inline bool ChartContext::IsParetoChart() const
	{
		return mGraphType == DssGraphTypePareto || mGraphType == DssGraphTypePareto_Percent;
	}
	inline void ChartContext::SetImageFormat(Int32 iFormat)
	{
		mImageFormat = iFormat;
	}
	inline Int32 ChartContext::GetImageFormat() const
	{
		return mImageFormat;
	}
	inline void ChartContext::SetPushGroup(bool iPushGroupFlag)
	{
		mPushGroupFlag = iPushGroupFlag;
	}
	inline bool ChartContext::GetPushGroup() const
	{
		return mPushGroupFlag;
	}
	inline void ChartContext::SetValueAxisShown(bool iIsValueAxisShown)
	{
		mIsValueAxisShown = iIsValueAxisShown;
	}
	inline bool ChartContext::IsValueAxisShown() const
	{
		return mIsValueAxisShown;
	}

	inline void ChartContext::EnableAnimation(bool iUseAnimation)
	{
		mUseAnimation = iUseAnimation;
	}

    inline void ChartContext::DisableAnimationForZoom(bool iDisableAnimation)
    {
        mUseAnimationForZoom = !iDisableAnimation;
    }
	inline bool ChartContext::IsUseAnimation() const
    {
        return mUseAnimation;
    }
    inline bool ChartContext::IsAnimationDisabledForZoom()
    {
        return !mUseAnimationForZoom;
    }

	inline void ChartContext::SetAnimationDuration(double iSeconds)
	{
		mAnimationDuration = iSeconds;
	}
	inline double ChartContext::GetAnimationDuration() const
	{
		return mAnimationDuration;
	}
    inline void ChartContext::EnterGrayoutMode()
    {
        mIsGrayoutMode = true;
    }
    inline void ChartContext::ExitGrayoutMode()
    {
        mIsGrayoutMode = false;
    }
    inline bool ChartContext::IsGrayoutMode() const
    {
        return mIsGrayoutMode;
    }
    inline void ChartContext::EnterDuplicateMode()
    {
        mIsDuplicated = true;
    }
    inline void ChartContext::ExitDuplicateMode()
    {
        mIsDuplicated = false;
    }
    inline bool ChartContext::IsDuplicateMode() const
    {
        return mIsDuplicated;
    }
	inline std::string ChartContext::GetTemplateNodeKey() const
	{
		return mTemplateNodeKey;
	}

	inline void ChartContext::SetFormatManager(FormatManager::Ptr iFormatManagerPtr)
	{
		mFormatManagerPtr = iFormatManagerPtr;
	}
	inline FormatManager::Ptr ChartContext::GetFormatManager() const
	{
		return mFormatManagerPtr;
	}

    inline void ChartContext::SetIntegrity(Int32 iIntegrity)
    {
        mChartIntegrity = iIntegrity;
    }
    inline Int32 ChartContext::GetIntegrity()
    {
        return mChartIntegrity;
    }

	inline void ChartContext::SetTimeSeriesChart(bool iIsTimeSeries)
	{
		mIsTimeSeriesChart = iIsTimeSeries;
		if (mIsTimeSeriesChart)
		{
			mGraphType = DssGraphTypeVertical_Line_Absolute;
		}
	}
	inline bool ChartContext::IsTimeSeriesChart() const
	{
		return mIsTimeSeriesChart;
	}

	inline void ChartContext::EnableMicroChart(bool iIsMicroChart)
	{
		mIsMicroChart = iIsMicroChart;
	}
	inline bool ChartContext::IsMicroChart() const
	{
		return mIsMicroChart;
	}

	inline void ChartContext::EnableVoiceOver(bool iIsVoiceOverMode)
	{
		mIsVoiceOver = iIsVoiceOverMode;
	}

	inline bool ChartContext::IsVoiceOver() const
	{
		return mIsVoiceOver;
	}

	inline void ChartContext::EnableGraphMatrix(bool iIsGraphMatrix)
	{
		mIsGraphMatrix = iIsGraphMatrix;
	}

	inline bool ChartContext::IsGraphMatrix() const
	{
		return mIsGraphMatrix;
	}

    inline void ChartContext::EnableVIDocument(bool iIsVIDocument)
    {
        mIsVIDocument = iIsVIDocument;
    }

    inline bool ChartContext::IsVIDocument() const
    {
        return mIsVIDocument;
    }

    inline void ChartContext::SetNeedColorColOffset(bool iIsNeedColorColOffset)
	{
		mIsNeedColorColOffset = iIsNeedColorColOffset;
	}

	inline void ChartContext::SetImageUsage(Int32 iImageUsage)
	{
		mImageUsage = iImageUsage;
	}
	inline Int32 ChartContext::GetImageUsage() const
	{
		return mImageUsage;
	}
	inline void ChartContext::EnableUniformAxis(bool iUseUniformAxis)
	{
		mIsUniformAxisMode = iUseUniformAxis;
	}
	inline bool ChartContext::IsUniformAxisMode() const
	{
		return mIsUniformAxisMode;
	}
	inline void ChartContext::SetEntireness(bool iIsEntireChart)
	{
		mIsEntireChart = iIsEntireChart;
	}
	inline bool ChartContext::IsEntireChart() const
	{
		return mIsEntireChart;
	}
	inline bool ChartContext::IsFontAntiAlias() const
	{
		return mIsFontAntiAlias;
	}
	inline void ChartContext::SetAdvancedChartTypeSupport(bool iAdvancedChartTypeSupport)
	{
		mAdvancedChartTypeSupport = iAdvancedChartTypeSupport;
	}
	inline bool ChartContext::HasAdvancedChartTypeSupport() const
	{
		return mAdvancedChartTypeSupport;
	}
#ifndef _VE_CE
	inline void ChartContext::SetLineJoin(EnumLineJoinType iLineJoinType)
	{
		mLineJoinType = iLineJoinType;
		switch (mLineJoinType) {
			case LJT_MITER:
				CGContextSetLineJoin(mpCGContext, kCGLineJoinMiter);
				break;
			case LJT_BEVEL:
				CGContextSetLineJoin(mpCGContext, kCGLineJoinBevel);
				break;
			case LJT_ROUND:
				CGContextSetLineJoin(mpCGContext, kCGLineJoinRound);
				break;
			default:
				_ASSERTE(false);
				break;
		}
	}
#endif
	inline EnumLineJoinType ChartContext::GetLineJoin()
	{
		return mLineJoinType;
	}
	inline void ChartContext::SetFontScale(double iRatio)
	{
		_ASSERTE(iRatio > 0);
		mFontScaleRatio = iRatio;
	}
	inline double ChartContext::GetFontScale() const
	{
		if (mAbsoluteFontLineSize)
			return mFontScaleRatio; //#468456
		else
			return 1.0;			    //#476838
	}

	inline void ChartContext::SetHasCurvePolygon(bool iHasCurvePolygon)
	{
		mHasCurvePolygon= iHasCurvePolygon;
	}
	inline bool ChartContext::HasCurvePolygon() const
	{
		return mHasCurvePolygon;
	}

    inline void ChartContext::ResetHighlightObjectList(std::vector<TripleId>* ipInputList)
    {
        mHighLightedObjectList.clear();
        if (ipInputList)
        {
            mHighLightedObjectList = *ipInputList;
        }
    }
    inline void ChartContext::GetHighLightObjectList(std::vector<TripleId>& iorCurrentList)
    {
        iorCurrentList = mHighLightedObjectList;
    }

	inline void ChartContext::SetBackgroundOpaque(bool iBackgroundOpaque)
	{
		mIsBackgroundOpaque = iBackgroundOpaque;
	}
	inline bool ChartContext::IsBackgroundOpaque() const
	{
		return mIsBackgroundOpaque;
	}

    inline void ChartContext::HideAxisElements(bool iHide)
    {
        mShouldHideAxisElements = iHide;
    }
    inline bool ChartContext::IsHidingAxisElements() const
    {
        return mShouldHideAxisElements;
    }
	inline void ChartContext::SetApplyMetricFormatting(bool iApplyMetricFormatting)
	{
		mApplyMetricFormatting = iApplyMetricFormatting;
	}
	inline bool ChartContext::GetApplyMetricFormatting() const
	{
		return mApplyMetricFormatting;
	}
    inline bool ChartContext::IsAxesStateNULL() const
    {
        if (mpAxesState) {
            return false;
        }
        else
            return true;
    }
    inline void ChartContext::SetCanvas(ChartCanvas *ipCanvas)
    {
        if(mpCanvas != NULL)
        {
            delete mpCanvas;
            mpCanvas = NULL;
        }
        mpCanvas = ipCanvas;
    }
    inline ChartCanvas* ChartContext::GetCanvas()
    {
        return mpCanvas;
    }
}
#endif
