//==============================================================================================
//	FILENAME	:	PolarPlot.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2009-02-16
//	Copyright (C) MicroStrategy Incorporated 2009
//	All Rights Reserved
//==============================================================================================

#ifndef MsiChart_PolarPlot_h
#define MsiChart_PolarPlot_h

#include "PDCHeader/PDCvector"

#include "NumberObject.h"
#include "Plot.h"
#include "PolarValueAxis.h"

namespace MsiChart
{
	struct PolarSeriesInfo
	{
		PolarSeriesInfo()
			: mValueAxisIndex(VA_Y1),			  
			  mShowDataLabels(true)			 
		{
		}
		EnumValueAxis mValueAxisIndex;			///< Value axis index - Y1 or Y2				
		bool  mShowDataLabels;					///< Show data labels or not	
		GraphTrendLineOptions mTrendLineOptions;///< Trendline options.		
		ChartAdvFormat mNumberFormat;			///< Number format of data labels.
	};

	///< This class is used to generate polar chart.
	class PolarPlot : public Plot
	{
	public:
		typedef MSynch::SmartPtr<PolarPlot> Ptr;

		/** Constructor
		@param irTripleId,ipManager,irDeviceRect Required by base class.
		@param iIsRadar Indicates whether current chart type is Radar or not.
		*/
		PolarPlot(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irDeviceRect, bool iIsRadar = false);
		
		//virtual functions from GraphObjectManager
		virtual void GenerateMapAndList();
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;
		virtual void Draw();
		virtual void GenerateView();
		
	protected:
		/** Generate axes.
		@return Return false if the space is not enough. Otherwise, return true.
		*/
		bool GenerateAxes();
		/**
		Generate data markers for one series.
		@param irPoints Specifies where to draw the markers.
		@param iSeriesIndex Specifies the series index.
		@param iMarkerShape Specifies the marker shape for this series.
		@param iMarkerPixelSize Specifies the marker size in pixels for this series.
		@param orMarkerRects Output the bounding boxes of the markers.
		*/
		void GenerateMarkersBySeries(const std::vector<Point2D>& irPoints, Int32 iSeriesIndex, 
			unsigned char iMarkerShape, Int32 iMarkerPixelSize, std::vector<Rect2D>& orMarkerRects);

		/**
		Generate data labels.
		@param irRects Specifies a rectangle where the corresponding marker is placed.
		@param iSeriesIndex Specifies the series index.
		*/
		void GenerateDatalabels(const std::vector<Rect2D>& irRects, Int32 iSeriesIndex);
		/**
		Generate data line collection for one series.
		@param iSeriesIndex Specifies the series index.
		@return Return the data line collection of current series. Return NULL if there's no data line for current series.
		*/
		GraphCollectionObject* GenerateSeriesDataLineCollection(Int32 iSeriesIndex);
		/**
		Generate data lines for one series.
		@param irPolarPoints Specifies the data positions.
		@param iSeriesIndex Specifies the series index.		
		@param ipCollection Specifies the data line collection of current series.
		*/
		void GenerateDataLinesBySeries(const std::vector<FPoint2D>& irPolarPoints, Int32 iSeriesIndex, GraphCollectionObject* ipCollection);

		/**
		Create one curve data line.
		@param irPolarPointSet Specifies the data points which need to be connected by one curve line.
		These data points should be described in polar coordinates.
		@param iSeriesIndex Specifies the series index.
		@param ipDataLineSet Specifies the collection for the data line.
		*/
		CurveLineObject::Ptr CreateOneCurveLine(const std::vector<FPoint2D>& irPolarPointSet, Int32 iSeriesIndex, GraphCollectionObject* ipDataLineSet);
		
		/// Create trend lines.
		void CreateTrendLines();
		/**
		Calculate data points for one series.
		@param iSeriesIndex Specifies the series index.
		@param orSeriesPolarPoints Output the data points in polar coordinates.
		@param iIsPreValue Indicates whether to get the previous cumulate data in curve Radar Stacked chart.
		@return Return true if all points in orSeriesPolarPoints are valid.
		*/
		bool CalculateSeriesPolarPoints(Int32 iSeriesIndex, std::vector<FPoint2D>& orSeriesPolarPoints, bool iIsPreValue = false) const;

