//====================================================================
//	Filename:		Kernel/SourceCode/ChartEngine/PrivateSource/HistogramPlot.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	2/13/09
//	Copyright (C) MicroStrategy, Inc. 2009
//==============================================================================================

#ifndef MSICHART_HISTOGRAMPLOT_H
#define	MSICHART_HISTOGRAMPLOT_H

#include "Plot.h"
#include "ValueAxis.h"
#include "CategoryPlot.h"
#include "NumberObject.h"

namespace MsiChart
{
	struct HistogramOptions
	{
		HistogramOptions()
			: mGraphType(DssGraphTypeVertical_Histogram) 
			, mOrientation(OR_VERTICAL)
			, mUseDepth(false)
			, mDepth(200)
			, mDepthDirection(45)
			, mShowDataLabel(false)
			, mGapWidth(40)
			, mTextPosition(DssGraphDTPositionOutMax)
		{}
		EnumDSSGraphType		mGraphType;
		EnumOrientation			mOrientation;
		Int32					mGapWidth;	///< Gap width between groups
		bool					mUseDepth;
		Int32					mDepth;
		Int32					mDepthDirection;
		bool					mShowDataLabel;
	//	bool					mShowValue;		
		EnumDSSGraphDataTextPosition		mTextPosition;
	//	EnumDSSGraphDataTextShowType		mTextShowType;
		ChartAdvFormat						mGraphNumberFormat;
		GraphTrendLineOptions				mTrendLineOptions;
	};

	/*
	struct HistogramSeriesOptions
	{
		typedef MSynch::SmartPtr<HistogramSeriesOptions> Ptr;

		SeriesOptions() : mShowDataLabels(false)
		{}

		bool						mShowDataLabels;	
		GraphTrendLineOptions		mTrendLineOptions;
		ChartAdvFormat				mGraphNumberFormat;
	};
	

	struct HistogramPlotContext
	{
		HistogramOptions mHistogramOptions;

		ValueAxis::Ptr mXValueAxis;
		ValueAxis::Ptr mY1ValueAxis;
	};
	*/
	/**
	This class is used to calculate the histogram data based on Dataset and generate bar risers and labels for a histogram chart.
	*/
	class HistogramPlot: public Plot
	{
	public:
		typedef MSynch::SmartPtr<HistogramPlot> Ptr;

		HistogramPlot(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irDataArea);
		~HistogramPlot();

		//Re-implemention the virtual functions inherited from GraphObjectManager
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;
		virtual void Draw();
		virtual void GenerateView();
		virtual void GenerateImageMap(ImageMap& iorImageMap); 
		virtual void GenerateMapAndList(); 
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;

		virtual bool GetToolTipData(Int32 iSeriesID, Int32 iGroupID, double& orValue, double& orPercentValue);
		virtual bool GetToolTipData(Int32 iSeriesID, Int32 iGroupID, double& orValue, double& orPercentValue, Int32 iOffset);

		/**
		Get the min & max value of the data in this plot for X value axis.
		It is used to set range of X ValueAxis.
		@param iorMin The output minimum value.
		@param iorMax The output maximum value.
		@param iorIsInitialized Indicates whether iorMin and iorMax are initialized or not.
		*/
		void GetXMinMaxValue(double& iorMin, double& iorMax, bool& iorIsInitialized);

		/**
		Get the min & max value of the data in this plot for Y1 value axis.
		It is used to set range of Y1 ValueAxis.
		@param iorMax The output maximum value.
		@param iorIsInitialized Indicates whether iorMin and iorMax are initialized or not.
		*/
		void GetY1MinMaxValue(double& iorMin, double& iorMax, bool& iorIsInitialized);

		/**
		Set the x value axis for the plot
		There can be only one x axis for a histogram plot currently.
		@param iValueAxisPtr The input pointer to x value axis. 
		*/
		void SetXValueAxis(ValueAxis::Ptr iValueAxisPtr);

		/**
		Set the Y1 value axis for the plot
		There can be only one Y1 axis for a histogram plot currently.
		@param iValueAxisPtr The input pointer to Y1 value axis. 
		*/
		void SetY1ValueAxis(ValueAxis::Ptr iValueAxisPtr);

		/**
		It checks whether the drawing order of each group need to be reversed.
		@return true means the drawing order is from small Group ID to large Group ID.
		false means the drawing order is from large Group ID to small Group ID.
		*/
		bool IsReverseGroupOrder();

	protected:
		virtual void SetGraphType(EnumDSSGraphType iType);

	private:
		/**
		Calculate the histogram data according to the X value axis and dataset.
		*/
		void hCalculateHistogramData();

		/**
		Load plot level properties.
		*/	
		void hLoadProperties();

		/**
		Load trendlines properties.
		*/
		void hLoadTrendlineProperties();

		/**
		Generate one riser and its corresponding data label if any and put them into mGraphObjectList and mAreaCollection.
		@param iGroupId Specifies the group id.
		*/
		void hGenerateOneRiser(Int32 iGroupId);

		/**
		Calculate the riser's start position and size.
		@param iGroupId Specifies the group id.
		@param orStart Output the riser's start position.
		@param orSize Output the riser's size.
		*/
		void hGetRiserStartPosAndSize(Int32 iGroupId, Int32& orStart, Int32& orSize);

		/**
		Generate one data label.
		@param iGroupId Specifies the group id.
		@param irValue The value of the data label.
		@param iDataLabelCollectionPtr The data labels collection.
		@return A text object delegating the data label.
		*/
		TextObject::Ptr hCreateDataLabel(Int32 iGroupId, const double& irValue, GraphCollectionObject::Ptr iDataLabelCollectionPtr);

		/**
		Generate the trendlines.
		*/
		void hGenerateTrendline();

		//private members
		std::vector<double> mHistogramData;		///< Holds the calculated histogram data.
		std::vector<double> mHistogramDataX;	///< Holds the X values of the data points, used to generate the trendlines
		ValueAxis::Ptr mXValueAxisPtr;			///< X value axis.
		ValueAxis::Ptr mY1ValueAxisPtr;			///< Y1 value axis.
		HistogramOptions mHistogramOptions;		///< The histogram options.

		GraphCollectionObject::Ptr mRiserCollectionPtr;		///< The risers collection.
		GraphCollectionObject::Ptr mDataLabelCollectionPtr;	///< The data labels collection.
	};
}

#endif