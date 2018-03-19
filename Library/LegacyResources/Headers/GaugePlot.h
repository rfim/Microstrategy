//====================================================================
//	Filename:		Kernel/SourceCode/ChartEngine/PrivateSource/GaugePlot.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	3/03/09
//	Copyright (C) MicroStrategy, Inc. 2009
//==============================================================================================

#ifndef MSICHART_GAUGEPLOT_H
#define	MSICHART_GAUGEPLOT_H

#include "PDCHeader/PDCvector"

#include "Plot.h"
#include "GaugeValueAxis.h"
#include "NumberObject.h"

namespace MsiChart
{
	struct GaugeSeriesOptions
	{
		typedef MSynch::SmartPtr<GaugeSeriesOptions> Ptr;

		GaugeSeriesOptions()
			: mMarkerShape(0)
			, mMarkerSize(34)
		{}
		
		unsigned char	mMarkerShape;
		Int32			mMarkerSize;
	};

	struct GaugeBandOptions
	{
		typedef MSynch::SmartPtr<GaugeBandOptions> Ptr;

		GaugeBandOptions(Int32 iBandId = 0, double iBandRangeMin = 0.0, double iBandRangeMax = 0.0)
			: mBandId(iBandId)
			, mBandRangeMin(iBandRangeMin)
			, mBandRangeMax(iBandRangeMax)
		{}

		Int32 mBandId;
		double mBandRangeMin;
		double mBandRangeMax;
	};

	struct GaugeOptions
	{
		GaugeOptions()
			: mStartAngle(325)
			, mStopAngle(215)
			, mCircularThickness(30)
			, mGaugesPerRow(2)
			, mBorderThickness(30)
			, mShowDataLabels(true)
			, mShowGaugeTitles(true)
			, mShowBeveledFrame(false)
			, mShowSecondaryValuesAsMarkers(true)
			, mDrawAxisLabelOutside(false)
			, mDrawCategoriesInReverseOrder(false)
			, mIsClipped(false)
			, mNeedleStyle(DssGraphGaugeNeedleStyleSimpleWithBase)
			, mBorderStyle(DssGraphGaugeBorderStyleSimple)
		{}

		Int32					mStartAngle;
		Int32					mStopAngle;
		Int32					mCircularThickness;
		Int32					mGaugesPerRow;
		Int32					mBorderThickness;
		bool					mShowDataLabels;
		bool					mShowGaugeTitles;
		bool					mShowBeveledFrame;
		bool					mShowSecondaryValuesAsMarkers;
		bool					mDrawAxisLabelOutside;
		bool					mDrawCategoriesInReverseOrder;
		bool					mIsClipped;
		EnumDSSGraphGaugeNeedleStyle	mNeedleStyle;
		EnumDSSGraphGaugeBorderStyle	mBorderStyle;
		ChartAdvFormat			mGraphNumberFormat;
		std::vector<GaugeBandOptions>		mShownBands;
	};


	class GaugePlot: public Plot
	{
	public:
		typedef MSynch::SmartPtr<GaugePlot> Ptr;

		GaugePlot(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irDataArea);
		~GaugePlot();

		//Re-implemention the virtual functions inherited from GraphObjectManager
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;
		virtual void Draw();
		virtual void GenerateView();
		virtual void GenerateImageMap(ImageMap& iorImageMap); 
		virtual void GenerateMapAndList(); 
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;
#ifndef _VE_CE
		virtual void GenerateInterpolationFrame(double iProgress, LayerObject* ipLayerObject);
#else
		virtual void GenerateInterpolationFrame(double iProgress);
#endif

	private:
		/**
		Load plot level properties.
		*/	
		void hLoadProperties();

		/**
		Load series level properties.
		@param iInfoIndex The index for mLineSeiresOptions.
		*/
		void hLoadSeriesProperties(Int32 iSeriesId);