		/**
		Parse series polar points into valid point groups.
		@param irPolarPonits Input all the polar points for current series.
		@orPointGroups Output valid point groups.
		*/
		void ParsePolarPoints(const std::vector<FPoint2D>& irPolarPoints, std::vector<std::vector<FPoint2D> >& orPointGroups) const;
		/**
		Append the first polar point and adjust the angle of the appended point by 2*PI.
		@param iorSeriesPolarPoints Specifies the vector of points which need to be extended. This vector should not be empty.
		*/
		void AppendFirstPolarPoint(std::vector<FPoint2D>& iorSeriesPolarPoints) const;
		/**
		Get formatted text using the number format of value axis.
		@param iSeriesIndex Specifies the series index.
		@param iGroupID Specifies the group ID.
		@param iIsXValue Indicates whether to get X value or Y value.
		@param iValue Indicates the data value.
		@param orText Output the string for the data value specified by the previous 3 parameters.
		@param ipCollection Specifies a collection which current label belongs to.
		*/
		bool GetVAFormattedText(Int32 iSeriesIndex, Int32 iGroupID, bool iIsXValue, double iValue, MBase::String& orText, GraphCollectionObject* ipCollection);		
		/**
		Get X-Y coordinates from polar coordinates.
		@param iAngleRad Specifies an angle in radian.
		@param iRadius Spcifies a radius.
		@return Return the corresponding X-Y coordinates.
		*/
		Point2D GetPointByAngleAndRadius(double iAngleRad, double iRadius) const;

		/**
		Get points in Cartesian coordinates from points in polar coordinates.
		@param irPolarPoints Specifies a vector of points in polar coordinates.
		@param orCartesianPoints Output a vector of points in Cartesian coordinates.
		*/
		void PolarToCartesian(const std::vector<FPoint2D>& irPolarPoints, std::vector<Point2D>& orCartesianPoints);

		void MergeGraphObjectList();
		/**
		Load plot level properties.
		*/
		void LoadProperties();		


		PolarInfo mPolarInfo;			///< Maintains polar information.

		PolarValueAxis::Ptr mXAxisPtr;	///< X Axis.
		PolarValueAxis::Ptr mY1AxisPtr;	///< Y1 Axis.
		PolarValueAxis::Ptr mY2AxisPtr;	///< Y2 Axis.

		std::vector<PolarSeriesInfo> mSeriesOptions;	///< Series options.			
		Int32 mGroupCount;				///< Number of groups.
		/**	This variable is only valid in Radar chart when mUseSpiderLines is false. 
		If mUseCircularDataLines is true, circular data lines are used. Otherwise, straight data lines are used. 
		In Polar chart, this data member should always be false.
		*/
		bool mUseCircularDataLines;	
		/** Indicates whether current chart is a stacked chart or not (for Radar).
		In Polar chart, this data member should always be false.
		*/
		bool mIsStacked;	
		/** Indicates whether current chart is area radar chart or not(for Radar). 
		In Polar chart, this data member should always be false.
		*/
		bool mIsAreaRadar;

		Int32 mLabelLocation;	///< Data label location
		Int32 mLabelFormat;		///< Data label format

	private:	
		/**
		Get the data values.
		In Radar stacked chart, you will get cumulated data.
		@param iSeriesIndex The index for mSeriesOptions.
		@param iGroupId The group ID.
		@param iIsXValue Indicates whether to get X value or Y value.
		@param iPreValue Indicates whether to get the previous cumulate data in Stacked layout. This is only used in stacked chart.
		@return true if successful, false if NULL in Dataset.
		*/
		virtual bool GetData(Int32 iSeriesIndex, Int32 iGroupId, bool iIsXValue, double& orValue) const;

		virtual PolarValueAxis::Ptr hGenerateXAxis();

		void hGenerateOneDataLine(const std::vector<FPoint2D>& irPolarPointSet, Int32 iSeriesIndex, GraphCollectionObject* ipDataLineSet);

		TextObject::Ptr hCreateDataLabel(Int32 iSeriesIndex, Int32 iGroupID, GraphCollectionObject* ipCollection);
		virtual bool hGetValueString(Int32 iSeriesIndex, Int32 iGroupID, MBase::String& orValue, GraphCollectionObject* ipCollection);
		/**
		Get the minimum and maximum value for value axis
		@param iValueAxis Specifies a value axis. VA_BOTH, VA_Y1, VA_Y2 indicate X, Y1, Y2 respectively.
		@param orMin: Minimum value for this axis
		@param orMax: Maximum value for this axis
		*/
		void hGetMinMaxValueForValueAxis(EnumValueAxis iValueAxis, double& orMin, double& orMax) const;	

		virtual void hMoveDataLabel(TextObject* ipText, const Rect2D& irRefRect, Int32 iSeriesIndex);		

		void hDrawTrendLinesAndEquations(bool iIsY2);		

		/**
		Load series level properties.
		@param iSeriesIndex The input series index. 
		*/
		void hLoadSeriesProperties(Int32 iSeriesIndex);

		void hCalculateTickAngles();	

		bool hIsAxisOnTop() const;

		bool mIsRadarChart;		
	};	

	inline bool PolarPlot::hIsAxisOnTop() const
	{
		return mIsRadarChart && (mIsAreaRadar || mIsStacked);
	}
}
#endif