		/**
		Generate one gauge and its corresponding data label if any and put them into mGraphObjectList and mAreaCollection.
		@param iGroupId Specifies the group id.
		*/
		void hGenerateOneGauge(Int32 iGroupId, const Point2D& irBasePoint, Int32 iNeedleSize);

		/**
		Get the min & max value of the data in this plot for gauge value axis.
		It is used to set range of GaugeValueAxis.
		@param iorMax The output maximum value.
		*/
		void hGetMinMaxValue(double& orMin, double& orMax);

		/**
		*/
		void hGenerateGaugeValueAxis(Int32 iRadius);

		/**
		*/
		void hCalculateSubFrameAreas(Int32 iGroupCount, std::vector<Rect2D>& orSubFrameAreas);

		void hGenerateSubFrames(const std::vector<Rect2D>& irSubFrameAreas);

		/**
		*/
		void hGenerateGroupTitleAndLabels(const std::vector<Rect2D>& irSubFrameAreas, std::vector<Point2D>& orBasePoints, Int32& orBorderRadius);

		/**
		*/
		void hCalcualteBorderRadiusAndBasePoint(const Rect2D& irSubFrameArea, Int32 iGroupTitieHeight, Int32 iGroupLabelHeight,
												Point2D& orBasePoint, Int32& orBorderRadius);

		/**
		*/
		void hGenerateOneGroupTitle(Int32 iGroupId);

		/**
		*/
		void hGenerateOneGroupDataLabel(Int32 iGroupId);

		/**
		*/
		Int32 hCalculateBorderThicknessInPixel(Int32 iRadius);

		Int32 hCalculateBorderRangeThicknessInPixel(Int32 iRadius);

		void hGenerateBands(Int32 iGroupId, const Point2D& irBasePoint, Int32 iRadius);

		bool hCalculateActualBandRange(const GaugeBandOptions &irGaugeBandOptions, double &orBandRangeMin, double &orBandRangeMax);

		Int32 hFindBandOptionIndexByBandId(Int32 iBandId) const;
	
		void hCalculateFinalAngles();
		
		bool hGetAngle(Int32 iGroupId, Int32 iSeriesId, float& orAngle);
		
		/**
		 Check whether current frame is the first frame in animation;
		 */
		bool hIsFirstFrame() const;
		
		Int32 hGetNeedleSize();				
		
		void hGenerateLabelsAndNeedleBase(Int32 iGroupId, const Point2D& irBasePoint, Int32 iNeedleSize);
		

		//private members
		GaugeValueAxis::Ptr mGaugeValueAxisPtr;
		GaugeOptions mGaugeOptions;
		std::vector<GaugeSeriesOptions::Ptr> mGaugeSeriesOptions;

		GraphCollectionObject::Ptr mGroupTitleCollectionPtr;
		GraphCollectionObject::Ptr mDataLabelCollectionPtr;
		GraphCollectionObject::Ptr mGaugeBorderCollectionPtr;
		GraphCollectionObject::Ptr mGaugePanelCollectionPtr;
		GraphCollectionObject::Ptr mNeedleBaseCollectionPtr;
		std::vector<GraphCollectionObject::Ptr> mGaugeSeriesCollections;
		std::vector<GraphCollectionObject::Ptr> mBandCollections;
		
		std::vector<Point2D> mBasePoints;
		Int32 mGroupCount;
		Int32 mBorderInnerRadius;
		std::vector<std::vector<float> > mVecVecAngles; ///< Maintains a vector of vector for data angles.
		std::vector<std::vector<bool> > mVecVecIsAngleValid; ///< Maintains a vector of vector for bool values.
		
		///< Maintains a list of GraphObject which need to be drawn after animation layer.
		std::vector<GraphObject::Ptr> mPostGraphObjectList;	
		
		GaugePlot* mpOldGaugePlot;
		float mStartAngleInRad;
		float mStopAngleInRad;

	};
}

#endif